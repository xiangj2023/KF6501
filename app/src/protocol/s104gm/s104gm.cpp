//=======================================================================================
// 名称: Siec104.cpp
// 功能: S104从站规约的代码实现
// 编写：李政 2011-12-25
// 改写：
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
	ON_SM_NOTIFYETHLINKED(CGm104)  //以太网建立连接
	ON_SM_NOTIFYETHCLOSED(CGm104)  //以太网关闭连接
END_MESSAGE_MAP()

BOOL  IsTimeNew(TAbsTime time1,TAbsTime time2);
void  MergeConfigWord(float *pSet);
void  SplitConfigWord(float *pSet);

//=========================================================================
// 函数功能: 实现从站104规约的应用层调度管理任务 
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

	// 检修压板投入时清除事件
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

// 从未读取的报告中选择最老的一条
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
	
	// 读告警报告
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
	
	// 读动作报告
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

	// 无有效报告直接返回
	if(!bAlarmValid&&!bStartValid&&!bActionValid)
		return FALSE;

	// 比较告警和动作
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
	// 刷新旧遥测值
#if(SYSTEM_TYPE == _SYS_KF1300_)
	for(int i=0;i<DB_GetRMNum();i++)
		DB_ReadRM(i,&m_pOldAI[i]);
#else	
	for(int i=0;i<DB_GetAINum();i++)
		DB_ReadSecondAI(i,&m_pOldAI[i]);
#endif	
	
}

