/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			UserLogic.cpp
* 
*   ���ģ�飺�߼�����
*
* 
*	���� �Զ���д����
* 
* 
* 
*   ���� _EmtpyUserFun_ ��д������ ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-12-30		��ʼ����
* 
*************************************************************************/

//#include "stdafx.h" 
#include "Logic.h"
#include "logicdef.h"
#include "relaydef.h" 
extern BYTE byLedFlashBuf[];

/****************************************************************************************
 * 
 * ��������: ��д������ ����д����������򼰲�������
 * 
 * ����˵��: 
 *			- WORD wParm, WORD wParm2
 *			  ����: 
 *			  ˵��: ����  �� JMP[BG1] �е�BG1
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: �̵���ֵ TRUE��FALSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
void _EmtpyUserFun_(WORD wParm, WORD wParm2, BOOL bContext) 
{
}

//���ڴ˶����Զ���д���������巽ʽ�ο�EmtpyWrite
//2011-6-16
//201 ���ƺ���
void LedFlash(WORD wRelayIndex,WORD LedFrequency, BOOL bContext)
{
	if(wRelayIndex <LED_RELAY_WORD_START) return;
	if(wRelayIndex >LED_RELAY_WORD_START + LED_RELAY_WORD_NUM) return;
	WORD wLedIndex = wRelayIndex - LED_RELAY_WORD_START;
	byLedFlashBuf[wLedIndex] <<= 1; // �ڸ�һλ��¼�ϴ�״̬,�������ж� 

	BYTE byFrequency = 0xF0 &(LedFrequency<<4);
	//��4Ϊ��Ƶ�ʣ� ����λ��ֵ�� ����bit0�� ���µģ�bit1����һ�ε�
	byLedFlashBuf[wLedIndex] = byFrequency | ( byLedFlashBuf[wLedIndex]&0xF);

	if(bContext)
	{
		byLedFlashBuf[wLedIndex] |= 0x01;
	}
	else
	{
		byLedFlashBuf[wLedIndex] &= (~0x01);
		
	}
}


// �Զ���д�̵����ֺ����б� �� �Զ��庯���밴���ݿ��Ӧ˳�����뺯���б�
USER_FUN_RELAY_WORD_PTR s_pfnUserFunList[] =
{
	//����������Զ���д����

	_EmtpyUserFun_,	//ʾ�� 200 �պ���
	
	LedFlash, //201


};


