//=========================================================================
// ����: sDF103.cpp
// ����: ������103��վ��Լ�����̿���
//=========================================================================
#include "siec103.h"
#include "parafunc.h"
#include "relayfun.h"

#ifdef INSTALL_S103 

void MergeConfigWord(float *pSet)
{
	WORD 	setnum,cfgnum,floatnum,wordnum;
	WORD	wValue1,wValue2;
	DWORD	*pValue = (DWORD *)pSet;
	int		i;
	
	setnum = DB_GetSETNum();
	cfgnum = (DB_GetCFGNum()+31)/32;
	
	wordnum = (DB_GetCFGNum()+15)/16;
	floatnum = setnum-cfgnum;

	// 1���ϲ�����������
	for(i=0;i<cfgnum;i++)
	{
		wValue1 = pSet[i*2];
		wValue2 = pSet[i*2+1];	
		pValue[i] = (wValue2<<16)+wValue1;
	}
	
	// 2������������ֵ
	for(i=0;i<floatnum;i++)
	 	pSet[cfgnum+i] = pSet[wordnum+i];	
}

void SplitConfigWord(float *pSet)
{
	WORD 	setnum,cfgnum,floatnum,wordnum;
	DWORD	*pValue = (DWORD *)pSet;
	int		i;

	setnum = DB_GetSETNum();
	cfgnum = (DB_GetCFGNum()+31)/32;
	
	wordnum = (DB_GetCFGNum()+15)/16;
	floatnum = setnum-cfgnum;

	//=======Ϊ���⸲�ǣ���Ҫ�Ӻ��濪ʼ����=====
	
	// 1������������ֵ
	for(i=floatnum-1;i>=0;i--)
		pSet[i+wordnum] = pSet[i+cfgnum];

	// 2��չ������������
	for(i=wordnum-1;i>=0;i--)
	{
		if(i%2)
			pSet[i] = HIWORD(pValue[i/2]);
		else
			pSet[i] = LOWORD(pValue[i/2]);
	}
}

BEGIN_MESSAGE_MAP(CSiec103,CProtocol)
	ON_SM_TIMEOUT(CSiec103)
	ON_SM_SOE(CSiec103)
//	ON_SM_BIBURST(CSiec103)
	ON_SM_RELAYEVENT(CSiec103)
	ON_SM_BOSELECTECHO(CSiec103)
	ON_SM_BOEXECECHO(CSiec103)
END_MESSAGE_MAP()


//=========================================================================
// ��������: ʵ�ִ�վ103��Լ��Ӧ�ò���ȹ������� 
//=========================================================================
void S103_Task(DWORD *pdwApp)
{
	CSiec103 *pS103=new CSiec103(pdwApp);
	if(pS103->Init()!=TRUE)
		pS103->Exit((char *)"S101 Init Error");
    	pS103->Run();
}

//*******************************************************************************************//
//  ��������: SearchOneFrame 
//  ��������: ���pBuf���Ƿ���һ����Ч����
//  �������: pBuf,Ҫ���ı�����ʼ��ַ
//            wLength,pBuf����Ч����
//  ����ֵ:  
//            FM_OK  +ʵ�ʱ��ĳ���   //��⵽һ��������֡
//            FM_ERR +�Ѵ����ֽ���   //��⵽һ��У������֡
//            FM_LESS+�Ѵ����ֽ���   //��⵽һ����������֡����δ���룩
//  ע: �Ѵ����ֽ�����Ϊ0���´���������һ��ַ��ʼ�������´������������Ѵ�������ֽڣ��Ӷ��ӿ�
//      �����ٶ�
//*******************************************************************************************//
DWORD CSiec103::SearchOneFrame(PBYTE pBuf,WORD wLength)
{
	WORD wFrameLength=5;

	if(wLength<MIN_RXDFRAME_SIZE)
		return FM_LESS;

	pRxdFrameHead = (T_M_FrameHead *)pBuf;

	switch(pRxdFrameHead->FrameFix_10.byStart)
	{
	case SYN_START_10:     //�̶�֡������
		{
		//��ַ����ж�
		if(DB_GetDevAddress() != pRxdFrameHead->FrameFix_10.byAddress)
			return FM_ERR|1;

		//У��͵��ж�
		if( (pRxdFrameHead->FrameFix_10.byChecksum) !=\
			  CheckSum((BYTE *)&(pRxdFrameHead->FrameFix_10.Control),2) )
			return FM_ERR|1;

		//�����ַ����ж�
		if( pRxdFrameHead->FrameFix_10.byStop != SYN_STOP )
			return FM_ERR|1;

		//֡У����ȷ
		return FM_OK|5;
		}

	case SYN_START_68:     //�ɱ�֡������
		{
		wFrameLength = pRxdFrameHead->FrameVar_68.byLength1+6;

		//�����ַ����ж�
		if( pRxdFrameHead->FrameVar_68.byStart1 != pRxdFrameHead->FrameVar_68.byStart2 )
			return FM_ERR|1;

		//���ȵ��ж�
		if( pRxdFrameHead->FrameVar_68.byLength1 != pRxdFrameHead->FrameVar_68.byLength2 )
			return FM_ERR|1;

		//��ַ����ж�
		if(DB_GetDevAddress() != pRxdFrameHead->FrameVar_68.byAddress)
		{
			if(pRxdFrameHead->FrameVar_68.byAddress != 255)
				return FM_ERR|1;
		}
		
		//У��͵��ж�
		if( pBuf[wFrameLength-2] !=\
			  CheckSum((BYTE *)&(pRxdFrameHead->FrameVar_68.Control),pRxdFrameHead->FrameVar_68.byLength1) )
			return FM_ERR|1;

		//�����ַ����ж�
		if( pBuf[wFrameLength-1] != SYN_STOP )
			return FM_ERR|1;

		//֡У����ȷ
		return FM_OK|wFrameLength;
		}

	default:
		return FM_ERR|1;
	}
}

//*******************************************************************************************//
//  ��������: ReceiveControlFlag
//  ��������: ������տ������еı�־(PRM,FCB,FCV)
//  �������:
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::ReceiveControlFlag(BYTE byStart)
{
	BYTE  *FCB;

	if(byStart == SYN_START_10)
	{
		//�ж����ӱ�־λ
		if(pRxdFrameHead->FrameFix_10.Control.PRM != PRM_MASTER)
			return FALSE;

		//�ж�֡������Чλ,����Ч���FCB�������ط�
		if(pRxdFrameHead->FrameFix_10.Control.FCV == FCV_VALID)
		{
			FCB = (BYTE *)(&pRxdFrameHead->FrameFix_10.Control);
			if( (m_DevInfo.byOldFCB) == *FCB )
			 {
				 SetFlag(SF_RETRY);
			 }
			 else
 				 m_DevInfo.byOldFCB = *FCB;
		}
		return TRUE;
	}

	if(byStart == SYN_START_68)
	{
		//�ж����ӱ�־λ
		if(pRxdFrameHead->FrameVar_68.Control.PRM != PRM_MASTER)
			return FALSE;

		//�ж�֡������Чλ,����Ч���FCB�������ط�
		if(pRxdFrameHead->FrameVar_68.Control.FCV == FCV_VALID)
		{
			 FCB = (BYTE *)(&pRxdFrameHead->FrameVar_68.Control);
			 if( m_DevInfo.byOldFCB == *FCB )
			 {
				 SetFlag(SF_RETRY);
			 }
			 else
 				 m_DevInfo.byOldFCB = *FCB;
		}
		return TRUE;
	}

	return TRUE;
}

//*******************************************************************************************//
//  ��������: CheckSum
//  ��������: ʵ���ۼӺ�У��
//  �������: BYTE * pBuf, ���뻺����
//            BYTE byLength, ����������
//  ����ֵ:   BYTE bySum, �ۼӺ�
//*******************************************************************************************//
BYTE CSiec103::CheckSum(BYTE * pBuf,BYTE byLength)
{
	BYTE bySum=0;

	for(WORD i=0;i<byLength;i++)
		bySum+=pBuf[i];

	return bySum;
}

BOOL CSiec103::Init(void)
{
	WORD i;
	
	BYTE flag =0;
	while( flag == 1)
	{
		OS_Sleep(20);
	}		

	//��ʱ3�룬��ֹϵͳ��λ�󣬻�δˢ�����ݣ���Լ�����ⷢ��
	Sleep(3000);

	if(CProtocol::Init() != TRUE)
			return FALSE;

	memset(&m_DevInfo,0,sizeof(TIECInfo));
	m_DevInfo.byCOL = COL_GEN;		   //���ݼ��𲻰���ͨ������

	// 1. =========  ���øù�Լ����Ҫ����Ϣ���͡�=========
	StartMsg(MCC_ReadComm,MAX_RXDFRAME_SIZE/2);  //��Ҫͨ������Ϣ

	m_LocalYXNum = DB_GetBINum();		//��ȡң���ֽ��� 
#if(SYSTEM_TYPE == _SYS_KF1300_)
	m_LocalYCNum = DB_GetRMNum();			//��ȡң�����
#else	
	m_LocalYCNum = DB_GetAINum();			//��ȡң�����
#endif	
	m_LocalDDNum = DB_GetCINum();			//��ȡ��ȸ���
	m_LocalSTNum = DB_GetSETNum()-((DB_GetCFGNum()+31)/32)+((DB_GetCFGNum()+15)/16);//��ֵ��������������Ҫ��ֳ�WORD

	m_nLeftActionReportNum = 0;
	m_pNextRelayEvent = NULL;
	
	// �ϴ����ٻ�ʱ���͵�ң�ű�
	m_OldYXTable = (BYTE *)malloc(m_LocalYXNum);

	// ��ʼ����ң��
	for(i=0;i<((m_LocalYXNum+15)/16);i++)
	{
		DB_ReadBIByte(i*2*8,&m_OldYXTable[i*2],DB_BI_BYTE81);
		DB_ReadBIByte(i*2*8+8,&m_OldYXTable[i*2+1],DB_BI_BYTE81);
	}
	//DB_ReadBI(i,&m_OldYXTable[i]);

	m_Txd.ReadPtr=0;
	m_Txd.Buf=GetTxdBuf();
	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf; 

	// ������һ֡��Ч������Ϣ
	m_pLast68Frame = (BYTE *)malloc(MAX_RXDFRAME_SIZE+6);

	// ���涨ֵ����
	m_ST.Buf = (BYTE *)malloc(sizeof(float)*m_LocalSTNum);

	//����CAN������ʱ��
	SetTimer(TM_1S,1000);

	m_NoRxdCounter = MAX_COMMERR_TIME;
	
    return TRUE;
}

