#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
// #include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include "../lib/queue/queue.h"

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////

#include "../config/color.h"
#include "../config/config.h"

/// GLOBL VARS ///////////////////////////////////////////////////////////////////////////////////

extern SDL_Window       *gWindow;               /// Main application window
extern SDL_Renderer     *gRenderer;             /// Main SDL renderer used for drawing
extern SDL_Texture      *gTexture;              /// General-purpose texture (frame buffer or image)
extern TTF_Font         *font;                  /// Global font resource
extern Queue_t          *toMainThrQueue;        /// Queue for sending messages/events to main thread
extern Queue_t          *toAppThrQueue;         /// Queue for sending messages/events to app thread
extern Queue_t          *keyboardEventQueue;    /// Queue for keyboard input events
extern pthread_mutex_t  SDLLock;                /// Mutex lock for SDL operations (thread-safety)
extern pthread_mutex_t  simFlagLock;            /// Mutex lock for simulation flags lock
extern simStatus_t      simStatus;              /// Current simulation status
extern simIntReg_t      negInterruptRegister;   /// Interrupt resgister
extern simIntReg_t      posInterruptRegister;   /// Interrupt resgister
extern simFlag_t        simFlag;                /// Software (System) flag

#ifndef __entry_critical_section
    /// @brief Enter a critical section (blocking until lock acquired)
    #define __entry_critical_section(mutex)         pthread_mutex_lock(mutex)
#endif
#ifndef __try_entry_critical_section
    /// @brief Try to enter a critical section (non-blocking, returns 0 if success)
    #define __try_entry_critical_section(mutex)      pthread_mutex_trylock(mutex)
#endif
#ifndef __exit_critical_section
    /// @brief Exit a critical section (release lock)
    #define __exit_critical_section(mutex)           pthread_mutex_unlock(mutex)
#endif

/// @brief Enter SDL critical section (blocking until lock acquired)
#define __entry_SDL_critical_section()          pthread_mutex_lock(&SDLLock)
/// @brief Try to enter SDL critical section (non-blocking, returns 0 if success)
#define __try_entry_SDL_critical_section()      pthread_mutex_trylock(&SDLLock)
/// @brief Exit SDL critical section (release lock)
#define __exit_SDL_critical_section()           pthread_mutex_unlock(&SDLLock)

/// DEBUG SETUP ///////////////////////////////////////////////////////////////////////////////////

#include "../lib/log/log.h"

/// @brief Sleep for a given number of milliseconds (please give input as integer)
#define __sim_sleep_ms(ms) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (ms) / 1000;                     \
    ts.tv_nsec = ((ms) % 1000) * 1000000L;        \
    nanosleep(&ts, NULL);                         \
} while(FALSE)

/// @brief Sleep for a given number of microseconds (please give input as integer)
#define __sim_sleep_us(us) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (us) / 1000000;                  \
    ts.tv_nsec = ((us) % 1000000) * 1000L;        \
    nanosleep(&ts, NULL);                         \
} while(FALSE)

/// @brief Sleep for a given number of seconds (please give input as integer)
#define __sim_sleep(sec) do {                     \
    struct timespec ts;                           \
    ts.tv_sec  = sec;                             \
    ts.tv_nsec = 0;                               \
    nanosleep(&ts, NULL);                         \
} while(FALSE)

/// @brief Set a flag, then another thread/itself will proceed
#define __set_flag(flag) do{    \
    if((flag) >= sizeof(simFlag)) { \
        __sim_log("Error: __set_flag(%d)", flag); \
        break; \
    } \
    __entry_critical_section(&simFlagLock); \
    simFlag |= __mask32(flag); \
    __exit_critical_section(&simFlagLock);  \
}while(FALSE);

/// @brief Set render flag, then main thread will render the screen
#define __set_render_req() do{  \
    __entry_critical_section(&simFlagLock); \
    simFlag |= __mask32(FLAG_RENDER_REQ); \
    __exit_critical_section(&simFlagLock);  \
}while(FALSE);

/// @brief Clear flag and render. Noted that ONLY CALL it from main thread
#define __clr_req_and_render() do{  \
    __entry_critical_section(&simFlagLock); \
    simFlag &= __inv_mask32(FLAG_RENDER_REQ); \
    SDL_RenderPresent(gRenderer); \
    __exit_critical_section(&simFlagLock);  \
}while(FALSE);

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
