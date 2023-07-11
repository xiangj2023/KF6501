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
	{WCTH_0, "不校正"},
	{WCTH_1, "30°校正"},
	{WCTH_2, "60°校正"},
	{WCTH_3, "90°校正"},
	{WCTH_4, "120°校正"},
	{WCTH_5, "150°校正"},
	{WCTH_6, "180°校正"},
	{WCTH_7, "210°校正"},
	{WCTH_8, "240°校正"},
	{WCTH_9, "270°校正"},
	{WCTH_10, "300°校正"},
	{WCTH_11, "330°校正"},
	{WCTH_12, "360°校正"},
	{WCTM_0, "不校正"},
	{WCTM_1, "30°校正"},
	{WCTM_2, "60°校正"},
	{WCTM_3, "90°校正"},
	{WCTM_4, "120°校正"},
	{WCTM_5, "150°校正"},
	{WCTM_6, "180°校正"},
	{WCTM_7, "210°校正"},
	{WCTM_8, "240°校正"},
	{WCTM_9, "270°校正"},
	{WCTM_10, "300°校正"},
	{WCTM_11, "330°校正"},
	{WCTM_12, "360°校正"},
	{WCTL_0, "不校正"},
	{WCTL_1, "30°校正"},
	{WCTL_2, "60°校正"},
	{WCTL_3, "90°校正"},
	{WCTL_4, "120°校正"},
	{WCTL_5, "150°校正"},
	{WCTL_6, "180°校正"},
	{WCTL_7, "210°校正"},
	{WCTL_8, "240°校正"},
	{WCTL_9, "270°校正"},
	{WCTL_10, "300°校正"},
	{WCTL_11, "330°校正"},
	{WCTL_12, "360°校正"},
	{WCTH_0, "星/星型"},
	{WCTH_1, "星/角型"},
	{WCTH_2, "角/角型"},
	{WCTM_0, "星/星型"},
	{WCTM_1, "星/角型"},
	{WCTM_2, "角/角型"},
	{WCTL_0, "星/星型"},
	{WCTL_1, "星/角型"},
	{WCTL_2, "角/角型"},
	{MBPH_0, "A相"},
	{MBPH_1, "B相"},
	{MBPH_2, "C相"},
	{LBPH_0, "A相"},
	{LBPH_1, "B相"},
	{LBPH_2, "C相"},
	{-1, NULL},
};
const TSetValueInfMap SetValueInfMap[]=
{
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{13, (TSetValueInf*)&(SetValueInf[0])}, //定值高压侧电流角度校正
	{13, (TSetValueInf*)&(SetValueInf[13])}, //定值中压侧电流角度校正
	{13, (TSetValueInf*)&(SetValueInf[26])}, //定值低压侧电流角度校正
	{3, (TSetValueInf*)&(SetValueInf[39])}, //定值高侧绕组/CT连接方式
	{3, (TSetValueInf*)&(SetValueInf[42])}, //定值中侧绕组/CT连接方式
	{3, (TSetValueInf*)&(SetValueInf[45])}, //定值低侧绕组/CT连接方式
	{3, (TSetValueInf*)&(SetValueInf[48])}, //定值中压侧公共相
	{3, (TSetValueInf*)&(SetValueInf[51])}, //定值低压侧公共相
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
	{0, NULL},
	{0, NULL},
};

