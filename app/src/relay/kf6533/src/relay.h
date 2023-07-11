
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			26 				//需自检的结构体的个数
#define PHASE_A 100
#define PHASE_B 200
#define PHASE_C 300
#define	R87IALARMTIME		16000/RELAY_SAMP_INTERVAL  //差流越限告警时间 = 10s/RELAY_SAMP_INTERVAL*625(微秒)

class CRelayTask:public CApp
{
   DECLARE_MESSAGE_MAP()
public:   

		LONG 	*dwSampBuf;     		//采样计算缓冲区 
		LONG	*dwPickDataBuf;			//突变量启动采样点数据缓冲区
		LONG	*dwComposeDataBuf;		//合成数据缓冲区
		LONG	*dwLineUDataBuf;		
		LONG	*dwPDDataBuf;
		LONG	*dwRecBuf;
		
		BYTE  	RelCFGCLR;				//保护配置退出，例如过流III段无抵押闭锁功能，则赋该值 
		BYTE	RelCFGSET;				//保护配置退出
		DWORD	dwChanDataMask;			//采样通道数据掩码
		DWORD	dwDCDataMask;			//数字量通道数据掩码	
		DWORD	dwComposeMask;

		LONG    lSampData1;
		LONG    lSampData2;
		LONG    lSampDate3;
		LONG 	lPickValue;		
		WORD	PickCheck;
		WORD	PickCheckNext[11];		
		TAbsTime 			PickUpTime;		//模值启动或者突变量启动时间定时器 
		
		const LONG TimeSet;			//差动速断的保护时间定值，为0
		BOOL 	b68IdH2Flag_A;	//A相差流二次闭锁标志	
		BOOL 	b68IdH2Flag_B;	//B相差流二次闭锁标志	
		BOOL 	b68IdH2Flag_C;	//C相差流二次闭锁标志	
		BOOL 	b68IdH5Flag_A;	//A相差流5次闭锁标志	
		BOOL 	b68IdH5Flag_B;	//B相差流5次闭锁标志	
		BOOL 	b68IdH5Flag_C;	//C相差流5次闭锁标志			
		BOOL 	b68IrH2Flag_A;	//A相制动电流2次闭锁标志	
		BOOL 	b68IrH2Flag_B;	//B相制动电流2次闭锁标志	
		BOOL 	b68IrH2Flag_C;	//C相制动电流2次闭锁标志
		BOOL 	b68IdHMulFlag_A; //A相差流综合谐波闭锁标志
		BOOL 	b68IdHMulFlag_B;	//B相差流综合谐波闭锁标志
		BOOL 	b68IdHMulFlag_C;	//C相差流综合谐波闭锁标志
		
        WORD	DCMacroArray[97];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//数字录波通道数据
		LONG	DCCaluResult[6];						//数字录波继电器组计算值



		TRelElecValPar			UA;
		TRelElecValPar			UB;
		TRelElecValPar			UC;
		TRelElecValPar			U0h;
		TRelElecValPar			Ua;
		TRelElecValPar			Ub;
		TRelElecValPar			Uc;
		TRelElecValPar			U0l;
		TRelElecValPar			IA;
		TRelElecValPar			IB;
		TRelElecValPar			IC;
		TRelElecValPar			Ia;
		TRelElecValPar			Ib;
		TRelElecValPar			Ic;
		TRelElecValPar			I0;
		TRelElecValPar			Ijx;
		TRelElecValPar			U2h;		//高压侧负序电压
		TRelElecValPar			U2l;		//低压侧负序电压
		TRelElecValPar			UAB;
		TRelElecValPar			UBC;
		TRelElecValPar			UCA;
		TRelElecValPar			Uab;
		TRelElecValPar			Ubc;
		TRelElecValPar			Uca;
		
		TRelElecValPar			Ida;		//A相差流
		TRelElecValPar			Idb;
		TRelElecValPar			Idc;
		TRelElecValPar			Ira;		//A相制动电流
		TRelElecValPar			Irb;
		TRelElecValPar			Irc;
		TRelElecValPar			Ida2;
		TRelElecValPar			Ida3;
		TRelElecValPar			Ira2;
		TRelElecValPar			Idb2;
		TRelElecValPar			Idb3;
		TRelElecValPar			Irb2;
		TRelElecValPar			Idc2;
		TRelElecValPar			Idc3;
		TRelElecValPar			Irc2;

