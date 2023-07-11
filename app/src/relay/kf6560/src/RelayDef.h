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

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----供电方式-----
#define 	BTMODE 	0	//直供供电
#define 	ATMODE 	1	//AT供电

//保护测量值定义-----------------------
#define  RM_UP 0
extern LONG* 	RM_pnUp; 		 // U
#define  RM_U1 1
extern LONG* 	RM_pnU1; 		 // U1
#define  RM_U2 2
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_UD1 3
extern LONG* 	RM_pnUd1; 		 // Ud1
#define  RM_UD2 4
extern LONG* 	RM_pnUd2; 		 // Ud2
#define  RM_UD3 5
extern LONG* 	RM_pnUd3; 		 // Ud3
#define  RM_IP 6
extern LONG* 	RM_pnIp; 		 // I
#define  RM_IM 7
extern LONG* 	RM_pnIm; 		 // Im
#define  RM_I1 8
extern LONG* 	RM_pnI1; 		 // I1
#define  RM_I2 9
extern LONG* 	RM_pnI2; 		 // I2
#define  RM_I3 10
extern LONG* 	RM_pnI3; 		 // I3
#define  RM_IH3 11
extern LONG* 	RM_pnIh3; 		 // Ih3
#define  RM_IH5 12
extern LONG* 	RM_pnIh5; 		 // Ih5
#define  RM_IH7 13
extern LONG* 	RM_pnIh7; 		 // Ih7
#define  RM_BOBRCH1 14
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 15
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 16
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 17
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     18    //保护测量值个数
#define RM_BUF_SIZE     72    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_UM 0
extern LONG* 	M_pnUM; 		 //U U
#define  M_U1M 1
extern LONG* 	M_pnU1M; 		 //U1 U1
#define  M_U2M 2
extern LONG* 	M_pnU2M; 		 //U2 U2
#define  M_UD1M 3
extern LONG* 	M_pnUd1M; 		 //Ud1 Ud1
#define  M_UD2M 4
extern LONG* 	M_pnUd2M; 		 //Ud2 Ud2
#define  M_UD3M 5
extern LONG* 	M_pnUd3M; 		 //Ud3 Ud3
#define  M_IM 6
extern LONG* 	M_pnIM; 		 //I I
#define  M_I1M 7
extern LONG* 	M_pnI1M; 		 //I1 I1
#define  M_I2M 8
extern LONG* 	M_pnI2M; 		 //I2 I2
#define  M_I3M 9
extern LONG* 	M_pnI3M; 		 //I3 I3
#define  M_P 10
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 11
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 12
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_IH1 13
extern LONG* 	M_pnIH1; 		 //IH1 IH1
#define  M_IH2 14
extern LONG* 	M_pnIH2; 		 //IH2 IH2
#define  M_IH3 15
extern LONG* 	M_pnIH3; 		 //IH3 IH3
#define  M_IH5 16
extern LONG* 	M_pnIH5; 		 //IH5 IH5
#define  M_IH7 17
extern LONG* 	M_pnIH7; 		 //IH7 IH7
#define  M_IH9 18
extern LONG* 	M_pnIH9; 		 //IH9 IH9
#define  M_IH11 19
extern LONG* 	M_pnIH11; 		 //IH11 IH11

#define MEASURE_COUNT     20    //测量值个数
#define MEASURE_BUF_SIZE     80    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 76)

//软压板----------------------------------
extern BYTE* 	SW_pby50_0; 		 //电流速断
extern BYTE* 	SW_pby50_1; 		 //过流保护
extern BYTE* 	SW_pby59; 		 //过压保护
extern BYTE* 	SW_pby27; 		 //低压保护
extern BYTE* 	SW_pby50_H; 		 //谐波过流保护
extern BYTE* 	SW_pby50ST; 		 //支路过流保护
extern BYTE* 	SW_pby60I; 		 //差流保护
extern BYTE* 	SW_pby60U; 		 //差压保护
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyBAK1; 		 //备用压板1
extern BYTE* 	SW_pbyBAK2; 		 //备用压板2

#define SW_COUNT     11    //软压板个数
#define SW_BUF_SIZE     11    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 156)

