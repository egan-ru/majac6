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
    VRCON=0;//генератор опорного

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
        mjN=1; // начальный фронт задний может и отсутствовать
        while(mjN<2); // подождать появления первого вронта т.е это 2

        for(i=950;i;i--){
            while(!msTime);
            msTime=0;
        }
        // wait 900ms after first rising edge

        if(mjN>3) continue; // всё с начала т.к. провал был

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


// проверка пяти импульсов прошла успешно! Теперь даём выходной импульс
// ради которого всё и затевалось.
if(MODE) LED=1; // Включить светодиод для контроля
        for(i=300;i;i--){// Выдать импульс во время -350мс по -50мс
            while(!msTime);
            msTime=0;
        }
if(MODE) LED=0;// посветив от времени -20мс по +950мс погаснуть

// тут должен начаться замер длительности 6-ого импульса
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
 				a++;			// считает время импульса в милисекундах
				if(!state)	break;

			} // до окончания 6-ого импульса считать время
		}

		if(!i) a=0xff; // ошибка слишком много
		if(a<90) a=0xFF;
		if(a!=0xFF)
		{
			unsigned int tempA;
			a-=90;
			tempA=a;

			a/=20; // получится деление с округлением
			tempA%=20;
			if((tempA < 5) || (tempA > 15) ) a = tempA | 0x80; // если ошибка больше нормы - не выдавать данные
														 // а выдать значение остатка от деления

		}

        a<<=1;
        a|=0xFE00; // стоп биты тут
        a&=0xFFFE; // старт бит тут
        send=1;
		while(send); // ждать окончание передачи байта


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
    *((signed char *)(&ADC)+1)>>=1; // если надо с ограничителем уровня
    GODONE=1;

// начало полосового фильтра на 1кГц

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

// конец полосового фильтра 1кГц

// начало  детектора огибающей
    if(!--cntVal) {
        level=0;

    }

    if(y>=0)
    if(level<=*((char*)(&y)+1)){ // if saved level below new value
        level=(char)(y>>8);    // store new value and refresh time-to-live
        cntVal=8;
    }

// конец детектора огибающей
// начало блока выдачи уровня с гистерезисом
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
                // низкий уровень

				if(!MODE) LED=0;
				state=0;
                ust=btemp;
                ust++;
                mjN|=1;
            }
        }
    }
  }
//конец блока выдачи уровня с гистерезисом

}
