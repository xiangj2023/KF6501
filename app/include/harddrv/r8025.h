#ifndef _R8025_H
#define _R8025_H

#include "system.h"
#include "timedef.h"


#define SECOND_REG	0x00
#define MINUTE_REG	0x01
#define HOUR_REG	0x02
#define WEEK_REG	0x03	//���ڼĴ�������д����
#define DAY_REG		0x04
#define MONTH_REG	0x05
#define YEAR_REG	0x06

#define OFFSET_REG	0x07	//���ֲ����Ĵ��������ڵ�������

#define CONTROL1_REG	0x0e	
#define CONTROL2_REG	0x0f

#define R8025_BUF_LEN	12	//�����д����������

#define R8025_READ_ADDR		0x65	//IIC��������ַ
#define R8025_WIRTE_ADDR	0x64	//IICд������ַ

void InitR8025(void);
DWORD WriteR8025Time(TSysTime *time);
DWORD ReadR8025Time(TSysTime *time);


#endif
