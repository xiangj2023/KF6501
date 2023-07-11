/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			max11046.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           max11046AD芯片驱动代码                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      	max11046AD芯片初始化、启动采样控制、采样中断处理及原始数据读取接口                 
*                                                                                        
*                                                                                  
* 函数                                                                             
*		   	                                                                  
*      	ADCInit			AD采样功能硬件初始化
*		ADSampleInt		AD采样中断处理 	                                                                  
*		ReadSampleData	读取某几个通道的指定周波号的连续34点采样数据   	                                                                  
*		ReadPreSingleData	读取某几个通道的由采样间隔指定的采样点数据   	                                                                  
*		   	                                                                  
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/12/18    初始创建                         
*                                                                                  
********************************************************************************************/

#include "max11046.h"
#include "daefunc.h"

extern DWORD dwRelayTaskID;
extern BOOL RelayTaskInitOK;

SHORT *pwADSampleData = NULL;	//原始采样数据缓冲区
DWORD dwADDataWritePtr;	//采样缓冲区数据读写指针
DWORD dwRelayDataPtr;	//保护处理数据指针，防止保护读写原始采样数据或录波处理过慢
//DWORD dwOldDataPtr;

DWORD dwADIntCounter = 1;

//DWORD dwADMsgCnt = 0;
DWORD dwADResumeCnt = 0;	//AD功能恢复计数，从异常到正常后需保证正常采样一个完整周波数据
DWORD dwSampleErrCnt = 0;
WORD wSampleErrFlag;

