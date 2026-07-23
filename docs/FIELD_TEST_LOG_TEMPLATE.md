# GateWatch Field Test Log

## Test identification

- Date:
- Location:
- Weather:
- Tester:
- Firmware commit:
- Meshtastic firmware reference:
- Node name:
- Region setting:
- Channel configuration:

## Hardware

- XIAO board revision:
- Wio-SX1262 module revision:
- Antenna model/frequency:
- Power source:
- PIR model:
- Microswitch model:
- Enclosure:

## Test cases

| Test | Expected result | Actual result | Pass/Fail | Evidence file |
|---|---|---|---|---|
| Boot with fence secure | Startup/status reports SECURE | | | |
| Release fence tension | FENCE BREAK received | | | |
| Restore tension | FENCE RESTORED received | | | |
| Motion while secure | MOTION DETECTED received | | | |
| Motion after break | MOTION NEAR BROKEN FENCE received | | | |
| PIR cooldown | No message flood | | | |
| Status request | Direct status reply | | | |
| 15-minute heartbeat | Status received | | | |
| Sensor cable disconnected | Break/fault state | | | |
| Receiver moved farther away | Message still received | | | |

## Range results

| Distance | Terrain/obstructions | Packets sent | Packets received | RSSI | SNR |
|---:|---|---:|---:|---:|---:|
| | | | | | |

## Power observations

- Idle current:
- Transmit peak:
- Battery voltage at start:
- Battery voltage at end:
- Test duration:

## Problems and corrections

Record failures honestly. Include the symptom, suspected cause, correction, and retest result.
