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
//----高压侧电流角度校正-----
#define 	WCTH_0 	0	//不校正
#define 	WCTH_1 	1	//30°校正
#define 	WCTH_2 	2	//60°校正
#define 	WCTH_3 	3	//90°校正
#define 	WCTH_4 	4	//120°校正
#define 	WCTH_5 	5	//150°校正
#define 	WCTH_6 	6	//180°校正
#define 	WCTH_7 	7	//210°校正
#define 	WCTH_8 	8	//240°校正
#define 	WCTH_9 	9	//270°校正
#define 	WCTH_10 	10	//300°校正
#define 	WCTH_11 	11	//330°校正
#define 	WCTH_12 	12	//360°校正
//----中压侧电流角度校正-----
#define 	WCTM_0 	0	//不校正
#define 	WCTM_1 	1	//30°校正
#define 	WCTM_2 	2	//60°校正
#define 	WCTM_3 	3	//90°校正
#define 	WCTM_4 	4	//120°校正
#define 	WCTM_5 	5	//150°校正
#define 	WCTM_6 	6	//180°校正
#define 	WCTM_7 	7	//210°校正
#define 	WCTM_8 	8	//240°校正
#define 	WCTM_9 	9	//270°校正
#define 	WCTM_10 	10	//300°校正
#define 	WCTM_11 	11	//330°校正
#define 	WCTM_12 	12	//360°校正
//----低压侧电流角度校正-----
#define 	WCTL_0 	0	//不校正
#define 	WCTL_1 	1	//30°校正
#define 	WCTL_2 	2	//60°校正
#define 	WCTL_3 	3	//90°校正
#define 	WCTL_4 	4	//120°校正
#define 	WCTL_5 	5	//150°校正
#define 	WCTL_6 	6	//180°校正
#define 	WCTL_7 	7	//210°校正
#define 	WCTL_8 	8	//240°校正
#define 	WCTL_9 	9	//270°校正
#define 	WCTL_10 	10	//300°校正
#define 	WCTL_11 	11	//330°校正
#define 	WCTL_12 	12	//360°校正
//----高侧绕组/CT连接方式-----
#define 	WCTH_0 	0	//星/星型
#define 	WCTH_1 	1	//星/角型
#define 	WCTH_2 	2	//角/角型
//----中侧绕组/CT连接方式-----
#define 	WCTM_0 	0	//星/星型
#define 	WCTM_1 	1	//星/角型
#define 	WCTM_2 	2	//角/角型
//----低侧绕组/CT连接方式-----
#define 	WCTL_0 	0	//星/星型
#define 	WCTL_1 	1	//星/角型
#define 	WCTL_2 	2	//角/角型
//----中压侧公共相-----
#define 	MBPH_0 	0	//A相
#define 	MBPH_1 	1	//B相
#define 	MBPH_2 	2	//C相
//----低压侧公共相-----
#define 	LBPH_0 	0	//A相
#define 	LBPH_1 	1	//B相
#define 	LBPH_2 	2	//C相

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//保护测量值定义-----------------------
#define  RM_UA 0
extern LONG* 	RM_pnUA; 		 // UA
#define  RM_UB 1
extern LONG* 	RM_pnUB; 		 // UB
#define  RM_UC 2
extern LONG* 	RM_pnUC; 		 // UC
#define  RM_UAB 3
extern LONG* 	RM_pnUAB; 		 // UAB
#define  RM_UBC 4
extern LONG* 	RM_pnUBC; 		 // UBC
#define  RM_UCA 5
extern LONG* 	RM_pnUCA; 		 // UCA
#define  RM_U2H 6
extern LONG* 	RM_pnU2H; 		 // U2H
#define  RM_U0 7
extern LONG* 	RM_pnU0; 		 // U0
#define  RM_IHA 8
extern LONG* 	RM_pnIHA; 		 // IHA
#define  RM_IHB 9
extern LONG* 	RM_pnIHB; 		 // IHB
#define  RM_IHC 10
extern LONG* 	RM_pnIHC; 		 // IHC
#define  RM_IMA 11
extern LONG* 	RM_pnIMA; 		 // IMA
#define  RM_IMB 12
extern LONG* 	RM_pnIMB; 		 // IMB
#define  RM_IMC 13
extern LONG* 	RM_pnIMC; 		 // IMC
#define  RM_ILA 14
extern LONG* 	RM_pnILA; 		 // ILA
#define  RM_ILB 15
extern LONG* 	RM_pnILB; 		 // ILB
#define  RM_ILC 16
extern LONG* 	RM_pnILC; 		 // ILC
#define  RM_I0 17
extern LONG* 	RM_pnI0; 		 // I0
#define  RM_IJX 18
extern LONG* 	RM_pnIjx; 		 // Ijx
#define  RM_IDA 19
extern LONG* 	RM_pnIDA; 		 // IDA
#define  RM_IDB 20
extern LONG* 	RM_pnIDB; 		 // IDB
#define  RM_IDC 21
extern LONG* 	RM_pnIDC; 		 // IDC
#define  RM_IRA 22
extern LONG* 	RM_pnIRA; 		 // IRA
#define  RM_IRB 23
extern LONG* 	RM_pnIRB; 		 // IRB
#define  RM_IRC 24
extern LONG* 	RM_pnIRC; 		 // IRC
#define  RM_IDA2 25
extern LONG* 	RM_pnIDA2; 		 // IDA2
#define  RM_IDA3 26
extern LONG* 	RM_pnIDA3; 		 // IDA3
#define  RM_IDA5 27
extern LONG* 	RM_pnIDA5; 		 // IDA5
#define  RM_IDB2 28
extern LONG* 	RM_pnIDB2; 		 // IDB2
#define  RM_IDB3 29
extern LONG* 	RM_pnIDB3; 		 // IDB3
#define  RM_IDB5 30
extern LONG* 	RM_pnIDB5; 		 // IDB5
#define  RM_IDC2 31
extern LONG* 	RM_pnIDC2; 		 // IDC2
#define  RM_IDC3 32
extern LONG* 	RM_pnIDC3; 		 // IDC3
#define  RM_IDC5 33
extern LONG* 	RM_pnIDC5; 		 // IDC5
#define  RM_IRA2 34
extern LONG* 	RM_pnIRA2; 		 // IRA2
#define  RM_IRB2 35
extern LONG* 	RM_pnIRB2; 		 // IRB2
#define  RM_IRC2 36
extern LONG* 	RM_pnIRC2; 		 // IRC2
#define  RM_AUA 37
extern LONG* 	RM_pnAUA; 		 // ΦUA
#define  RM_AUB 38
extern LONG* 	RM_pnAUB; 		 // ΦUB
#define  RM_AUC 39
extern LONG* 	RM_pnAUC; 		 // ΦUC
#define  RM_AIHA 40
extern LONG* 	RM_pnAIHA; 		 // ΦIHA
#define  RM_AIHB 41
extern LONG* 	RM_pnAIHB; 		 // ΦIHB
#define  RM_AIHC 42
extern LONG* 	RM_pnAIHC; 		 // ΦIHC
#define  RM_AIMA 43
extern LONG* 	RM_pnAIMA; 		 // ΦIMA
#define  RM_AIMB 44
extern LONG* 	RM_pnAIMB; 		 // ΦIMB
#define  RM_AIMC 45
extern LONG* 	RM_pnAIMC; 		 // ΦIMC
#define  RM_AILA 46
extern LONG* 	RM_pnAILA; 		 // ΦILA
#define  RM_AILB 47
extern LONG* 	RM_pnAILB; 		 // ΦILB
#define  RM_AILC 48
extern LONG* 	RM_pnAILC; 		 // ΦILC
#define  RM_BOBRCH1 49
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 50
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 51
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 52
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     53    //保护测量值个数
#define RM_BUF_SIZE     212    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UA 0
extern LONG* 	M_pnUA; 		 //UA UA
#define  M_UB 1
extern LONG* 	M_pnUB; 		 //UB UB
#define  M_UC 2
extern LONG* 	M_pnUC; 		 //UC UC
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_IHAM 4
extern LONG* 	M_pnIHAM; 		 //IHAM IHAM
#define  M_IHBM 5
extern LONG* 	M_pnIHBM; 		 //IHBM IHBM
#define  M_IHCM 6
extern LONG* 	M_pnIHCM; 		 //IHCM IHCM
#define  M_IMAM 7
extern LONG* 	M_pnIMAM; 		 //IMAM IMAM
#define  M_IMBM 8
extern LONG* 	M_pnIMBM; 		 //IMBM IMBM
#define  M_IMCM 9
extern LONG* 	M_pnIMCM; 		 //IMCM IMCM
#define  M_ILAM 10
extern LONG* 	M_pnILAM; 		 //ILAM ILAM
#define  M_ILBM 11
extern LONG* 	M_pnILBM; 		 //ILBM ILBM
#define  M_ILCM 12
extern LONG* 	M_pnILCM; 		 //ILCM ILCM
#define  M_I0M 13
extern LONG* 	M_pnI0M; 		 //I0M I0M
#define  M_IJXM 14
extern LONG* 	M_pnIjxM; 		 //IjxM IjxM

