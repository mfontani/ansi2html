#ifndef ANSI_RGB_H
#define ANSI_RGB_H

struct ansi_rgb
{
    union {
        struct {
#ifndef __BYTE_ORDER__
#error "Please define __BYTE_ORDER__"
#endif
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            unsigned char blue;  // 0-255
            unsigned char green; // 0-255
#else
            unsigned char green; // 0-255
            unsigned char blue;  // 0-255
#endif
            unsigned char red;   // 0-255
        };
        int rgb; // 0xRRGGBB
    };
};

#endif // ANSI_RGB_H
