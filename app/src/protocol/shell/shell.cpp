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
extern int SysTempture;		//装置温度值,放大10倍
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
/*函数名：sCanBusTask					       					*/
/*功能：CANBUS从站规约任务				       					*/
/*输入: 无						       							*/
/*输出：无						       							*/
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
ON_SM_NOTIFYETHLINKED(CWHShell)  //以太网建立连接
ON_SM_NOTIFYETHCLOSED(CWHShell)  //以太网关闭连接
END_MESSAGE_MAP()

BOOL CWHShell::Init(void)
{
	SetBufLen(512,4096);
	
	//初始化规约基类
	if(CProtocol::Init() != TRUE)
		return FALSE;
	
	StartMsg(MCC_CommIdle,100);
	StartMsg(MCC_ReadComm,MIN_FRAME_LEN);	

	//创建CAN基本定时器
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
/*函数名：	CanTxdMonitor					*/
/*功能： 	CANBUS从站规约发送管理函数			*/
/*输入: 	无						*/
/*输出：	无						*/
/*--------------------------------------------------------------*/
void CWHShell::TxdMonitor()
{
	return;
}

/*-------------------------------------------------------------*/
/*函数名：CanSearchOneFrame					*/
/*功能：  CANBUS从站规约搜帧函数		       		*/
/*输入:   PBYTE Buf		缓冲区指针			*/
/*	  WORD wLen		搜寻缓冲区长度、	       */
/*输出：  无						       */
/*-------------------------------------------------------------*/

DWORD CWHShell::SearchOneFrame(PBYTE Buf,WORD wLen)
{
	int 	i,j=0;
		
	if(wLen == 0)
		return FM_LESS;
	
	//接收指令
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
		// 0x08-->退格
		if(Buf[i] == '\r')
		{
			//打印成回车换行
			m_TxdInfo.pBuf[j++] = Buf[i];
			m_TxdInfo.pBuf[j++] = '\n';
		}
		else if(Buf[i] == '\n')
		{
			// 剔除掉 "\n"
			m_CommandPtr--;
		}
		else if((Buf[i] == 0x03)||(Buf[i] == 0x05)||(Buf[i] == 0x1A))
		{
			//不打印
		}
		else if(Buf[i] == 0x08)
		{
			m_CommandPtr--;

			//删除前一个字符
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
		// 搜索向上箭头
		if(m_CommandPtr >= 3)
		{
			if(m_CommandBuffer[m_CommandPtr-3] == 27)
			if(m_CommandBuffer[m_CommandPtr-2] == 91)
			if(m_CommandBuffer[m_CommandPtr-1] == 97)
			{
				m_CommandPtr -= 3;

				// 刷命令
				strcpy((char *)m_CommandBuffer+m_CommandPtr,m_LastCommand);
				m_CommandPtr += strlen(m_LastCommand);

				// 刷回显
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
/*函数名：CanOnFrame					*/
/*功能：  CANBUS从站规约解帧函数		       		*/
/*输入:   PBYTE Buf		帧缓冲区头指针			*/
/*输出：  无						       */
/*-------------------------------------------------------------*/
void CWHShell::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	TxdMonitor();
}

/*-------------------------------------------------------------*/
/*函数名：CanOnFrame					*/
/*功能：  CANBUS从站规约解帧函数		       		*/
/*输入:   PBYTE Buf		帧缓冲区头指针			*/
/*输出：  无						       */
/*-------------------------------------------------------------*/
void CWHShell::OnFrame(PBYTE Buf)
{
	//处理通信状态
	m_NoRxdCounter = MAX_COMMERR_TIME;
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	
	SearchCommand();
}


/*-------------------------------------------------------------*/
/*函数名：CanOnTimeOut					       */
/*功能：CANBUS从站规约定时器响应函数			       */
/*输入: DWORD id	定时器ID			       */
/*输出：无						       */
/*-------------------------------------------------------------*/
void CWHShell::OnTimeOut(DWORD id)
{	
	m_dwTimerCnt++;

	if(m_NoRxdCounter > 0)
	{
		// 倒计时
		m_NoRxdCounter--;

		// 通信失败时报错
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

	//先检查退出
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

		//打开关闭回显
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
	
	// 默认清除回车符之前的内容
	pCmdEnd = strstr(m_CommandBuffer,"\r");
	if(pCmdEnd == NULL)
		return FALSE;
	
	pCmdEnd++;
	
	//开始搜索
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
			//   显示NvRam / EEPROM
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

				// 正确读取时才能显示
				if(nRtn > 0)
				{
					bCommand = TRUE;
					m_wShowCnt = 0;
					m_wShowMax = nRtn;
				}
				break;
			}

			// 显示串口收发缓冲区			
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

			// 显示以太网口收发缓冲区			
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

			// 显示CAN网收发缓冲区			
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
	else if(strstr(m_CommandBuffer,"setbo"))	//遥控操作
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
	else if(strstr(m_CommandBuffer, "settime"))//对时命令
	{	//格式为:st 08/10/11 15:23:50			
		WORD data[8];
		int i = 0;
		TSysTime time;

		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SETTIME;

		m_bShowAll = FALSE;

		//在B格式时钟无效的情况下，采用通信规约对钟，并同时更新硬件时钟
		if(DAE_GetRunningFlags(SS_TIME_B))
		{
			//转换时间信息
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
	else if(strstr(m_CommandBuffer, "readtime"))	//帮助命令
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_GETTIME;

		m_TxdInfo.pBuf[0] = '\r';
		m_TxdInfo.pBuf[1] = '\n';
		WriteComm(2,0,0);
	}
	else if(strstr(m_CommandBuffer, "showsysval"))	//读取系统测量数据
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_SHOWSYSVALUE;
	}
	else if(strstr(m_CommandBuffer, "resetdata"))	//从FLASH更新保护数据
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_UPDATEDATA;
	}
#if(SYSTEM_TYPE == _SYS_KF6500_)
	else if(strstr(m_CommandBuffer, "resetcoef"))	//从FLASH更新系数数据
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_UPDATECF;
	}
#else
	else if(strstr(m_CommandBuffer, "resetchan"))	//从FLASH更新系数数据
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_UPDATECF;
	}
#endif
	else if(strstr(m_CommandBuffer, "resetpara"))	//从FLASH更新装置参数
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_RESETPARA;
	}
	else if(strstr(m_CommandBuffer, "clearevent"))	//从FLASH更新保护数据
	{
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_CLEAREVENT;
	}
	else if(strstr(m_CommandBuffer, "help"))	//帮助命令
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
	else if(strstr(m_CommandBuffer,"key "))	//遥控操作
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
	else if(strstr(m_CommandBuffer, "lcdinf"))	//显示液晶信息
	{
		m_wShowCnt = 0;
		bCommand = TRUE;
		m_wCmdType = CMD_TYPE_LCDINF;
	}
#endif	
	// 模拟报告
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
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x00<<8)+1,0,0,0);	//发送消息停止召唤子所数据				
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl2"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x01<<8)+1,0,0,0);	//发送消息停止召唤子所数据				
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl3"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x02<<8)+1,0,0,0);	//发送消息停止召唤子所数据				
	}
	else if(pCmdStart = strstr(m_CommandBuffer,"stopfl4"))
	{
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL),SM_STOPCALLDATA,(0x03<<8)+1,0,0,0);	//发送消息停止召唤子所数据				
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

	// 对确认的特殊处理
	if(m_wCmdType == CMD_TYPE_ACK)
	{
		ProcessAckCommand();
	}
	
	//记录一次新的指令执行
	m_bCommandExute = TRUE;

	//执行本次指令
	ExuteCommand();

	m_bCommandExute = FALSE;		

	//清除本次指令
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

				//每次最多打印10条
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
				sprintf((char *)&m_TxdInfo.pBuf[j],"---------操作记录%d-------- \r\n",m_nRecordRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				m_TxdInfo.pBuf[j] = 0;
				switch(tMiscEvent.nClass)
				{
				case MISC_CLASS_DOWNFILE:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 文件下载 \r\n");
					break;
				case MISC_CLASS_RESET:	
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 装置复位 \r\n");
					break;
				case MISC_CLASS_COMM:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 通信告警 \r\n");
					break;
				case MISC_CLASS_AO:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 遥调操作 \r\n");
					break;
				case MISC_CLASS_BO:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 遥控操作 \r\n");
					break;
				case MISC_CLASS_SYNC:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 同期操作 \r\n");
					break;
				case MISC_CLASS_SET:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 定值修改 \r\n");
					break;
				case MISC_CLASS_SETAREA:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 定值区投切 \r\n");
					break;
				case MISC_CLASS_SW:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 软压板投退 \r\n");
					break;
				case MISC_CLASS_CFG:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 设置配置字 \r\n");
					break;
				case MISC_CLASS_CF:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 设置系数 \r\n");
					break;
#if (SYSTEM_TYPE == _SYS_KF1300_)
				case MISC_CLASS_ZERODRIFT:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 零漂整定 \r\n");
					break;
				case MISC_CLASS_FULLVALUE:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 满值整定 \r\n");
					break;
				case MISC_CLASS_CHANNELSRC:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 输入通道源投退 \r\n");
					break;
//				case MISC_CLASS_CH:
//					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 通道参数修改 \r\n");
					break;
#endif
				case MISC_CLASS_PARA:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 参数修改 \r\n");
					break;
				case MISC_CLASS_POWER:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 装置掉电 \r\n");
					break;
				case MISC_CLASS_CLEARCI:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 电度清零 \r\n");
					break;
				case MISC_CLASS_MODIAI:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 修改遥测 \r\n");
					break;
				case MISC_CLASS_MODIBI:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 修改遥信 \r\n");
					break;
				case MISC_CLASS_MODICI:
					strcat((char *)&m_TxdInfo.pBuf[j],"类型: 修改电度 \r\n");
					break;
				default: 	
					sprintf((char *)&m_TxdInfo.pBuf[j],"类型: 未知操作(%d) \r\n",tMiscEvent.nClass);
					break;
				}
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				AbsTimeTo(&tMiscEvent.absTime, &tSysTime);
				sprintf((char *)&m_TxdInfo.pBuf[j],"时间: %d.%d.%d %d:%d:%d:%d \r\n",\
					tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				DB_GetPortName(tMiscEvent.dwOperator,strTemp);
				sprintf((char *)&m_TxdInfo.pBuf[j],"任务: %s \r\n",strTemp);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				switch(tMiscEvent.nClass)
				{
				case MISC_CLASS_DOWNFILE:
					strTemp[0] = '\0';
					switch(tMiscEvent.dwValue)
					{
					case 1:
						sprintf(strTemp,"文件: 基本参数");break;
					case 2:
						sprintf(strTemp,"文件: 保护数据");break;
					case 3:
						sprintf(strTemp,"文件: 保护描述");break;
					case 4:
						sprintf(strTemp,"文件: 运行程序");break;
					default:
						sprintf(strTemp,"文件: 其他参数");break;
					}
					sprintf((char *)&m_TxdInfo.pBuf[j],"%s\r\n结果: %s\r\n",strTemp,\
						tMiscEvent.dwAttr==0?"成功":"失败");
					break;
					
				case MISC_CLASS_RESET:	
//					sprintf((char *)&m_TxdInfo.pBuf[j],"状态: %s \r\n",tMiscEvent.dwValue==SYS_RESET_WARM?"热启动":"冷启动");
					sprintf((char *)&m_TxdInfo.pBuf[j],"状态: 0x%x \r\n",tMiscEvent.dwValue);
					break;
					
				case MISC_CLASS_COMM:
					sprintf((char *)&m_TxdInfo.pBuf[j],"通信端口: %x \r\n通信状态: %s \r\n",\
						tMiscEvent.dwAttr,tMiscEvent.dwValue==0?"OK":"Err");
					break;
					
				case MISC_CLASS_AO:
					sprintf((char *)&m_TxdInfo.pBuf[j],"序号: %d \r\n数值: %d \r\n",\
						tMiscEvent.dwValue,tMiscEvent.dwAttr);
					break;
					
				case MISC_CLASS_BO:
					strTemp[0] = '\0';
					switch(tMiscEvent.dwAttr)
					{
					case BO_NULL: strcat(strTemp,"空");break;
					case BO_OPEN: strcat(strTemp,"分");break;
					case BO_CLOSE: strcat(strTemp,"合");break;
					default: strcat(strTemp,"无效");break;
					}
					sprintf((char *)&m_TxdInfo.pBuf[j],"序号: %d \r\n属性: %s \r\n",\
						tMiscEvent.dwValue,strTemp);
					break;
					
				case MISC_CLASS_SET:
					sprintf((char *)&m_TxdInfo.pBuf[j],"设置序号: %d-%d \r\n",\
						HIWORD(tMiscEvent.dwValue),LOWORD(tMiscEvent.dwValue));
					break;
					
				case MISC_CLASS_SETAREA:
					sprintf((char *)&m_TxdInfo.pBuf[j],"投入区号: %d \r\n",tMiscEvent.dwValue);
					break;
					
				case MISC_CLASS_SW:
					sprintf((char *)&m_TxdInfo.pBuf[j],"序号: %d \r\n状态: %s\r\n",\
						tMiscEvent.dwValue,tMiscEvent.dwAttr==SW_STATUS_CUT?"切":"投");
					break;
					
				case MISC_CLASS_CFG:
					sprintf((char *)&m_TxdInfo.pBuf[j],"序号: %d \r\n状态: %s\r\n",\
						tMiscEvent.dwValue,tMiscEvent.dwAttr==CFG_STATUS_CLR?"切":"投");
					break;
					
				case MISC_CLASS_CF:
#if (SYSTEM_TYPE == _SYS_KF1300_)
				case MISC_CLASS_ZERODRIFT:
				case MISC_CLASS_FULLVALUE:
				case MISC_CLASS_CHANNELSRC:
#endif					
					sprintf((char *)&m_TxdInfo.pBuf[j],"序号: %d \r\n",tMiscEvent.dwValue);
					break;
					
				case MISC_CLASS_PARA:
					strTemp[0] = '\0';
					switch(tMiscEvent.dwValue)
					{
					case DB_PARA_DAI:	strcat(strTemp,"模拟采样点");break;
					case DB_PARA_DI:	strcat(strTemp,"数字采样点");break;
					case DB_PARA_AI:	strcat(strTemp,"遥测");break;
					case DB_PARA_BI:	strcat(strTemp,"遥信");break;
					case DB_PARA_CI:	strcat(strTemp,"电度");break;
					case DB_PARA_BO:	strcat(strTemp,"遥控");break;
					case DB_PARA_AO:	strcat(strTemp,"遥调");break;
					case DB_PARA_SER:	strcat(strTemp,"串口");break;
					case DB_PARA_CAN:	strcat(strTemp,"CAN");break;
					case DB_PARA_ETH:	strcat(strTemp,"以太网");break;
					case DB_PARA_CHANNEL: strcat(strTemp,"通道");break;
					case DB_PARA_MBO:	strcat(strTemp,"镜像位");break;
					default:			strcat(strTemp,"未知");break;
					}
					sprintf((char *)&m_TxdInfo.pBuf[j],"参数: %s \r\n序号: %d \r\n",\
						strTemp,tMiscEvent.dwAttr);
					break;
					
				case MISC_CLASS_POWER:
					sprintf((char *)&m_TxdInfo.pBuf[j],"状态: %s",tMiscEvent.dwValue==0?"上电":"停电");
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

				//每次最多打印10条
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
			//打印保护告警报告
			DB_GetRelayAlarmReportTail(&TailPtr);
			if(m_nALMRead != TailPtr)
			{
				j = 0;
				if(DB_ReadRelayAlarmReport(&AlarmRep, m_nALMRead) != DB_ERR_OK)
				{
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"<-----保护告警: %d is invalid----->\r\n",m_nALMRead);
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
				sprintf((char *)&m_TxdInfo.pBuf[j],"<-----保护告警: %d----->\r\n",m_nALMRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				DB_GetREDispByInf(AlarmRep.tEvent.tAction.nINF, &tREDisp);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"名称: %s(%d)    \r\n",tREDisp.strCHName,AlarmRep.tEvent.tAction.nINF);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				tAbsTime.Hi = AlarmRep.tEvent.tAction.AbsTimeHi;
				tAbsTime.Lo = AlarmRep.tEvent.tAction.AbsTimeLo;
				AbsTimeTo(&tAbsTime,&tSysTime);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"时间: %d.%d.%d %d:%d:%d:%d\r\n",\
					tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				pDataBuf = AlarmRep.aEvent+sizeof(TRelayRecordHead);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"电量:");
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

			//打印保护启动报告
			DB_GetRelayStartReportTail(&TailPtr);
			if(m_nSTTRead != TailPtr)
			{
				j = 0;
				DB_ReadRelayStartReport(&StartRep, m_nSTTRead);
				m_TxdInfo.pBuf[j++] = '\r';
				m_TxdInfo.pBuf[j++] = '\n';

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"<-----保护启动: %d----->\r\n",m_nSTTRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				DB_GetREDispByInf(StartRep.tEvent.tAction.nINF, &tREDisp);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"名称: %s(%d)    \r\n",tREDisp.strCHName,StartRep.tEvent.tAction.nINF);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				tAbsTime.Hi = StartRep.tEvent.tAction.AbsTimeHi;
				tAbsTime.Lo = StartRep.tEvent.tAction.AbsTimeLo;
				AbsTimeTo(&tAbsTime,&tSysTime);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"时间: %d.%d.%d %d:%d:%d:%d\r\n",\
					tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"电量: ");
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

			//打印保护动作报告
			DB_GetRelayActionReportTail(&TailPtr);
			if(m_nACTRead != TailPtr)
			{
				j = 0;
				if(DB_ReadRelayActionReport(&ActionRep, m_nACTRead) != DB_ERR_OK)
				{
					m_TxdInfo.pBuf[j++] = '\r';
					m_TxdInfo.pBuf[j++] = '\n';
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"<-----保护动作: %d is invalid----->\r\n",m_nACTRead);
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
				sprintf((char *)&m_TxdInfo.pBuf[j],"<-----保护动作: %d----->\r\n",m_nACTRead);
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				dwOffset = 4;
				
				//再逐个分报文处理
				for(int n=0;n<ActionRep.tEvent.Num;n++)
				{
					// 1。取一个分报文头
					memcpy(&tRelayEvent,ActionRep.aEvent+dwOffset,sizeof(TRelayEventHead));		

					// 2。打印报告信息
					DB_GetREDispByInf(tRelayEvent.nINF, &tREDisp);
					
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"分报告%d\r\n名称: %s(%d)    \r\n",n+1,tREDisp.strCHName,tRelayEvent.nINF);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);

					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"状态: %s\r\n",\
						(tRelayEvent.bStatus==0)?"复归":"动作");
					j += strlen((char *)&m_TxdInfo.pBuf[j]);
					
					tAbsTime.Hi = tRelayEvent.AbsTimeHi;
					tAbsTime.Lo = tRelayEvent.AbsTimeLo;
					AbsTimeTo(&tAbsTime,&tSysTime);

					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"时间: %d.%d.%d %d:%d:%d:%d\r\n",\
						tSysTime.Year,tSysTime.Month,tSysTime.Day,tSysTime.Hour,tSysTime.Minute,tSysTime.Second,tSysTime.MSecond);
					j += strlen((char *)&m_TxdInfo.pBuf[j]);

					// 3。取出并打印故障电量
					m_TxdInfo.pBuf[j] = 0;
					sprintf((char *)&m_TxdInfo.pBuf[j],"电量: ");
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

					// 4。递增下一个分报告
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
				strcpy((char *)&m_TxdInfo.pBuf[j],"\r\n  原始采样数据:\r\n");
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
			
			// 1。打开录波索引文件
			memset(pFileData,0x00,dwFileLength);
			rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
			if( rc == FILE_ERROR)
			{
				m_TxdInfo.pBuf[j] = 0;
				strcpy((char *)&m_TxdInfo.pBuf[j],"\r\n  录波目录文件读取异常!\r\n");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
			}
			else
			{
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n初始化:%s\r\n",\
					(pDistList->wMagic==DIST_INDEX_INIT)?"正常":"异常");
				j += strlen((char *)&m_TxdInfo.pBuf[j]);

				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"更新时间: %x.%x.%x %x:%x:%x:%x\r\n",\
					pDistList->Year,\
					pDistList->Month,\
					pDistList->Day,\
					pDistList->Hour,\
					pDistList->Minute,\
					pDistList->Second,\
					MAKEWORD(pDistList->MSecondLo, pDistList->MSecondHi));
				j += strlen((char *)&m_TxdInfo.pBuf[j]);
				
				m_TxdInfo.pBuf[j] = 0;
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n最大个数:%d\r\n",pDistList->nMax);
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
			// 先擦
			memset(m_TxdInfo.pBuf,' ',32);
			m_TxdInfo.pBuf[32] = '\r';
		
			// 再写
			sprintf((char *)m_TxdInfo.pBuf+33,"%2d年%2d月%2d日 %2d时%2d分%2d秒%3d毫秒.\r",\
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
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n T=%2d.%1d° Freq1=%d.%2dHz  Freq2=%d.%2dHz .",\
			SysTempture/10, SysTempture%10, GetFreqValue(0)/100, GetFreqValue(0)%100, GetFreqValue(1)/100, GetFreqValue(1)%100);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			TAbsTime AbsTime;
//			TSysTime time;
			AbsTime = PowerOnEvent.absTime;
			AbsTimeTo(&AbsTime, &time);
			
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n AD复位次数:%5d\r\n ",dwSampleErrCnt);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

/*			
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n停电时间:%2d年%2d月%2d日 %2d时%2d分%2d秒%3d毫秒.",\
					time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second, time.MSecond);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);

			AbsTime.Lo = PowerOnEvent.dwDevName;
			AbsTime.Hi = PowerOnEvent.dwOperator;
			AbsTimeTo(&AbsTime, &time);
			m_TxdInfo.pBuf[j] = 0;
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n上电时间:%2d年%2d月%2d日 %2d时%2d分%2d秒%3d毫秒.\r\n",
				time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second, time.MSecond);
			j += strlen((char *)&m_TxdInfo.pBuf[j]);
*/			

			WriteComm(j,0,0);
			break;

#else
		case CMD_TYPE_UPDATEDATA:
		case CMD_TYPE_UPDATECF:
			//将保护数据文件内容从FLASH文件导入EEPROM
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
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showbuf [netaX/netbX/serX/canX]  ---->显示端口规约报文\r\n");
			else if(bCurType == 1)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showerr [-a]  ----------------------->显示运行信息\r\n");
			else if(bCurType == 2)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showsam  ---------------------------->显示采样数据\r\n");
			else if(bCurType == 3)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrec [-a]  ----------------------->显示操作记录\r\n");
			else if(bCurType == 4)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showyc  ----------------------------->显示遥测数据\r\n");
			else if(bCurType == 5)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrm  ----------------------------->显示保护测量值\r\n");
			else if(bCurType == 6)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showyx  ----------------------------->显示遥信数据\r\n");
			else if(bCurType == 7)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showdd  ----------------------------->显示电度数据\r\n");
			else if(bCurType == 8)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showbie [-a]  ----------------------->显示变位遥信记录\r\n");
			else if(bCurType == 9)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showsoe [-a]  ----------------------->显示SOE记录\r\n");
			else if(bCurType == 10)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrep [-a]  ----------------------->显示保护动作报告\r\n");
			else if(bCurType == 11)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n setbo X       ----------------------->遥控操作:预置/执行/直接执行/撤销\r\n");
			else if(bCurType == 12)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n dist dir  --------------------------->显示录波信息\r\n");
			else if(bCurType == 13)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showrun   --------------------------->显示运行状态信息\r\n");
			else if(bCurType == 14)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showini   --------------------------->显示初始化状态信息\r\n");
			else if(bCurType == 15)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n settime yy/mm/dd hh:mm:ss  ---------->对时操作\r\n");
			else if(bCurType == 16)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n readtime    ------------------------->读系统时间");
			else if(bCurType == 17)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showvar [-mem/-eeprom] [pos]  ------->显示NvRam或者EEPROM\r\n");
			else if(bCurType == 18)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showvar [-ser[n]/-eth[n]/-can[n]] [-t/-r]	-->显示端口中断缓冲区\r\n");
			else if(bCurType == 19)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showport  --------------------------->显示系统端口配置\r\n");
			else if(bCurType == 20)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showstat  --------------------------->显示系统状态\r\n");
			else if(bCurType == 21)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetdata  -------------------------->恢复保护出厂配置\r\n");
			else if(bCurType == 22)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n reboot [-c]  ------------------------>重启设备\r\n");
			else if(bCurType == 23)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n clearevent--------------------------->清除装置事件\r\n");
			else if(bCurType == 24)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetpara---------------------------->恢复EEPROM参数\r\n");
			else if(bCurType == 25)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showmap------------------------------>显示镜像位参数\r\n");
			else if(bCurType == 26)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showraw------------------------------>显示保护测量值原始数据\r\n");
			else if(bCurType == 27)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n makerep [-alm] [no]------------------>模拟序号为no的保护或者告警事件\r\n");
#if(SYSTEM_TYPE == _SYS_KF6500_)
			else if(bCurType == 28)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetcoef  -------------------------->初始化系数出厂值\r\n");
			else if(bCurType == 29)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showsysval  ------------------------->读系统数据\r\n");
			else if(bCurType == 30)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n key U/D/L/R/A/M/E/N  ---------------->键盘控制操作\r\n");
			else if(bCurType == 31)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n lcdinf  ----------------------------->液晶背光参数信息\r\n");
			else if(bCurType == 32)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n showfl  ----------------------------->显示测距信息\r\n");
#else
			else if(bCurType == 28)
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n resetchan	-------------------------->初始化通道参数默认值\r\n");
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

				//数据位
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
				//校验位
				switch((tSerial.bFlag>>2)&0x03)
				{
					case 0:
						strcat((char *)&m_TxdInfo.pBuf[j],"N  ");break;
					case 1:
						strcat((char *)&m_TxdInfo.pBuf[j],"O  ");break;
					case 2:
						strcat((char *)&m_TxdInfo.pBuf[j],"D  ");break;
				}

				//停止位
				if((tSerial.bFlag>>4)&0x01)
					strcat((char *)&m_TxdInfo.pBuf[j],"2  ");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"1  ");
					

				//规约名称
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
				
				//比特率,数据掩码
				sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n CAN%d: 0x%x   %d  0x%x    ",MINOR_PORT(tCanbus.wPortID),tCanbus.wPortID,tCanbus.wBaud,tCanbus.dwMask);

				//规约名称
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

				// TCP、UDP
				if(tLink.dwMode & ETHLINK_MODE_UDP)
					strcat((char *)&m_TxdInfo.pBuf[j],"UDP  ");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"TCP	");
				 
				// 客户端还是服务器
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
				
				//规约名称
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

			// 半秒钟一个
			if((m_dwTimerCnt%(200/SHELL_BASIC_TIMER))!=0)
				break;
			
			m_TxdInfo.pBuf[j] = 0;

			// 打印表头
			if(m_wShowCnt==0)
				strcat((char *)m_TxdInfo.pBuf,"No   PORT  SAddr DAddr SideIP     Baud   SMBs  SMBn  RMBs  RMBn    COMM\r\n");
			
			sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"\r\n%2d:  ",m_wShowCnt+1);

			DB_GetMapBitObjDisp(m_wShowCnt,&tMapObj);
			// 连接模式
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

			// 其他信息
			NumToBaud(tMapObj.dwBaud,&dwBaud);
			sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],\
				"%5d %5d 0x%8x %6d %5d %5d %5d %5d   ",\
				tMapObj.bySelfAddr,tMapObj.bySideAddr,tMapObj.dwSideIP,dwBaud,\
				tMapObj.bySMBStart,tMapObj.bySMBNum,tMapObj.byRMBStart,tMapObj.byRMBNum);

			// 通信状态
			if(tMapObj.wControl&0x8000)
			{
				if(DAE_GetRunningFlags(SS_MAPBIT_COMM_ERR+m_wShowCnt))
					strcat((char *)&m_TxdInfo.pBuf[j],"BREAK");
				else
					strcat((char *)&m_TxdInfo.pBuf[j],"OK");
			}
			else//未投入
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

			// 打印标题
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

			// 逐行打印
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

			// 结束判断
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
			
			// 逐行打印			
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
			
			// 逐行打印			
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
			sprintf((char *)&m_TxdInfo.pBuf[j],"\r\n 装置温度:%.2f℃  液晶对比度参数:%d\r\n",value, g_wLCDContrast);
			WriteComm(strlen((char *)m_TxdInfo.pBuf),0,0);
			m_wCmdType = CMD_TYPE_NULL;
			break;
