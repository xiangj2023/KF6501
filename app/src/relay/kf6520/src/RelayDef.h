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
//----公里标方向-----
#define 	FOREWORD 	0	//正方向
#define 	BACKWORD 	1	//反方向

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----供电方式-----
#define 	BTMODE 	0	//直供
#define 	AT1MODE 	1	//AT
#define 	AT2MODE 	2	//客专

//保护测量值定义-----------------------
#define  RM_IP 0
extern LONG* 	RM_pnIp; 		 // Ip
#define  RM_IT 1
extern LONG* 	RM_pnIt; 		 // It
#define  RM_IF 2
extern LONG* 	RM_pnIf; 		 // If
#define  RM_UP 3
extern LONG* 	RM_pnUp; 		 // Up
#define  RM_U1 4
extern LONG* 	RM_pnU1; 		 // U1
#define  RM_U2 5
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_U3 6
extern LONG* 	RM_pnU3; 		 // U3
#define  RM_U4 7
extern LONG* 	RM_pnU4; 		 // U4
#define  RM_TL 8
extern LONG* 	RM_pnTL; 		 // TL
#define  RM_FL 9
extern LONG* 	RM_pnFL; 		 // FL
#define  RM_SS 10
extern LONG* 	RM_pnSS; 		 // SS
#define  RM_I2 11
extern LONG* 	RM_pnI2; 		 // I2
#define  RM_I3 12
extern LONG* 	RM_pnI3; 		 // I3
#define  RM_I5 13
extern LONG* 	RM_pnI5; 		 // I5
#define  RM_ITM 14
extern LONG* 	RM_pnItm; 		 // Itm
#define  RM_IFM 15
extern LONG* 	RM_pnIfm; 		 // Ifm
#define  RM_R 16
extern LONG* 	RM_pnR; 		 // R
#define  RM_X 17
extern LONG* 	RM_pnX; 		 // X
#define  RM_ZA 18
extern LONG* 	RM_pnZA; 		 // ΦZ
#define  RM_TR 19
extern LONG* 	RM_pnTR; 		 // TR
#define  RM_TX 20
extern LONG* 	RM_pnTX; 		 // TX
#define  RM_TZA 21
extern LONG* 	RM_pnTZA; 		 // ΦT
#define  RM_FR 22
extern LONG* 	RM_pnFR; 		 // FR
#define  RM_FX 23
extern LONG* 	RM_pnFX; 		 // FX
#define  RM_FZA 24
extern LONG* 	RM_pnFZA; 		 // ΦF
#define  RM_AUP 25
extern LONG* 	RM_pnAUP; 		 // ΦUP
#define  RM_AU1 26
extern LONG* 	RM_pnAU1; 		 // ΦU1
#define  RM_AU2 27
extern LONG* 	RM_pnAU2; 		 // ΦU2
#define  RM_AIP 28
extern LONG* 	RM_pnAIP; 		 // ΦIP
#define  RM_AIT 29
extern LONG* 	RM_pnAIT; 		 // ΦIT
#define  RM_AIF 30
extern LONG* 	RM_pnAIF; 		 // ΦIF
#define  RM_AITM 31
extern LONG* 	RM_pnAITM; 		 // ΦItM
#define  RM_AIFM 32
extern LONG* 	RM_pnAIFM; 		 // ΦIfM
#define  RM_BOBRCH1 33
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 34
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 35
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 36
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     37    //保护测量值个数
#define RM_BUF_SIZE     148    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------
#define  M_U1 0
extern LONG* 	M_pnU1; 		 //U1 U1
#define  M_U2 1
extern LONG* 	M_pnU2; 		 //U2 U2
#define  M_U3 2
extern LONG* 	M_pnU3; 		 //U3 U3
#define  M_U4 3
extern LONG* 	M_pnU4; 		 //U4 U4
#define  M_IT 4
extern LONG* 	M_pnIT; 		 //IT IT
#define  M_IF 5
extern LONG* 	M_pnIF; 		 //IF IF
#define  M_P 6
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 7
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 8
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_I1 9
extern LONG* 	M_pnI1; 		 //I1 I1
#define  M_I2 10
extern LONG* 	M_pnI2; 		 //I2 I2
#define  M_I3 11
extern LONG* 	M_pnI3; 		 //I3 I3
#define  M_I5 12
extern LONG* 	M_pnI5; 		 //I5 I5
#define  M_I7 13
extern LONG* 	M_pnI7; 		 //I7 I7
#define  M_I9 14
extern LONG* 	M_pnI9; 		 //I9 I9
#define  M_I11 15
extern LONG* 	M_pnI11; 		 //I11 I11
#define  M_R 16
extern LONG* 	M_pnR; 		 //R R
#define  M_X 17
extern LONG* 	M_pnX; 		 //X X
#define  M_F 18
extern LONG* 	M_pnF; 		 //F F

