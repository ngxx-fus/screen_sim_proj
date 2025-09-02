#ifndef __APP_H__
#define __APP_H__
#include "global.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>

void     queue_init(){
    __sim_entry("queue_init()");
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

void     font_exit() {
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

int      input_thread(void *arg) {
    __sim_entry("input_thread()");
    SDL_Event e;
    while(simStatus != STATUS_STOPPED){
        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
                case SDL_QUIT:
                    __sim_log("Event <SDL_QUIT> occured!");
                    simStatus = STATUS_STOPPED;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_q) {
                        __sim_log("Event: <SDLK_q> is pressed!");
                        if(simStatus == STATUS_RUNNING) simStatus = STATUS_STOPPED;
                        else __sim_log("Skipped because of simStatus=%d", simStatus);
                    }else if(e.key.keysym.sym == SDLK_c){
                        __sim_log("Event <SDL_KEYDOWN | SDLK_c> occured!");
                        simFlag |= __mask8(FLAG_CONTINUE);
                    }else if(SDLK_0 <= e.key.keysym.sym && e.key.keysym.sym <= SDLK_9){
                        uint8_t i = e.key.keysym.sym - SDLK_0;
                        negInterruptRegister |= __mask32(i);
                        posInterruptRegister &= __inv_mask32(i);
                        __sim_log("Interrupt: (negEdge) 0x%02x", negInterruptRegister);
                    }
                    
                    break;
                case SDL_KEYUP:
                    if(SDLK_0 <= e.key.keysym.sym && e.key.keysym.sym <= SDLK_9){
                        uint8_t i = e.key.keysym.sym - SDLK_0;
                        negInterruptRegister &= __inv_mask32(i);
                        posInterruptRegister |= __mask32(i);
                        __sim_log("Interrupt: (posEdge) 0x%02x", posInterruptRegister);
                    }else{
                        if(e.key.keysym.sym == SDLK_c){
                            simFlag &= __inv_mask32(FLAG_CONTINUE);
                        }
                    }
                    break;
            }
        }
    }

    __sim_exit("input_thread()");
    return 0;
}

__attribute__((weak)) 
void     main_app(){
    const char* msg = "[main_app] No main_app() implemented!\n";
    __sim_log("%s", msg);

    if (!font) {
        __sim_log("[main_app] Font not loaded!\n");
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

void     intro(){
    __sim_entry("intro()");
    char buffer[265];
    snprintf(
        buffer, sizeof(buffer), 
        "\nHello from NgxxFus!\nWxH=%dx%d\nDelay before flush: %dms + %dns\nPress <c> to continue!", 
        SCREEN_W, SCREEN_H,
        DELAY_BEFORE_FLUSH_MS, 
        DELAY_BEFORE_FLUSH_NS
    );
    __sim_log("%s", buffer);
    if (!font) {
        __sim_log("[main_app] Font not loaded!");
        return;
    }
    SDL_Color textColor = {255, 255, 255, 255}; // WHITE
    SDL_SetRenderDrawColor(gRenderer, BLACK); // BLACK
    SDL_RenderClear(gRenderer);

    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, buffer, textColor, SCREEN_W - 10);
    if (!surface) {
        __sim_log("<surface> is NULL!");
        return;
    }
    if (gTexture) SDL_DestroyTexture(gTexture);  // tránh leak
    gTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {5, 5, 0, 0};
    SDL_QueryTexture(gTexture, NULL, NULL, &dstRect.w, &dstRect.h);

    SDL_RenderCopy(gRenderer, gTexture, NULL, &dstRect);
    SDL_RenderPresent(gRenderer);
    
    while(1){
        if(simFlag & __mask32(FLAG_CONTINUE)) {
            simFlag &= __inv_mask32(FLAG_CONTINUE);
            __sim_log("Continue!");
            break;
        }
        SDL_Delay(1);
    }
    SDL_SetRenderDrawColor(gRenderer, BLACK);
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
    __sim_exit("intro()");
}

#endif