//配置----------------------------------
extern BYTE* 	CFG_pby50_0; 		 //速断保护
extern BYTE* 	CFG_pby50_1; 		 //过流保护
extern BYTE* 	CFG_pby50_H; 		 //谐波过流保护
extern BYTE* 	CFG_pby59; 		 //过压保护
extern BYTE* 	CFG_pby27; 		 //低压保护
extern BYTE* 	CFG_pby60I1; 		 //支路1差流
extern BYTE* 	CFG_pby60I2; 		 //支路2差流
extern BYTE* 	CFG_pby50ST; 		 //支路过流
extern BYTE* 	CFG_pby60U1; 		 //支路1差压
extern BYTE* 	CFG_pby60U2; 		 //支路2差压
extern BYTE* 	CFG_pby60U3; 		 //支路3差压
extern BYTE* 	CFG_pby74PT; 		 //PT断线检查
extern BYTE* 	CFG_pbyCTBR; 		 //CT断线检查
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyBAK1; 		 //备用配置1
extern BYTE* 	CFG_pbyBAK2; 		 //备用配置2

#define CFG_COUNT     16    //配置个数/
#define CFG_BUF_SIZE     16    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 170)

//定值----------------------------------
extern LONG* 	RS_pnF; 		 //配置字 F
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn50_0C; 		 //速断电流定值 50_0C
extern LONG* 	RS_pn50_0D; 		 //速断时间定值 50_0D
extern LONG* 	RS_pn50_1C; 		 //过流电流定值 50_1C
extern LONG* 	RS_pn50_1D; 		 //过流时间定值 50_1D
extern LONG* 	RS_pn50_HC; 		 //谐波过流电流定值 50_HC
extern LONG* 	RS_pn50_HD; 		 //谐波过流时间定值 50_HD
extern LONG* 	RS_pn59V; 		 //过压保护电压 59V
extern LONG* 	RS_pn59D; 		 //过压保护时间 59D
extern LONG* 	RS_pn27V; 		 //低压保护电压 27V
extern LONG* 	RS_pn27D; 		 //低压保护时间 27D
extern LONG* 	RS_pn27_LK50; 		 //低压保护电流闭锁 27_LK50
extern LONG* 	RS_pn60I1C; 		 //支路1差流保护电流 60I1C
extern LONG* 	RS_pn60I1D; 		 //支路1差流保护时间 60I1D
extern LONG* 	RS_pn60I2C; 		 //支路2差流保护电流 60I2C
extern LONG* 	RS_pn60I2D; 		 //支路2差流保护时间 60I2D
extern LONG* 	RS_pn50STC; 		 //支路过流保护电流 50STC
extern LONG* 	RS_pn50STD; 		 //支路过流保护时间 50STD
extern LONG* 	RS_pn60U1V; 		 //支路1差压保护电压 60U1V
extern LONG* 	RS_pn60U1D; 		 //支路1差压保护时间 60U1D
extern LONG* 	RS_pn60U2V; 		 //支路2差压保护电压 60U2V
extern LONG* 	RS_pn60U2D; 		 //支路2差压保护时间 60U2D
extern LONG* 	RS_pn60U3V; 		 //支路3差压保护电压 60U3V
extern LONG* 	RS_pn60U3D; 		 //支路3差压保护时间 60U3D
extern LONG* 	RS_pn74PTC; 		 //PT断线检测电流 74PTC
extern LONG* 	RS_pn74PTV; 		 //PT断线检测电压 74PTV
extern LONG* 	RS_pn74PTD; 		 //PT断线检测时间 74PTD
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD

#define RS_COUNT     29    //定值个数/
#define RS_BUF_SIZE     116    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 186)

