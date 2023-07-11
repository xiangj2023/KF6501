#include "protocol.h"
#include "debugcom.h"
#include "parafunc.h"		
#include "sample.h"
#include "shell.h"
#include "misi.h"
#include "netentry.h"
#include "relayfun.h"
#include "logic.h"
#include "canlink.h"

#if(SYSTEM_TYPE == _SYS_KF6500_)

#include "etpu_app.h"
#include "lcdcfg.h"


extern SHORT DispDataBuf[];
extern WORD DispReadPtr;
extern int SysTempture;		//װ���¶�ֵ,�Ŵ�10��
extern HANDLE	g_hLCDTaskID;
extern WORD g_wLCDContrast;
extern DWORD dwSampleErrCnt;

#else

extern SHORT DispDataBuf[];
extern WORD DispReadPtr;
extern OSHANDLE	g_hLCDTaskID;

#endif

extern TMiscEvent 		PowerOnEvent;
extern TSysConfigTable	*G_pCfgTable;
extern void *SysMallocPtr;

void GetDispData(SHORT* pbuf);
void BOTest(char cmd, char index);

static void MakeHEX(char *str,DWORD data,BYTE BitNum)
{
	BYTE BitData;

	for(int i=BitNum-1;i>=0;i--,str++)
	{
		BitData = (data >> 4*i)&0x0F;
		
		if(BitData < 10)
			*str = BitData+'0';
		else
			*str = BitData-10+'A';

	}
}

void GetProtocolName(BYTE No,char *Name)
{
	if(Name == NULL)
		return;
	
	switch(No)
	{
		case 0:
			strcat(Name,"NULL 	");break;
		case 1:
			strcat(Name,"NetPro	");break;
#if(SYSTEM_TYPE == _SYS_KF6500_)
		case 2:
			strcat(Name,"Maintain ");break;
#else
		case 2:
			strcat(Name,"LcdComm	");break;
#endif
		case 3:
			strcat(Name,"Shell	");break;
		case 4:
			strcat(Name,"IEC103	");break;
		case 5:
			strcat(Name,"IEC101	");break;
		case 6:
			strcat(Name,"IEC104	");break;
		case 7:
			strcat(Name,"GM104 ");break;
		default:
			strcat(Name,"Invalid	");break;
	}				
}

/*--------------------------------------------------------------*/
/*��������sCanBusTask					       					*/
/*���ܣ�CANBUS��վ��Լ����				       					*/
/*����: ��						       							*/
/*�������						       							*/
/*--------------------------------------------------------------*/
void WHShell_Task(DWORD *pdwApp)
{
	CWHShell *pShell = new CWHShell(pdwApp);

	if(pShell->Init()!=TRUE)
		pShell->Exit((char *)"SCanBus Init Error");

	pShell->Run();
}

BEGIN_MESSAGE_MAP(CWHShell,CProtocol)
ON_SM_TIMEOUT(CWHShell)
ON_SM_COMMIDLE(CWHShell)
ON_SM_NOTIFYETHLINKED(CWHShell)  //��̫����������
ON_SM_NOTIFYETHCLOSED(CWHShell)  //��̫���ر�����
END_MESSAGE_MAP()

BOOL CWHShell::Init(void)
{
	SetBufLen(512,4096);
	
	//��ʼ����Լ����
	if(CProtocol::Init() != TRUE)
		return FALSE;
	
	StartMsg(MCC_CommIdle,100);
	StartMsg(MCC_ReadComm,MIN_FRAME_LEN);	

	//����CAN������ʱ��
	SetTimer(TM_1S,SHELL_BASIC_TIMER);

	m_dwTimerCnt = 0;

	m_CommandPtr = 0;
	m_wShowCnt = 0;
	m_wShowMax = 0;
	m_ShowPortID = 0;
	m_bCommandExute = FALSE;
/*
	DB_Register_Reset(SYS_RESET_COLD);
	DB_Register_FileDown(PARACFGFILE, 0);
	DB_Register_Comm(1, MAKELONG(1,SC_PORT_SERIAL));
	DB_Register_BO(2, BO_CLOSE);
	DB_Register_SET(0, 3);
	DB_Register_SETArea(4);
	DB_Register_ParaChange(DB_PARA_DAI, 5);
	DB_Register_SW(6, SW_STATUS_SET);
	DB_Register_CFG(7, CFG_STATUS_CLR);
	DB_Register_CF(8);
*/

	m_bEcho = TRUE;
	m_RebootType = 0;
	m_wCmdType = CMD_TYPE_NULL;

	m_NoRxdCounter = MAX_COMMERR_TIME;

	RegisterCommStatus(COMM_STATUS_BREAK);

	return TRUE;
}

/*--------------------------------------------------------------*/
/*��������	CanTxdMonitor					*/
/*���ܣ� 	CANBUS��վ��Լ���͹�����			*/
/*����: 	��						*/
/*�����	��						*/
/*--------------------------------------------------------------*/
void CWHShell::TxdMonitor()
{
	return;
}

/*-------------------------------------------------------------*/
/*��������CanSearchOneFrame					*/
/*���ܣ�  CANBUS��վ��Լ��֡����		       		*/
/*����:   PBYTE Buf		������ָ��			*/
/*	  WORD wLen		��Ѱ���������ȡ�	       */
/*�����  ��						       */
/*-------------------------------------------------------------*/

DWORD CWHShell::SearchOneFrame(PBYTE Buf,WORD wLen)
{
	int 	i,j=0;
		
	if(wLen == 0)
		return FM_LESS;
	
	//����ָ��
	if((m_CommandPtr+wLen)>MAX_FRAME_LEN)
			wLen = MAX_FRAME_LEN-m_CommandPtr;

	for(i=0,j=0;i<wLen;i++)
	{
		if((Buf[i]>='A')&&(Buf[i]<='Z'))
			m_CommandBuffer[m_CommandPtr++] = 'a'+Buf[i]-'A';
		else
			m_CommandBuffer[m_CommandPtr++] = Buf[i];
		
		// 0x03-->ctrl+c
		// 0x1A-->ctrl+z
		// 0x08-->�˸�
		if(Buf[i] == '\r')
		{
			//��ӡ�ɻس�����
			m_TxdInfo.pBuf[j++] = Buf[i];
			m_TxdInfo.pBuf[j++] = '\n';
		}
		else if(Buf[i] == '\n')
		{
			// �޳��� "\n"
			m_CommandPtr--;
		}
		else if((Buf[i] == 0x03)||(Buf[i] == 0x05)||(Buf[i] == 0x1A))
		{
			//����ӡ
		}
		else if(Buf[i] == 0x08)
		{
			m_CommandPtr--;

			//ɾ��ǰһ���ַ�
			if(m_CommandPtr >= 1)
				m_CommandPtr--;
			
			if(m_bEcho) 
			{
				m_TxdInfo.pBuf[j++] = Buf[i];
				m_TxdInfo.pBuf[j++] = ' ';
				m_TxdInfo.pBuf[j++] = Buf[i];
			}
			else
			{
				m_TxdInfo.pBuf[j++] = ' ';
				m_TxdInfo.pBuf[j++] = Buf[i];
				WriteComm(j, 0, 0);
			}
		}
		else
		{
			m_TxdInfo.pBuf[j++] = Buf[i];
		}
		// �������ϼ�ͷ
		if(m_CommandPtr >= 3)
		{
			if(m_CommandBuffer[m_CommandPtr-3] == 27)
			if(m_CommandBuffer[m_CommandPtr-2] == 91)
			if(m_CommandBuffer[m_CommandPtr-1] == 97)
			{
				m_CommandPtr -= 3;

				// ˢ����
				strcpy((char *)m_CommandBuffer+m_CommandPtr,m_LastCommand);
				m_CommandPtr += strlen(m_LastCommand);

				// ˢ����
				sprintf((char *)m_TxdInfo.pBuf,"\r\n\r\n%s",m_LastCommand);
				j = strlen(m_LastCommand)+4;

				if(!m_bEcho) 
					WriteComm(j, 0, 0);
			}
		}	
	}

	if(m_bEcho)	
		WriteComm(j, 0, 0);

	return (FM_OK|wLen);
}

/*-------------------------------------------------------------*/
/*��������CanOnFrame					*/
/*���ܣ�  CANBUS��վ��Լ��֡����		       		*/
/*����:   PBYTE Buf		֡������ͷָ��			*/
/*�����  ��						       */
/*-------------------------------------------------------------*/
void CWHShell::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	TxdMonitor();
}

/*-------------------------------------------------------------*/
/*��������CanOnFrame					*/
/*���ܣ�  CANBUS��վ��Լ��֡����		       		*/
/*����:   PBYTE Buf		֡������ͷָ��			*/
/*�����  ��						       */
/*-------------------------------------------------------------*/
void CWHShell::OnFrame(PBYTE Buf)
{
	//����ͨ��״̬
	m_NoRxdCounter = MAX_COMMERR_TIME;
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	
	SearchCommand();
}


/*-------------------------------------------------------------*/
/*��������CanOnTimeOut					       */
/*���ܣ�CANBUS��վ��Լ��ʱ����Ӧ����			       */
/*����: DWORD id	��ʱ��ID			       */
/*�������						       */
/*-------------------------------------------------------------*/
void CWHShell::OnTimeOut(DWORD id)
{	
	m_dwTimerCnt++;

	if(m_NoRxdCounter > 0)
	{
		// ����ʱ
		m_NoRxdCounter--;

		// ͨ��ʧ��ʱ����
		if(m_NoRxdCounter == 0)
			RegisterCommStatus(COMM_STATUS_BREAK);
	}
	
	if(m_wCmdType != CMD_TYPE_NULL)	
		ExuteCommand();
}

void CWHShell::OnEthLinked(DWORD dwPara)
{
	RegisterCommStatus(COMM_STATUS_OK);
}

void CWHShell::OnEthClosed(DWORD dwPara)
{
	RegisterCommStatus(COMM_STATUS_BREAK);
}

