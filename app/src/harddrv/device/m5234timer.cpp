/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			m5234timer.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           coldfire5234��ʱ������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      5234 DMA��ʱ�������ӿڣ���ʱ��0��������ϵͳ���ȣ���ʱ��1����������ʱ                         
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	DITimerInit			DI��ʱ����ʼ��
*      	DITimerEnable		DI��ʱ������
*      	DITimerDisable		DI��ʱ���ر�
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/8/19    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "m5234timer.h"
#include "max11046.h"
#include "daedef.h"
#include "etpu_app.h"
#include "relay.h"

extern DWORD dwDIIntCnt;		//DI��ʱ���жϼ���
extern DWORD dwRelayTaskID;
extern DWORD dwCaluInterval;	//�ȹ����ɼ����ۼӼ��
#ifdef PRO_49I
	extern BOOL  g_bRel49Flag;		//�ȹ������ۼ���Ϣ��־
	extern BOOL  g_bRel49TimeOut;
#endif
extern TSysConfigTable	*G_pCfgTable;

extern void DITimerInt();

DWORD dwRelayMSCnt;		//�����ú��������

/*********************************************************************************************
 *
 * ��������     DI��ʱ����ʼ��
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
void DITimerInit()
{
	dwDIIntCnt = 0;//��ʼ��DI��ʱ�жϼ���

	//�жϼ���
//	OS_SetVector(MCF_INTC0_LVL_TMR1, DITimerInt, 0);
	
	DITimerDisable();
	DITimerEnable();

	OS_IntEnable(MCF_INTC0_LVL_TMR0);

	//�����ж����ȼ�
	MCF_INTC0_ICRn(20) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(5);
}

/*********************************************************************************************
 *
 * ��������     DI��ʱ���������з�Ƶ���㣬�����ж�
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
void DITimerEnable()
{
	DWORD Prescale, ClkCnt;

	//��ֹ��ʱ��
	MCF_TIMER_DTMR(DI_DMA_TIMER) = 0x00;

	//��λ��ʱ������ֵ
	MCF_TIMER_DTCN(DI_DMA_TIMER) = 0x00;

	//�����Ƶֵ
	Prescale = (IN_SYS_CLOCK / DI_INT_TIMER_RATE) >> 16;
	ClkCnt = ((IN_SYS_CLOCK / (Prescale + 1))
		       / DI_INT_TIMER_RATE);

	//���òο��Ĵ���
	MCF_TIMER_DTRR(DI_DMA_TIMER) = ClkCnt;

	//������ʱ��
	MCF_TIMER_DTMR(DI_DMA_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
			    	| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST \
			    	| (Prescale << 8));
	//���DMA��ʱ���ж�����λ
	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK20;
//	CLR_DMA_IMR_BIT(&DIIntTimer);
}

/*********************************************************************************************
 *
 * ��������     DI��ʱ���ر�
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
void DITimerDisable()
{
	//��DMA��ʱ���ж�����λ
	MCF_INTC0_IMRL |= MCF_INTC0_IMRL_INT_MASK20;

	MCF_TIMER_DTMR(DI_DMA_TIMER) = 0x00;
}

void RelayTimeInit()
{
	dwRelayMSCnt = 0;
	
//	MCF_GPIO_PDDR_TIMER = 0xff;

	//��ֹ��ʱ��
	MCF_TIMER_DTMR(RELAY_MS_TIMER) = 0x00;
	
	//��λ��ʱ������ֵ
	MCF_TIMER_DTCN(RELAY_MS_TIMER) = 0x00;
	
	//���òο��Ĵ���
	TimeSetDTRR(RELAY_MS_TIMER, RELAY_MS_RATE);
	
	//���DMA��ʱ���ж�����λ
//	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK21;
	
	//����ж�״̬
	MCF_TIMER_DTER(RELAY_MS_TIMER) = MCF_TIMER_DTER_REF;

	//�жϼ���
	OS_SetVector(MCF_INTC0_LVL_TMR2, RelayTimeInt, 0);
	OS_IntEnable(MCF_INTC0_LVL_TMR2);

	//�����ж����ȼ�
	MCF_INTC0_ICRn(21) = MCF_INTC0_ICRn_IL(2) | MCF_INTC0_ICRn_IP(5);

	//������ʱ��,��ֹ�����ж�
	MCF_TIMER_DTMR(RELAY_MS_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
					| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST);
}

//extern HANDLE			g_hWdogTaskID;

//������ʱ�ж�
void RelayTimeInt(void * arg)
{
	static DWORD  counter;
	
	//ע���������ж�״̬
	MCF_TIMER_DTER(RELAY_MS_TIMER) = MCF_TIMER_DTER_REF;

	counter++;

	if( (counter&0x01) == 0 )
	{
		//�����ú���������ۼ�
		dwRelayMSCnt++;

		//����ϵͳʱ���������ۼ�ˢ��
		if(G_pCfgTable != NULL)
		{
			G_pCfgTable->SystemStatus.stTime.Lo++;
			if(G_pCfgTable->SystemStatus.stTime.Lo == 0)
				G_pCfgTable->SystemStatus.stTime.Hi++;
		}
	}

	//ÿ5������һ��AD�ж��Ƿ�����
	if((counter%10) == 0)
	{
		CheckADStatus();
	}	

#ifdef PRO_49I
	if(g_bRel49Flag == TRUE)
	{
		if(dwCaluInterval != 0)
		{
			if((counter%dwCaluInterval) == 0)
				g_bRel49TimeOut = TRUE;
		}
		else
		{
			g_bRel49TimeOut = FALSE;
		}
	}
#endif
	//ң�Ų����жϴ�����ʱ���ڱ������붨ʱ�ж���ʵ��
	DITimerInt();

 	// ÿ5���뷢��һ�ξ���λ��Ϣ
	//if((counter%40) == 0)
	//	OS_PostMessage(g_hWdogTaskID, SM_RELAYRTMB, 4, 0, 0, 0);
}

void ADTimeInit(void)
{

	//�жϼ���
	OS_SetVector(MCF_INTC0_LVL_TMR3, ADTimeInt, 0);

	//��ֹ��ʱ��
	MCF_TIMER_DTMR(AD_START_TIMER) = 0x00;
	
	//��λ��ʱ������ֵ
	MCF_TIMER_DTCN(AD_START_TIMER) = 0x00;
	
	//ע���������ж�״̬
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF;

	//���òο��Ĵ���
	TimeSetDTRR(AD_START_TIMER, AD_TIME_RATE);
	
	//������ʱ��
	MCF_TIMER_DTMR(AD_START_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
					| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST);
	
	//���DMA��ʱ���ж�����λ
	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK22;
	
	OS_IntEnable(MCF_INTC0_LVL_TMR3);

	//�����ж����ȼ�
	MCF_INTC0_ICRn(22) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(6);
}

//AD����������ʱ�ж�
void ADTimeInt(void * arg)
{
	//ע���������ж�״̬
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF;
	
	ADStartSamp();
}

//intervalʱ�䵥λΪ΢��,���ݶ�ʱ������òο��Ĵ���
void TimeSetDTRR(BYTE no, DWORD interval)
{
	DWORD dwReference;

	dwReference = (IN_SYS_CLOCK/1000000)*interval - 1;

	//���òο��Ĵ���
	MCF_TIMER_DTRR(no) = dwReference;
}


#ifdef _DBUG_IRIGB_

//����B��ʽ����������峤�ȵļ�����ֵ
#define IRIG_8MS_H	(5000*8+100)
#define IRIG_8MS_L	(5000*8-100)
#define IRIG_2MS_H	(5000*2+100)
#define IRIG_2MS_L	(5000*2-100)
#define IRIG_5MS_H	(5000*5+100)
#define IRIG_5MS_L	(5000*5-100)

//����B��ʽ��������
#define IRIG_8MS		0x01
#define IRIG_5MS		0x02
#define IRIG_2MS		0x03
#define IRIG_INVALID	0x55

#define IRIGB_PULSE_H	0x55	//�������־
#define IRIGB_PULSE_L	0xaa	//�������־

BOOL bInResolve;		//�Ƿ�ʼ����B��ʽ����״̬�����Ƿ��ҵ�ʱ��ͷ(����2��8ms������)
DWORD dwCntPerInf;		//ÿ��������Ϣ���ڵ���������ۼ�ֵ���������㲶׽�������Ƿ�����
BYTE bOldPulseSts;		//�ϴ������״̬(����)
BYTE bOldPulseType;		//�ϴ���������(2��5��8����)
BYTE bOldHPulseType;	//�ϴ�����������
BOOL bIrigTimeValid;		//ʱ����Ϣ�Ƿ���Ч
TAbsTime IrigAbsTime;	//�����������ϵͳʱ��
TSysTime IrigSysTime;	//��������ϵͳʱ��
DWORD dwOldDTCR;		//�ϴζ�ʱ����׽�Ĵ���ֵ


//��ʱ��AD������ʱ��������B��ʽʱ�ӽ�������
void IrigBTimeInit()
{
	//��ʼ������
	bInResolve = FALSE;
	bIrigTimeValid = FALSE;
	dwOldDTCR = 0;
	bOldPulseType = IRIG_INVALID;
	
	//�жϼ���
	OS_SetVector(MCF_INTC0_LVL_TMR3, IrigBTimeInt, 0);

	//��ֹ��ʱ��
	MCF_TIMER_DTMR(AD_START_TIMER) = 0x00;
	
	//��λ��ʱ������ֵ
	MCF_TIMER_DTCN(AD_START_TIMER) = 0x00;
	
	//ע���������ж�״̬
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF|MCF_TIMER_DTER_CAP;

	//���òο��Ĵ���
//	TimeSetDTRR(AD_START_TIMER, AD_TIME_RATE);
	
	//������ʱ��,��׽���������������,15��Ԥ��Ƶ,ʱ��Ƶ��Ϊ75000000/15=5MHz
	MCF_TIMER_DTMR(AD_START_TIMER) = (MCF_TIMER_DTMR_ORRI | MCF_TIMER_DTMR_FRR \
			| MCF_TIMER_DTMR_CLK_DIV1 | MCF_TIMER_DTMR_RST|MCF_TIMER_DTMR_CE(3)|MCF_TIMER_DTMR_PS(15));
	
	//���DMA��ʱ���ж�����λ
	MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK22;
	
	OS_IntEnable(MCF_INTC0_LVL_TMR3);

	//�����ж����ȼ�
	MCF_INTC0_ICRn(22) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(6);
	
}

void IrigBTimeInt(void * arg)
{
	DWORD dwDTCR, dwPulseWide;
	BYTE bPulseType = IRIG_INVALID;
	//ע���������ж�״̬
	MCF_TIMER_DTER(AD_START_TIMER) = MCF_TIMER_DTER_REF;

	//��ȡ��ǰ����״̬�Ĵ���ֵ
	dwDTCR = MCF_TIMER_DTCR(AD_START_TIMER);

	//���㱾��������
	dwPulseWide = (dwDTCR > dwOldDTCR) ? (dwDTCR-dwOldDTCR) : (0xffffffff-dwOldDTCR+dwDTCR+1);

	//�жϱ�����������
	if( (dwPulseWide>IRIG_8MS_L) && (dwPulseWide<IRIG_8MS_H) )
		bPulseType = IRIG_8MS;

	if( (dwPulseWide>IRIG_5MS_L) && (dwPulseWide<IRIG_5MS_H) )
		bPulseType = IRIG_5MS;
	
	if( (dwPulseWide>IRIG_2MS_L) && (dwPulseWide<IRIG_2MS_H) )
		bPulseType = IRIG_2MS;

	
	
}

//��ȡ��ǰB��ʽ����ĵ�ƽ״̬
BOOL isIrigBPulsHigh()
{
	return TRUE;
}

#endif
