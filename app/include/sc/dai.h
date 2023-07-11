#ifndef _DAI_H_
#define _DAI_H_

#include "system.h"

//p100的电阻极值及对应的温度值
#define MIN_R_VALUE		6026
#define MAX_R_VALUE		17953
#define MIN_T_VALUE		-10000
#define MAX_T_VALUE		20000

#define DAI_BUF_LEN		3
#define MAX_DAI_NUM		2		//定义直流温度个数

//上次读取的电阻值及在温度表中的索引序号
struct TOldDAIValu
{
	DWORD dwRVale;		//阻值	放大100倍
	SDWORD sdwTValue;	//温度值	放大100倍
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
