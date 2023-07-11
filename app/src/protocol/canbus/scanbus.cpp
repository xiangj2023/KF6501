#include "protocol.h"
#include "parafunc.H"
#include "scanbus.H"
#include "db_err.h"
#include "relayfun.h"
#include "logic.h"
#include "logicdef.h"
#include "relay.h"

#if(SYSTEM_TYPE == _SYS_KF1300_)	
extern WORD* wACT1Cnt;  //动作1次数
extern WORD* wACT2Cnt; 	//动作2次数
extern WORD* wACT3Cnt; 	//动作3次数
#endif

extern int SysTempture;
extern BYTE IrigQuality;
extern DWORD dwRelayDataPtr;			//AD当前写指针

TFixTransData	g_tTransData;
BYTE g_pFileDownBuffer[MAX_FILE_LEN];

/*--------------------------------------------------------------*/
/*函数名：sCanBusTask					       					*/
/*功能：CANBUS从站规约任务				       					*/
/*输入: 无						       							*/
/*输出：无						       							*/
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
    /*取端口类型*/
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
    /*取端口类型*/
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
	//初始化规约基类
	if(CProtocol::Init() != TRUE)
		return FALSE;
	
	//-----------数据库接口------------------------------	
	m_wSAddr  = DB_GetDevAddress();
	
	//初始化网络配置信息
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

	// 5秒钟一次平安报文
	/*
	m_wTimer[AIBURST_ID] 	= 0;
	m_wTimer[AI_ID] 	= 0;
	m_wTimer[BI_ID] 	= 0;
	m_wTimer[CI_ID] 	= 0;
	m_wTimer[SOE_ID] 	= 0;
	*/
	
	//初始化计数值
	m_dwTaskRunCount = 0;
	m_NoRxdCounter = CAN_COMMERR;
	LastAIPtr = 0;
	LastBIPtr = 0;
	LastCIPtr = 0;
	LastBurstAIPtr = 0;

	//下装文件偏移量记录
	m_wFileOffset = 0;
	m_CurrentResSeq = APP_RESSEQ_MIN;
	m_LastReqSeq = APP_REQSEQ_MAX+1;	

	m_FileTransTimer = 0;

	m_RetryCount = 0;

	m_WrapHeadPtr = 0;
	
	//初始化遥测纪录缓冲区
	//-----------调用数据库接口-----------------
	pOldAI = (SWORD *)OS_MemAlloc(sizeof(SWORD)*DB_GetAINum());
	OS_MemSet(pOldAI,0,sizeof(SWORD)*DB_GetAINum());
	
	m_RetryBuf = (PBYTE)OS_MemAlloc(MAX_FRAME_LEN);
	OS_MemSet(m_RetryBuf,0,MAX_FRAME_LEN);

	memset(&m_File,0,sizeof(TCanFile));

	//最多支持256个定值类数据
	m_SET.pSET = (float *)OS_MemAlloc(256*sizeof(float));
	
	//设置需网络配置标志
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

	//创建CAN基本定时器
	SetTimer(TM_1S,CAN_TIMER_TICK);

	//分配预留空间
	m_pMemBlock = OS_MemAlloc(2048);

	//分配LED状态空间
	m_pLEDStatus = (BYTE *)OS_MemAlloc(LED_RELAY_WORD_NUM);
	
	//分配LED频率空间
	m_pLEDFreq = (BYTE *)OS_MemAlloc(LED_RELAY_WORD_NUM);

	m_nWatchPortDog = 0;

	m_nMaxFrameLen = Get_MaxFrameLen();
	m_nBurstIdle = Get_CommIdle();
	m_nRetryIdle = Get_RetryIdle();
		
	return TRUE;
}

/*--------------------------------------------------------------*/
/*函数名：	CanTxdMonitor					*/
/*功能： 	CANBUS从站规约发送管理函数			*/
/*输入: 	无						*/
/*输出：	无						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::TxdMonitor()
{
	//如有请求需确认
	if(CheckClearFlag(SF_NEEDCONFIRM))
	{
		TxdConfirm();
		return;
	}
	
	//从站发送未得到确认
	if(GetFlag(SF_WAITCONFIRM))
	{
		memcpy(m_TxdInfo.pBuf,m_RetryBuf,m_RetryLen);
		WriteComm(m_RetryLen,m_RetryAddr,0x80000000);	
		ProcessRetryStatus();

		// 平安报文复位
		m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;

		return;
	}

	//操作类的优先应答
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
	
	
	//如有遥控反校
	if(GetFlag(SF_HaveBOReturn))
	{
		TxdBOReturn();
		return;
	}

	//如有COE返校，则发送
	if(CheckClearFlag(SF_MAKECOEECHO))
	{
		TxdCoeRes();
		return;
	}

	//操作类的优先应答
 	if(CheckClearFlag(SF_SENDRES_BIATTR))
 	{
		TxdReadBIAttrRes();
		return;
 	}

	//操作类的优先应答
 	if(CheckClearFlag(SF_SENDRES_BOATTR))
 	{
		TxdReadBOAttrRes();
		return;
 	}

	//操作类的优先应答
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

	//收到配置帧之前只能发空闲报文
	if(GetFlag(SF_NeedConfig))
	{
		TxdIdleRep();
		StartMsg(MCC_CommIdle,CAN_RUNIDLE);
		return;
	}
 
	//如有紧急数据
	if(HaveClass1Data())
	{			
		//如有变位遥信，则发送
		if(GetFlag(SF_BURSTBI))
		{
			TxdBurstBI();
			return;
		}
		
		//如有SOE，则发送
		if(GetFlag(SF_SOE))
		{
			TxdSoe();
			return;
		}

		//如有SOE，则发送
		if(GetFlag(SF_AIE))
		{
			TxdAIE();
			return;
		}

		//如有保护报告，则发送
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
	
	//如有变化遥测
	if(CheckClearFlag(SF_BURSTAI))
	{
		if(TxdBurstAI())
			return;
	}
		
	//发全遥测间隔到
	if(CheckClearFlag(SF_AIREPORT))
	{
		if(TxdAI())
			return;	
	}
	
	//发全遥信间隔到
	if(CheckClearFlag(SF_BIREPORT))
	{
		if(TxdBI())
			return;	
	}
	
	//发全电度间隔到
	if(CheckClearFlag(SF_CIREPORT))
	{
		if(TxdCI())
			return;	
	}

	//发平安报文间隔到
	if(CheckClearFlag(SF_SAFEREPORT))
	{
		if(TxdSafetyRep())
			return;	
	}	

	//发全
#if(SYSTEM_TYPE == _SYS_KF1300_)
	if(CheckClearFlag(SF_LEDREPORT))
	{
		if(TxdLEDStatus(FALSE))
			return; 
	}
#endif

//空闲时发送空闲报文,并且停止IDLE信息
//	StopAndClearMsg(MCC_CommIdle);
}

/*-------------------------------------------------------------*/
/*函数名：CanSearchOneFrame					*/
/*功能：  CANBUS从站规约搜帧函数		       		*/
/*输入:   PBYTE Buf		缓冲区指针			*/
/*	  WORD wLen		搜寻缓冲区长度、	       */
/*输出：  无						       */
/*-------------------------------------------------------------*/

DWORD CSCanbusPro::SearchOneFrame(PBYTE Buf,WORD wLen)
{
	TRxdHead 		*pRxdHead=(TRxdHead *)Buf;
	WORD			wDesAddr;
	WORD			wFrmLen;
	
	//帧头检查
	if(pRxdHead->START!=0x68)
		return  FM_ERR;
	
	//长度检查
	wFrmLen = MAKEWORD(pRxdHead->LengthLo,pRxdHead->LengthHi);

	if(wFrmLen >= 2048)
		return FM_ERR|wLen;
	
	if(wFrmLen+4>wLen)
		return  FM_LESS;
		
	//地址检查
	wDesAddr = MAKEWORD(pRxdHead->DestAddrLO,pRxdHead->DestAddrHI);
//	if(!(wDesAddr&NET_ADDR_VALID))
//		return FM_ERR;

	//维护地址特殊
	if(0xAAAA != wDesAddr)
	{
		wDesAddr &= NET_ADDR_MASK;
		
		if((m_wSAddr!=wDesAddr)&&(dwGBAddress)!=wDesAddr)
			return  FM_ERR;
	}
	
	//校验检查
	if(Buf[wFrmLen+3] != LPC(0,Buf,wFrmLen+3))
		return   FM_ERR;	

	return (FM_OK|(wFrmLen+4));
}

/*-------------------------------------------------------------*/
/*函数名：CanOnFrame					*/
/*功能：  CANBUS从站规约解帧函数		       		*/
/*输入:   PBYTE Buf		帧缓冲区头指针			*/
/*输出：  无						       */
/*-------------------------------------------------------------*/
void CSCanbusPro::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	TxdMonitor();
}

