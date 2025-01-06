#include "ansi2html.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ITERM2_COLOR_SCHEMES
#define WITH_ITERM2_COLOR_SCHEMES "Has"
#else
#define WITH_ITERM2_COLOR_SCHEMES "Does NOT have"
#endif

#ifndef INPUT_BUFFER_SIZE
#define INPUT_BUFFER_SIZE 65536
#endif

#ifndef OUTPUT_BUFFER_SIZE
#define OUTPUT_BUFFER_SIZE 65536
#endif

#ifndef FLUSH_LEEWAY
#define FLUSH_LEEWAY 128
#endif

#define STRNGY_(x) #x
#define STRNGY(x) STRNGY_(x)
#define USAGE_FMT                                                              \
    "Usage: %s [options]\n"                                                    \
    "Options:\n"                                                               \
    "  --help                   Show this help.\n"                             \
    "  --strip, -S              Strip all ANSI, rather than HTML encode.\n"    \
    "  --list-palettes, -l      List the names of valid/known palettes.\n"     \
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
    "  --use-compact            Use compact output where possible.\n"          \
    "       Default is not to. Transforms color:#00ffaa into color:#0fa\n"     \
    "  --pre                    Wrap the output in a <pre> block, using the\n" \
    "       default foreground and background colors.\n"                       \
    "  --pre-add-style <style>  Add the style to the pre.\n"                   \
    "       Only usable with --pre.\n"                                         \
    "Note that the order of the options is important.\n"                       \
    "Buffers: " STRNGY(INPUT_BUFFER_SIZE                                       \
    ) " input buffer size, " STRNGY(OUTPUT_BUFFER_SIZE                         \
    ) " output buffer size.\n"                                                 \
      ""

static char output_buffer[OUTPUT_BUFFER_SIZE];
static size_t output_buffer_idx = 0;
static inline void flush_buffer(void)
{
    if (output_buffer_idx > 0)
    {
        FWRITE(output_buffer, 1, output_buffer_idx, stdout);
        output_buffer_idx = 0;
    }
}
static inline void str_to_buffer(const char *str, size_t len)
{
    if (output_buffer_idx + len + FLUSH_LEEWAY > sizeof(output_buffer))
        flush_buffer();
    // Append.
    memcpy(output_buffer + output_buffer_idx, str, len);
    output_buffer_idx += len;
    output_buffer[output_buffer_idx] = '\0';
}
static inline void append_to_buffer(const char *str)
{
    size_t len = strlen(str);
    if (output_buffer_idx + len + FLUSH_LEEWAY > sizeof(output_buffer))
        flush_buffer();
    // Append.
    memcpy(output_buffer + output_buffer_idx, str, len);
    output_buffer_idx += len;
    output_buffer[output_buffer_idx] = '\0';
}
static inline void char_to_buffer(const unsigned char c)
{
    if (output_buffer_idx + FLUSH_LEEWAY > sizeof(output_buffer))
        flush_buffer();
    output_buffer[output_buffer_idx++] = (char)c;
    output_buffer[output_buffer_idx] = '\0';
}

