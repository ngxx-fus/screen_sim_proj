#ifndef __SIM_LOG_H__
#define __SIM_LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stddef.h>
#include <sys/time.h>

void __sim_coreLog(const char* tag, const char* format, ...)
     __attribute__((format(printf, 2, 3)));
#define __sim_log(...)   __sim_coreLog("log",  __VA_ARGS__)
#define __sim_err(...)   __sim_coreLog("err",  __VA_ARGS__)
#define __sim_entry(...) __sim_coreLog(">>>",  __VA_ARGS__)
#define __sim_exit(...)  __sim_coreLog("<<<",  __VA_ARGS__)

// void __sim_vlog(const char* tag, const char* fmt, va_list args);
// void __sim_log_impl(const char* tag, const char* fmt, ...);
// #define __sim_log(...)   __sim_log_impl("log",  __VA_ARGS__)
// #define __sim_err(...)   __sim_log_impl("err",  __VA_ARGS__)
// #define __sim_entry(...) __sim_log_impl(">>>",  __VA_ARGS__)
// #define __sim_exit(...)  __sim_log_impl("<<<",  __VA_ARGS__)



#endif
