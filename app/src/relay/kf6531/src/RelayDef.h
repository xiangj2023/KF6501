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
//----低侧电流路数-----
#define 	LCUR_2 	0	//低侧两路电流
#define 	LCUR_4 	1	//低侧四路电流

//保护测量值定义-----------------------
#define  RM_UHAB 0
extern LONG* 	RM_pnUHAB; 		 // UHAB
#define  RM_UHBC 1
extern LONG* 	RM_pnUHBC; 		 // UHBC
#define  RM_UHCA 2
extern LONG* 	RM_pnUHCA; 		 // UHCA
#define  RM_UHA 3
extern LONG* 	RM_pnUHA; 		 // UHA
#define  RM_UHB 4
extern LONG* 	RM_pnUHB; 		 // UHB
#define  RM_UHC 5
extern LONG* 	RM_pnUHC; 		 // UHC
#define  RM_U0 6
extern LONG* 	RM_pnU0; 		 // U0
#define  RM_ULA 7
extern LONG* 	RM_pnULA; 		 // ULA
#define  RM_ULB 8
extern LONG* 	RM_pnULB; 		 // ULB
#define  RM_ULAB 9
extern LONG* 	RM_pnULAB; 		 // ULAB
#define  RM_IHA 10
extern LONG* 	RM_pnIHA; 		 // IHA
#define  RM_IHB 11
extern LONG* 	RM_pnIHB; 		 // IHB
#define  RM_IHC 12
extern LONG* 	RM_pnIHC; 		 // IHC
#define  RM_I0 13
extern LONG* 	RM_pnI0; 		 // I0
#define  RM_IJX 14
extern LONG* 	RM_pnIJX; 		 // IJX
#define  RM_ILA 15
extern LONG* 	RM_pnILA; 		 // ILA
#define  RM_ILB 16
extern LONG* 	RM_pnILB; 		 // ILB
#define  RM_IL1 17
extern LONG* 	RM_pnIL1; 		 // IL1
#define  RM_IL2 18
extern LONG* 	RM_pnIL2; 		 // IL2
#define  RM_IL3 19
extern LONG* 	RM_pnIL3; 		 // IL3
#define  RM_IL4 20
extern LONG* 	RM_pnIL4; 		 // IL4
#define  RM_BOBRCH1 21
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 22
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 23
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 24
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     25    //保护测量值个数
#define RM_BUF_SIZE     100    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UHA 0
extern LONG* 	M_pnUHA; 		 //UHA UHA
#define  M_UHB 1
extern LONG* 	M_pnUHB; 		 //UHB UHB
#define  M_UHC 2
extern LONG* 	M_pnUHC; 		 //UHC UHC
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_ULA 4
extern LONG* 	M_pnULA; 		 //ULA ULA
#define  M_ULB 5
extern LONG* 	M_pnULB; 		 //ULB ULB
#define  M_IHA 6
extern LONG* 	M_pnIHA; 		 //IHA IHA
#define  M_IHB 7
extern LONG* 	M_pnIHB; 		 //IHB IHB
#define  M_IHC 8
extern LONG* 	M_pnIHC; 		 //IHC IHC
#define  M_I0 9
extern LONG* 	M_pnI0; 		 //I0 I0
#define  M_ILA 10
extern LONG* 	M_pnILA; 		 //ILA ILA
#define  M_ILB 11
extern LONG* 	M_pnILB; 		 //ILB ILB
#define  M_IJX 12
extern LONG* 	M_pnIjx; 		 //Ijx Ijx
#define  M_IL1 13
extern LONG* 	M_pnIL1; 		 //IL1 IL1
#define  M_IL2 14
extern LONG* 	M_pnIL2; 		 //IL2 IL2
#define  M_IL3 15
extern LONG* 	M_pnIL3; 		 //IL3 IL3
#define  M_IL4 16
extern LONG* 	M_pnIL4; 		 //IL4 IL4

#define MEASURE_COUNT     17    //测量值个数
#define MEASURE_BUF_SIZE     68    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 104)

