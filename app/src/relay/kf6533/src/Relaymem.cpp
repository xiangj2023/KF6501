/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			Relaymem.c
* 
*   软件模块：保护接口文件
*
* 
*	描述	保护用数据定义，存储分配， 下半部分由工具软件自动生成
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
* 
*************************************************************************/



#include "RelayDef.h"
#include "Logicdef.h"
#include "Relay_defs.h"

#ifdef __PL_WIN_SIMULATION__
    BYTE _RAMDataBuf[32*1024];             //在window模拟平台中模拟
#endif

//对于不关联 延时定值 的继电器， 指向值为0的常数， 关联和未关联使用相同处理方式
const LONG NoTimeAddr[1] = {0};

//以下代码由工具软件生成




//当前定值区号定义-----------------------
WORD* 	G_pwCurAreaNo 	= 	(WORD*)(RELAY_RAM_ADDR_BASE + 0); 	//当前定值区号
//定值取值信息
const TSetValueInf SetValueInf[]=
{
	{-1, NULL},
};
const TSetValueInfMap SetValueInfMap[]=
{
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
};
//辅助定值取值信息
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //辅助定值额定电流
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
};

//保护测量值定义-----------------------
LONG* 	RM_pnUHA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	//UA UA
LONG* 	RM_pnUHB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	//UB UB
LONG* 	RM_pnUHC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	//UC UC
LONG* 	RM_pnUAB 	= 	(LONG*)(RM_BUF_ADDR + 12); 	//UAB UAB
LONG* 	RM_pnUBC 	= 	(LONG*)(RM_BUF_ADDR + 16); 	//UBC UBC
LONG* 	RM_pnUCA 	= 	(LONG*)(RM_BUF_ADDR + 20); 	//UCA UCA
LONG* 	RM_pnU0H 	= 	(LONG*)(RM_BUF_ADDR + 24); 	//U0H U0H
LONG* 	RM_pnULa 	= 	(LONG*)(RM_BUF_ADDR + 28); 	//Ua Ua
LONG* 	RM_pnULb 	= 	(LONG*)(RM_BUF_ADDR + 32); 	//Ub Ub
LONG* 	RM_pnULc 	= 	(LONG*)(RM_BUF_ADDR + 36); 	//Uc Uc
LONG* 	RM_pnULab 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// Uab
LONG* 	RM_pnULbc 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Ubc
LONG* 	RM_pnULca 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Uca
LONG* 	RM_pnU0L 	= 	(LONG*)(RM_BUF_ADDR + 52); 	//U0L U0L
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 56); 	//IA IA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 60); 	//IB IB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 64); 	//IC IC
LONG* 	RM_pnILa 	= 	(LONG*)(RM_BUF_ADDR + 68); 	//Ia Ia
LONG* 	RM_pnILb 	= 	(LONG*)(RM_BUF_ADDR + 72); 	//Ib Ib
LONG* 	RM_pnILc 	= 	(LONG*)(RM_BUF_ADDR + 76); 	//Ic Ic
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 80); 	//I0 I0
LONG* 	RM_pnIjx 	= 	(LONG*)(RM_BUF_ADDR + 84); 	//Ijx Ijx
LONG* 	RM_pnU2H 	= 	(LONG*)(RM_BUF_ADDR + 88); 	//U2H U2H
LONG* 	RM_pnU2L 	= 	(LONG*)(RM_BUF_ADDR + 92); 	//U2L U2L
LONG* 	RM_pnIDA 	= 	(LONG*)(RM_BUF_ADDR + 96); 	//IDA IDA
LONG* 	RM_pnIDB 	= 	(LONG*)(RM_BUF_ADDR + 100); 	//IDB IDB
LONG* 	RM_pnIDC 	= 	(LONG*)(RM_BUF_ADDR + 104); 	//IDC IDC
LONG* 	RM_pnIRA 	= 	(LONG*)(RM_BUF_ADDR + 108); 	//IRA IRA
LONG* 	RM_pnIRB 	= 	(LONG*)(RM_BUF_ADDR + 112); 	//IRB IRB
LONG* 	RM_pnIRC 	= 	(LONG*)(RM_BUF_ADDR + 116); 	//IRC IRC
LONG* 	RM_pnIDA2 	= 	(LONG*)(RM_BUF_ADDR + 120); 	//IDA2 IDA2
LONG* 	RM_pnIDA3 	= 	(LONG*)(RM_BUF_ADDR + 124); 	//IDA3 IDA3
LONG* 	RM_pnIDA5 	= 	(LONG*)(RM_BUF_ADDR + 128); 	//IDA5 IDA5
LONG* 	RM_pnIDB2 	= 	(LONG*)(RM_BUF_ADDR + 132); 	//IDB2 IDB2
LONG* 	RM_pnIDB3 	= 	(LONG*)(RM_BUF_ADDR + 136); 	//IDB3 IDB3
LONG* 	RM_pnIDB5 	= 	(LONG*)(RM_BUF_ADDR + 140); 	//IDB5 IDB5
LONG* 	RM_pnIDC2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	//IDC2 IDC2
LONG* 	RM_pnIDC3 	= 	(LONG*)(RM_BUF_ADDR + 148); 	//IDC3 IDC3
LONG* 	RM_pnIDC5 	= 	(LONG*)(RM_BUF_ADDR + 152); 	//IDC5 IDC5
LONG* 	RM_pnIRA2 	= 	(LONG*)(RM_BUF_ADDR + 156); 	//IRA2 IRA2
LONG* 	RM_pnIRB2 	= 	(LONG*)(RM_BUF_ADDR + 160); 	//IRB2 IRB2
LONG* 	RM_pnIRC2 	= 	(LONG*)(RM_BUF_ADDR + 164); 	//IRC2 IRC2
LONG* 	RM_pnAUHA 	= 	(LONG*)(RM_BUF_ADDR + 168); 	//AUA ΦUA
LONG* 	RM_pnAUHB 	= 	(LONG*)(RM_BUF_ADDR + 172); 	//AUB ΦUB
LONG* 	RM_pnAUHC 	= 	(LONG*)(RM_BUF_ADDR + 176); 	//AUC ΦUC
LONG* 	RM_pnAULa 	= 	(LONG*)(RM_BUF_ADDR + 180); 	//AUa ΦUa
LONG* 	RM_pnAULb 	= 	(LONG*)(RM_BUF_ADDR + 184); 	//AUb ΦUb
LONG* 	RM_pnAULc 	= 	(LONG*)(RM_BUF_ADDR + 188); 	//AUc ΦUc
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 192); 	//AIA ΦIA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 196); 	//AIB ΦIB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 200); 	//AIC ΦIC
LONG* 	RM_pnAILa 	= 	(LONG*)(RM_BUF_ADDR + 204); 	//AIa ΦIa
LONG* 	RM_pnAILb 	= 	(LONG*)(RM_BUF_ADDR + 208); 	//AIb ΦIb
LONG* 	RM_pnAILc 	= 	(LONG*)(RM_BUF_ADDR + 212); 	//AIc ΦIc
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 216); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 220); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 224); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 228); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnU0h 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0h U0h
LONG* 	M_pnUla 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Ua Ua
LONG* 	M_pnUlb 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ub Ub
LONG* 	M_pnUlc 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Uc Uc
LONG* 	M_pnU0l 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//U0l U0l
LONG* 	M_pnIHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IA IA
LONG* 	M_pnIHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IB IB
LONG* 	M_pnIHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//IC IC
LONG* 	M_pnIla 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Ia Ia
LONG* 	M_pnIlb 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ib Ib
LONG* 	M_pnIlc 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//Ic Ic
LONG* 	M_pnI0 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I0 I0
LONG* 	M_pnIjx 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//Ijx Ijx
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//COS COS


