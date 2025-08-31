#include "app.h"

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

__attribute__((weak)) void 
main_app() {
    printf("No main_app() implemented!\n");
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Screen Sim",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    /// Start sim
    main_app();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return 0;
}
