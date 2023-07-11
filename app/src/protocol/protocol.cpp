#include "msgmap.h"
#include "protocol.h"

BYTE BIT8_L1[]={
	0x80,		0x40,		0x20,		0x10,
	0x8,		0x4,		0x2,		0x1};

BYTE BIT8_R1[]={
	0x1,		0x2,		0x4,		0x8,
	0x10,		0x20,		0x40,		0x80};

WORD BIT16_R1[]={
	0x1,		0x2,		0x4,		0x8,
	0x10,		0x20,		0x40,		0x80,
	0x100,		0x200,		0x400,		0x800,
	0x1000,		0x2000,		0x4000,		0x8000};

WORD BIT16_L1[]={
	0x8000,		0x4000,		0x2000,		0x1000,
	0x800,		0x400,		0x200,		0x100,
	0x80,		0x40,		0x20,		0x10,
	0x8,		0x4,		0x2,		0x1};

DWORD BIT32_R1[]={
	0x1,		0x2,		0x4,		0x8,
	0x10,		0x20,		0x40,		0x80,
	0x100,		0x200,		0x400,		0x800,
	0x1000,		0x2000,		0x4000,		0x8000,
	0x10000,	0x20000,	0x40000,	0x80000,
	0x100000,	0x200000,	0x400000,	0x800000,
	0x1000000,	0x2000000,	0x4000000,	0x8000000,
	0x10000000,	0x20000000,	0x40000000,	0x80000000};

DWORD BIT32_L1[]={
	0x80000000,	0x40000000,	0x20000000,	0x10000000,
	0x8000000,	0x4000000,	0x2000000,	0x1000000,
	0x800000,	0x400000,	0x200000,	0x100000,
	0x80000,	0x40000,	0x20000,	0x10000,
	0x8000,		0x4000,		0x2000,		0x1000,
	0x800,		0x400,		0x200,		0x100,
	0x80,		0x40,		0x20,		0x10,
	0x8,		0x4,		0x2,		0x1};


void NeatenIoBuf(TBufMsg* pMsgBuf)
{
	register DWORD i,j;
	
    if(pMsgBuf->wReadPtr == 0)
    	return;
    
    	//�ݴ���
   	if(pMsgBuf->wReadPtr >= pMsgBuf->wWritePtr)//��ָ��>=дָ��Ϊ����
    {   
        pMsgBuf->wReadPtr = pMsgBuf->wWritePtr = 0;
        
        return ;
    }
    
    if(pMsgBuf->wWritePtr > pMsgBuf->wBufLen)//дָ��>��������С
    {   
        pMsgBuf->wReadPtr = pMsgBuf->wWritePtr = 0;
        
        return ;
    }
    
   	//��ǰ�ƶ�����
    i=0; 
    j = pMsgBuf->wReadPtr;
    
    while(j < pMsgBuf->wWritePtr)
        pMsgBuf->pBuf[i++] = pMsgBuf->pBuf[j++];
	
    pMsgBuf->wReadPtr = 0; //���ö�ָ��
    pMsgBuf->wWritePtr = i; //����дָ��
    
	return;
}


BEGIN_MESSAGE_MAP(CProtocol,CApp)
ON_SM_WRITECONT(CProtocol)
ON_SM_READCOMM(CProtocol)
END_MESSAGE_MAP()

CProtocol::CProtocol(DWORD *pdwApp):CApp(pdwApp)
{
	memset(&m_RxdInfo, 0, sizeof(m_RxdInfo));
	memset(&m_TxdInfo, 0, sizeof(m_TxdInfo));
	memset(&m_FrameInfo, 0, sizeof(m_FrameInfo));
	memset(&m_MsgCtrl, 0, sizeof(m_MsgCtrl));
	
	m_dwAppID = pdwApp[0];

	m_MsgCtrl.dwCode    = 0x00000000;
	m_MsgCtrl.dwChgFlag = 0xFFFFFFFF;
	m_wPortID = pdwApp[1];

	DAE_Portid2AppInfo(m_wPortID,&m_App);
		
	m_RxdInfo.wBufLen = 2048;
	m_TxdInfo.wBufLen = 2048;	
}

