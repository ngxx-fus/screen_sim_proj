#include "include/main.h"

void main_app(){
    __sim_entry("main_app()");
    srand(time(NULL));
    __set_color(WHITE);
    while (simStatus == STATUS_RUNNING){
        // __sim_log("[main_app()] runing...");
        if(rand()%2)
            __set_color(WHITE);
        else
            __set_color(BLACK);
        __set_pixel(rand()%SCREEN_H, rand()%SCREEN_W);
        lineBresenham(rand()%SCREEN_H, rand()%SCREEN_W, rand()%SCREEN_H, rand()%SCREEN_W);
        __render();
    }
    __sim_exit("main_app()");
}
