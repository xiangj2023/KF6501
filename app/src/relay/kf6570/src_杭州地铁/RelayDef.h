/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			RelayDef.h
* 
*   软件模块：保护接口文件
*
* 
*	描述  保护定义文件，上半部分是固定的数据结构， 下半部分由保护接口工具软件自动生成
* 
*	注意	请勿手动修改该文件
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-12-17		初始创建
*		kangqian	2010-04-21		增加压板投退继电器使用的参数数组
*************************************************************************/

#ifndef __RelayDef_H
#define __RelayDef_H


#include "sysdefs.h"


#ifdef __PL_WIN_SIMULATION__
	// 在window模拟平台中使用一个数组模拟ram 
	extern unsigned char _RAMDataBuf[];
	#define RELAY_RAM_ADDR_BASE	_RAMDataBuf
#else
#include "resm5234.h"
	//给保护用的片上ram地址
	#define RELAY_RAM_ADDR_BASE	RELAY_RAM_BASE // 硬件决定
#endif

//定值 值名称 的最大长度
#define  MAX_SETVALUNAME  256 

// 报告描述结构
typedef struct TReportInf{
    DWORD dwFialureNum;                // 故障电量个数 
    const LONG** pFailureData; // 故障电量描述信息 
	WORD wInf; //报告inf号
}TReportInf;

//逻辑方程用时间继电器时间参数（关联定值或辅助定值）
typedef struct TTimeSetting{
	LONG* pStartTime; //启动时间，指向关联的定值（或辅助定值）
	LONG* pReturnTime;	//返回时间，指向关联的定值（或辅助定值）
}TTimeSetting;

//定值取值描述列表
typedef struct TSetValueInf
{
	LONG lValue;
	char strName[MAX_SETVALUNAME];
}TSetValueInf;

//定值取值索引表
typedef struct TSetValueInfMap
{
	LONG nItem;
	TSetValueInf* pSetValueInf;
}TSetValueInfMap;

extern const WORD RelayWordTypeList[]; //继电器类型表
extern const struct TSetValueInfMap SetValueInfMap[];
extern const struct TSetValueInf SetValueInf[];

extern const struct TSetValueInfMap SysSetValueInfMap[];
extern const struct TSetValueInf SysSetValueInf[];

extern const struct TTimeSetting TimeSetting[];          // 时间继电器定值
extern  LONG*	TripSetting[]; //跳闸继电器延时定值

extern WORD const OutPutMap[]; //输出继电器-出口 映射表 保存开出号
extern WORD const InPutMap[]; //输入继电器-开入 映射表

extern BYTE* const SWMap[]; //压板映射表 直接指向关联的软压板在内存中的副本
extern BYTE* const CFGMap[]; //配置映射表

extern const struct TReportInf ReportInf[];          // 报告引用 
//系数默认值
extern const LONG CFDefaultVal1[]; //系数1
extern const LONG CFDefaultVal2[]; //系数2

//2010-4-21 11:25:33 
extern WORD const SWIndexMap[]; //压板继电器和压板号的映射关系， 保存软压板序号
                                //注意与SWMap[]的区别。SWIndexMap是为通过压板投退继电器投退软压板时使用的

//以下代码由工具软件生成


#define 	 SETTING_AREA_COUNT 	 16 		 //定值区个数
#define 	 RELAY_DATA_COUNT 	 16 		 //扰动数据区数，
#define 	 SAMPLE_COUNT 	 32 		 //每周波采样点数
#define 	 A15TYPE_SYSSETNO 	 0 		 //1,5A 类型辅助定值序号
#define 	 FAILDATA_DISPTYPE_CFGNO 	 0 		 //故障电量显示类型配置序号
#define 	 LangTYPE_SYSSETNO 	 0 		 //语言类型辅助定值序号

//当前定值区号定义-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //当前定值区号

