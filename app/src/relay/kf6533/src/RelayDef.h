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
#define 	 FAILDATA_DISPTYPE_CFGNO 	 6 		 //故障电量显示类型配置序号
#define 	 LangTYPE_SYSSETNO 	 0 		 //语言类型辅助定值序号

//当前定值区号定义-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //当前定值区号

//定值取值宏定义

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//保护测量值定义-----------------------
#define  RM_UHA 0
extern LONG* 	RM_pnUHA; 		 //UA UA
#define  RM_UHB 1
extern LONG* 	RM_pnUHB; 		 //UB UB
#define  RM_UHC 2
extern LONG* 	RM_pnUHC; 		 //UC UC
#define  RM_UAB 3
extern LONG* 	RM_pnUAB; 		 //UAB UAB
#define  RM_UBC 4
extern LONG* 	RM_pnUBC; 		 //UBC UBC
#define  RM_UCA 5
extern LONG* 	RM_pnUCA; 		 //UCA UCA
#define  RM_U0H 6
extern LONG* 	RM_pnU0H; 		 //U0H U0H
#define  RM_ULA 7
extern LONG* 	RM_pnULa; 		 //Ua Ua
#define  RM_ULB 8
extern LONG* 	RM_pnULb; 		 //Ub Ub
#define  RM_ULC 9
extern LONG* 	RM_pnULc; 		 //Uc Uc
#define  RM_ULAB 10
extern LONG* 	RM_pnULab; 		 // Uab
#define  RM_ULBC 11
extern LONG* 	RM_pnULbc; 		 // Ubc
#define  RM_ULCA 12
extern LONG* 	RM_pnULca; 		 // Uca
#define  RM_U0L 13
extern LONG* 	RM_pnU0L; 		 //U0L U0L
#define  RM_IHA 14
extern LONG* 	RM_pnIHA; 		 //IA IA
#define  RM_IHB 15
extern LONG* 	RM_pnIHB; 		 //IB IB
#define  RM_IHC 16
extern LONG* 	RM_pnIHC; 		 //IC IC
#define  RM_ILA 17
extern LONG* 	RM_pnILa; 		 //Ia Ia
#define  RM_ILB 18
extern LONG* 	RM_pnILb; 		 //Ib Ib
#define  RM_ILC 19
extern LONG* 	RM_pnILc; 		 //Ic Ic
#define  RM_I0 20
extern LONG* 	RM_pnI0; 		 //I0 I0
#define  RM_IJX 21
extern LONG* 	RM_pnIjx; 		 //Ijx Ijx
#define  RM_U2H 22
extern LONG* 	RM_pnU2H; 		 //U2H U2H
#define  RM_U2L 23
extern LONG* 	RM_pnU2L; 		 //U2L U2L
#define  RM_IDA 24
extern LONG* 	RM_pnIDA; 		 //IDA IDA
#define  RM_IDB 25
extern LONG* 	RM_pnIDB; 		 //IDB IDB
#define  RM_IDC 26
extern LONG* 	RM_pnIDC; 		 //IDC IDC
#define  RM_IRA 27
extern LONG* 	RM_pnIRA; 		 //IRA IRA
#define  RM_IRB 28
extern LONG* 	RM_pnIRB; 		 //IRB IRB
#define  RM_IRC 29
extern LONG* 	RM_pnIRC; 		 //IRC IRC
#define  RM_IDA2 30
extern LONG* 	RM_pnIDA2; 		 //IDA2 IDA2
#define  RM_IDA3 31
extern LONG* 	RM_pnIDA3; 		 //IDA3 IDA3
#define  RM_IDA5 32
extern LONG* 	RM_pnIDA5; 		 //IDA5 IDA5
#define  RM_IDB2 33
extern LONG* 	RM_pnIDB2; 		 //IDB2 IDB2
#define  RM_IDB3 34
extern LONG* 	RM_pnIDB3; 		 //IDB3 IDB3
#define  RM_IDB5 35
extern LONG* 	RM_pnIDB5; 		 //IDB5 IDB5
#define  RM_IDC2 36
extern LONG* 	RM_pnIDC2; 		 //IDC2 IDC2
#define  RM_IDC3 37
extern LONG* 	RM_pnIDC3; 		 //IDC3 IDC3
#define  RM_IDC5 38
extern LONG* 	RM_pnIDC5; 		 //IDC5 IDC5
#define  RM_IRA2 39
extern LONG* 	RM_pnIRA2; 		 //IRA2 IRA2
#define  RM_IRB2 40
extern LONG* 	RM_pnIRB2; 		 //IRB2 IRB2
#define  RM_IRC2 41
extern LONG* 	RM_pnIRC2; 		 //IRC2 IRC2
#define  RM_AUHA 42
extern LONG* 	RM_pnAUHA; 		 //AUA ΦUA
#define  RM_AUHB 43
extern LONG* 	RM_pnAUHB; 		 //AUB ΦUB
#define  RM_AUHC 44
extern LONG* 	RM_pnAUHC; 		 //AUC ΦUC
#define  RM_AULA 45
extern LONG* 	RM_pnAULa; 		 //AUa ΦUa
#define  RM_AULB 46
extern LONG* 	RM_pnAULb; 		 //AUb ΦUb
#define  RM_AULC 47
extern LONG* 	RM_pnAULc; 		 //AUc ΦUc
#define  RM_AIHA 48
extern LONG* 	RM_pnAIHA; 		 //AIA ΦIA
#define  RM_AIHB 49
extern LONG* 	RM_pnAIHB; 		 //AIB ΦIB
#define  RM_AIHC 50
extern LONG* 	RM_pnAIHC; 		 //AIC ΦIC
#define  RM_AILA 51
extern LONG* 	RM_pnAILa; 		 //AIa ΦIa
#define  RM_AILB 52
extern LONG* 	RM_pnAILb; 		 //AIb ΦIb
#define  RM_AILC 53
extern LONG* 	RM_pnAILc; 		 //AIc ΦIc
#define  RM_BOBRCH1 54
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 55
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 56
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 57
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     58    //保护测量值个数
#define RM_BUF_SIZE     232    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UHA 0
extern LONG* 	M_pnUHA; 		 //UA UA
#define  M_UHB 1
extern LONG* 	M_pnUHB; 		 //UB UB
#define  M_UHC 2
extern LONG* 	M_pnUHC; 		 //UC UC
#define  M_U0H 3
extern LONG* 	M_pnU0h; 		 //U0h U0h
#define  M_ULA 4
extern LONG* 	M_pnUla; 		 //Ua Ua
#define  M_ULB 5
extern LONG* 	M_pnUlb; 		 //Ub Ub
#define  M_ULC 6
extern LONG* 	M_pnUlc; 		 //Uc Uc
#define  M_U0L 7
extern LONG* 	M_pnU0l; 		 //U0l U0l
#define  M_IHA 8
extern LONG* 	M_pnIHA; 		 //IA IA
#define  M_IHB 9
extern LONG* 	M_pnIHB; 		 //IB IB
#define  M_IHC 10
extern LONG* 	M_pnIHC; 		 //IC IC
#define  M_ILA 11
extern LONG* 	M_pnIla; 		 //Ia Ia
#define  M_ILB 12
extern LONG* 	M_pnIlb; 		 //Ib Ib
#define  M_ILC 13
extern LONG* 	M_pnIlc; 		 //Ic Ic
#define  M_I0 14
extern LONG* 	M_pnI0; 		 //I0 I0
#define  M_IJX 15
extern LONG* 	M_pnIjx; 		 //Ijx Ijx
#define  M_P 16
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 17
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 18
extern LONG* 	M_pnCOS; 		 //COS COS

