#pragma once

// Human-readable identity included in every GateWatch application message.
#define GATEWATCH_NODE_NAME "GW-NORTH-01"

// Sensors on exposed XIAO header pins.
#define GATEWATCH_FENCE_PIN 43 // XIAO D6; NC switch to GND
#define GATEWATCH_PIR_PIN 44   // XIAO D7; active HIGH

#define GATEWATCH_FENCE_BREAK_LEVEL HIGH
#define GATEWATCH_PIR_ACTIVE_LEVEL HIGH

#define GATEWATCH_FENCE_DEBOUNCE_MS 750UL
#define GATEWATCH_PIR_DEBOUNCE_MS 200UL
#define GATEWATCH_PIR_WARMUP_MS 30000UL
#define GATEWATCH_PIR_COOLDOWN_MS 60000UL
#define GATEWATCH_HEARTBEAT_MS 900000UL
#define GATEWATCH_TX_SPACING_MS 15000UL
#define GATEWATCH_STARTUP_REPORT_MS 35000UL

// Meshtastic channel index used for broadcast alerts. Channel 0 is the primary channel.
#define GATEWATCH_ALERT_CHANNEL 0

// Bell character can trigger audible/vibration notifications in some clients.
#define GATEWATCH_APPEND_BELL 1