BOOL CWHShell::SearchCommand()
{
	WORD			wPortID;
	char			*pCmdStart,*pCmdEnd;
	char			*pCmdWord;
	CSerial 		*pSerial;
	CSocket 		*pSocket;
	int 			i,nRtn;
	BOOL			bCommand=FALSE;
	
	if(m_CommandPtr == 0)
		return FALSE;

	//�ȼ���˳�
	for(i=0;i<m_CommandPtr;i++)
	{
		if((m_CommandBuffer[i] == 0x03)||(m_CommandBuffer[i] == 0x1A))
		{
			m_wCmdType = CMD_TYPE_NULL;
			ClearCommand(&m_CommandBuffer[i+1]);
			
			SetDebugComm(0);
			m_ShowPortID=0;

			memcpy(m_TxdInfo.pBuf,"\r\nCommand Quit\r\n\r\n\r\n",22);
			WriteComm(22, 0, 0);
			return TRUE;
		}

		//�򿪹رջ���
		if(m_CommandBuffer[i] == 0x05)
		{
			ClearCommand(&m_CommandBuffer[i+1]);
			
			if(m_bEcho)
			{
				m_bEcho = FALSE;
				memcpy(m_TxdInfo.pBuf,"\r\nEcho Close\r\n\r\n\r\n",20);
				WriteComm(20, 0, 0);
			}
			else
			{
				m_bEcho = TRUE;
				memcpy(m_TxdInfo.pBuf,"\r\nEcho Open\r\n\r\n\r\n",19);
				WriteComm(19, 0, 0);
			}
			return TRUE;
		}
	}
	
	// Ĭ������س���֮ǰ������
	pCmdEnd = strstr(m_CommandBuffer,"\r");
	if(pCmdEnd == NULL)
		return FALSE;
	
	pCmdEnd++;
	
	//��ʼ����
	if(pCmdStart = strstr(m_CommandBuffer,"showbuf"))
	{	
		wPortID = m_ShowPortID;
		
		if(pCmdWord = strstr((char *)pCmdStart,"net"))
		{
			if(pCmdWord[3] == 'a')
			{
				if((pCmdWord[4]>'0')&&(pCmdWord[4]<'9'))
				{				
					m_wCmdType = CMD_TYPE_TXDRXD;
					bCommand = TRUE;
					wPortID = (SC_PORT_ETHERNETA<<8)+pCmdWord[4]-'0';
				}
			}
			else if(pCmdWord[3] == 'b')
			{
				if((pCmdWord[4]>'0')&&(pCmdWord[4]<'9'))
				{				
					m_wCmdType = CMD_TYPE_TXDRXD;
					bCommand = TRUE;
					wPortID = (SC_PORT_ETHERNETB<<8)+pCmdWord[4]-'0';
				}
			}
		}
		
		if(pCmdWord = strstr((char *)pCmdStart,"ser"))
		{
			if((pCmdWord[3]>'0')&&(pCmdWord[3]<'9'))
			{
				m_wCmdType = CMD_TYPE_TXDRXD;
				bCommand = TRUE;
				wPortID = (SC_PORT_SERIAL<<8)+pCmdWord[3]-'0'; 
			}
		}

		if(pCmdWord = strstr((char *)pCmdStart,"can"))
		{
			if((pCmdWord[3]>'0')&&(pCmdWord[3]<'9'))
			{
				m_wCmdType = CMD_TYPE_TXDRXD;
				bCommand = TRUE;
				wPortID = (SC_PORT_CANBUS<<8)+pCmdWord[3]-'0'; 
			}
		}

		if (m_ShowPortID != wPortID)
		{
			SetDebugComm(wPortID);
			m_ShowPortID = wPortID;
		}
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"showvar"))
	{	
		do{
			//   ��ʾNvRam / EEPROM
			if((strstr((char *)pCmdStart,"-mem")!=NULL)||(strstr((char *)pCmdStart,"-eeprom")!=NULL))
			{
				if(pCmdWord = strstr((char *)pCmdStart,"-mem"))
				{
					m_wCmdType = CMD_TYPE_VARMEM;
					pCmdWord += 4;
				}
				else if(pCmdWord = strstr((char *)pCmdStart,"-eeprom"))
				{
					m_wCmdType = CMD_TYPE_VAREEPROM;
					pCmdWord += 7;
				}
				bCommand = TRUE;
				m_dwOffset = 0;
				
				while(pCmdWord != pCmdEnd)
				{
					if((*pCmdWord>='0')&&(*pCmdWord<='9'))
					{
						m_dwOffset = atoi(pCmdWord);
						break;
					}
					else
						pCmdWord++;
				}
				
				if(m_wCmdType == CMD_TYPE_VARMEM)
					nRtn = DAE_ReadMemVar(DAE_VARTYPE_NVRAM<<16, m_dwOffset, 512, m_DataBuffer);
				else
					nRtn = DAE_ReadMemVar(DAE_VARTYPE_EEPROM<<16, m_dwOffset, 512, m_DataBuffer);

				// ��ȷ��ȡʱ������ʾ
				if(nRtn > 0)
				{
					bCommand = TRUE;
					m_wShowCnt = 0;
					m_wShowMax = nRtn;
				}
				break;
			}

			// ��ʾ�����շ�������			
			if(pCmdWord = strstr((char *)pCmdStart,"ser"))
			{
				if((pCmdWord[3]<'0')||(pCmdWord[3]>'9'))
					break;
				
				pSerial = GetSerial(pCmdWord[3]-'0');
				if(pSerial == NULL)
					break;
				
				bCommand = TRUE;
					
				pCmdWord += 4;
				
				if(pCmdWord = strstr((char *)pCmdStart,"-t"))
				{
					m_dwOffset = pSerial->GetTxdWritePtr();
					m_wCmdType = CMD_TYPE_VARTXD;
				}
				else //if(pCmdWord = strstr((char *)pCmdStart,"-r"))
				{
					m_dwOffset = pSerial->GetRxdWritePtr();
					m_wCmdType = CMD_TYPE_VARRXD;
				}

				m_dwCount = 0;
				m_ShowPortID = pSerial->m_dwPort;
				break;
			}

			// ��ʾ��̫�����շ�������			
			if(pCmdWord = strstr((char *)pCmdStart,"eth"))
			{
				if((pCmdWord[3]<'0')||(pCmdWord[3]>'9'))
					break;
				
				pSocket = GetSortSocket(pCmdWord[3]-'0'-1);
				if(pSocket == NULL)
					break;
				
				bCommand = TRUE;
					
				pCmdWord += 4;
				
				if(pCmdWord = strstr((char *)pCmdStart,"-t"))
				{
					m_dwOffset = pSocket->GetTxdWritePtr();
					m_wCmdType = CMD_TYPE_VARTXD;
				}
				else//if(pCmdWord = strstr((char *)pCmdStart,"-r"))
				{
					m_dwOffset = pSocket->GetRxdWritePtr();
					m_wCmdType = CMD_TYPE_VARRXD;
				}
				
				m_dwCount = 0;
				m_ShowPortID = pSocket->m_dwPort;
				break;
			}

			// ��ʾCAN���շ�������			
			if(pCmdWord = strstr((char *)pCmdStart,"can"))
			{
				if((pCmdWord[3]<'1')||(pCmdWord[3]>'2'))
					break;
				
				bCommand = TRUE;
				m_dwCount = pCmdWord[3]-'0';
					
				pCmdWord += 4;
				
				if(pCmdWord = strstr((char *)pCmdStart,"-t"))
				{
					m_dwOffset = GetCanTxdWritePtr(m_dwCount);
					m_wCmdType = CMD_TYPE_VARTXD;
				}
				else //if(pCmdWord = strstr((char *)pCmdStart,"-r"))
				{
					m_dwOffset = GetCanRxdWritePtr(m_dwCount);
					m_wCmdType = CMD_TYPE_VARRXD;
				}
			
				m_ShowPortID = MAKEWORD(m_dwCount,SC_PORT_CANBUS);
				m_dwCount = 0;
				break;
			}

		}while(0);
	}	
	else if(strstr(m_CommandBuffer,"showerr"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_RECORD;
		
		if(strstr(m_CommandBuffer,"-a"))
		{
			DB_GetRunRecordHead(&m_nRecordRead);
			m_bShowAll = TRUE;
		}
		else
		{
			DB_GetRunRecordTail(&m_nRecordRead);
			m_bShowAll = FALSE;
		}
	}
	else if(strstr(m_CommandBuffer,"showsam"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWSAMPLE;
		DispReadPtr = 0;
	}
	else if(strstr(m_CommandBuffer,"showrec"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_MISC;
		
		if(strstr(m_CommandBuffer,"-a"))
		{
			DB_Misc_GetHead(&m_nRecordRead);
			m_bShowAll = TRUE;
		}
		else
		{
			DB_Misc_GetTail(&m_nRecordRead);
			m_bShowAll = FALSE;
		}
	}
	else if(strstr(m_CommandBuffer,"showyc"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWYC;
		m_wShowCnt = 0;
		m_wShowMax = DB_GetAINum();
	}
	else if(strstr(m_CommandBuffer,"showrm"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWRM;
		m_wShowCnt = 0;
		m_wShowMax = DB_GetRMNum();
	}
	else if(strstr(m_CommandBuffer,"showraw"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWRMRAW;
		m_wShowCnt = 0;
		m_wShowMax = DB_GetRMNum();
	}
	else if(strstr(m_CommandBuffer,"showyx"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWYX;
		m_wShowCnt = 0;
		m_wShowMax = DB_GetBINum();
	}
	else if(strstr(m_CommandBuffer,"showdd"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWDD;
		m_wShowCnt = 0;
		m_wShowMax = DB_GetCINum();
	}
	else if(strstr(m_CommandBuffer,"showbie"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWBIE;
		
		if(strstr(m_CommandBuffer,"-a"))
		{
			DB_GetBIEHead(&m_nRecordRead);
			m_bShowAll = TRUE;
		}
		else
		{
			DB_GetBIETail(&m_nRecordRead);
			m_bShowAll = FALSE;
		}
	}
	else if(strstr(m_CommandBuffer,"showsoe"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWSOE;
		
		if(strstr(m_CommandBuffer,"-a"))
		{
			DB_GetSOEHead(&m_nRecordRead);
			m_bShowAll = TRUE;
		}
		else
		{
			DB_GetSOETail(&m_nRecordRead);
			m_bShowAll = FALSE;
		}
	}
	else if(strstr(m_CommandBuffer,"showrep"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWREP;
		
		if(strstr(m_CommandBuffer,"-a"))
		{
			DB_GetRelayStartReportHead(&m_nSTTRead);
			DB_GetRelayAlarmReportHead(&m_nALMRead);
			DB_GetRelayActionReportHead(&m_nACTRead);
			m_bShowAll = TRUE;
		}
		else
		{
			DB_GetRelayStartReportTail(&m_nSTTRead);
			DB_GetRelayAlarmReportTail(&m_nALMRead);
			DB_GetRelayActionReportTail(&m_nACTRead);
			m_bShowAll = FALSE;
		}
	}
	else if(strstr(m_CommandBuffer,"setbo"))	//ң�ز���
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_YK;

		m_bShowAll = FALSE;
		char value;

		if(m_CommandBuffer[6] >= 'a')
			value = m_CommandBuffer[6] - 'a' + 10;
		else
			value = m_CommandBuffer[6]-'0';
		BOTest(m_CommandBuffer[6], value);
	}
	else if(strstr(m_CommandBuffer,"dist"))
	{
		if(strstr(m_CommandBuffer,"dir"))
		{
			bCommand = TRUE;
			m_wCmdType = CMD_TYPE_SHOWDISTDIR;
		}
	}
	else if(strstr(m_CommandBuffer,"showrun"))
	{
		m_wShowCnt = 0;
		m_wShowMax = DAE_MAX_FLAGS;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWRUNSTA;
	}
	else if(strstr(m_CommandBuffer,"showini"))
	{
		m_wShowCnt = 0;
		m_wShowMax = DAE_MAX_FLAGS;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWINITSTA;
	}
	else if(strstr(m_CommandBuffer, "settime"))//��ʱ����
	{	//��ʽΪ:st 08/10/11 15:23:50			
		WORD data[8];
		int i = 0;
		TSysTime time;

		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SETTIME;

		m_bShowAll = FALSE;

		//��B��ʽʱ����Ч������£�����ͨ�Ź�Լ���ӣ���ͬʱ����Ӳ��ʱ��
		if(DAE_GetRunningFlags(SS_TIME_B))
		{
			//ת��ʱ����Ϣ
			for(i=0; i<6; i++)
			{
				data[i] = (m_CommandBuffer[8+i*3]-'0')*10 + (m_CommandBuffer[8+i*3+1]-'0');
			}
			
			memset((void*)&time, 0, sizeof(TSysTime));

			time.Year = data[0]+2000;
			time.Month = data[1];
			time.Day = data[2];
			time.Hour = data[3];
			time.Minute = data[4];
			time.Second = data[5];
			WriteSystemTime(&time);
			WriteHardTime(&time);
		}
	}
	else if(strstr(m_CommandBuffer, "readtime"))	//��������
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_GETTIME;

		m_TxdInfo.pBuf[0] = '\r';
		m_TxdInfo.pBuf[1] = '\n';
		WriteComm(2,0,0);
	}
	else if(strstr(m_CommandBuffer, "showsysval"))	//��ȡϵͳ��������
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWSYSVALUE;
	}
	else if(strstr(m_CommandBuffer, "resetdata"))	//��FLASH���±�������
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_UPDATEDATA;
	}
#if(SYSTEM_TYPE == _SYS_KF6500_)
	else if(strstr(m_CommandBuffer, "resetcoef"))	//��FLASH����ϵ������
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_UPDATECF;
	}
#else
	else if(strstr(m_CommandBuffer, "resetchan"))	//��FLASH����ϵ������
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_UPDATECF;
	}
#endif
	else if(strstr(m_CommandBuffer, "resetpara"))	//��FLASH����װ�ò���
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_RESETPARA;
	}
	else if(strstr(m_CommandBuffer, "clearevent"))	//��FLASH���±�������
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_CLEAREVENT;
	}
	else if(strstr(m_CommandBuffer, "help"))	//��������
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_HELP;
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"showport"))
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWPORT;
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"showmap"))
	{
		m_wShowCnt = 0;
		m_wShowMax = DB_GetMapBitObjNum();
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWMAPBIT;
	}
