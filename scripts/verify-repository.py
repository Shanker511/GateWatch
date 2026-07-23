#!/usr/bin/env python3
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
REQUIRED = [
    "README.md",
    "LICENSE",
    "START_HERE.md",
    "SUBMISSION_CHECKLIST.md",
    "docs/WIRING.md",
    "docs/SAFETY.md",
    "firmware/radio_sensor_smoke_test/platformio.ini",
    "firmware/meshtastic-overlay/scripts/apply_overlay.py",
    "hardware/bom.csv",
]

missing = [name for name in REQUIRED if not (ROOT / name).exists()]
concepts = sorted((ROOT / "assets/concepts").glob("*.png"))

if missing:
    print("Missing required files:")
    for name in missing:
        print(f" - {name}")
    sys.exit(1)

if len(concepts) != 10:
    print(f"Expected 10 concept images, found {len(concepts)}")
    sys.exit(1)

result = subprocess.run(
    [sys.executable, "-m", "unittest", "discover", "-s", "simulator", "-v"],
    cwd=ROOT,
)
if result.returncode:
    sys.exit(result.returncode)

print("Repository verification passed.")
print("Note: this does not prove the Meshtastic firmware compiles or the hardware works.")
