#ifndef _MAX11046_H_
#define _MAX11046_H_

#include "resm5234.h"
#include "system.h"
#include "sample.h"
#include "relay.h"

//定义AD启动采样控制引脚
#define AD1_START_PIN	0x10	//DT2_OUT
#define AD2_START_PIN	0x20	//DT2_IN
#define AD_RESUME_VAL 32

#define AD7606_CHAN_NUM	16		//定义7606通道个数,两片

//定义启动采样控制，用上升沿启动采样
#define ADStartPinHigh()	(MCF_GPIO_PODR_TIMER |= 0x30)
#define ADStartPinLow()		(MCF_GPIO_PODR_TIMER &= (~0x30))
//#define AD2StartPinHigh()	(MCF_GPIO_PODR_TIMER |= AD2_START_PIN)
//#define AD2StartPinLow()	(MCF_GPIO_PODR_TIMER &= (~AD2_START_PIN))

#define ADStartSamp()	\
{\
	ADStartPinLow();\
	ADStartPinHigh();\
}

void ADCInit();
void ADSampleInt(void * arg);

STATUS ReadSampleData(LONG *pData, DWORD ChanMask, BYTE CycleNo);
STATUS ReadPreSingleData(LONG *pData, BYTE *pChannel, BYTE Number,BYTE Interval);

STATUS ReadHisSampData(DWORD dwPtr, DWORD dwMask, SHORT *pBuf, DWORD wPointNum);
BOOL isSampDataLenFill(DWORD dwStartPtr, DWORD wPointNum);
STATUS WriteComposeData(LONG *pBuf, DWORD dwMask, BOOL isDatSampChan=FALSE);
STATUS ReadParticularTimeData(LONG *pData, TAbsTime DataTime);
STATUS ReadHisSampData_Single(DWORD dwPtr, DWORD dwChanNo, SHORT *pBuf, DWORD dwPointNum);

void CheckADStatus();
DWORD BackSamplePtr(DWORD dwStartPtr, DWORD wPointNum);


#endif