#define MEASURE_COUNT     19    //测量值个数
#define MEASURE_BUF_SIZE     76    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 236)

//软压板----------------------------------
extern BYTE* 	SW_pby87U; 		 //差动速断保护
extern BYTE* 	SW_pby87R; 		 //比率差动保护
extern BYTE* 	SW_pby50I_H; 		 //高压侧过流保护
extern BYTE* 	SW_pby50I_L; 		 //低压侧过流保护
extern BYTE* 	SW_pby50I_SH; 		 //母线充电保护
extern BYTE* 	SW_pby50N; 		 //零序过流保护
extern BYTE* 	SW_pby59N; 		 //零序过压保护
extern BYTE* 	SW_pby50OL; 		 //过负荷保护
extern BYTE* 	SW_pby50I_JX; 		 //间隙过流保护
extern BYTE* 	SW_pby49I; 		 //反时限过负荷
extern BYTE* 	SW_pby27U; 		 //失压保护
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2

#define SW_COUNT     14    //软压板个数
#define SW_BUF_SIZE     14    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 312)

//配置----------------------------------
extern BYTE* 	CFG_pby87U; 		 //差流速断保护
extern BYTE* 	CFG_pby87R; 		 //比率差动保护
extern BYTE* 	CFG_pby68IDH2MAXLK; 		 //二次谐波最大相闭锁
extern BYTE* 	CFG_pby68IDH2MULLK; 		 //二次谐波综合相闭锁
extern BYTE* 	CFG_pby68IDH2SPLLK; 		 //二次谐波分相闭锁
extern BYTE* 	CFG_pby68IRH2LK; 		 //制动电流二次谐波闭锁
extern BYTE* 	CFG_pby68IDMULLK; 		 //综合谐波闭锁
extern BYTE* 	CFG_pbyHCT1CLOCK; 		 //高压侧CT1点校正
extern BYTE* 	CFG_pbyHCT11CLOCK; 		 //高压侧CT11点校正
extern BYTE* 	CFG_pbyLCT1CLOCK; 		 //低压侧CT1点校正
extern BYTE* 	CFG_pbyLCT11CLOCK; 		 //低压侧CT11点校正
extern BYTE* 	CFG_pbyHAB; 		 //高压侧两相
extern BYTE* 	CFG_pby50I_H1; 		 //高压侧过流Ⅰ段
extern BYTE* 	CFG_pby50I_H1_PD; 		 //高压侧过流Ⅰ段功率方向
extern BYTE* 	CFG_pby50I_H1_47U; 		 //高压侧过流Ⅰ段复压闭锁
extern BYTE* 	CFG_pby50I_H2; 		 //高压侧过流Ⅱ段
extern BYTE* 	CFG_pby50I_H2_PD; 		 //高压侧过流Ⅱ段功率方向
extern BYTE* 	CFG_pby50I_H2_47U; 		 //高压侧过流Ⅱ段复压闭锁
extern BYTE* 	CFG_pby50I_H3; 		 //高压侧过流Ⅲ段
extern BYTE* 	CFG_pby50I_H3_PD; 		 //高压侧过流Ⅲ段功率方向
extern BYTE* 	CFG_pby50I_H3_47U; 		 //高压侧过流Ⅲ段复压闭锁
extern BYTE* 	CFG_pby50I_L1; 		 //低压侧过流Ⅰ段
extern BYTE* 	CFG_pby50I_L1_PD; 		 //低压侧过流Ⅰ段功率方向
extern BYTE* 	CFG_pby50I_L1_47U; 		 //低压侧过流Ⅰ段复压闭锁
extern BYTE* 	CFG_pby50I_L2; 		 //低压侧过流Ⅱ段
extern BYTE* 	CFG_pby50I_L2_PD; 		 //低压侧过流Ⅱ段功率方向
extern BYTE* 	CFG_pby50I_L2_47U; 		 //低压侧过流Ⅱ段复压闭锁
extern BYTE* 	CFG_pby50I_SH; 		 //母线充电保护
extern BYTE* 	CFG_pby49I; 		 //反时限过负荷保护
extern BYTE* 	CFG_pby50I_N1; 		 //零序过流Ⅰ段
extern BYTE* 	CFG_pby50I_N2; 		 //零序过流Ⅱ段
extern BYTE* 	CFG_pby59U_NH; 		 //高压侧零序过压保护
extern BYTE* 	CFG_pby59U_NL; 		 //低压侧零序过压告警
extern BYTE* 	CFG_pby50OL_1; 		 //过负荷Ⅰ段
extern BYTE* 	CFG_pby50OL_2; 		 //过负荷Ⅱ段
extern BYTE* 	CFG_pby50OL_3; 		 //过负荷Ⅲ段
extern BYTE* 	CFG_pby50OL_A; 		 //过负荷告警
extern BYTE* 	CFG_pby50I_JX; 		 //间隙过流保护
extern BYTE* 	CFG_pby27U; 		 //失压保护
extern BYTE* 	CFG_pby27U_50I; 		 //失压保护电流闭锁
extern BYTE* 	CFG_pbyPTCK; 		 //PT断线检测
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2

