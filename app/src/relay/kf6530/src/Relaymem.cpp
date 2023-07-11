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
	{YD11_T, "Y/D-11牵引"},
	{YD11_E, "Y/D-11电力"},
	{YV_T, "Y/V平衡变"},
	{YA_T, "Y/A平衡变"},
	{SCOTT, "SCOTT变"},
	{VV3_T, "三相V/V变"},
	{VV1_T, "单相V/V变"},
	{SIGNP_T, "单相变"},
	{VX_T, "V/X变压器"},
	{CT_Y, "星接"},
	{CT_D, "角接"},
	{APHASE, "A相回流"},
	{BPHASE, "B相回流"},
	{CPHASE, "C相回流"},
	{-1, NULL},
};
const TSetValueInfMap SetValueInfMap[]=
{
	{0, NULL},
	{0, NULL},
	{9, (TSetValueInf*)&(SetValueInf[0])}, //定值变压器类型
	{0, NULL},
	{2, (TSetValueInf*)&(SetValueInf[9])}, //定值高侧CT接线
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{3, (TSetValueInf*)&(SetValueInf[11])}, //定值VV变回流相
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
};

//保护测量值定义-----------------------
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// IA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// IB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// IC
LONG* 	RM_pnIL1 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// I1
LONG* 	RM_pnIL2 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// I2
LONG* 	RM_pnIL3 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// I3
LONG* 	RM_pnIL4 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// I4
LONG* 	RM_pnIDA 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// IDA
LONG* 	RM_pnIDB 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// IDB
LONG* 	RM_pnIDC 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// IDC
LONG* 	RM_pnIRA 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// IRA
LONG* 	RM_pnIRB 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// IRB
LONG* 	RM_pnIRC 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IRC
LONG* 	RM_pnIDA2 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// IDA2
LONG* 	RM_pnIDA3 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// IDA3
LONG* 	RM_pnIDA5 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// IDA5
LONG* 	RM_pnIDB2 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// IDB2
LONG* 	RM_pnIDB3 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IDB3
LONG* 	RM_pnIDB5 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// IDB5
LONG* 	RM_pnIDC2 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// IDC2
LONG* 	RM_pnIDC3 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IDC3
LONG* 	RM_pnIDC5 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// IDC5
LONG* 	RM_pnIRA2 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// IRA2
LONG* 	RM_pnIRB2 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// IRB2
LONG* 	RM_pnIRC2 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// IRC2
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// ΦIA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// ΦIB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// ΦIC
LONG* 	RM_pnAIL1 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// ΦI1
LONG* 	RM_pnAIL2 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// ΦI2
LONG* 	RM_pnAIL3 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// ΦI3
LONG* 	RM_pnAIL4 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// ΦI4
LONG* 	RM_pnUA 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// UA
LONG* 	RM_pnUB 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// UB
LONG* 	RM_pnUC 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// UC
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnIHAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//IHAM IHAM
LONG* 	M_pnIHBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//IHBM IHBM
LONG* 	M_pnIHCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//IHCM IHCM
LONG* 	M_pnIL1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//IL1M IL1M
LONG* 	M_pnIL2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IL2M IL2M
LONG* 	M_pnIL3M 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IL3M IL3M
LONG* 	M_pnIL4M 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IL4M IL4M


//软压板----------------------------------
BYTE* SW_pby87U 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//差动电流速断
BYTE* SW_pby87R 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//比率差动保护
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby87U_A 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//A相差流速断保护
BYTE* CFG_pby87U_B 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//B相差流速断保护
BYTE* CFG_pby87U_C 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//C相差流速断保护
BYTE* CFG_pby87R_A 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//A相比率差动保护
BYTE* CFG_pby87R_B 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//B相比率差动保护
BYTE* CFG_pby87R_C 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//C相比率差动保护
BYTE* CFG_pby68IdH2M_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//差流二次最大相闭锁
BYTE* CFG_pby68IdH2C_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//差流二次综合相闭锁
BYTE* CFG_pby68IdH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//差流二次分相闭锁
BYTE* CFG_pby68IrH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//制动二次闭锁
BYTE* CFG_pby68IdH5I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//差流五次闭锁
BYTE* CFG_pby68IdHMUL_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//差流综合谐波闭锁
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//备用配置2