#if(SYSTEM_TYPE == _SYS_KF6500_)
	else if(pCmdStart = strstr(m_CommandBuffer,"showfl"))
	{
		m_wShowCnt = 0;
		m_wShowMax = DB_GetFLObjNum()+1;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWFL;
	}
#endif	
	else if(pCmdStart = strstr(m_CommandBuffer,"showstat"))
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWSTAT;
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"reboot"))
	{
		if(strstr(m_CommandBuffer,"-c"))
		{
			m_RebootType = 0xAA;
		}
		else
		{
			m_RebootType = 0x55;
		}
		
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_REBOOT;
	}
#if(SYSTEM_TYPE == _SYS_KF6500_)
	else if(strstr(m_CommandBuffer,"key "))	//ң�ز���
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_KEY;

		m_bShowAll = FALSE;
		WORD wKeyValue = 0;

		switch(m_CommandBuffer[4])
		{
			case 'U':
			case 'u':
				wKeyValue = KEY_UP;
				break;
			case 'D':
			case 'd':
				wKeyValue = KEY_DOWN;
				break;
			case 'L':
			case 'l':
				wKeyValue = KEY_LEFT;
				break;
			case 'R':
			case 'r':
				wKeyValue = KEY_RIGHT;
				break;
			case 'A':
			case 'a':
				wKeyValue = KEY_ADD;
				break;
			case 'M':
			case 'm':
				wKeyValue = KEY_MINUS;
				break;
			case 'E':
			case 'e':
				wKeyValue = KEY_ESCAPE;
				break;
			case 'N':
			case 'n':
				wKeyValue = KEY_ENTER;
				break;
		}
		OS_PostMessage(g_hLCDTaskID, SM_KEYDOWN, wKeyValue, 0, 0, 0);
	}
	else if(strstr(m_CommandBuffer, "lcdinf"))	//��ʾҺ����Ϣ
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_LCDINF;
	}
#endif	
	// ģ�ⱨ��
	else if(pCmdStart = strstr(m_CommandBuffer,"makerep"))
	{	
		m_dwOffset = 0;
		bCommand = TRUE;
		
		if(strstr(m_CommandBuffer,"-alm"))
			m_wCmdType = CMD_TYPE_MAKEALM;
		else
			m_wCmdType = CMD_TYPE_MAKEREP;
		
		pCmdWord = pCmdStart+7;
		while(pCmdWord != pCmdEnd)
		{
			if((*pCmdWord>='0')&&(*pCmdWord<='9'))
			{
				m_dwOffset = atoi(pCmdWord);
				break;
			}
			else
				pCmdWord++;
		}	
	}
/*	
	else if(pCmdStart = strstr(m_CommandBuffer,"startfl1"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_CALLSUBDATA,0x11111111,0x6666,(0x00<<8)+1,0);	
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"startfl2"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_CALLSUBDATA,0x22222222,0x6666,(0x01<<8)+1,0);	
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"startfl3"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_CALLSUBDATA,0x33333333,0x6666,(0x02<<8)+1,0);	
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"startfl4"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_CALLSUBDATA,0x44444444,0x6666,(0x03<<8)+1,0);	
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl1"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x00<<8)+1,0,0,0);	//������Ϣֹͣ�ٻ���������				
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl2"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x01<<8)+1,0,0,0);	//������Ϣֹͣ�ٻ���������				
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl3"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x02<<8)+1,0,0,0);	//������Ϣֹͣ�ٻ���������				
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl4"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x03<<8)+1,0,0,0);	//������Ϣֹͣ�ٻ���������				
	}
*/	
	if(m_wCmdType == CMD_TYPE_NULL)
	{
		if((pCmdEnd-m_CommandBuffer)>1)
		{
			sprintf((char *)m_TxdInfo.pBuf,"\r\n\"%s\"? Please type \"help\" to get it first!\r\n",m_CommandBuffer);
			WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
		}
	}

	// ��ȷ�ϵ����⴦��
	if(m_wCmdType == CMD_TYPE_ACK)
	{
		ProcessAckCommand();
	}
	
	//��¼һ���µ�ָ��ִ��
	m_bCommandExute = TRUE;

	//ִ�б���ָ��
	ExuteCommand();

	m_bCommandExute = FALSE;		

	//�������ָ��
	ClearCommand(pCmdEnd);

/*
	if(!bCommand)
	{
		if(m_wCmdType == CMD_TYPE_NULL)
		{
			memcpy(m_TxdInfo.pBuf,"Unknow Command!!\r\n\r\n",18);
			WriteComm(18, 0, 0);
		}
		return FALSE;
	}
	else
	{
		return TRUE;
	}
*/
	return TRUE;
}

