#ifndef _EPTU_APP_H_
#define _EPTU_APP_H_

#include "system.h"

#include "etpu_util.h"
#include "etpu_set1.h"
#include "etpu_pwm.h"
#include "etpu_gpio.h"
#include "etpu_ic.h"

//定义频率测量计数上下限值,包括保护及测量装置	9375000/频率
#define M_FREQ_UPPER_LIMIT	210700		//44.5Hz
#define M_FREQ_LOWER_LIMIT	168900		//55.5Hz
#define P_FREQ_UPPER_LIMIT	190000		//49.5Hz
#define P_FREQ_LOWER_LIMIT	185500		//50.5Hz

//定义B格式信息位负脉冲类型
#define IRIG_B_ERR		0
#define IRIG_B_8MS		1
#define IRIG_B_5MS		2
#define IRIG_B_2MS		3

//定义各种脉冲的计数范围，以(37.5/4)MHz为时钟基准
#define PULSE_OFFSET	1000
#define PULSE_2MS_H		(18750+PULSE_OFFSET)	// 2毫秒计数上限 18750+400
#define PULSE_2MS_L		(18750-PULSE_OFFSET)	// 2毫秒计数下限 18750-400
#define PULSE_5MS_H		(46875+PULSE_OFFSET)	// 5毫秒计数上限 46875+400
#define PULSE_5MS_L		(46875-PULSE_OFFSET)	// 5毫秒计数下限 46875-400
#define PULSE_8MS_H		(75000+PULSE_OFFSET)	// 8毫秒计数上限 75000+400
#define PULSE_8MS_L		(75000-PULSE_OFFSET)	// 8毫秒计数下限 75000-400

//定义B格式脉冲中信息字的个数和信息位的个数
#define IRIG_B_WORD_NUM	10
#define IRIG_B_BIT_NUM	9

//定义B格式脉冲信息字的内容
#define IRIG_B_SEC		0
#define IRIG_B_MIN		1
#define IRIG_B_HOUR		2
#define IRIG_B_DAY		3
#define IRIG_B_H_DAY	4
#define IRIG_B_YEAR		5
#define IRIG_B_SAT		6
#define IRIG_B_QT		7
#define IRIG_B_SBS1		8
#define IRIG_B_SBS2		9

#define FREQ_DATA_BUF	9

//KF3500系统B格式部分定义
#define IRIG_35_PLANET	5
#define IRIG_35_YEAR	6	
//定义两种时钟格式
//#define		IRIG_KF6500	((DWORD)(0x01<<8))		//故障电量按照二次数据存储
//#define		DB_RELAY_CACLINT	((DWORD)(0x01<<9))		//是否计算积分电度

#define IRIG_KF6500 FALSE 
#define IRIG_DK3500 TRUE
struct TIrigBInt
{
	BOOL isInResolve;		//当前是否处于B格式解析状态，即是否找到了头
	BYTE bCurPulseType;		//当前脉冲类型
	BYTE bOldPulseType;		//上次脉冲类型
	BYTE bCurPulseWordNum;	//当前处于的信息字数
	BYTE bCurPulseBitNum;	//当前处于的信息字中的位数
	WORD bIrigWordInf[IRIG_B_WORD_NUM];
	TAbsTime OldAbsTime;	//上一秒解析出的系统时间
	BOOL isAbsTimeOk;		//当前系统时间是否有效(经过+1秒处理)
};

//频率测量
struct TFreqData
{
	DWORD dwOldCounter;
	DWORD dwPeriod;		//单位微秒
	DWORD dwErrCnt;
	DWORD dwIntCnt;
	DWORD dwUpLimit;
	DWORD dwLowLimit;
	DWORD dwPulseLen[FREQ_DATA_BUF];
	DWORD dwFreq;
};

//根据交流采样输入频率值，调整启动AD转换采样的PWM的频率值,占空比固定
#define ADStartFreqUpdate(freq)		fs_etpu_pwm_update(ETPU_AD_START, freq, 500, 9375000)
#define AD1StartFreqUpdate(freq)	fs_etpu_pwm_update(ETPU_AD1_START, freq, 500, 9375000)

/*第一路频率测量中断处理，本通道用于控制AD采样的频率*/
void EtpuFreq1Int(void *arg);

/*第二路频率测量中断处理，本通道用于同期控制*/
void EtpuFreq2Int(void *arg);

/*ETPU测频处理函数，通过GPIO输入功能实现，中断方式捕获任意边沿*/
BOOL EtpuFreqProc(BYTE channel);

/*定时频率测量通道监视*/
void FreqChanMonitor();

/*根据通道号取测量到的频率值，放大100倍*/
DWORD GetFreqValue(BYTE channel);

DWORD GetMidFreqValue(BYTE channel);

/*ETPU系统电源偏低监视中断函数，采用GPIO的输入捕捉方式，监视下降沿,任意沿中断*/
void EtpuPfoInt(void *arg);

/*ETPU测温处理函数，采用GPIO的输入捕捉方式，捕捉任意边沿*/
void EtpuTempProc(void *arg);

/*B格式时钟解析中断处理，采用GPIO的输入功能，捕捉任意边沿并产生中断*/
void EtpuIrigInt(void * arg);

/*将解析出的B格式时钟信息转换为相对于1970年的毫秒数*/
void IrigBToAbsTime();

/*检查B格式信号是否正常*/
void isIrigOk();
/*读取时钟的格式*/


#endif