#define MEASURE_COUNT     15    //测量值个数
#define MEASURE_BUF_SIZE     60    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 216)

//软压板----------------------------------
extern BYTE* 	SW_pby87U; 		 //差动电流速断
extern BYTE* 	SW_pby87R; 		 //比率差动保护
extern BYTE* 	SW_pby50I_H; 		 //高压侧过流保护
extern BYTE* 	SW_pby50I_M; 		 //中压侧过流保护
extern BYTE* 	SW_pby50I_L; 		 //低压侧过流保护
extern BYTE* 	SW_pby50OL; 		 //过负荷保护
extern BYTE* 	SW_pby27U; 		 //失压保护
extern BYTE* 	SW_pby59UN; 		 //零序过压保护
extern BYTE* 	SW_pby50IN; 		 //零序过流保护
extern BYTE* 	SW_pby50IJX; 		 //间隙过流保护
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2

#define SW_COUNT     13    //软压板个数
#define SW_BUF_SIZE     13    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 276)

//配置----------------------------------
extern BYTE* 	CFG_pby87U_A; 		 //A相差流速断保护
extern BYTE* 	CFG_pby87U_B; 		 //B相差流速断保护
extern BYTE* 	CFG_pby87U_C; 		 //C相差流速断保护
extern BYTE* 	CFG_pby87R_A; 		 //A相比率差动保护
extern BYTE* 	CFG_pby87R_B; 		 //B相比率差动保护
extern BYTE* 	CFG_pby87R_C; 		 //C相比率差动保护
extern BYTE* 	CFG_pbyDCRES_87R; 		 //差动非周期分量抑制
extern BYTE* 	CFG_pby68IdH2M_87R; 		 //差流二次最大相闭锁
extern BYTE* 	CFG_pby68IdH2C_87R; 		 //差流二次综合相闭锁
extern BYTE* 	CFG_pby68IdH2I_87R; 		 //差流二次分相闭锁
extern BYTE* 	CFG_pby68IrH2I_87R; 		 //制动电流二次闭锁
extern BYTE* 	CFG_pby68IdH5I_87R; 		 //差流五次谐波闭锁
extern BYTE* 	CFG_pby68IdHMUL_87R; 		 //差流综合谐波闭锁
extern BYTE* 	CFG_pbyMAB; 		 //中压侧两相
extern BYTE* 	CFG_pbyLAB; 		 //低压侧两相
extern BYTE* 	CFG_pby50I_H1; 		 //高压侧过流Ⅰ段
extern BYTE* 	CFG_pby50I_H1_PD; 		 //高过流Ⅰ段功率方向
extern BYTE* 	CFG_pby50I_H1_47U; 		 //高过流Ⅰ段复压闭锁
extern BYTE* 	CFG_pby50I_H2; 		 //高压侧过流Ⅱ段
extern BYTE* 	CFG_pby50I_H2_PD; 		 //高过流Ⅱ段功率方向
extern BYTE* 	CFG_pby50I_H2_47U; 		 //高过流Ⅱ段复压闭锁
extern BYTE* 	CFG_pby50I_M1; 		 //中压侧过流Ⅰ段
extern BYTE* 	CFG_pby50I_M2; 		 //中压侧过流Ⅱ段
extern BYTE* 	CFG_pby50I_L1; 		 //低压侧过流Ⅰ段
extern BYTE* 	CFG_pby50I_L2; 		 //低压侧过流Ⅱ段
extern BYTE* 	CFG_pby50OL_1; 		 //过负荷Ⅰ段
extern BYTE* 	CFG_pby50OL_2; 		 //过负荷Ⅱ段
extern BYTE* 	CFG_pby50OL_3; 		 //过负荷Ⅲ段
extern BYTE* 	CFG_pby27U; 		 //失压保护
extern BYTE* 	CFG_pby27U_50I; 		 //失压保护电流闭锁
extern BYTE* 	CFG_pby59UN; 		 //零序过压保护
extern BYTE* 	CFG_pby50IN; 		 //零序过流保护
extern BYTE* 	CFG_pby50IJX; 		 //间隙过流保护
extern BYTE* 	CFG_pby74VT; 		 //PT断线
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2

