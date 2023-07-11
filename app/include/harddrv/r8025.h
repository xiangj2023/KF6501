#ifndef _R8025_H
#define _R8025_H

#include "system.h"
#include "timedef.h"


#define SECOND_REG	0x00
#define MINUTE_REG	0x01
#define HOUR_REG	0x02
#define WEEK_REG	0x03	//星期寄存器不能写操作
#define DAY_REG		0x04
#define MONTH_REG	0x05
#define YEAR_REG	0x06

#define OFFSET_REG	0x07	//数字补偿寄存器，用于调整精度

#define CONTROL1_REG	0x0e	
#define CONTROL2_REG	0x0f

#define R8025_BUF_LEN	12	//定义读写缓冲区长度

#define R8025_READ_ADDR		0x65	//IIC读操作地址
#define R8025_WIRTE_ADDR	0x64	//IIC写操作地址

void InitR8025(void);
DWORD WriteR8025Time(TSysTime *time);
DWORD ReadR8025Time(TSysTime *time);


#endif
