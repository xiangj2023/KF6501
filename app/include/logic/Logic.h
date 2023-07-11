/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Logic.h
* 
*   ���ģ�飺�ɱ���߼�����
*
* 
*	����	����ɱ���߼������ⲿ�ӿں���
* 
* 
* 
*   ����	ResetAllRelayWord	��λ���м̵���
*			ReadRelayWord	���̵���ֵ ����0��1
*			WriteRelayWord	д�̵��� 						
*			ClearKeepRelayWord	����Ա��̵ּ��������ΪCLEAR_ALL_KEEP_RELAY_WORD��������е�
*			SetTimeRelayWord	����ʱ��̵����ֵ������ͷ���ʱ��
*			RunLogicParseModule	����ִ���߼�����, ִ��һ��
*			RecordRelayWord	��¼�̵���ֵ ����word[],byte[]. ���� word[0]���Ҫ��¼ֵ�ü̵������������������Ǽ̵������					
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-12-29		��ʼ����
* 
*************************************************************************/

#ifndef __Logic_H
#define __Logic_H
#include "system.h"
#include "logicdef.h"
#include "relaydef.h" 

#define PLC_DEBUG_ST1 0x55AAAA55
#define PLC_DEBUG_ST2 0xEB9090EB

#ifdef __cplusplus
extern "C"{
#endif


// FuncType ��
//@TypeID	TypeName	TypeLabel	Describe
//@99	��ת�̵���	JMP	��ת������ ����Ϊ��Ż��к�
#define			JMP		99	///��ת�̵����� 100�Ժ�Ϊ�߼�����ϵͳ����
#define		SYSFUN_TYPE_BASE		100
//#define		LEDFLASH_FUN		109
#define		MAX_SYSFUN_TYPE		199 //���ϵͳ�������ͣ� 200���Ժ�Ϊ�û��Զ��庯������
#define		USERFUN_TYPE_BASE	200

//ExpType ��
//@	5	����̵���	Pulse	
#define			PULSE	5	//����̵��� ������Ϊ���峣��
#define			GE		8	//�Ƚϼ̵����� ����Ϊ��������ֵ��������
#define			CONSTTIME 9

//RelayWordType ��
#define RW_TYPE_COUNTER	28

//pfnSysFun����  ����1������2�� д��ֵ
typedef void (*USER_FUN_RELAY_WORD_PTR)(WORD, WORD,BOOL);
extern LONG g_PLCStatus;
extern BYTE byRelayWordValue[];
extern DWORD g_dwPLCDebugStep1, g_dwPLCDebugStep2, g_dwPLCCurLine,g_dwPLCBreakPoint;

#ifdef __PL_WIN_SIMULATION__
#define PLC_ONESTEP (WM_USER+1)
#endif

//�߼�����״̬
#define PLC_SYS_OK  1
#define PLC_DEFAULT_OK 2
#define PLC_STATUS_ERR 0

//@ �߼����̲����ļ�ͷ������ 
typedef struct _TLogicPara_Head{
	BYTE byFileFlag[8];			// �ļ���־ 
	DWORD dwFileLen;			// �ļ����� 
	DWORD dwRelayWordNum;		// �̵����ָ��� 
	DWORD dwExpressionNum;		// ���ʽ���� 
	DWORD dwCalcNodeNum;		// ִ�нڵ���� 
}kTLogicPara_Head;

typedef struct _TCommonFileHead
{
	DWORD dwSize; //�ļ��ܳ���
	DWORD dwVersion; //�������ݿ�汾��
//	WORD wFlag; //Ԥ�� 2012-3-2 ȥ��wFlag���ռ�鵽wVersion���DWORD
	DWORD dwMagic; //Ԥ��
	TSysTime sysTime; //����ʱ��
	DWORD dwCheck; //У����
}TCommonFileHead;


//@ ���ʽ�����ṹ 
typedef struct _TLogicPara_Expression{
	WORD wResultRelayWordIndex;	// ����̵�������� 
	WORD wResultRelayWordType;	// ����̵�����ִ������ 
	WORD wParm; //����  
	WORD wParm2; // ����2 2009-7-13 15:32:30 ���ӣ� 

	WORD wStartCalcNodeIndex;		//��ʼִ�нڵ�������
	WORD wCalcNodeNum;			// ִ�нڵ����
	
	WORD wParaEx; //��չ���� 2012-4-28 �޸����ӵ�
	WORD wParaEx2; //��չ����2 2012-4-28 �޸����ӵ�
}kTLogicPara_Expression;

//@ ִ�нڵ������ṹ 
typedef struct _TLogicPara_CalcNode{
	WORD wRelayIndex;				// �̵����������� 
	WORD wOffset;					// ��תƫ�� 
	WORD wCalcType;				// ���㷽ʽ 
	WORD wGetType;				// ��ȡ��ʽ 
}kTLogicPara_CalcNode;

// ʱ��̵����ֶ������������ṹ 
typedef struct TTimeRelayWordInf{
    DWORD dwState;                     // �̵�����״̬
    DWORD dwTime;                      // ��¼��ǰʱ��,�����ж������򷵻� 
}TTimeRelayWordInf;

typedef struct TPulseRelayWordInf{
	BOOL bIsRun; //����̵����Ƿ�����
	DWORD dwTime; //���ж�ʱ��
	DWORD dwTimeParam; //���ζ�ֵ
}TPulseRelayWordInf;

typedef struct TGERelayWordInf{
	LONG* npParm1;
	LONG* npParm2;
}TGERelayWordInf;

// ��λ(��)���еļ̵�����
void ResetAllRelayWord(void);

// ���̵����� 
//BOOL ReadRelayWord(WORD wRelayWordIndex);
#define ReadRelayWord(index)\
	(((byRelayWordValue[index] & 0x01) == 0x01) ? TRUE : FALSE)


// д�̵����֣������淽ʽд�� 
void WriteRelayWord(WORD wRelayWordIndex, BOOL bContext);


// ���Ա��̵ּ����� 
#define CLEAR_ALL_KEEP_RELAY_WORD	((WORD)-1)		// ��ȫ���Ա��̵ּ����� 
#define LOGIC_READ_ALL_VALUE ((WORD)-1)  //������ֵ
void ResetHoldRelayWord(WORD wRelayWordIndex, WORD wRs=0);

// ��ʱ��̵�����
void ResetTimeRelayWord(WORD wRelayWordIndex, WORD wRsv=0);

//����բ�̵�����
void ResetTripRelayWord(WORD wRelayWordIndex, WORD wRsv=0);

//�岻�����Ա��̵ּ���
void ResetNVRamHoldRelayword(WORD wRelayWordIndex, WORD wRsv=0);

//�źż̵�����
void ResetSignalRelayWord(WORD wRelayWordIndex, WORD wRsv=0);

//����
void CtrlTest(WORD wNo, BOOL  bIsStart);

//��������̵���
void ResetCount(WORD wRelayWordIndex, WORD wRsv=0);
void IncreaseCount(WORD wRelayWordIndex, WORD wOffset);
void DecreaseCount(WORD wRelayWordIndex, WORD wOffset);
//���̵�����Ŷ�����ֵ
WORD ReadCountByIndex(WORD wRelayWordIndex);
//�������̵����Ŷ�����ֵ
WORD ReadCountByNo(WORD CountNo);

//���ܣ��߼����̴���,�ⲿ�����߼�����ʱѭ�����øú���
void LogicProcess(void);


// ��ʼ���ɱ���߼�ģ��, ֻ����һ�Σ� ����洢�ռ�
BOOL InitLogicParseModule();

// ��λ�ɱ���߼�ģ��,���߼����̲������غ����
BOOL ResetLogicParseModule(BOOL bIsNewPLCDownLoad = FALSE);

//���߼��������ݿ�汾
DWORD GetPLCDBVer();
//���߼�����ͨ���ļ�У����
WORD GetPLCCheckSum();


/****************************************************************************************
 * 
 * ��������: //��¼�̵���ֵ
 * 
 * ����˵��: 
 *			- WORD* pwRelayIndexList
 *			  ����: 
 *			  ˵��: 
 *
 *			- WORD* pDst
 *			  ����: 
 *			  ˵��:  �̵�������б� ��ʽ ���̵ܼ�������n�� �̵���1��ţ� �̵���2���,..�̵���n���
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
BOOL RecordRelayWord(WORD* pwRelayIndexList, LONG* pDst);

//ˢ������̵�������ң�ŵ��̵����֣�
void UpDateInPut();

//ˢ�³��ڼ̵�����д���ڼ̵��������ڣ�
void UpDateOutPut();

//ˢ����ѹ�壨����ѹ�嵽�̵����֣�
void UpDateSW();

//ˢ�����ã������õ��̵�����)
void UpDateCFG();

//����led�̵�����
void UpDateLed();

//���µ��Կ���̵��� (�ӹ�Լ���������µ��̵�����2012-02-08
void UpDataDeBugInRW();

//���µ��Կ����̵��� (�Ӽ̵������µ���Լ��������2012-02-08
void UpDataDeBugOutRW();

//��������ң��
STATUS ReadAllVirDI(BYTE *pBuf);

//ң��Ԥ��
void RemoteCtrlPre(WORD wRCNum, BOOL bContext);

//����У�̵���
BOOL RemoteCtrlVer(WORD wRCNum );

//дң��ִ�м̵���
void RemoteCtrlExe(WORD wRCNum, BOOL bContext);

//2012-02-13 ���ؿ���ִ��
void LocalCtrlExe(WORD wLCNum, BOOL bContext);

//����
void Report_Start(WORD wSeqID,WORD wReportIndex);

void Report_StartByInf(WORD wSeqID, WORD wInf);

void Report_Action(WORD wSeqID,WORD wReportIndex);

void Report_End(WORD wSeqID, WORD wRsv=0);

void Report_Alarm(WORD wReportIndex, WORD wRsv=0);


/************************************************************
	���ܣ���ȡ��˸led��ֵ
	������
	     WORD wIndex[in] Ҫ����led��ţ� ΪLOGIC_READ_ALL_VALUEʱ��ȡ���е�led
	     BYTE* pByFrequency[out] ����ledƵ��
	     BYTE* pByValue[out] ����ledֵ
	���أ�LONG  0:��ȡ�ɹ� 1:��ȡʧ��
************************************************************/
LONG GetFlashLedValue(WORD wIndex, BYTE* pByFrequency, BYTE* pByValue);


/************************************************************
	���ܣ���ȡ�������Ӽ̵�����ֵ
	������
	     WORD wStartIndex[in] Ҫ���ķ������Ӽ̵�����ʼ���
	     WORD wCount[in] Ҫ���ķ������Ӽ̵�������
	     
	     BYTE* pByValue[out] ָ�򷵻�ֵ��ָ��
	���أ�LONG  0:��ȡ�ɹ� 1:��ȡʧ��
************************************************************/
LONG GetTxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue);
/************************************************************
	���ܣ����ý������Ӽ̵�����ֵ
	������
	     WORD wStartIndex[in] Ҫд�Ľ������Ӽ̵�����ʼ���
	     WORD wCount[in] Ҫд�Ľ������Ӽ̵�������
	     
	     BYTE* pByValue[in] ָ������ֵ��ָ��
	���أ�LONG  0:��ȡ�ɹ� 1:��ȡʧ��
************************************************************/

