//=======================================================================================
// ����: Siec104.h
// ����: IEC104��վ��Լ��Ķ���
// ��д��lz 2011-12-23
// ��д��
//=======================================================================================
#ifndef _SIEC104_H
#define _SIEC104_H

#include "protocol.h"
#include "msgmap.h"
#include "relaydef.h"
#include "iecdefs.h"

#ifdef INSTALL_SGM104 


//����104�����õ�101�İ汾
#define VER1997			1997
#define VER2002			2002

#define VER				VER2002

#define MAX_TXDFRAME_SIZE  255  //�����֡��
#define MAX_RXDFRAME_SIZE  255  //������֡��
#define MIN_RXDFRAME_SIZE  5    //��С����֡��
#define MAX_COMMERR_TIME	30 		//10����������ͨ��ʧ��

const	BYTE	NBYTES_LINKADDR		= 2;	//��·��ַ�ֽ��� 1 or 2
const	BYTE	NBYTES_COMMADDR		= 2;	//������ַ�ֽ��� 1 or 2
const	BYTE	NBYTES_REASON		= 2;	//����ԭ���ֽ��� 1 or 2
const	BYTE	NBYTES_ELMENTADDR	= 3;	//��ϢԪ�ص�ַ�ֽ��� 2 or 3

const	BYTE	MAXFRMLEN	 = 255;			//���֡�� ����ͬ���֡�����	
const	BYTE	MAXGRPUP	 = 4;			//������

#if(SYSTEM_TYPE == _SYS_KF1300_)

#define GRP_AINUM   40     //ÿ����෢��ң�����	 96->48   
#define GRP_BINUM   127    //ÿ����෢��ң���ֽ�
#define GRP_CINUM   24     //ÿ����෢�͵�ȸ���	 32 -> 24
#define GRP_SOENUM  20     //24     //ÿ����෢��SOE����
#define GRP_AIENUM  16     //1     // ÿ����෢��AIE����

#else

#define GRP_AINUM   80     //ÿ����෢��ң�����	 96->48   
#define GRP_BINUM   127    //ÿ����෢��ң���ֽ�
#define GRP_CINUM   24     //ÿ����෢�͵�ȸ���	 32 -> 24
#define GRP_SOENUM  20     //24     //ÿ����෢��SOE����
#define GRP_AIENUM  16     //1     // ÿ����෢��AIE����

#endif

#define CHGAI_ABS	1		 //ai�仯��Χȡ����ֵ
#define CHGAI_PER	2		 //ai�仯��Χȡ�ٷֱ�

//ϵ�y�ض�����
#define SF_INIT_OVER		 (USER_SFLAG+0)	//��վ��ʼ�����
#define SF_BUFFLOWOVER	 	 (USER_SFLAG+1)	//���ͻ�������ռ��
#define SF_CallData	         (USER_SFLAG+2)	//Ŀǰ�����ٻ�����״̬
#define SF_STOPSEND	         (USER_SFLAG+3)	//��������ֹͣ����

