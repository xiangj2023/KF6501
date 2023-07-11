/*************************************************************************
* 
*   Copyright (C), 2007-2013, keyvia. Co., Ltd.
* 
*   文件名称：mModRtu.cpp
*   软件模块：Modbus_RTU主站规约
*   版 本 号：1.0
*   生成日期：2013-9-22
*   作    者：L.Z
*   功    能：<...>
* 
*************************************************************************/


/*************************************************************************
* 改动历史纪录：

Revision 1.0, 2013-9-22, L.Z 初始创建

*************************************************************************/
#include "mModRtu.h"
#ifdef INSTALL_MMODRTU


BEGIN_MESSAGE_MAP(CMModRtu,CProtocol)
	ON_SM_COMMIDLE(CMModRtu)
	ON_SM_TIMEOUT(CMModRtu)
	ON_SM_BOSELECT(CMModRtu)
	ON_SM_BOEXEC(CMModRtu)
	ON_SM_BODIRECT(CMModRtu)
END_MESSAGE_MAP()

//=========================================================================================================
//  函数名称: MModRtu_Task                            
//  函数功能: 实现主站类ModbusRTU规约的应用层调度管理任务       
//  输入参数:                                          
//  返回值：无                                         
//=========================================================================================================
void MModRtu_Task(DWORD *pdwApp)
{
    CMModRtu *mModbus=new CMModRtu(pdwApp);
	
	if(mModbus->Init()!=TRUE)
		mModbus->Exit("Mater Modbus Rtu Init Error");
    mModbus->Run();
	return; 
}

BOOL CMModRtu::InitPara()
{
	WORD	value,nFrameNo;
	char 	sec[30];
	int		i;
	
	m_TotalFrm = 0;
	
	if(g_IniParaNum <= 0 || g_IniParaNum > INIPARA_MAX_ITEMNUM)
	{
		return FALSE;
	}

	DB_GetPortName(m_wPortID,sec);

	// 先取出总帧数
	for(i = 0; i< g_IniParaNum; i++)
	{
		if(strcmp( g_IniParaData[i].Section, sec))
			continue;

		if(!strcmp( g_IniParaData[i].ParaName, "totalfrmnum"))
		{
			m_TotalFrm = atoix(g_IniParaData[i].ParaValue);
			break;
		}
	}

	if(m_TotalFrm == 0)
	{
		LogError("InitPara",FILE_LINE,"Init Modbus para error!!!");
		return FALSE;
	}

	m_pFrameList = (TFuncInfo *)OS_MemAlloc(m_TotalFrm*sizeof(TFuncInfo));
		
	// 再逐帧解析
	for (int i = 0; i< g_IniParaNum; i++)
	{
		if(strcmp( g_IniParaData[i].Section, sec))
			continue;
		
		if(!strcmp( g_IniParaData[i].ParaName, "totalfrmnum"))
			continue;

		if(!strcmp( g_IniParaData[i].ParaName, "commidle"))
		{
			m_CommIdle = atoix(g_IniParaData[i].ParaValue);
			continue;
		}
		
		//取出帧号和值
		nFrameNo = atoix(g_IniParaData[i].ParaName)-1;
		value = atoix(g_IniParaData[i].ParaValue);
		if(nFrameNo>=m_TotalFrm)
		{
			LogError("InitPara",FILE_LINE,"Init Modbus frame sequence error!!! Name=%s,FrmNo=%d FrmNum=%d",\
				g_IniParaData[i].ParaName,nFrameNo,m_TotalFrm);
			return FALSE;
		}
		
		if(strstr( g_IniParaData[i].ParaName, "address") != NULL)
		{
			m_pFrameList[nFrameNo].address = value;
			continue;
		}
		
		if(strstr( g_IniParaData[i].ParaName, "datatype") != NULL)
		{
			m_pFrameList[nFrameNo].datatype = value;
			continue;
		}
		
		if(strstr( g_IniParaData[i].ParaName, "funcode") != NULL)
		{
			m_pFrameList[nFrameNo].funcode = value;
			continue;
		}
		
		if(strstr( g_IniParaData[i].ParaName, "regaddr") != NULL)
		{
			m_pFrameList[nFrameNo].regaddr = value;
			continue;
		}
		
		if(strstr( g_IniParaData[i].ParaName, "regnum") != NULL)
		{
			m_pFrameList[nFrameNo].regnum = value;
			continue;
		}
		
		if(strstr( g_IniParaData[i].ParaName, "extmode") != NULL)
		{
			m_pFrameList[nFrameNo].extmode = value;
			continue;
		}

		if(strstr( g_IniParaData[i].ParaName, "datastart") != NULL)
		{
			m_pFrameList[nFrameNo].datastart = value;
			continue;
		}

		if(strstr( g_IniParaData[i].ParaName, "datanum") != NULL)
		{
			m_pFrameList[nFrameNo].datanum = value;
			continue;
		}
	}

	return TRUE;
}

