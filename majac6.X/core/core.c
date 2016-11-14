
#include "core.h"

void main(void){
    boot_init();
    wdt_init();
    coretimer_init();
    adctype0_init();
    
    
    for(;;){
        wdt_ctl();
        /*user application there*/
        
    }
}


/*
 * hardware dependent
 * TODO:
 * 
 */
void __interrupt generic_interrupt_handler(void){
    if(CORETIMER_REQ){
        coretimer_handler();
    }
}