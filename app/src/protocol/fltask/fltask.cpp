/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			fltask.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ���ͨ��������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ���ͨ��������ֱ����·������ģʽ����̫�������ں�͸������ͨ����                          
*                                                                                  
* ����                                                                             
*                                                                                  
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      LZ               12/04/15    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "socklib.h"
#include "inetlib.h"
#include "iflib.h"
#include "parafunc.h"
#include "logic.h"
#include "relay.h"
#include "relaydef.h"

#if(MODUEL_TYPE == MT_KF6571)
#include "fltask.h"

extern subChannelCheck 		HeartbeatFLData[];
extern subChannelCheck 		ManualFLData[];
extern subChannelCheck 		RelayFLData[FLCOMPLICATENUM][MAX_SUBSTATION_NUM];
extern subChannelCheck 		ForeFLData[];
extern subChannelCheck 		LowValFLData[];

extern subChannelCheck			HeartbeatData;//����Ϊ����ʱ�������ٻ�����������
extern subChannelCheck			UploadData[];	  //����Ϊ����ʱ�������ٻ�������(���������Ĳ�࣬ʵ���������ʹ��)
extern subChannelCheck*			g_pHistoryData; //Ӧ����NvRam�У��ٲ���ʷ����ʹ��
extern subChannelCheck*			g_pLowValData;	 //Ӧ����NvRam��, �ٲ��ѹ��������ʹ��

// �ٲ����������־
extern FLComplicate		FLComplicateInfo[];

extern BOOL RelayTaskInitOK;

//BOOL g_bCallRelayFLDataCompleted;
BOOL g_bCallManualFLDataCompleted;
BOOL g_bCallHistoryFLDataCompleted;
BOOL g_bCallLowValueFLDataCompleted;

#define _FL_DEBUG_

BEGIN_MESSAGE_MAP(CFLTask,CApp)
	ON_SM_TIMEOUT(CFLTask)
	ON_SM_CALLSUBDATA(CFLTask)
	ON_SM_DATAISOK(CFLTask)
	ON_SM_STOPCALLDATA(CFLTask)
	ON_SM_STEPOVERCMD(CFLTask)
	ON_SM_RELAYDATA(CFLTask)
END_MESSAGE_MAP()

void FLTask(DWORD *pdwApp)
{
	CFLTask *pFLTask = new CFLTask(pdwApp);
	if(pFLTask->Init() != TRUE)
	{
		::OS_Exit(0); 
	}
	
	pFLTask->Run();
}	

CFLSubDev::CFLSubDev()
{
	ClearAllPollCmd();
}

BOOL CFLSubDev::Init(BYTE no,TFL_Disp *pFLDisp,CFLTask *pFLTask)
{	
	m_pFLTask = pFLTask;
		
	if(pFLDisp == NULL)
		return FALSE;
	
	// ���ݲ���
	m_FLDisp = *pFLDisp;

	// ��������Ч��վ
	if(pFLDisp->wControl&FL_CTRLMASK_STEPOVERVAL)
		m_bValid = TRUE;
	else
		m_bValid = FALSE;
	
	// �Ƿ�Զ�����
	if(pFLDisp->wControl&FL_CTRLMASK_SIDE)
		m_bSideDev = TRUE;
	else
		m_bSideDev = FALSE;

	// ��ʼ������ģʽ
	m_byLinkMode = (pFLDisp->wControl&0x7000)>>12;

	m_byNetAddr = m_FLDisp.byNetAddr;
	m_byFLNo = m_FLDisp.byFLNo;
	m_byParaNo = no;
	
	// ��ʼ������IP��ַ
	m_dwNetIP = m_FLDisp.dwMNetIP;

	m_TxdInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
	m_TxdInfo.wBufLen = 1024;
	m_TxdInfo.wReadPtr = 0;
	m_TxdInfo.wWritePtr= 0;

	// ��ʼ��������־
	m_bHeart = TRUE;
	m_nNoAckCnt = 0;
	m_nTimeoutCnt = 0;
	
	return TRUE;
}

void CFLSubDev::ClearAllPollCmd()
{
	m_bRelayFL = FALSE; 		//�������
	m_bManualFL = FALSE;		//�ֶ����
	m_bForeFL = FALSE;			//�ٲ���ʷ
	m_bLowValFL = FALSE;		//�ٲ��ѹ����
	m_bStepOverCmd = FALSE; 	//Խ��ָ��ִ��
	m_bStepOverRet = FALSE; 	//Խ��ָ���
	m_bHeart = FALSE;

	m_nTimeoutCnt = 0;
	m_bProcFlag = FALSE;
}

// ���������������
BOOL CFLSubDev::FillFLSendData(subChannelCheck	*pFLData)
{
	BYTE 	*pTxdData = m_TxdInfo.pBuf+sizeof(TFLFrameHead)-1;

	if(m_CallType == FLTASK_MODE_AUTOFL)
	{
		*pTxdData = pFLData->ComplicateNo;
		pTxdData++;
	}
	
	pTxdData[0] = LLBYTE(pFLData->GPSStatus);
	pTxdData[1] = LHBYTE(pFLData->GPSStatus);
	pTxdData[2] = HLBYTE(pFLData->GPSStatus);
	pTxdData[3] = HHBYTE(pFLData->GPSStatus);

	pTxdData[4] = LLBYTE(pFLData->SubClock.Lo);
	pTxdData[5] = LHBYTE(pFLData->SubClock.Lo);
	pTxdData[6] = HLBYTE(pFLData->SubClock.Lo);
	pTxdData[7] = HHBYTE(pFLData->SubClock.Lo);

	pTxdData[8] = LOBYTE(pFLData->SubClock.Hi);
	pTxdData[9] = HIBYTE(pFLData->SubClock.Hi);

	pTxdData[10] = LLBYTE(pFLData->U1);
	pTxdData[11] = LHBYTE(pFLData->U1);
	pTxdData[12] = HLBYTE(pFLData->U1);
	pTxdData[13] = HHBYTE(pFLData->U1);
	
	pTxdData[14]  = LLBYTE(pFLData->U2);
	pTxdData[15]  = LHBYTE(pFLData->U2);
	pTxdData[16] = HLBYTE(pFLData->U2);
	pTxdData[17] = HHBYTE(pFLData->U2);
	
	pTxdData[18] = LLBYTE(pFLData->U3);
	pTxdData[19] = LHBYTE(pFLData->U3);
	pTxdData[20] = HLBYTE(pFLData->U3);
	pTxdData[21] = HHBYTE(pFLData->U3);
	
	pTxdData[22] = LLBYTE(pFLData->U4);
	pTxdData[23] = LHBYTE(pFLData->U4);
	pTxdData[24] = HLBYTE(pFLData->U4);
	pTxdData[25] = HHBYTE(pFLData->U4);
	
	pTxdData[26] = LLBYTE(pFLData->It1);
	pTxdData[27] = LHBYTE(pFLData->It1);
	pTxdData[28] = HLBYTE(pFLData->It1);
	pTxdData[29] = HHBYTE(pFLData->It1);
	
	pTxdData[30] = LLBYTE(pFLData->If1);
	pTxdData[31] = LHBYTE(pFLData->If1);
	pTxdData[32] = HLBYTE(pFLData->If1);
	pTxdData[33] = HHBYTE(pFLData->If1);
	
	pTxdData[34] = LLBYTE(pFLData->It2);
	pTxdData[35] = LHBYTE(pFLData->It2);
	pTxdData[36] = HLBYTE(pFLData->It2);
	pTxdData[37] = HHBYTE(pFLData->It2);
	
	pTxdData[38] = LLBYTE(pFLData->If2);
	pTxdData[39] = LHBYTE(pFLData->If2);
	pTxdData[40] = HLBYTE(pFLData->If2);
	pTxdData[41] = HHBYTE(pFLData->If2);
	
	pTxdData[42] = LLBYTE(pFLData->It3);
	pTxdData[43] = LHBYTE(pFLData->It3);
	pTxdData[44] = HLBYTE(pFLData->It3);
	pTxdData[45] = HHBYTE(pFLData->It3);
	
	pTxdData[46] = LLBYTE(pFLData->If3);
	pTxdData[47] = LHBYTE(pFLData->If3);
	pTxdData[48] = HLBYTE(pFLData->If3);
	pTxdData[49] = HHBYTE(pFLData->If3);
	
	pTxdData[50] = LLBYTE(pFLData->Iat1);
	pTxdData[51] = LHBYTE(pFLData->Iat1);
	pTxdData[52] = HLBYTE(pFLData->Iat1);
	pTxdData[53] = HHBYTE(pFLData->Iat1);
	
	pTxdData[54] = LLBYTE(pFLData->Iat2);
	pTxdData[55] = LHBYTE(pFLData->Iat2);
	pTxdData[56] = HLBYTE(pFLData->Iat2);
	pTxdData[57] = HHBYTE(pFLData->Iat2);
	
	pTxdData[58] = LLBYTE(pFLData->Iat);
	pTxdData[59] = LHBYTE(pFLData->Iat);
	pTxdData[60] = HLBYTE(pFLData->Iat);
	pTxdData[61] = HHBYTE(pFLData->Iat);

	pTxdData[62] = LLBYTE(pFLData->AIt1);
	pTxdData[63] = LHBYTE(pFLData->AIt1);
	pTxdData[64] = HLBYTE(pFLData->AIt1);
	pTxdData[65] = HHBYTE(pFLData->AIt1);

	
	pTxdData[66] = LLBYTE(pFLData->AIf1);
	pTxdData[67] = LHBYTE(pFLData->AIf1);
	pTxdData[68] = HLBYTE(pFLData->AIf1);
	pTxdData[69] = HHBYTE(pFLData->AIf1);


	pTxdData[70] = LLBYTE(pFLData->AIt2);
	pTxdData[71] = LHBYTE(pFLData->AIt2);
	pTxdData[72] = HLBYTE(pFLData->AIt2);
	pTxdData[73] = HHBYTE(pFLData->AIt2);


	pTxdData[74] = LLBYTE(pFLData->AIf2);
	pTxdData[75] = LHBYTE(pFLData->AIf2);
	pTxdData[76] = HLBYTE(pFLData->AIf2);
	pTxdData[77] = HHBYTE(pFLData->AIf2);

	pTxdData[78] = LLBYTE(pFLData->AIt3);
	pTxdData[79] = LHBYTE(pFLData->AIt3);
	pTxdData[80] = HLBYTE(pFLData->AIt3);
	pTxdData[81] = HHBYTE(pFLData->AIt3);

	
	pTxdData[82] = HHBYTE(pFLData->AIf3);
	pTxdData[83] = HHBYTE(pFLData->AIf3);
	pTxdData[84] = HHBYTE(pFLData->AIf3);
	pTxdData[85] = HHBYTE(pFLData->AIf3);
	
	pTxdData[86] = HHBYTE(pFLData->Ip1);		//2014-10-23 16:24:21��������������IP���ϴ���yanzh
	pTxdData[87] = HHBYTE(pFLData->Ip1);
	pTxdData[88] = HHBYTE(pFLData->Ip1);
	pTxdData[89] = HHBYTE(pFLData->Ip1);

	pTxdData[90] = HHBYTE(pFLData->Ip2);
	pTxdData[91] = HHBYTE(pFLData->Ip2);
	pTxdData[92] = HHBYTE(pFLData->Ip2);
	pTxdData[93] = HHBYTE(pFLData->Ip2);

	pTxdData[94] = HHBYTE(pFLData->Ip3);
	pTxdData[95] = HHBYTE(pFLData->Ip3);
	pTxdData[96] = HHBYTE(pFLData->Ip3);
	pTxdData[97] = HHBYTE(pFLData->Ip3);

	return TRUE;
}

