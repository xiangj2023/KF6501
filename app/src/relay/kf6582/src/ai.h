#ifndef _AI_H_
#define _AI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbasedef.h"
#include "dbfunc.h"
#include "parafunc.h"
#include "db_err.h"
#include "Relay_defs.h"



//定义AI定时器时间间隔为240毫秒，修改时需注意其可能会影响到测频,需保证如下关系:AI_TIMER_PERIOD/20 > FREQBUFSIZE
#define AI_TIMER_PERIOD			200	
//每次测量任务中需要计算的频率个数
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
//---------------测频用相关变量------------------------------

		SHORT *m_wFreqBuf;		
		LONG  *m_FreqCaluBuf;
		DWORD m_dwFreqMask;
		LONG m_dwFreq[NUM_FREQ_PER_AITASK];
		DWORD m_dwPreFreq;		
		BOOL 			m_FreqFlag;

//------------------遥测计算所用变量------------------------------------
		LONG			*m_pDataBuf;	//测量数据缓冲区	
		DWORD			m_AIDataMask;	//测量通道用的数据掩码
		
		LONG			m_U1;
		LONG			m_U2;
		LONG			m_U3;
		LONG			m_U4;
		LONG			m_U5;
		LONG			m_U6;
		LONG			m_U7;
		LONG			m_U8;
		LONG			m_I1;
		LONG			m_I2;
		LONG			m_I3;
		LONG			m_I4;
		LONG			m_I5;
		LONG			m_I6;
		LONG			m_I7;
		LONG			m_I8;
		
		LONG 			UM1Array[NUMOFYCVALUE];
		LONG 			UM2Array[NUMOFYCVALUE];
		LONG 			UM3Array[NUMOFYCVALUE];
		LONG 			UM4Array[NUMOFYCVALUE];
		LONG 			UM5Array[NUMOFYCVALUE];
		LONG 			UM6Array[NUMOFYCVALUE];
		LONG 			UM7Array[NUMOFYCVALUE];
		LONG 			UM8Array[NUMOFYCVALUE];
		LONG			IM1Array[NUMOFYCVALUE];
		LONG			IM2Array[NUMOFYCVALUE];
		LONG			IM3Array[NUMOFYCVALUE];
		LONG			IM4Array[NUMOFYCVALUE];
		LONG			IM5Array[NUMOFYCVALUE];
		LONG			IM6Array[NUMOFYCVALUE];
		LONG			IM7Array[NUMOFYCVALUE];
		LONG			IM8Array[NUMOFYCVALUE];

