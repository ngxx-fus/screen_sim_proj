#include "log.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>

void __sim_vlog(const char* tag, const char* fmt, va_list args) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm* t = localtime(&tv.tv_sec);
    char buf[32];
    strftime(buf, sizeof(buf), "%H:%M:%S", t);

    fprintf(stderr, "[%s.%06ld] [%s] ", buf, tv.tv_usec, tag);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
}

void __sim_log_impl(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __sim_vlog(tag, fmt, args);
    va_end(args);
}
