#include "ansi_color_palette.h"

// See also: https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit
struct ansi_color_palette real_PALETTE_VGA = {
    "VGA",
    {0, 0, 0},
    {170, 170, 170},
    {
      {0, 0, 0},
      {170, 0, 0},
      {0, 170, 0},
      {170, 85, 0},
      {0, 0, 170},
      {170, 0, 170},
      {0, 170, 170},
      {170, 170, 170},
      },
    {
      {85, 85, 85},
      {255, 85, 85},
      {85, 255, 85},
      {255, 255, 85},
      {85, 85, 255},
      {255, 85, 255},
      {85, 255, 255},
      {255, 255, 255},
      },
};
struct ansi_color_palette *PALETTE_VGA = &real_PALETTE_VGA;

struct ansi_color_palette real_PALETTE_WINXP_CONSOLE = {
    "Windows XP Console",
    {0, 0, 0},
    {192, 192, 192},
    {
      {0, 0, 0},
      {128, 0, 0},
      {0, 128, 0},
      {128, 128, 0},
      {0, 0, 128},
      {128, 0, 128},
      {0, 128, 128},
      {192, 192, 192},
      },
    {
      {128, 128, 128},
      {255, 0, 0},
      {0, 255, 0},
      {255, 255, 0},
      {0, 0, 255},
      {255, 0, 255},
      {0, 255, 255},
      {255, 255, 255},
      }
};
struct ansi_color_palette *PALETTE_WINXP_CONSOLE = &real_PALETTE_WINXP_CONSOLE;

struct ansi_color_palette real_PALETTE_POWERSHELL = {
    "Windows PowerShell 1.0-6.0",
    {0, 0, 0},
    {192, 192, 192},
    {
      {0, 0, 0},
      {128, 0, 0},
      {0, 128, 0},
      {238, 237, 240},
      {0, 0, 128},
      {1, 36, 86},
      {0, 128, 128},
      {192, 192, 192},
      },
    {
      {128, 128, 128},
      {255, 0, 0},
      {0, 255, 0},
      {255, 255, 0},
      {0, 0, 255},
      {255, 0, 255},
      {0, 255, 255},
      {255, 255, 255},
      }
};
struct ansi_color_palette *PALETTE_POWERSHELL = &real_PALETTE_POWERSHELL;

struct ansi_color_palette real_PALETTE_VSCODE = {
    "Visual Studio Code",
    {0, 0, 0},
    {229, 229, 229},
    {
      {0, 0, 0},
      {205, 49, 49},
      {13, 188, 121},
      {229, 229, 16},
      {36, 114, 200},
      {188, 63, 188},
      {17, 168, 205},
      {229, 229, 229},
      },
    {
      {102, 102, 102},
      {241, 76, 76},
      {35, 209, 139},
      {245, 245, 67},
      {59, 142, 234},
      {214, 112, 214},
      {41, 184, 219},
      {229, 229, 229},
      },
};
struct ansi_color_palette *PALETTE_VSCODE = &real_PALETTE_VSCODE;

struct ansi_color_palette real_PALETTE_WIN10_CONSOLE = {
    "Windows 10 Console",
    {12, 12, 12},
    {204, 204, 204},
    {
      {12, 12, 12},
      {197, 15, 31},
      {19, 161, 14},
      {193, 156, 0},
      {0, 55, 218},
      {136, 23, 152},
      {58, 150, 221},
      {204, 204, 204},
      },
    {
      {118, 118, 118},
      {231, 72, 86},
      {22, 198, 12},
      {249, 241, 165},
      {59, 120, 255},
      {180, 0, 158},
      {97, 214, 214},
      {242, 242, 242},
      }
};
struct ansi_color_palette *PALETTE_WIN10_CONSOLE = &real_PALETTE_WIN10_CONSOLE;

struct ansi_color_palette real_PALETTE_TERMINAL_APP = {
    "Terminal.app",
    {0, 0, 0},
    {191, 191, 191},
    {
      {0, 0, 0},
      {153, 0, 0},
      {0, 166, 0},
      {153, 153, 0},
      {0, 0, 178},
      {178, 0, 178},
      {0, 166, 178},
      {191, 191, 191},
      },
    {
      {102, 102, 102},
      {230, 0, 0},
      {0, 217, 0},
      {230, 230, 0},
      {0, 0, 255},
      {230, 0, 230},
      {0, 230, 230},
      {230, 230, 230},
      },
};
struct ansi_color_palette *PALETTE_TERMINAL_APP = &real_PALETTE_TERMINAL_APP;

