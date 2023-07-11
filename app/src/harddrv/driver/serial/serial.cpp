/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			serial.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           串口类基本功能                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      实现串口类基本功能，通过串口管理任务完成对串口数据的管理和控制                          
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	CSerial				串口类
*      	MainTask			串口类主任务，完成串口收发数据的管理                                                              
*      	CheckTxdTask	   	发送检查任务                                                                         
*      	DetectFrameTailTask	接收帧结束检查                                                                        
*      	GetLeftRxdNum    	取串口类接收缓冲区字符数                                                                       
*      	NotifyTask		 	检查各个串口通道的通信情况，并并根据情况通知对应的应用层任务                                                                       
*      	MyTimerTask		    串口类内部定时处理过程                                                                      
*      	MySetTimer		   	串口类内部定时器创建                                                                        
*      	MyKillTimer			串口类内部定时器删除                                                                      
*      	MyOnTimer       	串口类内部定时处理，主要完成串口控制信号的软件定时控制操作                                                                     
*      	GetCanSendNum       读串口通道发送缓冲区可以向串口物理通道发送的字节个数                                                                     
*		SetWriteMask		累加发送空闲及串口通道通信空闲计数
*		ClearWriteMask		清除发送空闲及串口通道通信空闲计数
*		SetReadMask			累加接收空闲及串口通道通信空闲计数
*		ClearReadMask		清除接收空闲及串口通道通信空闲计数
*		ClearFameTail		清接收帧尾判断标志
*		GetCanWriteNum		取可以向串口通道发送缓冲区写入的字节个数
*		NotifyToUse			消息通知处理
*		IsOpen				取串口通道打开标志
*		OpenComm			串口通道打开
*		SerialControl		设置串口控制信息
*		InitPara			初始化串口类通信参数，如串口类型、方向控制等
*		GetBaud				根据通信速率序号获取串口通信速率，并计算接收结束最大时间长度
*		GetRtsDelay			根据参数计算RTS方向控制延时时间
*		SerialTxdCtr		串口通道发送控制
*		ReadData			读取串口通道接收缓冲区内接收到的指定长度数据，一般供上层应用调用
*		WriteData			向串口通道发送缓冲区写入一定长度的数据，一般供上层应用调用
*		WriteToBuf			向串口通道接收缓冲区写入一定长度的数据，一般供底层驱动调用
*		ReadFromBuf			从串口通道发送缓冲区读取一定长度的数据，一般供底层驱动调用
*		WriteAChar			向对应串口物理通道写入一个字节的发送数据，一般供串口驱动调用以触发发送中断
*		ResetSerial			根据标志复位串口通道类，可能为发送缓冲区、接收缓冲区或整个串口通道
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/07/02    初始创建                         
*                                                                                  
********************************************************************************************/

#include "serial.h"
#include "ttydrv.h"
#include "misi.h"


CSerial* CSerial::m_pSerialMap[MAX_SERIAL_NUM];
 
 /*********************************************************************************************
  *
  * 功能描述	 串口类初始化
  *
  * 参数说明	  
  *
  * 返回代码
  * 			   
  *
  * 其它说明：			
  *
 **********************************************************************************************/
CSerial::CSerial()
{
	//初始化缓冲区指针
	m_wRxdWritePtr = 0;
	m_wRxdReadPtr = 0;
	m_wTxdWritePtr = 0;
	m_wTxdReadPtr = 0;
	m_bRxdTail = 0;
		
	m_dwNoRxdTime = 0;
	m_dwBaud = 1200;
//	m_wPortType = 0;
	
	m_bTxdMode = 0;
	m_bRxdMode = 0;
	m_bTxdOnOff = FALSE;
	m_dwNoRecive = 0;
	m_dwNoSend = 0; 
//	m_wResetCnt = 0;
	
	timerStatus = NONEED;
	
	m_dwFlag = 0;
		
	m_dwMaxNoRxdTime = 100/SERIAL_TASK_TIMER;
		
	if(m_dwMaxNoRxdTime == 0)
		m_dwMaxNoRxdTime = 1;
	
	//	memset((void*)&m_serialPara, 0, sizeof(pSerial->m_serialPara));
	memset((void*)&m_CommCtrl, 0, sizeof(m_CommCtrl));
	memset((void*)&m_PortInfo, 0, sizeof(m_PortInfo));
		
		
	m_bOpend = FALSE;
	
	m_TxdBuf = NULL;
	m_RxdBuf = NULL;
	m_wMaxTxdLen = MISITXDBUFLEN;
	m_wMaxRxdLen = MISIRXDBUFLEN;
		
	m_TxdBuf = (BYTE *)OS_MemAlloc(m_wMaxTxdLen);
	m_RxdBuf = (BYTE *)OS_MemAlloc(m_wMaxRxdLen);
	
	m_CommCtrl.dwNotifyRead = m_wMaxRxdLen/2;////读通知字符个数
	
	m_bWriteContinue = FALSE;
	m_bNotify = FALSE;
	
	m_bNeedMessage = FALSE;
	
	m_dwNoReadCount = 0;

	m_dwFECnt = 0;
	m_dwPECnt = 0;
	m_dwOECnt =0;

	m_dwCounter = 0;
}

