#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ansi2html.h"

#include "structs/ansi_color_type.h"

void show_ansi_style(
    struct ansi_style *s, const char *file, int line, const char *funcname
)
{
    if (!s)
        return;
    struct ansi_style_properties *props = &s->style_properties;
    struct ansi_color *fg = &s->color_foreground;
    struct ansi_color *bg = &s->color_background;
    printf(
        "Style:"
        " %cboldisbright"
        " %cbold %cfaint %citalic %cunderline"
        " %cslowB %cfastB %creverse %ccrossout"
        " %cfraktur %cdblunder %cframe %ccircle %coverline"
        " fg #%02X%02X%02X bg #%02X%02X%02X"
        " at %s:%d [%s]\n",
        s->bold_is_bright ? '+' : '-', props->bold ? '+' : '-',
        props->faint ? '+' : '-', props->italic ? '+' : '-',
        props->underline ? '+' : '-', props->slow_blink ? '+' : '-',
        props->fast_blink ? '+' : '-', props->reverse ? '+' : '-',
        props->crossout ? '+' : '-', props->fraktur ? '+' : '-',
        props->double_underline ? '+' : '-', props->frame ? '+' : '-',
        props->circle ? '+' : '-', props->overline ? '+' : '-', fg->rgb.red,
        fg->rgb.green, fg->rgb.blue, bg->rgb.red, bg->rgb.green, bg->rgb.blue,
        file, line, funcname
    );
}

// Convert ansi 16 color(s) to RGB, given a specific "palette".
void ansi16_to_rgb(
    int color, struct ansi_color_palette *palette, struct ansi_rgb *rgb
)
{
    static struct ansi_rgb rgb_error = {.rgb = 0xFF66FF};
    if (palette && rgb)
    {
        if (color >= 30 && color <= 37)
            *rgb = palette->base[color - 30];
        else if (color >= 40 && color <= 47)
            *rgb = palette->base[color - 40];
        else if (color >= 90 && color <= 97)
            *rgb = palette->bright[color - 90];
        else if (color >= 100 && color <= 107)
            *rgb = palette->bright[color - 100];
        else
            *rgb = rgb_error;
    }
    else if (rgb)
        *rgb = rgb_error;
}