#define CFG_COUNT     44    //配置个数/
#define CFG_BUF_SIZE     44    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 328)

//定值----------------------------------
extern LONG* 	RS_pnFUNCTION1; 		 //配置字1 FUNCTION1
extern LONG* 	RS_pnFUNCTION2; 		 //配置字2 FUNCTION2
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn87UC; 		 //差动速断电流 87UC
extern LONG* 	RS_pn87RC; 		 //差动门槛电流 87RC
extern LONG* 	RS_pnIRS1; 		 //比率制动拐点1 IRS1
extern LONG* 	RS_pnIRS2; 		 //比率制动拐点2 IRS2
extern LONG* 	RS_pnSLP1; 		 //比率制动系数1 SLP1
extern LONG* 	RS_pnSLP2; 		 //比率制动系数2 SLP2
extern LONG* 	RS_pnkrh2; 		 //二次谐波制动系数 krh2
extern LONG* 	RS_pnkrh; 		 //综合谐波制动系数 krh
extern LONG* 	RS_pnkb; 		 //平衡系数 kb
extern LONG* 	RS_pn50IH1C; 		 //高压侧过流Ⅰ段电流 50IH1C
extern LONG* 	RS_pn50IH1D; 		 //高压侧过流Ⅰ段时间 50IH1D
extern LONG* 	RS_pn50IH2C; 		 //高压侧过流Ⅱ段电流 50IH2C
extern LONG* 	RS_pn50IH2D; 		 //高压侧过流Ⅱ段时间 50IH2D
extern LONG* 	RS_pn50IH3C; 		 //高压侧过流Ⅲ段电流 50IH3C
extern LONG* 	RS_pn50IH3D; 		 //高压侧过流Ⅲ段时间 50IH3D
extern LONG* 	RS_pn50IL1C; 		 //低压侧过流Ⅰ段电流 50IL1C
extern LONG* 	RS_pn50IL1D; 		 //低压侧过流Ⅰ段时间 50IL1D
extern LONG* 	RS_pn50IL2C; 		 //低压侧过流Ⅱ段电流 50IL2C
extern LONG* 	RS_pn50IL2D; 		 //低压侧过流Ⅱ段时间 50IL2D
extern LONG* 	RS_pn47U_27U1; 		 //复压闭锁低电压 47U_27U1
extern LONG* 	RS_pn47U_59U2; 		 //复压闭锁负序电压 47U_59U2
extern LONG* 	RS_pnA_50I; 		 //过流保护灵敏角 A_50I
extern LONG* 	RS_pn50ISHC; 		 //母充保护电流 50ISHC
extern LONG* 	RS_pn50ISHD; 		 //母充保护时间 50ISHD
extern LONG* 	RS_pn50ISHT; 		 //母充保护投入时间 50ISHT
extern LONG* 	RS_pn50IN1C; 		 //零序过流Ⅰ段电流 50IN1C
extern LONG* 	RS_pn50IN1D; 		 //零序过流Ⅰ段时间 50IN1D
extern LONG* 	RS_pn50IN2C; 		 //零序过流Ⅱ段电流 50IN2C
extern LONG* 	RS_pn50IN2D; 		 //零序过流Ⅱ段时间 50IN2D
extern LONG* 	RS_pn59UNHV; 		 //高压侧零序过压电压 59UNHV
extern LONG* 	RS_pn59UNHD; 		 //高压侧零序过压时间 59UNHD
extern LONG* 	RS_pn59UNLV; 		 //低压侧零压告警电压 59UNLV
extern LONG* 	RS_pn59UNLD; 		 //低压侧零压告警时间 59UNLD
extern LONG* 	RS_pn50OL1C; 		 //过负荷Ⅰ段电流 50OL1C
extern LONG* 	RS_pn50OL1D; 		 //过负荷Ⅰ段时间 50OL1D
extern LONG* 	RS_pn50OL2C; 		 //过负荷Ⅱ段电流 50OL2C
extern LONG* 	RS_pn50OL2D; 		 //过负荷Ⅱ段时间 50OL2D
extern LONG* 	RS_pn50OL3C; 		 //过负荷Ⅲ段电流 50OL3C
extern LONG* 	RS_pn50OL3D; 		 //过负荷Ⅲ段时间 50OL3D
extern LONG* 	RS_pn50OLAC; 		 //过负荷告警电流 50OLAC
extern LONG* 	RS_pn50OLAD; 		 //过负荷告警时间 50OLAD
extern LONG* 	RS_pn50I_JXC; 		 //间隙过流电流 50I_JXC
extern LONG* 	RS_pn50I_JXD; 		 //间隙过流时间 50I_JXD
extern LONG* 	RS_pnRAC; 		 //变压器二次额定电流 RAC
extern LONG* 	RS_pn49K; 		 //热过负荷保护因子 49K
extern LONG* 	RS_pn49T; 		 //热过负荷时间常数 49T
extern LONG* 	RS_pn49AR; 		 //热过负荷告警比率 49AR
extern LONG* 	RS_pn49KT; 		 //设备停止扩充系数 49KT
extern LONG* 	RS_pn27V; 		 //失压保护电压 27V
extern LONG* 	RS_pn27D; 		 //失压保护时间 27D
extern LONG* 	RS_pn27U_50I; 		 //失压保护电流闭锁定值 27U_50I
extern LONG* 	RS_pn74VTC; 		 //PT断线检测电流 74VTC
extern LONG* 	RS_pn74VTV; 		 //PT断线检测电压 74VTV
extern LONG* 	RS_pn74VTD; 		 //PT断线检测时间 74VTD
extern LONG* 	RS_pnK87AR; 		 //差流越限报警比率 K87AR
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD

#define RS_COUNT     59    //定值个数/
#define RS_BUF_SIZE     236    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 372)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
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
extern LONG* 	AS_pnTCB; 		 //断路器动作延时
extern LONG* 	AS_pnTSW; 		 //电动隔开动作延时
extern LONG* 	AS_pn50IAC; 		 //IA有流定值
extern LONG* 	AS_pn37IAC; 		 //IA无流定值
extern LONG* 	AS_pn50IBC; 		 //IB有流定值
extern LONG* 	AS_pn37IBC; 		 //IB无流定值
extern LONG* 	AS_pn50ICC; 		 //IC有流定值
extern LONG* 	AS_pn37ICC; 		 //IC无流定值
extern LONG* 	AS_pn50IaC; 		 //Ia有流定值
extern LONG* 	AS_pn37IaC; 		 //Ia无流定值
extern LONG* 	AS_pn50IbC; 		 //Ib有流定值
extern LONG* 	AS_pn37IbC; 		 //Ib无流定值
extern LONG* 	AS_pn50IcC; 		 //Ic有流定值
extern LONG* 	AS_pn37IcC; 		 //Ic无流定值
extern LONG* 	AS_pn50I0C; 		 //I0有流定值
extern LONG* 	AS_pn37I0C; 		 //I0无流定值
extern LONG* 	AS_pn50IjxC; 		 //Ijx有流定值
extern LONG* 	AS_pn37IjxC; 		 //Ijx无流定值
extern LONG* 	AS_pn59UAV; 		 //UA有压定值
extern LONG* 	AS_pn27UAV; 		 //UA无压定值
extern LONG* 	AS_pn59UBV; 		 //UB有压定值
extern LONG* 	AS_pn27UBV; 		 //UB无压定值
extern LONG* 	AS_pn59UCV; 		 //UC有压定值
extern LONG* 	AS_pn27UCV; 		 //UC无压定值
extern LONG* 	AS_pn59U0hV; 		 //U0h有压定值
extern LONG* 	AS_pn27U0hV; 		 //U0h无压定值
extern LONG* 	AS_pn59UaV; 		 //Ua有压定值
extern LONG* 	AS_pn27UaV; 		 //Ua无压定值
extern LONG* 	AS_pn59UbV; 		 //Ub有压定值
extern LONG* 	AS_pn27UbV; 		 //Ub无压定值
extern LONG* 	AS_pn59UcV; 		 //Uc有压定值
extern LONG* 	AS_pn27UcV; 		 //Uc无压定值
extern LONG* 	AS_pn59U0lV; 		 //U0l有压定值
extern LONG* 	AS_pn27U0lV; 		 //U0l无压定值
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnTR3D; 		 //TR3保持时间
extern LONG* 	AS_pnTR4D; 		 //TR4保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnHPTRatio; 		 //HPT变比
extern LONG* 	AS_pnH0PTRatio; 		 //H0PT变比
extern LONG* 	AS_pnLPTRatio; 		 //LPT变比
extern LONG* 	AS_pnL0PTRatio; 		 //L0PT变比
extern LONG* 	AS_pnHCTRatio; 		 //HCT变比
extern LONG* 	AS_pnLCTRatio; 		 //LCT变比
extern LONG* 	AS_pnL0CTRatio; 		 //L0CT变比
extern LONG* 	AS_pnJXCTRatio; 		 //JXCT变比
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     9    //辅助定值中变比个数

