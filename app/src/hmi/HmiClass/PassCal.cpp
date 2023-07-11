/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			PassCal.cpp
* 
*   软件模块：动态密码计算
*
* 
*	描述
* 
* 
* 
*   函数 WORD g_kCalPass(DWORD dwInput) 输入随机码，返回动态密码
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2012-5-28		初始创建
* 
*************************************************************************/
#include "sysdefs.h"
#define PASSWORD_MAX 9999
WORD wBaseArray[10] = {0x1245,0x4590,0x890,0xFE89,0x55AA,0xEB90,0x9034,0xEB90,0x5FE3,0x93E3};
WORD g_kCalCheckSum(const BYTE *pBuf,DWORD nLen)
{
	WORD	wTmp = 0;
	DWORD	i;
	if(pBuf == NULL)
		return 0;

	//先计算一个累加基数
	for(i=0; i < nLen; i++)
	{
		wTmp += pBuf[i];
	}
	int nIndex = wTmp%9;

	//正式计算
	wTmp = wBaseArray[nIndex];
	for(i=0; i < nLen; i++)
	{
		wTmp += pBuf[i];
	}
	return ~wTmp;
}
WORD g_kCalPass(DWORD dwInput)
{
	BYTE* pBuf = (BYTE*)(&dwInput);
	WORD wRes =  g_kCalCheckSum(pBuf,4);
	return (wRes%PASSWORD_MAX);
}