// The 256 color palette is a 6x6x6 RGB cube, plus 24 grayscale colors.
struct ansi_rgb color_palette_256[256] = {
    // The first 16 are based on the wanted palette, and ignored here.
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},
    {.rgb = 0xFF66FF},

    // The 6x6x6 RGB for 16-231:
    {.rgb = 0x000000},
    {.rgb = 0x00005F},
    {.rgb = 0x000087},
    {.rgb = 0x0000AF},
    {.rgb = 0x0000D7},
    {.rgb = 0x0000FF},
    {.rgb = 0x005F00},
    {.rgb = 0x005F5F},
    {.rgb = 0x005F87},
    {.rgb = 0x005FAF},
    {.rgb = 0x005FD7},
    {.rgb = 0x005FFF},
    {.rgb = 0x008700},
    {.rgb = 0x00875F},
    {.rgb = 0x008787},
    {.rgb = 0x0087AF},
    {.rgb = 0x0087D7},
    {.rgb = 0x0087FF},
    {.rgb = 0x00AF00},
    {.rgb = 0x00AF5F},
    {.rgb = 0x00AF87},
    {.rgb = 0x00AFAF},
    {.rgb = 0x00AFD7},
    {.rgb = 0x00AFFF},
    {.rgb = 0x00D700},
    {.rgb = 0x00D75F},
    {.rgb = 0x00D787},
    {.rgb = 0x00D7AF},
    {.rgb = 0x00D7D7},
    {.rgb = 0x00D7FF},
    {.rgb = 0x00FF00},
    {.rgb = 0x00FF5F},
    {.rgb = 0x00FF87},
    {.rgb = 0x00FFAF},
    {.rgb = 0x00FFD7},
    {.rgb = 0x00FFFF},
    {.rgb = 0x5F0000},
    {.rgb = 0x5F005F},
    {.rgb = 0x5F0087},
    {.rgb = 0x5F00AF},
    {.rgb = 0x5F00D7},
    {.rgb = 0x5F00FF},
    {.rgb = 0x5F5F00},
    {.rgb = 0x5F5F5F},
    {.rgb = 0x5F5F87},
    {.rgb = 0x5F5FAF},
    {.rgb = 0x5F5FD7},
    {.rgb = 0x5F5FFF},
    {.rgb = 0x5F8700},
    {.rgb = 0x5F875F},
    {.rgb = 0x5F8787},
    {.rgb = 0x5F87AF},
    {.rgb = 0x5F87D7},
    {.rgb = 0x5F87FF},
    {.rgb = 0x5FAF00},
    {.rgb = 0x5FAF5F},
    {.rgb = 0x5FAF87},
    {.rgb = 0x5FAFAF},
    {.rgb = 0x5FAFD7},
    {.rgb = 0x5FAFFF},
    {.rgb = 0x5FD700},
    {.rgb = 0x5FD75F},
    {.rgb = 0x5FD787},
    {.rgb = 0x5FD7AF},
    {.rgb = 0x5FD7D7},
    {.rgb = 0x5FD7FF},
    {.rgb = 0x5FFF00},
    {.rgb = 0x5FFF5F},
    {.rgb = 0x5FFF87},
    {.rgb = 0x5FFFAF},
    {.rgb = 0x5FFFD7},
    {.rgb = 0x5FFFFF},
    {.rgb = 0x870000},
    {.rgb = 0x87005F},
    {.rgb = 0x870087},
    {.rgb = 0x8700AF},
    {.rgb = 0x8700D7},
    {.rgb = 0x8700FF},
    {.rgb = 0x875F00},
    {.rgb = 0x875F5F},
    {.rgb = 0x875F87},
    {.rgb = 0x875FAF},
    {.rgb = 0x875FD7},
    {.rgb = 0x875FFF},
    {.rgb = 0x878700},
    {.rgb = 0x87875F},
    {.rgb = 0x878787},
    {.rgb = 0x8787AF},
    {.rgb = 0x8787D7},
    {.rgb = 0x8787FF},
    {.rgb = 0x87AF00},
    {.rgb = 0x87AF5F},
    {.rgb = 0x87AF87},
    {.rgb = 0x87AFAF},
    {.rgb = 0x87AFD7},
    {.rgb = 0x87AFFF},
    {.rgb = 0x87D700},
    {.rgb = 0x87D75F},
    {.rgb = 0x87D787},
    {.rgb = 0x87D7AF},
    {.rgb = 0x87D7D7},
    {.rgb = 0x87D7FF},
    {.rgb = 0x87FF00},
    {.rgb = 0x87FF5F},
    {.rgb = 0x87FF87},
    {.rgb = 0x87FFAF},
    {.rgb = 0x87FFD7},
    {.rgb = 0x87FFFF},
    {.rgb = 0xAF0000},
    {.rgb = 0xAF005F},
    {.rgb = 0xAF0087},
    {.rgb = 0xAF00AF},
    {.rgb = 0xAF00D7},
    {.rgb = 0xAF00FF},
    {.rgb = 0xAF5F00},
    {.rgb = 0xAF5F5F},
    {.rgb = 0xAF5F87},
    {.rgb = 0xAF5FAF},
    {.rgb = 0xAF5FD7},
    {.rgb = 0xAF5FFF},
    {.rgb = 0xAF8700},
    {.rgb = 0xAF875F},
    {.rgb = 0xAF8787},
    {.rgb = 0xAF87AF},
    {.rgb = 0xAF87D7},
    {.rgb = 0xAF87FF},
    {.rgb = 0xAFAF00},
    {.rgb = 0xAFAF5F},
    {.rgb = 0xAFAF87},
    {.rgb = 0xAFAFAF},
    {.rgb = 0xAFAFD7},
    {.rgb = 0xAFAFFF},
    {.rgb = 0xAFD700},
    {.rgb = 0xAFD75F},
    {.rgb = 0xAFD787},
    {.rgb = 0xAFD7AF},
    {.rgb = 0xAFD7D7},
    {.rgb = 0xAFD7FF},
    {.rgb = 0xAFFF00},
    {.rgb = 0xAFFF5F},
    {.rgb = 0xAFFF87},
    {.rgb = 0xAFFFAF},
    {.rgb = 0xAFFFD7},
    {.rgb = 0xAFFFFF},
    {.rgb = 0xD70000},
    {.rgb = 0xD7005F},
    {.rgb = 0xD70087},
    {.rgb = 0xD700AF},
    {.rgb = 0xD700D7},
    {.rgb = 0xD700FF},
    {.rgb = 0xD75F00},
    {.rgb = 0xD75F5F},
    {.rgb = 0xD75F87},
    {.rgb = 0xD75FAF},
    {.rgb = 0xD75FD7},
    {.rgb = 0xD75FFF},
    {.rgb = 0xD78700},
    {.rgb = 0xD7875F},
    {.rgb = 0xD78787},
    {.rgb = 0xD787AF},
    {.rgb = 0xD787D7},
    {.rgb = 0xD787FF},
    {.rgb = 0xD7AF00},
    {.rgb = 0xD7AF5F},
    {.rgb = 0xD7AF87},
    {.rgb = 0xD7AFAF},
    {.rgb = 0xD7AFD7},
    {.rgb = 0xD7AFFF},
    {.rgb = 0xD7D700},
    {.rgb = 0xD7D75F},
    {.rgb = 0xD7D787},
    {.rgb = 0xD7D7AF},
    {.rgb = 0xD7D7D7},
    {.rgb = 0xD7D7FF},
    {.rgb = 0xD7FF00},
    {.rgb = 0xD7FF5F},
    {.rgb = 0xD7FF87},
    {.rgb = 0xD7FFAF},
    {.rgb = 0xD7FFD7},
    {.rgb = 0xD7FFFF},
    {.rgb = 0xFF0000},
    {.rgb = 0xFF005F},
    {.rgb = 0xFF0087},
    {.rgb = 0xFF00AF},
    {.rgb = 0xFF00D7},
    {.rgb = 0xFF00FF},
    {.rgb = 0xFF5F00},
    {.rgb = 0xFF5F5F},
    {.rgb = 0xFF5F87},
    {.rgb = 0xFF5FAF},
    {.rgb = 0xFF5FD7},
    {.rgb = 0xFF5FFF},
    {.rgb = 0xFF8700},
    {.rgb = 0xFF875F},
    {.rgb = 0xFF8787},
    {.rgb = 0xFF87AF},
    {.rgb = 0xFF87D7},
    {.rgb = 0xFF87FF},
    {.rgb = 0xFFAF00},
    {.rgb = 0xFFAF5F},
    {.rgb = 0xFFAF87},
    {.rgb = 0xFFAFAF},
    {.rgb = 0xFFAFD7},
    {.rgb = 0xFFAFFF},
    {.rgb = 0xFFD700},
    {.rgb = 0xFFD75F},
    {.rgb = 0xFFD787},
    {.rgb = 0xFFD7AF},
    {.rgb = 0xFFD7D7},
    {.rgb = 0xFFD7FF},
    {.rgb = 0xFFFF00},
    {.rgb = 0xFFFF5F},
    {.rgb = 0xFFFF87},
    {.rgb = 0xFFFFAF},
    {.rgb = 0xFFFFD7},
    {.rgb = 0xFFFFFF},

    // 232-255 for greyscale;
    {.rgb = 0x080808},
    {.rgb = 0x121212},
    {.rgb = 0x1C1C1C},
    {.rgb = 0x262626},
    {.rgb = 0x303030},
    {.rgb = 0x3A3A3A},
    {.rgb = 0x444444},
    {.rgb = 0x4E4E4E},
    {.rgb = 0x585858},
    {.rgb = 0x626262},
    {.rgb = 0x6C6C6C},
    {.rgb = 0x767676},
    {.rgb = 0x808080},
    {.rgb = 0x8A8A8A},
    {.rgb = 0x949494},
    {.rgb = 0x9E9E9E},
    {.rgb = 0xA8A8A8},
    {.rgb = 0xB2B2B2},
    {.rgb = 0xBCBCBC},
    {.rgb = 0xC6C6C6},
    {.rgb = 0xD0D0D0},
    {.rgb = 0xDADADA},
    {.rgb = 0xE4E4E4},
    {.rgb = 0xEEEEEE},
};