		TRelElecValPar			IHmax; //高压侧最大相电流
		TRelElecValPar			ILmax; //低压侧最大相电流
		TRelElecValPar			UHmax; //高压侧最大相电压
		TRelElecValPar			ULmax; //低压侧最大相电压
		TRelElecValPar			UHmin;
		TRelElecValPar			ULmin;
		TRelElecValPar			U_27U;  //失压保护用的电压，在高压侧三相时为三相最大电压，高压侧两相时为两相最小电压
		
		LONG					dw49IA;
		LONG					dw49IB;
		LONG					dw49IC;
		


		TRelGeneOvValPar		Rel87U_A;		//A差动速断
		TRelGeneOvValPar		Rel87U_B;		//B差动速断
		TRelGeneOvValPar		Rel87U_C;		//C差动速断		
		TRel87R					Rel87R_A;		//A比率差动
		TRel87R					Rel87R_B;		//B比率差动
		TRel87R					Rel87R_C;		//C比率差动		
		T87IAlarm				R87Alarm;		//差流越限保护元件
		TRelGeneOvValPar		Rel50I_H1;		//高压侧过流1段；
		TRelGeneOvValPar		Rel50I_H2;		//高压侧过流2段；
		TRelGeneOvValPar		Rel50I_H3;		//高压侧过流3段
		TRelGeneOvValPar		Rel50I_L1;		//低压侧过流1段
		TRelGeneOvValPar		Rel50I_L2;		//低压侧过流2段
		TRelGeneOvValPar		Rel50I_SH;		//母线充电保护
		TRelGeneOvValPar		Rel50I_N1;		//零序过流1段
		TRelGeneOvValPar		Rel50I_N2;		//零序过流2段
		TRelGeneOvValPar		Rel59U_NH;		//高压侧零序过压
		TRelGeneOvValPar		Rel59U_NL;		//低压侧零序过压
		TRelGeneOvValPar		Rel50OL_1;		//过负荷1段
		TRelGeneOvValPar		Rel50OL_2;		//过负荷2段
		TRelGeneOvValPar		Rel50OL_3;		//过负荷3段
		TRelGeneOvValPar_2		Rel50I_JX;		//间隙过流保护
		TRelGeneUnValPar		Rel27U;			//失压保护
		TThermalOverLoad		Rel49IA;			//反时限热过负荷保护
		TThermalOverLoad		Rel49IB;			//反时限热过负荷保护
		TThermalOverLoad		Rel49IC;			//反时限热过负荷保护

		TRelGeneOvValPar		Rel50OL_A;		//过负荷告警
		
		
		BOOL				TripFailFlag;					//跳闸失败标志 
		BOOL 				FaultOnFlag;					//进入故障循环处理标志 
		BOOL				FaultCheckOutFlag;			//故障循环处理检出故障标志 
		BOOL 				HaveMakeStartRptFlag;		//作启动报告标志 
		BOOL				m_bFaultLKFlag;				//故障起始时刻时间锁定标志，用于突变量启动
		BOOL				m_bLastRecStatusFlag;		//上次录波继电器字状态
		BOOL				m_bHaveMakeTripFailRpt;

		BOOL				HA_PDWithMemoryFlag;
		BOOL				HB_PDWithMemoryFlag;
		BOOL				HC_PDWithMemoryFlag;
		BOOL				LA_PDWithMemoryFlag;
		BOOL				LB_PDWithMemoryFlag;
		BOOL				LC_PDWithMemoryFlag;


		
		DWORD				FaultSINum;					//故障处理采样间隔计数器 
		DWORD				FaultLockTime;				//故障锁定定时器 
		WORD 				PickUpRW;					//模值或者突变量启动继电器字 
		WORD				HPFRW; 						//功率方向正方向继电器字
		WORD				HPRRW; 						//功率方向负方向继电器字		
		WORD				LPFRW; 						//功率方向正方向继电器字
		WORD				LPRRW; 						//功率方向负方向继电器字
		WORD				wHIPhase;                     //相别判断，表明高低压侧过流保护元件中使用的相别
		WORD				wLIPhase;                     //相别判断，表明高低压侧过流保护元件中使用的相别

		
		WORD				RptSerialNumOfAct;			//动作报告序列号
		WORD				RptSerialNumOfStart;		//带故障电量启动报告序列号
		WORD				RptSerialNumOf27U;			//低压保护报告序列号
		BOOL				m_bIsLogicOk;

