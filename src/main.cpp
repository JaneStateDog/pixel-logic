#include <SDL2/SDL.h>

#include <cmath>
#include <iostream>
#include <vector>


#define BASE_WIDTH 128
#define BASE_HEIGHT 128

#define WINDOW_WIDTH BASE_WIDTH * 3
#define WINDOW_HEIGHT BASE_HEIGHT * 3


typedef struct {
      Uint8 R;
      Uint8 G;
      Uint8 B;
      Uint8 A;

      int x;
      int y;
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

      std::vector<Pixel> pixels;

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
                                    Uint8 R = (Uint8)(std::rand() % 255);
                                    Uint8 G = (Uint8)(std::rand() % 255);
                                    Uint8 B = (Uint8)(std::rand() % 255);
                                    Uint8 A = (Uint8)(std::rand() % 255);
                                    for (int x = 0; x < zoom; x++) { for (int y = 0; y < zoom; y++) {
                                          pixels.push_back({R, B, G, A, mouseX + x, mouseY + y});
                                    } }
                                    
                                    break;
                              }

                              case SDL_BUTTON_RIGHT:
                                    for (int x = 0; x < zoom; x++) { for (int y = 0; y < zoom; y++) {
                                          std::remove_if(pixels.begin(), pixels.end(), [mouseX, mouseY, x, y](Pixel pixel) {
                                                return pixel.x == mouseX + x && pixel.y == mouseY + y;
                                          });
                                    } }
                                    
                                    break;
                        }
                  }
            }


            // Draw
            for (auto &pixel : pixels) {
                  if (pixel.x < gameWidth && pixel.x >= 0 && pixel.y < gameHeight && pixel.y >= 0) {
                        SDL_SetRenderDrawColor(drawingRenderer, pixel.R, pixel.G, pixel.B, pixel.A);
                        SDL_RenderDrawPoint(drawingRenderer, pixel.x, pixel.y);
                  }
            }

            SDL_RenderPresent(drawingRenderer);
      }


      SDL_DestroyRenderer(drawingRenderer);

      SDL_DestroyWindow(window);
      SDL_Quit();


      return false;
}