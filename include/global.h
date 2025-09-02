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

/// DEF ///////////////////////////////////////////////////////////////////////////////////////////


/// Status code
enum RETURN_CODE{
    STATUS_RANGE_ERROR  = -3,
    ERROR_NULL          = -2,
    ERROR_UNKNOWN       = -1,
    STATUS_OKE          = 0,
};
enum SIM_STATE_CODE{
    STATUS_STOPPED      = 0,
    STATUS_INIT         = 1,
    STATUS_RUNNING      = 2,
};
enum SIM_FLAG{
    FLAG_CONTINUE       = 0,
    FLAG_QUIT           = 1,
    FLAG_WAIT           = 2,
    FLAG_RENDER_REQ     = 3     
};
/// GLOBL VARS ///////////////////////////////////////////////////////////////////////////////////

/// @brief Main application window
extern SDL_Window       *gWindow;
/// @brief Main SDL renderer used for drawing
extern SDL_Renderer     *gRenderer;
/// @brief General-purpose texture (frame buffer or image)
extern SDL_Texture      *gTexture;
/// @brief Global font resource
extern TTF_Font         *font;
/// @brief Queue for sending messages/events to main thread
extern Queue_t          *toMainThrQueue;
/// @brief Queue for sending messages/events to app thread
extern Queue_t          *toAppThrQueue;
/// @brief Queue for keyboard input events
extern Queue_t          *keyboardEventQueue;
/// @brief Mutex lock for SDL operations (thread-safety)
extern pthread_mutex_t  SDLLock;
/// @brief Current simulation status
extern sim_status_t     simStatus;
/// @Brief Interrupt resgister
extern sim_int_reg_t    negInterruptRegister;
/// @Brief Interrupt resgister
extern sim_int_reg_t    posInterruptRegister;
/// @Brief Software quit flag
extern sim_flag_t       simFlag;

/// @brief Enter critical section (blocking until lock acquired)
#define __entry_critial_section()       pthread_mutex_lock(&SDLLock)
/// @brief Try to enter critical section (non-blocking, returns 0 if success)
#define __try_entry_critial_section()   pthread_mutex_trylock(&SDLLock)
/// @brief Exit critical section (release lock)
#define __exit_critial_section()        pthread_mutex_unlock(&SDLLock)

#define __mask8(i)                      (((uint8_t)1) << (i))
#define __mask32(i)                     (((uint32_t)1)<< (i))
#define __mask64(i)                     (((uint64_t)1)<< (i))
#define __inv_mask8(i)                  ((uint8_t)(~((uint8_t)1 << (i))))
#define __inv_mask32(i)                 ((uint32_t)(~((uint32_t)1 << (i))))
#define __inv_mask64(i)                 ((uint64_t)(~((uint64_t)1 << (i))))

/// DEBUG SETUP ///////////////////////////////////////////////////////////////////////////////////

#include "../lib/log/log.h"

/// @brief Sleep for a given number of milliseconds
#define __sim_sleep_ms(ms) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (ms) / 1000;                     \
    ts.tv_nsec = ((ms) % 1000) * 1000000L;        \
    nanosleep(&ts, NULL);                         \
} while(0)

/// @brief Sleep for a given number of microseconds
#define __sim_sleep_us(us) do {                   \
    struct timespec ts;                           \
    ts.tv_sec  = (us) / 1000000;                  \
    ts.tv_nsec = ((us) % 1000000) * 1000L;        \
    nanosleep(&ts, NULL);                         \
} while(0)

/// @brief Sleep for a given number of seconds
#define __sim_sleep(sec) do {                     \
    struct timespec ts;                           \
    ts.tv_sec  = sec;                             \
    ts.tv_nsec = 0;                               \
    nanosleep(&ts, NULL);                         \
} while(0)


///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
