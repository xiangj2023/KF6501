/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			flexcan.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           flexcan����                                                                      
*			                                                                
* ����                                                                             
*                                                                                  
*      mcf5234��CAN��������5234ֻ֧��1·CAN.  ������16��MB�����������ͻ���գ������������Ҫ                        
*      ��MB0�������ͣ�MB1��MB14�������ա�                                                                                  
*                                                                                        
* ����                                                                             
*                                                                                  
*      	FlexCanInit			flexcanӲ����ʼ��
*      	FlexCanMBInit		MB���ܳ�ʼ��                                                              
*      	FlexCanInt0		   	MB0�жϴ�����                                                                         
*      	FlexCanInt1	    	MB1�жϴ�����                                                                        
*      	FlexCanInt14    	MB14�жϴ�����                                                                       
*      	FlexCanTxd		 	�жϷ�������з��ʹ������                                                                       
*      	FlexCanRxd   		�жϷ�������н��մ������                                                                      
*      	FlexCanErrInt   	flexcan�����жϴ������                                                                        
*      	SJA1000ToFlexCanID  ��SJA1000��ʽ��IDת��Ϊflexcan��ʽ��ID                                                                      
*      	FlexCanToSJA1000ID  ��flexcan��ʽ��IDת��ΪSJA1000��ʽ��ID                                                                    
*      	FlexCanWrite       	flexcan���ݷ���                                                                     
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/9/2    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#if(MCF5235_FLEXCAN == YES)

#include "flexcan.h"
#include "canlink.h"
#include "daefunc.h"

WORD wFlexCanPort;

static DWORD dwFlexMsg;

//ͳ����Ϣ��¼
static TFlexCanStatic FlexCanStatic;

extern DWORD dwCanLinkTaskID;

DWORD dwFlexSelfAddr = 10;//0x0a
DWORD dwFlexPeerAddr = 20;//0x14

BYTE bFlexRxdBuf[9];

extern TCanPortData *pCanPortData[MAX_CAN_PORT];
extern TCanPort CanPortAttr[MAX_CAN_PORT];

DWORD dwSelfFramCnt = 0;
	
/*********************************************************************************************
 *
 * ��������     flexcanӲ����ʼ����CPU�ṩ16��MB��������ҪMB0�������ͣ�MB1������Ե㷢�ͣ�
 *				MB14�����鲥���͡�����FlexCAN��ÿ��MBռ��һ���ж���Դ����������ж�Ҳռ��һ��
 *				�������ж���Դ�������Ҫ����4���жϴ������
 *
 * ����˵��      - port		: ���� 	can���ڲ��˿����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD FlexCanOpen(WORD port)
{
	memset((void*)&FlexCanStatic, 0, sizeof(TFlexCanStatic));

	//��ʼ��CAN0���Ź��ܷ���
	MCF_GPIO_PAR_UART &= 0xf0ff;
	MCF_GPIO_PAR_UART |= 0x0a00;
	
	FlexCanStatic.dwCanHardResetCnt++;

	wFlexCanPort = port&0xff;

	ASSERT(wFlexCanPort < MAX_CAN_PORT);
	if( wFlexCanPort >= MAX_CAN_PORT )
	{
        LogError("FlexCanMBInit",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	
	FlexCanEnb();
	FlexCanSReset();	//CANģ�������λ���Ը�λ������ؼĴ�����״̬
	FlexCanMDis();

	//����ȵ������λ��־��������˵����λ���̽���
	while((MCF_CAN_CANMCR0&MCF_CAN_CANMCR_SOFTRST) != 0);

	//����ͨ������Ϊ50k
	MCF_CAN_CANCTRL0 |= MCF_CAN_CANCTRL_CLKSRC;	//�����ڲ�����ʱ��
	FlexCanEnb();
	MCF_CAN_CANCTRL0 |= ((CAN_PRESDIV<<24)|(CAN_PSEG1<<19)|(CAN_PSEG2<<16)|\
						(CAN_RJW<<22)|CAN_PROPSEG);
	
	//���巢�����ȼ�����
	MCF_CAN_CANCTRL0 |= (CAN_LMD_FIRST<<4);

	//��ʼ��MB
	for(int i=0; i<=MAX_MB_NO; i++)
		FlexCanMBInit(i, CAN_MB_DISALBE);
	FlexCanMBInit(CAN_MB0, CAN_MB_TXD);		//MB0��������
	FlexCanMBInit(CAN_MB1, CAN_MB_RXD);		//MB1��������
	FlexCanMBInit(CAN_MB14, CAN_MB_RXD);	//MB14��������

/*
	//���ý���MB(����ΪMB1)��ʶ��ID������ģ���ַ
	MCF_CAN0_MBUFX_ID(CAN_MB0) = 0;
	MCF_CAN0_MBUFX_ID(CAN_MB0) |= (0xff<<M_MANAGER_ADDR_OFFSET);
*/	
	//����ID����Ĵ���
	//MASK0�Ĵ�����ӦMB0~MB13,������Ե��ַ����
#ifdef CAN_MASTER
	MCF_CAN_RXGMASK0 = (M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET);
#else
	//ע�⣬�˴���Ҫ���Ӷ�������ַλ��ʶ�����ж��Ƿ�Ϊ�Լ����͵����ݣ������Լ����͵����ݻ���ջ�����
	MCF_CAN_RXGMASK0 = (SLAVE_MODULE_ADDR_MASK<<SLAVE_MODULE_ADDR_OFFSET)|(M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET);
#endif

	//MASK14�Ĵ�����ӦMB14,�����鲥����
	MCF_CAN_RX14MASK0 = (SLAVE_MOD_GRP_ADD_MASK<<SLAVE_MOD_GRP_ADD_OFFSET);

	//�жϼ���,ע�����CANͨ�ţ�ÿ��MBռ��һ���ж���Դ�������Ҫ�ֱ�����жϴ���
	OS_SetVector(MCF_INTC1_LVL_CAN0_BUF0, FlexCanInt0, 0);
	OS_SetVector(MCF_INTC1_LVL_CAN0_BUF1, FlexCanInt1, 0);
	OS_SetVector(MCF_INTC1_LVL_CAN0_BUF14, FlexCanInt14, 0);
	
	//ע������жϵ���ռ��һ���ж���Դ
	OS_SetVector(MCF_INTC1_LVL_CAN0_ERR, FlexCanErrInt, 0);

	MCF_INTC1_ICRn(8) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(7);
	MCF_INTC1_ICRn(9) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(6);
	MCF_INTC1_ICRn(22) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(5);
	MCF_INTC1_ICRn(24) = MCF_INTC1_ICRn_IL(2) | MCF_INTC1_ICRn_IP(4);

	OS_IntEnable(MCF_INTC1_LVL_CAN0_BUF0);
	OS_IntEnable(MCF_INTC1_LVL_CAN0_BUF1);
	OS_IntEnable(MCF_INTC1_LVL_CAN0_BUF14);
	OS_IntEnable(MCF_INTC1_LVL_CAN0_ERR);
	
	//������������ж�
	MCF_CAN_CANCTRL0 |= MCF_CAN_CANCTRL_ERRMSK;
	
	//����CAN�ж����μĴ���������MB0/MB1/MB14������������/�����ж�
	MCF_CAN_IMASK0 = MB_INT_MASK;

	//����жϱ�־
	MCF_CAN_IFLAG0 = 0xffff;

	//����CAN�����������ܣ���ʼ����
	MCF_CAN_CANMCR0 &= ~(MCF_CAN_CANMCR_MDIS|MCF_CAN_CANMCR_FRZ|MCF_CAN_CANMCR_HALT|\
						MCF_CAN_CANMCR_SUPV);

	OS_Sleep(10);

	//���CAN�Ƿ�׼����
	if((MCF_CAN_CANMCR0&MCF_CAN_CANMCR_NOTRDY) != 0)
	{
        LogError("FlexCanMBInit",FILE_LINE,"the CAN not ready, initailize fail.");
		return ERR_FAILURE;
	}

	//��ʼ������MB1Ϊ��Ե���յ�ַ,����ģ��ͨ�ŵ�ַ
	MCF_CAN0_MBUFX_CTRL(CAN_MB1) = 0;
