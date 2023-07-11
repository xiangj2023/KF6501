#ifndef _AI_H_
#define _AI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbfunc.h"
#include "parafunc.h"
#include "Relay_defs.h"

//定义AI定时器时间间隔为240毫秒，修改时需注意其可能会影响到测频,需保证如下关系:AI_TIMER_PERIOD/20 > FREQBUFSIZE
//遥测任务每20ms即一周波调度一次
#define AI_TIMER_PERIOD_ONE			20
//常规测量值计算的任务调度间隔数
#define AI_TIMER_PERIOD_NUM			10
//常规测量值计算的间隔时间
#define AI_TIMER_PERIOD			(AI_TIMER_PERIOD_ONE*AI_TIMER_PERIOD_NUM)	
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

		WORD 			wTaskScheduleCnt;//测量任务调度计数器
		BOOL			m_bCaluFreqFlag;
//---------------测频用相关变量------------------------------
		SHORT			*m_wFreqBuf;		
		LONG			*m_FreqCaluBuf;
		DWORD			m_dwFreqMask;
		LONG			m_dwFreq[NUM_FREQ_PER_AITASK];
		LONG			m_dwPreFreq;		
		BOOL			m_FreqFlag;
//-----------------------------------------------------------
		LONG			*m_pDataBuf;			//测量数据缓冲区
		LONG			*m_pComposeBuf;			//测量数据合成缓冲区
		LONG			*m_pCoefAdjustBuf;		//系数校正数据缓冲区
//		LONG			*m_pComposeAdjustBuf;								
//---------------变化遥测相关部分-----------------------------
		LONG			*m_pRTBuf;
		DWORD			m_RTMask;

		LONG			m_U1Last;
		LONG			m_U2Last;
		LONG			m_ItLast;
		LONG			m_IfLast;
		LONG			m_U1WritePtr;
		LONG			m_U2WritePtr;
		LONG			m_ItWritePtr;
		LONG			m_IfWritePtr;
		TAbsTime		m_AbsTime;
		TEventAI		m_EventAI;
//------------------------------------------------------------
		
		DWORD			m_AIDataMask;			//测量通道用的数据掩码
		DWORD			m_CoefAdjustMask;		//系数校正用的通道数据掩码


		LONG			m_lDACoef;				//系数校正时计算所得的角差系数
		DWORD			m_dwAdjustCnt;			//系数校正计算次数
		BOOL			m_bIsCoefAdjust;		//开始系数校正标志
		BOOL			m_bIsSourceOK;			//源已校正标志
		BOOL			m_bIsDriftAdjust;		//死区值校正标志
		DWORD			m_dwWaitCnt;			//系数校正开始等待次数

		
		QWORD			m_OneKilowattP;			// 1度电的数字量,有功		
		QWORD			m_OneKilowattQ;			// 1度电的数字量,无功
		QWORD			m_AccKilowattP;			//累加的功率值，有功
		QWORD			m_AccKilowattQ;			//累加的功率值，无功
		//**************************CT断线检测****************			yanxs 2013-03-06
		TRelCTBrokPar	RelCTIt;
		TRelCTBrokPar	RelCTIf;
		LONG			CTCurSet;			//CT断线检测电流门槛值
		LONG			CTTimeSet;			//CT断线告警时间
		LONG			m_It_p;					//CT断线检测保护电流It
		LONG			m_If_p;
		LONG			m_ItMax;				//CT断线检测T线最大电流
		LONG			m_ItMin;
		LONG			m_IfMax;
		LONG			m_IfMin;
		//***************************************************
		


//----------------遥测值定义-----------------------------
		LONG			m_Um;
		LONG			m_U1m;
		LONG			m_U2m;
		LONG			m_Im;
		LONG			m_Itm;
		LONG			m_Ifm;
		LONG			m_U3m;
		LONG			m_U4m;
		LONG			m_lMSP;				
		LONG			m_lMSQ;				
		LONG			m_lMSS;			
		LONG			m_lMSCOS;			
		TRelElecValPar		m_sI1;
		TRelElecValPar		m_sI2;
		TRelElecValPar		m_sI3;
		TRelElecValPar		m_sI5;
		TRelElecValPar		m_sI7;
		TRelElecValPar		m_sI9;
		TRelElecValPar		m_sI11;
		TRelElecValPar		m_Impd;
		LONG	 			m_Freq1;
		LONG				m_Freq2;
		
