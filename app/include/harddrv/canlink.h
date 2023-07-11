#ifndef _CANLINK_H_
#define _CANLINK_H_

#include "msgmap.h"
#include "system.h"
#include "portdefs.h"
//��ʱ����

//��ַ��־����
#define MAIN_MANGER_ADDR		(0x01<<0)
#define BACK_MANAGER_ADDR		(0x01<<1)
#define SLAVE_MODULE_GRP_ADDR	(0x01<<2)

//���������ʱ��־���˿ڴ�ʱ��Ҫ���
#define ENABLE_BROAD_TIME		0x01

#define GetSModuleNum()		(10)	//ȡ��ģ�����
#define GetSModuleAdd(no)	(no)	//ȡ������ģ���ַ
#define isMastModule()		(FALSE)	//�Ƿ�Ϊ����ģ��

//��ʱ�������


//#define GetCanModulueAddr()	(0x1a&SLAVE_MODULE_ADDR_MASK)	//ȡ��ģ��CANͨ�ŵ�ַ
#define CAN_API_FRAME		1		
#define CAN_LINK_FRAME		2

#define MAX_CAN_PORT		2		//����CAN���˿ڸ���
#define MAX_MODULE_NUM		64		//�������ģ�������ע����ڴ�ģ�飬��������Ϊ1

//Ӧ�ò㱨�Ļ���������
//#define CAN_API_BUF_LEN		1024

//����Ӧ�ò�ͳһ���ջ���������
//#define CAN_PORT_BUF_LEN	(CAN_API_BUF_LEN*2)

//���建����״̬,������Ҫ4��״̬����-->׼����-->����æ-->����-->����,������Ҫ3������-->����æ-->����-->����
#define CAN_BUF_EMPTY		(0x00)		//��������
#define CAN_DATA_READY		(0x01)		//����������׼����,�ȴ�����
#define CAN_BUF_BUSY		(0x02)		//������һֱ���ڷ��ͻ����ĳ��Ӧ�ò㱨��״̬
#define CAN_DATA_FINISH		(0x03)		//�����жϷ�����ɻ���ս�����������Ҫ��·��ȷ��

//������״̬����
#define API_BMANAGER_DATA	(0x01<<0)	//��Ҫ�㲥���͸����ù�������ݱ�־
#define SUB_FRAM_INDEX_ERR	(0x01<<1)	//�ְ���Ŵ���
#define FRAM_LEN_ERR		(0x01<<2)	//���зְ��ܳ��ȴ���(������������С)
#define RXD_BUF_OVERRUN		(0x01<<3)	//���ջ��������أ��жϽ��յ��������޷�����
#define RESEND_FLAG			(0x01<<4)	//�ط���־

//������·����Ч���ݳ���
#define CAN_LINK_BUF_LEN	8

#define CAN_RESEND_LEVEL	3		//�����ط���ֵ

//�����׼ID��־λƫ��
#define SUB_FRAM_NO_OFFSET			(0)			//Ӧ�ò�ְ����
#define SUB_FRAM_END_OFFSET			(8)			//�ְ�����
#define LINK_FRAM_TYPE_OFFSET		(9)			//�������ͱ�ʾ
#define STANDBY_AREA_OFFSET			(13)		//��������
#define SLAVE_MODULE_ADDR_OFFSET	(17)		//��ģ���ַ
#define SLAVE_MOD_GRP_ADD_OFFSET	(25)		//��ģ���鲥��ַ
#define B_MANAGER_ADDR_OFFSET		(27)		//������ַ
#define M_MANAGER_ADDR_OFFSET		(28)		//������ַ

//�����ʾID������ʾ������
#define SUB_FRAM_NO_MASK			(0xff)		//Ӧ�ò�ְ����
#define SUB_FRAM_END_MASK			(0x01)		//�ְ�����
#define LINK_FRAM_TYPE_MASK			(0x0f)		//�������ͱ�ʾ
#define STANDBY_AREA_MASK			(0x0f)		//��������
#define SLAVE_MODULE_ADDR_MASK		(0xff)		//��ģ���ַ
#define SLAVE_MOD_GRP_ADD_MASK		(0x03)		//��ģ���鲥��ַ
#define B_MANAGER_ADDR_MASK			(0x01)		//������ַ
#define M_MANAGER_ADDR_MASK			(0x01)		//������ַ