BOOL CGm104::SearchChangedAI(WORD *pAINo,long *pAIValue, DWORD ChangedValue, WORD wControlWord)//查找变化遥测
{
	long 	NewAIValue;
	long 	OldAIValue;
	DWORD 	AIChangedValue=0;	
	WORD 	AINo,nAINum;
	WORD 	AICount = 0;//记录本次查找的遥测个数
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
// 函数名称: CGm104::Init 
// 函数功能: 实现S104规约的初始化 
//=========================================================================
BOOL CGm104::Init(void)
{
	//延时3秒，防止系统复位后，还未刷新数据，规约便向外发送
	Sleep(3000);

	if(CProtocol::Init() != TRUE)
		return FALSE;

	m_Txd.ReadPtr=m_Txd.WritePtr=0;
	m_Txd.Buf = GetTxdBuf();
	pTxdFm = (TS104TxdFm *)m_Txd.Buf; 

	ResetVar();
	
	StartMsg(MCC_ReadComm, 6);  	//需要通道读消息
	StartMsg(MCC_CommIdle,200); 	//启动CommIdle消息   200ms

	// 缓存旧遥测数据
#if(SYSTEM_TYPE == _SYS_KF1300_)
	m_pOldAI = (long *)malloc(DB_GetRMNum()*sizeof(long));
#else
	m_pOldAI = (long *)malloc(DB_GetAINum()*sizeof(long));
#endif

	// 缓存定值数据
	m_LocalSTNum = DB_GetSETNum();//定值个数，配置字需要拆分成WORD
	m_ST.Buf = (BYTE *)malloc(sizeof(float)*(m_LocalSTNum+(DB_GetSWNum()+31)/32));

	//创建基本定时器
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

//初始化所有全局變量
BOOL CGm104::ResetVar(void)
{
	int i=0;
	
	BufWritePtr = 0;
	BufAckPtr = 0;

	m_dwTxdCount = 0;  //发送帧记数归零
	m_dwRxdCount = 0;  //接收帧记数归零

	// 清除所有发送定时器
	for(i=0;i<MAX_TIMERNUM;i++)
	{
		MyKillTimer(i);
	}

	// 清除所有发送缓冲区
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
	
	//清除缓冲满标志
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
		// 一帧超时，全部清除
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
		//发送S帧
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
		//发送U帧
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
// 函数功能: 发送数据帧推进缓冲区保存   
// 输入参数: 无
// 返回值:   无
//=========================================================================
BOOL CGm104::SaveDataBuf() 
{
	//没有空数组项，无法保存
	if(GetFlag(SF_BUFFLOWOVER))
    	return FALSE; 
	
	//继续填充
	m_TxDataBuf[BufWritePtr].nTxdSeqNum = m_dwTxdCount;//保存发送序号
	m_TxDataBuf[BufWritePtr].DataLen = m_Txd.WritePtr;   //保存发送指针
	memcpy(m_TxDataBuf[BufWritePtr].DataBuf,m_Txd.Buf, m_Txd.WritePtr);

	MySetTimer(BufWritePtr, T1);

	//递增写指针
   	BufWritePtr = (BufWritePtr+1)%IEC104_K;
	
	//到达待应答指针，满了
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
	m_Txd.WritePtr =  m_TxDataBuf[bItem].DataLen;//记录缓冲区长度
 
	WriteComm(m_Txd.WritePtr,DB_GetAddress(),0);
}

BOOL CGm104::KillPrevFrame(WORD dwTxdNum)
{       
	int 	i,j;
	BYTE	NoAckNum,AckNo;

	NoAckNum = (BufWritePtr+IEC104_K-BufAckPtr)%IEC104_K;
	
	//从待应答指针开始搜索
	for(i=0;i<NoAckNum; i++)
    {
    	if(m_TxDataBuf[(BufAckPtr+i)%IEC104_K].nTxdSeqNum == dwTxdNum)//找到发送序号
    		break;
    }

	//搜索到当前写指针仍未找到
	/*
						   R W		
						   | |
		|-------------------------------------------|
		|-|-|-|-|-|-|-|-|-|-| |-|-|-|-|-|-|-|-|-|-|-|
		|-------------------------------------------|
	*/
	if(i==NoAckNum)
		return FALSE;

	//找到发送帧
	for(j=0;j<=i;j++)
	{
		AckNo = (BufAckPtr+j)%IEC104_K;
		m_TxDataBuf[AckNo].nTxdSeqNum = -1;//置空标志
		m_TxDataBuf[AckNo].DataLen = 0;
		MyKillTimer(AckNo);
	}

	//递增至下一个未应答序号
	BufAckPtr = (BufAckPtr+i+1)%IEC104_K;

	//清除缓冲满标志
	SetFlag(SF_BUFFLOWOVER,FLAG_OFF);
		
	return TRUE;
}

BOOL CGm104::SetBO(TBO* pBO,BYTE NeedReturnFlag)//遥控设置函数
{
	BYTE Mode;
	WORD SwitchNo;

	//保存当前遥控信息
	//如果是遥控预置，保存遥控信息；如果不是预置，Attrib为上次保存的Attrib

	m_BO = *pBO; 	
	m_BO.NeedRetFlag = NeedReturnFlag;  //是否需等待遥控返校
	//m_BO.Flag = PBO_BUSY;  //遥控忙标志

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

BOOL CGm104::SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag)//遥控设置函数
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
		
	// 转换成系统时标
	pTime->Year = (strFileName[0]-'0')*10+(strFileName[1]-'0');
	pTime->Month = (strFileName[2]-'0')*10+(strFileName[3]-'0');
	pTime->Day = (strFileName[4]-'0')*10+(strFileName[5]-'0');
	pTime->Hour = (strFileName[6]-'0')*10+(strFileName[7]-'0');
	pTime->Minute = (strFileName[8]-'0')*10+(strFileName[9]-'0');
	pTime->Second = (strFileName[10]-'0')*10+(strFileName[11]-'0');
	pTime->MSecond = (strFileName[12]-'0')*100+(strFileName[13]-'0')*10+(strFileName[14]-'0');
		
	return TRUE;
}

// 初始化录波目录表
BOOL CGm104::InitDistDir()
{
	BYTE			*pFileData;
	char			*pFileName;
	BYTE			nHeadPtr,nTailPtr,nMax,nCnt;
	TDistIndex		*pDistList;
	DWORD			dwFileLength,dwRtnLength;
	STATUS			rc;

	pFileData = (BYTE *)g_pFileDownBuffer;
	
	// 1。打开录波索引文件
	dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*DIST_CAPACITY_MAX;
	memset(pFileData,0x00,dwFileLength);
	
	rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwFileLength != dwRtnLength))
	{
		// 索引文件出错
		LogError("Seic104::IinitDistDir",FILE_LINE,"\n Error on open DistList.ind!");
		return FALSE;
	}
			
	// 2。提取录波文件
	pDistList = (TDistIndex *)pFileData;
	pFileData += sizeof(TDistIndex);

	nMax = pDistList->nMax;
	nTailPtr = pDistList->nTailIndex;
	// 如果写指针对应的位置文件有效，从写指针开始读取	
	pFileName = (char *)pFileData+nTailPtr*DIST_INDEX_NAMELEN;
	if(strstr(pFileName,"INDEX_") != NULL)
		nHeadPtr = nTailPtr;
	else
		nHeadPtr = pDistList->nHeadIndex;

	nCnt = 0;
	// 依次读取录波文件
	do
	{
		pFileName = (char *)pFileData+nHeadPtr*DIST_INDEX_NAMELEN;

		// 只处理合法文件名
		if(pFileName = strstr(pFileName,"INDEX_"))
		{
			pFileName += 6;
			
			// 记录文件名
			memcpy(m_DistDir.strFileNameList[nCnt],pFileName,DIST_INDEX_NAMELEN);

			// 记录文件时间
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

// 初始化录波文件信息
BOOL CGm104::InitDistFile(BYTE nFileName)
{
	BYTE	nFileNo = (nFileName>>4)&0x0F;
	char 	*strFileName;
	char	strFileALLName[DIST_INDEX_NAMELEN];
	DWORD	dwCfgFlieLen,dwDataFileLen,dwRtnLength;
	STATUS			rc;

	m_DistDir.bStatus &= (~S104_FILESATUS_INITFILE);
	
	// 初始化目录
	m_DistFile.nSubDir = nFileNo;	// 文件名字		

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

	// 获取配置文件长度
	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".CFG");
	if(!FS_GetFileLength(strFileALLName,&dwCfgFlieLen))
		return FALSE;

	// 获取数据文件长度
	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".DAT");
	if(!FS_GetFileLength(strFileALLName,&dwDataFileLen))
		return FALSE;

	// 开辟文件缓冲区
	if(m_DistFile.pFileBuf != NULL)
		OS_MemFree(m_DistFile.pFileBuf);
		
	m_DistFile.pFileBuf = (BYTE *)OS_MemAlloc(dwCfgFlieLen+dwDataFileLen);
	if( m_DistFile.pFileBuf == NULL )
	{
		ReportMsg("CGm104::IinitDistFile() File %s buffer malloc error!",strFileName);
		return FALSE;
	}

	// 读取文件
	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".CFG");
	rc = FS_ReadFile(strFileALLName,0,dwCfgFlieLen,m_DistFile.pFileBuf,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwCfgFlieLen != dwRtnLength))
	{
		// 索引文件出错
		LogError("Seic104::InitDistFile",FILE_LINE,"\n Error on open file %s.CFG!",strFileALLName);
		return FALSE;
	}

	strcpy(strFileALLName,strFileName);
	strcat(strFileALLName,".DAT");
	rc = FS_ReadFile(strFileALLName,0,dwDataFileLen,m_DistFile.pFileBuf+dwCfgFlieLen,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwDataFileLen != dwRtnLength))
	{
		// 索引文件出错
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

	m_DistFile.nSecName = 0;;	// 节名字
	m_DistFile.nSegNo = 0; 	// 段号
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

	// 计算总节数
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
	
	// 计算总节数
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
	//定时器计数	
	m_dwTaskRunCount++;

	// 定时器操作
	MyCheckTimer();
        	
//	if((m_dwTaskRunCount%3) == 0)
//		SetFlag(SF_SendChangedAI);

	if(m_NoRxdCounter > 0)
	{
		// 倒计时
		m_NoRxdCounter--;

		// 通信失败时报错
		if(m_NoRxdCounter == 0)
		{
			RegisterCommStatus(COMM_STATUS_BREAK);

			bIsSTARTDT = FALSE;
			
			// 清除SOE发送
			m_nSOENoAckCnt = 0;
			m_nBIENoAckCnt = 0;
			m_nALMNoAckCnt = 0;
			m_nACTNoAckCnt = 0;
			m_nAIENoAckCnt = 0;
		}
	}

	// 找遗漏的新SOE
	if(GetNewBieNum() > 0)
		SetFlag(SF_HAVEBIE);
	
	// 找遗漏的新报告
	if(HaveNewReport())
		SetFlag(SF_HAVEREPORT);

	// 找遗漏的新SOE
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

//遥控预置返校消息映射
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
//  函数功能: 接收处理
//==============================================
void CGm104::OnFrame(BYTE* pBuf)
{
	//处理通信状态
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	
	m_NoRxdCounter = MAX_COMMERR_TIME;

	MySetTimer(T3_TIMERID,T3); //取消T3定时器

    // 处理接收
    pRxdFm=(TS104RxdFm *)pBuf;
	
	MyKillTimer(T1_TIMERID); 	//终止T1定时器
    MySetTimer(T3_TIMERID,T3); 	//复位T3定时器
    
    RxdAPCIInfo(); //接收控制域
    
    return;
}


//=========================================================================
// 函数功能: 检查Buf处是否是一个有效报文
// 输入参数: Buf:要检查的报文起始地址
//           Len:Buf的有效长度
// 返回值: 
//          FM_OK +实际报文长度 //检测到一个完整的帧
//          FM_ERR +已处理字节数 //检测到一个校验错误的帧
//          FM_LESS+已处理字节数 //检测到一个不完整的帧（还未收齐）
// 注:已处理字节数若为0则下次搜索从下一地址开始,否则下次搜索将跳过已处理过
//      的字节,从而加快搜索速度
//=========================================================================
DWORD CGm104::SearchOneFrame(PBYTE Buf,WORD Len)
{
    unsigned short FrameLen;

    pRxdFm=(TS104RxdFm *)Buf;
	
    if(pRxdFm->Start != STARTHEAD)
	    return FM_ERR|1;	 
	
    FrameLen=pRxdFm->Length; //帧长度

    if(FrameLen+2 > Len) //接收缓冲区的字节数小于帧长度，继续接收
		return FM_LESS;
    
    FrameLen += 2; //获得整帧报文长度
		 
    if(FrameLen > MAX_RXDFM_SIZE) //接收数据大于最大帧长
		return FM_ERR|1;
    
    if(FrameLen < MIN_RXDFM_SIZE) //接收数据小于最小帧长
		return FM_LESS;

	if(FrameLen > 6)		 		
	   if(pRxdFm->Address_L != DB_GetAddress())
			return FM_ERR|FrameLen;
	   
    return FM_OK|FrameLen; //返回整帧长度
}

BOOL CGm104::Handle_I_Format() //处理I格式
{  
	WORD dwAckCount=0;                                   

	m_dwRxdCount = (MAKEWORD(pRxdFm->Control1,pRxdFm->Control2)>>1);
	dwAckCount = MAKEWORD(pRxdFm->Control3,pRxdFm->Control4)>>1;
	
#ifdef S104_DEBUG			 
	ReportMsg("S104:    Recv I_FRAME--Send=%d ACK=%d!",m_dwRxdCount,dwAckCount);
#endif	

	ClearSentEvent(dwAckCount);

	// 清除之前发送帧
	dwAckCount = (dwAckCount+MAXSEQUENCER-1)%MAXSEQUENCER;
	KillPrevFrame(dwAckCount);

	// 计算下一帧发送的ACK
	m_dwRxdCount = (m_dwRxdCount+1)%MAXSEQUENCER;

	// 启动T2计时器	
	MySetTimer(T2_TIMERID, T2);

	// 如果发送缓冲区满，不再处理
	if(GetFlag(SF_BUFFLOWOVER))
		return FALSE;		
	
    return Rxd68();
}

BOOL CGm104::Handle_U_Format() //处理U格式
{
	BYTE Control = 0;
	
	switch(pRxdFm->Control1)
	{
		case STARTDT_ACT: //帧记数归零
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
	
	//组U帧应答
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

BOOL CGm104::Handle_S_Format() //处理S格式
{
	WORD dwAckCount;

	dwAckCount = MAKEWORD(pRxdFm->Control3,pRxdFm->Control4)>>1;
#ifdef S104_DEBUG			 
		ReportMsg("S104:    Recv S_FRAME--ACK=%d!",dwAckCount);
#endif

	ClearSentEvent(dwAckCount);

	// 清除之前发送帧
	dwAckCount = (dwAckCount+MAXSEQUENCER-1)%MAXSEQUENCER;
	KillPrevFrame(dwAckCount);
							
	// 收到即回复，不需延时
	TxdMonitor();
			
	return TRUE;
}

//处理控制域
BOOL CGm104::RxdAPCIInfo()
{
	unsigned char Control;
	Control = pRxdFm->Control1;
        
	switch(Control&GETBIT2)
    { //判断控制域格式
		case 0x3:Handle_U_Format();break; //处理U格式报文
		case 0x1:Handle_S_Format();break; //处理S格式报文
	    default: Handle_I_Format();break; //缺省处理I格式报文
    }  
	return TRUE;
}

BOOL CGm104::Rxd68(void)
{
	switch(pRxdFm->Style) //类型标志符
	{
		case 0x2D:  break;//45 单命令
		case 0x2E:  //46 双命令
		case 0x2F:
		case 0x3B:
		case 0x3C:  Rxd68_2E2F3B3C();  break; //带时标的遥控,104规约ASDU
		case 0x64:  Rxd68_64(); break;//总召唤/召唤某一组数据,100

		case 0x65:  Rxd68_65(); break;//召唤电度,101
		case 0x66:  Rxd68_66(); break;//读数据处理,102
		case 0x67:  Rxd68_67(); break;//时钟同步,103
		case 0x68:  Rxd68_68(); break; //测试链路,104

		case 0x69:  Rxd68_69(); break; //复位RTU,105
		case 0x6B:  Rxd68_6B(); break; //带时标的测试命令
		case 0x6E:  Rxd68_6E(); break; //设置参数,110
		case 145:	Rxd_145(); break;	//类型标识145：通用分类数据

		case 122:	Rxd_122(); break;	//文件类型标识122：F_SC_NA_1召唤目录, 选择文件, 召唤文件,召唤节
		case 124:	Rxd_124(); break;	//类型标识124：F_AF_NA_1 认可文件, 认可节(图88)
	}

	// 收到即回复，不需延时
	TxdMonitor();
			
	return TRUE;
}

BOOL CGm104::Rxd68_68(void)//测试链路
{
	return Txd68_68();
}

BOOL CGm104::Rxd68_6B(void)//带时标测试链路
{
	return Txd68_6B();
}

BOOL CGm104::Rxd68_6E(void)//设置参数
{
	return Txd68_6E();
}

BOOL CGm104::Rxd68_65_5(void)//召唤某组电度
{
	return Txd68_65_5();
}

BOOL CGm104::Rxd68_65_6(void)//召唤所有电度
{
    WORD FlagNo;
	
	for(FlagNo=SF_CIGRP1; FlagNo<=SF_ALLCISTOP; FlagNo++)
		SetFlag(FlagNo);
	
	m_S104Info.TxdIdleTxdFm = TITF_CallAllCI;

	return Txd68_65_7();	
}

BOOL CGm104::Rxd68_65(void)//召唤电度
{
	switch(pRxdFm->Reason_L)
	{
		case 5:  Rxd68_65_5(); break;  //召唤某组电度
        case 6:  Rxd68_65_6(); break;  //召唤所有电度
	}
	return TRUE;
}

BOOL CGm104::Rxd68_67(void)//时钟同步
{
	PBYTE pData=&pRxdFm->Data;
	TSysTime SysTime;
//	TAbsTime AbsTime;
	WORD MSecond;
	
	pData+=3;  //指向时间信息处
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

	Txd68_67();//对钟的确认
	return TRUE;
}

BOOL CGm104::Rxd68_64(void)//总召唤/召唤某一组数据
{
	WORD  GroupNo;
	PBYTE Buf=(PBYTE)&(pRxdFm->Start);

	//QOI
	if(Buf[15]==0x14) //总召唤
	{
		Rxd68_64_14(); 
		return TRUE;
	}
	
	GroupNo=Buf[15]-0x15; //召唤某一组数据

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


BOOL CGm104::Rxd68_64_14(void)//处理总召唤命令
{
	switch(pRxdFm->Reason_L)//传送原因
	{
	case 6:	//总召唤激活
		Rxd68_64_14_6();

		FreshOldAI();
		
		break;

	case 8://停止激活
		for(WORD SFlag=SF_BIGRP1;SFlag<=SF_ALLSTOP;SFlag++)
			SetFlag(SFlag,FLAG_OFF);
		break;
	}
	
	return TRUE;
}

BOOL CGm104::Rxd68_64_14_6(void)//处理总召唤激活命令
{
	m_S104Info.TxdIdleTxdFm = TITF_CallAll;

	//检修模式总召唤取消
	if(!GetMaintSWStatus())
		for(WORD SFlag=SF_BIGRP1;SFlag<=SF_ALLSTOP;SFlag++)
			SetFlag(SFlag);
	else
		SetFlag(SF_ALLSTOP);
	
	return Txd68_64_14_6();
}

BOOL CGm104::Rxd68_66(void)//读数据处理
{
   PBYTE pData;
   
   SetFlag(SF_CallData);   //当前处理召唤数据状态
   pData=&pRxdFm->Data;
   m_S104Info.CallDataAddr=MAKEWORD(pData[0],pData[1]);

   return TRUE;
}

BOOL CGm104::Rxd68_2E2F3B3C(void)//遥控处理
{  
	switch(pRxdFm->Reason_L)//传送原因
	{
		case 6:	Rxd68_2E2F3B3C_6(); return TRUE;//遥控预置/执行处理
		case 8: Rxd68_2E2F3B3C_8(); return TRUE;//停止激活
	}
	return TRUE;
}

BOOL CGm104::Rxd68_2E2F3B3C_6(void)//遥控预置/执行命令
{
	PBYTE 	pData=&pRxdFm->Data;
	WORD  	DCO;  //遥控命令限定词
	WORD  	BONo; //遥控路号
	DWORD 	Command;
	WORD 	Attrib;
	TBO		BO;
	
	BONo=MAKEWORD(pData[0],pData[1]);
	DCO=pData[3];

	switch(DCO&0x03)
	{
	case 0: case 3: return FALSE;
	case 1: Attrib=BO_OPEN;  break; //分
	case 2: Attrib=BO_CLOSE; break; //合
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
	case 0:	//执行
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

BOOL CGm104::Rxd68_2E2F3B3C_8(void)//遥控撤销命令
{   
	SetBO(PBO_CANCEL);
}

//=======================================================================================
// 函数名称: Rxd68_69
// 函数参数：void 
// 函数返回：bool 处理正确？1是，0不是
// 函数功能: 处理状态复位指令 
// 函数编写：赵宪文 2004-06-23
// 函数改写：
//=======================================================================================
BOOL CGm104::Rxd68_69(void)
{
	Txd68_69_Ack();

	//复位帧计数
	m_dwTxdCount = 0;  //发送帧记数归零
    m_dwRxdCount = 0;  //接收帧记数归零

	return TRUE;
}

//145 103的通用分类命令 type:21
//召唤定值区、召唤定值
//=======================================================================================
// 函数名称: Rxd_145
// 函数参数：void 
// 函数返回：处理正确？TRUE否则FALSE
// 函数功能: 召唤定值区、召唤定值
// 函数编写：lz 2011-11-26
// 函数改写：
//=======================================================================================
BOOL CGm104::Rxd_145(void)
{
	PBYTE 			pData=&pRxdFm->Data;
	TGenData		*pGenData;
	BYTE 			bINF;

	bINF = pData[5];
	//通用数据区指针
	pGenData = (TGenData *)&pData[7];

	switch(bINF)
	{
		case INF_M_GEN_RGROUPVALUE:	// 241   //读一个组的全部条目的值或属性
			if(pGenData->Gin.byGroup == 0)
			{
				if(pGenData->Gin.byEntry == 1)
					Txd68_145_STSReadEcho();   //取定值区号应答 
			}
			else if(pGenData->Gin.byGroup > 0 && pGenData->Gin.byEntry == 0)
				Rxd68_145_STRead();   //取定值应答
			break;

		case INF_M_GEN_WENTRYACK:		//249	带确认的写条目
			m_ST.Command = 0;
		
			if( pGenData->Gin.byGroup == 00  && pGenData->Gin.byEntry == 0x01)			 
				Txd68_145_STSSelectEcho();	//修改定值区号预置 
			else 
				Rxd68_145_STSelect();		//修改定值and软压板预置 
			break;
		
		case INF_M_GEN_WENTRYEXEC:		//250	带执行的写条目
		case INF_M_GEN_WENTRYCANCEL:	//251	写条目终止
			if( pGenData->Gin.byGroup == 00  && pGenData->Gin.byEntry == 0x01)			 
				Txd68_145_STSExecEcho();	//修改（定值区号）执行/撤销   
			else
				Txd68_145_STExecEcho(); 	//修改定值执行/撤销
			break;
		default:
			break;
	}		

	return TRUE;
}

//=======================================================================================
// 函数名称: Rxd68_145_2
// 函数参数：void 
// 函数返回：bool 处理正确？1是，0不是
// 函数功能: 取定值
// 函数编写：赵宪文 2004-06-23
// 函数改写：
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
		// 0x00代表当前定值区		
		if(pGenData->byGid == 0x00)
			m_ST.Group = DB_GetCurrentSETAreaNo();
		else
			m_ST.Group = pGenData->byGid-1;
			
		m_ST.No = SET_VALUE_ALL;
		m_ST.Offset = 0;
		m_ST.Num = m_LocalSTNum+((DB_GetSWNum()+31)/32);
		memset(m_ST.Buf,0,m_ST.Num*4);

		// 1 先读取全部软压板状态
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
		
		// 2 再取定值
		rc = DB_GetSETFloatValue(m_ST.Group, m_ST.No,pfValue+((DB_GetSWNum()+31)/32));
		if(rc != DB_ERR_OK)
		{
			m_ST.Result = 1;
			break;
		}
		
		// 所有浮点数字序翻转
		for(int i=0; i<m_ST.Num; i++)
			pfValue[i] = _ConvertFloat(pfValue[i]);

		m_ST.Result = 0;
	}while(0);
	
	SetFlag(SF_STSend);
	
    return TRUE;
}

//=======================================================================================
// 函数名称: Rxd68_147_STSelect
// 函数参数：void 
// 函数返回：bool 处理正确？1是，0不是
// 函数功能: 整区修改定值预置
// 函数编写：lz 2011-12-30
// 函数改写：
//=======================================================================================
BOOL CGm104::Rxd68_145_STSelect(void)
{
	PBYTE		pData=&pRxdFm->Data;
	TGenData	*pGenData = (TGenData *)&pData[7];;   //增加该指针的初始化，yanzhh 2014年4月14日 11:08:45
	BYTE 		Num	= pGenData->Ngd.Number;				//该帧定值个数
	PBYTE 		pdzData	= (PBYTE)&pGenData->Gin;		//定值元素开始地址
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
		
		m_ST.Num = m_LocalSTNum+((DB_GetSWNum()+31)/32);//加上一个软压板
		m_ST.Offset = 0;
	}
	else if(pGenData->Gin.byEntry != m_ST.Offset)
	{
		m_ST.Result = DB_ERR_NO;
		SetFlag(SF_STSelectEcho);
		return FALSE;
	}
	
	//逐个填充定值
	for(int i = 0; i < Num; i++)
	{
		pDZe = (TGenItem*)pdzData;
		memcpy(pfValue+(m_ST.Offset+i),&pDZe->byGid,4);
		pdzData += sizeof(TGenItem) + 4 - 1;
	}

	m_ST.Offset += Num;//当前收到的定值个数

	//没有后续帧
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
		// 所有浮点数字序翻转
		for(int i=((DB_GetSWNum()+31)/32); i<m_ST.Num; i++)
			pfValue[i] = _ConvertFloat(pfValue[i]);

		// 先写软压板
		rc = DB_SetAllSWStatus((BYTE *)pfValue);
		if(rc != DB_ERR_OK)
		{
			m_ST.Result = rc;
			LogError("Rxd68_147_STSelect",FILE_LINE,"Select SW error!");
			return FALSE;
		}

		// 再写入定值
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

//召唤目录或者文件
BOOL CGm104::Rxd_122(void)
{
	PBYTE 	pData=&pRxdFm->Data;
	WORD	nFileName;
	BYTE	nSecName;

	m_DistDir.dwInfoAddr = MAKEDWORD(pData[0],pData[1],pData[2],0);
	
	nFileName = MAKEWORD(pData[3],pData[4]);
	nSecName = pData[5];
		
	if(pRxdFm->Reason_L == 5)	//召唤目录
	{
#ifdef S104_DEBUG			 
		ReportMsg("S104:    Rxd_122!! Call dir!!!");
#endif
		if((nFileName&0xFF) == 0xFF)
		{
			// 读全目录
			InitDistDir();
			SetFlag(SF_CALLDIR);
		}
		else
		{
			// 读单个目录
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
		case 1: //选择文件
			// 初始化录波文件
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Select file %x!!!",nFileName);
#endif
			if(SelectFile(nFileName))
				// 发送文件就绪
				Txd_120_RDSJ_F_FR(TRUE);
			else
				// 发送文件就绪
				Txd_120_RDSJ_F_FR(FALSE);
				
			break;
			
		case 2: //请求文件
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Call file %x!!!",nFileName);
#endif
			if(!(m_DistDir.bStatus&(S104_FILESATUS_SELFILE)))
			{
				LogError("Rxd_122",FILE_LINE,"S104:	Rxd_122!! Call file before select!!!");
				return FALSE;
			}
			
			// 选择第一节
			SelectSec(0);
			Txd_121_RDSJ_F_SR();
			break;
			
		case 5: //选择节
			// 初始化发送段号
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_122!! Select sec %d!!!",nSecName);
#endif
			if(SelectSec(nSecName))
				Txd_121_RDSJ_F_SR();
			break;
			
		case 6: //请求节
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
		
		case 3: //仃止激活文件
		case 4: //删除文件
		case 7: //仃止激活节
		default:
			break;			
		}
	}

	return TRUE;
}

BOOL CGm104::Rxd_124(void)	//确认文件
{
	PBYTE 	pData=&pRxdFm->Data;
	WORD	nFileName;
	BYTE	nSecName;

	nFileName = MAKEWORD(pData[3],pData[4]);
	nSecName = pData[5];
	
	switch(pData[6] & 0x0f) //
	{
		case 1: //文件传输的肯定认可
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! File%x(%s) confirm OK!\r\n\r\n",nFileName,m_DistDir.strFileNameList[m_DistFile.nFileNo]);
#endif
			break;

		case 2: //文件传输的否定认可
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! File%x(%s) confirm ERR!\r\n\r\n",nFileName,m_DistDir.strFileNameList[m_DistFile.nFileNo]);
#endif
			break;

		case 3: //节传输的肯定认可
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! Sec%d confirm OK!\r\n\r\n",nSecName);
#endif
			// 最后的节
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

		case 4: //节传输的否定认可
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Rxd_124_File!! Sec%d confirm ERR!\r\n\r\n",nSecName);
#endif
			break;

		default:
			break;			
	}
	
	switch((pData[6] & 0xf0) >> 4)	//
	{
		case 1: //无所请求的存储空间
			break;
		case 2: //校验和错
			break;
		case 3: //非所期望的通信服务
			break;
		case 4: //非所期望的文件名称
			break;
		case 5: //非所期望的节名称
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
		return FALSE; //没有空数组项，无法保存
	}

	//初始化完成
	if(CheckClearFlag(SF_INIT_OVER))
	{
		if(Txd_InitOver())
			return TRUE;
	}

	//初始化完成
	if(CheckClearFlag(SF_BOEND))
	{
		return Txd68_2E2F3B3C(0x0A);
	}

	//发送一级数据 
	if(Txd_Class1())
		return TRUE;

	//处理总召
	switch(m_S104Info.TxdIdleTxdFm)
	{
	case TITF_CallAll:   
		if(TxdCallAll())
		{
			return TRUE;  
		}
		break;//总召唤应答
	case TITF_CallAllCI: 
		if(TxdCallAllCI())
			return TRUE;  
		break;//总召唤电度应答
	default:	
		break;//总召唤电度应答
	}

	//发送读定值返回
	if(GetFlag(SF_STSend))
	{
		if(Txd68_145_STReadEcho())	
			return TRUE;
	}

	//发送预置定值返回
	if(CheckClearFlag(SF_STSelectEcho))
	{
		if(Txd68_145_STSelectEcho())	
			return TRUE;
	}

//===========================录波文件查询 Start========================
	//召唤所有目录
	if(CheckClearFlag(SF_SENDDIR))
	{
		if(Txd_126_RDSJ_DIR_Auto())
			return TRUE;
	}
	
	//召唤所有目录
	if(GetFlag(SF_CALLDIR))
	{
		if(Txd_126_RDSJ_DIR_AllDir())
			return TRUE;
	}

	//发送段
	if(GetFlag(SF_SENDSEC))
	{
		if(Txd_125_RDSJ_F_SG())
			return TRUE;
	}

	//最后的段、节 
	if(GetFlag(SF_LASTSEG))
	{
		if(Txd_123_RDSJ_F_LS())
			return TRUE;
	}

	//主动上传扰动数据的目录	

//===========================录波文件查询 End========================

	//发送二级数据
	if(Txd_Class2()) //发送变化数据 
		return TRUE;

	return FALSE;
}

//处理总召唤遥测、遥信
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

//处理总召唤电度
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

BOOL CGm104::Txd68_68(void)//测试链路
{
BYTE Style=STARTHEAD,Reason=0x87;
BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0xAA; 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x55; 
	Txd_Tail(Num);
	
	return TRUE;
}

BOOL CGm104::Txd68_6B(void)//测试链路
{
	BYTE 	Style=0x6B,Reason=0x87;
	BYTE 	Num=1;
	PBYTE 	pData=&pRxdFm->Data;
	TSysTime sysTime;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //信息体地址 add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[0]; //返回测试命令次数
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[1]; //返回测试命令测试 

	ReadSystemTime(&sysTime);

	Txd68_LongTime(&sysTime); //加7个字节时间戳

	Txd_Tail(Num);
	
	return TRUE;
}

BOOL CGm104::Txd68_6E(void)//设置参数
{
	BYTE 	Style=0x6E,Reason=7;
	BYTE 	Num=1;
	PBYTE 	pData=&pRxdFm->Data;
	
	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[0]; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[1]; //信息体地址Hi  
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[2];; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[3]; //QPM
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[4]; //QPM
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[5]; //QPM
	Txd_Tail(Num);
	return TRUE;
}


BOOL CGm104::Txd_BurstBI(void) //发送变化遥信
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
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(event.wNo+BI_ADDR_START); //信息体地址Lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(event.wNo+BI_ADDR_START); //信息体地址Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = (event.bStatus==DB_BI_CLOSE)?1:0;
		
		// 记录个数
		m_nBIENoAckCnt++;
		// 记录发送序号
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


BOOL CGm104::SearchClass1(void) //检查有无1级数据
{
	// 遥信变位数据
	if(GetFlag(SF_HAVEBIE))
		return TRUE;

	//  有保护报告
	if(GetFlag(SF_HAVEREPORT))
		return TRUE;

	return FALSE; //无1级数据返回FALSE
}

BOOL CGm104::Txd_Soe(void) //发送SOE数据
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
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(BINo);  //点号L
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(BINo);  //点号H
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = (event.bStatus == DB_BI_CLOSE)?1:0;  //状态

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

		// 记录个数
		m_nSOENoAckCnt++;
		// 记录发送序号
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

BOOL CGm104::Txd_ChangeAI(void) //发送变化遥测数据
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
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo+AI_ADDR_START); //信息体地址Lo //AINo+0x701
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo+AI_ADDR_START); //信息体地址Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //地址add by xyb
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[0]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[1]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[2]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[3]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //遥测值品质
	}

	if(AITxdNum==0)
		    return FALSE;                  
		    
	Txd_Tail(AITxdNum); //变化数据SQ=0

#else
	BYTE Style=35,Reason=3;//传输原因突发
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
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo+AI_ADDR_START); //信息体地址Lo //AINo+0x701
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo+AI_ADDR_START); //信息体地址Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //地址add by xyb
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AIValue); //遥测值 Lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AIValue); //遥测值 Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //遥测值品质
	
		ReadSystemTime(&sysTime);

		Txd68_LongTime(&sysTime); //加7个字节时间戳
	}
	
	if(AITxdNum==0)
			return FALSE;				   
			
	Txd_Tail(AITxdNum); //变化数据SQ=0
