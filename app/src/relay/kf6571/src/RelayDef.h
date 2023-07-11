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
//----修正方向0:+,1------
#define 	FORWORD 	0	//正方向
#define 	BACKWORD 	1	//反方向
//----公里标方向0:+,1------
#define 	FORWORD 	0	//正方向
#define 	BACKWORD 	1	//反方向
//----通道类型-----
#define 	ETHNET 	0	//以太网直连
#define 	SERIAL 	1	//串口连接
#define 	ROUTER 	2	//路由连接

//辅助定值取值宏定义
//----额定电流-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----供电方式-----
#define 	AT1MODE 	0	//AT供电
#define 	AT2MODE 	1	//客专方式

//保护测量值定义-----------------------
#define  RM_U1 0
extern LONG* 	RM_pnU1; 		 // U1
#define  RM_U2 1
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_U3 2
extern LONG* 	RM_pnU3; 		 // U3
#define  RM_U4 3
extern LONG* 	RM_pnU4; 		 // U4
#define  RM_IT1 4
extern LONG* 	RM_pnIt1; 		 // It1
#define  RM_IF1 5
extern LONG* 	RM_pnIf1; 		 // If1
#define  RM_IT2 6
extern LONG* 	RM_pnIt2; 		 // It2
#define  RM_IF2 7
extern LONG* 	RM_pnIf2; 		 // If2
#define  RM_IT3 8
extern LONG* 	RM_pnIt3; 		 // It3
#define  RM_IF3 9
extern LONG* 	RM_pnIf3; 		 // If3
#define  RM_IAT1 10
extern LONG* 	RM_pnIat1; 		 // Iat1
#define  RM_IAT2 11
extern LONG* 	RM_pnIat2; 		 // Iat2
#define  RM_SSIAT 12
extern LONG* 	RM_pnSSIAT; 		 // IAT
#define  RM_U1A 13
extern LONG* 	RM_pnU1A; 		 // AU1
#define  RM_U2A 14
extern LONG* 	RM_pnU2A; 		 // AU2
#define  RM_U3A 15
extern LONG* 	RM_pnU3A; 		 // AU3
#define  RM_U4A 16
extern LONG* 	RM_pnU4A; 		 // AU4
#define  RM_IT1A 17
extern LONG* 	RM_pnIT1A; 		 // AIT1
#define  RM_IF1A 18
extern LONG* 	RM_pnIF1A; 		 // AIF1
#define  RM_IT2A 19
extern LONG* 	RM_pnIT2A; 		 // AIT2
#define  RM_IF2A 20
extern LONG* 	RM_pnIF2A; 		 // AIF2
#define  RM_IT3A 21
extern LONG* 	RM_pnIT3A; 		 // AIT3
#define  RM_IF3A 22
extern LONG* 	RM_pnIF3A; 		 // AIF3
#define  RM_BOBRCH1 23
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 24
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 25
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 26
extern LONG* 	RM_pnBOINCH2; 		 // IN2
#define  RM_SUB1_U1 27
extern LONG* 	RM_pnSub1_U1; 		 // U1
#define  RM_SUB1_U2 28
extern LONG* 	RM_pnSub1_U2; 		 // U2
#define  RM_SUB1_U3 29
extern LONG* 	RM_pnSub1_U3; 		 // U3
#define  RM_SUB1_U4 30
extern LONG* 	RM_pnSub1_U4; 		 // U4
#define  RM_SUB1_IT1 31
extern LONG* 	RM_pnSub1_IT1; 		 // IT1
#define  RM_SUB1_IF1 32
extern LONG* 	RM_pnSub1_IF1; 		 // IF1
#define  RM_SUB1_IT2 33
extern LONG* 	RM_pnSub1_IT2; 		 // IT2
#define  RM_SUB1_IF2 34
extern LONG* 	RM_pnSub1_IF2; 		 // IF2
#define  RM_SUB1_IT3 35
extern LONG* 	RM_pnSub1_IT3; 		 // IT3
#define  RM_SUB1_IF3 36
extern LONG* 	RM_pnSub1_IF3; 		 // IF3
#define  RM_SUB1_IAT1 37
extern LONG* 	RM_pnSub1_IAT1; 		 // IAT1
#define  RM_SUB1_IAT2 38
extern LONG* 	RM_pnSub1_IAT2; 		 // IAT2
#define  RM_SUB1_IAT 39
extern LONG* 	RM_pnSub1_IAT; 		 // IAT
#define  RM_SUB1_AIT1 40
extern LONG* 	RM_pnSub1_AIT1; 		 // AIT1
#define  RM_SUB1_AIF1 41
extern LONG* 	RM_pnSub1_AIF1; 		 // AIF1
#define  RM_SUB1_AIT2 42
extern LONG* 	RM_pnSub1_AIT2; 		 // AIT2
#define  RM_SUB1_AIF2 43
extern LONG* 	RM_pnSub1_AIF2; 		 // AIF2
#define  RM_SUB1_AIT3 44
extern LONG* 	RM_pnSub1_AIT3; 		 // AIT3
#define  RM_SUB1_AIF3 45
extern LONG* 	RM_pnSub1_AIF3; 		 // AIF3
#define  RM_SUB1_STATUS 46
extern LONG* 	RM_pnSub1_Status; 		 // ST
#define  RM_SUB1_YEAR 47
extern LONG* 	RM_pnSub1_year; 		 // Year
#define  RM_SUB1_MONTH 48
extern LONG* 	RM_pnSub1_month; 		 // Mon.
#define  RM_SUB1_DAY 49
extern LONG* 	RM_pnSub1_day; 		 // Day
#define  RM_SUB1_HOUR 50
extern LONG* 	RM_pnSub1_hour; 		 // Hour
#define  RM_SUB1_MINUTE 51
extern LONG* 	RM_pnSub1_minute; 		 // Min.
#define  RM_SUB1_NS 52
extern LONG* 	RM_pnSub1_NS; 		 // NS
#define  RM_SUB2_U1 53
extern LONG* 	RM_pnSub2_U1; 		 // U1
#define  RM_SUB2_U2 54
extern LONG* 	RM_pnSub2_U2; 		 // U2
#define  RM_SUB2_U3 55
extern LONG* 	RM_pnSub2_U3; 		 // U3
#define  RM_SUB2_U4 56
extern LONG* 	RM_pnSub2_U4; 		 // U4
#define  RM_SUB2_IT1 57
extern LONG* 	RM_pnSub2_IT1; 		 // IT1
#define  RM_SUB2_IF1 58
extern LONG* 	RM_pnSub2_IF1; 		 // IF1
#define  RM_SUB2_IT2 59
extern LONG* 	RM_pnSub2_IT2; 		 // IT2
#define  RM_SUB2_IF2 60
extern LONG* 	RM_pnSub2_IF2; 		 // IF2
#define  RM_SUB2_IT3 61
extern LONG* 	RM_pnSub2_IT3; 		 // IT3
#define  RM_SUB2_IF3 62
extern LONG* 	RM_pnSub2_IF3; 		 // IF3
#define  RM_SUB2_IAT1 63
extern LONG* 	RM_pnSub2_IAT1; 		 // IAT1
#define  RM_SUB2_IAT2 64
extern LONG* 	RM_pnSub2_IAT2; 		 // IAT2
#define  RM_SUB2_IAT 65
extern LONG* 	RM_pnSub2_IAT; 		 // IAT
#define  RM_SUB2_AIT1 66
extern LONG* 	RM_pnSub2_AIT1; 		 // AIT1
#define  RM_SUB2_AIF1 67
extern LONG* 	RM_pnSub2_AIF1; 		 // AIF1
#define  RM_SUB2_AIT2 68
extern LONG* 	RM_pnSub2_AIT2; 		 // AIT2
#define  RM_SUB2_AIF2 69
extern LONG* 	RM_pnSub2_AIF2; 		 // AIF2
#define  RM_SUB2_AIT3 70
extern LONG* 	RM_pnSub2_AIT3; 		 // AIT3
#define  RM_SUB2_AIF3 71
extern LONG* 	RM_pnSub2_AIF3; 		 // AIF3
#define  RM_SUB2_STATUS 72
extern LONG* 	RM_pnSub2_Status; 		 // ST
#define  RM_SUB2_YEAR 73
extern LONG* 	RM_pnSub2_year; 		 // Year
#define  RM_SUB2_MONTH 74
extern LONG* 	RM_pnSub2_month; 		 // Mon.
#define  RM_SUB2_DAY 75
extern LONG* 	RM_pnSub2_day; 		 // Day
#define  RM_SUB2_HOUR 76
extern LONG* 	RM_pnSub2_hour; 		 // Hour
#define  RM_SUB2_MINUTE 77
extern LONG* 	RM_pnSub2_minute; 		 // Min.
#define  RM_SUB2_NS 78
extern LONG* 	RM_pnSub2_NS; 		 // NS
#define  RM_SUB3_U1 79
extern LONG* 	RM_pnSub3_U1; 		 // U1
#define  RM_SUB3_U2 80
extern LONG* 	RM_pnSub3_U2; 		 // U2
#define  RM_SUB3_U3 81
extern LONG* 	RM_pnSub3_U3; 		 // U3
#define  RM_SUB3_U4 82
extern LONG* 	RM_pnSub3_U4; 		 // U4
#define  RM_SUB3_IT1 83
extern LONG* 	RM_pnSub3_IT1; 		 // IT1
#define  RM_SUB3_IF1 84
extern LONG* 	RM_pnSub3_IF1; 		 // IF1
#define  RM_SUB3_IT2 85
extern LONG* 	RM_pnSub3_IT2; 		 // IT2
#define  RM_SUB3_IF2 86
extern LONG* 	RM_pnSub3_IF2; 		 // IF2
#define  RM_SUB3_IT3 87
extern LONG* 	RM_pnSub3_IT3; 		 // IT3
#define  RM_SUB3_IF3 88
extern LONG* 	RM_pnSub3_IF3; 		 // IF3
#define  RM_SUB3_IAT1 89
extern LONG* 	RM_pnSub3_IAT1; 		 // IAT1
#define  RM_SUB3_IAT2 90
extern LONG* 	RM_pnSub3_IAT2; 		 // IAT2
#define  RM_SUB3_IAT 91
extern LONG* 	RM_pnSub3_IAT; 		 // IAT
#define  RM_SUB3_AIT1 92
extern LONG* 	RM_pnSub3_AIT1; 		 // AIT1
#define  RM_SUB3_AIF1 93
extern LONG* 	RM_pnSub3_AIF1; 		 // AIF1
#define  RM_SUB3_AIT2 94
extern LONG* 	RM_pnSub3_AIT2; 		 // AIT2
#define  RM_SUB3_AIF2 95
extern LONG* 	RM_pnSub3_AIF2; 		 // AIF2
#define  RM_SUB3_AIT3 96
extern LONG* 	RM_pnSub3_AIT3; 		 // AIT3
#define  RM_SUB3_AIF3 97
extern LONG* 	RM_pnSub3_AIF3; 		 // AIF3
#define  RM_SUB3_STATUS 98
extern LONG* 	RM_pnSub3_Status; 		 // ST
#define  RM_SUB3_YEAR 99
extern LONG* 	RM_pnSub3_year; 		 // Year
#define  RM_SUB3_MONTH 100
extern LONG* 	RM_pnSub3_month; 		 // Mon.
#define  RM_SUB3_DAY 101
extern LONG* 	RM_pnSub3_day; 		 // Day
#define  RM_SUB3_HOUR 102
extern LONG* 	RM_pnSub3_hour; 		 // Hour
#define  RM_SUB3_MINUTE 103
extern LONG* 	RM_pnSub3_minute; 		 // Min.
#define  RM_SUB3_NS 104
extern LONG* 	RM_pnSub3_NS; 		 // NS
#define  RM_SUB4_U1 105
extern LONG* 	RM_pnSub4_U1; 		 // U1
#define  RM_SUB4_U2 106
extern LONG* 	RM_pnSub4_U2; 		 // U2
#define  RM_SUB4_U3 107
extern LONG* 	RM_pnSub4_U3; 		 // U3
#define  RM_SUB4_U4 108
extern LONG* 	RM_pnSub4_U4; 		 // U4
#define  RM_SUB4_IT1 109
extern LONG* 	RM_pnSub4_IT1; 		 // IT1
#define  RM_SUB4_IF1 110
extern LONG* 	RM_pnSub4_IF1; 		 // IF1
#define  RM_SUB4_IT2 111
extern LONG* 	RM_pnSub4_IT2; 		 // IT2
#define  RM_SUB4_IF2 112
extern LONG* 	RM_pnSub4_IF2; 		 // IF2
#define  RM_SUB4_IT3 113
extern LONG* 	RM_pnSub4_IT3; 		 // IT3
#define  RM_SUB4_IF3 114
extern LONG* 	RM_pnSub4_IF3; 		 // IF3
#define  RM_SUB4_IAT1 115
extern LONG* 	RM_pnSub4_IAT1; 		 // IAT1
#define  RM_SUB4_IAT2 116
extern LONG* 	RM_pnSub4_IAT2; 		 // IAT2
#define  RM_SUB4_IAT 117
extern LONG* 	RM_pnSub4_IAT; 		 // IAT
#define  RM_SUB4_AIT1 118
extern LONG* 	RM_pnSub4_AIT1; 		 // AIT1
#define  RM_SUB4_AIF1 119
extern LONG* 	RM_pnSub4_AIF1; 		 // AIF1
#define  RM_SUB4_AIT2 120
extern LONG* 	RM_pnSub4_AIT2; 		 // AIT2
#define  RM_SUB4_AIF2 121
extern LONG* 	RM_pnSub4_AIF2; 		 // AIF2
#define  RM_SUB4_AIT3 122
extern LONG* 	RM_pnSub4_AIT3; 		 // AIT3
#define  RM_SUB4_AIF3 123
extern LONG* 	RM_pnSub4_AIF3; 		 // AIF3
#define  RM_SUB4_STATUS 124
extern LONG* 	RM_pnSub4_Status; 		 // ST
#define  RM_SUB4_YEAR 125
extern LONG* 	RM_pnSub4_year; 		 // Year
#define  RM_SUB4_MONTH 126
extern LONG* 	RM_pnSub4_month; 		 // Mon.
#define  RM_SUB4_DAY 127
extern LONG* 	RM_pnSub4_day; 		 // Day
#define  RM_SUB4_HOUR 128
extern LONG* 	RM_pnSub4_hour; 		 // Hour
#define  RM_SUB4_MINUTE 129
extern LONG* 	RM_pnSub4_minute; 		 // Min.
#define  RM_SUB4_NS 130
extern LONG* 	RM_pnSub4_NS; 		 // NS
#define  RM_SUB5_U1 131
extern LONG* 	RM_pnSub5_U1; 		 // U1
#define  RM_SUB5_U2 132
extern LONG* 	RM_pnSub5_U2; 		 // U2
#define  RM_SUB5_U3 133
extern LONG* 	RM_pnSub5_U3; 		 // U3
#define  RM_SUB5_U4 134
extern LONG* 	RM_pnSub5_U4; 		 // U4
#define  RM_SUB5_IT1 135
extern LONG* 	RM_pnSub5_IT1; 		 // IT1
#define  RM_SUB5_IF1 136
extern LONG* 	RM_pnSub5_IF1; 		 // IF1
#define  RM_SUB5_IT2 137
extern LONG* 	RM_pnSub5_IT2; 		 // IT2
#define  RM_SUB5_IF2 138
extern LONG* 	RM_pnSub5_IF2; 		 // IF2
#define  RM_SUB5_IT3 139
extern LONG* 	RM_pnSub5_IT3; 		 // IT3
#define  RM_SUB5_IF3 140
extern LONG* 	RM_pnSub5_IF3; 		 // IF3
#define  RM_SUB5_IAT1 141
extern LONG* 	RM_pnSub5_IAT1; 		 // IAT1
#define  RM_SUB5_IAT2 142
extern LONG* 	RM_pnSub5_IAT2; 		 // IAT2
#define  RM_SUB5_IAT 143
extern LONG* 	RM_pnSub5_IAT; 		 // IAT
#define  RM_SUB5_AIT1 144
extern LONG* 	RM_pnSub5_AIT1; 		 // AIT1
#define  RM_SUB5_AIF1 145
extern LONG* 	RM_pnSub5_AIF1; 		 // AIF1
#define  RM_SUB5_AIT2 146
extern LONG* 	RM_pnSub5_AIT2; 		 // AIT2
#define  RM_SUB5_AIF2 147
extern LONG* 	RM_pnSub5_AIF2; 		 // AIF2
#define  RM_SUB5_AIT3 148
extern LONG* 	RM_pnSub5_AIT3; 		 // AIT3
#define  RM_SUB5_AIF3 149
extern LONG* 	RM_pnSub5_AIF3; 		 // AIF3
#define  RM_SUB5_STATUS 150
extern LONG* 	RM_pnSub5_Status; 		 // ST
#define  RM_SUB5_YEAR 151
extern LONG* 	RM_pnSub5_year; 		 // Year
#define  RM_SUB5_MONTH 152
extern LONG* 	RM_pnSub5_month; 		 // Mon.
#define  RM_SUB5_DAY 153
extern LONG* 	RM_pnSub5_day; 		 // Day
#define  RM_SUB5_HOUR 154
extern LONG* 	RM_pnSub5_hour; 		 // Hour
#define  RM_SUB5_MINUTE 155
extern LONG* 	RM_pnSub5_minute; 		 // Min.
#define  RM_SUB5_NS 156
extern LONG* 	RM_pnSub5_NS; 		 // NS
#define  RM_DIST 157
extern LONG* 	RM_pnDist; 		 // D
#define  RM_KMS 158
extern LONG* 	RM_pnKMS; 		 // KMS
#define  RM_R_T 159
extern LONG* 	RM_pnR_T; 		 // R_T
#define  RM_X_T 160
extern LONG* 	RM_pnX_T; 		 // X_T
#define  RM_R_F 161
extern LONG* 	RM_pnR_F; 		 // R_F
#define  RM_X_F 162
extern LONG* 	RM_pnX_F; 		 // X_F
#define  RM_R_TF 163
extern LONG* 	RM_pnR_TF; 		 // R_TF
#define  RM_X_TF 164
extern LONG* 	RM_pnX_TF; 		 // X_TF
#define  RM_NS 165
extern LONG* 	RM_pnNS; 		 // NS
#define  RM_IP_H2 166
extern LONG* 	RM_pnIP_H2; 		 // IPh2
#define  RM_IP_H3 167
extern LONG* 	RM_pnIP_H3; 		 // IPh3
#define  RM_IP_H5 168
extern LONG* 	RM_pnIP_H5; 		 // IPh5
#define  RM_UP 169
extern LONG* 	RM_pnUp; 		 // Up
#define  RM_I1 170
extern LONG* 	RM_pnI1; 		 // I1
#define  RM_I2 171
extern LONG* 	RM_pnI2; 		 // I2
#define  RM_I3 172
extern LONG* 	RM_pnI3; 		 // I3

#define RM_COUNT     173    //保护测量值个数
#define RM_BUF_SIZE     692    //保护测量值缓冲区大小
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//测量值定义-----------------------

#define MEASURE_COUNT     0    //测量值个数
#define MEASURE_BUF_SIZE     0    //测量值缓冲区大小
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 696)

//软压板----------------------------------
extern BYTE* 	SW_pbyFL; 		 //故障测距
extern BYTE* 	SW_pbyMAINT; 		 //检修压板
extern BYTE* 	SW_pbyRESD1; 		 //备用压板1
extern BYTE* 	SW_pbyRESD2; 		 //备用压板2

#define SW_COUNT     4    //软压板个数
#define SW_BUF_SIZE     4    //软压板缓冲区大小
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 696)

//配置----------------------------------
extern BYTE* 	CFG_pby21_PI; 		 //阻抗启动
extern BYTE* 	CFG_pby50_PI; 		 //过流启动
extern BYTE* 	CFG_pby50DI_PI; 		 //增量启动
extern BYTE* 	CFG_pbyFL_AUTO; 		 //测距自适应
extern BYTE* 	CFG_pbyCCMODE; 		 //电流合成
extern BYTE* 	CFG_pbyAT_C_RATE; 		 //吸上电流比
extern BYTE* 	CFG_pbyUD_C_RATE; 		 //上下行电流比
extern BYTE* 	CFG_pbyNL_C_RATE; 		 //吸馈电流比
extern BYTE* 	CFG_pbyPHASESC; 		 //相间短路判断
extern BYTE* 	CFG_pbyNONEAT; 		 //变电所无AT
extern BYTE* 	CFG_pbyISSS; 		 //本所为变电所
extern BYTE* 	CFG_pbyPARALLG; 		 //平行四边形
extern BYTE* 	CFG_pby21_H_S; 		 //距离谐波抑制
extern BYTE* 	CFG_pby21_H_L; 		 //距离谐波闭锁
extern BYTE* 	CFG_pby50_H_S; 		 //过流谐波抑制
extern BYTE* 	CFG_pby50_H_L; 		 //过流谐波闭锁
extern BYTE* 	CFG_pby50_27V; 		 //过流启动低压闭锁
extern BYTE* 	CFG_pbyCALLDATA; 		 //召测取低压数据
extern BYTE* 	CFG_pbyCHECKGPS; 		 //GPS故障检测
extern BYTE* 	CFG_pbyCHECKCOM; 		 //通道自动检测
extern BYTE* 	CFG_pbyCROSSAERA; 		 //允许越区
extern BYTE* 	CFG_pbyRESD1; 		 //备用配置1
extern BYTE* 	CFG_pbyRESD2; 		 //备用配置2

#define CFG_COUNT     23    //配置个数/
#define CFG_BUF_SIZE     23    //配置缓冲区大小
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 700)

//定值----------------------------------
extern LONG* 	RS_pnCFG; 		 //配置字 CFG
extern LONG* 	RS_pnPIC; 		 //突变量启动电流 PIC
extern LONG* 	RS_pn21R; 		 //距离启动电阻 21R
extern LONG* 	RS_pn21X; 		 //距离启动电抗 21X
extern LONG* 	RS_pn78LA; 		 //线路阻抗角 78LA
extern LONG* 	RS_pn50C; 		 //过流定值 50C
extern LONG* 	RS_pn50_27V; 		 //过流启动低压闭锁定值 50_27V
extern LONG* 	RS_pn50DC; 		 //电流增量启动电流定值 50DC
extern LONG* 	RS_pn50D_37C; 		 //增量保护小电流闭锁 50D_37C
extern LONG* 	RS_pnFLT; 		 //故标启动延时 FLT
extern LONG* 	RS_pnKh1; 		 //二次谐波闭锁系数 Kh1
extern LONG* 	RS_pnKhr1; 		 //距离过流谐波加权 Khr1
extern LONG* 	RS_pnKhr2; 		 //增量保护谐波加权 Khr2
extern LONG* 	RS_pn27Uab; 		 //相间短路低压定值 27Uab
extern LONG* 	RS_pn27U; 		 //低压记录数据定值 27U
extern LONG* 	RS_pn27D; 		 //低压数据记录延时 27D
extern LONG* 	RS_pnN; 		 //子所个数 N
extern LONG* 	RS_pnNID; 		 //本所站号 NID
extern LONG* 	RS_pnD1; 		 //AT区间1长度 D1
extern LONG* 	RS_pnD2; 		 //AT区间2长度 D2
extern LONG* 	RS_pnD3; 		 //AT区间3长度 D3
extern LONG* 	RS_pnD4; 		 //AT区间4长度 D4
extern LONG* 	RS_pnD5; 		 //AT区间5长度 D5
extern LONG* 	RS_pnQss; 		 //变电所AT漏抗 Qss
extern LONG* 	RS_pnQ1; 		 //子所1AT漏抗 Q1
extern LONG* 	RS_pnQ2; 		 //子所2AT漏抗 Q2
extern LONG* 	RS_pnQ3; 		 //子所3AT漏抗 Q3
extern LONG* 	RS_pnQ4; 		 //子所4AT漏抗 Q4
extern LONG* 	RS_pnQ5; 		 //子所5AT漏抗 Q5
extern LONG* 	RS_pnKss; 		 //变电所电流分布系数 Kss
extern LONG* 	RS_pnK1; 		 //子所1AT电流分布系数 K1
extern LONG* 	RS_pnK2; 		 //子所2AT电流分布系数 K2
extern LONG* 	RS_pnK3; 		 //子所3AT电流分布系数 K3
extern LONG* 	RS_pnK4; 		 //子所4AT电流分布系数 K4
extern LONG* 	RS_pnK5; 		 //子所5AT电流分布系数 K5
extern LONG* 	RS_pnKF; 		 //故障判断系数 KF
extern LONG* 	RS_pnLa; 		 //供电臂长度 La
extern LONG* 	RS_pnLc; 		 //修正距离 Lc
extern LONG* 	RS_pnLD; 		 //修正方向0:+,1- LD
extern LONG* 	RS_pnS0; 		 //变电所公里标 S0
extern LONG* 	RS_pnSD; 		 //公里标方向0:+,1- SD
extern LONG* 	RS_pnZOFFSET; 		 //平行四边形偏移阻抗 ZOFFSET
extern LONG* 	RS_pnAOFFSET; 		 //阻抗四边形偏移角度 AOFFSET
extern LONG* 	RS_pnChanType; 		 //通道类型 ChanType
extern LONG* 	RS_pnCAID; 		 //越区全局编号 CAID
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
extern LONG* 	RS_pnTF21FLN; 		 //TF测距分段数 TF21FLN
extern LONG* 	RS_pnTF21FLN1L; 		 //TF测距第一段距离 TF21FLN1L
extern LONG* 	RS_pnTF21FLN1X; 		 //TF测距一段单位电抗 TF21FLN1X
extern LONG* 	RS_pnTF21FLN2L; 		 //TF测距第二段距离 TF21FLN2L
extern LONG* 	RS_pnTF21FLN2X; 		 //TF测距二段单位电抗 TF21FLN2X
extern LONG* 	RS_pnTF21FLN3L; 		 //TF测距第三段距离 TF21FLN3L
extern LONG* 	RS_pnTF21FLN3X; 		 //TF测距三段单位电抗 TF21FLN3X
extern LONG* 	RS_pnTF21FLN4L; 		 //TF测距第四段距离 TF21FLN4L
extern LONG* 	RS_pnTF21FLN4X; 		 //TF测距四段单位电抗 TF21FLN4X
extern LONG* 	RS_pnTF21FLN5L; 		 //TF测距第五段距离 TF21FLN5L
extern LONG* 	RS_pnTF21FLN5X; 		 //TF测距五段单位电抗 TF21FLN5X
extern LONG* 	RS_pnTF21FLN6L; 		 //TF测距第六段距离 TF21FLN6L
extern LONG* 	RS_pnTF21FLN6X; 		 //TF测距六段单位电抗 TF21FLN6X

