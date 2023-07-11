
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#define CHECK_NUM			7 				//需自检的结构体的个数

#define	RELAY_PHASE_A 0x10
#define	RELAY_PHASE_B 0x11
#define	RELAY_PHASE_C 0x12

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		LONG 	*dwSampBuf;     	//采样计算缓冲区 
		LONG 	*dwComposeBuf;		//合成采样点数据缓冲区，用于合成数据的傅氏计算 
		LONG	*dwPickDataBuf;		//突变量启动采样点数据缓冲区
		BYTE  	RelCFGCLR;			//保护配置退出，例如过流III段无抵押闭锁功能，则赋该值
		BYTE  	RelCFGSET;			//保护配置投入，例如过流III段无抵押闭锁功能，则赋该值 

		DWORD	dwChanDataMask;		//采样通道数据掩码
		DWORD	dwDCDataMask;		//数字量通道数据掩码	

		//突变量计算部分
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;		 //突变量本次计算值
		LONG	m_lPickUpValue;  //上一次的突变量
		WORD	PickCheck;		 //连续次数
		WORD	PickCheckNext[5];	 //不连续次数
		
		WORD	DCMacroArray[97];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//数字录波通道数据
		LONG	DCCaluResult[6];						//数字录波继电器组计算值

  
		TRelElecValPar			Ua;
		TRelElecValPar			Ub;
		TRelElecValPar			Uc;
		TRelElecValPar			Uab;
		TRelElecValPar			Ubc;
		TRelElecValPar			Uca;
		TRelElecValPar			U0;
		TRelElecValPar			U1;
		TRelElecValPar			U2;
		TRelElecValPar			U3;
		TRelElecValPar			Ia;
		TRelElecValPar			Ib;
		TRelElecValPar			Ic;
		TRelElecValPar			I0;
		TRelElecValPar			I1;
		TRelElecValPar			Ulmin;		//用于低压闭锁的电压,为三个线电压中的最小值
		TRelElecValPar			Ulmax;		//用于低压保护，为三个线电压中的最大值
		TRelElecValPar			NU2;		//负序电压
		TRelElecValPar			Ipmax;		//三相电流中的最大值
		TRelElecValPar			U5;
		TRelElecValPar			I5;
		TRelElecValPar			Factor1; //因子1
		TRelElecValPar			Factor2;//因子2


		TRelGeneOvValPar		R50I0;  //速断元件
		TRelGeneOvValPar		R50I1;	//过流元件
		TRelGeneOvValPar		R59U;	//过压元件
		TRelGeneOvValPar		R60I;	//不平衡电流元件
		TRelGeneOvValPar		R60U;	//不平衡电压元件
		TRelGeneUnValPar		R27U;	//低压


		
		BOOL				TripFailFlag;				//跳闸失败标志 
		BOOL 				FaultOnFlag;				//进入故障循环处理标志 
		BOOL				FaultCheckOutFlag;			//故障循环处理检出故障标志 
		BOOL 				HaveMakeStartRptFlag;		//作启动报告标志 
		BOOL				m_bFaultLKFlag;				//故障起始时刻时间锁定标志，用于突变量启动	
		BOOL				m_bLastRecStatusFlag;		//上次录波继电器字状态
		BOOL				m_bIsLogicOk;
		BOOL 				m_bLogicChanged;            //可编程逻辑变化标志
		DWORD				m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器
		BOOL 				m_b52BFFLAG;
		
		BOOL 				m_bHaveMakeTripFailRpt;
		
		DWORD				FaultSINum;					//故障处理采样间隔计数器 
		DWORD				FaultLockTime;				//故障锁定定时器 
		TAbsTime 			PickUpTime;					//模值启动或者突变量启动时间定时器 

		
		WORD				RptSerialNumOfAct;			//动作报告序列号
		WORD				RptSerialNumOfStart;		//带故障电量启动报告序列号
		WORD				RptSerialNumOf27U;			//低压保护报告序列号

		LONG				m_U2Set;
		DWORD				m_BusPTTimer;
		

public:
		void OnSampleEnd(DWORD ptr, DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitR50I0(void);
		void InitR50I1(void);
		void InitR60I(void);
		void InitR60U(void);
		void InitR59U(void);
		void InitR27U(void);		
		void InitStructCheck(void);
		void PTCheck();
		void R59NAlarm(); //零压告警
		void WriteComposeBuf();
		void FalutProcess(void);												//故障循环处理程序 
		BOOL RelayPickUpCHK(void);										//模值启动判别程序 
		void CaluUI(void);
		void CaluUpIp(void);
		void CaluU2(void);
		void CaluU5I5(void);
		void CaluUIAngle();
		void Process52BF(void);
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//保护元件整组复归函数 
		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);		
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase);
		TRelElecValPar ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);

};		

void RelayTask(DWORD *pdwApp);


#endif
