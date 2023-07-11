//=======================================================================================
// ����: Siec104.cpp
// ����: S104��վ��Լ�Ĵ���ʵ��
// ��д������ 2011-12-25
// ��д��
//=======================================================================================
#include <S104gm.h>
#include "relayfun.h"

#ifdef INSTALL_SGM104 


//#define S104_DEBUG

extern BYTE g_pFileDownBuffer[MAX_FILE_LEN];

BEGIN_MESSAGE_MAP(CGm104,CProtocol)
	ON_SM_COMMIDLE(CGm104)
	ON_SM_TIMEOUT(CGm104)
	ON_SM_SOE(CGm104)
	ON_SM_BIBURST(CGm104)
	ON_SM_DIST(CGm104)
	ON_SM_RELAYEVENT(CGm104)
	ON_SM_BOSELECTECHO(CGm104)
	ON_SM_BOEXECECHO(CGm104)
	ON_SM_NOTIFYETHLINKED(CGm104)  //��̫����������
	ON_SM_NOTIFYETHCLOSED(CGm104)  //��̫���ر�����
END_MESSAGE_MAP()

BOOL  IsTimeNew(TAbsTime time1,TAbsTime time2);
void  MergeConfigWord(float *pSet);
void  SplitConfigWord(float *pSet);

//=========================================================================
// ��������: ʵ�ִ�վ104��Լ��Ӧ�ò���ȹ������� 
//=========================================================================
void SGm104_Task(DWORD *pdwApp)
{
	CGm104 *pS104=new CGm104(pdwApp);
	if(pS104->Init()!=TRUE)
		pS104->Exit((char *)"S104 Init Error");
    	pS104->Run();
}

BOOL CGm104::HaveNewReport(void)
{
	WORD wNum;

	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}
	
	if(m_nLeftActionReportNum > 0)
		return TRUE;
	
	if((DB_GetRelayAlarmReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
		return TRUE;

	if((DB_GetRelayStartReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
		return TRUE;

	if((DB_GetRelayActionReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
		return TRUE;

	return FALSE;
}

// ��δ��ȡ�ı�����ѡ�����ϵ�һ��
BOOL CGm104::SearchNewReport(TRelayReport &Report,BYTE &Type)
{
	TRelayAlarmEv 	tAlram;
	TRelayActionEv 	tAction;
	BOOL			bAlarmValid=FALSE;
	BOOL			bStartValid=FALSE;
	BOOL			bActionValid=FALSE;
	TAbsTime		tm1,tm2,tm3;
	WORD wNum,wReadPtr;

	tm1.Hi=tm2.Hi=tm3.Hi=0xFFFF;
	tm1.Lo=tm2.Lo=tm3.Lo=0xFFFFFFFF;
	
	// ���澯����
	if((DB_GetRelayAlarmReportNum(&wNum) == DB_ERR_OK )&&(wNum > m_nALMNoAckCnt))
	{
		DB_GetRelayAlarmReportRead(&wReadPtr);

		if(DB_ReadRelayAlarmReport(&tAlram,(wReadPtr+m_nALMNoAckCnt)%MAX_RALM_NUM) == DB_ERR_OK)
		{
			tm1.Lo = tAlram.tEvent.tAction.AbsTimeLo;
			tm1.Hi = tAlram.tEvent.tAction.AbsTimeHi;
			bAlarmValid = TRUE;
		}
		else
		{
			DB_IncRelayAlarmReportRead();
			LogError("CGm104::SearchNewReport",FILE_LINE,"Read alarm report error!");
		}
	}
	
	// ����������
	if((DB_GetRelayActionReportNum(&wNum) == DB_ERR_OK )&&(wNum > m_nACTNoAckCnt))
	{
		DB_GetRelayActionReportRead(&wReadPtr);
		if(DB_ReadRelayActionReport(&tAction,(wReadPtr+m_nACTNoAckCnt)%MAX_RACT_NUM) == DB_ERR_OK)
		{
			tm3.Lo = tAction.tEvent.tAction.AbsTimeLo;
			tm3.Hi = tAction.tEvent.tAction.AbsTimeHi;
			bActionValid = TRUE;
		}
		else
		{
			DB_IncRelayActionReportRead();
			LogError("CGm104::SearchNewReport",FILE_LINE,"Read action report error!");
		}
	}

	// ����Ч����ֱ�ӷ���
	if(!bAlarmValid&&!bStartValid&&!bActionValid)
		return FALSE;

	// �Ƚϸ澯�Ͷ���
	if(IsTimeNew(tm1,tm3))
	{
		Type = DB_EVENT_RACT;
		Report.tAction = tAction;
	}
	else
	{
		Type = DB_EVENT_RALM;
		Report.tAlram = tAlram;
	}
		

	return TRUE;
}

void CGm104::FreshOldAI()
{
	// ˢ�¾�ң��ֵ
#if(SYSTEM_TYPE == _SYS_KF1300_)
	for(int i=0;i<DB_GetRMNum();i++)
		DB_ReadRM(i,&m_pOldAI[i]);
#else	
	for(int i=0;i<DB_GetAINum();i++)
		DB_ReadSecondAI(i,&m_pOldAI[i]);
#endif	
	
}

BOOL CGm104::SearchChangedAI(WORD *pAINo,long *pAIValue, DWORD ChangedValue, WORD wControlWord)//���ұ仯ң��
{
	long 	NewAIValue;
	long 	OldAIValue;
	DWORD 	AIChangedValue=0;	
	WORD 	AINo,nAINum;
	WORD 	AICount = 0;//��¼���β��ҵ�ң�����
	DWORD	iPersent;

#if(SYSTEM_TYPE == _SYS_KF1300_)
	nAINum = DB_GetRMNum();
#else
	nAINum = DB_GetAINum();
#endif	
	
	while (AICount++ < nAINum )
	{	
        if( m_wChangeAIReadPtr >= nAINum)
			m_wChangeAIReadPtr =0;
		
		AINo = m_wChangeAIReadPtr++;
		
#if(SYSTEM_TYPE == _SYS_KF1300_)
        DB_ReadRM(AINo,&NewAIValue);
#else
		DB_ReadSecondAI(AINo,&NewAIValue);
#endif
		OldAIValue  = m_pOldAI[AINo];

		if(NewAIValue == OldAIValue)
			continue;
		
		if(NewAIValue > OldAIValue)
			AIChangedValue = NewAIValue - OldAIValue;
		else 
			AIChangedValue = OldAIValue - NewAIValue;
		
		if(wControlWord == CHGAI_ABS)
		{
			if( AIChangedValue >= ChangedValue)
			{
				*pAIValue = NewAIValue;
				*pAINo = AINo;
				return TRUE;
			}
		}
		else if(wControlWord == CHGAI_PER)
		{
			DWORD     divisor = 0;

            if(OldAIValue<0)
				divisor=OldAIValue* (-1);
			else
				divisor=OldAIValue;

			if(OldAIValue==0)
			{
				if(NewAIValue == 0)
					continue;

				*pAIValue = NewAIValue;
				*pAINo = AINo;
				return TRUE;

			}

			iPersent =	 (AIChangedValue * 1000 * 10)/ divisor;

			if(iPersent >= (ChangedValue * 10))
			{
				*pAIValue = NewAIValue;
			    *pAINo = AINo;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//=========================================================================
// ��������: CGm104::Init 
// ��������: ʵ��S104��Լ�ĳ�ʼ�� 
//=========================================================================
BOOL CGm104::Init(void)
{
	//��ʱ3�룬��ֹϵͳ��λ�󣬻�δˢ�����ݣ���Լ�����ⷢ��
	Sleep(3000);

	if(CProtocol::Init() != TRUE)
		return FALSE;

	m_Txd.ReadPtr=m_Txd.WritePtr=0;
	m_Txd.Buf = GetTxdBuf();
	pTxdFm = (TS104TxdFm *)m_Txd.Buf; 

	ResetVar();
	
	StartMsg(MCC_ReadComm, 6);  	//��Ҫͨ������Ϣ
	StartMsg(MCC_CommIdle,200); 	//����CommIdle��Ϣ   200ms

	// �����ң������
#if(SYSTEM_TYPE == _SYS_KF1300_)
	m_pOldAI = (long *)malloc(DB_GetRMNum()*sizeof(long));
#else
	m_pOldAI = (long *)malloc(DB_GetAINum()*sizeof(long));
#endif

	// ���涨ֵ����
	m_LocalSTNum = DB_GetSETNum();//��ֵ��������������Ҫ��ֳ�WORD
	m_ST.Buf = (BYTE *)malloc(sizeof(float)*(m_LocalSTNum+(DB_GetSWNum()+31)/32));

	//����������ʱ��
	SetTimer(TM_1S,1000);

	m_NoRxdCounter = MAX_COMMERR_TIME;
	m_dwTaskRunCount = 0;

	m_nLeftActionReportNum = 0;
	m_wChangeAIReadPtr = 0;

	SetFlag(SF_INIT_OVER);
	
	m_dwRxdCount = 0;
	m_nSOENoAckCnt = 0;
	m_nBIENoAckCnt = 0;
	m_nAIENoAckCnt = 0;
	m_nALMNoAckCnt = 0;
	m_nACTNoAckCnt = 0;

	memset(&m_DistDir,0,sizeof(TDistDirInfo));
	memset(&m_DistFile,0,sizeof(TDistFileInfo));

	m_wFaultNo = 0;

	m_S104para.intsetcode = 3;
	if(InitPara())
		ReportMsg("Port 0x%x: Init IEC104 para from iconfig! intsetcode=%d",m_wPortID,m_S104para.intsetcode);	
	
	return TRUE;
}

BOOL CGm104::InitPara()
{
	BOOL 	rtn = FALSE;
	WORD	value;
	char 	sec[30];
	
	if(g_IniParaNum <= 0 || g_IniParaNum > INIPARA_MAX_ITEMNUM)
	{
		return FALSE;
	}

	DB_GetPortName(m_wPortID,sec);

	for (int i = 0; i< g_IniParaNum; i++)
	{
		if(strcmp( g_IniParaData[i].Section, sec))
			continue;

		if(!strcmp( g_IniParaData[i].ParaName, "intsetcode"))
		{
			value = atoix(g_IniParaData[i].ParaValue);
			m_S104para.intsetcode = value;
			rtn = TRUE;
			break;
		}
	}

	return rtn;
}

//��ʼ������ȫ��׃��
BOOL CGm104::ResetVar(void)
{
	int i=0;
	
	BufWritePtr = 0;
	BufAckPtr = 0;

	m_dwTxdCount = 0;  //����֡��������
	m_dwRxdCount = 0;  //����֡��������

	// ������з��Ͷ�ʱ��
	for(i=0;i<MAX_TIMERNUM;i++)
	{
		MyKillTimer(i);
	}

	// ������з��ͻ�����
	for(i=0;i<IEC104_K;i++)
	{
		m_TxDataBuf[i].nTxdSeqNum = -1;
		m_TxDataBuf[i].DataLen = 0;
	}

	bIsSTARTDT = FALSE;

	m_nSOENoAckCnt = 0;
	m_nBIENoAckCnt = 0;
	m_nALMNoAckCnt = 0;
	m_nACTNoAckCnt = 0;
	m_nAIENoAckCnt = 0;
	
	//�����������־
	SetFlag(SF_BUFFLOWOVER,FLAG_OFF);

	return TRUE;
}

BOOL CGm104::MySetTimer(BYTE bTimerID, WORD wSecond)
{
	if(bTimerID > MAX_TIMERNUM || wSecond == 0)
		return FALSE;

	m_timer[bTimerID].bTimer = TRUE;
	m_timer[bTimerID].wTick  = wSecond;
	m_timer[bTimerID].wValue = wSecond;

	return TRUE;
}

BOOL CGm104::MyKillTimer(BYTE bTimerID)
{
	if(bTimerID >=MAX_TIMERNUM)
		return FALSE;

	m_timer[bTimerID].bTimer = FALSE;
	m_timer[bTimerID].wTick  = 0;
	m_timer[bTimerID].wValue = 0;

	return TRUE;
}

void CGm104::MyCheckTimer()
{
	for(int i = 0; i < MAX_TIMERNUM; i++)
	{
		if(m_timer[i].bTimer)
		{
			m_timer[i].wValue --;

			if(m_timer[i].wValue == 0)
			{
				m_timer[i].wValue = m_timer[i].wTick;
				MyOnTimer(i);
			}
		}
	}
}
void CGm104::MyOnTimer(BYTE bTimerID)
{	
	if(bTimerID<IEC104_K)
	{
		// һ֡��ʱ��ȫ�����
		ResetVar();
		CloseComm(m_wPortID);
		RegisterCommStatus(COMM_STATUS_BREAK);
#ifdef S104_DEBUG			 
		ReportMsg("S104: T1 Timeout!--TxdPtr=%d,TxdCount=%d!",bTimerID,m_dwTxdCount);
#endif
		return;
	}
	
	switch(bTimerID)
	{
	case BO_TIMERID:
		MyKillTimer(BO_TIMERID);
		m_BO.RetResult = 1;
		Txd68_2E2F3B3C(7);		
		SetFlag(SF_BOEND);
		break;
		
	case T2_TIMERID:
		//����S֡
		MyKillTimer(T2_TIMERID);
		pTxdFm->Start	= STARTHEAD;
		pTxdFm->Length =  4;
		pTxdFm->Control1 = 1;
		pTxdFm->Control2 = 0;
		pTxdFm->Control3 = LOBYTE(m_dwRxdCount<<1);
		pTxdFm->Control4 = HIBYTE(m_dwRxdCount<<1);
		m_Txd.WritePtr=6;
		WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);
		return;
		
	case T3_TIMERID:
		MySetTimer(T1_TIMERID, T1);
		//����U֡
		pTxdFm->Start	= STARTHEAD;
		pTxdFm->Length =  4;
		pTxdFm->Control1 = TESTFR_ACT;
		pTxdFm->Control2 = 0;
		pTxdFm->Control3 = 0;
		pTxdFm->Control4 = 0;
		m_Txd.WritePtr=6;
		WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);
		return;
		
	case T1_TIMERID:
		ResetVar();
		CloseComm(m_wPortID);
		RegisterCommStatus(COMM_STATUS_BREAK);
#ifdef S104_DEBUG			 
		ReportMsg("S104: TEST_FRM T1 Timeout!");
#endif
		break;
		
	default:
		break;
	}
		
}


//=========================================================================
// ��������: ��������֡�ƽ�����������   
// �������: ��
// ����ֵ:   ��
//=========================================================================
BOOL CGm104::SaveDataBuf() 
{
	//û�п�������޷�����
	if(GetFlag(SF_BUFFLOWOVER))
    	return FALSE; 
	
	//�������
	m_TxDataBuf[BufWritePtr].nTxdSeqNum = m_dwTxdCount;//���淢�����
	m_TxDataBuf[BufWritePtr].DataLen = m_Txd.WritePtr;   //���淢��ָ��
	memcpy(m_TxDataBuf[BufWritePtr].DataBuf,m_Txd.Buf, m_Txd.WritePtr);

	MySetTimer(BufWritePtr, T1);

	//����дָ��
   	BufWritePtr = (BufWritePtr+1)%IEC104_K;
	
	//�����Ӧ��ָ�룬����
/*
					   W T		
					   | |
	|-------------------------------------------|
	|-|-|-|-|-|-|-|-|-| |-|-|-|-|-|-|-|-|-|-|-|-|
	|-------------------------------------------|
*/
	if((BufWritePtr+1)%IEC104_K == BufAckPtr)	
		SetFlag(SF_BUFFLOWOVER);
	
#ifdef S104_DEBUG			 
	ReportMsg("S104: Send I_FRAME.	Send=%d, Ack=%d (%d->%d)",m_dwTxdCount,m_dwRxdCount,BufAckPtr,BufWritePtr);
#endif

	return TRUE;
}

void CGm104::RetryFrame(BYTE bItem)  
{                                       
	m_Txd.WritePtr=m_Txd.ReadPtr=0;
	memcpy(m_Txd.Buf,m_TxDataBuf[bItem].DataBuf,m_TxDataBuf[bItem].DataLen);
	m_Txd.WritePtr =  m_TxDataBuf[bItem].DataLen;//��¼����������
 
	WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);
}

BOOL CGm104::KillPrevFrame(WORD dwTxdNum)
{       
	int 	i,j;
	BYTE	NoAckNum,AckNo;

	NoAckNum = (BufWritePtr+IEC104_K-BufAckPtr)%IEC104_K;
	
	//�Ӵ�Ӧ��ָ�뿪ʼ����
	for(i=0;i<NoAckNum; i++)
    {
    	if(m_TxDataBuf[(BufAckPtr+i)%IEC104_K].nTxdSeqNum == dwTxdNum)//�ҵ��������
    		break;
    }

	//��������ǰдָ����δ�ҵ�
	/*
						   R W		
						   | |
		|-------------------------------------------|
		|-|-|-|-|-|-|-|-|-|-| |-|-|-|-|-|-|-|-|-|-|-|
		|-------------------------------------------|
	*/
	if(i==NoAckNum)
		return FALSE;

	//�ҵ�����֡
	for(j=0;j<=i;j++)
	{
		AckNo = (BufAckPtr+j)%IEC104_K;
		m_TxDataBuf[AckNo].nTxdSeqNum = -1;//�ÿձ�־
		m_TxDataBuf[AckNo].DataLen = 0;
		MyKillTimer(AckNo);
	}

	//��������һ��δӦ�����
	BufAckPtr = (BufAckPtr+i+1)%IEC104_K;

	//�����������־
	SetFlag(SF_BUFFLOWOVER,FLAG_OFF);
		
	return TRUE;
}

BOOL CGm104::SetBO(TBO* pBO,BYTE NeedReturnFlag)//ң�����ú���
{
	BYTE Mode;
	WORD SwitchNo;

	//���浱ǰң����Ϣ
	//�����ң��Ԥ�ã�����ң����Ϣ���������Ԥ�ã�AttribΪ�ϴα����Attrib

	m_BO = *pBO; 	
	m_BO.NeedRetFlag = NeedReturnFlag;  //�Ƿ���ȴ�ң�ط�У
	//m_BO.Flag = PBO_BUSY;  //ң��æ��־

	if(m_BO.Attrib == BO_OPEN)
		SwitchNo = m_BO.No*2;
	else
		SwitchNo = m_BO.No*2+1;
		
	switch(m_BO.Command)
	{
	case PBO_SELECT:  	 DB_BOSelect( SwitchNo, BO_NULL);	        break;
	case PBO_EXECUTE:	 Mode=1;DB_BOExec( SwitchNo,BO_NULL,Mode);	break;
	case PBO_CANCEL:	 Mode=2;DB_BOExec( SwitchNo,BO_NULL,Mode);	break;
	case PBO_DIRECT:	 DB_BODirect(  SwitchNo, BO_NULL);	        break;
	default:		
		break;
	}
	return TRUE;
}

BOOL CGm104::SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag)//ң�����ú���
{
	TBO BO;

	BO = m_BO;
	
	BO.No = BONo;
	BO.Command = Command;
	BO.Attrib = Attrib;
	BO.NeedRetFlag = NeedReturnFlag;
	
	return SetBO(&BO);	
}

BYTE CGm104::CheckSum(BYTE * pBuf,DWORD dwLength)
{
	BYTE bySum=0;

	for(DWORD i=0;i<dwLength;i++)
		bySum+=pBuf[i];

	return bySum;
}

BOOL CGm104::DistFileName2Time(char *strFileName,TSysTime *pTime)
{
	if(pTime == NULL)
		return FALSE;
		
	// ת����ϵͳʱ��
	pTime->Year = (strFileName[0]-'0')*10+(strFileName[1]-'0');
	pTime->Month = (strFileName[2]-'0')*10+(strFileName[3]-'0');
	pTime->Day = (strFileName[4]-'0')*10+(strFileName[5]-'0');
	pTime->Hour = (strFileName[6]-'0')*10+(strFileName[7]-'0');
	pTime->Minute = (strFileName[8]-'0')*10+(strFileName[9]-'0');
	pTime->Second = (strFileName[10]-'0')*10+(strFileName[11]-'0');
	pTime->MSecond = (strFileName[12]-'0')*100+(strFileName[13]-'0')*10+(strFileName[14]-'0');
		
	return TRUE;
}

// ��ʼ��¼��Ŀ¼��
BOOL CGm104::InitDistDir()
{
	BYTE			*pFileData;
	char			*pFileName;
	BYTE			nHeadPtr,nTailPtr,nMax,nCnt;
	TDistIndex		*pDistList;
	DWORD			dwFileLength,dwRtnLength;
	STATUS			rc;

	pFileData = (BYTE *)g_pFileDownBuffer;
	
	// 1����¼�������ļ�
	dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*DIST_CAPACITY_MAX;
	memset(pFileData,0x00,dwFileLength);
	
	rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwFileLength != dwRtnLength))
	{
		// �����ļ�����
		LogError("Seic104::IinitDistDir",FILE_LINE,"\n Error on open DistList.ind!");
		return FALSE;
	}
			
	// 2����ȡ¼���ļ�
	pDistList = (TDistIndex *)pFileData;
	pFileData += sizeof(TDistIndex);

	nMax = pDistList->nMax;
	nTailPtr = pDistList->nTailIndex;
	// ���дָ���Ӧ��λ���ļ���Ч����дָ�뿪ʼ��ȡ	
	pFileName = (char *)pFileData+nTailPtr*DIST_INDEX_NAMELEN;
	if(strstr(pFileName,"INDEX_") != NULL)
		nHeadPtr = nTailPtr;
	else
		nHeadPtr = pDistList->nHeadIndex;

	nCnt = 0;
	// ���ζ�ȡ¼���ļ�
	do
	{
		pFileName = (char *)pFileData+nHeadPtr*DIST_INDEX_NAMELEN;

		// ֻ����Ϸ��ļ���
		if(pFileName = strstr(pFileName,"INDEX_"))
		{
			pFileName += 6;
			
			// ��¼�ļ���
			memcpy(m_DistDir.strFileNameList[nCnt],pFileName,DIST_INDEX_NAMELEN);

			// ��¼�ļ�ʱ��
			DistFileName2Time(pFileName,&m_DistDir.tFileTimeList[nCnt]);
			
			nCnt++;
		}
		
		nHeadPtr = (nHeadPtr+1)%nMax;
	}while(nHeadPtr != nTailPtr);

	m_DistDir.nCurDirNo = 0;
	m_DistDir.nSentCnt = 0;
	m_DistDir.nFileNum = nCnt;
	m_DistDir.bStatus |= S104_FILESATUS_INITDIR;

	return TRUE;
}

// ��ʼ��¼���ļ���Ϣ
BOOL CGm104::InitDistFile(BYTE nFileName)
{
	BYTE	nFileNo = (nFileName>>4)&0x0F;
	char 	*strFileName;
	char	strFileALLName[DIST_INDEX_NAMELEN];
	DWORD	dwCfgFlieLen,dwDataFileLen,dwRtnLength;
	STATUS			rc;

	m_DistDir.bStatus &= (~S104_FILESATUS_INITFILE);
	
	// ��ʼ��Ŀ¼
	m_DistFile.nSubDir = nFileNo;	// �ļ�����		

	if(!(m_DistDir.bStatus&S104_FILESATUS_INITDIR))
	{
		LogError("InitDistFile",FILE_LINE," Call one dir before init dir!!!");
		return FALSE;
	}
	
	if(nFileNo >= m_DistDir.nFileNum)
	{
		LogError("InitDistFile",FILE_LINE," File no is overflow!!!");
		return FALSE;
	}
	
	
	strFileName = m_DistDir.strFileNameList[nFileNo];

	// ��ȡ�����ļ�����
	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".CFG");
	if(!FS_GetFileLength(strFileALLName,&dwCfgFlieLen))
		return FALSE;

	// ��ȡ�����ļ�����
	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".DAT");
	if(!FS_GetFileLength(strFileALLName,&dwDataFileLen))
		return FALSE;

	// �����ļ�������
	if(m_DistFile.pFileBuf != NULL)
		OS_MemFree(m_DistFile.pFileBuf);
		
	m_DistFile.pFileBuf = (BYTE *)OS_MemAlloc(dwCfgFlieLen+dwDataFileLen);
	if( m_DistFile.pFileBuf == NULL )
	{
		ReportMsg("CGm104::IinitDistFile() File %s buffer malloc error!",strFileName);
		return FALSE;
	}

	// ��ȡ�ļ�
	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".CFG");
	rc = FS_ReadFile(strFileALLName,0,dwCfgFlieLen,m_DistFile.pFileBuf,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwCfgFlieLen != dwRtnLength))
	{
		// �����ļ�����
		LogError("Seic104::InitDistFile",FILE_LINE,"\n Error on open file %s.CFG!",strFileALLName);
		return FALSE;
	}

	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".DAT");
	rc = FS_ReadFile(strFileALLName,0,dwDataFileLen,m_DistFile.pFileBuf+dwCfgFlieLen,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwDataFileLen != dwRtnLength))
	{
		// �����ļ�����
		LogError("Seic104::InitDistFile",FILE_LINE,"\n Error on open file %s.DAT!",strFileALLName);
		return FALSE;
	}
	
	m_DistFile.pFileStart[0] = m_DistFile.pFileBuf;
	m_DistFile.pFileStart[1] = m_DistFile.pFileBuf+dwCfgFlieLen;

	m_DistFile.dwDataLen[0] = dwCfgFlieLen;
	m_DistFile.dwDataLen[1] = dwDataFileLen;
