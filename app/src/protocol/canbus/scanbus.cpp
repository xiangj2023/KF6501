#include "protocol.h"
#include "parafunc.H"
#include "scanbus.H"
#include "db_err.h"
#include "relayfun.h"
#include "logic.h"
#include "logicdef.h"
#include "relay.h"

#if(SYSTEM_TYPE == _SYS_KF1300_)	
extern WORD* wACT1Cnt;  //����1����
extern WORD* wACT2Cnt; 	//����2����
extern WORD* wACT3Cnt; 	//����3����
#endif

extern int SysTempture;
extern BYTE IrigQuality;
extern DWORD dwRelayDataPtr;			//AD��ǰдָ��

TFixTransData	g_tTransData;
BYTE g_pFileDownBuffer[MAX_FILE_LEN];

/*--------------------------------------------------------------*/
/*��������sCanBusTask					       					*/
/*���ܣ�CANBUS��վ��Լ����				       					*/
/*����: ��						       							*/
/*�������						       							*/
/*--------------------------------------------------------------*/
void SCanbus_Task(DWORD *pdwApp)
{
	CSCanbusPro *pSCanbus = new CSCanbusPro(pdwApp);

	if(pSCanbus->Init()!=TRUE)
		pSCanbus->Exit((char *)"SCanBus Init Error");

	pSCanbus->Run();
}

BEGIN_MESSAGE_MAP(CSCanbusPro,CProtocol)
ON_SM_TIMEOUT(CSCanbusPro)
ON_SM_SOE(CSCanbusPro)
ON_SM_AIE(CSCanbusPro)
ON_SM_RELAYEVENT(CSCanbusPro)
ON_SM_BIBURST(CSCanbusPro)
ON_SM_COMMIDLE(CSCanbusPro)
ON_SM_BOSELECTECHO(CSCanbusPro) 
ON_SM_BOEXECECHO(CSCanbusPro)
ON_SM_BODIRECTECHO(CSCanbusPro)
ON_SM_COEMAKE(CSCanbusPro)
ON_SM_AUTOADJUSTECHO(CSCanbusPro)
ON_SM_TRANSOIE(CSCanbusPro)
END_MESSAGE_MAP()

BOOL CSCanbusPro::IsNetTask(void)
{
	if(m_wPortID == PORT_ID_CANA)
		return TRUE;
	if(m_wPortID == PORT_ID_CANB)
		return TRUE;
	if(m_wPortID == PORT_ID_ETHA)
		return TRUE;
	if(m_wPortID == PORT_ID_ETHB)
		return TRUE;

	return FALSE;
}

WORD CSCanbusPro::Get_MaxFrameLen()
{
    /*ȡ�˿�����*/
    BYTE portType = MAJOR_PORT(m_wPortID);
	
	switch(portType)
	{
		case SC_PORT_SERIAL:	return SER_MAXFRAMELEN;
		case SC_PORT_CANBUS:	return CAN_MAXFRAMELEN;
		case SC_PORT_ETHERNETA:	
		case SC_PORT_ETHERNETB:	return ETH_MAXFRAMELEN;
		default: break;
	}

	return SER_MAXFRAMELEN;
}

WORD CSCanbusPro::Get_CommIdle()
{
    BYTE portType = MAJOR_PORT(m_wPortID);
	
	switch(portType)
	{
		case SC_PORT_SERIAL:	return SER_COMMIDLE;
		case SC_PORT_CANBUS:	return CAN_COMMIDLE;
		case SC_PORT_ETHERNETA:	
		case SC_PORT_ETHERNETB:	return ETH_COMMIDLE;
		default: break;
	}

	return SER_COMMIDLE;
}

WORD CSCanbusPro::Get_RetryIdle()
{
    /*ȡ�˿�����*/
    BYTE portType = MAJOR_PORT(m_wPortID);
	
	switch(portType)
	{
		case SC_PORT_SERIAL:	return SER_RETRYIDLE;
		case SC_PORT_CANBUS:	return CAN_RETRYIDLE;
		case SC_PORT_ETHERNETA:	
		case SC_PORT_ETHERNETB:	return ETH_RETRYIDLE;
		default: break;
	}

	return SER_RETRYIDLE;
}
/*

WORD CSCanbusPro::Get_TimerTick()
{
    BYTE portType = MAJOR_PORT(m_wPortID);
	
	switch(portType)
	{
		case SC_PORT_SERIAL:	return SER_TIMER_TICK;
		case SC_PORT_CANBUS:	return CAN_TIMER_TICK;
		case SC_PORT_ETHERNETA:	
		case SC_PORT_ETHERNETB:	return ETH_TIMER_TICK;
		default: break;
	}

	return SER_TIMER_TICK;
}

WORD CSCanbusPro::Get_RunIdle()
{
    BYTE portType = MAJOR_PORT(m_wPortID);
	
	switch(portType)
	{
		case SC_PORT_SERIAL:	return SER_RUNIDLE;
		case SC_PORT_CANBUS:	return CAN_RUNIDLE;
		case SC_PORT_ETHERNETA:	
		case SC_PORT_ETHERNETB:	return ETH_RUNIDLE;
		default: break;
	}

	return SER_RUNIDLE;
}
*/


BOOL CSCanbusPro::Init(void)
{
	//��ʼ����Լ����
	if(CProtocol::Init() != TRUE)
		return FALSE;
	
	//-----------���ݿ�ӿ�------------------------------	
	m_wSAddr  = DB_GetDevAddress();
	
	//��ʼ������������Ϣ
	dwGBAddress = 0x003F;
	
	m_NetCfg.wAIRepTM = CAN_AIREPTM;
	m_NetCfg.wBIRepTM = CAN_BIREPTM;
	m_NetCfg.wCIRepTM = CAN_CIREPTM;
	m_NetCfg.AIBurstRepTM 	= CAN_BURSTAIREPTM;
	m_NetCfg.SoeRepTM 		= CAN_BIEREPTM;
	m_NetCfg.BIBurstRepTM 	= CAN_BIBREPTM;
	m_NetCfg.TimingTM 	= CAN_RELAYREPTM;
	m_NetCfg.SaftyRepTM 	= CAN_SAFETYREPTM;
	m_NetCfg.wLEDStatusTM = CAN_LEDREPTM;

	memset(m_wTimer,0,sizeof(WORD)*TIMER_NUM);

	// 5����һ��ƽ������
	/*
	m_wTimer[AIBURST_ID] 	= 0;
	m_wTimer[AI_ID] 	= 0;
	m_wTimer[BI_ID] 	= 0;
	m_wTimer[CI_ID] 	= 0;
	m_wTimer[SOE_ID] 	= 0;
	*/
	
	//��ʼ������ֵ
	m_dwTaskRunCount = 0;
	m_NoRxdCounter = CAN_COMMERR;
	LastAIPtr = 0;
	LastBIPtr = 0;
	LastCIPtr = 0;
	LastBurstAIPtr = 0;

	//��װ�ļ�ƫ������¼
	m_wFileOffset = 0;
	m_CurrentResSeq = APP_RESSEQ_MIN;
	m_LastReqSeq = APP_REQSEQ_MAX+1;	

	m_FileTransTimer = 0;

	m_RetryCount = 0;

	m_WrapHeadPtr = 0;
	
	//��ʼ��ң���¼������
	//-----------�������ݿ�ӿ�-----------------
	pOldAI = (SWORD *)OS_MemAlloc(sizeof(SWORD)*DB_GetAINum());
	OS_MemSet(pOldAI,0,sizeof(SWORD)*DB_GetAINum());
	
	m_RetryBuf = (PBYTE)OS_MemAlloc(MAX_FRAME_LEN);
	OS_MemSet(m_RetryBuf,0,MAX_FRAME_LEN);

	memset(&m_File,0,sizeof(TCanFile));

	//���֧��256����ֵ������
	m_SET.pSET = (float *)OS_MemAlloc(256*sizeof(float));
	
	//�������������ñ�־
/*	
	if(!IsNetTask())//if(MAJOR_PORT(m_wPortID) == SC_PORT_SERIAL)
	{
		SetFlag(SF_NeedConfig,FLAG_OFF);
		m_NetCfg.dwStatus = MASK_BO|MASK_WRITE_SET|MASK_READ_SET;
	}
	else
	{
		SetFlag(SF_NeedConfig,FLAG_ON);
		m_NetCfg.dwStatus = 0;
	}
*/
	SetFlag(SF_NeedConfig,FLAG_ON);
	m_NetCfg.dwStatus = MASK_BO|MASK_WRITE_SET|MASK_READ_SET;

	StartMsg(MCC_CommIdle,CAN_RUNIDLE);
	StartMsg(MCC_ReadComm,MIN_FRAME_LEN);	

	//����CAN������ʱ��
	SetTimer(TM_1S,CAN_TIMER_TICK);

	//����Ԥ���ռ�
	m_pMemBlock = OS_MemAlloc(2048);

	//����LED״̬�ռ�
	m_pLEDStatus = (BYTE *)OS_MemAlloc(LED_RELAY_WORD_NUM);
	
	//����LEDƵ�ʿռ�
	m_pLEDFreq = (BYTE *)OS_MemAlloc(LED_RELAY_WORD_NUM);

	m_nWatchPortDog = 0;

	m_nMaxFrameLen = Get_MaxFrameLen();
	m_nBurstIdle = Get_CommIdle();
	m_nRetryIdle = Get_RetryIdle();
		
	return TRUE;
}

/*--------------------------------------------------------------*/
/*��������	CanTxdMonitor					*/
/*���ܣ� 	CANBUS��վ��Լ���͹�����			*/
/*����: 	��						*/
/*�����	��						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::TxdMonitor()
{
	//����������ȷ��
	if(CheckClearFlag(SF_NEEDCONFIRM))
	{
		TxdConfirm();
		return;
	}
	
	//��վ����δ�õ�ȷ��
	if(GetFlag(SF_WAITCONFIRM))
	{
		memcpy(m_TxdInfo.pBuf,m_RetryBuf,m_RetryLen);
		WriteComm(m_RetryLen,m_RetryAddr,0x80000000);	
		ProcessRetryStatus();

		// ƽ�����ĸ�λ
		m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;

		return;
	}

	//�����������Ӧ��
 	if(CheckClearFlag(SF_SENDRES_SET))
 	{
		TxdReadSETRes();
		return;
 	}

 	if(CheckClearFlag(SF_SENDRES_SYSTEMSET))
 	{
		TxdReadSystemSETRes();
		return;
 	}	

 	if(CheckClearFlag(SF_SENDRES_SW))
 	{
		TxdReadSWRes();
		return;
 	}	

 	if(CheckClearFlag(SF_SENDRES_CF))
 	{
		TxdReadCFRes();
		return;
 	}	
	
	
	//����ң�ط�У
	if(GetFlag(SF_HaveBOReturn))
	{
		TxdBOReturn();
		return;
	}

	//����COE��У������
	if(CheckClearFlag(SF_MAKECOEECHO))
	{
		TxdCoeRes();
		return;
	}

	//�����������Ӧ��
 	if(CheckClearFlag(SF_SENDRES_BIATTR))
 	{
		TxdReadBIAttrRes();
		return;
 	}

	//�����������Ӧ��
 	if(CheckClearFlag(SF_SENDRES_BOATTR))
 	{
		TxdReadBOAttrRes();
		return;
 	}

	//�����������Ӧ��
 	if(CheckClearFlag(SF_SENDRES_MBOPARA))
 	{
		TxdReadMBORes();
		return;
 	}
	
	if(CheckClearFlag(SF_SENDRES_DISTDIR))
	{
		TxdReadDistDirRes();
		return;
	}

	//�յ�����֮֡ǰֻ�ܷ����б���
	if(GetFlag(SF_NeedConfig))
	{
		TxdIdleRep();
		StartMsg(MCC_CommIdle,CAN_RUNIDLE);
		return;
	}
 
	//���н�������
	if(HaveClass1Data())
	{			
		//���б�λң�ţ�����
		if(GetFlag(SF_BURSTBI))
		{
			TxdBurstBI();
			return;
		}
		
		//����SOE������
		if(GetFlag(SF_SOE))
		{
			TxdSoe();
			return;
		}

		//����SOE������
		if(GetFlag(SF_AIE))
		{
			TxdAIE();
			return;
		}

		//���б������棬����
		if(GetFlag(SF_RELAYALARM))
		{
			TxdRelayAlarmReport();
			return;
		}
		if(GetFlag(SF_RELAYSTART))
		{
			TxdRelayStartReport();
			return;
		}
		if(GetFlag(SF_RELAYACTION))
		{
			TxdRelayActionReport();
			return;
		}

#if(SYSTEM_TYPE == _SYS_KF1300_)
		if(GetFlag(SF_LEDBURST))
		{
			TxdLEDStatus(TRUE);
			return;
		}
#endif

	}
	
	//���б仯ң��
	if(CheckClearFlag(SF_BURSTAI))
	{
		if(TxdBurstAI())
			return;
	}
		
	//��ȫң������
	if(CheckClearFlag(SF_AIREPORT))
	{
		if(TxdAI())
			return;	
	}
	
	//��ȫң�ż����
	if(CheckClearFlag(SF_BIREPORT))
	{
		if(TxdBI())
			return;	
	}
	
	//��ȫ��ȼ����
	if(CheckClearFlag(SF_CIREPORT))
	{
		if(TxdCI())
			return;	
	}

	//��ƽ�����ļ����
	if(CheckClearFlag(SF_SAFEREPORT))
	{
		if(TxdSafetyRep())
			return;	
	}	

	//��ȫ
#if(SYSTEM_TYPE == _SYS_KF1300_)
	if(CheckClearFlag(SF_LEDREPORT))
	{
		if(TxdLEDStatus(FALSE))
			return; 
	}
#endif

//����ʱ���Ϳ��б���,����ֹͣIDLE��Ϣ
//	StopAndClearMsg(MCC_CommIdle);
}

/*-------------------------------------------------------------*/
/*��������CanSearchOneFrame					*/
/*���ܣ�  CANBUS��վ��Լ��֡����		       		*/
/*����:   PBYTE Buf		������ָ��			*/
/*	  WORD wLen		��Ѱ���������ȡ�	       */
/*�����  ��						       */
/*-------------------------------------------------------------*/

DWORD CSCanbusPro::SearchOneFrame(PBYTE Buf,WORD wLen)
{
	TRxdHead 		*pRxdHead=(TRxdHead *)Buf;
	WORD			wDesAddr;
	WORD			wFrmLen;
	
	//֡ͷ���
	if(pRxdHead->START!=0x68)
		return  FM_ERR;
	
	//���ȼ��
	wFrmLen = MAKEWORD(pRxdHead->LengthLo,pRxdHead->LengthHi);

	if(wFrmLen >= 2048)
		return FM_ERR|wLen;
	
	if(wFrmLen+4>wLen)
		return  FM_LESS;
		
	//��ַ���
	wDesAddr = MAKEWORD(pRxdHead->DestAddrLO,pRxdHead->DestAddrHI);
//	if(!(wDesAddr&NET_ADDR_VALID))
//		return FM_ERR;

	//ά����ַ����
	if(0xAAAA != wDesAddr)
	{
		wDesAddr &= NET_ADDR_MASK;
		
		if((m_wSAddr!=wDesAddr)&&(dwGBAddress)!=wDesAddr)
			return  FM_ERR;
	}
	
	//У����
	if(Buf[wFrmLen+3] != LPC(0,Buf,wFrmLen+3))
		return   FM_ERR;	

	return (FM_OK|(wFrmLen+4));
}

/*-------------------------------------------------------------*/
/*��������CanOnFrame					*/
/*���ܣ�  CANBUS��վ��Լ��֡����		       		*/
/*����:   PBYTE Buf		֡������ͷָ��			*/
/*�����  ��						       */
/*-------------------------------------------------------------*/
void CSCanbusPro::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	TxdMonitor();
}

/*-------------------------------------------------------------*/
/*��������CanOnFrame					*/
/*���ܣ�  CANBUS��վ��Լ��֡����		       		*/
/*����:   PBYTE Buf		֡������ͷָ��			*/
/*�����  ��						       */
/*-------------------------------------------------------------*/
void CSCanbusPro::OnFrame(PBYTE Buf)
{
	BYTE			bySeq;
	WORD        	wAddr;
	BOOL			bAddrChange = FALSE;
	
	// ע��ͨ��ͨ��״̬�ָ�
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	
	m_NoRxdCounter = CAN_COMMERR;

	// ƽ�����ĸ�λ
	m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;
	
	m_RxdHead = (TRxdHead *)Buf;
	
	//��¼����֡���
	bySeq = m_RxdHead->APPControl&APP_SEQ_MASK;
	wAddr = MAKEWORD(m_RxdHead->SourceAddrLO,m_RxdHead->SourceAddrHI);

	//���ص�Ԫ��ַ�仯ʱ�����ط�
	if(m_wDAddr != wAddr)
	{
		m_wDAddr = wAddr;
	    bAddrChange = TRUE;
	}
	
	//����ȷ��
	if(m_RxdHead->APPControl&BIT_CON)
	{
		SetFlag(SF_NEEDCONFIRM);
		m_CommandSeq = bySeq;
		m_CommandAddr = wAddr;
		TxdMonitor();
	}

	//��¼��ǰ�µ�����SEQ��
	if(m_RxdHead->FunCode != FUN_CON)
		m_CurrentReqSeq = bySeq;

	switch(m_RxdHead->FunCode)
	{
	case FUN_CON:
		if(!RxdConfirm())
			return;
		
		break;	
	//ң�ػ�ң��Ԥ�á�ִ�С�������ֱ��ִ��
	case FUN_SELECT:	
	case FUN_SELECTWITHVALUE:
	case FUN_OPERATE:
	case FUN_CANCEL:
	case FUN_DIROPERATE:
		if(bAddrChange||(m_LastReqSeq!=bySeq))
		{						
			switch(m_RxdHead->ObjType)
			{
			case DOC_BO://ң������
				YKPocess();
				break;

			case DOC_SETTING:
				TxdWriteSETRes();
				break;
				
			case DOC_SETTINGAREA:
				TxdWriteSETAreaRes();
				break;
				
			case DOC_SYSTEMSETTING:
				TxdWriteSystemSETRes();
				break;

			case DOC_SW:
				TxdWriteSWRes();
				break;

			case DOC_CF:
				TxdWriteCFRes();
				break;

			case DOC_SYSRESET:
				//��������
				switch(Buf[12])
				{
					case RESET_WARM:// ������
						DAE_ResetDevice(SYS_RESET_COLD);
						break;
					case RESET_COLD:// ������
						DAE_ResetDevice(SYS_RESET_COLD);
						break;
					case RESET_DATA:// ���ݸ�λ	
#if(SYSTEM_TYPE == _SYS_KF1300_)	
						if(DB_ResetRelayData(~((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE)|(0x01<<DB_DATA_CF))))			
#else
						if(DB_UpdateRelayData())			
#endif
							DAE_ResetDevice(SYS_RESET_COLD);
						break;

					case RECOVER_DATA:// ϵ����ʼ�� 
#if(SYSTEM_TYPE == _SYS_KF1300_)	
						DB_ResetRelayData(~((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE)|(0x01<<DB_DATA_CF)));			
#else
						DB_UpdateRelayData();	
#endif
						break;	
					case RECOVER_COEF:// ϵ����ʼ�� 
#if(SYSTEM_TYPE == _SYS_KF1300_)
						DB_ResetRelayData((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE));	
#else
						DB_UpdateCFData();
#endif
						break;
					case RECOVER_PARA:// ϵͳ������ʼ�� 
						DB_ResetBasicConfig();
						break;
						
					default:
						break;
				}
				
				break;			

			case DOC_COE:
				RxdCoe();
				break;

			default:
				break;
			}
		}
		else
			ReportMsg("Recv command retry! Type=%d",m_RxdHead->ObjType);
		break;

	//дָ�����ݶ����ֵ
	case FUN_WRITEVALUE:
		switch(m_RxdHead->ObjType)
		{
		case DOC_NETCFG://������������	
			SetFlag(SF_NeedConfig,FLAG_OFF);
//			if(IsNetTask())//if(MAJOR_PORT(m_wPortID) != SC_PORT_SERIAL)
				RxdNetCfg();
			break;
		case DOC_COE:
			RxdCoe();
			break;
		case DOC_ABSTIME:
			RxdAbsTime();	
			break;
		case DOC_VARIAL:
			TxdWriteVarRes();		
			break;
		case DOC_CF:
			TxdWriteCFRes();
			break;

		case DOC_CHANNELSRC:
			TxdWriteChannelSrcRes();
			break;
		case DOC_CHANNEL:
			TxdWriteChannelValueRes();
			break;
		case DOC_CI:
			TxdSetCIValueRes();
			break;

		case DOC_SYSCONFIG:	
			TxdWriteSysParaRes();
			break;

		case DOC_CFG:
			TxdWriteCFGRes();
			break;

		case DOC_RW://�̵�����״̬
			RxdWriteRWStatus();
			break;			

		case DOC_BREAKERTRIPCNT:
			RxdWriteBreakerTripCnt();
			break;			

		default:
			break;
		}
		
		break;
		
	case FUN_READVALUE:
		switch(m_RxdHead->ObjType)
		{
		case DOC_AI://ң��
			TxdAIRes();
			break;

		case DOC_AIWITHQUALITY://��Ʒ��������ң��
			TxdAIWithQualityRes();
			break;

		case DOC_SRM://���βౣ������ֵ
			TxdSRMRes();
			break;
			
		case DOC_BIGROUP://����ң��
			TxdBIRes();
			break;

		case DOC_BIWITHQUALITY://��Ʒ�������ĵ���ң��
			TxdBIWithQualityRes();
			break;			
						
		case DOC_CI://��ȼ���ֵ
			TxdCIRes();
			break;

		case DOC_CIWITHQUALITY://��ȼ���ֵ
			TxdCIWithQualityRes();
			break;

		case DOC_ABSTIME://����ʱ��
			TxdAbsTimeRes();
			break;
			
		case DOC_SETTING:
			RxdReadSETReq();
			break;
			
		case DOC_SETTINGAREA:
			TxdReadSETAreaRes();
			break;
			
		case DOC_SYSTEMSETTING:
			RxdReadSystemSETReq();
			break;
			
		case DOC_SW:
			RxdReadSWReq();
			break;
			
		case DOC_CF:
			RxdReadCFReq();
			break;

		case DOC_CHANNELSRC:
			TxdReadChannelSrcRes();
			break;
		
		case DOC_CHANNEL:
			TxdReadChannelValueRes();
			break;

		case DOC_SYSCONFIG:	
			TxdReadSysParaRes();
			break;

		case DOC_VERSION:	
			TxdReadVersionRes();
			break;

		case DOC_DEVSTATUS:
			TxdReadDevStatusRes();
			break;

		case DOC_CFG:
			TxdReadCFGRes();
			break;

		case DOC_COE:
			TxdReadAOCoeRes();
			break;

		case DOC_LEDSTATUS:
			TxdLEDStatusRes();
			break;

		// �˿ڱ��ļ���
		case DOC_WATCHPORT:
			TxdWatchPortRes();
			break;

		// �����쿴
		case DOC_VARIAL:
			TxdVarDataRes();
			break;

		case DOC_DEVDISP:	
			TxdReadDevDispRes();
			break;

		case DOC_RW://�̵�����״̬
			TxdReadRWStatusRes();
			break;			

		case DOC_BREAKERTRIPCNT:
			TxdReadBreakerTripCntRes();
			break;
			
		case DOC_DCSIDATA:
			TxdReadDCSIDataRes();
			break;			

		case DOC_DISTDIR:
			RxdReadDistDirReq();
			break;			
			
		case DOC_CLASS1:
			TxdReadClass1();
			break;
			
		case DOC_CLASS2:
			TxdReadClass2();
			break;
			
		default:
			break;
		}
		break;

	case FUN_READSPECIALVALUE:
		break;
		
	case FUN_QUERYLOGBYREAD:
	case FUN_QUERYLOGBYOFF:
		switch(m_RxdHead->ObjType)
		{
		case DOC_BIBURST://����仯ң��
			TxdBIBByReadRes();
			break;
		case DOC_BIEVENT://���������˳���¼
			TxdBIEByReadRes();
			break;
		case DOC_LOGERROR://�쳣��Ϣ
			TxdLogInfoRes();
			break;
		case DOC_OPERATEEVENT://�����¼�
			TxdOptEventByReadRes();
			break;
		case DOC_RELAYALARMREPORT:
			TxdRelayAlarmReportByReadRes(FALSE);
			break;
		case DOC_RELAYALARMITEM:
			TxdRelayAlarmReportByReadRes(TRUE);
			break;
		case DOC_RELAYSTARTREPORT:
			TxdRelayStartReportByReadRes(FALSE);
			break;
		case DOC_RELAYSTARTITEM:
			TxdRelayStartReportByReadRes(TRUE);
			break;
		case DOC_RELAYACTIONREPORT:
			TxdRelayActionReportByReadRes(FALSE);
			break;
		case DOC_RELAYACTIONITEM:
			TxdRelayActionReportByReadRes(TRUE);
			break;
						
		case DOC_DBIBURST://˫��仯ң��
		case DOC_DBIEVENTT://˫������˳���¼
		case DOC_SAFETY://ƽ������
		case DOC_SYSTIME://���ʱ��
		break;
		}
		break;

	case FUN_QUERYLOGBYWRITE://����дָ��Ĳ�ѯ
		switch(m_RxdHead->ObjType)
		{
		case DOC_BIBURST://����仯ң��
			TxdBIBByWriteRes();
			break;
		case DOC_BIEVENT://���������˳���¼
			TxdBIEByWriteRes();
			break;
		case DOC_OPERATEEVENT://�����¼�
			TxdOptEventByWriteRes();
			break;
		case DOC_RELAYALARMREPORT:
			TxdRelayAlarmReportByWriteRes(FALSE);
			break;
		case DOC_RELAYSTARTREPORT:
			TxdRelayStartReportByWriteRes(FALSE);
			break;
		case DOC_RELAYACTIONREPORT:
			TxdRelayActionReportByWriteRes(FALSE);
			break;

		case DOC_RELAYALARMITEM:
			TxdRelayAlarmReportByWriteRes(TRUE);
			break;
		case DOC_RELAYSTARTITEM:
			TxdRelayStartReportByWriteRes(TRUE);
			break;
		case DOC_RELAYACTIONITEM:
			TxdRelayActionReportByWriteRes(TRUE);
			break;

		case DOC_VARIAL:
			TxdVarRes();
			break;
						
		case DOC_DBIBURST://˫��仯ң��
		case DOC_DBIEVENTT://˫������˳���¼
		case DOC_SAFETY://ƽ������
		case DOC_SYSTIME://���ʱ��
		break;
		}
		break;
		
	case FUN_READFILE:
		TxdFileLoad();
		break;	
		
	case FUN_WRITEFILE:
		TxdFileDown();
		break;	
		
	case FUN_READATTRIB://��ȡָ�����ݶ������������
		switch(m_RxdHead->ObjType)
		{
		case DOC_BIBURST://����仯ң��
		case DOC_BIEVENT://���������˳���¼
		case DOC_OPERATEEVENT://�����¼�
		case DOC_RELAYALARMREPORT:
		case DOC_RELAYSTARTREPORT:
		case DOC_RELAYACTIONREPORT:
			TxdWrapAttrRes();
			break;

		case DOC_CHANNEL:
			TxdReadChannelAttrRes();
			break;
			
		case DOC_AO:	
			RxdReadAOAttrReq();
			break;

		case DOC_BIGROUP:
		case DOC_BI:
			RxdReadBIAttrReq();
			break;

		case DOC_BO:	
			RxdReadBOAttrReq();
			break;

		case DOC_MAPBITOBJ:	
			RxdReadMBOReq();
			break;

		case DOC_FLPARA:	
			TxdReadFLParaRes();
			break;

		default:
			break;
		}

		break;
		
	case FUN_WRITEATTRIB://дָ�����ݶ������������
		switch(m_RxdHead->ObjType)
		{
		case DOC_CHANNEL:
			TxdWriteChannelAttrRes();
			break;
			
		case DOC_BIGROUP:
		case DOC_BI:
			TxdWriteBIAttrRes();
			break;

		case DOC_BO:	
			TxdWriteBOAttrRes();
			break;

		case DOC_AO:	
			TxdWriteAOAttrRes();
			break;

		case DOC_MAPBITOBJ:
			TxdWriteMBORes();
			break;

		case DOC_FLPARA:
			TxdWriteFLParaRes();
			break;

		default:
			break;
		}
		break;

	case FUN_COMMONCONTROL:
		switch(m_RxdHead->ObjType)
		{
		case DOC_RELAYCTRL://����
			TxdRelayControlRes(FALSE);
			break;
		case DOC_SPECIALCMD://�������
			TxdSpecialOperateRes();
			break;
		case DOC_RELAYCTRLWITHBLOCK://�������Ĵ���
			TxdRelayControlRes(TRUE);
			break;
		case DOC_TRANSDATA:
			RxdTransData();
			break;
		default:
			break;
		}
		break;

	//-----------------�߼����̵���----------------
	case FUN_LOGICDEBUG:
		switch(m_RxdHead->ObjType)
		{
		case DEBUG_RELAY_BP://����
			Rxd_DebugSetBreakPointReq();
			break;
		case DEBUG_RELAY_CONTEXT://�������
			Rxd_DebugGetContextReq();
			break;
		case DEBUG_RELAY_STEP1:
			Rxd_DebugStep1Req();
			break;
		case DEBUG_RELAY_STEP2:
			Rxd_DebugStep2Req();
			break;
		case DEBUG_RELAY_STOP:
			Rxd_DebugStopReq();
			break;
		case DEBUG_RELAY_WATCH:
			Rxd_DebugWatchReq();
			break;
		case DEBUG_RELAY_DBIN:
			Rxd_SetDBINReq();
			break;
		case DEBUG_RELAY_DBOUT:
			Rxd_GetDBOutReq();
			break;
			
		default:
			break;
		}
		break;

	case FUN_SUBSTITUTE:
	case FUN_UNSUBSTITUTE:
		switch(m_RxdHead->ObjType)
		{
			case DOC_AI:
				Rxd_SetAIValue(m_RxdHead->FunCode);
				break;
			case DOC_BI:	
				Rxd_SetBIValue(m_RxdHead->FunCode);
				break;
			default:
				break;
		}
		break;
		
	default:
		break;
	}
	
	TxdMonitor();

	// ��¼��һ���������
	if(m_RxdHead->FunCode != FUN_CON)
	if(bySeq <= APP_REQSEQ_MAX)
		m_LastReqSeq = bySeq; 
}