//软压板----------------------------------
extern BYTE* 	SW_pby50H; 		 //高侧过流保护
extern BYTE* 	SW_pby49; 		 //反时限过负荷
extern BYTE* 	SW_pby50L; 		 //低侧过流保护
extern BYTE* 	SW_pby50OL; 		 //过负荷保护
extern BYTE* 	SW_pby27; 		 //高侧失压保护
extern BYTE* 	SW_pby50N; 		 //零序过流保护
extern BYTE* 	SW_pby59N; 		 //零序过压保护
extern BYTE* 	SW_pby50JX; 		 //间隙过流保护
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2

#define SW_COUNT     11    //软压板个数
#define SW_BUF_SIZE     11    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 172)

//配置----------------------------------
extern BYTE* 	CFG_pby50HA; 		 //高侧A相过流
extern BYTE* 	CFG_pby50HB; 		 //高侧B相过流
extern BYTE* 	CFG_pby50HC; 		 //高侧C相过流
extern BYTE* 	CFG_pby50H_27; 		 //高侧低压启动
extern BYTE* 	CFG_pby49; 		 //反时限过负荷
extern BYTE* 	CFG_pby50LA1; 		 //低侧A相过流I段
extern BYTE* 	CFG_pby50LA2; 		 //低侧A相过流II段
extern BYTE* 	CFG_pby50LA_27; 		 //低侧A低压启动
extern BYTE* 	CFG_pby50LB1; 		 //低侧B相过流I段
extern BYTE* 	CFG_pby50LB2; 		 //低侧B相过流II段
extern BYTE* 	CFG_pby50LB_27; 		 //低侧B低压启动
extern BYTE* 	CFG_pby50OLA1; 		 //A过负荷告警
extern BYTE* 	CFG_pby50OLB1; 		 //B过负荷告警
extern BYTE* 	CFG_pby50OLC1; 		 //C过负荷告警
extern BYTE* 	CFG_pby59OLA2; 		 //A过负荷跳闸
extern BYTE* 	CFG_pby50OLB2; 		 //B过负荷跳闸
extern BYTE* 	CFG_pby50OLC2; 		 //C过负荷跳闸
extern BYTE* 	CFG_pby27; 		 //高压侧失压保护
extern BYTE* 	CFG_pby50N; 		 //零序过流保护
extern BYTE* 	CFG_pby59N; 		 //零序过压保护
extern BYTE* 	CFG_pby74PT; 		 //PT断线检测
extern BYTE* 	CFG_pby50DCG; 		 //间隙过流保护
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2

#define CFG_COUNT     25    //配置个数/
#define CFG_BUF_SIZE     25    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 186)

