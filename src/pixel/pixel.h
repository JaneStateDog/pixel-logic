#ifndef PIXEL
#define PIXEL

#include <stdint.h>
#include <stdbool.h>


typedef struct XY {
    uint16_t x;
    uint16_t y;
} xy;
bool cmp_xy(xy xy1, xy xy2);


typedef struct Pixel {
    xy pos;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} pixel;


#endif