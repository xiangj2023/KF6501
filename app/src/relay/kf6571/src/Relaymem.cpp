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
	{FORWORD, "正方向"},
	{BACKWORD, "反方向"},
	{FORWORD, "正方向"},
	{BACKWORD, "反方向"},
	{ETHNET, "以太网直连"},
	{SERIAL, "串口连接"},
	{ROUTER, "路由连接"},
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
	{2, (TSetValueInf*)&(SetValueInf[0])}, //定值修正方向0:+,1-
	{0, NULL},
	{2, (TSetValueInf*)&(SetValueInf[2])}, //定值公里标方向0:+,1-
	{0, NULL},
	{0, NULL},
	{3, (TSetValueInf*)&(SetValueInf[4])}, //定值通道类型
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
	{AT1MODE, "AT供电"},
	{AT2MODE, "客专方式"},
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
};

//保护测量值定义-----------------------
LONG* 	RM_pnU1 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// U1
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// U2
LONG* 	RM_pnU3 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// U3
LONG* 	RM_pnU4 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// U4
LONG* 	RM_pnIt1 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// It1
LONG* 	RM_pnIf1 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// If1
LONG* 	RM_pnIt2 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// It2
LONG* 	RM_pnIf2 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// If2
LONG* 	RM_pnIt3 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// It3
LONG* 	RM_pnIf3 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// If3
LONG* 	RM_pnIat1 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// Iat1
LONG* 	RM_pnIat2 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Iat2
LONG* 	RM_pnSSIAT 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IAT
LONG* 	RM_pnU1A 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// AU1
LONG* 	RM_pnU2A 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// AU2
LONG* 	RM_pnU3A 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// AU3
LONG* 	RM_pnU4A 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// AU4
LONG* 	RM_pnIT1A 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// AIT1
LONG* 	RM_pnIF1A 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// AIF1
LONG* 	RM_pnIT2A 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// AIT2
LONG* 	RM_pnIF2A 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// AIF2
LONG* 	RM_pnIT3A 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// AIT3
LONG* 	RM_pnIF3A 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// AIF3
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// IN2
LONG* 	RM_pnSub1_U1 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// U1
LONG* 	RM_pnSub1_U2 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// U2
LONG* 	RM_pnSub1_U3 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// U3
LONG* 	RM_pnSub1_U4 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// U4
LONG* 	RM_pnSub1_IT1 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// IT1
LONG* 	RM_pnSub1_IF1 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// IF1
LONG* 	RM_pnSub1_IT2 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// IT2
LONG* 	RM_pnSub1_IF2 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// IF2
LONG* 	RM_pnSub1_IT3 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// IT3
LONG* 	RM_pnSub1_IF3 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// IF3
LONG* 	RM_pnSub1_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// IAT1
LONG* 	RM_pnSub1_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// IAT2
LONG* 	RM_pnSub1_IAT 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// IAT
LONG* 	RM_pnSub1_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// AIT1
LONG* 	RM_pnSub1_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// AIF1
LONG* 	RM_pnSub1_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// AIT2
LONG* 	RM_pnSub1_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// AIF2
LONG* 	RM_pnSub1_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// AIT3
LONG* 	RM_pnSub1_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// AIF3
LONG* 	RM_pnSub1_Status 	= 	(LONG*)(RM_BUF_ADDR + 184); 	// ST
LONG* 	RM_pnSub1_year 	= 	(LONG*)(RM_BUF_ADDR + 188); 	// Year
LONG* 	RM_pnSub1_month 	= 	(LONG*)(RM_BUF_ADDR + 192); 	// Mon.
LONG* 	RM_pnSub1_day 	= 	(LONG*)(RM_BUF_ADDR + 196); 	// Day
LONG* 	RM_pnSub1_hour 	= 	(LONG*)(RM_BUF_ADDR + 200); 	// Hour
LONG* 	RM_pnSub1_minute 	= 	(LONG*)(RM_BUF_ADDR + 204); 	// Min.
LONG* 	RM_pnSub1_NS 	= 	(LONG*)(RM_BUF_ADDR + 208); 	// NS
LONG* 	RM_pnSub2_U1 	= 	(LONG*)(RM_BUF_ADDR + 212); 	// U1
LONG* 	RM_pnSub2_U2 	= 	(LONG*)(RM_BUF_ADDR + 216); 	// U2
LONG* 	RM_pnSub2_U3 	= 	(LONG*)(RM_BUF_ADDR + 220); 	// U3
LONG* 	RM_pnSub2_U4 	= 	(LONG*)(RM_BUF_ADDR + 224); 	// U4
LONG* 	RM_pnSub2_IT1 	= 	(LONG*)(RM_BUF_ADDR + 228); 	// IT1
LONG* 	RM_pnSub2_IF1 	= 	(LONG*)(RM_BUF_ADDR + 232); 	// IF1
LONG* 	RM_pnSub2_IT2 	= 	(LONG*)(RM_BUF_ADDR + 236); 	// IT2
LONG* 	RM_pnSub2_IF2 	= 	(LONG*)(RM_BUF_ADDR + 240); 	// IF2
LONG* 	RM_pnSub2_IT3 	= 	(LONG*)(RM_BUF_ADDR + 244); 	// IT3
LONG* 	RM_pnSub2_IF3 	= 	(LONG*)(RM_BUF_ADDR + 248); 	// IF3
LONG* 	RM_pnSub2_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 252); 	// IAT1
LONG* 	RM_pnSub2_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 256); 	// IAT2
LONG* 	RM_pnSub2_IAT 	= 	(LONG*)(RM_BUF_ADDR + 260); 	// IAT
LONG* 	RM_pnSub2_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 264); 	// AIT1
LONG* 	RM_pnSub2_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 268); 	// AIF1
LONG* 	RM_pnSub2_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 272); 	// AIT2
LONG* 	RM_pnSub2_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 276); 	// AIF2
LONG* 	RM_pnSub2_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 280); 	// AIT3
LONG* 	RM_pnSub2_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 284); 	// AIF3
LONG* 	RM_pnSub2_Status 	= 	(LONG*)(RM_BUF_ADDR + 288); 	// ST
LONG* 	RM_pnSub2_year 	= 	(LONG*)(RM_BUF_ADDR + 292); 	// Year
LONG* 	RM_pnSub2_month 	= 	(LONG*)(RM_BUF_ADDR + 296); 	// Mon.
LONG* 	RM_pnSub2_day 	= 	(LONG*)(RM_BUF_ADDR + 300); 	// Day
LONG* 	RM_pnSub2_hour 	= 	(LONG*)(RM_BUF_ADDR + 304); 	// Hour
LONG* 	RM_pnSub2_minute 	= 	(LONG*)(RM_BUF_ADDR + 308); 	// Min.
LONG* 	RM_pnSub2_NS 	= 	(LONG*)(RM_BUF_ADDR + 312); 	// NS
LONG* 	RM_pnSub3_U1 	= 	(LONG*)(RM_BUF_ADDR + 316); 	// U1
LONG* 	RM_pnSub3_U2 	= 	(LONG*)(RM_BUF_ADDR + 320); 	// U2
LONG* 	RM_pnSub3_U3 	= 	(LONG*)(RM_BUF_ADDR + 324); 	// U3
LONG* 	RM_pnSub3_U4 	= 	(LONG*)(RM_BUF_ADDR + 328); 	// U4
LONG* 	RM_pnSub3_IT1 	= 	(LONG*)(RM_BUF_ADDR + 332); 	// IT1
LONG* 	RM_pnSub3_IF1 	= 	(LONG*)(RM_BUF_ADDR + 336); 	// IF1
LONG* 	RM_pnSub3_IT2 	= 	(LONG*)(RM_BUF_ADDR + 340); 	// IT2
LONG* 	RM_pnSub3_IF2 	= 	(LONG*)(RM_BUF_ADDR + 344); 	// IF2
LONG* 	RM_pnSub3_IT3 	= 	(LONG*)(RM_BUF_ADDR + 348); 	// IT3
LONG* 	RM_pnSub3_IF3 	= 	(LONG*)(RM_BUF_ADDR + 352); 	// IF3
LONG* 	RM_pnSub3_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 356); 	// IAT1
LONG* 	RM_pnSub3_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 360); 	// IAT2
LONG* 	RM_pnSub3_IAT 	= 	(LONG*)(RM_BUF_ADDR + 364); 	// IAT
LONG* 	RM_pnSub3_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 368); 	// AIT1
LONG* 	RM_pnSub3_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 372); 	// AIF1
LONG* 	RM_pnSub3_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 376); 	// AIT2
LONG* 	RM_pnSub3_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 380); 	// AIF2
LONG* 	RM_pnSub3_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 384); 	// AIT3
LONG* 	RM_pnSub3_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 388); 	// AIF3
LONG* 	RM_pnSub3_Status 	= 	(LONG*)(RM_BUF_ADDR + 392); 	// ST
LONG* 	RM_pnSub3_year 	= 	(LONG*)(RM_BUF_ADDR + 396); 	// Year
LONG* 	RM_pnSub3_month 	= 	(LONG*)(RM_BUF_ADDR + 400); 	// Mon.
LONG* 	RM_pnSub3_day 	= 	(LONG*)(RM_BUF_ADDR + 404); 	// Day
LONG* 	RM_pnSub3_hour 	= 	(LONG*)(RM_BUF_ADDR + 408); 	// Hour
LONG* 	RM_pnSub3_minute 	= 	(LONG*)(RM_BUF_ADDR + 412); 	// Min.
LONG* 	RM_pnSub3_NS 	= 	(LONG*)(RM_BUF_ADDR + 416); 	// NS
LONG* 	RM_pnSub4_U1 	= 	(LONG*)(RM_BUF_ADDR + 420); 	// U1
LONG* 	RM_pnSub4_U2 	= 	(LONG*)(RM_BUF_ADDR + 424); 	// U2
LONG* 	RM_pnSub4_U3 	= 	(LONG*)(RM_BUF_ADDR + 428); 	// U3
LONG* 	RM_pnSub4_U4 	= 	(LONG*)(RM_BUF_ADDR + 432); 	// U4
LONG* 	RM_pnSub4_IT1 	= 	(LONG*)(RM_BUF_ADDR + 436); 	// IT1
LONG* 	RM_pnSub4_IF1 	= 	(LONG*)(RM_BUF_ADDR + 440); 	// IF1
LONG* 	RM_pnSub4_IT2 	= 	(LONG*)(RM_BUF_ADDR + 444); 	// IT2
LONG* 	RM_pnSub4_IF2 	= 	(LONG*)(RM_BUF_ADDR + 448); 	// IF2
LONG* 	RM_pnSub4_IT3 	= 	(LONG*)(RM_BUF_ADDR + 452); 	// IT3
LONG* 	RM_pnSub4_IF3 	= 	(LONG*)(RM_BUF_ADDR + 456); 	// IF3
LONG* 	RM_pnSub4_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 460); 	// IAT1
LONG* 	RM_pnSub4_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 464); 	// IAT2
LONG* 	RM_pnSub4_IAT 	= 	(LONG*)(RM_BUF_ADDR + 468); 	// IAT
LONG* 	RM_pnSub4_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 472); 	// AIT1
LONG* 	RM_pnSub4_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 476); 	// AIF1
LONG* 	RM_pnSub4_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 480); 	// AIT2
LONG* 	RM_pnSub4_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 484); 	// AIF2
LONG* 	RM_pnSub4_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 488); 	// AIT3
LONG* 	RM_pnSub4_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 492); 	// AIF3
LONG* 	RM_pnSub4_Status 	= 	(LONG*)(RM_BUF_ADDR + 496); 	// ST
LONG* 	RM_pnSub4_year 	= 	(LONG*)(RM_BUF_ADDR + 500); 	// Year
LONG* 	RM_pnSub4_month 	= 	(LONG*)(RM_BUF_ADDR + 504); 	// Mon.
LONG* 	RM_pnSub4_day 	= 	(LONG*)(RM_BUF_ADDR + 508); 	// Day
LONG* 	RM_pnSub4_hour 	= 	(LONG*)(RM_BUF_ADDR + 512); 	// Hour
LONG* 	RM_pnSub4_minute 	= 	(LONG*)(RM_BUF_ADDR + 516); 	// Min.
LONG* 	RM_pnSub4_NS 	= 	(LONG*)(RM_BUF_ADDR + 520); 	// NS
LONG* 	RM_pnSub5_U1 	= 	(LONG*)(RM_BUF_ADDR + 524); 	// U1
LONG* 	RM_pnSub5_U2 	= 	(LONG*)(RM_BUF_ADDR + 528); 	// U2
LONG* 	RM_pnSub5_U3 	= 	(LONG*)(RM_BUF_ADDR + 532); 	// U3
LONG* 	RM_pnSub5_U4 	= 	(LONG*)(RM_BUF_ADDR + 536); 	// U4
LONG* 	RM_pnSub5_IT1 	= 	(LONG*)(RM_BUF_ADDR + 540); 	// IT1
LONG* 	RM_pnSub5_IF1 	= 	(LONG*)(RM_BUF_ADDR + 544); 	// IF1
LONG* 	RM_pnSub5_IT2 	= 	(LONG*)(RM_BUF_ADDR + 548); 	// IT2
LONG* 	RM_pnSub5_IF2 	= 	(LONG*)(RM_BUF_ADDR + 552); 	// IF2
LONG* 	RM_pnSub5_IT3 	= 	(LONG*)(RM_BUF_ADDR + 556); 	// IT3
LONG* 	RM_pnSub5_IF3 	= 	(LONG*)(RM_BUF_ADDR + 560); 	// IF3
LONG* 	RM_pnSub5_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 564); 	// IAT1
LONG* 	RM_pnSub5_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 568); 	// IAT2
LONG* 	RM_pnSub5_IAT 	= 	(LONG*)(RM_BUF_ADDR + 572); 	// IAT
LONG* 	RM_pnSub5_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 576); 	// AIT1
LONG* 	RM_pnSub5_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 580); 	// AIF1
LONG* 	RM_pnSub5_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 584); 	// AIT2
LONG* 	RM_pnSub5_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 588); 	// AIF2
LONG* 	RM_pnSub5_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 592); 	// AIT3
LONG* 	RM_pnSub5_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 596); 	// AIF3
LONG* 	RM_pnSub5_Status 	= 	(LONG*)(RM_BUF_ADDR + 600); 	// ST
LONG* 	RM_pnSub5_year 	= 	(LONG*)(RM_BUF_ADDR + 604); 	// Year
LONG* 	RM_pnSub5_month 	= 	(LONG*)(RM_BUF_ADDR + 608); 	// Mon.
LONG* 	RM_pnSub5_day 	= 	(LONG*)(RM_BUF_ADDR + 612); 	// Day
LONG* 	RM_pnSub5_hour 	= 	(LONG*)(RM_BUF_ADDR + 616); 	// Hour
LONG* 	RM_pnSub5_minute 	= 	(LONG*)(RM_BUF_ADDR + 620); 	// Min.
LONG* 	RM_pnSub5_NS 	= 	(LONG*)(RM_BUF_ADDR + 624); 	// NS
LONG* 	RM_pnDist 	= 	(LONG*)(RM_BUF_ADDR + 628); 	// D
LONG* 	RM_pnKMS 	= 	(LONG*)(RM_BUF_ADDR + 632); 	// KMS
LONG* 	RM_pnR_T 	= 	(LONG*)(RM_BUF_ADDR + 636); 	// R_T
LONG* 	RM_pnX_T 	= 	(LONG*)(RM_BUF_ADDR + 640); 	// X_T
LONG* 	RM_pnR_F 	= 	(LONG*)(RM_BUF_ADDR + 644); 	// R_F
LONG* 	RM_pnX_F 	= 	(LONG*)(RM_BUF_ADDR + 648); 	// X_F
LONG* 	RM_pnR_TF 	= 	(LONG*)(RM_BUF_ADDR + 652); 	// R_TF
LONG* 	RM_pnX_TF 	= 	(LONG*)(RM_BUF_ADDR + 656); 	// X_TF
LONG* 	RM_pnNS 	= 	(LONG*)(RM_BUF_ADDR + 660); 	// NS
LONG* 	RM_pnIP_H2 	= 	(LONG*)(RM_BUF_ADDR + 664); 	// IPh2
LONG* 	RM_pnIP_H3 	= 	(LONG*)(RM_BUF_ADDR + 668); 	// IPh3
LONG* 	RM_pnIP_H5 	= 	(LONG*)(RM_BUF_ADDR + 672); 	// IPh5
LONG* 	RM_pnUp 	= 	(LONG*)(RM_BUF_ADDR + 676); 	// Up
LONG* 	RM_pnI1 	= 	(LONG*)(RM_BUF_ADDR + 680); 	// I1
LONG* 	RM_pnI2 	= 	(LONG*)(RM_BUF_ADDR + 684); 	// I2
LONG* 	RM_pnI3 	= 	(LONG*)(RM_BUF_ADDR + 688); 	// I3