//定值----------------------------------
extern LONG* 	RS_pnF1; 		 //配置字1 F1
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn50HAC; 		 //高压侧A过流电流 50HAC
extern LONG* 	RS_pn50HAD; 		 //高压侧A过流时间 50HAD
extern LONG* 	RS_pn50HBC; 		 //高压侧B过流电流 50HBC
extern LONG* 	RS_pn50HBD; 		 //高压侧B过流时间 50HBD
extern LONG* 	RS_pn50HCC; 		 //高压侧C过流电流 50HCC
extern LONG* 	RS_pn50HCD; 		 //高压侧C过流时间 50HCD
extern LONG* 	RS_pn27LU; 		 //低压侧低压定值 27LU
extern LONG* 	RS_pnTIN; 		 //变压器二次额定电流 TIN
extern LONG* 	RS_pn49K; 		 //热过负荷保护因子 49K
extern LONG* 	RS_pn49T; 		 //热过负荷时间常数 49T
extern LONG* 	RS_pn49AR; 		 //热过负荷告警定值 49AR
extern LONG* 	RS_pn49KT; 		 //设备停止扩充系数 49KT
extern LONG* 	RS_pn50LA1C; 		 //低侧A相过流I段定值 50LA1C
extern LONG* 	RS_pn50LA1D; 		 //低侧A相过流I段时间 50LA1D
extern LONG* 	RS_pn50LA2C; 		 //低侧A相过流II段定值 50LA2C
extern LONG* 	RS_pn50LA2D; 		 //低侧A相过流II段时间 50LA2D
extern LONG* 	RS_pn50LB1C; 		 //低侧B相过流I段定值 50LB1C
extern LONG* 	RS_pn50LB1D; 		 //低侧B相过流I段时间 50LB1D
extern LONG* 	RS_pn50LB2C; 		 //低侧B相过流II段定值 50LB2C
extern LONG* 	RS_pn50LB2D; 		 //低侧B相过流II段时间 50LB2D
extern LONG* 	RS_pn50OLA1C; 		 //A相过负荷I段电流 50OLA1C
extern LONG* 	RS_pn50OLA1D; 		 //A相过负荷I段时间 50OLA1D
extern LONG* 	RS_pn50OLB1C; 		 //B相过负荷I段电流 50OLB1C
extern LONG* 	RS_pn50OLB1D; 		 //B相过负荷I段时间 50OLB1D
extern LONG* 	RS_pn50OLC1C; 		 //C相过负荷I段电流 50OLC1C
extern LONG* 	RS_pn50OLC1D; 		 //C相过负荷I段时间 50OLC1D
extern LONG* 	RS_pn50OLA2C; 		 //A相过负荷II段电流 50OLA2C
extern LONG* 	RS_pn50OLA2D; 		 //A相过负荷II段时间 50OLA2D
extern LONG* 	RS_pn50OLB2C; 		 //B相过负荷II段电流 50OLB2C
extern LONG* 	RS_pn50OLB2D; 		 //B相过负荷II段时间 50OLB2D
extern LONG* 	RS_pn50OLC2C; 		 //C相过负荷II段电流 50OLC2C
extern LONG* 	RS_pn50OLC2D; 		 //C相过负荷II段时间 50OLC2D
extern LONG* 	RS_pn50NC; 		 //零序过流定值 50NC
extern LONG* 	RS_pn50ND; 		 //零序过流时间 50ND
extern LONG* 	RS_pn59NV; 		 //零序过压定值 59NV
extern LONG* 	RS_pn59ND; 		 //零序过压时间 59ND
extern LONG* 	RS_pn27HV; 		 //高压侧失压定值 27HV
extern LONG* 	RS_pn27HD; 		 //高压侧失压时间 27HD
extern LONG* 	RS_pn27H_LK50; 		 //高侧失压电流闭锁 27H_LK50
extern LONG* 	RS_pn74HPTV; 		 //高压侧PT断线定值 74HPTV
extern LONG* 	RS_pn74LPTV; 		 //低压侧PT断线定值 74LPTV
extern LONG* 	RS_pn50DCGC; 		 //间隙过流保护电流 50DCGC
extern LONG* 	RS_pn50DCGD; 		 //间隙过流保护时间 50DCGD
extern LONG* 	RS_pn74PTHC; 		 //高侧PT断线闭锁电流 74PTHC
extern LONG* 	RS_pn74PTLC; 		 //低侧PT断线闭锁电流 74PTLC
extern LONG* 	RS_pn74PTD; 		 //PT断线判断时间 74PTD
extern LONG* 	RS_pn52BFD; 		 //失灵保护判断时间 52BFD

