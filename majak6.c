 /**
    This file is part of majac6 (get time from radio majac)
    majac6 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    majac6 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with majac6.  If not, see <http://www.gnu.org/licenses/>.

	topic on ru forum
    http://www.microchip.su/showthread.php?t=17856&page=1

**/

#include "pic.h"



__CONFIG(HS & PWRTEN & MCLRDIS & BORDIS & WDTDIS);
/*
char buff[32]; // RS-232 output buffer
char beging=0,end=0;
*/


#define LED GPIO2
#define MODE GPIO3
bit msTime=0;
char mjN=0;
bit state=0; //copy for level detector state

//RS-232 variables
bit send=0; //should we transmit or not
unsigned int a;
#define pinOut GPIO0




void main(void){
unsigned int i;


    OPTION=0x0F; //TMR0 source is internal w wiith no prescaler
    VRCON=0;//shutdown comparator voltage reference

    CMCON=7; //Comparator off
    WPU=0;//disable all weak pullups
    INTCON=0;//disable all interrupts
    IOCB=0;//disable interruprs on port change
    GPIO=0;//all pins to 0
    TRISIO=0b111010;//all pins are inputs except GP0 and GP2

	pinOut=1;

    //ADCON0=0b00001001;//ADC: Select Ch2(AN2), Left justify, ADC On
    //ANSEL= 0b00010100;//ADC: conversion clock Fosc/8, AN2 is analog pin
    ADCON0=0b00000101;//ADC: Select Ch1(AN1), Left justify, ADC On
    ANSEL= 0b01010010;//ADC: conversion clock Fosc/16, AN1 is analog pin
    T0IE=1;
    GIE=1;


	do{
        mjN=1; // initiate dgs counter to 1 because first falling edge may absent
        while(mjN<2); // wait fitst rising edge

        for(i=950;i;i--){
            while(!msTime);
            msTime=0;
        }
        // wait 900ms after first rising edge

        if(mjN>3) continue; // go to begin due to falling edge

        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=4) continue; // if no second rising edge in window 950-1050

        for(i=900;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>5) continue; // too much rising edges in window 1050-1950

        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=6) continue; // if no third rising edge in window 1950-2050

        for(i=900;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>7) continue; // too much rising edges in window 2050-2950
        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=8) continue; // if no fourth rising edge in window 2950-3050

        for(i=900;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>9) continue; // too much rising edges in window 3050-3950

        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=10) continue; // if no fifth rising edge in window 3950-4050

        for(i=600;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>11) continue; // too much rising edges in window 4050-4650


// all five input pulses are ok
// begin output pulse
if(MODE) LED=1; // switch on control LED
        for(i=300;i;i--){// make pulse in time range from -350ms to -50ms
            while(!msTime);
            msTime=0;
        }
if(MODE) LED=0;// control LED was on from -20ms to +950ms, then swotch off

// here should start width measurement of 6-th pulse
        a=0;
		for(i=60;i;i--)
		{
			while(!msTime);
			msTime=0;
			if(state) break;

		}
		if(!i)
		{
			a=0xFF;
		}
		else
		{
			for(i=700;i;i--)
			{
				while(!msTime);
				msTime=0;
 				a++;			// counte pulse width in ms
				if(!state)	break;

			} // count time until the last of the sixth pulse
		}

		if(!i) a=0xff; // error! too much
		if(a<90) a=0xFF;
		if(a!=0xFF)
		{
			unsigned int tempA;
			a-=90;
			tempA=a;

			a/=20; // integer division
			tempA%=20;
			if((tempA < 5) || (tempA > 15) ) a = tempA | 0x80; // if error more than expected - don't send data
														 // but send reminder of division

		}

        a<<=1;
        a|=0xFE00; // stop bits
        a&=0xFFFE; // start bit
        send=1;
		while(send); // wait byte transmit ends


	}while(1);
}


void
interrupt isr(void){  //ISR should execute exactly at 8kHz rate (from timer)
static char cntBit=11; // bits counter

static signed int c,e;
static unsigned char level=0;
static signed char cntVal=8; // countdown from 8

signed int b,y;
signed int temp1;
static unsigned int ADC;
static char dvdrTime=8;
static char ust=0;
char    btemp;

    T0IF=0;


  if(send)
  {
    TMR0+=-205;// for 9600 bps soft UART

        if(--cntBit)
		{
            if(a&1) pinOut=1;
            else pinOut=0;
            *(signed int *)&a>>=1;
        }
        else
		{
            cntBit=11;
            send=0;
        }
  }
  else
  {
    TMR0+=-247;//for 125us


    *((char*)(&ADC)+1)=ADRESH^0x80; // 8 bits is enough
    *((signed char *)(&ADC)+1)>>=1; // if we need level cut
    GODONE=1;

// begin 1kHz bandpass filter

    b=ADC-e;
    b>>=1;
    b>>=1;
    b>>=1;
    b>>=1;  // integer divide by 16
            // for 170 Hz bandwith
    y=b+e;
    *((char *)(&temp1))=*((char *)(& e))=    y & 0xFF;
    *((char *)(&temp1)+1)=*((char *)(& e)+1)=y >> 8;

    temp1>>=1;      // *1/2
    temp1>>=1;      // *1/4
    temp1+=e;       // *5/4

    temp1>>=1;      // *5/8
    temp1+=e;       // *13/8
    temp1>>=1;      // *13/16
    temp1>>=1;      // *13/32

    e+=temp1;       // *45/32 = 1.40625 it almost 2*cos(Pi*2*1кГц/8кГц)=1.4142135
                    // or 1007 Hz at 8kHz ADC rate
    e-=c;
    c=y+b;

// end 1kHz bandpass filter

// begin envelope detector
    if(!--cntVal) {
        level=0;

    }

    if(y>=0)
    if(level<=*((char*)(&y)+1)){ // if saved level below new value
        level=(char)(y>>8);    // store new value and refresh time-to-live
        cntVal=8;
    }

// end envelope detector
// begin level output with hysteresis
    if(!--dvdrTime){
        dvdrTime|=8;
        msTime=1;

        btemp=level>>1;
        if(ust<btemp) {
            // high level

			if(!MODE) LED=1;
			state=1;
            ust=level;
            mjN++;
            mjN&=0xFE;// count only transition 0->1
        }
        else {
            btemp=ust>>1;
            if(btemp>=level){
                // low level

				if(!MODE) LED=0;
				state=0;
                ust=btemp;
                ust++;
                mjN|=1;
            }
        }
    }
  }
// end level output with hysteresis

}
