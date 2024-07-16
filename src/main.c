#include "ansi2html.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ITERM2_COLOR_SCHEMES
#define WITH_ITERM2_COLOR_SCHEMES "Has"
#else
#define WITH_ITERM2_COLOR_SCHEMES "Does NOT have"
#endif

#define USAGE_FMT                                                              \
    "Usage: %s [options]\n"                                                    \
    "Options:\n"                                                               \
    "  --help                   Show this help.\n"                             \
    "  --palette, -p <name>     Use the named palette. Default is vga.\n"      \
    "       " WITH_ITERM2_COLOR_SCHEMES " iTerm2-Color-Schemes palettes.\n"    \
    "  --bold-is-bright, -b     A bold color is a bright color.\n"             \
    "  --rgb-for <0-16,fg,bg> RRGGBB\n"                                        \
    "      Override the base 16 colors, or the foreground or background\n"     \
    "      color, with #RRGGBB.\n"                                             \
    "  --show-rgb-for <0-16,fg,bg>  Show the #RRGGBB for the given palette.\n" \
    "  --showcase-palette       Show the palette in a table.\n"                \
    "  --show-style-tag         Output the style tag contents.\n"              \
    "       Useful before using --use-classes. See also --bold-is-bright.\n"   \
    "  --use-classes            Use CSS classes where possible.\n"             \
    "       Default is to use inline styles.\n"                                \
    "  --pre                    Wrap the output in a <pre> block, using the\n" \
    "       default foreground and background colors.\n"                       \
    "  --pre-add-style <style>  Add the style to the pre.\n"                   \
    "       Only usable with --pre.\n"                                         \
    "Note that the order of the options is important.\n"                       \
    ""

