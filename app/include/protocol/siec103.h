//*******************************************************************************************//
//  ģ������: sDF103.H
//  ģ�鹦��: ʵ��IEC103��վ��Լ��ͷ�ļ�����
//  ��������: 2010.6.5
//  ˵����
//*******************************************************************************************//
#ifndef _SIEC103_H
#define _SIEC103_H

//********************  ͷ�ļ�������  ********************
#include "protocol.h"
#include "msgmap.h"
#include "relaydef.h"
#include "iecdefs.h"

#ifdef INSTALL_S103 

//********************  �궨����  ************************
//���Ļ�������С����
#define MAX_TXDFRAME_SIZE  255  //�����֡��
#define MAX_RXDFRAME_SIZE  255  //������֡��
#define MIN_RXDFRAME_SIZE  5    //��С����֡��

#define MAX_COMMERR_TIME	10 		//10����������ͨ��ʧ��

//�㲥��ַ�Ķ���
#define MASTER_BROADCAST_ADDRESS  0xFF      //��վ�Դ�վ�Ĺ㲥


//֡ͬ���ַ��Ķ���
#define SYN_START_10   0x10     //�̶�֡�������ַ�
#define SYN_START_68   0x68     //�ɱ�֡�������ַ�
#define SYN_STOP       0x16     //֡�����ַ�


//==== ������Ķ��� ====

//��������λ����
#define PRM_MASTER     1        //����վ
#define PRM_SLAVE      0        //�Ӷ�վ

//֡������Чλ�ĺ궨��
#define FCV_VALID      1        //֡����λ��Ч
#define FCV_INVALID    0        //֡����λ��Ч

//Ҫ�����λ����
#define ACD_HAVECLASS1 1        //��1���û�����
#define ACD_NOCLASS1   0        //��1���û�����

//����������λ����
#define DFC_OK         0        //����
#define DFC_OVERFLOW   1        //��������

//������Ķ���
//FC_S ��ʾ���ӷ���(��վ)
#define FC_S_ACK          0     //ȷ��
#define FC_S_BUSY         1     //��·æ
#define FC_S_DATA         8     //��������Ӧ
#define FC_S_NODATA       9     //���ٻ�����
#define FC_S_LINKOK       11    //��·����
#define FC_S_LINKERROR    14    //��·����δ����
#define FC_S_LINKUNFIN    15    //��·����δ���

//FC_M ��ʾ���Ʒ���(��վ)
#define FC_M_RESETCU      0     //��λͨ�ŵ�Ԫ
#define FC_M_DATA         3     //��������,Ҫ��ȷ��
#define FC_M_BROADCAST    4     //��������,��Ӧ��
#define FC_M_RESETFCB     7     //��λ֡����λ
#define FC_M_CALLLINK     9     //�ٻ���·״̬
#define FC_M_CALLCLASS1   10    //�ٻ�1������
#define FC_M_CALLCLASS2   11    //�ٻ�2������


//==== Ӧ�÷������ݵ�Ԫ���� ====

//���ͱ�ʶ�Ķ���,1-31Ϊ���ݷ�Χ,32-255Ϊר�÷�Χ
//TYP_S ��ʾ���ӷ���(��վ)
#define TYP_S_TM       1        //��ʱ��ı���
#define TYP_S_TMR      2        //�����ʱ��ı���
#define TYP_S_ME1      3        //����ֵ��
#define TYP_S_TME      4        //�����ʱ��ı���ֵ
#define TYP_S_IDENT    5        //��ʶ
#define TYP_S_SST      6        //ʱ��ͬ��
#define TYP_S_CALLSTOP 8        //���ٻ���ֹ
#define TYP_S_ME2      9        //����ֵ��
#define TYP_S_ME3	   50		//����ֵIII

#define TYP_S_BISP 	   40		//����ң��
#define TYP_S_BIDP 	   41		//˫��ң��
#define TYP_S_BIGP 	   44		//����ң��

#define TYP_S_GDATA    10       //ͨ�÷�������
#define TYP_S_GIDENT   11       //ͨ�÷����ʶ

#define TYP_S_DD	   36		//���������֡
#define TYP_S_YKRTN    64		//ң�ط���֡
#define TYP_S_ACK88    88       //�ٻ����������ȷ��


//TYP_M ��ʾ���Ʒ���(��վ),
#define TYP_M_SST      6        //ʱ��ͬ��
#define TYP_M_CALL     7        //���ٻ�
#define TYP_M_GDATA    10       //ͨ�÷�������
#define TYP_M_COMMAND  20       //һ������
#define TYP_M_GCOMMAND 21       //ͨ������

#define TYP_M_CALLDD   88		//�ٻ����������
#define TYP_M_YKCOMMAND 64      //ң�ش���

//����ԭ��,1-63Ϊ���ݷ�Χ,64-255Ϊר�÷�Χ
//COT_S ��ʾ���ӷ���(��վ)
#define COT_S_BURST    1        //ͻ��
#define COT_S_CYCLE    2        //ѭ��
#define COT_S_RESETFCB 3        //��λ֡����λ
#define COT_S_RESETCU  4        //��λͨ�ŵ�Ԫ
#define COT_S_START	   5		// ��������������
#define COT_S_TEST     7        //����ģʽ
#define COT_S_SST      8        //ʱ��ͬ��

#define COT_S_CALL     9        //���ٻ�
#define COT_S_CALLSTOP 10       //���ٻ���ֹ
#define COT_S_LOCAL    11       //���ز���
#define COT_S_REMOTE   12       //Զ������
#define COT_S_ACK      20       //����Ŀ϶��Ͽ�
#define COT_S_NAK      21       //����ķ��Ͽ�
#define COT_S_GWACK    40       //ͨ�÷���д����Ŀ϶��Ͽ�
#define COT_S_GWNAK    41       //ͨ�÷���д����ķ��Ͽ�
#define COT_S_GRACK    42       //ͨ�÷�����������Ч������Ӧ
#define COT_S_GRNAK    43       //ͨ�÷�����������Ч������Ӧ
#define COT_S_GWANS    44       //ͨ�÷���дȷ��

//COT_M ��ʾ���Ʒ���(��վ)
#define COT_M_SST      8        //ʱ��ͬ��
#define COT_M_CALL     9        //���ٻ�������
#define COT_M_COMMAND  20       //һ������
#define COT_M_GWRITE   40       //ͨ�÷���д����
#define COT_M_GREAD    42       //ͨ�÷��������

//��������
#define FUN_BU         1        //�����Ԫ
#define FUN_TZ         128      //���뱣��
#define FUN_I          160      //��������
#define FUN_IT         176      //��ѹ�������
#define FUN_IL         192      //��·�����
#define FUN_GEN        254      //ͨ�÷���
#define FUN_GLB        255      //ȫ��
#define FUN_DC         252      //ֱ������

//��Ϣ���
//INF_S ��ʾ���ӷ���(��վ)
//0-15,ϵͳ����
#define INF_S_GLB              0     //����ȫ�ֹ�������

//16-31,״̬
#define INF_S_BI           	16    //ң��
#define INF_S_SOE          	17    //SOE

//144-159,����ֵ
#define INF_S_MEA_I            144   //����ֵI
#define INF_S_MEA_IV           145   //����ֵI,V
#define INF_S_MEA_IVPQ         146   //����ֵI,V,P,Q
#define INF_S_MEA_IVN          147   //����ֵIn,Ven
#define INF_S_MEA_2            148   //����ֵ2
#define INF_S_MEA_3            148   //����ֵ2

//INF_M ��ʾ���Ʒ���(��վ)
//0-15,ϵͳ����
#define INF_M_GLB              0     //����ȫ�ֹ�������

