/************************************************************************************************/
/*CopyRight:																						*/
/*						天津凯发电气股份有限公司													*/
/*Description:																						*/
/*						保护用的数据结构定义及宏定义														*/
/*Modification history:																				*/
/*						创建：	yanzh 	08/03/21												*/
/*																								*/
/************************************************************************************************/



#ifndef _RELAY_DEFS_H_
#define _RELAY_DEFS_H_
#include "sysdefs.h"
#include "timedef.h"
//#include "relay.h"

#define MAX_SUBSTATION_NUM 	5		//测距最多5个子站
#define FLCOMPLICATENUM		4		//测距最大并发数

#define	CTRATIO 150
#define	STANDARDCOEF (23170/CTRATIO)    //23170=32768/sqrt(2)
#define	MULTIPLES 2			//突变量启动时计算的采样点值的放大倍数
#define ToAbs(val) (((val)>=0)? ((val)):(-(val)))    					//求绝对值 
#define RELAY_OFF 			0     							//继电器状态字 退出 
#define RELAY_ON  			1							//继电器状态字 投入 

#define TANG85	 			(1143)						//85度的正切值 
#define MAXINVERSETIME		100  						 //反时限保护动作最大时间所需的累加计算次数 100秒 
#define INVERSECOEFA 		28000						//一般反时限累加量定值计算系数 
#define INVERSECOEFB 		1350						//极反时限累加量定值计算系数 
#define INVERSECOEFC 		80							//甚反时限累加量定值计算系数 
#define MAXFLNUM			10							//故障测距最大段数 
#define NOCURCOEF			90							//判定保护跳闸后的无流系数 
#define RECOVERVOLCOEF		110							//欠量保护电量恢复系数 
#define IMPDMULTIPLE   		2000 						//阻抗放大倍数 
#define TANGMULTIPLE		100							//正切表值放大倍数 
#define SINWTS				1248					// 32*100*2*1000*SIN(wTs)即6400*SIN(11.25°)
#define NUMOFYCVALUE		9						//计算遥测值个数，用于取中值写入数据库，必须为奇数
//#define	INHERENTTIME		40						//保护动作时间修正值
//#define	INHERENTTIME		40   //保护动作的固有时间
#define FREQ_TYPE_PRO		0    //装置类型为保护装置
#define FREQ_TYPE_MEA		1	 //装置类型为测控装置
#define BO_CHECK_BUF_LEN	2	 //定义开出自检状态值保存数组的大小

#define MAX_FREQUENCE  56000  //软件测频最大有效频率
#define MIN_FREQUENCE  44000	 //软件测频最小有效频率
#define MIN_TESTU      1610  //10V电压门槛值

#define BO_BREAK_CNT		5	//每路开出自检次数

#define INVALIDRX 3276
#define ZERODRIFT   0
#define FULLVALUE   1

#define NORMALPICK  0x55  //正常保护入段
#define ACCPICK		0xAA	//加速保护入段

extern WORD AngleTab[];
extern LONG InverseCurCoefA[20][3];
extern LONG InverseCurCoefB[];
extern LONG InverseCurCoefC[];
extern WORD CosAngleTab[];
extern WORD SinAngleTab[];
extern WORD TangAngleTab[];
extern WORD VolHarmCal[];
extern WORD CurHarmCal[];
extern WORD CurHarmCal_6570_I[];
extern WORD VolHarmCal_6570_U[];
extern WORD CurHarmCal_6571_I[];
extern WORD VolHarmCal_6571_U[];

extern LONG MatrixCoef[8][9];
extern LONG MartixVV3[3][9];
extern LONG MartixVV1[3][9];
extern LONG MartixAT2[3][6];
extern LONG Matrix6533[9];
extern LONG Martix6535[13][9];
extern LONG Martix6535_AB[3][9];
extern LONG WCT6535[3][3];
extern WORD CRCTab[];
/*
typedef struct RELAYINFO{
	char RelayType[16];
	WORD nBI;
	WORD nBO;
	WORD nAI;
	WORD npAI;
	WORD nTO;
	WORD bak;
}TRelayInfo;
*/
/********************************************************
* 名称:		TLoadSupV
* 描述:		负荷监视电量结构
* 作者: 
* 日期:
* 备注:
********************************************************/
typedef struct TLOADSUPERVISION {
	LONG Voltage;
	LONG Current;
	LONG AngleVolt;
//	LONG AngleCurr;		//固定电流角度为0度，电压角度以电流角度为基准
	TAbsTime Time;
}TLoadSupV;

/********************************************************
* 名称:		TRelElecValPar
* 描述:		通用电量描述结构
* 作者: 
* 日期:
* 备注:
********************************************************/
typedef struct TREL_ELECVALUE_PAR {	
	LONG	Real;				//实部 
	LONG	Imag;				//虚部 
	LONG	Mod;             			//模值 
	LONG	Angle;			      //相角 
}TRelElecValPar;

