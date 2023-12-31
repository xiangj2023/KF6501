
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"


#define CHECK_NUM			21 				//需自检的结构体的个数
#define PHASE_A 100							//高压侧A相为电流最大相，用于功率方向判断,用于计算制动电流时IR1为最大相，用于计算制动电流2次值；
#define PHASE_B 200
#define PHASE_C 300
#define	R87IALARMTIME		16000/RELAY_SAMP_INTERVAL  //差流越限告警时间 = 10s/RELAY_SAMP_INTERVAL*625(微秒)

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		const LONG TimeSet;			//差动速断的保护时间定值，为0
		
		LONG 	*dwSampBuf;     	//采样计算缓冲区 
		LONG 	*dwRecBuf;			//合成采样点数据缓冲区，用于合成数据的录波 		
		LONG	*dwPickDataBuf;		//突变量启动采样点数据缓冲区
		LONG	*dwComposeBuf;		//合成通道数据缓冲区
		LONG	*dwLineUDataBuf;	//线电压合成通道数据缓冲区
		LONG    *dwPDDataBuf;		//功率方向记忆缓冲区
		BYTE  	RelCFGCLR;			//保护配置退出，例如过流III段无抵押闭锁功能，则赋该值
		BYTE  	RelCFGSET;			//保护配置投入，例如过流III段无抵押闭锁功能，则赋该值 
		DWORD	dwChanDataMask;		//采样通道数据掩码
		DWORD   dwComposeDataMask;
		DWORD	dwDCDataMask;		//数字量通道数据掩码
		
		LONG	IDADrift[5];		//差流A零漂值，连续保存4个周波
		LONG	IDBDrift[5];		//差流B零漂值，连续保存4个周波
		LONG	IDCDrift[5];		//差流C零漂值，连续保存4个周波
		LONG    lSampData1;
		LONG    lSampData2;
		LONG    lSampDate3;
		LONG 	lPickValue;
		WORD	PickCheck;
		WORD	PickCheckNext[14];
		LONG	lPickUpValueArray[14];

		WORD	DCMacroArray[65];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*4];	//数字录波通道数据
		LONG	DCCaluResult[4];						//数字录波继电器组计算值

		//--------------------计算合成通道时系数矩阵行数定位-----------------
		LONG	CoefCorH[9];		//高压侧计算差动电流及制动电流时所乘校正系数		
		LONG	CoefCorM[9];		//高压侧计算差动电流及制动电流时所乘校正系数
		LONG	CoefCorL[9];		//高压侧计算差动电流及制动电流时所乘校正系数

		TRelElecValPar			UA;		
		TRelElecValPar			UB;
		TRelElecValPar			UC;
		TRelElecValPar			UAB;		
		TRelElecValPar			UBC;
		TRelElecValPar			UCA;
		TRelElecValPar			U2H;		//高压侧负序电压
		TRelElecValPar			UHmax_ph; 			//失压保护用的电压，取相电压最大值
		TRelElecValPar			UHmin_li; 			//用于高过流复压闭锁，取线电压最小值

		TRelElecValPar			IHA;			//高侧A相电流
		TRelElecValPar			IHB;			//高侧B相电流
		TRelElecValPar			IHC;			//高侧C相电流
		TRelElecValPar			IMA;			//中侧A相电流
		TRelElecValPar			IMB;			//中侧B相电流
		TRelElecValPar			IMC;			//中侧C相电流
		TRelElecValPar			ILA;			//低侧A相电流
		TRelElecValPar			ILB;			//低侧B相电流
		TRelElecValPar			ILC;			//低侧C相电流

		TRelElecValPar			U0;				//零序电压
		TRelElecValPar			I0;				//零序电流
		TRelElecValPar			IJX;			//间隙电流
		
		
		TRelElecValPar			IHmax; 			//高压侧最大相电流
		TRelElecValPar			IMmax; 			//高压侧最大相电流
		TRelElecValPar			ILmax; 			//高压侧最大相电流
		WORD					wHIPhase;       //相别判断，表明高压侧过流保护元件中使用的相别

		TRelElecValPar			IRA_1;			//A相制动电流中间值1
		TRelElecValPar			IRA_2;			//A相制动电流中间值2
		TRelElecValPar			IRA_3;     		//A相制动电流中间值3
		TRelElecValPar			IRB_1;
		TRelElecValPar			IRB_2;
		TRelElecValPar			IRB_3;
		TRelElecValPar			IRC_1;
		TRelElecValPar			IRC_2;
		TRelElecValPar			IRC_3;
		WORD					wIRAPhase;		//取最大制动电流，用于计算制动电流二次谐波
		WORD					wIRBPhase;		//
		WORD					wIRCPhase;		//

		TRelElecValPar			IDA;			//A相差流
		TRelElecValPar			IDB;			//B相差流
		TRelElecValPar			IDC;			//C相差流
		TRelElecValPar			IRA;			//A相制动电流
		TRelElecValPar			IRB;			//B相制动电流
		TRelElecValPar			IRC;     		//C相制动电流
		TRelElecValPar			IDA2;
		TRelElecValPar			IDA3;
		TRelElecValPar			IDA5;
		TRelElecValPar			IRA2;		
		TRelElecValPar			IDB2;
		TRelElecValPar			IDB3;
		TRelElecValPar			IDB5;
		TRelElecValPar			IRB2;
		TRelElecValPar			IDC2;
		TRelElecValPar			IDC3;
		TRelElecValPar			IDC5;
		TRelElecValPar			IRC2;

		LONG					A_87RPI;		//非周期分量抑制后的差动电流门槛值
		LONG					B_87RPI;		//非周期分量抑制后的差动电流门槛值
		LONG					C_87RPI;		//非周期分量抑制后的差动电流门槛值
		LONG					IDAH2;			//非周期分量抑制后的A相差流二次谐波
		LONG 					IDBH2;			//非周期分量抑制后的B相差流二次谐波
		LONG					IDCH2;			//非周期分量抑制后的C相差流二次谐波
		TRelGeneOvValPar 		R87U_A;  		//A相差动速断
		TRelGeneOvValPar	 	R87U_B; 		//B相差动速断
		TRelGeneOvValPar 		R87U_C;			//C相差动速断
		TRel87R					R87R_A;			//A相比率差动
		TRel87R					R87R_B;			//B相比率差动
		TRel87R					R87R_C;			//C相比率差动
		TRelGeneOvValPar 		R50IH1;  		//高压侧1段过流
		TRelGeneOvValPar	 	R50IH2; 		//高压侧2段过流
		TRelGeneOvValPar 		R50IM1;			//中压侧1段过流
		TRelGeneOvValPar 		R50IM2;			//中压侧2段过流
		TRelGeneOvValPar 		R50IL1;			//低压侧1段过流
		TRelGeneOvValPar 		R50IL2;			//低压侧2段过流
		TRelGeneOvValPar 		R50OL1;			//过负荷1段
		TRelGeneOvValPar 		R50OL2;			//过负荷2段
		TRelGeneOvValPar 		R50OL3;			//过负荷3段
		TRelGeneUnValPar		Rel27U;			//失压保护
		TRelGeneOvValPar		R59U0;			//零序过压保护
		TRelGeneOvValPar		R50I0;			//零序过流保护
		TRelGeneOvValPar_2		R50IJX;			//间隙过流
		T87IAlarm				R87Alarm;		//差流越限保护元件


		BOOL					HA_PDWithMemoryFlag;	//A相功率方向记忆标志
		BOOL					HB_PDWithMemoryFlag;
		BOOL					HC_PDWithMemoryFlag;

		BOOL 					b68IdH2Flag_A;	//A相差流二次闭锁标志	
		BOOL 					b68IdH2Flag_B;	//B相差流二次闭锁标志	
		BOOL 					b68IdH2Flag_C;	//C相差流二次闭锁标志	
		BOOL 					b68IdH5Flag_A;	//A相差流5次闭锁标志	
		BOOL 					b68IdH5Flag_B;	//B相差流5次闭锁标志	
		BOOL 					b68IdH5Flag_C;	//C相差流5次闭锁标志			
		BOOL 					b68IrH2Flag_A;	//A相制动电流2次闭锁标志	
		BOOL 					b68IrH2Flag_B;	//B相制动电流2次闭锁标志	
		BOOL 					b68IrH2Flag_C;	//C相制动电流2次闭锁标志
		BOOL 					b68IdHMulFlag_A; //A相综合谐波闭锁标志
		BOOL 					b68IdHMulFlag_B;//B相综合谐波闭锁标志
		BOOL 					b68IdHMulFlag_C;//C相综合谐波闭锁标志

		BOOL 				FaultOnFlag;				//进入故障循环处理标志 
		BOOL				FaultCheckOutFlag;			//故障循环处理检出故障标志 
		BOOL 				HaveMakeStartRptFlag;		//作启动报告标志 
		BOOL				m_bFaultLKFlag;				//故障起始时刻时间锁定标志，用于突变量启动	
		BOOL				m_bLastRecStatusFlag;		//上次录波继电器字状态
		BOOL				m_bIsLogicOk;		

		BOOL				m_bHaveTripFailedRpt_H;		//已做高压侧断路器跳闸失败报告
		BOOL				m_bHaveTripFailedRpt_M;		//已做中压侧断路器跳闸失败报告
		BOOL				m_bHaveTripFailedRpt_L;		//已做低压侧断路器跳闸失败报告
		BOOL				m_b52BFRecoverFLAG_H;		//高压侧断路器跳闸失败恢复标志
		BOOL				m_b52BFRecoverFLAG_M;		//中压侧断路器跳闸失败恢复标志
		BOOL				m_b52BFRecoverFLAG_L;		//低压侧断路器跳闸失败恢复标志
		BOOL 				m_b52BFHFlag;				//已检出高侧断路器跳闸失败标志
		BOOL 				m_b52BFMFlag;				//已检出中侧断路器跳闸失败标志
		BOOL 				m_b52BFLFlag;				//已检出低侧断路器跳闸失败标志
		
		DWORD				FaultSINum;					//故障处理采样间隔计数器 
		DWORD				FaultLockTime;				//故障锁定定时器 
		WORD 				PickUpRW;					//模值或者突变量启动继电器字 
		TAbsTime 			PickUpTime;					//模值启动或者突变量启动时间定时器 

		
		WORD				RptSerialNumOfAct;			//动作报告序列号
		WORD				RptSerialNumOfStart;		//带故障电量启动报告序列号

		BOOL 				m_bLogicChanged;            //可编程逻辑变化标志
		DWORD				m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器
		//------------PT断线检测部分--------------
		LONG				U2Set;				//PT断线检测负序电压
		DWORD				HPTCheckTimer1;		
		DWORD				HPTCheckTimer2;
		DWORD				HPTCheckTimer3;
		DWORD				HPTCheckTimer4;
		BOOL				HPTMakeReportFlag;
		DWORD 				m_PTCheckTimer;  


