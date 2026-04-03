#!/usr/bin/env bash

FILE1="${1:-./build/zephyr/.config}"
FILE2="${2:-./build/zephyr/.config.old}"
OUTPUT="${3:-./build/menuconfig.changes}"

git diff --no-index "$FILE2" "$FILE1" \
    | grep -E '^\+' \
    | grep -vE '^\+\+\+' \
    | grep -vE '^\+#' \
    | sed 's/^+//' \
    > "$OUTPUT"
