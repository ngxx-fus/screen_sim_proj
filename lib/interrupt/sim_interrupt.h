#include "../../include/global.h"
#include "../queue/queue.h"
#include "../log/log.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <stdint.h>
#include <string.h>

#define INTERRUPT_ENTRY_EXIT_LOG    0
#define INTTERUPT_COUNT             10
#define ISR_FUNC(i)                 ISR_FUNC_PTR(i)()
#define ISR_FUNC_PTR(i)             interruptService[i]
#define CLR_ISR_FUNC(i)             ISR_FUNC_PTR(i) = NULL
#define SET_ISR_FUNC(i, isr)        ISR_FUNC_PTR(i) = (isr)

typedef void (*simISRFuncPtr_t)(void); 
enum INTERRUPT_TYPE_OF_INTERRUPT {
    INT_PULLDOWN     = 0,
    INT_PULLUP       = 1,
};
enum INTERRUPT_RETURN_CODE {
    INT_STATUS_OKE      = 0,
    INT_OUT_OF_RANGE    = -1
};

extern pthread_mutex_t simInterruptLock;                            /// Mutex lock for interupt
extern Queue_t*        queueInterrupt;                              /// Queue of interrupt request
extern simIntReg_t     negInterruptEventRegister;                   /// Flags of neg-edge interrupt event 
extern simIntReg_t     posInterruptEventRegister;                   /// Flags of pos-edge interrupt event
extern simIntReg_t     negInterruptEnableRegister;                  /// Flags of neg-edge interrupt enable 
extern simIntReg_t     posInterruptEnableRegister;                  /// Flags of pos-edge interrupt enable 
extern simISRFuncPtr_t interruptPullDownService[INTTERUPT_COUNT];   /// Pointers to ISR_FUNC (pulldown)
extern simISRFuncPtr_t interruptPullUpService[INTTERUPT_COUNT];     /// Pointers to ISR_FUNC (pullup)

#define TO_STRING(i) #i
#define isr_func_name(i)          __isr_fn##i
#define isr_func(i)               isr_func_name(i)()
#define default_isr_prototype(i)  __attribute__((weak)) void isr_func_name(i) ()
#define default_isr_definition(i) __attribute__((weak)) void isr_func_name(i) () \
                                    {__sim_log("Default isr_%s() is called!", TO_STRING(i));}

default_isr_prototype(0PU); default_isr_prototype(1PU); default_isr_prototype(2PU); 
default_isr_prototype(3PU); default_isr_prototype(4PU); default_isr_prototype(5PU); 
default_isr_prototype(6PU); default_isr_prototype(7PU); default_isr_prototype(8PU); 
default_isr_prototype(9PU); 

default_isr_prototype(0PD); default_isr_prototype(1PD); default_isr_prototype(2PD); 
default_isr_prototype(3PD); default_isr_prototype(4PD); default_isr_prototype(5PD); 
default_isr_prototype(6PD); default_isr_prototype(7PD); default_isr_prototype(8PD); 
default_isr_prototype(9PD);

/// @Brief Set-up simulation of interrupt 
simStatus_t     simInterruptInit();
/// @Brief Enable interrupt
/// @Param interruptID          Specify the specific interrupt (0 -> INTTERUPT_COUNT)
/// @Param typeOfInterrupt      Specify the type is PULLDOWN or PULLUP 
simStatus_t     simEnableInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt);
/// @Brief Disable interrupt
/// @Param interruptID          Specify the specific interrupt (0 -> INTTERUPT_COUNT)
/// @Param typeOfInterrupt      Specify the type is PULLDOWN or PULLUP 
simStatus_t     simDisableInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt);
/// @Brief Attach a custom isr_handler
/// @Param interruptID          Specify the specific interrupt (0 -> INTTERUPT_COUNT)
/// @Param typeOfInterrupt      Specify the type is PULLDOWN or PULLUP 
/// @Param isr                  Specify a function will be call when interrupt
simStatus_t     simAttachInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt, simISRFuncPtr_t isr);
/// @Brief Push an interrupt event to queue, then it will be processed
simStatus_t     simPushInterruptEvent(uint8_t interruptID, uint8_t typeOfInterrupt);
/// @Brief Delete queue, ...
void            simInterruptExit();
/// @Brief A thread to process all event on queue 
void            loopTrackInterruptService(void* arg);