//保护测量值定义-----------------------
LONG* 	RM_pnUA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// UA
LONG* 	RM_pnUB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// UB
LONG* 	RM_pnUC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// UC
LONG* 	RM_pnUAB 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// UAB
LONG* 	RM_pnUBC 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// UBC
LONG* 	RM_pnUCA 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// UCA
LONG* 	RM_pnU2H 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// U2H
LONG* 	RM_pnU0 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// U0
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// IHA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// IHB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// IHC
LONG* 	RM_pnIMA 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// IMA
LONG* 	RM_pnIMB 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IMB
LONG* 	RM_pnIMC 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// IMC
LONG* 	RM_pnILA 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// ILA
LONG* 	RM_pnILB 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// ILB
LONG* 	RM_pnILC 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// ILC
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// I0
LONG* 	RM_pnIjx 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// Ijx
LONG* 	RM_pnIDA 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// IDA
LONG* 	RM_pnIDB 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IDB
LONG* 	RM_pnIDC 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// IDC
LONG* 	RM_pnIRA 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// IRA
LONG* 	RM_pnIRB 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// IRB
LONG* 	RM_pnIRC 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// IRC
LONG* 	RM_pnIDA2 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// IDA2
LONG* 	RM_pnIDA3 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// IDA3
LONG* 	RM_pnIDA5 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// IDA5
LONG* 	RM_pnIDB2 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// IDB2
LONG* 	RM_pnIDB3 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// IDB3
LONG* 	RM_pnIDB5 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// IDB5
LONG* 	RM_pnIDC2 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// IDC2
LONG* 	RM_pnIDC3 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// IDC3
LONG* 	RM_pnIDC5 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// IDC5
LONG* 	RM_pnIRA2 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// IRA2
LONG* 	RM_pnIRB2 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// IRB2
LONG* 	RM_pnIRC2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// IRC2
LONG* 	RM_pnAUA 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// ΦUA
LONG* 	RM_pnAUB 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// ΦUB
LONG* 	RM_pnAUC 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// ΦUC
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// ΦIHA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// ΦIHB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// ΦIHC
LONG* 	RM_pnAIMA 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// ΦIMA
LONG* 	RM_pnAIMB 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// ΦIMB
LONG* 	RM_pnAIMC 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// ΦIMC
LONG* 	RM_pnAILA 	= 	(LONG*)(RM_BUF_ADDR + 184); 	// ΦILA
LONG* 	RM_pnAILB 	= 	(LONG*)(RM_BUF_ADDR + 188); 	// ΦILB
LONG* 	RM_pnAILC 	= 	(LONG*)(RM_BUF_ADDR + 192); 	// ΦILC
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 196); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 200); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 204); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 208); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnIHAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//IHAM IHAM
LONG* 	M_pnIHBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//IHBM IHBM
LONG* 	M_pnIHCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//IHCM IHCM
LONG* 	M_pnIMAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IMAM IMAM
LONG* 	M_pnIMBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IMBM IMBM
LONG* 	M_pnIMCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IMCM IMCM
LONG* 	M_pnILAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//ILAM ILAM
LONG* 	M_pnILBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//ILBM ILBM
LONG* 	M_pnILCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//ILCM ILCM
LONG* 	M_pnI0M 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I0M I0M
LONG* 	M_pnIjxM 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//IjxM IjxM


//软压板----------------------------------
BYTE* SW_pby87U 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//差动电流速断
BYTE* SW_pby87R 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//比率差动保护
BYTE* SW_pby50I_H 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//高压侧过流保护
BYTE* SW_pby50I_M 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//中压侧过流保护
BYTE* SW_pby50I_L 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//低压侧过流保护
BYTE* SW_pby50OL 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//过负荷保护
BYTE* SW_pby27U 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//失压保护
BYTE* SW_pby59UN 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//零序过压保护
BYTE* SW_pby50IN 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//零序过流保护
BYTE* SW_pby50IJX 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//间隙过流保护
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby87U_A 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//A相差流速断保护
BYTE* CFG_pby87U_B 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//B相差流速断保护
BYTE* CFG_pby87U_C 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//C相差流速断保护
BYTE* CFG_pby87R_A 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//A相比率差动保护
BYTE* CFG_pby87R_B 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//B相比率差动保护
BYTE* CFG_pby87R_C 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//C相比率差动保护
BYTE* CFG_pbyDCRES_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//差动非周期分量抑制
BYTE* CFG_pby68IdH2M_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//差流二次最大相闭锁
BYTE* CFG_pby68IdH2C_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//差流二次综合相闭锁
BYTE* CFG_pby68IdH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//差流二次分相闭锁
BYTE* CFG_pby68IrH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//制动电流二次闭锁
BYTE* CFG_pby68IdH5I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//差流五次谐波闭锁
BYTE* CFG_pby68IdHMUL_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//差流综合谐波闭锁
BYTE* CFG_pbyMAB 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//中压侧两相
BYTE* CFG_pbyLAB 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//低压侧两相
BYTE* CFG_pby50I_H1 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//高压侧过流Ⅰ段
BYTE* CFG_pby50I_H1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//高过流Ⅰ段功率方向
BYTE* CFG_pby50I_H1_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//高过流Ⅰ段复压闭锁
BYTE* CFG_pby50I_H2 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//高压侧过流Ⅱ段
BYTE* CFG_pby50I_H2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//高过流Ⅱ段功率方向
BYTE* CFG_pby50I_H2_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//高过流Ⅱ段复压闭锁
BYTE* CFG_pby50I_M1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//中压侧过流Ⅰ段
BYTE* CFG_pby50I_M2 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//中压侧过流Ⅱ段
BYTE* CFG_pby50I_L1 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//低压侧过流Ⅰ段
BYTE* CFG_pby50I_L2 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//低压侧过流Ⅱ段
BYTE* CFG_pby50OL_1 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//过负荷Ⅰ段
BYTE* CFG_pby50OL_2 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//过负荷Ⅱ段
BYTE* CFG_pby50OL_3 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//过负荷Ⅲ段
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//失压保护
BYTE* CFG_pby27U_50I 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//失压保护电流闭锁
BYTE* CFG_pby59UN 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//零序过压保护
BYTE* CFG_pby50IN 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//零序过流保护
BYTE* CFG_pby50IJX 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//间隙过流保护
BYTE* CFG_pby74VT 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//PT断线
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//备用配置2