//	m_DistFile.dwDataLen[1] = PERSECDATALEN*2+100;

	m_DistFile.bCheck[0] = CheckSum(m_DistFile.pFileStart[0],dwCfgFlieLen);
	m_DistFile.bCheck[1] = CheckSum(m_DistFile.pFileStart[1],dwDataFileLen);
//	m_DistFile.bCheck[1] = CheckSum(m_DistFile.pFileStart[1],PERSECDATALEN*2+100);

	m_DistFile.nSecName = 0;;	// ������
	m_DistFile.nSegNo = 0; 	// �κ�
	m_DistDir.bStatus |= S104_FILESATUS_INITFILE;
	m_DistDir.bStatus &= (~S104_FILESATUS_SELFILE);
	m_DistDir.bStatus &= (~S104_FILESATUS_SELSEC);

	return TRUE;
}

BOOL CGm104::SelectFile(BYTE nFileName)
{
	BYTE Type = (nFileName&0x0F);
	
	m_DistFile.nFileName = nFileName;
	
	if(!(m_DistDir.bStatus&S104_FILESATUS_INITFILE))
	{
		LogError("SelectFile",FILE_LINE,"Select file before file init!!!");
		return FALSE;
	}
	
	if((Type != 1)&&(Type != 2))
	{
		LogError("SelectFile",FILE_LINE,"Select file no is error!!!");
		return FALSE;
	}
	
	m_DistFile.nSecName = 0;
	m_DistFile.nSegNo = 0;
	
	if(Type == 1)//CFG
	{
		m_DistFile.nFileNo = 0;
		m_DistFile.dwFileLen = m_DistFile.dwDataLen[0];
		m_DistFile.bFileChs = m_DistFile.bCheck[0];
	}
	else//dat
	{
		m_DistFile.nFileNo = 1;
		m_DistFile.dwFileLen = m_DistFile.dwDataLen[1];
		m_DistFile.bFileChs = m_DistFile.bCheck[1];
	}

	// �����ܽ���
	m_DistFile.nSecNum = (m_DistFile.dwFileLen+PERSECDATALEN-1)/PERSECDATALEN;
	m_DistDir.bStatus |= S104_FILESATUS_SELFILE;
	m_DistDir.bStatus &= (~S104_FILESATUS_SELSEC);

	return TRUE;
}

BOOL CGm104::SelectSec(BYTE nSecName)
{
	if(!(m_DistDir.bStatus&S104_FILESATUS_SELFILE))
	{
		LogError("SelectFile",FILE_LINE,"Select sec before file select!!!");
		return FALSE;
	}
	
	if(nSecName >= m_DistFile.nSecNum)
	{
		LogError("SelectSec",FILE_LINE,"Select sec no is overflow!!!");
		return FALSE;
	}
	
	m_DistFile.nSecName = nSecName;
	m_DistFile.nSegNo = 0;

	if(nSecName < (m_DistFile.nSecNum-1))
		m_DistFile.dwSecLen = PERSECDATALEN;
	else
		m_DistFile.dwSecLen = m_DistFile.dwFileLen-((m_DistFile.nSecNum-1)*PERSECDATALEN);
	
	m_DistFile.nSegNum = (m_DistFile.dwSecLen+PERSEGDATALEN-1)/PERSEGDATALEN;
	
	// �����ܽ���
	m_DistFile.pSecStart = m_DistFile.pFileStart[m_DistFile.nFileNo]+nSecName*PERSECDATALEN;
	m_DistFile.bSecChs = CheckSum(m_DistFile.pSecStart,m_DistFile.dwSecLen);

	m_DistDir.bStatus |= S104_FILESATUS_SELSEC;

	return TRUE;
}

void CGm104::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	TxdMonitor();
}

