#ifndef __APP_H__
#define __APP_H__
#include "global.h"
#include <SDL2/SDL_scancode.h>

void     queue_init(){
    __sim_entry("queue_init()");
    simStatus = STATUS_RUNNING;
    qInit(&toMainThrQueue);
    qInit(&toAppThrQueue);
    qInit(&keyboardEventQueue);
    __sim_entry("queue_exit()");
}

void     queue_exit(){
    __sim_entry("queue_exit()");
    qFree(toMainThrQueue);
    qFree(toAppThrQueue);
    qFree(keyboardEventQueue);
    __sim_exit("queue_exit()");
}

return_t font_init(){
    __sim_entry("font_init()");
    if (TTF_Init() < 0) {
        printf("[font_init] TTF_Init failed: %s\n", TTF_GetError());
        return ERROR_UNKNOWN;
    }
    font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        printf("[font_init] Error loading font: %s\n", TTF_GetError());
        return ERROR_NULL;
    }
    __sim_exit("font_init()");
    return STATUS_OKE;
}

void font_exit() {
    __sim_entry("font_exit()");
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
    __sim_exit("font_exit()");
}

return_t screen_init(){
    __sim_entry("screen_init()");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[screen_init] SDL_Init failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }
    gWindow = SDL_CreateWindow(
    "Screen Sim",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 
    SDL_WINDOW_SHOWN
    );
    gRenderer = SDL_CreateRenderer(
        gWindow, 
        -1, 
        SDL_RENDERER_ACCELERATED
    );
    __sim_exit("screen_init()");
    return STATUS_OKE;
}

void     screen_exit(){
    __sim_entry("screen_exit()");
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    __sim_exit("screen_exit()");
}

__attribute__((weak)) 
void     main_app(){
    const char* msg = "[main_app] No main_app() implemented!\n";
    printf("%s", msg);

    if (!font) {
        printf("[main_app] Font not loaded!\n");
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255}; // WHITE
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // BLACK
    SDL_RenderClear(gRenderer);

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, msg, textColor, SCREEN_W - 10);
    if (!surface) {
        printf("TTF_RenderText_Blended_Wrapped error: %s\n", TTF_GetError());
        return;
    }
    if (gTexture) SDL_DestroyTexture(gTexture);  // tránh leak
    gTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {5, 5, 0, 0};
    SDL_QueryTexture(gTexture, NULL, NULL, &dstRect.w, &dstRect.h);

    SDL_RenderCopy(gRenderer, gTexture, NULL, &dstRect);
    SDL_RenderPresent(gRenderer);
}

int      input_thread(void *arg) {
    __sim_entry("input_loop()");
    void* __keycode_ptr = NULL;
    SDL_Event e;
    while (simStatus == STATUS_RUNNING) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    __sim_log("SDL_QUIT received");
                    simStatus = STATUS_STOPPED;
                    break;

                case SDL_KEYDOWN:
                    __keycode_ptr = malloc(sizeof(e.key.keysym.sym));
                    memcpy(__keycode_ptr, &e.key.keysym.sym, sizeof(e.key.keysym.sym));
                    __entry_critial_section();
                    qPush(keyboardEventQueue, __keycode_ptr, sizeof(e.key.keysym.sym));
                    __exit_critial_section();
                    __sim_log("KeyDown: %s (code=%d)", SDL_GetKeyName(e.key.keysym.sym), e.key.keysym.sym);
                    free(__keycode_ptr);
                    break;

                case SDL_KEYUP:
                    // __entry_critial_section();
                    // __sim_log("KeyUp: %s (code=%d)", SDL_GetKeyName(e.key.keysym.sym), e.key.keysym.sym);
                    // __exit_critial_section();
                    break;

                default:
                    break;
            }
        }

        __sim_sleep(0.01);
    }

    __sim_exit("input_loop()");
    return 0;
}

#endif