BOOL CSiec103::HaveNewReport(void)
{
	WORD 	wNum;
	BOOL	bHaveReport=FALSE;
	int		i;
	
	if(m_nLeftActionReportNum > 0)
	{
		// ����ѹ��Ͷ��ʱ����¼�
		if(GetMaintSWStatus())
			m_nLeftActionReportNum = 0;
		else
			bHaveReport = TRUE;
	}
	
	if((DB_GetRelayAlarmReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
	{
		// ����ѹ��Ͷ��ʱ����¼�
		if(GetMaintSWStatus())
		{		
			for(i=0;i<wNum;i++)
				DB_IncRelayAlarmReportRead();
		}
		else
			bHaveReport = TRUE;
	}
	

	if((DB_GetRelayActionReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
	{
		// ����ѹ��Ͷ��ʱ����¼�
		if(GetMaintSWStatus())
		{		
			for(i=0;i<wNum;i++)
				DB_IncRelayActionReportRead();
		}
		else
			bHaveReport = TRUE;
	}
	
	return bHaveReport;
}

BOOL CSiec103::HaveNewSOE(void)
{
	WORD 	wNum;
	int		i;
	
	if((DB_GetSOENum(&wNum) == DB_ERR_OK )&&(wNum > 0))
	{
		// ����ѹ��Ͷ��ʱ����¼�
		if(GetMaintSWStatus())
		{		
			for(i=0;i<wNum;i++)
				DB_IncSOERead();
			return FALSE;
		}
		else
			return TRUE;
	}	

	return FALSE;
}


BOOL CSiec103::GetACDFlag(void)
{
	if( GetFlag(SF_RESETCU)|GetFlag(SF_RESETFCB) )
		return TRUE;

	if( GetFlag(SF_START) )
		return TRUE;

	if( GetFlag(SF_SST_ACK) )
		return TRUE;

	// ң�ű�λ����
	if(GetFlag(SF_HAVESOE))
		return TRUE;

	//  �б�������
	if(GetFlag(SF_HAVEREPORT))
		return TRUE;

	// ��ͨ������
	if( GetFlagByte(SF_GEN_DATA/8))
		return TRUE;
	
	// �ٻ����
	if( GetFlag(SF_REPORT_DD) )
		return TRUE;
		
	// ���ٻ�����
	if( GetFlag(SF_CALLALL) )
		return TRUE;

	// ���ٻ�����
	//if( GetFlag(SF_CALLALL_STOP) )
	//	return TRUE;


	return FALSE;
}

//=========================================================================================================
// ��������: Send103P_AI
// ��������: ��103��ʽ��TXD���������YCֵ
// �������: ��
// ����ֵ: 
//=========================================================================================================
void CSiec103::Make103P_AI(DWORD AINo)
{
	TRM_Disp	tRMDisp;
	long 		YcValue=0;

#if(SYSTEM_TYPE == _SYS_KF6500_)
	DB_ReadSecondAI(AINo,&YcValue);
#else
	DB_ReadSecondRM(AINo,&YcValue);
	
	// ȡ�������ɹ�
	if(DB_GetRMDisp(AINo, &tRMDisp) != DB_ERR_OK)
		m_Txd.Buf[m_Txd.WritePtr++] = AINo;
	else
	{
		// ��д���
		m_Txd.Buf[m_Txd.WritePtr++] = LOBYTE(tRMDisp.nIndex);

		// ȥ��С���㲿��
		switch(tRMDisp.nDec)
		{
			case 1: YcValue = (long)(YcValue/10); break; 
			case 2: YcValue = (long)(YcValue/100); break;	
			case 3: YcValue = (long)(YcValue/1000); break;	
			case 4: YcValue = (long)(YcValue/10000); break;	
			default: break;	
		}
	}
#endif

	if(YcValue>=32767)
	{
		m_Txd.Buf[m_Txd.WritePtr++] = 0xFF;
		m_Txd.Buf[m_Txd.WritePtr++] = 0x7F;
	}
	else if(YcValue<=-32768)
	{
		m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
		m_Txd.Buf[m_Txd.WritePtr++] = 0x80;
	}
	else
	{
		m_Txd.Buf[m_Txd.WritePtr++] = LOBYTE(YcValue);
		m_Txd.Buf[m_Txd.WritePtr++] = HIBYTE(YcValue);
	}
}

//=========================================================================================================
// ��������: Make103P_BI
// ��������: 103��ʽ�ĵ���ң�ź͵���ң�ż��
// �������: ��
// ����ֵ: 
//=========================================================================================================
void CSiec103::Make103P_BI(DWORD BIGroupNo)
{
	BYTE		YxValue1=0,YxValue2=0;
	BYTE        ChangeFlag1=0,ChangeFlag2=0;
	WORD		wStartBINo;
	BYTE        i;

	wStartBINo = BIGroupNo*2*8;

	DB_ReadBIByte(wStartBINo,&YxValue1,DB_BI_BYTE81);

	for(i=0;i<8;i++)
		if( (m_OldYXTable[BIGroupNo*2]&(0x01<<i))!=(YxValue1&(0x01<<i)) )
			ChangeFlag1 |= 0x01<<i;

	if( (wStartBINo+8) >= m_LocalYXNum )
	{   
		YxValue2 = 0;
		ChangeFlag2 = 0;
	}
	else
	{
		DB_ReadBIByte(wStartBINo+8,&YxValue2,DB_BI_BYTE81);
		for(i=0;i<8;i++)
		if( (m_OldYXTable[BIGroupNo*2+1]&(0x01<<i))!=(YxValue2&(0x01<<i)) )
			ChangeFlag2 |= 0x01<<i;
	}

	m_Txd.Buf[m_Txd.WritePtr++] = YxValue1;
	m_Txd.Buf[m_Txd.WritePtr++] = YxValue2;
	m_Txd.Buf[m_Txd.WritePtr++] = ChangeFlag1;
	m_Txd.Buf[m_Txd.WritePtr++] = ChangeFlag2;
	m_Txd.Buf[m_Txd.WritePtr++] = 0;
}

//=========================================================================================================
// ��������: Make103P_Report
// ��������: ���103��ʽ�ı�������
// �������: ��
// ����ֵ: 
//=========================================================================================================
void CSiec103::Make103P_Report(WORD wFaultNo,TRelayEventHead *pEvent)//,TAbsTime *pStartTime)
{
	TAbsTime	tEvTime;
	TSysTime	tSysTime;
	WORD		wMsecond;
	BYTE		FD_Num;
	
	tEvTime.Lo = pEvent->AbsTimeLo;
	tEvTime.Hi = pEvent->AbsTimeHi;
	AbsTimeTo(&tEvTime, &tSysTime);
	
	// ���INF
	m_Txd.Buf[m_Txd.WritePtr++] = LOBYTE(pEvent->nINF);
	m_Txd.Buf[m_Txd.WritePtr++] = HIBYTE(pEvent->nINF);

	// INF��λ�ͱ���������0
	memset(m_Txd.Buf+m_Txd.WritePtr,0,6);
	m_Txd.Buf[m_Txd.WritePtr++] = 0;
	m_Txd.Buf[m_Txd.WritePtr++] = 0;

	// ��������
#if(SYSTEM_TYPE == _SYS_KF6500_)
	m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
	m_Txd.Buf[m_Txd.WritePtr++] = 0x65;
	m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
	m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
#else
	m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
	m_Txd.Buf[m_Txd.WritePtr++] = 0x13;
	m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
	m_Txd.Buf[m_Txd.WritePtr++] = 0x00;
#endif

	// ���״̬
	m_Txd.Buf[m_Txd.WritePtr++] = pEvent->bStatus;

	// ������ʱ��
	m_Txd.Buf[m_Txd.WritePtr++] = 0;
	m_Txd.Buf[m_Txd.WritePtr++] = 0;

	// ���������
	m_Txd.Buf[m_Txd.WritePtr++] = LOBYTE(wFaultNo);
	m_Txd.Buf[m_Txd.WritePtr++] = HIBYTE(wFaultNo);

	// ������ʱ��
	wMsecond = tSysTime.Second*1000+tSysTime.MSecond;
	m_Txd.Buf[m_Txd.WritePtr++] = LOBYTE(wMsecond);
	m_Txd.Buf[m_Txd.WritePtr++] = HIBYTE(wMsecond);
	m_Txd.Buf[m_Txd.WritePtr++] = tSysTime.Minute;
	m_Txd.Buf[m_Txd.WritePtr++] = tSysTime.Hour;

	if(pEvent->FD_Num <= MAX_IND61_FDNUM)
	{
		FD_Num = pEvent->FD_Num;
	}
	else
	{
		FD_Num = MAX_IND61_FDNUM;		
		LogError("Make103P_Report",FILE_LINE,"Report FD num is too many!!! FD_Num=%d",pEvent->FD_Num);
	}
	
	m_Txd.Buf[m_Txd.WritePtr++] = FD_Num;
	
	// ��������ź͹��ϵ���
	memcpy(m_Txd.Buf+m_Txd.WritePtr,pEvent+1,FD_Num*6);
	m_Txd.WritePtr += FD_Num*6;
	
	ReportMsg("ITEM: Inf=%d FD_Num=%d ",pEvent->nINF,FD_Num);
}

//*******************************************************************************************//
//  ��������: SetAsduHead
//  ��������: ����Ӧ�÷������ݵ�Ԫ�ı�ʶ��Ϣͷ
//  �������:
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::SetAsduHead(BYTE byTYP,BYTE byVSQ,BYTE byCOT,BYTE byADDRESS,BYTE byFUN,BYTE byINF)
{
	pTxdFrameHead->FrameVar_68.Asdu.byTYP=byTYP;
	pTxdFrameHead->FrameVar_68.Asdu.byVSQ=byVSQ;
	pTxdFrameHead->FrameVar_68.Asdu.byCOT=byCOT;
	pTxdFrameHead->FrameVar_68.Asdu.byADDRESS=byADDRESS;
	pTxdFrameHead->FrameVar_68.Asdu.byFUN=byFUN;
	pTxdFrameHead->FrameVar_68.Asdu.byINF=byINF;

	m_Txd.WritePtr+=6;

	return TRUE;
}

BOOL CSiec103::SetBO(TBO* pBO,BYTE NeedReturnFlag)//ң�����ú���
{
	BYTE Mode;
	WORD SwitchNo;
	BYTE Attrib;

	//���浱ǰң����Ϣ
	switch(pBO->Command)
	{
	case PBO_EXECUTE:	 
	case PBO_CANCEL:	 
		pBO->Attrib = Attrib = m_BO.Attrib; //�ϴα���ģ��ԣԣ�
		break;
	}
	
	//�����ң��Ԥ�ã�����ң����Ϣ���������Ԥ�ã�AttribΪ�ϴα����Attrib
	m_BO = *pBO; 
	
	switch(pBO->Attrib)
	{
	case PBO_OPEN:	Attrib = BO_OPEN;	break;
	case PBO_CLOSE:	Attrib = BO_CLOSE;	break;	
	case PBO_NULL:	Attrib = BO_NULL;	break;
	default:	break;	
    }

	m_BO.NeedRetFlag = NeedReturnFlag;  //�Ƿ���ȴ�ң�ط�У
	SwitchNo = pBO->No;

	switch(pBO->Command)
	{
	case PBO_SELECT:  	 DB_BOSelect( SwitchNo, Attrib );	        break;
	case PBO_EXECUTE:	 Mode=1;DB_BOExec( SwitchNo,Attrib,Mode );	break;
	case PBO_CANCEL:	 Mode=2;DB_BOExec( SwitchNo,Attrib,Mode );	break;
	case PBO_DIRECT:	 DB_BODirect(  SwitchNo, Attrib );	        break;
	default:		
		break;
	}
	return TRUE;
}

BOOL CSiec103::SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag)//ң�����ú���
{
	TBO BO;

	if(Attrib == PBO_OPEN)
		BO.No =	BONo*2;
	else if(Attrib == PBO_CLOSE)
		BO.No =	BONo*2+1;
	else
		BO.No =	BONo;
		
	BO.Command	=	Command;
	BO.Attrib	=	PBO_NULL;//Attrib;
	
	return SetBO(&BO,NeedReturnFlag);	
}

BOOL CSiec103::P103_YK_Pro(BYTE byYKNo,BYTE byActionTyp)
{
	BOOL	bReturn;

	switch(byActionTyp)
	{
	case 0x81://ң��ѡ��--��
		bReturn = SetBO(byYKNo,PBO_SELECT,PBO_OPEN);
		ReportMsg("IEC103: Recieve YK OFF slecct!!!");
		break;
	case 0x82://ң��ѡ��--��
		bReturn = SetBO(byYKNo,PBO_SELECT,PBO_CLOSE);
		ReportMsg("IEC103: Recieve YK ON select !!!");
		break;
	case 0x01://ң��ִ��--��
		bReturn = SetBO(byYKNo,PBO_EXECUTE,PBO_OPEN);
		ReportMsg("IEC103: Recieve YK OFF execute!!!");
		break;
	case 0x02://ң��ִ��--��
		bReturn = SetBO(byYKNo,PBO_EXECUTE,PBO_CLOSE);
		ReportMsg("IEC103: Recieve YK ON execute!!!");
		break;
	case 0x41://ң�س���--��
		bReturn = SetBO(byYKNo,PBO_CANCEL,PBO_OPEN);
		ReportMsg("IEC103: Recieve YK OFF cancel!!!");
		break;
	case 0x42://ң�س���--��
		bReturn = SetBO(byYKNo,PBO_CANCEL,PBO_CLOSE);
		ReportMsg("IEC103: Recieve YK ON cancel!!!");
		break;
	default:
		return FALSE;
	}
	return bReturn;
}

// ��δ��ȡ�ı�����ѡ�����µ�
BOOL CSiec103::SearchNewReport(TRelayReport &Report,BYTE &Type)
{
	TRelayAlarmEv 	tAlram;
//	TRelayStartEv 	tStart;
	TRelayActionEv 	tAction;
	BOOL			bAlarmValid=FALSE;
	BOOL			bStartValid=FALSE;
	BOOL			bActionValid=FALSE;
	TAbsTime		tm1,tm2,tm3;
	WORD wNum;

	tm1.Hi=tm2.Hi=tm3.Hi=0xFFFF;
	tm1.Lo=tm2.Lo=tm3.Lo=0xFFFFFFFF;
	
	// ���澯����
	if((DB_GetRelayAlarmReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
	{
		if(DB_ReadRelayAlarmReport(&tAlram,DB_BLANK) == DB_ERR_OK)
		{
			tm1.Lo = tAlram.tEvent.tAction.AbsTimeLo;
			tm1.Hi = tAlram.tEvent.tAction.AbsTimeHi;
			bAlarmValid = TRUE;
		}
		else
		{
			LogError("CSiec103::SearchNewReport",FILE_LINE,"Read alarm report error!");
			DB_IncRelayAlarmReportRead();
		}	
	}
	
	// ����������
//	if((DB_GetRelayStartReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
//	{
//		DB_ReadRelayStartReport(&tStart,DB_BLANK);
//		tm2.Lo = tStart.tEvent.tAction.AbsTimeLo;
//		tm2.Hi = tStart.tEvent.tAction.AbsTimeHi;
//		bStartValid = TRUE;
//	}
	
	// ����������
	if((DB_GetRelayActionReportNum(&wNum) == DB_ERR_OK )&&(wNum > 0))
	{
		if(DB_ReadRelayActionReport(&tAction,DB_BLANK) == DB_ERR_OK)
		{
			tm3.Lo = tAction.tEvent.tAction.AbsTimeLo;
			tm3.Hi = tAction.tEvent.tAction.AbsTimeHi;
			bActionValid = TRUE;
		}
		else
		{
			DB_IncRelayActionReportRead();
			LogError("CSiec103::SearchNewReport",FILE_LINE,"Read action report error!");
		}
	}

	// ����Ч����ֱ�ӷ���
	if(!bAlarmValid&&!bStartValid&&!bActionValid)
		return FALSE;

	// �ҳ���ɵı��� !!!
	
	// �Ƚϸ澯������
/*	
	if(IsTimeNew(tm1,tm2))
	{
		// �Ƚ������Ͷ���
		if(IsTimeNew(tm2,tm3))
		{
			Type = DB_EVENT_RACT;
			Report.tAction= tAction;
		}
		else
		{
			Type = DB_EVENT_RSTT;
			Report.tStart = tStart;
		}
	}
	else
*/	
	{
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
		
	}

	return TRUE;
/*	
	if(bAlarmValid)
	{
		// �и澯����
		if(bStartValid)
		{
			// ����������

			// �澯����vs��������
			if(IsTimeNew(tm1,tm2))
			{
				// *********�澯���ıȽ���*******
				if(bActionValid)
				{
					// �ж�������
					
					// �澯����vs��������
					if(IsTimeNew(tm3,tm1))
					{
						Type = DB_EVENT_RACT;
						Report.tAction = tAction;
						return TRUE;
					}
				}
			}
			else
			{
				// *********�������ıȽ���*******
				if(bActionValid)
				{
					// �ж�������
					
					// ��������vs��������
					if(IsTimeNew(tm3,tm2))
					{
						Type = DB_EVENT_RACT;
						Report.tAction = tAction;
						return TRUE;
					}
				}

				Type = DB_EVENT_RSTT;
				Report.tStart = tStart;
				return TRUE;
			}
		}
		else
		{
			// ����������
			if(bActionValid)
			{
				// �ж�������
				
				// �澯����vs��������
				if(IsTimeNew(tm3,tm1))
				{
					Type = DB_EVENT_RACT;
					Report.tAction = tAction;
					return TRUE;
				}
			}
		}
		
		Type = DB_EVENT_RALM;
		Report.tAlram = tAlram;
		return TRUE;
	}
	else if(bStartValid)
	{
		// ����������
		if(bActionValid)
		{
			// �ж�������
			
			// ��������vs��������
			if(IsTimeNew(tm3,tm2))
			{
				Type = DB_EVENT_RACT;
				Report.tAction = tAction;
				return TRUE;
			}
		}

		Type = DB_EVENT_RSTT;
		Report.tStart = tStart;
		return TRUE;
	}
	else if(bActionValid)
	{
		Type = DB_EVENT_RACT;
		Report.tAction= tAction;
		return TRUE;
	}
	Type = 0xFF;
	return FALSE;
*/
}

void CSiec103::OnTimeOut(DWORD id)
{
	WORD Num;
/*
	BYTE bTest=0;

	if(bTest)
	{
		TRelayActionEv  tEvent;
		TRelayEventHead	*pAction;
		BYTE			*pFDData;
		TAbsTime		tTime;
		DWORD			dwOffset;
		float			fValue;
		DWORD			dwValue;
		
		tEvent.tEvent.Num = 5;
		tEvent.tEvent.wFaultNo = 0x55;

		dwOffset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);

		for(int i=0;i<5;i++)
		{
			pAction = (TRelayEventHead *)(tEvent.aEvent+dwOffset);
			
			ReadAbsTime(&tTime);
			
			pAction->AbsTimeHi = tTime.Hi;
			pAction->AbsTimeLo = tTime.Lo;
			pAction->bStatus = 1;
			pAction->nINF = 2100;
			pAction->FD_Num= 9;

			pFDData = (BYTE *)(pAction+1);
			for(int j=0;j<9;j++)
			{
				pFDData[0+j*6] = j;
				pFDData[1+j*6] = 0;

				fValue = 100.1*(i+1);
				dwValue = *(DWORD *)&fValue;
				
				pFDData[2+j*6] = LLBYTE(dwValue);
				pFDData[3+j*6] = LHBYTE(dwValue);
				pFDData[4+j*6] = HLBYTE(dwValue);
				pFDData[5+j*6] = HHBYTE(dwValue);
			}

			dwOffset += sizeof(TRelayEventHead)+6*9;

			Sleep(100);
		}
		
		DB_WriteRelayActionReport(&tEvent);
	}
*/
	if(m_NoRxdCounter > 0)
	{
		// ����ʱ
		m_NoRxdCounter--;

		// ͨ��ʧ��ʱ����
		if(m_NoRxdCounter == 0)
			RegisterCommStatus(COMM_STATUS_BREAK);
	}
	
	// ����©���±���
	if(HaveNewReport())
		SetFlag(SF_HAVEREPORT);

	// ����©����SOE
	if( HaveNewSOE())
		SetFlag(SF_HAVESOE);
}

//*******************************************************************************************//
//  ��������: OnFrame
//  ��������: ʵ�ִ�վ103��Լ�Ľ��մ���
//  �������: pBuf ���ջ�����ָ��
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
void CSiec103::OnFrame(BYTE* pBuf)
{
	static BOOL bReturn=FALSE;

	pRxdFrameHead = (T_M_FrameHead *)pBuf;

	//����ͨ��״̬
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	m_NoRxdCounter = MAX_COMMERR_TIME;
	
	//����������еı�־
	if( ReceiveControlFlag(pRxdFrameHead->FrameFix_10.byStart) != TRUE )
		return;

	//�ط�
	if( CheckClearFlag(SF_RETRY) )
	{
		TxdRetry();
		return;
	}

	//����Ƿ���ң�ط�У
//	if(CheckClearFlag(SF_HaveBOReturn))
//	{
//		Txd68_BO_Echo_64();
//	}
	    
	switch(pRxdFrameHead->FrameFix_10.byStart)
	{
	case SYN_START_10:     //�̶�֡������

		switch(pRxdFrameHead->FrameFix_10.Control.FC)
		{
		case FC_M_RESETCU:	    bReturn=Rxd10_ResetCU();	    break;     //��λͨ�ŵ�Ԫ
		case FC_M_RESETFCB:	    bReturn=Rxd10_ResetFCB();	    break;     //��λFCB
		case FC_M_CALLLINK:	    bReturn=Rxd10_Call_Link();	    break;     //�ٻ���·״̬
		case FC_M_CALLCLASS1:	bReturn=Rxd10_Call_Class1();	break;     //�ٻ�һ������
		case FC_M_CALLCLASS2:   bReturn=Rxd10_Call_Class2();    break;	   //�ٻ���������	
		default:	break;
		}
		break;

	case SYN_START_68:     //�ɱ�֡������

		// ����68֡
		memcpy(m_pLast68Frame,pBuf,pRxdFrameHead->FrameVar_68.byLength1+6);
		
		switch(pRxdFrameHead->FrameVar_68.Asdu.byTYP)
		{
		case TYP_M_SST:         bReturn=Rxd68_SST_6();			break;		//��ʱ
		case TYP_M_CALL:	    bReturn=Rxd68_CallAll_7();		break;		//���ٻ�
		case TYP_M_COMMAND:		bReturn=Rxd68_Command_20();		break;		//һ������()
		case TYP_M_YKCOMMAND:	bReturn=Rxd68_YKCommand_64();	break;		//���ƶ�·������
		case TYP_M_CALLDD:		bReturn=Rxd68_CallDD_88();		break;		//�ٻ��������������
		case TYP_M_GCOMMAND:	bReturn=Rxd68_GComm_21();		break;		//ͨ����������
		case TYP_M_GDATA:		bReturn=Rxd68_GData_10();		break;		//ͨ������
		default:	break;
		}
		break;

	default:               //����֡
		break;
	}
}

void CSiec103::OnSOE()
{	
	if(HaveNewSOE())
		SetFlag(SF_HAVESOE);
}

void CSiec103::OnRelayEvent(DWORD MsgCode, DWORD wParam)
{	
	// �޳���������
	if(MsgCode == 0)
		return;
	
	if(HaveNewReport())
		SetFlag(SF_HAVEREPORT);
}

//ң��Ԥ�÷�У��Ϣӳ��
void CSiec103::OnBOSelectEcho(WORD switchno,WORD attrib,WORD result)	
{
	ReportMsg("IEC103: Send YK Select Echo!!!");

	m_BO.RetResult = result;
	Txd68_BO_Echo_64();
//	SetFlag(SF_HaveBOReturn);
}

void CSiec103::OnBOExecEcho(WORD switchno,WORD attrib,WORD result)	
{
	ReportMsg("IEC103: Send YK Execute Echo!!!");
	
	m_BO.RetResult = result;
	Txd68_BO_Echo_64();
//	SetFlag(SF_HaveBOReturn);
}

//*******************************************************************************************//
//  ��������: Txd68_Head
//  ��������: ʵ�ֿɱ�֡��֡ͷ�Ĵ���
//  �������: BYTE byFC,������
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_Head(BYTE byFC)
{
	m_Txd.ReadPtr=0;
		
	pTxdFrameHead->FrameVar_68.byStart1 = SYN_START_68;
	pTxdFrameHead->FrameVar_68.byStart2 = SYN_START_68;
	pTxdFrameHead->FrameVar_68.Control.RES = 0;
	pTxdFrameHead->FrameVar_68.Control.PRM = PRM_SLAVE;
	pTxdFrameHead->FrameVar_68.Control.ACD = GetACDFlag();
	pTxdFrameHead->FrameVar_68.Control.DFC = 0;
	pTxdFrameHead->FrameVar_68.Control.FC = byFC;
	pTxdFrameHead->FrameVar_68.byAddress = DB_GetAddress();

	m_Txd.WritePtr=(PBYTE)&(pTxdFrameHead->FrameVar_68.Asdu)-(PBYTE)&(pTxdFrameHead->FrameVar_68.byStart1);

	return TRUE;
}

//*******************************************************************************************//
//  ��������: Txd68_Tail
//  ��������: ʵ�ֿɱ�֡��֡β�Ĵ���
//  �������:
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_Tail()
{
	pTxdFrameHead->FrameVar_68.byLength1=m_Txd.WritePtr-4;
	pTxdFrameHead->FrameVar_68.byLength2=m_Txd.WritePtr-4;

	m_Txd.Buf[m_Txd.WritePtr++]=CheckSum((BYTE *)&(pTxdFrameHead->FrameVar_68.Control),pTxdFrameHead->FrameVar_68.byLength1);
	m_Txd.Buf[m_Txd.WritePtr++]=SYN_STOP;

//	HaveTxdFlag=1;
	if( WriteComm(m_Txd.WritePtr,pTxdFrameHead->FrameVar_68.byAddress,0) )
		return TRUE;

	return  FALSE;
}

//*******************************************************************************************//
//  ��������: Txd10_BaseFrame
//  ��������: ʵ�̶ֹ�֡��֡�ķ��ʹ���
//  �������: BYTE byFC,������
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd10_BaseFrame(BYTE byFC)
{
	m_Txd.ReadPtr=0;

	pTxdFrameHead->FrameFix_10.byStart = SYN_START_10;
	pTxdFrameHead->FrameFix_10.Control.RES = 0;
	pTxdFrameHead->FrameFix_10.Control.PRM = PRM_SLAVE;
	pTxdFrameHead->FrameFix_10.Control.ACD = GetACDFlag();
	pTxdFrameHead->FrameFix_10.Control.DFC = 0;
	pTxdFrameHead->FrameFix_10.Control.FC  = byFC;
	pTxdFrameHead->FrameFix_10.byAddress   = DB_GetAddress();
	pTxdFrameHead->FrameFix_10.byChecksum  = CheckSum((BYTE *)&(pTxdFrameHead->FrameFix_10.Control),2);
	pTxdFrameHead->FrameFix_10.byStop=SYN_STOP;

	m_Txd.WritePtr=sizeof(T_S_FrameFix);

//	HaveTxdFlag=1;
	if( WriteComm(m_Txd.WritePtr,pTxdFrameHead->FrameVar_68.byAddress,0) )
		return TRUE;
	else
		return FALSE;
}

//***************************************************************************************
// 	�������� TxdClass1Data
//	�������ܣ�ʵ��һ�����ݵķ��ʹ����������ٻ�ң�ź�SOE��
//  �����������
//  ����ֵ��  ��
//**************************************************************************************
BOOL CSiec103::TxdClass1Data(void)
{
	if( CheckClearFlag(SF_RESETCU)|CheckClearFlag(SF_RESETFCB) )
	{
		return Txd_Indication_5(COT_S_RESETCU);
	}

	if( CheckClearFlag(SF_START) )
	{
		return Txd_Indication_5(COT_S_START);
	}
	
	if( CheckClearFlag(SF_SST_ACK) )
	{
		return Txd_SST_6();
	}

	//��SOE
	if( CheckClearFlag(SF_HAVESOE) )
	{
		//�Դ�ʱ��ı��Ĵ���SOE
		return Txd68_SOE_41();
	}

	//�б�������
	if( CheckClearFlag(SF_HAVEREPORT) )
	{
		return Txd68_Report_61();
	}

	// ��ֵ�������
	if(GetFlagByte(SF_GEN_DATA/8))
	{
		//����ֵ
		if(GetFlag(SF_GEN_READSTDATA))
			return Txd68_STDataReadRtn_10(1,m_ST.Group+1);

		//����ֵ����
		if(CheckClearFlag(SF_GEN_READSTAREA))
			return Txd68_STAreaReadRtn_10();

		//����ѹ��
		if(CheckClearFlag(SF_GEN_READSBO))
			return Txd68_SBOReadRtn_10();

		//��������ֵ
		if(GetFlag(SF_GEN_READSYSSTDATA))
			return Txd68_STDataReadRtn_10(5,0xFF);

		//д��ֵ\�л���ֵ��\д��ѹ��
		if(CheckClearFlag(SF_GEN_WRITEWITHCONFIRM))
			return Txd68_GenWriteWithConfirm();
/*
		//д������ֵ
		if(CheckClearFlag(SF_GEN_WRITESTDATA))
			return Txd68_GenWriteWithConfirm();

		//�л���ֵ��
		if(CheckClearFlag(SF_GEN_WRITESTAREA))
			return Txd68_GenWriteWithConfirm();

		//д��ѹ��
		if(CheckClearFlag(SF_GEN_WRITESBO))
			return Txd68_GenWriteWithConfirm();
*/		
		//ִ��ȷ��
		if(CheckClearFlag(SF_GEN_WRITEWITHEXECUTE))
			return Txd68_GenWriteWithExucute();

		//����ȷ��
		if(CheckClearFlag(SF_GEN_WRITEWITHCANCEL))
			return Txd68_GenWriteWithExucute();
		
		SetFlagByte(SF_GEN_DATA/8, 0);
	}	
		
	//�е���ϱ�
	if( CheckClearFlag(SF_REPORT_DD) )
	{
		return Txd68_DD_36();
	}

	//�����ٻ���־
	if( GetFlag(SF_CALLALL) )
	{
		//���䵥��״̬
		if((CheckClearFlag(SF_CALLALL_BI))&&m_LocalYXNum)
			return Txd68_BI_44(COT_S_CALL);
		
		if((CheckClearFlag(SF_CALLALL_AI))&&m_LocalYCNum)
			return TxdClass2Data(COT_S_CALL);
		
		//���ٻ�����
		SetFlag(SF_CALLALL,FLAG_OFF);

		return Txd68_CallStop_8();
	}

//	if( GetFlag(SF_CALLALL_STOP) )
//	{
//		//�����ٻ�����֡
//		SetFlag(SF_CALLALL_STOP,FLAG_OFF);
//		return Txd68_CallStop_8();
//	}

	return Txd10_NoData_9();
}
//***************************************************************************************
// 	�������� TxdClass2Data
//	�������ܣ�ʵ�ֶ������ݵķ��ʹ���(Ŀǰ��Ϊң��ֵ��
//  �����������
//  ����ֵ��  ��
//**************************************************************************************
BOOL CSiec103::TxdClass2Data(BYTE Reason)
{
	if(m_LocalYCNum == 0)
		return Txd10_NoData_9();
	
#if(SYSTEM_TYPE == _SYS_KF1300_)
	return Txd68_RM_GEN(Reason);
#elif(KF6500_TYPE_AI == TYP_S_ME2)
	return Txd68_AI_9(Reason);
#elif(KF6500_TYPE_AI == TYP_S_ME3)
	return Txd68_AI_50(Reason);
#endif
}	

//***************************************************************************************
// 	��������: Txd68_CallStop_8
//	��������: �������ٻ�����֡
//  �������: ��
//  ����ֵ:   ���ͳɹ�  TRUE
//			  ����ʧ��	FALSE
//**************************************************************************************
BOOL CSiec103::Txd68_CallStop_8(void)
{
	BOOL			bReturn;

	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_CALLSTOP,0x81,COT_S_CALLSTOP,pTxdFrameHead->FrameVar_68.byAddress,255,0);

	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byScanNumber;

	bReturn = Txd68_Tail();
	
	return bReturn;
}

//=========================================================================================================
// ��������: Txd68_AI_9
// ��������: ����ASDU-9��ʽYCֵ
// �������: Reason ����ԭ��
// ����ֵ: ���ͽ��
//=========================================================================================================
BOOL CSiec103::Txd68_AI_9(BYTE Reason)
{
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_ME2,9,Reason,pTxdFrameHead->FrameVar_68.byAddress,DEV_FUN_TYPE,144);
	
	//����������
	Make103P_AI(M_TYPE_Ia);
	Make103P_AI(M_TYPE_Ib);
	Make103P_AI(M_TYPE_Ic);
	Make103P_AI(M_TYPE_Ua);
	Make103P_AI(M_TYPE_Ub);
	Make103P_AI(M_TYPE_Uc);
	Make103P_AI(M_TYPE_P);
	Make103P_AI(M_TYPE_Q);
	Make103P_AI(M_TYPE_PF);
		
	return Txd68_Tail();
}

//=========================================================================================================
// ��������: Txd68_AI_50
// ��������: ����ASDU-50��ʽYCֵ
// �������: Reason ����ԭ��
// ����ֵ: ���ͽ��
//=========================================================================================================
BOOL CSiec103::Txd68_AI_50(BYTE Reason)
{
	WORD SendNum=0;
		
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_ME3,m_LocalYCNum,Reason,pTxdFrameHead->FrameVar_68.byAddress,DEV_FUN_TYPE,148);
	
#if(SYSTEM_TYPE == _SYS_KF6500_)
	m_Txd.Buf[m_Txd.WritePtr++] = 0;
#endif
	
	
	//����������
	for(int i=0;i<m_LocalYCNum;i++)
	{
		// �������Լ����ʱ
#if(SYSTEM_TYPE == _SYS_KF1300_)
		if(!DB_IsMeasureSent(i))
			continue;
#endif

		Make103P_AI(i);
		SendNum++;
	}

	pTxdFrameHead->FrameVar_68.Asdu.byVSQ = SendNum;
		
	return Txd68_Tail();
}

//=========================================================================================================
// ��������: Txd68_RM_GEN
// ��������: ����ͨ�÷����ʽ�ı�������ֵ
// �������: Reason ����ԭ��
// ����ֵ: ���ͽ��
//=========================================================================================================
BOOL CSiec103::Txd68_RM_GEN(BYTE Reason)
{
	TRM_Disp	tRMDisp;
	TGenData	*pTxdGenData;
	long		lYcValue=0;
	float		fYcValue=0;
	DWORD		dwYcValue=0;
	WORD 		SendNum=0;
	int 		i;

	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;
	pTxdGenData = (TGenData	*)&pTxdFrameHead->FrameVar_68.Asdu.byGenData;
		
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_GDATA,0x81,Reason,DB_GetAddress(),FUN_GEN,INF_S_GEN_RENTRYVALUE);
	m_Txd.Buf[m_Txd.WritePtr++] = 0;
	
	// ��ȡ��������ֵ
	pTxdGenData->Ngd.Count = m_DevInfo.byLastCnt;
	pTxdGenData->Ngd.Number= 1;
	pTxdGenData->Gin.byGroup = 6;
	pTxdGenData->Gin.byEntry = 0xFF;
	pTxdGenData->byKod = 1;//ʵ��ֵ
		
	// GDD			
	// ��������������
	pTxdGenData->Gdd.byDataType = DAT_7;//������
	pTxdGenData->Gdd.byDataSize = 4;//DATASIZE

	m_Txd.WritePtr += 7;

	// �����䱣������ֵ
	for(int i=0;i<m_LocalYCNum;i++)
	{
		// �������Լ����ʱ
#if(SYSTEM_TYPE == _SYS_KF1300_)
		if(!DB_IsMeasureSent(i))
			continue;
		
		DB_ReadSecondRM(i,&lYcValue);

		fYcValue = lYcValue;
		// ȡ�������ɹ�
		if(DB_GetRMDisp(i, &tRMDisp) == DB_ERR_OK)
		{		
			// ȥ��С���㲿��
			switch(tRMDisp.nDec)
			{
				case 1: fYcValue = (fYcValue/10); break; 
				case 2: fYcValue = (fYcValue/100); break; 
				case 3: fYcValue = (fYcValue/1000); break;	
				case 4: fYcValue = (fYcValue/10000); break; 
				default: break; 
			}
		}
#else
		DB_ReadSecondRM(i,&lYcValue);

		fYcValue = lYcValue;
		// ȡ�������ɹ�
		if(DB_GetRMDisp(i, &tRMDisp) == DB_ERR_OK)
		{		
			// ȥ��С���㲿��
			switch(tRMDisp.nSecDec)
			{
				case 1: fYcValue = (fYcValue/10); break; 
				case 2: fYcValue = (fYcValue/100); break; 
				case 3: fYcValue = (fYcValue/1000); break;	
				case 4: fYcValue = (fYcValue/10000); break; 
				default: break; 
			}
		}
#endif		

		dwYcValue = *(DWORD *)&fYcValue;

		m_Txd.Buf[m_Txd.WritePtr++] = LLBYTE(dwYcValue);
		m_Txd.Buf[m_Txd.WritePtr++] = LHBYTE(dwYcValue);
		m_Txd.Buf[m_Txd.WritePtr++] = HLBYTE(dwYcValue);
		m_Txd.Buf[m_Txd.WritePtr++] = HHBYTE(dwYcValue);
		
		SendNum++;
	}

	// ��������ֵ����
	pTxdGenData->Gdd.byDataNum = SendNum;
				
	return Txd68_Tail();
}

//***************************************************************************************
// 	��������: Txd68_BI_44
//	��������: ���ٻ�ʱ����ĵ���״̬
//  �������: ��
//  ����ֵ:   ��
//**************************************************************************************
BOOL CSiec103::Txd68_BI_40(BYTE Reason)
{
	BYTE 	retBI;
	
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_BISP,m_LocalYXNum,Reason,DB_GetAddress(),DEV_FUN_TYPE,INF_S_BI);

    //����ң��
    for(int i=0;i<m_LocalYXNum&&m_Txd.WritePtr<250;i++)
    {
		DB_ReadBI(i, &retBI);
		m_Txd.Buf[m_Txd.WritePtr++] = (retBI==DB_BI_CLOSE)?1:0;
    }
	
	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byScanNumber;//SIN

	return Txd68_Tail();
}	

