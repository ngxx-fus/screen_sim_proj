/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include "../config/color.h"
#include "../config/config.h"

/// GLOBL VARS ///////////////////////////////////////////////////////////////////////////////////
extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;

/// APIs //////////////////////////////////////////////////////////////////////////////////////////
#define __set_pixel(x, y)               SDL_RenderDrawPoint(gRenderer, x, y)
#define __delay_ms(D)                   SDL_Delay(D)
#define __set_color(R,G,B,A)            SDL_SetRenderDrawColor(gRenderer,R,G,B,A)
#define __render()                      SDL_RenderPresent(gRenderer)
