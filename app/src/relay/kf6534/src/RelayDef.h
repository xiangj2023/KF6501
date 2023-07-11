/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			RelayDef.h
* 
*   软件模块：保护接口文件
*
* 
*	描述  保护定义文件，上半部分是固定的数据结构， 下半部分由保护接口工具软件自动生成
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
*		kangqian	2010-04-21		增加压板投退继电器使用的参数数组
*************************************************************************/

#ifndef __RelayDef_H
#define __RelayDef_H


#include "sysdefs.h"


#ifdef __PL_WIN_SIMULATION__
	// 在window模拟平台中使用一个数组模拟ram 
	extern unsigned char _RAMDataBuf[];
	#define RELAY_RAM_ADDR_BASE	_RAMDataBuf
#else
#include "resm5234.h"
	//给保护用的片上ram地址
	#define RELAY_RAM_ADDR_BASE	RELAY_RAM_BASE // 硬件决定
#endif

//定值 值名称 的最大长度
#define  MAX_SETVALUNAME  256 

// 报告描述结构
typedef struct TReportInf{
    DWORD dwFialureNum;                // 故障电量个数 
    const LONG** pFailureData; // 故障电量描述信息 
	WORD wInf; //报告inf号
}TReportInf;

//逻辑方程用时间继电器时间参数（关联定值或辅助定值）
typedef struct TTimeSetting{
	LONG* pStartTime; //启动时间，指向关联的定值（或辅助定值）
	LONG* pReturnTime;	//返回时间，指向关联的定值（或辅助定值）
}TTimeSetting;

//定值取值描述列表
typedef struct TSetValueInf
{
	LONG lValue;
	char strName[MAX_SETVALUNAME];
}TSetValueInf;

//定值取值索引表
typedef struct TSetValueInfMap
{
	LONG nItem;
	TSetValueInf* pSetValueInf;
}TSetValueInfMap;

extern const WORD RelayWordTypeList[]; //继电器类型表
extern const struct TSetValueInfMap SetValueInfMap[];
extern const struct TSetValueInf SetValueInf[];

extern const struct TSetValueInfMap SysSetValueInfMap[];
extern const struct TSetValueInf SysSetValueInf[];

extern const struct TTimeSetting TimeSetting[];          // 时间继电器定值
extern  LONG*	TripSetting[]; //跳闸继电器延时定值

extern WORD const OutPutMap[]; //输出继电器-出口 映射表 保存开出号
extern WORD const InPutMap[]; //输入继电器-开入 映射表

extern BYTE* const SWMap[]; //压板映射表 直接指向关联的软压板在内存中的副本
extern BYTE* const CFGMap[]; //配置映射表

extern const struct TReportInf ReportInf[];          // 报告引用 
//系数默认值
extern const LONG CFDefaultVal1[]; //系数1
extern const LONG CFDefaultVal2[]; //系数2

//2010-4-21 11:25:33 
extern WORD const SWIndexMap[]; //压板继电器和压板号的映射关系， 保存软压板序号
                                //注意与SWMap[]的区别。SWIndexMap是为通过压板投退继电器投退软压板时使用的

//以下代码由工具软件生成


#define 	 SETTING_AREA_COUNT 	 16 		 //定值区个数
#define 	 RELAY_DATA_COUNT 	 16 		 //扰动数据区数，
#define 	 SAMPLE_COUNT 	 32 		 //每周波采样点数
#define 	 A15TYPE_SYSSETNO 	 0 		 //1,5A 类型辅助定值序号
#define 	 FAILDATA_DISPTYPE_CFGNO 	 0 		 //故障电量显示类型配置序号
#define 	 LangTYPE_SYSSETNO 	 0 		 //语言类型辅助定值序号

//当前定值区号定义-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //当前定值区号

//定值取值宏定义

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//保护测量值定义-----------------------
#define  RM_UP1 0
extern LONG* 	RM_pnUp1; 		 // Up1
#define  RM_UP2 1
extern LONG* 	RM_pnUp2; 		 // Up2
#define  RM_UP3 2
extern LONG* 	RM_pnUp3; 		 // Up3
#define  RM_UP4 3
extern LONG* 	RM_pnUp4; 		 // Up4
#define  RM_UP5 4
extern LONG* 	RM_pnUp5; 		 // Up5
#define  RM_UP6 5
extern LONG* 	RM_pnUp6; 		 // Up6
#define  RM_UP7 6
extern LONG* 	RM_pnUp7; 		 // Up7
#define  RM_UP8 7
extern LONG* 	RM_pnUp8; 		 // Up8
#define  RM_IP1 8
extern LONG* 	RM_pnIp1; 		 // Ip1
#define  RM_IP2 9
extern LONG* 	RM_pnIp2; 		 // Ip2
#define  RM_IP3 10
extern LONG* 	RM_pnIp3; 		 // Ip3
#define  RM_IP4 11
extern LONG* 	RM_pnIp4; 		 // Ip4
#define  RM_IP5 12
extern LONG* 	RM_pnIp5; 		 // Ip5
#define  RM_IP6 13
extern LONG* 	RM_pnIp6; 		 // Ip6
#define  RM_IP7 14
extern LONG* 	RM_pnIp7; 		 // Ip7
#define  RM_IP8 15
extern LONG* 	RM_pnIp8; 		 // Ip8
#define  RM_IC1 16
extern LONG* 	RM_pnIC1; 		 // IC1
#define  RM_IC2 17
extern LONG* 	RM_pnIC2; 		 // IC2
#define  RM_BOBRCH1 18
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 19
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 20
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 21
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     22    //保护测量值个数
#define RM_BUF_SIZE     88    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UM1 0
extern LONG* 	M_pnUm1; 		 //Um1 Um1
#define  M_UM2 1
extern LONG* 	M_pnUm2; 		 //Um2 Um2
#define  M_UM3 2
extern LONG* 	M_pnUm3; 		 //Um3 Um3
#define  M_UM4 3
extern LONG* 	M_pnUm4; 		 //Um4 Um4
#define  M_UM5 4
extern LONG* 	M_pnUm5; 		 //Um5 Um5
#define  M_UM6 5
extern LONG* 	M_pnUm6; 		 //Um6 Um6
#define  M_UM7 6
extern LONG* 	M_pnUm7; 		 //Um7 Um7
#define  M_UM8 7
extern LONG* 	M_pnUm8; 		 //Um8 Um8
#define  M_IM1 8
extern LONG* 	M_pnIm1; 		 //Im1 Im1
#define  M_IM2 9
extern LONG* 	M_pnIm2; 		 //Im2 Im2
#define  M_IM3 10
extern LONG* 	M_pnIm3; 		 //Im3 Im3
#define  M_IM4 11
extern LONG* 	M_pnIm4; 		 //Im4 Im4
#define  M_IM5 12
extern LONG* 	M_pnIm5; 		 //Im5 Im5
#define  M_IM6 13
extern LONG* 	M_pnIm6; 		 //Im6 Im6
#define  M_IM7 14
extern LONG* 	M_pnIm7; 		 //Im7 Im7
#define  M_IM8 15
extern LONG* 	M_pnIm8; 		 //Im8 Im8

