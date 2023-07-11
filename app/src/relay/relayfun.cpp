/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			relayfun.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           		保护基本功能函数                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*                                
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	
*		
*		
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      yanzh             08/12/19    初始创建                         
*                                                                                  
********************************************************************************************/

#include "relayfun.h"
#include "logic.h"
#include "dbasedef.h"
#include "RelayDef.h"
#include "dbfunc.h"
#include "logicdef.h"
#include "sample.h"
#include "max11046.h"
#include "led.h"
#include "m5234timer.h"
#include "relay.h"
#include "scdefs.h"
#include "daefunc.h"
#include "RelayMemInit.h"
#include "relay_defs.h"
#include "ai.h"





extern DWORD 	dwRelayMSCnt;		//保护用毫秒计数器
extern BYTE 	byRelayWordValue[];
extern DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//开出自检失效状态，置位表示异常
extern DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//开出自检击穿状态，置位表示异常
extern DWORD 	dwBOStatus[MAX_BO_BOARD_NUM][2];
extern DWORD 	dwADDataWritePtr;

extern BYTE byDebugRelayBuf[]; //调试继电器缓冲区

//TAbsTime     RelayPickUpTime;
BYTE 	*g_MaintSW = NULL;
BYTE 	g_RptSerialNum = 0;			//报告序列号 
BOOL    RelayCheckErr = FALSE;			//保护自检出错
BOOL 	RelayCheckInitOK = FALSE;
WORD	RelayCheckAcc = 0;

/************************************************************
	功能：傅氏积分基波算法，除16
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluBaseFourier_S16(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14703*(-pDataBuf[1]+pDataBuf[33]);
			dwRealAmplitude += 565*(pDataBuf[2]+pDataBuf[16]-pDataBuf[18]-pDataBuf[32]);
			dwRealAmplitude += 1108*(pDataBuf[3]+pDataBuf[15]-pDataBuf[19]-pDataBuf[31]);
			dwRealAmplitude += 1609*(pDataBuf[4]+pDataBuf[14]-pDataBuf[20]-pDataBuf[30]);
			dwRealAmplitude += 2048*(pDataBuf[5]+pDataBuf[13]-pDataBuf[21]-pDataBuf[29]);
			dwRealAmplitude += 2408*(pDataBuf[6]+pDataBuf[12]-pDataBuf[22]-pDataBuf[28]);
			dwRealAmplitude += 2676*(pDataBuf[7]+pDataBuf[11]-pDataBuf[23]-pDataBuf[27]);
			dwRealAmplitude += 2841*(pDataBuf[8]+pDataBuf[10]-pDataBuf[24]-pDataBuf[26]);
			dwRealAmplitude += 2896*(pDataBuf[9]-pDataBuf[25]);
			dwRealAmplitude += 0*(-pDataBuf[17]);
			if(dwRealAmplitude>=0)                                                           /*四舍五入保证精度*/
				elec->Real = (dwRealAmplitude+32767)>>16;
			else
				elec->Real = (dwRealAmplitude-32767)>>16;

			dwImagAmplitude += 1448*(pDataBuf[1]+pDataBuf[33]);
			dwImagAmplitude += 2841*(pDataBuf[2]-pDataBuf[16]-pDataBuf[18]+pDataBuf[32]);
			dwImagAmplitude += 2676*(pDataBuf[3]-pDataBuf[15]-pDataBuf[19]+pDataBuf[31]);
			dwImagAmplitude += 2408*(pDataBuf[4]-pDataBuf[14]-pDataBuf[20]+pDataBuf[30]);
			dwImagAmplitude += 2048*(pDataBuf[5]-pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
			dwImagAmplitude += 1609*(pDataBuf[6]-pDataBuf[12]-pDataBuf[22]+pDataBuf[28]);
			dwImagAmplitude += 1108*(pDataBuf[7]-pDataBuf[11]-pDataBuf[23]+pDataBuf[27]);
			dwImagAmplitude += 565*(pDataBuf[8]-pDataBuf[10]-pDataBuf[24]+pDataBuf[26]);
			dwImagAmplitude += 0*(-pDataBuf[9]-pDataBuf[25]);
			dwImagAmplitude += 2896*(-pDataBuf[17]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+32767)>>16;
			else
				elec->Imag = (dwImagAmplitude-32767)>>16;
		}
}
/************************************************************
	功能：傅氏积分基波算法，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/

void CaluBaseFourier_S15(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14703*(-pDataBuf[1]+pDataBuf[33]);
			dwRealAmplitude += 565*(pDataBuf[2]+pDataBuf[16]-pDataBuf[18]-pDataBuf[32]);
			dwRealAmplitude += 1108*(pDataBuf[3]+pDataBuf[15]-pDataBuf[19]-pDataBuf[31]);
			dwRealAmplitude += 1609*(pDataBuf[4]+pDataBuf[14]-pDataBuf[20]-pDataBuf[30]);
			dwRealAmplitude += 2048*(pDataBuf[5]+pDataBuf[13]-pDataBuf[21]-pDataBuf[29]);
			dwRealAmplitude += 2408*(pDataBuf[6]+pDataBuf[12]-pDataBuf[22]-pDataBuf[28]);
			dwRealAmplitude += 2676*(pDataBuf[7]+pDataBuf[11]-pDataBuf[23]-pDataBuf[27]);
			dwRealAmplitude += 2841*(pDataBuf[8]+pDataBuf[10]-pDataBuf[24]-pDataBuf[26]);
			dwRealAmplitude += 2896*(pDataBuf[9]-pDataBuf[25]);
//			dwRealAmplitude += 0*(-pDataBuf[17]);

			elec->Real  = dwRealAmplitude>>15;
			
//			if(dwRealAmplitude>=0)                                                          //四舍五入保证精度
//				elec->Real = (dwRealAmplitude+16384)>>15;
//			else
//				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 1448*(pDataBuf[1]+pDataBuf[33]);
			dwImagAmplitude += 2841*(pDataBuf[2]-pDataBuf[16]-pDataBuf[18]+pDataBuf[32]);
			dwImagAmplitude += 2676*(pDataBuf[3]-pDataBuf[15]-pDataBuf[19]+pDataBuf[31]);
			dwImagAmplitude += 2408*(pDataBuf[4]-pDataBuf[14]-pDataBuf[20]+pDataBuf[30]);
			dwImagAmplitude += 2048*(pDataBuf[5]-pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
			dwImagAmplitude += 1609*(pDataBuf[6]-pDataBuf[12]-pDataBuf[22]+pDataBuf[28]);
			dwImagAmplitude += 1108*(pDataBuf[7]-pDataBuf[11]-pDataBuf[23]+pDataBuf[27]);
			dwImagAmplitude += 565*(pDataBuf[8]-pDataBuf[10]-pDataBuf[24]+pDataBuf[26]);
//			dwImagAmplitude += 0*(-pDataBuf[9]-pDataBuf[25]);
			dwImagAmplitude += 2896*(-pDataBuf[17]);

			elec->Imag = dwImagAmplitude>>15;
			
//			if(dwImagAmplitude>=0)
//				elec->Imag = (dwImagAmplitude+16384)>>15;
//			else
//				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	功能：傅氏积分基波算法，除14
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluBaseFourier_S14(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14703*(-pDataBuf[0]+pDataBuf[32]);
			dwRealAmplitude += 565*(pDataBuf[1]+pDataBuf[15]-pDataBuf[17]-pDataBuf[31]);
			dwRealAmplitude += 1108*(pDataBuf[2]+pDataBuf[14]-pDataBuf[18]-pDataBuf[30]);
			dwRealAmplitude += 1609*(pDataBuf[3]+pDataBuf[13]-pDataBuf[19]-pDataBuf[29]);
			dwRealAmplitude += 2048*(pDataBuf[4]+pDataBuf[12]-pDataBuf[20]-pDataBuf[28]);
			dwRealAmplitude += 2408*(pDataBuf[5]+pDataBuf[11]-pDataBuf[21]-pDataBuf[27]);
			dwRealAmplitude += 2676*(pDataBuf[6]+pDataBuf[10]-pDataBuf[22]-pDataBuf[26]);
			dwRealAmplitude += 2841*(pDataBuf[7]+pDataBuf[9]-pDataBuf[23]-pDataBuf[25]);
			dwRealAmplitude += 2896*(pDataBuf[8]-pDataBuf[24]);
			dwRealAmplitude += 0*(-pDataBuf[16]);
			if(dwRealAmplitude>=0)                                                           /*四舍五入保证精度*/
				elec->Real = (dwRealAmplitude+8192)>>14;
			else
				elec->Real = (dwRealAmplitude-8192)>>14;

			dwImagAmplitude += 1448*(pDataBuf[0]+pDataBuf[32]);
			dwImagAmplitude += 2841*(pDataBuf[1]-pDataBuf[15]-pDataBuf[17]+pDataBuf[31]);
			dwImagAmplitude += 2676*(pDataBuf[2]-pDataBuf[14]-pDataBuf[18]+pDataBuf[30]);
			dwImagAmplitude += 2408*(pDataBuf[3]-pDataBuf[13]-pDataBuf[19]+pDataBuf[29]);
			dwImagAmplitude += 2048*(pDataBuf[4]-pDataBuf[12]-pDataBuf[20]+pDataBuf[28]);
			dwImagAmplitude += 1609*(pDataBuf[5]-pDataBuf[11]-pDataBuf[21]+pDataBuf[27]);
			dwImagAmplitude += 1108*(pDataBuf[6]-pDataBuf[10]-pDataBuf[22]+pDataBuf[26]);
			dwImagAmplitude += 565*(pDataBuf[7]-pDataBuf[9]-pDataBuf[23]+pDataBuf[25]);
			dwImagAmplitude += 0*(-pDataBuf[8]-pDataBuf[24]);
			dwImagAmplitude += 2896*(-pDataBuf[16]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+8192)>>14;
			else
				elec->Imag = (dwImagAmplitude-8192)>>14;
		}
}

/************************************************************
	功能：傅氏积分二次谐波计算，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluSecondFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 7103*(-pDataBuf[0]);
			dwRealAmplitude += 1365*(pDataBuf[1]-pDataBuf[9]+pDataBuf[17]-pDataBuf[25]);
			dwRealAmplitude += 2239*(pDataBuf[2]-pDataBuf[10]+pDataBuf[18]-pDataBuf[26]);
			dwRealAmplitude += 2772*(pDataBuf[3]-pDataBuf[11]+pDataBuf[19]-pDataBuf[27]);
			dwRealAmplitude += 2882*(pDataBuf[4]-pDataBuf[12]+pDataBuf[20]-pDataBuf[28]);
			dwRealAmplitude += 2554*(pDataBuf[5]-pDataBuf[13]+pDataBuf[21]-pDataBuf[29]);
			dwRealAmplitude += 1837*(pDataBuf[6]-pDataBuf[14]+pDataBuf[22]-pDataBuf[30]);
			dwRealAmplitude += 841*(pDataBuf[7]-pDataBuf[15]+pDataBuf[23]-pDataBuf[31]);
			dwRealAmplitude += 284*(-pDataBuf[8]+pDataBuf[16]-pDataBuf[24]);
			dwRealAmplitude += 7387*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 2155*(pDataBuf[0]);
			dwImagAmplitude += 2554*(pDataBuf[1]-pDataBuf[9]+pDataBuf[17]-pDataBuf[25]);
			dwImagAmplitude += 1837*(pDataBuf[2]-pDataBuf[10]+pDataBuf[18]-pDataBuf[26]);
			dwImagAmplitude += 841*(pDataBuf[3]-pDataBuf[11]+pDataBuf[19]-pDataBuf[27]);
			dwImagAmplitude += 284*(-pDataBuf[4]+pDataBuf[12]-pDataBuf[20]+pDataBuf[28]);
			dwImagAmplitude += 1365*(-pDataBuf[5]+pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
			dwImagAmplitude += 2239*(-pDataBuf[6]+pDataBuf[14]-pDataBuf[22]+pDataBuf[30]);
			dwImagAmplitude += 2772*(-pDataBuf[7]+pDataBuf[15]-pDataBuf[23]+pDataBuf[31]);
			dwImagAmplitude += 2882*(-pDataBuf[8]+pDataBuf[16]-pDataBuf[24]);
			dwImagAmplitude += 728*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	功能：傅氏积分三次谐波计算，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluThirdFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 4400*(-pDataBuf[0]);
			dwRealAmplitude += 2048*(pDataBuf[1]-pDataBuf[9]-pDataBuf[17]+pDataBuf[25]);
			dwRealAmplitude += 2841*(pDataBuf[2]-pDataBuf[8]-pDataBuf[18]+pDataBuf[24]);
			dwRealAmplitude += 2676*(pDataBuf[3]-pDataBuf[7]-pDataBuf[19]+pDataBuf[23]);
			dwRealAmplitude += 1609*(pDataBuf[4]-pDataBuf[6]-pDataBuf[20]+pDataBuf[22]);
			dwRealAmplitude += 0*(-pDataBuf[5]-pDataBuf[21]);
			dwRealAmplitude += 565*(-pDataBuf[10]-pDataBuf[16]+pDataBuf[26]);
			dwRealAmplitude += 1108*(pDataBuf[11]+pDataBuf[15]-pDataBuf[27]-pDataBuf[31]);
			dwRealAmplitude += 2408*(pDataBuf[12]+pDataBuf[14]-pDataBuf[28]-pDataBuf[30]);
			dwRealAmplitude += 2896*(pDataBuf[13]-pDataBuf[29]);
			dwRealAmplitude += 4965*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;
			
			dwImagAmplitude += 2352*(pDataBuf[0]);
			dwImagAmplitude += 2048*(pDataBuf[1]+pDataBuf[9]-pDataBuf[17]-pDataBuf[25]);
			dwImagAmplitude += 565*(pDataBuf[2]+pDataBuf[8]-pDataBuf[18]-pDataBuf[24]);
			dwImagAmplitude += 1108*(-pDataBuf[3]-pDataBuf[7]+pDataBuf[19]+pDataBuf[23]);
			dwImagAmplitude += 2408*(-pDataBuf[4]-pDataBuf[6]+pDataBuf[20]+pDataBuf[22]);
			dwImagAmplitude += 2896*(-pDataBuf[5]+pDataBuf[21]);
			dwImagAmplitude += 2841*(pDataBuf[10]-pDataBuf[16]-pDataBuf[26]);
			dwImagAmplitude += 2676*(pDataBuf[11]-pDataBuf[15]-pDataBuf[27]+pDataBuf[31]);
			dwImagAmplitude += 1609*(pDataBuf[12]-pDataBuf[14]-pDataBuf[28]+pDataBuf[30]);
			dwImagAmplitude += 0*(-pDataBuf[13]-pDataBuf[29]);
			dwImagAmplitude += 489*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	功能：傅氏积分五次谐波计算，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluFifthFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 1949*(-pDataBuf[0]);
			dwRealAmplitude += 2841*(pDataBuf[1]-pDataBuf[11]-pDataBuf[17]+pDataBuf[27]);
			dwRealAmplitude += 2048*(pDataBuf[2]-pDataBuf[10]-pDataBuf[18]+pDataBuf[26]);
			dwRealAmplitude += 565*(-pDataBuf[3]+pDataBuf[9]+pDataBuf[19]-pDataBuf[25]);
			dwRealAmplitude += 2676*(-pDataBuf[4]+pDataBuf[8]+pDataBuf[20]-pDataBuf[24]);
			dwRealAmplitude += 2408*(-pDataBuf[5]+pDataBuf[7]+pDataBuf[21]-pDataBuf[23]);
			dwRealAmplitude += 0*(-pDataBuf[6]-pDataBuf[22]);
			dwRealAmplitude += 1108*(-pDataBuf[12]-pDataBuf[16]+pDataBuf[28]);
			dwRealAmplitude += 1609*(pDataBuf[13]+pDataBuf[15]-pDataBuf[29]-pDataBuf[31]);
			dwRealAmplitude += 2896*(pDataBuf[14]-pDataBuf[30]);
			dwRealAmplitude += 3057*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 2375*(pDataBuf[0]);
			dwImagAmplitude += 565*(pDataBuf[1]+pDataBuf[11]-pDataBuf[17]-pDataBuf[27]);
			dwImagAmplitude += 2048*(-pDataBuf[2]-pDataBuf[10]+pDataBuf[18]+pDataBuf[26]);
			dwImagAmplitude += 2841*(-pDataBuf[3]-pDataBuf[9]+pDataBuf[19]+pDataBuf[25]);
			dwImagAmplitude += 1108*(-pDataBuf[4]-pDataBuf[8]+pDataBuf[20]+pDataBuf[24]);
			dwImagAmplitude += 1609*(pDataBuf[5]+pDataBuf[7]-pDataBuf[21]-pDataBuf[23]);
			dwImagAmplitude += 2896*(pDataBuf[6]-pDataBuf[22]);
			dwImagAmplitude += 2676*(pDataBuf[12]-pDataBuf[16]-pDataBuf[28]);
			dwImagAmplitude += 2408*(pDataBuf[13]-pDataBuf[15]-pDataBuf[29]+pDataBuf[31]);
			dwImagAmplitude += 0*(-pDataBuf[14]-pDataBuf[30]);
			dwImagAmplitude += 301*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;	
		}
}
/************************************************************
	功能：傅氏积分七次谐波计算，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluSeventhFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 663*(-pDataBuf[0]);
			dwRealAmplitude += 2676*(pDataBuf[1]+pDataBuf[5]-pDataBuf[17]-pDataBuf[21]);
			dwRealAmplitude += 565*(-pDataBuf[2]-pDataBuf[4]+pDataBuf[18]+pDataBuf[20]);
			dwRealAmplitude += 2896*(-pDataBuf[3]+pDataBuf[19]);
			dwRealAmplitude += 1609*(pDataBuf[6]-pDataBuf[16]-pDataBuf[22]);
			dwRealAmplitude += 2048*(-pDataBuf[7]+pDataBuf[15]+pDataBuf[23]-pDataBuf[31]);
			dwRealAmplitude += 2408*(-pDataBuf[8]+pDataBuf[14]+pDataBuf[24]-pDataBuf[30]);
			dwRealAmplitude += 1108*(pDataBuf[9]-pDataBuf[13]-pDataBuf[25]+pDataBuf[29]);
			dwRealAmplitude += 2841*(pDataBuf[10]-pDataBuf[12]-pDataBuf[26]+pDataBuf[28]);
			dwRealAmplitude += 0*(-pDataBuf[11]-pDataBuf[27]);
			dwRealAmplitude += 2272*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 2184*(pDataBuf[0]);
			dwImagAmplitude += 1108*(-pDataBuf[1]+pDataBuf[5]+pDataBuf[17]-pDataBuf[21]);
			dwImagAmplitude += 2841*(-pDataBuf[2]+pDataBuf[4]+pDataBuf[18]-pDataBuf[20]);
			dwImagAmplitude += 0*(-pDataBuf[3]-pDataBuf[19]);
			dwImagAmplitude += 2408*(-pDataBuf[6]-pDataBuf[16]+pDataBuf[22]);
			dwImagAmplitude += 2048*(-pDataBuf[7]-pDataBuf[15]+pDataBuf[23]+pDataBuf[31]);
			dwImagAmplitude += 1609*(pDataBuf[8]+pDataBuf[14]-pDataBuf[24]-pDataBuf[30]);
			dwImagAmplitude += 2676*(pDataBuf[9]+pDataBuf[13]-pDataBuf[25]-pDataBuf[29]);
			dwImagAmplitude += 565*(-pDataBuf[10]-pDataBuf[12]+pDataBuf[26]+pDataBuf[28]);
			dwImagAmplitude += 2896*(-pDataBuf[11]+pDataBuf[27]);
			dwImagAmplitude += 224*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	功能：傅氏积分九次谐波计算，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluNinthFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 184*(pDataBuf[0]);
			dwRealAmplitude += 1609*(pDataBuf[1]+pDataBuf[7]-pDataBuf[17]-pDataBuf[23]);
			dwRealAmplitude += 2676*(-pDataBuf[2]-pDataBuf[6]+pDataBuf[18]+pDataBuf[22]);
			dwRealAmplitude += 565*(-pDataBuf[3]-pDataBuf[5]+pDataBuf[19]+pDataBuf[21]);
			dwRealAmplitude += 2896*(pDataBuf[4]-pDataBuf[20]);
			dwRealAmplitude += 2048*(pDataBuf[8]-pDataBuf[16]-pDataBuf[24]);
			dwRealAmplitude += 2408*(-pDataBuf[9]+pDataBuf[15]+pDataBuf[25]-pDataBuf[31]);
			dwRealAmplitude += 1108*(-pDataBuf[10]+pDataBuf[14]+pDataBuf[26]-pDataBuf[30]);
			dwRealAmplitude += 2841*(pDataBuf[11]-pDataBuf[13]-pDataBuf[27]+pDataBuf[29]);
			dwRealAmplitude += 0*(-pDataBuf[12]-pDataBuf[28]);
			dwRealAmplitude += 1864*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 1864*(pDataBuf[0]);
			dwImagAmplitude += 2408*(-pDataBuf[1]+pDataBuf[7]+pDataBuf[17]-pDataBuf[23]);
			dwImagAmplitude += 1108*(-pDataBuf[2]+pDataBuf[6]+pDataBuf[18]-pDataBuf[22]);
			dwImagAmplitude += 2841*(pDataBuf[3]-pDataBuf[5]-pDataBuf[19]+pDataBuf[21]);
			dwImagAmplitude += 0*(-pDataBuf[4]-pDataBuf[20]);
			dwImagAmplitude += 2048*(-pDataBuf[8]-pDataBuf[16]+pDataBuf[24]);
			dwImagAmplitude += 1609*(-pDataBuf[9]-pDataBuf[15]+pDataBuf[25]+pDataBuf[31]);
			dwImagAmplitude += 2676*(pDataBuf[10]+pDataBuf[14]-pDataBuf[26]-pDataBuf[30]);
			dwImagAmplitude += 565*(pDataBuf[11]+pDataBuf[13]-pDataBuf[27]-pDataBuf[29]);
			dwImagAmplitude += 2896*(-pDataBuf[12]+pDataBuf[28]);
			dwImagAmplitude += 184*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	功能：傅氏积分十一次谐波计算，除15
	参数：pDataBuf	--	傅里叶数据缓冲区
		  elec		--	待计算电量结构
	返回：无
************************************************************/
void CaluEleventhFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 774*(pDataBuf[0]);
		dwRealAmplitude += 0*(-pDataBuf[1]-pDataBuf[17]);
		dwRealAmplitude += 2408*(-pDataBuf[2]-pDataBuf[16]+pDataBuf[18]);
		dwRealAmplitude += 2676*(pDataBuf[3]+pDataBuf[15]-pDataBuf[19]-pDataBuf[31]);
		dwRealAmplitude += 565*(-pDataBuf[4]-pDataBuf[14]+pDataBuf[20]+pDataBuf[30]);
		dwRealAmplitude += 2048*(-pDataBuf[5]-pDataBuf[13]+pDataBuf[21]+pDataBuf[29]);
		dwRealAmplitude += 2841*(pDataBuf[6]+pDataBuf[12]-pDataBuf[22]-pDataBuf[28]);
		dwRealAmplitude += 1108*(-pDataBuf[7]-pDataBuf[11]+pDataBuf[23]+pDataBuf[27]);
		dwRealAmplitude += 1609*(-pDataBuf[8]-pDataBuf[10]+pDataBuf[24]+pDataBuf[26]);
		dwRealAmplitude += 2896*(pDataBuf[9]-pDataBuf[25]);
		dwRealAmplitude += 1634*(pDataBuf[32]);
		if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;
		dwImagAmplitude += 1448*(pDataBuf[0]);
		dwImagAmplitude += 2896*(-pDataBuf[1]+pDataBuf[17]);
		dwImagAmplitude += 1609*(pDataBuf[2]-pDataBuf[16]-pDataBuf[18]);
		dwImagAmplitude += 1108*(pDataBuf[3]-pDataBuf[15]-pDataBuf[19]+pDataBuf[31]);
		dwImagAmplitude += 2841*(-pDataBuf[4]+pDataBuf[14]+pDataBuf[20]-pDataBuf[30]);
		dwImagAmplitude += 2048*(pDataBuf[5]-pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
		dwImagAmplitude += 565*(pDataBuf[6]-pDataBuf[12]-pDataBuf[22]+pDataBuf[28]);
		dwImagAmplitude += 2676*(-pDataBuf[7]+pDataBuf[11]+pDataBuf[23]-pDataBuf[27]);
		dwImagAmplitude += 2408*(pDataBuf[8]-pDataBuf[10]-pDataBuf[24]+pDataBuf[26]);
		dwImagAmplitude += 0*(-pDataBuf[9]-pDataBuf[25]);
		dwImagAmplitude += 161*(pDataBuf[32]);
		if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
	}
}

/************************************************************
	功能：已知实部、虚部计算模值
	参数：elec		--	待计算电量结构，需已进行实部虚部计算
	返回：无
************************************************************/
void CaluModValue(TRelElecValPar *elec)
{
	DWORD dwBig,dwSmall,dwTemp1;
	if(elec!=NULL)
	{
		dwBig   = ToAbs(elec->Imag);
		dwSmall = ToAbs(elec->Real);
		if (dwBig!=dwSmall)
		{
			if(dwSmall>dwBig)
			{
				dwTemp1 = dwSmall;
				dwSmall = dwBig;
				dwBig   = dwTemp1; 
			}												     /*四舍五入保证精度*/
			dwTemp1 = dwBig+(dwSmall*10+15)/30;                                /* B+S/3 */
			if(dwTemp1<2)
			{
				elec->Mod =0;
				return;
			}
			dwTemp1 = dwTemp1+(dwBig*dwBig+dwSmall*dwSmall+dwTemp1/2)/dwTemp1;  /* B+S/3+(B*B+S*S)/(B+S/3) */
			elec->Mod = (dwTemp1+1)/2;
		}
		else
			elec->Mod = (dwBig*1414+500)/1000;
	}
	
}
/************************************************************
	功能：求阻抗的角度
	参数：elec		--	待计算电量结构，需已进行实部虚部计算
	返回：无
************************************************************/
void CaluImpdAngle(TRelElecValPar *pElec,const WORD *pAngleTab)
{
	TRelElecValPar elec;
	DWORD dwBig,dwSmall,dwTemp1;
	
	elec.Real = pElec->Real/10;
	elec.Imag = pElec->Imag/10;
	
	dwBig   = ToAbs(elec.Imag);
	dwSmall = ToAbs(elec.Real);
	if (dwBig!=dwSmall)
	{
		if(dwSmall>dwBig)
		{
			dwTemp1 = dwSmall;
			dwSmall = dwBig;
			dwBig   = dwTemp1; 
		}												     /*四舍五入保证精度*/
		dwTemp1 = dwBig+(dwSmall)/3;                                /* B+S/3 */
		dwTemp1 = dwTemp1+(dwBig*dwBig+dwSmall*dwSmall)/dwTemp1;  /* B+S/3+(B*B+S*S)/(B+S/3) */
		elec.Mod = dwTemp1/2;
	}
	else
		elec.Mod = (dwBig*1414+500)/1000;

	CaluAngle(&elec,pAngleTab);
	
	pElec->Mod   = elec.Mod*10;
	pElec->Angle = elec.Angle;
	
}

/************************************************************
	功能：角度计算
	参数：elec		--	待计算电量结构，需已进行实部虚部模值计算
		  pAngleTab	--	角度计算查询表基址
	返回：无
************************************************************/
void CaluAngle(TRelElecValPar *elec,const WORD *pAngleTab)
{
	LONG swReal;
	LONG swImag;
	DWORD  dwMod,dwReal,dwImag,dwAsin,dwangle;
	
	if (elec !=0)
	{
		swReal 	= elec->Real;
		swImag 	= elec->Imag;
		dwMod  	= elec->Mod;
		dwReal 	= ToAbs(swReal);
		dwImag 	= ToAbs(swImag);
		
		if((swReal==0)&&(swImag>0))			  /*判断电量是否位于坐标轴上*/
				dwangle = 9000;			
		if((swReal==0)&&(swImag<0))	
				dwangle = 27000;			
		if((swImag==0)&&(swReal>0))		
				dwangle = 0;			
		if((swImag==0)&&(swReal<0))			
				dwangle = 18000;

		if ((dwMod>1)&&swReal&&swImag)    
		{
			if(dwImag!=dwReal)
			{
				if(dwImag<dwReal)              /*如果虚部小于实部计算所得为正弦值*/
					dwAsin = (dwImag*6364)/(10*dwMod);
				else						   /*如果虚部大于实部计算所得为余弦值*/
					dwAsin = (dwReal*6364)/(10*dwMod);	

				if(dwAsin>449)  //下标越界
				{
					elec->Angle = 0;
					return;
				}
				dwangle = pAngleTab[dwAsin];				
				if(dwImag>dwReal)			   /*若为余弦，转换成正弦角度*/
					dwangle = 9000-dwangle;
			}
			else
				dwangle = 4500;
			
			if (swReal<0&&swImag>0)	           /*根据虚部实部的正负判断该电量位于哪一象限*/
				dwangle = 18000-dwangle;
			if (swReal<0&&swImag<0)
				dwangle = 18000+dwangle;
			if (swReal>0&&swImag<0)
				dwangle = 36000-dwangle;
			
		}
	//	dwangle = dwangle - BASEANGLE;  /*临时代码，此处为角度基准*/ 
	//	if(dwangle<=0)
	//		dwangle += 36000;			
		elec->Angle = dwangle;		
	}
}
/************************************************************
	功能：通道滤波
	参数：pDataBuf	--	待滤波通道原始数据缓冲区
	返回：无
************************************************************/
void Filter(LONG  pDataBuf[READ_POINT_NUM])
{
	LONG DCComponent=0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		DCComponent +=pDataBuf[i]; 
	}
	DCComponent=DCComponent/POINT_PER_PERIOD;
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		pDataBuf[i]=pDataBuf[i]-DCComponent;
	}

}
/************************************************************
	功能：32位迭代开方
	参数：par			--	待开方数
	返回：无
************************************************************/
WORD Evolution32(DWORD par)
{
	WORD SquareRoot=0x8000;
	if(par<0)
		return 0;
	for(LONG i=0;i<16;i++)
	{
		SquareRoot |= (1<<(15-i));			
		if(par < (DWORD)SquareRoot*SquareRoot)
			SquareRoot &=~(1<<(15-i));
	}
	return SquareRoot;
}
/************************************************************
	功能：64位迭代开方
	参数：par			--	待开方数
	返回：无
************************************************************/
DWORD Evolution64(QWORD par)
{
	DWORD SquareRoot=0x80000000;
	if(par<0)
		return 0;
	for(LONG i=0;i<32;i++)
	{
		SquareRoot |= (1<<(31-i));			
		if(par < (QWORD)SquareRoot*SquareRoot)
			SquareRoot &=~(1<<(31-i));
	}
	return SquareRoot;
}

/************************************************************
	功能：均方根值求模值
	参数：DataBuf		--	采样点数据缓冲区
		  Elec		--   引用参数，计算所得模值
	返回：无
************************************************************/
void  CaluMSMod(const LONG *DataBuf,LONG &Elec)
{
	LLONG tElec = 0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		tElec += DataBuf[i+2]*DataBuf[i+2];
	}
	
	Elec = Evolution64(tElec)/2;		//注意，此处应该除根号下32，这里只除 了4，即模值放大了2.828倍

}
/************************************************************
	功能：冒泡排序
	参数：Buf			--   待排序数据缓冲区
		  BufSize		--   数据缓冲区大小
	返回：无
************************************************************/
void BubbleSort(LONG *Buf,LONG BufSize)						//冒泡法排序 
{
	LONG i,j;
	LONG temp;
	for (i=1;i<BufSize;i++)
	{
		for(j=BufSize-1;j>=i;j--) 
		{
			if (Buf[j]<Buf[j-1])
			{
				temp = Buf[j];
				Buf[j] = Buf[j-1];
				Buf[j-1] = temp;
			}
		}
	}	
}

/************************************************************
	功能：向数据库中写入遥测值---已排序后的中值
	参数：no			--   遥测序号
		  val			--   遥测值
		  BubbleData	--   排序缓冲区
		  OriginalData	--   待排序数据缓冲区
	返回：无
************************************************************/
void WriteAIData(WORD no,LONG val, LONG *BubbleData,LONG *OriginalData)
{
	LONG value;
	if(NUMOFYCVALUE>3)  //如果排序基数小于3，不进行排序
	{
		for(LONG i=0;i<NUMOFYCVALUE-1;i++)
			OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];
		
		OriginalData[0]=val;	
		for(LONG i=0;i<NUMOFYCVALUE;i++)
		{	
			BubbleData[i]=OriginalData[i];
		}
		BubbleSort(BubbleData,NUMOFYCVALUE);
		value = BubbleData[NUMOFYCVALUE/2];
	}
	else
		value = val;
	
	DB_WriteAI(no,value);

}
void WriteAIData(WORD no,LONG val, LONG Drift,LONG *BubbleData,LONG *OriginalData,BOOL haveDrift) //写遥测值
{
	LONG value;
	if(NUMOFYCVALUE>3)  //如果排序基数小于3，不进行排序
	{
		for(LONG i=0;i<NUMOFYCVALUE-1;i++)
			OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];
		
		OriginalData[0]=val;	
		for(LONG i=0;i<NUMOFYCVALUE;i++)
		{	
			BubbleData[i]=OriginalData[i];
		}
		BubbleSort(BubbleData,NUMOFYCVALUE);
		value = BubbleData[NUMOFYCVALUE/2];
	}
	else
		value = val;

	if(haveDrift)
	{
		if(value<Drift)
			value =0;
	}
	
	DB_WriteAI(no,value);
}
/************************************************************
	功能：向数据库中写入保护测量值
	参数：RMNo			--   保护测量序号
		  val			--   保护测量值
		  pRM			--   保护测量值地址
	返回：无
************************************************************/
void WriteRMData(LONG *pRM,DWORD RMNo, LONG val) //写遥测值
{
	*pRM = val;
	if(RMNo>=RM_COUNT)
	{
		ReportMsg("relayfun err! rm no overflow!");
		return;
	}
	DB_LogWriteRM(RMNo, val); 
}

/************************************************************
	功能：原始数据经过冒泡排序后取中间值
	参数：num			--   原始数据个数
		  val			--   值
		  OriginalData	--   待排序数据缓冲区
	返回：无
************************************************************/
LONG GetSortData(LONG val,WORD num,LONG *OriginalData,LONG *BubbleData)
{
	LONG value;	
	if(num>3)
	{
		for(LONG i=0;i<num-1;i++)
			OriginalData[num-1-i]=OriginalData[num-2-i];
		
		OriginalData[0]=val;	
		for(LONG i=0;i<num;i++)
		{	
			BubbleData[i]=OriginalData[i];
		}
		BubbleSort(BubbleData,num);
		value = BubbleData[num/2];
	}
	else
		value = val;

	return value;
	
}

/************************************************************
	功能：阻抗计算
	参数：Uele		--   电压指针
		  Iele		--   电流指针
		  Zele		--	所得阻抗指针
		  MinCur		--	小电流值
	返回：无
************************************************************/

void CaluRX(TRelElecValPar *Uele,TRelElecValPar *Iele,TRelElecValPar *Zele,DWORD RW50I,LONG *Zcoef)
{		
//	if(ReadRelayWord(RW50I))
	if(Iele->Mod<15)
	{		
#ifdef PRO_6571
		Zele->Real = (INVALIDRX*(*Zcoef)/(*AS_pnZRatio))*1000;
		Zele->Imag = (INVALIDRX*(*Zcoef)/(*AS_pnZRatio))*1000;	
#else
		Zele->Real = INVALIDRX*(*Zcoef);
		Zele->Imag = INVALIDRX*(*Zcoef);	
#endif
	}
	else      //考虑下这里的溢出问题 
	{
		LLONG temp1,temp2,temp3;
		temp1 = Iele->Real* Iele->Real + Iele->Imag*Iele->Imag;
		temp2 = ((LLONG)Iele->Real * Uele->Real + (LLONG)Iele->Imag * Uele->Imag)*IMPDMULTIPLE;
		temp3 = ((LLONG)Iele->Real * Uele->Imag -(LLONG)Iele->Imag * Uele->Real)*IMPDMULTIPLE;

		if(temp1<100)
		{				
#ifdef PRO_6571
			Zele->Real = INVALIDRX*(*Zcoef)/(*AS_pnZRatio);
			Zele->Imag = INVALIDRX*(*Zcoef)/(*AS_pnZRatio); 
#else
			Zele->Real = INVALIDRX*(*Zcoef);
			Zele->Imag = INVALIDRX*(*Zcoef);	
#endif
			return;
		}		
		Zele->Real 	= temp2 /temp1;
		Zele->Imag	= temp3 /temp1;
	}	
}
/************************************************************
	功能：简单过量保护
	参数：Relay		--   简单的过欠量参数结构
	返回：无
************************************************************/
void SimpleOverValRelay(TRelSimpGeneOvUnPar *Relay)
{
	if((Relay->pTEleVal->Mod) >(*(Relay->pEleSet)))
	{
		if(ReadRelayWord(Relay->ACTRelayWord))
			return;
		if(ReadRelayWord(Relay->StartRelayWord))
		{
			WriteRelayWord(Relay->ACTRelayWord, RELAY_ON);
			return;
		}
		else
		{
			WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
			return;
		}
	}

	if((Relay->pTEleVal->Mod) <(*(Relay->pEleSet))*99/100)
	{
		WriteRelayWord(Relay->ACTRelayWord, RELAY_OFF);
		WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	}
    else
  	{	     		
  		if(ReadRelayWord(Relay->StartRelayWord))
			WriteRelayWord(Relay->ACTRelayWord, RELAY_ON);
		else
			return;
	}
}
/************************************************************
	功能：简单欠量保护
	参数：Relay		--   简单的过欠量参数结构
	返回：无
************************************************************/
void SimpleUnderValRelay(TRelSimpGeneOvUnPar *Relay)
{
	if ((Relay->pTEleVal->Mod) < (*(Relay->pEleSet)))
	{
		if (!ReadRelayWord(Relay->ACTRelayWord))
		{
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
			return;
		}
		else
			return;
	}
	else
	{
		if ((Relay->pTEleVal->Mod)<=(*(Relay->pEleSet))*101/100)
			return;
		else
		{
			WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
		}
	}

}