#define SF_BIGRP1		 (USER_SFLAG+8)   //���ٻ�ң����1   //����Ϊ8��λ��
#define SF_BIGRP2		 (USER_SFLAG+9)   //���ٻ�ң����2
#define SF_BIGRP3		 (USER_SFLAG+10)   //���ٻ�ң����3
#define SF_BIGRP4		 (USER_SFLAG+11)   //���ٻ�ң����4
#define SF_BIGRP5		 (USER_SFLAG+12)   //���ٻ�ң����5
#define SF_BIGRP6		 (USER_SFLAG+13)   //���ٻ�ң����6
#define SF_BIGRP7		 (USER_SFLAG+14)   //���ٻ�ң����7
#define SF_BIGRP8		 (USER_SFLAG+15)   //���ٻ�ң����8
#define SF_BIGRP9		 (USER_SFLAG+16)   //���ٻ�ң����9  
#define SF_BIGRP10		 (USER_SFLAG+17)   //���ٻ�ң����10
#define SF_BIGRP11		 (USER_SFLAG+18)   //���ٻ�ң����11
#define SF_BIGRP12		 (USER_SFLAG+19)   //���ٻ�ң����12
#define SF_BIGRP13		 (USER_SFLAG+20)   //���ٻ�ң����13
#define SF_BIGRP14		 (USER_SFLAG+21)   //���ٻ�ң����14
#define SF_BIGRP15		 (USER_SFLAG+22)   //���ٻ�ң����15
#define SF_BIGRP16		 (USER_SFLAG+23)   //���ٻ�ң����15
#define SF_AIGRP1		 (USER_SFLAG+24)   //���ٻ�ң����1
#define SF_AIGRP2		 (USER_SFLAG+25)   //���ٻ�ң����2
#define SF_AIGRP3		 (USER_SFLAG+26)   //���ٻ�ң����3
#define SF_AIGRP4		 (USER_SFLAG+27)   //���ٻ�ң����4
#define SF_AIGRP5		 (USER_SFLAG+28)   //���ٻ�ң����5
#define SF_AIGRP6		 (USER_SFLAG+29)   //���ٻ�ң����6
#define SF_AIGRP7		 (USER_SFLAG+30)   //���ٻ�ң����7
#define SF_AIGRP8		 (USER_SFLAG+31)   //���ٻ�ң����8
#define SF_AIGRP9		 (USER_SFLAG+32)   //���ٻ�ң����9
#define SF_AIGRP10		 (USER_SFLAG+33)   //���ٻ�ң����10
#define SF_AIGRP11		 (USER_SFLAG+34)   //���ٻ�ң����11
#define SF_AIGRP12		 (USER_SFLAG+35)   //���ٻ�ң����12
#define SF_AIGRP13		 (USER_SFLAG+36)   //���ٻ�ң����13
#define SF_AIGRP14		 (USER_SFLAG+37)   //���ٻ�ң����14
#define SF_AIGRP15		 (USER_SFLAG+38)   //���ٻ�ң����15
#define SF_AIGRP16		 (USER_SFLAG+39)   //���ٻ�ң����16
#define SF_ALLSTOP		 (USER_SFLAG+40)   //���ٻ�����

#define SF_CIGRP1		 (USER_SFLAG+48)   //���ٻ������1
#define SF_CIGRP2		 (USER_SFLAG+49)   //���ٻ������2
#define SF_CIGRP3		 (USER_SFLAG+50)   //���ٻ������3
#define SF_CIGRP4		 (USER_SFLAG+51)   //���ٻ������4
#define SF_ALLCISTOP     (USER_SFLAG+52)   //���ٻ���Ƚ���

#define SF_STSend			 (USER_SFLAG+56)//���Ͷ�ֵ��ֵ
#define SF_STSelectEcho		 (USER_SFLAG+57)//��ֵ
#define SF_SendChangedAI	 (USER_SFLAG+58)//�����ٻ�����
#define SF_HAVEREPORT		 (USER_SFLAG+59)//�б���
#define SF_HAVEBIE           (USER_SFLAG+60)//��ң�ű�λ
#define SF_HAVESOE           (USER_SFLAG+61)//��SOE
#define SF_BOEND         	 (USER_SFLAG+62)//ң�ع��̽���
#define SF_HAVEAIE           (USER_SFLAG+63)//��AIE

#define SF_SENDDIR			  (USER_SFLAG+64)	//�����ϴ�Ŀ¼
#define SF_CALLDIR			  (USER_SFLAG+66)	//�ٻ�Ŀ¼

#define SF_CALLRDSJ			  (USER_SFLAG+67)	//�򱣻�ģ���ٻ��Ŷ�����
#define SF_FILEREADY		  (USER_SFLAG+68)	//�ļ�׼����
#define SF_SENDSEC			  (USER_SFLAG+69)	//���ڴ��ͽ�
#define SF_LASTSEG			  (USER_SFLAG+70)	//���Ķ�
#define SF_LASTSEC			  (USER_SFLAG+71)	//���Ľ�

//TxdIdle��ϢʱӦ���͵�֡���TITF(m_Txd Idle Task Flag)
#define TITF_Null		0	//��		
#define TITF_CallAll    2	//���ٻ�
#define TITF_CallAllCI  3	//���ٻ����
#define TITF_AutoSend   4	//�����ϴ�ģʽ

