#ifndef _SJA1000_H_
#define _SJA1000_H_

#if(MCF5234_SJA1000 == YES)

#include "system.h"

//SJA1000寄存器地址定义
#define SJA_ADDR			EX_CAN_CS_BASE				//基质
#define SJA_MODE			(SJA_ADDR+0)					
#define SJA_COMMAND			(SJA_ADDR+1)
#define SJA__STATUS			(SJA_ADDR+2)
#define SJA_INTR			(SJA_ADDR+3)
#define SJA_INTENABLE		(SJA_ADDR+4)
#define SJA_BUSTIME0		(SJA_ADDR+6)
#define SJA_BUSTIME1		(SJA_ADDR+7)
#define SJA_OUTCTRL			(SJA_ADDR+8)
#define SJA_ARBLOSTCAP		(SJA_ADDR+11)
#define SJA_ERRCODECAP		(SJA_ADDR+12)
#define SJA_ERRWARNLMT		(SJA_ADDR+13)
#define SJA_RXERRCOUNT		(SJA_ADDR+14)
#define SJA_TXERRCOUNT		(SJA_ADDR+15)
#define SJA_ACSCODE			(SJA_ADDR+16)	//4
#define SJA_ACSMASK			(SJA_ADDR+20)	//4
#define SJA_RXDID			(SJA_ADDR+16)	//13
#define SJA_TXDID			(SJA_ADDR+16)	//13
#define SJA_TXDDATA			(SJA_ADDR+21)
#define SJA_RXDDATA			(SJA_ADDR+21)

#define SJA_RXMSGCOUNT	    (SJA_ADDR+29)
#define SJA_RXBUFSTRADDR	(SJA_ADDR+30)
#define SJA_CLOCKDIVIDER	(SJA_ADDR+31)
#define SJA_FIFO		    (SJA_ADDR+32)	//64
#define SJA_TXFIFOBUFFER	(SJA_ADDR+96)	//13


//模式寄存器功能位定义
#define MODE_RM			0x01		//复位
#define MODE_LOM		0x02		//只听
#define MODE_STM		0x04		//自检
#define MODE_AFM		0x08		//验收滤波
#define MODE_SM			0x10		//睡眠

//命令寄存器功能位定义
#define CMD_TR			0x01		//发送请求
#define CMD_AT			0x02		//终止发送
#define CMD_RRB			0x04		//释放接收缓冲器
#define CMD_CDO			0x08		//清除数据溢出
#define CMD_SRR			0x10		//自接收模式

//状态寄存器功能位定义
#define STS_RBS			0x01		//接收缓冲器状态
#define STS_DOS			0x02		//数据溢出状态
#define STS_TBS			0x04		//发送缓冲器状态
#define STS_TCS			0x08		//发送完毕状态
#define STS_RS			0x10		//接收状态
#define STS_TS			0x20		//发送状态
#define STS_ES			0x40		//错误状态
#define STS_BS			0x80		//总线状态

//中断使能及中断标志位定义
#define IR_IER_RI		0x01		//接收中断
#define IR_IER_TI		0x02		//发送中断
#define IR_IER_EI		0x04		//错误告警中断
#define IR_IER_DOI		0x08		//数据溢出
#define IR_IER_WUI		0x10		//唤醒
#define IR_IER_EPI		0x20		//错误消极中断
#define IR_IER_ALI		0x40		//仲裁丢失中断
#define IR_IER_BEI		0x80		//总线出错
#define IR_ERR_MASK		(IR_IER_EI|IR_IER_DOI|IR_IER_EPI|IR_IER_ALI|IR_IER_BEI)

//定义时钟分频寄存器功能位
#define CDR_CLOSE_CLK	0x08		//关闭时钟
#define CDR_RXINTEN		0x20		//
#define CDR_CBP			0x40		//
#define CDR_CAN_MOD		0x80		//CAN模式

//定义帧信息标示位
#define EFF_FF			0x80
#define EFF_RTR			0x40

//定义时钟分频值
#define CDR_CD_VALUE	0x4			//fosc/10 = 2.5MHz		

#define BRT0_BRP		0x18			// 25/(2*25) = 0.5MHz(tscl)
#define BRT0_SJW		0x40

#define BRT1_VALUE		0x25		//通信速率为0.5/(3+2+5)=0.05MHz	0x25(50K) 0x11(100K)0x23(62.5K)

#define Sja1000HDReset()		outportb(SJA_MODE, MODE_RM)
#define Sja1000Enable()		outport(SJA_MODE, 0x00)

//设置时钟分频，PeliCan模式，10分频(25M/10=2.5Mhz)
#define SetClkDiv()			outportb(SJA_CLOCKDIVIDER, CDR_CAN_MOD|CDR_CD_VALUE)

#define SetBusTime0()		outportb(SJA_BUSTIME0, BRT0_BRP|BRT0_SJW)
#define SetBusTime1()		outportb(SJA_BUSTIME1, BRT1_VALUE)	

#define SendRequest()		outportb(SJA_COMMAND, CMD_TR)
#define AbortSend()			outportb(SJA_COMMAND, CMD_AT)
#define ReleaseRxdBuf()		outportb(SJA_COMMAND, CMD_RRB)

struct TSja100Static
{
	DWORD dwIntCnt;
	DWORD dwTxdIntCnt;
	DWORD dwRxdIntCnt;
	DWORD dwErrorIntCnt;
};

DWORD Sja1000Open(WORD port);
void Sja1000Int(void * arg);
void Sja1000RxdInt(void);
void Sja1000TxdInt(void);
DWORD Sja1000Reset(WORD port);
DWORD Sja1000Write(WORD port, BYTE address, BYTE type, BOOL isNewWrite);


#endif

#endif
