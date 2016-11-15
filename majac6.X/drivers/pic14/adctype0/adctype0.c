#include "../../../core/hardware.h"

/*pic12f675 have 0..3 channels*/
#define SELECTED_CHANNEL        1
#define MOST_OF_ACCESS_8b
/*
 * TODO:
 * add macro for autoconfig adctype0
 * based on selected channel.
 * add macro for prefered access mode
 * (left justified for 8 bit access,
 * right justified for 10 bit access)
 */


/*
 * init ADC
 * freq: Fosc/16
 * analog input 1
 * left justified
 */
__attribute__((cold)) void adctype0_init(void){ 
    ANSEL= (0x05 << _ANSEL_ADCS_POSN)|((1<<SELECTED_CHANNEL)<<_ANSEL_ANS_POSN);
#ifdef MOST_OF_ACCESS_8b
    ADCON0 = _ADCON0_ADON_MASK|((1<<SELECTED_CHANNEL)<<_ADCON0_CHS0_POSN);
#else
    ADCON0 = _ADCON0_ADFM_MASK|_ADCON0_ADON_MASK|((1<<SELECTED_CHANNEL)<<_ADCON0_CHS0_POSN);
#endif
    PIR1&=~( _PIR1_ADIF_MASK);
}

/*
 * start new ADConversion
 */
void adctype0_ctl(void){
    ADCON0|=_ADCON0_GO_MASK;
    PIR1&=~( _PIR1_ADIF_MASK);
}

/*
 * get value from adc.
 * drop 2 lsb
 * ret 0 if conversion not rdy
 */
uint8_t adctype0_value_get8(void){
    if(PIR1&_PIR1_ADIF_MASK){
        PIR1&=~( _PIR1_ADIF_MASK);
#ifdef MOST_OF_ACCESS_8b 
        return ADRESH;
#else
        return (ADRESL>>2)|(ADRESH<<6);     //bad code can be generated there
#endif
    }
    return 0;
}

/*
 * get value from adc.
 * ret 0 if conversion not rdy
 */
uint16_t adctype0_value_get10(void){
    if(PIR1&_PIR1_ADIF_MASK){
        PIR1&=~( _PIR1_ADIF_MASK);
#ifdef MOST_OF_ACCESS_8b 
        return ((uint16_t)(ADRESH << 2)|(ADRESL>>6));   //bad code can be generated there
#else
        return ((uint16_t)ADRESH|ADRESL);
#endif  
    }
    return 0;    
}
