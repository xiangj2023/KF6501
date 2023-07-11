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
#define 	 FAILDATA_DISPTYPE_CFGNO 	 12 		 //故障电量显示类型配置序号
#define 	 LangTYPE_SYSSETNO 	 0 		 //语言类型辅助定值序号

//当前定值区号定义-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //当前定值区号

//定值取值宏定义
//----变压器类型-----
#define 	YD11_T 	0	//Y/D-11牵引
#define 	YD11_E 	1	//Y/D-11电力
#define 	YV_T 	2	//Y/V平衡变
#define 	YA_T 	3	//Y/A平衡变
#define 	SCOTT 	4	//SCOTT变
#define 	VV3_T 	5	//三相V/V变
#define 	VV1_T 	6	//单相V/V变
#define 	SIGNP_T 	7	//单相变
#define 	VX_T 	8	//V/X变压器
//----高侧CT接线-----
#define 	CT_Y 	0	//星接
#define 	CT_D 	1	//角接
//----VV变回流相-----
#define 	APHASE 	0	//A相回流
#define 	BPHASE 	1	//B相回流
#define 	CPHASE 	2	//C相回流

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//保护测量值定义-----------------------
#define  RM_IHA 0
extern LONG* 	RM_pnIHA; 		 // IA
#define  RM_IHB 1
extern LONG* 	RM_pnIHB; 		 // IB
#define  RM_IHC 2
extern LONG* 	RM_pnIHC; 		 // IC
#define  RM_IL1 3
extern LONG* 	RM_pnIL1; 		 // I1
#define  RM_IL2 4
extern LONG* 	RM_pnIL2; 		 // I2
#define  RM_IL3 5
extern LONG* 	RM_pnIL3; 		 // I3
#define  RM_IL4 6
extern LONG* 	RM_pnIL4; 		 // I4
#define  RM_IDA 7
extern LONG* 	RM_pnIDA; 		 // IDA
#define  RM_IDB 8
extern LONG* 	RM_pnIDB; 		 // IDB
#define  RM_IDC 9
extern LONG* 	RM_pnIDC; 		 // IDC
#define  RM_IRA 10
extern LONG* 	RM_pnIRA; 		 // IRA
#define  RM_IRB 11
extern LONG* 	RM_pnIRB; 		 // IRB
#define  RM_IRC 12
extern LONG* 	RM_pnIRC; 		 // IRC
#define  RM_IDA2 13
extern LONG* 	RM_pnIDA2; 		 // IDA2
#define  RM_IDA3 14
extern LONG* 	RM_pnIDA3; 		 // IDA3
#define  RM_IDA5 15
extern LONG* 	RM_pnIDA5; 		 // IDA5
#define  RM_IDB2 16
extern LONG* 	RM_pnIDB2; 		 // IDB2
#define  RM_IDB3 17
extern LONG* 	RM_pnIDB3; 		 // IDB3
#define  RM_IDB5 18
extern LONG* 	RM_pnIDB5; 		 // IDB5
#define  RM_IDC2 19
extern LONG* 	RM_pnIDC2; 		 // IDC2
#define  RM_IDC3 20
extern LONG* 	RM_pnIDC3; 		 // IDC3
#define  RM_IDC5 21
extern LONG* 	RM_pnIDC5; 		 // IDC5
#define  RM_IRA2 22
extern LONG* 	RM_pnIRA2; 		 // IRA2
#define  RM_IRB2 23
extern LONG* 	RM_pnIRB2; 		 // IRB2
#define  RM_IRC2 24
extern LONG* 	RM_pnIRC2; 		 // IRC2
#define  RM_AIHA 25
extern LONG* 	RM_pnAIHA; 		 // ΦIA
#define  RM_AIHB 26
extern LONG* 	RM_pnAIHB; 		 // ΦIB
#define  RM_AIHC 27
extern LONG* 	RM_pnAIHC; 		 // ΦIC
#define  RM_AIL1 28
extern LONG* 	RM_pnAIL1; 		 // ΦI1
#define  RM_AIL2 29
extern LONG* 	RM_pnAIL2; 		 // ΦI2
#define  RM_AIL3 30
extern LONG* 	RM_pnAIL3; 		 // ΦI3
#define  RM_AIL4 31
extern LONG* 	RM_pnAIL4; 		 // ΦI4
#define  RM_UA 32
extern LONG* 	RM_pnUA; 		 // UA
#define  RM_UB 33
extern LONG* 	RM_pnUB; 		 // UB
#define  RM_UC 34
extern LONG* 	RM_pnUC; 		 // UC
#define  RM_BOBRCH1 35
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 36
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 37
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 38
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     39    //保护测量值个数
#define RM_BUF_SIZE     156    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UA 0
extern LONG* 	M_pnUA; 		 //UA UA
#define  M_UB 1
extern LONG* 	M_pnUB; 		 //UB UB
#define  M_UC 2
extern LONG* 	M_pnUC; 		 //UC UC
#define  M_IHAM 3
extern LONG* 	M_pnIHAM; 		 //IHAM IHAM
#define  M_IHBM 4
extern LONG* 	M_pnIHBM; 		 //IHBM IHBM
#define  M_IHCM 5
extern LONG* 	M_pnIHCM; 		 //IHCM IHCM
#define  M_IL1M 6
extern LONG* 	M_pnIL1M; 		 //IL1M IL1M
#define  M_IL2M 7
extern LONG* 	M_pnIL2M; 		 //IL2M IL2M
#define  M_IL3M 8
extern LONG* 	M_pnIL3M; 		 //IL3M IL3M
#define  M_IL4M 9
extern LONG* 	M_pnIL4M; 		 //IL4M IL4M