#define ERROR(fmt, ...)                                                        \
    do                                                                         \
    {                                                                          \
        flush_buffer();                                                        \
        (void)fprintf(stderr, "ERROR: " fmt, ##__VA_ARGS__);                   \
        exit(1);                                                               \
    } while (0)

static inline __attribute__((always_inline)) void just_strip_it(void)
{
    enum parser_state
    {
        STATE_TEXT,
        STATE_GOT_ESC,
        STATE_GOT_ESC_BRACKET,
    } state = STATE_TEXT;

    char buffer[INPUT_BUFFER_SIZE];
    buffer[0] = '\0';
    size_t sgr_chars_len = 0;
    unsigned char sgrs[128];
    size_t sgrs_len = 0;
    long current_sgr_value = 0L;
    size_t read = 0;
    size_t begun_at = read;

    do
    {
        register size_t buffer_len = FREAD(buffer, 1, sizeof(buffer), stdin);
        if (buffer_len == 0)
            break;
        if (buffer_len > sizeof(buffer))
            ERROR(
                "Buffer overflow, read %zu bytes, but buffer is %zu "
                "bytes.\n",
                buffer_len, sizeof(buffer)
            );
        register int buffer_idx = 0;
        while (buffer_idx < (int)buffer_len)
        {
            read++;
            register unsigned char c = buffer[buffer_idx++];
            switch (state)
            {
            default:
                ERROR("Invalid state %d.\n", (int)state);
            case STATE_TEXT:
                if (c == '\033')
                    state = STATE_GOT_ESC;
                else
                    char_to_buffer(c);
                break;
            case STATE_GOT_ESC:
                if (c == '[')
                {
                    state = STATE_GOT_ESC_BRACKET;
                    sgr_chars_len = 0;
                    sgrs_len = 0;
                    current_sgr_value = 0L;
                    begun_at = read;
                }
                else
                {
                    char_to_buffer('\x1b');
                    char_to_buffer(c);
                    state = STATE_TEXT;
                }
                break;
            case STATE_GOT_ESC_BRACKET:
                sgr_chars_len++;
                // Read until "m":
                if (c == 'm')
                {
                    // We have the end of the SGR sequence.
                    state = STATE_TEXT;
                }
                else
                {
                    // The character should be a digit, or semicolon.
                    if (c >= '0' && c <= '9')
                    {
                        long prev = current_sgr_value;
                        current_sgr_value *= 10;
                        current_sgr_value += c - '0';
                        if (prev > current_sgr_value)
                            ERROR(
                                "SGR sequence contains invalid number "
                                "'%ld' "
                                "at %zu characters read / %zu in SGR sequence "
                                "which begun at %zu characters read.\n",
                                current_sgr_value, read, sgr_chars_len, begun_at
                            );
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
                                ERROR(
                                    "SGR sequence too long, at %zu "
                                    "characters read / %zu in SGR sequence "
                                    "which begun at %zu characters read.\n",
                                    read, sgr_chars_len, begun_at
                                );
                            current_sgr_value = 0;
                        }
                        else
                            ERROR(
                                "SGR sequence contains invalid number "
                                "'%ld' at %zu characters read / %zu in SGR "
                                "sequence which begun at %zu characters "
                                "read.\n",
                                current_sgr_value, read, sgr_chars_len, begun_at
                            );
                    }
                    else
                        ERROR(
                            "SGR sequence contains invalid character "
                            "'%c' at %zu characters read / %zu in SGR sequence "
                            "which begun at %zu characters read.\n",
                            c, read, sgr_chars_len, begun_at
                        );
                }
                break;
            }
        }
    } while (1);

    flush_buffer();
    exit(0);
}

#define VRGB(r, g, b) (unsigned int)(r), (unsigned int)(g), (unsigned int)(b)