void CGm104::OnTimeOut(DWORD dwTimerID)
{
	//��ʱ������	
	m_dwTaskRunCount++;

	// ��ʱ������
	MyCheckTimer();
        	
//	if((m_dwTaskRunCount%3) == 0)
//		SetFlag(SF_SendChangedAI);

	if(m_NoRxdCounter > 0)
	{
		// ����ʱ
		m_NoRxdCounter--;

		// ͨ��ʧ��ʱ����
		if(m_NoRxdCounter == 0)
		{
			RegisterCommStatus(COMM_STATUS_BREAK);

			bIsSTARTDT = FALSE;
			
			// ���SOE����
			m_nSOENoAckCnt = 0;
			m_nBIENoAckCnt = 0;
			m_nALMNoAckCnt = 0;
			m_nACTNoAckCnt = 0;
			m_nAIENoAckCnt = 0;
		}
	}

	// ����©����SOE
	if(GetNewBieNum() > 0)
		SetFlag(SF_HAVEBIE);
	
	// ����©���±���
	if(HaveNewReport())
		SetFlag(SF_HAVEREPORT);

	// ����©����SOE
	if( GetNewSoeNum() > 0 )
		SetFlag(SF_HAVESOE);

	return;
}

void CGm104::OnBIBurst()
{
	if(GetNewBieNum() > 0)
		SetFlag(SF_HAVEBIE);
}

void CGm104::OnSOE()
{	
	if(GetNewSoeNum() > 0)
		SetFlag(SF_HAVESOE);
}

void CGm104::OnRelayEvent(DWORD MsgCode, DWORD wParam)
{	
	if(HaveNewReport())
		SetFlag(SF_HAVEREPORT);
}

//ң��Ԥ�÷�У��Ϣӳ��
void CGm104::OnBOSelectEcho(WORD switchno,WORD attrib,WORD result)	
{
	m_BO.RetResult = result;

	Txd68_2E2F3B3C(7);
}

void CGm104::OnBOExecEcho(WORD switchno,WORD attrib,WORD result)	
{	
	m_BO.RetResult = result;

	if(m_BO.Command == PBO_EXECUTE)
		Txd68_2E2F3B3C(7);
	else
		Txd68_2E2F3B3C(9);

	SetFlag(SF_BOEND);
}

void CGm104::OnEthLinked(DWORD dwPara)
{
	TSysTime time;

	ReadSystemTime(&time);
	
	ResetVar();
#ifdef S104_DEBUG
	LogError("OnEthLinked",FILE_LINE,"Link accept! Port=%x Time=%d:%d.%d",m_wPortID,time.Minute,time.Second,time.MSecond);
#endif
}

void CGm104::OnEthClosed(DWORD dwPara)
{
	TSysTime time;

	ReadSystemTime(&time);
	
	ResetVar();
	RegisterCommStatus(COMM_STATUS_BREAK);
#ifdef S104_DEBUG
	LogError("OnEthClosed",FILE_LINE,"Link break! Port=%x Time=%d:%d.%d",m_wPortID,time.Minute,time.Second,time.MSecond);
#endif
}

void CGm104::OnDisturb(DWORD FileNo)
{
	m_DistDir.dwInfoAddr = DB_GetAddress();
	InitDistDir();
	SetFlag(SF_SENDDIR);
}

//==============================================
//  ��������: ���մ���
//==============================================
void CGm104::OnFrame(BYTE* pBuf)
{
	//����ͨ��״̬
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	
	m_NoRxdCounter = MAX_COMMERR_TIME;

	MySetTimer(T3_TIMERID,T3); //ȡ��T3��ʱ��

    // �������
    pRxdFm=(TS104RxdFm *)pBuf;
	
	MyKillTimer(T1_TIMERID); 	//��ֹT1��ʱ��
    MySetTimer(T3_TIMERID,T3); 	//��λT3��ʱ��
    
    RxdAPCIInfo(); //���տ�����
    
    return;
}


//=========================================================================
// ��������: ���Buf���Ƿ���һ����Ч����
// �������: Buf:Ҫ���ı�����ʼ��ַ
//           Len:Buf����Ч����
// ����ֵ: 
//          FM_OK +ʵ�ʱ��ĳ��� //��⵽һ��������֡
//          FM_ERR +�Ѵ����ֽ��� //��⵽һ��У������֡
//          FM_LESS+�Ѵ����ֽ��� //��⵽һ����������֡����δ���룩
// ע:�Ѵ����ֽ�����Ϊ0���´���������һ��ַ��ʼ,�����´������������Ѵ����
//      ���ֽ�,�Ӷ��ӿ������ٶ�
//=========================================================================
DWORD CGm104::SearchOneFrame(PBYTE Buf,WORD Len)
{
    unsigned short FrameLen;

    pRxdFm=(TS104RxdFm *)Buf;
	
    if(pRxdFm->Start != STARTHEAD)
	    return FM_ERR|1;	 
	
    FrameLen=pRxdFm->Length; //֡����

    if(FrameLen+2 > Len) //���ջ��������ֽ���С��֡���ȣ���������
		return FM_LESS;
    
    FrameLen += 2; //�����֡���ĳ���
		 
    if(FrameLen > MAX_RXDFM_SIZE) //�������ݴ������֡��
		return FM_ERR|1;
    
    if(FrameLen < MIN_RXDFM_SIZE) //��������С����С֡��
		return FM_LESS;

	if(FrameLen > 6)		 		
	   if(pRxdFm->Address_L != DB_GetAddress())
			return FM_ERR|FrameLen;
	   
    return FM_OK|FrameLen; //������֡����
}

BOOL CGm104::Handle_I_Format() //����I��ʽ
{  
	WORD dwAckCount=0;                                   

	m_dwRxdCount = (MAKEWORD(pRxdFm->Control1,pRxdFm->Control2)>>1);
	dwAckCount = MAKEWORD(pRxdFm->Control3,pRxdFm->Control4)>>1;
	
#ifdef S104_DEBUG			 
	ReportMsg("S104:    Recv I_FRAME--Send=%d ACK=%d!",m_dwRxdCount,dwAckCount);
#endif	

	ClearSentEvent(dwAckCount);

	// ���֮ǰ����֡
	dwAckCount = (dwAckCount+MAXSEQUENCER-1)%MAXSEQUENCER;
	KillPrevFrame(dwAckCount);

	// ������һ֡���͵�ACK
	m_dwRxdCount = (m_dwRxdCount+1)%MAXSEQUENCER;

	// ����T2��ʱ��	
	MySetTimer(T2_TIMERID, T2);

	// ������ͻ������������ٴ���
	if(GetFlag(SF_BUFFLOWOVER))
		return FALSE;		
	
    return Rxd68();
}

BOOL CGm104::Handle_U_Format() //����U��ʽ
{
	BYTE Control = 0;
	
	switch(pRxdFm->Control1)
	{
		case STARTDT_ACT: //֡��������
			 Control |= STARTDT_CON; 
			 ResetVar();
			 MySetTimer(T3_TIMERID,T3);
			 bIsSTARTDT = TRUE;
#ifdef S104_DEBUG			 
			 ReportMsg("S104:    Recv U_FRAME--STARTDT!");
#endif			 
			 break;
		case STOPDT_ACT:
			 Control |= STOPDT_CON;
			 ResetVar();
			 MyKillTimer(T3_TIMERID);
			 bIsSTARTDT = FALSE;
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv U_FRAME--STOPDT!");
#endif			 
			 break;
		case TESTFR_ACT:
			 Control |= TESTFR_CON;
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv U_FRAME--TESTFR!");
#endif			 
			break;
		
		default: 
			return FALSE;
	}
	
	//��U֡Ӧ��
	pTxdFm->Start= STARTHEAD;
	pTxdFm->Length =  4;
	pTxdFm->Control1 = Control;
	pTxdFm->Control2 = 0;
	pTxdFm->Control3 = 0;
	pTxdFm->Control4 = 0;
	m_Txd.WritePtr=6;
	WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);
	
	return TRUE;	
}

BOOL CGm104::Handle_S_Format() //����S��ʽ
{
	WORD dwAckCount;

	dwAckCount = MAKEWORD(pRxdFm->Control3,pRxdFm->Control4)>>1;
#ifdef S104_DEBUG			 
		ReportMsg("S104:    Recv S_FRAME--ACK=%d!",dwAckCount);
#endif

	ClearSentEvent(dwAckCount);

	// ���֮ǰ����֡
	dwAckCount = (dwAckCount+MAXSEQUENCER-1)%MAXSEQUENCER;
	KillPrevFrame(dwAckCount);
							
	// �յ����ظ���������ʱ
	TxdMonitor();
			
	return TRUE;
}

//���������
BOOL CGm104::RxdAPCIInfo()
{
	unsigned char Control;
	Control = pRxdFm->Control1;
        
	switch(Control&GETBIT2)
    { //�жϿ������ʽ
		case 0x3:Handle_U_Format();break; //����U��ʽ����
		case 0x1:Handle_S_Format();break; //����S��ʽ����
	    default: Handle_I_Format();break; //ȱʡ����I��ʽ����
    }  
	return TRUE;
}

BOOL CGm104::Rxd68(void)
{
	switch(pRxdFm->Style) //���ͱ�־��
	{
		case 0x2D:  break;//45 ������
		case 0x2E:  //46 ˫����
		case 0x2F:
		case 0x3B:
		case 0x3C:  Rxd68_2E2F3B3C();  break; //��ʱ���ң��,104��ԼASDU
		case 0x64:  Rxd68_64(); break;//���ٻ�/�ٻ�ĳһ������,100

		case 0x65:  Rxd68_65(); break;//�ٻ����,101
		case 0x66:  Rxd68_66(); break;//�����ݴ���,102
		case 0x67:  Rxd68_67(); break;//ʱ��ͬ��,103
		case 0x68:  Rxd68_68(); break; //������·,104

		case 0x69:  Rxd68_69(); break; //��λRTU,105
		case 0x6B:  Rxd68_6B(); break; //��ʱ��Ĳ�������
		case 0x6E:  Rxd68_6E(); break; //���ò���,110
		case 145:	Rxd_145(); break;	//���ͱ�ʶ145��ͨ�÷�������

		case 122:	Rxd_122(); break;	//�ļ����ͱ�ʶ122��F_SC_NA_1�ٻ�Ŀ¼, ѡ���ļ�, �ٻ��ļ�,�ٻ���
		case 124:	Rxd_124(); break;	//���ͱ�ʶ124��F_AF_NA_1 �Ͽ��ļ�, �Ͽɽ�(ͼ88)
	}

	// �յ����ظ���������ʱ
	TxdMonitor();
			
	return TRUE;
}

BOOL CGm104::Rxd68_68(void)//������·
{
	return Txd68_68();
}

BOOL CGm104::Rxd68_6B(void)//��ʱ�������·
{
	return Txd68_6B();
}

BOOL CGm104::Rxd68_6E(void)//���ò���
{
	return Txd68_6E();
}

BOOL CGm104::Rxd68_65_5(void)//�ٻ�ĳ����
{
	return Txd68_65_5();
}

BOOL CGm104::Rxd68_65_6(void)//�ٻ����е��
{
    WORD FlagNo;
	
	for(FlagNo=SF_CIGRP1; FlagNo<=SF_ALLCISTOP; FlagNo++)
		SetFlag(FlagNo);
	
	m_S104Info.TxdIdleTxdFm = TITF_CallAllCI;

	return Txd68_65_7();	
}

BOOL CGm104::Rxd68_65(void)//�ٻ����
{
	switch(pRxdFm->Reason_L)
	{
		case 5:  Rxd68_65_5(); break;  //�ٻ�ĳ����
        case 6:  Rxd68_65_6(); break;  //�ٻ����е��
	}
	return TRUE;
}

BOOL CGm104::Rxd68_67(void)//ʱ��ͬ��
{
	PBYTE pData=&pRxdFm->Data;
	TSysTime SysTime;
//	TAbsTime AbsTime;
	WORD MSecond;
	
	pData+=3;  //ָ��ʱ����Ϣ��
	MSecond = MAKEWORD(pData[0],pData[1]); //
	SysTime.MSecond = MSecond%1000;
	SysTime.Second  = MSecond/1000;
	SysTime.Minute = pData[2];
	SysTime.Hour = pData[3];
	SysTime.Day = pData[4]&0x1F;
	SysTime.Month = pData[5]&0x0F;
	SysTime.Year = (pData[6]&0x7F)+2000;

	//ToAbsTime(&SysTime, &AbsTime);
	//WriteAbsTime(&AbsTime);
	if(DAE_GetRunningFlags(SS_TIME_B))
		WriteSystemTime(&SysTime);

	Txd68_67();//���ӵ�ȷ��
	return TRUE;
}

BOOL CGm104::Rxd68_64(void)//���ٻ�/�ٻ�ĳһ������
{
	WORD  GroupNo;
	PBYTE Buf=(PBYTE)&(pRxdFm->Start);

	//QOI
	if(Buf[15]==0x14) //���ٻ�
	{
		Rxd68_64_14(); 
		return TRUE;
	}
	
	GroupNo=Buf[15]-0x15; //�ٻ�ĳһ������

	if(Buf[15]>=0x15 && Buf[15]<=0x1C)
	{
		GroupNo=Buf[15]-0x15;
		
		return Txd_BIGroup(GroupNo,GroupNo+0x15);
	}
	
	if(Buf[15]>=29 && Buf[15]<=36)
	{
		GroupNo=Buf[15]-21;
		
		return Txd_AIGroup(GroupNo,GroupNo+21);
	}

	return TRUE;
}


BOOL CGm104::Rxd68_64_14(void)//�������ٻ�����
{
	switch(pRxdFm->Reason_L)//����ԭ��
	{
	case 6:	//���ٻ�����
		Rxd68_64_14_6();

		FreshOldAI();
		
		break;

	case 8://ֹͣ����
		for(WORD SFlag=SF_BIGRP1;SFlag<=SF_ALLSTOP;SFlag++)
			SetFlag(SFlag,FLAG_OFF);
		break;
	}
	
	return TRUE;
}

BOOL CGm104::Rxd68_64_14_6(void)//�������ٻ���������
{
	m_S104Info.TxdIdleTxdFm = TITF_CallAll;

	//����ģʽ���ٻ�ȡ��
	if(!GetMaintSWStatus())
		for(WORD SFlag=SF_BIGRP1;SFlag<=SF_ALLSTOP;SFlag++)
			SetFlag(SFlag);
	else
		SetFlag(SF_ALLSTOP);
	
	return Txd68_64_14_6();
}

BOOL CGm104::Rxd68_66(void)//�����ݴ���
{
   PBYTE pData;
   
   SetFlag(SF_CallData);   //��ǰ�����ٻ�����״̬
   pData=&pRxdFm->Data;
   m_S104Info.CallDataAddr=MAKEWORD(pData[0],pData[1]);

   return TRUE;
}