// �л�ͨ������
void CFLSubDev::SwitchNetComm()
{
	// ������������л�
	if(!m_pFLTask->IsMainStation())
		return;
		
	// ����Ӧ��
	if(m_bHaveAck)
	{
		m_nNoAckCnt = 0;
		return;
	}

	// ÿ��γ�ʱ�л�����
	m_nNoAckCnt++;
	if(m_nNoAckCnt < 5)
		return;
	
	m_nNoAckCnt = 0;
	// ��Ϊ����
	if(m_FLDisp.dwMNetIP == m_dwNetIP)
	{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: SwitchNetComm-->0x%x to 0x%x",m_dwNetIP,m_FLDisp.dwSNetIP);
#endif
		m_dwNetIP = m_FLDisp.dwSNetIP;
	}
	else
	{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: SwitchNetComm-->0x%x to 0x%x",m_dwNetIP,m_FLDisp.dwMNetIP);
#endif
		m_dwNetIP = m_FLDisp.dwMNetIP;
	}
}

// ����������������
BOOL CFLSubDev::SaveFLrecvData(subChannelCheck	*pFLData)
{
	BYTE 	*pRxdData = (BYTE *)m_pRxdFrame+sizeof(TFLFrameHead)-1;

	if(m_CallType == FLTASK_MODE_AUTOFL)
	{
		pFLData->ComplicateNo = pRxdData[0];
		pRxdData++;
	}

	pFLData->GPSStatus = MAKEDWORD(pRxdData[0],pRxdData[1],pRxdData[2],pRxdData[3]);
	pFLData->SubClock.Lo = MAKEDWORD(pRxdData[4],pRxdData[5],pRxdData[6],pRxdData[7]);
	pFLData->SubClock.Hi = MAKEWORD(pRxdData[8],pRxdData[9]);

	pFLData->U1 = MAKEDWORD(pRxdData[10],pRxdData[11],pRxdData[12],pRxdData[13]);
	pFLData->U2 = MAKEDWORD(pRxdData[14],pRxdData[15],pRxdData[16],pRxdData[17]);
	pFLData->U3 = MAKEDWORD(pRxdData[18],pRxdData[19],pRxdData[20],pRxdData[21]);
	pFLData->U4 = MAKEDWORD(pRxdData[22],pRxdData[23],pRxdData[24],pRxdData[25]);
	pFLData->It1 = MAKEDWORD(pRxdData[26],pRxdData[27],pRxdData[28],pRxdData[29]);
	pFLData->If1 = MAKEDWORD(pRxdData[30],pRxdData[31],pRxdData[32],pRxdData[33]);
	pFLData->It2 = MAKEDWORD(pRxdData[34],pRxdData[35],pRxdData[36],pRxdData[37]);
	pFLData->If2 = MAKEDWORD(pRxdData[38],pRxdData[39],pRxdData[40],pRxdData[41]);
	pFLData->It3 = MAKEDWORD(pRxdData[42],pRxdData[43],pRxdData[44],pRxdData[45]);
	pFLData->If3 = MAKEDWORD(pRxdData[46],pRxdData[47],pRxdData[48],pRxdData[49]);
	pFLData->Iat1 = MAKEDWORD(pRxdData[50],pRxdData[51],pRxdData[52],pRxdData[53]);
	pFLData->Iat2 = MAKEDWORD(pRxdData[54],pRxdData[55],pRxdData[56],pRxdData[57]);
	pFLData->Iat = MAKEDWORD(pRxdData[58],pRxdData[59],pRxdData[60],pRxdData[61]);

	pFLData->AIt1= MAKEDWORD(pRxdData[62],pRxdData[63],pRxdData[64],pRxdData[65]);
	pFLData->AIf1= MAKEDWORD(pRxdData[66],pRxdData[67],pRxdData[68],pRxdData[69]);
	pFLData->AIt2 = MAKEDWORD(pRxdData[70],pRxdData[71],pRxdData[72],pRxdData[73]);
	pFLData->AIf2 = MAKEDWORD(pRxdData[74],pRxdData[75],pRxdData[76],pRxdData[77]);
	pFLData->AIt3 = MAKEDWORD(pRxdData[78],pRxdData[79],pRxdData[80],pRxdData[81]);
	pFLData->AIf3 = MAKEDWORD(pRxdData[82],pRxdData[83],pRxdData[84],pRxdData[85]);

	
	pFLData->Ip1= MAKEDWORD(pRxdData[86],pRxdData[87],pRxdData[88],pRxdData[89]); //2014-10-23 16:23:49 ��������IP�������ϴ�   yanzh
	pFLData->Ip2 = MAKEDWORD(pRxdData[90],pRxdData[91],pRxdData[92],pRxdData[93]);
	pFLData->Ip3 = MAKEDWORD(pRxdData[94],pRxdData[95],pRxdData[96],pRxdData[97]);

	return TRUE;
}

BOOL CFLSubDev::SendData()
{
	switch(m_pFLTask->GetLinkMode())
	{
		case FLTASK_LINK_ETHERNET:
			// �л�����
			SwitchNetComm();
			
			// ����ͨ��ͨ��SOCKET����
			if(!m_pFLTask->SendUDPData(m_dwNetIP,(BYTE *)m_TxdInfo.pBuf,m_TxdInfo.wWritePtr))
			{
//				LogError("CFLTask::SendData",FILE_LINE,"send error.errno=%d",errno);
			}
			break;
			
		case FLTASK_LINK_SERIAL:
			// ����ͨ��ֱ�ӷ���������
			::WriteComm(m_pFLTask->m_SerPortID,(BYTE *)m_TxdInfo.pBuf,m_TxdInfo.wWritePtr,0);
			// ÿ�δ��ڷ��ͽ�����ս��ջ�����
			m_pFLTask->ClearRecvFrame();
			break;
			
		case FLTASK_LINK_TRANS:
			// ·��ͨ��������͸�����ݻ�����
			DAE_WriteTransData(0x0300, m_pFLTask->m_TransPortID, m_TxdInfo.wWritePtr, m_TxdInfo.pBuf);
			break;
			
		default:
			break;
	}
	
	m_bHaveAck = FALSE;
	m_nTimeoutCnt = FLTASK_TIMEOUT;

	return TRUE;
}

BOOL CFLSubDev::TxdFrame(BYTE FuncCode,WORD wDataLen)
{
	TFLFrameHead *pFrame = (TFLFrameHead *)m_TxdInfo.pBuf;
	
	pFrame->START = 0x68;
	pFrame->DevNo = m_byFLNo;
	pFrame->Func = FuncCode;
	pFrame->DataLenLO = LOBYTE(wDataLen);
	pFrame->DataLenHI = HIBYTE(wDataLen);

	m_TxdInfo.pBuf[wDataLen+5] = LPC(0x55,m_TxdInfo.pBuf,wDataLen+5);
	m_TxdInfo.wWritePtr = wDataLen+6;

	return SendData();
}

// ����Խ���л�����
BOOL CFLSubDev::TxdStepOverCmd()
{
	TFLFrameHead *pTxHead = (TFLFrameHead *)m_TxdInfo.pBuf;

#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: TxdStepOverCmd-->%d-%d",m_byFLNo,m_byFLDataIndex);
#endif

	if(IsSideDev())
		pTxHead->DATA = 1;	//�Զ��豸
	else
		pTxHead->DATA = 0;	//�����豸

	m_CallType = FLTASK_MODE_SOSWITCH;
		
	return TxdFrame(FLTASK_FUNC_STEPOVERCMDREQ,1);
}

// ����Խ����������
BOOL CFLSubDev::TxdStepOverRet()
{
	TFLFrameHead *pTxHead = (TFLFrameHead *)m_TxdInfo.pBuf;

#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: TxdStepOverRet-->%d-%d",m_byFLNo,m_byFLDataIndex);
#endif

	if(IsSideDev())
		pTxHead->DATA = 1;	//�Զ�����
	else
		pTxHead->DATA = 0;	//��������

	m_CallType = FLTASK_MODE_SORETURN;
		
	return TxdFrame(FLTASK_FUNC_STEPOVERRETREQ,1);
}

// ���ͱ����������
BOOL CFLSubDev::TxdRelayFLCmd()
{
	BYTE 	*pTxdData = m_TxdInfo.pBuf+sizeof(TFLFrameHead)-1;
	TAbsTime time;
	
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: TxdRelayFLCmd(%d)-->%d-%d",m_pFLTask->GetRelayFLSeq(),m_byFLNo,m_byFLDataIndex);
#endif
	m_pFLTask->GetRelayFLTime(&time);
		
	pTxdData[0] = LLBYTE(time.Lo);
	pTxdData[1] = LHBYTE(time.Lo);
	pTxdData[2] = HLBYTE(time.Lo);
	pTxdData[3] = HHBYTE(time.Lo);
	
	pTxdData[4] = LOBYTE(time.Hi);
	pTxdData[5] = HIBYTE(time.Hi);

	pTxdData[6] = m_pFLTask->GetRelayFLSeq();

	m_CallType = FLTASK_MODE_AUTOFL;

	return TxdFrame(FLTASK_FUNC_RELAYREQ,7);
}

