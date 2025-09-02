#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stdint.h"

typedef int8_t      sim_ret_t;
typedef int32_t     sim_size_t;
/// Default return type  
typedef int8_t      return_t;
/// Type of status
typedef uint8_t     sim_status_t;
typedef uint32_t    sim_flag_t;
typedef uint32_t    sim_int_reg_t;

#define SCREEN_W    240
#define SCREEN_H    172
#define FONT_PATH   "/usr/share/fonts/TTF/DejaVuSans.ttf"
#define FONT_SIZE   12
/// Delay (ms + ns) before flush via <SDL_RenderPresent(gRenderer)>
#define DELAY_BEFORE_FLUSH_MS 200
/// Delay (ms + ns) before flush via <SDL_RenderPresent(gRenderer)>
#define DELAY_BEFORE_FLUSH_NS 0

#endif
