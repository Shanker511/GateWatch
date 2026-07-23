#pragma once

#include <Arduino.h>

namespace GateWatchPins {
constexpr uint8_t LORA_DIO1 = 2;   // XIAO D1
constexpr uint8_t LORA_RESET = 3;  // XIAO D2
constexpr uint8_t LORA_BUSY = 4;   // XIAO D3
constexpr uint8_t LORA_CS = 5;     // XIAO D4
constexpr uint8_t LORA_RF_SW = 6;  // XIAO D5 / RXEN
constexpr uint8_t LORA_SCK = 7;    // XIAO D8
constexpr uint8_t LORA_MISO = 8;   // XIAO D9
constexpr uint8_t LORA_MOSI = 9;   // XIAO D10
constexpr uint8_t FENCE = 43;      // XIAO D6, NC switch to GND
constexpr uint8_t PIR = 44;        // XIAO D7, active HIGH
}

namespace GateWatchRadio {
constexpr float FREQUENCY_MHZ = 915.0; // Change for your legal test region.
constexpr float BANDWIDTH_KHZ = 125.0;
constexpr uint8_t SPREADING_FACTOR = 9;
constexpr uint8_t CODING_RATE = 7;
constexpr int8_t POWER_DBM = 10;
constexpr uint16_t PREAMBLE = 8;
constexpr float TCXO_VOLTAGE = 1.8;
}