#define RS_COUNT     49    //定值个数/
#define RS_BUF_SIZE     196    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 212)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnLCN; 		 //低侧电流路数
extern LONG* 	AS_pnT1PI; 		 //T1启动延时
extern LONG* 	AS_pnT1RE; 		 //T1返回延时
extern LONG* 	AS_pnT2PI; 		 //T2启动延时
extern LONG* 	AS_pnT2RE; 		 //T2返回延时
extern LONG* 	AS_pnT3PI; 		 //T3启动延时
extern LONG* 	AS_pnT3RE; 		 //T3返回延时
extern LONG* 	AS_pnT4PI; 		 //T4启动延时
extern LONG* 	AS_pnT4RE; 		 //T4返回延时
extern LONG* 	AS_pnT5PI; 		 //T5启动延时
extern LONG* 	AS_pnT5RE; 		 //T5返回延时
extern LONG* 	AS_pnT6PI; 		 //T6启动延时
extern LONG* 	AS_pnT6RE; 		 //T6返回延时
extern LONG* 	AS_pnT7PI; 		 //T7启动延时
extern LONG* 	AS_pnT7RE; 		 //T7返回延时
extern LONG* 	AS_pnT8PI; 		 //T8启动延时
extern LONG* 	AS_pnT8RE; 		 //T8返回延时
extern LONG* 	AS_pnT9PI; 		 //T9启动延时
extern LONG* 	AS_pnT9RE; 		 //T9返回延时
extern LONG* 	AS_pnT10PI; 		 //T10启动延时
extern LONG* 	AS_pnT10RE; 		 //T10返回延时
extern LONG* 	AS_pnT11PI; 		 //T11启动延时
extern LONG* 	AS_pnT11RE; 		 //T11返回延时
extern LONG* 	AS_pnT12PI; 		 //T12启动延时
extern LONG* 	AS_pnT12RE; 		 //T12返回延时
extern LONG* 	AS_pnT13PI; 		 //T13启动延时
extern LONG* 	AS_pnT13RE; 		 //T13返回延时
extern LONG* 	AS_pnT14PI; 		 //T14启动延时
extern LONG* 	AS_pnT14RE; 		 //T14返回延时
extern LONG* 	AS_pnT15PI; 		 //T15启动延时
extern LONG* 	AS_pnT15RE; 		 //T15返回延时
extern LONG* 	AS_pnT16PI; 		 //T16启动延时
extern LONG* 	AS_pnT16RE; 		 //T16返回延时
extern LONG* 	AS_pn50HAC; 		 //IHA有流定值
extern LONG* 	AS_pn37HAC; 		 //IHA无流定值
extern LONG* 	AS_pn50HBC; 		 //IHB有流定值
extern LONG* 	AS_pn37HBC; 		 //IHB无流定值
extern LONG* 	AS_pn50HCC; 		 //IHC有流定值
extern LONG* 	AS_pn37HCC; 		 //IHC无流定值
extern LONG* 	AS_pn59HAV; 		 //UHA有压定值
extern LONG* 	AS_pn27HAV; 		 //UHA无压定值
extern LONG* 	AS_pn59HBV; 		 //UHB有压定值
extern LONG* 	AS_pn27HBV; 		 //UHB无压定值
extern LONG* 	AS_pn59HCV; 		 //UHC有压定值
extern LONG* 	AS_pn27HCV; 		 //UHC无压定值
extern LONG* 	AS_pn50LAC; 		 //ILA有流定值
extern LONG* 	AS_pn37LAC; 		 //ILA无流定值
extern LONG* 	AS_pn50LBC; 		 //ILB有流定值
extern LONG* 	AS_pn37LBC; 		 //ILB无流定值
extern LONG* 	AS_pn59LAV; 		 //ULA有压定值
extern LONG* 	AS_pn27LAV; 		 //ULA无压定值
extern LONG* 	AS_pn59LBV; 		 //ULB有压定值
extern LONG* 	AS_pn27LBV; 		 //ULB无压定值
extern LONG* 	AS_pn50NC; 		 //I0有流定值
extern LONG* 	AS_pn37NC; 		 //I0无流定值
extern LONG* 	AS_pn59NV; 		 //U0有压定值
extern LONG* 	AS_pn27NV; 		 //U0无压定值
extern LONG* 	AS_pn50JXC; 		 //IJX有流定值
extern LONG* 	AS_pn37JXC; 		 //IJX无流定值
extern LONG* 	AS_pnTCB; 		 //断路器动作延时
extern LONG* 	AS_pnTSW; 		 //隔开动作延时
extern LONG* 	AS_pnTR1T; 		 //TR1保持时间
extern LONG* 	AS_pnTR2T; 		 //TR2保持时间
extern LONG* 	AS_pnTR3T; 		 //TR3保持时间
extern LONG* 	AS_pnTR4T; 		 //TR4保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnHPTRT; 		 //高侧PT变比
extern LONG* 	AS_pnHU0PTRT; 		 //高侧零序PT变比
extern LONG* 	AS_pnHCTRT; 		 //高侧CT变比
extern LONG* 	AS_pnHI0CTRT; 		 //高侧零序CT变比
extern LONG* 	AS_pnLPTRT; 		 //低侧PT变比
extern LONG* 	AS_pnLCT1RT; 		 //低侧CT1变比
extern LONG* 	AS_pnLCT2RT; 		 //低侧CT2变比
extern LONG* 	AS_pnLCT3RT; 		 //低侧CT3变比
extern LONG* 	AS_pnLCT4RT; 		 //低侧CT4变比
extern LONG* 	AS_pnRT1; 		 //RT1

#define AS_RT_NUM     10    //辅助定值中变比个数

#define AS_COUNT     77    //辅助定值个数
#define AS_BUF_SIZE     308    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 408)

