#include "hardware.h"

#ifdef pic12f675_xc8
/*
 * pic14
 * timer0 based
 * core timer version
 */

void (*volatile ct_handler0)(void);
uint8_t ct_counter0;
uint8_t ct_counter0;
uint8_t ct_period0;

void (*volatile ct_handler1)(void);
uint8_t ct_counter1;
uint8_t ct_counter1;
uint8_t ct_period1;

static uint8_t compare;

__attribute__((cold)) void coretimer_init(void){
    ct_handler0 = NULL;
    ct_handler1 = NULL;
    compare = UINT8_MAX;
}

/*
 * dec timers on delay
 * if delay > counter proc counter and reload
 * counter period value
 * else dec counter on delay value
 */
static void ct_proc(uint8_t delay){
    if(ct_handler0){
        if(ct_counter0 > delay) ct_counter0-=delay; 
        else{
            (*ct_handler0)();
            ct_counter0 = ct_period0;
        }     
    }
    if(ct_handler1){
        if(ct_counter1 > delay) ct_counter1-=delay; 
        else{
            (*ct_handler1)();
            ct_counter1 = ct_period1;
        }     
    }    
}

/*
 * find min value of counters
 * return 0xff if no active timers
 */
static uint8_t ct_find(void){
uint8_t result;    
    result = 0xff;
    if(ct_handler0){
        if(result > ct_counter0) result = ct_counter0;
    }
    if(ct_handler1){
        if(result > ct_counter1) result = ct_counter1;
    }
    return result;
}


__attribute__((hot)) void coretimer_handler(void){  
    INTCON&=~(1<< _INTCON_T0IF_MASK);
    ct_proc(compare);
    if(ct_handler0||ct_handler1){
        compare = ct_find();
        TMR0 = UINT8_MAX - compare;
    }else{
        INTCON&=(1<<_INTCON_T0IE_MASK);
    }
}

result_t coretimer_routine1_start(uint8_t period, void (*handler)(void)){
    if(ct_handler1 == NULL){
        ct_counter0 = ct_period0 = period;
        ct_handler0 = handler;
        return SUCCESS;
    }
    return BUSY;
}

result_t coretimer_routine1_stop(void){
    if(ct_handler1){
        ct_handler1 = 0;
        ct_counter0 = ct_period0 = 0;
        return SUCCESS;
    }
    return ERROR;
}

#endif