BOOL CProtocol::Init()
{
	BYTE bCommStatus;
	
	if(m_RxdInfo.wBufLen == 0)
		m_RxdInfo.wBufLen = 2048;
	if(m_TxdInfo.wBufLen == 0)
		m_TxdInfo.wBufLen = 2048;
		
	m_RxdInfo.pBuf = new BYTE[m_RxdInfo.wBufLen];	
	m_TxdInfo.pBuf = new BYTE[m_TxdInfo.wBufLen];
		
	if(m_RxdInfo.pBuf == NULL || m_TxdInfo.pBuf == NULL)
		return FALSE;
	
	DisableRetry();

	memset(&m_Flags,0,sizeof(FLAGS));
	
	m_GBAddressFlag=1;
	m_CommErrCnt = 0;

	m_TxdFrmCnt = 0;
	m_RxdFrmCnt = 0;

	DAE_GetCommStatus(m_wPortID, bCommStatus);
	if(bCommStatus == COMM_STATUS_OK)
		SetFlag(SF_CommErr,FLAG_OFF);
	else
		SetFlag(SF_CommErr,FLAG_ON);
	
	return CommOpen();
}

void CProtocol::SetBufLen(WORD wRxdLen, WORD wTxdLen)
{
	m_RxdInfo.wBufLen = wRxdLen;
	m_TxdInfo.wBufLen = wTxdLen;
}

BYTE* CProtocol::GetTxdBuf()
{
	return m_TxdInfo.pBuf;
}

void CProtocol::GetBufLen(WORD& wRxdLen, WORD& wTxdLen)//referrence call
{
	wRxdLen = m_RxdInfo.wBufLen;
	wTxdLen = m_TxdInfo.wBufLen;
}

BOOL CProtocol::SearchFrame(void)
{
	DWORD dwRtn;
	DWORD dwLen;//�Ѵ�������ֽ���
	short nMsgLen;
	
	m_GBAddressFlag=1;
	NeatenIoBuf(&m_RxdInfo);
 
	while(1)
	{
		nMsgLen = m_RxdInfo.wWritePtr - m_RxdInfo.wReadPtr;
		
		if(nMsgLen<=0)
		{
			m_bHaveRxdFm = FALSE;
			return FALSE;
		}
		
		dwRtn = SearchOneFrame(&m_RxdInfo.pBuf[m_RxdInfo.wReadPtr], nMsgLen);
		dwLen = dwRtn & ~FM_SHIELD; //Len=�Ѵ�������ֽ���

		switch(dwRtn& FM_SHIELD)
		{
		case FM_OK:
			DisableRetry();
			m_FrameInfo.pBuf = &m_RxdInfo.pBuf[m_RxdInfo.wReadPtr];  //��¼��Ч���ĵ���ʼ��ַ
			m_FrameInfo.wWritePtr = (WORD)dwLen; //��¼��Ч���ĵĳ���

			m_RxdInfo.wReadPtr += (WORD)dwLen; //ָ���Ƶ���һ���Ĵ�
			
//			if(!m_GBAddressFlag)	
//				CommRegister(DCR_Rxd);//ע�ᵱǰ�豸����һ����
			DisplayRxdBuf();//for debug

			if(m_RxdInfo.wReadPtr >= m_RxdInfo.wWritePtr)
				m_bHaveRxdFm = FALSE;
			
			return TRUE;
		
		case FM_ERR:
			if(!dwLen)
				m_RxdInfo.wReadPtr++;
			else 
				m_RxdInfo.wReadPtr += (WORD)dwLen; //ָ���Ƶ���һ���Ĵ�
			break;
		case FM_LESS:
			/* 2003-5-22*/
			/*��ǿ�ݴ��ԡ�����Լ���ջ��������������ҹ�Լ����֡�������Ϊ֡���Ȳ�������������*/
			/*��������˿ڻ�����������Ҳ������������Ϊ�˿�����һֱ���ܱ����ߣ��˿��ػ�����*/
			/*һֱ���ù�Լ����readcomm��Ϣ����������commidle��Ϣ���ܲ�������ʱӦ����ָ����ơ�*/
			
			if(((m_RxdInfo.wWritePtr-m_RxdInfo.wReadPtr)>=m_RxdInfo.wBufLen)&&(dwLen==0))
			  m_RxdInfo.wReadPtr +=1;

			m_RxdInfo.wReadPtr += (WORD)dwLen; 

			m_bHaveRxdFm = FALSE;
			
			return FALSE;
		}
	}
}

DWORD CProtocol::SearchOneFrame(BYTE* pBuf, WORD nLen)
{
	return FM_ERR|1;
}