//定值取值宏定义

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//保护测量值定义-----------------------
#define  RM_AUHA 0
extern LONG* 	RM_pnAUHA; 		 // ΦUHA
#define  RM_AUHB 1
extern LONG* 	RM_pnAUHB; 		 // ΦUHB
#define  RM_AUHC 2
extern LONG* 	RM_pnAUHC; 		 // ΦUHC
#define  RM_AULA 3
extern LONG* 	RM_pnAULA; 		 // ΦULA
#define  RM_AULB 4
extern LONG* 	RM_pnAULB; 		 // ΦULB
#define  RM_AULC 5
extern LONG* 	RM_pnAULC; 		 // ΦULC
#define  RM_AIHA 6
extern LONG* 	RM_pnAIHA; 		 // ΦIHA
#define  RM_AIHB 7
extern LONG* 	RM_pnAIHB; 		 // ΦIHB
#define  RM_AIHC 8
extern LONG* 	RM_pnAIHC; 		 // ΦIHC
#define  RM_AILA 9
extern LONG* 	RM_pnAILA; 		 // ΦILA
#define  RM_AILB 10
extern LONG* 	RM_pnAILB; 		 // ΦILB
#define  RM_AILC 11
extern LONG* 	RM_pnAILC; 		 // ΦILC
#define  RM_BOBRCH1 12
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 13
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 14
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 15
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     16    //保护测量值个数
#define RM_BUF_SIZE     64    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UHA 0
extern LONG* 	M_pnUHA; 		 //UA UA
#define  M_UHB 1
extern LONG* 	M_pnUHB; 		 //UB UB
#define  M_UHC 2
extern LONG* 	M_pnUHC; 		 //UC UC
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_ULA 4
extern LONG* 	M_pnULA; 		 //Ua Ua
#define  M_ULB 5
extern LONG* 	M_pnULB; 		 //Ub Ub
#define  M_ULC 6
extern LONG* 	M_pnULC; 		 //Uc Uc
#define  M_IHA 7
extern LONG* 	M_pnIHA; 		 //IA IA
#define  M_IHB 8
extern LONG* 	M_pnIHB; 		 //IB IB
#define  M_IHC 9
extern LONG* 	M_pnIHC; 		 //IC IC
#define  M_ILA 10
extern LONG* 	M_pnILA; 		 //Ia Ia
#define  M_ILB 11
extern LONG* 	M_pnILB; 		 //Ib Ib
#define  M_ILC 12
extern LONG* 	M_pnILC; 		 //Ic Ic
#define  M_I1 13
extern LONG* 	M_pnI1; 		 //I1 I1
#define  M_I2 14
extern LONG* 	M_pnI2; 		 //I2 I2
#define  M_PA 15
extern LONG* 	M_pnPA; 		 //PA PA
#define  M_QA 16
extern LONG* 	M_pnQA; 		 //QA QA
#define  M_COSA 17
extern LONG* 	M_pnCOSA; 		 //COSA COSA
#define  M_PB 18
extern LONG* 	M_pnPB; 		 //PB PB
#define  M_QB 19
extern LONG* 	M_pnQB; 		 //QB QB
#define  M_COSB 20
extern LONG* 	M_pnCOSB; 		 //COSB COSB
#define  M_PC 21
extern LONG* 	M_pnPC; 		 //PC PC
#define  M_QC 22
extern LONG* 	M_pnQC; 		 //QC QC
#define  M_COSC 23
extern LONG* 	M_pnCOSC; 		 //COSC COSC
#define  M_P 24
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 25
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 26
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_F 27
extern LONG* 	M_pnF; 		 //F F
#define  M_UHAB 28
extern LONG* 	M_pnUHAB; 		 //UAB UAB
#define  M_UHBC 29
extern LONG* 	M_pnUHBC; 		 //UBC UBC
#define  M_UHCA 30
extern LONG* 	M_pnUHCA; 		 //UCA UCA
#define  M_UHA1 31
extern LONG* 	M_pnUHA1; 		 //UA1 UA1
#define  M_UHA2 32
extern LONG* 	M_pnUHA2; 		 //UA2 UA2
#define  M_UHA3 33
extern LONG* 	M_pnUHA3; 		 //UA3 UA3
#define  M_UHA5 34
extern LONG* 	M_pnUHA5; 		 //UA5 UA5
#define  M_UHA7 35
extern LONG* 	M_pnUHA7; 		 //UA7 UA7
#define  M_UHA9 36
extern LONG* 	M_pnUHA9; 		 //UA9 UA9
#define  M_UHA11 37
extern LONG* 	M_pnUHA11; 		 //UA11 UA11
#define  M_UHB1 38
extern LONG* 	M_pnUHB1; 		 //UB1 UB1
#define  M_UHB2 39
extern LONG* 	M_pnUHB2; 		 //UB2 UB2
#define  M_UHB3 40
extern LONG* 	M_pnUHB3; 		 //UB3 UB3
#define  M_UHB5 41
extern LONG* 	M_pnUHB5; 		 //UB5 UB5
#define  M_UHB7 42
extern LONG* 	M_pnUHB7; 		 //UB7 UB7
#define  M_UHB9 43
extern LONG* 	M_pnUHB9; 		 //UB9 UB9
#define  M_UHB11 44
extern LONG* 	M_pnUHB11; 		 //UB11 UB11
#define  M_UHC1 45
extern LONG* 	M_pnUHC1; 		 //UC1 UC1
#define  M_UHC2 46
extern LONG* 	M_pnUHC2; 		 //UC2 UC2
#define  M_UHC3 47
extern LONG* 	M_pnUHC3; 		 //UC3 UC3
#define  M_UHC5 48
extern LONG* 	M_pnUHC5; 		 //UC5 UC5
#define  M_UHC7 49
extern LONG* 	M_pnUHC7; 		 //UC7 UC7
#define  M_UHC9 50
extern LONG* 	M_pnUHC9; 		 //UC9 UC9
#define  M_UHC11 51
extern LONG* 	M_pnUHC11; 		 //UC11 UC11
#define  M_IHA1 52
extern LONG* 	M_pnIHA1; 		 //IA1 IA1
#define  M_IHA2 53
extern LONG* 	M_pnIHA2; 		 //IA2 IA2
#define  M_IHA3 54
extern LONG* 	M_pnIHA3; 		 //IA3 IA3
#define  M_IHA5 55
extern LONG* 	M_pnIHA5; 		 //IA5 IA5
#define  M_IHA7 56
extern LONG* 	M_pnIHA7; 		 //IA7 IA7
#define  M_IHA9 57
extern LONG* 	M_pnIHA9; 		 //IA9 IA9
#define  M_IHA11 58
extern LONG* 	M_pnIHA11; 		 //IA11 IA11
#define  M_IHB1 59
extern LONG* 	M_pnIHB1; 		 //IB1 IB1
#define  M_IHB2 60
extern LONG* 	M_pnIHB2; 		 //IB2 IB2
#define  M_IHB3 61
extern LONG* 	M_pnIHB3; 		 //IB3 IB3
#define  M_IHB5 62
extern LONG* 	M_pnIHB5; 		 //IB5 IB5
#define  M_IHB7 63
extern LONG* 	M_pnIHB7; 		 //IB7 IB7
#define  M_IHB9 64
extern LONG* 	M_pnIHB9; 		 //IB9 IB9
#define  M_IHB11 65
extern LONG* 	M_pnIHB11; 		 //IB11 IB11
#define  M_IHC1 66
extern LONG* 	M_pnIHC1; 		 //IC1 IC1
#define  M_IHC2 67
extern LONG* 	M_pnIHC2; 		 //IC2 IC2
#define  M_IHC3 68
extern LONG* 	M_pnIHC3; 		 //IC3 IC3
#define  M_IHC5 69
extern LONG* 	M_pnIHC5; 		 //IC5 IC5
#define  M_IHC7 70
extern LONG* 	M_pnIHC7; 		 //IC7 IC7
#define  M_IHC9 71
extern LONG* 	M_pnIHC9; 		 //IC9 IC9
#define  M_IHC11 72
extern LONG* 	M_pnIHC11; 		 //IC11 IC11
#define  M_IA1 73
extern LONG* 	M_pnIa1; 		 //Ia1 Ia1
#define  M_IA2 74
extern LONG* 	M_pnIa2; 		 //Ia2 Ia2
#define  M_IA3 75
extern LONG* 	M_pnIa3; 		 //Ia3 Ia3
#define  M_IA5 76
extern LONG* 	M_pnIa5; 		 //Ia5 Ia5
#define  M_IA7 77
extern LONG* 	M_pnIa7; 		 //Ia7 Ia7
#define  M_IA9 78
extern LONG* 	M_pnIa9; 		 //Ia9 Ia9
#define  M_IA11 79
extern LONG* 	M_pnIa11; 		 //Ia11 Ia11
#define  M_IB1 80
extern LONG* 	M_pnIb1; 		 //Ib1 Ib1
#define  M_IB2 81
extern LONG* 	M_pnIb2; 		 //Ib2 Ib2
#define  M_IB3 82
extern LONG* 	M_pnIb3; 		 //Ib3 Ib3
#define  M_IB5 83
extern LONG* 	M_pnIb5; 		 //Ib5 Ib5
#define  M_IB7 84
extern LONG* 	M_pnIb7; 		 //Ib7 Ib7
#define  M_IB9 85
extern LONG* 	M_pnIb9; 		 //Ib9 Ib9
#define  M_IB11 86
extern LONG* 	M_pnIb11; 		 //Ib11 Ib11
#define  M_IC1 87
extern LONG* 	M_pnIc1; 		 //Ic1 Ic1
#define  M_IC2 88
extern LONG* 	M_pnIc2; 		 //Ic2 Ic2
#define  M_IC3 89
extern LONG* 	M_pnIc3; 		 //Ic3 Ic3
#define  M_IC5 90
extern LONG* 	M_pnIc5; 		 //Ic5 Ic5
#define  M_IC7 91
extern LONG* 	M_pnIc7; 		 //Ic7 Ic7
#define  M_IC9 92
extern LONG* 	M_pnIc9; 		 //Ic9 Ic9
#define  M_IC11 93
extern LONG* 	M_pnIc11; 		 //Ic11 Ic11
#define  M_CUSTOM1 94
extern LONG* 	M_pnCustom1; 		 //CTM1 CTM1
#define  M_CUSTOM2 95
extern LONG* 	M_pnCustom2; 		 //CTM2 CTM2
#define  M_CUSTOM3 96
extern LONG* 	M_pnCustom3; 		 //CTM3 CTM3
#define  M_CUSTOM4 97
extern LONG* 	M_pnCustom4; 		 //CTM4 CTM4
#define  M_CUSTOM5 98
extern LONG* 	M_pnCustom5; 		 //CTM5 CTM5
#define  M_CUSTOM6 99
extern LONG* 	M_pnCustom6; 		 //CTM6 CTM6
#define  M_CUSTOM7 100
extern LONG* 	M_pnCustom7; 		 //CTM7 CTM7
#define  M_CUSTOM8 101
extern LONG* 	M_pnCustom8; 		 //CTM8 CTM8

