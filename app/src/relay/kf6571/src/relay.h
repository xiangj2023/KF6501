
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#include "channel.h"

#define CHECK_NUM			1 
#define RPT_NULL 65535

#define	T_R			0x200  	//TR短路故障
#define	F_R			0x201	//FR短路故障
#define	T_F			0x202	//TF短路故障
#define	U_D			0x203	//相间短路故障

#define	RANGE1		0x210	//第一区间故障
#define	RANGE2		0x211	//第二区间故障
#define	RANGE3		0x212	//第三区间故障
#define	RANGE4		0x213	//第四区间故障
#define	RANGE5		0x214	//第五区间故障

#define	UPLINE		0x220	//上行
#define	DOWNLINE	0x221	//下行

//用于记录和判断3个电流的大小，及备用电流是备用哪一路的标志
#define I1I2I3		0x300   //I1>I2>I3
#define I1I3I2		0x301	//I1>I3>I2
#define I3I1I2		0x302	//I3>I1>I2
#define I3I2I1		0x303	//I3>I2>I1
#define I2I1I3		0x304	//I2>I1>I3
#define I2I3I1		0x305	//I2>I3>I1

#define LOCKSSDATANUM FLCOMPLICATENUM+1

#define FLMODE_NULL   0x400   //未定义的模式
#define FLMODE_0   0x401
#define FLMODE_1   0x402
#define FLMODE_2   0x403
#define FLMODE_3   0x404
#define FLMODE_4   0x405
#define FLMODE_5   0x406
#define FLMODE_6   0x407
#define FLMODE_7   0x408
#define FLMODE_8   0x409
#define FLMODE_9   0x410
#define FLMODE_10   0x411
#define FLMODE_11   0x412
#define FLMODE_12   0x413

enum RelayType{Relay50,Relay21,Relay50D1,Relay50D2};

















