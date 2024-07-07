#!/bin/bash

set -e

str=$'\e[0;31mred\e[0;1;31mbold red\e[0m'

# Base test with VGA palette, and no "bold is bright" option
html=$(printf '%s' "$str" | ./ansi2html -p vga)
printf '%s\n' "$str" | cat -v
printf '%s\n' "$html" | cat -v
# In VGA, 31m is dark red / AA0000
# ... and NOT the "bright red" / FF5555
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" && exit 1
grep -q 'font-weight:bold' <<< "$html" || exit 1

# Base test with VGA palette, but with "bold is bright" option enabled
html=$(printf '%s' "$str" | ./ansi2html -p vga -b)
printf '%s\n' "$str" | cat -v
printf '%s\n' "$html" | cat -v
# In VGA, 31m is dark red / AA0000 but 1;31m is bright red / FF5555
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" || exit 1
grep -q 'font-weight:bold' <<< "$html" && exit 1

# A separate "and now bold", then "no bold" should DWIM:
str=$'\e[0;31mred\n\e[1mbold red\n\e[22mred, normal intensity\n\e[0m'
printf '%s\n' "$str" | cat -v
# First line should have the color, and no bold.
html=$(printf '%s' "$str" | ./ansi2html -p vga | head -n 1)
printf '%s\n' "$html" | cat -v
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" && exit 1
grep -q 'font-weight:bold' <<< "$html" && exit 1
# Second line should have the color, bold
html=$(printf '%s' "$str" | ./ansi2html -p vga | head -n 2 | tail -n 1)
printf '%s\n' "$html" | cat -v
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" && exit 1
grep -q 'font-weight:bold' <<< "$html" || exit 1
# Third line should have the color, no bold
html=$(printf '%s' "$str" | ./ansi2html -p vga | head -n 3 | tail -n 1)
printf '%s\n' "$html" | cat -v
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" && exit 1
grep -q 'font-weight:bold' <<< "$html" && exit 1

# With "bold is bright", it should DWIM too:
printf '%s\n' "$str" | cat -v
# First line should have the color, and no bold.
html=$(printf '%s' "$str" | ./ansi2html -b -p vga | head -n 1)
printf '%s\n' "$html" | cat -v
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" && exit 1
grep -q 'font-weight:bold' <<< "$html" && exit 1
# Second line should have the color, bold (bright)
html=$(printf '%s' "$str" | ./ansi2html -b -p vga | head -n 2 | tail -n 1)
printf '%s\n' "$html" | cat -v
grep -q 'color:#AA0000' <<< "$html" && exit 1
grep -q 'color:#FF5555' <<< "$html" || exit 1
grep -q 'font-weight:bold' <<< "$html" && exit 1
# Third line should have the color, no bold
html=$(printf '%s' "$str" | ./ansi2html -b -p vga | head -n 3 | tail -n 1)
printf '%s\n' "$html" | cat -v
grep -q 'color:#AA0000' <<< "$html" || exit 1
grep -q 'color:#FF5555' <<< "$html" && exit 1
grep -q 'font-weight:bold' <<< "$html" && exit 1

printf '%s: All tests passed.\n' "$0"
exit 0