/*-------------------------------------------------------------*/
/*��������CanOnTimeOut					       */
/*���ܣ�CANBUS��վ��Լ��ʱ����Ӧ����			       */
/*����: DWORD id	��ʱ��ID			       */
/*�������						       */
/*-------------------------------------------------------------*/
void CSCanbusPro::OnTimeOut(DWORD id)
{
//	DWORD			dwDataMask;
	int			i;

//	TAbsTime time;
	
	//��ʱ������	
	m_dwTaskRunCount++;

	// ��ض˿ڼ��ӿ��Ź�
	if (m_nWatchPortDog > 0)//2000.6.8
	{
		m_nWatchPortDog--;

		if(m_nWatchPortDog == 0)
		{
			SetDebugComm(0);
			ReportMsg("�رմ��ڼ���0X%x\n",m_wWatchPortID);
			m_wWatchPortID = 0;
		}
	}
	
	if((m_wPortID == PORT_ID_ETHB)&&((m_dwTaskRunCount%100) == 0))
	{
/*
		TEventBI tSOE;
		
		tSOE.wNo = (m_dwTaskRunCount/50)%10;
		
		tSOE.bStatus = ((m_dwTaskRunCount/50)%2)==1?BI_CLOSE:BI_OPEN;

		ReadAbsTime(&tSOE.stTime);
			
		DB_WriteSOE(&tSOE);

		TRelayEventItem tAlarm;
		
		tAlarm.nINF = 2;
		tAlarm.bStatus = (m_dwTaskRunCount/100)%2;
		ReadAbsTime(&tAlarm.stTime);
		tAlarm.FD_Value[0] = m_dwTaskRunCount;
		tAlarm.FD_Value[1] = m_dwTaskRunCount*2;
		DB_RelayAlarm (&tAlarm);

		long pFDList[10];

		DB_RelayStart(55, 125, NULL, TRUE);

		for(int j=0;j<10;j++)
			pFDList[j] = m_dwTaskRunCount;
		
		DB_RelayWriteFD(55, pFDList);
		DB_RelayEnd(55);
		long 				pFDList[32];
		TRelayEventItem 	tAction;
		
		for(int j=0;j<DB_GetRMNum();j++)
		{
			DB_ReadSecondRM(j,&pFDList[j]);
			DB_ReadSecondRM(j,&tAction.FD_Value[j]);
		}
		
		DB_RelayStart(55, ReportInf[0].wInf, NULL, FALSE);
		
		DB_RelayWriteFD(55, pFDList);

		tAction.nINF = ReportInf[2].wInf;
		tAction.bStatus = (m_dwTaskRunCount%100)%2;
		ReadAbsTime(&tAction.stTime);

		DB_RelayAction(55, &tAction);

		DB_RelayEnd(55);
	*/
	}


/*	//zqz-t ��ʱʹ��Ĭ������
	//��������Ƿ�ı�(ÿ20��CAN_TIMER_TICK)
	if(m_dwTaskRunCount%CAN_CHECKCFG==0)
	{ 	
		dwDataMask = DAE_GetNetBDataMask();
		if(m_NetCfg.dwStatus != dwDataMask)
		{
			AlterConfig(dwDataMask);
		}
	}
*/
/*
	if((m_dwTaskRunCount%20)==0)
	{
		for(i=0;i<384;i++)
		{
			m_TxdInfo.pBuf[i] = i%16;
		}
		m_TxdWritePtr = 384;
		WriteComm(m_TxdWritePtr,GetLayerAddr(m_wDAddr)|NET_ADDR_MASK,0);
	}
*/
	//�ļ����䳬ʱ�ж�
	if(m_File.nTimer > 0)
	{
		m_File.nTimer--;
		if(m_File.nTimer == 0)
			ClearFileTrans();
	}
	
	//��ʱδ�յ�������Ϊͨ��ʧ��
	do{
		if(m_NoRxdCounter == 0)
			break;
		
		m_NoRxdCounter--;
		
		if(m_NoRxdCounter > 0)
			break;
		
		//ע��˿�ͨ��״̬
		RegisterCommStatus(COMM_STATUS_BREAK);

		// ������ʱ��������
		if(m_wTimer[SAFETY_ID] != 0)
			SetFlag(SF_NeedConfig);

		// ����ͨ�ż��
		StartMsg(MCC_CommIdle,CAN_RUNIDLE);
		
		// �����������������Ч
		if(!IsNetTask())
			break;
		
//��������
//				m_NetCfg.dwStatus = 0;
//				if((m_wPortID == PORT_ID_CANA)||(m_wPortID == PORT_ID_ETHA))
//				{
//					��һ������ͨ������ʱ
//					if(!DAE_GetRunningFlags(SS_NETBCOMM))
//					{
//						DAE_SetNetADataMask(0);
//						DAE_SetNetBDataMask(MASK_ALL);
//					}
//				}
//				else if((m_wPortID == PORT_ID_CANB)||(m_wPortID == PORT_ID_ETHB))
//				{
//				��һ������ͨ������ʱ
//					if(!DAE_GetRunningFlags(SS_NETACOMM))
//					{
//						DAE_SetNetBDataMask(0);
//						DAE_SetNetADataMask(MASK_ALL);
//					}
//				}
	}while(1);
		
	//�ط����ݽ׶ν�ֹ��ʱ����	
	if(GetFlag(SF_WAITCONFIRM))
		return;

	for(i= 0;i<TIMER_NUM;i++)
	{
		if(m_wTimer[i] == 0)
			continue;

		m_wTimer[i]--;
		if(m_wTimer[i] == 0)
		{
			switch(i)
			{
			case AIBURST_ID:
				//���ұ仯ң���ϴ�
				if((m_NetCfg.dwStatus&MASK_AIE)==0)
					return;
					
				SetFlag(SF_BURSTAI);
				m_wTimer[AIBURST_ID] = (m_NetCfg.AIBurstRepTM*100+CAN_TIMER_TICK-1)/CAN_TIMER_TICK;
				break;
			
			case AI_ID:
				//ȫң���ϴ�
				if((m_NetCfg.dwStatus&MASK_AI)==0)
					return;
					
				SetFlag(SF_AIREPORT);
				m_wTimer[AI_ID] = m_NetCfg.wAIRepTM*1000/CAN_TIMER_TICK;
				break;
			
			case BI_ID:
				//ȫң���ϴ�
				if((m_NetCfg.dwStatus&MASK_BI)==0)
					return;
					
				SetFlag(SF_BIREPORT);
				m_wTimer[BI_ID] = m_NetCfg.wBIRepTM*1000/CAN_TIMER_TICK;
				break;

#if(SYSTEM_TYPE == _SYS_KF1300_)
			case LED_ID:
				//LED״̬�ϴ�
				if((m_NetCfg.dwStatus&MASK_LED)==0)
					return;
					
				SetFlag(SF_LEDREPORT);
				m_wTimer[LED_ID] = m_NetCfg.wLEDStatusTM*1000/CAN_TIMER_TICK;
				break;
#endif			
			case CI_ID:
				//ȫ����ϴ�
				if((m_NetCfg.dwStatus&MASK_CI)==0)
					return;
					
				SetFlag(SF_CIREPORT);
				m_wTimer[CI_ID] = m_NetCfg.wCIRepTM*1000/CAN_TIMER_TICK;
				break;
			
			case BIBURST_ID:
				//��λң���ϴ�
				if((m_NetCfg.dwStatus&MASK_BIE)==0)
					return;
					
				SetFlag(SF_BURSTBI);
				break;
			
			case SOE_ID:
				//SOE�ϴ�
				if((m_NetCfg.dwStatus&MASK_SOE)==0)
					return;
					
				SetFlag(SF_SOE);
				break;

			case RELAYALARM_ID:
				//�����澯�����ϴ�
				if((m_NetCfg.dwStatus&MASK_RELAYALARMEV)==0)
					return;
					
				SetFlag(SF_RELAYALARM);
				break;
				
			case RELAYSTART_ID:
				//�������������ϴ�
				if((m_NetCfg.dwStatus&MASK_RELAYSTARTEV)==0)
					return;
					
				SetFlag(SF_RELAYSTART);
				break;
				
			case RELAYACTION_ID:
				//�������뱨���ϴ�
				if((m_NetCfg.dwStatus&MASK_RELAYACTIONEV)==0)
					return;
					
				SetFlag(SF_RELAYACTION);
				break;
				
			case BORET_ID:
				//ң�ط�У
				if((m_NetCfg.dwStatus&MASK_BO)==0)
					return;
					
				SetFlag(SF_HaveBOReturn);
				break;
			
			case SAFETY_ID:
				m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;
				SetFlag(SF_SAFEREPORT);
				break;

			case TIMING_ID:
				m_wTimer[TIMING_ID] = m_NetCfg.TimingTM*60*1000/CAN_TIMER_TICK;
				if(DAE_GetRunningFlags(SS_TIME_B)||DAE_GetRunningFlags(SS_TIME_GPS))
					SetFlag(SF_TIMING);
					
				break;
				
			default:
				break;
			}
			
			//����100����ͨ��������Ϣ
			StartMsg(MCC_CommIdle,m_nBurstIdle);
		}
	}

	
	// ���LED��״̬	
#if(SYSTEM_TYPE == _SYS_KF1300_)
	BYTE	bStatus,bFreq;

	// 1���Ӽ��һ��״̬�仯
	if((m_dwTaskRunCount%10)==0)
	{
		for(i=0;i<LED_RELAY_WORD_NUM;i++)
		{
			if(GetFlashLedValue(i,&bFreq,&bStatus) != 0)
				continue;
			
			if((bStatus != m_pLEDStatus[i])||(bFreq != m_pLEDFreq[i]))
			{
				// ˢ��״ֵ̬
				m_pLEDStatus[i] = bStatus;
				m_pLEDFreq[i] = bFreq;
				SetFlag(SF_LEDBURST);
				m_wTimer[LED_ID] = m_NetCfg.wLEDStatusTM*1000/CAN_TIMER_TICK;

//				ReportMsg("LED %d change,STATUS=%d FREQ=%d--->",i,bStatus,bFreq);
			}
		}
	}
#endif
}

/*--------------------------------------------------------------*/
/*��������CanOnSoe												*/
/*���ܣ�  CANBUS��վ��ԼSOE��Ϣ��Ӧ����		       				*/
/*����:   DWORD dwDevID		����SOE���豸ID�����ã�				*/
/*�����  ��						       						*/
/*--------------------------------------------------------------*/

void CSCanbusPro::OnSOE()
{	
	// ����ѹ��Ͷ��ʱ�˳�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}
	
	//ȡ��ǰ���������ָ��	
	if((m_NetCfg.dwStatus&MASK_SOE)==0)
		return;
	SetFlag(SF_SOE);
	
	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*��������OnAIE													*/
/*���ܣ�  CANBUS��վ��ԼAIE��Ϣ��Ӧ����		       				*/
/*����:   NONE													*/
/*�����  ��						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnAIE()
{	
	// ����ѹ��Ͷ��ʱ�˳�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}
	
	//ȡ��ǰ���������ָ��	
	if((m_NetCfg.dwStatus&MASK_AIE)==0)
		return;
	SetFlag(SF_AIE);
	
	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*��������OnRelayEvent											*/
/*���ܣ�  CANBUS��վ��Լ����������Ϣ��Ӧ����		       		*/
/*����:   MsgCode	��������													*/
/*�����  ��						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayEvent(DWORD MsgCode, DWORD wParam)
{
	// ����ѹ��Ͷ��ʱ�˳�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}

	//�б����ʱ�̿����� 
	switch(MsgCode)
	{
	//����
	case 0: OnRelayStart();		break;

	//����
	case 1: OnRelayAction();    break;		

	//�澯
	case 2: OnRelayAlarm();    break;	

	default: break;
	}
}

/*--------------------------------------------------------------*/
/*��������OnRelayAlarm											*/
/*���ܣ�  CANBUS��վ��Լ�����澯��Ϣ��Ӧ����		       		*/
/*����:   DWORD dwDevID		����SOE���豸ID�����ã�				*/
/*�����  ��						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayAlarm()
{	
	//ȡ��ǰ���������ָ��	
	if((m_NetCfg.dwStatus&MASK_RELAYALARMEV)==0)
		return;

	SetFlag(SF_RELAYALARM);
	
	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*��������OnRelayStart												*/
/*���ܣ�  CANBUS��վ��Լ����������Ϣ��Ӧ����		       				*/
/*����:   DWORD dwDevID		����SOE���豸ID�����ã�				*/
/*�����  ��						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayStart()
{	
	//ȡ��ǰ���������ָ��	
	if((m_NetCfg.dwStatus&MASK_RELAYSTARTEV)==0)
		return;

	SetFlag(SF_RELAYSTART);
	
	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*��������OnRelayAction												*/
/*���ܣ�  CANBUS��վ��Լ����������Ϣ��Ӧ����		       				*/
/*����:   DWORD dwDevID		����SOE���豸ID�����ã�				*/
/*�����  ��						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayAction()
{	
	//ȡ��ǰ���������ָ��	
	if((m_NetCfg.dwStatus&MASK_RELAYACTIONEV)==0)
		return;

	SetFlag(SF_RELAYACTION);
	
	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*-------------------------------------------------------------*/
/*��������CanOnBIBurst						*/
/*���ܣ�  CANBUS��վ��Լ��λң����Ϣ��Ӧ����		       	*/
/*����:   DWORD dwDevID		����SOE���豸ID�����ã�		*/
/*�����  ��						       */
/*-------------------------------------------------------------*/

void CSCanbusPro::OnBIBurst()
{
	// ����ѹ��Ͷ��ʱ�˳�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}

	if((m_NetCfg.dwStatus&MASK_BIE)==0)
		return;
		
	SetFlag(SF_BURSTBI);

	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*-------------------------------------------------------------*/
/*��������CanOnBoSelectEcho					*/
/*���ܣ�  CANBUS��վ��Լң��ѡ��У��Ϣ��Ӧ����	       	*/
/*����:   					*/
/*		WORD 	switchno					*/
/*		WORD 	attrib						*/
/*		WORD 	result	*/
/*			=0   �ɹ� =1 ʧ��	*/
/*�����  ��						       */
/*-------------------------------------------------------------*/
	
void CSCanbusPro::OnBOSelectEcho(WORD switchno,WORD attrib,WORD result)	
{	
	m_Bo.RetResult = result;
	SetFlag(SF_HaveBOReturn);

	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

void CSCanbusPro::OnBOExecEcho(WORD switchno,WORD Mode,WORD result)
{
	m_Bo.RetResult = result;
	SetFlag(SF_HaveBOReturn);	

	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

void CSCanbusPro::OnBODirectEcho(WORD switchno,WORD Mode,WORD result)	
{
	m_Bo.RetResult = result;
	SetFlag(SF_HaveBOReturn);
	
	//����40�������ͨ��������Ϣ
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

void CSCanbusPro::OnCOEMake(DWORD hPara,DWORD wPara,DWORD pPara,WORD lPara)
{	
	m_Bo.No = LOLOBYTE(wPara);
	m_Bo.Command = LOHIBYTE(wPara);
	m_Bo.Attrib = HIWORD(wPara);
	
	SetFlag(SF_MAKECOEECHO);
}

// KF1300��������
void CSCanbusPro::OnAutoAdjustEcho(DWORD dwResult,DWORD dwClass)
{
#if(SYSTEM_TYPE == _SYS_KF1300_)	
	TTxdHead	*pTxdHead;
	PBYTE		pDataField;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_SELECTWITHVALUE);
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	
	pTxdHead->ObjType	= DOC_COE;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	
	memcpy(pDataField,m_Bo.Info,4);
	
	m_TxdWritePtr += 4;
	
	if(dwResult)
		pTxdHead->Status |= STATUS_CTL_ERROR;
		
	TxdTail(0); 
	
#elif(SYSTEM_TYPE == _SYS_KF6500_)	
	TTxdHead	*pTxdHead;
	PBYTE		pDataField;

	ReportMsg("Net protocol recv AutoAdjustEcho. Result=%d Class=%x",dwResult,dwClass);
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_WRITEVALUE);
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;

	pTxdHead->ObjType	= DOC_COE;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	memcpy(pDataField,m_Bo.Info,9);
				
	m_TxdWritePtr += 9;

	if(dwResult == FAILURE)
		pTxdHead->Status |= STATUS_CTL_ERROR;
		
	TxdTail(0); 
#endif		
	return;
}

//��Ӧ͸��������Ϣ
void CSCanbusPro::OnTransOIE(DWORD dwPort)
{
	WORD	Tail = DAE_GetTransDataTail();
	
	if(!GetFlag(SF_NeedConfig))
	{
		while(m_TransDataRP != Tail)
		{
			// ��Ч���ݼ���
			if(DAE_ReadTransData((BYTE *)m_pMemBlock,m_TransDataRP) == 0)
			{
				m_TransDataRP = (m_TransDataRP+1)%DAE_MAX_TRANSNUM;
				continue;
			}
			
			TxdTransData();
			
			// ����ָ��
			m_TransDataRP = (m_TransDataRP+1)%DAE_MAX_TRANSNUM;
		}
	}
}

void CSCanbusPro::AlterConfig(DWORD dwFlag) 
{
	TCanNetCfg 		*pNetCfg;
	WORD			Num;
	
	//ȡ��ǰ���������ָ��
	pNetCfg = &m_NetCfg;
		
	pNetCfg->dwStatus = dwFlag;

	//����ң�ⶨʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if(pNetCfg->dwStatus&MASK_AI)
		m_wTimer[AI_ID] = pNetCfg->wAIRepTM*1000/CAN_TIMER_TICK;
	else
		pNetCfg->wAIRepTM = 0;
	
	//����ң�Ŷ�ʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if((pNetCfg->dwStatus&MASK_BI)&&(DB_GetBINum()!=0))
		m_wTimer[BI_ID] = pNetCfg->wBIRepTM*1000/CAN_TIMER_TICK;
	else
		pNetCfg->wBIRepTM = 0;
		
	//���õ�ȶ�ʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if((pNetCfg->dwStatus&MASK_CI)&&(DB_GetCINum()!=0))
		m_wTimer[CI_ID] = pNetCfg->wCIRepTM*1000/CAN_TIMER_TICK;
	else
		pNetCfg->wCIRepTM = 0;
			
			
	//���ñ仯ң�ⶨʱ�ڼ��
	if(pNetCfg->dwStatus&MASK_AIE)
	{
		if(!DB_AIEIsEmpty())
			SetFlag(SF_AIE);	

		m_wTimer[AIBURST_ID] = (pNetCfg->AIBurstRepTM*100+CAN_TIMER_TICK-1)/CAN_TIMER_TICK;		
	}
	else
		pNetCfg->AIBurstRepTM = 0;
	
	//���ñ�λң���ط����		
	if(pNetCfg->dwStatus&MASK_BIE)
	{
		DB_GetBIENum(&Num);
		if(Num>0)
			SetFlag(SF_BURSTBI);	
	}
	else
	{
		pNetCfg->BIBurstRepTM = 0;
		//ȥ����λң�������־
		SetFlag(SF_BURSTBI);
		//ȥ����λң���ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIBURST))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}
	
	//����SOE�ط����	
	if(pNetCfg->dwStatus&MASK_SOE)
	{
		DB_GetSOENum(&Num);
		if(Num>0)
			SetFlag(SF_SOE);	
	}
	else
	{
		pNetCfg->SoeRepTM = 0;
		//ȥ����λң�������־
		SetFlag(SF_SOE,FLAG_OFF);
		//ȥ����λң���ط���־�����˶�ָ��
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIEVENT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
			//����ָ����˸���	
	}

	//���ñ����澯�����ط����	
	if(pNetCfg->dwStatus&MASK_RELAYALARMEV)
	{
		DB_GetRelayAlarmReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYALARM);	
	}
	else
	{
		//ȥ�������澯�����־
		SetFlag(SF_RELAYALARM,FLAG_OFF);
		//ȥ�������澯�����ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYALARMREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//���ñ������������ط����	
	if(pNetCfg->dwStatus&MASK_RELAYSTARTEV)
	{
		DB_GetRelayStartReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYSTART);	
	}
	else
	{
		//ȥ���������������־
		SetFlag(SF_RELAYSTART,FLAG_OFF);
		//ȥ�����������ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYSTARTREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//���ñ������������ط����	
	if(pNetCfg->dwStatus&MASK_RELAYACTIONEV)
	{
		DB_GetRelayActionReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYACTION);	
	}
	else
	{
		//ȥ������������־
		SetFlag(SF_RELAYACTION,FLAG_OFF);
		//ȥ�����������ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYACTIONREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	if(!(pNetCfg->dwStatus&(MASK_TIME)))
		pNetCfg->TimingTM = 0;
		
	//��ƽ�����ķ��ͼ��	
	m_wTimer[SAFETY_ID] = pNetCfg->SaftyRepTM*1000/CAN_TIMER_TICK;

	if(!(pNetCfg->dwStatus&MASK_BO))
	{	
		//ȥ��ң�ط�У�����־
		SetFlag(SF_HaveBOReturn,FLAG_OFF);

		//ȥ��ң�ط�У�ط���־
		if(GetFlag(SF_WAITCONFIRM)&&(m_ResInfo.ObjType == DOC_BO))
			SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}
	
	return;
}

BOOL CSCanbusPro::ProcessRetryStatus()
{
	if(m_RetryCount > 0)
	{
		m_RetryCount--;
		if(m_RetryCount == 0)
		{
			//Ŀǰֻȡ��ң���ط�
			switch(m_ResInfo.ResFunCode&0x7F)
			{
				case FUN_SELECT:
				case FUN_OPERATE:
				case FUN_CANCEL:
				case FUN_DIROPERATE:
					switch(m_ResInfo.ObjType)
					{
						case	DOC_BO: 
							SetFlag(SF_HaveBOReturn,FLAG_OFF);	
							m_wTimer[BORET_ID] = 0;
							break;
						default:
							break;	
					}
					break;
				default:
					break;	
			}

			// ����ط���־
			SetFlag(SF_WAITCONFIRM,FLAG_OFF);

			// �л�����������IDLE
			StartMsg(MCC_CommIdle,CAN_RUNIDLE);
			
			return FALSE;
		}
		else
		{
			StartMsg(MCC_CommIdle,m_nRetryIdle);
			return TRUE;
		}
	}

	return FALSE;
}
BYTE CSCanbusPro::HaveClass1Data()
{
	WORD	wNum;
		
	if(GetFlag(SF_HaveBOReturn))
		return TRUE;
	
	if((DB_GetBIENum(&wNum)==DB_ERR_OK)&&(wNum>0))
		return TRUE;
	
	if((DB_GetSOENum(&wNum)==DB_ERR_OK)&&(wNum>0))
		return TRUE;
	
	if((DB_GetRelayAlarmReportNum(&wNum)==DB_ERR_OK)&&(wNum>0))
		return TRUE;
		
	if((DB_GetRelayActionReportNum(&wNum)==DB_ERR_OK)&&(wNum>0))
		return TRUE;

#if(SYSTEM_TYPE == _SYS_KF1300_)
	DWORD	dwNum;

	if(GetFlag(SF_LEDBURST))
		return TRUE;
	if((DB_GetAIENum(&dwNum)==DB_ERR_OK)&&(dwNum>0))
		return TRUE;
#else
	if((DB_GetAIENum(&wNum)==DB_ERR_OK)&&(wNum>0))
		return TRUE;
#endif

	return FALSE;
}

BYTE CSCanbusPro::GetStationStatus()
{	
	BYTE byStatus=0;
	
	if(HaveClass1Data())
		byStatus |= HAVE_CLASS1;
	
	if(GetFlag(SF_NeedConfig))	
		byStatus |= NEED_CONFIG;

	if(GetFlag(SF_TIMING))	
		byStatus |= STATUS_TIM_ERROR;

	return byStatus;
}

void CSCanbusPro::IncResReq()
{
	m_CurrentResSeq++;
	if(m_CurrentResSeq>APP_RESSEQ_MAX)
		m_CurrentResSeq=APP_RESSEQ_MIN;
}

void CSCanbusPro::BackRetryFrame()
{
	SetFlag(SF_WAITCONFIRM);
	
	m_RetryLen = m_TxdWritePtr;

	//�����λң��֡�����Ա��ط�
	memcpy(m_RetryBuf,m_TxdInfo.pBuf,m_RetryLen);
	StartMsg(MCC_CommIdle,m_nRetryIdle);
	m_RetryCount = MAX_RETRY_NUM;
}

//�ж�ĳһң���Ƿ�仯
BOOL  CSCanbusPro::AIChange(long diff,long OldAI,WORD wAIDeadArea)
{
	diff = (diff<0)?(-diff):diff;
	OldAI = (OldAI<0)?(-OldAI):OldAI;
	return (diff>(OldAI*wAIDeadArea/10000));
}

WORD  CSCanbusPro::GetLayerAddr(WORD wAddr)
{
	if(wAddr&CLASS_4_ADDR_VALID)
		return (wAddr&CLASS_4_ADDR_MASK)>>CLASS_4_ADDR_OFFSET;
	else if(wAddr&CLASS_4_ADDR_VALID)
		return (wAddr&CLASS_3_ADDR_MASK)>>CLASS_3_ADDR_OFFSET;
	else if(wAddr&CLASS_4_ADDR_VALID)
		return (wAddr&CLASS_2_ADDR_MASK)>>CLASS_2_ADDR_OFFSET;
	else if(wAddr&CLASS_4_ADDR_VALID)
		return (wAddr&CLASS_1_ADDR_MASK)>>CLASS_1_ADDR_OFFSET;
	else
		return 0xFFFF;
}

void CSCanbusPro::TxdHead(BYTE AppCtrl,BYTE FunCode)
{
	TTxdHead 	*pTxdHead;

	memset(m_TxdInfo.pBuf,0,m_TxdInfo.wBufLen);
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	
	pTxdHead->START  = 0x68;
	pTxdHead->DestAddrLO  = LOBYTE(m_wDAddr);
	pTxdHead->DestAddrHI  = HIBYTE(m_wDAddr);
	pTxdHead->SourceAddrLO  = LOBYTE(m_wSAddr);
	pTxdHead->SourceAddrHI  = HIBYTE(m_wSAddr);
	pTxdHead->TControl    = 0xC0;	
	pTxdHead->APPControl  = AppCtrl;
	pTxdHead->FunCode  = 0x80+FunCode;
	pTxdHead->Status  = GetStationStatus();	

	m_RetryAddr = m_wDAddr;
	
	m_TxdWritePtr = 13;
}

void CSCanbusPro::TxdTail(DWORD dwTxdFlag)
{
	WORD	wLength;
	
	wLength = m_TxdWritePtr-3;
	
	m_TxdInfo.pBuf[1] = LOBYTE(wLength);
	m_TxdInfo.pBuf[2] = HIBYTE(wLength);
	
	m_TxdInfo.pBuf[m_TxdWritePtr] = LPC(0,m_TxdInfo.pBuf,m_TxdWritePtr);
	m_TxdWritePtr++;
	
	WriteComm(m_TxdWritePtr,GetLayerAddr(m_wDAddr)|NET_ADDR_MASK,dwTxdFlag);
	
	// ƽ�����ĸ�λ
	m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;
}

void CSCanbusPro::TxdConfirm()
{	
	TTxdHead 	*pTxdHead;

	TxdHead(0xC0+m_CommandSeq,FUN_CON);	

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pTxdHead->ObjType = DOC_NONE;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	
	TxdTail(0x80000000);
	
	return;
}

BOOL  CSCanbusPro::TxdAI()
{
	TTxdHead 	*pTxdHead;
	PBYTE  		pDataField;
	WORD		AINum,i;
	long		yc;
	DWORD		value;
	
	if(m_NetCfg.dwStatus&MASK_AI==0)
		return FALSE;

	TxdHead(0xC0+m_CurrentResSeq,FUN_REPORTVALUE);	
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;

	pTxdHead->ObjType 	= DOC_AI;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;

	//START
	pDataField[0] = LOBYTE(LastAIPtr);
	pDataField[1] = HIBYTE(LastAIPtr);
	
	//-----------�������ݿ�ӿ�-----------------
	i=0;	
#if(SYSTEM_TYPE == _SYS_KF1300_)
	AINum = DB_GetRMNum();
#else
	AINum = DB_GetAINum();
#endif
	
	while(LastAIPtr<AINum)
	{
		//-----------�������ݿ�ӿ�-----------------
#if(SYSTEM_TYPE == _SYS_KF1300_)
		DB_ReadSecondRM(LastAIPtr,&yc);
#else
		DB_ReadSecondAI(LastAIPtr,&yc);
#endif
		pOldAI[LastAIPtr] = yc;
		
		value = *(DWORD *)(&yc);
		pDataField[4+i*4] = LLBYTE(value);
		pDataField[5+i*4] = LHBYTE(value);
		pDataField[6+i*4] = HLBYTE(value);
		pDataField[7+i*4] = HHBYTE(value);
	
		i++;
		LastAIPtr++;
		
		if((i+1)*4+17>m_nMaxFrameLen)
			break;
	}
	
	pDataField[2] = LOBYTE(i);
	pDataField[3] = HIBYTE(i);
	
	LastAIPtr %= AINum;		

	m_TxdWritePtr += 4+i*4;

	TxdTail(0);
	
	IncResReq();
			
	return TRUE;
}

BOOL  CSCanbusPro::TxdBI()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pDataField;
	WORD		BIByteNum,i;
	BYTE		BIByte;

	// ����ѹ��Ͷ��ʱ�˳�
	if(GetMaintSWStatus())
		return FALSE;
			
	if((m_NetCfg.dwStatus&MASK_BI)==0)
		return FALSE;

	TxdHead(0xC0+m_CurrentResSeq,FUN_REPORTVALUE);	
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;	
	pTxdHead->ObjType 	= DOC_BIGROUP;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	
	//-----------�������ݿ�ӿ�-----------------
	BIByteNum = (DB_GetBINum()+7)/8;
	pDataField[0] = BIByteNum;
	
	for(i=0;i<BIByteNum;i++)
	{
	//-----------�������ݿ�ӿ�-----------------
		DB_ReadBIByte(i*8,&BIByte,DB_BI_BYTE81);
		pDataField[i+1] = BIByte;
	}
	
	m_TxdWritePtr += 1+BIByteNum;

	TxdTail(0);

	IncResReq();
		
	return TRUE;
}

BOOL  CSCanbusPro::TxdCI()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pDataField;
	WORD		CINum,i;
	DWORD		CIValue=0;
		
	if((m_NetCfg.dwStatus&MASK_CI)==0)
		return FALSE;
		
	TxdHead(0xC0+m_CurrentResSeq,FUN_REPORTVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_CI;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
			
	pDataField[0] = LastCIPtr;
	pDataField[1] = 0;
	
	//-----------�������ݿ�ӿ�-----------------
	CINum = DB_GetCINum();
	i=0;	
	while(LastCIPtr<CINum)
	{
	//-----------�������ݿ�ӿ�-----------------
	   DB_ReadCI(LastCIPtr,&CIValue);
	   pDataField[i*4+4] = LLBYTE(CIValue);	
	   pDataField[i*4+5] = LHBYTE(CIValue);	
	   pDataField[i*4+6] = HLBYTE(CIValue);	
	   pDataField[i*4+7] = HHBYTE(CIValue);	

	   i++;
	   LastCIPtr++;
	   
	   if((i+1)*4+17>m_nMaxFrameLen)
		break;	
	}
	
	pDataField[2] = i;
	pDataField[3] = 0;
	LastCIPtr %= CINum;
	
	m_TxdWritePtr += 4+i*4;	
	
	TxdTail(0);

	IncResReq();
			
	return TRUE;
}

BOOL CSCanbusPro::TxdBurstBI()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pDataField;
	TBurstBI 	bib;
	WORD		i,WritePtr,Event_rp,Event_tail;
		
	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}
	
	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIBURST;
	pTxdHead->ObjQulifier 	= (INDEX_16BIT<<4)+QUALI_NUM_8;
	
	//������䣬�����Ӧ������Ϣ
	DB_GetBIERead(&Event_rp);
	DB_GetBIETail(&Event_tail);
		
	WritePtr = 1;
	
	//�����һ�������Ƿ��»�����Խ��
	for(i=0;(Event_rp != Event_tail)&&(i<20);i++)
	{
	//-----------�������ݿ�ӿ�----------------	   	
	   	//����ȡ�귵��	
		if(DB_ReadBIE(&bib ,Event_rp))
			break;
			
	 	pDataField[WritePtr++] = LOBYTE(bib.wNo);
	 	pDataField[WritePtr++] = HIBYTE(bib.wNo);
	   	pDataField[WritePtr++] = bib.bStatus;

		//������ָ��
		Event_rp = (Event_rp+1)%MAX_BIE_NUM;
	}

	if(i==0)
		return FALSE;
	
	pDataField[0] = i;
	
		//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_BIBURST;
	m_ResInfo.ResSeq  = m_CurrentResSeq;
	m_ResInfo.ObjNum = i;

	m_TxdWritePtr  = WritePtr+13; 
	
	TxdTail(0);
	
	IncResReq();
	
	BackRetryFrame();	

	return TRUE;
}

BOOL CSCanbusPro::TxdSoe()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	TEventBI 	bie;
	WORD		i,WritePtr,Event_rp,Event_tail;

	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}
	
	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIEVENT;
	pTxdHead->ObjQulifier 	= (INDEX_16BIT<<4)+QUALI_NUM_8;
	

	//������䣬�����Ӧ������Ϣ
	DB_GetSOERead(&Event_rp);
	DB_GetSOETail(&Event_tail);
	
	WritePtr = 1;
	
	for(i=0;(Event_rp != Event_tail)&&(i<10);i++)
	{
	    //-----------�������ݿ�ӿ�-----------------
		if(DB_ReadSOE(&bie ,Event_rp))
			break;
			
		pDataField[WritePtr++] = LOBYTE(bie.wNo);
		pDataField[WritePtr++] = HIBYTE(bie.wNo);
	   	pDataField[WritePtr++] = bie.bStatus;
	   	pDataField[WritePtr++] = LOLOBYTE(bie.stTime.Lo);
	   	pDataField[WritePtr++] = LOHIBYTE(bie.stTime.Lo);
	   	pDataField[WritePtr++] = HILOBYTE(bie.stTime.Lo);
	   	pDataField[WritePtr++] = HIHIBYTE(bie.stTime.Lo);
	   	pDataField[WritePtr++] = LOBYTE(bie.stTime.Hi);
	   	pDataField[WritePtr++] = HIBYTE(bie.stTime.Hi);
	   		
		//������ָ��
		Event_rp = (Event_rp+1)%MAX_SOE_NUM;
	}

	if(i==0)
		return FALSE;
	
	pDataField[0] = i;
		
		//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_BIEVENT;
	m_ResInfo.ResSeq  = m_CurrentResSeq;
	m_ResInfo.ObjNum = i;

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	IncResReq();

	BackRetryFrame();	
	
	return TRUE;
}

BOOL CSCanbusPro::TxdAIE()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	TEventAI 	aie;
	WORD		i,WritePtr,Event_rp,Event_tail;

	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}
	
	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_AIEVENT;
	pTxdHead->ObjQulifier 	= (INDEX_16BIT<<4)+QUALI_NUM_8;
	

	//������䣬�����Ӧ������Ϣ
	DB_GetAIERead(&Event_rp);
	DB_GetAIETail(&Event_tail);
	
	WritePtr = 1;
	
	for(i=0;(Event_rp != Event_tail)&&(i<16);i++)
	{
	    //-----------�������ݿ�ӿ�-----------------
		if(DB_ReadAIE(&aie ,Event_rp))
			break;
			
		pDataField[WritePtr++] = LOBYTE(aie.nNo);
		pDataField[WritePtr++] = HIBYTE(aie.nNo);
	   	pDataField[WritePtr++] = aie.bStatus;
	   	pDataField[WritePtr++] = LOLOBYTE(aie.lValue);
	   	pDataField[WritePtr++] = LOHIBYTE(aie.lValue);
	   	pDataField[WritePtr++] = HILOBYTE(aie.lValue);
	   	pDataField[WritePtr++] = HIHIBYTE(aie.lValue);
	   	pDataField[WritePtr++] = LOLOBYTE(aie.stTime.Lo);
	   	pDataField[WritePtr++] = LOHIBYTE(aie.stTime.Lo);
	   	pDataField[WritePtr++] = HILOBYTE(aie.stTime.Lo);
	   	pDataField[WritePtr++] = HIHIBYTE(aie.stTime.Lo);
	   	pDataField[WritePtr++] = LOBYTE(aie.stTime.Hi);
	   	pDataField[WritePtr++] = HIBYTE(aie.stTime.Hi);
	   		
		//������ָ��
		Event_rp = (Event_rp+1)%MAX_AIE_NUM;
	}

	if(i==0)
		return FALSE;
	
	pDataField[0] = i;
		
		//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_AIEVENT;
	m_ResInfo.ResSeq  = m_CurrentResSeq;
	m_ResInfo.ObjNum = i;

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	IncResReq();

	BackRetryFrame();	
	
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayAlarmReport()
{
	TTxdHead 			*pTxdHead;
	PBYTE       		pDataField;
	PBYTE				pActionData;	
	TRelayAlarmEv		tReport;
	TRelayEventHead		tActionHead;	
	WORD				Event_rp,Event_tail;
	WORD				i,WritePtr;
	
	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}
	
	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_RELAYALARMREPORT;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	//������䣬�����Ӧ������Ϣ
	DB_GetRelayAlarmReportRead(&Event_rp);
	DB_GetRelayAlarmReportTail(&Event_tail);
	
	WritePtr = 4;

	for(i=0;(Event_rp != Event_tail)&&(i<1);i++)
	{
		if(DB_ReadRelayAlarmReport(&tReport,Event_rp))
			continue;

		pActionData = (BYTE *)&tReport.tEvent.tAction;

		memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));

		if(tActionHead.FD_Num > MAX_FD_NUM)
		{
			ReportMsg("Alarm event report FD num error!	FD_num=%d",tActionHead.FD_Num);
			break;
		}
		
		if((WritePtr+sizeof(TRelayEventHead)+tActionHead.FD_Num*6) > (m_nMaxFrameLen-13))
			return FALSE;

		WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,FALSE);
		pActionData += sizeof(TRelayEventHead)+tActionHead.FD_Num*6;

//		ReportMsg("Alarm event report!	Read= %d Inf=%d FD_num=%d",\
			Event_rp,tActionHead.nINF,tActionHead.FD_Num);
		
	 	Event_rp = (Event_rp+1)%MAX_RALM_NUM;
	 }


	if(i==0)
	{
		DB_IncRelayAlarmReportRead();
		return FALSE;
	}
	
	// �������
	pDataField[0] = LOBYTE(Event_rp);
	pDataField[1] = HIBYTE(Event_rp);
	pDataField[2] = 1;
	pDataField[3] = 0;
		
		//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_RELAYALARMREPORT;
	m_ResInfo.ResSeq  = m_CurrentResSeq;
	m_ResInfo.ObjNum = 1;

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	IncResReq();

	BackRetryFrame();	
	
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayStartReport()
{
	TTxdHead 			*pTxdHead;
	PBYTE       		pDataField;
	PBYTE				pActionData;	
	TRelayStartEv		tReport;
	TRelayEventHead		tActionHead;	
	WORD				Event_rp,Event_tail;
	WORD				i,WritePtr;
	
	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}

	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_RELAYSTARTREPORT;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	//������䣬�����Ӧ������Ϣ
	DB_GetRelayStartReportRead(&Event_rp);
	DB_GetRelayStartReportTail(&Event_tail);
	
	WritePtr = 4;
	
	for(i=0;(Event_rp != Event_tail)&&(i<1);i++)
	{
		if(DB_ReadRelayStartReport(&tReport,Event_rp))
			continue;

		pActionData = (BYTE *)&tReport.tEvent.tAction;

		memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));

		if(tActionHead.FD_Num > MAX_FD_NUM)
		{
			ReportMsg("Start event report FD num error!	FD_num=%d",tActionHead.FD_Num);
			break;
		}
		
		if((WritePtr+sizeof(TRelayEventHead)+tActionHead.FD_Num*6) > (m_nMaxFrameLen-13))
			break;
				
		WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,FALSE);
		pActionData += sizeof(TRelayEventHead)+tActionHead.FD_Num*6;

