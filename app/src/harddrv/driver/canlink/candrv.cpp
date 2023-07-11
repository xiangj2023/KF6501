/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			canddrv.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           CAN网驱动接口                                                                      
*			                                                                
* 描述                                                                             
*                                                                                  
*       对FLEXCAN、SJA1000等CAN芯片驱动进行封装，提供统一的操作接口。                      
*                                                                                        
* 函数                                                                             
*                                                                                  
*      	CanHardInit			CAN初始化
*      	CanHardOpen			CAN硬件打开                                                              
*      	CanHardWrite	   	CAN硬件数据发送                                                                         
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/9/7    初始创建                         
*                                                                                  
********************************************************************************************/
#if(MCF5234_SJA1000 == YES)
	#include "sja1000.h"
#endif

#if(MCF5235_FLEXCAN == YES)
	#include "flexcan.h"
#endif

#include "candrv.h"
#include "canlink.h"

//CAN网驱动接口初始化
TCanDriver CanDriver[MAX_CAN_PORT] = 
{
#if(MCF5235_FLEXCAN == YES)
		{NULL,	FlexCanOpen,		NULL,	FlexCanWrite, FlexCanReset},
#else
				{NULL,			NULL,		NULL,	NULL, 	NULL		},
#endif

#if(MCF5234_SJA1000 == YES)
	{NULL, 	Sja1000Open,		NULL,	Sja1000Write, Sja1000Reset},
#else
			{NULL,			NULL,		NULL,	NULL,	 NULL		},
#endif
};

/*********************************************************************************************
 *
 * 功能描述     CAN初始化
 *
 * 参数说明      - port		: 输入 	can网内部端口序号
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanHardInit(WORD port)
{
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardInit",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pInit != NULL)
		return CanDriver[port].pInit(port);
	
	return ERR_FAILURE;
}

/*********************************************************************************************
 *
 * 功能描述     CAN硬件打开
 *
 * 参数说明      - port		: 输入 	can网内部端口序号
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanHardOpen(WORD port)
{
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardOpen",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pOpen != NULL)
		return CanDriver[port].pOpen(port);

	return ERR_FAILURE;
}

/*********************************************************************************************
 *
 * 功能描述     CAN硬件数据发送
 *
 * 参数说明      - port		: 输入 	can网内部端口序号
 *				 - address	: 输入	从模块地址
 *				 - type		: 输入	发送数据类型，链路命令还是应用层数据
 *				 - isNewWrite	:输入	是否为重发报文
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD CanHardWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite)
{
	static WORD counter[MAX_CAN_PORT];
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardOpen",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pWrite == NULL)
		return 	ERR_FAILURE;

	//检查CAN端口发送是否空闲
	if(isCanPortTxdIdle(port) == FALSE)
	{
//		ReportMsg("the can%d txd busy, send failed.", port);
		counter[port]++;
		
		//端口长时间发送繁忙，可能有问题，重新复位
		if(counter[port]>CAN_HW_RESET_LEVEL)
		{
			counter[port]++;
			CanHardReset(port);
		}
		return ERR_FAILURE;
	}

	
	counter[port] = 0;

	return CanDriver[port].pWrite(port, address, type, isNewWrite);
}


DWORD CanHardReset(WORD port)
{
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardReset",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pReset != NULL)
		return CanDriver[port].pReset(port);

	return ERR_FAILURE;
}

