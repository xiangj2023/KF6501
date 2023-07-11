#ifndef _AI_H_
#define _AI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbfunc.h"
#include "parafunc.h"
#include "Relay_defs.h"

//#define AI_TIMER_PERIOD			200		//定义AI定时器时间间隔为10毫秒
//定义AI定时器时间间隔为240毫秒，修改时需注意其可能会影响到测频,需保证如下关系:AI_TIMER_PERIOD/20 > FREQBUFSIZE
#define AI_TIMER_PERIOD			200	
//每次测量任务中需要计算的频率个数
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)
//------------------------------------------------------------------
#define CTBROKERDELAY		15			//CT断线检测时间 3s     3000/AI_TIMER_PERIOD = 15
#define CTBRKPM				15				//CT断线检测Ip与Im倍数关系
#define CTBRCHECKVALUE 		1612		//CT断线检测阀值，0.05In对应的测量值数字量
//------------------------------------------------------------------

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
//---------------测频用相关变量------------------------------
		SHORT			*m_wFreqBuf;		
		LONG			*m_FreqCaluBuf;
		DWORD			m_dwFreqMask;
		LONG			m_dwFreq;
		DWORD			m_dwPreFreq;		
		BOOL			m_FreqFlag;
//-----------------------------------------------------------
		LONG			*m_pDataBuf;			//测量数据缓冲区
		LONG			*m_pComposeBuf;			//测量数据合成缓冲区
		LONG			*m_pCoefAdjustBuf;		//系数校正数据缓冲区
		
		DWORD			m_AIDataMask;			//测量通道用的数据掩码
		DWORD			m_CoefAdjustMask;		//系数校正用的通道数据掩码


		DWORD			m_dwAdjustCnt;			//系数校正计算次数
		BOOL			m_bIsCoefAdjust;		//开始系数校正标志
		BOOL			m_bIsSourceOK;			//源已校正标志
		BOOL			m_bIsDriftAdjust;		//死区值校正标志
		DWORD			m_dwWaitCnt;			//系数校正开始等待次数

		LONG			m_dwSourceU;
		LONG			m_dwSourceI;
		QWORD			m_OneKilowattP;			// 1度电的数字量,有功		
		QWORD			m_OneKilowattQ;			// 1度电的数字量,无功
		QWORD			m_AccKilowattP;			//累加的功率值，有功
		QWORD			m_AccKilowattQ;			//累加的功率值，无功


//----------------遥测值定义-----------------------------

		LONG			Ua;
		LONG			Ub;
		LONG			Uc;
		LONG			U0;
		LONG			U1;
		LONG			U2;
		LONG			U3;
		LONG			Uab;
		LONG			Ubc;
		LONG			Uca;
		LONG			IA;
		LONG			IB;
		LONG			IC;
		LONG			I0;
		LONG			I1;
		
		LONG			Pa;
		LONG			Qa;
		LONG			Pb;
		LONG			Qb;
		LONG			Pc;
		LONG			Qc;
		LLONG			P;
		LLONG			Q;
		LLONG			S;
		LONG			cos;

		TRelElecValPar	IAM;
		TRelElecValPar	IBM;
		TRelElecValPar	ICM;
		TRelElecValPar	UAM;
//---------------CT断线检查功能增加内容-----------------------	 yanxs 2013-03-22
		TRelCTBrokPar	RelCTA;
		TRelCTBrokPar	RelCTB;
		TRelCTBrokPar	RelCTC;
		LONG			CTCurSet;			//CT断线检测电流门槛值
		LONG			CTTimeSet;			//CT断线告警时间
		LONG			Ia_p;				//CT断线检测保护电流Ia
		LONG			Ib_p;
		LONG			Ic_p;
		LONG			IaMax;				//CT断线检测a相最大电流
		LONG			IaMin;
		LONG			IbMax;
		LONG			IbMin;
		LONG			IcMax;
		LONG			IcMin;

