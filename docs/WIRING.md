# Wiring Reference

This document is the authoritative prototype wiring reference. The images under `assets/concepts/` are communication artwork and may simplify or mislabel details.

## Bare Wio-SX1262 to XIAO ESP32-S3

The following exposed-header mapping is based on the working header-board pin definition documented in the Meshtastic firmware project.

| Wio signal | XIAO label | ESP32-S3 GPIO | Notes |
|---|---|---:|---|
| DIO1 | D1 | 2 | SX1262 interrupt |
| NRST | D2 | 3 | Radio reset |
| BUSY | D3 | 4 | Radio busy line |
| NSS / CS | D4 | 5 | SPI chip select |
| RF_SW / RXEN | D5 | 6 | External receive switch control |
| SCK | D8 | 7 | SPI clock |
| MISO | D9 | 8 | SPI radio to MCU |
| MOSI | D10 | 9 | SPI MCU to radio |
| VCC | 3V3 | — | 3.3 V only |
| GND | GND | — | Connect all grounds |
| ANT | U.FL/IPEX | — | Matched antenna; no GPIO connection |

Use short wiring and a solid ground reference. Add a 100 µF electrolytic capacitor and a 100 nF ceramic capacitor close to the radio module.

## Fence tension switch

Use the normally-closed contact. This creates a fail-safe behavior where a broken sensor cable also appears as an alarm.

```text
XIAO D6 / GPIO43 ---- microswitch NC
GND ----------------- microswitch COM
```

Firmware mode: `INPUT_PULLUP`

| Electrical state | Reading | GateWatch state |
|---|---|---|
| NC switch closed | LOW | SECURE |
| Switch open | HIGH | FENCE BREAK |
| Cable cut/disconnected | HIGH | FENCE BREAK |

For long cable runs, place a 1 kΩ series resistor near the XIAO input and a 100 nF capacitor from the GPIO side of the resistor to ground.

## PIR motion sensor

Only use a PIR output that is safe for 3.3 V logic.

```text
PIR VCC ---- XIAO 3V3
PIR GND ---- XIAO GND
PIR OUT ---- XIAO D7 / GPIO44
```

| PIR output | GateWatch state |
|---|---|
| LOW | IDLE |
| HIGH | MOTION |

The firmware ignores PIR activity during warm-up and applies a cooldown to prevent mesh flooding.

## Breadboard warning

The Wio-SX1262 is a surface-mount module. It must be mounted to a breakout, carrier, or mechanically secured perfboard assembly. Do not use unsupported flying wires attached directly to the module pads for field testing.

## Power

- Wio-SX1262 supply: 3.3 V only
- Bench power: XIAO USB-C is acceptable
- Place decoupling close to the radio
- Attach the antenna before transmitting
- Set the legal Meshtastic region before over-the-air tests
