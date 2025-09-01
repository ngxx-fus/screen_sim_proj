#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <queue/queue.h>
#include <stdarg.h>
#include <pthread.h>

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include "../config/color.h"
#include "../config/config.h"

/// DEF ///////////////////////////////////////////////////////////////////////////////////////////

/// Default return type  
typedef int8_t return_t;
typedef int8_t sim_status_t;
/// Status code
enum RETURN_CODE{
    ERROR_NULL      = -2,
    ERROR_UNKNOWN   = -1,
    STATUS_OKE      = 0,
};
enum SIM_STATE_CODE{
    STATUS_STOPPED = 0,
    STATUS_RUNNING = 1,
};

/// GLOBL VARS ///////////////////////////////////////////////////////////////////////////////////
extern SDL_Window       *gWindow;
extern SDL_Renderer     *gRenderer;
extern SDL_Texture      *gTexture;
extern TTF_Font         *font;
extern Queue_t          *toMainThrQueue;
extern Queue_t          *toAppThrQueue;
extern Queue_t          *keyboardEventQueue;
extern pthread_mutex_t  SDLLock;
extern sim_status_t     simStatus;
/// DEBUG SETUP ///////////////////////////////////////////////////////////////////////////////////

static void __sim_vlog(const char* tag, const char* fmt, va_list args) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char buf[32];
    strftime(buf, sizeof(buf), "%H:%M:%S", t);

    fprintf(stderr, "[%s] [%s] ", buf, tag);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
}

static void __sim_log_impl(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __sim_vlog(tag, fmt, args);
    va_end(args);
}

#define __sim_log(...)   __sim_log_impl("LOG",  __VA_ARGS__)
#define __sim_err(...)   __sim_log_impl("ERR",  __VA_ARGS__)
#define __sim_entry(...) __sim_log_impl(">>>",  __VA_ARGS__)
#define __sim_exit(...)  __sim_log_impl("<<<",  __VA_ARGS__)
#define __sim_sleep_ms(ms) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (ms) / 1000;                     \
    ts.tv_nsec = ((ms) % 1000) * 1000000L;        \
    nanosleep(&ts, NULL);                         \
} while(0)
#define __sim_sleep_us(us) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (us) / 1000000;                  \
    ts.tv_nsec = ((us) % 1000000) * 1000L;        \
    nanosleep(&ts, NULL);                         \
} while(0)
#define __sim_sleep(sec) do {                     \
    struct timespec ts;                           \
    ts.tv_sec  = sec;                             \
    ts.tv_nsec = 0;                               \
    nanosleep(&ts, NULL);                         \
} while(0)

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