//һ�������ʶ DCO
#define DCO_M_TESTMOD		0   //����ģʽ
#define DCO_M_RUNMOD		1   //����ģʽ
#define DCO_M_CALLYX		10	//�ٻ�������
#define DCO_M_HISEVENT		20  //ȡ��ʱ������¼
#define DCO_M_TESTAD		30	//����ADͨ��
#define DCO_M_SYSINF		40	//ȡģ��ϵͳ��Ϣ
#define DCO_M_DEVREVERT		50	//װ�ø���

#define CMD_ST_SELECT   			0x0001
#define CMD_ST_EXECUTE  			0x0002
#define CMD_ST_CANCEL   			0x0004
#define CMD_STS_READ    			0x0008
#define CMD_STS_SELECT  			0x0010
#define CMD_STS_EXECUTE 			0x0020
#define CMD_STS_CANCEL  			0x0040
#define CMD_ST_READ     			0x0080
#define CMD_CFG_READ     			0x0100
#define CMD_SBO_READ      			0x0200
#define CMD_SBO_SELECT    			0x0400
#define CMD_SBO_EXECUTE  			0x0800
#define CMD_SBO_CANCEL  			0x1000
#define CMD_SYSST_SELECT    		0x2000

//==== ���ݼ���Ķ��� ====
#define COL_NOGEN              2     //δ����ͨ�÷������
#define COL_GEN                3     //����ͨ�÷������

#define SF_RETRY			(USER_SFLAG+0)   //�ط���־
#define SF_CALLALL          (USER_SFLAG+1)   //���ٻ�
#define SF_CALLALL_AI       (USER_SFLAG+2)   //���ٻ�
#define SF_CALLALL_BI       (USER_SFLAG+3)   //���ٻ�
#define SF_CALLALL_STOP     (USER_SFLAG+4)	 //���ٻ�����
#define SF_HAVESOE          (USER_SFLAG+5)   //��SOE
#define SF_HAVEREPORT       (USER_SFLAG+6)   //�б���

#define SF_REPORT_DD		(USER_SFLAG+7)   //�ٻ����
#define SF_RESETCU			(USER_SFLAG+8)
#define SF_RESETFCB			(USER_SFLAG+9)
#define SF_START			(USER_SFLAG+10)
#define SF_SST_ACK			(USER_SFLAG+11)

#define SF_HaveBOReturn   	(USER_SFLAG+12)    //��ң�ط�У

#define SF_GEN_DATA					(USER_SFLAG+32)		//ͨ�����ݱ�־��
#define SF_GEN_READSTDATA			(USER_SFLAG+32)		//����ֵ
#define SF_GEN_READSTAREA			(USER_SFLAG+33)		//����ֵ��
#define SF_GEN_READSBO				(USER_SFLAG+34)		//����ѹ��
#define SF_GEN_READSYSSTDATA		(USER_SFLAG+35)		//��ϵͳ��ֵ
#define SF_GEN_WRITEWITHCONFIRM		(USER_SFLAG+36)		//��ȷ�ϵ�д
#define SF_GEN_WRITEWITHEXECUTE		(USER_SFLAG+37)		//��ִ�е�д
#define SF_GEN_WRITEWITHCANCEL		(USER_SFLAG+38)		//д��Ŀ��ֹ

//#define SF_GEN_WRITEALLSTDATA		(USER_SFLAG+35)		//д���ж�ֵ
//#define SF_GEN_WRITESTDATA			(USER_SFLAG+36)		//д������ֵ
//#define SF_GEN_WRITESTAREA			(USER_SFLAG+37)		//д��ֵ����
//#define SF_GEN_WRITESBO				(USER_SFLAG+38)		//д��ѹ��
//#define SF_GEN_WRITESYSSET			(USER_SFLAG+39)		//д��ѹ��

//==== Ӧ�÷������ݵ�Ԫ���� ====

