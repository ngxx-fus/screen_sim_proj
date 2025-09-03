#include "sim_interrupt.h"

Queue_t*        interruptEventQueue = NULL;
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


simStatus_t simInterruptInit(){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("interruptInit()");
    #endif

    qInit(&interruptEventQueue);
    posInterruptEventRegister = 0;
    negInterruptEventRegister = 0;
    posInterruptEnableRegister = 0;
    negInterruptEnableRegister = 0;

    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("interruptInit()");
    #endif
    return INT_STATUS_OKE;
}

simStatus_t     simEnableInterrupt(interruptID_t interruptID, interruptType_t typeOfInterrupt){
    switch(typeOfInterrupt){
        case INT_PULLDOWN:
            negInterruptEnableRegister |= __mask32(interruptID);
            break;
        case INT_PULLUP:
            posInterruptEnableRegister |= __mask32(interruptID);
            break;
        default:
            __sim_log("[simEnableInterrupt] typeOfInterrupt=%d is unknown", typeOfInterrupt);
            return INT_UNKNOWN_TYPE;
    
    }
    return INT_STATUS_OKE;
}

simStatus_t     simDisableInterrupt(interruptID_t interruptID, interruptType_t typeOfInterrupt){
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

simStatus_t     simAttachInterrupt(interruptID_t interruptID, interruptType_t typeOfInterrupt, simISRFuncPtr_t isr){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simAttachInterrupt(%d, %d, %p)", interruptID, typeOfInterrupt, isr);
    #endif
    if(interruptID >= INTTERUPT_COUNT) {
        __sim_log("[simAttachInterrupt] interruptID=%d out of range(0, %d)!", interruptID, INTTERUPT_COUNT);
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

simStatus_t     simDetachInterrupt(interruptID_t interruptID, interruptType_t typeOfInterrupt){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simDetachInterrupt(%d, %d)", interruptID, typeOfInterrupt);
    #endif
    if(interruptID >= INTTERUPT_COUNT) {
        __sim_log("[simDetachInterrupt] interruptID=%d out of range(0, %d)!", interruptID, INTTERUPT_COUNT);
        return INT_OUT_OF_RANGE;
    }
    simDisableInterrupt(interruptID, typeOfInterrupt);
    if(typeOfInterrupt == INT_PULLDOWN){
        interruptPullDownService[interruptID] = NULL;
        switch (interruptID) {
            case 0: interruptPullDownService[0] = isr_func_name(0PD); break;
            case 1: interruptPullDownService[1] = isr_func_name(1PD); break;
            case 2: interruptPullDownService[2] = isr_func_name(2PD); break;
            case 3: interruptPullDownService[3] = isr_func_name(3PD); break;
            case 4: interruptPullDownService[4] = isr_func_name(4PD); break;
            case 5: interruptPullDownService[5] = isr_func_name(5PD); break;
            case 6: interruptPullDownService[6] = isr_func_name(6PD); break;
            case 7: interruptPullDownService[7] = isr_func_name(7PD); break;
            case 8: interruptPullDownService[8] = isr_func_name(8PD); break;
            case 9: interruptPullDownService[9] = isr_func_name(9PD); break;
        }
    }else{
        switch (interruptID) {
            case 0: interruptPullDownService[0] = isr_func_name(0PU); break;
            case 1: interruptPullDownService[1] = isr_func_name(1PU); break;
            case 2: interruptPullDownService[2] = isr_func_name(2PU); break;
            case 3: interruptPullDownService[3] = isr_func_name(3PU); break;
            case 4: interruptPullDownService[4] = isr_func_name(4PU); break;
            case 5: interruptPullDownService[5] = isr_func_name(5PU); break;
            case 6: interruptPullDownService[6] = isr_func_name(6PU); break;
            case 7: interruptPullDownService[7] = isr_func_name(7PU); break;
            case 8: interruptPullDownService[8] = isr_func_name(8PU); break;
            case 9: interruptPullDownService[9] = isr_func_name(9PU); break;
        }
    };
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("simDetachInterrupt()");
    #endif
    return STATUS_OKE;
}

void simInterruptExit(){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simInterruptExit()");
    #endif

    qFree(interruptEventQueue);

    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("simInterruptExit()");
    #endif
}

simStatus_t     simPushInterruptEvent(interruptID_t interruptID, interruptType_t typeOfInterrupt){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("simPushInterruptEvent(%d, %d)", interruptID, typeOfInterrupt);
    #endif
    
    if(interruptID >= INTTERUPT_COUNT) {
        __sim_log("[simPushInterruptEvent] interruptID=%d out of range(0, %d)!", interruptID, INTTERUPT_COUNT);
        return INT_OUT_OF_RANGE;
    }
    simISRFuncPtr_t isr = NULL;
    switch (typeOfInterrupt) {
        case INT_PULLDOWN:
            if((negInterruptEnableRegister & __mask8(interruptID)) == 0) break;
            isr = interruptPullDownService[interruptID];
            break;
        case INT_PULLUP:
            if((posInterruptEnableRegister & __mask8(interruptID)) == 0) break;
            isr = interruptPullUpService[interruptID];
            break;
        default:
            __sim_log("[simPushInterruptEvent] typeOfInterrupt=%d is unknown", typeOfInterrupt);
            return INT_UNKNOWN_TYPE;
    }
    if(isr) qPush(interruptEventQueue, &isr, sizeof(isr));
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("simPushInterruptEvent()");
    #endif
    return STATUS_OKE;
}

int  loopTrackInterruptService(void* arg){
    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_entry("loopTrackInterruptService()");
    #endif

    while(simStatus != STATUS_STOPPED){
        while(!qIsEmpty(interruptEventQueue)){
            simISRFuncPtr_t isr;
            qDequeue(interruptEventQueue, &isr, sizeof(isr));
            #if LOG_INTERRUPT_EVENT == 1
                __sim_log("[loopTrackInterruptService] Got the isr=%p", isr);
            #endif
            if(isr) 
                isr();
            #if LOG_INTERRUPT_EVENT == 1
            else 
                __sim_log("[loopTrackInterruptService] Cannot call to the isr!");
            #endif

        }
        __sim_sleep_ns(__USEC(50));
    }

    #if INTERRUPT_ENTRY_EXIT_LOG == 1
        __sim_exit("loopTrackInterruptService()");
    #endif
    return STATUS_OKE;
}