//=========================================================================================================
//  函数名称: CMModRtu::Init         
//  函数功能: 实现主站Modbus规约的初始化 
//  输入参数:                         
//  返回值：无                        
//=========================================================================================================
BOOL CMModRtu::Init(void)
{	
	//延时3秒，防止系统复位后，还未刷新数据，规约便向外发送
	Sleep(3000);

	if(CProtocol::Init() != TRUE)
		return FALSE;
		
	StartMsg(MCC_ReadComm,MIN_RXDFM_SIZE);  //需要通道读消息
	
	m_Txd.ReadPtr=m_Txd.WritePtr=0; //清发送读指针
    m_Txd.Buf=GetTxdBuf();
	m_TxdBuf = m_Txd.Buf;

	if(!InitPara())
	{
		ReportMsg("Port 0x%x: Init Modbus para error!",m_wPortID);
		return FALSE;
	}
			
	// 注册遥控信息
	for(int i=0;i<m_TotalFrm;i++)
	{
		if(m_pFrameList[i].datatype == MOD_DATATYPE_BO)
			DB_RegisterBOMsg(m_pFrameList[i].datastart);
	}

	m_NoRxdCounter = MAX_COMMERR_TIME;

	//创建CAN基本定时器
	SetTimer(TM_1S,1000);

	StartMsg(MCC_CommIdle,m_CommIdle);  //需要通道idle 1S
	
	TxdMonitor();
	
    return TRUE;
}

void CMModRtu::OnCommIdle(DWORD dwLen,DWORD PortID)
{
	TxdMonitor();
}

void CMModRtu::OnTimeOut(DWORD id)
{
	if(m_NoRxdCounter > 0)
	{
		// 倒计时
		m_NoRxdCounter--;

		// 通信失败时报错
		if(m_NoRxdCounter == 0)
			RegisterCommStatus(COMM_STATUS_BREAK);
	}	
}

void CMModRtu::OnBOSelect(WORD no, WORD attr, WORD par)
{
	m_BO.No = no;
	m_BO.Attrib = attr;
	m_BO.Command = PBO_SELECT;
	SetFlag(SF_HaveBO);
}
void CMModRtu::OnBOExec(WORD no, WORD attr, WORD mode)
{
	m_BO.No = no;
	m_BO.Attrib = attr;
	m_BO.Command = PBO_EXECUTE;
	SetFlag(SF_HaveBO);
}
void CMModRtu::OnBODirect(WORD no, WORD attr, WORD par)
{
	m_BO.No = no;
	m_BO.Attrib = attr;
	m_BO.Command = PBO_DIRECT;
	SetFlag(SF_HaveBO);
}

WORD CMModRtu::Crc16(BYTE * pBuf, BYTE Len)
{
	WORD	wTmp,wRtn;
	
	wRtn = 0xFFFF;
	
	for(int j=0; j<Len; j++)
	{
		wTmp = pBuf[j];
		wTmp <<= 1;
		
		for(int i=8; i>0; i--)
		{
			wTmp >>= 1;
			if((wTmp ^ wRtn) & 1)
				wRtn = (wRtn >> 1)^0xa001;
			else
				wRtn >>= 1;
		}
	}
	
	return	wRtn;
}

// 获取遥控帧
BOOL CMModRtu::GetBOFrame(WORD nBO)
{
	for(int i=0;i<m_TotalFrm;i++)
	{
		if(m_pFrameList[i].datatype != MOD_DATATYPE_BO)
			continue;

		if(m_pFrameList[i].datastart != nBO)
			continue;

		m_BOInfoNo = i;
		return TRUE;
	}

	return FALSE;
}

// 切换至下一轮询帧
BOOL CMModRtu::SwitchToNextFrame()
{
	for(int i=0;i<m_TotalFrm;i++)
	{
		if(m_pFrameList[m_PollCnt].datatype != MOD_DATATYPE_BO)
			return TRUE;

		m_PollCnt = (m_PollCnt+1)%m_TotalFrm;
	}

	// 未找到
	return FALSE;
}


