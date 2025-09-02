#include "../../include/global.h"
#include "../queue/queue.h"
#include "../log/log.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <stdint.h>
#include <string.h>

#define INTTERUPT_COUNT             10
#define INTERRUPT_ENTRY_EXIT_LOG    1
#define ISR_FUNC(i)                 ISR_FUNC_PTR(i)()
#define ISR_FUNC_PTR(i)             interruptService[i]
#define CLR_ISR_FUNC(i)             ISR_FUNC_PTR(i) = NULL
#define SET_ISR_FUNC(i, isr)        ISR_FUNC_PTR(i) = (isr)

typedef void (*ISR_t)(void); 

extern Queue_t* queueInterrupt;
extern ISR_t    interruptService[INTTERUPT_COUNT];

#ifndef __mask8
    #define __mask8(i)                      (((uint8_t)1) << (i))
#endif
#ifndef __mask32
    #define __mask32(i)                     (((uint32_t)1)<< (i))
#endif
#ifndef __mask64
    #define __mask64(i)                     (((uint64_t)1)<< (i))
#endif
#ifndef __inv_mask8
    #define __inv_mask8(i)                  ((uint8_t)(~((uint8_t)1 << (i))))
#endif
#ifndef __inv_mask32
    #define __inv_mask32(i)                 ((uint32_t)(~((uint32_t)1 << (i))))
#endif
#ifndef __inv_mask64
    #define __inv_mask64(i)                 ((uint64_t)(~((uint64_t)1 << (i))))
#endif
#ifndef __is_null
    #define __is__not_null(ptr) ((ptr)!=NULL)
#endif
#ifndef __is_not_null
    #define __is_not_null(ptr) ((ptr)!=NULL)
#endif

#define isr_func_name(i)          __default_isr##i
#define default_isr_prototype(i)  __attribute__((weak)) void isr_func_name(i) ()
#define default_isr_definition(i) __attribute__((weak)) void isr_func_name(i) (){__sim_log("Default isr%d()", i);}

default_isr_prototype(0); default_isr_prototype(1); default_isr_prototype(2); 
default_isr_prototype(3); default_isr_prototype(4); default_isr_prototype(5); 
default_isr_prototype(6); default_isr_prototype(7); default_isr_prototype(8); 
default_isr_prototype(9); 

simStatus_t interruptInit(){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("interruptInit()");
    #endif

    qInit(&queueInterrupt);
    
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("interruptInit()");
    #endif
    return STATUS_OKE;
}

void simInterruptExit(){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simInterruptExit()");
    #endif

    qFree(queueInterrupt);

    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("simInterruptExit()");
    #endif
}

void loopTrackInterruptService(void* arg){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("loopTrackInterruptService()");
    #endif

    while(simStatus != STATUS_STOPPED){
        while(!qIsEmpty(queueInterrupt)){
            uint32_t keysym;
            qDequeue(queueInterrupt, &keysym, sizeof(keysym));
            if(SDLK_0 <= keysym && keysym <= SDLK_9){
                uint32_t i = keysym - SDLK_0;
                // if(__is_not_null(ptr)
            }
        }
    }

    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("loopTrackInterruptService()");
    #endif
}