#endif
	return TRUE;
	
}

BOOL CGm104::Txd_Class1(void) //发送1级数据
{
	// 遥信变位数据
	if(GetFlag(SF_HAVEBIE))
	{
		if(Txd_BurstBI())
			return TRUE;
		else
			SetFlag(SF_HAVEBIE,FLAG_OFF);
	}
	
	//  有保护报告
	if(GetFlag(SF_HAVEREPORT))
	{
		if(Txd_RelayEvent())
			return TRUE;
		else
			SetFlag(SF_HAVEREPORT,FLAG_OFF);
	}

    return FALSE;
}


BOOL CGm104::Txd_Class2(void) //发送2级数据
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

    return FALSE;//无2级数据返回FALSE
}


BOOL CGm104::Txd68_65_5(void)//召唤某组电度
{
	WORD 	GroupNo;
	PBYTE 	pData=&pRxdFm->Data;

	GroupNo = pData[3]&0x0F-1;
	
	return Txd_CIGroup(GroupNo,GroupNo+38);
}

BOOL CGm104::Txd68_65_7(void)//召唤所有电度的确认
{
	Txd_Head(0x65,7);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Hi     
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x45; //COI
	Txd_Tail(1);
	return TRUE;
}

BOOL CGm104::Txd_AllCIStop(void)//召唤所有电度的确认
{
	Txd_Head(0x65,10);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x45; //COI
	Txd_Tail(1);
	return TRUE;
}

