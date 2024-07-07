#include <stdbool.h>
#include <stdio.h>

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
    static struct ansi_rgb rgb_error = {0xff, 0x66, 0xff};
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
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},
    {0xff, 0x66, 0xff},

    // The 6x6x6 RGB for 16-231:
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x5f},
    {0x00, 0x00, 0x87},
    {0x00, 0x00, 0xaf},
    {0x00, 0x00, 0xd7},
    {0x00, 0x00, 0xff},
    {0x00, 0x5f, 0x00},
    {0x00, 0x5f, 0x5f},
    {0x00, 0x5f, 0x87},
    {0x00, 0x5f, 0xaf},
    {0x00, 0x5f, 0xd7},
    {0x00, 0x5f, 0xff},
    {0x00, 0x87, 0x00},
    {0x00, 0x87, 0x5f},
    {0x00, 0x87, 0x87},
    {0x00, 0x87, 0xaf},
    {0x00, 0x87, 0xd7},
    {0x00, 0x87, 0xff},
    {0x00, 0xaf, 0x00},
    {0x00, 0xaf, 0x5f},
    {0x00, 0xaf, 0x87},
    {0x00, 0xaf, 0xaf},
    {0x00, 0xaf, 0xd7},
    {0x00, 0xaf, 0xff},
    {0x00, 0xd7, 0x00},
    {0x00, 0xd7, 0x5f},
    {0x00, 0xd7, 0x87},
    {0x00, 0xd7, 0xaf},
    {0x00, 0xd7, 0xd7},
    {0x00, 0xd7, 0xff},
    {0x00, 0xff, 0x00},
    {0x00, 0xff, 0x5f},
    {0x00, 0xff, 0x87},
    {0x00, 0xff, 0xaf},
    {0x00, 0xff, 0xd7},
    {0x00, 0xff, 0xff},
    {0x5f, 0x00, 0x00},
    {0x5f, 0x00, 0x5f},
    {0x5f, 0x00, 0x87},
    {0x5f, 0x00, 0xaf},
    {0x5f, 0x00, 0xd7},
    {0x5f, 0x00, 0xff},
    {0x5f, 0x5f, 0x00},
    {0x5f, 0x5f, 0x5f},
    {0x5f, 0x5f, 0x87},
    {0x5f, 0x5f, 0xaf},
    {0x5f, 0x5f, 0xd7},
    {0x5f, 0x5f, 0xff},
    {0x5f, 0x87, 0x00},
    {0x5f, 0x87, 0x5f},
    {0x5f, 0x87, 0x87},
    {0x5f, 0x87, 0xaf},
    {0x5f, 0x87, 0xd7},
    {0x5f, 0x87, 0xff},
    {0x5f, 0xaf, 0x00},
    {0x5f, 0xaf, 0x5f},
    {0x5f, 0xaf, 0x87},
    {0x5f, 0xaf, 0xaf},
    {0x5f, 0xaf, 0xd7},
    {0x5f, 0xaf, 0xff},
    {0x5f, 0xd7, 0x00},
    {0x5f, 0xd7, 0x5f},
    {0x5f, 0xd7, 0x87},
    {0x5f, 0xd7, 0xaf},
    {0x5f, 0xd7, 0xd7},
    {0x5f, 0xd7, 0xff},
    {0x5f, 0xff, 0x00},
    {0x5f, 0xff, 0x5f},
    {0x5f, 0xff, 0x87},
    {0x5f, 0xff, 0xaf},
    {0x5f, 0xff, 0xd7},
    {0x5f, 0xff, 0xff},
    {0x87, 0x00, 0x00},
    {0x87, 0x00, 0x5f},
    {0x87, 0x00, 0x87},
    {0x87, 0x00, 0xaf},
    {0x87, 0x00, 0xd7},
    {0x87, 0x00, 0xff},
    {0x87, 0x5f, 0x00},
    {0x87, 0x5f, 0x5f},
    {0x87, 0x5f, 0x87},
    {0x87, 0x5f, 0xaf},
    {0x87, 0x5f, 0xd7},
    {0x87, 0x5f, 0xff},
    {0x87, 0x87, 0x00},
    {0x87, 0x87, 0x5f},
    {0x87, 0x87, 0x87},
    {0x87, 0x87, 0xaf},
    {0x87, 0x87, 0xd7},
    {0x87, 0x87, 0xff},
    {0x87, 0xaf, 0x00},
    {0x87, 0xaf, 0x5f},
    {0x87, 0xaf, 0x87},
    {0x87, 0xaf, 0xaf},
    {0x87, 0xaf, 0xd7},
    {0x87, 0xaf, 0xff},
    {0x87, 0xd7, 0x00},
    {0x87, 0xd7, 0x5f},
    {0x87, 0xd7, 0x87},
    {0x87, 0xd7, 0xaf},
    {0x87, 0xd7, 0xd7},
    {0x87, 0xd7, 0xff},
    {0x87, 0xff, 0x00},
    {0x87, 0xff, 0x5f},
    {0x87, 0xff, 0x87},
    {0x87, 0xff, 0xaf},
    {0x87, 0xff, 0xd7},
    {0x87, 0xff, 0xff},
    {0xaf, 0x00, 0x00},
    {0xaf, 0x00, 0x5f},
    {0xaf, 0x00, 0x87},
    {0xaf, 0x00, 0xaf},
    {0xaf, 0x00, 0xd7},
    {0xaf, 0x00, 0xff},
    {0xaf, 0x5f, 0x00},
    {0xaf, 0x5f, 0x5f},
    {0xaf, 0x5f, 0x87},
    {0xaf, 0x5f, 0xaf},
    {0xaf, 0x5f, 0xd7},
    {0xaf, 0x5f, 0xff},
    {0xaf, 0x87, 0x00},
    {0xaf, 0x87, 0x5f},
    {0xaf, 0x87, 0x87},
    {0xaf, 0x87, 0xaf},
    {0xaf, 0x87, 0xd7},
    {0xaf, 0x87, 0xff},
    {0xaf, 0xaf, 0x00},
    {0xaf, 0xaf, 0x5f},
    {0xaf, 0xaf, 0x87},
    {0xaf, 0xaf, 0xaf},
    {0xaf, 0xaf, 0xd7},
    {0xaf, 0xaf, 0xff},
    {0xaf, 0xd7, 0x00},
    {0xaf, 0xd7, 0x5f},
    {0xaf, 0xd7, 0x87},
    {0xaf, 0xd7, 0xaf},
    {0xaf, 0xd7, 0xd7},
    {0xaf, 0xd7, 0xff},
    {0xaf, 0xff, 0x00},
    {0xaf, 0xff, 0x5f},
    {0xaf, 0xff, 0x87},
    {0xaf, 0xff, 0xaf},
    {0xaf, 0xff, 0xd7},
    {0xaf, 0xff, 0xff},
    {0xd7, 0x00, 0x00},
    {0xd7, 0x00, 0x5f},
    {0xd7, 0x00, 0x87},
    {0xd7, 0x00, 0xaf},
    {0xd7, 0x00, 0xd7},
    {0xd7, 0x00, 0xff},
    {0xd7, 0x5f, 0x00},
    {0xd7, 0x5f, 0x5f},
    {0xd7, 0x5f, 0x87},
    {0xd7, 0x5f, 0xaf},
    {0xd7, 0x5f, 0xd7},
    {0xd7, 0x5f, 0xff},
    {0xd7, 0x87, 0x00},
    {0xd7, 0x87, 0x5f},
    {0xd7, 0x87, 0x87},
    {0xd7, 0x87, 0xaf},
    {0xd7, 0x87, 0xd7},
    {0xd7, 0x87, 0xff},
    {0xd7, 0xaf, 0x00},
    {0xd7, 0xaf, 0x5f},
    {0xd7, 0xaf, 0x87},
    {0xd7, 0xaf, 0xaf},
    {0xd7, 0xaf, 0xd7},
    {0xd7, 0xaf, 0xff},
    {0xd7, 0xd7, 0x00},
    {0xd7, 0xd7, 0x5f},
    {0xd7, 0xd7, 0x87},
    {0xd7, 0xd7, 0xaf},
    {0xd7, 0xd7, 0xd7},
    {0xd7, 0xd7, 0xff},
    {0xd7, 0xff, 0x00},
    {0xd7, 0xff, 0x5f},
    {0xd7, 0xff, 0x87},
    {0xd7, 0xff, 0xaf},
    {0xd7, 0xff, 0xd7},
    {0xd7, 0xff, 0xff},
    {0xff, 0x00, 0x00},
    {0xff, 0x00, 0x5f},
    {0xff, 0x00, 0x87},
    {0xff, 0x00, 0xaf},
    {0xff, 0x00, 0xd7},
    {0xff, 0x00, 0xff},
    {0xff, 0x5f, 0x00},
    {0xff, 0x5f, 0x5f},
    {0xff, 0x5f, 0x87},
    {0xff, 0x5f, 0xaf},
    {0xff, 0x5f, 0xd7},
    {0xff, 0x5f, 0xff},
    {0xff, 0x87, 0x00},
    {0xff, 0x87, 0x5f},
    {0xff, 0x87, 0x87},
    {0xff, 0x87, 0xaf},
    {0xff, 0x87, 0xd7},
    {0xff, 0x87, 0xff},
    {0xff, 0xaf, 0x00},
    {0xff, 0xaf, 0x5f},
    {0xff, 0xaf, 0x87},
    {0xff, 0xaf, 0xaf},
    {0xff, 0xaf, 0xd7},
    {0xff, 0xaf, 0xff},
    {0xff, 0xd7, 0x00},
    {0xff, 0xd7, 0x5f},
    {0xff, 0xd7, 0x87},
    {0xff, 0xd7, 0xaf},
    {0xff, 0xd7, 0xd7},
    {0xff, 0xd7, 0xff},
    {0xff, 0xff, 0x00},
    {0xff, 0xff, 0x5f},
    {0xff, 0xff, 0x87},
    {0xff, 0xff, 0xaf},
    {0xff, 0xff, 0xd7},
    {0xff, 0xff, 0xff},

    // 232-255 for greyscale;
    {0x08, 0x08, 0x08},
    {0x12, 0x12, 0x12},
    {0x1c, 0x1c, 0x1c},
    {0x26, 0x26, 0x26},
    {0x30, 0x30, 0x30},
    {0x3a, 0x3a, 0x3a},
    {0x44, 0x44, 0x44},
    {0x4e, 0x4e, 0x4e},
    {0x58, 0x58, 0x58},
    {0x62, 0x62, 0x62},
    {0x6c, 0x6c, 0x6c},
    {0x76, 0x76, 0x76},
    {0x80, 0x80, 0x80},
    {0x8a, 0x8a, 0x8a},
    {0x94, 0x94, 0x94},
    {0x9e, 0x9e, 0x9e},
    {0xa8, 0xa8, 0xa8},
    {0xb2, 0xb2, 0xb2},
    {0xbc, 0xbc, 0xbc},
    {0xc6, 0xc6, 0xc6},
    {0xd0, 0xd0, 0xd0},
    {0xda, 0xda, 0xda},
    {0xe4, 0xe4, 0xe4},
    {0xee, 0xee, 0xee},
};

