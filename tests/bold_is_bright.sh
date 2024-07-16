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

# 256 colors 0-7 with "1;" with "-b" are treated as bright:
str=$'\e[0;38;5;1mred\e[1mbright\e[0m'
want='<span style="color:#AA0000;">red</span><span style="color:#FF5555;">bright</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes:
want='<span class="fg-1">red</span><span class="fg-9">bright</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

# 256 colors 8-15 with "1;" with "-b" are instead _bold_:
str=$'\e[0;38;5;9mred\e[1mbold\e[0m'
want='<span style="color:#FF5555;">red</span><span style="font-weight:bold;color:#FF5555;">bold</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes:
want='<span class="fg-9">red</span><span class="bold fg-9">bold</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

# Also 16+:
str=$'\e[0;38;5;88mred\e[1mbold\e[0m'
want='<span style="color:#870000;">red</span><span style="font-weight:bold;color:#870000;">bold</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes:
want='<span class="fg-88">red</span><span class="bold fg-88">bold</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

# But also the _default color_ gets bold with "-b":
str=$'default\e[1mbold\e[0m'
want='default<span style="font-weight:bold;">bold</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes:
want='default<span class="bold">bold</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

# Reverse!
str=$'default\e[1mbold\e[7mreverse\e[0m'
want='default<span style="font-weight:bold;">bold</span><span style="font-weight:bold;color:#000000;background-color:#AAAAAA;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# With classes:
want='default<span class="bold">bold</span><span class="bold" style="color:#000000;background-color:#AAAAAA;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

# Reverse with "bold is bright"!
str=$'\e[0;31;42mredongreen\e[1mbright\e[7mreverse\e[0m'
want='<span style="color:#AA0000;background-color:#00AA00;">redongreen</span><span style="color:#FF5555;background-color:#00AA00;">bright</span><span style="color:#55FF55;background-color:#FF5555;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

# Classes "detect" that "-b" shouldn't brighten the background color:
want='<span class="fg-1 bg-2">redongreen</span><span class="fg-9 bg-2">bright</span><span class="fg-10 bg-1">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

done_testing
