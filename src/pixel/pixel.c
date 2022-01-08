#include "pixel.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../uthash/uthash.h"


bool cmp_xy(xy xy1, xy xy2) { return ((xy1.x == xy2.x) && (xy1.y == xy2.y)); }


// Initalize the pixels hashtable as null
pixel *pixels = NULL;

void insert_pixel(xy pos, rgba color) {
    // Look in the hashtable to see if the inputted pos/key already exists
    pixel *p;
    HASH_FIND(hh, pixels, &pos, sizeof(xy), p); // I don't think this is gonna work lmao

    // If the inputted pos/key does not exist, create a new pixel
    // Otherwise just readjust the data in the pixel
    if (p == NULL) {
        // Create this new pixel and store a pointer to it
        pixel *p = calloc(1, sizeof(pixel)); // We use calloc because uthash requires our structures to be zeored

        // Set the data of this new pixel
        p->pos = pos;
        p->color = color;

        // Add it to the hashtable
        HASH_ADD(hh, pixels, pos, sizeof(xy), p); // Bro macros are wack. Instead of putting in "p->pos" or "p->hh" or whatever, I just put the name of the variable in and it uses it with the structure?? That's so cool
    } else {
        p->color = color;
    }
}

void delete_pixel(xy pos) {
    // Find the pointer to the pixel first
    pixel *p;
    HASH_FIND(hh, pixels, &pos, sizeof(xy), p);

    // If we found a pixel, delete it
    if (p != NULL) {
        HASH_DEL(pixels, p);
        free(p);
    }
}