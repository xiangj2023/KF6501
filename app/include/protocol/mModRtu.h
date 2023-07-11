/*************************************************************************
* 
*   Copyright (C), 2007-2013, keyvia. Co., Ltd.
* 
*   文件名称：mModRtu.h
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
#ifndef _MMODRTU_H
#define _MMODRTU_H

#include "protocol.h"
#include "msgmap.h"
#include "iecdefs.h"

#ifdef INSTALL_MMODRTU

	
//报文大小定义
#define MAX_TXDFM_SIZE  28   //最大发送帧长
#define MAX_RXDFM_SIZE  256  //最大接收帧长
#define MIN_RXDFM_SIZE  5    //最小接收帧长

#define MAX_COMMERR_TIME	10

//宏定义区文件引用区
#define MOD_DATATYPE_AI 	0x01  
#define MOD_DATATYPE_BI 	0x02  
#define MOD_DATATYPE_CI 	0x03  
#define MOD_DATATYPE_BO 	0x04  

#define	MOD_EXTMODE_WORD	0x01
#define	MOD_EXTMODE_BYTE	0x02
#define	MOD_EXTMODE_BIT		0x03
#define	MOD_EXTMODE_DWORD	0x04

#define 	SF_HaveBO         	(USER_SFLAG+0)    //有遥控
#define 	SF_TIMING         	(USER_SFLAG+1)    //有对时

typedef struct
{
	BYTE	address;		//设备地址
	BYTE	datatype;		//数据类型
	BYTE	funcode;		//功能码
	WORD	regaddr;		//寄存器地址
	BYTE	regnum;			//寄存器个数或者数据
	BYTE	extmode;		//解析方式
	BYTE	datastart;		//数据存放起始
	BYTE	datanum;		//数据存放个数
}TFuncInfo;

//				 ========= 紧急/突发/受控任务标志 =========

class CMModRtu : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	BOOL Init(void);//初始化
    CMModRtu(DWORD *pdwApp):CProtocol(pdwApp){};

protected:
	CommBuf 	m_Txd;
	BYTE		*m_TxdBuf;
	TFuncInfo 	*m_pFrameList;
	TFuncInfo 	*m_pLastFunc;	//轮询帧类别
//	BYTE 		m_CommIdle;		//通信空闲时间
	WORD		m_CommIdle;		//通信空闲时间			yanxs 20140428  使空闲时间大于255ms
	BYTE 		m_TotalFrm;		//总帧类别数
	BYTE 		m_PollCnt;		//轮询序号
	BYTE		m_BOInfoNo;		//遥控信息序号
	TBO			m_BO;			//遥控信息

	DWORD	m_NoRxdCounter;
	
	BOOL InitPara();
	
	void OnCommIdle(DWORD dwLen,DWORD PortID);
	void OnTimeOut(DWORD TimerID);
	void OnBOSelect(WORD no, WORD attr, WORD par);
	void OnBOExec(WORD no, WORD attr, WORD mode);
	void OnBODirect(WORD no, WORD attr, WORD par);
	
	//搜索有效帧	
	DWORD SearchOneFrame(PBYTE pBuf,WORD wLength);
	//==== 接收报文处理函数 ====
	void OnFrame(BYTE* pBuf);

	WORD Crc16(BYTE * pBuf, BYTE Len);
	BOOL GetBOFrame(WORD nBO);
	BOOL SwitchToNextFrame();
	
	//发送报文处理函数
	BOOL TxdMonitor(void);
	BOOL Txd_Frame(TFuncInfo *pFrame);

	BOOL Rxd_Func0102(BYTE *pBuf);
	BOOL Rxd_Func0304(BYTE *pBuf);
	BOOL Rxd_Func0506(BYTE *pBuf);
};

#endif
#endif
