#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>
#include <map>


#define GAME_WIDTH 128
#define GAME_HEIGHT 128

#define WINDOW_WIDTH GAME_WIDTH * 3
#define WINDOW_HEIGHT GAME_HEIGHT * 3


typedef struct {
      Uint8 R;
      Uint8 G;
      Uint8 B;
      Uint8 A;
} Pixel;


int main() {
      srand(time(NULL));


      SDL_Event event;

      SDL_Init(SDL_INIT_VIDEO);

      SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      
      SDL_Renderer *drawingRenderer = SDL_CreateRenderer(window, -1, 0);
      SDL_RenderSetLogicalSize(drawingRenderer, GAME_WIDTH, GAME_HEIGHT);


      int holding = 0;
      int size = 1;

      std::map<std::pair<int, int>, Pixel> pixels;

      while (true) {
            // Clear screen
            SDL_SetRenderDrawColor(drawingRenderer, 0, 0, 0, 0);
            SDL_RenderClear(drawingRenderer);


            // Events
            SDL_PumpEvents();


            if (SDL_PollEvent(&event)) {
                  bool quit = false;

                  switch (event.type) {
                        case SDL_MOUSEBUTTONDOWN: holding = event.button.button; break;
                        case SDL_MOUSEBUTTONUP: holding = 0; break;

                        case SDL_MOUSEWHEEL:
                              if (event.wheel.y > 0) { size++; }
                              else if (event.wheel.y < 0) { size--; }

                              break;


                        case SDL_QUIT: quit = true; break;
                  }

                  if (quit) { break; }

                  if (holding != 0) {
                        switch (holding) {
                              case SDL_BUTTON_LEFT:  {
                                    Pixel pixel = {(Uint8)(std::rand() % 255), (Uint8)(std::rand() % 255), (Uint8)(std::rand() % 255), 255};
                                    for (int x = 0; x < size; x++) for (int y = 0; y < size; y++) {
                                          pixels[std::make_pair(event.button.x + x, event.button.y + y)] = pixel;
                                    }
                                    
                                    break;
                              }

                              case SDL_BUTTON_RIGHT:
                                    pixels.erase(std::make_pair(event.button.x, event.button.y));
                                    break;
                        }
                  }
            }


            // Draw
            for (auto &pair : pixels) {
                  std::pair<int, int> pos = pair.first;
                  Pixel RGBA = pair.second;

                  SDL_SetRenderDrawColor(drawingRenderer, RGBA.R, RGBA.G, RGBA.B, RGBA.A);
                  SDL_RenderDrawPoint(drawingRenderer, pos.first, pos.second);
            }

            SDL_RenderPresent(drawingRenderer);
      }


      SDL_DestroyRenderer(drawingRenderer);

      SDL_DestroyWindow(window);
      SDL_Quit();


      return false;
}