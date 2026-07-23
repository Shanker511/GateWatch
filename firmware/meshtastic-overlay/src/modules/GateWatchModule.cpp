#include "GateWatchModule.h"

#include "GateWatchConfig.h"
#include "MeshService.h"
#include "mesh/Channels.h"
#include "configuration.h"
#include "main.h"

#include <Arduino.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

GateWatchModule::GateWatchModule()
    : SinglePortModule("gatewatch", meshtastic_PortNum_TEXT_MESSAGE_APP), concurrency::OSThread("GateWatch")
{
}

bool GateWatchModule::updateInput(DebouncedInput &input, bool raw, uint32_t debounceMs, uint32_t now)
{
    if (!input.initialized) {
        input.raw = raw;
        input.stable = raw;
        input.changedAt = now;
        input.initialized = true;
        return false;
    }

    if (raw != input.raw) {
        input.raw = raw;
        input.changedAt = now;
    }

    if (input.stable != input.raw && (uint32_t)(now - input.changedAt) >= debounceMs) {
        input.stable = input.raw;
        return true;
    }
    return false;
}

int32_t GateWatchModule::runOnce()
{
    const uint32_t now = millis();

    if (firstRun) {
        firstRun = false;
        bootAt = now;
        lastHeartbeatAt = now;
        pinMode(GATEWATCH_FENCE_PIN, INPUT_PULLUP);
        pinMode(GATEWATCH_PIR_PIN, INPUT_PULLDOWN);

        const bool rawFenceBroken = digitalRead(GATEWATCH_FENCE_PIN) == GATEWATCH_FENCE_BREAK_LEVEL;
        const bool rawPirActive = digitalRead(GATEWATCH_PIR_PIN) == GATEWATCH_PIR_ACTIVE_LEVEL;
        updateInput(fenceInput, rawFenceBroken, 0, now);
        updateInput(pirInput, rawPirActive, 0, now);
        fenceBroken = fenceInput.stable;
        pirActive = false; // Ignore PIR state until warm-up completes.

        LOG_INFO("GateWatch initialized: fence=%s", fenceBroken ? "BROKEN" : "SECURE");
        return setStartDelay();
    }

    const bool rawFenceBroken = digitalRead(GATEWATCH_FENCE_PIN) == GATEWATCH_FENCE_BREAK_LEVEL;
    if (updateInput(fenceInput, rawFenceBroken, GATEWATCH_FENCE_DEBOUNCE_MS, now)) {
        onFenceChanged(fenceInput.stable, now);
    }

    const bool pirWarmed = (uint32_t)(now - bootAt) >= GATEWATCH_PIR_WARMUP_MS;
    if (pirWarmed) {
        const bool rawPirActive = digitalRead(GATEWATCH_PIR_PIN) == GATEWATCH_PIR_ACTIVE_LEVEL;
        if (updateInput(pirInput, rawPirActive, GATEWATCH_PIR_DEBOUNCE_MS, now)) {
            onPirChanged(pirInput.stable, now);
        }
    }

    if (!startupQueued && (uint32_t)(now - bootAt) >= GATEWATCH_STARTUP_REPORT_MS) {
        startupQueued = true;
        queueStatus(BROADCAST_NODE, GATEWATCH_ALERT_CHANNEL, false);
    }

    if ((uint32_t)(now - lastHeartbeatAt) >= GATEWATCH_HEARTBEAT_MS) {
        lastHeartbeatAt = now;
        queueStatus(BROADCAST_NODE, GATEWATCH_ALERT_CHANNEL, true);
    }

    transmitNext(now);
    return 100;
}

void GateWatchModule::onFenceChanged(bool broken, uint32_t now)
{
    (void)now;
    fenceBroken = broken;
    char message[meshtastic_Constants_DATA_PAYLOAD_LEN + 1];
    snprintf(message, sizeof(message), "%s | %s", GATEWATCH_NODE_NAME, broken ? "FENCE BREAK" : "FENCE RESTORED");
    enqueue(message, broken ? AlertPriority::BREAK : AlertPriority::RESTORE);
    LOG_WARN("GateWatch fence state: %s", broken ? "BROKEN" : "SECURE");
}

void GateWatchModule::onPirChanged(bool active, uint32_t now)
{
    pirActive = active;
    if (!active)
        return;

    if (lastMotionAlertAt != 0 && (uint32_t)(now - lastMotionAlertAt) < GATEWATCH_PIR_COOLDOWN_MS)
        return;

    lastMotionAlertAt = now;
    char message[meshtastic_Constants_DATA_PAYLOAD_LEN + 1];
    snprintf(message, sizeof(message), "%s | %s", GATEWATCH_NODE_NAME,
             fenceBroken ? "MOTION NEAR BROKEN FENCE" : "MOTION DETECTED");
    enqueue(message, fenceBroken ? AlertPriority::BREAK : AlertPriority::MOTION);
    LOG_INFO("GateWatch PIR event; fence=%s", fenceBroken ? "BROKEN" : "SECURE");
}