//由于故测距6571，不需要全部16路模拟采样数据，因此需要剔除无效通道数据
#ifdef PRO_6571
	BOOL ChanValFlag[AD7606_CHAN_NUM] = {TRUE, TRUE, TRUE, TRUE, FALSE,  FALSE,  FALSE,  FALSE, \
									TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
#else
#ifdef PRO_6535
	BOOL ChanValFlag[AD7606_CHAN_NUM] = {TRUE, TRUE, TRUE, TRUE, TRUE,  TRUE,	TRUE,  TRUE, \
										TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE};
#else
	BOOL ChanValFlag[AD7606_CHAN_NUM] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, \
									TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
#endif
#endif
/*********************************************************************************************
 *
 * 功能描述     AD采样功能硬件初始化，包括AD芯片模式设置，中断加载，启动采样引脚初始化等
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void ADCInit()
{
	//初始化采样缓冲区及数据操作指针
	pwADSampleData = (SHORT *)OS_Malloc(SAMPLE_DATA_LEN*sizeof(WORD));
	if(pwADSampleData == NULL)
	{
		LogError("ADCInit",FILE_LINE,"malloc error!");
		return;
	}
	memset(pwADSampleData, 0, SAMPLE_DATA_LEN*sizeof(WORD));

	dwADDataWritePtr = 0;

	//定时器引脚初始化
	MCF_GPIO_PAR_TIMER = 0x0000;	//将所有定时器引脚设置为I/O方式
	MCF_GPIO_PDDR_TIMER = 0xff;		//初始化定时器引脚都为输出功能
	MCF_GPIO_PODR_TIMER = 0x0c;	//初始化定时器引脚初始状态为低,闭锁遥控

	outport(ADC1_CS_BASE, 0x0004);	//设置ADC工作模式:外部参考、CONVST模式
	outport(ADC2_CS_BASE, 0x0004);

	//ADC为IRQ6
	OS_SetVector(MCF_INTC0_LVL_LVL6, ADSampleInt, 0);
	OS_IntEnable(MCF_INTC0_LVL_LVL6);

	//设置下降沿中断
#if(AD_DEV_TYPE == AD7606_DEV)
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_EPPAx_LEVEL);	//设置低电平触发
#else
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_EPPAx_FALLING);//设置下降沿触发中断
#endif
	//中断允许
	MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE6;

	//设置为输出方式
	MCF_GPIO_PDDR_TIMER |= (AD1_START_PIN|AD2_START_PIN);

	//设置初始状态为低
	ADStartPinLow();
}

/*********************************************************************************************
 *
 * 功能描述     AD采样中断处理，包括清中断，数据读取，以及通知保护任务等，对于不同的AD芯片
 *				由于通道路数不同，因此读取方式存在一定差异。
 *
 * 参数说明      
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void ADSampleInt(void * arg)
{
	static BYTE bADSampleCnt;
	int i;
	BYTE data;
	
	//清除中断
	MCF_EPORT_EPFR |= 0x40;

	bADSampleCnt++;

	//刷新AD中断计数，并更新AD芯片工作状态为正常。
	dwADIntCounter++;

	//AD功能恢复计数递减
	if(dwADResumeCnt)
		dwADResumeCnt--;

	
#if(AD_DEV_TYPE == AD7606_DEV)
	//WORD wFlag;
	//检查AD采样功能异常标志
	wSampleErrFlag = inport(0xa20300c0);
	if(wSampleErrFlag != 0xaaaa)
		dwSampleErrCnt++;

	BYTE *pBuf = (BYTE*)&pwADSampleData[dwADDataWritePtr];

	for(i=0; i<AD7606_CHAN_NUM*2; i++)
	{
		data = inportb(AD7606_BASE+i);

		//只有有效通道的数据保存到采样缓冲区
		if(ChanValFlag[i/2] == TRUE)
		{
			*pBuf = data;
			pBuf++;
		}
	}

	dwADDataWritePtr += AD_CHAN_NUM;

#else
	//取第一片AD的原始采样数据	
	for(i=0; i<CHANNEL_PER_AD; i++)
	{
		pwADSampleData[dwADDataWritePtr] =inport(ADC1_CS_BASE);// MAKEWORD((BYTE)dwADIntCounter, (BYTE)i);//
		dwADDataWritePtr++;
	}

	//取第二片AD的原始采样数据	
	for(i=0; i<CHANNEL_PER_AD; i++)
	{
		pwADSampleData[dwADDataWritePtr] =inport(ADC2_CS_BASE);//MAKEWORD((BYTE)dwADIntCounter, (BYTE)(i+8));
		dwADDataWritePtr++;
	}

#if(AD_DEV_TYPE == AD7656_DEV)
	//取第三片AD的原始采样数据,对于最后一片最后2路无效	
	for(i=0; i<(CHANNEL_PER_AD-2); i++)
	{
		pwADSampleData[dwADDataWritePtr] = inport(ADC3_CS_BASE);//MAKEWORD((BYTE)dwADIntCounter, (BYTE)(i+16));//
		dwADDataWritePtr++;
	}
#endif
#endif
	dwADDataWritePtr += COMP_CHAN_NUM;
	dwADDataWritePtr %= SAMPLE_DATA_LEN;

	//增加发送消息，通知保护任务,注意在AD功能恢复后，必须保证完成一周波采样才能通知保护任务
	if((bADSampleCnt%RELAY_SAMP_INTERVAL) || dwADResumeCnt)
	{
		return;
	}
	bADSampleCnt = 0;
	
	//保护处理时以发送消息前锁存的写指针为准.
	dwRelayDataPtr = dwADDataWritePtr;
	
	//只有等到保护任务初始化完成后才能进行采样处理
	if(RelayTaskInitOK == FALSE)
		return;
	
	OS_PostMessage(dwRelayTaskID, SM_RELAY_INT, EV_RELAYBIT, 0, 0, 0);
}

/*********************************************************************************************
 *
 * 功能描述     从原始采样缓冲区中读取某几个通道的指定周波号的连续34点采样数据，数据按照通道
 *				保存顺序。
 *
 * 参数说明      - pData	: 输出 	需要保存的数据缓冲区
 *				 - ChanMask : 输入	通道掩码
 *				 - CycleNo	: 输入	周波号，当前周波为0，上一周波为1，依此类推
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS ReadSampleData(LONG *pData, DWORD ChanMask, BYTE CycleNo)
{
	SHORT *pwSampleData;
	DWORD dwReadPtr, dwPriWritePtr;
	BYTE channel;
	int  ChannelNo, LineNo, offset;

	//合法性检查
	ASSERT(pData != NULL);
	ASSERT(ChanMask != 0);
	if((pData == NULL) || (ChanMask == 0) || (pwADSampleData == NULL))
		return ERR_FAILURE;

	dwPriWritePtr = dwRelayDataPtr;
	pwSampleData = pwADSampleData;

	dwReadPtr = (SAMPLE_DATA_LEN+dwPriWritePtr-(CycleNo+1)*SAM_LEN_PER_CYC-2*CHANNNEL_NUM)%SAMPLE_DATA_LEN;

	for( ChannelNo=0; ChannelNo<CHANNNEL_NUM; ChannelNo++)
	{
		if(ChanMask&(0x01<<ChannelNo))	//通道掩码有效
		{
			offset = dwReadPtr+ChannelNo;
			
			for( LineNo=0; LineNo<READ_POINT_NUM; LineNo++)
			{
				*pData++ = (LONG)pwSampleData[offset];
				offset += CHANNNEL_NUM;
				offset %= SAMPLE_DATA_LEN;
			}
		}
	}
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     从原始采样缓冲区中读取某几个通道的由采样间隔指定的采样点数据，每个通道都要保存
 *				3个周波的同相位数据，数据按照通道从旧到新的顺序保存。
 *
 * 参数说明      - pData	: 输出 	需要保存的数据缓冲区
 *				 - pChannel : 输入	通道路号指针
 *				 - Number	: 输入	通道个数
 *				 - Interval	: 输入	采样间隔数
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS ReadPreSingleData(LONG *pData, BYTE *pChannel, BYTE Number,BYTE Interval)
{
	SHORT *pwSampleData, *pTemp;
	DWORD dwReadPtr, dwPriWritePtr, dwTempPtr;
	BYTE channel;
	int i, j, k;
	
	//合法性检查
	ASSERT(pData != NULL);
	ASSERT(pChannel != NULL);
	if((pData == NULL) || (pChannel == NULL) || (Number == 0) || (pwADSampleData == NULL))
		return ERR_FAILURE;

	//读取原始采样写指针
	dwPriWritePtr = dwRelayDataPtr;//dwADDataWritePtr;

	//根据当前写指针和指定的采样间隔，计算需要的数据块的头位置
	dwReadPtr = (dwPriWritePtr+SAMPLE_DATA_LEN-(READ_SAMP_CYC_NUM-1)*SAM_LEN_PER_CYC-\
				Interval*CHANNNEL_NUM)%SAMPLE_DATA_LEN;

	pwSampleData = pwADSampleData;

	for(k=0; k<Number; k++)	//多个通道
	{
		for(i=0; i<READ_SAMP_CYC_NUM; i++)// 3个周波
		{
			//移动到下一个周波的本通道数据的开始位置
			dwTempPtr = dwReadPtr+pChannel[k]+i*SAM_LEN_PER_CYC; 
			dwTempPtr %= SAMPLE_DATA_LEN;

			//取连续多个采样间隔的单个通道的数据
			for(j=0; j<Interval; j++)	//多个采样间隔
			{
				*pData = (LONG)pwSampleData[dwTempPtr];
				pData++;
				//移动到下一个采样间隔
				dwTempPtr += CHANNNEL_NUM;
				dwTempPtr %= SAMPLE_DATA_LEN;
			}
		}	
	}
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     从原始采样缓冲区指定位置，读取指定点数和通道的历史采样数据
 *				
 *
 * 参数说明      - dwPtr	: 输入 	历史数据起始位置
 *				 - ChanMask : 输入	通道掩码
 *				 - pBuf		: 输出	需要保存的数据缓冲区
 *				 - wPointNum	: 输入	需要读取的采样点数
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS ReadHisSampData(DWORD dwPtr, DWORD dwMask, SHORT *pBuf, DWORD dwPointNum)
{
/*	
	SHORT	*pwSampleData, *pTemp;
	DWORD	dwReadPtr, dwPriWritePtr;
	DWORD	block1, block2;
	int 		i, j;
		
	if((dwMask==0) || (pBuf == NULL) || (dwPointNum == 0) )
		return ERR_FAILURE;
	
	//当前写指针前有一块连续数据长度满足要求
	if((dwPtr+dwPointNum*CHANNNEL_NUM) <= SAMPLE_DATA_LEN)
	{
//		ReportMsg("Disturb Record:	%d-->%d",dwPtr,dwPtr+dwPointNum*CHANNNEL_NUM);
		
		//计算出需要的数据块起始地址
		pwSampleData = pwADSampleData+dwPtr;

		for(i=0; i<dwPointNum; i++)
		{
			pTemp = pwSampleData+i*CHANNNEL_NUM;
			
			for(j=0; j<CHANNNEL_NUM; j++)
			{
				if(dwMask&(0x01<<j))	//通道掩码有效
				{
					*pBuf = pTemp[j];
					pBuf++;
					
				}		
			}
		}

	}
	else	//当前写指针前的数据块长度不能满足要求，需要从两块原始数据中读取
	{
		//注意根据顺序要求，需要先取尾部的数据块
		
		//计算每个数据块中采样点数
		block1 = PERIOD_NUMBER*POINT_PER_PERIOD - dwPtr/CHANNNEL_NUM;
		block2 = dwPointNum - block1;

		//取第一块采样数据
//		ReportMsg("Disturb Record Part1:  %d-->%d",dwPtr,dwPtr+block1*CHANNNEL_NUM);

		pwSampleData = pwADSampleData+dwPtr;		//指向第一块原始采样数据的开始
		
		for(i=0; i<block1; i++)
		{
			pTemp = pwSampleData+i*CHANNNEL_NUM;
			
			for(j=0; j<CHANNNEL_NUM; j++)
			{
				if(dwMask&(0x01<<j))	//通道掩码有效
				{
					*pBuf = pTemp[j];
					pBuf++;
				}
			}
		}

		//取第二块采样数据,在原始缓冲区的开始
//		ReportMsg("Disturb Record Part2:  0-->%d",block2*CHANNNEL_NUM);

		pwSampleData = pwADSampleData;
		
		for(i=0; i<block2; i++)
		{
			pTemp = pwSampleData+i*CHANNNEL_NUM;
			
			for(j=0; j<CHANNNEL_NUM; j++)
			{				
				if(dwMask&(0x01<<j))	//通道掩码有效
				{
					*pBuf = pTemp[j];
					pBuf++;
				}
			}
		}
	}
*/	

	SHORT	*pwSampleData;
	int 	i, j;
	
	if((dwMask==0) || (pBuf == NULL) || (dwPointNum == 0) || (pwADSampleData == NULL))
		return ERR_FAILURE;


	for(i=0; i<dwPointNum; i++)
	{
		//计算出需要的数据块起始地址
		pwSampleData = pwADSampleData+((dwPtr+i*CHANNNEL_NUM)%SAMPLE_DATA_LEN);
		
		for(j=0; j<CHANNNEL_NUM; j++, pwSampleData++)
		{
			if(dwMask&(0x01<<j))	//通道掩码有效
			{
				*pBuf = *pwSampleData;
				pBuf++;
			}
		}
	}
	
	return ERR_SUCCESS;
}
STATUS ReadHisSampData_Single(DWORD dwPtr, DWORD dwChanNo, SHORT *pBuf, DWORD dwPointNum)
{
	SHORT	*pwSampleData;
	int 	i, j;
	
	if((pBuf == NULL) || (dwPointNum == 0) || (pwADSampleData == NULL) )
		return ERR_FAILURE;


	for(i=0; i<dwPointNum; i++)
	{
		//计算出需要的数据块起始地址
		pwSampleData = pwADSampleData+((dwPtr+i*CHANNNEL_NUM)%SAMPLE_DATA_LEN)+dwChanNo;
		
		*pBuf = *pwSampleData;
		pBuf++;
	}
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     检查从原始采样缓冲区指定位置到当前保护处理到的采样数据位置之间的数据点数是否
 *				达到指定长度要求
 *
 * 参数说明      - dwStartPtr	: 输入 	原始采样数据缓冲区开始位置
 *				 - wPointNum : 输入	参与比较的通道点数
 *
 * 返回代码
 *                是否达到长度要求
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL isSampDataLenFill(DWORD dwStartPtr, DWORD wPointNum)
{
	if( ( (SAMPLE_DATA_LEN+dwRelayDataPtr-dwStartPtr)%SAMPLE_DATA_LEN )>=
		(wPointNum*CHANNNEL_NUM) )
		return TRUE;
	else
		return FALSE;
}

DWORD BackSamplePtr(DWORD dwStartPtr, DWORD wPointNum)
{	
	return  (SAMPLE_DATA_LEN+ dwStartPtr-wPointNum*CHANNNEL_NUM)%(SAMPLE_DATA_LEN);
}

/*********************************************************************************************
 *
 * 功能描述     写入合成通道数据，每个通道的点数为RELAY_SAMP_INTERVAL的定义,可以写入合同模拟数据
 *				也可写入数字录波通道数据
 *
 * 参数说明      - pBuf	: 	输入 	需要写入的数据
 *				 - dwMask : 输入	通道掩码
 *				 - isDatSampChan : 输入	是否为数字录波通道数据
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
extern BOOL tFlag1;

STATUS WriteComposeData(LONG *pBuf, DWORD dwMask, BOOL isDatSampChan)
{
	DWORD dwStartPtr;
	SHORT *pdwPtr;
	int i, j, start, end;
	DWORD dwPos;
	SHORT m_pBuf;

//	static	BOOL tLastFlag1=tFlag1;
		
	if( (pBuf == NULL) || (dwMask == 0) || (pwADSampleData == NULL))
		return ERR_FAILURE;
		
	//根据保护处理的当前位置，推算需要写入数据的位置
	dwStartPtr = (SAMPLE_DATA_LEN+dwRelayDataPtr-CHANNNEL_NUM*RELAY_SAMP_INTERVAL)%SAMPLE_DATA_LEN;

	if(isDatSampChan == TRUE)	//数字录波通道
	{
		start = AD_CHAN_NUM+A_COM_CHAN_NUM;	//数字录波通道起始序号24
		end = CHANNNEL_NUM;	//数字录波通道结束序号28
	}
	else	//模拟合成通道
	{
		start = AD_CHAN_NUM;	//模拟合成通道起始序号16
		end = AD_CHAN_NUM+A_COM_CHAN_NUM;	//模拟合成通道结束序号24
	}
	//只写入合成数据通道数据
	for( j=start; j<end; j++)
	{
		if(dwMask&(0x01<<j))
		{
			for(i=0; i<RELAY_SAMP_INTERVAL; i++)
			{
				dwPos = (dwStartPtr+i*CHANNNEL_NUM+j)%SAMPLE_DATA_LEN;
				//--------------------由LONG强制转换成SHORT------------------
				if(!isDatSampChan)	//数字录波通道
				{
				if(*pBuf > 32767)
					m_pBuf = 32767;
				else if(*pBuf < -32768)
					m_pBuf = -32768;
				else
					m_pBuf = (SHORT)(*pBuf);
				pwADSampleData[dwPos] = m_pBuf;
				}
				else
					pwADSampleData[dwPos] = (SHORT)(*pBuf);
//				pwADSampleData[dwPos] = (SHORT)(*pBuf);
				pBuf++;
			}
		}
	}
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     检查AD采样芯片是否运行正常
 *
 * 参数说明      
 *				 
 *
 * 返回代码
 *               无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CheckADStatus()
{
	static DWORD dwOldCnt;

	if(RelayTaskInitOK == TRUE)
	{
		if(dwADIntCounter == dwOldCnt)	//AD中断计数没有变化，说明AD异常
		{
			DAE_SetRunningFlags(SS_AD_ERR);
//			printf("\nAD error cnt = %d......\n", dwADIntCounter);

			//设置AD功能恢复计数初值，在采样中断中递减
			dwADResumeCnt = AD_RESUME_VAL;
		}
		else
		{
			DAE_ClearRunningFlags(SS_AD_ERR);
//			printf("\nAD OK cnt = %d......\n", dwADIntCounter);
		}
		
		dwOldCnt = dwADIntCounter;
	}
}

/*********************************************************************************************
 *
 * 功能描述     读取指定历史时刻之前的34个点的所有通道的原始采样数据
 *
 * 参数说明      - pData: 		输出 	需要读取的数据
 *				 - DataTime : 	输入	需要读取的数据的绝对时标
 *				 
 *
 * 返回代码
 *               成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
#define PARTICULAR_TIME_LIMIT		50000	//定义读取历史时刻数据的时间范围，与当前时刻比较，单位毫秒
#define SAMP_INTERVAL				625		//定义采样间隔，单位微秒
STATUS ReadParticularTimeData(LONG *pData, TAbsTime DataTime)
{
	SHORT *pwSampleData;
	DWORD dwReadPtr, dwPriWritePtr, number;
	int  ChannelNo, LineNo, offset;
	TAbsTime CurTime;
	QWORD time1, time2;
	DWORD time3;

	//锁存采样缓冲区当前写指针
	dwPriWritePtr = dwADDataWritePtr;
	pwSampleData = pwADSampleData;

	//记录当前时刻,检查指定时刻的合法性，必须比当前时刻早，且必须在缓冲区的有效范围
	ReadAbsTime(&CurTime);
	time1 = (((QWORD)CurTime.Hi)<<32) + CurTime.Lo;
	time2 = (((QWORD)DataTime.Hi)<<32) + DataTime.Lo;
	if(time2>time1)
		return ERR_FAILURE;
	
	time3 = time1 - time2;
	if(time3>PARTICULAR_TIME_LIMIT)
		return ERR_FAILURE;

	//将时间差转换为微秒，并根据采样间隔时间推算所处的缓冲区位置 x*1000/1000=x*8/5
	number = time3;
	//number *= 8;
	//number /= 5;

	number += (POINT_PER_PERIOD+2);	//注意找到指定位置后还需要前推34个点
	
	//合法性检查
	ASSERT(pData != NULL);
	if(pData == NULL)
		return ERR_FAILURE;

	//获取数据在采样缓冲区的位置
	dwReadPtr = (SAMPLE_DATA_LEN+dwPriWritePtr-number*CHANNNEL_NUM)%SAMPLE_DATA_LEN;

	for( ChannelNo=0; ChannelNo<CHANNNEL_NUM; ChannelNo++)
	{
		//if(ChanMask&(0x01<<ChannelNo))	//通道掩码有效
		{
			offset = dwReadPtr+ChannelNo;
			
			for( LineNo=0; LineNo<READ_POINT_NUM; LineNo++)
			{
				*pData++ = (LONG)pwSampleData[offset];
				offset += CHANNNEL_NUM;
				offset %= SAMPLE_DATA_LEN;
			}
		}
	}
	
	return ERR_SUCCESS;
}

#define DISP_SAMP
#ifdef DISP_SAMP

////临时测试
//显示用采样缓冲区
void DispSample();
void GetDispData(SHORT* pbuf);

SHORT DispDataBuf[DISP_DATA_LEN];
WORD DispReadPtr = 0;

void DispSample()
{
	SHORT *ptr;
	
	if(DispReadPtr == 0)
	{
		memset((void*)DispDataBuf, 0, sizeof(DispDataBuf));
		GetDispData(DispDataBuf);
//		ReportMsg("\n  原始采样数据:");
	}
	ptr = &DispDataBuf[DispReadPtr];
	ReportMsg("%6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",\
			ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],\
		ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15], ptr[16], ptr[17]);
/*
	ReportMsg("  AD1: %6d %6d %6d %6d %6d %6d %6d %6d ..",\
			ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	ReportMsg("  AD2: %6d %6d %6d %6d %6d %6d %6d %6d ..",\
			ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);
*/
	DispReadPtr += CHANNNEL_NUM;
	DispReadPtr %= DISP_DATA_LEN;
}

