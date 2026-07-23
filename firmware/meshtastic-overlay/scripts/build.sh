#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OVERLAY_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
WORK_DIR="${GATEWATCH_WORK_DIR:-$OVERLAY_DIR/.work}"
MESHTASTIC_DIR="$WORK_DIR/meshtastic"
MESHTASTIC_REF="${MESHTASTIC_REF:-master}"

mkdir -p "$WORK_DIR"
if [[ ! -d "$MESHTASTIC_DIR/.git" ]]; then
  git clone https://github.com/meshtastic/firmware.git "$MESHTASTIC_DIR"
fi

git -C "$MESHTASTIC_DIR" fetch --tags origin
git -C "$MESHTASTIC_DIR" checkout --force "$MESHTASTIC_REF"
git -C "$MESHTASTIC_DIR" submodule update --init --recursive
python3 "$SCRIPT_DIR/apply_overlay.py" "$MESHTASTIC_DIR"

if ! command -v pio >/dev/null 2>&1; then
  python3 -m pip install --user platformio
  export PATH="$HOME/.local/bin:$PATH"
fi

cd "$MESHTASTIC_DIR"
pio run -e gatewatch-xiao-s3

echo "Build output: $MESHTASTIC_DIR/.pio/build/gatewatch-xiao-s3"
