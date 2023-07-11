/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			misi.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           �����ػ��������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      �����ػ�������ɶ����д��ڵĹ�����Ϊ�˿ڲ��ṩ��Ӧ�ӿ�                          
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	MisiDaemon			�����ػ�������ں���
*      	MisiOnTimer			��������ʱ�������                                                              
*      	MisiInit	   		                                                                         
*      	MisiOpen    		���ڴ򿪣����˿ڲ�ʹ��                                                                        
*      	MisiCtrl	    	�������ã����˿ڲ�ʹ��                                                                       
*      	MisiWrite		 	�򴮿�д�����ݣ����˿ڲ�ʹ��                                                                       
*      	MisiRead		    �Ӵ��ڶ�ȡһ���������ݣ����˿ڲ�ʹ��                                                                      
*      	MisiReset		   	���ڸ�λ�����˿ڲ�ʹ��                                                                        
*      	GetPortSort	   		���ݶ˿ںŻ�ȡ�������                                                                        
*      	GetSortSerial	   	���ݴ�����Ż�ȡ��Ӧ����ͨ������ָ��                                                                        
*      	GetSerial		   	���ݶ˿ںŻ�ȡ��Ӧ����ͨ������ָ��                                                                        
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/7/7    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "misi.h"
#include "msgdefs.h"
#include "ttydrv.h"
#include "parafunc.h"

CSerial * m_pSerial = NULL;
DWORD m_dwMisiTaskID;
DWORD m_dwPortNum = 0;


/*********************************************************************************************
 *
 * ��������     �����ػ�������ں���
 *
 * ����˵��      - arg	: ���� 	������ڲ����������±������񴴽�ʱ����˳���Ӧ
 *
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void MisiDaemon(DWORD *arg)
{
	DWORD dwCount,dwNum;
	TMessage msg;
	TSerialPara	tSerialPara;
	TMBO_Disp	tMapBitObj;
	int		i,j;
	
	ASSERT(arg != NULL);
	if(arg == NULL)
	{
		LogError("MisiDaemon", FILE_LINE, "arg = NULL");
		OS_Exit(6);
	}
	//ע��argΪ���������ַ����һ������Ϊ����ID���ڶ�������Ϊ���ڸ���,�������񴴽�ʱ����˳��
	m_dwPortNum = DB_GetSerialNum();

	// ��Ӿ���λ����˿�
	for(i=0; i<DB_GetMapBitObjNum(); i++)
	{
		DB_GetMapBitObjDisp(i, &tMapBitObj);
		
		// ���ѳ�ʼ���Ķ˿����Ҳ���ʱ���ۼ�
		for(j=0; j<DB_GetSerialNum(); j++)
		{
			DB_GetSerialPara(j, &tSerialPara);
			if(tSerialPara.wPortID == DB_GetMBOPortID(&tMapBitObj))
				break;
		}
		
		if(j == DB_GetSerialNum())
			m_dwPortNum++;
	}
	
	if(m_dwPortNum == 0)
	{
		LogError("MisiDaemon", FILE_LINE, "dwPortNum = 0");
		return;
	}
		
	m_pSerial = new CSerial[m_dwPortNum];
	
	CSerial::ResetSerialMap();
	
	if(m_pSerial == NULL)
	{
		LogError("MisiDaemon",FILE_LINE,"m_pSerial=NULL");
		return;
	}

	MisiInit();
	SerialInit();

	m_dwMisiTaskID = arg[0];//GetAppID();

	////////////////////////
	OS_SetTimer(m_dwMisiTaskID,1,SERIAL_TASK_TIMER, NULL);
	////////////////////////
                        
    dwCount = 0;
    dwNum = 1000/SERIAL_TASK_TIMER;   
    
    ///////////////////////////////////////test
    //���ó�ʼ״̬Ϊ������Ϣ��ȷ
    DWORD status = ERR_MESSAGE_OK;
	OS_TaskDesp *pTask;
	pTask = OS_GetTaskTab(m_dwMisiTaskID);
    
    //ѭ�����ý�����Ϣ����,ֱ���յ������˳���Ϣ
    while(status != ERR_MESSAGE_QUIT)
    {
        //����ȡ��Ϣ����,��������Ϣ����ͷ,ȡ��Ҫ�������Ϣ
        status = OS_GetMessage(m_dwMisiTaskID,&msg);
        
        //�жϽ��յ���Ϣ�Ƿ�Ϊ��Ч��Ϣ
        if(status == ERR_MESSAGE_OK)
        {

			//����������м���
			if(pTask != NULL)
				pTask->Counter = 0;

       	    switch(LOWORD(msg.lParam))
       	    {
       	    	case    SM_TIMEOUT:
       	    		MisiOnTimer();
					break;
/*		    
			if( dwCount++ > dwNum )
			{
			        dwCount = 0;
			        PSOS_TaskRegister(0,NULL);
			}
*/			
	        default:
       	    		break;
       	    }
            //��Ϣ��Ч,������Ϣ���Ⱥ����Ը���Ϣ���е���
     
        }
    }    
	
}

