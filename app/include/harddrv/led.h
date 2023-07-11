#ifndef _LED_H_
#define _LED_H_

#include "resm5234.h"
#include "system.h"

//临时定义指示灯
#define RUN_LED		26//26--绿，27--红
#define ALARM_LED	25//24--绿, 25--红
#define ACTION_LED	26

#define CHECK_LED_NUM	24	//自检灯个数,0~23

#define LED_PORT_NUM	4	//LED占用端口个数

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