// Convert an ansi 256 color to RGB, given a specific "palette" for the lower
// 16 colors.
void ansi256_to_rgb(
    int color, struct ansi_color_palette *palette, struct ansi_rgb *rgb
)
{
    static struct ansi_rgb rgb_error = {0xff, 0x66, 0xff};
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
        style->color_foreground.color_type = COLOR_TYPE_16;
        style->color_foreground.fg_or_bg = ANSI_FG;
        style->color_foreground.rgb = palette->base[7];
        style->color_background.color_type = COLOR_TYPE_16;
        style->color_background.fg_or_bg = ANSI_BG;
        style->color_background.rgb = palette->base[0];
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
            fg->color_type = COLOR_TYPE_16;
            fg->fg_or_bg = ANSI_FG;
            fg->rgb = palette->base[7];
            bg->color_type = COLOR_TYPE_16;
            bg->fg_or_bg = ANSI_BG;
            bg->rgb = palette->base[0];
            DEBUG(" 0 -> Reset all properties\n");
            break;
        case 1:
            props->bold = true;
            DEBUG(" 1 -> Bold\n");
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
            DEBUG("22 -> Neither bold nor faint\n");
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
            fg->color_type = COLOR_TYPE_16;
            fg->fg_or_bg = ANSI_FG;
            fg->rgb = palette->base[7];
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
                else if (sgr[i + 2] == 2) // 48;2;R;G;B
                {
                    if (i + 5 < len)
                    {
                        bg->color_type = COLOR_TYPE_24BIT;
                        bg->fg_or_bg = ANSI_BG;
                        bg->rgb.red = sgr[i + 3];
                        bg->rgb.green = sgr[i + 4];
                        bg->rgb.blue = sgr[i + 5];
                        DEBUG(
                            "%d;2;%d;%d;%d -> Background color 24-bit -> bg "
                            "RGB %02X%02X%02X\n",
                            sgr[i], sgr[i + 3], sgr[i + 4], sgr[i + 5],
                            bg->rgb.red, bg->rgb.green, bg->rgb.blue
                        );
                        i += 5;
                        break;
                    }
                    // Error?
                }
            }
            break;
        case 49: // Default background color
            bg->color_type = COLOR_TYPE_16;
            bg->fg_or_bg = ANSI_BG;
            bg->rgb = palette->base[0];
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
            ansi16_to_rgb(sgr[i] - 60, palette, &fg->rgb);
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
            ansi16_to_rgb(sgr[i] - 100, palette, &bg->rgb);
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