static inline __attribute__((always_inline)) void ansi2html(
    struct ansi_style style, struct ansi_color_palette *palette,
    bool wrap_in_pre, char *pre_add_style, bool use_classes, bool use_compact
)
{
    if (wrap_in_pre)
    {
        printf("<pre %sstyle=\"", use_classes ? "class=\"ansi2html\" " : "");
        printf(
            "color:#%02X%02X%02X;background-color:#%02X%02X%02X;",
            VRGB(
                palette->default_fg.red, palette->default_fg.green,
                palette->default_fg.blue
            ),
            VRGB(
                palette->default_bg.red, palette->default_bg.green,
                palette->default_bg.blue
            )
        );
        if (pre_add_style)
            append_to_buffer(pre_add_style);
        str_to_buffer("\">", 2);
    }

    // We "keep" the span "declaration" for the current chunk of text, so we
    // can output it if we need to, and close it before opening a new one:
    const char *span = NULL;
    // We keep track of whether we have outputted a span, so we can close it
    // or output it the first time:
    bool span_outputted = false;

    enum parser_state
    {
        STATE_TEXT,
        STATE_GOT_ESC,
        STATE_GOT_ESC_BRACKET,
    } state = STATE_TEXT;

    char buffer[INPUT_BUFFER_SIZE];
    buffer[0] = '\0';
    size_t sgr_chars_len = 0;
    size_t current_sgr_len = 0;
    unsigned char sgrs[128];
    size_t sgrs_len = 0;
    long current_sgr_value = 0L;
    size_t read = 0;
    size_t begun_at = read;

    // Read STDIN, and convert ANSI to HTML:
    do
    {
        register size_t buffer_len = FREAD(buffer, 1, sizeof(buffer), stdin);
        if (buffer_len == 0)
            break;
        if (buffer_len > sizeof(buffer))
            ERROR(
                "Buffer overflow, read %zu bytes, but buffer is %zu "
                "bytes.\n",
                buffer_len, sizeof(buffer)
            );
        register int buffer_idx = 0;
        while (buffer_idx < (int)buffer_len)
        {
            read++;
            register unsigned char c = buffer[buffer_idx++];
            switch (state)
            {
            default:
                ERROR("Invalid state %d.\n", (int)state);
            case STATE_TEXT:
                if (c == '\033')
                {
                    state = STATE_GOT_ESC;
                }
                else
                {
                    if (span && !span_outputted)
                    {
                        append_to_buffer(span);
                        span_outputted = true;
                    }
                    if (c == '&')
                        str_to_buffer("&amp;", 5);
                    else if (c == '<')
                        str_to_buffer("&lt;", 4);
                    else if (c == '>')
                        str_to_buffer("&gt;", 4);
                    else
                        char_to_buffer(c);
                }
                break;
            case STATE_GOT_ESC:
                if (c == '[')
                {
                    state = STATE_GOT_ESC_BRACKET;
                    sgr_chars_len = 0;
                    current_sgr_len = 0;
                    sgrs_len = 0;
                    current_sgr_value = 0L;
                    begun_at = read;
                }
                else
                {
                    if (span && !span_outputted)
                    {
                        append_to_buffer(span);
                        span_outputted = true;
                    }
                    str_to_buffer("&#9243;", 7);
                    if (c == '<')
                        str_to_buffer("&lt;", 4);
                    else if (c == '>')
                        str_to_buffer("&gt;", 4);
                    else if (c == '&')
                        str_to_buffer("&amp;", 5);
                    else
                        char_to_buffer(c);
                    state = STATE_TEXT;
                }
                break;
            case STATE_GOT_ESC_BRACKET:
                sgr_chars_len++;
                // Read until "m":
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
                                ERROR(
                                    "SGR sequence too long, at %zu "
                                    "characters read / %zu in SGR sequence "
                                    "which begun at %zu characters read.\n",
                                    read, sgr_chars_len, begun_at
                                );
                        }
                        else
                            ERROR(
                                "SGR sequence contains invalid number "
                                "'%ld' "
                                "at %zu characters read / %zu in SGR sequence "
                                "which begun at %zu characters read.\n",
                                current_sgr_value, read, sgr_chars_len, begun_at
                            );
                    }
                    state = STATE_TEXT;
                    set_ansi_style_properties(palette, &style, sgrs, sgrs_len);
                    if (span_outputted)
                        str_to_buffer("</span>", 7);
                    span = ansi_span_start(
                        &style, palette, use_classes, use_compact
                    );
                    span_outputted = false;
                }
                else
                {
                    // The character should be a digit, or semicolon.
                    if (c >= '0' && c <= '9')
                    {
                        long prev = current_sgr_value;
                        current_sgr_value *= 10;
                        current_sgr_value += c - '0';
                        if (prev > current_sgr_value)
                            ERROR(
                                "SGR sequence contains invalid number "
                                "'%ld' "
                                "at %zu characters read / %zu in SGR sequence "
                                "which begun at %zu characters read.\n",
                                current_sgr_value, read, sgr_chars_len, begun_at
                            );
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
                                ERROR(
                                    "SGR sequence too long, at %zu "
                                    "characters read / %zu in SGR sequence "
                                    "which begun at %zu characters read.\n",
                                    read, sgr_chars_len, begun_at
                                );
                            current_sgr_len = 0;
                            current_sgr_value = 0;
                        }
                        else
                            ERROR(
                                "SGR sequence contains invalid number "
                                "'%ld' at %zu characters read / %zu in SGR "
                                "sequence which begun at %zu characters "
                                "read.\n",
                                current_sgr_value, read, sgr_chars_len, begun_at
                            );
                    }
                    else
                        ERROR(
                            "SGR sequence contains invalid character "
                            "'%c' at %zu characters read / %zu in SGR sequence "
                            "which begun at %zu characters read.\n",
                            c, read, sgr_chars_len, begun_at
                        );
                }
                break;
            }
        }
    } while (1);

    if (span_outputted)
        str_to_buffer("</span>", 7);

    if (state == STATE_GOT_ESC)
        str_to_buffer("&#9243;", 7);

    if (wrap_in_pre)
        str_to_buffer("</pre>", 6);

    flush_buffer();
}

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
#define LIST_ALL_PALETTES()                                                    \
    do                                                                         \
    {                                                                          \
        for (size_t j = 0;                                                     \
             j < sizeof(named_palettes) / sizeof(named_palettes[0]); j++)      \
            (void)fprintf(stdout, "%s\n", named_palettes[j].name);             \
    } while (0)
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
    // Do you want compact output, i.e. color:#0FA instead of color:#00FFAA
    // where possible?
    bool use_compact = false;
    // Do you "just" want to strip all the ANSI codes, and output the text?
    bool just_strip = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--list-palettes") == 0 ||
            strcmp(argv[i], "-l") == 0)
        {
            LIST_ALL_PALETTES();
            exit(0);
        }
        else if (strcmp(argv[i], "--strip") == 0 || strcmp(argv[i], "-S") == 0)
        {
            just_strip = true;
        }
        else if ((strcmp(argv[i], "--palette") == 0) ||
                 (strcmp(argv[i], "-p") == 0))
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
                        "#%02X%02X%02X\n",
                        VRGB(
                            palette->default_fg.red, palette->default_fg.green,
                            palette->default_fg.blue
                        )
                    );
                    exit(0);
                }
                else if (!strcmp(argv[i], "bg"))
                {
                    (void)printf(
                        "#%02X%02X%02X\n",
                        VRGB(
                            palette->default_bg.red, palette->default_bg.green,
                            palette->default_bg.blue
                        )
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
                    (void)printf(
                        "#%02X%02X%02X\n", VRGB(rgb.red, rgb.green, rgb.blue)
                    );
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
        else if (strcmp(argv[i], "--use-compact") == 0)
        {
            use_compact = true;
        }
        else if (strcmp(argv[i], "--show-style-tag") == 0)
        {
            {
                bool did_compact = false;
                if (use_compact)
                {
                    if (((palette->default_fg.red % 17) == 0 &&
                         (palette->default_fg.green % 17) == 0 &&
                         (palette->default_fg.blue % 17) == 0) ||
                        ((palette->default_bg.red % 17) == 0 &&
                         (palette->default_bg.green % 17) == 0 &&
                         (palette->default_bg.blue % 17) == 0))
                    {
                        did_compact = true;
                        printf(".ansi2html{");
                        if ((palette->default_fg.red % 17) == 0 &&
                            (palette->default_fg.green % 17) == 0 &&
                            (palette->default_fg.blue % 17) == 0)
                            printf(
                                "color:#%01X%01X%01X;",
                                VRGB(
                                    palette->default_fg.red / 17,
                                    palette->default_fg.green / 17,
                                    palette->default_fg.blue / 17
                                )
                            );
                        else
                            printf(
                                "color:#%02X%02X%02X;",
                                VRGB(
                                    palette->default_fg.red,
                                    palette->default_fg.green,
                                    palette->default_fg.blue
                                )
                            );
                        if ((palette->default_bg.red % 17) == 0 &&
                            (palette->default_bg.green % 17) == 0 &&
                            (palette->default_bg.blue % 17) == 0)
                            printf(
                                "background-color:#%01X%01X%01X;",
                                VRGB(
                                    palette->default_bg.red / 17,
                                    palette->default_bg.green / 17,
                                    palette->default_bg.blue / 17
                                )
                            );
                        else
                            printf(
                                "background-color:#%02X%02X%02X;",
                                VRGB(
                                    palette->default_bg.red,
                                    palette->default_bg.green,
                                    palette->default_bg.blue
                                )
                            );
                        printf("}");
                    }
                }
                if (!did_compact)
                    printf(
                        ".ansi2html{"
                        "color:#%02X%02X%02X;background-color:#%02X%02X%02X;"
                        "}",
                        VRGB(
                            palette->default_fg.red, palette->default_fg.green,
                            palette->default_fg.blue
                        ),
                        VRGB(
                            palette->default_bg.red, palette->default_bg.green,
                            palette->default_bg.blue
                        )
                    );
            }
            if (use_compact && (palette->default_fg.red % 17) == 0 &&
                (palette->default_fg.green % 17) == 0 &&
                (palette->default_fg.blue % 17) == 0)
                printf(
                    ".ansi2html .fg-default{color:#%01X%01X%01X;}",
                    VRGB(
                        palette->default_fg.red / 17,
                        palette->default_fg.green / 17,
                        palette->default_fg.blue / 17
                    )
                );
            else
                printf(
                    ".ansi2html .fg-default{color:#%02X%02X%02X;}",
                    VRGB(
                        palette->default_fg.red, palette->default_fg.green,
                        palette->default_fg.blue
                    )
                );
            if (use_compact && (palette->default_bg.red % 17) == 0 &&
                (palette->default_bg.green % 17) == 0 &&
                (palette->default_bg.blue % 17) == 0)
                printf(
                    ".ansi2html .bg-default{background-color:#%01X%01X%01X;}",
                    VRGB(
                        palette->default_bg.red / 17,
                        palette->default_bg.green / 17,
                        palette->default_bg.blue / 17
                    )
                );
            else
                printf(
                    ".ansi2html .bg-default{background-color:#%02X%02X%02X;}",
                    VRGB(
                        palette->default_bg.red, palette->default_bg.green,
                        palette->default_bg.blue
                    )
                );
            for (int j = 0; j < 256; j++)
            {
                struct ansi_rgb rgb = {0};
                ansi256_to_rgb(j, palette, &rgb);
                if (use_compact && (rgb.red % 17) == 0 &&
                    (rgb.green % 17) == 0 && (rgb.blue % 17) == 0)
                {
                    printf(
                        ".ansi2html .fg-%d{color:#%01X%01X%01X;}", j,
                        VRGB(rgb.red / 17, rgb.green / 17, rgb.blue / 17)
                    );
                    printf(
                        ".ansi2html .bg-%d{background-color:#%01X%01X%01X;}", j,
                        VRGB(rgb.red / 17, rgb.green / 17, rgb.blue / 17)
                    );
                }
                else
                {
                    printf(
                        ".ansi2html .fg-%d{color:#%02X%02X%02X;}", j,
                        VRGB(rgb.red, rgb.green, rgb.blue)
                    );
                    printf(
                        ".ansi2html .bg-%d{background-color:#%02X%02X%02X;}", j,
                        VRGB(rgb.red, rgb.green, rgb.blue)
                    );
                }
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

    if (!just_strip)
    {
        ansi2html(
            style, palette, wrap_in_pre, pre_add_style, use_classes, use_compact
        );
        exit(0);
    }

    just_strip_it();
    exit(0);
}
