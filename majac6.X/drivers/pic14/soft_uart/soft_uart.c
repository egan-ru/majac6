 /**
    This file is simple soft uart driver for pic14 (get time from radio majac)
    majac6 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    simple soft uart driver for pic14 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with simple soft uart driver for pic14.  If not, see <http://www.gnu.org/licenses/>.

	topic on ru forum
    http://www.microchip.su/showthread.php?t=17856&page=1
**/

#include "../../../core/hardware.h"

#define OUT_PIN     GPIO0

typedef enum{
    idle = 0,
    tx_req,
    start,
    data,
    stop,
    done,
}soft_uart_state_t;

void soft_uart_init(void){
    
}

void soft_uart_tx(uint8_t data){
    
}