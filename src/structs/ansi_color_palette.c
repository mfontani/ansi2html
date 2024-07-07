#include "ansi_color_palette.h"

// See also: https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit
struct ansi_color_palette real_PALETTE_VGA = {
    "VGA",
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

struct ansi_color_palette real_PALETTE_WIN10_CONSOLE = {
    "Windows 10 Console",
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
