/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			led.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ledָʾ�ƿ���                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      led����                          
*                                                                                  
* ����                                                                             
*                                                                                  
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/12/19    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "led.h"

//ע��LED��ֻ��д�����ܶ��������Ҫ��¼�˿�״̬��������Ҫ���浽NVRAM��,�����ڶ�������е���
//��ü��ź�������,��λϨ����λ����

BYTE  bLedStatus[LED_PORT_NUM];
BOOL bIsledCheck = FALSE;

//��ʱÿ�γ�ʼ��ʱ�ر����е�
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

//���ݱ���ĵ�ǰ״̬��ˢ���߼�����ָʾ��
void UpdateLogicLed()
{
	for(int i=0; i<3; i++)
		outportb(LED1_CS_BASE+i, bLedStatus[i]);
		
}