void CWHShell::ExuteCommand()
{
	TRecordBlock  	event;
	TMiscEvent		tMiscEvent;
	TBurstBI 		bie;
	TEventBI 		soe;
	TRelayAlarmEv 	AlarmRep;
	TRelayStartEv 	StartRep;
	TRelayActionEv 	ActionRep;
	TRelayEventHead tRelayEvent;
	TRE_Disp		tREDisp;
	TRM_Disp		tRMDisp;

	TAbsTime		tAbsTime;
	TSysTime		tSysTime;
	
	WORD		  	RnLen,MaxLen;
	WORD		  	ReadPtr,TailPtr;
	WORD		  	i,j;

	long		  	lValue;
	BYTE		  	byValue;
	float			fFDVlaue;
	DWORD			dwValue;
	WORD			wFDIndex;
	BYTE			*pDataBuf;
	DWORD			dwOffset;
	char			strTemp[32];
	SHORT 			*ptr;
	DWORD			dwFileLength,dwRtnLength;
	CSerial 		*pSerial;
	CSocket 		*pSocket;
	STATUS			rc;
	DWORD			dwBaud;

	switch(m_wCmdType)
	{
		case CMD_TYPE_TXDRXD:
			RnLen = ReadPrompt((char *)m_TxdInfo.pBuf,1023);
			if (RnLen)
			{
				WriteComm(RnLen,0,0);
			}
			break;
			
		case CMD_TYPE_RECORD:
			DB_GetRunRecordTail(&TailPtr);

			i = 0;
			
			while(m_nRecordRead != TailPtr)
			{
				DB_ReadRunRecord(&event, m_nRecordRead);
				memcpy((char *)m_TxdInfo.pBuf,event.szRecordMsg,event.nCharSize%256);
				WriteComm(event.nCharSize,0,0);
				i++;
				m_nRecordRead = (m_nRecordRead+1)%MAX_RUNREC_NUM;

				//ÿ������ӡ10��
//				if(i >= 5)
					break;
			}
			break;

		case CMD_TYPE_MISC:
			DB_Misc_GetTail(&TailPtr);

			i = 0;
			
			while(m_nRecordRead != TailPtr)
			{
				j = 0;
				DB_Misc_Read(m_nRecordRead,&tMiscEvent);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"---------������¼%d-------- \r\n",m_nRecordRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				m_TxdInfo.pBuf[j] = 0;
				switch(tMiscEvent.nClass)
				{
				case MISC_CLASS_DOWNFILE:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: �ļ����� \r\n");
					break;
				case MISC_CLASS_RESET:	
					strcat((char *)&m_TxdInfo.pBuf[j],"����: װ�ø�λ \r\n");
					break;
				case MISC_CLASS_COMM:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ͨ�Ÿ澯 \r\n");
					break;
				case MISC_CLASS_AO:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ң������ \r\n");
					break;
				case MISC_CLASS_BO:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ң�ز��� \r\n");
					break;
				case MISC_CLASS_SYNC:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ͬ�ڲ��� \r\n");
					break;
				case MISC_CLASS_SET:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ��ֵ�޸� \r\n");
					break;
				case MISC_CLASS_SETAREA:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ��ֵ��Ͷ�� \r\n");
					break;
				case MISC_CLASS_SW:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ��ѹ��Ͷ�� \r\n");
					break;
				case MISC_CLASS_CFG:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ���������� \r\n");
					break;
				case MISC_CLASS_CF:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ����ϵ�� \r\n");
					break;
#if (SYSTEM_TYPE == _SYS_KF1300_)
				case MISC_CLASS_ZERODRIFT:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ��Ư���� \r\n");
					break;
				case MISC_CLASS_FULLVALUE:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ��ֵ���� \r\n");
					break;
				case MISC_CLASS_CHANNELSRC:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ����ͨ��ԴͶ�� \r\n");
					break;
//				case MISC_CLASS_CH:
//					strcat((char *)&m_TxdInfo.pBuf[j],"����: ͨ�������޸� \r\n");
					break;
#endif
				case MISC_CLASS_PARA:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: �����޸� \r\n");
					break;
				case MISC_CLASS_POWER:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: װ�õ��� \r\n");
					break;
				case MISC_CLASS_CLEARCI:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: ������� \r\n");
					break;
				case MISC_CLASS_MODIAI:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: �޸�ң�� \r\n");
					break;
				case MISC_CLASS_MODIBI:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: �޸�ң�� \r\n");
					break;
				case MISC_CLASS_MODICI:
					strcat((char *)&m_TxdInfo.pBuf[j],"����: �޸ĵ�� \r\n");
					break;
				default: 	
					sprintf((char *)&m_TxdInfo.pBuf[j],"����: δ֪����(%d) \r\n",tMiscEvent.nClass);
					break;
				}
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				AbsTimeTo(&tMiscEvent.absTime, &tSysTime);
				sprintf((char *)&m_TxdInfo.pBuf[j],"ʱ��: %d.%d.%d %d:%d:%d:%d \r\n",\
					tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				DB_GetPortName(tMiscEvent.dwOperator,strTemp);
				sprintf((char *)&m_TxdInfo.pBuf[j],"����: %s \r\n",strTemp);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				switch(tMiscEvent.nClass)
				{
				case MISC_CLASS_DOWNFILE:
					strTemp[0] = '\0';
					switch(tMiscEvent.dwValue)
					{
					case 1:
						sprintf(strTemp,"�ļ�: ��������");break;
					case 2:
						sprintf(strTemp,"�ļ�: ��������");break;
					case 3:
						sprintf(strTemp,"�ļ�: ��������");break;
					case 4:
						sprintf(strTemp,"�ļ�: ���г���");break;
					default:
						sprintf(strTemp,"�ļ�: ��������");break;
					}
					sprintf((char *)&m_TxdInfo.pBuf[j],"%s\r\n���: %s\r\n",strTemp,\
						tMiscEvent.dwAttr==0?"�ɹ�":"ʧ��");
					break;
					
				case MISC_CLASS_RESET:	
//					sprintf((char *)&m_TxdInfo.pBuf[j],"״̬: %s \r\n",tMiscEvent.dwValue==SYS_RESET_WARM?"������":"������");
					sprintf((char *)&m_TxdInfo.pBuf[j],"״̬: 0x%x \r\n",tMiscEvent.dwValue);
					break;
					
				case MISC_CLASS_COMM:
					sprintf((char *)&m_TxdInfo.pBuf[j],"ͨ�Ŷ˿�: %x \r\nͨ��״̬: %s \r\n",\
						tMiscEvent.dwAttr,tMiscEvent.dwValue==0?"OK":"Err");
					break;
					
				case MISC_CLASS_AO:
					sprintf((char *)&m_TxdInfo.pBuf[j],"���: %d \r\n��ֵ: %d \r\n",\
						tMiscEvent.dwValue,tMiscEvent.dwAttr);
					break;
					
				case MISC_CLASS_BO:
					strTemp[0] = '\0';
					switch(tMiscEvent.dwAttr)
					{
					case BO_NULL: strcat(strTemp,"��");break;
					case BO_OPEN: strcat(strTemp,"��");break;
					case BO_CLOSE: strcat(strTemp,"��");break;
					default: strcat(strTemp,"��Ч");break;
					}
					sprintf((char *)&m_TxdInfo.pBuf[j],"���: %d \r\n����: %s \r\n",\
						tMiscEvent.dwValue,strTemp);
					break;
					
				case MISC_CLASS_SET:
					sprintf((char *)&m_TxdInfo.pBuf[j],"�������: %d-%d \r\n",\
						HIWORD(tMiscEvent.dwValue),LOWORD(tMiscEvent.dwValue));
					break;
					
				case MISC_CLASS_SETAREA:
					sprintf((char *)&m_TxdInfo.pBuf[j],"Ͷ������: %d \r\n",tMiscEvent.dwValue);
					break;
					
				case MISC_CLASS_SW:
					sprintf((char *)&m_TxdInfo.pBuf[j],"���: %d \r\n״̬: %s\r\n",\
						tMiscEvent.dwValue,tMiscEvent.dwAttr==SW_STATUS_CUT?"��":"Ͷ");
					break;
					
				case MISC_CLASS_CFG:
					sprintf((char *)&m_TxdInfo.pBuf[j],"���: %d \r\n״̬: %s\r\n",\
						tMiscEvent.dwValue,tMiscEvent.dwAttr==CFG_STATUS_CLR?"��":"Ͷ");
					break;
					
				case MISC_CLASS_CF:
#if (SYSTEM_TYPE == _SYS_KF1300_)
				case MISC_CLASS_ZERODRIFT:
				case MISC_CLASS_FULLVALUE:
				case MISC_CLASS_CHANNELSRC:
#endif					
					sprintf((char *)&m_TxdInfo.pBuf[j],"���: %d \r\n",tMiscEvent.dwValue);
					break;
					
				case MISC_CLASS_PARA:
					strTemp[0] = '\0';
					switch(tMiscEvent.dwValue)
					{
					case DB_PARA_DAI:	strcat(strTemp,"ģ�������");break;
					case DB_PARA_DI:	strcat(strTemp,"���ֲ�����");break;
					case DB_PARA_AI:	strcat(strTemp,"ң��");break;
					case DB_PARA_BI:	strcat(strTemp,"ң��");break;
					case DB_PARA_CI:	strcat(strTemp,"���");break;
					case DB_PARA_BO:	strcat(strTemp,"ң��");break;
					case DB_PARA_AO:	strcat(strTemp,"ң��");break;
					case DB_PARA_SER:	strcat(strTemp,"����");break;
					case DB_PARA_CAN:	strcat(strTemp,"CAN");break;
					case DB_PARA_ETH:	strcat(strTemp,"��̫��");break;
					case DB_PARA_CHANNEL: strcat(strTemp,"ͨ��");break;
					case DB_PARA_MBO:	strcat(strTemp,"����λ");break;
					default:			strcat(strTemp,"δ֪");break;
					}
					sprintf((char *)&m_TxdInfo.pBuf[j],"����: %s \r\n���: %d \r\n",\
						strTemp,tMiscEvent.dwAttr);
					break;
					
				case MISC_CLASS_POWER:
					sprintf((char *)&m_TxdInfo.pBuf[j],"״̬: %s",tMiscEvent.dwValue==0?"�ϵ�":"ͣ��");
					break;
					
				case MISC_CLASS_SYNC:
				case MISC_CLASS_CLEARCI:
				case MISC_CLASS_MODIAI:
				case MISC_CLASS_MODIBI:
				case MISC_CLASS_MODICI:
				default: 	
					break;
				}
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				WriteComm(j,0,0);

				i++;
				m_nRecordRead = (m_nRecordRead+1)%MAX_MISC_NUM;

				//ÿ������ӡ10��
//				if(i >= 5)
					break;
			}
			break;

		case CMD_TYPE_SHOWYC:
		case CMD_TYPE_SHOWRM:
		case CMD_TYPE_SHOWRMRAW:
			if(!m_bCommandExute)
				break;		

			for(i=m_wShowCnt,j=0; i<m_wShowMax; i++)
			{
				if((i%5) == 0)
				{
					m_TxdInfo.pBuf[j] = 0;
					if(m_wCmdType == CMD_TYPE_SHOWYC)
					{
						sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nYC %2d~%2d: ",i,i+4);
					}
					else if(m_wCmdType == CMD_TYPE_SHOWRM)
					{
						sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nRM %2d~%2d: ",i,i+4);
					}
					else
					{
						sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nRAW %2d~%2d: ",i,i+4);
					}
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
				}

				if(m_wCmdType == CMD_TYPE_SHOWYC)
					DB_ReadSecondAI(i,&lValue);
				else if(m_wCmdType == CMD_TYPE_SHOWRM)
					DB_ReadSecondRM(i,&lValue);
				else
					DB_ReadRM(i,&lValue);
								
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"%5d  ",lValue);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';
			WriteComm(j,0,0);
			break;
			
		case CMD_TYPE_SHOWYX:
			if(!m_bCommandExute)
				break;
						
			for(i=m_wShowCnt,j=0; i<m_wShowMax; i++)
			{
				if((i%16) == 0)
				{
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nYX %2d~%2d: ",i,i+15);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
				}
				
				DB_ReadBI(i,&byValue);
						
				if(byValue == DB_BI_CLOSE)
					m_TxdInfo.pBuf[j++] = 'H';
				else
					m_TxdInfo.pBuf[j++] = 'F';
				
				m_TxdInfo.pBuf[j++] 	= ' ';
				m_TxdInfo.pBuf[j++] 	= ' ';

			}			
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';
			WriteComm(j,0,0);
			break;
			
		case CMD_TYPE_SHOWDD:
			if(!m_bCommandExute)
				break;
						
			for(i=m_wShowCnt,j=0; i<m_wShowMax; i++)
			{
				if((i%8) == 0)
				{
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nDD %d~%d: ",i,i+7);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
				}
				
				DB_ReadCI(i,&dwValue);
							
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"%d	",dwValue);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';
			WriteComm(j,0,0);
			break;

		case CMD_TYPE_SHOWBIE:	
			DB_GetBIETail(&TailPtr);
			j = 0;
			
			if(m_nRecordRead != TailPtr)
			{
				DB_ReadBIE(&bie, m_nRecordRead);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nBIE %d: No=%d ST=%d \r\n",
					m_nRecordRead,bie.wNo,(bie.bStatus == BI_CLOSE)?1:0);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				m_nRecordRead = (m_nRecordRead+1)%MAX_BIE_NUM;
				WriteComm(j,0,0);
			}
			break;
			
		case CMD_TYPE_SHOWSOE:	
			DB_GetSOETail(&TailPtr);
			j = 0;
			if(m_nRecordRead != TailPtr)
			{
				DB_ReadSOE(&soe, m_nRecordRead);				
				m_TxdInfo.pBuf[j] = 0;
				AbsTimeTo(&soe.stTime,&tSysTime);
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nSOE %d: %d.%d.%d %d:%d:%d:%d No=%d ST=%d \r\n",
					m_nRecordRead,tSysTime.Year,tSysTime.Month,tSysTime.Day,\
					tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond,\
					soe.wNo,(soe.bStatus == BI_CLOSE)?1:0);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				m_nRecordRead = (m_nRecordRead+1)%MAX_SOE_NUM;
				WriteComm(j,0,0);
			}
			break;
			
		case CMD_TYPE_SHOWREP:
			//��ӡ�����澯����
			DB_GetRelayAlarmReportTail(&TailPtr);
			if(m_nALMRead != TailPtr)
			{
				j = 0;
				if(DB_ReadRelayAlarmReport(&AlarmRep, m_nALMRead) != DB_ERR_OK)
				{
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"<-----�����澯: %d is invalid----->\r\n",m_nALMRead);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					
					m_nALMRead = (m_nALMRead+1)%MAX_RALM_NUM;
					WriteComm(j,0,0);
					break;
				}
				
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"<-----�����澯: %d----->\r\n",m_nALMRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				DB_GetREDispByInf(AlarmRep.tEvent.tAction.nINF, &tREDisp);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"����: %s(%d)    \r\n",tREDisp.strCHName,AlarmRep.tEvent.tAction.nINF);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				tAbsTime.Hi = AlarmRep.tEvent.tAction.AbsTimeHi;
				tAbsTime.Lo = AlarmRep.tEvent.tAction.AbsTimeLo;
				AbsTimeTo(&tAbsTime,&tSysTime);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"ʱ��: %d.%d.%d %d:%d:%d:%d\r\n",\
					tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				pDataBuf = AlarmRep.aEvent+sizeof(TRelayRecordHead);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"����:");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				for(int m=0;m<AlarmRep.tEvent.tAction.FD_Num;m++)
				{
					wFDIndex = MAKEWORD(pDataBuf[m*6+0],pDataBuf[m*6+1]);
					dwValue = MAKEDWORD(pDataBuf[m*6+2],pDataBuf[m*6+3],pDataBuf[m*6+4],pDataBuf[m*6+5]);
					fFDVlaue = *((float *)&dwValue);

					m_TxdInfo.pBuf[j] = 0;
					DB_GetRMDisp(tREDisp.wRMList[m],&tRMDisp);									
					sprintf((char *)&m_TxdInfo.pBuf[j],"%s=%f  ",tRMDisp.strSymbol,fFDVlaue);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
				}

				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_nALMRead = (m_nALMRead+1)%MAX_RALM_NUM;
				WriteComm(j,0,0);

				break;
			}

			//��ӡ������������
			DB_GetRelayStartReportTail(&TailPtr);
			if(m_nSTTRead != TailPtr)
			{
				j = 0;
				DB_ReadRelayStartReport(&StartRep, m_nSTTRead);
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"<-----��������: %d----->\r\n",m_nSTTRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				DB_GetREDispByInf(StartRep.tEvent.tAction.nINF, &tREDisp);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"����: %s(%d)    \r\n",tREDisp.strCHName,StartRep.tEvent.tAction.nINF);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				tAbsTime.Hi = StartRep.tEvent.tAction.AbsTimeHi;
				tAbsTime.Lo = StartRep.tEvent.tAction.AbsTimeLo;
				AbsTimeTo(&tAbsTime,&tSysTime);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"ʱ��: %d.%d.%d %d:%d:%d:%d\r\n",\
					tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"����: ");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				pDataBuf = StartRep.aEvent+sizeof(TRelayRecordHead);
				for(int m=0;m<StartRep.tEvent.tAction.FD_Num;m++)
				{
					wFDIndex = MAKEWORD(pDataBuf[m*6+0],pDataBuf[m*6+1]);
					dwValue = MAKEDWORD(pDataBuf[m*6+2],pDataBuf[m*6+3],pDataBuf[m*6+4],pDataBuf[m*6+5]);
					fFDVlaue = *((float *)&dwValue);
								
					m_TxdInfo.pBuf[j] = 0;
					DB_GetRMDisp(tREDisp.wRMList[m],&tRMDisp);
					sprintf((char *)&m_TxdInfo.pBuf[j],"%s=%f  ",tRMDisp.strSymbol,fFDVlaue);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
				}

				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_nSTTRead = (m_nSTTRead+1)%MAX_RSTT_NUM;
				WriteComm(j,0,0);
				
				break;
			}

			//��ӡ������������
			DB_GetRelayActionReportTail(&TailPtr);
			if(m_nACTRead != TailPtr)
			{
				j = 0;
				if(DB_ReadRelayActionReport(&ActionRep, m_nACTRead) != DB_ERR_OK)
				{
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"<-----��������: %d is invalid----->\r\n",m_nACTRead);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					
					m_nACTRead = (m_nACTRead+1)%MAX_RACT_NUM;
					WriteComm(j,0,0);
					break;
				}
				
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"<-----��������: %d----->\r\n",m_nACTRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				dwOffset = 4;
				
				//������ֱ��Ĵ���
				for(int n=0;n<ActionRep.tEvent.Num;n++)
				{
					// 1��ȡһ���ֱ���ͷ
					memcpy(&tRelayEvent,ActionRep.aEvent+dwOffset,sizeof(TRelayEventHead));		

					// 2����ӡ������Ϣ
					DB_GetREDispByInf(tRelayEvent.nINF, &tREDisp);
					
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"�ֱ���%d\r\n����: %s(%d)    \r\n",n+1,tREDisp.strCHName,tRelayEvent.nINF);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);

					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"״̬: %s\r\n",\
						(tRelayEvent.bStatus==0)?"����":"����");
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
					
					tAbsTime.Hi = tRelayEvent.AbsTimeHi;
					tAbsTime.Lo = tRelayEvent.AbsTimeLo;
					AbsTimeTo(&tAbsTime,&tSysTime);

					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"ʱ��: %d.%d.%d %d:%d:%d:%d\r\n",\
						tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);

					// 3��ȡ������ӡ���ϵ���
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"����: ");
					j += strlen((char *)&m_TxdInfo.pBuf[j]);

					dwOffset += sizeof(tRelayEvent);
					pDataBuf = &ActionRep.aEvent[dwOffset];
					for(int m=0;m<tRelayEvent.FD_Num;m++)
					{
						wFDIndex = MAKEWORD(pDataBuf[m*6+0],pDataBuf[m*6+1]);
						dwValue = MAKEDWORD(pDataBuf[m*6+2],pDataBuf[m*6+3],pDataBuf[m*6+4],pDataBuf[m*6+5]);
						fFDVlaue = *((float *)&dwValue);

						m_TxdInfo.pBuf[j] = 0;
						DB_GetRMDisp(tREDisp.wRMList[m],&tRMDisp);											
						sprintf((char *)&m_TxdInfo.pBuf[j],"%s=%f  ",tRMDisp.strSymbol,fFDVlaue);
						j += strlen((char *)&m_TxdInfo.pBuf[j]);
					}

					// 4��������һ���ֱ���
					dwOffset += tRelayEvent.FD_Num*6;
						
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
				}
				
				m_nACTRead = (m_nACTRead+1)%MAX_RACT_NUM;
				WriteComm(j,0,0);

				break;
			}
			break;

		case CMD_TYPE_SHOWSAMPLE:
			if((m_dwTimerCnt%SHELL_SAMPLE_TIMER)!=0)
				break;
			
			j=0;
			if(DispReadPtr == 0)
			{
				memset((void*)DispDataBuf, 0, sizeof(DISP_DATA_LEN*sizeof(WORD)));
				GetDispData(DispDataBuf);

				m_TxdInfo.pBuf[j] = 0;
				strcpy((char *)&m_TxdInfo.pBuf[j],"\r\n  ԭʼ��������:\r\n");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			
			ptr = &DispDataBuf[DispReadPtr];
			m_TxdInfo.pBuf[j] = 0;
#if(SYSTEM_TYPE == _SYS_KF6500_)
			sprintf((char *)&m_TxdInfo.pBuf[j],\
				"%6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d \r\n",\
				ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],\
				ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15],\
				ptr[16], ptr[17], ptr[18], ptr[19], ptr[20], ptr[21]);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			
			DispReadPtr += CHANNNEL_NUM;
			DispReadPtr %= DISP_DATA_LEN;
			WriteComm(j,0,0);