#define MEASURE_COUNT     16    //测量值个数
#define MEASURE_BUF_SIZE     64    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 92)

//软压板----------------------------------
extern BYTE* 	SW_pbyBZT; 		 //备自投功能
extern BYTE* 	SW_pbyACAT1; 		 //1AT自动控制
extern BYTE* 	SW_pbyACAT2; 		 //2AT自动控制
extern BYTE* 	SW_pby60I_AT1; 		 //1AT差流保护
extern BYTE* 	SW_pby50T_AT1; 		 //1ATT线过流
extern BYTE* 	SW_pby50F_AT1; 		 //1ATF线过流
extern BYTE* 	SW_pby50CS_AT1; 		 //1AT碰壳过流
extern BYTE* 	SW_pby60I_AT2; 		 //2AT差流保护
extern BYTE* 	SW_pby50T_AT2; 		 //2ATT线过流
extern BYTE* 	SW_pby50F_AT2; 		 //2ATF线过流
extern BYTE* 	SW_pby50CS_AT2; 		 //2AT碰壳过流
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2
extern BYTE* 	SW_pbyBAK3; 		 //备用压板3
extern BYTE* 	SW_pbyBAK4; 		 //备用压板4

#define SW_COUNT     16    //软压板个数
#define SW_BUF_SIZE     16    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 156)

//配置----------------------------------
extern BYTE* 	CFG_pbyBZT; 		 //备自投功能
extern BYTE* 	CFG_pbyREBZT; 		 //备自投重新投入
extern BYTE* 	CFG_pbyACAT1; 		 //1AT自动控制
extern BYTE* 	CFG_pbyACAT2; 		 //2AT自动控制
extern BYTE* 	CFG_pby60I_AT1; 		 //1AT差流保护
extern BYTE* 	CFG_pby50T_AT1; 		 //1ATT线过流
extern BYTE* 	CFG_pby50F_AT1; 		 //1ATF线过流
extern BYTE* 	CFG_pby50CS_AT1; 		 //1AT碰壳过流
extern BYTE* 	CFG_pby60I_AT2; 		 //2AT差流保护
extern BYTE* 	CFG_pby50T_AT2; 		 //2ATT线过流
extern BYTE* 	CFG_pby50F_AT2; 		 //2ATF线过流
extern BYTE* 	CFG_pby50CS_AT2; 		 //2AT碰壳过流
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pby74PT1; 		 //PT1断线检查
extern BYTE* 	CFG_pby74PT2; 		 //PT2断线检查
extern BYTE* 	CFG_pby74PT3; 		 //PT3断线检查
extern BYTE* 	CFG_pby74PT4; 		 //PT4断线检查
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2
extern BYTE* 	CFG_pbyBAK3; 		 //备用配置3
extern BYTE* 	CFG_pbyBAK4; 		 //备用配置4

#define CFG_COUNT     21    //配置个数/
#define CFG_BUF_SIZE     21    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 172)

