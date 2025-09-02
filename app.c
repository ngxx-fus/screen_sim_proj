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
simStatus_t     simStatus               = STATUS_STOPPED;
simIntReg_t     negInterruptRegister    = 0;
simIntReg_t     posInterruptRegister    = 0;
uint32_t        simFlag                 = 0;

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
    SDL_Thread *thread1 = SDL_CreateThread(input_thread, "InputThread", NULL);
    if (!thread1) {
        __sim_log("[main] Create InputThread failed: %s", SDL_GetError());
        return ERROR_UNKNOWN;
    }

    if (interruptInit() != STATUS_OKE)  return ERROR_UNKNOWN;

    intro();

    simStatus = STATUS_RUNNING;
    
    __sim_log("[+] AppThread");
    SDL_Thread *thread0 = SDL_CreateThread(app_thread, "AppThread", NULL);
    if (!thread0) {
        printf("[main] AppThread failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }

    while (simStatus == STATUS_RUNNING) {
        __entry_critical_section(&simFlagLock);
        if(simFlag & __mask32(FLAG_RENDER_REQ)){
            __entry_SDL_critical_section();
            SDL_RenderPresent(gRenderer);
            __exit_SDL_critical_section();
            simFlag &= __inv_mask32(FLAG_RENDER_REQ);
        }
        __exit_critical_section(&simFlagLock);
    }

    __sim_exit("main()");
    return 0;
}

