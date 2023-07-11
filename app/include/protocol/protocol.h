#ifndef _CPROTOCOL_H_
#define _CPROTOCOL_H_

#include "system.h"
#include "appcore.h"
#include "platform.h"
#include "portdefs.h"
#include "daedef.h"
#include "daefunc.h"
#include "dbfunc.h"

#define INSTALL_S103
#define INSTALL_S104
//#define INSTALL_SGM104
//#define INSTALL_MMODRTU

#define DF_Exit				OS_EXIT


//��־����
#define MAX_TASKFLAGNO    	256    			//ÿ��ģ�����ı�־λ��

#define FLAG_OFF   			0
#define FLAG_ON    			1
#define FLAG_SWAP  			2

//=============  ��־λ����  ===============
#define 	SYSTEM_SFLAG     	0      		//ϵͳ������־λ��
#define 	USER_SFLAG       	32     		//�û���־λ�� 

#define 	SF_CommErr         	(SYSTEM_SFLAG+0)    //ͨѶ�жϣ�����ͨѶ�Ż�
#define 	SF_RxdNeed        	(SYSTEM_SFLAG+1)    //��Ҫ����֡,����ͨѶ�ж��ж�
#define 	SF_Sleep	  	  	(SYSTEM_SFLAG+2)    //ģ�鴦�����������ڣ����������ݣ�
#define 	SF_WakeUp	  	  	(SYSTEM_SFLAG+3)    //�������ԣ�������վ��Լ��ͨѶ�Ż�

//��������������ϵͳר���¼���ǣ���������������

//=============  ͨѶ���λ����  ===============
#define FM_SHIELD   		0xFFFF0000      //������
#define FM_OK       		0x00010000      //��⵽һ��������֡
#define FM_ERR      		0x00020000      //��⵽һ��У������֡
#define FM_LESS     		0x00030000      //��⵽һ����������֡����δ���룩

//����Ҫ��ͨѶ��Ϣ
#define MCC_COMMMSG 		(0x1F)  		//��ҪCommCtrl���Ƶ���Ϣ
#define MCC_ReadComm		(1<<0)  		//��ҪSM_READCOMM��Ϣ
#define MCC_WriteComm		(1<<1)  		//��ҪSM_WRITECOMM��Ϣ
#define MCC_CommIdle		(1<<2)  		//��ҪSM_COMMIDLE��Ϣ
#define MCC_CommTxdIdle		(1<<3)  		//��ҪSM_TXDIDLE��Ϣ
#define MCC_Baud			(1<<4)  		//�ı䲨����
#define MCC_CLOSELINK		(1<<5) 
#define DRVBUFSIZE  512      				//�ײ��������ͣ����ջ�������С

#define TM_1S     			 1       //1		//�붨ʱ

typedef struct 
 {
    DWORD Flag[ MAX_TASKFLAGNO/32 ];
 } FLAGS ; //��Լio


struct TBufMsg
{
	WORD wReadPtr;
	WORD wWritePtr;
	WORD wBufLen;
	BYTE* pBuf;
};

struct TMsgCtrl
{
	DWORD dwCode;     		//��Ϣӳ��
	DWORD dwChgFlag;  		//�ı��־
	
	WORD  wRxdPreNum;       //������wRxdPreNum���ַ�����Ϣ
	WORD  wTxdPreNum;       //��������wTxdPreNum���ַ�����Ϣ
	DWORD dwCommIdleTime;   //ͨ������CommIdleTime�������Ϣ  
	DWORD dwTxdIdleTime;  	//ͨ������CommIdleTime�������Ϣ
	DWORD dwBaud;  			//�����ʡ�����Ϊ����ʹ�ò��ı䲨����
};

struct CommBuf
{
	WORD ReadPtr;
	WORD WritePtr;
	BYTE* Buf;
};

/*------------------------------------------------------------------------------*/
/*        ע��ͨѶ����/�����¼� DevCommRegiste������Flag�Ķ���                  */
/*------------------------------------------------------------------------------*/
#define DCR_Txd   1    //����һ����
#define DCR_Rxd   2    //����һ��ȷ����

void NeatenIoBuf(TBufMsg* pMsgBuf);

//*******************************  ��Լ�ඨ����  *******************************//
class CProtocol: public CApp
{
	DECLARE_MESSAGE_MAP()

public:
   	CProtocol(DWORD *pdwApp);
	BOOL Init();
	void Exit(void *p)
	{
		ReportMsg((char *)p);
		::OS_Exit(0); 
	};
	
private:
	BOOL m_bHaveRxdFm; //�Ƿ���δ������ĺ�����
	
	TBufMsg  m_FrameInfo;
	TMsgCtrl m_MsgCtrl;
	FLAGS 	 m_Flags;
	DWORD	 m_TxdFlag; 
	WORD	 m_CommErrCnt;
	WORD 	 m_dwRetryCount;  //�ط�����ͳ��

	WORD	m_TxdFrmCnt;
	WORD	m_RxdFrmCnt;
	
	void 	CommCtrl();
	DWORD 	ReadFromComm();

//	void 	NeatenIoBuf(TBufMsg* pMsgBuf);