private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitR87U_A(void);
		void InitR87U_B(void);
		void InitR87U_C(void);
		void InitR87R_A(void);
		void InitR87R_B(void);
		void InitR87R_C(void);
		void InitR50IH1(void);
		void InitR50IH2(void);
		void InitR50IM1(void);
		void InitR50IM2(void);
		void InitR50IL1(void);
		void InitR50IL2(void);
		void InitR50OL1(void);
		void InitR50OL2(void);
		void InitR50OL3(void);
		void InitR27U(void);
		void InitR59UN(void);
		void InitR50IN(void);
		void InitR50I_JX(void);
		void InitR87IAlarm(void);
		void InitStructCheck(void);		
		BOOL WriteComposeBuf();

		void ResetIdHLock();
		void R68IdH2M(void);		//二次谐波最大相闭锁
		void R68IdH2C(void);		//二次谐波综合相闭锁
		void R68IdH2I(void);		//二次谐波分相闭锁
		void R68IrH2M(void);		//制动电流二次谐波抑制
		void R68IdH5(void);			//五次谐波闭锁
		void R68IdHI(void);			//综合谐波闭锁
		void R68IdAped(void);		//非周期分量抑制
		
		
		void Process52BF(void);
		void FaultProcess(void);												//故障循环处理程序 
		BOOL RelayPickUpCHK(void);												//模值启动判别程序 
		
		void CaluUI(void);
		void CaluMaxMin(void);
		void CaluR87(void);
		void CaluDrift(void);
		void PTCheck(void);//PT断线判断 
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//保护元件整组复归函数 

		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据
		
		TRelElecValPar CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc); //计算负序电压
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);		
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase);
		TRelElecValPar ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);
		inline LONG MaxAB(LONG par1,LONG par2)
		{
			return (par1>=par2)? par1:par2;
		};
		
		inline LONG MaxABC(LONG x,LONG y,LONG z)
        {
             return (x>y?(x>z?x:z):(y>z?y:z));
        };	

};		

void RelayTask(DWORD *pdwApp);


#endif