//		ReportMsg("Start event report!  Read= %d Inf=%d FD_num=%d",\
			Event_rp,tActionHead.nINF,tActionHead.FD_Num);

		Event_rp = (Event_rp+1)%MAX_RSTT_NUM;
	 }


	if(i==0)
	{
		DB_IncRelayStartReportRead();
		return FALSE;
	}
	
	pDataField[0] = LOBYTE(Event_rp);
	pDataField[1] = HIBYTE(Event_rp);
	pDataField[2] = 1;
	pDataField[3] = 0;
		
		//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_RELAYSTARTREPORT;
	m_ResInfo.ResSeq  = m_CurrentResSeq;
	m_ResInfo.ObjNum = 1;

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	IncResReq();

	BackRetryFrame();	
	
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayActionReport()
{
	TTxdHead 			*pTxdHead;
	PBYTE       		pDataField;
	PBYTE				pActionData,pItemNum;	
	TRelayActionEv		tReport;
	TRelayEventHead		tActionHead;	
	WORD				Event_rp,Event_tail;
	WORD				i,j,WritePtr;
	
	// ����ѹ��Ͷ��ʱ����¼�
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return FALSE;
	}

	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_RELAYACTIONREPORT;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	//������䣬�����Ӧ������Ϣ
	DB_GetRelayActionReportRead(&Event_rp);
	DB_GetRelayActionReportTail(&Event_tail);
	
	WritePtr = 4;
	
//	for(i=0;(Event_rp != Event_tail)&&((WritePtr+16)<MAX_FRAME_LEN);i++)
	for(i=0;(Event_rp != Event_tail)&&(i<1);i++)
	{
//		ReportMsg("Send Action Report RP=%d TP=%d",Event_rp,Event_tail);
		if(DB_ReadRelayActionReport(&tReport,Event_rp))
			continue;

		if((WritePtr+2) > (m_nMaxFrameLen-13))
			return FALSE;

		// ��¼�ֱ�����дλ��
		pItemNum = pDataField+WritePtr;

		// ������Ŀ��
		WritePtr += 2;
		
		pActionData = (BYTE *)&tReport.tEvent.tAction;

		for(j=0;j<tReport.tEvent.Num;j++)
		{
			memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));

			if(tActionHead.FD_Num > MAX_FD_NUM)
			{
				ReportMsg("Action event report FD num error! FD_num=%d",tActionHead.FD_Num);
				break;
			}
			
			if((WritePtr+sizeof(TRelayEventHead)+tActionHead.FD_Num*6) > (m_nMaxFrameLen-13))
				break;
				
			WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,FALSE);
			pActionData += sizeof(TRelayEventHead)+tActionHead.FD_Num*6;


//			ReportMsg("Action event report!  Read= %d Inf=%d FD_num=%d",\
//				Event_rp,tActionHead.nINF,tActionHead.FD_Num);
		}
		
		pItemNum[0] = j;
		pItemNum[1] = 0;
		
		Event_rp = (Event_rp+1)%MAX_RACT_NUM;
	 }
	
	if(i == 0)
	{
		DB_IncRelayActionReportRead();
		return FALSE;
	}
	
//	ReportMsg("Send action event report. ItemNum=%d !!! ",j);
	
	pDataField[0] = LOBYTE(Event_rp);
	pDataField[1] = HIBYTE(Event_rp);
	pDataField[2] = 1;
	pDataField[3] = 0;
		
		//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_RELAYACTIONREPORT;
	m_ResInfo.ResSeq  = m_CurrentResSeq;
	m_ResInfo.ObjNum = 1;

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	IncResReq();

	BackRetryFrame();	
	
	return TRUE;
}

BOOL CSCanbusPro::TxdBurstAI()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField;
	WORD		AINum,WritePtr,i,j;
	long		AINew,AIOld,diff;
	DWORD		value;
			
	TxdHead(0xC0+m_CurrentResSeq,FUN_REPORTVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_AIBURST;
	pTxdHead->ObjQulifier 	= (INDEX_16BIT<<4)+QUALI_NUM_16;
	
	//START
	i=0;
	//-----------�������ݿ�ӿ�-----------------
#if(SYSTEM_TYPE == _SYS_KF1300_)
	AINum = DB_GetRMNum();	
#else
	AINum = DB_GetAINum();	
#endif
	WritePtr = 2;
	
	for(j=0;j<AINum;j++)
	{
	//-----------�������ݿ�ӿ�-----------------
#if(SYSTEM_TYPE == _SYS_KF1300_)
		DB_ReadSecondRM(LastBurstAIPtr,&AINew);
#else
		DB_ReadSecondAI(LastBurstAIPtr,&AINew);
#endif
		AIOld = pOldAI[LastBurstAIPtr];
		
		diff = AINew-AIOld;
		
		if(AIChange((long)diff,(long)AIOld,m_NetCfg.AIDeadArea))
		{
			value = *(DWORD *)(&AINew);
			
			pDataField[WritePtr++] = LOBYTE(LastBurstAIPtr);
			pDataField[WritePtr++] = HIBYTE(LastBurstAIPtr);
			pDataField[WritePtr++] = LLBYTE(value);
			pDataField[WritePtr++] = LHBYTE(value);
			pDataField[WritePtr++] = HLBYTE(value);
			pDataField[WritePtr++] = HHBYTE(value);
		
			i++;
			
			pOldAI[LastBurstAIPtr] = AINew;
			
			if(WritePtr+14+4>m_nMaxFrameLen)
			{
			//-----------�������ݿ�ӿ�-----------------
				LastBurstAIPtr = (LastBurstAIPtr+1)%DB_GetAINum();
//				pProInfo->LastBurstAIPtr = (pProInfo->LastBurstAIPtr+1)%100;
					break;
			}
		}
	//-----------�������ݿ�ӿ�-----------------
		LastBurstAIPtr = (LastBurstAIPtr+1)%AINum;
	}
	
	if(i == 0)
	   return FALSE;
	
	pDataField[0] = LOBYTE(i);
	pDataField[1] = HIBYTE(i);

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);
	
	IncResReq();
			
	return TRUE;
}

BOOL CSCanbusPro::TxdBOReturn()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField;
	WORD		WritePtr;
		
	TxdHead(0xE0+m_CommandSeq,m_Bo.Command); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BO;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_BO;
	m_ResInfo.ResSeq  = m_CommandSeq;
	m_ResInfo.ObjNum = 1;
	
	WritePtr = 0;

	pDataField[WritePtr++] = 1;
	pDataField[WritePtr++] = m_Bo.No;
	pDataField[WritePtr++] = m_Bo.Attrib;
	pDataField[WritePtr++] = m_Bo.Info[0];
	pDataField[WritePtr++] = m_Bo.Info[1];
	pDataField[WritePtr++] = m_Bo.RetResult;
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0x80000000);
//	ReportMsg("CanBus YK  RETURN! Addr=%d",pProInfo->m_CommandAddr);

	BackRetryFrame();	

	return TRUE;
}


BOOL CSCanbusPro::TxdSafetyRep()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField;
	//-----------�������ݿ�ӿ�-----------------
//	TSysTime 	SysTime;
	DWORD		dwTemp;
	
	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	
	pTxdHead->ObjType 	= DOC_SAFETY;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	//ȡ��ǰʱ��
	//-----------�������ݿ�ӿ�-----------------
/*	
	if(ReadSysTime(&SysTime)!=0)
		return FALSE;		
	dwTemp = Dostounix(&SysTime);
*/	
	dwTemp = 0x12345678;
	pDataField[0] = LLBYTE(dwTemp);
	pDataField[1] = LHBYTE(dwTemp);
	pDataField[2] = HLBYTE(dwTemp);
	pDataField[3] = HHBYTE(dwTemp);
	
	//ȡ�豸״̬����
	memset(pDataField+4,0,4);
	
	//ȡ�豸��������
	//-----------�������ݿ�ӿ�-----------------
	if((m_wPortID == PORT_ID_CANA)||(m_wPortID == PORT_ID_ETHA))
		dwTemp = DAE_GetNetADataMask();
	else if((m_wPortID == PORT_ID_CANB)||(m_wPortID == PORT_ID_ETHB))
		dwTemp = DAE_GetNetBDataMask();
	else
		dwTemp = MASK_ALL;

	pDataField[8] = LLBYTE(dwTemp);
	pDataField[9] = LHBYTE(dwTemp);
	pDataField[10] = HLBYTE(dwTemp);
	pDataField[11] = HHBYTE(dwTemp);

	m_TxdWritePtr += 12;
	
	TxdTail(0);

	IncResReq();
	
	return TRUE;
}


void CSCanbusPro::TxdIdleRep()
{
	TTxdHead 	*pTxdHead;

	TxdHead(0xC0+m_CurrentResSeq,FUN_IDLE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pTxdHead->ObjType = DOC_NONE;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	
	TxdTail(0);	
	
	IncResReq();

	return;	
}

BOOL CSCanbusPro::TxdAIRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField,pRecData;
	WORD		Start;
	WORD		Num,Max,i;
	WORD		WritePtr;
	SDWORD		sYcValue;

#if(SYSTEM_TYPE == _SYS_KF1300_)
	Max = DB_GetRMNum();
#else
	Max = DB_GetAINum();
#endif

	if(Max <= 0)
		return FALSE;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_AI;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier; 
	
	WritePtr=0;
	switch(m_RxdHead->ObjQulifier)
	{
		case 0x07:
			Start = MAKEWORD(pRecData[0],pRecData[1]);
			Num = MAKEWORD(pRecData[2],pRecData[3]);
			
			// ��ʼ
			if(Start > Max) Start = Max;
			if(Start+Num > Max) Num = Max-Start;
			if(Num > 200) Num = 200;
				
			break;
			
		default:
			return FALSE;	
	}	
	
	if(Start>=Max)
		return FALSE;
	
	for(i=Start;i<Start+Num;i++)
	{
	//-----------�������ݿ�ӿ�-----------------
#if(SYSTEM_TYPE == _SYS_KF1300_)
	   DB_ReadSecondRM(i,&sYcValue);
#else
	   DB_ReadSecondAI(i,&sYcValue);
#endif
	   pDataField[WritePtr++] = LLBYTE(*((DWORD *)&sYcValue));
	   pDataField[WritePtr++] = LHBYTE(*((DWORD *)&sYcValue));
	   pDataField[WritePtr++] = HLBYTE(*((DWORD *)&sYcValue));
	   pDataField[WritePtr++] = HHBYTE(*((DWORD *)&sYcValue));
	 } 		

	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdAIWithQualityRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	TTxdHead	*pTxdHead;
	PBYTE		pDataField,pRecData;
	WORD		i,Start,Num,Max;
	WORD		WritePtr;
	long		lValue;
	TRunTimeRM	*pRMBase;

	Max = DB_GetRMNum();
	pRMBase = G_pCfgTable->DBaseConfig.pRM;

	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_AIWITHQUALITY;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 

	Start = MAKEWORD(pRecData[0],pRecData[1]);
	Num = MAKEWORD(pRecData[2],pRecData[3]);

	switch(m_RxdHead->ObjQulifier)
	{
		case 0x07:
			// ��ʼ
			if(Start > Max) Start = Max;
			if(Start+Num > Max) Num = Max-Start;
			if(Num > 200) Num = 200;
				
			break;
			
		default:
			Start = 0;
			Num = Max;	
			break;
	}	

	pDataField[0] = LOBYTE(Start);
	pDataField[1] = HIBYTE(Start);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);

	//-----------�������ݿ�ӿ�-----------------
	WritePtr=4;

	for(i=0;i<Num;i++)
	{
	   DB_ReadSecondRM(Start+i,&lValue);  

	   pDataField[WritePtr++] = LLBYTE(lValue);
	   pDataField[WritePtr++] = LHBYTE(lValue);
	   pDataField[WritePtr++] = HLBYTE(lValue);
	   pDataField[WritePtr++] = HHBYTE(lValue);
	   pDataField[WritePtr++] = pRMBase[i].wStatus;
	}	


	m_TxdWritePtr = WritePtr+13;

	TxdTail(0); 
	
	return TRUE;
#else
	TTxdHead	*pTxdHead;
	PBYTE		pDataField,pRecData;
	WORD		i,Start,Num,Max;
	WORD		WritePtr;
	long		lValue;
	TRunTimeAI	*pAIBase;

	Max = DB_GetAINum();
	pAIBase = DB_GetAIBase();

	if(pAIBase == NULL)
		return FALSE;

	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_AIWITHQUALITY;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 

	Start = MAKEWORD(pRecData[0],pRecData[1]);
	Num = MAKEWORD(pRecData[2],pRecData[3]);

	switch(m_RxdHead->ObjQulifier)
	{
		case 0x07:
			// ��ʼ
			if(Start > Max) Start = Max;
			if(Start+Num > Max) Num = Max-Start;
			if(Num > 200) Num = 200;
				
			break;
			
		default:
			Start = 0;
			Num = Max;	
			break;
	}	

	pDataField[0] = LOBYTE(Start);
	pDataField[1] = HIBYTE(Start);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);

	//-----------�������ݿ�ӿ�-----------------
	WritePtr=4;

	for(i=0;i<Num;i++)
	{
	   DB_ReadSecondAI(Start+i,&lValue);	

	   pDataField[WritePtr++] = LLBYTE(lValue);
	   pDataField[WritePtr++] = LHBYTE(lValue);
	   pDataField[WritePtr++] = HLBYTE(lValue);
	   pDataField[WritePtr++] = HHBYTE(lValue);
	   pDataField[WritePtr++] = pAIBase[i].wStatus;
	}	


	m_TxdWritePtr = WritePtr+13;

	TxdTail(0); 
			
	return TRUE;
#endif
}

BOOL CSCanbusPro::TxdBIRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Start,Stop;
	WORD		i;
	WORD 		WritePtr;
	BYTE 		BIValue;
	
	if(DB_GetBINum()<=0)
		return FALSE;
			
	pRecData = (PBYTE)&m_RxdHead->Range;
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIGROUP;	
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier; 
	
	WritePtr = 0;
	
	switch(m_RxdHead->ObjQulifier)
	{
		case 0x03:
			Start = 0;
			
			if(pRecData[0] > DB_GetBINum())
				Stop = DB_GetBINum();
			else
				Stop = pRecData[0];
				
			pDataField[WritePtr++] = Stop;
		case 0x00:
			Start = pRecData[0];
			pDataField[WritePtr++] = Start;

			if(pRecData[1]==0xFF)
			{
	//-----------�������ݿ�ӿ�-----------------
				Stop = DB_GetBINum();
			}
			else
			{
				Stop = pRecData[1];
				if(Stop > DB_GetBINum())
					Stop = DB_GetBINum();
			}
			pDataField[WritePtr++] = Stop;
			break;
		case 0x01:
			Start = pRecData[0];
			pDataField[WritePtr++] = pRecData[0];
			pDataField[WritePtr++] = pRecData[1];
			
			if(MAKEWORD(pRecData[3],pRecData[2])==0xFFFF)
			{
	//-----------�������ݿ�ӿ�-----------------
				Stop = DB_GetBINum();
			}
			else
			{
				Stop = pRecData[2];
				if(Stop > DB_GetBINum())
					Stop = DB_GetBINum();
			}
			pDataField[WritePtr++] = Stop;
			pDataField[WritePtr++] = 0;
			break;
			
		default://09
			return FALSE;
		
	}	
	
	if(Start>=DB_GetBINum())
		return FALSE;
		
	for(i=Start;i<Stop;i=i+8)
	//-----------�������ݿ�ӿ�-----------------
	{
		DB_ReadBIByte(i,&BIValue,DB_BI_BYTE18);
		pDataField[WritePtr++] = BIValue;		
	}

	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
				
	return TRUE;
}

BOOL CSCanbusPro::TxdBIWithQualityRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		i,Start,Num,Max;
	WORD 		WritePtr;
	TRunTimeBI	*pBIBase;

	
	Max = DB_GetBINum();
	pBIBase = DB_GetBIBase();
	
	if(pBIBase == NULL)
		return FALSE;

	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;

	pTxdHead->ObjType	= DOC_BIWITHQUALITY;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 

	Start = MAKEWORD(pRecData[0],pRecData[1]);
	Num = MAKEWORD(pRecData[2],pRecData[3]);
	
	switch(m_RxdHead->ObjQulifier)
	{
		case 0x07:
			// ��ʼ
			if(Start > Max) Start = Max;
			if(Start+Num > Max) Num = Max-Start;
			if(Num > 1000) Num = 1000;
				
			break;
			
		default:
			Start = 0;
			Num = Max;	
			break;
	}	

	pDataField[0] = LOBYTE(Start);
	pDataField[1] = HIBYTE(Start);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);
		
	WritePtr=4;
	
	for(i=0;i<Num;i++)
	//-----------�������ݿ�ӿ�-----------------
	{
		pDataField[WritePtr++] = pBIBase[Start+i].bStatus;		
	}
				
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0); 
						
	return TRUE;
}
BOOL CSCanbusPro::TxdCIRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField,pRecData;
	WORD		Start,Stop;
	WORD		i;
	WORD 		WritePtr;
	DWORD		dwCIVal;
	
	if(DB_GetCINum()<=0)
		return FALSE;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_CI;	
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier; 
	
	WritePtr=0;
	
	switch(m_RxdHead->ObjQulifier)
	{
		case 0x03:
			Start = 0;
			if(pRecData[0] > DB_GetCINum())
				Stop = DB_GetCINum();
			else
				Stop = pRecData[0];
				
			pDataField[WritePtr++] = Stop;
			break;
		case 0x00:
			Start = pRecData[0];
			pDataField[WritePtr++] = Start;

			if(pRecData[1]==0xFF)
			{
	//-----------�������ݿ�ӿ�-----------------
				Stop = DB_GetCINum();
			}
			else
			{
				Stop = pRecData[1];
				if(Stop > DB_GetCINum())
					Stop = DB_GetCINum();
			}
			pDataField[WritePtr++] = Stop;
			break;
		case 0x01:
			Start = pRecData[0];
			pDataField[WritePtr++] = pRecData[0];
			pDataField[WritePtr++] = pRecData[1];
			
			if(MAKEWORD(pRecData[2],pRecData[3])==0xFFFF)
			{
	//-----------�������ݿ�ӿ�-----------------
				Stop = DB_GetCINum();
			}
			else
			{
				Stop = pRecData[2];
				if(Stop > DB_GetCINum())
					Stop = DB_GetCINum();
				
			}
			pDataField[WritePtr++] = Stop;
			pDataField[WritePtr++] = 0;
			break;
		default:
			return FALSE;	
	}	
	
	if(Start>=DB_GetCINum())
		return FALSE;
	
	for(i=Start;i<Stop;i++)
	{
	//-----------�������ݿ�ӿ�-----------------
	   DB_ReadCI(i,&dwCIVal);	
	   pDataField[WritePtr++] = LOLOBYTE(dwCIVal);
	   pDataField[WritePtr++] = LOHIBYTE(dwCIVal);
	   pDataField[WritePtr++] = HILOBYTE(dwCIVal);
	   pDataField[WritePtr++] = HIHIBYTE(dwCIVal);
	}
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdCIWithQualityRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField,pRecData;
	WORD		i,Start,Num,Max;
	WORD 		WritePtr;
	DWORD		dwCIVal;
	TRunTimeCI	*pCIBase;
	
	Max = DB_GetCINum();
	pCIBase = DB_GetCIBase();
	
	if(pCIBase == NULL)
		return FALSE;

	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_CIWITHQUALITY;		
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier; 
	Start = MAKEWORD(pRecData[0],pRecData[1]);
	Num = MAKEWORD(pRecData[2],pRecData[3]);
		
	switch(m_RxdHead->ObjQulifier)
	{
		case 0x07:
			// ��ʼ
			if(Start > Max) Start = Max;
			if(Start+Num > Max) Num = Max-Start;
			if(Num > 200) Num = 200;
				
			break;
			
		default:
			Start = 0;
			Num = Max;	
			break;
	}	
	
	pDataField[0] = LOBYTE(Start);
	pDataField[1] = HIBYTE(Start);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);
		
	WritePtr=4;
	
	for(i=0;i<Num;i++)
	{
		DB_ReadCI(i,&dwCIVal);	 
		pDataField[WritePtr++] = LLBYTE(dwCIVal);
		pDataField[WritePtr++] = LHBYTE(dwCIVal);
		pDataField[WritePtr++] = HLBYTE(dwCIVal);
		pDataField[WritePtr++] = HHBYTE(dwCIVal);
		pDataField[WritePtr++] = pCIBase[Start+i].bStatus;
	}
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdSetCIValueRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength;
	BYTE 		byDDNo;
	DWORD		dwCIValue;
	STATUS		rc = DB_ERR_OK;

	//ȡ����������ͷָ��		
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetCFNum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CI;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	//������ֵ
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		// 1��ȡ��ֵ����Ϣ
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		Max = DB_GetCINum();
		
		// ���ȼ��
		if(wFrameLength < (9+4+Num*sizeof(long)))
			return FALSE;
		
		// 2����ż��

		if(StartNo > Max)
			Num = 0;
		// Խ��ʱ
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			
	
		// 3��˳��д��
		for(i=0;i<Num;i++)
		{
			dwCIValue = MAKEDWORD(pRecData[4+i*4],pRecData[5+i*4],pRecData[6+i*4],pRecData[7+i*4]);
			rc |= DB_SetCI(i+StartNo,dwCIValue);
		}
		
		// ����̶�
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				

		// ��䷵��ֵ
		memcpy(pDataField+4,pRecData+4,Num*sizeof(long));
	
		m_TxdWritePtr += 4+Num*sizeof(long);
	}
	else
	{
		//���õ�ȳ�ֵ
		Num = pRecData[0];
		for(i=0;i<Num;i++)
		{
			byDDNo = pRecData[i*5+1];
			dwCIValue = MAKEDWORD(pRecData[i*5+2],pRecData[i*5+3],pRecData[i*5+4],pRecData[i*5+5]);
			DB_SetCI(byDDNo,dwCIValue);
		}
			
		//����������
		pDataField[0] = Num;
		
		m_TxdWritePtr += 3;
		
	}
	
	TxdTail(0); 

	return TRUE;

}

BOOL CSCanbusPro::TxdCoeRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE     	pDataField;
			
	TxdHead(0xC0+m_CurrentReqSeq,FUN_WRITEVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_COE;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NUM_8; 
	
	pDataField[0] = 1;
	pDataField[1] = m_Bo.No;
	pDataField[2] = m_Bo.Command;
	pDataField[3] = LOBYTE(m_Bo.Attrib);
	pDataField[4] = HIBYTE(m_Bo.Attrib);
	
	m_TxdWritePtr += 5;
	
	TxdTail(0);	
	
	return TRUE;	
}

WORD CSCanbusPro::FillBurstBIEvent(BYTE *pBuf,TBurstBI *pEvnet)
{
	pBuf[0] = LOBYTE(pEvnet->wNo);
	pBuf[1] = HIBYTE(pEvnet->wNo);
	pBuf[2] = pEvnet->bStatus;
	
	return 3;
}

WORD CSCanbusPro::FillSOEEvent(BYTE *pBuf,TEventBI *pEvnet)
{
	pBuf[0] = LOBYTE(pEvnet->wNo);
	pBuf[1] = HIBYTE(pEvnet->wNo);
	pBuf[2] = pEvnet->bStatus;
	pBuf[3] = LOLOBYTE(pEvnet->stTime.Lo);
	pBuf[4] = LOHIBYTE(pEvnet->stTime.Lo);
	pBuf[5] = HILOBYTE(pEvnet->stTime.Lo);
	pBuf[6] = HIHIBYTE(pEvnet->stTime.Lo);
	pBuf[7] = LOBYTE(pEvnet->stTime.Hi);
	pBuf[8] = HIBYTE(pEvnet->stTime.Hi);

	return 9;
}

WORD CSCanbusPro::FillOperateEvent(BYTE *pBuf,TMiscEvent *pEvnet)
{
	pBuf[0] = (BYTE)pEvnet->nClass;
	pBuf[1] = (BYTE)(pEvnet->nClass>>8);
	pBuf[2] = LLBYTE(pEvnet->absTime.Lo);
	pBuf[3] = LHBYTE(pEvnet->absTime.Lo);
	pBuf[4] = HLBYTE(pEvnet->absTime.Lo);
	pBuf[5] = HHBYTE(pEvnet->absTime.Lo);
	pBuf[6] = LOBYTE(pEvnet->absTime.Hi);
	pBuf[7] = HIBYTE(pEvnet->absTime.Hi);
	pBuf[8] = LLBYTE(pEvnet->dwOperator);
	pBuf[9] = LHBYTE(pEvnet->dwOperator);
	pBuf[10] = HLBYTE(pEvnet->dwOperator);
	pBuf[11] = HHBYTE(pEvnet->dwOperator);
	pBuf[12] = LLBYTE(pEvnet->dwValue);
	pBuf[13] = LHBYTE(pEvnet->dwValue);
	pBuf[14] = HLBYTE(pEvnet->dwValue);
	pBuf[15] = HHBYTE(pEvnet->dwValue);
	pBuf[16] = LLBYTE(pEvnet->dwAttr);
	pBuf[17] = LHBYTE(pEvnet->dwAttr);
	pBuf[18] = HLBYTE(pEvnet->dwAttr);
	pBuf[19] = HHBYTE(pEvnet->dwAttr);

	return 20;
}

WORD CSCanbusPro::FillRelayEvent(BYTE *pBuf,BYTE *pEvent,BOOL bItem)
{
	TRelayEventHead		tActionHead;	
	
	memcpy(&tActionHead,pEvent,sizeof(TRelayEventHead));

	pBuf[0] = LOBYTE(tActionHead.nINF);
	pBuf[1] = HIBYTE(tActionHead.nINF);
	pBuf[2] = LOBYTE(tActionHead.bStatus);
	pBuf[3] = HIBYTE(tActionHead.bStatus);
	pBuf[4] = LOBYTE(tActionHead.FD_Num);
	pBuf[5] = HIBYTE(tActionHead.FD_Num);
	pBuf[6] = LLBYTE(tActionHead.AbsTimeLo);
	pBuf[7] = LHBYTE(tActionHead.AbsTimeLo);
	pBuf[8] = HLBYTE(tActionHead.AbsTimeLo);
	pBuf[9] = HHBYTE(tActionHead.AbsTimeLo);
	pBuf[10] = LOBYTE(tActionHead.AbsTimeHi);
	pBuf[11] = HIBYTE(tActionHead.AbsTimeHi);
	
	// �����ϵ���
	if(bItem)
		return 12;
	
	memcpy(pBuf+12,pEvent+sizeof(TRelayEventHead),tActionHead.FD_Num*6);
	return 12+tActionHead.FD_Num*6;
}

WORD CSCanbusPro::FillInvalidEvent(BYTE *pBuf)
{
	memset(pBuf,0,12);
	
	pBuf[0] = 0xFF;//LOBYTE(tActionHead.nINF);
	pBuf[1] = 0xFF;//HIBYTE(tActionHead.nINF);
	pBuf[2] = 1;//LOBYTE(tActionHead.bStatus);
	pBuf[3] = 0;//HIBYTE(tActionHead.bStatus);
	pBuf[4] = 0;//LOBYTE(tActionHead.FD_Num);
	pBuf[5] = 0;//HIBYTE(tActionHead.FD_Num);
//	pBuf[6] = LLBYTE(tActionHead.AbsTimeLo);
//	pBuf[7] = LHBYTE(tActionHead.AbsTimeLo);
//	pBuf[8] = HLBYTE(tActionHead.AbsTimeLo);
//	pBuf[9] = HHBYTE(tActionHead.AbsTimeLo);
//	pBuf[10] = LOBYTE(tActionHead.AbsTimeHi);
//	pBuf[11] = HIBYTE(tActionHead.AbsTimeHi);
	return 12;
}

BOOL CSCanbusPro::TxdWrapAttrRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE     	pDataField;
	WORD		Max=0,Read=0,Head=0,Tail=0;
	WORD 		WritePtr=0;
			
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READATTRIB); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= m_RxdHead->ObjType;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;

	switch(m_RxdHead->ObjType)
	{
		case DOC_BIBURST://����仯ң��
			Max = MAX_BIE_NUM;
			DB_GetBIEHead(&Head);
			DB_GetBIETail(&Tail);
			DB_GetBIERead(&Read);
			break;
			
		case DOC_BIEVENT://���������˳���¼
			Max = MAX_SOE_NUM;
			DB_GetSOEHead(&Head);
			DB_GetSOETail(&Tail);
			DB_GetSOERead(&Read);
			break;

		case DOC_OPERATEEVENT://�����¼�
			Max = MAX_MISC_NUM;
			DB_Misc_GetHead(&Head);
			DB_Misc_GetTail(&Tail);
			DB_Misc_GetRead(&Read);
			break;

		case DOC_RELAYALARMREPORT://�澯��¼
			Max = MAX_RALM_NUM;
			DB_GetRelayAlarmReportHead(&Head);
			DB_GetRelayAlarmReportTail(&Tail);
			DB_GetRelayAlarmReportRead(&Read);
			break;

		case DOC_RELAYSTARTREPORT://������¼
			Max = MAX_RSTT_NUM;
			DB_GetRelayStartReportHead(&Head);
			DB_GetRelayStartReportTail(&Tail);
			DB_GetRelayStartReportRead(&Read);
			break;

		case DOC_RELAYACTIONREPORT://������¼
			Max = MAX_RACT_NUM;
			DB_GetRelayActionReportHead(&Head);
			DB_GetRelayActionReportTail(&Tail);
			DB_GetRelayActionReportRead(&Read);
			break;

		default:
			break;
	}
	
	//ȡBIBͷָ��
	pDataField[WritePtr++] = 40;
	pDataField[WritePtr++] = 0;
	pDataField[WritePtr++] = LOBYTE(Max);
	pDataField[WritePtr++] = HIBYTE(Max);
	pDataField[WritePtr++] = LOBYTE(Head);
	pDataField[WritePtr++] = HIBYTE(Head);
	pDataField[WritePtr++] = LOBYTE(Read);
	pDataField[WritePtr++] = HIBYTE(Read);
	pDataField[WritePtr++] = LOBYTE(Tail);
	pDataField[WritePtr++] = HIBYTE(Tail);
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);	
			
	return TRUE;

}

BOOL CSCanbusPro::TxdBIBByReadRes()
{
	TTxdHead 	*pTxdHead;
	TBurstBI 	bib;
	PBYTE     	pDataField,pRecData;
	WORD		i,j,Start,Head,Tail;
	WORD 		WritePtr;
	WORD		wOffset;
	WORD		wNum;
			
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIBURST;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	WritePtr=4;
	
	//ȡBIBͷָ��
	DB_GetBIEHead(&Head);
	DB_GetBIETail(&Tail);
	if(wOffset==0)
		m_WrapHeadPtr = Head;
	
	//�����ȡBIB����ʼλ��
	Start = (m_WrapHeadPtr+wOffset)%MAX_BIE_NUM;

	// ָ���ݴ����ж�
//	if(Tail>=Head)
//	{
//		if((Start < m_WrapHeadPtr)||(Start > Tail))
//			wNum = 0;
//	}

	for(i=Start,j=0;(i != Tail)&&(j<wNum);j++)
	{
	//-----------�������ݿ�ӿ�-----------------
		if(DB_ReadBIE(&bib ,i))
			break;
				
		WritePtr += FillBurstBIEvent(pDataField+WritePtr,&bib);
	   	
	   	i = (i+1)%MAX_BIE_NUM;
	}

	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;

}