/************************************************************
	功能：作保护动作报告
	参数：RptCategory	--   动作报告的Inf号
		  SerialNum	--	报告序列号，唯一，数据库组织用
		  RptRatioTab	--	故障电量显示一次值，变比表地址
	返回：无
************************************************************/
void MakeActRpt(WORD RptCategory,WORD SerialNum)
{
	if(RptCategory == RPT_NULL) //无效的INF号
		return;
	TRelayEventItem eventreport; 	

	
	//组成动作报告
	ReadAbsTime(&eventreport.stTime);	
	eventreport.bStatus  = 1;   //动作
	eventreport.nINF     = ReportInf[RptCategory].wInf;//INF号
	
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
		eventreport.FD_Value[j] = *(ReportInf[RptCategory].pFailureData[j]);
	
	DB_RelayAction(SerialNum,&eventreport);  //向数据库中写入动作报告
}
/************************************************************
	功能：作告警报告
	参数：RptCategory	--   告警报告的Inf号
		  RptRatioTab	--	故障电量显示一次值，变比表地址
	返回：无
************************************************************/
void MakeAlarmRpt(WORD RptCategory)
{
	TRelayEventItem eventreport; 	
	ReadAbsTime(&eventreport.stTime);	
	eventreport.bStatus  = 1;   //动作
	eventreport.nINF     = ReportInf[RptCategory].wInf;//INF号
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
	{
		eventreport.FD_Value[j] = *(ReportInf[RptCategory].pFailureData[j]);
	}
	DB_RelayAlarm(&eventreport);
}
/************************************************************
	功能：作带故障电量的启动报告
	参数：RptCategory	--   启动报告的Inf号
		  RptSerialNum	--   报告序列号，唯一，数据库组织用
	返回：无
************************************************************/

void MakeStartRpt(WORD RptCategory,WORD RptSerialNum )
{	
	LONG pFD[48]; //要保证启动报告的故障电量个数少于16
	
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
		pFD[j] = *(ReportInf[RptCategory].pFailureData[j]);

	DB_RelayWriteFD (RptSerialNum, pFD);
	DB_RelayEnd(RptSerialNum);		
}
/************************************************************
	功能：作带故障电量的入段报告
	参数：RptCategory	--   启动报告的Inf号
		  RptSerialNum	--   报告序列号，唯一，数据库组织用
	返回：无
************************************************************/
void MakePickupRpt(WORD RptCategory, WORD RptSerialNum)
{	
	LONG pFD[MAX_FD_NUM]; 
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
		pFD[j] = *(ReportInf[RptCategory].pFailureData[j]);
	DB_RelayWriteFD (RptSerialNum, pFD);
}
/************************************************************
	功能：通用过量保护
	参数：Relay		--  通用过量保护参数结构
		  dwK35		--  K35计算结果，谐波自适应用
		  RptSerialNum --  报告序列号，唯一，数据库组织报告用
	返回：无
************************************************************/
void GeneralOverValueRelay(TRelGeneOvValPar *Relay)
{	
	LONG ElecSet = *(Relay->pElecSet);

	if(*(Relay->pRelSW) == SW_STATUS_CUT)   //压板未投入，直接复位退出 
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay(Relay);   //保护未投入且加速未投或加速不允许，清标志，返回 
		return;
	}
		

	if (*(Relay->pRelCFG) == CFG_STATUS_CLR)  //保护未投入 
	{
		if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord)))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);   //保护未投入且加速未投或加速不允许，清标志，返回 
			return;
		}		
	}
	if (*(Relay->pRelHSuprCFG) == CFG_STATUS_SET) //谐波抑制投入 
	{
		//考虑电流为0的情况 
		if (Relay->pRelElec->Mod != 0)
			ElecSet = (*Relay->dwK35)*ElecSet/(*Relay->pH2Coef);
	}

	if (ReadRelayWord(Relay->AccActRelayWord) || ReadRelayWord(Relay->ActRelayWord)) //加速或者过量保护已经动作过 
	{	
		//过量保护的返回分两种情况
		// 1. 过流保护，跳闸成功后电流消失，保护返回
		// 2. 过压保护，跳闸成功后有可能仍然过压，此时需用其他条件返回
		if (((Relay->pRelElec->Mod) < (ElecSet*NOCURCOEF/100))||(!ReadRelayWord(Relay->RSTRelayWord)))
		{		
			Relay->byRelRCounter++;
			if(Relay->byRelRCounter>3)   //增加返回计数，yanzh 2012年8月23日14:04:07
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay(Relay);
			}
			return;
		}
		else
		{
			Relay->byRelRCounter=0;
			return;		
		}
	}
	
	if (Relay->pRelElec->Mod < ElecSet) //当前电流小于定值 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //保护入段了 
		{
			if (Relay->pRelElec->Mod < ElecSet*99/100) //保护已经入段且电流小于返回定值 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay(Relay);
				return;
			}			
		}
		else
		{
			if(Relay->pRelElec->Mod >= *(Relay->pElecSet))      //谐波抑制投入的情况下，当电流大于初始定值时保护不能返回
				return;
			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);
			return;		
		}
	}
	
	if (ReadRelayWord(Relay->LockRelayWord))
	{
		//入段闭锁继电器字置位
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay(Relay); 
		return;
	}	
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (*(Relay->pRelHLockCFG) == CFG_STATUS_SET)
	{
		if (ReadRelayWord(Relay->H2LockRelayWord))      //二次谐波闭锁投入且保护被闭锁 
		{
			ResetOverValueRelay(Relay);
			return;
		}
	}
	if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord))) //加速未投入或者加速不允许，进行功率方向判断 
	{
		if (*(Relay->pRelPDCFG) == CFG_STATUS_SET)   //过量保护功率方向投入 
		{
			if(*(Relay->pRelFDCFG) == CFG_STATUS_SET)   //过量保护正方向投入 
			{
				if(ReadRelayWord(Relay->PRewardRelayWord)&&(!ReadRelayWord(Relay->PForwardRelayWord)))  //反方向继电器字置位，正方向继电器字没有置位 
				{
					ResetOverValueRelay(Relay);
					return;
				}
			}
			else
			{
				if(ReadRelayWord(Relay->PForwardRelayWord)&&(!ReadRelayWord(Relay->PRewardRelayWord)))   //正方向继电器字置位，反方向继电器字没有置位 
				{
					ResetOverValueRelay(Relay);
					return;
				}				
			}
		}
	}
	
	if (*(Relay->pRelVolLockCFG) == CFG_STATUS_SET)
	{
		if ((Relay->pRelVolt->Mod) > (*(Relay->pVolLockSet)))     //低压闭锁投入且电压大于定值 
		{
			ResetOverValueRelay(Relay);
			return;
		}
	}
	if (*(Relay->pRel47UCFG) == CFG_STATUS_SET)   //复压闭锁功能
	{
		if(ReadRelayWord(Relay->R47ULKRelayWord))
		{
			ResetOverValueRelay(Relay);
			return;
		}
	}
	if (!ReadRelayWord(Relay->StartRelayWord))   //保护没有入段 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>=2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			if((*(Relay->pRelAccCFG) == CFG_STATUS_SET)&&(ReadRelayWord(Relay->AccRelayWord))) //yanzh 2012-5-7 15:21:11
			{
				Relay->wPickUpType = ACCPICK;
				MakeActRpt(Relay->wAccPickUpRptNum,*Relay->RptSerialNum);
			}
			else
			{				
				if(Relay->bMakeActRpt == TRUE)
					MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
				Relay->wPickUpType = NORMALPICK;
			}
			return;        //北京型式实验注释掉，减少保护动作时间，1次入段，2次跳闸
		}
		else
			return;
	}  
	
	if ((*(Relay->pRelAccCFG) == CFG_STATUS_SET) && (Relay->wPickUpType == ACCPICK))
	{															   //加速投入且加速允许了，加速动作 	
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pAccActTimeSet)))
		{
			WriteRelayWord(Relay->AccActRelayWord,RELAY_ON);      //延时已到，将加速动作继电器字置位 
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //延时已到，同时将动作继电器字置位 
			MakeActRpt(Relay->wAccReportNum,*Relay->RptSerialNum);
			Relay->byRelRCounter = 0;
			return;
		}		
	}
	if(*(Relay->pRelCFG) == CFG_STATUS_SET)
	{
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pTimeSet)))
		{
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //延时已到，将继电器字置位 			
			Relay->byRelRCounter = 0;
			if(Relay->bMakeActRpt == TRUE)
			{
				MakeActRpt(Relay->wReportNum,*Relay->RptSerialNum);
			}
			else
				MakeAlarmRpt(Relay->wReportNum);
		}
	}

}
/************************************************************
	功能：通用过量保护,只置位继电器字，不带谐波抑制功能
	参数：Relay		--  通用过量保护参数结构
		  dwK35		--  K35计算结果，谐波自适应用
		  RptSerialNum --  报告序列号，唯一，数据库组织报告用
	返回：无
************************************************************/
void GeneralOverValueRelay_2(TRelGeneOvValPar *Relay)
{	
	LONG ElecSet = *(Relay->pElecSet);

	if((*(Relay->pRelSW) == SW_STATUS_CUT)||(*(Relay->pRelCFG) == CFG_STATUS_CLR))   //压板未投入，直接复位退出 
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay(Relay);   //保护未投入且加速未投或加速不允许，清标志，返回 
		return;
	}
	if (ReadRelayWord(Relay->AccActRelayWord) || ReadRelayWord(Relay->ActRelayWord)) //加速或者过量保护已经动作过 
	{	
		//过量保护的返回分两种情况
		// 1. 过流保护，跳闸成功后电流消失，保护返回
		// 2. 过压保护，跳闸成功后有可能仍然过压，此时需用其他条件返回
		if (((Relay->pRelElec->Mod) < (ElecSet*NOCURCOEF/100))||(!ReadRelayWord(Relay->RSTRelayWord)))
		{		
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);
			return;
		}
		else			
			return;		
	}
	
	if (Relay->pRelElec->Mod < ElecSet) //当前电流小于定值 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //保护入段了 
		{
			if (Relay->pRelElec->Mod < ElecSet*99/100) //保护已经入段且电流小于返回定值 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay(Relay);
				return;
			}			
		}
		else
		{			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);
			return;		
		}
	}
	
	if (ReadRelayWord(Relay->LockRelayWord))
	{
		ResetOverValueRelay(Relay); //入段继电器字有意义且入段被闭锁
		return;
	}
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (!ReadRelayWord(Relay->StartRelayWord))   //保护没有入段 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			return;
		}
		else
			return;
	}  
	if ((*(Relay->pSysTimer) - Relay->dwRelTimer + INHERENTTIME) > (*(Relay->pTimeSet)))
	{
		WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //延时已到，将继电器字置位 
	}

}

/************************************************************
	功能：复位通用过量保护
	参数：Relay		--  通用过量保护参数结构
	返回：无
************************************************************/
void ResetOverValueRelay(TRelGeneOvValPar *Relay,BOOL resetFlag)
{	
	//if (ReadRelayWord(Relay->StartRelayWord)&&(*Relay->RptPickupFlag == TRUE))   //保护入段 
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //保护入段 
	{
		if(Relay->wPickUpType == ACCPICK)
		{
			MakeActRpt(Relay->wAccResetRptNum, *Relay->RptSerialNum);	
		}
		else
		{
			if(Relay->bMakeActRpt == TRUE)
			MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
		}
	}
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);    
	WriteRelayWord(Relay->AccActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ActRelayWord,RELAY_OFF);
	Relay->dwRelTimer = 0;
	Relay->byRelFCounter =0;
	Relay->byRelRCounter = 0;	
}
/************************************************************
	功能：通用过量保护---两个电量判断
	参数：Relay		--  通用过量保护参数结构
		  dwK35		--  K35计算结果，谐波自适应用
		  RptSerialNum --  报告序列号，唯一，数据库组织报告用
	返回：无
************************************************************/
void GeneralOverValueRelay_3(TRelGeneOvValPar_2 *Relay)
{	
	LONG ElecSet = *(Relay->pElec1Set);

	if(*(Relay->pRelSW) == SW_STATUS_CUT)   //压板未投入，直接复位退出 
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay_3(Relay);   //保护未投入且加速未投或加速不允许，清标志，返回 
		return;
	}
		

	if (*(Relay->pRelCFG) == CFG_STATUS_CLR)  //保护未投入 
	{
		if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord)))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay_3(Relay);   //保护未投入且加速未投或加速不允许，清标志，返回 
			return;
		}		
	}
	if (*(Relay->pRelHSuprCFG) == CFG_STATUS_SET) //谐波抑制投入 
	{
		//考虑电流为0的情况 
		if (Relay->pRelElec1->Mod != 0)
			ElecSet = (*Relay->dwK35)*ElecSet/(*Relay->pH2Coef);
	}

	if (ReadRelayWord(Relay->AccActRelayWord) || ReadRelayWord(Relay->ActRelayWord)) //加速或者过量保护已经动作过 
	{	
		//过量保护的返回分两种情况
		// 1. 过流保护，跳闸成功后电流消失，保护返回
		// 2. 过压保护，跳闸成功后有可能仍然过压，此时需用其他条件返回
		if (((Relay->pRelElec1->Mod) < (ElecSet*NOCURCOEF/100))||(!ReadRelayWord(Relay->RSTRelayWord)))
		{		
			Relay->byRelRCounter++;
			if(Relay->byRelRCounter >3)
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay_3(Relay);
			}
			return;
		}
		else
		{
			Relay->byRelRCounter = 0;
			return;	
		}
	}
	
	if (Relay->pRelElec1->Mod < ElecSet) //当前主电量小于定值 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //保护入段了 
		{
			if ((Relay->pRelElec1->Mod < ElecSet*99/100)&&(Relay->pRelElec2->Mod < *Relay->pElec2Set)) //保护已经入段且电流小于返回定值 
			{
				Relay->byRelRCounter++;
				if(Relay->byRelRCounter>3)
				{
					WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
					ResetOverValueRelay_3(Relay);
					return;
				}
			}
			else
				Relay->byRelRCounter = 0;
		}
		else
		{
			if(Relay->pRelElec1->Mod >= *(Relay->pElec1Set))      //谐波抑制投入的情况下，当电流大于初始定值时保护不能返回
				return;
			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay_3(Relay);
			return;		
		}
	}
	
	if (ReadRelayWord(Relay->LockRelayWord))
	{
		//入段闭锁继电器字置位
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay_3(Relay); 
		return;
	}	
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (*(Relay->pRelHLockCFG) == CFG_STATUS_SET)
	{
		if (ReadRelayWord(Relay->H2LockRelayWord))      //二次谐波闭锁投入且保护被闭锁 
		{
			ResetOverValueRelay_3(Relay);
			return;
		}
	}
	if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord))) //加速未投入或者加速不允许，进行功率方向判断 
	{
		if (*(Relay->pRelPDCFG) == CFG_STATUS_SET)   //过量保护功率方向投入 
		{
			if(*(Relay->pRelFDCFG) == CFG_STATUS_SET)   //过量保护正方向投入 
			{
				if(ReadRelayWord(Relay->PRewardRelayWord)&&(!ReadRelayWord(Relay->PForwardRelayWord)))  //反方向继电器字置位，正方向继电器字没有置位 
				{
					ResetOverValueRelay_3(Relay);
					return;
				}
			}
			else
			{
				if(ReadRelayWord(Relay->PForwardRelayWord)&&(!ReadRelayWord(Relay->PRewardRelayWord)))   //正方向继电器字置位，反方向继电器字没有置位 
				{
					ResetOverValueRelay_3(Relay);
					return;
				}				
			}
		}
	}
	
	if (*(Relay->pRelVolLockCFG) == CFG_STATUS_SET)
	{
		if ((Relay->pRelVolt->Mod) > (*(Relay->pVolLockSet)))     //低压闭锁投入且电压大于定值 
		{
			ResetOverValueRelay_3(Relay);
			return;
		}
	}
	if (*(Relay->pRel47UCFG) == CFG_STATUS_SET)   //复压闭锁功能
	{
		if(ReadRelayWord(Relay->R47ULKRelayWord))
		{
			ResetOverValueRelay_3(Relay);
			return;
		}
	}
	if (!ReadRelayWord(Relay->StartRelayWord))   //保护没有入段 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>=2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			if((*(Relay->pRelAccCFG) == CFG_STATUS_SET)&&(ReadRelayWord(Relay->AccRelayWord))) //yanzh 2012-5-7 15:21:11
			{
				Relay->wPickUpType = ACCPICK;
				MakeActRpt(Relay->wAccPickUpRptNum,*Relay->RptSerialNum);
			}
			else
			{
				Relay->wPickUpType = NORMALPICK;				
				if(Relay->bMakeActRpt == TRUE)							//yanxs  2013-03-18   保护启动报告
					MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
			}
			return;
		}
		else
			return;
	}  
	
	if ((*(Relay->pRelAccCFG) == CFG_STATUS_SET) && (Relay->wPickUpType == ACCPICK))
	{															   //加速投入且加速允许了，加速动作 	
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pAccActTimeSet)))
		{
			WriteRelayWord(Relay->AccActRelayWord,RELAY_ON);      //延时已到，将加速动作继电器字置位 
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //延时已到，同时将动作继电器字置位 
			MakeActRpt(Relay->wAccReportNum,*Relay->RptSerialNum);
			Relay->byRelRCounter = 0;
			return;
		}		
	}
	if(*(Relay->pRelCFG) == CFG_STATUS_SET)
	{
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pTimeSet)))
		{
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //延时已到，将继电器字置位
			Relay->byRelRCounter = 0;
			if(Relay->bMakeActRpt == TRUE)
			{
				MakeActRpt(Relay->wReportNum,*Relay->RptSerialNum);
			}
			else
				MakeAlarmRpt(Relay->wReportNum);
		}
	}

}
/************************************************************
	功能：复位通用过量保护
	参数：Relay		--  通用过量保护参数结构
	返回：无
************************************************************/
void ResetOverValueRelay_3(TRelGeneOvValPar_2 *Relay ,BOOL resetFlag)
{	
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //保护入段 
	{
		if(Relay->wPickUpType == ACCPICK)
			MakeActRpt(Relay->wAccResetRptNum, *Relay->RptSerialNum);	
		else
		{
		if(Relay->bMakeActRpt == TRUE)
			MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);
		}
	}
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);    
	WriteRelayWord(Relay->AccActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ActRelayWord,RELAY_OFF);
	Relay->dwRelTimer = 0;
	Relay->byRelFCounter =0;
	Relay->byRelRCounter = 0;	
}

