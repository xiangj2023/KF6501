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



		
		
		
//------------------系数校正用变量---------------------------------------------

		TRelElecValPar		U1;
		TRelElecValPar		U2;
		TRelElecValPar		U3;
		TRelElecValPar		U4;
		TRelElecValPar		IT1;
		TRelElecValPar		IF1;
		TRelElecValPar		IT2;
		TRelElecValPar		IF2;
		TRelElecValPar		IT3;
		TRelElecValPar		IF3;
		TRelElecValPar		IAT1;
		TRelElecValPar		IAT2;

		LONG				midU1;
		LONG				midU2;
		LONG				midU3;
		LONG				midU4;
		LONG				midIT1;
		LONG				midIF1;
		LONG				midIT2;
		LONG				midIF2;
		LONG				midIT3;
		LONG				midIF3;
		LONG				midIAT1;
		LONG				midIAT2;
		
		LONG				m_pAdjustData[12];
		
		LONG				AdjustU1[NUMOFYCVALUE];
		LONG				AdjustU2[NUMOFYCVALUE];
		LONG				AdjustU3[NUMOFYCVALUE];
		LONG				AdjustU4[NUMOFYCVALUE];
		LONG				AdjustIT1[NUMOFYCVALUE];
		LONG				AdjustIF1[NUMOFYCVALUE];
		LONG				AdjustIT2[NUMOFYCVALUE];
		LONG				AdjustIF2[NUMOFYCVALUE];
		LONG				AdjustIT3[NUMOFYCVALUE];
		LONG				AdjustIF3[NUMOFYCVALUE];
		LONG				AdjustIAT1[NUMOFYCVALUE];
		LONG				AdjustIAT2[NUMOFYCVALUE];
		


	// 1.标志定义
		DWORD			m_CoefAdjustDataMask;		//通道掩码
		LONG			*m_pCoefAdjustBuf;			//系数校正采样数据缓冲区
		DWORD			m_dwSourceU;				//系数校正电压
		DWORD			m_dwSourceI;				//系数校正电流
		DWORD			m_dwAdjustCnt;				//系数校正计算次数		
		DWORD			m_dwWaitCnt;				//系数校正后等待的次数
		BOOL			m_bIsCoefAdjust;			//开始系数校正标志		
		BOOL			m_bIsSourceOK;				//源校正标志
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据

		DWORD      dwMsgID;

		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
		
		LONG CoefAdjustSort(LONG Val,LONG* OriginalData);
		void WriteMSComposeBuf(LONG *ComposeBuf,LONG*DataBuf);
		void CoefAdjust(void);//系数校正		
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
};
void DITask(DWORD *pdwApp);

#endif