//---------------用于系数校正的变量定义-----------------
		LONG			AdjAA;				//系数校正时计算所得的角差系数
		LONG			AdjBA;				//系数校正时计算所得的角差系数		
		LONG			AdjCA;				//系数校正时计算所得的角差系数
		
		LONG			AdjUa;
		LONG			AdjUb;
		LONG			AdjUc;
		LONG			AdjU0;
		LONG			AdjU1;
		LONG			AdjU2;
		LONG			AdjU3;		
		LONG			AdjIA;
		LONG			AdjIB;
		LONG			AdjIC;
		LONG			AdjI0;
		LONG			AdjI1;
		LONG			AdjPa;
		LONG			AdjQa;
		LONG			AdjPb;
		LONG			AdjQb;
		LONG			AdjPc;
		LONG			AdjQc;

		TRelElecValPar	Uap;
		TRelElecValPar	Ubp;
		TRelElecValPar	Ucp;
		TRelElecValPar	U0p;
		TRelElecValPar	U1p;
		TRelElecValPar	U2p;
		TRelElecValPar	U3p;
		TRelElecValPar	Iap;
		TRelElecValPar	Ibp;
		TRelElecValPar	Icp;
		TRelElecValPar	I0p;
		TRelElecValPar	I1p;
		

		LONG			MidAdjUa;
		LONG			MidAdjUb;
		LONG			MidAdjUc;
		LONG			MidAdjU0;
		LONG			MidAdjU1;
		LONG			MidAdjU2;
		LONG			MidAdjU3;		
		LONG			MidAdjIA;
		LONG			MidAdjIB;
		LONG			MidAdjIC;
		LONG			MidAdjI0;
		LONG			MidAdjI1;
		
		LONG			MidAdjUap;
		LONG			MidAdjUbp;
		LONG			MidAdjUcp;
		LONG			MidAdjU0p;
		LONG			MidAdjU1p;
		LONG			MidAdjU2p;
		LONG			MidAdjU3p;
		LONG			MidAdjIap;
		LONG			MidAdjIbp;
		LONG			MidAdjIcp;
		LONG			MidAdjI0p;
		LONG			MidAdjI1p;
		
		
		
//---------------用于排序的数组定义------------------------

		// 上传遥测值排序数组
		
		LONG			UaArray[NUMOFYCVALUE];
		LONG			UbArray[NUMOFYCVALUE];
		LONG			UcArray[NUMOFYCVALUE];
		LONG			U0Array[NUMOFYCVALUE];
		LONG			U1Array[NUMOFYCVALUE];
		LONG			U2Array[NUMOFYCVALUE];
		LONG			U3Array[NUMOFYCVALUE];
		LONG			UabArray[NUMOFYCVALUE];
		LONG			UbcArray[NUMOFYCVALUE];
		LONG			UcaArray[NUMOFYCVALUE];
		LONG			IaArray[NUMOFYCVALUE];
		LONG			IbArray[NUMOFYCVALUE];
		LONG			IcArray[NUMOFYCVALUE];
		LONG			I0Array[NUMOFYCVALUE];
		LONG			I1Array[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			cosArray[NUMOFYCVALUE];
		

		//系数校正电量排序数组

		
		LONG			AdjUaArray[NUMOFYCVALUE];
		LONG			AdjUbArray[NUMOFYCVALUE];
		LONG			AdjUcArray[NUMOFYCVALUE];
		LONG			AdjU0Array[NUMOFYCVALUE];
		LONG			AdjU1Array[NUMOFYCVALUE];
		LONG			AdjU2Array[NUMOFYCVALUE];
		LONG			AdjU3Array[NUMOFYCVALUE];		
		LONG			AdjIAArray[NUMOFYCVALUE];
		LONG			AdjIBArray[NUMOFYCVALUE];
		LONG			AdjICArray[NUMOFYCVALUE];
		LONG			AdjI0Array[NUMOFYCVALUE];
		LONG			AdjI1Array[NUMOFYCVALUE];
		
		LONG			UapArray[NUMOFYCVALUE];
		LONG			UbpArray[NUMOFYCVALUE];
		LONG			UcpArray[NUMOFYCVALUE];
		LONG			U0pArray[NUMOFYCVALUE];
		LONG			U1pArray[NUMOFYCVALUE];
		LONG			U2pArray[NUMOFYCVALUE];
		LONG			U3pArray[NUMOFYCVALUE];
		LONG			IapArray[NUMOFYCVALUE];
		LONG			IbpArray[NUMOFYCVALUE];
		LONG			IcpArray[NUMOFYCVALUE];
		LONG			I0pArray[NUMOFYCVALUE];
		LONG			I1pArray[NUMOFYCVALUE];
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[39];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据


//-----------------临时定义值-------------------------
		LONG			UaDrift;
		LONG			UbDrift;
		LONG			UcDrift;
		LONG			U0Drift;
		LONG			U1Drift;
		LONG			U2Drift;
		LONG			U3Drift;
		LONG			IADrift;
		LONG			IBDrift;
		LONG			ICDrift;
		LONG			I0Drift;
		LONG			I1Drift;

		DWORD      dwMsgID;
		
		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){};
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);
		void Init(void);
		void CaluUIAngle(void);
		void WriteMSComposeBuf(LONG *ComposeBuf,const LONG*DataBuf);
		void CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ);//计算测量有功，无功
		void CoefAdjust(void);//系数校正
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
		//----------CT断线检查-------------
		void CaluMaxMin(void);
		void InitCTACheck();
		void InitCTBCheck();		
		void InitCTCCheck();		
		//--------------------------------
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);

};
void DITask(DWORD *pdwApp);

#endif