// �����ֶ��������
BOOL CFLSubDev::TxdManualFLCmd()
{
	BYTE *pTxdData = m_TxdInfo.pBuf+sizeof(TFLFrameHead)-1;

#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: TxdManualFLCmd-->%d-%d",m_byFLNo,m_byFLDataIndex);
#endif
		
	pTxdData[0] = LLBYTE(m_pFLTask->m_tFLTime.Lo);
	pTxdData[1] = LHBYTE(m_pFLTask->m_tFLTime.Lo);
	pTxdData[2] = HLBYTE(m_pFLTask->m_tFLTime.Lo);
	pTxdData[3] = HHBYTE(m_pFLTask->m_tFLTime.Lo);
	
	pTxdData[4] = LOBYTE(m_pFLTask->m_tFLTime.Hi);
	pTxdData[5] = HIBYTE(m_pFLTask->m_tFLTime.Hi);
	
	m_CallType = FLTASK_MODE_MANUALFL;
	
	return TxdFrame(FLTASK_FUNC_MANUALREQ,6);
}

// �ٲ���ʷ�������
BOOL CFLSubDev::TxdHistoryFLCmd()
{
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: TxdHistoryFLCmd-->%d-%d",m_byFLNo,m_byFLDataIndex);
#endif
			
	m_CallType = FLTASK_MODE_HISTORYFL;

	return TxdFrame(FLTASK_FUNC_HISTREQ,0);
}

// �ٲ��ѹ������������
BOOL CFLSubDev::TxdLowValFLCmd()
{
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: TxdLowValFLCmd-->%d-%d",m_byFLNo,m_byFLDataIndex);
#endif

	m_CallType = FLTASK_MODE_LOWVOLFL;
				
	return TxdFrame(FLTASK_FUNC_LOWVALREQ,0);
}

// �ٲ��ѹ������������
BOOL CFLSubDev::TxdHeartFLCmd()
{
	subChannelCheck	*pFLData;

#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: TxdHeartFLCmd-->%d-%d",m_byFLNo,m_byFLDataIndex);
#endif
					
	// ���δӦ��������ձ�־	
	if( !m_bHaveAck )
	{
		// ��ȡ����������
		pFLData = &HeartbeatFLData[m_byFLDataIndex];
		pFLData->subRxdFlag = FALSE;
		DAE_SetRunningFlags(SS_FL1_COMM_ERR+m_byParaNo);
	}
	else
		DAE_ClearRunningFlags(SS_FL1_COMM_ERR+m_byParaNo);

	// ���Ӧ���־
	m_bHeart = FALSE;

	m_CallType = FLTASK_MODE_HEARTBEAT;
	
	return TxdFrame(FLTASK_FUNC_HEARTREQ,0);
}

// ���շ���Խ���л���������
BOOL CFLSubDev::RxdStepOverCmdReq()
{
	BYTE 	*pTxdData = m_TxdInfo.pBuf+sizeof(TFLFrameHead)-1;
	BOOL	bValid = FALSE;
	STATUS 	rc;
	
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: RxdStepOverCmdReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_SOSWITCH;

	// 1���л���ֵ��Ҫ���ֱ��⻹�ǶԲ�
	if(m_pRxdFrame->DATA == 0)
	{
		if(m_pFLTask->IsMainStation())
		{
			LogError("RxdStepOverCmdReq",FILE_LINE,"Main staiton recv self step over command!!!");
			return FALSE;
		}
			
		rc = DB_SetCurrentSETAreaNo(FLTASK_SETAREA_STEPOVER_A);
		rc |= DB_SetCurrentSETAreaNoConfirm(FLTASK_SETAREA_STEPOVER_A);
	}
	else
	{		
		rc = DB_SetCurrentSETAreaNo(FLTASK_SETAREA_STEPOVER_B);
		rc |= DB_SetCurrentSETAreaNoConfirm(FLTASK_SETAREA_STEPOVER_B);
	}
	
	if(rc != DB_ERR_OK)
	{
		LogError("RxdStepOverCmdReq",FILE_LINE,"Set SET error!");
		return FALSE;
	}
			
	//�Ƿ�ֹͣ��๦��	
	if(*CFG_pbyAT_C_RATE == CFG_STATUS_SET)
		bValid = TRUE;
	if(*CFG_pbyUD_C_RATE == CFG_STATUS_SET)
		bValid = TRUE;
	if(*CFG_pbyNL_C_RATE == CFG_STATUS_SET)
		bValid = TRUE;
	if(*AS_pnPM == AT2MODE)
		bValid = TRUE;

	pTxdData[0] = m_pRxdFrame->DATA;

	// 2�����ͷ�����Ϣ
	if(bValid)
		pTxdData[1] = 0xAA;
	else
		pTxdData[1] = 0x55;

	// ��ȡ��վ��Ŷ�ֵ
	pTxdData[2] = *RS_pnNID;
	
	TxdFrame(FLTASK_FUNC_STEPOVERCMDRES,3);
	
	m_pFLTask->InitFLPara();
	
	return TRUE;
}

// ���շ���Խ��������������
BOOL CFLSubDev::RxdStepOverRetReq()
{
	BYTE 	*pTxdData = m_TxdInfo.pBuf+sizeof(TFLFrameHead)-1;
	BOOL	bValid = FALSE;
	STATUS 	rc;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdStepOverRetReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_SORETURN;

	if(!m_pFLTask->IsStepOver())
	{
		LogError("RxdStepOverRetReq",FILE_LINE,"Normal dev recv step over return command!!!");
		return FALSE;
	}

	if(m_pRxdFrame->DATA == 0)
	if(m_pFLTask->IsMainStation())
	{
		LogError("RxdStepOverRetReq",FILE_LINE,"Main staiton recv self step over command!!!");
		return FALSE;
	}

	// 1���л���ֵ
	rc = DB_SetCurrentSETAreaNo(FLTASK_SETAREA_NORMAL);
	rc |= DB_SetCurrentSETAreaNoConfirm(FLTASK_SETAREA_NORMAL);
	if(rc != DB_ERR_OK)
	{
		LogError("RxdStepOverRetReq",FILE_LINE,"Set SET error!");
		return FALSE;
	}
	
	//�Ƿ�ֹͣ��๦��	
	if(*CFG_pbyAT_C_RATE == CFG_STATUS_SET)
		bValid = TRUE;
	if(*CFG_pbyUD_C_RATE == CFG_STATUS_SET)
		bValid = TRUE;
	if(*CFG_pbyNL_C_RATE == CFG_STATUS_SET)
		bValid = TRUE;
	if(*AS_pnPM == AT2MODE)
		bValid = TRUE;

	pTxdData[0] = m_pRxdFrame->DATA;

	// 2�����ͷ�����Ϣ
	if(bValid)
		pTxdData[1] = 0xAA;
	else
		pTxdData[1] = 0x55;
	
	// ��ȡ��վ��Ŷ�ֵ
	pTxdData[2] = *RS_pnNID;

	// �ٷ��ͷ�����Ϣ
	TxdFrame(FLTASK_FUNC_STEPOVERRETRES,3);
	
	m_pFLTask->InitFLPara();

	return TRUE;
}

// ���շ��ͱ��������������ֻ�����������
BOOL CFLSubDev::RxdRelayFLCmdReq()
{
	//����֡
	BYTE *pRxdData = m_pFLTask->GetRecvFrame()+sizeof(TFLFrameHead)-1;
	DWORD TimeLo;
	WORD TimeHi;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdRelayFLCmdReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_AUTOFL;

	if(m_pFLTask->IsMainStation())
	{
		LogError("RxdRelayFLCmdReq",FILE_LINE,"Main staiton recv call Realy FL data command!!!");
		return FALSE;
	}

	TimeLo = MAKEDWORD(pRxdData[0],pRxdData[1],pRxdData[2],pRxdData[3]);
	TimeHi = MAKEWORD(pRxdData[4],pRxdData[5]);

	m_CallType = FLTASK_MODE_AUTOFL;
	
	// ������������Ϣ׼�����ݣ���Ϣ��Ӧ�з���
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_GETFLDATA, TimeHi, TimeLo, (pRxdData[6]<<8)+FLTASK_MODE_AUTOFL,0);

	return TRUE;
}

// ���շ����ֶ������������
BOOL CFLSubDev::RxdManualFLCmdReq()
{
	//����֡
	BYTE *pRxdData = m_pFLTask->GetRecvFrame()+sizeof(TFLFrameHead)-1;
	DWORD TimeLo;
	WORD TimeHi;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdManualFLCmdReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_MANUALFL;
							
	if(m_pFLTask->IsMainStation())
	{
		LogError("RxdManualFLCmdReq",FILE_LINE,"Main staiton recv call Manual FL data command!!!");
		return FALSE;
	}

	TimeLo = MAKEDWORD(pRxdData[0],pRxdData[1],pRxdData[2],pRxdData[3]);
	TimeHi = MAKEWORD(pRxdData[4],pRxdData[5]);
	
	m_CallType = FLTASK_MODE_MANUALFL;

	// ������������Ϣ׼�����ݣ���Ϣ��Ӧ�з���
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_GETFLDATA, TimeHi, TimeLo, (pRxdData[6]<<8)+FLTASK_MODE_MANUALFL,0);
	
	return TRUE;
}

// �����ٲ���ʷ�����������
BOOL CFLSubDev::RxdHistoryFLCmdReq()
{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdHistoryFLCmdReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_HISTORYFL;
							
	if(m_pFLTask->IsMainStation())
	{
		LogError("RxdHistoryFLCmdReq",FILE_LINE,"Main staiton recv call History FL data command!!!");
		return FALSE;
	}

	FillFLSendData(g_pHistoryData);
	
	return TxdFrame(FLTASK_FUNC_HISTRES,86);
}

