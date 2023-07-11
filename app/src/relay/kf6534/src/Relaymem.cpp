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
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
};

//保护测量值定义-----------------------
LONG* 	RM_pnUp1 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// Up1
LONG* 	RM_pnUp2 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// Up2
LONG* 	RM_pnUp3 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// Up3
LONG* 	RM_pnUp4 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// Up4
LONG* 	RM_pnUp5 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// Up5
LONG* 	RM_pnUp6 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// Up6
LONG* 	RM_pnUp7 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// Up7
LONG* 	RM_pnUp8 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// Up8
LONG* 	RM_pnIp1 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// Ip1
LONG* 	RM_pnIp2 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// Ip2
LONG* 	RM_pnIp3 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// Ip3
LONG* 	RM_pnIp4 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Ip4
LONG* 	RM_pnIp5 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Ip5
LONG* 	RM_pnIp6 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// Ip6
LONG* 	RM_pnIp7 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// Ip7
LONG* 	RM_pnIp8 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// Ip8
LONG* 	RM_pnIC1 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// IC1
LONG* 	RM_pnIC2 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IC2
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// IN2


//测量值定义-----------------------
LONG* 	M_pnUm1 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//Um1 Um1
LONG* 	M_pnUm2 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//Um2 Um2
LONG* 	M_pnUm3 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//Um3 Um3
LONG* 	M_pnUm4 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//Um4 Um4
LONG* 	M_pnUm5 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Um5 Um5
LONG* 	M_pnUm6 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Um6 Um6
LONG* 	M_pnUm7 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Um7 Um7
LONG* 	M_pnUm8 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//Um8 Um8
LONG* 	M_pnIm1 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//Im1 Im1
LONG* 	M_pnIm2 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//Im2 Im2
LONG* 	M_pnIm3 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//Im3 Im3
LONG* 	M_pnIm4 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Im4 Im4
LONG* 	M_pnIm5 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Im5 Im5
LONG* 	M_pnIm6 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//Im6 Im6
LONG* 	M_pnIm7 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//Im7 Im7
LONG* 	M_pnIm8 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//Im8 Im8


//软压板----------------------------------
BYTE* SW_pbyBZT 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//备自投功能
BYTE* SW_pbyACAT1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//1AT自动控制
BYTE* SW_pbyACAT2 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//2AT自动控制
BYTE* SW_pby60I_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//1AT差流保护
BYTE* SW_pby50T_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//1ATT线过流
BYTE* SW_pby50F_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//1ATF线过流
BYTE* SW_pby50CS_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//1AT碰壳过流
BYTE* SW_pby60I_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//2AT差流保护
BYTE* SW_pby50T_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//2ATT线过流
BYTE* SW_pby50F_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//2ATF线过流
BYTE* SW_pby50CS_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//2AT碰壳过流
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//检修压板
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//备用压板1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 13); 	//备用压板2
BYTE* SW_pbyBAK3 	= 	(BYTE*)(SW_BUF_ADDR + 14); 	//备用压板3
BYTE* SW_pbyBAK4 	= 	(BYTE*)(SW_BUF_ADDR + 15); 	//备用压板4

//配置----------------------------------
BYTE* CFG_pbyBZT 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//备自投功能
BYTE* CFG_pbyREBZT 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//备自投重新投入
BYTE* CFG_pbyACAT1 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//1AT自动控制
BYTE* CFG_pbyACAT2 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//2AT自动控制
BYTE* CFG_pby60I_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//1AT差流保护
BYTE* CFG_pby50T_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//1ATT线过流
BYTE* CFG_pby50F_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//1ATF线过流
BYTE* CFG_pby50CS_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//1AT碰壳过流
BYTE* CFG_pby60I_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//2AT差流保护
BYTE* CFG_pby50T_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//2ATT线过流
BYTE* CFG_pby50F_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//2ATF线过流
BYTE* CFG_pby50CS_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//2AT碰壳过流
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//失灵保护
BYTE* CFG_pby74PT1 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//PT1断线检查
BYTE* CFG_pby74PT2 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//PT2断线检查
BYTE* CFG_pby74PT3 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//PT3断线检查
BYTE* CFG_pby74PT4 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//PT4断线检查
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//备用配置1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//备用配置2
BYTE* CFG_pbyBAK3 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//备用配置3
BYTE* CFG_pbyBAK4 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//备用配置4