#define GET_CAN_MODULE_ADD(id)		((id>>SLAVE_MODULE_ADDR_OFFSET)&SLAVE_MODULE_ADDR_MASK)
#define GET_CAN_FRAME_TYPE(id)		((id>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK)


//��·��֡���Ͷ���
#define API_N_FRAM_TYPE		(0x01)	//Ӧ�ò���ͨ����
#define API_U_FRAM_TYPE		(0x02)	//Ӧ�ò��������

#define LINK_BROAD_TIME		(0x08)	//�㲥��ʱ
#define LINK_ACK_TYPE		(0x09)	//�϶�ȷ�ϻش�
#define LINK_NACK_TYPE		(0x0a)	//�񶨻ش�
#define LINK_RESET_TYPE		(0x0b)	//��·��λ
#define LINK_FRAM_FLAG		(0x08)	//��·�㱨�ı�ʾ

/*
//��·����Ϣ�������Ͷ���
#define HD_TXD_API_FRAM_END		1	//Ӧ�ò����ݱ�Ӳ����ȫ����
#define HD_TXD_LINK_FRAM_END	2	//��·�㱨�ı�Ӳ����ȫ����
#define HD_RXD_API_FRAM_END		3	//Ӧ�ò����ݱ���ȫ����,���յ�����һ���ְ�Ϊ׼�����ǲ�һ����������ȷ
#define HD_RXD_LINK_FRAM_END	4
#define HD_RXD_LINK_FRAM_ACK	4	//���յ���·�϶��ش�(˵�����ͳɹ�)
#define HD_RXD_LINK_FRAM_NACK	5	//���յ���·�񶨻ش�(˵������ʧ��)
#define HD_RXD_BROAD_TIME		6	//���յ��㲥��ʱ����
*/

//����CAN��·����ʱʱ����
#define CAN_LINK_TIME		10	
#define CAN_BROADCAST_TIME	(10000/CAN_LINK_TIME)	//�㲥��ʱ���
#define CAN_PORT_BUSY_CNT	(5000/CAN_LINK_TIME)	//����˿�æ������ֵ
#define CAN_RESEND_TIME		(2000/CAN_LINK_TIME)	//��ʱ�ط��ȴ�ʱ��
#define CAN_RXD_BUSY_TIME	(6000/CAN_LINK_TIME)	//����æʱ����ֵ

#define CAN_HW_RESET_LEVEL	200		// Ӳ����λ��ֵ����������ʧ���ۼƴ���

//CAN�˿ڱ�־λ����
#define CAN_PORT_OPEN		(0x01<<0)
#define CAN_PORT_TXD_IDLE	(0x01<<1)
#define CAN_PORT_RXD_IDLE	(0x01<<2)


//����CAN����·���쳣����
#define CAN_PORT_TXD_BUSY_ERR		1
#define CAN_PORT_RXD_BUSY_ERR		2
#define CAN_API_TXD_BUF_BUSY_ERR	3
#define CAN_API_RXD_BUF_BUSY_ERR	4
#define CAN_LINK_TXD_BUF_BUSY_ERR	5
#define CAN_LINK_RXD_BUF_BUSY_ERR	6

//����CANͳһ���ͻ���������
#define CAN_TXD_PORT_BUF_NUM	2

#define CAN_FRAME_BUF_LEN		(512+64)		//���Ļ��泤��
#define CAN_API_BUF_NUM			4			//Ӧ�ò㱨�Ļ������

#define CAN_MONITOR_BUF_LEN		1024

//����CAN��·�����ݼ��ӻ�����
struct TCanMonitorBuf
{
	BYTE TxdBuffer[CAN_MONITOR_BUF_LEN];
	WORD wTxdWritePtr;
	BYTE RxdBuffer[CAN_MONITOR_BUF_LEN];
	WORD wRxdWritePtr;
};

