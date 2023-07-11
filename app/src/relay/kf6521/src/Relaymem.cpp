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
	{SYN_PHASE_UA, "UA"},
	{SYN_PHASE_UB, "UB"},
	{SYN_PHASE_UC, "UC"},
	{SYN_PHASE_UAB, "UAB"},
	{SYN_PHASE_UBC, "UBC"},
	{SYN_PHASE_UCA, "UCA"},
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
	{6, (TSetValueInf*)&(SetValueInf[0])}, //定值同期相别
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
LONG* 	RM_pnF2 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// F2
LONG* 	RM_pnDiffU 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// ΔU
LONG* 	RM_pnDiffA 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// ΔΦ
LONG* 	RM_pnDiffdq 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// Δdq
LONG* 	RM_pnIam 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// Iam
LONG* 	RM_pnIbm 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// Ibm
LONG* 	RM_pnIcm 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// Icm
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 184); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 188); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 192); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 196); 	// IN2


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
LONG* 	M_pnF2 	= 	(LONG*)(MEASURE_BUF_ADDR + 76); 	//F2 F2
LONG* 	M_pnVD 	= 	(LONG*)(MEASURE_BUF_ADDR + 80); 	//VD VD
LONG* 	M_pnAD 	= 	(LONG*)(MEASURE_BUF_ADDR + 84); 	//AD AD


//软压板----------------------------------
BYTE* SW_pby50I0 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//电流速断
BYTE* SW_pby50I1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//过流Ⅰ段
BYTE* SW_pby50I2 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//过流Ⅱ段
BYTE* SW_pby50I3 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//过流Ⅲ段
BYTE* SW_pby50N1 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//零序过流Ⅰ段
BYTE* SW_pby50N2 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//零序过流Ⅱ段
BYTE* SW_pby50ISH 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//母线充电保护
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//低压保护
BYTE* SW_pby79 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//重合闸
BYTE* SW_pbyJDXX 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//接地选线
BYTE* SW_pby25 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//检同期
BYTE* SW_pby83 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//备自投
BYTE* SW_pby50IOL 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//过负荷告警
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 13); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 14); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 15); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby50I0 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//电流速断
BYTE* CFG_pby50I0_47 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//电流速断复压闭锁
BYTE* CFG_pby50I0_67 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//电流速断功率方向
BYTE* CFG_pby50I0_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//电流速断方向正向
BYTE* CFG_pby50I1 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//过流I段
BYTE* CFG_pby50I1_47 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//I段复压闭锁
BYTE* CFG_pby50I1_67 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//I段功率方向
BYTE* CFG_pby50I1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//I段方向正向
BYTE* CFG_pby50I2 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//过流II段
BYTE* CFG_pby50I2_47 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//II段复压闭锁
BYTE* CFG_pby50I2_67 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//II段功率方向
BYTE* CFG_pby50I2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//II段方向正向
BYTE* CFG_pby50I3 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//过流III段
BYTE* CFG_pby50I3_47 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//III段复压闭锁
BYTE* CFG_pby50I3_67 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//III段功率方向
BYTE* CFG_pby50I3_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//III段方向正向
BYTE* CFG_pby50N1 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//零序过流I段
BYTE* CFG_pby50N2 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//零序过流II段
BYTE* CFG_pby50I_SH 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//母线充电保护
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//低压保护
BYTE* CFG_pby79_50I0 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//电流速断重合
BYTE* CFG_pby79_50I1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//过流I段重合
BYTE* CFG_pby79_50I2 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//过流II段重合
BYTE* CFG_pby79_50I3 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//过流III段重合
BYTE* CFG_pby79UC 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//偷跳重合闸
BYTE* CFG_pby79_25 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//重合闸检同期
BYTE* CFG_pby79_27 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//重合闸检无压
BYTE* CFG_pby79_59 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//重合闸检有压
BYTE* CFG_pby50I1_94 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//过流I段加速
BYTE* CFG_pby50I2_94 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//过流II段加速
BYTE* CFG_pby50I3_94 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//过流III段加速
BYTE* CFG_pby59N_74 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//零压告警
BYTE* CFG_pbyJDXX 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//接地选线
BYTE* CFG_pbyXHJD 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//消弧线圈接地
BYTE* CFG_pbyPTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//PT断线检查
BYTE* CFG_pby25 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//手合检同期
BYTE* CFG_pbySH_27 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//手合检无压
BYTE* CFG_pby83 	= 	(BYTE*)(CFG_BUF_ADDR + 37); 	//备自投
BYTE* CFG_pby50IOL 	= 	(BYTE*)(CFG_BUF_ADDR + 38); 	//过负荷告警
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 39); 	//CT断线检查
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 40); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 41); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 42); 	//备用配置2

