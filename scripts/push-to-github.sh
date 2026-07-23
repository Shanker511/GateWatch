#!/usr/bin/env bash
set -euo pipefail

REPO_URL="https://github.com/Shanker511/GateWatch.git"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK="$(mktemp -d -t gatewatch-push-XXXXXX)"

command -v git >/dev/null || { echo "Git is not installed" >&2; exit 1; }

git clone "$REPO_URL" "$WORK"
(
  cd "$ROOT"
  tar --exclude='./.git' -cf - .
) | (
  cd "$WORK"
  tar -xf -
)

cd "$WORK"
python3 -m unittest discover -s simulator -v
git add --all
if git diff --cached --quiet; then
  echo "No changes to commit."
  echo "Working copy: $WORK"
  exit 0
fi

git commit -m "Add GateWatch fence monitoring prototype"
git push origin HEAD:main

echo "GateWatch was pushed successfully."
echo "Local working copy: $WORK"
