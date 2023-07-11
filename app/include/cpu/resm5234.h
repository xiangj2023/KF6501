
#ifndef _RESM5234_H_
#define _RESM5234_H_

#include "syscfg.h"

#define HW_VERSION_10		10
#define HW_VERSION_11		11
#define HW_VERSION_12		12

#define HARD_WARE_VER	HW_VERSION_12

//硬件资源定义

//CPU内部寄存器空间基址定义，使用缺省配置
#define IPSBAR_ADDRESS      0x40000000	
#define __IPSBAR			IPSBAR_ADDRESS

//定义中断向量号范围
//对于INTC0,向量号=64+中断资源号, 对于INTC1,向量号=128+中断资源号
#define MIN_I_VECT_NO		65		
#define MAX_I_VECT_NO		192

//外部中断定义
#define BASE_CAN_IRQ		3
#define EXTERN_CAN_IRQ		4	//扩展CAN与扩展以太网共享该中断资源
#define ADC_IRQ				6	//下降沿有效

//定义串口最大个数
#define MAX_SERIAL_NUM		(2+1)


//串口硬件配置定义
#define MCF5234_UART		YES
#define MCF5235_FLEXCAN		YES
#define MCF5234_SJA1000		YES

//装置模块类型定义
#define MODULE_TYPE			MD_KV3523

//定义内部系统时钟频率，即CPU内核频率的一半
#define IN_SYS_CLOCK		75000000	/* System clock speed 75 MHz */

//系统资源定义，只定义各个部分的片选基址
#define SDRAM_BASE			0x80000000		//SDRAM，16M字节，32位
#define CODE_FLASH_BASE		0x00000000		//程序FLASH，4M字节，16位
#define DATE_FLASH_BASE		0x10000000		//参数FLASH，4M字节，16位
#define ADC_CS_BASE			0x30000000
#define ADC1_CS_BASE		0x30050050		//ADC1，1字，16位	
#define ADC2_CS_BASE		0x300500a0		//ADC2，1字，16位			
#define ADC3_CS_BASE		0x300500C0		//ADC3，1字，16位			
#define EX_NET_CS_BASE		0x90000000		//扩展以太网，16位
#define EX_CAN_CS_BASE		0xa2000000		//扩展CAN网，128字节，8位
#define EEPROM_PRO			0xa2030055		//EEPROM写保护，0禁止，1允许，1字节
#define WATCHDOG_BASE		0xa203005a		//看门狗刷新
#define EXTER_IF_BASE		0xb0000000
#define DOA_CS_BASE			0xb0050000		//YKA，写遥控及读内部遥信、反校，32字节，8位
#define DOB_CS_BASE			0xb00500a0		//YKB，保护动作，32字节，8位
#define DOC_CS_BASE			0xb0050060		//YKC，写遥控及读内部遥信、反校，32字节，8位
#define DOD_CS_BASE			0xb00500c0		//YKD，保护动作，32字节，8位

#define DBASE_NVRAM_SIZE      	0x80000 

#define DBASE_NVRAM_LENGTH      0x60000 
#define DBASE_NVRAM_BASE        (0x20000000)     
#define	DBASE_NVRAM_END			(DBASE_NVRAM_BASE + DBASE_NVRAM_LENGTH)

#define	DBASE_NVRAM_DATA		0x20060000			//用于电度等动态数据
#define	DBASE_NVRAM_CI			DBASE_NVRAM_DATA	//电度



#define	DBASE_NVRAM_6520			0x20070000			//用于6520装置负荷监测
#define DBASE_NVRAM_6520_LENGTH     512 
#define	DBASE_NVRAM_6520_END		(DBASE_NVRAM_6520 + DBASE_NVRAM_6520_LENGTH)

#define LOGIC_NVRAM_BASE (DBASE_NVRAM_6520_END+0xFF)
#define LOGIC_NVRAM_LENGTH 0xFF
#define LOGIC_NVRAM_END (LOGIC_NVRAM_BASE+LOGIC_NVRAM_LENGTH)


#define FL_NAVRAM_BASE (LOGIC_NVRAM_END)
#define FL_NAVRAM_LENGTH  1536
#define FL_NAVRAM_END (FL_NAVRAM_BASE+FL_NAVRAM_LENGTH)

