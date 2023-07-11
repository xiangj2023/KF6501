#ifndef _LED_H_
#define _LED_H_

#include "resm5234.h"
#include "system.h"

//��ʱ����ָʾ��
#define RUN_LED		26//26--�̣�27--��
#define ALARM_LED	25//24--��, 25--��
#define ACTION_LED	26

#define CHECK_LED_NUM	24	//�Լ�Ƹ���,0~23

#define LED_PORT_NUM	4	//LEDռ�ö˿ڸ���

/*
#define LedAllOn()	\
{\
	outportb(LED1_CS_BASE,0xff);\
	outportb(LED2_CS_BASE,0xff);\
	outportb(LED3_CS_BASE,0xff);\
	outportb(LED4_CS_BASE,0xff);\
}

#define LedAllOff()	\
{\
	outportb(LED1_CS_BASE,0x00);\
	outportb(LED2_CS_BASE,0x00);\
	outportb(LED3_CS_BASE,0x00);\
	outportb(LED4_CS_BASE,0x00);\
}
*/
void LedInit();
void LEDOnOff(WORD wNo, BOOL bIsOn);
void LedAllOn();
void LedAllOff();
void TestLed(BYTE number);
void UpdateLogicLed();


#endif