BOOL CGm104::Rxd68_2E2F3B3C(void)//ң�ش���
{  
	switch(pRxdFm->Reason_L)//����ԭ��
	{
		case 6:	Rxd68_2E2F3B3C_6(); return TRUE;//ң��Ԥ��/ִ�д���
		case 8: Rxd68_2E2F3B3C_8(); return TRUE;//ֹͣ����
	}
	return TRUE;
}

BOOL CGm104::Rxd68_2E2F3B3C_6(void)//ң��Ԥ��/ִ������
{
	PBYTE 	pData=&pRxdFm->Data;
	WORD  	DCO;  //ң�������޶���
	WORD  	BONo; //ң��·��
	DWORD 	Command;
	WORD 	Attrib;
	TBO		BO;
	
	BONo=MAKEWORD(pData[0],pData[1]);
	DCO=pData[3];

	switch(DCO&0x03)
	{
	case 0: case 3: return FALSE;
	case 1: Attrib=BO_OPEN;  break; //��
	case 2: Attrib=BO_CLOSE; break; //��
	}

	switch(pRxdFm->Style)
	{
	case 0x2E:  BONo-=BO_ADDR_START;  Is101BO = TRUE;break;
	case 0x2F:  BONo-=AO_ADDR_START;  Is101BO = TRUE;break;
	case 0x3B:  BONo-=BO_ADDR_START;  Is101BO = FALSE;break;
	case 0x3C:  BONo-=AO_ADDR_START;  Is101BO = FALSE;break;
	}

	BO.Info[0]=pRxdFm->Style;
	BO.Info[1]=pData[0];
	BO.Info[2]=pData[1];
	BO.Info[3]=pData[2];
	BO.Info[4]=pData[3];
	
	BO.No=BONo;
	BO.Attrib=Attrib;

	switch(DCO&0x80)
	{
	case 0:	//ִ��
		Command=PBO_EXECUTE; 
#ifdef S104_DEBUG
		LogError("Rxd68_2E2F3B3C_6",FILE_LINE,"EXUTE Command = %d  No= %d  Attrib= %d\n",Command,BO.No,BO.Attrib);
#endif
		break;

	case 0x80:	
		Command=PBO_SELECT;
#ifdef S104_DEBUG
		LogError("Rxd68_2E2F3B3C_6",FILE_LINE,"SELECT Command = %d  No= %d  Attrib= %d\n",Command,BO.No,BO.Attrib);
#endif
		break;
	}

	BO.Command=Command; 

	SetBO(&BO);
	
	return TRUE;
}

BOOL CGm104::Rxd68_2E2F3B3C_8(void)//ң�س�������
{   
	SetBO(PBO_CANCEL);
}

//=======================================================================================
// ��������: Rxd68_69
// ����������void 
// �������أ�bool ������ȷ��1�ǣ�0����
// ��������: ����״̬��λָ�� 
// ������д�������� 2004-06-23
// ������д��
//=======================================================================================
BOOL CGm104::Rxd68_69(void)
{
	Txd68_69_Ack();

	//��λ֡����
	m_dwTxdCount = 0;  //����֡��������
    m_dwRxdCount = 0;  //����֡��������

	return TRUE;
}

//145 103��ͨ�÷������� type:21
//�ٻ���ֵ�����ٻ���ֵ
//=======================================================================================
// ��������: Rxd_145
// ����������void 
// �������أ�������ȷ��TRUE����FALSE
// ��������: �ٻ���ֵ�����ٻ���ֵ
// ������д��lz 2011-11-26
// ������д��
//=======================================================================================
BOOL CGm104::Rxd_145(void)
{
	PBYTE 			pData=&pRxdFm->Data;
	TGenData		*pGenData;
	BYTE 			bINF;

	bINF = pData[5];
	//ͨ��������ָ��
	pGenData = (TGenData *)&pData[7];

	switch(bINF)
	{
		case INF_M_GEN_RGROUPVALUE:	// 241   //��һ�����ȫ����Ŀ��ֵ������
			if(pGenData->Gin.byGroup == 0)
			{
				if(pGenData->Gin.byEntry == 1)
					Txd68_145_STSReadEcho();   //ȡ��ֵ����Ӧ�� 
			}
			else if(pGenData->Gin.byGroup > 0 && pGenData->Gin.byEntry == 0)
				Rxd68_145_STRead();   //ȡ��ֵӦ��
			break;

		case INF_M_GEN_WENTRYACK:		//249	��ȷ�ϵ�д��Ŀ
			m_ST.Command = 0;
		
			if( pGenData->Gin.byGroup == 00  && pGenData->Gin.byEntry == 0x01)			 
				Txd68_145_STSSelectEcho();	//�޸Ķ�ֵ����Ԥ�� 
			else 
				Rxd68_145_STSelect();		//�޸Ķ�ֵand��ѹ��Ԥ�� 
			break;
		
		case INF_M_GEN_WENTRYEXEC:		//250	��ִ�е�д��Ŀ
		case INF_M_GEN_WENTRYCANCEL:	//251	д��Ŀ��ֹ
			if( pGenData->Gin.byGroup == 00  && pGenData->Gin.byEntry == 0x01)			 
				Txd68_145_STSExecEcho();	//�޸ģ���ֵ���ţ�ִ��/����   
			else
				Txd68_145_STExecEcho(); 	//�޸Ķ�ִֵ��/����
			break;
		default:
			break;
	}		

	return TRUE;
}

//=======================================================================================
// ��������: Rxd68_145_2
// ����������void 
// �������أ�bool ������ȷ��1�ǣ�0����
// ��������: ȡ��ֵ
// ������д�������� 2004-06-23
// ������д��
//=======================================================================================
BOOL CGm104::Rxd68_145_STRead(void)
{
	PBYTE 		pData=&pRxdFm->Data;
	TGenData	*pGenData;
	BYTE		status;
	STATUS	 	rc=DB_ERR_OK;
	float		*pfValue;
	DWORD		*pdwSWValue;
	
	pGenData = (TGenData *)&pData[7];
	pfValue = (float *)m_ST.Buf;
	pdwSWValue = (DWORD *)m_ST.Buf;
	
	do{		
		// 0x00����ǰ��ֵ��		
		if(pGenData->byGid == 0x00)
			m_ST.Group = DB_GetCurrentSETAreaNo();
		else
			m_ST.Group = pGenData->byGid-1;
			
		m_ST.No = SET_VALUE_ALL;
		m_ST.Offset = 0;
		m_ST.Num = m_LocalSTNum+((DB_GetSWNum()+31)/32);
		memset(m_ST.Buf,0,m_ST.Num*4);

		// 1 �ȶ�ȡȫ����ѹ��״̬
		for(int i=0;i<DB_GetSWNum();i++)
		{
			rc |= DB_GetSWStatus(i, status);
		
			if(status == SW_STATUS_SET)
				pdwSWValue[i/32] |= (DWORD)1<<(i%32);
			else
				pdwSWValue[i/32] &= ~((DWORD)1<<(i%32));
		}

		if(rc != DB_ERR_OK)
		{
			m_ST.Result = 1;
			break;
		}
		
		// 2 ��ȡ��ֵ
		rc = DB_GetSETFloatValue(m_ST.Group, m_ST.No,pfValue+((DB_GetSWNum()+31)/32));
		if(rc != DB_ERR_OK)
		{
			m_ST.Result = 1;
			break;
		}
		
		// ���и���������ת
		for(int i=0; i<m_ST.Num; i++)
			pfValue[i] = _ConvertFloat(pfValue[i]);

		m_ST.Result = 0;
	}while(0);
	
	SetFlag(SF_STSend);
	
    return TRUE;
}

//=======================================================================================
// ��������: Rxd68_147_STSelect
// ����������void 
// �������أ�bool ������ȷ��1�ǣ�0����
// ��������: �����޸Ķ�ֵԤ��
// ������д��lz 2011-12-30
// ������д��
//=======================================================================================
BOOL CGm104::Rxd68_145_STSelect(void)
{
	PBYTE		pData=&pRxdFm->Data;
	TGenData	*pGenData = (TGenData *)&pData[7];;   //���Ӹ�ָ��ĳ�ʼ����yanzhh 2014��4��14�� 11:08:45
	BYTE 		Num	= pGenData->Ngd.Number;				//��֡��ֵ����
	PBYTE 		pdzData	= (PBYTE)&pGenData->Gin;		//��ֵԪ�ؿ�ʼ��ַ
	TGenItem	*pDZe;
	STATUS		rc=DB_ERR_OK;
	float		*pfValue = (float *)m_ST.Buf;

	pGenData = (TGenData *)&pData[7];
	
	if(pGenData->Gin.byEntry == 0)
	{
		if(pGenData->Gin.byGroup == 0xFF)
			m_ST.Group = DB_GetCurrentSETAreaNo();
		else
			m_ST.Group = pGenData->Gin.byGroup-1;
		
		m_ST.Num = m_LocalSTNum+((DB_GetSWNum()+31)/32);//����һ����ѹ��
		m_ST.Offset = 0;
	}
	else if(pGenData->Gin.byEntry != m_ST.Offset)
	{
		m_ST.Result = DB_ERR_NO;
		SetFlag(SF_STSelectEcho);
		return FALSE;
	}
	
	//�����䶨ֵ
	for(int i = 0; i < Num; i++)
	{
		pDZe = (TGenItem*)pdzData;
		memcpy(pfValue+(m_ST.Offset+i),&pDZe->byGid,4);
		pdzData += sizeof(TGenItem) + 4 - 1;
	}

	m_ST.Offset += Num;//��ǰ�յ��Ķ�ֵ����

	//û�к���֡
	if(pGenData->Ngd.Cont == 0)
	{
		SetFlag(SF_STSelectEcho);
		
		if(m_ST.Num != m_ST.Offset)
		{
			m_ST.Result = DB_ERR_NO;
			LogError("Rxd68_147_STSelect",FILE_LINE,"Select SET num error! Recv(%d) != %d\r\n\r\n ",m_ST.Offset,m_ST.Num);
			return FALSE;
		}

#ifdef S104_DEBUG			 
		LogError("Rxd68_147_STSelect",FILE_LINE,"Select SET! Group=%d\r\n\r\n ",m_ST.Group);
#endif
		// ���и���������ת
		for(int i=((DB_GetSWNum()+31)/32); i<m_ST.Num; i++)
			pfValue[i] = _ConvertFloat(pfValue[i]);

		// ��д��ѹ��
		rc = DB_SetAllSWStatus((BYTE *)pfValue);
		if(rc != DB_ERR_OK)
		{
			m_ST.Result = rc;
			LogError("Rxd68_147_STSelect",FILE_LINE,"Select SW error!");
			return FALSE;
		}

		// ��д�붨ֵ
		rc = DB_SetSETValue(m_ST.Group,SET_VALUE_ALL,pfValue+((DB_GetSWNum()+31)/32));
		if(rc != DB_ERR_OK)
		{
			m_ST.Result = rc;
			LogError("Rxd68_147_STSelect",FILE_LINE,"Select SET error!");
			return FALSE;
		}
	}
	
	return TRUE;
}

//�ٻ�Ŀ¼�����ļ�
BOOL CGm104::Rxd_122(void)
{
	PBYTE 	pData=&pRxdFm->Data;
	WORD	nFileName;
	BYTE	nSecName;

	m_DistDir.dwInfoAddr = MAKEDWORD(pData[0],pData[1],pData[2],0);
	
	nFileName = MAKEWORD(pData[3],pData[4]);
	nSecName = pData[5];
		
	if(pRxdFm->Reason_L == 5)	//�ٻ�Ŀ¼
	{
#ifdef S104_DEBUG			 
		ReportMsg("S104:    Rxd_122!! Call dir!!!");
#endif
		if((nFileName&0xFF) == 0xFF)
		{
			// ��ȫĿ¼
			InitDistDir();
			SetFlag(SF_CALLDIR);
		}
		else
		{
			// ������Ŀ¼
			if(InitDistFile(nFileName))
				Txd_126_RDSJ_DIR_SubDir();
			else
				Txd_120_RDSJ_F_FR(FALSE);
		}
		
	}
	else
	{
		switch(pData[6] & 0x0f) //
		{
		case 1: //ѡ���ļ�
			// ��ʼ��¼���ļ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Select file %x!!!",nFileName);
#endif
			if(SelectFile(nFileName))
				// �����ļ�����
				Txd_120_RDSJ_F_FR(TRUE);
			else
				// �����ļ�����
				Txd_120_RDSJ_F_FR(FALSE);
				
			break;
			
		case 2: //�����ļ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Call file %x!!!",nFileName);
#endif
			if(!(m_DistDir.bStatus&(S104_FILESATUS_SELFILE)))
			{
				LogError("Rxd_122",FILE_LINE,"S104:	Rxd_122!! Call file before select!!!");
				return FALSE;
			}
			
			// ѡ���һ��
			SelectSec(0);
			Txd_121_RDSJ_F_SR();
			break;
			
		case 5: //ѡ���
			// ��ʼ�����Ͷκ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Select sec %d!!!",nSecName);
#endif
			if(SelectSec(nSecName))
				Txd_121_RDSJ_F_SR();
			break;
			
		case 6: //�����
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Call sec %d!!!",nSecName);
#endif
			if(!(m_DistDir.bStatus&(S104_FILESATUS_SELSEC)))
			{
				LogError("Rxd_122",FILE_LINE,"S104:	Rxd_122!! Call sec before select!!!");
				return FALSE;
			}
			
			m_DistFile.nSecName = nSecName;
			m_DistFile.nSegNo = 0;
			SetFlag(SF_SENDSEC);
			break;
		
		case 3: //��ֹ�����ļ�
		case 4: //ɾ���ļ�
		case 7: //��ֹ�����
		default:
			break;			
		}
	}

	return TRUE;
}

BOOL CGm104::Rxd_124(void)	//ȷ���ļ�
{
	PBYTE 	pData=&pRxdFm->Data;
	WORD	nFileName;
	BYTE	nSecName;

	nFileName = MAKEWORD(pData[3],pData[4]);
	nSecName = pData[5];
	
	switch(pData[6] & 0x0f) //
	{
		case 1: //�ļ�����Ŀ϶��Ͽ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! File%x(%s) confirm OK!\r\n\r\n",nFileName,m_DistDir.strFileNameList[m_DistFile.nFileNo]);
#endif
			break;

		case 2: //�ļ�����ķ��Ͽ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! File%x(%s) confirm ERR!\r\n\r\n",nFileName,m_DistDir.strFileNameList[m_DistFile.nFileNo]);
#endif
			break;

		case 3: //�ڴ���Ŀ϶��Ͽ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! Sec%d confirm OK!\r\n\r\n",nSecName);
#endif
			// ���Ľ�
			if((m_DistFile.nSecName+1) == m_DistFile.nSecNum)
			{
				Txd_123_RDSJ_F_LS();
			}
			else
			{
				if(SelectSec(m_DistFile.nSecName+1))
					Txd_121_RDSJ_F_SR();
			}
			break;

		case 4: //�ڴ���ķ��Ͽ�
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! Sec%d confirm ERR!\r\n\r\n",nSecName);
#endif
			break;

		default:
			break;			
	}
	
	switch((pData[6] & 0xf0) >> 4)	//
	{
		case 1: //��������Ĵ洢�ռ�
			break;
		case 2: //У��ʹ�
			break;
		case 3: //����������ͨ�ŷ���
			break;
		case 4: //�����������ļ�����
			break;
		case 5: //���������Ľ�����
			break;
		default:
			break;			
	}
	
	return TRUE;
};

