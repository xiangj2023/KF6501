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
};
//辅助定值取值信息
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{PHASEU, "57.7V"},
	{LINEU, "100V"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //辅助定值额定电流
	{2, (TSetValueInf*)&(SysSetValueInf[2])}, //辅助定值额定电压
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
LONG* 	RM_pnUa 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// Ua
LONG* 	RM_pnUb 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// Ub
LONG* 	RM_pnUc 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// Uc
LONG* 	RM_pnU0 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// U0
LONG* 	RM_pnUL1 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// UL1
LONG* 	RM_pnUL2 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// UL2
LONG* 	RM_pnUL3 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// UL3
LONG* 	RM_pnUab 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// Uab
LONG* 	RM_pnUbc 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// Ubc
LONG* 	RM_pnUca 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// Uca
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// U2
LONG* 	RM_pnU05 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// U05
LONG* 	RM_pnIa 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Ia
LONG* 	RM_pnIb 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// Ib
LONG* 	RM_pnIc 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// Ic
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// I0
LONG* 	RM_pnI1 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// I1
LONG* 	RM_pnI05 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// I05
LONG* 	RM_pnAUa 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// ΦUa
LONG* 	RM_pnAUb 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// ΦUb
LONG* 	RM_pnAUc 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// ΦUc
LONG* 	RM_pnAUab 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// ΦUab
LONG* 	RM_pnAUbc 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// ΦUbc
LONG* 	RM_pnAUca 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// ΦUca
LONG* 	RM_pnAUl1 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// ΦUl1
LONG* 	RM_pnAUl2 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// ΦUl2
LONG* 	RM_pnAUl3 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// ΦUl3
LONG* 	RM_pnAIpa 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// ΦIpa
LONG* 	RM_pnAIpb 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// ΦIpb
LONG* 	RM_pnAIpc 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// ΦIpc
LONG* 	RM_pnAIP1 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// ΦIp1
LONG* 	RM_pnAIma 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// ΦIma
LONG* 	RM_pnAImb 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// ΦImb
LONG* 	RM_pnAImc 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// ΦImc
LONG* 	RM_pnAU01 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// ΦU01
LONG* 	RM_pnAI01 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// ΦI01
LONG* 	RM_pnAU05 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// ΦU05
LONG* 	RM_pnAI05 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// ΦI05
LONG* 	RM_pnF1 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// F1
LONG* 	RM_pnIam 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// Iam
LONG* 	RM_pnIbm 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// Ibm
LONG* 	RM_pnIcm 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// Icm
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUa 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//Ua Ua
LONG* 	M_pnUb 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//Ub Ub
LONG* 	M_pnUc 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//Uc Uc
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnUab 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Uab Uab
LONG* 	M_pnUbc 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ubc Ubc
LONG* 	M_pnUca 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Uca Uca
LONG* 	M_pnU1 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//U1 U1
LONG* 	M_pnU2 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//U2 U2
LONG* 	M_pnU3 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//U3 U3
LONG* 	M_pnIa 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//Ia Ia
LONG* 	M_pnIb 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Ib Ib
LONG* 	M_pnIc 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ic Ic
LONG* 	M_pnI0 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I0 I0
LONG* 	M_pnI1 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I1 I1
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//COS COS
LONG* 	M_pnF1 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//F1 F1


//软压板----------------------------------
BYTE* SW_pby50I0 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//电流速断
BYTE* SW_pby50I1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//过流保护
BYTE* SW_pby59U 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//过压保护
BYTE* SW_pby27U 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//低压保护
BYTE* SW_pby60I 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//不平衡电流
BYTE* SW_pby60U 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//不平衡电压
BYTE* SW_pbyJDXX 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//接地选线1
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby50I0 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//电流速断
BYTE* CFG_pby50I1 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//过流保护
BYTE* CFG_pby59U 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//过压保护
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//低压保护
BYTE* CFG_pby60I 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//不平衡电流保护
BYTE* CFG_pby60U 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//不平衡电压保护
BYTE* CFG_pby59N 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//零压告警
BYTE* CFG_pbyJDXX 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//接地选线
BYTE* CFG_pbyXHXQ 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//消弧线圈接地
BYTE* CFG_pbyPTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//PT断线检查
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//CT断线检查
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//备用配置2

//定值----------------------------------
LONG* RS_pnCFG 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字 CFG
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//突变量启动电流 PIC
LONG* RS_pn50I0C 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//电流速断电流 50I0C
LONG* RS_pn50I0D 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//电流速断时间 50I0D
LONG* RS_pn50I1C 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//过流电流定值 50I1C
LONG* RS_pn50I1D 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//过流时间定值 50I1D
LONG* RS_pn59U 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//过压电压定值 59U
LONG* RS_pn59D 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//过压时间定值 59D
LONG* RS_pn27U 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//低压保护电压 27U
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//低压保护时间 27D
LONG* RS_pn27ILO 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//低压闭锁电流定值 27ILO
LONG* RS_pn60IC 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//不平衡电流保护电流 60IC
LONG* RS_pn60ID 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//不平衡电流保护时间 60ID
LONG* RS_pn60UV 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//不平衡电压保护电压 60UV
LONG* RS_pn60UD 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//不平衡电压保护时间 60UD
LONG* RS_pn59NU 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//零压告警电压 59NU
LONG* RS_pn59ND 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//零压告警时间 59ND
LONG* RS_pn74PTU 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//PT断线检测电压 74PTU
LONG* RS_pn74PTI 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//PT断线检测电流 74PTI
LONG* RS_pn74PTD 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//PT断线检测时间 74PTD
LONG* RS_pn74PTU2 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//负序电压检测定值 74PTU2
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//失灵保护时间 52BFD

//辅助定值----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//额定电流
LONG* AS_pnPM 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//额定电压
LONG* AS_pnT1PI 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//T1启动延时
LONG* AS_pnT1RE 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//T1返回延时
LONG* AS_pnT2PI 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//T2启动延时
LONG* AS_pnT2RE 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//T2返回延时
LONG* AS_pnT3PI 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//T3启动延时
LONG* AS_pnT3RE 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//T3返回延时
LONG* AS_pnT4PI 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//T4启动延时
LONG* AS_pnT4RE 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//T4返回延时
LONG* AS_pnT5PI 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//T5启动延时
LONG* AS_pnT5RE 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//T5返回延时
LONG* AS_pnT6PI 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//T6启动延时
LONG* AS_pnT6RE 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//T6返回延时
LONG* AS_pnT7PI 	= 	(LONG*)(AS_BUF_ADDR + 56); 	//T7启动延时
LONG* AS_pnT7RE 	= 	(LONG*)(AS_BUF_ADDR + 60); 	//T7返回延时
LONG* AS_pnT8PI 	= 	(LONG*)(AS_BUF_ADDR + 64); 	//T8启动延时
LONG* AS_pnT8RE 	= 	(LONG*)(AS_BUF_ADDR + 68); 	//T8返回延时
LONG* AS_pnT9PI 	= 	(LONG*)(AS_BUF_ADDR + 72); 	//T9启动延时
LONG* AS_pnT9RE 	= 	(LONG*)(AS_BUF_ADDR + 76); 	//T9返回延时
LONG* AS_pnT10PI 	= 	(LONG*)(AS_BUF_ADDR + 80); 	//T10启动延时
LONG* AS_pnT10RE 	= 	(LONG*)(AS_BUF_ADDR + 84); 	//T10返回延时
LONG* AS_pnT11PI 	= 	(LONG*)(AS_BUF_ADDR + 88); 	//T11启动延时
LONG* AS_pnT11RE 	= 	(LONG*)(AS_BUF_ADDR + 92); 	//T11返回延时
LONG* AS_pnT12PI 	= 	(LONG*)(AS_BUF_ADDR + 96); 	//T12启动延时
LONG* AS_pnT12RE 	= 	(LONG*)(AS_BUF_ADDR + 100); 	//T12返回延时
LONG* AS_pnT13PI 	= 	(LONG*)(AS_BUF_ADDR + 104); 	//T13启动延时
LONG* AS_pnT13RE 	= 	(LONG*)(AS_BUF_ADDR + 108); 	//T13返回延时
LONG* AS_pnT14PI 	= 	(LONG*)(AS_BUF_ADDR + 112); 	//T14启动延时
LONG* AS_pnT14RE 	= 	(LONG*)(AS_BUF_ADDR + 116); 	//T14返回延时
LONG* AS_pnT15PI 	= 	(LONG*)(AS_BUF_ADDR + 120); 	//T15启动延时
LONG* AS_pnT15RE 	= 	(LONG*)(AS_BUF_ADDR + 124); 	//T15返回延时
LONG* AS_pnT16PI 	= 	(LONG*)(AS_BUF_ADDR + 128); 	//T16启动延时
LONG* AS_pnT16RE 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//T16返回延时
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//断路器动作延时
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//电隔动作延时
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//TR2保持时间
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//TR3保持时间
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//故障录波触发时间
LONG* AS_pn59UA 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//Ua有压定值
LONG* AS_pn27UA 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//Ua无压定值
LONG* AS_pn59UB 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//Ub有压定值
LONG* AS_pn27UB 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//Ub无压定值
LONG* AS_pn59UC 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//Uc有压定值
LONG* AS_pn27UC 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//Uc无压定值
LONG* AS_pn59U0 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//U0有压定值
LONG* AS_pn27U0 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//U0无压定值
LONG* AS_pn59U1 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//U1有压定值
LONG* AS_pn27U1 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U1无压定值
LONG* AS_pn59U2 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U2有压定值
LONG* AS_pn27U2 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U2无压定值
LONG* AS_pn59U3 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//U3有压定值
LONG* AS_pn27U3 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//U3无压定值
LONG* AS_pn50IA 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//Ia有流定值
LONG* AS_pn37IA 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//Ia无流定值
LONG* AS_pn50IB 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//Ib有流定值
LONG* AS_pn37IB 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//Ib无流定值
LONG* AS_pn50IC 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//Ic有流定值
LONG* AS_pn37IC 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//Ic无流定值
LONG* AS_pn50I0 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//I0有流定值
LONG* AS_pn37I0 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//I0无流定值
LONG* AS_pn50I1 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//I1有流定值
LONG* AS_pn37I1 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//I1无流定值

//变比----------------------------------
LONG* AS_pnPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//PT变比
LONG* AS_pnPCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//保护CT变比
LONG* AS_pnMCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//测量CT变比
LONG* AS_pnZCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//零序CT变比
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//rt1

//系数----------------------------------
LONG* CF_pnUp 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//Up
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U0
LONG* CF_pnIp 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//Ip
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//I0
LONG* CF_pnI1 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//I1
LONG* CF_pnUaM 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UaM
LONG* CF_pnUbM 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//UbM
LONG* CF_pnUcM 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UcM
LONG* CF_pnUL1M 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//UL1M
LONG* CF_pnUL2M 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//UL2M
LONG* CF_pnUL3M 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//UL3M
LONG* CF_pnU0M 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//U0M
LONG* CF_pnIAM 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IAM
LONG* CF_pnIBM 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IBM
LONG* CF_pnICM 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//ICM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//I0M
LONG* CF_pnI1M 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//I1M
LONG* CF_pnPA 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//PA
LONG* CF_pnQA 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//QA
LONG* CF_pnPB 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//PB
LONG* CF_pnQB 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//QB
LONG* CF_pnPC 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//PC
LONG* CF_pnQC 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//QC
LONG* CF_pnAA 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//AA
LONG* CF_pnBA 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//BA
LONG* CF_pnCA 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//CA
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//C4
LONG* CF_pnIaDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//IaDRIFT
LONG* CF_pnIbDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IbDRIFT
LONG* CF_pnIcDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IcDRIFT
LONG* CF_pnI0DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//I0DRIFT
LONG* CF_pnI1DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//I1DRIFT
LONG* CF_pnUaDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//UaDRIFT
LONG* CF_pnUbDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//UbDRIFT
LONG* CF_pnUcDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//UcDRIFT
LONG* CF_pnU0DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//U0DRIFT
LONG* CF_pnUl1DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//Ul1DRIFT
LONG* CF_pnUl2DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//Ul2DRIFT
LONG* CF_pnUl3DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//Ul3DRIFT

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 176), // IN1
    (LONG*)(RM_BUF_ADDR + 180) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 168), // BR1
    (LONG*)(RM_BUF_ADDR + 172) // BR2
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

