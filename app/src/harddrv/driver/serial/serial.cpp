/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			serial.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           �������������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ʵ�ִ�����������ܣ�ͨ�����ڹ���������ɶԴ������ݵĹ���Ϳ���                          
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	CSerial				������
*      	MainTask			��������������ɴ����շ����ݵĹ���                                                              
*      	CheckTxdTask	   	���ͼ������                                                                         
*      	DetectFrameTailTask	����֡�������                                                                        
*      	GetLeftRxdNum    	ȡ��������ջ������ַ���                                                                       
*      	NotifyTask		 	����������ͨ����ͨ������������������֪ͨ��Ӧ��Ӧ�ò�����                                                                       
*      	MyTimerTask		    �������ڲ���ʱ�������                                                                      
*      	MySetTimer		   	�������ڲ���ʱ������                                                                        
*      	MyKillTimer			�������ڲ���ʱ��ɾ��                                                                      
*      	MyOnTimer       	�������ڲ���ʱ������Ҫ��ɴ��ڿ����źŵ������ʱ���Ʋ���                                                                     
*      	GetCanSendNum       ������ͨ�����ͻ����������򴮿�����ͨ�����͵��ֽڸ���                                                                     
*		SetWriteMask		�ۼӷ��Ϳ��м�����ͨ��ͨ�ſ��м���
*		ClearWriteMask		������Ϳ��м�����ͨ��ͨ�ſ��м���
*		SetReadMask			�ۼӽ��տ��м�����ͨ��ͨ�ſ��м���
*		ClearReadMask		������տ��м�����ͨ��ͨ�ſ��м���
*		ClearFameTail		�����֡β�жϱ�־
*		GetCanWriteNum		ȡ�����򴮿�ͨ�����ͻ�����д����ֽڸ���
*		NotifyToUse			��Ϣ֪ͨ����
*		IsOpen				ȡ����ͨ���򿪱�־
*		OpenComm			����ͨ����
*		SerialControl		���ô��ڿ�����Ϣ
*		InitPara			��ʼ��������ͨ�Ų������紮�����͡�������Ƶ�
*		GetBaud				����ͨ��������Ż�ȡ����ͨ�����ʣ���������ս������ʱ�䳤��
*		GetRtsDelay			���ݲ�������RTS���������ʱʱ��
*		SerialTxdCtr		����ͨ�����Ϳ���
*		ReadData			��ȡ����ͨ�����ջ������ڽ��յ���ָ���������ݣ�һ�㹩�ϲ�Ӧ�õ���
*		WriteData			�򴮿�ͨ�����ͻ�����д��һ�����ȵ����ݣ�һ�㹩�ϲ�Ӧ�õ���
*		WriteToBuf			�򴮿�ͨ�����ջ�����д��һ�����ȵ����ݣ�һ�㹩�ײ���������
*		ReadFromBuf			�Ӵ���ͨ�����ͻ�������ȡһ�����ȵ����ݣ�һ�㹩�ײ���������
*		WriteAChar			���Ӧ��������ͨ��д��һ���ֽڵķ������ݣ�һ�㹩�������������Դ��������ж�
*		ResetSerial			���ݱ�־��λ����ͨ���࣬����Ϊ���ͻ����������ջ���������������ͨ��
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/07/02    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "serial.h"
#include "ttydrv.h"
#include "misi.h"


CSerial* CSerial::m_pSerialMap[MAX_SERIAL_NUM];
 
 /*********************************************************************************************
  *
  * ��������	 �������ʼ��
  *
  * ����˵��	  
  *
  * ���ش���
  * 			   
  *
  * ����˵����			
  *
 **********************************************************************************************/
CSerial::CSerial()
{
	//��ʼ��������ָ��
	m_wRxdWritePtr = 0;
	m_wRxdReadPtr = 0;
	m_wTxdWritePtr = 0;
	m_wTxdReadPtr = 0;
	m_bRxdTail = 0;
		
	m_dwNoRxdTime = 0;
	m_dwBaud = 1200;
//	m_wPortType = 0;
	
	m_bTxdMode = 0;
	m_bRxdMode = 0;
	m_bTxdOnOff = FALSE;
	m_dwNoRecive = 0;
	m_dwNoSend = 0; 
//	m_wResetCnt = 0;
	
	timerStatus = NONEED;
	
	m_dwFlag = 0;
		
	m_dwMaxNoRxdTime = 100/SERIAL_TASK_TIMER;
		
	if(m_dwMaxNoRxdTime == 0)
		m_dwMaxNoRxdTime = 1;
	
	//	memset((void*)&m_serialPara, 0, sizeof(pSerial->m_serialPara));
	memset((void*)&m_CommCtrl, 0, sizeof(m_CommCtrl));
	memset((void*)&m_PortInfo, 0, sizeof(m_PortInfo));
		
		
	m_bOpend = FALSE;
	
	m_TxdBuf = NULL;
	m_RxdBuf = NULL;
	m_wMaxTxdLen = MISITXDBUFLEN;
	m_wMaxRxdLen = MISIRXDBUFLEN;
		
	m_TxdBuf = (BYTE *)OS_MemAlloc(m_wMaxTxdLen);
	m_RxdBuf = (BYTE *)OS_MemAlloc(m_wMaxRxdLen);
	
	m_CommCtrl.dwNotifyRead = m_wMaxRxdLen/2;////��֪ͨ�ַ�����
	
	m_bWriteContinue = FALSE;
	m_bNotify = FALSE;
	
	m_bNeedMessage = FALSE;
	
	m_dwNoReadCount = 0;

	m_dwFECnt = 0;
	m_dwPECnt = 0;
	m_dwOECnt =0;

	m_dwCounter = 0;
}