//�궨�����ļ�������
#define MAX_TXDFM_SIZE  255  //�����֡��
#define MAX_RXDFM_SIZE  255  //������֡��
#define MIN_RXDFM_SIZE  6    //��С����֡��
#define MIN_FM_SIZE 4
#define MAX_104SUBDEVNO	5	//104��Լ�ҽ�ģ�����

#define T1 20  	//T1ʱ������information of Test APCI's
#define T2 5  	//T2ʱ������Supervisory acknowledgements(t2<t1)
#define T3 20  	//T3ʱ������idle state(t3>t1);

#define STARTHEAD	0x68
#define ACT			0x1
#define CON			0x2
#define GETBIT1		0x1
#define GETBIT2		0x3 
#define IEC_K_104	0x7 

#define  STARTDT_ACT 0x7
#define  STARTDT_CON 0x0B
#define  STOPDT_ACT  0x13
#define  STOPDT_CON  0x23
#define  TESTFR_ACT  0x43
#define  TESTFR_CON  0x83
#define  MAXSEQUENCER   0x8000

//���� K W��ֵ
#define IEC104_K			12
#define IEC104_W			8

#define T1_TIMERID			(IEC104_K+0)		//T1��ʱ��
#define T2_TIMERID			(IEC104_K+1)		//T2��ʱ��
#define T3_TIMERID	 		(IEC104_K+2)		//T3��ʱ��			
#define BO_TIMERID			(IEC104_K+3)		//ң��Ԥ�ó�ʱ��ʱ��
#define MAX_TIMERNUM		(IEC104_K+4) 		//��ʱ���ܸ���

//��Ϣ���ַ����
#define AI_ADDR_START  0x4001	//ң������ʼ���
#define AI_ADDR_STOP   0x5000	//ң������ֹ���
#define BI_ADDR_START  0x0001	//ң������ʼ���
#define BI_ADDR_STOP   0x1000	//ң������ֹ���
#define CI_ADDR_START  0x6401	//�������ʼ���
#define CI_ADDR_STOP   0x6600	//�������ֹ���
#define BO_ADDR_START  0x6001	//ң������ʼ���
#define BO_ADDR_STOP   0x6200	//ң������ֹ���
#define AO_ADDR_START  0x6201	//ң������ʼ���
#define AO_ADDR_STOP   0x6400	//ң������ֹ���

#define STSELECT	0				//Ԥ��
#define STEXEC		1				//ִ��	
#define STCANCEL	2				//����
#define STSBOSELECT	1<<3			//��ѹ��Ԥ��
#define STSBOEXEC	1<<4			//��ѹ��ִ��
#define STSBOCANCEL	1<<5			//��ѹ�峷��
#define STSBONONE	1<<6			//��������ѹ��

#define MAX_IND8D_FDNUM		37
#define MAX_IND8C_FDNUM		32

typedef struct 
{
    unsigned char Start; //�����ַ�
    unsigned char Length; //����
    unsigned char Control1; //������1
    unsigned char Control2; //������2
    unsigned char Control3; //������3
    unsigned char Control4; //������4
    unsigned char Style;   //���ͱ�ʶ
    unsigned char Definitive; //�ṹ�޶���
    unsigned char Reason_L; //����ԭ��
    unsigned char Reason_H; //����ԭ��
    unsigned char Address_L;   //������ַ����վվַ��
    unsigned char Address_H;   //������ַ����վվַ��
    unsigned char Data;  //���ݿ�ʼ
}TS104RxdFm,TS104TxdFm; //I104S��Լ�Ŀɱ�֡���ṹ    

typedef struct
{
   int    nTxdSeqNum; //��¼���Ͱ����
   BYTE   DataLen;//���ݳ���
   BYTE   DataBuf[MAXFRMLEN];  //���ݻ�����
}TDataBuf; //�����վ104�ķ��ͱ���

typedef struct 
{
	DWORD TxdIdleTxdFm; //TxdIdle��ϢʱӦ���͵�֡��� ��TITF_������
	WORD CallDataAddr; //�ٻ����ݵĵ�ַ��
	
}TS104Info; //S101��Լ��ÿһģ�����Ϣ     

