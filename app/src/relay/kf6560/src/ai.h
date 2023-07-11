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
//------------------------------------------------------------------
#define CTBRKPM				15				//CT断线检测Ip与Im倍数关系
#define CTBRCHECKVALUE 		1612		//CT断线检测阀值，0.05In对应的测量值数字量
//------------------------------------------------------------------

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
//==============遥测计算用的变量定义=====================================
		LONG					*m_pDataBuf;										//测量数据缓冲区
		LONG					*m_pComposeBuf;
		DWORD					m_AIDataMask;										//测量通道用的数据掩码
		LONG					m_Um;
		LONG					m_U1m;
		LONG					m_U2m;
		
		LONG					m_Ud1m;
		LONG					m_Ud2m;
		LONG					m_Ud3m;
		LONG					m_Im;
		LONG					m_I1m;
		LONG					m_I2m;
		LONG					m_I3m;		
		LONG					m_lMSP;				//有功
		LONG					m_lMSQ;				//无功
		LONG					m_lMSS;				//视在功率
		LONG					m_lMSCOS;			//功率因数
		TRelElecValPar			m_Ih1m;
		TRelElecValPar			m_Ih2m;
		TRelElecValPar			m_Ih3m;
		TRelElecValPar			m_Ih5m;
		TRelElecValPar			m_Ih7m;
		TRelElecValPar			m_Ih9m;
		TRelElecValPar			m_Ih11m;
		//**************************CT断线检测****************			yanxs 2013-03-06
		TRelCTBrokPar			RelCTI;
		LONG					CTCurSet;			//CT断线检测电流门槛值
		LONG					CTTimeSet;			//CT断线告警时间
		LONG					m_Ip;				//CT断线检测保护电流It
		LONG					m_IMax;				//CT断线检测T线最大电流
		LONG					m_IMin;
		//***************************************************
		
		LONG			UMArray[NUMOFYCVALUE];
		LONG			U1MArray[NUMOFYCVALUE];
		LONG			U2MArray[NUMOFYCVALUE];
		LONG			Ud1MArray[NUMOFYCVALUE];
		LONG			Ud2MArray[NUMOFYCVALUE];
		LONG			Ud3MArray[NUMOFYCVALUE];
		LONG			IMArray[NUMOFYCVALUE];
		LONG			I1MArray[NUMOFYCVALUE];
		LONG			I2MArray[NUMOFYCVALUE];
		LONG			I3MArray[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			COSArray[NUMOFYCVALUE];
		LONG			IH1MArray[NUMOFYCVALUE];
		LONG			IH2MArray[NUMOFYCVALUE];
		LONG			IH3MArray[NUMOFYCVALUE];
		LONG			IH5MArray[NUMOFYCVALUE];
		LONG			IH7MArray[NUMOFYCVALUE];
		LONG			IH9MArray[NUMOFYCVALUE];
		LONG			IH11MArray[NUMOFYCVALUE];


//=============系数校正用的变量===========================================
		LONG			*m_pCoefAdjustBuf;			//系数校正数据缓冲区		
		DWORD			m_CoefAdjustMask;			//系数校正用的通道数据掩码
		DWORD			m_dwAdjustCnt;				//系数校正计算次数
		DWORD			m_dwWaitCnt;				//系数校正后等待的次数
		BOOL			m_bIsCoefAdjust;			//开始系数校正标志
		BOOL			m_bHaveAdjustAngle;			//角差已校正标志
		BOOL			m_bIsDriftAdjust;			//死区值校正标志
  // 1.校正计算出的变量值
		LONG			m_AdjustUm;
		LONG			m_AdjustU1m;
		LONG			m_AdjustU2m;
		LONG			m_AdjustUd1m;
		LONG			m_AdjustUd2m;
		LONG			m_AdjustUd3m;
		LONG			m_AdjustIm;
		LONG			m_AdjustI1m;
		LONG			m_AdjustI2m;
		LONG			m_AdjustI3m;
		LONG			m_AdjustP;
		LONG			m_AdjustQ;			
		LONG			m_lDACoef;	//系数校正时计算所得的角差系数
		TRelElecValPar	m_AdjustUp;
		TRelElecValPar	m_AdjustU1p;
		TRelElecValPar	m_AdjustU2p;
		TRelElecValPar	m_AdjustUd1p;
		TRelElecValPar	m_AdjustUd2p;
		TRelElecValPar	m_AdjustUd3p;
		TRelElecValPar	m_AdjustIp;
		TRelElecValPar	m_AdjustI1p;
		TRelElecValPar	m_AdjustI2p;
		TRelElecValPar	m_AdjustI3p;
 // 2.各通道死区值定义		
		LONG			m_lDriftU;
		LONG			m_lDriftU1;
 		LONG			m_lDriftU2;
		LONG			m_lDriftUd1;
		LONG			m_lDriftUd2;
		LONG			m_lDriftUd3;
		LONG			m_lDriftIm;
		LONG			m_lDriftI1;
		LONG			m_lDriftI2;
		LONG			m_lDriftI3;
 // 3.系数校正中源电压、电流值
		DWORD			m_dwSourceU;
		DWORD			m_dwSourceI;
 // 4.系数校正中各电量的待累加量值--经过冒泡排序后的
		LONG			m_lMidUp;
		LONG			m_lMidU1p;
		LONG			m_lMidU2p;
		LONG			m_lMidUd1p;
		LONG			m_lMidUd2p;
		LONG			m_lMidUd3p;
		LONG			m_lMidIp;
		LONG			m_lMidI1p;
		LONG			m_lMidI2p;
		LONG			m_lMidI3p;
		LONG			m_lMidUm;
		LONG			m_lMidU1m;
		LONG			m_lMidU2m;
		LONG			m_lMidUd1m;
		LONG			m_lMidUd2m;
		LONG			m_lMidUd3m;
		LONG			m_lMidIm;
		LONG			m_lMidI1m;
		LONG			m_lMidI2m;
		LONG			m_lMidI3m;
		LONG			m_lMidP;
		LONG			m_lMidQ;
 // 5.系数校正各电量数组
		LONG			AArrayUp[NUMOFYCVALUE];
		LONG			AArrayU1p[NUMOFYCVALUE];
		LONG			AArrayU2p[NUMOFYCVALUE];		
		LONG			AArrayUd1p[NUMOFYCVALUE];
		LONG			AArrayUd2p[NUMOFYCVALUE];
		LONG			AArrayUd3p[NUMOFYCVALUE];
		LONG			AArrayIp[NUMOFYCVALUE];
		LONG			AArrayI1p[NUMOFYCVALUE];
		LONG			AArrayI2p[NUMOFYCVALUE];
		LONG			AArrayI3p[NUMOFYCVALUE];
		LONG			AArrayUm[NUMOFYCVALUE];
		LONG			AArrayU1m[NUMOFYCVALUE];
		LONG			AArrayU2m[NUMOFYCVALUE];		
		LONG			AArrayUd1m[NUMOFYCVALUE];
		LONG			AArrayUd2m[NUMOFYCVALUE];
		LONG			AArrayUd3m[NUMOFYCVALUE];
		LONG			AArrayIm[NUMOFYCVALUE];
		LONG			AArrayI1m[NUMOFYCVALUE];
		LONG			AArrayI2m[NUMOFYCVALUE];
		LONG			AArrayI3m[NUMOFYCVALUE];
		LONG			AArrayP[NUMOFYCVALUE];
		LONG			AArrayQ[NUMOFYCVALUE];
 // 6.系数校正中各系数的累加量				
		LONG			m_pAdjustData[22];	 

 
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据

		DWORD      dwMsgID;
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
//		void OnAutoAdjustStep1(DWORD arg);			
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);		
		void Init(void);
		
		void CaluMSPQ(LONG *UDataBuf,LONG *IDataBuf,LONG DACoef, LONG &MSP,LONG &MSQ,LONG &MSCOS,BOOL coef = TRUE);//计算测量有功，无功
		void CoefAdjust(void);//系数校正
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG AiBubbleSort(LONG Val,LONG OriginalData[NUMOFYCVALUE]);		
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void WriteMSComposeData(LONG *caludatabuf,LONG *originaldatabuf);
		//***********************CT断线检查
		void InitCTI(void);
		void CaluCTMaxMin(void);
		//*****************************
//		void RefreshCommRW();		
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
};
void DITask(DWORD *pdwApp);

#endif
