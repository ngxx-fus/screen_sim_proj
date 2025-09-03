#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stdint.h"

/// DEF ///////////////////////////////////////////////////////////////////////////////////////////

#define SCREEN_W    240
#define SCREEN_H    172
#define FONT_PATH   "/usr/share/fonts/TTF/DejaVuSans.ttf"
#define FONT_SIZE   12
#define RENDER_FPS  100
/// Delay (ms + ns) before flush via <SDL_RenderPresent(gRenderer)>
#define DELAY_BEFORE_FLUSH_MS 10
/// Delay (ms + ns) before flush via <SDL_RenderPresent(gRenderer)>
#define DELAY_BEFORE_FLUSH_NS 0

#ifndef TRUE_FALSE
    #define TRUE    0x1
    #define FALSE   0x0
#endif

typedef int32_t     simSize_t;
typedef int8_t      simStatus_t;
typedef uint32_t    simFlag_t;
typedef uint32_t    simIntReg_t;

#ifndef __mask8
    // Set i-th bit as 1-bit of 8 bits datatype (E.g: i=3 <---> 00001000B) 
    #define __mask8(i)                      (((uint8_t)1) << (i))
#endif
#ifndef __mask32
    // Set i-th bit as 1-bit of 32 bits datatype (E.g: i=8 <---> 00_00_01_00H) 
    #define __mask32(i)                     (((uint32_t)1)<< (i))
#endif
#ifndef __mask64
    // Set i-th bit as 1-bit of 64 bits datatype (E.g: i=8 <---> 00_00_00_00_00_00_01_00H) 
    #define __mask64(i)                     (((uint64_t)1)<< (i))
#endif
#ifndef __inv_mask8
    // Set i-th bit as 0-bit of 8 bits datatype (E.g: i=3 <---> 11110111B) 
    #define __inv_mask8(i)                  ((uint8_t)(~((uint8_t)1 << (i))))
#endif
#ifndef __inv_mask32
    // Set i-th bit as 0-bit of 32 bits datatype (E.g: i=8 <---> 11_11_10_11H) 
    #define __inv_mask32(i)                 ((uint32_t)(~((uint32_t)1 << (i))))
#endif
#ifndef __inv_mask64
    // Set i-th bit as 0-bit of 64 bits datatype (E.g: i=8 <---> 11_11_11_11_11_11_10_11H) 
    #define __inv_mask64(i)                 ((uint64_t)(~((uint64_t)1 << (i))))
#endif

/// Status code
enum RETURN_CODE{                           // Return code
    STATUS_RANGE_ERROR  = -3,               // Out of size or related problem
    ERROR_NULL          = -2,               // Null error
    ERROR_UNKNOWN       = -1,               // Just an error
    STATUS_OKE          = 0,                // Everything is good
};
enum SIM_STATE_CODE{                        // System status
    STATUS_STOPPED      = 0,                // At begin or stopping
    STATUS_INIT         = 1,                // Initializing state
    STATUS_RUNNING      = 2,                // Simulation state
};
enum SIM_FLAG{                              // Flags of status
    FLAG_CONTINUE       = 0x1,              // Continue something 
    FLAG_QUIT           = 0x2,              // Quit something
    FLAG_WAIT           = 0x3,              // Wait for something
    FLAG_RENDER_REQ     = 0x4               // Set render request
};

#endif