#ifdef CAN_MASTER
	MCF_CAN0_MBUFX_ID(CAN_MB1) = (M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET);
#else
	MCF_CAN0_MBUFX_ID(CAN_MB1) = (DB_GetDevAddress()<<SLAVE_MODULE_ADDR_OFFSET);
#endif
	MCF_CAN0_MBUFX_CTRL(CAN_MB1) = (MB_RX_EMPTY|MB_CTL_IDE_MASK);
	
	//��ʼ������MB14Ϊ��ģ�����ַ
	MCF_CAN0_MBUFX_CTRL(CAN_MB14) = 0;
	MCF_CAN0_MBUFX_ID(CAN_MB14) = (0x03<<SLAVE_MOD_GRP_ADD_OFFSET);
	MCF_CAN0_MBUFX_CTRL(CAN_MB14) |= (MB_RX_EMPTY|MB_CTL_IDE_MASK);
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     flexcan��MB���ܳ�ʼ����������Ҫ���Գ�ʼ��Ϊ���͡����ա����ù��ܣ���ʼ״̬
 *				��Ϊ�Ǽ���״̬
 *
 * ����˵��      - no		: ���� 	MB���
 *       		 - type		: ����	��Ҫ���õĹ�������
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanMBInit(BYTE no, BYTE type)
{
	ASSERT(no <= MAX_MB_NO);
	if(no>MAX_MB_NO)
	{
        LogError("FlexCanMBInit",FILE_LINE,"the MB no=%d ,and is invalide.",no);
		return;
	}

	//������MB
	if(type == CAN_MB_TXD)
	{
		ClearMDCode(no);
		MCF_CAN0_MBUFX_CTRL(no) |= MB_TX_INACTIVE;
	}
	else if(type == CAN_MB_RXD)	//������MB
	{
		ClearMDCode(no);
		MCF_CAN0_MBUFX_CTRL(no) |= MB_RX_INACTIVE;
	}
	else	//��ֹʹ��
	{
		ClearMDCode(no);
	}
	
}


