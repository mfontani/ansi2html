#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

# CSI K is a no-op when stripping OR converting to HTML
str=$'Hello, \e[0;1mBold\e[K\e[0m world!'
want=$'Hello, <span style="font-weight:bold;">Bold</span> world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want=$'Hello, Bold world!'
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# CSI 0 K is also a no-op, same for CSI 1 K and CSI 2 K
for k in 0 1 2; do
    str=$(printf 'Hello, \e[0;1mBold\e[%sK\e[0m world!' "$k")
    want=$'Hello, <span style="font-weight:bold;">Bold</span> world!'
    got=$(printf '%s' "$str" | ./ansi2html -p vga)
    str_eq_html "$str" "$want" "$got"

    want=$'Hello, Bold world!'
    got=$(printf '%s' "$str" | ./ansi2html -S)
    str_eq_html "$str" "$want" "$got"
done

done_testing