#define MEASURE_COUNT     19    //测量值个数
#define MEASURE_BUF_SIZE     76    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 152)

//软压板----------------------------------
extern BYTE* 	SW_pby21; 		 //距离保护
extern BYTE* 	SW_pby50_0; 		 //电流速断
extern BYTE* 	SW_pby50_123; 		 //过流保护
extern BYTE* 	SW_pby50D; 		 //电流增量保护
extern BYTE* 	SW_pby27; 		 //低压保护
extern BYTE* 	SW_pby79; 		 //重合闸
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyRESD1; 		 //备用压板1
extern BYTE* 	SW_pbyRESD2; 		 //备用压板2

#define SW_COUNT     9    //软压板个数
#define SW_BUF_SIZE     9    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 228)

//配置----------------------------------
extern BYTE* 	CFG_pby21_1; 		 //距离I段
extern BYTE* 	CFG_pby21_1_D; 		 //距离I段正向
extern BYTE* 	CFG_pby21_2; 		 //距离II段
extern BYTE* 	CFG_pby21_2_D; 		 //距离II段正向
extern BYTE* 	CFG_pby21_3; 		 //距离III段
extern BYTE* 	CFG_pby21_3_D; 		 //距离III段正向
extern BYTE* 	CFG_pby21_4; 		 //距离IV段
extern BYTE* 	CFG_pby21_4_D; 		 //距离IV段正向
extern BYTE* 	CFG_pbyParallelC; 		 //平行四边形特性
extern BYTE* 	CFG_pby21_HLK; 		 //距离谐波闭锁
extern BYTE* 	CFG_pby21_HRES; 		 //距离谐波抑制
extern BYTE* 	CFG_pby50_0; 		 //电流速断
extern BYTE* 	CFG_pby50_0_27; 		 //速断低压闭锁
extern BYTE* 	CFG_pby50_0_HLK; 		 //速断谐波闭锁
extern BYTE* 	CFG_pby50_0_HRES; 		 //速断谐波抑制
extern BYTE* 	CFG_pby50_0_D; 		 //速断保护正方向
extern BYTE* 	CFG_pby50_0_67; 		 //速断功率方向
extern BYTE* 	CFG_pby50_1; 		 //过流I段
extern BYTE* 	CFG_pby50_1_D; 		 //过流I段正方向
extern BYTE* 	CFG_pby50_1_67; 		 //过流I段功率方向
extern BYTE* 	CFG_pby50_1_27; 		 //过流I段低压闭锁
extern BYTE* 	CFG_pby50_2; 		 //过流II段
extern BYTE* 	CFG_pby50_2_D; 		 //过流II段正方向
extern BYTE* 	CFG_pby50_2_67; 		 //过流II段功率方向
extern BYTE* 	CFG_pby50_2_27; 		 //过流II段低压闭锁
extern BYTE* 	CFG_pby50_3; 		 //过流III段
extern BYTE* 	CFG_pby50_HLK; 		 //过流谐波闭锁
extern BYTE* 	CFG_pby50_HRES; 		 //过流谐波抑制
extern BYTE* 	CFG_pby51; 		 //反时限过流
extern BYTE* 	CFG_pby50D; 		 //电流增量保护
extern BYTE* 	CFG_pby79UC; 		 //偷跳重合闸
extern BYTE* 	CFG_pby79_21; 		 //距离保护重合
extern BYTE* 	CFG_pby79_50_0; 		 //电流速断重合
extern BYTE* 	CFG_pby79_50_123; 		 //过流保护重合
extern BYTE* 	CFG_pby79_51; 		 //反时限重合
extern BYTE* 	CFG_pby79_50D; 		 //增量保护重合
extern BYTE* 	CFG_pby79_59; 		 //重合闸检有压
extern BYTE* 	CFG_pby21_2_94; 		 //距离II段加速
extern BYTE* 	CFG_pby21_3_94; 		 //距离III段加速
extern BYTE* 	CFG_pby21_4_94; 		 //距离IV段加速
extern BYTE* 	CFG_pby50_1_94; 		 //过流I段加速
extern BYTE* 	CFG_pby50_2_94; 		 //过流II段加速
extern BYTE* 	CFG_pby50_3_94; 		 //过流III段加速
extern BYTE* 	CFG_pby27; 		 //低压保护
extern BYTE* 	CFG_pby52BF; 		 //失灵保护
extern BYTE* 	CFG_pbyPT1BR; 		 //PT1断线检查
extern BYTE* 	CFG_pbyPT2BR; 		 //PT2断线检查
extern BYTE* 	CFG_pby21_FL; 		 //故障测距
extern BYTE* 	CFG_pbyLOADREC; 		 //负荷录波
extern BYTE* 	CFG_pbyCTBR; 		 //CT断线检查
extern BYTE* 	CFG_pbyRESD1; 		 //备用配置1
extern BYTE* 	CFG_pbyRESD2; 		 //备用配置2