//==== �ṹ������ ========

struct TProTimer{
	BOOL bTimer;
	WORD wTick;
	WORD wValue;
};

#define	S104_FILESATUS_IDLE		0
#define	S104_FILESATUS_INITDIR	0x01
#define	S104_FILESATUS_AUTODIR	0x02
#define	S104_FILESATUS_INITFILE	0x04
#define	S104_FILESATUS_SELFILE	0x08
#define	S104_FILESATUS_SELSEC	0x10

typedef struct
{
	BYTE		bStatus;	// ״̬����Ϣ
	BYTE		nCurDirNo;	// ��ǰ����Ŀ¼���	
	DWORD		dwInfoAddr;	// ��Ϣ���ַ
	BYTE		nFileNum;	// �ļ��ܸ���
	BYTE		nSentCnt;	// �ѷ����ļ�����
	char		strFileNameList[DIST_CAPACITY_MAX][DIST_INDEX_NAMELEN];//�ļ������б�
	TSysTime	tFileTimeList[DIST_CAPACITY_MAX];//¼���ļ�ʱ���
}TDistDirInfo;

typedef struct
{
	WORD        addrLen;		//������ַ����
	WORD        rsnLen;			//����ԭ�򳤶�
	WORD        elemaddrLen;	//��ϢԪ�س���

	WORD        yxbase;			//ң�Ż�ַ
	WORD		ycbase;			//ң���ַ
    WORD        ykbase;			//ң�ػ�ַ
	WORD        ddbase;			//��Ȼ�ַ

	WORD		yxfrmType;		//ң��֡���� 1-����,3-˫��
	WORD		soefrmType;		//SOE֡���� 2-��ʱ�� 30-����,31-˫��
	WORD		ycfrmType;		//ң��֡���� 9-��һ��,11-��Ȼ�,13-����  21-����Ʒ�������Ĺ�һ��ֵ
	WORD		ycbfrmType;		//�仯ң��֡���� 9-��һ��,11-��Ȼ�,13-����  21-����Ʒ�������Ĺ�һ��ֵ
	WORD		ykfrmType;		//ң��֡���� 45-������  46-˫����
	WORD		ddfrmType;		//���֡���� 15
	WORD		rptfrmType;		//����֡����

	WORD		intsetcode;		//���Ͷ�ֵ����
}TS104Para;

// ������
#define MAXFILENUM			2
#define PERSECDATALEN		(10*1024)
#define PERSEGDATALEN		200

typedef struct
{
	BYTE*		pFileBuf;	// �ļ����ݻ�����
	
	DWORD		dwFileLen;	// ���ٻ����ļ�����
	DWORD		dwSecLen;	// ���ٻ��Ľڳ���
	WORD		nSubDir;	// ��Ŀ¼��		
	WORD		nFileName;	// �ļ�����		
	BYTE		nFileNo;	// �ļ����
	BYTE		bFileChs;	// �ļ�У���
	
	BYTE*		pSecStart;	// �����ݻ���
	BYTE		nSecNum;	// ����
	BYTE		nSecName;	// ������
	BYTE		bSecChs;	// ��У��

	WORD		nSegNum;	// ����
	WORD		nSegNo;		// �κ�

	BYTE*		pFileStart[MAXFILENUM];	//������ʼλֵ
	DWORD		dwDataLen[MAXFILENUM];	//���ݳ���
	BYTE		bCheck[MAXFILENUM];	//����У���
	BYTE		bReady[MAXFILENUM];	//�ļ�׼����
}TDistFileInfo;


class CGm104 : public CProtocol
{
	DECLARE_MESSAGE_MAP()
//��׼��Լ����
protected:
	DWORD		m_dwTaskRunCount;	//������������ʱ��ͳ��(��)
	WORD		m_NoRxdCounter;		//�޽������ݼ���
	WORD  		m_LocalSTNum;		//��վ��ֵ����
	WORD		m_wFaultNo;
	
	TProTimer 	m_timer[MAX_TIMERNUM];