/************************************************************
	功能：功率方向判断
	参数：Uelec		--  电压
		  Ielec		--  电流	
		  PFRW		--  功率正方向继电器字
		  PRRW		--  功率反方向继电器字
	返回：FALSE		--  正常功率方向判断
		  TRUE		--  带记忆功率方向判断
************************************************************/
BOOL  PowerDirCheck(TRelElecValPar *Uelec,TRelElecValPar *Ielec, LONG *Databuf,LONG LineAngle,LONG UPCOEF,WORD PFRW,WORD PRRW,WORD channel,BOOL traceflag)
{
  	
	LONG temp1,temp2,temp3,temp4;
	LONG Angle;
	DWORD dwmask;
	TRelElecValPar tempU;
	BOOL	result;
	temp3=Uelec->Mod*Ielec->Mod/32;
	if(temp3 < 100)
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_OFF);
		return FALSE;
		
	}
	//带记忆功率方向判断
	if(traceflag)
	{
		if(Uelec->Mod<15*UPCOEF/100)
		{
			dwmask =0x01<<channel; 
			ReadSampleData(Databuf, dwmask, 4);
			CaluBaseFourier_S15(Databuf,&tempU);
			CaluModValue(&tempU);
			result = TRUE;
			if(tempU.Mod<15*UPCOEF/100)
			{			
				WriteRelayWord(PFRW, RELAY_OFF);
				WriteRelayWord(PRRW, RELAY_OFF);
				return result;
			}
		}		
		else
		{
			tempU = *Uelec;
			result =FALSE;
		}
	}
	else
	{
		tempU = *Uelec;
		result =FALSE;
	}
	
	temp1=((tempU.Real*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)+(tempU.Imag*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*CosAngleTab[LineAngle];
	temp2=((tempU.Imag*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)-(tempU.Real*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*SinAngleTab[LineAngle];
	temp4 = temp1+temp2;
	if((temp1 + temp2) > 0)   
	{
		WriteRelayWord(PFRW, RELAY_ON);
		WriteRelayWord(PRRW, RELAY_OFF);		
	}	
	else 
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_ON);
	}
	return result;
}

/************************************************************
	功能：90°接线的功率方向判断
	参数：Uelec		--  电压
		  Ielec		--  电流	
		  PFRW		--  功率正方向继电器字
		  PRRW		--  功率反方向继电器字
	返回：FALSE		--  正常功率方向判断
		  TRUE		--  带记忆功率方向判断
************************************************************/
BOOL  PowerDirCheck_90(TRelElecValPar *Uelec,TRelElecValPar *Ielec, LONG *Databuf,LONG LineAngle,LONG UPCOEF,WORD PFRW,WORD PRRW,WORD channel,BOOL traceflag)
{
  	
	LONG temp1,temp2,temp3,temp4;
	LONG Angle;
	DWORD dwmask;
	TRelElecValPar tempU;
	BOOL	result;
	temp3=Uelec->Mod*Ielec->Mod/32;
	if(temp3 < 100)
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_OFF);
		return FALSE;
		
	}
	//带记忆功率方向判断
	if(traceflag)
	{
		if(Uelec->Mod<15*UPCOEF/100)
		{
			dwmask =0x01<<channel; 
			ReadSampleData(Databuf, dwmask, 4);
			CaluBaseFourier_S15(Databuf,&tempU);
			CaluModValue(&tempU);
			result = TRUE;
			if(tempU.Mod<15*UPCOEF/100)
			{			
				WriteRelayWord(PFRW, RELAY_OFF);
				WriteRelayWord(PRRW, RELAY_OFF);
				return result;
			}
		}		
		else
		{
			tempU = *Uelec;
			result =FALSE;
		}
	}
	else
	{
		tempU = *Uelec;
		result =FALSE;
	}
	temp1=((tempU.Real*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)+(tempU.Imag*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*CosAngleTab[LineAngle];
	temp2=((tempU.Imag*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)-(tempU.Real*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*SinAngleTab[LineAngle];
	temp4 = temp1+temp2;
	if((temp1 - temp2) > 0)   
	{
		WriteRelayWord(PFRW, RELAY_ON);
		WriteRelayWord(PRRW, RELAY_OFF);		
	}	
	else 
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_ON);		
	}
	return result;
}

/************************************************************
	功能：通用欠量保护
	参数：Relay		--  通用欠量保护参数结构
		  RptNum		--  报告序列号，唯一，数据库组织报告用，引用参数，在函数中
		  			     会修改改值，与通用过量保护等不同
	返回：无
************************************************************/
void GeneralUnderValRelay(TRelGeneUnValPar *Relay)
{
	if ((*(Relay->pRelCFG) == CFG_STATUS_CLR) || (*(Relay->pRelSW) == SW_STATUS_CUT))
	{
		ResetUnderValRelay(Relay);  //保护未投入 
		return;		
	}

	if (ReadRelayWord(Relay->ActRelayWord)||ReadRelayWord(Relay->AlarmRelayWord)) //保护已经动作过了 
	{
		if(ReadRelayWord(Relay->wR52BFRealyword))    //断路器跳闸失败
		{
			if(!Relay->bHaveMake52BFRptFlag)
			{
				MakeActRpt(Relay->wTRFailRptNum,Relay->R27RptSerialNum);   //作断路器跳闸失败报告
//				DB_RelayEnd(Relay->R27RptSerialNum);
				Relay->bHaveMake52BFRptFlag = TRUE;				
				ResetUnderValRelay(Relay);
			}
			return;	

		}
		if ((Relay->pRelElec->Mod > (*(Relay->pELeSet)*RECOVERVOLCOEF/100)) || \
			(!ReadRelayWord(Relay->RSTRelayWord)))//电量返回或者闭锁解除 
		{
//			DB_RelayEnd(Relay->R27RptSerialNum);		
			ResetUnderValRelay(Relay);
			return;	
		}
		else    //保护动作了，但是没有返回 
			return;
	}

	if(ReadRelayWord(Relay->LockRelayWord))  //入段闭锁继电器有效，且被置位 
	{
		ResetUnderValRelay(Relay);
		return;	
	}
	
	if (Relay->pRelElec->Mod > (*Relay->pELeSet))   //当前电量大于定值 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //保护入段 
		{
			if (Relay->pRelElec->Mod > (*Relay->pELeSet*101/100))  //大于定值的1.01倍 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetUnderValRelay(Relay); 				
//				DB_RelayEnd(Relay->R27RptSerialNum);
				return;
			}
		}
		else
		{         
			//电量大于定值且保护没有入段，则清标志 
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetUnderValRelay(Relay);   //电量大于定值的1.01倍 
			return;
		}
	}
	if(*Relay->pRelCurLockCFG == CFG_STATUS_SET)
	{
		if(Relay->pRelLockElec->Mod > *Relay->pLockCurSet)
		{
			ResetUnderValRelay(Relay);  
			return;
		}
	}
		
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (!ReadRelayWord(Relay->StartRelayWord))  //保护尚未入段 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>2)   //故障计数器大于2保护入段 
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			ReadAbsTime(&Relay->R27UPickUpTime);
			DB_RelayStart(++(*Relay->RptSerialNum),ReportInf[Relay->wPickUpRptNum].wInf,&Relay->R27UPickUpTime,FALSE); //向数据库中写入启动报告			
			Relay->R27RptSerialNum = *Relay->RptSerialNum;
//			MakeActRpt(Relay->wPickUpReportNum,Relay->R27RptSerialNum);
			return;
		}
		else
			return;
	}
	if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*Relay->pTimeSet) )  //时间满足了 
	{
		if (*(Relay->pRelAlarmCFG) == CFG_STATUS_SET)      //欠量保护投入告警
		{			
			WriteRelayWord(Relay->AlarmRelayWord,RELAY_ON);	 //告警 
			MakeAlarmRpt(Relay->wAlarmRptNum);
		}
		else
		{			
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);  //跳闸 			
			MakeActRpt(Relay->wActRptNum,Relay->R27RptSerialNum);
		}
	}
}
/************************************************************
	功能：复位通用欠量保护
	参数：Relay		--  通用欠量保护参数结构
	返回：无，置相应的继电器字
************************************************************/
void ResetUnderValRelay(TRelGeneUnValPar *Relay,BOOL resetFlag )
{
	Relay->byRelFCounter = 0;
	Relay->byRelRCounter = 0;
	Relay->dwRelTimer    = 0;
	Relay->bHaveMake52BFRptFlag = FALSE;
	if(ReadRelayWord(Relay->StartRelayWord))
	{	
//		MakeActRpt(Relay->wResetReportNum, Relay->R27RptSerialNum);	
		DB_RelayEnd(Relay->R27RptSerialNum);
	}
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->AlarmRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
}
/************************************************************
	功能：平行四边形判断
	参数：Rel21		--  通用距离保护参数结构
	返回：TRUE		--  当前阻抗点位于四边形内
		  FALSE		--  当前阻抗点位于四边形外
************************************************************/
BOOL Parallelogram(TRel21Par *Relay)
{
	LONG RSet = *(Relay->pRel21RSet);
	LONG XSet = *(Relay->pRel21XSet);
	LONG RCurrent = Relay->pRel21Impd->Real;
	LONG XCurrent = Relay->pRel21Impd->Imag;
	//LONG K35;
	DWORD tempimpd;
	LONG Angle;

	if ((Relay->pRel21Impd->Imag == (INVALIDRX*(*Relay->pnZCoef))) || (Relay->pRel21Impd->Real == (INVALIDRX*(*Relay->pnZCoef))))
	{
		return FALSE;
	}
	if (*(Relay->pRel21DirCFG) == CFG_STATUS_CLR)   //当前投入的是反方向 
	{
		RCurrent = 0 - RCurrent;     
		XCurrent = 0 - XCurrent;
	}
	if (*(Relay->pRel21HSuprCFG) == CFG_STATUS_SET)  //谐波抑制投入 
	{
		RSet = RSet*(*Relay->pH2Coef)/(*Relay->dwK35);
		XSet = XSet*(*Relay->pH2Coef)/(*Relay->dwK35);
	}
	//平行四边形上边 
	if (XCurrent> XSet)  
	{
		return FALSE;
	}
	//平行四边形下边 
	if ((XCurrent + (*(Relay->pRel21ZOffset))) < 0)  
	{
		return FALSE;
	}
	 //平行四边形左边 
	 //由液晶任务保证  *Relay->pRel21LineAngleSet-10 非负
	if (XCurrent*TANGMULTIPLE > (RCurrent + (*(Relay->pRel21ZOffset)))*(Relay->pTangTabAddr[*Relay->pRel21LineAngleSet-10]))
	{  
		return FALSE;
	}
	//平行四边形右边 
	if (XCurrent*TANGMULTIPLE < (RCurrent- RSet )*(Relay->pTangTabAddr[*Relay->pRel21LineAngleSet-10]))
	{	
		return FALSE;
	}
	
	return TRUE;

}
/************************************************************
	功能：阻抗四边形判断
	参数：Rel21		--  通用距离保护参数结构
	返回：TRUE		--  当前阻抗点位于四边形内
		  FALSE		--  当前阻抗点位于四边形外
************************************************************/
BOOL ImpdQuadrangle(TRel21Par *Relay)
{
	LONG RSet = *(Relay->pRel21RSet);
	LONG XSet = *(Relay->pRel21XSet);
	LONG RCurrent = Relay->pRel21Impd->Real;
	LONG XCurrent = Relay->pRel21Impd->Imag;
	DWORD tempimpd;
	LONG	Angle;
	if ((Relay->pRel21Impd->Imag == (INVALIDRX*(*Relay->pnZCoef))) || (Relay->pRel21Impd->Real == (INVALIDRX*(*Relay->pnZCoef))))
	{
		return FALSE;
	}
	if (*(Relay->pRel21DirCFG) == CFG_STATUS_CLR)   //当前投入的是反方向 
	{
		RCurrent = 0 - RCurrent;
		XCurrent = 0 - XCurrent;
	}
	if (*(Relay->pRel21HSuprCFG) == CFG_STATUS_SET)  //谐波抑制投入 
	{
		RSet = RSet*(*Relay->pH2Coef)/(*Relay->dwK35);
		XSet = XSet*(*Relay->pH2Coef)/(*Relay->dwK35);
	}
	
	//四边形判断部分： 
	//阻抗四边形在第一第四象限中 
	//阻抗四边形上边界 
	if(XCurrent > XSet)  
	{
		return FALSE;
	}
	if(RCurrent < 0)
	{
		return FALSE;
	}
	//四边形15°下边界 
	if (XCurrent*TANGMULTIPLE < (Relay->pTangTabAddr[*(Relay->pRel21AOffset)-10])*RCurrent*(-1))
	{
		return FALSE;
	}
	//四边形垂直R轴部分边界 
	if (XCurrent < 0)
	{
		if (RCurrent > RSet)
		{
			return FALSE;
		}
	}
	else
	{
		//四边形85°边界 
		if (XCurrent*TANGMULTIPLE >TANG85*RCurrent)
		{
			return FALSE;
		}
		
		//四边形线路阻抗角边界 
		if (XCurrent*TANGMULTIPLE < ((RCurrent - RSet)*(Relay->pTangTabAddr[*(Relay->pRel21LineAngleSet)-10])))
		{
			return FALSE;
		}	
	}
	return TRUE;
}

