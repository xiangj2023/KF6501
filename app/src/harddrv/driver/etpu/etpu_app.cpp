/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			etpu_app.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ETPU����ģ��                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ����ϵͳӦ��Ҫ������ETPU�Ļ������ܿ⣬��ɲ�Ƶ�����¡�B��ʽ������ϵͳ��Դƫ�ͼ��ӵȹ���                         
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	EtpuInit			ETPUӦ�ó�ʼ��
*      	EtpuFreqProc		Ƶ�ʲ�������
*      	EtpuPfoInt			ϵͳ12V��Դ��ѹƫ�ͼ����жϴ���
*		EtpuTempProc		ϵͳ�¶Ȳ�������
*		EtpuIrigInt			B��ʽ�����жϴ���
*
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               09/06/9    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "etpu_app.h"
#include "led.h"
#include "etpu_cfg.h"
#include "daefunc.h"
#include "relay.h"
#include "relayfun.h"
#include "dbfunc.h"
extern int EtpuInit();

extern DWORD dwRelayTaskID;

int SysTempture;		//װ���¶�ֵ,�Ŵ�10��
TIrigBInt IrigBInfor;
DWORD dwIrigIntCnt = 0;

TFreqData FreqData[MAX_FREQ_NUM];

LONG FreqBubbleData[FREQ_DATA_BUF];


DWORD g_dwFreqPWM = 1600000;
BYTE IrigQuality = 0;	//B��ʽ�ź�����
BYTE IrigBFlag;


/*********************************************************************************************
 *
 * ��������     ETPU���ܳ�ʼ��
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��ʼ�������0�ɹ�������ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
int EtpuInit()
{
	int rtn;

	memset((void*)FreqData, 0, sizeof(TFreqData)*MAX_FREQ_NUM);
	//��������ָ����Ƶ�ʵ�����Χ
	if(FREQ_TYPE == FREQ_TYPE_PRO)
	{
		FreqData[0].dwUpLimit = P_FREQ_UPPER_LIMIT;
		FreqData[0].dwLowLimit = P_FREQ_LOWER_LIMIT;
	}
	else
	{
		FreqData[0].dwUpLimit = M_FREQ_UPPER_LIMIT;
		FreqData[0].dwLowLimit = M_FREQ_LOWER_LIMIT;
	}
	
	//�ڶ�·Ƶ�ʹ̶�Ϊͬ��ʹ�ã���Χ�̶�
	FreqData[1].dwUpLimit = M_FREQ_UPPER_LIMIT;
	FreqData[1].dwLowLimit = M_FREQ_LOWER_LIMIT;
	
	
	//B��ʽ�źŽ����жϼ���
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_IRIG1, EtpuIrigInt, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_IRIG1);
	MCF_INTC1_ICRn(27+ETPU_IRIG1) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(6);

	
	//ϵͳ��Դƫ�ͼ����жϼ���
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_PFO, EtpuPfoInt, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_PFO);
	MCF_INTC1_ICRn(27+ETPU_PFO) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(7);


	//�¶ȼ������жϷ�ʽʵ�֣�����GPIO��ʽ����Ҫ����
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_TEMP, EtpuTempProc, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_TEMP);
	MCF_INTC1_ICRn(27+ETPU_TEMP) = MCF_INTC1_ICRn_IL(1) | MCF_INTC1_ICRn_IP(5);
/*
	//����Ƶ�ʲ����ж�
	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_FREQ1, EtpuFreq1Int, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_FREQ1);
	MCF_INTC1_ICRn(27+ETPU_FREQ1) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(5);

	OS_SetVector(MCF_INTC1_INT_LVL27+ETPU_FREQ2, EtpuFreq2Int, 0);
	OS_IntEnable(MCF_INTC1_INT_LVL27+ETPU_FREQ2);
	MCF_INTC1_ICRn(27+ETPU_FREQ2) = MCF_INTC1_ICRn_IL(5) | MCF_INTC1_ICRn_IP(4);
*/	
	//��ʼ��ETPU
	rtn = (int)my_system_etpu_init();
	if(rtn)
		return rtn;

	//����ETPU
	my_system_etpu_start();

	IrigBFlag = DB_WhichIRGB();
	
	return rtn;
}

