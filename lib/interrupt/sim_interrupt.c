#include "sim_interrupt.h"

Queue_t*        queueInterrupt = NULL;
simIntReg_t     negInterruptEventRegister = 0; 
simIntReg_t     posInterruptEventRegister = 0;
simIntReg_t     negInterruptEnableRegister = 0;
simIntReg_t     posInterruptEnableRegister = 0;

simISRFuncPtr_t interruptPullDownService[INTTERUPT_COUNT] = {
    isr_func_name(0PD), isr_func_name(1PD), isr_func_name(2PD), 
    isr_func_name(3PD), isr_func_name(4PD), isr_func_name(5PD), 
    isr_func_name(6PD), isr_func_name(7PD), isr_func_name(8PD), 
    isr_func_name(9PD),
};
simISRFuncPtr_t interruptPullUpService[INTTERUPT_COUNT] = {
    isr_func_name(0PU), isr_func_name(1PU), isr_func_name(2PU), 
    isr_func_name(3PU), isr_func_name(4PU), isr_func_name(5PU), 
    isr_func_name(6PU), isr_func_name(7PU), isr_func_name(8PU), 
    isr_func_name(9PU), 
};

default_isr_definition(0PU); default_isr_definition(1PU); default_isr_definition(2PU); 
default_isr_definition(3PU); default_isr_definition(4PU); default_isr_definition(5PU); 
default_isr_definition(6PU); default_isr_definition(7PU); default_isr_definition(8PU); 
default_isr_definition(9PU); 

default_isr_definition(0PD); default_isr_definition(1PD); default_isr_definition(2PD); 
default_isr_definition(3PD); default_isr_definition(4PD); default_isr_definition(5PD); 
default_isr_definition(6PD); default_isr_definition(7PD); default_isr_definition(8PD); 
default_isr_definition(9PD);


simStatus_t interruptInit(){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("interruptInit()");
    #endif

    qInit(&queueInterrupt);
    
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("interruptInit()");
    #endif
    return INT_STATUS_OKE;
}

simStatus_t     simEnableInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt){
    if(typeOfInterrupt == INT_PULLDOWN){
        negInterruptEnableRegister |= __mask32(interruptID);
    }else{
        posInterruptEnableRegister |= __mask32(interruptID);
    }
    return INT_STATUS_OKE;
}

simStatus_t     simDisableInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simDisableInterrupt(%d, %d)", interruptID, typeOfInterrupt);
    #endif   
    if(typeOfInterrupt == INT_PULLDOWN){
        negInterruptEnableRegister &= __inv_mask32(interruptID);
    }else{
        posInterruptEnableRegister &= __inv_mask32(interruptID);
    }
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("simDisableInterrupt(...)");
    #endif   
    return INT_STATUS_OKE;
}

simStatus_t     simAttachInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt, simISRFuncPtr_t isr){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simAttachInterrupt(%d, %d, %p)", interruptID, typeOfInterrupt, isr);
    #endif
    if(interruptID >= INTTERUPT_COUNT) {
        __sim_log("interruptID=%d out of range(0, %d)!", interruptID, INTTERUPT_COUNT);
        return INT_OUT_OF_RANGE;
    }
    simEnableInterrupt(interruptID, typeOfInterrupt);
    if(typeOfInterrupt == INT_PULLDOWN){
        interruptPullDownService[interruptID] = isr;
    }else{
        interruptPullUpService[interruptID] = isr;
    };
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("simAttachInterrupt()");
    #endif
    return STATUS_OKE;
}

simStatus_t     simDetachInterrupt(uint8_t interruptID, uint8_t typeOfInterrupt){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simDetachInterrupt(%d, %d)", interruptID, typeOfInterrupt);
    #endif
    if(interruptID >= INTTERUPT_COUNT) {
        __sim_log("interruptID=%d out of range(0, %d)!", interruptID, INTTERUPT_COUNT);
        return INT_OUT_OF_RANGE;
    }
    simDisableInterrupt(interruptID, typeOfInterrupt);
    if(typeOfInterrupt == INT_PULLDOWN){
        interruptPullDownService[interruptID] = NULL;
    }else{
        interruptPullUpService[interruptID] = NULL;
    };
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("pushInterruptEvent()");
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


