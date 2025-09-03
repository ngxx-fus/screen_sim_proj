#ifndef __SIM_LOG_H__
#define __SIM_LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stddef.h>
#include <sys/time.h>
#include <pthread.h>

extern pthread_mutex_t  simLogLock;             /// Mutex lock for logging lock

#if !defined(__entry_exit_critical_section) || defined(__entry_exit_critical_section_log_h)
#define __entry_exit_critical_section           /// Macro for entry/exit critical section (global)
#define __entry_exit_critical_section_log_h     /// Macro for entry/exit critial section (inside log.c)
    void __entry_critical_section(pthread_mutex_t* mutex);
    void __exit_critical_section(pthread_mutex_t* mutex);
#endif

void __sim_coreLog(const char* tag, const char* format, ...) __attribute__((format(printf, 2, 3)));
#define __sim_log(...)   __sim_coreLog("log",  __VA_ARGS__)
#define __sim_err(...)   __sim_coreLog("err",  __VA_ARGS__)
#define __sim_entry(...) __sim_coreLog(">>>",  __VA_ARGS__)
#define __sim_exit(...)  __sim_coreLog("<<<",  __VA_ARGS__)

#endif