BOOL CGm104::Txd_CIGroup(WORD GroupNo,BYTE Reason)//发送一组电度
{
	DWORD CINo,CITxdNum,CIValue;

	CINo=GroupNo*GRP_CINUM;
	if(CINo>=DB_GetCINum())
		return FALSE;
	
	Txd_Head(0x0F,Reason);

	for(CITxdNum=0;CITxdNum<GRP_CINUM&&CINo<DB_GetCINum(); CINo++,CITxdNum++)
	{
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(CINo+CI_ADDR_START); //信息体地址
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(CINo+CI_ADDR_START); //信息体地址
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
		DB_ReadCI(CINo,&CIValue);
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HHBYTE(CIValue); //
		m_Txd.Buf[ m_Txd.WritePtr++ ] = CITxdNum+1; //顺序号
	}

	Txd_Tail(CITxdNum); //SQ=0
	return TRUE;
}

BOOL CGm104::Txd68_67(void)//对钟的确认
{
BYTE Style=0x67,Reason=7;
BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Hi     
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	PBYTE pData=&pRxdFm->Data;
	pData+=3;
	for(WORD i=0;i<7;i++)
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pData[i]; //
	Txd_Tail(Num);
	return TRUE;
}

BOOL CGm104::Txd_AIGroup(WORD GroupNo,BYTE Reason)//发送一组遥测  OK
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
		                               
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[0]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[1]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[2]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[3]; //遥测值
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //遥测值品质
	}

	//VSQ,连续
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
									   
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(lValue); //遥测值 Lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(lValue); //遥测值 Hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;//带品质描述
	}
	
	//VSQ,连续
	Txd_Tail(AITxdNum|0x80);
