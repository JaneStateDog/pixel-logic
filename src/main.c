#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "pixel.h"


#define WIDTH 128
#define HEIGHT 128
#define WIN_WIDTH WIDTH * 6
#define WIN_HEIGHT HEIGHT * 6


int main() {
    int multiple = 1;


    // We allocate enough memory for a decent amount of pixels, for now
    int pixelsSize = (WIDTH * HEIGHT) * multiple;
    pixel *pixels = malloc(pixelsSize * sizeof(pixel));
    // As we zoom out and draw more pixels, we can use realloc to allocate the actual needed memory for this array


    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0); // Removed 'set logical renderer' because we are doing this differently and better now


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
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) { multiple++; } 
                    else if (event.wheel.y < 0) { multiple--; }
                    if (multiple <= 0) { multiple = 1; }

                    break;

                case SDL_QUIT: quit = 1; break;
            }

            if (quit) { break; }
        }


        int x;
        int y;
        Uint32 buttons = SDL_GetMouseState(&x, &y);
        x /= (WIN_WIDTH / (WIDTH * multiple));
        y /= (WIN_HEIGHT / (HEIGHT * multiple));

        if ((buttons & SDL_BUTTON_LMASK) != 0) {
            if ((WIDTH * multiple) * (HEIGHT * multiple) > pixelsSize) {
                pixelsSize = (WIDTH * multiple) * (HEIGHT * multiple);
                pixels = realloc(pixels, pixelsSize * sizeof(pixel));
            }

            // pixels[(x * (WIDTH * multiple)) + y] = (pixel){.x = x, .y = y, .r = 255, .g = 255, .b = 255, .a = 255};

            for (int ix = 0; ix < multiple; ix++) { for (int iy = 0; iy < multiple; iy++) {
                int i = ((x + ix) * (WIDTH * multiple)) + (y + iy);
                if (i < pixelsSize) {
                    pixels[i] = (pixel){.x = (x + ix), .y = (y + iy), .r = 255, .g = 255, .b = 255, .a = 255};
                }
            } }
        }


        // Draw
        // Pixels
        for (int i = 0; i < pixelsSize; i++) {
            pixel p = pixels[i];

            SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, p.a);

            SDL_RenderFillRect(renderer, &((SDL_Rect){.x = p.x * (WIN_WIDTH / (WIDTH * multiple)), .y = p.y * (WIN_HEIGHT / (HEIGHT * multiple)), .w = (WIN_WIDTH / (WIDTH * multiple)), .h = (WIN_HEIGHT / (HEIGHT * multiple))}));
        }

        SDL_RenderPresent(renderer);
    }


    // Free thingies
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();
    return 0;
}