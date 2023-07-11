/*************************************************************************
* 
*   Copyright (C), 2007-2013, keyvia. Co., Ltd.
* 
*   �ļ����ƣ�mModRtu.h
*   ���ģ�飺Modbus_RTU��վ��Լ
*   �� �� �ţ�1.0
*   �������ڣ�2013-9-22
*   ��    �ߣ�L.Z
*   ��    �ܣ�<...>
* 
*************************************************************************/


/*************************************************************************
* �Ķ���ʷ��¼��

Revision 1.0, 2013-9-22, L.Z ��ʼ����

*************************************************************************/
#ifndef _MMODRTU_H
#define _MMODRTU_H

#include "protocol.h"
#include "msgmap.h"
#include "iecdefs.h"

#ifdef INSTALL_MMODRTU

	
//���Ĵ�С����
#define MAX_TXDFM_SIZE  28   //�����֡��
#define MAX_RXDFM_SIZE  256  //������֡��
#define MIN_RXDFM_SIZE  5    //��С����֡��

#define MAX_COMMERR_TIME	10

//�궨�����ļ�������
#define MOD_DATATYPE_AI 	0x01  
#define MOD_DATATYPE_BI 	0x02  
#define MOD_DATATYPE_CI 	0x03  
#define MOD_DATATYPE_BO 	0x04  

#define	MOD_EXTMODE_WORD	0x01
#define	MOD_EXTMODE_BYTE	0x02
#define	MOD_EXTMODE_BIT		0x03
#define	MOD_EXTMODE_DWORD	0x04

#define 	SF_HaveBO         	(USER_SFLAG+0)    //��ң��
#define 	SF_TIMING         	(USER_SFLAG+1)    //�ж�ʱ

typedef struct
{
	BYTE	address;		//�豸��ַ
	BYTE	datatype;		//��������
	BYTE	funcode;		//������
	WORD	regaddr;		//�Ĵ�����ַ
	BYTE	regnum;			//�Ĵ���������������
	BYTE	extmode;		//������ʽ
	BYTE	datastart;		//���ݴ����ʼ
	BYTE	datanum;		//���ݴ�Ÿ���
}TFuncInfo;

//				 ========= ����/ͻ��/�ܿ������־ =========

class CMModRtu : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	BOOL Init(void);//��ʼ��
    CMModRtu(DWORD *pdwApp):CProtocol(pdwApp){};

protected:
	CommBuf 	m_Txd;
	BYTE		*m_TxdBuf;
	TFuncInfo 	*m_pFrameList;
	TFuncInfo 	*m_pLastFunc;	//��ѯ֡���
//	BYTE 		m_CommIdle;		//ͨ�ſ���ʱ��
	WORD		m_CommIdle;		//ͨ�ſ���ʱ��			yanxs 20140428  ʹ����ʱ�����255ms
	BYTE 		m_TotalFrm;		//��֡�����
	BYTE 		m_PollCnt;		//��ѯ���
	BYTE		m_BOInfoNo;		//ң����Ϣ���
	TBO			m_BO;			//ң����Ϣ

	DWORD	m_NoRxdCounter;
	
	BOOL InitPara();
	
	void OnCommIdle(DWORD dwLen,DWORD PortID);
	void OnTimeOut(DWORD TimerID);
	void OnBOSelect(WORD no, WORD attr, WORD par);
	void OnBOExec(WORD no, WORD attr, WORD mode);
	void OnBODirect(WORD no, WORD attr, WORD par);
	
	//������Ч֡	
	DWORD SearchOneFrame(PBYTE pBuf,WORD wLength);
	//==== ���ձ��Ĵ����� ====
	void OnFrame(BYTE* pBuf);

	WORD Crc16(BYTE * pBuf, BYTE Len);
	BOOL GetBOFrame(WORD nBO);
	BOOL SwitchToNextFrame();
	
	//���ͱ��Ĵ�����
	BOOL TxdMonitor(void);
	BOOL Txd_Frame(TFuncInfo *pFrame);

	BOOL Rxd_Func0102(BYTE *pBuf);
	BOOL Rxd_Func0304(BYTE *pBuf);
	BOOL Rxd_Func0506(BYTE *pBuf);
};

#endif
#endif
