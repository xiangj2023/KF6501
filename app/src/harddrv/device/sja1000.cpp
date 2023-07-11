/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/

/********************************************************************************************
*                                                                                  
* �ļ�����          
*			sja1000.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           sja1000����                                                                      
*			                                                                
* ����                                                                             
*                                                                                  
*       sja1000�����ӿ�                       
*                                                                                        
*                                                                                        
* ����                                                                             
*                                                                                  
*      	Sja1000Open			SJA1000Ӳ����ʼ��
*      	Sja1000Int		   	SJA1000�жϴ�����                                                                         
*      	Sja1000RxdInt	    �����жϴ�����                                                                        
*      	Sja1000TxdInt    	�����жϴ�����                                                                       
*      	Sja1000Reset	 	�жϷ�������з��ʹ������                                                                       
*      	Sja1000Write   		�������ݣ����������ж�                                                                      
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/9/2    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#if(MCF5234_SJA1000 == YES)

#include "sja1000.h"
#include "canlink.h"
#include "daefunc.h"

WORD wSja1000Port;
//DWORD wMyAdd = 10;
//DWORD wPeerAdd = 20;

//static DWORD dwSjaTxdIntCnt, dwSjaRxdIntCnt, dwSjaErrorIntCnt;

//BYTE bSja1000RxdBuf[9];

static TSja100Static Sja100Static;

extern TCanPortData *pCanPortData[MAX_CAN_PORT];
extern TCanPort CanPortAttr[MAX_CAN_PORT];
extern DWORD dwCanLinkTaskID;

/*********************************************************************************************
 *
 * ��������     SJA1000Ӳ����ʼ��
 *
 * ����˵��      - port		: ���� 	can���ڲ��˿����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD Sja1000Open(WORD port)
{
	WORD address = 0;
	
	wSja1000Port = port;
	
	//��λ
	Sja1000HDReset();

	SetClkDiv();
	
	outportb(SJA_OUTCTRL, 0x1a);//����OCR����

	//����ͨ������Ϊ50k
	SetBusTime0();
	SetBusTime1();

//��ַλ��ƥ�����
#ifdef CAN_MASTER
	outportb(SJA_ACSMASK, 0x7f);	//D29
	outportb(SJA_ACSMASK+1, 0xff);	
	
	outportb(SJA_ACSMASK+2, 0x7f);	//D21~24
	outportb(SJA_ACSMASK+3, 0xff);	//D17~20
	
	address = 0x8000;
	
#else
	//˫�˲�����,ֻ��ƥ��D13~D28λ,��һ�����ý��յ�Ե��ַ���ڶ������ý����鲥��ַ
	outportb(SJA_ACSMASK, 0xf0);	//D21~24
	outportb(SJA_ACSMASK+1, 0x0f);	//D17~20

	outportb(SJA_ACSMASK+2, 0xcf);	//D25~D26
	outportb(SJA_ACSMASK+3, 0xff);
	
	address = DB_GetDevAddress();
	address <<= 4;
	address |= 0x3000;
#endif

	outportb(SJA_ACSCODE+0, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+1, (BYTE)address);
	
	outportb(SJA_ACSCODE+2, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+3, (BYTE)address);

	//SJA1000�ж�ΪIRQ3
	OS_SetVector(MCF_INTC0_LVL_LVL3, Sja1000Int, 0);
	OS_IntEnable(MCF_INTC0_LVL_LVL3);
	
	//���õ�ƽ�����ж�
	MCF_EPORT_EPPAR |= MCF_EPORT_EPPAR_EPPA3(MCF_EPORT_EPPAR_EPPAx_LEVEL);
	//���ݷ���Ϊ����
	MCF_EPORT_EPDDR &= (~MCF_EPORT_EPDDR_EPDD3);
	//�ж�����
	MCF_EPORT_EPIER |= MCF_EPORT_EPIER_EPIE3;

	//�����ж�����λ
	outportb(SJA_INTENABLE, IR_IER_RI|IR_IER_TI|IR_IER_EI|IR_IER_DOI|IR_IER_BEI);

	//��������ģʽ
	Sja1000Enable();

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     sja1000�жϴ����������ա����ͺ��쳣����
 *
 * ����˵��      
 *				 ��
 *       		 
 *       		 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void Sja1000Int(void * arg)
{
	static DWORD dwSjaErrIntCnt;
	BYTE status;

	//�����Ӧ���ⲿ�жϱ�־
	MCF_EPORT_EPFR |= MCF_EPORT_EPFR_EPF3;
	
	status = inportb(SJA_INTR);	//���ж�״̬,�Ժ�Ĵ���״̬�Զ����

	//�����ж�
	if(status & IR_IER_RI)	
	{
		Sja1000RxdInt();
		Sja100Static.dwRxdIntCnt++;
		dwSjaErrIntCnt = 0;
	}

	//�����ж�
	if(status & IR_IER_TI)
	{
		Sja1000TxdInt();
		Sja100Static.dwTxdIntCnt++;
		dwSjaErrIntCnt = 0;
	}

	//�����ж�
	if(status & IR_ERR_MASK)
	{
		Sja100Static.dwErrorIntCnt++;
		
		dwSjaErrIntCnt++;
		if(dwSjaErrIntCnt>100)
		{
			dwSjaErrIntCnt = 0;
			Sja1000Reset(wSja1000Port);
		}
	}
}

/*********************************************************************************************
 *
 * ��������     sja1000�����жϴ�������Ӧ�ò㱨�ģ���Ҫ���ְ�����Ƿ���ȷ��������·�㱨��
 *				ֱ��֪ͨ�ػ�������
 *
 * ����˵��      
 *				 ��
 *       		 
 *       		 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void Sja1000RxdInt(void)
{
	WORD len,  type, index;
	DWORD id;
	int i;

	//��ȡID
	id = MAKEDWORD(inportb(SJA_RXDID+4), inportb(SJA_RXDID+3), 
			inportb(SJA_RXDID+2), inportb(SJA_RXDID+1));
	id >>= 3;
	
	len = inportb(SJA_RXDID)&0x000f;	//ȡ���ݳ���
	len %= (CAN_LINK_BUF_LEN+1);
	type = (id>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK; 		//��鱨������
		
	//���ݱ��Ľ�����Ӧ����
	if(type&LINK_FRAM_FLAG) //��·�������
	{
		TCanLinkFram *pLinkRxd;
		pLinkRxd = &pCanPortData[wSja1000Port]->CanLinkCmdBuf.RxdBuf;
			
		//�����ջ������Ƿ����
		if(pLinkRxd->dwStatus != CAN_BUF_EMPTY)
		{
			//���չ���
			pLinkRxd->dwFlag |= RXD_BUF_OVERRUN;
		}
			
		//������·����
		for(i=0; i<len; i++)
			pLinkRxd->buffer[i] = inportb(SJA_RXDDATA+i);
	
		pLinkRxd->dwStatus = CAN_DATA_FINISH;	//����������ɣ���·����Դ���
	
		//֪ͨ��·������,�յ���·�㱨��
		OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wSja1000Port, 0, 0), 0, 0, 0);
	}
	else	//Ӧ�ò㱨�Ĵ���
	{
		TCanCellBuf *pApiRxd;

		pApiRxd = &pCanPortData[wSja1000Port]->CanDevBuf.RxdBuf;
			
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
				}
							
				//����ԭ���ǲ��������ݣ�ֱ�Ӹ���ԭ��������
				pApiRxd->length = 0;
				pApiRxd->ptr = 0;
			}	
			
			//��������
			pApiRxd->dwStatus = CAN_BUF_BUSY;
			for(i=0; i<len; i++)
				pApiRxd->Buffer[pApiRxd->ptr++] = inportb(SJA_RXDDATA+i);
			pApiRxd->length += len;
				
			pApiRxd->length %= CAN_FRAME_BUF_LEN;
			pApiRxd->ptr %= CAN_FRAME_BUF_LEN;
			
		}
		else
		{
			pApiRxd->dwFlag |= SUB_FRAM_INDEX_ERR;
		}
			
		//����ǲ������һ���ְ�
		if(id & (SUB_FRAM_END_MASK<<SUB_FRAM_END_OFFSET))
		{
			pApiRxd->dwStatus = CAN_DATA_FINISH;	//����������ɣ���·����Դ���
				
			//���һ���ְ�������ȷ��֪ͨ��·��������п϶�ȷ��
			OS_PostMessage(dwCanLinkTaskID, SM_CANTXDRXDOK, MAKEDWORD(type, wSja1000Port, 0, 0), 0, 0, 0);
		}
	}

	//�ͷŽ���BUF
	ReleaseRxdBuf();
}

/*********************************************************************************************
 *
 * ��������     sja1000�����жϴ�������Ӧ�ò㱨�ģ���Ҫ���ְ�����Ƿ���ȷ��������·�㱨��
 *				ֱ��֪ͨ�ػ�������
 *
 * ����˵��      
 *				 ��
 *       		 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void Sja1000TxdInt(void)
{
	DWORD id;
	
	//������·�㷢�͹��̣�������·������̣���������·�㷢���ж�ʱ��˵����·�����Ѿ��������
	TCanLinkFram *pLinkTxdBuf;
	pLinkTxdBuf = &pCanPortData[wSja1000Port]->CanLinkCmdBuf.TxdBuf;
	
	if(pLinkTxdBuf->dwStatus == CAN_BUF_BUSY)
	{
		//�����·���ͻ����������б�־�����Խ�������·�����ݷ���
		memset((void*)pLinkTxdBuf, 0 , sizeof(TCanLinkFram));
				
		//����CAN�����Ϳ��б�־��־
		CanPortAttr[wSja1000Port].dwFlag |= CAN_PORT_TXD_IDLE;
		return;
	}

	//Ӧ�ò㱨�����ݷ��ʹ���
	TCanCellBuf *pApiTxdBuf;
	WORD len, sendnum;

	pApiTxdBuf = &pCanPortData[wSja1000Port]->CanDevBuf.TxdBuf;
	if( pApiTxdBuf->dwStatus== CAN_BUF_BUSY )	//Ӧ�ò㷢�ͻ�����æ��˵����������û�з��ͽ�������Ҫ��������
	{
		//���ݷ�����ɣ���������״̬
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
		{
			pApiTxdBuf->ptr = 0;
			
			//����CAN�����Ϳ��б�־��־
			CanPortAttr[wSja1000Port].dwFlag |= CAN_PORT_TXD_IDLE;
			
			pApiTxdBuf->dwStatus = CAN_DATA_FINISH;	//���ݷ��ͽ���
			return;
		}
		
		//���㷢�����ݳ���
		sendnum = pApiTxdBuf->length - pApiTxdBuf->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;

		//����Ҫ���͵������͵���ӦMB����������
		for(int i=0; i<len; i++)
			outportb(SJA_TXDDATA+i, pApiTxdBuf->Buffer[pApiTxdBuf->ptr++]);
		
		//����Ҫ���ۼ�Ӧ�ò�ְ���ţ���д���Ӧ�Ĵ���
		pApiTxdBuf->dwCanID += (0x01<<SUB_FRAM_NO_OFFSET);

		//��鱾�η����Ƿ�Ϊ���һ���ְ�
		if(pApiTxdBuf->ptr == pApiTxdBuf->length)
			pApiTxdBuf->dwCanID |= (0x01<<SUB_FRAM_END_OFFSET);
	
		//д�����ݳ��ȼ�FF��ʾ
		outportb(SJA_TXDID, len|EFF_FF);

		//д��ID
		id = pApiTxdBuf->dwCanID;
		id <<= 3;
		outportb(SJA_TXDID+1, (BYTE)(id>>24));
		outportb(SJA_TXDID+2, (BYTE)(id>>16));
		outportb(SJA_TXDID+3, (BYTE)(id>>8));
		outportb(SJA_TXDID+4, (BYTE)id);

		//������������
		SendRequest();
		return;
	}

	//�������������CAN�����Ϳ��б�־��־
	CanPortAttr[wSja1000Port].dwFlag |= CAN_PORT_TXD_IDLE;
	
}

DWORD Sja1000Reset(WORD port)
{
	WORD address;

//	ReportMsg("SJA1000 hardware reset....");
	Sja1000HDReset();

	SetClkDiv();
	
	outportb(SJA_OUTCTRL, 0x1a);//����OCR����

	//����ͨ������Ϊ50k
	SetBusTime0();
	SetBusTime1();

	//��ַλ��ƥ�����
#ifdef CAN_MASTER
	outportb(SJA_ACSMASK, 0x7f);	//D29
	outportb(SJA_ACSMASK+1, 0xff);	
		
	outportb(SJA_ACSMASK+2, 0x7f);	//D21~24
	outportb(SJA_ACSMASK+3, 0xff);	//D17~20
		
	address = 0x8000;
		
#else
	//˫�˲�����,ֻ��ƥ��D13~D28λ,��һ�����ý��յ�Ե��ַ���ڶ������ý����鲥��ַ
	outportb(SJA_ACSMASK, 0xf0);	//D21~24
	outportb(SJA_ACSMASK+1, 0x0f);	//D17~20
	
	outportb(SJA_ACSMASK+2, 0xcf);	//D25~D26
	outportb(SJA_ACSMASK+3, 0xff);
		
	address = DB_GetDevAddress();
	address <<= 4;
	address |= 0x3000;
#endif
	
	outportb(SJA_ACSCODE+0, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+1, (BYTE)address);
		
	outportb(SJA_ACSCODE+2, (BYTE)(address>>8));
	outportb(SJA_ACSCODE+3, (BYTE)address);

	//�����ж�����λ
	outportb(SJA_INTENABLE, IR_IER_RI|IR_IER_TI|IR_IER_EI|IR_IER_DOI|IR_IER_BEI);

	//��������ģʽ
	Sja1000Enable();

	memset(&pCanPortData[wSja1000Port]->CanDevBuf, 0, sizeof(TCanDevBuf));
	memset(&pCanPortData[wSja1000Port]->CanLinkCmdBuf, 0, sizeof(TCanLinkCmdBuf));
	
	CanPortAttr[wSja1000Port].dwFlag = (CAN_PORT_OPEN|CAN_PORT_TXD_IDLE|CAN_PORT_RXD_IDLE);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     sja1000���ݷ��ͣ�����Ӧ�ò����ݰ�����·������������ᴥ��һ�η����ж�
 *
 * ����˵��      - port			: ���� 	�������ݽṹָ��
 *				 - address		: ����  �������ݵ�ַ
 *       		 - type			: ����	��������
 *       		 - isNewWrite	: ����	�Ƿ�Ϊ�ط�
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD Sja1000Write(WORD port, BYTE address, BYTE type, BOOL isNewWrite)
{
	DWORD id;
	WORD len;
	WORD sendnum;

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
			outportb(SJA_TXDDATA+i, pLinkTxd->buffer[i]);
		
		len = pLinkTxd->length;
/*
#ifdef CAN_MASTER		
		pLinkTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
		pLinkTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		id = pLinkTxd->dwCanID;

		pLinkTxd->dwStatus = CAN_BUF_BUSY;	//���ݷ��ͽ���
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

		//ע����ЩӦ�ò㱨�Ŀ���һ���ְ��Ϳ��Է�����ɣ������Ҫ�ж��Ƿ���Ҫ���÷ְ�������־
		if(pApiTxd->length<=CAN_LINK_BUF_LEN)
			pApiTxd->dwCanID |= (0x1<<SUB_FRAM_END_OFFSET);

		//���㷢�����ݳ���
		sendnum = pApiTxd->length - pApiTxd->ptr;
		len = (sendnum >= CAN_LINK_BUF_LEN) ? CAN_LINK_BUF_LEN : sendnum;
		
		//����Ҫ���͵������͵���ӦMB����������
		for(int i=0; i<len; i++)
		{
			outportb(SJA_TXDDATA+i, pApiTxd->Buffer[pApiTxd->ptr++]);
		}
/*		
#ifdef CAN_MASTER		
		pApiTxd->dwCanID &= ~(((DWORD)M_MANAGER_ADDR_MASK)<<M_MANAGER_ADDR_OFFSET);
#else
		pApiTxd->dwCanID &= ~(((DWORD)SLAVE_MODULE_ADDR_MASK)<<SLAVE_MODULE_ADDR_OFFSET);
#endif
*/
		id = pApiTxd->dwCanID;

		pApiTxd->dwStatus = CAN_BUF_BUSY;
	}

	
	//д�����ݳ��ȼ�FF��ʾ
	outportb(SJA_TXDID, len|EFF_FF);
	
	//д��ID
	id <<= 3;
	outportb(SJA_TXDID+1, (BYTE)(id>>24));
	outportb(SJA_TXDID+2, (BYTE)(id>>16));
	outportb(SJA_TXDID+3, (BYTE)(id>>8));
	outportb(SJA_TXDID+4, (BYTE)id);
	
	//������������
	SendRequest();
	
	return ERR_SUCCESS;
}