//定值----------------------------------
LONG* RS_pnCFG1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字1 CFG1
LONG* RS_pnCFG2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//配置字2 CFG2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//突变量启动电流 PIC
LONG* RS_pnWHCTC 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//高压侧电流角度校正 WHCTC
LONG* RS_pnWMCTC 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//中压侧电流角度校正 WMCTC
LONG* RS_pnWLCTC 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//低压侧电流角度校正 WLCTC
LONG* RS_pnWCTHCON 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//高侧绕组/CT连接方式 WCTHCON
LONG* RS_pnWCTMCON 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//中侧绕组/CT连接方式 WCTMCON
LONG* RS_pnWCTLCON 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//低侧绕组/CT连接方式 WCTLCON
LONG* RS_pnMBph 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//中压侧公共相 MBph
LONG* RS_pnLBph 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//低压侧公共相 LBph
LONG* RS_pn87UIA 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//A相差动速断定值 87UIA
LONG* RS_pn87RIA 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//A相差动电流定值 87RIA
LONG* RS_pnIRS1A 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//A相制动拐点1 IRS1A
LONG* RS_pnIRS2A 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//A相制动拐点2 IRS2A
LONG* RS_pn87UIB 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//B相差动速断定值 87UIB
LONG* RS_pn87RIB 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//B相差动电流定值 87RIB
LONG* RS_pnIRS1B 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//B相制动拐点1 IRS1B
LONG* RS_pnIRS2B 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//B相制动拐点2 IRS2B
LONG* RS_pn87UIC 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//C相差动速断定值 87UIC
LONG* RS_pn87RIC 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//C相差动电流定值 87RIC
LONG* RS_pnIRS1C 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//C相制动拐点1 IRS1C
LONG* RS_pnIRS2C 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//C相制动拐点2 IRS2C
LONG* RS_pnSLP1 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//比率制动系数1 SLP1
LONG* RS_pnSLP2 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//比率制动系数2 SLP2
LONG* RS_pnKrh2 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//二次谐波制动系数 Krh2
LONG* RS_pnKrh5 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//五次谐波制动系数 Krh5
LONG* RS_pnKrhm 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//综合谐波制动系数 Krhm
LONG* RS_pnKrav 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//差动非周期加权系数 Krav
LONG* RS_pnK2av 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//二次非周期加权系数 K2av
LONG* RS_pnKma 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//中压侧A相平衡系数 Kma
LONG* RS_pnKmb 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//中压侧B相平衡系数 Kmb
LONG* RS_pnKmc 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//中压侧C相平衡系数 Kmc
LONG* RS_pnKla 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//低压侧A相平衡系数 Kla
LONG* RS_pnKlb 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//低压侧B相平衡系数 Klb
LONG* RS_pnKlc 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//低压侧C相平衡系数 Klc
LONG* RS_pnK87AR 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//差流越限报警比率 K87AR
LONG* RS_pn50IH1C 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//高压侧过流Ⅰ段电流 50IH1C
LONG* RS_pn50IH1D 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//高压侧过流Ⅰ段时间 50IH1D
LONG* RS_pn50IH2C 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//高压侧过流Ⅱ段电流 50IH2C
LONG* RS_pn50IH2D 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//高压侧过流Ⅱ段时间 50IH2D
LONG* RS_pn47U_27U1 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//高过流闭锁低电压 47U_27U1
LONG* RS_pn47U_59U2 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//高过流闭锁负序电压 47U_59U2
LONG* RS_pnA_50IH 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//高压侧过流灵敏角 A_50IH
LONG* RS_pn50IM1C 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//中压侧过流Ⅰ段电流 50IM1C
LONG* RS_pn50IM1D 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//中压侧过流Ⅰ段时间 50IM1D
LONG* RS_pn50IM2C 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//中压侧过流Ⅱ段电流 50IM2C
LONG* RS_pn50IM2D 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//中压侧过流Ⅱ段时间 50IM2D
LONG* RS_pn50IL1C 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//低压侧过流Ⅰ段电流 50IL1C
LONG* RS_pn50IL1D 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//低压侧过流Ⅰ段时间 50IL1D
LONG* RS_pn50IL2C 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//低压侧过流Ⅱ段电流 50IL2C
LONG* RS_pn50IL2D 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//低压侧过流Ⅱ段时间 50IL2D
LONG* RS_pn50OL1C 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//过负荷Ⅰ段电流 50OL1C
LONG* RS_pn50OL1D 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//过负荷Ⅰ段时间 50OL1D
LONG* RS_pn50OL2C 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//过负荷Ⅱ段电流 50OL2C
LONG* RS_pn50OL2D 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//过负荷Ⅱ段时间 50OL2D
LONG* RS_pn50OL3C 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//过负荷Ⅲ段电流 50OL3C
LONG* RS_pn50OL3D 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//过负荷Ⅲ段时间 50OL3D
LONG* RS_pn27UV 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//失压保护相电压 27UV
LONG* RS_pn27UD 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//失压保护时间 27UD
LONG* RS_pn27U_50I 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//失压保护电流闭锁 27U_50I
LONG* RS_pn59UNV 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//零序过压电压定值 59UNV
LONG* RS_pn59UND 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//零序过压时间定值 59UND
LONG* RS_pn50INC 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//零序过流电流定值 50INC
LONG* RS_pn50IND 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//零序过流时间定值 50IND
LONG* RS_pn50IJXC 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//间隙过流电流定值 50IJXC
LONG* RS_pn50IJXD 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//间隙过流时间定值 50IJXD
LONG* RS_pn74VTV 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//PT断线检测电压 74VTV
LONG* RS_pn74VTC 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//PT断线检测电流 74VTC
LONG* RS_pn74VTD 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//PT断线检测时间 74VTD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//失灵保护时间 52BFD

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
LONG* AS_pn59UA 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//UA有压定值
LONG* AS_pn27UA 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//UA无压定值
LONG* AS_pn59UB 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//UB有压定值
LONG* AS_pn27UB 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//UB无压定值
LONG* AS_pn59UC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//UC有压定值
LONG* AS_pn27UC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//UC无压定值
LONG* AS_pn59UN 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//UN有压定值
LONG* AS_pn27UN 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//UN无压定值
LONG* AS_pn50HAC 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//IHA有流定值
LONG* AS_pn37HAC 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//IHA无流定值
LONG* AS_pn50HBC 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//IHB有流定值
LONG* AS_pn37HBC 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//IHB无流定值
LONG* AS_pn50HCC 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//IHC有流定值
LONG* AS_pn37HCC 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//IHC无流定值
LONG* AS_pn50MAC 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//IMA有流定值
LONG* AS_pn37MAC 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//IMA无流定值
LONG* AS_pn50MBC 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//IMB有流定值
LONG* AS_pn37MBC 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//IMB无流定值
LONG* AS_pn50MCC 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//IMC有流定值
LONG* AS_pn37MCC 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//IMC无流定值
LONG* AS_pn50LAC 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//ILA有流定值
LONG* AS_pn37LAC 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//ILA无流定值
LONG* AS_pn50LBC 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//ILB有流定值
LONG* AS_pn37LBC 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//ILB无流定值
LONG* AS_pn50LCC 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//ILC有流定值
LONG* AS_pn37LCC 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//ILC无流定值
LONG* AS_pn50INC 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//IN有流定值
LONG* AS_pn37INC 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//IN无流定值
LONG* AS_pn50IJXC 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//IJX有流定值
LONG* AS_pn37IJXC 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//IJX无流定值
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//断路器动作延时
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//隔开动作延时
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//TR2保持时间
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//TR3保持时间
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnHPT 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//高压侧PT变比
LONG* AS_pnNPT 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//零序PT变比
LONG* AS_pnHACT 	= 	(LONG*)(AS_BUF_ADDR + 284); 	//高压侧A相CT变比
LONG* AS_pnHBCT 	= 	(LONG*)(AS_BUF_ADDR + 288); 	//高压侧B相CT变比
LONG* AS_pnHCCT 	= 	(LONG*)(AS_BUF_ADDR + 292); 	//高压侧C相CT变比
LONG* AS_pnMACT 	= 	(LONG*)(AS_BUF_ADDR + 296); 	//中压测A相CT变比
LONG* AS_pnMBCT 	= 	(LONG*)(AS_BUF_ADDR + 300); 	//中压测B相CT变比
LONG* AS_pnMCCT 	= 	(LONG*)(AS_BUF_ADDR + 304); 	//中压测C相CT变比
LONG* AS_pnLACT 	= 	(LONG*)(AS_BUF_ADDR + 308); 	//低压侧A相CT变比
LONG* AS_pnLBCT 	= 	(LONG*)(AS_BUF_ADDR + 312); 	//低压侧B相CT变比
LONG* AS_pnLCCT 	= 	(LONG*)(AS_BUF_ADDR + 316); 	//低压侧C相CT变比
LONG* AS_pnNCT 	= 	(LONG*)(AS_BUF_ADDR + 320); 	//零序CT变比
LONG* AS_pnJXCT 	= 	(LONG*)(AS_BUF_ADDR + 324); 	//间隙CT变比
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 328); 	//固定变比