BOOL CSCanbusPro::TxdBIEByReadRes()
{
	TTxdHead 	*pTxdHead;
	TEventBI 	soe;
	PBYTE       pDataField,pRecData;
	WORD		i,j,Start,Tail;
	WORD 		WritePtr;
	WORD		wOffset;
	WORD		wNum;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIEVENT;	
	pTxdHead->ObjQulifier = (INDEX_8BIT<<4)+QUALI_ADDR_16;//m_RxdHead->ObjQulifier; 
		
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	WritePtr=4;
	
	//ȡBIEͷָ��
	if(wOffset==0)
		DB_GetSOEHead(&m_WrapHeadPtr);
	DB_GetSOETail(&Tail);

	//�����ȡBIE����ʼλ��
	Start = (m_WrapHeadPtr+wOffset)%MAX_SOE_NUM;

	// ָ���ݴ����ж�
//	if(Tail>=m_WrapHeadPtr)
//	{
//		if((Start < m_WrapHeadPtr)||(Start > Tail))
//			wNum = 0;
//	}
	
	for(i=Start,j=0;(i != Tail)&&(j<wNum);j++)
	{
		//-----------�������ݿ�ӿ�-----------------
		if(DB_ReadSOE(&soe ,i))
				break;

		WritePtr += FillSOEEvent(pDataField+WritePtr,&soe);
	 	
	 	i = (i+1)%MAX_SOE_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}


BOOL CSCanbusPro::TxdOptEventByReadRes()
{
	TTxdHead 		*pTxdHead;
 	TMiscEvent		OptEvent;
 	PBYTE     		pDataField,pRecData;
	WORD			i,j,Start;
	WORD 			WritePtr;
	WORD			wOffset;
	WORD			wNum,wTail;
			
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_OPERATEEVENT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	
    WritePtr = 4;
	//ȡLogEventͷָ��
	if(wOffset==0)
		DB_Misc_GetHead(&m_OptEventPtr);

	//�����ȡLogEvent����ʼλ��
	Start = (m_OptEventPtr+wOffset)%MAX_MISC_NUM;

	DB_Misc_GetTail(&wTail);

	if(wTail>=m_OptEventPtr)
	{
		if((Start < m_WrapHeadPtr)||(Start > wTail))
			wNum = 0;
	}
	
	for(i=Start,j=0;(i!=wTail)&&(j<wNum);j++)
	{
		// �ж�������
		if((WritePtr+sizeof(TMiscEvent)+13) >= m_nMaxFrameLen)
			break;

		//-----------�������ݿ�ӿ�-----------------
		DB_Misc_Read(i,&OptEvent);

		WritePtr += FillOperateEvent(pDataField+WritePtr,&OptEvent);

		i = (i+1)%MAX_MISC_NUM;
	}
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;

	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayAlarmReportByReadRes(BOOL bList)
{
	TTxdHead 			*pTxdHead;
	TRelayEventHead		*pActionHead;	
	PBYTE				pRecData;
	PBYTE       		pDataField;
	TRelayAlarmEv		tReport;
	WORD				Event_tail,Read_Start,Read_Num;
	WORD				i,j,WritePtr,nItemLen;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_RELAYALARMREPORT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
		
	Read_Start = MAKEWORD(pRecData[0],pRecData[1]);
	Read_Num = MAKEWORD(pRecData[2],pRecData[3]);

	WritePtr=4;
	
	//ȡBIEͷָ��
	if(Read_Start==0)
		DB_GetRelayAlarmReportHead(&m_WrapHeadPtr);
	DB_GetRelayAlarmReportTail(&Event_tail);

	//�����ȡBIE����ʼλ��
	Read_Start = (m_WrapHeadPtr+Read_Start)%MAX_RALM_NUM;

	// ָ���ݴ����ж�
//	if(Event_tail>=m_WrapHeadPtr)
//	{
//		if((Read_Start < m_WrapHeadPtr)||(Read_Start > Event_tail))
//			Read_Num = 0;
//	}
	
	for(i=Read_Start,j=0;(i != Event_tail)&&(j<Read_Num);j++)
	{
		if(DB_ReadRelayAlarmReport(&tReport,i))
		{
			// ���ȼ�� 
			if((WritePtr+12) > (m_nMaxFrameLen-13))
				break;

			// �����Ч����
			WritePtr += FillInvalidEvent(pDataField+WritePtr);
			LogError("TxdRelayAlarmReportByReadRes",FILE_LINE,"Read relay alarm report(%d) error!",i);
			i = (i+1)%MAX_RALM_NUM;
			continue;
		}

		pActionHead = &tReport.tEvent.tAction;

		if(bList)
			nItemLen = sizeof(TRelayEventHead);
		else
			nItemLen = sizeof(TRelayEventHead)+pActionHead->FD_Num*6;
		
		// ���ȼ�� 
		if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
			break;

		WritePtr += FillRelayEvent(pDataField+WritePtr,(BYTE *)pActionHead,bList);		
		
	 	i = (i+1)%MAX_RALM_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayStartReportByReadRes(BOOL bList)
{
	TTxdHead 			*pTxdHead;
	TRelayEventHead		*pActionHead;	
	PBYTE				pRecData;
	PBYTE       		pDataField;
	TRelayStartEv		tReport;
	WORD				Event_tail,Read_Start,Read_Num;
	WORD				i,j,WritePtr;
	WORD				nItemLen;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_RELAYSTARTREPORT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
		
	Read_Start = MAKEWORD(pRecData[0],pRecData[1]);
	Read_Num = MAKEWORD(pRecData[2],pRecData[3]);

	WritePtr=4;
	
	//ȡBIEͷָ��
	if(Read_Start==0)
		DB_GetRelayStartReportHead(&m_WrapHeadPtr);
	DB_GetRelayStartReportTail(&Event_tail);

	//�����ȡBIE����ʼλ��
	Read_Start = (m_WrapHeadPtr+Read_Start)%MAX_RSTT_NUM;

	// ָ���ݴ����ж�
//	if(Event_tail>=m_WrapHeadPtr)
//	{
//		if((Read_Start < m_WrapHeadPtr)||(Read_Start > Event_tail))
//			Read_Num = 0;
//	}
	
	for(i=Read_Start,j=0;(i != Event_tail)&&(j<Read_Num);j++)
	{
		if(DB_ReadRelayStartReport(&tReport,i))
			continue;

		pActionHead = &tReport.tEvent.tAction;

		if(bList)
			nItemLen = sizeof(TRelayEventHead);
		else
			nItemLen = sizeof(TRelayEventHead)+pActionHead->FD_Num*6;
		
		// ���ȼ�� 
		if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
			break;

		WritePtr += FillRelayEvent(pDataField+WritePtr,(BYTE *)pActionHead,bList);		
		
	 	i = (i+1)%MAX_RSTT_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayActionReportByReadRes(BOOL bList)
{
	TTxdHead 			*pTxdHead;
	TRelayEventHead		tActionHead;	
	PBYTE				pRecData;
	PBYTE       		pDataField;
	PBYTE				pActionData;	
	TRelayActionEv		tReport;
	WORD				Event_tail,Read_Start,Read_Num;
	WORD				i,j,m,WritePtr;
	WORD				nItemLen;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_RELAYACTIONREPORT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
		
	Read_Start = MAKEWORD(pRecData[0],pRecData[1]);
	Read_Num = MAKEWORD(pRecData[2],pRecData[3]);

	WritePtr=4;
	
	//ȡBIEͷָ��
	if(Read_Start==0)
		DB_GetRelayActionReportHead(&m_WrapHeadPtr);
	DB_GetRelayActionReportTail(&Event_tail);

	//�����ȡBIE����ʼλ��
	Read_Start = (m_WrapHeadPtr+Read_Start)%MAX_RACT_NUM;

	// ָ���ݴ����ж�
//	if(Event_tail>=m_WrapHeadPtr)
//	{
//		if((Read_Start < m_WrapHeadPtr)||(Read_Start > Event_tail))
//			Read_Num = 0;
//	}
	
	for(i=Read_Start,j=0;(i != Event_tail)&&(j<Read_Num);j++)
	{
		if(DB_ReadRelayActionReport(&tReport,i))
		{
			// ���ȼ�� 
			if((WritePtr+14) > (m_nMaxFrameLen-13))
				break;

			// �����Ч����
			pDataField[WritePtr++] = 1;
			pDataField[WritePtr++] = 0;
			WritePtr += FillInvalidEvent(pDataField+WritePtr);
			i = (i+1)%MAX_RACT_NUM;
			
			LogError("TxdRelayActionReportByReadRes",FILE_LINE,"Read relay action report(%d) error!",i);
			continue;
		}
		
		// ����һ���ֱ��泤��
		pActionData = (BYTE *)&tReport.tEvent.tAction;	
		memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));
		if((WritePtr+2+sizeof(TRelayEventHead)+tActionHead.FD_Num*6) > (m_nMaxFrameLen-13))
		{
			pDataField[WritePtr++] = 0;
			pDataField[WritePtr++] = 0;
			break;
		}
		
		pDataField[WritePtr++] = LOBYTE(tReport.tEvent.Num);
		pDataField[WritePtr++] = HIBYTE(tReport.tEvent.Num);

		for(m=0;m<tReport.tEvent.Num;m++)
		{
			memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));

			if(bList)
				nItemLen = sizeof(TRelayEventHead);
			else
				nItemLen = sizeof(TRelayEventHead)+tActionHead.FD_Num*6;
			
			// ���ȼ��	
			if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
				break;

			WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,bList);

			pActionData += nItemLen;

			// ��ȡ�б�ʱֻ�����һ���ֱ���
			if(bList)
				m = tReport.tEvent.Num;
		}

		// ���������
		if(m != tReport.tEvent.Num)
			break;
		
	 	i = (i+1)%MAX_RACT_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);	
			
	return TRUE;
}


//��дָ���ѯ
BOOL CSCanbusPro::TxdBIBByWriteRes()
{
	TTxdHead 	*pTxdHead;
	TBurstBI 	bib;
	PBYTE     	pDataField,pRecData;
	WORD		i,j,Start,Stop,Head,Tail;
	WORD 		WritePtr;
	WORD		wOffset,wNum;
			
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYWRITE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIBURST;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	WritePtr=4;
	
	// ȡBIBͷβָ��
	DB_GetBIEHead(&Head);
	DB_GetBIETail(&Tail);

	// �����ȡBIB����ͣλ��
	Start = (Tail+MAX_BIE_NUM-1-wOffset)%MAX_BIE_NUM;
	Stop = (Head+MAX_BIE_NUM-1)%MAX_BIE_NUM;
		
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
	//-----------�������ݿ�ӿ�-----------------
		if(DB_ReadBIE(&bib ,i))
			break;
				
		WritePtr += FillBurstBIEvent(pDataField+WritePtr,&bib);

		// ����	   	
	   	i = (i+MAX_BIE_NUM-1)%MAX_BIE_NUM;
	}

	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;

}

BOOL CSCanbusPro::TxdBIEByWriteRes()
{
	TTxdHead 	*pTxdHead;
	TEventBI 	soe;
	PBYTE       pDataField,pRecData;
	WORD		i,j,Start,Stop,Head,Tail;
	WORD 		WritePtr;
	WORD		wOffset,wNum;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYWRITE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_BIEVENT;	
	pTxdHead->ObjQulifier = (INDEX_8BIT<<4)+QUALI_ADDR_16;//m_RxdHead->ObjQulifier; 
		
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	WritePtr=4;
	
	// ȡSOEͷβָ��
	DB_GetSOEHead(&Head);
	DB_GetSOETail(&Tail);

	// �����ȡBIB����ͣλ��
	Start = (Tail+MAX_SOE_NUM-1-wOffset)%MAX_SOE_NUM;
	Stop = (Head+MAX_SOE_NUM-1)%MAX_SOE_NUM;
		
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		//-----------�������ݿ�ӿ�-----------------
		if(DB_ReadSOE(&soe ,i))
				break;

		WritePtr += FillSOEEvent(pDataField+WritePtr,&soe);
	 	
		// ����	   	
	   	i = (i+MAX_SOE_NUM-1)%MAX_SOE_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}


BOOL CSCanbusPro::TxdOptEventByWriteRes()
{
	TTxdHead 		*pTxdHead;
 	TMiscEvent		OptEvent;
 	PBYTE     		pDataField,pRecData;
	WORD			i,j,Start,Stop,Head,Tail;
	WORD 			WritePtr;
	WORD			wOffset,wNum;
			
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYWRITE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_OPERATEEVENT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	
    WritePtr = 4;

	// ȡ��������ͷβָ��
	DB_Misc_GetHead(&Head);
	DB_Misc_GetTail(&Tail);

	// �����ȡ���������¼����ͣλ��
	Start = (Tail+MAX_MISC_NUM-1-wOffset)%MAX_MISC_NUM;
	Stop = (Head+MAX_MISC_NUM-1)%MAX_MISC_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		// �ж�������
		if((WritePtr+sizeof(TMiscEvent)+13) >= m_nMaxFrameLen)
			break;

		//-----------�������ݿ�ӿ�-----------------
		DB_Misc_Read(i,&OptEvent);

		WritePtr += FillOperateEvent(pDataField+WritePtr,&OptEvent);

		// ����	   	
	   	i = (i+MAX_MISC_NUM-1)%MAX_MISC_NUM;
	}
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;

	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayAlarmReportByWriteRes(BOOL bList)
{
	TTxdHead 			*pTxdHead;
	TRelayEventHead		*pActionHead;	
	PBYTE				pRecData;
	PBYTE       		pDataField;
	TRelayAlarmEv		tReport;
	WORD				i,j,Start,Stop,Head,Tail;
	WORD 				WritePtr;
	WORD				wOffset,wNum,nItemLen;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYWRITE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	if(bList)
		pTxdHead->ObjType 	= DOC_RELAYALARMITEM;	
	else
		pTxdHead->ObjType 	= DOC_RELAYALARMREPORT;	
		
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
		
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);

	WritePtr=4;
	
	//ȡAlarmͷβָ��
	DB_GetRelayAlarmReportHead(&Head);
	DB_GetRelayAlarmReportTail(&Tail);

	// �����ȡ�澯��¼����ͣλ��
	Start = (Tail+MAX_RALM_NUM-1-wOffset)%MAX_RALM_NUM;
	Stop = (Head+MAX_RALM_NUM-1)%MAX_RALM_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		if(DB_ReadRelayAlarmReport(&tReport,i))
		{
			// ���ȼ�� 
			if((WritePtr+12) > (m_nMaxFrameLen-13))
				break;
			
			// �����Ч����
			WritePtr += FillInvalidEvent(pDataField+WritePtr);
			LogError("TxdRelayAlarmReportByWriteRes",FILE_LINE,"Read relay alarm report(%d) error!",i);
			i = (i+MAX_RALM_NUM-1)%MAX_RALM_NUM;
			continue;
		}

		pActionHead = &tReport.tEvent.tAction;

		if(bList)
			nItemLen = sizeof(TRelayEventHead);
		else
			nItemLen = sizeof(TRelayEventHead)+pActionHead->FD_Num*6;
		
		// ���ȼ�� 
		if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
			break;

		WritePtr += FillRelayEvent(pDataField+WritePtr,(BYTE *)pActionHead,bList);		
		
		// ����	   	
	   	i = (i+MAX_RALM_NUM-1)%MAX_RALM_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayStartReportByWriteRes(BOOL bList)
{
	TTxdHead 			*pTxdHead;
	TRelayEventHead		*pActionHead;	
	PBYTE				pRecData;
	PBYTE       		pDataField;
	TRelayStartEv		tReport;
	WORD				i,j,Start,Stop,Head,Tail;
	WORD 				WritePtr;
	WORD				wOffset,wNum,nItemLen;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYWRITE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	if(bList)
		pTxdHead->ObjType 	= DOC_RELAYSTARTITEM;	
	else
		pTxdHead->ObjType 	= DOC_RELAYSTARTREPORT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
		
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);

	WritePtr=4;
	
	//ȡStartͷβָ��
	DB_GetRelayStartReportHead(&Head);
	DB_GetRelayStartReportTail(&Tail);

	// �����ȡ������¼����ͣλ��
	Start = (Tail+MAX_RSTT_NUM-1-wOffset)%MAX_RSTT_NUM;
	Stop = (Head+MAX_RSTT_NUM-1)%MAX_RSTT_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		if(DB_ReadRelayStartReport(&tReport,i))
			continue;

		pActionHead = &tReport.tEvent.tAction;

		if(bList)
			nItemLen = sizeof(TRelayEventHead);
		else
			nItemLen = sizeof(TRelayEventHead)+pActionHead->FD_Num*6;
		
		// ���ȼ�� 
		if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
			break;

		WritePtr += FillRelayEvent(pDataField+WritePtr,(BYTE *)pActionHead,bList);		
		
		// ����	   	
	   	i = (i+MAX_RSTT_NUM-1)%MAX_RSTT_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdRelayActionReportByWriteRes(BOOL bList)
{
	TTxdHead 			*pTxdHead;
	TRelayEventHead		tActionHead;	
	PBYTE				pRecData;
	PBYTE       		pDataField;
	PBYTE				pActionData;	
	TRelayActionEv		tReport;
	WORD				i,j,m,Start,Stop,Head,Tail;
	WORD 				WritePtr;
	WORD				wOffset,wNum,nItemLen;
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYWRITE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	if(bList)
		pTxdHead->ObjType 	= DOC_RELAYACTIONITEM;	
	else
		pTxdHead->ObjType 	= DOC_RELAYACTIONREPORT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
		
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);

	WritePtr=4;
	
	//ȡͷβָ��
	DB_GetRelayActionReportHead(&Head);
	DB_GetRelayActionReportTail(&Tail);

	// �����ȡ����ͣλ��
	Start = (Tail+MAX_RACT_NUM-1-wOffset)%MAX_RACT_NUM;
	Stop = (Head+MAX_RACT_NUM-1)%MAX_RACT_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		if(DB_ReadRelayActionReport(&tReport,i))
		{
			// ���ȼ�� 
			if((WritePtr+14) > (m_nMaxFrameLen-13))
				break;

			// �����Ч����
			pDataField[WritePtr++] = 1;
			pDataField[WritePtr++] = 0;
			WritePtr += FillInvalidEvent(pDataField+WritePtr);
			i = (i+MAX_RACT_NUM-1)%MAX_RACT_NUM;
			
			LogError("TxdRelayActionReportByWriteRes",FILE_LINE,"Read relay action report(%d) error!",i);
			continue;
		}
		
		pActionData = (BYTE *)&tReport.tEvent.tAction;
		memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));

		if(bList)
		{
			// ��ȡ�б�ʱֻ�����һ���ֱ���
			nItemLen = sizeof(TRelayEventHead);
			tReport.tEvent.Num = 1;
		}
		else
			nItemLen = sizeof(TRelayEventHead)+tActionHead.FD_Num*6;
			
		// ��鱨�泤��
		if((WritePtr+2+nItemLen) > (m_nMaxFrameLen-13))
		{
			pDataField[WritePtr++] = 0;
			pDataField[WritePtr++] = 0;
			break;
		}

		// ����Ŀ�����ֱ������
		if(!bList)
		{
			pDataField[WritePtr++] = LOBYTE(tReport.tEvent.Num);
			pDataField[WritePtr++] = HIBYTE(tReport.tEvent.Num);
		}
		
		for(m=0;m<tReport.tEvent.Num;m++)
		{
			memcpy(&tActionHead,pActionData,sizeof(TRelayEventHead));

			if(bList)
				nItemLen = sizeof(TRelayEventHead);
			else
				nItemLen = sizeof(TRelayEventHead)+tActionHead.FD_Num*6;
			
			// ���ȼ��	
			if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
				break;

			WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,bList);
			pActionData += nItemLen;
		}

		// ���������
		if(m != tReport.tEvent.Num)
			break;
		
		// ����	   	
	   	i = (i+MAX_RACT_NUM-1)%MAX_RACT_NUM;
	 }
	 		
	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdVarRes()
{
	/*
	TRxdHead 	*pRxdHead;
	TTxdHead 	*pTxdHead;
	TVarOut 	VarOut;
	PBYTE         pDataField,pRecData;
	BYTE 		WritePtr;
	DWORD		dwOffset;
	WORD		wVarName,wLen;
			
	pRxdHead = m_RxdHead;
	pRecData = (PBYTE)&pRxdHead->Range;
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	WritePtr=0;
	
	pTxdHead->START  = 0x68;
	pTxdHead->DestAddrLO  = LOBYTE(m_wDAddr);
	pTxdHead->DestAddrHI  = HIBYTE(m_wDAddr);
	pTxdHead->SourceAddrLO  = LOBYTE(m_wSAddr);
	pTxdHead->SourceAddrHI  = HIBYTE(m_wSAddr);
	pTxdHead->TControl   	= 0xC0;	
	pTxdHead->APPControl  	= 0xC0+m_CurrentReqSeq;
	pTxdHead->FunCode  	= 0x80+FUN_QUERYLOGBYOFF;
	pTxdHead->Status  	= GetStationStatus();	
	pTxdHead->ObjType 	= DOC_VARIAL;
	
	pTxdHead->ObjQulifier = pRxdHead->ObjQulifier; 
	
	//����������
	pDataField[0] =1;
	
	//ȡ����������Ϣ
	wVarName = MAKEWORD(pRecData[1],pRecData[2]);
	dwOffset = MAKEDWORD(pRecData[3],pRecData[4],pRecData[5],pRecData[6]);
	wLen = MAKEWORD(pRecData[7],pRecData[8]);
	
	//������ѯ
	if(DB_LookVar(wVarName,dwOffset,wLen,&VarOut))
		return  FALSE;
	
	if(VarOut.nLen>m_nMaxFrameLen-sizeof(TTxdHead)-3)
		return  FALSE;
		
	//������
	memcpy(&pDataField[1],&pRecData[1],6);
	pDataField[7] = LOBYTE(VarOut.nLen);
	pDataField[8] = HIBYTE(VarOut.nLen);
	
	DB_ReadVar(&VarOut,&pDataField[9],VarOut.nLen);
	
    WritePtr += 9+VarOut.nLen;	
        
	pTxdHead->Length  = WritePtr+10; 
	
	pDataField[WritePtr] = LPC(0,&pTxdHead->START,pTxdHead->Length+2);
	
	WriteComm(pTxdHead->Length+3,m_wDAddr,0);
			
*/
	return TRUE;
}

BOOL CSCanbusPro::TxdWriteVarRes()
{
	/*
	TRxdHead 	*pRxdHead;
	TTxdHead 	*pTxdHead;
	TVarOut 	VarOut;
	PBYTE         pDataField,pRecData;
	BYTE 		WritePtr;
	DWORD		dwOffset;
	WORD		wVarName,wLen;
			
	pRxdHead = m_RxdHead;
	pRecData = (PBYTE)&pRxdHead->Range;
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	WritePtr=0;
	
	pTxdHead->START  = 0x68;
	pTxdHead->DestAddrLO  = LOBYTE(m_wDAddr);
	pTxdHead->DestAddrHI  = HIBYTE(m_wDAddr);
	pTxdHead->SourceAddrLO  = LOBYTE(m_wSAddr);
	pTxdHead->SourceAddrHI  = HIBYTE(m_wSAddr);
	pTxdHead->TControl   	= 0xC0;	
	pTxdHead->APPControl  	= 0xC0+m_CurrentReqSeq;
	pTxdHead->FunCode  	= 0x80+FUN_WRITEVALUE;
	pTxdHead->Status  	= GetStationStatus();	
	pTxdHead->ObjType 	= DOC_VARIAL;
	
	pTxdHead->ObjQulifier = pRxdHead->ObjQulifier; 
	
	//����������
	pDataField[0] =1;
	
	//ȡ����������Ϣ
	wVarName = MAKEWORD(pRecData[1],pRecData[2]);
	dwOffset = MAKEDWORD(pRecData[3],pRecData[4],pRecData[5],pRecData[6]);
	wLen = MAKEWORD(pRecData[7],pRecData[8]);
	
	//������ѯ
	if(DB_LookVar(wVarName,dwOffset,wLen,&VarOut))
		return  FALSE;
	
	if(VarOut.nLen>m_nMaxFrameLen-sizeof(TTxdHead)-3)
		return  FALSE;
		
	//д����
	DB_WriteVar(&VarOut,&pRecData[9],VarOut.nLen);
	
	memcpy(&pDataField[1],&pRecData[1],8);	
    WritePtr += 9;	
        
	pTxdHead->Length  = WritePtr+10; 
	
	pDataField[WritePtr] = LPC(0,&pTxdHead->START,pTxdHead->Length+2);
	
	WriteComm(pTxdHead->Length+3,m_wDAddr,0);
*/
	
	return TRUE;
}
BOOL CSCanbusPro::TxdLogInfoRes()
{
	TTxdHead 		*pTxdHead;
 	TRecordBlock	LogInfo;
 	PBYTE         	pDataField,pRecData;
	WORD			i,j,Start;
	WORD 			WritePtr;
	WORD			wOffset;
	WORD			wNum,wTail;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_QUERYLOGBYOFF); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_LOGERROR;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	wOffset = MAKEWORD(pRecData[0],pRecData[1]);
	wNum = MAKEWORD(pRecData[2],pRecData[3]);
	WritePtr=4;
	
	//ȡLogInfoͷָ��
	if(wOffset==0)
		DB_GetRunRecordHead(&m_LogInfoPtr);

	//�����ȡLogInfo����ʼλ��
	Start = (m_LogInfoPtr+wOffset)%MAX_RUNREC_NUM;

	DB_GetRunRecordTail(&wTail);
	
	if(wTail>=m_LogInfoPtr)
	{
		if((Start < m_LogInfoPtr)||(Start > wTail))
			wNum = 0;
	}
	
	for(i=Start,j=0;(i!=wTail)&&(j<wNum);j++)
	{
		//-----------�������ݿ�ӿ�-----------------
		DB_ReadRunRecord(&LogInfo ,i);

		// �ж�������
		if((WritePtr+LogInfo.nCharSize+1+2+13) >= m_nMaxFrameLen)
			break;
		
		pDataField[WritePtr++] = LOBYTE(LogInfo.nCharSize+1);
		pDataField[WritePtr++] = HIBYTE(LogInfo.nCharSize+1);
					
		memcpy(&pDataField[WritePtr],LogInfo.szRecordMsg,LogInfo.nCharSize);
		WritePtr +=LogInfo.nCharSize; 

		// ���ַ�0��β
		pDataField[WritePtr++] = '\0';   		
			        	   			
		i = (i+1)%MAX_RUNREC_NUM;
	}

	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = j;
	pDataField[3] = 0;

	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
			
	return TRUE;

}

BOOL CSCanbusPro::TxdAbsTimeRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	TAbsTime 	AbsTime;
			
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_ABSTIME;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 	 			
	
	ReadAbsTime(&AbsTime);
	
	pDataField[0] = LOLOBYTE(AbsTime.Lo);
	pDataField[1] = LOHIBYTE(AbsTime.Lo);
	pDataField[2] = HILOBYTE(AbsTime.Lo);
	pDataField[3] = HIHIBYTE(AbsTime.Lo);
	pDataField[4] = LOBYTE(AbsTime.Hi);
	pDataField[5] = HIBYTE(AbsTime.Hi);
	
	m_TxdWritePtr += 6;
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdWriteSETRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,AreaNo,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	DWORD		dwSET;
	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SETTING;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	WritePtr = 0;
	Max = DB_GetSETNum();
	
	switch(m_RxdHead->FunCode)
	{
		case FUN_SELECT:
		case FUN_DIROPERATE:
			//������ֵ
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_32)
			{
				m_SET.SETNo = SET_VALUE_ALL;
				
				// 1��ȡ��ֵ����Ϣ
				AreaNo = MAKEWORD(pRecData[0],pRecData[1]);
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				Num = MAKEWORD(pRecData[4],pRecData[5]);
				
				// ���ȼ��
				if(wFrameLength < (9+8+Num*sizeof(float)))
					return FALSE;
				
				// 2����ż��
				if(StartNo == 0)
				{
					 m_SET.AreaNo = AreaNo;
					 m_SET.Off = 0;
				}
				else
				{
					// ���ż��
					if(AreaNo != m_SET.AreaNo)
						Num = 0;

					// ����˳��д��
					else if(StartNo != m_SET.Off)
						Num = 0;
					
					// Խ��ʱ
					else if(StartNo >= Max)
						Num = 0;
						
					else if((StartNo+Num)>Max)
						Num = Max-m_SET.Off;	
				}
				
				// 3����˳���ж�
				for(i=0;i<Num;i++)
				{
					dwSET = MAKEDWORD(pRecData[8+i*4],pRecData[9+i*4],pRecData[10+i*4],pRecData[11+i*4]);
					m_SET.pSET[m_SET.Off] = *(float *)&dwSET;
					m_SET.Off++;
				}

				// ������ֵд��
				if(m_RxdHead->APPControl&0x40)
				{
					// �������
					if(m_SET.Off != Max)
					{
						pTxdHead->Status |= STATUS_CTL_ERROR;
						Num = 0;
					}
					else
					{
						rc = DB_SetSETValue(AreaNo, SET_VALUE_ALL, m_SET.pSET);
						if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
							rc |= DB_SetSETValueConfirm(AreaNo, SET_VALUE_ALL,DB_EXECUTE);
							
						if(rc != DB_ERR_OK)
						{
							pTxdHead->Status |= STATUS_CTL_ERROR;
							Num = 0;
						}
					}
				}
				
				// ����̶�
				memcpy(pDataField,pRecData,4);
				pDataField[4] = LOBYTE(Num);
				pDataField[5] = HIBYTE(Num);
				pDataField[6] = 0;
				pDataField[7] = 0;
				
				// ��䷵��ֵ
				memcpy(pDataField+8,pRecData+8,Num*sizeof(float));

				WritePtr = 8+Num*sizeof(float);
			}
			else //��ʱ��֧��һ����ֵ
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);

				// ���ȼ��
				if(wFrameLength < (9+2+Num*8))
					return FALSE;
				
				AreaNo = MAKEWORD(pRecData[2],pRecData[3]);
				StartNo = MAKEWORD(pRecData[4],pRecData[5]);
				m_SET.AreaNo = AreaNo;
				m_SET.SETNo = StartNo;
				
				dwSET = MAKEDWORD(pRecData[6],pRecData[7],pRecData[8],pRecData[9]);
				
				// ��䷵������
				rc = DB_SetSETValue(AreaNo, StartNo, (float *)(DWORD *)&dwSET);
				if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
					rc |= DB_SetSETValueConfirm(AreaNo, StartNo,DB_EXECUTE);
				if(rc != DB_ERR_OK)
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					pDataField[0] = 0;
					pDataField[1] = 0;
					WritePtr = 2;
				}
				else
				{
					pDataField[0] = 1;
					pDataField[1] = 0;
					memcpy(pDataField+2,pRecData+2,8);
					WritePtr = 10;
				}
			}

			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;

			break;
			
		case FUN_OPERATE:
			rc = DB_SetSETValueConfirm(m_SET.AreaNo, m_SET.SETNo,DB_EXECUTE);

			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;
			break;
			
		case FUN_CANCEL:
			rc = DB_SetSETValueConfirm(m_SET.AreaNo, m_SET.SETNo,DB_CANCEL);
			break;

		default:
			return FALSE;
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteSETAreaRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		AreaNo;
	WORD		wFrameLength;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;

	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	
	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SETTINGAREA;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	
	AreaNo = MAKEWORD(pRecData[0],pRecData[1]);
	
	//��������		
	switch(m_RxdHead->FunCode)
	{
		case FUN_SELECT:
		case FUN_DIROPERATE:
			if(wFrameLength < (9+2))
				return FALSE;

			rc = DB_SetCurrentSETAreaNo(AreaNo);
			
			if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
				rc |= DB_SetCurrentSETAreaNoConfirm(AreaNo,DB_EXECUTE);

			m_SET.AreaNo = AreaNo;
				
				//�������
			if(rc != DB_ERR_OK)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				// Ͷ��ʧ��ʱ���ص�ǰ��ֵ����
				pDataField[0] = DB_GetCurrentSETAreaNo();
				pDataField[1] = 0;
			}
			else
				memcpy(&pDataField[0],pRecData,2);

			m_TxdWritePtr += 2;
			break;
			
		case FUN_OPERATE:
			//-----------�������ݿ�ӿ�-----------------
			rc = DB_SetCurrentSETAreaNoConfirm(m_SET.AreaNo,DB_EXECUTE);
			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;
			break;
			
		case FUN_CANCEL:
			rc = DB_SetCurrentSETAreaNoConfirm(m_SET.AreaNo,DB_CANCEL);
			break;
		default:
			return FALSE;
	}

	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteSystemSETRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	DWORD		dwSET;
	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetSystemSETNum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SYSTEMSETTING;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	WritePtr = 0;
	
	switch(m_RxdHead->FunCode)
	{
		case FUN_SELECT:
		case FUN_DIROPERATE:
			//������ֵ
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
			{
				m_SET.SETNo = SET_VALUE_ALL;
					
				// 1��ȡ��ֵ����Ϣ
				StartNo = MAKEWORD(pRecData[0],pRecData[1]);
				Num = MAKEWORD(pRecData[2],pRecData[3]);

				// ���ȼ��
				if(wFrameLength < (9+4+Num*sizeof(float)))
					return FALSE;
				
				// 2����ż��
				if(StartNo == 0)
					 m_SET.Off = 0;
				else
				{
					// ����д���������ż��
					if(StartNo > Max)
						Num = 0;
					// ����˳��д��
					else if(StartNo != m_SET.Off)
						Num = 0;
					// Խ��ʱ
					else if((StartNo+Num)>Max)
						Num = Max-StartNo;			
				}
				
				// 3����˳���ж�
				for(i=0;i<Num;i++)
				{
					dwSET = MAKEDWORD(pRecData[4+i*4],pRecData[5+i*4],pRecData[6+i*4],pRecData[7+i*4]);
					m_SET.pSET[m_SET.Off] = *(float *)&dwSET;
					m_SET.Off++;
				}

				// ������ֵд��
				if(m_RxdHead->APPControl&0x40)
				{
					// �������
					if(m_SET.Off != Max)
					{
						pTxdHead->Status |= STATUS_CTL_ERROR;
						Num = 0;
					}
					else
					{
						rc = DB_SetSystemSETValue(SET_VALUE_ALL, m_SET.pSET);
						if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
							rc |= DB_SetSystemSETValueConfirm(SET_VALUE_ALL,DB_EXECUTE);
						if(rc != DB_ERR_OK)
						{
							pTxdHead->Status |= STATUS_CTL_ERROR;
							Num = 0;
						}
					}
				}

				// ����̶�
				pDataField[0] = pRecData[0];
				pDataField[1] = pRecData[1];
				pDataField[2] = LOBYTE(Num);
				pDataField[3] = HIBYTE(Num);
				
				// ��䷵��ֵ
				memcpy(pDataField+4,pRecData+4,Num*sizeof(float));
				WritePtr = 4+Num*sizeof(float);
			}
			else //��ʱ��֧��һ����ֵ
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);
				// ���ȼ��
				if(wFrameLength < (9+2+Num*6))
					return FALSE;
				
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				dwSET = MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]);

				m_SET.SETNo = StartNo;
				
				// ��䷵��ֵ
				rc = DB_SetSystemSETValue(StartNo, (float *)(DWORD *)&dwSET);
				if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
					rc |= DB_SetSystemSETValueConfirm(StartNo,DB_EXECUTE);
				if(rc != DB_ERR_OK)
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					pDataField[0] = 0;
					pDataField[1] = 0;
					WritePtr = 2;
				}
				else
				{
					pDataField[0] = 1;
					pDataField[1] = 0;
					memcpy(pDataField+2,pRecData+2,6);
					WritePtr = 8;
				}
			}
			break;
			
		case FUN_OPERATE:
			rc = DB_SetSystemSETValueConfirm(m_SET.SETNo,DB_EXECUTE);

			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;
				
			break;
			
		case FUN_CANCEL:
			rc = DB_SetSystemSETValueConfirm(m_SET.SETNo,DB_CANCEL);
			break;
			
		default:
			return FALSE;
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteSWRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		StartNo,Num;
	WORD		wFrameLength,WritePtr;
	BYTE		byStatus;
	BYTE		*pSWStatus = (BYTE *)m_pMemBlock;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SW;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	WritePtr = 0;
	
	switch(m_RxdHead->FunCode)
	{
		case FUN_SELECT:
		case FUN_DIROPERATE:
			//����Ͷ��
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
			{
				m_SET.SETNo = SET_VALUE_ALL;
				StartNo = MAKEWORD(pRecData[0],pRecData[1]);
				Num = MAKEWORD(pRecData[2],pRecData[3]);
				
				// ��䷵��ֵ
				memcpy(pDataField,pRecData,4+Num);
				WritePtr = 4+Num;
								
				// ���ȼ��
				if(wFrameLength < (9+4+Num))
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					break;
				}

				// ��ʽ���
				if((StartNo!=0)||(Num != DB_GetSWNum()))
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					break;
				}

				// д������
				for(int i=0;i<Num;i++)
				{
					if(pRecData[4+i] == 0xAA)
						pSWStatus[i/8] |= 0x01<<(i%8);
					else
						pSWStatus[i/8] &= ~(0x01<<(i%8));
				}
				rc = DB_SetAllSWStatus(pSWStatus);
				if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
					rc |= DB_SetAllSWStatusConfirm();
					
				if(rc != DB_ERR_OK)
					pTxdHead->Status |= STATUS_CTL_ERROR;
								
			}
			else //��ʱ��֧��һ����ѹ��
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);
				// ���ȼ��
				if(wFrameLength < (9+2+Num))
					return FALSE;
				
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				byStatus = (pRecData[4]==0xAA)?SW_STATUS_SET:SW_STATUS_CUT;

				m_SET.SETNo = StartNo;
				
				// ��䷵��ֵ
				rc = DB_SetSWStatus(StartNo, byStatus);
				if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode == FUN_DIROPERATE))
					rc |= DB_SetSWStatusConfirm(StartNo,DB_EXECUTE);

				if(rc != DB_ERR_OK)
					pTxdHead->Status |= STATUS_CTL_ERROR;

				memcpy(pDataField+2,pRecData+2,3);
				WritePtr = 5;
			}
			break;
			
		case FUN_OPERATE:
			if(m_SET.SETNo == SET_VALUE_ALL)
				rc = DB_SetAllSWStatusConfirm();
			else
				rc = DB_SetSWStatusConfirm(m_SET.SETNo);
			
			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;
				
			break;
			
		case FUN_CANCEL:
			if(m_SET.SETNo == SET_VALUE_ALL)
				rc = DB_SetAllSWStatusConfirm(DB_CANCEL);
			else
				rc = DB_SetSWStatusConfirm(m_SET.SETNo,DB_CANCEL);
			break;
			
		default:
			return FALSE;
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;

}

