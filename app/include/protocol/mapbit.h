#ifndef _MAPBIT_H_
#define _MAPBIT_H_

#include "resm5234.h"
#include "system.h"
#include "msgmap.h"
#include "protocol.h"

#define MAPBIT_TIME						10						//���忴�Ź���ʱ��ʱ����
#define MAPBIT_COMMBREAK_MAX			(15000/MAPBIT_TIME)		//����ͨ���ж�ʱ��

class CMapBit;

typedef struct
{
	BYTE	START;			//��ʼ
	BYTE	DesADDR;		//Ŀ�ĵ�ַ
	BYTE	SrcADDR;		//Դ��ַ
	BYTE	DATA1[2];		//����λ	
	BYTE	DATA2[2];		//����λȡ��	
	BYTE	CRC;			//У��
}TMBFrame;


// ��̫������
class CMBEth
{
	private:
		DWORD		m_ip;				//�Զ�IP��ַ
		BYTE		m_ObjNo;			//�������
		BYTE		m_SelfAddr; 		//����վַ
		BYTE		m_SideAddr; 		//�Զ�վַ
		BYTE		m_SMBStart; 		//���;���λ��ʼλ��
		BYTE		m_SMBNum;			//���;���λ����
		BYTE		m_RMBStart; 		//���վ���λ��ʼλ��
		BYTE		m_RMBNum;			//���վ���λ����

		TBufMsg  m_FrameInfo;
		TBufMsg  m_RecvInfo;
		
	public:
		DWORD		m_dwCommBreakCnt;	//ͨ���жϼ�����

		CMapBit		*m_pMapBit;
		
		BOOL Init(CMapBit *pMapClass,BYTE no,TMBO_Disp	*pMBO);
		BOOL SendData();
		BOOL RecvData();
};

// CAN������
class CMBCan
{
	private:
		WORD 		m_PortID;
		BYTE		m_ObjNo;			//�������
		BYTE		m_SelfAddr; 		//����վַ
		BYTE		m_SideAddr; 		//�Զ�վַ
		BYTE		m_SMBStart; 		//���;���λ��ʼλ��
		BYTE		m_SMBNum;			//���;���λ����
		BYTE		m_RMBStart; 		//���վ���λ��ʼλ��
		BYTE		m_RMBNum;			//���վ���λ����


	public:	
		
		BOOL Init(BYTE no,TMBO_Disp	*pMBO);
		BOOL SendData();
		BOOL RecvData(BYTE *pBuf);
		
		BYTE GetSelfAddress(){return m_SelfAddr;};	
		BYTE GetSideAddress(){return m_SideAddr;};	
		WORD GetPortID(){return m_PortID;};	
};

// ���ڹ���
class CMBSer
{
	private:
		WORD 		m_PortID;			//�˿�ID
		BOOL		m_bOpen;			//�˿ڴ򿪱�־
		BYTE		m_ObjNo;			//�������
		BYTE		m_SelfAddr; 		//����վַ
		BYTE		m_SideAddr; 		//�Զ�վַ
		BYTE		m_SMBStart; 		//���;���λ��ʼλ��
		BYTE		m_SMBNum;			//���;���λ����
		BYTE		m_RMBStart; 		//���վ���λ��ʼλ��
		BYTE		m_RMBNum;			//���վ���λ����
	public:

		BOOL Init(BYTE no,TMBO_Disp	*pMBO);
		BOOL SendData();
		BOOL RecvData(BYTE *pBuf);

		BYTE GetSelfAddress(){return m_SelfAddr;};	
		BYTE GetSideAddress(){return m_SideAddr;};	
		WORD GetPortID(){return m_PortID;};	
};

class CMapBit:public CApp
{
    DECLARE_MESSAGE_MAP()

	private:
		int 	m_hSocket;
		DWORD	m_dwError;
		DWORD	m_dwWriteBlockNum;

		DWORD m_dwTickCnt; //Tick����
		
		TBufMsg  m_SerFrameInfo;
		TBufMsg  m_CanFrameInfo;

		TBufMsg  m_SerRxdInfo;
		TBufMsg  m_CanRxdInfo;
			
		BYTE		m_EthNum;
		BYTE		m_SerNum;
		BYTE		m_CanNum;

		CMBEth  	m_EthInfo[MAX_MAPBITOBJ_NUM];
		CMBCan 		m_CanInfo[MAX_MAPBITOBJ_NUM];
		CMBSer 		m_SerInfo[MAX_MAPBITOBJ_NUM];
			
		BOOL 	CreateSocket();
		BOOL 	CloseSocket();
		
		void 	OnTimeOut(DWORD id);
		void 	OnRelayRTMB(DWORD num);
		void 	OnReadComm(DWORD dwPort, DWORD dwFlag);
		void 	OnNetCardParaChange();

		void 	SerRecvProcess(WORD wPortID);
		void 	CanRecvProcess(WORD wPortID);
		
	public:
		WORD 	SendUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
		WORD 	RecvUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
	
		CMapBit(DWORD *pdwApp);
		BOOL Init(void);
};		

void MapBitTask(DWORD *pdwApp);


#endif
