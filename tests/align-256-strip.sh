#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

for seqi in $(seq 0 254); do

    str="A$(for i in $(seq 1 "$seqi"); do printf '\e[38;5;%dmX' "$i"; done)$(printf '\e[0m')B"
    want="A$(for i in $(seq 1 "$seqi"); do printf 'X'; done)B"
    got=$(printf '%s' "$str" | ./ansi2html -S)
    str_eq_html "$str" "$want" "$got"

done

done_testing