DWORD CProtocol::GetGBAddress(void)
{
	return 0xffff;
}

BOOL CProtocol::CommOpen()
{
 	if(::OpenComm(m_wPortID, DRVBUFSIZE, DRVBUFSIZE,0) != 0)
		return FALSE;

	return TRUE;	
}

void CProtocol::OnWriteCont(DWORD dwPort, DWORD dwFlag)
{
	DWORD dwRtn;
    BYTE* pBuf;
	WORD wLen;
    
    pBuf = &m_TxdInfo.pBuf[m_TxdInfo.wReadPtr];
    wLen = m_TxdInfo.wWritePtr - m_TxdInfo.wReadPtr;
    
    if(wLen > 0)
	{ 
		dwRtn = ::WriteComm(m_wPortID, pBuf, wLen, 0);
//		ReportMsg("Protocol WriteCont Txd Read=%d Write=%d TxdNum=%d RealNum=%d",\
//			m_TxdInfo.wReadPtr,m_TxdInfo.wWritePtr,wLen,dwRtn);		
		m_TxdInfo.wReadPtr += dwRtn;
	}
}

DWORD CProtocol::ReadFromComm()
{
	DWORD dwRtn;
	BYTE* pBuf;
	WORD wLen;
	
	NeatenIoBuf(&m_RxdInfo);//����m_Rxd�ڴ�	�������Ѵ������ݣ�
	
	pBuf = &m_RxdInfo.pBuf[m_RxdInfo.wWritePtr];
    	wLen = m_RxdInfo.wBufLen - m_RxdInfo.wWritePtr;
    

	dwRtn = ::ReadComm(m_wPortID, pBuf, wLen, 0);

	if(dwRtn)
	{		
		m_bHaveRxdFm = TRUE;
		m_RxdInfo.wWritePtr += (WORD)dwRtn;
	}
	else 
		m_bHaveRxdFm = FALSE;
	
    return dwRtn;
}

WORD CProtocol::WriteComm(WORD wLen, WORD Address, DWORD dwFlag)
{
	WORD 	wRtn;
	BYTE* 	pBuf;
	
	if(Address == GetGBAddress())
	{
		dwFlag |= WCF_BROADCAST;//�㲥����
		DisableRetry();
	}
//	else 
//		CommRegister(DCR_Txd);//ע�ᵱǰ�豸����һ�ǹ㲥����
	
	dwFlag |= WCF_FRAMETAIL;//���͵ı��Ľ���	
	m_TxdInfo.wWritePtr = wLen;
	m_TxdInfo.wReadPtr = 0;
	m_TxdFlag = Address|dwFlag;
	
	pBuf = &m_TxdInfo.pBuf[m_TxdInfo.wReadPtr];
	
	if(wLen)
	{	
		DisplayTxdBuf();//for debug
		wRtn = ::WriteComm(m_wPortID, pBuf, wLen, m_TxdFlag);
//		ReportMsg("Protocol WriteComm Txd Read=%d Write=%d TxdNum=%d RealNum=%d",\
//			m_TxdInfo.wReadPtr,m_TxdInfo.wWritePtr,wLen,wRtn);		
		m_TxdInfo.wReadPtr += wRtn;
		
		return wRtn;
	}
	return 0;
}

void CProtocol::TxdRetry(void)
{
	WORD wLen;
	
	wLen = m_TxdInfo.wWritePtr;
	if( wLen )
	{
		WriteComm(wLen,(WORD)(m_TxdFlag&0xff),m_TxdFlag&0xffff0000);
	}
}

void CProtocol::OnReadComm(DWORD dwPort, DWORD dwFlag)
{
	ReadFromComm(); 

	while(m_bHaveRxdFm)
	{	
		if(SearchFrame())
			OnFrame(m_FrameInfo.pBuf);
	}
} 
  
void CProtocol::StartMsg(DWORD dwMsg)
{
	m_MsgCtrl.dwCode    |= dwMsg;
	m_MsgCtrl.dwChgFlag |= dwMsg;
	
	CommCtrl();
}