// Convert an ansi 256 color to RGB, given a specific "palette" for the lower
// 16 colors.
void ansi256_to_rgb(
    int color, struct ansi_color_palette *palette, struct ansi_rgb *rgb
)
{
    static struct ansi_rgb rgb_error = {.rgb = 0xFF66FF};
    if (palette && rgb)
    {
        if (color >= 0 && color <= 15)
        {
            if (color <= 7)
                *rgb = palette->base[color];
            else
                *rgb = palette->bright[color - 8];
            return;
        }
        if (color >= 16 && color <= 255)
            *rgb = color_palette_256[color];
        else
            *rgb = rgb_error;
    }
    else if (rgb)
        *rgb = rgb_error;
}

void showcase_palette(struct ansi_color_palette *palette)
{
    if (!palette)
        return;
    printf(
        "Default: \e[0;38;2;%d;%d;%dm#%02X%02X%02X\e[0m on "
        "\e[0;48;2;%d;%d;%dm#%02X%02X%02X\e[0m: "
        "\e[0;38;2;%d;%d;%d;48;2;%d;%d;%dmExample\e[0m\n",
        palette->default_fg.red, palette->default_fg.green,
        palette->default_fg.blue, palette->default_fg.red,
        palette->default_fg.green, palette->default_fg.blue,
        palette->default_bg.red, palette->default_bg.green,
        palette->default_bg.blue, palette->default_bg.red,
        palette->default_bg.green, palette->default_bg.blue,
        palette->default_fg.red, palette->default_fg.green,
        palette->default_fg.blue, palette->default_bg.red,
        palette->default_bg.green, palette->default_bg.blue
    );
    for (int i = 0; i < 8; i++)
    {
        printf(
            "Color %d: "
            "base: "
            "\e[0;38;2;%d;%d;%dm#%02X%02X%02X "
            "\e[0;48;2;%d;%d;%dm#%02X%02X%02X\e[0m"
            " bright: "
            "\e[0;38;2;%d;%d;%dm#%02X%02X%02X "
            "\e[0;48;2;%d;%d;%dm#%02X%02X%02X\e[0m"
            "\n",
            i, palette->base[i].red, palette->base[i].green,
            palette->base[i].blue, palette->base[i].red, palette->base[i].green,
            palette->base[i].blue, palette->base[i].red, palette->base[i].green,
            palette->base[i].blue, palette->base[i].red, palette->base[i].green,
            palette->base[i].blue, palette->bright[i].red,
            palette->bright[i].green, palette->bright[i].blue,
            palette->bright[i].red, palette->bright[i].green,
            palette->bright[i].blue, palette->bright[i].red,
            palette->bright[i].green, palette->bright[i].blue,
            palette->bright[i].red, palette->bright[i].green,
            palette->bright[i].blue
        );
    }
    // Show "it" using the "default foreground "on" default background for the
    // parts in-between the colors.
    // clang-format off
#define SHOWCASE_RESET()                                                       \
    printf(                                                                    \
        "\e[0;38;2;%d;%d;%d;48;2;%d;%d;%dm",                                   \
        palette->default_fg.red,                                               \
        palette->default_fg.green,                                             \
        palette->default_fg.blue,                                              \
        palette->default_bg.red,                                               \
        palette->default_bg.green,                                             \
        palette->default_bg.blue                                               \
    )
    // clang-format on
    SHOWCASE_RESET();
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
            printf("%-7s", " ");
        // clang-format off
        printf("\e[48;2;%d;%d;%dm%-2dm      ",
            palette->base[i].red,
            palette->base[i].green,
            palette->base[i].blue,
            i + 40
        );
        // clang-format on
    }
    printf("\e[0m\n");
    for (int i = 0; i < 8; i++)
    {
        SHOWCASE_RESET();
        // clang-format off
        printf("\e[38;2;%d;%d;%dm%-2dm    ",
            palette->base[i].red,
            palette->base[i].green,
            palette->base[i].blue,
            i + 30
        );
        // clang-format on
        for (int j = 0; j < 8; j++)
        {
            // clang-format off
            printf(
                "\e[0;38;2;%d;%d;%d;48;2;%d;%d;%dm%02d;%02dm",
                palette->base[i].red, palette->base[i].green,
                palette->base[i].blue, palette->base[j].red,
                palette->base[j].green, palette->base[j].blue,
                i + 30, j + 40
            );
            // clang-format on
            SHOWCASE_RESET();
            printf("   ");
        }
        printf("\e[0m\n");
    }
    SHOWCASE_RESET();
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
            printf("%-7s", " ");
        // clang-format off
        printf("\e[48;2;%d;%d;%dm%-3dm     ",
            palette->bright[i].red,
            palette->bright[i].green,
            palette->bright[i].blue,
            i + 100
        );
        // clang-format on
    }
    printf("\e[0m\n");
    for (int i = 0; i < 8; i++)
    {
        SHOWCASE_RESET();
        // clang-format off
        printf("\e[38;2;%d;%d;%dm%-2dm    ",
            palette->bright[i].red,
            palette->bright[i].green,
            palette->bright[i].blue,
            i + 90
        );
        // clang-format on
        for (int j = 0; j < 8; j++)
        {
            // clang-format off
            printf("\e[0;38;2;%d;%d;%d;48;2;%d;%d;%dm%02d;%03dm",
                palette->bright[i].red,
                palette->bright[i].green,
                palette->bright[i].blue,
                palette->bright[j].red,
                palette->bright[j].green,
                palette->bright[j].blue,
                i + 90, j + 100
            );
            // clang-format on
            SHOWCASE_RESET();
            printf("  ");
        }
        printf("\e[0m\n");
    }
}

