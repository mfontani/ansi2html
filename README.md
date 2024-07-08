# ansi2html

A C program that converts (most) ANSI color escape (16, 256, and 24-bit) foreground and background color, and "style" codes to HTML.

## Usage

```bash
ansi2html --palette vga < input.txt > output.html
```

See `ansi2html --help` for more options.

You can choose a number of different palettes, which can be given as a parameter to the program. List the available ones with `ansi2html --palette`.

You can optionally enable the "bold is bright" feature with `--bold-is-bright`, which will make bold text (i.e. for `\e[0;1;31m` appear as bright (i.e. `\e[0;91m`) text. This is disabled by default.

The markup can be wrapped in a `pre` tag, if the `--pre` option is given.

If `--pre` is enabled, it will add the default foreground and background color to the tag. If you wish to set different ones, use the `--pre-fg-color` and `--pre-bg-color` options. If you moreover want to add more styles to the pre tag, use the `--pre-add-style` option.

You can use the `--palette NAME --rgb-for 0-16` option to show the RGB color for the given 0-16 color.

By default, the program uses in-line styles for the colors. If you want to use CSS classes instead, use the `--use-classes` option. You can output the CSS contents that should go in the `style` tag with the `--show-style-tag` option.

Example:

```bash
(
    printf '<!DOCTYPE html><head><title>...</title>\n';
    printf '<style>\n;
    ansi2html --palette vga --show-style-tag;
    printf '\n/* Additional styles... */\n';
    printf '</style>\n';
    printf '</head><body><!-- additional markup -->\n';
    ansi2html --palette vga --use-classes --pre < input.txt
    printf '<!-- more markup -->\n';
    printf '</body></html>\n';
) > output.html
```

## How to compile (user)

You can likely get away with something like this, until I get automations working:

```bash
env NO_ASAN=1 make ansi2html
env NO_ASAN=1 make CC=clang ansi2html
```

## How to compile (developer)

Run `make CC=gcc-14 all`. It'll:

- format the code (you'll need `clang-format` installed)
- create the tags file (you'll need `ctags` installed)
- compile the program with `gcc-14` and with ASAN enabled

Else, use the `CC` variable to specify the compiler you want to use, the `NO_ASAN=1` variable to disable Address Sanitizer, and the `ansi2html` target if you just want the executable.

## License

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, mf@marcofontani.it

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