CSerial::~CSerial()
{
}

/*********************************************************************************************
 *
 * ��������      ��������������ɴ����շ����ݵĹ���
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
void CSerial::MainTask()
{
	//���ô����Ƿ��
	if(m_bOpend == FALSE)
		return;
	
	CheckTxdTask();
	
	MyTimerTask();////��ʱ����
	DetectFrameTailTask();	
	NotifyTask();//////������д����ʱ��

	m_dwCounter++;

//	if((m_dwCounter%100)==0)
//		ReportMsg("The Misi task counter=%d. ", m_dwCounter);
}

/*********************************************************************************************
 *
 * ��������      ���ͼ�����񣬵����ͻ�����������ʱ���ڶ�Ӧ�������������´������ͣ�����Ҫ���
 *				 ��ʽ����RTS�ź�ʱ���RTS�źŵĴ���ʱ����
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
void CSerial::CheckTxdTask()
{
	WORD 	wTxdNum;
    
	wTxdNum = GetCanSendNum();

	if(wTxdNum)
	{
		if(m_bTxdOnOff == FALSE)
		{
//			m_wResetCnt = 0;
			if(m_bNeedCtrlRTS)
			{
				if(timerStatus == NONEED)
				{
				    if(m_wRTSNoDelay)
				    {
    				    if(m_bRTSLogicHigh)
        					//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, FALSE);
						 	SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
        				else
    					    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, TRUE);
							SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
    			    }
    			    else
    			    {
    				    if(m_bRTSLogicHigh)
        					//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, FALSE);
							SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
        				else
    					    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, TRUE);
							SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
                    }    			    
					MySetTimer(RTSTIMERID, m_wRTSOnDelay);
					timerStatus = WAITENABLETXD;
				}//pSerial->timerStatus = == NONEED		
			}
			else
				SerialTxdCtr(TRUE);/////дһ���ַ������ж�
		}//m_bTxdOnOff == FALSE
		else
		{
/*		ע��˴��Ĵ���ʽ�������ͻ����������ݵ��ǳ�ʱ�䲻�ܻ�ȡ���ڷ�����Դʱ			
			m_wResetCnt++;
			if(m_wResetCnt>=(10000/SERIAL_TASK_TIMER))
			{
				BYTE bIntReg;
				bIntReg = inportb(0x83000c00+0x01);	
				
//				SerialResetSerialBase(pSerial);
//				SerialInit();
				SerialOpen(m_dwPort&0x000F);
				SerialRxdCtr(m_dwPort&0x000F, TRUE);
//				SerialTxdCtr(m_dwPort&0x000F, TRUE);
				m_bTxdOnOff = FALSE;
			}
*/			
			if(m_bNeedCtrlRTS)////���ڵȴ��ر�RTSʱ���������ݷ���Ӧ���رն�ʱ��
			{
				if(timerStatus == WAITOFFRTS)
					MyKillTimer(RTSTIMERID);
			}
			
		}
	}
	else
	{
		if(m_bNeedCtrlRTS)
		{
		    BOOL bRtsL = FALSE;
		
		    bRtsL = (m_bTxdMode & MODEM_RTS);
		    if(m_bRTSLogicHigh)
		        bRtsL = !bRtsL;
			if((timerStatus == NONEED) && bRtsL && (IsTxdEnd() == TRUE))
			{
				MySetTimer(RTSTIMERID, m_wRTSOffDelay-1);
				timerStatus = WAITOFFRTS;
			}
		}
	}	
}