#define MEASURE_COUNT     10    //测量值个数
#define MEASURE_BUF_SIZE     40    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 160)

//软压板----------------------------------
extern BYTE* 	SW_pby87U; 		 //差动电流速断
extern BYTE* 	SW_pby87R; 		 //比率差动保护
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2

#define SW_COUNT     5    //软压板个数
#define SW_BUF_SIZE     5    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 200)

//配置----------------------------------
extern BYTE* 	CFG_pby87U_A; 		 //A相差流速断保护
extern BYTE* 	CFG_pby87U_B; 		 //B相差流速断保护
extern BYTE* 	CFG_pby87U_C; 		 //C相差流速断保护
extern BYTE* 	CFG_pby87R_A; 		 //A相比率差动保护
extern BYTE* 	CFG_pby87R_B; 		 //B相比率差动保护
extern BYTE* 	CFG_pby87R_C; 		 //C相比率差动保护
extern BYTE* 	CFG_pby68IdH2M_87R; 		 //差流二次最大相闭锁
extern BYTE* 	CFG_pby68IdH2C_87R; 		 //差流二次综合相闭锁
extern BYTE* 	CFG_pby68IdH2I_87R; 		 //差流二次分相闭锁
extern BYTE* 	CFG_pby68IrH2I_87R; 		 //制动二次闭锁
extern BYTE* 	CFG_pby68IdH5I_87R; 		 //差流五次闭锁
extern BYTE* 	CFG_pby68IdHMUL_87R; 		 //差流综合谐波闭锁
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2

#define CFG_COUNT     15    //配置个数/
#define CFG_BUF_SIZE     15    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 206)

