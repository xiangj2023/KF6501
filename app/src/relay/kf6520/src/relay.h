
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#define CHECK_NUM			17 				//需自检的结构体的个数
class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		LONG 	*dwSampBuf;     	//采样计算缓冲区 
		LONG 	*dwRecBuf;			//合成采样点数据缓冲区，用于合成数据的录波 
		LONG 	*dwComposeBuf;		//合成采样点数据缓冲区，用于合成数据的傅氏计算 
		LONG 	*dwIncreCurBuf;		//增量保护历史采样点数据缓冲区 
		LONG	*dwPickDataBuf;		//突变量启动采样点数据缓冲区
		LONG	*dwPDDataBuf;
		BYTE  	RelCFGCLR;			//保护配置退出，例如过流III段无抵押闭锁功能，则赋该值
		BYTE  	RelCFGSET;			//保护配置投入，例如过流III段无抵押闭锁功能，则赋该值 
		LONG	K35;
		DWORD	dwChanDataMask;		//采样通道数据掩码
		DWORD	dwInCreDataMask;	//增量保护计算所用的通道数据掩码
		DWORD	dwComposeDataMask;	//模拟量合成通道数据掩码
		DWORD	dwDCDataMask;		//数字量通道数据掩码		

	
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;
		LONG	m_lPickUpValue;
		WORD	PickCheck;		
		WORD	PickCheckNext;
		
		WORD	DCMacroArray[97];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//数字录波通道数据
		LONG	DCCaluResult[6];						//数字录波继电器组计算值

		TRelElecValPar			It;
		TRelElecValPar			If;
		TRelElecValPar			Ip;
		TRelElecValPar			U1;
		TRelElecValPar			U2;
		TRelElecValPar			U3;
		TRelElecValPar			U4;
		TRelElecValPar			Up;
		TRelElecValPar			ULock;		//速断过流保护中的低压闭锁电压
		TRelElecValPar			I2;
		TRelElecValPar			I3;
		TRelElecValPar			I5;
		TRelElecValPar			Impd;
		TRelElecValPar			ImpdT;
		TRelElecValPar			ImpdF;
		
		TRelGeneOvValPar 		Rel50I0;  							//速断保护元件 
		TRelGeneOvValPar	 	Rel50I1; 							//过流1段保护元件 
		TRelGeneOvValPar 		Rel50I2;							//过流2段保护元件 
		TRelGeneOvValPar 		Rel50I3;							//过流3段保护元件 

		TRel21Par 				Rel211;									//距离1段保护元件 
		TRel21Par 				Rel212;									//距离2段保护元件 
		TRel21Par				Rel213;									//距离3段保护元件 
		TRel21Par 				Rel214;									//距离4段保护元件 

		TRelGeneUnValPar 		Rel27U;							//低电压保护元件 
		TRelInverseCurPar 		Rel51I;							//反时限过流保护元件 		
		TRelFaultLocatePar 		Rel21FL_T;						//T线故障测距元件 
		TRelFaultLocatePar 		Rel21FL_F;						//F线故障测距元件 
		TRelPTBrokPar 			RelPT1;							//PT1断线告警元件 
		TRelPTBrokPar 			RelPT2;							//PT2断线告警元件 

		TRel79Par				Rel79;							//重合闸保护元件 
		TRel50DIPar				Rel50DI;							//增量保护元件 

		
		BOOL				TripFailFlag;				//跳闸失败标志 
		BOOL 				FaultOnFlag;				//进入故障循环处理标志 
		BOOL				CurRecoverFlag;				//重合闸动作后，电流恢复标志 
		BOOL				FaultCheckOutFlag;			//故障循环处理检出故障标志 
		BOOL 				HaveMakeStartRptFlag;		//作启动报告标志 
		BOOL				m_bFaultLKFlag;				//故障起始时刻时间锁定标志，用于突变量启动	
		BOOL				m_bLastRecStatusFlag;		//上次录波继电器字状态
		//BOOL				m_bNextRelayStart;			//本次保护启动判断中可能发生再次动作标志
		BOOL				m_bHaveTripFailedRpt;		//已经做过断路器跳闸失败报告
		BOOL				m_bHaveRelayAct;			//本次保护启动判断中保护曾动作过标志	
		BOOL				m_bIsLogicOk;
		BOOL				m_bIsHaveMakePTRpt;		//已作PT断线闭锁距离保护报告
		BOOL				m_bOneOhmPTLK;			//发生阻抗小于1欧姆PT断线		
		BOOL				m_bPDCheck;				//是否带记忆功率方向判断
		BOOL 				m_b52BFFLAG;
		BOOL				m_bR79PickUpRpt;
		LONG				m_lOtherRelayTrip;		//其他保护动作，除增量保护外
		
		DWORD				FaultSINum;					//故障处理采样间隔计数器 
		DWORD				FaultLockTime;				//故障锁定定时器 
		WORD 				PickUpRW;					//模值或者突变量启动继电器字 
		TAbsTime 			PickUpTime;					//模值启动或者突变量启动时间定时器 

