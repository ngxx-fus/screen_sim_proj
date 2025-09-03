#include "log.h"

void __sim_coreLog(const char* tag, const char* format, ...) {
    struct timeval __timeVal;
    gettimeofday(&__timeVal, NULL);

    struct tm __tmBuf;
    localtime_r(&__timeVal.tv_sec, &__tmBuf);

    char __buffer[32];
    strftime(__buffer, sizeof(__buffer), "%H:%M:%S", &__tmBuf);

    va_list args;
    va_start(args, format);
    fprintf(stderr, "[%s.%06ld] [%s] ", __buffer, __timeVal.tv_usec, tag);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}


// void __sim_vlog(const char* tag, const char* fmt, va_list args) {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
// 
//     struct tm* t = localtime(&tv.tv_sec);
//     char buf[32];
//     strftime(buf, sizeof(buf), "%H:%M:%S", t);
// 
//     fprintf(stderr, "[%s.%06ld] [%s] ", buf, tv.tv_usec, tag);
//     vfprintf(stderr, fmt, args);
//     fprintf(stderr, "\n");
// }
// 
// void __sim_log_impl(const char* tag, const char* fmt, ...) {
//     va_list args;
//     va_start(args, fmt); /// Collect all variables after `fmt` 
//     __sim_vlog(tag, fmt, args);
//     va_end(args);
// }