#define AS_COUNT     81    //辅助定值个数
#define AS_BUF_SIZE     324    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 608)

//系数----------------------------------
extern LONG* 	CF_pnUp; 		 //Up
extern LONG* 	CF_pnU0h; 		 //U0h
extern LONG* 	CF_pnU0l; 		 //U0l
extern LONG* 	CF_pnIp; 		 //Ip
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnIjx; 		 //Ijx
extern LONG* 	CF_pnId; 		 //Id
extern LONG* 	CF_pnUAM; 		 //UAM
extern LONG* 	CF_pnUBM; 		 //UBM
extern LONG* 	CF_pnUCM; 		 //UCM
extern LONG* 	CF_pnU0hM; 		 //U0hM
extern LONG* 	CF_pnUaM; 		 //UaM
extern LONG* 	CF_pnUbM; 		 //UbM
extern LONG* 	CF_pnUcM; 		 //UcM
extern LONG* 	CF_pnU0lM; 		 //U0lM
extern LONG* 	CF_pnIAM; 		 //IAM
extern LONG* 	CF_pnIBM; 		 //IBM
extern LONG* 	CF_pnICM; 		 //ICM
extern LONG* 	CF_pnIaM; 		 //IaM
extern LONG* 	CF_pnIbM; 		 //IbM
extern LONG* 	CF_pnIcM; 		 //IcM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnIjxM; 		 //IjxM
extern LONG* 	CF_pnUAD; 		 //UAD
extern LONG* 	CF_pnUBD; 		 //UBD
extern LONG* 	CF_pnUCD; 		 //UCD
extern LONG* 	CF_pnU0hD; 		 //U0hD
extern LONG* 	CF_pnUaD; 		 //UaD
extern LONG* 	CF_pnUbD; 		 //UbD
extern LONG* 	CF_pnUcD; 		 //UcD
extern LONG* 	CF_pnU0lD; 		 //U0lD
extern LONG* 	CF_pnIAD; 		 //IAD
extern LONG* 	CF_pnIBD; 		 //IBD
extern LONG* 	CF_pnICD; 		 //ICD
extern LONG* 	CF_pnIaD; 		 //IaD
extern LONG* 	CF_pnIbD; 		 //IbD
extern LONG* 	CF_pnIcD; 		 //IcD
extern LONG* 	CF_pnI0D; 		 //I0D
extern LONG* 	CF_pnIjxD; 		 //IjxD
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnPA; 		 //PA
extern LONG* 	CF_pnQA; 		 //QA
extern LONG* 	CF_pnPB; 		 //PB
extern LONG* 	CF_pnQB; 		 //QB
extern LONG* 	CF_pnPC; 		 //PC
extern LONG* 	CF_pnQC; 		 //QC
extern LONG* 	CF_pnAA; 		 //AA
extern LONG* 	CF_pnBA; 		 //BA
extern LONG* 	CF_pnCA; 		 //CA

