#include "app.h"

SDL_Window      *gWindow        = NULL;
SDL_Renderer    *gRenderer      = NULL;
SDL_Texture     *gTexture       = NULL;
TTF_Font        *font           = NULL;
Queue_t         *toMainThrQueue = NULL;
Queue_t         *toAppThrQueue  = NULL;
pthread_mutex_t SDLLock         = PTHREAD_MUTEX_INITIALIZER;
sim_status_t    simStatus      = STATUS_STOPPED;

int app_thread(void *data) {
    main_app();
    return 0;
}

int main() {
    __sim_entry("main()");
    simStatus = STATUS_RUNNING;
    qInit(&toMainThrQueue);
    qInit(&toAppThrQueue);
    if (screen_init() != STATUS_OKE) return ERROR_UNKNOWN;
    if (font_init()   != STATUS_OKE) return ERROR_UNKNOWN;

    SDL_Thread *thread = SDL_CreateThread(app_thread, "AppThread", NULL);
    if (!thread) {
        printf("[main] SDL_CreateThread failed: %s\n", SDL_GetError());
        return ERROR_UNKNOWN;
    }

    SDL_Event e;
    while (simStatus == STATUS_RUNNING) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                simStatus = STATUS_STOPPED;
            }
        }
        SDL_RenderPresent(gRenderer);
        SDL_Delay(16); // ~60 FPS tick
    }

    screen_exit();
    qFree(toMainThrQueue);
    qFree(toAppThrQueue);
    __sim_exit("main()");
    return 0;
}

