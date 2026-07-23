#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>
#include "GateWatchPins.h"

SX1262 radio = new Module(GateWatchPins::LORA_CS, GateWatchPins::LORA_DIO1,
                          GateWatchPins::LORA_RESET, GateWatchPins::LORA_BUSY);

constexpr uint32_t FENCE_DEBOUNCE_MS = 750;
constexpr uint32_t PIR_DEBOUNCE_MS = 200;
constexpr uint32_t PIR_WARMUP_MS = 30000;
constexpr uint32_t PIR_COOLDOWN_MS = 60000;
constexpr uint32_t HEARTBEAT_MS = 300000;

struct InputState {
    bool raw = false;
    bool stable = false;
    uint32_t changedAt = 0;
};

InputState fence;
InputState pir;
uint32_t bootAt = 0;
uint32_t lastPirAlertAt = 0;
uint32_t lastHeartbeatAt = 0;

bool updateInput(InputState &state, bool raw, uint32_t debounceMs, uint32_t now) {
    if (raw != state.raw) {
        state.raw = raw;
        state.changedAt = now;
    }
    if (state.stable != state.raw && now - state.changedAt >= debounceMs) {
        state.stable = state.raw;
        return true;
    }
    return false;
}

void sendEvent(const String &event) {
    String message = "GW-TEST | " + event;
    Serial.printf("TX: %s\n", message.c_str());
    int16_t state = radio.transmit(message);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println("TX complete");
    } else {
        Serial.printf("TX failed, RadioLib code %d\n", state);
    }
}

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\nGateWatch radio + sensor smoke test");

    pinMode(GateWatchPins::FENCE, INPUT_PULLUP);
    pinMode(GateWatchPins::PIR, INPUT_PULLDOWN);
    fence.raw = fence.stable = digitalRead(GateWatchPins::FENCE) == HIGH;
    pir.raw = pir.stable = false;

    SPI.begin(GateWatchPins::LORA_SCK, GateWatchPins::LORA_MISO,
              GateWatchPins::LORA_MOSI, GateWatchPins::LORA_CS);

    radio.setRfSwitchPins(GateWatchPins::LORA_RF_SW, RADIOLIB_NC);
    int16_t state = radio.begin(GateWatchRadio::FREQUENCY_MHZ,
                                GateWatchRadio::BANDWIDTH_KHZ,
                                GateWatchRadio::SPREADING_FACTOR,
                                GateWatchRadio::CODING_RATE,
                                RADIOLIB_SX126X_SYNC_WORD_PRIVATE,
                                GateWatchRadio::POWER_DBM,
                                GateWatchRadio::PREAMBLE,
                                GateWatchRadio::TCXO_VOLTAGE,
                                false);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.printf("SX1262 init failed, RadioLib code %d\n", state);
        while (true) delay(1000);
    }
    radio.setDio2AsRfSwitch(true);
    Serial.println("SX1262 initialized. Antenna must remain attached.");

    bootAt = millis();
    lastHeartbeatAt = bootAt;
    sendEvent(fence.stable ? "BOOT fence=BROKEN" : "BOOT fence=SECURE");
}

void loop() {
    const uint32_t now = millis();
    const bool rawFenceBroken = digitalRead(GateWatchPins::FENCE) == HIGH;
    if (updateInput(fence, rawFenceBroken, FENCE_DEBOUNCE_MS, now)) {
        sendEvent(fence.stable ? "FENCE BREAK" : "FENCE RESTORED");
    }

    if (now - bootAt >= PIR_WARMUP_MS) {
        const bool rawPir = digitalRead(GateWatchPins::PIR) == HIGH;
        if (updateInput(pir, rawPir, PIR_DEBOUNCE_MS, now) && pir.stable) {
            if (lastPirAlertAt == 0 || now - lastPirAlertAt >= PIR_COOLDOWN_MS) {
                lastPirAlertAt = now;
                sendEvent(fence.stable ? "MOTION NEAR BROKEN FENCE" : "MOTION DETECTED");
            }
        }
    }

    if (now - lastHeartbeatAt >= HEARTBEAT_MS) {
        lastHeartbeatAt = now;
        sendEvent(String("HEARTBEAT fence=") + (fence.stable ? "BROKEN" : "SECURE") +
                  " motion=" + (pir.stable ? "ACTIVE" : "IDLE"));
    }
    delay(20);
}
