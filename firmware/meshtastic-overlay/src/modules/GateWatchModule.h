#pragma once

#include "GateWatchConfig.h"
#include "SinglePortModule.h"
#include "concurrency/OSThread.h"
#include <stdint.h>

class GateWatchModule : public SinglePortModule, private concurrency::OSThread
{
  public:
    GateWatchModule();

  protected:
    int32_t runOnce() override;
    ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    enum class AlertPriority : uint8_t { HEARTBEAT = 0, MOTION = 1, RESTORE = 2, BREAK = 3 };

    struct DebouncedInput {
        bool raw = false;
        bool stable = false;
        uint32_t changedAt = 0;
        bool initialized = false;
    };

    struct PendingMessage {
        bool used = false;
        uint32_t to = 0xffffffffU;
        uint8_t channel = 0;
        AlertPriority priority = AlertPriority::HEARTBEAT;
        char text[meshtastic_Constants_DATA_PAYLOAD_LEN + 1] = {0};
    };

    static constexpr uint8_t QUEUE_SIZE = 6;
    static constexpr uint32_t BROADCAST_NODE = 0xffffffffU;

    bool firstRun = true;
    bool startupQueued = false;
    bool fenceBroken = false;
    bool pirActive = false;
    uint32_t bootAt = 0;
    uint32_t lastMotionAlertAt = 0;
    uint32_t lastHeartbeatAt = 0;
    uint32_t lastTxAt = 0;
    DebouncedInput fenceInput;
    DebouncedInput pirInput;
    PendingMessage queue[QUEUE_SIZE];

    bool updateInput(DebouncedInput &input, bool raw, uint32_t debounceMs, uint32_t now);
    void onFenceChanged(bool broken, uint32_t now);
    void onPirChanged(bool active, uint32_t now);
    void queueStatus(uint32_t to, uint8_t channel, bool heartbeat);
    bool enqueue(const char *text, AlertPriority priority, uint32_t to = BROADCAST_NODE,
                 uint8_t channel = GATEWATCH_ALERT_CHANNEL);
    void transmitNext(uint32_t now);
    bool sendText(const char *text, uint32_t to, uint8_t channel);
    static void uppercase(char *text);
};