void CProtocol::StartMsg(DWORD dwMsg, DWORD dwMsgValue)
{
	switch(dwMsg)
	{
	case MCC_ReadComm:		
		m_MsgCtrl.wRxdPreNum = dwMsgValue;		
		break;
	case MCC_WriteComm:		
		m_MsgCtrl.wTxdPreNum = dwMsgValue;		
		break;
	case MCC_CommIdle:		
		m_MsgCtrl.dwCommIdleTime = dwMsgValue;	
		break;
	case MCC_CommTxdIdle:	
		m_MsgCtrl.dwTxdIdleTime = dwMsgValue;
		break;
	default:
		return;
	}
	
	StartMsg(dwMsg);
}

void CProtocol::StopMsg(DWORD dwMsgNo)
{	
	m_MsgCtrl.dwCode    &= ~dwMsgNo;
	m_MsgCtrl.dwChgFlag |= dwMsgNo;
	
	CommCtrl();
}

void CProtocol::CommCtrl(void)
{
	TCtlComm CtlComm;
	
	CtlComm.dwCtlMask = 0xFFFFFFFF;
	CtlComm.dwFuncCode = FC_GET;
	
	CtrlComm(m_wPortID, &CtlComm);
	
//	CtlComm.dwCtlMask = 0;
	
	if(m_MsgCtrl.dwChgFlag & MCC_ReadComm)
	{
		m_MsgCtrl.dwChgFlag ^= MCC_ReadComm;
		if(m_MsgCtrl.wRxdPreNum > DRVBUFSIZE )
			m_MsgCtrl.wRxdPreNum = DRVBUFSIZE - 10;
		
		CtlComm.dwCtlMask |= CM_NOTIFYREAD;
		if(m_MsgCtrl.dwCode & MCC_ReadComm ) 
			CtlComm.dwNotifyRead =  m_MsgCtrl.wRxdPreNum;//��ҪSM_READCOMM��Ϣ
		else 
		{
			CtlComm.dwCtlMask &= ~MCC_ReadComm;
			CtlComm.dwNotifyRead = 0; 
		}
	}
	
	if(m_MsgCtrl.dwChgFlag & MCC_WriteComm)
	{
		m_MsgCtrl.dwChgFlag ^= MCC_WriteComm;
		CtlComm.dwCtlMask	|= CM_NOTIFYWRITE;
		
		if(m_MsgCtrl.wTxdPreNum > 100)
			m_MsgCtrl.wTxdPreNum = 100;
		
		if(m_MsgCtrl.dwCode & MCC_WriteComm )//��ҪSM_WRITECOMM��Ϣ
			CtlComm.dwNotifyWrite = m_MsgCtrl.wTxdPreNum; 
		else
		{
			CtlComm.dwCtlMask	&= ~CM_NOTIFYWRITE;
			CtlComm.dwNotifyWrite = 0; 
		}
	}
	
	if(m_MsgCtrl.dwChgFlag & MCC_CommIdle)
	{
		m_MsgCtrl.dwChgFlag ^= MCC_CommIdle;
		CtlComm.dwCtlMask   |= CM_NOTIFYCOMMBREAK;

		if(m_MsgCtrl.dwCode & MCC_CommIdle )//��ҪSM_COMMIDLE��Ϣ
			CtlComm.dwNotifyCommBreak = m_MsgCtrl.dwCommIdleTime; 
		else
		{		
			CtlComm.dwCtlMask   &= ~CM_NOTIFYCOMMBREAK;
			CtlComm.dwNotifyCommBreak = 0; 
		}
	}
	
	if(m_MsgCtrl.dwChgFlag & MCC_CommTxdIdle)
	{
		m_MsgCtrl.dwChgFlag ^= MCC_CommTxdIdle;
		CtlComm.dwCtlMask |= CM_NOTIFYTXDBREAK;
				
		if(m_MsgCtrl.dwCode & MCC_CommTxdIdle )//��ҪSM_TXDIDLE��Ϣ
			CtlComm.dwNotifyTxdBreak = m_MsgCtrl.dwTxdIdleTime; 
		else
		{
			CtlComm.dwCtlMask &= ~CM_NOTIFYTXDBREAK;
			CtlComm.dwNotifyTxdBreak = 0; 
		}
	}
	
	CtlComm.dwFuncCode = FC_SET;
	
	::CtrlComm(m_wPortID, &CtlComm);

	return;
}


BOOL CProtocol::ShowMsg(void* head,BYTE *pbuf,WORD len,void *tail)
{
	DisplayCommBuf(GetPortID(),head,pbuf,len,tail);
  	return TRUE;
}

