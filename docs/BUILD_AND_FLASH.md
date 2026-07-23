# Build and Flash

## Standalone smoke test

The smoke test verifies the XIAO, Wio-SX1262 wiring, tension switch, PIR sensor, and point-to-point LoRa transmission before the full Meshtastic build is introduced.

Requirements:

- Python 3
- PlatformIO Core or VS Code with PlatformIO
- USB data cable
- antenna attached to the Wio-SX1262

```bash
cd firmware/radio_sensor_smoke_test
python -m pip install platformio
python -m platformio run -e gatewatch-smoke-tx
python -m platformio run -e gatewatch-smoke-tx --target upload
python -m platformio device monitor --baud 115200
```

Optional matching point-to-point receiver:

```bash
python -m platformio run -e gatewatch-smoke-rx
python -m platformio run -e gatewatch-smoke-rx --target upload
```

The smoke-test packets are ordinary LoRa packets and are not Meshtastic-compatible.

## Meshtastic overlay

Windows:

```powershell
cd firmware\meshtastic-overlay
.\scripts\build.ps1
```

Linux/macOS:

```bash
cd firmware/meshtastic-overlay
./scripts/build.sh
```

The scripts clone the official firmware, apply the GateWatch variant and module, patch module initialization, and run PlatformIO.

Set `MESHTASTIC_REF` to a known release or commit when freezing a tested competition build.

After flashing:

1. Set the legal LoRa region.
2. Configure a private primary channel shared by the field node and receiver.
3. Confirm normal text messages between the nodes.
4. Trigger the fence switch and PIR.
5. Send `GW STATUS` from the receiver.