//定值----------------------------------
LONG* RS_pnCFG1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字 CFG1
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//突变量启动电流 PIC
LONG* RS_pnTRTY 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//变压器类型 TRTY
LONG* RS_pnLINUM 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//低压侧电流相数 LINUM
LONG* RS_pnCT1CON 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//高侧CT接线 CT1CON
LONG* RS_pn87UIA 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//A相差动速断定值 87UIA
LONG* RS_pn87RIA 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//A相差动电流定值 87RIA
LONG* RS_pnIRS1A 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//A相制动拐点1 IRS1A
LONG* RS_pnIRS2A 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//A相制动拐点2 IRS2A
LONG* RS_pn87UIB 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//B相差动速断定值 87UIB
LONG* RS_pn87RIB 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//B相差动电流定值 87RIB
LONG* RS_pnIRS1B 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//B相制动拐点1 IRS1B
LONG* RS_pnIRS2B 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//B相制动拐点2 IRS2B
LONG* RS_pn87UIC 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//C相差动速断定值 87UIC
LONG* RS_pn87RIC 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//C相差动电流定值 87RIC
LONG* RS_pnIRS1C 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//C相制动拐点1 IRS1C
LONG* RS_pnIRS2C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//C相制动拐点2 IRS2C
LONG* RS_pnSLP1 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//比率制动系数1 SLP1
LONG* RS_pnSLP2 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//比率制动系数2 SLP2
LONG* RS_pnKrh2 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//二次制动系数 Krh2
LONG* RS_pnKrh5 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//五次制动系数 Krh5
LONG* RS_pnKrh 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//综合谐波制动系数 Krh
LONG* RS_pnKrav 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//差动非周期加权 Krav
LONG* RS_pnK2av 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//二次非周期加权 K2av
LONG* RS_pnKba 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//a相平衡系数 Kba
LONG* RS_pnKbb 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//b相平衡系数 Kbb
LONG* RS_pnKbc 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//c相平衡系数 Kbc
LONG* RS_pnK87AR 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//差流越限报警比率 K87AR
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//失灵保护时间 52BFD
LONG* RS_pnBph 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//VV变回流相 Bph

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
LONG* AS_pn50HAC 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//IHA有流定值
LONG* AS_pn37HAC 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//IHA无流定值
LONG* AS_pn50HBC 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//IHB有流定值
LONG* AS_pn37HBC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IHB无流定值
LONG* AS_pn50HCC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IHC有流定值
LONG* AS_pn37HCC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//IHC无流定值
LONG* AS_pn50L1C 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IL1有流定值
LONG* AS_pn37L1C 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//IL1无流定值
LONG* AS_pn50L2C 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//IL2有流定值
LONG* AS_pn37L2C 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//IL2无流定值
LONG* AS_pn50L3C 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//IL3有流定值
LONG* AS_pn37L3C 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//IL3无流定值
LONG* AS_pn50L4C 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//IL4有流定值
LONG* AS_pn37L4C 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//IL4无流定值
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//断路器动作延时
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//隔开动作延时
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//TR2保持时间
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//TR3保持时间
LONG* AS_pn59UA 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//UA有压定值
LONG* AS_pn27UA 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//UA无压定值
LONG* AS_pn59UB 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//UB有压定值
LONG* AS_pn27UB 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//UB无压定值
LONG* AS_pn59UC 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//UC有压定值
LONG* AS_pn27UC 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//UC无压定值
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnHACT 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//高压侧A相CT变比
LONG* AS_pnHBCT 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//高压侧B相CT变比
LONG* AS_pnHCCT 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//高压侧C相CT变比
LONG* AS_pnL1CT 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//低压侧CT1变比
LONG* AS_pnL2CT 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//低压侧CT2变比
LONG* AS_pnL3CT 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//低压侧CT3变比
LONG* AS_pnL4CT 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//低压侧CT4变比
LONG* AS_pnHPT 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//高压侧PT变比
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//固定变比