#define CFG_COUNT     37    //配置个数/
#define CFG_BUF_SIZE     37    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 290)

//定值----------------------------------
extern LONG* 	RS_pnCFG1; 		 //配置字1 CFG1
extern LONG* 	RS_pnCFG2; 		 //配置字2 CFG2
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pnWHCTC; 		 //高压侧电流角度校正 WHCTC
extern LONG* 	RS_pnWMCTC; 		 //中压侧电流角度校正 WMCTC
extern LONG* 	RS_pnWLCTC; 		 //低压侧电流角度校正 WLCTC
extern LONG* 	RS_pnWCTHCON; 		 //高侧绕组/CT连接方式 WCTHCON
extern LONG* 	RS_pnWCTMCON; 		 //中侧绕组/CT连接方式 WCTMCON
extern LONG* 	RS_pnWCTLCON; 		 //低侧绕组/CT连接方式 WCTLCON
extern LONG* 	RS_pnMBph; 		 //中压侧公共相 MBph
extern LONG* 	RS_pnLBph; 		 //低压侧公共相 LBph
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
extern LONG* 	RS_pnKrh2; 		 //二次谐波制动系数 Krh2
extern LONG* 	RS_pnKrh5; 		 //五次谐波制动系数 Krh5
extern LONG* 	RS_pnKrhm; 		 //综合谐波制动系数 Krhm
extern LONG* 	RS_pnKrav; 		 //差动非周期加权系数 Krav
extern LONG* 	RS_pnK2av; 		 //二次非周期加权系数 K2av
extern LONG* 	RS_pnKma; 		 //中压侧A相平衡系数 Kma
extern LONG* 	RS_pnKmb; 		 //中压侧B相平衡系数 Kmb
extern LONG* 	RS_pnKmc; 		 //中压侧C相平衡系数 Kmc
extern LONG* 	RS_pnKla; 		 //低压侧A相平衡系数 Kla
extern LONG* 	RS_pnKlb; 		 //低压侧B相平衡系数 Klb
extern LONG* 	RS_pnKlc; 		 //低压侧C相平衡系数 Klc
extern LONG* 	RS_pnK87AR; 		 //差流越限报警比率 K87AR
extern LONG* 	RS_pn50IH1C; 		 //高压侧过流Ⅰ段电流 50IH1C
extern LONG* 	RS_pn50IH1D; 		 //高压侧过流Ⅰ段时间 50IH1D
extern LONG* 	RS_pn50IH2C; 		 //高压侧过流Ⅱ段电流 50IH2C
extern LONG* 	RS_pn50IH2D; 		 //高压侧过流Ⅱ段时间 50IH2D
extern LONG* 	RS_pn47U_27U1; 		 //高过流闭锁低电压 47U_27U1
extern LONG* 	RS_pn47U_59U2; 		 //高过流闭锁负序电压 47U_59U2
extern LONG* 	RS_pnA_50IH; 		 //高压侧过流灵敏角 A_50IH
extern LONG* 	RS_pn50IM1C; 		 //中压侧过流Ⅰ段电流 50IM1C
extern LONG* 	RS_pn50IM1D; 		 //中压侧过流Ⅰ段时间 50IM1D
extern LONG* 	RS_pn50IM2C; 		 //中压侧过流Ⅱ段电流 50IM2C
extern LONG* 	RS_pn50IM2D; 		 //中压侧过流Ⅱ段时间 50IM2D
extern LONG* 	RS_pn50IL1C; 		 //低压侧过流Ⅰ段电流 50IL1C
extern LONG* 	RS_pn50IL1D; 		 //低压侧过流Ⅰ段时间 50IL1D
extern LONG* 	RS_pn50IL2C; 		 //低压侧过流Ⅱ段电流 50IL2C
extern LONG* 	RS_pn50IL2D; 		 //低压侧过流Ⅱ段时间 50IL2D
extern LONG* 	RS_pn50OL1C; 		 //过负荷Ⅰ段电流 50OL1C
extern LONG* 	RS_pn50OL1D; 		 //过负荷Ⅰ段时间 50OL1D
extern LONG* 	RS_pn50OL2C; 		 //过负荷Ⅱ段电流 50OL2C
extern LONG* 	RS_pn50OL2D; 		 //过负荷Ⅱ段时间 50OL2D
extern LONG* 	RS_pn50OL3C; 		 //过负荷Ⅲ段电流 50OL3C
extern LONG* 	RS_pn50OL3D; 		 //过负荷Ⅲ段时间 50OL3D
extern LONG* 	RS_pn27UV; 		 //失压保护相电压 27UV
extern LONG* 	RS_pn27UD; 		 //失压保护时间 27UD
extern LONG* 	RS_pn27U_50I; 		 //失压保护电流闭锁 27U_50I
extern LONG* 	RS_pn59UNV; 		 //零序过压电压定值 59UNV
extern LONG* 	RS_pn59UND; 		 //零序过压时间定值 59UND
extern LONG* 	RS_pn50INC; 		 //零序过流电流定值 50INC
extern LONG* 	RS_pn50IND; 		 //零序过流时间定值 50IND
extern LONG* 	RS_pn50IJXC; 		 //间隙过流电流定值 50IJXC
extern LONG* 	RS_pn50IJXD; 		 //间隙过流时间定值 50IJXD
extern LONG* 	RS_pn74VTV; 		 //PT断线检测电压 74VTV
extern LONG* 	RS_pn74VTC; 		 //PT断线检测电流 74VTC
extern LONG* 	RS_pn74VTD; 		 //PT断线检测时间 74VTD
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD

#define RS_COUNT     71    //定值个数/
#define RS_BUF_SIZE     284    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 328)

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
extern LONG* 	AS_pn59UA; 		 //UA有压定值
extern LONG* 	AS_pn27UA; 		 //UA无压定值
extern LONG* 	AS_pn59UB; 		 //UB有压定值
extern LONG* 	AS_pn27UB; 		 //UB无压定值
extern LONG* 	AS_pn59UC; 		 //UC有压定值
extern LONG* 	AS_pn27UC; 		 //UC无压定值
extern LONG* 	AS_pn59UN; 		 //UN有压定值
extern LONG* 	AS_pn27UN; 		 //UN无压定值
extern LONG* 	AS_pn50HAC; 		 //IHA有流定值
extern LONG* 	AS_pn37HAC; 		 //IHA无流定值
extern LONG* 	AS_pn50HBC; 		 //IHB有流定值
extern LONG* 	AS_pn37HBC; 		 //IHB无流定值
extern LONG* 	AS_pn50HCC; 		 //IHC有流定值
extern LONG* 	AS_pn37HCC; 		 //IHC无流定值
extern LONG* 	AS_pn50MAC; 		 //IMA有流定值
extern LONG* 	AS_pn37MAC; 		 //IMA无流定值
extern LONG* 	AS_pn50MBC; 		 //IMB有流定值
extern LONG* 	AS_pn37MBC; 		 //IMB无流定值
extern LONG* 	AS_pn50MCC; 		 //IMC有流定值
extern LONG* 	AS_pn37MCC; 		 //IMC无流定值
extern LONG* 	AS_pn50LAC; 		 //ILA有流定值
extern LONG* 	AS_pn37LAC; 		 //ILA无流定值
extern LONG* 	AS_pn50LBC; 		 //ILB有流定值
extern LONG* 	AS_pn37LBC; 		 //ILB无流定值
extern LONG* 	AS_pn50LCC; 		 //ILC有流定值
extern LONG* 	AS_pn37LCC; 		 //ILC无流定值
extern LONG* 	AS_pn50INC; 		 //IN有流定值
extern LONG* 	AS_pn37INC; 		 //IN无流定值
extern LONG* 	AS_pn50IJXC; 		 //IJX有流定值
extern LONG* 	AS_pn37IJXC; 		 //IJX无流定值
extern LONG* 	AS_pnTCB; 		 //断路器动作延时
extern LONG* 	AS_pnTSW; 		 //隔开动作延时
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnTR3D; 		 //TR3保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnHPT; 		 //高压侧PT变比
extern LONG* 	AS_pnNPT; 		 //零序PT变比
extern LONG* 	AS_pnHACT; 		 //高压侧A相CT变比
extern LONG* 	AS_pnHBCT; 		 //高压侧B相CT变比
extern LONG* 	AS_pnHCCT; 		 //高压侧C相CT变比
extern LONG* 	AS_pnMACT; 		 //中压测A相CT变比
extern LONG* 	AS_pnMBCT; 		 //中压测B相CT变比
extern LONG* 	AS_pnMCCT; 		 //中压测C相CT变比
extern LONG* 	AS_pnLACT; 		 //低压侧A相CT变比
extern LONG* 	AS_pnLBCT; 		 //低压侧B相CT变比
extern LONG* 	AS_pnLCCT; 		 //低压侧C相CT变比
extern LONG* 	AS_pnNCT; 		 //零序CT变比
extern LONG* 	AS_pnJXCT; 		 //间隙CT变比
extern LONG* 	AS_pnRT; 		 //固定变比