/*********************************************************************************************
 *
 * ��������      ����֡�������
 *
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
void CSerial:: DetectFrameTailTask()
{
	BOOL bRxdTail;

	m_dwNoRxdTime++;

	bRxdTail = (m_dwNoRxdTime >= m_dwMaxNoRxdTime);
	bRxdTail &= (GetLeftRxdNum() > 0);

	m_bRxdTail = bRxdTail;
}


/*********************************************************************************************
 *
 * ��������      ȡ��������ջ������ַ���
 *
 *
 * ����˵��      
 *       		 
 *
 * ���ش���
 *                �ַ�����
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD CSerial::GetLeftRxdNum()/////��Լ���Դӻ����������ַ���
{
	WORD wLen;
	WORD wRxdWritePtr;
	WORD wRxdReadPtr;
	
//	OS_Protect(0);
	wRxdWritePtr = m_wRxdWritePtr;	/////��Դ����,��ֹ�ڽ������в���ʱ��ָ��仯
	wRxdReadPtr = m_wRxdReadPtr;	///////
//	OS_UnProtect(0);

	if(wRxdWritePtr >= wRxdReadPtr)
		wLen = wRxdWritePtr - wRxdReadPtr;
	else
		wLen = m_wMaxRxdLen - (wRxdReadPtr-wRxdWritePtr);

	return wLen;
}

/*********************************************************************************************
 *
 * ��������      ����������ͨ����ͨ������������������֪ͨ��Ӧ��Ӧ�ò�����
 *
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
void CSerial::NotifyTask()
{
	BOOL bTxdBreak;
	BOOL bCommBreak;
	BOOL bWriteNotify;
	BOOL bNotify;
	BOOL bReadNotify;
	DWORD dwTime;
	DWORD dwFlag;
	
	if(m_bNeedMessage == FALSE)//////����Ҫ֪ͨ������Ϣ
		return;
	
	SetReadMask();/////��ͨ������ʱ��++
	SetWriteMask();////дͨ������ʱ��++

	if(m_bWriteContinue && GetCanWriteNum())
	{
		m_bWriteContinue = FALSE;
		NotifyToUse(SM_WRITECONT, 0, 0);
	}

	bTxdBreak = (m_CommCtrl.dwNotifyTxdBreak > 0);
	dwTime = notifyCount.dwNotifyTxdBreak * SERIAL_TASK_TIMER;
	bTxdBreak &= (dwTime >= m_CommCtrl.dwNotifyTxdBreak);

	bCommBreak = (m_CommCtrl.dwNotifyCommBreak > 0);
	dwTime = notifyCount.dwNotifyCommBreak * SERIAL_TASK_TIMER/2;
	bCommBreak &= (dwTime >= m_CommCtrl.dwNotifyCommBreak);

	bWriteNotify = (m_CommCtrl.dwNotifyWrite > 0);
	bWriteNotify &= (GetCanSendNum() <= m_CommCtrl.dwNotifyWrite);

	bReadNotify = (m_CommCtrl.dwNotifyRead > 0);
	bReadNotify &= (GetLeftRxdNum() >= m_CommCtrl.dwNotifyRead);
	bReadNotify |= m_bRxdTail;///֡β

	bNotify = (bTxdBreak | bCommBreak | bWriteNotify | bReadNotify);
	
	if(!bNotify)
		return;

	if(m_bNotify)
	{
		if(bReadNotify)
		{
			
			if(m_bRxdTail)
				dwFlag = RCF_FRAME_TAIL;
			else
				dwFlag = 0;

			//GPRS�յ�����ʱ,������߼���
			NotifyToUse(SM_READCOMM, 0, dwFlag);
			ClearFameTail();
		}
		else if(bCommBreak)
		{
			NotifyToUse(SM_COMMIDLE, 0, 0);
			ClearReadMask();
			notifyCount.dwNotifyCommBreak = 0;
		}
		else if(bTxdBreak)
		{
			NotifyToUse(SM_COMMTXDIDLE, 0, 0);
			notifyCount.dwNotifyTxdBreak = 0;
		}
		else if(bWriteNotify)
		{
			NotifyToUse(SM_WRITECOMM, 0, 0);
		}
		
	}
	else
	{
		m_dwNoReadCount++;

		ClearReadMask();
		ClearWriteMask();
		ClearFameTail();

//		if(m_dwNoReadCount >= NOREADCOUNT)
		{
			m_dwNoReadCount = 0;
			m_bNotify = TRUE;
		}
	}
}

/*********************************************************************************************
 *
 * ��������      �������ڲ���ʱ�������
 *
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
void CSerial::MyTimerTask()
{
	BYTE i;
	for(i = 0; i < TIMERSUM; i++)
	{
		if(timeMsg[i].bTimer)
		{
			timeMsg[i].wValue --;

			if(timeMsg[i].wValue == 0)
			{
				timeMsg[i].wValue = timeMsg[i].wTick;
				MyOnTimer(i+1);
			}
		}
	}
}

/*********************************************************************************************
 *
 * ��������      �������ڲ���ʱ������
 *
 *
 * ����˵��      - bTimerID	: ����	�������ڲ���ʱ��ID  
 *       		 - offset	: ����	��ʱʱ����(��λ����)
 *       		 
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL CSerial::MySetTimer(BYTE bTimerID, WORD wMSecond)
{
	if(bTimerID > TIMERSUM || bTimerID == 0)
		return FALSE;

	timeMsg[bTimerID-1].bTimer = TRUE;
	timeMsg[bTimerID-1].wTick = wMSecond/SERIAL_TASK_TIMER;
//	if(pSerial->timeMsg[bTimerID-1].wTick == 0)
//		pSerial->timeMsg[bTimerID-1].wTick = 1;
	//ע����������ʱ�������ϼ�ⶨʱ������˵�һ���������壬��������һ������ֵ����������ʱ��һ��
	timeMsg[bTimerID-1].wTick += 1;
	timeMsg[bTimerID-1].wValue = timeMsg[bTimerID-1].wTick;

	return TRUE;
}

/*********************************************************************************************
 *
 * ��������      �������ڲ���ʱ��ɾ��
 *
 *
 * ����˵��      - bTimerID	: ����	�������ڲ���ʱ��ID  
 *       		 
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL CSerial::MyKillTimer(BYTE bTimerID)
{
	if(bTimerID > TIMERSUM || bTimerID == 0)
		return FALSE;

	timeMsg[bTimerID-1].bTimer = FALSE;
	timeMsg[bTimerID-1].wTick = 0;
	timeMsg[bTimerID-1].wValue = 0;

	return TRUE;
}

/*********************************************************************************************
 *
 * ��������      �������ڲ���ʱ������Ҫ��ɴ��ڿ����źŵ������ʱ����
 *
 *
 * ����˵��      - bTimerID	: ����	�������ڲ���ʱ��ID  
 *       		 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::MyOnTimer(BYTE bTimerID)
{
	MyKillTimer(bTimerID);

	if(bTimerID == RTSTIMERID)
	{

		if(timerStatus == WAITENABLETXD)
			SerialTxdCtr(TRUE);

		if(timerStatus == WAITOFFRTS)
		{
		    if(m_wRTSNoDelay)
		    {
    		    if(m_bRTSLogicHigh)
    				//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,TRUE);
					SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
    			else
    			    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,FALSE);
					SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
    	    }
    	    else
    	    {
/*      	    //ԭ�����Ƿ�������
    		    if(m_bRTSLogicHigh)
    				SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,FALSE);
    			else
    			    SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,TRUE);
*/
 			    
    		    if(m_bRTSLogicHigh)
    				//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,TRUE);
					SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
    			else
    			    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,FALSE );
				SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
    	    }
    	    m_dwFlag = 0;
	    }
	}

	timerStatus = NONEED;
}

