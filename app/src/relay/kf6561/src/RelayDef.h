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
#define 	 FAILDATA_DISPTYPE_CFGNO 	 47 		 //故障电量显示类型配置序号
#define 	 LangTYPE_SYSSETNO 	 0 		 //语言类型辅助定值序号

//当前定值区号定义-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //当前定值区号

//定值取值宏定义

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----额定电压-----
#define 	PHASEU 	0	//57.7V
#define 	LINEU 	1	//100V

//保护测量值定义-----------------------
#define  RM_UA 0
extern LONG* 	RM_pnUa; 		 // Ua
#define  RM_UB 1
extern LONG* 	RM_pnUb; 		 // Ub
#define  RM_UC 2
extern LONG* 	RM_pnUc; 		 // Uc
#define  RM_U0 3
extern LONG* 	RM_pnU0; 		 // U0
#define  RM_UL1 4
extern LONG* 	RM_pnUL1; 		 // UL1
#define  RM_UL2 5
extern LONG* 	RM_pnUL2; 		 // UL2
#define  RM_UL3 6
extern LONG* 	RM_pnUL3; 		 // UL3
#define  RM_UAB 7
extern LONG* 	RM_pnUab; 		 // Uab
#define  RM_UBC 8
extern LONG* 	RM_pnUbc; 		 // Ubc
#define  RM_UCA 9
extern LONG* 	RM_pnUca; 		 // Uca
#define  RM_U2 10
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_U05 11
extern LONG* 	RM_pnU05; 		 // U05
#define  RM_IA 12
extern LONG* 	RM_pnIa; 		 // Ia
#define  RM_IB 13
extern LONG* 	RM_pnIb; 		 // Ib
#define  RM_IC 14
extern LONG* 	RM_pnIc; 		 // Ic
#define  RM_I0 15
extern LONG* 	RM_pnI0; 		 // I0
#define  RM_I1 16
extern LONG* 	RM_pnI1; 		 // I1
#define  RM_I05 17
extern LONG* 	RM_pnI05; 		 // I05
#define  RM_AUA 18
extern LONG* 	RM_pnAUa; 		 // ΦUa
#define  RM_AUB 19
extern LONG* 	RM_pnAUb; 		 // ΦUb
#define  RM_AUC 20
extern LONG* 	RM_pnAUc; 		 // ΦUc
#define  RM_AUAB 21
extern LONG* 	RM_pnAUab; 		 // ΦUab
#define  RM_AUBC 22
extern LONG* 	RM_pnAUbc; 		 // ΦUbc
#define  RM_AUCA 23
extern LONG* 	RM_pnAUca; 		 // ΦUca
#define  RM_AUL1 24
extern LONG* 	RM_pnAUl1; 		 // ΦUl1
#define  RM_AUL2 25
extern LONG* 	RM_pnAUl2; 		 // ΦUl2
#define  RM_AUL3 26
extern LONG* 	RM_pnAUl3; 		 // ΦUl3
#define  RM_AIPA 27
extern LONG* 	RM_pnAIpa; 		 // ΦIpa
#define  RM_AIPB 28
extern LONG* 	RM_pnAIpb; 		 // ΦIpb
#define  RM_AIPC 29
extern LONG* 	RM_pnAIpc; 		 // ΦIpc
#define  RM_AIP1 30
extern LONG* 	RM_pnAIP1; 		 // ΦIp1
#define  RM_AIMA 31
extern LONG* 	RM_pnAIma; 		 // ΦIma
#define  RM_AIMB 32
extern LONG* 	RM_pnAImb; 		 // ΦImb
#define  RM_AIMC 33
extern LONG* 	RM_pnAImc; 		 // ΦImc
#define  RM_AU01 34
extern LONG* 	RM_pnAU01; 		 // ΦU01
#define  RM_AI01 35
extern LONG* 	RM_pnAI01; 		 // ΦI01
#define  RM_AU05 36
extern LONG* 	RM_pnAU05; 		 // ΦU05
#define  RM_AI05 37
extern LONG* 	RM_pnAI05; 		 // ΦI05
#define  RM_F1 38
extern LONG* 	RM_pnF1; 		 // F1
#define  RM_IAM 39
extern LONG* 	RM_pnIam; 		 // Iam
#define  RM_IBM 40
extern LONG* 	RM_pnIbm; 		 // Ibm
#define  RM_ICM 41
extern LONG* 	RM_pnIcm; 		 // Icm
#define  RM_BOBRCH1 42
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 43
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 44
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 45
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     46    //保护测量值个数
#define RM_BUF_SIZE     184    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UA 0
extern LONG* 	M_pnUa; 		 //Ua Ua
#define  M_UB 1
extern LONG* 	M_pnUb; 		 //Ub Ub
#define  M_UC 2
extern LONG* 	M_pnUc; 		 //Uc Uc
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_UAB 4
extern LONG* 	M_pnUab; 		 //Uab Uab
#define  M_UBC 5
extern LONG* 	M_pnUbc; 		 //Ubc Ubc
#define  M_UCA 6
extern LONG* 	M_pnUca; 		 //Uca Uca
#define  M_U1 7
extern LONG* 	M_pnU1; 		 //U1 U1
#define  M_U2 8
extern LONG* 	M_pnU2; 		 //U2 U2
#define  M_U3 9
extern LONG* 	M_pnU3; 		 //U3 U3
#define  M_IA 10
extern LONG* 	M_pnIa; 		 //Ia Ia
#define  M_IB 11
extern LONG* 	M_pnIb; 		 //Ib Ib
#define  M_IC 12
extern LONG* 	M_pnIc; 		 //Ic Ic
#define  M_I0 13
extern LONG* 	M_pnI0; 		 //I0 I0
#define  M_I1 14
extern LONG* 	M_pnI1; 		 //I1 I1
#define  M_P 15
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 16
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 17
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_F1 18
extern LONG* 	M_pnF1; 		 //F1 F1

