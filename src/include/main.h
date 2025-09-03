#include "../../include/sim_api.h"
#include "time.h"
#include "math.h"

#define abs(x) (((x)<0)?(x):(-1*(x)))

static void __guess_coreLog(const char* tag, const char* format, ...) {
    struct timeval __timeVal;
    gettimeofday(&__timeVal, NULL);

    struct tm __tmBuf;
    localtime_r(&__timeVal.tv_sec, &__tmBuf);

    char __buffer[32];
    strftime(__buffer, sizeof(__buffer), "%H:%M:%S", &__tmBuf);

    va_list args;
    va_start(args, format);
    printf("[%s.%06ld] [%s] ", __buffer, __timeVal.tv_usec, tag);
    vfprintf(stdout, format, args);
    printf("\n"); 
    fflush(stdout); 
    va_end(args);
}

#define __guess_log(...)    __guess_coreLog("guess] [log", __VA_ARGS__) 
#define __guess_entry(...)  __guess_coreLog("guess] [>>>", __VA_ARGS__)
#define __guess_exit(...)   __guess_coreLog("guess] [<<<", __VA_ARGS__)

/// @brief 
/// @param x1 
/// @param y1 
/// @param x2 
/// @param y2 
void lineBresenham(int x1, int y1, int x2, int y2) {
    int Dx = abs(x2 - x1);
    int Dy = abs(y2 - y1);
    int x_unit = (x2 > x1) ? 1 : -1;
    int y_unit = (y2 > y1) ? 1 : -1;

    int x = x1;
    int y = y1;

    __set_pixel(x, y);

    if (Dy <= Dx) {
        int p = 2 * Dy - Dx;
        int c1 = 2 * Dy;
        int c2 = 2 * (Dy - Dx);
        while (x != x2) {
            x += x_unit;
            if (p < 0) {
                p += c1;
            } else {
                p += c2;
                y += y_unit;
            }
            __set_pixel(x, y);
        }
    } else {
        int p = 2 * Dx - Dy;
        int c1 = 2 * Dx;
        int c2 = 2 * (Dx - Dy);
        while (y != y2) {
            y += y_unit;
            if (p < 0) {
                p += c1;
            } else {
                p += c2;
                x += x_unit;
            }
            __set_pixel(x, y);
        }
    }
}



