#include "../include/main.h"

simColor_t backgroundColor = HEX32_COLORS[0];

void __change_background(){
    static uint8_t colorID = 0;
    backgroundColor = HEX32_COLORS[++colorID];
    rep(i, 0, SCREEN_H*SCREEN_W)
        simScreenBuffer[i] = backgroundColor;
}

void __local_isr(){
    __guess_coreLog("guess] [>>>", "__local_isr()");
    uint32_t x0 =  rand()%SCREEN_H, 
             y0 =  rand()%SCREEN_W,
             x1 =  rand()%SCREEN_H,
             y1 =  rand()%SCREEN_W;
    lineBresenham(x0, y1, x1, y1);
    __guess_coreLog("guess] [log", "Call lineBresenham(%d, %d, %d, %d)", x0, y0, x1, y1);

    __guess_coreLog("guess] [<<<", "__local_isr()");
}

void main_app(){
    __guess_entry("main_app()");
    simAttachInterrupt(1, 0, __local_isr);
    simAttachInterrupt(2, 0, __change_background);
    srand(time(NULL));
    while (simStatus == STATUS_RUNNING){
        __simSetPixel(rand()%SCREEN_H, rand()%SCREEN_W, HEX32_COLORS[rand()%30]);
        __simFlushScreenBuffer();
    }

    __guess_exit("main_app()");
}