#define MEASURE_COUNT     102    //测量值个数
#define MEASURE_BUF_SIZE     408    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 68)

//软压板----------------------------------
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2
extern BYTE* 	SW_pbyBAK3; 		 //备用压板3
extern BYTE* 	SW_pbyBAK4; 		 //备用压板4

#define SW_COUNT     5    //软压板个数
#define SW_BUF_SIZE     5    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 476)

//配置----------------------------------
extern BYTE* 	CFG_pbyMODSET; 		 //定值修改功能
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2
extern BYTE* 	CFG_pbyBAK3; 		 //备用配置3
extern BYTE* 	CFG_pbyBAK4; 		 //备用配置4

#define CFG_COUNT     5    //配置个数/
#define CFG_BUF_SIZE     5    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 482)

//定值----------------------------------
extern LONG* 	RS_pnF; 		 //配置字 F
extern LONG* 	RS_pnT1PI; 		 //T1启动延时 T1PI
extern LONG* 	RS_pnT1RE; 		 //T1返回延时 T1RE
extern LONG* 	RS_pnT2PI; 		 //T2启动延时 T2PI
extern LONG* 	RS_pnT2RE; 		 //T2返回延时 T2RE
extern LONG* 	RS_pnT3PI; 		 //T3启动延时 T3PI
extern LONG* 	RS_pnT3RE; 		 //T3返回延时 T3RE
extern LONG* 	RS_pnT4PI; 		 //T4启动延时 T4PI
extern LONG* 	RS_pnT4RE; 		 //T4返回延时 T4RE
extern LONG* 	RS_pnT5PI; 		 //T5启动延时 T5PI
extern LONG* 	RS_pnT5RE; 		 //T5返回延时 T5RE
extern LONG* 	RS_pnT6PI; 		 //T6启动延时 T6PI
extern LONG* 	RS_pnT6RE; 		 //T6返回延时 T6RE
extern LONG* 	RS_pnT7PI; 		 //T7启动延时 T7PI
extern LONG* 	RS_pnT7RE; 		 //T7返回延时 T7RE
extern LONG* 	RS_pnT8PI; 		 //T8启动延时 T8PI
extern LONG* 	RS_pnT8RE; 		 //T8返回延时 T8RE
extern LONG* 	RS_pnT9PI; 		 //T9启动延时 T9PI
extern LONG* 	RS_pnT9RE; 		 //T9返回延时 T9RE
extern LONG* 	RS_pnT10PI; 		 //T10启动延时 T10PI
extern LONG* 	RS_pnT10RE; 		 //T10返回延时 T10RE
extern LONG* 	RS_pnT11PI; 		 //T11启动延时 T11PI
extern LONG* 	RS_pnT11RE; 		 //T11返回延时 T11RE
extern LONG* 	RS_pnT12PI; 		 //T12启动延时 T12PI
extern LONG* 	RS_pnT12RE; 		 //T12返回延时 T12RE
extern LONG* 	RS_pnT13PI; 		 //T13启动延时 T13PI
extern LONG* 	RS_pnT13RE; 		 //T13返回延时 T13RE
extern LONG* 	RS_pnT14PI; 		 //T14启动延时 T14PI
extern LONG* 	RS_pnT14RE; 		 //T14返回延时 T14RE
extern LONG* 	RS_pnT15PI; 		 //T15启动延时 T15PI
extern LONG* 	RS_pnT15RE; 		 //T15返回延时 T15RE
extern LONG* 	RS_pnT16PI; 		 //T16启动延时 T16PI
extern LONG* 	RS_pnT16RE; 		 //T16返回延时 T16RE
extern LONG* 	RS_pn59HA; 		 //UHA有压定值 59HA
extern LONG* 	RS_pn27HA; 		 //UHA无压定值 27HA
extern LONG* 	RS_pn59HB; 		 //UHB有压定值 59HB
extern LONG* 	RS_pn27HB; 		 //UHB无压定值 27HB
extern LONG* 	RS_pn59HC; 		 //UHC有压定值 59HC
extern LONG* 	RS_pn27HC; 		 //UHC无压定值 27HC
extern LONG* 	RS_pn59N; 		 //U0有压定值 59N
extern LONG* 	RS_pn27N; 		 //U0无压定值 27N
extern LONG* 	RS_pn59LA; 		 //ULA有压定值 59LA
extern LONG* 	RS_pn27LA; 		 //ULA无压定值 27LA
extern LONG* 	RS_pn59LB; 		 //ULB有压定值 59LB
extern LONG* 	RS_pn27LB; 		 //ULB无压定值 27LB
extern LONG* 	RS_pn59LC; 		 //ULC有压定值 59LC
extern LONG* 	RS_pn27LC; 		 //ULC无压定值 27LC
extern LONG* 	RS_pn50HA; 		 //IHA有流定值 50HA
extern LONG* 	RS_pn37HA; 		 //IHA无流定值 37HA
extern LONG* 	RS_pn50HB; 		 //IHB有流定值 50HB
extern LONG* 	RS_pn37HB; 		 //IHB无流定值 37HB
extern LONG* 	RS_pn50HC; 		 //IHC有流定值 50HC
extern LONG* 	RS_pn37HC; 		 //IHC无流定值 37HC
extern LONG* 	RS_pn50LA; 		 //ILA有流定值 50LA
extern LONG* 	RS_pn37LA; 		 //ILA无流定值 37LA
extern LONG* 	RS_pn50LB; 		 //ILB有流定值 50LB
extern LONG* 	RS_pn37LB; 		 //ILB无流定值 37LB
extern LONG* 	RS_pn50LC; 		 //ILC有流定值 50LC
extern LONG* 	RS_pn37LC; 		 //ILC无流定值 37LC
extern LONG* 	RS_pn50I1; 		 //I1有流定值 50I1
extern LONG* 	RS_pn37I1; 		 //I1无流定值 37I1
extern LONG* 	RS_pn50I2; 		 //I2有流定值 50I2
extern LONG* 	RS_pn37I2; 		 //I2无流定值 37I2
extern LONG* 	RS_pnTCB; 		 //断路器动作时间 TCB
extern LONG* 	RS_pnTSW; 		 //电动隔开动作时间 TSW

