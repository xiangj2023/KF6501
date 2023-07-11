/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			m5234iic.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           m5234的IIC驱动代码                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      提供IIC的有关初始化、读写控制等操作接口                          
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	M5234IICInit		IIC初始化
*		M5234IICRW			IIC读写操作接口
*		M5234IICInt			IIC中断处理函数
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/11/21    初始创建                         
*                                                                                  
********************************************************************************************/

#include "m5234iic.h"

TM5234iic TM5234IICBuf;		//iic通信缓存

/*********************************************************************************************
 *
 * 功能描述     IIC初始化，设置对应引脚，设置时钟频率，加载中断
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234IICInit(void)
{
	memset((void*)&TM5234IICBuf, 0, sizeof(TM5234iic));

	//设置I2C_SCL(L14) I2C_SDA(L15)作为I2C功能引脚
	MCF_GPIO_PAR_FECI2C |= 0x0f;

	//设置时钟周期为468.75kHz，不要太高，因为时钟芯片要求不能超过400kHz
	MCF_I2C_I2FDR = IIC_PRESCALER;

	MCF_I2C_I2CR |= MCF_I2C_I2CR_IEN;	//iic功能允许

	MCF_INTC0_ICRn(17) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(3);

	OS_SetVector(MCF_INTC0_LVL_IIC, M5234IICInt, 0);	//中断加载
	OS_IntEnable(MCF_INTC0_LVL_IIC);

		
	MCF_I2C_I2CR |= MCF_I2C_I2CR_IIEN;	//中断允许

}

/*********************************************************************************************
 *
 * 功能描述     IIC读写操作接口，完成读写操作，对于读操作，地址和命令写入后直接切换到接收模式
 *				没有采用RESTART方式。
 *
 * 参数说明      - bDevAddr	: 输入 	设备地址
 *				 - bCmdAddr	: 输入	命令地址
 *				 - bBuf		: 双向  写入或读取数据缓冲区地址
 *				 - bLen		: 输入	写入或读取的数据长度
 *				 - isRead	: 输入	是否为读操作
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD M5234IICRW(BYTE bDevAddr, BYTE bCmdAddr, BYTE *bBuf, BYTE bLen, BOOL isRead)
{
	int Delay;
	BYTE status;

	TM5234IICBuf.bSlaveDevAddr = bDevAddr;

	TM5234IICBuf.bDevCmdAddr = bCmdAddr; 		//保存被写设备命令地址
	TM5234IICBuf.bIicBuf = bBuf; 				//保存发送缓冲区地址
	TM5234IICBuf.bIicDataLen = bLen; 			//保存发送数据长度
	TM5234IICBuf.isRead = isRead;
	TM5234IICBuf.bIicEnd = 0;
	TM5234IICBuf.bCmdAddrEnb = 2;	//设置命令地址标志
	Delay = IIC_DELAY*(bLen+2)*100;	//延时计数

	MCF_I2C_I2CR |= MCF_I2C_I2CR_IEN;	//iic功能允许

	MCF_I2C_I2AR = bDevAddr;	
	
	MCF_I2C_I2CR |= MCF_I2C_I2CR_TXAK;	//不产生ACK信号响应(主机接收时需要)
		
	MCF_I2C_I2CR |= MCF_I2C_I2CR_MTX;	//设置发送模式
	
	MCF_I2C_I2CR |= MCF_I2C_I2CR_MSTA;	//设置主模式，产生start信号
	MCF_I2C_I2DR = TM5234IICBuf.bSlaveDevAddr;	//发送地址字节

	//	OS_Sleep(Delay);	//此处不能用sleep方式等待，不知道原因	
	//等待数据发送完成
	for(int i=0; i<100; i++)
	{
		SysDelay(Delay);	//等待数据发送完成
		if(0xff == TM5234IICBuf.bIicEnd)	//检查是否真正发送成功
			return ERR_SUCCESS;
	}
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * 功能描述     IIC中断处理函数
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void M5234IICInt(void * arg)
{
	BYTE status, temp;

	status = MCF_I2C_I2SR;	//读取状态寄存器
//	TM5234IICBuf.bReg = status;
	MCF_I2C_I2SR = 0x00;					//清除状态寄存器状态

	if(status & MCF_I2C_I2SR_IAL)	//仲裁信号丢失
	{
		MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//产生STOP信号
		MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//中止iic功能
		TM5234IICBuf.bIicEnd = 1;	//异常中止
		return;
	}

	if(MCF_I2C_I2CR & MCF_I2C_I2CR_MTX)	//发送模式
	{
		if( (status&MCF_I2C_I2SR_RXAK) == 0)	//收到ACK信号
		{
			if(TM5234IICBuf.bCmdAddrEnb == 2)	//刚才发送的是从地址字节
			{
				MCF_I2C_I2DR = TM5234IICBuf.bDevCmdAddr;	//发送从命令信息
				TM5234IICBuf.bCmdAddrEnb = 0;
			}
			else	//命令信息发送完成,发送或接收数据信息
			{
				if(TM5234IICBuf.isRead == FALSE)	//写操作，发送数据信息
				{
					if(TM5234IICBuf.bIicDataLen)	//发送剩余数据
					{
						MCF_I2C_I2DR = *TM5234IICBuf.bIicBuf++;
						TM5234IICBuf.bIicDataLen--;
					}
					else	//数据发送完成,结束发送
					{
						MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//产生STOP信号
						TM5234IICBuf.bIicEnd = 0xff;	//写操作正常结束
						MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//中止iic功能
						return;
					}
				}
				else	//读操作，发送完从地址和命令信息后，切换到接收状态
				{
					//切换到接收状态
					MCF_I2C_I2CR &= ~(MCF_I2C_I2CR_MTX|MCF_I2C_I2CR_TXAK);
					if(TM5234IICBuf.bIicDataLen == 1)	//读倒数第二数据前置产生STOP信号标志
						MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);

					//模拟进行一次读操作
					temp = MCF_I2C_I2DR;
				}
			}
		}
		else	//发送状态没有收到ACK信号，中止
		{
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//产生STOP信号
			TM5234IICBuf.bIicEnd = 2;	//异常中止
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//中止iic功能
			return;
		}
	}
	else	//接收模式，主机每收到一个字节信息后会自动响应ACK
	{
		if(TM5234IICBuf.bIicDataLen == 1)	//读倒数第二数据前置不产生ACK信号标志
			MCF_I2C_I2CR |= MCF_I2C_I2CR_TXAK;
		else if(TM5234IICBuf.bIicDataLen == 0)//最后一个接收数据，读操作正常结束
		{
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//产生STOP信号
			TM5234IICBuf.bIicEnd = 0xff;	//
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//中止iic功能
		}
		*TM5234IICBuf.bIicBuf++ = MCF_I2C_I2DR;
		if(TM5234IICBuf.bIicDataLen)
			TM5234IICBuf.bIicDataLen--;
	}
}