typedef struct LOCKSSDATA{

	TSubData subData;	
	TRelElecValPar			lockU1; 
	TRelElecValPar			lockU2;
	TRelElecValPar			lockU3;
	TRelElecValPar			lockU4;
	TRelElecValPar			lockIT1;
	TRelElecValPar			lockIF1;
	TRelElecValPar			lockIT2;
	TRelElecValPar			lockIF2;
	TRelElecValPar			lockIT3;
	TRelElecValPar			lockIF3;
	TRelElecValPar			lockIAT1;
	TRelElecValPar			lockIAT2;		
	TRelElecValPar			lockUp1;
	TRelElecValPar			lockUp2;
	TRelElecValPar			lockIp1;
	TRelElecValPar			lockIp2;
	TRelElecValPar			lockIp3;
	TRelElecValPar			lockImpd;
	TRelElecValPar			lockImpd_T;
	TRelElecValPar			lockImpd_F;

	WORD					exchangeFlag;
	RelayType               relaytype;
	
}SLockSSData;

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		DWORD   dwDataPtr;

		BYTE	RelCFGCLR;
		BYTE 	RelCFGSET;
		WORD    wFaultSINum;
		DWORD	dwChanDataMask;									//采样通道数据掩码
		DWORD   dwComposeDataMask;			
		DWORD	dwInCre1DataMask;	//增量保护计算所用的通道数据掩码
		DWORD	dwInCre2DataMask;	//增量保护计算所用的通道数据掩码
		LONG	*dwComposeBuf;   //合成数据缓冲区
		LONG 	*dwSampBuf;     	//采样计算缓冲区 	
		LONG    *dwATSingleBuf;		//变电所无AT时，上下行单个AT吸上电流合成
		LONG	*dwComposeUBuf;
		LONG	*dwRecBuf;		
		LONG 	*dwIncre1CurBuf;		//增量1保护历史采样点数据缓冲区		
		LONG 	*dwIncre2CurBuf;		//增量1保护历史采样点数据缓冲区 
		LONG	*dwTraceDataBuf;
		LONG 	*dwCCModeDataBuf;	//电流合成方式，合成通道数据缓冲区
		LONG 	*dwSubCCModeDataBuf; //电流合成方式，当主所召唤数据时,子所合成数据缓冲区
		DWORD	dwCTRateCoef1;		//电流合成方式，合成通道系数折算系数,下行
		DWORD	dwCTRateCoef2;		//电流合成方式，合成通道系数折算系数,上行
		
		DWORD	dwDCDataMask;							//数字量通道数据掩码		
		WORD	DCMacroArray[33];						//数字录波继电器字组
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*2];	//数字录波通道数据
		LONG	DCCaluResult[2];						//数字录波继电器组计算值		
		BOOL	m_bIsLogicOk;				//逻辑方程初始化成功标志
		BOOL	m_bLastRecStatusFlag;       //可编程录波触发标志
		BOOL 	m_bStartHandOverFlag;		//启动越区切换命令标志
		BOOL	m_bRecoverHandOverFlag;		//恢复越区切换命令标志
		WORD	m_SSPSelect;



		LONG				m_lOtherRelayTrip;		//其他保护动作，除增量保护外


		
		TRelElecValPar 			m_U1;                   //计算用本所电量
		TRelElecValPar 			m_U2;
		TRelElecValPar 			m_U3;
		TRelElecValPar 			m_U4;
		TRelElecValPar			m_ULock;	//过流保护低压闭锁用的电压
		TRelElecValPar 			m_IT1;
		TRelElecValPar 			m_IF1;
		TRelElecValPar 			m_IT2;
		TRelElecValPar 			m_IF2;
		TRelElecValPar 			m_IT3;
		TRelElecValPar 			m_IF3;
		TRelElecValPar 			m_IAT1;
		TRelElecValPar 			m_IAT2;
		TRelElecValPar 			m_UP1;
		TRelElecValPar 			m_UP2;
		TRelElecValPar 			m_U13;
		TRelElecValPar 			m_U24;
		TRelElecValPar 			m_IP1;
		TRelElecValPar 			m_IP2;
		TRelElecValPar 			m_IP3;
		TRelElecValPar			m_IP1_h2;
		TRelElecValPar			m_IP1_h3;
		TRelElecValPar			m_IP1_h5;
		TRelElecValPar			m_IP2_h2;
		TRelElecValPar			m_IP2_h3;
		TRelElecValPar			m_IP2_h5;
		TRelElecValPar			m_IP3_h2;
		TRelElecValPar			m_IP3_h3;
		TRelElecValPar			m_IP3_h5;		
		TRelElecValPar			m_UP;     //用来做距离启动、过流启动、增量启动的电量
		TRelElecValPar			m_IP;
		TRelElecValPar			m_Impd;
		TRelElecValPar			m_IP_h2;
		TRelElecValPar			m_IP_h3;
		TRelElecValPar			m_IP_h5;
		TRelElecValPar			m_I1_AT;   //本所的馈线1合成吸上电流
		TRelElecValPar 			m_I2_AT;   //本所的馈线2合成吸上电流
		TRelElecValPar 			m_I3_AT;   //本所的馈线3合成吸上电流
		TRelElecValPar 			m_I_AT;   //本所的吸上电流
		LONG					K35;
		WORD					RW37IP;

		//保护动作元件定义
		TRel50DIPar				Rel50D1;
		TRel50DIPar				Rel50D2;
		TRel21Par				Rel21;
		TRelGeneOvValPar		Rel50;		
		TRelFaultLocatePar 		T_FaultLocate;
		TRelFaultLocatePar		F_FaultLocate;
		TRelFaultLocatePar		TF_FaultLocate;
		//用于增量保护判断的中间电量，根据备用电流的情况进行赋值
		TRelElecValPar  		m_I50D1_0;
		TRelElecValPar  		m_I50D1_2;
		TRelElecValPar  		m_I50D1_3;
		TRelElecValPar  		m_I50D1_5;
		TRelElecValPar  		m_I50D2_0;
		TRelElecValPar  		m_I50D2_2;
		TRelElecValPar  		m_I50D2_3;
		TRelElecValPar  		m_I50D2_5;
		//一次值
		LONG					m_U1mod;				//本所电量模值
		LONG					m_U2mod;
		LONG					m_U3mod;
		LONG					m_U4mod;
		LONG					m_IT1mod;
		LONG					m_IF1mod;
		LONG					m_IT2mod;
		LONG					m_IF2mod;
		LONG					m_IT3mod;
		LONG					m_IF3mod;
		LONG					m_IAT1mod;
		LONG					m_IAT2mod;
		LONG					m_UP1mod;
		LONG					m_UP2mod;
		LONG					m_U13mod;
		LONG					m_U24mod;
		LONG					m_IP1mod;
		LONG					m_IP2mod;
		LONG					m_IP3mod;
		LONG					m_IAT_SS;//变电所吸上电流一次值
		LONG					m_iat_ss;//变电所吸上电流二次值
		BYTE					m_CB;

		//当本所为子所时，使用以下变量进行计算追溯值
		TRelElecValPar					subU1;		
		TRelElecValPar					subU2;
		TRelElecValPar					subU3;
		TRelElecValPar					subU4;
		TRelElecValPar					subIT1;
		TRelElecValPar					subIF1;
		TRelElecValPar					subIT2;
		TRelElecValPar					subIF2;
		TRelElecValPar					subIT3;
		TRelElecValPar					subIF3;
		TRelElecValPar					subIAT1;
		TRelElecValPar					subIAT2;
		TRelElecValPar					subIAT;
		TRelElecValPar					subIp1;
		TRelElecValPar					subIp2;
		TRelElecValPar					subIp3;

		//以下定义的是用于测距计算的缓冲区
		//锁定的主所数据,其中前4个用于保护的并发，最后一个用于手动召测
		SLockSSData					lkSSData[FLCOMPLICATENUM+1]; 
		//测距计算用的数据，包括主所以及最大4个子所
		TSubData					RelaySubData[FLCOMPLICATENUM][MAX_SUBSTATION_NUM+1];          
		TSubData					ManualSubData[MAX_SUBSTATION_NUM+1];
		TSubData					HistorySubData[MAX_SUBSTATION_NUM+1];
		TSubData					LowVolSubData[MAX_SUBSTATION_NUM+1];
		TSubData					HeartbeatSubData[MAX_SUBSTATION_NUM+1];
		TSubData					*pTSubData;

		
		//各区间的相关定值
		LONG						DistanceSet[MAX_SUBSTATION_NUM];
		LONG						DistCoef[MAX_SUBSTATION_NUM+1];//电流分布系数
		WORD						ChanErrRW[MAX_SUBSTATION_NUM];//子所通道故障继电器字表
		LONG						QValue[MAX_SUBSTATION_NUM+1];


		
		BOOL					bStopRefreshRMFlag;//故障测距启动后停止刷新保护测量值标志
		TAbsTime 				FLPickTime;			//故障测距启动锁定时间
		DWORD					dwFLPickupTime; 	//测距启动时间
		DWORD					dwNoFaultTime; 		//无故障时间
		DWORD					dwFaultLKTime;		//故障动作时间
		TAbsTime 				PickUpTime;			//模值启动或者突变量启动时间定时器 		
		WORD					RptSerialNumOfStart;//带故障电量启动报告序列号
		LONG					*dwPickUpBuf;  		//突变量启动判断用的数据缓冲区
		
		BOOL 					m_bFaultOnFlag;		//进入故障循环处理标志 
		BOOL 					m_bHaveTripFlag;    //故障动作标志
		BOOL					m_bFaultCheckOutFlag;//曾经检出过故障标志
		DWORD					m_dwFaultSINum;
		
		BOOL 					HaveMakeStartRptFlag;		//作启动报告标志 
		BOOL					putoutLedFlag;//灭掉点亮的测距动作LED灯
		
		LONG					lSampData1;
		LONG					lSampData2;
		LONG					lSampDate3;
		LONG					lPickValue;		
		WORD					m_wPickCheck;					//本次保护任务中连续突变量大于定值计数器
		WORD					m_wPickCheckNext[3];			//两次保护任务中连续突变量大于定值计数器
		LONG					m_lPickUpValueArray[3];			//两次保护任务中连续突变量大于定值计数器

		
		BYTE					channelCheckRptSerNo;
		DWORD					manualFLPickTime; //实验测距启动时间
		BOOL					manualFLPickFlag; //实验测距启动标志
		BOOL					manualFLTimeoutFlag; //实验测距超时标志
		BOOL					lastManualRWStatus;
		WORD					manualFLSerialNo;	//实验启动的故障测距报告序列号
		WORD    				RptSerialNumOfAct;  //保护启动的故障测距报告序列号
		BOOL					SubGPSTimeoutFlag;//有子所GPS时钟质量差

		DWORD					faultINF;
		WORD					faultType;//故障类型
		WORD					faultRange;//故障区间
		WORD					faultDirection; //故障方向
		WORD					exchangeElecFlag; //在出现变电所备用馈线投运的情况下，记录下交换状态
		LONG					*pRMHeadAddr;//子所保护测量值首地址
		BOOL					lastVolHaveMark;//上一次低压定值比较情况
		DWORD					lowVolCounter;
		DWORD					lowVolPerTime;
		DWORD					lowVolTimeSet;
		BYTE 					manualDataType; //手动召测时，要召测的数据类型

		BOOL					haveMakeGPSERRRpt; //已经做过GPS故障报告
		BOOL 					m_bLogicChanged;            //可编程逻辑变化标志
		DWORD					m_dwWaitLogicCounter;		//可编程逻辑变化后停止自检用的计数器


		//临时阻抗，用来电抗法测距时赋予主所锁定的阻抗
		TRelElecValPar			m_LImpd_T;
		TRelElecValPar			m_LImpd_F;
		TRelElecValPar			m_LImpd_TF;
		//实时计算的阻抗
		TRelElecValPar			Impd_T;
		TRelElecValPar			Impd_F;
		TRelElecValPar			Impd_TF;




		int   			rpt_F_TR;  //上行TR故障
		int 			rpt_F_FR; //上行FR故障
		int				rpt_B_TR;	//下行TR故障
		int				rpt_B_FR;   //下行FR故障
		int				rpt_TF;     //TF故障
		int				rpt_TimeOut;//超时故障测距失败
		int				rpt_ComTest; //通信测试
		int				rpt_GpsErr; //GPS故障测距失败
		int				rpt_FLFail; //测距失败
		int 			rpt_F_TF; //上行TF故障
		int				rpt_B_TF;	//下行TF故障

		
		WORD 			powerSupplyMode;
		//保护动作元件标识，用于上下行解列后，判断是哪条线路发生的故障
		RelayType      relayType;
