#ifndef ANSI_STYLE_H
#define ANSI_STYLE_H

#include "ansi_color.h"
#include "ansi_style_properties.h"
#include <stdbool.h>

// The next "chunk of text" has this style:
struct ansi_style
{
    struct ansi_style_properties style_properties;
    struct ansi_color color_foreground;
    struct ansi_color color_background;
    bool bold_is_bright;
};

#endif // ANSI_STYLE_H
