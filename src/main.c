#include "pixel/pixel.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "uthash.h"


#define WIDTH 16 // Game width and height, also known as how many pixels can be on the screen with a multiple of 1
#define HEIGHT 9
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720


int main() {
    // Initalize randomization
    srand(time(NULL));


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


        int x;
        int y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);

        // Get the x and y position relative to the game world
        int relX = round(x / (WIN_WIDTH / (WIDTH * multiple)));
        int relY = round(y / (WIN_HEIGHT / (HEIGHT * multiple)));

        // Get the x and y position that is relative to the game world and snapped to the grid according to multiple
        uint16_t snappedX = round(relX / multiple) * multiple;
        uint16_t snappedY = round(relY / multiple) * multiple;

        if (x != lastX && y != lastY) { // Check that the mouse has moved since the last frame
            if ((buttons & SDL_BUTTON_LMASK) != 0) {
                // Place pixels
                rgba color = {.r = rand() % 256, .g = rand() % 256, .b = rand() % 256, .a = rand() % 256};
                for (int ix = 0; ix < multiple; ix++) { for (int iy = 0; iy < multiple; iy++) {
                    insert_pixel((xy){.x = snappedX + ix, .y = snappedY + iy}, color);
                } }
            } else if ((buttons & SDL_BUTTON_RMASK) != 0) {
                // Delete pixels
                for (int ix = 0; ix < multiple; ix++) { for (int iy = 0; iy < multiple; iy++) {
                    delete_pixel((xy){.x = snappedX + ix, .y = snappedY + iy});
                } }
            }

            lastX = x;
            lastY = y;
        }


        // Draw
        // Pixels
        pixel *p;
        for (p = pixels; p != NULL; p = p->hh.next) { // This is how uthash iterates through hashetables
            SDL_SetRenderDrawColor(renderer, p->color.r, p->color.g, p->color.b, p->color.a);

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