#include "include/main.h"
#include <SDL2/SDL_timer.h>

void __local_isr(){
    __guess_coreLog("guess] [>>>", "__local_isr()");

    uint32_t x0 =  rand()%SCREEN_H, 
             y0 =  rand()%SCREEN_W,
             x1 =  rand()%SCREEN_H,
             y1 =  rand()%SCREEN_W;
    __set_color(WHITE);
    lineBresenham(x0, y1, x1, y1);
    __guess_coreLog("guess] [log", "Call lineBresenham(%d, %d, %d, %d)", x0, y0, x1, y1);

    __guess_coreLog("guess] [<<<", "__local_isr()");
}

void main_app(){
    __guess_entry("main_app()");

    simAttachInterrupt(2, 0, __local_isr);
    srand(time(NULL));
    __set_color(WHITE);
    while (simStatus == STATUS_RUNNING){
        if(rand()%2)
            __set_color(GRAY);
        else
            __set_color(BLACK);

        __set_pixel(rand()%SCREEN_H, rand()%SCREEN_W);
        __render();
    }

    __guess_exit("main_app()");
}
