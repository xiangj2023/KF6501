/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			logicmem.cpp
* 
*   ���ģ�飺
*
* 
*	����
* 
* 
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2009-1-6		��ʼ����
* 
*************************************************************************/
//window
//#ifdef __PL_WIN_SIMULATION__
//#include "stdafx.h" 
//#endif

#include "logicDef.h"
#include "Logic.h"
#include "PLConfig.h"
//@#include "PlatFormDef.h"

//��windows��ģ�������߼�����ʱʹ��
#ifdef __PL_WIN_SIMULATION__
    BYTE _PLDataBuf[10*1024];             /* ��windowģ��ƽ̨��ģ��e2,���߼����̵�.plc�ļ��� */
#endif

//�߼�����״̬

TTimeRelayWordInf TimeRelayWordInf[TIME_RELAY_WORD_NUM+1];  // ʱ��̵�����

//2010-4-21 ���ӳ���ʱ��̵������������
WORD TIMERelayWordConst[CONST_TIME_RW_NUM+1][2]; //ʱ��̵�����������(����ڷ�����д����)
TTimeRelayWordInf ConstTimeRelayWordInf[CONST_TIME_RW_NUM+1]; //����ʱ��̵���

DWORD TripRelayWordInf[TRIP_RELAY_WORD_NUM+1];  //��բ�̵�����ʱ

TPulseRelayWordInf PulseRelayWordInf[PULSE_RELAY_WORD_NUM+1]; //����̵���

TGERelayWordInf GERelayWordInf[GE_RELAY_WORD_NUM+1]; //�Ƚϼ̵���
LONG GEParmList[GE_RELAY_WORD_NUM+1]; //����Ƚϼ̵�������(�������)

BYTE byRelayWordValue[RELAY_WORD_NUM+1];

BYTE byVInValue[VIN_RELAY_WORD_NUM+1]; //��ң�ŵ�ֵ
BYTE byOutPreValue[OUTPUT_RELAY_WORD_NUM+1]; // ��¼���ڼ̵�������һ��״̬

//@WORD wInputRelayMap[INPUT_RELAY_WORD_NUM+1]; //����̵����뿪��ӳ���ϵ
//@WORD wVirInputRelayMap[VIN_RELAY_WORD_NUM+1]; //��ң���뿪��ӳ���ϵ

//2012-5-23 �޸Ŀ������Ժ�ң�Ÿ����͵�ȸ������仯�ˣ����Զ��ڹ�ϵ�������
//@LONG InputRelayWordBI[INPUT_RELAY_WORD_NUM+1]; //����̵�����BI�Ĺ�����ϵ


BYTE byDebugRelayBuf[VIRTURAL_RELAY_WORD_NUM+1]; //���Լ̵���������
//2011-6-16 10:37:49 Ϊ1300Led������˸�������� Ϊ������·�̵����ͽ�����·�̵������ӻ���
BYTE byLedFlashBuf[LED_RELAY_WORD_NUM+1]; 
BYTE byTxLinkBuf[TXLINK_RW_NUM+1]; ////+1��Ϊ�˷�ֹ����Ϊ0���������ڣ���Ҫ�������룬���º���Ҳ��Ҫ����������鷳
BYTE byRxLinkBuf[RXLINK_RW_NUM+1];

//2012-2-8 ���Լ̵���������
BYTE byDebugInRWBuf[DEBUG_IN_NUM+1];
BYTE byDebugOutRwBuf[DEBUG_OUT_NUM+1];

//2012-4-28 �����̵���
LONG nCountRelayBuf[CNT_RELAY_WORD_NUM+1];

#ifdef __PL_WIN_SIMULATION__
BYTE* g_byPLCSource = (BYTE*)(PL_ADDR_BASE);

BOOL g_bIsEndThread;
#endif

//��������ɹ����������


