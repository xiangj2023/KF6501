//=======================================================================================
// 名称: Siec104.h
// 功能: IEC104从站规约类的定义
// 编写：lz 2011-12-23
// 改写：
//=======================================================================================

#ifndef _IECDEFS_H
#define _IECDEFS_H

#include "sysdefs.h"	/*系统数据类型定义及常用宏定义*/

/*---------------------宏定义区-----------------------*/
#define		PBO_SELECT			1		//遥控选择
#define	 	PBO_EXECUTE			2		//遥控执行
#define	  	PBO_CANCEL 			3		//遥控撤销
#define		PBO_DIRECT			4		//遥控直接执行
#define		PBO_SUCESS  		0		//遥控成功
#define		PBO_FAIL			1		//遥控失败
				
#define     PBO_NULL			0		//不用
#define 	PBO_OPEN			1		//控分
#define 	PBO_CLOSE			2		//控合
	
#define		PBOINFOSIZE			4		//遥控保留信息字节数

//240-255,通用分类功能
#define INF_S_GEN_RGROUPITEM   240   //读所有被定义组的标题
#define INF_S_GEN_RGROUPVALUE  241   //读一个组的全部条目的值或属性
#define INF_S_GEN_RENTRYITEM   243   //读单个条目的目录
#define INF_S_GEN_RENTRYVALUE  244   //读单个条目的值或属性
#define INF_S_GEN_CALLALL      245   //对通用分类数据的总查询
#define INF_S_GEN_WENTRY       248   //写条目
#define INF_S_GEN_WENTRYACK    249   //带确认的写条目
#define INF_S_GEN_WENTRYEXEC   250   //带执行的写条目
#define INF_S_GEN_WENTRYCANCEL 251   //写条目终止

//240-255,通用分类功能
#define INF_M_GEN_RGROUPITEM   240   //读所有被定义组的标题
#define INF_M_GEN_RGROUPVALUE  241   //读一个组的全部条目的值或属性
#define INF_M_GEN_RENTRYITEM   243   //读单个条目的目录
#define INF_M_GEN_RENTRYVALUE  244   //读单个条目的值或属性
#define INF_M_GEN_CALLALL      245   //对通用分类数据的总查询
#define INF_M_GEN_WENTRY       248   //写条目
#define INF_M_GEN_WENTRYACK    249   //带确认的写条目
#define INF_M_GEN_WENTRYEXEC   250   //带执行的写条目
#define INF_M_GEN_WENTRYCANCEL 251   //写条目终止

//GDD中的数据类型  定值用
#define GDD_DATATYP_NULL	0	//无数据
#define GDD_DATATYP_OS8		1	//ASCII8位码
#define GDD_DATATYP_UI		3	//无符号整数
#define GDD_DATATYP_I		4	//整数 
#define GDD_DATATYP_UF		5	//无符号浮点数
#define GDD_DATATYP_F		6	//浮点数
#define GDD_DATATYP_R32_23	7	//IEEE标准754短实数

//==== 通用分类数据定义 ====

//描述的类别(KOD)
#define KOD_0                  0     //无所指定的描述类别
#define KOD_1                  1     //实际值
#define KOD_2                  2     //缺省值
#define KOD_3                  3     //量程
#define KOD_5                  5     //精度
#define KOD_6                  6     //因子
#define KOD_7                  7     //参比
#define KOD_8                  8     //列表
#define KOD_9                  9     //量纲
#define KOD_10                 10    //描述
#define KOD_12                 12    //口令条目
#define KOD_19                 19    //相应的功能类型和信息序号
#define KOD_23                 23    //相关联的条目

//数据类型
#define DAT_1                  1     //ASCII8位码
#define DAT_2                  2     //成组8位串
#define DAT_3                  3     //无符号整数
#define DAT_4                  4     //整数
#define DAT_5                  5     //无符号浮点数
#define DAT_6                  6     //浮点数
#define DAT_7                  7     //IEEE754短实数
#define DAT_8                  8     //IEEE754实数
#define DAT_9                  9     //双点信息
#define DAT_10                 10    //单点信息
#define DAT_12                 12    //带品质描述的被测值
#define DAT_14                 14    //二进制时间
#define DAT_15                 15    //通用分类标识序号
#define DAT_16                 16    //相对时间
#define DAT_17                 17    //功能类型和信息序号
#define DAT_18                 18    //带时标的报文
#define DAT_19                 19    //带相对时标的报文
#define DAT_20                 20    //带相对时标的被测值
#define DAT_21                 21    //外部文本序号
#define DAT_22                 22    //通用分类回答码
#define DAT_23                 23    //数据结构
#define DAT_24                 24    //索引

//********************	结构定义区	**********************

struct TBO{
	WORD Flag;		//任务状态	PBO_NEW, PBO_BUSY, PBO_OK, PBO_NUL
	WORD No;		//遥控路号
	WORD Command;	//动作 PBO_SELECT PBO_EXECUTE  PBO_CANCEL PBO_DIRECT
	WORD Attrib;	//状态（合分）PBO_NULL PBO_OPEN, PBO_CLOSE,

	WORD NeedRetFlag; //是否需要遥控返校信息标志
	WORD RetResult; //遥控返校状态 PBO_SUCESS  PBO_FAIL
	BYTE Info[20];	//保留，遥于保留特殊信息
};

struct TST
{
	WORD	Group;	 
	WORD	No; 	
	WORD	Command;	
	WORD	Attrib; 
	WORD	Num;	  //数据个数
	WORD	Offset;   //偏移量
	WORD	Result;   //返回结果
	PBYTE	Buf;  
};//定值 


#pragma	pack(1)

//通用分类标识序号结构定义
typedef struct
{
	BYTE byGroup;               //组
	BYTE byEntry;               //条目
}TGin;

//后续数据结构
typedef struct
{
	BYTE Cont:1;                //后续状态位
	BYTE Number:7;              //数目
}TNext;

//通用分类数据描述结构
typedef struct
{
	BYTE byDataType;            //数据类型
	BYTE byDataSize;            //数据宽度
	BYTE byDataNum;
	//	TNext Next;                 //后续数据
}TGdd;

//通用分类数据集数目
typedef struct
{
	BYTE Cont:1;                //后续状态位
	BYTE Count:1;				//相同RII的一位计数 0<-->1交替
	BYTE Number:6;              //数目
}TNgd;

//通用分类数据结构
typedef struct
{
	TNgd Ngd;					//通用分类数据集数目
	TGin Gin;                   //通用分类标识序号
	BYTE byKod;                 //描述的类别
	TGdd Gdd;                   //通用分类数据描述
	BYTE byGid;                 //通用分类标识数据
}TGenData;

struct TGenItem
{
	TGin Gin;                   //通用分类标识序号
	BYTE byKod;                 //描述的类别
	TGdd Gdd;                   //通用分类数据描述
	BYTE byGid;                 //通用分类标识数据
};

//定义处理时需要存储的信息结构
typedef struct
{
	BYTE byOldFCB;          //上次正确接收后保存的FCB位
	BYTE byLastCnt;			//通用数据计数标记		
	WORD byRII;				//遥控返回结果
	BYTE byDCC;
	BYTE byCallDDSQ;		//召唤电度顺序号	
	BYTE byScanNumber;      //扫描序号
	BYTE byCOL;             //兼容级别
	BYTE byGRC;             //GRC
}TIECInfo;

#pragma pack()

typedef union{
	TRelayAlarmEv 	tAlram;
	TRelayStartEv 	tStart;
	TRelayActionEv 	tAction;
}TRelayReport;

#endif