BOOL CMModRtu::TxdMonitor(void)
{
	if(CheckClearFlag(SF_HaveBO))
	{
		// 遥控序号合法性检查
		if(GetBOFrame(m_BO.No))
		{
			if(m_BO.Command == PBO_SELECT)
			{
				// 预置直接返校
	    		DB_BOSelectEcho(m_BO.No,m_BO.Attrib,0);  
			}
			else 
				return Txd_Frame(&m_pFrameList[m_BOInfoNo]);
		}
		else
			LogError("CMModRtu::TxdMonitor",FILE_LINE,"BO no=%d is invalid!!!",m_BO.No);
	}

	// 轮询
	if(SwitchToNextFrame())
	{
		Txd_Frame(&m_pFrameList[m_PollCnt]);//查询任务
		m_PollCnt = (m_PollCnt+1)%m_TotalFrm;
	}
	
	return TRUE;
}

//=====================================================================================
//  函数功能: 检查Buf处是否是一个有效报文
//  输入参数: Buf:要检查的报文起始地址
//            Len:Buf的有效长度
//  返回值：  
//           FM_OK  +实际报文长度   //检测到一个完整的帧
//           FM_ERR +已处理字节数   //检测到一个校验错误的帧
//           FM_LESS+已处理字节数   //检测到一个不完整的帧（还未收齐）

//  注:已处理字节数若为0则下次搜索从下一地址开始，否则下次搜索将跳过已处理过的字节，从而加快搜索速度
//=====================================================================================
DWORD CMModRtu::SearchOneFrame(PBYTE pBuf,WORD wLength)
{
	BYTE	FrmLength;
	WORD	wCrc,wRenCrc;
	
	if(wLength < MIN_RXDFM_SIZE)
	{
		ReportMsg("CMModRtu::SearchOneFrame 1");
		return FM_LESS;
	}
	
	if(pBuf[0] != m_pLastFunc->address)
	{
		ReportMsg("CMModRtu::SearchOneFrame 2");
		return FM_ERR|1;
	}
	
	if(pBuf[1]&0x80)
	{
		FrmLength=3;
	}
	else
	{
	    switch(pBuf[1])
	    {
			case 1:
			case 2:
			case 3:
			case 4:
				FrmLength=pBuf[2]+3;
				break;
			case 5:
			case 6:
				FrmLength=6;
				break;
			case 16:
				FrmLength=7+pBuf[6];
				break;
			case 20:
				FrmLength=3+pBuf[2];
				break;
			default:
				ReportMsg("CMModRtu::SearchOneFrame 3");
				return FM_ERR|1;
	    }
	}
	
	if(wLength < (FrmLength+2))
	{
		ReportMsg("CMModRtu::SearchOneFrame 4");
		return FM_LESS;
	}
	
	wCrc = MAKEWORD(pBuf[FrmLength+0],pBuf[FrmLength+1]);
	wRenCrc=Crc16(pBuf,FrmLength);
	if(wRenCrc!= wCrc)
	{
		ReportMsg("CMModRtu::SearchOneFrame 5");
		return FM_ERR|1;
	}
	
	return FM_OK|(FrmLength+2);
}

//=====================================================================================
//  函数功能: 接收处理
//  返回值：  无                        
//=====================================================================================
void CMModRtu::OnFrame(BYTE* pBuf)
{
	BYTE	nDataLen;
	
	m_NoRxdCounter = MAX_COMMERR_TIME;
	if(GetFlag(SF_CommErr))
		RegisterCommStatus(COMM_STATUS_OK);
		
	if(pBuf[1] != m_pLastFunc->funcode)
	{
		LogError("OnFrame",FILE_LINE,"Recieve code(%d) != Expected code(%d)",pBuf[1],m_pLastFunc->funcode);
		return;
	}
		
    // 处理接收
	switch( pBuf[1]&0x7F )
	{
		case 1:
		case 2:
			nDataLen = pBuf[2];
			if(nDataLen != ((m_pLastFunc->regnum+7)/8) )
			{
				LogError("OnFrame",FILE_LINE,"Recieve len(%d) != Expected len(%d)",pBuf[2],(m_pLastFunc->regnum+7)/8);
				return;
			}
			
			Rxd_Func0102(pBuf);
			break;
			
		case 3:
		case 4:
			nDataLen = pBuf[2];
			if(nDataLen != (m_pLastFunc->regnum*2) )
			{
				LogError("OnFrame",FILE_LINE,"Recieve len(%d) != Expected len(%d)",pBuf[2],m_pLastFunc->regnum*2);
				return;
			}
			
			Rxd_Func0304(pBuf);
			break;
			
		case 5:
		case 6:
			Rxd_Func0506(pBuf);
			break;
		
		default:
			break;
	}
	
	return;
}

