#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

str=$'\e[0;31mred\e[0;1;31mbold red\e[0m'
want=$'<span style="color:#AA0000;">red</span><span style="font-weight:bold;color:#AA0000;">bold red</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Base test with VGA palette, but with "bold is bright" option enabled
want=$'<span style="color:#AA0000;">red</span><span style="color:#FF5555;">bold red</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# A separate "and now bold", then "no bold" should DWIM:
str=$'\e[0;31mred\e[1mbold red\e[22mred, normal intensity\e[0m'
want=$'<span style="color:#AA0000;">red</span><span style="font-weight:bold;color:#AA0000;">bold red</span><span style="color:#AA0000;">red, normal intensity</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want=$'<span style="color:#AA0000;">red</span><span style="color:#FF5555;">bold red</span><span style="color:#AA0000;">red, normal intensity</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# Stripping:
want=$'redbold redred, normal intensity'
got=$(printf '%s' "$str" | ./ansi2html -S)

done_testing