//--------------用于角度计算的变量定义---------------------		
		TRelElecValPar		m_sIp;
		TRelElecValPar		m_sItm;
		TRelElecValPar		m_sIfm;
		TRelElecValPar		m_sUp;
		TRelElecValPar		m_sU1;
		TRelElecValPar		m_sU2;
		TRelElecValPar		m_sIt;
		TRelElecValPar		m_sIf;


//---------------用于系数校正的变量定义-----------------
		LONG			m_AdjustUm;
		LONG			m_AdjustU1m;
		LONG			m_AdjustU2m;
		LONG			m_AdjustIm;
		LONG			m_AdjustItm;
		LONG			m_AdjustIfm;
		LONG			m_AdjustP;
		LONG			m_AdjustQ;
		LONG			m_AdjustCos;
		LONG			m_dwSourceU;		//系数校正源电压
		LONG			m_dwSourceI;		//系数校正源电流
		
		TRelElecValPar		m_AdjustIp;
		TRelElecValPar		m_AdjustIt;
		TRelElecValPar		m_AdjustIf;
		TRelElecValPar		m_AdjustUp;
		TRelElecValPar		m_AdjustU1;
		TRelElecValPar		m_AdjustU2;
		TRelElecValPar		m_AdjustIh1;

		LONG MedianIp;
		LONG MedianIt;
		LONG MedianIf;
		LONG MedianUp;
		LONG MedianU1;
		LONG MedianU2;
		LONG MedianIm;
		LONG MedianItm;
		LONG MedianIfm;
		LONG MedianUm;
		LONG MedianU1m;
		LONG MedianU2m;
		LONG MedianP;
		LONG MedianIh1;
		
		
//---------------用于排序的数组定义------------------------

		// 上传遥测值排序数组
		LONG			UMArray[NUMOFYCVALUE];		
		LONG			U1Array[NUMOFYCVALUE];
		LONG			U2Array[NUMOFYCVALUE];		
		LONG			U3Array[NUMOFYCVALUE];
		LONG			U4Array[NUMOFYCVALUE];
		LONG			IMArray[NUMOFYCVALUE];
		LONG			ITArray[NUMOFYCVALUE];
		LONG			IFArray[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			COSArray[NUMOFYCVALUE];
		LONG			I1Array[NUMOFYCVALUE];
		LONG			I2Array[NUMOFYCVALUE];
		LONG			I3Array[NUMOFYCVALUE];
		LONG			I5Array[NUMOFYCVALUE];
		LONG			I7Array[NUMOFYCVALUE];
		LONG			I9Array[NUMOFYCVALUE];
		LONG			I11Array[NUMOFYCVALUE];
		LONG			RArray[NUMOFYCVALUE];
		LONG			XArray[NUMOFYCVALUE];
//		LONG			HArray[NUMOFYCVALUE];

		//系数校正电量排序数组
		LONG			AdjustIp[NUMOFYCVALUE];
		LONG			AdjustIt[NUMOFYCVALUE];
		LONG			AdjustIf[NUMOFYCVALUE];
		LONG			AdjustUp[NUMOFYCVALUE];
		LONG			AdjustU1[NUMOFYCVALUE];
		LONG			AdjustU2[NUMOFYCVALUE];
		LONG			AdjustIm[NUMOFYCVALUE];
		LONG			AdjustItm[NUMOFYCVALUE];
		LONG			AdjustIfm[NUMOFYCVALUE];
		LONG			AdjustUm[NUMOFYCVALUE];
		LONG			AdjustU1m[NUMOFYCVALUE];
		LONG			AdjustU2m[NUMOFYCVALUE];
		LONG			AdjustP[NUMOFYCVALUE];
//		LONG			AdjustIh1[NUMOFYCVALUE];
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[15];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据


//-----------------临时定义值-------------------------
		LONG      ItmDrift;
		LONG      IfmDrift;
		LONG      U1mDrift;
		LONG      U2mDrift;

		DWORD      dwMsgID;
		
		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){};
		void OnTimeOut(DWORD id);
		//void OnAutoAdjustStep1(DWORD arg);	
		//void OnAutoAdjustStep2(DWORD arg,DWORD Uval,DWORD Ival);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);
		void Init(void);
		
		void WriteMSComposeBuf();
		void CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ,LONG &MSCOS,BOOL coef = TRUE);//计算测量有功，无功
		void CoefAdjust(void);//系数校正
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
		//***********************CT断线检查
		void InitCTIt(void);
		void InitCTIf(void);
		void CaluCTMaxMin(void);
		//*****************************

};
void DITask(DWORD *pdwApp);

#endif