//系数----------------------------------
LONG* CF_pnUp 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//Up
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U0
LONG* CF_pnIp 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//Ip
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//I0
LONG* CF_pnIjx 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//Ijx
LONG* CF_pnUAM 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UAM
LONG* CF_pnUBM 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//UBM
LONG* CF_pnUCM 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UCM
LONG* CF_pnU0M 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//U0M
LONG* CF_pnIHAM 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IHAM
LONG* CF_pnIHBM 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IHBM
LONG* CF_pnIHCM 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IHCM
LONG* CF_pnIMAM 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IMAM
LONG* CF_pnIMBM 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IMBM
LONG* CF_pnIMCM 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//IMCM
LONG* CF_pnILAM 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//ILAM
LONG* CF_pnILBM 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//ILBM
LONG* CF_pnILCM 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//ILCM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//I0M
LONG* CF_pnIjxM 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//IjxM
LONG* CF_pnUAD 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//UAD
LONG* CF_pnUBD 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//UBD
LONG* CF_pnUCD 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//UCD
LONG* CF_pnU0D 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//U0D
LONG* CF_pnIHAD 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//IHAD
LONG* CF_pnIHBD 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//IHBD
LONG* CF_pnIHCD 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//IHCD
LONG* CF_pnIMAD 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//IMAD
LONG* CF_pnIMBD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//IMBD
LONG* CF_pnIMCD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//IMCD
LONG* CF_pnILAD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//ILAD
LONG* CF_pnILBD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//ILBD
LONG* CF_pnILCD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//ILCD
LONG* CF_pnI0D 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//I0D
LONG* CF_pnIjxD 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//IjxD
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//C4

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 204), // IN1
    (LONG*)(RM_BUF_ADDR + 208) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 196), // BR1
    (LONG*)(RM_BUF_ADDR + 200) // BR2
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