//软压板----------------------------------
BYTE* SW_pby87U 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//差动速断保护
BYTE* SW_pby87R 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//比率差动保护
BYTE* SW_pby50I_H 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//高压侧过流保护
BYTE* SW_pby50I_L 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//低压侧过流保护
BYTE* SW_pby50I_SH 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//母线充电保护
BYTE* SW_pby50N 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//零序过流保护
BYTE* SW_pby59N 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//零序过压保护
BYTE* SW_pby50OL 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//过负荷保护
BYTE* SW_pby50I_JX 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//间隙过流保护
BYTE* SW_pby49I 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//反时限过负荷
BYTE* SW_pby27U 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//失压保护
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 13); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby87U 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//差流速断保护
BYTE* CFG_pby87R 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//比率差动保护
BYTE* CFG_pby68IDH2MAXLK 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//二次谐波最大相闭锁
BYTE* CFG_pby68IDH2MULLK 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//二次谐波综合相闭锁
BYTE* CFG_pby68IDH2SPLLK 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//二次谐波分相闭锁
BYTE* CFG_pby68IRH2LK 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//制动电流二次谐波闭锁
BYTE* CFG_pby68IDMULLK 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//综合谐波闭锁
BYTE* CFG_pbyHCT1CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//高压侧CT1点校正
BYTE* CFG_pbyHCT11CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//高压侧CT11点校正
BYTE* CFG_pbyLCT1CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//低压侧CT1点校正
BYTE* CFG_pbyLCT11CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//低压侧CT11点校正
BYTE* CFG_pbyHAB 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//高压侧两相
BYTE* CFG_pby50I_H1 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//高压侧过流Ⅰ段
BYTE* CFG_pby50I_H1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//高压侧过流Ⅰ段功率方向
BYTE* CFG_pby50I_H1_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//高压侧过流Ⅰ段复压闭锁
BYTE* CFG_pby50I_H2 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//高压侧过流Ⅱ段
BYTE* CFG_pby50I_H2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//高压侧过流Ⅱ段功率方向
BYTE* CFG_pby50I_H2_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//高压侧过流Ⅱ段复压闭锁
BYTE* CFG_pby50I_H3 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//高压侧过流Ⅲ段
BYTE* CFG_pby50I_H3_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//高压侧过流Ⅲ段功率方向
BYTE* CFG_pby50I_H3_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//高压侧过流Ⅲ段复压闭锁
BYTE* CFG_pby50I_L1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//低压侧过流Ⅰ段
BYTE* CFG_pby50I_L1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//低压侧过流Ⅰ段功率方向
BYTE* CFG_pby50I_L1_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//低压侧过流Ⅰ段复压闭锁
BYTE* CFG_pby50I_L2 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//低压侧过流Ⅱ段
BYTE* CFG_pby50I_L2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//低压侧过流Ⅱ段功率方向
BYTE* CFG_pby50I_L2_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//低压侧过流Ⅱ段复压闭锁
BYTE* CFG_pby50I_SH 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//母线充电保护
BYTE* CFG_pby49I 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//反时限过负荷保护
BYTE* CFG_pby50I_N1 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//零序过流Ⅰ段
BYTE* CFG_pby50I_N2 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//零序过流Ⅱ段
BYTE* CFG_pby59U_NH 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//高压侧零序过压保护
BYTE* CFG_pby59U_NL 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//低压侧零序过压告警
BYTE* CFG_pby50OL_1 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//过负荷Ⅰ段
BYTE* CFG_pby50OL_2 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//过负荷Ⅱ段
BYTE* CFG_pby50OL_3 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//过负荷Ⅲ段
BYTE* CFG_pby50OL_A 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//过负荷告警
BYTE* CFG_pby50I_JX 	= 	(BYTE*)(CFG_BUF_ADDR + 37); 	//间隙过流保护
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 38); 	//失压保护
BYTE* CFG_pby27U_50I 	= 	(BYTE*)(CFG_BUF_ADDR + 39); 	//失压保护电流闭锁
BYTE* CFG_pbyPTCK 	= 	(BYTE*)(CFG_BUF_ADDR + 40); 	//PT断线检测
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 41); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 42); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 43); 	//备用配置2

