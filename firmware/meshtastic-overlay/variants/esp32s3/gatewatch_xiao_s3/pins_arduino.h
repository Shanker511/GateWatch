#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define USB_VID 0x2886
#define USB_PID 0x0059
#define USB_MANUFACTURER "Seeed"
#define USB_PRODUCT "GateWatch XIAO ESP32-S3"
#define USB_SERIAL "GateWatch"

static const uint8_t TX = 43;
static const uint8_t RX = 44;

// These Arduino defaults overlap GateWatch functions. Do not start Wire without remapping it.
static const uint8_t SDA = 5;
static const uint8_t SCL = 6;

static const uint8_t SS = 4;
static const uint8_t MOSI = 9;
static const uint8_t MISO = 8;
static const uint8_t SCK = 7;

static const uint8_t A0 = 1;
static const uint8_t A1 = 2;
static const uint8_t A2 = 3;
static const uint8_t A3 = 4;
static const uint8_t A4 = 5;
static const uint8_t A5 = 6;

static const uint8_t D0 = 1;
static const uint8_t D1 = 2;
static const uint8_t D2 = 3;
static const uint8_t D3 = 4;
static const uint8_t D4 = 5;
static const uint8_t D5 = 6;
static const uint8_t D6 = 43;
static const uint8_t D7 = 44;
static const uint8_t D8 = 7;
static const uint8_t D9 = 8;
static const uint8_t D10 = 9;

#endif