//***************************************************************************************
// 	��������: Txd68_BI_44
//	��������: ���ٻ�ʱ����ĵ���״̬��״̬�仯��Ϣ֡
//  �������: ��
//  ����ֵ:   ��
//**************************************************************************************
BOOL CSiec103::Txd68_BI_44(BYTE Reason)
{
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_CALLYX,(m_LocalYXNum+15)/16,Reason,DB_GetAddress(),DEV_FUN_TYPE,INF_S_BI);

    //����ң��
    for(int i=0;i<((m_LocalYXNum+15)/16)&&m_Txd.WritePtr<250;i++)
        Make103P_BI(i);

	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byScanNumber;//SIN

	return Txd68_Tail();
}	

//***************************************************************************************
// 	��������: Txd68_BI_41
//	��������: SOE����
//  �������: ��
//  ����ֵ:   ��
//**************************************************************************************
BOOL CSiec103::Txd68_SOE_41(void)
{
	TEventBI 		tEvent;
	TSysTime		time;
	WORD			MSecond;
	WORD			Num = 0;
	int 			i;

	Txd68_Head(FC_S_DATA);

	DB_GetSOENum(&Num);
	
	SetAsduHead(TYP_S_BURSTSPI,0x80,COT_S_BURST,DB_GetAddress(),DEV_FUN_TYPE,0);

	// 1 ��inf��ʼ���
	m_Txd.WritePtr--;

	for(i=0;i<Num;i++)
	{
		DB_ReadSOE(&tEvent);
		DB_IncSOERead();

		AbsTimeTo(&tEvent.stTime, &time);
		MSecond=time.MSecond+time.Second*1000;

		m_Txd.Buf[m_Txd.WritePtr++] = tEvent.wNo+1;
		m_Txd.Buf[m_Txd.WritePtr++] = (tEvent.bStatus == BI_CLOSE)?1:0;
		m_Txd.Buf[m_Txd.WritePtr++] = LOLOBYTE(MSecond);
		m_Txd.Buf[m_Txd.WritePtr++] = LOHIBYTE(MSecond);
		m_Txd.Buf[m_Txd.WritePtr++] = time.Minute;
		m_Txd.Buf[m_Txd.WritePtr++] = time.Hour;

		ReportMsg("Report soe! No=%d Status=%d ",tEvent.wNo,tEvent.bStatus);

		pTxdFrameHead->FrameVar_68.Asdu.byVSQ++;
		
		// Խ��ʱ����
		if((m_Txd.WritePtr+6)>(255-7))
			break;
	}
	
	m_Txd.Buf[m_Txd.WritePtr++] = 0;//SIN
	
	// ���ڷ��͹�һ�����ݺ󣬸������ܻᷢ���仯����Ҫ����ˢ��
	pTxdFrameHead->FrameVar_68.Control.ACD = GetACDFlag();
	
	if(!Txd68_Tail())
	{
		SetFlag(SF_RETRY);
		return FALSE;
	}
	
	return TRUE;
}

