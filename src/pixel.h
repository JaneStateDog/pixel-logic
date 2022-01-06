#ifndef PIXEL
#define PIXEL

#include <stdint.h>


typedef struct XY {
    uint16_t x;
    uint16_t y;
} xy;

typedef struct Pixel {
    xy pos;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a; // If a pixel has an alpha of 0, we consider it invalid and it can be overwritten
} pixel;


#endif