/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			RelayMemInit.h
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
*		kangqian	2008-12-20		��ʼ����
* 
*************************************************************************/
#ifndef __RelayMemInit_H
#define __RelayMemInit_H
#include "system.h"
#include "relaydef.h"


#ifdef __cplusplus
extern "C"{
#endif

BOOL InitAllRelayMem(void);
BOOL ResetMemRM();
BOOL ResetMemBI();

BOOL InitMemSetting();
BOOL InitMemCFG();
BOOL InitMemSoftSwitch();
BOOL InitMemCoeff();
BOOL InitMemAssistSetting();

/**********************/
#ifdef __cplusplus
}
#endif

#endif /* __RelayMemInit_H */
