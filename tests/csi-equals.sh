#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

# CSI = 0 ... 19 h and CSI = ... l are no-op
for l in h l; do
    for k in $(seq 0 19); do
        str=$(printf 'Hello, \e[0;1mBold\e[=%s%s\e[0m world!' "$k" "$l")
        want=$'Hello, <span style="font-weight:bold;">Bold</span> world!'
        got=$(printf '%s' "$str" | ./ansi2html -p vga)
        str_eq_html "$str" "$want" "$got"

        want=$'Hello, Bold world!'
        got=$(printf '%s' "$str" | ./ansi2html -S)
        str_eq_html "$str" "$want" "$got"
    done
done

done_testing