#endif
	
	return TRUE;
}

BOOL CGm104::Txd_BIGroup(WORD GroupNo,BYTE Reason)//发送一组遥信
{
	WORD 	BINo,BITxdNum;
	BYTE	BIStatus;
	
	BINo = GroupNo*GRP_BINUM;
	if(BINo>=DB_GetBINum())
		return FALSE;
	
	Txd_Head(1,Reason);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(BINo+BI_ADDR_START); //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(BINo+BI_ADDR_START); //信息体地址Hi
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

BOOL CGm104::Txd_AllStop(void)//发送总召唤结束帧
{
	BYTE Style=0x64,Reason=0x0A;
	BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x14; //COI
	Txd_Tail(Num);
	return TRUE;
}

BOOL CGm104::Txd68_64_14_6(void)//处理总召唤激活命令
{
	BYTE Style=0x64,Reason=7;
	BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x14; //COI
	Txd_Tail(Num);
	return TRUE;
}

//=======================================================================================
// 函数功能: 发送3个字节的短时标
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
// 函数功能: 发送7个字节的长时标
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
// 函数功能: 处理报文的头部
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
	
	m_Txd.WritePtr=(PBYTE)&pTxdFm->Data-(PBYTE)&pTxdFm->Start; //指针减法

	return TRUE;
}
//=======================================================================================
// 函数功能: 处理报文的尾部
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
    MySetTimer(T3_TIMERID,T3); 	//复位T3定时器
	
	return TRUE;
}


BOOL CGm104::Txd68_66_AI(WORD AINo)
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	float	fValue;
	BYTE	*pValue;
	
	Txd_Head(13,5);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo); //LOBYTE(AINo+0x701)信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo); //HIBYTE(AINo+0x701)信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	DB_ReadFloatRM(AINo,&fValue);
	fValue = _ConvertFloat(fValue);
	pValue = (BYTE *)&fValue;

	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[0]; //遥测值
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[1]; //遥测值
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[2]; //遥测值
	m_Txd.Buf[ m_Txd.WritePtr++ ] = pValue[3]; //遥测值
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //遥测值品质

	Txd_Tail(1);
#else	
	long AIValue;

	Txd_Head(11,5);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AINo); //LOBYTE(AINo+0x701)信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AINo); //HIBYTE(AINo+0x701)信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb

	DB_ReadSecondAI(AINo,&AIValue);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(AIValue); //遥测值 Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(AIValue); //遥测值 Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //品质描述
	Txd_Tail(1);
#endif	
	return TRUE;
}
	