/************************************************************
	功能：通用距离保护
	参数：Rel21		--  通用距离保护参数结构
	返回：无
************************************************************/
void GeneralRelay21(TRel21Par *Relay)
{
	if(*(Relay->pRel21SW) == SW_STATUS_CUT)
	{		
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetRelay21(Relay);		   //距离保护未投入、后加速未投或者后加速不允许，退出
		return;
	}
	if (*(Relay->pRel21CFG) == CFG_STATUS_CLR)
	{
		if ((*(Relay->pRel21ACCCFG) == CFG_STATUS_CLR) || (!ReadRelayWord(Relay->ACCRelayWord)) )
		{
			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);           //距离保护未投入、后加速未投或者后加速不允许，退出
			return;
		}		
	}
	if(ReadRelayWord(Relay->ACTRelayWord)||ReadRelayWord(Relay->ACCActRelayWord))
	{
		if(ReadRelayWord(Relay->R37IRelayWord))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);	//阻抗点位与四边形之外
			return;
		}
		else
		{
			return;
		}			
	}

	if (*(Relay->pRel21QuadTypeCFG) == CFG_STATUS_SET)
	{
		//调用平行四边形判断
		if(!Parallelogram(Relay))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);	//阻抗点位与四边形之外
			return;
		}
	}
	else
	{
		//调用阻抗四边形判断 
		if(!ImpdQuadrangle(Relay))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);//阻抗点位与四边形之外 
			return;
		}
	}
	
	if(ReadRelayWord(Relay->LockRelayWord))
	{		
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetRelay21(Relay);//入段闭锁继电器字被置位，闭锁 
		return;
	}
	
	if(*Relay->pRel21HLockCFG == CFG_STATUS_SET)	//二次谐波闭锁投入?
	{
		if(ReadRelayWord(Relay->H2LockRelayWord))  //二次谐波闭锁继电器字置位
		{			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);            
			return;
		}
	}
	//如果距离或者加速已经动作过了，尚未返回 
	if (ReadRelayWord(Relay->ACCActRelayWord) || ReadRelayWord(Relay->ACTRelayWord))
	{
		return;
	}
	
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);//置故障循环控制继电器字 
	if (!ReadRelayWord(Relay->StartRelayWord))   //保护没有入段 
	{
		Relay->byRel21FCounter++;
		if (Relay->byRel21FCounter>=2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRel21Timer = *(Relay->pSysTimer);
			if((*(Relay->pRel21ACCCFG) == CFG_STATUS_SET) && (ReadRelayWord(Relay->ACCRelayWord))) //yanzh 2012-5-7 15:21:50
			{					
				MakeActRpt(Relay->wAccPickUpRptNum,*Relay->RptSerialNum);
				Relay->wPickUpType = ACCPICK;		
			}
			else
			{				
				MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
				Relay->wPickUpType = NORMALPICK;
			}
		}
		else
		{
			return;
		}
	}
	if ((*(Relay->pRel21ACCCFG) == CFG_STATUS_SET) && (Relay->wPickUpType == ACCPICK))
	{
	    //后加速投入且允许，后加速动作 
		if ((*(Relay->pSysTimer) - Relay->dwRel21Timer+INHERENTTIME) > (*(Relay->pRel21AccTSet)))
		{
			WriteRelayWord(Relay->ACCActRelayWord,RELAY_ON);  //延时已满足置加速动作继电器字 
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);      //同时将动作继电器字置位 
			MakeActRpt(Relay->wRel21AccRptNum,*Relay->RptSerialNum);
			return;
		}
	}
	if(*(Relay->pRel21CFG) == CFG_STATUS_SET)
	{
		if ((*(Relay->pSysTimer) - Relay->dwRel21Timer+INHERENTTIME) > (*(Relay->pRel21TSet)))
		{		
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
			MakeActRpt(Relay->wRel21RptNum,*Relay->RptSerialNum);
		}
	}
}
/************************************************************
	功能：复位通用距离保护元件
	参数：Rel21		--  通用距离保护参数结构
	返回：无
************************************************************/
void ResetRelay21(TRel21Par *Relay,BOOL resetFlag)
{
	
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //保护入段 
	{
		if(Relay->wPickUpType == ACCPICK)
		{
			MakeActRpt(Relay->wAccResetRptNum, *Relay->RptSerialNum);	
		}
		else
		{
			MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
		}
	}
	WriteRelayWord(Relay->ACCActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	Relay->dwRel21Timer = 0;
	Relay->byRel21FCounter = 0;
	Relay->byRel21RCounter = 0;

}
/************************************************************
	功能：PT断线检查
	参数：Relay		--  PT断线
	返回：无
************************************************************/
void CheckPTBrok_1U(TRelPTBrokPar *Relay)
{
	if ((*Relay->pRelPTBrokCFG) == CFG_STATUS_CLR)  //PT断线未投入 
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		if(ReadRelayWord(Relay->ActRelayWord))
		{
			MakeAlarmRpt(Relay->wRelResRptNum);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		}
		return;
	}	
	if(Relay->pRelU1->Mod > *Relay->pR74PTU1Set)   //PT断线恢复
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		if(ReadRelayWord(Relay->ActRelayWord))
		{
			MakeAlarmRpt(Relay->wRelResRptNum);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		}
		return;
	}
	if(Relay->pRelCur->Mod < *Relay->pR74PTISet)   //无流，直接退出
	{
		return;
	}
	
	if(Relay->pRelU1->Mod < *Relay->pR74PTU1Set)  //U小于定值
	{
		if(!ReadRelayWord(Relay->ActRelayWord))		//尚未动作
		{
			if(!ReadRelayWord(Relay->StartRelayWord))		//尚未入段
			{
				Relay->dwRelTimer = *Relay->dwSysTimer;
				WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
			}
			else
			{
				if((*Relay->dwSysTimer - Relay->dwRelTimer) > *Relay->pR74PTTSet)
				{
					WriteRelayWord(Relay->ActRelayWord, RELAY_ON);
					MakeAlarmRpt(Relay->wRelRptNum);
				}
			}
		}		
	}
}
/************************************************************
	功能：PT断线检查
	参数：Relay		--  PT断线
	返回：无
************************************************************/
void CheckPTBrok_2U(TRelPTBrokPar *Relay)
{
	if ((*Relay->pRelPTBrokCFG) == CFG_STATUS_CLR)  //PT断线未投入 
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		if(ReadRelayWord(Relay->ActRelayWord))
		{
			MakeAlarmRpt(Relay->wRelResRptNum);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		}
		return;
	}
	if(ReadRelayWord(Relay->ActRelayWord)) //如果已经发生PT断线，只有在电压恢复的情况下才能复归PT断线
	{
		if(Relay->pRelU1->Mod > *Relay->pR74PTU1Set)   //PT断线恢复
		{
			Relay->dwRelTimer = 0;
			WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);			
			MakeAlarmRpt(Relay->wRelResRptNum);
			return;
		}
		else
			return;
	}
	if(ReadRelayWord(Relay->LKPIRelayWord)) //入段闭锁继电器字被置位		
	{		
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		return;
	}
	if(Relay->pRelU1->Mod > *Relay->pR74PTU1Set)    //主判断电压大于定值
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		return;
	}
	if((Relay->pRelCur->Mod > *Relay->pR74PTISet)||(Relay->pRelU2->Mod > *Relay->pR74PTU2Set))
	{
		if(!ReadRelayWord(Relay->StartRelayWord))		//尚未入段
		{
			Relay->dwRelTimer = *Relay->dwSysTimer;
			WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
		}
		else
		{
			if((*Relay->dwSysTimer - Relay->dwRelTimer) > *Relay->pR74PTTSet)
			{
				WriteRelayWord(Relay->ActRelayWord, RELAY_ON);
				MakeAlarmRpt(Relay->wRelRptNum);
			}
		}
	}
}

/************************************************************
	功能：实现反时限过流保护
	参数：Relay		--  反时限过流保护参数结构
	返回：无
************************************************************/
void InverseCurRelay(TRelInverseCurPar *Relay)
{
	if ((*Relay->pRelCFG == CFG_STATUS_CLR) || (*Relay->pRelSW == SW_STATUS_CUT)) //保护未投入 
	{
		ResetInverseCurRel(Relay);
		return;
	}
	if (ReadRelayWord(Relay->ACTRelayWord))   //保护已经动作过 
	{
		if(Relay->pRelElec->Mod<*Relay->pRelCurSet*NOCURCOEF/100)
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_OFF);
			ResetInverseCurRel(Relay);
			return;
		}
		else 
			return;
	}
	if (ReadRelayWord(Relay->StartRelayWord)) //保护已经入段了 
	{
		if ((Relay->pRelElec->Mod) < Relay->dwCaluCurSet)   						//反时限已经入段，当前电流小于定值 
		{
			Relay->byRelRCounter += RELAY_SAMP_INTERVAL; 											//故障返回计数器累加 
			if (Relay->byRelRCounter >= POINT_PER_PERIOD)	//一周波内始终小于定值，保护元件复位 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_OFF);
				ResetInverseCurRel(Relay);
				return;
			}
			else
				return;
		}		
	}
	else
	{
		if (Relay->pRelElec->Mod < Relay->dwCaluCurSet)	//没有入段，电流小于1.1倍定值，直接返回 
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_OFF);
			ResetInverseCurRel(Relay);
			return;
		}
		else		//电流大于1.1倍定值，并且保护没有入段
		{
			/*if (*Relay->RptPickupFlag ==FALSE)
			{
				*Relay->RptPickupFlag =TRUE;
				ReadAbsTime(&RelayPickUpTime);
				DB_RelayStart(*Relay->RptPickupSerialNum,ReportInf[Relay->wPickUpReportNum].wInf,&RelayPickUpTime,FALSE); //向数据库中写入启动报告
				MakePickupRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);
			}
			else
				MakeActRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);*/
			MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON); //置入段继电器字 
			WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_ON);
		}		
	}	
	Relay->LimitCouter++;							//反时限极限动作时间计数器累加 
	Relay->byRelRCounter = 0;
	
	if (Relay->LimitCouter > Relay->MaxCouter)		
	{
		WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);  //反时限动作 
		MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
	}	

	InverseCurIntegral(Relay);						   //调用反时限累加程序 
	
	if (Relay->CurCounter > Relay->CurCounterSet)
	{
		WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);  //反时限动作
		MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
	}	
}
/************************************************************
	功能：反时限过流保护电流累加量计算
	参数：Relay		--  反时限过流保护参数结构
	返回：无
************************************************************/
void InverseCurIntegral(TRelInverseCurPar *Relay)
{
	BYTE Multiples;
	LONG coefA,coefB,coefC;
	if ((*Relay->pRelTypeSet > 3) || (*Relay->pRelTypeSet < 1) )
	{
		return;
	}
	if (*Relay->pRelTypeSet == 1)
	{
		Multiples	= Relay->pRelElec->Mod/(*(Relay->pRelCurSet));
				
		if (Multiples>=20)
			Multiples = 19;
		
		coefA = Relay->pCoefTabA[Multiples][0];
		coefB = Relay->pCoefTabA[Multiples][1];
		coefC = Relay->pCoefTabA[Multiples][2];		
	}
	else if (*Relay->pRelTypeSet == 2)
	{
		coefA= Relay->pCoefTabB[0];
		coefB = Relay->pCoefTabB[1];
		coefC = Relay->pCoefTabB[2];
	}
	else if (*Relay->pRelTypeSet == 3)
	{
		coefA = Relay->pCoefTabC[0];
		coefB = Relay->pCoefTabC[1];
		coefC = Relay->pCoefTabC[2];
	}
	
	Relay->CurItem = (LLONG)coefA*(*(Relay->pRelCurSet))*(*(Relay->pRelCurSet)) \
		+(LLONG)coefB*(*(Relay->pRelCurSet))*Relay->pRelElec->Mod \
			+(LLONG)coefC*Relay->pRelElec->Mod*Relay->pRelElec->Mod; 
	Relay->CurCounter += Relay->CurItem;
}
/************************************************************
	功能：复位反时限过流保护元件
	参数：Relay		--  反时限过流保护参数结构
	返回：无
************************************************************/
void ResetInverseCurRel(TRelInverseCurPar *Relay,BOOL resetFlag)
{
	//if (ReadRelayWord(Relay->StartRelayWord)&&(*Relay->RptPickupFlag == TRUE))   //保护入段 
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //保护入段 
	{
	//	MakeActRpt(Relay->wResetReportNum, *Relay->RptPickupSerialNum);	
		MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
	}
	Relay->byRelFCounter=0;
	Relay->byRelRCounter=0;
	Relay->LimitCouter =0;
	Relay->CurCounter =0;
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);

}
/************************************************************
	功能：电流增量保护
	参数：Relay		--  电流增量保护参数结构
	返回：无
************************************************************/
void IncreCurrentRelay(TRel50DIPar *Relay)									
{
	LONG	IncreCur;
	if((*Relay->pRelCFG == CFG_STATUS_CLR) ||(*Relay->pRelSW ==SW_STATUS_CUT ))     //保护未投入
	{	
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetR50DI(Relay);
		return;
	}
	if(ReadRelayWord(Relay->ACTRelayWord))				//保护已经动作过
	{
		if(Relay->pRelElec1->Mod < *Relay->pRelLockCurSet)	//当前电流小于小电流闭锁电流
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;			
		}
		else 
			return;
	}

	if(ReadRelayWord(Relay->StartRelayWord))		//保护已经入段了
	{
		if(Relay->pRelElec1->Mod < *Relay->pRelLockCurSet)	//当前电流小于小电流闭锁电流
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;			
		}
		//保护入段了，但是有其他保护动作过
		if(*Relay->OtherRelTrip == TRUE)
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;
		}
		
		if(*Relay->pFaultSINum<SAMP_PER_SECOND) //如果保护已经入段，但是未达到1s的延时，仍要判断增量电流大于定值，若小于则返回 yanzhh 2012年5月25日16:31:17
		{
			IncreCur = (Relay->pRelElec1->Mod - Relay->dwPreI1Mod)*(*Relay->pH2Coef) - (*Relay->pRelKhr)*(Relay->pRelElec3->Mod + Relay->pRelElec5->Mod \
				- Relay->dwPreI3Mod - Relay->dwPreI5Mod);
			if(IncreCur < *Relay->pRelCurSet*(*Relay->pH2Coef))
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetR50DI(Relay);
				return;
			}
		}
		if(*(Relay->pSysTimer) -Relay->dwRelTimer+INHERENTTIME >(*Relay->pRelTimeSet))     //延时已满足
		{
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
			MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
			return;			
		}
		else
			return;
	}
	if(*Relay->pFaultSINum<SAMP_PER_SECOND)   //只在1s内判断启动，若在该时间内，增量电流小于定值，则保护返回
	{
		if(Relay->pRelElec1->Mod < *Relay->pRelLockCurSet)
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;			
		}
		if(*Relay->OtherRelTrip == TRUE)  //有其他保护动作?
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;
		}
		if(ReadRelayWord(Relay->H2LockRelayWord))   //二次谐波闭锁
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;
		}
		//计算增量电流 I=I1h-I1q-Khr*(I3h+I5h-I3q-I5q)
		IncreCur = (Relay->pRelElec1->Mod - Relay->dwPreI1Mod)*(*Relay->pH2Coef) - (*Relay->pRelKhr)*(Relay->pRelElec3->Mod + Relay->pRelElec5->Mod \
			- Relay->dwPreI3Mod - Relay->dwPreI5Mod);
		if(IncreCur > *Relay->pRelCurSet*(*Relay->pH2Coef))
		{	
			Relay->wRelFCounter++;
			if(Relay->wRelFCounter<3)
				return;
			if( ReadRelayWord(Relay->LockRelayWord))
			{
				ResetR50DI(Relay);
				return;
			}
			/*if (*Relay->RptPickupFlag ==FALSE)
			{
				*Relay->RptPickupFlag =TRUE;
				ReadAbsTime(&RelayPickUpTime);
				DB_RelayStart(*Relay->RptPickupSerialNum,ReportInf[Relay->wPickUpReportNum].wInf,&RelayPickUpTime,FALSE); //向数据库中写入启动报告
				MakePickupRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);
			}
			else
				MakeActRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);*/
			MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *Relay->pSysTimer;
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
		}
	}
	else   //故障检测时间超过了500ms，不在进行判断，直接复位保护元件
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetR50DI(Relay);
		return;
	}
}
/************************************************************
	功能：计算故障启动前后的增量电流
	参数：Relay		--  电流增量保护参数结构
	返回：无
************************************************************/

void CalcInCreCurrent(TRel50DIPar *Relay,LONG *DataBuf,DWORD dwMask)	
{	
	if((*Relay->pRelCFG == CFG_STATUS_CLR) ||(*Relay->pRelSW ==SW_STATUS_CUT ))
		return;
	if(Relay->PreCurHaveCalcFlag)
		return;
	if(ReadRelayWord(Relay->R79ActRelayWord)) //重合闸动作继电器字
	{
		Relay->dwPreI1Mod = 0;
		Relay->dwPreI3Mod = 0;
		Relay->dwPreI5Mod = 0;
		Relay->PreCurHaveCalcFlag = TRUE;
	}
	else
	{
		TRelElecValPar I1p,I3p,I5p;
		
		ReadSampleData(DataBuf,dwMask,2);  //读取故障前2周波数据
#ifdef PRO_6571
		CaluBaseFourier_20(DataBuf,&I1p);  //6571装置为20点采样
		CaluThirdFourier_20(DataBuf,&I3p);
		CaluFifthFourier_20(DataBuf,&I5p);
#else
		CaluBaseFourier_S15(DataBuf,&I1p);
		CaluThirdFourier(DataBuf,&I3p);
		CaluFifthFourier(DataBuf,&I5p);
#endif	
		CaluModValue(&I1p);
		CaluModValue(&I3p);
		CaluModValue(&I5p);
	
		Relay->dwPreI1Mod = I1p.Mod;
		Relay->dwPreI3Mod = I3p.Mod;
		Relay->dwPreI5Mod = I5p.Mod;
		Relay->PreCurHaveCalcFlag = TRUE;	
	}
}
/************************************************************
	功能：复位增量保护元件
	参数：Relay		--  电流增量保护参数结构
	返回：无
************************************************************/

