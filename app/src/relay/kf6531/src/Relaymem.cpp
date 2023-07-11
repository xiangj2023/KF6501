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
};
//辅助定值取值信息
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{LCUR_2, "低侧两路电流"},
	{LCUR_4, "低侧四路电流"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //辅助定值额定电流
	{2, (TSetValueInf*)&(SysSetValueInf[2])}, //辅助定值低侧电流路数
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnUHAB 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// UHAB
LONG* 	RM_pnUHBC 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// UHBC
LONG* 	RM_pnUHCA 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// UHCA
LONG* 	RM_pnUHA 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// UHA
LONG* 	RM_pnUHB 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// UHB
LONG* 	RM_pnUHC 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// UHC
LONG* 	RM_pnU0 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// U0
LONG* 	RM_pnULA 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// ULA
LONG* 	RM_pnULB 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// ULB
LONG* 	RM_pnULAB 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// ULAB
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// IHA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// IHB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IHC
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// I0
LONG* 	RM_pnIJX 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// IJX
LONG* 	RM_pnILA 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// ILA
LONG* 	RM_pnILB 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// ILB
LONG* 	RM_pnIL1 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IL1
LONG* 	RM_pnIL2 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// IL2
LONG* 	RM_pnIL3 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// IL3
LONG* 	RM_pnIL4 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IL4
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UHA UHA
LONG* 	M_pnUHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UHB UHB
LONG* 	M_pnUHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UHC UHC
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnULA 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//ULA ULA
LONG* 	M_pnULB 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//ULB ULB
LONG* 	M_pnIHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//IHA IHA
LONG* 	M_pnIHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IHB IHB
LONG* 	M_pnIHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IHC IHC
LONG* 	M_pnI0 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//I0 I0
LONG* 	M_pnILA 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//ILA ILA
LONG* 	M_pnILB 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//ILB ILB
LONG* 	M_pnIjx 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ijx Ijx
LONG* 	M_pnIL1 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//IL1 IL1
LONG* 	M_pnIL2 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//IL2 IL2
LONG* 	M_pnIL3 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//IL3 IL3
LONG* 	M_pnIL4 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//IL4 IL4


//软压板----------------------------------
BYTE* SW_pby50H 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//高侧过流保护
BYTE* SW_pby49 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//反时限过负荷
BYTE* SW_pby50L 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//低侧过流保护
BYTE* SW_pby50OL 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//过负荷保护
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//高侧失压保护
BYTE* SW_pby50N 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//零序过流保护
BYTE* SW_pby59N 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//零序过压保护
BYTE* SW_pby50JX 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//间隙过流保护
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby50HA 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//高侧A相过流
BYTE* CFG_pby50HB 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//高侧B相过流
BYTE* CFG_pby50HC 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//高侧C相过流
BYTE* CFG_pby50H_27 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//高侧低压启动
BYTE* CFG_pby49 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//反时限过负荷
BYTE* CFG_pby50LA1 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//低侧A相过流I段
BYTE* CFG_pby50LA2 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//低侧A相过流II段
BYTE* CFG_pby50LA_27 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//低侧A低压启动
BYTE* CFG_pby50LB1 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//低侧B相过流I段
BYTE* CFG_pby50LB2 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//低侧B相过流II段
BYTE* CFG_pby50LB_27 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//低侧B低压启动
BYTE* CFG_pby50OLA1 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//A过负荷告警
BYTE* CFG_pby50OLB1 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//B过负荷告警
BYTE* CFG_pby50OLC1 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//C过负荷告警
BYTE* CFG_pby59OLA2 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//A过负荷跳闸
BYTE* CFG_pby50OLB2 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//B过负荷跳闸
BYTE* CFG_pby50OLC2 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//C过负荷跳闸
BYTE* CFG_pby27 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//高压侧失压保护
BYTE* CFG_pby50N 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//零序过流保护
BYTE* CFG_pby59N 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//零序过压保护
BYTE* CFG_pby74PT 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//PT断线检测
BYTE* CFG_pby50DCG 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//间隙过流保护
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//备用配置2

//定值----------------------------------
LONG* RS_pnF1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字1 F1
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//突变量启动电流 PIC
LONG* RS_pn50HAC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//高压侧A过流电流 50HAC
LONG* RS_pn50HAD 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//高压侧A过流时间 50HAD
LONG* RS_pn50HBC 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//高压侧B过流电流 50HBC
LONG* RS_pn50HBD 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//高压侧B过流时间 50HBD
LONG* RS_pn50HCC 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//高压侧C过流电流 50HCC
LONG* RS_pn50HCD 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//高压侧C过流时间 50HCD
LONG* RS_pn27LU 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//低压侧低压定值 27LU
LONG* RS_pnTIN 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//变压器二次额定电流 TIN
LONG* RS_pn49K 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//热过负荷保护因子 49K
LONG* RS_pn49T 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//热过负荷时间常数 49T
LONG* RS_pn49AR 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//热过负荷告警定值 49AR
LONG* RS_pn49KT 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//设备停止扩充系数 49KT
LONG* RS_pn50LA1C 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//低侧A相过流I段定值 50LA1C
LONG* RS_pn50LA1D 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//低侧A相过流I段时间 50LA1D
LONG* RS_pn50LA2C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//低侧A相过流II段定值 50LA2C
LONG* RS_pn50LA2D 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//低侧A相过流II段时间 50LA2D
LONG* RS_pn50LB1C 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//低侧B相过流I段定值 50LB1C
LONG* RS_pn50LB1D 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//低侧B相过流I段时间 50LB1D
LONG* RS_pn50LB2C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//低侧B相过流II段定值 50LB2C
LONG* RS_pn50LB2D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//低侧B相过流II段时间 50LB2D
LONG* RS_pn50OLA1C 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//A相过负荷I段电流 50OLA1C
LONG* RS_pn50OLA1D 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//A相过负荷I段时间 50OLA1D
LONG* RS_pn50OLB1C 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//B相过负荷I段电流 50OLB1C
LONG* RS_pn50OLB1D 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//B相过负荷I段时间 50OLB1D
LONG* RS_pn50OLC1C 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//C相过负荷I段电流 50OLC1C
LONG* RS_pn50OLC1D 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//C相过负荷I段时间 50OLC1D
LONG* RS_pn50OLA2C 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//A相过负荷II段电流 50OLA2C
LONG* RS_pn50OLA2D 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//A相过负荷II段时间 50OLA2D
LONG* RS_pn50OLB2C 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//B相过负荷II段电流 50OLB2C
LONG* RS_pn50OLB2D 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//B相过负荷II段时间 50OLB2D
LONG* RS_pn50OLC2C 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//C相过负荷II段电流 50OLC2C
LONG* RS_pn50OLC2D 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//C相过负荷II段时间 50OLC2D
LONG* RS_pn50NC 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//零序过流定值 50NC
LONG* RS_pn50ND 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//零序过流时间 50ND
LONG* RS_pn59NV 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//零序过压定值 59NV
LONG* RS_pn59ND 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//零序过压时间 59ND
LONG* RS_pn27HV 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//高压侧失压定值 27HV
LONG* RS_pn27HD 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//高压侧失压时间 27HD
LONG* RS_pn27H_LK50 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//高侧失压电流闭锁 27H_LK50
LONG* RS_pn74HPTV 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//高压侧PT断线定值 74HPTV
LONG* RS_pn74LPTV 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//低压侧PT断线定值 74LPTV
LONG* RS_pn50DCGC 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//间隙过流保护电流 50DCGC
LONG* RS_pn50DCGD 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//间隙过流保护时间 50DCGD
LONG* RS_pn74PTHC 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//高侧PT断线闭锁电流 74PTHC
LONG* RS_pn74PTLC 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//低侧PT断线闭锁电流 74PTLC
LONG* RS_pn74PTD 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//PT断线判断时间 74PTD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//失灵保护判断时间 52BFD

//辅助定值----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//额定电流
LONG* AS_pnLCN 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//低侧电流路数
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
LONG* AS_pn50HAC 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//IHA有流定值
LONG* AS_pn37HAC 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//IHA无流定值
LONG* AS_pn50HBC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IHB有流定值
LONG* AS_pn37HBC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IHB无流定值
LONG* AS_pn50HCC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//IHC有流定值
LONG* AS_pn37HCC 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IHC无流定值
LONG* AS_pn59HAV 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//UHA有压定值
LONG* AS_pn27HAV 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//UHA无压定值
LONG* AS_pn59HBV 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//UHB有压定值
LONG* AS_pn27HBV 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//UHB无压定值
LONG* AS_pn59HCV 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//UHC有压定值
LONG* AS_pn27HCV 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//UHC无压定值
LONG* AS_pn50LAC 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//ILA有流定值
LONG* AS_pn37LAC 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//ILA无流定值
LONG* AS_pn50LBC 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//ILB有流定值
LONG* AS_pn37LBC 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//ILB无流定值
LONG* AS_pn59LAV 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//ULA有压定值
LONG* AS_pn27LAV 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//ULA无压定值
LONG* AS_pn59LBV 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//ULB有压定值
LONG* AS_pn27LBV 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//ULB无压定值
LONG* AS_pn50NC 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//I0有流定值
LONG* AS_pn37NC 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//I0无流定值
LONG* AS_pn59NV 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//U0有压定值
LONG* AS_pn27NV 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//U0无压定值
LONG* AS_pn50JXC 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//IJX有流定值
LONG* AS_pn37JXC 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//IJX无流定值
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//断路器动作延时
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//隔开动作延时
LONG* AS_pnTR1T 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//TR1保持时间
LONG* AS_pnTR2T 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//TR2保持时间
LONG* AS_pnTR3T 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//TR3保持时间
LONG* AS_pnTR4T 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//TR4保持时间
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnHPTRT 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//高侧PT变比
LONG* AS_pnHU0PTRT 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//高侧零序PT变比
LONG* AS_pnHCTRT 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//高侧CT变比
LONG* AS_pnHI0CTRT 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//高侧零序CT变比
LONG* AS_pnLPTRT 	= 	(LONG*)(AS_BUF_ADDR + 284); 	//低侧PT变比
LONG* AS_pnLCT1RT 	= 	(LONG*)(AS_BUF_ADDR + 288); 	//低侧CT1变比
LONG* AS_pnLCT2RT 	= 	(LONG*)(AS_BUF_ADDR + 292); 	//低侧CT2变比
LONG* AS_pnLCT3RT 	= 	(LONG*)(AS_BUF_ADDR + 296); 	//低侧CT3变比
LONG* AS_pnLCT4RT 	= 	(LONG*)(AS_BUF_ADDR + 300); 	//低侧CT4变比
LONG* AS_pnRT1 	= 	(LONG*)(AS_BUF_ADDR + 304); 	//RT1

//系数----------------------------------
LONG* CF_pnUHAB 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UHAB
LONG* CF_pnUHBC 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UHBC
LONG* CF_pnUHCA 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//UHCA
LONG* CF_pnUHA 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//UHA
LONG* CF_pnUHB 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//UHB
LONG* CF_pnUHC 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UHC
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//U0
LONG* CF_pnULA 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//ULA
LONG* CF_pnULB 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//ULB
LONG* CF_pnIHA 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IHA
LONG* CF_pnIHB 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IHB
LONG* CF_pnIHC 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IHC
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//I0
LONG* CF_pnIL1 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IL1
LONG* CF_pnIL2 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//IL2
LONG* CF_pnIL3 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//IL3
LONG* CF_pnIL4 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IL4
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//C4
LONG* CF_pnUHAM 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//UHAM
LONG* CF_pnUHBM 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//UHBM
LONG* CF_pnUHCM 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//UHCM
LONG* CF_pnU0M 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//U0M
LONG* CF_pnULAM 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//ULAM
LONG* CF_pnULBM 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//ULBM
LONG* CF_pnIHAM 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//IHAM
LONG* CF_pnIHBM 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//IHBM
LONG* CF_pnIHCM 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//IHCM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//I0M
LONG* CF_pnIL1M 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IL1M
LONG* CF_pnIL2M 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IL2M
LONG* CF_pnIL3M 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//IL3M
LONG* CF_pnIL4M 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//IL4M
LONG* CF_pnUHAD 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//UHAD
LONG* CF_pnUHBD 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//UHBD
LONG* CF_pnUHCD 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//UHCD
LONG* CF_pnU0D 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//U0D
LONG* CF_pnULAD 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//ULAD
LONG* CF_pnULBD 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//ULBD
LONG* CF_pnIHAD 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//IHAD
LONG* CF_pnIHBD 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//IHBD
LONG* CF_pnIHCD 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//IHCD
LONG* CF_pnI0D 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//I0D
LONG* CF_pnIL1D 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//IL1D
LONG* CF_pnIL2D 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//IL2D
LONG* CF_pnIL3D 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//IL3D
LONG* CF_pnIL4D 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//IL4D

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 92), // IN1
    (LONG*)(RM_BUF_ADDR + 96) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 84), // BR1
    (LONG*)(RM_BUF_ADDR + 88) // BR2
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

