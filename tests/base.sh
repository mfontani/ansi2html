#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

# A simple string is output as-is
str=$'Hello, world!'
want=$'Hello, world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# A \e[0m resets the color
str=$'Hello, \e[38;2;0;0;170mBlue FG\e[0m world!'
want=$'Hello, <span style="color:#0000AA;">Blue FG</span> world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# A \e[m also resets the color
str=$'Hello, \e[38;2;0;0;170mBlue FG\e[m world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# A \e[;m also resets the color
str=$'Hello, \e[38;2;0;0;170mBlue FG\e[;m world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Or any number of semicolons, really
str=$'Hello, \e[38;2;0;0;170mBlue FG\e[;;;;m world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# A large number of zeros can be used, too - but there's a limit to how many
# numbers can be used in a SGR sequence: max is 127.
str=$(printf 'Hello, \e[38;2;0;0;170mBlue FG\e[%sm world!' "$(for _ in $(seq 1 127); do printf '0;'; done)")
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

want=$'Hello, <span style="color:#00A;">Blue FG</span> world!'
got=$(printf '%s' "$str" | ./ansi2html -p vga --use-compact)
str_eq_html "$str" "$want" "$got"

# This errors with "SGR sequence too long":
str=$(printf '\e[%sm' "$(for _ in $(seq 1 128); do printf '0;'; done)")
want='ERROR: SGR sequence too long, at 258 characters read / 256 in SGR sequence which begun at 2 characters read.'
got=$(printf '%s' "$str" | ./ansi2html -p vga 2>&1 || true)
str_eq_html "$str" "$want" "$got"

# The "matter" is about number of numbers in the sequence, not characters.
# This is the limit: 3 * 42 = 126, + 1 for the "0" at the end = 127, which is
# the maximum allowed:
str=$(printf 'foo\e[%s0mbar' "$(for _ in $(seq 1 42); do printf '0;1;32;'; done)")
want='foobar'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# This is one too many:
str=$(printf '\e[%s0;0m' "$(for _ in $(seq 1 42); do printf '0;1;32;'; done)")
want='ERROR: SGR sequence too long, at 300 characters read / 298 in SGR sequence which begun at 2 characters read.'
got=$(printf '%s' "$str" | ./ansi2html -p vga 2>&1 || true)
str_eq_html "$str" "$want" "$got"

# We just don't support numbers outside 0-255 in SGRs:
str=$'\e[0;666m'
want="ERROR: SGR sequence contains invalid number '666' at 8 characters read / 6 in SGR sequence which begun at 2 characters read."
got=$(printf '%s' "$str" | ./ansi2html -p vga 2>&1 || true)
str_eq_html "$str" "$want" "$got"

# Nor other characters than 0-9 or semicolons (yet)
str=$'\e[0;Xm'
want="ERROR: SGR sequence contains invalid character 'X' at 5 characters read / 3 in SGR sequence which begun at 2 characters read."
got=$(printf '%s' "$str" | ./ansi2html -p vga 2>&1 || true)
str_eq_html "$str" "$want" "$got"

# HTML-unsafe characters (<>&) are escaped
str=$'<>&"\x27'
want=$'&lt;&gt;&amp;"\x27'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# A lone _ending_ ESC gets replaced with the Unicode symbol for ESC, in HTML
# decimal entities format: ␛ U+241B Symbol For Escape Unicode Character
str=$'Foo \e'
want=$'Foo &#9243;'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# An ESC not followed by "[" is output "as-is" (with the ESC replaced by the
# HTML decimal entity format for U+241B), but with HTML entities after it
# properly escaped.
str=$'Foo \e&bar'
want=$'Foo &#9243;&amp;bar'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

done_testing