#define CFG_COUNT     52    //配置个数/
#define CFG_BUF_SIZE     52    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 238)

//定值----------------------------------
extern LONG* 	RS_pnFUNCTION1; 		 //配置字1 FUNCTION1
extern LONG* 	RS_pnFUNCTION2; 		 //配置字2 FUNCTION2
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn21_1R; 		 //距离I段电阻 21_1R
extern LONG* 	RS_pn21_1X; 		 //距离I段电抗 21_1X
extern LONG* 	RS_pn21_1D; 		 //距离I段时间 21_1D
extern LONG* 	RS_pn21_2R; 		 //距离II段电阻 21_2R
extern LONG* 	RS_pn21_2X; 		 //距离II段电抗 21_2X
extern LONG* 	RS_pn21_2D; 		 //距离II段时间 21_2D
extern LONG* 	RS_pn21_3R; 		 //距离III段电阻 21_3R
extern LONG* 	RS_pn21_3X; 		 //距离III段电抗 21_3X
extern LONG* 	RS_pn21_3D; 		 //距离III段时间 21_3D
extern LONG* 	RS_pn21_4R; 		 //距离IV段电阻 21_4R
extern LONG* 	RS_pn21_4X; 		 //距离IV段电抗 21_4X
extern LONG* 	RS_pn21_4D; 		 //距离IV段时间 21_4D
extern LONG* 	RS_pn21ZOFFSET; 		 //平行四边形偏移阻抗 21ZOFFSET
extern LONG* 	RS_pn21AOFFSET; 		 //阻抗四边形偏移角度 21AOFFSET
extern LONG* 	RS_pn50_0C; 		 //电流速断电流 50_0C
extern LONG* 	RS_pn50_0D; 		 //电流速断时间 50_0D
extern LONG* 	RS_pn50_0_27V; 		 //速断低压闭锁 50_0_27V
extern LONG* 	RS_pn50_1C; 		 //过流I段电流 50_1C
extern LONG* 	RS_pn50_1D; 		 //过流I段时间 50_1D
extern LONG* 	RS_pn50_1_27V; 		 //过流I段低压闭锁 50_1_27V
extern LONG* 	RS_pn50_2C; 		 //过流II段电流 50_2C
extern LONG* 	RS_pn50_2D; 		 //过流II段时间 50_2D
extern LONG* 	RS_pn50_2_27V; 		 //过流II段低压闭锁 50_2_27V
extern LONG* 	RS_pn50_3C; 		 //过流III段电流 50_3C
extern LONG* 	RS_pn50_3D; 		 //过流III段时间 50_3D
extern LONG* 	RS_pn51C; 		 //反时限过流电流 51C
extern LONG* 	RS_pn51TD; 		 //反时限过流时间 51TD
extern LONG* 	RS_pn51TYPE; 		 //反时限曲线类型 51TYPE
extern LONG* 	RS_pn50DC; 		 //电流增量保护电流 50DC
extern LONG* 	RS_pn50DD; 		 //电流增量保护时间 50DD
extern LONG* 	RS_pn50D_37C; 		 //增量保护小电流闭锁 50D_37C
extern LONG* 	RS_pnH2_PCT; 		 //二次谐波闭锁系数 H2_PCT
extern LONG* 	RS_pnFACTOR1; 		 //距离过流谐波加权 FACTOR1
extern LONG* 	RS_pnFACTOR2; 		 //增量保护谐波加权 FACTOR2
extern LONG* 	RS_pnZANGLE; 		 //线路阻抗角 ZANGLE
extern LONG* 	RS_pn79SH; 		 //重合闸投入次数 79SH
extern LONG* 	RS_pn79D1; 		 //第一次重合闸时间 79D1
extern LONG* 	RS_pn79D2; 		 //第二次重合闸时间 79D2
extern LONG* 	RS_pn79RD; 		 //重合闸充电时间 79RD
extern LONG* 	RS_pn79CLSD; 		 //重合闸动作监视时间 79CLSD
extern LONG* 	RS_pn94D; 		 //后加速动作时间 94D
extern LONG* 	RS_pnSOTFD; 		 //后加速投入时间 SOTFD
extern LONG* 	RS_pn27V; 		 //低压保护电压 27V
extern LONG* 	RS_pn27D; 		 //低压保护时间 27D
extern LONG* 	RS_pn27_37C; 		 //低压闭锁电流 27_37C
extern LONG* 	RS_pn52BFD; 		 //失灵保护时间 52BFD
extern LONG* 	RS_pn74VTV; 		 //PT断线检测电压 74VTV
extern LONG* 	RS_pn74VTBLC; 		 //PT断线闭锁电流 74VTBLC
extern LONG* 	RS_pn74VTC; 		 //PT断线检测电流 74VTC
extern LONG* 	RS_pn74VTD; 		 //PT断线检测时间 74VTD
extern LONG* 	RS_pnS0; 		 //公里标 S0
extern LONG* 	RS_pnSD; 		 //公里标方向 SD
extern LONG* 	RS_pnLc; 		 //修正距离 Lc
extern LONG* 	RS_pnT21FLN; 		 //T线测距分段数 T21FLN
extern LONG* 	RS_pnT21FLN1L; 		 //T线第一段距离 T21FLN1L
extern LONG* 	RS_pnT21FLN1X; 		 //T线第一段单位电抗 T21FLN1X
extern LONG* 	RS_pnT21FLN2L; 		 //T线第二段距离 T21FLN2L
extern LONG* 	RS_pnT21FLN2X; 		 //T线第二段单位电抗 T21FLN2X
extern LONG* 	RS_pnT21FLN3L; 		 //T线第三段距离 T21FLN3L
extern LONG* 	RS_pnT21FLN3X; 		 //T线第三段单位电抗 T21FLN3X
extern LONG* 	RS_pnT21FLN4L; 		 //T线第四段距离 T21FLN4L
extern LONG* 	RS_pnT21FLN4X; 		 //T线第四段单位电抗 T21FLN4X
extern LONG* 	RS_pnT21FLN5L; 		 //T线第五段距离 T21FLN5L
extern LONG* 	RS_pnT21FLN5X; 		 //T线第五段单位电抗 T21FLN5X
extern LONG* 	RS_pnT21FLN6L; 		 //T线第六段距离 T21FLN6L
extern LONG* 	RS_pnT21FLN6X; 		 //T线第六段单位电抗 T21FLN6X
extern LONG* 	RS_pnT21FLN7L; 		 //T线第七段距离 T21FLN7L
extern LONG* 	RS_pnT21FLN7X; 		 //T线第七段单位电抗 T21FLN7X
extern LONG* 	RS_pnT21FLN8L; 		 //T线第八段距离 T21FLN8L
extern LONG* 	RS_pnT21FLN8X; 		 //T线第八段单位电抗 T21FLN8X
extern LONG* 	RS_pnT21FLN9L; 		 //T线第九段距离 T21FLN9L
extern LONG* 	RS_pnT21FLN9X; 		 //T线第九段单位电抗 T21FLN9X
extern LONG* 	RS_pnT21FLN10L; 		 //T线第十段距离 T21FLN10L
extern LONG* 	RS_pnT21FLN10X; 		 //T线第十段单位电抗 T21FLN10X
extern LONG* 	RS_pnF21FLN; 		 //F线测距分段数 F21FLN
extern LONG* 	RS_pnF21FLN1L; 		 //F线第一段距离 F21FLN1L
extern LONG* 	RS_pnF21FLN1X; 		 //F线第一段单位电抗 F21FLN1X
extern LONG* 	RS_pnF21FLN2L; 		 //F线第二段距离 F21FLN2L
extern LONG* 	RS_pnF21FLN2X; 		 //F线第二段单位电抗 F21FLN2X
extern LONG* 	RS_pnF21FLN3L; 		 //F线第三段距离 F21FLN3L
extern LONG* 	RS_pnF21FLN3X; 		 //F线第三段单位电抗 F21FLN3X
extern LONG* 	RS_pnF21FLN4L; 		 //F线第四段距离 F21FLN4L
extern LONG* 	RS_pnF21FLN4X; 		 //F线第四段单位电抗 F21FLN4X
extern LONG* 	RS_pnF21FLN5L; 		 //F线第五段距离 F21FLN5L
extern LONG* 	RS_pnF21FLN5X; 		 //F线第五段单位电抗 F21FLN5X
extern LONG* 	RS_pnF21FLN6L; 		 //F线第六段距离 F21FLN6L
extern LONG* 	RS_pnF21FLN6X; 		 //F线第六段单位电抗 F21FLN6X
extern LONG* 	RS_pnF21FLN7L; 		 //F线第七段距离 F21FLN7L
extern LONG* 	RS_pnF21FLN7X; 		 //F线第七段单位电抗 F21FLN7X
extern LONG* 	RS_pnF21FLN8L; 		 //F线第八段距离 F21FLN8L
extern LONG* 	RS_pnF21FLN8X; 		 //F线第八段单位电抗 F21FLN8X
extern LONG* 	RS_pnF21FLN9L; 		 //F线第九段距离 F21FLN9L
extern LONG* 	RS_pnF21FLN9X; 		 //F线第九段单位电抗 F21FLN9X
extern LONG* 	RS_pnF21FLN10L; 		 //F线第十段距离 F21FLN10L
extern LONG* 	RS_pnF21FLN10X; 		 //F线第十段单位电抗 F21FLN10X