//反时限过负荷告警1100----------
const  LONG* FailureInf1100[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//反时限过负荷动作1101----------
const  LONG* FailureInf1101[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//反时限过负荷返回1102----------
const  LONG* FailureInf1102[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48) // IHC
};

//高压侧A过流启动1103----------
const  LONG* FailureInf1103[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//高压侧A过流动作1104----------
const  LONG* FailureInf1104[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高压侧A过流返回1105----------
const  LONG* FailureInf1105[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//高压侧B过流启动1106----------
const  LONG* FailureInf1106[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//高压侧B过流动作1107----------
const  LONG* FailureInf1107[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高压侧B过流返回1108----------
const  LONG* FailureInf1108[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//高压侧C过流启动1109----------
const  LONG* FailureInf1109[] = {
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//高压侧C过流动作1110----------
const  LONG* FailureInf1110[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高压侧C过流返回1111----------
const  LONG* FailureInf1111[] = {
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧A过流Ⅰ段启动1112----------
const  LONG* FailureInf1112[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧A过流Ⅰ段动作1113----------
const  LONG* FailureInf1113[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧A过流Ⅰ段返回1114----------
const  LONG* FailureInf1114[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧A过流Ⅱ段启动1115----------
const  LONG* FailureInf1115[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧A过流Ⅱ段动作1116----------
const  LONG* FailureInf1116[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧A过流Ⅱ段返回1117----------
const  LONG* FailureInf1117[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧B过流Ⅰ段启动1118----------
const  LONG* FailureInf1118[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧B过流Ⅰ段动作1119----------
const  LONG* FailureInf1119[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧B过流Ⅰ段返回1120----------
const  LONG* FailureInf1120[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧B过流Ⅱ段启动1121----------
const  LONG* FailureInf1121[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//低压侧B过流Ⅱ段动作1122----------
const  LONG* FailureInf1122[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧B过流Ⅱ段返回1123----------
const  LONG* FailureInf1123[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//间隙过流保护启动1124----------
const  LONG* FailureInf1124[] = {
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//间隙过流保护动作1125----------
const  LONG* FailureInf1125[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//间隙过流保护返回1126----------
const  LONG* FailureInf1126[] = {
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//零序过流保护启动1127----------
const  LONG* FailureInf1127[] = {
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//零序过流保护动作1128----------
const  LONG* FailureInf1128[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//零序过流保护返回1129----------
const  LONG* FailureInf1129[] = {
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//零序过压保护启动1130----------
const  LONG* FailureInf1130[] = {
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//零序过压保护动作1131----------
const  LONG* FailureInf1131[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//零序过压保护返回1132----------
const  LONG* FailureInf1132[] = {
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//高侧A过负荷Ⅰ告警1133----------
const  LONG* FailureInf1133[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧B过负荷Ⅰ告警1134----------
const  LONG* FailureInf1134[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧C过负荷Ⅰ告警1135----------
const  LONG* FailureInf1135[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧A过负荷Ⅱ启动1136----------
const  LONG* FailureInf1136[] = {
    (LONG*)(RM_BUF_ADDR + 40) // IHA
};

//高侧A过负荷Ⅱ动作1137----------
const  LONG* FailureInf1137[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧A过负荷Ⅱ返回1138----------
const  LONG* FailureInf1138[] = {
    (LONG*)(RM_BUF_ADDR + 40) // IHA
};

//高侧B过负荷Ⅱ启动1139----------
const  LONG* FailureInf1139[] = {
    (LONG*)(RM_BUF_ADDR + 44) // IHB
};

//高侧B过负荷Ⅱ动作1140----------
const  LONG* FailureInf1140[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧B过负荷Ⅱ返回1141----------
const  LONG* FailureInf1141[] = {
    (LONG*)(RM_BUF_ADDR + 44) // IHB
};

//高侧C过负荷Ⅱ启动1142----------
const  LONG* FailureInf1142[] = {
    (LONG*)(RM_BUF_ADDR + 48) // IHC
};

//高侧C过负荷Ⅱ动作1143----------
const  LONG* FailureInf1143[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧C过负荷Ⅱ返回1144----------
const  LONG* FailureInf1144[] = {
    (LONG*)(RM_BUF_ADDR + 48) // IHC
};

//高压侧失压动作1145----------
const  LONG* FailureInf1145[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高压侧PT断线1146----------
const  LONG* FailureInf1146[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高压侧PT断线恢复1147----------
const  LONG* FailureInf1147[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧A相PT断线1148----------
const  LONG* FailureInf1148[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧A相PT断线恢复1149----------
const  LONG* FailureInf1149[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧B相PT断线1150----------
const  LONG* FailureInf1150[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低压侧B相PT断线恢复1151----------
const  LONG* FailureInf1151[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//保护启动1152----------
const  LONG* FailureInf1152[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//高侧断路器跳闸失败1153----------
const  LONG* FailureInf1153[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//通道测试1154----------
const  LONG* FailureInf1154[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//失灵保护1155----------
const  LONG* FailureInf1155[] = {
    {NULL}
};

//低侧A相断路器跳闸失败1156----------
const  LONG* FailureInf1156[] = {
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//低侧B相断路器跳闸失败1157----------
const  LONG* FailureInf1157[] = {
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
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
    {21, FailureInf1100, 1100},//反时限过负荷告警
    {21, FailureInf1101, 1101},//反时限过负荷动作
    {3, FailureInf1102, 1102},//反时限过负荷返回
    {4, FailureInf1103, 1103},//高压侧A过流启动
    {21, FailureInf1104, 1104},//高压侧A过流动作
    {4, FailureInf1105, 1105},//高压侧A过流返回
    {4, FailureInf1106, 1106},//高压侧B过流启动
    {21, FailureInf1107, 1107},//高压侧B过流动作
    {4, FailureInf1108, 1108},//高压侧B过流返回
    {4, FailureInf1109, 1109},//高压侧C过流启动
    {21, FailureInf1110, 1110},//高压侧C过流动作
    {4, FailureInf1111, 1111},//高压侧C过流返回
    {4, FailureInf1112, 1112},//低压侧A过流Ⅰ段启动
    {21, FailureInf1113, 1113},//低压侧A过流Ⅰ段动作
    {4, FailureInf1114, 1114},//低压侧A过流Ⅰ段返回
    {4, FailureInf1115, 1115},//低压侧A过流Ⅱ段启动
    {21, FailureInf1116, 1116},//低压侧A过流Ⅱ段动作
    {4, FailureInf1117, 1117},//低压侧A过流Ⅱ段返回
    {4, FailureInf1118, 1118},//低压侧B过流Ⅰ段启动
    {21, FailureInf1119, 1119},//低压侧B过流Ⅰ段动作
    {4, FailureInf1120, 1120},//低压侧B过流Ⅰ段返回
    {4, FailureInf1121, 1121},//低压侧B过流Ⅱ段启动
    {21, FailureInf1122, 1122},//低压侧B过流Ⅱ段动作
    {4, FailureInf1123, 1123},//低压侧B过流Ⅱ段返回
    {2, FailureInf1124, 1124},//间隙过流保护启动
    {21, FailureInf1125, 1125},//间隙过流保护动作
    {2, FailureInf1126, 1126},//间隙过流保护返回
    {2, FailureInf1127, 1127},//零序过流保护启动
    {21, FailureInf1128, 1128},//零序过流保护动作
    {2, FailureInf1129, 1129},//零序过流保护返回
    {1, FailureInf1130, 1130},//零序过压保护启动
    {21, FailureInf1131, 1131},//零序过压保护动作
    {1, FailureInf1132, 1132},//零序过压保护返回
    {21, FailureInf1133, 1133},//高侧A过负荷Ⅰ告警
    {21, FailureInf1134, 1134},//高侧B过负荷Ⅰ告警
    {21, FailureInf1135, 1135},//高侧C过负荷Ⅰ告警
    {1, FailureInf1136, 1136},//高侧A过负荷Ⅱ启动
    {21, FailureInf1137, 1137},//高侧A过负荷Ⅱ动作
    {1, FailureInf1138, 1138},//高侧A过负荷Ⅱ返回
    {1, FailureInf1139, 1139},//高侧B过负荷Ⅱ启动
    {21, FailureInf1140, 1140},//高侧B过负荷Ⅱ动作
    {1, FailureInf1141, 1141},//高侧B过负荷Ⅱ返回
    {1, FailureInf1142, 1142},//高侧C过负荷Ⅱ启动
    {21, FailureInf1143, 1143},//高侧C过负荷Ⅱ动作
    {1, FailureInf1144, 1144},//高侧C过负荷Ⅱ返回
    {21, FailureInf1145, 1145},//高压侧失压动作
    {21, FailureInf1146, 1146},//高压侧PT断线
    {21, FailureInf1147, 1147},//高压侧PT断线恢复
    {21, FailureInf1148, 1148},//低压侧A相PT断线
    {21, FailureInf1149, 1149},//低压侧A相PT断线恢复
    {21, FailureInf1150, 1150},//低压侧B相PT断线
    {21, FailureInf1151, 1151},//低压侧B相PT断线恢复
    {21, FailureInf1152, 1152},//保护启动
    {8, FailureInf1153, 1153},//高侧断路器跳闸失败
    {21, FailureInf1154, 1154},//通道测试
    {0, FailureInf1155, 1155},//失灵保护
    {4, FailureInf1156, 1156},//低侧A相断路器跳闸失败
    {4, FailureInf1157, 1157},//低侧B相断路器跳闸失败
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 248) ,
	 (LONG*)(AS_BUF_ADDR + 252) ,
	 (LONG*)(AS_BUF_ADDR + 256) ,
	 (LONG*)(AS_BUF_ADDR + 260) ,
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
	 (WORD)(68) ,
	 (WORD)(69) ,
	 (WORD)(70) ,
	 (WORD)(71) ,
	 (WORD)(72) ,
	 (WORD)(73) ,
	 (WORD)(74) ,
	 (WORD)(75) ,
	 (WORD)(76) ,
	 (WORD)(77) ,
	 (WORD)(78) ,
	 (WORD)(79) ,
	 (WORD)(80) ,
	 (WORD)(81) ,
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
	 (WORD)(30) ,
	 (WORD)(31) ,
	 (WORD)(32) ,
	 (WORD)(33) ,
	 (WORD)(34) ,
	 (WORD)(35) ,
	 (WORD)(36) ,
	 (WORD)(37) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 7) ,
	 (BYTE*)(CFG_BUF_ADDR + 8) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 6) ,
	 (BYTE*)(CFG_BUF_ADDR + 9) ,
	 (BYTE*)(CFG_BUF_ADDR + 22) ,
	 (BYTE*)(CFG_BUF_ADDR + 23) ,
	 (BYTE*)(CFG_BUF_ADDR + 24) ,
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	11500, //UHAB
	11500, //UHBC
	11500, //UHCA
	11500, //UHA
	11500, //UHB
	11500, //UHC
	11500, //U0
	11500, //ULA
	11500, //ULB
	3050, //IHA
	3050, //IHB
	3050, //IHC
	3050, //I0
	3050, //IL1
	3050, //IL2
	3050, //IL3
	3050, //IL4
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	16100, //UHAM
	16100, //UHBM
	16100, //UHCM
	16100, //U0M
	16100, //ULAM
	16100, //ULBM
	4334, //IHAM
	4334, //IHBM
	4334, //IHCM
	4334, //I0M
	4334, //IL1M
	4334, //IL2M
	4334, //IL3M
	4334, //IL4M
	21, //UHAD
	21, //UHBD
	21, //UHCD
	21, //U0D
	21, //ULAD
	21, //ULBD
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //I0D
	21, //IL1D
	21, //IL2D
	21, //IL3D
	21, //IL4D
};
//系数2
const LONG CFDefaultVal2[]=
{
	11500, //UHAB
	11500, //UHBC
	11500, //UHCA
	11500, //UHA
	11500, //UHB
	11500, //UHC
	11500, //U0
	11500, //ULA
	11500, //ULB
	15250, //IHA
	15250, //IHB
	15250, //IHC
	15250, //I0
	15250, //IL1
	15250, //IL2
	15250, //IL3
	15250, //IL4
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	16100, //UHAM
	16100, //UHBM
	16100, //UHCM
	16100, //U0M
	16100, //ULAM
	16100, //ULBM
	21670, //IHAM
	21670, //IHBM
	21670, //IHCM
	21670, //I0M
	21670, //IL1M
	21670, //IL2M
	21670, //IL3M
	21670, //IL4M
	21, //UHAD
	21, //UHBD
	21, //UHCD
	21, //U0D
	21, //ULAD
	21, //ULBD
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //I0D
	21, //IL1D
	21, //IL2D
	21, //IL3D
	21, //IL4D
};

//传动----------------------------------
