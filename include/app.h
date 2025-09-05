#ifndef __APP_H__
#define __APP_H__

#ifdef LOG_HEADER_INCLUDE
#pragma message("INCLUDE: app.h")
#endif 

#include "global.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include "../lib/interrupt/sim_interrupt.h"

static simStatus_t simQueueInit(){
    __sim_entry("simQueueInit()");
    qInit(&toMainThrQueue);
    qInit(&toAppThrQueue);
    qInit(&keyboardEventQueue);
    __sim_entry("queue_exit()");
    return STATUS_OKE;
}

static void     simQueueExit(){
    __sim_entry("queue_exit()");
    qFree(toMainThrQueue);
    qFree(toAppThrQueue);
    qFree(keyboardEventQueue);
    __sim_exit("queue_exit()");
}

static simStatus_t font_init(){
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

static void     simFontExit() {
    __sim_entry("simFontExit()");
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
    __sim_exit("simFontExit()");
}

static simStatus_t screen_init(){
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
    simScreenBufferTexture = SDL_CreateTexture(
        gRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        SCREEN_W,
        SCREEN_H
    );
    if (!simScreenBufferTexture) {
        __sim_log("Create simScreenBufferTexture failed: %s", SDL_GetError());
        exit(1);
    }
    __sim_exit("screen_init()");
    return STATUS_OKE;
}

static void     simScreenExit(){
    __sim_entry("simScreenExit()");
    SDL_DestroyTexture(simScreenBufferTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    __sim_exit("simScreenExit()");
}

__attribute__((weak)) 
void     main_app(){
    char buffer[265];
    snprintf(
        buffer, sizeof(buffer), 
        "NO main_app() IMPLEMENT!" 
    );
    __sim_log("%s", buffer);
    if (!font) {
        __sim_log("[main_app] Font not loaded!");
        return;
    }
    SDL_Color textColor = {255, 255, 255, 255}; // WHITE
    SDL_SetRenderDrawColor(gRenderer, RGBA_BLACK); // BLACK
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

}

static int      input_thread(void *arg) {
    __sim_entry("input_thread()");
    SDL_Event e;
    while(simStatus != STATUS_STOPPED){
        while (SDL_PollEvent(&e)) {
            // __sim_log("Event <%d> occured!", e.type);
            switch (e.type)
            {
                case SDL_QUIT:
                    __sim_log("Event <SDL_QUIT> occured!");
                    simStatus = STATUS_STOPPED;
                    __simSetFlag(FLAG_QUIT);
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_q) {
                        __sim_log("Event: <SDLK_q> is pressed!");
                        simStatus = STATUS_STOPPED;
                        __simSetFlag(FLAG_QUIT);
                    }else 
                    if(e.key.keysym.sym == SDLK_c){
                        __sim_log("Event <SDL_KEYDOWN | SDLK_c> occured!");
                        __simSetFlag(FLAG_CONTINUE);
                    }else 
                    if(SDLK_0 <= e.key.keysym.sym && e.key.keysym.sym <= SDLK_9){
                        uint8_t i = e.key.keysym.sym - SDLK_0;
                        simPushInterruptEvent(i, INT_PULLDOWN);
                    }
                    break;
                case SDL_KEYUP:
                    if(SDLK_0 <= e.key.keysym.sym && e.key.keysym.sym <= SDLK_9){
                        uint8_t i = e.key.keysym.sym - SDLK_0;
                        simPushInterruptEvent(i, INT_PULLUP);
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

static void     intro(){
    __sim_entry("intro()");
    char buffer[265];
    snprintf(
        buffer, sizeof(buffer), 
        "\nHello from NgxxFus!\nWxH=%dx%d\nFPS=%d\nPress <c> to continue!\nPress <q> to quit!", 
        SCREEN_W, SCREEN_H,
        RENDER_FPS 
    );
    __sim_log("%s", buffer);
    if (!font) {
        __sim_log("[main_app] Font not loaded!");
        return;
    }
    SDL_Color textColor = {255, 255, 255, 255}; // WHITE
    SDL_SetRenderDrawColor(gRenderer, RGBA_BLACK); // BLACK
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
    
    while(simStatus != STATUS_STOPPED){
        if(__simCheckFlag(FLAG_CONTINUE)) {
            __simClearFlag(FLAG_CONTINUE);
            __sim_log("Continue!");
            break;
        }
        if(__simCheckFlag(FLAG_QUIT)){
            __sim_log("Quit!");
            return;
        }
        SDL_Delay(1);
    }
    SDL_SetRenderDrawColor(gRenderer, RGBA_BLACK);
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
    __sim_exit("intro()");
}

#endif
