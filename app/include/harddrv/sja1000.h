#ifndef _SJA1000_H_
#define _SJA1000_H_

#if(MCF5234_SJA1000 == YES)

#include "system.h"

//SJA1000�Ĵ�����ַ����
#define SJA_ADDR			EX_CAN_CS_BASE				//����
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


//ģʽ�Ĵ�������λ����
#define MODE_RM			0x01		//��λ
#define MODE_LOM		0x02		//ֻ��
#define MODE_STM		0x04		//�Լ�
#define MODE_AFM		0x08		//�����˲�
#define MODE_SM			0x10		//˯��

//����Ĵ�������λ����
#define CMD_TR			0x01		//��������
#define CMD_AT			0x02		//��ֹ����
#define CMD_RRB			0x04		//�ͷŽ��ջ�����
#define CMD_CDO			0x08		//����������
#define CMD_SRR			0x10		//�Խ���ģʽ

//״̬�Ĵ�������λ����
#define STS_RBS			0x01		//���ջ�����״̬
#define STS_DOS			0x02		//�������״̬
#define STS_TBS			0x04		//���ͻ�����״̬
#define STS_TCS			0x08		//�������״̬
#define STS_RS			0x10		//����״̬
#define STS_TS			0x20		//����״̬
#define STS_ES			0x40		//����״̬
#define STS_BS			0x80		//����״̬

//�ж�ʹ�ܼ��жϱ�־λ����
#define IR_IER_RI		0x01		//�����ж�
#define IR_IER_TI		0x02		//�����ж�
#define IR_IER_EI		0x04		//����澯�ж�
#define IR_IER_DOI		0x08		//�������
#define IR_IER_WUI		0x10		//����
#define IR_IER_EPI		0x20		//���������ж�
#define IR_IER_ALI		0x40		//�ٲö�ʧ�ж�
#define IR_IER_BEI		0x80		//���߳���
#define IR_ERR_MASK		(IR_IER_EI|IR_IER_DOI|IR_IER_EPI|IR_IER_ALI|IR_IER_BEI)

//����ʱ�ӷ�Ƶ�Ĵ�������λ
#define CDR_CLOSE_CLK	0x08		//�ر�ʱ��
#define CDR_RXINTEN		0x20		//
#define CDR_CBP			0x40		//
#define CDR_CAN_MOD		0x80		//CANģʽ

//����֡��Ϣ��ʾλ
#define EFF_FF			0x80
#define EFF_RTR			0x40

//����ʱ�ӷ�Ƶֵ
#define CDR_CD_VALUE	0x4			//fosc/10 = 2.5MHz		

#define BRT0_BRP		0x18			// 25/(2*25) = 0.5MHz(tscl)
#define BRT0_SJW		0x40

#define BRT1_VALUE		0x25		//ͨ������Ϊ0.5/(3+2+5)=0.05MHz	0x25(50K) 0x11(100K)0x23(62.5K)

#define Sja1000HDReset()		outportb(SJA_MODE, MODE_RM)
#define Sja1000Enable()		outport(SJA_MODE, 0x00)

//����ʱ�ӷ�Ƶ��PeliCanģʽ��10��Ƶ(25M/10=2.5Mhz)
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
