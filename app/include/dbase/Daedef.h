/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      paradefs.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义运行环境相关的数据结构																*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      李政																							*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*    	TSysConfigTable						系统配置表													*/
/*		TStatus								系统状态													*/
/*		TMCBH								静态内存分配												*/
/*		TRecordWrap							运行记录库													*/
/*		TRecordBlock						运行记录定义												*/
/*		TMiscWrap							操作记录库													*/
/*		TEventMisc							操作记录定义												*/
/*		TAppInfo							数据库任务表												*/
/* 函数																									*/
/*																										*/
/*      None																							*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*		Dbasedef.h						数据库结构定义													*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
/********************************************************************************************************/
#ifndef __DAEDEFS_H
#define __DAEDEFS_H

#include "Dbasedef.h"		/*数据库定义*/

//系统自检状态
#define		SS_INTERNAL_RAM_ERR			(0)    	//内部RAM异常
#define		SS_EETERNAL_RAM_ERR			(1)    	//外部RAM异常
#define		SS_PROGRAM_ROM_EERR			(2)    	//程序ROM异常
#define		SS_NVRAM_ERR				(3)    	//NVRAM测试出错
#define		SS_FLASH_ERR				(4)   	//FLASH测试出错
#define		SS_EEPROM_ERR				(5)    	//EEPROM出错
#define		SS_AD_ERR					(6)    	//AD采样异常
#define		SS_LCD_ERR					(7)    	//液晶显示故障
#define		SS_INSULATION_ERR			(8)    	//输出光隔击穿
#define		SS_INSULATION_INV			(9)    	//输出光隔失效
#define		SS_INSULATION_ALM			(10)   	//光隔失电告警 
#define		SS_CONTROL_LOOP_BREAK		(11)   	//控制回路断线
#define		SS_PT_BREAK					(12)   	//PT断线
#define		SS_CT_BREAK					(13)   	//CT断线
#define		SS_CLOCK_ERR				(14)   	//时钟故障
#define		SS_PPWER_ERR				(15)   	//电源故障
#define		SS_DSP_ERR					(16)   	//DSP故障

#define		SS_TIME_B					(17)    //B格式对钟异常
#define		SS_TIME_GPS					(18)    //GPS时钟故障
#define		SS_ETHERNET_ERR				(19)    //以太网通讯故障
#define		SS_CAN_ERR					(20)    //设备通讯故障
#define		SS_PARA_CHANGE				(21)    //参数文件改变                    
#define		SS_DOWNLOAD_PARA			(22)    //需要下装参数
#define		SS_UPLOAD_PARA				(23)    //需要上装参数                  
#define		SS_PARA_ERR					(24)    //参数文件错
#define		SS_PARA_ROM_ERR				(26)    //参数ROM异常
#define		SS_CANBUS_ERR				(27)    //CANBUS网故障
#define		SS_PARTNER_ERR				(28)    //备机故障
#define		SS_COLD_RESET				(29)    //系统冷启动 
#define		SS_NETACOMM					(30)    //A网通信异常 
#define		SS_NETBCOMM					(31)    //B网通信异常 
#define 	SS_12V_ERR					(32)	//12V电源异常

#define		SS_SYS_HARD_ERR				(40)    //硬件
#define		SS_SYS_CONFIG_ERR			(41)    //系统配置错
#define		SS_SYS_STATUS_ERR			(42)    //系统状态错
#define		SS_NVRAM_LINK_ERR			(43)    //NVRAM链表错
#define		SS_NVRAM_INIT				(44)    //NVRAM初始化

#define		SS_FILE_BASICPARA_ERR		(64)	//基本参数文件错误
#define		SS_FILE_RELAYDISP_ERR		(65)	//保护描述文件错误
#define		SS_FILE_RELAYDATA_ERR		(66)	//保护数据文件错误
#define		SS_FILE_RELAYEVENT_ERR		(67)	//保护事件文件错误
#define		SS_FILE_BASICEVENT_ERR		(68)	//常规事件文件错误

#define		SS_FILE_BASICPARA_CHG		(72)	//基本参数文件修改
#define		SS_FILE_RELAYDISP_CHG		(73)	//保护描述文件修改
#define		SS_FILE_RELAYDATA_CHG		(74)	//保护数据文件修改
#define		SS_FILE_RELAYEVENT_CHG		(75)	//保护事件文件修改
#define		SS_FILE_BASICEVENT_CHG		(76)	//常规事件文件修改


#define		SS_FILE_DIFFRELAYDATA_ERR	(80)	//保护数据个数不一致

