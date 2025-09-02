#ifndef __SIM_API_H__
#define __SIM_API_H__

/// HEADERS ///////////////////////////////////////////////////////////////////////////////////////
#include "global.h"
#include "../lib/log/log.h"

// #define LOG_SET_PIXEL
// #define LOG_RENDER_PIXEL
// #define LOG_SET_COLOR

/// FUNCS & APIS //////////////////////////////////////////////////////////////////////////////////

/// @brief Set color of "pen"
/// @param R Red channel (0–255)
/// @param G Green channel (0–255)
/// @param B Blue channel (0–255)
/// @param A Alpha channel (0–255)
void __set_color(uint8_t R, uint8_t G, uint8_t B, uint8_t A){
    #ifdef LOG_SET_COLOR
        __sim_entry("__set_color(r=%d, g=%d, b=%d, a=%d)", 
                (uint32_t)(R), (uint32_t)(G), (uint32_t)(B), (uint32_t)(A));
    #endif

    __entry_SDL_critical_section();
    SDL_SetRenderDrawColor(gRenderer, R, G, B, A);
    __exit_SDL_critical_section();

    #ifdef LOG_SET_COLOR
        __sim_exit("__set_color(...)");
    #endif
}

/// @brief Draw a pixel with current color (set via __set_color).
/// @param x X-coordinate
/// @param y Y-coordinate
/// @return STATUS_OKE on success, STATUS_RANGE_ERROR if out of range.
simStatus_t __set_pixel(simSize_t x, simSize_t y) {
    #ifdef LOG_SET_PIXEL
        __sim_entry("__set_pixel(x=%d, y=%d)", (uint32_t)(x), (uint32_t)(y));
    #endif

    if (x < 0 || x >= SCREEN_H || y < 0 || y >= SCREEN_W) {
        __sim_log("Range error! | x=%d | y=%d", (int)(x), (int)(y));
        return STATUS_RANGE_ERROR;
    }
    __entry_SDL_critical_section();
    simStatus_t res = SDL_RenderDrawPoint(gRenderer, y, x);
    __exit_SDL_critical_section();

    #ifdef LOG_SET_PIXEL
        __sim_exit("__set_pixel()");
    #endif
    return res;
}

/// @brief Flush the current rendering buffer to the screen.
/// Adds a delay before presenting to control framerate.
/// @return STATUS_OKE on success.
simStatus_t __render(){
    #ifdef log_set_pixel
        __sim_entry("__render()");
    #endif
    
    struct timespec ts;
    ts.tv_sec  = (DELAY_BEFORE_FLUSH_MS) / 1000;
    ts.tv_nsec = DELAY_BEFORE_FLUSH_NS + (DELAY_BEFORE_FLUSH_MS%1000) * 1000000ULL;
    nanosleep(&ts, NULL);   
    // __entry_SDL_critical_section(); 
    // SDL_RenderPresent(gRenderer);
    simFlag |= __mask32(FLAG_RENDER_REQ);
    // __exit_SDL_critical_section();
    
    #ifdef LOG_SET_PIXEL
        __sim_exit("__render()");
    #endif
    return STATUS_OKE;
}

/// @brief Draw a line between two points.
/// @param x1 Starting X
/// @param y1 Starting Y
/// @param x2 Ending X
/// @param y2 Ending Y
#define __draw_line(x1,y1,x2,y2) \
        do { \
            __entry_SDL_critical_section(); \
            SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2); \
            __exit_SDL_critical_section(); \
        } while(FALSE)

/// @brief Draw a rectangle outline.
/// @param x Top-left X
/// @param y Top-left Y
/// @param w Width
/// @param h Height
#define __draw_rect(x,y,w,h) \
        do { \
            SDL_Rect r = {x,y,w,h}; \
            __entry_SDL_critical_section(); \
            SDL_RenderDrawRect(gRenderer,&r); \
            __exit_SDL_critical_section(); \
        } while(FALSE)

/// @brief Draw a filled rectangle.
/// @param x Top-left X
/// @param y Top-left Y
/// @param w Width
/// @param h Height
#define __fill_rect(x,y,w,h) \
        do { \
            SDL_Rect r = {x,y,w,h}; \
            __entry_SDL_critical_section(); \
            SDL_RenderFillRect(gRenderer,&r); \
            __exit_SDL_critical_section(); \
        } while(FALSE)

/// @brief Delay execution for a given number of milliseconds.
/// @param D Time in milliseconds
#define __delay_ms(D) SDL_Delay(D)

#endif
