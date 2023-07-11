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
		LONG	U0;
		LONG	IHA;
		LONG	IHB;
		LONG	IHC;
		LONG	IMA;
		LONG	IMB;
		LONG	IMC;
		LONG	ILA;
		LONG	ILB;
		LONG	ILC;
		LONG	I0;
		LONG	IJX;

		
		LONG	UaArray[NUMOFYCVALUE];
		LONG	UbArray[NUMOFYCVALUE];
		LONG	UcArray[NUMOFYCVALUE];
		LONG	U0Array[NUMOFYCVALUE];
		LONG	IHAArray[NUMOFYCVALUE];
		LONG	IHBArray[NUMOFYCVALUE];
		LONG	IHCArray[NUMOFYCVALUE];
		LONG	IMAArray[NUMOFYCVALUE];
		LONG	IMBArray[NUMOFYCVALUE];
		LONG	IMCArray[NUMOFYCVALUE];
		LONG	ILAArray[NUMOFYCVALUE];
		LONG	ILBArray[NUMOFYCVALUE];
		LONG	ILCArray[NUMOFYCVALUE];
		LONG	I0Array[NUMOFYCVALUE];
		LONG	IJXArray[NUMOFYCVALUE];


//---------------用于系数校正的变量定义-----------------
		LONG			m_dwSourceU;		//系数校正源电压
		LONG			m_dwSourceI;		//系数校正源电流

		TRelElecValPar		m_AdjustUA;
		TRelElecValPar		m_AdjustUB;
		TRelElecValPar		m_AdjustUC;
		TRelElecValPar		m_AdjustU0;
		TRelElecValPar		m_AdjustIHA;
		TRelElecValPar		m_AdjustIHB;
		TRelElecValPar		m_AdjustIHC;
		TRelElecValPar		m_AdjustIMA;
		TRelElecValPar		m_AdjustIMB;
		TRelElecValPar		m_AdjustIMC;
		TRelElecValPar		m_AdjustILA;
		TRelElecValPar		m_AdjustILB;
		TRelElecValPar		m_AdjustILC;
		TRelElecValPar		m_AdjustI0;
		TRelElecValPar		m_AdjustIJX;

		LONG MidUA;
		LONG MidUB;
		LONG MidUC;
		LONG MidU0;
		LONG MidIHA;
		LONG MidIHB;
		LONG MidIHC;
		LONG MidIMA;
		LONG MidIMB;
		LONG MidIMC;
		LONG MidILA;
		LONG MidILB;
		LONG MidILC;
		LONG MidI0;
		LONG MidIJX;

		BOOL m_bIsDriftAdjust;
		LONG	UaDrift;
		LONG	UbDrift;
		LONG	UcDrift;
		LONG	U0Drift;
		LONG	IHADrift;
		LONG	IHBDrift;
		LONG	IHCDrift;
		LONG	IMADrift;
		LONG	IMBDrift;
		LONG	IMCDrift;
		LONG	ILADrift;
		LONG	ILBDrift;
		LONG	ILCDrift;
		LONG	I0Drift;
		LONG	IJXDrift;

		LONG	UaM;
		LONG	UbM;
		LONG	UcM;
		LONG	U0M;
		LONG	IHAM;
		LONG	IHBM;
		LONG	IHCM;
		LONG	IMAM;
		LONG	IMBM;
		LONG	IMCM;
		LONG	ILAM;
		LONG	ILBM;
		LONG	ILCM;
		LONG	I0M;
		LONG	IJXM;

		LONG	MidUaM;
		LONG	MidUbM;
		LONG	MidUcM;
		LONG	MidU0M;
		LONG	MidIHAM;
		LONG	MidIHBM;
		LONG	MidIHCM;
		LONG	MidIMAM;
		LONG	MidIMBM;
		LONG	MidIMCM;
		LONG	MidILAM;
		LONG	MidILBM;
		LONG	MidILCM;
		LONG	MidI0M;
		LONG	MidIJXM;


		
//---------------用于排序的数组定义------------------------


		//系数校正电量排序数组
		LONG			AdjustUA[NUMOFYCVALUE];
		LONG			AdjustUB[NUMOFYCVALUE];
		LONG			AdjustUC[NUMOFYCVALUE];
		LONG			AdjustU0[NUMOFYCVALUE];
		LONG			AdjustIHA[NUMOFYCVALUE];
		LONG			AdjustIHB[NUMOFYCVALUE];
		LONG			AdjustIHC[NUMOFYCVALUE];		
		LONG			AdjustIMA[NUMOFYCVALUE];
		LONG			AdjustIMB[NUMOFYCVALUE];
		LONG			AdjustIMC[NUMOFYCVALUE];
		LONG			AdjustILA[NUMOFYCVALUE];
		LONG			AdjustILB[NUMOFYCVALUE];
		LONG			AdjustILC[NUMOFYCVALUE];
		LONG			AdjustI0[NUMOFYCVALUE];
		LONG			AdjustIJX[NUMOFYCVALUE];

		LONG			AdjustUaM[NUMOFYCVALUE];
		LONG			AdjustUbM[NUMOFYCVALUE];
		LONG			AdjustUcM[NUMOFYCVALUE];
		LONG			AdjustU0M[NUMOFYCVALUE];
		LONG			AdjustIHAM[NUMOFYCVALUE];
		LONG			AdjustIHBM[NUMOFYCVALUE];
		LONG			AdjustIHCM[NUMOFYCVALUE];
		LONG			AdjustIMAM[NUMOFYCVALUE];
		LONG			AdjustIMBM[NUMOFYCVALUE];
		LONG			AdjustIMCM[NUMOFYCVALUE];
		LONG			AdjustILAM[NUMOFYCVALUE];
		LONG			AdjustILBM[NUMOFYCVALUE];
		LONG			AdjustILCM[NUMOFYCVALUE];
		LONG			AdjustI0M[NUMOFYCVALUE];
		LONG			AdjustIJXM[NUMOFYCVALUE];
		
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[45];		//用于保存系数的累加值
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

};
void DITask(DWORD *pdwApp);

#endif
