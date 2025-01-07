#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

for seqi in $(seq 0 254); do

    str="A$(for i in $(seq 1 "$seqi"); do printf '\e[38;2;%d;%d;%dmX' "$i" "$i" "$i"; done)$(printf '\e[0m')B"
    want="A$(for i in $(seq 1 "$seqi"); do printf '<span style="color:#%02X%02X%02X;">X</span>' "$i" "$i" "$i"; done)B"
    got=$(printf '%s' "$str" | ./ansi2html --rgb-for fg F011F0 -p vga --use-classes)
    str_eq_html "$str" "$want" "$got"

done

done_testing
