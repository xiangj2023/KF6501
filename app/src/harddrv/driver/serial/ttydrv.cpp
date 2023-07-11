/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			ttydrv.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           串口物理通道操作接口                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      完成对所有串口物理通道的操作接口封装，供串口管理任务使用                          
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	SerialInit			串口物理通道初始化
*      	SerialOpen			串口物理通道打开                                                              
*      	SerialRxdCtr	   	串口物理通道接收控制                                                                         
*      	SerialTxdCtr    	串口物理通道发送控制                                                                        
*      	SerialIOCtl	    	串口物理通道一般输入输出控制                                                                       
*      	WriteAChar		 	向串口物理通道写入一个字节，以触发串口发送中断                                                                       
*      	ResetComm		    复位串口物理通道                                                                      
*      	GetSerialMap	   	取串口物理通道的映射表                                                                        
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/7/9    初始创建                         
*                                                                                  
********************************************************************************************/
#include "ttydrv.h"
#include "misi.h"

#include "m5234uart.h"

//串口功能接口函数初始化
const TSerialFunc SerialFuncs[] =
{
#if (MCF5234_UART ==YES)    
    {M5234UartInit, M5234UartOpen, M5234UartRxd,  M5234UartTxd,
		M5234UartIOCtl, M5234UartWriteAChar},
#else
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL},
#endif  	
};

////#if(MODULE_TYPE== MD_KV5234)
/*逻辑串口到物理串口的映射*/
TSerialMap SerialMap[MAX_SERIAL_NUM] =
{

    /* 驱动程序号,  物理通道号, 所属芯片号,  在本类串口中的偏移    */ 
	{ 0,			  0,			0,			0 },  /* 保留 */
	{ M5234_UART,     0,			0,			0 },  /* 逻辑串口1, MCF5234的第0个UART */
	{ M5234_UART,     2,			0,			1 },  /* 逻辑串口2, MCF5234的第2个UART */
};
////#endif

/*********************************************************************************************
 *
 * 功能描述     串口物理通道初始化
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void SerialInit()
{
	for(int i=0; i<ArraySize(SerialFuncs); i++)
		if(SerialFuncs[i].pInit != NULL)
			SerialFuncs[i].pInit();
} 


/*********************************************************************************************
 *
 * 功能描述     串口物理通道打开
 *
 * 参数说明      - wPort	: 输入 	串口号
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD SerialOpen(WORD wPort)
{
	CSerial* pSerial;
	WORD nDrvNo;
	
	pSerial = GetSerial(wPort);
	
	if(pSerial == NULL)
		return ERR_SERIAL_CHANNEL;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialOpen",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrvNo = SerialMap[wPort].nDriver;
	
//	pSerial->m_wPortType = nDrvNo;
	if(SerialFuncs[nDrvNo].pOpen != NULL)
	  return SerialFuncs[nDrvNo].pOpen(wPort, &SerialMap[wPort]);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     串口物理通道接收控制
 *
 * 参数说明      - wPort	: 输入 	串口号
 *				 - bFlag	: 输入	控制标志
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD SerialRxdCtr(WORD wPort, BYTE bFlag)
{
	int nDrvNo;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialRxdCtr",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrvNo = SerialMap[wPort].nDriver;

	if(SerialFuncs[nDrvNo].pRxdCtr != NULL)
		return SerialFuncs[nDrvNo].pRxdCtr(wPort, bFlag);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     串口物理通道发送控制
 *
 * 参数说明      - wPort	: 输入 	串口号
 *				 - bFlag	: 输入	控制标志
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD SerialTxdCtr(WORD wPort, BYTE bFlag)
{
	int nDrvNo;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialRxdCtr",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrvNo = SerialMap[wPort].nDriver;

	if(SerialFuncs[nDrvNo].pTxdCtr != NULL)
		return SerialFuncs[nDrvNo].pTxdCtr(wPort, bFlag);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     串口物理通道一般输入输出控制，如MODEM信号、串口通信参数等
 *
 * 参数说明      - wPort	: 输入 	串口号
 *				 - Cmd		: 输入	控制命令
 * 			     - pdwPar1	: |---MODEM_SET:		输入	MODEM输出控制信号标志位
 *							  |---MODEM_GET:		输出	MODEM输入状态信号		
 *							  |---BAUD_SET:			输入	准备写入的波特率值		
 *							  |---BAUD_GET:			输出	读取的波特率值	
 *							  |---COM_PAR_SET:		输入	准备设置的串口通信参数标志		
 *							  |---COM_PAR_GET:		输出	读出的通信参数标志		
 *							  |---COM_CHAN_RESET	无效		
 *							  |---COM_TXD_END		无效		
 *
 *				 - pdwPar2	: |---MODEM_SET:		输入	MODEM输出控制信号高低标志
 *							  |---MODEM_GET:		无效		
 *							  |---BAUD_SET:			无效		
 *							  |---BAUD_GET:			无效
 *							  |---COM_PAR_SET:		无效		
 *							  |---COM_PAR_GET:		无效		
 *							  |---COM_CHAN_RESET	无效		
 *							  |---COM_TXD_END		无效		
 *
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL SerialIOCtl(WORD wPort, BYTE Cmd, DWORD *pdwPar1, DWORD *pdwPar2)
{
	int nDrv;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialRxdCtr",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrv = SerialMap[wPort].nDriver;
	return SerialFuncs[nDrv].pIOCtl(wPort, Cmd, pdwPar1, pdwPar2);	
}

/*********************************************************************************************
 *
 * 功能描述     向串口物理通道写入一个字节，以触发串口发送中断
 *
 * 参数说明      - wPort	: 输入 	串口号
 *				 - bData	: 输入	发送的字节数据
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL  WriteAChar(WORD wPort, BYTE bData)
{
	WORD nDrv;

	if(wPort > MAX_SERIAL_NUM)
	{
		LogError("WriteAChar",FILE_LINE,"port no invalidate");
		return 0;
	}
	
	nDrv = SerialMap[wPort].nDriver;
	
	return SerialFuncs[nDrv].pWriteData(wPort, bData);	

}

/*********************************************************************************************
 *
 * 功能描述     复位串口物理通道
 *
 * 参数说明      - wPort	: 输入 	串口号
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void ResetComm(WORD wPort)
{
	CSerial* pSerial;
	
	pSerial = GetSerial(wPort);
	
	if(pSerial == NULL)
		return;
	
	pSerial->ResetSerial(RESET_SERIAL);
}

/*********************************************************************************************
 *
 * 功能描述     取串口物理通道的映射表
 *
 * 参数说明      - wPort	: 输入 	串口号
 *
 * 返回代码
 *                串口通道的映射表
 *
 * 其它说明：          
 *
**********************************************************************************************/
TSerialMap GetSerialMap(WORD wPort)
{
	return SerialMap[wPort];
}