//测量值定义-----------------------


//软压板----------------------------------
BYTE* SW_pbyFL 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//故障测距
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//检修压板
BYTE* SW_pbyRESD1 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//备用压板1
BYTE* SW_pbyRESD2 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//备用压板2

//配置----------------------------------
BYTE* CFG_pby21_PI 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//阻抗启动
BYTE* CFG_pby50_PI 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//过流启动
BYTE* CFG_pby50DI_PI 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//增量启动
BYTE* CFG_pbyFL_AUTO 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//测距自适应
BYTE* CFG_pbyCCMODE 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//电流合成
BYTE* CFG_pbyAT_C_RATE 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//吸上电流比
BYTE* CFG_pbyUD_C_RATE 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//上下行电流比
BYTE* CFG_pbyNL_C_RATE 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//吸馈电流比
BYTE* CFG_pbyPHASESC 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//相间短路判断
BYTE* CFG_pbyNONEAT 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//变电所无AT
BYTE* CFG_pbyISSS 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//本所为变电所
BYTE* CFG_pbyPARALLG 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//平行四边形
BYTE* CFG_pby21_H_S 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//距离谐波抑制
BYTE* CFG_pby21_H_L 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//距离谐波闭锁
BYTE* CFG_pby50_H_S 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//过流谐波抑制
BYTE* CFG_pby50_H_L 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//过流谐波闭锁
BYTE* CFG_pby50_27V 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//过流启动低压闭锁
BYTE* CFG_pbyCALLDATA 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//召测取低压数据
BYTE* CFG_pbyCHECKGPS 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//GPS故障检测
BYTE* CFG_pbyCHECKCOM 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//通道自动检测
BYTE* CFG_pbyCROSSAERA 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//允许越区
BYTE* CFG_pbyRESD1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//备用配置1
BYTE* CFG_pbyRESD2 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//备用配置2

