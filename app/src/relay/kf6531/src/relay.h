
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			24 				//需自检的结构体的个数

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:
		LONG 	*dwSampBuf;     	//采样计算缓冲区 
		LONG 	*dwRecBuf;			//合成采样点数据缓冲区，用于合成数据的录波 		
		LONG	*dwPickDataBuf;		//突变量启动采样点数据缓冲区
		LONG	*dwComposeBuf;
		BYTE  	RelCFGCLR;			//保护配置退出，例如过流III段无抵押闭锁功能，则赋该值
		BYTE  	RelCFGSET;			//保护配置投入，例如过流III段无抵押闭锁功能，则赋该值 
		DWORD	dwChanDataMask;		//采样通道数据掩码
		DWORD	dwDCDataMask;		//数字量通道数据掩码
		DWORD	dwComposeDataMask;		//合成通道ILA,ILB,IJX数字量通道数据掩码
	
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;		
		WORD	m_wPickCheck;					//本次保护任务中连续突变量大于定值计数器
		WORD	m_wPickCheckNext[7];			//两次保护任务中连续突变量大于定值计数器
		LONG	m_lPickUpValueArray[7];			//两次保护任务中连续突变量大于定值计数器

		WORD	DCMacroArray[97];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//数字录波通道数据
		LONG	DCCaluResult[6];						//数字录波继电器组计算值保存数组

		LONG 					IAP;				//热过负荷保护电流A,真有效值
		LONG 					IBP;				//热过负荷保护电流B,真有效值
		LONG 					ICP;				//热过负荷保护电流C,真有效值

		TRelElecValPar			U_R50IH;		//高侧过流低压闭锁用的电压
		TRelElecValPar			U_R27U;			//三相失压保护判断用的电压,为高低压侧5个电压中的最大值
		TRelElecValPar			PhaseUHMax;		//高侧相电压最大值
		TRelElecValPar			PhaseUHMin;		//高侧相电压最小值
		TRelElecValPar			PhaseULMin;		//低侧电压最小值
		TRelElecValPar			PhaseULMax;		//低侧电压最小值
		TRelElecValPar			LineULAB;		//低侧线电压
		TRelElecValPar			I_R27U;			//高侧失压保护电流闭锁用的电流
	
		TRelElecValPar			LineUHAB;		//AB线电压
		TRelElecValPar			LineUHBC;		//BC线电压
		TRelElecValPar			LineUHCA;		//CA线电压
		TRelElecValPar			LineUHMax;		//线电压最大值
		TRelElecValPar			UHMin;			//线电压最小值
		TRelElecValPar			ULmin1;
		TRelElecValPar			ULmin2;
		TRelElecValPar			UHA;
		TRelElecValPar			UHB;
		TRelElecValPar			UHC;
		TRelElecValPar			U0;
		TRelElecValPar			ULA;
		TRelElecValPar			ULB;		
		TRelElecValPar			IHA;
		TRelElecValPar			IHB;
		TRelElecValPar			IHC;
		TRelElecValPar			I0;
		TRelElecValPar			IL1;
		TRelElecValPar			IL2;
		TRelElecValPar			IL3;
		TRelElecValPar			IL4;
		TRelElecValPar			ILA;
		TRelElecValPar			ILB;
		TRelElecValPar			IJX;

		
		
		TRelGeneOvValPar 		Rel50I_IA;  		//高压侧A相过流
		TRelGeneOvValPar	 	Rel50I_IB; 			//高压侧B相过流			
		TRelGeneOvValPar 		Rel50I_IC;			//高压侧C相过流			
		TRelGeneOvValPar 		Rel50I_Ia1;			//低压侧a相过流I段	
		TRelGeneOvValPar 		Rel50I_Ia2;			//低压侧a相过流II段			
		TRelGeneOvValPar 		Rel50I_Ib1;			//低压侧b相过流I段
		TRelGeneOvValPar 		Rel50I_Ib2;			//低压侧b相过流II段
		TRelGeneOvValPar_2		Rel50I_JX;			//间隙过流
		TThermalOverLoad		Rel49I_IA;			//反时限热过负荷保护
		TThermalOverLoad		Rel49I_IB;			//反时限热过负荷保护
		TThermalOverLoad		Rel49I_IC;			//反时限热过负荷保护
		TRelGeneOvValPar		Rel50IA1;			//A过负荷1
		TRelGeneOvValPar		Rel50IA2;			//A过负荷2
		TRelGeneOvValPar		Rel50IB1;			//B过负荷1
		TRelGeneOvValPar		Rel50IB2;			//B过负荷2
		TRelGeneOvValPar		Rel50IC1;			//C过负荷1
		TRelGeneOvValPar		Rel50IC2;			//C过负荷2
		TRelGeneOvValPar		Rel50I0;			//零序过流保护
		TRelGeneOvValPar		Rel59U0;			//零序过压保护
		TRelGeneUnValPar 		Rel27U;				//低电压保护元件
		TRelPTBrokPar			Rel74PTH;			//高侧PT断线
		TRelPTBrokPar			Rel74PTLA;			//低侧A相PT断线
		TRelPTBrokPar			Rel74PTLB;			//低侧B相PT断线

		
		BOOL				m_bTripFailFlag;				//跳闸失败标志 
		BOOL 				m_bFaultOnFlag;				//进入故障循环处理标志 
		BOOL				m_bFaultCheckOutFlag;			//故障循环处理检出故障标志 
		BOOL 				m_bHaveMakeStartRptFlag;		//作启动报告标志 
		BOOL				m_bFaultLKFlag;				//故障起始时刻时间锁定标志，用于突变量启动	
		BOOL				m_bLastRecStatusFlag;		//上次录波继电器字状态
		BOOL				m_bIsLogicOk;				//逻辑方程初始化标志
		BOOL 				m_bHaveMakeWarnRpt;			//热过负荷保护已作告警报告标志
		BOOL 				m_bHaveMakeActRpt;			//热过负荷保护已作动作报告标志
		BOOL 				m_bHaveMakeRSTRpt;			//热过负荷保护已作复归报告标志

		BOOL 				m_b52BFLAFlag;				//低侧A断路器跳闸失败标志
		BOOL 				m_b52BFLBFlag;				//低侧B断路器跳闸失败标志
		BOOL 				m_b52BFHFlag;
		BOOL				m_bHaveMake52BF_H_RptFlag; 	//已作低压侧断路器跳闸失败报告		
		BOOL				m_bHaveMake52BF_LA_RptFlag; 	//已作低压侧断路器跳闸失败报告
		BOOL				m_bHaveMake52BF_LB_RptFlag; 	//已作低压侧断路器跳闸失败报告
		BOOL				m_b52BFRecover_H_FLAG;
		BOOL				m_b52BFRecover_LA_FLAG;
		BOOL				m_b52BFRecover_LB_FLAG;
		
		
		DWORD				m_dwFaultSINum;					//故障处理采样间隔计数器 
		DWORD				m_dwFaultLockTime;				//故障锁定定时器 
		TAbsTime 			PickUpTime;						//模值启动或者突变量启动时间定时器 

		
		WORD				m_wRptSerialNumOfAct;			//动作报告序列号
		WORD				m_wRptSerialNumOfStart;		//带故障电量启动报告序列号
		WORD				m_wRptSerialNumOf27U;			//低压保护报告序列号
		BOOL 				m_bLogicChanged;            //可编程逻辑变化标志
		DWORD				m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器