// �����ٲ��ѹ������������
BOOL CFLSubDev::RxdLowValFLCmdReq()
{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdLowValFLCmdReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_LOWVOLFL;
							
	if(m_pFLTask->IsMainStation())
	{
		LogError("RxdLowValFLCmdReq",FILE_LINE,"Main staiton recv call LowValSave FL data command!!!");
		return FALSE;
	}

	// ֱ����ȡ���ݷ���
	FillFLSendData(g_pLowValData);
	return TxdFrame(FLTASK_FUNC_LOWVALRES,86);
}

// �����ٲ�������������
BOOL CFLSubDev::RxdHeartBeatCmdReq()
{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdHeartBeatCmdReq <--%d",m_pRxdFrame->DevNo);
#endif

	m_CallType = FLTASK_MODE_HEARTBEAT;
							
	if(m_pFLTask->IsMainStation())
	{
		LogError("RxdHeartBeatCmdReq",FILE_LINE,"Main staiton recv call HeartBeat FL data command!!!");
		return FALSE;
	}

	// ֱ����ȡ���ݷ���
	FillFLSendData(&HeartbeatData);
	return TxdFrame(FLTASK_FUNC_HEARTRES,86);
}


// ���շ���Խ���л���������
BOOL CFLSubDev::RxdStepOverCmdRes()
{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdStepOverCmdRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	// ������ʱ������
	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdStepOverCmdRes",FILE_LINE,"Sub staiton recv error command!!!");
		return FALSE;
	}
	
	// 1��������豸��ѯ��־
	m_bStepOverCmd = FALSE;
	
	// 2�����ݷ��ؽ��Ͷ����ѯ�豸����Ҫ����
	//if(m_pRxdFrame->DATA == 0x55)
	//	Disenable();

	//if(m_pRxdFrame->DATA == 0xAA)
	//	Enable();

	return TRUE;
}

// ���շ���Խ��������������
BOOL CFLSubDev::RxdStepOverRetRes()
{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdStepOverRetRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	// ������ʱ������
	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdStepOverRetRes",FILE_LINE,"Sub staiton recv error command!!!");
		return FALSE;
	}
	
	// 1��������豸��ѯ��־
	m_bStepOverRet = FALSE;
	
	// 2�����ݷ��ؽ��Ͷ����ѯ�豸����Ҫ����
	//if(m_pRxdFrame->DATA == 0x55)
	//	Disenable();

	//if(m_pRxdFrame->DATA == 0xAA)
	//	Enable();

	return TRUE;
}

// ���շ��ͱ��������������ֻ�����������
BOOL CFLSubDev::RxdRelayFLCmdRes()
{
	subChannelCheck	*pFLData;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdRelayFLCmdRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdRelayFLCmdRes",FILE_LINE,"Sub staiton recv error data response!!!");
		return FALSE;
	}

	if(m_pRxdFrame->DATA != m_pFLTask->GetRelayFLSeq())
	{
		LogError("RxdRelayFLCmdRes",FILE_LINE,"Main staiton recv error relay FL SEQ!!!");
		return FALSE;
	}
		
	// 1��������豸��ѯ��־
	m_bRelayFL = FALSE;
	
	// 2����ȡ����������
	pFLData = &RelayFLData[m_pFLTask->GetRelayFLSeq()][m_byFLDataIndex];

	// 3��д����������
	SaveFLrecvData(pFLData);

	// 4�� ˢ��ͨ�ųɹ���־
	pFLData->subRxdFlag = TRUE;
	
	return TRUE;
}

// ���շ����ֶ������������
BOOL CFLSubDev::RxdManualFLCmdRes()
{
	subChannelCheck	*pFLData;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdManualFLCmdRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdManualFLCmdRes",FILE_LINE,"Sub staiton recv error data response!!!");
		return FALSE;
	}
							
	// 1��������豸��ѯ��־
	m_bManualFL = FALSE;
	
	// 2����ȡ����������
	pFLData = &ManualFLData[m_byFLDataIndex];

	// 3��д����������
	SaveFLrecvData(pFLData);

	// 4��ˢ��ͨ�ųɹ���־
	pFLData->subRxdFlag = TRUE;
	
	return TRUE;
}

// �����ٲ���ʷ�����������
BOOL CFLSubDev::RxdHistoryFLCmdRes()
{
	subChannelCheck	*pFLData;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdHistoryFLCmdRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdHistoryFLCmdRes",FILE_LINE,"Sub staiton recv error data response!!!");
		return FALSE;
	}
							
	// 1��������豸��ѯ��־
	m_bForeFL = FALSE;
	
	// 2����ȡ����������
	pFLData = &ForeFLData[m_byFLDataIndex];

	// 3��д����������
	SaveFLrecvData(pFLData);

	// 4��ˢ��ͨ�ųɹ���־
	pFLData->subRxdFlag = TRUE;
	
	return TRUE;
}

// �����ٲ��ѹ������������
BOOL CFLSubDev::RxdLowValFLCmdRes()
{
	subChannelCheck	*pFLData;

#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdLowValFLCmdRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdLowValFLCmdRes",FILE_LINE,"Sub staiton recv error data response!!!");
		return FALSE;
	}
							
	// 1��������豸��ѯ��־
	m_bLowValFL = FALSE;
	
	// 2 ��ȡ����������
	pFLData = &LowValFLData[m_byFLDataIndex];

	// 3 д����������
	SaveFLrecvData(pFLData);

	// 4��ˢ��ͨ�ųɹ���־
	pFLData->subRxdFlag = TRUE;
	
	return TRUE;
}

// �����ٲ�������������
BOOL CFLSubDev::RxdHeartBeatCmdRes()
{
	subChannelCheck	*pFLData;
	
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: RxdHeartBeatCmdRes <--%d-%d",m_pRxdFrame->DevNo,m_byFLDataIndex);
#endif

	if(!m_pFLTask->IsMainStation())
	{
		LogError("RxdHeartBeatCmdRes",FILE_LINE,"Sub staiton recv error data response!!!");
		return FALSE;
	}
							
	// 1��������豸��ѯ��־
	m_bHeart = FALSE;
	
	// 2 ��ȡ����������
	pFLData = &HeartbeatFLData[m_byFLDataIndex];

	// 3 д����������
	SaveFLrecvData(pFLData);

	// 4��ˢ��ͨ�ųɹ���־
	pFLData->subRxdFlag = TRUE;
	
	return TRUE;
}

// �������ʹ���
BOOL CFLSubDev::TxdProcess()
{
	// ���ΰ������ȼ�����
	if(m_bStepOverCmd)
	{
		TxdStepOverCmd();
		return TRUE;
	}
	if(m_bStepOverRet)
	{
		TxdStepOverRet();
		return TRUE;
	}
	if(m_bRelayFL)
	{
		TxdRelayFLCmd();
		return TRUE;
	}
	if(m_bManualFL)
	{
		TxdManualFLCmd();
		return TRUE;
	}
	if(m_bForeFL)
	{
		TxdHistoryFLCmd();
		return TRUE;
	}
	if(m_bLowValFL)
	{
		TxdLowValFLCmd();
		return TRUE;
	}
	if(m_bHeart)
	{
		TxdHeartFLCmd();
		return TRUE;
	}
	
	return FALSE;
}


// �������մ���
BOOL CFLSubDev::OnFrame()
{
	m_nTimeoutCnt = 0;
	m_bHaveAck = TRUE;

	// ��ʼ������֡ͷ
	m_pRxdFrame = (TFLFrameHead *)m_pFLTask->GetRecvFrame();
	
	//�������봦��
	switch(m_pRxdFrame->Func)
	{
		case FLTASK_FUNC_HEARTREQ: 			RxdHeartBeatCmdReq(); break;
		case FLTASK_FUNC_HEARTRES: 			RxdHeartBeatCmdRes(); break;
		case FLTASK_FUNC_RELAYREQ:			RxdRelayFLCmdReq(); break;
		case FLTASK_FUNC_RELAYRES:			RxdRelayFLCmdRes(); break;
		case FLTASK_FUNC_MANUALREQ:			RxdManualFLCmdReq(); break;
		case FLTASK_FUNC_MANUALRES:			RxdManualFLCmdRes(); break;
		case FLTASK_FUNC_HISTREQ:			RxdHistoryFLCmdReq(); break;
		case FLTASK_FUNC_HISTRES:			RxdHistoryFLCmdRes(); break;
		case FLTASK_FUNC_LOWVALREQ:			RxdLowValFLCmdReq(); break;
		case FLTASK_FUNC_LOWVALRES:			RxdLowValFLCmdRes(); break;
		case FLTASK_FUNC_STEPOVERCMDREQ:	RxdStepOverCmdReq(); break;
		case FLTASK_FUNC_STEPOVERCMDRES:	RxdStepOverCmdRes(); break;
		case FLTASK_FUNC_STEPOVERRETREQ:	RxdStepOverRetReq(); break;
		case FLTASK_FUNC_STEPOVERRETRES:	RxdStepOverRetRes(); break;
		default: break;	
	}
		
	return TRUE;
}

BOOL CFLSubDev::OnDataIsOK()
{
	FillFLSendData(&UploadData[0]);

	if(m_CallType == FLTASK_MODE_AUTOFL)
	{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: OnDataIsOK-->AutoFL");
#endif
		return TxdFrame(FLTASK_FUNC_RELAYRES,87);
	}
	else
	{
#ifdef	_FL_DEBUG_
		ReportMsg("FL Task: OnDataIsOK-->ManualFL");
#endif
		return TxdFrame(FLTASK_FUNC_MANUALRES,86);
	}
}

CFLTask::CFLTask(DWORD *pdwApp):CApp(pdwApp)
{
	m_nTickCnt = 0;
	m_FLSubNum = 0;
	
	m_hSocket = -1;
	m_dwError = 0;
	m_dwWriteBlockNum = 0;	
}	