#define RS_COUNT     65    //定值个数/
#define RS_BUF_SIZE     260    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 488)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnTR1D; 		 //跳闸继电器字1保持时间
extern LONG* 	AS_pnTR2D; 		 //跳闸继电器字2保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnHPTRT; 		 //高侧PT变比
extern LONG* 	AS_pnLPTRT; 		 //低侧PT变比
extern LONG* 	AS_pnU0PTRT; 		 //零序PT变比
extern LONG* 	AS_pnHCTRT; 		 //高侧CT变比
extern LONG* 	AS_pnLACTRT; 		 //低侧ACT变比
extern LONG* 	AS_pnLBCTRT; 		 //低侧BCT变比
extern LONG* 	AS_pnLCCTRT; 		 //低侧CCT变比
extern LONG* 	AS_pnI1CTRT; 		 //I1CT变比
extern LONG* 	AS_pnI2CTRT; 		 //I2CT变比
extern LONG* 	AS_pnRT; 		 //RT

#define AS_RT_NUM     10    //辅助定值中变比个数

#define AS_COUNT     14    //辅助定值个数
#define AS_BUF_SIZE     56    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 748)

//系数----------------------------------
extern LONG* 	CF_pnUHA; 		 //UHA
extern LONG* 	CF_pnUHB; 		 //UHB
extern LONG* 	CF_pnUHC; 		 //UHC
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnULA; 		 //ULA
extern LONG* 	CF_pnULB; 		 //ULB
extern LONG* 	CF_pnULC; 		 //ULC
extern LONG* 	CF_pnIHA; 		 //IHA
extern LONG* 	CF_pnIHB; 		 //IHB
extern LONG* 	CF_pnIHC; 		 //IHC
extern LONG* 	CF_pnILA; 		 //ILA
extern LONG* 	CF_pnILB; 		 //ILB
extern LONG* 	CF_pnILC; 		 //ILC
extern LONG* 	CF_pnI1; 		 //I1
extern LONG* 	CF_pnI2; 		 //I2
extern LONG* 	CF_pnPA; 		 //PA
extern LONG* 	CF_pnPB; 		 //PB
extern LONG* 	CF_pnPC; 		 //PC
extern LONG* 	CF_pnQA; 		 //QA
extern LONG* 	CF_pnQB; 		 //QB
extern LONG* 	CF_pnQC; 		 //QC
extern LONG* 	CF_pnAA; 		 //AA
extern LONG* 	CF_pnAB; 		 //AB
extern LONG* 	CF_pnAC; 		 //AC
extern LONG* 	CF_pnUHAD; 		 //UHADRIFT
extern LONG* 	CF_pnUHBD; 		 //UHBDRIFT
extern LONG* 	CF_pnUHCD; 		 //UHCDRIFT
extern LONG* 	CF_pnU0D; 		 //U0DRIFT
extern LONG* 	CF_pnULAD; 		 //ULADRIFT
extern LONG* 	CF_pnULBD; 		 //ULBDRIFT
extern LONG* 	CF_pnULCD; 		 //ULCDRIFT
extern LONG* 	CF_pnIHAD; 		 //IHADRIFT
extern LONG* 	CF_pnIHBD; 		 //IHBDRIFT
extern LONG* 	CF_pnIHCD; 		 //IHCDRIFT
extern LONG* 	CF_pnILAD; 		 //ILADRIFT
extern LONG* 	CF_pnILBD; 		 //ILBDRIFT
extern LONG* 	CF_pnILCD; 		 //ILCDRIFT
extern LONG* 	CF_pnI1D; 		 //I1DRIFT
extern LONG* 	CF_pnI2D; 		 //I2DRIFT
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnUHAH; 		 //UHAH
extern LONG* 	CF_pnUHBH; 		 //UHBH
extern LONG* 	CF_pnUHCH; 		 //UHCH
extern LONG* 	CF_pnIHAH; 		 //IHAH
extern LONG* 	CF_pnIHBH; 		 //IHBH
extern LONG* 	CF_pnIHCH; 		 //IHCH
extern LONG* 	CF_pnUAB; 		 //UAB
extern LONG* 	CF_pnUBC; 		 //UBC
extern LONG* 	CF_pnUCA; 		 //UCA