//		WORD				RptSerialNumOfPick;			//入段报告序列号   yanxs 13-02-05
//		BOOL				RptFlagOfPickUp;				//已做入段报告启动标志
		WORD				RptSerialNumOfAct;			//动作报告序列号
		WORD				RptSerialNumOfStart;		//带故障电量启动报告序列号
		WORD				RptSerialNumOf27U;			//低压保护报告序列号

		BOOL 				m_bLogicChanged;            //可编程逻辑变化标志
		DWORD				m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器

		//保护报告，针对供电方式的不同，对应的故障电量不同
		WORD				wRep211;
		WORD				wRep212;
		WORD				wRep213;
		WORD				wRep214;
		WORD				wRep50I0;
		WORD				wRep50I1;
		WORD				wRep50I2;
		WORD				wRep50I3;
		WORD				wRep51I;
		WORD				wRep50DI;
		WORD				wRepR212Acc;
		WORD				wRepR213Acc;
		WORD				wRepR214Acc;
		WORD				wRep50I1Acc;
		WORD				wRep50I2Acc;
		WORD				wRep50I3Acc;
		WORD				wRep27U;
		//*************************增加入段报告部分    **********************   yanxs   13-02-05
		WORD				wRep211Pick;
		WORD				wRep212Pick;
		WORD				wRep213Pick;
		WORD				wRep214Pick;
		WORD				wRep50I0Pick;
		WORD				wRep50I1Pick;
		WORD				wRep50I2Pick;
		WORD				wRep50I3Pick;
		WORD				wRep51IPick;
		WORD				wRep50DIPick;
		WORD				wRepR212AccPick;					//增加加速入段报告   2013-06-20   yanxs
   		WORD				wRepR213AccPick;
		WORD				wRepR214AccPick;
		WORD				wRep50I1AccPick;
		WORD				wRep50I2AccPick;
		WORD				wRep50I3AccPick;
//		WORD				wRep27UPick;
		//*******************************************
		//*************************增加返回报告部分    **********************	yanxs	13-06-20
		WORD				wRep211RES;
		WORD				wRep212RES;
		WORD				wRep213RES;
		WORD				wRep214RES;
		WORD				wRep50I0RES;
		WORD				wRep50I1RES;
		WORD				wRep50I2RES;
		WORD				wRep50I3RES;
		WORD				wRep51IRES;
		WORD				wRep50DIRES;
		WORD				wRepR212AccRES;					
		WORD				wRepR213AccRES;
		WORD				wRepR214AccRES;
		WORD				wRep50I1AccRES;
		WORD				wRep50I2AccRES;
		WORD				wRep50I3AccRES;
private:


public:
		void OnSampleEnd(DWORD ptr, DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitRel50I0(void);
		void InitRel50I1(void);
		void InitRel50I2(void);
		void InitRel50I3(void);
		void InitRel211(void);
		void InitRel212(void);
		void InitRel213(void);
		void InitRel214(void);
		void InitRel27U(void);
		void InitRel51I(void);
		void InitRel21FL_T(void);
		void InitRel21FL_F(void);
		void InitRelPT1(void);
		void InitRelPT2(void);
		void InitRel79(void);
		void InitRel50DI(void);
		void InitRepSerialNum(void);
		void InitLoadMonitor(void);
		void InitStructCheck(void);
		void PTCheck();
		void WriteComposeBuf();
		void FalutProcess(void);												//故障循环处理程序 
		BOOL RelayPickUpCHK(void);										//模值启动判别程序 
		void CaluUI(void);
		void CaluUpIp(void);
		void CaluZ(void);
		void CaluUIAngle(void);
		void Process52BF(void);

		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void CalcFaultLocateSetTab(TRelFaultLocatePar *Relay);	//计算故障测距定值表 
		void FaultLocate(TRelFaultLocatePar *Relay);		//电抗法故障测距 
		void ResetAllRelay(BOOL resetFlag = FALSE);											//保护元件整组复归函数 

		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据
		void LoadMonitor();//负荷监测

};		

void RelayTask(DWORD *pdwApp);


#endif
