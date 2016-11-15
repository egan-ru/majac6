 /**
    This file is simple adctype0 driver for pic14 (get time from radio majac)
    majac6 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple adctype0 driver for pic14 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple adctype0 driver for pic14.  If not, see <http://www.gnu.org/licenses/>.

	topic on ru forum
    http://www.microchip.su/showthread.php?t=17856&page=1
**/

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