    WORD 		m_dwTxdCount;  		//��ǰ����֡����
    WORD 		m_dwRxdCount;  		//��ǰ����֡���� 
    BYTE		BufWritePtr,BufAckPtr;
	
    BOOL 		bIsSTARTDT; 		//�������ݷ��ͱ�־ 
    BOOL 		Is101BO;			//�ж�101����104ң�ظ�ʽ
	BYTE		m_nLeftActionReportNum;
	WORD		m_wChangeAIReadPtr;

	WORD  m_nSOENoAckCnt;		//SOE���͸���
	WORD  m_nSOENewTxSeq;		//SOE�������
	
	WORD  m_nBIENoAckCnt;		//BIE���͸���
	WORD  m_nBIENewTxSeq;		//BIE�������

	WORD  m_nALMNoAckCnt;		//�澯���淢�͸���
	WORD  m_nALMNewTxSeq;		//�澯���淢�����

	WORD  m_nACTNoAckCnt;		//�������淢�͸���
	WORD  m_nACTNewTxSeq;		//�������淢�����

	WORD  m_nAIENoAckCnt;		//AIE���͸���
	WORD  m_nAIENewTxSeq;		//AIE�������
	
    TS104RxdFm *pRxdFm;  			//����֡ͷָ��
    TS104TxdFm *pTxdFm;  			//����֡ͷָ��
    CommBuf 	m_Txd;
    
    TS104Info  	m_S104Info;				//S104��Լ��Ϣ 
    TDataBuf   	m_TxDataBuf[IEC104_K];	//����104��ԼK֡���ݰ�
    
	TBO	  		m_BO;
	TST			m_ST;
	BYTE				m_CurrentReportType;	//��ǰ���ͱ�������
	TRelayReport		m_tCurrentReport;
	TRelayEventHead		*m_pNextRelayEvent;

	long		*m_pOldAI;

	TDistDirInfo	m_DistDir;
	TDistFileInfo	m_DistFile;
	TS104Para		m_S104para;
	
public:
    BOOL Init(void);//��ʼ��
	BOOL InitPara(void);
    CGm104(DWORD *pdwApp):CProtocol(pdwApp)
	{ 
	    m_dwTxdCount = 0;  //����֡����
	    m_dwRxdCount = 0;  //����֡����
	    bIsSTARTDT = false;
	    Is101BO = FALSE;
    };
	
protected:
//�������ĵĴ���
    BOOL	RxdAPCIInfo();        //���մ���APCI��Ϣ
    BOOL	Txd68_ShortTime();    //���Ͷ�ʱ��ʱ��
    BOOL	Txd68_LongTime(TSysTime *pTime);     //���ͳ�ʱ��ʱ�� ȡ3610ʱ��

    BOOL	Handle_I_Format();    //����I��ʽ
    BOOL	Handle_U_Format();    //����U��ʽ
    BOOL	Handle_S_Format();    //����S��ʽ     
    
	BOOL	Txd_InitOver(void);		//��վ��ʼ�����
	BOOL	ResetVar(void);			//��ʼ��׃��

	// ��ʱ�ط�����
	BOOL 	MySetTimer(BYTE bTimerID, WORD wSecond);
	BOOL 	MyKillTimer(BYTE bTimerID);
	void 	MyCheckTimer();
	void 	MyOnTimer(BYTE bTimerID);
	BOOL 	SaveDataBuf(); 
	void 	RetryFrame(BYTE bItem);
	BOOL 	KillPrevFrame(WORD dwTxdNum);

	//��Ϣ��Ӧ����
	void 	OnCommIdle(DWORD dwLen,DWORD PortID);
    void	OnTimeOut(DWORD dwTimerID);
	void 	OnBIBurst();
	void  	OnSOE();
	void 	OnRelayEvent(DWORD MsgCode, DWORD wParam);
	void 	OnAIE();

	//ң��Ԥ�÷�У��Ϣӳ��
	void 	OnBOSelectEcho(WORD switchno,WORD attrib,WORD result);	
	void 	OnBOExecEcho(WORD switchno,WORD attrib,WORD result);
	