void ResetR50DI(TRel50DIPar *Relay,BOOL resetFlag)
{
	//if (ReadRelayWord(Relay->StartRelayWord)&&(*Relay->RptPickupFlag == TRUE))   //保护入段 
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //保护入段 
	{
	//	MakeActRpt(Relay->wResetReportNum, *Relay->RptPickupSerialNum);	
		MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
	}
	Relay->PreCurHaveCalcFlag = FALSE;
	Relay->dwRelTimer =0;
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	
}
/************************************************************
	功能：重合闸充放电元件 
	参数：Relay		--  重合闸参数结构
	返回：无
************************************************************/
void AutoRCCharge(TRel79Par *Relay)
{
	
	if(ReadRelayWord(Relay->R79LOCKRW))   //重合闸被闭锁
		return;
	
	if(ReadRelayWord(Relay->R79RSSRW))                         //重合闸处于复归状态
	{
		if(ReadRelayWord(Relay->R7952A)&&(!ReadRelayWord(Relay->R7952B)))				//断路器处于合位
		{
			WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79RSSRW,RELAY_ON);
			Relay->R79RSFLAG = FALSE;
			Relay->R79ShotCnt = 0;	
			
			if(Relay->R79ShotCnt == 0)		//动作次数为0，是一次重合闸
			{
				WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
				WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
			}
			if(Relay->R79ShotCnt == 1)		//动作次数为1，是二次重合闸
			{		
				WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
				WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
			}		
			return;

		}
		else
		{
			if(Relay->R79RSFLAG)     //充放电延时已经启动
			{
				if((*Relay->dwSysTimer - Relay->R79Timer) > *Relay->R79RSTSet)//放电时间已经满足
				{
					WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
					WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
					WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
					Relay->R79RSFLAG = FALSE;
					return;
				}
				else       //放电时间尚不满足，直接返回
					return;
			}
			else				//放电延时尚未启动
			{
				Relay->R79RSFLAG = TRUE;		//启动放电延时
				Relay->R79Timer = *(Relay->dwSysTimer);
				return;
			}
		}
	}
	if(ReadRelayWord(Relay->R79CYSRW))	//重合闸处于周期状态
	{
		WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79CYSRW, RELAY_ON);
		Relay->R79RSFLAG = FALSE;
		
		if(Relay->R79ShotCnt == 0)		//动作次数为0，是一次重合闸
		{
			WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
			WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
		}
		if(Relay->R79ShotCnt == 1)		//动作次数为1，是二次重合闸
		{		
			WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
			WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
		}		
		return;

	}
	if(!ReadRelayWord(Relay->R7952A))	//断路器处于分位
	{
		WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
		Relay->R79RSFLAG = FALSE;

		if(Relay->R79ShotCnt == 0)		//动作次数为0，是一次重合闸
		{
			WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
			WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
		}
		if(Relay->R79ShotCnt == 1)		//动作次数为1，是二次重合闸
		{		
			WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
			WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
		}		
		return;

	}
	if(!Relay->R79RSFLAG)		//断路器合位，充电延时尚未启动
	{
		Relay->R79RSFLAG=TRUE;             //启动充电延时
		Relay->R79Timer = *(Relay->dwSysTimer);	//初始化充电延时
		return;
	}
	if((*Relay->dwSysTimer - Relay->R79Timer) > *Relay->R79RSTSet) //充电延时已经满足
	{
		WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW, RELAY_ON);
		Relay->R79RSFLAG = FALSE;
		Relay->R79ShotCnt  = 0;
		Relay->R79LKFlag 	 = FALSE;
	}
	if(Relay->R79ShotCnt == 0)		//动作次数为0，是一次重合闸
	{
		WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
		WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
	}
	if(Relay->R79ShotCnt == 1)		//动作次数为1，是二次重合闸
	{		
		WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
		WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
	}		
}
/************************************************************
	功能：重合闸动作元件 
	参数：Relay		--  重合闸参数结构
	返回：无
************************************************************/	
void AutoRCAct(TRel79Par *Relay)
{	
	//LK继电器字只在复归状态下判断，重合闸启动后仅监视继电器字能打断重合闸
	if((ReadRelayWord(Relay->R79RSSRW))&&(ReadRelayWord(Relay->R79LOCKRW)))
	{
		ResetAutoRC(Relay);		
		return;		
	}
	if(ReadRelayWord(Relay->R79TRCHECK)&&!ReadRelayWord(Relay->R79PIRW))
	{
		if(!(*Relay->pR79PIRptFlag))
		{			
			MakeActRpt(Relay->wRpt79NPI_1,*Relay->RptSerialNum);
			*Relay->pR79PIRptFlag = TRUE;
		}
	}

	if(Relay->R79TFLAG == FALSE)  //重合闸未进入延时状态
	{
		if(*Relay->R79SW==SW_STATUS_CUT) //压板未投入，直接复位退出;配置作在闭锁继电器字中。
		{
			ResetAutoRC(Relay);
			return;
		}
	}
			
	AutoRCCharge(Relay);		//调用重合闸充电子程序

	if(ReadRelayWord(Relay->R79LOSRW))	//重合闸在闭锁状态
	{
		WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
		return;
	}
	if(ReadRelayWord(Relay->R79CLCRW))	//重合闸命令撤销继电器字置位
	{
		if(ReadRelayWord(Relay->R79CLRW))
		{
			if(Relay->R79ShotCnt == 1)				
				MakeActRpt(Relay->wRpt79_1_FAIL,*Relay->RptSerialNum);
			else
				MakeActRpt(Relay->wRpt79_2_FAIL,*Relay->RptSerialNum);
		}
		WriteRelayWord(Relay->R79CLRW,RELAY_OFF);//复位重合闸动作继电器字
		WriteRelayWord(Relay->R79TORW, RELAY_OFF);
		Relay->R79TFLAG = FALSE;

		if(Relay->R79ShotCnt >= *Relay->R79NumSet)    //最高次重合闸已经动作过
		{
			ResetAutoRC(Relay);
			return;
		}		
	}
	else    //重合闸命令撤销逻辑继电器字没有置位
	{
//		if(ReadRelayWord(Relay->R79CLRW))//重合闸已经动作过		
		if(Relay->R79CLFlag)//增加重合闸动作标志，该标志在复位函数中和79PI满足时清零。by yanzh
		{
			if((*(Relay->dwSysTimer) - Relay->R79Timer) > (*Relay->R79SOTFTSet+1000))		//合于故障延时已满足
			{
				if(ReadRelayWord(Relay->R7952A)&&!ReadRelayWord(Relay->R7952B))
				{					
					if(Relay->R79ShotCnt == 1)
						MakeActRpt(Relay->wRpt79_1_SUCC,*Relay->RptSerialNum);
					else
						MakeActRpt(Relay->wRpt79_2_SUCC,*Relay->RptSerialNum);
				}
				else
				{
					if(Relay->R79ShotCnt == 1)
						MakeActRpt(Relay->wRpt79_1_FAIL,*Relay->RptSerialNum);
					else
						MakeActRpt(Relay->wRpt79_2_FAIL,*Relay->RptSerialNum);
				}
				ResetAutoRC(Relay);
				return;
			}
			else			//合于故障检测延时未满足，直接返回，有可能继续保护动作，发生二次重合闸
				return;
		}
		if(Relay->R79TFLAG) //重合闸已经进入延时状态
		{
			if(!ReadRelayWord(Relay->R79TORW))	  //重合闸动作延时尚未满足
			{
				if(Relay->R79ShotCnt == 0)
				{
					if((*Relay->dwSysTimer - Relay->R79Timer+AUTORCTIME) < (*Relay->R79T1Set))  //作固有时间修正 766ms
						return;
				}
				if(Relay->R79ShotCnt == 1)
				{
					if((*Relay->dwSysTimer - Relay->R79Timer+AUTORCTIME) < (*Relay->R79T2Set)) //作固有时间修正 766ms
						return;
				}
			}
	
			if(!Relay->R79CLSFLAG) //重合闸动作监视延时标志尚未置位
			{
				WriteRelayWord(Relay->R79TORW, RELAY_ON);   //置延时到继电器字，初始化动作监视继电器字
				Relay->R79Timer = *Relay->dwSysTimer;	
				Relay->R79CLSFLAG = TRUE;
				return;
			}
			if(ReadRelayWord(Relay->R79CLSRW))  //动作监视满足，置重合闸动作继电器字
			{
				WriteRelayWord(Relay->R79CLRW, RELAY_ON);
				Relay->R79CLFlag = TRUE;
				ResetHoldRelayWord(Relay->R79PIRW);
				WriteRelayWord(Relay->R79TORW, RELAY_OFF);
				Relay->R79ShotCnt++;
				Relay->R79TFLAG = FALSE;
				Relay->R79CLSFLAG = FALSE;
				Relay->R79PISFLAG = FALSE;
				Relay->R79Timer = *(Relay->dwSysTimer);	

				if(*Relay->pFaultOnFlag == FALSE)    //重合闸偷跳
				{
					TAbsTime 	PickUpTime;

					ReadAbsTime(&PickUpTime);
					DB_RelayStart(++(*Relay->SysRptSerialNum),RELAY_INF_NOUSED,&PickUpTime,TRUE); 
					*Relay->RptSerialNum = *Relay->SysRptSerialNum;

					if(ReadRelayWord(Relay->R79SH0RW)&&!ReadRelayWord(Relay->R79SH1RW))   //作重合闸动作报告
						MakeActRpt(Relay->wRpt79_1,*Relay->RptSerialNum);
					if(!ReadRelayWord(Relay->R79SH0RW)&&ReadRelayWord(Relay->R79SH1RW))
						MakeActRpt(Relay->wRpt79_2,*Relay->RptSerialNum);
					
					DB_RelayEnd(*Relay->RptSerialNum);
					return;

				}
				else
				{
					if(ReadRelayWord(Relay->R79SH0RW)&&!ReadRelayWord(Relay->R79SH1RW))   //作重合闸动作报告
						MakeActRpt(Relay->wRpt79_1,*Relay->RptSerialNum);
					if(!ReadRelayWord(Relay->R79SH0RW)&&ReadRelayWord(Relay->R79SH1RW))
						MakeActRpt(Relay->wRpt79_2,*Relay->RptSerialNum);
					
					return;
				}
			}
			else
			{
				if(*(Relay->dwSysTimer) - Relay->R79Timer>(*Relay->R79CLSTSet)) //在定值时间内，重合闸动作监视逻辑一直不满足，进入闭锁状态
				{					
					if(Relay->R79ShotCnt == 0)
						MakeActRpt(Relay->wRpt79LK_1,*Relay->RptSerialNum);	
					if(Relay->R79ShotCnt == 1)						
						MakeActRpt(Relay->wRpt79LK_2,*Relay->RptSerialNum); 
					
					ResetAutoRC(Relay);//作重合闸被闭锁报告	
					return;
				}
				else
				{
					WriteRelayWord(Relay->R79TORW, RELAY_ON);   //在动作监视期间，一直写延时到继电器字
					return;
				}			
			}
		}			
	}
		
	if(!Relay->R79PISFLAG)//重合闸启动监视延时尚未启动
	{
		if(ReadRelayWord(Relay->R79PIRW)) //重合闸启动逻辑已经满足
		{			
			Relay->R79PISFLAG = TRUE;
			Relay->R79Timer = *Relay->dwSysTimer;//初始化启动监视延时定时器			
			Relay->R79CLFlag = FALSE;
		}
		else
			return;
	}
	if(!ReadRelayWord(Relay->R79PISRW)) //重合闸启动监视逻辑尚未满足,该继电器字为中间继电器字，由逻辑进行配置
	{
		if((*Relay->dwSysTimer - Relay->R79Timer) >*Relay->R79PISTSet) //启动监视时间已满足，表明重合闸不符合启动条件，进入闭锁状态
		{
			if(Relay->R79ShotCnt == 0)
				MakeActRpt(Relay->wRpt79NPI_1,*Relay->RptSerialNum);
			if(Relay->R79ShotCnt == 1)				
				MakeActRpt(Relay->wRpt79NPI_2,*Relay->RptSerialNum);
			ResetAutoRC(Relay);			
			return;
		}
	}
	else
	{
		if(!Relay->R79TFLAG)    //重合闸启动监视逻辑满足，重合闸尚未开始延时，进入周期状态
		{			
			if(Relay->R79ShotCnt == 0)
			{
				MakeActRpt(Relay->wRpt79PI_1,*Relay->RptSerialNum); //做重合闸启动报告
			}
			if(Relay->R79ShotCnt == 1)
			{
				MakeActRpt(Relay->wRpt79PI_2,*Relay->RptSerialNum); //做重合闸启动报告
			}
			*Relay->pR79PIRptFlag = TRUE;
			Relay->R79TFLAG = TRUE;
			Relay->R79Timer = *Relay->dwSysTimer;
			WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79CYSRW, RELAY_ON);	
		}
	}
}

/************************************************************
	功能：重合闸复位函数
	参数：Relay		--  重合闸参数结构
	返回：无
************************************************************/
void ResetAutoRC(TRel79Par *Relay)
{
	WriteRelayWord(Relay->R79CLRW,RELAY_OFF);
	ResetHoldRelayWord(Relay->R79PIRW);
	WriteRelayWord(Relay->R79TORW,RELAY_OFF);
	Relay->R79ShotCnt = 0;
	Relay->R79PISFLAG = FALSE;
	Relay->R79TFLAG = FALSE;
	Relay->R79CLFlag = FALSE;		
	Relay->R79CLSFLAG = FALSE;
	WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
	WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
	WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
}
/************************************************************
	功能：变压器热过负荷保护初始化计算子程序
	参数：Relay		--  重合闸参数结构
	返回：无
************************************************************/
void InitThermalOverLoad(TThermalOverLoad *Relay)
{
	LLONG kin=*Relay->pRatedInSet;
	
	Relay->dwKIn = kin*(*Relay->p49KSet)*1414*(*Relay->pICoef)/(1000*(*Relay->pIHACoef));
	//Relay->dwKIn = *Relay->pRatedInSet*(*Relay->p49KSet)*1414*(*Relay->pICoef)/(1000*(*Relay->pIHACoef));    //K放大100倍，In放大了Coef倍
																									//此处KIN为真有效值，而定值关联的系数为A相得傅里叶系数并且
																									//要转换本相的系数
	Relay->dw49Warn = *Relay->p49ARSet*10000;				//告警系数定值放大了100倍
	Relay->dw49Warn_099 = Relay->dw49Warn*99/100;

	if(*Relay->p49TSet>100)
	{
		if(*Relay->p49TSet>1000)
		{			
			Relay->l49OT = *Relay->p49TSet;
			Relay->lExpT = 999833;
		}
		else
		{			
			Relay->l49OT = *Relay->p49TSet*2;
			Relay->lExpT = 999667;
		}
	}
	else
	{		
		Relay->l49OT = *Relay->p49TSet*6;
		Relay->lExpT = 999000;
	}


	Relay->dw49AThermaPreVal =0;
}
/************************************************************
	功能：变压器热过负荷保护子程序
	参数：TThermalOverLoad		--  过负荷保护参数结构
	返回：无
************************************************************/
void RelThermalOverLoad(TThermalOverLoad *Relay)
{
	LLONG	tempIk;
	tempIk = Relay->pCurrent*100000/Relay->dwKIn;
	tempIk = tempIk*tempIk;
	Relay->dw49AThermaVal = tempIk+(Relay->dw49AThermaPreVal-tempIk)*Relay->lExpT/1000000;
	Relay->dw49AThermaPreVal = Relay->dw49AThermaVal;
	if(Relay->dw49AThermaVal>Relay->dw49Warn)//热累加量已经大于告警定值
	{
		if(!ReadRelayWord(Relay->R49WarnRW))
		{
			WriteRelayWord(Relay->R49WarnRW,RELAY_ON);			
		}
		
		if(Relay->dw49AThermaVal>1000000)
		{
			WriteRelayWord(Relay->R49ActRW,RELAY_ON);			
		}
	}
	else
	{
		if(Relay->dw49AThermaVal<Relay->dw49Warn_099)
		{
			WriteRelayWord(Relay->R49ActRW,RELAY_OFF);
			WriteRelayWord(Relay->R49WarnRW,RELAY_OFF);
		}
	}	
}

/************************************************************
	功能：逻辑方程处理
	参数：无
	返回：无
************************************************************/
/*
void LogicProcess(void)
{	
	//逻辑方程处理
	LONG nLineSys = 0, nLineUser = 0;

	if(byRelayWordValue[1] != 0x1)
	{
		byRelayWordValue[1] = 0x1;
		LogError("CRelayTask",FILE_LINE,"RelayWord True1 changed");
	}
	UpDateInputRelayWord();
	if((gPLC_SysPLCStatus == PLC_ERR_OK)||(gPLC_DefPLCStatus == PLC_ERR_OK))
		RunLogicParseModule(LOGIC_Sys, &nLineSys);
//	if(gPLC_UserPLCStatus == PLC_ERR_OK)
//		RunLogicParseModule(LOGIC_User, &nLineUser);
	
	UpDateLed();

	UpDateOutPut();
}
*/
/*********************************************************************************************
 *
 * 功能描述     检查指定序号开出是否出于动作状态
 *
 * 参数说明      - no	: 输入 	开出序号
 *				 
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL isBoBusy(BYTE no)
{
	BYTE RegStatus;

	//检查本路开出是否正在出口操作,否则跳过
	if((byRelayWordValue[RW_OUT1+no]&0xf0) == 0)
	{
		return FALSE;
/*		
		//开出1和开出25为遥控启动(闭锁)控制，只有别的开出没有操作时，才能进行此路开出的自检
		if(no%24)
		{
			return FALSE;	//设置允许自检标志
		}
		else	//单独处理遥控启动(闭锁)开出通道
		{
			RegStatus = MCF_GPIO_PODR_TIMER;
			if(no/24)	//第二块板对应YK_EN3、YK_EN4
			{
				RegStatus &= (DTI1_YKEN3|DTI0_YKEN4);
				RegStatus ^= DTI1_YKEN3;
			}
			else
			{
				RegStatus &= (DTO1_YKEN1|DTO0_YKEN2);
				RegStatus ^= DTO1_YKEN1;
			}

			//本路遥控闭锁出于闭锁状态，允许自检
			if(RegStatus == 0)
				return FALSE;
		}
*/		
	}
	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述 	开出击穿自检，连续3次本操作完成一路开出自检，每次都检测为异常在判断为本通道击穿
 *
 * 参数说明 	 无
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/
void BoBreakCheck()
{
	DWORD address;
	BYTE status, mask;
	DWORD BaseAddr[2] = {DOA_CS_BASE+25, DOC_CS_BASE+25};
	DWORD flag = 1;
	static BYTE bBoBreakCheckNo = 0;	
	static BOOL bHaveBreakFlag = FALSE;
	static BYTE bBreakCheckCnt = 0;
	static BYTE bBreakErrCnt = 0;
	WORD wBoNo;
	if(bHaveBreakFlag == TRUE)//已经有开出击穿的情况了，即不再进行检查，除非复位装置
		return;
	
	//根据OutPutMap表查找对应物理开出序号
	wBoNo = OutPutMap[bBoBreakCheckNo];
			
	//开出击穿自检
	if((byRelayWordValue[RW_OUT1+bBoBreakCheckNo]&0x0f) == 0)	
	{
 		mask = (0x01<<(wBoNo%8));
		address = BaseAddr[wBoNo/24]+(wBoNo%24)/8;

		//读取开出状态
		status = inportb(address);
		//如果出于导通状态的话重新读该状态
		if((status&mask) == 0)
			status = inportb(address);
			
		flag <<= (wBoNo%24);
		if((status&mask) == 0) //开出击穿
		{
			bBreakErrCnt++;
			//ReportMsg("bo%d break error cnt=%d.", wBoNo, bBreakErrCnt);
		}
		
		bBreakCheckCnt++;
		if(bBreakCheckCnt<BO_BREAK_CNT)
			return;
		
		//连续BO_BREAK_CNT次检测异常，则判断为本通道击穿
		if(bBreakErrCnt == BO_BREAK_CNT)
		{
			dwBoBreakStatus[wBoNo/24] |= flag;
			ReportMsg("the BO%d break check error.", wBoNo);
		}
		else
			dwBoBreakStatus[wBoNo/24] &= (~flag);			
		
		bBreakCheckCnt = bBreakErrCnt = 0;
		
		//全部状态都正常则开出击穿正常，否则故障
		if((dwBoBreakStatus[0] == 0)&&(dwBoBreakStatus[1] == 0))
			DAE_ClearRunningFlags(SS_INSULATION_ERR);
		else
			DAE_SetRunningFlags(SS_INSULATION_ERR);
					
	}
		
	//转下OutPutMap表的一路开出,按照OUTPUT_RELAY_WORD_NUM的实际大小检查
	bBoBreakCheckNo++;
	if(bBoBreakCheckNo == OUTPUT_RELAY_WORD_NUM)
	{
		if((dwBoBreakStatus[0]!=0)||(dwBoBreakStatus[1]!=0))
		{
			bHaveBreakFlag = TRUE;
			*RM_pnBOBRCH1 = dwBoBreakStatus[0];
			*RM_pnBOBRCH2 = dwBoBreakStatus[1];			
			DB_LogWriteRM(RM_BOBRCH1, *RM_pnBOBRCH1);
			DB_LogWriteRM(RM_BOBRCH2, *RM_pnBOBRCH2);
			MakeAlarmRpt(R_OEBR);			
			DAE_SetRunningFlags(SS_INSULATION_ERR);
		}
		else
		{
			if(bHaveBreakFlag)
			{
				bHaveBreakFlag = FALSE;
			}			
			DAE_ClearRunningFlags(SS_INSULATION_ERR);			
		}
	}
	bBoBreakCheckNo %= OUTPUT_RELAY_WORD_NUM;

}

