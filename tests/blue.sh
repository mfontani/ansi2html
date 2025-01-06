#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

str=$'\e[38;2;0;0;170mBlue FG\e[0m\e[48;2;0;0;170mBlue BG\e[0m\e[48;2;0;0;170m\e[38;2;192;151;188mNice FG/BG\e[7mreversed\e[0m'
want=$'<span style="color:#0000AA;">Blue FG</span><span style="background-color:#0000AA;">Blue BG</span><span style="color:#C097BC;background-color:#0000AA;">Nice FG/BG</span><span style="color:#0000AA;background-color:#C097BC;">reversed</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Stripping:
want=$'Blue FGBlue BGNice FG/BGreversed'
got=$(printf '%s' "$str" | ./ansi2html -S)

want=$'<span style="color:#00A;">Blue FG</span><span style="background-color:#00A;">Blue BG</span><span style="color:#C097BC;background-color:#00A;">Nice FG/BG</span><span style="color:#00A;background-color:#C097BC;">reversed</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-compact)
str_eq_html "$str" "$want" "$got"

done_testing