#ifdef DEBUG_SET_ANSI_STYLE_PROPERTIES
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

// Given the wanted palette and the current style in effect, and given an SGR
// sequence (array of unsigned chars), set the properties, colors, etc in the
// palette accordingly.
void set_ansi_style_properties(
    struct ansi_color_palette *palette, struct ansi_style *style,
    unsigned char *sgr, size_t len
)
{
    if (!sgr || !style || !palette)
        return;
    DEBUG("\n\nset_ansi_style_properties: len %zu\n", len);
    static struct ansi_style_properties empty_props = {0};

    // Both "\e[0m" and "\e[m" reset all properties. Easy early exit.
    if (len == 0 || (len == 1 && sgr[0] == 0))
    {
        style->style_properties = empty_props;
        // Also reset the colors to default:
        style->color_foreground.color_type = COLOR_TYPE_DEFAULT_FG;
        style->color_foreground.fg_or_bg = ANSI_FG;
        style->color_foreground.rgb = palette->default_fg;
        style->color_foreground.is_base_color = false;
        style->color_foreground.base_color = -1;
        style->color_background.color_type = COLOR_TYPE_DEFAULT_BG;
        style->color_background.fg_or_bg = ANSI_BG;
        style->color_background.is_base_color = false;
        style->color_background.base_color = -1;
        style->color_background.rgb = palette->default_bg;
        DEBUG("Reset all properties\n\n");
        return;
    }

    struct ansi_style_properties *props = &style->style_properties;
    struct ansi_color *fg = &style->color_foreground;
    struct ansi_color *bg = &style->color_background;
    for (size_t i = 0; i < len; i++)
    {
        DEBUG("sgr[%zu] = %d\n", i, sgr[i]);
        switch (sgr[i])
        {
        case 0: // Reset all to default
            *props = empty_props;
            fg->color_type = COLOR_TYPE_DEFAULT_FG;
            fg->fg_or_bg = ANSI_FG;
            fg->rgb = palette->default_fg;
            fg->is_base_color = false;
            fg->base_color = -1;
            bg->color_type = COLOR_TYPE_DEFAULT_BG;
            bg->fg_or_bg = ANSI_BG;
            bg->is_base_color = false;
            bg->base_color = -1;
            bg->rgb = palette->default_bg;
            DEBUG(" 0 -> Reset all properties\n");
            break;
        case 1:
            props->bold = true;
            if (style->bold_is_bright)
            {
                if (fg->is_base_color && fg->color_type == COLOR_TYPE_16)
                {
                    if (fg->base_color < 8)
                        fg->rgb = palette->bright[fg->base_color];
                    else if (fg->base_color < 16)
                        fg->rgb = palette->bright[fg->base_color - 8];
                    DEBUG(" 1 -> Bold [bright] base color fg brightened\n");
                }
                else
                {
                    DEBUG(" 1 -> Bold is bright (but not a base color)\n");
                }
            }
            else
            {
                DEBUG(" 1 -> Bold\n");
            }
            break;
        case 2:
            props->faint = true;
            DEBUG(" 2 -> Faint\n");
            break;
        case 3:
            props->italic = true;
            DEBUG(" 3 -> Italic\n");
            break;
        case 4:
            props->underline = true;
            DEBUG(" 4 -> Underline\n");
            break;
        case 5:
            props->slow_blink = true;
            DEBUG(" 5 -> Slow blink\n");
            break;
        case 6:
            props->fast_blink = true;
            DEBUG(" 6 -> Fast blink\n");
            break;
        case 7:
            props->reverse = true;
            DEBUG(" 7 -> Reverse\n");
            break;
        // 8: conceal
        case 9:
            props->crossout = true;
            DEBUG(" 9 -> Crossout\n");
            break;
        // 10: primary font
        // 11-19: alternate font
        case 20:
            props->fraktur = true;
            DEBUG("20 -> Fraktur\n");
            break;
        case 21:
            props->double_underline = true;
            DEBUG("21 -> Double underline\n");
            break;
        case 22: // Normal intensity
            props->bold = false;
            props->faint = false;
            if (style->bold_is_bright)
            {
                if (fg->is_base_color && fg->color_type == COLOR_TYPE_16)
                {
                    if (fg->base_color < 8)
                        fg->rgb = palette->base[fg->base_color];
                    else if (fg->base_color < 16)
                        fg->rgb = palette->base[fg->base_color - 8];
                    DEBUG("22 -> Neither bold nor faint, fg base color\n");
                }
                else
                {
                    DEBUG("22 -> Neither bold nor faint, fg not base color\n");
                }
            }
            else
            {
                DEBUG("22 -> Neither bold nor faint\n");
            }
            break;
        case 23: // Neither italics, nor blackletter
            props->italic = false;
            props->fraktur = false;
            DEBUG("23 -> Neither italic nor fraktur\n");
            break;
        case 24: // Not underlined
            props->underline = false;
            props->double_underline = false;
            DEBUG("24 -> Not underlined\n");
            break;
        case 25: // Blink off
            props->slow_blink = false;
            props->fast_blink = false;
            DEBUG("25 -> Blink off\n");
            break;
        // 26: Proportional spacing
        case 27: // Not reversed
            props->reverse = false;
            DEBUG("27 -> Not reversed\n");
            break;
        // 28: not revealed
        case 29: // Not crossed out
            props->crossout = false;
            DEBUG("29 -> Not crossed out\n");
            break;
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
            fg->color_type = COLOR_TYPE_16;
            fg->fg_or_bg = ANSI_FG;
            fg->is_base_color = true;
            fg->base_color = sgr[i] - 30;
            if (style->bold_is_bright && props->bold)
                ansi16_to_rgb(sgr[i] + 60, palette, &fg->rgb);
            else
                ansi16_to_rgb(sgr[i], palette, &fg->rgb);
            DEBUG(
                "%d -> Foreground color -> fg RGB %02X%02X%02X\n", sgr[i],
                fg->rgb.red, fg->rgb.green, fg->rgb.blue
            );
            break;
        case 38: // Set foreground color, depending on next token
            if (i + 1 < len)
            {
                DEBUG("38 -> Set foreground color, depending on next token\n");
                if (sgr[i + 1] == 5) // 38;5;N: 256 color
                {
                    DEBUG("OK 38;5;N: 256 color\n");
                    if (i + 2 < len)
                    {
                        DEBUG("OK i + 2 < len\n");
                        fg->color_type = COLOR_TYPE_256;
                        fg->fg_or_bg = ANSI_FG;
                        fg->is_base_color = true;
                        fg->base_color = sgr[i + 2];
                        ansi256_to_rgb(sgr[i + 2], palette, &fg->rgb);
                        DEBUG(
                            "%d;5;%d -> Foreground color 256 %d -> fg RGB "
                            "%02X%02X%02X\n",
                            sgr[i], sgr[i + 2], sgr[i + 2], fg->rgb.red,
                            fg->rgb.green, fg->rgb.blue
                        );
                        i += 2;
                        break;
                    }
                    // Error?
                }
                else if (sgr[i + 1] == 2) // 38;2;R;G;B
                {
                    DEBUG("OK 38;2;N... 24-bit color\n");
                    if (i + 4 < len)
                    {
                        fg->color_type = COLOR_TYPE_24BIT;
                        fg->fg_or_bg = ANSI_FG;
                        fg->is_base_color = false;
                        fg->rgb.red = sgr[i + 2];
                        fg->rgb.green = sgr[i + 3];
                        fg->rgb.blue = sgr[i + 4];
                        DEBUG(
                            "%d;2;%d;%d;%d -> Foreground color 24-bit -> fg "
                            "RGB %02X%02X%02X\n",
                            sgr[i], sgr[i + 2], sgr[i + 3], sgr[i + 4],
                            fg->rgb.red, fg->rgb.green, fg->rgb.blue
                        );
                        i += 4;
                        break;
                    }
                    // Error?
                }
            }
            break;
        case 39: // Default foreground color
            fg->color_type = COLOR_TYPE_DEFAULT_FG;
            fg->fg_or_bg = ANSI_FG;
            fg->is_base_color = false;
            fg->base_color = -1;
            fg->rgb = palette->default_fg;
            DEBUG(
                "39 -> Default foreground color -> fg RGB %02X%02X%02X\n",
                fg->rgb.red, fg->rgb.green, fg->rgb.blue
            );
            break;
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
            bg->color_type = COLOR_TYPE_16;
            bg->fg_or_bg = ANSI_BG;
            bg->is_base_color = true;
            bg->base_color = sgr[i] - 40;
            // Background color doesn't "do" "bold is bright".
            ansi16_to_rgb(sgr[i], palette, &bg->rgb);
            DEBUG(
                "%d -> Background color -> bg RGB %02X%02X%02X\n", sgr[i],
                bg->rgb.red, bg->rgb.green, bg->rgb.blue
            );
            break;
        case 48: // Set background color, depending on next token
            if (i + 1 < len)
            {
                if (sgr[i + 1] == 5) // 48;5;N: 256 color
                {
                    if (i + 2 < len)
                    {
                        bg->color_type = COLOR_TYPE_256;
                        bg->fg_or_bg = ANSI_BG;
                        bg->is_base_color = true;
                        bg->base_color = sgr[i + 2];
                        ansi256_to_rgb(sgr[i + 2], palette, &bg->rgb);
                        DEBUG(
                            "%d;5;%d -> Background color 256 %d -> bg RGB "
                            "%02X%02X%02X\n",
                            sgr[i], sgr[i + 2], sgr[i + 2], bg->rgb.red,
                            bg->rgb.green, bg->rgb.blue
                        );
                        i += 2;
                        break;
                    }
                    // Error?
                }
                else if (sgr[i + 1] == 2) // 48;2;R;G;B
                {
                    if (i + 4 < len)
                    {
                        bg->color_type = COLOR_TYPE_24BIT;
                        bg->fg_or_bg = ANSI_BG;
                        bg->is_base_color = false;
                        bg->rgb.red = sgr[i + 2];
                        bg->rgb.green = sgr[i + 3];
                        bg->rgb.blue = sgr[i + 4];
                        DEBUG(
                            "%d;2;%d;%d;%d -> Background color 24-bit -> bg "
                            "RGB %02X%02X%02X\n",
                            sgr[i], sgr[i + 2], sgr[i + 3], sgr[i + 4],
                            bg->rgb.red, bg->rgb.green, bg->rgb.blue
                        );
                        i += 4;
                        break;
                    }
                    // Error?
                }
            }
            break;
        case 49: // Default background color
            bg->color_type = COLOR_TYPE_DEFAULT_BG;
            bg->fg_or_bg = ANSI_BG;
            bg->is_base_color = false;
            bg->base_color = -1;
            bg->rgb = palette->default_bg;
            DEBUG(
                "49 -> Default background color -> bg RGB %02X%02X%02X\n",
                bg->rgb.red, bg->rgb.green, bg->rgb.blue
            );
            break;
        // 50: Disable proportional spacing
        case 51:
            props->frame = true;
            DEBUG("51 -> Frame\n");
            break;
        case 52:
            props->circle = true;
            DEBUG("52 -> Circle\n");
            break;
        case 53:
            props->overline = true;
            DEBUG("53 -> Overline\n");
            break;
        case 54: // Neither framed nor encircled
            props->frame = false;
            props->circle = false;
            DEBUG("54 -> Neither framed nor encircled\n");
            break;
        case 55:
            props->overline = false;
            DEBUG("55 -> Overline off\n");
            break;
        // 56: ?
        // 57: ?
        // 58: used by some for "set underline color"
        // 59: used by some for "set default underline color"
        // 60-65: ?
        // 73-75: ?
        case 90:
        case 91:
        case 92:
        case 93:
        case 94:
        case 95:
        case 96:
        case 97:
            fg->color_type = COLOR_TYPE_16;
            fg->fg_or_bg = ANSI_FG;
            fg->is_base_color = true;
            fg->base_color = sgr[i] - 90 + 8;
            ansi16_to_rgb(sgr[i], palette, &fg->rgb);
            DEBUG(
                "%d -> Foreground color -> fg RGB %02X%02X%02X\n", sgr[i],
                fg->rgb.red, fg->rgb.green, fg->rgb.blue
            );
            break;
        case 100:
        case 101:
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
            bg->color_type = COLOR_TYPE_16;
            bg->fg_or_bg = ANSI_BG;
            bg->is_base_color = true;
            bg->base_color = sgr[i] - 100 + 8;
            ansi16_to_rgb(sgr[i], palette, &bg->rgb);
            DEBUG(
                "%d -> Background color -> bg RGB %02X%02X%02X\n", sgr[i],
                bg->rgb.red, bg->rgb.green, bg->rgb.blue
            );
            break;
        default:
            DEBUG("Unknown SGR code %d\n", sgr[i]);
            break;
        }
    }
    DEBUG("Done for set_ansi_style_properties[%zu]\n", len);
}

