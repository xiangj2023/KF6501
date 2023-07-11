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
};
//辅助定值取值信息
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{BTMODE, "直供供电"},
	{ATMODE, "AT供电"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //辅助定值额定电流
	{2, (TSetValueInf*)&(SysSetValueInf[2])}, //辅助定值供电方式
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnUp 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// U
LONG* 	RM_pnU1 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// U1
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// U2
LONG* 	RM_pnUd1 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// Ud1
LONG* 	RM_pnUd2 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// Ud2
LONG* 	RM_pnUd3 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// Ud3
LONG* 	RM_pnIp 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// I
LONG* 	RM_pnIm 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// Im
LONG* 	RM_pnI1 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// I1
LONG* 	RM_pnI2 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// I2
LONG* 	RM_pnI3 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// I3
LONG* 	RM_pnIh3 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Ih3
LONG* 	RM_pnIh5 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Ih5
LONG* 	RM_pnIh7 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// Ih7
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUM 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//U U
LONG* 	M_pnU1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//U1 U1
LONG* 	M_pnU2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//U2 U2
LONG* 	M_pnUd1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//Ud1 Ud1
LONG* 	M_pnUd2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Ud2 Ud2
LONG* 	M_pnUd3M 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ud3 Ud3
LONG* 	M_pnIM 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//I I
LONG* 	M_pnI1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//I1 I1
LONG* 	M_pnI2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//I2 I2
LONG* 	M_pnI3M 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//I3 I3
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//COS COS
LONG* 	M_pnIH1 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//IH1 IH1
LONG* 	M_pnIH2 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//IH2 IH2
LONG* 	M_pnIH3 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//IH3 IH3
LONG* 	M_pnIH5 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//IH5 IH5
LONG* 	M_pnIH7 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//IH7 IH7
LONG* 	M_pnIH9 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//IH9 IH9
LONG* 	M_pnIH11 	= 	(LONG*)(MEASURE_BUF_ADDR + 76); 	//IH11 IH11


//软压板----------------------------------
BYTE* SW_pby50_0 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//电流速断
BYTE* SW_pby50_1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//过流保护
BYTE* SW_pby59 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//过压保护
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//低压保护
BYTE* SW_pby50_H 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//谐波过流保护
BYTE* SW_pby50ST 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//支路过流保护
BYTE* SW_pby60I 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//差流保护
BYTE* SW_pby60U 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//差压保护
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby50_0 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//速断保护
BYTE* CFG_pby50_1 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//过流保护
BYTE* CFG_pby50_H 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//谐波过流保护
BYTE* CFG_pby59 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//过压保护
BYTE* CFG_pby27 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//低压保护
BYTE* CFG_pby60I1 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//支路1差流
BYTE* CFG_pby60I2 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//支路2差流
BYTE* CFG_pby50ST 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//支路过流
BYTE* CFG_pby60U1 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//支路1差压
BYTE* CFG_pby60U2 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//支路2差压
BYTE* CFG_pby60U3 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//支路3差压
BYTE* CFG_pby74PT 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//PT断线检查
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//CT断线检查
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//失灵保护
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//备用配置2

//定值----------------------------------
LONG* RS_pnF 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字 F
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//突变量启动电流 PIC
LONG* RS_pn50_0C 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//速断电流定值 50_0C
LONG* RS_pn50_0D 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//速断时间定值 50_0D
LONG* RS_pn50_1C 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//过流电流定值 50_1C
LONG* RS_pn50_1D 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//过流时间定值 50_1D
LONG* RS_pn50_HC 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//谐波过流电流定值 50_HC
LONG* RS_pn50_HD 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//谐波过流时间定值 50_HD
LONG* RS_pn59V 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//过压保护电压 59V
LONG* RS_pn59D 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//过压保护时间 59D
LONG* RS_pn27V 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//低压保护电压 27V
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//低压保护时间 27D
LONG* RS_pn27_LK50 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//低压保护电流闭锁 27_LK50
LONG* RS_pn60I1C 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//支路1差流保护电流 60I1C
LONG* RS_pn60I1D 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//支路1差流保护时间 60I1D
LONG* RS_pn60I2C 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//支路2差流保护电流 60I2C
LONG* RS_pn60I2D 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//支路2差流保护时间 60I2D
LONG* RS_pn50STC 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//支路过流保护电流 50STC
LONG* RS_pn50STD 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//支路过流保护时间 50STD
LONG* RS_pn60U1V 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//支路1差压保护电压 60U1V
LONG* RS_pn60U1D 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//支路1差压保护时间 60U1D
LONG* RS_pn60U2V 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//支路2差压保护电压 60U2V
LONG* RS_pn60U2D 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//支路2差压保护时间 60U2D
LONG* RS_pn60U3V 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//支路3差压保护电压 60U3V
LONG* RS_pn60U3D 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//支路3差压保护时间 60U3D
LONG* RS_pn74PTC 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//PT断线检测电流 74PTC
LONG* RS_pn74PTV 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//PT断线检测电压 74PTV
LONG* RS_pn74PTD 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//PT断线检测时间 74PTD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//失灵保护时间 52BFD

//辅助定值----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//额定电流
LONG* AS_pnPM 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//供电方式
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
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//隔开动作延时
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//TR2保持时间
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//TR3保持时间
LONG* AS_pn50IP 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IP有流定值
LONG* AS_pn37IP 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//IP无流定值
LONG* AS_pn50I1 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//I1有流定值
LONG* AS_pn37I1 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//I1无流定值
LONG* AS_pn50I2 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//I2有流定值
LONG* AS_pn37I2 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//I2无流定值
LONG* AS_pn50I3 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//I3有流定值
LONG* AS_pn37I3 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//I3无流定值
LONG* AS_pn59UP 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//Up有压定值
LONG* AS_pn27UP 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//Up无压定值
LONG* AS_pn59U1 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U1有压定值
LONG* AS_pn27U1 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U1无压定值
LONG* AS_pn59U2 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U2有压定值
LONG* AS_pn27U2 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//U2无压定值
LONG* AS_pn59Ud1 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//Ud1有压定值
LONG* AS_pn27Ud1 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//Ud1无压定值
LONG* AS_pn59Ud2 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//Ud2有压定值
LONG* AS_pn27Ud2 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//Ud2无压定值
LONG* AS_pn59Ud3 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//Ud3有压定值
LONG* AS_pn27Ud3 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//Ud3无压定值
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnU1PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//U1PT变比
LONG* AS_pnU2PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//U2PT变比
LONG* AS_pnUd1PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//Ud1PT变比
LONG* AS_pnUd2PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//Ud2PT变比
LONG* AS_pnUd3PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//Ud3PT变比
LONG* AS_pnIPCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//保护CT变比
LONG* AS_pnIMCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//测量CT变比
LONG* AS_pnId1CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//Id1CT变比
LONG* AS_pnId2CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//Id2CT变比
LONG* AS_pnId3CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//Id3CT变比
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//rt1

//系数----------------------------------
LONG* CF_pnUP 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UP
LONG* CF_pnU1P 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U1P
LONG* CF_pnU2P 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U2P
LONG* CF_pnUd1P 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//Ud1P
LONG* CF_pnUd2P 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//Ud2P
LONG* CF_pnUd3P 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//Ud3P
LONG* CF_pnIP 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//IP
LONG* CF_pnI1P 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//I1P
LONG* CF_pnI2P 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//I2P
LONG* CF_pnI3P 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//I3P
LONG* CF_pnUM 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//UM
LONG* CF_pnU1M 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//U1M
LONG* CF_pnU2M 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//U2M
LONG* CF_pnUd1M 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//Ud1M
LONG* CF_pnUd2M 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//Ud2M
LONG* CF_pnUd3M 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//Ud3M
LONG* CF_pnIM 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IM
LONG* CF_pnI1M 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//I1M
LONG* CF_pnI2M 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//I2M
LONG* CF_pnI3M 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//I3M
LONG* CF_pnP 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//P
LONG* CF_pnQ 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//Q
LONG* CF_pnA 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//A
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//C4
LONG* CF_pnDRIFTU 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//DRIFTU
LONG* CF_pnDRIFTU1 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//DRIFTU1
LONG* CF_pnDRIFTU2 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//DRIFTU2
LONG* CF_pnDRIFTUD1 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//DRIFTUD1
LONG* CF_pnDRIFTUD2 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//DRIFTUD2
LONG* CF_pnDRIFTUD3 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//DRIFTUD3
LONG* CF_pnDRIFTIM 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//DRIFTIM
LONG* CF_pnDRIFTI1 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//DRIFTI1
LONG* CF_pnDRIFTI2 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//DRIFTI2
LONG* CF_pnDRIFTI3 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//DRIFTI3

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 64), // IN1
    (LONG*)(RM_BUF_ADDR + 68) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 56), // BR1
    (LONG*)(RM_BUF_ADDR + 60) // BR2
};