/*********************************************************************************************
 *
 * ��������     ��һ·Ƶ�ʲ����жϴ�����ͨ�����ڿ���AD������Ƶ�ʣ���Ƶ��ֵԽ��ʱ�������ϴε�
 *				��Чֵ����AD������������0.5����û�б��жϣ���ΪƵ��Ϊ0������50Hz���Ʋ���,��ʱ��Ҫ
 *				��֤Ƶ����ʾΪ0.
 *
 * ����˵��      - arg	: ���� 	
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void EtpuFreq1Int(void *arg)
{
	EtpuFreqProc(0);
	//����������Ƶ��ֵ����AD����Ƶ��
//	if(EtpuFreqProc(0) == TRUE)
//		ADStartFreqUpdate(FreqData[0].dwFreq);
//	ADStartFreqUpdate(g_dwFreqPWM);
//	ADStartFreqUpdate(g_dwFreqPWM);

}

/*********************************************************************************************
 *
 * ��������     �ڶ�·Ƶ�ʲ����жϴ�����ͨ������ͬ�ڿ��ƣ�������1����û�б��жϣ���ΪƵ��Ϊ0��
 *				Ƶ����ʾΪ0.
 *
 * ����˵��      - arg	: ���� 	
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void EtpuFreq2Int(void *arg)
{
	EtpuFreqProc(1);
}

/*********************************************************************************************
 *
 * ��������     ETPU��Ƶ��������ͨ��GPIO���빦��ʵ�֣��жϷ�ʽ����������ء����������ͨ��
 *				����֮���ʱ�䣬Ȼ�����Ƶ��ֵ
 *
 * ����˵��      - channel	: ���� 	ETPUͨ�����
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL EtpuFreqProc(BYTE channel)
{
	TFreqData *pFreq;
	DWORD value;
	DWORD counter;

	MCF_ETPU_ECnSCR(ETPU_FREQ1+channel) = 0xC0C00000;	//����ж�
	pFreq = &FreqData[channel];

	pFreq->dwIntCnt++;
	if(pFreq->dwIntCnt&0x01)
		return FALSE;
	 
	//���ݵ�ǰʱ�Ӽ�������������ʱ��
	counter = MCF_ETPU_ETB1R;	//��ȡ��׼ʱ�ӵĵ�ǰʱ�Ӽ�����24λ
	value = (0x1000000+counter-pFreq->dwOldCounter)%0x1000000;
/*
	static LONG ttcounter = 0;
	TestBuf[ttcounter] = value;
	ttcounter++;
	if(ttcounter==200)
		ttcounter = 0;

*/
	
	pFreq->dwOldCounter = counter;
	
	//������ֵ�Ƿ�Ϸ�,���Ϸ����β�����Ч������
	if( (value>pFreq->dwUpLimit) || (value<pFreq->dwLowLimit) )
	{
		pFreq->dwErrCnt++;
		return FALSE;
	}

	//������ʷ������������м��˲�����

//	value = GetSortData(value,FREQ_DATA_BUF,(LONG*)pFreq->dwPulseLen,FreqBubbleData);
//	pFreq->dwPulseLen[2] = pFreq->dwPulseLen[1];
//	pFreq->dwPulseLen[1] = pFreq->dwPulseLen[0];
//	pFreq->dwPulseLen[0] = value;

//	value = GetMidFreqValue(channel);
	if(value == 0)
		return FALSE;
	
	//���������ȣ���λ΢��,��ʽ:x/(37.5/4)��8x/75
	pFreq->dwPeriod = (value<<3)/75;

	//��������AD������PWM��Ƶ��
	pFreq->dwFreq = (300000000*10)/value;
	return TRUE;

}


DWORD GetMidFreqValue(BYTE channel)
{
	DWORD max = 0;
	DWORD min = 0x1000000;
	DWORD middle = 0;
	DWORD value;
	
	for(int i=0; i<FREQ_DATA_BUF; i++)
	{
		value = FreqData[channel].dwPulseLen[i];
		middle += value;
		if(max<value)
			max = value;
		if(min>value)
			min = value;
	}

	middle -= (max+min);
	return (middle/(FREQ_DATA_BUF-2));
}