struct style_or_class
{
    char *style;
    size_t style_len;
    char *class;
    size_t class_len;
};
static struct style_or_class style_bold = {"font-weight:bold;", 17, "bold", 4};
static struct style_or_class style_faint = {"opacity:0.67;", 13, "faint", 5};
static struct style_or_class style_italic = {
    "font-style:italic;", 18, "italic", 6
};
static struct style_or_class style_double_underline = {
    "border-bottom:3px double;", 25, "double-underline", 16
};
static struct style_or_class style_underline = {
    "text-decoration:underline;", 26, "underline", 9
};
static struct style_or_class style_slow_blink = {
    "text-decoration:blink;", 22, "slow-blink", 10
};
static struct style_or_class style_fast_blink = {
    "text-decoration:blink;", 22, "fast-blink", 10
};
static struct style_or_class style_crossout = {
    "text-decoration:line-through;", 29, "crossout", 8
};
static struct style_or_class style_fraktur = {
    "font-family:fraktur;", 20, "fraktur", 7
};
static struct style_or_class style_frame = {
    "border:1px solid;", 17, "frame", 5
};
static struct style_or_class style_circle = {
    "border:1px solid;border-radius:50%;", 35, "circle", 6
};
static struct style_or_class style_overline = {
    "text-decoration:overline;", 25, "overline", 8
};