//保护数据异常22----------
const  LONG* FailureInf22[] = {
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

//逻辑方程故障故障恢复63----------
const  LONG* FailureInf63[] = {
    {NULL}
};

//5V电源故障恢复64----------
const  LONG* FailureInf64[] = {
    {NULL}
};

//保护数据异常恢复65----------
const  LONG* FailureInf65[] = {
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

//温度过高506----------
const  LONG* FailureInf506[] = {
    {NULL}
};

//差流越限4100----------
const  LONG* FailureInf4100[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//A相差动速断启动4101----------
const  LONG* FailureInf4101[] = {
    {NULL}
};

//A相差流速断动作4102----------
const  LONG* FailureInf4102[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//A相差动速断返回4103----------
const  LONG* FailureInf4103[] = {
    {NULL}
};

//B相差动速断启动4104----------
const  LONG* FailureInf4104[] = {
    {NULL}
};

//B相差动速断动作4105----------
const  LONG* FailureInf4105[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//B相差动速断返回4106----------
const  LONG* FailureInf4106[] = {
    {NULL}
};

//C相差动速断启动4107----------
const  LONG* FailureInf4107[] = {
    {NULL}
};

//C相差动速断动作4108----------
const  LONG* FailureInf4108[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//C相差动速断返回4109----------
const  LONG* FailureInf4109[] = {
    {NULL}
};

//A相比率差动启动4110----------
const  LONG* FailureInf4110[] = {
    {NULL}
};

//A相比率差动动作4111----------
const  LONG* FailureInf4111[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//A相比率差动返回4112----------
const  LONG* FailureInf4112[] = {
    {NULL}
};

//B相比率差动启动4113----------
const  LONG* FailureInf4113[] = {
    {NULL}
};

//B相比率差动动作4114----------
const  LONG* FailureInf4114[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//B相比率差动返回4115----------
const  LONG* FailureInf4115[] = {
    {NULL}
};

//C相比率差动启动4116----------
const  LONG* FailureInf4116[] = {
    {NULL}
};

//C相比率差动动作4117----------
const  LONG* FailureInf4117[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//C相比率差动返回4118----------
const  LONG* FailureInf4118[] = {
    {NULL}
};

//高压侧过流Ⅰ段启动4119----------
const  LONG* FailureInf4119[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//高压侧过流Ⅰ段动作4120----------
const  LONG* FailureInf4120[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ΦILC
};

//高压侧过流Ⅰ段返回4121----------
const  LONG* FailureInf4121[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//高压侧过流Ⅱ段启动4122----------
const  LONG* FailureInf4122[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//高压侧过流Ⅱ段动作4123----------
const  LONG* FailureInf4123[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ΦILC
};

//高压侧过流Ⅱ段返回4124----------
const  LONG* FailureInf4124[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//中压侧过流Ⅰ段启动4125----------
const  LONG* FailureInf4125[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//中压侧过流Ⅰ段动作4126----------
const  LONG* FailureInf4126[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ΦUC
};

//中压侧过流Ⅰ段返回4127----------
const  LONG* FailureInf4127[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//中压侧过流Ⅱ段启动4128----------
const  LONG* FailureInf4128[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//中压侧过流Ⅱ段动作4129----------
const  LONG* FailureInf4129[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ΦUC
};

//中压侧过流Ⅱ段返回4130----------
const  LONG* FailureInf4130[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//低压侧过流Ⅰ段启动4131----------
const  LONG* FailureInf4131[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//低压侧过流Ⅰ段动作4132----------
const  LONG* FailureInf4132[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ΦUC
};

//低压侧过流Ⅰ段返回4133----------
const  LONG* FailureInf4133[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//低压侧过流Ⅱ段启动4134----------
const  LONG* FailureInf4134[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//低压侧过流Ⅱ段动作4135----------
const  LONG* FailureInf4135[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ΦILC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ΦUC
};

//低压侧过流Ⅱ段返回4136----------
const  LONG* FailureInf4136[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//过负荷Ⅰ段启动4137----------
const  LONG* FailureInf4137[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//过负荷Ⅰ段动作4138----------
const  LONG* FailureInf4138[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ΦILC
};

//过负荷Ⅰ段返回4139----------
const  LONG* FailureInf4139[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//过负荷Ⅱ段启动4140----------
const  LONG* FailureInf4140[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//过负荷Ⅱ段动作4141----------
const  LONG* FailureInf4141[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ΦILC
};

//过负荷Ⅱ段返回4142----------
const  LONG* FailureInf4142[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//过负荷Ⅲ段启动4143----------
const  LONG* FailureInf4143[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//过负荷Ⅲ段动作4144----------
const  LONG* FailureInf4144[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ΦILC
};

//过负荷Ⅲ段返回4145----------
const  LONG* FailureInf4145[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//失压保护动作4146----------
const  LONG* FailureInf4146[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168) // ΦIHC
};

//高压侧PT断线4147----------
const  LONG* FailureInf4147[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ΦUA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ΦUB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ΦUC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24) // U2H
};

//通道测试4148----------
const  LONG* FailureInf4148[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144), // IRC2
    (LONG*)(RM_BUF_ADDR + 160), // ΦIHA
    (LONG*)(RM_BUF_ADDR + 164), // ΦIHB
    (LONG*)(RM_BUF_ADDR + 168), // ΦIHC
    (LONG*)(RM_BUF_ADDR + 172), // ΦIMA
    (LONG*)(RM_BUF_ADDR + 176), // ΦIMB
    (LONG*)(RM_BUF_ADDR + 180), // ΦIMC
    (LONG*)(RM_BUF_ADDR + 184), // ΦILA
    (LONG*)(RM_BUF_ADDR + 188), // ΦILB
    (LONG*)(RM_BUF_ADDR + 192) // ΦILC
};

//保护启动4149----------
const  LONG* FailureInf4149[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//失灵保护4150----------
const  LONG* FailureInf4150[] = {
    {NULL}
};

//高侧断路器跳闸失败4151----------
const  LONG* FailureInf4151[] = {
    {NULL}
};

//中侧断路器跳闸失败4152----------
const  LONG* FailureInf4152[] = {
    {NULL}
};

//低侧断路器跳闸失败4153----------
const  LONG* FailureInf4153[] = {
    {NULL}
};

//零序过压保护启动4154----------
const  LONG* FailureInf4154[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 28) // U0
};

//零序过压保护动作4155----------
const  LONG* FailureInf4155[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 28) // U0
};

//零序过压保护返回4156----------
const  LONG* FailureInf4156[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 28) // U0
};

//零序过流保护启动4157----------
const  LONG* FailureInf4157[] = {
    (LONG*)(RM_BUF_ADDR + 68) // I0
};

//零序过流保护动作4158----------
const  LONG* FailureInf4158[] = {
    (LONG*)(RM_BUF_ADDR + 28), // U0
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 68) // I0
};

//零序过流保护返回4159----------
const  LONG* FailureInf4159[] = {
    (LONG*)(RM_BUF_ADDR + 68) // I0
};

//间隙过流保护启动4160----------
const  LONG* FailureInf4160[] = {
    (LONG*)(RM_BUF_ADDR + 72) // Ijx
};

//间隙过流保护动作4161----------
const  LONG* FailureInf4161[] = {
    (LONG*)(RM_BUF_ADDR + 28), // U0
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 68), // I0
    (LONG*)(RM_BUF_ADDR + 72) // Ijx
};

//间隙过流保护返回4162----------
const  LONG* FailureInf4162[] = {
    (LONG*)(RM_BUF_ADDR + 28), // U0
    (LONG*)(RM_BUF_ADDR + 72) // Ijx
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
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf50, 50},//AD故障恢复
    {0, FailureInf51, 51},//软压板故障恢复
    {0, FailureInf52, 52},//EEPROM定值故障恢复
    {0, FailureInf55, 55},//RAM定值故障恢复
    {0, FailureInf58, 58},//程序ROM故障恢复
    {0, FailureInf60, 60},//通信通道异常恢复
    {0, FailureInf62, 62},//NVRAM内容出错恢复
    {0, FailureInf63, 63},//逻辑方程故障故障恢复
    {0, FailureInf64, 64},//5V电源故障恢复
    {0, FailureInf65, 65},//保护数据异常恢复
    {0, FailureInf500, 500},//保护启动
    {0, FailureInf501, 501},//重瓦斯
    {0, FailureInf502, 502},//压力释放
    {0, FailureInf503, 503},//超温跳闸
    {0, FailureInf504, 504},//轻瓦斯
    {0, FailureInf505, 505},//油位异常
    {0, FailureInf506, 506},//温度过高
    {27, FailureInf4100, 4100},//差流越限
    {0, FailureInf4101, 4101},//A相差动速断启动
    {36, FailureInf4102, 4102},//A相差流速断动作
    {0, FailureInf4103, 4103},//A相差动速断返回
    {0, FailureInf4104, 4104},//B相差动速断启动
    {36, FailureInf4105, 4105},//B相差动速断动作
    {0, FailureInf4106, 4106},//B相差动速断返回
    {0, FailureInf4107, 4107},//C相差动速断启动
    {36, FailureInf4108, 4108},//C相差动速断动作
    {0, FailureInf4109, 4109},//C相差动速断返回
    {0, FailureInf4110, 4110},//A相比率差动启动
    {36, FailureInf4111, 4111},//A相比率差动动作
    {0, FailureInf4112, 4112},//A相比率差动返回
    {0, FailureInf4113, 4113},//B相比率差动启动
    {36, FailureInf4114, 4114},//B相比率差动动作
    {0, FailureInf4115, 4115},//B相比率差动返回
    {0, FailureInf4116, 4116},//C相比率差动启动
    {36, FailureInf4117, 4117},//C相比率差动动作
    {0, FailureInf4118, 4118},//C相比率差动返回
    {6, FailureInf4119, 4119},//高压侧过流Ⅰ段启动
    {28, FailureInf4120, 4120},//高压侧过流Ⅰ段动作
    {6, FailureInf4121, 4121},//高压侧过流Ⅰ段返回
    {6, FailureInf4122, 4122},//高压侧过流Ⅱ段启动
    {28, FailureInf4123, 4123},//高压侧过流Ⅱ段动作
    {6, FailureInf4124, 4124},//高压侧过流Ⅱ段返回
    {3, FailureInf4125, 4125},//中压侧过流Ⅰ段启动
    {24, FailureInf4126, 4126},//中压侧过流Ⅰ段动作
    {3, FailureInf4127, 4127},//中压侧过流Ⅰ段返回
    {3, FailureInf4128, 4128},//中压侧过流Ⅱ段启动
    {24, FailureInf4129, 4129},//中压侧过流Ⅱ段动作
    {3, FailureInf4130, 4130},//中压侧过流Ⅱ段返回
    {3, FailureInf4131, 4131},//低压侧过流Ⅰ段启动
    {24, FailureInf4132, 4132},//低压侧过流Ⅰ段动作
    {3, FailureInf4133, 4133},//低压侧过流Ⅰ段返回
    {3, FailureInf4134, 4134},//低压侧过流Ⅱ段启动
    {24, FailureInf4135, 4135},//低压侧过流Ⅱ段动作
    {3, FailureInf4136, 4136},//低压侧过流Ⅱ段返回
    {6, FailureInf4137, 4137},//过负荷Ⅰ段启动
    {28, FailureInf4138, 4138},//过负荷Ⅰ段动作
    {6, FailureInf4139, 4139},//过负荷Ⅰ段返回
    {6, FailureInf4140, 4140},//过负荷Ⅱ段启动
    {28, FailureInf4141, 4141},//过负荷Ⅱ段动作
    {6, FailureInf4142, 4142},//过负荷Ⅱ段返回
    {6, FailureInf4143, 4143},//过负荷Ⅲ段启动
    {28, FailureInf4144, 4144},//过负荷Ⅲ段动作
    {6, FailureInf4145, 4145},//过负荷Ⅲ段返回
    {12, FailureInf4146, 4146},//失压保护动作
    {16, FailureInf4147, 4147},//高压侧PT断线
    {36, FailureInf4148, 4148},//通道测试
    {18, FailureInf4149, 4149},//保护启动
    {0, FailureInf4150, 4150},//失灵保护
    {0, FailureInf4151, 4151},//高侧断路器跳闸失败
    {0, FailureInf4152, 4152},//中侧断路器跳闸失败
    {0, FailureInf4153, 4153},//低侧断路器跳闸失败
    {4, FailureInf4154, 4154},//零序过压保护启动
    {4, FailureInf4155, 4155},//零序过压保护动作
    {4, FailureInf4156, 4156},//零序过压保护返回
    {1, FailureInf4157, 4157},//零序过流保护启动
    {5, FailureInf4158, 4158},//零序过流保护动作
    {1, FailureInf4159, 4159},//零序过流保护返回
    {1, FailureInf4160, 4160},//间隙过流保护启动
    {6, FailureInf4161, 4161},//间隙过流保护动作
    {2, FailureInf4162, 4162},//间隙过流保护返回
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 280) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 280) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 280) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 260) ,
	 (LONG*)(AS_BUF_ADDR + 264) ,
	 (LONG*)(AS_BUF_ADDR + 268) ,
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
};
//开出继电器映射到实际开出
const WORD OutPutMap[] = {
	 (WORD)(0) ,
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
	 (WORD)(30) ,
	 (WORD)(31) ,
	 (WORD)(32) ,
	 (WORD)(33) ,
	 (WORD)(34) ,
	 (WORD)(35) ,
	 (WORD)(36) ,
	 (WORD)(37) ,
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
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	30592, //Up
	11600, //U0
	3050, //Ip
	3050, //I0
	3050, //Ijx
	43238, //UAM
	43238, //UBM
	43238, //UCM
	16400, //U0M
	4336, //IHAM
	4336, //IHBM
	4336, //IHCM
	4336, //IMAM
	4336, //IMBM
	4336, //IMCM
	4336, //ILAM
	4336, //ILBM
	4336, //ILCM
	4336, //I0M
	4336, //IjxM
	21, //UAD
	21, //UBD
	21, //UCD
	21, //U0D
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //IMAD
	21, //IMBD
	21, //IMCD
	21, //ILAD
	21, //ILBD
	21, //ILCD
	21, //I0D
	21, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};
//系数2
const LONG CFDefaultVal2[]=
{
	30592, //Up
	11600, //U0
	15250, //Ip
	15250, //I0
	15250, //Ijx
	43238, //UAM
	43238, //UBM
	43238, //UCM
	16400, //U0M
	21680, //IHAM
	21680, //IHBM
	21680, //IHCM
	21680, //IMAM
	21680, //IMBM
	21680, //IMCM
	21680, //ILAM
	21680, //ILBM
	21680, //ILCM
	21680, //I0M
	21680, //IjxM
	21, //UAD
	21, //UBD
	21, //UCD
	21, //U0D
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //IMAD
	21, //IMBD
	21, //IMCD
	21, //ILAD
	21, //ILBD
	21, //ILCD
	21, //I0D
	21, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};

//传动----------------------------------