#define AS_RT_NUM     14    //辅助定值中变比个数

#define AS_COUNT     83    //辅助定值个数
#define AS_BUF_SIZE     332    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 612)

//系数----------------------------------
extern LONG* 	CF_pnUp; 		 //Up
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnIp; 		 //Ip
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnIjx; 		 //Ijx
extern LONG* 	CF_pnUAM; 		 //UAM
extern LONG* 	CF_pnUBM; 		 //UBM
extern LONG* 	CF_pnUCM; 		 //UCM
extern LONG* 	CF_pnU0M; 		 //U0M
extern LONG* 	CF_pnIHAM; 		 //IHAM
extern LONG* 	CF_pnIHBM; 		 //IHBM
extern LONG* 	CF_pnIHCM; 		 //IHCM
extern LONG* 	CF_pnIMAM; 		 //IMAM
extern LONG* 	CF_pnIMBM; 		 //IMBM
extern LONG* 	CF_pnIMCM; 		 //IMCM
extern LONG* 	CF_pnILAM; 		 //ILAM
extern LONG* 	CF_pnILBM; 		 //ILBM
extern LONG* 	CF_pnILCM; 		 //ILCM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnIjxM; 		 //IjxM
extern LONG* 	CF_pnUAD; 		 //UAD
extern LONG* 	CF_pnUBD; 		 //UBD
extern LONG* 	CF_pnUCD; 		 //UCD
extern LONG* 	CF_pnU0D; 		 //U0D
extern LONG* 	CF_pnIHAD; 		 //IHAD
extern LONG* 	CF_pnIHBD; 		 //IHBD
extern LONG* 	CF_pnIHCD; 		 //IHCD
extern LONG* 	CF_pnIMAD; 		 //IMAD
extern LONG* 	CF_pnIMBD; 		 //IMBD
extern LONG* 	CF_pnIMCD; 		 //IMCD
extern LONG* 	CF_pnILAD; 		 //ILAD
extern LONG* 	CF_pnILBD; 		 //ILBD
extern LONG* 	CF_pnILCD; 		 //ILCD
extern LONG* 	CF_pnI0D; 		 //I0D
extern LONG* 	CF_pnIjxD; 		 //IjxD
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4