void show_additional_styles(void)
{
#define PUTS_STYLE_OR_CLASS(s)                                                 \
    do                                                                         \
    {                                                                          \
        PUTS(".ansi2html .");                                                  \
        PUTS(s.class);                                                         \
        PUTC('{');                                                             \
        PUTS(s.style);                                                         \
        PUTC('}');                                                             \
    } while (0)
    PUTS_STYLE_OR_CLASS(style_bold);
    PUTS_STYLE_OR_CLASS(style_faint);
    PUTS_STYLE_OR_CLASS(style_italic);
    PUTS_STYLE_OR_CLASS(style_double_underline);
    PUTS_STYLE_OR_CLASS(style_underline);
    PUTS_STYLE_OR_CLASS(style_slow_blink);
    PUTS_STYLE_OR_CLASS(style_fast_blink);
    PUTS_STYLE_OR_CLASS(style_crossout);
    PUTS_STYLE_OR_CLASS(style_fraktur);
    PUTS_STYLE_OR_CLASS(style_frame);
    PUTS_STYLE_OR_CLASS(style_circle);
    PUTS_STYLE_OR_CLASS(style_overline);
#undef PUTS_STYLE_OR_CLASS
}

static inline void styles_for_props(
    struct ansi_style *s, struct ansi_style_properties *props,
    struct ansi_color_palette *palette, char *style, size_t *style_len,
    char *classes, size_t *class_len, bool use_classes
)
{
    if (!s || !props || !palette || !style || !classes || !style_len ||
        !class_len)
        return;
    char *ps = style;
    *ps = '\0';
    size_t slen = 0;
    char *pc = classes;
    *pc = '\0';
    size_t clen = 0;
#define ADD_STYLE(cond, spec)                                                  \
    if (cond)                                                                  \
    {                                                                          \
        if (use_classes)                                                       \
        {                                                                      \
            if (pc != classes)                                                 \
            {                                                                  \
                *pc++ = ' ';                                                   \
                *pc = '\0';                                                    \
                clen++;                                                        \
            }                                                                  \
            for (size_t i = 0; i < spec.class_len; i++)                        \
                *pc++ = spec.class[i];                                         \
            *pc = '\0';                                                        \
            clen += spec.class_len;                                            \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            for (size_t i = 0; i < spec.style_len; i++)                        \
                *ps++ = spec.style[i];                                         \
            *ps = '\0';                                                        \
            slen += spec.style_len;                                            \
        }                                                                      \
    }
    if (props->bold)
    {
        struct ansi_color *fg =
            props->reverse ? &s->color_background : &s->color_foreground;
        if (!s->bold_is_bright || !fg->is_base_color ||
            fg->color_type != COLOR_TYPE_16)
            ADD_STYLE(true, style_bold);
    }
    else if (props->faint)
    {
        ADD_STYLE(true, style_faint);
    }
    ADD_STYLE(props->italic, style_italic);
    ADD_STYLE(props->double_underline, style_double_underline);
    ADD_STYLE(props->underline, style_underline);
    ADD_STYLE(props->slow_blink, style_slow_blink);
    ADD_STYLE(props->fast_blink, style_fast_blink);
    ADD_STYLE(props->crossout, style_crossout);
    ADD_STYLE(props->fraktur, style_fraktur);
    ADD_STYLE(props->frame, style_frame);
    ADD_STYLE(props->circle, style_circle);
    ADD_STYLE(props->overline, style_overline);
    *style_len = slen;
    *class_len = clen;
#undef ADD_STYLE
    (void)ps;
    (void)pc;
}

