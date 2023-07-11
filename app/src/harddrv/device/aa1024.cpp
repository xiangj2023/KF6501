/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			aa1024.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           25AA1024串行EEPROM驱动，SPI接口，128K字节，页长度为256字节，最大时钟周期20MHz                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      根据保护要求提供所有操作接口，包括读写、保护、擦除、低功耗控制等。                          
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	AA1024WEnable		25AA1024写使能开关控制
*		AA1024ReadStatus	25AA1024读状态寄存器
*		AA1024WriteStatus	25AA1024写状态寄存器
*		AA1024WriteBP		25AA1024写状态寄存器的BP0/BP1
*		AA1024DPDInOut		25AA1024低功耗模式控制
*		AA1024PageRead		AA1024单页范围内读操作
*		AA1024PageWrite		AA1024单页范围内写操作
*		AA1024Read			AA1024读操作
*		AA1024Write			AA1024单页范围内写操作
*		AA1024PageWrite		AA1024单页范围内写操作
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               09/02/14    初始创建                         
*                                                                                  
********************************************************************************************/

#if(EEPROM_MODULE == AA1024)

#include "aa1024.h"
#include "m5234spi.h"

/*********************************************************************************************
 *
 * 功能描述     25AA1024写使能开关控制
 *
 * 参数说明      - isEnable	: 输入 	写使能开关标志
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024WEnable(BOOL isEnable)
{
	BYTE bCmdBuf[10],bCmdLen,bStatusReg;
	BYTE bDataBuf[10], bDataLen;
	
	//将SPI设置为AA1024需要的模式
	SetAA1024Mode();
	
	SetSpiCs0Disact();	//设置片选无效

	if(isEnable == TRUE)	//写允许
		bCmdBuf[0] = AA1024_WREN;
	else					//写禁止
		bCmdBuf[0] = AA1024_WRDI;
	
	//最多连续3次控制写使能
	for(int i=0; i<3; i++)
	{
		//使能控制操作
		if(M5234SpiOperate(AA1024_CS_ADDR, bCmdBuf, 1, bDataBuf, 0, SPI_WRITE) == ERR_FAILURE)
		{
			M5234SpiReset();
			ReportMsg("AA1024EnablWrite Error.");
			continue;
		}
		
		//由于发送完控制写使能命令到真正反映到状态寄存器有一定延时
		for(int j=0; j<AA1024_STSREG_DELAY; j++)
		{
			//读状态寄存器
			if(AA1024ReadStatus(&bStatusReg) == ERR_SUCCESS)
			{
				//检查写使能操作是否成功
				if( (isEnable == TRUE) && (bStatusReg&AA1024_STS_WEL) )
					return ERR_SUCCESS;
				
				//检查写禁止操作是否成功
				if( (isEnable == FALSE) && (~(bStatusReg&AA1024_STS_WEL)) )
					return ERR_SUCCESS;
			}
		}
	}

	//重复多次操作，写使能开关控制失败
	ReportMsg("AA1024EnablWrite Error1.");
	return 	ERR_FAILURE;
}

/*********************************************************************************************
 *
 * 功能描述     25AA1024读状态寄存器
 *
 * 参数说明      - status	: 输出 	读取的状态寄存器值
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024ReadStatus(BYTE *status)
{
	BYTE bCmdBuf[10],bCmdLen;
//	BYTE bDataBuf[10], bDataLen;
//	DWORD rtn;
	
	bCmdBuf[0] = AA1024_RDSR;
	
//	bCmdLen = 1;
	
//	memset((void *)bDataBuf, 0, sizeof(bDataBuf));
	
//	bDataLen = 1;
	
	SetSpiCs0Disact();	//设置片选无效
	
	return M5234SpiOperate(AA1024_CS_ADDR, bCmdBuf, 1, status, 1, SPI_READ);
/*	
	*status = bDataBuf[0];
	if(rtn == ERR_FAILURE)
	{
		ReportMsg("AA1024ReadStatus ERROR, data=%x.", bDataBuf[0]);
	}
	return rtn;
*/	
}