		BOOL				m_b49IHaveMakeWarnRpt; 		//反时限热过负荷保护用的做报告标志
		BOOL				m_b49IHaveMakeActRpt;			
		BOOL				m_b49IHaveMakeRSTRpt;	
		BOOL				m_b52BFFLAG;




		LONG				U2Set;
		DWORD				HPTCheckTimer1;
		DWORD				HPTCheckTimer2;
		DWORD				HPTCheckTimer3;
		DWORD				HPTCheckTimer4;
		BOOL				HPTMakeReportFlag;

		DWORD				LPTCheckTimer1;
		DWORD				LPTCheckTimer2;
		DWORD				LPTCheckTimer3;
		DWORD				LPTCheckTimer4;
		BOOL				LPTMakeReportFlag;
		BOOL				m_bLogicChanged;
		DWORD				m_dwWaitLogicCounter;
		DWORD 				m_PTCheckTimer;
		
private:


public:
		
	    void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);		
		void InitStructCheck(void);		//初始化保护元件结构校验数组

		void InitRel87U_A(void);		//A差动速断
		void InitRel87U_B(void);		//B差动速断
		void InitRel87U_C(void);		//C差动速断		
		void InitRel87R_A(void);		//A比率差动
		void InitRel87R_B(void);		//B比率差动
		void InitRel87R_C(void);		//C比率差动
		void InitRel50I_H1(void);		//高压侧过流1段；
		void InitRel50I_H2(void);		//高压侧过流2段；
		void InitRel50I_H3(void);		//高压侧过流3段
		void InitRel50I_L1(void);		//低压侧过流1段
		void InitRel50I_L2(void);		//低压侧过流2段
		void InitRel50I_SH(void);		//母线充电保护
		void InitRel50I_N1(void);		//零序过流1段
		void InitRel50I_N2(void);		//零序过流2段
		void InitRel59U_NH(void);		//高压侧零序过压
		void InitRel59U_NL(void);		//低压侧零序过压
		void InitRel50OL1(void);		//过负荷1段
		void InitRel50OL2(void);		//过负荷2段
		void InitRel50OL3(void);		//过负荷3段
		void InitRel50IJX(void);		//间隙过流保护
		void InitRel27U(void);			//失压保护
		void InitRel49I(void);			//反时限热过负荷保护
		void InitR87IAlarm();

		void InitRel50OLA(void);		//过负荷告警

		void ResetIdHLock(void);
		void R68IdH2M(void);		//二次谐波最大相闭锁
		void R68IdH2C(void);		//二次谐波综合相闭锁
		void R68IdH2I(void);		//二次谐波分相闭锁
		void R68IrH2M(void);		//制动电流二次谐波抑制
		void R68IdHI(void);		//综合谐波抑制
		void FalutProcess(void);									//故障循环处理程序 
		BOOL RelayPickUpCHK(void);									//保护启动判别程序 
		void Process52BF(void);
		void ComposeData(void);
		void CaluUI(void);
		void CaluR87(void);//计算差动电流，制动电流
		void Calu49I(void); //计算反时限过负荷电流
		void Calu47U(void);//计算复合电压闭锁相关电压量
		void CaluMaxMin(void);
		TRelElecValPar CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc); //计算负序电压
		void PTCheck(void);//PT断线判断 
		void InitFalutHAB(void);//高压侧两相投入时，差动电流等故障电量置零
//		void InitRepSerialNum(void);//初始化保护动作报告。
		
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);										//保护元件整组复归函数 
		void ThermalOverLoad();
		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据

		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);		
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase);
		TRelElecValPar ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);
		
        //求三相电流或电压中最大值
		inline LONG MAX(LONG x,LONG y,LONG z)
        {
             return (x>y?(x>z?x:z):(y>z?y:z));
        };	
		
        //求两相电压中最小值
		inline LONG MIN(LONG x,LONG y)
        {
             return (x<y?x:y);
        };
	
};

void RelayTask(DWORD *pdwApp);


#endif
