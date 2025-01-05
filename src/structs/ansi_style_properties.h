#ifndef ANSI_STYLE_PROPERTIES_H
#define ANSI_STYLE_PROPERTIES_H

#include <stdbool.h>

struct ansi_style_properties
{
    union
    {
        struct
        {
            bool bold : 1;
            bool faint : 1;
            bool italic : 1;
            bool underline : 1;
            bool slow_blink : 1;
            bool fast_blink : 1;
            bool reverse : 1;
            bool crossout : 1;
            bool fraktur : 1;
            bool double_underline : 1;
            bool frame : 1;
            bool circle : 1;
            bool overline : 1;
        };
        unsigned short any;
    };
};

#endif // ANSI_STYLE_PROPERTIES_H