	void 	OnEthLinked(DWORD dwPara);
	void 	OnEthClosed(DWORD dwPara);
	void 	OnDisturb(DWORD dwFileNo);
	
	//ͨ��ң����Ϣ��ʽ����ң��,Ĭ��Ϊ��Ҫ��У,���ڽϸ���(�豣��϶�ԭʼ��Ϣ)��ң��
	BOOL 	SetBO(TBO* pBO,BYTE NeedReturnFlag=1);
	//ͨ��ң��·��\����\���Է�ʽ����ң��,Ĭ��Ϊ��Ҫ��У,���ڲ��豣��ԭʼ��Ϣ��ң��
	BOOL 	SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag=1);
	BOOL 	SetBO(WORD Command){return SetBO(m_BO.No,Command,m_BO.Attrib);};
	BOOL 	HaveNewReport(void);
	BOOL 	SearchNewReport(TRelayReport &Report,BYTE &Type);
	BOOL 	SearchChangedAI(WORD *pAINo,long *pAIValue, DWORD ChangedValue, WORD wControlWord);//���ұ仯ң��
	void 	FreshOldAI();

    DWORD	SearchOneFrame(PBYTE Buf,WORD Len);
    void	OnFrame(BYTE* pBuf);
    BOOL	Rxd68(void); //����ASDU���ݵ�Ԫ��֡