BOOL CSCanbusPro::TxdWriteCFRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	DWORD		dwCF;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetCFNum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CF;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	WritePtr = 0;
	
	switch(m_RxdHead->FunCode)
	{
		case FUN_SELECT:
		case FUN_DIROPERATE:
		case FUN_WRITEVALUE:	
			//������ֵ
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
			{
				m_SET.SETNo = CF_VALUE_ALL;
				
				// 1��ȡ��ֵ����Ϣ
				StartNo = MAKEWORD(pRecData[0],pRecData[1]);
				Num = MAKEWORD(pRecData[2],pRecData[3]);
				// ���ȼ��
				if(wFrameLength < (9+4+Num*sizeof(long)))
					return FALSE;

				rc = DB_ERR_OK;
				
				// 2����ż��
				if(StartNo == 0)
					 m_SET.Off = 0;
				
				// ����д���������ż��
				if(StartNo > Max)
					Num = 0;
				// ����˳��д��
				else if(StartNo != m_SET.Off)
					Num = 0;
				// Խ��ʱ
				else if((StartNo+Num)>Max)
					Num = Max-StartNo;			

				// 3����˳���ж�
				for(i=0;i<Num;i++)
				{
					dwCF = MAKEDWORD(pRecData[4+i*4],pRecData[5+i*4],pRecData[6+i*4],pRecData[7+i*4]);
					m_SET.pSET[m_SET.Off] = *(float *)&dwCF;
					m_SET.Off++;
				}

				// ������ֵд��
				if(m_RxdHead->APPControl&0x40)
				{
					// �������
					if(m_SET.Off != Max)
					{
						pTxdHead->Status |= STATUS_CTL_ERROR;
						Num = 0;
					}
					else
					{
						rc = DB_SetCFValue(CF_VALUE_ALL, (long *)m_SET.pSET);
						
						if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode != FUN_SELECT))
							rc |= DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);

						if(rc != DB_ERR_OK)
						{
							pTxdHead->Status |= STATUS_CTL_ERROR;
							Num = 0;
						}
					}
				}

				// ����̶�
				pDataField[0] = pRecData[0];
				pDataField[1] = pRecData[1];
				pDataField[2] = LOBYTE(Num);
				pDataField[3] = HIBYTE(Num);				
				// ��䷵��ֵ
				memcpy(pDataField+4,pRecData+4,Num*sizeof(long));

				WritePtr = 4+Num*sizeof(long);
			}
			else //��ʱ��֧��һ����ֵ
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);
				// ���ȼ��
				if(wFrameLength < (9+2+Num*6))
					return FALSE;
				
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				dwCF = MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]);

				m_SET.SETNo = StartNo;
				
				// ��䷵��ֵ
				rc = DB_SetCFValue(StartNo, (long *)&dwCF);
				
				if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode != FUN_SELECT))
					rc |= DB_SetCFValueConfirm(StartNo, DB_EXECUTE);
				
				if(rc != DB_ERR_OK)
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					pDataField[0] = 0;
					pDataField[1] = 0;
					WritePtr = 2;
				}
				else
				{
					pDataField[0] = 1;
					pDataField[1] = 0;
					memcpy(pDataField+2,pRecData+2,6);
					WritePtr = 8;
				}
			}
			break;
			
		case FUN_OPERATE:
			rc = DB_SetCFValueConfirm(m_SET.SETNo,DB_EXECUTE);
				
			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;	
			break;
			
		case FUN_CANCEL:
			rc = DB_SetSystemSETValueConfirm(m_SET.SETNo,DB_CANCEL);
			break;

		default:
			return FALSE;
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::RxdReadSETReq()
{
	PBYTE		pRecData;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	pRecData = (PBYTE)&m_RxdHead->Range;
	
	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.AreaNo = MAKEWORD(pRecData[0],pRecData[1]);
		m_SET.SETNo = SET_VALUE_ALL;
		m_SET.Off = 0;
		
		// �Ȼ��涨ֵ
		rc = DB_GetSETFloatValue(m_SET.AreaNo, SET_VALUE_ALL, m_SET.pSET);
		if(rc == DB_ERR_OK)
			SetFlag(SF_SENDRES_SET);
	}

	return TRUE;
}

BOOL CSCanbusPro::RxdReadSystemSETReq()
{
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = SET_VALUE_ALL;
		m_SET.Off = 0;
		
		// �Ȼ��涨ֵ
		rc = DB_GetSystemSETFloatValue(SET_VALUE_ALL, m_SET.pSET);
		if(rc == DB_ERR_OK)
			SetFlag(SF_SENDRES_SYSTEMSET);
	}

	return TRUE;
}

BOOL CSCanbusPro::RxdReadSWReq()
{
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;
	
	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = SET_VALUE_ALL;
		m_SET.Off = 0;
		SetFlag(SF_SENDRES_SW);
	}

	return TRUE;
}

BOOL CSCanbusPro::RxdReadCFReq()
{
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = CF_VALUE_ALL;
		m_SET.Off = 0;
		
		// �Ȼ��涨ֵ
		rc = DB_GetCFValue(CF_VALUE_ALL, (long *)m_SET.pSET);
		if(rc == DB_ERR_OK)
			SetFlag(SF_SENDRES_CF);
	}

	return TRUE;
}

BOOL CSCanbusPro::TxdReadSETRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		AreaNo,Start;
	WORD		Num,Max;
	WORD		WritePtr;
	DWORD		dwSET;
	BYTE		byAppCtrl = 0x20;
//	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Max = DB_GetSETNum();
	AreaNo = m_SET.AreaNo;
	Start = m_SET.Off;	
	Num = PERFRM_NUM_SET;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_SET) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SETTING;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_32;
	
	if(m_SET.SETNo == SET_VALUE_ALL)
	{		
		pDataField[0] = LOBYTE(AreaNo);
		pDataField[1] = HIBYTE(AreaNo);
		pDataField[2] = LOBYTE(Start);
		pDataField[3] = HIBYTE(Start);
		pDataField[4] = LOBYTE(Num);
		pDataField[5] = HIBYTE(Num);
		pDataField[6] = 0;
		pDataField[7] = 0;
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=8;

		for(int i=0;i<Num;i++)
		{
			dwSET = *(DWORD *)&m_SET.pSET[Start+i];
			pDataField[WritePtr++] = LLBYTE(dwSET);
			pDataField[WritePtr++] = LHBYTE(dwSET);
			pDataField[WritePtr++] = HLBYTE(dwSET);
			pDataField[WritePtr++] = HHBYTE(dwSET);
		}

		m_SET.Off += Num;
	}
	/*���ȡ��ֵ�ݲ�����
	else
	{
		// 1��ȡ��ֵ����Ϣ
		AreaNo = MAKEWORD(pRecData[0],pRecData[1]);
		SETNo = MAKEWORD(pRecData[2],pRecData[3]);
		
		// 2�����Ʊ�̶�
		memcpy(pDataField,pRecData,4);

		// 3����ȡ����
		rc = DB_GetSETFloatValue(AreaNo, SETNo, (float *)&dwSET);
		if(rc == DB_ERR_OK)
		{
			pDataField[10] = LLBYTE(dwSET);
			pDataField[11] = LHBYTE(dwSET);
			pDataField[12] = HLBYTE(dwSET);
			pDataField[13] = HHBYTE(dwSET);
		}
		
		// 5����䷵��״̬
		StatusPtr = 4;
		m_TxdWritePtr += 5+sizeof(float);
	}
	*/

	m_TxdWritePtr = WritePtr+13;
	
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_SETTING;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = 1;

	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdReadSETAreaRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		AreaNo;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SETTINGAREA;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	
	AreaNo = DB_GetCurrentSETAreaNo();
	pDataField[0] = LOBYTE(AreaNo);
	pDataField[1] = HIBYTE(AreaNo);
	
	m_TxdWritePtr += 2;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::TxdReadSystemSETRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	DWORD		dwSET;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Max = DB_GetSystemSETNum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_SET;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_SET) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SYSTEMSETTING;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	if(m_SET.SETNo == SET_VALUE_ALL)
	{		
		pDataField[0] = LOBYTE(Start);
		pDataField[1] = HIBYTE(Start);
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			dwSET = *(DWORD *)&m_SET.pSET[Start+i];
			pDataField[WritePtr++] = LLBYTE(dwSET);
			pDataField[WritePtr++] = LHBYTE(dwSET);
			pDataField[WritePtr++] = HLBYTE(dwSET);
			pDataField[WritePtr++] = HHBYTE(dwSET);
		}

		m_SET.Off += Num;
	}

	m_TxdWritePtr = WritePtr+13;
	
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = FUN_READVALUE;
	m_ResInfo.ObjType = DOC_SYSTEMSETTING;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = 1;

	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdReadSWRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	BYTE		SWStatus;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Max = DB_GetSWNum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_SET;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_SET) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SW;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	if(m_SET.SETNo == SET_VALUE_ALL)
	{		
		pDataField[0] = LOBYTE(Start);
		pDataField[1] = HIBYTE(Start);
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			DB_GetSWStatus(Start+i,SWStatus);
			pDataField[WritePtr++] = (SWStatus==SW_STATUS_SET)?0xAA:0x55;
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;

	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = FUN_READVALUE;
	m_ResInfo.ObjType = DOC_SW;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = 1;
	
	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdReadCFRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	DWORD		dwSET;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Max = DB_GetCFNum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_SET;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_SET) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CF;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	if(m_SET.SETNo == CF_VALUE_ALL)
	{		
		pDataField[0] = LOBYTE(Start);
		pDataField[1] = HIBYTE(Start);
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			dwSET = *(DWORD *)&m_SET.pSET[Start+i];
			pDataField[WritePtr++] = LLBYTE(dwSET);
			pDataField[WritePtr++] = LHBYTE(dwSET);
			pDataField[WritePtr++] = HLBYTE(dwSET);
			pDataField[WritePtr++] = HHBYTE(dwSET);
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;
	
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = FUN_READVALUE;
	m_ResInfo.ObjType = DOC_CF;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = 1;

	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

void CSCanbusPro::ClearFileTrans()
{
	//�ͷſռ�
	if(m_File.pData != NULL)
	{
		// �Ǳ�������������Ҫ�ͷ�
		if(m_File.pData != g_pFileDownBuffer)
		{
			OS_MemFree(m_File.pData);
		}
	}
	
	memset(&m_File,0,sizeof(TCanFile));
}

BOOL CSCanbusPro::TxdFileLoad()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecvData;
	PBYTE       pDataFile;
	char    	strFileName[32];	//�ļ���
	DWORD		dwFileLength;		//�ļ�����
	DWORD 		nPerSize;   		//ÿ���ļ�����
	WORD		wFileCrc;			//�ļ�У��
	BYTE		byOptType;			//�ļ���������1����������2����������3������д��4������д
	WORD		wFileSectionNo;
	STATUS		rc;

	//	0������������
	//	1���ļ������쳣
	//	2���ڴ�����쳣
	//	3���ļ���ȡ�쳣
	//	4���ļ�д���쳣
	//	5���ļ����䳬ʱ�жϣ�
	//	6���ļ�У�����
	//	7��ǿ���ж�*/
	
	pRecvData = &m_RxdHead->Range;
	
	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataFile = (PBYTE)&pTxdHead->Range;
	
	pTxdHead->ObjType	= m_RxdHead->ObjType;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	m_File.nTimer = MAX_FILE_WAITTIME;
	
	switch(m_RxdHead->ObjType)
	{
		case DOC_FILEDISP:
			//�����ļ�������Ϣ
			memcpy(pDataFile,pRecvData,42);
			m_TxdWritePtr += 42;
			
			byOptType = pRecvData[0];
			
			memcpy(strFileName,pRecvData+2,32);
			
			//�����������䣬���³�ʼ��
			if(byOptType == FILE_STOP_READ)
			{
				if(m_File.nLastSection >= (m_File.nTotalSection-1))
				{
					ReportMsg("Protocol read file %s end!",strFileName);
				}
				else
				{
					ReportMsg("Protocol read file %s break!",strFileName);
					pDataFile[1] = 7;//ǿ���ж�
				}
				
				//����ļ�������Ϣ
				ClearFileTrans();
			}
			else
			{
				//����ļ�������Ϣ
				ClearFileTrans();
				
				// 1����ȡ�ļ�����
				rc = DAE_GetFileLength(strFileName,&dwFileLength);
				if((rc != DB_ERR_OK)||(dwFileLength > MAX_PROFILE_LEN))//����ļ���2M,�����ļ�
				{
					ReportMsg("Protocol read file %s Length Err!",strFileName);

					pDataFile[1] = 1;//�ļ������쳣
					TxdTail(0);
					
					return TRUE;
				}
							
				// 2��������ʱ�õ��ļ�������
				if(dwFileLength <= MAX_FILE_LEN)
					m_File.pData = g_pFileDownBuffer;
				else
				{
					m_File.pData = (BYTE *)OS_MemAlloc(dwFileLength);
					if( m_File.pData == NULL )
					{
						ReportMsg("Protocol Read File %s MemAlloc Err!",strFileName);
						pDataFile[1] = 2;//�ڴ�����쳣
						TxdTail(0);
						return TRUE;
					}
				}
				
				// 3����ȡ�ļ�����
				rc = DAE_ReadFile(strFileName,m_File.pData,dwFileLength);
				if( rc != DB_ERR_OK)
				{
					ReportMsg("Protocol read file %s Err!",strFileName);
					
					ClearFileTrans();
					
					pDataFile[1] = 3;//�ļ���ȡ�쳣
					TxdTail(0);
					return TRUE;
				}

				m_File.bStatus = FILE_STATUS_BEGINTRANS;
				
				//���°�װ�ļ�����
				wFileCrc = DAE_CheckSum((BYTE *)m_File.pData,dwFileLength);
				pDataFile[34] = LLBYTE(dwFileLength);
				pDataFile[35] = LHBYTE(dwFileLength);
				pDataFile[36] = HLBYTE(dwFileLength);
				pDataFile[37] = HHBYTE(dwFileLength);
//				pDataFile[38] = LOBYTE(MAX_FILE_SECTIONSIZE);//Ŀǰ�ݶ�Ϊ�̶��峤
//				pDataFile[39] = HIBYTE(MAX_FILE_SECTIONSIZE);
				pDataFile[40] = LOBYTE(wFileCrc);
				pDataFile[41] = HIBYTE(wFileCrc);
				
				pDataFile[1] = 0;//�ļ���ȡ����

				// ��ʼ���ļ���Ϣ
				memcpy(m_File.strFileName,strFileName,32);
				m_File.dwFileLength = dwFileLength;
				m_File.wFileCrc = wFileCrc;
				m_File.nLastSection = 0xFFFF;
				m_File.nPerSize = MAKEWORD(pRecvData[38],pRecvData[39]);
				m_File.nTotalSection = (dwFileLength+m_File.nPerSize-1)/m_File.nPerSize;
			}
			
			TxdTail(0); 

			return TRUE;
			
		case DOC_FILEDATA:
			wFileSectionNo = MAKEWORD(pRecvData[0],pRecvData[1]);
			m_File.nLastSection = wFileSectionNo;

			// �����ٻ��Ķκ�Ӧ��
			pDataFile[0] = pRecvData[0];
			pDataFile[1] = pRecvData[1];

			//��δ�������䣬���³�ʼ��
			if(m_File.bStatus != FILE_STATUS_BEGINTRANS)
			{
				ReportMsg("Protocol read file %s unknowed!",m_File.strFileName);
				pDataFile[2] = 0x01;
				m_TxdWritePtr += 3;
			}
			else if(wFileSectionNo >= m_File.nTotalSection)
			{
				ReportMsg("Protocol read file %s overflow!",m_File.strFileName);
				pDataFile[2] = 0x02;
				m_TxdWritePtr += 3;
			}
			else
			{
				pDataFile[2] = 0x00;

				// ����֡��
				nPerSize = m_File.dwFileLength-wFileSectionNo*m_File.nPerSize;
				if(nPerSize >= m_File.nPerSize)
					nPerSize = m_File.nPerSize;
				
				memcpy(pDataFile+3,&m_File.pData[wFileSectionNo*m_File.nPerSize],nPerSize);
				m_TxdWritePtr += 3+nPerSize;
			}

			TxdTail(0);

//			ReportMsg("SCANBUS: recv file load data. SEQ=%d",wFileSectionNo);
			
			return TRUE;
			
		default:
			return FALSE;
	}
}

BOOL CSCanbusPro::TxdFileDown()
{	
	TTxdHead 	*pTxdHead;
	PBYTE		pRecvData;
	PBYTE       pDataFile;
	char    	strFileName[32];	//�ļ���
	DWORD		dwFileLength;		//�ļ�����
	DWORD 		nOffset;   			//�ļ�ƫ��
	WORD 		nPerSize;   		//ÿ���ļ�����
	WORD		wFileCrc;			//�ļ�У��
	BYTE		byOptType;			//�ļ���������1����������2����������3������д��4������д
	WORD		wFileSectionNo;
	STATUS		rc;

	//	0������������
	//	1���ļ������쳣
	//	2���ڴ�����쳣
	//	3���ļ���ȡ�쳣
	//	4���ļ�д���쳣
	//	5���ļ����䳬ʱ�жϣ�
	//	6���ļ�У�����
	//	7��ǿ���ж�*/
	
	pRecvData = &m_RxdHead->Range;
	
	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataFile = (PBYTE)&pTxdHead->Range;
	
	pTxdHead->ObjType	= m_RxdHead->ObjType;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	m_File.nTimer = MAX_FILE_WAITTIME;
	
	switch(m_RxdHead->ObjType)
	{
		case DOC_FILEDISP:
			//����ļ�������Ϣ
			memcpy(pDataFile,pRecvData,42);
			m_TxdWritePtr += 42;
			
			byOptType = pRecvData[0];
			
			memcpy(strFileName,pRecvData+2,32);
			
			dwFileLength = MAKEDWORD(pRecvData[34],pRecvData[35],pRecvData[36],pRecvData[37]);
			nPerSize = MAKEWORD(pRecvData[38],pRecvData[39]);
			wFileCrc = MAKEWORD(pRecvData[40],pRecvData[41]);

			//�����������䣬���³�ʼ��
			if(byOptType == FILE_STOP_WRITE)
			{
				do{
					// 1.����ļ���
					if(strcmp(m_File.strFileName,strFileName) !=0 )
					{
						ReportMsg("Protocol write file %s is unknowed!",strFileName);
						pDataFile[1] = 6;//�ļ�У�����
						break;
					}
					
					// 2.���κ�
					if(m_File.nLastSection != (m_File.nTotalSection-1))
					{
						ReportMsg("Protocol write file %s break!",strFileName);
						pDataFile[1] = 7;//ǿ���ж�
						break;
					}
					
					// 3�����У��
					if(DAE_CheckSum(m_File.pData, dwFileLength) != wFileCrc)
					{
						ReportMsg("Protocol write file %s check error!",strFileName);
						pDataFile[1] = 6;//�ļ�У�����
						break;
					}	
					
					// 4��д���ļ�

					// ��ͣͨ����Ϣ
					StopMsg(MCC_CommIdle);

					rc = DAE_WriteFile(strFileName,m_File.pData,dwFileLength);

					// ����ͨ����Ϣ
					StartMsg(MCC_CommIdle);

					if(rc != DB_ERR_OK)
					{
						ReportMsg("Protocol write file %s save error!",strFileName);
						pDataFile[1] = 4;//�ļ�д���쳣
					}
					else
					{
						ReportMsg("Protocol write file %s save OK!",strFileName);
						pDataFile[1] = 0;//�ļ���������
					}

					TxdTail(0);
					
					// ע���ļ���װ
					for(int i=0;i<strlen(strFileName);i++)
					{
						if((strFileName[i]>='A')&&(strFileName[i]<='Z'))
							strFileName[i] = 'a'+strFileName[i]-'A';
					}
					
					DB_Register_FileDown(strFileName,pDataFile[1]);	
					
				}while(0);
				
				//�ͷſռ�
				ClearFileTrans();
			}
			else if(byOptType == FILE_START_WRITE)
			{
				//�ͷſռ�
				ClearFileTrans();
				
				// 1����ȡ�ļ�����
				if(dwFileLength > MAX_PROFILE_LEN)
				{
					ReportMsg("Protocol Write File %s Length Err!",strFileName);

					pDataFile[1] = 1;//�ļ������쳣
					TxdTail(0);
					
					return TRUE;
				}
							
				// 2��������ʱ�õ��ļ�������
				if(dwFileLength <= MAX_FILE_LEN)
					m_File.pData = g_pFileDownBuffer;
				else
				{
					m_File.pData = (BYTE *)OS_MemAlloc(dwFileLength);
					if( m_File.pData == NULL )
					{
						ReportMsg("Protocol Write File %s MemAlloc Err!",strFileName);
						pDataFile[1] = 2;//�ڴ�����쳣
						TxdTail(0);
						return TRUE;
					}
				}
				
				m_File.bStatus = FILE_STATUS_BEGINTRANS;
				
				//��¼�ļ�����
				memcpy(m_File.strFileName,strFileName,32);
				m_File.dwFileLength = dwFileLength;
				m_File.nPerSize = nPerSize;//Ŀǰ�ݶ�Ϊ�̶��峤
				m_File.nTotalSection = (dwFileLength+nPerSize-1)/nPerSize;
				m_File.nLastSection = 0xFFFF;
				for(int i=0;i<dwFileLength;i++)
					wFileCrc += m_File.pData[i];
				m_File.wFileCrc = wFileCrc;
				
//				m_TxdInfo.pBuf[m_TxdWritePtr++] = 0;
				
				TxdTail(0);
			}
			
			return TRUE;
			
		case DOC_FILEDATA:
			wFileSectionNo = MAKEWORD(pRecvData[0],pRecvData[1]);
			
			//��δ�������䣬���³�ʼ��
			do{
				if(m_File.bStatus != FILE_STATUS_BEGINTRANS)
				{
					ReportMsg("Protocol Write File %s unknowed!",m_File.strFileName);
					pDataFile[2] = 0x01;
					wFileSectionNo = 0;
					break;
				}

				// �κ�Խ���ж�
				if(wFileSectionNo >= m_File.nTotalSection)
				{
					ReportMsg("Protocol Write File %s overflow!",m_File.strFileName);
					pDataFile[2] = 0x02;
					wFileSectionNo = m_File.nLastSection+1;
					break;
				}

				// �κŲ������ж�
				if(m_File.nLastSection == 0xFFFF)
				{
					if(wFileSectionNo != 0)
					{
						ReportMsg("Protocol Write File %s seq Rec %d != 0 error!",m_File.strFileName,wFileSectionNo);
						pDataFile[2] = 0x02;	
						wFileSectionNo = 0;
						break;
					}
				}	
				else if(wFileSectionNo != (m_File.nLastSection+1))
				{
					ReportMsg("Protocol Write File %s seq Rec %d != %d error!",m_File.strFileName,wFileSectionNo,m_File.nLastSection+1);
					pDataFile[2] = 0x02;
					wFileSectionNo = m_File.nLastSection+1;
					break;
				}
				
				pDataFile[2] = 0x00;

				nOffset = wFileSectionNo*m_File.nPerSize;
				
				if(nOffset < m_File.dwFileLength)
				{
					//  ���һ֡����
					if(nOffset+m_File.nPerSize >= m_File.dwFileLength)
						nPerSize = m_File.dwFileLength-nOffset;
					else
						nPerSize = m_File.nPerSize;
					
					//  ��������
					memcpy(&m_File.pData[wFileSectionNo*m_File.nPerSize],pRecvData+3,nPerSize);
				}
				
				// ��¼�ϴ���ȷ���յĶ�
				m_File.nLastSection = wFileSectionNo;
				
				wFileSectionNo++;
			}while(0);
			
			pDataFile[0] = LOBYTE(wFileSectionNo);
			pDataFile[1] = HIBYTE(wFileSectionNo);

			m_TxdWritePtr += 3;

			TxdTail(0);

			return TRUE;
			
		default:
			return FALSE;
	}
}

void CSCanbusPro::YKPocess()
{
	PBYTE       pRecData;
	WORD 		wRealYKID;
	WORD 		wAttr;

	if((m_NetCfg.dwStatus&MASK_BO)==0)
		return;

	pRecData = (PBYTE)&m_RxdHead->Range;
	
	m_Bo.No = pRecData[1];
	m_Bo.Command = m_RxdHead->FunCode;
	m_Bo.Attrib = pRecData[2]&0x03;
	m_Bo.NeedRetFlag = pRecData[2]>>7;
	m_Bo.Info[0] = pRecData[3];
	m_Bo.Info[1] = pRecData[4];
	m_Bo.Info[2] = LOBYTE(m_CommandAddr);
	m_Bo.Info[3] = HIBYTE(m_CommandAddr);

	wRealYKID	=  m_Bo.No;
	wAttr		=  m_Bo.Attrib;
	
	/*	
		if(PBO_OPEN == pBo->Attrib )
			wRealYKID = pBo->No*2;
		else if(PBO_CLOSE ==pBo->Attrib)
			wRealYKID = pBo->No*2+1;
	*/
	
	switch(m_Bo.Command)
	{
	case FUN_SELECT:
		//-----------�������ݿ�ӿ�-----------------
//		ReportMsg("CanBus YK SELECT! No=%d",wRealYKID);
		DB_BOSelect(wRealYKID,wAttr);
//		DB_Register_BO(wRealYKID,wAttr);
		break;
	case FUN_OPERATE:
		//-----------�������ݿ�ӿ�-----------------
//		ReportMsg("CanBus YK Execute! No=%d",wRealYKID);
		DB_BOExec(wRealYKID,wAttr,1);
		DB_Register_BO(wRealYKID,wAttr);
		break;
	case FUN_CANCEL:
		//-----------�������ݿ�ӿ�-----------------
//		ReportMsg("CanBus YK Cancel! No=%d",wRealYKID);
		DB_BOExec(wRealYKID,wAttr,2);
//		DB_Register_BO(wRealYKID,wAttr);
		break;
	case FUN_DIROPERATE:
		//-----------�������ݿ�ӿ�-----------------
//		ReportMsg("CanBus YK DirectExec! No=%d",wRealYKID);
		DB_BODirect(wRealYKID,wAttr);
		DB_Register_BO(wRealYKID,wAttr);
		break;
	}
}

BOOL CSCanbusPro::RxdConfirm()
{
	BYTE		bySeq=15;
	WORD		i,Num=0;
		
	bySeq = (m_RxdHead->APPControl)&APP_SEQ_MASK;

	//��Ϊ��ǰ��Ӧ֡����ȴ�ȷ�ϱ�־
	if(bySeq == m_ResInfo.ResSeq)
	{
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);
		switch(m_ResInfo.ResFunCode&0x7F)
		{
			case FUN_REPORTVALUE:
				switch(m_ResInfo.ObjType)
				{
					case	DOC_BIBURST: 
					//-----------�������ݿ�ӿ�-----------------							
						//������ָ��
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncBIERead();
						
						if(DB_GetBIENum(&Num)!= DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_BURSTBI,FLAG_OFF);
//						else
//							m_wTimer[BIBURST_ID] = m_NetCfg.BIBurstRepTM*100/CAN_TIMER_TICK;
						break;

					case	DOC_BIEVENT:
					//-----------�������ݿ�ӿ�-----------------
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncSOERead();

						if(DB_GetSOENum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_SOE,FLAG_OFF);
//						else
//							m_wTimer[SOE_ID] = m_NetCfg.SoeRepTM*100/CAN_TIMER_TICK;

						break;

					case	DOC_AIEVENT:
					//-----------�������ݿ�ӿ�-----------------
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncAIERead();

						if(DB_GetAIENum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_AIE,FLAG_OFF);

						break;

					case	DOC_RELAYALARMREPORT://�����澯����
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncRelayAlarmReportRead();

						if(DB_GetRelayAlarmReportNum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_RELAYALARM,FLAG_OFF);		
						break;

					case	DOC_RELAYSTARTREPORT://������������
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncRelayStartReportRead();

						if(DB_GetRelayStartReportNum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_RELAYSTART,FLAG_OFF);		
						break;

					case	DOC_RELAYACTIONREPORT://������������
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncRelayActionReportRead();

						if(DB_GetRelayActionReportNum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_RELAYACTION,FLAG_OFF);		
						break;

					case	DOC_LEDSTATUS://LED��״̬
						SetFlag(SF_LEDBURST,FLAG_OFF);		
						break;

					default:								
						break;	
				}
				break;
			
			case FUN_SELECT:
			case FUN_OPERATE:
			case FUN_CANCEL:
			case FUN_DIROPERATE:
				switch(m_ResInfo.ObjType)
				{
					case	DOC_BO: 
						SetFlag(SF_HaveBOReturn,FLAG_OFF);	
						m_wTimer[BORET_ID] = 0;
						break;
					default:
						break;	
				}
				break;

			case FUN_READVALUE:
				switch(m_ResInfo.ObjType)
				{
					case	DOC_SETTING: 	
						// ���ͽ���
						if(m_SET.Off >= DB_GetSETNum())
							SetFlag(SF_SENDRES_SET,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_SET,FLAG_ON);	
						break;
						
					case	DOC_SYSTEMSETTING: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetSystemSETNum())
							SetFlag(SF_SENDRES_SYSTEMSET,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_SYSTEMSET,FLAG_ON);	
						break;

					case	DOC_SW: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetSWNum())
							SetFlag(SF_SENDRES_SW,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_SW,FLAG_ON);	
						break;

					case	DOC_CF: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetCFNum())
							SetFlag(SF_SENDRES_CF,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_CF,FLAG_ON);	
						break;

					case	DOC_BI: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetBINum())
							SetFlag(SF_SENDRES_BIATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BIATTR,FLAG_ON);	
						break;

					case	DOC_BO: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetBONum())
							SetFlag(SF_SENDRES_BOATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BOATTR,FLAG_ON);	
						break;

					case	DOC_DISTDIR: 	
						// ���ͽ���
						if(m_DistDir.nSentCnt >= m_DistDir.nFileNum)
							SetFlag(SF_SENDRES_DISTDIR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_DISTDIR,FLAG_ON);	
						break;
						
					default:
						break;	
				}
				break;

			case FUN_READATTRIB:
				switch(m_ResInfo.ObjType)
				{
					case	DOC_BI: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetBINum())
							SetFlag(SF_SENDRES_BIATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BIATTR,FLAG_ON);	
						break;

					case	DOC_BO: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetBONum())
							SetFlag(SF_SENDRES_BOATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BOATTR,FLAG_ON);	
						break;
						
					case	DOC_MAPBITOBJ: 
						// ���ͽ���
						if(m_SET.Off >= DB_GetMapBitObjNum())
							SetFlag(SF_SENDRES_MBOPARA,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_MBOPARA,FLAG_ON);	
						break;
					default:
						break;	
				}
				break;
				
			default:
				break;	
		}

		// �����ȷ������
		memset(&m_ResInfo,0,sizeof(TResInfo));
	}
	else
		return FALSE;

	return TRUE;
}

void CSCanbusPro::RxdNetCfg()
{
	PBYTE		pBuf;
	WORD		Num;
	DWORD 		Mask;
	
	pBuf	= (PBYTE)&m_RxdHead->Range;
	
	//��λ֡���
	m_CurrentResSeq = APP_RESSEQ_MIN;
	m_CurrentReqSeq = APP_REQSEQ_MAX;
	
	//ȡ��������
	Mask = MAKEDWORD(pBuf[0],pBuf[1],pBuf[2],pBuf[3]);

	if(m_NetCfg.dwStatus!=Mask)
	{
		m_NetCfg.dwStatus = Mask;

		//�����ݿ�ע����������Ϣ,ǿ��ÿ��CAN������ƽ������
		if((m_wPortID == PORT_ID_CANA)||(m_wPortID == PORT_ID_ETHA))
		{
			DAE_SetNetADataMask(m_NetCfg.dwStatus);
			DAE_SetNetBDataMask(MASK_ALL-m_NetCfg.dwStatus);
		}
		else if((m_wPortID == PORT_ID_CANB)||(m_wPortID == PORT_ID_ETHB))
		{
			DAE_SetNetBDataMask(m_NetCfg.dwStatus);
			DAE_SetNetADataMask(MASK_ALL-m_NetCfg.dwStatus);
		}
	}
	
	//����ң�ⶨʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if((m_NetCfg.dwStatus&MASK_AI)&&(DB_GetAINum()!=0))
	{
		m_NetCfg.wAIRepTM = MAKEWORD(pBuf[4],pBuf[5]);
		m_wTimer[AI_ID] = m_NetCfg.wAIRepTM*1000/CAN_TIMER_TICK;
	}
	else
		m_NetCfg.wAIRepTM = 0;
	
	//����ң�Ŷ�ʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if((m_NetCfg.dwStatus&MASK_BI)&&(DB_GetBINum()!=0))
	{
		m_NetCfg.wBIRepTM = MAKEWORD(pBuf[6],pBuf[7]);
		m_wTimer[BI_ID] = m_NetCfg.wBIRepTM*1000/CAN_TIMER_TICK;
	}
	else
		m_NetCfg.wBIRepTM = 0;

#if(SYSTEM_TYPE == _SYS_KF1300_)
	//���ü̵����ֶ�ʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if(m_NetCfg.dwStatus&MASK_LED)
	{
		m_NetCfg.wLEDStatusTM = CAN_LEDREPTM;
		m_wTimer[LED_ID] = CAN_LEDREPTM*1000/CAN_TIMER_TICK;

		// �յ������ȷ���һ��LED״̬
		SetFlag(SF_LEDREPORT);
	}
	else
		m_NetCfg.wLEDStatusTM = 0;
#endif

	//���õ�ȶ�ʱ�������������ʱ��
	//-----------�������ݿ�ӿ�-----------------
	if((m_NetCfg.dwStatus&MASK_CI)&&(DB_GetCINum()!=0))
	{
		m_NetCfg.wCIRepTM = MAKEWORD(pBuf[8],pBuf[9]);
		m_wTimer[CI_ID] = m_NetCfg.wCIRepTM*1000/CAN_TIMER_TICK;
	}
	else
		m_NetCfg.wCIRepTM = 0;
			
	m_NetCfg.AIDeadArea = MAKEWORD(pBuf[10],pBuf[11]);
			
	//���ñ仯ң�ⶨʱ�ڼ��
	if(m_NetCfg.dwStatus&MASK_AIE)
	{
		if(!DB_AIEIsEmpty())
			SetFlag(SF_AIE);	

		m_NetCfg.AIBurstRepTM = MAKEWORD(pBuf[12],pBuf[13]);
		m_wTimer[AIBURST_ID] = (m_NetCfg.AIBurstRepTM*100+CAN_TIMER_TICK-1)/CAN_TIMER_TICK;		
	}
	else
		m_NetCfg.AIBurstRepTM = 0;
	
	//���ñ�λң���ط����		
	if(m_NetCfg.dwStatus&MASK_BIE)
	{
		m_NetCfg.BIBurstRepTM = MAKEWORD(pBuf[14],pBuf[15]);
		DB_GetBIENum(&Num);
		if(Num>0)
			SetFlag(SF_BURSTBI);	
	}
	else
	{
		m_NetCfg.BIBurstRepTM = 0;
		//ȥ����λң�������־
		SetFlag(SF_BURSTBI,FLAG_OFF);
		//ȥ����λң���ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIBURST))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}
	
	//����SOE�ط����	
	if(m_NetCfg.dwStatus&MASK_SOE)
	{
		m_NetCfg.SoeRepTM = MAKEWORD(pBuf[16],pBuf[17]);
		DB_GetSOENum(&Num);
		if(Num>0)
			SetFlag(SF_SOE);	
	}
	else
	{
		m_NetCfg.SoeRepTM = 0;
		//ȥ����λң�������־
		SetFlag(SF_SOE,FLAG_OFF);
		//ȥ����λң���ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIEVENT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//���ñ����澯�����ط����	
	if(m_NetCfg.dwStatus&MASK_RELAYALARMEV)
	{
		DB_GetRelayAlarmReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYALARM);	
	}
	else
	{
		//ȥ�������־
		SetFlag(SF_RELAYALARM,FLAG_OFF);
		//ȥ���ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYALARMREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//���ñ������������ط����	
	if(m_NetCfg.dwStatus&MASK_RELAYSTARTEV)
	{
		DB_GetRelayStartReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYSTART);	
	}
	else
	{
		//ȥ�������־
		SetFlag(SF_RELAYSTART,FLAG_OFF);
		//ȥ���ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYSTARTREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//���ñ������������ط����	
	if(m_NetCfg.dwStatus&MASK_RELAYACTIONEV)
	{
		DB_GetRelayActionReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYACTION);	
	}
	else
	{
		//ȥ�������־
		SetFlag(SF_RELAYACTION,FLAG_OFF);
		//ȥ���ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYACTIONREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

		
	if(m_NetCfg.dwStatus&MASK_TIME)
	{
		m_NetCfg.TimingTM = MAKEWORD(pBuf[18],pBuf[19]);
		m_wTimer[TIMING_ID] = m_NetCfg.TimingTM*60*1000/CAN_TIMER_TICK;
//		ReportMsg("Recv config! TimingTM=%d",m_NetCfg.TimingTM);
	}
	else
		m_NetCfg.TimingTM = 0;
	
	//��ƽ�����ķ��ͼ��	
	m_NetCfg.SaftyRepTM = MAKEWORD(pBuf[22],pBuf[23]);
	m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;

	if(!(m_NetCfg.dwStatus&MASK_BO))
	{	
		//ȥ��ң�ط�У�����־
		SetFlag(SF_HaveBOReturn,FLAG_OFF);
		//ȥ��ң�ط�У�ط���־
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BO))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}
	return;

}

