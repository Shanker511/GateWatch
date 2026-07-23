# Meshtastic Overlay

This overlay adds a custom `gatewatch-xiao-s3` environment and a `GateWatchModule` to the official Meshtastic firmware source tree.

## Radio mapping

- D1 / GPIO2: Wio DIO1
- D2 / GPIO3: Wio NRST
- D3 / GPIO4: Wio BUSY
- D4 / GPIO5: Wio NSS/CS
- D5 / GPIO6: Wio RF_SW/RXEN
- D8 / GPIO7: Wio SCK
- D9 / GPIO8: Wio MISO
- D10 / GPIO9: Wio MOSI

## Sensor mapping

- D6 / GPIO43: normally-closed fence switch to ground
- D7 / GPIO44: active-high PIR output

## Build status

The source is intended to be compiled against current upstream Meshtastic firmware. The GitHub Actions workflow is the build authority. A successful desktop simulator test does not prove that this overlay compiles or that the radio wiring works.