/*********************************************************************************************
 *
 * ��������     flexcan��MB0�жϴ�����������MB0�������ͣ�һ���������ж�˵��������һ��
 *				��ȷ�����ݷ��͡�
 *
 * ����˵��      - arg		: ����
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanInt0(void * arg)
{
	if(MCF_CAN_IFLAG0&MCF_CAN_IFLAG_BUF0I)	//���MB�жϱ�־
	{
		//�����Ӧ�жϱ�־
		ClearFlexCanIntFlg(CAN_MB0);
		
		FlexCanStatic.dwCanIntTxdOKCtn++;
		FlexCanTxd(CAN_MB0);
	}
	else	//����ԭ���ж�
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
}

/*********************************************************************************************
 *
 * ��������     flexcan��MB1�жϴ�����������MB1������Ե����ݽ��գ�һ���������ж�˵��
 *				������һ����ȷ�����ݽ��ա�
 *
 * ����˵��      - arg		: ����
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanInt1(void * arg)
{
	if(MCF_CAN_IFLAG0&MCF_CAN_IFLAG_BUF1I)	//����ӦMB�жϱ�־�Ƿ����
	{
		//�����Ӧ�жϱ�־
		ClearFlexCanIntFlg(CAN_MB1);
		
		FlexCanStatic.dwCanIntRxdOKCtn++;
		FlexCanRxd(CAN_MB1);
	}
	else
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
}

/*********************************************************************************************
 *
 * ��������     flexcan��MB14�жϴ�����������MB14�����鲥���ݽ��գ�һ���������ж�˵��
 *				������һ����ȷ�����ݽ��ա�
 *
 * ����˵��      - arg		: ����
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanInt14(void * arg)
{
	if(MCF_CAN_IFLAG0&MCF_CAN_IFLAG_BUF14I)	//����ӦMB�жϱ�־�Ƿ����
	{
		//�����Ӧ�жϱ�־
		ClearFlexCanIntFlg(CAN_MB14);
		
		FlexCanStatic.dwCanIntRxdOKCtn++;
		FlexCanRxd(CAN_MB14);
	}
	else
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
}

/*********************************************************************************************
 *
 * ��������     �жϷ�������з��ʹ�����̣���ɺ����ְ��ķ��ʹ���һ�����������ݰ����ͽ�����
 *				��ʱ֪ͨ��·��������񡣶�����·������(���ݳ��Ȳ�����8��)��һ�����뱾���̣�
 *				˵����·�����Ѿ�������ɡ�
 *
 * ����˵��      - no		: ���� 	MB���
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanTxd(BYTE no)
{
	WORD wMBCtl;
	
	MCF_CAN0_MBUFX_CTRL(no) = MB_TX_INACTIVE;

	//������·�㷢�͹��̣�������·������̣���������·�㷢���ж�ʱ��˵����·�����Ѿ��������
	
	TCanLinkFram *pLinkTxdBuf;
	pLinkTxdBuf = &pCanPortData[wFlexCanPort]->CanLinkCmdBuf.TxdBuf;
	
	if(pLinkTxdBuf->dwStatus == CAN_BUF_BUSY)
	{
		//�����·���ͻ����������б�־�����Խ�������·�����ݷ���
		memset((void*)pLinkTxdBuf, 0 , sizeof(TCanLinkFram));
				
		//����CAN�����Ϳ��б�־��־
		CanPortAttr[wFlexCanPort].dwFlag |= CAN_PORT_TXD_IDLE;
		return;
	}

	//Ӧ�ò㱨�����ݷ��ʹ���
	TCanCellBuf *pApiTxdBuf;
	WORD len, sendnum;

	pApiTxdBuf = &pCanPortData[wFlexCanPort]->CanDevBuf.TxdBuf;
	if( pApiTxdBuf->dwStatus== CAN_BUF_BUSY )	//Ӧ�ò㷢�ͻ�����æ��˵����������û�з��ͽ�������Ҫ��������
	{
		//���ݷ�����ɣ���������״̬
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
		{
			pApiTxdBuf->ptr = 0;
			
			//����CAN�����Ϳ��б�־��־
			CanPortAttr[wFlexCanPort].dwFlag |= CAN_PORT_TXD_IDLE;
			
			pApiTxdBuf->dwStatus = CAN_DATA_FINISH;	//���ݷ��ͽ���
			return;
		}
		
		//���㷢�����ݳ���
		sendnum = pApiTxdBuf->length - pApiTxdBuf->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;

		//����Ҫ���͵������͵���ӦMB����������
		for(int i=0; i<len; i++)
			MCF_CAN0_MBUFX_BYTEX(no, i) = pApiTxdBuf->Buffer[pApiTxdBuf->ptr++];
		
		//����Ҫ���ۼ�Ӧ�ò�ְ���ţ���д���Ӧ�Ĵ���
		pApiTxdBuf->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);
	
		//д�����ݳ���
		wMBCtl = (len<<MB_CLT_LEN_OFFSET);
		
		wMBCtl |= MB_CTL_IDE_MASK;	//��չ��ʽ
		wMBCtl |= MB_TX_SEND_DATA;		//��������
		wMBCtl |= MB_CTL_SRR_MASK; //��������֡��־

		//��鱾�η����Ƿ�Ϊ���һ���ְ�
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
		{
			pApiTxdBuf->dwCanID |= (0x01<<SUB_FRAM_END_OFFSET);
		}
		MCF_CAN0_MBUFX_ID(no) = pApiTxdBuf->dwCanID;
		
		MCF_CAN0_MBUFX_CTRL(no) =  wMBCtl;	//��������

		return;
	}

	//�������������CAN�����Ϳ��б�־��־
	CanPortAttr[wFlexCanPort].dwFlag |= CAN_PORT_TXD_IDLE;
}

/*********************************************************************************************
 *
 * ��������     �жϷ�������н��մ�����̡�����Ӧ�ò�ְ�����ż��ÿ���ְ�����Ƿ���ȷ����
 *				�յ����һ���ְ���֪ͨ��·������д���������·�����һ�����뱾������˵��
 *				�Ѿ��յ�һ����������·���ġ�
 *
 * ����˵��      - no		: ���� 	MB���
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanRxd(BYTE no)
{
	WORD len,  type, index;
	DWORD id;
	int i;
	char data;
	static int reg;
	
	//���ȹرս���MB����,��ֹ�����ݽ���
	ClearMDCode(no);

	reg = MCF_CAN_CANCTRL0;
	
	id = MCF_CAN0_MBUFX_ID(no);		//ȡ����ID

	if(id&(M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET))
		dwSelfFramCnt++;
	
	len = (MCF_CAN0_MBUFX_CTRL(no)&0x000f)>>MB_CLT_LEN_OFFSET;	//ȡ���ݳ���
	len %= (CAN_LINK_BUF_LEN+1);
	type = (id>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK;			//��鱨������
	
	//���ݱ��Ľ�����Ӧ����
	if(type&LINK_FRAM_FLAG)	//��·�������
	{
		TCanLinkFram *pLinkRxd;

		pLinkRxd = &pCanPortData[wFlexCanPort]->CanLinkCmdBuf.RxdBuf;
		
		//�����ջ������Ƿ����
		if(pLinkRxd->dwStatus != CAN_BUF_EMPTY)
		{
			//���չ���
			pLinkRxd->dwFlag |= RXD_BUF_OVERRUN;
			FlexCanStatic.dwCanRxdOverRunCnt++;
//			pCanPortData[wFlexCanPort]->CanStatic.dwLinkRxdOverRun++;
		}
		
		//������·����
		for(i=0; i<len; i++)
			pLinkRxd->buffer[i] = MCF_CAN0_MBUFX_BYTEX(no, i);

		pLinkRxd->dwStatus = CAN_DATA_FINISH;	//����������ɣ���·����Դ���

		//֪ͨ��·������,�յ���·�㱨��
		OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wFlexCanPort, 0, 0), 0, 0, 0);
	}
	else	//Ӧ�ò㱨�Ĵ���
	{
		TCanCellBuf *pApiRxd;

		pApiRxd = &pCanPortData[wFlexCanPort]->CanDevBuf.RxdBuf;
		
		pApiRxd->dwCanID = id;

		//���ְ�����Ƿ���ȷ
		index = (id>>SUB_FRAM_NO_OFFSET)&SUB_FRAM_NO_MASK;
		if((index -(pApiRxd->ptr+CAN_LINK_BUF_LEN-1)/CAN_LINK_BUF_LEN) == 1)
		{
			//��������Ƿ���ȷ�����յ����������ȷŵ����ջ��������������·����ͳһ��鴦��
			//��һ���ְ���Ҫ��������
			if(index == 1)	
			{
				if(pApiRxd->dwStatus != CAN_BUF_EMPTY)//�����ջ������Ƿ��
				{
					pApiRxd->dwFlag |= RXD_BUF_OVERRUN;
//					pCanPortData[wFlexCanPort]->CanStatic.dwApiRxdOverRun++;
				}
						
				//����ԭ���ǲ��������ݣ�ֱ�Ӹ���ԭ��������
				pApiRxd->length = 0;
				pApiRxd->ptr = 0;
			}	
			
			//��������
			pApiRxd->dwStatus = CAN_BUF_BUSY;
			for(i=0; i<len; i++)
				pApiRxd->Buffer[pApiRxd->ptr++] = MCF_CAN0_MBUFX_BYTEX(no, i);
			pApiRxd->length += len;
			
			pApiRxd->length %= CAN_FRAME_BUF_LEN;
			pApiRxd->ptr %= CAN_FRAME_BUF_LEN;
			
		}
		else
		{
			for(i=0; i<len; i++)
				data = MCF_CAN0_MBUFX_BYTEX(no, i);
			pApiRxd->dwFlag |= SUB_FRAM_INDEX_ERR;
//			pCanPortData[wFlexCanPort]->CanStatic.dwApiRxdErrCnt++;
		}
		
		//����ǲ������һ���ְ�
		if(id & (SUB_FRAM_END_MASK<<SUB_FRAM_END_OFFSET))
		{
			pApiRxd->dwStatus = CAN_DATA_FINISH;	//����������ɣ���·����Դ���
			
			//���һ���ְ�������ȷ��֪ͨ��·��������п϶�ȷ��
			OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wFlexCanPort, 0, 0), 0, 0, 0);
		}
	}
	
	MCF_CAN0_MBUFX_CTRL(no) = (MB_RX_EMPTY|MB_CTL_IDE_MASK);
}

/*********************************************************************************************
 *
 * ��������     flexcan�����жϴ������
 *
 * ����˵��      - arg		: ����
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void FlexCanErrInt(void * arg)
{
	WORD status;
	
	//��ȡ����״̬�Ĵ������������ȱ��棬��Ϊ��������������еĲ���λ��Ϣ
	status = MCF_CAN_ERRSTAT0;
	
	if(status & MCF_CAN_ERRSTAT_ERRINT)	//�����ж�
	{
		FlexCanStatic.dwCanIntErrCnt++;
		
		//���ڴ����־�ж��ڷ��ͽ�������û����ȷ���֣�������ͳ�ƽ��ա����ʹ����ж�
		
	}
	else	//����ԭ��
	{
		FlexCanStatic.dwUnknowIntCnt++;
	}
	
	MCF_CAN_ERRSTAT0 = 0xffff;	//��������жϱ�־
}

/*********************************************************************************************
 *
 * ��������     flexcan���ݷ��ͣ�����Ӧ�ò����ݰ�����·������������ᴥ��һ�η����ж�
 *
 * ����˵��      - pBuf			: ���� 	�������ݽṹָ��
 *       		 - type			: ����	��������
 *       		 - isNewWrite	: ����	�Ƿ�Ϊ�ط�
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD FlexCanWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite)
{
	DWORD id;
	WORD len, wMBCtl;
	WORD sendnum;

	MCF_CAN0_MBUFX_CTRL(CAN_MB0) = MB_TX_INACTIVE;
	
	if(type == CAN_LINK_FRAME)	//��·������
	{
		TCanLinkFram *pLinkTxd;
		
		pLinkTxd = &pCanPortData[port]->CanLinkCmdBuf.TxdBuf;
		if(pLinkTxd->dwStatus != CAN_DATA_READY)
		{
			LogError("FlexCanWrite",FILE_LINE,"the txd link buffer's status error.");
			return ERR_FAILURE;
		}

		if(pLinkTxd->length>CAN_LINK_BUF_LEN)
		{
			LogError("FlexCanWrite",FILE_LINE,"the txd link buffer length error.");
			return ERR_FAILURE;
		}
			
		//����Ӳ��ͨ������æ��־
		CanPortAttr[port].dwFlag &= (~CAN_PORT_TXD_IDLE);
		
		//�ְ����Ϊ1
		pLinkTxd->dwCanID &= ~(SUB_FRAM_NO_MASK<<SUB_FRAM_NO_OFFSET);
		pLinkTxd->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);

		//д�뷢��FIFO
		for(int i=0; i<pLinkTxd->length; i++)
			MCF_CAN0_MBUFX_BYTEX(CAN_MB0, i) = pLinkTxd->buffer[i];
		
		len = pLinkTxd->length;

		//д��ID
/*		
#ifdef CAN_MASTER		
		pLinkTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
	pLinkTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		MCF_CAN0_MBUFX_ID(CAN_MB0) = pLinkTxd->dwCanID;

		pLinkTxd->dwStatus = CAN_BUF_BUSY;	//���ݷ��ͽ���
		
		//д�����ݳ���
		wMBCtl = len<<MB_CLT_LEN_OFFSET;
		
		wMBCtl |= MB_CTL_IDE_MASK;	//��չ��ʽ
		wMBCtl |= MB_TX_SEND_DATA; 	//��������
		wMBCtl |= MB_CTL_SRR_MASK; //��������֡��־

		MCF_CAN0_MBUFX_CTRL(CAN_MB0) =  wMBCtl; //��������
	}
	else if(type == CAN_API_FRAME)	//Ӧ�ò�����
	{
		TCanCellBuf *pApiTxd;
		
		pApiTxd = &pCanPortData[port]->CanDevBuf.TxdBuf;
		
		//ֻ�д���׼��״̬�ķ������ݲ��ܿ�ʼ���ͣ�����˵�������ݽ��뷢��״̬
		if(pApiTxd->dwStatus != CAN_DATA_READY)
		{
			LogError("FlexCanWrite",FILE_LINE,"the txd api buffer's status error.");
			return ERR_FAILURE;
		}
		
		//����CAN������æ��־
		CanPortAttr[port].dwFlag &= (~CAN_PORT_TXD_IDLE);

		if(isNewWrite == FALSE)
		{
			pCanPortData[port]->CanStatic.dwApiResendCnt++;
			pApiTxd->ptr = 0;
		}

		//������һ���ְ���־
		pApiTxd->dwCanID &= ~(0x01<<SUB_FRAM_END_OFFSET);

		//��һ�η��ͣ���һ���ְ����Ϊ1
		pApiTxd->dwCanID &= ~(SUB_FRAM_NO_MASK<<SUB_FRAM_NO_OFFSET);
		pApiTxd->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);

		//���㷢�����ݳ���
		sendnum = pApiTxd->length - pApiTxd->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;
		
		//����Ҫ���͵������͵���ӦMB����������
		for(int i=0; i<len; i++)
		{
			MCF_CAN0_MBUFX_BYTEX(CAN_MB0, i) = pApiTxd->Buffer[pApiTxd->ptr++];
		}
/*		
#ifdef CAN_MASTER		
		pApiTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
		pApiTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		if(pApiTxd->length<=CAN_LINK_BUF_LEN)
			pApiTxd->dwCanID |= (0x1<<SUB_FRAM_END_OFFSET);
		
		MCF_CAN0_MBUFX_ID(CAN_MB0) = pApiTxd->dwCanID;
		
		//д�����ݳ���
		wMBCtl = len<<MB_CLT_LEN_OFFSET;
		
		wMBCtl |= MB_CTL_IDE_MASK;	//��չ��ʽ
		wMBCtl |= MB_TX_SEND_DATA; 	//��������
		wMBCtl |= MB_CTL_SRR_MASK; //��������֡��־

		pApiTxd->dwStatus = CAN_BUF_BUSY;
		
		MCF_CAN0_MBUFX_CTRL(CAN_MB0) =  wMBCtl; //��������
		
	}
	return ERR_SUCCESS;
}

DWORD FlexCanReset(WORD port)
{
	DWORD reg;
//	ReportMsg("FlexCanReset........");
	
	//��λCANģ��
	FlexCanSReset();
	FlexCanStatic.dwCanHardResetCnt++;

	OS_Sleep(10);
	FlexCanEnb();
	MCF_CAN_IMASK0 = MB_INT_MASK;
	MCF_CAN_IFLAG0 = 0xffff;
	
	//����CAN�����������ܣ���ʼ����
	MCF_CAN_CANMCR0 &= ~(MCF_CAN_CANMCR_MDIS|MCF_CAN_CANMCR_FRZ|MCF_CAN_CANMCR_HALT|\
						MCF_CAN_CANMCR_SUPV);

	memset(&pCanPortData[wFlexCanPort]->CanDevBuf, 0, sizeof(TCanDevBuf));
	memset(&pCanPortData[wFlexCanPort]->CanLinkCmdBuf, 0, sizeof(TCanLinkCmdBuf));
	
	CanPortAttr[wFlexCanPort].dwFlag = (CAN_PORT_OPEN|CAN_PORT_TXD_IDLE|CAN_PORT_RXD_IDLE);
	
	return ERR_SUCCESS;
}


#endif
