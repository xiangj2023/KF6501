
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			13 				//需自检的结构体的个数

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
public:
		LONG			*dwSampBuf;			//采样数据缓冲区
		LONG			*dwPickDataBuf;			//突变量启动历史采样数据缓冲区
		LONG			*dwComposeBuf;
		LONG			*dwRECBuf;
		BYTE			RelCFGCLR;				//保护配置退出
		BYTE			RelCFGSET;				//保护配置退出

		DWORD			dwChanDataMask;		//用来读取采样数据的通道掩码
		DWORD			dwDCDataMask;
		DWORD			dwComposeMask;

		LONG    lSampData1;
		LONG    lSampData2;
		LONG    lSampDate3;
		LONG 	lPickValue;
		LONG	m_lPickUpValue;
		WORD	PickCheck;
		WORD	PickCheckNext;
		
		WORD	DCMacroArray[97];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//数字录波通道数据
		LONG	DCCaluResult[6];						//数字录波继电器组计算值

		
		TRelElecValPar		UP;	//保护电压
		TRelElecValPar		U1;	//差压1
		TRelElecValPar		U2;	//差压2
		TRelElecValPar		Ud1;	//差压2
		TRelElecValPar		Ud2;	//差压2
		TRelElecValPar		Ud3;	//差压3
		TRelElecValPar		IP;	//保护电流
		TRelElecValPar		I1;	//差流1
		TRelElecValPar		I2;	//差流2
		TRelElecValPar		I3;	//差流3
		TRelElecValPar		IP2;
		TRelElecValPar		IP3; //三次谐波
		TRelElecValPar		IP5; //五次谐波
		TRelElecValPar		IP7; //七次谐波
		TRelElecValPar		IPH; //谐波过流保护用的合成谐波电流
		QWORD				m_IPH;
		
		

		 TRelGeneOvValPar  		Rel50I0;			//速断保护元件
		 TRelGeneOvValPar		Rel50I1;			//过流保护元件
		 TRelGeneOvValPar		Rel59U;				//过压保护元件
		 TRelGeneOvValPar		Rel50IH;			//谐波过流保护元件
		 TRelGeneOvValPar		Rel60I1;			//差流1保护元件
		 TRelGeneOvValPar		Rel60I2;			//差流2保护元件
		 TRelGeneOvValPar		Rel50ST;			//支路过流保护元件
		 TRelGeneOvValPar		Rel60Ud1;			//差压1保护元件
		 TRelGeneOvValPar		Rel60Ud2;			//差压2保护元件
		 TRelGeneOvValPar		Rel60Ud3;			//差压3保护元件
		 TRelGeneUnValPar		Rel27U;				//低压保护元件
		 TRelPTBrokPar			RelPT;


		BOOL				m_b52BFFlag;					//跳闸失败标志 
		BOOL 				FaultOnFlag;					//进入故障循环处理标志 
		BOOL				FaultCheckOutFlag;			//故障循环处理检出故障标志 
		BOOL 				HaveMakeStartRptFlag;		//作启动报告标志 
		BOOL				m_bFaultLKFlag;				//故障起始时刻时间锁定标志，用于突变量启动		
//		BOOL				m_bNextRelayStart;			//本次保护启动判断中可能发生再次动作标志
		BOOL				m_bHaveRelayAct;			//本次保护启动判断中保护曾动作过标志	
		
		BOOL				m_bLastRecStatusFlag;		//上次录波继电器字状态
		BOOL				m_bHaveMakeTripFailRpt;
		
		DWORD				FaultSINum;					//故障处理采样间隔计数器 
		DWORD				FaultLockTime;				//故障锁定定时器 
		WORD 				PickUpRW;					//模值或者突变量启动继电器字 
		TAbsTime 			PickUpTime;					//模值启动或者突变量启动时间定时器 


		WORD				RptSerialNumOfAct;			//动作报告序列号
		WORD				RptSerialNumOfStart;			//带故障电量启动报告序列号
		WORD				RptSerialNumOf27U;			//低压保护报告序列号
		BOOL				m_bIsLogicOk;
		BOOL 				m_bLogicChanged;            //可编程逻辑变化标志
		DWORD				m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器


private:

public:
		CRelayTask(DWORD *pdwApp);

		void OnSampleEnd(DWORD arg);
		void WriteCompseData(void);
		void OnTimeOut(DWORD id);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
	    void InitRel50I0(void);		//初始化速断保护元件
		void InitRel50I1(void);		//初始化过流保护元件
	    void InitRel59U(void);		//初始化过压保护元件
	    void InitRel50IH(void);		//初始化谐波过流保护元件
	    void InitRel60I1(void);		//初始化差流1保护元件
	    void InitRel60I2(void);		//初始化差流2保护元件
	    void InitRel50ST(void);		//初始化支路过流保护元件
	    void InitRel60Ud1(void);		//初始化差压1保护元件
	    void InitRel60Ud2(void);		//初始化差压2保护元件
	    void InitRel60Ud3(void);		//初始化差压3保护元件
	    void InitRel27U(void);		//初始化低压保护元件
	    void InitRelPT(void);		//初始化PT断线保护元件
		void InitStructCheck(void);

		void FalutProcess(void);												//故障循环处理程序 
		BOOL RelayPickUpCHK(void);										//模值启动判别程序 
		void CaluUI(void);
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//保护元件整组复归函数
		void Process52BF();

		
		void InitDCRecInfo();	//初始化录波参数
		void WriteDCChannel();	//写入数字录波数据		
};		

void RelayTask(DWORD *pdwApp);


#endif