private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitRel50D();
		void InitRel21();
		void InitRel50();
		void InitSubInfo();
		void InitRptSN();
		void InitFLIntervalSet(); //初始化测距各区间的相关定值		
		void InitFaultLocate();
		void CaluFaultLocateSetTable(TRelFaultLocatePar*);		
 		void WriteComposeBuf();
		void InitStructCheck(void);		
		void CaluUI(void);
		void CaluImpd(WORD no);
		void CaluKM();//计算公里标
		void WriteRM(void);//向数据库中写入保护测量值
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		void OnLogicChanged(void);
		void OnStepOver(WORD rc);
		void InitDCRecInfo();
		void WriteDCChannel();	//写入数字录波数据
		BOOL RelayPickUpCheck();
		void ChannelCheck();
		void FaultLocate();
		void FLPickUpCheck();
		void FaultLocateAuto();   //保护启动
		void FaultLocateManual(); //手动实验启动
 		void FaultLocateCalu(TSubData *subData,subChannelCheck FLData[],WORD no,BYTE rptSerialNo);//测距计算
		void BoostingCurrent(TSubData *subData);		//吸上电流比
		void UpDownLineCurrent(BYTE rptSerialNo); // 上下行电流比
		void B_LineCurrent(BYTE rptSerialNo);		//吸馈电流比
		BOOL XLineWithSub(TSubData *tsubData,WORD no,BYTE rptSerialNo); //线性电抗法测距
		BOOL XLineWithoutSub(BYTE rptSerialNo);
 		void FLSubTimeout(subChannelCheck FLData[],BYTE rptSerialNo);//召唤子所数据超时分析		
		void LockingSSData(WORD no,WORD FLType,BOOL refreshFlag); //锁定主所电量
		void RefreshSubData(TSubData *subData,subChannelCheck fldata[]);	//更新子所电量
 		void MakeSSFLRpt(WORD no,BYTE rptSerialNo,WORD SSInf); //作变电所测距动作报告
		void MakeSubFLRpt(subChannelCheck FLData[],WORD no,BYTE rptSerialNo,WORD subInfType); //作子所测距动作报告
		void MakeSingleSubFLRpt(WORD subno,DWORD inf,subChannelCheck *data,BYTE rptSerialNo);
		void ResetFoultLocate();		
		void ResetAllRelay();
		void OnGetFLData(WORD arg1,DWORD arg2,DWORD dataType);
		void LockDataForLowVol();//当电压低于一定值时锁定数据
		DWORD GetGPSStatus(STATUS flag,BYTE CB);
		void checkSelfGps();//检查本装置的GPS状态		
 		void HandOverProcess(void);
		
		LONG MAX4(LONG x,LONG y,LONG z,LONG w);
		LONG MAX5(LONG x,LONG y,LONG z,LONG w,long v);
	 	LONG MAX3(LONG x,LONG y,LONG z);		
	 	LONG MIN3(LONG x,LONG y,LONG z);
		LONG MAX2(LONG x,LONG y);
	 	LONG MIN2(LONG x,LONG y);

		WORD CheckPowerSupplyMode(TSubData *tsubData);
		void CalcFLMode_0(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_1(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_2(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_3(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_4(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_5(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_6(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_7(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_8(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_9(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_10(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_11(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_12(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_13(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		//电抗法测距，不需要判断压板配置等投入，在自适应模式下使用
		void CalcFLX(TRelFaultLocatePar *Relay);
		//根据备隔的位置状态，调整电流顺序
		void AdjustCurrent();
		//当上下行解列时，判断是上行发生故障还是下行发生故障
		int  WhichShortcircuit(TSubData *tsubData,WORD no);
		//三个子所的故障测距，不进行自适应判断，采用固定模式
		void CalcFLWith3Sub(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		//越区测距，不进行自适应判断，采用固定模式
		void CalcFLWithHandOver(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		//判断两个电流的模值是否近似相等
		BOOL IsAEqualB(LONG A, LONG B);
		//本所为子所时，向心跳数据缓冲区中写入实时数据
		void RefreshHeartData();
		//开出自检
		void BOCheck();
		// TF故障时故障方向的判断
		WORD TFFaultDir(TSubData *tsubData);

};		

void RelayTask(DWORD *pdwApp);


#endif