//系数----------------------------------
extern LONG* 	CF_pnUHAB; 		 //UHAB
extern LONG* 	CF_pnUHBC; 		 //UHBC
extern LONG* 	CF_pnUHCA; 		 //UHCA
extern LONG* 	CF_pnUHA; 		 //UHA
extern LONG* 	CF_pnUHB; 		 //UHB
extern LONG* 	CF_pnUHC; 		 //UHC
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnULA; 		 //ULA
extern LONG* 	CF_pnULB; 		 //ULB
extern LONG* 	CF_pnIHA; 		 //IHA
extern LONG* 	CF_pnIHB; 		 //IHB
extern LONG* 	CF_pnIHC; 		 //IHC
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnIL1; 		 //IL1
extern LONG* 	CF_pnIL2; 		 //IL2
extern LONG* 	CF_pnIL3; 		 //IL3
extern LONG* 	CF_pnIL4; 		 //IL4
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnUHAM; 		 //UHAM
extern LONG* 	CF_pnUHBM; 		 //UHBM
extern LONG* 	CF_pnUHCM; 		 //UHCM
extern LONG* 	CF_pnU0M; 		 //U0M
extern LONG* 	CF_pnULAM; 		 //ULAM
extern LONG* 	CF_pnULBM; 		 //ULBM
extern LONG* 	CF_pnIHAM; 		 //IHAM
extern LONG* 	CF_pnIHBM; 		 //IHBM
extern LONG* 	CF_pnIHCM; 		 //IHCM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnIL1M; 		 //IL1M
extern LONG* 	CF_pnIL2M; 		 //IL2M
extern LONG* 	CF_pnIL3M; 		 //IL3M
extern LONG* 	CF_pnIL4M; 		 //IL4M
extern LONG* 	CF_pnUHAD; 		 //UHAD
extern LONG* 	CF_pnUHBD; 		 //UHBD
extern LONG* 	CF_pnUHCD; 		 //UHCD
extern LONG* 	CF_pnU0D; 		 //U0D
extern LONG* 	CF_pnULAD; 		 //ULAD
extern LONG* 	CF_pnULBD; 		 //ULBD
extern LONG* 	CF_pnIHAD; 		 //IHAD
extern LONG* 	CF_pnIHBD; 		 //IHBD
extern LONG* 	CF_pnIHCD; 		 //IHCD
extern LONG* 	CF_pnI0D; 		 //I0D
extern LONG* 	CF_pnIL1D; 		 //IL1D
extern LONG* 	CF_pnIL2D; 		 //IL2D
extern LONG* 	CF_pnIL3D; 		 //IL3D
extern LONG* 	CF_pnIL4D; 		 //IL4D

#define CF_COUNT     49    //系数个数
#define CF_BUF_SIZE     196    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 716)

