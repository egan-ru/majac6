 /**
    This file is part of simple adctype0 driver for pic14 (get time from radio majac)
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

#ifndef ADCTYPE0_H
#define ADCTYPE0_H

#define ADCTYPE0_VER       0x00000001

void adctype0_init(void);
void adctype0_ctl(void);
uint8_t adctype0_value_get8(void);
uint16_t adctype0_value_get10(void);

#endif
