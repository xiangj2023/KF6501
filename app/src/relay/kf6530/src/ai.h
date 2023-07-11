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
		LONG			*m_pDataBuf;
		LONG			*m_pCoefAdjustBuf;									//系数校正数据缓冲区		
		DWORD			m_CoefAdjustMask;									//系数校正用的通道数据掩码
		DWORD			m_dwAdjustCnt;					//系数校正计算次数
		BOOL			m_bIsCoefAdjust;				//开始系数校正标志
		BOOL			m_bIsHaveAdjustSource;			//已经源校正标志
		DWORD			m_dwWaitCnt;					//系数校正开始等待次数


//        SHORT           *dwTestFBuf;		  //测频缓冲区
//        DWORD           dwTestFDataMask;	  //测频数据掩码
//        DWORD           m_dwFpre; 			  //前次测频
//        DWORD           m_dwFnow;              //本次测频值
//		DWORD           m_dwTs;                //本次测频采样周期

		LONG	Ua;
		LONG	Ub;
		LONG	Uc;
		LONG	IA;
		LONG	IB;
		LONG	IC;
		LONG	I1;
		LONG	I2;
		LONG	I3;
		LONG	I4;

		
		LONG	UaArray[NUMOFYCVALUE];
		LONG	UbArray[NUMOFYCVALUE];
		LONG	UcArray[NUMOFYCVALUE];
		LONG	IAArray[NUMOFYCVALUE];
		LONG	IBArray[NUMOFYCVALUE];
		LONG	ICArray[NUMOFYCVALUE];
		LONG	I1Array[NUMOFYCVALUE];
		LONG	I2Array[NUMOFYCVALUE];
		LONG	I3Array[NUMOFYCVALUE];
		LONG	I4Array[NUMOFYCVALUE];


//---------------用于系数校正的变量定义-----------------
		LONG			m_dwSourceU;		//系数校正源电压
		LONG			m_dwSourceI;		//系数校正源电流

		TRelElecValPar		m_AdjustUA;
		TRelElecValPar		m_AdjustUB;
		TRelElecValPar		m_AdjustUC;
		TRelElecValPar		m_AdjustIHA;
		TRelElecValPar		m_AdjustIHB;
		TRelElecValPar		m_AdjustIHC;
		TRelElecValPar		m_AdjustIL1;
		TRelElecValPar		m_AdjustIL2;
		TRelElecValPar		m_AdjustIL3;
		TRelElecValPar		m_AdjustIL4;

		LONG MidUA;
		LONG MidUB;
		LONG MidUC;
		LONG MidIHA;
		LONG MidIHB;
		LONG MidIHC;
		LONG MidIL1;
		LONG MidIL2;
		LONG MidIL3;
		LONG MidIL4;

		BOOL m_bIsDriftAdjust;
		LONG	UaDrift;
		LONG	UbDrift;
		LONG	UcDrift;
		LONG	IADrift;
		LONG	IBDrift;
		LONG	ICDrift;
		LONG	I1Drift;
		LONG	I2Drift;
		LONG	I3Drift;
		LONG	I4Drift;

		LONG	UaM;
		LONG	UbM;
		LONG	UcM;
		LONG	IAM;
		LONG	IBM;
		LONG	ICM;
		LONG	I1M;
		LONG	I2M;
		LONG	I3M;
		LONG	I4M;

		LONG	MidUaM;
		LONG	MidUbM;
		LONG	MidUcM;
		LONG	MidIAM;
		LONG	MidIBM;
		LONG	MidICM;
		LONG	MidI1M;
		LONG	MidI2M;
		LONG	MidI3M;
		LONG	MidI4M;


		
//---------------用于排序的数组定义------------------------


		//系数校正电量排序数组
		LONG			AdjustUA[NUMOFYCVALUE];
		LONG			AdjustUB[NUMOFYCVALUE];
		LONG			AdjustUC[NUMOFYCVALUE];
		LONG			AdjustIHA[NUMOFYCVALUE];
		LONG			AdjustIHB[NUMOFYCVALUE];
		LONG			AdjustIHC[NUMOFYCVALUE];		
		LONG			AdjustIL1[NUMOFYCVALUE];
		LONG			AdjustIL2[NUMOFYCVALUE];
		LONG			AdjustIL3[NUMOFYCVALUE];
		LONG			AdjustIL4[NUMOFYCVALUE];

		LONG			AdjustUaM[NUMOFYCVALUE];
		LONG			AdjustUbM[NUMOFYCVALUE];
		LONG			AdjustUcM[NUMOFYCVALUE];
		LONG			AdjustIAM[NUMOFYCVALUE];
		LONG			AdjustIBM[NUMOFYCVALUE];
		LONG			AdjustICM[NUMOFYCVALUE];
		LONG			AdjustI1M[NUMOFYCVALUE];
		LONG			AdjustI2M[NUMOFYCVALUE];
		LONG			AdjustI3M[NUMOFYCVALUE];
		LONG			AdjustI4M[NUMOFYCVALUE];
		
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[30];		//用于保存系数的累加值
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
