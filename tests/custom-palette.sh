#!/bin/bash

. "$(dirname "$0")/.functions.sh"

str=$'\e[0;31mred\e[0mdefault'
want='<span style="color:#AA0000;">red</span>default'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want='<span style="color:#FF3030;">red</span>default'
got=$(printf '%s' "$str" | ./ansi2html -p vga --rgb-for 1 FF3030)
str_eq_html "$str" "$want" "$got"

want='<pre style="color:#999999;background-color:#111111;"><span style="color:#FF3030;">red</span>default</pre>'
got=$(printf '%s' "$str" | ./ansi2html -p vga --rgb-for 1 FF3030 --rgb-for fg 999999 --rgb-for bg 111111 --pre)
str_eq_html "$str" "$want" "$got"

# Stripping:
want='reddefault'
got=$(printf '%s' "$str" | ./ansi2html -S)

done_testing