// ��ȡһ֡
BOOL CFLTask::SearchOneFLFrame(PBYTE Buf,WORD wLen)
{
	TFLFrameHead		*pRxdHead=(TFLFrameHead *)Buf;
	WORD				wFrameLen;

	//���ȼ��
	if(wLen < FLTASK_MINFRMLEN)
		return	FM_LESS;
	
	//֡ͷ���
	if(pRxdHead->START!=0x68)
		return	FM_ERR;

	//֡�����
	wFrameLen = MAKEWORD(pRxdHead->DataLenLO,pRxdHead->DataLenHI)+sizeof(TFLFrameHead);
	if(wFrameLen >= FLTASK_MAXFRMLEN)
		return  FM_ERR|wLen;
		
	//У����
	if(Buf[wFrameLen-1] != LPC(0x55, Buf, wFrameLen-1))
		return	FM_ERR;

	// �л�����Ӧ�豸
	if(!SwitchToAddress(pRxdHead->DevNo))
	{
		ReportMsg("FL objiect no is unknowed! (%d)",pRxdHead->DevNo);
		return	FM_ERR|wFrameLen;
	}
	
	return (FM_OK|wFrameLen);
}

BOOL CFLTask::SearchFLFrame(TBufMsg* pMsgBuf,TBufMsg  *pFrameInfo)
{
	DWORD dwRtn;
	DWORD dwLen;//�Ѵ�������ֽ���
	short nMsgLen;
	
	NeatenIoBuf(pMsgBuf);
 
	while(1)
	{
		nMsgLen = pMsgBuf->wWritePtr - pMsgBuf->wReadPtr;
		
		if(nMsgLen<=0)
		{
			return FALSE;
		}
		
		dwRtn = SearchOneFLFrame(&pMsgBuf->pBuf[pMsgBuf->wReadPtr], nMsgLen);
		dwLen = dwRtn & ~FM_SHIELD; //Len=�Ѵ�������ֽ���

		switch(dwRtn& FM_SHIELD)
		{
		case FM_OK:
			pFrameInfo->pBuf = &pMsgBuf->pBuf[pMsgBuf->wReadPtr];  //��¼��Ч���ĵ���ʼ��ַ
			pFrameInfo->wWritePtr = (WORD)dwLen; //��¼��Ч���ĵĳ���

			pMsgBuf->wReadPtr += (WORD)dwLen; //ָ���Ƶ���һ���Ĵ�
			
			if(pMsgBuf->wReadPtr >= pMsgBuf->wWritePtr)
				pMsgBuf->wReadPtr = pMsgBuf->wWritePtr;
			
			return TRUE;
		
		case FM_ERR:
			if(!dwLen)
				pMsgBuf->wReadPtr++;
			else 
				pMsgBuf->wReadPtr += (WORD)dwLen; //ָ���Ƶ���һ���Ĵ�
			break;
		case FM_LESS:
			if(((pMsgBuf->wWritePtr-pMsgBuf->wReadPtr)>=pMsgBuf->wBufLen)&&(dwLen==0))
			  pMsgBuf->wReadPtr +=1;

			pMsgBuf->wReadPtr += (WORD)dwLen; 

			return FALSE;
		}
	}
}

BOOL CFLTask::Init(void)
{	
	// �ȴ�������������
	while(!RelayTaskInitOK)
	{
		Sleep(100);
	}

//	Sleep(((*RS_pnCAID)%MAX_SUBSTATION_NUM)*200);

	InitFLPara();
	
	// ��ʼ�������� 	
	m_RecvInfo.pBuf = (BYTE *)OS_MemAlloc(1024);
	m_RecvInfo.wBufLen = 1024;
	m_RecvInfo.wReadPtr = 0;
	m_RecvInfo.wWritePtr= 0;

	//����Ԥ���ռ�
	m_pMemBlock = OS_MemAlloc(1024);
	m_TransDataRP = DAE_GetTransDataTail();
	m_TransPortID = DAE_TRANSPORT_BROADCAST;

	// 20�������
	SetTimer(1,FLTASK_TICK);

	// ����
	//OnStepOverCmd(0);

	// ������ಢ������
	memset(&m_tRelayFLInfo,0,sizeof(TRelayFLCtrlInfo));
	
	return TRUE;
}

BOOL CFLTask::InitFLPara(void)
{	
	// ��ǰ�Ƿ�Խ��
	if(DB_GetCurrentSETAreaNo() != FLTASK_SETAREA_NORMAL)
		m_bStepOver	= TRUE;
	else
		m_bStepOver	= FALSE;

	// ������������
	if((*CFG_pbyISSS) == CFG_STATUS_SET)
	{
		ReportMsg("FL Task: Init main station!");
		// ��ʼ���������豸
		if(!InitMainST())
			return FALSE;
	}
	else
	{
		ReportMsg("FL Task: Init sub station! Addr=0x%x %d",(DWORD)CFG_pbyISSS,*CFG_pbyISSS);
		// ��ʼ������������
		if(!InitSubST())
			return FALSE;
	}

	// ����ģʽ
	m_byLinkMode = *RS_pnChanType;
	// �Ƿ���ͨ��
	if(m_byLinkMode == FLTASK_LINK_SERIAL)
	{
		m_SerPortID = FLTASK_SERPORTID;
		OpenComm(m_SerPortID, 512, 512,0);
	}

	m_DevNo = 0;
	m_bCommand = FALSE;

	m_bRelayFL = FALSE; 		//�������
	m_bManualFL = FALSE;		//�ֶ����
	m_bForeFL = FALSE;			//�ٲ���ʷ
	m_bLowValFL = FALSE;		//�ٲ��ѹ����

	return TRUE;
}

BOOL CFLTask::InitMainST(void)
{
	TFL_Disp	tFL;
	
	m_FLSubNum = DB_GetFLObjNum();
	if(m_FLSubNum == 0)
		return FALSE;
	
	m_bMainStation = TRUE;
	
	// �����ʼ�������վ����
	for(int i=0;i<m_FLSubNum;i++)
	{
		// ȡ����
		if(DB_GetFLObjDisp(i, &tFL) != DB_ERR_OK)
		{
			LogError("InitMainST",FILE_LINE,"Para error");
			return FALSE;
		}
	
		if(!m_FLSubDev[i].Init(i,&tFL,this))
			return FALSE;
	}

	// ��ȡ������ַ���
	m_FLMainNo = *RS_pnCAID;
	
	// ��ʼ��Ϊ��ѯ�Ӽ�
	SwitchPollingSubSet(FLTASK_SUBSET_POLLING);

	
	m_bCommValid = TRUE;

	// �жϵ�ǰ������ѹ���Ƿ�Ͷ��
	if(*SW_pbyFL == SW_STATUS_CUT)
	{
		ReportMsg("FL Task: Main station quit because SW_pbyFL is invalid!");
		m_bCommValid = FALSE;
	}
 	// �жϵ�ǰ��ֵ��AT���ϵ������Ƿ�Ͷ��
	else if((*CFG_pbyAT_C_RATE == CFG_STATUS_CLR)&&(*AS_pnPM == AT1MODE))
	{
		ReportMsg("FL Task: Main station quit because CFG_pbyAT_C_RATE is invalid!");
		m_bCommValid = FALSE;
	}
		
	return TRUE;
}

BOOL CFLTask::InitSubST(void)
{
	TFL_Disp	tFL;
	CFLSubDev	*pDev = m_FLSubDev;

	m_bMainStation = FALSE;
		
	// ��ȡ��վ��Ŷ�ֵ
	tFL.byFLNo = *RS_pnCAID;
	
	// ��ȡ����ģʽ
	switch(*RS_pnChanType)
	{
		case 0: tFL.wControl = 0x8000; break;	//����ֱ��
		case 1: tFL.wControl = 0x9000; break;	// ����
		case 2: tFL.wControl = 0xA000; break;	//͸�� 
		default: break;
	}
	
	// ������ַ
	tFL.byNetAddr = DB_GetDevAddress();
	
	return pDev->Init(0,&tFL,this);
}

// ���͹���
void CFLTask::TxdMonitor()
{
	CFLSubDev *pDev;

	// ������ʱ�޷��͹���
	if(!m_bMainStation)
		return;

	// ����Ҫͨ��ʱ����
	if(!m_bCommValid)
		return;
		
	// ����ͨ�� ���� ͸��
	if(m_byLinkMode != FLTASK_LINK_ETHERNET)
	{
		pDev = &m_FLSubDev[m_DevNo];
		//�ǿ���״̬ʱ����
		if(pDev->m_nTimeoutCnt != 0)
			return;

		// �л�����һ���豸
		if(!SwitchToNextSerDev())
			return;
		
		pDev = &m_FLSubDev[m_DevNo];

		// ���ʹ���
		pDev->TxdProcess();

		return;
	}

	// ����ͨ��ʱ������վ���д���
	for(int i=0;i<m_FLSubNum;i++)
	{
		pDev = &m_FLSubDev[i];
		
		// ����״̬ʱ����
		if(pDev->m_nTimeoutCnt != 0)
			continue;

		if(!pDev->IsPolling())
			continue;
		
		pDev->TxdProcess();
	}
}

void CFLTask::RxdMonitor()
{
	CFLSubDev *pDev;

	// ȡ����
	if(RecvData() == 0)
		return;
	
	if(SearchFLFrame(&m_RecvInfo,&m_FrameInfo))
	{
		// ȡ��ǰ�豸
		pDev = GetDev();

		// ��������
		pDev->OnFrame();
	}
}