//报告访问宏-----------------------
#define R_OEERR                     0    //开出光隔失效
#define R_OEBR                      1    //开出光隔击穿
#define R_ADERR                     2    //AD故障
#define R_SLERR                     3    //软压板故障
#define R_EEPROMERR                 4    //EEPROM定值故障
#define R_RAMERR                    5    //RAM定值故障
#define R_ROMERR                    6    //程序ROM故障
#define R_RESETAN                   7    //装置异常复位
#define R_COMERR                    8    //通信通道异常
#define R_NVRAMERR                  9    //NVRAM内容出错
#define R_LOGICERR                 10    //逻辑方程故障
#define R_RCERR                    11    //保护自检错误
#define R_POWERERR                 12    //5V电源故障
#define R_EEPROM_W_ERR             13    //EEPROM写入故障
#define R_ADERR_R                  14    //AD故障恢复
#define R_SLERR_R                  15    //软压板故障恢复
#define R_EEPROMERR_R              16    //EEPROM定值故障恢复
#define R_RAMERR_R                 17    //RAM定值故障恢复
#define R_ROMERR_R                 18    //程序ROM故障恢复
#define R_COMERR_R                 19    //通信通道异常恢复
#define R_NVRAMERR_R               20    //NVRAM内容出错恢复
#define R_LOGICERR_R               21    //逻辑方程故障故障恢复
#define R_POWERERR_R               22    //5V电源故障恢复
#define R_ACTPICK                  23    //保护启动
#define R_R49AR                    24    //反时限过负荷告警
#define R_R49ACT                   25    //反时限过负荷动作
#define R_R49RST                   26    //反时限过负荷返回
#define R_R50IHA_PI                27    //高压侧A过流启动
#define R_R50IHA                   28    //高压侧A过流动作
#define R_R50IHA_RES               29    //高压侧A过流返回
#define R_R50IHB_PI                30    //高压侧B过流启动
#define R_R50IHB                   31    //高压侧B过流动作
#define R_R50IHB_RES               32    //高压侧B过流返回
#define R_R50IHC_PI                33    //高压侧C过流启动
#define R_R50IHC                   34    //高压侧C过流动作
#define R_R50IHC_RES               35    //高压侧C过流返回
#define R_R50ILA1_PI               36    //低压侧A过流Ⅰ段启动
#define R_R50ILA1                  37    //低压侧A过流Ⅰ段动作
#define R_R50ILA1_RES              38    //低压侧A过流Ⅰ段返回
#define R_R50ILA2_PI               39    //低压侧A过流Ⅱ段启动
#define R_R50ILA2                  40    //低压侧A过流Ⅱ段动作
#define R_R50ILA2_RES              41    //低压侧A过流Ⅱ段返回
#define R_R50ILB1_PI               42    //低压侧B过流Ⅰ段启动
#define R_R50ILB1                  43    //低压侧B过流Ⅰ段动作
#define R_R50ILB1_RES              44    //低压侧B过流Ⅰ段返回
#define R_R50ILB2_PI               45    //低压侧B过流Ⅱ段启动
#define R_R50ILB2                  46    //低压侧B过流Ⅱ段动作
#define R_R50ILB2_RES              47    //低压侧B过流Ⅱ段返回
#define R_R50IJX_PI                48    //间隙过流保护启动
#define R_R50IJX                   49    //间隙过流保护动作
#define R_R50IJX_RES               50    //间隙过流保护返回
#define R_R50IN_PI                 51    //零序过流保护启动
#define R_R50IN                    52    //零序过流保护动作
#define R_R50IN_RES                53    //零序过流保护返回
#define R_R59UN_PI                 54    //零序过压保护启动
#define R_R59UN                    55    //零序过压保护动作
#define R_R59UN_RES                56    //零序过压保护返回
#define R_R50IA1                   57    //高侧A过负荷Ⅰ告警
#define R_R50IB1                   58    //高侧B过负荷Ⅰ告警
#define R_R50IC1                   59    //高侧C过负荷Ⅰ告警
#define R_R50IA2_PI                60    //高侧A过负荷Ⅱ启动
#define R_R50IA2                   61    //高侧A过负荷Ⅱ动作
#define R_R50IA2_RES               62    //高侧A过负荷Ⅱ返回
#define R_R50IB2_PI                63    //高侧B过负荷Ⅱ启动
#define R_R50IB2                   64    //高侧B过负荷Ⅱ动作
#define R_R50IB2_RES               65    //高侧B过负荷Ⅱ返回
#define R_R50IC2_PI                66    //高侧C过负荷Ⅱ启动
#define R_R50IC2                   67    //高侧C过负荷Ⅱ动作
#define R_R50IC2_RES               68    //高侧C过负荷Ⅱ返回
#define R_R27U                     69    //高压侧失压动作
#define R_RHPTBR                   70    //高压侧PT断线
#define R_RHPTBRRST                71    //高压侧PT断线恢复
#define R_RLAPTBR                  72    //低压侧A相PT断线
#define R_RLAPTBRRST               73    //低压侧A相PT断线恢复
#define R_RLBPTBR                  74    //低压侧B相PT断线
#define R_RLBPTBRRST               75    //低压侧B相PT断线恢复
#define R_PICKUP                   76    //保护启动
#define R_TR_H_FAIL                77    //高侧断路器跳闸失败
#define R_COMTEST                  78    //通道测试
#define R_52BF                     79    //失灵保护
#define R_TR_LA_FAIL               80    //低侧A相断路器跳闸失败
#define R_TR_LB_FAIL               81    //低侧B相断路器跳闸失败
#define R_RELDATAERR               82    //保护数据异常
#define R_RELDATAERR_R             83    //保护数据异常恢复
#define MAX_REPORT_INDEX           83    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     21    //传动个数

#endif