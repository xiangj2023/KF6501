/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			canlink.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           CAN����·������Լ���Ӧ�ڶ˿ڹ���Ĳ����ӿ�                                                                      
*			                                                                
* ����                                                                             
*                                                                                  
*       ����·����й�����·�����ݰ���:ģ��㻺��Ͷ˿ڲ㻺�壬����ģ��㻺��Ϊÿ��ͨ��                      
*       ��ģ���ṩһ���շ�������(������·��������Ӧ�ò����ݻ���)���˿ڲ㻺�������϶�Ӧ                                                                                  
*       �����CAN�˿ڣ�ÿ��CAN�˿ڶ�Ӧһ�飬ÿ��������ͺͽ��ջ��壬���н��ջ���Ϊ���λ�����                                                                                 
*       ���ͻ���Ϊ��ͨ�������塣                                                                                 
*                                                                                        
* ����                                                                             
*                                                                                  
*      	CCanTask::Init		CAN��·�����ʼ��
*      	CCanTask::OnTimeOut	CAN��·��ʱ��Ϣ����                                                              
*      	CCanTask::ErrProc  	CAN��·������                                                                         
*		CCanTask::RxdProc	ģ��㻺��������ݴ���
*		CCanTask::TxdProc	ģ��㻺�巢�����ݴ���
*		CCanTask::PortBufProc		�˿ڻ���������
*		CCanTask::isPortTxdBufIdle	���˿ڲ㷢�ͻ������Ƿ����
*		CCanTask::OnCanHardWare		CANӲ�������շ���Ϣ��Ӧ����
*		CCanTask::ApiRxdFramProc	����Ӧ�ñ��Ĵ���
*		CCanTask::CanLinkInit		CAN��·������ʼ��
*		ChangeToCanID		���ݴ�ģ���Ե��ַ�ͱ�־,ת��Ϊ��׼����ID
*		isCanPortTxdIdle	���ָ��CAN���˿��Ƿ��Ϳ���
*		CanOpen				CAN���˿ڹ���ӿڣ��˿ڴ�
*		CanRead				CAN���˿ڹ���ӿڣ���ָ��CAN���˿ڵĽ��ն˿ڻ�������ȡһ����������
*		CanWrite			CAN���˿ڹ���ӿڣ���ָ��CAN���˿ڵķ��Ͷ˿ڻ�����д��һ����������
*		
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/9/7    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "canlink.h"
#include "candrv.h"
#include "daefunc.h"

BEGIN_MESSAGE_MAP(CCanTask,CApp)
	ON_SM_TIMEOUT(CCanTask)
	ON_SM_CANTXDRXDOK(CCanTask)
END_MESSAGE_MAP()


//��ַӳ��������±�ΪCANͨ�ŵ�ַ������Ϊģ���豸ƫ�ƣ�����ֱ�Ӷ�λ�����pCanModuleData
DWORD dwCanAddMapTab[MAX_MODULE_NUM];

//CAN��·������ID
DWORD dwCanLinkTaskID;

BOOL bCanLinkInit = FALSE;

//CAN�˿����ݻ���
TCanPortData *pCanPortData[MAX_CAN_PORT];

//CAN���˿�����
TCanPort CanPortAttr[MAX_CAN_PORT];

DWORD canwritecnt = 0;

//CAN����·���������
void CanLinkTask(DWORD *pdwApp)
{
	CCanTask *pCanTask = new CCanTask(pdwApp);
	pCanTask->Init();
	
	bCanLinkInit = TRUE;
	
	pCanTask->Run();
}	

CCanTask::CCanTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