//���ͱ�ʶ�Ķ���,1-31Ϊ���ݷ�Χ,32-255Ϊר�÷�Χ
//TYP_S ��ʾ���ӷ���(��վ)
//TYP_M ��ʾ���Ʒ���(��վ)
#define TYP_S_CALLSPI   40					//���ٻ�ʱ����ĵ�����Ϣ״̬֡				 
#define TYP_S_BURSTSPI  41					//��ʱ��ĵ�����Ϣ״̬�仯֡
#define TYP_S_CALLDPI	42					//���ٻ�ʱ�����˫����Ϣ״̬֡
#define TYP_S_BURSTDPI	43					//��ʱ���˫����Ϣ״̬�仯֡
#define TYP_S_CALLYX	44					//���ٻ�ʱ����ĵ���״̬��״̬�仯��Ϣ֡
#define TYP_S_REPORT	61					//�����ʱ��Ĺ��ϱ��汨��

#define PBO_NEW         0x40    //��ң������
#define PBO_BUSY        0x41    //ң�����ڴ����������·����ȴ���У��
#define PBO_OK          0x42    //ң�ط�У��ȷ��ִ����ȷ
#define PBO_ERR         0x43    //ң�ش��󣨲�֧�ֵ�ң�����

/*��Ҫ�ڱ���ͷ�ļ����涨��*/
#define KF6500_TYPE_AI	TYP_S_ME3
#define KF6500_TYPE_BI	TYP_S_ME2

#define M_TYPE_Ia		0//M_I
#define M_TYPE_Ib		0//M_IT
#define M_TYPE_Ic		0//M_IF
#define M_TYPE_Ua		0//M_U1
#define M_TYPE_Ub		0//M_U2
#define M_TYPE_Uc		0//M_U3
#define M_TYPE_P		0//M_P
#define M_TYPE_Q		0//M_Q
#define M_TYPE_PF		0//RM_FZA

#define DEV_FUN_TYPE	FUN_BU

#define MAX_IND61_FDNUM	38 		//61֡�������ϵ�������

#pragma	pack(1)

//������ṹ����(���Ʒ���)
typedef struct
{
	BYTE RES:1;            		//����
	BYTE PRM:1;                 //����վ��ʶλ
	BYTE FCB:1;                 //֡����λ
	BYTE FCV:1;                 //֡������Чλ
	BYTE FC:4;                  //������
}T_M_Control;

//������ṹ����(���ӷ���)
typedef struct
{
	BYTE RES:1;            		//����
	BYTE PRM:1;                 //����վ��ʶλ
	BYTE ACD:1;                 //Ҫ�����λ
	BYTE DFC:1;                 //����������
	BYTE FC:4;                  //������
}T_S_Control;

//Ӧ�÷������ݵ�Ԫ�ṹ
typedef struct
{
	BYTE byTYP;                 //���ͱ�ʶ
	BYTE byVSQ;                 //�ɱ�ṹ�޶���
	BYTE byCOT;                 //����ԭ��
	BYTE byADDRESS;             //Ӧ�÷���Ԫ��ַ
	BYTE byFUN;                 //��������
	BYTE byINF;                 //��Ϣ���
	BYTE byData;                //������
	BYTE byGenData;				//ͨ��������
}TAsdu;

//�̶���������֡ͷ�ṹ(���Ʒ���)
typedef struct
{
	BYTE byStart;               //�����ַ�
	T_M_Control Control;        //������
	BYTE byAddress;             //��ַ��
	BYTE byChecksum;            //֡У���
	BYTE byStop;                //�����ַ�
}T_M_FrameFix;

//�ɱ䳤������֡ͷ�ṹ(���Ʒ���)
typedef struct
{
	BYTE byStart1;              //�����ַ�
	BYTE byLength1;             //����
	BYTE byLength2;             //����(�ظ�)
	BYTE byStart2;              //�����ַ�(�ظ�)
	T_M_Control Control;          //������
	BYTE byAddress;             //��ַ��
	TAsdu Asdu;                 //Ӧ�÷������ݵ�Ԫ
}T_M_FrameVar;

