#ifndef __SIM_API_H__
#define __SIM_API_H__
#ifdef LOG_HEADER_INCLUDE
#pragma message("INCLUDE: sim_api.h")
#endif
/// HEADERS ///////////////////////////////////////////////////////////////////////////////////////
#include "global.h"
#include "../lib/log/log.h"
#include "../lib/interrupt/sim_interrupt.h"

#define LOG_SET_PIXEL       0
#define LOG_SCREEN_FLUSH    0

/// SCREEN BUFFER API /////////////////////////////////////////////////////////////////////////////
#define __inRangeAB(x, A, B)            (((A) <= (x))  && ((x) < (B)))
#define __inRange0B(x, B)               ((0 <= (x)) && ((x) < (B)))

/// Set the color of a pixel 
static void __simSetPixel(simSize_t x, simSize_t y, simColor_t color){
    #if defined(LOG_SCREEN_FLUSH) && LOG_SCREEN_FLUSH > 0 
        __sim_entry("__simSetPixel()");
    #endif
    if(__inRange0B(x, SCREEN_H) && __inRange0B(y, SCREEN_W)){
        __entry_critical_section(&simScreenBufferLock);
        simScreenBuffer[x + y * SCREEN_H] =  color;
        __exit_critical_section(&simScreenBufferLock);
    }else{
        __sim_err("[__simSetPixel] (x=%d, y=%d) is out of range!", x, y);
    }
    #if defined(LOG_SCREEN_FLUSH) && LOG_SCREEN_FLUSH > 0 
        __sim_exit("__simSetPixel()");
    #endif
}

/// Set flag to flush the screen
static void __simFlushScreenBuffer(){
    #if defined(LOG_SCREEN_FLUSH) && LOG_SCREEN_FLUSH > 0 
        __sim_entry("__simFlushScreenBuffer()");
    #endif
    __simSetFlag(FLAG_RENDER_REQ);
    #if defined(RENDER_FPS)
        SDL_Delay(1000/RENDER_FPS);
    #else 
        SDL_Delay(50);
    #endif
    #if defined(LOG_SCREEN_FLUSH) && LOG_SCREEN_FLUSH > 0 
    __sim_exit("__simFlushScreenBuffer()");
    #endif
}


#endif