BOOL CGm104::TxdMonitor(void)
{
	if(!bIsSTARTDT)
	{
		return FALSE;
	}

	if(GetFlag(SF_BUFFLOWOVER))
	{
		return FALSE; //û�п�������޷�����
	}

	//��ʼ�����
	if(CheckClearFlag(SF_INIT_OVER))
	{
		if(Txd_InitOver())
			return TRUE;
	}

	//��ʼ�����
	if(CheckClearFlag(SF_BOEND))
	{
		return Txd68_2E2F3B3C(0x0A);
	}

	//����һ������ 
	if(Txd_Class1())
		return TRUE;

	//��������
	switch(m_S104Info.TxdIdleTxdFm)
	{
	case TITF_CallAll:   
		if(TxdCallAll())
		{
			return TRUE;  
		}
		break;//���ٻ�Ӧ��
	case TITF_CallAllCI: 
		if(TxdCallAllCI())
			return TRUE;  
		break;//���ٻ����Ӧ��
	default:	
		break;//���ٻ����Ӧ��
	}

	//���Ͷ���ֵ����
	if(GetFlag(SF_STSend))
	{
		if(Txd68_145_STReadEcho())	
			return TRUE;
	}

	//����Ԥ�ö�ֵ����
	if(CheckClearFlag(SF_STSelectEcho))
	{
		if(Txd68_145_STSelectEcho())	
			return TRUE;
	}

//===========================¼���ļ���ѯ Start========================
	//�ٻ�����Ŀ¼
	if(CheckClearFlag(SF_SENDDIR))
	{
		if(Txd_126_RDSJ_DIR_Auto())
			return TRUE;
	}
	
	//�ٻ�����Ŀ¼
	if(GetFlag(SF_CALLDIR))
	{
		if(Txd_126_RDSJ_DIR_AllDir())
			return TRUE;
	}

	//���Ͷ�
	if(GetFlag(SF_SENDSEC))
	{
		if(Txd_125_RDSJ_F_SG())
			return TRUE;
	}

	//���ĶΡ��� 
	if(GetFlag(SF_LASTSEG))
	{
		if(Txd_123_RDSJ_F_LS())
			return TRUE;
	}

	//�����ϴ��Ŷ����ݵ�Ŀ¼	

//===========================¼���ļ���ѯ End========================

	//���Ͷ�������
	if(Txd_Class2()) //���ͱ仯���� 
		return TRUE;

	return FALSE;
}

//�������ٻ�ң�⡢ң��
BOOL CGm104::TxdCallAll(void)
{
if(CheckClearFlag(SF_AIGRP1))
	if( Txd_AIGroup(0,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP2))	
	if( Txd_AIGroup(1,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP3))	
	if( Txd_AIGroup(2,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP4))	
	if( Txd_AIGroup(3,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP5))	
	if( Txd_AIGroup(4,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP6))	
	if( Txd_AIGroup(5,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP7))	
	if( Txd_AIGroup(6,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP8))	
	if( Txd_AIGroup(7,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP9))	
	if( Txd_AIGroup(8,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP10))	
	if( Txd_AIGroup(9,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP11))	
	if( Txd_AIGroup(10,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP12))	
	if( Txd_AIGroup(11,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP13))	
	if( Txd_AIGroup(12,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP14))	
	if( Txd_AIGroup(13,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP15))	
	if( Txd_AIGroup(14,20) )
		return TRUE;
if(CheckClearFlag(SF_AIGRP16))	
	if( Txd_AIGroup(15,20) )
		return TRUE;

if(CheckClearFlag(SF_BIGRP1))	
	if( Txd_BIGroup(0,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP2))	
	if( Txd_BIGroup(1,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP3))	
	if( Txd_BIGroup(2,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP4))	
	if( Txd_BIGroup(3,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP5))	
	if( Txd_BIGroup(4,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP6))	
	if( Txd_BIGroup(5,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP7))	
	if( Txd_BIGroup(6,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP8))	
	if( Txd_BIGroup(7,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP9))	
	if( Txd_BIGroup(8,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP10))	
	if( Txd_BIGroup(9,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP11))	
	if( Txd_BIGroup(10,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP12))	
	if( Txd_BIGroup(11,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP13))	
	if( Txd_BIGroup(12,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP14))	
	if( Txd_BIGroup(13,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP15))	
	if( Txd_BIGroup(14,20) )
		return TRUE;
if(CheckClearFlag(SF_BIGRP16))	
	if( Txd_BIGroup(15,20) )
		return TRUE;

if(CheckClearFlag(SF_ALLSTOP))	
	if( Txd_AllStop( ) )
		return TRUE; 
return FALSE;
}

//�������ٻ����
BOOL CGm104::TxdCallAllCI(void)
{
if(CheckClearFlag(SF_CIGRP1))	
	if( Txd_CIGroup(0,3) )
		return TRUE;
if(CheckClearFlag(SF_CIGRP2))	
	if( Txd_CIGroup(1,3) )
		return TRUE;
if(CheckClearFlag(SF_CIGRP3))	
	if( Txd_CIGroup(2,3) )
		return TRUE;
if(CheckClearFlag(SF_CIGRP4))	
	if( Txd_CIGroup(3,3) )
		return TRUE;
   
if(CheckClearFlag(SF_ALLCISTOP)) 
	if( Txd_AllCIStop( ) )
		return TRUE;

return FALSE;
}

BOOL CGm104::Txd68_68(void)//������·
{
BYTE Style=STARTHEAD,Reason=0x87;
BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0xAA; 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x55; 
	Txd_Tail(Num);
	
	return TRUE;
}

BOOL CGm104::Txd68_6B(void)//������·
{
	BYTE 	Style=0x6B,Reason=0x87;
	BYTE 	Num=1;
	PBYTE 	pData=&pRxdFm->Data;
	TSysTime sysTime;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��Ϣ���ַ add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[0]; //���ز����������
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[1]; //���ز���������� 

	ReadSystemTime(&sysTime);

	Txd68_LongTime(&sysTime); //��7���ֽ�ʱ���

	Txd_Tail(Num);
	
	return TRUE;
}

BOOL CGm104::Txd68_6E(void)//���ò���
{
	BYTE 	Style=0x6E,Reason=7;
	BYTE 	Num=1;
	PBYTE 	pData=&pRxdFm->Data;
	
	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[0]; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[1]; //��Ϣ���ַHi  
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[2];; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[3]; //QPM
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[4]; //QPM
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[5]; //QPM
	Txd_Tail(Num);
	return TRUE;
}


BOOL CGm104::Txd_BurstBI(void) //���ͱ仯ң��
{
	WORD		BITxdNum;
	WORD 		wBIEReadPtr;
	TBurstBI 	event;
	
	Txd_Head(1,0x03);
	
	if(DB_GetBIERead(&wBIEReadPtr) != DB_ERR_OK)
		return FALSE;
	
	for(BITxdNum=0;BITxdNum<GRP_BINUM/3&&GetNewBieNum(); BITxdNum++)
	{
		if( DB_ReadBIE(&event) != DB_ERR_OK)
			break;
		
		if(::DB_ReadBIE(&event ,(wBIEReadPtr+m_nBIENoAckCnt)%MAX_BIE_NUM) != DB_ERR_OK)
			break;
		
//		DB_IncBIERead();
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(event.wNo+BI_ADDR_START); //��Ϣ���ַLo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(event.wNo+BI_ADDR_START); //��Ϣ���ַHi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = (event.bStatus==DB_BI_CLOSE)?1:0;
		
		// ��¼����
		m_nBIENoAckCnt++;
		// ��¼�������
		m_nBIENewTxSeq = m_dwTxdCount;
		
#ifdef S104_DEBUG			 
		ReportMsg("S104:	Send bie enent! No=%d Status=%d",event.wNo,event.bStatus);
#endif		
	}

	if(BITxdNum==0)
		return FALSE;
	
	Txd_Tail(BITxdNum);
	
	return TRUE;
}


BOOL CGm104::SearchClass1(void) //�������1������
{
	// ң�ű�λ����
	if(GetFlag(SF_HAVEBIE))
		return TRUE;

	//  �б�������
	if(GetFlag(SF_HAVEREPORT))
		return TRUE;

	return FALSE; //��1�����ݷ���FALSE
}

BOOL CGm104::Txd_Soe(void) //����SOE����
{
	TEventBI 	event;
	TSysTime	time;
	WORD		BINo;
	WORD 		MSecond;
	WORD		SoeTxdNum;
	WORD 		wSOEReadPtr;
	
	Txd_Head(30,3);

	if(DB_GetSOERead(&wSOEReadPtr) != DB_ERR_OK)
		return FALSE;
		
	for(SoeTxdNum=0;SoeTxdNum<GRP_SOENUM&&GetNewSoeNum(); SoeTxdNum++)
	{
		if(::DB_ReadSOE(&event ,(wSOEReadPtr+m_nSOENoAckCnt)%MAX_SOE_NUM) != DB_ERR_OK)
				break;
		
//		DB_IncSOERead();
		
		BINo=event.wNo+1;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(BINo);  //���L
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(BINo);  //���H
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = (event.bStatus == DB_BI_CLOSE)?1:0;  //״̬

		AbsTimeTo(&event.stTime,&time);
		MSecond=time.Second*1000+time.MSecond;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(MSecond);
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(MSecond);
		m_Txd.Buf[ m_Txd.WritePtr++ ] = time.Minute;
		
//long time for DF8002		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = time.Hour;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = time.Day;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = time.Month;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = time.Year%100;

		// ��¼����
		m_nSOENoAckCnt++;
		// ��¼�������
		m_nSOENewTxSeq = m_dwTxdCount;

#ifdef S104_DEBUG			 
		ReportMsg("S104:	Send soe enent! No=%d Status=%d",event.wNo,event.bStatus);
#endif		
	}
		
	if(SoeTxdNum==0)
		return FALSE;

	Txd_Tail(SoeTxdNum);
	
	return TRUE;
}

BOOL CGm104::Txd_ChangeAI(void) //���ͱ仯ң������
{
#if(SYSTEM_TYPE == _SYS_KF1300_)	
	WORD AITxdNum,AINo;
	long 	lValue;
	float	fValue;
	BYTE	*pValue;
	
	Txd_Head(13,3);

	for(AITxdNum=0;AITxdNum<GRP_AINUM/2; AITxdNum++)
	{
		if (m_Txd.WritePtr   >= 240-6)
		{
			break;
		}

		if(SearchChangedAI(&AINo,&lValue,5,CHGAI_PER)==FALSE)
			break;

		DB_ReadFloatRM(AINo, &fValue);
		fValue = _ConvertFloat(fValue);
		pValue = (BYTE *)&fValue;
			
		m_pOldAI[AINo] = lValue;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo+AI_ADDR_START); //��Ϣ���ַLo //AINo+0x701
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo+AI_ADDR_START); //��Ϣ���ַHi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��ַadd by xyb
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[0]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[1]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[2]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[3]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //ң��ֵƷ��
	}

	if(AITxdNum==0)
		    return FALSE;                  
		    
	Txd_Tail(AITxdNum); //�仯����SQ=0

#else
	BYTE Style=35,Reason=3;//����ԭ��ͻ��
	WORD AITxdNum,AINo;
	long AIValue;
	TSysTime sysTime;
		
	Txd_Head(Style,Reason);
	
	for(AITxdNum=0;AITxdNum<GRP_AINUM/2; AITxdNum++)
	{
		if (AITxdNum  >= 16)
		{
			break;
		}
		if(SearchChangedAI(&AINo,&AIValue,5,CHGAI_PER)==FALSE)	   //
			break;
	
		m_pOldAI[AINo] = AIValue;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo+AI_ADDR_START); //��Ϣ���ַLo //AINo+0x701
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo+AI_ADDR_START); //��Ϣ���ַHi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //��ַadd by xyb
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AIValue); //ң��ֵ Lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AIValue); //ң��ֵ Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //ң��ֵƷ��
	
		ReadSystemTime(&sysTime);

		Txd68_LongTime(&sysTime); //��7���ֽ�ʱ���
	}
	
	if(AITxdNum==0)
			return FALSE;				   
			
	Txd_Tail(AITxdNum); //�仯����SQ=0
#endif
	return TRUE;
	
}

BOOL CGm104::Txd_Class1(void) //����1������
{
	// ң�ű�λ����
	if(GetFlag(SF_HAVEBIE))
	{
		if(Txd_BurstBI())
			return TRUE;
		else
			SetFlag(SF_HAVEBIE,FLAG_OFF);
	}
	
	//  �б�������
	if(GetFlag(SF_HAVEREPORT))
	{
		if(Txd_RelayEvent())
			return TRUE;
		else
			SetFlag(SF_HAVEREPORT,FLAG_OFF);
	}

    return FALSE;
}


BOOL CGm104::Txd_Class2(void) //����2������
{
    if(GetFlag(SF_HAVESOE))
    {
		if(Txd_Soe())	
			return TRUE;
		else
			SetFlag(SF_HAVESOE,FLAG_OFF);
    }
	
//	if(CheckClearFlag(SF_SendChangedAI))
	{
		if(Txd_ChangeAI())	
			return TRUE;
	}

    return FALSE;//��2�����ݷ���FALSE
}


BOOL CGm104::Txd68_65_5(void)//�ٻ�ĳ����
{
	WORD 	GroupNo;
	PBYTE 	pData=&pRxdFm->Data;

	GroupNo = pData[3]&0x0F-1;
	
	return Txd_CIGroup(GroupNo,GroupNo+38);
}

BOOL CGm104::Txd68_65_7(void)//�ٻ����е�ȵ�ȷ��
{
	Txd_Head(0x65,7);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַHi     
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x45; //COI
	Txd_Tail(1);
	return TRUE;
}

BOOL CGm104::Txd_AllCIStop(void)//�ٻ����е�ȵ�ȷ��
{
	Txd_Head(0x65,10);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x45; //COI
	Txd_Tail(1);
	return TRUE;
}