//����ͳ�Ƽ�¼
struct TCanStatic
{
	DWORD dwApiRxdOkCnt;
	DWORD dwApiRxdErrCnt;
	DWORD dwApiTxdOkCnt;
	DWORD dwApiTxdErrCnt;
	DWORD dwApiRxdOverRun;
	DWORD dwApiTxdOverRun;
	DWORD dwApiResendCnt;

	DWORD dwLinkRxdOkCnt;
	DWORD dwLinkRxdErrCnt;
	DWORD dwLinkTxdOkCnt;
	DWORD dwLinkTxdErrCnt;
	DWORD dwLinkRxdOverRun;		//���չ���
};

struct TCanCellBuf
{
	BYTE Buffer[CAN_FRAME_BUF_LEN];
	WORD length;
	WORD ptr;
	DWORD dwStatus;		//������״̬
	DWORD dwFlag;		//���ݱ�־
	WORD wResendDelay;		//Ӧ�ò㱨�ĳ�ʱ�ط����
	WORD wResendCnt;		//��ʱ�ط�����
	DWORD dwCanID;
	DWORD dwRxdBusyCnt;		//����æ��������ʱ��Ҫ���쳣����
};

//CAN�豸�㻺��,ʵ��CAN��׼��Ԫ�Ĳ�ֺ�����Ļ���,�������������ɲ���
struct TCanDevBuf
{
	TCanCellBuf TxdBuf;
	TCanCellBuf RxdBuf;
};

//��·�㴦��֡�����ݳ��ȿ�����8�ֽ���
struct TCanLinkFram
{
	BYTE buffer[CAN_LINK_BUF_LEN];
	BYTE length;
	DWORD dwStatus;		//������״̬
	DWORD dwFlag;		//���ݱ�־
	DWORD dwCanID;
};

//CAN��·�㱨�Ļ��棬������ֱ������·�㴦�����ϴ���Ӧ�ò�,��˲����Ƕ��黺�棬���Լ�ʱ����
struct TCanLinkCmdBuf
{
	TCanLinkFram TxdBuf;
	TCanLinkFram RxdBuf;
};

struct TCanFramBuf
{
	BYTE Buffer[CAN_FRAME_BUF_LEN];
	WORD length;
	DWORD dwCanID;
};

//CANӦ�ò㱨�Ķ��壬���ͺͽ��ձ��Ļ�����Ϊ���Σ����ϲ�Ӧ�ú���·��֮���γɻ���
struct TCanApiFram
{
	TCanFramBuf TxdBuf[CAN_API_BUF_NUM];	
	BYTE TxdWBufNo;
	BYTE TxdRBufNo;
	BOOL isTxdBufEmpty;
	
	TCanFramBuf RxdBuf[CAN_API_BUF_NUM];	
	BYTE RxdWBufNo;
	BYTE RxdRBufNo;
	BOOL isRxdBufEmpty;
};

//CAN�˿����ݽṹ����
struct TCanPortData
{
	BYTE address;
	
	TCanDevBuf CanDevBuf;		//�豸�㻺�棬�����С��Ԫ�Ĳ�ֺ�����

	TCanLinkCmdBuf CanLinkCmdBuf;		//��·�������;

	TCanApiFram CanApiNFram;	//Ӧ�ò���ͨ֡����
	
	TCanApiFram CanApiUFram;	//Ӧ�ò����֡����,��Ҫ���ȴ���

	TCanMonitorBuf MonitorBuf;

	TCanStatic CanStatic;

	TCtlComm CommCtl;
	DWORD dwComIdleCnt;
	DWORD dwComIdleLevel;
};

//CAN�˿�����
struct TCanPort
{
	DWORD dwTargetId;		//�˿ڶ�ӦӦ�ò�����ID
	DWORD dwFlag;
	DWORD dwTxdBusyCnt;		
	DWORD dwRxdBusyCnt;
	BOOL bEnableBroadTime;	//�����ʱ
};

class CCanTask:public CApp
{
    DECLARE_MESSAGE_MAP()

	BYTE bModuleNum;
	DWORD dwTimeCnt;
	BYTE address; 
	
	private:
		//CAN��·��ʱ��Ϣ����														  
		void OnTimeOut(DWORD id);

		//CANͨ�����ɼ���
		void PortLoadMonitor(BYTE port);
		
