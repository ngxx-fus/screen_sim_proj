#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// #define LOG_HEADER_INCLUDE
#ifdef LOG_HEADER_INCLUDE
#pragma message("INCLUDE: global.h")
#endif

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

#include "../config/config.h"
#include "../config/color.h"

/// GLOBL VARS ///////////////////////////////////////////////////////////////////////////////////

extern SDL_Window       *gWindow;               /// Main application window
extern SDL_Renderer     *gRenderer;             /// Main SDL renderer used for drawing
extern SDL_Texture      *gTexture;              /// General-purpose texture (frame buffer or image)
extern TTF_Font         *font;                  /// Global font resource
extern Queue_t          *toMainThrQueue;        /// Queue for sending messages/events to main thread
extern Queue_t          *toAppThrQueue;         /// Queue for sending messages/events to app thread
extern Queue_t          *keyboardEventQueue;    /// Queue for keyboard input events
extern pthread_mutex_t  SDLLock;                /// Mutex lock for SDL operations (thread-safety)
extern pthread_mutex_t  simFlagLock;            /// Mutex lock for simulation flags 
extern pthread_mutex_t  simScreenBufferLock;    /// Mutex lock for screen buffer
extern simStatus_t      simStatus;              /// Current simulation status
extern simFlag_t        simFlag;                /// Software (System) flag
extern SDL_Event        eventSendRenderRequest; /// Event that uses to send the render request; Only call from outside main thread;
extern SDL_Event        eventReceiveRenderRequest;  /// Event uses to stored the send the render request event; Only call from inside main thread;
extern simColor_t       simScreenBuffer[SCREEN_W * SCREEN_H];
extern SDL_Texture*     simScreenBufferTexture;

#ifndef rep
#define rep(i, a, b) for( simSize_t i = (a); (i) < (b); ++(i))
#endif

#ifndef rep
#define rev(i, a, b) for( simSize_t i = (a); (i) > (b); --(i))
#endif

/// @brief Combine 4*8 bit color channel into 32bit RGBA, A=0xFF (255)
#define __combiRGB(R, G, B) ((((simColor_t)R)<<24) | (((simColor_t)G)<<16) | (((simColor_t)B)<<8) | ((simColor_t)0xFF))
/// @brief Combine 4*8 bit color channel into 32bit RGBA 
#define __combiRGBA(R, G, B, A) ((((simColor_t)R)<<24) | (((simColor_t)G)<<16) | (((simColor_t)B)<<8) | ((simColor_t)A))
/// @brief Get 8 bit RED from 32 bit RGBA
#define __getRFromRGBA(RGBA)    (((simColor_t)(RGBA) >> 24) & 0xFF)
/// @brief Get 8 bit GREEN from 32 bit RGBA
#define __getGFromRGBA(RGBA)    (((simColor_t)(RGBA) >> 16) & 0xFF)
/// @brief Get 8 bit BLUE from 32 bit RGBA
#define __getBFromRGBA(RGBA)    (((simColor_t)(RGBA) >>  8) & 0xFF)
/// @brief Get 8 bit ALPHA from 32 bit RGBA
#define __getAFromRGBA(RGBA)    (((simColor_t)(RGBA) >>  0) & 0xFF)

#ifndef __entry_exit_critical_section
#define __entry_exit_critical_section
    static inline void __entry_critical_section(pthread_mutex_t* mutex) {
        int rc = pthread_mutex_lock(mutex);
        if (rc) {
            fprintf(stderr, "Mutex lock failed: %d\n", rc);
        }
    }

    static inline void __exit_critical_section(pthread_mutex_t* mutex) {
        int rc = pthread_mutex_unlock(mutex);
        if (rc) {
            fprintf(stderr, "Mutex unlock failed: %d\n", rc);
        }
    }
#endif 