/*********************************************************************************************
 *
 * 功能描述 	开出失效自检，每次处理一路
 *
 * 参数说明 	 无
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/
void BoInvalidCheck()
{
	DWORD flag = 1;
	BYTE status, mask;
	DWORD IntLev;
	DWORD BaseAddr[2] = {DOA_CS_BASE, DOC_CS_BASE};
	DWORD dwOperAdd, dwCheckAdd;
	BYTE command, PortStatus1, PortStatus2, ByteOffset;
	static BYTE bBoInvalidCheckNo = 0;
	static BOOL bHaveInvalidFlag = FALSE;
//	static BOOL bLedIsON = FALSE;
	WORD wBoNo;

	if(bHaveInvalidFlag == TRUE)//已经检查出有开出失效的存在，即不在作检查
		return;
	//根据OutPutMap下标查找对应开出的物理序号
	wBoNo = OutPutMap[bBoInvalidCheckNo];	

 
	if((byRelayWordValue[RW_OUT1+bBoInvalidCheckNo]&0x0f) == 0)		
	{
		
 		mask = (0x01<<(wBoNo%8));
		ByteOffset = (wBoNo%24)/8;
		
		dwCheckAdd = BaseAddr[wBoNo/24]+25+ByteOffset;  //反校地址
		dwOperAdd = BaseAddr[wBoNo/24]+wBoNo%24;		//开出地址
		
		//中断保护
		IntLev = OS_IntLock();
		//开出启动
		if(wBoNo%24 == 0)
		{
			if(wBoNo == 0)
			{
				MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
				MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=1
			}
			else
			{
				MCF_GPIO_PODR_TIMER &= (~DTI1_YKEN3);	//YK_EN3=0
				MCF_GPIO_PODR_TIMER |= DTI0_YKEN4;		//YK_EN4=1
			}
		}
		outportb(dwOperAdd,0x55);
		outportb(0xb0030050, 0xff);

//		if(status&mask)
			status = inportb(dwCheckAdd);
		//开出撤销
		if(wBoNo%24 == 0)
		{
			if(wBoNo == 0)
			{
				MCF_GPIO_PODR_TIMER |= DTO1_YKEN1;			//YK_EN1=0
				MCF_GPIO_PODR_TIMER &= (~DTO0_YKEN2);		//YK_EN2=1
			}
			else
			{
				MCF_GPIO_PODR_TIMER |= DTI1_YKEN3;			//YK_EN3=0
				MCF_GPIO_PODR_TIMER &= (~DTI0_YKEN4);		//YK_EN4=1
			}
		}
		outportb(dwOperAdd,0xAA);
		OS_IntUnLock(IntLev);
		
		flag <<= (wBoNo%24);
		if(status&mask) //开出失效(没有导通)
		{
			dwBoInvalidStatus[wBoNo/24] |= flag;
			ReportMsg("the BO%d invalid check error.", wBoNo);
		}
		else	//开出正常(导通)
		{
			dwBoInvalidStatus[wBoNo/24] &= (~flag);
		}
	}
	
	//转下一路开出，根据OUTPUT_RELAY_WORD_NUM的实际定义检查
	bBoInvalidCheckNo++;
	if(bBoInvalidCheckNo == OUTPUT_RELAY_WORD_NUM)
	{
		if((dwBoInvalidStatus[0]!=0)||(dwBoInvalidStatus[1]!=0))  //检查完一遍，有开出失效情况，作告警报告
		{
			bHaveInvalidFlag = TRUE;
			*RM_pnBOINCH1 = dwBoInvalidStatus[0];
			*RM_pnBOINCH2 = dwBoInvalidStatus[1];			
			DB_LogWriteRM(RM_BOINCH1, *RM_pnBOINCH1);
			DB_LogWriteRM(RM_BOINCH2, *RM_pnBOINCH2);
			DAE_SetRunningFlags(SS_INSULATION_INV);
			MakeAlarmRpt(R_OEERR);			
		}
		else
		{
			if(bHaveInvalidFlag)
			{
				bHaveInvalidFlag = FALSE;				
				DAE_ClearRunningFlags(SS_INSULATION_INV);
			}
			*RM_pnBOINCH1 = dwBoInvalidStatus[0];
			*RM_pnBOINCH2 = dwBoInvalidStatus[1];
		}
	}
	bBoInvalidCheckNo %= OUTPUT_RELAY_WORD_NUM;
}

/*********************************************************************************************
 *
 * 功能描述 	结构体累加校验和计算
 *
 * 参数说明 	 无
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/

void StructACCCheck(TStructCheck *structcheck,WORD StructNum)
{
	WORD acc = 0x55aa;
	static WORD checknum = 0;
	for(LONG i=0;i<structcheck[checknum].bigsize;i++)
	{
		acc += structcheck[checknum].pstruct[i];
	}
	if(acc!=structcheck[checknum].acc)
	{
		if(!InitAllRelayMem())
		{
			RelayCheckErr = TRUE;
			MakeAlarmRpt(R_RCERR);
		}
		acc = 0x55aa;
		for(LONG i=0;i<structcheck[checknum].bigsize;i++)
		{
			acc += structcheck[checknum].pstruct[i];
		}
		structcheck[checknum].acc = acc;
	}
	if(StructNum == 0)
	{
		ReportMsg("The relay check vlaue error.StructNum=%d",StructNum);
		return;
	}
	checknum++;	
	checknum %= StructNum;
	
}
/*********************************************************************************************
 *
 * 功能描述 	保护定值、压板、配置、辅助定值、系数、变比自检
 *
 * 参数说明 	 无
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/

void RelayCheck()
{
	WORD start = SW_BUF_ADDR;
	WORD end   = CF_BUF_ADDR+CF_BUF_SIZE;
	WORD acc   = 0x55aa;
	WORD *p    = (WORD*)SW_BUF_ADDR;
	for(LONG i=0;i<(end-start)/2;i++)
	{
		acc += *p;
		p++;
	}
	if(acc!=RelayCheckAcc)
	{
		
		ReportMsg("The relay check vlaue error.now acc=%d,RelayCheckAcc = %d",acc,RelayCheckAcc);
		if(!InitAllRelayMem())
		{
			RelayCheckErr = TRUE;
			MakeAlarmRpt(R_RCERR);
			ReportMsg("The relay check value error,and InitAllMem Err!!!!.");
		}
		else
			RelayCheckAcc = acc;
	}
}
/*********************************************************************************************
 *
 * 功能描述 	比率差动保护元件
 *
 * 参数说明 	 比率差动保护参数结构
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/
void Rel87RI(TRel87R *Relay)
{
	LONG R87RISet; //差动电流动作定值
	if((*Relay->p87RCFG !=CFG_STATUS_SET )||(*Relay->p87RSW != SW_STATUS_SET))  //保护未投入
	{
		Reset87RI(Relay);
		return;
	}
	if(ReadRelayWord(Relay->ACTRelayWord))		//保护已经动作
	{
		if(Relay->pId->Mod < *Relay->p87RISet*NOCURCOEF/100)	//当前电流小于动作门槛值
		{
			Reset87RI(Relay);
			return;
		}
		else
			return;
	}
	if(Relay->pId->Mod > *Relay->p87RPI)  	//当前电流大于谐波抑制后的门槛值
	{
		/*if((*Relay->p68IdH2MCFG == CFG_STATUS_SET)||(*Relay->p68IdH2CCFG == CFG_STATUS_SET)||(*Relay->p68IdH2ICFG == CFG_STATUS_SET)|| \
			(*Relay->p68IrH2MCFG == CFG_STATUS_SET)||(*Relay->p68IdH5CFG == CFG_STATUS_SET)||(*Relay->p68IdHMulCFG == CFG_STATUS_SET))
		{
			if((*Relay->pIdH2Flag == TRUE)||(*Relay->pIdH5Flag == TRUE)||(*Relay->pIrH2Flag == TRUE)||(*Relay->pIdHMulFlag == TRUE))	//被闭锁，复位，返回
			{
				Reset87RI(Relay);
				return;
			}
		}*/
		if((*Relay->p68IdH2MCFG == CFG_STATUS_SET)||(*Relay->p68IdH2CCFG == CFG_STATUS_SET)||(*Relay->p68IdH2ICFG == CFG_STATUS_SET))
		{
			if(*Relay->pIdH2Flag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
		if(*Relay->p68IrH2MCFG == CFG_STATUS_SET)
		{
			if(*Relay->pIrH2Flag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
		if(*Relay->p68IdH5CFG == CFG_STATUS_SET)
		{
			if(*Relay->pIdH5Flag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
		if(*Relay->p68IdHMulCFG == CFG_STATUS_SET)
		{
			if(*Relay->pIdHMulFlag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
	}
	else
	{
		Reset87RI(Relay);
		return;
	}
	if(Relay->pIr->Mod <= *Relay->pIRS1Set)  //制动电流位于第一段内，动作定值为门槛值
	{
		R87RISet = *Relay->p87RPI*100;
	}
	else if((Relay->pIr->Mod > *Relay->pIRS1Set)&&(Relay->pIr->Mod <= *Relay->pIRS2Set)) //制动电流在第二段内
	{
		R87RISet = *Relay->pSLP1Set*(Relay->pIr->Mod - *Relay->pIRS1Set) + *Relay->p87RPI*100;
	}
	else if(Relay->pIr->Mod > *Relay->pIRS2Set)	//制动电流在第三段内
	{
		LONG tId;
		tId = *Relay->pSLP1Set*(*Relay->pIRS2Set - *Relay->pIRS1Set) + *Relay->p87RPI*100;
		R87RISet = *Relay->pSLP2Set*(Relay->pIr->Mod - *Relay->pIRS2Set) + tId;
	}
	if(Relay->pId->Mod*100 < R87RISet)//当前差动电流小于动作值
	{		
		Reset87RI(Relay);
		return;
	}
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	if(!ReadRelayWord(Relay->StartRelayWord)) //尚未入段
	{
		MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
		WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
		Relay->wRelFCounter++;
		return;
	}
	Relay->wRelFCounter++;
	if(Relay->wRelFCounter >=2)	//动作
	{
		WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
		MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
	}
		

}
/*********************************************************************************************
 *
 * 功能描述 	复位比率差动保护元件
 *
 * 参数说明 	 比率差动保护参数结构
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/
void Reset87RI(TRel87R *Relay,BOOL resetFlag)
{	
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //保护入段 
	{
		MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
	}
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
	Relay->wRelFCounter = 0;
	Relay->wRelRCounter = 0;
	Relay->dwRelTimer	= 0;
}
/*********************************************************************************************
 *
 * 功能描述 	差流越限告警处理
 *
 * 参数说明 	 差流越限告警元件
 *				 
 *
 * 返回代码
 *				  无
 *
 * 其它说明：		   
 *
**********************************************************************************************/
void R87IAlarm(T87IAlarm *Relay)
{
	if(*Relay->p87R_A_CFG==CFG_STATUS_SET)
	{
		if(Relay->pIDA->Mod > *Relay->p87RIASet*(*Relay->p87RIKSet)/100)
			Relay->ACnt++;
		else
		{		
			Relay->ACnt = 0;
			Relay->AAlarmFlag = FALSE;
		}
		if(Relay->ACnt > Relay->AlarmCnt)
		{
			Relay->AAlarmFlag = TRUE;
		}
	}
	if(*Relay->p87R_B_CFG==CFG_STATUS_SET)
	{
		if(Relay->pIDB->Mod > *Relay->p87RIBSet*(*Relay->p87RIKSet)/100)
			Relay->BCnt++;
		else
		{
			Relay->BCnt = 0;
			Relay->BAlarmFlag = FALSE;
		}
		if(Relay->BCnt > Relay->AlarmCnt)
		{
			Relay->BAlarmFlag = TRUE;
		}
	}
	if(*Relay->p87R_C_CFG==CFG_STATUS_SET)
	{
		if(Relay->pIDC->Mod > *Relay->p87RICSet*(*Relay->p87RIKSet)/100)
			Relay->CCnt++;
		else
		{
			Relay->CCnt = 0;
			Relay->CAlarmFlag = FALSE;
		}
		if(Relay->CCnt > Relay->AlarmCnt)
		{
			Relay->CAlarmFlag = TRUE;
		}
	}
	
	if((Relay->CAlarmFlag == TRUE)||(Relay->BAlarmFlag == TRUE)||(Relay->AAlarmFlag == TRUE))
	{
		WriteRelayWord(Relay->ARRW,RELAY_ON);
		if(Relay->MakeRptFlag == FALSE)
		{
			MakeAlarmRpt(Relay->wRelRptNum);
			Relay->MakeRptFlag = TRUE;			
		}
	}
	else
	{
		WriteRelayWord(Relay->ARRW,RELAY_OFF);
		Relay->MakeRptFlag = FALSE;		
	}
}
/*********************************************************************************************
 *
 * 功能描述     捕捉一周波两端过零点附近的各三个采样点，利用拉格朗日二次插值算法分别计算
                此两个过零点的逼近值，利用其时间差计算被测电量的频率
                该函数计算一组频率值
 *				
 * 参数说明    - Fpre :      输入 前次采样频率
			   - TestElec :  输入 被测电量
			   - TFMask:     输入 测频电量数据掩码		 
			   - dwTestFBuf: 输入 测频缓冲区
			   - Fnow:       输出 本次测频值
			   - ts:         输出 本次采样周期

 * 返回代码      无
**********************************************************************************************/

BOOL TestFrequence(DWORD Fpre,LONG TestElec,DWORD TFMask,SHORT *wTestFBuf,LONG *dwCaluBuf,LONG *Fnow,BOOL trace)
{
	LLONG x0=0,x1=0,x2=0;
	LLONG y0=0,y1=0,y2=0;
	LLONG yb = 0;
	LLONG Fm = 0;
	LLONG temp1,temp2,temp3;
	LONG num = 0;
	DWORD wYval[7];
	DWORD temp;
	WORD wCounter=0;
	DWORD	dwDataPtr = dwADDataWritePtr;

	
	dwDataPtr = BackSamplePtr(dwDataPtr,POINT_PER_PERIOD*FREQBUFSIZE);
	ReadHisSampData(dwDataPtr,TFMask,wTestFBuf,POINT_PER_PERIOD*FREQBUFSIZE);//读取9个周波的采样点

	//滤波，滤掉高次谐波
	for(LONG i =POINT_PER_PERIOD*FREQBUFSIZE-1;i>16;i-- )
	{
		dwCaluBuf[i]=wTestFBuf[i]-wTestFBuf[i-16];
	}
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-1;i>17;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-17];
	}
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-1;i>18;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-18];
	}
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-1;i>17;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-17];
	}
	
	//前次频率不在合理频率变化范围之内，前次频率取50Hz
	if(Fpre>MAX_FREQUENCE||Fpre<MIN_FREQUENCE) 
	{
		Fpre = 50000;	
//		ReportMsg("error,force freq= 5000");		
	}
	//当前电压值过小，令所测频率值=0，返回
	if(ToAbs(TestElec)<MIN_TESTU)
	{
	   return FALSE;
	}
	//捕捉过零点并插值计算过零点近似值
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-2;i>2;i--)
	{		
		//if(((LLONG)dwCaluBuf[i]*dwCaluBuf[i-1]<=0)&&((LLONG)dwCaluBuf[i+1]*dwCaluBuf[i-2]<=0)&&(dwCaluBuf[i]!=0))
		if((((dwCaluBuf[i]<=0)&&(dwCaluBuf[i-1]>=0))||((dwCaluBuf[i]>=0)&&(dwCaluBuf[i-1]<=0)))&&
			(((dwCaluBuf[i+1]<=0)&&(dwCaluBuf[i-2]>=0))||((dwCaluBuf[i+1]>=0)&&(dwCaluBuf[i-2]<=0)))&&
			(dwCaluBuf[i]!=0))
		{
			num++;
			if(num%2)
			{
				x0 = (LONG)dwCaluBuf[i-2];
				y0 = (i-2)*1000;
				x1 = (LONG)dwCaluBuf[i-1];
				y1 = (i-1)*1000;
				x2 = (LONG)dwCaluBuf[i];
				y2 = i*1000;

				temp1 = y0*x1*x2;
				temp2 = (x0-x1)*(x0-x2);
				yb += temp1/temp2;
				temp1 = y1*x0*x2;
				temp2 = (x1-x0)*(x1-x2);
				yb += temp1/temp2;
				temp1 = y2*x0*x1;
				temp2 = (x2-x0)*(x2-x1);
				yb += temp1/temp2;
				
				wYval[wCounter]= yb;
				yb = 0;
				wCounter++;
				if(wCounter == (NUM_FREQ_PER_AITASK+2))
					break;				
			}	
		}
		if(i==3)
		{
	//		ReportMsg("Error: cannot find 7 zero value");
			return FALSE;
		}
	}

	if(wCounter == (NUM_FREQ_PER_AITASK+2))
	{
		for(LONG j=0;j<NUM_FREQ_PER_AITASK;j++)
		{
			if(!trace)
			{
				Fm=1000000000000/(625*(wYval[j]-wYval[j+1]));
				Fnow[j]=(LONG)Fm;	
			}
			else
			{
				temp = wYval[j]-wYval[j+1];
				temp3 = Fpre;
				Fnow[j] = (temp3*10000*POINT_PER_PERIOD+(temp/2))/temp;  //频率跟踪，固定采样点数 
			}
			
		}
	}
	return TRUE;

}
/*********************************************************************************************
 *
 * 功能描述     捕捉一周波两端过零点附近的各三个采样点，利用拉格朗日二次插值算法分别计算
                此两个过零点的逼近值，利用其时间差计算被测电量的频率
                该函数计算当前最近一周波采样点的频率
 *				
 * 参数说明    - Fpre :      输入 前次采样频率
			   - TestElec :  输入 被测电量
			   - TFMask:     输入 测频电量数据掩码		 
			   - dwTestFBuf: 输入 测频缓冲区
			   - Fnow:       输出 本次测频值
			   - ts:         输出 本次采样周期

 * 返回代码      无
**********************************************************************************************/

