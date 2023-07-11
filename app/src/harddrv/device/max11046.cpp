/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			max11046.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           max11046ADоƬ��������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      	max11046ADоƬ��ʼ���������������ơ������жϴ���ԭʼ���ݶ�ȡ�ӿ�                 
*                                                                                        
*                                                                                  
* ����                                                                             
*		   	                                                                  
*      	ADCInit			AD��������Ӳ����ʼ��
*		ADSampleInt		AD�����жϴ��� 	                                                                  
*		ReadSampleData	��ȡĳ����ͨ����ָ���ܲ��ŵ�����34���������   	                                                                  
*		ReadPreSingleData	��ȡĳ����ͨ�����ɲ������ָ���Ĳ���������   	                                                                  
*		   	                                                                  
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/12/18    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "max11046.h"
#include "daefunc.h"

extern DWORD dwRelayTaskID;
extern BOOL RelayTaskInitOK;

SHORT *pwADSampleData = NULL;	//ԭʼ�������ݻ�����
DWORD dwADDataWritePtr;	//�������������ݶ�дָ��
DWORD dwRelayDataPtr;	//������������ָ�룬��ֹ������дԭʼ�������ݻ�¼���������
//DWORD dwOldDataPtr;

DWORD dwADIntCounter = 1;

//DWORD dwADMsgCnt = 0;
DWORD dwADResumeCnt = 0;	//AD���ָܻ����������쳣���������豣֤��������һ�������ܲ�����
DWORD dwSampleErrCnt = 0;
WORD wSampleErrFlag;

//���ڹʲ��6571������Ҫȫ��16·ģ��������ݣ������Ҫ�޳���Чͨ������
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
 * ��������     AD��������Ӳ����ʼ��������ADоƬģʽ���ã��жϼ��أ������������ų�ʼ����
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void ADCInit()
{
	//��ʼ�����������������ݲ���ָ��
	pwADSampleData = (SHORT *)OS_Malloc(SAMPLE_DATA_LEN*sizeof(WORD));
	if(pwADSampleData == NULL)
	{
		LogError("ADCInit",FILE_LINE,"malloc error!");
		return;
	}
	memset(pwADSampleData, 0, SAMPLE_DATA_LEN*sizeof(WORD));

	dwADDataWritePtr = 0;

	//��ʱ�����ų�ʼ��
	MCF_GPIO_PAR_TIMER = 0x0000;	//�����ж�ʱ����������ΪI/O��ʽ
	MCF_GPIO_PDDR_TIMER = 0xff;		//��ʼ����ʱ�����Ŷ�Ϊ�������
	MCF_GPIO_PODR_TIMER = 0x0c;	//��ʼ����ʱ�����ų�ʼ״̬Ϊ��,����ң��

	outport(ADC1_CS_BASE, 0x0004);	//����ADC����ģʽ:�ⲿ�ο���CONVSTģʽ
	outport(ADC2_CS_BASE, 0x0004);

	//ADCΪIRQ6
	OS_SetVector(MCF_INTC0_LVL_LVL6, ADSampleInt, 0);
	OS_IntEnable(MCF_INTC0_LVL_LVL6);

	//�����½����ж�
#if(AD_DEV_TYPE == AD7606_DEV)
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_EPPAx_LEVEL);	//���õ͵�ƽ����
#else
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_EPPAx_FALLING);//�����½��ش����ж�
#endif
	//�ж�����
	MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE6;

	//����Ϊ�����ʽ
	MCF_GPIO_PDDR_TIMER |= (AD1_START_PIN|AD2_START_PIN);

	//���ó�ʼ״̬Ϊ��
	ADStartPinLow();
}