//***************************************************************************************
// 	��������: Txd68_RelayReport
//	��������: ������������-
//  �������: ��
//  ����ֵ:   ��
//**************************************************************************************
BOOL CSiec103::Txd68_Report_61()
{
	WORD			nLen;
	BYTE 			bType;

	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;

	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_REPORT,0x80,COT_S_BURST,DB_GetAddress(),DEV_FUN_TYPE,0);
	// 1.��inf��ʼ���
	m_Txd.WritePtr--;

	// 2.�ȶ�ȡ�������汨��
	do
	{
		// û��ʣ�౨��ʱ
		if((m_nLeftActionReportNum == 0)||(m_pNextRelayEvent == NULL))
		{
			m_pNextRelayEvent = NULL;
			m_nLeftActionReportNum = 0;
			
			// 3.�ٰ���ʱ��˳�������ȡ����
			if(!SearchNewReport(m_tCurrentReport,bType))
				break;
			
			if(bType == DB_EVENT_RACT)
			{
				// ��ʼ������ָ��
				m_pNextRelayEvent = &m_tCurrentReport.tAction.tEvent.tAction;
				m_nLeftActionReportNum = m_tCurrentReport.tAction.tEvent.Num;

				LogError("Txd68_Report_61",FILE_LINE,"Send a new action report!!!");
				
				// ֱ�ӵ���ָ��
				DB_IncRelayActionReportRead();
			}
			else if(bType == DB_EVENT_RALM)
			{
				// ��ʼ������ָ��
				m_pNextRelayEvent = &m_tCurrentReport.tAlram.tEvent.tAction;
				m_nLeftActionReportNum = 1;

				LogError("Txd68_Report_61",FILE_LINE,"Send a new alarm report!!!");
				// ֱ�ӵ���ָ��
				DB_IncRelayAlarmReportRead();
			}
			else if(bType == DB_EVENT_RSTT)
			{
				// ��ʼ������ָ��
				m_pNextRelayEvent = &m_tCurrentReport.tStart.tEvent.tAction;
				m_nLeftActionReportNum = 1;
				
				LogError("Txd68_Report_61",FILE_LINE,"Send a new start report!!!");
				// ֱ�ӵ���ָ��
				DB_IncRelayStartReportRead();
			}
			else
			{
				LogError("Txd68_Report_61",FILE_LINE,"Search event type error!!!");
				break;
			}

		}

		// ��Ч������ֱ���޳�
		if(m_pNextRelayEvent->FD_Num > MAX_FD_NUM)
		{
			m_pNextRelayEvent = NULL;	
			m_nLeftActionReportNum = 0;
			LogError("Txd68_Report_61",FILE_LINE,"Invalid event report!!!");
			break;
		}
		
		// ����һ��103���ĳ���
		nLen = 18+m_pNextRelayEvent->FD_Num*6;

		// Խ��ʱ����
		if((m_Txd.WritePtr-4+nLen)>255)
			break;
		
		// ��䱣������
		Make103P_Report(m_tCurrentReport.tAction.tEvent.wFaultNo,m_pNextRelayEvent);
			
		// �ݼ�ʣ��ֱ������
		m_nLeftActionReportNum--;
		
		// ������һ���ݱ���ָ��
		m_pNextRelayEvent = (TRelayEventHead *)((BYTE *)(m_pNextRelayEvent+1)+m_pNextRelayEvent->FD_Num*6);
		
		// ����103�������
		pTxdFrameHead->FrameVar_68.Asdu.byVSQ++;
	}while((m_Txd.WritePtr-4)<255);
	
	ReportMsg("Send item num = %d!!!",pTxdFrameHead->FrameVar_68.Asdu.byVSQ-0x80);

	// ���ڷ��͹�һ�����ݺ󣬸������ܻᷢ���仯����Ҫ����ˢ��
	pTxdFrameHead->FrameVar_68.Control.ACD = GetACDFlag();

	// 4�����ͱ���
	if(!Txd68_Tail())
	{
		SetFlag(SF_RETRY);
		return FALSE;
	}
		
	return TRUE;
}