//定值----------------------------------
LONG* RS_pnCFG 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字 CFG
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//突变量启动电流 PIC
LONG* RS_pn21R 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//距离启动电阻 21R
LONG* RS_pn21X 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//距离启动电抗 21X
LONG* RS_pn78LA 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//线路阻抗角 78LA
LONG* RS_pn50C 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//过流定值 50C
LONG* RS_pn50_27V 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//过流启动低压闭锁定值 50_27V
LONG* RS_pn50DC 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//电流增量启动电流定值 50DC
LONG* RS_pn50D_37C 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//增量保护小电流闭锁 50D_37C
LONG* RS_pnFLT 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//故标启动延时 FLT
LONG* RS_pnKh1 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//二次谐波闭锁系数 Kh1
LONG* RS_pnKhr1 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//距离过流谐波加权 Khr1
LONG* RS_pnKhr2 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//增量保护谐波加权 Khr2
LONG* RS_pn27Uab 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//相间短路低压定值 27Uab
LONG* RS_pn27U 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//低压记录数据定值 27U
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//低压数据记录延时 27D
LONG* RS_pnN 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//子所个数 N
LONG* RS_pnNID 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//本所站号 NID
LONG* RS_pnD1 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//AT区间1长度 D1
LONG* RS_pnD2 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//AT区间2长度 D2
LONG* RS_pnD3 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//AT区间3长度 D3
LONG* RS_pnD4 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//AT区间4长度 D4
LONG* RS_pnD5 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//AT区间5长度 D5
LONG* RS_pnQss 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//变电所AT漏抗 Qss
LONG* RS_pnQ1 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//子所1AT漏抗 Q1
LONG* RS_pnQ2 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//子所2AT漏抗 Q2
LONG* RS_pnQ3 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//子所3AT漏抗 Q3
LONG* RS_pnQ4 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//子所4AT漏抗 Q4
LONG* RS_pnQ5 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//子所5AT漏抗 Q5
LONG* RS_pnKss 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//变电所电流分布系数 Kss
LONG* RS_pnK1 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//子所1AT电流分布系数 K1
LONG* RS_pnK2 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//子所2AT电流分布系数 K2
LONG* RS_pnK3 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//子所3AT电流分布系数 K3
LONG* RS_pnK4 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//子所4AT电流分布系数 K4
LONG* RS_pnK5 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//子所5AT电流分布系数 K5
LONG* RS_pnKF 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//故障判断系数 KF
LONG* RS_pnLa 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//供电臂长度 La
LONG* RS_pnLc 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//修正距离 Lc
LONG* RS_pnLD 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//修正方向0:+,1- LD
LONG* RS_pnS0 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//变电所公里标 S0
LONG* RS_pnSD 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//公里标方向0:+,1- SD
LONG* RS_pnZOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//平行四边形偏移阻抗 ZOFFSET
LONG* RS_pnAOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//阻抗四边形偏移角度 AOFFSET
LONG* RS_pnChanType 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//通道类型 ChanType
LONG* RS_pnCAID 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//越区全局编号 CAID
LONG* RS_pnT21FLN 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//T线测距分段数 T21FLN
LONG* RS_pnT21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//T线第一段距离 T21FLN1L
LONG* RS_pnT21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//T线第一段单位电抗 T21FLN1X
LONG* RS_pnT21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//T线第二段距离 T21FLN2L
LONG* RS_pnT21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//T线第二段单位电抗 T21FLN2X
LONG* RS_pnT21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//T线第三段距离 T21FLN3L
LONG* RS_pnT21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//T线第三段单位电抗 T21FLN3X
LONG* RS_pnT21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//T线第四段距离 T21FLN4L
LONG* RS_pnT21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//T线第四段单位电抗 T21FLN4X
LONG* RS_pnT21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//T线第五段距离 T21FLN5L
LONG* RS_pnT21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//T线第五段单位电抗 T21FLN5X
LONG* RS_pnT21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//T线第六段距离 T21FLN6L
LONG* RS_pnT21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//T线第六段单位电抗 T21FLN6X
LONG* RS_pnF21FLN 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//F线测距分段数 F21FLN
LONG* RS_pnF21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//F线第一段距离 F21FLN1L
LONG* RS_pnF21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//F线第一段单位电抗 F21FLN1X
LONG* RS_pnF21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//F线第二段距离 F21FLN2L
LONG* RS_pnF21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//F线第二段单位电抗 F21FLN2X
LONG* RS_pnF21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//F线第三段距离 F21FLN3L
LONG* RS_pnF21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//F线第三段单位电抗 F21FLN3X
LONG* RS_pnF21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//F线第四段距离 F21FLN4L
LONG* RS_pnF21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//F线第四段单位电抗 F21FLN4X
LONG* RS_pnF21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//F线第五段距离 F21FLN5L
LONG* RS_pnF21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//F线第五段单位电抗 F21FLN5X
LONG* RS_pnF21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//F线第六段距离 F21FLN6L
LONG* RS_pnF21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//F线第六段单位电抗 F21FLN6X
LONG* RS_pnTF21FLN 	= 	(LONG*)(RS_BUF_ADDR + 284); 	//TF测距分段数 TF21FLN
LONG* RS_pnTF21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 288); 	//TF测距第一段距离 TF21FLN1L
LONG* RS_pnTF21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 292); 	//TF测距一段单位电抗 TF21FLN1X
LONG* RS_pnTF21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 296); 	//TF测距第二段距离 TF21FLN2L
LONG* RS_pnTF21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 300); 	//TF测距二段单位电抗 TF21FLN2X
LONG* RS_pnTF21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 304); 	//TF测距第三段距离 TF21FLN3L
LONG* RS_pnTF21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 308); 	//TF测距三段单位电抗 TF21FLN3X
LONG* RS_pnTF21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 312); 	//TF测距第四段距离 TF21FLN4L
LONG* RS_pnTF21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 316); 	//TF测距四段单位电抗 TF21FLN4X
LONG* RS_pnTF21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 320); 	//TF测距第五段距离 TF21FLN5L
LONG* RS_pnTF21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 324); 	//TF测距五段单位电抗 TF21FLN5X
LONG* RS_pnTF21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 328); 	//TF测距第六段距离 TF21FLN6L
LONG* RS_pnTF21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 332); 	//TF测距六段单位电抗 TF21FLN6X

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
LONG* AS_pn50I1 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//I1有流定值
LONG* AS_pn37I1 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//I1无流定值
LONG* AS_pn50I2 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//I2有流定值
LONG* AS_pn37I2 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//I2无流定值
LONG* AS_pn50I3 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//I3有流定值
LONG* AS_pn37I3 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//I3无流定值
LONG* AS_pn59U1 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//U1有压定值
LONG* AS_pn27U1 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//U1无压定值
LONG* AS_pn59U2 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//U2有压定值
LONG* AS_pn27U2 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//U2无压定值
LONG* AS_pn59U3 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//U3有压定值
LONG* AS_pn27U3 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U3无压定值
LONG* AS_pn59U4 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U4有压定值
LONG* AS_pn27U4 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U4无压定值
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnUPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//U变比
LONG* AS_pnI1CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//I1变比
LONG* AS_pnI2CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//I2变比
LONG* AS_pnI3CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//I3变比
LONG* AS_pnIATCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//IAT变比
LONG* AS_pnZRatio 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//阻抗变比
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//RT