#define RS_COUNT     98    //定值个数/
#define RS_BUF_SIZE     392    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 290)

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
extern LONG* 	AS_pnTSW; 		 //电隔动作延时
extern LONG* 	AS_pn50IPC; 		 //IP有流定值
extern LONG* 	AS_pn37IPC; 		 //IP无流定值
extern LONG* 	AS_pn50ITC; 		 //It有流定值
extern LONG* 	AS_pn37ITC; 		 //It无流定值
extern LONG* 	AS_pn50IFC; 		 //If有流定值
extern LONG* 	AS_pn37IFC; 		 //If无流定值
extern LONG* 	AS_pn59UPV; 		 //Up有压定值
extern LONG* 	AS_pn27UPV; 		 //Up无压定值
extern LONG* 	AS_pn59U1V; 		 //U1有压定值
extern LONG* 	AS_pn27U1V; 		 //U1无压定值
extern LONG* 	AS_pn59U2V; 		 //U2有压定值
extern LONG* 	AS_pn27U2V; 		 //U2无压定值
extern LONG* 	AS_pn59U3V; 		 //U3有压定值
extern LONG* 	AS_pn27U3V; 		 //U3无压定值
extern LONG* 	AS_pn59U4V; 		 //U4有压定值
extern LONG* 	AS_pn27U4V; 		 //U4无压定值
extern LONG* 	AS_pnLMT; 		 //负荷监测起始时间
extern LONG* 	AS_pnLMCMAX; 		 //负荷监测电流限值
extern LONG* 	AS_pnLMVMIN; 		 //负荷监测电压限值
extern LONG* 	AS_pnTR1D; 		 //TR1保持时间
extern LONG* 	AS_pnTR2D; 		 //TR2保持时间
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间
extern LONG* 	AS_pnC_THRD; 		 //电流变化记录阀值
extern LONG* 	AS_pnV_THRD; 		 //电压变化记录阀值