/*********************************************************************************************
 *
 * 功能描述     25AA1024写状态寄存器
 *
 * 参数说明      - status	: 输入 	写入的状态值
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024WriteStatus(BYTE status)
{
	BYTE bCmdBuf[10],bCmdLen;
	BYTE bDataBuf[10], bDataLen, data;
	int i=3;
	BYTE flag = 0;

	//控制WP引脚，允许写操作
	AA1024WUnP();

	//写使能打开
	if( AA1024WEnable(TRUE) == ERR_FAILURE )
		return ERR_FAILURE;


	//写状态寄存器
	bCmdBuf[0] = AA1024_WRSR;
	bCmdBuf[1] = status;

	//为保证可靠，支持3次重复
	//for(i=0; i<3; i++)
	while(i--)
	{
		if(M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 2, bDataBuf, 0, SPI_WRITE) == ERR_SUCCESS)
		{
			flag = 0xff;
			break; 
		}
	}

	if(flag == 0)
	{
		ReportMsg("AA1024WriteStatus Error.");
		return ERR_FAILURE;
	}
	//通过读取WIP状态，检查写入状态是否正确
	for(i=0; i<AA1024_STSREG_DELAY; i++)
	{
		//读状态寄存器，用SysDelay延时不够
		OS_Sleep(10);
		if( AA1024ReadStatus(&data) == ERR_SUCCESS)
		{
			if( (data&AA1024_STS_WIP) == 0)
			{
				//控制WP引脚，禁止写操作
				AA1024WP();
				return ERR_SUCCESS;
			}
		}
	}
	//控制WP引脚，禁止写操作
	AA1024WP();
	
	ReportMsg("AA1024WriteStatus Error1, data=%x status=%x.", data, status);
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * 功能描述     25AA1024写状态寄存器的BP0/BP1，进行块保护控制
 *
 * 参数说明      - data	: 输入 	写入的命令信息
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024WriteBP(BYTE data)
{
	BYTE bStatusReg;

	//检查写入命令是否合法
	if( (data != AA1024_BLOCK_PT) && (data != AA1024_BLOCK_UNPT) )
		return ERR_FAILURE;

	//将SPI设置为AA1024需要的模式
	SetAA1024Mode();

	SetSpiCs0Disact();	//设置片选无效

	//写状态寄存器,因为写寄存器内部有读状态检查，因此不再重复检查
	return AA1024WriteStatus(data);
	/*	
	if(AA1024WriteStatus(data) == ERR_FAILURE)
	{
		ReportMsg("AA1024WriteBP Error.");
		return ERR_FAILURE;
	}
	//由于发送完控制写使能命令到真正反映到状态寄存器有一定延时
	for(int j=0; j<AA1024_STSREG_DELAY; j++)
	{
		//读状态寄存器
		if(AA1024ReadStatus(&bStatusReg) == ERR_SUCCESS)
		{
			//检查标志写入操作是否成功
			if( (bStatusReg&0xfc) == (data&0xfc) )
				return ERR_SUCCESS;
		}
	}
	
	ReportMsg("AA1024WriteStatus Error1, data=%x bStatusReg=%x.", data, bStatusReg);
	return	ERR_FAILURE;
*/	
}


/*********************************************************************************************
 *
 * 功能描述     25AA1024低功耗模式控制
 *
 * 参数说明      - isIn	: 输入 	进入还是退出
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024DPDInOut(BOOL isIn)
{
	BYTE bCmdBuf[10],bCmdLen;
	BYTE bDataBuf[10], bDataLen;
	DWORD rtn;
//	BYTE status = 0;
//	DWORD delay = 1000;

	//判断是进入还是退出低功耗模式
	if(isIn == TRUE)
		bCmdBuf[0] = AA1024_DPD;
	else
		bCmdBuf[0] = AA1024_RDPD;
	
	SetSpiCs0Disact();	//设置片选无效
	
	rtn = M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 1, bDataBuf, 0, SPI_WRITE);

	//低功耗模式的切换需要一定的稳定时间，约100us
	OS_Sleep(10);

//	AA1024ReadStatus(&status);
	
	return rtn;
}


/*********************************************************************************************
 *
 * 功能描述     AA1024单页范围内读操作，与页写配合使用，用来检查页写入数据是否正确
 *
 * 参数说明      - pBuf		: 输出 	读数据缓冲区
 *				 - len		: 输入	读取数据长度
 *				 - address	: 输入	读取数据的起始地址	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024PageRead(BYTE *pBuf, DWORD len, DWORD address)
{
	BYTE bCmdBuf[10],bCmdLen;
	int i=3;
	
/*
	ASSERT(len <= AA1024_PAGE_LEN);
	if(len>AA1024_PAGE_LEN)
		return ERR_FAILURE;

	ASSERT(address<AA1024_LEN);
	if(address>=AA1024_LEN)
		return ERR_FAILURE;
*/	
	bCmdBuf[0] = AA1024_READ;
	bCmdBuf[1] = (BYTE)(address>>16);
	bCmdBuf[2] = (BYTE)(address>>8);
	bCmdBuf[3] = (BYTE)(address);

	while(i--)
	{
		if(M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 4, pBuf, \
					len, SPI_READ) == ERR_SUCCESS)
			return ERR_SUCCESS;
	}
	
	return ERR_FAILURE;
}


