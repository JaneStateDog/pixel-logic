#ifndef PIXEL
#define PIXEL

#include <stdint.h>
#include <stdbool.h>
#include "../uthash.h"


typedef struct XY {
    uint16_t x;
    uint16_t y;
} xy;
bool cmp_xy(xy xy1, xy xy2);


typedef struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} rgba;


/*
    Base pixel/chip types:
    - AND gate (two inputs, one output)
    - OR gate (two inputs, one output)
    - NOT gate (one inputs, one output)
    - SR Latch (two inputs, one output)

    Wire (wires should be colorable, but this is just the basics for now):
    - Off (white)
    - On (red)
*/

typedef struct Pixel {
    xy pos;
    rgba color;

    UT_hash_handle hh; // To make this structure hashable using uthash
} pixel;
void insert_pixel(xy pos, rgba color);
void delete_pixel(xy pos);

// The pixels hashtable
extern pixel *pixels;


#endif