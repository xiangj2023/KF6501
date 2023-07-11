/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			ttydrv.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ��������ͨ�������ӿ�                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ��ɶ����д�������ͨ���Ĳ����ӿڷ�װ�������ڹ�������ʹ��                          
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	SerialInit			��������ͨ����ʼ��
*      	SerialOpen			��������ͨ����                                                              
*      	SerialRxdCtr	   	��������ͨ�����տ���                                                                         
*      	SerialTxdCtr    	��������ͨ�����Ϳ���                                                                        
*      	SerialIOCtl	    	��������ͨ��һ�������������                                                                       
*      	WriteAChar		 	�򴮿�����ͨ��д��һ���ֽڣ��Դ������ڷ����ж�                                                                       
*      	ResetComm		    ��λ��������ͨ��                                                                      
*      	GetSerialMap	   	ȡ��������ͨ����ӳ���                                                                        
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/7/9    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "ttydrv.h"
#include "misi.h"

#include "m5234uart.h"

//���ڹ��ܽӿں�����ʼ��
const TSerialFunc SerialFuncs[] =
{
#if (MCF5234_UART ==YES)    
    {M5234UartInit, M5234UartOpen, M5234UartRxd,  M5234UartTxd,
		M5234UartIOCtl, M5234UartWriteAChar},
#else
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL},
#endif  	
};

////#if(MODULE_TYPE== MD_KV5234)
/*�߼����ڵ������ڵ�ӳ��*/
TSerialMap SerialMap[MAX_SERIAL_NUM] =
{

    /* ���������,  ����ͨ����, ����оƬ��,  �ڱ��മ���е�ƫ��    */ 
	{ 0,			  0,			0,			0 },  /* ���� */
	{ M5234_UART,     0,			0,			0 },  /* �߼�����1, MCF5234�ĵ�0��UART */
	{ M5234_UART,     2,			0,			1 },  /* �߼�����2, MCF5234�ĵ�2��UART */
};
////#endif

/*********************************************************************************************
 *
 * ��������     ��������ͨ����ʼ��
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void SerialInit()
{
	for(int i=0; i<ArraySize(SerialFuncs); i++)
		if(SerialFuncs[i].pInit != NULL)
			SerialFuncs[i].pInit();
} 


/*********************************************************************************************
 *
 * ��������     ��������ͨ����
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD SerialOpen(WORD wPort)
{
	CSerial* pSerial;
	WORD nDrvNo;
	
	pSerial = GetSerial(wPort);
	
	if(pSerial == NULL)
		return ERR_SERIAL_CHANNEL;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialOpen",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrvNo = SerialMap[wPort].nDriver;
	
//	pSerial->m_wPortType = nDrvNo;
	if(SerialFuncs[nDrvNo].pOpen != NULL)
	  return SerialFuncs[nDrvNo].pOpen(wPort, &SerialMap[wPort]);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ��������ͨ�����տ���
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *				 - bFlag	: ����	���Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD SerialRxdCtr(WORD wPort, BYTE bFlag)
{
	int nDrvNo;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialRxdCtr",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrvNo = SerialMap[wPort].nDriver;

	if(SerialFuncs[nDrvNo].pRxdCtr != NULL)
		return SerialFuncs[nDrvNo].pRxdCtr(wPort, bFlag);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ��������ͨ�����Ϳ���
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *				 - bFlag	: ����	���Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD SerialTxdCtr(WORD wPort, BYTE bFlag)
{
	int nDrvNo;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialRxdCtr",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrvNo = SerialMap[wPort].nDriver;

	if(SerialFuncs[nDrvNo].pTxdCtr != NULL)
		return SerialFuncs[nDrvNo].pTxdCtr(wPort, bFlag);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ��������ͨ��һ������������ƣ���MODEM�źš�����ͨ�Ų�����
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *				 - Cmd		: ����	��������
 * 			     - pdwPar1	: |---MODEM_SET:		����	MODEM��������źű�־λ
 *							  |---MODEM_GET:		���	MODEM����״̬�ź�		
 *							  |---BAUD_SET:			����	׼��д��Ĳ�����ֵ		
 *							  |---BAUD_GET:			���	��ȡ�Ĳ�����ֵ	
 *							  |---COM_PAR_SET:		����	׼�����õĴ���ͨ�Ų�����־		
 *							  |---COM_PAR_GET:		���	������ͨ�Ų�����־		
 *							  |---COM_CHAN_RESET	��Ч		
 *							  |---COM_TXD_END		��Ч		
 *
 *				 - pdwPar2	: |---MODEM_SET:		����	MODEM��������źŸߵͱ�־
 *							  |---MODEM_GET:		��Ч		
 *							  |---BAUD_SET:			��Ч		
 *							  |---BAUD_GET:			��Ч
 *							  |---COM_PAR_SET:		��Ч		
 *							  |---COM_PAR_GET:		��Ч		
 *							  |---COM_CHAN_RESET	��Ч		
 *							  |---COM_TXD_END		��Ч		
 *
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL SerialIOCtl(WORD wPort, BYTE Cmd, DWORD *pdwPar1, DWORD *pdwPar2)
{
	int nDrv;

	if(wPort >= MAX_SERIAL_NUM)
	{
		LogError("SerialRxdCtr",FILE_LINE,"Port No Invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	nDrv = SerialMap[wPort].nDriver;
	return SerialFuncs[nDrv].pIOCtl(wPort, Cmd, pdwPar1, pdwPar2);	
}

/*********************************************************************************************
 *
 * ��������     �򴮿�����ͨ��д��һ���ֽڣ��Դ������ڷ����ж�
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *				 - bData	: ����	���͵��ֽ�����
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL  WriteAChar(WORD wPort, BYTE bData)
{
	WORD nDrv;

	if(wPort > MAX_SERIAL_NUM)
	{
		LogError("WriteAChar",FILE_LINE,"port no invalidate");
		return 0;
	}
	
	nDrv = SerialMap[wPort].nDriver;
	
	return SerialFuncs[nDrv].pWriteData(wPort, bData);	

}

/*********************************************************************************************
 *
 * ��������     ��λ��������ͨ��
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void ResetComm(WORD wPort)
{
	CSerial* pSerial;
	
	pSerial = GetSerial(wPort);
	
	if(pSerial == NULL)
		return;
	
	pSerial->ResetSerial(RESET_SERIAL);
}

/*********************************************************************************************
 *
 * ��������     ȡ��������ͨ����ӳ���
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *
 * ���ش���
 *                ����ͨ����ӳ���
 *
 * ����˵����          
 *
**********************************************************************************************/
TSerialMap GetSerialMap(WORD wPort)
{
	return SerialMap[wPort];
}