void CProtocol::DisplayRxdBuf()
{
	char strHead[32];
	TSysTime tm;

	ReadSystemTime(&tm);
	
	sprintf(strHead,"\r\nRxd%4d(%02d:%02d:%02d-%03d):",m_RxdFrmCnt++,tm.Hour,tm.Minute,tm.Second,tm.MSecond);
	
	ShowMsg(strHead,m_FrameInfo.pBuf,m_FrameInfo.wWritePtr,(char *)"[OK]");
}

void CProtocol::DisplayTxdBuf()
{
	char strHead[32];
	TSysTime tm;

	ReadSystemTime(&tm);
	
	sprintf(strHead,"\r\nTxd%4d(%02d:%02d:%02d-%03d):",m_TxdFrmCnt++,tm.Hour,tm.Minute,tm.Second,tm.MSecond);
	
	ShowMsg(strHead,m_TxdInfo.pBuf,m_TxdInfo.wWritePtr,(char *)"\r\n");
}

//========================================================================================================
//  ��������: SetFlag
//  �������: pFlags��m_Flags��������ַ  FlagNo����Ǻ� Status ����ǵ�״̬��ON/OFF
//  ��������: ��Flags�������ڵĵ�FlagNo��λ��λ��Status
//  ����ֵ��  ��
//========================================================================================================
void CProtocol::SetFlag(DWORD FlagNo,DWORD Status)
{// ����ÿ��ģ��ı�־
	DWORD ByteNo; //FlagNo�������ֽں�
	DWORD BitNo;  //FlagNo���ֽ���������λ��
    BYTE *pFlag=(BYTE *)&m_Flags;//
	
    ByteNo = FlagNo>>3;
	BitNo  = FlagNo&0x7;

	switch(Status)
	{
	case FLAG_ON:
		pFlag[ByteNo] |= BIT8_L1[BitNo];
		break;
	case FLAG_OFF:
		pFlag[ByteNo] &= ~BIT8_L1[BitNo];
		break;
	case FLAG_SWAP:
		pFlag[ByteNo] ^= BIT8_L1[BitNo];
		break;
	}
}


//========================================================================================================
//  ��������: GetFlag
//  �������: pFlags��m_Flags��������ַ  FlagNo����Ǻ� 
//  ��������: ��ȡm_Flags�������ڵĵ�FlagNo��λ�õ�״̬λ
//  ����ֵ��  0: ��־λΪ��   ��0����־λ��Ϊ��
//========================================================================================================
DWORD CProtocol::GetFlag(DWORD FlagNo)
{// ��ȡÿ��ģ��ı�־
	DWORD ByteNo; //FlagNo�������ֽں�
	DWORD BitNo;  //FlagNo���ֽ���������λ��
    BYTE *pFlag=(BYTE *)&m_Flags;//

    ByteNo = FlagNo>>3;
	BitNo  = FlagNo&7;

	return pFlag[ByteNo] & BIT8_L1[BitNo];
}



//========================================================================================================
//  ��������: SetFlagByte
//  �������: pFlags��m_Flags��������ַ  ByteNo���ֽں� Status ����ǵ�״̬
//  ��������: ��Flags�������ڵĵ�ByteNo�ֽ��ó�Status�������������ñ�־
//  ����ֵ��  ��
//========================================================================================================

void CProtocol::SetFlagByte(DWORD ByteNo,BYTE Status)
{
    PBYTE pFlag=(PBYTE)&m_Flags;//
    pFlag[ByteNo] = Status;
}

//========================================================================================================
//  ��������: GetFlagByte
//  �������: pFlags��m_Flags��������ַ  ByteNo���ֽں� Status ����ǵ�״̬
//  ��������: ��Flags�������ڵĵ�ByteNo�ֽڶ��������ڿ����ж����ޱ�־λ
//  ����ֵ��  m_Flags�������ڵĵ�ByteNo�ֽ�
//========================================================================================================

BYTE CProtocol::GetFlagByte(DWORD ByteNo)
{
    PBYTE pFlag=(PBYTE)&m_Flags;//
	
    return pFlag[ByteNo];
}

//========================================================================================================
//  ��������: SetFlagWord
//  �������: pFlags��m_Flags��������ַ  WordNo����� Status ����ǵ�״̬
//  ��������: ��Flags�������ڵĵڣ�ord���ó�Status�������������ñ�־
//  ����ֵ��  ��
//========================================================================================================
void CProtocol::SetFlagWord (DWORD WordNo,WORD Status)
{
    BYTE *pFlag=(BYTE *)&m_Flags;//

    WordNo <<=1; 
    pFlag[WordNo]   = HIBYTE(Status);
    pFlag[WordNo+1] = LOBYTE(Status);
}

