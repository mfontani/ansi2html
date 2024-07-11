#include "structs/ansi_color_palette.h"
#include "structs/ansi_style.h"
#include <stddef.h>

extern void set_ansi_style_properties(
    struct ansi_color_palette *palette, struct ansi_style *style,
    unsigned char *sgr, size_t len
);

extern void show_ansi_style(
    struct ansi_style *s, const char *file, int line, const char *funcname
);

// Returns a static string. Beware.
extern char *ansi_span_start(
    struct ansi_style *s, struct ansi_color_palette *palette, bool use_classes
);

extern void
reset_ansi_props(struct ansi_style *s, struct ansi_color_palette *palette);

extern void showcase_palette(struct ansi_color_palette *palette);

extern void ansi256_to_rgb(
    int color, struct ansi_color_palette *palette, struct ansi_rgb *rgb
);
