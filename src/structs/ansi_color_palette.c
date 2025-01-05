#include "ansi_color_palette.h"

// See also: https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit
struct ansi_color_palette real_PALETTE_VGA = {
    "VGA",
    {.rgb = 0x000000},
    {.rgb = 0xAAAAAA},
    {
      {.rgb = 0x000000},
      {.rgb = 0xAA0000},
      {.rgb = 0x00AA00},
      {.rgb = 0xAA5500},
      {.rgb = 0x0000AA},
      {.rgb = 0xAA00AA},
      {.rgb = 0x00AAAA},
      {.rgb = 0xAAAAAA},
      },
    {
      {.rgb = 0x555555},
      {.rgb = 0xFF5555},
      {.rgb = 0x55FF55},
      {.rgb = 0xFFFF55},
      {.rgb = 0x5555FF},
      {.rgb = 0xFF55FF},
      {.rgb = 0x55FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_VGA = &real_PALETTE_VGA;

struct ansi_color_palette real_PALETTE_WINXP_CONSOLE = {
    "Windows XP Console",
    {.rgb = 0x000000},
    {.rgb = 0xC0C0C0},
    {
      {.rgb = 0x000000},
      {.rgb = 0x800000},
      {.rgb = 0x008000},
      {.rgb = 0x808000},
      {.rgb = 0x000080},
      {.rgb = 0x800080},
      {.rgb = 0x008080},
      {.rgb = 0xC0C0C0},
      },
    {
      {.rgb = 0x808080},
      {.rgb = 0xFF0000},
      {.rgb = 0x00FF00},
      {.rgb = 0xFFFF00},
      {.rgb = 0x0000FF},
      {.rgb = 0xFF00FF},
      {.rgb = 0x00FFFF},
      {.rgb = 0xFFFFFF},
      }
};
struct ansi_color_palette *PALETTE_WINXP_CONSOLE = &real_PALETTE_WINXP_CONSOLE;

struct ansi_color_palette real_PALETTE_POWERSHELL = {
    "Windows PowerShell 1.0-6.0",
    {.rgb = 0x000000},
    {.rgb = 0xC0C0C0},
    {
      {.rgb = 0x000000},
      {.rgb = 0x800000},
      {.rgb = 0x008000},
      {.rgb = 0xEEEDF0},
      {.rgb = 0x000080},
      {.rgb = 0x012456},
      {.rgb = 0x008080},
      {.rgb = 0xC0C0C0},
      },
    {
      {.rgb = 0x808080},
      {.rgb = 0xFF0000},
      {.rgb = 0x00FF00},
      {.rgb = 0xFFFF00},
      {.rgb = 0x0000FF},
      {.rgb = 0xFF00FF},
      {.rgb = 0x00FFFF},
      {.rgb = 0xFFFFFF},
      }
};
struct ansi_color_palette *PALETTE_POWERSHELL = &real_PALETTE_POWERSHELL;

struct ansi_color_palette real_PALETTE_VSCODE = {
    "Visual Studio Code",
    {.rgb = 0x000000},
    {.rgb = 0xE5E5E5},
    {
      {.rgb = 0x000000},
      {.rgb = 0xCD3131},
      {.rgb = 0x0DBC79},
      {.rgb = 0xE5E510},
      {.rgb = 0x2472C8},
      {.rgb = 0xBC3FBC},
      {.rgb = 0x11A8CD},
      {.rgb = 0xE5E5E5},
      },
    {
      {.rgb = 0x666666},
      {.rgb = 0xF14C4C},
      {.rgb = 0x23D18B},
      {.rgb = 0xF5F543},
      {.rgb = 0x3B8EEA},
      {.rgb = 0xD670D6},
      {.rgb = 0x29B8DB},
      {.rgb = 0xE5E5E5},
      },
};
struct ansi_color_palette *PALETTE_VSCODE = &real_PALETTE_VSCODE;

struct ansi_color_palette real_PALETTE_WIN10_CONSOLE = {
    "Windows 10 Console",
    {.rgb = 0x0C0C0C},
    {.rgb = 0xCCCCCC},
    {
      {.rgb = 0x0C0C0C},
      {.rgb = 0xC50F1F},
      {.rgb = 0x13A10E},
      {.rgb = 0xC19C00},
      {.rgb = 0x0037DA},
      {.rgb = 0x881798},
      {.rgb = 0x3A96DD},
      {.rgb = 0xCCCCCC},
      },
    {
      {.rgb = 0x767676},
      {.rgb = 0xE74856},
      {.rgb = 0x16C60C},
      {.rgb = 0xF9F1A5},
      {.rgb = 0x3B78FF},
      {.rgb = 0xB4009E},
      {.rgb = 0x61D6D6},
      {.rgb = 0xF2F2F2},
      }
};
struct ansi_color_palette *PALETTE_WIN10_CONSOLE = &real_PALETTE_WIN10_CONSOLE;

struct ansi_color_palette real_PALETTE_TERMINAL_APP = {
    "Terminal.app",
    {.rgb = 0x000000},
    {.rgb = 0xBFBFBF},
    {
      {.rgb = 0x000000},
      {.rgb = 0x990000},
      {.rgb = 0x00A600},
      {.rgb = 0x999900},
      {.rgb = 0x0000B2},
      {.rgb = 0xB200B2},
      {.rgb = 0x00A6B2},
      {.rgb = 0xBFBFBF},
      },
    {
      {.rgb = 0x666666},
      {.rgb = 0xE60000},
      {.rgb = 0x00D900},
      {.rgb = 0xE6E600},
      {.rgb = 0x0000FF},
      {.rgb = 0xE600E6},
      {.rgb = 0x00E6E6},
      {.rgb = 0xE6E6E6},
      },
};
struct ansi_color_palette *PALETTE_TERMINAL_APP = &real_PALETTE_TERMINAL_APP;

struct ansi_color_palette real_PALETTE_PUTTY = {
    "PuTTY",
    {.rgb = 0x000000},
    {.rgb = 0xBBBBBB},
    {
      {.rgb = 0x000000},
      {.rgb = 0xBB0000},
      {.rgb = 0x00BB00},
      {.rgb = 0xBBBB00},
      {.rgb = 0x0000BB},
      {.rgb = 0xBB00BB},
      {.rgb = 0x00BBBB},
      {.rgb = 0xBBBBBB},
      },
    {
      {.rgb = 0x555555},
      {.rgb = 0xFF5555},
      {.rgb = 0x55FF55},
      {.rgb = 0xFFFF55},
      {.rgb = 0x5555FF},
      {.rgb = 0xFF55FF},
      {.rgb = 0x55FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_PUTTY = &real_PALETTE_PUTTY;

struct ansi_color_palette real_PALETTE_MIRC = {
    "mIRC",
    {.rgb = 0x000000},
    {.rgb = 0xD2D2D2},
    {
      {.rgb = 0x000000},
      {.rgb = 0x7F0000},
      {.rgb = 0x009300},
      {.rgb = 0xFC7F00},
      {.rgb = 0x00007F},
      {.rgb = 0x9C009C},
      {.rgb = 0x009393},
      {.rgb = 0xD2D2D2},
      },
    {
      {.rgb = 0x7F7F7F},
      {.rgb = 0xFF0000},
      {.rgb = 0x00FC00},
      {.rgb = 0xFFFF00},
      {.rgb = 0x0000FC},
      {.rgb = 0xFF00FF},
      {.rgb = 0x00FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_MIRC = &real_PALETTE_MIRC;

struct ansi_color_palette real_PALETTE_XTERM = {
    "xterm",
    {.rgb = 0x000000},
    {.rgb = 0xE5E5E5},
    {
      {.rgb = 0x000000},
      {.rgb = 0xCD0000},
      {.rgb = 0x00CD00},
      {.rgb = 0xCDCD00},
      {.rgb = 0x0000EE},
      {.rgb = 0xCD00CD},
      {.rgb = 0x00CDCD},
      {.rgb = 0xE5E5E5},
      },
    {
      {.rgb = 0x7F7F7F},
      {.rgb = 0xFF0000},
      {.rgb = 0x00FF00},
      {.rgb = 0xFFFF00},
      {.rgb = 0x5C5CFF},
      {.rgb = 0xFF00FF},
      {.rgb = 0x00FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_XTERM = &real_PALETTE_XTERM;

struct ansi_color_palette real_PALETTE_UBUNTU = {
    "ubuntu",
    {.rgb = 0x010101},
    {.rgb = 0xCCCCCC},
    {
      {.rgb = 0x010101},
      {.rgb = 0xDE382B},
      {.rgb = 0x39B54A},
      {.rgb = 0xFFC706},
      {.rgb = 0x006FB8},
      {.rgb = 0x762671},
      {.rgb = 0x2CB5E9},
      {.rgb = 0xCCCCCC},
      },
    {
      {.rgb = 0x808080},
      {.rgb = 0xFF0000},
      {.rgb = 0x00FF00},
      {.rgb = 0xFFFF00},
      {.rgb = 0x0000FF},
      {.rgb = 0xFF00FF},
      {.rgb = 0x00FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_UBUNTU = &real_PALETTE_UBUNTU;

struct ansi_color_palette real_PALETTE_ECLIPSE_TERMINAL = {
    "Eclipse Terminal",
    {.rgb = 0x000000},
    {.rgb = 0xE5E5E5},
    {
      {.rgb = 0x000000},
      {.rgb = 0xCD0000},
      {.rgb = 0x00CD00},
      {.rgb = 0xCDCD00},
      {.rgb = 0x0000EE},
      {.rgb = 0xCD00CD},
      {.rgb = 0x00CDCD},
      {.rgb = 0xE5E5E5},
      },
    {
      {.rgb = 0x000000},
      {.rgb = 0xFF0000},
      {.rgb = 0x00FF00},
      {.rgb = 0xFFFF00},
      {.rgb = 0x5C5CFF},
      {.rgb = 0xFF00FF},
      {.rgb = 0x00FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_ECLIPSE_TERMINAL =
    &real_PALETTE_ECLIPSE_TERMINAL;

// https://spec.draculatheme.com/
struct ansi_color_palette real_PALETTE_DRACULA = {
    "Dracula",
    {.rgb = 0x21222C},
    {.rgb = 0xF8F8F2},
    {
      {.rgb = 0x21222C},
      {.rgb = 0xFF5555},
      {.rgb = 0x50FA7B},
      {.rgb = 0xF1FA8C},
      {.rgb = 0xBD93F9},
      {.rgb = 0xFF79C6},
      {.rgb = 0x8BE9FD},
      {.rgb = 0xF8F8F2},
      },
    {
      {.rgb = 0x6272A4},
      {.rgb = 0xFF6E6E},
      {.rgb = 0x69FF94},
      {.rgb = 0xFFFFA5},
      {.rgb = 0xD6ACFF},
      {.rgb = 0xFF92DF},
      {.rgb = 0xA4FFFF},
      {.rgb = 0xFFFFFF},
      },
};
struct ansi_color_palette *PALETTE_DRACULA = &real_PALETTE_DRACULA;
