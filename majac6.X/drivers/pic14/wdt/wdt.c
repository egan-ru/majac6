#include "../../../core/hardware.h"

/*
 * set prescaller 256 to wdt and set on
 * 
 */
void __attribute__((cold)) wdt_init(void){
    OPTION_REG&= ~(_OPTION_REG_PS_MASK|_OPTION_REG_PSA_MASK);
    OPTION_REG|= (0x7 << _OPTION_REG_PS_POSITION)|(_OPTION_REG_PSA_MASK);
}


__attribute__((hot,leaf)) void wdt_ctl(void){
    asm("clrwdt");
}