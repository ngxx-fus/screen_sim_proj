#include "include/app.h"
#include "config/config.h"
#include "include/global.h"

SDL_Window      *gWindow                = NULL;
SDL_Renderer    *gRenderer              = NULL;
SDL_Texture     *gTexture               = NULL;
TTF_Font        *font                   = NULL;
Queue_t         *toMainThrQueue         = NULL;
Queue_t         *toAppThrQueue          = NULL;
Queue_t         *keyboardEventQueue     = NULL;
pthread_mutex_t SDLLock                 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t simFlagLock             = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t simScreenBufferLock     = PTHREAD_MUTEX_INITIALIZER;
simStatus_t     simStatus               = STATUS_STOPPED;
uint32_t        simFlag                 = 0;
uint32_t        simRenderEventNumber    = 0;
uint32_t        simScreenBuffer[SCREEN_W * SCREEN_H];
SDL_Texture*    simScreenBufferTexture = NULL;

int app_thread(void *data) {
    __sim_entry("app_thread(...)");
    main_app();
    __sim_exit("app_thread(...)");
    return 0;
}

int main() {
    __sim_entry("main()");
    
    atexit(simScreenExit);
    atexit(simFontExit);
    atexit(simQueueExit);
    atexit(simInterruptExit);

    simStatus = STATUS_INIT;

    if (simQueueInit() != STATUS_OKE)   return ERROR_UNKNOWN;
    if (screen_init() != STATUS_OKE)    return ERROR_UNKNOWN;
    if (font_init()   != STATUS_OKE)    return ERROR_UNKNOWN;
    
    __sim_log("[+] InputThread");
    SDL_Thread *thread0 = SDL_CreateThread(input_thread, "InputThread", NULL);
    if (!thread0) {
        __sim_log("[main] Create InputThread failed: %s", SDL_GetError());
        return ERROR_UNKNOWN;
    }
    if (simInterruptInit() != STATUS_OKE)  return ERROR_UNKNOWN;
    intro();
    if(__simCheckFlag(FLAG_QUIT)) return 0;
    simEnableInterrupt(0, INT_PULLDOWN);
    simRenderEventNumber = SDL_RegisterEvents(1);
    if(simRenderEventNumber == (Uint32)-1){
        __sim_log("[main()] Failed to register an event number!");
        return -1;
    }
    __sim_log("[+] LoopInterruptService");
    SDL_Thread *thread1 = SDL_CreateThread(loopTrackInterruptService, "LoopInterruptService", NULL);
    if (!thread1) {
        printf("[main] AppThread failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }

    simStatus = STATUS_RUNNING;
    
    __sim_log("[+] AppThread");
    SDL_Thread *thread2 = SDL_CreateThread(app_thread, "AppThread", NULL);
    if (!thread2) {
        printf("[main] AppThread failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }
    __sim_entry("eventProcessLoop<...>");
    while (simStatus != STATUS_STOPPED) {
        if(__simCheckFlag(FLAG_RENDER_REQ)){
            // __sim_log("[eventProcessLoop] Has a render request!");
            __simClearFlag(FLAG_RENDER_REQ);
            __entry_SDL_critical_section();
            SDL_UpdateTexture(simScreenBufferTexture, NULL, simScreenBuffer, SCREEN_W * sizeof(simColor_t));
            SDL_RenderCopy(gRenderer, simScreenBufferTexture, NULL, NULL);
            SDL_RenderPresent(gRenderer);
            __exit_SDL_critical_section();
        }
        #if defined(RENDER_FPS)
            // __sim_log("Delay for %dms!", 1000/RENDER_FPS);
            SDL_Delay(1000/RENDER_FPS);
        #else 
            SDL_Delay(50);
        #endif
    }
    __sim_exit("eventProcessLoop<...>");

//LEGACY//      while (simStatus != STATUS_STOPPED) {
//LEGACY//          if(simFlag & __mask32(FLAG_RENDER_REQ)){
//LEGACY//              __entry_SDL_critical_section();
//LEGACY//              SDL_RenderPresent(gRenderer);
//LEGACY//              __exit_SDL_critical_section();
//LEGACY//              simFlag &= __inv_mask32(FLAG_RENDER_REQ);
//LEGACY//          }
//LEGACY//          #if defined(RENDER_FPS) && RENDER_FPS > 0 
//LEGACY//              __sim_sleep_ms(1000/RENDER_FPS);
//LEGACY//          #else
//LEGACY//              __sim_sleep_ns(__MSEC(50));
//LEGACY//          #endif
//LEGACY//      }
    __sim_exit("main()");
    return 0;
}

