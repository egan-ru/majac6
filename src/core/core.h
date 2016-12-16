#ifndef CORE_H
#define	CORE_H
#include "hardware.h"
#include "core_timer.h"
#include "../drivers/pic14/wdt/wdt.h"
#include "../drivers/pic14/adctype0/adctype0.h"


#ifdef pic12f675_xc8 
/*
 * hardware dependent
 * TODO:
 * this function should be included to startup.as
 * now we use prebuild microchip startup.as
 */
static inline __attribute__((always_inline,cold)) void boot_init(void){
    OPTION_REG=0;
    INTCON=0;
    VRCON=0;
    CMCON= 7;   //comparators off
    WPU=0;
    IOCB=0;
    GPIO=0;
    TRISIO = (1<<1)|(1<<3)|(1<<4)|(1<<5);
    ANSEL = 0;
    INTCON|= _INTCON_GIE_MASK|_INTCON_PEIE_MASK;    
}
#endif

#endif	/* CORE_H */
