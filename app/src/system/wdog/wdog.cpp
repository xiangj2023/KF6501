/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			wdog.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ���Ź�������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ���Ź�������������ɨ�裬��Դ���澯��ˢ�µȴ���                          
*                                                                                  
* ����                                                                             
*                                                                                  
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/11/21    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "wdog.h"
#include "lcdcfg.h"
#include "led.h"
#include "timedef.h"
#include "etpu_app.h"

BEGIN_MESSAGE_MAP(CWDog,CApp)
	ON_SM_TIMEOUT(CWDog)
	ON_SM_LEDCHECK(CWDog)
//	ON_SM_RELAYRTMB(CWDog)
	ON_SM_SYS_RESET(CWDog)
END_MESSAGE_MAP()

//extern void DispSample();

extern DWORD g_hLCDTaskID;
extern BOOL bIsledCheck;

extern OSINT8 CurTaskCount;
//����������
extern TTask_Map TaskTable[];

void WDogTask(DWORD *pdwApp)
{
	CWDog *pWDog = new CWDog(pdwApp);
	pWDog->Init();
	pWDog->Run();
}	

CWDog::CWDog(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CWDog::Init(void)
{
	dwRefreshCnt = 0;
	wKeyKeepCnt = 0;
	dwLedCnt = 0;
	wOldKeyStatus = KEY_INVALID;	//��ʼ����״̬
	FreshFlag = TRUE;
	
	LedInit();
	SetTimer(1, WDOG_TIME);

//	m_hSocket = -1;
}

//LED�Լ���Ϣ��Ӧ,���Լ�ɱ��ָʾ��
void CWDog::OnLedCheck(DWORD id)
{
	if(bIsledCheck == FALSE)
	{
		bIsledCheck = TRUE;

		//�ر����пɱ�̵�
		outportb(LED1_CS_BASE, 0);
		outportb(LED2_CS_BASE, 0);
		outportb(LED3_CS_BASE, 0);

		bCheckLedNo = 0;
	}
}
//��λ��Ϣ��Ӧ��ͨ��ֹͣˢ�¿��Ź�ʵ��
void CWDog::OnSysReset(void)
{
	FreshFlag = FALSE;
}

void CWDog::OnTimeOut(DWORD id)
{
	WORD KeyStatus ;
	BYTE bKey0, bKey1;
	int i;
	TSysTime SysTime;

	bKey0 = bKey1 = 0xff;
	
	dwRefreshCnt++;
	
	//ˢ�¿��Ź�
	if( ((dwRefreshCnt%DWOG_REFRESH_CNT) == 0) && (FreshFlag == TRUE))
	{
		outportb(WATCHDOG_BASE, 0x55);
	}

	//��ʱ��ӡԭʼ��������
//	if((dwRefreshCnt%20)==0)
//		DispSample();

	if( (dwRefreshCnt%RUN_LED_TIME) == 0)
	{
		//��ʱ���Ƶ�ʲ���ͨ���Ƿ�����
		FreqChanMonitor();

		//��ʱ���B��ʽ�����ź��Ƿ�����
		isIrigOk();
		
		dwLedCnt++;

		//LED�Լ촦��
		if(bIsledCheck == TRUE)
		{					
			//�˳��Լ�״̬
			if(bCheckLedNo == CHECK_LED_NUM)
			{
				bIsledCheck = FALSE;
				//outportb(LED3_CS_BASE, 0);
				UpdateLogicLed();
			}
			else
			{
				DWORD status = 1;
				status <<= bCheckLedNo;
				for(int i=0; i<3; i++)
				{
					outportb(LED1_CS_BASE+i, (BYTE)status);
					status >>= 8;
				}
				bCheckLedNo++;
			}
		}
		else
		{
			if(dwLedCnt%2)
			{
				LEDOnOff(RUN_LED, TRUE);
			}
			else
			{
				LEDOnOff(RUN_LED, FALSE);

				//���������ⲿ���ŵ��µƱ��������
				for(int i=(RUN_LED+1); i<28; i++)
					LEDOnOff(i, FALSE);
			}
		}

	}

	//��ѯ��ʽ�������������б�Ҫ��ȥ��
	//����ԭ��:���볬��ȥ��ʱ��İ���������Ч����ʱ�䰴��ֻ����һ��

	//���ڼ��϶࣬��Ҫ��2���˿ڶ�ȡ����
	bKey0 = inportb(KEY1_CS_BASE);
	bKey1 = inportb(KEY2_CS_BASE);
	KeyStatus = MAKEWORD(bKey0, bKey1)&KEY_INVALID;

	if(KeyStatus != KEY_INVALID)	//�м�����
	{
		if(KeyStatus == wOldKeyStatus)	//���ϴμ�ֵ��ͬ�����������
		{
			if(wKeyKeepCnt != 0xffff)	//���̳�ʱ�䰴��ʱ����ֹ�ظ���������Ϣ
			{
				wKeyKeepCnt++;
				if(wKeyKeepCnt >= KEY_KEEP_CNT)	//����ȥ��ʱ�䣬�ж�Ϊ��Ч����
				{
					if(g_hLCDTaskID != NULL)
					{
//						ReportMsg("The Key %x is down, L=%x H=%x.", wOldKeyStatus, bKey0, bKey1);
						OS_PostMessage(g_hLCDTaskID, SM_KEYDOWN, wOldKeyStatus, 0, 0, 0);
						wKeyKeepCnt = 0xffff;	//����Ϊ�Ѿ�����״̬

					}
				}
			}
		}
		else	//�µļ��̰���
		{
			wOldKeyStatus = KeyStatus;
			wKeyKeepCnt = 0;
		}
		
	}
	else
	{
		wKeyKeepCnt = 0;
		wOldKeyStatus = KEY_INVALID;
	}

	//�������
	TaskWatch();

//	if( (dwRefreshCnt%(5000/WDOG_TIME)) == (5000/WDOG_TIME-1))
//		CreateUDPSocket();
}

//������ӹ���
void CWDog::TaskWatch(void)
{
	if((dwRefreshCnt%SECOND_TIME) == 0)
	{
		for(int i=1; i<=CurTaskCount; i++)
		{
			if(TaskTable[i].pTask == NULL)
				continue;
			if(TaskTable[i].pTask->IsDeleted)
				continue;
			TaskTable[i].pTask->Counter++;

			//����ʱ��û��ˢ�¼����������������쳣��ϵͳ��λ
			if(TaskTable[i].pTask->Counter>(TASK_DEAD_TIME/SECOND_TIME))
			{
				FreshFlag = FALSE;
				LogError("TaskWatch",FILE_LINE,"task %s error and system reset.",TaskTable[i].pTask->TaskName);
			}
		}
	}
}
/*
#include "socklib.h"
#include "inetlib.h"
#include "iflib.h"

static		DWORD testRTMB=0;
void CWDog::OnRelayRTMB(DWORD num)
{
	char 		TestFrame[100];
	TAbsTime 	time;
	DWORD		lo;
	
	testRTMB++;

	ReadAbsTime(&time);

	lo = time.Lo % 1000;

	if((testRTMB%1000) == 0)
		sprintf(TestFrame,"%d%d%d\r\n\r\n****************************************\r\n\0",lo/100,(lo%100)/10,lo%10);
	else if((testRTMB%10) == 0)
		sprintf(TestFrame,"%d%d%d\r\n\0",lo/100,(lo%100)/10,lo%10);
	else
		sprintf(TestFrame,"%d%d%d  \0",lo/100,(lo%100)/10,lo%10);
		
	SendUDPData(0xc0a864b1, (BYTE *)TestFrame,strlen(TestFrame));
}

BOOL CWDog::CreateUDPSocket()
{
	struct sockaddr_in selfAddr;
	struct linger Linger;
	int hSocket;
	int nVal;

	if(m_hSocket != -1)
		return TRUE;

	hSocket = -1;
			
	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(hSocket == -1)
	{
		LogError("CWHShell::CreateUDPSocket",FILE_LINE,"create socket error.errno=%d",errno);
		return FALSE;
	}

	nVal = 2*1024;
	setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nVal, sizeof(int));
	setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nVal, sizeof(int));

	nVal = TRUE; 
	if((int)setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nVal, sizeof(int)) == -1)
	{
		LogError("CWHShell::CreateUDPSocket",FILE_LINE,"setsockopt error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}

	nVal = 1;//m_bBockMode;
	if(-1 == ioctl(hSocket,FIONBIO ,(int)&nVal) )
	{
		//ioctlsocket==ioctl,psos��socket���ò�����ʽ��FIONBIO������������ʽ��nbio=0:block,1:noBlock
		close(hSocket);
		return FALSE;
	}

	Linger.l_onoff = 1;
	Linger.l_linger = 0;
	if(setsockopt(hSocket, SOL_SOCKET, SO_LINGER, \
		(char *)&Linger, sizeof(Linger)) < 0)
	{
		close(hSocket);
		return FALSE;
	}
	
	//bind local address
	memset((char *)&selfAddr, 0, sizeof(struct sockaddr_in));
	selfAddr.sin_family = AF_INET;
	selfAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(m_netInfo.dwSelfIP);
	selfAddr.sin_port = 6500;
	 
	if(-1 == bind(hSocket, (sockaddr*)&selfAddr, sizeof(sockaddr)))
	{
		LogError("CWHShell::CreateUDPSocket",FILE_LINE,"bind error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}
	
	nVal = 1;;
	if(-1 == (int)setsockopt(hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&nVal,sizeof(int)))
	{
		LogError("CWHShell::CreateUDPSocket",FILE_LINE,"bind error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}
		
	if(hSocket == -1)
		return FALSE;

	m_hSocket = hSocket;
	
	return TRUE;
}

BOOL CWDog::CloseUDPSocket()
{
	if(m_hSocket == -1)
		return FALSE;
	
	close(m_hSocket);
	m_hSocket = -1;
	return TRUE;
}

BOOL CWDog::SendUDPData(DWORD ip,BYTE *pBuf,WORD wLen)
{
	long 	lgRtn;
	int 	nAddrLen;
	struct sockaddr_in toAddr;

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6500);
	toAddr.sin_addr.s_addr = htonl(ip);

	nAddrLen = sizeof(struct sockaddr_in);

	if(m_hSocket == -1)
		return FALSE;
	
	if(wLen == 0)
		return FALSE;
 		
	lgRtn = sendto(m_hSocket, (char*)pBuf, wLen, 0, (sockaddr*)&toAddr, nAddrLen);

	if(lgRtn > 0)
	{
		m_dwError = 0;
	}

	if(lgRtn <= 0)////=0���ӶϿ�
	{
		// ��������
		if(errno == EWOULDBLOCK)
		{
			m_dwWriteBlockNum ++;
			if(m_dwWriteBlockNum > 100)
			{
				m_dwWriteBlockNum = 0;
				CloseUDPSocket();
			}
		}
		// ��������
		else
		{
			m_dwWriteBlockNum = 0;
		
			m_dwError ++;				
			if(m_dwError > 20)
			{
				m_dwError = 0;
				CloseUDPSocket();
			}
		}
		
		return FALSE;
	}

	return TRUE;
}

BOOL CWDog::RecvUDPData(DWORD ip,BYTE *pBuf,WORD &wLen)
{
	long nReadNum;
	BYTE bBuf[1024];
	WORD wMaxRead;
	int nAddrLen;
	struct sockaddr_in toAddr;

	wLen = 0;
	
	if(m_hSocket == -1)
		return FALSE;
		
	wMaxRead = 1023;
		
	memset((void *)&toAddr,0 ,sizeof(struct sockaddr_in));
	nAddrLen = sizeof(struct sockaddr_in);
		
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6500);
	toAddr.sin_addr.s_addr = htonl(ip);
	
	nReadNum = recvfrom(m_hSocket, (char*)bBuf, wMaxRead, 0, (sockaddr*)&toAddr, &nAddrLen);

	// =0 ʱ��ʾ�ѶϿ�
	if(nReadNum <= 0)
	{
		if(errno == EWOULDBLOCK)
			return TRUE;
		
		m_dwError ++;
		if(m_dwError > 20)
		{
			m_dwError = 0;
			CloseUDPSocket();
		}
	}
	
	if(nReadNum > 0)
	{
		m_dwError = 0;
		DWORD dwRxdIP = ntohl(toAddr.sin_addr.s_addr);
		ReportMsg("CWHShell::RecvUDPData,From %d.%d.%d.%d Len=%d",HHBYTE(dwRxdIP),HLBYTE(dwRxdIP),\
			LHBYTE(dwRxdIP),LLBYTE(dwRxdIP),nReadNum);
	}
}
*/

