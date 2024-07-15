#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

str=$'\e[0;31mred\e[0;1;31mbold/bright red\e[0m'
# Without "bold is bright", it's bold, not bright:
want='<span style="color:#AA0000;">red</span><span style="font-weight:bold;color:#AA0000;">bold/bright red</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# With classes, it's bold, not bright:
want='<span class="fg-1">red</span><span class="bold fg-1">bold/bright red</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-classes)
str_eq_html "$str" "$want" "$got"

# With "bold is bright", it's bright, not bold:
want='<span style="color:#AA0000;">red</span><span style="color:#FF5555;">bold/bright red</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes, it's bright, not bold:
want='<span class="fg-1">red</span><span class="fg-9">bold/bright red</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

str=$'\e[0;31mred\e[0;1;31mbold/bright red\e[42mon green\e[0m'
# Without "bold is bright", it's bold, not bright:
want='<span style="color:#AA0000;">red</span><span style="font-weight:bold;color:#AA0000;">bold/bright red</span><span style="font-weight:bold;color:#AA0000;background-color:#00AA00;">on green</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# With classes, it's bold, not bright:
want='<span class="fg-1">red</span><span class="bold fg-1">bold/bright red</span><span class="bold fg-1 bg-2">on green</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-classes)
str_eq_html "$str" "$want" "$got"

# With "bold is bright", it's bright, not bold (and background is dark):
want='<span style="color:#AA0000;">red</span><span style="color:#FF5555;">bold/bright red</span><span style="color:#FF5555;background-color:#00AA00;">on green</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes, it's bright, not bold (and background is dark):
want='<span class="fg-1">red</span><span class="fg-9">bold/bright red</span><span class="fg-9 bg-2">on green</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

done_testing