int main(int argc, char *argv[])
{
#define SHOW_USAGE()                                                           \
    do                                                                         \
    {                                                                          \
        (void)fprintf(stderr, USAGE_FMT, argv[0]);                             \
    } while (0)
    struct named_palettes
    {
        const char *name;
        struct ansi_color_palette *palette;
    };
    struct named_palettes named_palettes[] = {
        {"vga",              PALETTE_VGA             },
        {"winxp-console",    PALETTE_WINXP_CONSOLE   },
        {"powershell",       PALETTE_POWERSHELL      },
        {"vscode",           PALETTE_VSCODE          },
        {"win10-console",    PALETTE_WIN10_CONSOLE   },
        {"terminal-app",     PALETTE_TERMINAL_APP    },
        {"putty",            PALETTE_PUTTY           },
        {"mirc",             PALETTE_MIRC            },
        {"xterm",            PALETTE_XTERM           },
        {"ubuntu",           PALETTE_UBUNTU          },
        {"eclipse-terminal", PALETTE_ECLIPSE_TERMINAL},
        {"dracula",          PALETTE_DRACULA         },
#ifdef ITERM2_COLOR_SCHEMES
#include "iterm2_color_schemes/named_palettes.h"
#endif
    };
#define SHOW_VALID_PALETTES()                                                  \
    do                                                                         \
    {                                                                          \
        (void)fprintf(stderr, "Valid palettes:\n");                            \
        for (size_t j = 0;                                                     \
             j < sizeof(named_palettes) / sizeof(named_palettes[0]); j++)      \
            (void)fprintf(stderr, "- %s\n", named_palettes[j].name);           \
    } while (0)

    // We start with a "reset" style:
    struct ansi_style style = {0};
    // Need to provide a known palette. This is the default:
    struct ansi_color_palette *palette = PALETTE_VGA;

    // Want this wrapped in a "pre" block?
    bool wrap_in_pre = false;
    // And/or would you like some additional style to be added to the pre?
    char *pre_add_style = NULL;
    // Do you want to use CSS classes, or in-line styles?
    bool use_classes = false;

    for (int i = 1; i < argc; i++)
    {
        if ((strcmp(argv[i], "--palette") == 0) || (strcmp(argv[i], "-p") == 0))
        {
            if (i + 1 < argc)
            {
                i++;
                palette = NULL;
                for (size_t j = 0;
                     j < sizeof(named_palettes) / sizeof(named_palettes[0]);
                     j++)
                {
                    if (strcmp(argv[i], named_palettes[j].name) == 0)
                    {
                        palette = named_palettes[j].palette;
                        reset_ansi_props(&style, palette);
                        break;
                    }
                }
                if (!palette)
                {
                    (void
                    )fprintf(stderr, "Error: Unknown palette '%s'.\n", argv[i]);
                    SHOW_VALID_PALETTES();
                    SHOW_USAGE();
                    exit(1);
                }
            }
            else
            {
                (void
                )fprintf(stderr, "Error: Missing argument to '--palette'.\n");
                SHOW_VALID_PALETTES();
                SHOW_USAGE();
                exit(1);
            }
            continue;
        }
        else if (strcmp(argv[i], "--help") == 0)
        {
            SHOW_USAGE();
            exit(0);
        }
        else if (strcmp(argv[i], "--show-rgb-for") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                if (!strcmp(argv[i], "fg"))
                {
                    (void)printf(
                        "#%02X%02X%02X\n", palette->default_fg.red,
                        palette->default_fg.green, palette->default_fg.blue
                    );
                    exit(0);
                }
                else if (!strcmp(argv[i], "bg"))
                {
                    (void)printf(
                        "#%02X%02X%02X\n", palette->default_bg.red,
                        palette->default_bg.green, palette->default_bg.blue
                    );
                    exit(0);
                }
                char *endptr = NULL;
                long color = strtol(argv[i], &endptr, 10);
                if (!endptr || *endptr != '\0')
                {
                    (void)fprintf(
                        stderr,
                        "Error: Invalid color '%s' needs to be 0-15 or fg or "
                        "bg.\n",
                        argv[i]
                    );
                    SHOW_USAGE();
                    exit(1);
                }
                if (color >= 0 && color <= 15)
                {
                    struct ansi_rgb rgb = {0};
                    if (color < 8)
                        rgb = palette->base[color];
                    else
                        rgb = palette->bright[color - 8];
                    (void
                    )printf("#%02X%02X%02X\n", rgb.red, rgb.green, rgb.blue);
                    exit(0);
                }
                else
                {
                    (void)fprintf(
                        stderr,
                        "Error: Invalid color '%s' needs to be 0-15 or fg or "
                        "bg.\n",
                        argv[i]
                    );
                    SHOW_USAGE();
                    exit(1);
                }
            }
            else
            {
                (void)fprintf(
                    stderr, "Error: Missing argument to '--show-rgb-for'.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--showcase-palette") == 0)
        {
            showcase_palette(palette);
            exit(0);
        }
        else if ((strcmp(argv[i], "--bold-is-bright") == 0) ||
                 (strcmp(argv[i], "-b") == 0))
        {
            style.bold_is_bright = true;
        }
        else if (strcmp(argv[i], "--pre") == 0)
        {
            wrap_in_pre = true;
        }
        else if (strcmp(argv[i], "--pre-add-style") == 0)
        {
            if (!wrap_in_pre)
            {
                (void)fprintf(
                    stderr, "Error: Need to use --pre before --pre-add-style.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
            if (i + 1 < argc)
            {
                i++;
                pre_add_style = argv[i];
            }
            else
            {
                (void)fprintf(
                    stderr, "Error: Missing argument to '--pre-add-style'.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--use-classes") == 0)
        {
            use_classes = true;
        }
        else if (strcmp(argv[i], "--show-style-tag") == 0)
        {
            printf(
                ".ansi2html{"
                "color:#%02X%02X%02X;background-color:#%02X%02X%02X;"
                "}",
                palette->default_fg.red, palette->default_fg.green,
                palette->default_fg.blue, palette->default_bg.red,
                palette->default_bg.green, palette->default_bg.blue
            );
            printf(
                ".ansi2html .fg-default{color:#%02X%02X%02X;}",
                palette->default_fg.red, palette->default_fg.green,
                palette->default_fg.blue
            );
            printf(
                ".ansi2html .bg-default{background-color:#%02X%02X%02X;}",
                palette->default_bg.red, palette->default_bg.green,
                palette->default_bg.blue
            );
            for (int j = 0; j < 256; j++)
            {
                struct ansi_rgb rgb = {0};
                ansi256_to_rgb(j, palette, &rgb);
                printf(
                    ".ansi2html .fg-%d{color:#%02X%02X%02X;}", j, rgb.red,
                    rgb.green, rgb.blue
                );
                printf(
                    ".ansi2html .bg-%d{background-color:#%02X%02X%02X;}", j,
                    rgb.red, rgb.green, rgb.blue
                );
            }
            show_additional_styles();
            exit(0);
        }
        else if (!strcmp(argv[i], "--rgb-for"))
        {
            struct ansi_rgb *which_rgb = NULL;
            if (i + 1 < argc)
            {
                i++;
                if (!strcmp(argv[i], "fg"))
                    which_rgb = &palette->default_fg;
                else if (!strcmp(argv[i], "bg"))
                    which_rgb = &palette->default_bg;
                else
                {
                    char *endptr = NULL;
                    long color = strtol(argv[i], &endptr, 10);
                    if (!endptr || *endptr != '\0')
                    {
                        (void)fprintf(
                            stderr,
                            "Error: Invalid color '%s' needs to be 0-15 or fg "
                            "or bg.\n",
                            argv[i]
                        );
                        SHOW_USAGE();
                        exit(1);
                    }
                    if (color < 8)
                        which_rgb = &palette->base[color];
                    else if (color < 16)
                        which_rgb = &palette->bright[color - 8];
                }
                if (!which_rgb)
                {
                    (void)fprintf(
                        stderr,
                        "Error: Invalid color '%s' needs to be 0-15 or fg or "
                        "bg.\n",
                        argv[i]
                    );
                    SHOW_USAGE();
                    exit(1);
                }
                if (i + 1 < argc)
                {
                    i++;
                    // The color should be in the format #RRGGBB or RRGGBB:
                    char *color = argv[i];
                    if (color[0] == '#')
                        color++;
                    if (strlen(color) != 6)
                    {
                        (void)fprintf(
                            stderr,
                            "Error: Invalid color '%s' needs to be #RRGGBB.\n",
                            argv[i]
                        );
                        SHOW_USAGE();
                        exit(1);
                    }
                    char *endptr = NULL;
                    long lrgb = strtol(color, &endptr, 16);
                    if (!endptr || *endptr != '\0')
                    {
                        (void)fprintf(
                            stderr,
                            "Error: Invalid color '%s' needs to be #RRGGBB.\n",
                            argv[i]
                        );
                        SHOW_USAGE();
                        exit(1);
                    }
                    if (lrgb < 0 || lrgb > 0xFFFFFF)
                    {
                        (void)fprintf(
                            stderr,
                            "Error: Invalid color '%s' needs to be #RRGGBB.\n",
                            argv[i]
                        );
                        SHOW_USAGE();
                        exit(1);
                    }
                    unsigned int rgb = (unsigned int)lrgb;
                    which_rgb->red = (rgb >> 16) & 0xFF;
                    which_rgb->green = (rgb >> 8) & 0xFF;
                    which_rgb->blue = rgb & 0xFF;
                }
                else
                {
                    (void)fprintf(
                        stderr, "Error: Missing argument 2 for '--rgb-for'.\n"
                    );
                    SHOW_USAGE();
                    exit(1);
                }
            }
            else
            {
                (void)fprintf(
                    stderr, "Error: Missing argument 1 for '--rgb-for'.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
#undef CHECK_COLOR_ARG
        }
        else
        {
            (void)fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
            SHOW_USAGE();
            exit(1);
        }
    }

    if (wrap_in_pre)
    {
        printf("<pre %sstyle=\"", use_classes ? "class=\"ansi2html\" " : "");
        printf(
            "color:#%02X%02X%02X;background-color:#%02X%02X%02X;",
            palette->default_fg.red, palette->default_fg.green,
            palette->default_fg.blue, palette->default_bg.red,
            palette->default_bg.green, palette->default_bg.blue
        );
        if (pre_add_style)
            PUTS(pre_add_style);
        PUTS("\">");
    }

    // We "keep" the span "declaration" for the current chunk of text, so we
    // can output it if we need to, and close it before opening a new one:
    const char *span = NULL;
    // We keep track of whether we have outputted a span, so we can close it
    // or output it the first time:
    bool span_outputted = false;

#define OUTPUT_SPAN_IF_NEEDED()                                                \
    do                                                                         \
    {                                                                          \
        if (span && !span_outputted)                                           \
        {                                                                      \
            PUTS(span);                                                        \
            span_outputted = true;                                             \
        }                                                                      \
    } while (0)

    // Read STDIN, and convert ANSI to HTML:
    int c;
    size_t read = 0;
    while ((c = GETCHAR()) != EOF)
    {
        read++;
        size_t sgr_chars_len = 0;
        size_t current_sgr_len = 0;
        unsigned char sgrs[128];
        size_t sgrs_len = 0;
        long current_sgr_value = 0L;
        switch (c)
        {
        case '\033':
            // The sequence should start with '[':
            c = GETCHAR();
            if (c == EOF)
            {
                OUTPUT_SPAN_IF_NEEDED();
                PUTS("&#9243;");
                break;
            }
            read++;
            if (c != '[')
            {
                OUTPUT_SPAN_IF_NEEDED();
                PUTS("&#9243;");
                if (c == '<')
                    PUTS("&lt;");
                else if (c == '>')
                    PUTS("&gt;");
                else if (c == '&')
                    PUTS("&amp;");
                else
                    PUTC(c);
                break;
            }
            // We have an escape sequence. Read until 'm':
            sgr_chars_len = 0;
            size_t begun_at = read;
            while ((c = GETCHAR()) != EOF)
            {
                read++;
                sgr_chars_len++;
                if (c == 'm')
                {
                    // We have the end of the SGR sequence.
                    if (current_sgr_len > 0)
                    {
                        if (current_sgr_value >= 0 && current_sgr_value <= 255)
                        {
                            unsigned char sgr_value =
                                (unsigned char)current_sgr_value;
                            if (sgrs_len < sizeof(sgrs) - 1)
                                sgrs[sgrs_len++] = sgr_value;
                            else
                            {
                                (void)fprintf(
                                    stderr,
                                    "Error: SGR sequence too long, at %zu "
                                    "characters read / %zu in SGR sequence "
                                    "which begun at %zu characters read.\n",
                                    read, sgr_chars_len, begun_at
                                );
                                exit(1);
                            }
                            break;
                        }
                        (void)fprintf(
                            stderr,
                            "Error: SGR sequence contains invalid number '%ld' "
                            "at %zu characters read / %zu in SGR sequence "
                            "which begun at %zu characters read.\n",
                            current_sgr_value, read, sgr_chars_len, begun_at
                        );
                        exit(1);
                    }
                    break;
                }
                // The character should be a digit, or semicolon.
                if (c >= '0' && c <= '9')
                {
                    long prev = current_sgr_value;
                    current_sgr_value *= 10;
                    current_sgr_value += c - '0';
                    if (prev > current_sgr_value)
                    {
                        (void)fprintf(
                            stderr,
                            "Error: SGR sequence contains invalid number '%ld' "
                            "at %zu characters read / %zu in SGR sequence "
                            "which begun at %zu characters read.\n",
                            current_sgr_value, read, sgr_chars_len, begun_at
                        );
                        exit(1);
                    }
                    current_sgr_len++;
                }
                else if (c == ';')
                {
                    if (current_sgr_value >= 0 && current_sgr_value <= 255)
                    {
                        unsigned char sgr_value =
                            (unsigned char)current_sgr_value;
                        if (sgrs_len < sizeof(sgrs) - 1)
                            sgrs[sgrs_len++] = sgr_value;
                        else
                        {
                            (void)fprintf(
                                stderr,
                                "Error: SGR sequence too long, at %zu "
                                "characters read / %zu in SGR sequence "
                                "which begun at %zu characters read.\n",
                                read, sgr_chars_len, begun_at
                            );
                            exit(1);
                        }
                        current_sgr_len = 0;
                        current_sgr_value = 0;
                    }
                    else
                    {
                        (void)fprintf(
                            stderr,
                            "Error: SGR sequence contains invalid number "
                            "'%ld' at %zu characters read / %zu in SGR "
                            "sequence which begun at %zu characters "
                            "read.\n",
                            current_sgr_value, read, sgr_chars_len, begun_at
                        );
                        exit(1);
                    }
                }
                else
                {
                    (void)fprintf(
                        stderr,
                        "Error: SGR sequence contains invalid character "
                        "'%c' at %zu characters read / %zu in SGR sequence "
                        "which begun at %zu characters read.\n",
                        c, read, sgr_chars_len, begun_at
                    );
                    exit(1);
                }
            }
            set_ansi_style_properties(palette, &style, sgrs, sgrs_len);
            if (span_outputted)
                PUTS("</span>");
            span = ansi_span_start(&style, palette, use_classes);
            span_outputted = false;
            break;
        default:
            OUTPUT_SPAN_IF_NEEDED();
            if (c == '&')
                PUTS("&amp;");
            else if (c == '<')
                PUTS("&lt;");
            else if (c == '>')
                PUTS("&gt;");
            else
                PUTC(c);
            break;
        }
    }

    if (span_outputted)
        PUTS("</span>");

    if (wrap_in_pre)
        PUTS("</pre>");

    return 0;
}
