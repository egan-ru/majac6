 /**
    This file is part of majac6 (get time from radio majac)
    majac6 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    majac6 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with majac6.  If not, see <http://www.gnu.org/licenses/>.

	topic on ru forum
    http://www.microchip.su/showthread.php?t=17856&page=1

**/

#include "core.h"
#include "../drivers/pic14/soft_uart/soft_uart.h"

typedef enum{
    
    
}majac6_fsm_t;

majac6_fsm_t the_majac6;



void main(void){
    boot_init();
    wdt_init();
    coretimer_init();
    adctype0_init();
    soft_uart_init();
    
    for(;;){
        wdt_ctl();
        
//        if(!waiting){
//            switch(the_majac6){
//                
//                
//            }
//        }
        
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