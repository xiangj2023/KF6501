/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			led.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           led指示灯控制                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      led控制                          
*                                                                                  
* 函数                                                                             
*                                                                                  
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/12/19    初始创建                         
*                                                                                  
********************************************************************************************/

#include "led.h"

//注意LED口只能写，不能读，因此需要记录端口状态，并且需要保存到NVRAM中,由于在多个任务中调用
//最好加信号量保护,置位熄灭，清位点亮

BYTE  bLedStatus[LED_PORT_NUM];
BOOL bIsledCheck = FALSE;

//临时每次初始化时关闭所有灯
void LedInit()
{
	for(int i=0; i<LED_PORT_NUM; i++)
		bLedStatus[i] = 0x00;
	LedAllOff();
}

void LEDOnOff(WORD wNo, BOOL bIsOn)
{
	BYTE offset;

	if(bIsledCheck == TRUE)
		return;
	
	offset = wNo/8;
	if(bIsOn == TRUE)
		bLedStatus[offset] |= 0x01<<(wNo%8);
	else
		bLedStatus[offset] &= ~(0x01<<(wNo%8));
		
	outportb(LED1_CS_BASE+offset, bLedStatus[offset]);
}


void LedAllOn()
{
	outportb(LED1_CS_BASE,0xff);
	outportb(LED2_CS_BASE,0xff);
	outportb(LED3_CS_BASE,0xff);
	outportb(LED4_CS_BASE,0xff);
}

void LedAllOff()	
{
	outportb(LED1_CS_BASE,0x00);
	outportb(LED2_CS_BASE,0x00);
	outportb(LED3_CS_BASE,0x00);
	outportb(LED4_CS_BASE,0x00);
}

void TestLed(BYTE number)
{
	LedAllOff();

	outportb(LED1_CS_BASE+number/8,(0x01<<(number%8)));
	
}

//根据保存的当前状态，刷新逻辑定义指示灯
void UpdateLogicLed()
{
	for(int i=0; i<3; i++)
		outportb(LED1_CS_BASE+i, bLedStatus[i]);
		
}