/*********************************************************************************************
 *
 * ��������      ������ͨ�����ͻ����������򴮿�����ͨ�����͵��ֽڸ���
 *
 *
 * ����˵��       
 *       		 
 *
 * ���ش���
 *                �����͵��ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD CSerial::GetCanSendNum()/////д���������ַ�����,
{
	WORD wLen;
	WORD wTxdWritePtr;
	WORD wTxdReadPtr;
	
	//�ٽ���Դ����(�ж��в�����)
//	OS_Protect(0);
	wTxdWritePtr = m_wTxdWritePtr;
	wTxdReadPtr = m_wTxdReadPtr;
//	OS_UnProtect(0);

	wLen = (wTxdWritePtr + m_wMaxTxdLen - wTxdReadPtr)%m_wMaxTxdLen;

	return wLen;
}

/*********************************************************************************************
 *
 * ��������      �ۼӷ��Ϳ��м�����ͨ��ͨ�ſ��м���
 *
 * ����˵��       
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::SetWriteMask()
{
	notifyCount.dwNotifyTxdBreak ++;
	notifyCount.dwNotifyCommBreak ++;
}

/*********************************************************************************************
 *
 * ��������      ������Ϳ��м�����ͨ��ͨ�ſ��м���
 *
 * ����˵��       
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::ClearWriteMask()
{
	notifyCount.dwNotifyTxdBreak = 0;
	notifyCount.dwNotifyCommBreak = 0;
}

/*********************************************************************************************
 *
 * ��������      �ۼӽ��տ��м�����ͨ��ͨ�ſ��м���
 *
 * ����˵��       
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::SetReadMask()
{
	notifyCount.dwNotifyRxdBreak ++;
	notifyCount.dwNotifyCommBreak ++;
}

/*********************************************************************************************
 *
 * ��������      ������տ��м�����ͨ��ͨ�ſ��м���
 *
 * ����˵��       
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::ClearReadMask()
{
	notifyCount.dwNotifyRxdBreak = 0;
	notifyCount.dwNotifyCommBreak = 0;
}

/*********************************************************************************************
 *
 * ��������      �����֡β�жϱ�־
 *
 * ����˵��       
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::ClearFameTail()
{
	m_dwNoRxdTime = 0;
	m_bRxdTail = FALSE;
}

/*********************************************************************************************
 *
 * ��������      ȡ�����򴮿�ͨ�����ͻ�����д����ֽڸ���
 *
 * ����˵��       
 *
 * ���ش���
 *                ����д����ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD CSerial::GetCanWriteNum()
{
	WORD wLen;
	WORD wTxdWritePtr;
	WORD wTxdReadPtr;

	//�ٽ���Դ����(�ж��в�����)ע��ú��������ж��е��ã���˲�����OS_Protect���б���
//	OS_Protect(0);
	wTxdWritePtr = m_wTxdWritePtr;
	wTxdReadPtr = m_wTxdReadPtr;
//	OS_UnProtect(0);

	wLen = (m_wMaxTxdLen + wTxdReadPtr - wTxdWritePtr)%m_wMaxTxdLen;

	if(wLen == 0)
		return m_wMaxTxdLen-1;
	return wLen-1;
}

/*********************************************************************************************
 *
 * ��������      ��Ϣ֪ͨ����
 *
 * ����˵��      - wMsg	: 	����	��Ϣ����  
 *				 - wLen	: 	����	
 *				 - dwFlag:	����	��־��Ϣ
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
STATUS CSerial::NotifyToUse(WORD wMsg, WORD wLen, DWORD dwFlag)
{
	if(m_dwTagetID == 0)
		return ERR_FAILURE;

	m_dwNoReadCount = 0;
	m_bNotify = FALSE;
	
	wLen = 0;
	dwFlag = 0;
	
//	if(pSerial->m_TagetID)
//		OS_PostMessage(pSerial->m_TagetID, wMsg, MAKELONG(wLen, pSerial->m_wPortID), dwFlag,0, 0);
	if(m_dwTagetID)
		OS_PostMessage(m_dwTagetID, wMsg, wLen, m_dwPort, dwFlag, 0);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������      ȡ����ͨ���򿪱�־
 *
 * ����˵��      
 *
 * ���ش���
 *                ���ڴ�״̬
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL CSerial::IsOpen()
{
	return m_bOpend;
}

/*********************************************************************************************
 *
 * ��������      ����ͨ����
 *
 * ����˵��      - wRxdLen	: 	����	Ӧ�ò���ջ���������
 *				 - wTxdLen	: 	����	Ӧ�ò㷢�ͻ���������	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
STATUS CSerial::OpenComm(WORD wRxdLen, WORD wTxdLen)
{
//	DWORD dwVarName;
	WORD wPortNo;
	
	InitPara();

//	dwVarName = pSerial->m_dwPort&0xFFFF0000;
//	dwVarName += ((pSerial->m_dwPort&0xFF)<<8);

	if(wRxdLen == 0 || wRxdLen > MISIRXDBUFLEN)
	{
		m_bOpend = FALSE;
		return ERR_FAILURE;
	}
	if(wTxdLen == 0 || wTxdLen > MISITXDBUFLEN)
	{
		m_bOpend = FALSE;
		return ERR_FAILURE;
	}

	wPortNo = MINOR_PORT(m_dwPort);

	SerialOpen(wPortNo);
	SerialRxdCtr(wPortNo, TRUE);
	::SerialTxdCtr(wPortNo, TRUE);

	OS_Sleep(10);
	
	m_bOpend = TRUE;

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������      ���ô��ڿ�����Ϣ
 *
 * ����˵��      - pCtrl	: 	����	������Ϣ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
STATUS CSerial::SerialControl(TCtlComm* pCtrl)
{
	if(pCtrl == NULL)
		return ERR_FAILURE;
	
	if(pCtrl->dwFuncCode == FC_GET)
	{
		*pCtrl = m_CommCtrl;
		return ERR_SUCCESS;
	}
	
	if(pCtrl->dwFuncCode != FC_SET)
		return ERR_FAILURE;
	
	m_bNeedMessage = TRUE;
	
	m_CommCtrl = *pCtrl;

	if(m_CommCtrl.dwNotifyWrite > m_wMaxTxdLen)
		m_CommCtrl.dwNotifyWrite = m_wMaxTxdLen;

	if(m_CommCtrl.dwNotifyRead > m_wMaxRxdLen)
		m_CommCtrl.dwNotifyRead = m_wMaxRxdLen/2;
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������      ��ʼ��������ͨ�Ų������紮�����͡�������Ƶ�
 *
 * ����˵��      
 *
 * ���ش���
 *                
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::InitPara()
{
	BYTE bMedium;

	GetBaud();
	GetRtsDelay();

	bMedium = m_serialPara.bFlag;

	//����485��ʽ����Ӳ���Զ�����RTS��ʽ
	if(bMedium & SERIAL_ATTR_RTSCTR)
		m_bNeedCtrlRTS = TRUE;
	else
		m_bNeedCtrlRTS = FALSE;
	
	if(bMedium & SERIAL_ATTR_RTSDELAY)
	    m_wRTSNoDelay = TRUE;
	else
	    m_wRTSNoDelay = FALSE;
	
	m_bRTSLogicHigh = FALSE;
    
}

/*********************************************************************************************
 *
 * ��������      ����ͨ��������Ż�ȡ����ͨ�����ʣ���������ս������ʱ�䳤��
 *
 * ����˵��      
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::GetBaud()
{
	DWORD dwOneTime;

	NumToBaud((WORD)m_serialPara.byBaud, &m_dwBaud);
	dwOneTime = 10*1000/m_dwBaud;

	m_dwMaxNoRxdTime = NOHCARNUM*dwOneTime/SERIAL_TASK_TIMER;

	if(m_dwMaxNoRxdTime < 2)
		m_dwMaxNoRxdTime = 2;
}

/*********************************************************************************************
 *
 * ��������      ���ݲ�������RTS���������ʱʱ��
 *
 * ����˵��      
 *
 * ���ش���
 *               �� 
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::GetRtsDelay()
{
	m_wRTSOnDelay = m_serialPara.byRTSOnDelay;
	m_wRTSOffDelay = m_serialPara.byRTSOffDelay;

	m_wRTSOnDelay *= 10;
	m_wRTSOffDelay *= 10;
	
	//��С����Ϊ���ڹ���ʱ���Ķ�ʱ���
	//if(m_wRTSOnDelay < SERIAL_TASK_TIMER)
		m_wRTSOnDelay = SERIAL_TASK_TIMER;
	//if(m_wRTSOffDelay < SERIAL_TASK_TIMER)
		m_wRTSOffDelay = SERIAL_TASK_TIMER;
}

/*********************************************************************************************
 *
 * ��������      ����ͨ�����Ϳ���
 *
 * ����˵��      - bOnOff	: 	����	������Ϣ	
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::SerialTxdCtr(BOOL bOnOff)
{
	if(::SerialTxdCtr(MINOR_PORT(m_dwPort), bOnOff) == ERR_SUCCESS)
		m_bTxdOnOff = bOnOff;
		
}

/*********************************************************************************************
 *
 * ��������      ��ȡ����ͨ�����ջ������ڽ��յ���ָ���������ݣ�һ�㹩�ϲ�Ӧ�õ���
 *
 * ����˵��      - pBuf		: 	���	��Ҫ����Ķ�ȡ��������	
 *				 - wLen		:	����	׼����ȡ�����ݳ���
 *				 - dwFlag	:	����	��־
 *
 * ���ش���
 *                ʵ�ʶ�ȡ���������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD CSerial::ReadData(BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	WORD wLeft;
	WORD i;
	
	if(!m_bOpend)
		return 0;

	m_bNotify = TRUE;
	m_dwNoReadCount = 0;

	wLeft = GetLeftRxdNum();

	if(wLen<wLeft)
		wLeft = wLen;

	if(wLeft == 0 || pBuf == NULL)
		return 0;

//	OS_Protect(0);
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_RxdBuf[m_wRxdReadPtr];
		m_wRxdReadPtr +=1;
		m_wRxdReadPtr %= m_wMaxRxdLen;
	}
//	OS_UnProtect(0);

	return wLeft;
}

/*********************************************************************************************
 *
 * ��������      �򴮿�ͨ�����ͻ�����д��һ�����ȵ����ݣ�һ�㹩�ϲ�Ӧ�õ���
 *
 * ����˵��      - pBuf		: 	���	��Ҫд�������	
 *				 - wLen		:	����	׼��д������ݳ���
 *				 - dwFlag	:	����	��־
 *
 * ���ش���
 *                ʵ��д��������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD CSerial::WriteData(BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	WORD wLeft;
	WORD i;
	
	if(!m_bOpend)
		return 0;

	m_bWriteContinue = FALSE;
	m_dwNoReadCount = 0;
	m_bNotify = TRUE;
	notifyCount.dwNotifyCommBreak = 0;
	
	m_dwFlag = dwFlag;		//Ӧ�ò����ݰ��Ƿ�һ֡����
	
	wLeft = GetCanWriteNum();

	if(wLen > wLeft)
	{
		wLen = wLeft;
		m_bWriteContinue = TRUE;
	}

	if(wLen == 0 || pBuf == NULL)
		return 0;

	OS_Protect(0);
	for(i = 0; i< wLen; i++)
	{
		m_TxdBuf[m_wTxdWritePtr] = pBuf[i];
		m_wTxdWritePtr +=1;
		m_wTxdWritePtr %= m_wMaxTxdLen;
	}
	OS_UnProtect(0);

	m_bTxdOnOff = FALSE;
		
	return wLeft;
}

/*********************************************************************************************
 *
 * ��������      �򴮿�ͨ�����ջ�����д��һ�����ȵ����ݣ�һ�㹩�ײ���������
 *
 * ����˵��      - pBuf	: 	���	��Ҫд�������	
 *				 - wLen :	����	׼��д������ݳ���
 *
 * ���ش���
 *                ʵ��д��������ֽڸ���
 *
 * ����˵����          
 *
**********************************************************************************************/
WORD CSerial::WriteToBuf(BYTE* pBuf, WORD wLen)
{
	DWORD dwRxdWritePtr;
	WORD i;

	ClearReadMask();
	ClearFameTail();
	
	//OS_Protect(0);
	m_dwNoRecive = 0;/////�յ��ַ��б�������
	
	dwRxdWritePtr = m_wRxdWritePtr;
	
	for(i=0; i<wLen; i++)
	{
		m_RxdBuf[dwRxdWritePtr] = pBuf[i];
		dwRxdWritePtr = (dwRxdWritePtr+1)%m_wMaxRxdLen;
	}
	
	m_wRxdWritePtr = dwRxdWritePtr;
	//OS_UnProtect(0);
	
	return wLen;
}