	BOOL 	SearchFrame();//����У����ȷ�ı���

	void 	OnReadComm(DWORD  dwPort, DWORD dwFlag);
	void 	OnWriteCont(DWORD dwPort, DWORD dwFlag);
	
protected:
	TBufMsg			m_RxdInfo;
	TBufMsg 		m_TxdInfo;
	DWORD  			m_dwAppID;   		//����ID��
	DWORD 			m_wPortID;			//�˿�ID��
	BYTE  			m_GBAddressFlag; 	//�Ƿ��ǹ㲥��ַ
	WORD			m_TxdWritePtr;		//����дָ��
	BYTE* 			GetTxdBuf();
	TAppInfo   		m_App;

	BOOL 	CommOpen();
	DWORD 	GetPortID(void){return m_wPortID;};//��ȡ�˿�ID��
	void 	SetBufLen(WORD wRxdLen, WORD wTxdLen);
	void 	GetBufLen(WORD& wRxdLen, WORD& wTxdLen);
	void 	StartMsg(DWORD MsgID);
	void 	StartMsg(DWORD MsgID, DWORD MsgValue);
	void 	StopMsg(DWORD MsgID);
	WORD 	WriteComm(WORD wLen, WORD Address = 0, DWORD dwFlag = 0);
	BOOL 	ShowMsg(void* head,BYTE *pbuf=NULL,WORD len=0,void *tail=NULL);//��ʾ��Ϣ
	
	void 	ResetRetryCount(){ m_dwRetryCount=0;};   //�ط�������0
	void 	IncRetryCount(){ m_dwRetryCount++;};     //�ط�������1
	void 	DisableRetry(){ m_dwRetryCount=1000;};   //��ֹ�ط�
	WORD 	GetRetryCount(){return m_dwRetryCount;}; //��ȡ�ط�����
    void 	TxdRetry(void); //�ط�����
	void 	DisplayRxdBuf();
	void 	DisplayTxdBuf();
	
	virtual DWORD SearchOneFrame(BYTE* pBuf, WORD nLen);
	virtual void OnFrame(BYTE* pBuf){};

	virtual DWORD GetGBAddress(void);
	
	void 	ClearRxdBuf(void)//��ս��ջ�����
	{	
	     	while(ReadFromComm())
		    m_RxdInfo.wWritePtr = m_RxdInfo.wReadPtr = 0;
	};  

	void	SetFlag(DWORD FlagNo,DWORD Status=FLAG_ON);
	DWORD	GetFlag(DWORD FlagNo);
	void	SetFlagByte(DWORD ByteNo,BYTE Status);
	BYTE	GetFlagByte(DWORD ByteNo);
	void	SetFlagWord (DWORD WordNo,WORD Status);
	WORD	GetFlagWord(DWORD WordNo);
	void 	SetFlagDWord (DWORD DWordNo,DWORD Status);
	DWORD	GetFlagDWord (DWORD DWordNo);
	BOOL  	CheckClearFlag(DWORD FlagNo);
	BOOL  	RegisterCommStatus(BYTE Status);
};

/*��׼LPCУ���㷨*/
BYTE LPC(BYTE BeginCode,F_PBYTE Buf,WORD num);

BYTE CheckSum(BYTE BeginCode,F_PBYTE Buf,DWORD num);

BOOL  IsTimeNew(TAbsTime time1,TAbsTime time2);

#define INIPARA_MAX_LENGTH		5120		//�ļ�����Ϊ5k
#define INIPARA_MAX_LINELENGTH  100			//ÿ�е���󳤶�
#define INIPARA_MAX_SEGLENGTH   30			//��������󳤶�
#define INIPARA_MAX_PARALENGTH  30			//����������󳤶�
#define INIPARA_MAX_VALUELENGTH 30			//����ֵ����󳤶�

#define INIPARA_MAX_FILENAMELENGTH 256		//����ֵ����󳤶�

#define INIPARA_MAX_ITEMNUM 100			//������Ŀ������

typedef struct
{
	char Section[INIPARA_MAX_SEGLENGTH];
	char ParaName[INIPARA_MAX_PARALENGTH];
	char ParaValue[INIPARA_MAX_VALUELENGTH];
}TIniParaData;

int 	InitProtocolPara();
int 	chtohex(char c);
DWORD 	atoix(char* ParaValue);

void 	SCanbus_Task(DWORD *pdwApp);
void 	WHShell_Task(DWORD *pdwApp);

#ifdef INSTALL_S103
void 	S103_Task(DWORD *pdwApp);
#else
#define S103_Task NULL
#endif

#ifdef INSTALL_S104
void 	S104_Task(DWORD *pdwApp);
#else
#define S104_Task NULL
#endif

#ifdef INSTALL_SGM104
void 	SGm104_Task(DWORD *pdwApp);
#else
#define SGm104_Task NULL
#endif

#ifdef INSTALL_MMODRTU
void 	MModRtu_Task(DWORD *pdwApp);
#else
#define MModRtu_Task NULL
#endif

extern TIniParaData* g_IniParaData;
extern DWORD        g_IniParaNum;


#endif

