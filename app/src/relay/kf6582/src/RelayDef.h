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
#define  RM_BOBRCH1 16
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 17
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 18
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 19
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     20    //保护测量值个数
#define RM_BUF_SIZE     80    //保护测量值缓冲区大小
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
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 84)

//软压板----------------------------------
extern BYTE* 	SW_pbyBZT; 		 //备自投功能
extern BYTE* 	SW_pbyZBBZT; 		 //主变备自投
extern BYTE* 	SW_pbySYBZT; 		 //失压备自投
extern BYTE* 	SW_pbyEARTHQK; 		 //地震监测
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2
extern BYTE* 	SW_pbyBAK3; 		 //备用压板3
extern BYTE* 	SW_pbyBAK4; 		 //备用压板4

#define SW_COUNT     9    //软压板个数
#define SW_BUF_SIZE     9    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 148)

//配置----------------------------------
extern BYTE* 	CFG_pbyBZT; 		 //备自投功能
extern BYTE* 	CFG_pbyREBZT; 		 //备自投重新投入
extern BYTE* 	CFG_pbyZBBZT; 		 //主变备自投
extern BYTE* 	CFG_pbySYBZT; 		 //失压备自投
extern BYTE* 	CFG_pbyZLYX; 		 //直列优先
extern BYTE* 	CFG_pby74PT1; 		 //PT1断线检查
extern BYTE* 	CFG_pby74PT2; 		 //PT2断线检查
extern BYTE* 	CFG_pby74PT3; 		 //PT3断线检查
extern BYTE* 	CFG_pby74PT4; 		 //PT4断线检查
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2
extern BYTE* 	CFG_pbyBAK3; 		 //备用配置3
extern BYTE* 	CFG_pbyBAK4; 		 //备用配置4

#define CFG_COUNT     13    //配置个数/
#define CFG_BUF_SIZE     13    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 158)

//定值----------------------------------
extern LONG* 	RS_pnF; 		 //配置字 F
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

#define RS_COUNT     101    //定值个数/
#define RS_BUF_SIZE     404    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 172)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
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

#define AS_COUNT     21    //辅助定值个数
#define AS_BUF_SIZE     84    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 576)

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

#define CF_COUNT     52    //系数个数
#define CF_BUF_SIZE     208    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 660)

