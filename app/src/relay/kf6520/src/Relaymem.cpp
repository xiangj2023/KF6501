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
	{FOREWORD, "正方向"},
	{BACKWORD, "反方向"},
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
	{2, (TSetValueInf*)&(SetValueInf[0])}, //定值公里标方向
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
	{BTMODE, "直供"},
	{AT1MODE, "AT"},
	{AT2MODE, "客专"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //辅助定值额定电流
	{3, (TSetValueInf*)&(SysSetValueInf[2])}, //辅助定值供电方式
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnIp 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// Ip
LONG* 	RM_pnIt 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// It
LONG* 	RM_pnIf 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// If
LONG* 	RM_pnUp 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// Up
LONG* 	RM_pnU1 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// U1
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// U2
LONG* 	RM_pnU3 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// U3
LONG* 	RM_pnU4 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// U4
LONG* 	RM_pnTL 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// TL
LONG* 	RM_pnFL 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// FL
LONG* 	RM_pnSS 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// SS
LONG* 	RM_pnI2 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// I2
LONG* 	RM_pnI3 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// I3
LONG* 	RM_pnI5 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// I5
LONG* 	RM_pnItm 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// Itm
LONG* 	RM_pnIfm 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// Ifm
LONG* 	RM_pnR 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// R
LONG* 	RM_pnX 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// X
LONG* 	RM_pnZA 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// ΦZ
LONG* 	RM_pnTR 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// TR
LONG* 	RM_pnTX 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// TX
LONG* 	RM_pnTZA 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// ΦT
LONG* 	RM_pnFR 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// FR
LONG* 	RM_pnFX 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// FX
LONG* 	RM_pnFZA 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// ΦF
LONG* 	RM_pnAUP 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// ΦUP
LONG* 	RM_pnAU1 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// ΦU1
LONG* 	RM_pnAU2 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// ΦU2
LONG* 	RM_pnAIP 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// ΦIP
LONG* 	RM_pnAIT 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// ΦIT
LONG* 	RM_pnAIF 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// ΦIF
LONG* 	RM_pnAITM 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// ΦItM
LONG* 	RM_pnAIFM 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// ΦIfM
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnU1 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//U1 U1
LONG* 	M_pnU2 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//U2 U2
LONG* 	M_pnU3 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//U3 U3
LONG* 	M_pnU4 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U4 U4
LONG* 	M_pnIT 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//IT IT
LONG* 	M_pnIF 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//IF IF
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//COS COS
LONG* 	M_pnI1 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//I1 I1
LONG* 	M_pnI2 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//I2 I2
LONG* 	M_pnI3 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//I3 I3
LONG* 	M_pnI5 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//I5 I5
LONG* 	M_pnI7 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I7 I7
LONG* 	M_pnI9 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I9 I9
LONG* 	M_pnI11 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//I11 I11
LONG* 	M_pnR 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//R R
LONG* 	M_pnX 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//X X
LONG* 	M_pnF 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//F F


//软压板----------------------------------
BYTE* SW_pby21 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//距离保护
BYTE* SW_pby50_0 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//电流速断
BYTE* SW_pby50_123 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//过流保护
BYTE* SW_pby50D 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//电流增量保护
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//低压保护
BYTE* SW_pby79 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//重合闸
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//检修压板
BYTE* SW_pbyRESD1 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//备用压板1
BYTE* SW_pbyRESD2 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby21_1 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//距离I段
BYTE* CFG_pby21_1_D 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//距离I段正向
BYTE* CFG_pby21_2 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//距离II段
BYTE* CFG_pby21_2_D 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//距离II段正向
BYTE* CFG_pby21_3 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//距离III段
BYTE* CFG_pby21_3_D 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//距离III段正向
BYTE* CFG_pby21_4 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//距离IV段
BYTE* CFG_pby21_4_D 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//距离IV段正向
BYTE* CFG_pbyParallelC 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//平行四边形特性
BYTE* CFG_pby21_HLK 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//距离谐波闭锁
BYTE* CFG_pby21_HRES 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//距离谐波抑制
BYTE* CFG_pby50_0 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//电流速断
BYTE* CFG_pby50_0_27 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//速断低压闭锁
BYTE* CFG_pby50_0_HLK 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//速断谐波闭锁
BYTE* CFG_pby50_0_HRES 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//速断谐波抑制
BYTE* CFG_pby50_0_D 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//速断保护正方向
BYTE* CFG_pby50_0_67 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//速断功率方向
BYTE* CFG_pby50_1 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//过流I段
BYTE* CFG_pby50_1_D 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//过流I段正方向
BYTE* CFG_pby50_1_67 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//过流I段功率方向
BYTE* CFG_pby50_1_27 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//过流I段低压闭锁
BYTE* CFG_pby50_2 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//过流II段
BYTE* CFG_pby50_2_D 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//过流II段正方向
BYTE* CFG_pby50_2_67 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//过流II段功率方向
BYTE* CFG_pby50_2_27 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//过流II段低压闭锁
BYTE* CFG_pby50_3 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//过流III段
BYTE* CFG_pby50_HLK 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//过流谐波闭锁
BYTE* CFG_pby50_HRES 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//过流谐波抑制
BYTE* CFG_pby51 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//反时限过流
BYTE* CFG_pby50D 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//电流增量保护
BYTE* CFG_pby79UC 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//偷跳重合闸
BYTE* CFG_pby79_21 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//距离保护重合
BYTE* CFG_pby79_50_0 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//电流速断重合
BYTE* CFG_pby79_50_123 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//过流保护重合
BYTE* CFG_pby79_51 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//反时限重合
BYTE* CFG_pby79_50D 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//增量保护重合
BYTE* CFG_pby79_59 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//重合闸检有压
BYTE* CFG_pby21_2_94 	= 	(BYTE*)(CFG_BUF_ADDR + 37); 	//距离II段加速
BYTE* CFG_pby21_3_94 	= 	(BYTE*)(CFG_BUF_ADDR + 38); 	//距离III段加速
BYTE* CFG_pby21_4_94 	= 	(BYTE*)(CFG_BUF_ADDR + 39); 	//距离IV段加速
BYTE* CFG_pby50_1_94 	= 	(BYTE*)(CFG_BUF_ADDR + 40); 	//过流I段加速
BYTE* CFG_pby50_2_94 	= 	(BYTE*)(CFG_BUF_ADDR + 41); 	//过流II段加速
BYTE* CFG_pby50_3_94 	= 	(BYTE*)(CFG_BUF_ADDR + 42); 	//过流III段加速
BYTE* CFG_pby27 	= 	(BYTE*)(CFG_BUF_ADDR + 43); 	//低压保护
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 44); 	//失灵保护
BYTE* CFG_pbyPT1BR 	= 	(BYTE*)(CFG_BUF_ADDR + 45); 	//PT1断线检查
BYTE* CFG_pbyPT2BR 	= 	(BYTE*)(CFG_BUF_ADDR + 46); 	//PT2断线检查
BYTE* CFG_pby21_FL 	= 	(BYTE*)(CFG_BUF_ADDR + 47); 	//故障测距
BYTE* CFG_pbyLOADREC 	= 	(BYTE*)(CFG_BUF_ADDR + 48); 	//负荷录波
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 49); 	//CT断线检查
BYTE* CFG_pbyRESD1 	= 	(BYTE*)(CFG_BUF_ADDR + 50); 	//备用配置1
BYTE* CFG_pbyRESD2 	= 	(BYTE*)(CFG_BUF_ADDR + 51); 	//备用配置2

