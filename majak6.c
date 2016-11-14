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

//
//	��������� ��� PIC12F675 � ����������� �� ������ 8���
//	������������� ��� ��������� ������� ������� �������
//	�� "����� ����" � �������� ������������ 6-��� �������
//  ��� ��������� ���� �� ����������� ������� �.�. ���������� ���
//  ������ �� ����������� �������
//	�����: �������� ����� ���������� �.������������
//	imeda@yandex.ru	- ��� ��������
//	�������� �� ����� ������ �� ����������, �������� ��� ����������
//	��������� ��� ����� 9.50
//  2006�.

#include "pic.h"



__CONFIG(HS & PWRTEN & MCLRDIS & BORDIS & WDTDIS);
/*
char buff[32]; // ����� ��� ������ � ���� RS232
char beging=0,end=0;
*/


#define LED GPIO2
#define MODE GPIO3 // ����� ������
bit msTime=0;
char mjN=0;
bit state=0; // ����� ��������� ��������� ������

// ���������� ��� RS232
bit send=0; // ���������� ��� ��� ������
unsigned int a;
#define pinOut GPIO0




void main(void){
unsigned int i;


    OPTION=0x0F;//������������ ��� ���0 �� �����������
    VRCON=0;//��������� ��������

    CMCON=7;//���� ������������
    WPU=0;//�������� ��� ���������
    INTCON=0;//������ ���� ����������
    IOCB=0;//���������� �� ��� ������
    GPIO=0;//��� ���� ��������
    TRISIO=0b111010;//��� ����� ����� GP0 � GP2

	pinOut=1;

    //ADCON0=0b00001001;//��� AN2 � ��� ������������ �����
    //ANSEL= 0b00010100;//Fosc/8 ����2 ����������
    ADCON0=0b00000101;//��� AN1 � ��� ������������ �����
    ANSEL= 0b01010010;//Fosc/16 ����1 ����������
    T0IE=1;
    GIE=1;


	do{
        mjN=1; // ��������� ����� ������ ����� � �������������
        while(mjN<2); // ��������� ��������� ������� ������ �.� ��� 2

        for(i=950;i;i--){
            while(!msTime);
            msTime=0;
        }
        // ����� ������� ������ ��������� 900��

        if(mjN>3) continue; // �� � ������ �.�. ������ ���

        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=4) continue; // ���� ��� ������� ������ � ������� 950-1050

        for(i=900;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>5) continue; // ����� ������� �� ����� 1050-1950

        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=6) continue; // ���� ��� �������� ������ � ������� 1950-2050

        for(i=900;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>7) continue; // ����� ������� �� ����� 2050-2950
        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=8) continue; // ���� ��� ��������� ������ � ������� 2950-3050

        for(i=900;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>9) continue; // ����� ������� �� ����� 3050-3950

        for(i=100;i;i--){
            while(!msTime);
            msTime=0;
        }
        if(mjN!=10) continue; // ���� ��� ������ ������ � ������� 3950-4050

        for(i=600;i;i--){
            while(!msTime);
            msTime=0;
        }

        if(mjN>11) continue; // ����� ������� �� ����� 4050-4650


// �������� ���� ��������� ������ �������! ������ ��� �������� �������
// ���� �������� �� � ����������.
if(MODE) LED=1; // �������� ��������� ��� ��������
        for(i=300;i;i--){// ������ ������� �� ����� -350�� �� -50��
            while(!msTime);
            msTime=0;
        }
if(MODE) LED=0;// �������� �� ������� -20�� �� +950�� ���������

// ��� ������ �������� ����� ������������ 6-��� ��������
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
 				a++;			// ������� ����� �������� � ������������
				if(!state)	break;

			} // �� ��������� 6-��� �������� ������� �����
		}

		if(!i) a=0xff; // ������ ������� �����
		if(a<90) a=0xFF;
		if(a!=0xFF)
		{
			unsigned int tempA;
			a-=90;
			tempA=a;

			a/=20; // ��������� ������� � �����������
			tempA%=20;
			if((tempA < 5) || (tempA > 15) ) a = tempA | 0x80; // ���� ������ ������ ����� - �� �������� ������
														 // � ������ �������� ������� �� �������

		}

        a<<=1;
        a|=0xFE00; // ���� ���� ���
        a&=0xFFFE; // ����� ��� ���
        send=1;
		while(send); // ����� ��������� �������� �����


	}while(1);
}


void
interrupt isr(void){  //���������� ����� 8��� ��������� ����������
static char cntBit=11; // ��� ����� ����� ���

static signed int c,e;
static unsigned char level=0;
static signed char cntVal=8; // ������� �� 8

signed int b,y;
signed int temp1;
static unsigned int ADC;
static char dvdrTime=8;
static char ust=0;
char    btemp;

    T0IF=0;


  if(send)
  {
    TMR0+=-205;//������������� 9600 ���/���

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
    TMR0+=-247;//������������� 125���


    *((char*)(&ADC)+1)=ADRESH^0x80; // 8 ��� ������
    *((signed char *)(&ADC)+1)>>=1; // ���� ���� � ������������� ������
    GODONE=1;

// ������ ���������� ������� �� 1���

    b=ADC-e;
    b>>=1;
    b>>=1;
    b>>=1;
    b>>=1;  // ����� �� 16 ��� ����������
            // �������� ������ 170��
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

    e+=temp1;       // *45/32 = 1.40625 ����� 2*cos(Pi*2*1���/8���)=1.4142135
                    // ��� 1007 �� ��� 8��� �������� ���
    e-=c;
    c=y+b;

// ����� ���������� ������� 1���

// ������  ��������� ���������
    if(!--cntVal) {
        level=0;

    }

    if(y>=0)
    if(level<=*((char*)(&y)+1)){ // �������� ������� ������ ��� ����� ��������
        level=(char)(y>>8);    // �������� � ���������� ����� ���� ��������
        cntVal=8;
    }

// ����� ��������� ���������
// ������ ����� ������ ������ � ������������
    if(!--dvdrTime){
        dvdrTime|=8;
        msTime=1;

        btemp=level>>1;
        if(ust<btemp) {
            // ������� �������

			if(!MODE) LED=1;
			state=1;
            ust=level;
            mjN++;
            mjN&=0xFE;// ��������� ������ ����� �������� 0->1
        }
        else {
            btemp=ust>>1;
            if(btemp>=level){
                // ������ �������

				if(!MODE) LED=0;
				state=0;
                ust=btemp;
                ust++;
                mjN|=1;
            }
        }
    }
  }
//����� ����� ������ ������ � ������������

}