//AD故障3----------
const  LONG* FailureInf3[] = {
    {NULL}
};

//软压板故障4----------
const  LONG* FailureInf4[] = {
    {NULL}
};

//EEPROM定值出错5----------
const  LONG* FailureInf5[] = {
    {NULL}
};

//RAM定值出错8----------
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

//外部RAM故障15----------
const  LONG* FailureInf15[] = {
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

//逻辑方程故障故障恢复63----------
const  LONG* FailureInf63[] = {
    {NULL}
};

//保护启动500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//过压保护启动3000----------
const  LONG* FailureInf3000[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//过压保护动作3001----------
const  LONG* FailureInf3001[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//过压保护返回3002----------
const  LONG* FailureInf3002[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//谐波过流保护启动3003----------
const  LONG* FailureInf3003[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//谐波过流保护动作3004----------
const  LONG* FailureInf3004[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//谐波过流保护返回3005----------
const  LONG* FailureInf3005[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路1差流保护启动3006----------
const  LONG* FailureInf3006[] = {
    (LONG*)(RM_BUF_ADDR + 32) // I1
};

//支路1差流保护动作3007----------
const  LONG* FailureInf3007[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路1差流保护返回3008----------
const  LONG* FailureInf3008[] = {
    (LONG*)(RM_BUF_ADDR + 32) // I1
};

//支路2差流保护启动3009----------
const  LONG* FailureInf3009[] = {
    (LONG*)(RM_BUF_ADDR + 36) // I2
};

//支路2差流保护动作3010----------
const  LONG* FailureInf3010[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路2差流保护返回3011----------
const  LONG* FailureInf3011[] = {
    (LONG*)(RM_BUF_ADDR + 36) // I2
};

//支路过流保护启动3012----------
const  LONG* FailureInf3012[] = {
    (LONG*)(RM_BUF_ADDR + 40) // I3
};

//支路过流保护动作3013----------
const  LONG* FailureInf3013[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路过流保护返回3014----------
const  LONG* FailureInf3014[] = {
    (LONG*)(RM_BUF_ADDR + 40) // I3
};

//支路1差压保护启动3015----------
const  LONG* FailureInf3015[] = {
    (LONG*)(RM_BUF_ADDR + 12) // Ud1
};

//支路1差压保护动作3016----------
const  LONG* FailureInf3016[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路1差压保护返回3017----------
const  LONG* FailureInf3017[] = {
    (LONG*)(RM_BUF_ADDR + 12) // Ud1
};

//支路2差压保护启动3018----------
const  LONG* FailureInf3018[] = {
    (LONG*)(RM_BUF_ADDR + 16) // Ud2
};

//支路2差压保护动作3019----------
const  LONG* FailureInf3019[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路2差压保护返回3020----------
const  LONG* FailureInf3020[] = {
    (LONG*)(RM_BUF_ADDR + 16) // Ud2
};

//支路3差压保护启动3021----------
const  LONG* FailureInf3021[] = {
    (LONG*)(RM_BUF_ADDR + 20) // Ud3
};

//支路3差压保护动作3022----------
const  LONG* FailureInf3022[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//支路3差压保护返回3023----------
const  LONG* FailureInf3023[] = {
    (LONG*)(RM_BUF_ADDR + 20) // Ud3
};

//电流速断启动3024----------
const  LONG* FailureInf3024[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//电流速断动作3025----------
const  LONG* FailureInf3025[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//电流速断返回3026----------
const  LONG* FailureInf3026[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//过流保护启动3027----------
const  LONG* FailureInf3027[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//过流保护动作3028----------
const  LONG* FailureInf3028[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//过流保护返回3029----------
const  LONG* FailureInf3029[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//低压保护动作3030----------
const  LONG* FailureInf3030[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//PT断线3031----------
const  LONG* FailureInf3031[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//PT断线恢复3032----------
const  LONG* FailureInf3032[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//CT断线3033----------
const  LONG* FailureInf3033[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 28) // Im
};

//CT断线恢复3034----------
const  LONG* FailureInf3034[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 28) // Im
};

//断路器跳闸失败3035----------
const  LONG* FailureInf3035[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//通道测试3036----------
const  LONG* FailureInf3036[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//保护启动3037----------
const  LONG* FailureInf3037[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
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

//保护数据异常22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//保护数据异常恢复65----------
const  LONG* FailureInf65[] = {
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

//报告引用定义------------------------------------------
const struct TReportInf ReportInf[] = {
    {2, FailureInf1, 1},//开出光隔失效
    {2, FailureInf2, 2},//开出光隔击穿
    {0, FailureInf3, 3},//AD故障
    {0, FailureInf4, 4},//软压板故障
    {0, FailureInf5, 5},//EEPROM定值出错
    {0, FailureInf8, 8},//RAM定值出错
    {0, FailureInf11, 11},//程序ROM故障
    {0, FailureInf13, 13},//装置异常复位
    {0, FailureInf14, 14},//通信通道异常
    {0, FailureInf15, 15},//外部RAM故障
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
    {0, FailureInf63, 63},//逻辑方程故障故障恢复
    {0, FailureInf500, 500},//保护启动
    {2, FailureInf3000, 3000},//过压保护启动
    {13, FailureInf3001, 3001},//过压保护动作
    {2, FailureInf3002, 3002},//过压保护返回
    {3, FailureInf3003, 3003},//谐波过流保护启动
    {13, FailureInf3004, 3004},//谐波过流保护动作
    {3, FailureInf3005, 3005},//谐波过流保护返回
    {1, FailureInf3006, 3006},//支路1差流保护启动
    {13, FailureInf3007, 3007},//支路1差流保护动作
    {1, FailureInf3008, 3008},//支路1差流保护返回
    {1, FailureInf3009, 3009},//支路2差流保护启动
    {13, FailureInf3010, 3010},//支路2差流保护动作
    {1, FailureInf3011, 3011},//支路2差流保护返回
    {1, FailureInf3012, 3012},//支路过流保护启动
    {13, FailureInf3013, 3013},//支路过流保护动作
    {1, FailureInf3014, 3014},//支路过流保护返回
    {1, FailureInf3015, 3015},//支路1差压保护启动
    {13, FailureInf3016, 3016},//支路1差压保护动作
    {1, FailureInf3017, 3017},//支路1差压保护返回
    {1, FailureInf3018, 3018},//支路2差压保护启动
    {13, FailureInf3019, 3019},//支路2差压保护动作
    {1, FailureInf3020, 3020},//支路2差压保护返回
    {1, FailureInf3021, 3021},//支路3差压保护启动
    {13, FailureInf3022, 3022},//支路3差压保护动作
    {1, FailureInf3023, 3023},//支路3差压保护返回
    {1, FailureInf3024, 3024},//电流速断启动
    {13, FailureInf3025, 3025},//电流速断动作
    {1, FailureInf3026, 3026},//电流速断返回
    {1, FailureInf3027, 3027},//过流保护启动
    {13, FailureInf3028, 3028},//过流保护动作
    {1, FailureInf3029, 3029},//过流保护返回
    {13, FailureInf3030, 3030},//低压保护动作
    {4, FailureInf3031, 3031},//PT断线
    {4, FailureInf3032, 3032},//PT断线恢复
    {3, FailureInf3033, 3033},//CT断线
    {3, FailureInf3034, 3034},//CT断线恢复
    {1, FailureInf3035, 3035},//断路器跳闸失败
    {13, FailureInf3036, 3036},//通道测试
    {13, FailureInf3037, 3037},//保护启动
    {0, FailureInf20, 20},//5V电源故障
    {0, FailureInf21, 21},//EEPROM写入故障
    {0, FailureInf64, 64},//5V电源故障恢复
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
    {0, FailureInf501, 501},//重瓦斯
    {0, FailureInf502, 502},//压力释放
    {0, FailureInf503, 503},//超温跳闸
    {0, FailureInf504, 504},//轻瓦斯
    {0, FailureInf505, 505},//油位异常
    {0, FailureInf506, 506},//温度过高
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
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
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 112) },
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
	 (BYTE*)(SW_BUF_ADDR + 0) ,
	 (BYTE*)(SW_BUF_ADDR + 8) ,
	 (BYTE*)(SW_BUF_ADDR + 9) ,
	 (BYTE*)(SW_BUF_ADDR + 10) ,
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
	 (WORD)(0) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(10) ,
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
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	11400, //UP
	11400, //U1P
	11400, //U2P
	30400, //Ud1P
	30400, //Ud2P
	30400, //Ud3P
	3050, //IP
	3060, //I1P
	3040, //I2P
	3040, //I3P
	16100, //UM
	16100, //U1M
	16100, //U2M
	42900, //Ud1M
	42900, //Ud2M
	42900, //Ud3M
	64500, //IM
	4310, //I1M
	4290, //I2M
	4300, //I3M
	346400, //P
	346400, //Q
	116, //A
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	1, //DRIFTU
	1, //DRIFTU1
	1, //DRIFTU2
	1, //DRIFTUD1
	1, //DRIFTUD2
	1, //DRIFTUD3
	1, //DRIFTIM
	1, //DRIFTI1
	1, //DRIFTI2
	1, //DRIFTI3
};
//系数2
const LONG CFDefaultVal2[]=
{
	11400, //UP
	11400, //U1P
	11400, //U2P
	30400, //Ud1P
	30400, //Ud2P
	30400, //Ud3P
	15250, //IP
	15250, //I1P
	15240, //I2P
	15250, //I3P
	16100, //UM
	16100, //U1M
	16100, //U2M
	42900, //Ud1M
	42900, //Ud2M
	42900, //Ud3M
	322500, //IM
	21500, //I1M
	21500, //I2M
	21500, //I3M
	346400, //P
	346400, //Q
	116, //A
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	1, //DRIFTU
	1, //DRIFTU1
	1, //DRIFTU2
	1, //DRIFTUD1
	1, //DRIFTUD2
	1, //DRIFTUD3
	1, //DRIFTIM
	1, //DRIFTI1
	1, //DRIFTI2
	1, //DRIFTI3
};

//传动----------------------------------