/*********************************************************************************************
 *
 * ��������     ��ʱƵ�ʲ���ͨ�����ӣ���û��Ƶ���źų���0.5��ʱ��Ƶ����ʾΪ0���Ҷ��ڵ�һ·Ƶ��
 *				ͨ�������û��Ƶ���źţ�����50Hz����AD����Ƶ��
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *               ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FreqChanMonitor()
{
	static DWORD dwOldFreqIntCnt[MAX_FREQ_NUM];

	//ͨ������Ƶ��ͨ�����жϼ����Ƿ�仯�����ж��Ƿ���Ƶ���źţ�û����Ƶ��ֵ����
	for(int i=0; i<MAX_FREQ_NUM; i++)
	{
		if(dwOldFreqIntCnt[i] == FreqData[i].dwIntCnt)
		{
			FreqData[i].dwFreq = 0;
			FreqData[i].dwPeriod = 0;
			FreqData[i].dwOldCounter = 0;
		}
		dwOldFreqIntCnt[i] = FreqData[i].dwIntCnt;
	}

	//�����һ·Ƶ��ֵΪ0������50Hz����
//	if(FreqData[0].dwFreq == 0)
//		ADStartFreqUpdate(1600*10);
}
/*********************************************************************************************
 *
 * ��������     ����ͨ����ȡ��������Ƶ��ֵ���Ŵ�100��
 *				
 *
 * ����˵��      - channel	: ���� 	ETPUͨ�����
 *				 
 *
 * ���ش���
 *               Ƶ��ֵ
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD GetFreqValue(BYTE channel)
{
	if(channel>1)
		return 0;
	if(FreqData[channel].dwPeriod == 0)
		return 0;
	else
		return (100000000/FreqData[channel].dwPeriod);
}

/*********************************************************************************************
 *
 * ��������     ETPUϵͳ��Դƫ�ͼ����жϺ���������GPIO�����벶׽��ʽ�������½���,�������ж�
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void EtpuPfoInt(void *arg)
{
	MCF_ETPU_ECnSCR(ETPU_PFO) = 0xC0C00000;	//����ж�

	//�����ǰ״̬Ϊ�ͣ�˵����Դ��ѹƫ�ͷ���
	if(~(MCF_ETPU_ECnSCR(ETPU_PFO)&MCF_ETPU_ECnSCR_IPS))	
	{
		//��ʱͨ����Ϣ֪ͨ�������������Ӧ����.
		//OS_PostMessage(dwRelayTaskID, (WORD)SM_POWERDOWN, 0,0,0,0);
		DAE_SetRunningFlags(SS_12V_ERR);
	}
	else//��Դ����
	{
		DAE_ClearRunningFlags(SS_12V_ERR);
	}
}

/*********************************************************************************************
 *
 * ��������     ETPU���´�����������GPIO�����벶׽��ʽ����׽������أ������¶�оƬ���Ϊ��׼
 *				���������ʵ���ϰ��հ����ڲ������м��㡣
 *				�¶�оƬMAX6576�����㹫ʽΪ:ʱ��(us)/640 - 273.15
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void EtpuTempProc(void *arg)
{
	static DWORD dwOldTempCnt;
	DWORD dwCurCnt;
	int value;
	MCF_ETPU_ECnSCR(ETPU_TEMP) = 0xC0C00000; //����ж�

	dwCurCnt = MCF_ETPU_ETB1R;		//��ȡ��׼ʱ�ӵĵ�ǰʱ�Ӽ�����24λ

	//�������ϴμ�����ֵ�����������������
	if(dwCurCnt>dwOldTempCnt)
		value = dwCurCnt-dwOldTempCnt;
	else
		value = 0x1000000 + dwCurCnt - dwOldTempCnt;
	dwOldTempCnt = dwCurCnt;

	//�����¶�ֵ���Ŵ�10��,ע�ⲻ�ܲ������ֱ��ز�׽ʵ���϶���׽��������أ���˲������ǰ�����ڵ�ʱ�䳤��
	value /= 300;
	value -= 2732;
	if( (value>-1000) && (value<1000))
		SysTempture = value;
}

/*********************************************************************************************
 *
 * ��������     B��ʽʱ�ӽ����жϴ�������GPIO�����빦�ܣ���׽������ز������жϡ�
 *				ֻ���������ؼ��㸺����ĳ��ȣ����ж��������ͣ��ҵ�����Ŀ�ʼ�󣬽���ʱ����Ϣ
 *				���ڲ����µ�P0��������ʱ������һ���������ʱ����Ϣ��1���д��ϵͳʱ��.
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void EtpuIrigInt(void * arg)
{
	BYTE type = IRIG_B_ERR;
	DWORD value, dwCurCnt;
	static DWORD dwOldCnt;

	MCF_ETPU_ECnSCR(ETPU_IRIG1) = 0xC0C00000;	//����ж�

	//��ȡ���񵽵ĸ���������

	dwCurCnt = MCF_ETPU_ETB1R;		//��ȡ��׼ʱ�ӵĵ�ǰʱ�Ӽ�����24λ
	
	//����ʱ�Ӽ�����
	if(dwCurCnt>dwOldCnt)
		value = dwCurCnt - dwOldCnt;
	else
		value = 0x1000000 + dwCurCnt - dwOldCnt;
	dwOldCnt = dwCurCnt;

	//������Ҫ��ֻ�������壬����������
	if(MCF_ETPU_ECnSCR(ETPU_IRIG1)&MCF_ETPU_ECnSCR_IPS)
	{
		//��������������ж���������
		if((value>PULSE_2MS_L)&&(value<PULSE_2MS_H))
			type = IRIG_B_2MS;
		else if((value>PULSE_5MS_L)&&(value<PULSE_5MS_H))
			type = IRIG_B_5MS;
		else if((value>PULSE_8MS_L)&&(value<PULSE_8MS_H))
			type = IRIG_B_8MS;
		
		
		switch(type)
		{
			case IRIG_B_ERR:
				memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
				break;
			case IRIG_B_8MS:	// 0
			case IRIG_B_5MS:	// 1
				//���û�н������״̬������
				if(IrigBInfor.isInResolve == FALSE)
					break;
				
				//���һ����Ϣ�ֵ����һ����Ϣλ����ʱ���ʽת���ͼ�1�봦��
				//����λ��Ϣ
				if(type == IRIG_B_5MS)
					IrigBInfor.bIrigWordInf[IrigBInfor.bCurPulseWordNum] |= \
										(0x01<<IrigBInfor.bCurPulseBitNum);
				IrigBInfor.bCurPulseBitNum++;
				
				if( ( IrigBInfor.bCurPulseWordNum == (IRIG_B_WORD_NUM-1) ) && \
					( IrigBInfor.bCurPulseBitNum == (IRIG_B_BIT_NUM) ) )
				{	
					//������������һ��ʱ��ת��Ϊ���ʱ��
					IrigBToAbsTime();

					//����һ�������ʱ��+1����Ϊ��ʱ�����ϵͳʱ��
					value = IrigBInfor.OldAbsTime.Lo;
					IrigBInfor.OldAbsTime.Lo += 1000;
					if(IrigBInfor.OldAbsTime.Lo<value)
						IrigBInfor.OldAbsTime.Hi++;
					IrigBInfor.isAbsTimeOk = TRUE;
				}
				
				if(IrigBInfor.bCurPulseBitNum>IRIG_B_BIT_NUM)
				{
					//��ʼ�����״̬����־
					memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
				}
				dwIrigIntCnt++;
				break;
			case IRIG_B_2MS:	//��ʾλ
				//����Ƿ�ΪP0,ͨ�����ʱ����Ϣ�Ƿ���Ч�����ϸ�����Ϊ�����Ϣ�ֵ������Ϣλ
				//����������Ծ�����Ӧ����Ŀ�ʼλ��
				if(IrigBInfor.isAbsTimeOk == TRUE)
				{
					//����ϵͳʱ��
					WriteAbsTime(&IrigBInfor.OldAbsTime);
					
					//��ʼ�����״̬����־
					memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
//					IrigBInfor.bOldPulseType = type;
					IrigBInfor.isInResolve = FALSE;
					break;
				}
				if(IrigBInfor.bOldPulseType == IRIG_B_2MS)	//��Pr��Ϣ
				{
					IrigBInfor.isInResolve = TRUE;	//�ҵ���Ϣ��ʼ
					IrigBInfor.bCurPulseBitNum = 0;
					IrigBInfor.bCurPulseWordNum = 0;
				}
				//P0~P9
				else if((IrigBInfor.bOldPulseType == IRIG_B_8MS)|| \
						(IrigBInfor.bOldPulseType == IRIG_B_5MS) )
				{
					if(IrigBInfor.isInResolve == TRUE)
					{
						//������һ����Ϣ�ֵĿ�ʼ
						IrigBInfor.bCurPulseBitNum = 0;
						IrigBInfor.bCurPulseWordNum++;
						if(IrigBInfor.bCurPulseWordNum>IRIG_B_WORD_NUM)
						{
							//��ʼ�����״̬����־
							memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
						}
					}
				}
				else if(IrigBInfor.bOldPulseType == IRIG_B_ERR)
				{
					memset((void*)&IrigBInfor, 0, sizeof(TIrigBInt));
					IrigBInfor.isInResolve = FALSE;
				}
				dwIrigIntCnt++;
				break;
		}
		
		IrigBInfor.bOldPulseType = type;
	}
}


/*********************************************************************************************
 *
 * ��������     ����������B��ʽʱ����Ϣת��Ϊ�����1970��ĺ�����
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void IrigBToAbsTime()
{
	int t, year, day, hour, minute, second;
	DWORD i = 0;
	static int OldYear;
	BYTE PlanetNum;

	//Ϊ������KF6500ϵͳB��ʽ��׼��KF3500ϵͳB��ʽ��׼����Ҫ�������ý����������ݵķְ�����
	//����3500ϵͳ����5��6��7���ֽ������в���
	//����������

	if(IrigBFlag)	//KF3500
	{
		year = 2000 + (IrigBInfor.bIrigWordInf[IRIG_35_YEAR]&0x0f) + \
				((IrigBInfor.bIrigWordInf[IRIG_35_YEAR]>>5)&0x0f)*10;

		//������
		PlanetNum = (IrigBInfor.bIrigWordInf[IRIG_35_PLANET]&0x0f) + \
				((IrigBInfor.bIrigWordInf[IRIG_35_PLANET]>>5)&0x0f)*10;

		//��������������ʱ����Ϣ����
		if(PlanetNum>3)
			IrigQuality = 0;
		else if(PlanetNum>1)
		{
			IrigQuality = 3;
		}
		else
		{
			IrigQuality = 10;
		}
	}
	else	//KF6500
	{
		year = 2000 + (IrigBInfor.bIrigWordInf[IRIG_B_YEAR]&0x0f) + \
				((IrigBInfor.bIrigWordInf[IRIG_B_YEAR]>>5)&0x0f)*10;
		
		//ͬʱˢ��B��ʽ�ź�����
		IrigQuality = (IrigBInfor.bIrigWordInf[IRIG_B_QT]>>1)&0x0f;
	}
	
	//����������
	day = (IrigBInfor.bIrigWordInf[IRIG_B_DAY]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_DAY]>>5)&0x0f)*10+\
			(IrigBInfor.bIrigWordInf[IRIG_B_H_DAY]&0x03)*100;

	//����Сʱ����
	hour = (IrigBInfor.bIrigWordInf[IRIG_B_HOUR]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_HOUR]>>5)&0x07)*10;
	
	//�����������
	minute = (IrigBInfor.bIrigWordInf[IRIG_B_MIN]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_MIN]>>5)&0x07)*10;

	//����������
	second = (IrigBInfor.bIrigWordInf[IRIG_B_SEC]&0x0f) + \
			((IrigBInfor.bIrigWordInf[IRIG_B_SEC]>>5)&0x07)*10;
	
    for(t=1970;t<year;t++)
    {
        if((t & 0x03)==0)
            i+=LeapYearSecond;
        else
            i+=NoneLeapYearSecond;
    }

	i += (day-1)*86400;
	i += hour*3600;
	i += minute*60;
	i += second;

	//����ʱ��ת��Ϊ����ʱ��
	SecondToMSecond(i, 0, &IrigBInfor.OldAbsTime);

}


 /*********************************************************************************************
  *
  * ��������	 ���B��ʽ�ź��Ƿ�����
  *
  * ����˵��	  ��
  * 			  
  *
  * ���ش���
  * 			   ��
  *
  * ����˵����			
  *
 **********************************************************************************************/
 void isIrigOk()
 {
 	static DWORD dwPreCnt;

	//ͨ�����һ��ʱ����B��ʽ�ź���������Ƿ�仯�ж��Ƿ�����
	if(dwIrigIntCnt == dwPreCnt)	//�����ޱ仯��˵���ź��쳣	
	{
		DAE_SetRunningFlags(SS_TIME_B);
	}
	else	//����
	{
		DAE_ClearRunningFlags(SS_TIME_B);
	}
	dwPreCnt = dwIrigIntCnt;
 }