//定值----------------------------------
LONG* RS_pnCFG1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字 CFG1
LONG* RS_pnCFG2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//配置字2 CFG2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//突变量启动电流 PIC
LONG* RS_pn50P0C 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//电流速断电流 50P0C
LONG* RS_pn50P0D 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//电流速断时间 50P0D
LONG* RS_pn50P0_27 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//电流速断低压定值 50P0_27
LONG* RS_pn50P0_47 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//电流速断负序电压 50P0_47
LONG* RS_pn50P1C 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//过流Ⅰ段电流 50P1C
LONG* RS_pn50P1D 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//过流Ⅰ段时间 50P1D
LONG* RS_pn50P1_27 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//过流Ⅰ段低压定值 50P1_27
LONG* RS_pn50P1_47 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//过流Ⅰ段负序电压 50P1_47
LONG* RS_pn50P2C 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//过流Ⅱ段电流 50P2C
LONG* RS_pn50P2D 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//过流Ⅱ段时间 50P2D
LONG* RS_pn50P2_27 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//过流Ⅱ段低压定值 50P2_27
LONG* RS_pn50P2_47 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//过流Ⅱ段负序电压 50P2_47
LONG* RS_pn50P3C 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//过流Ⅲ段电流 50P3C
LONG* RS_pn50P3D 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//过流Ⅲ段时间 50P3D
LONG* RS_pn50P3_27 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//过流Ⅲ段低压定值 50P3_27
LONG* RS_pn50P3_47 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//过流Ⅲ段负序电压 50P3_47
LONG* RS_pn67ANG 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//功率方向灵敏角 67ANG
LONG* RS_pn50N1C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//零序过流Ⅰ段电流 50N1C
LONG* RS_pn50N1D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//零序过流Ⅰ段时间 50N1D
LONG* RS_pn50N2C 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//零序过流Ⅱ段电流 50N2C
LONG* RS_pn50N2D 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//零序过流Ⅱ段时间 50N2D
LONG* RS_pn50ISHC 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//母线充电保护电流 50ISHC
LONG* RS_pn50ISHD 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//母线充电保护时间 50ISHD
LONG* RS_pn50ISHT 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//母充保护投入时间 50ISHT
LONG* RS_pn79SH 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//重合闸投入次数 79SH
LONG* RS_pn79D1 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//第一次重合闸时间 79D1
LONG* RS_pn79D2 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//第二次重合闸时间 79D2
LONG* RS_pn79CLSD 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//重合闸动作监视延时 79CLSD
LONG* RS_pn79CT 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//重合闸充电时间 79CT
LONG* RS_pn25_27 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//重合闸检无压定值 25_27
LONG* RS_pn25PA 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//同期功角 25PA
LONG* RS_pn25PH 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//同期相别 25PH
LONG* RS_pn25DT 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//同期检测时间 25DT
LONG* RS_pn25VUL 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//检同期电压上限 25VUL
LONG* RS_pn25VLL 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//检同期电压下限 25VLL
LONG* RS_pn25VD 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//同期允许压差 25VD
LONG* RS_pn25FD 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//同期允许频差 25FD
LONG* RS_pn52CLT 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//断路器合闸时间 52CLT
LONG* RS_pn94D 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//后加速动作时间 94D
LONG* RS_pnSOTFD 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//后加速投入时间 SOTFD
LONG* RS_pn27U 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//低压保护电压 27U
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//低压保护时间 27D
LONG* RS_pn27_LK37 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//低压电流闭锁定值 27_LK37
LONG* RS_pn59NU 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//零压告警电压 59NU
LONG* RS_pn59ND 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//零压告警时间 59ND
LONG* RS_pn27BZT 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//备自投无压定值 27BZT
LONG* RS_pn59BZT 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//备自投有压定值 59BZT
LONG* RS_pn50BZT 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//备自投有流定值 50BZT
LONG* RS_pnCHARD 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//备自投充电时间 CHARD
LONG* RS_pn50OLC 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//过负荷电流 50OLC
LONG* RS_pn50OLD 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//过负荷时间 50OLD
LONG* RS_pnPT1_V 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//母线PT断线检测电压 PT1_V
LONG* RS_pnPT1_C 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//母线PT断线检测电流 PT1_C
LONG* RS_pnPT1_D 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//母线PT断线检测时间 PT1_D
LONG* RS_pnPT2_V1 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//线路PT断线母线有压 PT2_V1
LONG* RS_pnPT2_C 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//线路PT断线母线有流 PT2_C
LONG* RS_pnPT2_V2 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//线路PT断线线路无压 PT2_V2
LONG* RS_pnPT2_D 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//线路PT断线检测时间 PT2_D
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//失灵保护时间 52BFD

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
    (LONG*)(RM_BUF_ADDR + 192), // IN1
    (LONG*)(RM_BUF_ADDR + 196) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 184), // BR1
    (LONG*)(RM_BUF_ADDR + 188) // BR2
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

