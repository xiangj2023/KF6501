#ifndef _FLTASK_H_
#define _FLTASK_H_

#include "resm5234.h"
#include "system.h"
#include "msgmap.h"
#include "protocol.h"

#define FLTASK_TICK						20						//���嶨ʱ������20mS
#define FLTASK_TIMEOUT					(3000/FLTASK_TICK)		//����ͨ�ų�ʱʱ��
#define FLTASK_HEARTBEET				(6000/FLTASK_TICK)		//������������
#define FLTASK_COMMBREAK_MAX			(20000/FLTASK_TICK)		//����ͨ���ж�ʱ��

#define FLTASK_MINFRMLEN				6						//��С֡��
#define FLTASK_MAXFRMLEN				1024					//���֡��

#define FLTASK_SERPORTID				0x102	//����ȱʡ���ں�
#define FLTASK_SETAREA_NORMAL				0		//������ֵ����
#define FLTASK_SETAREA_STEPOVER_A			1		//����Խ����ֵ����
#define FLTASK_SETAREA_STEPOVER_B			2		//�Բ�Խ����ֵ����

#define FLTASK_MODE_HEARTBEAT 	0	//����
#define FLTASK_MODE_AUTOFL	  	1	//�Զ����
#define FLTASK_MODE_MANUALFL  	2	//�ֶ����
#define FLTASK_MODE_HISTORYFL 	3	//��ʷ�ٲ�
#define FLTASK_MODE_LOWVOLFL  	4	//��ѹ�ٲ�
#define FLTASK_MODE_SOSWITCH	5	//Խ������
#define FLTASK_MODE_SORETURN	6	//Խ����������

#define FL_SUBST_SELFAT 	1   //����AT��
#define FL_SUBST_SELFSP 	2	//���������
#define FL_SUBST_SIDESP 	3	//�Բ������
#define FL_SUBST_SIDEAT 	4	//�Բ�AT��
#define FL_SUBST_SIDEMAIN 	5	//�Բ�����


#define FLTASK_FUNC_HEARTREQ  		0x00	//��������
#define FLTASK_FUNC_HEARTRES  		0x80	//������Ӧ

#define FLTASK_FUNC_RELAYREQ  		0x01	//���������������
#define FLTASK_FUNC_RELAYRES  		0x81	//�������������Ӧ

#define FLTASK_FUNC_MANUALREQ  		0x02	//�ֶ��������
#define FLTASK_FUNC_MANUALRES  		0x82	//�ֶ������Ӧ

#define FLTASK_FUNC_HISTREQ  		0x03	//�ٲ���ʷ����
#define FLTASK_FUNC_HISTRES  		0x83	//�ٲ���ʷ��Ӧ

#define FLTASK_FUNC_LOWVALREQ  		0x04	//�ٲ��ѹ��������
#define FLTASK_FUNC_LOWVALRES  		0x84	//�ٲ��ѹ������Ӧ

#define FLTASK_FUNC_STEPOVERCMDREQ  		0x05	//Խ������ָ��
#define FLTASK_FUNC_STEPOVERCMDRES  		0x85	//Խ��������Ӧ

#define FLTASK_FUNC_STEPOVERRETREQ  		0x06	//Խ������ָ��
#define FLTASK_FUNC_STEPOVERRETRES  		0x86	//Խ��������Ӧ

#define FLTASK_SUBSET_POLLING  		1	//��ѯģʽ
#define FLTASK_SUBSET_COMMAND  		2	//����ģʽ

#define FLTASK_LINK_SERIAL  		1	//��������ģʽ
#define FLTASK_LINK_TRANS  			2	//·������ģʽ
#define FLTASK_LINK_ETHERNET  		0	//��̫��ֱ����

#define FLTASK_COMMAND_SWITCH		0x00	//Խ������
#define FLTASK_COMMAND_RETURN		0x01	//Խ����������


class CFLTask;

typedef struct
{
	BYTE	START;			//��ʼ
	BYTE	DevNo;			//�豸���
	BYTE	Func;			//������
	BYTE	DataLenLO;		//�����򳤶ȵ��ֽ�	
	BYTE	DataLenHI;		//�����򳤶ȸ��ֽ�	
	BYTE	DATA;			//������	
}TFLFrameHead;

// ��վ��Ϣ
class CFLSubDev
{
	private:
		CFLTask		*m_pFLTask;
		BYTE		m_byParaNo;			//�������
		BYTE		m_byNetAddr;		//����ͨ�ŵ�ַ
		BYTE		m_byFLNo;			//�����
		BYTE		m_byLinkMode;		//���ӷ�ʽ
		BOOL		m_bRoute;			//�Ƿ�·��
		BOOL		m_bValid;			//�Ƿ������Ч
		BOOL		m_bInclude;			//�Ƿ������ÿ�ι�������ʱ�����Ӽ�
		BOOL		m_bSideDev;			//�ǶԲ��豸
		TFL_Disp	m_FLDisp;			//����������		