/********************************************************
* 名称:		TRelSimpGeneOvUnPar
* 描述:		通用的简单过量、欠量保护元件参数结构
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TREL_SIMPLEGENERALOVUN_PAR{
	const LONG			*pEleSet;						//保护定值指针 
	TRelElecValPar			*pTEleVal;						//电量指针 
	WORD				ACTRelayWord;					//动作继电器字 
	WORD				StartRelayWord;					//入段继电器字
	WORD				wExcepWordNum;					//本结构对象计算CRC校验时需排除在外的字节数，本结构为2 
}TRelSimpGeneOvUnPar;
/********************************************************
* 名称:		TRelGeneOvValPar
* 描述:		通用过量保护元件参数结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TREL_GENERELOVERVALUE_PAR{
	const LONG		*pElecSet;          						//过量保护电量定值 
	const LONG		*pTimeSet;				 			//过量保护时间定值 
	const LONG		*pHarmKhr1Set;						//过量保护谐波加权系数 
	const LONG		*pVolLockSet;				 	    	//过量保护电压闭锁定值 
	const LONG		*pAccActTimeSet;						//过量保护后加速动作时间定值 
	const LONG		*pH2Coef;							//谐波闭锁系数、抑制系数的放大倍数 

	const BYTE		*pRelCFG;							//过量保护功能控制字--1:投入，2:退出  
	const BYTE		*pRelSW;         						//过量保护软压板------1:投入，2:退出  
	const BYTE		*pRelHLockCFG;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	const BYTE		*pRelHSuprCFG; 						//谐波抑制功能控制位--1:投入，2:退出 
	const BYTE		*pRelPDCFG;		    					//功率方向功能控制位--1:投入，2:退出 
	const BYTE		*pRelVolLockCFG;						//低压闭锁功能控制位--1:投入，2:退出  
	const BYTE		*pRelFDCFG;							//过量保护正方向功能控制位----1:投入，2:退出 
	const BYTE		*pRelAccCFG;							//有无后加速功能------1:有，  2:无 
	const BYTE      *pRel47UCFG;						//复压闭锁功能控制位
	const BYTE 		*pRelFLCFG;							//故障测距配置字 
	DWORD			*pSysTimer;							//系统定时器指针 	
	
	TRelElecValPar		*pRelElec;							//当前电量指针 		
	TRelElecValPar		*pRelHarm3;							//当前三次谐波指针 	
	TRelElecValPar		*pRelHarm5;							//当前五次谐波指针 	
	TRelElecValPar		*pRelVolt;							//当前用来闭锁的电压的指针 

	WORD			StartRelayWord;						//过量保护入段继电器字 
	WORD			ActRelayWord;						//过量保护动作继电器字 
	WORD			AccRelayWord;						//后加速允许继电器字 
	WORD			AccActRelayWord;					//后加速动作继电器字 
	WORD			PForwardRelayWord;					//功率正方向继电器字 
	WORD			PRewardRelayWord;					//功率反方向继电器字 
	WORD			H2LockRelayWord;					//二次谐波闭锁继电器字 
	WORD			R47ULKRelayWord;					//复压闭锁继电器字
	WORD			LockRelayWord;						//入段闭锁继电器字 
	WORD			RSTRelayWord;						//返回闭锁继电器字 
	WORD			LoopCtrlRelayWord;					//保护循环控制继电器字 
	
	WORD			wReportNum;							//过量保护报告序号 
	WORD			wAccReportNum;						//加速动作报告序号 
	WORD			*RptSerialNum;
	LONG			*dwK35;

	BOOL			bMakeActRpt;						//FALSE,作告警报告；TRUE，作动作报告
	//**********************入段返回报告部分*************	yanxs  13-02-05
	WORD			wPickUpReportNum;						//入段报告序列号
	WORD			wResetReportNum;						//返回报告序列号
	WORD			wAccPickUpRptNum;						//入段报告序列号
	WORD			wAccResetRptNum;						//返回报告序列号
//	WORD			*RptPickupSerialNum;					
//	BOOL			*RptPickupFlag;							//是否已做入段报告启动标志
	//**********************************************************************	
	DWORD			dwRelTimer;			 				//定时器 
	WORD			byRelFCounter;						//故障计数器 
	WORD			byRelRCounter;						//返回计数器 
	WORD			wExcepWordNum;						//TRelGeneOvValPar 通用过量 本结构对象计算CRC校验时需排除在外的字数，本结构为8,本结构大小为72字 
	WORD			wPickUpType;						//保护入段类型:加速入段或者一般入段			
	WORD			wInfo;								//备用 
	
}TRelGeneOvValPar;
/********************************************************
* 名称:		TRelGeneOvValPar_2
* 描述:		通用过量保护元件参数结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TREL_GENERELOVERVALUE_PAR_2{
	const LONG		*pElec1Set;          						//过量保护电量定值 	
	const LONG		*pElec2Set;          						//过量保护电量定值 
	const LONG		*pTimeSet;				 			//过量保护时间定值 
	const LONG		*pHarmKhr1Set;						//过量保护谐波加权系数 
	const LONG		*pVolLockSet;				 	    	//过量保护电压闭锁定值 
	const LONG		*pAccActTimeSet;						//过量保护后加速动作时间定值 
	const LONG		*pH2Coef;							//谐波闭锁系数、抑制系数的放大倍数 

	const BYTE		*pRelCFG;							//过量保护功能控制字--1:投入，2:退出  
	const BYTE		*pRelSW;         						//过量保护软压板------1:投入，2:退出  
	const BYTE		*pRelHLockCFG;			    			//谐波闭锁功能控制位--1:投入，2:退出 
	const BYTE		*pRelHSuprCFG; 						//谐波抑制功能控制位--1:投入，2:退出 
	const BYTE		*pRelPDCFG;		    					//功率方向功能控制位--1:投入，2:退出 
	const BYTE		*pRelVolLockCFG;						//低压闭锁功能控制位--1:投入，2:退出  
	const BYTE		*pRelFDCFG;							//过量保护正方向功能控制位----1:投入，2:退出 
	const BYTE		*pRelAccCFG;							//有无后加速功能------1:有，  2:无 
	const BYTE      *pRel47UCFG;						//复压闭锁功能控制位
	const BYTE 		*pRelFLCFG;							//故障测距配置字 
	DWORD			*pSysTimer;							//系统定时器指针 	
	
	TRelElecValPar		*pRelElec1;							//当前主电量指针	
	TRelElecValPar		*pRelElec2;							//当前次电量指针 		
	TRelElecValPar		*pRelHarm3;							//当前三次谐波指针 	
	TRelElecValPar		*pRelHarm5;							//当前五次谐波指针 	
	TRelElecValPar		*pRelVolt;							//当前用来闭锁的电压的指针 

	WORD			StartRelayWord;						//过量保护入段继电器字 
	WORD			ActRelayWord;						//过量保护动作继电器字 
	WORD			AccRelayWord;						//后加速允许继电器字 
	WORD			AccActRelayWord;					//后加速动作继电器字 
	WORD			PForwardRelayWord;					//功率正方向继电器字 
	WORD			PRewardRelayWord;					//功率反方向继电器字 
	WORD			H2LockRelayWord;					//二次谐波闭锁继电器字 
	WORD			R47ULKRelayWord;					//复压闭锁继电器字
	WORD			LockRelayWord;						//入段闭锁继电器字 
	WORD			RSTRelayWord;						//返回闭锁继电器字 
	WORD			LoopCtrlRelayWord;					//保护循环控制继电器字 
	
	WORD			wReportNum;							//过量保护报告序号 
	WORD			wAccReportNum;						//加速动作报告序号 
	WORD			*RptSerialNum;
	LONG			*dwK35;

	BOOL			bMakeActRpt;						//FALSE,作告警报告；TRUE，作动作报告
	//**********************入段返回报告部分*************	yanxs  13-03-18
	WORD			wPickUpReportNum;						//入段报告序列号
	WORD			wResetReportNum;						//返回报告序列号
	WORD			wAccPickUpRptNum;						//入段报告序列号
	WORD			wAccResetRptNum;						//返回报告序列号
	//**********************************************************************	


	
	DWORD			dwRelTimer;			 				//定时器 
	WORD			byRelFCounter;						//故障计数器 
	WORD			byRelRCounter;						//返回计数器 
	WORD			wExcepWordNum;						//TRelGeneOvValPar_2 通用过量2 本结构对象计算CRC校验时需排除在外的字数，本结构为8,本结构大小为76字 
	WORD			wPickUpType;						//保护入段类型:加速入段或者一般入段			
	WORD			wInfo;								//备用 
	
}TRelGeneOvValPar_2;

/********************************************************
* 名称:		TRel21Par
* 描述:		通用距离保护元件参数结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct  TREL21PAR{
	const LONG		*pRel21RSet;					//距离保护电阻定值 
	const LONG		*pRel21XSet;						//距离保护电抗定值 
	const LONG		*pRel21TSet;						//距离保护时间定值 
	const LONG		*pRel21Khr1Set;					//距离保护谐波抑制系数 
	const LONG		*pRel21LineAngleSet;				//线路阻抗角 
	const LONG		*pRel21AccTSet;					//距离后加速动作时间 
	const LONG		*pRel21PTBreakCurSet;			//PT断线闭锁电流定值 
	const LONG		*pH2Coef;						//谐波闭锁系数、抑制系数的放大倍数 
	const LONG		*pRel21ZOffset;					//平行四边形阻抗偏移值 
	const LONG		*pRel21AOffset;					//阻抗四边形角度偏移定值

	
	const BYTE		*pRel21CFG;						//距离保护配置 
	const BYTE		*pRel21SW;						//距离保护软压板 
	const BYTE		*pRel21DirCFG;					//距离保护方向 
	const BYTE		*pRel21QuadTypeCFG;				//平行四边形 
	const BYTE		*pRel21HLockCFG;				//距离保护谐波闭锁 
	const BYTE		*pRel21HSuprCFG;				//距离保护谐波抑制 
	const BYTE		*pRel21PTBreakCFG;				//PT断线检查 
	const BYTE		*pRel21ACCCFG;					//后加速功能控制 
	const BYTE 		*pRelFLCFG;						//故障测距配置字 
	const DWORD   	*pSysTimer;						//系统定时器指针 
	const WORD		*pTangTabAddr;					//正切表首地址 
	

	TRelElecValPar		*pRel21Cur;						//电流指针 
	TRelElecValPar		*pRel21Vol;						//电压指针 
	TRelElecValPar  	*pRel21Impd;					//当前的阻抗 
	TRelElecValPar		*pRel21H3;						//三次谐波指针 
	TRelElecValPar		*pRel21H5;						//五次谐波指针 
	
	LONG		   *pnZCoef;						   //阻抗系数     //2013-04-17   用于阻抗过大判断返回

	WORD			StartRelayWord;					//距离保护入段继电器字 
	WORD			ACTRelayWord;					//距离保护动作继电器字 
	WORD			ACCRelayWord;					//后加速允许继电器字 
	WORD			ACCActRelayWord;				//后加速动作继电器字 
	WORD			PTBreakRelayWord;				//PT断线继电器字 
	WORD			H2LockRelayWord;				//二次谐波闭锁继电器字 
	WORD			LockRelayWord;					//入段闭锁继电器字 
	WORD			LoopCtrlRelayWord;				//保护循环控制继电器字 

	WORD			wRel21RptNum;				     	//距离保护报告序号 
	WORD			wRel21AccRptNum;				//距离保护后加速报告序号 
	WORD			wRel21PTLockRptNum;			//PT断线闭锁距离保护报告序号 

	WORD			*RptSerialNum;
	LONG			*dwK35;
	
	WORD			R37IRelayWord;					//无流继电器字	
	//**********************入段返回报告部分*************	yanxs  13-03-08
	WORD			wPickUpReportNum;						//入段报告序列号
	WORD			wResetReportNum;						//返回报告序列号
	WORD			wAccPickUpRptNum;						//入段报告序列号
	WORD			wAccResetRptNum;						//返回报告序列号
//	WORD			*RptPickupSerialNum;
//	BOOL			*RptPickupFlag;
	//**********************************************************************	
	DWORD			dwRel21Timer;					//定时器 	
	WORD			byRel21FCounter;					//距离保护故障计数器 
	WORD			byRel21RCounter;				//距离保护返回计数器 
	WORD			wExcepWordNum;					//TRel21Par 距离保护 本结构对象计算CRC校验时需排除在外的字节数，本结构为8，大小为82 	
	WORD			wPickUpType;						//保护入段类型:加速入段或者一般入段			
	WORD			wInfo;							//备用 
}TRel21Par;

/********************************************************
* 名称:		TRelGeneUnValPar
* 描述:		通用欠量保护元件参数结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TREL_GENERELUNDERVALUE_PAR{
	
	const LONG		*pELeSet;						//电量定值 
	const LONG		*pTimeSet;						//时间定值 
	const LONG		*pLockCurSet;					//闭锁电流定值 

	const BYTE		*pRelCFG;					  	 //欠量保护配置 
	const BYTE		*pRelSW;					  	 //欠量保护软压板 
	const BYTE		*pRelAlarmCFG;						//欠量保护投告警
	const BYTE		*pRelCurLockCFG;					//欠量保护电流闭锁
	const DWORD		*pSysTimer;						//系统定时器指针 

	TRelElecValPar		*pRelElec;						//当前的电量指针 
	TRelElecValPar		*pRelLockElec;					//闭锁电流 

	WORD			ActRelayWord;					//欠量保护动作继电器字 
	WORD			StartRelayWord;					//欠量保护入段继电器字 
	WORD			AlarmRelayWord;					//欠量保护告警继电器字 
	WORD			LockRelayWord;					//入段闭锁继电器字 
	WORD			RSTRelayWord;					//故障返回闭锁继电器字 
	WORD			LoopCtrlRelayWord;				//故障循环控制继电器字 
	WORD			wR52BFRealyword;				//断路器跳闸失败继电器字
	WORD			wActRptNum;						//欠量保护动作报告序号 
	WORD			wAlarmRptNum;					//欠量保护告警报告序号 
	WORD			wTRFailRptNum;					//断路器跳闸失败报告序号
	WORD			wPickUpRptNum;					//启动报告序号
	BYTE			*RptSerialNum;	
//	//***************************保护报告增加保护启动及返回报告**********		yanxs  2013-03-08
//	WORD			wResetReportNum;				//低压保护返回报告
//	WORD			wPickUpReportNum;				//低压保护启动报告
//	//**********************************************************************
    TAbsTime		R27UPickUpTime;					//低压保护启动时间
    WORD			R27RptSerialNum;				//作报告使用的序列号
	DWORD			dwRelTimer;						//欠量保护定时器 	
	WORD			byRelFCounter;					//故障计数器 
	WORD			byRelRCounter;					//返回计数器 
	BOOL			bHaveMake52BFRptFlag;
	WORD			wExcepWordNum;					//TRelGeneUnValPar 通用欠量 本结构对象计算CRC校验时需排除在外的字节数，本结构为14 本结构大小为48
	WORD			wInfo;							//备用 

}TRelGeneUnValPar;
/********************************************************
* 名称:		TRelInverseCurPar
* 描述:		反时限过流保护结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 

typedef struct TRELINVERSECURPAR{
	
	const LONG		*pRelCurSet;                   			 //反时限保护电流定值 
	const LONG		*pRelTimeSet;					//反时限保护时间定值 
	const LONG		*pRelTypeSet;					//反时限保护曲线类型 

	const BYTE		*pRelCFG;						//反时限保护配置 
	const BYTE		*pRelSW;						//反时限保护软压板 
	const DWORD		*pSysTimer;						//系统定时器 
	const LONG		(*pCoefTabA)[3];					//一般反时限系数表地址 
	const LONG		*pCoefTabB;						//极反时限系数表地址 
	const LONG		*pCoefTabC;						//甚反时限系数表地址 

	TRelElecValPar		*pRelElec;						//当前的电量指针 
	DWORD			dwCaluCurSet;					//计算后的反时限电流定值 

	WORD			ACTRelayWord;					//反时限动作继电器字 
	WORD			StartRelayWord;					//反时限入段继电器字 
	WORD			LockRelayWord;					//入段闭锁继电器字 
	WORD			RSTRelayWord;					//返回闭锁继电器字 
	WORD			LoopCtrlRelayWord;				//故障循环控制继电器字 
	WORD			wRelRptNum;					//反时限保护报告序号 
	WORD			*RptSerialNum;		
	//**********************入段返回报告部分*************	yanxs  13-03-08
	WORD			wPickUpReportNum;						//入段报告序列号
	WORD			wResetReportNum;						//返回报告序列号
//	WORD			*RptPickupSerialNum;
//	BOOL			*RptPickupFlag;
	//**********************************************************************	
	LONG			MaxCouter;						//最大累加次数 	
	LLONG			CurCounterSet;					//电流累加量计算后的定值 (28000*50*SAMPNUM*Is*Is*Ts)  
	
	LLONG			CurItem;						//电流累加量的当前项 
	LLONG			CurCounter;						//电流累加量 
	LONG			LimitCouter;					//极限动作时间判断用的累加次数 	
	WORD			byRelFCounter;					//故障计数器 
	WORD			byRelRCounter;					//返回计数器 
	DWORD			dwRelTimer;						//反时限保护定时器 	
	WORD			wExcepWordNum;					//TRelInverseCurPar 反时限过流 本结构对象计算CRC校验时需排除在外的字节数，本结构为16 大小为56 
	WORD			wInfo;							//备用 

}TRelInverseCurPar;
/********************************************************
* 名称:		TRelFaultLocatePar
* 描述:		故障测距参数结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 

typedef struct TRELFAULTLOCATEPAR{
	
	const BYTE		*pRel21FLCFG;						//故障测距配置字 
	const LONG		*pRel21FLNSet;                             	       //测距分段数定值 
	const LONG		*pRel21FL1LSet; 						//第1段距离定值 
	const LONG		*pRel21FL1XSet;						//第1段单位电抗定值 
	const LONG		*pRel21FL2LSet; 						//第2段距离定值 
	const LONG		*pRel21FL2XSet;  					//第2段单位电抗定值 
	const LONG		*pRel21FL3LSet;  						//第3段距离定值 
	const LONG		*pRel21FL3XSet; 						//第3段单位电抗定值 
	const LONG		*pRel21FL4LSet;  						//第4段距离定值 
	const LONG		*pRel21FL4XSet; 						//第4段单位电抗定值 
	const LONG		*pRel21FL5LSet;  						//第5段距离定值 
	const LONG		*pRel21FL5XSet; 						//第5段单位电抗定值 
	const LONG		*pRel21FL6LSet; 						//第6段距离定值 
	const LONG		*pRel21FL6XSet; 						//第6段单位电抗定值 
	const LONG		*pRel21FL7LSet;						//第7段距离定值 
	const LONG		*pRel21FL7XSet; 						//第7段单位电抗定值 
	const LONG		*pRel21FL8LSet; 						//第8段距离定值 
	const LONG		*pRel21FL8XSet;						//第8段单位电抗定值 
	const LONG		*pRel21FL9LSet; 						//第9段距离定值 
	const LONG		*pRel21FL9XSet;						//第9段单位电抗定值 
	const LONG		*pRel21FL10LSet; 					//第10段距离定值 
	const LONG		*pRel21FL10XSet;					//第10段距离定值 
	const LONG		*pRel21FLS0;	//公里标
	const LONG	    *pRel21FLSD;	//公里标方向
	const LONG	    *pRel21FLLc;	//修正距离

     LONG			*pFaultValueL;						//故障电量L的指针 
	 WORD			FaultValueNum;						//故障电量序号 
	 LONG			*pSS;
	 WORD			SSNO;
	 
	 LONG			*pnLCoef;							//距离放大倍数 
	 DWORD			SetTab[30];							//计算定值表

	TRelElecValPar  *pRel21FLImpd;						//当前阻抗的指针 	
	WORD			wExcepWordNum;						//TRelFaultLocatePar 故障测距 本结构对象计算CRC校验时需排除在外的字节数，本结构为0，大小为114 
	
}TRelFaultLocatePar;

/********************************************************
* 名称:		TRelPTBrokPar
* 描述:		pt断线判断参数结构 
* 作者: 
* 日期:
* 备注: 该结构用于判断两路电压1个电流，或者1路电压1路电流的情况
********************************************************/ 
typedef struct TRELPTBROKPAR{

	const BYTE 		*pRelPTBrokCFG;						//PT断线配置 

	const LONG		*pR74PTU1Set;						//PT断线检测电压定值
	const LONG		*pR74PTU2Set;						//PT断线检测电压定值
	const LONG		*pR74PTISet;						//pt断线检测电流定值	
	const LONG		*pR74PTTSet;						//pt断线检测时间定值
	
	TRelElecValPar		*pRelCur;						//判断用电流 
	TRelElecValPar		*pRelU1;						//判断用第一路电压，主判断电压	
	TRelElecValPar		*pRelU2;						//判断用第二路电压，辅助判断电压

	WORD			StartRelayWord;						//PT断线入段继电器字 
	WORD			ActRelayWord;						//PT断线动作继电器字 	
	const DWORD		*dwSysTimer;						//系统定时器 
	WORD			LKPIRelayWord;						//入段闭锁继电器字
	WORD			wRelRptNum;							//PT断线动作报告号
	WORD			wRelResRptNum;						//PT断线返回报告号
	
	WORD			wFCounter;
	DWORD			dwRelTimer;							//PT断线定时器 	
	WORD			wExcepWordNum;						//TRelPTBrokPar PT断线 本结构对象计算CRC校验时需排除在外的字节数，本结构为5，大小为28 

}TRelPTBrokPar;
/********************************************************
* 名称:		TRel79Par
* 描述:		重合闸元件参数结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TRELAUTORECLOSEPAR{

	const LONG		*R79NumSet;						//投入的重合闸次数 
	const LONG		*R79T1Set;							//第一次重合闸动作时间 
	const LONG		*R79T2Set;							//第二次重合闸动作时间 
	const LONG		*R79RSTSet;							//重合闸充放电时间 
	const LONG		*R79SOTFTSet;						//合于故障检测延时，即为后加速投入时间 
	const LONG		*R79PISTSet;						//重合闸启动监视延时，即为断路器动作延时 
	const LONG		*R79CLSTSet;						//重合闸动作监视延时 

	
	const BYTE		*R79SCFG;						//偷跳重合闸配置
	const BYTE		*R79SW;						//重合闸软压板

	WORD			R79RW;						//重合闸配置继电器字--中间继电器字，在逻辑中进行配置
	WORD			R79CYSRW;					//重合闸周期状态继电器字--自保持继电器字 
	WORD			R79RSSRW;					//重合闸复归状态继电器字--自保持继电器字 
	WORD			R79LOSRW;					//重合闸闭锁状态继电器字--自保持继电器字 

	WORD			R79SH0RW;					//重合闸动作状态继电器字一--输入继电器字 		
	WORD			R79SH1RW;					//重合闸动作状态继电器字二--输入继电器字 

	WORD			R79PIRW;					//重合闸启动逻辑继电器字--自保持继电器字 
	WORD			R79PISRW;					//重合闸启动监视逻辑继电器字--中间继电器字 
	WORD			R79TORW;					//重合闸延时到逻辑继电器字--输入继电器字 
	WORD			R79CLSRW;					//重合闸动作监视逻辑继电器字--中间继电器字 
	WORD			R79CLRW;					//重合闸动作逻辑继电器字--自保持继电器字 
	WORD			R79CLCRW;					//重合闸动作命令撤销逻辑继电器字--中间继电器字 
	WORD			R79LOCKRW;					//重合闸闭锁逻辑继电器字--中间继电器字 
	WORD			R79SOTFRW;					//重合闸合于故障逻辑继电器字--输入继电器字 
	WORD			R79TRCHECK;					//重合闸启动条件继电器字--中间继电器字 by yanzh 2013年6月18日 16:57:16
	
	WORD			R7952A;						//重合闸断路器合位继电器字 
	WORD			R7952B;						//重合闸断路器分位继电器字 
	const DWORD		*dwSysTimer;				//系统定时器指针 
	WORD			wRpt79_1;					//一次重合闸动作报告号 
	WORD			wRpt79_2;					//二次重合闸动作报告号 
	WORD			wRpt79Fail;					//重合闸失败报告号 
	WORD			wRpt79LK_1;					//重合闸被闭锁报告号	
	WORD			wRpt79LK_2;					//重合闸被闭锁报告号
	WORD			wPickUpRptNum;				//保护启动报告序号

	WORD			wRpt79_1_SUCC;			//一次重合闸成功报告  by yanzh 2013年6月18日 17:31:57
	WORD			wRpt79_1_FAIL;			//一次重合闸失败报告
	WORD			wRpt79_2_SUCC;			//二次重合闸成功报告
	WORD			wRpt79_2_FAIL;			//二次重合闸失败报告
	WORD			wRpt79PI_1;				//一次重合闸启动报告	
	WORD			wRpt79PI_2;				//二次重合闸启动报告	
	WORD			wRpt79NPI_1;				//重合闸未启动报告	
	WORD			wRpt79NPI_2;				//重合闸未启动报告

	

	WORD			*RptSerialNum;
	BYTE			*SysRptSerialNum;
	BOOL			*pFaultOnFlag;
	BOOL			*pR79PIRptFlag;				//重合闸启动报告标志


	BOOL			R79RSFLAG;					//重合闸充放电延时启动标志 
	BOOL			R79PISFLAG;					//重合闸启动监视延时启动标志 
	BOOL			R79TFLAG;					//重合闸动作延时启动标志 
	BOOL			R79CLSFLAG;					//重合闸动作监视延时启动标志 
	BOOL			R79LKFlag;					//作重合闸被闭锁报告标志	
	BOOL 			R79CLFlag;					//重合闸动作标志
	
	DWORD			R79Timer;					//重合闸动作延时定时器 
	WORD			R79ShotCnt;					//重合闸已经动作次数 	
	WORD			wExcepWordNum;				//TRel79Par 重合闸 本结构对象计算CRC校验时需排除在外的字节数，本结构为16，大小为64 
}TRel79Par;

/********************************************************
* 名称:		TRel50DIPar
* 描述:		增量保护元件 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct INCRECURRENTPAR{

	const LONG		*pRelCurSet;                   			 //增量保护电流定值 
	const LONG		*pRelTimeSet;					//增量保护时间定值 
	const LONG		*pRelLockCurSet;					//增量保护小电流闭锁定值 
	const LONG		*pRelKh1;						//二次谐波闭锁系数 
	const LONG		*pRelKhr;						//增量保护谐波加权系数 
	const LONG		*pH2Coef;						//二次谐波闭锁系数，谐波加权系数等的放大倍数 

	const BYTE		*pRelCFG;						//增量保护配置 
	const BYTE		*pRelSW;						//增量保护软压板 


	TRelElecValPar		*pRelElec1;						//当前基波电流指针 
	TRelElecValPar		*pRelElec2;						//当前二次谐波电流指针 
	TRelElecValPar		*pRelElec3;						//当前三次谐波电流指针 
	TRelElecValPar		*pRelElec5;						//当前五次谐波电流指针 

	DWORD     		*pFaultSINum;					//故障处理采样间隔计数器指针 

	WORD			ACTRelayWord;					//增量保护动作继电器字 
	WORD			StartRelayWord;					//增量保护入段继电器字 
	WORD			LockRelayWord;					//入段闭锁继电器字 
	WORD			RSTRelayWord;					//返回闭锁继电器字 
	WORD			LoopCtrlRelayWord;				//故障循环控制继电器字 
	WORD			R79ActRelayWord;
	WORD			H2LockRelayWord;				//二次谐波闭锁继电器字
	WORD			*RptSerialNum;					//系统报告序列号 
	const DWORD		*pSysTimer;						//系统定时器 	
	LONG			*OtherRelTrip;					//其他保护已经动作标志
	WORD			wRelRptNum;					//增量保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-08
	WORD			wPickUpReportNum;						//入段报告序列号
	WORD			wResetReportNum;						//返回报告序列号
//	WORD			*RptPickupSerialNum;
//	BOOL			*RptPickupFlag;
	//**********************************************************************	

	LONG			dwPreI1Mod;						//故障前基波模值 
	LONG			dwPreI3Mod;						//故障前三次谐波模值 
	LONG			dwPreI5Mod;						//故障前五次谐波模值 
	BOOL			PreCurHaveCalcFlag;				//故障前电量已经计算标志 
	WORD			wRelFCounter;					//故障计数器 
	WORD			wRelRCounter;					//返回计数器 	
	DWORD			dwRelTimer;						//保护定时器 		
	WORD			wExcepWordNum;					//TRel50DIPar 增量保护 本结构对象计算CRC校验时需排除在外的字节数，本结构为14，大小为58 
	WORD			Info;							//备用信息 
	
}TRel50DIPar;
/********************************************************
* 名称:		ThermalOverLoad
* 描述:		反时限热过负荷保护元件 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct THERMALOVERLOAD{

	const LONG		*p49KSet;		//反时限过负荷保护因子
	const LONG		*p49TSet;		//反时限过负荷时间常数
	const LONG		*p49ARSet;		//反时限过负荷告警定值
	const LONG		*p49KTSet;		//设备停止扩充系数
	const LONG		*pRatedInSet;		//额定电流值
	const BYTE		*pRel49CFG;		//反时限过负荷保护配置
	const BYTE		*pRel49SW;		//反时限过负荷软压板
	const LONG		*pICoef;
	const LONG		*pIHACoef;
	
	DWORD			dwKIn;			//K倍额定电流值		
	LONG			l49OT;			//反时限过负荷热累加时间间隔
	LONG			l49KOT;			//反时限过负荷散热时间间隔
	LONG			lExpT;			//反时限过负荷热累加时间常数
	LONG			lExpKT;			//反时限过负荷散热时间常数
	
	WORD			R49WarnRW;		//告警继电器字	
	WORD			R49ActRW;		//动作继电器字
	WORD			R49ActRptNum;	//动作报告序号
	WORD			R49WarnRptNum;	//告警报告序号
	WORD			R49RSTRptNum;	//返回报告序号
	const DWORD		*pSysTimer;		//系统定时器 	
	DWORD			dw49Warn;		//热量累加值告警值
	DWORD			dw49Warn_099;	//热累加告警返回值
	
	LLONG			dw49AThermaVal;		//热累加值
	LLONG			dw49AThermaPreVal;	//前期热累加值
	DWORD			pCurrent;			//当前电流值	
	DWORD			PreCurrent;			//上一次计算电流值			
	DWORD			dwRelTimer;			//欠量保护定时器 	
	WORD			wExcepWordNum;		//TThermalOverLoad 热过负荷 本结构对象计算CRC校验时需排除在外的字节数，本结构为16，大小为56 
	
	
	
} TThermalOverLoad;
/********************************************************
* 名称:		TStructCheck
* 描述:		保护元件自检结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TSTRUCTCHECK{
		WORD		*pstruct;
		WORD		 bigsize;
		WORD		 acc;
} TStructCheck;
/********************************************************
* 名称:		TRel87R
* 描述:		比率差动保护元件结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TREL87R{
	const LONG		*p87RISet;		//差动电流定值
	const LONG		*pIRS1Set;		//制动拐点1定值
	const LONG		*pIRS2Set;		//制动拐点2定值	
	const LONG		*pSLP1Set;		//比率制动系数1
	const LONG		*pSLP2Set;		//比率制动系数2	
	const LONG		*pKrh2Set;		//二次谐波制动系数
	const LONG		*pKrh5Set;		//五次谐波制动系数
	const LONG		*pKravSet;		//比率差动非周期分量加权系数
	const LONG		*pK2avSet;		//二次谐波非周期分量加权系数

	const BYTE		*p87RCFG;		//保护配置
	const BYTE		*p87RSW;		//软压板
	const BYTE		*p68IdH2MCFG;		//差流二次谐波最大相闭锁配置
	const BYTE		*p68IdH2CCFG;		//差流二次谐波综合相闭锁配置
	const BYTE		*p68IdH2ICFG;		//差流二次谐波分相闭锁配置
	const BYTE		*p68IrH2MCFG;		//制动电流二次谐波闭锁配置
	const BYTE		*p68IdH5CFG;		//差流5次谐波闭锁
	const BYTE		*p68IdHMulCFG;		//差流综合谐波闭锁配置

	const BOOL		*pIdH2Flag;		//二次谐波闭锁标志
	const BOOL		*pIdH5Flag;		//五次谐波闭锁标志
	const BOOL		*pIrH2Flag;		//制动电流二次谐波闭锁标志
	const BOOL      *pIdHMulFlag;	//综合谐波闭锁标志

	LONG 			*p87RPI;		//抑制计算后的差动电流门槛定值
	TRelElecValPar			*pId;			//差流电流
	TRelElecValPar			*pIr;			//制动电流

	WORD			ACTRelayWord;					//动作继电器字 
	WORD			StartRelayWord;					//入段继电器字 
	WORD			LockRelayWord;					//入段闭锁继电器字 
	WORD			RSTRelayWord;					//返回闭锁继电器字 
	WORD			LoopCtrlRelayWord;				//故障循环控制继电器字 
	
	WORD			*RptSerialNum;					//系统报告序列号 
	const DWORD		*pSysTimer;						//系统定时器 
	WORD			wRelRptNum;					//保护动作报告 
	//**********************入段返回报告部分*************	yanxs  13-03-18
	WORD			wPickUpReportNum;						//入段报告序列号
	WORD			wResetReportNum;						//返回报告序列号
	//**********************************************************************	
	WORD			wRelFCounter;					//故障计数器 
	WORD			wRelRCounter;					//返回计数器 	
	DWORD			dwRelTimer;						//保护定时器 		
	WORD			wExcepWordNum;					//TRel87R 比率差动 本结构对象计算CRC校验时需排除在外的字节数，本结构为7，大小为68
	WORD			Info;							//备用信息 
} TRel87R;
/********************************************************
* 名称:		TRel87R
* 描述:		比率差动保护元件结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct T87IALRAM{

	const BYTE		*p87RSW;		//软压板
	const BYTE		*p87R_A_CFG;		//保护配置
	const BYTE		*p87R_B_CFG;		//保护配置
	const BYTE		*p87R_C_CFG;		//保护配置

	const LONG		*p87RIASet; 	//A差动电流定值
	const LONG		*p87RIBSet; 	//B差动电流定值
	const LONG		*p87RICSet; 	//C差动电流定值
	const LONG		*p87RIKSet;		//差流越限告警比率

	TRelElecValPar  *pIDA;			//差流A
	TRelElecValPar	*pIDB;			//差流B
	TRelElecValPar	*pIDC;			//差流C
	
	LONG			AlarmCnt;		//差流越限告警时间
	WORD			ARRW;			//差流越限告警继电器字
	WORD			wRelRptNum;		//差流越限告警报告序列号

	LONG			ACnt;			//A相计数器
	LONG			BCnt;			//B相计数器
	LONG			CCnt;			//C相计数器
	BOOL			AAlarmFlag;		//A相告警标志
	BOOL			BAlarmFlag;		//B相告警标志
	BOOL			CAlarmFlag;		//C相告警标志
	BOOL			MakeRptFlag;	//作告警报告标志	
	WORD			wExcepWordNum;					//T87IAlarm 差流越限 本结构对象计算CRC校验时需排除在外的字节数，本结构为16，大小为42 
	WORD			Info;							//备用信息 
}T87IAlarm;

typedef struct SUBCHANNELCHECK{

	WORD 	subNo;				//子所通道序号
	DWORD 	subTimer;			//子所通道监视定时器
	BOOL 	subRxdFlag;			//子所接收数据标志
	BOOL 	subFaultFlag;		//子所通道故障标志
	WORD	subFaultInf;		//子所通道故障报告INF号
	WORD	subGPSInf;			//子所GPS时钟质量差报告INF号
	WORD	subDataInf;			//子所故障数据INF号	
	WORD	subDataLossInf;		//子所通道故障后数据丢失INF号
	DWORD	GPSStatus;		  	//GPS卫星钟质量
	TAbsTime SubClock;			//子所时钟
	WORD	ComplicateNo;		//并发序列号
	
	LONG	U1;
	LONG	U2;
	LONG	U3;
	LONG	U4;
	LONG	It1;
	LONG 	If1;
	LONG	It2;
	LONG	If2;
	LONG	It3;
	LONG	If3;
	LONG	Iat1;
	LONG	Iat2;
	LONG	Iat;
	LONG	AIt1;
	LONG	AIf1;
	LONG	AIt2;
	LONG	AIf2;
	LONG	AIt3;
	LONG	AIf3;
	LONG 	Ip1;
	LONG	Ip2;
	LONG	Ip3;

} subChannelCheck;
typedef struct SUBDATA{
	
	LONG					U1; //子所1数据
	LONG					U2;
	LONG					U3;
	LONG					U4;
	LONG					IT1;
	LONG					IF1;		
	LONG					IT2;
	LONG					IF2;
	LONG					IT3;
	LONG					IF3;
	LONG					IAT1;
	LONG					IAT2;
	LONG					IAT;
	LONG					AU1;
	LONG					AU2;
	LONG					AU3;
	LONG					AU4;
	LONG					AIT1;
	LONG					AIF1;
	LONG					AIT2;
	LONG					AIF2;
	LONG					AIT3;
	LONG					AIF3;
	LONG					AIAT1;
	LONG					AIAT2;
	LONG					CBStatus;
	LONG 					IP1;
	LONG					IP2;
	LONG					IP3;
}TSubData;
typedef struct FLCOMPLICATE {

	BOOL pickupFlag;      //本次测距启动标志
	BOOL dataOKFlag;		//本次测距数据收齐标志
	DWORD pickupTime;    //本次测距启动时间
	BYTE rptSerialNo;	//本次测距的报告序号，因为保护启动总是先做出
						//若有并发，需记下该序号
} FLComplicate;

/********************************************************
* 名称:		TRel25R
* 描述:		检同期元件结构 
* 作者: 
* 日期:
* 备注:
********************************************************/ 
typedef struct TREL25R{

	const LONG		*p25_27Set; 	//合闸检无压定值
	const LONG		*p25DTSet;		//同期检测时间定值
	const LONG		*p25VULSet; 	//检同期电压上限定值
	const LONG		*p25VLLSet; 	//检同期电压下限定值
	const LONG		*p25VDSet;   	//同期允许压差定值
	const LONG		*p25FDSet;		//同期允许频差定值
	const LONG		*p25PASet;		//同期功角定值
	const LONG		*p25PHSet;		//同期相别
	const LONG		*p52CLT;		//断路器合闸时间
	
	const BYTE		*p25RCFG;		//手合检同期配置
	const BYTE		*p79_25RCFG;	//重合闸检同期配置
	const BYTE		*p25RSW;		//手合检同期软压板

	TRelElecValPar	*pUsyn;
	TRelElecValPar	*pU1;
	LONG 			*pDA;		    //角差
	LONG 			*pDV; 	//压差
	LONG 			*pDF;	//频差
	LONG            *pPA;         //导前角
	
	WORD			w52A;			//断路器合位继电器字
	WORD            w79_25PI;          //同期启动继电器字
	WORD            wMCL_25PI;          //同期启动继电器字
	WORD            w25LK;          //检同期闭锁继电器字
	WORD			w25ActRW;		//检同期动作继电器字
	WORD			w25INT;			//检同期中断继电器字
	WORD			R25SucRptNum;	//检同期成功报告序号
	WORD			R25FalRptNum;	//检同期失败报告序号
	WORD			R25LKRptNum;	//检同期被闭锁报告序号
	WORD			R25INTRptNum;	//检同期被中断报告序号
	WORD			R25_27RptNum;	//无压空合报告序号
	WORD			*RptSerialNum;					//系统报告序列号 
	const DWORD		*pSysTimer;						//系统定时器 


	BOOL			bManSynFlag;		        //手合检同期标志
	BOOL            bJudgeNetend;            //网判断完成标志
	BOOL			bSameNetFlag;		    //同网标志
	BOOL			bDiffNetFlag;		    //异网标志
	BOOL			bSigleNetFlag;		    //单网标志
	BOOL			bJudgeSameNetPIFlag;		//同网判断延时入段标志
	BOOL			bJudgeDiffNetPIFlag;		//异网判断延时入段标志
	BOOL			bJudgeSigleNetPIFlag;	//单网判断延时入段标志
	BOOL			bBusPTBreakFlag;	    //母线PT断线标志
	BOOL			bHaveMakeLKRptFlag;		//已作同期被闭锁报告标志
	BOOL			bHaveMakeINTRptFlag;
	BOOL			bDVOutLimitFlag;		//电压越限标志

	DWORD            dwInitSingleNetChkTimer;      //单网判断时间定时器
	DWORD            dwInitSameleNetChkTimer;      //同网判断时间定时器
	DWORD            dwInitDiffleNetChkTimer;      //异网判断时间定时器
	DWORD            dwSynClosePulseTimer;         //检同期合闸脉冲定时器
	DWORD            dwSynCheckTime;               //检同期时间定时器

	WORD			wExcepWordNum;					//TRel25R 检同期 本结构对象计算CRC校验时需排除在外的字节数，本结构为36，大小为88 
	WORD			Info;							//备用信息 
}TRel25R;

