#ifndef CORE_H
#define	CORE_H
#include "hardware.h"


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
    
    //ADCON0=0b00001001;//вкл AN2 и АЦП выравнивание влево
    //ANSEL= 0b00010100;//Fosc/8 вход2 аналоговый
    
    
  //  ADCON0=0b00000101;//вкл AN1 и АЦП выравнивание влево
   // ANSEL= 0b01010010;//Fosc/16 вход1 аналоговый
    
 //   T0IE=1;
 //   GIE=1;    
    
    
}
#endif

#endif	/* CORE_H */