#define CF_COUNT     52    //系数个数
#define CF_BUF_SIZE     208    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 932)

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
#define R_LOGICERR_R               21    //逻辑方程故障恢复
#define R_POWERERR_R               22    //5V电源故障恢复
#define R_TRPICK                   23    //保护启动
#define R_RTR_GAS                  24    //重瓦斯
#define R_RTR_YLSF                 25    //压力释放
#define R_RTR_23                   26    //超温跳闸
#define R_R74_GAS                  27    //轻瓦斯
#define R_G74_OIL                  28    //油位异常
#define R_G74_23                   29    //温度告警
#define R_PICKUP                   30    //保护启动
#define R_87UA_PI                  31    //差动速断启动(A相)
#define R_87UB_PI                  32    //差动速断启动(B相)
#define R_87UC_PI                  33    //差动速断启动(C相)
#define R_87U                      34    //差动速断保护动作
#define R_87UA_RES                 35    //差动速断返回(A相)
#define R_87UB_RES                 36    //差动速断返回(B相)
#define R_87UC_RES                 37    //差动速断返回(C相)
#define R_87RA_PI                  38    //比率差动启动(A相)
#define R_87RB_PI                  39    //比率差动启动(B相)
#define R_87RC_PI                  40    //比率差动启动(C相)
#define R_87R                      41    //比率差动保护动作
#define R_87RA_RES                 42    //比率差动返回(A相)
#define R_87RB_RES                 43    //比率差动返回(B相)
#define R_87RC_RES                 44    //比率差动返回(C相)
#define R_50I_H1_PI                45    //高压侧过流Ⅰ段启动
#define R_50I_H1                   46    //高压侧过流Ⅰ段动作
#define R_50I_H1_RES               47    //高压侧过流Ⅰ段返回
#define R_50I_H2_PI                48    //高压侧过流Ⅱ段启动
#define R_50I_H2                   49    //高压侧过流Ⅱ段动作
#define R_50I_H2_RES               50    //高压侧过流Ⅱ段返回
#define R_50I_H3_PI                51    //高压侧过流Ⅲ段启动
#define R_50I_H3                   52    //高压侧过流Ⅲ段动作
#define R_50I_H3_RES               53    //高压侧过流Ⅲ段返回
#define R_50I_L1_PI                54    //低压侧过流Ⅰ段启动
#define R_50I_L1                   55    //低压侧过流Ⅰ段动作
#define R_50I_L1_RES               56    //低压侧过流Ⅰ段返回
#define R_50I_L2_PI                57    //低压侧过流Ⅱ段启动
#define R_50I_L2                   58    //低压侧过流Ⅱ段动作
#define R_50I_L2_RES               59    //低压侧过流Ⅱ段返回
#define R_50I_SH_PI                60    //母线充电保护启动
#define R_50I_SH                   61    //母线充电保护动作
#define R_50I_SH_RES               62    //母线充电保护返回
#define R_49IAR                    63    //反时限过负荷告警
#define R_49I                      64    //反时限过负荷动作
#define R_49I_RES                  65    //反时限过负荷返回
#define R_50I_N1_PI                66    //零序过流Ⅰ段启动
#define R_50I_N1                   67    //零序过流Ⅰ段动作
#define R_50I_N1_RES               68    //零序过流Ⅰ段返回
#define R_50I_N2_PI                69    //零序过流Ⅱ段启动
#define R_50I_N2                   70    //零序过流Ⅱ段动作
#define R_50I_N2_RES               71    //零序过流Ⅱ段返回
#define R_59U_H_PI                 72    //高压侧零序过压启动
#define R_59U_H                    73    //高压侧零序过压动作
#define R_59U_H_RES                74    //高压侧零序过压返回
#define R_50OL_1_PI                75    //过负荷Ⅰ段启动
#define R_50OL_1                   76    //过负荷Ⅰ段动作
#define R_50OL_1_RES               77    //过负荷Ⅰ段返回
#define R_50OL_2_PI                78    //过负荷Ⅱ段启动
#define R_50OL_2                   79    //过负荷Ⅱ段动作
#define R_50OL_2_RES               80    //过负荷Ⅱ段返回
#define R_50OL_3_PI                81    //过负荷Ⅲ段启动
#define R_50OL_3                   82    //过负荷Ⅲ段动作
#define R_50OL_3_RES               83    //过负荷Ⅲ段返回
#define R_50I_JX_PI                84    //间隙过流保护启动
#define R_50I_JX                   85    //间隙过流保护动作
#define R_50I_JX_RES               86    //间隙过流保护返回
#define R_27U                      87    //失压保护
#define R_59U_L                    88    //低压侧零序过压告警
#define R_87AR                     89    //差流越限
#define R_HPTAR                    90    //高压侧PT断线
#define R_LPTAR                    91    //低压侧PT断线
#define R_52BF                     92    //断路器跳闸失败
#define R_COMTEST                  93    //通道测试
#define R_HPTAR_RES                94    //高压侧PT断线恢复
#define R_LPTAR_RES                95    //低压侧PT断线恢复
#define R_87AR_RES                 96    //差流越限恢复
#define R_RELDATAERR               97    //保护数据异常
#define R_RELDATAERR_R             98    //保护数据异常恢复
#define R_50OL_A                   99    //过负荷告警
#define R_50OL_A_PI               100    //过负荷告警启动
#define R_50OL_A_RES              101    //过负荷告警返回
#define MAX_REPORT_INDEX          101    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     19    //传动个数

#endif