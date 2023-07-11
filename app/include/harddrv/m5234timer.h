#ifndef _M5234TIMER_H_
#define _M5234TIMER_H_

//#include "sysdefs.h"
//#include "mcf523x.h"
//#include "resm5234.h"
#include "system.h"

//����DMA��ʱ����ؼĴ������ݽṹ
struct TCFTimer{
	volatile WORD 	*tmr;		/* timer mode register */
	volatile BYTE	*ter;		/* timer event register */
	volatile DWORD 	*tcn;		/* timer counter register */
	volatile DWORD 	*tcr;		/* timer capture register */
	volatile DWORD 	*trr;		/* timer reference register */
	volatile DWORD	*imr;		/* interrupt mask register */

	DWORD		imrMask;	/* mask bit in IMR register */
};

//DMA��ʱ��ʹ�÷���
#define DI_DMA_TIMER	1
#define RELAY_MS_TIMER	2	//���������ö�ʱ��
#define AD_START_TIMER	3	//AD���ö�ʱ��

//��ʱ����ʱ�������,ÿ�����жϴ���
#define DI_INT_TIMER_RATE	(1000000/500)

//���嶨ʱ��ʱ��������λ΢��
#define RELAY_MS_RATE		(1000/2)	//��ʱ1����
#define AD_TIME_RATE		(625)	//AD�𶯶�ʱ���625΢��

//���嶨ʱ�����ſ��ƹ�ϵ
#define DTO0_YKEN2		0x01
#define DTI0_YKEN4		0x02
#define DTO1_YKEN1		0x04
#define DTI1_YKEN3		0x08

#define SET_DMA_IMR_BIT(pTimer) \
		{ *(volatile DWORD *) (pTimer)->imr |= (pTimer)->imrMask; }

#define CLR_DMA_IMR_BIT(pTimer) \
		{ *(volatile DWORD *) (pTimer)->imr &= ~(pTimer)->imrMask; }

/*************************************************************************************
* DI��ʱ����ʼ��                                                          
*************************************************************************************/
void DITimerInit();

/*************************************************************************************
* DI��ʱ������                                                          
*************************************************************************************/
void DITimerEnable();

/*************************************************************************************
* DI��ʱ����ֹ                                                          
*************************************************************************************/
void DITimerDisable();

void RelayTimeInit();
void RelayTimeInt(void * arg);

void ADTimeInit(void);
void ADTimeInt(void * arg);


void TimeSetDTRR(BYTE no, DWORD interval);

#endif
