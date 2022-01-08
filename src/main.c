#include "pixel/pixel.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>


#define WIDTH 16 // Game width and height, also known as how many pixels can be on the screen with a multiple of 1
#define HEIGHT 9
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720


int main() {
    // Initalize randomization
    srand(time(NULL));


    // We allocate enough memory for a decent amount of pixels, for now
    int pixelsSize = 100; // This number is not too important since it will automatically get expanded as more pixels are spawned in
    int pixelsUsed = 0;
    pixel **pixels = malloc(pixelsSize * sizeof(pixel*)); // An array of pointers to pixels
    // As we zoom out and draw more pixels, we can use realloc to allocate the actual needed memory for this array
    for (int i = 0; i < pixelsSize; i++) { pixels[i] = NULL; } // Initalize pixels to all nulls


    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0); // Removed 'set logical renderer' because we are doing this differently and better now
    int multiple = 1;


    int lastX;
    int lastY;

    while (1) {
        // Events
        SDL_PumpEvents();

        // If there is an event, do things
        if (SDL_PollEvent(&event)) {
            int quit = 0;

            int tMultiple = multiple; // According to the laws of C code, we cannot define variables in a switch case
            // I'm sure there is a good reason for it, but I have not looked it up
            // That's why this is declared here

            switch (event.type) {
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) { tMultiple++; } 
                    else if (event.wheel.y < 0) { tMultiple--; }

                    if (tMultiple > 0 && HEIGHT * tMultiple < WIN_HEIGHT && WIDTH * tMultiple < WIN_WIDTH) { 
                        multiple = tMultiple;
                    }

                    printf("Multiple: %d\n", multiple);

                    break;

                case SDL_QUIT: quit = 1; break;
            }

            if (quit == true) { break; }
        }


        // If we think we need it, allocate more memory for pixels
        if (pixelsUsed + multiple > pixelsSize) {
            pixelsSize += (multiple * multiple) * 2;

            pixel **tPixels = realloc(pixels, pixelsSize * sizeof(pixel*));
            if (tPixels != NULL) { pixels = tPixels; }
        } // Write code VERY soon that will go through the pixels, find null ones, shift the array so they go away, then reallocate the array so it only uses the memory needed


        int x;
        int y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);

        // Get the x and y position relative to the game world
        int relX = round(x / (WIN_WIDTH / (WIDTH * multiple)));
        int relY = round(y / (WIN_HEIGHT / (HEIGHT * multiple)));

        if ((buttons & SDL_BUTTON_LMASK) != 0 && x != lastX && y != lastY) { // Check that we hit the left click button and the mouse has moved since last frame
            // Place the pixels in
            int l = 0;
            //for (int ix = 0; ix < multiple; ix++) { for (int iy = 0; iy < multiple; iy++) {
            int ix = 0;
            int iy = 0;
            // Test that there isn't already a pixel placed where we are trying to place one
            bool found = false;
            for (int i = 0; i < pixelsSize; i++) {
                if (pixels[i] != NULL && cmp_xy(pixels[i]->pos, (xy){.x = relX + ix, .y = relY + iy}) == true) {
                    found = true; break;
                }
            }
            if (found == true) { continue; }

            // Shift all pixels back the needed amount
            for (int i = pixelsSize - 1; i >= 0; i--) {
                if (i > pixelsSize) { continue; }
                if (pixels[i] != NULL) { 
                    pixels[i + 1] = pixels[i];
                    pixels[i] = NULL;
                }
            }

            // Place the pixel
            pixels[l] = &((pixel){.pos = {.x = relX + ix, .y = relY + iy}, .r = rand() % 256, .g = rand() % 256, .b = rand() % 256, .a = 255});
            printf("%d:%d\n", pixels[l]->pos.x, pixels[l]->pos.y);
            if (pixels[l] == NULL) { printf("is null"); }
            // Using 256 on the rands because rand goes between 0 and the modulo value, so the maximum value this will get to is 255, which is what we want
            l++;

            pixelsUsed++;
            //} }

            printf("Bytes allocated for the pixels array: %d\nBytes used for pixels: %d\n", pixelsSize * (int)sizeof(pixel*), pixelsUsed * (int)sizeof(pixel*));

            lastX = x;
            lastY = y;
        }


        // Draw
        // Pixels
        for (int i = 0; i < pixelsSize; i++) {
            if (pixels[i] == NULL) { continue; }
            pixel *p = pixels[i];
            //printf("am drawing %d:%d:%d:%d:%d:%d\n", p->pos.x, p->pos.y, p->r, p->g, p->b, p->a);

            SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, p->a);

            int winWidthConversion = round(WIN_WIDTH / (WIDTH * multiple));
            int winHeightConversion = round(WIN_HEIGHT / (HEIGHT * multiple));

            SDL_RenderFillRect(renderer, &((SDL_Rect){.x = p->pos.x * winWidthConversion, .y = p->pos.y * winHeightConversion, 
                                            .w = winWidthConversion, .h = winHeightConversion}));
        }

        // Present the buffered rendering data to the screen, then clear the buffer
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }


    // Free thingies
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();
    return 0;
}