typedef struct TLINEDATA{
	float I0;		//零序电流基波幅值
	float U0;		//零序电压基波幅值
	float I0_5;		//零序电流五次谐波幅值
	float U0_5;		//零序电压五次谐波幅值
	float I0_A;		//零序电流基波相对角度
	float I0_5_A;	//零序电流五次谐波相对角度
}TLineData;

/********************************************************
* 名称:		TRelCTBrokPar
* 描述:		CT断线判断参数结构 
* 作者: 
* 日期:
* 备注: 	该结构用于判断保护及测量CT之间电流不同的情况，用一次值对应的数字量
********************************************************/ 
typedef struct TRELCTBROKPAR{

	const BYTE 		*pRelCTBrokCFG;						//CT断线配置 
	LONG			*pRelCurSet;						//CT断线判断电流最小门槛值
	LONG			*pRelTimeSet;						//CT断线检测时间
	const DWORD		*dwSysTimer;						//系统定时器 

	LONG			*pRelCurMax;						//判断用大电流 
	LONG			*pRelCurMin;						//判断用小电流 

	WORD			StartRelayWord;						//CT断线入段继电器字 
	WORD			ActRelayWord;						//CT断线动作继电器字 	
	WORD			wRelRptNum;							//CT断线动作报告号
	WORD			wRelResRptNum;							//CT断线动作报告号
	
	WORD			byRelFCounter;						//故障计数器 
	WORD			byRelRCounter;						//返回计数器 
	DWORD			dwRelTimer;							//CT断线定时器 	
	WORD			wExcepWordNum;						//TRelCTBrokPar PT断线 本结构对象计算CRC校验时需排除在外的字节数，本结构为6，大小为18 

}TRelCTBrokPar;

#endif















