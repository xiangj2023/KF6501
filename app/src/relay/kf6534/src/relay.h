
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			9 

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:
		LONG 	*dwSampBuf;     									//采样计算缓冲区 
		LONG 	*dwRecBuf;											//合成采样点数据缓冲区，用于合成数据的录波 
		LONG	*dwComposeBuf;		
	
		DWORD	dwChanDataMask;									//采样通道数据掩码
		DWORD	dwRecDataMask;									//增量保护计算所用的通道数据掩码
		DWORD	dwDCDataMask;		//数字量通道数据掩码		
		WORD	DCMacroArray[97];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//数字录波通道数据
		LONG	DCCaluResult[6];						//数字录波继电器组计算值
		
		BYTE  	RelCFGCLR;			//保护配置退出，例如过流III段无抵押闭锁功能，则赋该值
		BYTE  	RelCFGSET;			//保护配置投入，例如过流III段无抵押闭锁功能，则赋该值 

/*		
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;		
		WORD	m_wPickCheck;					//本次保护任务中连续突变量大于定值计数器
		WORD	m_wPickCheckNext[8];			//两次保护任务中连续突变量大于定值计数器
		LONG	m_lPickUpValueArray[87];			//两次保护任务中连续突变量大于定值计数器
*/		
		TRelElecValPar			U1;
		TRelElecValPar			U2;
		TRelElecValPar			U3;
		TRelElecValPar			U4;
		TRelElecValPar			U5;
		TRelElecValPar			U6;
		TRelElecValPar			U7;
		TRelElecValPar			U8;

		TRelElecValPar			I1;     //IT1
		TRelElecValPar			I2;		//IF1
		TRelElecValPar			I3;		//IP1
		TRelElecValPar			I4;		//IT2
		TRelElecValPar			I5;		//IF2
		TRelElecValPar			I6;		//IP2
		TRelElecValPar			I7;
		TRelElecValPar			I8;
		
		TRelElecValPar			IC1;		//IC1
		TRelElecValPar			IC2;		//IC2

		TRelGeneOvValPar	RelR50IC1;
		TRelGeneOvValPar	RelR50IT1;
		TRelGeneOvValPar	RelR50IF1;
		TRelGeneOvValPar	RelR50IP1;
		TRelGeneOvValPar	RelR50IC2;
		TRelGeneOvValPar	RelR50IT2;
		TRelGeneOvValPar	RelR50IF2;
		TRelGeneOvValPar	RelR50IP2;
	
		BOOL				m_bIsLogicOk;
		BOOL				m_bLastRecStatusFlag;			//上次录波继电器字状态

		
/***********************AT1相关变量区******************************/

		LONG		*dwPickDataBuf_AT1;		 //突变量启动采样点数据缓冲区
		WORD    	RptSerialNumOfAct_AT1;   //动作报告序列号
		WORD    	RptSerialNumOfStart_AT1; //启动报告序列号		
		TAbsTime 	PickUpTime_AT1;			 //模值启动或者突变量启动时间定时器
		WORD		PickCheck_AT1;			 //突变量启动计数器
		LONG		m_lPickUpValue_AT1[4];	 //相邻点突变量启动值
		WORD		m_wPickCheckNext_AT1[4]; //两次保护任务中连续突变量大于定值的计数器		
		WORD		m_wPickCheck_AT1;		//本次判断中连续突变量大于定值的计数器
		
		LONG		lSampData1_AT1;   		//突变量值1
		LONG		lSampData2_AT1;			//突变量值2
		LONG		lSampDate3_AT1;			//突变量值3
		LONG		lPickValue_AT1;   		//本次突变量值

		
		BOOL 		FaultOnFlag_AT1;					//进入故障循环处理标志 		
		DWORD		FaultSINum_AT1;					//故障处理采样间隔计数器 		
		BOOL 		HaveMakeStartRptFlag_AT1;		//作启动报告标志 
		BOOL		FaultCheckOutFlag_AT1;			//故障循环处理检出故障标志 
		DWORD		FaultLockTime_AT1;				//故障锁定定时器 		
		BOOL		m_bFaultLKFlag_AT1;				//故障起始时刻时间锁定标志，用于突变量启动	
		BOOL 		m_b52BFFlag_AT1;
		BOOL        m_bHaveRelayAct_AT1;				//本次保护启动判断中保护曾动作过标志
		BOOL        m_bHaveMake52BFRpt_AT1;				//断路器失败报告已作出
/***********************AT2相关变量区******************************/

		LONG		*dwPickDataBuf_AT2; 	 //突变量启动采样点数据缓冲区
		WORD		RptSerialNumOfAct_AT2;	 //动作报告序列号
		WORD		RptSerialNumOfStart_AT2; //启动报告序列号		
		TAbsTime	PickUpTime_AT2; 		 //模值启动或者突变量启动时间定时器
		WORD		PickCheck_AT2;			 //突变量启动计数器
		LONG		m_lPickUpValue_AT2[4];	 //相邻点突变量启动值
		WORD		m_wPickCheckNext_AT2[4]; //两次保护任务中连续突变量大于定值的计数器 	
		WORD		m_wPickCheck_AT2;		//本次判断中连续突变量大于定值的计数器
		
		LONG		lSampData1_AT2; 		//突变量值1
		LONG		lSampData2_AT2; 		//突变量值2
		LONG		lSampDate3_AT2; 		//突变量值3
		LONG		lPickValue_AT2; 		//本次突变量值

		
		BOOL		FaultOnFlag_AT2;					//进入故障循环处理标志		
		DWORD		FaultSINum_AT2; 				//故障处理采样间隔计数器		
		BOOL		HaveMakeStartRptFlag_AT2;		//作启动报告标志 
		BOOL		FaultCheckOutFlag_AT2;			//故障循环处理检出故障标志 
		DWORD		FaultLockTime_AT2;				//故障锁定定时器		
		BOOL		m_bFaultLKFlag_AT2; 			//故障起始时刻时间锁定标志，用于突变量启动	
		BOOL		m_b52BFFlag_AT2;
		
		BOOL        m_bHaveMake52BFRpt_AT2;
		BOOL		m_bHaveRelayAct_AT2;

		BOOL 				m_bLogicChanged;            //可编程逻辑变化标志
		DWORD				m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器



		
private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void WriteComposeBuf(DWORD dwMask);
		
		void InitRelR50IC1(void);
		void InitRelR50IT1(void);
		void InitRelR50IF1(void);
		void InitRelR50IP1(void);
		void InitRelR50IC2(void);
		void InitRelR50IT2(void);
		void InitRelR50IF2(void);
		void InitRelR50IP2(void);
		
		void InitStructCheck(void);
		void CaluUI(void);

		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		void OnLogicChanged(void);

		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据

		
/************AT1保护函数***************************************************************/		
		void ResetAllRelay_AT1(BOOL resetFlag = FALSE);		
		void FalutProcess_AT1(void);//故障循环处理程序		
		BOOL RelayPickUpCHK_AT1(void);
		void Process52BF_AT1();
		
/************AT1保护函数***************************************************************/		
		void ResetAllRelay_AT2(BOOL resetFlag = FALSE);		
		void FalutProcess_AT2(void);//故障循环处理程序		
		BOOL RelayPickUpCHK_AT2(void);
		void Process52BF_AT2();

};		

void RelayTask(DWORD *pdwApp);


#endif