BOOL CGm104::Txd_CIGroup(WORD GroupNo,BYTE Reason)//����һ����
{
	DWORD CINo,CITxdNum,CIValue;

	CINo=GroupNo*GRP_CINUM;
	if(CINo>=DB_GetCINum())
		return FALSE;
	
	Txd_Head(0x0F,Reason);

	for(CITxdNum=0;CITxdNum<GRP_CINUM&&CINo<DB_GetCINum(); CINo++,CITxdNum++)
	{
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(CINo+CI_ADDR_START); //��Ϣ���ַ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(CINo+CI_ADDR_START); //��Ϣ���ַ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
		DB_ReadCI(CINo,&CIValue);
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HHBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = CITxdNum+1; //˳���
	}

	Txd_Tail(CITxdNum); //SQ=0
	return TRUE;
}

BOOL CGm104::Txd68_67(void)//���ӵ�ȷ��
{
BYTE Style=0x67,Reason=7;
BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַHi     
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	PBYTE pData=&pRxdFm->Data;
	pData+=3;
	for(WORD i=0;i<7;i++)
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[i]; //
	Txd_Tail(Num);
	return TRUE;
}

BOOL CGm104::Txd_AIGroup(WORD GroupNo,BYTE Reason)//����һ��ң��  OK
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	
	WORD 	AINo,AITxdNum,nAINum;
	float   fValue;
	BYTE	*pValue;
	
	nAINum = DB_GetRMNum();
		
	AINo=GroupNo*GRP_AINUM;

	if(AINo>=nAINum)
		return FALSE;
	
	Txd_Head(13,Reason);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo+AI_ADDR_START); //LOBYTE(AINo+0x701);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo+AI_ADDR_START); //HIBYTE(AINo+0x701);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	for(AITxdNum=0;AITxdNum<GRP_AINUM&&AINo<nAINum; AINo++,AITxdNum++)
	{
		DB_ReadFloatRM(AINo,&fValue);
		fValue = _ConvertFloat(fValue);
		pValue = (BYTE *)&fValue;
		                               
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[0]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[1]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[2]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[3]; //ң��ֵ
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //ң��ֵƷ��
	}

	//VSQ,����
	Txd_Tail(AITxdNum|0x80);

#else
		
	WORD	AINo,AITxdNum,nAINum;
	long	lValue;
	
	nAINum = DB_GetAINum();
		
	AINo=GroupNo*GRP_AINUM;
	
	if(AINo>=nAINum)
		return FALSE;
	
	Txd_Head(11,Reason);
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo+AI_ADDR_START); //LOBYTE(AINo+0x701);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo+AI_ADDR_START); //HIBYTE(AINo+0x701);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	for(AITxdNum=0;AITxdNum<GRP_AINUM&&AINo<nAINum; AINo++,AITxdNum++)
	{
		DB_ReadSecondAI(AINo,&lValue);
									   
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(lValue); //ң��ֵ Lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(lValue); //ң��ֵ Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;//��Ʒ������
	}
	
	//VSQ,����
	Txd_Tail(AITxdNum|0x80);
#endif
	
	return TRUE;
}

BOOL CGm104::Txd_BIGroup(WORD GroupNo,BYTE Reason)//����һ��ң��
{
	WORD 	BINo,BITxdNum;
	BYTE	BIStatus;
	
	BINo = GroupNo*GRP_BINUM;
	if(BINo>=DB_GetBINum())
		return FALSE;
	
	Txd_Head(1,Reason);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(BINo+BI_ADDR_START); //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(BINo+BI_ADDR_START); //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	for(BITxdNum=0;BITxdNum<GRP_BINUM&&BINo<DB_GetBINum(); BINo++,BITxdNum++)
	{
		DB_ReadBI(BINo,&BIStatus);
		m_Txd.Buf[ m_Txd.WritePtr++ ] = (BIStatus == DB_BI_CLOSE)?1:0;
	}

	//VSQ=1
	Txd_Tail(BITxdNum|0x80); 

	return TRUE;
}

BOOL CGm104::Txd_AllStop(void)//�������ٻ�����֡
{
	BYTE Style=0x64,Reason=0x0A;
	BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x14; //COI
	Txd_Tail(Num);
	return TRUE;
}

BOOL CGm104::Txd68_64_14_6(void)//�������ٻ���������
{
	BYTE Style=0x64,Reason=7;
	BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x14; //COI
	Txd_Tail(Num);
	return TRUE;
}

//=======================================================================================
// ��������: ����3���ֽڵĶ�ʱ��
//=======================================================================================
BOOL CGm104::Txd68_ShortTime()
{
	TAbsTime absTime;
	TSysTime sysTime;
	WORD MSecond;

	ReadAbsTime(&absTime);
	AbsTimeTo(&absTime,&sysTime);

	MSecond = sysTime.Second*1000+sysTime.MSecond;
	m_Txd.Buf[m_Txd.WritePtr++]= LOBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++]= HIBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++]= sysTime.Minute&0x3f;

	return TRUE;
}
//=======================================================================================
// ��������: ����7���ֽڵĳ�ʱ��
//=======================================================================================
BOOL CGm104::Txd68_LongTime(TSysTime *pTime)
{
	WORD MSecond;

	if(pTime == NULL)
		return FALSE;
	
	MSecond = pTime->Second*1000+pTime->MSecond;

	m_Txd.Buf[m_Txd.WritePtr++] = LOBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++] = HIBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++] = pTime->Minute;  
	m_Txd.Buf[m_Txd.WritePtr++] = pTime->Hour;  
	m_Txd.Buf[m_Txd.WritePtr++] = pTime->Day;  
	m_Txd.Buf[m_Txd.WritePtr++] = pTime->Month;  
	m_Txd.Buf[m_Txd.WritePtr++] = pTime->Year%100;  

	return TRUE;
}   

//=======================================================================================
// ��������: �����ĵ�ͷ��
//=======================================================================================
BOOL CGm104::Txd_Head(BYTE Style,WORD Reason)
{
	WORD t_txcount;
	WORD t_rxcount;

	m_dwRxdCount %= MAXSEQUENCER;
	m_dwTxdCount %= MAXSEQUENCER;

	t_txcount = m_dwTxdCount<<1;
	t_rxcount = m_dwRxdCount<<1;
	
	pTxdFm->Start	= STARTHEAD;
    pTxdFm->Control1 = LOBYTE(t_txcount);
    pTxdFm->Control2 = HIBYTE(t_txcount);
    pTxdFm->Control3 = LOBYTE(t_rxcount);
    pTxdFm->Control4 = HIBYTE(t_rxcount);

	pTxdFm->Address_L= DB_GetAddress();
	pTxdFm->Address_H= 0;
	pTxdFm->Style	=Style;
	pTxdFm->Reason_L=LOBYTE(Reason);
	pTxdFm->Reason_H=0;
	
	m_Txd.WritePtr=(PBYTE)&pTxdFm->Data-(PBYTE)&pTxdFm->Start; //ָ�����

	return TRUE;
}
//=======================================================================================
// ��������: �����ĵ�β��
//=======================================================================================
BOOL CGm104::Txd_Tail(BYTE Num)
{
	BYTE* pBuf;
		
	pBuf = m_Txd.Buf;

	pBuf[1] = m_Txd.WritePtr-2;
	pBuf[2] = LOBYTE(m_dwTxdCount<<1);
	pBuf[3] = HIBYTE(m_dwTxdCount<<1);
	pBuf[4] = LOBYTE(m_dwRxdCount<<1);
	pBuf[5] = HIBYTE(m_dwRxdCount<<1);
	pBuf[7] = Num;//VSQ

	WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);
	SaveDataBuf();
	
	m_dwTxdCount = (m_dwTxdCount+1)%MAXSEQUENCER;
	MyKillTimer(T2_TIMERID);
    MySetTimer(T3_TIMERID,T3); 	//��λT3��ʱ��
	
	return TRUE;
}


BOOL CGm104::Txd68_66_AI(WORD AINo)
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	float	fValue;
	BYTE	*pValue;
	
	Txd_Head(13,5);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo); //LOBYTE(AINo+0x701)��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo); //HIBYTE(AINo+0x701)��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	DB_ReadFloatRM(AINo,&fValue);
	fValue = _ConvertFloat(fValue);
	pValue = (BYTE *)&fValue;

	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[0]; //ң��ֵ
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[1]; //ң��ֵ
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[2]; //ң��ֵ
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[3]; //ң��ֵ
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //ң��ֵƷ��

	Txd_Tail(1);
#else	
	long AIValue;

	Txd_Head(11,5);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo); //LOBYTE(AINo+0x701)��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo); //HIBYTE(AINo+0x701)��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	DB_ReadSecondAI(AINo,&AIValue);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AIValue); //ң��ֵ Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AIValue); //ң��ֵ Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //Ʒ������
	Txd_Tail(1);
#endif	
	return TRUE;
}
	
BOOL CGm104::Txd68_66_BI(WORD BINo)
{
	BYTE BIStatus;
	
	Txd_Head(1,5);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(BINo+1); //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(BINo+1); //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	DB_ReadBI(BINo,&BIStatus);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = (BIStatus == DB_BI_CLOSE)?1:0;
    m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;//�޶��� 
   	Txd_Tail(1);
	return TRUE;
}
BOOL CGm104::Txd68_66_CI(WORD CINo)
{
	DWORD CIValue;

	Txd_Head(15,5);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(CINo); //LOBYTE(CINo+0xc01)��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(CINo); //HIBYTE(CINo+0xc01)��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	DB_ReadCI(CINo,&CIValue);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(CIValue); //
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(CIValue); //
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(CIValue); //
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HHBYTE(CIValue); //
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //
	
	Txd_Tail(1);

   return TRUE;
}

BOOL CGm104::Txd68_2E2F3B3C(BYTE Reason) //����ң��Ԥ��/ִ��	
{
	TSysTime	sysTime;
	
	if(m_BO.RetResult!=PBO_SUCESS)
		Txd_Head(m_BO.Info[0],47);
	else
		Txd_Head(m_BO.Info[0],Reason);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[1]; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[2]; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[3]; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[4]; //DCO

	if(!Is101BO)
	{
		ReadSystemTime(&sysTime);
		Txd68_LongTime(&sysTime); //��7���ֽ�ʱ���
	}
	
	if(Reason == 9)
		Is101BO = FALSE;
		
#ifdef S104_DEBUG			 
	LogError("Txd68_2E2F3B3C",FILE_LINE,"RetResult =%d!! Command = %d  No= %d  Attrib=%d Reason=%d\r\n\r\n ",m_BO.RetResult,m_BO.Command,m_BO.No,m_BO.Attrib,Reason);
#endif

	Txd_Tail(1); 

	return TRUE;
}

BOOL CGm104::Txd68_69_Ack(void)
{
	memcpy(m_Txd.Buf,pRxdFm, pRxdFm->Length + 2);
	Txd_Head(0x69,0x07);
	m_Txd.WritePtr=pRxdFm->Length + 2;

	WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);

	return TRUE;

}

//=========================================================================
// ��������: ��վ��ʼ�����
// �������: ��
// ����ֵ:   bool��ʼ�����
//	ע��
//=========================================================================

BOOL CGm104::Txd_InitOver(void)
{
	BYTE Style=0x46,Reason=4;
	BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַLo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //��Ϣ���ַHi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x01; //COI
	Txd_Tail(Num);
	return TRUE;
}

//=========================================================================
// ��������: ����������״̬�澯 ��ѹ�¹ʼ�
// �������: ��
// ����ֵ:	 BOOL
//	ע�� 20110418 ��
//=========================================================================
BOOL CGm104::Txd_RelayEvent(void)
{
	TAbsTime		AbsTime;
	TSysTime 		SysTime;
	WORD	   		MSecond;  
	BYTE 			Num=0;
	BYTE			FD_Num;
	BYTE			*pFD_Data;
	
	// û��ʣ�౨��ʱ
	if((m_nLeftActionReportNum == 0)||(m_pNextRelayEvent == NULL))
	{
		m_pNextRelayEvent = NULL;
		m_nLeftActionReportNum = 0;
		
		// 3.�ٰ���ʱ��˳�������ȡ����
		if(!SearchNewReport(m_tCurrentReport,m_CurrentReportType))
			return FALSE;
		
		if(m_CurrentReportType == DB_EVENT_RACT)
		{
			// ��ʼ������ָ��
			m_pNextRelayEvent = &m_tCurrentReport.tAction.tEvent.tAction;
			m_nLeftActionReportNum = m_tCurrentReport.tAction.tEvent.Num;
	
#ifdef S104_DEBUG
			LogError("Txd68_Report_61",FILE_LINE,"Send a new action report!!!");
#endif			
			// ֱ�ӵ���ָ��
			//DB_IncRelayActionReportRead();
			m_nACTNoAckCnt++;
		}
		else if(m_CurrentReportType == DB_EVENT_RALM)
		{
			// ��ʼ������ָ��
			m_pNextRelayEvent = &m_tCurrentReport.tAlram.tEvent.tAction;
			m_nLeftActionReportNum = 1;
	
#ifdef S104_DEBUG
			LogError("Txd68_Report_61",FILE_LINE,"Send a new alarm report!!!");
#endif
			// ֱ�ӵ���ָ��
			//DB_IncRelayAlarmReportRead();			
			m_nALMNoAckCnt++;
		}
		else
		{
			LogError("Txd68_Report_61",FILE_LINE,"Search event type error!!!");
			return FALSE;
		}
		
	}
	
	// ��Ч������ֱ���޳�
	if(m_pNextRelayEvent->FD_Num > MAX_FD_NUM)
	{
		m_pNextRelayEvent = NULL;	
		m_nLeftActionReportNum = 0;
		LogError("Txd68_Report_61",FILE_LINE,"Invalid event report!!!");
		return FALSE;
	}

	// 2.�ȶ�ȡ�������汨��
	Txd_Head(140,3);
			
	// ��䱣������
	Txd_ElementAddr(0);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_pNextRelayEvent->nINF); //INF
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_pNextRelayEvent->nINF); //INF

	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_pNextRelayEvent->bStatus + 1;  //��2����1
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//���ʱ���8λ m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//���ʱ���8λ
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//���ʱ���8λ

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_wFaultNo);  //������ŵ�8λ
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_wFaultNo);  //������Ÿ�8λ
	
	m_wFaultNo ++;
	m_wFaultNo %= 0xffff; 
	
	AbsTime.Lo = m_pNextRelayEvent->AbsTimeLo;
	AbsTime.Hi = m_pNextRelayEvent->AbsTimeHi;
	AbsTimeTo(&AbsTime,&SysTime);
	MSecond = SysTime.MSecond+SysTime.Second*1000;

	m_Txd.Buf[m_Txd.WritePtr++] = LOLOBYTE(MSecond);	//��øĳ�7���ֽڵ�ʱ��
	m_Txd.Buf[m_Txd.WritePtr++] = LOHIBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++] = SysTime.Minute;
	m_Txd.Buf[m_Txd.WritePtr++] = SysTime.Hour;
	m_Txd.Buf[m_Txd.WritePtr++] = SysTime.Day;
	m_Txd.Buf[m_Txd.WritePtr++] = SysTime.Month;
	m_Txd.Buf[m_Txd.WritePtr++] = SysTime.Year-2000;

	if(m_pNextRelayEvent->FD_Num <= MAX_IND8D_FDNUM)
	{
		FD_Num = m_pNextRelayEvent->FD_Num;
	}
	else
	{
		FD_Num = MAX_IND8D_FDNUM;	//���ϵ�������
		LogError("Txd_RelayEvent",FILE_LINE,"Report FD num is too many!!! FD_Num=%d",m_pNextRelayEvent->FD_Num);
	}

	m_Txd.Buf[m_Txd.WritePtr++] = FD_Num;	//���ϵ�������

	pFD_Data = (BYTE *)(m_pNextRelayEvent+1);
	// ��������ź͹��ϵ���
	for(int i = 0; i<FD_Num; i++,pFD_Data += 6)
	{
		m_Txd.Buf[m_Txd.WritePtr++] = pFD_Data[0];  //���ϵ���������
		m_Txd.Buf[m_Txd.WritePtr++] = pFD_Data[1];
	
		m_Txd.Buf[m_Txd.WritePtr++] = GDD_DATATYP_R32_23; //���ϵ�����������
	
		//���ϵ���ֵ��float���ͣ����ĸ��ֽڴ���
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pFD_Data[2];
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pFD_Data[3];
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pFD_Data[4];
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pFD_Data[5];
	}
	
	if(m_CurrentReportType == DB_EVENT_RACT)
		m_nACTNewTxSeq = m_dwTxdCount;
	else
		m_nALMNewTxSeq = m_dwTxdCount;
	
	Txd_Tail(1);
	