char *ansi_span_start(
    struct ansi_style *s, struct ansi_color_palette *palette, bool use_classes,
    bool use_compact
)
{
    if (!s || !palette)
        return NULL;
    struct ansi_style_properties *props = &s->style_properties;
    struct ansi_color *fg = &s->color_foreground;
    struct ansi_color *bg = &s->color_background;
    if (props->reverse)
    {
        fg = &s->color_background;
        bg = &s->color_foreground;
    }
    static char classes[512];
    static char styles[512];
    classes[0] = '\0';
    styles[0] = '\0';
    size_t style_len = 0;
    size_t class_len = 0;
    styles_for_props(
        s, props, palette, styles, &style_len, classes, &class_len, use_classes
    );
    char *pc = classes + class_len;
    char *ps = styles + style_len;
    // RGB hex for "default foreground" color in the palette for foreground
    // should result in nothing at all. Also, if the current foreground color
    // Just So Happens to have the same RGB as the default foreground color.
    if (fg->color_type == COLOR_TYPE_DEFAULT_FG ||
        (fg->rgb.red == palette->default_fg.red &&
         fg->rgb.green == palette->default_fg.green &&
         fg->rgb.blue == palette->default_fg.blue))
        ;
    else if (use_classes && (fg->color_type == COLOR_TYPE_16 ||
                             fg->color_type == COLOR_TYPE_256))
    {
        if (pc != classes)
        {
            *pc++ = ' ';
            *pc = '\0';
        }
        char this_class[8] = {'f', 'g', '-', '\0', '\0', '\0', '\0'};
        unsigned char this_color = fg->base_color;
        if (s->bold_is_bright && props->bold && fg->is_base_color &&
            fg->base_color < 8 && fg->color_type == COLOR_TYPE_16)
            this_color += 8;
        if (this_color < 10)
        {
            this_class[3] = (char)('0' + this_color);
            (void)memcpy(pc, this_class, 4);
            pc += 4;
        }
        else if (this_color < 100)
        {
            this_class[3] = (char)('0' + (this_color / 10));
            this_class[4] = (char)('0' + (this_color % 10));
            (void)memcpy(pc, this_class, 5);
            pc += 5;
        }
        else
        {
            this_class[3] = (char)('0' + (this_color / 100));
            this_class[4] = (char)('0' + ((this_color / 10) % 10));
            this_class[5] = (char)('0' + (this_color % 10));
            memcpy(pc, this_class, 6);
            pc += 6;
        }
        *pc = '\0';
    }
    else
    {
        //                      012345678901234
        char this_style[16] = {"color:#000000;"};
        // Given a number 0-15, convert it to a 1-digit hex number and put it
        // in the right place in the string.
#define N2HEX(n, start)                                                        \
    do                                                                         \
    {                                                                          \
        if ((n) < 10)                                                          \
            *(start) = (char)('0' + (n));                                      \
        else                                                                   \
            *(start) = (char)('A' + (n - 10));                                 \
    } while (0)
#define COLOR2HEX(color, offset)                                               \
    do                                                                         \
    {                                                                          \
        char *pstart = this_style + offset;                                    \
        if (use_compact && ((color).red % 17) == 0 &&                          \
            ((color).green % 17) == 0 && ((color).blue % 17) == 0)             \
        {                                                                      \
            N2HEX((color).red / 17, pstart);                                   \
            N2HEX((color).green / 17, pstart + 1);                             \
            N2HEX((color).blue / 17, pstart + 2);                              \
            *(pstart + 3) = ';';                                               \
            *(pstart + 4) = '\0';                                              \
            (void)memcpy(ps, this_style, offset + 4);                          \
            ps += offset + 4;                                                  \
            *ps = '\0';                                                        \
            break;                                                             \
        }                                                                      \
        N2HEX((color).red / 16, pstart);                                       \
        N2HEX((color).red % 16, pstart + 1);                                   \
        N2HEX((color).green / 16, pstart + 2);                                 \
        N2HEX((color).green % 16, pstart + 3);                                 \
        N2HEX((color).blue / 16, pstart + 4);                                  \
        N2HEX((color).blue % 16, pstart + 5);                                  \
        (void)memcpy(ps, this_style, offset + 7);                              \
        ps += offset + 7;                                                      \
        *ps = '\0';                                                            \
    } while (0)
        struct ansi_rgb rgb = fg->rgb;
        if (s->bold_is_bright && props->bold && fg->is_base_color &&
            fg->base_color < 8 && fg->color_type == COLOR_TYPE_16)
            rgb = palette->bright[fg->base_color];
        COLOR2HEX(rgb, 7);
    }
    // Same for background:
    if (bg->color_type == COLOR_TYPE_DEFAULT_BG ||
        (bg->rgb.red == palette->default_bg.red &&
         bg->rgb.green == palette->default_bg.green &&
         bg->rgb.blue == palette->default_bg.blue))
        ;
    else if (use_classes && (bg->color_type == COLOR_TYPE_16 ||
                             bg->color_type == COLOR_TYPE_256))
    {
        if (pc != classes)
        {
            *pc++ = ' ';
            *pc = '\0';
        }
        char this_class[8] = {'b', 'g', '-', '\0', '\0', '\0', '\0'};
        unsigned char this_color = bg->base_color;
        if (this_color < 10)
        {
            this_class[3] = (char)('0' + this_color);
            (void)memcpy(pc, this_class, 4);
            pc += 4;
        }
        else if (this_color < 100)
        {
            this_class[3] = (char)('0' + (this_color / 10));
            this_class[4] = (char)('0' + (this_color % 10));
            (void)memcpy(pc, this_class, 5);
            pc += 5;
        }
        else
        {
            this_class[3] = (char)('0' + (this_color / 100));
            this_class[4] = (char)('0' + ((this_color / 10) % 10));
            this_class[5] = (char)('0' + (this_color % 10));
            (void)memcpy(pc, this_class, 6);
            pc += 6;
        }
        *pc = '\0';
    }
    else
    {
        //                      01234567890123456789012345
        char this_style[32] = {"background-color:#000000;"};
        COLOR2HEX(bg->rgb, 18);
    }
#undef N2HEX
#undef COLOR2HEX
    if (!classes[0] && !styles[0])
        return NULL;
    (void)ps;
    (void)pc;
    static char span_start[1024] = {0};
    char *p = span_start;
    (void)memcpy(p, "<span ", 6);
    p += 6;
    *p = '\0';
    if (classes[0])
    {
        (void)memcpy(p, "class=\"", 7);
        p += 7;
        *p = '\0';
        size_t len = strlen(classes);
        (void)memcpy(p, classes, len);
        p += len;
        *p++ = '"';
        *p = '\0';
    }
    if (classes[0] && styles[0])
    {
        *p++ = ' ';
        *p = '\0';
    }
    if (styles[0])
    {
        (void)memcpy(p, "style=\"", 7);
        p += 7;
        *p = '\0';
        size_t len = strlen(styles);
        (void)memcpy(p, styles, len);
        p += len;
        *p++ = '"';
        *p = '\0';
    }
    *p++ = '>';
    *p = '\0';
    return span_start;
}

void reset_ansi_props(struct ansi_style *s, struct ansi_color_palette *palette)
{
    if (!s || !palette)
        return;
    s->style_properties = (struct ansi_style_properties){0};
    s->color_foreground.color_type = COLOR_TYPE_DEFAULT_FG;
    s->color_foreground.is_base_color = false;
    s->color_foreground.base_color = -1;
    s->color_foreground.fg_or_bg = ANSI_FG;
    s->color_foreground.rgb = palette->default_fg;
    s->color_background.color_type = COLOR_TYPE_DEFAULT_BG;
    s->color_background.is_base_color = false;
    s->color_background.base_color = -1;
    s->color_background.fg_or_bg = ANSI_BG;
    s->color_background.rgb = palette->default_bg;
}