//֡ͷ�ṹ(���Ʒ���)
typedef union
{
	T_M_FrameFix FrameFix_10;
	T_M_FrameVar FrameVar_68;
}T_M_FrameHead;

//�̶��������ݼ��ӷ���֡ͷ�ṹ
typedef struct
{
	BYTE byStart;               //�����ַ�
	T_S_Control Control;        //������
	BYTE byAddress;             //��ַ��
	BYTE byChecksum;            //֡У���
	BYTE byStop;                //�����ַ�
}T_S_FrameFix;

//�ɱ䳤�����ݼ��ӷ���֡ͷ�ṹ
typedef struct
{
	BYTE byStart1;              //�����ַ�
	BYTE byLength1;             //����
	BYTE byLength2;             //����(�ظ�)
	BYTE byStart2;              //�����ַ�(�ظ�)
	T_S_Control Control;        //������
	BYTE byAddress;             //��ַ��
	TAsdu Asdu;                 //Ӧ�÷������ݵ�Ԫ
}T_S_FrameVar;

//���ӷ���֡ͷ�ṹ
typedef union
{
	T_S_FrameFix FrameFix_10;
	T_S_FrameVar FrameVar_68;
}T_S_FrameHead;

#pragma pack()

//********************  �ඨ����  ************************
class CSiec103 : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	TCtlComm ctlComm;

	BOOL Init(void);//��ʼ��
    CSiec103(DWORD *pdwApp):CProtocol(pdwApp){};
	