//定值----------------------------------
LONG* RS_pnFUNCTION1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字1 FUNCTION1
LONG* RS_pnFUNCTION2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//配置字2 FUNCTION2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//突变量启动电流 PIC
LONG* RS_pn21_1R 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//距离I段电阻 21_1R
LONG* RS_pn21_1X 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//距离I段电抗 21_1X
LONG* RS_pn21_1D 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//距离I段时间 21_1D
LONG* RS_pn21_2R 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//距离II段电阻 21_2R
LONG* RS_pn21_2X 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//距离II段电抗 21_2X
LONG* RS_pn21_2D 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//距离II段时间 21_2D
LONG* RS_pn21_3R 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//距离III段电阻 21_3R
LONG* RS_pn21_3X 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//距离III段电抗 21_3X
LONG* RS_pn21_3D 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//距离III段时间 21_3D
LONG* RS_pn21_4R 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//距离IV段电阻 21_4R
LONG* RS_pn21_4X 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//距离IV段电抗 21_4X
LONG* RS_pn21_4D 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//距离IV段时间 21_4D
LONG* RS_pn21ZOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//平行四边形偏移阻抗 21ZOFFSET
LONG* RS_pn21AOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//阻抗四边形偏移角度 21AOFFSET
LONG* RS_pn50_0C 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//电流速断电流 50_0C
LONG* RS_pn50_0D 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//电流速断时间 50_0D
LONG* RS_pn50_0_27V 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//速断低压闭锁 50_0_27V
LONG* RS_pn50_1C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//过流I段电流 50_1C
LONG* RS_pn50_1D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//过流I段时间 50_1D
LONG* RS_pn50_1_27V 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//过流I段低压闭锁 50_1_27V
LONG* RS_pn50_2C 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//过流II段电流 50_2C
LONG* RS_pn50_2D 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//过流II段时间 50_2D
LONG* RS_pn50_2_27V 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//过流II段低压闭锁 50_2_27V
LONG* RS_pn50_3C 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//过流III段电流 50_3C
LONG* RS_pn50_3D 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//过流III段时间 50_3D
LONG* RS_pn51C 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//反时限过流电流 51C
LONG* RS_pn51TD 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//反时限过流时间 51TD
LONG* RS_pn51TYPE 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//反时限曲线类型 51TYPE
LONG* RS_pn50DC 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//电流增量保护电流 50DC
LONG* RS_pn50DD 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//电流增量保护时间 50DD
LONG* RS_pn50D_37C 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//增量保护小电流闭锁 50D_37C
LONG* RS_pnH2_PCT 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//二次谐波闭锁系数 H2_PCT
LONG* RS_pnFACTOR1 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//距离过流谐波加权 FACTOR1
LONG* RS_pnFACTOR2 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//增量保护谐波加权 FACTOR2
LONG* RS_pnZANGLE 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//线路阻抗角 ZANGLE
LONG* RS_pn79SH 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//重合闸投入次数 79SH
LONG* RS_pn79D1 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//第一次重合闸时间 79D1
LONG* RS_pn79D2 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//第二次重合闸时间 79D2
LONG* RS_pn79RD 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//重合闸充电时间 79RD
LONG* RS_pn79CLSD 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//重合闸动作监视时间 79CLSD
LONG* RS_pn94D 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//后加速动作时间 94D
LONG* RS_pnSOTFD 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//后加速投入时间 SOTFD
LONG* RS_pn27V 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//低压保护电压 27V
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//低压保护时间 27D
LONG* RS_pn27_37C 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//低压闭锁电流 27_37C
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//失灵保护时间 52BFD
LONG* RS_pn74VTV 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//PT断线检测电压 74VTV
LONG* RS_pn74VTBLC 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//PT断线闭锁电流 74VTBLC
LONG* RS_pn74VTC 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//PT断线检测电流 74VTC
LONG* RS_pn74VTD 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//PT断线检测时间 74VTD
LONG* RS_pnS0 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//公里标 S0
LONG* RS_pnSD 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//公里标方向 SD
LONG* RS_pnLc 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//修正距离 Lc
LONG* RS_pnT21FLN 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//T线测距分段数 T21FLN
LONG* RS_pnT21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//T线第一段距离 T21FLN1L
LONG* RS_pnT21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//T线第一段单位电抗 T21FLN1X
LONG* RS_pnT21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//T线第二段距离 T21FLN2L
LONG* RS_pnT21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//T线第二段单位电抗 T21FLN2X
LONG* RS_pnT21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//T线第三段距离 T21FLN3L
LONG* RS_pnT21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//T线第三段单位电抗 T21FLN3X
LONG* RS_pnT21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//T线第四段距离 T21FLN4L
LONG* RS_pnT21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//T线第四段单位电抗 T21FLN4X
LONG* RS_pnT21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//T线第五段距离 T21FLN5L
LONG* RS_pnT21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//T线第五段单位电抗 T21FLN5X
LONG* RS_pnT21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//T线第六段距离 T21FLN6L
LONG* RS_pnT21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//T线第六段单位电抗 T21FLN6X
LONG* RS_pnT21FLN7L 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//T线第七段距离 T21FLN7L
LONG* RS_pnT21FLN7X 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//T线第七段单位电抗 T21FLN7X
LONG* RS_pnT21FLN8L 	= 	(LONG*)(RS_BUF_ADDR + 284); 	//T线第八段距离 T21FLN8L
LONG* RS_pnT21FLN8X 	= 	(LONG*)(RS_BUF_ADDR + 288); 	//T线第八段单位电抗 T21FLN8X
LONG* RS_pnT21FLN9L 	= 	(LONG*)(RS_BUF_ADDR + 292); 	//T线第九段距离 T21FLN9L
LONG* RS_pnT21FLN9X 	= 	(LONG*)(RS_BUF_ADDR + 296); 	//T线第九段单位电抗 T21FLN9X
LONG* RS_pnT21FLN10L 	= 	(LONG*)(RS_BUF_ADDR + 300); 	//T线第十段距离 T21FLN10L
LONG* RS_pnT21FLN10X 	= 	(LONG*)(RS_BUF_ADDR + 304); 	//T线第十段单位电抗 T21FLN10X
LONG* RS_pnF21FLN 	= 	(LONG*)(RS_BUF_ADDR + 308); 	//F线测距分段数 F21FLN
LONG* RS_pnF21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 312); 	//F线第一段距离 F21FLN1L
LONG* RS_pnF21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 316); 	//F线第一段单位电抗 F21FLN1X
LONG* RS_pnF21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 320); 	//F线第二段距离 F21FLN2L
LONG* RS_pnF21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 324); 	//F线第二段单位电抗 F21FLN2X
LONG* RS_pnF21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 328); 	//F线第三段距离 F21FLN3L
LONG* RS_pnF21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 332); 	//F线第三段单位电抗 F21FLN3X
LONG* RS_pnF21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 336); 	//F线第四段距离 F21FLN4L
LONG* RS_pnF21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 340); 	//F线第四段单位电抗 F21FLN4X
LONG* RS_pnF21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 344); 	//F线第五段距离 F21FLN5L
LONG* RS_pnF21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 348); 	//F线第五段单位电抗 F21FLN5X
LONG* RS_pnF21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 352); 	//F线第六段距离 F21FLN6L
LONG* RS_pnF21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 356); 	//F线第六段单位电抗 F21FLN6X
LONG* RS_pnF21FLN7L 	= 	(LONG*)(RS_BUF_ADDR + 360); 	//F线第七段距离 F21FLN7L
LONG* RS_pnF21FLN7X 	= 	(LONG*)(RS_BUF_ADDR + 364); 	//F线第七段单位电抗 F21FLN7X
LONG* RS_pnF21FLN8L 	= 	(LONG*)(RS_BUF_ADDR + 368); 	//F线第八段距离 F21FLN8L
LONG* RS_pnF21FLN8X 	= 	(LONG*)(RS_BUF_ADDR + 372); 	//F线第八段单位电抗 F21FLN8X
LONG* RS_pnF21FLN9L 	= 	(LONG*)(RS_BUF_ADDR + 376); 	//F线第九段距离 F21FLN9L
LONG* RS_pnF21FLN9X 	= 	(LONG*)(RS_BUF_ADDR + 380); 	//F线第九段单位电抗 F21FLN9X
LONG* RS_pnF21FLN10L 	= 	(LONG*)(RS_BUF_ADDR + 384); 	//F线第十段距离 F21FLN10L
LONG* RS_pnF21FLN10X 	= 	(LONG*)(RS_BUF_ADDR + 388); 	//F线第十段单位电抗 F21FLN10X

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
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//电隔动作延时
LONG* AS_pn50IPC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IP有流定值
LONG* AS_pn37IPC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IP无流定值
LONG* AS_pn50ITC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//It有流定值
LONG* AS_pn37ITC 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//It无流定值
LONG* AS_pn50IFC 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//If有流定值
LONG* AS_pn37IFC 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//If无流定值
LONG* AS_pn59UPV 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//Up有压定值
LONG* AS_pn27UPV 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//Up无压定值
LONG* AS_pn59U1V 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//U1有压定值
LONG* AS_pn27U1V 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//U1无压定值
LONG* AS_pn59U2V 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//U2有压定值
LONG* AS_pn27U2V 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//U2无压定值
LONG* AS_pn59U3V 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//U3有压定值
LONG* AS_pn27U3V 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U3无压定值
LONG* AS_pn59U4V 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U4有压定值
LONG* AS_pn27U4V 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U4无压定值
LONG* AS_pnLMT 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//负荷监测起始时间
LONG* AS_pnLMCMAX 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//负荷监测电流限值
LONG* AS_pnLMVMIN 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//负荷监测电压限值
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//TR2保持时间
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//故障录波触发时间
LONG* AS_pnC_THRD 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//电流变化记录阀值
LONG* AS_pnV_THRD 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//电压变化记录阀值