BOOL CFLTask::CreateSocket()
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
		LogError("CFLTask::CreateUDPSocket",FILE_LINE,"create socket error.errno=%d",errno);
		return FALSE;
	}

	nVal = 2*1024;
	setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nVal, sizeof(int));
	setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nVal, sizeof(int));

	nVal = TRUE; 
	if((int)setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nVal, sizeof(int)) == -1)
	{
		LogError("CFLTask::CreateUDPSocket",FILE_LINE,"setsockopt error.errno=%d",errno);
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
	setsockopt(hSocket, SOL_SOCKET, SO_LINGER, \
		(char *)&Linger, sizeof(Linger));
	
	//bind local address
	memset((char *)&selfAddr, 0, sizeof(struct sockaddr_in));
	selfAddr.sin_family = AF_INET;
	selfAddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl(m_netInfo.dwSelfIP);
	selfAddr.sin_port = 6571;
	 
	if(-1 == bind(hSocket, (sockaddr*)&selfAddr, sizeof(sockaddr)))
	{
		LogError("CFLTask::CreateUDPSocket",FILE_LINE,"bind error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}
	
	nVal = 1;;
	if(-1 == (int)setsockopt(hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&nVal,sizeof(int)))
	{
		LogError("CFLTask::CreateUDPSocket",FILE_LINE,"bind error.errno=%d",errno);
		close(hSocket);
		return FALSE;
	}
		
	m_hSocket = hSocket;
	
	return TRUE;
}

BOOL CFLTask::CloseSocket()
{
	if(m_hSocket == -1)
		return FALSE;
	
	close(m_hSocket);
	m_hSocket = -1;
	return TRUE;
}

WORD CFLTask::SendUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen)
{
	long 	lgRtn;
	int 	nAddrLen;
	struct sockaddr_in toAddr;

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6571);
	toAddr.sin_addr.s_addr = htonl(ipAddr);

	nAddrLen = sizeof(struct sockaddr_in);

	if(!CreateSocket())
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
		//Ŀ������û��		
		if(errno == EHOSTDOWN)
			return FALSE;
			
		// ��������
		if(errno == EWOULDBLOCK)
		{
			m_dwWriteBlockNum ++;
			if(m_dwWriteBlockNum > 100)
			{
				m_dwWriteBlockNum = 0;
				CloseSocket();
			}
			return FALSE;
		}
		
		// ��������
		m_dwWriteBlockNum = 0;
		
		m_dwError ++;				
		if(m_dwError > 20)
		{
			m_dwError = 0;
			CloseSocket();
		}
		
		return FALSE;
	}

	return TRUE;
}

WORD CFLTask::RecvUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen)
{
	long nReadNum=0;
	int nAddrLen;
	struct sockaddr_in toAddr;

	if(wLen == 0)
		return 0;
	
	if(wLen > 1023)
		wLen = 1023;
	
	if(!CreateSocket())
		return 0;
				
	memset((void *)&toAddr,0 ,sizeof(struct sockaddr_in));
	nAddrLen = sizeof(struct sockaddr_in);
		
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(6571);
	toAddr.sin_addr.s_addr = htonl(0);
	
	nReadNum = recvfrom(m_hSocket, (char*)pBuf, wLen, 0, (sockaddr*)&toAddr, &nAddrLen);

	// =0 ʱ��ʾ�ѶϿ�
	if(nReadNum <= 0)
	{
		if(errno == EHOSTDOWN)
			return 0;
			
		if(errno == EWOULDBLOCK)
			return 0;
		
		m_dwError ++;
		if(m_dwError > 20)
		{
			m_dwError = 0;
			CloseSocket();
		}
	}
	
	if(nReadNum > 0)
	{
		m_dwError = 0;

		DWORD dwRxdIP;

		dwRxdIP = ntohl(toAddr.sin_addr.s_addr);

		// ����ʱ��Ҫ���ݶԶ˵�IP�����ַ	
		if(!IsMainStation())
			GetDev()->m_dwNetIP = dwRxdIP;
//		ReportMsg("CFLTask::RecvUDPData,From %d.%d.%d.%d Len=%d",HHBYTE(dwRxdIP),HLBYTE(dwRxdIP),\
//			LHBYTE(dwRxdIP),LLBYTE(dwRxdIP),nReadNum);
		return nReadNum;
	}

	return 0;
}

WORD CFLTask::RecvTransData(BYTE * pBuf,WORD wLen)
{
	TTransDataHead	*pTransHead = (TTransDataHead	*)m_pMemBlock;
	WORD 	Tail = DAE_GetTransDataTail();
	WORD	wRtnLen=0;
	
	for(;m_TransDataRP != Tail;m_TransDataRP = (m_TransDataRP+1)%DAE_MAX_TRANSNUM)
	{
		// ��Ч���ݼ���
		if(DAE_ReadTransData((BYTE *)m_pMemBlock,m_TransDataRP) == 0)
			continue;

		// �ǲ�������޳�
		if(pTransHead->wClass != 0x0300)
			continue;

		// �㲥��Ϣ�޳�
		if(pTransHead->wDesPort == DAE_TRANSPORT_BROADCAST)
			continue;
		
		// ���Ȳ���ֱ������
		if((wRtnLen+pTransHead->wDataLen) > wLen)
			return wRtnLen;

		// �������ݵ���ָ��
		memcpy(pBuf+wRtnLen,&pTransHead->Data,pTransHead->wDataLen);
		wRtnLen += pTransHead->wDataLen;

		// ˢ��͸���˿�ID
		m_TransPortID = pTransHead->wSrcPort;
	}

	return wRtnLen;
}

CFLSubDev *CFLTask::GetDev()
{
	if(IsMainStation())
		return  &m_FLSubDev[m_DevNo];
	else
		return m_FLSubDev;
}

WORD CFLTask::RecvData()
{
	CFLSubDev	*pDev;
	BYTE		*pBuf;
	DWORD 		dwRtn = 0;
	WORD 		wLen;

	pDev = GetDev();
	NeatenIoBuf(&m_RecvInfo);//����m_Rxd�ڴ� �������Ѵ������ݣ�
	pBuf = &m_RecvInfo.pBuf[m_RecvInfo.wWritePtr];
	wLen = m_RecvInfo.wBufLen - m_RecvInfo.wWritePtr;

	switch(m_byLinkMode)
	{
		case FLTASK_LINK_ETHERNET:
			// ����ͨ��ͨ��SOCKET����
			dwRtn = RecvUDPData(pDev->m_dwNetIP,pBuf, wLen);
			break;
			
		case FLTASK_LINK_SERIAL:
			// ����ͨ���Ӵ��ڶ�ȡ
			dwRtn = ::ReadComm(m_SerPortID, pBuf, wLen, 0);
			break;
			
		case FLTASK_LINK_TRANS:
			// ��͸����Ϣ�ռ��ȡ
			dwRtn = RecvTransData(pBuf, wLen);
			break;
		default:
			break;
		
	}

	if(dwRtn > 0)
	{
		m_RecvInfo.wWritePtr += (WORD)dwRtn;
		return dwRtn;
	}
	
	return 0;
}

BOOL CFLTask::IsCallDataEnd(BYTE byFLType)
{
	BOOL	bIsCalling = FALSE;
	
	for(int i=0;i<m_FLSubNum;i++)
	{
		if((byFLType == FLTASK_MODE_AUTOFL)&&m_FLSubDev[i].IsPolling()&&(m_FLSubDev[i].m_bRelayFL))
			bIsCalling = TRUE;
		
		if((byFLType == FLTASK_MODE_MANUALFL)&&m_FLSubDev[i].IsPolling()&&(m_FLSubDev[i].m_bManualFL))
			bIsCalling = TRUE;

		if((byFLType == FLTASK_MODE_HISTORYFL)&&m_FLSubDev[i].IsPolling()&&(m_FLSubDev[i].m_bForeFL))
			bIsCalling = TRUE;

		if((byFLType == FLTASK_MODE_LOWVOLFL)&&m_FLSubDev[i].IsPolling()&&(m_FLSubDev[i].m_bLowValFL))
			bIsCalling = TRUE;
	}

	if(bIsCalling)
		return FALSE;
	else
		return TRUE;
}

BOOL CFLTask::IsCommandEnd()
{
	BOOL	bIsCalling = FALSE;
	
	for(int i=0;i<m_FLSubNum;i++)
	{
		if(!m_FLSubDev[i].IsPolling())
			continue;
			
		if((m_CmdType == FLTASK_MODE_SOSWITCH)&&(m_FLSubDev[i].m_bStepOverCmd))
			bIsCalling = TRUE;
		
		if((m_CmdType == FLTASK_MODE_SORETURN)&&(m_FLSubDev[i].m_bStepOverRet))
			bIsCalling = TRUE;
	}

	if(bIsCalling)
		return FALSE;
	else
		return TRUE;
}

void CFLTask::OnTimeOut(DWORD id)
{
	int 			i;
	TRelayEventItem tAction;
	
	m_nTickCnt++;

	if(IsMainStation())
	{
		// ������ʱ����
		for(i=0;i<m_FLSubNum;i++)
		{
			// ����ͨ�ų�ʱ����
			if(m_FLSubDev[i].m_nTimeoutCnt > 0)
				m_FLSubDev[i].m_nTimeoutCnt--;

			//	������״̬ʱһֱ������������
			if(!m_bCommand)
			if((m_nTickCnt%FLTASK_HEARTBEET) == 0)
				m_FLSubDev[i].m_bHeart = TRUE;
		}

		// �ٲ�������
		if(m_bRelayFL)
		{
			// ���̽������
			if(IsCallDataEnd(FLTASK_MODE_AUTOFL))
			{
#ifdef	_FL_DEBUG_
				ReportMsg("FL Task: Call relay data cmd is success!!! Seq=%d",GetRelayFLSeq());
#endif				
				// ���β�������
				FLComplicateInfo[GetRelayFLSeq()].dataOKFlag = TRUE;
				PopRelayFL();

				// �����Ȼ��δ����ı���������񣬼�������
				if(HaveRelayFLCmd())
				{
					m_bRelayFL = TRUE;
					
					for(int i=0;i<m_FLSubNum;i++)
						m_FLSubDev[i].m_bRelayFL = TRUE;
				}
				else
				{
					m_bRelayFL = FALSE;
				}	

				// ֪ͨ��������Խ�����
				//OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_GETFLDATA, m_tFLTime.Lo,m_tFLTime.Hi, m_FLType, 0);
			}
		}

		if(m_bManualFL)
		{
			// ���̽������
			if(IsCallDataEnd(FLTASK_MODE_MANUALFL))
			{
#ifdef	_FL_DEBUG_
				ReportMsg("FL Task: Call manual data cmd is success!!!");
#endif
				m_bManualFL = FALSE;
				g_bCallManualFLDataCompleted = TRUE;
				// ֪ͨ��������Խ�����
				//OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_GETFLDATA, m_tFLTime.Lo,m_tFLTime.Hi, m_FLType, 0);
			}
		}

		if(m_bForeFL)
		{
			// ���̽������
			if(IsCallDataEnd(FLTASK_MODE_HISTORYFL))
			{
#ifdef	_FL_DEBUG_
				ReportMsg("FL Task: Call history data cmd is success!!!");
#endif
				m_bForeFL = FALSE;
				g_bCallHistoryFLDataCompleted = TRUE;
				// ֪ͨ��������Խ�����
				//OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_GETFLDATA, m_tFLTime.Lo,m_tFLTime.Hi, m_FLType, 0);
			}
		}

		if(m_bLowValFL)
		{
			// ���̽������
			if(IsCallDataEnd(FLTASK_MODE_LOWVOLFL))
			{
#ifdef	_FL_DEBUG_
				ReportMsg("FL Task: Call lowval data cmd is success!!!");
#endif
				m_bLowValFL = FALSE;
				g_bCallLowValueFLDataCompleted = TRUE;
				// ֪ͨ��������Խ�����
				//OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_GETFLDATA, m_tFLTime.Lo,m_tFLTime.Hi, m_FLType, 0);
			}
		}

		// Խ��������
		if(m_bCommand)
		{
			// ���̽������
			if(IsCommandEnd())
			{
				// ����ִ�н������л���������ѯģʽ
				OnStopStepOver((m_CmdType == FLTASK_MODE_SOSWITCH)?0:1);

#ifdef	_FL_DEBUG_
				ReportMsg("FL Task: Step over cmd is success!!!");
#endif

				// ֪ͨ��������Խ�����
				OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_STEPOVERECHO, 0,0,0,0);
			}

			// ��ʱ���
			if(m_nCommandCnt > 0)
			{
				m_nCommandCnt--;
				if(m_nCommandCnt == 0)
				{
					ReportMsg("FL Task: Step over cmd is timeout!!!");

					// ����ִ�н������л���������ѯģʽ
					OnStopStepOver((m_CmdType == FLTASK_MODE_SOSWITCH)?0:1);						
					
					// ֪ͨ��������Խ�����
					OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_STEPOVERECHO, 1,0, 0, 0);
				}
			}	
		}
	}

		// TEST
		// �����һ���ֻ�
/*		
		static DWORD TestCnt=0;
		BYTE	TestBuf1[10] = {0x68,0x01,0x00,0x00,0x00,0x69};
		BYTE	TestBuf2[10] = {0x68,0x01,0x01,0x00,0x00,0x6A};
		BYTE	TestBuf3[10] = {0x68,0x01,0x02,0x00,0x00,0x6B};
		BYTE	TestBuf4[10] = {0x68,0x01,0x03,0x00,0x00,0x6C};
		BYTE	TestBuf5[10] = {0x68,0x01,0x04,0x00,0x00,0x6D};
		BYTE	TestBuf6[10] = {0x68,0x01,0x05,0x01,0x00,0x01,0x70};
		BYTE	TestBuf7[10] = {0x68,0x01,0x06,0x01,0x00,0x01,0x71};

		if((m_nTickCnt%(30000/FLTASK_TICK)) == 0)
		{
			if((TestCnt%6) == 0)
				OnCallSubData(0x012345,0x6789,FLTASK_MODE_AUTOFL);
			if((TestCnt%6) == 1)
				OnCallSubData(0x012345,0x6789,FLTASK_MODE_MANUALFL);
			if((TestCnt%6) == 2)
				OnCallSubData(0x012345,0x6789,FLTASK_MODE_HISTORYFL);
			if((TestCnt%6) == 3)
				OnCallSubData(0x012345,0x6789,FLTASK_MODE_LOWVOLFL);
			if((TestCnt%6) == 4)
				OnStepOverCmd(0);
			if((TestCnt%6) == 5)
				OnStepOverCmd(1);

			if((TestCnt%7) == 0)
				DAE_WriteTransData(0x0300,SYS_PORTID_FL, 6, TestBuf1);
			if((TestCnt%7) == 1)
				DAE_WriteTransData(0x0300,SYS_PORTID_FL, 6, TestBuf2);
			if((TestCnt%7) == 2)
				DAE_WriteTransData(0x0300,SYS_PORTID_FL, 6, TestBuf3);
			if((TestCnt%7) == 3)
				DAE_WriteTransData(0x0300,SYS_PORTID_FL, 6, TestBuf4);
			if((TestCnt%7) == 4)
				DAE_WriteTransData(0x0300,SYS_PORTID_FL, 6, TestBuf5);

			if((TestCnt%2) == 0)		
				OnStepOverCmd(0); // Խ��
			if((TestCnt%2) == 1)
				DAE_WriteTransData(0x0300,SYS_PORTID_FL, 7, TestBuf6); // �Բ�Խ��
		
			TestCnt++;
		}
*/

	// �������͹���
	TxdMonitor();

	// ���ݽ��մ���
	RxdMonitor();
}

// �������
void CFLTask::OnCallSubData(DWORD dwTimeLO,WORD wTimeHi,WORD Mode)
{
	BYTE FLType;
	BYTE FLSeq;
	
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: OnCallSubData() Mode=%x @@@@@@@@@@@@@@@@@",Mode);
#endif

	if(!m_bMainStation)
		return;

	if(!m_bCommValid)
		return;

	// �����ٲ�	
	m_tFLTime.Lo = dwTimeLO;
	m_tFLTime.Hi = wTimeHi;

	FLType = Mode&0xFF;
	FLSeq = (Mode>>8)&0xFF;

	switch(FLType)
	{
		case FLTASK_MODE_AUTOFL:	
			// ������ಢ�����
			if(PushRelayFL(FLSeq,&m_tFLTime))
			{			
				// ���ڴ�������н�ֹ���
				if(!m_bRelayFL)
				{
					m_bRelayFL = TRUE;
					for(int i=0;i<m_FLSubNum;i++)
						m_FLSubDev[i].m_bRelayFL = TRUE;
				}

				FLComplicateInfo[FLSeq].dataOKFlag = FALSE;
			}
			break;
			
		case FLTASK_MODE_MANUALFL:
			m_bManualFL = TRUE;
			g_bCallManualFLDataCompleted = FALSE;
			for(int i=0;i<m_FLSubNum;i++)
				m_FLSubDev[i].m_bManualFL = TRUE;
			break;
			
		case FLTASK_MODE_HISTORYFL:
			m_bForeFL = TRUE;
			g_bCallHistoryFLDataCompleted = FALSE;
			for(int i=0;i<m_FLSubNum;i++)
				m_FLSubDev[i].m_bForeFL = TRUE;
			break;
			
		case FLTASK_MODE_LOWVOLFL:
			m_bLowValFL = TRUE;
			g_bCallLowValueFLDataCompleted = FALSE;
			for(int i=0;i<m_FLSubNum;i++)
				m_FLSubDev[i].m_bLowValFL = TRUE;
			break;
			
		default:
			LogError("OnCallSubData",FILE_LINE,"Invalid mode (%x)!",Mode);
			break;
	}
}

// ֹͣ���
void CFLTask::OnStopCallData(DWORD Mode)
{
	BYTE FLType;
	BYTE FLSeq;
	
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: OnStopCallData() Mode=%x @@@@@@@@@@@@@@@@@",Mode);
#endif

	if(!m_bMainStation)
		return;

	FLType = Mode&0xFF;
	FLSeq = (Mode>>8)&0xFF;
	
	// ֹͣ�ٲ�
	switch(FLType)
	{
		case FLTASK_MODE_AUTOFL:
			// �޳���ǰ��ѯ
			if(IsCurrentRelayFL(FLSeq))
			{
				PopRelayFL();

				if(HaveRelayFLCmd())
				{
					// ��������
					m_bRelayFL = TRUE;
					for(int i=0;i<m_FLSubNum;i++)
						m_FLSubDev[i].m_bRelayFL = TRUE;
				}
				else
				{
					// ȫ���ر�
					m_bRelayFL = FALSE;
					for(int i=0;i<m_FLSubNum;i++)
						m_FLSubDev[i].m_bRelayFL = FALSE;
				}
			}
			// �޳�������ѯ
			else
				DelRelayFL(FLSeq);
			
			break;
			
		case FLTASK_MODE_MANUALFL:
			m_bManualFL = FALSE;
			for(int i=0;i<m_FLSubNum;i++)
				m_FLSubDev[i].m_bManualFL = FALSE;
			break;
			
		case FLTASK_MODE_HISTORYFL:
			m_bForeFL = FALSE;
			for(int i=0;i<m_FLSubNum;i++)
				m_FLSubDev[i].m_bForeFL = FALSE;
			break;
			
		case FLTASK_MODE_LOWVOLFL:
			m_bLowValFL = FALSE;
			for(int i=0;i<m_FLSubNum;i++)
				m_FLSubDev[i].m_bLowValFL = FALSE;
			break;
			
		default:
			LogError("OnCallSubData",FILE_LINE,"Invalid mode (%x)!",Mode);
			break;
	}
}

// �������׼����
void CFLTask::OnDataIsOk(DWORD DataPtr,WORD Result)
{
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: OnDataIsOk() Result=%d",Result);
#endif

	if(m_bMainStation)
		return;

	// ���Ͳ������
	GetDev()->OnDataIsOK();
}

// Խ������
void CFLTask::OnStepOverCmd(DWORD Mode)
{
	STATUS 		rc;

	if(!m_bMainStation)
		return;

	if(!m_bCommValid)
		return;

#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: OnStepOverCmd() Mode=%d  @@@@@@@@@@@@@@@@@",Mode);
#endif
	
	// �л���ֵ��
	if(Mode == 0)
	{
		// Խ������
		rc = DB_SetCurrentSETAreaNo(FLTASK_SETAREA_STEPOVER_A);
		rc |= DB_SetCurrentSETAreaNoConfirm(FLTASK_SETAREA_STEPOVER_A);
		if(rc != DB_ERR_OK)
		{
			LogError("OnStepOverCmd",FILE_LINE,"Set SET error!");
			return;
		}
			
		m_CmdType = FLTASK_MODE_SOSWITCH;	
	}
	else
	{
		// Խ������
		rc = DB_SetCurrentSETAreaNo(FLTASK_SETAREA_NORMAL);
		rc |= DB_SetCurrentSETAreaNoConfirm(FLTASK_SETAREA_NORMAL);
		if(rc != DB_ERR_OK)
		{
			LogError("OnStepOverRet",FILE_LINE,"Read SET error!");
			return;
		}

		m_CmdType = FLTASK_MODE_SORETURN;	
	}
	
	// ���³�ʼ��������
	InitFLPara();
	
	// ˢ�������Ӽ�
	SwitchPollingSubSet(FLTASK_SUBSET_COMMAND);
	
	//�������п����������Ϳ�����Ϣ
	for(int i=0;i<m_FLSubNum;i++)
	{
		m_FLSubDev[i].ClearAllPollCmd();
		
		if(m_FLSubDev[i].IsPolling())
		{
			if(Mode == 0)
				// Խ������
				m_FLSubDev[i].m_bStepOverCmd = TRUE;
			else
				// Խ������
				m_FLSubDev[i].m_bStepOverRet = TRUE;
		}	
	}

	m_bCommand = TRUE;
	m_nCommandCnt = FLTASK_COMMBREAK_MAX;	
}