	public:
		CFLSubDev();
		
		DWORD 		m_dwNetIP;			//IP��ַ
		WORD		m_nTimeoutCnt;		//ͨ�ų�ʱ����
		WORD		m_nNoAckCnt;		//ͨ����Ӧ�����
		BOOL		m_bProcFlag;		//��ѯ��־
		BOOL		m_bHeart;			//������
		BOOL		m_bHaveAck;			//��Ӧ��
		BOOL		m_bRelayFL;			//�������
		BOOL		m_bManualFL;		//�ֶ����
		BOOL		m_bForeFL;			//�ٲ���ʷ
		BOOL		m_bLowValFL;		//�ٲ��ѹ����
		BOOL		m_bStepOverCmd;		//Խ��ָ��ִ��
		BOOL		m_bStepOverRet;		//Խ��ָ���

		BYTE		m_byFLDataIndex;	//������������
		BYTE		m_CallType;			//�ٲ�����
		
		TBufMsg  	m_TxdInfo;			//���ͻ���
		TFLFrameHead *m_pRxdFrame;		//����֡;

		BOOL 	Init(BYTE no,TFL_Disp *pFLDisp,CFLTask *pFLTask);
		void 	ClearAllPollCmd();
		
		BYTE	GetFLNo(){return m_byFLNo;};
		BYTE	GetDevType(){return (m_FLDisp.wControl&0x0F);};
		BYTE	GetLinkMode(){return m_byLinkMode;};
		BOOL	IsValid(){return m_bValid;};
		BOOL	IsPolling(){return m_bInclude;};
		BOOL	IsSideDev(){return m_bSideDev;};
		
		void 	Include(){m_bInclude = TRUE;};
		void 	Disinclude(){m_bInclude = FALSE;};
/*		
		BOOL 	Enable()
		{	
			if(m_bValid)
				return TRUE;
			
			m_bValid = TRUE;
			m_FLDisp.wControl |= 0x8000;
			return DB_SetFLObjDisp(m_byParaNo, &m_FLDisp);
		};
		
		BOOL 	Disenable()
		{
			if(!m_bValid)
				return TRUE;
			
			m_bValid = FALSE;
			m_FLDisp.wControl &= 0x7FFF;	
			return DB_SetFLObjDisp(m_byParaNo, &m_FLDisp);
		};
*/		
		void 	SwitchNetComm();
		BOOL 	SendData();
		BOOL 	TxdProcess();
		BOOL 	OnFrame();
		BOOL 	OnDataIsOK();

		BOOL 	FillFLSendData(subChannelCheck	*pFLData);
		BOOL 	SaveFLrecvData(subChannelCheck	*pFLData);
		BOOL 	TxdFrame(BYTE FuncCode,WORD wDataLen);

		// ����Խ���л�����
		BOOL 	TxdStepOverCmd();
		// ����Խ����������
		BOOL 	TxdStepOverRet();
		// ���ͱ����������
		BOOL 	TxdRelayFLCmd();
		// �����ֶ��������
		BOOL 	TxdManualFLCmd();
		// �ٲ���ʷ�������
		BOOL 	TxdHistoryFLCmd();
		// �ٲ��ѹ������������
		BOOL 	TxdLowValFLCmd();
		// �ٲ��ѹ������������
		BOOL 	TxdHeartFLCmd();

		// ���շ���Խ���л���������
		BOOL 	RxdStepOverCmdReq();
		// ���շ���Խ��������������
		BOOL 	RxdStepOverRetReq();
		// ���շ��ͱ��������������
		BOOL 	RxdRelayFLCmdReq();
		// ���շ����ֶ������������
		BOOL 	RxdManualFLCmdReq();
		// �����ٲ���ʷ�����������
		BOOL 	RxdHistoryFLCmdReq();
		// �����ٲ��ѹ������������
		BOOL 	RxdLowValFLCmdReq();		
		// �����ٲ�������������
		BOOL 	RxdHeartBeatCmdReq();

		// ���շ���Խ���л�������Ӧ
		BOOL 	RxdStepOverCmdRes();
		// ���շ���Խ������������Ӧ
		BOOL 	RxdStepOverRetRes();
		// ���շ��ͱ������������Ӧ
		BOOL 	RxdRelayFLCmdRes();
		// ���շ����ֶ����������Ӧ
		BOOL 	RxdManualFLCmdRes();
		// �����ٲ���ʷ���������Ӧ
		BOOL 	RxdHistoryFLCmdRes();
		// �����ٲ��ѹ����������Ӧ
		BOOL 	RxdLowValFLCmdRes();
		// ��������������Ӧ
		BOOL 	RxdHeartBeatCmdRes();
};