//***************************************************************************************
// 	��������: Txd68_Alarm_1
//	��������: ���ͱ����澯����
//  �������: ��
//  ����ֵ:   ���ͳɹ�  TRUE
//			  ����ʧ��	FALSE
//**************************************************************************************
/*
BOOL CSiec103::Txd68_Alarm_1(void)
{
	TEventBI 		tEvent;
	TSysTime		time;
	WORD			MSecond;

	DB_ReadSOE(&tEvent);
	AbsTimeTo(&tEvent.stTime, &time);

	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_TM,0x81,COT_S_BURST,DB_GetAddress(),DEV_FUN_TYPE,tEvent.wNo+1);

	MSecond = time.MSecond+time.Second*1000;

	m_Txd.Buf[m_Txd.WritePtr++] = tEvent.bStatus;
	m_Txd.Buf[m_Txd.WritePtr++] = LOLOBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++] = LOHIBYTE(MSecond);
	m_Txd.Buf[m_Txd.WritePtr++] = time.Minute;
	m_Txd.Buf[m_Txd.WritePtr++] = time.Hour;

	m_Txd.Buf[m_Txd.WritePtr++] = 0;//SIN

	if(!Txd68_Tail())
		return FALSE;
	
	DB_IncSOERead();

	return TRUE;
}
*/

//***************************************************************************************
// 	��������: Txd68_YK_Return_64
//	��������: ����ң�ط�У
//  �������: ��
//  ����ֵ:   ���ͳɹ�  TRUE
//			  ����ʧ��	FALSE
//**************************************************************************************
BOOL CSiec103::Txd68_BO_Echo_64(void)
{
	BYTE			byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF;
	BOOL			bReturn;
	WORD			switchno;

	//��ȷ��֡����
	Txd68_Head(FC_S_ACK);

	byTYP = TYP_S_YKRTN;				//ң�ط�У
	byVSQ = 0x81;    
	if(m_BO.RetResult == 0)
		byCOT=COT_S_ACK;	//���سɹ�
	else			
		byCOT=COT_S_NAK;	//����ʧ��

	byADDRESS = pTxdFrameHead->FrameVar_68.byAddress;

	switchno = m_BO.No/2;
	
	byFUN = (switchno/28)+1;	
	byINF = (switchno%28)+48;				//ȡң��·��

	SetAsduHead(byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF);

	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byDCC;
	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byRII;

	bReturn = Txd68_Tail();
	
	return bReturn;
}	

//***************************************************************************************
// 	��������: Txd68_DD_36
//	��������: ���͵��֡
//  �������: ��
//  ����ֵ:   ���ͳɹ�  TRUE
//			  ����ʧ��	FALSE
//**************************************************************************************
BOOL CSiec103::Txd68_DD_36(void)
{
	BYTE			byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF;
	BYTE			i;
	DWORD			Counter;
	BOOL			bReturn;

	Txd68_Head(FC_S_DATA);

	byTYP = TYP_S_DD;				//ASDU-36
	byVSQ = m_LocalDDNum;    
	byCOT = COT_S_CYCLE;			//ѭ��
	byADDRESS = pTxdFrameHead->FrameVar_68.byAddress;
	byFUN = 1;						//����װ��
	byINF = 0;						//�ٻ��������������

	SetAsduHead(byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF);
	
	for(i=0;i<m_LocalDDNum;i++)
	{
		DB_ReadCI(i,&Counter);
		
		m_Txd.Buf[m_Txd.WritePtr++] = LOLOBYTE(Counter);
		m_Txd.Buf[m_Txd.WritePtr++] = LOHIBYTE(Counter);
		m_Txd.Buf[m_Txd.WritePtr++] = HILOBYTE(Counter);
		m_Txd.Buf[m_Txd.WritePtr++] = HIHIBYTE(Counter);
		m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byCallDDSQ;
	}

	bReturn = Txd68_Tail();

	return bReturn;
}

BOOL CSiec103::Txd_Indication_5(BYTE Reason)
{
	BYTE			byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF;
	BOOL			bReturn;

	Txd68_Head(FC_S_DATA);

	byTYP = TYP_S_IDENT;				//ASDU-36
	byVSQ = 0x81;    
	byCOT = Reason;			
	byADDRESS = pTxdFrameHead->FrameVar_68.byAddress;
	byFUN = 255;
	switch(Reason)
	{
	case COT_S_RESETCU:
		byINF = 3;
		break;
	case COT_S_START:
		byINF = 4;
		break;
	default:
		byINF = 0;
		break;
	}
	SetAsduHead(byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF);

	m_Txd.Buf[m_Txd.WritePtr++] = COL_GEN;//���ݼ���
	m_Txd.Buf[m_Txd.WritePtr++] = 'K';
	m_Txd.Buf[m_Txd.WritePtr++] = 'E';
	m_Txd.Buf[m_Txd.WritePtr++] = 'Y';
	m_Txd.Buf[m_Txd.WritePtr++] = 'V';
	m_Txd.Buf[m_Txd.WritePtr++] = 'A';
	m_Txd.Buf[m_Txd.WritePtr++] = '-';
	m_Txd.Buf[m_Txd.WritePtr++] = 'K';
	m_Txd.Buf[m_Txd.WritePtr++] = 'F';
	m_Txd.Buf[m_Txd.WritePtr++] = '6';
	m_Txd.Buf[m_Txd.WritePtr++] = '5';
	m_Txd.Buf[m_Txd.WritePtr++] = '0';
	m_Txd.Buf[m_Txd.WritePtr++] = '0';

	bReturn = Txd68_Tail();

	return bReturn;

}

BOOL CSiec103::Txd_SST_6(void)
{
	BYTE byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF;

	Txd68_Head(FC_S_DATA);

	byTYP=TYP_S_SST;
	byVSQ=0x81;
	byCOT=COT_S_SST;
	byADDRESS=pTxdFrameHead->FrameVar_68.byAddress;
	byFUN=FUN_GLB;
	byINF=0;

	SetAsduHead(byTYP,byVSQ,byCOT,byADDRESS,byFUN,byINF);


	m_Txd.Buf[m_Txd.WritePtr++]=Time[0];
	m_Txd.Buf[m_Txd.WritePtr++]=Time[1];
	m_Txd.Buf[m_Txd.WritePtr++]=Time[2];
	m_Txd.Buf[m_Txd.WritePtr++]=Time[3];
	m_Txd.Buf[m_Txd.WritePtr++]=Time[4];
	m_Txd.Buf[m_Txd.WritePtr++]=Time[5];
	m_Txd.Buf[m_Txd.WritePtr++]=Time[6];

	Txd68_Tail();

	return TRUE;
}

/*
0000H	��ı��⣺�̵籣���豸��ϵͳ��Ϣ
0001H	��ȡ����������
0002H	��ȡϵͳ����״̬
0003H	��ֵ��������
0004H	��ֵ�����Ʊ�
	
0100H	��ı��⣺�豸��ֵ��
0101H	1����ֵ��
0102H	2����ֵ��
010nH	N����ֵ��
01FFH   ��ǰ��ֵ��
	
2000H	��ı��⣺��������
2001H	��ֵ�����л�����
2002H	����������ֵ���޸�����
2003H	Զ��������Ͷ��
*/

