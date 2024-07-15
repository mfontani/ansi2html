#ifndef ANSI_COLOR_PALETTE_H
#define ANSI_COLOR_PALETTE_H

#include "ansi_rgb.h"

// Color conversion may work on "palettes", for the 16 base colors, as many
// terminal types have their own idea about how those colors should look like.
struct ansi_color_palette
{
    // "name" of the palette.
    char *name;
    // Preferred RGB for the background color
    struct ansi_rgb default_bg;
    // Preferred RGB for the foreground color
    struct ansi_rgb default_fg;
    // RGB for the 8 "base" colors (30-37 and 40-47)
    struct ansi_rgb base[8];
    // RGB for the 8 "bright" colors (either 1;30-37 or 90-97; or 100-107)
    struct ansi_rgb bright[8];
};

extern struct ansi_color_palette *PALETTE_VGA;
extern struct ansi_color_palette *PALETTE_WINXP_CONSOLE;
extern struct ansi_color_palette *PALETTE_POWERSHELL;
extern struct ansi_color_palette *PALETTE_WIN10_CONSOLE;
extern struct ansi_color_palette *PALETTE_VSCODE;
extern struct ansi_color_palette *PALETTE_TERMINAL_APP;
extern struct ansi_color_palette *PALETTE_PUTTY;
extern struct ansi_color_palette *PALETTE_MIRC;
extern struct ansi_color_palette *PALETTE_XTERM;
extern struct ansi_color_palette *PALETTE_UBUNTU;
extern struct ansi_color_palette *PALETTE_ECLIPSE_TERMINAL;
extern struct ansi_color_palette *PALETTE_DRACULA;

#include "../iterm2_color_schemes/ansi_color_palette.h"

#endif // ANSI_COLOR_PALETTE_H