//定值----------------------------------
extern LONG* 	RS_pnCFG1; 		 //配置字 CFG1
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pnTRTY; 		 //变压器类型 TRTY
extern LONG* 	RS_pnLINUM; 		 //低压侧电流相数 LINUM
extern LONG* 	RS_pnCT1CON; 		 //高侧CT接线 CT1CON
extern LONG* 	RS_pn87UIA; 		 //A相差动速断定值 87UIA
extern LONG* 	RS_pn87RIA; 		 //A相差动电流定值 87RIA
extern LONG* 	RS_pnIRS1A; 		 //A相制动拐点1 IRS1A
extern LONG* 	RS_pnIRS2A; 		 //A相制动拐点2 IRS2A
extern LONG* 	RS_pn87UIB; 		 //B相差动速断定值 87UIB
extern LONG* 	RS_pn87RIB; 		 //B相差动电流定值 87RIB
extern LONG* 	RS_pnIRS1B; 		 //B相制动拐点1 IRS1B
extern LONG* 	RS_pnIRS2B; 		 //B相制动拐点2 IRS2B
extern LONG* 	RS_pn87UIC; 		 //C相差动速断定值 87UIC
extern LONG* 	RS_pn87RIC; 		 //C相差动电流定值 87RIC
extern LONG* 	RS_pnIRS1C; 		 //C相制动拐点1 IRS1C
extern LONG* 	RS_pnIRS2C; 		 //C相制动拐点2 IRS2C
extern LONG* 	RS_pnSLP1; 		 //比率制动系数1 SLP1
extern LONG* 	RS_pnSLP2; 		 //比率制动系数2 SLP2
extern LONG* 	RS_pnKrh2; 		 //二次制动系数 Krh2
extern LONG* 	RS_pnKrh5; 		 //五次制动系数 Krh5
extern LONG* 	RS_pnKrh; 		 //综合谐波制动系数 Krh
extern LONG* 	RS_pnKrav; 		 //差动非周期加权 Krav
extern LONG* 	RS_pnK2av; 		 //二次非周期加权 K2av
extern LONG* 	RS_pnKba; 		 //a相平衡系数 Kba
extern LONG* 	RS_pnKbb; 		 //b相平衡系数 Kbb
extern LONG* 	RS_pnKbc; 		 //c相平衡系数 Kbc
extern LONG* 	RS_pnK87AR; 		 //差流越限报警比率 K87AR
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD
extern LONG* 	RS_pnBph; 		 //VV变回流相 Bph

#define RS_COUNT     30    //定值个数/
#define RS_BUF_SIZE     120    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 224)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnT1PI; 		 //T1启动延时
extern LONG* 	AS_pnT1RE; 		 //T1返回延时
extern LONG* 	AS_pnT2PI; 		 //T2启动延时
extern LONG* 	AS_pnT2RE; 		 //T2返回延时
extern LONG* 	AS_pnT3PI; 		 //T3启动延时
extern LONG* 	AS_pnT3RE; 		 //T3返回延时
extern LONG* 	AS_pnT4PI; 		 //T4启动延时
extern LONG* 	AS_pnT4RE; 		 //T4返回延时
extern LONG* 	AS_pnT5PI; 		 //T5启动延时
extern LONG* 	AS_pnT5RE; 		 //T5返回延时
extern LONG* 	AS_pnT6PI; 		 //T6启动延时
extern LONG* 	AS_pnT6RE; 		 //T6返回延时
extern LONG* 	AS_pnT7PI; 		 //T7启动延时
extern LONG* 	AS_pnT7RE; 		 //T7返回延时
extern LONG* 	AS_pnT8PI; 		 //T8启动延时
extern LONG* 	AS_pnT8RE; 		 //T8返回延时
extern LONG* 	AS_pnT9PI; 		 //T9启动延时
extern LONG* 	AS_pnT9RE; 		 //T9返回延时
extern LONG* 	AS_pnT10PI; 		 //T10启动延时
extern LONG* 	AS_pnT10RE; 		 //T10返回延时
extern LONG* 	AS_pnT11PI; 		 //T11启动延时
extern LONG* 	AS_pnT11RE; 		 //T11返回延时
extern LONG* 	AS_pnT12PI; 		 //T12启动延时
extern LONG* 	AS_pnT12RE; 		 //T12返回延时
extern LONG* 	AS_pnT13PI; 		 //T13启动延时
extern LONG* 	AS_pnT13RE; 		 //T13返回延时
extern LONG* 	AS_pnT14PI; 		 //T14启动延时
extern LONG* 	AS_pnT14RE; 		 //T14返回延时
extern LONG* 	AS_pnT15PI; 		 //T15启动延时
extern LONG* 	AS_pnT15RE; 		 //T15返回延时
extern LONG* 	AS_pnT16PI; 		 //T16启动延时
extern LONG* 	AS_pnT16RE; 		 //T16返回延时
extern LONG* 	AS_pn50HAC; 		 //IHA有流定值
extern LONG* 	AS_pn37HAC; 		 //IHA无流定值
extern LONG* 	AS_pn50HBC; 		 //IHB有流定值
extern LONG* 	AS_pn37HBC; 		 //IHB无流定值
extern LONG* 	AS_pn50HCC; 		 //IHC有流定值
extern LONG* 	AS_pn37HCC; 		 //IHC无流定值
extern LONG* 	AS_pn50L1C; 		 //IL1有流定值
extern LONG* 	AS_pn37L1C; 		 //IL1无流定值
extern LONG* 	AS_pn50L2C; 		 //IL2有流定值
extern LONG* 	AS_pn37L2C; 		 //IL2无流定值
extern LONG* 	AS_pn50L3C; 		 //IL3有流定值
extern LONG* 	AS_pn37L3C; 		 //IL3无流定值
extern LONG* 	AS_pn50L4C; 		 //IL4有流定值
extern LONG* 	AS_pn37L4C; 		 //IL4无流定值
extern LONG* 	AS_pnTCB; 		 //断路器动作延时
extern LONG* 	AS_pnTSW; 		 //隔开动作延时
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnTR3D; 		 //TR3保持时间
extern LONG* 	AS_pn59UA; 		 //UA有压定值
extern LONG* 	AS_pn27UA; 		 //UA无压定值
extern LONG* 	AS_pn59UB; 		 //UB有压定值
extern LONG* 	AS_pn27UB; 		 //UB无压定值
extern LONG* 	AS_pn59UC; 		 //UC有压定值
extern LONG* 	AS_pn27UC; 		 //UC无压定值
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnHACT; 		 //高压侧A相CT变比
extern LONG* 	AS_pnHBCT; 		 //高压侧B相CT变比
extern LONG* 	AS_pnHCCT; 		 //高压侧C相CT变比
extern LONG* 	AS_pnL1CT; 		 //低压侧CT1变比
extern LONG* 	AS_pnL2CT; 		 //低压侧CT2变比
extern LONG* 	AS_pnL3CT; 		 //低压侧CT3变比
extern LONG* 	AS_pnL4CT; 		 //低压侧CT4变比
extern LONG* 	AS_pnHPT; 		 //高压侧PT变比
extern LONG* 	AS_pnRT; 		 //固定变比

