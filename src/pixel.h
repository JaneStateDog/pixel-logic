#ifndef PIXEL
#define PIXEL

#include <SDL2/SDL.h>


typedef struct Pixel {
    int x;
    int y;

    Uint16 r;
    Uint16 g;
    Uint16 b;
    Uint16 a;
} pixel;


#endif