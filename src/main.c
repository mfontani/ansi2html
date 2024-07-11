#include "ansi2html.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fputc_unlocked etc. aren't available, seemingly, under musl.
// #ifdef NO_UNLOCKED_STDIO
#define PUTCHAR(c) (void)fputc(c, stdout)
#define FPUTS(s, fd) (void)fputs(s, fd)
// #else
// #define PUTCHAR(c) (void)fputc_unlocked(c, stdout)
// #define FPUTS(s, fd) (void)fputs_unlocked(s, fd)
// #endif

#define USAGE_FMT                                                              \
    "Usage: %s [options]\n"                                                    \
    "Options:\n"                                                               \
    "  --help                   Show this help.\n"                             \
    "  --palette, -p <name>     Use the named palette. Default is vga.\n"      \
    "  --rgb-for <0-16>         Show the #RRGGBB for the given palette.\n"     \
    "  --showcase-palette       Show the palette in a table.\n"                \
    "  --show-style-tag         Output the style tag contents.\n"              \
    "                           Useful before using --use-classes.\n"          \
    "                           See also --bold-is-bright.\n"                  \
    "  --use-classes            Use CSS classes where possible.\n"             \
    "                           Default is to use inline styles.\n"            \
    "                           Use --show-style-tag first, to output it.\n"   \
    "  --bold-is-bright, -b     A bold color is a bright color.\n"             \
    "  --pre                    Wrap the output in a <pre> block, using the\n" \
    "                           default foreground and background colors.\n"   \
    "  --pre-fg-color <color>   Override the foreground color for the pre.\n"  \
    "                           Only usable with --pre.\n"                     \
    "  --pre-bg-color <color>   Override the background color for the pre.\n"  \
    "                           Only usable with --pre.\n"                     \
    "  --pre-add-style <style>  Add the style to the pre.\n"                   \
    "                           Only usable with --pre.\n"                     \
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
    // If you want to wrap it in a <pre>, would you like to override the
    // default foreground color to something else?
    char *pre_fg_color = NULL;
    // ... Or would you like to override the default background color to
    // something else?
    char *pre_bg_color = NULL;
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
        else if (strcmp(argv[i], "--rgb-for") == 0)
        {
            if (i + 1 < argc)
            {
                i++;
                long color = strtol(argv[i], NULL, 10);
                if (color >= 0 && color <= 15)
                {
                    if (!palette)
                    {
                        (void)fprintf(
                            stderr,
                            "Error: Need to provide a palette with --palette "
                            "before using --rgb-for.\n"
                        );
                        SHOW_VALID_PALETTES();
                        SHOW_USAGE();
                        exit(1);
                    }
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
                        stderr, "Error: Invalid color '%s' needs to be 0-15.\n",
                        argv[i]
                    );
                    SHOW_USAGE();
                    exit(1);
                }
            }
            else
            {
                (void
                )fprintf(stderr, "Error: Missing argument to '--rgb-for'.\n");
                SHOW_USAGE();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--showcase-palette") == 0)
        {
            if (!palette)
            {
                (void)fprintf(
                    stderr,
                    "Error: Need to provide a palette with --palette before "
                    "using --showcase-palette.\n"
                );
                SHOW_VALID_PALETTES();
                SHOW_USAGE();
                exit(1);
            }
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
        else if (strcmp(argv[i], "--pre-fg-color") == 0)
        {
            if (!wrap_in_pre)
            {
                (void)fprintf(
                    stderr, "Error: Need to use --pre before --pre-fg-color.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
            if (i + 1 < argc)
            {
                i++;
                pre_fg_color = argv[i];
            }
            else
            {
                (void)fprintf(
                    stderr, "Error: Missing argument to '--pre-fg-color'.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--pre-bg-color") == 0)
        {
            if (!wrap_in_pre)
            {
                (void)fprintf(
                    stderr, "Error: Need to use --pre before --pre-bg-color.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
            if (i + 1 < argc)
            {
                i++;
                pre_bg_color = argv[i];
            }
            else
            {
                (void)fprintf(
                    stderr, "Error: Missing argument to '--pre-bg-color'.\n"
                );
                SHOW_USAGE();
                exit(1);
            }
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
                palette->base[7].red, palette->base[7].green,
                palette->base[7].blue, palette->base[0].red,
                palette->base[0].green, palette->base[0].blue
            );
            printf(
                ".ansi2html .fg-default{color:#%02X%02X%02X;}",
                palette->base[7].red, palette->base[7].green,
                palette->base[7].blue
            );
            printf(
                ".ansi2html .bg-default{background-color:#%02X%02X%02X;}",
                palette->base[0].red, palette->base[0].green,
                palette->base[0].blue
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
            FPUTS(".ansi2html .bold{font-weight:bold;}", stdout);
            if (style.bold_is_bright)
                for (int j = 0; j < 8; j++)
                {
                    struct ansi_rgb rgb = {0};
                    ansi256_to_rgb(j + 8, palette, &rgb);
                    printf(
                        ".ansi2html "
                        ".bold.fg-%d{font-weight:initial;color:#%02X%02X%02X;}",
                        j, rgb.red, rgb.green, rgb.blue
                    );
                }
            FPUTS(".ansi2html .faint{opacity:0.67;}", stdout);
            FPUTS(".ansi2html .italic{font-style:italic;}", stdout);
            FPUTS(".ansi2html .underline{text-decoration:underline;}", stdout);
            printf(".ansi2html "
                   ".double-underline{text-decoration:underline;border-bottom:"
                   "3px double;}");
            FPUTS(".ansi2html .slow-blink{text-decoration:blink;}", stdout);
            FPUTS(".ansi2html .fast-blink{text-decoration:blink;}", stdout);
            FPUTS(
                ".ansi2html .crossout{text-decoration:line-through;}", stdout
            );
            FPUTS(".ansi2html .fraktur{font-family:fraktur;}", stdout);
            FPUTS(".ansi2html .frame{border:1px solid;}", stdout);
            FPUTS(
                ".ansi2html .circle{border:1px solid;border-radius:50%%;}",
                stdout
            );
            FPUTS(".ansi2html .overline{text-decoration:overline;}", stdout);
            exit(0);
        }
        else
        {
            (void)fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
            SHOW_USAGE();
            exit(1);
        }
    }

    if (!palette)
    {
        (void
        )fprintf(stderr, "Error: Need to provide a palette with --palette.\n");
        SHOW_VALID_PALETTES();
        SHOW_USAGE();
        exit(1);
    }

    if (wrap_in_pre)
    {
        printf("<pre %s style=\"", use_classes ? "class=\"ansi2html\"" : "");
        if (pre_fg_color && pre_bg_color)
            printf("color:%s;background-color:%s;", pre_fg_color, pre_bg_color);
        else if (pre_fg_color)
            printf(
                "color:%s;background-color:#%02X%02X%02X;", pre_fg_color,
                palette->base[0].red, palette->base[0].green,
                palette->base[0].blue
            );
        else if (pre_bg_color)
            printf(
                "color:#%02X%02X%02X;background-color:%s;",
                palette->base[7].red, palette->base[7].green,
                palette->base[7].blue, pre_bg_color
            );
        else
            printf(
                "color:#%02X%02X%02X;background-color:#%02X%02X%02X;",
                palette->base[7].red, palette->base[7].green,
                palette->base[7].blue, palette->base[0].red,
                palette->base[0].green, palette->base[0].blue
            );
        if (pre_add_style)
            printf("%s", pre_add_style);
        FPUTS("\">", stdout);
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
            FPUTS(span, stdout);                                               \
            span_outputted = true;                                             \
        }                                                                      \
    } while (0)

    // Read STDIN, and convert ANSI to HTML:
    int c;
    size_t read = 0;
    while ((c = getchar()) != EOF)
    {
        read++;
        unsigned char sgr_chars[256];
        size_t sgr_chars_len = 0;
        unsigned char current_sgr[128];
        size_t current_sgr_len = 0;
        unsigned char sgrs[128];
        size_t sgrs_len = 0;
        long current_sgr_value = 0L;
        switch (c)
        {
        case '\033':
            // The sequence should start with '[':
            c = getchar();
            if (c == EOF)
            {
                OUTPUT_SPAN_IF_NEEDED();
                FPUTS("&#9243;", stdout);
                break;
            }
            read++;
            if (c != '[')
            {
                OUTPUT_SPAN_IF_NEEDED();
                FPUTS("&#9243;", stdout);
                if (c == '<')
                    FPUTS("&lt;", stdout);
                else if (c == '>')
                    FPUTS("&gt;", stdout);
                else if (c == '&')
                    FPUTS("&amp;", stdout);
                else
                    PUTCHAR(c);
                break;
            }
            // We have an escape sequence. Read until 'm':
            sgr_chars[0] = '\0';
            sgr_chars_len = 0;
            size_t begun_at = read;
            while ((c = getchar()) != EOF)
            {
                read++;
                if (c == 'm')
                {
                    // We have the end of the SGR sequence.
                    // sgr_chars[sgr_chars_len] = '\0';
                    // "convert" the last number:
                    if (current_sgr_len > 0)
                    {
                        current_sgr[current_sgr_len] = '\0';
                        current_sgr_value =
                            strtol((char *)current_sgr, NULL, 10);
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
                if (sgr_chars_len < sizeof(sgr_chars) - 1)
                {
                    // The character should be a digit, or semicolon.
                    if (c >= '0' && c <= '9')
                        current_sgr[current_sgr_len++] = (unsigned char)c;
                    else if (c == ';')
                    {
                        current_sgr[current_sgr_len] = '\0';
                        current_sgr_value =
                            strtol((char *)current_sgr, NULL, 10);
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
                else
                {
                    (void)fprintf(
                        stderr,
                        "Error: SGR sequence too long, at %zu characters read "
                        "/ %zu in SGR sequence which begun at %zu characters "
                        "read.\n",
                        read, sgr_chars_len, begun_at
                    );
                    exit(1);
                }
            }
            set_ansi_style_properties(palette, &style, sgrs, sgrs_len);
            if (span_outputted)
                FPUTS("</span>", stdout);
            span = ansi_span_start(&style, palette, use_classes);
            span_outputted = false;
            break;
        default:
            OUTPUT_SPAN_IF_NEEDED();
            if (c == '&')
                FPUTS("&amp;", stdout);
            else if (c == '<')
                FPUTS("&lt;", stdout);
            else if (c == '>')
                FPUTS("&gt;", stdout);
            else
                PUTCHAR(c);
            break;
        }
    }

    if (span_outputted)
        FPUTS("</span>", stdout);

    if (wrap_in_pre)
        FPUTS("</pre>", stdout);

    return 0;
}