#define RS_COUNT     84    //定值个数/
#define RS_BUF_SIZE     336    //定值缓冲区大小
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 726)

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
extern LONG* 	AS_pn50I1; 		 //I1有流定值
extern LONG* 	AS_pn37I1; 		 //I1无流定值
extern LONG* 	AS_pn50I2; 		 //I2有流定值
extern LONG* 	AS_pn37I2; 		 //I2无流定值
extern LONG* 	AS_pn50I3; 		 //I3有流定值
extern LONG* 	AS_pn37I3; 		 //I3无流定值
extern LONG* 	AS_pn59U1; 		 //U1有压定值
extern LONG* 	AS_pn27U1; 		 //U1无压定值
extern LONG* 	AS_pn59U2; 		 //U2有压定值
extern LONG* 	AS_pn27U2; 		 //U2无压定值
extern LONG* 	AS_pn59U3; 		 //U3有压定值
extern LONG* 	AS_pn27U3; 		 //U3无压定值
extern LONG* 	AS_pn59U4; 		 //U4有压定值
extern LONG* 	AS_pn27U4; 		 //U4无压定值
extern LONG* 	AS_pnRECD; 		 //故障录波触发时间

//变比----------------------------------
extern LONG* 	AS_pnUPTRatio; 		 //U变比
extern LONG* 	AS_pnI1CTRatio; 		 //I1变比
extern LONG* 	AS_pnI2CTRatio; 		 //I2变比
extern LONG* 	AS_pnI3CTRatio; 		 //I3变比
extern LONG* 	AS_pnIATCTRatio; 		 //IAT变比
extern LONG* 	AS_pnZRatio; 		 //阻抗变比
extern LONG* 	AS_pnRT; 		 //RT

