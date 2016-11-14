#include "hardware.h"

#ifdef pic12f675_xc8
/*
 * pic14
 * timer0 based
 * core timer version
 */

void (*handler0)(void);
void (*handler1)(void);
volatile uint8_t counter0, counter1;
uint8_t pr0, pr1;

static uint8_t compare;

__attribute__((cold)) void coretimer_init(void){
    handler0 = NULL;
    handler1 = NULL;
    compare = UINT8_MAX;
}

__attribute__((hot)) void coretimer_handler(void){
uint8_t min_compare;    
    INTCON&=~(1<< _INTCON_T0IF_MASK);
    min_compare = UINT8_MAX;
    if(handler0){
        if(!counter0){
            (*handler0)();
            counter0 = pr0;
        }else{
            counter0-=compare;
        }
        if(counter0 < min_compare) min_compare = counter0;
    }
    if(handler1){
        if(!counter1){
            (*handler1)();
            counter1 = pr1;
        }else{
            counter1-=compare;
        }
        if(counter1 < min_compare) min_compare = counter1;
    }
    compare = min_compare;
    TMR0 = UINT8_MAX - min_compare;
}

#endif