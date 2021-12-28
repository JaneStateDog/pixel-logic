#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>
#include <map>


#define BASE_WIDTH 128
#define BASE_HEIGHT 128

#define WINDOW_WIDTH BASE_WIDTH * 3
#define WINDOW_HEIGHT BASE_HEIGHT * 3


typedef struct {
      Uint8 R;
      Uint8 G;
      Uint8 B;
      Uint8 A;

      int size;
} Pixel;


int main() {
      srand(time(NULL));


      int gameWidth = BASE_WIDTH;
      int gameHeight = BASE_HEIGHT;
      int zoom = 1;


      SDL_Event event;

      SDL_Init(SDL_INIT_VIDEO);

      SDL_Window *window = SDL_CreateWindow("pixel-logic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
      
      SDL_Renderer *drawingRenderer = SDL_CreateRenderer(window, -1, 0);
      SDL_RenderSetLogicalSize(drawingRenderer, gameWidth, gameHeight);


      int holding = 0;

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
                              if (event.wheel.y < 0 && zoom < 14) { zoom++; }
                              if (event.wheel.y > 0 && zoom > 1) { zoom--; }

                              gameWidth = BASE_WIDTH * zoom;
                              gameHeight = BASE_HEIGHT * zoom;

                              std::cout << zoom << std::endl;

                              SDL_RenderSetLogicalSize(drawingRenderer, gameWidth, gameHeight);

                              break;


                        case SDL_QUIT: quit = true; break;
                  }

                  if (quit) { break; }

                  if (holding != 0) {
                        int mouseX = round(event.button.x / zoom) * zoom;
                        int mouseY = round(event.button.y / zoom) * zoom;

                        switch (holding) {
                              case SDL_BUTTON_LEFT:  {
                                    Pixel pixel = {(Uint8)(std::rand() % 255), (Uint8)(std::rand() % 255), (Uint8)(std::rand() % 255), 255, zoom};
                                    pixels[std::make_pair(mouseX, mouseY)] = pixel;
                                    
                                    break;
                              }

                              case SDL_BUTTON_RIGHT:
                                    pixels.erase(std::make_pair(mouseX, mouseY));
                                    break;
                        }
                  }
            }


            // Draw
            for (auto &pair : pixels) {
                  std::pair<int, int> pos = pair.first;
                  Pixel pixel = pair.second;

                  SDL_SetRenderDrawColor(drawingRenderer, pixel.R, pixel.G, pixel.B, pixel.A);

                  SDL_Rect rect = {pos.first, pos.second, pixel.size, pixel.size};
                  SDL_RenderFillRect(drawingRenderer, &rect);
            }

            SDL_RenderPresent(drawingRenderer);
      }


      SDL_DestroyRenderer(drawingRenderer);

      SDL_DestroyWindow(window);
      SDL_Quit();


      return false;
}