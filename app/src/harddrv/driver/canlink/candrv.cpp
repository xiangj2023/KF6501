/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			canddrv.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           CAN�������ӿ�                                                                      
*			                                                                
* ����                                                                             
*                                                                                  
*       ��FLEXCAN��SJA1000��CANоƬ�������з�װ���ṩͳһ�Ĳ����ӿڡ�                      
*                                                                                        
* ����                                                                             
*                                                                                  
*      	CanHardInit			CAN��ʼ��
*      	CanHardOpen			CANӲ����                                                              
*      	CanHardWrite	   	CANӲ�����ݷ���                                                                         
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/9/7    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#if(MCF5234_SJA1000 == YES)
	#include "sja1000.h"
#endif

#if(MCF5235_FLEXCAN == YES)
	#include "flexcan.h"
#endif

#include "candrv.h"
#include "canlink.h"

//CAN�������ӿڳ�ʼ��
TCanDriver CanDriver[MAX_CAN_PORT] = 
{
#if(MCF5235_FLEXCAN == YES)
		{NULL,	FlexCanOpen,		NULL,	FlexCanWrite, FlexCanReset},
#else
				{NULL,			NULL,		NULL,	NULL, 	NULL		},
#endif

#if(MCF5234_SJA1000 == YES)
	{NULL, 	Sja1000Open,		NULL,	Sja1000Write, Sja1000Reset},
#else
			{NULL,			NULL,		NULL,	NULL,	 NULL		},
#endif
};

/*********************************************************************************************
 *
 * ��������     CAN��ʼ��
 *
 * ����˵��      - port		: ���� 	can���ڲ��˿����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanHardInit(WORD port)
{
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardInit",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pInit != NULL)
		return CanDriver[port].pInit(port);
	
	return ERR_FAILURE;
}

/*********************************************************************************************
 *
 * ��������     CANӲ����
 *
 * ����˵��      - port		: ���� 	can���ڲ��˿����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanHardOpen(WORD port)
{
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardOpen",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pOpen != NULL)
		return CanDriver[port].pOpen(port);

	return ERR_FAILURE;
}

/*********************************************************************************************
 *
 * ��������     CANӲ�����ݷ���
 *
 * ����˵��      - port		: ���� 	can���ڲ��˿����
 *				 - address	: ����	��ģ���ַ
 *				 - type		: ����	�����������ͣ���·�����Ӧ�ò�����
 *				 - isNewWrite	:����	�Ƿ�Ϊ�ط�����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD CanHardWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite)
{
	static WORD counter[MAX_CAN_PORT];
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardOpen",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pWrite == NULL)
		return 	ERR_FAILURE;

	//���CAN�˿ڷ����Ƿ����
	if(isCanPortTxdIdle(port) == FALSE)
	{
//		ReportMsg("the can%d txd busy, send failed.", port);
		counter[port]++;
		
		//�˿ڳ�ʱ�䷢�ͷ�æ�����������⣬���¸�λ
		if(counter[port]>CAN_HW_RESET_LEVEL)
		{
			counter[port]++;
			CanHardReset(port);
		}
		return ERR_FAILURE;
	}

	
	counter[port] = 0;

	return CanDriver[port].pWrite(port, address, type, isNewWrite);
}


DWORD CanHardReset(WORD port)
{
	ASSERT(port<MAX_CAN_PORT);
	if(port >= MAX_CAN_PORT)
	{
        LogError("CanHardReset",FILE_LINE,"the flexcan port invalide.");
		return ERR_FAILURE;
	}
	if(CanDriver[port].pReset != NULL)
		return CanDriver[port].pReset(port);

	return ERR_FAILURE;
}

