/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			r8025.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           R8025时钟芯片驱动代码                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      提供时钟芯片初始化、读时钟信息、写时钟信息等操作接口                          
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	InitR8025			时钟芯片初始化
*      	WriteR8025Time		向时钟芯片写入时间信息
*      	ReadR8025Time		读时钟芯片时间信息
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/7/10    初始创建                         
*                                                                                  
********************************************************************************************/

#include "r8025.h"
#include "m5234iic.h"

extern void InitR8025Sub();

BYTE *pR8025Buf;

void InitR8025(void)
{
	//初始化iic总线
	M5234IICInit();
	
	pR8025Buf = (BYTE *)OS_MemAlloc(R8025_BUF_LEN);
	if(pR8025Buf == NULL)
	{
		LogError("InitR8025", FILE_LINE, "error");
		return;
	}
	pR8025Buf[0] = 0x20;	//设置无中断，24小时制，其它位不做修改
	pR8025Buf[1] = 0xa0;
	
	if( M5234IICRW(R8025_WIRTE_ADDR, CONTROL1_REG<<4, pR8025Buf, 1, FALSE) == ERR_FAILURE)
		LogError("InitR8025", FILE_LINE, "init CONTROL1_REG error");
	if( M5234IICRW(R8025_WIRTE_ADDR, CONTROL2_REG<<4, &pR8025Buf[1],1, FALSE) == ERR_FAILURE)
		LogError("InitR8025", FILE_LINE, "init CONTROL1_REG error");
}

void InitR8025Sub()
{
	pR8025Buf[0] = 0x20;	//设置无中断，24小时制，其它位不做修改
	pR8025Buf[1] = 0xa0;
	if( M5234IICRW(R8025_WIRTE_ADDR, CONTROL1_REG<<4, pR8025Buf, 1, FALSE) == ERR_FAILURE)
		LogError("InitR8025", FILE_LINE, "init CONTROL1_REG error");
	if( M5234IICRW(R8025_WIRTE_ADDR, CONTROL2_REG<<4, &pR8025Buf[1],1, FALSE) == ERR_FAILURE)
		LogError("InitR8025", FILE_LINE, "init CONTROL1_REG error");
}

//向时钟芯片写入时间信息
DWORD WriteR8025Time(TSysTime *time)
{
	//按BCD码方式根据寄存器顺序放入时间信息
	pR8025Buf[0] = ( time->Second%10 + ((time->Second/10)<<4) ) & 0x7f;
	pR8025Buf[1] = ( time->Minute%10 + ((time->Minute/10)<<4) ) & 0x7f;
	pR8025Buf[2] = ( time->Hour%10 + ((time->Hour/10)<<4) ) & 0x3f;
	
	pR8025Buf[3] = time->Week&0x07;
	if(pR8025Buf[3] == 7)	//星期日
		pR8025Buf[3] = 0;
	
	pR8025Buf[4] = (time->Day%10 + ((time->Day/10)<<4) ) & 0x3f;
	pR8025Buf[5] = (time->Month%10 + ((time->Month/10)<<4) ) & 0x1f;
	pR8025Buf[6] = time->Year%10 + (((time->Year%100)/10)<<4);
	
	return M5234IICRW(R8025_WIRTE_ADDR, (SECOND_REG<<4)&0xf0, pR8025Buf, 7, FALSE);
}	

DWORD ReadR8025Time(TSysTime *time)
{
	BYTE i=0;
	DWORD status;
	
	for(i=0; i<R8025_BUF_LEN; i++)
		pR8025Buf[i] = 0;
	
	status = M5234IICRW(R8025_WIRTE_ADDR, (SECOND_REG<<4)|0x04, pR8025Buf, 7, TRUE);
	if(status != ERR_SUCCESS)
	{
		LogError("ReadR8025Time", FILE_LINE, "read time error.");
		return status;
	}
	//转换时间信息格式		
	time->Second = (pR8025Buf[0]&0x0f) + (pR8025Buf[0]>>4)*10;
	time->Minute = (pR8025Buf[1]&0x0f) + (pR8025Buf[1]>>4)*10;
	time->Hour = (pR8025Buf[2]&0x0f) + (pR8025Buf[2]>>4)*10;
	time->Week = pR8025Buf[3]&0x07;
	if(time->Week == 0)	//星期日
		time->Week = 7;
	time->Day = (pR8025Buf[4]&0x0f) + (pR8025Buf[4]>>4)*10;
	time->Month = (pR8025Buf[5]&0x0f) + (pR8025Buf[5]>>4)*10;

	//调整为2000年以后
	time->Year = (pR8025Buf[6]&0x0f) + (pR8025Buf[6]>>4)*10+2000;
	
	//判断时间信息是否有效
	if(time->Second>60 || time->Minute>60 || time->Hour>24 || time->Week>7 || time->Day>31 || \
				time->Month>12 || time->Year>2100)
	{
		LogError("ReadR8025",FILE_LINE,"time invalidate");
/*
		time->Year = 6;
		time->Month = 1;
		time->Day = 1;
		time->Hour = 0;
		time->Minute = 0;
		time->Second = 0;
*/		
		return ERR_FAILURE;
	}
//	time->Year += 2000;	
	return ERR_SUCCESS;	
}	