/*********************************************************************************************
 *
 * ��������     AD�����жϴ����������жϣ����ݶ�ȡ���Լ�֪ͨ��������ȣ����ڲ�ͬ��ADоƬ
 *				����ͨ��·����ͬ����˶�ȡ��ʽ����һ�����졣
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void ADSampleInt(void * arg)
{
	static BYTE bADSampleCnt;
	int i;
	BYTE data;
	
	//����ж�
	MCF_EPORT_EPFR |= 0x40;

	bADSampleCnt++;

	//ˢ��AD�жϼ�����������ADоƬ����״̬Ϊ������
	dwADIntCounter++;

	//AD���ָܻ������ݼ�
	if(dwADResumeCnt)
		dwADResumeCnt--;

	
#if(AD_DEV_TYPE == AD7606_DEV)
	//WORD wFlag;
	//���AD���������쳣��־
	wSampleErrFlag = inport(0xa20300c0);
	if(wSampleErrFlag != 0xaaaa)
		dwSampleErrCnt++;

	BYTE *pBuf = (BYTE*)&pwADSampleData[dwADDataWritePtr];

	for(i=0; i<AD7606_CHAN_NUM*2; i++)
	{
		data = inportb(AD7606_BASE+i);

		//ֻ����Чͨ�������ݱ��浽����������
		if(ChanValFlag[i/2] == TRUE)
		{
			*pBuf = data;
			pBuf++;
		}
	}

	dwADDataWritePtr += AD_CHAN_NUM;

#else
	//ȡ��һƬAD��ԭʼ��������	
	for(i=0; i<CHANNEL_PER_AD; i++)
	{
		pwADSampleData[dwADDataWritePtr] =inport(ADC1_CS_BASE);// MAKEWORD((BYTE)dwADIntCounter, (BYTE)i);//
		dwADDataWritePtr++;
	}

	//ȡ�ڶ�ƬAD��ԭʼ��������	
	for(i=0; i<CHANNEL_PER_AD; i++)
	{
		pwADSampleData[dwADDataWritePtr] =inport(ADC2_CS_BASE);//MAKEWORD((BYTE)dwADIntCounter, (BYTE)(i+8));
		dwADDataWritePtr++;
	}

#if(AD_DEV_TYPE == AD7656_DEV)
	//ȡ����ƬAD��ԭʼ��������,�������һƬ���2·��Ч	
	for(i=0; i<(CHANNEL_PER_AD-2); i++)
	{
		pwADSampleData[dwADDataWritePtr] = inport(ADC3_CS_BASE);//MAKEWORD((BYTE)dwADIntCounter, (BYTE)(i+16));//
		dwADDataWritePtr++;
	}
#endif
#endif
	dwADDataWritePtr += COMP_CHAN_NUM;
	dwADDataWritePtr %= SAMPLE_DATA_LEN;

	//���ӷ�����Ϣ��֪ͨ��������,ע����AD���ָܻ��󣬱��뱣֤���һ�ܲ���������֪ͨ��������
	if((bADSampleCnt%RELAY_SAMP_INTERVAL) || dwADResumeCnt)
	{
		return;
	}
	bADSampleCnt = 0;
	
	//��������ʱ�Է�����Ϣǰ�����дָ��Ϊ׼.
	dwRelayDataPtr = dwADDataWritePtr;
	
	//ֻ�еȵ����������ʼ����ɺ���ܽ��в�������
	if(RelayTaskInitOK == FALSE)
		return;
	
	OS_PostMessage(dwRelayTaskID, SM_RELAY_INT, EV_RELAYBIT, 0, 0, 0);
}

/*********************************************************************************************
 *
 * ��������     ��ԭʼ�����������ж�ȡĳ����ͨ����ָ���ܲ��ŵ�����34��������ݣ����ݰ���ͨ��
 *				����˳��
 *
 * ����˵��      - pData	: ��� 	��Ҫ��������ݻ�����
 *				 - ChanMask : ����	ͨ������
 *				 - CycleNo	: ����	�ܲ��ţ���ǰ�ܲ�Ϊ0����һ�ܲ�Ϊ1����������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
STATUS ReadSampleData(LONG *pData, DWORD ChanMask, BYTE CycleNo)
{
	SHORT *pwSampleData;
	DWORD dwReadPtr, dwPriWritePtr;
	BYTE channel;
	int  ChannelNo, LineNo, offset;

	//�Ϸ��Լ��
	ASSERT(pData != NULL);
	ASSERT(ChanMask != 0);
	if((pData == NULL) || (ChanMask == 0) || (pwADSampleData == NULL))
		return ERR_FAILURE;

	dwPriWritePtr = dwRelayDataPtr;
	pwSampleData = pwADSampleData;

	dwReadPtr = (SAMPLE_DATA_LEN+dwPriWritePtr-(CycleNo+1)*SAM_LEN_PER_CYC-2*CHANNNEL_NUM)%SAMPLE_DATA_LEN;

	for( ChannelNo=0; ChannelNo<CHANNNEL_NUM; ChannelNo++)
	{
		if(ChanMask&(0x01<<ChannelNo))	//ͨ��������Ч
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
 * ��������     ��ԭʼ�����������ж�ȡĳ����ͨ�����ɲ������ָ���Ĳ��������ݣ�ÿ��ͨ����Ҫ����
 *				3���ܲ���ͬ��λ���ݣ����ݰ���ͨ���Ӿɵ��µ�˳�򱣴档
 *
 * ����˵��      - pData	: ��� 	��Ҫ��������ݻ�����
 *				 - pChannel : ����	ͨ��·��ָ��
 *				 - Number	: ����	ͨ������
 *				 - Interval	: ����	���������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
STATUS ReadPreSingleData(LONG *pData, BYTE *pChannel, BYTE Number,BYTE Interval)
{
	SHORT *pwSampleData, *pTemp;
	DWORD dwReadPtr, dwPriWritePtr, dwTempPtr;
	BYTE channel;
	int i, j, k;
	
	//�Ϸ��Լ��
	ASSERT(pData != NULL);
	ASSERT(pChannel != NULL);
	if((pData == NULL) || (pChannel == NULL) || (Number == 0) || (pwADSampleData == NULL))
		return ERR_FAILURE;

	//��ȡԭʼ����дָ��
	dwPriWritePtr = dwRelayDataPtr;//dwADDataWritePtr;

	//���ݵ�ǰдָ���ָ���Ĳ��������������Ҫ�����ݿ��ͷλ��
	dwReadPtr = (dwPriWritePtr+SAMPLE_DATA_LEN-(READ_SAMP_CYC_NUM-1)*SAM_LEN_PER_CYC-\
				Interval*CHANNNEL_NUM)%SAMPLE_DATA_LEN;

	pwSampleData = pwADSampleData;

	for(k=0; k<Number; k++)	//���ͨ��
	{
		for(i=0; i<READ_SAMP_CYC_NUM; i++)// 3���ܲ�
		{
			//�ƶ�����һ���ܲ��ı�ͨ�����ݵĿ�ʼλ��
			dwTempPtr = dwReadPtr+pChannel[k]+i*SAM_LEN_PER_CYC; 
			dwTempPtr %= SAMPLE_DATA_LEN;

			//ȡ���������������ĵ���ͨ��������
			for(j=0; j<Interval; j++)	//����������
			{
				*pData = (LONG)pwSampleData[dwTempPtr];
				pData++;
				//�ƶ�����һ���������
				dwTempPtr += CHANNNEL_NUM;
				dwTempPtr %= SAMPLE_DATA_LEN;
			}
		}	
	}
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ��ԭʼ����������ָ��λ�ã���ȡָ��������ͨ������ʷ��������
 *				
 *
 * ����˵��      - dwPtr	: ���� 	��ʷ������ʼλ��
 *				 - ChanMask : ����	ͨ������
 *				 - pBuf		: ���	��Ҫ��������ݻ�����
 *				 - wPointNum	: ����	��Ҫ��ȡ�Ĳ�������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
	
	//��ǰдָ��ǰ��һ���������ݳ�������Ҫ��
	if((dwPtr+dwPointNum*CHANNNEL_NUM) <= SAMPLE_DATA_LEN)
	{
//		ReportMsg("Disturb Record:	%d-->%d",dwPtr,dwPtr+dwPointNum*CHANNNEL_NUM);
		
		//�������Ҫ�����ݿ���ʼ��ַ
		pwSampleData = pwADSampleData+dwPtr;

		for(i=0; i<dwPointNum; i++)
		{
			pTemp = pwSampleData+i*CHANNNEL_NUM;
			
			for(j=0; j<CHANNNEL_NUM; j++)
			{
				if(dwMask&(0x01<<j))	//ͨ��������Ч
				{
					*pBuf = pTemp[j];
					pBuf++;
					
				}		
			}
		}

	}
	else	//��ǰдָ��ǰ�����ݿ鳤�Ȳ�������Ҫ����Ҫ������ԭʼ�����ж�ȡ
	{
		//ע�����˳��Ҫ����Ҫ��ȡβ�������ݿ�
		
		//����ÿ�����ݿ��в�������
		block1 = PERIOD_NUMBER*POINT_PER_PERIOD - dwPtr/CHANNNEL_NUM;
		block2 = dwPointNum - block1;

		//ȡ��һ���������
//		ReportMsg("Disturb Record Part1:  %d-->%d",dwPtr,dwPtr+block1*CHANNNEL_NUM);

		pwSampleData = pwADSampleData+dwPtr;		//ָ���һ��ԭʼ�������ݵĿ�ʼ
		
		for(i=0; i<block1; i++)
		{
			pTemp = pwSampleData+i*CHANNNEL_NUM;
			
			for(j=0; j<CHANNNEL_NUM; j++)
			{
				if(dwMask&(0x01<<j))	//ͨ��������Ч
				{
					*pBuf = pTemp[j];
					pBuf++;
				}
			}
		}

		//ȡ�ڶ����������,��ԭʼ�������Ŀ�ʼ
//		ReportMsg("Disturb Record Part2:  0-->%d",block2*CHANNNEL_NUM);

		pwSampleData = pwADSampleData;
		
		for(i=0; i<block2; i++)
		{
			pTemp = pwSampleData+i*CHANNNEL_NUM;
			
			for(j=0; j<CHANNNEL_NUM; j++)
			{				
				if(dwMask&(0x01<<j))	//ͨ��������Ч
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
		//�������Ҫ�����ݿ���ʼ��ַ
		pwSampleData = pwADSampleData+((dwPtr+i*CHANNNEL_NUM)%SAMPLE_DATA_LEN);
		
		for(j=0; j<CHANNNEL_NUM; j++, pwSampleData++)
		{
			if(dwMask&(0x01<<j))	//ͨ��������Ч
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
		//�������Ҫ�����ݿ���ʼ��ַ
		pwSampleData = pwADSampleData+((dwPtr+i*CHANNNEL_NUM)%SAMPLE_DATA_LEN)+dwChanNo;
		
		*pBuf = *pwSampleData;
		pBuf++;
	}
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ����ԭʼ����������ָ��λ�õ���ǰ���������Ĳ�������λ��֮������ݵ����Ƿ�
 *				�ﵽָ������Ҫ��
 *
 * ����˵��      - dwStartPtr	: ���� 	ԭʼ�������ݻ�������ʼλ��
 *				 - wPointNum : ����	����Ƚϵ�ͨ������
 *
 * ���ش���
 *                �Ƿ�ﵽ����Ҫ��
 *
 * ����˵����          
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
 * ��������     д��ϳ�ͨ�����ݣ�ÿ��ͨ���ĵ���ΪRELAY_SAMP_INTERVAL�Ķ���,����д���ͬģ������
 *				Ҳ��д������¼��ͨ������
 *
 * ����˵��      - pBuf	: 	���� 	��Ҫд�������
 *				 - dwMask : ����	ͨ������
 *				 - isDatSampChan : ����	�Ƿ�Ϊ����¼��ͨ������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
		
	//���ݱ�������ĵ�ǰλ�ã�������Ҫд�����ݵ�λ��
	dwStartPtr = (SAMPLE_DATA_LEN+dwRelayDataPtr-CHANNNEL_NUM*RELAY_SAMP_INTERVAL)%SAMPLE_DATA_LEN;

	if(isDatSampChan == TRUE)	//����¼��ͨ��
	{
		start = AD_CHAN_NUM+A_COM_CHAN_NUM;	//����¼��ͨ����ʼ���24
		end = CHANNNEL_NUM;	//����¼��ͨ���������28
	}
	else	//ģ��ϳ�ͨ��
	{
		start = AD_CHAN_NUM;	//ģ��ϳ�ͨ����ʼ���16
		end = AD_CHAN_NUM+A_COM_CHAN_NUM;	//ģ��ϳ�ͨ���������24
	}
	//ֻд��ϳ�����ͨ������
	for( j=start; j<end; j++)
	{
		if(dwMask&(0x01<<j))
		{
			for(i=0; i<RELAY_SAMP_INTERVAL; i++)
			{
				dwPos = (dwStartPtr+i*CHANNNEL_NUM+j)%SAMPLE_DATA_LEN;
				//--------------------��LONGǿ��ת����SHORT------------------
				if(!isDatSampChan)	//����¼��ͨ��
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
 * ��������     ���AD����оƬ�Ƿ���������
 *
 * ����˵��      
 *				 
 *
 * ���ش���
 *               ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CheckADStatus()
{
	static DWORD dwOldCnt;

	if(RelayTaskInitOK == TRUE)
	{
		if(dwADIntCounter == dwOldCnt)	//AD�жϼ���û�б仯��˵��AD�쳣
		{
			DAE_SetRunningFlags(SS_AD_ERR);
//			printf("\nAD error cnt = %d......\n", dwADIntCounter);

			//����AD���ָܻ�������ֵ���ڲ����ж��еݼ�
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
 * ��������     ��ȡָ����ʷʱ��֮ǰ��34���������ͨ����ԭʼ��������
 *
 * ����˵��      - pData: 		��� 	��Ҫ��ȡ������
 *				 - DataTime : 	����	��Ҫ��ȡ�����ݵľ���ʱ��
 *				 
 *
 * ���ش���
 *               �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
#define PARTICULAR_TIME_LIMIT		50000	//�����ȡ��ʷʱ�����ݵ�ʱ�䷶Χ���뵱ǰʱ�̱Ƚϣ���λ����
#define SAMP_INTERVAL				625		//��������������λ΢��
STATUS ReadParticularTimeData(LONG *pData, TAbsTime DataTime)
{
	SHORT *pwSampleData;
	DWORD dwReadPtr, dwPriWritePtr, number;
	int  ChannelNo, LineNo, offset;
	TAbsTime CurTime;
	QWORD time1, time2;
	DWORD time3;

	//���������������ǰдָ��
	dwPriWritePtr = dwADDataWritePtr;
	pwSampleData = pwADSampleData;

	//��¼��ǰʱ��,���ָ��ʱ�̵ĺϷ��ԣ�����ȵ�ǰʱ���磬�ұ����ڻ���������Ч��Χ
	ReadAbsTime(&CurTime);
	time1 = (((QWORD)CurTime.Hi)<<32) + CurTime.Lo;
	time2 = (((QWORD)DataTime.Hi)<<32) + DataTime.Lo;
	if(time2>time1)
		return ERR_FAILURE;
	
	time3 = time1 - time2;
	if(time3>PARTICULAR_TIME_LIMIT)
		return ERR_FAILURE;

	//��ʱ���ת��Ϊ΢�룬�����ݲ������ʱ�����������Ļ�����λ�� x*1000/1000=x*8/5
	number = time3;
	//number *= 8;
	//number /= 5;

	number += (POINT_PER_PERIOD+2);	//ע���ҵ�ָ��λ�ú���Ҫǰ��34����
	
	//�Ϸ��Լ��
	ASSERT(pData != NULL);
	if(pData == NULL)
		return ERR_FAILURE;

	//��ȡ�����ڲ�����������λ��
	dwReadPtr = (SAMPLE_DATA_LEN+dwPriWritePtr-number*CHANNNEL_NUM)%SAMPLE_DATA_LEN;

	for( ChannelNo=0; ChannelNo<CHANNNEL_NUM; ChannelNo++)
	{
		//if(ChanMask&(0x01<<ChannelNo))	//ͨ��������Ч
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

////��ʱ����
//��ʾ�ò���������
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
//		ReportMsg("\n  ԭʼ��������:");
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

//ȡ��ʾ��ԭʼ��������
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
		
	if(dwPtr>=DISP_DATA_LEN)	//һ������
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
	else		//��������
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