typedef struct
{
	BYTE		m_SeqCnt;					//��������
	BYTE		m_SeqList[FLCOMPLICATENUM];	//������ű�
	TAbsTime	m_Time[FLCOMPLICATENUM];	//�������ʱ��
}TRelayFLCtrlInfo;

class CFLTask:public CApp
{
    DECLARE_MESSAGE_MAP()

	private:
		BYTE	m_FLSubNum;			//�����������
		BYTE	m_FLMainNo;			//�������ȫ�ֱ��
		BYTE	m_byLinkMode;		//����ģʽ
		BOOL	m_bMainStation;		//�Ƿ�����
		BOOL	m_bCommValid;		//�Ƿ���Ҫͨ��
		BOOL	m_bStepOver;		//�ǵ�ǰ����Խ��
		
		int 	m_hSocket;
		DWORD	m_dwError;
		DWORD	m_dwWriteBlockNum;

		DWORD 	m_nTickCnt; 		//Tick����

		BYTE	m_DevNo;	//��ǰ��ѯģ��
		CFLSubDev m_FLSubDev[MAX_SUBSTATION_NUM];//�������

		void		*m_pMemBlock;		//Ԥ���ڴ��						
		TBufMsg  	m_FrameInfo;		//֡����
		TBufMsg  	m_RecvInfo;			//���ջ���
			
		BOOL 	CreateSocket();
		BOOL 	CloseSocket();
		
		void 	OnTimeOut(DWORD id);
		void 	OnCallSubData(DWORD dwTimeLO,WORD wTimeHi,WORD Mode);
		void 	OnStopCallData(DWORD Mode);
		void 	OnStepOverCmd(DWORD Mode);
		void 	OnStopStepOver(DWORD Mode);
		void 	OnDataIsOk(DWORD DataPtr,WORD Result);
		void 	OnRelayChange(DWORD dwDataType,DWORD dwIndex);

		BOOL 	SearchOneFLFrame(PBYTE Buf,WORD wLen);
		BOOL 	SearchFLFrame(TBufMsg* pMsgBuf,TBufMsg  *pFrameInfo);

	public:
		CFLTask(DWORD *pdwApp);
		
		BOOL 	Init(void);
		BOOL 	InitFLPara(void);
		BOOL 	InitSubST(void);
		BOOL 	InitMainST(void);
		
		BOOL		m_bCommand;			//Խ���������
		DWORD		m_nCommandCnt;		//�����ʱ
		BYTE		m_CmdType;			//Խ����������
		TAbsTime	m_tFLTime;			//���ʱ��
		WORD 		m_SerPortID;		//����ID
		WORD		m_TransPortID;		//͸���˿�ID
		DWORD		m_dwMSPAddr;		//����IP��ַ
		WORD		m_TransDataRP;		//͸�����ݶ�ָ��	
		
		BOOL		m_bRelayFL;			//��������������
		BOOL		m_bManualFL;		//�����ֶ�������
		BOOL		m_bForeFL;			//�����ٲ���ʷ����
		BOOL		m_bLowValFL;		//�����ٲ��ѹ��������

		TRelayFLCtrlInfo	m_tRelayFLInfo;	//���������Ϣ

		WORD 	SendUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
		WORD 	RecvUDPData(DWORD ipAddr,BYTE * pBuf,WORD wLen);
		WORD 	RecvTransData(BYTE * pBuf,WORD wLen);

		BOOL 	IsCallDataEnd(BYTE byFLType); //�ٲ����
		BOOL 	IsCommandEnd();			//Խ���л�����
				
		CFLSubDev *GetDev();

		BYTE	GetLinkMode(){return m_byLinkMode;};
		BYTE	*GetRecvFrame(){return m_FrameInfo.pBuf;};
		void	ClearRecvFrame(){m_FrameInfo.wWritePtr = 0;};
		BOOL	IsMainStation(){return m_bMainStation;};	//�Ƿ�����	
		BOOL	IsStepOver(){return m_bStepOver;};	//�Ƿ�Խ��	
		
		BOOL 	SwitchToNextSerDev(void);
		BOOL 	SwitchToAddress(BYTE Addr);
		void 	SwitchPollingSubSet(BYTE Mode);

		WORD 	RecvData();		//��������

		void 	TxdMonitor();	//���͹���
		void 	RxdMonitor();	//���չ���

		
		BOOL PushRelayFL(BYTE Seq,TAbsTime *pTime);		
		BOOL PopRelayFL();
		BOOL DelRelayFL(BYTE Seq);
		BOOL IsCurrentRelayFL(BYTE Seq);
		BOOL HaveRelayFLCmd();
		BYTE GetRelayFLSeq();
		BOOL GetRelayFLTime(TAbsTime *pTime);			
};

void	FLTASKTask(DWORD *pdwApp);

#endif
