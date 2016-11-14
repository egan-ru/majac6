#include "../../../core/hardware.h"

/*pic12f675 have 0..3 channels*/
#define SELECTED_CHANNEL        1

/*
 * init ADC
 * freq: Fosc/16
 * analog input 1
 * left justified
 */
void adctype0_init(void){ 
    ANSEL= (0x05 << _ANSEL_ADCS_POSN)|((1<<SELECTED_CHANNEL)<<_ANSEL_ANS_POSN);
    ADCON0 = _ADCON0_ADON_MASK|((1<<SELECTED_CHANNEL)<<_ADCON0_CHS0_POSN);
}

void adctype0_ctl(void){
    
}
