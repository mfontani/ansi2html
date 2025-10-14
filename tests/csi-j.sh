#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

# CSI J is a no-op when stripping OR converting to HTML
str=$'Hello, \e[0;1mBold\e[J\e[0m world!'
want=$'Hello, <span style="font-weight:bold;">Bold</span> world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want=$'Hello, Bold world!'
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# CSI 0 J is also a no-op, same for CSI 1 J, CSI 2 J and CSI 3 J
for k in 0 1 2 3; do
    str=$(printf 'Hello, \e[0;1mBold\e[%sJ\e[0m world!' "$k")
    want=$'Hello, <span style="font-weight:bold;">Bold</span> world!'
    got=$(printf '%s' "$str" | ./ansi2html -p vga)
    str_eq_html "$str" "$want" "$got"

    want=$'Hello, Bold world!'
    got=$(printf '%s' "$str" | ./ansi2html -S)
    str_eq_html "$str" "$want" "$got"
done

done_testing
