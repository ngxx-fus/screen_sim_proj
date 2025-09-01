#ifndef __SIM_API_H__
#define __SIM_API_H__

/// HEADERS ///////////////////////////////////////////////////////////////////////////////////////
#include "global.h"

/// FUNCS & APIS //////////////////////////////////////////////////////////////////////////////////
#define __set_color(R,G,B,A) \
    do { __entry_critial_section(); SDL_SetRenderDrawColor(gRenderer,R,G,B,A); __exit_critial_section(); } while(0)
#define __set_pixel(x, y) \
    do { __entry_critial_section(); SDL_RenderDrawPoint(gRenderer, x, y); __exit_critial_section(); } while(0)
#define __draw_line(x1,y1,x2,y2) \
    do { __entry_critial_section(); SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2); __exit_critial_section(); } while(0)
#define __draw_rect(x,y,w,h) \
    do { SDL_Rect r = {x,y,w,h}; __entry_critial_section(); SDL_RenderDrawRect(gRenderer,&r); __exit_critial_section(); } while(0)
#define __fill_rect(x,y,w,h) \
    do { SDL_Rect r = {x,y,w,h}; __entry_critial_section(); SDL_RenderFillRect(gRenderer,&r); __exit_critial_section(); } while(0)
#define __delay_ms(D) SDL_Delay(D)
#define __render() SDL_RenderPresent(gRenderer)

#endif