//通道测试2600----------
const  LONG* FailureInf2600[] = {
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
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 148), // ΦI05
    (LONG*)(RM_BUF_ADDR + 144) // ΦU05
};

//保护启动2601----------
const  LONG* FailureInf2601[] = {
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
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 148), // ΦI05
    (LONG*)(RM_BUF_ADDR + 144) // ΦU05
};

//过流Ⅰ段启动2602----------
const  LONG* FailureInf2602[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅰ段动作2603----------
const  LONG* FailureInf2603[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//过流Ⅰ段返回2604----------
const  LONG* FailureInf2604[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅱ段启动2605----------
const  LONG* FailureInf2605[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅱ段动作2606----------
const  LONG* FailureInf2606[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//过流Ⅱ段返回2607----------
const  LONG* FailureInf2607[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅲ段启动2608----------
const  LONG* FailureInf2608[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅲ段动作2609----------
const  LONG* FailureInf2609[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//过流Ⅲ段返回2610----------
const  LONG* FailureInf2610[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅱ段加速动作2611----------
const  LONG* FailureInf2611[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//过流Ⅲ段加速动作2612----------
const  LONG* FailureInf2612[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//零序过流Ⅰ段启动2613----------
const  LONG* FailureInf2613[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//零序过流Ⅰ段动作2614----------
const  LONG* FailureInf2614[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//零序过流Ⅰ段返回2615----------
const  LONG* FailureInf2615[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//零序过流Ⅱ段启动2616----------
const  LONG* FailureInf2616[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//零序过流Ⅱ段动作2617----------
const  LONG* FailureInf2617[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//零序过流Ⅱ段返回2618----------
const  LONG* FailureInf2618[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//低压保护跳闸2619----------
const  LONG* FailureInf2619[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//断路器跳闸失败2620----------
const  LONG* FailureInf2620[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 68) // I05
};

//一次重合闸出口2621----------
const  LONG* FailureInf2621[] = {
    {NULL}
};

//二次重合闸出口2622----------
const  LONG* FailureInf2622[] = {
    {NULL}
};

//一次重合闸被闭锁2623----------
const  LONG* FailureInf2623[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//备自投启动2624----------
const  LONG* FailureInf2624[] = {
    {NULL}
};

//备自投退出2625----------
const  LONG* FailureInf2625[] = {
    {NULL}
};

//备自投成功2626----------
const  LONG* FailureInf2626[] = {
    {NULL}
};

//备自投失败2627----------
const  LONG* FailureInf2627[] = {
    {NULL}
};

//跳1断路器2628----------
const  LONG* FailureInf2628[] = {
    {NULL}
};

//合1断路器2629----------
const  LONG* FailureInf2629[] = {
    {NULL}
};

//跳1断路器失败2630----------
const  LONG* FailureInf2630[] = {
    {NULL}
};

//合1断路器失败2631----------
const  LONG* FailureInf2631[] = {
    {NULL}
};

//跳2断路器2632----------
const  LONG* FailureInf2632[] = {
    {NULL}
};

//合2断路器2633----------
const  LONG* FailureInf2633[] = {
    {NULL}
};

//跳2断路器失败2634----------
const  LONG* FailureInf2634[] = {
    {NULL}
};

//合2断路器失败2635----------
const  LONG* FailureInf2635[] = {
    {NULL}
};

//跳3断路器2636----------
const  LONG* FailureInf2636[] = {
    {NULL}
};

//合3断路器2637----------
const  LONG* FailureInf2637[] = {
    {NULL}
};

//跳3断路器失败2638----------
const  LONG* FailureInf2638[] = {
    {NULL}
};

//合3断路器失败2639----------
const  LONG* FailureInf2639[] = {
    {NULL}
};

//分4断路器2640----------
const  LONG* FailureInf2640[] = {
    {NULL}
};

//合4断路器2641----------
const  LONG* FailureInf2641[] = {
    {NULL}
};

//分4断路器失败2642----------
const  LONG* FailureInf2642[] = {
    {NULL}
};

//合4断路器失败2643----------
const  LONG* FailureInf2643[] = {
    {NULL}
};

//跳1隔开2644----------
const  LONG* FailureInf2644[] = {
    {NULL}
};

//合1隔开2645----------
const  LONG* FailureInf2645[] = {
    {NULL}
};

//跳1隔开失败2646----------
const  LONG* FailureInf2646[] = {
    {NULL}
};

//合1隔开失败2647----------
const  LONG* FailureInf2647[] = {
    {NULL}
};

//跳2隔开2648----------
const  LONG* FailureInf2648[] = {
    {NULL}
};

//合2隔开2649----------
const  LONG* FailureInf2649[] = {
    {NULL}
};

//跳2隔开失败2650----------
const  LONG* FailureInf2650[] = {
    {NULL}
};

//合2隔开失败2651----------
const  LONG* FailureInf2651[] = {
    {NULL}
};

//跳3隔开2652----------
const  LONG* FailureInf2652[] = {
    {NULL}
};

//合3隔开2653----------
const  LONG* FailureInf2653[] = {
    {NULL}
};

//分3隔开失败2654----------
const  LONG* FailureInf2654[] = {
    {NULL}
};

//合3隔开失败2655----------
const  LONG* FailureInf2655[] = {
    {NULL}
};

//分4隔开2656----------
const  LONG* FailureInf2656[] = {
    {NULL}
};

//合4隔开2657----------
const  LONG* FailureInf2657[] = {
    {NULL}
};

//分4隔开失败2658----------
const  LONG* FailureInf2658[] = {
    {NULL}
};

//合4隔开失败2659----------
const  LONG* FailureInf2659[] = {
    {NULL}
};

//1号线失压启动自投2660----------
const  LONG* FailureInf2660[] = {
    {NULL}
};

//2号线失压启动自投2661----------
const  LONG* FailureInf2661[] = {
    {NULL}
};

//线路PT断线恢复2662----------
const  LONG* FailureInf2662[] = {
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
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//母线PT断线恢复2663----------
const  LONG* FailureInf2663[] = {
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

//线路PT断线2664----------
const  LONG* FailureInf2664[] = {
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
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//母线PT断线2665----------
const  LONG* FailureInf2665[] = {
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

//线路接地2666----------
const  LONG* FailureInf2666[] = {
    {NULL}
};

//母线接地2667----------
const  LONG* FailureInf2667[] = {
    {NULL}
};

//零压告警2668----------
const  LONG* FailureInf2668[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//同期操作2669----------
const  LONG* FailureInf2669[] = {
    {NULL}
};

//同期失败2670----------
const  LONG* FailureInf2670[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 152), // F1
    (LONG*)(RM_BUF_ADDR + 156), // F2
    (LONG*)(RM_BUF_ADDR + 160), // ΔU
    (LONG*)(RM_BUF_ADDR + 164) // ΔΦ
};

//同期成功2671----------
const  LONG* FailureInf2671[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 152), // F1
    (LONG*)(RM_BUF_ADDR + 156), // F2
    (LONG*)(RM_BUF_ADDR + 160), // ΔU
    (LONG*)(RM_BUF_ADDR + 164) // ΔΦ
};

//无压空合2672----------
const  LONG* FailureInf2672[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 152), // F1
    (LONG*)(RM_BUF_ADDR + 156), // F2
    (LONG*)(RM_BUF_ADDR + 160), // ΔU
    (LONG*)(RM_BUF_ADDR + 164) // ΔΦ
};

//同期被闭锁2673----------
const  LONG* FailureInf2673[] = {
    {NULL}
};

//同期被中断2674----------
const  LONG* FailureInf2674[] = {
    {NULL}
};

//重合闸失败2675----------
const  LONG* FailureInf2675[] = {
    {NULL}
};

//偷跳重合闸2676----------
const  LONG* FailureInf2676[] = {
    {NULL}
};

//失灵保护2677----------
const  LONG* FailureInf2677[] = {
    {NULL}
};

//A相CT断线2678----------
const  LONG* FailureInf2678[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//A相CT断线恢复2679----------
const  LONG* FailureInf2679[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//B相CT断线2680----------
const  LONG* FailureInf2680[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//B相CT断线恢复2681----------
const  LONG* FailureInf2681[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//C相CT断线2682----------
const  LONG* FailureInf2682[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//C相CT断线恢复2683----------
const  LONG* FailureInf2683[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
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

//二次重合闸被闭锁2684----------
const  LONG* FailureInf2684[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//一次重合闸成功2685----------
const  LONG* FailureInf2685[] = {
    {NULL}
};

//二次重合闸成功2686----------
const  LONG* FailureInf2686[] = {
    {NULL}
};

//一次重合闸失败2687----------
const  LONG* FailureInf2687[] = {
    {NULL}
};

//二次重合闸失败2688----------
const  LONG* FailureInf2688[] = {
    {NULL}
};

//一次重合闸启动2689----------
const  LONG* FailureInf2689[] = {
    {NULL}
};

//二次重合闸启动2690----------
const  LONG* FailureInf2690[] = {
    {NULL}
};

//一次重合闸未启动2691----------
const  LONG* FailureInf2691[] = {
    {NULL}
};

//二次重合闸未启动2692----------
const  LONG* FailureInf2692[] = {
    {NULL}
};

//过流Ⅱ段加速启动2693----------
const  LONG* FailureInf2693[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅱ段加速返回2694----------
const  LONG* FailureInf2694[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅲ段加速启动2695----------
const  LONG* FailureInf2695[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅲ段加速返回2696----------
const  LONG* FailureInf2696[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//保护数据异常22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//保护数据异常恢复65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//电流速断启动2697----------
const  LONG* FailureInf2697[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//电流速断动作2698----------
const  LONG* FailureInf2698[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//电流速断返回2699----------
const  LONG* FailureInf2699[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过负荷告警2700----------
const  LONG* FailureInf2700[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120) // ΦIp1
};

//过流Ⅰ段加速启动2701----------
const  LONG* FailureInf2701[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//过流Ⅰ段加速动作2702----------
const  LONG* FailureInf2702[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//过流Ⅰ段加速返回2703----------
const  LONG* FailureInf2703[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//母线充电保护启动2704----------
const  LONG* FailureInf2704[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//母线充电保护动作2705----------
const  LONG* FailureInf2705[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ΦUa
    (LONG*)(RM_BUF_ADDR + 76), // ΦUb
    (LONG*)(RM_BUF_ADDR + 80), // ΦUc
    (LONG*)(RM_BUF_ADDR + 84), // ΦUab
    (LONG*)(RM_BUF_ADDR + 88), // ΦUbc
    (LONG*)(RM_BUF_ADDR + 92), // ΦUca
    (LONG*)(RM_BUF_ADDR + 96), // ΦUl1
    (LONG*)(RM_BUF_ADDR + 100), // ΦUl2
    (LONG*)(RM_BUF_ADDR + 104), // ΦUl3
    (LONG*)(RM_BUF_ADDR + 108), // ΦIpa
    (LONG*)(RM_BUF_ADDR + 112), // ΦIpb
    (LONG*)(RM_BUF_ADDR + 116), // ΦIpc
    (LONG*)(RM_BUF_ADDR + 120), // ΦIp1
    (LONG*)(RM_BUF_ADDR + 136), // ΦU01
    (LONG*)(RM_BUF_ADDR + 140), // ΦI01
    (LONG*)(RM_BUF_ADDR + 144), // ΦU05
    (LONG*)(RM_BUF_ADDR + 148) // ΦI05
};

//母线充电保护返回2706----------
const  LONG* FailureInf2706[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
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
    {33, FailureInf2600, 2600},//通道测试
    {33, FailureInf2601, 2601},//保护启动
    {9, FailureInf2602, 2602},//过流Ⅰ段启动
    {35, FailureInf2603, 2603},//过流Ⅰ段动作
    {9, FailureInf2604, 2604},//过流Ⅰ段返回
    {9, FailureInf2605, 2605},//过流Ⅱ段启动
    {35, FailureInf2606, 2606},//过流Ⅱ段动作
    {9, FailureInf2607, 2607},//过流Ⅱ段返回
    {9, FailureInf2608, 2608},//过流Ⅲ段启动
    {35, FailureInf2609, 2609},//过流Ⅲ段动作
    {9, FailureInf2610, 2610},//过流Ⅲ段返回
    {35, FailureInf2611, 2611},//过流Ⅱ段加速动作
    {35, FailureInf2612, 2612},//过流Ⅲ段加速动作
    {2, FailureInf2613, 2613},//零序过流Ⅰ段启动
    {35, FailureInf2614, 2614},//零序过流Ⅰ段动作
    {2, FailureInf2615, 2615},//零序过流Ⅰ段返回
    {2, FailureInf2616, 2616},//零序过流Ⅱ段启动
    {35, FailureInf2617, 2617},//零序过流Ⅱ段动作
    {2, FailureInf2618, 2618},//零序过流Ⅱ段返回
    {35, FailureInf2619, 2619},//低压保护跳闸
    {6, FailureInf2620, 2620},//断路器跳闸失败
    {0, FailureInf2621, 2621},//一次重合闸出口
    {0, FailureInf2622, 2622},//二次重合闸出口
    {10, FailureInf2623, 2623},//一次重合闸被闭锁
    {0, FailureInf2624, 2624},//备自投启动
    {0, FailureInf2625, 2625},//备自投退出
    {0, FailureInf2626, 2626},//备自投成功
    {0, FailureInf2627, 2627},//备自投失败
    {0, FailureInf2628, 2628},//跳1断路器
    {0, FailureInf2629, 2629},//合1断路器
    {0, FailureInf2630, 2630},//跳1断路器失败
    {0, FailureInf2631, 2631},//合1断路器失败
    {0, FailureInf2632, 2632},//跳2断路器
    {0, FailureInf2633, 2633},//合2断路器
    {0, FailureInf2634, 2634},//跳2断路器失败
    {0, FailureInf2635, 2635},//合2断路器失败
    {0, FailureInf2636, 2636},//跳3断路器
    {0, FailureInf2637, 2637},//合3断路器
    {0, FailureInf2638, 2638},//跳3断路器失败
    {0, FailureInf2639, 2639},//合3断路器失败
    {0, FailureInf2640, 2640},//分4断路器
    {0, FailureInf2641, 2641},//合4断路器
    {0, FailureInf2642, 2642},//分4断路器失败
    {0, FailureInf2643, 2643},//合4断路器失败
    {0, FailureInf2644, 2644},//跳1隔开
    {0, FailureInf2645, 2645},//合1隔开
    {0, FailureInf2646, 2646},//跳1隔开失败
    {0, FailureInf2647, 2647},//合1隔开失败
    {0, FailureInf2648, 2648},//跳2隔开
    {0, FailureInf2649, 2649},//合2隔开
    {0, FailureInf2650, 2650},//跳2隔开失败
    {0, FailureInf2651, 2651},//合2隔开失败
    {0, FailureInf2652, 2652},//跳3隔开
    {0, FailureInf2653, 2653},//合3隔开
    {0, FailureInf2654, 2654},//分3隔开失败
    {0, FailureInf2655, 2655},//合3隔开失败
    {0, FailureInf2656, 2656},//分4隔开
    {0, FailureInf2657, 2657},//合4隔开
    {0, FailureInf2658, 2658},//分4隔开失败
    {0, FailureInf2659, 2659},//合4隔开失败
    {0, FailureInf2660, 2660},//1号线失压启动自投
    {0, FailureInf2661, 2661},//2号线失压启动自投
    {13, FailureInf2662, 2662},//线路PT断线恢复
    {14, FailureInf2663, 2663},//母线PT断线恢复
    {13, FailureInf2664, 2664},//线路PT断线
    {14, FailureInf2665, 2665},//母线PT断线
    {0, FailureInf2666, 2666},//线路接地
    {0, FailureInf2667, 2667},//母线接地
    {10, FailureInf2668, 2668},//零压告警
    {0, FailureInf2669, 2669},//同期操作
    {22, FailureInf2670, 2670},//同期失败
    {22, FailureInf2671, 2671},//同期成功
    {22, FailureInf2672, 2672},//无压空合
    {0, FailureInf2673, 2673},//同期被闭锁
    {0, FailureInf2674, 2674},//同期被中断
    {0, FailureInf2675, 2675},//重合闸失败
    {0, FailureInf2676, 2676},//偷跳重合闸
    {0, FailureInf2677, 2677},//失灵保护
    {6, FailureInf2678, 2678},//A相CT断线
    {6, FailureInf2679, 2679},//A相CT断线恢复
    {6, FailureInf2680, 2680},//B相CT断线
    {6, FailureInf2681, 2681},//B相CT断线恢复
    {6, FailureInf2682, 2682},//C相CT断线
    {6, FailureInf2683, 2683},//C相CT断线恢复
    {0, FailureInf20, 20},//5V电源故障
    {0, FailureInf21, 21},//EEPROM写入故障
    {0, FailureInf64, 64},//5V电源故障恢复
    {10, FailureInf2684, 2684},//二次重合闸被闭锁
    {0, FailureInf2685, 2685},//一次重合闸成功
    {0, FailureInf2686, 2686},//二次重合闸成功
    {0, FailureInf2687, 2687},//一次重合闸失败
    {0, FailureInf2688, 2688},//二次重合闸失败
    {0, FailureInf2689, 2689},//一次重合闸启动
    {0, FailureInf2690, 2690},//二次重合闸启动
    {0, FailureInf2691, 2691},//一次重合闸未启动
    {0, FailureInf2692, 2692},//二次重合闸未启动
    {9, FailureInf2693, 2693},//过流Ⅱ段加速启动
    {9, FailureInf2694, 2694},//过流Ⅱ段加速返回
    {9, FailureInf2695, 2695},//过流Ⅲ段加速启动
    {9, FailureInf2696, 2696},//过流Ⅲ段加速返回
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
    {9, FailureInf2697, 2697},//电流速断启动
    {35, FailureInf2698, 2698},//电流速断动作
    {9, FailureInf2699, 2699},//电流速断返回
    {31, FailureInf2700, 2700},//过负荷告警
    {9, FailureInf2701, 2701},//过流Ⅰ段加速启动
    {35, FailureInf2702, 2702},//过流Ⅰ段加速动作
    {9, FailureInf2703, 2703},//过流Ⅰ段加速返回
    {9, FailureInf2704, 2704},//母线充电保护启动
    {35, FailureInf2705, 2705},//母线充电保护动作
    {9, FailureInf2706, 2706},//母线充电保护返回
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
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
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 168) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 104) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
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
	 (BYTE*)(SW_BUF_ADDR + 10) ,
	 (BYTE*)(SW_BUF_ADDR + 11) ,
	 (BYTE*)(SW_BUF_ADDR + 12) ,
	 (BYTE*)(SW_BUF_ADDR + 13) ,
	 (BYTE*)(SW_BUF_ADDR + 14) ,
	 (BYTE*)(SW_BUF_ADDR + 15) ,
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
	 (WORD)(14) ,
	 (WORD)(15) ,
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