private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		void ThermalOverLoad(void);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitRel50I_IA(void);
		void InitRel50I_IB(void);
		void InitRel50I_IC(void);
		void InitRel50I_Ia1(void);
		void InitRel50I_Ia2(void);
		void InitRel50I_Ib1(void);
		void InitRel50I_Ib2(void);
		void InitRel50I_JX(void);
		void InitRel49IHA(void);
		void InitRel49IHB(void);
		void InitRel49IHC(void);
		void InitRel50IA1(void);
		void InitRel50IA2(void);
		void InitRel50IB1(void);
		void InitRel50IB2(void);
		void InitRel50IC1(void);
		void InitRel50IC2(void);
		void InitRel50IN(void);
		void InitRel59UN(void);
		void InitRel27U(void);
		void InitRel74PT(void);
		void InitStructCheck(void);		
		
		void FalutProcess(void);												//故障循环处理程序 
		BOOL RelayPickUpCHK(void);												//模值启动判别程序 

		BOOL WriteComposeBuf(void);
		void CaluUI(void);
		void CaluUpIp(void);
		void CaluUIAngle(void);
		void PTCheck();
		void Process52BF();
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//保护元件整组复归函数 

		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据
		TRelElecValPar& MaxElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C);		
		TRelElecValPar& MinElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C);
		inline LONG MaxAB(LONG par1,LONG par2)
		{
			return (par1>=par2)? par1:par2;
		};


};		

void RelayTask(DWORD *pdwApp);


#endif
