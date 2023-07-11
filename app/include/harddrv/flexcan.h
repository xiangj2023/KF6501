#ifndef _FLEXCAN_H_
#define _FLEXCAN_H_

#if(MCF5235_FLEXCAN == YES)

#include "system.h"

//����ϵͳʱ��Ϊ75Mhz��CANͨ������Ϊ100kbps�����Ƚ�ϵͳƵ��Ԥ��Ƶ��1M��Ȼ��ֱ��趨
//segment1=7(��PSEG1��PROPSEG���), segment2=2,�ڼ���ͬ���Σ�0.5M/(4+3+2+1) = 0.05Mhz
#define CAN_PRESDIV		(IN_SYS_CLOCK/500000 - 1)	//����Ԥ��Ƶ
#define CAN_PSEG1		2////2//7	
#define CAN_PSEG2		2////2//7
#define CAN_PROPSEG		2////2//7
#define CAN_RJW			1	//���巢��ͬ����ת���
#define CAN_LMD_FIRST	1	//���巢�����ȼ�����Ϊ��MD�����ȷ���

//��ʱ��ʹ�õ���MB��ż���ʽ
#define CAN_MB_TXD		0
#define CAN_MB_RXD		1
#define CAN_MB_DISALBE	2

#define CAN_MB0			0	//��������
#define CAN_MB1			1	//��������
#define CAN_MB14		14	//��������

#define MAX_MB_NO		15

//���巢��MB����
#define MB_RX_INACTIVE		(0x0000<<8)
#define MB_RX_EMPTY			(0x0004<<8)
#define MB_RX_FULL			(0x0002<<8)
#define MB_RX_OVERRUN		(0x0006<<8)
#define MB_RX_BUSY			(0x0001<<8)	

//�������MB����
#define MB_TX_INACTIVE		(0x0008<<8)
#define MB_TX_SEND_DATA		(0x000c<<8)	//����֡���ͽ�������뵽MB_TX_INACTIVE״̬

#define MB_CODE_MASK		(0x000f<<8)

//����MB�п��Ʊ�־λ
#define MB_CLT_LEN_OFFSET	0
#define MB_CLT_RTR_OFFSET	4
#define MB_CTL_IDE_OFFSET	5
#define MB_CTL_SRR_OFFSET	6

#define MB_CTL_LEN_MASK		(0x0f<<MB_CLT_LEN_OFFSET)
#define MB_CTL_RTR_MASK		(0x01<<MB_CLT_RTR_OFFSET)
#define MB_CTL_IDE_MASK		(0x01<<MB_CTL_IDE_OFFSET)
#define MB_CTL_SRR_MASK		(0x01<<MB_CTL_SRR_OFFSET)

//���������ж�����λ��ָ�����ķ���/�����ж�
#define MB_INT_MASK	(MCF_CAN_IFLAG_BUF0I|MCF_CAN_IFLAG_BUF1I|MCF_CAN_IFLAG_BUF14I)

/*
//ID�й�λ�ö���
//ID�������Ϣƫ�ƶ���
#define ID_LFRAM_TYPE_OFFSET		0		//��·�㱨������ƫ��
#define ID_BMANAGER_ADDR_OFFSET		4		//���������ַ
#define ID_MMANAGER_ADDR_OFFSET		5		//���������ַ
#define ID_SMODLUE_GRP_ADDR_OFFSET	6		//��ģ���鲥��ַ
#define ID_API_FRAM_INDEX_OFFSET	8		//Ӧ�ò�֡�ְ����
#define ID_API_FRAM_END_OFFSET		16		//Ӧ�ò�ְ�������־
#define ID_SMODULE_PPP_ADDR_OFFSET	17		//��ģ���ַ

#define ID_LFRAM_TYPE_MASK			(0x0f<<ID_LFRAM_TYPE_OFFSET)
#define ID_BMANAGER_ADDR_MASK		(0x01<<ID_BMANAGER_ADDR_OFFSET)
#define ID_MMANAGER_ADDR_MASK		(0x01<<ID_MMANAGER_ADDR_OFFSET)
#define ID_SMODLUE_GRP_ADDR_MASK	(0x03<<ID_SMODLUE_GRP_ADDR_OFFSET)
#define ID_API_FRAM_INDEX_MASK		(0xff<<ID_API_FRAM_INDEX_OFFSET)
#define ID_API_FRAM_END_MASK		(0x01<<ID_API_FRAM_END_OFFSET)
#define ID_SMODULE_PPP_ADDR_MASK	(0x7f<<ID_SMODULE_PPP_ADDR_OFFSET)

//ÿ���ְ�����ۼ�ֵ
#define FLEXCAN_FRAM_INDEX_ADD		(0x01<<ID_API_FRAM_INDEX_OFFSET)
*/
//���帴λ�������ޣ��������жϴ���������ֵʱ��λӲ��
#define FLEXCAN_RESET_LEVEL		1000

