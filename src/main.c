#include "sim_api.h"
#define abs(x) (((x)<0)?(x):(-1*(x)))

void lineBresenham(int x1, int y1, int x2, int y2){
    int x, y, Dx, Dy, p, c1, c2;
    Dx = abs(x2 - x1);
    Dy = abs(y2 - y1);
    p = 2*Dy - Dx;
    c1 = 2*Dy;
    c2 = 2*(Dy-Dx);
    x = x1;
    y = y1;
 
    int x_unit = 1, y_unit = 1;
 
    if (x2 - x1 < 0)
        x_unit = -x_unit;
    if (y2 - y1 < 0)
        y_unit = -y_unit;
 
    if (x1 == x2)
    {
        while (y != y2+1)
        {
            y += y_unit;
            __set_pixel(x, y);
        }
    }
 
    else if (y1 == y2)
    {
        while (x != x2+1)
        {
            x += x_unit;
            __set_pixel(x, y);
        }
    }
    else{
        __set_pixel(x, y);
        while(x != x2){
            if (p<0) p += c1;
            else{
                p += c2;
                y += y_unit;
            }
            x += x_unit;
            __set_pixel(x, y);
        }
    }
}

void main_app(){
    __sim_entry("main_app()");
    while (simStatus == STATUS_RUNNING){
        __sim_log("[main_app()] runing...");
        __sim_sleep(2);
    }
}