/*********************************************************************************************
 *
 * ��������      �Ӵ���ͨ�����ͻ�������ȡһ�����ȵ����ݣ�һ�㹩�ײ���������
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
WORD CSerial::ReadFromBuf(BYTE* pBuf, WORD wLen)
{
	WORD i;
	WORD wNum;
	DWORD dwTxdReadPtr;

	wNum = GetCanSendNum();
 	
 	if(wNum == 0)
 		return 0;
 		
 	if(wLen > wNum)
 		wLen = wNum;
 	
	ClearWriteMask();
	
	//OS_Protect(0);
	m_dwNoRecive = 0;
	m_dwNoSend = 0;
	
	dwTxdReadPtr = m_wTxdReadPtr;
	
	for(i=0; i<wLen; i++)
	{
		pBuf[i] = m_TxdBuf[dwTxdReadPtr];
	
		dwTxdReadPtr ++;
		dwTxdReadPtr %= m_wMaxTxdLen;
	}
	
	m_wTxdReadPtr = dwTxdReadPtr;
	//OS_UnProtect(0);
	
	return wLen;
}

/*********************************************************************************************
 *
 * ��������      ���Ӧ��������ͨ��д��һ���ֽڵķ������ݣ�һ�㹩�������������Դ��������ж�
 *
 * ����˵��      	
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::WriteAChar()
{
	BYTE bData;
	
	if(ReadFromBuf(&bData, 1))
		::WriteAChar(MINOR_PORT(m_dwPort), bData);
}

/*********************************************************************************************
 *
 * ��������      ���ݱ�־��λ����ͨ���࣬����Ϊ���ͻ����������ջ���������������ͨ��
 *
 * ����˵�� 	 - dwFlag	:	����	��λ���ͱ�־     	
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::ResetSerial(DWORD dwFlag)
{
	if(dwFlag&RESET_RXDBUF)///��λ���ջ�����
	{
		m_wRxdWritePtr = 0;
		m_wRxdReadPtr = 0;		
		m_bRxdTail = 0;
		m_dwNoRxdTime = 0;
		m_dwNoReadCount = 0;
	}
	if(dwFlag&RESET_TXDBUF)
	{
		m_wRxdWritePtr = 0;
		m_wRxdReadPtr = 0;
		m_wTxdWritePtr = 0;
		m_wTxdReadPtr = 0;

		m_bRxdTail = 0;
		m_dwNoRxdTime = 0;

		m_dwNoSend = 0;

		m_bTxdOnOff = FALSE;
	
		m_bWriteContinue = FALSE;
		m_bNotify = FALSE;
	}
	if(dwFlag&RESET_SERIAL)
		ResetSerial();
}

/*********************************************************************************************
 *
 * ��������      ��λ����ͨ���࣬���رմ�������ͨ���ķ��ͺͽ���
 *
 * ����˵�� 	  	
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CSerial::ResetSerial()
{		
	SerialOpen(MINOR_PORT(m_dwPort));

	m_wRxdWritePtr = 0;
	m_wRxdReadPtr = 0;
	m_wTxdWritePtr = 0;
	m_wTxdReadPtr = 0;

	m_bRxdTail = 0;
	m_dwNoRxdTime = 0;

	m_bTxdMode = 0;
	m_bRxdMode = 0;

	m_dwNoRecive = 0;
	m_dwNoSend = 0;

	m_bTxdOnOff = FALSE;
	
	m_bWriteContinue = FALSE;
	m_bNotify = FALSE;
	m_dwNoReadCount = 0;
	
	timerStatus = NONEED;
	
	m_dwFlag = 0;

	for(int i=1; i <= TIMERSUM; i++)
		MyKillTimer(i);
	
	SerialRxdCtr(MINOR_PORT(m_dwPort), FALSE);
	::SerialTxdCtr(MINOR_PORT(m_dwPort), FALSE);
	
#if	(__DEBUG__ == YES)		
	LogError("ResetComm", FILE_LINE, "ResetComm--%d", MINOR_PORT(m_dwPort));
#endif
}

/*********************************************************************************************
 *
 * ��������      ��鴮�ڷ��������Ƿ��������ͽ���
 *
 * ����˵�� 	  	
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL CSerial::IsTxdEnd()
{
	return SerialIOCtl(MINOR_PORT(m_dwPort), COM_TXD_END, (DWORD *)NULL, (DWORD *)NULL);
}
/*********************************************************************************************
 *
 * ��������      ���ݲ�������Ż�ȡ��Ӧ�Ĳ�������ֵ
 *
 * ����˵�� 	 - wNum		:	����	���������	  	
 *				 - dwBaud	:	���	������ֵ
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void NumToBaud(WORD wNum, DWORD *dwBaud)
{
	DWORD Baud[] = {110,150,300,600,1200,2400,4800,9600,19200,38400,57600,115200};

	if( wNum <= ( sizeof(Baud)/sizeof(DWORD) ) )
		*dwBaud = Baud[wNum];
	else
		*dwBaud = 9600;
}

/*********************************************************************************************
 *
 * ��������      ���ݲ�������ֵ��ȡ��Ӧ�Ĳ��������
 *
 * ����˵�� 	 - dwBaud	:	����	������ֵ	  	
 *				 - wNum		:	���	���������
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void BaudToNum(DWORD dwBaud, BYTE *byNum)
{
	switch(dwBaud)
	{
		case 110:		*byNum = 0;		return;
		case 150:		*byNum = 1;		return;
		case 300:		*byNum = 2;		return;
		case 600:		*byNum = 3;		return;
		case 1200:		*byNum = 4;		return;
		case 2400:		*byNum = 5;		return;
		case 4800:		*byNum = 6;		return;
		case 9600:		*byNum = 7;		return;
		case 19200:		*byNum = 8;		return;
		case 38400:		*byNum = 9;		return;
		case 57600:		*byNum = 10;	return;
		case 115200:	*byNum = 11;	return;
		default:		return;	
	}
}

WORD CSerial::GetTxdWritePtr()
{
	return m_wTxdWritePtr;
}

WORD CSerial::GetRxdWritePtr()
{
	return m_wRxdWritePtr;
}

/*********************************************************************************************
 *
 * ��������      �Ӵ���ͨ�����ͻ�����ָ��λ�ö�ȡһ�����ȵ����ݣ���ά���鿴
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
WORD CSerial::ReadFromTxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;

	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = m_TxdBuf[(wOffset+i)%m_wMaxTxdLen];
	}
		
	return wLen;
}

/*********************************************************************************************
 *
 * ��������      �Ӵ���ͨ�����ջ�����ָ��λ�ö�ȡһ�����ȵ����ݣ���ά���鿴
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
WORD CSerial::ReadFromRxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;

	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = m_RxdBuf[(wOffset+i)%m_wMaxRxdLen];
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
WORD CSerial::ReadNewTxdData(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i;
		
	if(m_wTxdWritePtr >= wOffset)
		wLeft = m_wTxdWritePtr - wOffset;
	else
		wLeft = m_wMaxTxdLen - (wOffset-m_wTxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
		
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_TxdBuf[(wOffset+i)%m_wMaxTxdLen];
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
WORD CSerial::ReadNewRxdData(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i;

	if(m_wRxdWritePtr >= wOffset)
		wLeft = m_wRxdWritePtr - wOffset;
	else
		wLeft = m_wMaxRxdLen - (wOffset-m_wRxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
	
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_RxdBuf[(wOffset+i)%m_wMaxRxdLen];
	}
	
	return wLeft;
}

