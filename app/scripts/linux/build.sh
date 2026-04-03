#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/../../"
VENV_ACTIVATE="$ROOT_DIR/../../.venv/Scripts/activate"

if [ -z "$VIRTUAL_ENV" ]; then
    echo "Trying to enable .venv at zephyr root (assuming West T2/T3 topology of workspace)"
    if [ ! -f "$VENV_ACTIVATE" ]; then
        echo "ERROR: Could not find venv activate script."
        exit 1
    fi
    source "$VENV_ACTIVATE"
fi

cd "$ROOT_DIR"
BOARD=sam_v71_xult/samv71q21b
echo
echo "Building..."
echo "Board: $BOARD"
echo "Output: ${ROOT_DIR}build/"
echo

if west build "$@" -d build -b "$BOARD" . -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON; then
    echo
    echo "BUILD SUCCEEDED."
else
    echo
    echo "BUILD FAILED."
    exit 1
fi
