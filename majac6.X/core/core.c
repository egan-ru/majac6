
#include "core.h"


#include <xc.h>

void main(void){
    boot_init();
    wdt_init();
    coretimer_init();
    
    
    for(;;){
        wdt_ctl();
        
        
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