#define AS_RT_NUM     9    //辅助定值中变比个数

#define AS_COUNT     68    //辅助定值个数
#define AS_BUF_SIZE     272    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 344)

//系数----------------------------------
extern LONG* 	CF_pnIA; 		 //IA
extern LONG* 	CF_pnIB; 		 //IB
extern LONG* 	CF_pnIC; 		 //IC
extern LONG* 	CF_pnI1; 		 //I1
extern LONG* 	CF_pnI2; 		 //I2
extern LONG* 	CF_pnI3; 		 //I3
extern LONG* 	CF_pnI4; 		 //I4
extern LONG* 	CF_pnIDA; 		 //IDA
extern LONG* 	CF_pnIDB; 		 //IDB
extern LONG* 	CF_pnIDC; 		 //IDC
extern LONG* 	CF_pnIRA; 		 //IRA
extern LONG* 	CF_pnIRB; 		 //IRB
extern LONG* 	CF_pnIRC; 		 //IRC
extern LONG* 	CF_pnU; 		 //U
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnUAM; 		 //UAM
extern LONG* 	CF_pnUBM; 		 //UBM
extern LONG* 	CF_pnUCM; 		 //UCM
extern LONG* 	CF_pnIAM; 		 //IAM
extern LONG* 	CF_pnIBM; 		 //IBM
extern LONG* 	CF_pnICM; 		 //ICM
extern LONG* 	CF_pnI1M; 		 //I1M
extern LONG* 	CF_pnI2M; 		 //I2M
extern LONG* 	CF_pnI3M; 		 //I3M
extern LONG* 	CF_pnI4M; 		 //I4M
extern LONG* 	CF_pnUAD; 		 //UAD
extern LONG* 	CF_pnUBD; 		 //UBD
extern LONG* 	CF_pnUCD; 		 //UCD
extern LONG* 	CF_pnIAD; 		 //IAD
extern LONG* 	CF_pnIBD; 		 //IBD
extern LONG* 	CF_pnICD; 		 //ICD
extern LONG* 	CF_pnI1D; 		 //I1D
extern LONG* 	CF_pnI2D; 		 //I2D
extern LONG* 	CF_pnI3D; 		 //I3D
extern LONG* 	CF_pnI4D; 		 //I4D

#define CF_COUNT     38    //系数个数
#define CF_BUF_SIZE     152    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 616)