#endif
#if(SYSTEM_TYPE == _SYS_KF1300_)
			sprintf((char *)&m_TxdInfo.pBuf[j],	"| %6d %6d %6d %6d %6d | %6d %6d | %6d %6d |\r\n",\
					ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7], ptr[8]);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			DispReadPtr += (ACHANNEL_NUM+DCHANNEL_NUM*2);
			DispReadPtr %= DISP_DATA_LEN;
			WriteComm(j,0,0);
#endif
			break;

		case CMD_TYPE_SHOWDISTDIR:
			BYTE			*pFileData;
			TDistIndex		*pDistList;
			char			*pFileName;
			STATUS			rc;

			if(!m_bCommandExute)
				break;
			
			dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*(G_pCfgTable->DBaseConfig.pDistPara->wRecordCapacity+1);
			pFileData = (BYTE *)OS_Malloc(dwFileLength);
			pDistList = (TDistIndex *)pFileData;
			j = 0;
			
			// 1����¼�������ļ�
			memset(pFileData,0x00,dwFileLength);
			rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
			if( rc == FILE_ERROR)
			{
				m_TxdInfo.pBuf[j] = 0;
				strcpy((char *)&m_TxdInfo.pBuf[j],"\r\n  ¼��Ŀ¼�ļ���ȡ�쳣!\r\n");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			else
			{
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n��ʼ��:%s\r\n",\
					(pDistList->wMagic==DIST_INDEX_INIT)?"����":"�쳣");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"����ʱ��: %x.%x.%x %x:%x:%x:%x\r\n",\
					pDistList->Year,\
					pDistList->Month,\
					pDistList->Day,\
					pDistList->Hour,\
					pDistList->Minute,\
					pDistList->Second,\
					MAKEWORD(pDistList->MSecondLo, pDistList->MSecondHi));
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n������:%d\r\n",pDistList->nMax);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				i=pDistList->nHeadIndex;
				do{
					m_TxdInfo.pBuf[j] = 0;

					pFileName = (char *)(pFileData+sizeof(TDistIndex)+i*DIST_INDEX_NAMELEN);
					if(strstr(pFileName,"INDEX"))
						sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nNo%d: %s.XXX\r\n",i,pFileName+6);
					else
						sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nNo%d: INVALID FILE\r\n",i);

					j += strlen((char *)&m_TxdInfo.pBuf[j]);

					i++;
					i %= pDistList->nMax;
				}while(i != pDistList->nHeadIndex);
			}
			WriteComm(j,0,0);
			break;

		case CMD_TYPE_SHOWRUNSTA:
		case CMD_TYPE_SHOWINITSTA:				
			j = 0;
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n%3d-%3d:",m_wShowCnt,m_wShowCnt+31);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			
			for(i=m_wShowCnt;(i<m_wShowCnt+32)&&(i<DAE_MAX_FLAGS);i++)
			{
				if((i%8) == 0)
				{
					m_TxdInfo.pBuf[j++] = ' ';
					m_TxdInfo.pBuf[j++] = ' ';
				}
				m_TxdInfo.pBuf[j] = 0;
				if(m_wCmdType == CMD_TYPE_SHOWRUNSTA)
					m_TxdInfo.pBuf[j] = DAE_GetRunningFlags(i)?'X':'-';
				else if(m_wCmdType == CMD_TYPE_SHOWINITSTA)
					m_TxdInfo.pBuf[j] = DAE_GetInitFlags(i)?'X':'-';
				j += 1;
			}
			m_wShowCnt += 32;
			if(m_wShowCnt >= m_wShowMax)
			{
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_wCmdType = CMD_TYPE_NULL;
			}
			
			WriteComm(j,0,0);
			break;
		case CMD_TYPE_GETTIME:
			TSysTime time;
			ReadSystemTime(&time);
			time.Year %= 100;
			
			j = 0;
			// �Ȳ�
			memset(m_TxdInfo.pBuf,' ',32);
			m_TxdInfo.pBuf[32] = '\r';
		
			// ��д
			sprintf((char *)m_TxdInfo.pBuf+33,"%2d��%2d��%2d�� %2dʱ%2d��%2d��%3d����.\r",\
				time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second, time.MSecond);
		
			j += 33+strlen((char *)&m_TxdInfo.pBuf[33]);
		
			WriteComm(j,0,0);
			break;
			
#if(SYSTEM_TYPE == _SYS_KF6500_)
		case CMD_TYPE_UPDATEDATA:
		case CMD_TYPE_UPDATECF:
			if(m_wCmdType == CMD_TYPE_UPDATEDATA)
				rc = DB_UpdateRelayData();
			else
				rc = DB_UpdateCFData();

		    if(rc == DB_ERR_OK)	
		    {
				sprintf((char *)m_TxdInfo.pBuf,"\r\nUpdate relay data from default file OK!!!");
		    }
			else
			{
				sprintf((char *)m_TxdInfo.pBuf,"\r\nUpdate relay data from default file Error!!!");
			}
			
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';
			WriteComm(j,0,0);
			
			m_wCmdType = CMD_TYPE_NULL;

			break;

		case CMD_TYPE_SHOWSYSVALUE:
			j = 0;
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n T=%2d.%1d�� Freq1=%d.%2dHz  Freq2=%d.%2dHz .",\
			SysTempture/10, SysTempture%10, GetFreqValue(0)/100, GetFreqValue(0)%100, GetFreqValue(1)/100, GetFreqValue(1)%100);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			TAbsTime AbsTime;
//			TSysTime time;
			AbsTime = PowerOnEvent.absTime;
			AbsTimeTo(&AbsTime, &time);
			
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n AD��λ����:%5d\r\n ",dwSampleErrCnt);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

/*			
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nͣ��ʱ��:%2d��%2d��%2d�� %2dʱ%2d��%2d��%3d����.",\
					time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second, time.MSecond);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			AbsTime.Lo = PowerOnEvent.dwDevName;
			AbsTime.Hi = PowerOnEvent.dwOperator;
			AbsTimeTo(&AbsTime, &time);
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n�ϵ�ʱ��:%2d��%2d��%2d�� %2dʱ%2d��%2d��%3d����.\r\n",
				time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second, time.MSecond);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
*/			

			WriteComm(j,0,0);
			break;

#else
		case CMD_TYPE_UPDATEDATA:
		case CMD_TYPE_UPDATECF:
			//�����������ļ����ݴ�FLASH�ļ�����EEPROM
			j = 0;
			m_TxdInfo.pBuf[j] = 0;

			if(m_wCmdType == CMD_TYPE_UPDATEDATA)
			{
				if(DB_ResetRelayData(~((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE))))			
					sprintf((char *)m_TxdInfo.pBuf,"Update relay data success!!!");
				else
					sprintf((char *)m_TxdInfo.pBuf,"Update relay data failure!!!");
			}
			else
			{
				if(DB_ResetRelayData((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE)))			
					sprintf((char *)m_TxdInfo.pBuf,"Update channel para success!!!");
				else
					sprintf((char *)m_TxdInfo.pBuf,"Update channel para failure!!!");
			}

			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';
			WriteComm(j,0,0);
				
			m_wCmdType = CMD_TYPE_NULL;

			break;
#endif
		case CMD_TYPE_RESETPARA:
			DB_ResetBasicConfig();
			sprintf((char *)m_TxdInfo.pBuf,"\r\nRecover basic config from file OK!!!");
			
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';
			WriteComm(j,0,0);
			
			m_wCmdType = CMD_TYPE_NULL;

			break;

		case CMD_TYPE_HELP:
			static BYTE bCurType;
			j = 0;
			m_TxdInfo.pBuf[j] = 0;
			if(bCurType == 0)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showbuf [netaX/netbX/serX/canX]  ---->��ʾ�˿ڹ�Լ����\r\n");
			else if(bCurType == 1)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showerr [-a]  ----------------------->��ʾ������Ϣ\r\n");
			else if(bCurType == 2)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showsam  ---------------------------->��ʾ��������\r\n");
			else if(bCurType == 3)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrec [-a]  ----------------------->��ʾ������¼\r\n");
			else if(bCurType == 4)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showyc  ----------------------------->��ʾң������\r\n");
			else if(bCurType == 5)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrm  ----------------------------->��ʾ��������ֵ\r\n");
			else if(bCurType == 6)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showyx  ----------------------------->��ʾң������\r\n");
			else if(bCurType == 7)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showdd  ----------------------------->��ʾ�������\r\n");
			else if(bCurType == 8)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showbie [-a]  ----------------------->��ʾ��λң�ż�¼\r\n");
			else if(bCurType == 9)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showsoe [-a]  ----------------------->��ʾSOE��¼\r\n");
			else if(bCurType == 10)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrep [-a]  ----------------------->��ʾ������������\r\n");
			else if(bCurType == 11)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n setbo X       ----------------------->ң�ز���:Ԥ��/ִ��/ֱ��ִ��/����\r\n");
			else if(bCurType == 12)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n dist dir  --------------------------->��ʾ¼����Ϣ\r\n");
			else if(bCurType == 13)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrun   --------------------------->��ʾ����״̬��Ϣ\r\n");
			else if(bCurType == 14)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showini   --------------------------->��ʾ��ʼ��״̬��Ϣ\r\n");
			else if(bCurType == 15)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n settime yy/mm/dd hh:mm:ss  ---------->��ʱ����\r\n");
			else if(bCurType == 16)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n readtime    ------------------------->��ϵͳʱ��");
			else if(bCurType == 17)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showvar [-mem/-eeprom] [pos]  ------->��ʾNvRam����EEPROM\r\n");
			else if(bCurType == 18)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showvar [-ser[n]/-eth[n]/-can[n]] [-t/-r]	-->��ʾ�˿��жϻ�����\r\n");
			else if(bCurType == 19)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showport  --------------------------->��ʾϵͳ�˿�����\r\n");
			else if(bCurType == 20)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showstat  --------------------------->��ʾϵͳ״̬\r\n");
			else if(bCurType == 21)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetdata  -------------------------->�ָ�������������\r\n");
			else if(bCurType == 22)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n reboot [-c]  ------------------------>�����豸\r\n");
			else if(bCurType == 23)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n clearevent--------------------------->���װ���¼�\r\n");
			else if(bCurType == 24)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetpara---------------------------->�ָ�EEPROM����\r\n");
			else if(bCurType == 25)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showmap------------------------------>��ʾ����λ����\r\n");
			else if(bCurType == 26)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showraw------------------------------>��ʾ��������ֵԭʼ����\r\n");
			else if(bCurType == 27)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n makerep [-alm] [no]------------------>ģ�����Ϊno�ı������߸澯�¼�\r\n");
#if(SYSTEM_TYPE == _SYS_KF6500_)
			else if(bCurType == 28)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetcoef  -------------------------->��ʼ��ϵ������ֵ\r\n");
			else if(bCurType == 29)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showsysval  ------------------------->��ϵͳ����\r\n");
			else if(bCurType == 30)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n key U/D/L/R/A/M/E/N  ---------------->���̿��Ʋ���\r\n");
			else if(bCurType == 31)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n lcdinf  ----------------------------->Һ�����������Ϣ\r\n");
			else if(bCurType == 32)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showfl  ----------------------------->��ʾ�����Ϣ\r\n");
#else
			else if(bCurType == 28)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetchan	-------------------------->��ʼ��ͨ������Ĭ��ֵ\r\n");
#endif
			else
			{
				bCurType = 0;
				m_wCmdType = CMD_TYPE_NULL;
				return;
			}
			bCurType++;
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			WriteComm(j,0,0);
			break;	

		case CMD_TYPE_SHOWPORT:
			TSerialPara 	tSerial;
			TCanBusPara		tCanbus;
			TEthPortPara	tLink;
			DWORD			dwBaud;
			BYTE			bCommStatus;
			
			j = 0;
			for(i=0;i<DB_GetSerialNum();i++)
			{
				m_TxdInfo.pBuf[j] = 0;
				DB_GetSerialPara(i,&tSerial);
				NumToBaud(tSerial.byBaud,&dwBaud);
				
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n SER%d: 0x%x   %d  ",MINOR_PORT(tSerial.wPortID),tSerial.wPortID,dwBaud);

				//����λ
				switch(tSerial.bFlag&0x03)
				{
					case 0:
						strcat((char *)&m_TxdInfo.pBuf[j],"8  ");break;
					case 1:
						strcat((char *)&m_TxdInfo.pBuf[j],"7  ");break;
					case 2:
						strcat((char *)&m_TxdInfo.pBuf[j],"6  ");break;
					case 3:
						strcat((char *)&m_TxdInfo.pBuf[j],"5  ");break;
				}
				//У��λ
				switch((tSerial.bFlag>>2)&0x03)
				{
					case 0:
						strcat((char *)&m_TxdInfo.pBuf[j],"N  ");break;
					case 1:
						strcat((char *)&m_TxdInfo.pBuf[j],"O  ");break;
					case 2:
						strcat((char *)&m_TxdInfo.pBuf[j],"D  ");break;
				}

				//ֹͣλ
				if((tSerial.bFlag>>4)&0x01)
					strcat((char *)&m_TxdInfo.pBuf[j],"2  ");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"1  ");
					

				//��Լ����
				GetProtocolName(tSerial.byDriver,(char *)&m_TxdInfo.pBuf[j]);
				
				if(DAE_GetCommStatus(tSerial.wPortID, bCommStatus) != DB_ERR_OK)
					strcat((char *)&m_TxdInfo.pBuf[j],"INV");
				else if(bCommStatus==COMM_STATUS_BREAK)
					strcat((char *)&m_TxdInfo.pBuf[j],"BREAK");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"OK");				

				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';

			
			for(i=0;i<DB_GetCANNum();i++)
			{
				m_TxdInfo.pBuf[j] = 0;
				DB_GetCANPara(i,&tCanbus);
				
				//������,��������
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n CAN%d: 0x%x   %d  0x%x    ",MINOR_PORT(tCanbus.wPortID),tCanbus.wPortID,tCanbus.wBaud,tCanbus.dwMask);

				//��Լ����
				GetProtocolName(tCanbus.byDriver,(char *)&m_TxdInfo.pBuf[j]);
				
				if(DAE_GetCommStatus(tCanbus.wPortID, bCommStatus) != DB_ERR_OK)
					strcat((char *)&m_TxdInfo.pBuf[j],"INV");
				else if(bCommStatus==COMM_STATUS_BREAK)
					strcat((char *)&m_TxdInfo.pBuf[j],"BREAK");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"OK");				

				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';

			for(i=0;i<DB_GetEthLinkNum();i++)
			{
				m_TxdInfo.pBuf[j] = 0;
				DB_GetETHPara(i,&tLink);
				
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n Eth%d: 0x%x   ",i+1,tLink.wPortID);

				// TCP��UDP
				if(tLink.dwMode & ETHLINK_MODE_UDP)
					strcat((char *)&m_TxdInfo.pBuf[j],"UDP  ");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"TCP	");
				 
				// �ͻ��˻��Ƿ�����
				if(tLink.dwMode & ETHLINK_MODE_CLIENT)
				{
					strcat((char *)&m_TxdInfo.pBuf[j],"Client	");
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%d.%d.%d.%d:%d	",HHBYTE(tLink.dwSideIP),HLBYTE(tLink.dwSideIP),\
						LHBYTE(tLink.dwSideIP),LLBYTE(tLink.dwSideIP),tLink.dwSidePort);
				}
				else
				{
					strcat((char *)&m_TxdInfo.pBuf[j],"Server	");
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%d.%d.%d.%d:%d	",HHBYTE(tLink.dwSelfIP),HLBYTE(tLink.dwSelfIP),\
						LHBYTE(tLink.dwSelfIP),LLBYTE(tLink.dwSelfIP),tLink.dwSelfPort);
				}		
				
				//��Լ����
				GetProtocolName(tLink.wDriver,(char *)&m_TxdInfo.pBuf[j]);

				if(DAE_GetCommStatus(tLink.wPortID, bCommStatus) != DB_ERR_OK)
					strcat((char *)&m_TxdInfo.pBuf[j],"INV");
				else if(bCommStatus==COMM_STATUS_BREAK)
					strcat((char *)&m_TxdInfo.pBuf[j],"BREAK");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"OK");				
				
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}

			WriteComm(j,0,0);

			m_wCmdType = CMD_TYPE_NULL;
			break;	

		case CMD_TYPE_SHOWMAPBIT:
			TMBO_Disp tMapObj;

			if(m_wShowMax == 0)
			{
				m_wCmdType = CMD_TYPE_NULL;
				strcat((char *)m_TxdInfo.pBuf,"No para!\r\n");
				WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
				break;
			}

			// ������һ��
			if((m_dwTimerCnt%(200/SHELL_BASIC_TIMER))!=0)
				break;
			
			m_TxdInfo.pBuf[j] = 0;

			// ��ӡ��ͷ
			if(m_wShowCnt==0)
				strcat((char *)m_TxdInfo.pBuf,"No   PORT  SAddr DAddr SideIP     Baud   SMBs  SMBn  RMBs  RMBn    COMM\r\n");
			
			sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"\r\n%2d:  ",m_wShowCnt+1);

			DB_GetMapBitObjDisp(m_wShowCnt,&tMapObj);
			// ����ģʽ
			switch(tMapObj.wControl& 0x000F)
			{
				case MAPBIT_PORTTYPE_ETHA:  strcat((char *)&m_TxdInfo.pBuf[j],"EthA  "); break;
				case MAPBIT_PORTTYPE_ETHB:  strcat((char *)&m_TxdInfo.pBuf[j],"EthB  "); break;
				case MAPBIT_PORTTYPE_SERIAL1:  strcat((char *)&m_TxdInfo.pBuf[j],"Ser1  "); break;
				case MAPBIT_PORTTYPE_SERIAL2:  strcat((char *)&m_TxdInfo.pBuf[j],"Ser2  "); break;
				case MAPBIT_PORTTYPE_SERIAL3:  strcat((char *)&m_TxdInfo.pBuf[j],"Ser3  "); break;
				case MAPBIT_PORTTYPE_CANA:  strcat((char *)&m_TxdInfo.pBuf[j],"CanA  "); break;
				case MAPBIT_PORTTYPE_CANB:  strcat((char *)&m_TxdInfo.pBuf[j],"CanB  "); break;
			}

			// ������Ϣ
			NumToBaud(tMapObj.dwBaud,&dwBaud);
			sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],\
				"%5d %5d 0x%8x %6d %5d %5d %5d %5d   ",\
				tMapObj.bySelfAddr,tMapObj.bySideAddr,tMapObj.dwSideIP,dwBaud,\
				tMapObj.bySMBStart,tMapObj.bySMBNum,tMapObj.byRMBStart,tMapObj.byRMBNum);

			// ͨ��״̬
			if(tMapObj.wControl&0x8000)
			{
				if(DAE_GetRunningFlags(SS_MAPBIT_COMM_ERR+m_wShowCnt))
					strcat((char *)&m_TxdInfo.pBuf[j],"BREAK");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"OK");
			}
			else//δͶ��
				strcat((char *)&m_TxdInfo.pBuf[j],"INV");
			
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			m_wShowCnt++;
			if(m_wShowCnt == m_wShowMax)
			{
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_wCmdType = CMD_TYPE_NULL;
			}
			
			WriteComm(j,0,0);			
			break;			
			
		case CMD_TYPE_VARMEM:
		case CMD_TYPE_VAREEPROM:			
			j = 0;

			// ��ӡ����
			if(m_wShowCnt == 0)
			{
				if(m_wCmdType == CMD_TYPE_VARMEM)
					sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n===============NvRAM:%8d-%8d===============\r\n",\
						m_dwOffset,m_dwOffset+512);
				else
					sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n===============EEProm:%6d-%6d===============\r\n",\
						m_dwOffset,m_dwOffset+512);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}

			// ���д�ӡ
			m_TxdInfo.pBuf[j] = 0;


			MakeHEX((char *)&m_TxdInfo.pBuf[j],m_wShowCnt+m_dwOffset,4);j += 4;	
			m_TxdInfo.pBuf[j++] = '-';
			MakeHEX((char *)&m_TxdInfo.pBuf[j],m_wShowCnt+m_dwOffset+15,4);j += 4;
			m_TxdInfo.pBuf[j++] = ':';
			m_TxdInfo.pBuf[j++] = ' ';
			
//			sprintf((char *)&m_TxdInfo.pBuf[j],"0x%x-0x%x:",m_wShowCnt+m_dwOffset,m_wShowCnt+m_dwOffset+15);
//			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			
			for(i=0;(i<16)&&(m_wShowCnt<m_wShowMax);m_wShowCnt++,i++)
			{
				m_TxdInfo.pBuf[j] = 0;
//				sprintf((char *)&m_TxdInfo.pBuf[j],"%2x ",m_DataBuffer[m_wShowCnt]);
//				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				MakeHEX((char *)&m_TxdInfo.pBuf[j],m_DataBuffer[m_wShowCnt],2);j += 2;
				m_TxdInfo.pBuf[j++] = ' ';
			}
			m_TxdInfo.pBuf[j++] = '\r';
			m_TxdInfo.pBuf[j++] = '\n';

			// �����ж�
			if(m_wShowCnt >= m_wShowMax)
				m_wCmdType = CMD_TYPE_NULL;

			WriteComm(j,0,0);
			break;	

		case CMD_TYPE_VARTXD:
			j = 0;

			if(HIBYTE(m_ShowPortID) == SC_PORT_SERIAL)
			{
				pSerial = GetSerial(LOBYTE(m_ShowPortID));
				if(pSerial == NULL)
					break;

				MaxLen = MISITXDBUFLEN;
				
				RnLen = pSerial->ReadNewTxdData(m_DataBuffer, m_dwOffset, 64);
			}
			else if(HIBYTE(m_ShowPortID) == SC_PORT_CANBUS)
			{
				MaxLen = CAN_MONITOR_BUF_LEN;
				
				RnLen = ReadNewCanTxdData(LOBYTE(m_ShowPortID),m_DataBuffer, m_dwOffset, 64);
			}
			else
			{
				pSocket = GetSortSocket(GetSocketID(m_ShowPortID));
				if(pSocket == NULL)
					break;

				MaxLen = TXDBUFLEN;
				
				RnLen = pSocket->ReadNewTxdData(m_DataBuffer, m_dwOffset, 64);
			}
			
			// ���д�ӡ			
			for(i=0;i<RnLen;i++,m_dwCount++)
			{
				if((m_dwCount%16) == 0)
				{
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
				}
				
				m_TxdInfo.pBuf[j] = 0;
				
				MakeHEX((char *)&m_TxdInfo.pBuf[j],m_DataBuffer[i],2);j += 2; 
				m_TxdInfo.pBuf[j++] = ' ';
				/*
				if(m_DataBuffer[i]<16)
					sprintf((char *)&m_TxdInfo.pBuf[j],"0%x ",m_DataBuffer[i]);
				else
					sprintf((char *)&m_TxdInfo.pBuf[j],"%x ",m_DataBuffer[i]);
				
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				*/
			}

			m_dwOffset = (m_dwOffset+RnLen)%MaxLen;		

			WriteComm(j,0,0);
			break;	
		case CMD_TYPE_VARRXD:						
			j = 0;

			if(HIBYTE(m_ShowPortID) == SC_PORT_SERIAL)
			{
				pSerial = GetSerial(LOBYTE(m_ShowPortID));
				if(pSerial == NULL)
					break;

				MaxLen = MISIRXDBUFLEN;
				
				RnLen = pSerial->ReadNewRxdData(m_DataBuffer, m_dwOffset, 64);
			}
			else if(HIBYTE(m_ShowPortID) == SC_PORT_CANBUS)
			{
				MaxLen = CAN_MONITOR_BUF_LEN;
				
				RnLen = ReadNewCanRxdData(LOBYTE(m_ShowPortID),m_DataBuffer, m_dwOffset, 64);
			}
			else
			{
				pSocket = GetSortSocket(GetSocketID(m_ShowPortID));
				if(pSocket == NULL)
					break;

				MaxLen = RXDBUFLEN;
				
				RnLen = pSocket->ReadNewRxdData(m_DataBuffer, m_dwOffset, 64);
			}
			
			// ���д�ӡ			
			for(i=0;i<RnLen;i++,m_dwCount++)
			{
				if((m_dwCount%16) == 0)
				{
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
				}
				
				m_TxdInfo.pBuf[j] = 0;
				
				MakeHEX((char *)&m_TxdInfo.pBuf[j],m_DataBuffer[i],2);j += 2; 
				m_TxdInfo.pBuf[j++] = ' ';
/*				
				if(m_DataBuffer[i]<16)
					sprintf((char *)&m_TxdInfo.pBuf[j],"0%x ",m_DataBuffer[i]);
				else
					sprintf((char *)&m_TxdInfo.pBuf[j],"%x ",m_DataBuffer[i]);
				
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
*/
			}

			m_dwOffset = (m_dwOffset+RnLen)%MaxLen;		

			WriteComm(j,0,0);
			break;	

		case CMD_TYPE_SHOWSTAT:
			
			j = 0;
			// DRAM:
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\nDRAM:\r\n  Total: %d\r\n  LastMalloc: %d\r\n\r\n",SYS_DRAM_LEN,((DWORD)SysMallocPtr-SYS_DRAM_BASE));

			// NVRAM:
			sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"NVRAM:\r\n  Total: %d\r\n  Malloced: %d\r\n  Left: %d\r\n\r\n",
				DBASE_NVRAM_LENGTH,G_pCfgTable->NvRamLink.pFree-DBASE_NVRAM_BASE,G_pCfgTable->NvRamLink.dwSize);

			// EEPROMRAM:
			sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"EEPROM:\r\n  Total: %d\r\n  Malloced: %d\r\n  Left: %d\r\n\r\n",
				0x20000,EEPROM_RELAY_SINGAL+EEPROM_RELAY_SINGALSIZE,0x20000-(EEPROM_RELAY_SINGAL+EEPROM_RELAY_SINGALSIZE));

			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			WriteComm(j,0,0);
			m_wCmdType = CMD_TYPE_NULL;
			break;

		case CMD_TYPE_REBOOT:
			if(m_RebootType == 0xAA)
			{
				sprintf((char *)m_TxdInfo.pBuf,"\r\n Are you sure cold restart(Y/N)?  ");
			}
			else
			{
				sprintf((char *)m_TxdInfo.pBuf,"\r\n Are you sure warm restart(Y/N)?  ");
			}
			WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
			m_wCmdTypeLast = CMD_TYPE_REBOOT;
			m_wCmdType = CMD_TYPE_ACK;
			break;
		
		case CMD_TYPE_CLEAREVENT:
			sprintf((char *)m_TxdInfo.pBuf,"\r\n Are you sure clear all event?(Y/N)?  ");
		
			WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
			m_wCmdTypeLast = CMD_TYPE_CLEAREVENT;
			m_wCmdType = CMD_TYPE_ACK;
			break;