//系数----------------------------------
LONG* CF_pnIA 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//IA
LONG* CF_pnIB 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//IB
LONG* CF_pnIC 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//IC
LONG* CF_pnI1 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//I1
LONG* CF_pnI2 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//I2
LONG* CF_pnI3 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//I3
LONG* CF_pnI4 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//I4
LONG* CF_pnIDA 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//IDA
LONG* CF_pnIDB 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IDB
LONG* CF_pnIDC 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IDC
LONG* CF_pnIRA 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IRA
LONG* CF_pnIRB 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IRB
LONG* CF_pnIRC 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IRC
LONG* CF_pnU 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//U
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//C4
LONG* CF_pnUAM 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//UAM
LONG* CF_pnUBM 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//UBM
LONG* CF_pnUCM 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//UCM
LONG* CF_pnIAM 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//IAM
LONG* CF_pnIBM 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//IBM
LONG* CF_pnICM 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//ICM
LONG* CF_pnI1M 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//I1M
LONG* CF_pnI2M 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//I2M
LONG* CF_pnI3M 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//I3M
LONG* CF_pnI4M 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//I4M
LONG* CF_pnUAD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//UAD
LONG* CF_pnUBD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//UBD
LONG* CF_pnUCD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//UCD
LONG* CF_pnIAD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IAD
LONG* CF_pnIBD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IBD
LONG* CF_pnICD 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//ICD
LONG* CF_pnI1D 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//I1D
LONG* CF_pnI2D 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//I2D
LONG* CF_pnI3D 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//I3D
LONG* CF_pnI4D 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//I4D

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 148), // IN1
    (LONG*)(RM_BUF_ADDR + 152) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 140), // BR1
    (LONG*)(RM_BUF_ADDR + 144) // BR2
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

//逻辑方程故障故障恢复63----------
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

//温度过高506----------
const  LONG* FailureInf506[] = {
    {NULL}
};

//失电告警507----------
const  LONG* FailureInf507[] = {
    {NULL}
};

//重瓦斯1508----------
const  LONG* FailureInf508[] = {
    {NULL}
};

//压力释放1509----------
const  LONG* FailureInf509[] = {
    {NULL}
};

//超温跳闸1510----------
const  LONG* FailureInf510[] = {
    {NULL}
};

//轻瓦斯1511----------
const  LONG* FailureInf511[] = {
    {NULL}
};

//油位异常1512----------
const  LONG* FailureInf512[] = {
    {NULL}
};

//温度过高1513----------
const  LONG* FailureInf513[] = {
    {NULL}
};

//失电告警1514----------
const  LONG* FailureInf514[] = {
    {NULL}
};

//重瓦斯2515----------
const  LONG* FailureInf515[] = {
    {NULL}
};

//压力释放2516----------
const  LONG* FailureInf516[] = {
    {NULL}
};

//超温跳闸2517----------
const  LONG* FailureInf517[] = {
    {NULL}
};

//轻瓦斯2518----------
const  LONG* FailureInf518[] = {
    {NULL}
};

//油位异常2519----------
const  LONG* FailureInf519[] = {
    {NULL}
};

//温度过高2520----------
const  LONG* FailureInf520[] = {
    {NULL}
};

//失电告警2521----------
const  LONG* FailureInf521[] = {
    {NULL}
};