CSerial::~CSerial()
{
}

/*********************************************************************************************
 *
 * 功能描述      串口类主任务，完成串口收发数据的管理
 *
 * 参数说明      
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::MainTask()
{
	//检查该串口是否打开
	if(m_bOpend == FALSE)
		return;
	
	CheckTxdTask();
	
	MyTimerTask();////定时任务
	DetectFrameTailTask();	
	NotifyTask();//////监测读、写空闲时间

	m_dwCounter++;

//	if((m_dwCounter%100)==0)
//		ReportMsg("The Misi task counter=%d. ", m_dwCounter);
}

/*********************************************************************************************
 *
 * 功能描述      发送检查任务，当发送缓冲区有数据时，在对应串口允许的情况下触发发送；当需要软件
 *				 方式控制RTS信号时完成RTS信号的带延时控制
 *
 * 参数说明      
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::CheckTxdTask()
{
	WORD 	wTxdNum;
    
	wTxdNum = GetCanSendNum();

	if(wTxdNum)
	{
		if(m_bTxdOnOff == FALSE)
		{
//			m_wResetCnt = 0;
			if(m_bNeedCtrlRTS)
			{
				if(timerStatus == NONEED)
				{
				    if(m_wRTSNoDelay)
				    {
    				    if(m_bRTSLogicHigh)
        					//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, FALSE);
						 	SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
        				else
    					    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, TRUE);
							SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
    			    }
    			    else
    			    {
    				    if(m_bRTSLogicHigh)
        					//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, FALSE);
							SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
        				else
    					    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS, TRUE);
							SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
                    }    			    
					MySetTimer(RTSTIMERID, m_wRTSOnDelay);
					timerStatus = WAITENABLETXD;
				}//pSerial->timerStatus = == NONEED		
			}
			else
				SerialTxdCtr(TRUE);/////写一个字符产生中断
		}//m_bTxdOnOff == FALSE
		else
		{
/*		注意此处的处理方式，当发送缓冲区有数据但是长时间不能获取串口发送资源时			
			m_wResetCnt++;
			if(m_wResetCnt>=(10000/SERIAL_TASK_TIMER))
			{
				BYTE bIntReg;
				bIntReg = inportb(0x83000c00+0x01);	
				
//				SerialResetSerialBase(pSerial);
//				SerialInit();
				SerialOpen(m_dwPort&0x000F);
				SerialRxdCtr(m_dwPort&0x000F, TRUE);
//				SerialTxdCtr(m_dwPort&0x000F, TRUE);
				m_bTxdOnOff = FALSE;
			}
*/			
			if(m_bNeedCtrlRTS)////当在等待关闭RTS时，又有数据发送应当关闭定时器
			{
				if(timerStatus == WAITOFFRTS)
					MyKillTimer(RTSTIMERID);
			}
			
		}
	}
	else
	{
		if(m_bNeedCtrlRTS)
		{
		    BOOL bRtsL = FALSE;
		
		    bRtsL = (m_bTxdMode & MODEM_RTS);
		    if(m_bRTSLogicHigh)
		        bRtsL = !bRtsL;
			if((timerStatus == NONEED) && bRtsL && (IsTxdEnd() == TRUE))
			{
				MySetTimer(RTSTIMERID, m_wRTSOffDelay-1);
				timerStatus = WAITOFFRTS;
			}
		}
	}	
}