#define AS_RT_NUM     7    //辅助定值中变比个数

#define AS_COUNT     60    //辅助定值个数
#define AS_BUF_SIZE     240    //辅助定值缓冲区大小
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 1062)

//系数----------------------------------
extern LONG* 	CF_pnU1; 		 //U1
extern LONG* 	CF_pnU2; 		 //U2
extern LONG* 	CF_pnU3; 		 //U3
extern LONG* 	CF_pnU4; 		 //U4
extern LONG* 	CF_pnIT1; 		 //IT1
extern LONG* 	CF_pnIF1; 		 //IF1
extern LONG* 	CF_pnIT2; 		 //IT2
extern LONG* 	CF_pnIF2; 		 //IF2
extern LONG* 	CF_pnIT3; 		 //IT3
extern LONG* 	CF_pnIF3; 		 //IF3
extern LONG* 	CF_pnIAT1; 		 //IAT1
extern LONG* 	CF_pnIAT2; 		 //IAT2
extern LONG* 	CF_pnZ; 		 //Z
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4

#define CF_COUNT     17    //系数个数
#define CF_BUF_SIZE     68    //系数缓冲区大小
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 1302)

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
#define R_POWERERR                 13    //5V电源故障
#define R_EEPROM_W_ERR             14    //EEPROM写入故障
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
#define R_FLPICKUP                 26    //故障测距启动
#define R_TRBREAK_F                27    //上行T-R故障
#define R_FRBREAK_F                28    //上行F-R故障
#define R_TRBREAK_B                29    //下行T-R故障
#define R_FRBREAK_B                30    //下行F-R故障
#define R_TFBREAK                  31    //T-F故障
#define R_SUB1GPSERR               32    //子所1GPS故障
#define R_SUB2GPSERR               33    //子所2GPS故障
#define R_SUB3GPSERR               34    //子所3GPS故障
#define R_SUB4GPSERR               35    //子所4GPS故障
#define R_SUB5GPSERR               36    //子所5GPS故障
#define R_FLACT                    37    //保护测距动作
#define R_CHANNELOK                38    //通道测试成功
#define R_CHANNELERR               39    //通道故障
#define R_CHANNEL1ERR              40    //通道1故障
#define R_CHANNEL2ERR              41    //通道2故障
#define R_CHANNEL3ERR              42    //通道3故障
#define R_CHANNEL4ERR              43    //通道4故障
#define R_CHANNEL5ERR              44    //通道5故障
#define R_PHASEBR                  45    //相间故障
#define R_GPSERR                   46    //GPS故障
#define R_FLFAIL_CHANNEL           47    //通道故障测距失败
#define R_TIMEOUT                  48    //超时故障测距失败
#define R_CALLACT                  49    //召测测距动作
#define R_PTALARM                  50    //PT断线
#define R_COMTEST                  51    //通信测试
#define R_SUB1DATA                 52    //子所1故障数据
#define R_SUB2DATA                 53    //子所2故障数据
#define R_SUB3DATA                 54    //子所3故障数据
#define R_SUB4DATA                 55    //子所4故障数据
#define R_SUB5DATA                 56    //子所5故障数据
#define R_GPSFAULT                 57    //GPS故障测距失败
#define R_SUB1LOSS                 58    //子所1数据丢失
#define R_SUB2LOSS                 59    //子所2数据丢失
#define R_SUB3LOSS                 60    //子所3数据丢失
#define R_SUB4LOSS                 61    //子所4数据丢失
#define R_SUB5LOSS                 62    //子所5数据丢失
#define R_FLFAIL                   63    //故障测距失败
#define R_GPSFAULT_UD              64    //GPS故障测距失败
#define R_STEPSUCC                 65    //越区切换成功
#define R_STEPFAIL                 66    //越区切换失败
#define R_TIMEOUT_UD               67    //超时故障测距失败
#define R_TR_F_X                   68    //上行TR故障-电抗测距
#define R_FR_F_X                   69    //上行FR故障-电抗测距
#define R_TR_B_X                   70    //下行TR故障-电抗测距
#define R_FR_B_X                   71    //下行FR故障-电抗测距
#define R_PICKUP                   72    //保护启动
#define R_TRBREAK_F_UD             73    //上行T-R故障
#define R_FRBREAK_F_UD             74    //上行F-R故障
#define R_TRBREAK_B_UD             75    //下行T-R故障
#define R_FRBREAK_B_UD             76    //下行F-R故障
#define R_TFBREAK_UD               77    //T-F故障
#define R_COMTEST_UD               78    //通信测试
#define R_FLFAIL_UD                79    //故障测距失败
#define R_RELDATAERR               80    //保护数据异常
#define R_UNKOWNMODE               81    //供电方式异常
#define R_TFBREAK_F_UD             82    //上行T-F故障
#define R_TFBREAK_B_UD             83    //下行T-F故障
#define R_TFBREAK_F                84    //上行T-F故障
#define R_TFBREAK_B                85    //下行T-F故障
#define MAX_REPORT_INDEX           85    //最大报告索引宏,大于该值为非法报告


//传动----------------------------------
#define RTC_COUNT     7    //传动个数

#endif