//定值----------------------------------
LONG* RS_pnFUNCTION1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字1 FUNCTION1
LONG* RS_pnFUNCTION2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//配置字2 FUNCTION2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//突变量启动电流 PIC
LONG* RS_pn87UC 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//差动速断电流 87UC
LONG* RS_pn87RC 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//差动门槛电流 87RC
LONG* RS_pnIRS1 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//比率制动拐点1 IRS1
LONG* RS_pnIRS2 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//比率制动拐点2 IRS2
LONG* RS_pnSLP1 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//比率制动系数1 SLP1
LONG* RS_pnSLP2 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//比率制动系数2 SLP2
LONG* RS_pnkrh2 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//二次谐波制动系数 krh2
LONG* RS_pnkrh 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//综合谐波制动系数 krh
LONG* RS_pnkb 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//平衡系数 kb
LONG* RS_pn50IH1C 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//高压侧过流Ⅰ段电流 50IH1C
LONG* RS_pn50IH1D 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//高压侧过流Ⅰ段时间 50IH1D
LONG* RS_pn50IH2C 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//高压侧过流Ⅱ段电流 50IH2C
LONG* RS_pn50IH2D 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//高压侧过流Ⅱ段时间 50IH2D
LONG* RS_pn50IH3C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//高压侧过流Ⅲ段电流 50IH3C
LONG* RS_pn50IH3D 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//高压侧过流Ⅲ段时间 50IH3D
LONG* RS_pn50IL1C 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//低压侧过流Ⅰ段电流 50IL1C
LONG* RS_pn50IL1D 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//低压侧过流Ⅰ段时间 50IL1D
LONG* RS_pn50IL2C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//低压侧过流Ⅱ段电流 50IL2C
LONG* RS_pn50IL2D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//低压侧过流Ⅱ段时间 50IL2D
LONG* RS_pn47U_27U1 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//复压闭锁低电压 47U_27U1
LONG* RS_pn47U_59U2 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//复压闭锁负序电压 47U_59U2
LONG* RS_pnA_50I 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//过流保护灵敏角 A_50I
LONG* RS_pn50ISHC 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//母充保护电流 50ISHC
LONG* RS_pn50ISHD 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//母充保护时间 50ISHD
LONG* RS_pn50ISHT 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//母充保护投入时间 50ISHT
LONG* RS_pn50IN1C 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//零序过流Ⅰ段电流 50IN1C
LONG* RS_pn50IN1D 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//零序过流Ⅰ段时间 50IN1D
LONG* RS_pn50IN2C 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//零序过流Ⅱ段电流 50IN2C
LONG* RS_pn50IN2D 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//零序过流Ⅱ段时间 50IN2D
LONG* RS_pn59UNHV 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//高压侧零序过压电压 59UNHV
LONG* RS_pn59UNHD 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//高压侧零序过压时间 59UNHD
LONG* RS_pn59UNLV 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//低压侧零压告警电压 59UNLV
LONG* RS_pn59UNLD 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//低压侧零压告警时间 59UNLD
LONG* RS_pn50OL1C 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//过负荷Ⅰ段电流 50OL1C
LONG* RS_pn50OL1D 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//过负荷Ⅰ段时间 50OL1D
LONG* RS_pn50OL2C 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//过负荷Ⅱ段电流 50OL2C
LONG* RS_pn50OL2D 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//过负荷Ⅱ段时间 50OL2D
LONG* RS_pn50OL3C 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//过负荷Ⅲ段电流 50OL3C
LONG* RS_pn50OL3D 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//过负荷Ⅲ段时间 50OL3D
LONG* RS_pn50OLAC 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//过负荷告警电流 50OLAC
LONG* RS_pn50OLAD 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//过负荷告警时间 50OLAD
LONG* RS_pn50I_JXC 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//间隙过流电流 50I_JXC
LONG* RS_pn50I_JXD 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//间隙过流时间 50I_JXD
LONG* RS_pnRAC 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//变压器二次额定电流 RAC
LONG* RS_pn49K 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//热过负荷保护因子 49K
LONG* RS_pn49T 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//热过负荷时间常数 49T
LONG* RS_pn49AR 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//热过负荷告警比率 49AR
LONG* RS_pn49KT 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//设备停止扩充系数 49KT
LONG* RS_pn27V 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//失压保护电压 27V
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//失压保护时间 27D
LONG* RS_pn27U_50I 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//失压保护电流闭锁定值 27U_50I
LONG* RS_pn74VTC 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//PT断线检测电流 74VTC
LONG* RS_pn74VTV 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//PT断线检测电压 74VTV
LONG* RS_pn74VTD 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//PT断线检测时间 74VTD
LONG* RS_pnK87AR 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//差流越限报警比率 K87AR
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//失灵保护时间 52BFD

//辅助定值----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//额定电流
LONG* AS_pnT1PI 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//T1启动延时
LONG* AS_pnT1RE 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//T1返回延时
LONG* AS_pnT2PI 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//T2启动延时
LONG* AS_pnT2RE 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//T2返回延时
LONG* AS_pnT3PI 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//T3启动延时
LONG* AS_pnT3RE 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//T3返回延时
LONG* AS_pnT4PI 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//T4启动延时
LONG* AS_pnT4RE 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//T4返回延时
LONG* AS_pnT5PI 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//T5启动延时
LONG* AS_pnT5RE 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//T5返回延时
LONG* AS_pnT6PI 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//T6启动延时
LONG* AS_pnT6RE 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//T6返回延时
LONG* AS_pnT7PI 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//T7启动延时
LONG* AS_pnT7RE 	= 	(LONG*)(AS_BUF_ADDR + 56); 	//T7返回延时
LONG* AS_pnT8PI 	= 	(LONG*)(AS_BUF_ADDR + 60); 	//T8启动延时
LONG* AS_pnT8RE 	= 	(LONG*)(AS_BUF_ADDR + 64); 	//T8返回延时
LONG* AS_pnT9PI 	= 	(LONG*)(AS_BUF_ADDR + 68); 	//T9启动延时
LONG* AS_pnT9RE 	= 	(LONG*)(AS_BUF_ADDR + 72); 	//T9返回延时
LONG* AS_pnT10PI 	= 	(LONG*)(AS_BUF_ADDR + 76); 	//T10启动延时
LONG* AS_pnT10RE 	= 	(LONG*)(AS_BUF_ADDR + 80); 	//T10返回延时
LONG* AS_pnT11PI 	= 	(LONG*)(AS_BUF_ADDR + 84); 	//T11启动延时
LONG* AS_pnT11RE 	= 	(LONG*)(AS_BUF_ADDR + 88); 	//T11返回延时
LONG* AS_pnT12PI 	= 	(LONG*)(AS_BUF_ADDR + 92); 	//T12启动延时
LONG* AS_pnT12RE 	= 	(LONG*)(AS_BUF_ADDR + 96); 	//T12返回延时
LONG* AS_pnT13PI 	= 	(LONG*)(AS_BUF_ADDR + 100); 	//T13启动延时
LONG* AS_pnT13RE 	= 	(LONG*)(AS_BUF_ADDR + 104); 	//T13返回延时
LONG* AS_pnT14PI 	= 	(LONG*)(AS_BUF_ADDR + 108); 	//T14启动延时
LONG* AS_pnT14RE 	= 	(LONG*)(AS_BUF_ADDR + 112); 	//T14返回延时
LONG* AS_pnT15PI 	= 	(LONG*)(AS_BUF_ADDR + 116); 	//T15启动延时
LONG* AS_pnT15RE 	= 	(LONG*)(AS_BUF_ADDR + 120); 	//T15返回延时
LONG* AS_pnT16PI 	= 	(LONG*)(AS_BUF_ADDR + 124); 	//T16启动延时
LONG* AS_pnT16RE 	= 	(LONG*)(AS_BUF_ADDR + 128); 	//T16返回延时
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//断路器动作延时
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//电动隔开动作延时
LONG* AS_pn50IAC 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//IA有流定值
LONG* AS_pn37IAC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IA无流定值
LONG* AS_pn50IBC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IB有流定值
LONG* AS_pn37IBC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//IB无流定值
LONG* AS_pn50ICC 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IC有流定值
LONG* AS_pn37ICC 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//IC无流定值
LONG* AS_pn50IaC 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//Ia有流定值
LONG* AS_pn37IaC 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//Ia无流定值
LONG* AS_pn50IbC 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//Ib有流定值
LONG* AS_pn37IbC 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//Ib无流定值
LONG* AS_pn50IcC 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//Ic有流定值
LONG* AS_pn37IcC 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//Ic无流定值
LONG* AS_pn50I0C 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//I0有流定值
LONG* AS_pn37I0C 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//I0无流定值
LONG* AS_pn50IjxC 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//Ijx有流定值
LONG* AS_pn37IjxC 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//Ijx无流定值
LONG* AS_pn59UAV 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//UA有压定值
LONG* AS_pn27UAV 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//UA无压定值
LONG* AS_pn59UBV 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//UB有压定值
LONG* AS_pn27UBV 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//UB无压定值
LONG* AS_pn59UCV 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//UC有压定值
LONG* AS_pn27UCV 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//UC无压定值
LONG* AS_pn59U0hV 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//U0h有压定值
LONG* AS_pn27U0hV 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//U0h无压定值
LONG* AS_pn59UaV 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//Ua有压定值
LONG* AS_pn27UaV 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//Ua无压定值
LONG* AS_pn59UbV 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//Ub有压定值
LONG* AS_pn27UbV 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//Ub无压定值
LONG* AS_pn59UcV 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//Uc有压定值
LONG* AS_pn27UcV 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//Uc无压定值
LONG* AS_pn59U0lV 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//U0l有压定值
LONG* AS_pn27U0lV 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//U0l无压定值
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//TR2保持时间
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//TR3保持时间
LONG* AS_pnTR4D 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//TR4保持时间
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 284); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnHPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 288); 	//HPT变比
LONG* AS_pnH0PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 292); 	//H0PT变比
LONG* AS_pnLPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 296); 	//LPT变比
LONG* AS_pnL0PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 300); 	//L0PT变比
LONG* AS_pnHCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 304); 	//HCT变比
LONG* AS_pnLCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 308); 	//LCT变比
LONG* AS_pnL0CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 312); 	//L0CT变比
LONG* AS_pnJXCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 316); 	//JXCT变比
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 320); 	//rt1