/*********************************************************************************************
 *
 * 功能描述      接收帧结束检查
 *
 *
 * 参数说明      
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial:: DetectFrameTailTask()
{
	BOOL bRxdTail;

	m_dwNoRxdTime++;

	bRxdTail = (m_dwNoRxdTime >= m_dwMaxNoRxdTime);
	bRxdTail &= (GetLeftRxdNum() > 0);

	m_bRxdTail = bRxdTail;
}


/*********************************************************************************************
 *
 * 功能描述      取串口类接收缓冲区字符数
 *
 *
 * 参数说明      
 *       		 
 *
 * 返回代码
 *                字符个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::GetLeftRxdNum()/////规约可以从缓冲区读的字符数
{
	WORD wLen;
	WORD wRxdWritePtr;
	WORD wRxdReadPtr;
	
//	OS_Protect(0);
	wRxdWritePtr = m_wRxdWritePtr;	/////资源保护,防止在进行下列操作时，指针变化
	wRxdReadPtr = m_wRxdReadPtr;	///////
//	OS_UnProtect(0);

	if(wRxdWritePtr >= wRxdReadPtr)
		wLen = wRxdWritePtr - wRxdReadPtr;
	else
		wLen = m_wMaxRxdLen - (wRxdReadPtr-wRxdWritePtr);

	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      检查各个串口通道的通信情况，并并根据情况通知对应的应用层任务。
 *
 *
 * 参数说明      
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::NotifyTask()
{
	BOOL bTxdBreak;
	BOOL bCommBreak;
	BOOL bWriteNotify;
	BOOL bNotify;
	BOOL bReadNotify;
	DWORD dwTime;
	DWORD dwFlag;
	
	if(m_bNeedMessage == FALSE)//////不需要通知任务消息
		return;
	
	SetReadMask();/////读通道空闲时间++
	SetWriteMask();////写通道空闲时间++

	if(m_bWriteContinue && GetCanWriteNum())
	{
		m_bWriteContinue = FALSE;
		NotifyToUse(SM_WRITECONT, 0, 0);
	}

	bTxdBreak = (m_CommCtrl.dwNotifyTxdBreak > 0);
	dwTime = notifyCount.dwNotifyTxdBreak * SERIAL_TASK_TIMER;
	bTxdBreak &= (dwTime >= m_CommCtrl.dwNotifyTxdBreak);

	bCommBreak = (m_CommCtrl.dwNotifyCommBreak > 0);
	dwTime = notifyCount.dwNotifyCommBreak * SERIAL_TASK_TIMER/2;
	bCommBreak &= (dwTime >= m_CommCtrl.dwNotifyCommBreak);

	bWriteNotify = (m_CommCtrl.dwNotifyWrite > 0);
	bWriteNotify &= (GetCanSendNum() <= m_CommCtrl.dwNotifyWrite);

	bReadNotify = (m_CommCtrl.dwNotifyRead > 0);
	bReadNotify &= (GetLeftRxdNum() >= m_CommCtrl.dwNotifyRead);
	bReadNotify |= m_bRxdTail;///帧尾

	bNotify = (bTxdBreak | bCommBreak | bWriteNotify | bReadNotify);
	
	if(!bNotify)
		return;

	if(m_bNotify)
	{
		if(bReadNotify)
		{
			
			if(m_bRxdTail)
				dwFlag = RCF_FRAME_TAIL;
			else
				dwFlag = 0;

			//GPRS收到数据时,清除掉线计数
			NotifyToUse(SM_READCOMM, 0, dwFlag);
			ClearFameTail();
		}
		else if(bCommBreak)
		{
			NotifyToUse(SM_COMMIDLE, 0, 0);
			ClearReadMask();
			notifyCount.dwNotifyCommBreak = 0;
		}
		else if(bTxdBreak)
		{
			NotifyToUse(SM_COMMTXDIDLE, 0, 0);
			notifyCount.dwNotifyTxdBreak = 0;
		}
		else if(bWriteNotify)
		{
			NotifyToUse(SM_WRITECOMM, 0, 0);
		}
		
	}
	else
	{
		m_dwNoReadCount++;

		ClearReadMask();
		ClearWriteMask();
		ClearFameTail();

//		if(m_dwNoReadCount >= NOREADCOUNT)
		{
			m_dwNoReadCount = 0;
			m_bNotify = TRUE;
		}
	}
}

/*********************************************************************************************
 *
 * 功能描述      串口类内部定时处理过程
 *
 *
 * 参数说明      
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::MyTimerTask()
{
	BYTE i;
	for(i = 0; i < TIMERSUM; i++)
	{
		if(timeMsg[i].bTimer)
		{
			timeMsg[i].wValue --;

			if(timeMsg[i].wValue == 0)
			{
				timeMsg[i].wValue = timeMsg[i].wTick;
				MyOnTimer(i+1);
			}
		}
	}
}

/*********************************************************************************************
 *
 * 功能描述      串口类内部定时器创建
 *
 *
 * 参数说明      - bTimerID	: 输入	串口类内部定时器ID  
 *       		 - offset	: 输入	定时时间间隔(单位毫秒)
 *       		 
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL CSerial::MySetTimer(BYTE bTimerID, WORD wMSecond)
{
	if(bTimerID > TIMERSUM || bTimerID == 0)
		return FALSE;

	timeMsg[bTimerID-1].bTimer = TRUE;
	timeMsg[bTimerID-1].wTick = wMSecond/SERIAL_TASK_TIMER;
//	if(pSerial->timeMsg[bTimerID-1].wTick == 0)
//		pSerial->timeMsg[bTimerID-1].wTick = 1;
	//注意先启动定时器，马上检测定时器，因此第一遍检测无意义，必须增加一个计数值才能与期望时间一致
	timeMsg[bTimerID-1].wTick += 1;
	timeMsg[bTimerID-1].wValue = timeMsg[bTimerID-1].wTick;

	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述      串口类内部定时器删除
 *
 *
 * 参数说明      - bTimerID	: 输入	串口类内部定时器ID  
 *       		 
 *
 * 返回代码
 *                TRUE或FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL CSerial::MyKillTimer(BYTE bTimerID)
{
	if(bTimerID > TIMERSUM || bTimerID == 0)
		return FALSE;

	timeMsg[bTimerID-1].bTimer = FALSE;
	timeMsg[bTimerID-1].wTick = 0;
	timeMsg[bTimerID-1].wValue = 0;

	return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述      串口类内部定时处理，主要完成串口控制信号的软件定时控制
 *
 *
 * 参数说明      - bTimerID	: 输入	串口类内部定时器ID  
 *       		 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::MyOnTimer(BYTE bTimerID)
{
	MyKillTimer(bTimerID);

	if(bTimerID == RTSTIMERID)
	{

		if(timerStatus == WAITENABLETXD)
			SerialTxdCtr(TRUE);

		if(timerStatus == WAITOFFRTS)
		{
		    if(m_wRTSNoDelay)
		    {
    		    if(m_bRTSLogicHigh)
    				//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,TRUE);
					SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
    			else
    			    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,FALSE);
					SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
    	    }
    	    else
    	    {
/*      	    //原来的是否有问题
    		    if(m_bRTSLogicHigh)
    				SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,FALSE);
    			else
    			    SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,TRUE);
*/
 			    
    		    if(m_bRTSLogicHigh)
    				//SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,TRUE);
					SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)TRUE);
    			else
    			    //SetModemStatus(MINOR_PORT(m_dwPort), MODEM_RTS,FALSE );
				SerialIOCtl(MINOR_PORT(m_dwPort), MODEM_SET, (DWORD *)MODEM_RTS, (DWORD *)FALSE);
    	    }
    	    m_dwFlag = 0;
	    }
	}

	timerStatus = NONEED;
}

