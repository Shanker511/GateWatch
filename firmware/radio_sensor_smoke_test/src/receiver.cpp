#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>
#include "GateWatchPins.h"

SX1262 radio = new Module(GateWatchPins::LORA_CS, GateWatchPins::LORA_DIO1,
                          GateWatchPins::LORA_RESET, GateWatchPins::LORA_BUSY);

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\nGateWatch P2P smoke-test receiver");

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
    Serial.println("Listening...");
}

void loop() {
    String message;
    int16_t state = radio.receive(message);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.printf("RX: %s | RSSI %.1f dBm | SNR %.1f dB\n",
                      message.c_str(), radio.getRSSI(), radio.getSNR());
    } else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
        Serial.printf("RX error, RadioLib code %d\n", state);
    }
}