/*
BYTE GetIrigPulseType()
{
	static DWORD dwOldCnt;
	DWORD dwCurCnt, value;

	dwCurCnt = MCF_ETPU_ETB1R;		//��ȡ��׼ʱ�ӵĵ�ǰʱ�Ӽ�����24λ
	
	//����ʱ�Ӽ�����
	if(dwCurCnt>dwOldCnt)
		value = dwCurCnt - dwOldCnt;
	else
		value = 0x1000000 + dwCurCnt - dwOldCnt;
	dwOldCnt = dwCurCnt;

	//��������������ж���������
	if((value>PULSE_2MS_L)&&(value<PULSE_2MS_H))
		return IRIG_B_2MS;
	
	if((value>PULSE_5MS_L)&&(value<PULSE_5MS_H))
		return IRIG_B_5MS;
	
	if((value>PULSE_8MS_L)&&(value<PULSE_8MS_H))
		return IRIG_B_8MS;
	
	return IRIG_B_ERR;
}
*/

/*
void EtpuPWMInt(void * arg)
{
	iPwmIntCnt++;
	dwICCounter[3] = (DWORD)eTPU->CHAN[ETPU_IC].SCR.R;
	dwICCounter[4] = (DWORD)eTPU->CHAN[ETPU_PWM].SCR.R;

	// Erase interrupt flag 
	eTPU->CISR_A.R |= (0x01<<ETPU_PWM);

	if(iPwmIntCnt%2)
		outportb(LED1_CS_BASE,0xff);
	else
		outportb(LED1_CS_BASE,0x00);
}

void EtpuICInt(void * arg)
{
	iICIntCnt++;
	
	eTPU->CISR_A.R |= (0x01<<ETPU_IC);
	dwICCounter[0] = (DWORD)fs_etpu_ic_read_trans_count(ETPU_IC);
	dwICCounter[1] = (DWORD)fs_etpu_ic_read_final_time(ETPU_IC);
	dwICCounter[2] = (DWORD)fs_etpu_ic_read_last_time(ETPU_IC);
	
	if(iICIntCnt%2)
		outportb(LED2_CS_BASE,0xff);
	else
		outportb(LED2_CS_BASE,0x00);
	
}

void ReadEtpuICValue()
{
	dwICCounter[0] = (DWORD)fs_etpu_ic_read_trans_count(ETPU_TEMP);
	dwICCounter[1] = (DWORD)fs_etpu_ic_read_final_time(ETPU_TEMP);
	dwICCounter[2] = (DWORD)fs_etpu_ic_read_last_time(ETPU_TEMP);
	printf("etpu trans=%d final=%x last=%d.\n", dwICCounter[0],dwICCounter[1],dwICCounter[2]);
}
*/
/*********************************************************************************************
 *
 * �������� 	���B��ʽ�ź��Ƿ�����
 *
 * ����˵�� 	 ��
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/