//取显示用原始采样数据
void GetDispData(SHORT* pbuf)
{
	DWORD dwPtr, dwWritePtr, len;
	SHORT* pRead;
	int i;

//	if(RelayTaskInitOK == FALSE)
//		return;
	
	if(pwADSampleData == NULL)
		return;
	
	dwWritePtr  = dwADDataWritePtr;
	dwPtr = dwWritePtr;
		
	if(dwPtr>=DISP_DATA_LEN)	//一块数据
	{
		dwPtr -= DISP_DATA_LEN;
		pRead = &pwADSampleData[dwPtr];
		for(i=0; i<DISP_DATA_LEN; i++)
		{
			*pbuf = *pRead;
			pbuf++;
			pRead++;
		}
	}
	else		//两块数据
	{
		dwPtr = SAMPLE_DATA_LEN + dwWritePtr - DISP_DATA_LEN;
		len = DISP_DATA_LEN - dwWritePtr;
		pRead = &pwADSampleData[dwPtr];
		for(i=0; i<len; i++)
		{
			*pbuf = *pRead;
			pbuf++;
			pRead++;
		}

		len = dwWritePtr;
		pRead = pwADSampleData;
		for(i=0; i<len; i++)
		{
			*pbuf = *pRead;
			pbuf++;
			pRead++;
		}
	}
}

#endif

