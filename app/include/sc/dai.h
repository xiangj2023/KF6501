#ifndef _DAI_H_
#define _DAI_H_

#include "system.h"

//p100�ĵ��輫ֵ����Ӧ���¶�ֵ
#define MIN_R_VALUE		6026
#define MAX_R_VALUE		17953
#define MIN_T_VALUE		-10000
#define MAX_T_VALUE		20000

#define DAI_BUF_LEN		3
#define MAX_DAI_NUM		2		//����ֱ���¶ȸ���

//�ϴζ�ȡ�ĵ���ֵ�����¶ȱ��е��������
struct TOldDAIValu
{
	DWORD dwRVale;		//��ֵ	�Ŵ�100��
	SDWORD sdwTValue;	//�¶�ֵ	�Ŵ�100��
	WORD wIndex;		
};

struct TDAIBuf
{
	DWORD dwValue[MAX_DAI_NUM][DAI_BUF_LEN];
	BYTE bWritePtr;
	BYTE bDAINum;
};

void DAIInit();
void DAIProc();
SDWORD CalTemperatur(DWORD dwRvalue);
void bubble_sort(int *x, int n);

#endif