//定值----------------------------------
extern LONG* 	RS_pnF; 		 //配置字 F
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn60I_AT1C; 		 //1AT差流定值 60I_AT1C
extern LONG* 	RS_pn60I_AT1T; 		 //1AT差流动作时间 60I_AT1T
extern LONG* 	RS_pn50T_AT1C; 		 //1ATT线过流定值 50T_AT1C
extern LONG* 	RS_pn50T_AT1T; 		 //1ATT线过流动作时间 50T_AT1T
extern LONG* 	RS_pn50F_AT1C; 		 //1ATF线过流定值 50F_AT1C
extern LONG* 	RS_pn50F_AT1T; 		 //1ATF线过流动作时间 50F_AT1T
extern LONG* 	RS_pn50CS_AT1C; 		 //1AT碰壳过流定值 50CS_AT1C
extern LONG* 	RS_pn50CS_AT1T; 		 //1AT碰壳过流动作时间 50CS_AT1T
extern LONG* 	RS_pn60I_AT2C; 		 //2AT差流定值 60I_AT2C
extern LONG* 	RS_pn60I_AT2T; 		 //2AT差流动作时间 60I_AT2T
extern LONG* 	RS_pn50T_AT2C; 		 //2ATT线过流定值 50T_AT2C
extern LONG* 	RS_pn50T_AT2T; 		 //2ATT线过流动作时间 50T_AT2T
extern LONG* 	RS_pn50F_AT2C; 		 //2ATF线过流定值 50F_AT2C
extern LONG* 	RS_pn50F_AT2T; 		 //2ATF线过流动作时间 50F_AT2T
extern LONG* 	RS_pn50CS_AT2C; 		 //2AT碰壳过流定值 50CS_AT2C
extern LONG* 	RS_pn50CS_AT2T; 		 //2AT碰壳过流动作时间 50CS_AT2T
extern LONG* 	RS_pn59U1; 		 //U1有压 59U1
extern LONG* 	RS_pn27U1; 		 //U1无压 27U1
extern LONG* 	RS_pn59U2; 		 //U2有压 59U2
extern LONG* 	RS_pn27U2; 		 //U2无压 27U2
extern LONG* 	RS_pn59U3; 		 //U3有压 59U3
extern LONG* 	RS_pn27U3; 		 //U3无压 27U3
extern LONG* 	RS_pn59U4; 		 //U4有压 59U4
extern LONG* 	RS_pn27U4; 		 //U4无压 27U4
extern LONG* 	RS_pn59U5; 		 //U5有压 59U5
extern LONG* 	RS_pn27U5; 		 //U5无压 27U5
extern LONG* 	RS_pn59U6; 		 //U6有压 59U6
extern LONG* 	RS_pn27U6; 		 //U6无压 27U6
extern LONG* 	RS_pn59U7; 		 //U7有压 59U7
extern LONG* 	RS_pn27U7; 		 //U7无压 27U7
extern LONG* 	RS_pn59U8; 		 //U8有压 59U8
extern LONG* 	RS_pn27U8; 		 //U8无压 27U8
extern LONG* 	RS_pn50I1; 		 //I1有流 50I1
extern LONG* 	RS_pn37I1; 		 //I1无流 37I1
extern LONG* 	RS_pn50I2; 		 //I2有流 50I2
extern LONG* 	RS_pn37I2; 		 //I2无流 37I2
extern LONG* 	RS_pn50I3; 		 //I3有流 50I3
extern LONG* 	RS_pn37I3; 		 //I3无流 37I3
extern LONG* 	RS_pn50I4; 		 //I4有流 50I4
extern LONG* 	RS_pn37I4; 		 //I4无流 37I4
extern LONG* 	RS_pn50I5; 		 //I5有流 50I5
extern LONG* 	RS_pn37I5; 		 //I5无流 37I5
extern LONG* 	RS_pn50I6; 		 //I6有流 50I6
extern LONG* 	RS_pn37I6; 		 //I6无流 37I6
extern LONG* 	RS_pn50I7; 		 //I7有流 50I7
extern LONG* 	RS_pn37I7; 		 //I7无流 37I7
extern LONG* 	RS_pn50I8; 		 //I8有流 50I8
extern LONG* 	RS_pn37I8; 		 //I8无流 37I8
extern LONG* 	RS_pnT1PI; 		 //T1启动延时 T1PI
extern LONG* 	RS_pnT1RE; 		 //T1返回延时 T1RE
extern LONG* 	RS_pnT2PI; 		 //T2启动延时 T2PI
extern LONG* 	RS_pnT2RE; 		 //T2返回延时 T2RE
extern LONG* 	RS_pnT3PI; 		 //T3启动延时 T3PI
extern LONG* 	RS_pnT3RE; 		 //T3返回延时 T3RE
extern LONG* 	RS_pnT4PI; 		 //T4启动延时 T4PI
extern LONG* 	RS_pnT4RE; 		 //T4返回延时 T4RE
extern LONG* 	RS_pnT5PI; 		 //T5启动延时 T5PI
extern LONG* 	RS_pnT5RE; 		 //T5返回延时 T5RE
extern LONG* 	RS_pnT6PI; 		 //T6启动延时 T6PI
extern LONG* 	RS_pnT6RE; 		 //T6返回延时 T6RE
extern LONG* 	RS_pnT7PI; 		 //T7启动延时 T7PI
extern LONG* 	RS_pnT7RE; 		 //T7返回延时 T7RE
extern LONG* 	RS_pnT8PI; 		 //T8启动延时 T8PI
extern LONG* 	RS_pnT8RE; 		 //T8返回延时 T8RE
extern LONG* 	RS_pnT9PI; 		 //T9启动延时 T9PI
extern LONG* 	RS_pnT9RE; 		 //T9返回延时 T9RE
extern LONG* 	RS_pnT10PI; 		 //T10启动延时 T10PI
extern LONG* 	RS_pnT10RE; 		 //T10返回延时 T10RE
extern LONG* 	RS_pnT11PI; 		 //T11启动延时 T11PI
extern LONG* 	RS_pnT11RE; 		 //T11返回延时 T11RE
extern LONG* 	RS_pnT12PI; 		 //T12启动延时 T12PI
extern LONG* 	RS_pnT12RE; 		 //T12返回延时 T12RE
extern LONG* 	RS_pnT13PI; 		 //T13启动延时 T13PI
extern LONG* 	RS_pnT13RE; 		 //T13返回延时 T13RE
extern LONG* 	RS_pnT14PI; 		 //T14启动延时 T14PI
extern LONG* 	RS_pnT14RE; 		 //T14返回延时 T14RE
extern LONG* 	RS_pnT15PI; 		 //T15启动延时 T15PI
extern LONG* 	RS_pnT15RE; 		 //T15返回延时 T15RE
extern LONG* 	RS_pnT16PI; 		 //T16启动延时 T16PI
extern LONG* 	RS_pnT16RE; 		 //T16返回延时 T16RE
extern LONG* 	RS_pnT17PI; 		 //T17启动延时 T17PI
extern LONG* 	RS_pnT17RE; 		 //T17返回延时 T17RE
extern LONG* 	RS_pnT18PI; 		 //T18启动延时 T18PI
extern LONG* 	RS_pnT18RE; 		 //T18返回延时 T18RE
extern LONG* 	RS_pnT19PI; 		 //T19启动延时 T19PI
extern LONG* 	RS_pnT19RE; 		 //T19返回延时 T19RE
extern LONG* 	RS_pnT20PI; 		 //T20启动延时 T20PI
extern LONG* 	RS_pnT20RE; 		 //T20返回延时 T20RE
extern LONG* 	RS_pnT21PI; 		 //T21启动延时 T21PI
extern LONG* 	RS_pnT21RE; 		 //T21返回延时 T21RE
extern LONG* 	RS_pnT22PI; 		 //T22启动延时 T22PI
extern LONG* 	RS_pnT22RE; 		 //T22返回延时 T22RE
extern LONG* 	RS_pnT23PI; 		 //T23启动延时 T23PI
extern LONG* 	RS_pnT23RE; 		 //T23返回延时 T23RE
extern LONG* 	RS_pnT24PI; 		 //T24启动延时 T24PI
extern LONG* 	RS_pnT24RE; 		 //T24返回延时 T24RE
extern LONG* 	RS_pnT25PI; 		 //T25启动延时 T25PI
extern LONG* 	RS_pnT25RE; 		 //T25返回延时 T25RE
extern LONG* 	RS_pnT26PI; 		 //T26启动延时 T26PI
extern LONG* 	RS_pnT26RE; 		 //T26返回延时 T26RE
extern LONG* 	RS_pnT27PI; 		 //T27启动延时 T27PI
extern LONG* 	RS_pnT27RE; 		 //T27返回延时 T27RE
extern LONG* 	RS_pnT28PI; 		 //T28启动延时 T28PI
extern LONG* 	RS_pnT28RE; 		 //T28返回延时 T28RE
extern LONG* 	RS_pnT29PI; 		 //T29启动延时 T29PI
extern LONG* 	RS_pnT29RE; 		 //T29返回延时 T29RE
extern LONG* 	RS_pnT30PI; 		 //T30启动延时 T30PI
extern LONG* 	RS_pnT30RE; 		 //T30返回延时 T30RE
extern LONG* 	RS_pnT31PI; 		 //T31启动延时 T31PI
extern LONG* 	RS_pnT31RE; 		 //T31返回延时 T31RE
extern LONG* 	RS_pnT32PI; 		 //T32启动延时 T32PI
extern LONG* 	RS_pnT32RE; 		 //T32返回延时 T32RE
extern LONG* 	RS_pnTCB; 		 //断路器动作时间 TCB
extern LONG* 	RS_pnTSW; 		 //电动隔开动作时间 TSW
extern LONG* 	RS_pnREBZTD; 		 //自投重投入等待时间 REBZTD
extern LONG* 	RS_pnCHARD; 		 //备自投充电时间 CHARD
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD

#define RS_COUNT     119    //定值个数/
#define RS_BUF_SIZE     476    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 194)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnTR3D; 		 //TR3保持时间
extern LONG* 	AS_pnTR4D; 		 //TR4保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnPT1Ratio; 		 //PT1变比
extern LONG* 	AS_pnPT2Ratio; 		 //PT2变比
extern LONG* 	AS_pnPT3Ratio; 		 //PT3变比
extern LONG* 	AS_pnPT4Ratio; 		 //PT4变比
extern LONG* 	AS_pnPT5Ratio; 		 //PT5变比
extern LONG* 	AS_pnPT6Ratio; 		 //PT6变比
extern LONG* 	AS_pnPT7Ratio; 		 //PT7变比
extern LONG* 	AS_pnPT8Ratio; 		 //PT8变比
extern LONG* 	AS_pnCT1Ratio; 		 //CT1变比
extern LONG* 	AS_pnCT2Ratio; 		 //CT2变比
extern LONG* 	AS_pnCT3Ratio; 		 //CT3变比
extern LONG* 	AS_pnCT4Ratio; 		 //CT4变比
extern LONG* 	AS_pnCT5Ratio; 		 //CT5变比
extern LONG* 	AS_pnCT6Ratio; 		 //CT6变比
extern LONG* 	AS_pnCT7Ratio; 		 //CT7变比
extern LONG* 	AS_pnCT8Ratio; 		 //CT8变比
extern LONG* 	AS_pnRT1; 		 //RT1

#define AS_RT_NUM     17    //辅助定值中变比个数

#define AS_COUNT     23    //辅助定值个数
#define AS_BUF_SIZE     92    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 670)