//========================================================================================================
//  ��������: GetFlagWord
//  �������: pFlags��m_Flags��������ַ  ByteNo���ֽں� Status ����ǵ�״̬
//  ��������: ��Flags�������ڵĵ�WordNo�ֶ��������ڿ����ж����ޱ�־λ
//  ����ֵ��  m_Flags�������ڵĵ�WordNo��
//========================================================================================================
WORD CProtocol::GetFlagWord(DWORD WordNo)
{
    BYTE *pFlag=(BYTE *)&m_Flags;//
	BYTE LO,HI;	
	if( WordNo >= MAX_TASKFLAGNO/sizeof(WORD) )//Խ��
	      return 0;
	WordNo <<=1;
    HI = pFlag[WordNo];
    LO = pFlag[WordNo+1];
    return MAKEWORD(LO,HI);
}

//========================================================================================================
//  ��������: SetFlagDWord
//  �������: pFlags��m_Flags��������ַ  DWordNo����� Status ����ǵ�״̬
//  ��������: ��Flags�������ڵĵ�D��ordNo˫���ó�Status�������������ñ�־
//  ����ֵ��  ��
//========================================================================================================
void CProtocol::SetFlagDWord (DWORD DWordNo,DWORD Status)
{
    BYTE *pFlag=(BYTE *)&m_Flags;

	if(DWordNo >= MAX_TASKFLAGNO/(sizeof(DWORD)) )//Խ��
      	return;
	
	DWordNo<<=2; //*=4;
	pFlag[DWordNo  ] = HHBYTE( Status );
    pFlag[DWordNo+1] = LHBYTE( Status );
	pFlag[DWordNo+2] = LHBYTE( Status );
    pFlag[DWordNo+3] = LLBYTE( Status );
}

//========================================================================================================
//  ��������: GetFlagDWord
//  �������: pFlags��m_Flags��������ַ  DWordNo���ֺ� Status ����ǵ�״̬
//  ��������: ��Flags�������ڵĵ�DWordNo�ֶ��������ڿ����ж����ޱ�־λ
//  ����ֵ��  m_Flags�������ڵĵ�WordNo��
//========================================================================================================
DWORD CProtocol::GetFlagDWord (DWORD DWordNo)
{
    BYTE *pFlag=(BYTE *)&m_Flags;//
	BYTE LL,LH,HL,HH;	
	
	if(DWordNo >= MAX_TASKFLAGNO/(sizeof(DWORD)) )//Խ��
		return 0;
    
	DWordNo<<=2;
	
	HH = pFlag[DWordNo];
	HL = pFlag[DWordNo+1];
	LH = pFlag[DWordNo+2];
	LL = pFlag[DWordNo+3];

    return MAKEDWORD(LL,LH,HL,HH);
}

BOOL  CProtocol::CheckClearFlag(DWORD FlagNo)//���___���
{
	if(GetFlag(FlagNo))
	{ 
		SetFlag(FlagNo,FLAG_OFF); 
		return TRUE;
	}
	return FALSE;
}

BOOL  CProtocol::RegisterCommStatus(BYTE Status)
{
	if(Status == COMM_STATUS_BREAK)
		SetFlag(SF_CommErr);
	else
		SetFlag(SF_CommErr,FLAG_OFF);
	
	DAE_SetCommStatus(m_wPortID, Status);
}

//========================================================================================================
//  ��������: LPC
//  �������: BeginCode: ����(0��0xFF)��Buf��ҪУ������ݵ���ʼ��ַ  num��ҪУ������ݵĸ���
//  ��������: ��׼LPCУ���㷨
//  ����ֵ��  LPCУ����
//========================================================================================================
BYTE LPC(BYTE BeginCode,F_PBYTE Buf,WORD num)
{
register DWORD i;
register BYTE Result = BeginCode;
 
for(i=0;i<num;i++)
   Result ^= Buf[i];
return Result;
}

BYTE CheckSum(BYTE BeginCode,F_PBYTE Buf,DWORD num)
{
register DWORD i;
register BYTE Result = 0;
 
for(i=0;i<num;i++)
   Result += Buf[i];
return Result;
}