//报告访问宏-----------------------
#define R_RAMERR                    0    //RAM定值出错
#define R_RESETAN                   1    //装置异常复位
#define R_COMERR                    2    //通信通道异常
#define R_NVRAMERR                  3    //NVRAM内容出错
#define R_ROMERR                    4    //程序ROM故障
#define R_EEPROMERR                 5    //EEPROM定值出错
#define R_ADERR                     6    //AD故障
#define R_OEBR                      7    //开出光隔击穿
#define R_OEERR                     8    //开出光隔失效
#define R_SLERR                     9    //软压板故障
#define R_LOGICERR                 10    //逻辑方程故障
#define R_ACTPICK                  11    //保护启动
#define R_COMTEST                  12    //通道测试
#define R_RCERR                    13    //保护自检错误
#define R_PI_BZT                   14    //备自投启动
#define R_PI_1ZB                   15    //1#变故障启动自投
#define R_PI_2ZB                   16    //2#变故障启动自投
#define R_PI_1SY                   17    //1#进线失压启动自投
#define R_PI_2SY                   18    //2#进线失压启动自投
#define R_SY1ZB1                   19    //1#进线失压且1#变故障
#define R_SY1ZB2                   20    //1#进线失压且2#变故障
#define R_SY2ZB2                   21    //2#进线失压且2#变故障
#define R_SY2ZB1                   22    //2#进线失压且1#变故障
#define R_BZTOFF                   23    //备投退出
#define R_ZB1GZ                    24    //1#变故障
#define R_ZB2GZ                    25    //2#变故障
#define R_SY1                      26    //1#进线失压
#define R_SY2                      27    //2#进线失压
#define R_SU_BZT                   28    //备自投成功
#define R_FA_BZT                   29    //备自投失败 
#define R_PT1DX                    30    //PT1断线
#define R_PT2DX                    31    //PT2断线
#define R_PT3DX                    32    //PT3断线
#define R_PT4DX                    33    //PT4断线
#define R_INF_LOCK                 34    //装置被闭锁
#define R_INF_LKCL                 35    //闭锁被清除
#define R_27T_L1                   36    //1#进线低压保护动作
#define R_27T_L2                   37    //2#进线低压保护动作
#define R_TR_1DL                   38    //跳1断路器
#define R_CL_1DL                   39    //合1断路器
#define R_FA_TR1DL                 40    //跳1断路器失败
#define R_FA_CL1DL                 41    //合1断路器失败
#define R_TR_2DL                   42    //跳2断路器
#define R_CL_2DL                   43    //合2断路器
#define R_FA_TR2DL                 44    //跳2断路器失败
#define R_FA_CL2DL                 45    //合2断路器失败
#define R_TR_3DL                   46    //跳3断路器
#define R_CL_3DL                   47    //合3断路器
#define R_FA_TR3DL                 48    //跳3断路器失败
#define R_FA_CL3DL                 49    //合3断路器失败
#define R_TR_4DL                   50    //分4断路器
#define R_CL_4DL                   51    //合4断路器
#define R_FA_TR4DL                 52    //分4断路器失败
#define R_FA_CL4DL                 53    //合4断路器失败
#define R_TR_5DL                   54    //分5断路器
#define R_CL_5DL                   55    //合5断路器
#define R_FA_TR5DL                 56    //分5断路器失败
#define R_FA_CL5DL                 57    //合5断路器失败
#define R_TR_6DL                   58    //分6断路器
#define R_CL_6DL                   59    //合6断路器
#define R_FA_TR6DL                 60    //分6断路器失败
#define R_FA_CL6DL                 61    //合6断路器失败
#define R_TR_7DL                   62    //分7断路器
#define R_CL_7DL                   63    //合7断路器
#define R_FA_TR7DL                 64    //分7断路器失败
#define R_FA_CL7DL                 65    //合7断路器失败
#define R_TR_8DL                   66    //分8断路器
#define R_CL_8DL                   67    //合8断路器
#define R_FA_TR8DL                 68    //分8断路器失败
#define R_FA_CL8DL                 69    //合8断路器失败
#define R_TR_9DL                   70    //分9断路器
#define R_CL_9DL                   71    //合9断路器
#define R_FA_TR9DL                 72    //分9断路器失败
#define R_FA_CL9DL                 73    //合9断路器失败
#define R_TR_10DL                  74    //分10断路器
#define R_CL_10DL                  75    //合10断路器
#define R_FA_TR10DL                76    //分10断路器失败
#define R_FA_CL10DL                77    //合10断路器失败
#define R_TR_1GK                   78    //跳1隔开
#define R_CL_1GK                   79    //合1隔开
#define R_FA_TR1GK                 80    //跳1隔开失败
#define R_FA_CL1GK                 81    //合1隔开失败
#define R_TR_2GK                   82    //跳2隔开
#define R_CL_2GK                   83    //合2隔开
#define R_FA_TR2GK                 84    //跳2隔开失败
#define R_FA_CL2GK                 85    //合2隔开失败
#define R_TR_3GK                   86    //跳3隔开
#define R_CL_3GK                   87    //合3隔开
#define R_FA_TR3GK                 88    //分3隔开失败
#define R_FA_CL3GK                 89    //合3隔开失败
#define R_TR_4GK                   90    //分4隔开
#define R_CL_4GK                   91    //合4隔开
#define R_FA_TR4GK                 92    //分4隔开失败
#define R_FA_CL4GK                 93    //合4隔开失败
#define R_TR_5GK                   94    //分5隔开
#define R_CL_5GK                   95    //合5隔开
#define R_FA_TR5GK                 96    //分5隔开失败
#define R_FA_CL5GK                 97    //合5隔开失败
#define R_TR_6GK                   98    //分6隔开
#define R_CL_6GK                   99    //合6隔开
#define R_FA_TR6GK                100    //分6隔开失败
#define R_FA_CL6GK                101    //合6隔开失败
#define R_TR_7GK                  102    //分7隔开
#define R_CL_7GK                  103    //合7隔开
#define R_FA_TR7GK                104    //分7隔开失败
#define R_FA_CL7GK                105    //合7隔开失败
#define R_TR_8GK                  106    //分8隔开
#define R_CL_8GK                  107    //合8隔开
#define R_FA_TR8GK                108    //分8隔开失败
#define R_FA_CL8GK                109    //合8隔开失败
#define R_TR_9GK                  110    //分9隔开
#define R_CL_9GK                  111    //合9隔开
#define R_FA_TR9GK                112    //分9隔开失败
#define R_FA_CL9GK                113    //合9隔开失败
#define R_TR_10GK                 114    //分10隔开
#define R_CL_10GK                 115    //合10隔开
#define R_FA_TR10GK               116    //分10隔开失败
#define R_FA_CL10GK               117    //合10隔开失败
#define R_TR_101                  118    //分101断路器
#define R_CL_101                  119    //合101断路器
#define R_FA_TR101                120    //分101断路器失败
#define R_FA_CL101                121    //合101断路器失败
#define R_TR_102                  122    //分102断路器
#define R_CL_102                  123    //合102断路器
#define R_FA_TR102                124    //分102断路器失败
#define R_FA_CL102                125    //合102断路器失败
#define R_TR_103                  126    //分103断路器
#define R_CL_103                  127    //合103断路器
#define R_FA_TR103                128    //跳103断路器失败
#define R_FA_CL103                129    //合103断路器失败
#define R_TR_104                  130    //跳104断路器
#define R_CL_104                  131    //合104断路器
#define R_FA_TR104                132    //跳104断路器失败
#define R_FA_CL104                133    //合104断路器失败
#define R_TR_201                  134    //分201断路器
#define R_CL_201                  135    //合201断路器
#define R_FA_TR201                136    //分201断路器失败
#define R_FA_CL201                137    //合201断路器失败
#define R_TR_202                  138    //分202断路器
#define R_CL_202                  139    //合202断路器
#define R_FA_TR202                140    //分202断路器失败
#define R_FA_CL202                141    //合202断路器失败
#define R_TR_203                  142    //分203断路器
#define R_CL_203                  143    //合203断路器
#define R_FA_TR203                144    //分203断路器失败
#define R_FA_CL203                145    //合203断路器失败
#define R_TR_204                  146    //分204断路器
#define R_CL_204                  147    //合204断路器
#define R_FA_TR204                148    //分204断路器失败
#define R_FA_CL204                149    //合204断路器失败
#define R_TR_201A                 150    //分201A断路器
#define R_CL_201A                 151    //合201A断路器
#define R_FA_TR201A               152    //分201A断路器失败
#define R_FA_CL201A               153    //合201A断路器失败
#define R_TR_201B                 154    //分201B断路器
#define R_CL_201B                 155    //合201B断路器
#define R_FA_TR201B               156    //跳201B断路器失败
#define R_FA_CL201B               157    //合201B断路器失败
#define R_TR_202A                 158    //跳202A断路器
#define R_CL_202A                 159    //合202A断路器
#define R_FA_TR202A               160    //跳202A断路器失败
#define R_FA_CL202A               161    //合202A断路器失败
#define R_TR_202B                 162    //跳202B断路器   
#define R_CL_202B                 163    //合202B断路器
#define R_FA_TR202B               164    //跳202B断路器失败
#define R_FA_CL202B               165    //合202B断路器失败
#define R_TR_1011                 166    //分1011隔开
#define R_CL_1011                 167    //合1011隔开
#define R_FA_TR1011               168    //分2011隔开失败
#define R_FA_CL1011               169    //合1011隔开失败
#define R_TR_1021                 170    //分1021隔开
#define R_CL_1021                 171    //合1021隔开
#define R_FA_TR1021               172    //分1021隔开失败
#define R_FA_CL1021               173    //合1021隔开失败
#define R_TR_1001                 174    //分1001隔开
#define R_CL_1001                 175    //合1001隔开
#define R_FA_TR1001               176    //分1001隔开失败
#define R_FA_CL1001               177    //合1001隔开失败
#define R_TR_1002                 178    //分1002隔开
#define R_CL_1002                 179    //合1002隔开
#define R_FA_TR1002               180    //分1002隔开失败
#define R_FA_CL1002               181    //合1002隔开失败
#define R_TR_1019                 182    //分1019隔开
#define R_CL_1019                 183    //合1019隔开
#define R_FA_TR1019               184    //分1019隔开失败
#define R_FA_CL1019               185    //合1019隔开失败
#define R_TR_1029                 186    //分1029隔开
#define R_CL_1029                 187    //合1029隔开
#define R_FA_TR1029               188    //分1029隔开失败
#define R_FA_CL1029               189    //合1029隔开失败
#define R_TR_1039                 190    //跳1039隔开
#define R_CL_1039                 191    //合1039隔开
#define R_FA_TR1039               192    //分1039隔开失败
#define R_FA_CL1039               193    //合1039隔开失败
#define R_TR_1049                 194    //分1049隔开
#define R_CL_1049                 195    //合1049隔开
#define R_FA_TR1049               196    //分1049隔开失败
#define R_FA_CL1049               197    //合1049隔开失败
#define R_AT1_ZWS                 198    //1AT重瓦斯
#define R_AT1_YLSF                199    //1AT压力释放
#define R_AT1_WDGG                200    //1AT温度过高
#define R_AT1_QWS                 201    //1AT轻瓦斯
#define R_AT1_WDGJ                202    //1AT温度告警
#define R_AT1_YWYC                203    //1AT油位异常
#define R_AT1_YSYC                204    //1AT油速异常
#define R_AT2_ZWS                 205    //2AT重瓦斯
#define R_AT2_YLSF                206    //2AT压力释放
#define R_AT2_WDGG                207    //2AT温度过高
#define R_AT2_QWS                 208    //2AT轻瓦斯
#define R_AT2_WDGJ                209    //2AT温度告警
#define R_AT2_YWYC                210    //2AT油位异常
#define R_AT2_YSYC                211    //2AT油速异常
#define R_AT3_ZWS                 212    //3AT重瓦斯
#define R_AT3_YLSF                213    //3AT压力释放
#define R_AT3_WDGG                214    //3AT温度过高
#define R_AT3_QWS                 215    //3AT轻瓦斯
#define R_AT3_WDGJ                216    //3AT温度告警
#define R_AT3_YWYC                217    //3AT油位异常
#define R_AT3_YSYC                218    //3AT油速异常
#define R_AT4_ZWS                 219    //4AT重瓦斯
#define R_AT4_YLSF                220    //4AT压力释放
#define R_AT4_WDGG                221    //4AT温度过高
#define R_AT4_QWS                 222    //4AT轻瓦斯
#define R_AT4_WDGJ                223    //4AT温度告警
#define R_AT4_YWYC                224    //4AT油位异常
#define R_AT4_YSYC                225    //4AT油速异常
#define R_TR_1ATKF                226    //分1AT快分开关
#define R_TR_2ATKF                227    //分2AT快分开关
#define R_TR_3ATKF                228    //分3AT快分开关
#define R_TR_4ATKF                229    //分4AT快分开关
#define R_FA_TR1ATKF              230    //分1AT快分失败
#define R_FA_TR2ATKF              231    //分2AT快分失败
#define R_FA_TR3ATKF              232    //分3AT快分失败
#define R_FA_TR4ATKF              233    //分4AT快分失败
#define R_SU_ATOP                 234    //动作成功
#define R_FA_ATOP                 235    //动作失败
#define R_SU_79_59                236    //检有压合闸成功
#define R_FA_79_59                237    //检有合闸失败
#define R_TR_241                  238    //分241断路器
#define R_CL_241                  239    //合241断路器
#define R_FA_TR241                240    //分241断路器失败
#define R_FA_CL241                241    //合241断路器失败
#define R_TR_242                  242    //分242断路器
#define R_CL_242                  243    //合242断路器
#define R_FA_TR242                244    //分242断路器失败
#define R_FA_CL242                245    //合242断路器失败
#define R_TR_271                  246    //分271断路器
#define R_CL_271                  247    //合271断路器
#define R_FA_TR271                248    //分271断路器失败
#define R_FA_CL271                249    //合271断路器失败
#define R_TR_272                  250    //分272断路器
#define R_CL_272                  251    //合272断路器
#define R_FA_TR272                252    //分272断路器失败
#define R_FA_CL272                253    //合272断路器失败
#define R_TR_281                  254    //分281断路器
#define R_CL_281                  255    //合281断路器
#define R_FA_TR281                256    //分281断路器失败
#define R_FA_CL281                257    //合281断路器失败
#define R_TR_2411                 258    //分2411隔开
#define R_CL_2411                 259    //合2411隔开
#define R_FA_TR2411               260    //分2411隔开失败
#define R_FA_CL2411               261    //合2411隔开失败
#define R_TR_2421                 262    //分2421隔开
#define R_CL_2421                 263    //合2421隔开
#define R_FA_TR2421               264    //分2421隔开失败
#define R_FA_CL2421               265    //合2421隔开失败
#define R_TR_2711                 266    //分2711隔开
#define R_CL_2711                 267    //合2711隔开
#define R_FA_TR2711               268    //分2711隔开失败
#define R_FA_CL2711               269    //合2711隔开失败
#define R_TR_2721                 270    //分2721隔开
#define R_CL_2721                 271    //合2721隔开
#define R_FA_TR2721               272    //分2721隔开失败
#define R_FA_CL2721               273    //合2721隔开失败
#define R_NVRAMERR_R              274    //NVRAM内容出错恢复
#define R_ROMERR_R                275    //程序ROM故障恢复
#define R_RAMERR_R                276    //RAM定值故障恢复
#define R_EEPROMERR_R             277    //EEPROM定值故障恢复
#define R_SLERR_R                 278    //软压板故障恢复
#define R_ADERR_R                 279    //AD故障恢复
#define R_LOGICERR_R              280    //逻辑方程故障故障恢复
#define R_COMERR_R                281    //通信通道异常恢复
#define R_EARTHQK                 282    //地震报警
#define R_POWERERR                283    //5V电源故障
#define R_EEPROM_W_ERR            284    //EEPROM写入故障
#define R_POWERERR_R              285    //5V电源故障恢复
#define R_FA_TRDR1                286    //1号电容跳闸失败
#define R_FA_TRDR2                287    //2号电容跳闸失败
#define R_FA_TRDR3                288    //3号电容跳闸失败
#define R_FA_TRDR4                289    //4号电容跳闸失败
#define R_PT1_RES                 290    //PT1断线恢复
#define R_PT2_RES                 291    //PT2断线恢复
#define R_PT3_RES                 292    //PT3断线恢复
#define R_PT4_RES                 293    //PT4断线恢复
#define R_1SY_RES                 294    //一号进线失压恢复
#define R_2SY_RES                 295    //二号进线失压恢复
#define R_RELDATAERR              296    //保护数据异常
#define R_RELDATAERR_R            297    //保护数据异常恢复
#define MAX_REPORT_INDEX          297    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     32    //传动个数

#endif