#!/usr/bin/env bash

#скрипт для подсчёта строчек кода
find src include -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.c" \) -print0 2>/dev/null |
  xargs -0 wc -l 2>/dev/null |
  awk '{sum += $1} END {printf "Всего строк в src/ и include/: %d\n", sum}'