//变比----------------------------------
LONG* AS_pnPT1Ratio 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//PT1变比
LONG* AS_pnPT2Ratio 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//PT2变比
LONG* AS_pnPT3Ratio 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//PT3变比
LONG* AS_pnPT4Ratio 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//PT4变比
LONG* AS_pnPCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//保护CT变比
LONG* AS_pnMCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//测量CT变比
LONG* AS_pnZRatio 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//阻抗变比
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//rt1

//系数----------------------------------
LONG* CF_pnIP 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//IP
LONG* CF_pnUP 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UP
LONG* CF_pnU3 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U3
LONG* CF_pnU4 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//U4
LONG* CF_pnITM 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//ITM
LONG* CF_pnIFM 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//IFM
LONG* CF_pnU1M 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//U1M
LONG* CF_pnU2M 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//U2M
LONG* CF_pnU3M 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//U3M
LONG* CF_pnU4M 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//U4M
LONG* CF_pnP 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//P
LONG* CF_pnQ 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//Q
LONG* CF_pnA 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//A
LONG* CF_pnZ 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//Z
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//C4
LONG* CF_pnITDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//ITDRIFT
LONG* CF_pnIFDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//IFDRIFT
LONG* CF_pnU1DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//U1DRIFT
LONG* CF_pnU2DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//U2DRIFT
LONG* CF_pnU3DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//U3DRIFT
LONG* CF_pnU4DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//U4DRIFT

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 140), // IN1
    (LONG*)(RM_BUF_ADDR + 144) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 132), // BR1
    (LONG*)(RM_BUF_ADDR + 136) // BR2
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