void CSCanbusPro::RxdCoe()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	PBYTE       pRecData;
	BYTE		COEType;
	DWORD		COEValue;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	memcpy(m_Bo.Info,pRecData,9);

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NUM_8))
	{
		COEType = pRecData[1];
		COEValue = MAKEDWORD(pRecData[3],pRecData[4],pRecData[2],pRecData[1]);
	}
	else if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		COEType = pRecData[0];
		COEValue = MAKEDWORD(pRecData[2],pRecData[3],pRecData[1],pRecData[0]);
	}
	else
	{
		ReportMsg("Scanbus: RxdCoe() error ObjQulifier(%d)!!!",m_RxdHead->ObjQulifier);
		return;
	}
	
	switch(COEType)
	{
		case 0:// ��������Ư����
		case 1:// ��������ֵ����			
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AUTOADJUSTSTEP1, RtosGetCurrentTaskId(), COEValue,0,0 );
			return;
			
		case 2:// ����1��Ư����
		case 3:// ����1��ֵ����
		case 4:// ����2��Ư����
		case 5:// ����2��ֵ����
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AUTOADJUSTSTEP1, RtosGetCurrentTaskId(), COEValue,0,0 );
			return;

		default:// ֱ�������У��
			if(m_RxdHead->FunCode== FUN_SELECTWITHVALUE)
			{
				TxdSelectAOCoeRes();
			}
			else
			{
				TxdWriteAOCoeRes();
			}
			break;
	}
#else
	PBYTE		pRecData;
	WORD		wU,wI;
	
	// ֡�޶��������ϸ��飬�ݽ�֧��03
	if(m_RxdHead->ObjQulifier != ((INDEX_SIZE_0<<4)+QUALI_NUM_8))
		return;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	// ����֡��Ϣ
	memcpy(m_Bo.Info,pRecData,9);
	
	// ȡ������ѹУ��ֵ
	for(int i=0;i<m_RxdHead->Range;i++)
	{
		// ��������
		if(pRecData[i*4+1] == 9)
			wU = MAKEWORD(pRecData[3+i*4],pRecData[4+i*4]);
		else if(pRecData[i*4+1] == 10)
			wI = MAKEWORD(pRecData[3+i*4],pRecData[4+i*4]);
		else 
		// ֱ�ӷ�У����
		{
			ReportMsg("KF6500 Net protocol AutoAdjust data error!!!");
			OnAutoAdjustEcho(1,0);
			return;
		}
	}

	// ����У����Ϣ
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AUTOADJUST, RtosGetCurrentTaskId(), wU,wI,0 );

	ReportMsg("Net protocol send AutoAdjust. U =%d I=%d",wU,wI);
#endif
}

BOOL CSCanbusPro::RxdAbsTime()
{
	PBYTE       pRecData;
	TAbsTime 	AbsTime;	
	TSysTime 	SysTime;
	
	//��B��ʽʱ����Ч������£�����ͨ�Ź�Լ���ӣ���ͬʱ����Ӳ��ʱ��
	if(DAE_GetRunningFlags(SS_TIME_B))
	{
		pRecData = (PBYTE)&m_RxdHead->Range;
		
		AbsTime.Lo = MAKEDWORD(pRecData[0],pRecData[1],pRecData[2],pRecData[3]);
		AbsTime.Hi = MAKEWORD(pRecData[4],pRecData[5]);
		
		::WriteAbsTime(&AbsTime);
		AbsTimeTo(&AbsTime, &SysTime);
		WriteHardTime(&SysTime);
	}
	
	SetFlag(SF_TIMING,FLAG_OFF);
	
	return TRUE;
}

BOOL CSCanbusPro::TxdReadChannelAttrRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	
	TChannel_Disp	tChannel;
	TTxdHead 		*pTxdHead;
	PBYTE       	pDataField;
	WORD			Num,WritePtr;
	STATUS			rc = DB_ERR_OK;
	WORD 			i;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	Num = DB_GetChannelNum();
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READATTRIB);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CHANNEL;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	pDataField[0] = LOBYTE(0);
	pDataField[1] = HIBYTE(0);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);
	
	//-----------�������ݿ�ӿ�-----------------
	WritePtr=4;

	for(i=0;i<Num;i++)
	{
		rc= DB_GetChannelDisp(i, &tChannel);
		pDataField[WritePtr++] = LOBYTE(tChannel.wMeasureRange);
		pDataField[WritePtr++] = HIBYTE(tChannel.wMeasureRange);
		pDataField[WritePtr++] = LOBYTE(tChannel.wSampleClass);
		pDataField[WritePtr++] = HIBYTE(tChannel.wSampleClass);
	}
		
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);
#endif
	return TRUE;
}

BOOL CSCanbusPro::TxdWriteChannelAttrRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	TChannel_Disp	*pChannel;
	TTxdHead 		*pTxdHead;
	PBYTE			pRecData;
	PBYTE       	pDataField;
	STATUS			rc;
	WORD			no,Num;
	int				WritePtr,i; 		
		
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CHANNEL;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	pChannel = (TChannel_Disp *)m_pMemBlock;

	Num = DB_GetChannelNum();
	
	// ȫ������
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		for(i=0;i<DB_GetChannelNum();i++)
		{
			pChannel[i].wMeasureRange = MAKEWORD(pRecData[i*4+4],pRecData[i*4+5]);
			pChannel[i].wSampleClass = MAKEWORD(pRecData[i*4+6],pRecData[i*4+7]);
		}

		rc = DB_SetChannelDisp(DB_PARA_ALL, pChannel);
						
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;

		// ��䷵��ֵ
		memcpy(pDataField,pRecData,4+DB_GetChannelNum()*4);

		WritePtr = 4+DB_GetChannelNum()*4;
	}
	else //��ʱ��֧��һ����ֵ
	{
		no = MAKEWORD(pRecData[2],pRecData[3]);
			
		pChannel->wMeasureRange = MAKEWORD(pRecData[4],pRecData[5]);
		pChannel->wSampleClass = MAKEWORD(pRecData[6],pRecData[7]);

		// ��䷵��ֵ
		rc = DB_SetChannelDisp(no, pChannel);
				
		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = 0;
			memcpy(pDataField+2,pRecData+2,6);
			WritePtr = 8;
		}
	}
	
	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdReadChannelSrcRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	TTxdHead 		*pTxdHead;
	PBYTE       	pDataField;
	WORD			Num,WritePtr;
	STATUS			rc = DB_ERR_OK;
	WORD 			i;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Num = DB_GetChannelSrcNum();
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CHANNELSRC;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	pDataField[0] = LOBYTE(0);
	pDataField[1] = HIBYTE(0);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);
	
	//-----------�������ݿ�ӿ�-----------------
	WritePtr=4;

	for(i=0;i<Num;i++)
	{
		if(DB_IsChannelSrcValid(i))
		{
			pDataField[WritePtr++] = LLBYTE(FLAG_CHANNELSRC_VALID);
			pDataField[WritePtr++] = LHBYTE(FLAG_CHANNELSRC_VALID);
			pDataField[WritePtr++] = HLBYTE(FLAG_CHANNELSRC_VALID);
			pDataField[WritePtr++] = HHBYTE(FLAG_CHANNELSRC_VALID);
		}
		else
		{
			memset(pDataField+WritePtr,0,4);
			WritePtr += 4;
		}	
	}
		
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteChannelSrcRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	TTxdHead 		*pTxdHead;
	PBYTE			pRecData;
	PBYTE       	pDataField;
	STATUS			rc = DB_ERR_OK;
	WORD			no;
	int				WritePtr,i; 		
		
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CHANNELSRC;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
		
	// ȫ������
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		for(i=0;i<DB_GetChannelSrcNum();i++)
		{
			if(FLAG_CHANNELSRC_VALID == MAKEDWORD(pRecData[i*4+4],pRecData[i*4+5],pRecData[i*4+6],pRecData[i*4+7]))
				rc |= DB_EnableChannelSrc(i);
			else
				rc |= DB_DisenableChannelSrc(i);
		}

		// ����ʧ�ܼ�¼
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;
						
		// ��䷵��ֵ
		memcpy(pDataField,4+pRecData,DB_GetChannelSrcNum()*4);

		WritePtr = 4+DB_GetChannelSrcNum()*4;
	}
	else //��ʱ��֧��һ����ֵ
	{
		no = MAKEWORD(pRecData[2],pRecData[3]);
			
		if(FLAG_CHANNELSRC_VALID == MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]))
			rc = DB_EnableChannelSrc(no);
		else
			rc = DB_DisenableChannelSrc(no);

		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = 0;
			memcpy(pDataField+2,pRecData+2,6);
			WritePtr = 8;
		}
	}
	
	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdReadChannelValueRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	TTxdHead 		*pTxdHead;
	PBYTE       	pDataField;
	WORD			Num,WritePtr;
	STATUS			rc = DB_ERR_OK;
	WORD 			wValue,i;
	long 			lValue;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Num = DB_GetChannelNum();
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CHANNEL;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	pDataField[0] = LOBYTE(0);
	pDataField[1] = HIBYTE(0);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);
	
	//-----------�������ݿ�ӿ�-----------------
	WritePtr=4;

	for(i=0;i<Num;i++)
	{
		rc |= DB_GetZeroDriftValue(i, &lValue);
		pDataField[WritePtr++] = LLBYTE(lValue);
		pDataField[WritePtr++] = LHBYTE(lValue);
		pDataField[WritePtr++] = HLBYTE(lValue);
		pDataField[WritePtr++] = HHBYTE(lValue);
		
		rc |= DB_GetChannelFullValue(i, &wValue);
		pDataField[WritePtr++] = LOBYTE(wValue);
		pDataField[WritePtr++] = HIBYTE(wValue);
	}
	
	// ����ʧ�ܼ�¼
	if(rc != DB_ERR_OK)
		pTxdHead->Status |= STATUS_CTL_ERROR;

	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteChannelValueRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	long			*pZDvalue;
	WORD			*pFullvalue;
	TTxdHead 		*pTxdHead;
	PBYTE			pRecData;
	PBYTE       	pDataField;
	STATUS			rc = DB_ERR_OK;
	WORD			no;
	int				WritePtr,i; 		
		
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CHANNEL;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	pZDvalue = (long *)m_pMemBlock;
	pFullvalue = (WORD *)m_pMemBlock;
	
	// ȫ������
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		// ��������Ư
		for(i=0;i<DB_GetChannelNum();i++)
			pZDvalue[i] = MAKEDWORD(pRecData[i*6+4],pRecData[i*6+5],pRecData[i*6+6],pRecData[i*6+7]);

		rc = DB_SetZeroDriftValue(CHANNEL_VALUE_ALL,pZDvalue);

		// ��������ֵ
		for(i=0;i<DB_GetChannelNum();i++)
			pFullvalue[i] = MAKEWORD(pRecData[i*6+8],pRecData[i*6+9]);

		rc |= DB_SetChannelFullValue(CHANNEL_VALUE_ALL,pFullvalue);

		
		// ����ʧ�ܼ�¼
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;

		// ��䷵��ֵ
		memcpy(pDataField,pRecData,DB_GetChannelNum()*6);

		WritePtr = 4+DB_GetChannelNum()*6;
	}
	else //��ʱ��֧��һ����ֵ
	{
		no = MAKEWORD(pRecData[2],pRecData[3]);

		*pZDvalue = MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]);			
		rc = DB_SetZeroDriftValue(CHANNEL_VALUE_ALL,pZDvalue);

		*pFullvalue = MAKEWORD(pRecData[8],pRecData[9]);			
		rc = DB_SetChannelFullValue(CHANNEL_VALUE_ALL,pFullvalue);

		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = 0;
			memcpy(pDataField+2,pRecData+2,8);
			WritePtr = 10;
		}
	}
	
	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);
#endif

	return TRUE;
}

// ֱ�������������
BOOL CSCanbusPro::TxdWriteAOCoeRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	TTxdHead	*pTxdHead;
	PBYTE		pRecData;
	PBYTE		pDataField;
	WORD		No,Num;
	WORD		wValue;
	BYTE		byCoeType;
	STATUS		rc;
		
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
		
	pRecData = (PBYTE)&m_RxdHead->Range;
	
	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_COE;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	// ȡ������
	pRecData = (PBYTE)&m_RxdHead->Range;
	pRecData++;

	for(int i=0;i<m_RxdHead->Range;i++)
	{
		No = pRecData[1];					
		byCoeType = pRecData[0]-100;
		wValue = MAKEWORD(pRecData[2],pRecData[3]);
		
		if(No >= DB_GetAONum())
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			LogError("TxdWriteAOCoeRes",FILE_LINE,"AO no(%d) >= num(%d)",No,DB_GetAONum());
			break;
		}

		// ��д��
		rc = DB_WriteAOCoe(No,byCoeType,wValue);
		if(rc != DB_ERR_OK)		
			pTxdHead->Status |= STATUS_CTL_ERROR;

		pRecData += 4;
	};
	
	memcpy(pDataField,m_Bo.Info,9);
	
	m_TxdWritePtr += 9;

	TxdTail(0);
#endif
	
	return TRUE;
}


BOOL CSCanbusPro::RxdReadBIAttrReq()
{	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = DB_PARA_ALL;
		m_SET.Off = 0;
		
		SetFlag(SF_SENDRES_BIATTR);
	}

	return TRUE;
}

BOOL CSCanbusPro::TxdReadBIAttrRes()
{
	TBIPara		tBIAttr;
	TDIPara		tDIAttr;
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	Max = DB_GetBINum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_BIATTR;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_BIATTR) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READATTRIB);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_BI;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	if(m_SET.SETNo == DB_PARA_ALL)
	{		
		pDataField[0] = LOBYTE(Start);
		pDataField[1] = HIBYTE(Start);
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			DB_GetBIPara(Start+i, &tBIAttr);
			DB_GetDIPara(tBIAttr.wMaptoDINo, &tDIAttr);
			
			pDataField[WritePtr++] = LOBYTE(tBIAttr.wControl);
			pDataField[WritePtr++] = HIBYTE(tBIAttr.wControl);
			pDataField[WritePtr++] = LOBYTE(tDIAttr.wFilterTime);
			pDataField[WritePtr++] = HIBYTE(tDIAttr.wFilterTime);
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;

	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_BI;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = Num;
	
	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteBIAttrRes()
{
	TTxdHead 	*pTxdHead;
	TBIProPara	*pBI;
	TBIPara		*pBIAttr,tBIAttr;
	TDIPara		*pDIAttr,tDIAttr;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetBINum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_BI;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	WritePtr = 0;

	pBI = (TBIProPara *)m_pMemBlock;

	//������ֵ
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1��ȡ��ֵ����Ϣ
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		// ���ȼ��
		if(wFrameLength < (9+4+Num*4))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2����ż��
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// ����д���������ż��
		if(StartNo > Max)
			Num = 0;
		// ����˳��д��
		else if(StartNo != m_SET.Off)
			Num = 0;
		// Խ��ʱ
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3����˳���ж�
		for(i=0;i<Num;i++)
		{
			pBI[StartNo+i].wControl = MAKEWORD(pRecData[4+i*4],pRecData[5+i*4]);
			pBI[StartNo+i].wFilterTime = MAKEWORD(pRecData[6+i*4],pRecData[7+i*4]);
			
			m_SET.Off++;
		}

		// ������ֵд��
		if(m_RxdHead->APPControl&0x40)
		{
			// �������
			if(m_SET.Off != Max)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				Num = 0;
			}
			else
			{
				// ȡϵͳ�޸Ļ�����
				pBIAttr = (TBIPara	*)DB_GetParaModBuf(DB_PARA_BI);		
				pDIAttr = (TDIPara	*)DB_GetParaModBuf(DB_PARA_DI);

				do{
					rc = DB_GetBIPara(DB_PARA_ALL,pBIAttr); 		
					rc |= DB_GetDIPara(DB_PARA_ALL,pDIAttr); 	
					if(rc != DB_ERR_OK)
						break;
					
					// 1��������BI
					// ����޸�
					for(i=0;i<Max;i++)
						pBIAttr[i].wControl = pBI[i].wControl;
					
					rc |= DB_WriteBIPara(DB_PARA_ALL, pBIAttr);			
					if(rc != DB_ERR_OK)
						break;
					
					rc |= DB_WriteBIParaConfirm(DB_PARA_ALL);
					if(rc != DB_ERR_OK)
						break;
					
					// 2��������DI
					for(i=0;i<Max;i++)
					{
						// ����޸�
						if(pBIAttr[i].wMaptoDINo >= DB_GetDINum())
							continue;
						
						pDIAttr[pBIAttr[i].wMaptoDINo].wFilterTime = pBI[i].wFilterTime;
					}
						
					rc |= DB_WriteDIPara(DB_PARA_ALL, pDIAttr);
					if(rc != DB_ERR_OK)
						break;

					rc |= DB_WriteDIParaConfirm(DB_PARA_ALL);					
				}while(0);
				
				if(rc != DB_ERR_OK)
					pTxdHead->Status |= STATUS_CTL_ERROR;
				
			}
		}
		// ����̶�
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				
		// ��䷵��ֵ
		memcpy(pDataField+4,pRecData+4,Num*4);

		WritePtr = 4+Num*4;
	}
	else //��ʱ��֧��һ����ֵ	
	{
		Num = MAKEWORD(pRecData[0],pRecData[1]);
		// ���ȼ��
		if(wFrameLength < (9+2+Num*6))
			return FALSE;
		
		StartNo = MAKEWORD(pRecData[2],pRecData[3]);
		m_SET.SETNo = StartNo;
		
		// ��䷵��ֵ
		// ��ȡ���滻��д��		
		rc = DB_GetBIPara(StartNo, &tBIAttr);
		tBIAttr.wControl = MAKEWORD(pRecData[4+i*4],pRecData[5+i*4]);
		rc |= DB_WriteBIPara(StartNo, &tBIAttr);
		if(rc == DB_ERR_OK)
			rc |= DB_WriteBIParaConfirm(StartNo);
		
		rc |= DB_GetDIPara(tBIAttr.wMaptoDINo, &tDIAttr);		
		tDIAttr.wFilterTime = MAKEWORD(pRecData[6+i*4],pRecData[7+i*4]);
		rc |= DB_WriteDIPara(tBIAttr.wMaptoDINo, &tDIAttr);
		if(rc == DB_ERR_OK)
			rc |= DB_WriteDIParaConfirm(tBIAttr.wMaptoDINo);
		
		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = 0;
			memcpy(pDataField+2,pRecData+2,6);
			WritePtr = 8;
		}
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::RxdReadBOAttrReq()
{	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = DB_PARA_ALL;
		m_SET.Off = 0;
		
		SetFlag(SF_SENDRES_BOATTR);
	}

	return TRUE;
}

BOOL CSCanbusPro::TxdReadBOAttrRes()
{
	TBOPara		tBOAttr;
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	Max = DB_GetBONum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_BOATTR;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_BOATTR) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READATTRIB);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_BO;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	if(m_SET.SETNo == DB_PARA_ALL)
	{		
		pDataField[0] = LOBYTE(Start);
		pDataField[1] = HIBYTE(Start);
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			DB_GetBOPara(Start+i, &tBOAttr);
			
			pDataField[WritePtr++] = LOBYTE(tBOAttr.wLastTime);
			pDataField[WritePtr++] = HIBYTE(tBOAttr.wLastTime);
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;
	
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_BO;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = Num;

	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteBOAttrRes()
{
	TTxdHead 	*pTxdHead;
	TBOPara		tBOAttr;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetBONum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_BO;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	WritePtr = 0;

	//������ֵ
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1��ȡ��ֵ����Ϣ
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		// ���ȼ��
		if(wFrameLength < (9+4+Num*2))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2����ż��
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// ����д���������ż��
		if(StartNo > Max)
			Num = 0;
		// ����˳��д��
		else if(StartNo != m_SET.Off)
			Num = 0;
		// Խ��ʱ
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3����˳���ж�
		for(i=0;i<Num;i++)
		{
			// �ȶ�ȡ
			DB_GetBOPara(StartNo+i, &tBOAttr);

			// ���滻
			tBOAttr.wLastTime = MAKEWORD(pRecData[4+i*2],pRecData[5+i*2]);

			//��д��
			DB_WriteBOPara(StartNo+i, &tBOAttr);
			
			m_SET.Off++;
		}

		// ������ֵд��
		if(m_RxdHead->APPControl&0x40)
		{
			// �������
			if(m_SET.Off != Max)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				Num = 0;
			}
			else
			{
				if(DB_WriteBOParaConfirm(DB_PARA_ALL) != DB_ERR_OK)
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					Num = 0;
				}
			}
		}

		// ����̶�
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				
		// ��䷵��ֵ
		memcpy(pDataField+4,pRecData+4,Num*2);

		WritePtr = 4+Num*2;
	}
	else //��ʱ��֧��һ����ֵ
	{
		Num = MAKEWORD(pRecData[0],pRecData[1]);
		// ���ȼ��
		if(wFrameLength < (9+2+Num*6))
			return FALSE;
		
		StartNo = MAKEWORD(pRecData[2],pRecData[3]);
		m_SET.SETNo = StartNo;
		
		// ��䷵��ֵ
		// �ȶ�ȡ
		rc = DB_GetBOPara(StartNo+i, &tBOAttr);
		
		// ���滻
		tBOAttr.wLastTime= MAKEWORD(pRecData[4+i*2],pRecData[5+i*2]);
		
		//��д��
		rc |= DB_WriteBOPara(StartNo, &tBOAttr);
		
		if(rc == DB_ERR_OK)
			rc |= DB_WriteBOParaConfirm(StartNo);
		
		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = 0;
			memcpy(pDataField+2,pRecData+2,6);
			WritePtr = 8;
		}
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::RxdReadAOAttrReq()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = DB_PARA_ALL;
		m_SET.Off = 0;
		
		SetFlag(SF_SENDRES_AOATTR);
	}

	TxdReadAOAttrRes();
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdReadAOAttrRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	TAO_Disp	tAOAttr;
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	BYTE		byAppCtrl = 0x00;
	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	Max = DB_GetAONum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_AOATTR;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_AOATTR) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READATTRIB);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_AO;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16;
	
	if(m_SET.SETNo == DB_PARA_ALL)
	{		
		pDataField[0] = LOBYTE(Start);
		pDataField[1] = HIBYTE(Start);
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			DB_GetAOPara(Start+i, &tAOAttr);
			
			pDataField[WritePtr++] = LOBYTE(tAOAttr.wControl);
			pDataField[WritePtr++] = HIBYTE(tAOAttr.wControl);
			pDataField[WritePtr++] = LOBYTE(tAOAttr.wCHIndex);
			pDataField[WritePtr++] = HIBYTE(tAOAttr.wCHIndex);
			pDataField[WritePtr++] = LOBYTE(tAOAttr.wRange);
			pDataField[WritePtr++] = HIBYTE(tAOAttr.wRange);
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;

	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_AO;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = Num;
	
	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteAOAttrRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)

	TTxdHead 	*pTxdHead;
	TAO_Disp	tAOAttr;
	TDIPara		tDIAttr;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetAONum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_AO;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	WritePtr = 0;

	//������ֵ
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1��ȡ��ֵ����Ϣ
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		// ���ȼ��
		if(wFrameLength < (9+4+Num*2))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2����ż��
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// ����д���������ż��
		if(StartNo > Max)
			Num = 0;
		// ����˳��д��
		else if(StartNo != m_SET.Off)
			Num = 0;
		// Խ��ʱ
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3����˳���ж�
		for(i=0;i<Num;i++)
		{
			// �ȶ�ȡ
			DB_GetAOPara(StartNo+i, &tAOAttr);

			// ���滻
			tAOAttr.wControl = MAKEWORD(pRecData[4+i*6],pRecData[5+i*6]);
			tAOAttr.wCHIndex = MAKEWORD(pRecData[6+i*6],pRecData[7+i*6]);
			tAOAttr.wRange   = MAKEWORD(pRecData[8+i*6],pRecData[9+i*6]);

			//��д��
			DB_WriteAOPara(StartNo+i, &tAOAttr);
			
			m_SET.Off++;
		}

		// ������ֵд��
		if(m_RxdHead->APPControl&0x40)
		{
			// �������
			if(m_SET.Off != Max)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				Num = 0;
			}
			else
			{
				if(DB_WriteAOParaConfirm(DB_PARA_ALL) != DB_ERR_OK)
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					Num = 0;
				}
			}
		}

		// ����̶�
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				
		// ��䷵��ֵ
		memcpy(pDataField+4,pRecData+4,Num*6);

		WritePtr = 4+Num*6;
	}
	else //��ʱ��֧��һ����ֵ
	{
		Num = MAKEWORD(pRecData[0],pRecData[1]);
		// ���ȼ��
		if(wFrameLength < (9+2+Num*6))
			return FALSE;
		
		StartNo = MAKEWORD(pRecData[2],pRecData[3]);
		m_SET.SETNo = StartNo;
		
		// ��䷵��ֵ
		// �ȶ�ȡ
		rc = DB_GetAOPara(StartNo+i, &tAOAttr);
		
		// ���滻
		tAOAttr.wControl= MAKEWORD(pRecData[4],pRecData[5]);
		tAOAttr.wCHIndex= MAKEWORD(pRecData[6],pRecData[7]);
		tAOAttr.wRange  = MAKEWORD(pRecData[8],pRecData[9]);
		
		//��д��
		rc |= DB_WriteAOPara(StartNo, &tAOAttr);
		
		if(rc == DB_ERR_OK)
			rc |= DB_WriteAOParaConfirm(StartNo);
		
		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = 0;
			memcpy(pDataField+2,pRecData+2,8);
			WritePtr = 8;
		}
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdReadSysParaRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	DWORD		dwPara1,dwPara2,dwPara3;
	WORD		wPara1;

	TSerialConfig	tSerCfg;
	TCanbusConfig	tCanCfg;
	TEthLinkConfig	tEthCfg;
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_SYSCONFIG;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 	 			

	pDataField[0] = m_RxdHead->Range;

	switch(m_RxdHead->Range)
	{
		case DB_SYSPARA_ETHA:
		case DB_SYSPARA_ETHB:
			if(m_RxdHead->Range == DB_SYSPARA_ETHA)
				DB_GetEthAPara(dwPara1,dwPara2,dwPara3);
			else
				DB_GetEthBPara(dwPara1,dwPara2,dwPara3);
			
			pDataField[1] = LLBYTE(dwPara1);
			pDataField[2] = LHBYTE(dwPara1);
			pDataField[3] = HLBYTE(dwPara1);
			pDataField[4] = HHBYTE(dwPara1);
			pDataField[5] = LLBYTE(dwPara2);
			pDataField[6] = LHBYTE(dwPara2);
			pDataField[7] = HLBYTE(dwPara2);
			pDataField[8] = HHBYTE(dwPara2);
			pDataField[9] = LLBYTE(dwPara3);
			pDataField[10] = LHBYTE(dwPara3);
			pDataField[11] = HLBYTE(dwPara3);
			pDataField[12] = HHBYTE(dwPara3);
			m_TxdWritePtr += 12+1;
			break;
						
		case DB_SYSPARA_ADDR:
			wPara1 = DB_GetAddress();
			pDataField[1] = LLBYTE(wPara1);
			pDataField[2] = LHBYTE(wPara1);
			m_TxdWritePtr += 2+1;
			break;

		case DB_SYSPARA_PASSWORD:	
			DB_GetAccessPassWord((char *)pDataField+1,(char *)pDataField+9);
			m_TxdWritePtr += 16+1;
			break;

		case DB_SYSPARA_SERPARA:
			// ���ζ�ȡ����䴮�ڲ���
			pDataField[1] = DB_GetSerialNum();
			
			for(int i=0;i<DB_GetSerialNum();i++)
			{
				DB_GetSerialConfig(i,&tSerCfg);
				pDataField[2+i*4] = tSerCfg.byNo;
				pDataField[3+i*4] = tSerCfg.byMode;
				pDataField[4+i*4] = tSerCfg.byDriver;
				pDataField[5+i*4] = tSerCfg.byBaud;
			}
			m_TxdWritePtr += 4*DB_GetSerialNum()+2;
			break;

		case DB_SYSPARA_CANPARA:
			pDataField[1] = DB_GetCANNum();
			
			// ���ζ�ȡ�����CAN������
			for(int i=0;i<DB_GetCANNum();i++)
			{
				DB_GetCanbusConfig(i,&tCanCfg);
				pDataField[2+i*4] = tCanCfg.byNo;
				pDataField[3+i*4] = tCanCfg.byDriver;
				pDataField[4+i*4] = LOBYTE(tCanCfg.wBaud);
				pDataField[5+i*4] = HIBYTE(tCanCfg.wBaud);
			}
			
			m_TxdWritePtr += 4*DB_GetCANNum()+2;
			break;

		case DB_SYSPARA_ETHPARA:
			pDataField[1] = DB_GetEthLinkNum();
			
			// ���ζ�ȡ�������̫�����Ӳ���
			for(int i=0;i<DB_GetEthLinkNum();i++)
			{
				DB_GetEthLinkConfig(i,&tEthCfg);
				pDataField[2+i*10] = tEthCfg.byNetCardNo;
				pDataField[3+i*10] = tEthCfg.byLinkNo;
				pDataField[4+i*10] = tEthCfg.byDriver;
				pDataField[5+i*10] = tEthCfg.byMode;
				pDataField[6+i*10] = LLBYTE(tEthCfg.dwSideIP);
				pDataField[7+i*10] = LHBYTE(tEthCfg.dwSideIP);
				pDataField[8+i*10] = HLBYTE(tEthCfg.dwSideIP);
				pDataField[9+i*10] = HHBYTE(tEthCfg.dwSideIP);
				pDataField[10+i*10] = LOBYTE(tEthCfg.wPort);
				pDataField[11+i*10] = HIBYTE(tEthCfg.wPort);
			}
			
			m_TxdWritePtr += 10*DB_GetEthLinkNum()+2;
			break;

		case DB_SYSPARA_CTRLWORD:
			dwPara1 = DB_GetSystemControlWord();
			pDataField[1] = LLBYTE(dwPara1);
			pDataField[2] = LHBYTE(dwPara1);
			pDataField[3] = HLBYTE(dwPara1);
			pDataField[4] = HHBYTE(dwPara1);
						
			m_TxdWritePtr += 5;
			break;

		default:
			break;
	}
		
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteSysParaRes()
{
	TTxdHead 		*pTxdHead;
	PBYTE       	pDataField;
	PBYTE      	 	pRecData;
	DWORD			dwPara1,dwPara2,dwPara3;
	WORD			wPara1,wDataLen;
	STATUS			rc = DB_ERR_OK;
	TSerialConfig	*pSerCfg;
	TCanbusConfig	*pCanCfg;
	TEthLinkConfig	*pEthCfg;
		
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SYSCONFIG;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;

	switch(pRecData[0])
	{
		case DB_SYSPARA_ETHA:
		case DB_SYSPARA_ETHB:	
			dwPara1 = MAKEDWORD(pRecData[1],pRecData[2],pRecData[3],pRecData[4]);
			dwPara2 = MAKEDWORD(pRecData[5],pRecData[6],pRecData[7],pRecData[8]);
			dwPara3 = MAKEDWORD(pRecData[9],pRecData[10],pRecData[11],pRecData[12]);

			if(pRecData[0] == DB_SYSPARA_ETHA)
				rc = DB_ModifyEthAPara(dwPara1,dwPara2,dwPara3);
			else
				rc = DB_ModifyEthBPara(dwPara1,dwPara2,dwPara3);

			memcpy(pDataField,pRecData,12+1);
			m_TxdWritePtr += 1+12;

			break;
						
		case DB_SYSPARA_ADDR:
			wPara1 = MAKEWORD(pRecData[1],pRecData[2]);
			rc = DB_ModifyAddress(wPara1);

			memcpy(pDataField,pRecData,2+1);
			m_TxdWritePtr += 1+2;
			break;

		case DB_SYSPARA_PASSWORD:	
			rc = DB_SetAccessPassWord((char *)pRecData+1,(char *)pRecData+9);

			memcpy(pDataField,pRecData,2+16);
			m_TxdWritePtr += 1+16;
			break;

		case DB_SYSPARA_SERPARA:
			// �������ô��ڲ���
			if(pRecData[1] != DB_GetSerialNum())
				rc = DB_ERR_FAILURE;
			else
			{
				pSerCfg = (TSerialConfig	*)m_pMemBlock;
					
				for(int i=0;i<pRecData[1];i++)
				{
					pSerCfg[i].byNo = pRecData[2+i*4];
					pSerCfg[i].byMode= pRecData[3+i*4];
					pSerCfg[i].byDriver = pRecData[4+i*4];
					pSerCfg[i].byBaud = pRecData[5+i*4];
				}
				
				rc = DB_SetSerialConfig(DB_PARA_ALL,pSerCfg);
			}

			wDataLen = 4*pRecData[1]+2;

			memcpy(pDataField,pRecData,wDataLen);
			m_TxdWritePtr += wDataLen;
			break;

		case DB_SYSPARA_CANPARA:
			// ��������CAN������
			if(pRecData[1] != DB_GetCANNum())
				rc = DB_ERR_FAILURE;
			else
			{
				pCanCfg = (TCanbusConfig	*)m_pMemBlock;
				
				for(int i=0;i<pRecData[1];i++)
				{
					pCanCfg[i].byNo = pRecData[2+i*4];
					pCanCfg[i].byDriver = pRecData[3+i*4];
					pCanCfg[i].wBaud= MAKEWORD(pRecData[4+i*4],pRecData[5+i*4]);
					
				}
				rc = DB_SetCanbusConfig(DB_PARA_ALL,pCanCfg);
			}
			
			wDataLen = 4*pRecData[1]+2;
			
			memcpy(pDataField,pRecData,wDataLen);
			m_TxdWritePtr += wDataLen;
			break;

		case DB_SYSPARA_ETHPARA:
			// ����������̫�����Ӳ���
			if(pRecData[1] != DB_GetEthLinkNum())
				rc = DB_ERR_FAILURE;
			else
			{
				pEthCfg = (TEthLinkConfig	*)m_pMemBlock;
				
				for(int i=0;i<pRecData[1];i++)
				{
					pEthCfg[i].byNetCardNo = pRecData[2+i*10];
					pEthCfg[i].byLinkNo = pRecData[3+i*10];
					pEthCfg[i].byDriver = pRecData[4+i*10];
					pEthCfg[i].byMode = pRecData[5+i*10];
					pEthCfg[i].dwSideIP = MAKEDWORD(pRecData[6+i*10],pRecData[7+i*10],pRecData[8+i*10],pRecData[9+i*10]);
					pEthCfg[i].wPort = MAKEWORD(pRecData[10+i*10],pRecData[11+i*10]);
					
				}
				rc = DB_SetEthLinkConfig(DB_PARA_ALL,pEthCfg);
			}
			
			wDataLen = 10*pRecData[1]+2;
			
			memcpy(pDataField,pRecData,wDataLen);
			m_TxdWritePtr += wDataLen;
			break;

		case DB_SYSPARA_CTRLWORD:
			dwPara1 = MAKEDWORD(pRecData[1],pRecData[2],pRecData[3],pRecData[4]);
			DB_SetSystemControlWord(dwPara1);
			memcpy(pDataField,pRecData,5);
			m_TxdWritePtr += 5;
			break;

		default:
			break;
	}

	if(rc != DB_ERR_OK)
		pTxdHead->Status |= STATUS_CTL_ERROR;

	TxdTail(0);
	
	return TRUE;
}