/*********************************************************************************************
 *
 * 功能描述      读串口通道发送缓冲区可以向串口物理通道发送的字节个数
 *
 *
 * 参数说明       
 *       		 
 *
 * 返回代码
 *                允许发送的字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::GetCanSendNum()/////写缓冲区中字符个数,
{
	WORD wLen;
	WORD wTxdWritePtr;
	WORD wTxdReadPtr;
	
	//临界资源保护(中断中不操作)
//	OS_Protect(0);
	wTxdWritePtr = m_wTxdWritePtr;
	wTxdReadPtr = m_wTxdReadPtr;
//	OS_UnProtect(0);

	wLen = (wTxdWritePtr + m_wMaxTxdLen - wTxdReadPtr)%m_wMaxTxdLen;

	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      累加发送空闲及串口通道通信空闲计数
 *
 * 参数说明       
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::SetWriteMask()
{
	notifyCount.dwNotifyTxdBreak ++;
	notifyCount.dwNotifyCommBreak ++;
}

/*********************************************************************************************
 *
 * 功能描述      清除发送空闲及串口通道通信空闲计数
 *
 * 参数说明       
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::ClearWriteMask()
{
	notifyCount.dwNotifyTxdBreak = 0;
	notifyCount.dwNotifyCommBreak = 0;
}

/*********************************************************************************************
 *
 * 功能描述      累加接收空闲及串口通道通信空闲计数
 *
 * 参数说明       
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::SetReadMask()
{
	notifyCount.dwNotifyRxdBreak ++;
	notifyCount.dwNotifyCommBreak ++;
}

/*********************************************************************************************
 *
 * 功能描述      清除接收空闲及串口通道通信空闲计数
 *
 * 参数说明       
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::ClearReadMask()
{
	notifyCount.dwNotifyRxdBreak = 0;
	notifyCount.dwNotifyCommBreak = 0;
}

/*********************************************************************************************
 *
 * 功能描述      清接收帧尾判断标志
 *
 * 参数说明       
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::ClearFameTail()
{
	m_dwNoRxdTime = 0;
	m_bRxdTail = FALSE;
}

/*********************************************************************************************
 *
 * 功能描述      取可以向串口通道发送缓冲区写入的字节个数
 *
 * 参数说明       
 *
 * 返回代码
 *                可以写入的字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::GetCanWriteNum()
{
	WORD wLen;
	WORD wTxdWritePtr;
	WORD wTxdReadPtr;

	//临界资源保护(中断中不操作)注意该函数会在中断中调用，因此不能用OS_Protect进行保护
//	OS_Protect(0);
	wTxdWritePtr = m_wTxdWritePtr;
	wTxdReadPtr = m_wTxdReadPtr;
//	OS_UnProtect(0);

	wLen = (m_wMaxTxdLen + wTxdReadPtr - wTxdWritePtr)%m_wMaxTxdLen;

	if(wLen == 0)
		return m_wMaxTxdLen-1;
	return wLen-1;
}

/*********************************************************************************************
 *
 * 功能描述      消息通知处理
 *
 * 参数说明      - wMsg	: 	输入	消息类型  
 *				 - wLen	: 	输入	
 *				 - dwFlag:	输入	标志信息
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS CSerial::NotifyToUse(WORD wMsg, WORD wLen, DWORD dwFlag)
{
	if(m_dwTagetID == 0)
		return ERR_FAILURE;

	m_dwNoReadCount = 0;
	m_bNotify = FALSE;
	
	wLen = 0;
	dwFlag = 0;
	
//	if(pSerial->m_TagetID)
//		OS_PostMessage(pSerial->m_TagetID, wMsg, MAKELONG(wLen, pSerial->m_wPortID), dwFlag,0, 0);
	if(m_dwTagetID)
		OS_PostMessage(m_dwTagetID, wMsg, wLen, m_dwPort, dwFlag, 0);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述      取串口通道打开标志
 *
 * 参数说明      
 *
 * 返回代码
 *                串口打开状态
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL CSerial::IsOpen()
{
	return m_bOpend;
}

/*********************************************************************************************
 *
 * 功能描述      串口通道打开
 *
 * 参数说明      - wRxdLen	: 	输入	应用层接收缓冲区长度
 *				 - wTxdLen	: 	输入	应用层发送缓冲区长度	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS CSerial::OpenComm(WORD wRxdLen, WORD wTxdLen)
{
//	DWORD dwVarName;
	WORD wPortNo;
	
	InitPara();

//	dwVarName = pSerial->m_dwPort&0xFFFF0000;
//	dwVarName += ((pSerial->m_dwPort&0xFF)<<8);

	if(wRxdLen == 0 || wRxdLen > MISIRXDBUFLEN)
	{
		m_bOpend = FALSE;
		return ERR_FAILURE;
	}
	if(wTxdLen == 0 || wTxdLen > MISITXDBUFLEN)
	{
		m_bOpend = FALSE;
		return ERR_FAILURE;
	}

	wPortNo = MINOR_PORT(m_dwPort);

	SerialOpen(wPortNo);
	SerialRxdCtr(wPortNo, TRUE);
	::SerialTxdCtr(wPortNo, TRUE);

	OS_Sleep(10);
	
	m_bOpend = TRUE;

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述      设置串口控制信息
 *
 * 参数说明      - pCtrl	: 	输入	控制信息	
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS CSerial::SerialControl(TCtlComm* pCtrl)
{
	if(pCtrl == NULL)
		return ERR_FAILURE;
	
	if(pCtrl->dwFuncCode == FC_GET)
	{
		*pCtrl = m_CommCtrl;
		return ERR_SUCCESS;
	}
	
	if(pCtrl->dwFuncCode != FC_SET)
		return ERR_FAILURE;
	
	m_bNeedMessage = TRUE;
	
	m_CommCtrl = *pCtrl;

	if(m_CommCtrl.dwNotifyWrite > m_wMaxTxdLen)
		m_CommCtrl.dwNotifyWrite = m_wMaxTxdLen;

	if(m_CommCtrl.dwNotifyRead > m_wMaxRxdLen)
		m_CommCtrl.dwNotifyRead = m_wMaxRxdLen/2;
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述      初始化串口类通信参数，如串口类型、方向控制等
 *
 * 参数说明      
 *
 * 返回代码
 *                
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::InitPara()
{
	BYTE bMedium;

	GetBaud();
	GetRtsDelay();

	bMedium = m_serialPara.bFlag;

	//对于485方式采用硬件自动控制RTS方式
	if(bMedium & SERIAL_ATTR_RTSCTR)
		m_bNeedCtrlRTS = TRUE;
	else
		m_bNeedCtrlRTS = FALSE;
	
	if(bMedium & SERIAL_ATTR_RTSDELAY)
	    m_wRTSNoDelay = TRUE;
	else
	    m_wRTSNoDelay = FALSE;
	
	m_bRTSLogicHigh = FALSE;
    
}

/*********************************************************************************************
 *
 * 功能描述      根据通信速率序号获取串口通信速率，并计算接收结束最大时间长度
 *
 * 参数说明      
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::GetBaud()
{
	DWORD dwOneTime;

	NumToBaud((WORD)m_serialPara.byBaud, &m_dwBaud);
	dwOneTime = 10*1000/m_dwBaud;

	m_dwMaxNoRxdTime = NOHCARNUM*dwOneTime/SERIAL_TASK_TIMER;

	if(m_dwMaxNoRxdTime < 2)
		m_dwMaxNoRxdTime = 2;
}

/*********************************************************************************************
 *
 * 功能描述      根据参数计算RTS方向控制延时时间
 *
 * 参数说明      
 *
 * 返回代码
 *               无 
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::GetRtsDelay()
{
	m_wRTSOnDelay = m_serialPara.byRTSOnDelay;
	m_wRTSOffDelay = m_serialPara.byRTSOffDelay;

	m_wRTSOnDelay *= 10;
	m_wRTSOffDelay *= 10;
	
	//最小必须为串口管理定时器的定时间隔
	//if(m_wRTSOnDelay < SERIAL_TASK_TIMER)
		m_wRTSOnDelay = SERIAL_TASK_TIMER;
	//if(m_wRTSOffDelay < SERIAL_TASK_TIMER)
		m_wRTSOffDelay = SERIAL_TASK_TIMER;
}

/*********************************************************************************************
 *
 * 功能描述      串口通道发送控制
 *
 * 参数说明      - bOnOff	: 	输入	控制信息	
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::SerialTxdCtr(BOOL bOnOff)
{
	if(::SerialTxdCtr(MINOR_PORT(m_dwPort), bOnOff) == ERR_SUCCESS)
		m_bTxdOnOff = bOnOff;
		
}

/*********************************************************************************************
 *
 * 功能描述      读取串口通道接收缓冲区内接收到的指定长度数据，一般供上层应用调用
 *
 * 参数说明      - pBuf		: 	输出	需要保存的读取到的数据	
 *				 - wLen		:	输入	准备读取的数据长度
 *				 - dwFlag	:	输入	标志
 *
 * 返回代码
 *                实际读取到的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::ReadData(BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	WORD wLeft;
	WORD i;
	
	if(!m_bOpend)
		return 0;

	m_bNotify = TRUE;
	m_dwNoReadCount = 0;

	wLeft = GetLeftRxdNum();

	if(wLen<wLeft)
		wLeft = wLen;

	if(wLeft == 0 || pBuf == NULL)
		return 0;

//	OS_Protect(0);
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_RxdBuf[m_wRxdReadPtr];
		m_wRxdReadPtr +=1;
		m_wRxdReadPtr %= m_wMaxRxdLen;
	}
//	OS_UnProtect(0);

	return wLeft;
}

/*********************************************************************************************
 *
 * 功能描述      向串口通道发送缓冲区写入一定长度的数据，一般供上层应用调用
 *
 * 参数说明      - pBuf		: 	输出	需要写入的数据	
 *				 - wLen		:	输入	准备写入的数据长度
 *				 - dwFlag	:	输入	标志
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::WriteData(BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	WORD wLeft;
	WORD i;
	
	if(!m_bOpend)
		return 0;

	m_bWriteContinue = FALSE;
	m_dwNoReadCount = 0;
	m_bNotify = TRUE;
	notifyCount.dwNotifyCommBreak = 0;
	
	m_dwFlag = dwFlag;		//应用层数据包是否一帧结束
	
	wLeft = GetCanWriteNum();

	if(wLen > wLeft)
	{
		wLen = wLeft;
		m_bWriteContinue = TRUE;
	}

	if(wLen == 0 || pBuf == NULL)
		return 0;

	OS_Protect(0);
	for(i = 0; i< wLen; i++)
	{
		m_TxdBuf[m_wTxdWritePtr] = pBuf[i];
		m_wTxdWritePtr +=1;
		m_wTxdWritePtr %= m_wMaxTxdLen;
	}
	OS_UnProtect(0);

	m_bTxdOnOff = FALSE;
		
	return wLeft;
}

/*********************************************************************************************
 *
 * 功能描述      向串口通道接收缓冲区写入一定长度的数据，一般供底层驱动调用
 *
 * 参数说明      - pBuf	: 	输出	需要写入的数据	
 *				 - wLen :	输入	准备写入的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::WriteToBuf(BYTE* pBuf, WORD wLen)
{
	DWORD dwRxdWritePtr;
	WORD i;

	ClearReadMask();
	ClearFameTail();
	
	//OS_Protect(0);
	m_dwNoRecive = 0;/////收到字符有变量清零
	
	dwRxdWritePtr = m_wRxdWritePtr;
	
	for(i=0; i<wLen; i++)
	{
		m_RxdBuf[dwRxdWritePtr] = pBuf[i];
		dwRxdWritePtr = (dwRxdWritePtr+1)%m_wMaxRxdLen;
	}
	
	m_wRxdWritePtr = dwRxdWritePtr;
	//OS_UnProtect(0);
	
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      从串口通道发送缓冲区读取一定长度的数据，一般供底层驱动调用
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::ReadFromBuf(BYTE* pBuf, WORD wLen)
{
	WORD i;
	WORD wNum;
	DWORD dwTxdReadPtr;

	wNum = GetCanSendNum();
 	
 	if(wNum == 0)
 		return 0;
 		
 	if(wLen > wNum)
 		wLen = wNum;
 	
	ClearWriteMask();
	
	//OS_Protect(0);
	m_dwNoRecive = 0;
	m_dwNoSend = 0;
	
	dwTxdReadPtr = m_wTxdReadPtr;
	
	for(i=0; i<wLen; i++)
	{
		pBuf[i] = m_TxdBuf[dwTxdReadPtr];
	
		dwTxdReadPtr ++;
		dwTxdReadPtr %= m_wMaxTxdLen;
	}
	
	m_wTxdReadPtr = dwTxdReadPtr;
	//OS_UnProtect(0);
	
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      向对应串口物理通道写入一个字节的发送数据，一般供串口驱动调用以触发发送中断
 *
 * 参数说明      	
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::WriteAChar()
{
	BYTE bData;
	
	if(ReadFromBuf(&bData, 1))
		::WriteAChar(MINOR_PORT(m_dwPort), bData);
}

/*********************************************************************************************
 *
 * 功能描述      根据标志复位串口通道类，可能为发送缓冲区、接收缓冲区或整个串口通道
 *
 * 参数说明 	 - dwFlag	:	输入	复位类型标志     	
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::ResetSerial(DWORD dwFlag)
{
	if(dwFlag&RESET_RXDBUF)///复位接收缓冲区
	{
		m_wRxdWritePtr = 0;
		m_wRxdReadPtr = 0;		
		m_bRxdTail = 0;
		m_dwNoRxdTime = 0;
		m_dwNoReadCount = 0;
	}
	if(dwFlag&RESET_TXDBUF)
	{
		m_wRxdWritePtr = 0;
		m_wRxdReadPtr = 0;
		m_wTxdWritePtr = 0;
		m_wTxdReadPtr = 0;

		m_bRxdTail = 0;
		m_dwNoRxdTime = 0;

		m_dwNoSend = 0;

		m_bTxdOnOff = FALSE;
	
		m_bWriteContinue = FALSE;
		m_bNotify = FALSE;
	}
	if(dwFlag&RESET_SERIAL)
		ResetSerial();
}

/*********************************************************************************************
 *
 * 功能描述      复位串口通道类，并关闭串口物理通道的发送和接收
 *
 * 参数说明 	  	
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CSerial::ResetSerial()
{		
	SerialOpen(MINOR_PORT(m_dwPort));

	m_wRxdWritePtr = 0;
	m_wRxdReadPtr = 0;
	m_wTxdWritePtr = 0;
	m_wTxdReadPtr = 0;

	m_bRxdTail = 0;
	m_dwNoRxdTime = 0;

	m_bTxdMode = 0;
	m_bRxdMode = 0;

	m_dwNoRecive = 0;
	m_dwNoSend = 0;

	m_bTxdOnOff = FALSE;
	
	m_bWriteContinue = FALSE;
	m_bNotify = FALSE;
	m_dwNoReadCount = 0;
	
	timerStatus = NONEED;
	
	m_dwFlag = 0;

	for(int i=1; i <= TIMERSUM; i++)
		MyKillTimer(i);
	
	SerialRxdCtr(MINOR_PORT(m_dwPort), FALSE);
	::SerialTxdCtr(MINOR_PORT(m_dwPort), FALSE);
	
#if	(__DEBUG__ == YES)		
	LogError("ResetComm", FILE_LINE, "ResetComm--%d", MINOR_PORT(m_dwPort));
#endif
}

/*********************************************************************************************
 *
 * 功能描述      检查串口发送数据是否真正发送结束
 *
 * 参数说明 	  	
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL CSerial::IsTxdEnd()
{
	return SerialIOCtl(MINOR_PORT(m_dwPort), COM_TXD_END, (DWORD *)NULL, (DWORD *)NULL);
}
/*********************************************************************************************
 *
 * 功能描述      根据波特率序号获取对应的波特率数值
 *
 * 参数说明 	 - wNum		:	输入	波特率序号	  	
 *				 - dwBaud	:	输出	波特率值
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void NumToBaud(WORD wNum, DWORD *dwBaud)
{
	DWORD Baud[] = {110,150,300,600,1200,2400,4800,9600,19200,38400,57600,115200};

	if( wNum <= ( sizeof(Baud)/sizeof(DWORD) ) )
		*dwBaud = Baud[wNum];
	else
		*dwBaud = 9600;
}

/*********************************************************************************************
 *
 * 功能描述      根据波特率数值获取对应的波特率序号
 *
 * 参数说明 	 - dwBaud	:	输入	波特率值	  	
 *				 - wNum		:	输出	波特率序号
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void BaudToNum(DWORD dwBaud, BYTE *byNum)
{
	switch(dwBaud)
	{
		case 110:		*byNum = 0;		return;
		case 150:		*byNum = 1;		return;
		case 300:		*byNum = 2;		return;
		case 600:		*byNum = 3;		return;
		case 1200:		*byNum = 4;		return;
		case 2400:		*byNum = 5;		return;
		case 4800:		*byNum = 6;		return;
		case 9600:		*byNum = 7;		return;
		case 19200:		*byNum = 8;		return;
		case 38400:		*byNum = 9;		return;
		case 57600:		*byNum = 10;	return;
		case 115200:	*byNum = 11;	return;
		default:		return;	
	}
}

WORD CSerial::GetTxdWritePtr()
{
	return m_wTxdWritePtr;
}

WORD CSerial::GetRxdWritePtr()
{
	return m_wRxdWritePtr;
}

/*********************************************************************************************
 *
 * 功能描述      从串口通道发送缓冲区指定位置读取一定长度的数据，供维护查看
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::ReadFromTxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;

	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = m_TxdBuf[(wOffset+i)%m_wMaxTxdLen];
	}
		
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      从串口通道接收缓冲区指定位置读取一定长度的数据，供维护查看
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::ReadFromRxdBuf(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD i;

	for(i = 0; i< wLen; i++)
	{
		pBuf[i] = m_RxdBuf[(wOffset+i)%m_wMaxRxdLen];
	}
	
	return wLen;
}

/*********************************************************************************************
 *
 * 功能描述      获取发送写指针之前的新数据，供维护查看
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::ReadNewTxdData(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i;
		
	if(m_wTxdWritePtr >= wOffset)
		wLeft = m_wTxdWritePtr - wOffset;
	else
		wLeft = m_wMaxTxdLen - (wOffset-m_wTxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
		
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_TxdBuf[(wOffset+i)%m_wMaxTxdLen];
	}
	
	return wLeft;
}

/*********************************************************************************************
 *
 * 功能描述      获取接收写指针之前的新数据，供维护查看
 *
 * 参数说明      - pBuf	: 	输出	需要读取的数据缓存	
 *				 - wLen :	输入	准备读取的数据长度
 *
 * 返回代码
 *                实际写入的数据字节个数
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CSerial::ReadNewRxdData(BYTE* pBuf, WORD wOffset,WORD wLen)
{
	WORD wLeft;
	WORD i;

	if(m_wRxdWritePtr >= wOffset)
		wLeft = m_wRxdWritePtr - wOffset;
	else
		wLeft = m_wMaxRxdLen - (wOffset-m_wRxdWritePtr);
		
	if(wLen<wLeft)
		wLeft = wLen;
	
	if(wLeft == 0 || pBuf == NULL)
		return 0;
	
	for(i = 0; i< wLeft; i++)
	{
		pBuf[i] = m_RxdBuf[(wOffset+i)%m_wMaxRxdLen];
	}
	
	return wLeft;
}