//系数----------------------------------
LONG* CF_pnUp 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//Up
LONG* CF_pnU0h 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U0h
LONG* CF_pnU0l 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U0l
LONG* CF_pnIp 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//Ip
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//I0
LONG* CF_pnIjx 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//Ijx
LONG* CF_pnId 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//Id
LONG* CF_pnUAM 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UAM
LONG* CF_pnUBM 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//UBM
LONG* CF_pnUCM 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//UCM
LONG* CF_pnU0hM 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//U0hM
LONG* CF_pnUaM 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//UaM
LONG* CF_pnUbM 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//UbM
LONG* CF_pnUcM 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//UcM
LONG* CF_pnU0lM 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//U0lM
LONG* CF_pnIAM 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//IAM
LONG* CF_pnIBM 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IBM
LONG* CF_pnICM 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//ICM
LONG* CF_pnIaM 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//IaM
LONG* CF_pnIbM 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//IbM
LONG* CF_pnIcM 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//IcM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//I0M
LONG* CF_pnIjxM 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//IjxM
LONG* CF_pnUAD 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//UAD
LONG* CF_pnUBD 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//UBD
LONG* CF_pnUCD 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//UCD
LONG* CF_pnU0hD 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//U0hD
LONG* CF_pnUaD 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//UaD
LONG* CF_pnUbD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//UbD
LONG* CF_pnUcD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//UcD
LONG* CF_pnU0lD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//U0lD
LONG* CF_pnIAD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IAD
LONG* CF_pnIBD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IBD
LONG* CF_pnICD 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//ICD
LONG* CF_pnIaD 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//IaD
LONG* CF_pnIbD 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//IbD
LONG* CF_pnIcD 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//IcD
LONG* CF_pnI0D 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//I0D
LONG* CF_pnIjxD 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//IjxD
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//C4
LONG* CF_pnPA 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//PA
LONG* CF_pnQA 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//QA
LONG* CF_pnPB 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//PB
LONG* CF_pnQB 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//QB
LONG* CF_pnPC 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//PC
LONG* CF_pnQC 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//QC
LONG* CF_pnAA 	= 	(LONG*)(CF_BUF_ADDR + 196); 	//AA
LONG* CF_pnBA 	= 	(LONG*)(CF_BUF_ADDR + 200); 	//BA
LONG* CF_pnCA 	= 	(LONG*)(CF_BUF_ADDR + 204); 	//CA

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 224), // IN1
    (LONG*)(RM_BUF_ADDR + 228) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 216), // BR1
    (LONG*)(RM_BUF_ADDR + 220) // BR2
};

//AD故障3----------
const  LONG* FailureInf3[] = {
    {NULL}
};

//软压板故障4----------
const  LONG* FailureInf4[] = {
    {NULL}
};

//EEPROM定值故障5----------
const  LONG* FailureInf5[] = {
    {NULL}
};

//RAM定值故障8----------
const  LONG* FailureInf8[] = {
    {NULL}
};

//程序ROM故障11----------
const  LONG* FailureInf11[] = {
    {NULL}
};

//装置异常复位13----------
const  LONG* FailureInf13[] = {
    {NULL}
};

//通信通道异常14----------
const  LONG* FailureInf14[] = {
    {NULL}
};

//NVRAM内容出错16----------
const  LONG* FailureInf16[] = {
    {NULL}
};

//逻辑方程故障17----------
const  LONG* FailureInf17[] = {
    {NULL}
};

//保护自检错误18----------
const  LONG* FailureInf18[] = {
    {NULL}
};

//5V电源故障20----------
const  LONG* FailureInf20[] = {
    {NULL}
};

//EEPROM写入故障21----------
const  LONG* FailureInf21[] = {
    {NULL}
};

//AD故障恢复50----------
const  LONG* FailureInf50[] = {
    {NULL}
};

//软压板故障恢复51----------
const  LONG* FailureInf51[] = {
    {NULL}
};

//EEPROM定值故障恢复52----------
const  LONG* FailureInf52[] = {
    {NULL}
};

//RAM定值故障恢复55----------
const  LONG* FailureInf55[] = {
    {NULL}
};

//程序ROM故障恢复58----------
const  LONG* FailureInf58[] = {
    {NULL}
};

//通信通道异常恢复60----------
const  LONG* FailureInf60[] = {
    {NULL}
};

//NVRAM内容出错恢复62----------
const  LONG* FailureInf62[] = {
    {NULL}
};

//逻辑方程故障恢复63----------
const  LONG* FailureInf63[] = {
    {NULL}
};

//5V电源故障恢复64----------
const  LONG* FailureInf64[] = {
    {NULL}
};

//保护启动500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//重瓦斯501----------
const  LONG* FailureInf501[] = {
    {NULL}
};

//压力释放502----------
const  LONG* FailureInf502[] = {
    {NULL}
};

//超温跳闸503----------
const  LONG* FailureInf503[] = {
    {NULL}
};