// �Ƚ�����ʱ����Դ�С.time1>=time2�򷵻�TRUE
BOOL  IsTimeNew(TAbsTime time1,TAbsTime time2)
{
	if(time1.Hi > time2.Hi)
		return TRUE;
	if((time1.Hi == time2.Hi)&&(time1.Lo >= time2.Lo))
		return TRUE;
	return FALSE;
}

//zxw 20120413 add
TIniParaData* g_IniParaData;
DWORD        g_IniParaNum;

int chtohex(char c)
{
	int i = 0;
	if(c >='0' && c <= '9')
		i = (c - '0');
	else if((c >='a' && c <= 'f') )
		i = (c - 'a' + 10);
	else if(c >='A' && c <= 'F')
		i = (c - 'A' + 10);
	else
		i= 0;
	return i;
}

DWORD atoix(char* ParaValue)
{
	int ix = 0;
	char* ps = ParaValue;
	if((*ps == '0' && (*(ps+1) ==  'x' || *(ps+1) ==  'X' )))
	{
		ps+=2;
		while(*ps)
		{
			ix <<= 4;
			ix += chtohex(*ps);
			ps++;
		}
	}		
	else
		ix =  atoi(ParaValue);
	return ix;
}

//******************************************************/
//�������ƣ�InterpretIniWin����
//�������ܣ���ģ������ȡ���ļ��еñ���������ֵ
//�������壺void
//���ߣ�	������
//����ʱ�䣺2010-09-04
//�汾��	v1.0
//�޸�ʱ�䣺2010-09-04
//�޸��ߣ�	������
//�޸����ݣ�����//ע�͵�֧�֡����ӱ��������пո��֧��
//�޸ĺ�汾��v1.1
//�޸�ʱ�䣺2010-09-04
//�޸��ߣ�	������
//�޸����ݣ�����"TAB"֧��
//�޸ĺ�汾��v1.2
//*****************************************************/
int InterpretIniWin(char* m_szIniBuffer, int nFileLen)
{
	int itemNUm = 0;
	char *ps;
	char *psz;
	char *tmp;
	char SegName[INIPARA_MAX_SEGLENGTH];	//����

	DWORD Len = strlen(m_szIniBuffer);

	if (Len <=0 || Len > nFileLen)
	{
		return -1;
	}
	ps = m_szIniBuffer;
	psz = m_szIniBuffer;

	while(*ps && (ps - psz ) <= Len)
	{
		if(*ps == '[')								//�α�ʾ
		{
			ps++;
			while(*ps == 0x20) ps++;				//�����ո�
			int i = 0;
			while(*ps != ']')
			{
				if(i >= INIPARA_MAX_SEGLENGTH)
				{
					ReportMsg("����̫�� ���� û�� ] ����\n");
					break;
				}
				while(*ps == 0x20) ps++;				//�����ո�
				SegName[i++] = *ps++;
			}
			SegName[i] = 0;
			tmp = ps;
			while(!((*ps == 0x0d) && (*(ps+1) == 0x0a)) && *ps)
			{
				ps++;
				if (ps - tmp >INIPARA_MAX_LINELENGTH)
				{
					ReportMsg("error!  Line too long! 1 linelen = %d\n",ps - tmp);
					break;
				}
			}
			ps+=2;
		}
		else if(*ps == ';')							//ע��
		{
			tmp = ps;
			while(!((*ps == 0x0d) && (*(ps+1) == 0x0a))&& *ps)
			{
				ps++;
				if (ps - tmp >INIPARA_MAX_LINELENGTH)
				{
					ReportMsg("error!  Line too long! 2 linelen = %d\n",ps - tmp);
					break;
				}
			}
			ps+=2;
		}
		else if(*ps == '/'  && *(ps+1) == '/')							//"//"��ʼ��ע��
		{
			tmp = ps;
			while(!((*ps == 0x0d) && (*(ps+1) == 0x0a)) && *ps)
			{
				ps++;
				if (ps - tmp >INIPARA_MAX_LINELENGTH)
				{
					ReportMsg("error!  Line too long! 3 linelen = %d",ps - tmp);
					break;
				}
			}
			ps+=2;
		}
		else if(*ps == 0x20)							//�ո�ͷ
		{
			while(*ps ==  0x20 ) ps++;
		}
		else if(*ps == 0x09)							//TAB��ͷ
		{
			while(*ps ==  0x09 ) ps++;
		}

		else if(*ps ==0x0d && *(ps+1) == 0x0a)		//����
		{
			ps+=2;
		}
		else										//����
		{
			if(*ps > 127 || *ps < 0)
			{
				ReportMsg("�������зǷ��ַ�����\n");
			}
			char ParaName[INIPARA_MAX_SEGLENGTH];
			char ParaValue[INIPARA_MAX_VALUELENGTH];

			memset(ParaName,0,INIPARA_MAX_SEGLENGTH);
			memset(ParaValue,0,INIPARA_MAX_VALUELENGTH);

			int i = 0;
			while((*ps != 0x20 && *ps != '=' && *ps != 0x09 ))
			{
				if(i >= INIPARA_MAX_SEGLENGTH)
				{
					ReportMsg("�����ַ�̫������ParaName=%s\n",ParaName[i]);
					break;
				}
				ParaName[i++] = *ps++;		
			}
			ParaName[i] = '\0';
			while(*ps != '=')
			{
				while(*ps == 0x20 || *ps == 0x09) ps++;
			}

			if(*ps == '=')
				ps++;

			while(*ps == 0x20 || *ps == 0x09) ps++;

			i = 0;
			while(!(*ps == 0x0d && *(ps+1) == 0x0a) && *ps)
			{
				if(i >= INIPARA_MAX_VALUELENGTH)
				{
					ReportMsg("����ֵ�ַ�̫������\n");
					break;
				}
				ParaValue[i++] = *ps++;	
				while(*ps == 0x20 || *ps == 0x09) ps++;
			}
			ParaValue[i] = '\0';

			while(*ps == 0x20 || *ps == 0x09) ps++;
			tmp = ps;
			while(!((*ps == 0x0d) && (*(ps+1) == 0x0a)) && *ps)
			{
				ps++;
				if (ps - tmp >INIPARA_MAX_LINELENGTH)
				{
					ReportMsg("error!  Line too long! 4 linelen = %d ps=%s\n",ps - tmp,ps);
					break;
				}
			}
			ps += 2;


			if(g_IniParaData == NULL || g_IniParaNum == 0)
			{
				itemNUm ++;
			}
			else
			{
				strcpy(g_IniParaData[itemNUm].Section,SegName);
				strcpy(g_IniParaData[itemNUm].ParaName,ParaName);
				strcpy(g_IniParaData[itemNUm].ParaValue,ParaValue);
				itemNUm ++;

				if (itemNUm >= g_IniParaNum)
				{
					return itemNUm;
				}
			}
		}//end else

	}//end while
	return itemNUm;
}