BOOL CGm104::Txd68_66_BI(WORD BINo)
{
	BYTE BIStatus;
	
	Txd_Head(1,5);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(BINo+1); //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(BINo+1); //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	DB_ReadBI(BINo,&BIStatus);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = (BIStatus == DB_BI_CLOSE)?1:0;
    m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;//限定词 
   	Txd_Tail(1);
	return TRUE;
}
BOOL CGm104::Txd68_66_CI(WORD CINo)
{
	DWORD CIValue;

	Txd_Head(15,5);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(CINo); //LOBYTE(CINo+0xc01)信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(CINo); //HIBYTE(CINo+0xc01)信息体地址Hi
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

BOOL CGm104::Txd68_2E2F3B3C(BYTE Reason) //发送遥控预置/执行	
{
	TSysTime	sysTime;
	
	if(m_BO.RetResult!=PBO_SUCESS)
		Txd_Head(m_BO.Info[0],47);
	else
		Txd_Head(m_BO.Info[0],Reason);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[1]; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[2]; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[3]; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_BO.Info[4]; //DCO

	if(!Is101BO)
	{
		ReadSystemTime(&sysTime);
		Txd68_LongTime(&sysTime); //加7个字节时间戳
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
// 函数功能: 子站初始化完成
// 输入参数: 无
// 返回值:   bool初始化完成
//	注：
//=========================================================================

BOOL CGm104::Txd_InitOver(void)
{
	BYTE Style=0x46,Reason=4;
	BYTE Num=1;

	Txd_Head(Style,Reason);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 00; //信息体地址Hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //add by xyb
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x01; //COI
	Txd_Tail(Num);
	return TRUE;
}

//=========================================================================
// 函数功能: 保护动作和状态告警 高压事故简报
// 输入参数: 无
// 返回值:	 BOOL
//	注： 20110418 加
//=========================================================================
BOOL CGm104::Txd_RelayEvent(void)
{
	TAbsTime		AbsTime;
	TSysTime 		SysTime;
	WORD	   		MSecond;  
	BYTE 			Num=0;
	BYTE			FD_Num;
	BYTE			*pFD_Data;
	
	// 没有剩余报告时
	if((m_nLeftActionReportNum == 0)||(m_pNextRelayEvent == NULL))
	{
		m_pNextRelayEvent = NULL;
		m_nLeftActionReportNum = 0;
		
		// 3.再按照时间顺序逐个读取报告
		if(!SearchNewReport(m_tCurrentReport,m_CurrentReportType))
			return FALSE;
		
		if(m_CurrentReportType == DB_EVENT_RACT)
		{
			// 初始化报告指针
			m_pNextRelayEvent = &m_tCurrentReport.tAction.tEvent.tAction;
			m_nLeftActionReportNum = m_tCurrentReport.tAction.tEvent.Num;
	
#ifdef S104_DEBUG
			LogError("Txd68_Report_61",FILE_LINE,"Send a new action report!!!");
#endif			
			// 直接递增指针
			//DB_IncRelayActionReportRead();
			m_nACTNoAckCnt++;
		}
		else if(m_CurrentReportType == DB_EVENT_RALM)
		{
			// 初始化报告指针
			m_pNextRelayEvent = &m_tCurrentReport.tAlram.tEvent.tAction;
			m_nLeftActionReportNum = 1;
	
#ifdef S104_DEBUG
			LogError("Txd68_Report_61",FILE_LINE,"Send a new alarm report!!!");
#endif
			// 直接递增指针
			//DB_IncRelayAlarmReportRead();			
			m_nALMNoAckCnt++;
		}
		else
		{
			LogError("Txd68_Report_61",FILE_LINE,"Search event type error!!!");
			return FALSE;
		}
		
	}
	
	// 无效的数据直接剔除
	if(m_pNextRelayEvent->FD_Num > MAX_FD_NUM)
	{
		m_pNextRelayEvent = NULL;	
		m_nLeftActionReportNum = 0;
		LogError("Txd68_Report_61",FILE_LINE,"Invalid event report!!!");
		return FALSE;
	}

	// 2.先读取动作缓存报告
	Txd_Head(140,3);
			
	// 填充保护动作
	Txd_ElementAddr(0);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_pNextRelayEvent->nINF); //INF
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_pNextRelayEvent->nINF); //INF

	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_pNextRelayEvent->bStatus + 1;  //合2，分1
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//相对时间低8位 m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//相对时间低8位
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//相对时间高8位

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_wFaultNo);  //故障序号低8位
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_wFaultNo);  //故障序号高8位
	
	m_wFaultNo ++;
	m_wFaultNo %= 0xffff; 
	
	AbsTime.Lo = m_pNextRelayEvent->AbsTimeLo;
	AbsTime.Hi = m_pNextRelayEvent->AbsTimeHi;
	AbsTimeTo(&AbsTime,&SysTime);
	MSecond = SysTime.MSecond+SysTime.Second*1000;

	m_Txd.Buf[m_Txd.WritePtr++] = LOLOBYTE(MSecond);	//最好改成7个字节的时间
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
		FD_Num = MAX_IND8D_FDNUM;	//故障电量个数
		LogError("Txd_RelayEvent",FILE_LINE,"Report FD num is too many!!! FD_Num=%d",m_pNextRelayEvent->FD_Num);
	}

	m_Txd.Buf[m_Txd.WritePtr++] = FD_Num;	//故障电量个数

	pFD_Data = (BYTE *)(m_pNextRelayEvent+1);
	// 填充索引号和故障电量
	for(int i = 0; i<FD_Num; i++,pFD_Data += 6)
	{
		m_Txd.Buf[m_Txd.WritePtr++] = pFD_Data[0];  //故障电量索引号
		m_Txd.Buf[m_Txd.WritePtr++] = pFD_Data[1];
	
		m_Txd.Buf[m_Txd.WritePtr++] = GDD_DATATYP_R32_23; //故障电量数据类型
	
		//故障电量值是float类型，用四个字节传送
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

	// 递减剩余分报告个数
	m_nLeftActionReportNum--;

	// 计算下一个份报告指针
	m_pNextRelayEvent = (TRelayEventHead *)((BYTE *)(m_pNextRelayEvent+1)+m_pNextRelayEvent->FD_Num*6);
		
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
///////////////***************************************************************************8

//取定值区号返回响应
//103 typ = 10
//=======================================================================================
// 函数名称: Txd68_145_STSReadEcho
// 函数参数：void
// 函数返回：bool
// 函数功能:  取定值区号返回响应
// 函数编写：赵宪文 2004-06-23
// 函数改写：
// 其他：	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STSReadEcho(void)
{
	PBYTE 		pData=&pTxdFm->Data;
	TGenData	*pGenData;
	BYTE 		Num=1;        //可变结构限定词

	pGenData = (TGenData *)&pData[7];

	Txd_Head(145,5);

	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_RGROUPVALUE;	//INF 241   //读一个组的全部条目的值或属性;
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;					//RII

	pGenData->Ngd.Number = 1;
	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;

	if(m_ST.Group >= 4)		//返回错误	数据个数为0
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
		
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//数据类型为无符号整数
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		pGenData->byGid			= DB_GetCurrentSETAreaNo()+ 1;

		m_Txd.WritePtr += sizeof(TGenData);
	}

	Txd_Tail(Num);  

	return TRUE;
}

//=======================================================================================
// 函数名称: Txd68_145_STSReadEcho
// 函数参数：void
// 函数返回：bool
// 函数功能:  修改定值区号预置返回
// 函数编写：赵宪文 2004-06-23
// 函数改写：
// 其他：	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STSSelectEcho(void)
{
	PBYTE 		pRxdData = &pRxdFm->Data;
	PBYTE 		pTxdData = &pTxdFm->Data;
	TGenItem	*pelemData;
	TGenData	*pGenData;
	STATUS		rc = DB_ERR_OK;
	
	//定值区号选择
	pelemData = (TGenItem *)&pRxdData[8];
	m_ST.Group = pelemData->byGid-1;
	
	pGenData = (TGenData *)&pTxdData[7];
		
	rc = DB_SetCurrentSETAreaNo(m_ST.Group);

	Txd_Head(145,5);
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_WENTRYACK;	//INF    249   带确认的写条目;	   
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;					//RII


	pGenData->Ngd.Number = 1;
	pGenData->Ngd.Count = 0;
	pGenData->Ngd.Cont = 0;

	if(rc != DB_ERR_OK)		//返回错误	数据个数为0
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
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//数据类型为无符号整数
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		pGenData->byGid	= m_ST.Group+1;
		m_Txd.WritePtr += sizeof(TGenData);
		Txd_Tail(1);	
	}

	return TRUE;
}

//=======================================================================================
// 函数名称: Txd68_145_STSExecEcho
// 函数参数：
// 函数返回：
// 函数功能:  修改（定值区号）执行/撤销返回
// 函数编写：lz 2011-12-30
// 函数改写：
//=======================================================================================
BOOL CGm104::Txd68_145_STSExecEcho(void)
{//修改（定值区号）执行/撤销返回
	PBYTE 		pRxdData = &pRxdFm->Data;
	PBYTE 		pTxdData = &pTxdFm->Data;
	TGenData	*pGenData;
	BYTE 		bINF;
	STATUS		rc;

	bINF = pRxdData[5];
	
	if(bINF == INF_M_GEN_WENTRYEXEC)
	{//执行
		rc = DB_SetCurrentSETAreaNoConfirm(m_ST.Group,DB_EXECUTE);
	}
	else if(bINF == INF_M_GEN_WENTRYCANCEL)
	{//撤销
		rc = DB_SetCurrentSETAreaNoConfirm(m_ST.Group,DB_CANCEL);
	}	

	pGenData = (TGenData *)&pTxdData[7];

	if(rc == DB_ERR_OK)
		Txd_Head(145,5);
	else
		Txd_Head(145,0x45);
		
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = bINF;	//INF 250   带执行的写条目;	
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
		
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//数据类型为无符号整数
		pGenData->Gdd.byDataSize = 1;
		pGenData->Gdd.byDataNum = 1;
		
		pGenData->byGid	= m_ST.Group+1;

		m_Txd.WritePtr += sizeof(TGenData);
		Txd_Tail(1);  
	}

	return TRUE;
}



