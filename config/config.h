#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stdint.h"

#ifdef LOG_HEADER_INCLUDE
#pragma message("INCLUDE: config.h")
#endif 

/// DEF ///////////////////////////////////////////////////////////////////////////////////////////

#define SCREEN_W    640U 
#define SCREEN_H    480U
#define FONT_PATH   "/usr/share/fonts/TTF/DejaVuSans.ttf"
#define FONT_SIZE   12
#define RENDER_FPS  60
/// Delay (ms + ns) before flush via <SDL_RenderPresent(gRenderer)>
#define DELAY_BEFORE_FLUSH_MS 10
/// Delay (ms + ns) before flush via <SDL_RenderPresent(gRenderer)>
#define DELAY_BEFORE_FLUSH_NS 0

#ifndef TRUE_FALSE
    #define TRUE    0x1
    #define FALSE   0x0
#endif

typedef int8_t      simStatus_t;
typedef uint32_t    simColor_t;
typedef int32_t     simSize_t;
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

enum RETURN_CODE{                           // Return code
    STATUS_RANGE_ERROR  = -3,               // Out of size or related problem
    ERROR_NULL          = -2,               // Null error
    ERROR_UNKNOWN       = -1,               // Just an error
    STATUS_OKE          = 0,                // Everything is good
};
/// Status code [unsigned: 0-->255]
enum SIM_STATE_CODE{                        // System status
    STATUS_STOPPED      = 0,                // At begin or stopping
    STATUS_INIT         = 1,                // Initializing state
    STATUS_RUNNING      = 2,                // Simulation state
};
// Flags are using in the simulation
enum SIM_FLAG{
    FLAG_RESERVED       = 0,
    FLAG_CONTINUE       = 1,                // Continue something 
    FLAG_QUIT           = 2,                // Quit something
    FLAG_WAIT           = 3,                // Wait for something
    FLAG_RENDER_REQ     = 4,                // Set render request
    FLAG_RESERVED1, 
    FLAG_RESERVED2, 
    FLAG_RESERVED3, 
    FLAG_RESERVED4, 
    FLAG_RESERVED5, 
    FLAG_RESERVED6, 
    FLAG_RESERVED7, 
    FLAG_RESERVED8, 
    FLAG_RESERVED9, 
    FLAG_RESERVED10, 
    FLAG_RESERVED11, 
    FLAG_RESERVED12, 
    FLAG_RESERVED13, 
    FLAG_RESERVED14, 
    FLAG_RESERVED15, 
    FLAG_RESERVED16, 
    FLAG_RESERVED17, 
    FLAG_RESERVED18, 
    FLAG_RESERVED19, 
    FLAG_RESERVED20, 
    FLAG_RESERVED21, 
    FLAG_RESERVED22, 
    FLAG_RESERVED23, 
    FLAG_RESERVED24, 
    FLAG_RESERVED25, 
    FLAG_RESERVED26, 
    FLAG_RESERVED27,                        // MAX FLAG VALUE : 31
};

#endif