//���ͱ��Ĵ�����
	BOOL	TxdMonitor(void);

    BOOL	Txd_Head(BYTE dwCode,WORD Reason);//���ͱ���ͷ
    BOOL	Txd_Tail(BYTE Num);//���ͱ���β

    BOOL	TxdCallAll(void);
    BOOL	TxdCallAllCI(void);
    BOOL	Txd_Class1(void); //����1������
    BOOL	Rxd10_0B(void);//0x0B �ٻ������û����� ??
    BOOL	Txd_Class2(void); //����2������

    BOOL	Rxd68_64(void);//���ٻ�/�ٻ�ĳһ������
    BOOL	Rxd68_64_14(void);//�������ٻ�����
    BOOL	Rxd68_64_14_6(void);//�������ٻ���������
    BOOL	Txd68_64_14_6(void);//�������ٻ���������
    BOOL	Txd_AIGroup(WORD GroupNo,BYTE Reason);//����һ��ң��
    BOOL	Txd_BIGroup(WORD GroupNo,BYTE Reason);//����һ��ң��
    BOOL	Txd_CIGroup(WORD GroupNo,BYTE Reason);//����һ����
    BOOL	Txd_AllStop(void);//�������ٻ�����֡
    BOOL	Rxd68_67(void);//���Ӵ���
    BOOL	Txd68_67(void);//���� 
    BOOL	Rxd68_6E(void);//���ò���
    BOOL	Txd68_6E(void);//���ò���
    BOOL	Rxd68_68(void);//������·
    BOOL	Txd68_68(void);//������·
	BOOL	Txd_NoData(void){ return 0; }; //������������֡
    BOOL	SearchClass1(void); //�������1������
    BOOL	Txd_BurstBI(void); //���ͱ仯ң��
    BOOL	Rxd68_65(void);//�ٻ����
    BOOL	Rxd68_65_5(void); //�ٻ�ĳ����
    BOOL	Txd68_65_5(void); //�ٻ�ĳ����
    BOOL	Rxd68_65_6(void); ////�ٻ����е��
    BOOL	Txd68_65_7(void); ////�ٻ����е��
    BOOL	Txd_AllCIStop(void);//�������ٻ���Ƚ���֡
    BOOL	Txd_Soe(void); //����SOE����
    BOOL	Txd_ChangeAI(void); //���ͱ仯ң������
    BOOL	Rxd68_66(void);//�������
    BOOL	Txd68_66_AI(WORD AINo);//�����ɶ������γɵ�ң������
    BOOL	Txd68_66_BI(WORD BINo);//�����ɶ������γɵ�ң������
    BOOL	Txd68_66_CI(WORD CINo);//�����ɶ������γɵĵ������

	BOOL	Rxd68_2E2F3B3C(void);//ң�ش���
    BOOL	Rxd68_2E2F3B3C_6(void);//ң��Ԥ��/ִ������
    BOOL	Rxd68_2E2F3B3C_8(void);//ң�س�������
    BOOL	Txd68_2E2F3B3C(BYTE Reason); //����ң��Ԥ��/ִ��/������У	
    
	//104��Լ��ʱ��Ĳ�������
    BOOL	Rxd68_6B(void);//��ʱ�������·
    BOOL	Txd68_6B(void);//��ʱ�������·
    BOOL	Rxd68_69(void); //��λRTU
	BOOL	Txd68_69_Ack(void);

	DWORD	ToDevID(BYTE* Devid);
	//�¹ʼ򱨴����� 
	BOOL	Txd_RelayEvent(void);	//���ͱ����¼�����

	//��ֵ������
	BOOL	Rxd_145(void);			//�ٻ���ֵ�����ٻ���ֵ
	BOOL	Rxd_147(void);			//�޸Ķ�ֵ��Ԥ�� ���޸Ķ�ֵ��ִ��
	BOOL	Rxd68_145_STRead();     //ȡ��ֵ
	BOOL	Rxd68_145_STSelect();   //�����޸Ķ�ֵԤ��

	BOOL	Txd68_145_STReadEcho(void);		//ȡ��ֵ����
	BOOL	Txd68_145_STSelectEcho(void);	//�����޸Ķ�ֵԤ�÷���
	BOOL	Txd68_145_STExecEcho(void);		//�޸Ķ�ִֵ��/��������
	
	BOOL	Txd68_145_STSReadEcho(void);	//ȡ��ֵ���ŷ���
	BOOL	Txd68_145_STSExecEcho(void);	//�޸Ķ�ֵ����ִ��/��������
	BOOL	Txd68_145_STSSelectEcho(void);	//�޸Ķ�ֵ����Ԥ�÷���

	void Txd_ElementAddr(DWORD dwAddr)
	{
		m_Txd.Buf[ m_Txd.WritePtr++ ] = LOLOBYTE(dwAddr); //��Ϣ���ַLo  :װ��ID

		if(NBYTES_ELMENTADDR == 2 || NBYTES_ELMENTADDR == 3)
			m_Txd.Buf[ m_Txd.WritePtr++ ] = LOHIBYTE(dwAddr); //��Ϣ���ַHi	װ��ID
		if(NBYTES_ELMENTADDR == 3)
			m_Txd.Buf[ m_Txd.WritePtr++ ] = HILOBYTE(dwAddr);	//��Ϣ���ַ:

		return;
	}

	DWORD GetNewSoeNum(void);
	DWORD GetNewBieNum(void);
	DWORD GetNewAIENum(void);
	void  ClearSentEvent(DWORD dwAckCount);
	void  ClearAllEvent();

	//¼���ļ���ѯ����
	BYTE CheckSum(BYTE * pBuf,DWORD dwLength);
	BOOL DistFileName2Time(char *strFileName,TSysTime *pTime);
	BOOL InitDistDir();
	BOOL InitDistFile(BYTE nFileName);
	BOOL SelectFile(BYTE nFileName);
	BOOL SelectSec(BYTE nSecName);

	BOOL	Rxd_122(void);					//�ٻ�
	BOOL	Rxd_124(void);					//ȷ���ļ�
	
	BOOL	Txd_126_RDSJ_DIR_AllDir(); 		//�ٻ��ϴ�Ŀ¼
	BOOL	Txd_126_RDSJ_DIR_SubDir(); 		//�ٻ�����Ŀ¼
	BOOL	Txd_126_RDSJ_DIR_Auto(); 		//�����ϴ�Ŀ¼
	BOOL	Txd_120_RDSJ_F_FR(BOOL bReady);	//�ļ�׼������	
	BOOL	Txd_121_RDSJ_F_SR(void);		//��׼������
	BOOL	Txd_125_RDSJ_F_SG(void);		//�ϴ��Ŷ����� ���ͱ�ʶ125��F_SG_NA_1 ��
	BOOL	Txd_123_RDSJ_F_LS(void);		//���Ľ�,���Ķ�

};

#endif
#endif