//报告访问宏-----------------------
#define R_OEERR                     0    //开出光隔失效
#define R_OEBR                      1    //开出光隔击穿
#define R_ADERR                     2    //AD故障
#define R_SLERR                     3    //软压板故障
#define R_EEPROMERR                 4    //EEPROM定值故障
#define R_RAMERR                    5    //RAM定值故障
#define R_ROMERR                    6    //程序ROM故障
#define R_RESETAN                   7    //装置异常复位
#define R_COMERR                    8    //通信通道异常
#define R_NVRAMERR                  9    //NVRAM内容出错
#define R_LOGICERR                 10    //逻辑方程故障
#define R_RCERR                    11    //保护自检错误
#define R_POWERERR                 12    //5V电源故障
#define R_EEPROM_W_ERR             13    //EEPROM写入故障
#define R_ADERR_R                  14    //AD故障恢复
#define R_SLERR_R                  15    //软压板故障恢复
#define R_EEPROMERR_R              16    //EEPROM定值故障恢复
#define R_RAMERR_R                 17    //RAM定值故障恢复
#define R_ROMERR_R                 18    //程序ROM故障恢复
#define R_COMERR_R                 19    //通信通道异常恢复
#define R_NVRAMERR_R               20    //NVRAM内容出错恢复
#define R_LOGICERR_R               21    //逻辑方程故障故障恢复
#define R_POWERERR_R               22    //5V电源故障恢复
#define R_ACTPICK                  23    //保护启动
#define R_RTR_GAS                  24    //重瓦斯
#define R_RTR_YLSF                 25    //压力释放
#define R_RTR_23                   26    //超温跳闸
#define R_R74_GAS                  27    //轻瓦斯
#define R_R74_OIL                  28    //油位异常
#define R_R74_23                   29    //温度过高
#define R_RLP                      30    //失电告警
#define R_RTR_GAS_1                31    //重瓦斯1
#define R_RTR_YLSF_1               32    //压力释放1
#define R_RTR_23_1                 33    //超温跳闸1
#define R_R74_GAS_1                34    //轻瓦斯1
#define R_R74_OIL_1                35    //油位异常1
#define R_R74_23_1                 36    //温度过高1
#define R_RLP_1                    37    //失电告警1
#define R_RTR_GAS_2                38    //重瓦斯2
#define R_RTR_YLSF_2               39    //压力释放2
#define R_RTR_23_2                 40    //超温跳闸2
#define R_R74_GAS_2                41    //轻瓦斯2
#define R_R74_OIL_2                42    //油位异常2
#define R_R74_23_2                 43    //温度过高2
#define R_RLP_2                    44    //失电告警2
#define R_AR87R                    45    //差流越限
#define R_R87UA_PI                 46    //A相差动速断启动
#define R_R87UA                    47    //A相差流速断动作
#define R_R87UA_RES                48    //A相差动速断返回
#define R_R87UB_PI                 49    //B相差动速断启动
#define R_R87UB                    50    //B相差动速断动作
#define R_R87UB_RES                51    //B相差动速断返回
#define R_R87UC_PI                 52    //C相差动速断启动
#define R_R87UC                    53    //C相差动速断动作
#define R_R87UC_RES                54    //C相差动速断返回
#define R_R87RA_PI                 55    //A相比率差动启动
#define R_R87RA                    56    //A相比率差动动作
#define R_R87RA_RES                57    //A相比率差动返回
#define R_R87RB_PI                 58    //B相比率差动启动
#define R_R87RB                    59    //B相比率差动动作
#define R_R87RB_RES                60    //B相比率差动返回
#define R_R87RC_PI                 61    //C相比率差动启动
#define R_R87RC                    62    //C相比率差动动作
#define R_R87RC_RES                63    //C相比率差动返回
#define R_COMTEST                  64    //通道测试
#define R_PICKUP                   65    //保护启动
#define R_TRFAIL                   66    //断路器跳闸失败
#define R_52BF                     67    //失灵保护
#define R_AR87R_RES                68    //差流越限返回
#define R_RELDATAERR               69    //保护数据异常
#define R_RELDATAERR_R             70    //保护数据异常恢复
#define MAX_REPORT_INDEX           70    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     15    //传动个数

#endif