//辅助定值----------------------------------
extern LONG* 	AS_pnRAC; 		 //额定电流
extern LONG* 	AS_pnPM; 		 //供电方式
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
extern LONG* 	AS_pnTSW; 		 //隔开动作延时
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnTR3D; 		 //TR3保持时间
extern LONG* 	AS_pn50IP; 		 //IP有流定值
extern LONG* 	AS_pn37IP; 		 //IP无流定值
extern LONG* 	AS_pn50I1; 		 //I1有流定值
extern LONG* 	AS_pn37I1; 		 //I1无流定值
extern LONG* 	AS_pn50I2; 		 //I2有流定值
extern LONG* 	AS_pn37I2; 		 //I2无流定值
extern LONG* 	AS_pn50I3; 		 //I3有流定值
extern LONG* 	AS_pn37I3; 		 //I3无流定值
extern LONG* 	AS_pn59UP; 		 //Up有压定值
extern LONG* 	AS_pn27UP; 		 //Up无压定值
extern LONG* 	AS_pn59U1; 		 //U1有压定值
extern LONG* 	AS_pn27U1; 		 //U1无压定值
extern LONG* 	AS_pn59U2; 		 //U2有压定值
extern LONG* 	AS_pn27U2; 		 //U2无压定值
extern LONG* 	AS_pn59Ud1; 		 //Ud1有压定值
extern LONG* 	AS_pn27Ud1; 		 //Ud1无压定值
extern LONG* 	AS_pn59Ud2; 		 //Ud2有压定值
extern LONG* 	AS_pn27Ud2; 		 //Ud2无压定值
extern LONG* 	AS_pn59Ud3; 		 //Ud3有压定值
extern LONG* 	AS_pn27Ud3; 		 //Ud3无压定值
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnU1PTRatio; 		 //U1PT变比
extern LONG* 	AS_pnU2PTRatio; 		 //U2PT变比
extern LONG* 	AS_pnUd1PTRatio; 		 //Ud1PT变比
extern LONG* 	AS_pnUd2PTRatio; 		 //Ud2PT变比
extern LONG* 	AS_pnUd3PTRatio; 		 //Ud3PT变比
extern LONG* 	AS_pnIPCTRatio; 		 //保护CT变比
extern LONG* 	AS_pnIMCTRatio; 		 //测量CT变比
extern LONG* 	AS_pnId1CTRatio; 		 //Id1CT变比
extern LONG* 	AS_pnId2CTRatio; 		 //Id2CT变比
extern LONG* 	AS_pnId3CTRatio; 		 //Id3CT变比
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     11    //辅助定值中变比个数

#define AS_COUNT     71    //辅助定值个数
#define AS_BUF_SIZE     284    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 302)

//系数----------------------------------
extern LONG* 	CF_pnUP; 		 //UP
extern LONG* 	CF_pnU1P; 		 //U1P
extern LONG* 	CF_pnU2P; 		 //U2P
extern LONG* 	CF_pnUd1P; 		 //Ud1P
extern LONG* 	CF_pnUd2P; 		 //Ud2P
extern LONG* 	CF_pnUd3P; 		 //Ud3P
extern LONG* 	CF_pnIP; 		 //IP
extern LONG* 	CF_pnI1P; 		 //I1P
extern LONG* 	CF_pnI2P; 		 //I2P
extern LONG* 	CF_pnI3P; 		 //I3P
extern LONG* 	CF_pnUM; 		 //UM
extern LONG* 	CF_pnU1M; 		 //U1M
extern LONG* 	CF_pnU2M; 		 //U2M
extern LONG* 	CF_pnUd1M; 		 //Ud1M
extern LONG* 	CF_pnUd2M; 		 //Ud2M
extern LONG* 	CF_pnUd3M; 		 //Ud3M
extern LONG* 	CF_pnIM; 		 //IM
extern LONG* 	CF_pnI1M; 		 //I1M
extern LONG* 	CF_pnI2M; 		 //I2M
extern LONG* 	CF_pnI3M; 		 //I3M
extern LONG* 	CF_pnP; 		 //P
extern LONG* 	CF_pnQ; 		 //Q
extern LONG* 	CF_pnA; 		 //A
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnDRIFTU; 		 //DRIFTU
extern LONG* 	CF_pnDRIFTU1; 		 //DRIFTU1
extern LONG* 	CF_pnDRIFTU2; 		 //DRIFTU2
extern LONG* 	CF_pnDRIFTUD1; 		 //DRIFTUD1
extern LONG* 	CF_pnDRIFTUD2; 		 //DRIFTUD2
extern LONG* 	CF_pnDRIFTUD3; 		 //DRIFTUD3
extern LONG* 	CF_pnDRIFTIM; 		 //DRIFTIM
extern LONG* 	CF_pnDRIFTI1; 		 //DRIFTI1
extern LONG* 	CF_pnDRIFTI2; 		 //DRIFTI2
extern LONG* 	CF_pnDRIFTI3; 		 //DRIFTI3

