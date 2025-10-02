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

done_testing
