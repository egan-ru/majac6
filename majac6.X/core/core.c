
#include "core.h"


#include <xc.h>

void main(void){
    boot_init();
    wdt_init();
    
    for(;;){
        
    }
}


/*
 * hardware dependent
 * TODO:
 * 
 */
void __interrupt generic_interrupt_handler(void){
    
    
    
}