//系数----------------------------------
extern LONG* 	CF_pnUP1; 		 //UP1
extern LONG* 	CF_pnUP2; 		 //UP2
extern LONG* 	CF_pnUP3; 		 //UP3
extern LONG* 	CF_pnUP4; 		 //UP4
extern LONG* 	CF_pnUP5; 		 //UP5
extern LONG* 	CF_pnUP6; 		 //UP6
extern LONG* 	CF_pnUP7; 		 //UP7
extern LONG* 	CF_pnUP8; 		 //UP8
extern LONG* 	CF_pnIP1; 		 //IP1
extern LONG* 	CF_pnIP2; 		 //IP2
extern LONG* 	CF_pnIP3; 		 //IP3
extern LONG* 	CF_pnIP4; 		 //IP4
extern LONG* 	CF_pnIP5; 		 //IP5
extern LONG* 	CF_pnIP6; 		 //IP6
extern LONG* 	CF_pnIP7; 		 //IP7
extern LONG* 	CF_pnIP8; 		 //IP8
extern LONG* 	CF_pnIC1; 		 //IC1
extern LONG* 	CF_pnIC2; 		 //IC2
extern LONG* 	CF_pnUM1; 		 //UM1
extern LONG* 	CF_pnUM2; 		 //UM2
extern LONG* 	CF_pnUM3; 		 //UM3
extern LONG* 	CF_pnUM4; 		 //UM4
extern LONG* 	CF_pnUM5; 		 //UM5
extern LONG* 	CF_pnUM6; 		 //UM6
extern LONG* 	CF_pnUM7; 		 //UM7
extern LONG* 	CF_pnUM8; 		 //UM8
extern LONG* 	CF_pnIM1; 		 //IM1
extern LONG* 	CF_pnIM2; 		 //IM2
extern LONG* 	CF_pnIM3; 		 //IM3
extern LONG* 	CF_pnIM4; 		 //IM4
extern LONG* 	CF_pnIM5; 		 //IM5
extern LONG* 	CF_pnIM6; 		 //IM6
extern LONG* 	CF_pnIM7; 		 //IM7
extern LONG* 	CF_pnIM8; 		 //IM8
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnU1D; 		 //U1D
extern LONG* 	CF_pnU2D; 		 //U2D
extern LONG* 	CF_pnU3D; 		 //U3D
extern LONG* 	CF_pnU4D; 		 //U4D
extern LONG* 	CF_pnU5D; 		 //U5D
extern LONG* 	CF_pnU6D; 		 //U6D
extern LONG* 	CF_pnU7D; 		 //U7D
extern LONG* 	CF_pnU8D; 		 //U8D
extern LONG* 	CF_pnI1D; 		 //I1D
extern LONG* 	CF_pnI2D; 		 //I2D
extern LONG* 	CF_pnI3D; 		 //I3D
extern LONG* 	CF_pnI4D; 		 //I4D
extern LONG* 	CF_pnI5D; 		 //I5D
extern LONG* 	CF_pnI6D; 		 //I6D
extern LONG* 	CF_pnI7D; 		 //I7D
extern LONG* 	CF_pnI8D; 		 //I8D

#define CF_COUNT     54    //系数个数
#define CF_BUF_SIZE     216    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 762)