#ifdef S104_DEBUG			 
	ReportMsg("ITEM: Inf=%d FD_Num=%d ",m_pNextRelayEvent->nINF,FD_Num);
#endif

	// �ݼ�ʣ��ֱ������
	m_nLeftActionReportNum--;

	// ������һ���ݱ���ָ��
	m_pNextRelayEvent = (TRelayEventHead *)((BYTE *)(m_pNextRelayEvent+1)+m_pNextRelayEvent->FD_Num*6);
		
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
///////////////***************************************************************************8

//ȡ��ֵ���ŷ�����Ӧ
//103 typ = 10
//=======================================================================================
// ��������: Txd68_145_STSReadEcho
// ����������void
// �������أ�bool
// ��������:  ȡ��ֵ���ŷ�����Ӧ
// ������д�������� 2004-06-23
// ������д��
// ������	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STSReadEcho(void)
{
	PBYTE 		pData=&pTxdFm->Data;
	TGenData	*pGenData;
	BYTE 		Num=1;        //�ɱ�ṹ�޶���

	pGenData = (TGenData *)&pData[7];

	Txd_Head(145,5);

	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_RGROUPVALUE;	//INF 241   //��һ�����ȫ����Ŀ��ֵ������;
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;					//RII

	pGenData->Ngd.Number = 1;
	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;

	if(m_ST.Group >= 4)		//���ش���	���ݸ���Ϊ0
	{
		pGenData->Ngd.Number = 0;
		Num = 0;
		m_Txd.WritePtr += sizeof(TNgd);
	}
	else
	{
		pGenData->Gin.byGroup = 0;
		pGenData->Gin.byEntry = 1;

		pGenData->byKod = 0;
		
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//��������Ϊ�޷�������
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		pGenData->byGid			= DB_GetCurrentSETAreaNo()+ 1;

		m_Txd.WritePtr += sizeof(TGenData);
	}

	Txd_Tail(Num);  

	return TRUE;
}

//=======================================================================================
// ��������: Txd68_145_STSReadEcho
// ����������void
// �������أ�bool
// ��������:  �޸Ķ�ֵ����Ԥ�÷���
// ������д�������� 2004-06-23
// ������д��
// ������	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STSSelectEcho(void)
{
	PBYTE 		pRxdData = &pRxdFm->Data;
	PBYTE 		pTxdData = &pTxdFm->Data;
	TGenItem	*pelemData;
	TGenData	*pGenData;
	STATUS		rc = DB_ERR_OK;
	
	//��ֵ����ѡ��
	pelemData = (TGenItem *)&pRxdData[8];
	m_ST.Group = pelemData->byGid-1;
	
	pGenData = (TGenData *)&pTxdData[7];
		
	rc = DB_SetCurrentSETAreaNo(m_ST.Group);

	Txd_Head(145,5);
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_WENTRYACK;	//INF    249   ��ȷ�ϵ�д��Ŀ;	   
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;					//RII


	pGenData->Ngd.Number = 1;
	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;

	if(rc != DB_ERR_OK)		//���ش���	���ݸ���Ϊ0
	{
		pGenData->Ngd.Number = 0;
		m_Txd.WritePtr += sizeof(TNgd);
		Txd_Tail(0);	
	}
	else
	{
		pGenData->Gin.byGroup = 0;
		pGenData->Gin.byEntry = 1;
		pGenData->byKod = 0;		
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//��������Ϊ�޷�������
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		pGenData->byGid	= m_ST.Group+1;
		m_Txd.WritePtr += sizeof(TGenData);
		Txd_Tail(1);	
	}

	return TRUE;
}

//=======================================================================================
// ��������: Txd68_145_STSExecEcho
// ����������
// �������أ�
// ��������:  �޸ģ���ֵ���ţ�ִ��/��������
// ������д��lz 2011-12-30
// ������д��
//=======================================================================================
BOOL CGm104::Txd68_145_STSExecEcho(void)
{//�޸ģ���ֵ���ţ�ִ��/��������
	PBYTE 		pRxdData = &pRxdFm->Data;
	PBYTE 		pTxdData = &pTxdFm->Data;
	TGenData	*pGenData;
	BYTE 		bINF;
	STATUS		rc;

	bINF = pRxdData[5];
	
	if(bINF == INF_M_GEN_WENTRYEXEC)
	{//ִ��
		rc = DB_SetCurrentSETAreaNoConfirm(m_ST.Group,DB_EXECUTE);
	}
	else if(bINF == INF_M_GEN_WENTRYCANCEL)
	{//����
		rc = DB_SetCurrentSETAreaNoConfirm(m_ST.Group,DB_CANCEL);
	}	

	pGenData = (TGenData *)&pTxdData[7];

	if(rc == DB_ERR_OK)
		Txd_Head(145,5);
	else
		Txd_Head(145,0x45);
		
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = bINF;	//INF 250   ��ִ�е�д��Ŀ;	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;					//RII

	pGenData->Ngd.Number = 1;
	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;
	
	if(rc != DB_ERR_OK)
	{
		pGenData->Ngd.Number = 0;
		m_Txd.WritePtr += sizeof(TNgd);
		Txd_Tail(0);  
	}
	else
	{
		pGenData->Gin.byGroup = 0;
		pGenData->Gin.byEntry = 1;

		pGenData->byKod = 0;
		
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//��������Ϊ�޷�������
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		
		pGenData->byGid	= m_ST.Group+1;

		m_Txd.WritePtr += sizeof(TGenData);
		Txd_Tail(1);  
	}

	return TRUE;
}



//=======================================================================================
// ��������: Txd68_145_STSExecEcho
// ����������void
// �������أ�bool
// ��������:  ȡ��ֵ����
// ������д��lz 2011-12-30
// ������д��
//=======================================================================================
BOOL CGm104::Txd68_145_STReadEcho(void)
{
	PBYTE 	pData=&pTxdFm->Data;
	TNgd	*Ngd;
	BOOL  	bisLastFrm = FALSE;
	BYTE 	Num=(MAX_TXDFRAME_SIZE - (sizeof(TS104TxdFm)-1+8)) / (sizeof(TGenItem)-1+4);        //�ɱ�ṹ�޶���

	Ngd = (TNgd *)&pData[7];

	if(Num + m_ST.Offset >= m_ST.Num)
	{
		Num = m_ST.Num - m_ST.Offset;
		SetFlag(SF_STSend,FLAG_OFF);
		bisLastFrm = TRUE;
	}
	
	Txd_Head(145,5);
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_RGROUPVALUE;  //INF	241   ��һ�����ȫ����Ŀ��ֵ������
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;		//RII

	Ngd->Number = Num;
	Ngd->Count = 0;
	Ngd->Cont = 1;
	if(bisLastFrm)
		Ngd->Cont = 0;

	if(m_ST.Result != 0)//���ش���
	{
		Ngd->Number = 0;
		Num = 0;
		m_Txd.WritePtr += sizeof(TNgd);
		bisLastFrm = TRUE;  
		Ngd->Cont = 0;
		SetFlag(SF_STSend,FLAG_OFF);
	}
	else
	{
		PBYTE pdzData	= (PBYTE)&pData[7] + sizeof(TNgd);			//��ֵԪ�ؿ�ʼ��ַ
		PBYTE ptmp = pdzData;
		TGenItem* pDZe;

		for(int i = 0; i < Num; i++)
		{
			pDZe = (TGenItem*)pdzData;

			pDZe->Gin.byGroup = m_ST.Group + 1;		//��ֵ��Ŵ�1��ʼ
			pDZe->Gin.byEntry = m_ST.Offset+ i;		//��ֵ��Ŀ��0��ʼ 
			pDZe->byKod = 0;
			
			if((m_ST.Offset+ i) < ((DB_GetSWNum()+31)/32+(DB_GetCFGNum()+31)/32))
			{
				pDZe->Gdd.byDataType = m_S104para.intsetcode;	// ������������
				pDZe->Gdd.byDataSize = 4;
				pDZe->Gdd.byDataNum = 0x81; //����״̬λ
			}
			else
			{
				pDZe->Gdd.byDataType = GDD_DATATYP_R32_23;	// �������� 7 Ϊ
				pDZe->Gdd.byDataSize = 4;
				pDZe->Gdd.byDataNum = 0x81;	//����״̬λ
			}
			
			memcpy(&pDZe->byGid,m_ST.Buf+(m_ST.Offset + i)*sizeof(float),4);

			pdzData += sizeof(TGenItem)-1+4;
		}

		if(bisLastFrm)
		{
			pDZe->Gdd.byDataNum &= 0x7F;		//����״̬λ = 0;
			SetFlag(SF_STSend,FLAG_OFF);
		}
		
		m_ST.Offset += Num;
		m_Txd.WritePtr += pdzData - ptmp + 1;
	}
	
	Txd_Tail(Num);  

	return TRUE;
}


//=======================================================================================
// ��������: Txd68_145_STSelectEcho
// ����������void
// �������أ�bool
// ��������:  �����޸Ķ�ֵԤ�÷���
// ������д�������� 2004-06-23
// ������д��
// ������	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STSelectEcho(void )
{
	PBYTE 		pData=&pTxdFm->Data;
	TGenData	*pGenData;

	pGenData = (TGenData *)&pData[7];

	if(m_ST.Result != DB_ERR_OK)	
		Txd_Head(145,0x45);
	else
		Txd_Head(145,0x05);
		
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_WENTRYACK; 		//INF    249   ��ȷ�ϵ�д��Ŀ;
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//RII

	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;
	if(m_ST.Result != DB_ERR_OK)
	{
		pGenData->Ngd.Number = 0;
		m_Txd.WritePtr += sizeof(TNgd);
		Txd_Tail(0);
	}
	else
	{
		pGenData->Ngd.Number = 1;
		pGenData->Gin.byGroup = m_ST.Group + 1;
		pGenData->Gin.byEntry = 0;

		pGenData->byKod = 0;
			
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//��������Ϊ�޷�������
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		pGenData->byGid= m_ST.Group + 1;
	    
		m_Txd.WritePtr += sizeof(TGenData);
		
		Txd_Tail(1);  
	}
	return TRUE;
}

//103 typ = 10
//=======================================================================================
// ��������: Txd68_145_STExecEcho
// ����������void
// �������أ�bool
// ��������:  �޸Ķ�ִֵ��/��������
// ������д�������� 2004-06-23
// ������д��
// ������	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STExecEcho(void)
{
	PBYTE 		pData=&pTxdFm->Data;
	TGenData	*pGenData;
	BYTE 		bINF;
	STATUS		rc = DB_ERR_OK;

	bINF = *(&pRxdFm->Data+5);
	//ͨ��������ָ��
	pGenData = (TGenData *)&pData[7];
	
	if(bINF == INF_M_GEN_WENTRYEXEC)
	{//ִ��
		rc = DB_SetAllSWStatusConfirm(DB_EXECUTE);
		rc |= DB_SetSETValueConfirm(m_ST.Group,SET_VALUE_ALL,DB_EXECUTE);
	}
	else if(bINF == INF_M_GEN_WENTRYCANCEL)
	{//����
		rc = DB_SetAllSWStatusConfirm(DB_CANCEL);
		rc |= DB_SetSETValueConfirm(m_ST.Group,SET_VALUE_ALL,DB_CANCEL);
	}

	if(rc != DB_ERR_OK)
		Txd_Head(145,0x45);
	else
		Txd_Head(145,0x05);
		
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //װ�õ�ַUI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = bINF;//INF 250   //��ִ�е�д��Ŀ;
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x0; //RII

	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;
	if(rc != DB_ERR_OK)
	{
		pGenData->Ngd.Number = 0;
		Txd_Tail(0);  
		LogError("Txd68_145_STExecEcho",FILE_LINE,"Execute SW error!");
	}
	else
	{
		pGenData->Ngd.Number = 1;
	    pGenData->Gin.byGroup = m_ST.Group + 1;
		pGenData->Gin.byEntry = 0;
		pGenData->byKod = 0;		
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//��������Ϊ�޷�������
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		pGenData->byGid	= m_ST.Group + 1;

		m_Txd.WritePtr += sizeof(TGenData);
		Txd_Tail(0x81);  
#ifdef S104_DEBUG			 
		LogError("Txd68_145_STExecEcho",FILE_LINE,"Execute SET OK!");
#endif
	}
		
	return TRUE;
}