void CFLTask::OnStopStepOver(DWORD Mode)
{
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: OnStopStepOver() Mode=%d",Mode);
#endif

	if(!m_bMainStation)
		return;

	if(!m_bCommValid)
		return;

	// ֹͣ�ٲ�
	m_bCommand = FALSE;
	m_CmdType = 0xFF;

	// ֹͣ���в�����
	for(int i=0;i<m_FLSubNum;i++)
		m_FLSubDev[i].ClearAllPollCmd();

	// �л��ز��ģʽ	
	SwitchPollingSubSet(FLTASK_SUBSET_POLLING);
}

/****************************************************************************************
��  ��: OnRelayChange
��  ��: ���������޸ĺ����Ӧ���������³�ʼ��װ�ò����ṹ
��  ��: ��
����ֵ: ��
*****************************************************************************************/
void CFLTask::OnRelayChange(DWORD dwDataType,DWORD dwIndex)
{
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: OnRelayChange() @@@@@@@@@@@@@@@@@");
#endif
	Sleep(50);
	InitFLPara();
}

BOOL CFLTask::SwitchToNextSerDev(void)
{
	DWORD 			i;
	CFLSubDev		*pDev;

	// ����ֱ��ͨ��ʱ��Ч
	if(m_byLinkMode == FLTASK_LINK_ETHERNET)
		return 0;
	
	for(i=0;i<m_FLSubNum;i++)
	{
		pDev = &m_FLSubDev[i];

		// �����������ѯ
		if(!pDev->IsPolling())
			continue;

		// ����Ѿ��������������һ��
		if(pDev->m_bProcFlag)
			continue;

		pDev->m_bProcFlag = TRUE;
		// ���õ�ǰ�豸���
		m_DevNo = i;
		return TRUE;
	}
	
	//��ͨѶȫ��ʹ��
	for(i=0;i<m_FLSubNum;i++)
	   m_FLSubDev[i].m_bProcFlag = FALSE;//���Ѵ����־

	//�ҵ���һ����ѯ�豸
	for(i=0;i<m_FLSubNum;i++)
	{
		pDev = &m_FLSubDev[i];

		if(!pDev->IsPolling())
			continue;

		pDev->m_bProcFlag = TRUE;
		// ���õ�ǰ�豸���
		m_DevNo = i;
		return TRUE;
	}
	return 	FALSE;
}

BOOL CFLTask::SwitchToAddress(BYTE Addr)
{
	if(IsMainStation())
	{
		for(int i=0;i<m_FLSubNum;i++)
		{
			if(m_FLSubDev[i].GetFLNo() == Addr)
			{
				m_DevNo = i;
				return TRUE;
			}
		}

		// ������Խ���������
		if(m_FLMainNo == Addr)
			return TRUE;
			
		return FALSE;
	}
	else
		return (m_FLSubDev->GetFLNo() == Addr);
}

// ������ѯ�Ӽ�
void CFLTask::SwitchPollingSubSet(BYTE Mode)
{
	int 	i,Cnt;
	
	if(!IsMainStation())
	{
		ReportMsg("FL Task: Sub station switch polling set error!");
		return;
	}
	
	if(Mode == FLTASK_SUBSET_POLLING)
	{
		if(IsStepOver())
		{
			// Խ��ʱ���ݱ�־ʹ��
			for(i=0,Cnt=0;i<m_FLSubNum;i++)
			{		
				if(m_FLSubDev[i].IsValid())
				{
					m_FLSubDev[i].Include();
					m_FLSubDev[i].m_byFLDataIndex = Cnt;
					Cnt++;
				}
				else
				{
					m_FLSubDev[i].Disinclude();
					m_FLSubDev[i].m_byFLDataIndex = 0xFF;
				}	
			}	
		}
		else
		{
			// ��Խ��ʱ�����豸ʹ��
			for(i=0,Cnt=0;i<m_FLSubNum;i++)
			{		
				if(!m_FLSubDev[i].IsSideDev())
				{
					m_FLSubDev[i].Include();
					m_FLSubDev[i].m_byFLDataIndex = Cnt;
					Cnt++;
				}
				else
				{
					m_FLSubDev[i].Disinclude();
					m_FLSubDev[i].m_byFLDataIndex = 0xFF;
				}	
			}	
		}
	}

	if(Mode == FLTASK_SUBSET_COMMAND)
	{
		// �����л�ģʽ��������AT����ȫ��ʹ��
		for(i=0,Cnt=0;i<m_FLSubNum;i++)
		{		
//			if( (m_FLSubDev[i].GetDevType()) == FL_SUBST_SELFAT )
//			{
//				m_FLSubDev[i].Disinclude();
//				m_FLSubDev[i].m_byFLDataIndex = 0xFF;
//			}
//			else
			{
				m_FLSubDev[i].Include();
				m_FLSubDev[i].m_byFLDataIndex = Cnt;
				Cnt++;
			}
			
		}	
	}

	// ��������
	if(Cnt != *RS_pnN)
		LogError("SwitchPollingSubSet",FILE_LINE,"FL dev num is fault!");
}

BOOL CFLTask::PushRelayFL(BYTE Seq,TAbsTime *pTime)
{
	if(Seq >= FLCOMPLICATENUM)
	{
		LogError("PushRelayFL",FILE_LINE,"Invalid Relay FL seq(%d)!",Seq);
		return FALSE;
	}

	if(m_tRelayFLInfo.m_SeqCnt >= FLCOMPLICATENUM)
	{
		LogError("PushRelayFL",FILE_LINE,"Relay FL couter is OVERFLOW(%d)!",m_tRelayFLInfo.m_SeqCnt);
		return FALSE;
	}

	for(int i=0;i<m_tRelayFLInfo.m_SeqCnt;i++)
	{
		if(m_tRelayFLInfo.m_SeqList[i] == Seq)
		{
			LogError("PushRelayFL",FILE_LINE,"Relay FL seq is conflict!(%d)",m_tRelayFLInfo.m_SeqCnt);
			return FALSE;
		}
	}
	
	// ��¼��ź�ʱ��	
	m_tRelayFLInfo.m_SeqList[m_tRelayFLInfo.m_SeqCnt] = Seq;
	m_tRelayFLInfo.m_Time[m_tRelayFLInfo.m_SeqCnt].Lo = pTime->Lo;
	m_tRelayFLInfo.m_Time[m_tRelayFLInfo.m_SeqCnt].Hi = pTime->Hi;

	m_tRelayFLInfo.m_SeqCnt++;

#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: Push relay FL process! seq=%d CNT=%d<-----------",Seq,m_tRelayFLInfo.m_SeqCnt);
#endif

	return TRUE;
}

BOOL CFLTask::PopRelayFL()
{
	if(m_tRelayFLInfo.m_SeqCnt == 0)
		return FALSE;

#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: Pop relay FL process seq=%d!  Left=%d----------->",m_tRelayFLInfo.m_SeqList[0],m_tRelayFLInfo.m_SeqCnt-1);
#endif

	// ɾ����һ��
	for(int i=0;i<m_tRelayFLInfo.m_SeqCnt-1;i++)
		m_tRelayFLInfo.m_SeqList[i] = m_tRelayFLInfo.m_SeqList[i+1];
	
	m_tRelayFLInfo.m_SeqCnt--;

}

BOOL CFLTask::DelRelayFL(BYTE Seq)
{
	BYTE i,FoundNo=0xFF;
	
	if(m_tRelayFLInfo.m_SeqCnt == 0)
		return FALSE;

	// �ҳ����
	for(i=0;i<m_tRelayFLInfo.m_SeqCnt;i++)
	{
		if(m_tRelayFLInfo.m_SeqList[i] == Seq)
		{
			FoundNo = i;
			break;
		}
	}

	// δ�ҵ�
	if(FoundNo == 0xFF)
		return FALSE;

	// �޳�	
	for(i=FoundNo;i<m_tRelayFLInfo.m_SeqCnt-1;i++)
		m_tRelayFLInfo.m_SeqList[i] = m_tRelayFLInfo.m_SeqList[i+1];
		
	m_tRelayFLInfo.m_SeqCnt--;
	
#ifdef	_FL_DEBUG_
	ReportMsg("FL Task: Delete relay FL process! seq=%d Left=%d----------->",Seq,m_tRelayFLInfo.m_SeqCnt);
#endif

	return TRUE;
}

BOOL CFLTask::IsCurrentRelayFL(BYTE Seq)
{
	return (m_tRelayFLInfo.m_SeqList[0] == Seq);
}

BOOL CFLTask::HaveRelayFLCmd()
{
	if(m_tRelayFLInfo.m_SeqCnt > 0)
		return TRUE;
	else
		return FALSE;
}


BYTE CFLTask::GetRelayFLSeq()
{
	return m_tRelayFLInfo.m_SeqList[0];
}

BOOL CFLTask::GetRelayFLTime(TAbsTime *pTime)
{
	if(m_tRelayFLInfo.m_SeqCnt == 0)
		return FALSE;
	
	pTime->Lo = m_tRelayFLInfo.m_Time[m_tRelayFLInfo.m_SeqCnt-1].Lo;
	pTime->Hi = m_tRelayFLInfo.m_Time[m_tRelayFLInfo.m_SeqCnt-1].Hi;
	
	return TRUE;
}

#endif