protected:
	WORD  m_LocalYXNum;						//��վң����
	WORD  m_LocalYCNum;						//��վң����
	WORD  m_LocalDDNum;						//��վ�����
	WORD  m_LocalSTNum;						//��վ��ֵ����
	BYTE *m_OldYXTable;						//�����ϴ����ٻ����͵�ң��

	BYTE  	Time[7];
	BYTE	m_nLeftActionReportNum;
	WORD	m_NoRxdCounter;					//�޽������ݼ���
	
	TBO	  				m_BO;
	TST					m_ST;
	WORD				m_SBONo;			//��ѹ���
	TRelayReport		m_tCurrentReport;
	TRelayEventHead		*m_pNextRelayEvent;
	
	BYTE				*m_pLast68Frame;	//������һ֡���Ʒ�����
	CommBuf			m_Txd;
	
	T_S_FrameHead * pTxdFrameHead;     //����֡ͷָ��
	T_M_FrameHead * pRxdFrameHead;     //����֡ͷָ��

	TIECInfo		m_DevInfo;       //����ʱ��Ҫ�洢����Ϣ�ṹ

	DWORD	SearchOneFrame(PBYTE pBuf,WORD wLength);
	BYTE	CheckSum(BYTE * pBuf,BYTE byLength);		  //����У���
	BOOL	ReceiveControlFlag(BYTE byStart);            //������տ����ֵı�־λ
	
	//����֡������
	void OnFrame(BYTE* pBuf);   //���ս�֡

	BOOL Rxd10_ResetCU(void);              //���ո�λͨ�ŵ�Ԫ֡
	BOOL Rxd10_ResetFCB(void);			   //���ո�λ֡����λ֡
	BOOL Rxd10_Call_Link(void);			   //�ٻ���·״̬
	BOOL Rxd10_Call_Class1(void);		   //�ٻ�һ������
	BOOL Rxd10_Call_Class2(void);		   //�ٻ���������
	BOOL Rxd68_SST_6(void);				   //��ʱ
	BOOL Rxd68_CallAll_7(void);			   //���ٻ�
	BOOL Rxd68_Command_20(void);		   //һ������()
	BOOL Rxd68_YKCommand_64(void);		   //ң������
	BOOL Rxd68_CallDD_88(void);			   //�ٻ����

	BOOL Rxd68_GComm_21(void);
	BOOL Rxd68_STAreaRead_21();
	BOOL Rxd68_STDataRead_21();
	BOOL Rxd68_SBORead_21();
	BOOL Rxd68_SYSSTDataRead_21();
	
	BOOL Rxd68_GData_10(void);
	BOOL Rxd68_STDataWriteAll_10();
	BOOL Rxd68_STDataWriteOne_10();
	BOOL Rxd68_STAreaWrite_10();
	BOOL Rxd68_SBOWrite_10();
	BOOL Rxd68_SysSTDataWrite_10();

	//����֡������
	//======  ���ݷ�Χ  ============
	BOOL Txd10_BaseFrame(BYTE byFC);
	BOOL Txd10_ACK(void) { return Txd10_BaseFrame(FC_S_ACK); };
	BOOL Txd10_LinkOk_11(void) { return Txd10_BaseFrame(FC_S_LINKOK); };
	BOOL Txd10_NoData_9(void) { return Txd10_BaseFrame(FC_S_NODATA); };
	BOOL TxdClass1Data(void);
	BOOL TxdClass2Data(BYTE Reason);
	BOOL Txd68_AI_9(BYTE Reason);
	BOOL Txd68_AI_50(BYTE Reason);
	BOOL Txd68_RM_GEN(BYTE Reason);
	BOOL Txd68_Tm_1(void);
	BOOL Txd68_CallStop_8(void);
	BOOL Txd_Indication_5(BYTE Reason);
	BOOL Txd_SST_6(void);

	BOOL Txd68_STDataReadRtn_10(BYTE Group,BYTE Entry);
	BOOL Txd68_STAreaReadRtn_10();
	BOOL Txd68_SBOReadRtn_10();
	BOOL Txd68_GenWriteWithConfirm();
	BOOL Txd68_GenWriteWithExucute();

	//=======  ר�÷�Χ  ===========
	BOOL Txd68_DD_36(void);
	BOOL Txd68_CallAllYX_1(TEventBI *evSoe,BYTE Reason);
	BOOL Txd68_BI_40(BYTE Reason);
	BOOL Txd68_BI_44(BYTE Reason);
	BOOL Txd68_SOE_41(void);
	BOOL Txd68_Report_61();
	BOOL Txd68_BO_Echo_64(void);

	//========  ��Ӧ���� ============
	void OnTimeOut(DWORD id);
	void OnSOE();
	void OnRelayEvent(DWORD MsgCode, DWORD wParam);
	void OnBOSelectEcho(WORD switchno,WORD attrib,WORD result);
	void OnBOExecEcho(WORD switchno,WORD attrib,WORD result);	

	//����������
	BOOL HaveNewReport(void);
	BOOL HaveNewSOE(void);
	BOOL GetACDFlag(void);
	BOOL Txd68_Head(BYTE byFC);
	BOOL Txd68_Tail(void);
	BOOL SetAsduHead(BYTE byTYP,BYTE byVSQ,BYTE byCOT,BYTE byADDRESS,BYTE byFUN,BYTE byINF);
	void Make103P_BI(DWORD BIGroupNo);
	void Make103P_AI(DWORD AINo);
	void Make103P_Report(WORD wFaultNo,TRelayEventHead *pEvent);//TAbsTime *pStartTime);
	BOOL P103_YK_Pro(BYTE byYKNo,BYTE byActionTyp);
	BOOL SearchNewReport(TRelayReport &Report,BYTE &Type);
	
		//ͨ��ң����Ϣ��ʽ����ң��,Ĭ��Ϊ��Ҫ��У,���ڽϸ���(�豣��϶�ԭʼ��Ϣ)��ң��
	BOOL SetBO(TBO* pBO,BYTE NeedReturnFlag=1);
	//ͨ��ң��·��\����\���Է�ʽ����ң��,Ĭ��Ϊ��Ҫ��У,���ڲ��豣��ԭʼ��Ϣ��ң��
	BOOL SetBO(WORD BONo, WORD Command,WORD Attrib,BYTE NeedReturnFlag=1);
};

#endif

#endif