BOOL CGm104::Txd_126_RDSJ_DIR_AllDir(void)
{
	BYTE	Style = 126;
	BYTE	Reason = 5;		// ͻ��
	BYTE    Num = 0;
			
	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	for(int i=m_DistDir.nSentCnt;i<m_DistDir.nFileNum;i++,Num++)
	{
		if(m_Txd.WritePtr +  2 + 3 + 1 + 7 > MAXFRMLEN)
			break;

		m_Txd.Buf[ m_Txd.WritePtr++ ] = m_DistDir.nSentCnt<<4;		//file name hi  ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len 
		
		if (m_DistDir.nSentCnt == (m_DistDir.nFileNum-1))
			m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x60;	//FOR=0,
		else
			m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x40;	//FOR=0,
				
		Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistDir.nSentCnt]); //��7���ֽڳ�ʱ��
		m_DistDir.nSentCnt ++;
	}

	// Ŀ¼�����������־
	if (m_DistDir.nSentCnt >= m_DistDir.nFileNum)
		SetFlag(SF_CALLDIR,FLAG_OFF);
	
	Txd_Tail(Num); 

#ifdef S104_DEBUG			 
	ReportMsg("S104:    ��������Ŀ¼�� Num =%d  Sent=%d Total=%d\n",Num,m_DistDir.nSentCnt,m_DistDir.nFileNum);
#endif

	return TRUE;
}

BOOL CGm104::Txd_126_RDSJ_DIR_SubDir()
{
	BYTE	Style = 126;
	BYTE	Reason = 3;		// ͻ��
			
	Txd_Head(Style,Reason);
	
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = (m_DistFile.nSubDir<<4)+1;		//file name LO( CFG )
 	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name HI 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(m_DistFile.dwDataLen[0]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(m_DistFile.dwDataLen[0]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(m_DistFile.dwDataLen[0]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x80;	//FOR=0,		
	Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistFile.nSubDir]); //��7���ֽڳ�ʱ��

	m_Txd.Buf[ m_Txd.WritePtr++ ] = (m_DistFile.nSubDir<<4)+2;		//file name LO( CFG )
 	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name HI 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(m_DistFile.dwDataLen[1]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(m_DistFile.dwDataLen[1]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(m_DistFile.dwDataLen[1]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0xA0;	//FOR=0,		
	Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistFile.nSubDir]); //��7���ֽڳ�ʱ��

	Txd_Tail(2); 

#ifdef S104_DEBUG			 
	ReportMsg("S104:    ���͵���Ŀ¼�ļ��б� FileNo=%x\n",m_DistFile.nSubDir);
#endif

	return TRUE;
}

BOOL CGm104::Txd_126_RDSJ_DIR_Auto(void)
{
	BYTE	Style = 126;
	BYTE	Reason = 3;		// ͻ��
			
	Txd_Head(Style,Reason);
	
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = (m_DistDir.nFileNum-1)<<4;	//file name lo  ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x60;	//FOR=0,
				
	Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistDir.nFileNum-1]); //��7���ֽڳ�ʱ��

	Txd_Tail(1); 

#ifdef S104_DEBUG			 
	ReportMsg("S104:    ���ͱ仯Ŀ¼�� FileNo=%d\n",m_DistDir.nFileNum-1);
#endif

	return TRUE;
}

BOOL	CGm104::Txd_120_RDSJ_F_FR(BOOL bReady)	//�ļ�׼������	
{
	BYTE	Style = 120;
	BYTE	Reason= 13;		//�ļ�����
	WORD	wFileName;
	
	Txd_Head(Style,Reason);
	
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	if(bReady)
	{
		wFileName = m_DistFile.nFileName;		
			
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(wFileName);	//file name hi	ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(wFileName);	//file name lo
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOLOBYTE(m_DistFile.dwFileLen); //file len lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOHIBYTE(m_DistFile.dwFileLen); //file len hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HILOBYTE(m_DistFile.dwFileLen); //file len 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//�ļ�׼�������޶���(FRQ)
#ifdef S104_DEBUG			 
		ReportMsg("S104:	Txd_120_RDSJ_F_FR �ļ�׼������!!FINENAME = %x\r\n",wFileName);
#endif
	}
	else
	{
		wFileName = m_DistFile.nSubDir<<4;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(wFileName);	//file name hi	ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(wFileName);	//file name lo
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //file len lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //file len hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //file len 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x80;	//�ļ�׼�������޶���(FRQ),����
#ifdef S104_DEBUG			 
		ReportMsg("S104:	Txd_120_RDSJ_F_FR �ļ�׼���쳣!!FINENAME = %x\r\n",wFileName);
#endif
	}

	Txd_Tail(1);  
	
	return TRUE;
}

BOOL	CGm104::Txd_121_RDSJ_F_SR()	//��׼������
{
	BYTE 	Style = 121;
	BYTE	Reason= 13;		//�ļ�����
	BYTE	nSecNo = m_DistFile.nSecName;
	
	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_DistFile.nFileName);	//file name lo ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_DistFile.nFileName);	//file name hi
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_DistFile.nSecName;	//7.2.6.34������(NOS) UI8

	if(nSecNo >= m_DistFile.nSecNum)
	{
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x80;	//��׼�������޶���(SRQ) CP8{UI7[1..7],BS1[8]}
	}
	else
	{
		// ����ڳ���
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOLOBYTE(m_DistFile.dwSecLen);	//�ڵĳ���(LOF) UI24[1..24]<0..16777215> 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOHIBYTE(m_DistFile.dwSecLen);	//�ڵĳ��� 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HILOBYTE(m_DistFile.dwSecLen);	//�ڵĳ��� 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//��׼�������޶���(SRQ) CP8{UI7[1..7],BS1[8]}
	}
	
	Txd_Tail(1);	

#ifdef S104_DEBUG			 
	ReportMsg("S104:    Txd_121_RDSJ_F_SR ��׼������!!secNAME = %d\n",m_DistFile.nSecName);
#endif

	return TRUE;
}

BOOL	CGm104::Txd_125_RDSJ_F_SG(void)//�ϴ��Ŷ����� ���ͱ�ʶ125��F_SG_NA_1 ��
{
	DWORD	len;			//�εĳ���			
	BYTE 	*pData ;		//�ε�����		
	BYTE	Style = 125;
	BYTE	Reason=13;		//�ļ�����
	BYTE	nSecNo = m_DistFile.nSecName;
	DWORD	dwDataOff;
	
	dwDataOff = m_DistFile.nSegNo*PERSEGDATALEN;

	pData = m_DistFile.pSecStart+dwDataOff;

	// �ж����һ��
	if(m_DistFile.nSegNo == (m_DistFile.nSegNum-1))
	{
		len = m_DistFile.dwSecLen-(m_DistFile.nSegNo*PERSEGDATALEN);

		SetFlag(SF_SENDSEC,FLAG_OFF);
		SetFlag(SF_LASTSEG);

		// ����������һ��
		if(nSecNo == (m_DistFile.nSecNum-1))
			SetFlag(SF_LASTSEC);
	}
	else
	{
		len = PERSEGDATALEN;
	}
	

	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_DistFile.nFileName);	//file name hi  ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_DistFile.nFileName);	//file name lo

	m_Txd.Buf[ m_Txd.WritePtr++ ] = nSecNo;	//7.2.6.34������(NOS) UI8

	m_Txd.Buf[ m_Txd.WritePtr++ ] = len;	//�εĳ���(LOF) UI24[1..24]<0..16777215> 

	memcpy(&m_Txd.Buf[ m_Txd.WritePtr], pData,len);
	m_Txd.WritePtr += len;

	m_DistFile.nSegNo++;

	Txd_Tail(1);  

#ifdef S104_DEBUG			 
	ReportMsg("S104:    Txd_125_RDSJ_F_SG ��!!secNAME = %d SegNo= %d\n",nSecNo,m_DistFile.nSegNo);
#endif

	return TRUE;
}


BOOL	CGm104::Txd_123_RDSJ_F_LS(void)	//���Ľ�,���Ķ�
{
	BYTE bLsq = 0,chs;
	BYTE Style = 123;
	BYTE Reason=13;		//�ļ�����

	if(CheckClearFlag(SF_LASTSEG))
	{
		bLsq = 3;
		chs = m_DistFile.bSecChs;
	}

	else if(CheckClearFlag(SF_LASTSEC))
	{
		bLsq = 1;
		chs = m_DistFile.bFileChs;
	}

	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_DistFile.nFileName);	//file name hi  ÿ��ģ����Ա���8��·������ 0~7��0��λ���µ�·������
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_DistFile.nFileName);	//file name lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_DistFile.nSecName;	//7.2.6.34������(NOS) UI8
	m_Txd.Buf[ m_Txd.WritePtr++ ] = bLsq;	//LSQ�����Ľ�,���Ķ��޶���(��7.2.6.31�ж� 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = chs;	//CHS��У���(��7.2.6.37�ж���) 

	Txd_Tail(1);  

#ifdef S104_DEBUG			 
	if(bLsq == 1)
		ReportMsg("S104:    Txd_123_RDSJ_F_LS ���Ľ�!!secNAME = %d\n",m_DistFile.nSecName);
	else
		ReportMsg("S104:    Txd_123_RDSJ_F_LS ���Ķ�!!secNAME = %d\n",m_DistFile.nSecName);
#endif

	return TRUE;

}

DWORD CGm104::GetNewSoeNum(void)
{
	WORD num=0;
	
	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return 0;
	}
	
	::DB_GetSOENum(&num); 

	//�Ѿ���������������
	if(num < m_nSOENoAckCnt)
	{
#ifdef S104_DEBUG			 
		ReportMsg("S104:    FAULT!!!!!!!!!!!!!!!!!!!!!!!!! soe num(%d)<AckCnt(%d)",num,m_nSOENoAckCnt);
#endif
		m_nSOENoAckCnt = 0;
		return num;
	}
	
	return num-m_nSOENoAckCnt;
}

DWORD CGm104::GetNewBieNum(void)
{
	WORD num=0;

	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return 0;
	}
	
	::DB_GetBIENum(&num);
	
	//�Ѿ���������������
	if(num < m_nBIENoAckCnt)
	{
#ifdef S104_DEBUG			 
		ReportMsg("S104:    FAULT!!!!!!!!!!!!!!!!!!!!!!!!! bie num(%d)<AckCnt(%d)",num,m_nBIENoAckCnt);
#endif
		m_nBIENoAckCnt = 0;
		return num;
	}
	
	return num-m_nBIENoAckCnt;
}

DWORD CGm104::GetNewAIENum(void)
{
	WORD num=0;
	
	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return 0;
	}
	
	::DB_GetAIENum(&num); 

	//�Ѿ���������������
	if(num < m_nAIENoAckCnt)
	{
		ReportMsg("S104:    FAULT!!!!!!!!!!!!!!!!!!!!!!!!! aie num(%d)<AckCnt(%d)",num,m_nAIENoAckCnt);
		m_nAIENoAckCnt = 0;
		return num;
	}
	
	return num-m_nAIENoAckCnt;
}

void CGm104::ClearSentEvent(DWORD dwAckCount)
{
	// ��SOE��ŵĴ���
	if(m_nSOENoAckCnt>0)
	{
		// SOE�õ���Ӧ��
		if( ((dwAckCount+0x8000-m_nSOENewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nSOENewTxSeq)%0x8000)<=IEC104_K)
		{
			for(int i=0;i<m_nSOENoAckCnt;i++)
				DB_IncSOERead();
	
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv soe ack! AckCnt=%d",m_nSOENoAckCnt);
#endif
			m_nSOENoAckCnt = 0;
		}
	}
	
	// ��BIE��ŵĴ���
	if(m_nBIENoAckCnt>0)
	{
		// SOE�õ���Ӧ��
		if( ((dwAckCount+0x8000-m_nBIENewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nBIENewTxSeq)%0x8000)<=IEC104_K)
		{
			for(int i=0;i<m_nBIENoAckCnt;i++)
				DB_IncBIERead();
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv bie ack! AckCnt=%d",m_nBIENoAckCnt);
#endif
			m_nBIENoAckCnt = 0;
		}
	}

	// ��AIE��ŵĴ���
	if(m_nAIENoAckCnt>0)
	{
		// AIE�õ���Ӧ��
		if( ((dwAckCount+0x8000-m_nAIENewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nAIENewTxSeq)%0x8000)<=IEC104_K)
		{
			for(int i=0;i<m_nAIENoAckCnt;i++)
				DB_IncAIERead();
	
#ifdef S104_DEBUG			 
			ReportMsg("S104:	Recv AIE ack! AckCnt=%d",m_nAIENoAckCnt);
#endif
			m_nAIENoAckCnt = 0;
		}
	}
		

	// �Ը澯������ŵĴ���
	if(m_nALMNoAckCnt>0)
	{
		// �澯����õ���Ӧ��
		if( ((dwAckCount+0x8000-m_nALMNewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nALMNewTxSeq)%0x8000)<=IEC104_K)
		{
			// ����澯����
			for(int i=0;i<m_nALMNoAckCnt;i++)
				DB_IncRelayAlarmReportRead();

#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv alm report ack! AckCnt=%d",m_nALMNoAckCnt);
#endif
			m_nALMNoAckCnt = 0;
		}
	}

	// �Զ���������ŵĴ���
	if(m_nACTNoAckCnt>0)
	{
		// ��������õ���Ӧ��
		if( ((dwAckCount+0x8000-m_nACTNewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nACTNewTxSeq)%0x8000)<=IEC104_K)
		{
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv act report ack! AckCnt=%d",m_nACTNoAckCnt);
#endif
			
			// �����������
			if(m_nLeftActionReportNum != 0)
			{
				// �ֱ���û�з�����
				for(int i=0;i<m_nACTNoAckCnt-1;i++)
					DB_IncRelayActionReportRead();
				m_nACTNoAckCnt = 1;
			}
			else
			{
				// �ֱ���ȫ��������
				for(int i=0;i<m_nACTNoAckCnt;i++)
					DB_IncRelayActionReportRead();
				m_nACTNoAckCnt = 0;
			}

		}
	}
}

void CGm104::ClearAllEvent()
{
	WORD Num,i; 
	
	m_nLeftActionReportNum = 0;
	m_pNextRelayEvent = NULL;
	m_nALMNoAckCnt = 0;
	m_nBIENoAckCnt = 0;
	m_nSOENoAckCnt = 0;
	m_nAIENoAckCnt = 0;
	
	SetFlag(SF_HAVEREPORT,FLAG_OFF);		
	SetFlag(SF_HAVEBIE,FLAG_OFF);		
	SetFlag(SF_HAVESOE,FLAG_OFF);		

	DB_GetBIENum(&Num);
	for(i=0;i<Num;i++)
		DB_IncBIERead();

	DB_GetSOENum(&Num);
	for(i=0;i<Num;i++)
		DB_IncSOERead();
	
	DB_GetRelayAlarmReportNum(&Num);
	for(i=0;i<Num;i++)
		DB_IncRelayAlarmReportRead();

	DB_GetRelayActionReportNum(&Num);
	for(i=0;i<Num;i++)
		DB_IncRelayActionReportRead();

	return;
}

#endif