void GateWatchModule::queueStatus(uint32_t to, uint8_t channel, bool heartbeat)
{
    char message[meshtastic_Constants_DATA_PAYLOAD_LEN + 1];
    const unsigned long uptimeMinutes = millis() / 60000UL;
    snprintf(message, sizeof(message), "%s | %s fence=%s motion=%s up=%lum", GATEWATCH_NODE_NAME,
             heartbeat ? "HEARTBEAT" : "STATUS", fenceBroken ? "BROKEN" : "SECURE",
             pirActive ? "ACTIVE" : "IDLE", uptimeMinutes);
    enqueue(message, AlertPriority::HEARTBEAT, to, channel);
}

bool GateWatchModule::enqueue(const char *text, AlertPriority priority, uint32_t to, uint8_t channel)
{
    int empty = -1;
    int replace = -1;
    AlertPriority lowest = AlertPriority::BREAK;

    for (uint8_t i = 0; i < QUEUE_SIZE; ++i) {
        if (!queue[i].used) {
            empty = i;
            break;
        }
        if (queue[i].priority <= lowest) {
            lowest = queue[i].priority;
            replace = i;
        }
    }

    int slot = empty;
    if (slot < 0 && replace >= 0 && priority > lowest)
        slot = replace;
    if (slot < 0) {
        LOG_WARN("GateWatch queue full; dropping message: %s", text);
        return false;
    }

    queue[slot].used = true;
    queue[slot].to = to;
    queue[slot].channel = channel;
    queue[slot].priority = priority;
    strncpy(queue[slot].text, text, sizeof(queue[slot].text) - 1);
    queue[slot].text[sizeof(queue[slot].text) - 1] = '\0';
    return true;
}

void GateWatchModule::transmitNext(uint32_t now)
{
    if ((uint32_t)(now - lastTxAt) < GATEWATCH_TX_SPACING_MS)
        return;

    int selected = -1;
    AlertPriority best = AlertPriority::HEARTBEAT;
    for (uint8_t i = 0; i < QUEUE_SIZE; ++i) {
        if (!queue[i].used)
            continue;
        if (selected < 0 || queue[i].priority > best) {
            selected = i;
            best = queue[i].priority;
        }
    }

    if (selected < 0)
        return;

    if (sendText(queue[selected].text, queue[selected].to, queue[selected].channel)) {
        queue[selected].used = false;
        lastTxAt = now;
    }
}

bool GateWatchModule::sendText(const char *text, uint32_t to, uint8_t channel)
{
    if (to == BROADCAST_NODE && channels.isDefaultChannel(channel)) {
        LOG_ERROR("GateWatch will not send application alerts on the default public channel");
        return false;
    }

    meshtastic_MeshPacket *packet = allocDataPacket();
    packet->to = to;
    packet->channel = channel;
    packet->want_ack = to != BROADCAST_NODE;

    size_t length = strnlen(text, meshtastic_Constants_DATA_PAYLOAD_LEN);
    memcpy(packet->decoded.payload.bytes, text, length);
#if GATEWATCH_APPEND_BELL
    if (length < meshtastic_Constants_DATA_PAYLOAD_LEN) {
        packet->decoded.payload.bytes[length++] = 7;
    }
#endif
    packet->decoded.payload.size = length;

    LOG_INFO("GateWatch TX to=%x ch=%u msg=%.*s", packet->to, packet->channel,
             packet->decoded.payload.size, packet->decoded.payload.bytes);
    service->sendToMesh(packet);
    return true;
}

void GateWatchModule::uppercase(char *text)
{
    for (; *text; ++text)
        *text = (char)toupper((unsigned char)*text);
}

ProcessMessage GateWatchModule::handleReceived(const meshtastic_MeshPacket &mp)
{
    if (mp.decoded.payload.size == 0)
        return ProcessMessage::CONTINUE;

    char command[64] = {0};
    const size_t length = mp.decoded.payload.size < sizeof(command) - 1 ? mp.decoded.payload.size : sizeof(command) - 1;
    memcpy(command, mp.decoded.payload.bytes, length);
    command[length] = '\0';
    uppercase(command);

    if (strstr(command, "GW STATUS") != nullptr) {
        queueStatus(mp.from, mp.channel, false);
    } else if (strstr(command, "GW HELP") != nullptr) {
        enqueue("GateWatch commands: GW STATUS | GW HELP", AlertPriority::MOTION, mp.from, mp.channel);
    }

    // Let the normal TextMessageModule display/store the message too.
    return ProcessMessage::CONTINUE;
}