		//��·��϶�ȷ�ϱ��ķ���
		DWORD TxdAck(BYTE port);

		//��·���ȷ�ϱ��ķ���
		DWORD TxdNAck(BYTE port);

		//CAN��·������
		void ErrProc(BYTE type);

		//ģ��㻺��������ݴ���
		void RxdProc(BYTE port);

		//ģ��㻺�巢�����ݴ���
		void TxdProc(BYTE port);

		//�˿ڻ���������
		void PortBufProc(BYTE port);

		void PortBuf2FramBuf(BYTE port);

		//���˿ڲ㷢�ͻ������Ƿ����
		BOOL isPortTxdBufIdle(BYTE port);

		//CANӲ�������շ���Ϣ��Ӧ����
		void OnCanHardWare(DWORD arg);

		//����Ӧ�ñ��Ĵ���
		void ApiRxdFramProc(BYTE port);

		//CAN��·������ʼ��
		DWORD CanLinkInit();
		
		DWORD LinkRxdFramProc(BYTE port);
	
	public:
		CCanTask(DWORD *pdwApp);
		void Init(void);
};		

/*************************************************************************************
* CAN����·���������														  
*************************************************************************************/
void CanLinkTask(DWORD *pdwApp);

/*************************************************************************************
* �������CAN�˿ڵĶ˿ڲ㷢�ͻ�����,�����ݸ��Ƶ���Ӧģ��㷢�ͻ�����														  
*************************************************************************************/
void PortBufToModuleBuf(BYTE port);

/*************************************************************************************
* ���ݴ�ģ���Ե��ַ�ͱ�־,ת��Ϊ��׼����ID														  
*************************************************************************************/
DWORD ChangeToCanID(BYTE address, BYTE flag);

/*************************************************************************************
* ȡģ��㷢��Ӧ�ñ������ݣ���Ӳ�������жϵ��á�														  
*************************************************************************************/
//BOOL GetTxdApiData(TCanApiBuf *pApiTxdBuf, BYTE *pData, WORD *pLen);

/*************************************************************************************
* ���ָ��CAN���˿��Ƿ��Ϳ���														  
*************************************************************************************/
BOOL isCanPortTxdIdle(BYTE port);

/*************************************************************************************
* CAN���˿ڹ���ӿڣ��˿ڴ�														  
*************************************************************************************/
DWORD CanOpen(BYTE bPortSort, DWORD dwFlag);

/*************************************************************************************
* CAN���˿ڹ���ӿڣ���ָ��CAN���˿ڵĽ��ն˿ڻ�������ȡһ����������														  
*************************************************************************************/
DWORD CanRead(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag);

/*************************************************************************************
* CAN���˿ڹ���ӿڣ���ָ��CAN���˿ڵķ��Ͷ˿ڻ�����д��һ����������														  
*************************************************************************************/
DWORD CanWrite(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag);

DWORD CanCtrl(BYTE portSort,TCtlComm *comm);

WORD GetCanTxdWritePtr(WORD portSort);

WORD GetCanRxdWritePtr(WORD portSort);

WORD ReadFromCanTxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen);

WORD ReadFromCanRxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen);

WORD ReadNewCanTxdData(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen);

WORD ReadNewCanRxdData(WORD portSort,BYTE* pBuf, WORD wOffset,WORD wLen);


//#define CAN_MASTER

#define FLEXCAN_PORT	1
#define SJA1000_PORT	2

class CCanTest:public CApp
{
    DECLARE_MESSAGE_MAP()

	BYTE bTxdBuf[CAN_FRAME_BUF_LEN];
	BYTE bRxdBuf[CAN_FRAME_BUF_LEN];
	BYTE bAnswerFram[20];
	BYTE flag;
	DWORD timecounter;
	WORD port;
	DWORD writecnt, readcnt;
	BYTE address;
	
	private:
		//CAN��·��ʱ��Ϣ����														  
		void OnTimeOut(DWORD id);
		void OnReadComm(DWORD dwPort, DWORD dwFlag);
		void OnCommIdle(DWORD dwLen,DWORD PortID);

	public:
		CCanTest(DWORD *pdwApp);
		void Init(void);
};

#endif
