#ifndef PIXEL
#define PIXEL

#include <stdint.h>
#include <SDL2/SDL.h>
#include "../uthash.h"


void draw_point(SDL_Point pos, SDL_Color color); // For drawing points in our rendering system


typedef struct {
    SDL_Point pos;
    SDL_Color color;

    SDL_bool state; // on/true or off/false

    SDL_bool touched;

    UT_hash_handle hh; // To make this structure hashable using uthash
} pixel;
pixel *find_pixel(SDL_Point pos);
void insert_pixel(SDL_Point pos, SDL_Color color);
void delete_pixel(SDL_Point pos);
void draw_pixel(pixel *p);
void recursive_change_pixel_state(pixel *p, SDL_bool state);

// The pixels hashtable
extern pixel *pixels;


/*
    Base pixel/chip types:
    - BUTTON (one output) (more of a switch than a button, but eh (if we put actual buttons in later on this is going to be a pain in the butt))
    - AND (two inputs, one output)
    - OR (two inputs, one output)
    - NOT (one inputs, one output)
    - SR_LATCH (two inputs, one output)

    Output is blue, input in green

    Wire (wires should be colorable, but this is just the basics for now):
    - off (white, aka DEFAULT_PIXEL_COLOR)
    - on (red)
*/
typedef enum {
    BUTTON,
    AND,
    OR,
    NOT,
    SR_LATCH
} chip_types;


#define CHIP_BODY_MAX 3
#define CHIP_BODY_EXPRESSION SDL_Color[CHIP_BODY_MAX][CHIP_BODY_MAX]
typedef struct {
    chip_types type;
    SDL_bool *memory; // Only the SR_LATCH uses this (for now.) It is an array of bools. For the SR_LATCH, since it is a 1 bit gate,
    // we only use the first index of the arrary. The reason we use an array at all is in case in the future we need
    // other types of gates that use more bits of memory (even though there is no plan for that)
    // For gates that don't use memory, have this just go to NULL
    // ACTUALLY a lot of that is a lie lel, BUTTON now uses a bit of memory since it's basicaly a switch haha sry future and past me(s?)

    SDL_Point pos;
    SDL_Rect rect;
    SDL_Color (*body)[CHIP_BODY_MAX]; // A 2D array holding the data about how this chip should appear and where the inputs and outputs are
    // We have to give this 2D array a set maximum size, so for now we'll do 3x3 and we can expand later if needed

    UT_hash_handle hh; // To make this structure hashable
} chip;
chip *find_chip(SDL_Point pos, SDL_bool ignoreNonNulls, SDL_Point *relativePos);
void init_chip(chip *c);
void insert_chip(SDL_Point pos, chip_types type);
void delete_chip(SDL_Point pos);
void draw_chip(chip *c);
void update_chip(chip *c);

// The chips hashtable
extern chip *chips;


#endif