BOOL CMModRtu::Txd_Frame(TFuncInfo 	*pFrame)
{
	WORD	CRC;
	
	m_Txd.Buf[0] = pFrame->address;
	m_Txd.Buf[1] = pFrame->funcode;
	m_Txd.Buf[2] = HIBYTE(pFrame->regaddr);
	m_Txd.Buf[3] = LOBYTE(pFrame->regaddr);
	m_Txd.Buf[4] = HIBYTE(pFrame->regnum);	
	m_Txd.Buf[5] = LOBYTE(pFrame->regnum);
	
	CRC = Crc16(m_Txd.Buf, 6);
	m_Txd.Buf[6] = LOBYTE(CRC);
	m_Txd.Buf[7] = HIBYTE(CRC);

	WriteComm(8, m_Txd.Buf[0], 0);

	m_pLastFunc = pFrame;

	return TRUE;
}


BOOL CMModRtu::Rxd_Func0102(BYTE *pBuf)
{
	if(m_pLastFunc->datatype != MOD_DATATYPE_BI)
	{
		LogError("Rxd_Func0102",FILE_LINE,"FunCode=%d is not BI!!!",pBuf[1]);
		return FALSE;
	}
	
	// 解析遥信
	for(int i=0;i<m_pLastFunc->datanum;i++)
	{
		if(pBuf[3+(i/8)]&(0x01<<(i%8)))
		{
			DB_WriteBI(m_pLastFunc->datastart+i, BI_CLOSE);
		}
		else
		{
			DB_WriteBI(m_pLastFunc->datastart+i, BI_OPEN);
		}
	}

	return TRUE;
}

BOOL CMModRtu::Rxd_Func0304(BYTE *pBuf)
{
	short	sValue;
	long	lValue;
	DWORD	dwValue;
	
	switch(m_pLastFunc->datatype)
	{
		// 解析遥测
		case MOD_DATATYPE_AI:
			for(int i=0;i<m_pLastFunc->datanum;i++)
			{
				if(m_pLastFunc->extmode == MOD_EXTMODE_WORD)
				{
					sValue = MAKEWORD(pBuf[3+i*2+1],pBuf[3+i*2]);
					lValue = sValue;
				}
				if(m_pLastFunc->extmode == MOD_EXTMODE_DWORD)
				{
					lValue = MAKEDWORD(pBuf[3+i*4+3],pBuf[3+i*4+2],pBuf[3+i*4+1],pBuf[3+i*4]);
				}
					
				DB_WriteAI(m_pLastFunc->datastart+i, lValue);
			}
			break;
		// 解析电度
		case MOD_DATATYPE_CI:
			for(int i=0;i<m_pLastFunc->datanum;i++)
			{
				if(m_pLastFunc->extmode == MOD_EXTMODE_WORD)
				{
					sValue = MAKEWORD(pBuf[3+i*2+1],pBuf[3+i*2]);
					dwValue = sValue;
				}
				if(m_pLastFunc->extmode == MOD_EXTMODE_DWORD)
				{
					dwValue = MAKEDWORD(pBuf[3+i*4+3],pBuf[3+i*4+2],pBuf[3+i*4+1],pBuf[3+i*4]);
				}
					
				DB_WriteCI(m_pLastFunc->datastart+i, dwValue);
			}
			break;
		// 解析遥信
		case MOD_DATATYPE_BI:
			for(int i=0;i<m_pLastFunc->datanum;i++)
			{
				if(m_pLastFunc->extmode == MOD_EXTMODE_BIT)
				{
					if(pBuf[3+(i/8)]&(0x01<<(i%8)))
					{
						DB_WriteBI(m_pLastFunc->datastart+i, BI_CLOSE);
					}
					else
					{
						DB_WriteBI(m_pLastFunc->datastart+i, BI_OPEN);
					}
				}
				if(m_pLastFunc->extmode == MOD_EXTMODE_WORD)
				{
					dwValue = MAKEWORD(pBuf[3+i*2+1],pBuf[3+i*2]);
					if(dwValue > 0)
					{
						DB_WriteBI(m_pLastFunc->datastart+i, BI_CLOSE);
					}
					else
					{
						DB_WriteBI(m_pLastFunc->datastart+i, BI_OPEN);
					}
				}
				if(m_pLastFunc->extmode == MOD_EXTMODE_BYTE)
				{
					ReportMsg("Rxd_Func0304: Write BI no=%d Value=%d",m_pLastFunc->datastart+i,pBuf[3+i]);
					if(pBuf[3+i] > 0)
						DB_WriteBI(m_pLastFunc->datastart+i, BI_CLOSE);
					else
						DB_WriteBI(m_pLastFunc->datastart+i, BI_OPEN);
				}
			}
			
	}
}

BOOL CMModRtu::Rxd_Func0506(BYTE *pBuf)
{
	// 解析遥控
	if((m_BO.Command != PBO_EXECUTE)&&(m_BO.Command != PBO_DIRECT))
		return FALSE;
		
	return DB_BOSelectEcho(m_BO.No,m_BO.Attrib,0);
}

#endif