#if(SYSTEM_TYPE == _SYS_KF6500_)
		case CMD_TYPE_LCDINF:
			j=0;
			float value = SysTempture;
			value /= 10;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n װ���¶�:%.2f��  Һ���ԱȶȲ���:%d\r\n",value, g_wLCDContrast);
			WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
			m_wCmdType = CMD_TYPE_NULL;
			break;
#endif			
		case CMD_TYPE_ACK:
			break;

		case CMD_TYPE_MAKEREP:
		case CMD_TYPE_MAKEALM:
			if(DB_GetREDisp(m_dwOffset, &tREDisp) != DB_ERR_OK)
				sprintf((char *)m_TxdInfo.pBuf,"\r\nģ���%d�ű���ʧ��:(\r\n",m_dwOffset);
			else
			{
				if(m_wCmdType == CMD_TYPE_MAKEREP)
				{
					SimActionReport(m_dwOffset);
					sprintf((char *)m_TxdInfo.pBuf,"\r\nģ���%d�Ŷ�������ɹ�! \r\n\r\n��������: %s \r\n������: %d \r\n��������: %d\r\n",\
						m_dwOffset,tREDisp.strCHName,tREDisp.wInf,tREDisp.nFD);
				}
				else
				{
					Report_Alarm(m_dwOffset);
					sprintf((char *)m_TxdInfo.pBuf,"\r\nģ���%d�Ÿ澯����ɹ�! \r\n\r\n��������: %s \r\n������: %d \r\n��������: %d\r\n",\
						m_dwOffset,tREDisp.strCHName,tREDisp.wInf,tREDisp.nFD);
				}
				
			}
			
			WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
			m_wCmdType = CMD_TYPE_NULL;
			break;
			
		
		case CMD_TYPE_SHOWFL:
#if(SYSTEM_TYPE == _SYS_KF6500_)
			TFL_Disp tFLObj;
			BYTE	bCfgStatus;
			long	lSetvalue;
		
			if(m_wShowMax == 0)
			{
				m_wCmdType = CMD_TYPE_NULL;
				strcat((char *)m_TxdInfo.pBuf,"No para!\r\n");
				WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
				break;
			}
		
			// ������һ��
			if((m_dwTimerCnt%(200/SHELL_BASIC_TIMER))!=0)
				break;
			
			m_TxdInfo.pBuf[j] = 0;
		
			// ��ӡ��ͷ
			if(m_wShowCnt==0)
			{
				DB_GetCFGStatus(SET_AREA_CURRENT, 10, bCfgStatus);
				if(bCfgStatus == CFG_STATUS_SET)
				{
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"\r\n�豸����: ����\r\n"); 
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"��������: %d\r\n",DB_GetFLObjNum());
				}
				else
				{
					m_wShowMax = 1;
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�豸����: ����\r\n"); 
				}
				DB_GetSETFixValue(SET_AREA_CURRENT,44, &lSetvalue);
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�豸���: %d\r\n",lSetvalue);
				
				if(DB_GetCurrentSETAreaNo() == 0)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�������: ����\r\n");
				else if(DB_GetCurrentSETAreaNo() == 1)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�������: ����Խ��\r\n");
				else if(DB_GetCurrentSETAreaNo() == 2)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�������: �Բ�Խ��\r\n");
				else
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�������: δ֪\r\n");
									
				DB_GetSETFixValue(SET_AREA_CURRENT,43, &lSetvalue);
				if(lSetvalue == 1)
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"���ӷ�ʽ: ����\r\n");
				else if(lSetvalue == 2)
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"���ӷ�ʽ: ͸��\r\n");
				else if(lSetvalue == 0)
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"���ӷ�ʽ: ����\r\n");
				else
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"���ӷ�ʽ: δ֪\r\n");
			}
			else
			{
				if(m_wShowCnt == 1)
					strcat((char *)m_TxdInfo.pBuf,"\r\nSubNo   TYPE    IPA		 IPB	   COMM\r\n");
		
				// �������
				DB_GetFLObjDisp(m_wShowCnt-1,&tFLObj);
				
				if(DB_GetCurrentSETAreaNo() == 0)
				{
					// �����޳��Բ�
					if(tFLObj.wControl&FL_CTRLMASK_SIDE)
					{
						m_wShowCnt++;
						if(m_wShowCnt == m_wShowMax)
							m_wCmdType = CMD_TYPE_NULL;
						return;
					}
				}
				else if(DB_GetCurrentSETAreaNo() == 1)
				{
					// �����޳���Ч
					if(!(tFLObj.wControl&FL_CTRLMASK_STEPOVERVAL))
					{
						m_wShowCnt++;
						if(m_wShowCnt == m_wShowMax)
							m_wCmdType = CMD_TYPE_NULL;
						return;
					}
				}
				
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"\r\n%2d      ",tFLObj.byFLNo);
		
				// �豸����
				if(tFLObj.wControl&FL_CTRLMASK_SIDE)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"�Բ�����%d",tFLObj.byFLNo);
				else
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"��������%d",tFLObj.byFLNo);
/*					
				switch(tFLObj.wControl&0x000F)
				{
					case FL_SUBST_SELFAT:  strcat((char *)&m_TxdInfo.pBuf[j],"����AT	"); break;
					case FL_SUBST_SELFSP:  strcat((char *)&m_TxdInfo.pBuf[j],"����SP	"); break;
					case FL_SUBST_SIDESP:  strcat((char *)&m_TxdInfo.pBuf[j],"�Բ�SP	"); break;
					case FL_SUBST_SIDEAT:  strcat((char *)&m_TxdInfo.pBuf[j],"�Բ�AT	"); break;
					case FL_SUBST_SIDEMAIN:  strcat((char *)&m_TxdInfo.pBuf[j],"�Բ�����  "); break;
				}
*/		
				// ������
				//sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%4x	",tFLObj.wControl);
				
				// IPA
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%8x	",tFLObj.dwMNetIP);
		
				// IPB
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%8x	",tFLObj.dwSNetIP);
		
				// ͨ��״̬
				if(DAE_GetRunningFlags(SS_FL1_COMM_ERR+m_wShowCnt-1))
					strcat((char *)&m_TxdInfo.pBuf[j],"BREAK");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"OK");
			}
			
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
			m_wShowCnt++;
			if(m_wShowCnt == m_wShowMax)
			{
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';
				m_wCmdType = CMD_TYPE_NULL;
			}
			
			WriteComm(j,0,0);	
