#ifndef GLOBALS
#define GLOBALS

#include <SDL2/SDL.h>


#define DEFAULT_PIXEL_COLOR (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255}

#define RED (SDL_Color){.r = 255, .g = 0, .b = 0, .a = 255}
#define GREEN (SDL_Color){.r = 0, .g = 255, .b = 0, .a = 255}
#define BLUE (SDL_Color){.r = 0, .g = 0, .b = 255, .a = 255}
#define GREY (SDL_Color){.r = 100, .g = 100, .b = 100, .a = 255}


#define WIDTH 16 // Game width and height, also known as how many pixels can be on the screen with a multiple of 1
#define HEIGHT 9
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720


extern SDL_Renderer *renderer;

extern int multiple;
extern int placeMultiple;


#endif