/*********************************************************************************************
 *
 * ��������     CAN��·�����ʼ��
 *
 * ����˵��      ��
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::Init(void)
{
	
	dwCanLinkTaskID = GetTaskId();

	address = (BYTE)DB_GetDevAddress();
		
	CanLinkInit();
	
	SetTimer(1, CAN_LINK_TIME);
}

/*********************************************************************************************
 *
 * ��������     CAN��·��ʱ��Ϣ������ʱ�������CAN�˿ڣ��ڶ˿ڴ򿪵�����£�˳���������
 *				����:��鷢���Ƿ�һֱ���ڷǿ���״̬������ʱ�䴦�ڷ��ͷǿ���̬�������쳣����
 *				�������Ƿ�һֱ���ڷǿ���״̬������ʱ�䴦�ڽ��շǿ���̬�������쳣����
 *				ģ������ݽ��մ���(ѭ������ģ��Ľ�������)��ģ������ݷ��ʹ���(ѭ������
 *				ģ��ķ�������)���˿ڲ����ݻ�����������ʱ����
 *
 * ����˵��      - id		: ���� 	����
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::OnTimeOut(DWORD id)
{
	dwTimeCnt++;
	
	for(int i=0; i<MAX_CAN_PORT; i++)
	{
		//ֻ����CAN���˿ڴ򿪵�����²��ܽ������ݴ���
		if(CanPortAttr[i].dwFlag & CAN_PORT_OPEN)
		{
			//ͨ�����ɼ���
			PortLoadMonitor(i);

			//�������ݴ���
			RxdProc(i);

			//��ʱ���˿ڲ㻺��������������Ӧ����
			PortBufProc(i);

			//�������ݴ���
			TxdProc(i);

			//������ʱ
//			if( ( !(dwTimeCnt%CAN_BROADCAST_TIME) ) && (CanPortAttr[i].bEnableBroadTime == TRUE) )
//				BroadCastTime();
				
		}
	}
	
}

/*********************************************************************************************
 *
 * ��������     CANͨ�����ɼ���
 *
 * ����˵��      - port		: ���� 	ͨ����
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::PortLoadMonitor(BYTE port)
{
	
	//��鷢���Ƿ����
	if(CanPortAttr[port].dwFlag & CAN_PORT_TXD_IDLE)
		CanPortAttr[port].dwTxdBusyCnt = 0;
	else
	{
		CanPortAttr[port].dwTxdBusyCnt++;
		if(CanPortAttr[port].dwTxdBusyCnt>=CAN_PORT_BUSY_CNT)
		{
			CanPortAttr[port].dwTxdBusyCnt = 0;
			ErrProc(CAN_PORT_TXD_BUSY_ERR);
		}
	}
	
	//�������Ƿ����
	if(CanPortAttr[port].dwFlag & CAN_PORT_RXD_IDLE)
		CanPortAttr[port].dwRxdBusyCnt = 0;
	else
	{
		CanPortAttr[port].dwRxdBusyCnt++;
		if(CanPortAttr[port].dwRxdBusyCnt>=CAN_PORT_BUSY_CNT)
		{
			CanPortAttr[port].dwRxdBusyCnt = 0;
			ErrProc(CAN_PORT_RXD_BUSY_ERR);
		}
	}
	
}

/*********************************************************************************************
 *
 * ��������     CAN��·������
 *
 * ����˵��      - type		: ���� 	��������
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::ErrProc(BYTE type)
{
	
}

/*********************************************************************************************
 *
 * ��������     ģ��㻺��������ݴ���:˳�������д�ģ����ջ����������ȼ����·�㡢Ӧ�ò�����
 *				�Ƿ���أ�Ȼ����Ӧ�ò���ձ����Ƿ�׼���ã���׼�����򽫸������͸��˿ڲ㻷�ν���
 *				��������
 *
 * ����˵��      - port		: ���� 	�˿�
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::RxdProc(BYTE port)
{
	TCanPortData *pCanChannel;
	TCanLinkFram *pLinkRxd;
	TCanCellBuf *pApiRxd;
	BYTE address;

	//������ջ�����
//	for(int i=0; i<bModuleNum; i++)
	{
		pCanChannel = pCanPortData[port];

		//���ȼ�鲢������·������
		pLinkRxd = &pCanChannel->CanLinkCmdBuf.RxdBuf;

		//��·�����������ɣ��ȴ�����
		if(pLinkRxd->dwStatus == CAN_DATA_FINISH)
		{
			LinkRxdFramProc(port);
		}
		//��·�����æ���쳣
		else if(pLinkRxd->dwStatus == CAN_BUF_BUSY)
		{
			memset(pLinkRxd, 0, sizeof(TCanLinkFram));
			pLinkRxd->dwStatus = CAN_BUF_EMPTY;
			pCanChannel->CanStatic.dwLinkRxdErrCnt++;
		}

		//����Ӧ�ò����ݻ�����
		pApiRxd = &pCanChannel->CanDevBuf.RxdBuf;
		address = (BYTE)GET_CAN_MODULE_ADD(pApiRxd->dwCanID);
		
		//Ӧ�ò㱨�Ľ�����ɣ����ǲ�һ����ȷ
		if(pApiRxd->dwStatus == CAN_DATA_FINISH)
		{
			//�����ջ��������ݱ�־
			//�зְ���ų���/֡�ܳ��ȳ������������ģ������з񶨻ش�
			if((pApiRxd->dwFlag & SUB_FRAM_INDEX_ERR) || (pApiRxd->dwFlag & FRAM_LEN_ERR))
			{
				pCanChannel->CanStatic.dwApiRxdErrCnt++;
				TxdNAck(port);
				
				memset(pApiRxd, 0, sizeof(TCanCellBuf));
			}
			else	//����������ȷ�����ݱ�������,д��Ӧ�ò㻺������
			{
				if(pApiRxd->dwFlag & RXD_BUF_OVERRUN)
					pCanChannel->CanStatic.dwApiRxdOverRun++;
				
				TxdAck(port);
				ApiRxdFramProc(port);
			}
		}
		//Ӧ�ò���ջ�����æ����Ҫ�ȴ�,�����ʱ��æ�Ļ���˵����·�����⣬��Ҫ���¸�λ��·
		else if(pApiRxd->dwStatus == CAN_BUF_BUSY)
		{
			pApiRxd->dwRxdBusyCnt++;

			//��ʱ���������æ��������ջ���������ȷ��
			if(pApiRxd->dwRxdBusyCnt>CAN_RXD_BUSY_TIME)
			{
				pApiRxd->dwRxdBusyCnt = 0;
				pCanChannel->CanStatic.dwApiRxdErrCnt++;
				TxdNAck(port);
				
				memset(pApiRxd, 0, sizeof(TCanCellBuf));
			}
		}
	}
}


/*********************************************************************************************
 *
 * ��������    ������·�㱨�Ľ��մ���
 *
 * ����˵��      - port	: 	���� 	CAN���˿ں�
 *
 * ���ش���
 *               �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CCanTask::LinkRxdFramProc(BYTE port)
{
	DWORD type, address;
	TCanStatic *pStatic;
	TCanCellBuf *pCellTxdBuf;
	
	TCanLinkFram *pLinkRxd = &pCanPortData[port]->CanLinkCmdBuf.RxdBuf;

	//����ͳ�ƴ���
	pStatic = &pCanPortData[port]->CanStatic;
	pStatic->dwLinkRxdOkCnt++;

	if(pLinkRxd->dwFlag & RXD_BUF_OVERRUN)
		pStatic->dwLinkRxdOverRun++;

	//ȡ��·�㱨������
	type = GET_CAN_FRAME_TYPE(pLinkRxd->dwCanID);
	address = GET_CAN_MODULE_ADD(pLinkRxd->dwCanID);
	
	//�����·����ջ��棬�ȴ��µĴ���
	memset(pLinkRxd, 0, sizeof(TCanLinkFram));
	
	switch(type)
	{
		//��·��϶�ȷ�ϣ�˵������������ȷ�����գ����Ӧ�ò㱨�ķ��ͻ��棬׼���·���
		case LINK_ACK_TYPE:
			if(pCanPortData[port]->CanDevBuf.TxdBuf.dwStatus == CAN_DATA_FINISH)
				memset(&pCanPortData[port]->CanDevBuf.TxdBuf,0, sizeof(TCanCellBuf));

			pStatic->dwApiTxdOkCnt++;
			break;
			
		//��·���ȷ�ϣ�˵����������ʧ�ܣ��Ըղŵ�Ӧ�ò㱨�Ľ����ط�
		case LINK_NACK_TYPE:
			//���Ӧ�ò����ݷ��ͻ������ǿգ��ط�������
			pCellTxdBuf = &pCanPortData[port]->CanDevBuf.TxdBuf;
			if(pCellTxdBuf->dwStatus != CAN_BUF_EMPTY)
			{
				pCellTxdBuf->dwStatus = CAN_DATA_READY;
				CanHardWrite(port, GET_CAN_MODULE_ADD(pCellTxdBuf->dwCanID), CAN_API_FRAME, FALSE);
			}
			pStatic->dwApiTxdErrCnt++;
			break;
		//��λ��·�������ǰ��·�㷢�ͻ��棬���п϶�ȷ��	
		case LINK_RESET_TYPE:
			ReportMsg("receive can%d link reset command.", port+1);
			CanHardReset(port);
			TxdAck(port);
			break;
		case LINK_BROAD_TIME:
			break;
		default:
			break;
	}

	return ERR_SUCCESS;
}

//��·��϶�ȷ�ϱ��ķ���
DWORD CCanTask::TxdAck(BYTE port)
{
	DWORD id = 0;
	TCanLinkFram *pLinkTxd;

	pLinkTxd = &pCanPortData[port]->CanLinkCmdBuf.TxdBuf;
	
	//��������
	id = LINK_ACK_TYPE<<LINK_FRAM_TYPE_OFFSET;
	
	//ģ���ַ
//#ifdef CAN_MASTER
	id |= ((DWORD)address)<<SLAVE_MODULE_ADDR_OFFSET;
//#endif
	//������ַ
	id |= 0x1<<M_MANAGER_ADDR_OFFSET;
	//������ַ
	id |= 0x1<<B_MANAGER_ADDR_OFFSET;

	id |= 0x1<<SUB_FRAM_END_OFFSET;
	
	pLinkTxd->dwStatus = CAN_DATA_READY;
	pLinkTxd->dwFlag = 0;

	//���ڱ���������ID�п�����ȫʶ�𣬶�����·�㱨�ģ����˶�ʱ�⣬�������е����������壬���峤��Ϊ1������0xaa;
	pLinkTxd->length = 1;
	pLinkTxd->buffer[0] = 0x55;

	pLinkTxd->dwCanID = id;

	//���ͱ���
	return CanHardWrite(port, address, CAN_LINK_FRAME, TRUE);
}

//��·���ȷ�ϱ��ķ���
DWORD CCanTask::TxdNAck(BYTE port)
{
	DWORD id = 0;
	TCanLinkFram *pLinkTxd;

	pLinkTxd = &pCanPortData[port]->CanLinkCmdBuf.TxdBuf;
	
	//��������
	id = LINK_NACK_TYPE<<LINK_FRAM_TYPE_OFFSET;
	//ģ���ַ
//#ifdef CAN_MASTER
	id |= ((DWORD)address)<<SLAVE_MODULE_ADDR_OFFSET;
//#endif
	//������ַ
	id |= 0x1<<M_MANAGER_ADDR_OFFSET;
	//������ַ
	id |= 0x1<<B_MANAGER_ADDR_OFFSET;

	id |= 0x1<<SUB_FRAM_END_OFFSET;
	
	pLinkTxd->dwStatus = CAN_DATA_READY;
	pLinkTxd->dwFlag = 0;

	//���ڱ���������ID�п�����ȫʶ�𣬶�����·�㱨�ģ����˶�ʱ�⣬�������е����������壬���峤��Ϊ1������0xaa;
	pLinkTxd->length = 1;
	pLinkTxd->buffer[0] = 0xaa;

	pLinkTxd->dwCanID = id;

	//���ͱ���
	return CanHardWrite(port, address, CAN_LINK_FRAME, TRUE);
}

/*********************************************************************************************
 *
 * ��������     ģ��㻺�巢�����ݴ�������ÿ����ģ��˳��������´���:���˿ڻ�����Ӧ�ò㱨
 *				���Ƿ���Ӧ��ʱ�������ʱ����3���ط����ط����ɹ�������Ӧ�ñ��ģ����˿�
 *				�������Ƿ�����·��������Ҫ���ͣ����������з��ͣ����˿ڻ������Ƿ���Ӧ�ò���
 *				����Ҫ���ͣ����������з���
 *
 * ����˵��      - port		: ���� 	�˿�
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::TxdProc(BYTE port)
{
	TCanPortData *pCanTxd;
	BYTE address;
	TCanCellBuf *pApiFramTxd;
	
	//for(int i=0; i<bModuleNum; i++)
	{
		pCanTxd = pCanPortData[port];

		address = pCanTxd->address;
		
		pApiFramTxd = &pCanTxd->CanDevBuf.TxdBuf;
		
		//��·����������ط���ֻ����Ӧ�ò����ݵ��ط������ڳ�ʱ��Ӧ���ط��������3�Σ�
		//����3�Σ������÷��ͱ��ġ������ڷ�Ӧ���һֱ���ط�����
		//����Ƿ���Ӧ�ò�������Ҫ����
		if(pApiFramTxd->dwStatus == CAN_DATA_READY)
		{
			//ע��ÿ��ֻ�ܷ���һ����ַ�����ݣ���Ϊ�˿�ֻ��һ��
			CanHardWrite(port, address, CAN_API_FRAME, TRUE);
			return;
		}
		//���ڷ�����ȴ�����״̬����æ
		else if((pApiFramTxd->dwStatus == CAN_DATA_FINISH) || (pApiFramTxd->dwStatus == CAN_BUF_BUSY))
		{
			
			//��ʱ�ط�ʱ�����
			pApiFramTxd->wResendDelay++;

			//��ʱ�ط�����
			if(pApiFramTxd->wResendDelay >= CAN_RESEND_TIME)
			{
				pApiFramTxd->wResendDelay = 0;

				//�����ط����������������ͱ���
				if(pApiFramTxd->wResendCnt >= CAN_RESEND_LEVEL)
				{
					//���Ӧ�ñ��ķ���������
					memset((void*)pApiFramTxd, 0, sizeof(TCanCellBuf));
					
					//CanHardReset(port);
				}
				else	//�ط�����
				{
					
					pApiFramTxd->dwStatus = CAN_DATA_READY;
					CanHardWrite(port, address, CAN_API_FRAME, FALSE);
					pApiFramTxd->wResendCnt++;
					return;
				}
			}
			
		}
		else	//�����ڷ�����ĵȴ�����״̬��������ط��ȴ�����ֵ
		{
			pApiFramTxd->wResendDelay = 0;
		}

	}
}

/*********************************************************************************************
 *
 * ��������     �˿ڻ������������˿ڷ��ͻ������Ƿ���У�������֪ͨӦ�ò����񣻷��򽫶˿�
 *				���ͻ��������ݸ��Ƶ���Ӧ��ģ�鷢�ͻ������С�
 *
 * ����˵��      - port		: ���� 	�˿�
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
extern TSysConfigTable	*G_pCfgTable;
void CCanTask::PortBufProc(BYTE port)
{
	//�˿ڷ��ͻ�����ȫ�����У�֪ͨӦ�ò㷢�Ϳ���
	if(isPortTxdBufIdle(port) == TRUE)
	{
		pCanPortData[port]->dwComIdleCnt++;
		if(pCanPortData[port]->dwComIdleCnt > pCanPortData[port]->dwComIdleLevel)
		{
//			if(port == 1)
//				printf("report com idle =%d, idlelevel=%d.\n", G_pCfgTable->SystemStatus.stTime.Lo%10000,\
//					pCanPortData[port]->dwComIdleLevel);
			OS_PostMessage(CanPortAttr[port].dwTargetId, SM_COMMIDLE, port, 0, 0, 0);
			pCanPortData[port]->dwComIdleCnt = 0;
		}
	}
	else
	{
		pCanPortData[port]->dwComIdleCnt = 0;
		
		//�˿ڷ��ͻ������ǿգ������ݸ��Ƶ���Ӧ��ַ����·��Ӧ�ñ��Ļ�����
		PortBuf2FramBuf(port);
	}
}

/*********************************************************************************************
 *
 * ��������     ���˿ڲ㷢�ͻ������Ƿ����
 *
 * ����˵��      - port		: ���� 	�˿�
 *
 * ���ش���
 *               �ǻ�� 
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL CCanTask::isPortTxdBufIdle(BYTE port)
{
	TCanApiFram *pApiNBuf, *pApiUBuf;

	pApiNBuf = &pCanPortData[port]->CanApiNFram;
	pApiUBuf = &pCanPortData[port]->CanApiUFram;

	//��ͨ���ĺͽ������ķ��ͻ�����������
	if( (pApiNBuf->TxdRBufNo == pApiNBuf->TxdWBufNo) && (pApiUBuf->TxdRBufNo == pApiUBuf->TxdWBufNo) )
		return TRUE;
	else 
		return FALSE;
}

/*********************************************************************************************
 *
 * ��������     �˿ڷ��ͻ��������ݸ��Ƶ���Ӧ��ַ����·��Ӧ�ñ��Ļ�����,���Ǳ��뱣֤��·��Ӧ�ñ��Ļ�����
 *				Ӧ�ñ��Ļ�����Ϊ���У������ܸ���
 *
 * ����˵��      - port		: ���� 	�˿�
 *
 * ���ش���
 *               �ǻ�� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::PortBuf2FramBuf(BYTE port)
{
	TCanFramBuf *pTxdBuf;
	TCanCellBuf *pLinkBuf;
	TCanApiFram *pApiBuf = NULL;

	//���ȼ������������ķ��ͻ�����,Ȼ������ͨ����
	if(pCanPortData[port]->CanApiUFram.TxdRBufNo != pCanPortData[port]->CanApiUFram.TxdWBufNo)
		pApiBuf = &pCanPortData[port]->CanApiUFram;
	else if(pCanPortData[port]->CanApiNFram.TxdRBufNo != pCanPortData[port]->CanApiNFram.TxdWBufNo)
		pApiBuf = &pCanPortData[port]->CanApiNFram;
	
	if(pApiBuf == NULL)
		return;

	//��������Ҫ���ͣ��������͵�Ӧ����·�����ݷ��ͻ�����
	pTxdBuf = &pApiBuf->TxdBuf[pApiBuf->TxdRBufNo];
	pLinkBuf = &pCanPortData[port]->CanDevBuf.TxdBuf;

	//�����·��Ӧ�ñ��ķ��ͻ������Ƿ�գ��ǿյĻ����ܸ��Ǹû�����
	if(pLinkBuf->dwStatus != CAN_BUF_EMPTY)
	{
//		ReportMsg("link data txd buffer busy, send frame failed, status = %x.", pLinkBuf->dwStatus);
		return;
	}

	//���ݸ��Ƶ���Ӧ������
	if(pApiBuf->TxdRBufNo != pApiBuf->TxdWBufNo)
	{
		memset(pLinkBuf, 0, sizeof(TCanCellBuf));
		memcpy(pLinkBuf->Buffer, pTxdBuf->Buffer, pTxdBuf->length);
		pLinkBuf->dwCanID = pTxdBuf->dwCanID;
		pLinkBuf->length = pTxdBuf->length;
		pLinkBuf->dwStatus = CAN_DATA_READY;

		//��ԭʼ��������ָ�����
		pApiBuf->TxdRBufNo++;
		pApiBuf->TxdRBufNo %= CAN_API_BUF_NUM;
	}
}

/*********************************************************************************************
 *
 * ��������     CANӲ�������շ���Ϣ��Ӧ������Ϣ���Ͱ���:�������յ�Ӧ�ò����ݡ��յ���·��϶�
 *				�ش��յ���·��񶨻ش��յ���ʱ����ȡ�
 *
 * ����˵��      - arg		: ���� 	��Ϣ����
 *
 * ���ش���
 *               �ǻ�� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::OnCanHardWare(DWORD arg)
{
	BYTE type, port;
	TCanStatic *pStatic;
	TCanCellBuf *pApiRxd;

	TCanPortData *pCanChannel;
	TCanCellBuf *pCellTxdBuf;

	//������Ϣ����Ϣ��
	type = (BYTE)arg;
	port = (BYTE)(arg>>8);
	ASSERT(port<MAX_CAN_PORT);
	if(port>=MAX_CAN_PORT)
	{
		LogError("OnCanTxdRxdOK",FILE_LINE,"the port number invalide.");
		return;
	}
//	address = (BYTE)(arg>>16);

	pCanChannel = pCanPortData[port];
	pStatic = &pCanPortData[port]->CanStatic;

	switch(type)
	{
		case API_N_FRAM_TYPE:	//Ӧ�ò㱨�Ľ�����ɣ�����Ҫ����Ƿ�����
		case API_U_FRAM_TYPE:
			pApiRxd = &pCanChannel->CanDevBuf.RxdBuf;
//			address = (BYTE)GET_CAN_MODULE_ADD(pApiRxd->dwCanID);
			
			//Ӧ�ò㱨�Ľ�����ɣ����ǲ�һ����ȷ
			if(pApiRxd->dwStatus == CAN_DATA_FINISH)
			{
				//�����ջ��������ݱ�־
				//�зְ���ų���/֡�ܳ��ȳ������������ģ������з񶨻ش�
				if((pApiRxd->dwFlag & SUB_FRAM_INDEX_ERR) || (pApiRxd->dwFlag & FRAM_LEN_ERR))
				{
					pCanChannel->CanStatic.dwApiRxdErrCnt++;
					
					memset(pApiRxd, 0, sizeof(TCanCellBuf));
					TxdNAck(port);
				}
				else	//����������ȷ�����ݱ�������,д��Ӧ�ò㻺������
				{
					if(pApiRxd->dwFlag & RXD_BUF_OVERRUN)
						pCanChannel->CanStatic.dwApiRxdOverRun++;
					pCanChannel->CanStatic.dwApiRxdOkCnt++;
					
					TxdAck(port);
					
					ApiRxdFramProc(port);
				}
			}
			break;
		case LINK_ACK_TYPE:	//���յ���·��϶��ش�
			
			//�����·����ջ��棬�ȴ��µĴ���
			memset(&pCanPortData[port]->CanLinkCmdBuf.RxdBuf, 0, sizeof(TCanLinkFram));

			//˵���ղŷ��͵�Ӧ�ò����ݱ��Է���ȷ���գ����Խ�����Ӧ�ò����ݵķ���
			if(pCanPortData[port]->CanDevBuf.TxdBuf.dwStatus == CAN_DATA_FINISH)
				memset(&pCanPortData[port]->CanDevBuf.TxdBuf,0, sizeof(TCanCellBuf));
			
			pStatic->dwApiTxdOkCnt++;
			
//			//������Ϣ��Ӧ�ò�����,���Ϳ��п��Է�����Ӧ�ò�����
//			OS_PostMessage(CanPortAttr[port].dwTargetId, SM_COMMIDLE, port, address, 0, 0);
			break;
		case LINK_NACK_TYPE:	//�յ���·��񶨻ش���Ҫ�ط�Ӧ�ò�����
			//�����·����ջ��棬�ȴ��µĴ���
			memset(&pCanPortData[port]->CanLinkCmdBuf.RxdBuf, 0, sizeof(TCanLinkFram));

			//���Ӧ�ò����ݷ��ͻ������ǿգ��ط�������
			pCellTxdBuf = &pCanPortData[port]->CanDevBuf.TxdBuf;
			if(pCellTxdBuf->dwStatus != CAN_BUF_EMPTY)
			{
				pCellTxdBuf->dwStatus = CAN_DATA_READY;
				CanHardWrite(port, GET_CAN_MODULE_ADD(pCellTxdBuf->dwCanID), CAN_API_FRAME, FALSE);
			}
			pStatic->dwApiTxdErrCnt++;
			break;
		case LINK_BROAD_TIME:		//���յ��㲥��ʱ����
			//����ϵͳʱ��
			
			//�����·����ջ��棬�ȴ��µĴ���
			memset(&pCanPortData[port]->CanLinkCmdBuf.RxdBuf, 0, sizeof(TCanLinkFram));

			break;
		case LINK_RESET_TYPE://��λ��·
			//�����·����ջ��棬�ȴ��µĴ���
			CanHardReset(port);
			TxdAck(port);
			break;
		default:
			break;
	}
}

/*********************************************************************************************
 *
 * ��������     ����Ӧ�ñ��Ĵ�����������ģ���Ӧ�ý��ձ��ĸ��Ƶ��˿ڲ㻷�ν��ջ���������
 *				֪ͨӦ�ò�������
 *
 * ����˵��      - port		: ���� 	CAN���˿����
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CCanTask::ApiRxdFramProc(BYTE port)
{
	TCanCellBuf *pRxdFrm;
	TCanApiFram *pApiRxd;
	DWORD type;
	BYTE *pFrame;

	pRxdFrm = &pCanPortData[port]->CanDevBuf.RxdBuf;

	type = (pRxdFrm->dwCanID>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK;

	//�������ĺ���ͨ���ķֱ𱣴�
	if(type == API_U_FRAM_TYPE)
		pApiRxd = &pCanPortData[port]->CanApiUFram;
	else
		pApiRxd = &pCanPortData[port]->CanApiNFram;


	//�������ݵ�Ӧ�ò㻺����,����д���������
	OS_Protect(0);
	pFrame = pRxdFrm->Buffer;
	memcpy(&pApiRxd->RxdBuf[pApiRxd->RxdWBufNo].Buffer, pRxdFrm->Buffer, pRxdFrm->length);
	pApiRxd->RxdBuf[pApiRxd->RxdWBufNo].length = pRxdFrm->length;
	pApiRxd->RxdBuf[pApiRxd->RxdWBufNo].dwCanID = pRxdFrm->dwCanID;
	pApiRxd->RxdWBufNo++;
	pApiRxd->RxdWBufNo %= CAN_API_BUF_NUM;

	//ͬʱ�����յ���Ӧ�ò㱨���͵����ӻ�����
	TCanMonitorBuf *pMonitor = &pCanPortData[port]->MonitorBuf;
	BYTE *pBuf = pMonitor->RxdBuffer;
	for(int i=0; i<pRxdFrm->length; i++)
	{
		pBuf[pMonitor->wRxdWritePtr++] = pFrame[i];
		pMonitor->wRxdWritePtr %= CAN_MONITOR_BUF_LEN;
	}
	
	OS_UnProtect(0);

	//�����·��Ӧ�ñ��Ļ���
	memset(pRxdFrm, 0, sizeof(TCanCellBuf));
	
	//������Ϣ��Ӧ�ò�����
	OS_PostMessage(CanPortAttr[port].dwTargetId, SM_READCOMM, 0, 0, 0, 0);
	
}

/*********************************************************************************************
 *
 * ��������     CAN��·������ʼ��������ģ����Ŀ��Ϊģ���Ͷ˿ڲ�����Ӧ��������
 *
 * ����˵��      ��
 *
 * ���ش���
 *               �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CCanTask::CanLinkInit()
{
	BYTE i;
//	BYTE *pTxdBuf, *pRxdBuf;
//	TCanPortData *pModule;
	
	memset((void*)CanPortAttr, 0, sizeof(TCanPort)*MAX_CAN_PORT);

	//��ʼ��ÿ��CANģ��
	for(i=0; i<MAX_CAN_PORT; i++)
	{
		//��ʼ��CAN�˿�����
		
		pCanPortData[i] = (TCanPortData*)OS_MemAlloc(sizeof(TCanPortData));
		ASSERT(pCanPortData[i] != NULL);
		if(pCanPortData[i] == NULL)
		{
			LogError("CanLinkInit",FILE_LINE,"malloc for pCanModuleData invalide.");
			return ERR_FAILURE;
		}

		memset((void*)pCanPortData[i], 0, sizeof(TCanPortData));

	}

	//����Ĭ��COMIDLE ��ֵΪ500����
	pCanPortData[i]->dwComIdleLevel = 500/CAN_LINK_TIME;
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ���ݴ�ģ���Ե��ַ�ͱ�־(������ַ��������ַ����ģ���鲥��ַ)��ת��Ϊ��׼
 *				����ID,������SJA1000��׼��ID.
 *
 * ����˵��      - address	: ���� 	��ģ���Ե��ַ
 *				 - flag		: ����	��ַ��ʶ
 *
 * ���ش���
 *               ��׼ID
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD ChangeToCanID(BYTE address, BYTE flag)
{
	DWORD id = 0;

	id = address;

	id <<= SLAVE_MODULE_ADDR_OFFSET;	//ת����ģ���ַ
	if(flag & MAIN_MANGER_ADDR)			//������ַ
		id |= (0x01<<M_MANAGER_ADDR_OFFSET);
	if(flag & BACK_MANAGER_ADDR)		//������ַ
		id |= (0x01<<B_MANAGER_ADDR_OFFSET);
	if(flag & SLAVE_MODULE_GRP_ADDR)	//��ģ���鲥��ַ
		id |= (0x03<<SLAVE_MOD_GRP_ADD_OFFSET);
	return id;
}

/*********************************************************************************************
 *
 * ��������    ���ָ��CAN���˿��Ƿ��Ϳ���
 *
 * ����˵��      - port		: ���� 	CAN���˿����
 *
 * ���ش���
 *               �����Ƿ����
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL isCanPortTxdIdle(BYTE port)
{
	//���CAN�����Ƿ���У�������������ܷ�������
	if( CanPortAttr[port].dwFlag & CAN_PORT_TXD_IDLE )
		return TRUE;
	else
		return FALSE;
}


/*********************************************************************************************
 *
 * ��������    CAN���˿ڹ���ӿڣ��˿ڴ�
 *
 * ����˵��      - bPortSort	: ���� 	CAN���˿ں�
 *				 - dwFlag		: ����	��ʶ
 *
 * ���ش���
 *               �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanOpen(BYTE bPortSort, DWORD dwFlag)
{
	DWORD status;
	ASSERT(bPortSort>0);
	if(bPortSort == 0)
	{
		LogError("CanOpen",FILE_LINE,"the port number invalide.");
		return ERR_FAILURE;
	}

	//����Ƿ������ʱ(�����Ͷ�ʱ����)
	if(dwFlag & ENABLE_BROAD_TIME)
		CanPortAttr[bPortSort-1].bEnableBroadTime = TRUE;
	else
		CanPortAttr[bPortSort-1].bEnableBroadTime = FALSE;
	
	status = CanHardOpen(bPortSort-1);
	if(status == ERR_SUCCESS)
	{
		CanPortAttr[bPortSort-1].dwFlag = CAN_PORT_OPEN|CAN_PORT_TXD_IDLE|CAN_PORT_RXD_IDLE;
	}

	//��ȡ�˿ڹ�������ID
	CanPortAttr[bPortSort-1].dwTargetId = RtosGetCurrentTaskId();
	
	return status;
}

/*********************************************************************************************
 *
 * ��������    CAN���˿ڹ���ӿڣ���ָ��CAN���˿ڵĽ��ն˿ڻ������ж�ȡһ�����ȵ����ݡ�
 *
 * ����˵��      - bPortSort	: ���� 	CAN���˿ں�
 *				 - buf			: ���	��ȡ���ݻ�����
 *				 - dwLen		: ����	׼����ȡ�����ݳ���
 *				 - dwFlag		: ����	��ʶ
 *
 * ���ش���
 *               ʵ�ʶ�ȡ�����ݳ���
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanRead(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag)
{
	WORD len;
	TCanPortData *pModule;
	TCanApiFram *pRxdFram;
	
	ASSERT(bPortSort>0);
	if((bPortSort == 0) ||(bPortSort>MAX_CAN_PORT))
	{
		LogError("CanWrite",FILE_LINE,"the port number invalide.");
		return 0;
	}

	//������ݳ����Ƿ���Ч
	if( (buf == NULL) || (dwLen == 0))
		return 0;

//	index = dwCanAddMapTab[dwFlag&0xff];
	pModule = pCanPortData[bPortSort-1];

	//���ȴ�����յ���������
	pRxdFram = &pModule->CanApiUFram;
	if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
	{
		len = pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].length;
		OS_Protect(0);
		memcpy(buf, pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].Buffer, len);
		pRxdFram->RxdRBufNo++;
		pRxdFram->RxdRBufNo %= CAN_API_BUF_NUM;
		OS_UnProtect(0);

		//����Ƿ��к���������Ҫ����,֪ͨӦ�ò�����
		if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
			OS_PostMessage(CanPortAttr[bPortSort-1].dwTargetId, SM_READCOMM, 0, 0, 0, 0);

		return len;
	}

	//������ͨ���ձ���
	pRxdFram = &pModule->CanApiNFram;
	if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
	{
		len = pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].length;
		OS_Protect(0);
		memcpy(buf, pRxdFram->RxdBuf[pRxdFram->RxdRBufNo].Buffer, len);
		pRxdFram->RxdRBufNo++;
		pRxdFram->RxdRBufNo %= CAN_API_BUF_NUM;
		OS_UnProtect(0);

		//����Ƿ��к���������Ҫ����,֪ͨӦ�ò�����
		if(pRxdFram->RxdWBufNo != pRxdFram->RxdRBufNo)
			OS_PostMessage(CanPortAttr[bPortSort-1].dwTargetId, SM_READCOMM, 0, 0, 0, 0);

		return len;
	}

	return 0;
}

/*********************************************************************************************
 *
 * ��������    CAN���˿ڹ���ӿڣ���ָ��CAN���˿ڵķ��Ͷ˿ڻ�����д��һ�����ȵ����ݡ�
 *
 * ����˵��      - bPortSort	: ���� 	CAN���˿ں�
 *				 - buf			: ���	ԭʼд�����ݻ�����
 *				 - dwLen		: ����	׼��д������ݳ���
 *				 - dwFlag		: ����	��ʶ,����ֽڱ�ʾ��ģ���ַ�����λ��ʾ�Ƿ�Ϊ��������
 *
 * ���ش���
 *               ʵ��д������ݳ���
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanWrite(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag)
{
	WORD len, num;
	TCanApiFram *pTxdFram;
	DWORD type;
//	static DWORD lotime, errcnt;
//	DWORD time;

	ASSERT(bPortSort>0);
	if((bPortSort == 0) ||(bPortSort>MAX_CAN_PORT))
	{
		LogError("CanWrite",FILE_LINE,"the port number invalide.");
		return 0;
	}

	//������ݳ����Ƿ���Ч
	if( (buf == NULL) || (dwLen == 0) || (dwLen>CAN_FRAME_BUF_LEN))
		return 0;
/*
	if(bPortSort == 2)
	{
		time = G_pCfgTable->SystemStatus.stTime.Lo;
		printf("SJA100 CanWrite time=%d.\n", G_pCfgTable->SystemStatus.stTime.Lo%10000);

		if((time-lotime)<80)
			errcnt++;
		lotime = time;
	}
*/	
	//�ֱ�����ͨ���ĺͽ�������
	if(dwFlag &0x80000000)
	{
		pTxdFram = &pCanPortData[bPortSort-1]->CanApiUFram;
		type = API_U_FRAM_TYPE<<LINK_FRAM_TYPE_OFFSET;
	}
	else
	{
		pTxdFram = &pCanPortData[bPortSort-1]->CanApiNFram;
		type = API_N_FRAM_TYPE<<LINK_FRAM_TYPE_OFFSET;
	}

	//���������ݴ���
	num = pTxdFram->TxdWBufNo;
	OS_Protect(0);
	memcpy(pTxdFram->TxdBuf[num].Buffer, buf, dwLen);
	pTxdFram->TxdBuf[num].length = dwLen;
	