#endif			
			break;			
		
		default:
			m_wCmdType = CMD_TYPE_NULL;
			break;
	}
}

void CWHShell::ClearCommand(char *pCmdEnd)
{
	int i,j;
	BYTE	len;
	
	//���ջ���ָ������
	if(m_CommandPtr >= MAX_FRAME_LEN)//дָ��>��������С
	{
		m_CommandPtr = 0;
		memset(m_CommandBuffer,0,MAX_FRAME_LEN);
		return;
	}
	
	//�ϴ������
	len = pCmdEnd-m_CommandBuffer-1;
	if(len>64)
		len = 64;

	// ��С��ָ��
	if(len>1)
	{
		memcpy(m_LastCommand,m_CommandBuffer,len);
		m_LastCommand[len] = '\0';
	}
	
	//��������ݽ�ǰ������ݸ���
	for(i=pCmdEnd-m_CommandBuffer,j=0; i< m_CommandPtr; i++)
	{		
		m_CommandBuffer[j++] = m_CommandBuffer[i];
	}

	//��������ָ��
	m_CommandPtr = j;

	//ʣ��ռ���0
	while(j<MAX_FRAME_LEN)
		m_CommandBuffer[j++] = 0;
}

void CWHShell::ProcessAckCommand()
{
	WORD wOutLen = 0;
	
	if(strstr(m_CommandBuffer,"y"))
	{
		switch(m_wCmdTypeLast)
		{
			case CMD_TYPE_REBOOT:
				sprintf((char *)m_TxdInfo.pBuf,"\r\n\ Now start reset,the device will restart for a while......\r\n ");
				WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
				
				if(m_RebootType == 0xAA)
					DAE_ResetDevice(1);
				else
					DAE_ResetDevice(0);
				break;
				
			case CMD_TYPE_CLEAREVENT:
				sprintf((char *)m_TxdInfo.pBuf,"\r\n\ Now start clear,please wait ...... \r\n ");
				WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);

				if(DB_ClearEvent(0xFFFF) == DB_ERR_OK)
					sprintf((char *)m_TxdInfo.pBuf,"\r\n Clear event record OK!!!");
				else
					sprintf((char *)m_TxdInfo.pBuf,"\r\n Clear event record failure!!!");
					
				wOutLen = strlen((char *)m_TxdInfo.pBuf);
				m_TxdInfo.pBuf[wOutLen++] = '\r';
				m_TxdInfo.pBuf[wOutLen++] = '\n';
				WriteComm(wOutLen,0,0);
				break;	

			default:
				break;
		}
		
		m_wCmdType = CMD_TYPE_NULL;
	}	
	else if(strstr(m_CommandBuffer,"n"))
	{
		sprintf((char *)m_TxdInfo.pBuf,"\r\n\ Command quit! \r\n ");
		WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
		
		m_wCmdType = CMD_TYPE_NULL;
	}
}