//系数----------------------------------
LONG* CF_pnU1 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//U1
LONG* CF_pnU2 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U2
LONG* CF_pnU3 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U3
LONG* CF_pnU4 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//U4
LONG* CF_pnIT1 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//IT1
LONG* CF_pnIF1 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//IF1
LONG* CF_pnIT2 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//IT2
LONG* CF_pnIF2 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//IF2
LONG* CF_pnIT3 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IT3
LONG* CF_pnIF3 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IF3
LONG* CF_pnIAT1 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IAT1
LONG* CF_pnIAT2 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IAT2
LONG* CF_pnZ 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//Z
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//C4

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 100), // IN1
    (LONG*)(RM_BUF_ADDR + 104) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 92), // BR1
    (LONG*)(RM_BUF_ADDR + 96) // BR2
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

//保护数据异常恢复65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//保护启动500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//故障测距启动2400----------
const  LONG* FailureInf2400[] = {
    {NULL}
};

//上行T-R故障2401----------
const  LONG* FailureInf2401[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行F-R故障2402----------
const  LONG* FailureInf2402[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行T-R故障2427----------
const  LONG* FailureInf2427[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行F-R故障2428----------
const  LONG* FailureInf2428[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//T-F故障2403----------
const  LONG* FailureInf2403[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//子所1GPS故障2404----------
const  LONG* FailureInf2404[] = {
    (LONG*)(RM_BUF_ADDR + 108), // U1
    (LONG*)(RM_BUF_ADDR + 112), // U2
    (LONG*)(RM_BUF_ADDR + 124), // IT1
    (LONG*)(RM_BUF_ADDR + 128), // IF1
    (LONG*)(RM_BUF_ADDR + 132), // IT2
    (LONG*)(RM_BUF_ADDR + 136), // IF2
    (LONG*)(RM_BUF_ADDR + 140), // IT3
    (LONG*)(RM_BUF_ADDR + 144), // IF3
    (LONG*)(RM_BUF_ADDR + 148), // IAT1
    (LONG*)(RM_BUF_ADDR + 152), // IAT2
    (LONG*)(RM_BUF_ADDR + 156), // IAT
    (LONG*)(RM_BUF_ADDR + 160), // AIT1
    (LONG*)(RM_BUF_ADDR + 164), // AIF1
    (LONG*)(RM_BUF_ADDR + 168), // AIT2
    (LONG*)(RM_BUF_ADDR + 172), // AIF2
    (LONG*)(RM_BUF_ADDR + 176), // AIT3
    (LONG*)(RM_BUF_ADDR + 180), // AIF3
    (LONG*)(RM_BUF_ADDR + 184) // ST
};

//子所2GPS故障2405----------
const  LONG* FailureInf2405[] = {
    (LONG*)(RM_BUF_ADDR + 212), // U1
    (LONG*)(RM_BUF_ADDR + 216), // U2
    (LONG*)(RM_BUF_ADDR + 228), // IT1
    (LONG*)(RM_BUF_ADDR + 232), // IF1
    (LONG*)(RM_BUF_ADDR + 236), // IT2
    (LONG*)(RM_BUF_ADDR + 240), // IF2
    (LONG*)(RM_BUF_ADDR + 244), // IT3
    (LONG*)(RM_BUF_ADDR + 248), // IF3
    (LONG*)(RM_BUF_ADDR + 252), // IAT1
    (LONG*)(RM_BUF_ADDR + 256), // IAT2
    (LONG*)(RM_BUF_ADDR + 260), // IAT
    (LONG*)(RM_BUF_ADDR + 264), // AIT1
    (LONG*)(RM_BUF_ADDR + 268), // AIF1
    (LONG*)(RM_BUF_ADDR + 272), // AIT2
    (LONG*)(RM_BUF_ADDR + 276), // AIF2
    (LONG*)(RM_BUF_ADDR + 280), // AIT3
    (LONG*)(RM_BUF_ADDR + 284), // AIF3
    (LONG*)(RM_BUF_ADDR + 288) // ST
};

//子所3GPS故障2406----------
const  LONG* FailureInf2406[] = {
    (LONG*)(RM_BUF_ADDR + 316), // U1
    (LONG*)(RM_BUF_ADDR + 320), // U2
    (LONG*)(RM_BUF_ADDR + 332), // IT1
    (LONG*)(RM_BUF_ADDR + 336), // IF1
    (LONG*)(RM_BUF_ADDR + 340), // IT2
    (LONG*)(RM_BUF_ADDR + 344), // IF2
    (LONG*)(RM_BUF_ADDR + 348), // IT3
    (LONG*)(RM_BUF_ADDR + 352), // IF3
    (LONG*)(RM_BUF_ADDR + 356), // IAT1
    (LONG*)(RM_BUF_ADDR + 360), // IAT2
    (LONG*)(RM_BUF_ADDR + 364), // IAT
    (LONG*)(RM_BUF_ADDR + 368), // AIT1
    (LONG*)(RM_BUF_ADDR + 372), // AIF1
    (LONG*)(RM_BUF_ADDR + 376), // AIT2
    (LONG*)(RM_BUF_ADDR + 380), // AIF2
    (LONG*)(RM_BUF_ADDR + 384), // AIT3
    (LONG*)(RM_BUF_ADDR + 388), // AIF3
    (LONG*)(RM_BUF_ADDR + 392) // ST
};

//子所4GPS故障2407----------
const  LONG* FailureInf2407[] = {
    (LONG*)(RM_BUF_ADDR + 420), // U1
    (LONG*)(RM_BUF_ADDR + 424), // U2
    (LONG*)(RM_BUF_ADDR + 436), // IT1
    (LONG*)(RM_BUF_ADDR + 440), // IF1
    (LONG*)(RM_BUF_ADDR + 444), // IT2
    (LONG*)(RM_BUF_ADDR + 448), // IF2
    (LONG*)(RM_BUF_ADDR + 452), // IT3
    (LONG*)(RM_BUF_ADDR + 456), // IF3
    (LONG*)(RM_BUF_ADDR + 460), // IAT1
    (LONG*)(RM_BUF_ADDR + 464), // IAT2
    (LONG*)(RM_BUF_ADDR + 468), // IAT
    (LONG*)(RM_BUF_ADDR + 472), // AIT1
    (LONG*)(RM_BUF_ADDR + 476), // AIF1
    (LONG*)(RM_BUF_ADDR + 480), // AIT2
    (LONG*)(RM_BUF_ADDR + 484), // AIF2
    (LONG*)(RM_BUF_ADDR + 488), // AIT3
    (LONG*)(RM_BUF_ADDR + 492), // AIF3
    (LONG*)(RM_BUF_ADDR + 496) // ST
};

//子所5GPS故障2453----------
const  LONG* FailureInf2453[] = {
    (LONG*)(RM_BUF_ADDR + 524), // U1
    (LONG*)(RM_BUF_ADDR + 528), // U2
    (LONG*)(RM_BUF_ADDR + 540), // IT1
    (LONG*)(RM_BUF_ADDR + 544), // IF1
    (LONG*)(RM_BUF_ADDR + 548), // IT2
    (LONG*)(RM_BUF_ADDR + 552), // IF2
    (LONG*)(RM_BUF_ADDR + 556), // IT3
    (LONG*)(RM_BUF_ADDR + 560), // IF3
    (LONG*)(RM_BUF_ADDR + 564), // IAT1
    (LONG*)(RM_BUF_ADDR + 568), // IAT2
    (LONG*)(RM_BUF_ADDR + 572), // IAT
    (LONG*)(RM_BUF_ADDR + 576), // AIT1
    (LONG*)(RM_BUF_ADDR + 580), // AIF1
    (LONG*)(RM_BUF_ADDR + 584), // AIT2
    (LONG*)(RM_BUF_ADDR + 588), // AIF2
    (LONG*)(RM_BUF_ADDR + 592), // AIT3
    (LONG*)(RM_BUF_ADDR + 596), // AIF3
    (LONG*)(RM_BUF_ADDR + 600) // ST
};

//保护测距动作2408----------
const  LONG* FailureInf2408[] = {
    {NULL}
};

//通道测试成功2409----------
const  LONG* FailureInf2409[] = {
    {NULL}
};

//通道故障2410----------
const  LONG* FailureInf2410[] = {
    {NULL}
};

//通道1故障2411----------
const  LONG* FailureInf2411[] = {
    {NULL}
};

//通道2故障2412----------
const  LONG* FailureInf2412[] = {
    {NULL}
};

//通道3故障2421----------
const  LONG* FailureInf2421[] = {
    {NULL}
};

//通道4故障2422----------
const  LONG* FailureInf2422[] = {
    {NULL}
};

//通道5故障2454----------
const  LONG* FailureInf2454[] = {
    {NULL}
};

//相间故障2415----------
const  LONG* FailureInf2415[] = {
    {NULL}
};

//GPS故障2416----------
const  LONG* FailureInf2416[] = {
    {NULL}
};

//通道故障测距失败2414----------
const  LONG* FailureInf2414[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//超时故障测距失败2417----------
const  LONG* FailureInf2417[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//召测测距动作2418----------
const  LONG* FailureInf2418[] = {
    {NULL}
};

//PT断线2419----------
const  LONG* FailureInf2419[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36) // If3
};

//通信测试2420----------
const  LONG* FailureInf2420[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//子所1故障数据2423----------
const  LONG* FailureInf2423[] = {
    (LONG*)(RM_BUF_ADDR + 108), // U1
    (LONG*)(RM_BUF_ADDR + 112), // U2
    (LONG*)(RM_BUF_ADDR + 124), // IT1
    (LONG*)(RM_BUF_ADDR + 128), // IF1
    (LONG*)(RM_BUF_ADDR + 132), // IT2
    (LONG*)(RM_BUF_ADDR + 136), // IF2
    (LONG*)(RM_BUF_ADDR + 140), // IT3
    (LONG*)(RM_BUF_ADDR + 144), // IF3
    (LONG*)(RM_BUF_ADDR + 148), // IAT1
    (LONG*)(RM_BUF_ADDR + 152), // IAT2
    (LONG*)(RM_BUF_ADDR + 156), // IAT
    (LONG*)(RM_BUF_ADDR + 160), // AIT1
    (LONG*)(RM_BUF_ADDR + 164), // AIF1
    (LONG*)(RM_BUF_ADDR + 168), // AIT2
    (LONG*)(RM_BUF_ADDR + 172), // AIF2
    (LONG*)(RM_BUF_ADDR + 176), // AIT3
    (LONG*)(RM_BUF_ADDR + 180), // AIF3
    (LONG*)(RM_BUF_ADDR + 184) // ST
};

//子所2故障数据2424----------
const  LONG* FailureInf2424[] = {
    (LONG*)(RM_BUF_ADDR + 212), // U1
    (LONG*)(RM_BUF_ADDR + 216), // U2
    (LONG*)(RM_BUF_ADDR + 228), // IT1
    (LONG*)(RM_BUF_ADDR + 232), // IF1
    (LONG*)(RM_BUF_ADDR + 236), // IT2
    (LONG*)(RM_BUF_ADDR + 240), // IF2
    (LONG*)(RM_BUF_ADDR + 244), // IT3
    (LONG*)(RM_BUF_ADDR + 248), // IF3
    (LONG*)(RM_BUF_ADDR + 252), // IAT1
    (LONG*)(RM_BUF_ADDR + 256), // IAT2
    (LONG*)(RM_BUF_ADDR + 260), // IAT
    (LONG*)(RM_BUF_ADDR + 264), // AIT1
    (LONG*)(RM_BUF_ADDR + 268), // AIF1
    (LONG*)(RM_BUF_ADDR + 272), // AIT2
    (LONG*)(RM_BUF_ADDR + 276), // AIF2
    (LONG*)(RM_BUF_ADDR + 280), // AIT3
    (LONG*)(RM_BUF_ADDR + 284), // AIF3
    (LONG*)(RM_BUF_ADDR + 288) // ST
};

//子所3故障数据2425----------
const  LONG* FailureInf2425[] = {
    (LONG*)(RM_BUF_ADDR + 316), // U1
    (LONG*)(RM_BUF_ADDR + 320), // U2
    (LONG*)(RM_BUF_ADDR + 332), // IT1
    (LONG*)(RM_BUF_ADDR + 336), // IF1
    (LONG*)(RM_BUF_ADDR + 340), // IT2
    (LONG*)(RM_BUF_ADDR + 344), // IF2
    (LONG*)(RM_BUF_ADDR + 348), // IT3
    (LONG*)(RM_BUF_ADDR + 352), // IF3
    (LONG*)(RM_BUF_ADDR + 356), // IAT1
    (LONG*)(RM_BUF_ADDR + 360), // IAT2
    (LONG*)(RM_BUF_ADDR + 364), // IAT
    (LONG*)(RM_BUF_ADDR + 368), // AIT1
    (LONG*)(RM_BUF_ADDR + 372), // AIF1
    (LONG*)(RM_BUF_ADDR + 376), // AIT2
    (LONG*)(RM_BUF_ADDR + 380), // AIF2
    (LONG*)(RM_BUF_ADDR + 384), // AIT3
    (LONG*)(RM_BUF_ADDR + 388), // AIF3
    (LONG*)(RM_BUF_ADDR + 392) // ST
};

//子所4故障数据2426----------
const  LONG* FailureInf2426[] = {
    (LONG*)(RM_BUF_ADDR + 420), // U1
    (LONG*)(RM_BUF_ADDR + 424), // U2
    (LONG*)(RM_BUF_ADDR + 436), // IT1
    (LONG*)(RM_BUF_ADDR + 440), // IF1
    (LONG*)(RM_BUF_ADDR + 444), // IT2
    (LONG*)(RM_BUF_ADDR + 448), // IF2
    (LONG*)(RM_BUF_ADDR + 452), // IT3
    (LONG*)(RM_BUF_ADDR + 456), // IF3
    (LONG*)(RM_BUF_ADDR + 460), // IAT1
    (LONG*)(RM_BUF_ADDR + 464), // IAT2
    (LONG*)(RM_BUF_ADDR + 468), // IAT
    (LONG*)(RM_BUF_ADDR + 472), // AIT1
    (LONG*)(RM_BUF_ADDR + 476), // AIF1
    (LONG*)(RM_BUF_ADDR + 480), // AIT2
    (LONG*)(RM_BUF_ADDR + 484), // AIF2
    (LONG*)(RM_BUF_ADDR + 488), // AIT3
    (LONG*)(RM_BUF_ADDR + 492), // AIF3
    (LONG*)(RM_BUF_ADDR + 496) // ST
};

//子所5故障数据2452----------
const  LONG* FailureInf2452[] = {
    (LONG*)(RM_BUF_ADDR + 524), // U1
    (LONG*)(RM_BUF_ADDR + 528), // U2
    (LONG*)(RM_BUF_ADDR + 540), // IT1
    (LONG*)(RM_BUF_ADDR + 544), // IF1
    (LONG*)(RM_BUF_ADDR + 548), // IT2
    (LONG*)(RM_BUF_ADDR + 552), // IF2
    (LONG*)(RM_BUF_ADDR + 556), // IT3
    (LONG*)(RM_BUF_ADDR + 560), // IF3
    (LONG*)(RM_BUF_ADDR + 564), // IAT1
    (LONG*)(RM_BUF_ADDR + 568), // IAT2
    (LONG*)(RM_BUF_ADDR + 572), // IAT
    (LONG*)(RM_BUF_ADDR + 576), // AIT1
    (LONG*)(RM_BUF_ADDR + 580), // AIF1
    (LONG*)(RM_BUF_ADDR + 584), // AIT2
    (LONG*)(RM_BUF_ADDR + 588), // AIF2
    (LONG*)(RM_BUF_ADDR + 592), // AIT3
    (LONG*)(RM_BUF_ADDR + 596), // AIF3
    (LONG*)(RM_BUF_ADDR + 600) // ST
};

//GPS故障测距失败2429----------
const  LONG* FailureInf2429[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//子所1数据丢失2430----------
const  LONG* FailureInf2430[] = {
    (LONG*)(RM_BUF_ADDR + 108), // U1
    (LONG*)(RM_BUF_ADDR + 112), // U2
    (LONG*)(RM_BUF_ADDR + 124), // IT1
    (LONG*)(RM_BUF_ADDR + 128), // IF1
    (LONG*)(RM_BUF_ADDR + 132), // IT2
    (LONG*)(RM_BUF_ADDR + 136), // IF2
    (LONG*)(RM_BUF_ADDR + 140), // IT3
    (LONG*)(RM_BUF_ADDR + 144), // IF3
    (LONG*)(RM_BUF_ADDR + 148), // IAT1
    (LONG*)(RM_BUF_ADDR + 152), // IAT2
    (LONG*)(RM_BUF_ADDR + 156), // IAT
    (LONG*)(RM_BUF_ADDR + 160), // AIT1
    (LONG*)(RM_BUF_ADDR + 164), // AIF1
    (LONG*)(RM_BUF_ADDR + 168), // AIT2
    (LONG*)(RM_BUF_ADDR + 172), // AIF2
    (LONG*)(RM_BUF_ADDR + 176), // AIT3
    (LONG*)(RM_BUF_ADDR + 180), // AIF3
    (LONG*)(RM_BUF_ADDR + 184) // ST
};

//子所2数据丢失2431----------
const  LONG* FailureInf2431[] = {
    (LONG*)(RM_BUF_ADDR + 212), // U1
    (LONG*)(RM_BUF_ADDR + 216), // U2
    (LONG*)(RM_BUF_ADDR + 228), // IT1
    (LONG*)(RM_BUF_ADDR + 232), // IF1
    (LONG*)(RM_BUF_ADDR + 236), // IT2
    (LONG*)(RM_BUF_ADDR + 240), // IF2
    (LONG*)(RM_BUF_ADDR + 244), // IT3
    (LONG*)(RM_BUF_ADDR + 248), // IF3
    (LONG*)(RM_BUF_ADDR + 252), // IAT1
    (LONG*)(RM_BUF_ADDR + 256), // IAT2
    (LONG*)(RM_BUF_ADDR + 260), // IAT
    (LONG*)(RM_BUF_ADDR + 264), // AIT1
    (LONG*)(RM_BUF_ADDR + 268), // AIF1
    (LONG*)(RM_BUF_ADDR + 272), // AIT2
    (LONG*)(RM_BUF_ADDR + 276), // AIF2
    (LONG*)(RM_BUF_ADDR + 280), // AIT3
    (LONG*)(RM_BUF_ADDR + 284), // AIF3
    (LONG*)(RM_BUF_ADDR + 288) // ST
};

//子所3数据丢失2432----------
const  LONG* FailureInf2432[] = {
    (LONG*)(RM_BUF_ADDR + 316), // U1
    (LONG*)(RM_BUF_ADDR + 320), // U2
    (LONG*)(RM_BUF_ADDR + 332), // IT1
    (LONG*)(RM_BUF_ADDR + 336), // IF1
    (LONG*)(RM_BUF_ADDR + 340), // IT2
    (LONG*)(RM_BUF_ADDR + 344), // IF2
    (LONG*)(RM_BUF_ADDR + 348), // IT3
    (LONG*)(RM_BUF_ADDR + 352), // IF3
    (LONG*)(RM_BUF_ADDR + 356), // IAT1
    (LONG*)(RM_BUF_ADDR + 360), // IAT2
    (LONG*)(RM_BUF_ADDR + 364), // IAT
    (LONG*)(RM_BUF_ADDR + 368), // AIT1
    (LONG*)(RM_BUF_ADDR + 372), // AIF1
    (LONG*)(RM_BUF_ADDR + 376), // AIT2
    (LONG*)(RM_BUF_ADDR + 380), // AIF2
    (LONG*)(RM_BUF_ADDR + 384), // AIT3
    (LONG*)(RM_BUF_ADDR + 388), // AIF3
    (LONG*)(RM_BUF_ADDR + 392) // ST
};

//子所4数据丢失2433----------
const  LONG* FailureInf2433[] = {
    (LONG*)(RM_BUF_ADDR + 420), // U1
    (LONG*)(RM_BUF_ADDR + 424), // U2
    (LONG*)(RM_BUF_ADDR + 436), // IT1
    (LONG*)(RM_BUF_ADDR + 440), // IF1
    (LONG*)(RM_BUF_ADDR + 444), // IT2
    (LONG*)(RM_BUF_ADDR + 448), // IF2
    (LONG*)(RM_BUF_ADDR + 452), // IT3
    (LONG*)(RM_BUF_ADDR + 456), // IF3
    (LONG*)(RM_BUF_ADDR + 460), // IAT1
    (LONG*)(RM_BUF_ADDR + 464), // IAT2
    (LONG*)(RM_BUF_ADDR + 468), // IAT
    (LONG*)(RM_BUF_ADDR + 472), // AIT1
    (LONG*)(RM_BUF_ADDR + 476), // AIF1
    (LONG*)(RM_BUF_ADDR + 480), // AIT2
    (LONG*)(RM_BUF_ADDR + 484), // AIF2
    (LONG*)(RM_BUF_ADDR + 488), // AIT3
    (LONG*)(RM_BUF_ADDR + 492), // AIF3
    (LONG*)(RM_BUF_ADDR + 496) // ST
};

//子所5数据丢失2451----------
const  LONG* FailureInf2451[] = {
    (LONG*)(RM_BUF_ADDR + 524), // U1
    (LONG*)(RM_BUF_ADDR + 528), // U2
    (LONG*)(RM_BUF_ADDR + 540), // IT1
    (LONG*)(RM_BUF_ADDR + 544), // IF1
    (LONG*)(RM_BUF_ADDR + 548), // IT2
    (LONG*)(RM_BUF_ADDR + 552), // IF2
    (LONG*)(RM_BUF_ADDR + 556), // IT3
    (LONG*)(RM_BUF_ADDR + 560), // IF3
    (LONG*)(RM_BUF_ADDR + 564), // IAT1
    (LONG*)(RM_BUF_ADDR + 568), // IAT2
    (LONG*)(RM_BUF_ADDR + 572), // IAT
    (LONG*)(RM_BUF_ADDR + 576), // AIT1
    (LONG*)(RM_BUF_ADDR + 580), // AIF1
    (LONG*)(RM_BUF_ADDR + 584), // AIT2
    (LONG*)(RM_BUF_ADDR + 588), // AIF2
    (LONG*)(RM_BUF_ADDR + 592), // AIT3
    (LONG*)(RM_BUF_ADDR + 596), // AIF3
    (LONG*)(RM_BUF_ADDR + 600) // ST
};

//故障测距失败2434----------
const  LONG* FailureInf2434[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//GPS故障测距失败2449----------
const  LONG* FailureInf2449[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//越区切换成功2435----------
const  LONG* FailureInf2435[] = {
    {NULL}
};

//越区切换失败2436----------
const  LONG* FailureInf2436[] = {
    {NULL}
};

//超时故障测距失败2445----------
const  LONG* FailureInf2445[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行TR故障-电抗测距2437----------
const  LONG* FailureInf2437[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行FR故障-电抗测距2438----------
const  LONG* FailureInf2438[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行TR故障-电抗测距2439----------
const  LONG* FailureInf2439[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行FR故障-电抗测距2440----------
const  LONG* FailureInf2440[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//保护启动2441----------
const  LONG* FailureInf2441[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 8), // U3
    (LONG*)(RM_BUF_ADDR + 12), // U4
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 60), // AU3
    (LONG*)(RM_BUF_ADDR + 64), // AU4
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行T-R故障2442----------
const  LONG* FailureInf2442[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行F-R故障2443----------
const  LONG* FailureInf2443[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行T-R故障2447----------
const  LONG* FailureInf2447[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行F-R故障2448----------
const  LONG* FailureInf2448[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//T-F故障2444----------
const  LONG* FailureInf2444[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//通信测试2446----------
const  LONG* FailureInf2446[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//故障测距失败2450----------
const  LONG* FailureInf2450[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//保护数据异常22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//供电方式异常2455----------
const  LONG* FailureInf2455[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行T-F故障2456----------
const  LONG* FailureInf2456[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行T-F故障2457----------
const  LONG* FailureInf2457[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//上行T-F故障2458----------
const  LONG* FailureInf2458[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//下行T-F故障2459----------
const  LONG* FailureInf2459[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
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
    {0, FailureInf65, 65},//保护数据异常恢复
    {0, FailureInf500, 500},//保护启动
    {0, FailureInf2400, 2400},//故障测距启动
    {31, FailureInf2401, 2401},//上行T-R故障
    {31, FailureInf2402, 2402},//上行F-R故障
    {31, FailureInf2427, 2427},//下行T-R故障
    {31, FailureInf2428, 2428},//下行F-R故障
    {31, FailureInf2403, 2403},//T-F故障
    {18, FailureInf2404, 2404},//子所1GPS故障
    {18, FailureInf2405, 2405},//子所2GPS故障
    {18, FailureInf2406, 2406},//子所3GPS故障
    {18, FailureInf2407, 2407},//子所4GPS故障
    {18, FailureInf2453, 2453},//子所5GPS故障
    {0, FailureInf2408, 2408},//保护测距动作
    {0, FailureInf2409, 2409},//通道测试成功
    {0, FailureInf2410, 2410},//通道故障
    {0, FailureInf2411, 2411},//通道1故障
    {0, FailureInf2412, 2412},//通道2故障
    {0, FailureInf2421, 2421},//通道3故障
    {0, FailureInf2422, 2422},//通道4故障
    {0, FailureInf2454, 2454},//通道5故障
    {0, FailureInf2415, 2415},//相间故障
    {0, FailureInf2416, 2416},//GPS故障
    {31, FailureInf2414, 2414},//通道故障测距失败
    {31, FailureInf2417, 2417},//超时故障测距失败
    {0, FailureInf2418, 2418},//召测测距动作
    {8, FailureInf2419, 2419},//PT断线
    {31, FailureInf2420, 2420},//通信测试
    {18, FailureInf2423, 2423},//子所1故障数据
    {18, FailureInf2424, 2424},//子所2故障数据
    {18, FailureInf2425, 2425},//子所3故障数据
    {18, FailureInf2426, 2426},//子所4故障数据
    {18, FailureInf2452, 2452},//子所5故障数据
    {31, FailureInf2429, 2429},//GPS故障测距失败
    {18, FailureInf2430, 2430},//子所1数据丢失
    {18, FailureInf2431, 2431},//子所2数据丢失
    {18, FailureInf2432, 2432},//子所3数据丢失
    {18, FailureInf2433, 2433},//子所4数据丢失
    {18, FailureInf2451, 2451},//子所5数据丢失
    {31, FailureInf2434, 2434},//故障测距失败
    {27, FailureInf2449, 2449},//GPS故障测距失败
    {0, FailureInf2435, 2435},//越区切换成功
    {0, FailureInf2436, 2436},//越区切换失败
    {27, FailureInf2445, 2445},//超时故障测距失败
    {31, FailureInf2437, 2437},//上行TR故障-电抗测距
    {31, FailureInf2438, 2438},//上行FR故障-电抗测距
    {31, FailureInf2439, 2439},//下行TR故障-电抗测距
    {31, FailureInf2440, 2440},//下行FR故障-电抗测距
    {23, FailureInf2441, 2441},//保护启动
    {27, FailureInf2442, 2442},//上行T-R故障
    {27, FailureInf2443, 2443},//上行F-R故障
    {27, FailureInf2447, 2447},//下行T-R故障
    {27, FailureInf2448, 2448},//下行F-R故障
    {27, FailureInf2444, 2444},//T-F故障
    {27, FailureInf2446, 2446},//通信测试
    {27, FailureInf2450, 2450},//故障测距失败
    {0, FailureInf22, 22},//保护数据异常
    {31, FailureInf2455, 2455},//供电方式异常
    {27, FailureInf2456, 2456},//上行T-F故障
    {27, FailureInf2457, 2457},//下行T-F故障
    {31, FailureInf2458, 2458},//上行T-F故障
    {31, FailureInf2459, 2459},//下行T-F故障
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
8,8,8,8,8,8,8,8,// 大于等于继电器
9,9,9,9,9,9,9,// 常数时间继电器
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 144) ,
	 (LONG*)(AS_BUF_ADDR + 148) ,
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
};
//压板继电器映射到压板号压板号
const WORD  SWIndexMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 12) ,
	 (BYTE*)(CFG_BUF_ADDR + 13) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 11) ,
	 (BYTE*)(CFG_BUF_ADDR + 16) ,
	 (BYTE*)(CFG_BUF_ADDR + 17) ,
	 (BYTE*)(CFG_BUF_ADDR + 18) ,
	 (BYTE*)(CFG_BUF_ADDR + 19) ,
	 (BYTE*)(CFG_BUF_ADDR + 20) ,
	 (BYTE*)(CFG_BUF_ADDR + 21) ,
	 (BYTE*)(CFG_BUF_ADDR + 22) ,
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	114, //U1
	114, //U2
	114, //U3
	114, //U4
	305, //IT1
	306, //IF1
	304, //IT2
	304, //IF2
	304, //IT3
	304, //IF3
	304, //IAT1
	304, //IAT2
	752, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};
//系数2
const LONG CFDefaultVal2[]=
{
	114, //U1
	114, //U2
	114, //U3
	114, //U4
	1525, //IT1
	1525, //IF1
	1524, //IT2
	1525, //IF2
	1525, //IT3
	1525, //IF3
	1525, //IAT1
	1525, //IAT2
	752, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};

//传动----------------------------------