#define		SS_RAM_SW_ERR				(96)    //RAM软压板出错
#define		SS_RAM_CFG_ERR				(97)    //RAM配置字出错
#define		SS_RAM_SET_ERR				(98)    //RAM定值出错
#define		SS_RAM_SYSSET_ERR			(99)    //RAM辅助定值出错
#define		SS_RAM_CF_ERR				(100)    //RAM系数出错
#define		SS_RAM_DATA_ERR				(111)    //RAM数据出错

#define		SS_NVRAM_MISC_ERR			(112)    //NVRAM操作记录数据错
#define		SS_NVRAM_RECORD_ERR			(113)    //NVRAM运行记录库出错
#define		SS_NVRAM_BURSTBI_ERR		(114)    //NVRAM变位遥信出错
#define		SS_NVRAM_EVENTBI_ERR		(115)    //NVRAM SOE 出错
#define		SS_NVRAM_RSTTWRAP_ERR		(116)    //NVRAM 保护启动报文出错
#define		SS_NVRAM_CI_ERR				(117)    //NVRAM运行记录库出错
#define		SS_NVRAM_RALMWRAP_ERR		(118)    //NVRAM告警报告异常
#define		SS_NVRAM_RACTWRAP_ERR		(119)    //NVRAM动作报告异常

#define		SS_EEPROM_SW_ERR			(128)    //EEPROM软压板出错
#define		SS_EEPROM_CFG_ERR			(129)    //EEPROM配置字出错
#define		SS_EEPROM_SET_ERR			(130)    //EEPROM定值出错
#define		SS_EEPROM_SETAREA_ERR		(131)    //EEPROM定值区号出错
#define		SS_EEPROM_SYSSET_ERR		(132)    //EEPROM辅助定值出错
#define		SS_EEPROM_CF_ERR			(133)    //EEPROM系数出错
#define		SS_EEPROM_RACTWRAP_ERR		(134)    //
#define		SS_EEPROM_RALMWRAP_ERR		(135)    //

#define		SS_EEPROM_DATA_ERR			(136)    //EEPROM数据出错
#define		SS_EEPROM_WRITE_ERR			(137)    //EEPROM写入出错

#define		SS_MAPBIT_PARA_ERR			(256)    //镜像位参数出错
#define		SS_MAPBIT_FUNC_USE			(257)    //镜像位功能投入
#define		SS_MAPBIT_COMM_ERR			(264)    //镜像位对象通信失败(32位)

#define		SS_FL1_COMM_ERR			(296)	 //测距1通信失败
#define		SS_FL2_COMM_ERR			(297)	 //测距2通信失败
#define		SS_FL3_COMM_ERR			(298)	 //测距3通信失败
#define		SS_FL4_COMM_ERR			(299)	 //测距4通信失败
#define		SS_FL5_COMM_ERR			(300)	 //测距5通信失败

#define		SS_PORT_COMM_ERR		(320)	 //端口通信状态(预留16个)
#define		SS_ETHA_COMM_ERR		(320)	 //以太网A通信失败(预留5个连接)
#define		SS_ETHB_COMM_ERR		(325)	 //以太网B通信失败(预留5个连接)
#define		SS_SER_COMM_ERR			(330)	 //串口通信失败(预留4个串口)
#define		SS_CAN_COMM_ERR			(334)	 //CAN1通信失败(预留2个CAN口)

#define	DAE_MAX_FLAGS			512

typedef struct 
{
    DWORD Flag[ DAE_MAX_FLAGS/32 ];  //
}DAEFLAGS ; 

#define	SYS_RUNNING_OK		0x494E4954				//"INIT"

//系统运行状态
typedef struct
{
	DWORD					dwFlag;					//运行标志		
	TAbsTime				stTime;					//系统时间		
	DAEFLAGS				Status;					//运行状态
	DAEFLAGS				InitStatus;				//装置初始化状态
	DWORD					dwNetAMask;				//A网数据掩码		
	DWORD					dwNetBMask;				//B网数据掩码		
	DWORD					dwRelayFaultNo;			//保护故障编号
	DWORD					dwRsv[8];				//备用				
}TStatus;

//NvRAM内存前缀
#define	MCB_SIGN_ALLOC			0x4E474953				//"SIGN"
#define	MCB_SIGN_FREE			0x5453414c				//"LAST"

typedef struct
{
	DWORD					dwSign;					//数据标志	=MCB_SIGN_ALLOC  已分配
													//			=MCB_SIGN_LAST  最后一块
													//			=其他  无效
	DWORD					dwSize;					//内存块长度	
}TMCB;

//NvRAM内存分配描述
#define	MMLNK_SIGN_VALID		0x54494E49				//"INIT"

typedef struct
{
	DWORD					dwSign;					//有效标志		=0xAAAA有效，其他无效
	DWORD					dwSize;					//空闲内存长度	
	BYTE*					pFree;					//空闲内存指针	
	TMCB*					pAlloced;				//已分配内存指针	
}TMemLink;