//#define	DBASE_NVRAM_6520		0x20078000			//用于6520装置负荷监测

/*
#define EEPROM_RELAY_EVENT      			0		
#define EEPROM_RELAY_SINGAL     			(111*1024)		
#define EEPROM_SYSPARA_LCDCTLPAR        	(111*1024+512)	//液晶背光调节参数，4字节，当前辉度及是否自动调节
#define EEPROM_RELAY_DATA					(112*1024)
*/
	//=======================EEPROM的地址空间划分===============================
	// ************************系统参数区********************
#define 	EEPROM_SYSPARA_START     	(0)			//起始
#define 	EEPROM_SYSPARA_LENGTH     	(2*1024)	//长度
	
#define 	EEPROM_SYSPARA_LCDCTLPAR    (EEPROM_SYSPARA_START)	//液晶背光调节参数，4字节，当前辉度及是否自动调节
#define		EEPROM_BASICCONFIG_START	(EEPROM_SYSPARA_START+4)	//系统配置信息，包括IP地址，网络地址等
	
#define		EEPROM_DI_FILTERTIME		(EEPROM_SYSPARA_START+512)
#define		EEPROM_DI_FILTERTIME_SIZE	(512)  		//DI的消颤周期长度
		
#define		EEPROM_DISTINDEX_START		(EEPROM_DI_FILTERTIME+EEPROM_DI_FILTERTIME_SIZE)	
#define		EEPROM_DISTINDEX_SIZE		(1024)  	//录波索引文件
	
	// ************************系数区*************************
#define     EEPROM_RELAY_CFBASE			(EEPROM_SYSPARA_START+EEPROM_SYSPARA_LENGTH) //起始
#define     EEPROM_RELAY_CFSIZE			(2*1024)	//长度
	
	// ************************保护定值类数据区***************
#define 	EEPROM_RELAY_DATA			(EEPROM_RELAY_CFBASE+EEPROM_RELAY_CFSIZE)	//起始	       			
#define 	EEPROM_RELAY_DATASIZE		(28*1024)	//长度
	
	// ************************保护事项数据区*****************
#define 	EEPROM_RELAY_EVENT       	(EEPROM_RELAY_DATA+EEPROM_RELAY_DATASIZE)//起始
#define 	EEPROM_RELAY_EVENTSIZE      (sizeof(TFileHead)+sizeof(TRelayEventPtrList)+\
													sizeof(TRelayActionEv)*MAX_RACT_NUM+sizeof(TRelayAlarmEv)*MAX_RALM_NUM+1024)
													
	// ************************信号类数据区******************
#define 	EEPROM_RELAY_SINGAL			(EEPROM_RELAY_EVENT+EEPROM_RELAY_EVENTSIZE)	//起始
#define 	EEPROM_RELAY_SINGALSIZE		(1*1024)	//长度

#define	RELAY_RAM_BASE		0xA0008000		//定义保护用内部RAM起始地址
#define RELAY_RAM_LEN		0x8000			//定义保护用内部RAM长度

//判断装置上电或复位类型判断标志(在CPU内部RAM中定义)
#define SYS_START_FLAG_ADD	(RELAY_RAM_BASE+RELAY_RAM_LEN-4)

//系统启动时，判断程序是否自动进入bootrom的标志位置，可以在bootrom中进行修改，掉电丢失
#define SYS_BROM_FLAG_ADD	(RELAY_RAM_BASE+RELAY_RAM_LEN-8)

//中断资源定义
#define EXT_CAN_INT			MCF_INTC0_LVL_LVL4	//扩展以太网或扩展CAN网中断
#define ADC_INT				MCF_INTC0_LVL_LVL6	//采样中断，全部ADC采样完成后产生中断
#define LOW_POWER_INT		MCF_INTC0_LVL_LVL7	//系统供电电压低保护

//定义时钟芯片类型
#define R8025				1
#define CLOCK_MODULE		R8025


//定义串行EEPROM类型
#define AA1024				1
#define AA512				2
#define EEPROM_MODULE		AA1024

//定义AD采样芯片类型
#define MAX11046_DEV			1
#define AD7656_DEV			2
#define AD7606_DEV			3

