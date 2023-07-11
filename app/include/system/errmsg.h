#ifndef _ERRMSG_H
#define _ERRMSG_H

/**********************************************************************/
/* 定义系统调用错误信息,对于ucosii(v2.5)系统,由于1~255为错误信息定义  */
/* 为便于管理，将其进行改造，统一错误信息定义为：0x8000+errinf	      */
/* 调整后，从0x800~0x8fff为操作系统平台调用错误信息，整个系统错误信息 */
/* 应在此范围之外													  */
/**********************************************************************/

#define ERR_SUCCESS     0   		  	/*函数调用成功*/
#define ERR_FAILURE     0x1    			/*函数调用失败*/

//操作系统平台错误信息定义
#define ERR_NO_FIND_TASK      	0x8100  //没有找到指定的任务描述表
#define ERR_NO_FIND_TIMER     	0x8101  //没有找到指定的定时器
#define ERR_INVALID_TIMER     	0x8102  //无效的定时器
#define ERR_MEM_FAILED        	0x8103  //系统分配内存失败
#define ERR_CREATE_MSG_FAILED 	0x8104  //创建消息队列失败
#define ERR_INVALID_PRIO      	0x8105  //任务优先级重复了
#define ERR_INVALID_SEM       	0x8106  //引用的信号量无效,指标识号越界
#define ERR_SEM_NO_CREATE     	0x8107  //信号量没有被创建
#define ERR_SEM_CREATE_ERR    	0x8108  //信号量创建失败
#define ERR_GET_SEM_ERR       	0x8109  //获取信号量失败
#define ERR_RELEASE_SEM_ERR   	0x8109  //获取信号量失败

//消息处理错误信息定义
#define ERR_APPIDINVALIDATE    	0x2000  /*appid invalidate*/

#define ERR_MESSAGE_NULL       	0x2100  /*GetMessage routine*/
#define ERR_MESSAGE_QUIT       	0x2103  /*GetMessage routine */
#define ERR_MESSAGE_OK         	0x2104  /*GetMessage routine */
#define ERR_POST_EVSEND        	0x2005   /*PostMessage routine*/

#define ERR_PMTIMEOUT          	0x2001 /*post message timeout*/
#define ERR_POST_QSEND         	0x2002   /*PostMessage routine*/
#define ERR_ALLOCMEM           	0x2003
#define ERR_GET_FREE_NODE      	0x2004
#define ERR_GET_MUTEX	       	0x2005
#define ERR_POST_SEM	       	0x2006
#define ERR_GET_SEM	       	   	0x2007
#define ERR_POST_MSG			0x2008

/*GetDevID函数返回错误代码*/
#define ERR_PORTID              0x2600     /*端口号非法*/  
/*WaitPortReady函数返回错误代码*/
#define ERR_PORT_INIT           0x2601     /*端口初始化错误*/

#define ERR_SERIAL_CHANNEL      0x2610     /*串口通道号错误*/
#define ERR_SERIAL_INIT         0x2611     /*串口没初始化*/
#define ERR_SERIAL_OPEN         0x2612     /*串口打开错误*/
/*prompt返回错误代码*/
#define ERR_PROMPT              0x2620     /*控制台字符长度配置为零*/
/*LookVar和AddVar错误代码*/
#define ERR_VARID               0x2621     /*变量ID 号错误*/
#define ERR_VARTABLEFULL        0x2622     /*变量表已满*/

/*ReadFile函数返回错误代码*/
#define ERR_OPEN                0x25001  /*文件打开错误*/
#define ERR_FILESTATE           0x25002  /*读文件状态错误*/
#define ERR_FILELEN             0x25003  /*文件长度非法*/
#define ERR_OFFSET              0x25004  /*输入参数读文件偏移量非法,或写偏移量非法WriteFile返回*/
#define ERR_FILESEEK            0x25005  /*文件指针移动错误,WriteFile和ReadFile*/
#define ERR_FILEREAD            0x25006  /*读文件错误*/
#define ERR_CHECKSUM            0x25007  /*文件校验错误*/

/*WriteFle函数返回错误代码*/
#define ERR_WRITELEN            0x25008   /*写文件长度错误*/
#define ERR_WRITEFILE           0x25009   /*写文件出错*/

#endif