//距离Ⅰ段启动2000----------
const  LONG* FailureInf2000[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅰ段动作2001----------
const  LONG* FailureInf2001[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅰ段返回2002----------
const  LONG* FailureInf2002[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段启动2003----------
const  LONG* FailureInf2003[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段动作2004----------
const  LONG* FailureInf2004[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅱ段返回2005----------
const  LONG* FailureInf2005[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段启动2006----------
const  LONG* FailureInf2006[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段动作2007----------
const  LONG* FailureInf2007[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅲ段返回2008----------
const  LONG* FailureInf2008[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段启动2009----------
const  LONG* FailureInf2009[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段动作2010----------
const  LONG* FailureInf2010[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅳ段返回2011----------
const  LONG* FailureInf2011[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段加速启动2012----------
const  LONG* FailureInf2012[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段加速动作2013----------
const  LONG* FailureInf2013[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅱ段加速返回2014----------
const  LONG* FailureInf2014[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段加速启动2015----------
const  LONG* FailureInf2015[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段加速动作2016----------
const  LONG* FailureInf2016[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅲ段加速返回2017----------
const  LONG* FailureInf2017[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段加速启动2018----------
const  LONG* FailureInf2018[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段加速动作2019----------
const  LONG* FailureInf2019[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅳ段加速返回2020----------
const  LONG* FailureInf2020[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流速断启动2021----------
const  LONG* FailureInf2021[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流速断动作2022----------
const  LONG* FailureInf2022[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//电流速断返回2023----------
const  LONG* FailureInf2023[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段启动2024----------
const  LONG* FailureInf2024[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段动作2025----------
const  LONG* FailureInf2025[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅰ段返回2026----------
const  LONG* FailureInf2026[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段启动2027----------
const  LONG* FailureInf2027[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段动作2028----------
const  LONG* FailureInf2028[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅱ段返回2029----------
const  LONG* FailureInf2029[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段启动2030----------
const  LONG* FailureInf2030[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段动作2031----------
const  LONG* FailureInf2031[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅲ段返回2032----------
const  LONG* FailureInf2032[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段加速启动2033----------
const  LONG* FailureInf2033[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段加速动作2034----------
const  LONG* FailureInf2034[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅰ段加速返回2035----------
const  LONG* FailureInf2035[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段加速启动2036----------
const  LONG* FailureInf2036[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段加速动作2037----------
const  LONG* FailureInf2037[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅱ段加速返回2038----------
const  LONG* FailureInf2038[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段加速启动2039----------
const  LONG* FailureInf2039[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段加速动作2040----------
const  LONG* FailureInf2040[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅲ段加速返回2041----------
const  LONG* FailureInf2041[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流增量保护启动2042----------
const  LONG* FailureInf2042[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流增量保护动作2043----------
const  LONG* FailureInf2043[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//电流增量保护返回2044----------
const  LONG* FailureInf2044[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//反时限过流启动2045----------
const  LONG* FailureInf2045[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//反时限过流动作2046----------
const  LONG* FailureInf2046[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//反时限过流返回2047----------
const  LONG* FailureInf2047[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//低压保护跳闸2048----------
const  LONG* FailureInf2048[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//低压保护告警2049----------
const  LONG* FailureInf2049[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//偷跳重合闸2050----------
const  LONG* FailureInf2050[] = {
    {NULL}
};

//一次重合闸出口2051----------
const  LONG* FailureInf2051[] = {
    {NULL}
};

//二次重合闸出口2052----------
const  LONG* FailureInf2052[] = {
    {NULL}
};

//PT断线闭锁距离保护2053----------
const  LONG* FailureInf2053[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//一次重合闸被闭锁2054----------
const  LONG* FailureInf2054[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20) // U2
};

//二次重合闸被闭锁2250----------
const  LONG* FailureInf2250[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20) // U2
};

//失灵保护动作2055----------
const  LONG* FailureInf2055[] = {
    {NULL}
};

//PT1断线2056----------
const  LONG* FailureInf2056[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//PT2断线2057----------
const  LONG* FailureInf2057[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//PT1断线返回2058----------
const  LONG* FailureInf2058[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//PT2断线返回2059----------
const  LONG* FailureInf2059[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//重合闸失败2060----------
const  LONG* FailureInf2060[] = {
    {NULL}
};

//保护启动2061----------
const  LONG* FailureInf2061[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//断路器跳闸失败2062----------
const  LONG* FailureInf2062[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//通道测试2063----------
const  LONG* FailureInf2063[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52), // I5
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 84), // ΦT
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 96), // ΦF
    (LONG*)(RM_BUF_ADDR + 100), // ΦUP
    (LONG*)(RM_BUF_ADDR + 104), // ΦU1
    (LONG*)(RM_BUF_ADDR + 108), // ΦU2
    (LONG*)(RM_BUF_ADDR + 112), // ΦIP
    (LONG*)(RM_BUF_ADDR + 116), // ΦIT
    (LONG*)(RM_BUF_ADDR + 120) // ΦIF
};

//CT1断线告警2064----------
const  LONG* FailureInf2064[] = {
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 56) // Itm
};

//CT2断线告警2065----------
const  LONG* FailureInf2065[] = {
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 60) // Ifm
};

//CT1断线返回2066----------
const  LONG* FailureInf2066[] = {
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 56) // Itm
};

//CT2断线返回2067----------
const  LONG* FailureInf2067[] = {
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 60) // Ifm
};

//一次重合闸启动2068----------
const  LONG* FailureInf2068[] = {
    {NULL}
};

//二次重合闸启动2249----------
const  LONG* FailureInf2249[] = {
    {NULL}
};

//一次重合闸成功2069----------
const  LONG* FailureInf2069[] = {
    {NULL}
};

//一次重合闸失败2070----------
const  LONG* FailureInf2070[] = {
    {NULL}
};

//二次重合闸成功2071----------
const  LONG* FailureInf2071[] = {
    {NULL}
};

//二次重合闸失败2072----------
const  LONG* FailureInf2072[] = {
    {NULL}
};

//一次重合闸未启动2073----------
const  LONG* FailureInf2073[] = {
    {NULL}
};

//二次重合闸未启动2251----------
const  LONG* FailureInf2251[] = {
    {NULL}
};

//距离Ⅰ段启动2100----------
const  LONG* FailureInf2100[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅰ段动作2101----------
const  LONG* FailureInf2101[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅰ段返回2102----------
const  LONG* FailureInf2102[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段启动2103----------
const  LONG* FailureInf2103[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段动作2104----------
const  LONG* FailureInf2104[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅱ段返回2105----------
const  LONG* FailureInf2105[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段启动2106----------
const  LONG* FailureInf2106[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段动作2107----------
const  LONG* FailureInf2107[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅲ段返回2108----------
const  LONG* FailureInf2108[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段启动2109----------
const  LONG* FailureInf2109[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段动作2110----------
const  LONG* FailureInf2110[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅳ段返回2111----------
const  LONG* FailureInf2111[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段加速启动2112----------
const  LONG* FailureInf2112[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段加速动作2113----------
const  LONG* FailureInf2113[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅱ段加速返回2114----------
const  LONG* FailureInf2114[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段加速启动2115----------
const  LONG* FailureInf2115[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段加速动作2116----------
const  LONG* FailureInf2116[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅲ段加速返回2117----------
const  LONG* FailureInf2117[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段加速启动2118----------
const  LONG* FailureInf2118[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段加速动作2119----------
const  LONG* FailureInf2119[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅳ段加速返回2120----------
const  LONG* FailureInf2120[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流速断启动2121----------
const  LONG* FailureInf2121[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流速断动作2122----------
const  LONG* FailureInf2122[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//电流速断返回2123----------
const  LONG* FailureInf2123[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段启动2124----------
const  LONG* FailureInf2124[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段动作2125----------
const  LONG* FailureInf2125[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅰ段返回2126----------
const  LONG* FailureInf2126[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段启动2127----------
const  LONG* FailureInf2127[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段动作2128----------
const  LONG* FailureInf2128[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅱ段返回2129----------
const  LONG* FailureInf2129[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段启动2130----------
const  LONG* FailureInf2130[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段动作2131----------
const  LONG* FailureInf2131[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅲ段返回2132----------
const  LONG* FailureInf2132[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段加速启动2133----------
const  LONG* FailureInf2133[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段加速动作2134----------
const  LONG* FailureInf2134[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅰ段加速返回2135----------
const  LONG* FailureInf2135[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段加速启动2136----------
const  LONG* FailureInf2136[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段加速动作2137----------
const  LONG* FailureInf2137[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅱ段加速返回2138----------
const  LONG* FailureInf2138[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段加速启动2139----------
const  LONG* FailureInf2139[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段加速动作2140----------
const  LONG* FailureInf2140[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅲ段加速返回2141----------
const  LONG* FailureInf2141[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流增量保护启动2142----------
const  LONG* FailureInf2142[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流增量保护动作2143----------
const  LONG* FailureInf2143[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//电流增量保护返回2144----------
const  LONG* FailureInf2144[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//反时限过流启动2145----------
const  LONG* FailureInf2145[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//反时限过流动作2146----------
const  LONG* FailureInf2146[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//反时限过流返回2147----------
const  LONG* FailureInf2147[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//低压保护跳闸2148----------
const  LONG* FailureInf2148[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅰ段启动2200----------
const  LONG* FailureInf2200[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅰ段动作2201----------
const  LONG* FailureInf2201[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅰ段返回2202----------
const  LONG* FailureInf2202[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段启动2203----------
const  LONG* FailureInf2203[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段动作2204----------
const  LONG* FailureInf2204[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅱ段返回2205----------
const  LONG* FailureInf2205[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段启动2206----------
const  LONG* FailureInf2206[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段动作2207----------
const  LONG* FailureInf2207[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅲ段返回2208----------
const  LONG* FailureInf2208[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段启动2209----------
const  LONG* FailureInf2209[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段动作2210----------
const  LONG* FailureInf2210[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅳ段返回2211----------
const  LONG* FailureInf2211[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段加速启动2212----------
const  LONG* FailureInf2212[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅱ段加速动作2213----------
const  LONG* FailureInf2213[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅱ段加速返回2214----------
const  LONG* FailureInf2214[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段加速启动2215----------
const  LONG* FailureInf2215[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅲ段加速动作2216----------
const  LONG* FailureInf2216[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅲ段加速返回2217----------
const  LONG* FailureInf2217[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段加速启动2218----------
const  LONG* FailureInf2218[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//距离Ⅳ段加速动作2219----------
const  LONG* FailureInf2219[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//距离Ⅳ段加速返回2220----------
const  LONG* FailureInf2220[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流速断启动2221----------
const  LONG* FailureInf2221[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流速断动作2222----------
const  LONG* FailureInf2222[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//电流速断返回2223----------
const  LONG* FailureInf2223[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段启动2224----------
const  LONG* FailureInf2224[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段动作2225----------
const  LONG* FailureInf2225[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅰ段返回2226----------
const  LONG* FailureInf2226[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段启动2227----------
const  LONG* FailureInf2227[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段动作2228----------
const  LONG* FailureInf2228[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅱ段返回2229----------
const  LONG* FailureInf2229[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段启动2230----------
const  LONG* FailureInf2230[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段动作2231----------
const  LONG* FailureInf2231[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅲ段返回2232----------
const  LONG* FailureInf2232[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段加速启动2233----------
const  LONG* FailureInf2233[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅰ段加速动作2234----------
const  LONG* FailureInf2234[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅰ段加速返回2235----------
const  LONG* FailureInf2235[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段加速启动2236----------
const  LONG* FailureInf2236[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅱ段加速动作2237----------
const  LONG* FailureInf2237[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅱ段加速返回2238----------
const  LONG* FailureInf2238[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段加速启动2239----------
const  LONG* FailureInf2239[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//过流Ⅲ段加速动作2240----------
const  LONG* FailureInf2240[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//过流Ⅲ段加速返回2241----------
const  LONG* FailureInf2241[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流增量保护启动2242----------
const  LONG* FailureInf2242[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//电流增量保护动作2243----------
const  LONG* FailureInf2243[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//电流增量保护返回2244----------
const  LONG* FailureInf2244[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//反时限过流启动2245----------
const  LONG* FailureInf2245[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//反时限过流动作2246----------
const  LONG* FailureInf2246[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//反时限过流返回2247----------
const  LONG* FailureInf2247[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ΦZ
};

//低压保护跳闸2248----------
const  LONG* FailureInf2248[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ΦZ
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
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
    {0, FailureInf63, 63},//逻辑方程故障恢复
    {0, FailureInf64, 64},//5V电源故障恢复
    {0, FailureInf500, 500},//保护启动
    {3, FailureInf2000, 2000},//距离Ⅰ段启动
    {18, FailureInf2001, 2001},//距离Ⅰ段动作
    {3, FailureInf2002, 2002},//距离Ⅰ段返回
    {3, FailureInf2003, 2003},//距离Ⅱ段启动
    {18, FailureInf2004, 2004},//距离Ⅱ段动作
    {3, FailureInf2005, 2005},//距离Ⅱ段返回
    {3, FailureInf2006, 2006},//距离Ⅲ段启动
    {18, FailureInf2007, 2007},//距离Ⅲ段动作
    {3, FailureInf2008, 2008},//距离Ⅲ段返回
    {3, FailureInf2009, 2009},//距离Ⅳ段启动
    {18, FailureInf2010, 2010},//距离Ⅳ段动作
    {3, FailureInf2011, 2011},//距离Ⅳ段返回
    {3, FailureInf2012, 2012},//距离Ⅱ段加速启动
    {18, FailureInf2013, 2013},//距离Ⅱ段加速动作
    {3, FailureInf2014, 2014},//距离Ⅱ段加速返回
    {3, FailureInf2015, 2015},//距离Ⅲ段加速启动
    {18, FailureInf2016, 2016},//距离Ⅲ段加速动作
    {3, FailureInf2017, 2017},//距离Ⅲ段加速返回
    {3, FailureInf2018, 2018},//距离Ⅳ段加速启动
    {18, FailureInf2019, 2019},//距离Ⅳ段加速动作
    {3, FailureInf2020, 2020},//距离Ⅳ段加速返回
    {3, FailureInf2021, 2021},//电流速断启动
    {18, FailureInf2022, 2022},//电流速断动作
    {3, FailureInf2023, 2023},//电流速断返回
    {3, FailureInf2024, 2024},//过流Ⅰ段启动
    {18, FailureInf2025, 2025},//过流Ⅰ段动作
    {3, FailureInf2026, 2026},//过流Ⅰ段返回
    {3, FailureInf2027, 2027},//过流Ⅱ段启动
    {18, FailureInf2028, 2028},//过流Ⅱ段动作
    {3, FailureInf2029, 2029},//过流Ⅱ段返回
    {3, FailureInf2030, 2030},//过流Ⅲ段启动
    {18, FailureInf2031, 2031},//过流Ⅲ段动作
    {3, FailureInf2032, 2032},//过流Ⅲ段返回
    {3, FailureInf2033, 2033},//过流Ⅰ段加速启动
    {18, FailureInf2034, 2034},//过流Ⅰ段加速动作
    {3, FailureInf2035, 2035},//过流Ⅰ段加速返回
    {3, FailureInf2036, 2036},//过流Ⅱ段加速启动
    {18, FailureInf2037, 2037},//过流Ⅱ段加速动作
    {3, FailureInf2038, 2038},//过流Ⅱ段加速返回
    {3, FailureInf2039, 2039},//过流Ⅲ段加速启动
    {18, FailureInf2040, 2040},//过流Ⅲ段加速动作
    {3, FailureInf2041, 2041},//过流Ⅲ段加速返回
    {3, FailureInf2042, 2042},//电流增量保护启动
    {18, FailureInf2043, 2043},//电流增量保护动作
    {3, FailureInf2044, 2044},//电流增量保护返回
    {3, FailureInf2045, 2045},//反时限过流启动
    {18, FailureInf2046, 2046},//反时限过流动作
    {3, FailureInf2047, 2047},//反时限过流返回
    {18, FailureInf2048, 2048},//低压保护跳闸
    {18, FailureInf2049, 2049},//低压保护告警
    {0, FailureInf2050, 2050},//偷跳重合闸
    {0, FailureInf2051, 2051},//一次重合闸出口
    {0, FailureInf2052, 2052},//二次重合闸出口
    {8, FailureInf2053, 2053},//PT断线闭锁距离保护
    {2, FailureInf2054, 2054},//一次重合闸被闭锁
    {2, FailureInf2250, 2250},//二次重合闸被闭锁
    {0, FailureInf2055, 2055},//失灵保护动作
    {5, FailureInf2056, 2056},//PT1断线
    {5, FailureInf2057, 2057},//PT2断线
    {5, FailureInf2058, 2058},//PT1断线返回
    {5, FailureInf2059, 2059},//PT2断线返回
    {0, FailureInf2060, 2060},//重合闸失败
    {10, FailureInf2061, 2061},//保护启动
    {3, FailureInf2062, 2062},//断路器跳闸失败
    {26, FailureInf2063, 2063},//通道测试
    {2, FailureInf2064, 2064},//CT1断线告警
    {2, FailureInf2065, 2065},//CT2断线告警
    {2, FailureInf2066, 2066},//CT1断线返回
    {2, FailureInf2067, 2067},//CT2断线返回
    {0, FailureInf2068, 2068},//一次重合闸启动
    {0, FailureInf2249, 2249},//二次重合闸启动
    {0, FailureInf2069, 2069},//一次重合闸成功
    {0, FailureInf2070, 2070},//一次重合闸失败
    {0, FailureInf2071, 2071},//二次重合闸成功
    {0, FailureInf2072, 2072},//二次重合闸失败
    {0, FailureInf2073, 2073},//一次重合闸未启动
    {0, FailureInf2251, 2251},//二次重合闸未启动
    {3, FailureInf2100, 2100},//距离Ⅰ段启动
    {9, FailureInf2101, 2101},//距离Ⅰ段动作
    {3, FailureInf2102, 2102},//距离Ⅰ段返回
    {3, FailureInf2103, 2103},//距离Ⅱ段启动
    {9, FailureInf2104, 2104},//距离Ⅱ段动作
    {3, FailureInf2105, 2105},//距离Ⅱ段返回
    {3, FailureInf2106, 2106},//距离Ⅲ段启动
    {9, FailureInf2107, 2107},//距离Ⅲ段动作
    {3, FailureInf2108, 2108},//距离Ⅲ段返回
    {3, FailureInf2109, 2109},//距离Ⅳ段启动
    {9, FailureInf2110, 2110},//距离Ⅳ段动作
    {3, FailureInf2111, 2111},//距离Ⅳ段返回
    {3, FailureInf2112, 2112},//距离Ⅱ段加速启动
    {9, FailureInf2113, 2113},//距离Ⅱ段加速动作
    {3, FailureInf2114, 2114},//距离Ⅱ段加速返回
    {3, FailureInf2115, 2115},//距离Ⅲ段加速启动
    {9, FailureInf2116, 2116},//距离Ⅲ段加速动作
    {3, FailureInf2117, 2117},//距离Ⅲ段加速返回
    {3, FailureInf2118, 2118},//距离Ⅳ段加速启动
    {9, FailureInf2119, 2119},//距离Ⅳ段加速动作
    {3, FailureInf2120, 2120},//距离Ⅳ段加速返回
    {3, FailureInf2121, 2121},//电流速断启动
    {9, FailureInf2122, 2122},//电流速断动作
    {3, FailureInf2123, 2123},//电流速断返回
    {3, FailureInf2124, 2124},//过流Ⅰ段启动
    {9, FailureInf2125, 2125},//过流Ⅰ段动作
    {3, FailureInf2126, 2126},//过流Ⅰ段返回
    {3, FailureInf2127, 2127},//过流Ⅱ段启动
    {9, FailureInf2128, 2128},//过流Ⅱ段动作
    {3, FailureInf2129, 2129},//过流Ⅱ段返回
    {3, FailureInf2130, 2130},//过流Ⅲ段启动
    {9, FailureInf2131, 2131},//过流Ⅲ段动作
    {3, FailureInf2132, 2132},//过流Ⅲ段返回
    {3, FailureInf2133, 2133},//过流Ⅰ段加速启动
    {9, FailureInf2134, 2134},//过流Ⅰ段加速动作
    {3, FailureInf2135, 2135},//过流Ⅰ段加速返回
    {3, FailureInf2136, 2136},//过流Ⅱ段加速启动
    {9, FailureInf2137, 2137},//过流Ⅱ段加速动作
    {3, FailureInf2138, 2138},//过流Ⅱ段加速返回
    {3, FailureInf2139, 2139},//过流Ⅲ段加速启动
    {9, FailureInf2140, 2140},//过流Ⅲ段加速动作
    {3, FailureInf2141, 2141},//过流Ⅲ段加速返回
    {3, FailureInf2142, 2142},//电流增量保护启动
    {9, FailureInf2143, 2143},//电流增量保护动作
    {3, FailureInf2144, 2144},//电流增量保护返回
    {3, FailureInf2145, 2145},//反时限过流启动
    {9, FailureInf2146, 2146},//反时限过流动作
    {3, FailureInf2147, 2147},//反时限过流返回
    {9, FailureInf2148, 2148},//低压保护跳闸
    {3, FailureInf2200, 2200},//距离Ⅰ段启动
    {10, FailureInf2201, 2201},//距离Ⅰ段动作
    {3, FailureInf2202, 2202},//距离Ⅰ段返回
    {3, FailureInf2203, 2203},//距离Ⅱ段启动
    {10, FailureInf2204, 2204},//距离Ⅱ段动作
    {3, FailureInf2205, 2205},//距离Ⅱ段返回
    {3, FailureInf2206, 2206},//距离Ⅲ段启动
    {10, FailureInf2207, 2207},//距离Ⅲ段动作
    {3, FailureInf2208, 2208},//距离Ⅲ段返回
    {3, FailureInf2209, 2209},//距离Ⅳ段启动
    {10, FailureInf2210, 2210},//距离Ⅳ段动作
    {3, FailureInf2211, 2211},//距离Ⅳ段返回
    {3, FailureInf2212, 2212},//距离Ⅱ段加速启动
    {10, FailureInf2213, 2213},//距离Ⅱ段加速动作
    {3, FailureInf2214, 2214},//距离Ⅱ段加速返回
    {3, FailureInf2215, 2215},//距离Ⅲ段加速启动
    {10, FailureInf2216, 2216},//距离Ⅲ段加速动作
    {3, FailureInf2217, 2217},//距离Ⅲ段加速返回
    {3, FailureInf2218, 2218},//距离Ⅳ段加速启动
    {10, FailureInf2219, 2219},//距离Ⅳ段加速动作
    {3, FailureInf2220, 2220},//距离Ⅳ段加速返回
    {3, FailureInf2221, 2221},//电流速断启动
    {10, FailureInf2222, 2222},//电流速断动作
    {3, FailureInf2223, 2223},//电流速断返回
    {3, FailureInf2224, 2224},//过流Ⅰ段启动
    {10, FailureInf2225, 2225},//过流Ⅰ段动作
    {3, FailureInf2226, 2226},//过流Ⅰ段返回
    {3, FailureInf2227, 2227},//过流Ⅱ段启动
    {10, FailureInf2228, 2228},//过流Ⅱ段动作
    {3, FailureInf2229, 2229},//过流Ⅱ段返回
    {3, FailureInf2230, 2230},//过流Ⅲ段启动
    {10, FailureInf2231, 2231},//过流Ⅲ段动作
    {3, FailureInf2232, 2232},//过流Ⅲ段返回
    {3, FailureInf2233, 2233},//过流Ⅰ段加速启动
    {10, FailureInf2234, 2234},//过流Ⅰ段加速动作
    {3, FailureInf2235, 2235},//过流Ⅰ段加速返回
    {3, FailureInf2236, 2236},//过流Ⅱ段加速启动
    {10, FailureInf2237, 2237},//过流Ⅱ段加速动作
    {3, FailureInf2238, 2238},//过流Ⅱ段加速返回
    {3, FailureInf2239, 2239},//过流Ⅲ段加速启动
    {10, FailureInf2240, 2240},//过流Ⅲ段加速动作
    {3, FailureInf2241, 2241},//过流Ⅲ段加速返回
    {3, FailureInf2242, 2242},//电流增量保护启动
    {10, FailureInf2243, 2243},//电流增量保护动作
    {3, FailureInf2244, 2244},//电流增量保护返回
    {3, FailureInf2245, 2245},//反时限过流启动
    {10, FailureInf2246, 2246},//反时限过流动作
    {3, FailureInf2247, 2247},//反时限过流返回
    {10, FailureInf2248, 2248},//低压保护跳闸
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
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
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 176) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 220) ,
	 (LONG*)(AS_BUF_ADDR + 224) ,
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
	 (BYTE*)(SW_BUF_ADDR + 5) ,
	 (BYTE*)(SW_BUF_ADDR + 6) ,
	 (BYTE*)(SW_BUF_ADDR + 7) ,
	 (BYTE*)(SW_BUF_ADDR + 8) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 43) ,
	 (BYTE*)(CFG_BUF_ADDR + 44) ,
	 (BYTE*)(CFG_BUF_ADDR + 45) ,
	 (BYTE*)(CFG_BUF_ADDR + 46) ,
	 (BYTE*)(CFG_BUF_ADDR + 47) ,
	 (BYTE*)(CFG_BUF_ADDR + 30) ,
	 (BYTE*)(CFG_BUF_ADDR + 34) ,
	 (BYTE*)(CFG_BUF_ADDR + 31) ,
	 (BYTE*)(CFG_BUF_ADDR + 32) ,
	 (BYTE*)(CFG_BUF_ADDR + 33) ,
	 (BYTE*)(CFG_BUF_ADDR + 35) ,
	 (BYTE*)(CFG_BUF_ADDR + 37) ,
	 (BYTE*)(CFG_BUF_ADDR + 38) ,
	 (BYTE*)(CFG_BUF_ADDR + 39) ,
	 (BYTE*)(CFG_BUF_ADDR + 40) ,
	 (BYTE*)(CFG_BUF_ADDR + 41) ,
	 (BYTE*)(CFG_BUF_ADDR + 42) ,
	 (BYTE*)(CFG_BUF_ADDR + 36) ,
	 (BYTE*)(CFG_BUF_ADDR + 48) ,
	 (BYTE*)(CFG_BUF_ADDR + 49) ,
	 (BYTE*)(CFG_BUF_ADDR + 50) ,
	 (BYTE*)(CFG_BUF_ADDR + 51) ,
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	3050, //IP
	11500, //UP
	11500, //U3
	11500, //U4
	64480, //ITM
	64480, //IFM
	16100, //U1M
	16100, //U2M
	16100, //U3M
	16100, //U4M
	129900, //P
	129900, //Q
	18, //A
	752, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	14, //ITDRIFT
	14, //IFDRIFT
	30, //U1DRIFT
	30, //U2DRIFT
	30, //U3DRIFT
	30, //U4DRIFT
};
//系数2
const LONG CFDefaultVal2[]=
{
	15250, //IP
	11500, //UP
	11500, //U3
	11500, //U4
	322400, //ITM
	322400, //IFM
	16100, //U1M
	16100, //U2M
	16100, //U3M
	16100, //U4M
	658980, //P
	658980, //Q
	18, //A
	150, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	14, //ITDRIFT
	14, //IFDRIFT
	30, //U1DRIFT
	30, //U2DRIFT
	30, //U3DRIFT
	30, //U4DRIFT
};

//传动----------------------------------