//变比----------------------------------
extern LONG* 	AS_pnPT1Ratio; 		 //PT1变比
extern LONG* 	AS_pnPT2Ratio; 		 //PT2变比
extern LONG* 	AS_pnPT3Ratio; 		 //PT3变比
extern LONG* 	AS_pnPT4Ratio; 		 //PT4变比
extern LONG* 	AS_pnPCTRatio; 		 //保护CT变比
extern LONG* 	AS_pnMCTRatio; 		 //测量CT变比
extern LONG* 	AS_pnZRatio; 		 //阻抗变比
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     8    //辅助定值中变比个数

#define AS_COUNT     68    //辅助定值个数
#define AS_BUF_SIZE     272    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 682)

//系数----------------------------------
extern LONG* 	CF_pnIP; 		 //IP
extern LONG* 	CF_pnUP; 		 //UP
extern LONG* 	CF_pnU3; 		 //U3
extern LONG* 	CF_pnU4; 		 //U4
extern LONG* 	CF_pnITM; 		 //ITM
extern LONG* 	CF_pnIFM; 		 //IFM
extern LONG* 	CF_pnU1M; 		 //U1M
extern LONG* 	CF_pnU2M; 		 //U2M
extern LONG* 	CF_pnU3M; 		 //U3M
extern LONG* 	CF_pnU4M; 		 //U4M
extern LONG* 	CF_pnP; 		 //P
extern LONG* 	CF_pnQ; 		 //Q
extern LONG* 	CF_pnA; 		 //A
extern LONG* 	CF_pnZ; 		 //Z
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnITDRIFT; 		 //ITDRIFT
extern LONG* 	CF_pnIFDRIFT; 		 //IFDRIFT
extern LONG* 	CF_pnU1DRIFT; 		 //U1DRIFT
extern LONG* 	CF_pnU2DRIFT; 		 //U2DRIFT
extern LONG* 	CF_pnU3DRIFT; 		 //U3DRIFT
extern LONG* 	CF_pnU4DRIFT; 		 //U4DRIFT