//定义系统频率监测路数
#define MAX_FREQ_NUM		2

//定义应用程序长度范围
#define MOST_CODE_FILE_LEN		0x380000
#define LEAST_CODE_FILE_LEN		0x100000

#if(HARD_WARE_VER == HW_VERSION_10)
	//一次样机面板资源定义
	#define LED1_CS_BASE		0xb0030050		//面板LED，4字节，8位，32个灯
	#define LED2_CS_BASE		0xb0030051		//面板LED，4字节，8位，32个灯
	#define LED3_CS_BASE		0xb0030052		//面板LED，4字节，8位，32个灯
	#define LED4_CS_BASE		0xb0030053		//面板LED，4字节，8位，32个灯
	#define KEY1_CS_BASE		0xb0030054		//面板KEY，2字节，8位
	#define LCD_CS_BASE			0xb0030000		//面板LCD，2字节，8位

	//定义LED

	//定义液晶控制器硬件操作地址
	#define LCD_DATA_REG		0xB0030057		//读写数据寄存器
	#define LCD_CMD_REG			0xB0030056		//读写命令寄存器
	#define LCD_RESET_ADDR		0xB0030055		//复位LCD，电背景灯

	//定义使用的AD芯片
	#define AD_DEV_TYPE			MAX11046_DEV

	#define BIA_CS_BASE			0xb00a0055		//YXA，5字节，8位,序号颠倒
	#define BIB_CS_BASE			0xb00a00a0		//YXB，5字节，8位

#elif(HARD_WARE_VER == HW_VERSION_11 ||HARD_WARE_VER == HW_VERSION_12)

	//二次样机面板资源定义
	//指示灯定义(只写)
	#define LED1_CS_BASE		0xb0030052		//面板LED，4字节，8位，32个灯
	#define LED2_CS_BASE		0xb0030053		
	#define LED3_CS_BASE		0xb0030054		
	#define LED4_CS_BASE		0xb0030055		
	//键盘定义(只读)
	#define KEY1_CS_BASE		0xb0030052		//面板KEY，2字节，8位
	#define KEY2_CS_BASE		0xb0030053		
	//定义液晶控制器硬件操作地址
	#define LCD_DATA_REG		0xC0050001		//读写数据寄存器
	#define LCD_CMD_REG			0xC0050000		//读写命令寄存器
	#define LCD_RESET_ADDR		0xC0050006		//复位LCD(0x5x)
	#define LCD_BACK_LIGHT		0xC0050007		//液晶背光控制(0xx5开，其它关)

	//定义使用的AD芯片
	#define AD_DEV_TYPE 		AD7606_DEV//AD7656_DEV////AD7656_DEV//

	#define BIA_CS_BASE 		0xb00a0050		//YXA，5字节，8位,序号为正
	#define BIB_CS_BASE 		0xb00a00a0		//YXB，5字节，8位
	
	//定义AD7606操作地址,与看门狗同一个片选，8位口
	#define AD7606_BASE 	0xa2030090		//看门狗刷新

#endif


//临时定义系统任务优先级
/****************************************************************************
	常用操作系统优先级顺序说明
	PSOS:		0~255，优先级序号越大级别越高；
	vxworks:	0~255，优先级序号越小级别越高；
	UCOSII:		0~63,优先级序号越小级别越高;
*****************************************************************************/
#if OS_PLATFORM == OS_VXWORKS

		//采样任务
#define RELAY_PRIORITY		50
#define WDOG_PRIORITY		65
		
		//常用端口链路守护任务
#define MISI_PRIORITY		150
#define CAN_PRIORITY		145
#define ETHERNET_PRIORITY	146
		
#define DI_PRIORITY 		170
#define AI_PRIORITY 		180
#define BO_PRIORITY 		185

#define MAPBIT_PRIORITY 	190
#define NETPRO_PRIORITY 	191
#define REMOTE_PRIORITY		192
#define SHELL_PRIORITY		193
#define DB_PRIORITY 		199
#define LCD_PRIORITY		200
#define BACKUP_PRIORITY 	201

		//定义缺省任务堆栈
#define TASK_STACK_LEN		(1024*10)
	
#endif	

#endif 
