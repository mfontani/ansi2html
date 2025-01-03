#include "structs/ansi_color_palette.h"
#include "structs/ansi_style.h"
#include <stddef.h>

#ifdef _GNU_SOURCE
#define PUTC(c) (void)fputc_unlocked(c, stdout)
#define PUTS(s) (void)fputs_unlocked(s, stdout)
#define GETCHAR() getchar_unlocked()
#define FREAD(buf, size, count, stream) fread_unlocked(buf, size, count, stream)
#define FWRITE(buf, size, count, stream)                                       \
    fwrite_unlocked(buf, size, count, stream)
#else
#define PUTC(c) (void)fputc(c, stdout)
#define PUTS(s) (void)fputs(s, stdout)
#define GETCHAR() getchar()
#define FREAD(buf, size, count, stream) fread(buf, size, count, stream)
#define FWRITE(buf, size, count, stream) fwrite(buf, size, count, stream)
#endif

extern void set_ansi_style_properties(
    struct ansi_color_palette *palette, struct ansi_style *style,
    unsigned char *sgr, size_t len
);

extern void show_ansi_style(
    struct ansi_style *s, const char *file, int line, const char *funcname
);

// Returns a static string. Beware.
extern char *ansi_span_start(
    struct ansi_style *s, struct ansi_color_palette *palette, bool use_classes,
    bool use_compact
);

extern void
reset_ansi_props(struct ansi_style *s, struct ansi_color_palette *palette);

extern void showcase_palette(struct ansi_color_palette *palette);

extern void ansi256_to_rgb(
    int color, struct ansi_color_palette *palette, struct ansi_rgb *rgb
);

extern void show_additional_styles(void);