#define CF_COUNT     24    //系数个数
#define CF_BUF_SIZE     96    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 954)

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
#define R_LOGICERR_R               21    //逻辑方程故障恢复
#define R_POWERERR_R               22    //5V电源故障恢复
#define R_ACTPICK                  23    //保护启动
#define R_R211_PI                  24    //距离Ⅰ段启动
#define R_R211                     25    //距离Ⅰ段动作
#define R_R211_RES                 26    //距离Ⅰ段返回
#define R_R212_PI                  27    //距离Ⅱ段启动
#define R_R212                     28    //距离Ⅱ段动作
#define R_R212_RES                 29    //距离Ⅱ段返回
#define R_R213_PI                  30    //距离Ⅲ段启动
#define R_R213                     31    //距离Ⅲ段动作
#define R_R213_RES                 32    //距离Ⅲ段返回
#define R_R214_PI                  33    //距离Ⅳ段启动
#define R_R214                     34    //距离Ⅳ段动作
#define R_R214_RES                 35    //距离Ⅳ段返回
#define R_R212_94_PI               36    //距离Ⅱ段加速启动
#define R_R212_94                  37    //距离Ⅱ段加速动作
#define R_R212_94_RES              38    //距离Ⅱ段加速返回
#define R_R213_94_PI               39    //距离Ⅲ段加速启动
#define R_R213_94                  40    //距离Ⅲ段加速动作
#define R_R213_94_RES              41    //距离Ⅲ段加速返回
#define R_R214_94_PI               42    //距离Ⅳ段加速启动
#define R_R214_94                  43    //距离Ⅳ段加速动作
#define R_R214_94_RES              44    //距离Ⅳ段加速返回
#define R_R50I0_PI                 45    //电流速断启动
#define R_R50I0                    46    //电流速断动作
#define R_R50I0_RES                47    //电流速断返回
#define R_R50I1_PI                 48    //过流Ⅰ段启动
#define R_R50I1                    49    //过流Ⅰ段动作
#define R_R50I1_RES                50    //过流Ⅰ段返回
#define R_R50I2_PI                 51    //过流Ⅱ段启动
#define R_R50I2                    52    //过流Ⅱ段动作
#define R_R50I2_RES                53    //过流Ⅱ段返回
#define R_R50I3_PI                 54    //过流Ⅲ段启动
#define R_R50I3                    55    //过流Ⅲ段动作
#define R_R50I3_RES                56    //过流Ⅲ段返回
#define R_R50I1_94_PI              57    //过流Ⅰ段加速启动
#define R_R50I1_94                 58    //过流Ⅰ段加速动作
#define R_R50I1_94_RES             59    //过流Ⅰ段加速返回
#define R_R50I2_94_PI              60    //过流Ⅱ段加速启动
#define R_R50I2_94                 61    //过流Ⅱ段加速动作
#define R_R50I2_94_RES             62    //过流Ⅱ段加速返回
#define R_R50I3_94_PI              63    //过流Ⅲ段加速启动
#define R_R50I3_94                 64    //过流Ⅲ段加速动作
#define R_R50I3_94_RES             65    //过流Ⅲ段加速返回
#define R_R50DI_PI                 66    //电流增量保护启动
#define R_R50DI                    67    //电流增量保护动作
#define R_R50DI_RES                68    //电流增量保护返回
#define R_R51I_PI                  69    //反时限过流启动
#define R_R51I                     70    //反时限过流动作
#define R_R51I_RES                 71    //反时限过流返回
#define R_R27TR                    72    //低压保护跳闸
#define R_R27ALARM                 73    //低压保护告警
#define R_TRSTEAL                  74    //偷跳重合闸
#define R_R79_1                    75    //一次重合闸出口
#define R_R79_2                    76    //二次重合闸出口
#define R_PTLK                     77    //PT断线闭锁距离保护
#define R_R79LK_1                  78    //一次重合闸被闭锁
#define R_R79LK_2                  79    //二次重合闸被闭锁
#define R_R52BF                    80    //失灵保护动作
#define R_PT1                      81    //PT1断线
#define R_PT2                      82    //PT2断线
#define R_PT1_RES                  83    //PT1断线返回
#define R_PT2_RES                  84    //PT2断线返回
#define R_R79FAIL                  85    //重合闸失败
#define R_PICKUP                   86    //保护启动
#define R_TRFAIL                   87    //断路器跳闸失败
#define R_COMTEST                  88    //通道测试
#define R_CT1                      89    //CT1断线告警
#define R_CT2                      90    //CT2断线告警
#define R_CT1_RES                  91    //CT1断线返回
#define R_CT2_RES                  92    //CT2断线返回
#define R_R79PI_1                  93    //一次重合闸启动
#define R_R79PI_2                  94    //二次重合闸启动
#define R_R79_1_SUCC               95    //一次重合闸成功
#define R_R79_1_FAIL               96    //一次重合闸失败
#define R_R79_2_SUCC               97    //二次重合闸成功
#define R_R79_2_FAIL               98    //二次重合闸失败
#define R_R79NPI_1                 99    //一次重合闸未启动
#define R_R79NPI_2                100    //二次重合闸未启动
#define R_R211_D_PI               101    //距离Ⅰ段启动
#define R_R211_D                  102    //距离Ⅰ段动作
#define R_R211_D_RES              103    //距离Ⅰ段返回
#define R_R212_D_PI               104    //距离Ⅱ段启动
#define R_R212_D                  105    //距离Ⅱ段动作
#define R_R212_D_RES              106    //距离Ⅱ段返回
#define R_R213_D_PI               107    //距离Ⅲ段启动
#define R_R213_D                  108    //距离Ⅲ段动作
#define R_R213_D_RES              109    //距离Ⅲ段返回
#define R_R214_D_PI               110    //距离Ⅳ段启动
#define R_R214_D                  111    //距离Ⅳ段动作
#define R_R214_D_RES              112    //距离Ⅳ段返回
#define R_R212_94_D_PI            113    //距离Ⅱ段加速启动
#define R_R212_94_D               114    //距离Ⅱ段加速动作
#define R_R212_94_D_RES           115    //距离Ⅱ段加速返回
#define R_R213_94_D_PI            116    //距离Ⅲ段加速启动
#define R_R213_94_D               117    //距离Ⅲ段加速动作
#define R_R213_94_D_RES           118    //距离Ⅲ段加速返回
#define R_R214_94_D_PI            119    //距离Ⅳ段加速启动
#define R_R214_94_D               120    //距离Ⅳ段加速动作
#define R_R214_94_D_RES           121    //距离Ⅳ段加速返回
#define R_R50I0_D_PI              122    //电流速断启动
#define R_R50I0_D                 123    //电流速断动作
#define R_R50I0_D_RES             124    //电流速断返回
#define R_R50I1_D_PI              125    //过流Ⅰ段启动
#define R_R50I1_D                 126    //过流Ⅰ段动作
#define R_R50I1_D_RES             127    //过流Ⅰ段返回
#define R_R50I2_D_PI              128    //过流Ⅱ段启动
#define R_R50I2_D                 129    //过流Ⅱ段动作
#define R_R50I2_D_RES             130    //过流Ⅱ段返回
#define R_R50I3_D_PI              131    //过流Ⅲ段启动
#define R_R50I3_D                 132    //过流Ⅲ段动作
#define R_R50I3_D_RES             133    //过流Ⅲ段返回
#define R_R50I1_94_D_PI           134    //过流Ⅰ段加速启动
#define R_R50I1_94_D              135    //过流Ⅰ段加速动作
#define R_R50I1_94_D_RES          136    //过流Ⅰ段加速返回
#define R_R50I2_94_D_PI           137    //过流Ⅱ段加速启动
#define R_R50I2_94_D              138    //过流Ⅱ段加速动作
#define R_R50I2_94_D_RES          139    //过流Ⅱ段加速返回
#define R_R50I3_94_D_PI           140    //过流Ⅲ段加速启动
#define R_R50I3_94_D              141    //过流Ⅲ段加速动作
#define R_R50I3_94_D_RES          142    //过流Ⅲ段加速返回
#define R_R50DI_D_PI              143    //电流增量保护启动
#define R_R50DI_D                 144    //电流增量保护动作
#define R_R50DI_D_RES             145    //电流增量保护返回
#define R_R51I_D_PI               146    //反时限过流启动
#define R_R51I_D                  147    //反时限过流动作
#define R_R51I_D_RES              148    //反时限过流返回
#define R_R27U_D                  149    //低压保护跳闸
#define R_R211_S_PI               150    //距离Ⅰ段启动
#define R_R211_S                  151    //距离Ⅰ段动作
#define R_R211_S_RES              152    //距离Ⅰ段返回
#define R_R212_S_PI               153    //距离Ⅱ段启动
#define R_R212_S                  154    //距离Ⅱ段动作
#define R_R212_S_RES              155    //距离Ⅱ段返回
#define R_R213_S_PI               156    //距离Ⅲ段启动
#define R_R213_S                  157    //距离Ⅲ段动作
#define R_R213_S_RES              158    //距离Ⅲ段返回
#define R_R214_S_PI               159    //距离Ⅳ段启动
#define R_R214_S                  160    //距离Ⅳ段动作
#define R_R214_S_RES              161    //距离Ⅳ段返回
#define R_R212_94_S_PI            162    //距离Ⅱ段加速启动
#define R_R212_94_S               163    //距离Ⅱ段加速动作
#define R_R212_94_S_RES           164    //距离Ⅱ段加速返回
#define R_R213_94_S_PI            165    //距离Ⅲ段加速启动
#define R_R213_94_S               166    //距离Ⅲ段加速动作
#define R_R213_94_S_RES           167    //距离Ⅲ段加速返回
#define R_R214_94_S_PI            168    //距离Ⅳ段加速启动
#define R_R214_94_S               169    //距离Ⅳ段加速动作
#define R_R214_94_S_RES           170    //距离Ⅳ段加速返回
#define R_R50I0_S_PI              171    //电流速断启动
#define R_R50I0_S                 172    //电流速断动作
#define R_R50I0_S_RES             173    //电流速断返回
#define R_R50I1_S_PI              174    //过流Ⅰ段启动
#define R_R50I1_S                 175    //过流Ⅰ段动作
#define R_R50I1_S_RES             176    //过流Ⅰ段返回
#define R_R50I2_S_PI              177    //过流Ⅱ段启动
#define R_R50I2_S                 178    //过流Ⅱ段动作
#define R_R50I2_S_RES             179    //过流Ⅱ段返回
#define R_R50I3_S_PI              180    //过流Ⅲ段启动
#define R_R50I3_S                 181    //过流Ⅲ段动作
#define R_R50I3_S_RES             182    //过流Ⅲ段返回
#define R_R50I1_94_S_PI           183    //过流Ⅰ段加速启动
#define R_R50I1_94_S              184    //过流Ⅰ段加速动作
#define R_R50I1_94_S_RES          185    //过流Ⅰ段加速返回
#define R_R50I2_94_S_PI           186    //过流Ⅱ段加速启动
#define R_R50I2_94_S              187    //过流Ⅱ段加速动作
#define R_R50I2_94_S_RES          188    //过流Ⅱ段加速返回
#define R_R50I3_94_S_PI           189    //过流Ⅲ段加速启动
#define R_R50I3_94_S              190    //过流Ⅲ段加速动作
#define R_R50I3_94_S_RES          191    //过流Ⅲ段加速返回
#define R_R50DI_S_PI              192    //电流增量保护启动
#define R_R50DI_S                 193    //电流增量保护动作
#define R_R50DI_S_RES             194    //电流增量保护返回
#define R_R51I_S_PI               195    //反时限过流启动
#define R_R51I_S                  196    //反时限过流动作
#define R_R51I_S_RES              197    //反时限过流返回
#define R_R27U_S                  198    //低压保护跳闸
#define R_RELDATAERR              199    //保护数据异常
#define R_RELDATAERR_R            200    //保护数据异常恢复
#define MAX_REPORT_INDEX          200    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     13    //传动个数

#endif