//#ifdef CAN_MASTER
	pTxdFram->TxdBuf[num].dwCanID = (DB_GetDevAddress()&0x00ff)<<SLAVE_MODULE_ADDR_OFFSET;
//	printf("Can%d Write, module address=%d.\n ", bPortSort-1, DB_GetDevAddress());
/*
#else
	pTxdFram->TxdBuf[num].dwCanID = 0;
#endif
*/
	//���÷�����������ַ
	pTxdFram->TxdBuf[num].dwCanID |= M_MANAGER_ADDR_MASK<<M_MANAGER_ADDR_OFFSET;
	pTxdFram->TxdBuf[num].dwCanID |= B_MANAGER_ADDR_MASK<<B_MANAGER_ADDR_OFFSET;

	//���ñ������ͱ�ʾ
	pTxdFram->TxdBuf[num].dwCanID |= type;
	
	//�ƶ���������дָ��
	pTxdFram->TxdWBufNo++;
	pTxdFram->TxdWBufNo %= CAN_API_BUF_NUM;

	//ͬʱ�����ݸ��Ƶ����ӻ�����
	BYTE *pBuf = pCanPortData[bPortSort-1]->MonitorBuf.TxdBuffer;
	TCanMonitorBuf *pMonitor = &pCanPortData[bPortSort-1]->MonitorBuf;
	for(int i=0; i<dwLen; i++)
	{
		pBuf[pMonitor->wTxdWritePtr++] = buf[i];
		pMonitor->wTxdWritePtr %= CAN_MONITOR_BUF_LEN;
	}
	OS_UnProtect(0);

	return dwLen;
}

