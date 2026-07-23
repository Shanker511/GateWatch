# GateWatch — Start Here

This repository targets a **Seeed Studio XIAO ESP32-S3**, a **bare Wio-SX1262 module**, a normally-closed mechanical fence-tension switch, and a 3.3 V-compatible PIR sensor.

## Do this first

1. Read `docs/SAFETY.md`.
2. Read `docs/WIRING.md` and verify every Wio module pad before soldering.
3. Build or obtain a carrier/breakout for the bare module. Do not hang jumper wires directly from unsupported pads.
4. Attach the correct antenna before powering firmware that may transmit.
5. Power the radio from 3.3 V only.

## Validation sequence

### 1. Run desktop tests

```powershell
python -m unittest discover -s simulator -v
```

### 2. Flash the standalone smoke test

```powershell
cd firmware\radio_sensor_smoke_test
python -m pip install platformio
python -m platformio run -e gatewatch-smoke-tx
python -m platformio run -e gatewatch-smoke-tx --target upload
python -m platformio device monitor --baud 115200
```

Expected serial events:

```text
BOOT fence=SECURE
FENCE BREAK
FENCE RESTORED
MOTION DETECTED
MOTION NEAR BROKEN FENCE
HEARTBEAT fence=SECURE motion=IDLE
```

### 3. Build the Meshtastic overlay

Windows PowerShell:

```powershell
cd firmware\meshtastic-overlay
.\scripts\build.ps1
```

Linux/macOS:

```bash
cd firmware/meshtastic-overlay
./scripts/build.sh
```

The GitHub Actions workflow performs the same build after the repository is pushed.

### 4. Collect evidence

Use `docs/FIELD_TEST_LOG_TEMPLATE.md` and add real photographs under `assets/photos/`. Concept art is not accepted as authenticity evidence.