BOOL CalcImmeFrequence(DWORD ADDataWritePtr,LONG TestElec,DWORD TFMask,SHORT *wTestFBuf,LONG *dwCaluBuf,LONG *Fnow)
{
	LLONG x0=0,x1=0,x2=0;
	LLONG y0=0,y1=0,y2=0;	
	LLONG temp1,temp2,temp3;
	LLONG yb = 0;
	LLONG Fm = 0;
	LONG num = 0;
	DWORD wYval[7];
	WORD wCounter=0;
	DWORD	dwDataPtr = ADDataWritePtr;
	
	dwDataPtr = BackSamplePtr(dwDataPtr,FREQUENCE_SAMP_NUM);
	
	
	//ReadHisSampData(dwDataPtr,TFMask,wTestFBuf,FREQUENCE_SAMP_NUM);
	ReadHisSampData_Single(dwDataPtr,TFMask,wTestFBuf,FREQUENCE_SAMP_NUM);

	

	//滤波，滤掉高次谐波
	for(LONG i =FREQUENCE_SAMP_NUM-1;i>16;i-- )
	{
		dwCaluBuf[i]=wTestFBuf[i]-wTestFBuf[i-16];
	}
	for(LONG i=FREQUENCE_SAMP_NUM-1;i>17;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-17];
	}
	for(LONG i=FREQUENCE_SAMP_NUM-1;i>18;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-18];
	}
	//当前电压值过小，令所测频率值=0，返回
	if(ToAbs(TestElec)<MIN_TESTU)
	{
		*Fnow= 0;
	   return FALSE;
	}
	//捕捉过零点并插值计算过零点近似值
	for(LONG i=FREQUENCE_SAMP_NUM-RELAY_SAMP_INTERVAL-2;i>2;i--)
	{	
		if(((dwCaluBuf[i]>0)&&(dwCaluBuf[i-1]<0)&&(dwCaluBuf[i+1]>0)&&(dwCaluBuf[i-2]<0))||
			((dwCaluBuf[i]<0)&&(dwCaluBuf[i-1]>0)&&(dwCaluBuf[i+1]<0)&&(dwCaluBuf[i-2]>0)))
		{
			num++;
			if(num%2)
			{
				x0 = (LONG)dwCaluBuf[i-2];
				y0 = (i-2)*1000;
				x1 = (LONG)dwCaluBuf[i-1];
				y1 = (i-1)*1000;
				x2 = (LONG)dwCaluBuf[i];
				y2 = i*1000;

				temp1 = y0*x1*x2;
				temp2 = (x0-x1)*(x0-x2);
				yb += temp1/temp2;
				temp1 = y1*x0*x2;
				temp2 = (x1-x0)*(x1-x2);
				yb += temp1/temp2;
				temp1 = y2*x0*x1;
				temp2 = (x2-x0)*(x2-x1);
				yb += temp1/temp2;
				
				wYval[wCounter]= yb;
				yb = 0;
				wCounter++;
				if(wCounter == 2)
					break;				
			}	
		}
	
		if(i==3)
		{
//			ReportMsg("Error: cannot find 7 zero value");
			return FALSE;
		}
	}



	if(wCounter == 2)
	{
		Fm=1000000000000/(625*(wYval[0]-wYval[1]));
		*Fnow=(LONG)Fm;	 //放大了1000倍
//		if((*Fnow>50010)||(*Fnow<49990))
//			ReportMsg("Error: 11111");
	}
	
	return TRUE;
}

/*
void WriteDebugRW(WORD wIndex,BYTE byVal)
{
	WORD wDebugIndex;

	if(wIndex < VIRTUAL_RELAY_WORD_START)
		return;

	wDebugIndex = wIndex - VIRTUAL_RELAY_WORD_START;

	if(wDebugIndex >= VIRTURAL_RELAY_WORD_NUM)
		return;
	
	byDebugRelayBuf[wDebugIndex] = byVal;
}
BYTE ReadRWFullValue(WORD wIndex)
{
	if(wIndex >= RELAY_WORD_NUM)
		return BYTE(-1);
	return byRelayWordValue[wIndex];
}
*/
BOOL Relay_GetBOINStatus(BYTE *pStatus)
{
	if(pStatus == NULL)
		return FALSE;

	*pStatus++ = LLBYTE(dwBoInvalidStatus[0]);
	*pStatus++ = LHBYTE(dwBoInvalidStatus[0]);
	*pStatus++ = HLBYTE(dwBoInvalidStatus[0]);
	*pStatus++ = HHBYTE(dwBoInvalidStatus[0]);
	
	*pStatus++ = LLBYTE(dwBoInvalidStatus[1]);
	*pStatus++ = LHBYTE(dwBoInvalidStatus[1]);
	*pStatus++ = HLBYTE(dwBoInvalidStatus[1]);
	*pStatus++ = HHBYTE(dwBoInvalidStatus[1]);
	
	return TRUE;
}

BOOL Relay_GetBOBRStatus(BYTE *pStatus)
{
	if(pStatus == NULL)
		return FALSE;

	*pStatus++ = LLBYTE(dwBoBreakStatus[0]);
	*pStatus++ = LHBYTE(dwBoBreakStatus[0]);
	*pStatus++ = HLBYTE(dwBoBreakStatus[0]);
	*pStatus++ = HHBYTE(dwBoBreakStatus[0]);
	
	*pStatus++ = LLBYTE(dwBoBreakStatus[1]);
	*pStatus++ = LHBYTE(dwBoBreakStatus[1]);
	*pStatus++ = HLBYTE(dwBoBreakStatus[1]);
	*pStatus++ = HHBYTE(dwBoBreakStatus[1]);
	
	return TRUE;
}
BOOL GetMaintSWStatus()
{
	if(*g_MaintSW == SW_STATUS_CUT)
		return FALSE;
	else
		return TRUE;
}

/************************************************************************************************
*功能: 模拟指定序号的报告
*参数: 报告序号
*返回: 无
*备注: 
*************************************************************************************************/
void SimActionReport(WORD no)
{
	// 填写一个无效启动
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(0x55AA,RELAY_INF_NOUSED,&AbsTime,TRUE);
	// 填写一个动作报告
	Report_Action(0x55AA,no);
	// 结束一个报告
	Report_End(0x55AA,no);
}
/************************************************************************************************
*功能: 傅里叶分析计算电量基波，适用于20点采样的测距装置
*参数: 报告序号
*返回: 无
*备注: 
*************************************************************************************************/
void CaluBaseFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14629*(-pDataBuf[0]+pDataBuf[20]);
			dwRealAmplitude += 1432*(pDataBuf[1]+pDataBuf[9]-pDataBuf[11]-pDataBuf[19]);
			dwRealAmplitude += 2724*(pDataBuf[2]+pDataBuf[8]-pDataBuf[12]-pDataBuf[18]);
			dwRealAmplitude += 3749*(pDataBuf[3]+pDataBuf[7]-pDataBuf[13]-pDataBuf[17]);
			dwRealAmplitude += 4407*(pDataBuf[4]+pDataBuf[6]-pDataBuf[14]-pDataBuf[16]);
			dwRealAmplitude += 4634*(pDataBuf[5]-pDataBuf[15]);
			dwRealAmplitude += 0*(-pDataBuf[10]);
			if(dwRealAmplitude>=0)                                                           /*四舍五入保证精度*/
				elec->Real = (dwRealAmplitude+32767)>>15;
			else
				elec->Real = (dwRealAmplitude-32767)>>15;

			dwImagAmplitude += 2317*(pDataBuf[0]+pDataBuf[20]);
			dwImagAmplitude += 4407*(pDataBuf[1]-pDataBuf[9]-pDataBuf[11]+pDataBuf[19]);
			dwImagAmplitude += 3749*(pDataBuf[2]-pDataBuf[8]-pDataBuf[12]+pDataBuf[18]);
			dwImagAmplitude += 2724*(pDataBuf[3]-pDataBuf[7]-pDataBuf[13]+pDataBuf[17]);
			dwImagAmplitude += 1432*(pDataBuf[4]-pDataBuf[6]-pDataBuf[14]+pDataBuf[16]);
			dwImagAmplitude += 0*(-pDataBuf[5]-pDataBuf[15]);
			dwImagAmplitude += 4634*(-pDataBuf[10]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+32767)>>15;
			else
				elec->Imag = (dwImagAmplitude-32767)>>15;
		}
}
void CaluSecondFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 6681*(-pDataBuf[0]);
		dwRealAmplitude += 3277*(pDataBuf[1]-pDataBuf[6]+pDataBuf[11]-pDataBuf[16]);
		dwRealAmplitude += 4577*(pDataBuf[2]-pDataBuf[7]+pDataBuf[12]-pDataBuf[17]);
		dwRealAmplitude += 4129*(pDataBuf[3]-pDataBuf[8]+pDataBuf[13]-pDataBuf[18]);
		dwRealAmplitude += 2104*(pDataBuf[4]-pDataBuf[9]+pDataBuf[14]-pDataBuf[19]);
		dwRealAmplitude += 725*(-pDataBuf[5]+pDataBuf[10]-pDataBuf[15]);
		dwRealAmplitude += 7406*(pDataBuf[20]);
		if(dwRealAmplitude>=0)                                                           /*四舍五入保证精度*/
			elec->Real = (dwRealAmplitude+32767)>>15;
		else
			elec->Real = (dwRealAmplitude-32767)>>15;

		dwImagAmplitude += 3404*(pDataBuf[0]);
		dwImagAmplitude += 3277*(pDataBuf[1]-pDataBuf[6]+pDataBuf[11]-pDataBuf[16]);
		dwImagAmplitude += 725*(pDataBuf[2]-pDataBuf[7]+pDataBuf[12]-pDataBuf[17]);
		dwImagAmplitude += 2104*(-pDataBuf[3]+pDataBuf[8]-pDataBuf[13]+pDataBuf[18]);
		dwImagAmplitude += 4129*(-pDataBuf[4]+pDataBuf[9]-pDataBuf[14]+pDataBuf[19]);
		dwImagAmplitude += 4577*(-pDataBuf[5]+pDataBuf[10]-pDataBuf[15]);
		dwImagAmplitude += 1173*(pDataBuf[20]);
		if(dwImagAmplitude>=0)
			elec->Imag = (dwImagAmplitude+32767)>>15;
		else
			elec->Imag = (dwImagAmplitude-32767)>>15;
	}
}
void CaluThirdFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 3609*(-pDataBuf[0]);
		dwRealAmplitude += 4407*(pDataBuf[1]-pDataBuf[5]-pDataBuf[11]+pDataBuf[15]);
		dwRealAmplitude += 3749*(pDataBuf[2]-pDataBuf[4]-pDataBuf[12]+pDataBuf[14]);
		dwRealAmplitude += 0*(-pDataBuf[3]-pDataBuf[13]);
		dwRealAmplitude += 1432*(-pDataBuf[6]-pDataBuf[10]+pDataBuf[16]);
		dwRealAmplitude += 2724*(pDataBuf[7]+pDataBuf[9]-pDataBuf[17]-pDataBuf[19]);
		dwRealAmplitude += 4634*(pDataBuf[8]-pDataBuf[18]);
		dwRealAmplitude += 5041*(pDataBuf[20]);
		if(dwRealAmplitude>=0)                                                           /*四舍五入保证精度*/
			elec->Real = (dwRealAmplitude+32767)>>15;
		else
			elec->Real = (dwRealAmplitude-32767)>>15;

		dwImagAmplitude += 3609*(pDataBuf[0]);
		dwImagAmplitude += 1432*(pDataBuf[1]+pDataBuf[5]-pDataBuf[11]-pDataBuf[15]);
		dwImagAmplitude += 2724*(-pDataBuf[2]-pDataBuf[4]+pDataBuf[12]+pDataBuf[14]);
		dwImagAmplitude += 4634*(-pDataBuf[3]+pDataBuf[13]);
		dwImagAmplitude += 4407*(pDataBuf[6]-pDataBuf[10]-pDataBuf[16]);
		dwImagAmplitude += 3749*(pDataBuf[7]-pDataBuf[9]-pDataBuf[17]+pDataBuf[19]);
		dwImagAmplitude += 0*(-pDataBuf[8]-pDataBuf[18]);
		dwImagAmplitude += 798*(pDataBuf[20]);
		if(dwImagAmplitude>=0)
			elec->Imag = (dwImagAmplitude+32767)>>15;
		else
			elec->Imag = (dwImagAmplitude-32767)>>15;
	}
}
void CaluFifthFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 513*(-pDataBuf[0]);
		dwRealAmplitude += 3749*(pDataBuf[1]-pDataBuf[3]+pDataBuf[5]-pDataBuf[7]+pDataBuf[9] \
			-pDataBuf[11]+pDataBuf[13]-pDataBuf[15]+pDataBuf[17]-pDataBuf[19]);
		dwRealAmplitude += 2724*(-pDataBuf[2]+pDataBuf[4]-pDataBuf[6]+pDataBuf[8]-pDataBuf[10] \
			+pDataBuf[12]-pDataBuf[14]+pDataBuf[16]-pDataBuf[18]);
		dwRealAmplitude += 3236*(pDataBuf[20]);
		
		if(dwRealAmplitude>=0)                                                           /*四舍五入保证精度*/
			elec->Real = (dwRealAmplitude+32767)>>15;
		else
			elec->Real = (dwRealAmplitude-32767)>>15;

		dwImagAmplitude += 3236*(pDataBuf[0]);
		dwImagAmplitude += 2724*(-pDataBuf[1]+pDataBuf[3]-pDataBuf[5]+pDataBuf[7]-pDataBuf[9] \
			+pDataBuf[11]-pDataBuf[13]+pDataBuf[15]-pDataBuf[17]+pDataBuf[19]);
		dwImagAmplitude += 3749*(-pDataBuf[2]+pDataBuf[4]-pDataBuf[6]+pDataBuf[8]-pDataBuf[10] \
			+pDataBuf[12]-pDataBuf[14]+pDataBuf[16]-pDataBuf[18]);
		dwImagAmplitude += 513*(pDataBuf[20]);

		if(dwImagAmplitude>=0)
			elec->Imag = (dwImagAmplitude+32767)>>15;
		else
			elec->Imag = (dwImagAmplitude-32767)>>15;
	}
}
void MakeActRptWithoutStart(DWORD inf)
{	
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(0x55BB,RELAY_INF_NOUSED,&AbsTime,FALSE);
	MakeActRpt(inf,0x55BB);
	DB_RelayEnd(0x55BB);
}
void CTCheck(TRelCTBrokPar *Relay)
{
	if(*(Relay->pRelCTBrokCFG) == CFG_STATUS_CLR)	//CT断线未投入
	{
		if(ReadRelayWord(Relay->ActRelayWord))
			MakeAlarmRpt(Relay->wRelResRptNum);
		ResetCTCheck(Relay);
		return;
	}
	if(*(Relay->pRelCurMax) < *(Relay->pRelCurSet))
		return;
	if(ReadRelayWord(Relay->ActRelayWord))
	{		
		if(*(Relay->pRelCurMax) < (*(Relay->pRelCurMin))*3)
		{
			if(*(Relay->pRelCurMax) - *(Relay->pRelCurMin) < *(Relay->pRelCurSet))
			{			
				Relay->byRelRCounter++; 
				if(Relay->byRelRCounter > 5)
				{
					MakeAlarmRpt(Relay->wRelResRptNum);
					ResetCTCheck(Relay);
				}
			}
			else
				Relay->byRelRCounter = 0; 
			return;
		}
		return;
	}
	if(*(Relay->pRelCurMax) < (*(Relay->pRelCurMin))*3)
	{
		ResetCTCheck(Relay);
	}
	else
	{
		if(!ReadRelayWord(Relay->StartRelayWord))		//尚未入段
		{
			Relay->byRelFCounter++;
			if(Relay->byRelFCounter > 2)
			{
				Relay->dwRelTimer = *(Relay->dwSysTimer);
				WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
				return;
			}
		}
		else
		{
			if((*(Relay->dwSysTimer) - Relay->dwRelTimer) >= *(Relay->pRelTimeSet))
			{
				WriteRelayWord(Relay->ActRelayWord, RELAY_ON);
				MakeAlarmRpt(Relay->wRelRptNum);
			}
		}
	}
}
void ResetCTCheck(TRelCTBrokPar *Relay)
{
	WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
	Relay->byRelFCounter = 0; 
	Relay->byRelRCounter = 0; 
	Relay->dwRelTimer = 0; 
}
BOOL PhaseCheck(const LONG *A1Buf,const LONG *A2Buf)
{
	LLONG value = 0;
	for(int i=0;i<POINT_PER_PERIOD;i++)
	{
		value += A1Buf[i]*A2Buf[i];
	}
	if(value>0)
		return TRUE;
	else
		return FALSE;
}
void RefreshCommRW(WORD ETHRW,WORD COMRW,WORD CANRW,WORD MAINRW)
{		
	static DWORD cnt = 0;
	WORD sernum,cannum,ethnum;	
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();
	TSerialPara tSerial;
	TCanBusPara tCan;
	TEthPortPara tEth;
	BOOL maintFlag = FALSE;
	if(ethnum > 0)
	{
		for(WORD i=0;i<ethnum;i++)
		{			
			DB_GetETHPara(i,&tEth);
			WriteCommRW(i,tEth.wPortID,ETHRW,SS_ETHA_COMM_ERR);
			if(tEth.wDriver == 2) // 2代表维护规约
			{
				if(!ReadRelayWord(ETHRW+i))
					maintFlag = TRUE;					
			}
		}
	}
	else
	{
		for(WORD i=0;i<10;i++)
		{
			if(!ReadRelayWord(ETHRW+i))
				WriteRelayWord(ETHRW+i,RELAY_ON);
		}
	}
	if(sernum > 0)
	{
		for(WORD i=0;i<sernum;i++)
		{
			DB_GetSerialPara(i, &tSerial);		
			WriteCommRW(i,tSerial.wPortID,COMRW,SS_SER_COMM_ERR);			
			if(tSerial.byDriver == 2) // 2代表维护规约
			{
				if(!ReadRelayWord(COMRW+i))
					maintFlag = TRUE;					
			}
		}
	}
	else		
	{
		if(!ReadRelayWord(COMRW))
			WriteRelayWord(COMRW,RELAY_ON);
	}
	if(cannum > 0)
	{
		for(WORD i=0;i<cannum;i++)
		{
			DB_GetCANPara(i, &tCan);
			WriteCommRW(i,tCan.wPortID,CANRW,SS_CAN_COMM_ERR);			
			if(tCan.byDriver == 2) // 2代表维护规约
			{
				if(!ReadRelayWord(CANRW+i))
					maintFlag = TRUE;					
			}
		}
	}
	else
	{
		for(WORD i=0;i<2;i++)			
		{
			if(!ReadRelayWord(CANRW+i))
				WriteRelayWord(CANRW+i,RELAY_ON);
		}
	}
	
	cnt++;
	if(cnt>100)
	{
		if(maintFlag == TRUE)
		{
			if(!ReadRelayWord(MAINRW))
				WriteRelayWord(MAINRW,RELAY_ON);
		}
		else
		{
			if(ReadRelayWord(MAINRW))
				WriteRelayWord(MAINRW,RELAY_OFF);
		}
	}
}

void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag)
{		
	BYTE commuStatus;
	if(DAE_GetCommStatus(wPortID,commuStatus)!=DB_ERR_OK)		
	{
		if(!ReadRelayWord(RelayWord+i))
			WriteRelayWord(RelayWord+i,RELAY_ON);
	}
	else
	{
//		if((DAE_GetRunningFlags(RunningFlag+i))||(commuStatus == COMM_STATUS_BREAK))
		if(commuStatus == COMM_STATUS_BREAK)    //20140314  yxs
		{
			if(!ReadRelayWord(RelayWord+i))
				WriteRelayWord(RelayWord+i,RELAY_ON);
		}
		else
		{
			if(ReadRelayWord(RelayWord+i))
				WriteRelayWord(RelayWord+i,RELAY_OFF);
		}
	}
}

//:~