/*********************************************************************************************
 *
 * ��������     ��������ʱ�������
 *
 * ����˵��      
 *
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void MisiOnTimer()
{
	for(int i=0; i<m_dwPortNum; i++)
		m_pSerial[i].MainTask();
}

/*********************************************************************************************
 *
 * ��������     ���ڳ�ʼ��
 *
 * ����˵��      
 *
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void MisiInit()
{
	TSerialPara 	SerialPar;
	TMBO_Disp		tMapBitObj;
	int 			i,j,m=DB_GetSerialNum();
	WORD			wMapPortID;
	
	for(i=0; i<DB_GetSerialNum(); i++)
	{
		if(DB_GetSerialPara(i, &SerialPar) != DB_ERR_OK)
			continue;

		// �����map�˿ڣ����վ���λ�����ʼ��
		if(DB_IsMapBitPort(SerialPar.wPortID))
		{
			DB_PortID2MapBitObjDisp(SerialPar.wPortID,&tMapBitObj);
			
			m_pSerial[i].m_dwPort = SerialPar.wPortID;
			m_pSerial[i].m_serialPara.wPortID	= SerialPar.wPortID;
			m_pSerial[i].m_serialPara.bFlag 	= 0;
			m_pSerial[i].m_serialPara.byDriver	= 0;	
			
			BaudToNum(tMapBitObj.dwBaud,&m_pSerial[i].m_serialPara.byBaud);
		}
		else
		{
			m_pSerial[i].m_dwPort = SerialPar.wPortID;
			m_pSerial[i].m_serialPara = SerialPar;
			m_pSerial[i].InitSerialMap();
		}
	}

	// ��Ӿ���λ����˿�
	for(i=0; i<DB_GetMapBitObjNum(); i++)
	{
		DB_GetMapBitObjDisp(i, &tMapBitObj);

		// ���ѳ�ʼ���Ķ˿����Ҳ���ʱ����ʼ��������˿�
		for(j=0; j<m_dwPortNum; j++)
			if(m_pSerial[j].m_dwPort == DB_GetMBOPortID(&tMapBitObj))
				break;

		if(j == m_dwPortNum)
		{
			m_pSerial[m].m_dwPort = DB_GetMBOPortID(&tMapBitObj);
			m_pSerial[m].m_serialPara.wPortID	= DB_GetMBOPortID(&tMapBitObj);
			m_pSerial[m].m_serialPara.bFlag		= 0;
			m_pSerial[m].m_serialPara.byDriver	= 0;	
			
			BaudToNum(tMapBitObj.dwBaud,&m_pSerial[m].m_serialPara.byBaud);

			m++;
		}
	}
}

/*********************************************************************************************
 *
 * ��������     ���ڴ򿪣����˿ڲ�ʹ��
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *				 - dwRxdLen : ����	Ӧ�ò������ձ��ĳ���
 *				 - dwTxdLen : ����	Ӧ�ò�����ͱ��ĳ���
 *				 - dwMode	: ����	���ò���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD  MisiOpen(DWORD dwPort, DWORD dwRxdLen, DWORD dwTxdLen, DWORD dwMode)
{
	int nNum;
	STATUS bRtn;
//	DWORD dwMask;
	
	int pn = MAX_SERIAL_NUM;
	if(dwPort >= pn)
		return ERR_FAILURE;
	
	nNum = GetPortSort(dwPort);
	
	if(nNum < 0 || nNum >= m_dwPortNum)
		return ERR_FAILURE;

//	dwMask = splx(MAX_ILEV);

	m_pSerial[nNum].m_dwTagetID = OS_GetTaskTab(SELF_TASK_ID)->dwTaskID;

	bRtn = m_pSerial[nNum].OpenComm(dwRxdLen, dwTxdLen);

//	(void)splx(dwMask);
	
	return bRtn;
}

/*********************************************************************************************
 *
 * ��������     �������ã����˿ڲ�ʹ��
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *				 - pCtrl 	: ����	�������ò���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD  MisiCtrl(DWORD dwPort , TCtlComm *pCtrl)
{
	int nNum;
	DWORD dwRtn;

	if(pCtrl == NULL || dwPort >= MAX_SERIAL_NUM)	
		return ERR_FAILURE;

	nNum = GetPortSort(dwPort);

	if(nNum < 0 || nNum > m_dwPortNum)
		return ERR_FAILURE;
	
	dwRtn =  m_pSerial[nNum].SerialControl(pCtrl);

	return dwRtn;
}

/*********************************************************************************************
 *
 * ��������     �򴮿�д�����ݣ����˿ڲ�ʹ��
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *				 - pBuf 	: ����	׼��д�������
 *				 - wLen		: ����	׼��д��������ֽڳ���
 *				 - dwFlag	: ����	���ò���
 *
 * ���ش���
 *                ʵ��д������ݳ���
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD MisiWrite(DWORD dwPort, BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	CSerial* pSerial;
	
	if(dwPort >= MAX_SERIAL_NUM)
		return 0;
			
	pSerial = GetSerial(dwPort);
	if(pSerial == NULL)
		return 0;
	
	
	return pSerial->WriteData(pBuf, wLen, dwFlag);
}

/*********************************************************************************************
 *
 * ��������     �Ӵ��ڶ�ȡһ���������ݣ����˿ڲ�ʹ��
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *				 - pBuf 	: ���	׼����ȡ������
 *				 - wLen		: ����	׼����ȡ�������ֽڳ���
 *				 - dwFlag	: ����	���ò���
 *
 * ���ش���
 *                ʵ�ʶ�ȡ�����ݳ���
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD MisiRead(DWORD dwPort, BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	CSerial* pSerial;

	if(dwPort >= MAX_SERIAL_NUM)
		return 0;
	
	pSerial = GetSerial(dwPort);
	
	if(pSerial == NULL)
		return 0;
	
	
	return pSerial->ReadData(pBuf, wLen, dwFlag);
}

/*********************************************************************************************
 *
 * ��������     ���ڸ�λ�����˿ڲ�ʹ��
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *				 - dwFlag 	: ���	��λ��־��Ϣ
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD MisiReset(DWORD dwPort, DWORD dwFlag)
{
	CSerial* pSerial;
	
	if(dwPort >= MAX_SERIAL_NUM)
		return ERR_FAILURE;
	
	pSerial = GetSerial(dwPort);
	
	if(pSerial == NULL)
		return ERR_FAILURE;
	
	pSerial->ResetSerial(dwFlag);
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ���ݶ˿ںŻ�ȡ�������
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *
 * ���ش���
 *                �������
 *
 * ����˵����          
 *
**********************************************************************************************/
int GetPortSort(DWORD dwPort)
{
	DWORD dwLPort;
	
	if(m_pSerial == NULL)
		return -1;

	for(int i = 0; i < m_dwPortNum; i++)
	{
		dwLPort = MINOR_PORT(m_pSerial[i].m_dwPort);
		if( dwLPort == dwPort)
			return i;
	}

	return -1;
}

/*********************************************************************************************
 *
 * ��������     ���ݴ�����Ż�ȡ��Ӧ����ͨ������ָ��
 *
 * ����˵��      - wNum	: ���� 	�������
 *
 * ���ش���
 *                ������ָ��
 *
 * ����˵����          
 *
**********************************************************************************************/
CSerial * GetSortSerial(WORD wNum)
{
	if(wNum >= m_dwPortNum)
		return NULL;
	return &m_pSerial[wNum];
}

/*********************************************************************************************
 *
 * ��������     ���ݶ˿ںŻ�ȡ��Ӧ����ͨ������ָ��
 *
 * ����˵��      - dwPort	: ���� 	�˿ں�
 *
 * ���ش���
 *                ������ָ��
 *
 * ����˵����          
 *
**********************************************************************************************/
CSerial * GetSerial(DWORD dwPort)
{
	int nNum;

	nNum = GetPortSort(dwPort);
	if(nNum < 0 || nNum > m_dwPortNum)
		return NULL;

	return &m_pSerial[nNum];
}