//轻瓦斯504----------
const  LONG* FailureInf504[] = {
    {NULL}
};

//油位异常505----------
const  LONG* FailureInf505[] = {
    {NULL}
};

//温度告警506----------
const  LONG* FailureInf506[] = {
    {NULL}
};

//保护启动2800----------
const  LONG* FailureInf2800[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ΦIA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ΦIB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ΦIC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ΦIa
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ΦIb
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ΦIc
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 156), //IRA2 IRA2
    (LONG*)(RM_BUF_ADDR + 160), //IRB2 IRB2
    (LONG*)(RM_BUF_ADDR + 164), //IRC2 IRC2
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//差动速断启动(A相)2801----------
const  LONG* FailureInf2801[] = {
    {NULL}
};

//差动速断启动(B相)2802----------
const  LONG* FailureInf2802[] = {
    {NULL}
};

//差动速断启动(C相)2803----------
const  LONG* FailureInf2803[] = {
    {NULL}
};

//差动速断保护动作2804----------
const  LONG* FailureInf2804[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ΦIA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ΦIB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ΦIC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ΦIa
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ΦIb
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ΦIc
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 156), //IRA2 IRA2
    (LONG*)(RM_BUF_ADDR + 160), //IRB2 IRB2
    (LONG*)(RM_BUF_ADDR + 164), //IRC2 IRC2
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8) //UC UC
};

//差动速断返回(A相)2805----------
const  LONG* FailureInf2805[] = {
    {NULL}
};

//差动速断返回(B相)2806----------
const  LONG* FailureInf2806[] = {
    {NULL}
};

//差动速断返回(C相)2807----------
const  LONG* FailureInf2807[] = {
    {NULL}
};

//比率差动启动(A相)2808----------
const  LONG* FailureInf2808[] = {
    {NULL}
};

//比率差动启动(B相)2809----------
const  LONG* FailureInf2809[] = {
    {NULL}
};

//比率差动启动(C相)2810----------
const  LONG* FailureInf2810[] = {
    {NULL}
};

//比率差动保护动作2811----------
const  LONG* FailureInf2811[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ΦIA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ΦIB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ΦIC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ΦIa
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ΦIb
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ΦIc
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 156), //IRA2 IRA2
    (LONG*)(RM_BUF_ADDR + 160), //IRB2 IRB2
    (LONG*)(RM_BUF_ADDR + 164), //IRC2 IRC2
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8) //UC UC
};

//比率差动返回(A相)2812----------
const  LONG* FailureInf2812[] = {
    {NULL}
};

//比率差动返回(B相)2813----------
const  LONG* FailureInf2813[] = {
    {NULL}
};

//比率差动返回(C相)2814----------
const  LONG* FailureInf2814[] = {
    {NULL}
};

