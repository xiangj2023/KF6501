#ifndef _M5234TIMER_H_
#define _M5234TIMER_H_

//#include "sysdefs.h"
//#include "mcf523x.h"
//#include "resm5234.h"
#include "system.h"

//定义DMA定时器相关寄存器数据结构
struct TCFTimer{
	volatile WORD 	*tmr;		/* timer mode register */
	volatile BYTE	*ter;		/* timer event register */
	volatile DWORD 	*tcn;		/* timer counter register */
	volatile DWORD 	*tcr;		/* timer capture register */
	volatile DWORD 	*trr;		/* timer reference register */
	volatile DWORD	*imr;		/* interrupt mask register */

	DWORD		imrMask;	/* mask bit in IMR register */
};

//DMA定时器使用分配
#define DI_DMA_TIMER	1
#define RELAY_MS_TIMER	2	//保护毫秒用定时器
#define AD_START_TIMER	3	//AD起动用定时器

//定时器定时间隔定义,每秒钟中断次数
#define DI_INT_TIMER_RATE	(1000000/500)

//定义定时器时间间隔，单位微妙
#define RELAY_MS_RATE		(1000/2)	//定时1毫秒
#define AD_TIME_RATE		(625)	//AD起动定时间隔625微妙

//定义定时器引脚控制关系
#define DTO0_YKEN2		0x01
#define DTI0_YKEN4		0x02
#define DTO1_YKEN1		0x04
#define DTI1_YKEN3		0x08

#define SET_DMA_IMR_BIT(pTimer) \
		{ *(volatile DWORD *) (pTimer)->imr |= (pTimer)->imrMask; }

#define CLR_DMA_IMR_BIT(pTimer) \
		{ *(volatile DWORD *) (pTimer)->imr &= ~(pTimer)->imrMask; }

/*************************************************************************************
* DI定时器初始化                                                          
*************************************************************************************/
void DITimerInit();

/*************************************************************************************
* DI定时器允许                                                          
*************************************************************************************/
void DITimerEnable();

/*************************************************************************************
* DI定时器禁止                                                          
*************************************************************************************/
void DITimerDisable();

void RelayTimeInit();
void RelayTimeInt(void * arg);

void ADTimeInit(void);
void ADTimeInt(void * arg);


void TimeSetDTRR(BYTE no, DWORD interval);

#endif