LONG SetRxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue);

/************************************************************
	���ܣ����õ��Լ̵�����ֵ
	������
	     WORD wIndex[in] Ҫд�ļ̵�������ţ�
	     
	     BYTE byVal[in] ֵ
************************************************************/
void WriteDebugRW(WORD wIndex,BYTE byVal);

/************************************************************
	���ܣ����̵���ȫֵ
	������
	     WORD wIndex[in] Ҫ���ļ̵�������ţ�
	     
	����:    BYTE  �̵���ֵ
************************************************************/
BYTE ReadRWFullValue(WORD wIndex);

/************************************************************
	���ܣ����߼����̵�ǰ�к�
	������
	     
	     
	����:    DWORD  ��ǰ����ִ�е��߼����̵��к�
************************************************************/
const WORD GetPLCCurLine(); 
const WORD GetPLCDebugBreakPoint();
void SetPLCDebugBreakPoint(DWORD dwVal);
void SetPLCDebugStep1(DWORD dwVal);
void SetPLCDebugStep2(DWORD dwVal);
void StopPLCDebug();
void SetPLCDebugLoop(DWORD dwVal);
void SetPLCDebugOneStep(DWORD dwVal);

/************************************************************
	���ܣ�д���Կ����ֵ
	������
	     WORD wDebugInIndex[in] Ҫд�ĵ��Կ�������
	     
	     BYTE byVal[in] ֵ
************************************************************/
void WriteDebugIn(WORD wDebugInIndex,BYTE byVal);

/************************************************************
	���ܣ������Կ�����ֵ
	������
	     WORD wDebugOutIndex[in] Ҫ���ĵ��Կ��������
	     
	����ֵ     BYTE ֵ
************************************************************/
BYTE ReadDebugOut(WORD wDebugOutIndex);

/*******************/

#ifdef __cplusplus
}
#endif

#endif /* __Logic_H */