/*********************************************************************************************
 *
 * 功能描述     AA1024单页范围内写操作
 *
 * 参数说明      - pBuf		: 输入 	写入数据缓冲区
 *				 - len		: 输入	写入数据长度
 *				 - address	: 输入	写入数据的起始地址	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024PageWrite(BYTE *pBuf, DWORD len, DWORD address)
{
	BYTE bCmdBuf[10],bCmdLen, status;
	int i = 3;

	ASSERT(len <= AA1024_PAGE_LEN);
	if(len>AA1024_PAGE_LEN)
		return ERR_FAILURE;

	//注意，对于每次写操作都要执行一次写使能操作，否则写入失败
	if( AA1024WEnable(TRUE) == ERR_FAILURE )
		return ERR_FAILURE;

	//进行写操作，范围必须在一页内
	bCmdBuf[0] = AA1024_WRITE;
	bCmdBuf[1] = (BYTE)(address>>16);
	bCmdBuf[2] = (BYTE)(address>>8);
	bCmdBuf[3] = (BYTE)(address);

	while(i--)
	{
		if(M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 4, pBuf, len, SPI_WRITE) == ERR_SUCCESS)
		{
			//检查是否写完
			OS_Sleep(10);
			AA1024ReadStatus(&status);
			if( (status&AA1024_STS_WIP) == 0)	//写完成
			{
//				ReportMsg("AA1024PageWrite address=%d, len=%d OK.",address, len);
				return ERR_SUCCESS;
			}
		}
	}

	
	ReportMsg("AA1024PageWrite address=%d, len=%d ERROR  ...",address, len);
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * 功能描述     AA1024读操作，起始地址和长度任意，只要在AA1024的有效空间内即可。
 *				对于长数据读，为了防止系统长时间处于中断状态，分为小段读取，增加可靠性
 *
 * 参数说明      - pBuf		: 输出 	读数据缓冲区
 *				 - len		: 输入	读取数据长度
 *				 - address	: 输入	读取数据的起始地址	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024Read(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD dwTrueLen, dwTrueAddr, dwReadLen;
	BYTE *pReadPtr;
	
	//参数合法性检查
	ASSERT((address+len)<=AA1024_LEN);
	if((address+len)>AA1024_LEN)
		return ERR_FAILURE;

	//退出低功耗模式
	if( AA1024DPDInOut(FALSE) == ERR_FAILURE)
		return ERR_FAILURE;
	
	pReadPtr = pBuf;
	dwTrueLen = len;
	dwTrueAddr = address;

	//为了减少读持续时间(防止长时间中断)，每次最多读取一个页长度的数据，
	//注意不一定是页对齐，因为读操作可以跨页
	while(dwTrueLen>0)
	{
		if(dwTrueLen<AA1024_PAGE_LEN)
			dwReadLen = dwTrueLen;
		else
			dwReadLen = AA1024_PAGE_LEN;
		dwTrueLen -= dwReadLen;

		//借用页读函数进行读操作
		if(AA1024PageRead(pReadPtr, dwReadLen, dwTrueAddr) == ERR_FAILURE)
		{
			AA1024DPDInOut(TRUE);	//操作失败，返回前进入低功耗模式
			return ERR_FAILURE;
		}
		//移动读缓冲区指针和读地址位置
		pReadPtr += dwReadLen;
		dwTrueAddr += dwReadLen;
	}

	AA1024DPDInOut(TRUE);	//操作完成，返回前进入低功耗模式
	return ERR_SUCCESS;
	
}

/*********************************************************************************************
 *
 * 功能描述     AA1024写操作，起始地址和长度任意，只要在AA1024的有效空间内即可。
 *				由于AA1024只支持页写，当长度跨页时，需要分成多页写入，并需要通过读操作，
 *				检查每次写入的数据是否正确。
 *
 * 参数说明      - pBuf		: 输入 	写入数据缓冲区
 *				 - len		: 输入	写入数据长度
 *				 - address	: 输入	写入数据的起始地址	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024Write(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD dwTrueLen, dwTrueAddr, dwWriteLen;
	BYTE *pWritePtr;
	BYTE flag1 = 0;
	BYTE flag2 = 0;
	BYTE bReadBuf[AA1024_PAGE_LEN];	//比较读数据缓冲区

	//参数合法性检查
	ASSERT((address+len)<=AA1024_LEN);
	if((address+len)>AA1024_LEN)
		return ERR_FAILURE;


	//退出低功耗模式
	if( AA1024DPDInOut(FALSE) == ERR_FAILURE)
		return ERR_FAILURE;

	//禁止块保护
	if( AA1024WriteBP(AA1024_BLOCK_UNPT) == ERR_FAILURE)
	{
		AA1024DPDInOut(TRUE);	//恢复进入低功耗模式
		return ERR_FAILURE;
	}

	pWritePtr = pBuf;
	dwTrueLen = len;
	dwTrueAddr = address;

	//首先处理非页对齐的部分
	if((dwTrueAddr%AA1024_PAGE_LEN))
	{
		//计算第一块写入数据的长度
		if((dwTrueAddr%AA1024_PAGE_LEN+dwTrueLen) > AA1024_PAGE_LEN)
			dwWriteLen = AA1024_PAGE_LEN - dwTrueAddr%AA1024_PAGE_LEN;
		else
			dwWriteLen = len;
		//写入第一块数据
		if( AA1024PageWrite(pWritePtr, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
		{
//			memset(bReadBuf, 0, AA1024_PAGE_LEN);
//			SysDelay(delay);
//			OS_Sleep(10);
			//重新读出数据进行比较检查
			if(AA1024PageRead(bReadBuf, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
			{
//				ReportMsg("read data:%x %x %x %x", bReadBuf[0],bReadBuf[1],bReadBuf[2],bReadBuf[3]);
				//数据比较
				if( DataCmp(pWritePtr, bReadBuf, dwWriteLen) == TRUE )
				{
					//写入正确移动写入数据缓冲区指针和写入地址,修改需要写入的数据长度
					pWritePtr += dwWriteLen;
					dwTrueAddr += dwWriteLen;
					dwTrueLen -= dwWriteLen;
					flag1 = 0xaa;	
				}
				else
				{
					ReportMsg("AA1024Write cmp error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
				}
			}
		}
	}
	else
		flag1 = 0xaa;
	
	if(flag1)	//第一块写入正确
	{
		while(dwTrueLen)
		{
			flag2 = 0;
			//计算每次写入的数据长度
			if(dwTrueLen<AA1024_PAGE_LEN)
				dwWriteLen = dwTrueLen;
			else
				dwWriteLen = AA1024_PAGE_LEN;
			
			//写入数据
			if( AA1024PageWrite(pWritePtr, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
			{
				
//				ReportMsg("write data:%x %x %x %x", pWritePtr[0],pWritePtr[1],pWritePtr[2],pWritePtr[3]);
//				memset(bReadBuf, 0, AA1024_PAGE_LEN);
//				OS_Sleep(10);
//				SysDelay(delay);
				//重新读出数据进行比较检查
				if(AA1024PageRead(bReadBuf, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
				{
//					ReportMsg("read data:%x %x %x %x", bReadBuf[0],bReadBuf[1],bReadBuf[2],bReadBuf[3]);
					//数据比较
					if( DataCmp(pWritePtr, bReadBuf, dwWriteLen) == TRUE )
					{
						//写入正确移动写入数据缓冲区指针和写入地址,修改需要写入的数据长度
						pWritePtr += dwWriteLen;
						dwTrueAddr += dwWriteLen;
						dwTrueLen -= dwWriteLen;
						flag2 = 0xaa;	
					}
					else
					{
						ReportMsg("AA1024Write cmp error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
						break;
					}
				}
				else
				{
					ReportMsg("AA1024Write read error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
					break;
				}
			}
			else
			{
				ReportMsg("AA1024Write page read error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
				break;
			}
		}
		flag2 = 0xaa;
	}

	AA1024WriteBP(AA1024_BLOCK_PT);	//恢复块保护
	
	AA1024WEnable(FALSE);	//写操作结束后，调用写禁止

	AA1024DPDInOut(TRUE);	//恢复进入低功耗状态


	if( (flag1 == 0xaa) &&(flag2 == 0xaa) )//所有页都写入成功
		return ERR_SUCCESS;

	return ERR_FAILURE;
}

/*********************************************************************************************
 *
 * 功能描述     AA1024片擦除操作
 *
 * 参数说明      无
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD AA1024ChipErase()
{
	BYTE bCmdBuf[10],bCmdLen,bStatusReg;
	BYTE bDataBuf[10], bDataLen;
	int i=3;
	
	//将SPI设置为AA1024需要的模式
	SetAA1024Mode();
	
	SetSpiCs0Disact();	//设置片选无效

	bCmdBuf[0] = AA1024_CE;
	

	//最多连续3次擦除
	while(i--)
	{
		//使能控制操作
		if(M5234SpiOperate(AA1024_CS_ADDR, bCmdBuf, 1, bDataBuf, 0, SPI_WRITE) == ERR_FAILURE)
		{
			M5234SpiReset();
			ReportMsg("AA1024ChipErase Error.");
			continue;
		}

		
		//由于擦除完成最大需要10ms时间，需要延时等待
		OS_Sleep(10);
		//读状态寄存器
		if(AA1024ReadStatus(&bStatusReg) == ERR_SUCCESS)
		{
			//检查写使能操作是否成功
			if( (bStatusReg&AA1024_STS_WIP) == 0)
				return ERR_SUCCESS;
		}
	}

	//片擦除失败
	ReportMsg("AA1024ChipErase Error1.");
	return 	ERR_FAILURE;
}

/*********************************************************************************************
 *
 * 功能描述     EEPROM读操作，需要临界资源保护
 *
 * 参数说明      - pBuf		: 输出 	读数据缓冲区
 *				 - len		: 输入	读取数据长度
 *				 - address	: 输入	读取数据的起始地址	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD EepromRead(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD rtn;

	//临界资源保护,考虑多个SPI外设的情况
    OS_LockM(SPI_LOCK);

	AA1024DPDInOut(FALSE);	//退出低功耗模式

	rtn = AA1024Read(pBuf, len, address);

	AA1024DPDInOut(TRUE);	//进入低功耗模式

	OS_UnLockM(SPI_LOCK);

	return rtn;
}

/*********************************************************************************************
 *
 * 功能描述     EEPROM写操作，需要临界资源保护
 *
 * 参数说明      - pBuf		: 输入 	写入数据缓冲区
 *				 - len		: 输入	写入数据长度
 *				 - address	: 输入	写入数据的起始地址	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD EEPROMWrite(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD rtn;
	int i=3;

	
//	ReportMsg("The task%x write eeprom.", RtosGetCurrentTaskId());
	//临界资源保护,考虑多个SPI外设的情况
    OS_LockM(SPI_LOCK);

	while(i--)	
	{
		if( (rtn = AA1024Write(pBuf, len, address)) == ERR_SUCCESS )
			break;
		
		//写不成功，复位SPI接口
		M5234SpiReset();
	}
	
	OS_UnLockM(SPI_LOCK);

	return rtn;
}

/*********************************************************************************************
 *
 * 功能描述     EEPROM整片擦除操作
 *
 * 参数说明      无
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD EEPROMChipErase()
{
	DWORD rtn;

	//临界资源保护,考虑多个SPI外设的情况
    OS_LockM(SPI_LOCK);

	AA1024DPDInOut(FALSE);	//退出低功耗模式

	AA1024WEnable(TRUE);	//擦除前先执行WREN
	rtn = AA1024ChipErase();
	AA1024WEnable(FALSE);	
	
	AA1024DPDInOut(TRUE);	//进入低功耗模式

	OS_UnLockM(SPI_LOCK);

	return rtn;
}
/*********************************************************************************************
 *
 * 功能描述     两组数据内容比较
 *
 
  * 参数说明	  - pData1	 : 输入  比较数据1
  * 			  - pData2 	 : 输入  比较数据2
  * 			  - len		 : 输入  比较数据长度  
  *
 * 返回代码
 *                数据内容一致或不一致
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL DataCmp(BYTE *pData1, BYTE *pData2, DWORD len)
{
	DWORD length;
	BYTE *pBuf1, *pBuf2;

	length = len;
	pBuf1 = pData1;
	pBuf2 = pData2;

	while(length--)
	{
		if(*pBuf1 != *pBuf2)
			return FALSE;
		pBuf1++;
		pBuf2++;
	}
	return TRUE;
}

#ifdef _DBUG_AA1024_

#endif



#endif