//*******************************************************************************************//
//  ��������: Txd68_STDataRtn_10
//  ��������: ��������ֵ����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_STDataReadRtn_10(BYTE Group,BYTE Entry)
{
	TGenData		*pTxdGenData;
	int 			i,Pos;

	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;
	pTxdGenData = (TGenData	*)&pTxdFrameHead->FrameVar_68.Asdu.byGenData;
		
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_GDATA,0x81,m_ST.Result,DB_GetAddress(),FUN_GEN,INF_S_GEN_RENTRYVALUE);
	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byRII;
	
	// ��ȡ��ֵ
	if(m_ST.Result == COT_S_GRACK)
	{
		Pos = m_Txd.WritePtr;
		
		pTxdGenData->Ngd.Count = m_DevInfo.byLastCnt;
		pTxdGenData->Ngd.Number= 1;
		pTxdGenData->Gin.byGroup = Group;
		pTxdGenData->Gin.byEntry = Entry;
		pTxdGenData->byKod = 1;//ʵ��ֵ
		
		// GDD			
		// ��������������
		pTxdGenData->Gdd.byDataType = DAT_7;//������
		pTxdGenData->Gdd.byDataSize = 4;//DATASIZE
		
		// �����䶨ֵ��
		for(i=0;m_ST.Offset<m_ST.Num;i++)
		{
			if((Pos+7+i*4+7) > (MAX_TXDFRAME_SIZE-2))
				break;

			// GID			
			m_Txd.Buf[Pos+7+i*4] = m_ST.Buf[m_ST.Offset*4+3];
			m_Txd.Buf[Pos+7+i*4+1] = m_ST.Buf[m_ST.Offset*4+2];
			m_Txd.Buf[Pos+7+i*4+2] = m_ST.Buf[m_ST.Offset*4+1];
			m_Txd.Buf[Pos+7+i*4+3] = m_ST.Buf[m_ST.Offset*4+0];
			
			m_ST.Offset++;
		}

		// ��ֵ����
		pTxdGenData->Gdd.byDataNum = i;
		
		if(m_ST.Offset<m_ST.Num)
		{
			pTxdGenData->Ngd.Cont = 1;
			pTxdGenData->Gdd.byDataNum |= 0x80;
		}
		else
		{
			// �������
			SetFlag(SF_GEN_READSTDATA,FLAG_OFF);
			SetFlag(SF_GEN_READSYSSTDATA,FLAG_OFF);
			m_ST.Offset = 0;
			pTxdGenData->Ngd.Cont = 0;
		}
		
		m_Txd.WritePtr += 7+i*4;

		// ��ת
		m_DevInfo.byLastCnt = (m_DevInfo.byLastCnt+1)%2;
		
	}
	else
	{
		SetFlag(SF_GEN_READSTDATA,FLAG_OFF);
		SetFlag(SF_GEN_READSYSSTDATA,FLAG_OFF);
		// ������0
		m_Txd.Buf[m_Txd.WritePtr++] = 0;
	}

	// ���¼��ACD��־
	pTxdFrameHead->FrameVar_68.Control.ACD = GetACDFlag();
	
	return Txd68_Tail();
}

//*******************************************************************************************//
//  ��������: Txd68_STAreaRtn_10
//  ��������: ����ǰ��ֵ���ŷ���
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_STAreaReadRtn_10()
{
	TGenData		*pTxdGenData;

	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;
	pTxdGenData = (TGenData	*)&pTxdFrameHead->FrameVar_68.Asdu.byGenData;
		
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_GDATA,0x81,m_ST.Result,DB_GetAddress(),FUN_GEN,INF_S_GEN_RENTRYVALUE);
	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byRII;

	if(m_ST.Result == COT_S_GRACK)
	{
		pTxdGenData->Ngd.Count = 0;
		pTxdGenData->Ngd.Cont = 0;
		pTxdGenData->Ngd.Number= 1;
		
		pTxdGenData->Gin.byGroup = 02;
		pTxdGenData->Gin.byEntry = 01;

		pTxdGenData->byKod = 1;//ʵ��ֵ
		
		pTxdGenData->Gdd.byDataType = DAT_3;//����
		pTxdGenData->Gdd.byDataSize = 1;//DATASIZE
		pTxdGenData->Gdd.byDataNum = 1;

		pTxdGenData->byGid = m_ST.Group;

		m_Txd.WritePtr += sizeof(TGenData);
	}
	else
		// ������0
		m_Txd.Buf[m_Txd.WritePtr++] = 0;
			
	return Txd68_Tail();		
}

//*******************************************************************************************//
//  ��������: Txd68_SBOReadRtn_10
//  ��������: ����ѹ��״̬����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_SBOReadRtn_10()
{
	TGenData		*pTxdGenData;

	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;
	pTxdGenData = (TGenData	*)&pTxdFrameHead->FrameVar_68.Asdu.byGenData;
		
	Txd68_Head(FC_S_DATA);

	SetAsduHead(TYP_S_GDATA,0x81,m_ST.Result,DB_GetAddress(),FUN_GEN,INF_S_GEN_RENTRYVALUE);
	m_Txd.Buf[m_Txd.WritePtr++] = m_DevInfo.byRII;

	if(m_ST.Result == COT_S_GRACK)
	{
		pTxdGenData->Ngd.Count = 0;
		pTxdGenData->Ngd.Cont = 0;
		pTxdGenData->Ngd.Number= 1;

		if(m_ST.No == 0xFF)
		{
			pTxdGenData->Gin.byGroup = 04;
			pTxdGenData->Gin.byEntry = 0xFF;
			pTxdGenData->byKod = 1;//ʵ��ֵ
			pTxdGenData->Gdd.byDataType = DAT_2;//λ��
			pTxdGenData->Gdd.byDataSize = 1;//DATASIZE
			pTxdGenData->Gdd.byDataNum = m_ST.Num;
			memcpy(&pTxdGenData->byGid,m_ST.Buf,(m_ST.Num+7)/8);
			m_Txd.WritePtr += sizeof(TGenData)-1+(m_ST.Num+7)/8;
		}
		else
		{
			pTxdGenData->Gin.byGroup = 04;
			pTxdGenData->Gin.byEntry = m_ST.No+1;
			pTxdGenData->byKod = 1;//ʵ��ֵ
			pTxdGenData->Gdd.byDataType = DAT_9;//˫����Ϣ
			pTxdGenData->Gdd.byDataSize = 1;//DATASIZE
			pTxdGenData->Gdd.byDataNum = 1;
			pTxdGenData->byGid = m_ST.Buf[0];
			m_Txd.WritePtr += sizeof(TGenData);
		}
		
	}
	else
		// ������0
		m_Txd.Buf[m_Txd.WritePtr++] = 0;
			
	return Txd68_Tail();		
}


//*******************************************************************************************//
//  ��������: Txd68_GenWriteWithConfirm
//  ��������: ��ȷ�ϵ�д��Ŀ����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_GenWriteWithConfirm()
{
	pRxdFrameHead = (T_M_FrameHead *)m_pLast68Frame;
	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;

	// 1.���ƽ���֡
	memcpy(pTxdFrameHead,pRxdFrameHead,pRxdFrameHead->FrameVar_68.byLength1+6);

	// 2.������д������
	pTxdFrameHead->FrameVar_68.Control.RES = 0;
	pTxdFrameHead->FrameVar_68.Control.PRM = PRM_SLAVE;
	pTxdFrameHead->FrameVar_68.Control.ACD = GetACDFlag();
	pTxdFrameHead->FrameVar_68.Control.DFC = 0;
	pTxdFrameHead->FrameVar_68.Control.FC = FC_S_DATA;

	// 3.������дԭ��
	pTxdFrameHead->FrameVar_68.Asdu.byCOT=m_ST.Result;

	// 4.������У�鷢��
	m_Txd.WritePtr = pRxdFrameHead->FrameVar_68.byLength1+4;
	
	return Txd68_Tail();		
}

//*******************************************************************************************//
//  ��������: Txd68_GenWriteWithExucute
//  ��������: ��ִ�е�д��Ŀ����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Txd68_GenWriteWithExucute()
{
	TGenData		*pTxdGenData;
	
	pRxdFrameHead = (T_M_FrameHead *)m_pLast68Frame;
	pTxdFrameHead = (T_S_FrameHead *)m_Txd.Buf;

	pTxdGenData = (TGenData	*)&pTxdFrameHead->FrameVar_68.Asdu.byGenData;

	// 1.���ƽ���֡
	memcpy(pTxdFrameHead,pRxdFrameHead,pRxdFrameHead->FrameVar_68.byLength1+6);

	// 2.������д������
	pTxdFrameHead->FrameVar_68.Control.RES = 0;
	pTxdFrameHead->FrameVar_68.Control.PRM = PRM_SLAVE;
	pTxdFrameHead->FrameVar_68.Control.ACD = GetACDFlag();
	pTxdFrameHead->FrameVar_68.Control.DFC = 0;
	pTxdFrameHead->FrameVar_68.Control.FC = FC_S_DATA;

	// 3.������дԭ��
	pTxdFrameHead->FrameVar_68.Asdu.byCOT=m_ST.Result;

	// 4.��дͨ�÷���ش���
	pTxdGenData->Ngd.Number = 1;
	pTxdGenData->Ngd.Cont = 0;
	pTxdGenData->Ngd.Count = 0;

	do
	{
		// ��ѹ�����
		if(m_ST.Command&CMD_SBO_SELECT)
		{
			pTxdGenData->Gin.byGroup = 4;
			pTxdGenData->Gin.byEntry = m_ST.No+1;
			m_ST.Command &= ~CMD_SBO_SELECT;
			break;	
		}

		// ��ֵ����
		if(m_ST.Command&CMD_ST_SELECT)
		{
			if(m_ST.No == SET_VALUE_ALL)
			{
				pTxdGenData->Gin.byGroup = 1;
				pTxdGenData->Gin.byEntry = m_ST.Group+1;
			}
			else
			{
				pTxdGenData->Gin.byGroup = 2;
				pTxdGenData->Gin.byEntry = 2;
			}

			m_ST.Command &= ~CMD_ST_SELECT;

			break;
		}

		// ��ֵ������
		if(m_ST.Command&CMD_STS_SELECT)
		{
			pTxdGenData->Gin.byGroup = 2;
			pTxdGenData->Gin.byEntry = 1;
			m_ST.Command &= ~CMD_STS_SELECT;
			break;	
		}

		// ������ֵ����
		if(m_ST.Command&CMD_SYSST_SELECT)
		{
			pTxdGenData->Gin.byGroup = 5;
			pTxdGenData->Gin.byEntry = m_ST.No+1;
			m_ST.Command &= ~CMD_SYSST_SELECT;
			break;	
		}
	}while(0);
		
	pTxdGenData->byKod = 0;
	
	pTxdGenData->Gdd.byDataType = DAT_22;//����
	pTxdGenData->Gdd.byDataSize = 1;//DATASIZE
	pTxdGenData->Gdd.byDataNum = 1;

	if(m_ST.Result == COT_S_GWACK)
		pTxdGenData->byGid= m_DevInfo.byGRC;
	else
		pTxdGenData->byGid = 12;
		
	// 4.������У�鷢��
	m_Txd.WritePtr = sizeof(T_M_FrameVar)+sizeof(TGenData)-1;
	
	return Txd68_Tail();		
}

//*******************************************************************************************//
//  ��������: Txd68_STDataWriteRtn_10
//  ��������: д������ֵ����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
//BOOL CSiec103::Txd68_STDataWriteAllRtn_10()
//{
//}

//*******************************************************************************************//
//  ��������: Txd68_STDataWriteRtn_10
//  ��������: д������ֵ����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
//BOOL CSiec103::Txd68_STDataWriteOneRtn_10()
//{
//}
//

//*******************************************************************************************//
//  ��������: Txd68_STAreaWriteRtn_10
//  ��������: Ͷ��ֵ����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
//BOOL CSiec103::Txd68_STAreaWriteRtn_10()
//{
//}

//*******************************************************************************************//
//  ��������: Txd68_SBOWriteRtn_10
//  ��������: Ͷ��ѹ��
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�; FALSE,ʧ��
//*******************************************************************************************//
//BOOL CSiec103::Txd68_SBOWriteRtn_10()
//{
//}

//*******************************************************************************************//
//  ��������: Rxd10_ResetCU
//  ��������: ��λͨ�ŵ�Ԫ֡����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Rxd10_ResetCU(void)
{
	BOOL bReturn;
	SetFlag(SF_RESETCU);
	bReturn=Txd10_ACK();

	return bReturn;
}
//*******************************************************************************************//
//  ��������: Rxd10_ResetFCB
//  ��������: ��λ֡����֡����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Rxd10_ResetFCB(void)
{
   BOOL bReturn;

   m_DevInfo.byOldFCB = 0;
	SetFlag(SF_RESETFCB);
   bReturn = Txd10_ACK();

   return bReturn;
}
//*******************************************************************************************//
//  ��������: Rxd10_Call_Link
//  ��������: �ٻ���·״̬֡����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Rxd10_Call_Link(void)
{
   BOOL bReturn;
   bReturn = Txd10_LinkOk_11();

   return bReturn;
}

//*******************************************************************************************//
//  ��������: Rxd10_Call_Class1
//  ��������: �ٻ�һ������֡����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Rxd10_Call_Class1(void)
{
   BOOL bReturn;
   bReturn = TxdClass1Data();

   return bReturn;
}

//*******************************************************************************************//
//  ��������: Rxd10_Call_Class2
//  ��������: �ٻ���������֡����
//  �������: ��
//  ����ֵ:   TRUE,�ɹ�;
//            FALSE,ʧ��
//*******************************************************************************************//
BOOL CSiec103::Rxd10_Call_Class2(void)
{
   BOOL bReturn;
   bReturn = TxdClass2Data(COT_S_CYCLE);

   return bReturn;
}

