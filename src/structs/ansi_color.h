#ifndef ANSI_COLOR_H
#define ANSI_COLOR_H

#include "ansi_color_type.h"
#include "ansi_fg_or_bg.h"
#include "ansi_rgb.h"
#include <stdbool.h>

struct ansi_color
{
    // this can be COLOR_TYPE_16, COLOR_TYPE_256 or COLOR_TYPE_24BIT
    struct ansi_color_type *color_type;
    // this can be ANSI_BG or ANSI_FG
    struct ansi_fg_or_bg *fg_or_bg;
    // The RGB of this color:
    struct ansi_rgb rgb;
    // Whether the color was one of the lower 16 (useful for "bold is bright")
    bool is_base_color;
    // If it was a base color, which one?
    unsigned char base_color;
};

#endif // ANSI_COLOR_H