//=======================================================================================
// 函数名称: Txd68_145_STSExecEcho
// 函数参数：void
// 函数返回：bool
// 函数功能:  取定值返回
// 函数编写：lz 2011-12-30
// 函数改写：
//=======================================================================================
BOOL CGm104::Txd68_145_STReadEcho(void)
{
	PBYTE 	pData=&pTxdFm->Data;
	TNgd	*Ngd;
	BOOL  	bisLastFrm = FALSE;
	BYTE 	Num=(MAX_TXDFRAME_SIZE - (sizeof(TS104TxdFm)-1+8)) / (sizeof(TGenItem)-1+4);        //可变结构限定词

	Ngd = (TNgd *)&pData[7];

	if(Num + m_ST.Offset >= m_ST.Num)
	{
		Num = m_ST.Num - m_ST.Offset;
		SetFlag(SF_STSend,FLAG_OFF);
		bisLastFrm = TRUE;
	}
	
	Txd_Head(145,5);
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_RGROUPVALUE;  //INF	241   读一个组的全部条目的值或属性
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;		//RII

	Ngd->Number = Num;
	Ngd->Count = 0;
	Ngd->Cont = 1;
	if(bisLastFrm)
		Ngd->Cont = 0;

	if(m_ST.Result != 0)//返回错误
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
		PBYTE pdzData	= (PBYTE)&pData[7] + sizeof(TNgd);			//定值元素开始地址
		PBYTE ptmp = pdzData;
		TGenItem* pDZe;

		for(int i = 0; i < Num; i++)
		{
			pDZe = (TGenItem*)pdzData;

			pDZe->Gin.byGroup = m_ST.Group + 1;		//定值组号从1开始
			pDZe->Gin.byEntry = m_ST.Offset+ i;		//定值条目从0开始 
			pDZe->byKod = 0;
			
			if((m_ST.Offset+ i) < ((DB_GetSWNum()+31)/32+(DB_GetCFGNum()+31)/32))
			{
				pDZe->Gdd.byDataType = m_S104para.intsetcode;	// 数据类型整形
				pDZe->Gdd.byDataSize = 4;
				pDZe->Gdd.byDataNum = 0x81; //后续状态位
			}
			else
			{
				pDZe->Gdd.byDataType = GDD_DATATYP_R32_23;	// 数据类型 7 为
				pDZe->Gdd.byDataSize = 4;
				pDZe->Gdd.byDataNum = 0x81;	//后续状态位
			}
			
			memcpy(&pDZe->byGid,m_ST.Buf+(m_ST.Offset + i)*sizeof(float),4);

			pdzData += sizeof(TGenItem)-1+4;
		}

		if(bisLastFrm)
		{
			pDZe->Gdd.byDataNum &= 0x7F;		//后续状态位 = 0;
			SetFlag(SF_STSend,FLAG_OFF);
		}
		
		m_ST.Offset += Num;
		m_Txd.WritePtr += pdzData - ptmp + 1;
	}
	
	Txd_Tail(Num);  

	return TRUE;
}


//=======================================================================================
// 函数名称: Txd68_145_STSelectEcho
// 函数参数：void
// 函数返回：bool
// 函数功能:  整区修改定值预置返回
// 函数编写：赵宪文 2004-06-23
// 函数改写：
// 其他：	 103 typ = 10
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
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = INF_M_GEN_WENTRYACK; 		//INF    249   带确认的写条目;
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
			
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//数据类型为无符号整数
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
// 函数名称: Txd68_145_STExecEcho
// 函数参数：void
// 函数返回：bool
// 函数功能:  修改定值执行/撤销返回
// 函数编写：赵宪文 2004-06-23
// 函数改写：
// 其他：	 103 typ = 10
//=======================================================================================
BOOL CGm104::Txd68_145_STExecEcho(void)
{
	PBYTE 		pData=&pTxdFm->Data;
	TGenData	*pGenData;
	BYTE 		bINF;
	STATUS		rc = DB_ERR_OK;

	bINF = *(&pRxdFm->Data+5);
	//通用数据区指针
	pGenData = (TGenData *)&pData[7];
	
	if(bINF == INF_M_GEN_WENTRYEXEC)
	{//执行
		rc = DB_SetAllSWStatusConfirm(DB_EXECUTE);
		rc |= DB_SetSETValueConfirm(m_ST.Group,SET_VALUE_ALL,DB_EXECUTE);
	}
	else if(bINF == INF_M_GEN_WENTRYCANCEL)
	{//撤销
		rc = DB_SetAllSWStatusConfirm(DB_CANCEL);
		rc |= DB_SetSETValueConfirm(m_ST.Group,SET_VALUE_ALL,DB_CANCEL);
	}

	if(rc != DB_ERR_OK)
		Txd_Head(145,0x45);
	else
		Txd_Head(145,0x05);
		
	Txd_ElementAddr(0);
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()&0xFF; //装置地址UI16
	m_Txd.Buf[ m_Txd.WritePtr++ ] = DB_GetAddress()>>8;	 

	m_Txd.Buf[ m_Txd.WritePtr++ ] = bINF;//INF 250   //带执行的写条目;
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
		pGenData->Gdd.byDataType = GDD_DATATYP_UI;		//数据类型为无符号整数
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
	BYTE	Reason = 5;		// 突发
	BYTE    Num = 0;
			
	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	for(int i=m_DistDir.nSentCnt;i<m_DistDir.nFileNum;i++,Num++)
	{
		if(m_Txd.WritePtr +  2 + 3 + 1 + 7 > MAXFRMLEN)
			break;

		m_Txd.Buf[ m_Txd.WritePtr++ ] = m_DistDir.nSentCnt<<4;		//file name hi  每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len 
		
		if (m_DistDir.nSentCnt == (m_DistDir.nFileNum-1))
			m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x60;	//FOR=0,
		else
			m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x40;	//FOR=0,
				
		Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistDir.nSentCnt]); //加7个字节长时标
		m_DistDir.nSentCnt ++;
	}

	// 目录发送完清除标志
	if (m_DistDir.nSentCnt >= m_DistDir.nFileNum)
		SetFlag(SF_CALLDIR,FLAG_OFF);
	
	Txd_Tail(Num); 

#ifdef S104_DEBUG			 
	ReportMsg("S104:    发送总召目录！ Num =%d  Sent=%d Total=%d\n",Num,m_DistDir.nSentCnt,m_DistDir.nFileNum);
#endif

	return TRUE;
}

