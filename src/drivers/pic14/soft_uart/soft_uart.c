 /**
    This file is simple soft uart driver for pic14 (get time from radio majac)
    majac6 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple soft uart driver for pic14 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple soft uart driver for pic14.  If not, see <http://www.gnu.org/licenses/>.

	topic on ru forum
    http://www.microchip.su/showthread.php?t=17856&page=1
**/

#include "../../../core/hardware.h"
#include "../../../core/core_timer.h"

/*
 * todo:
 * add rx primitives
 * int start, some ct handler - routine
 */

/*module use ct_handler1 resource*/
#define BAUDRATE9600    103
#define BIT_COUNT       8

#define PARITY_EVEN


#define LINE_CLR    do{GPIO&=~(1<<0);}while(0)
#define LINE_SET    do{GPIO|=(1<<0);}while(0)


typedef enum{
    idle = 0,
    start,
    data,
#ifdef PARITY_EVEN
    parity,      
#endif
    stop,
    done,
}soft_uart_state_t;

soft_uart_state_t soft_uart_state;
static uint8_t substate;
static uint8_t tx_data;

#ifdef PARITY_EVEN
static bool parity_even;
#endif

void soft_uart_init(void){
    LINE_SET;
    soft_uart_state = idle;
    substate = 0;
    ct_handler1 = NULL;
}

/*
 * should be called every baud period
 */
void soft_uart_ctl(void){
    switch(soft_uart_state){
        case start:
            LINE_SET;
            ++soft_uart_state;
            substate = BIT_COUNT;
        break;
        
        case data:
            if(tx_data&0x01){
                LINE_SET;
            }else{
                LINE_CLR;
            }
            tx_data>>=1;
            --substate;
            if(!substate) ++soft_uart_state;
        break;
        
        
#ifdef PARITY_EVEN       
        case parity:
            if(parity_even){
                LINE_SET;
            }else{
                LINE_CLR;
            }       
            ++soft_uart_state;
        break;
#endif
        
        case stop:
            LINE_SET;
            ++soft_uart_state;
        break;
        
        case done:
        default:
            LINE_SET;
            soft_uart_state = idle;
            substate = 0;
            ct_handler1 = NULL;
        break;
    }
}

result_t soft_uart_tx(uint8_t data){
#ifdef PARITY_EVEN 
uint8_t parity_counter, parity_data;
#endif    
    
    if(soft_uart_state == idle){
        soft_uart_state = start;
        substate = 0;
        tx_data = data;
        #ifdef PARITY_EVEN
        parity_data = 0;
        for(parity_counter = 8;parity_counter;--parity_counter){
            parity_data+= data&0x01;
            data>>=1;
        }
        parity_even = parity_data&0x01;
        #endif
        coretimer_routine1_start(BAUDRATE9600, soft_uart_ctl);
        return SUCCESS;
    }
    return BUSY;
}