#define MEASURE_COUNT     19    //测量值个数
#define MEASURE_BUF_SIZE     76    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 188)

//软压板----------------------------------
extern BYTE* 	SW_pby50I0; 		 //电流速断
extern BYTE* 	SW_pby50I1; 		 //过流保护
extern BYTE* 	SW_pby59U; 		 //过压保护
extern BYTE* 	SW_pby27U; 		 //低压保护
extern BYTE* 	SW_pby60I; 		 //不平衡电流
extern BYTE* 	SW_pby60U; 		 //不平衡电压
extern BYTE* 	SW_pbyJDXX; 		 //接地选线1
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2

#define SW_COUNT     10    //软压板个数
#define SW_BUF_SIZE     10    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 264)

//配置----------------------------------
extern BYTE* 	CFG_pby50I0; 		 //电流速断
extern BYTE* 	CFG_pby50I1; 		 //过流保护
extern BYTE* 	CFG_pby59U; 		 //过压保护
extern BYTE* 	CFG_pby27U; 		 //低压保护
extern BYTE* 	CFG_pby60I; 		 //不平衡电流保护
extern BYTE* 	CFG_pby60U; 		 //不平衡电压保护
extern BYTE* 	CFG_pby59N; 		 //零压告警
extern BYTE* 	CFG_pbyJDXX; 		 //接地选线
extern BYTE* 	CFG_pbyXHXQ; 		 //消弧线圈接地
extern BYTE* 	CFG_pbyPTBR; 		 //PT断线检查
extern BYTE* 	CFG_pbyCTBR; 		 //CT断线检查
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2

#define CFG_COUNT     14    //配置个数/
#define CFG_BUF_SIZE     14    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 276)

//定值----------------------------------
extern LONG* 	RS_pnCFG; 		 //配置字 CFG
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn50I0C; 		 //电流速断电流 50I0C
extern LONG* 	RS_pn50I0D; 		 //电流速断时间 50I0D
extern LONG* 	RS_pn50I1C; 		 //过流电流定值 50I1C
extern LONG* 	RS_pn50I1D; 		 //过流时间定值 50I1D
extern LONG* 	RS_pn59U; 		 //过压电压定值 59U
extern LONG* 	RS_pn59D; 		 //过压时间定值 59D
extern LONG* 	RS_pn27U; 		 //低压保护电压 27U
extern LONG* 	RS_pn27D; 		 //低压保护时间 27D
extern LONG* 	RS_pn27ILO; 		 //低压闭锁电流定值 27ILO
extern LONG* 	RS_pn60IC; 		 //不平衡电流保护电流 60IC
extern LONG* 	RS_pn60ID; 		 //不平衡电流保护时间 60ID
extern LONG* 	RS_pn60UV; 		 //不平衡电压保护电压 60UV
extern LONG* 	RS_pn60UD; 		 //不平衡电压保护时间 60UD
extern LONG* 	RS_pn59NU; 		 //零压告警电压 59NU
extern LONG* 	RS_pn59ND; 		 //零压告警时间 59ND
extern LONG* 	RS_pn74PTU; 		 //PT断线检测电压 74PTU
extern LONG* 	RS_pn74PTI; 		 //PT断线检测电流 74PTI
extern LONG* 	RS_pn74PTD; 		 //PT断线检测时间 74PTD
extern LONG* 	RS_pn74PTU2; 		 //负序电压检测定值 74PTU2
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD

#define RS_COUNT     22    //定值个数/
#define RS_BUF_SIZE     88    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 292)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnPM; 		 //额定电压
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
extern LONG* 	AS_pnTCB; 		 //断路器动作延时
extern LONG* 	AS_pnTSW; 		 //电隔动作延时
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnTR3D; 		 //TR3保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间
extern LONG* 	AS_pn59UA; 		 //Ua有压定值
extern LONG* 	AS_pn27UA; 		 //Ua无压定值
extern LONG* 	AS_pn59UB; 		 //Ub有压定值
extern LONG* 	AS_pn27UB; 		 //Ub无压定值
extern LONG* 	AS_pn59UC; 		 //Uc有压定值
extern LONG* 	AS_pn27UC; 		 //Uc无压定值
extern LONG* 	AS_pn59U0; 		 //U0有压定值
extern LONG* 	AS_pn27U0; 		 //U0无压定值
extern LONG* 	AS_pn59U1; 		 //U1有压定值
extern LONG* 	AS_pn27U1; 		 //U1无压定值
extern LONG* 	AS_pn59U2; 		 //U2有压定值
extern LONG* 	AS_pn27U2; 		 //U2无压定值
extern LONG* 	AS_pn59U3; 		 //U3有压定值
extern LONG* 	AS_pn27U3; 		 //U3无压定值
extern LONG* 	AS_pn50IA; 		 //Ia有流定值
extern LONG* 	AS_pn37IA; 		 //Ia无流定值
extern LONG* 	AS_pn50IB; 		 //Ib有流定值
extern LONG* 	AS_pn37IB; 		 //Ib无流定值
extern LONG* 	AS_pn50IC; 		 //Ic有流定值
extern LONG* 	AS_pn37IC; 		 //Ic无流定值
extern LONG* 	AS_pn50I0; 		 //I0有流定值
extern LONG* 	AS_pn37I0; 		 //I0无流定值
extern LONG* 	AS_pn50I1; 		 //I1有流定值
extern LONG* 	AS_pn37I1; 		 //I1无流定值

//变比----------------------------------
extern LONG* 	AS_pnPTRatio; 		 //PT变比
extern LONG* 	AS_pnPCTRatio; 		 //保护CT变比
extern LONG* 	AS_pnMCTRatio; 		 //测量CT变比
extern LONG* 	AS_pnZCTRatio; 		 //零序CT变比
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     5    //辅助定值中变比个数

#define AS_COUNT     69    //辅助定值个数
#define AS_BUF_SIZE     276    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 380)