//*******************************************************************************************//
//  ��������: Rxd68_SST_6
//  ��������: ��ʱ֡����
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_SST_6(void)
{
    TSysTime		SysTime;	
    BYTE			*Data;
	WORD			MSecond,Second,i;

    Data = &(pRxdFrameHead->FrameVar_68.Asdu.byData);

	for(i=0;i<7;i++)
		Time[i] = Data[i];

	MSecond = (Data[0]+Data[1]*256)%1000;
	Second = (Data[0]+Data[1]*256)/1000;

	SysTime.MSecond = MSecond;
	SysTime.Second	= Second;
	SysTime.Minute	= Data[2]&0x3f;
	SysTime.Hour	= Data[3]&0x1f;
	SysTime.Day		= Data[4]&0x1f;
	SysTime.Month	= Data[5]&0x0f;
	SysTime.Year	= (Data[6]%100)+2000;

	if(DAE_GetRunningFlags(SS_TIME_B))
		WriteSystemTime(&SysTime);

	if(pRxdFrameHead->FrameVar_68.byAddress == 255)
		return TRUE;

	SetFlag(SF_SST_ACK);

	return Txd10_ACK();
}
//*******************************************************************************************//
//  ��������: Rxd68_CallAll_7
//  ��������: ���ٻ�����֡����
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_CallAll_7(void)
{
	BOOL bReturn;
	BYTE *Data;

	Data = &(pRxdFrameHead->FrameVar_68.Asdu.byData);

	//ȡɨ����
	m_DevInfo.byScanNumber	= Data[0];

	SetFlag(SF_CALLALL);

	if(!GetMaintSWStatus())
	{
		SetFlag(SF_CALLALL_AI);
		SetFlag(SF_CALLALL_BI);
	}
	
	bReturn = Txd10_ACK();//ȷ��
	
	return bReturn;
}
//*******************************************************************************************//
//  ��������: Rxd68_Command_20
//  ��������: һ������֡����()
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_Command_20(void)
{
	switch(pRxdFrameHead->FrameVar_68.Asdu.byData)
	{
		case DCO_M_TESTMOD:  // �������״̬
		case DCO_M_RUNMOD:   // ��������״̬
		case DCO_M_DEVREVERT:// װ�ø���
			return Txd10_ACK();
		case DCO_M_CALLYX:   // �ٻ�ң��
			return Txd68_BI_44(COT_S_REMOTE);
		default:
			break;
	}
	
	return Txd10_ACK(); 		
}

//*******************************************************************************************//
//  ��������: Rxd68_GComm_21
//  ��������:  ����ͨ����������
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_GComm_21(void)
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;

	m_DevInfo.byRII = pAsdu->byData;

	// ��֧�ֶ�������Ŀ��ֵ������
	//if(pAsdu->byINF != INF_M_GEN_RENTRYVALUE)
	//	return FALSE;

	// ����ͨ�÷�����
	m_DevInfo.byLastCnt = 0;
		
	// ����GIN���ֶ�ֵ���Ƕ�ֵ��
	switch(pGenData->Gin.byGroup)
	{
		case 1://��ȡ������ֵ
			Rxd68_STDataRead_21();
			break;

		case 2://��ȡ��ֵ����
			Rxd68_STAreaRead_21();
			break;

		case 3://��ȡ��ѹ��
		case 4://��ȡ��ѹ��
			Rxd68_SBORead_21();
			break;

		case 5://��ȡ������ֵ
			Rxd68_SYSSTDataRead_21();
			break;
			
		default:
			return FALSE;
	}

	// �ظ�ȷ��
	Txd10_ACK();
	
	return TRUE;
}

BOOL CSiec103::Rxd68_STAreaRead_21()
{
	m_ST.Group = DB_GetCurrentSETAreaNo();
	m_ST.Result = COT_S_GRACK;

	SetFlag(SF_GEN_READSTAREA);
	
	return TRUE;
}

BOOL CSiec103::Rxd68_STDataRead_21()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	STATUS	 rc;

	// 0xFF����ǰ��ֵ��
	if(pGenData->Gin.byEntry == 0xFF)
		m_ST.Group = DB_GetCurrentSETAreaNo();
	else
		m_ST.Group = pGenData->Gin.byEntry-1;
		
	m_ST.No = SET_VALUE_ALL;
	m_ST.Command |= CMD_ST_READ;
	m_ST.Offset = 0;
	m_ST.Num = m_LocalSTNum;
	
	rc = DB_GetSETFloatValue(m_ST.Group, m_ST.No,(float *)m_ST.Buf);
	if(rc != DB_ERR_OK)
		m_ST.Result = COT_S_GRNAK;
	else
	{
		// ���������
		SplitConfigWord((float *)m_ST.Buf);
		m_ST.Result = COT_S_GRACK;
	}
	
	SetFlag(SF_GEN_READSTDATA);
	
	return TRUE;
}

BOOL CSiec103::Rxd68_SYSSTDataRead_21()
{
	STATUS	 rc;

	// 0xFF����ǰ��ֵ��
	m_ST.No = SET_VALUE_ALL;
	m_ST.Command |= CMD_ST_READ;
	m_ST.Offset = 0;
	m_ST.Num = DB_GetSystemSETNum();
	
	rc = DB_GetSystemSETFloatValue(m_ST.No,(float *)m_ST.Buf);
	if(rc != DB_ERR_OK)
		m_ST.Result = COT_S_GRNAK;
	else
		m_ST.Result = COT_S_GRACK;
	
	SetFlag(SF_GEN_READSYSSTDATA);
	
	return TRUE;
}

BOOL CSiec103::Rxd68_SBORead_21()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	WORD	num;
	BYTE	status;
	STATUS	 rc=DB_ERR_OK;

	num = DB_GetSWNum();
	m_ST.Offset = 0;
	m_ST.Num = DB_GetSWNum();
	m_ST.No = pGenData->Gin.byEntry;

	// 0xFF����ȫ����ѹ��
	if(pGenData->Gin.byEntry == 0xFF)
	{		
		// ��ȡȫ����ѹ��״̬
		memset(m_ST.Buf,0,(num+7)/8);
		for(int i=0;i<num;i++)
		{
			rc |= DB_GetSWStatus(i, status);

			if(status == SW_STATUS_SET)
				m_ST.Buf[i/8] |= (BYTE)1<<(i%8);
			else
				m_ST.Buf[i/8] &= ~((BYTE)1<<(i%8));
		}
	}
	else
	{
		// ��ȡ������ѹ��״̬
		rc = DB_GetSWStatus(pGenData->Gin.byEntry-1, m_ST.Buf[0]);
	}
	
	if(rc != DB_ERR_OK)
		m_ST.Result = COT_S_GRNAK;
	else
		m_ST.Result = COT_S_GRACK;

	SetFlag(SF_GEN_READSBO);

	return TRUE;
}


//*******************************************************************************************//
//  ��������: Rxd68_GData_10
//  ��������:  ����ͨ����������
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_GData_10(void)
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	STATUS	 rc=DB_ERR_OK;
	
	m_DevInfo.byGRC = 0;
	
	// ��֧�ֶ�������Ŀ��ֵ������

	// ִ�в���
	if(pAsdu->byINF == INF_M_GEN_WENTRYEXEC)
	{
		do
		{
			// Ԥ�ù���ѹ��
			if(m_ST.Command&CMD_SBO_SELECT)
			{
				if(m_SBONo == SET_VALUE_ALL)
					rc = DB_SetAllSWStatusConfirm();
				else
				{
					rc = DB_SetSWStatusConfirm(m_SBONo);
					ReportMsg("Siec103 recv write one SW Exute! No=%d Rst=0x%x",m_SBONo,rc);
				}
				break;
			}
			
			// Ԥ�ù���ֵ
			if(m_ST.Command&CMD_ST_SELECT)
			{
				rc = DB_SetSETValueConfirm(m_ST.Group, m_ST.No);
				break;
			}
			
			// Ԥ�ù���ֵ��
			if(m_ST.Command&CMD_STS_SELECT)
			{
				rc = DB_SetCurrentSETAreaNoConfirm(m_ST.Group);
				break;
			}
			
			// Ԥ�ù�������ֵ
			if(m_ST.Command&CMD_SYSST_SELECT)
			{
				rc = DB_SetSystemSETValueConfirm(m_ST.No);
				break;
			}
		}while(0);
		
		// ���϶�δԤ�ù�����ʧ��
		if(!(m_ST.Command&(CMD_ST_SELECT|CMD_STS_SELECT|CMD_SYSST_SELECT|CMD_SBO_SELECT)))
		{
			m_DevInfo.byGRC = 2;
			rc = DB_ERR_FAILURE;
		}	
			
		if(rc == DB_ERR_OK)
			m_ST.Result = COT_S_GWACK;
		else
			m_ST.Result = COT_S_GWNAK;

		SetFlag(SF_GEN_WRITEWITHEXECUTE);
			
		// �ظ�ȷ��
		Txd10_ACK();
		
		return TRUE;
	}

	// ��������
	if(pAsdu->byINF == INF_M_GEN_WENTRYCANCEL)
	{
		// Ԥ�ù���ѹ��
		do
		{
			if(m_ST.Command&CMD_SBO_SELECT)
			{
				if(m_SBONo == SET_VALUE_ALL)				//lz 20140307
					rc = DB_SetAllSWStatusConfirm(DB_CANCEL);
				else
					rc = DB_SetSWStatusConfirm(m_SBONo,DB_CANCEL);////lz 20140307
				break;
			}
			
			// Ԥ�ù���ֵ
			if(m_ST.Command&CMD_ST_SELECT)
			{
				rc = DB_SetSETValueConfirm(m_ST.Group, m_ST.No,DB_CANCEL);
				break;
			}
			
			// Ԥ�ù���ֵ��        ����λ�õ�����yanzhh 2014��4��14�� 11:07:48
			if(m_ST.Command&CMD_STS_SELECT)				
			{
				rc = DB_SetCurrentSETAreaNoConfirm(m_ST.Group,DB_CANCEL);
				break;
			}

			// Ԥ�ù�������ֵ
			if(m_ST.Command&CMD_SYSST_SELECT)
			{
				rc = DB_SetSystemSETValueConfirm(m_ST.No,DB_CANCEL);
				break;
			}
		}while(0);

		// ���϶�δԤ�ù�����ʧ��
		if(!(m_ST.Command&(CMD_ST_SELECT|CMD_STS_SELECT|CMD_SYSST_SELECT|CMD_SBO_SELECT)))
		{
			m_DevInfo.byGRC = 2;
			rc = DB_ERR_FAILURE;
		}	

		if(rc == DB_ERR_OK)
			m_ST.Result = COT_S_GWACK;
		else
			m_ST.Result = COT_S_GWNAK;

		SetFlag(SF_GEN_WRITEWITHCANCEL);
			
		// �ظ�ȷ��
		Txd10_ACK();
		
		return TRUE;
	}
		
	// ����GIN���ֶ�ֵ���Ƕ�ֵ��
	switch(pGenData->Gin.byGroup)
	{
		case 1://д������ֵ
			Rxd68_STDataWriteAll_10();
			break;
			
		case 2:
			//Ͷ��ֵ����
			if(pGenData->Gin.byEntry == 1)
				Rxd68_STAreaWrite_10();
			//д����������ֵ
			else if(pGenData->Gin.byEntry == 2)
				Rxd68_STDataWriteOne_10();
			break;
			
		case 3:
		case 4:
			//д��ѹ��
			Rxd68_SBOWrite_10();
			break;

		case 5:
			//������ֵ
			Rxd68_SysSTDataWrite_10();
			break;
			
		default:
			m_DevInfo.byGRC = 1;
			return FALSE;
	}

	// �ظ�ȷ��
	Txd10_ACK();

	return TRUE;
}

