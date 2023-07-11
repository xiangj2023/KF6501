/****************************************************************************************/
/*                                                                                  	*/
/* 文件名称                                           版本号                        	*/
/*                                                                                  	*/
/*      syscfg.h	                                     1.0                          	*/
/*                                                                                  	*/
/* 版权                                                                                 */
/*      天津凯发电气股份有限公司                                                       	*/
/*                                                                                  	*/
/* 描述                                                                             	*/
/*                                                                                  	*/
/*      该文件对系统软件的各个功能模块进行定义和裁剪配置                               	*/
/*                                                                                  	*/
/* 修改历史                                                                         	*/
/*                                                                                  	*/
/*      修改人           修改日期       		修改内容                                */
/*                                                                                  	*/
/*      zqz      		 08/03/21			初始创建,版本号1.0                          */
/*                                                                                  	*/
/****************************************************************************************/

#ifndef _SYSCFG_H_
#define _SYSCFG_H_

#define 	_SYS_KF6500_		1
#define 	_SYS_KF1300_		2

#define 	SYSTEM_TYPE			_SYS_KF6500_

#define YES                     1
#define NO                      0

/********************************************************************************/
/*	调试信息输出端口类型定义													*/
/*  __TO_CONSOLE__	输出信息到标准控制台										*/
/*  __TO_NVRAM__ 	输出信息到系统NVRAM存储区，通过维护软件可以进行查看    		*/
/*  __TO_NULL__ 	空输出												    	*/
/********************************************************************************/
#define __TO_CONSOLE__			1		
#define __TO_NVRAM__			2
#define __TO_ALL__             	3
#define __TO_NULL__             4

/*------------------------------------------------------------------------------*/
/*  _DMESSAGE_OUTPUT 用于配置系统运行异常信息的输出控制台，包括ASSERT、TRACE、  */
/*  ReportMsg、LogError等记录异常信息的函数的输出。                             */
/*------------------------------------------------------------------------------*/
#define _MESSAGE_OUTPUT			__TO_ALL__//__TO_NVRAM__//__TO_CONSOLE__

/*---------------------------------------------------------------------------------*/
/* 调试所有消息                                                                    */
/*---------------------------------------------------------------------------------*/
#define DISP_ALL_MESSAGE        0xffff

/*----------------------------------------------------------------------------------*/
/*  __DEBUG 为系统总的调试开关，只有选定该选项，才能选择系统其它的调试跟踪功能。设置*/
/*  为YES，系统的ASSERT 诊断宏、TRACE宏、DBG_REPORT宏有效 							*/
/*----------------------------------------------------------------------------------*/
#define __DEBUG__               YES

#define __DEBUG_MEMORY_T		NO		/*任务内存交换区调试*/
#define __DEBUG_MESSAGE_T		NO		/*消息调度调试*/
#define __DEBUG_LOGERROR_T		NO		/*LOGERROR调式*/
#define __DEBUG_NVRAM_T			NO		/*不掉电RAM区调式*/
#define __DEBUG_COMM_T			NO		/*端口缓冲区调式*/
#define __DEBUG_TASK_T			NO		/*跟踪任务运行轨迹*/
#define __DEBUG_PROTECT			NO		/*保护功能调试*/			

#define __LOG_ERR__				YES		/*logerr信息输出开关*/
#define __REP_MSG__				YES		/*reportmsg信息输出开关*/
#define __INT_LOG__				YES		/*在vxworks中断中不能通过printf输出信息*/

/*----------------------------------------------------------------------------------*/
/*  DISP_MESSAGE 当前调试跟踪的消息，只有__DEBUG_MESSAGE_T设置为YES时，DISP_MESSAGE */
/*  才有效。=DISP_ALL_MESSAGE时，跟踪所有消息，设置为NO时不跟踪任何消息，当设置为某 */
/*  一具体定义的消息时（在message.h中定义），则只跟踪该消息                         */
/************************************************************************************/
#define DISP_MESSAGE            YES


/*---------------------------------------------------------------------------------*/
/* 操作系统类型定义                                                                */
/*---------------------------------------------------------------------------------*/
#define OS_NULL				0x00 	 /*NULL OS*/
#define OS_PSOS             0x01     /*pSOStem 操作系统*/
#define OS_UCOSII			0x02     /*UCOSII 操作系统*/
#define OS_VXWORKS          0x03     /*VxWork操作系统*/
#define OS_WIN32			0x04	
#define OS_LINUX			0x05

//当前使用的操作系统
#define OS_PLATFORM         OS_VXWORKS       

/*---------------------------------------------------------------------------------*/
/* CPU类型定义		                                                               */
/*---------------------------------------------------------------------------------*/
#define CPU_COLDFIRE		0x01
#define CPU_ARM7			0x02
#define CPU_X86				0x03
#define CPU_HC12			0x04

//当前系统使用的CPU类型
#define SYS_CPU_TYPE		CPU_COLDFIRE


//定义当前模块类型
#define MD_DEFAULT			0

#define MD_KV3510			10
#define MD_KV3511			10
#define MD_KV3512			10

#define MD_KV3520			20
#define MD_KV3521			21
#define MD_KV3522			22
#define MD_KV3523			23
#define MD_KV3530			30
#define MD_KV3531			31
#define MD_KV3532			32
#define MD_KV3533			33
#define MD_KV3560			60

#define MD_KV3581			81
#define MD_KV3582			82
#define MD_KV3583			83



#define MK_KV


/************************************************************************************/
/*        系    统    模    块    配    置                                          */
/*  配置软件功能模块，相应选项设置为YES，编译程序时会连入相应的模块，设置为NO时， 	*/
/*  相应的模块不进行编译，对应的功能也被禁止                                        */
/*----------------------------------------------------------------------------------*/
#define INSTALL_SERIAL			    YES				/*是否安装了串口驱动程序*/
#define INSTALL_NETCARD2		    YES				/*是否安装了第二路以太网驱动程序*/
#define INSTALL_CANBUS			    YES				/*是否安装了CANBUS*/
#define INSTALL_LCD					YES				/*是否安装了LCD程序*/
#define INSTALL_NET                	YES             /*是否安装了以太网管理模块*/
#define INSTALL_LAN1                YES             /*是否安装了以太网1*/
#define INSTALL_LAN2                YES             /*是否安装了以太网2*/
#define INSTALL_TFFS                NO              /*是否安装了Flash文件系统*/
#define INSTALL_PLC					NO				/*是否安装了PLC功能模块*/
#define INSTALL_IEC61850			NO				/*是否安装了61850功能模块*/

#define INSTALL_HASH				YES				/*是否安装了HASH功能模块*/

/************************************************************************************/
/*  定义系统使用的动态RAM空间地址及长度											 	*/
/*----------------------------------------------------------------------------------*/
#define SYS_DRAM_BASE		0x80000000	//系统使用的动态RAM起始地址
#define SYS_DRAM_LEN		0x1000000	//系统使用的动态RAM长度


#endif
