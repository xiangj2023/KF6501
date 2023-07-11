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
		LONG			*m_pCoefAdjustBuf;									//系数校正数据缓冲区
		LONG			*m_pDataBuf;										//测量数据缓冲区
		LONG			*m_pComposeBuf;										//低侧合成电流缓冲区
		DWORD			m_CoefAdjustMask;									//系数校正用的通道数据掩码
		DWORD			m_dwAdjustCnt;					//系数校正计算次数
		BOOL			m_bIsCoefAdjust;				//开始系数校正标志
		BOOL			m_bIsHaveAdjustSource;			//已经源校正标志
		DWORD			m_dwWaitCnt;					//系数校正开始等待次数

		LONG			Uha;
		LONG			Uhb;
		LONG			Uhc;
		LONG			U0;
		LONG			Ula;
		LONG			Ulb;
		LONG			Iha;
		LONG			Ihb;
		LONG			Ihc;
		LONG			I0;
		LONG			I1;
		LONG			I2;
		LONG			I3;
		LONG			I4;
		LONG			Ijx;
		LONG			Ila;
		LONG			Ilb;

//---------------用于系数校正的变量定义-----------------
		LONG			m_dwSourceU;		//系数校正源电压
		LONG			m_dwSourceI;		//系数校正源电流
		
		TRelElecValPar		m_AdjustUHA;
		TRelElecValPar		m_AdjustUHB;
		TRelElecValPar		m_AdjustUHC;
		TRelElecValPar		m_AdjustU0;
		TRelElecValPar		m_AdjustULA;
		TRelElecValPar		m_AdjustULB;
		TRelElecValPar		m_AdjustIHA;		
		TRelElecValPar		m_AdjustIHB;
		TRelElecValPar		m_AdjustIHC;
		TRelElecValPar		m_AdjustI0;
		TRelElecValPar		m_AdjustIL1;
		TRelElecValPar		m_AdjustIL2;
		TRelElecValPar		m_AdjustIL3;
		TRelElecValPar		m_AdjustIL4;


		LONG MidUHA;
		LONG MidUHB;
		LONG MidUHC;
		LONG MidU0;
		LONG MidULA;
		LONG MidULB;
		LONG MidIHA;
		LONG MidIHB;
		LONG MidIHC;
		LONG MidI0;
		LONG MidIL1;
		LONG MidIL2;
		LONG MidIL3;
		LONG MidIL4;
		
		BOOL 	m_bIsDriftAdjust;
		LONG	UHADrift;
		LONG	UHBDrift;
		LONG	UHCDrift;
		LONG	U0Drift;
		LONG	ULADrift;
		LONG	ULBDrift;
		LONG	IHADrift;
		LONG	IHBDrift;
		LONG	IHCDrift;
		LONG	I0Drift;
		LONG	IL1Drift;
		LONG	IL2Drift;
		LONG	IL3Drift;
		LONG	IL4Drift;

		LONG 	MidUHAM;
		LONG 	MidUHBM;
		LONG 	MidUHCM;
		LONG 	MidU0M;
		LONG 	MidULAM;
		LONG 	MidULBM;
		LONG 	MidIHAM;
		LONG 	MidIHBM;
		LONG 	MidIHCM;
		LONG 	MidI0M;
		LONG 	MidIL1M;
		LONG 	MidIL2M;
		LONG 	MidIL3M;
		LONG 	MidIL4M;
		
		LONG 	m_adjUHAM;
		LONG 	m_adjUHBM;
		LONG 	m_adjUHCM;
		LONG 	m_adjU0M;
		LONG 	m_adjULAM;
		LONG 	m_adjULBM;
		LONG 	m_adjIHAM;
		LONG 	m_adjIHBM;
		LONG 	m_adjIHCM;
		LONG 	m_adjI0M;
		LONG 	m_adjIL1M;
		LONG 	m_adjIL2M;
		LONG 	m_adjIL3M;
		LONG 	m_adjIL4M;
		
//---------------用于排序的数组定义------------------------


		//系数校正电量排序数组
		LONG			AdjustUHA[NUMOFYCVALUE];
		LONG			AdjustUHB[NUMOFYCVALUE];
		LONG			AdjustUHC[NUMOFYCVALUE];
		LONG			AdjustU0[NUMOFYCVALUE];
		LONG			AdjustULA[NUMOFYCVALUE];
		LONG			AdjustULB[NUMOFYCVALUE];
		LONG			AdjustIHA[NUMOFYCVALUE];
		LONG			AdjustIHB[NUMOFYCVALUE];
		LONG			AdjustIHC[NUMOFYCVALUE];
		LONG			AdjustI0[NUMOFYCVALUE];
		LONG			AdjustIL1[NUMOFYCVALUE];
		LONG			AdjustIL2[NUMOFYCVALUE];
		LONG			AdjustIL3[NUMOFYCVALUE];
		LONG			AdjustIL4[NUMOFYCVALUE];

		
		LONG			AdjustUHAM[NUMOFYCVALUE];
		LONG			AdjustUHBM[NUMOFYCVALUE];
		LONG			AdjustUHCM[NUMOFYCVALUE];
		LONG			AdjustU0M[NUMOFYCVALUE];
		LONG			AdjustULAM[NUMOFYCVALUE];
		LONG			AdjustULBM[NUMOFYCVALUE];
		LONG			AdjustIHAM[NUMOFYCVALUE];
		LONG			AdjustIHBM[NUMOFYCVALUE];
		LONG			AdjustIHCM[NUMOFYCVALUE];
		LONG			AdjustI0M[NUMOFYCVALUE];
		LONG			AdjustIL1M[NUMOFYCVALUE];
		LONG			AdjustIL2M[NUMOFYCVALUE];
		LONG			AdjustIL3M[NUMOFYCVALUE];
		LONG			AdjustIL4M[NUMOFYCVALUE];


		//正常计算中用于遥测值的排序数组
		
		LONG			ArrayUHAM[NUMOFYCVALUE];
		LONG			ArrayUHBM[NUMOFYCVALUE];
		LONG			ArrayUHCM[NUMOFYCVALUE];
		LONG			ArrayU0M[NUMOFYCVALUE];
		LONG			ArrayULAM[NUMOFYCVALUE];
		LONG			ArrayULBM[NUMOFYCVALUE];
		LONG			ArrayIHAM[NUMOFYCVALUE];
		LONG			ArrayIHBM[NUMOFYCVALUE];
		LONG			ArrayIHCM[NUMOFYCVALUE];
		LONG			ArrayI0M[NUMOFYCVALUE];
		LONG			ArrayIL1M[NUMOFYCVALUE];
		LONG			ArrayIL2M[NUMOFYCVALUE];
		LONG			ArrayIL3M[NUMOFYCVALUE];
		LONG			ArrayIL4M[NUMOFYCVALUE];
		LONG			ArrayILAM[NUMOFYCVALUE];
		LONG			ArrayILBM[NUMOFYCVALUE];
		LONG			ArrayIJXM[NUMOFYCVALUE];
//---------------------------------------------------------		
		LONG			m_pAdjustData[42];		//用于保存系数的累加值
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据



		DWORD      dwMsgID;
		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){};
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
		
		void CoefAdjust(void);//系数校正
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG CoefAdjustSort(LONG Val,LONG* OriginalData);		
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);

};
void DITask(DWORD *pdwApp);

#endif