#define CF_COUNT     37    //系数个数
#define CF_BUF_SIZE     148    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 586)

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
#define R_OUTRAMERR                 9    //外部RAM故障
#define R_NVRAMERR                 10    //NVRAM内容出错
#define R_LOGICERR                 11    //逻辑方程故障
#define R_RCERR                    12    //保护自检错误
#define R_ADERR_R                  13    //AD故障恢复
#define R_SLERR_R                  14    //软压板故障恢复
#define R_EEPROMERR_R              15    //EEPROM定值故障恢复
#define R_RAMERR_R                 16    //RAM定值故障恢复
#define R_ROMERR_R                 17    //程序ROM故障恢复
#define R_COMERR_R                 18    //通信通道异常恢复
#define R_NVRAMERR_R               19    //NVRAM内容出错恢复
#define R_LOGICERR_R               20    //逻辑方程故障故障恢复
#define R_ACTPICK                  21    //保护启动
#define R_R59U_PI                  22    //过压保护启动
#define R_R59U                     23    //过压保护动作
#define R_R59U_RES                 24    //过压保护返回
#define R_R50IH_PI                 25    //谐波过流保护启动
#define R_R50IH                    26    //谐波过流保护动作
#define R_R50IH_RES                27    //谐波过流保护返回
#define R_R60I1_PI                 28    //支路1差流保护启动
#define R_R60I1                    29    //支路1差流保护动作
#define R_R60I1_RES                30    //支路1差流保护返回
#define R_R60I2_PI                 31    //支路2差流保护启动
#define R_R60I2                    32    //支路2差流保护动作
#define R_R60I2_RES                33    //支路2差流保护返回
#define R_R50ST_PI                 34    //支路过流保护启动
#define R_R50ST                    35    //支路过流保护动作
#define R_R50ST_RES                36    //支路过流保护返回
#define R_R60U1_PI                 37    //支路1差压保护启动
#define R_R60U1                    38    //支路1差压保护动作
#define R_R60U1_RES                39    //支路1差压保护返回
#define R_R60U2_PI                 40    //支路2差压保护启动
#define R_R60U2                    41    //支路2差压保护动作
#define R_R60U2_RES                42    //支路2差压保护返回
#define R_R60U3_PI                 43    //支路3差压保护启动
#define R_R60U3                    44    //支路3差压保护动作
#define R_R60U3_RES                45    //支路3差压保护返回
#define R_R50I0_PI                 46    //电流速断启动
#define R_R50I0                    47    //电流速断动作
#define R_R50I0_RES                48    //电流速断返回
#define R_R50I1_PI                 49    //过流保护启动
#define R_R50I1                    50    //过流保护动作
#define R_R50I1_RES                51    //过流保护返回
#define R_R27U                     52    //低压保护动作
#define R_PTBR                     53    //PT断线
#define R_PTBR_RES                 54    //PT断线恢复
#define R_CTBR                     55    //CT断线
#define R_CTBR_RES                 56    //CT断线恢复
#define R_TRFAIL                   57    //断路器跳闸失败
#define R_COMTEST                  58    //通道测试
#define R_PICKUP                   59    //保护启动
#define R_POWERERR                 60    //5V电源故障
#define R_EEPROM_W_ERR             61    //EEPROM写入故障
#define R_POWERERR_R               62    //5V电源故障恢复
#define R_RELDATAERR               63    //保护数据异常
#define R_RELDATAERR_R             64    //保护数据异常恢复
#define R_ZWS                      65    //重瓦斯
#define R_YLSF                     66    //压力释放
#define R_CWTZ                     67    //超温跳闸
#define R_QWS                      68    //轻瓦斯
#define R_YWYC                     69    //油位异常
#define R_WDGG                     70    //温度过高
#define MAX_REPORT_INDEX           70    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     7    //传动个数

#endif