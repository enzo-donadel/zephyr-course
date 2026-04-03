#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/../../"
VENV_ACTIVATE="$ROOT_DIR/../../.venv/Scripts/activate"

if [ ! -f "$VENV_ACTIVATE" ]; then
    echo "ERROR: Could not find venv activate script."
    echo "Expected at: $VENV_ACTIVATE"
    exit 1
fi

source "$VENV_ACTIVATE"

cd "$ROOT_DIR"

if [ ! -f "${ROOT_DIR}build/zephyr/zephyr.hex" ]; then
    echo "ERROR: Build output not found. Run build.sh first."
    exit 1
fi

echo
if west flash -d build "$@"; then
    echo
    echo "FLASH SUCCEEDED."
else
    echo
    echo "FLASH FAILED."
    exit 1
fi