void ansi_style_span_style(
    struct ansi_style *s, struct ansi_color_palette *palette
)
{
    if (!s || !palette)
        return;
    struct ansi_style_properties *props = &s->style_properties;
    struct ansi_color *fg = &s->color_foreground;
    struct ansi_color *bg = &s->color_background;
    // RGB hex for color "7" in the palette for foreground should result in an
    // "initial" color value, and not its "RGB" value.
    if (fg->color_type == COLOR_TYPE_16 &&
        fg->rgb.red == palette->base[7].red &&
        fg->rgb.green == palette->base[7].green &&
        fg->rgb.blue == palette->base[7].blue)
        printf("%scolor:inherit;", props->reverse ? "background-" : "");
    else
        printf(
            "%scolor:#%02X%02X%02X;", props->reverse ? "background-" : "",
            fg->rgb.red, fg->rgb.green, fg->rgb.blue
        );
    // Same for background:
    if (bg->color_type == COLOR_TYPE_16 &&
        bg->rgb.red == palette->base[0].red &&
        bg->rgb.green == palette->base[0].green &&
        bg->rgb.blue == palette->base[0].blue)
        printf("%scolor:inherit;", props->reverse ? "" : "background-");
    else
        printf(
            "%scolor:#%02X%02X%02X;", props->reverse ? "" : "background-",
            bg->rgb.red, bg->rgb.green, bg->rgb.blue
        );
    if (!s->bold_is_bright && props->bold)
        printf("font-weight:bold;");
    else if (props->faint)
        printf("font-weight:lighter;");
    if (props->italic)
        printf("font-style:italic;");
    if (props->double_underline)
        printf("text-decoration:underline;border-bottom:3px double;");
    else if (props->underline)
        printf("text-decoration:underline;");
    if (props->slow_blink || props->fast_blink)
        printf("text-decoration:blink;");
    if (props->crossout)
        printf("text-decoration:line-through;");
    if (props->fraktur)
        printf("font-family:fraktur;");
    if (props->frame)
        printf("border:1px solid;");
    if (props->circle)
        printf("border:1px solid;border-radius:50%%;");
    if (props->overline)
        printf("text-decoration:overline;");
}

void reset_ansi_props(struct ansi_style *s, struct ansi_color_palette *palette)
{
    if (!s || !palette)
        return;
    s->style_properties = (struct ansi_style_properties){0};
    s->color_foreground.color_type = COLOR_TYPE_16;
    s->color_foreground.fg_or_bg = ANSI_FG;
    s->color_foreground.rgb = palette->base[7];
    s->color_background.color_type = COLOR_TYPE_16;
    s->color_background.fg_or_bg = ANSI_BG;
    s->color_background.rgb = palette->base[0];
}
