#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "pixel.h"


#define WIDTH 128
#define HEIGHT 128
#define WIN_WIDTH WIDTH * 6
#define WIN_HEIGHT HEIGHT * 6


int main() {
    // We allocate enough memory for a decent amount of pixels, for now
    int pixelsSize = 32;
    int pixelsUsed = 0;
    pixel *pixels = malloc(pixelsSize * sizeof(pixel));
    // As we zoom out and draw more pixels, we can use realloc to allocate the actual needed memory for this array


    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // Not technically needed lol


    bool holding = false;

    while (1) {
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);


        // Events
        SDL_PumpEvents();

        // If there is an event, do things
        if (SDL_PollEvent(&event)) {
            int quit = 0;

            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN: holding = true; break;
                case SDL_MOUSEBUTTONUP: holding = false; break;

                case SDL_QUIT: quit = 1; break;
            }

            if (quit) { break; }
        }


        if (holding) {
            if (pixelsUsed == pixelsSize) {
                pixelsSize *= 2;
                pixels = realloc(pixels, pixelsSize * sizeof(pixel));
            }

            bool found = false;
            for (int i = 0; i < pixelsSize; i++) {
                pixel p = pixels[i];
                if (p.x == event.button.x && p.y == event.button.y) { found = true; break; }
            }
            
            if (!found) {
                pixels[pixelsUsed++] = (pixel){.x = event.button.x, .y = event.button.y, .r = 250, .g = 20, .b = 20, .a = 100};
            }
        }


        // Draw
        // Pixels
        for (int i = 0; i < pixelsSize; i++) {
            pixel p = pixels[i];

            SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, p.a);
            SDL_RenderDrawPoint(renderer, p.x, p.y);
        }

        // ACTUALLY SET THE RENDERER AND DRAW THE FORKING THING YOU STOOPID MORON
        SDL_RenderPresent(renderer);
    }


    // Free memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();
    return 0;
}