//定值----------------------------------
LONG* RS_pnF 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//配置字 F
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//突变量启动电流 PIC
LONG* RS_pn60I_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//1AT差流定值 60I_AT1C
LONG* RS_pn60I_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//1AT差流动作时间 60I_AT1T
LONG* RS_pn50T_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//1ATT线过流定值 50T_AT1C
LONG* RS_pn50T_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//1ATT线过流动作时间 50T_AT1T
LONG* RS_pn50F_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//1ATF线过流定值 50F_AT1C
LONG* RS_pn50F_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//1ATF线过流动作时间 50F_AT1T
LONG* RS_pn50CS_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//1AT碰壳过流定值 50CS_AT1C
LONG* RS_pn50CS_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//1AT碰壳过流动作时间 50CS_AT1T
LONG* RS_pn60I_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//2AT差流定值 60I_AT2C
LONG* RS_pn60I_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//2AT差流动作时间 60I_AT2T
LONG* RS_pn50T_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//2ATT线过流定值 50T_AT2C
LONG* RS_pn50T_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//2ATT线过流动作时间 50T_AT2T
LONG* RS_pn50F_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//2ATF线过流定值 50F_AT2C
LONG* RS_pn50F_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//2ATF线过流动作时间 50F_AT2T
LONG* RS_pn50CS_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//2AT碰壳过流定值 50CS_AT2C
LONG* RS_pn50CS_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//2AT碰壳过流动作时间 50CS_AT2T
LONG* RS_pn59U1 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//U1有压 59U1
LONG* RS_pn27U1 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//U1无压 27U1
LONG* RS_pn59U2 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//U2有压 59U2
LONG* RS_pn27U2 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//U2无压 27U2
LONG* RS_pn59U3 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//U3有压 59U3
LONG* RS_pn27U3 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//U3无压 27U3
LONG* RS_pn59U4 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//U4有压 59U4
LONG* RS_pn27U4 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//U4无压 27U4
LONG* RS_pn59U5 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//U5有压 59U5
LONG* RS_pn27U5 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//U5无压 27U5
LONG* RS_pn59U6 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//U6有压 59U6
LONG* RS_pn27U6 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//U6无压 27U6
LONG* RS_pn59U7 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//U7有压 59U7
LONG* RS_pn27U7 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//U7无压 27U7
LONG* RS_pn59U8 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//U8有压 59U8
LONG* RS_pn27U8 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//U8无压 27U8
LONG* RS_pn50I1 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//I1有流 50I1
LONG* RS_pn37I1 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//I1无流 37I1
LONG* RS_pn50I2 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//I2有流 50I2
LONG* RS_pn37I2 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//I2无流 37I2
LONG* RS_pn50I3 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//I3有流 50I3
LONG* RS_pn37I3 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//I3无流 37I3
LONG* RS_pn50I4 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//I4有流 50I4
LONG* RS_pn37I4 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//I4无流 37I4
LONG* RS_pn50I5 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//I5有流 50I5
LONG* RS_pn37I5 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//I5无流 37I5
LONG* RS_pn50I6 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//I6有流 50I6
LONG* RS_pn37I6 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//I6无流 37I6
LONG* RS_pn50I7 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//I7有流 50I7
LONG* RS_pn37I7 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//I7无流 37I7
LONG* RS_pn50I8 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//I8有流 50I8
LONG* RS_pn37I8 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//I8无流 37I8
LONG* RS_pnT1PI 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//T1启动延时 T1PI
LONG* RS_pnT1RE 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//T1返回延时 T1RE
LONG* RS_pnT2PI 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//T2启动延时 T2PI
LONG* RS_pnT2RE 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//T2返回延时 T2RE
LONG* RS_pnT3PI 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//T3启动延时 T3PI
LONG* RS_pnT3RE 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//T3返回延时 T3RE
LONG* RS_pnT4PI 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//T4启动延时 T4PI
LONG* RS_pnT4RE 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//T4返回延时 T4RE
LONG* RS_pnT5PI 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//T5启动延时 T5PI
LONG* RS_pnT5RE 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//T5返回延时 T5RE
LONG* RS_pnT6PI 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//T6启动延时 T6PI
LONG* RS_pnT6RE 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//T6返回延时 T6RE
LONG* RS_pnT7PI 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//T7启动延时 T7PI
LONG* RS_pnT7RE 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//T7返回延时 T7RE
LONG* RS_pnT8PI 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//T8启动延时 T8PI
LONG* RS_pnT8RE 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//T8返回延时 T8RE
LONG* RS_pnT9PI 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//T9启动延时 T9PI
LONG* RS_pnT9RE 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//T9返回延时 T9RE
LONG* RS_pnT10PI 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//T10启动延时 T10PI
LONG* RS_pnT10RE 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//T10返回延时 T10RE
LONG* RS_pnT11PI 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//T11启动延时 T11PI
LONG* RS_pnT11RE 	= 	(LONG*)(RS_BUF_ADDR + 284); 	//T11返回延时 T11RE
LONG* RS_pnT12PI 	= 	(LONG*)(RS_BUF_ADDR + 288); 	//T12启动延时 T12PI
LONG* RS_pnT12RE 	= 	(LONG*)(RS_BUF_ADDR + 292); 	//T12返回延时 T12RE
LONG* RS_pnT13PI 	= 	(LONG*)(RS_BUF_ADDR + 296); 	//T13启动延时 T13PI
LONG* RS_pnT13RE 	= 	(LONG*)(RS_BUF_ADDR + 300); 	//T13返回延时 T13RE
LONG* RS_pnT14PI 	= 	(LONG*)(RS_BUF_ADDR + 304); 	//T14启动延时 T14PI
LONG* RS_pnT14RE 	= 	(LONG*)(RS_BUF_ADDR + 308); 	//T14返回延时 T14RE
LONG* RS_pnT15PI 	= 	(LONG*)(RS_BUF_ADDR + 312); 	//T15启动延时 T15PI
LONG* RS_pnT15RE 	= 	(LONG*)(RS_BUF_ADDR + 316); 	//T15返回延时 T15RE
LONG* RS_pnT16PI 	= 	(LONG*)(RS_BUF_ADDR + 320); 	//T16启动延时 T16PI
LONG* RS_pnT16RE 	= 	(LONG*)(RS_BUF_ADDR + 324); 	//T16返回延时 T16RE
LONG* RS_pnT17PI 	= 	(LONG*)(RS_BUF_ADDR + 328); 	//T17启动延时 T17PI
LONG* RS_pnT17RE 	= 	(LONG*)(RS_BUF_ADDR + 332); 	//T17返回延时 T17RE
LONG* RS_pnT18PI 	= 	(LONG*)(RS_BUF_ADDR + 336); 	//T18启动延时 T18PI
LONG* RS_pnT18RE 	= 	(LONG*)(RS_BUF_ADDR + 340); 	//T18返回延时 T18RE
LONG* RS_pnT19PI 	= 	(LONG*)(RS_BUF_ADDR + 344); 	//T19启动延时 T19PI
LONG* RS_pnT19RE 	= 	(LONG*)(RS_BUF_ADDR + 348); 	//T19返回延时 T19RE
LONG* RS_pnT20PI 	= 	(LONG*)(RS_BUF_ADDR + 352); 	//T20启动延时 T20PI
LONG* RS_pnT20RE 	= 	(LONG*)(RS_BUF_ADDR + 356); 	//T20返回延时 T20RE
LONG* RS_pnT21PI 	= 	(LONG*)(RS_BUF_ADDR + 360); 	//T21启动延时 T21PI
LONG* RS_pnT21RE 	= 	(LONG*)(RS_BUF_ADDR + 364); 	//T21返回延时 T21RE
LONG* RS_pnT22PI 	= 	(LONG*)(RS_BUF_ADDR + 368); 	//T22启动延时 T22PI
LONG* RS_pnT22RE 	= 	(LONG*)(RS_BUF_ADDR + 372); 	//T22返回延时 T22RE
LONG* RS_pnT23PI 	= 	(LONG*)(RS_BUF_ADDR + 376); 	//T23启动延时 T23PI
LONG* RS_pnT23RE 	= 	(LONG*)(RS_BUF_ADDR + 380); 	//T23返回延时 T23RE
LONG* RS_pnT24PI 	= 	(LONG*)(RS_BUF_ADDR + 384); 	//T24启动延时 T24PI
LONG* RS_pnT24RE 	= 	(LONG*)(RS_BUF_ADDR + 388); 	//T24返回延时 T24RE
LONG* RS_pnT25PI 	= 	(LONG*)(RS_BUF_ADDR + 392); 	//T25启动延时 T25PI
LONG* RS_pnT25RE 	= 	(LONG*)(RS_BUF_ADDR + 396); 	//T25返回延时 T25RE
LONG* RS_pnT26PI 	= 	(LONG*)(RS_BUF_ADDR + 400); 	//T26启动延时 T26PI
LONG* RS_pnT26RE 	= 	(LONG*)(RS_BUF_ADDR + 404); 	//T26返回延时 T26RE
LONG* RS_pnT27PI 	= 	(LONG*)(RS_BUF_ADDR + 408); 	//T27启动延时 T27PI
LONG* RS_pnT27RE 	= 	(LONG*)(RS_BUF_ADDR + 412); 	//T27返回延时 T27RE
LONG* RS_pnT28PI 	= 	(LONG*)(RS_BUF_ADDR + 416); 	//T28启动延时 T28PI
LONG* RS_pnT28RE 	= 	(LONG*)(RS_BUF_ADDR + 420); 	//T28返回延时 T28RE
LONG* RS_pnT29PI 	= 	(LONG*)(RS_BUF_ADDR + 424); 	//T29启动延时 T29PI
LONG* RS_pnT29RE 	= 	(LONG*)(RS_BUF_ADDR + 428); 	//T29返回延时 T29RE
LONG* RS_pnT30PI 	= 	(LONG*)(RS_BUF_ADDR + 432); 	//T30启动延时 T30PI
LONG* RS_pnT30RE 	= 	(LONG*)(RS_BUF_ADDR + 436); 	//T30返回延时 T30RE
LONG* RS_pnT31PI 	= 	(LONG*)(RS_BUF_ADDR + 440); 	//T31启动延时 T31PI
LONG* RS_pnT31RE 	= 	(LONG*)(RS_BUF_ADDR + 444); 	//T31返回延时 T31RE
LONG* RS_pnT32PI 	= 	(LONG*)(RS_BUF_ADDR + 448); 	//T32启动延时 T32PI
LONG* RS_pnT32RE 	= 	(LONG*)(RS_BUF_ADDR + 452); 	//T32返回延时 T32RE
LONG* RS_pnTCB 	= 	(LONG*)(RS_BUF_ADDR + 456); 	//断路器动作时间 TCB
LONG* RS_pnTSW 	= 	(LONG*)(RS_BUF_ADDR + 460); 	//电动隔开动作时间 TSW
LONG* RS_pnREBZTD 	= 	(LONG*)(RS_BUF_ADDR + 464); 	//自投重投入等待时间 REBZTD
LONG* RS_pnCHARD 	= 	(LONG*)(RS_BUF_ADDR + 468); 	//备自投充电时间 CHARD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 472); 	//失灵保护时间 52BFD

//辅助定值----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//额定电流
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//TR1保持时间
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//TR2保持时间
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//TR3保持时间
LONG* AS_pnTR4D 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//TR4保持时间
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//故障录波触发时间

//变比----------------------------------
LONG* AS_pnPT1Ratio 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//PT1变比
LONG* AS_pnPT2Ratio 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//PT2变比
LONG* AS_pnPT3Ratio 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//PT3变比
LONG* AS_pnPT4Ratio 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//PT4变比
LONG* AS_pnPT5Ratio 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//PT5变比
LONG* AS_pnPT6Ratio 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//PT6变比
LONG* AS_pnPT7Ratio 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//PT7变比
LONG* AS_pnPT8Ratio 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//PT8变比
LONG* AS_pnCT1Ratio 	= 	(LONG*)(AS_BUF_ADDR + 56); 	//CT1变比
LONG* AS_pnCT2Ratio 	= 	(LONG*)(AS_BUF_ADDR + 60); 	//CT2变比
LONG* AS_pnCT3Ratio 	= 	(LONG*)(AS_BUF_ADDR + 64); 	//CT3变比
LONG* AS_pnCT4Ratio 	= 	(LONG*)(AS_BUF_ADDR + 68); 	//CT4变比
LONG* AS_pnCT5Ratio 	= 	(LONG*)(AS_BUF_ADDR + 72); 	//CT5变比
LONG* AS_pnCT6Ratio 	= 	(LONG*)(AS_BUF_ADDR + 76); 	//CT6变比
LONG* AS_pnCT7Ratio 	= 	(LONG*)(AS_BUF_ADDR + 80); 	//CT7变比
LONG* AS_pnCT8Ratio 	= 	(LONG*)(AS_BUF_ADDR + 84); 	//CT8变比
LONG* AS_pnRT1 	= 	(LONG*)(AS_BUF_ADDR + 88); 	//RT1

//系数----------------------------------
LONG* CF_pnUP1 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UP1
LONG* CF_pnUP2 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UP2
LONG* CF_pnUP3 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//UP3
LONG* CF_pnUP4 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//UP4
LONG* CF_pnUP5 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//UP5
LONG* CF_pnUP6 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UP6
LONG* CF_pnUP7 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//UP7
LONG* CF_pnUP8 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UP8
LONG* CF_pnIP1 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IP1
LONG* CF_pnIP2 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IP2
LONG* CF_pnIP3 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IP3
LONG* CF_pnIP4 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IP4
LONG* CF_pnIP5 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IP5
LONG* CF_pnIP6 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IP6
LONG* CF_pnIP7 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//IP7
LONG* CF_pnIP8 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//IP8
LONG* CF_pnIC1 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IC1
LONG* CF_pnIC2 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//IC2
LONG* CF_pnUM1 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//UM1
LONG* CF_pnUM2 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//UM2
LONG* CF_pnUM3 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//UM3
LONG* CF_pnUM4 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//UM4
LONG* CF_pnUM5 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//UM5
LONG* CF_pnUM6 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//UM6
LONG* CF_pnUM7 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//UM7
LONG* CF_pnUM8 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//UM8
LONG* CF_pnIM1 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//IM1
LONG* CF_pnIM2 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//IM2
LONG* CF_pnIM3 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//IM3
LONG* CF_pnIM4 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//IM4
LONG* CF_pnIM5 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//IM5
LONG* CF_pnIM6 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IM6
LONG* CF_pnIM7 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IM7
LONG* CF_pnIM8 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//IM8
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//C4
LONG* CF_pnU1D 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//U1D
LONG* CF_pnU2D 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//U2D
LONG* CF_pnU3D 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//U3D
LONG* CF_pnU4D 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//U4D
LONG* CF_pnU5D 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//U5D
LONG* CF_pnU6D 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//U6D
LONG* CF_pnU7D 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//U7D
LONG* CF_pnU8D 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//U8D
LONG* CF_pnI1D 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//I1D
LONG* CF_pnI2D 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//I2D
LONG* CF_pnI3D 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//I3D
LONG* CF_pnI4D 	= 	(LONG*)(CF_BUF_ADDR + 196); 	//I4D
LONG* CF_pnI5D 	= 	(LONG*)(CF_BUF_ADDR + 200); 	//I5D
LONG* CF_pnI6D 	= 	(LONG*)(CF_BUF_ADDR + 204); 	//I6D
LONG* CF_pnI7D 	= 	(LONG*)(CF_BUF_ADDR + 208); 	//I7D
LONG* CF_pnI8D 	= 	(LONG*)(CF_BUF_ADDR + 212); 	//I8D