#endif			
		case CMD_TYPE_ACK:
			break;

		case CMD_TYPE_MAKEREP:
		case CMD_TYPE_MAKEALM:
			if(DB_GetREDisp(m_dwOffset, &tREDisp) != DB_ERR_OK)
				sprintf((char *)m_TxdInfo.pBuf,"\r\n模拟第%d号报告失败:(\r\n",m_dwOffset);
			else
			{
				if(m_wCmdType == CMD_TYPE_MAKEREP)
				{
					SimActionReport(m_dwOffset);
					sprintf((char *)m_TxdInfo.pBuf,"\r\n模拟第%d号动作报告成功! \r\n\r\n事项名称: %s \r\n事项编号: %d \r\n电量个数: %d\r\n",\
						m_dwOffset,tREDisp.strCHName,tREDisp.wInf,tREDisp.nFD);
				}
				else
				{
					Report_Alarm(m_dwOffset);
					sprintf((char *)m_TxdInfo.pBuf,"\r\n模拟第%d号告警报告成功! \r\n\r\n事项名称: %s \r\n事项编号: %d \r\n电量个数: %d\r\n",\
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
		
			// 半秒钟一个
			if((m_dwTimerCnt%(200/SHELL_BASIC_TIMER))!=0)
				break;
			
			m_TxdInfo.pBuf[j] = 0;
		
			// 打印表头
			if(m_wShowCnt==0)
			{
				DB_GetCFGStatus(SET_AREA_CURRENT, 10, bCfgStatus);
				if(bCfgStatus == CFG_STATUS_SET)
				{
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"\r\n设备类型: 主所\r\n"); 
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"子所个数: %d\r\n",DB_GetFLObjNum());
				}
				else
				{
					m_wShowMax = 1;
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"设备类型: 子所\r\n"); 
				}
				DB_GetSETFixValue(SET_AREA_CURRENT,44, &lSetvalue);
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"设备编号: %d\r\n",lSetvalue);
				
				if(DB_GetCurrentSETAreaNo() == 0)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"测距类型: 区内\r\n");
				else if(DB_GetCurrentSETAreaNo() == 1)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"测距类型: 本测越区\r\n");
				else if(DB_GetCurrentSETAreaNo() == 2)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"测距类型: 对测越区\r\n");
				else
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"测距类型: 未知\r\n");
									
				DB_GetSETFixValue(SET_AREA_CURRENT,43, &lSetvalue);
				if(lSetvalue == 1)
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"连接方式: 串口\r\n");
				else if(lSetvalue == 2)
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"连接方式: 透传\r\n");
				else if(lSetvalue == 0)
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"连接方式: 网络\r\n");
				else
					strcat((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"连接方式: 未知\r\n");
			}
			else
			{
				if(m_wShowCnt == 1)
					strcat((char *)m_TxdInfo.pBuf,"\r\nSubNo   TYPE    IPA		 IPB	   COMM\r\n");
		
				// 子所编号
				DB_GetFLObjDisp(m_wShowCnt-1,&tFLObj);
				
				if(DB_GetCurrentSETAreaNo() == 0)
				{
					// 区内剔除对侧
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
					// 跨区剔除无效
					if(!(tFLObj.wControl&FL_CTRLMASK_STEPOVERVAL))
					{
						m_wShowCnt++;
						if(m_wShowCnt == m_wShowMax)
							m_wCmdType = CMD_TYPE_NULL;
						return;
					}
				}
				
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"\r\n%2d      ",tFLObj.byFLNo);
		
				// 设备类型
				if(tFLObj.wControl&FL_CTRLMASK_SIDE)
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"对侧子所%d",tFLObj.byFLNo);
				else
					sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"本侧子所%d",tFLObj.byFLNo);