#define CF_COUNT     39    //系数个数
#define CF_BUF_SIZE     156    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 944)

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
#define R_RELDATAERR               14    //保护数据异常
#define R_ADERR_R                  15    //AD故障恢复
#define R_SLERR_R                  16    //软压板故障恢复
#define R_EEPROMERR_R              17    //EEPROM定值故障恢复
#define R_RAMERR_R                 18    //RAM定值故障恢复
#define R_ROMERR_R                 19    //程序ROM故障恢复
#define R_COMERR_R                 20    //通信通道异常恢复
#define R_NVRAMERR_R               21    //NVRAM内容出错恢复
#define R_LOGICERR_R               22    //逻辑方程故障故障恢复
#define R_POWERERR_R               23    //5V电源故障恢复
#define R_RELDATAERR_R             24    //保护数据异常恢复
#define R_ACTPICK                  25    //保护启动
#define R_RTR_GAS                  26    //重瓦斯
#define R_RTR_YLSF                 27    //压力释放
#define R_RTR_23                   28    //超温跳闸
#define R_R74_GAS                  29    //轻瓦斯
#define R_R74_OIL                  30    //油位异常
#define R_R74_23                   31    //温度过高
#define R_AR87R                    32    //差流越限
#define R_R87UA_PI                 33    //A相差动速断启动
#define R_R87UA                    34    //A相差流速断动作
#define R_R87UA_RES                35    //A相差动速断返回
#define R_R87UB_PI                 36    //B相差动速断启动
#define R_R87UB                    37    //B相差动速断动作
#define R_R87UB_RES                38    //B相差动速断返回
#define R_R87UC_PI                 39    //C相差动速断启动
#define R_R87UC                    40    //C相差动速断动作
#define R_R87UC_RES                41    //C相差动速断返回
#define R_R87RA_PI                 42    //A相比率差动启动
#define R_R87RA                    43    //A相比率差动动作
#define R_R87RA_RES                44    //A相比率差动返回
#define R_R87RB_PI                 45    //B相比率差动启动
#define R_R87RB                    46    //B相比率差动动作
#define R_R87RB_RES                47    //B相比率差动返回
#define R_R87RC_PI                 48    //C相比率差动启动
#define R_R87RC                    49    //C相比率差动动作
#define R_R87RC_RES                50    //C相比率差动返回
#define R_R50IH1_PI                51    //高压侧过流Ⅰ段启动
#define R_R50IH1                   52    //高压侧过流Ⅰ段动作
#define R_R50IH1_RES               53    //高压侧过流Ⅰ段返回
#define R_R50IH2_PI                54    //高压侧过流Ⅱ段启动
#define R_R50IH2                   55    //高压侧过流Ⅱ段动作
#define R_R50IH2_RES               56    //高压侧过流Ⅱ段返回
#define R_R50IM1_PI                57    //中压侧过流Ⅰ段启动
#define R_R50IM1                   58    //中压侧过流Ⅰ段动作
#define R_R50IM1_RES               59    //中压侧过流Ⅰ段返回
#define R_R50IM2_PI                60    //中压侧过流Ⅱ段启动
#define R_R50IM2                   61    //中压侧过流Ⅱ段动作
#define R_R50IM2_RES               62    //中压侧过流Ⅱ段返回
#define R_R50IL1_PI                63    //低压侧过流Ⅰ段启动
#define R_R50IL1                   64    //低压侧过流Ⅰ段动作
#define R_R50IL1_RES               65    //低压侧过流Ⅰ段返回
#define R_R50IL2_PI                66    //低压侧过流Ⅱ段启动
#define R_R50IL2                   67    //低压侧过流Ⅱ段动作
#define R_R50IL2_RES               68    //低压侧过流Ⅱ段返回
#define R_R50OL1_PI                69    //过负荷Ⅰ段启动
#define R_R50OL1                   70    //过负荷Ⅰ段动作
#define R_R50OL1_RES               71    //过负荷Ⅰ段返回
#define R_R50OL2_PI                72    //过负荷Ⅱ段启动
#define R_R50OL2                   73    //过负荷Ⅱ段动作
#define R_R50OL2_RES               74    //过负荷Ⅱ段返回
#define R_R50OL3_PI                75    //过负荷Ⅲ段启动
#define R_R50OL3                   76    //过负荷Ⅲ段动作
#define R_R50OL3_RES               77    //过负荷Ⅲ段返回
#define R_R27U                     78    //失压保护动作
#define R_R74VT                    79    //高压侧PT断线
#define R_COMTEST                  80    //通道测试
#define R_PICKUP                   81    //保护启动
#define R_52BF                     82    //失灵保护
#define R_TRFAIL_H                 83    //高侧断路器跳闸失败
#define R_TRFAIL_M                 84    //中侧断路器跳闸失败
#define R_TRFAIL_L                 85    //低侧断路器跳闸失败
#define R_R59UN_PI                 86    //零序过压保护启动
#define R_R59UN                    87    //零序过压保护动作
#define R_R59UN_RES                88    //零序过压保护返回
#define R_R50IN_PI                 89    //零序过流保护启动
#define R_R50IN                    90    //零序过流保护动作
#define R_R50IN_RES                91    //零序过流保护返回
#define R_R50IJX_PI                92    //间隙过流保护启动
#define R_R50IJX                   93    //间隙过流保护动作
#define R_R50IJX_RES               94    //间隙过流保护返回
#define MAX_REPORT_INDEX           94    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     21    //传动个数

#endif