//------------------系数校正用变量---------------------------------------------
		// 1.标志定义
		LONG			*m_pCoefAdjustBuf;			//系数校正采样数据缓冲区
		DWORD			m_dwSourceU;				//系数校正电压
		DWORD			m_dwSourceI;				//系数校正电流
		DWORD			m_dwAdjustCnt;				//系数校正计算次数		
		DWORD			m_dwWaitCnt;				//系数校正后等待的次数
		BOOL			m_bIsCoefAdjust;			//开始系数校正标志		
		BOOL			m_bIsDriftAdjust;			//死区值校正标志
		BOOL			m_bIsSourceOK;				//源校正标志
		// 2.死区值定义
		LONG			m_lDriftU1;
		LONG			m_lDriftU2;
		LONG			m_lDriftU3;
		LONG			m_lDriftU4;
		LONG			m_lDriftU5;
		LONG			m_lDriftU6;
		LONG			m_lDriftU7;
		LONG			m_lDriftU8;
		LONG			m_lDriftI1;
		LONG			m_lDriftI2;
		LONG			m_lDriftI3;
		LONG			m_lDriftI4;
		LONG			m_lDriftI5;
		LONG			m_lDriftI6;
		LONG			m_lDriftI7;
		LONG			m_lDriftI8;
		// 3.经过冒泡排序后的校正结果
		LONG			m_lMidU1p;
		LONG			m_lMidU2p;
		LONG			m_lMidU3p;
		LONG			m_lMidU4p;
		LONG			m_lMidU5p;
		LONG			m_lMidU6p;
		LONG			m_lMidU7p;
		LONG			m_lMidU8p;
		LONG			m_lMidI1p;
		LONG			m_lMidI2p;
		LONG			m_lMidI3p;
		LONG			m_lMidI4p;
		LONG			m_lMidI5p;
		LONG			m_lMidI6p;
		LONG			m_lMidI7p;
		LONG			m_lMidI8p;
		
		LONG			m_lMidU1m;
		LONG			m_lMidU2m;
		LONG			m_lMidU3m;
		LONG			m_lMidU4m;
		LONG			m_lMidU5m;
		LONG			m_lMidU6m;
		LONG			m_lMidU7m;
		LONG			m_lMidU8m;
		LONG			m_lMidI1m;
		LONG			m_lMidI2m;
		LONG			m_lMidI3m;
		LONG			m_lMidI4m;
		LONG			m_lMidI5m;
		LONG			m_lMidI6m;
		LONG			m_lMidI7m;
		LONG			m_lMidI8m;
		// 4.各电量待排序数组
		LONG			AArrayU1p[NUMOFYCVALUE];		
		LONG			AArrayU2p[NUMOFYCVALUE];
		LONG			AArrayU3p[NUMOFYCVALUE];
		LONG			AArrayU4p[NUMOFYCVALUE];
		LONG			AArrayU5p[NUMOFYCVALUE];
		LONG			AArrayU6p[NUMOFYCVALUE];
		LONG			AArrayU7p[NUMOFYCVALUE];
		LONG			AArrayU8p[NUMOFYCVALUE];
		LONG			AArrayI1p[NUMOFYCVALUE];
		LONG			AArrayI2p[NUMOFYCVALUE];		
		LONG			AArrayI3p[NUMOFYCVALUE];
		LONG			AArrayI4p[NUMOFYCVALUE];
		LONG			AArrayI5p[NUMOFYCVALUE];
		LONG			AArrayI6p[NUMOFYCVALUE];
		LONG			AArrayI7p[NUMOFYCVALUE];
		LONG			AArrayI8p[NUMOFYCVALUE];
		
		LONG			AArrayU1m[NUMOFYCVALUE];		
		LONG			AArrayU2m[NUMOFYCVALUE];
		LONG			AArrayU3m[NUMOFYCVALUE];
		LONG			AArrayU4m[NUMOFYCVALUE];
		LONG			AArrayU5m[NUMOFYCVALUE];
		LONG			AArrayU6m[NUMOFYCVALUE];
		LONG			AArrayU7m[NUMOFYCVALUE];
		LONG			AArrayU8m[NUMOFYCVALUE];
		LONG			AArrayI1m[NUMOFYCVALUE];
		LONG			AArrayI2m[NUMOFYCVALUE];		
		LONG			AArrayI3m[NUMOFYCVALUE];
		LONG			AArrayI4m[NUMOFYCVALUE];
		LONG			AArrayI5m[NUMOFYCVALUE];
		LONG			AArrayI6m[NUMOFYCVALUE];
		LONG			AArrayI7m[NUMOFYCVALUE];
		LONG			AArrayI8m[NUMOFYCVALUE];
		//未经过冒泡排序的计算结果		
		LONG			m_AdjustU1m;
		LONG			m_AdjustU2m;
		LONG			m_AdjustU3m;
		LONG			m_AdjustU4m;
		LONG			m_AdjustU5m;
		LONG			m_AdjustU6m;
		LONG			m_AdjustU7m;
		LONG			m_AdjustU8m;
		LONG			m_AdjustI1m;
		LONG			m_AdjustI2m;
		LONG			m_AdjustI3m;
		LONG			m_AdjustI4m;
		LONG			m_AdjustI5m;
		LONG			m_AdjustI6m;
		LONG			m_AdjustI7m;
		LONG			m_AdjustI8m;
		
		TRelElecValPar			m_AdjustU1p;
		TRelElecValPar			m_AdjustU2p;
		TRelElecValPar			m_AdjustU3p;
		TRelElecValPar			m_AdjustU4p;
		TRelElecValPar			m_AdjustU5p;
		TRelElecValPar			m_AdjustU6p;
		TRelElecValPar			m_AdjustU7p;
		TRelElecValPar			m_AdjustU8p;
		TRelElecValPar			m_AdjustI1p;
		TRelElecValPar			m_AdjustI2p;
		TRelElecValPar			m_AdjustI3p;
		TRelElecValPar			m_AdjustI4p;
		TRelElecValPar			m_AdjustI5p;
		TRelElecValPar			m_AdjustI6p;
		TRelElecValPar			m_AdjustI7p;
		TRelElecValPar			m_AdjustI8p;
		
		LONG			m_pAdjustData[32];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据

		
		DWORD      dwMsgID;
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
	//	void OnAutoAdjustStep1(DWORD arg);	
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
//		void RefreshCommRW();		
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
		
//		void WriteMSComposeBuf(LONG *ComposeBuf,LONG*DataBuf);
		void CoefAdjust(void);//系数校正		
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
};
void DITask(DWORD *pdwApp);

#endif