struct TFlexCanStatic
{
	DWORD dwCanIntErrCnt;
	DWORD dwCanIntTxdErrCnt;
	DWORD dwCanIntRxdErrCnt;
	DWORD dwCanIntOKCnt;
	DWORD dwCanIntTxdOKCtn;
	DWORD dwCanIntRxdOKCtn;
	DWORD dwUnknowIntCnt;
	DWORD dwCanHardResetCnt;
	DWORD dwCanRxdOverRunCnt;
};

//����CAN�����λ
#define FlexCanSReset()		(MCF_CAN_CANMCR0 |= MCF_CAN_CANMCR_SOFTRST)	

//����CANģ�鹦�ܽ�ֹ
#define FlexCanMDis()		(MCF_CAN_CANMCR0 |= MCF_CAN_CANMCR_MDIS)

#define FlexCanEnb()		(MCF_CAN_CANMCR0 &= (~MCF_CAN_CANMCR_MDIS))

#define ClearMDCode(no)		(MCF_CAN0_MBUFX_CTRL(no) &= ~MB_CODE_MASK)	

//����MB�Ľ��չ���
#define ActiveMBRxd(no)	\
do{	\
	MCF_CAN0_MBUFX_CTRL(no) &= (~MB_CODE_MASK);	\
	MCF_CAN0_MBUFX_CTRL(no) |= 0x0400;	\
}while(0);

//���flexcan�жϱ�־����λ����
#define ClearFlexCanIntFlg(no)	((MCF_CAN_IFLAG0 |= (0x01<<no)))

//����IDȡ��ģ���ַ
#define GetFlexCanAdd(id)	((BYTE)(id>>ID_SMODULE_PPP_ADDR_OFFSET))

/*************************************************************************************
* flexcanӲ����ʼ��                                                          
*************************************************************************************/
DWORD FlexCanOpen(WORD port);

/*************************************************************************************
* flexcan��MB���ܳ�ʼ��                                                          
*************************************************************************************/
void FlexCanMBInit(BYTE no, BYTE type);

/*************************************************************************************
* flexcan��MB0�жϴ�����														  
*************************************************************************************/
void FlexCanInt0(void * arg);

/*************************************************************************************
* flexcan��MB1�жϴ�����														  
*************************************************************************************/
void FlexCanInt1(void * arg);

/*************************************************************************************
* flexcan��MB14�жϴ�����														  
*************************************************************************************/
void FlexCanInt14(void * arg);

/*************************************************************************************
* �жϷ�������з��ʹ������														  
*************************************************************************************/
void FlexCanTxd(BYTE no);

/*************************************************************************************
* �жϷ�������н��մ������														  
*************************************************************************************/
void FlexCanRxd(BYTE no);

/*************************************************************************************
* flexcan�����жϴ������														  
*************************************************************************************/
void FlexCanErrInt(void * arg);

/*************************************************************************************
* flexcan���ݷ���														  
*************************************************************************************/
DWORD FlexCanWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite);

DWORD FlexCanReset(WORD port);

#endif

#endif
