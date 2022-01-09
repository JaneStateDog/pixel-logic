#include "pixel.h"

#include "../globals/globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "../uthash.h"


void draw_point(SDL_Point pos, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int winWidthConversion = round(WIN_WIDTH / (WIDTH * multiple));
    int winHeightConversion = round(WIN_HEIGHT / (HEIGHT * multiple));

    SDL_RenderFillRect(renderer, &((SDL_Rect){.x = pos.x * winWidthConversion, .y = pos.y * winHeightConversion, 
                                    .w = winWidthConversion, .h = winHeightConversion}));
}


// Initalize the pixels hashtable as null
pixel *pixels = NULL;

pixel *find_pixel(SDL_Point pos) {
    pixel *p;
    HASH_FIND(hh, pixels, &pos, sizeof(SDL_Point), p);

    return p;
}

void insert_pixel(SDL_Point pos, SDL_Color color) {
    // Look in the hashtable to see if the inputted pos/key already exists
    pixel *p = find_pixel(pos);

    // If the inputted pos/key does not exist, create a new pixel
    // Otherwise just readjust the data in the pixel
    if (p == NULL) {
        // Confirm the pixel we are trying to place is not on top of a chip
        chip *c = find_chip(pos, SDL_TRUE);
        if (c != NULL) { return; }

        // Create this new pixel and store a pointer to it
        pixel *p = calloc(1, sizeof(pixel)); // We use calloc because uthash requires our structures to be zeroed

        // Set the data of this new pixel
        p->pos = pos;
        p->color = color;

        // Add it to the hashtable
        // Bro macros are wack. Instead of putting in "p->pos" or "p->hh" or whatever, I just put the name of the variable in and it uses it with the structure?? That's so cool
        HASH_ADD(hh, pixels, pos, sizeof(SDL_Point), p); 
    } else {
        p->color = color;
    }
}

void delete_pixel(SDL_Point pos) {
    // First, find the pointer to the pixel
    pixel *p = find_pixel(pos);

    // If we found a pixel, delete it
    if (p != NULL) {
        HASH_DEL(pixels, p);
        free(p);
    }
}

void draw_pixel(pixel *p) {
    SDL_Color color = p->color;

    if (p->state == SDL_TRUE) { color = RED; }

    draw_point(p->pos, color);
}

void recursive_change_pixel_state(pixel *p, SDL_bool state) {
    if (p->state != state) {
        p->state = state;

        pixel *ps[4] = {find_pixel((SDL_Point){.x = p->pos.x + 1, .y = p->pos.y}), find_pixel((SDL_Point){.x = p->pos.x - 1, .y = p->pos.y}),
                        find_pixel((SDL_Point){.x = p->pos.x, .y = p->pos.y + 1}), find_pixel((SDL_Point){.x = p->pos.x, .y = p->pos.y - 1})};

        for (int i = 0; i < 4; i++) {
            if (ps[i] != NULL && ps[i]->state != state) {
                recursive_change_pixel_state(ps[i], state);
            }
        }
    }
}


// Initalize the chips hashtable as null
chip *chips = NULL;

chip *find_chip(SDL_Point pos, SDL_bool ignoreNonNulls) {
    // Look to see if there is a chip at the inputted position
    chip *c;
    HASH_FIND(hh, chips, &pos, sizeof(SDL_Point), c);

    // If ignore non nulls is true, we need to make sure we check that upper left corner instead of ignoring it
    if (ignoreNonNulls == SDL_TRUE) { c = NULL; }

    // If there wasn't a chip, it's time to do an ugly brute forcey method to test if the inputted position is in the
    // body of any chips
    if (c == NULL) {
        chip *ic;
        for (ic = chips; ic != NULL; ic = ic->hh.next) {
            if (SDL_PointInRect(&pos, &((SDL_Rect){.x = ic->pos.x, .y = ic->pos.y, .w = ic->rect.w, .h = ic->rect.h}))) {
                if (ignoreNonNulls == SDL_TRUE) {
                    SDL_Color color = ic->body[pos.x - ic->pos.x][pos.y - ic->pos.y];
                    if (!(color.r == GREY.r && color.g == GREY.g && color.b == GREY.b && color.a == GREY.a)) { continue; }
                }

                c = ic;
                break;
            }
        }
    }

    return c;
}

void init_chip(chip *c) {
    if (c->body != NULL) { free(c->body); } // If we've already initalized this chip, make sure to free the body before making a new one
    c->body = calloc(CHIP_BODY_MAX * CHIP_BODY_MAX, sizeof(SDL_Color));

    // Initalize the chip's data depending on it's type
    switch(c->type) {
        case BUTTON:
            c->body[0][0] = BLUE;
            c->body[0][1] = GREY;
            c->rect = (SDL_Rect){.w = 1, .h = 2};

            c->memory = calloc(1, sizeof(SDL_bool));
            c->memory[0] = SDL_FALSE;

            break;
        case AND:
            c->body[0][0] = GREY; c->body[1][0] = BLUE; c->body[2][0] = GREY;
            c->body[0][1] = GREEN; c->body[1][1] = GREY; c->body[2][1] = GREEN;
            c->rect = (SDL_Rect){.w = 3, .h = 2};

            c->memory = NULL;

            break;
        case OR:
            c->body[0][0] = GREY; c->body[1][0] = BLUE; c->body[2][0] = GREY;
            c->body[0][1] = GREEN; c->body[1][1] = GREY; c->body[2][1] = GREEN;
            c->rect = (SDL_Rect){.w = 3, .h = 2};

            c->memory = NULL;

            break;
        case NOT:
            c->body[0][0] = BLUE;
            c->body[0][1] = GREY;
            c->body[0][2] = GREEN;
            c->rect = (SDL_Rect){.w = 1, .h = 3};

            c->memory = NULL;

            break;
        case SR_LATCH:
            c->body[0][0] = GREY; c->body[1][0] = BLUE; c->body[2][0] = GREY;
            c->body[0][1] = GREEN; c->body[1][1] = GREY; c->body[2][1] = GREEN;
            c->rect = (SDL_Rect){.w = 3, .h = 2};

            c->memory = calloc(1, sizeof(SDL_bool));

            break;
    }
}

void insert_chip(SDL_Point pos, chip_types type) {
    // This whole function is an almost exact copy of insert_pixel, so I'm not going to bother commenting it since it will just be a copy most of the time
    chip *c = find_chip(pos, SDL_FALSE);

    if (c == NULL) {
        chip *c = calloc(1, sizeof(chip));

        c->pos = pos;
        c->type = type;
        
        init_chip(c);

        HASH_ADD(hh, chips, pos, sizeof(SDL_Point), c); 
    } else {
        c->type = type;
        init_chip(c);
    }
}

void delete_chip(SDL_Point pos) {
    // Same case as insert_chip, this is basically the same as delete_pixel, so not much reason to write comments for it
    chip *c = find_chip(pos, SDL_FALSE);

    if (c != NULL) {
        HASH_DEL(chips, c);
        free(c);
    }
}

void draw_chip(chip *c) {
    for (int ix = 0; ix < c->rect.w; ix++) { for (int iy = 0; iy < c->rect.h; iy++) {
        draw_point((SDL_Point){.x = c->pos.x + ix, .y = c->pos.y + iy}, c->body[ix][iy]);
    } }
}