#define CF_COUNT     52    //系数个数
#define CF_BUF_SIZE     208    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 804)

//报告访问宏-----------------------
#define R_RAMERR                    0    //RAM定值故障
#define R_RESETAN                   1    //装置异常复位
#define R_COMERR                    2    //通信通道异常
#define R_NVRAMERR                  3    //NVRAM内容出错
#define R_ROMERR                    4    //程序ROM故障
#define R_EEPROMERR                 5    //EEPROM定值出错
#define R_ADERR                     6    //AD故障
#define R_OEBR                      7    //开出光隔击穿
#define R_OEERR                     8    //开出光隔失效
#define R_SLERR                     9    //软压板故障
#define R_LOGICERR                 10    //逻辑方程故障
#define R_COMTEST                  11    //通道测试
#define R_RCERR                    12    //保护自检错误
#define R_NVRAMERR_R               13    //NVRAM内容出错恢复
#define R_ROMERR_R                 14    //程序ROM故障恢复
#define R_RAMERR_R                 15    //RAM定值故障恢复
#define R_EEPROMERR_R              16    //EEPROM定值故障恢复
#define R_SLERR_R                  17    //软压板故障恢复
#define R_ADERR_R                  18    //AD故障恢复
#define R_LOGICERR_R               19    //逻辑方程故障故障恢复
#define R_COMERR_R                 20    //通信通道异常恢复
#define R_POWERERR                 21    //5V电源故障
#define R_EEPROM_W_ERR             22    //EEPROM写入故障
#define R_POWERERR_R               23    //5V电源故障恢复
#define R_RELDATAERR               24    //保护数据异常
#define R_RELDATAERR_R             25    //保护数据异常恢复
#define MAX_REPORT_INDEX           25    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     32    //传动个数

#endif