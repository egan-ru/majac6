#ifndef ADCTYPE0_H
#define ADCTYPE0_H

#define ADCTYPE0_VER       0x00000001

void adctype0_init(void);
void adctype0_ctl(void);
uint8_t adctype0_value_get8(void);
uint16_t adctype0_value_get10(void);

#endif
