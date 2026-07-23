#pragma once

// GateWatch exposed-header XIAO ESP32-S3 + bare/header Wio-SX1262.
// This does not use the official kit's hidden 30-pin B2B radio connections.

#define LED_POWER 48
#define LED_STATE_ON 1
#define BUTTON_PIN 21
#define BUTTON_NEED_PULLUP
#define BATTERY_PIN -1

#define USE_SX1262
#define LORA_MISO 8
#define LORA_SCK 7
#define LORA_MOSI 9
#define LORA_CS 5
#define LORA_RESET 3
#define LORA_DIO1 2
#define LORA_DIO2 6

#define SX126X_CS LORA_CS
#define SX126X_DIO1 LORA_DIO1
#define SX126X_BUSY 4
#define SX126X_RESET LORA_RESET

// Wio-SX1262: DIO2 controls the internal antenna switch, DIO3 powers the TCXO,
// and the exposed RF_SW/RXEN control is on XIAO D5 / GPIO6.
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_RXEN 6
#define SX126X_TXEN RADIOLIB_NC
#define SX126X_DIO3_TCXO_VOLTAGE 1.8

#include "GateWatchConfig.h"