//高压侧过流Ⅰ段启动2815----------
const  LONG* FailureInf2815[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//高压侧过流Ⅰ段动作2816----------
const  LONG* FailureInf2816[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//高压侧过流Ⅰ段返回2817----------
const  LONG* FailureInf2817[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//高压侧过流Ⅱ段启动2818----------
const  LONG* FailureInf2818[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//高压侧过流Ⅱ段动作2819----------
const  LONG* FailureInf2819[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//高压侧过流Ⅱ段返回2820----------
const  LONG* FailureInf2820[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//高压侧过流Ⅲ段启动2821----------
const  LONG* FailureInf2821[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//高压侧过流Ⅲ段动作2822----------
const  LONG* FailureInf2822[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//高压侧过流Ⅲ段返回2823----------
const  LONG* FailureInf2823[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//低压侧过流Ⅰ段启动2824----------
const  LONG* FailureInf2824[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//低压侧过流Ⅰ段动作2825----------
const  LONG* FailureInf2825[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//低压侧过流Ⅰ段返回2826----------
const  LONG* FailureInf2826[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//低压侧过流Ⅱ段启动2827----------
const  LONG* FailureInf2827[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//低压侧过流Ⅱ段动作2828----------
const  LONG* FailureInf2828[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//低压侧过流Ⅱ段返回2829----------
const  LONG* FailureInf2829[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//母线充电保护启动2830----------
const  LONG* FailureInf2830[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76) //Ic Ic
};

//母线充电保护动作2831----------
const  LONG* FailureInf2831[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//母线充电保护返回2832----------
const  LONG* FailureInf2832[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76) //Ic Ic
};

//反时限过负荷告警2833----------
const  LONG* FailureInf2833[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//反时限过负荷动作2834----------
const  LONG* FailureInf2834[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//反时限过负荷返回2835----------
const  LONG* FailureInf2835[] = {
    {NULL}
};

//零序过流Ⅰ段启动2836----------
const  LONG* FailureInf2836[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//零序过流Ⅰ段动作2837----------
const  LONG* FailureInf2837[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//零序过流Ⅰ段返回2838----------
const  LONG* FailureInf2838[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//零序过流Ⅱ段启动2839----------
const  LONG* FailureInf2839[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//零序过流Ⅱ段动作2840----------
const  LONG* FailureInf2840[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//零序过流Ⅱ段返回2841----------
const  LONG* FailureInf2841[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//高压侧零序过压启动2842----------
const  LONG* FailureInf2842[] = {
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//高压侧零序过压动作2843----------
const  LONG* FailureInf2843[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//高压侧零序过压返回2844----------
const  LONG* FailureInf2844[] = {
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//过负荷Ⅰ段启动2845----------
const  LONG* FailureInf2845[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//过负荷Ⅰ段动作2846----------
const  LONG* FailureInf2846[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//过负荷Ⅰ段返回2847----------
const  LONG* FailureInf2847[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//过负荷Ⅱ段启动2848----------
const  LONG* FailureInf2848[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//过负荷Ⅱ段动作2849----------
const  LONG* FailureInf2849[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//过负荷Ⅱ段返回2850----------
const  LONG* FailureInf2850[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//过负荷Ⅲ段启动2851----------
const  LONG* FailureInf2851[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//过负荷Ⅲ段动作2852----------
const  LONG* FailureInf2852[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//过负荷Ⅲ段返回2853----------
const  LONG* FailureInf2853[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//间隙过流保护启动2854----------
const  LONG* FailureInf2854[] = {
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//间隙过流保护动作2855----------
const  LONG* FailureInf2855[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//间隙过流保护返回2856----------
const  LONG* FailureInf2856[] = {
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//失压保护2857----------
const  LONG* FailureInf2857[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//低压侧零序过压告警2858----------
const  LONG* FailureInf2858[] = {
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//差流越限2859----------
const  LONG* FailureInf2859[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ΦIA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ΦIB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ΦIC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ΦIa
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ΦIb
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ΦIc
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36) //Uc Uc
};

//高压侧PT断线2860----------
const  LONG* FailureInf2860[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//低压侧PT断线2861----------
const  LONG* FailureInf2861[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//断路器跳闸失败2862----------
const  LONG* FailureInf2862[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//通道测试2863----------
const  LONG* FailureInf2863[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 168), //AUA ΦUA
    (LONG*)(RM_BUF_ADDR + 172), //AUB ΦUB
    (LONG*)(RM_BUF_ADDR + 176), //AUC ΦUC
    (LONG*)(RM_BUF_ADDR + 180), //AUa ΦUa
    (LONG*)(RM_BUF_ADDR + 184), //AUb ΦUb
    (LONG*)(RM_BUF_ADDR + 188), //AUc ΦUc
    (LONG*)(RM_BUF_ADDR + 192), //AIA ΦIA
    (LONG*)(RM_BUF_ADDR + 196), //AIB ΦIB
    (LONG*)(RM_BUF_ADDR + 200), //AIC ΦIC
    (LONG*)(RM_BUF_ADDR + 204), //AIa ΦIa
    (LONG*)(RM_BUF_ADDR + 208), //AIb ΦIb
    (LONG*)(RM_BUF_ADDR + 212) //AIc ΦIc
};

//高压侧PT断线恢复2864----------
const  LONG* FailureInf2864[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8) //UC UC
};

//低压侧PT断线恢复2865----------
const  LONG* FailureInf2865[] = {
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36) //Uc Uc
};

//差流越限恢复2866----------
const  LONG* FailureInf2866[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ΦIA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ΦIB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ΦIC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ΦIa
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ΦIb
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ΦIc
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116) //IRC IRC
};

//保护数据异常22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//保护数据异常恢复65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//过负荷告警2867----------
const  LONG* FailureInf2867[] = {
    {NULL}
};

//过负荷告警启动2868----------
const  LONG* FailureInf2868[] = {
    {NULL}
};

//过负荷告警返回2869----------
const  LONG* FailureInf2869[] = {
    {NULL}
};

//报告引用定义------------------------------------------
const struct TReportInf ReportInf[] = {
    {2, FailureInf1, 1},//开出光隔失效
    {2, FailureInf2, 2},//开出光隔击穿
    {0, FailureInf3, 3},//AD故障
    {0, FailureInf4, 4},//软压板故障
    {0, FailureInf5, 5},//EEPROM定值故障
    {0, FailureInf8, 8},//RAM定值故障
    {0, FailureInf11, 11},//程序ROM故障
    {0, FailureInf13, 13},//装置异常复位
    {0, FailureInf14, 14},//通信通道异常
    {0, FailureInf16, 16},//NVRAM内容出错
    {0, FailureInf17, 17},//逻辑方程故障
    {0, FailureInf18, 18},//保护自检错误
    {0, FailureInf20, 20},//5V电源故障
    {0, FailureInf21, 21},//EEPROM写入故障
    {0, FailureInf50, 50},//AD故障恢复
    {0, FailureInf51, 51},//软压板故障恢复
    {0, FailureInf52, 52},//EEPROM定值故障恢复
    {0, FailureInf55, 55},//RAM定值故障恢复
    {0, FailureInf58, 58},//程序ROM故障恢复
    {0, FailureInf60, 60},//通信通道异常恢复
    {0, FailureInf62, 62},//NVRAM内容出错恢复
    {0, FailureInf63, 63},//逻辑方程故障恢复
    {0, FailureInf64, 64},//5V电源故障恢复
    {0, FailureInf500, 500},//保护启动
    {0, FailureInf501, 501},//重瓦斯
    {0, FailureInf502, 502},//压力释放
    {0, FailureInf503, 503},//超温跳闸
    {0, FailureInf504, 504},//轻瓦斯
    {0, FailureInf505, 505},//油位异常
    {0, FailureInf506, 506},//温度告警
    {45, FailureInf2800, 2800},//保护启动
    {0, FailureInf2801, 2801},//差动速断启动(A相)
    {0, FailureInf2802, 2802},//差动速断启动(B相)
    {0, FailureInf2803, 2803},//差动速断启动(C相)
    {33, FailureInf2804, 2804},//差动速断保护动作
    {0, FailureInf2805, 2805},//差动速断返回(A相)
    {0, FailureInf2806, 2806},//差动速断返回(B相)
    {0, FailureInf2807, 2807},//差动速断返回(C相)
    {0, FailureInf2808, 2808},//比率差动启动(A相)
    {0, FailureInf2809, 2809},//比率差动启动(B相)
    {0, FailureInf2810, 2810},//比率差动启动(C相)
    {33, FailureInf2811, 2811},//比率差动保护动作
    {0, FailureInf2812, 2812},//比率差动返回(A相)
    {0, FailureInf2813, 2813},//比率差动返回(B相)
    {0, FailureInf2814, 2814},//比率差动返回(C相)
    {8, FailureInf2815, 2815},//高压侧过流Ⅰ段启动
    {21, FailureInf2816, 2816},//高压侧过流Ⅰ段动作
    {8, FailureInf2817, 2817},//高压侧过流Ⅰ段返回
    {8, FailureInf2818, 2818},//高压侧过流Ⅱ段启动
    {21, FailureInf2819, 2819},//高压侧过流Ⅱ段动作
    {8, FailureInf2820, 2820},//高压侧过流Ⅱ段返回
    {8, FailureInf2821, 2821},//高压侧过流Ⅲ段启动
    {21, FailureInf2822, 2822},//高压侧过流Ⅲ段动作
    {8, FailureInf2823, 2823},//高压侧过流Ⅲ段返回
    {8, FailureInf2824, 2824},//低压侧过流Ⅰ段启动
    {21, FailureInf2825, 2825},//低压侧过流Ⅰ段动作
    {8, FailureInf2826, 2826},//低压侧过流Ⅰ段返回
    {8, FailureInf2827, 2827},//低压侧过流Ⅱ段启动
    {21, FailureInf2828, 2828},//低压侧过流Ⅱ段动作
    {8, FailureInf2829, 2829},//低压侧过流Ⅱ段返回
    {3, FailureInf2830, 2830},//母线充电保护启动
    {21, FailureInf2831, 2831},//母线充电保护动作
    {3, FailureInf2832, 2832},//母线充电保护返回
    {21, FailureInf2833, 2833},//反时限过负荷告警
    {21, FailureInf2834, 2834},//反时限过负荷动作
    {0, FailureInf2835, 2835},//反时限过负荷返回
    {2, FailureInf2836, 2836},//零序过流Ⅰ段启动
    {21, FailureInf2837, 2837},//零序过流Ⅰ段动作
    {2, FailureInf2838, 2838},//零序过流Ⅰ段返回
    {2, FailureInf2839, 2839},//零序过流Ⅱ段启动
    {21, FailureInf2840, 2840},//零序过流Ⅱ段动作
    {2, FailureInf2841, 2841},//零序过流Ⅱ段返回
    {1, FailureInf2842, 2842},//高压侧零序过压启动
    {21, FailureInf2843, 2843},//高压侧零序过压动作
    {1, FailureInf2844, 2844},//高压侧零序过压返回
    {3, FailureInf2845, 2845},//过负荷Ⅰ段启动
    {21, FailureInf2846, 2846},//过负荷Ⅰ段动作
    {3, FailureInf2847, 2847},//过负荷Ⅰ段返回
    {3, FailureInf2848, 2848},//过负荷Ⅱ段启动
    {21, FailureInf2849, 2849},//过负荷Ⅱ段动作
    {3, FailureInf2850, 2850},//过负荷Ⅱ段返回
    {3, FailureInf2851, 2851},//过负荷Ⅲ段启动
    {21, FailureInf2852, 2852},//过负荷Ⅲ段动作
    {3, FailureInf2853, 2853},//过负荷Ⅲ段返回
    {2, FailureInf2854, 2854},//间隙过流保护启动
    {21, FailureInf2855, 2855},//间隙过流保护动作
    {2, FailureInf2856, 2856},//间隙过流保护返回
    {21, FailureInf2857, 2857},//失压保护
    {10, FailureInf2858, 2858},//低压侧零序过压告警
    {33, FailureInf2859, 2859},//差流越限
    {10, FailureInf2860, 2860},//高压侧PT断线
    {10, FailureInf2861, 2861},//低压侧PT断线
    {8, FailureInf2862, 2862},//断路器跳闸失败
    {36, FailureInf2863, 2863},//通道测试
    {3, FailureInf2864, 2864},//高压侧PT断线恢复
    {3, FailureInf2865, 2865},//低压侧PT断线恢复
    {18, FailureInf2866, 2866},//差流越限恢复
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
    {0, FailureInf2867, 2867},//过负荷告警
    {0, FailureInf2868, 2868},//过负荷告警启动
    {0, FailureInf2869, 2869},//过负荷告警返回
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
8,8,8,8,8,8,8,8,// 大于等于继电器
9,9,9,9,9,9,9,9,// 常数时间继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 发送链接继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 接收链接继电器
// 电保持中间继
// 电保持自保持继
// 调试输出继电器
// 调试输入继电器
// 当地控制继电器
0,0,0,0,0,0,0,0,// 计数继电器

};
//继电器时间定值(关联到定值或辅助定值去)
const  TTimeSetting TimeSetting[] = {
	{ (LONG*)(AS_BUF_ADDR + 4), (LONG*)(AS_BUF_ADDR + 8) },
	{ (LONG*)(AS_BUF_ADDR + 12), (LONG*)(AS_BUF_ADDR + 16) },
	{ (LONG*)(AS_BUF_ADDR + 20), (LONG*)(AS_BUF_ADDR + 24) },
	{ (LONG*)(AS_BUF_ADDR + 28), (LONG*)(AS_BUF_ADDR + 32) },
	{ (LONG*)(AS_BUF_ADDR + 36), (LONG*)(AS_BUF_ADDR + 40) },
	{ (LONG*)(AS_BUF_ADDR + 44), (LONG*)(AS_BUF_ADDR + 48) },
	{ (LONG*)(AS_BUF_ADDR + 52), (LONG*)(AS_BUF_ADDR + 56) },
	{ (LONG*)(AS_BUF_ADDR + 60), (LONG*)(AS_BUF_ADDR + 64) },
	{ (LONG*)(AS_BUF_ADDR + 68), (LONG*)(AS_BUF_ADDR + 72) },
	{ (LONG*)(AS_BUF_ADDR + 76), (LONG*)(AS_BUF_ADDR + 80) },
	{ (LONG*)(AS_BUF_ADDR + 84), (LONG*)(AS_BUF_ADDR + 88) },
	{ (LONG*)(AS_BUF_ADDR + 92), (LONG*)(AS_BUF_ADDR + 96) },
	{ (LONG*)(AS_BUF_ADDR + 100), (LONG*)(AS_BUF_ADDR + 104) },
	{ (LONG*)(AS_BUF_ADDR + 108), (LONG*)(AS_BUF_ADDR + 112) },
	{ (LONG*)(AS_BUF_ADDR + 116), (LONG*)(AS_BUF_ADDR + 120) },
	{ (LONG*)(AS_BUF_ADDR + 124), (LONG*)(AS_BUF_ADDR + 128) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 108) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 232) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 268) ,
	 (LONG*)(AS_BUF_ADDR + 272) ,
	 (LONG*)(AS_BUF_ADDR + 276) ,
	 (LONG*)(AS_BUF_ADDR + 280) ,
};
//开入继电器映射到实际开入
const WORD InPutMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
	 (WORD)(5) ,
	 (WORD)(6) ,
	 (WORD)(7) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(10) ,
	 (WORD)(11) ,
	 (WORD)(12) ,
	 (WORD)(13) ,
	 (WORD)(14) ,
	 (WORD)(15) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
	 (WORD)(19) ,
	 (WORD)(20) ,
	 (WORD)(21) ,
	 (WORD)(22) ,
	 (WORD)(23) ,
	 (WORD)(24) ,
	 (WORD)(25) ,
	 (WORD)(26) ,
	 (WORD)(27) ,
	 (WORD)(28) ,
	 (WORD)(29) ,
	 (WORD)(30) ,
	 (WORD)(31) ,
	 (WORD)(32) ,
	 (WORD)(33) ,
	 (WORD)(34) ,
	 (WORD)(35) ,
	 (WORD)(36) ,
	 (WORD)(37) ,
	 (WORD)(38) ,
	 (WORD)(39) ,
	 (WORD)(40) ,
	 (WORD)(41) ,
	 (WORD)(42) ,
	 (WORD)(43) ,
	 (WORD)(44) ,
	 (WORD)(45) ,
	 (WORD)(46) ,
	 (WORD)(47) ,
	 (WORD)(48) ,
	 (WORD)(49) ,
	 (WORD)(50) ,
	 (WORD)(51) ,
	 (WORD)(52) ,
	 (WORD)(53) ,
	 (WORD)(54) ,
	 (WORD)(55) ,
	 (WORD)(56) ,
	 (WORD)(57) ,
	 (WORD)(58) ,
	 (WORD)(59) ,
	 (WORD)(60) ,
	 (WORD)(61) ,
	 (WORD)(62) ,
	 (WORD)(63) ,
	 (WORD)(64) ,
	 (WORD)(65) ,
	 (WORD)(66) ,
	 (WORD)(67) ,
};
//开出继电器映射到实际开出
const WORD OutPutMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
	 (WORD)(5) ,
	 (WORD)(6) ,
	 (WORD)(7) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(10) ,
	 (WORD)(11) ,
	 (WORD)(12) ,
	 (WORD)(13) ,
	 (WORD)(24) ,
	 (WORD)(25) ,
	 (WORD)(26) ,
	 (WORD)(27) ,
	 (WORD)(28) ,
	 (WORD)(29) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
	 (WORD)(19) ,
	 (WORD)(20) ,
	 (WORD)(21) ,
	 (WORD)(22) ,
	 (WORD)(23) ,
	 (WORD)(40) ,
	 (WORD)(41) ,
	 (WORD)(42) ,
	 (WORD)(43) ,
	 (WORD)(44) ,
};
//压板继电器映射到压板值在内存中位置
BYTE* const SWMap[] = {
	 (BYTE*)(SW_BUF_ADDR + 0) ,
	 (BYTE*)(SW_BUF_ADDR + 1) ,
	 (BYTE*)(SW_BUF_ADDR + 2) ,
	 (BYTE*)(SW_BUF_ADDR + 3) ,
	 (BYTE*)(SW_BUF_ADDR + 4) ,
	 (BYTE*)(SW_BUF_ADDR + 5) ,
	 (BYTE*)(SW_BUF_ADDR + 6) ,
	 (BYTE*)(SW_BUF_ADDR + 7) ,
	 (BYTE*)(SW_BUF_ADDR + 8) ,
	 (BYTE*)(SW_BUF_ADDR + 9) ,
	 (BYTE*)(SW_BUF_ADDR + 10) ,
	 (BYTE*)(SW_BUF_ADDR + 11) ,
	 (BYTE*)(SW_BUF_ADDR + 12) ,
	 (BYTE*)(SW_BUF_ADDR + 13) ,
};
//压板继电器映射到压板号压板号
const WORD  SWIndexMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
	 (WORD)(5) ,
	 (WORD)(6) ,
	 (WORD)(7) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(10) ,
	 (WORD)(11) ,
	 (WORD)(12) ,
	 (WORD)(13) ,
};
//配置继电器映射到配置
BYTE* const CFGMap[] = {
	 (BYTE*)(CFG_BUF_ADDR + 0) ,
	 (BYTE*)(CFG_BUF_ADDR + 1) ,
	 (BYTE*)(CFG_BUF_ADDR + 2) ,
	 (BYTE*)(CFG_BUF_ADDR + 3) ,
	 (BYTE*)(CFG_BUF_ADDR + 4) ,
	 (BYTE*)(CFG_BUF_ADDR + 5) ,
	 (BYTE*)(CFG_BUF_ADDR + 6) ,
	 (BYTE*)(CFG_BUF_ADDR + 7) ,
	 (BYTE*)(CFG_BUF_ADDR + 8) ,
	 (BYTE*)(CFG_BUF_ADDR + 9) ,
	 (BYTE*)(CFG_BUF_ADDR + 10) ,
	 (BYTE*)(CFG_BUF_ADDR + 11) ,
	 (BYTE*)(CFG_BUF_ADDR + 12) ,
	 (BYTE*)(CFG_BUF_ADDR + 13) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 15) ,
	 (BYTE*)(CFG_BUF_ADDR + 16) ,
	 (BYTE*)(CFG_BUF_ADDR + 17) ,
	 (BYTE*)(CFG_BUF_ADDR + 18) ,
	 (BYTE*)(CFG_BUF_ADDR + 19) ,
	 (BYTE*)(CFG_BUF_ADDR + 20) ,
	 (BYTE*)(CFG_BUF_ADDR + 21) ,
	 (BYTE*)(CFG_BUF_ADDR + 22) ,
	 (BYTE*)(CFG_BUF_ADDR + 23) ,
	 (BYTE*)(CFG_BUF_ADDR + 24) ,
	 (BYTE*)(CFG_BUF_ADDR + 25) ,
	 (BYTE*)(CFG_BUF_ADDR + 26) ,
	 (BYTE*)(CFG_BUF_ADDR + 27) ,
	 (BYTE*)(CFG_BUF_ADDR + 28) ,
	 (BYTE*)(CFG_BUF_ADDR + 29) ,
	 (BYTE*)(CFG_BUF_ADDR + 30) ,
	 (BYTE*)(CFG_BUF_ADDR + 31) ,
	 (BYTE*)(CFG_BUF_ADDR + 32) ,
	 (BYTE*)(CFG_BUF_ADDR + 33) ,
	 (BYTE*)(CFG_BUF_ADDR + 34) ,
	 (BYTE*)(CFG_BUF_ADDR + 35) ,
	 (BYTE*)(CFG_BUF_ADDR + 36) ,
	 (BYTE*)(CFG_BUF_ADDR + 37) ,
	 (BYTE*)(CFG_BUF_ADDR + 38) ,
	 (BYTE*)(CFG_BUF_ADDR + 39) ,
	 (BYTE*)(CFG_BUF_ADDR + 40) ,
	 (BYTE*)(CFG_BUF_ADDR + 41) ,
	 (BYTE*)(CFG_BUF_ADDR + 42) ,
	 (BYTE*)(CFG_BUF_ADDR + 43) ,
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	30897, //Up
	11500, //U0h
	11500, //U0l
	3050, //Ip
	3050, //I0
	3050, //Ijx
	3050, //Id
	43689, //UAM
	43689, //UBM
	43689, //UCM
	16152, //U0hM
	43689, //UaM
	43689, //UbM
	43689, //UcM
	16152, //U0lM
	4310, //IAM
	4310, //IBM
	4310, //ICM
	4310, //IaM
	4310, //IbM
	4310, //IcM
	4310, //I0M
	4310, //IjxM
	20, //UAD
	20, //UBD
	20, //UCD
	20, //U0hD
	20, //UaD
	20, //UbD
	20, //UcD
	20, //U0lD
	20, //IAD
	20, //IBD
	20, //ICD
	20, //IaD
	20, //IbD
	20, //IcD
	20, //I0D
	20, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
};
//系数2
const LONG CFDefaultVal2[]=
{
	30897, //Up
	11500, //U0h
	11500, //U0l
	15250, //Ip
	15250, //I0
	15250, //Ijx
	15250, //Id
	43689, //UAM
	43689, //UBM
	43689, //UCM
	16152, //U0hM
	43689, //UaM
	43689, //UbM
	43689, //UcM
	16152, //U0lM
	21550, //IAM
	21550, //IBM
	21550, //ICM
	21550, //IaM
	21550, //IbM
	21550, //IcM
	21550, //I0M
	21550, //IjxM
	20, //UAD
	20, //UBD
	20, //UCD
	20, //U0hD
	20, //UaD
	20, //UbD
	20, //UcD
	20, //U0lD
	20, //IAD
	20, //IBD
	20, //ICD
	20, //IaD
	20, //IbD
	20, //IcD
	20, //I0D
	20, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
};

//传动----------------------------------
