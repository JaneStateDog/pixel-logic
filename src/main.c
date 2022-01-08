#include "globals/globals.h"
#include "pixel/pixel.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "uthash.h"


int main() { 
    // Initalize randomization
    srand(time(NULL));


    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);    
    SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);


    insert_chip((SDL_Point){.x = 3, .y = 3}, AND);
    insert_chip((SDL_Point){.x = 7, .y = 3}, OR);
    insert_chip((SDL_Point){.x = 3, .y = 6}, NOT);
    insert_chip((SDL_Point){.x = 7, .y = 6}, BUTTON);


    SDL_Point lastPos;

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

            if (quit == SDL_TRUE) { break; }
        }


        SDL_Point pos;
        Uint32 buttons = SDL_GetMouseState(&pos.x, &pos.y);

        // Get the pos relative to the game world
        SDL_Point relPos = {.x = round(pos.x / (WIN_WIDTH / (WIDTH * multiple))), .y = round(pos.y / (WIN_HEIGHT / (HEIGHT * multiple)))};

        // Get the pos that is relative to the game world and snapped to the grid according to multiple
        SDL_Point snappedPos = {.x = round(relPos.x / multiple) * multiple, .y = round(relPos.y / multiple) * multiple};

        if (pos.x != lastPos.x && pos.y != lastPos.y) { // Check that the mouse has moved since the last frame
            if ((buttons & SDL_BUTTON_LMASK) != 0) {
                // Place pixels
                for (int ix = 0; ix < multiple; ix++) { for (int iy = 0; iy < multiple; iy++) {
                    insert_pixel((SDL_Point){.x = snappedPos.x + ix, .y = snappedPos.y + iy}, DEFAULT_PIXEL_COLOR);
                } }
            } else if ((buttons & SDL_BUTTON_RMASK) != 0) {
                // Delete pixels
                for (int ix = 0; ix < multiple; ix++) { for (int iy = 0; iy < multiple; iy++) {
                    delete_pixel((SDL_Point){.x = snappedPos.x + ix, .y = snappedPos.y + iy});
                } }
            }

            lastPos = pos;
        }


        // Draw
        // Chips
        chip *c;
        for (c = chips; c != NULL; c = c->hh.next) { // This is how uthash iterates through hashetables, it's wack but really cool at the same time
            draw_chip(c);
        }

        // Pixels
        pixel *p;
        for (p = pixels; p != NULL; p = p->hh.next) { // It's so wackyyy
            draw_pixel(p);
        }

        // Present the buffered rendering data to the screen
        SDL_RenderPresent(renderer);

        // Clear the rendering buffer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }


    // Free thingies
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();
    return 0;
}