BOOL CSCanbusPro::TxdReadVersionRes()
{
	TTxdHead 		*pTxdHead;
	PBYTE       	pDataField;
	TCodeFileInf 	tFileInf;
	TAbsTime 		AbsTime;
	DWORD			dwCheckLen;
			
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pTxdHead->ObjType 	= DOC_VERSION;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 	 			

	pDataField = (PBYTE)&pTxdHead->Range;
	memset(pDataField,0,32);

	if(DB_GetCodeFileVerInf(&tFileInf, &dwCheckLen) != ERR_SUCCESS)
	{
		memset(&tFileInf,0,sizeof(TCodeFileInf));
		tFileInf.CompileTime.Year = 2011;
		tFileInf.CompileTime.Month= 1;
		tFileInf.CompileTime.Day = 1;
		tFileInf.CompileTime.Hour = 0;
		tFileInf.CompileTime.Minute= 0;

		memcpy(&tFileInf.EditTime,&tFileInf.CompileTime,sizeof(TSysTime));
//		strcpy(tFileInf.HardWareVer,"1.0.0.0");
//		strcpy(tFileInf.SoftWareVer,"1.0.0.0");
		strcpy(tFileInf.UserInf,"KF1300");

		tFileInf.wCheckSum = DAE_CheckSum((BYTE *)&tFileInf,sizeof(TCodeFileInf));
	}

	// ����汾��
	//for(int i=0; i<4; i++)
	//	tFileInf.SoftWareVer[i] = MAKEWORD(HIBYTE(tFileInf.SoftWareVer[i]), \
	//								LOBYTE(tFileInf.SoftWareVer[i]) );
	memcpy((char *)pDataField,tFileInf.SoftWareVer,VER_INF_LEN);//����汾��Ϣ���ַ���
	pDataField +=16;
	
	// ����У����
	*pDataField++ = LOBYTE(tFileInf.wCheckSum);
	*pDataField++ = HIBYTE(tFileInf.wCheckSum);

	dwCheckLen = GetPLCDBVer();
	*pDataField++ = LLBYTE(dwCheckLen);
	*pDataField++ = LHBYTE(dwCheckLen);
	*pDataField++ = HLBYTE(dwCheckLen);
	*pDataField++ = HHBYTE(dwCheckLen);
	
	// �߼�У����
	dwCheckLen = GetPLCCheckSum();
	*pDataField++ = LOBYTE(dwCheckLen);
	*pDataField++ = HIBYTE(dwCheckLen);
	
	//���ݿ�汾��Ϣ���汾�ţ��ļ�У����
	DB_GetDataBaseVersion(pDataField);
	pDataField += 8;

	//MAC��ַ��Ϣ����������
	BYTE MacInf[6];
	for(int i=0; i<2; i++)
	{
		DB_GetNetMac(i, MacInf);
		for(int j=0; j<6; j++)
		{
			*pDataField++ = MacInf[j];
		}
	}
	
	m_TxdWritePtr += 44;
		
	TxdTail(0);	

	return TRUE;
}

BOOL CSCanbusPro::TxdReadDevStatusRes()
{
	TTxdHead	*pTxdHead;
	PBYTE		pDataField;
			
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pTxdHead->ObjType	= DOC_DEVSTATUS;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;				
	
	pDataField = (PBYTE)&pTxdHead->Range;
	memset(pDataField,0,32);
	
	// �¶�
	pDataField[0] = LOBYTE(SysTempture);
	pDataField[1] = HIBYTE(SysTempture);
	
	// �������״̬
	Relay_GetBOINStatus(pDataField+2);
	Relay_GetBOBRStatus(pDataField+10);

	// ����λ����ͨ��״̬4
	DAE_GetMBOCommStatus(pDataField+18);
	
	// �˿�ͨ��״̬
	for(int i=0;i<16;i++)
	{
		if(DAE_GetRunningFlags(SS_PORT_COMM_ERR+i))
			pDataField[26+(i/8)] |= (0x01<<(i%8));
	}
	
	// ������״̬
	if(DAE_GetRunningFlags(SS_TIME_B))
		pDataField[30] = 0x80|(IrigQuality&0x0F);
	else
		pDataField[30] = (IrigQuality&0x0F);

	//  ȡ���״̬
#if(SYSTEM_TYPE == _SYS_KF6500_)
	for(int i=0;i<DB_GetFLObjNum();i++)
	{
		if(DAE_GetRunningFlags(SS_FL1_COMM_ERR+i))
			pDataField[31] |= (0x01<<i);
	}
#endif		

	m_TxdWritePtr += 32;
	
	TxdTail(0); 
	
	return TRUE;
}

BOOL CSCanbusPro::TxdReadCFGRes()
{
	TTxdHead 	*pTxdHead;
	DWORD 		*pCfgWord;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD		Num,nZeroNo;
	WORD		WritePtr;
	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;

	Num = (DB_GetCFGNum()+31)/32;
	
	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CFG;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	
	WritePtr=0;
	pCfgWord = (DWORD *)m_pMemBlock;

	// ȡ��ֵ����
	nZeroNo = MAKEWORD(pRecData[0],pRecData[1]);

	rc = DB_GetCFGWord(nZeroNo,pCfgWord);
	
	if(rc != DB_ERR_OK)
		pTxdHead->Status |= STATUS_CTL_ERROR;
	else
	{
		// ����
		pDataField[WritePtr++] = pRecData[0];
		pDataField[WritePtr++] = pRecData[1];
		
		//-----------�������ݿ�ӿ�-----------------
		for(int i=0;i<Num;i++)
		{
			pDataField[WritePtr++] = LLBYTE(pCfgWord[i]);
			pDataField[WritePtr++] = LHBYTE(pCfgWord[i]);
			pDataField[WritePtr++] = HLBYTE(pCfgWord[i]);
			pDataField[WritePtr++] = HHBYTE(pCfgWord[i]);
		}
	}
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteCFGRes()
{
	TTxdHead 	*pTxdHead;
	DWORD 		*pCfgWord;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,Num,nZeroNo;
	WORD		wFrameLength,WritePtr;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_SET)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CFG;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	WritePtr = 0;
	Num = (DB_GetCFGNum()+31)/32;	
	nZeroNo = MAKEWORD(pRecData[0],pRecData[1]);
	pCfgWord = (DWORD *)m_pMemBlock;
	
	//������ֵ
//	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_NORANGE)
	{
		// 1��ȡ��ֵ����Ϣ
		// ���ȼ��
		if(wFrameLength < (9+Num*sizeof(DWORD)))
			return FALSE;

		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];

		for(i=0;i<Num;i++)
			pCfgWord[i] = MAKEDWORD(pRecData[2+i*4],pRecData[3+i*4],pRecData[4+i*4],pRecData[5+i*4]);

		rc = DB_SetCFGWord(nZeroNo, (DWORD *)pCfgWord);
		
		if((rc == DB_ERR_OK)&&(m_RxdHead->FunCode != FUN_SELECT))
			rc |= DB_SetCFGWordConfirm(nZeroNo,DB_EXECUTE);
		
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;
		
		memcpy(pDataField,pRecData,2+Num*sizeof(DWORD));
		WritePtr = 2+Num*sizeof(DWORD);
	}
	
	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}


// ����ָ��
BOOL CSCanbusPro::TxdRelayControlRes(BOOL bBlock)
{
	TTxdHead	*pTxdHead;
	PBYTE		pRecData;
	PBYTE		pDataField;
	WORD		no,time;
	
	if((m_NetCfg.dwStatus&MASK_BO)==0)
		return FALSE;
	pRecData = (PBYTE)&m_RxdHead->Range;
			
	TxdHead(0xC0+m_CommandSeq,FUN_COMMONCONTROL); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	if(!bBlock)
		pTxdHead->ObjType	= DOC_RELAYCTRL;
	else
		pTxdHead->ObjType	= DOC_RELAYCTRLWITHBLOCK;
		
	pTxdHead->ObjQulifier	= (INDEX_16BIT<<4)+QUALI_NUM_8;

	no = MAKEWORD(pRecData[1],pRecData[2]);
	time = MAKEWORD(pRecData[3],pRecData[4]);

#if(SYSTEM_TYPE == _SYS_KF1300_)
	if(!bBlock)
		DB_BOTest(no, time);
	else
	{
		if(Relay_GetCtrlBlockStatus() == CTRL_BLOCK_LOCAL)
			DB_BOTestWithBlock(no, time);
		else
			//Զ��ʱ����
			pTxdHead->Status |= STATUS_CTL_BLOCK;
	}
#else
	DB_BOTest(no, time);
#endif

	memcpy(pDataField,pRecData,5);
		
	m_TxdWritePtr = 5+13;
		
	TxdTail(0);
		
	return TRUE;
}

BOOL CSCanbusPro::TxdSpecialOperateRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE		pRecData;
	PBYTE       pDataField;
	STATUS		rc;
	int			i;
	WORD		nIndex;
	
	if((m_NetCfg.dwStatus&MASK_BO)==0)
		return FALSE;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_SPECIALCMD;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	pRecData = (PBYTE)&m_RxdHead->Range;
					
	switch(pRecData[0])
	{
		case 1:// ģ�ⱨ��ָ��		
			memcpy(pDataField,pRecData,4);
			m_TxdWritePtr += 4;
			
			nIndex = MAKEWORD(pRecData[2],pRecData[3]);
#if(SYSTEM_TYPE == _SYS_KF1300_)
			if(nIndex == 0xFFFF)
			{
				ReportTest();
				break;
			}
#endif

			if(nIndex >= DB_GetRENum())
			{
				ReportMsg("Make relay report: report index is overflow!");
				break;
			}

			if(pRecData[1] == 0)
			{
				SimActionReport(nIndex);
				ReportMsg("simulate action report. Index=%d",nIndex);
			}
			else
			{
				Report_Alarm(nIndex);
				ReportMsg("simulate alarm report. Index=%d",nIndex);
			}
			
			break;
			
		case 2:// ����¼��ָ��
#if(SYSTEM_TYPE == _SYS_KF1300_)
			Relay_StartDisturbRecord();
#else
			// Modify by lz. 2013.11.06
			TAbsTime abstime;
			ReadAbsTime(&abstime);
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DBASE), SM_STARTDIST, dwRelayDataPtr,*AS_pnRECD*16/10,abstime.Lo,abstime.Hi);
#endif
			memcpy(pDataField,pRecData,1);
			m_TxdWritePtr += 1;
			break;

		case 3:// ��ֵ����

			do{
				if((pRecData[1] >= DB_GetSETAreaNum())||\
					(pRecData[2] >= DB_GetSETAreaNum())||\
					(pRecData[3] >= DB_GetSETAreaNum()))
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					break;
				}
				
				// ��ȡԴ��ֵ��
				rc = DB_GetSETFloatValue(pRecData[1], SET_VALUE_ALL, (float *)m_pMemBlock);
				if(rc != DB_ERR_OK)
					break;
				
				// ���д��Ŀ�Ķ�ֵ��
				for(i=pRecData[2];i<=pRecData[3];i++)
				{
					rc |= DB_SetSETValue(i, SET_VALUE_ALL, (float *)m_pMemBlock);
					if(rc != DB_ERR_OK)
						break;
					
					rc |= DB_SetSETValueConfirm(i, SET_VALUE_ALL);
					if(rc != DB_ERR_OK)
						break;

					Sleep(100);
				}
			}while(0);
			
			if(rc != DB_ERR_OK)
				pTxdHead->Status |= STATUS_CTL_ERROR;

			memcpy(pDataField,pRecData,4);
			m_TxdWritePtr += 4;
			break;

		case 4://���ܰ���
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_RELAY), SM_KEYDOWN, RtosGetCurrentTaskId(), pRecData[1],0,0 );			
			memcpy(pDataField,pRecData,1);
			m_TxdWritePtr += 1;
			break;
			
		case 5:
			nIndex = MAKEWORD(pRecData[1],pRecData[2]);
			DB_ClearEvent(nIndex);
			memcpy(pDataField,pRecData,3);
			m_TxdWritePtr += 3;
			break;
			
		default:
			break;
	}
		
	TxdTail(0);	
	
	return TRUE;
}

// ֱ�������������
BOOL CSCanbusPro::TxdSelectAOCoeRes()
{
	TTxdHead	*pTxdHead;
	PBYTE		pDataField;
    BYTE        Type,ChanNo;
	WORD        AdjustValue;
		
	if((m_NetCfg.dwStatus&MASK_BO)==0)
		return FALSE;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_SELECTWITHVALUE);
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;

	pTxdHead->ObjType	= DOC_COE;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE;

	memcpy(pDataField,m_Bo.Info,4);

	m_TxdWritePtr += 4;
		
	TxdTail(0); 

#if(SYSTEM_TYPE == _SYS_KF1300_)

	ChanNo = m_Bo.Info[1];   
	AdjustValue = MAKEWORD(m_Bo.Info[2],m_Bo.Info[3]);  

	switch(m_Bo.Info[0])
	{
		case 100:
		case 120:
			Type = 0;  //0~5V
			break;
		case 101:
		case 121:
			Type = 1;   //0~10V
			break;
		case 102:
		case 122:
			Type = 2;  //-5~5V
			break;
		case 103:
		case 123:
			Type = 3;   //-10~10V
			break;
		case 104:
		case 124:
			Type = 5;   //4~20mA
			break;
		case 105:
		case 125:
			Type = 6;	//0~20mA
			break;
		case 106:
		case 126:
			Type = 7;	//0~24mA
			break;
		default:
			break;
	}
	
    //������Ϣ��AI,ֹͣ��ģ�����������ת������
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AOADJUST, 0, 0,0,0 );			

	//����У�����������Լ������������д�뵽˫��RAM��
	WriteDAChanData(ChanNo, AdjustValue); 
	WriteDAChanType(ChanNo, Type);

    ReportMsg("chan%d,AO=%d",ChanNo,AdjustValue);
	
#endif

	return TRUE;
}

BOOL CSCanbusPro::TxdReadAOCoeRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
		
	TTxdHead	*pTxdHead;
	TAO_Disp 	tAOAttr;
	PBYTE		pRecData;
	PBYTE		pDataField;
	WORD		No,Num;
	WORD		wValue;
	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;
			
	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE);
		
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_COE;
	pTxdHead->ObjQulifier = (INDEX_8BIT<<4)+QUALI_NUM_8;

	Num = DB_GetAONum();
	
	*pDataField++ = Num*AO_USE_RANGENUM*3;
	
	// �����ȡ
	for(int i=0;i<Num;i++)
	{
		// ����Ư
		for(int j=0;j<AO_USE_RANGENUM;j++)
		{
			rc |= DB_ReadAOCoe(i, j,&wValue);
			if(rc != DB_ERR_OK)
			{
				ReportMsg("SCANBUS read AO coef error. CHAN=%d TYPE=%d!!!",i,100+j);
			}
			*pDataField++ = 100+j;
			*pDataField++ = i;
			*pDataField++ = LOBYTE(wValue);
			*pDataField++ = HIBYTE(wValue);
		}

		// ����ֵ
		for(int j=0;j<AO_USE_RANGENUM;j++)
		{
			rc |= DB_ReadAOCoe(i, 20+j,&wValue);
			if(rc != DB_ERR_OK)
			{
				ReportMsg("SCANBUS read AO coef error. CHAN=%d TYPE=%d!!!",i,120+j);
			}
			*pDataField++ = 100+20+j;
			*pDataField++ = i;
			*pDataField++ = LOBYTE(wValue);
			*pDataField++ = HIBYTE(wValue);
		}

		// ��Ŀ��ֵ
		for(int j=0;j<AO_USE_RANGENUM;j++)
		{
			rc |= DB_ReadAOCoe(i, 40+j,&wValue);
			if(rc != DB_ERR_OK)
			{
				ReportMsg("SCANBUS read AO coef error. CHAN=%d TYPE=%d!!!",i,120+j);
			}
			*pDataField++ = 100+40+j;
			*pDataField++ = i;
			*pDataField++ = LOBYTE(wValue);
			*pDataField++ = HIBYTE(wValue);
		}

	}

	if(rc != DB_ERR_OK) 	
		pTxdHead->Status |= STATUS_CTL_ERROR;
				
	m_TxdWritePtr += 1+Num*AO_USE_RANGENUM*3*4;
	
	TxdTail(0);

#endif

	return TRUE;
}

BOOL  CSCanbusPro::TxdLEDStatus(BOOL bBurst)
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	TTxdHead 	*pTxdHead;
	PBYTE		pDataField;
	WORD		i;
	BYTE		bStatus,bFreq;
		
	if((m_NetCfg.dwStatus&MASK_LED)==0)
		return FALSE;

	if(bBurst)
		TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE);	
	else
		TxdHead(0xC0+m_CurrentResSeq,FUN_REPORTVALUE);	
		
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;	
	pTxdHead->ObjType 	= DOC_LEDSTATUS;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	
	//-----------�������ݿ�ӿ�-----------------
	pDataField[0] = (LED_RELAY_WORD_NUM+1)/2;

	for(i=0;i<LED_RELAY_WORD_NUM;i++)
	{
	//-----------�������ݿ�ӿ�-----------------
		GetFlashLedValue(i,&bFreq,&bStatus);

		// ˢ��״ֵ̬
		m_pLEDStatus[i] = bStatus;
		m_pLEDFreq[i] = bFreq;
		
		// ��ɫ
		if(i%2)
			pDataField[1+i/2] += ((bFreq<<1)+(bStatus&0x01))<<4;
		// ��ɫ
		else
			pDataField[1+i/2] = ((bFreq<<1)+(bStatus&0x01))&0x0F;
	}
	
	m_TxdWritePtr += 1+pDataField[0];

	TxdTail(0);

	if(bBurst)
	{
		//����ȷ����Ϣ
		m_ResInfo.ResFunCode = pTxdHead->FunCode;
		m_ResInfo.ObjType = DOC_LEDSTATUS;
		m_ResInfo.ResSeq  = m_CurrentResSeq;
		m_ResInfo.ObjNum = pDataField[0];
		BackRetryFrame();	
	}
	
	IncResReq();
#endif		
	return TRUE;
}

BOOL  CSCanbusPro::TxdLEDStatusRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	TTxdHead 	*pTxdHead;
	PBYTE		pDataField;
	WORD		i;
	BYTE		bStatus,bFreq;
		
	if((m_NetCfg.dwStatus&MASK_LED)==0)
		return FALSE;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_REPORTVALUE);
		
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;	
	pTxdHead->ObjType 	= DOC_LEDSTATUS;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	
	//-----------�������ݿ�ӿ�-----------------
	pDataField[0] = (LED_RELAY_WORD_NUM+1)/2;

	for(i=0;i<LED_RELAY_WORD_NUM;i++)
	{
	//-----------�������ݿ�ӿ�-----------------
		GetFlashLedValue(i,&bFreq,&bStatus);

		// ˢ��״ֵ̬
		m_pLEDStatus[i] = bStatus;
		m_pLEDFreq[i] = bFreq;
		
		// ��ɫ
		if(i%2)
			pDataField[1+i/2] += ((bFreq<<1)+(bStatus&0x01))<<4;
		// ��ɫ
		else
			pDataField[1+i/2] = ((bFreq<<1)+(bStatus&0x01))&0x0F;
	}
	
	m_TxdWritePtr += 1+pDataField[0];

	TxdTail(0);
#endif		
	return TRUE;
}

// �鿴�˿ڱ���
BOOL CSCanbusPro::TxdWatchPortRes()
{
	TTxdHead 		*pTxdHead;
 	PBYTE         	pDataField,pRecData;
	WORD			wPortID,wLen;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_WATCHPORT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 

	// ���Ӷ˿�ID���Ź�,�趨30��
	m_nWatchPortDog = 30*(1000/CAN_TIMER_TICK);
	
	wPortID = MAKEWORD(pRecData[0],pRecData[1]);
	wLen=MAKEWORD(pRecData[2],pRecData[3]);

	// ��ʱ��ض˿ںű仯
	if (m_wWatchPortID != wPortID)
	{
		ReportMsg("��ʼ�鿴�˿�:0x%x",wPortID);
		SetDebugComm(wPortID);
		m_wWatchPortID = wPortID;
	}

	// ��ȡ��������
	wLen = ReadPrompt((char *)(pDataField+4),wLen);

	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = LOBYTE(wLen);
	pDataField[3] = HIBYTE(wLen);

	m_TxdWritePtr += wLen+4;

	TxdTail(0);	
			
	return TRUE;
}

// �鿴��������
BOOL CSCanbusPro::TxdVarDataRes()
{
	TTxdHead 		*pTxdHead;
 	PBYTE         	pDataField,pRecData;
	DWORD			dwVarID,dwStartPos,dwMax,dwLen;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_VARIAL;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 

	dwVarID = MAKEDWORD(pRecData[0],pRecData[1],pRecData[2],pRecData[3]);

	// ��ȡ�����ռ��С
	dwMax = DAE_GetMemVarSize(dwVarID);
	dwStartPos = MAKEDWORD(pRecData[8],pRecData[9],pRecData[10],pRecData[11]);
	dwLen=MAKEWORD(pRecData[12],pRecData[13]);

	// ��ȡ��������
	dwLen = DAE_ReadMemVar(dwVarID, dwStartPos, dwLen, pDataField+14);

	memcpy(pDataField,pRecData,14);
	
	pDataField[4] = LLBYTE(dwMax);
	pDataField[5] = LHBYTE(dwMax);
	pDataField[6] = HLBYTE(dwMax);
	pDataField[7] = HHBYTE(dwMax);
	pDataField[12] = LOBYTE(dwLen);
	pDataField[13] = HIBYTE(dwLen);

	m_TxdWritePtr += dwLen+14;
	
	TxdTail(0);	
			
	return TRUE;
}

BOOL CSCanbusPro::TxdSRMRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField,pRecData;
	WORD		i,Start,Num,Max;
	WORD 		WritePtr;
	long		lValue;
	
	Max = DB_GetRMNum();

	pRecData = (PBYTE)&m_RxdHead->Range;
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_SRM;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_16; 
	
	Start = MAKEWORD(pRecData[0],pRecData[1]);
	Num = MAKEWORD(pRecData[2],pRecData[3]);
	
	switch(m_RxdHead->ObjQulifier)
	{
		case 0x07:
			// ��ʼ
			if(Start > Max) Start = Max;
			if(Start+Num > Max) Num = Max-Start;
			if(Num > 200) Num = 200;
				
			break;
			
		default:
			Start = 0;
			Num = Max;	
			break;
	}	

	pDataField[0] = LOBYTE(Start);
	pDataField[1] = HIBYTE(Start);
	pDataField[2] = LOBYTE(Num);
	pDataField[3] = HIBYTE(Num);
	
	//-----------�������ݿ�ӿ�-----------------
	WritePtr=4;
	
	for(i=0;i<Num;i++)
	{
	   DB_ReadSecondRM(Start+i,&lValue);  

	   pDataField[WritePtr++] = LLBYTE(lValue);
	   pDataField[WritePtr++] = LHBYTE(lValue);
	   pDataField[WritePtr++] = HLBYTE(lValue);
	   pDataField[WritePtr++] = HHBYTE(lValue);
	}	
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);	
			
	return TRUE;
}

// �豸������Ϣ
BOOL CSCanbusPro::TxdReadDevDispRes()
{
	TTxdHead 		*pTxdHead;
	PBYTE       	pDataField;
			
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pTxdHead->ObjType 	= DOC_DEVDISP;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 	 			

	pDataField = (PBYTE)&pTxdHead->Range;
	memset(pDataField,0,54);

	//	char		DevType[16];			//�豸����
	strcpy((char *)pDataField,PRO_TYPE);
	//	WORD		nAI;					//ң�����				
	pDataField[16] = LOBYTE(DB_GetAINum());
	pDataField[17] = HIBYTE(DB_GetAINum());
	//	WORD		nBI;					//ң�Ÿ���				
	pDataField[18] = LOBYTE(DB_GetBINum());
	pDataField[19] = HIBYTE(DB_GetBINum());
	//	WORD		nCI;					//��ȸ���				
	pDataField[20] = LOBYTE(DB_GetCINum());
	pDataField[21] = HIBYTE(DB_GetCINum());
	//	WORD		nBO;					//ң�ظ���				
	pDataField[22] = LOBYTE(DB_GetBONum());
	pDataField[23] = HIBYTE(DB_GetBONum());
	//	WORD		nSerNum;				//���ڸ���				
	pDataField[24] = LOBYTE(DB_GetSerialNum());
	pDataField[25] = HIBYTE(DB_GetSerialNum());
	//	WORD		nCanNum;				//CAN���˿ڸ��� 		
	pDataField[26] = LOBYTE(DB_GetCANNum());
	pDataField[27] = HIBYTE(DB_GetCANNum());
	//	WORD		nEthNum;				//��̫���˿ڸ���			
	pDataField[28] = LOBYTE(DB_GetEthLinkNum());
	pDataField[29] = HIBYTE(DB_GetEthLinkNum());
	//	WORD		nSW;					//��ѹ�����		
	pDataField[30] = LOBYTE(DB_GetSWNum());
	pDataField[31] = HIBYTE(DB_GetSWNum());
	//	WORD		nCFG;					//�����ָ���		
	pDataField[32] = LOBYTE(DB_GetCFGNum());
	pDataField[33] = HIBYTE(DB_GetCFGNum());
	//	WORD		nSET;					//��ֵ����		
	pDataField[34] = LOBYTE(DB_GetSETNum());
	pDataField[35] = HIBYTE(DB_GetSETNum());
	//	WORD		nSETArea;				//��ֵ������		
	pDataField[36] = LOBYTE(DB_GetSETAreaNum());
	pDataField[37] = HIBYTE(DB_GetSETAreaNum());
	//	WORD		nSETAreaCur;			//��ǰ��ֵ����		
	pDataField[38] = LOBYTE(DB_GetCurrentSETAreaNo());
	pDataField[39] = HIBYTE(DB_GetCurrentSETAreaNo());
	//	WORD		nSystemSET; 			//������ֵ����	
	pDataField[40] = LOBYTE(DB_GetSystemSETNum());
	pDataField[41] = HIBYTE(DB_GetSystemSETNum());
	//	WORD		nCF;					//ϵ������		
	pDataField[42] = LOBYTE(DB_GetCFNum());
	pDataField[43] = HIBYTE(DB_GetCFNum());
	//	WORD		nRM;					//��������ֵ����	
	pDataField[44] = LOBYTE(DB_GetRMNum());
	pDataField[45] = HIBYTE(DB_GetRMNum());
	//	WORD		nRE;					//������������	
	pDataField[46] = LOBYTE(DB_GetRENum());
	pDataField[47] = HIBYTE(DB_GetRENum());
	//	WORD		nTC;					//��������
	pDataField[48] = LOBYTE(DB_GetTCNum());
	pDataField[49] = HIBYTE(DB_GetTCNum());
	//	WORD		nLanguageNo;			//���Ա��
	pDataField[50] = LOBYTE(DB_GetLanguageNo());
	pDataField[51] = HIBYTE(DB_GetLanguageNo());
	//	WORD		nMBO;					//����λ�������
	pDataField[52] = LOBYTE(DB_GetMapBitObjNum());
	pDataField[53] = HIBYTE(DB_GetMapBitObjNum());
	
	
	m_TxdWritePtr += 54;
		
	TxdTail(0);	

	return TRUE;
}

BOOL CSCanbusPro::TxdReadRWStatusRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	Num = pRecData[0];
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DOC_RW;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	*pDataField++ = Num;

	for(i=0;i<Num;i++)
	//-----------�������ݿ�ӿ�-----------------
	{
		no = MAKEWORD(pRecData[1+i*2],pRecData[2+i*2]);
		// ��ȡ�̵�����״̬		
		*pDataField++ = ReadRWFullValue(no);

//		ReportMsg("Protocol read RW no=%d value=%d",no,ReadRWFullValue(no));
	}
				
	m_TxdWritePtr += 1+Num;
	
	TxdTail(0); 
						
	return TRUE;
}

BOOL CSCanbusPro::RxdWriteRWStatus()
{
	PBYTE       pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	Num = pRecData[0];
		
	for(i=0;i<Num;i++)
	//-----------�������ݿ�ӿ�-----------------
	{
		no = MAKEWORD(pRecData[1+i*3],pRecData[2*i*3]);
		// д�̵�����״̬
		WriteDebugRW(no, pRecData[3+i*3]);
	}
										
	return TRUE;
}

BOOL CSCanbusPro::RxdReadMBOReq()
{	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	if(m_RxdHead->ObjQulifier == ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		m_SET.SETNo = DB_PARA_ALL;
		m_SET.Off = 0;
		
		SetFlag(SF_SENDRES_MBOPARA);
	}

	return TRUE;
}