/// @brief Enter SDL critical section (blocking until lock acquired)
// #define __entry_SDL_critical_section()          pthread_mutex_lock(&SDLLock)
/// @brief Try to enter SDL critical section (non-blocking, returns 0 if success)
// #define __try_entry_SDL_critical_section()      pthread_mutex_trylock(&SDLLock)
/// @brief Exit SDL critical section (release lock)
// #define __exit_SDL_critical_section()           pthread_mutex_unlock(&SDLLock)

/// @brief Enter SDL critical section (blocking until lock acquired)
#define __entry_SDL_critical_section() do {                                \
    int __rc = pthread_mutex_lock(&SDLLock);                               \
    if (__rc != 0) {                                                       \
        __sim_log("[SDL_CRIT] pthread_mutex_lock failed (rc=%d, %s)",      \
                  __rc, strerror(__rc));                                   \
    }                                                                      \
} while(0)

/// @brief Try to enter SDL critical section (non-blocking, returns 0 if success)
#define __try_entry_SDL_critical_section() ({                              \
    int __rc = pthread_mutex_trylock(&SDLLock);                            \
    if (__rc != 0 && __rc != EBUSY) {                                      \
        __sim_log("[SDL_CRIT] pthread_mutex_trylock failed (rc=%d, %s)",   \
                  __rc, strerror(__rc));                                   \
    }                                                                      \
    __rc;                                                                  \
})

/// @brief Exit SDL critical section (release lock)
#define __exit_SDL_critical_section() do {                                 \
    int __rc = pthread_mutex_unlock(&SDLLock);                             \
    if (__rc != 0) {                                                       \
        __sim_log("[SDL_CRIT] pthread_mutex_unlock failed (rc=%d, %s)",    \
                  __rc, strerror(__rc));                                   \
    }                                                                      \
} while(0)

typedef int (*simPThreadFunc)(void*);

/// DEBUG SETUP ///////////////////////////////////////////////////////////////////////////////////

#include "../lib/log/log.h"

#define __USEC(i)  i##000ULL                    /// Convert from microsecond to nanosecond 
#define __MSEC(i)  i##000000ULL                 /// Convert from millisecond to nanosecond
#define __SEC(i)   i##000000000ULL              /// Convert from second to nanosecond

/// @brief Sleep for a given number of milliseconds (please give input as integer)
#define __sim_sleep_ms(ms) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (ms) / 1000;                     \
    ts.tv_nsec = ((ms) % 1000) * 1000000L;        \
    nanosleep(&ts, NULL);                         \
} while(FALSE)

/// @brief Sleep for a given number of nanoseconds (input must be integer)
#define __sim_sleep_ns(ns) do {                         \
    struct timespec ts;                                 \
    ts.tv_sec  = (time_t)((ns) / 1000000000UL);         \
    ts.tv_nsec = (long)((ns) % 1000000000UL);           \
    nanosleep(&ts, NULL);                               \
} while(0)

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


/// FLAGS CTL ///////////////////////////////////////////////////////////////////////////////////// 

static inline void __simSetFlag(simFlag_t f){
    if(f >= sizeof(simFlag_t)*8){
        __sim_log("[__simSetFlag] Unknown flag f=%d", f);
        return;
    }
    __entry_critical_section(&simFlagLock);
    simFlag |= (f);
    __exit_critical_section(&simFlagLock);
}

static inline simFlag_t __simCheckFlag(simFlag_t f){
    if(f >= sizeof(simFlag_t)*8){
        __sim_log("[__simSetFlag] Unknown flag f=%d", f);
        return 0;
    }
    simFlag_t res = FALSE;
    __entry_critical_section(&simFlagLock);
    res = (simFlag & (f))?(TRUE):(FALSE);
    __exit_critical_section(&simFlagLock);
    return res;
}

static inline void __simClearFlag(simFlag_t f){
    if(f >= sizeof(simFlag_t)*8){
        __sim_log("[__simSetFlag] Unknown flag f=%d", f);
        return;
    }
    __entry_critical_section(&simFlagLock);
    simFlag |= (f);
    __exit_critical_section(&simFlagLock);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