//报告访问宏-----------------------
#define R_OEERR                     0    //开出光隔失效
#define R_OEBR                      1    //开出光隔击穿
#define R_ADERR                     2    //AD故障
#define R_SLERR                     3    //软压板故障
#define R_EEPROMERR                 4    //EEPROM定值出错
#define R_RAMERR                    5    //RAM定值出错
#define R_ROMERR                    6    //程序ROM故障
#define R_RESETAN                   7    //装置异常复位
#define R_COMERR                    8    //通信通道异常
#define R_NVRAMERR                  9    //NVRAM内容出错
#define R_LOGICERR                 10    //逻辑方程故障
#define R_RCERR                    11    //保护自检错误
#define R_COMTEST                  12    //通道测试
#define R_ADERR_R                  13    //AD故障恢复
#define R_SLERR_R                  14    //软压板故障恢复
#define R_EEPROMERR_R              15    //EEPROM定值故障恢复
#define R_RAMERR_R                 16    //RAM定值故障恢复
#define R_ROMERR_R                 17    //程序ROM故障恢复
#define R_COMERR_R                 18    //通信通道异常恢复
#define R_NVRAMERR_R               19    //NVRAM内容出错恢复
#define R_LOGICERR_R               20    //逻辑方程故障故障恢复
#define R_ACTPICK                  21    //保护启动
#define R_PI_BZT                   22    //备自投启动
#define R_PI_1ZB                   23    //1#变故障启动自投
#define R_PI_2ZB                   24    //2#变故障启动自投
#define R_PI_1SY                   25    //1#进线失压启动自投
#define R_PI_2SY                   26    //2#进线失压启动自投
#define R_SY1ZB1                   27    //1#进线失压且1#变故障
#define R_SY1ZB2                   28    //1#进线失压且2#变故障
#define R_SY2ZB2                   29    //2#进线失压且2#变故障
#define R_SY2ZB1                   30    //2#进线失压且1#变故障
#define R_BZTOFF                   31    //备投退出
#define R_ZB1GZ                    32    //1#变故障
#define R_ZB2GZ                    33    //2#变故障
#define R_SY1                      34    //1#进线失压
#define R_SY2                      35    //2#进线失压
#define R_SU_BZT                   36    //备自投成功
#define R_FA_BZT                   37    //备自投失败 
#define R_PT1DX                    38    //PT1断线
#define R_PT2DX                    39    //PT2断线
#define R_PT3DX                    40    //PT3断线
#define R_PT4DX                    41    //PT4断线
#define R_INF_LOCK                 42    //装置被闭锁
#define R_INF_LKCL                 43    //闭锁被清除
#define R_60I_1AT                  44    //1AT差流动作
#define R_50T_1AT                  45    //1ATT线过流动作
#define R_50F_1AT                  46    //1ATF线过流动作
#define R_50CS_1AT                 47    //1AT碰壳过流保护动作
#define R_60I_2AT                  48    //2AT差流动作
#define R_50T_2AT                  49    //2ATT线过流保护动作
#define R_50F_2AT                  50    //2ATF线过流保护动作
#define R_50CS_2AT                 51    //2AT碰壳过流保护动作
#define R_60I_3AT                  52    //3AT差流动作
#define R_50T_3AT                  53    //3ATT线过流动作
#define R_50F_3AT                  54    //3ATF线过流动作
#define R_50CS_3AT                 55    //3AT碰壳过流保护动作
#define R_60I_4AT                  56    //4AT差流动作
#define R_50T_4AT                  57    //4ATT线过流保护动作
#define R_50F_4AT                  58    //4ATF线过流保护动作
#define R_50CS_4AT                 59    //4AT碰壳过流保护动作
#define R_27T_L1                   60    //1#进线低压保护动作
#define R_27T_L2                   61    //2#进线低压保护动作
#define R_TR_1DL                   62    //分1断路器
#define R_CL_1DL                   63    //合1断路器
#define R_FA_TR1DL                 64    //分1断路器失败
#define R_FA_CL1DL                 65    //合1断路器失败
#define R_TR_2DL                   66    //分2断路器
#define R_CL_2DL                   67    //合2断路器
#define R_FA_TR2DL                 68    //分2断路器失败
#define R_FA_CL2DL                 69    //合2断路器失败
#define R_TR_3DL                   70    //分3断路器
#define R_CL_3DL                   71    //合3断路器
#define R_FA_TR3DL                 72    //分3断路器失败
#define R_FA_CL3DL                 73    //合3断路器失败
#define R_TR_4DL                   74    //分4断路器
#define R_CL_4DL                   75    //合4断路器
#define R_FA_TR4DL                 76    //分4断路器失败
#define R_FA_CL4DL                 77    //合4断路器失败
#define R_TR_5DL                   78    //分5断路器
#define R_CL_5DL                   79    //合5断路器
#define R_FA_TR5DL                 80    //分5断路器失败
#define R_FA_CL5DL                 81    //合5断路器失败
#define R_TR_6DL                   82    //分6断路器
#define R_CL_6DL                   83    //合6断路器
#define R_FA_TR6DL                 84    //分6断路器失败
#define R_FA_CL6DL                 85    //合6断路器失败
#define R_TR_7DL                   86    //分7断路器
#define R_CL_7DL                   87    //合7断路器
#define R_FA_TR7DL                 88    //分7断路器失败
#define R_FA_CL7DL                 89    //合7断路器失败
#define R_TR_8DL                   90    //分8断路器
#define R_CL_8DL                   91    //合8断路器
#define R_FA_TR8DL                 92    //分8断路器失败
#define R_FA_CL8DL                 93    //合8断路器失败
#define R_TR_9DL                   94    //分9断路器
#define R_CL_9DL                   95    //合9断路器
#define R_FA_TR9DL                 96    //分9断路器失败
#define R_FA_CL9DL                 97    //合9断路器失败
#define R_TR_10DL                  98    //分10断路器
#define R_CL_10DL                  99    //合10断路器
#define R_FA_TR10DL               100    //分10断路器失败
#define R_FA_CL10DL               101    //合10断路器失败
#define R_TR_1GK                  102    //分1隔开
#define R_CL_1GK                  103    //合1隔开
#define R_FA_TR1GK                104    //分1隔开失败
#define R_FA_CL1GK                105    //合1隔开失败
#define R_TR_2GK                  106    //分2隔开
#define R_CL_2GK                  107    //合2隔开
#define R_FA_TR2GK                108    //分2隔开失败
#define R_FA_CL2GK                109    //合2隔开失败
#define R_TR_3GK                  110    //分3隔开
#define R_CL_3GK                  111    //合3隔开
#define R_FA_TR3GK                112    //分3隔开失败
#define R_FA_CL3GK                113    //合3隔开失败
#define R_TR_4GK                  114    //分4隔开
#define R_CL_4GK                  115    //合4隔开
#define R_FA_TR4GK                116    //分4隔开失败
#define R_FA_CL4GK                117    //合4隔开失败
#define R_TR_5GK                  118    //分5隔开
#define R_CL_5GK                  119    //合5隔开
#define R_FA_TR5GK                120    //分5隔开失败
#define R_FA_CL5GK                121    //合5隔开失败
#define R_TR_6GK                  122    //分6隔开
#define R_CL_6GK                  123    //合6隔开
#define R_FA_TR6GK                124    //分6隔开失败
#define R_FA_CL6GK                125    //合6隔开失败
#define R_TR_7GK                  126    //分7隔开
#define R_CL_7GK                  127    //合7隔开
#define R_FA_TR7GK                128    //分7隔开失败
#define R_FA_CL7GK                129    //合7隔开失败
#define R_TR_8GK                  130    //分8隔开
#define R_CL_8GK                  131    //合8隔开
#define R_FA_TR8GK                132    //分8隔开失败
#define R_FA_CL8GK                133    //合8隔开失败
#define R_TR_9GK                  134    //分9隔开
#define R_CL_9GK                  135    //合9隔开
#define R_FA_TR9GK                136    //分9隔开失败
#define R_FA_CL9GK                137    //合9隔开失败
#define R_TR_10GK                 138    //分10隔开
#define R_CL_10GK                 139    //合10隔开
#define R_FA_TR10GK               140    //分10隔开失败
#define R_FA_CL10GK               141    //合10隔开失败
#define R_TR_101                  142    //分101断路器
#define R_CL_101                  143    //合101断路器
#define R_FA_TR101                144    //分101断路器失败
#define R_FA_CL101                145    //合101断路器失败
#define R_TR_102                  146    //分102断路器
#define R_CL_102                  147    //合102断路器
#define R_FA_TR102                148    //分102断路器失败
#define R_FA_CL102                149    //合102断路器失败
#define R_TR_103                  150    //分103断路器
#define R_CL_103                  151    //合103断路器
#define R_FA_TR103                152    //分103断路器失败
#define R_FA_CL103                153    //合103断路器失败
#define R_TR_104                  154    //分104断路器
#define R_CL_104                  155    //合104断路器
#define R_FA_TR104                156    //分104断路器失败
#define R_FA_CL104                157    //合104断路器失败
#define R_TR_201                  158    //分201断路器
#define R_CL_201                  159    //合201断路器
#define R_FA_TR201                160    //分201断路器失败
#define R_FA_CL201                161    //合201断路器失败
#define R_TR_202                  162    //分202断路器
#define R_CL_202                  163    //合202断路器
#define R_FA_TR202                164    //分202断路器失败
#define R_FA_CL202                165    //合202断路器失败
#define R_TR_203                  166    //分203断路器
#define R_CL_203                  167    //合203断路器
#define R_FA_TR203                168    //分203断路器失败
#define R_FA_CL203                169    //合203断路器失败
#define R_TR_204                  170    //分204断路器
#define R_CL_204                  171    //合204断路器
#define R_FA_TR204                172    //分204断路器失败
#define R_FA_CL204                173    //合204断路器失败
#define R_TR_201A                 174    //分201A断路器
#define R_CL_201A                 175    //合201A断路器
#define R_FA_TR201A               176    //分201A断路器失败
#define R_FA_CL201A               177    //合201A断路器失败
#define R_TR_201B                 178    //分201B断路器
#define R_CL_201B                 179    //合201B断路器
#define R_FA_TR201B               180    //分201B断路器失败
#define R_FA_CL201B               181    //合201B断路器失败
#define R_TR_202A                 182    //分202A断路器
#define R_CL_202A                 183    //合202A断路器
#define R_FA_TR202A               184    //分202A断路器失败
#define R_FA_CL202A               185    //合202A断路器失败
#define R_TR_202B                 186    //分202B断路器   
#define R_CL_202B                 187    //合202B断路器
#define R_FA_TR202B               188    //分202B断路器失败
#define R_FA_CL202B               189    //合202B断路器失败
#define R_TR_1011                 190    //分1011隔开
#define R_CL_1011                 191    //合1011隔开
#define R_FA_TR1011               192    //分2011隔开失败
#define R_FA_CL1011               193    //合1011隔开失败
#define R_TR_1021                 194    //分1021隔开
#define R_CL_1021                 195    //合1021隔开
#define R_FA_TR1021               196    //分1021隔开失败
#define R_FA_CL1021               197    //合1021隔开失败
#define R_TR_1001                 198    //分1001隔开
#define R_CL_1001                 199    //合1001隔开
#define R_FA_TR1001               200    //分1001隔开失败
#define R_FA_CL1001               201    //合1001隔开失败
#define R_TR_1002                 202    //分1002隔开
#define R_CL_1002                 203    //合1002隔开
#define R_FA_TR1002               204    //分1002隔开失败
#define R_FA_CL1002               205    //合1002隔开失败
#define R_TR_1019                 206    //分1019隔开
#define R_CL_1019                 207    //合1019隔开
#define R_FA_TR1019               208    //分1019隔开失败
#define R_FA_CL1019               209    //合1019隔开失败
#define R_TR_1029                 210    //分1029隔开
#define R_CL_1029                 211    //合1029隔开
#define R_FA_TR1029               212    //分1029隔开失败
#define R_FA_CL1029               213    //合1029隔开失败
#define R_TR_1039                 214    //分1039隔开
#define R_CL_1039                 215    //合1039隔开
#define R_FA_TR1039               216    //分1039隔开失败
#define R_FA_CL1039               217    //合1039隔开失败
#define R_TR_1049                 218    //分1049隔开
#define R_CL_1049                 219    //合1049隔开
#define R_FA_TR1049               220    //分1049隔开失败
#define R_FA_CL1049               221    //合1049隔开失败
#define R_AT1_ZWS                 222    //1AT重瓦斯
#define R_AT1_YLSF                223    //1AT压力释放
#define R_AT1_WDGG                224    //1AT温度过高
#define R_AT1_QWS                 225    //1AT轻瓦斯
#define R_AT1_WDGJ                226    //1AT温度告警
#define R_AT1_YWYC                227    //1AT油位异常
#define R_AT1_YSYC                228    //1AT油速异常
#define R_AT2_ZWS                 229    //2AT重瓦斯
#define R_AT2_YLSF                230    //2AT压力释放
#define R_AT2_WDGG                231    //2AT温度过高
#define R_AT2_QWS                 232    //2AT轻瓦斯
#define R_AT2_WDGJ                233    //2AT温度告警
#define R_AT2_YWYC                234    //2AT油位异常
#define R_AT2_YSYC                235    //2AT油速异常
#define R_AT3_ZWS                 236    //3AT重瓦斯
#define R_AT3_YLSF                237    //3AT压力释放
#define R_AT3_WDGG                238    //3AT温度过高
#define R_AT3_QWS                 239    //3AT轻瓦斯
#define R_AT3_WDGJ                240    //3AT温度告警
#define R_AT3_YWYC                241    //3AT油位异常
#define R_AT3_YSYC                242    //3AT油速异常
#define R_AT4_ZWS                 243    //4AT重瓦斯
#define R_AT4_YLSF                244    //4AT压力释放
#define R_AT4_WDGG                245    //4AT温度过高
#define R_AT4_QWS                 246    //4AT轻瓦斯
#define R_AT4_WDGJ                247    //4AT温度告警
#define R_AT4_YWYC                248    //4AT油位异常
#define R_AT4_YSYC                249    //4AT油速异常
#define R_TR_1ATKF                250    //分1AT快分开关
#define R_TR_2ATKF                251    //分2AT快分开关
#define R_TR_3ATKF                252    //分3AT快分开关
#define R_TR_4ATKF                253    //分4AT快分开关
#define R_FA_TR1ATKF              254    //分1AT快分失败
#define R_FA_TR2ATKF              255    //分2AT快分失败
#define R_FA_TR3ATKF              256    //分3AT快分失败
#define R_FA_TR4ATKF              257    //分4AT快分失败
#define R_SU_ATOP                 258    //动作成功
#define R_FA_ATOP                 259    //动作失败
#define R_SU_79_59                260    //检有压合闸成功
#define R_FA_79_59                261    //检有合闸失败
#define R_TR_241                  262    //分241断路器
#define R_CL_241                  263    //合241断路器
#define R_FA_TR241                264    //分241断路器失败
#define R_FA_CL241                265    //合241断路器失败
#define R_TR_242                  266    //分242断路器
#define R_CL_242                  267    //合242断路器
#define R_FA_TR242                268    //分242断路器失败
#define R_FA_CL242                269    //合242断路器失败
#define R_TR_271                  270    //分271断路器
#define R_CL_271                  271    //合271断路器
#define R_FA_TR271                272    //分271断路器失败
#define R_FA_CL271                273    //合271断路器失败
#define R_TR_272                  274    //分272断路器
#define R_CL_272                  275    //合272断路器
#define R_FA_TR272                276    //分272断路器失败
#define R_FA_CL272                277    //合272断路器失败
#define R_TR_281                  278    //分281断路器
#define R_CL_281                  279    //合281断路器
#define R_FA_TR281                280    //分281断路器失败
#define R_FA_CL281                281    //合281断路器失败
#define R_TR_2411                 282    //分2411隔开
#define R_CL_2411                 283    //合2411隔开
#define R_FA_TR2411               284    //分2411隔开失败
#define R_FA_CL2411               285    //合2411隔开失败
#define R_TR_2421                 286    //分2421隔开
#define R_CL_2421                 287    //合2421隔开
#define R_FA_TR2421               288    //分2421隔开失败
#define R_FA_CL2421               289    //合2421隔开失败
#define R_TR_2711                 290    //分2711隔开
#define R_CL_2711                 291    //合2711隔开
#define R_FA_TR2711               292    //分2711隔开失败
#define R_FA_CL2711               293    //合2711隔开失败
#define R_TR_2721                 294    //分2721隔开
#define R_CL_2721                 295    //合2721隔开
#define R_FA_TR2721               296    //分2721隔开失败
#define R_FA_CL2721               297    //合2721隔开失败
#define R_60I_1AT_PI              298    //1AT差流启动
#define R_60I_1AT_RES             299    //1AT差流返回
#define R_50T_1AT_PI              300    //1AT T线过流启动
#define R_50T_1AT_RES             301    //1AT T线过流返回
#define R_50F_1AT_PI              302    //1AT F线过流启动
#define R_50F_1AT_RES             303    //1AT F线过流返回
#define R_50CS_1AT_PI             304    //1AT碰壳过流启动
#define R_50CS_1AT_RES            305    //1AT碰壳过流返回
#define R_60I_2AT_PI              306    //2AT差流启动
#define R_60I_2AT_RES             307    //2AT差流返回
#define R_50T_2AT_PI              308    //2AT T线过流启动
#define R_50T_2AT_RES             309    //2AT T线过流返回
#define R_50F_2AT_PI              310    //2AT F线过流启动
#define R_50F_2AT_RES             311    //2AT F线过流返回
#define R_50CS_2AT_PI             312    //2AT碰壳过流启动
#define R_50CS_2AT_RES            313    //2AT碰壳过流返回
#define R_PICKUP                  314    //保护启动
#define R_52BF_AT1                315    //AT1断路器跳闸失败
#define R_52BF_AT2                316    //AT2断路器跳闸失败
#define R_PI_1AT                  317    //1AT故障启动自投
#define R_PI_2AT                  318    //2AT故障启动自投
#define R_POWERERR                319    //5V电源故障
#define R_EEPROM_W_ERR            320    //EEPROM写入故障
#define R_POWERERR_R              321    //5V电源故障恢复
#define R_RELDATAERR              322    //保护数据异常
#define R_RELDATAERR_R            323    //保护数据异常恢复
#define MAX_REPORT_INDEX          323    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     28    //传动个数

#endif