BOOL CGm104::Txd_126_RDSJ_DIR_SubDir()
{
	BYTE	Style = 126;
	BYTE	Reason = 3;		// 突发
			
	Txd_Head(Style,Reason);
	
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = (m_DistFile.nSubDir<<4)+1;		//file name LO( CFG )
 	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name HI 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(m_DistFile.dwDataLen[0]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(m_DistFile.dwDataLen[0]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(m_DistFile.dwDataLen[0]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x80;	//FOR=0,		
	Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistFile.nSubDir]); //加7个字节长时标

	m_Txd.Buf[ m_Txd.WritePtr++ ] = (m_DistFile.nSubDir<<4)+2;		//file name LO( CFG )
 	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name HI 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LLBYTE(m_DistFile.dwDataLen[1]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LHBYTE(m_DistFile.dwDataLen[1]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HLBYTE(m_DistFile.dwDataLen[1]);	//file len
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0xA0;	//FOR=0,		
	Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistFile.nSubDir]); //加7个字节长时标

	Txd_Tail(2); 

#ifdef S104_DEBUG			 
	ReportMsg("S104:    发送单个目录文件列表！ FileNo=%x\n",m_DistFile.nSubDir);
#endif

	return TRUE;
}

BOOL CGm104::Txd_126_RDSJ_DIR_Auto(void)
{
	BYTE	Style = 126;
	BYTE	Reason = 3;		// 突发
			
	Txd_Head(Style,Reason);
	
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = (m_DistDir.nFileNum-1)<<4;	//file name lo  每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file name hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len hi
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//file len 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x60;	//FOR=0,
				
	Txd68_LongTime(&m_DistDir.tFileTimeList[m_DistDir.nFileNum-1]); //加7个字节长时标

	Txd_Tail(1); 

#ifdef S104_DEBUG			 
	ReportMsg("S104:    发送变化目录！ FileNo=%d\n",m_DistDir.nFileNum-1);
#endif

	return TRUE;
}

BOOL	CGm104::Txd_120_RDSJ_F_FR(BOOL bReady)	//文件准备就绪	
{
	BYTE	Style = 120;
	BYTE	Reason= 13;		//文件传输
	WORD	wFileName;
	
	Txd_Head(Style,Reason);
	
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	if(bReady)
	{
		wFileName = m_DistFile.nFileName;		
			
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(wFileName);	//file name hi	每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(wFileName);	//file name lo
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOLOBYTE(m_DistFile.dwFileLen); //file len lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOHIBYTE(m_DistFile.dwFileLen); //file len hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HILOBYTE(m_DistFile.dwFileLen); //file len 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0;	//文件准备就绪限定词(FRQ)
#ifdef S104_DEBUG			 
		ReportMsg("S104:	Txd_120_RDSJ_F_FR 文件准备就绪!!FINENAME = %x\r\n",wFileName);
#endif
	}
	else
	{
		wFileName = m_DistFile.nSubDir<<4;
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(wFileName);	//file name hi	每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(wFileName);	//file name lo
		
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //file len lo
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //file len hi
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0; //file len 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x80;	//文件准备就绪限定词(FRQ),出错
#ifdef S104_DEBUG			 
		ReportMsg("S104:	Txd_120_RDSJ_F_FR 文件准备异常!!FINENAME = %x\r\n",wFileName);
#endif
	}

	Txd_Tail(1);  
	
	return TRUE;
}

BOOL	CGm104::Txd_121_RDSJ_F_SR()	//节准备就绪
{
	BYTE 	Style = 121;
	BYTE	Reason= 13;		//文件传输
	BYTE	nSecNo = m_DistFile.nSecName;
	
	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_DistFile.nFileName);	//file name lo 每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_DistFile.nFileName);	//file name hi
	
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_DistFile.nSecName;	//7.2.6.34节名称(NOS) UI8

	if(nSecNo >= m_DistFile.nSecNum)
	{
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x80;	//节准备就绪限定词(SRQ) CP8{UI7[1..7],BS1[8]}
	}
	else
	{
		// 计算节长度
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOLOBYTE(m_DistFile.dwSecLen);	//节的长度(LOF) UI24[1..24]<0..16777215> 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOHIBYTE(m_DistFile.dwSecLen);	//节的长度 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = HILOBYTE(m_DistFile.dwSecLen);	//节的长度 
		m_Txd.Buf[ m_Txd.WritePtr++ ] = 0x00;	//节准备就绪限定词(SRQ) CP8{UI7[1..7],BS1[8]}
	}
	
	Txd_Tail(1);	

#ifdef S104_DEBUG			 
	ReportMsg("S104:    Txd_121_RDSJ_F_SR 节准备就绪!!secNAME = %d\n",m_DistFile.nSecName);
#endif

	return TRUE;
}

BOOL	CGm104::Txd_125_RDSJ_F_SG(void)//上传扰动数据 类型标识125：F_SG_NA_1 段
{
	DWORD	len;			//段的长度			
	BYTE 	*pData ;		//段的数据		
	BYTE	Style = 125;
	BYTE	Reason=13;		//文件传输
	BYTE	nSecNo = m_DistFile.nSecName;
	DWORD	dwDataOff;
	
	dwDataOff = m_DistFile.nSegNo*PERSEGDATALEN;

	pData = m_DistFile.pSecStart+dwDataOff;

	// 判断最后一段
	if(m_DistFile.nSegNo == (m_DistFile.nSegNum-1))
	{
		len = m_DistFile.dwSecLen-(m_DistFile.nSegNo*PERSEGDATALEN);

		SetFlag(SF_SENDSEC,FLAG_OFF);
		SetFlag(SF_LASTSEG);

		// 如果还是最后一节
		if(nSecNo == (m_DistFile.nSecNum-1))
			SetFlag(SF_LASTSEC);
	}
	else
	{
		len = PERSEGDATALEN;
	}
	

	Txd_Head(Style,Reason);
	Txd_ElementAddr(m_DistDir.dwInfoAddr);

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_DistFile.nFileName);	//file name hi  每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_DistFile.nFileName);	//file name lo

	m_Txd.Buf[ m_Txd.WritePtr++ ] = nSecNo;	//7.2.6.34节名称(NOS) UI8

	m_Txd.Buf[ m_Txd.WritePtr++ ] = len;	//段的长度(LOF) UI24[1..24]<0..16777215> 

	memcpy(&m_Txd.Buf[ m_Txd.WritePtr], pData,len);
	m_Txd.WritePtr += len;

	m_DistFile.nSegNo++;

	Txd_Tail(1);  

#ifdef S104_DEBUG			 
	ReportMsg("S104:    Txd_125_RDSJ_F_SG 段!!secNAME = %d SegNo= %d\n",nSecNo,m_DistFile.nSegNo);
#endif

	return TRUE;
}


BOOL	CGm104::Txd_123_RDSJ_F_LS(void)	//最后的节,最后的段
{
	BYTE bLsq = 0,chs;
	BYTE Style = 123;
	BYTE Reason=13;		//文件传输

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

	m_Txd.Buf[ m_Txd.WritePtr++ ] = LOBYTE(m_DistFile.nFileName);	//file name hi  每个模块可以保存8组路波数据 0~7，0号位最新的路波数据
	m_Txd.Buf[ m_Txd.WritePtr++ ] = HIBYTE(m_DistFile.nFileName);	//file name lo
	m_Txd.Buf[ m_Txd.WritePtr++ ] = m_DistFile.nSecName;	//7.2.6.34节名称(NOS) UI8
	m_Txd.Buf[ m_Txd.WritePtr++ ] = bLsq;	//LSQ＝最后的节,最后的段限定词(在7.2.6.31中定 
	m_Txd.Buf[ m_Txd.WritePtr++ ] = chs;	//CHS＝校验和(在7.2.6.37中定义) 

	Txd_Tail(1);  

#ifdef S104_DEBUG			 
	if(bLsq == 1)
		ReportMsg("S104:    Txd_123_RDSJ_F_LS 最后的节!!secNAME = %d\n",m_DistFile.nSecName);
	else
		ReportMsg("S104:    Txd_123_RDSJ_F_LS 最后的段!!secNAME = %d\n",m_DistFile.nSecName);
#endif

	return TRUE;

}

DWORD CGm104::GetNewSoeNum(void)
{
	WORD num=0;
	
	// 检修压板投入时清除事件
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return 0;
	}
	
	::DB_GetSOENum(&num); 

	//已经出现了其他分流
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

	// 检修压板投入时清除事件
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return 0;
	}
	
	::DB_GetBIENum(&num);
	
	//已经出现了其他分流
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
	
	// 检修压板投入时清除事件
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return 0;
	}
	
	::DB_GetAIENum(&num); 

	//已经出现了其他分流
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
	// 对SOE序号的处理
	if(m_nSOENoAckCnt>0)
	{
		// SOE得到了应答
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
	
	// 对BIE序号的处理
	if(m_nBIENoAckCnt>0)
	{
		// SOE得到了应答
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

	// 对AIE序号的处理
	if(m_nAIENoAckCnt>0)
	{
		// AIE得到了应答
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
		

	// 对告警报告序号的处理
	if(m_nALMNoAckCnt>0)
	{
		// 告警报告得到了应答
		if( ((dwAckCount+0x8000-m_nALMNewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nALMNewTxSeq)%0x8000)<=IEC104_K)
		{
			// 清除告警报告
			for(int i=0;i<m_nALMNoAckCnt;i++)
				DB_IncRelayAlarmReportRead();

#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv alm report ack! AckCnt=%d",m_nALMNoAckCnt);
#endif
			m_nALMNoAckCnt = 0;
		}
	}

	// 对动作报告序号的处理
	if(m_nACTNoAckCnt>0)
	{
		// 动作报告得到了应答
		if( ((dwAckCount+0x8000-m_nACTNewTxSeq)%0x8000)>=1 \
			&& ((dwAckCount+0x8000-m_nACTNewTxSeq)%0x8000)<=IEC104_K)
		{
#ifdef S104_DEBUG			 
			ReportMsg("S104:    Recv act report ack! AckCnt=%d",m_nACTNoAckCnt);
#endif
			
			// 清除动作报告
			if(m_nLeftActionReportNum != 0)
			{
				// 分报告没有发送完
				for(int i=0;i<m_nACTNoAckCnt-1;i++)
					DB_IncRelayActionReportRead();
				m_nACTNoAckCnt = 1;
			}
			else
			{
				// 分报告全部发送完
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