/*
void TestSja1000Txd()
{
	BYTE TxdBuf[20], len;
	int i;
	
	len = bSja1000RxdBuf[0];
	
	TxdBuf[0] = (EFF_FF|len);
	
	TxdBuf[1] = wMyAdd;
	TxdBuf[2] = 0;
	TxdBuf[3] = 0;
	TxdBuf[4] = 0;

	for(i=0; i<len; i++)
		TxdBuf[5+i] = bSja1000RxdBuf[i+1];

	for(i=0; i<(len+5); i++)
		outportb(SJA_TXBUFFER+i, TxdBuf[i]);

	SendRequest();
	
}

void TestSja1000Hard(DWORD *pdwApp)
{
	OS_TaskDesp *pTask = NULL;

	pTask = OS_GetTaskTab(SELF_TASK_ID);

	Sja1000Open(0);
	
	memset(bSja1000RxdBuf, 0, sizeof(bSja1000RxdBuf));
	//outportb(LED1_CS_BASE,0xff);
	while(1)
	{
		//OS_Sleep(1000);
		//printf("Test SJA1000 hardware...\n");
		//outportb(LED1_CS_BASE,0);
		//outportb(LED2_CS_BASE,0);
		//outportb(LED3_CS_BASE,0);
		//printf("Sja1000: Txd=%d Rxd=%d Error = %d.\n",dwSjaTxdIntCnt, dwSjaRxdIntCnt, dwSjaErrorIntCnt);
		if(bSja1000RxdBuf[0])
		{
			TestSja1000Txd();
			bSja1000RxdBuf[0] = 0;
		}

		OS_Sleep(20);

		pTask->Counter = 0;
	}
}
*/
#endif
