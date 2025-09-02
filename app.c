#include "include/app.h"
#include "config/config.h"
#include "include/global.h"
// #include <cstdint>

SDL_Window      *gWindow        = NULL;
SDL_Renderer    *gRenderer      = NULL;
SDL_Texture     *gTexture       = NULL;
TTF_Font        *font           = NULL;
Queue_t         *toMainThrQueue = NULL;
Queue_t         *toAppThrQueue  = NULL;
Queue_t         *keyboardEventQueue = NULL;
pthread_mutex_t SDLLock         = PTHREAD_MUTEX_INITIALIZER;
sim_status_t    simStatus       = STATUS_STOPPED;
sim_int_reg_t   negInterruptRegister   = 0;
sim_int_reg_t   posInterruptRegister   = 0;
uint32_t        simFlag                = 0;

int app_thread(void *data) {
    __sim_entry("app_thread(...)");

    main_app();
    
    __sim_exit("app_thread(...)");
    return 0;
}

int main() {
    __sim_entry("main()");
    
    atexit(screen_exit);
    atexit(font_exit);
    atexit(queue_exit);

    simStatus = STATUS_INIT;

    queue_init();
    if (screen_init() != STATUS_OKE) return ERROR_UNKNOWN;
    if (font_init()   != STATUS_OKE) return ERROR_UNKNOWN;
    
    __sim_log("[+] InputThread");
    SDL_Thread *thread1 = SDL_CreateThread(input_thread, "InputThread", NULL);
    if (!thread1) {
        printf("[main] Create InputThread failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }

    intro();

    simStatus = STATUS_RUNNING;
    
    __sim_log("[+] AppThread");
    SDL_Thread *thread0 = SDL_CreateThread(app_thread, "AppThread", NULL);
    if (!thread0) {
        printf("[main] AppThread failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }

    while (simStatus == STATUS_RUNNING) {
        if(simFlag & __mask32(FLAG_RENDER_REQ)){
            simFlag &= __inv_mask32(FLAG_RENDER_REQ);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(DELAY_BEFORE_FLUSH_MS); // ~60 
        }
        SDL_Delay(1);
    }

    __sim_exit("main()");
    return 0;
}