#define WRAP_INIT_FLAG		0x54494E49				//"INIT"

//运行记录库
#define	ES_MAX_LINE			254
#define MAX_RUNREC_NUM		256	

typedef struct
{
	WORD					nCharSize;					//记录长度,本条记录的长度
	char					szRecordMsg[ES_MAX_LINE];	//记录体 
}TRecordBlock;


typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TRecordBlock			pEvent[MAX_RUNREC_NUM];		//事项缓存			
}TRecordWrap;


//操作记录库
#define MAX_MISC_NUM			256	

#define MISC_CLASS_RESET		1   //系统复位事件
#define MISC_CLASS_DOWNFILE		2	//下装文件
#define MISC_CLASS_COMM			3	//通讯中断
#define MISC_CLASS_BO			4	//遥控操作
#define MISC_CLASS_AO			5	//遥调操作
#define MISC_CLASS_SYNC			6	//同期操作
#define MISC_CLASS_SET			7	//更改定值
#define MISC_CLASS_SETAREA		8	//更改定值区号
#define MISC_CLASS_SW			9	//更改软压板
#define MISC_CLASS_CFG			10	//更改配置
#define MISC_CLASS_CF			11	//更改系数
#define MISC_CLASS_PARA			12	//修改参数
#define MISC_CLASS_POWER		13	//装置掉电
#define MISC_CLASS_CLEARCI		14  //电度底数清零
#define MISC_CLASS_MODIAI		15	//修改遥测
#define MISC_CLASS_MODIBI		16	//修改遥信
#define MISC_CLASS_MODICI		17	//修改电度
#define MISC_CLASS_CHANNELSRC	18	//投退通道类型
#define MISC_CLASS_ZERODRIFT	19	//更改零漂
#define MISC_CLASS_FULLVALUE	20	//更改满值
#define MISC_CLASS_AOCOEF		21	//模拟量输出整定

typedef struct
{
	WORD					nClass;						//操作类别	
	TAbsTime				absTime;					//操作时间	
	DWORD					dwDevName;					//操作对象	发生操作事件的设备名
	DWORD					dwOperator;					//操作源		操作任务号或端口号
	DWORD					dwValue;					//操作值		
	DWORD					dwAttr;						//操作属性	
}TMiscEvent;

typedef struct
{
	DWORD					dwMagic;					//初始化标志,有效标志WRAP_INIT_FLAG
	DWORD					hSem;						//信号量ID,共享资源保护
	WORD					nMax;						//缓冲大小		
	WORD					nHeadIndex;					//头指针			
	WORD					nTailIndex;					//尾指针			
	TMiscEvent				pEvent[MAX_MISC_NUM];		//事项缓存			
}TMiscWrap;

/*----------------------------------------------------------------------------------------*/
/* 定义任务入口映射表项                                                                   */
/*----------------------------------------------------------------------------------------*/
#define MAX_TASK_NAME 		8
#define DEFINE_APPLICATION(name,entry,pri,sstack)      {name,(TTASK_ENTRY)entry,pri,sstack}

typedef struct {
    char          			szName[MAX_TASK_NAME];      /*任务名称*/
    TTASK_ENTRY   			fpEntry;                  	/*该类任务对应的任务入口*/
    WORD          			wPriority;            	 	/*对应任务的优先级*/
    WORD          			wSysStack;             		/*创建该任务时需要的系统堆栈大小*/
}TAppItem;

//Lcd、Dbase、Misi、WatchDag、AI、BI、BO等
//数据库任务表
#define SYS_PORT_MAX       				16
#define SYS_PORTID_AI       			((SC_PORT_SYSTEM<<8)+1)
#define SYS_PORTID_DI       			((SC_PORT_SYSTEM<<8)+2)
#define SYS_PORTID_BO       			((SC_PORT_SYSTEM<<8)+3)
#define SYS_PORTID_DBASE        		((SC_PORT_SYSTEM<<8)+4)
#define SYS_PORTID_SERADMIN     		((SC_PORT_SYSTEM<<8)+5)
#define SYS_PORTID_CANADMIN     		((SC_PORT_SYSTEM<<8)+6)
#define SYS_PORTID_ETHMAIN     			((SC_PORT_SYSTEM<<8)+7)
#define SYS_PORTID_WATCHDOG          	((SC_PORT_SYSTEM<<8)+8)
#define SYS_PORTID_RELAY          		((SC_PORT_SYSTEM<<8)+9)
#define SYS_PORTID_BACKUP          		((SC_PORT_SYSTEM<<8)+10)
#define SYS_PORTID_MAPBIT          		((SC_PORT_SYSTEM<<8)+11)
#define SYS_PORTID_LCD        			((SC_PORT_SYSTEM<<8)+12)    
#define SYS_PORTID_FL        			((SC_PORT_SYSTEM<<8)+13)    