/*					
				switch(tFLObj.wControl&0x000F)
				{
					case FL_SUBST_SELFAT:  strcat((char *)&m_TxdInfo.pBuf[j],"本测AT	"); break;
					case FL_SUBST_SELFSP:  strcat((char *)&m_TxdInfo.pBuf[j],"本测SP	"); break;
					case FL_SUBST_SIDESP:  strcat((char *)&m_TxdInfo.pBuf[j],"对侧SP	"); break;
					case FL_SUBST_SIDEAT:  strcat((char *)&m_TxdInfo.pBuf[j],"对侧AT	"); break;
					case FL_SUBST_SIDEMAIN:  strcat((char *)&m_TxdInfo.pBuf[j],"对侧主所  "); break;
				}
*/		
				// 控制字
				//sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%4x	",tFLObj.wControl);
				
				// IPA
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%8x	",tFLObj.dwMNetIP);
		
				// IPB
				sprintf((char *)&m_TxdInfo.pBuf[j+strlen((char *)&m_TxdInfo.pBuf[j])],"%8x	",tFLObj.dwSNetIP);
		
				// 通信状态
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
	
	//接收缓冲指针修正
	if(m_CommandPtr >= MAX_FRAME_LEN)//写指针>缓冲区大小
	{
		m_CommandPtr = 0;
		memset(m_CommandBuffer,0,MAX_FRAME_LEN);
		return;
	}
	
	//上次命令保存
	len = pCmdEnd-m_CommandBuffer-1;
	if(len>64)
		len = 64;

	// 最小的指令
	if(len>1)
	{
		memcpy(m_LastCommand,m_CommandBuffer,len);
		m_LastCommand[len] = '\0';
	}
	
	//后面的数据将前面的数据覆盖
	for(i=pCmdEnd-m_CommandBuffer,j=0; i< m_CommandPtr; i++)
	{		
		m_CommandBuffer[j++] = m_CommandBuffer[i];
	}

	//更新命令指针
	m_CommandPtr = j;

	//剩余空间清0
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