//保护启动500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//电流速断启动3100----------
const  LONG* FailureInf3100[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//电流速断动作3101----------
const  LONG* FailureInf3101[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//电流速断返回3102----------
const  LONG* FailureInf3102[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//过流保护启动3103----------
const  LONG* FailureInf3103[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//过流保护动作3104----------
const  LONG* FailureInf3104[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//过流保护返回3105----------
const  LONG* FailureInf3105[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//不平衡电流保护启动3106----------
const  LONG* FailureInf3106[] = {
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//不平衡电流保护动作3107----------
const  LONG* FailureInf3107[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//不平衡电流保护返回3108----------
const  LONG* FailureInf3108[] = {
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//不平衡电压保护启动3109----------
const  LONG* FailureInf3109[] = {
    (LONG*)(RM_BUF_ADDR + 16) // UL1
};

//不平衡电压保护动作3110----------
const  LONG* FailureInf3110[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//不平衡电压保护返回3111----------
const  LONG* FailureInf3111[] = {
    (LONG*)(RM_BUF_ADDR + 16) // UL1
};

//过压保护启动3112----------
const  LONG* FailureInf3112[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过压保护动作3113----------
const  LONG* FailureInf3113[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//过压保护返回3114----------
const  LONG* FailureInf3114[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//低压保护动作3115----------
const  LONG* FailureInf3115[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//零压告警3116----------
const  LONG* FailureInf3116[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//母线PT断线3117----------
const  LONG* FailureInf3117[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 40), // U2
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//线路接地3118----------
const  LONG* FailureInf3118[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//母线接地3119----------
const  LONG* FailureInf3119[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//保护启动3120----------
const  LONG* FailureInf3120[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//断路器跳闸失败3121----------
const  LONG* FailureInf3121[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//通道测试3122----------
const  LONG* FailureInf3122[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//失灵保护3123----------
const  LONG* FailureInf3123[] = {
    {NULL}
};

//A相CT断线3124----------
const  LONG* FailureInf3124[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//A相CT断线恢复3125----------
const  LONG* FailureInf3125[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//B相CT断线3126----------
const  LONG* FailureInf3126[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//B相CT断线恢复3127----------
const  LONG* FailureInf3127[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//C相CT断线3128----------
const  LONG* FailureInf3128[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//C相CT断线恢复3129----------
const  LONG* FailureInf3129[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//5V电源故障20----------
const  LONG* FailureInf20[] = {
    {NULL}
};

//EEPROM写入故障21----------
const  LONG* FailureInf21[] = {
    {NULL}
};

//5V电源故障恢复64----------
const  LONG* FailureInf64[] = {
    {NULL}
};

//母线PT断线恢复3130----------
const  LONG* FailureInf3130[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 40), // U2
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//保护数据异常22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//保护数据异常恢复65----------
const  LONG* FailureInf65[] = {
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
    {0, FailureInf50, 50},//AD故障恢复
    {0, FailureInf51, 51},//软压板故障恢复
    {0, FailureInf52, 52},//EEPROM定值故障恢复
    {0, FailureInf55, 55},//RAM定值故障恢复
    {0, FailureInf58, 58},//程序ROM故障恢复
    {0, FailureInf60, 60},//通信通道异常恢复
    {0, FailureInf62, 62},//NVRAM内容出错恢复
    {0, FailureInf63, 63},//逻辑方程故障恢复
    {0, FailureInf500, 500},//保护启动
    {3, FailureInf3100, 3100},//电流速断启动
    {15, FailureInf3101, 3101},//电流速断动作
    {3, FailureInf3102, 3102},//电流速断返回
    {3, FailureInf3103, 3103},//过流保护启动
    {15, FailureInf3104, 3104},//过流保护动作
    {3, FailureInf3105, 3105},//过流保护返回
    {1, FailureInf3106, 3106},//不平衡电流保护启动
    {15, FailureInf3107, 3107},//不平衡电流保护动作
    {1, FailureInf3108, 3108},//不平衡电流保护返回
    {1, FailureInf3109, 3109},//不平衡电压保护启动
    {15, FailureInf3110, 3110},//不平衡电压保护动作
    {1, FailureInf3111, 3111},//不平衡电压保护返回
    {6, FailureInf3112, 3112},//过压保护启动
    {15, FailureInf3113, 3113},//过压保护动作
    {6, FailureInf3114, 3114},//过压保护返回
    {15, FailureInf3115, 3115},//低压保护动作
    {15, FailureInf3116, 3116},//零压告警
    {14, FailureInf3117, 3117},//母线PT断线
    {15, FailureInf3118, 3118},//线路接地
    {15, FailureInf3119, 3119},//母线接地
    {15, FailureInf3120, 3120},//保护启动
    {5, FailureInf3121, 3121},//断路器跳闸失败
    {15, FailureInf3122, 3122},//通道测试
    {0, FailureInf3123, 3123},//失灵保护
    {6, FailureInf3124, 3124},//A相CT断线
    {6, FailureInf3125, 3125},//A相CT断线恢复
    {6, FailureInf3126, 3126},//B相CT断线
    {6, FailureInf3127, 3127},//B相CT断线恢复
    {6, FailureInf3128, 3128},//C相CT断线
    {6, FailureInf3129, 3129},//C相CT断线恢复
    {0, FailureInf20, 20},//5V电源故障
    {0, FailureInf21, 21},//EEPROM写入故障
    {0, FailureInf64, 64},//5V电源故障恢复
    {14, FailureInf3130, 3130},//母线PT断线恢复
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
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
	{ (LONG*)(AS_BUF_ADDR + 8), (LONG*)(AS_BUF_ADDR + 12) },
	{ (LONG*)(AS_BUF_ADDR + 16), (LONG*)(AS_BUF_ADDR + 20) },
	{ (LONG*)(AS_BUF_ADDR + 24), (LONG*)(AS_BUF_ADDR + 28) },
	{ (LONG*)(AS_BUF_ADDR + 32), (LONG*)(AS_BUF_ADDR + 36) },
	{ (LONG*)(AS_BUF_ADDR + 40), (LONG*)(AS_BUF_ADDR + 44) },
	{ (LONG*)(AS_BUF_ADDR + 48), (LONG*)(AS_BUF_ADDR + 52) },
	{ (LONG*)(AS_BUF_ADDR + 56), (LONG*)(AS_BUF_ADDR + 60) },
	{ (LONG*)(AS_BUF_ADDR + 64), (LONG*)(AS_BUF_ADDR + 68) },
	{ (LONG*)(AS_BUF_ADDR + 72), (LONG*)(AS_BUF_ADDR + 76) },
	{ (LONG*)(AS_BUF_ADDR + 80), (LONG*)(AS_BUF_ADDR + 84) },
	{ (LONG*)(AS_BUF_ADDR + 88), (LONG*)(AS_BUF_ADDR + 92) },
	{ (LONG*)(AS_BUF_ADDR + 96), (LONG*)(AS_BUF_ADDR + 100) },
	{ (LONG*)(AS_BUF_ADDR + 104), (LONG*)(AS_BUF_ADDR + 108) },
	{ (LONG*)(AS_BUF_ADDR + 112), (LONG*)(AS_BUF_ADDR + 116) },
	{ (LONG*)(AS_BUF_ADDR + 120), (LONG*)(AS_BUF_ADDR + 124) },
	{ (LONG*)(AS_BUF_ADDR + 128), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 84) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 144) ,
	 (LONG*)(AS_BUF_ADDR + 148) ,
	 (LONG*)(AS_BUF_ADDR + 152) ,
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
	 (WORD)(14) ,
	 (WORD)(15) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
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
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	30890, //Up
	30890, //U0
	3050, //Ip
	15250, //I0
	15250, //I1
	43690, //UaM
	43690, //UbM
	43690, //UcM
	43690, //UL1M
	43690, //UL2M
	43690, //UL3M
	43690, //U0M
	64480, //IAM
	64480, //IBM
	64480, //ICM
	21600, //I0M
	21600, //I1M
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	4, //IaDRIFT
	4, //IbDRIFT
	4, //IcDRIFT
	4, //I0DRIFT
	4, //I1DRIFT
	4, //UaDRIFT
	4, //UbDRIFT
	4, //UcDRIFT
	4, //U0DRIFT
	4, //Ul1DRIFT
	4, //Ul2DRIFT
	4, //Ul3DRIFT
};
//系数2
const LONG CFDefaultVal2[]=
{
	30890, //Up
	30890, //U0
	15250, //Ip
	15250, //I0
	15250, //I1
	43690, //UaM
	43690, //UbM
	43690, //UcM
	43690, //UL1M
	43690, //UL2M
	43690, //UL3M
	43690, //U0M
	322400, //IAM
	322400, //IBM
	322400, //ICM
	21600, //I0M
	21600, //I1M
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	4, //IaDRIFT
	4, //IbDRIFT
	4, //IcDRIFT
	4, //I0DRIFT
	4, //I1DRIFT
	4, //UaDRIFT
	4, //UbDRIFT
	4, //UcDRIFT
	4, //U0DRIFT
	4, //Ul1DRIFT
	4, //Ul2DRIFT
	4, //Ul3DRIFT
};

//传动----------------------------------