#define	APP_STATUS_PORTSTATUSSHIFT		0
#define	APP_STATUS_PORTSTATUSMASK		0x00000003
#define	APP_STATUS_ACTIVEMASK			0x00000080

#define	APP_STATUS_MAIN					0
#define	APP_STATUS_WARMBACK				1
#define	APP_STATUS_COLDBACK				2

/*定义端口事件标志*/
#define APP_EVENT_ALL     				(0xFFFF)
#define APP_EVENT_AIE     				(1<<0)     /* 有遥测事件*/
#define APP_EVENT_AIF     				(1<<1)     /* 有冻结遥测*/
#define APP_EVENT_SOE     				(1<<2)     /* 有SOE*/
#define APP_EVENT_BIE     				(1<<3)     /* 有变位遥信*/
#define APP_EVENT_CIF     				(1<<4)     /* 有冻结电度*/
#define APP_EVENT_OIE     				(1<<5)     /* 有透明数据*/
#define APP_EVENT_SPIE    				(1<<6)     /* 有带时标的SPI*/
#define APP_EVENT_RALM    				(1<<7)     /* 有带时标的SPI*/
#define APP_EVENT_RSTT    				(1<<8)     /* 有带时标的SPI*/
#define APP_EVENT_RACT    				(1<<9)     /* 有带时标的SPI*/
#define APP_EVENT_COMM    				(1<<10)     /* 端口通信中断*/

#define	GET_APPINFO_PORTSTATUS(s)			((s)&APP_STATUS_PORTSTATUSMASK)	

typedef struct
{
	TAppItem 				*pAppItem;					//任务描述表
	HANDLE					hTaskID;					//任务标识			
	DWORD					dwPortID;					//端口号				
	DWORD					bStatus;					//工作状态,D0~D3端口状态
														//			=0 主端口                       
														//			=1 冷备用端口
														//			=2 热备用端口        

														//			D7
														//			=1 端口激活 
														//			=0 不激活
														//其他备用
	DWORD					bMask;						//数据掩码,可访问数据权限
	DWORD					bEvent;						//事项标志字,发生事项的标志信息
	WORD					nBIERead;					//变位遥信读指针		
	WORD					nSOERead;					//SOE读指针			
	WORD					nRSTTRead;					//保护启动报告读指针	
	WORD					nRALMRead;					//保护告警报告读指针	
	WORD					nRACTRead;					//保护动作报告读指针	
	WORD					nAIERead;					//AIE记录读指针			
	WORD					nLogRead;					//运行记录读指针		
	WORD					nMISCRead;					//操作记录读指针			
}TAppInfo;

#define DAE_MAX_APP_NUM			32

//环境配置表
typedef struct
{
	TStatus					SystemStatus;				//系统状态	
	TMemLink				NvRamLink;					//静态内存分配
	TRecordWrap				RecordWrap;					//运行记录	
	TMiscWrap				MiscWrap;					//操作记录	
	TAppInfo				TaskApp[DAE_MAX_APP_NUM];   //数据库任务表
	TDBaseConfigTable		DBaseConfig;				//数据库配置表
}TSysConfigTable;

#define	DB_BLANK			0xFFFF

#define BAISIC_INIT_FLAG		0x494E4954				//"INIT"
//基本配置表
typedef struct
{
	DWORD	dwInitFlag;			//初始化标志
	DWORD	dwNetAIP;			//A网IP地址
	DWORD	dwNetAMask;			//A网IP掩码
	DWORD	dwNetAGWIP;			//A网网关IP地址
	DWORD	dwNetBIP;			//A网IP地址
	DWORD	dwNetBMask;		//A网IP掩码
	DWORD	dwNetBGWIP;			//A网网关IP地址
	WORD	wNetAddress;		//组网地址
	char	chPassWord1[8];		//密码1					
	char	chPassWord2[8];		//密码2					
	WORD	wCheck;				//校验
}TBasicConfig;

#define DAE_MAX_TRANSLEN		1024
#define DAE_MAX_TRANSNUM		16

//透明数据区定义
typedef struct
{
	WORD  	wClass;		//类型
	WORD	wDesPort;	//目的端口号，
	WORD	wSrcPort;	//源端口号，发起申请的一端
	WORD	wDataLen;	//数据长度
	BYTE	Data;		//数据区
}TTransDataHead;

typedef union
{
	TTransDataHead tHead;
	BYTE	byDataBuf[DAE_MAX_TRANSLEN];
}TFixTransData;

typedef struct
{
	WORD					nTailIndex;						//尾指针			
	TFixTransData			pEvent[DAE_MAX_TRANSNUM];		//事项缓存			
}TTransWrap;

#endif
