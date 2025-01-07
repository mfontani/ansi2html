#!/bin/bash

set -e

. "$(dirname "$0")/".functions.sh

# Various tests for -S which strips ANSI sequences.
# The input strings are in various "states" of 16-byte and 32-byte alignment,
# to ensure the AVX2 and SSE2 code paths are tested properly.

S08='1_3_5_7X'
S16='123456789_12345A'
S32='123456789_123456789_1234567890A'

# No ANSI sequences, less than 16 bytes.
#     1234567890123
str=$'Hello, world!'
want=$'Hello, world!'
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 16 bytes.
str="${S16}"
want="${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 16 bytes.
str="${S16}x"
want="${S16}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 32 bytes.
str="${S32}"
want="${S32}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 32 bytes.
str="${S32}x"
want="${S32}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 48 bytes.
str="${S32}${S16}"
want="${S32}${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 48 bytes.
str="${S32}${S16}x"
want="${S32}${S16}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 64 bytes.
str="${S32}${S32}"
want="${S32}${S32}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 64 bytes.
str="${S32}${S32}x"
want="${S32}${S32}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 80 bytes.
str="${S32}${S32}${S16}"
want="${S32}${S32}${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 80 bytes.
str="${S32}${S32}${S16}x"
want="${S32}${S32}${S16}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 96 bytes.
str="${S32}${S32}${S32}"
want="${S32}${S32}${S32}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 96 bytes.
str="${S32}${S32}${S32}x"
want="${S32}${S32}${S32}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 112 bytes.
str="${S32}${S32}${S32}${S16}"
want="${S32}${S32}${S32}${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 112 bytes.
str="${S32}${S32}${S32}${S16}x"
want="${S32}${S32}${S32}${S16}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, exactly 128 bytes.
str="${S32}${S32}${S32}${S32}"
want="${S32}${S32}${S32}${S32}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# No ANSI sequences, just a smidgen more than 128 bytes.
str="${S32}${S32}${S32}${S32}x"
want="${S32}${S32}${S32}${S32}x"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, less than 16 bytes.
str=$(printf '\e[31mHello, world!\e[0m')
want=$'Hello, world!'
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 16 bytes boundary:
str=$(printf '%s\e[31m%s' "${S08}" "${S08}")
want="${S08}${S08}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 32 bytes boundary:
str=$(printf '%s\e[31m%s' "${S16}" "${S16}")
want="${S16}${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 48 bytes boundary:
str=$(printf '%s\e[31m%s' "${S32}" "${S16}")
want="${S32}${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 64 bytes boundary:
str=$(printf '%s%s\e[31m%s' "${S32}" "${S32}" "${S16}")
want="${S32}${S32}${S16}"
got=$(printf '%s' "$str" | ./ansi2html -S)
str_eq_html "$str" "$want" "$got"

########################################################

# Similar, but without -S.

# Add some ANSI into the mix, less than 16 bytes.
str=$(printf '\e[31mHello, world!\e[0m')
want=$'<span style="color:#AA0000;">Hello, world!</span>'
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 16 bytes boundary:
str=$(printf '%s\e[31m%s' "${S08}" "${S08}")
want=$(printf '%s<span style="color:#AA0000;">%s</span>' "${S08}" "${S08}")
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 32 bytes boundary:
str=$(printf '%s\e[31m%s' "${S16}" "${S16}")
want=$(printf '%s<span style="color:#AA0000;">%s</span>' "${S16}" "${S16}")
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 48 bytes boundary:
str=$(printf '%s\e[31m%s' "${S32}" "${S16}")
want=$(printf '%s<span style="color:#AA0000;">%s</span>' "${S32}" "${S16}")
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

# Add some ANSI into the mix, "straddling" the 64 bytes boundary:
str=$(printf '%s%s\e[31m%s' "${S32}" "${S32}" "${S16}")
want=$(printf '%s%s<span style="color:#AA0000;">%s</span>' "${S32}" "${S32}" "${S16}")
got=$(printf '%s' "$str" | ./ansi2html -p vga)
str_eq_html "$str" "$want" "$got"

done_testing