BOOL CSCanbusPro::TxdReadMBORes()
{
	TTxdHead 	*pTxdHead;
	TMBO_Disp	tMBO;
	PBYTE       pDataField;
	WORD 		Start;
	WORD		Num,Max;
	WORD		WritePtr;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	Max = DB_GetMapBitObjNum();
	Start = m_SET.Off;	
	Num = PERFRM_NUM_MBO;

	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// �������һ֡
	if((Start+PERFRM_NUM_MBO) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;
	}
		
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READATTRIB);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_MAPBITOBJ;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_8;
	
	if(m_SET.SETNo == DB_PARA_ALL)
	{		
		pDataField[0] = Start;
		pDataField[1] = Num;
		
		//-----------�������ݿ�ӿ�-----------------
		WritePtr=2;

		for(int i=0;i<Num;i++)
		{
			DB_GetMapBitObjDisp(Start+i, &tMBO);

			memset(pDataField+WritePtr,0,sizeof(TMBO_Disp));
			memcpy(pDataField+WritePtr,tMBO.strStationName,20);
			WritePtr += 20;
			
			pDataField[WritePtr++] = LOBYTE(tMBO.wControl);
			pDataField[WritePtr++] = HIBYTE(tMBO.wControl);		
			pDataField[WritePtr++] = tMBO.bySelfAddr;
			pDataField[WritePtr++] = tMBO.bySideAddr;
			pDataField[WritePtr++] = LLBYTE(tMBO.dwSideIP);
			pDataField[WritePtr++] = LHBYTE(tMBO.dwSideIP);
			pDataField[WritePtr++] = HLBYTE(tMBO.dwSideIP);
			pDataField[WritePtr++] = HHBYTE(tMBO.dwSideIP);
			pDataField[WritePtr++] = LLBYTE(tMBO.dwBaud);
			pDataField[WritePtr++] = LHBYTE(tMBO.dwBaud);
			pDataField[WritePtr++] = HLBYTE(tMBO.dwBaud);
			pDataField[WritePtr++] = HHBYTE(tMBO.dwBaud);
			pDataField[WritePtr++] = tMBO.bySMBStart;
			pDataField[WritePtr++] = tMBO.bySMBNum;
			pDataField[WritePtr++] = tMBO.byRMBStart;
			pDataField[WritePtr++] = tMBO.byRMBNum;
			WritePtr += 4;//����
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;
	
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_MAPBITOBJ;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = Num;
	
	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;

	return TRUE;
}

BOOL CSCanbusPro::TxdWriteMBORes()
{
	TTxdHead 	*pTxdHead;
	TMBO_Disp	*pMBO;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,StartNo,Num,Max;
	WORD		wFrameLength,WritePtr;
	STATUS		rc;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetMapBitObjNum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_MAPBITOBJ;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	WritePtr = 0;

	pMBO = (TMBO_Disp *)m_pMemBlock;

	//�����޸�
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_8)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1��ȡ��ֵ����Ϣ
		StartNo = pRecData[0];
		Num = pRecData[1];
		// ���ȼ��
		if(wFrameLength < (9+2+Num*sizeof(TMBO_Disp)))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2����ż��
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// ����д���������ż��
		if(StartNo >= Max)
			Num = 0;
		// ����˳��д��
		else if(StartNo != m_SET.Off)
			Num = 0;
		// Խ��ʱ
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3����˳���ж�
		pMBO += StartNo;
		pRecData += 2;
		for(i=0;i<Num;i++)
		{
			memset(pMBO,0,sizeof(TMBO_Disp));
			memcpy(pMBO->strStationName,pRecData,20);			
			pMBO->wControl = MAKEWORD(pRecData[20],pRecData[21]);
			pMBO->bySelfAddr = pRecData[22];
			pMBO->bySideAddr = pRecData[23];
			pMBO->dwSideIP = MAKEDWORD(pRecData[24],pRecData[25],pRecData[26],pRecData[27]);
			pMBO->dwBaud = MAKEDWORD(pRecData[28],pRecData[29],pRecData[30],pRecData[31]);
			pMBO->bySMBStart = pRecData[32];
			pMBO->bySMBNum  = pRecData[33];
			pMBO->byRMBStart  = pRecData[34];
			pMBO->byRMBNum  = pRecData[35];
			
			m_SET.Off++;
			pMBO++;
			pRecData += sizeof(TMBO_Disp);
		}

		// ������ֵд��
		if(m_RxdHead->APPControl&0x40)
		{
			// �������
			if(m_SET.Off != Max)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				Num = 0;
			}
			else
			{
				// �����޸�
				rc = DB_SetMapBitObjDisp(DB_PARA_ALL, (TMBO_Disp *)m_pMemBlock);
				if(rc != DB_ERR_OK)
					pTxdHead->Status |= STATUS_CTL_ERROR;
				
			}
		}

		// ��䷵��ֵ
		memcpy(pDataField,(BYTE *)&m_RxdHead->Range,2+Num*sizeof(TMBO_Disp));
		pDataField[1] = Num;

		WritePtr = 2+Num*sizeof(TMBO_Disp);
	}
	else //��ʱ��֧��һ����ֵ	
	{
		Num = pRecData[0];
		StartNo = pRecData[1];
		
		// ���ȼ��
		if(wFrameLength < (9+2+sizeof(TMBO_Disp)))
			return FALSE;
		
		pRecData += 2;

		memset(pMBO,0,sizeof(TMBO_Disp));
		memcpy(pMBO->strStationName,pRecData,20);			
		pMBO->wControl = MAKEWORD(pRecData[20],pRecData[21]);
		pMBO->bySelfAddr = pRecData[22];
		pMBO->bySideAddr = pRecData[23];
		pMBO->dwSideIP = MAKEDWORD(pRecData[24],pRecData[25],pRecData[26],pRecData[27]);
		pMBO->dwBaud = MAKEDWORD(pRecData[28],pRecData[29],pRecData[30],pRecData[31]);
		pMBO->bySMBStart = pRecData[32];
		pMBO->bySMBNum	= pRecData[33];
		pMBO->byRMBStart  = pRecData[34];
		pMBO->byRMBNum	= pRecData[35];
		
		rc = DB_SetMapBitObjDisp(StartNo, (TMBO_Disp *)m_pMemBlock);
		
		if(rc != DB_ERR_OK)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			pDataField[0] = 0;
			pDataField[1] = 0;
			WritePtr = 2;
		}
		else
		{
			pDataField[0] = 1;
			pDataField[1] = StartNo;
			memcpy(pDataField+2,(BYTE *)&m_RxdHead->Range+2,sizeof(TMBO_Disp));
			WritePtr = 2+sizeof(TMBO_Disp);
		}
	}

	m_TxdWritePtr += WritePtr;
	
	TxdTail(0);

	return TRUE;
}

BOOL CSCanbusPro::TxdReadFLParaRes()
{
#if(SYSTEM_TYPE == _SYS_KF6500_)
	TTxdHead 	*pTxdHead;
	TFL_Disp	tFL;
	PBYTE       pDataField;
	WORD		Num;
	WORD		WritePtr;
	BYTE		byAppCtrl = 0x20;
	
	if((m_NetCfg.dwStatus&MASK_READ_PARA)==0)
		return FALSE;

	Num = DB_GetFLObjNum();
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READATTRIB);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_FLPARA;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	
	//-----------�������ݿ�ӿ�-----------------
	pDataField[0] = Num;
	WritePtr=1;

	for(int i=0;i<Num;i++)
	{
		DB_GetFLObjDisp(i, &tFL);

		pDataField[WritePtr++] = LOBYTE(tFL.wControl);
		pDataField[WritePtr++] = HIBYTE(tFL.wControl);		
		pDataField[WritePtr++] = tFL.byNetAddr;
		pDataField[WritePtr++] = tFL.byFLNo;
		pDataField[WritePtr++] = LLBYTE(tFL.dwMNetIP);
		pDataField[WritePtr++] = LHBYTE(tFL.dwMNetIP);
		pDataField[WritePtr++] = HLBYTE(tFL.dwMNetIP);
		pDataField[WritePtr++] = HHBYTE(tFL.dwMNetIP);
		pDataField[WritePtr++] = LLBYTE(tFL.dwSNetIP);
		pDataField[WritePtr++] = LHBYTE(tFL.dwSNetIP);
		pDataField[WritePtr++] = HLBYTE(tFL.dwSNetIP);
		pDataField[WritePtr++] = HHBYTE(tFL.dwSNetIP);
		WritePtr += 4;//����
	}
	
	m_TxdWritePtr = WritePtr+13;
	
	TxdTail(0);

	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;
#endif
	return TRUE;
}

BOOL CSCanbusPro::TxdWriteFLParaRes()
{
#if(SYSTEM_TYPE == _SYS_KF6500_)
	TTxdHead 	*pTxdHead;
	TFL_Disp	*pFL;
	PBYTE		pRecData;
	PBYTE       pDataField;
	WORD 		i,Num;
	WORD		wFrameLength;
	STATUS		rc = DB_ERR_OK;
	
	if((m_NetCfg.dwStatus&MASK_WRITE_PARA)==0)
		return FALSE;
	
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Num = DB_GetFLObjNum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_FLPARA;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	pFL = (TFL_Disp *)m_pMemBlock;

	// ���ȼ��
	do{
		if(wFrameLength < (9+1+Num*sizeof(TFL_Disp)))
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			break;
		}

		if(pRecData[0] != Num)
		{
			pTxdHead->Status |= STATUS_CTL_ERROR;
			break;
		}

		pRecData++;
		// �����޸�
		for(i=0;i<Num;i++)
		{
			memset(pFL,0,sizeof(TFL_Disp));
			pFL->wControl = MAKEWORD(pRecData[0],pRecData[1]);
			pFL->byNetAddr = pRecData[2];
			pFL->byFLNo = pRecData[3];
			pFL->dwMNetIP = MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]);
			pFL->dwSNetIP = MAKEDWORD(pRecData[8],pRecData[9],pRecData[10],pRecData[11]);
			pFL->dwRsv = MAKEDWORD(pRecData[12],pRecData[13],pRecData[14],pRecData[15]);
			
			pFL++;
			pRecData += sizeof(TFL_Disp);
		}

		rc = DB_SetFLObjDisp(DB_PARA_ALL, (TFL_Disp *)m_pMemBlock);
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;
	}while(0);
	
	// ��䷵��ֵ
	memcpy(pDataField,(BYTE *)&m_RxdHead->Range,1+Num*sizeof(TFL_Disp));
	m_TxdWritePtr += 1+Num*sizeof(TFL_Disp);
	
	TxdTail(0);
#endif
	return TRUE;
}
BOOL CSCanbusPro::TxdReadBreakerTripCntRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	TTxdHead 		*pTxdHead;
 	PBYTE         	pDataField;
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;

	pTxdHead->ObjType 	= DOC_BREAKERTRIPCNT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 
	pDataField[0] = LOBYTE(*wACT1Cnt);
	pDataField[1] = HIBYTE(*wACT1Cnt);
	pDataField[2] = LOBYTE(*wACT2Cnt);
	pDataField[3] = HIBYTE(*wACT2Cnt);
	pDataField[4] = LOBYTE(*wACT3Cnt);
	pDataField[5] = HIBYTE(*wACT3Cnt);

	m_TxdWritePtr += 6;
	
	TxdTail(0);	
#endif

	return TRUE;
}

BOOL CSCanbusPro::RxdWriteBreakerTripCnt()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
 	PBYTE         	pRecData;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	*wACT1Cnt = MAKEWORD(pRecData[0],pRecData[1]);
	*wACT2Cnt = MAKEWORD(pRecData[2],pRecData[3]);
	*wACT3Cnt = MAKEWORD(pRecData[4],pRecData[5]);
#endif

	return TRUE;
}

//---------------------DebugPLC----------------
BOOL CSCanbusPro::Rxd_DebugStep1Req()
{
	
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	DWORD dwStep = MAKEDWORD(pRecData[1], pRecData[2], pRecData[3], pRecData[4]);
	SetPLCDebugStep1(dwStep);
	return Txd_DebugStep1Res();
}
BOOL CSCanbusPro::Txd_DebugStep1Res()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_STEP1;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	*pDataField++ = 1;

	*pDataField++ = pRecData[1];;
	*pDataField++ = pRecData[2];;
	*pDataField++ = pRecData[3];;
	*pDataField++ = pRecData[4];;

				
	m_TxdWritePtr += 1+4;
	
	TxdTail(0); 
						
	return TRUE;
}
BOOL CSCanbusPro::Rxd_DebugStep2Req()
{
	PBYTE        pRecData;

	pRecData = (PBYTE)&m_RxdHead->Range;
	DWORD dwStep = MAKEDWORD(pRecData[1], pRecData[2], pRecData[3], pRecData[4]);
	SetPLCDebugStep2(dwStep);
	return Txd_DebugStep2Res();
}
BOOL CSCanbusPro::Txd_DebugStep2Res()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_STEP2;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	*pDataField++ = 1;

	*pDataField++ = pRecData[1];;
	*pDataField++ = pRecData[2];;
	*pDataField++ = pRecData[3];;
	*pDataField++ = pRecData[4];;

				
	m_TxdWritePtr += 1+4;
	
	TxdTail(0); 
						
	return TRUE;	
}

BOOL CSCanbusPro::Rxd_DebugSetBreakPointReq()
{
	PBYTE        pRecData;

	pRecData = (PBYTE)&m_RxdHead->Range;
	WORD wBK = MAKEWORD(pRecData[0], pRecData[1]);

//-1��ʾ�������һ��
	if(wBK == (WORD)-1)
	{
		SetPLCDebugOneStep(1);
	}
	else
	{
		SetPLCDebugBreakPoint(wBK);
		SetPLCDebugLoop(1);
	}
	return Txd_DebugSetBreakPointRes();
}
BOOL CSCanbusPro::Txd_DebugSetBreakPointRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_BP;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	*pDataField++ = pRecData[0];
	*pDataField++ = pRecData[1];


	m_TxdWritePtr += 2;
	
	TxdTail(0); 
						
	return TRUE;	
}

BOOL CSCanbusPro::Rxd_DebugWatchReq()
{
	
	return Txd_DebugWatchRes();
}
BOOL CSCanbusPro::Txd_DebugWatchRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	Num = pRecData[0];
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_WATCH;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	

	*pDataField++ = Num;


//�̵������
	for(i=0;i<2*Num;i++)
	{
		*pDataField++ = pRecData[1+i];
	}

//�̵���ֵ


	for(i=0;i<Num;i++)
	//-----------�������ݿ�ӿ�-----------------
	{
		no = MAKEWORD(pRecData[1+i*2],pRecData[2+i*2]);
		// ��ȡ�̵�����״̬		
		*pDataField++ = ReadRWFullValue(no);

//		ReportMsg("Protocol read RW no=%d value=%d",no,ReadRWFullValue(no));
	}
				
	m_TxdWritePtr += 1+Num*2+Num;
	
	TxdTail(0); 

						
	return TRUE;	
}

//===================================���Կɱ���߼�===========================================
BOOL CSCanbusPro::Rxd_DebugGetContextReq()
{
	PBYTE        pRecData;
	pRecData = (PBYTE)&m_RxdHead->Range;

	return Txd_DebugGetContextRes();
	
}
BOOL CSCanbusPro::Txd_DebugGetContextRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	Num = pRecData[4];
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_CONTEXT;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	
	WORD wLine, wBP;
	wLine = GetPLCCurLine();
	wBP = GetPLCDebugBreakPoint();
	
	*pDataField++ = LOBYTE(wLine);
	*pDataField++ = HIBYTE(wLine);
	
	*pDataField++ = LOBYTE(wBP);
	*pDataField++ = HIBYTE(wBP);

	*pDataField++ = Num;

//�̵������
	for(i=0;i<2*Num;i++)
	{
		*pDataField++ = pRecData[5+i];
	}

//�̵���ֵ


	for(i=0;i<Num;i++)
	//-----------�������ݿ�ӿ�-----------------
	{
		no = MAKEWORD(pRecData[5+i*2],pRecData[6+i*2]);
		// ��ȡ�̵�����״̬		
		*pDataField++ = ReadRWFullValue(no);

//		ReportMsg("Protocol read RW no=%d value=%d",no,ReadRWFullValue(no));
	}
				
	m_TxdWritePtr += 4+1+Num*2+Num;
	
	TxdTail(0); 
						
	return TRUE;
}
BOOL CSCanbusPro::Rxd_DebugStopReq()
{
	StopPLCDebug();
	return Txd_DebugStopRes();
	
}
BOOL CSCanbusPro::Txd_DebugStopRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD		Num,no;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	Num = pRecData[0];
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_STOP;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 
	
	TxdTail(0); 
						
	return TRUE;
}

BOOL CSCanbusPro::Rxd_SetDBINReq()
{
	
	PBYTE        pRecData;
	WORD i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	BYTE byCount = pRecData[0];
	WORD wStart = MAKEWORD(pRecData[1], pRecData[2]);
	
	//�޸Ļ���������
	
	if(byCount > DEBUG_IN_NUM)
		byCount = DEBUG_IN_NUM;
	for(i=0;i<byCount;i++)
	{
		// д�̵�����״̬ 	
		WriteDebugIn(wStart+i, pRecData[3+i]);
	}

	//��Ӧ��
	Txd_SetDBINRes(byCount,wStart);
	
}
BOOL CSCanbusPro::Txd_SetDBINRes(BYTE byCout, WORD wStart)
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;

		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_DBIN;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;
	
	
	*pDataField++ = byCout;
	
	*pDataField++ = LOBYTE(wStart);
	*pDataField++ = HIBYTE(wStart);


	m_TxdWritePtr += 3;
	
	TxdTail(0); 
						
	return TRUE;	
}

BOOL CSCanbusPro::Rxd_GetDBOutReq()
{
	Txd_GetDBOutRes();
}
BOOL CSCanbusPro::Txd_GetDBOutRes()
{
	TTxdHead 	*pTxdHead;
	PBYTE        pDataField,pRecData;
	WORD 		i;

	pRecData = (PBYTE)&m_RxdHead->Range;
	BYTE byCount = pRecData[0];
	WORD wStart = MAKEWORD(pRecData[1], pRecData[2]);
		
	TxdHead(0xC0+m_CurrentReqSeq,FUN_LOGICDEBUG); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;

	pTxdHead->ObjType	= DEBUG_RELAY_DBOUT;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField = (PBYTE)&pTxdHead->Range;

	if(byCount > DEBUG_OUT_NUM)
		byCount = DEBUG_OUT_NUM;

	*pDataField++ = byCount;
	
	*pDataField++ = LOBYTE(wStart);
	*pDataField++ = HIBYTE(wStart);

	//�̵���ֵ
	for(i=0;i<byCount;i++)
	{
		// ��ȡ�̵�����״̬		
		*pDataField++ = ReadDebugOut(wStart+i);
	}
				
	m_TxdWritePtr += 3+byCount;
	
	TxdTail(0); 
						
	return TRUE;		
}

//===================================���Կɱ���߼�===========================================
//����͸������
BOOL CSCanbusPro::RxdTransData()
{
	BYTE			*pRecData = &m_RxdHead->Range;
	BYTE			*pTransData = pRecData+sizeof(TTransDataHead)-1;
    TAppInfo 		*pAppInfo;
	WORD			wClass,wPort,i;

	wClass = MAKEWORD(pRecData[0],pRecData[1]);

	switch(wClass)
	{
		case 0x0000://�������
			//�����б�Э��˿ڹ㲥
			pAppInfo = &G_pCfgTable->TaskApp[SYS_PORT_MAX];
			for(i=SYS_PORT_MAX;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
			{
				if(pAppInfo->pAppItem == NULL)
					continue;

				// �޳�����Լ
				if(pAppInfo->dwPortID == m_wPortID)
					continue;

				// �ҵ�ͬ���Լ
				if(pAppInfo->pAppItem->fpEntry != (TTASK_ENTRY)SCanbus_Task)
					continue;

				// ���Լ������Ϣ
				DAE_WriteTransData(wClass, pAppInfo->dwPortID, 0, pTransData);
			}
			break;
			
		case 0x0001://ȷ�Ϲ���
			//����Դ�˿�ʱ����
			if(m_TransReqPortID == m_wPortID)
			{
				LogError("RxdTransData",FILE_LINE,"Connect ack from the same port(%x)!",m_wPortID);
				break;
			}

			// ��Դ�˿ں���������Ϣ
			DAE_WriteTransData(wClass, m_TransReqPortID, 0,pTransData);
			DAE_WriteTransData(wClass, m_wPortID, 0,pTransData);
			break;
						
		case 0x0100://����������
		case 0x0200://д��������
			//pTDHead->wDesPort = MAKEWORD(pRecData[2],pRecData[3]);
			//pTDHead->wDataLen = MAKEWORD(pRecData[6],pRecData[7]);
			DAE_WriteTransData(wClass, MAKEWORD(pRecData[2],pRecData[3]), MAKEWORD(pRecData[6],pRecData[7]),pTransData);

			TSysTime stTime1;

			ReadSystemTime(&stTime1);
			ReportMsg("Port 0x%x: Rxd trans data<<<<< Time=%d.%d.%d.%d FILE_NO=%d",m_wPortID,stTime1.Hour,\
				stTime1.Minute,stTime1.Second,stTime1.MSecond,MAKEWORD(pRecData[10],pRecData[11]));

			break;

		case 0x0300://�������
			DAE_WriteTransData(wClass, SYS_PORTID_FL, MAKEWORD(pRecData[6],pRecData[7]),pTransData);
			break;
			
		default:
			break;
	}
	
	return TRUE;
}

//����͸������
BOOL CSCanbusPro::TxdTransData()
{
	TTransDataHead 	*pTDHead = (TTransDataHead 	*)&m_pMemBlock;
	TTxdHead 		*pTxdHead;
 	PBYTE         	pDataField;

	TxdHead(0xC0+m_CurrentResSeq,FUN_COMMONCONTROL); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;

	pTxdHead->ObjType 	= DOC_TRANSDATA;	
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NORANGE; 
	
	switch(pTDHead->wClass)
	{
		case 0x0000://�������				
			memset(pDataField,0,sizeof(TTransDataHead));			
			m_TxdWritePtr += sizeof(TTransDataHead)-1;
			ReportMsg("Port 0x%x: send trans data 0x0000>>>>>>>>",m_wPortID);

			// ��¼�����������
			m_TransReqPortID = pTDHead->wSrcPort;
			break;
			
		case 0x0010://��Ӧ����
			pDataField[0] = 0x10;
			pDataField[1] = 0x00;

			//���˿ڷ�������ģ���֪: Ŀ�Ķ˿������������
			if(pTDHead->wSrcPort == m_wPortID)
			{
				pDataField[2] = LOBYTE(m_TransReqPortID);
				pDataField[3] = HIBYTE(m_TransReqPortID);
			}
			//�Է��˿ڷ���������֪: Ŀ�Ķ˿�����Ӧ��
			else if(pTDHead->wDesPort == m_wPortID)
			{
				pDataField[2] = LOBYTE(pTDHead->wSrcPort);
				pDataField[3] = HIBYTE(pTDHead->wSrcPort);
			}
			else
			{
				LogError("TxdTransData",FILE_LINE,"Invalis trans data port 0x%x!",m_wPortID);
				return FALSE;
			}

			// Դ�˿�������
			pDataField[4] = LOBYTE(m_wPortID);
			pDataField[5] = HIBYTE(m_wPortID);
			pDataField[6] = 0;
			pDataField[7] = 0;
			m_TxdWritePtr += sizeof(TTransDataHead)-1;

			ReportMsg("Port 0x%x: send trans data 0x0010! from 0x%x to 0x%x>>>>>>>>",\
				m_wPortID,pTDHead->wSrcPort,pTDHead->wDesPort);
			break;
			
		case 0x0100://����������
		case 0x0200://д��������
		case 0x0300://���ͨ��
			if(pTDHead->wDesPort != m_wPortID)
			{
				LogError("TxdTransData",FILE_LINE,"DesPort is error( %x != %x)!",pTDHead->wDesPort,m_wPortID);
				break;
			}

			pDataField[0] = LOBYTE(pTDHead->wClass);
			pDataField[1] = HIBYTE(pTDHead->wClass);
			pDataField[2] = LOBYTE(pTDHead->wDesPort);
			pDataField[3] = HIBYTE(pTDHead->wDesPort);
			pDataField[4] = LOBYTE(pTDHead->wSrcPort);
			pDataField[5] = HIBYTE(pTDHead->wSrcPort);
			
			if(pTDHead->wDataLen <= (1024-sizeof(TTransDataHead)))
			{
				pDataField[6] = LOBYTE(pTDHead->wDataLen);
				pDataField[7] = HIBYTE(pTDHead->wDataLen);
				memcpy(pDataField+8,&pTDHead->Data,pTDHead->wDataLen);
			}
			else
			{
				LogError("TxdTransData",FILE_LINE,"Trans data length is overflow!");
				return FALSE;
			}
			
			m_TxdWritePtr += pTDHead->wDataLen+sizeof(TTransDataHead)-1;

//			ReportMsg("Port 0x%x: send trans data 0x%x! from 0x%x to 0x%x Len=%d>>>>>>>>",\
//				m_wPortID,pTDHead->wClass,pTDHead->wSrcPort,pTDHead->wDesPort,pTDHead->wDataLen);
			break;
		default:
			break;
	}

	TxdTail(0);	

	TSysTime stTime2;
	
	ReadSystemTime(&stTime2);
	ReportMsg("Port 0x%x: Txd trans data>>>>> Time=%d.%d.%d.%d FILE_NO=%d",m_wPortID,stTime2.Hour,\
		stTime2.Minute,stTime2.Second,stTime2.MSecond,MAKEWORD(pDataField[10],pDataField[11]));

	return TRUE;
}

BOOL CSCanbusPro::TxdReadDCSIDataRes()
{
#if(SYSTEM_TYPE == _SYS_KF1300_)
	TTxdHead 	*pTxdHead;
	PBYTE       pDataField;
	WORD		i,WritePtr=0;
	TLoadSupV	stSI;
	
	TxdHead(0xC0+m_CurrentReqSeq,FUN_READVALUE); 

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType 	= DOC_DCSIDATA;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_NUM_8; 

	pDataField[WritePtr++] = MAX_DCSIDATA_NUM;
	
	for(i=0;i<MAX_DCSIDATA_NUM;i++)
	{	
	   ReadHistrorySIData(i,&stSI);

	   pDataField[WritePtr++] = LLBYTE(*((DWORD *)&stSI.Value));
	   pDataField[WritePtr++] = LHBYTE(*((DWORD *)&stSI.Value));
	   pDataField[WritePtr++] = HLBYTE(*((DWORD *)&stSI.Value));
	   pDataField[WritePtr++] = HHBYTE(*((DWORD *)&stSI.Value));
	   
	   pDataField[WritePtr++] = LLBYTE(stSI.Time.Lo);
	   pDataField[WritePtr++] = LHBYTE(stSI.Time.Lo);
	   pDataField[WritePtr++] = HLBYTE(stSI.Time.Lo);
	   pDataField[WritePtr++] = HHBYTE(stSI.Time.Lo);
	   
	   pDataField[WritePtr++] = LOBYTE(stSI.Time.Hi);
	   pDataField[WritePtr++] = HIBYTE(stSI.Time.Hi);
	 } 		

	m_TxdWritePtr = WritePtr+13;
	TxdTail(0);	
#endif			
	return TRUE;
}

BOOL CSCanbusPro::Rxd_SetAIValue(BYTE FunCode)
{
	BYTE	*pRecData = &m_RxdHead->Range;
	WORD	Index;
	long	lValue;
	
	for(int i=0;i<pRecData[0];i++)
	{
		Index = MAKEWORD(pRecData[i*6+1],pRecData[i*6+2]);
		lValue = MAKEDWORD(pRecData[i*6+3],pRecData[i*6+4],pRecData[i*6+5],pRecData[i*6+6]);

#if(SYSTEM_TYPE == _SYS_KF1300_)
		if(FunCode == FUN_SUBSTITUTE)
			DB_WriteRM(Index, lValue, DB_WRITE_COMPEL);
		else
			DB_WriteRM(Index, lValue, DB_WRITE_UNCOMPEL);
#else
		if(FunCode == FUN_SUBSTITUTE)
			DB_WriteAI(Index, lValue, DB_WRITE_COMPEL);
		else
			DB_WriteAI(Index, lValue, DB_WRITE_UNCOMPEL);
#endif
	}
	
	return TRUE;
}

BOOL CSCanbusPro::Rxd_SetBIValue(BYTE FunCode)
{
	BYTE	*pRecData = &m_RxdHead->Range;
	WORD	Index;
	BYTE	Status;
	
	for(int i=0;i<pRecData[0];i++)
	{
		Index = MAKEWORD(pRecData[i*3+1],pRecData[i*3+2]);

		if(FunCode == FUN_SUBSTITUTE)
			DB_WriteBI(Index, pRecData[i*3+3], DB_WRITE_COMPEL);
		else
			DB_WriteBI(Index, pRecData[i*3+3], DB_WRITE_UNCOMPEL);
	}

	return TRUE;
}

void CSCanbusPro::ClearAllEvent()
{
	WORD Num,i; 
	
	SetFlag(SF_BURSTBI,FLAG_OFF);		
	SetFlag(SF_SOE,FLAG_OFF);		
	SetFlag(SF_RELAYALARM,FLAG_OFF);		
	SetFlag(SF_RELAYACTION,FLAG_OFF);		
	SetFlag(SF_RELAYSTART,FLAG_OFF);		
	SetFlag(SF_AIE,FLAG_OFF);		

	DB_GetBIENum(&Num);
	for(i=0;i<Num;i++)
		DB_IncBIERead();

	DB_GetSOENum(&Num);
	for(i=0;i<Num;i++)
		DB_IncSOERead();

	DB_GetAIENum(&Num);
	for(i=0;i<Num;i++)
		DB_IncAIERead();
	
	DB_GetRelayAlarmReportNum(&Num);
	for(i=0;i<Num;i++)
		DB_IncRelayAlarmReportRead();

	DB_GetRelayActionReportNum(&Num);
	for(i=0;i<Num;i++)
		DB_IncRelayActionReportRead();

	DB_GetRelayStartReportNum(&Num);
	for(i=0;i<Num;i++)
		DB_IncRelayStartReportRead();

	return;
}

BOOL CSCanbusPro::RxdReadDistDirReq()
{
	PBYTE			pRecData;
	BYTE			*pFileData;
	char			*pFileName;
	TDistIndex		*pDistList;
	TSysTime		*pFiletime;
	DWORD			dwFileLength,dwRtnLength;
	BYTE			nHeadPtr,nTailPtr,nMax,nCnt;
	STATUS			rc;
	
	pRecData = (PBYTE)&m_RxdHead->Range;
	
	if(m_RxdHead->ObjQulifier != ((INDEX_SIZE_0<<4)+QUALI_NORANGE))
	{
		LogError("CSCanbusPro::RxdReadDistDirReq",FILE_LINE,"Not suspond Object Qulifier!");
		return FALSE;
	}

	memset(&m_DistDir,0,sizeof(TDistDirItem));
		
	pFileData = (BYTE *)g_pFileDownBuffer;
	
	// 1����¼�������ļ�
	dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*DIST_CAPACITY_MAX;
	memset(pFileData,0x00,dwFileLength);
	
	rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwFileLength != dwRtnLength))
	{
		// �����ļ�����
		LogError("CSCanbusPro::RxdReadDistDirReq",FILE_LINE,"Error on open DistList.ind!");
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
			memcpy(m_DistDir.strFileNameList[nCnt],pFileName,DIST_FILENAME_LEN);

			// ��¼�ļ�ʱ��
			pFiletime = &m_DistDir.tFileTimeList[nCnt];
			// ת����ϵͳʱ��
			pFiletime->Year = (pFileName[0]-'0')*10+(pFileName[1]-'0');
			pFiletime->Month = (pFileName[2]-'0')*10+(pFileName[3]-'0');
			pFiletime->Day = (pFileName[4]-'0')*10+(pFileName[5]-'0');
			pFiletime->Hour = (pFileName[6]-'0')*10+(pFileName[7]-'0');
			pFiletime->Minute = (pFileName[8]-'0')*10+(pFileName[9]-'0');
			pFiletime->Second = (pFileName[10]-'0')*10+(pFileName[11]-'0');
			pFiletime->MSecond = (pFileName[12]-'0')*100+(pFileName[13]-'0')*10+(pFileName[14]-'0');
			
			nCnt++;
		}
		
		nHeadPtr = (nHeadPtr+1)%nMax;
	}while(nHeadPtr != nTailPtr);

	m_DistDir.nSentCnt = 0;
	m_DistDir.nFileNum = nCnt;
	SetFlag(SF_SENDRES_DISTDIR,FLAG_ON);

	return TRUE;
}

BOOL CSCanbusPro::TxdReadDistDirRes()
{
	TTxdHead	*pTxdHead;
	TSysTime	*pFiletime;
	char		*strFileName;
	PBYTE		pDataField;
	BYTE		Start,Num,Max;
	WORD		WritePtr;
	BYTE		byAppCtrl = 0x20;
	char		strFileALLName[DIST_INDEX_NAMELEN];
	DWORD		nSize;
		
	if((m_NetCfg.dwStatus&MASK_READ_SET)==0)
		return FALSE;
	
	Max = m_DistDir.nFileNum;
	Start = m_DistDir.nSentCnt;	
	Num = PERFRM_NUM_DISTDIR;
	
	// ���͵�һ֡
	if(Start == 0)
		byAppCtrl |= 0x80;
			
	// �������һ֡
	if((Start+PERFRM_NUM_DISTDIR) >= Max)
	{
		Num = Max-Start;
		byAppCtrl |= 0x40;

		SetFlag(SF_SENDRES_DISTDIR,FLAG_OFF);
	}
			
	TxdHead(byAppCtrl+m_CurrentReqSeq,FUN_READVALUE);
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_DISTDIR;
	pTxdHead->ObjQulifier = (INDEX_SIZE_0<<4)+QUALI_ADDR_8;
		
	pDataField[0] = Start;
	pDataField[1] = Num;
			
	//-----------�������ݿ�ӿ�-----------------
	WritePtr=2;
	
	for(int i=0;i<Num;i++)
	{
		// �ļ���
		strFileName = m_DistDir.strFileNameList[Start+i];
		memcpy(pDataField+WritePtr,strFileName,DIST_FILENAME_LEN);
		WritePtr += DIST_FILENAME_LEN;

		// ��ȡ�����ļ�����
		strcpy(strFileALLName,strFileName);
		strcat(strFileALLName,".CFG");
		if(!FS_GetFileLength(strFileALLName,&nSize))
		{
			LogError("TxdReadDistDirRes",FILE_LINE,"Read dist file error(%s) error!",strFileALLName);
			nSize = 0;
		}
		pDataField[WritePtr++] = LLBYTE(nSize);
		pDataField[WritePtr++] = LHBYTE(nSize);
		pDataField[WritePtr++] = HLBYTE(nSize);
		pDataField[WritePtr++] = HHBYTE(nSize);
		
		// ��ȡ�����ļ�����
		strcpy(strFileALLName,strFileName);
		strcat(strFileALLName,".DAT");
		nSize = 0;
		if(!FS_GetFileLength(strFileALLName,&nSize))
		{
			LogError("TxdReadDistDirRes",FILE_LINE,"Read dist file error(%s) error!",strFileALLName);
			nSize = 0;
		}
		pDataField[WritePtr++] = LLBYTE(nSize);
		pDataField[WritePtr++] = LHBYTE(nSize);
		pDataField[WritePtr++] = HLBYTE(nSize);
		pDataField[WritePtr++] = HHBYTE(nSize);
		
		// ʱ��
		pFiletime = &m_DistDir.tFileTimeList[Start+i];
		pDataField[WritePtr++] = pFiletime->Year;
		pDataField[WritePtr++] = pFiletime->Month;
		pDataField[WritePtr++] = pFiletime->Day;
		pDataField[WritePtr++] = pFiletime->Hour;
		pDataField[WritePtr++] = pFiletime->Minute;
		pDataField[WritePtr++] = pFiletime->Second;
		pDataField[WritePtr++] = LOBYTE(pFiletime->MSecond);
		pDataField[WritePtr++] = HIBYTE(pFiletime->MSecond);
	}
	
	m_DistDir.nSentCnt += Num;

	m_TxdWritePtr = WritePtr+13;
		
	//����ȷ����Ϣ
	m_ResInfo.ResFunCode = pTxdHead->FunCode;
	m_ResInfo.ObjType = DOC_DISTDIR;
	m_ResInfo.ResSeq  = m_CurrentReqSeq;
	m_ResInfo.ObjNum = 1;
	
	TxdTail(0);
	
	m_CurrentReqSeq = (m_CurrentReqSeq+1)&0x0F;
	
	return TRUE;
}

BOOL CSCanbusPro::TxdReadClass1()
{
	WORD wNum;
	
	//����ң�ط�У
	if(GetFlag(SF_HaveBOReturn))
	{
		if(TxdBOReturn())
			return TRUE;
	}
	
	//���б�λң�ţ�����
	if((DB_GetBIENum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdBurstBI())
			return TRUE;
	}
	
	//����SOE������
	if((DB_GetSOENum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdSoe())
			return TRUE;
	}
	
	
	//���б������棬����
	if((DB_GetRelayAlarmReportNum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdRelayAlarmReport())
			return TRUE;
	}

	if((DB_GetRelayActionReportNum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdRelayActionReport())
			return TRUE;
	}
	
#if(SYSTEM_TYPE == _SYS_KF1300_)
	if(GetFlag(SF_LEDBURST))
	{
		if(TxdLEDStatus(TRUE))
			return TRUE;
	}
	
	//����SOE������
	DWORD dwNum;
	if((DB_GetAIENum(&dwNum)==DB_ERR_OK)&&(dwNum>0))
	{
		if(TxdAIE())
			return TRUE;
	}
else
	if((DB_GetAIENum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdAIE())
			return TRUE;
	}
#endif

	return FALSE;
}

BOOL CSCanbusPro::TxdReadClass2()
{
	if(TxdReadClass1())
		return TRUE;

	//���б仯ң��
	return TxdBurstAI();
}

