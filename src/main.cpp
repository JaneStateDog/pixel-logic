#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>


#define GAME_WIDTH 128
#define GAME_HEIGHT 128

#define WINDOW_WIDTH GAME_WIDTH * 3
#define WINDOW_HEIGHT GAME_HEIGHT * 3


int main() {
      srand(time(NULL));


      SDL_Event event;

      SDL_Init(SDL_INIT_VIDEO);

      SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      
      SDL_Renderer *drawingRenderer = SDL_CreateRenderer(window, -1, 0);
      SDL_RenderSetLogicalSize(drawingRenderer, GAME_WIDTH, GAME_HEIGHT);


      bool holding = false;
      int size = 1;

      while (true) {
            // Clear screen
            // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            // SDL_RenderClear(renderer);
            // Since we only have a drawing renderer right now, which we do not want to clear
            // there is no need for this code at this moment


            // Events
            SDL_PumpEvents();


            if (SDL_PollEvent(&event)) {
                  bool quit = false;

                  switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN:
                              holding = true; break;
                        case SDL_MOUSEBUTTONUP:
                              holding = false; break;

                        case SDL_MOUSEWHEEL:
                              if (event.wheel.y > 0) { size++; }
                              else if (event.wheel.y < 0) { size--; }

                              break;


                        case SDL_QUIT: quit = true; break;
                  }

                  if (quit) { break; }

                  if (holding) {
                        SDL_SetRenderDrawColor(drawingRenderer, std::rand() % 255, std::rand() % 255, std::rand() % 255, 255);

                        SDL_Rect rect;
                        rect.x = event.button.x; rect.y = event.button.y;
                        rect.w = size; rect.h = size;
                        SDL_RenderFillRect(drawingRenderer, &rect);
                  }
            }


            // Draw
            SDL_RenderPresent(drawingRenderer);
      }

      SDL_DestroyRenderer(drawingRenderer);

      SDL_DestroyWindow(window);
      SDL_Quit();


      return false;
}