//系数----------------------------------
extern LONG* 	CF_pnUp; 		 //Up
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnIp; 		 //Ip
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnI1; 		 //I1
extern LONG* 	CF_pnUaM; 		 //UaM
extern LONG* 	CF_pnUbM; 		 //UbM
extern LONG* 	CF_pnUcM; 		 //UcM
extern LONG* 	CF_pnUL1M; 		 //UL1M
extern LONG* 	CF_pnUL2M; 		 //UL2M
extern LONG* 	CF_pnUL3M; 		 //UL3M
extern LONG* 	CF_pnU0M; 		 //U0M
extern LONG* 	CF_pnIAM; 		 //IAM
extern LONG* 	CF_pnIBM; 		 //IBM
extern LONG* 	CF_pnICM; 		 //ICM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnI1M; 		 //I1M
extern LONG* 	CF_pnPA; 		 //PA
extern LONG* 	CF_pnQA; 		 //QA
extern LONG* 	CF_pnPB; 		 //PB
extern LONG* 	CF_pnQB; 		 //QB
extern LONG* 	CF_pnPC; 		 //PC
extern LONG* 	CF_pnQC; 		 //QC
extern LONG* 	CF_pnAA; 		 //AA
extern LONG* 	CF_pnBA; 		 //BA
extern LONG* 	CF_pnCA; 		 //CA
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnIaDRIFT; 		 //IaDRIFT
extern LONG* 	CF_pnIbDRIFT; 		 //IbDRIFT
extern LONG* 	CF_pnIcDRIFT; 		 //IcDRIFT
extern LONG* 	CF_pnI0DRIFT; 		 //I0DRIFT
extern LONG* 	CF_pnI1DRIFT; 		 //I1DRIFT
extern LONG* 	CF_pnUaDRIFT; 		 //UaDRIFT
extern LONG* 	CF_pnUbDRIFT; 		 //UbDRIFT
extern LONG* 	CF_pnUcDRIFT; 		 //UcDRIFT
extern LONG* 	CF_pnU0DRIFT; 		 //U0DRIFT
extern LONG* 	CF_pnUl1DRIFT; 		 //Ul1DRIFT
extern LONG* 	CF_pnUl2DRIFT; 		 //Ul2DRIFT
extern LONG* 	CF_pnUl3DRIFT; 		 //Ul3DRIFT

#define CF_COUNT     42    //系数个数
#define CF_BUF_SIZE     168    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 656)

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
#define R_ADERR_R                  12    //AD故障恢复
#define R_SLERR_R                  13    //软压板故障恢复
#define R_EEPROMERR_R              14    //EEPROM定值故障恢复
#define R_RAMERR_R                 15    //RAM定值故障恢复
#define R_ROMERR_R                 16    //程序ROM故障恢复
#define R_COMERR_R                 17    //通信通道异常恢复
#define R_NVRAMERR_R               18    //NVRAM内容出错恢复
#define R_LOGICERR_R               19    //逻辑方程故障恢复
#define R_ACTPICK                  20    //保护启动
#define R_R50I0_PI                 21    //电流速断启动
#define R_R50I0                    22    //电流速断动作
#define R_R50I0_RES                23    //电流速断返回
#define R_R50I1_PI                 24    //过流保护启动
#define R_R50I1                    25    //过流保护动作
#define R_R50I1_RES                26    //过流保护返回
#define R_R60I_PI                  27    //不平衡电流保护启动
#define R_R60I                     28    //不平衡电流保护动作
#define R_R60I_RES                 29    //不平衡电流保护返回
#define R_R60U_PI                  30    //不平衡电压保护启动
#define R_R60U                     31    //不平衡电压保护动作
#define R_R60U_RES                 32    //不平衡电压保护返回
#define R_R59U_PI                  33    //过压保护启动
#define R_R59U                     34    //过压保护动作
#define R_R59U_RES                 35    //过压保护返回
#define R_R27U                     36    //低压保护动作
#define R_R59NU                    37    //零压告警
#define R_R74PT                    38    //母线PT断线
#define R_64LINE                   39    //线路接地
#define R_64BUS                    40    //母线接地
#define R_PICKUP                   41    //保护启动
#define R_TRFAIL                   42    //断路器跳闸失败
#define R_COMTEST                  43    //通道测试
#define R_52BF                     44    //失灵保护
#define R_CTBR_A                   45    //A相CT断线
#define R_CTBR_A_RES               46    //A相CT断线恢复
#define R_CTBR_B                   47    //B相CT断线
#define R_CTBR_B_RES               48    //B相CT断线恢复
#define R_CTBR_C                   49    //C相CT断线
#define R_CTBR_C_RES               50    //C相CT断线恢复
#define R_POWERERR                 51    //5V电源故障
#define R_EEPROM_W_ERR             52    //EEPROM写入故障
#define R_POWERERR_R               53    //5V电源故障恢复
#define R_R74PT_RES                54    //母线PT断线恢复
#define R_RELDATAERR               55    //保护数据异常
#define R_RELDATAERR_R             56    //保护数据异常恢复
#define MAX_REPORT_INDEX           56    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     10    //传动个数

#endif