BOOL CSiec103::Rxd68_STDataWriteAll_10()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	BYTE 	 *pData = &pGenData->byGid;
	BYTE 	 *pValue;
	BYTE	 byDataNum;
	STATUS	 rc;
	
	// 0xFF����ǰ��ֵ��
	if(pGenData->Gin.byEntry == 0xFF)
		m_ST.Group = DB_GetCurrentSETAreaNo();
	else
		m_ST.Group = pGenData->Gin.byEntry-1;
	
	m_ST.No = SET_VALUE_ALL;
	m_ST.Command |= CMD_ST_SELECT;

	// ȡ��ֵ����
	byDataNum = pGenData->Gdd.byDataNum&0x7F;

	ReportMsg("Siec103 recv write all SET command! Area=%d Off=%d Num=%d",m_ST.Group,m_ST.Offset,byDataNum);
	
	// ��ֵ����Խ��ֱ�ӷ��ش���
	do
	{
		if( (m_ST.Offset+byDataNum) > m_LocalSTNum)
		{
			m_ST.Offset = 0;
			m_ST.Result = COT_S_GWNAK;
			break;
		}	
		
		// ������ֵ������ת��
		pValue = m_ST.Buf+m_ST.Offset*4;
		for(int i=0;i<byDataNum;i++)
		{
			pValue[i*4+0] = pData[i*4+3];
			pValue[i*4+1] = pData[i*4+2];
			pValue[i*4+2] = pData[i*4+1];
			pValue[i*4+3] = pData[i*4+0];
		}
		
		m_ST.Offset += byDataNum;

		// ���һ֡д��Ԥ��
		if((!pGenData->Ngd.Cont)&&!(pGenData->Gdd.byDataNum&0x80))
		//if(m_ST.Offset == m_LocalSTNum)
		{
			// ��������ֱ�ӷ���
			if(m_ST.Offset != m_LocalSTNum)
			{
				m_ST.Offset = 0;
				m_ST.Result = COT_S_GWNAK;
				break;
			}	

			// Ԥ�ö�ֵ

			// �ϲ�������
			MergeConfigWord((float *)m_ST.Buf);
			
			rc = DB_SetSETValue(m_ST.Group,SET_VALUE_ALL,(float *)m_ST.Buf);
			if(rc != DB_ERR_OK)
			{
				m_ST.Offset = 0;
				m_ST.Result = COT_S_GWNAK;
				break;
			}
			
			m_ST.Offset = 0;
		}
		
		m_ST.Num = m_LocalSTNum;
		m_ST.Result = COT_S_GWANS;
	}while(0);

	SetFlag(SF_GEN_WRITEWITHCONFIRM);

	return TRUE;
}

BOOL CSiec103::Rxd68_STDataWriteOne_10()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	TGdd	 *pGdd = &pGenData->Gdd;
	TGin	 *pGin;
	STATUS	 rc;
	BYTE	 cfgdwordnum,cfgwordnum;
	DWORD	 dwValue;
	WORD	 wCfgWord;
	float	 fValue;

	m_ST.Command |= CMD_ST_SELECT;

	// ��ֵ����Խ��ֱ�ӷ��ش���
	do
	{		
		pGdd += 1;

		// Ŀǰ��ʱ��֧�ֵ�����ֵ�޸�
		if(pGdd->byDataNum != 2)
		{
			m_ST.Result = COT_S_GWNAK;
			break;
		}

		// ��������
		pGin = (TGin *)(pGdd+1);		
		m_ST.Num = 1;
		m_ST.Group = pGin->byGroup;
		m_ST.No = pGin->byEntry;
		memcpy(m_ST.Buf,(BYTE *)(pGin+1)+sizeof(TGdd),sizeof(float));
		
		cfgwordnum = (DB_GetCFGNum()+15)/16;
		cfgdwordnum = (DB_GetCFGNum()+31)/32;

		// ȡ����ֵ
		dwValue = MAKEDWORD(m_ST.Buf[0],m_ST.Buf[1],m_ST.Buf[2],m_ST.Buf[3]);
		fValue =  *(float *)&dwValue;
		
		// �޸���������Ҫ�ϲ�
		if( m_ST.No < cfgwordnum)
		{
			wCfgWord = fValue;
			
			// �ȶ���DWORD������
			DB_GetSETFloatValue(m_ST.Group, m_ST.No/2, (float *)&dwValue);
			
			// ����ӦWORD�û����ϲ�ΪDWORD
			if(m_ST.No % 2)
			{
				dwValue &= 0xFFFF;
				dwValue |= wCfgWord<<16;
			}
			else
			{
				dwValue &= 0xFFFF0000;
				dwValue |= wCfgWord;
			}

			m_ST.No /= 2;
		}
		else
		{
			// ������ֵ��������
			m_ST.No -= (cfgwordnum-cfgdwordnum);
			
		}
		
		// ˢ�¶�ֵ
		memcpy(m_ST.Buf,&dwValue,4);

		// ���ö�ֵ
		rc = DB_SetSETValue(m_ST.Group,m_ST.No,(float *)m_ST.Buf);
		
		if(rc != DB_ERR_OK)
		{
			m_ST.Result = COT_S_GWNAK;
			break;
		}
		
		m_ST.Result = COT_S_GWANS;
	}while(0);

	SetFlag(SF_GEN_WRITEWITHCONFIRM);

	return TRUE;
}

BOOL CSiec103::Rxd68_STAreaWrite_10()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	STATUS	 rc;
	
	m_ST.Command |= CMD_STS_SELECT;
	m_ST.Group = pGenData->byGid;

	// Ԥ�ö�ֵ����
	rc = DB_SetCurrentSETAreaNo(m_ST.Group);
	if(rc != DB_ERR_OK)
		m_ST.Result = COT_S_GWNAK;
	else
		m_ST.Result = COT_S_GWANS;
	
	SetFlag(SF_GEN_WRITEWITHCONFIRM);

	return TRUE;
}

BOOL CSiec103::Rxd68_SBOWrite_10()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	BYTE 	 *pData = &pGenData->byGid;
	BYTE	 byDataNum;
	STATUS	 rc;

	m_ST.Command |= CMD_SBO_SELECT;
	
	// ȡ��ֵ����
	byDataNum = pGenData->Gdd.byDataNum&0x7F;

	// 0xFF����ȫ����ѹ��
	if(pGenData->Gin.byEntry == 0xFF)
	{
		m_SBONo = SET_VALUE_ALL;
		m_ST.Num = DB_GetSWNum();
		
		do
		{
			if( (m_ST.Offset+byDataNum) > m_ST.Num)
			{
				m_ST.Offset = 0;
				m_ST.Result = COT_S_GWNAK;
				break;
			}	
			
			// ������ֵ
			memcpy(m_ST.Buf+m_ST.Offset,pData,(byDataNum+7)/8);
			
			m_ST.Offset += byDataNum;

			// ���һ֡д��Ԥ��
			if((!pGenData->Ngd.Cont)&&!(pGenData->Gdd.byDataNum&0x80))
			{
				// ��������ֱ�ӷ���
				if(m_ST.Offset != m_ST.Num)
				{
					m_ST.Offset = 0;
					m_ST.Result = COT_S_GWNAK;
					break;
				}	

				// Ԥ�ö�ֵ
				rc = DB_SetAllSWStatus(m_ST.Buf);
				if(rc != DB_ERR_OK)
				{
					m_ST.Offset = 0;
					m_ST.Result = COT_S_GWNAK;
					break;
				}
				
				m_ST.Offset = 0;
			}
			
			m_ST.Result = COT_S_GWANS;
		}while(0);		
	}
	else
	{
		m_SBONo = pGenData->Gin.byEntry-1;
		
		// Ԥ����ѹ��
		rc = DB_SetSWStatus(m_SBONo,(pGenData->byGid==1)?CFG_STATUS_CLR:CFG_STATUS_SET);
		if(rc != DB_ERR_OK)
		{
			ReportMsg("Siec103 recv write one SW select Error! No=%d Value=%d",m_SBONo,pGenData->byGid);
			m_ST.Result = COT_S_GWNAK;
		}
		else
		{
			ReportMsg("Siec103 recv write one SW select OK! No=%d Value=%d",m_SBONo,pGenData->byGid);
			m_ST.Result = COT_S_GWANS;
		}
	}
		
	SetFlag(SF_GEN_WRITEWITHCONFIRM);

	return TRUE;
}

BOOL CSiec103::Rxd68_SysSTDataWrite_10()
{
	TAsdu	 *pAsdu = &pRxdFrameHead->FrameVar_68.Asdu;
	TGenData *pGenData = (TGenData *)&pAsdu->byGenData;
	BYTE 	 *pData = &pGenData->byGid;
	BYTE 	 *pValue;
	BYTE	 byDataNum;
	STATUS	 rc;
	
	m_ST.Command |= CMD_SYSST_SELECT;

	// ȡ��ֵ����
	byDataNum = pGenData->Gdd.byDataNum&0x7F;

	ReportMsg("Siec103 recv write all system SET command! Off=%d Num=%d",m_ST.Offset,byDataNum);
	
	// 0xFF����������ֵ
	if(pGenData->Gin.byEntry == 0xFF)
	{
		m_ST.No = SET_VALUE_ALL;
		m_ST.Num = DB_GetSystemSETNum();
		
		do
		{
			if( (m_ST.Offset+byDataNum) > m_ST.Num)
			{
				m_ST.Offset = 0;
				m_ST.Result = COT_S_GWNAK;
				break;
			}	
			
			// ������ֵ������ת��
			pValue = m_ST.Buf+m_ST.Offset*4;
			for(int i=0;i<byDataNum;i++)
			{
				pValue[i*4+0] = pData[i*4+3];
				pValue[i*4+1] = pData[i*4+2];
				pValue[i*4+2] = pData[i*4+1];
				pValue[i*4+3] = pData[i*4+0];
			}
			
			m_ST.Offset += byDataNum;

			// ���һ֡д��Ԥ��
			if((!pGenData->Ngd.Cont)&&!(pGenData->Gdd.byDataNum&0x80))
			{
				// ��������ֱ�ӷ���
				if(m_ST.Offset != m_ST.Num)
				{
					m_ST.Offset = 0;
					m_ST.Result = COT_S_GWNAK;
					break;
				}	

				// Ԥ�ö�ֵ
				rc = DB_SetSystemSETValue(SET_VALUE_ALL,(float *)m_ST.Buf);
				if(rc != DB_ERR_OK)
				{
					m_ST.Offset = 0;
					m_ST.Result = COT_S_GWNAK;
					break;
				}
				m_ST.Offset = 0;
			}		
			m_ST.Result = COT_S_GWANS;
		}while(0);
	}
	else
	{
		do
		{		
			m_ST.No = pGenData->Gin.byEntry-1;	
			m_ST.Num = 1;
			
			// Ŀǰ��ʱ��֧�ֵ�����ֵ�޸�
			if(pGenData->Gdd.byDataNum != 1)
			{
				m_ST.Result = COT_S_GWNAK;
				break;
			}
		
			// ��������
			m_ST.Buf[0] = pData[3];
			m_ST.Buf[1] = pData[2];
			m_ST.Buf[2] = pData[1];
			m_ST.Buf[3] = pData[0];

			// ���ö�ֵ
			rc = DB_SetSystemSETValue(m_ST.No,(float *)m_ST.Buf);
			
			if(rc != DB_ERR_OK)
			{
				m_ST.Result = COT_S_GWNAK;
				break;
			}
			
			m_ST.Result = COT_S_GWANS;
		}while(0);
	}

	SetFlag(SF_GEN_WRITEWITHCONFIRM);

	return TRUE;
}

//*******************************************************************************************//
//  ��������: Rxd68_YKCommand_64
//  ��������: ��·����������֡����()
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_YKCommand_64(void)
{
	BOOL bReturn;
	BYTE ykNo;
	BYTE *Data;

	Data = &(pRxdFrameHead->FrameVar_68.Asdu.byData);

	ykNo = (pRxdFrameHead->FrameVar_68.Asdu.byFUN-1)*28+pRxdFrameHead->FrameVar_68.Asdu.byINF-48;
	m_DevInfo.byDCC = Data[0];
	m_DevInfo.byRII = Data[1];

	//ң�ش������
	bReturn = P103_YK_Pro(ykNo,m_DevInfo.byDCC);

	return bReturn;
}
//*******************************************************************************************//
//  ��������: Rxd68_CallDD_88
//  ��������: �ٻ����֡����
//  �������: ��
//  ����ֵ:   ��;
//*******************************************************************************************//
BOOL CSiec103::Rxd68_CallDD_88(void)
{
	BOOL bReturn;
	BYTE *Data;

	Data = &(pRxdFrameHead->FrameVar_68.Asdu.byData);

	SetFlag(SF_REPORT_DD);

	//��ȷ��֡����
	Txd68_Head(FC_S_ACK);

	pTxdFrameHead->FrameVar_68.Asdu.byTYP = pRxdFrameHead->FrameVar_68.Asdu.byTYP;
	pTxdFrameHead->FrameVar_68.Asdu.byVSQ = pRxdFrameHead->FrameVar_68.Asdu.byVSQ;
	pTxdFrameHead->FrameVar_68.Asdu.byCOT = pRxdFrameHead->FrameVar_68.Asdu.byCOT;
	pTxdFrameHead->FrameVar_68.Asdu.byADDRESS = pRxdFrameHead->FrameVar_68.Asdu.byADDRESS;
	pTxdFrameHead->FrameVar_68.Asdu.byFUN = pRxdFrameHead->FrameVar_68.Asdu.byFUN;
	pTxdFrameHead->FrameVar_68.Asdu.byINF = pRxdFrameHead->FrameVar_68.Asdu.byINF;

	m_Txd.Buf[m_Txd.WritePtr++] = Data[0];
	m_Txd.Buf[m_Txd.WritePtr++] = Data[1];

	bReturn = Txd68_Tail();
	
	return bReturn;
}

#endif
