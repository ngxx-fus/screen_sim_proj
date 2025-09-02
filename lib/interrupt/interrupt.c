#include "interrupt.h"

Queue_t* queueInterrupt = NULL;
ISR_t    interruptService[INTTERUPT_COUNT] = {
    isr_func_name(0), isr_func_name(1), isr_func_name(2), 
    isr_func_name(3), isr_func_name(4), isr_func_name(5), 
    isr_func_name(6), isr_func_name(7), isr_func_name(8), 
    isr_func_name(9)
};

default_isr_definition(0); default_isr_definition(1); default_isr_definition(2); 
default_isr_definition(3); default_isr_definition(4); default_isr_definition(5); 
default_isr_definition(6); default_isr_definition(7); default_isr_definition(8); 
default_isr_definition(9); 