/*-------------------------------------------------------------*/
/*函数名：CanOnFrame					*/
/*功能：  CANBUS从站规约解帧函数		       		*/
/*输入:   PBYTE Buf		帧缓冲区头指针			*/
/*输出：  无						       */
/*-------------------------------------------------------------*/
void CSCanbusPro::OnFrame(PBYTE Buf)
{
	BYTE			bySeq;
	WORD        	wAddr;
	BOOL			bAddrChange = FALSE;
	
	// 注册通信通信状态恢复
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
	
	m_NoRxdCounter = CAN_COMMERR;

	// 平安报文复位
	m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;
	
	m_RxdHead = (TRxdHead *)Buf;
	
	//纪录接收帧序号
	bySeq = m_RxdHead->APPControl&APP_SEQ_MASK;
	wAddr = MAKEWORD(m_RxdHead->SourceAddrLO,m_RxdHead->SourceAddrHI);

	//主控单元地址变化时不判重发
	if(m_wDAddr != wAddr)
	{
		m_wDAddr = wAddr;
	    bAddrChange = TRUE;
	}
	
	//如需确认
	if(m_RxdHead->APPControl&BIT_CON)
	{
		SetFlag(SF_NEEDCONFIRM);
		m_CommandSeq = bySeq;
		m_CommandAddr = wAddr;
		TxdMonitor();
	}

	//记录当前新的请求SEQ号
	if(m_RxdHead->FunCode != FUN_CON)
		m_CurrentReqSeq = bySeq;

	switch(m_RxdHead->FunCode)
	{
	case FUN_CON:
		if(!RxdConfirm())
			return;
		
		break;	
	//遥控或遥调预置、执行、撤销、直接执行
	case FUN_SELECT:	
	case FUN_SELECTWITHVALUE:
	case FUN_OPERATE:
	case FUN_CANCEL:
	case FUN_DIROPERATE:
		if(bAddrChange||(m_LastReqSeq!=bySeq))
		{						
			switch(m_RxdHead->ObjType)
			{
			case DOC_BO://遥控类型
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
				//启动类型
				switch(Buf[12])
				{
					case RESET_WARM:// 冷启动
						DAE_ResetDevice(SYS_RESET_COLD);
						break;
					case RESET_COLD:// 冷启动
						DAE_ResetDevice(SYS_RESET_COLD);
						break;
					case RESET_DATA:// 数据复位	
#if(SYSTEM_TYPE == _SYS_KF1300_)	
						if(DB_ResetRelayData(~((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE)|(0x01<<DB_DATA_CF))))			
#else
						if(DB_UpdateRelayData())			
#endif
							DAE_ResetDevice(SYS_RESET_COLD);
						break;

					case RECOVER_DATA:// 系数初始化 
#if(SYSTEM_TYPE == _SYS_KF1300_)	
						DB_ResetRelayData(~((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE)|(0x01<<DB_DATA_CF)));			
#else
						DB_UpdateRelayData();	
#endif
						break;	
					case RECOVER_COEF:// 系数初始化 
#if(SYSTEM_TYPE == _SYS_KF1300_)
						DB_ResetRelayData((0x01<<DB_DATA_ZERODRIFT)|(0x01<<DB_DATA_FULLVALUE));	
#else
						DB_UpdateCFData();
#endif
						break;
					case RECOVER_PARA:// 系统参数初始化 
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

	//写指定数据对象的值
	case FUN_WRITEVALUE:
		switch(m_RxdHead->ObjType)
		{
		case DOC_NETCFG://网络配置类型	
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

		case DOC_RW://继电器字状态
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
		case DOC_AI://遥测
			TxdAIRes();
			break;

		case DOC_AIWITHQUALITY://带品质描述的遥测
			TxdAIWithQualityRes();
			break;

		case DOC_SRM://二次侧保护测量值
			TxdSRMRes();
			break;
			
		case DOC_BIGROUP://单点遥信
			TxdBIRes();
			break;

		case DOC_BIWITHQUALITY://带品质描述的单点遥信
			TxdBIWithQualityRes();
			break;			
						
		case DOC_CI://电度计数值
			TxdCIRes();
			break;

		case DOC_CIWITHQUALITY://电度计数值
			TxdCIWithQualityRes();
			break;

		case DOC_ABSTIME://绝对时钟
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

		// 端口报文监视
		case DOC_WATCHPORT:
			TxdWatchPortRes();
			break;

		// 变量察看
		case DOC_VARIAL:
			TxdVarDataRes();
			break;

		case DOC_DEVDISP:	
			TxdReadDevDispRes();
			break;

		case DOC_RW://继电器字状态
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
		case DOC_BIBURST://单点变化遥信
			TxdBIBByReadRes();
			break;
		case DOC_BIEVENT://单点的事项顺序纪录
			TxdBIEByReadRes();
			break;
		case DOC_LOGERROR://异常信息
			TxdLogInfoRes();
			break;
		case DOC_OPERATEEVENT://操作事件
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
						
		case DOC_DBIBURST://双点变化遥信
		case DOC_DBIEVENTT://双点事项顺序纪录
		case DOC_SAFETY://平安报文
		case DOC_SYSTIME://相对时钟
		break;
		}
		break;

	case FUN_QUERYLOGBYWRITE://基于写指针的查询
		switch(m_RxdHead->ObjType)
		{
		case DOC_BIBURST://单点变化遥信
			TxdBIBByWriteRes();
			break;
		case DOC_BIEVENT://单点的事项顺序纪录
			TxdBIEByWriteRes();
			break;
		case DOC_OPERATEEVENT://操作事件
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
						
		case DOC_DBIBURST://双点变化遥信
		case DOC_DBIEVENTT://双点事项顺序纪录
		case DOC_SAFETY://平安报文
		case DOC_SYSTIME://相对时钟
		break;
		}
		break;
		
	case FUN_READFILE:
		TxdFileLoad();
		break;	
		
	case FUN_WRITEFILE:
		TxdFileDown();
		break;	
		
	case FUN_READATTRIB://读取指定数据对象的数据属性
		switch(m_RxdHead->ObjType)
		{
		case DOC_BIBURST://单点变化遥信
		case DOC_BIEVENT://单点的事项顺序纪录
		case DOC_OPERATEEVENT://操作事件
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
		
	case FUN_WRITEATTRIB://写指定数据对象的数据属性
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
		case DOC_RELAYCTRL://传动
			TxdRelayControlRes(FALSE);
			break;
		case DOC_SPECIALCMD://特殊操作
			TxdSpecialOperateRes();
			break;
		case DOC_RELAYCTRLWITHBLOCK://带闭锁的传动
			TxdRelayControlRes(TRUE);
			break;
		case DOC_TRANSDATA:
			RxdTransData();
			break;
		default:
			break;
		}
		break;

	//-----------------逻辑方程调试----------------
	case FUN_LOGICDEBUG:
		switch(m_RxdHead->ObjType)
		{
		case DEBUG_RELAY_BP://传动
			Rxd_DebugSetBreakPointReq();
			break;
		case DEBUG_RELAY_CONTEXT://特殊操作
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

	// 记录上一次命令序号
	if(m_RxdHead->FunCode != FUN_CON)
	if(bySeq <= APP_REQSEQ_MAX)
		m_LastReqSeq = bySeq; 
}

/*-------------------------------------------------------------*/
/*函数名：CanOnTimeOut					       */
/*功能：CANBUS从站规约定时器响应函数			       */
/*输入: DWORD id	定时器ID			       */
/*输出：无						       */
/*-------------------------------------------------------------*/
void CSCanbusPro::OnTimeOut(DWORD id)
{
//	DWORD			dwDataMask;
	int			i;

//	TAbsTime time;
	
	//定时器计数	
	m_dwTaskRunCount++;

	// 监控端口监视看门狗
	if (m_nWatchPortDog > 0)//2000.6.8
	{
		m_nWatchPortDog--;

		if(m_nWatchPortDog == 0)
		{
			SetDebugComm(0);
			ReportMsg("关闭串口监视0X%x\n",m_wWatchPortID);
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


/*	//zqz-t 暂时使用默认配置
	//检测配置是否改变(每20个CAN_TIMER_TICK)
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
	//文件传输超时判断
	if(m_File.nTimer > 0)
	{
		m_File.nTimer--;
		if(m_File.nTimer == 0)
			ClearFileTrans();
	}
	
	//超时未收到数据判为通信失败
	do{
		if(m_NoRxdCounter == 0)
			break;
		
		m_NoRxdCounter--;
		
		if(m_NoRxdCounter > 0)
			break;
		
		//注册端口通信状态
		RegisterCommStatus(COMM_STATUS_BREAK);

		// 有心跳时申请配置
		if(m_wTimer[SAFETY_ID] != 0)
			SetFlag(SF_NeedConfig);

		// 调整通信间隔
		StartMsg(MCC_CommIdle,CAN_RUNIDLE);
		
		// 以下网络针对主网有效
		if(!IsNetTask())
			break;
		
//分流控制
//				m_NetCfg.dwStatus = 0;
//				if((m_wPortID == PORT_ID_CANA)||(m_wPortID == PORT_ID_ETHA))
//				{
//					另一个网络通信正常时
//					if(!DAE_GetRunningFlags(SS_NETBCOMM))
//					{
//						DAE_SetNetADataMask(0);
//						DAE_SetNetBDataMask(MASK_ALL);
//					}
//				}
//				else if((m_wPortID == PORT_ID_CANB)||(m_wPortID == PORT_ID_ETHB))
//				{
//				另一个网络通信正常时
//					if(!DAE_GetRunningFlags(SS_NETACOMM))
//					{
//						DAE_SetNetBDataMask(0);
//						DAE_SetNetADataMask(MASK_ALL);
//					}
//				}
	}while(1);
		
	//重发数据阶段禁止定时发送	
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
				//查找变化遥测上传
				if((m_NetCfg.dwStatus&MASK_AIE)==0)
					return;
					
				SetFlag(SF_BURSTAI);
				m_wTimer[AIBURST_ID] = (m_NetCfg.AIBurstRepTM*100+CAN_TIMER_TICK-1)/CAN_TIMER_TICK;
				break;
			
			case AI_ID:
				//全遥测上传
				if((m_NetCfg.dwStatus&MASK_AI)==0)
					return;
					
				SetFlag(SF_AIREPORT);
				m_wTimer[AI_ID] = m_NetCfg.wAIRepTM*1000/CAN_TIMER_TICK;
				break;
			
			case BI_ID:
				//全遥信上传
				if((m_NetCfg.dwStatus&MASK_BI)==0)
					return;
					
				SetFlag(SF_BIREPORT);
				m_wTimer[BI_ID] = m_NetCfg.wBIRepTM*1000/CAN_TIMER_TICK;
				break;

#if(SYSTEM_TYPE == _SYS_KF1300_)
			case LED_ID:
				//LED状态上传
				if((m_NetCfg.dwStatus&MASK_LED)==0)
					return;
					
				SetFlag(SF_LEDREPORT);
				m_wTimer[LED_ID] = m_NetCfg.wLEDStatusTM*1000/CAN_TIMER_TICK;
				break;
#endif			
			case CI_ID:
				//全电度上传
				if((m_NetCfg.dwStatus&MASK_CI)==0)
					return;
					
				SetFlag(SF_CIREPORT);
				m_wTimer[CI_ID] = m_NetCfg.wCIRepTM*1000/CAN_TIMER_TICK;
				break;
			
			case BIBURST_ID:
				//变位遥信上传
				if((m_NetCfg.dwStatus&MASK_BIE)==0)
					return;
					
				SetFlag(SF_BURSTBI);
				break;
			
			case SOE_ID:
				//SOE上传
				if((m_NetCfg.dwStatus&MASK_SOE)==0)
					return;
					
				SetFlag(SF_SOE);
				break;

			case RELAYALARM_ID:
				//保护告警报告上传
				if((m_NetCfg.dwStatus&MASK_RELAYALARMEV)==0)
					return;
					
				SetFlag(SF_RELAYALARM);
				break;
				
			case RELAYSTART_ID:
				//保护启动报告上传
				if((m_NetCfg.dwStatus&MASK_RELAYSTARTEV)==0)
					return;
					
				SetFlag(SF_RELAYSTART);
				break;
				
			case RELAYACTION_ID:
				//保护动须报告上传
				if((m_NetCfg.dwStatus&MASK_RELAYACTIONEV)==0)
					return;
					
				SetFlag(SF_RELAYACTION);
				break;
				
			case BORET_ID:
				//遥控返校
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
			
			//开启100毫秒通道空闲消息
			StartMsg(MCC_CommIdle,m_nBurstIdle);
		}
	}

	
	// 检查LED灯状态	
#if(SYSTEM_TYPE == _SYS_KF1300_)
	BYTE	bStatus,bFreq;

	// 1秒钟检测一次状态变化
	if((m_dwTaskRunCount%10)==0)
	{
		for(i=0;i<LED_RELAY_WORD_NUM;i++)
		{
			if(GetFlashLedValue(i,&bFreq,&bStatus) != 0)
				continue;
			
			if((bStatus != m_pLEDStatus[i])||(bFreq != m_pLEDFreq[i]))
			{
				// 刷新状态值
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
/*函数名：CanOnSoe												*/
/*功能：  CANBUS从站规约SOE消息响应函数		       				*/
/*输入:   DWORD dwDevID		产生SOE的设备ID（不用）				*/
/*输出：  无						       						*/
/*--------------------------------------------------------------*/

void CSCanbusPro::OnSOE()
{	
	// 检修压板投入时退出
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}
	
	//取当前任务的数据指针	
	if((m_NetCfg.dwStatus&MASK_SOE)==0)
		return;
	SetFlag(SF_SOE);
	
	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*函数名：OnAIE													*/
/*功能：  CANBUS从站规约AIE消息响应函数		       				*/
/*输入:   NONE													*/
/*输出：  无						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnAIE()
{	
	// 检修压板投入时退出
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}
	
	//取当前任务的数据指针	
	if((m_NetCfg.dwStatus&MASK_AIE)==0)
		return;
	SetFlag(SF_AIE);
	
	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*函数名：OnRelayEvent											*/
/*功能：  CANBUS从站规约保护报告消息响应函数		       		*/
/*输入:   MsgCode	动作类型													*/
/*输出：  无						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayEvent(DWORD MsgCode, DWORD wParam)
{
	// 检修压板投入时退出
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}

	//有报告的时刻开背光 
	switch(MsgCode)
	{
	//启动
	case 0: OnRelayStart();		break;

	//动作
	case 1: OnRelayAction();    break;		

	//告警
	case 2: OnRelayAlarm();    break;	

	default: break;
	}
}

/*--------------------------------------------------------------*/
/*函数名：OnRelayAlarm											*/
/*功能：  CANBUS从站规约保护告警消息响应函数		       		*/
/*输入:   DWORD dwDevID		产生SOE的设备ID（不用）				*/
/*输出：  无						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayAlarm()
{	
	//取当前任务的数据指针	
	if((m_NetCfg.dwStatus&MASK_RELAYALARMEV)==0)
		return;

	SetFlag(SF_RELAYALARM);
	
	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*函数名：OnRelayStart												*/
/*功能：  CANBUS从站规约保护启动消息响应函数		       				*/
/*输入:   DWORD dwDevID		产生SOE的设备ID（不用）				*/
/*输出：  无						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayStart()
{	
	//取当前任务的数据指针	
	if((m_NetCfg.dwStatus&MASK_RELAYSTARTEV)==0)
		return;

	SetFlag(SF_RELAYSTART);
	
	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*--------------------------------------------------------------*/
/*函数名：OnRelayAction												*/
/*功能：  CANBUS从站规约保护动作消息响应函数		       				*/
/*输入:   DWORD dwDevID		产生SOE的设备ID（不用）				*/
/*输出：  无						       						*/
/*--------------------------------------------------------------*/
void CSCanbusPro::OnRelayAction()
{	
	//取当前任务的数据指针	
	if((m_NetCfg.dwStatus&MASK_RELAYACTIONEV)==0)
		return;

	SetFlag(SF_RELAYACTION);
	
	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*-------------------------------------------------------------*/
/*函数名：CanOnBIBurst						*/
/*功能：  CANBUS从站规约变位遥信消息响应函数		       	*/
/*输入:   DWORD dwDevID		产生SOE的设备ID（不用）		*/
/*输出：  无						       */
/*-------------------------------------------------------------*/

void CSCanbusPro::OnBIBurst()
{
	// 检修压板投入时退出
	if(GetMaintSWStatus())
	{
		ClearAllEvent();
		return;
	}

	if((m_NetCfg.dwStatus&MASK_BIE)==0)
		return;
		
	SetFlag(SF_BURSTBI);

	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

/*-------------------------------------------------------------*/
/*函数名：CanOnBoSelectEcho					*/
/*功能：  CANBUS从站规约遥控选择返校消息响应函数	       	*/
/*输入:   					*/
/*		WORD 	switchno					*/
/*		WORD 	attrib						*/
/*		WORD 	result	*/
/*			=0   成功 =1 失败	*/
/*输出：  无						       */
/*-------------------------------------------------------------*/
	
void CSCanbusPro::OnBOSelectEcho(WORD switchno,WORD attrib,WORD result)	
{	
	m_Bo.RetResult = result;
	SetFlag(SF_HaveBOReturn);

	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

void CSCanbusPro::OnBOExecEcho(WORD switchno,WORD Mode,WORD result)
{
	m_Bo.RetResult = result;
	SetFlag(SF_HaveBOReturn);	

	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

void CSCanbusPro::OnBODirectEcho(WORD switchno,WORD Mode,WORD result)	
{
	m_Bo.RetResult = result;
	SetFlag(SF_HaveBOReturn);
	
	//开启40毫秒快速通道空闲消息
	StartMsg(MCC_CommIdle,m_nBurstIdle);
}

void CSCanbusPro::OnCOEMake(DWORD hPara,DWORD wPara,DWORD pPara,WORD lPara)
{	
	m_Bo.No = LOLOBYTE(wPara);
	m_Bo.Command = LOHIBYTE(wPara);
	m_Bo.Attrib = HIWORD(wPara);
	
	SetFlag(SF_MAKECOEECHO);
}

// KF1300整定返回
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

//响应透明数据消息
void CSCanbusPro::OnTransOIE(DWORD dwPort)
{
	WORD	Tail = DAE_GetTransDataTail();
	
	if(!GetFlag(SF_NeedConfig))
	{
		while(m_TransDataRP != Tail)
		{
			// 无效数据继续
			if(DAE_ReadTransData((BYTE *)m_pMemBlock,m_TransDataRP) == 0)
			{
				m_TransDataRP = (m_TransDataRP+1)%DAE_MAX_TRANSNUM;
				continue;
			}
			
			TxdTransData();
			
			// 递增指针
			m_TransDataRP = (m_TransDataRP+1)%DAE_MAX_TRANSNUM;
		}
	}
}

void CSCanbusPro::AlterConfig(DWORD dwFlag) 
{
	TCanNetCfg 		*pNetCfg;
	WORD			Num;
	
	//取当前任务的数据指针
	pNetCfg = &m_NetCfg;
		
	pNetCfg->dwStatus = dwFlag;

	//设置遥测定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if(pNetCfg->dwStatus&MASK_AI)
		m_wTimer[AI_ID] = pNetCfg->wAIRepTM*1000/CAN_TIMER_TICK;
	else
		pNetCfg->wAIRepTM = 0;
	
	//设置遥信定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if((pNetCfg->dwStatus&MASK_BI)&&(DB_GetBINum()!=0))
		m_wTimer[BI_ID] = pNetCfg->wBIRepTM*1000/CAN_TIMER_TICK;
	else
		pNetCfg->wBIRepTM = 0;
		
	//设置电度定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if((pNetCfg->dwStatus&MASK_CI)&&(DB_GetCINum()!=0))
		m_wTimer[CI_ID] = pNetCfg->wCIRepTM*1000/CAN_TIMER_TICK;
	else
		pNetCfg->wCIRepTM = 0;
			
			
	//设置变化遥测定时期间隔
	if(pNetCfg->dwStatus&MASK_AIE)
	{
		if(!DB_AIEIsEmpty())
			SetFlag(SF_AIE);	

		m_wTimer[AIBURST_ID] = (pNetCfg->AIBurstRepTM*100+CAN_TIMER_TICK-1)/CAN_TIMER_TICK;		
	}
	else
		pNetCfg->AIBurstRepTM = 0;
	
	//设置变位遥信重发间隔		
	if(pNetCfg->dwStatus&MASK_BIE)
	{
		DB_GetBIENum(&Num);
		if(Num>0)
			SetFlag(SF_BURSTBI);	
	}
	else
	{
		pNetCfg->BIBurstRepTM = 0;
		//去掉变位遥信事项标志
		SetFlag(SF_BURSTBI);
		//去掉变位遥信重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIBURST))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}
	
	//设置SOE重发间隔	
	if(pNetCfg->dwStatus&MASK_SOE)
	{
		DB_GetSOENum(&Num);
		if(Num>0)
			SetFlag(SF_SOE);	
	}
	else
	{
		pNetCfg->SoeRepTM = 0;
		//去掉变位遥信事项标志
		SetFlag(SF_SOE,FLAG_OFF);
		//去掉变位遥信重发标志并回退读指针
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIEVENT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
			//计算指针回退个数	
	}

	//设置保护告警报告重发间隔	
	if(pNetCfg->dwStatus&MASK_RELAYALARMEV)
	{
		DB_GetRelayAlarmReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYALARM);	
	}
	else
	{
		//去掉保护告警报告标志
		SetFlag(SF_RELAYALARM,FLAG_OFF);
		//去掉保护告警报告重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYALARMREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//设置保护启动报告重发间隔	
	if(pNetCfg->dwStatus&MASK_RELAYSTARTEV)
	{
		DB_GetRelayStartReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYSTART);	
	}
	else
	{
		//去掉保护启动事项标志
		SetFlag(SF_RELAYSTART,FLAG_OFF);
		//去掉保护启动重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYSTARTREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//设置保护动作报告重发间隔	
	if(pNetCfg->dwStatus&MASK_RELAYACTIONEV)
	{
		DB_GetRelayActionReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYACTION);	
	}
	else
	{
		//去掉保护动作标志
		SetFlag(SF_RELAYACTION,FLAG_OFF);
		//去掉保护动作重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYACTIONREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	if(!(pNetCfg->dwStatus&(MASK_TIME)))
		pNetCfg->TimingTM = 0;
		
	//设平安报文发送间隔	
	m_wTimer[SAFETY_ID] = pNetCfg->SaftyRepTM*1000/CAN_TIMER_TICK;

	if(!(pNetCfg->dwStatus&MASK_BO))
	{	
		//去掉遥控返校事项标志
		SetFlag(SF_HaveBOReturn,FLAG_OFF);

		//去掉遥控反校重发标志
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
			//目前只取消遥控重发
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

			// 清除重发标志
			SetFlag(SF_WAITCONFIRM,FLAG_OFF);

			// 切换回正常运行IDLE
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

	//保存变位遥信帧内容以备重发
	memcpy(m_RetryBuf,m_TxdInfo.pBuf,m_RetryLen);
	StartMsg(MCC_CommIdle,m_nRetryIdle);
	m_RetryCount = MAX_RETRY_NUM;
}

//判断某一遥测是否变化
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
	
	// 平安报文复位
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
	
	//-----------调用数据库接口-----------------
	i=0;	
#if(SYSTEM_TYPE == _SYS_KF1300_)
	AINum = DB_GetRMNum();
#else
	AINum = DB_GetAINum();
#endif
	
	while(LastAIPtr<AINum)
	{
		//-----------调用数据库接口-----------------
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

	// 检修压板投入时退出
	if(GetMaintSWStatus())
		return FALSE;
			
	if((m_NetCfg.dwStatus&MASK_BI)==0)
		return FALSE;

	TxdHead(0xC0+m_CurrentResSeq,FUN_REPORTVALUE);	
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;	
	pTxdHead->ObjType 	= DOC_BIGROUP;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NUM_8;
	
	//-----------调用数据库接口-----------------
	BIByteNum = (DB_GetBINum()+7)/8;
	pDataField[0] = BIByteNum;
	
	for(i=0;i<BIByteNum;i++)
	{
	//-----------调用数据库接口-----------------
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
	
	//-----------调用数据库接口-----------------
	CINum = DB_GetCINum();
	i=0;	
	while(LastCIPtr<CINum)
	{
	//-----------调用数据库接口-----------------
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
		
	// 检修压板投入时清除事件
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
	
	//数据填充，填充响应数据信息
	DB_GetBIERead(&Event_rp);
	DB_GetBIETail(&Event_tail);
		
	WritePtr = 1;
	
	//检测下一个数据是否导致缓冲区越界
	for(i=0;(Event_rp != Event_tail)&&(i<20);i++)
	{
	//-----------调用数据库接口----------------	   	
	   	//数据取完返回	
		if(DB_ReadBIE(&bib ,Event_rp))
			break;
			
	 	pDataField[WritePtr++] = LOBYTE(bib.wNo);
	 	pDataField[WritePtr++] = HIBYTE(bib.wNo);
	   	pDataField[WritePtr++] = bib.bStatus;

		//递增读指针
		Event_rp = (Event_rp+1)%MAX_BIE_NUM;
	}

	if(i==0)
		return FALSE;
	
	pDataField[0] = i;
	
		//填充待确认信息
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

	// 检修压板投入时清除事件
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
	

	//数据填充，填充响应数据信息
	DB_GetSOERead(&Event_rp);
	DB_GetSOETail(&Event_tail);
	
	WritePtr = 1;
	
	for(i=0;(Event_rp != Event_tail)&&(i<10);i++)
	{
	    //-----------调用数据库接口-----------------
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
	   		
		//递增读指针
		Event_rp = (Event_rp+1)%MAX_SOE_NUM;
	}

	if(i==0)
		return FALSE;
	
	pDataField[0] = i;
		
		//填充待确认信息
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

	// 检修压板投入时清除事件
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
	

	//数据填充，填充响应数据信息
	DB_GetAIERead(&Event_rp);
	DB_GetAIETail(&Event_tail);
	
	WritePtr = 1;
	
	for(i=0;(Event_rp != Event_tail)&&(i<16);i++)
	{
	    //-----------调用数据库接口-----------------
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
	   		
		//递增读指针
		Event_rp = (Event_rp+1)%MAX_AIE_NUM;
	}

	if(i==0)
		return FALSE;
	
	pDataField[0] = i;
		
		//填充待确认信息
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
	
	// 检修压板投入时清除事件
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
	
	//数据填充，填充响应数据信息
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
	
	// 报告个数
	pDataField[0] = LOBYTE(Event_rp);
	pDataField[1] = HIBYTE(Event_rp);
	pDataField[2] = 1;
	pDataField[3] = 0;
		
		//填充待确认信息
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
	
	// 检修压板投入时清除事件
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
	
	//数据填充，填充响应数据信息
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
		
		//填充待确认信息
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
	
	// 检修压板投入时清除事件
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
	
	//数据填充，填充响应数据信息
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

		// 记录分报告填写位置
		pItemNum = pDataField+WritePtr;

		// 跳过条目数
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
		
		//填充待确认信息
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
	//-----------调用数据库接口-----------------
#if(SYSTEM_TYPE == _SYS_KF1300_)
	AINum = DB_GetRMNum();	
#else
	AINum = DB_GetAINum();	
#endif
	WritePtr = 2;
	
	for(j=0;j<AINum;j++)
	{
	//-----------调用数据库接口-----------------
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
			//-----------调用数据库接口-----------------
				LastBurstAIPtr = (LastBurstAIPtr+1)%DB_GetAINum();
//				pProInfo->LastBurstAIPtr = (pProInfo->LastBurstAIPtr+1)%100;
					break;
			}
		}
	//-----------调用数据库接口-----------------
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
	
	//填充待确认信息
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
	//-----------调用数据库接口-----------------
//	TSysTime 	SysTime;
	DWORD		dwTemp;
	
	TxdHead(0xE0+m_CurrentResSeq,FUN_REPORTVALUE); 
	
	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = &pTxdHead->Range;
	
	pTxdHead->ObjType 	= DOC_SAFETY;
	pTxdHead->ObjQulifier 	= (INDEX_SIZE_0<<4)+QUALI_NORANGE;
	//取当前时钟
	//-----------调用数据库接口-----------------
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
	
	//取设备状态掩码
	memset(pDataField+4,0,4);
	
	//取设备数据掩码
	//-----------调用数据库接口-----------------
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
			
			// 起始
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
	//-----------调用数据库接口-----------------
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
			// 起始
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

	//-----------调用数据库接口-----------------
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
			// 起始
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

	//-----------调用数据库接口-----------------
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
	//-----------调用数据库接口-----------------
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
	//-----------调用数据库接口-----------------
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
	//-----------调用数据库接口-----------------
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
			// 起始
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
	//-----------调用数据库接口-----------------
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
	//-----------调用数据库接口-----------------
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
	//-----------调用数据库接口-----------------
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
	//-----------调用数据库接口-----------------
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
			// 起始
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

	//取接受数据区头指针		
	wFrameLength = MAKEWORD(m_RxdHead->LengthLo,m_RxdHead->LengthHi);
	Max = DB_GetCFNum();
		
	pRecData = (PBYTE)&m_RxdHead->Range;

	TxdHead(0xC0+m_CurrentReqSeq,m_RxdHead->FunCode);

	pTxdHead = (TTxdHead *)m_TxdInfo.pBuf;
	pDataField = (PBYTE)&pTxdHead->Range;
	pTxdHead->ObjType	= DOC_CI;
	pTxdHead->ObjQulifier = m_RxdHead->ObjQulifier;
	
	//整区定值
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		// 1。取定值包信息
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		Max = DB_GetCINum();
		
		// 长度检查
		if(wFrameLength < (9+4+Num*sizeof(long)))
			return FALSE;
		
		// 2。序号检查

		if(StartNo > Max)
			Num = 0;
		// 越界时
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			
	
		// 3。顺序写入
		for(i=0;i<Num;i++)
		{
			dwCIValue = MAKEDWORD(pRecData[4+i*4],pRecData[5+i*4],pRecData[6+i*4],pRecData[7+i*4]);
			rc |= DB_SetCI(i+StartNo,dwCIValue);
		}
		
		// 填充变程段
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				

		// 填充返回值
		memcpy(pDataField+4,pRecData+4,Num*sizeof(long));
	
		m_TxdWritePtr += 4+Num*sizeof(long);
	}
	else
	{
		//设置电度初值
		Num = pRecData[0];
		for(i=0;i<Num;i++)
		{
			byDDNo = pRecData[i*5+1];
			dwCIValue = MAKEDWORD(pRecData[i*5+2],pRecData[i*5+3],pRecData[i*5+4],pRecData[i*5+5]);
			DB_SetCI(byDDNo,dwCIValue);
		}
			
		//填充变量个数
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
	
	// 填充故障电量
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
		case DOC_BIBURST://单点变化遥信
			Max = MAX_BIE_NUM;
			DB_GetBIEHead(&Head);
			DB_GetBIETail(&Tail);
			DB_GetBIERead(&Read);
			break;
			
		case DOC_BIEVENT://单点的事项顺序纪录
			Max = MAX_SOE_NUM;
			DB_GetSOEHead(&Head);
			DB_GetSOETail(&Tail);
			DB_GetSOERead(&Read);
			break;

		case DOC_OPERATEEVENT://操作事件
			Max = MAX_MISC_NUM;
			DB_Misc_GetHead(&Head);
			DB_Misc_GetTail(&Tail);
			DB_Misc_GetRead(&Read);
			break;

		case DOC_RELAYALARMREPORT://告警记录
			Max = MAX_RALM_NUM;
			DB_GetRelayAlarmReportHead(&Head);
			DB_GetRelayAlarmReportTail(&Tail);
			DB_GetRelayAlarmReportRead(&Read);
			break;

		case DOC_RELAYSTARTREPORT://启动记录
			Max = MAX_RSTT_NUM;
			DB_GetRelayStartReportHead(&Head);
			DB_GetRelayStartReportTail(&Tail);
			DB_GetRelayStartReportRead(&Read);
			break;

		case DOC_RELAYACTIONREPORT://动作记录
			Max = MAX_RACT_NUM;
			DB_GetRelayActionReportHead(&Head);
			DB_GetRelayActionReportTail(&Tail);
			DB_GetRelayActionReportRead(&Read);
			break;

		default:
			break;
	}
	
	//取BIB头指针
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
	
	//取BIB头指针
	DB_GetBIEHead(&Head);
	DB_GetBIETail(&Tail);
	if(wOffset==0)
		m_WrapHeadPtr = Head;
	
	//计算读取BIB的起始位置
	Start = (m_WrapHeadPtr+wOffset)%MAX_BIE_NUM;

	// 指针容错性判断
//	if(Tail>=Head)
//	{
//		if((Start < m_WrapHeadPtr)||(Start > Tail))
//			wNum = 0;
//	}

	for(i=Start,j=0;(i != Tail)&&(j<wNum);j++)
	{
	//-----------调用数据库接口-----------------
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
	
	//取BIE头指针
	if(wOffset==0)
		DB_GetSOEHead(&m_WrapHeadPtr);
	DB_GetSOETail(&Tail);

	//计算读取BIE的起始位置
	Start = (m_WrapHeadPtr+wOffset)%MAX_SOE_NUM;

	// 指针容错性判断
//	if(Tail>=m_WrapHeadPtr)
//	{
//		if((Start < m_WrapHeadPtr)||(Start > Tail))
//			wNum = 0;
//	}
	
	for(i=Start,j=0;(i != Tail)&&(j<wNum);j++)
	{
		//-----------调用数据库接口-----------------
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
	//取LogEvent头指针
	if(wOffset==0)
		DB_Misc_GetHead(&m_OptEventPtr);

	//计算读取LogEvent的起始位置
	Start = (m_OptEventPtr+wOffset)%MAX_MISC_NUM;

	DB_Misc_GetTail(&wTail);

	if(wTail>=m_OptEventPtr)
	{
		if((Start < m_WrapHeadPtr)||(Start > wTail))
			wNum = 0;
	}
	
	for(i=Start,j=0;(i!=wTail)&&(j<wNum);j++)
	{
		// 判断最大个数
		if((WritePtr+sizeof(TMiscEvent)+13) >= m_nMaxFrameLen)
			break;

		//-----------调用数据库接口-----------------
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
	
	//取BIE头指针
	if(Read_Start==0)
		DB_GetRelayAlarmReportHead(&m_WrapHeadPtr);
	DB_GetRelayAlarmReportTail(&Event_tail);

	//计算读取BIE的起始位置
	Read_Start = (m_WrapHeadPtr+Read_Start)%MAX_RALM_NUM;

	// 指针容错性判断
//	if(Event_tail>=m_WrapHeadPtr)
//	{
//		if((Read_Start < m_WrapHeadPtr)||(Read_Start > Event_tail))
//			Read_Num = 0;
//	}
	
	for(i=Read_Start,j=0;(i != Event_tail)&&(j<Read_Num);j++)
	{
		if(DB_ReadRelayAlarmReport(&tReport,i))
		{
			// 长度检查 
			if((WritePtr+12) > (m_nMaxFrameLen-13))
				break;

			// 填充无效报文
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
		
		// 长度检查 
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
	
	//取BIE头指针
	if(Read_Start==0)
		DB_GetRelayStartReportHead(&m_WrapHeadPtr);
	DB_GetRelayStartReportTail(&Event_tail);

	//计算读取BIE的起始位置
	Read_Start = (m_WrapHeadPtr+Read_Start)%MAX_RSTT_NUM;

	// 指针容错性判断
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
		
		// 长度检查 
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
	
	//取BIE头指针
	if(Read_Start==0)
		DB_GetRelayActionReportHead(&m_WrapHeadPtr);
	DB_GetRelayActionReportTail(&Event_tail);

	//计算读取BIE的起始位置
	Read_Start = (m_WrapHeadPtr+Read_Start)%MAX_RACT_NUM;

	// 指针容错性判断
//	if(Event_tail>=m_WrapHeadPtr)
//	{
//		if((Read_Start < m_WrapHeadPtr)||(Read_Start > Event_tail))
//			Read_Num = 0;
//	}
	
	for(i=Read_Start,j=0;(i != Event_tail)&&(j<Read_Num);j++)
	{
		if(DB_ReadRelayActionReport(&tReport,i))
		{
			// 长度检查 
			if((WritePtr+14) > (m_nMaxFrameLen-13))
				break;

			// 填充无效报文
			pDataField[WritePtr++] = 1;
			pDataField[WritePtr++] = 0;
			WritePtr += FillInvalidEvent(pDataField+WritePtr);
			i = (i+1)%MAX_RACT_NUM;
			
			LogError("TxdRelayActionReportByReadRes",FILE_LINE,"Read relay action report(%d) error!",i);
			continue;
		}
		
		// 检查第一个分报告长度
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
			
			// 长度检查	
			if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
				break;

			WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,bList);

			pActionData += nItemLen;

			// 读取列表时只处理第一个分报告
			if(bList)
				m = tReport.tEvent.Num;
		}

		// 不完整检查
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


//按写指针查询
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
	
	// 取BIB头尾指针
	DB_GetBIEHead(&Head);
	DB_GetBIETail(&Tail);

	// 计算读取BIB的起停位置
	Start = (Tail+MAX_BIE_NUM-1-wOffset)%MAX_BIE_NUM;
	Stop = (Head+MAX_BIE_NUM-1)%MAX_BIE_NUM;
		
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
	//-----------调用数据库接口-----------------
		if(DB_ReadBIE(&bib ,i))
			break;
				
		WritePtr += FillBurstBIEvent(pDataField+WritePtr,&bib);

		// 倒退	   	
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
	
	// 取SOE头尾指针
	DB_GetSOEHead(&Head);
	DB_GetSOETail(&Tail);

	// 计算读取BIB的起停位置
	Start = (Tail+MAX_SOE_NUM-1-wOffset)%MAX_SOE_NUM;
	Stop = (Head+MAX_SOE_NUM-1)%MAX_SOE_NUM;
		
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		//-----------调用数据库接口-----------------
		if(DB_ReadSOE(&soe ,i))
				break;

		WritePtr += FillSOEEvent(pDataField+WritePtr,&soe);
	 	
		// 倒退	   	
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

	// 取操作事项头尾指针
	DB_Misc_GetHead(&Head);
	DB_Misc_GetTail(&Tail);

	// 计算读取操作事项记录的起停位置
	Start = (Tail+MAX_MISC_NUM-1-wOffset)%MAX_MISC_NUM;
	Stop = (Head+MAX_MISC_NUM-1)%MAX_MISC_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		// 判断最大个数
		if((WritePtr+sizeof(TMiscEvent)+13) >= m_nMaxFrameLen)
			break;

		//-----------调用数据库接口-----------------
		DB_Misc_Read(i,&OptEvent);

		WritePtr += FillOperateEvent(pDataField+WritePtr,&OptEvent);

		// 倒退	   	
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
	
	//取Alarm头尾指针
	DB_GetRelayAlarmReportHead(&Head);
	DB_GetRelayAlarmReportTail(&Tail);

	// 计算读取告警记录的起停位置
	Start = (Tail+MAX_RALM_NUM-1-wOffset)%MAX_RALM_NUM;
	Stop = (Head+MAX_RALM_NUM-1)%MAX_RALM_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		if(DB_ReadRelayAlarmReport(&tReport,i))
		{
			// 长度检查 
			if((WritePtr+12) > (m_nMaxFrameLen-13))
				break;
			
			// 填充无效报文
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
		
		// 长度检查 
		if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
			break;

		WritePtr += FillRelayEvent(pDataField+WritePtr,(BYTE *)pActionHead,bList);		
		
		// 倒退	   	
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
	
	//取Start头尾指针
	DB_GetRelayStartReportHead(&Head);
	DB_GetRelayStartReportTail(&Tail);

	// 计算读取启动记录的起停位置
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
		
		// 长度检查 
		if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
			break;

		WritePtr += FillRelayEvent(pDataField+WritePtr,(BYTE *)pActionHead,bList);		
		
		// 倒退	   	
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
	
	//取头尾指针
	DB_GetRelayActionReportHead(&Head);
	DB_GetRelayActionReportTail(&Tail);

	// 计算读取的起停位置
	Start = (Tail+MAX_RACT_NUM-1-wOffset)%MAX_RACT_NUM;
	Stop = (Head+MAX_RACT_NUM-1)%MAX_RACT_NUM;
	
	for(i=Start,j=0;(i != Stop)&&(j<wNum);j++)
	{
		if(DB_ReadRelayActionReport(&tReport,i))
		{
			// 长度检查 
			if((WritePtr+14) > (m_nMaxFrameLen-13))
				break;

			// 填充无效报文
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
			// 读取列表时只处理第一个分报告
			nItemLen = sizeof(TRelayEventHead);
			tReport.tEvent.Num = 1;
		}
		else
			nItemLen = sizeof(TRelayEventHead)+tActionHead.FD_Num*6;
			
		// 检查报告长度
		if((WritePtr+2+nItemLen) > (m_nMaxFrameLen-13))
		{
			pDataField[WritePtr++] = 0;
			pDataField[WritePtr++] = 0;
			break;
		}

		// 非条目项填充分报告个数
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
			
			// 长度检查	
			if((WritePtr+nItemLen) > (m_nMaxFrameLen-13))
				break;

			WritePtr += FillRelayEvent(pDataField+WritePtr,pActionData,bList);
			pActionData += nItemLen;
		}

		// 不完整检查
		if(m != tReport.tEvent.Num)
			break;
		
		// 倒退	   	
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
	
	//填充变量个数
	pDataField[0] =1;
	
	//取变量描述信息
	wVarName = MAKEWORD(pRecData[1],pRecData[2]);
	dwOffset = MAKEDWORD(pRecData[3],pRecData[4],pRecData[5],pRecData[6]);
	wLen = MAKEWORD(pRecData[7],pRecData[8]);
	
	//变量查询
	if(DB_LookVar(wVarName,dwOffset,wLen,&VarOut))
		return  FALSE;
	
	if(VarOut.nLen>m_nMaxFrameLen-sizeof(TTxdHead)-3)
		return  FALSE;
		
	//读变量
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
	
	//填充变量个数
	pDataField[0] =1;
	
	//取变量描述信息
	wVarName = MAKEWORD(pRecData[1],pRecData[2]);
	dwOffset = MAKEDWORD(pRecData[3],pRecData[4],pRecData[5],pRecData[6]);
	wLen = MAKEWORD(pRecData[7],pRecData[8]);
	
	//变量查询
	if(DB_LookVar(wVarName,dwOffset,wLen,&VarOut))
		return  FALSE;
	
	if(VarOut.nLen>m_nMaxFrameLen-sizeof(TTxdHead)-3)
		return  FALSE;
		
	//写变量
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
	
	//取LogInfo头指针
	if(wOffset==0)
		DB_GetRunRecordHead(&m_LogInfoPtr);

	//计算读取LogInfo的起始位置
	Start = (m_LogInfoPtr+wOffset)%MAX_RUNREC_NUM;

	DB_GetRunRecordTail(&wTail);
	
	if(wTail>=m_LogInfoPtr)
	{
		if((Start < m_LogInfoPtr)||(Start > wTail))
			wNum = 0;
	}
	
	for(i=Start,j=0;(i!=wTail)&&(j<wNum);j++)
	{
		//-----------调用数据库接口-----------------
		DB_ReadRunRecord(&LogInfo ,i);

		// 判断最大个数
		if((WritePtr+LogInfo.nCharSize+1+2+13) >= m_nMaxFrameLen)
			break;
		
		pDataField[WritePtr++] = LOBYTE(LogInfo.nCharSize+1);
		pDataField[WritePtr++] = HIBYTE(LogInfo.nCharSize+1);
					
		memcpy(&pDataField[WritePtr],LogInfo.szRecordMsg,LogInfo.nCharSize);
		WritePtr +=LogInfo.nCharSize; 

		// 以字符0结尾
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
			//整区定值
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_32)
			{
				m_SET.SETNo = SET_VALUE_ALL;
				
				// 1。取定值包信息
				AreaNo = MAKEWORD(pRecData[0],pRecData[1]);
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				Num = MAKEWORD(pRecData[4],pRecData[5]);
				
				// 长度检查
				if(wFrameLength < (9+8+Num*sizeof(float)))
					return FALSE;
				
				// 2。序号检查
				if(StartNo == 0)
				{
					 m_SET.AreaNo = AreaNo;
					 m_SET.Off = 0;
				}
				else
				{
					// 区号检查
					if(AreaNo != m_SET.AreaNo)
						Num = 0;

					// 必须顺序写入
					else if(StartNo != m_SET.Off)
						Num = 0;
					
					// 越界时
					else if(StartNo >= Max)
						Num = 0;
						
					else if((StartNo+Num)>Max)
						Num = Max-m_SET.Off;	
				}
				
				// 3。包顺序判断
				for(i=0;i<Num;i++)
				{
					dwSET = MAKEDWORD(pRecData[8+i*4],pRecData[9+i*4],pRecData[10+i*4],pRecData[11+i*4]);
					m_SET.pSET[m_SET.Off] = *(float *)&dwSET;
					m_SET.Off++;
				}

				// 满包定值写入
				if(m_RxdHead->APPControl&0x40)
				{
					// 个数检查
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
				
				// 填充变程段
				memcpy(pDataField,pRecData,4);
				pDataField[4] = LOBYTE(Num);
				pDataField[5] = HIBYTE(Num);
				pDataField[6] = 0;
				pDataField[7] = 0;
				
				// 填充返回值
				memcpy(pDataField+8,pRecData+8,Num*sizeof(float));

				WritePtr = 8+Num*sizeof(float);
			}
			else //暂时仅支持一个定值
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);

				// 长度检查
				if(wFrameLength < (9+2+Num*8))
					return FALSE;
				
				AreaNo = MAKEWORD(pRecData[2],pRecData[3]);
				StartNo = MAKEWORD(pRecData[4],pRecData[5]);
				m_SET.AreaNo = AreaNo;
				m_SET.SETNo = StartNo;
				
				dwSET = MAKEDWORD(pRecData[6],pRecData[7],pRecData[8],pRecData[9]);
				
				// 填充返回数据
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
	
	//缓存数据		
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
				
				//填充数据
			if(rc != DB_ERR_OK)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				// 投入失败时返回当前定值区号
				pDataField[0] = DB_GetCurrentSETAreaNo();
				pDataField[1] = 0;
			}
			else
				memcpy(&pDataField[0],pRecData,2);

			m_TxdWritePtr += 2;
			break;
			
		case FUN_OPERATE:
			//-----------调用数据库接口-----------------
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
			//整区定值
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
			{
				m_SET.SETNo = SET_VALUE_ALL;
					
				// 1。取定值包信息
				StartNo = MAKEWORD(pRecData[0],pRecData[1]);
				Num = MAKEWORD(pRecData[2],pRecData[3]);

				// 长度检查
				if(wFrameLength < (9+4+Num*sizeof(float)))
					return FALSE;
				
				// 2。序号检查
				if(StartNo == 0)
					 m_SET.Off = 0;
				else
				{
					// 整区写入需做区号检查
					if(StartNo > Max)
						Num = 0;
					// 必须顺序写入
					else if(StartNo != m_SET.Off)
						Num = 0;
					// 越界时
					else if((StartNo+Num)>Max)
						Num = Max-StartNo;			
				}
				
				// 3。包顺序判断
				for(i=0;i<Num;i++)
				{
					dwSET = MAKEDWORD(pRecData[4+i*4],pRecData[5+i*4],pRecData[6+i*4],pRecData[7+i*4]);
					m_SET.pSET[m_SET.Off] = *(float *)&dwSET;
					m_SET.Off++;
				}

				// 满包定值写入
				if(m_RxdHead->APPControl&0x40)
				{
					// 个数检查
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

				// 填充变程段
				pDataField[0] = pRecData[0];
				pDataField[1] = pRecData[1];
				pDataField[2] = LOBYTE(Num);
				pDataField[3] = HIBYTE(Num);
				
				// 填充返回值
				memcpy(pDataField+4,pRecData+4,Num*sizeof(float));
				WritePtr = 4+Num*sizeof(float);
			}
			else //暂时仅支持一个定值
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);
				// 长度检查
				if(wFrameLength < (9+2+Num*6))
					return FALSE;
				
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				dwSET = MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]);

				m_SET.SETNo = StartNo;
				
				// 填充返回值
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
			//整组投退
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
			{
				m_SET.SETNo = SET_VALUE_ALL;
				StartNo = MAKEWORD(pRecData[0],pRecData[1]);
				Num = MAKEWORD(pRecData[2],pRecData[3]);
				
				// 填充返回值
				memcpy(pDataField,pRecData,4+Num);
				WritePtr = 4+Num;
								
				// 长度检查
				if(wFrameLength < (9+4+Num))
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					break;
				}

				// 格式检查
				if((StartNo!=0)||(Num != DB_GetSWNum()))
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					break;
				}

				// 写入数据
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
			else //暂时仅支持一个软压板
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);
				// 长度检查
				if(wFrameLength < (9+2+Num))
					return FALSE;
				
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				byStatus = (pRecData[4]==0xAA)?SW_STATUS_SET:SW_STATUS_CUT;

				m_SET.SETNo = StartNo;
				
				// 填充返回值
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
			//整区定值
			if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
			{
				m_SET.SETNo = CF_VALUE_ALL;
				
				// 1。取定值包信息
				StartNo = MAKEWORD(pRecData[0],pRecData[1]);
				Num = MAKEWORD(pRecData[2],pRecData[3]);
				// 长度检查
				if(wFrameLength < (9+4+Num*sizeof(long)))
					return FALSE;

				rc = DB_ERR_OK;
				
				// 2。序号检查
				if(StartNo == 0)
					 m_SET.Off = 0;
				
				// 整区写入需做区号检查
				if(StartNo > Max)
					Num = 0;
				// 必须顺序写入
				else if(StartNo != m_SET.Off)
					Num = 0;
				// 越界时
				else if((StartNo+Num)>Max)
					Num = Max-StartNo;			

				// 3。包顺序判断
				for(i=0;i<Num;i++)
				{
					dwCF = MAKEDWORD(pRecData[4+i*4],pRecData[5+i*4],pRecData[6+i*4],pRecData[7+i*4]);
					m_SET.pSET[m_SET.Off] = *(float *)&dwCF;
					m_SET.Off++;
				}

				// 满包定值写入
				if(m_RxdHead->APPControl&0x40)
				{
					// 个数检查
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

				// 填充变程段
				pDataField[0] = pRecData[0];
				pDataField[1] = pRecData[1];
				pDataField[2] = LOBYTE(Num);
				pDataField[3] = HIBYTE(Num);				
				// 填充返回值
				memcpy(pDataField+4,pRecData+4,Num*sizeof(long));

				WritePtr = 4+Num*sizeof(long);
			}
			else //暂时仅支持一个定值
			{
				Num = MAKEWORD(pRecData[0],pRecData[1]);
				// 长度检查
				if(wFrameLength < (9+2+Num*6))
					return FALSE;
				
				StartNo = MAKEWORD(pRecData[2],pRecData[3]);
				dwCF = MAKEDWORD(pRecData[4],pRecData[5],pRecData[6],pRecData[7]);

				m_SET.SETNo = StartNo;
				
				// 填充返回值
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
		
		// 先缓存定值
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
		
		// 先缓存定值
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
		
		// 先缓存定值
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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
	/*多个取定值暂不考虑
	else
	{
		// 1。取定值包信息
		AreaNo = MAKEWORD(pRecData[0],pRecData[1]);
		SETNo = MAKEWORD(pRecData[2],pRecData[3]);
		
		// 2。复制变程段
		memcpy(pDataField,pRecData,4);

		// 3。读取数据
		rc = DB_GetSETFloatValue(AreaNo, SETNo, (float *)&dwSET);
		if(rc == DB_ERR_OK)
		{
			pDataField[10] = LLBYTE(dwSET);
			pDataField[11] = LHBYTE(dwSET);
			pDataField[12] = HLBYTE(dwSET);
			pDataField[13] = HHBYTE(dwSET);
		}
		
		// 5。填充返回状态
		StatusPtr = 4;
		m_TxdWritePtr += 5+sizeof(float);
	}
	*/

	m_TxdWritePtr = WritePtr+13;
	
	//填充待确认信息
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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
	
	//填充待确认信息
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
		WritePtr=4;

		for(int i=0;i<Num;i++)
		{
			DB_GetSWStatus(Start+i,SWStatus);
			pDataField[WritePtr++] = (SWStatus==SW_STATUS_SET)?0xAA:0x55;
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;

	//填充待确认信息
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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
	
	//填充待确认信息
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
	//释放空间
	if(m_File.pData != NULL)
	{
		// 非保留缓冲区，需要释放
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
	char    	strFileName[32];	//文件名
	DWORD		dwFileLength;		//文件长度
	DWORD 		nPerSize;   		//每段文件长度
	WORD		wFileCrc;			//文件校验
	BYTE		byOptType;			//文件操作类型1，启动读；2，结束读；3，启动写；4，结束写
	WORD		wFileSectionNo;
	STATUS		rc;

	//	0，操作正常；
	//	1，文件长度异常
	//	2，内存分配异常
	//	3，文件读取异常
	//	4，文件写入异常
	//	5，文件传输超时中断；
	//	6，文件校验错误；
	//	7，强制中断*/
	
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
			//拷贝文件描述信息
			memcpy(pDataFile,pRecvData,42);
			m_TxdWritePtr += 42;
			
			byOptType = pRecvData[0];
			
			memcpy(strFileName,pRecvData+2,32);
			
			//如已启动传输，重新初始化
			if(byOptType == FILE_STOP_READ)
			{
				if(m_File.nLastSection >= (m_File.nTotalSection-1))
				{
					ReportMsg("Protocol read file %s end!",strFileName);
				}
				else
				{
					ReportMsg("Protocol read file %s break!",strFileName);
					pDataFile[1] = 7;//强制中断
				}
				
				//清除文件操作信息
				ClearFileTrans();
			}
			else
			{
				//清除文件操作信息
				ClearFileTrans();
				
				// 1。读取文件长度
				rc = DAE_GetFileLength(strFileName,&dwFileLength);
				if((rc != DB_ERR_OK)||(dwFileLength > MAX_PROFILE_LEN))//最大文件长2M,程序文件
				{
					ReportMsg("Protocol read file %s Length Err!",strFileName);

					pDataFile[1] = 1;//文件长度异常
					TxdTail(0);
					
					return TRUE;
				}
							
				// 2。分配临时用的文件缓冲区
				if(dwFileLength <= MAX_FILE_LEN)
					m_File.pData = g_pFileDownBuffer;
				else
				{
					m_File.pData = (BYTE *)OS_MemAlloc(dwFileLength);
					if( m_File.pData == NULL )
					{
						ReportMsg("Protocol Read File %s MemAlloc Err!",strFileName);
						pDataFile[1] = 2;//内存分配异常
						TxdTail(0);
						return TRUE;
					}
				}
				
				// 3。读取文件内容
				rc = DAE_ReadFile(strFileName,m_File.pData,dwFileLength);
				if( rc != DB_ERR_OK)
				{
					ReportMsg("Protocol read file %s Err!",strFileName);
					
					ClearFileTrans();
					
					pDataFile[1] = 3;//文件读取异常
					TxdTail(0);
					return TRUE;
				}

				m_File.bStatus = FILE_STATUS_BEGINTRANS;
				
				//重新包装文件属性
				wFileCrc = DAE_CheckSum((BYTE *)m_File.pData,dwFileLength);
				pDataFile[34] = LLBYTE(dwFileLength);
				pDataFile[35] = LHBYTE(dwFileLength);
				pDataFile[36] = HLBYTE(dwFileLength);
				pDataFile[37] = HHBYTE(dwFileLength);
//				pDataFile[38] = LOBYTE(MAX_FILE_SECTIONSIZE);//目前暂定为固定桢长
//				pDataFile[39] = HIBYTE(MAX_FILE_SECTIONSIZE);
				pDataFile[40] = LOBYTE(wFileCrc);
				pDataFile[41] = HIBYTE(wFileCrc);
				
				pDataFile[1] = 0;//文件读取正常

				// 初始化文件信息
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

			// 按照召唤的段号应答
			pDataFile[0] = pRecvData[0];
			pDataFile[1] = pRecvData[1];

			//如未启动传输，重新初始化
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

				// 计算帧长
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
	char    	strFileName[32];	//文件名
	DWORD		dwFileLength;		//文件长度
	DWORD 		nOffset;   			//文件偏移
	WORD 		nPerSize;   		//每段文件长度
	WORD		wFileCrc;			//文件校验
	BYTE		byOptType;			//文件操作类型1，启动读；2，结束读；3，启动写；4，结束写
	WORD		wFileSectionNo;
	STATUS		rc;

	//	0，操作正常；
	//	1，文件长度异常
	//	2，内存分配异常
	//	3，文件读取异常
	//	4，文件写入异常
	//	5，文件传输超时中断；
	//	6，文件校验错误；
	//	7，强制中断*/
	
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
			//填充文件描述信息
			memcpy(pDataFile,pRecvData,42);
			m_TxdWritePtr += 42;
			
			byOptType = pRecvData[0];
			
			memcpy(strFileName,pRecvData+2,32);
			
			dwFileLength = MAKEDWORD(pRecvData[34],pRecvData[35],pRecvData[36],pRecvData[37]);
			nPerSize = MAKEWORD(pRecvData[38],pRecvData[39]);
			wFileCrc = MAKEWORD(pRecvData[40],pRecvData[41]);

			//如已启动传输，重新初始化
			if(byOptType == FILE_STOP_WRITE)
			{
				do{
					// 1.检查文件名
					if(strcmp(m_File.strFileName,strFileName) !=0 )
					{
						ReportMsg("Protocol write file %s is unknowed!",strFileName);
						pDataFile[1] = 6;//文件校验错误
						break;
					}
					
					// 2.检查段号
					if(m_File.nLastSection != (m_File.nTotalSection-1))
					{
						ReportMsg("Protocol write file %s break!",strFileName);
						pDataFile[1] = 7;//强制中断
						break;
					}
					
					// 3。检查校验
					if(DAE_CheckSum(m_File.pData, dwFileLength) != wFileCrc)
					{
						ReportMsg("Protocol write file %s check error!",strFileName);
						pDataFile[1] = 6;//文件校验错误
						break;
					}	
					
					// 4。写入文件

					// 暂停通道消息
					StopMsg(MCC_CommIdle);

					rc = DAE_WriteFile(strFileName,m_File.pData,dwFileLength);

					// 重启通道消息
					StartMsg(MCC_CommIdle);

					if(rc != DB_ERR_OK)
					{
						ReportMsg("Protocol write file %s save error!",strFileName);
						pDataFile[1] = 4;//文件写入异常
					}
					else
					{
						ReportMsg("Protocol write file %s save OK!",strFileName);
						pDataFile[1] = 0;//文件操作正常
					}

					TxdTail(0);
					
					// 注册文件下装
					for(int i=0;i<strlen(strFileName);i++)
					{
						if((strFileName[i]>='A')&&(strFileName[i]<='Z'))
							strFileName[i] = 'a'+strFileName[i]-'A';
					}
					
					DB_Register_FileDown(strFileName,pDataFile[1]);	
					
				}while(0);
				
				//释放空间
				ClearFileTrans();
			}
			else if(byOptType == FILE_START_WRITE)
			{
				//释放空间
				ClearFileTrans();
				
				// 1。读取文件长度
				if(dwFileLength > MAX_PROFILE_LEN)
				{
					ReportMsg("Protocol Write File %s Length Err!",strFileName);

					pDataFile[1] = 1;//文件长度异常
					TxdTail(0);
					
					return TRUE;
				}
							
				// 2。分配临时用的文件缓冲区
				if(dwFileLength <= MAX_FILE_LEN)
					m_File.pData = g_pFileDownBuffer;
				else
				{
					m_File.pData = (BYTE *)OS_MemAlloc(dwFileLength);
					if( m_File.pData == NULL )
					{
						ReportMsg("Protocol Write File %s MemAlloc Err!",strFileName);
						pDataFile[1] = 2;//内存分配异常
						TxdTail(0);
						return TRUE;
					}
				}
				
				m_File.bStatus = FILE_STATUS_BEGINTRANS;
				
				//记录文件属性
				memcpy(m_File.strFileName,strFileName,32);
				m_File.dwFileLength = dwFileLength;
				m_File.nPerSize = nPerSize;//目前暂定为固定桢长
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
			
			//如未启动传输，重新初始化
			do{
				if(m_File.bStatus != FILE_STATUS_BEGINTRANS)
				{
					ReportMsg("Protocol Write File %s unknowed!",m_File.strFileName);
					pDataFile[2] = 0x01;
					wFileSectionNo = 0;
					break;
				}

				// 段号越界判断
				if(wFileSectionNo >= m_File.nTotalSection)
				{
					ReportMsg("Protocol Write File %s overflow!",m_File.strFileName);
					pDataFile[2] = 0x02;
					wFileSectionNo = m_File.nLastSection+1;
					break;
				}

				// 段号不连续判断
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
					//  最后一帧处理
					if(nOffset+m_File.nPerSize >= m_File.dwFileLength)
						nPerSize = m_File.dwFileLength-nOffset;
					else
						nPerSize = m_File.nPerSize;
					
					//  拷贝数据
					memcpy(&m_File.pData[wFileSectionNo*m_File.nPerSize],pRecvData+3,nPerSize);
				}
				
				// 记录上次正确接收的段
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
		//-----------调用数据库接口-----------------
//		ReportMsg("CanBus YK SELECT! No=%d",wRealYKID);
		DB_BOSelect(wRealYKID,wAttr);
//		DB_Register_BO(wRealYKID,wAttr);
		break;
	case FUN_OPERATE:
		//-----------调用数据库接口-----------------
//		ReportMsg("CanBus YK Execute! No=%d",wRealYKID);
		DB_BOExec(wRealYKID,wAttr,1);
		DB_Register_BO(wRealYKID,wAttr);
		break;
	case FUN_CANCEL:
		//-----------调用数据库接口-----------------
//		ReportMsg("CanBus YK Cancel! No=%d",wRealYKID);
		DB_BOExec(wRealYKID,wAttr,2);
//		DB_Register_BO(wRealYKID,wAttr);
		break;
	case FUN_DIROPERATE:
		//-----------调用数据库接口-----------------
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

	//如为当前响应帧清除等待确认标志
	if(bySeq == m_ResInfo.ResSeq)
	{
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);
		switch(m_ResInfo.ResFunCode&0x7F)
		{
			case FUN_REPORTVALUE:
				switch(m_ResInfo.ObjType)
				{
					case	DOC_BIBURST: 
					//-----------调用数据库接口-----------------							
						//递增读指针
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
					//-----------调用数据库接口-----------------
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
					//-----------调用数据库接口-----------------
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncAIERead();

						if(DB_GetAIENum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_AIE,FLAG_OFF);

						break;

					case	DOC_RELAYALARMREPORT://保护告警报告
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncRelayAlarmReportRead();

						if(DB_GetRelayAlarmReportNum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_RELAYALARM,FLAG_OFF);		
						break;

					case	DOC_RELAYSTARTREPORT://保护启动报告
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncRelayStartReportRead();

						if(DB_GetRelayStartReportNum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_RELAYSTART,FLAG_OFF);		
						break;

					case	DOC_RELAYACTIONREPORT://保护动作报告
						for(i=0;i<m_ResInfo.ObjNum;i++)
							DB_IncRelayActionReportRead();

						if(DB_GetRelayActionReportNum(&Num) != DB_ERR_OK)
							break;

						if(Num==0)
							SetFlag(SF_RELAYACTION,FLAG_OFF);		
						break;

					case	DOC_LEDSTATUS://LED灯状态
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
						// 发送结束
						if(m_SET.Off >= DB_GetSETNum())
							SetFlag(SF_SENDRES_SET,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_SET,FLAG_ON);	
						break;
						
					case	DOC_SYSTEMSETTING: 
						// 发送结束
						if(m_SET.Off >= DB_GetSystemSETNum())
							SetFlag(SF_SENDRES_SYSTEMSET,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_SYSTEMSET,FLAG_ON);	
						break;

					case	DOC_SW: 
						// 发送结束
						if(m_SET.Off >= DB_GetSWNum())
							SetFlag(SF_SENDRES_SW,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_SW,FLAG_ON);	
						break;

					case	DOC_CF: 
						// 发送结束
						if(m_SET.Off >= DB_GetCFNum())
							SetFlag(SF_SENDRES_CF,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_CF,FLAG_ON);	
						break;

					case	DOC_BI: 
						// 发送结束
						if(m_SET.Off >= DB_GetBINum())
							SetFlag(SF_SENDRES_BIATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BIATTR,FLAG_ON);	
						break;

					case	DOC_BO: 
						// 发送结束
						if(m_SET.Off >= DB_GetBONum())
							SetFlag(SF_SENDRES_BOATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BOATTR,FLAG_ON);	
						break;

					case	DOC_DISTDIR: 	
						// 发送结束
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
						// 发送结束
						if(m_SET.Off >= DB_GetBINum())
							SetFlag(SF_SENDRES_BIATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BIATTR,FLAG_ON);	
						break;

					case	DOC_BO: 
						// 发送结束
						if(m_SET.Off >= DB_GetBONum())
							SetFlag(SF_SENDRES_BOATTR,FLAG_OFF);
						else
							SetFlag(SF_SENDRES_BOATTR,FLAG_ON);	
						break;
						
					case	DOC_MAPBITOBJ: 
						// 发送结束
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

		// 清除待确认内容
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
	
	//复位帧序号
	m_CurrentResSeq = APP_RESSEQ_MIN;
	m_CurrentReqSeq = APP_REQSEQ_MAX;
	
	//取数据掩码
	Mask = MAKEDWORD(pBuf[0],pBuf[1],pBuf[2],pBuf[3]);

	if(m_NetCfg.dwStatus!=Mask)
	{
		m_NetCfg.dwStatus = Mask;

		//向数据库注册新配置消息,强制每个CAN网传送平安报文
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
	
	//设置遥测定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if((m_NetCfg.dwStatus&MASK_AI)&&(DB_GetAINum()!=0))
	{
		m_NetCfg.wAIRepTM = MAKEWORD(pBuf[4],pBuf[5]);
		m_wTimer[AI_ID] = m_NetCfg.wAIRepTM*1000/CAN_TIMER_TICK;
	}
	else
		m_NetCfg.wAIRepTM = 0;
	
	//设置遥信定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if((m_NetCfg.dwStatus&MASK_BI)&&(DB_GetBINum()!=0))
	{
		m_NetCfg.wBIRepTM = MAKEWORD(pBuf[6],pBuf[7]);
		m_wTimer[BI_ID] = m_NetCfg.wBIRepTM*1000/CAN_TIMER_TICK;
	}
	else
		m_NetCfg.wBIRepTM = 0;

#if(SYSTEM_TYPE == _SYS_KF1300_)
	//设置继电器字定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if(m_NetCfg.dwStatus&MASK_LED)
	{
		m_NetCfg.wLEDStatusTM = CAN_LEDREPTM;
		m_wTimer[LED_ID] = CAN_LEDREPTM*1000/CAN_TIMER_TICK;

		// 收到配置先发送一次LED状态
		SetFlag(SF_LEDREPORT);
	}
	else
		m_NetCfg.wLEDStatusTM = 0;
#endif

	//设置电度定时器间隔并开启定时器
	//-----------调用数据库接口-----------------
	if((m_NetCfg.dwStatus&MASK_CI)&&(DB_GetCINum()!=0))
	{
		m_NetCfg.wCIRepTM = MAKEWORD(pBuf[8],pBuf[9]);
		m_wTimer[CI_ID] = m_NetCfg.wCIRepTM*1000/CAN_TIMER_TICK;
	}
	else
		m_NetCfg.wCIRepTM = 0;
			
	m_NetCfg.AIDeadArea = MAKEWORD(pBuf[10],pBuf[11]);
			
	//设置变化遥测定时期间隔
	if(m_NetCfg.dwStatus&MASK_AIE)
	{
		if(!DB_AIEIsEmpty())
			SetFlag(SF_AIE);	

		m_NetCfg.AIBurstRepTM = MAKEWORD(pBuf[12],pBuf[13]);
		m_wTimer[AIBURST_ID] = (m_NetCfg.AIBurstRepTM*100+CAN_TIMER_TICK-1)/CAN_TIMER_TICK;		
	}
	else
		m_NetCfg.AIBurstRepTM = 0;
	
	//设置变位遥信重发间隔		
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
		//去掉变位遥信事项标志
		SetFlag(SF_BURSTBI,FLAG_OFF);
		//去掉变位遥信重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIBURST))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}
	
	//设置SOE重发间隔	
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
		//去掉变位遥信事项标志
		SetFlag(SF_SOE,FLAG_OFF);
		//去掉变位遥信重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_BIEVENT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//设置保护告警报告重发间隔	
	if(m_NetCfg.dwStatus&MASK_RELAYALARMEV)
	{
		DB_GetRelayAlarmReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYALARM);	
	}
	else
	{
		//去掉事项标志
		SetFlag(SF_RELAYALARM,FLAG_OFF);
		//去掉重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYALARMREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//设置保护启动报告重发间隔	
	if(m_NetCfg.dwStatus&MASK_RELAYSTARTEV)
	{
		DB_GetRelayStartReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYSTART);	
	}
	else
	{
		//去掉事项标志
		SetFlag(SF_RELAYSTART,FLAG_OFF);
		//去掉重发标志
		if(GetFlag(SF_WAITCONFIRM)&&\
				(m_ResInfo.ObjType == DOC_RELAYSTARTREPORT))
		SetFlag(SF_WAITCONFIRM,FLAG_OFF);		
	}

	//设置保护动作报告重发间隔	
	if(m_NetCfg.dwStatus&MASK_RELAYACTIONEV)
	{
		DB_GetRelayActionReportNum(&Num);
		if(Num>0)
			SetFlag(SF_RELAYACTION);	
	}
	else
	{
		//去掉事项标志
		SetFlag(SF_RELAYACTION,FLAG_OFF);
		//去掉重发标志
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
	
	//设平安报文发送间隔	
	m_NetCfg.SaftyRepTM = MAKEWORD(pBuf[22],pBuf[23]);
	m_wTimer[SAFETY_ID] = m_NetCfg.SaftyRepTM*1000/CAN_TIMER_TICK;

	if(!(m_NetCfg.dwStatus&MASK_BO))
	{	
		//去掉遥控返校事项标志
		SetFlag(SF_HaveBOReturn,FLAG_OFF);
		//去掉遥控反校重发标志
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
		case 0:// 电流环零漂整定
		case 1:// 电流环满值整定			
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AUTOADJUSTSTEP1, RtosGetCurrentTaskId(), COEValue,0,0 );
			return;
			
		case 2:// 光纤1零漂整定
		case 3:// 光纤1满值整定
		case 4:// 光纤2零漂整定
		case 5:// 光纤2满值整定
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AUTOADJUSTSTEP1, RtosGetCurrentTaskId(), COEValue,0,0 );
			return;

		default:// 直流量输出校正
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
	
	// 帧限定词类型严格检查，暂仅支持03
	if(m_RxdHead->ObjQulifier != ((INDEX_SIZE_0<<4)+QUALI_NUM_8))
		return;
	
	pRecData = (PBYTE)&m_RxdHead->Range;

	// 备份帧信息
	memcpy(m_Bo.Info,pRecData,9);
	
	// 取电流电压校正值
	for(int i=0;i<m_RxdHead->Range;i++)
	{
		// 电流整定
		if(pRecData[i*4+1] == 9)
			wU = MAKEWORD(pRecData[3+i*4],pRecData[4+i*4]);
		else if(pRecData[i*4+1] == 10)
			wI = MAKEWORD(pRecData[3+i*4],pRecData[4+i*4]);
		else 
		// 直接返校错误
		{
			ReportMsg("KF6500 Net protocol AutoAdjust data error!!!");
			OnAutoAdjustEcho(1,0);
			return;
		}
	}

	// 发送校正消息
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AUTOADJUST, RtosGetCurrentTaskId(), wU,wI,0 );

	ReportMsg("Net protocol send AutoAdjust. U =%d I=%d",wU,wI);
#endif
}

BOOL CSCanbusPro::RxdAbsTime()
{
	PBYTE       pRecData;
	TAbsTime 	AbsTime;	
	TSysTime 	SysTime;
	
	//在B格式时钟无效的情况下，采用通信规约对钟，并同时更新硬件时钟
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
	
	//-----------调用数据库接口-----------------
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
	
	// 全部参数
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

		// 填充返回值
		memcpy(pDataField,pRecData,4+DB_GetChannelNum()*4);

		WritePtr = 4+DB_GetChannelNum()*4;
	}
	else //暂时仅支持一个定值
	{
		no = MAKEWORD(pRecData[2],pRecData[3]);
			
		pChannel->wMeasureRange = MAKEWORD(pRecData[4],pRecData[5]);
		pChannel->wSampleClass = MAKEWORD(pRecData[6],pRecData[7]);

		// 填充返回值
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
	
	//-----------调用数据库接口-----------------
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
		
	// 全部参数
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		for(i=0;i<DB_GetChannelSrcNum();i++)
		{
			if(FLAG_CHANNELSRC_VALID == MAKEDWORD(pRecData[i*4+4],pRecData[i*4+5],pRecData[i*4+6],pRecData[i*4+7]))
				rc |= DB_EnableChannelSrc(i);
			else
				rc |= DB_DisenableChannelSrc(i);
		}

		// 操作失败记录
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;
						
		// 填充返回值
		memcpy(pDataField,4+pRecData,DB_GetChannelSrcNum()*4);

		WritePtr = 4+DB_GetChannelSrcNum()*4;
	}
	else //暂时仅支持一个定值
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
	
	//-----------调用数据库接口-----------------
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
	
	// 操作失败记录
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
	
	// 全部参数
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		// 先设置零漂
		for(i=0;i<DB_GetChannelNum();i++)
			pZDvalue[i] = MAKEDWORD(pRecData[i*6+4],pRecData[i*6+5],pRecData[i*6+6],pRecData[i*6+7]);

		rc = DB_SetZeroDriftValue(CHANNEL_VALUE_ALL,pZDvalue);

		// 再设置满值
		for(i=0;i<DB_GetChannelNum();i++)
			pFullvalue[i] = MAKEWORD(pRecData[i*6+8],pRecData[i*6+9]);

		rc |= DB_SetChannelFullValue(CHANNEL_VALUE_ALL,pFullvalue);

		
		// 操作失败记录
		if(rc != DB_ERR_OK)
			pTxdHead->Status |= STATUS_CTL_ERROR;

		// 填充返回值
		memcpy(pDataField,pRecData,DB_GetChannelNum()*6);

		WritePtr = 4+DB_GetChannelNum()*6;
	}
	else //暂时仅支持一个定值
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

// 直流输出整定返回
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

	// 取出数据
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

		// 再写入
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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

	//填充待确认信息
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

	//整区定值
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1。取定值包信息
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		// 长度检查
		if(wFrameLength < (9+4+Num*4))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2。序号检查
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// 整区写入需做区号检查
		if(StartNo > Max)
			Num = 0;
		// 必须顺序写入
		else if(StartNo != m_SET.Off)
			Num = 0;
		// 越界时
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3。包顺序判断
		for(i=0;i<Num;i++)
		{
			pBI[StartNo+i].wControl = MAKEWORD(pRecData[4+i*4],pRecData[5+i*4]);
			pBI[StartNo+i].wFilterTime = MAKEWORD(pRecData[6+i*4],pRecData[7+i*4]);
			
			m_SET.Off++;
		}

		// 满包定值写入
		if(m_RxdHead->APPControl&0x40)
		{
			// 个数检查
			if(m_SET.Off != Max)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				Num = 0;
			}
			else
			{
				// 取系统修改缓冲区
				pBIAttr = (TBIPara	*)DB_GetParaModBuf(DB_PARA_BI);		
				pDIAttr = (TDIPara	*)DB_GetParaModBuf(DB_PARA_DI);

				do{
					rc = DB_GetBIPara(DB_PARA_ALL,pBIAttr); 		
					rc |= DB_GetDIPara(DB_PARA_ALL,pDIAttr); 	
					if(rc != DB_ERR_OK)
						break;
					
					// 1。先设置BI
					// 逐个修改
					for(i=0;i<Max;i++)
						pBIAttr[i].wControl = pBI[i].wControl;
					
					rc |= DB_WriteBIPara(DB_PARA_ALL, pBIAttr);			
					if(rc != DB_ERR_OK)
						break;
					
					rc |= DB_WriteBIParaConfirm(DB_PARA_ALL);
					if(rc != DB_ERR_OK)
						break;
					
					// 2。再设置DI
					for(i=0;i<Max;i++)
					{
						// 逐个修改
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
		// 填充变程段
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				
		// 填充返回值
		memcpy(pDataField+4,pRecData+4,Num*4);

		WritePtr = 4+Num*4;
	}
	else //暂时仅支持一个定值	
	{
		Num = MAKEWORD(pRecData[0],pRecData[1]);
		// 长度检查
		if(wFrameLength < (9+2+Num*6))
			return FALSE;
		
		StartNo = MAKEWORD(pRecData[2],pRecData[3]);
		m_SET.SETNo = StartNo;
		
		// 填充返回值
		// 读取、替换、写入		
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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
	
	//填充待确认信息
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

	//整区定值
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1。取定值包信息
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		// 长度检查
		if(wFrameLength < (9+4+Num*2))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2。序号检查
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// 整区写入需做区号检查
		if(StartNo > Max)
			Num = 0;
		// 必须顺序写入
		else if(StartNo != m_SET.Off)
			Num = 0;
		// 越界时
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3。包顺序判断
		for(i=0;i<Num;i++)
		{
			// 先读取
			DB_GetBOPara(StartNo+i, &tBOAttr);

			// 再替换
			tBOAttr.wLastTime = MAKEWORD(pRecData[4+i*2],pRecData[5+i*2]);

			//再写入
			DB_WriteBOPara(StartNo+i, &tBOAttr);
			
			m_SET.Off++;
		}

		// 满包定值写入
		if(m_RxdHead->APPControl&0x40)
		{
			// 个数检查
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

		// 填充变程段
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				
		// 填充返回值
		memcpy(pDataField+4,pRecData+4,Num*2);

		WritePtr = 4+Num*2;
	}
	else //暂时仅支持一个定值
	{
		Num = MAKEWORD(pRecData[0],pRecData[1]);
		// 长度检查
		if(wFrameLength < (9+2+Num*6))
			return FALSE;
		
		StartNo = MAKEWORD(pRecData[2],pRecData[3]);
		m_SET.SETNo = StartNo;
		
		// 填充返回值
		// 先读取
		rc = DB_GetBOPara(StartNo+i, &tBOAttr);
		
		// 再替换
		tBOAttr.wLastTime= MAKEWORD(pRecData[4+i*2],pRecData[5+i*2]);
		
		//再写入
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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

	//填充待确认信息
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

	//整区定值
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_16)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1。取定值包信息
		StartNo = MAKEWORD(pRecData[0],pRecData[1]);
		Num = MAKEWORD(pRecData[2],pRecData[3]);
		// 长度检查
		if(wFrameLength < (9+4+Num*2))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2。序号检查
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// 整区写入需做区号检查
		if(StartNo > Max)
			Num = 0;
		// 必须顺序写入
		else if(StartNo != m_SET.Off)
			Num = 0;
		// 越界时
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3。包顺序判断
		for(i=0;i<Num;i++)
		{
			// 先读取
			DB_GetAOPara(StartNo+i, &tAOAttr);

			// 再替换
			tAOAttr.wControl = MAKEWORD(pRecData[4+i*6],pRecData[5+i*6]);
			tAOAttr.wCHIndex = MAKEWORD(pRecData[6+i*6],pRecData[7+i*6]);
			tAOAttr.wRange   = MAKEWORD(pRecData[8+i*6],pRecData[9+i*6]);

			//再写入
			DB_WriteAOPara(StartNo+i, &tAOAttr);
			
			m_SET.Off++;
		}

		// 满包定值写入
		if(m_RxdHead->APPControl&0x40)
		{
			// 个数检查
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

		// 填充变程段
		pDataField[0] = pRecData[0];
		pDataField[1] = pRecData[1];
		pDataField[2] = LOBYTE(Num);
		pDataField[3] = HIBYTE(Num);				
		// 填充返回值
		memcpy(pDataField+4,pRecData+4,Num*6);

		WritePtr = 4+Num*6;
	}
	else //暂时仅支持一个定值
	{
		Num = MAKEWORD(pRecData[0],pRecData[1]);
		// 长度检查
		if(wFrameLength < (9+2+Num*6))
			return FALSE;
		
		StartNo = MAKEWORD(pRecData[2],pRecData[3]);
		m_SET.SETNo = StartNo;
		
		// 填充返回值
		// 先读取
		rc = DB_GetAOPara(StartNo+i, &tAOAttr);
		
		// 再替换
		tAOAttr.wControl= MAKEWORD(pRecData[4],pRecData[5]);
		tAOAttr.wCHIndex= MAKEWORD(pRecData[6],pRecData[7]);
		tAOAttr.wRange  = MAKEWORD(pRecData[8],pRecData[9]);
		
		//再写入
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
			// 依次读取并填充串口参数
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
			
			// 依次读取并填充CAN网参数
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
			
			// 依次读取并填充以太网连接参数
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
			// 依次设置串口参数
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
			// 依次设置CAN网参数
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
			// 依次设置以太网连接参数
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

	// 程序版本号
	//for(int i=0; i<4; i++)
	//	tFileInf.SoftWareVer[i] = MAKEWORD(HIBYTE(tFileInf.SoftWareVer[i]), \
	//								LOBYTE(tFileInf.SoftWareVer[i]) );
	memcpy((char *)pDataField,tFileInf.SoftWareVer,VER_INF_LEN);//软件版本信息，字符串
	pDataField +=16;
	
	// 程序校验码
	*pDataField++ = LOBYTE(tFileInf.wCheckSum);
	*pDataField++ = HIBYTE(tFileInf.wCheckSum);

	dwCheckLen = GetPLCDBVer();
	*pDataField++ = LLBYTE(dwCheckLen);
	*pDataField++ = LHBYTE(dwCheckLen);
	*pDataField++ = HLBYTE(dwCheckLen);
	*pDataField++ = HHBYTE(dwCheckLen);
	
	// 逻辑校验码
	dwCheckLen = GetPLCCheckSum();
	*pDataField++ = LOBYTE(dwCheckLen);
	*pDataField++ = HIBYTE(dwCheckLen);
	
	//数据库版本信息，版本号，文件校验码
	DB_GetDataBaseVersion(pDataField);
	pDataField += 8;

	//MAC地址信息，两个网口
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
	
	// 温度
	pDataField[0] = LOBYTE(SysTempture);
	pDataField[1] = HIBYTE(SysTempture);
	
	// 开出光隔状态
	Relay_GetBOINStatus(pDataField+2);
	Relay_GetBOBRStatus(pDataField+10);

	// 镜像位对象通信状态4
	DAE_GetMBOCommStatus(pDataField+18);
	
	// 端口通信状态
	for(int i=0;i<16;i++)
	{
		if(DAE_GetRunningFlags(SS_PORT_COMM_ERR+i))
			pDataField[26+(i/8)] |= (0x01<<(i%8));
	}
	
	// 卫星钟状态
	if(DAE_GetRunningFlags(SS_TIME_B))
		pDataField[30] = 0x80|(IrigQuality&0x0F);
	else
		pDataField[30] = (IrigQuality&0x0F);

	//  取测距状态
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

	// 取定值区号
	nZeroNo = MAKEWORD(pRecData[0],pRecData[1]);

	rc = DB_GetCFGWord(nZeroNo,pCfgWord);
	
	if(rc != DB_ERR_OK)
		pTxdHead->Status |= STATUS_CTL_ERROR;
	else
	{
		// 区号
		pDataField[WritePtr++] = pRecData[0];
		pDataField[WritePtr++] = pRecData[1];
		
		//-----------调用数据库接口-----------------
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
	
	//整区定值
//	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_NORANGE)
	{
		// 1。取定值包信息
		// 长度检查
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


// 传动指令
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
			//远方时闭锁
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
		case 1:// 模拟报告指令		
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
			
		case 2:// 启动录波指令
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

		case 3:// 定值复制

			do{
				if((pRecData[1] >= DB_GetSETAreaNum())||\
					(pRecData[2] >= DB_GetSETAreaNum())||\
					(pRecData[3] >= DB_GetSETAreaNum()))
				{
					pTxdHead->Status |= STATUS_CTL_ERROR;
					break;
				}
				
				// 读取源定值区
				rc = DB_GetSETFloatValue(pRecData[1], SET_VALUE_ALL, (float *)m_pMemBlock);
				if(rc != DB_ERR_OK)
					break;
				
				// 逐个写入目的定值区
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

		case 4://功能按键
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

// 直流输出整定返回
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
	
    //发送消息给AI,停止其模拟量输入输出转换工作
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_AI), SM_AOADJUST, 0, 0,0,0 );			

	//将所校正的数字量以及输出量程类型写入到双口RAM中
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
	
	// 逐个读取
	for(int i=0;i<Num;i++)
	{
		// 读零漂
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

		// 读满值
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

		// 读目标值
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
	
	//-----------调用数据库接口-----------------
	pDataField[0] = (LED_RELAY_WORD_NUM+1)/2;

	for(i=0;i<LED_RELAY_WORD_NUM;i++)
	{
	//-----------调用数据库接口-----------------
		GetFlashLedValue(i,&bFreq,&bStatus);

		// 刷新状态值
		m_pLEDStatus[i] = bStatus;
		m_pLEDFreq[i] = bFreq;
		
		// 绿色
		if(i%2)
			pDataField[1+i/2] += ((bFreq<<1)+(bStatus&0x01))<<4;
		// 红色
		else
			pDataField[1+i/2] = ((bFreq<<1)+(bStatus&0x01))&0x0F;
	}
	
	m_TxdWritePtr += 1+pDataField[0];

	TxdTail(0);

	if(bBurst)
	{
		//填充待确认信息
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
	
	//-----------调用数据库接口-----------------
	pDataField[0] = (LED_RELAY_WORD_NUM+1)/2;

	for(i=0;i<LED_RELAY_WORD_NUM;i++)
	{
	//-----------调用数据库接口-----------------
		GetFlashLedValue(i,&bFreq,&bStatus);

		// 刷新状态值
		m_pLEDStatus[i] = bStatus;
		m_pLEDFreq[i] = bFreq;
		
		// 绿色
		if(i%2)
			pDataField[1+i/2] += ((bFreq<<1)+(bStatus&0x01))<<4;
		// 红色
		else
			pDataField[1+i/2] = ((bFreq<<1)+(bStatus&0x01))&0x0F;
	}
	
	m_TxdWritePtr += 1+pDataField[0];

	TxdTail(0);
#endif		
	return TRUE;
}

// 查看端口报文
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

	// 监视端口ID看门狗,设定30秒
	m_nWatchPortDog = 30*(1000/CAN_TIMER_TICK);
	
	wPortID = MAKEWORD(pRecData[0],pRecData[1]);
	wLen=MAKEWORD(pRecData[2],pRecData[3]);

	// 随时监控端口号变化
	if (m_wWatchPortID != wPortID)
	{
		ReportMsg("开始查看端口:0x%x",wPortID);
		SetDebugComm(wPortID);
		m_wWatchPortID = wPortID;
	}

	// 读取数据内容
	wLen = ReadPrompt((char *)(pDataField+4),wLen);

	pDataField[0] = pRecData[0];
	pDataField[1] = pRecData[1];
	pDataField[2] = LOBYTE(wLen);
	pDataField[3] = HIBYTE(wLen);

	m_TxdWritePtr += wLen+4;

	TxdTail(0);	
			
	return TRUE;
}

// 查看变量数据
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

	// 读取变量空间大小
	dwMax = DAE_GetMemVarSize(dwVarID);
	dwStartPos = MAKEDWORD(pRecData[8],pRecData[9],pRecData[10],pRecData[11]);
	dwLen=MAKEWORD(pRecData[12],pRecData[13]);

	// 读取变量内容
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
			// 起始
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
	
	//-----------调用数据库接口-----------------
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

// 设备描述信息
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

	//	char		DevType[16];			//设备类型
	strcpy((char *)pDataField,PRO_TYPE);
	//	WORD		nAI;					//遥测个数				
	pDataField[16] = LOBYTE(DB_GetAINum());
	pDataField[17] = HIBYTE(DB_GetAINum());
	//	WORD		nBI;					//遥信个数				
	pDataField[18] = LOBYTE(DB_GetBINum());
	pDataField[19] = HIBYTE(DB_GetBINum());
	//	WORD		nCI;					//电度个数				
	pDataField[20] = LOBYTE(DB_GetCINum());
	pDataField[21] = HIBYTE(DB_GetCINum());
	//	WORD		nBO;					//遥控个数				
	pDataField[22] = LOBYTE(DB_GetBONum());
	pDataField[23] = HIBYTE(DB_GetBONum());
	//	WORD		nSerNum;				//串口个数				
	pDataField[24] = LOBYTE(DB_GetSerialNum());
	pDataField[25] = HIBYTE(DB_GetSerialNum());
	//	WORD		nCanNum;				//CAN网端口个数 		
	pDataField[26] = LOBYTE(DB_GetCANNum());
	pDataField[27] = HIBYTE(DB_GetCANNum());
	//	WORD		nEthNum;				//以太网端口个数			
	pDataField[28] = LOBYTE(DB_GetEthLinkNum());
	pDataField[29] = HIBYTE(DB_GetEthLinkNum());
	//	WORD		nSW;					//软压板个数		
	pDataField[30] = LOBYTE(DB_GetSWNum());
	pDataField[31] = HIBYTE(DB_GetSWNum());
	//	WORD		nCFG;					//配置字个数		
	pDataField[32] = LOBYTE(DB_GetCFGNum());
	pDataField[33] = HIBYTE(DB_GetCFGNum());
	//	WORD		nSET;					//定值个数		
	pDataField[34] = LOBYTE(DB_GetSETNum());
	pDataField[35] = HIBYTE(DB_GetSETNum());
	//	WORD		nSETArea;				//定值区个数		
	pDataField[36] = LOBYTE(DB_GetSETAreaNum());
	pDataField[37] = HIBYTE(DB_GetSETAreaNum());
	//	WORD		nSETAreaCur;			//当前定值区号		
	pDataField[38] = LOBYTE(DB_GetCurrentSETAreaNo());
	pDataField[39] = HIBYTE(DB_GetCurrentSETAreaNo());
	//	WORD		nSystemSET; 			//辅助定值个数	
	pDataField[40] = LOBYTE(DB_GetSystemSETNum());
	pDataField[41] = HIBYTE(DB_GetSystemSETNum());
	//	WORD		nCF;					//系数个数		
	pDataField[42] = LOBYTE(DB_GetCFNum());
	pDataField[43] = HIBYTE(DB_GetCFNum());
	//	WORD		nRM;					//保护测量值个数	
	pDataField[44] = LOBYTE(DB_GetRMNum());
	pDataField[45] = HIBYTE(DB_GetRMNum());
	//	WORD		nRE;					//保护动作个数	
	pDataField[46] = LOBYTE(DB_GetRENum());
	pDataField[47] = HIBYTE(DB_GetRENum());
	//	WORD		nTC;					//传动个数
	pDataField[48] = LOBYTE(DB_GetTCNum());
	pDataField[49] = HIBYTE(DB_GetTCNum());
	//	WORD		nLanguageNo;			//语言编号
	pDataField[50] = LOBYTE(DB_GetLanguageNo());
	pDataField[51] = HIBYTE(DB_GetLanguageNo());
	//	WORD		nMBO;					//镜像位对象个数
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
	//-----------调用数据库接口-----------------
	{
		no = MAKEWORD(pRecData[1+i*2],pRecData[2+i*2]);
		// 读取继电器字状态		
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
	//-----------调用数据库接口-----------------
	{
		no = MAKEWORD(pRecData[1+i*3],pRecData[2*i*3]);
		// 写继电器字状态
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

	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
		
	// 发送最后一帧
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
		
		//-----------调用数据库接口-----------------
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
			WritePtr += 4;//备用
		}
		
		m_SET.Off += Num;
	}
	m_TxdWritePtr = WritePtr+13;
	
	//填充待确认信息
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

	//整区修改
	if(m_RxdHead->ObjQulifier == (INDEX_SIZE_0<<4)+QUALI_ADDR_8)
	{
		m_SET.SETNo = DB_PARA_ALL;
		
		// 1。取定值包信息
		StartNo = pRecData[0];
		Num = pRecData[1];
		// 长度检查
		if(wFrameLength < (9+2+Num*sizeof(TMBO_Disp)))
			return FALSE;

		rc = DB_ERR_OK;
		
		// 2。序号检查
		if(StartNo == 0)
			 m_SET.Off = 0;
		
		// 整区写入需做区号检查
		if(StartNo >= Max)
			Num = 0;
		// 必须顺序写入
		else if(StartNo != m_SET.Off)
			Num = 0;
		// 越界时
		else if((StartNo+Num)>Max)
			Num = Max-StartNo;			

		// 3。包顺序判断
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

		// 满包定值写入
		if(m_RxdHead->APPControl&0x40)
		{
			// 个数检查
			if(m_SET.Off != Max)
			{
				pTxdHead->Status |= STATUS_CTL_ERROR;
				Num = 0;
			}
			else
			{
				// 整体修改
				rc = DB_SetMapBitObjDisp(DB_PARA_ALL, (TMBO_Disp *)m_pMemBlock);
				if(rc != DB_ERR_OK)
					pTxdHead->Status |= STATUS_CTL_ERROR;
				
			}
		}

		// 填充返回值
		memcpy(pDataField,(BYTE *)&m_RxdHead->Range,2+Num*sizeof(TMBO_Disp));
		pDataField[1] = Num;

		WritePtr = 2+Num*sizeof(TMBO_Disp);
	}
	else //暂时仅支持一个定值	
	{
		Num = pRecData[0];
		StartNo = pRecData[1];
		
		// 长度检查
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
	
	//-----------调用数据库接口-----------------
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
		WritePtr += 4;//备用
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

	// 长度检查
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
		// 整体修改
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
	
	// 填充返回值
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

//-1表示向后运行一行
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


//继电器序号
	for(i=0;i<2*Num;i++)
	{
		*pDataField++ = pRecData[1+i];
	}

//继电器值


	for(i=0;i<Num;i++)
	//-----------调用数据库接口-----------------
	{
		no = MAKEWORD(pRecData[1+i*2],pRecData[2+i*2]);
		// 读取继电器字状态		
		*pDataField++ = ReadRWFullValue(no);

//		ReportMsg("Protocol read RW no=%d value=%d",no,ReadRWFullValue(no));
	}
				
	m_TxdWritePtr += 1+Num*2+Num;
	
	TxdTail(0); 

						
	return TRUE;	
}

//===================================调试可编程逻辑===========================================
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

//继电器序号
	for(i=0;i<2*Num;i++)
	{
		*pDataField++ = pRecData[5+i];
	}

//继电器值


	for(i=0;i<Num;i++)
	//-----------调用数据库接口-----------------
	{
		no = MAKEWORD(pRecData[5+i*2],pRecData[6+i*2]);
		// 读取继电器字状态		
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
	
	//修改缓冲区内容
	
	if(byCount > DEBUG_IN_NUM)
		byCount = DEBUG_IN_NUM;
	for(i=0;i<byCount;i++)
	{
		// 写继电器字状态 	
		WriteDebugIn(wStart+i, pRecData[3+i]);
	}

	//回应答
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

	//继电器值
	for(i=0;i<byCount;i++)
	{
		// 读取继电器字状态		
		*pDataField++ = ReadDebugOut(wStart+i);
	}
				
	m_TxdWritePtr += 3+byCount;
	
	TxdTail(0); 
						
	return TRUE;		
}

//===================================调试可编程逻辑===========================================
//接收透明数据
BOOL CSCanbusPro::RxdTransData()
{
	BYTE			*pRecData = &m_RxdHead->Range;
	BYTE			*pTransData = pRecData+sizeof(TTransDataHead)-1;
    TAppInfo 		*pAppInfo;
	WORD			wClass,wPort,i;

	wClass = MAKEWORD(pRecData[0],pRecData[1]);

	switch(wClass)
	{
		case 0x0000://请求关联
			//向所有本协议端口广播
			pAppInfo = &G_pCfgTable->TaskApp[SYS_PORT_MAX];
			for(i=SYS_PORT_MAX;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
			{
				if(pAppInfo->pAppItem == NULL)
					continue;

				// 剔除本规约
				if(pAppInfo->dwPortID == m_wPortID)
					continue;

				// 找到同类规约
				if(pAppInfo->pAppItem->fpEntry != (TTASK_ENTRY)SCanbus_Task)
					continue;

				// 向规约发送消息
				DAE_WriteTransData(wClass, pAppInfo->dwPortID, 0, pTransData);
			}
			break;
			
		case 0x0001://确认关联
			//来自源端口时抱错
			if(m_TransReqPortID == m_wPortID)
			{
				LogError("RxdTransData",FILE_LINE,"Connect ack from the same port(%x)!",m_wPortID);
				break;
			}

			// 向源端口和自身发送消息
			DAE_WriteTransData(wClass, m_TransReqPortID, 0,pTransData);
			DAE_WriteTransData(wClass, m_wPortID, 0,pTransData);
			break;
						
		case 0x0100://读数据请求
		case 0x0200://写数据请求
			//pTDHead->wDesPort = MAKEWORD(pRecData[2],pRecData[3]);
			//pTDHead->wDataLen = MAKEWORD(pRecData[6],pRecData[7]);
			DAE_WriteTransData(wClass, MAKEWORD(pRecData[2],pRecData[3]), MAKEWORD(pRecData[6],pRecData[7]),pTransData);

			TSysTime stTime1;

			ReadSystemTime(&stTime1);
			ReportMsg("Port 0x%x: Rxd trans data<<<<< Time=%d.%d.%d.%d FILE_NO=%d",m_wPortID,stTime1.Hour,\
				stTime1.Minute,stTime1.Second,stTime1.MSecond,MAKEWORD(pRecData[10],pRecData[11]));

			break;

		case 0x0300://测距数据
			DAE_WriteTransData(wClass, SYS_PORTID_FL, MAKEWORD(pRecData[6],pRecData[7]),pTransData);
			break;
			
		default:
			break;
	}
	
	return TRUE;
}

//发送透明数据
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
		case 0x0000://请求关联				
			memset(pDataField,0,sizeof(TTransDataHead));			
			m_TxdWritePtr += sizeof(TTransDataHead)-1;
			ReportMsg("Port 0x%x: send trans data 0x0000>>>>>>>>",m_wPortID);

			// 记录下请求关联方
			m_TransReqPortID = pTDHead->wSrcPort;
			break;
			
		case 0x0010://响应关联
			pDataField[0] = 0x10;
			pDataField[1] = 0x00;

			//本端口发给自身的，告知: 目的端口是请求关联方
			if(pTDHead->wSrcPort == m_wPortID)
			{
				pDataField[2] = LOBYTE(m_TransReqPortID);
				pDataField[3] = HIBYTE(m_TransReqPortID);
			}
			//对方端口发回来，告知: 目的端口是响应方
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

			// 源端口填自身
			pDataField[4] = LOBYTE(m_wPortID);
			pDataField[5] = HIBYTE(m_wPortID);
			pDataField[6] = 0;
			pDataField[7] = 0;
			m_TxdWritePtr += sizeof(TTransDataHead)-1;

			ReportMsg("Port 0x%x: send trans data 0x0010! from 0x%x to 0x%x>>>>>>>>",\
				m_wPortID,pTDHead->wSrcPort,pTDHead->wDesPort);
			break;
			
		case 0x0100://读数据请求
		case 0x0200://写数据请求
		case 0x0300://测距通道
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
	
	// 1。打开录波索引文件
	dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*DIST_CAPACITY_MAX;
	memset(pFileData,0x00,dwFileLength);
	
	rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwFileLength != dwRtnLength))
	{
		// 索引文件出错
		LogError("CSCanbusPro::RxdReadDistDirReq",FILE_LINE,"Error on open DistList.ind!");
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
			memcpy(m_DistDir.strFileNameList[nCnt],pFileName,DIST_FILENAME_LEN);

			// 记录文件时间
			pFiletime = &m_DistDir.tFileTimeList[nCnt];
			// 转换成系统时标
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
	
	// 发送第一帧
	if(Start == 0)
		byAppCtrl |= 0x80;
			
	// 发送最后一帧
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
			
	//-----------调用数据库接口-----------------
	WritePtr=2;
	
	for(int i=0;i<Num;i++)
	{
		// 文件名
		strFileName = m_DistDir.strFileNameList[Start+i];
		memcpy(pDataField+WritePtr,strFileName,DIST_FILENAME_LEN);
		WritePtr += DIST_FILENAME_LEN;

		// 获取配置文件长度
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
		
		// 获取数据文件长度
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
		
		// 时间
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
		
	//填充待确认信息
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
	
	//如有遥控反校
	if(GetFlag(SF_HaveBOReturn))
	{
		if(TxdBOReturn())
			return TRUE;
	}
	
	//如有变位遥信，则发送
	if((DB_GetBIENum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdBurstBI())
			return TRUE;
	}
	
	//如有SOE，则发送
	if((DB_GetSOENum(&wNum)==DB_ERR_OK)&&(wNum>0))
	{
		if(TxdSoe())
			return TRUE;
	}
	
	
	//如有保护报告，则发送
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
	
	//如有SOE，则发送
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

	//如有变化遥测
	return TxdBurstAI();
}