/*********************************************************************************************
 *
 * ��������    CAN���˿ڿ��ƽӿڣ���Ҫ��������COMIDLEʱ����
 *
 * ����˵��      - bPortSort	: ���� 	CAN���˿ں�
 *				 - comm			: ���	���Ʋ���
 *
 * ���ش���
 *               
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanCtrl(BYTE portSort,TCtlComm *comm)
{
	BYTE port;
	
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("CanCtrl",FILE_LINE,"the port number invalide.");
		return ERR_FAILURE;
	}

	port = portSort - 1;
	//��ȡ���Ʋ���
	if(comm->dwFuncCode == FC_GET)
	{
		*comm = pCanPortData[port]->CommCtl;
		return ERR_SUCCESS;
	}

	if(comm->dwFuncCode == FC_SET)
	{
		pCanPortData[port]->CommCtl = *comm;
		
		if(comm->dwCtlMask & CM_NOTIFYCOMMBREAK)
		{
//			if(port == 1)
//				ReportMsg("set CAN%d comidle level=%dms", port, comm->dwNotifyCommBreak);
			
			//��������COMIDLE��ֵ���������ǰ����ֵ
			//pCanPortData[port]->dwComIdleCnt = 0;
			pCanPortData[port]->dwComIdleLevel = comm->dwNotifyCommBreak/CAN_LINK_TIME;
		}
		return ERR_SUCCESS;
	}
	
	return ERR_FAILURE;
}

WORD GetCanTxdWritePtr(WORD portSort)
{
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("GetCanTxdWritePtr",FILE_LINE,"the port number invalide.");
		return 0;
	}
	
	return pCanPortData[portSort-1]->MonitorBuf.wTxdWritePtr;
}

WORD GetCanRxdWritePtr(WORD portSort)
{
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("GetCanRxdWritePtr",FILE_LINE,"the port number invalide.");
		return 0;
	}
	
	return pCanPortData[portSort-1]->MonitorBuf.wRxdWritePtr;
}

/*********************************************************************************************
 *
 * ��������      ��CANͨ�����ͻ�����ָ��λ�ö�ȡһ�����ȵ����ݣ���ά���鿴
 *
 * ����˵��   	- portSort:	����	����˿��еĶ˿����   
 *				- pBuf	: 	���	��Ҫ��ȡ�����ݻ���	
 *				- wLen :	����	׼����ȡ�����ݳ���
 *				- wOffset:	����	�������ʼ��ַ��ƫ��
 *
 *
 * ���ش���
 *                ʵ��д��������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD ReadFromCanTxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;
	BYTE *pCanTxd;
	
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadFromCanTxdBuf",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanTxd = pCanPortData[portSort-1]->MonitorBuf.TxdBuffer;
	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = pCanTxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
		
	return wLen;
}

/*********************************************************************************************
 *
 * ��������      ��CANͨ�����ջ�����ָ��λ�ö�ȡһ�����ȵ����ݣ���ά���鿴
 *
 * ����˵��   	- portSort:	����	����˿��еĶ˿����   
 *				- pBuf	: 	���	��Ҫ��ȡ�����ݻ���	
 *				- wLen :	����	׼����ȡ�����ݳ���
 *				- wOffset:	����	�������ʼ��ַ��ƫ��
 *
 * ���ش���
 *                ʵ��д��������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD ReadFromCanRxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;
	BYTE *pCanRxd;

	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadFromCanRxdBuf",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanRxd = pCanPortData[portSort-1]->MonitorBuf.RxdBuffer;
	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = pCanRxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
	
	return wLen;
}

/*********************************************************************************************
 *
 * ��������      ��ȡ����дָ��֮ǰ�������ݣ���ά���鿴
 *
 * ����˵��      - pBuf	: 	���	��Ҫ��ȡ�����ݻ���	
 *				 - wLen :	����	׼����ȡ�����ݳ���
 *
 * ���ش���
 *                ʵ��д��������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD ReadNewCanTxdData(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i, wTxdWritePtr;
	BYTE *pCanTxd;
		
	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadFromCanRxdBuf",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanTxd = pCanPortData[portSort-1]->MonitorBuf.TxdBuffer;
	wTxdWritePtr =  pCanPortData[portSort-1]->MonitorBuf.wTxdWritePtr;
		
	if(wTxdWritePtr >= wOffset)
		wLeft = wTxdWritePtr - wOffset;
	else
		wLeft = CAN_MONITOR_BUF_LEN - (wOffset-wTxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
		
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = pCanTxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
	
	return wLeft;
}

/*********************************************************************************************
 *
 * ��������      ��ȡ����дָ��֮ǰ�������ݣ���ά���鿴
 *
 * ����˵��      - pBuf	: 	���	��Ҫ��ȡ�����ݻ���	
 *				 - wLen :	����	׼����ȡ�����ݳ���
 *
 * ���ش���
 *                ʵ��д��������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD ReadNewCanRxdData(WORD portSort,BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i, wRxdWritePtr;
	BYTE *pCanRxd;

	ASSERT(portSort>0);
	if((portSort == 0) ||(portSort>MAX_CAN_PORT))
	{
		LogError("ReadNewCanRxdData",FILE_LINE,"the port number invalide.");
		return 0;
	}

	pCanRxd = pCanPortData[portSort-1]->MonitorBuf.RxdBuffer;
	wRxdWritePtr =  pCanPortData[portSort-1]->MonitorBuf.wRxdWritePtr;

	if(wRxdWritePtr >= wOffset)
		wLeft = wRxdWritePtr - wOffset;
	else
		wLeft = CAN_MONITOR_BUF_LEN - (wOffset-wRxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
	
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = pCanRxd[(wOffset+i)%CAN_MONITOR_BUF_LEN];
	}
	
	return wLeft;
}


#define CAN_DEBUG
#ifdef CAN_DEBUG

extern DWORD dwSelfFramCnt;

BEGIN_MESSAGE_MAP(CCanTest,CApp)
	ON_SM_TIMEOUT(CCanTest)
	ON_SM_READCOMM(CCanTest)
	ON_SM_COMMIDLE(CCanTest)
END_MESSAGE_MAP()

void CanTestTask(DWORD *pdwApp);

void CanTestTask(DWORD *pdwApp)
{

	CCanTest *pCanTest = new CCanTest(pdwApp);
	pCanTest->Init();
	
	pCanTest->Run();

}

CCanTest::CCanTest(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CCanTest::Init(void)
{
	int i;
#ifdef 	CAN_MASTER
	ReportMsg("Master can task is running...");
#else
	ReportMsg("Slaver can task is running...");
#endif
	flag = 0;
	timecounter = 0;
	writecnt = readcnt = 0;

	port = SJA1000_PORT;
	
	for(i=0; i<sizeof(bTxdBuf); i++)
		bTxdBuf[i] = i;

	for(i=0; i<sizeof(bAnswerFram); i++)
		bAnswerFram[i] = 0x80+i;

	address = DB_GetDevAddress();
	
	SetTimer(1, 100);
}

void CCanTest::OnTimeOut(DWORD id)
{
	if(bCanLinkInit == TRUE)
	{
		if(flag == 0)
		{
			CanOpen(port, 0);
			flag = 0xff;
		}
#ifdef CAN_MASTER
		if(flag == 0xff)
		{
			CanWrite(port, bTxdBuf, 32, address);
			writecnt++;
		}
#endif			
	}

	timecounter++;
	if((timecounter%20) == 0)
	{
#ifdef CAN_MASTER
		ReportMsg("send fram counter=%d, txdokcnt=%d, txderrcnt=%d", writecnt,\
		pCanPortData[port-1]->CanStatic.dwApiTxdOkCnt,\
		pCanPortData[port-1]->CanStatic.dwApiTxdErrCnt);
#else
		ReportMsg("receive counter=%d, apirxdokcnt=%d, selffrm=%d, apirxderror=%d, ", readcnt, \
		pCanPortData[port-1]->CanStatic.dwApiRxdOkCnt,\
		dwSelfFramCnt, \
		pCanPortData[port-1]->CanStatic.dwApiRxdErrCnt);
#endif
	}
}

void CCanTest::OnReadComm(DWORD dwPort, DWORD dwFlag)
{
	DWORD len;

	readcnt++;
	
	len = CanRead(port, bRxdBuf, CAN_FRAME_BUF_LEN, 0);
	if(len)
		ReportMsg("receive frame len=%d", len);
	
#ifndef CAN_MASTER
	CanWrite(port, bAnswerFram, sizeof(bAnswerFram), address);
#endif	

}

void CCanTest::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	static DWORD idlecnt;

	idlecnt++;
}

#endif

