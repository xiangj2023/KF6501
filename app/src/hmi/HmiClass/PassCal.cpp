/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			PassCal.cpp
* 
*   ���ģ�飺��̬�������
*
* 
*	����
* 
* 
* 
*   ���� WORD g_kCalPass(DWORD dwInput) ��������룬���ض�̬����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2012-5-28		��ʼ����
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

	//�ȼ���һ���ۼӻ���
	for(i=0; i < nLen; i++)
	{
		wTmp += pBuf[i];
	}
	int nIndex = wTmp%9;

	//��ʽ����
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