struct ansi_color_palette real_PALETTE_PUTTY = {
    "PuTTY",
    {0, 0, 0},
    {187, 187, 187},
    {
      {0, 0, 0},
      {187, 0, 0},
      {0, 187, 0},
      {187, 187, 0},
      {0, 0, 187},
      {187, 0, 187},
      {0, 187, 187},
      {187, 187, 187},
      },
    {
      {85, 85, 85},
      {255, 85, 85},
      {85, 255, 85},
      {255, 255, 85},
      {85, 85, 255},
      {255, 85, 255},
      {85, 255, 255},
      {255, 255, 255},
      },
};
struct ansi_color_palette *PALETTE_PUTTY = &real_PALETTE_PUTTY;

struct ansi_color_palette real_PALETTE_MIRC = {
    "mIRC",
    {0, 0, 0},
    {210, 210, 210},
    {
      {0, 0, 0},
      {127, 0, 0},
      {0, 147, 0},
      {252, 127, 0},
      {0, 0, 127},
      {156, 0, 156},
      {0, 147, 147},
      {210, 210, 210},
      },
    {
      {127, 127, 127},
      {255, 0, 0},
      {0, 252, 0},
      {255, 255, 0},
      {0, 0, 252},
      {255, 0, 255},
      {0, 255, 255},
      {255, 255, 255},
      },
};
struct ansi_color_palette *PALETTE_MIRC = &real_PALETTE_MIRC;

struct ansi_color_palette real_PALETTE_XTERM = {
    "xterm",
    {0, 0, 0},
    {229, 229, 229},
    {
      {0, 0, 0},
      {205, 0, 0},
      {0, 205, 0},
      {205, 205, 0},
      {0, 0, 238},
      {205, 0, 205},
      {0, 205, 205},
      {229, 229, 229},
      },
    {
      {127, 127, 127},
      {255, 0, 0},
      {0, 255, 0},
      {255, 255, 0},
      {92, 92, 255},
      {255, 0, 255},
      {0, 255, 255},
      {255, 255, 255},
      },
};
struct ansi_color_palette *PALETTE_XTERM = &real_PALETTE_XTERM;

struct ansi_color_palette real_PALETTE_UBUNTU = {
    "ubuntu",
    {1, 1, 1},
    {204, 204, 204},
    {
      {1, 1, 1},
      {222, 56, 43},
      {57, 181, 74},
      {255, 199, 6},
      {0, 111, 184},
      {118, 38, 113},
      {44, 181, 233},
      {204, 204, 204},
      },
    {
      {128, 128, 128},
      {255, 0, 0},
      {0, 255, 0},
      {255, 255, 0},
      {0, 0, 255},
      {255, 0, 255},
      {0, 255, 255},
      {255, 255, 255},
      },
};
struct ansi_color_palette *PALETTE_UBUNTU = &real_PALETTE_UBUNTU;

struct ansi_color_palette real_PALETTE_ECLIPSE_TERMINAL = {
    "Eclipse Terminal",
    {0, 0, 0},
    {229, 229, 229},
    {
      {0, 0, 0},
      {205, 0, 0},
      {0, 205, 0},
      {205, 205, 0},
      {0, 0, 238},
      {205, 0, 205},
      {0, 205, 205},
      {229, 229, 229},
      },
    {
      {0, 0, 0},
      {255, 0, 0},
      {0, 255, 0},
      {255, 255, 0},
      {92, 92, 255},
      {255, 0, 255},
      {0, 255, 255},
      {255, 255, 255},
      },
};
struct ansi_color_palette *PALETTE_ECLIPSE_TERMINAL =
    &real_PALETTE_ECLIPSE_TERMINAL;

// https://spec.draculatheme.com/
struct ansi_color_palette real_PALETTE_DRACULA = {
    "Dracula",
    {0x21, 0x22, 0x2C},
    {0xF8, 0xF8, 0xF2},
    {
      {0x21, 0x22, 0x2C},
      {0xFF, 0x55, 0x55},
      {0x50, 0xFA, 0x7B},
      {0xF1, 0xFA, 0x8C},
      {0xBD, 0x93, 0xF9},
      {0xFF, 0x79, 0xC6},
      {0x8B, 0xE9, 0xFD},
      {0xF8, 0xF8, 0xF2},
      },
    {
      {0x62, 0x72, 0xA4},
      {0xFF, 0x6E, 0x6E},
      {0x69, 0xFF, 0x94},
      {0xFF, 0xFF, 0xA5},
      {0xD6, 0xAC, 0xFF},
      {0xFF, 0x92, 0xDF},
      {0xA4, 0xFF, 0xFF},
      {0xFF, 0xFF, 0xFF},
      },
};
struct ansi_color_palette *PALETTE_DRACULA = &real_PALETTE_DRACULA;
