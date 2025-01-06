#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

str=$'\e[0;31mred\e[7mreverse\e[0m'
want='<span style="color:#AA0000;">red</span><span style="color:#000000;background-color:#AA0000;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want='<span class="fg-1">red</span><span class="bg-1" style="color:#000000;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-classes)
str_eq_html "$str" "$want" "$got"

str=$'\e[0;31mred\e[42mon green\e[7mreverse\e[0m'
want='<span style="color:#A00;">red</span><span style="color:#A00;background-color:#0A0;">on green</span><span style="color:#0A0;background-color:#A00;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-compact)
str_eq_html "$str" "$want" "$got"

str=$'\e[0;31mred\e[42mon green\e[7mreverse\e[0m'
want='<span style="color:#AA0000;">red</span><span style="color:#AA0000;background-color:#00AA00;">on green</span><span style="color:#00AA00;background-color:#AA0000;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want='<span class="fg-1">red</span><span class="fg-1 bg-2">on green</span><span class="fg-2 bg-1">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-classes)
str_eq_html "$str" "$want" "$got"

str=$'\e[0;1;31mred\e[42mon green\e[7mreverse\e[0m'
want='<span style="font-weight:bold;color:#AA0000;">red</span><span style="font-weight:bold;color:#AA0000;background-color:#00AA00;">on green</span><span style="font-weight:bold;color:#00AA00;background-color:#AA0000;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want='<span class="bold fg-1">red</span><span class="bold fg-1 bg-2">on green</span><span class="bold fg-2 bg-1">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-classes)
str_eq_html "$str" "$want" "$got"

want='<span style="color:#FF5555;">red</span><span style="color:#FF5555;background-color:#00AA00;">on green</span><span style="color:#55FF55;background-color:#FF5555;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b)
str_eq_html "$str" "$want" "$got"

want='<span style="color:#F55;">red</span><span style="color:#F55;background-color:#0A0;">on green</span><span style="color:#5F5;background-color:#F55;">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-compact)
str_eq_html "$str" "$want" "$got"

want='<span class="fg-9">red</span><span class="fg-9 bg-2">on green</span><span class="fg-10 bg-1">reverse</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga -b --use-classes)
str_eq_html "$str" "$want" "$got"

# bg 40/fg 37 "reversed" should be bg 47, fg 30
str=$'\e[0;40;37mgreyonblack\e[7mreversed\e[0;30;47msame\e[0m'
want='<span style="color:#AAAAAA;background-color:#000000;">greyonblack</span><span style="color:#000000;background-color:#AAAAAA;">reversed</span><span style="color:#000000;background-color:#AAAAAA;">same</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --rgb-for fg '#989898' --rgb-for bg '#111111')
str_eq_html "$str" "$want" "$got"

want='<span style="color:#AAA;background-color:#000;">greyonblack</span><span style="color:#000;background-color:#AAA;">reversed</span><span style="color:#000;background-color:#AAA;">same</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --rgb-for fg '#989898' --rgb-for bg '#111111' --use-compact)
str_eq_html "$str" "$want" "$got"

# But when bold + "bold is bright" is in the mix... oh boy
str=$'\e[0;1;40;37mgreyonblack\e[7mreversed\e[0;1;30;47msame\e[0m'
want='<span style="color:#FFFFFF;background-color:#000000;">greyonblack</span><span style="color:#555555;background-color:#FFFFFF;">reversed</span><span style="color:#555555;background-color:#AAAAAA;">same</span>'
got=$(printf '%s' "$str" | ./ansi2html -b -p vga --rgb-for fg '#989898' --rgb-for bg '#111111')
str_eq_html "$str" "$want" "$got"

# with classes...
want='<span class="fg-15 bg-0">greyonblack</span><span class="fg-8 bg-7">reversed</span><span class="fg-8 bg-7">same</span>'
got=$(printf '%s' "$str" | ./ansi2html -b -p vga --rgb-for fg '#989898' --rgb-for bg '#111111' --use-classes)
str_eq_html "$str" "$want" "$got"

# Stripping:
want='greyonblackreversedsame'
got=$(printf '%s' "$str" | ./ansi2html -S)

done_testing
