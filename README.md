# ansi2html

A C program that converts (most) ANSI color escape (16, 256, and 24-bit) foreground and background color, and "style" codes to HTML.

## How to install

There are two binary versions on the [releases page](https://github.com/mfontani/ansi2html/releases):
- `ansi2html`, which contains all base palettes _and_ palettes imported from [the iTerm2-Color-Schemes repo](https://github.com/mbadolato/iTerm2-Color-Schemes/tree/master/Xresources) (with an `i2_` prefix)
- `ansi2html-noi2`, which contains only the base palettes, and is smaller (if you know all colors in the palette, can use the `--rgb-for` option multiple times, to customize the foreground, background and all base 16 colors)

You can download a pre-packaged binary from the
[releases page](https://github.com/mfontani/ansi2html/releases),
where binaries for both Linux, Windows, and MacOS x86_64 and arm64 are available,
or use a tool such as [ubi](https://github.com/houseabsolute/ubi) to fetch it
and install it to your local `$HOME/bin` (assuming it's in your `$PATH`, it'll
Just Work®).

```bash
$ ubi --project mfontani/ansi2html --in "$HOME/bin"
$ printf '\e[31mred!\e[0m\n' | ansi2html
```

## Usage

```bash
ansi2html < input.txt > output.html
```

Or, if you'd like to strip ANSI color codes instead:

```bash
ansi2html --strip < input.txt > output.txt
```

See `ansi2html --help` for more options. Options' order matters.

You can choose a number of different palettes, which can be given as a parameter to the program. List the available ones with `ansi2html --list-palettes`. The default palette is `vga`.

To get a nice preview of some/all the palettes, you might want to run something like:

```bash
ansi2html --list-palettes | \
    grep -e kanaga -e dracula | \
    xargs -I {} sh -c 'echo ; printf "%s\n" "{}" ; ansi2html --palette {} --rgb-for bg 000000 --showcase-palette'
```

You can further customize the palette (i.e. pick a palette, then customize a few colors; or use the default `vga` palette, and customize _all_ the colors) using the `--rgb-for <0-16,fg,bg> RRGGBB` option (e.g. `--rgb-for 0 101010` to customize the color 0 to be a dark black, or `--rgb-for fg 406040` to make the default foreground text a dark green).
Note that only the first sixteen colors (dark 0-7 and bright 8-15), the default foreground, and the default background colors can be customized. The rest of the 256 colors (16-255) or indeed the 24-bit colors cannot be customized.

You can optionally enable the "bold is bright" feature with `--bold-is-bright` (or `-b`), which will make bold text for the first 7 base colors (i.e. for `\e[0;1;30m` through `\e[0;1;37m`) appear as bright text (i.e. `\e[0;90m` through `\e[0;97m`), as if the brighter colors were used for the foreground). This feature is disabled by default.

The markup outputted can be wrapped in a `pre` tag, by using the `--pre` option. If used, it will contain a `style` attribute, containing the default foreground and background colors' specification for the palette. If you wish to change those, use the `--rgb-for` option. If you need to add more styles to the pre tag, you can use the `--pre-add-style` option (i.e. `--pre-add-style 'padding:0.5em;'`).

You can use the `--palette NAME --show-rgb-for <0-16,fg,bg>` option to show the RGB color for the given 0-16, foreground or background color. Or `--palette NAME --showcase-palette` to see the entire (foreground, background, and base 16 colors for a) palette.

By default, the program uses _in-line style attributes_ for the colors. If you want to use CSS classes instead, use the `--use-classes` option. You can output the CSS contents that should go in the `style` tag with the `--show-style-tag` option. Note that 24-bit colors will always use in-line style attributes, even if `--use-classes` is used.

Example using classes:

```bash
(
    printf '<!DOCTYPE html><head><title>...</title>\n';
    printf '<style>\n';
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

You can likely get away with something like this:

```bash
env NO_ASAN=1 make ansi2html
env NO_ASAN=1 make CC=clang ansi2html
```

For the `-S` option (strip ANSI codes), on x86_64 an AVX2 or SSE2 version is available when compiled with `-march=native`.
To pick a specific implementation (which can only be compiled if possible), pass on `-DWANT_SSE2` or `-DWANT_AVX2` in `CC=` or `DEFINES`.
Alternatively, use `-DWANT_DEFAULT` to use the default implementation (i.e. without any forced intrinsics).

## How to compile (developer)

Run `make CC=gcc-14 all`. It'll:

- format the code (you'll need `clang-format` installed)
- create the tags file (you'll need `ctags` installed)
- compile the program with `gcc-14` and with ASAN enabled

Else, use the `CC` variable to specify the compiler you want to use, the `NO_ASAN=1` variable to disable Address Sanitizer, and the `ansi2html` target if you just want the executable.

## License

The MIT License (MIT)

Copyright (c) 2024-2025 Marco Fontani, mf@marcofontani.it

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

But see also [src/iterm2_color_schemes](src/iterm2_color_schemes/README.md) for the licenses of the palettes imported from the iTerm2-Color-Schemes repo.
