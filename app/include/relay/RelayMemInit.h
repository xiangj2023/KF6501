/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			RelayMemInit.h
* 
*   软件模块：
*
* 
*	描述
* 
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-12-20		初始创建
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