int InitProtocolPara()
{
	DWORD 	nFileLen;
	char	*szIniBuffer=NULL;
	int 	IniParaNum;
	
	if (DAE_GetFileLength("protocol.ini",&nFileLen) != DB_ERR_OK)
		return -1;
	if(INIPARA_MAX_LENGTH < nFileLen)
	{
		ReportMsg("InitProtocolPara: File is too long!! nFileLen=%d\n",nFileLen);
		return -1;
	}
	
	szIniBuffer = new char[nFileLen+1];
	if (!szIniBuffer)
		return -1;
	
	memset(szIniBuffer,0,nFileLen+1);

	if (DAE_ReadFile("protocol.ini",(BYTE *)szIniBuffer,nFileLen) != DB_ERR_OK)
	{
		ReportMsg("InitProtocolPara: Read ini error!! nFileLen=%d\n",nFileLen);
		return -1;
	}

	g_IniParaNum = InterpretIniWin(szIniBuffer,nFileLen);
	
	if (g_IniParaNum <= 0)
	{
		ReportMsg("InitProtocolPara:  g_IniParaNum = %d\n",g_IniParaNum);
		return FALSE;
	}
	
	g_IniParaData = new TIniParaData[g_IniParaNum];
	if (!g_IniParaData)
	{
		return FALSE;
	}
	memset(g_IniParaData,0,sizeof(TIniParaData)*g_IniParaNum );

	IniParaNum = InterpretIniWin(szIniBuffer,nFileLen);
	if (g_IniParaNum != IniParaNum)
	{
		ReportMsg("InitProtocolPara:  IniParaNum(%d) != g_IniParaNum(%d)",IniParaNum,g_IniParaNum);
		return false;
	}
}