//故障电量引用定义------------------------------------
//开出光隔失效1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 80), // IN1
    (LONG*)(RM_BUF_ADDR + 84) // IN2
};

//开出光隔击穿2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 72), // BR1
    (LONG*)(RM_BUF_ADDR + 76) // BR2
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

//通道测试19----------
const  LONG* FailureInf19[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
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

//备自投启动1500----------
const  LONG* FailureInf1500[] = {
    {NULL}
};

//1#变故障启动自投1501----------
const  LONG* FailureInf1501[] = {
    {NULL}
};

//2#变故障启动自投1502----------
const  LONG* FailureInf1502[] = {
    {NULL}
};

//1#进线失压启动自投1503----------
const  LONG* FailureInf1503[] = {
    {NULL}
};

//2#进线失压启动自投1504----------
const  LONG* FailureInf1504[] = {
    {NULL}
};

//1#进线失压且1#变故障1505----------
const  LONG* FailureInf1505[] = {
    {NULL}
};

//1#进线失压且2#变故障1506----------
const  LONG* FailureInf1506[] = {
    {NULL}
};

//2#进线失压且2#变故障1507----------
const  LONG* FailureInf1507[] = {
    {NULL}
};

//2#进线失压且1#变故障1508----------
const  LONG* FailureInf1508[] = {
    {NULL}
};

//备投退出1509----------
const  LONG* FailureInf1509[] = {
    {NULL}
};

//1#变故障1510----------
const  LONG* FailureInf1510[] = {
    {NULL}
};

//2#变故障1511----------
const  LONG* FailureInf1511[] = {
    {NULL}
};

//1#进线失压1512----------
const  LONG* FailureInf1512[] = {
    {NULL}
};

//2#进线失压1513----------
const  LONG* FailureInf1513[] = {
    {NULL}
};

//备自投成功1514----------
const  LONG* FailureInf1514[] = {
    {NULL}
};

//备自投失败 1515----------
const  LONG* FailureInf1515[] = {
    {NULL}
};

//PT1断线1516----------
const  LONG* FailureInf1516[] = {
    {NULL}
};

//PT2断线1517----------
const  LONG* FailureInf1517[] = {
    {NULL}
};

//PT3断线1518----------
const  LONG* FailureInf1518[] = {
    {NULL}
};

//PT4断线1519----------
const  LONG* FailureInf1519[] = {
    {NULL}
};

//装置被闭锁1520----------
const  LONG* FailureInf1520[] = {
    {NULL}
};

//闭锁被清除1521----------
const  LONG* FailureInf1521[] = {
    {NULL}
};

//1AT差流动作1522----------
const  LONG* FailureInf1522[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//1ATT线过流动作1523----------
const  LONG* FailureInf1523[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//1ATF线过流动作1524----------
const  LONG* FailureInf1524[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//1AT碰壳过流保护动作1525----------
const  LONG* FailureInf1525[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2AT差流动作1526----------
const  LONG* FailureInf1526[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2ATT线过流保护动作1527----------
const  LONG* FailureInf1527[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2ATF线过流保护动作1528----------
const  LONG* FailureInf1528[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2AT碰壳过流保护动作1529----------
const  LONG* FailureInf1529[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//3AT差流动作1530----------
const  LONG* FailureInf1530[] = {
    {NULL}
};

//3ATT线过流动作1531----------
const  LONG* FailureInf1531[] = {
    {NULL}
};

//3ATF线过流动作1532----------
const  LONG* FailureInf1532[] = {
    {NULL}
};

//3AT碰壳过流保护动作1533----------
const  LONG* FailureInf1533[] = {
    {NULL}
};

//4AT差流动作1534----------
const  LONG* FailureInf1534[] = {
    {NULL}
};

//4ATT线过流保护动作1535----------
const  LONG* FailureInf1535[] = {
    {NULL}
};

//4ATF线过流保护动作1536----------
const  LONG* FailureInf1536[] = {
    {NULL}
};

//4AT碰壳过流保护动作1537----------
const  LONG* FailureInf1537[] = {
    {NULL}
};

//1#进线低压保护动作1538----------
const  LONG* FailureInf1538[] = {
    {NULL}
};

//2#进线低压保护动作1539----------
const  LONG* FailureInf1539[] = {
    {NULL}
};

//分1断路器1540----------
const  LONG* FailureInf1540[] = {
    {NULL}
};

//合1断路器1541----------
const  LONG* FailureInf1541[] = {
    {NULL}
};

//分1断路器失败1542----------
const  LONG* FailureInf1542[] = {
    {NULL}
};

//合1断路器失败1543----------
const  LONG* FailureInf1543[] = {
    {NULL}
};

//分2断路器1544----------
const  LONG* FailureInf1544[] = {
    {NULL}
};

//合2断路器1545----------
const  LONG* FailureInf1545[] = {
    {NULL}
};

//分2断路器失败1546----------
const  LONG* FailureInf1546[] = {
    {NULL}
};

//合2断路器失败1547----------
const  LONG* FailureInf1547[] = {
    {NULL}
};

//分3断路器1548----------
const  LONG* FailureInf1548[] = {
    {NULL}
};

//合3断路器1549----------
const  LONG* FailureInf1549[] = {
    {NULL}
};

//分3断路器失败1550----------
const  LONG* FailureInf1550[] = {
    {NULL}
};

//合3断路器失败1551----------
const  LONG* FailureInf1551[] = {
    {NULL}
};

//分4断路器1552----------
const  LONG* FailureInf1552[] = {
    {NULL}
};

//合4断路器1553----------
const  LONG* FailureInf1553[] = {
    {NULL}
};

//分4断路器失败1554----------
const  LONG* FailureInf1554[] = {
    {NULL}
};

//合4断路器失败1555----------
const  LONG* FailureInf1555[] = {
    {NULL}
};

//分5断路器1556----------
const  LONG* FailureInf1556[] = {
    {NULL}
};

//合5断路器1557----------
const  LONG* FailureInf1557[] = {
    {NULL}
};

//分5断路器失败1558----------
const  LONG* FailureInf1558[] = {
    {NULL}
};

//合5断路器失败1559----------
const  LONG* FailureInf1559[] = {
    {NULL}
};

//分6断路器1560----------
const  LONG* FailureInf1560[] = {
    {NULL}
};

//合6断路器1561----------
const  LONG* FailureInf1561[] = {
    {NULL}
};

//分6断路器失败1562----------
const  LONG* FailureInf1562[] = {
    {NULL}
};

//合6断路器失败1563----------
const  LONG* FailureInf1563[] = {
    {NULL}
};

//分7断路器1564----------
const  LONG* FailureInf1564[] = {
    {NULL}
};

//合7断路器1565----------
const  LONG* FailureInf1565[] = {
    {NULL}
};

//分7断路器失败1566----------
const  LONG* FailureInf1566[] = {
    {NULL}
};

//合7断路器失败1567----------
const  LONG* FailureInf1567[] = {
    {NULL}
};

//分8断路器1568----------
const  LONG* FailureInf1568[] = {
    {NULL}
};

//合8断路器1569----------
const  LONG* FailureInf1569[] = {
    {NULL}
};

//分8断路器失败1570----------
const  LONG* FailureInf1570[] = {
    {NULL}
};

//合8断路器失败1571----------
const  LONG* FailureInf1571[] = {
    {NULL}
};

//分9断路器1572----------
const  LONG* FailureInf1572[] = {
    {NULL}
};

//合9断路器1573----------
const  LONG* FailureInf1573[] = {
    {NULL}
};

//分9断路器失败1574----------
const  LONG* FailureInf1574[] = {
    {NULL}
};

//合9断路器失败1575----------
const  LONG* FailureInf1575[] = {
    {NULL}
};

//分10断路器1576----------
const  LONG* FailureInf1576[] = {
    {NULL}
};

//合10断路器1577----------
const  LONG* FailureInf1577[] = {
    {NULL}
};

//分10断路器失败1578----------
const  LONG* FailureInf1578[] = {
    {NULL}
};

//合10断路器失败1579----------
const  LONG* FailureInf1579[] = {
    {NULL}
};

//分1隔开1580----------
const  LONG* FailureInf1580[] = {
    {NULL}
};

//合1隔开1581----------
const  LONG* FailureInf1581[] = {
    {NULL}
};

//分1隔开失败1582----------
const  LONG* FailureInf1582[] = {
    {NULL}
};

//合1隔开失败1583----------
const  LONG* FailureInf1583[] = {
    {NULL}
};

//分2隔开1584----------
const  LONG* FailureInf1584[] = {
    {NULL}
};

//合2隔开1585----------
const  LONG* FailureInf1585[] = {
    {NULL}
};

//分2隔开失败1586----------
const  LONG* FailureInf1586[] = {
    {NULL}
};

//合2隔开失败1587----------
const  LONG* FailureInf1587[] = {
    {NULL}
};

//分3隔开1588----------
const  LONG* FailureInf1588[] = {
    {NULL}
};

//合3隔开1589----------
const  LONG* FailureInf1589[] = {
    {NULL}
};

//分3隔开失败1590----------
const  LONG* FailureInf1590[] = {
    {NULL}
};

//合3隔开失败1591----------
const  LONG* FailureInf1591[] = {
    {NULL}
};

//分4隔开1592----------
const  LONG* FailureInf1592[] = {
    {NULL}
};

//合4隔开1593----------
const  LONG* FailureInf1593[] = {
    {NULL}
};

//分4隔开失败1594----------
const  LONG* FailureInf1594[] = {
    {NULL}
};

//合4隔开失败1595----------
const  LONG* FailureInf1595[] = {
    {NULL}
};

//分5隔开1596----------
const  LONG* FailureInf1596[] = {
    {NULL}
};

//合5隔开1597----------
const  LONG* FailureInf1597[] = {
    {NULL}
};

//分5隔开失败1598----------
const  LONG* FailureInf1598[] = {
    {NULL}
};

//合5隔开失败1599----------
const  LONG* FailureInf1599[] = {
    {NULL}
};

//分6隔开1600----------
const  LONG* FailureInf1600[] = {
    {NULL}
};

//合6隔开1601----------
const  LONG* FailureInf1601[] = {
    {NULL}
};

//分6隔开失败1602----------
const  LONG* FailureInf1602[] = {
    {NULL}
};

//合6隔开失败1603----------
const  LONG* FailureInf1603[] = {
    {NULL}
};

//分7隔开1604----------
const  LONG* FailureInf1604[] = {
    {NULL}
};

//合7隔开1605----------
const  LONG* FailureInf1605[] = {
    {NULL}
};

//分7隔开失败1606----------
const  LONG* FailureInf1606[] = {
    {NULL}
};

//合7隔开失败1607----------
const  LONG* FailureInf1607[] = {
    {NULL}
};

//分8隔开1608----------
const  LONG* FailureInf1608[] = {
    {NULL}
};

//合8隔开1609----------
const  LONG* FailureInf1609[] = {
    {NULL}
};

//分8隔开失败1610----------
const  LONG* FailureInf1610[] = {
    {NULL}
};

//合8隔开失败1611----------
const  LONG* FailureInf1611[] = {
    {NULL}
};

//分9隔开1612----------
const  LONG* FailureInf1612[] = {
    {NULL}
};

//合9隔开1613----------
const  LONG* FailureInf1613[] = {
    {NULL}
};

//分9隔开失败1614----------
const  LONG* FailureInf1614[] = {
    {NULL}
};

//合9隔开失败1615----------
const  LONG* FailureInf1615[] = {
    {NULL}
};

//分10隔开1616----------
const  LONG* FailureInf1616[] = {
    {NULL}
};

//合10隔开1617----------
const  LONG* FailureInf1617[] = {
    {NULL}
};

//分10隔开失败1618----------
const  LONG* FailureInf1618[] = {
    {NULL}
};

//合10隔开失败1619----------
const  LONG* FailureInf1619[] = {
    {NULL}
};

//分101断路器1620----------
const  LONG* FailureInf1620[] = {
    {NULL}
};

//合101断路器1621----------
const  LONG* FailureInf1621[] = {
    {NULL}
};

//分101断路器失败1622----------
const  LONG* FailureInf1622[] = {
    {NULL}
};

//合101断路器失败1623----------
const  LONG* FailureInf1623[] = {
    {NULL}
};

//分102断路器1624----------
const  LONG* FailureInf1624[] = {
    {NULL}
};

//合102断路器1625----------
const  LONG* FailureInf1625[] = {
    {NULL}
};

//分102断路器失败1626----------
const  LONG* FailureInf1626[] = {
    {NULL}
};

//合102断路器失败1627----------
const  LONG* FailureInf1627[] = {
    {NULL}
};

//分103断路器1628----------
const  LONG* FailureInf1628[] = {
    {NULL}
};

//合103断路器1629----------
const  LONG* FailureInf1629[] = {
    {NULL}
};

//分103断路器失败1630----------
const  LONG* FailureInf1630[] = {
    {NULL}
};

//合103断路器失败1631----------
const  LONG* FailureInf1631[] = {
    {NULL}
};

//分104断路器1632----------
const  LONG* FailureInf1632[] = {
    {NULL}
};

//合104断路器1633----------
const  LONG* FailureInf1633[] = {
    {NULL}
};

//分104断路器失败1634----------
const  LONG* FailureInf1634[] = {
    {NULL}
};

//合104断路器失败1635----------
const  LONG* FailureInf1635[] = {
    {NULL}
};

//分201断路器1636----------
const  LONG* FailureInf1636[] = {
    {NULL}
};

//合201断路器1637----------
const  LONG* FailureInf1637[] = {
    {NULL}
};

//分201断路器失败1638----------
const  LONG* FailureInf1638[] = {
    {NULL}
};

//合201断路器失败1639----------
const  LONG* FailureInf1639[] = {
    {NULL}
};

//分202断路器1640----------
const  LONG* FailureInf1640[] = {
    {NULL}
};

//合202断路器1641----------
const  LONG* FailureInf1641[] = {
    {NULL}
};

//分202断路器失败1642----------
const  LONG* FailureInf1642[] = {
    {NULL}
};

//合202断路器失败1643----------
const  LONG* FailureInf1643[] = {
    {NULL}
};

//分203断路器1644----------
const  LONG* FailureInf1644[] = {
    {NULL}
};

//合203断路器1645----------
const  LONG* FailureInf1645[] = {
    {NULL}
};

//分203断路器失败1646----------
const  LONG* FailureInf1646[] = {
    {NULL}
};

//合203断路器失败1647----------
const  LONG* FailureInf1647[] = {
    {NULL}
};

//分204断路器1648----------
const  LONG* FailureInf1648[] = {
    {NULL}
};

//合204断路器1649----------
const  LONG* FailureInf1649[] = {
    {NULL}
};

//分204断路器失败1650----------
const  LONG* FailureInf1650[] = {
    {NULL}
};

//合204断路器失败1651----------
const  LONG* FailureInf1651[] = {
    {NULL}
};

//分201A断路器1652----------
const  LONG* FailureInf1652[] = {
    {NULL}
};

//合201A断路器1653----------
const  LONG* FailureInf1653[] = {
    {NULL}
};

//分201A断路器失败1654----------
const  LONG* FailureInf1654[] = {
    {NULL}
};

//合201A断路器失败1655----------
const  LONG* FailureInf1655[] = {
    {NULL}
};

//分201B断路器1656----------
const  LONG* FailureInf1656[] = {
    {NULL}
};

//合201B断路器1657----------
const  LONG* FailureInf1657[] = {
    {NULL}
};

//分201B断路器失败1658----------
const  LONG* FailureInf1658[] = {
    {NULL}
};

//合201B断路器失败1659----------
const  LONG* FailureInf1659[] = {
    {NULL}
};

//分202A断路器1660----------
const  LONG* FailureInf1660[] = {
    {NULL}
};

//合202A断路器1661----------
const  LONG* FailureInf1661[] = {
    {NULL}
};

//分202A断路器失败1662----------
const  LONG* FailureInf1662[] = {
    {NULL}
};

//合202A断路器失败1663----------
const  LONG* FailureInf1663[] = {
    {NULL}
};

//分202B断路器   1664----------
const  LONG* FailureInf1664[] = {
    {NULL}
};

//合202B断路器1665----------
const  LONG* FailureInf1665[] = {
    {NULL}
};

//分202B断路器失败1666----------
const  LONG* FailureInf1666[] = {
    {NULL}
};

//合202B断路器失败1667----------
const  LONG* FailureInf1667[] = {
    {NULL}
};

//分1011隔开1668----------
const  LONG* FailureInf1668[] = {
    {NULL}
};

//合1011隔开1669----------
const  LONG* FailureInf1669[] = {
    {NULL}
};

//分2011隔开失败1670----------
const  LONG* FailureInf1670[] = {
    {NULL}
};

//合1011隔开失败1671----------
const  LONG* FailureInf1671[] = {
    {NULL}
};

//分1021隔开1672----------
const  LONG* FailureInf1672[] = {
    {NULL}
};

//合1021隔开1673----------
const  LONG* FailureInf1673[] = {
    {NULL}
};

//分1021隔开失败1674----------
const  LONG* FailureInf1674[] = {
    {NULL}
};

//合1021隔开失败1675----------
const  LONG* FailureInf1675[] = {
    {NULL}
};

//分1001隔开1676----------
const  LONG* FailureInf1676[] = {
    {NULL}
};

//合1001隔开1677----------
const  LONG* FailureInf1677[] = {
    {NULL}
};

//分1001隔开失败1678----------
const  LONG* FailureInf1678[] = {
    {NULL}
};

//合1001隔开失败1679----------
const  LONG* FailureInf1679[] = {
    {NULL}
};

//分1002隔开1680----------
const  LONG* FailureInf1680[] = {
    {NULL}
};

//合1002隔开1681----------
const  LONG* FailureInf1681[] = {
    {NULL}
};

//分1002隔开失败1682----------
const  LONG* FailureInf1682[] = {
    {NULL}
};

//合1002隔开失败1683----------
const  LONG* FailureInf1683[] = {
    {NULL}
};

//分1019隔开1684----------
const  LONG* FailureInf1684[] = {
    {NULL}
};

//合1019隔开1685----------
const  LONG* FailureInf1685[] = {
    {NULL}
};

//分1019隔开失败1686----------
const  LONG* FailureInf1686[] = {
    {NULL}
};

//合1019隔开失败1687----------
const  LONG* FailureInf1687[] = {
    {NULL}
};

//分1029隔开1688----------
const  LONG* FailureInf1688[] = {
    {NULL}
};

//合1029隔开1689----------
const  LONG* FailureInf1689[] = {
    {NULL}
};

//分1029隔开失败1690----------
const  LONG* FailureInf1690[] = {
    {NULL}
};

//合1029隔开失败1691----------
const  LONG* FailureInf1691[] = {
    {NULL}
};

//分1039隔开1692----------
const  LONG* FailureInf1692[] = {
    {NULL}
};

//合1039隔开1693----------
const  LONG* FailureInf1693[] = {
    {NULL}
};

//分1039隔开失败1694----------
const  LONG* FailureInf1694[] = {
    {NULL}
};

//合1039隔开失败1695----------
const  LONG* FailureInf1695[] = {
    {NULL}
};

//分1049隔开1696----------
const  LONG* FailureInf1696[] = {
    {NULL}
};

//合1049隔开1697----------
const  LONG* FailureInf1697[] = {
    {NULL}
};

//分1049隔开失败1698----------
const  LONG* FailureInf1698[] = {
    {NULL}
};

//合1049隔开失败1699----------
const  LONG* FailureInf1699[] = {
    {NULL}
};

//1AT重瓦斯1700----------
const  LONG* FailureInf1700[] = {
    {NULL}
};

//1AT压力释放1701----------
const  LONG* FailureInf1701[] = {
    {NULL}
};

//1AT温度过高1702----------
const  LONG* FailureInf1702[] = {
    {NULL}
};

//1AT轻瓦斯1703----------
const  LONG* FailureInf1703[] = {
    {NULL}
};

//1AT温度告警1704----------
const  LONG* FailureInf1704[] = {
    {NULL}
};

//1AT油位异常1705----------
const  LONG* FailureInf1705[] = {
    {NULL}
};

//1AT油速异常1706----------
const  LONG* FailureInf1706[] = {
    {NULL}
};

//2AT重瓦斯1707----------
const  LONG* FailureInf1707[] = {
    {NULL}
};

//2AT压力释放1708----------
const  LONG* FailureInf1708[] = {
    {NULL}
};

//2AT温度过高1709----------
const  LONG* FailureInf1709[] = {
    {NULL}
};

//2AT轻瓦斯1710----------
const  LONG* FailureInf1710[] = {
    {NULL}
};

//2AT温度告警1711----------
const  LONG* FailureInf1711[] = {
    {NULL}
};

//2AT油位异常1712----------
const  LONG* FailureInf1712[] = {
    {NULL}
};

//2AT油速异常1713----------
const  LONG* FailureInf1713[] = {
    {NULL}
};

//3AT重瓦斯1714----------
const  LONG* FailureInf1714[] = {
    {NULL}
};

//3AT压力释放1715----------
const  LONG* FailureInf1715[] = {
    {NULL}
};

//3AT温度过高1716----------
const  LONG* FailureInf1716[] = {
    {NULL}
};

//3AT轻瓦斯1717----------
const  LONG* FailureInf1717[] = {
    {NULL}
};

//3AT温度告警1718----------
const  LONG* FailureInf1718[] = {
    {NULL}
};

//3AT油位异常1719----------
const  LONG* FailureInf1719[] = {
    {NULL}
};

//3AT油速异常1720----------
const  LONG* FailureInf1720[] = {
    {NULL}
};

//4AT重瓦斯1721----------
const  LONG* FailureInf1721[] = {
    {NULL}
};

//4AT压力释放1722----------
const  LONG* FailureInf1722[] = {
    {NULL}
};

//4AT温度过高1723----------
const  LONG* FailureInf1723[] = {
    {NULL}
};

//4AT轻瓦斯1724----------
const  LONG* FailureInf1724[] = {
    {NULL}
};

//4AT温度告警1725----------
const  LONG* FailureInf1725[] = {
    {NULL}
};

//4AT油位异常1726----------
const  LONG* FailureInf1726[] = {
    {NULL}
};

//4AT油速异常1727----------
const  LONG* FailureInf1727[] = {
    {NULL}
};

//分1AT快分开关1728----------
const  LONG* FailureInf1728[] = {
    {NULL}
};

//分2AT快分开关1729----------
const  LONG* FailureInf1729[] = {
    {NULL}
};

//分3AT快分开关1730----------
const  LONG* FailureInf1730[] = {
    {NULL}
};

//分4AT快分开关1731----------
const  LONG* FailureInf1731[] = {
    {NULL}
};

//分1AT快分失败1732----------
const  LONG* FailureInf1732[] = {
    {NULL}
};

//分2AT快分失败1733----------
const  LONG* FailureInf1733[] = {
    {NULL}
};

//分3AT快分失败1734----------
const  LONG* FailureInf1734[] = {
    {NULL}
};

//分4AT快分失败1735----------
const  LONG* FailureInf1735[] = {
    {NULL}
};

//动作成功1736----------
const  LONG* FailureInf1736[] = {
    {NULL}
};

//动作失败1737----------
const  LONG* FailureInf1737[] = {
    {NULL}
};

//检有压合闸成功1738----------
const  LONG* FailureInf1738[] = {
    {NULL}
};

//检有合闸失败1739----------
const  LONG* FailureInf1739[] = {
    {NULL}
};

//分241断路器1740----------
const  LONG* FailureInf1740[] = {
    {NULL}
};

//合241断路器1741----------
const  LONG* FailureInf1741[] = {
    {NULL}
};

//分241断路器失败1742----------
const  LONG* FailureInf1742[] = {
    {NULL}
};

//合241断路器失败1743----------
const  LONG* FailureInf1743[] = {
    {NULL}
};

//分242断路器1744----------
const  LONG* FailureInf1744[] = {
    {NULL}
};

//合242断路器1745----------
const  LONG* FailureInf1745[] = {
    {NULL}
};

//分242断路器失败1746----------
const  LONG* FailureInf1746[] = {
    {NULL}
};

//合242断路器失败1747----------
const  LONG* FailureInf1747[] = {
    {NULL}
};

//分271断路器1748----------
const  LONG* FailureInf1748[] = {
    {NULL}
};

//合271断路器1749----------
const  LONG* FailureInf1749[] = {
    {NULL}
};

//分271断路器失败1750----------
const  LONG* FailureInf1750[] = {
    {NULL}
};

//合271断路器失败1751----------
const  LONG* FailureInf1751[] = {
    {NULL}
};

//分272断路器1752----------
const  LONG* FailureInf1752[] = {
    {NULL}
};

//合272断路器1753----------
const  LONG* FailureInf1753[] = {
    {NULL}
};

//分272断路器失败1754----------
const  LONG* FailureInf1754[] = {
    {NULL}
};

//合272断路器失败1755----------
const  LONG* FailureInf1755[] = {
    {NULL}
};

//分281断路器1756----------
const  LONG* FailureInf1756[] = {
    {NULL}
};

//合281断路器1757----------
const  LONG* FailureInf1757[] = {
    {NULL}
};

//分281断路器失败1758----------
const  LONG* FailureInf1758[] = {
    {NULL}
};

//合281断路器失败1759----------
const  LONG* FailureInf1759[] = {
    {NULL}
};

//分2411隔开1760----------
const  LONG* FailureInf1760[] = {
    {NULL}
};

//合2411隔开1761----------
const  LONG* FailureInf1761[] = {
    {NULL}
};

//分2411隔开失败1762----------
const  LONG* FailureInf1762[] = {
    {NULL}
};

//合2411隔开失败1763----------
const  LONG* FailureInf1763[] = {
    {NULL}
};

//分2421隔开1764----------
const  LONG* FailureInf1764[] = {
    {NULL}
};

//合2421隔开1765----------
const  LONG* FailureInf1765[] = {
    {NULL}
};

//分2421隔开失败1766----------
const  LONG* FailureInf1766[] = {
    {NULL}
};

//合2421隔开失败1767----------
const  LONG* FailureInf1767[] = {
    {NULL}
};

//分2711隔开1768----------
const  LONG* FailureInf1768[] = {
    {NULL}
};

//合2711隔开1769----------
const  LONG* FailureInf1769[] = {
    {NULL}
};

//分2711隔开失败1770----------
const  LONG* FailureInf1770[] = {
    {NULL}
};

//合2711隔开失败1771----------
const  LONG* FailureInf1771[] = {
    {NULL}
};

//分2721隔开1772----------
const  LONG* FailureInf1772[] = {
    {NULL}
};

//合2721隔开1773----------
const  LONG* FailureInf1773[] = {
    {NULL}
};

//分2721隔开失败1774----------
const  LONG* FailureInf1774[] = {
    {NULL}
};

//合2721隔开失败1775----------
const  LONG* FailureInf1775[] = {
    {NULL}
};

//1AT差流启动1777----------
const  LONG* FailureInf1777[] = {
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT差流返回1778----------
const  LONG* FailureInf1778[] = {
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT T线过流启动1779----------
const  LONG* FailureInf1779[] = {
    (LONG*)(RM_BUF_ADDR + 32) // Ip1
};

//1AT T线过流返回1780----------
const  LONG* FailureInf1780[] = {
    (LONG*)(RM_BUF_ADDR + 32) // Ip1
};

//1AT F线过流启动1781----------
const  LONG* FailureInf1781[] = {
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT F线过流返回1782----------
const  LONG* FailureInf1782[] = {
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT碰壳过流启动1783----------
const  LONG* FailureInf1783[] = {
    (LONG*)(RM_BUF_ADDR + 40) // Ip3
};

//1AT碰壳过流返回1784----------
const  LONG* FailureInf1784[] = {
    (LONG*)(RM_BUF_ADDR + 40) // Ip3
};

//2AT差流启动1785----------
const  LONG* FailureInf1785[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT差流返回1786----------
const  LONG* FailureInf1786[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT T线过流启动1787----------
const  LONG* FailureInf1787[] = {
    (LONG*)(RM_BUF_ADDR + 44) // Ip4
};

//2AT T线过流返回1788----------
const  LONG* FailureInf1788[] = {
    (LONG*)(RM_BUF_ADDR + 44) // Ip4
};

//2AT F线过流启动1789----------
const  LONG* FailureInf1789[] = {
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT F线过流返回1790----------
const  LONG* FailureInf1790[] = {
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT碰壳过流启动1791----------
const  LONG* FailureInf1791[] = {
    (LONG*)(RM_BUF_ADDR + 52) // Ip6
};

//2AT碰壳过流返回1792----------
const  LONG* FailureInf1792[] = {
    (LONG*)(RM_BUF_ADDR + 52) // Ip6
};

//保护启动4000----------
const  LONG* FailureInf4000[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//AT1断路器跳闸失败4001----------
const  LONG* FailureInf4001[] = {
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40) // Ip3
};

//AT2断路器跳闸失败4002----------
const  LONG* FailureInf4002[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52) // Ip6
};

//1AT故障启动自投4003----------
const  LONG* FailureInf4003[] = {
    {NULL}
};

//2AT故障启动自投4004----------
const  LONG* FailureInf4004[] = {
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
    {0, FailureInf16, 16},//NVRAM内容出错
    {0, FailureInf17, 17},//逻辑方程故障
    {0, FailureInf18, 18},//保护自检错误
    {18, FailureInf19, 19},//通道测试
    {0, FailureInf50, 50},//AD故障恢复
    {0, FailureInf51, 51},//软压板故障恢复
    {0, FailureInf52, 52},//EEPROM定值故障恢复
    {0, FailureInf55, 55},//RAM定值故障恢复
    {0, FailureInf58, 58},//程序ROM故障恢复
    {0, FailureInf60, 60},//通信通道异常恢复
    {0, FailureInf62, 62},//NVRAM内容出错恢复
    {0, FailureInf63, 63},//逻辑方程故障故障恢复
    {0, FailureInf500, 500},//保护启动
    {0, FailureInf1500, 1500},//备自投启动
    {0, FailureInf1501, 1501},//1#变故障启动自投
    {0, FailureInf1502, 1502},//2#变故障启动自投
    {0, FailureInf1503, 1503},//1#进线失压启动自投
    {0, FailureInf1504, 1504},//2#进线失压启动自投
    {0, FailureInf1505, 1505},//1#进线失压且1#变故障
    {0, FailureInf1506, 1506},//1#进线失压且2#变故障
    {0, FailureInf1507, 1507},//2#进线失压且2#变故障
    {0, FailureInf1508, 1508},//2#进线失压且1#变故障
    {0, FailureInf1509, 1509},//备投退出
    {0, FailureInf1510, 1510},//1#变故障
    {0, FailureInf1511, 1511},//2#变故障
    {0, FailureInf1512, 1512},//1#进线失压
    {0, FailureInf1513, 1513},//2#进线失压
    {0, FailureInf1514, 1514},//备自投成功
    {0, FailureInf1515, 1515},//备自投失败 
    {0, FailureInf1516, 1516},//PT1断线
    {0, FailureInf1517, 1517},//PT2断线
    {0, FailureInf1518, 1518},//PT3断线
    {0, FailureInf1519, 1519},//PT4断线
    {0, FailureInf1520, 1520},//装置被闭锁
    {0, FailureInf1521, 1521},//闭锁被清除
    {18, FailureInf1522, 1522},//1AT差流动作
    {18, FailureInf1523, 1523},//1ATT线过流动作
    {18, FailureInf1524, 1524},//1ATF线过流动作
    {18, FailureInf1525, 1525},//1AT碰壳过流保护动作
    {18, FailureInf1526, 1526},//2AT差流动作
    {18, FailureInf1527, 1527},//2ATT线过流保护动作
    {18, FailureInf1528, 1528},//2ATF线过流保护动作
    {18, FailureInf1529, 1529},//2AT碰壳过流保护动作
    {0, FailureInf1530, 1530},//3AT差流动作
    {0, FailureInf1531, 1531},//3ATT线过流动作
    {0, FailureInf1532, 1532},//3ATF线过流动作
    {0, FailureInf1533, 1533},//3AT碰壳过流保护动作
    {0, FailureInf1534, 1534},//4AT差流动作
    {0, FailureInf1535, 1535},//4ATT线过流保护动作
    {0, FailureInf1536, 1536},//4ATF线过流保护动作
    {0, FailureInf1537, 1537},//4AT碰壳过流保护动作
    {0, FailureInf1538, 1538},//1#进线低压保护动作
    {0, FailureInf1539, 1539},//2#进线低压保护动作
    {0, FailureInf1540, 1540},//分1断路器
    {0, FailureInf1541, 1541},//合1断路器
    {0, FailureInf1542, 1542},//分1断路器失败
    {0, FailureInf1543, 1543},//合1断路器失败
    {0, FailureInf1544, 1544},//分2断路器
    {0, FailureInf1545, 1545},//合2断路器
    {0, FailureInf1546, 1546},//分2断路器失败
    {0, FailureInf1547, 1547},//合2断路器失败
    {0, FailureInf1548, 1548},//分3断路器
    {0, FailureInf1549, 1549},//合3断路器
    {0, FailureInf1550, 1550},//分3断路器失败
    {0, FailureInf1551, 1551},//合3断路器失败
    {0, FailureInf1552, 1552},//分4断路器
    {0, FailureInf1553, 1553},//合4断路器
    {0, FailureInf1554, 1554},//分4断路器失败
    {0, FailureInf1555, 1555},//合4断路器失败
    {0, FailureInf1556, 1556},//分5断路器
    {0, FailureInf1557, 1557},//合5断路器
    {0, FailureInf1558, 1558},//分5断路器失败
    {0, FailureInf1559, 1559},//合5断路器失败
    {0, FailureInf1560, 1560},//分6断路器
    {0, FailureInf1561, 1561},//合6断路器
    {0, FailureInf1562, 1562},//分6断路器失败
    {0, FailureInf1563, 1563},//合6断路器失败
    {0, FailureInf1564, 1564},//分7断路器
    {0, FailureInf1565, 1565},//合7断路器
    {0, FailureInf1566, 1566},//分7断路器失败
    {0, FailureInf1567, 1567},//合7断路器失败
    {0, FailureInf1568, 1568},//分8断路器
    {0, FailureInf1569, 1569},//合8断路器
    {0, FailureInf1570, 1570},//分8断路器失败
    {0, FailureInf1571, 1571},//合8断路器失败
    {0, FailureInf1572, 1572},//分9断路器
    {0, FailureInf1573, 1573},//合9断路器
    {0, FailureInf1574, 1574},//分9断路器失败
    {0, FailureInf1575, 1575},//合9断路器失败
    {0, FailureInf1576, 1576},//分10断路器
    {0, FailureInf1577, 1577},//合10断路器
    {0, FailureInf1578, 1578},//分10断路器失败
    {0, FailureInf1579, 1579},//合10断路器失败
    {0, FailureInf1580, 1580},//分1隔开
    {0, FailureInf1581, 1581},//合1隔开
    {0, FailureInf1582, 1582},//分1隔开失败
    {0, FailureInf1583, 1583},//合1隔开失败
    {0, FailureInf1584, 1584},//分2隔开
    {0, FailureInf1585, 1585},//合2隔开
    {0, FailureInf1586, 1586},//分2隔开失败
    {0, FailureInf1587, 1587},//合2隔开失败
    {0, FailureInf1588, 1588},//分3隔开
    {0, FailureInf1589, 1589},//合3隔开
    {0, FailureInf1590, 1590},//分3隔开失败
    {0, FailureInf1591, 1591},//合3隔开失败
    {0, FailureInf1592, 1592},//分4隔开
    {0, FailureInf1593, 1593},//合4隔开
    {0, FailureInf1594, 1594},//分4隔开失败
    {0, FailureInf1595, 1595},//合4隔开失败
    {0, FailureInf1596, 1596},//分5隔开
    {0, FailureInf1597, 1597},//合5隔开
    {0, FailureInf1598, 1598},//分5隔开失败
    {0, FailureInf1599, 1599},//合5隔开失败
    {0, FailureInf1600, 1600},//分6隔开
    {0, FailureInf1601, 1601},//合6隔开
    {0, FailureInf1602, 1602},//分6隔开失败
    {0, FailureInf1603, 1603},//合6隔开失败
    {0, FailureInf1604, 1604},//分7隔开
    {0, FailureInf1605, 1605},//合7隔开
    {0, FailureInf1606, 1606},//分7隔开失败
    {0, FailureInf1607, 1607},//合7隔开失败
    {0, FailureInf1608, 1608},//分8隔开
    {0, FailureInf1609, 1609},//合8隔开
    {0, FailureInf1610, 1610},//分8隔开失败
    {0, FailureInf1611, 1611},//合8隔开失败
    {0, FailureInf1612, 1612},//分9隔开
    {0, FailureInf1613, 1613},//合9隔开
    {0, FailureInf1614, 1614},//分9隔开失败
    {0, FailureInf1615, 1615},//合9隔开失败
    {0, FailureInf1616, 1616},//分10隔开
    {0, FailureInf1617, 1617},//合10隔开
    {0, FailureInf1618, 1618},//分10隔开失败
    {0, FailureInf1619, 1619},//合10隔开失败
    {0, FailureInf1620, 1620},//分101断路器
    {0, FailureInf1621, 1621},//合101断路器
    {0, FailureInf1622, 1622},//分101断路器失败
    {0, FailureInf1623, 1623},//合101断路器失败
    {0, FailureInf1624, 1624},//分102断路器
    {0, FailureInf1625, 1625},//合102断路器
    {0, FailureInf1626, 1626},//分102断路器失败
    {0, FailureInf1627, 1627},//合102断路器失败
    {0, FailureInf1628, 1628},//分103断路器
    {0, FailureInf1629, 1629},//合103断路器
    {0, FailureInf1630, 1630},//分103断路器失败
    {0, FailureInf1631, 1631},//合103断路器失败
    {0, FailureInf1632, 1632},//分104断路器
    {0, FailureInf1633, 1633},//合104断路器
    {0, FailureInf1634, 1634},//分104断路器失败
    {0, FailureInf1635, 1635},//合104断路器失败
    {0, FailureInf1636, 1636},//分201断路器
    {0, FailureInf1637, 1637},//合201断路器
    {0, FailureInf1638, 1638},//分201断路器失败
    {0, FailureInf1639, 1639},//合201断路器失败
    {0, FailureInf1640, 1640},//分202断路器
    {0, FailureInf1641, 1641},//合202断路器
    {0, FailureInf1642, 1642},//分202断路器失败
    {0, FailureInf1643, 1643},//合202断路器失败
    {0, FailureInf1644, 1644},//分203断路器
    {0, FailureInf1645, 1645},//合203断路器
    {0, FailureInf1646, 1646},//分203断路器失败
    {0, FailureInf1647, 1647},//合203断路器失败
    {0, FailureInf1648, 1648},//分204断路器
    {0, FailureInf1649, 1649},//合204断路器
    {0, FailureInf1650, 1650},//分204断路器失败
    {0, FailureInf1651, 1651},//合204断路器失败
    {0, FailureInf1652, 1652},//分201A断路器
    {0, FailureInf1653, 1653},//合201A断路器
    {0, FailureInf1654, 1654},//分201A断路器失败
    {0, FailureInf1655, 1655},//合201A断路器失败
    {0, FailureInf1656, 1656},//分201B断路器
    {0, FailureInf1657, 1657},//合201B断路器
    {0, FailureInf1658, 1658},//分201B断路器失败
    {0, FailureInf1659, 1659},//合201B断路器失败
    {0, FailureInf1660, 1660},//分202A断路器
    {0, FailureInf1661, 1661},//合202A断路器
    {0, FailureInf1662, 1662},//分202A断路器失败
    {0, FailureInf1663, 1663},//合202A断路器失败
    {0, FailureInf1664, 1664},//分202B断路器   
    {0, FailureInf1665, 1665},//合202B断路器
    {0, FailureInf1666, 1666},//分202B断路器失败
    {0, FailureInf1667, 1667},//合202B断路器失败
    {0, FailureInf1668, 1668},//分1011隔开
    {0, FailureInf1669, 1669},//合1011隔开
    {0, FailureInf1670, 1670},//分2011隔开失败
    {0, FailureInf1671, 1671},//合1011隔开失败
    {0, FailureInf1672, 1672},//分1021隔开
    {0, FailureInf1673, 1673},//合1021隔开
    {0, FailureInf1674, 1674},//分1021隔开失败
    {0, FailureInf1675, 1675},//合1021隔开失败
    {0, FailureInf1676, 1676},//分1001隔开
    {0, FailureInf1677, 1677},//合1001隔开
    {0, FailureInf1678, 1678},//分1001隔开失败
    {0, FailureInf1679, 1679},//合1001隔开失败
    {0, FailureInf1680, 1680},//分1002隔开
    {0, FailureInf1681, 1681},//合1002隔开
    {0, FailureInf1682, 1682},//分1002隔开失败
    {0, FailureInf1683, 1683},//合1002隔开失败
    {0, FailureInf1684, 1684},//分1019隔开
    {0, FailureInf1685, 1685},//合1019隔开
    {0, FailureInf1686, 1686},//分1019隔开失败
    {0, FailureInf1687, 1687},//合1019隔开失败
    {0, FailureInf1688, 1688},//分1029隔开
    {0, FailureInf1689, 1689},//合1029隔开
    {0, FailureInf1690, 1690},//分1029隔开失败
    {0, FailureInf1691, 1691},//合1029隔开失败
    {0, FailureInf1692, 1692},//分1039隔开
    {0, FailureInf1693, 1693},//合1039隔开
    {0, FailureInf1694, 1694},//分1039隔开失败
    {0, FailureInf1695, 1695},//合1039隔开失败
    {0, FailureInf1696, 1696},//分1049隔开
    {0, FailureInf1697, 1697},//合1049隔开
    {0, FailureInf1698, 1698},//分1049隔开失败
    {0, FailureInf1699, 1699},//合1049隔开失败
    {0, FailureInf1700, 1700},//1AT重瓦斯
    {0, FailureInf1701, 1701},//1AT压力释放
    {0, FailureInf1702, 1702},//1AT温度过高
    {0, FailureInf1703, 1703},//1AT轻瓦斯
    {0, FailureInf1704, 1704},//1AT温度告警
    {0, FailureInf1705, 1705},//1AT油位异常
    {0, FailureInf1706, 1706},//1AT油速异常
    {0, FailureInf1707, 1707},//2AT重瓦斯
    {0, FailureInf1708, 1708},//2AT压力释放
    {0, FailureInf1709, 1709},//2AT温度过高
    {0, FailureInf1710, 1710},//2AT轻瓦斯
    {0, FailureInf1711, 1711},//2AT温度告警
    {0, FailureInf1712, 1712},//2AT油位异常
    {0, FailureInf1713, 1713},//2AT油速异常
    {0, FailureInf1714, 1714},//3AT重瓦斯
    {0, FailureInf1715, 1715},//3AT压力释放
    {0, FailureInf1716, 1716},//3AT温度过高
    {0, FailureInf1717, 1717},//3AT轻瓦斯
    {0, FailureInf1718, 1718},//3AT温度告警
    {0, FailureInf1719, 1719},//3AT油位异常
    {0, FailureInf1720, 1720},//3AT油速异常
    {0, FailureInf1721, 1721},//4AT重瓦斯
    {0, FailureInf1722, 1722},//4AT压力释放
    {0, FailureInf1723, 1723},//4AT温度过高
    {0, FailureInf1724, 1724},//4AT轻瓦斯
    {0, FailureInf1725, 1725},//4AT温度告警
    {0, FailureInf1726, 1726},//4AT油位异常
    {0, FailureInf1727, 1727},//4AT油速异常
    {0, FailureInf1728, 1728},//分1AT快分开关
    {0, FailureInf1729, 1729},//分2AT快分开关
    {0, FailureInf1730, 1730},//分3AT快分开关
    {0, FailureInf1731, 1731},//分4AT快分开关
    {0, FailureInf1732, 1732},//分1AT快分失败
    {0, FailureInf1733, 1733},//分2AT快分失败
    {0, FailureInf1734, 1734},//分3AT快分失败
    {0, FailureInf1735, 1735},//分4AT快分失败
    {0, FailureInf1736, 1736},//动作成功
    {0, FailureInf1737, 1737},//动作失败
    {0, FailureInf1738, 1738},//检有压合闸成功
    {0, FailureInf1739, 1739},//检有合闸失败
    {0, FailureInf1740, 1740},//分241断路器
    {0, FailureInf1741, 1741},//合241断路器
    {0, FailureInf1742, 1742},//分241断路器失败
    {0, FailureInf1743, 1743},//合241断路器失败
    {0, FailureInf1744, 1744},//分242断路器
    {0, FailureInf1745, 1745},//合242断路器
    {0, FailureInf1746, 1746},//分242断路器失败
    {0, FailureInf1747, 1747},//合242断路器失败
    {0, FailureInf1748, 1748},//分271断路器
    {0, FailureInf1749, 1749},//合271断路器
    {0, FailureInf1750, 1750},//分271断路器失败
    {0, FailureInf1751, 1751},//合271断路器失败
    {0, FailureInf1752, 1752},//分272断路器
    {0, FailureInf1753, 1753},//合272断路器
    {0, FailureInf1754, 1754},//分272断路器失败
    {0, FailureInf1755, 1755},//合272断路器失败
    {0, FailureInf1756, 1756},//分281断路器
    {0, FailureInf1757, 1757},//合281断路器
    {0, FailureInf1758, 1758},//分281断路器失败
    {0, FailureInf1759, 1759},//合281断路器失败
    {0, FailureInf1760, 1760},//分2411隔开
    {0, FailureInf1761, 1761},//合2411隔开
    {0, FailureInf1762, 1762},//分2411隔开失败
    {0, FailureInf1763, 1763},//合2411隔开失败
    {0, FailureInf1764, 1764},//分2421隔开
    {0, FailureInf1765, 1765},//合2421隔开
    {0, FailureInf1766, 1766},//分2421隔开失败
    {0, FailureInf1767, 1767},//合2421隔开失败
    {0, FailureInf1768, 1768},//分2711隔开
    {0, FailureInf1769, 1769},//合2711隔开
    {0, FailureInf1770, 1770},//分2711隔开失败
    {0, FailureInf1771, 1771},//合2711隔开失败
    {0, FailureInf1772, 1772},//分2721隔开
    {0, FailureInf1773, 1773},//合2721隔开
    {0, FailureInf1774, 1774},//分2721隔开失败
    {0, FailureInf1775, 1775},//合2721隔开失败
    {2, FailureInf1777, 1777},//1AT差流启动
    {2, FailureInf1778, 1778},//1AT差流返回
    {1, FailureInf1779, 1779},//1AT T线过流启动
    {1, FailureInf1780, 1780},//1AT T线过流返回
    {1, FailureInf1781, 1781},//1AT F线过流启动
    {1, FailureInf1782, 1782},//1AT F线过流返回
    {1, FailureInf1783, 1783},//1AT碰壳过流启动
    {1, FailureInf1784, 1784},//1AT碰壳过流返回
    {2, FailureInf1785, 1785},//2AT差流启动
    {2, FailureInf1786, 1786},//2AT差流返回
    {1, FailureInf1787, 1787},//2AT T线过流启动
    {1, FailureInf1788, 1788},//2AT T线过流返回
    {1, FailureInf1789, 1789},//2AT F线过流启动
    {1, FailureInf1790, 1790},//2AT F线过流返回
    {1, FailureInf1791, 1791},//2AT碰壳过流启动
    {1, FailureInf1792, 1792},//2AT碰壳过流返回
    {18, FailureInf4000, 4000},//保护启动
    {3, FailureInf4001, 4001},//AT1断路器跳闸失败
    {3, FailureInf4002, 4002},//AT2断路器跳闸失败
    {0, FailureInf4003, 4003},//1AT故障启动自投
    {0, FailureInf4004, 4004},//2AT故障启动自投
    {0, FailureInf20, 20},//5V电源故障
    {0, FailureInf21, 21},//EEPROM写入故障
    {0, FailureInf64, 64},//5V电源故障恢复
    {0, FailureInf22, 22},//保护数据异常
    {0, FailureInf65, 65},//保护数据异常恢复
};

//继电器执行类型数组----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 保护继电器字
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 中间继电器字
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 自保持中间继电器字
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// 时间继电器字
3,3,3,3,// 跳闸继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 虚拟继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开入继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 开出继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 压板继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 配置继电器
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// 脉冲继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 传动继电器
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// 信号继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED继电器
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// 虚拟开入
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控预置继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控执行继电器
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// 遥控反校继电器
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
	{ (LONG*)(RS_BUF_ADDR + 200), (LONG*)(RS_BUF_ADDR + 204) },
	{ (LONG*)(RS_BUF_ADDR + 208), (LONG*)(RS_BUF_ADDR + 212) },
	{ (LONG*)(RS_BUF_ADDR + 216), (LONG*)(RS_BUF_ADDR + 220) },
	{ (LONG*)(RS_BUF_ADDR + 224), (LONG*)(RS_BUF_ADDR + 228) },
	{ (LONG*)(RS_BUF_ADDR + 232), (LONG*)(RS_BUF_ADDR + 236) },
	{ (LONG*)(RS_BUF_ADDR + 240), (LONG*)(RS_BUF_ADDR + 244) },
	{ (LONG*)(RS_BUF_ADDR + 248), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(RS_BUF_ADDR + 256), (LONG*)(RS_BUF_ADDR + 260) },
	{ (LONG*)(RS_BUF_ADDR + 264), (LONG*)(RS_BUF_ADDR + 268) },
	{ (LONG*)(RS_BUF_ADDR + 272), (LONG*)(RS_BUF_ADDR + 276) },
	{ (LONG*)(RS_BUF_ADDR + 280), (LONG*)(RS_BUF_ADDR + 284) },
	{ (LONG*)(RS_BUF_ADDR + 288), (LONG*)(RS_BUF_ADDR + 292) },
	{ (LONG*)(RS_BUF_ADDR + 296), (LONG*)(RS_BUF_ADDR + 300) },
	{ (LONG*)(RS_BUF_ADDR + 304), (LONG*)(RS_BUF_ADDR + 308) },
	{ (LONG*)(RS_BUF_ADDR + 312), (LONG*)(RS_BUF_ADDR + 316) },
	{ (LONG*)(RS_BUF_ADDR + 320), (LONG*)(RS_BUF_ADDR + 324) },
	{ (LONG*)(RS_BUF_ADDR + 328), (LONG*)(RS_BUF_ADDR + 332) },
	{ (LONG*)(RS_BUF_ADDR + 336), (LONG*)(RS_BUF_ADDR + 340) },
	{ (LONG*)(RS_BUF_ADDR + 344), (LONG*)(RS_BUF_ADDR + 348) },
	{ (LONG*)(RS_BUF_ADDR + 352), (LONG*)(RS_BUF_ADDR + 356) },
	{ (LONG*)(RS_BUF_ADDR + 360), (LONG*)(RS_BUF_ADDR + 364) },
	{ (LONG*)(RS_BUF_ADDR + 368), (LONG*)(RS_BUF_ADDR + 372) },
	{ (LONG*)(RS_BUF_ADDR + 376), (LONG*)(RS_BUF_ADDR + 380) },
	{ (LONG*)(RS_BUF_ADDR + 384), (LONG*)(RS_BUF_ADDR + 388) },
	{ (LONG*)(RS_BUF_ADDR + 392), (LONG*)(RS_BUF_ADDR + 396) },
	{ (LONG*)(RS_BUF_ADDR + 400), (LONG*)(RS_BUF_ADDR + 404) },
	{ (LONG*)(RS_BUF_ADDR + 408), (LONG*)(RS_BUF_ADDR + 412) },
	{ (LONG*)(RS_BUF_ADDR + 416), (LONG*)(RS_BUF_ADDR + 420) },
	{ (LONG*)(RS_BUF_ADDR + 424), (LONG*)(RS_BUF_ADDR + 428) },
	{ (LONG*)(RS_BUF_ADDR + 432), (LONG*)(RS_BUF_ADDR + 436) },
	{ (LONG*)(RS_BUF_ADDR + 440), (LONG*)(RS_BUF_ADDR + 444) },
	{ (LONG*)(RS_BUF_ADDR + 448), (LONG*)(RS_BUF_ADDR + 452) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 464) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 472) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 472) },
};
//跳闸继电器时间定值(关联到定值或辅助定值去)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 4) ,
	 (LONG*)(AS_BUF_ADDR + 8) ,
	 (LONG*)(AS_BUF_ADDR + 12) ,
	 (LONG*)(AS_BUF_ADDR + 16) ,
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
	 (WORD)(38) ,
	 (WORD)(39) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 8) ,
	 (BYTE*)(CFG_BUF_ADDR + 5) ,
	 (BYTE*)(CFG_BUF_ADDR + 6) ,
	 (BYTE*)(CFG_BUF_ADDR + 7) ,
	 (BYTE*)(CFG_BUF_ADDR + 9) ,
	 (BYTE*)(CFG_BUF_ADDR + 10) ,
	 (BYTE*)(CFG_BUF_ADDR + 11) ,
	 (BYTE*)(CFG_BUF_ADDR + 13) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 15) ,
	 (BYTE*)(CFG_BUF_ADDR + 16) ,
	 (BYTE*)(CFG_BUF_ADDR + 17) ,
	 (BYTE*)(CFG_BUF_ADDR + 18) ,
	 (BYTE*)(CFG_BUF_ADDR + 19) ,
	 (BYTE*)(CFG_BUF_ADDR + 20) ,
};
//系数默认值
//系数1
const LONG CFDefaultVal1[]=
{
	11500, //UP1
	11500, //UP2
	11500, //UP3
	11500, //UP4
	11500, //UP5
	11500, //UP6
	11500, //UP7
	11500, //UP8
	3060, //IP1
	3060, //IP2
	3060, //IP3
	3060, //IP4
	3060, //IP5
	3060, //IP6
	3060, //IP7
	3060, //IP8
	3060, //IC1
	3060, //IC2
	16100, //UM1
	16100, //UM2
	16100, //UM3
	16100, //UM4
	16100, //UM5
	16100, //UM6
	16100, //UM7
	16100, //UM8
	4336, //IM1
	4334, //IM2
	4334, //IM3
	4334, //IM4
	4335, //IM5
	4334, //IM6
	4332, //IM7
	4332, //IM8
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	11, //U1D
	11, //U2D
	11, //U3D
	12, //U4D
	12, //U5D
	11, //U6D
	11, //U7D
	11, //U8D
	3, //I1D
	3, //I2D
	3, //I3D
	3, //I4D
	3, //I5D
	3, //I6D
	3, //I7D
	3, //I8D
};
//系数2
const LONG CFDefaultVal2[]=
{
	11500, //UP1
	11500, //UP2
	11500, //UP3
	11500, //UP4
	11500, //UP5
	11500, //UP6
	11500, //UP7
	11500, //UP8
	15300, //IP1
	15300, //IP2
	15300, //IP3
	15300, //IP4
	15300, //IP5
	15300, //IP6
	15300, //IP7
	15300, //IP8
	15300, //IC1
	15300, //IC2
	16100, //UM1
	16100, //UM2
	16100, //UM3
	16100, //UM4
	16100, //UM5
	16100, //UM6
	16100, //UM7
	16100, //UM8
	21600, //IM1
	21600, //IM2
	21600, //IM3
	21600, //IM4
	21600, //IM5
	21600, //IM6
	21600, //IM7
	21600, //IM8
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	1, //U1D
	1, //U2D
	1, //U3D
	1, //U4D
	1, //U5D
	1, //U6D
	1, //U7D
	1, //U8D
	1, //I1D
	1, //I2D
	1, //I3D
	1, //I4D
	1, //I5D
	1, //I6D
	1, //I7D
	1, //I8D
};

//传动----------------------------------