//差流越限1001----------
const  LONG* FailureInf1001[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//A相差动速断启动1002----------
const  LONG* FailureInf1002[] = {
    {NULL}
};

//A相差流速断动作1003----------
const  LONG* FailureInf1003[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//A相差动速断返回1004----------
const  LONG* FailureInf1004[] = {
    {NULL}
};

//B相差动速断启动1005----------
const  LONG* FailureInf1005[] = {
    {NULL}
};

//B相差动速断动作1006----------
const  LONG* FailureInf1006[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//B相差动速断返回1007----------
const  LONG* FailureInf1007[] = {
    {NULL}
};

//C相差动速断启动1008----------
const  LONG* FailureInf1008[] = {
    {NULL}
};

//C相差动速断动作1009----------
const  LONG* FailureInf1009[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//C相差动速断返回1010----------
const  LONG* FailureInf1010[] = {
    {NULL}
};

//A相比率差动启动1011----------
const  LONG* FailureInf1011[] = {
    {NULL}
};

//A相比率差动动作1012----------
const  LONG* FailureInf1012[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//A相比率差动返回1013----------
const  LONG* FailureInf1013[] = {
    {NULL}
};

//B相比率差动启动1014----------
const  LONG* FailureInf1014[] = {
    {NULL}
};

//B相比率差动动作1015----------
const  LONG* FailureInf1015[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//B相比率差动返回1016----------
const  LONG* FailureInf1016[] = {
    {NULL}
};

//C相比率差动启动1017----------
const  LONG* FailureInf1017[] = {
    {NULL}
};

//C相比率差动动作1018----------
const  LONG* FailureInf1018[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//C相比率差动返回1019----------
const  LONG* FailureInf1019[] = {
    {NULL}
};

//通道测试1024----------
const  LONG* FailureInf1024[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//保护启动1025----------
const  LONG* FailureInf1025[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//断路器跳闸失败1026----------
const  LONG* FailureInf1026[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 24) // I4
};

//失灵保护1027----------
const  LONG* FailureInf1027[] = {
    {NULL}
};

//差流越限返回1028----------
const  LONG* FailureInf1028[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ΦIA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ΦIB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ΦIC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ΦI1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ΦI2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ΦI3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ΦI4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48) // IRC
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
    {0, FailureInf20, 20},//5V电源故障
    {0, FailureInf21, 21},//EEPROM写入故障
    {0, FailureInf50, 50},//AD故障恢复
    {0, FailureInf51, 51},//软压板故障恢复
    {0, FailureInf52, 52},//EEPROM定值故障恢复
    {0, FailureInf55, 55},//RAM定值故障恢复
    {0, FailureInf58, 58},//程序ROM故障恢复
    {0, FailureInf60, 60},//通信通道异常恢复
    {0, FailureInf62, 62},//NVRAM内容出错恢复
    {0, FailureInf63, 63},//逻辑方程故障故障恢复
    {0, FailureInf64, 64},//5V电源故障恢复
    {0, FailureInf500, 500},//保护启动
    {0, FailureInf501, 501},//重瓦斯
    {0, FailureInf502, 502},//压力释放
    {0, FailureInf503, 503},//超温跳闸
    {0, FailureInf504, 504},//轻瓦斯
    {0, FailureInf505, 505},//油位异常
    {0, FailureInf506, 506},//温度过高
    {0, FailureInf507, 507},//失电告警
    {0, FailureInf508, 508},//重瓦斯1
    {0, FailureInf509, 509},//压力释放1
    {0, FailureInf510, 510},//超温跳闸1
    {0, FailureInf511, 511},//轻瓦斯1
    {0, FailureInf512, 512},//油位异常1
    {0, FailureInf513, 513},//温度过高1
    {0, FailureInf514, 514},//失电告警1
    {0, FailureInf515, 515},//重瓦斯2
    {0, FailureInf516, 516},//压力释放2
    {0, FailureInf517, 517},//超温跳闸2
    {0, FailureInf518, 518},//轻瓦斯2
    {0, FailureInf519, 519},//油位异常2
    {0, FailureInf520, 520},//温度过高2
    {0, FailureInf521, 521},//失电告警2
    {32, FailureInf1001, 1001},//差流越限
    {0, FailureInf1002, 1002},//A相差动速断启动
    {35, FailureInf1003, 1003},//A相差流速断动作
    {0, FailureInf1004, 1004},//A相差动速断返回
    {0, FailureInf1005, 1005},//B相差动速断启动
    {35, FailureInf1006, 1006},//B相差动速断动作
    {0, FailureInf1007, 1007},//B相差动速断返回
    {0, FailureInf1008, 1008},//C相差动速断启动
    {35, FailureInf1009, 1009},//C相差动速断动作
    {0, FailureInf1010, 1010},//C相差动速断返回
    {0, FailureInf1011, 1011},//A相比率差动启动
    {35, FailureInf1012, 1012},//A相比率差动动作
    {0, FailureInf1013, 1013},//A相比率差动返回
    {0, FailureInf1014, 1014},//B相比率差动启动
    {35, FailureInf1015, 1015},//B相比率差动动作
    {0, FailureInf1016, 1016},//B相比率差动返回
    {0, FailureInf1017, 1017},//C相比率差动启动
    {35, FailureInf1018, 1018},//C相比率差动动作
    {0, FailureInf1019, 1019},//C相比率差动返回
    {35, FailureInf1024, 1024},//通道测试
    {35, FailureInf1025, 1025},//保护启动
    {7, FailureInf1026, 1026},//断路器跳闸失败
    {0, FailureInf1027, 1027},//失灵保护
    {20, FailureInf1028, 1028},//差流越限返回
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 112) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 196) ,
	 (LONG*)(AS_BUF_ADDR + 200) ,
	 (LONG*)(AS_BUF_ADDR + 204) ,
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
};
//压板继电器映射到压板值在内存中位置
BYTE* const SWMap[] = {
	 (BYTE*)(SW_BUF_ADDR + 0) ,
	 (BYTE*)(SW_BUF_ADDR + 1) ,
	 (BYTE*)(SW_BUF_ADDR + 2) ,
	 (BYTE*)(SW_BUF_ADDR + 3) ,
	 (BYTE*)(SW_BUF_ADDR + 4) ,
};
//压板继电器映射到压板号压板号
const WORD  SWIndexMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
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
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	3060, //IA
	3060, //IB
	3060, //IC
	3060, //I1
	3060, //I2
	3060, //I3
	3060, //I4
	3060, //IDA
	3060, //IDB
	3060, //IDC
	3050, //IRA
	3050, //IRB
	3050, //IRC
	30592, //U
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	43238, //UAM
	43238, //UBM
	43238, //UCM
	4336, //IAM
	4336, //IBM
	4336, //ICM
	4336, //I1M
	4336, //I2M
	4336, //I3M
	4336, //I4M
	21, //UAD
	21, //UBD
	21, //UCD
	21, //IAD
	21, //IBD
	21, //ICD
	21, //I1D
	21, //I2D
	21, //I3D
	21, //I4D
};
//系数2
const LONG CFDefaultVal2[]=
{
	15250, //IA
	15250, //IB
	15250, //IC
	15250, //I1
	15250, //I2
	15250, //I3
	15250, //I4
	15250, //IDA
	15250, //IDB
	15250, //IDC
	15250, //IRA
	15250, //IRB
	15250, //IRC
	30592, //U
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	43238, //UAM
	43238, //UBM
	43238, //UCM
	21680, //IAM
	21680, //IBM
	21680, //ICM
	21680, //I1M
	21680, //I2M
	21680, //I3M
	21680, //I4M
	21, //UAD
	21, //UBD
	21, //UCD
	21, //IAD
	21, //IBD
	21, //ICD
	21, //I1D
	21, //I2D
	21, //I3D
	21, //I4D
};

//传动----------------------------------
