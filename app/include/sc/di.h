#ifndef _DI_H_
#define _DI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbasedef.h"
#include "dbfunc.h"
#include "parafunc.h"

/********************************************************************************
		DI������ʱ����
********************************************************************************/
//ģ��DI������Ϣ
struct TDICfg
{
	BYTE bDiBoardNum;		//DI�����
	DWORD dwDiBoardAddr[MAX_DI_BOARD_NUM];	//����DI�����ַ
	BYTE bDiNumPerBoard[MAX_DI_BOARD_NUM];	//ÿ��DI�������DI·��
};

//����������ˢ�·�ʽ
//#define DB_IMCREMENT   		0x01                                                           
//#define DB_UPDATE      		0x02

//DIλ���Զ���
#define DI_IS_CI			0x01	//�ǵ��
#define DI_IS_REVERSE		0x02	//ȡ��
#define DI_CREAT_SOE		0x04	//�Ƿ����SOE

/********************************************************************************
		DI������ʱ�������
********************************************************************************/

#define DI_TIMER_PERIOD		10		//����DI��ʱ��ʱ����Ϊ10����

//����ң�Ű�ʶ�����ַ����
#define DI_BOARD_ID_ADD		0x04	
#define DI_BOARD_ID			0xa8


//����ȥ��ʱ��,��λΪ0.5MS����ң�Ųɼ���ʱ����ʱ����
#define MAX_SMOOTH_TIME			5000
#define DEFAULT_BI_SMOOTH_TIME	(40*2)		//ȱʡң��ȥ��ʱ��
#define DEFAULT_CI_SMOOTH_TIME	(50*2)		//ȱʡ���ȥ��ʱ��

//SOE��ʱ���������ȶ���
#define SOE_BUF_LEN		64

//DI��λ��ʱ������
#define DI_BUF_LEN		128

//����DI�ɼ��������״̬
#define DI_STEP_0		0
#define DI_STEP_3		3
#define DI_STEP_4		4
#define DI_STEP_5		5
#define DI_STEP_6		6
#define DI_STEP_9		9
#define DI_STEP_10		10
#define DI_STEP_11		11

//����ң������
#define MAX_DI_TYPE		3
#define REAL_DI			0
#define INNER_DI		1
#define VIR_DI			2

struct TDIType{
	WORD wDIBitNum[MAX_DI_TYPE];	//ÿ������DIλ�ĸ���
	WORD wDIPortBase[MAX_DI_TYPE];	//ÿ������DIλ��ռ�Ķ˿ڵ�ַ����ʼ��ַ
	WORD wDIPortNum;				//��������DI��ռ�еĶ˿ڵ�ַ��	
};

//DI�仯��¼���壬��¼����ͬһ���ֽڶ˿ڵ�ַ��ͬʱ������λ�Ķ��λ����Ϣ
struct TDIBuf{
	BYTE		bDIStatus;		//DI��ǰ״̬��ע����һ���˿ڵ�ַ�ϵ�����λ��Ϣ
	BYTE		bDIPortNo;		//����״̬�仯�Ķ˿ں�
	BYTE 		bDIChgBit;		//�����仯��λ��Ϣ(��λΪ1˵����λ�����˱�λ)
	DWORD		dwDITimeCnt;	//�����仯ʱDI��ʱ������ֵ���������ɺ���ʱ���¼
	TAbsTime	Time;			//�����仯ʱ��ϵͳʱ��
};

//DIλ��Ϣ�ṹ����
struct TDIBitData
{
	BYTE bStatusTab;		//DIλ״̬��
	BYTE bAttribute;		//DIλ���ԣ�����ң�Ż��ǵ�ȡ��Ƿ�ȡ����
	WORD wIndex;			//DI������,�˱�Ϊң�Ż������ʵ����ţ���Ҫ���ݲ�����ʼ��
	DWORD dwSmoothTime;		//DI����ȥ��ʱ��
	DWORD dwKeepTime;		//DI״̬����ʱ��
	TAbsTime qwChangeTime;		//DI״̬�仯ʱ��
};

//DI�˿ڷ�ʽ�ṹ����(һ��ʵ�ʰ������Ҫ��Ϊ���8λ�˿ڵ���Ϣ�ṹ)������ÿ��DI�߼���DI������һ��
//��8��������(��34·DI)�������Ҫ֪��ÿһ���ֽڶ˿���������Ч��DIλ����
struct TDIPortData
{
	DWORD dwPortAddr;		//���˿ڶ�Ӧ�������ַ
	BYTE bBiNumInPort;		//���˿�����ЧDIλ�ĸ���
	BYTE bOldStatus;		//�ϵ�DI״̬
//	BYTE bTaskOldStatus;	//�������ϵ�DI״̬
};

//DI�ɼ������ඨ��
class CDIProc:public CApp
{
protected:
    DECLARE_MESSAGE_MAP()
		
public:
	
	TEventBI *pSoeBuf;		//SOE��ʱ���壬����д���ݿ��Ƶ��
	WORD wSoeReadPtr;		//SOE��ʱ�����ָ��
	WORD wSoeWritePtr;		//SOE��ʱ����дָ��
	
	CDIProc(DWORD *pdwApp);
	~CDIProc();
	/*************************************************************************************
	* DI��ʼ��														  
	*************************************************************************************/
	void Init();
	
	/*************************************************************************************
	* DI�����ֽڶ˿ڵĲ�����ʼ��														  
	*************************************************************************************/
	void DIPortInit();

	/*************************************************************************************
	* DI����λ�Ĳ�����ʼ��														  
	*************************************************************************************/
	void DIBitInit();

	/*************************************************************************************
	* DI��ʼ״̬ȷ��														  
	*************************************************************************************/
	void BIStatusInit(BYTE type);

	/*************************************************************************************
	* DI��λ��Ϣ��Ӧ														  
	*************************************************************************************/
	void OnDIChange(DWORD id);

	/*************************************************************************************
	* DI��ʱ��Ϣ��Ӧ														  
	*************************************************************************************/
	void OnTimeOut(DWORD id);

	/*************************************************************************************
	* DI�ɼ������쳣����														  
	*************************************************************************************/
	void ErrorProc();
	
	/*************************************************************************************
	*  ����ָ���ֽڶ˿��������Ӧ���ܵ�DI���														  
	*************************************************************************************/
	WORD GetDIIndex(BYTE bPortNo);
	/*************************************************************************************
	*ʵʱ�޸�BI������Ч
	**************************************************************************************/
	void OnBiParaChange(DWORD id);
};

void DITask(DWORD *pdwApp);

void DITimerInt();
STATUS ScanDIPort(BYTE type);


#endif
