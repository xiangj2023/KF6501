#ifndef _FLEXCAN_H_
#define _FLEXCAN_H_

#if(MCF5235_FLEXCAN == YES)

#include "system.h"

//假设系统时钟为75Mhz，CAN通信速率为100kbps，首先将系统频率预分频到1M，然后分别设定
//segment1=7(由PSEG1和PROPSEG组成), segment2=2,在加上同步段，0.5M/(4+3+2+1) = 0.05Mhz
#define CAN_PRESDIV		(IN_SYS_CLOCK/500000 - 1)	//定义预分频
#define CAN_PSEG1		2////2//7	
#define CAN_PSEG2		2////2//7
#define CAN_PROPSEG		2////2//7
#define CAN_RJW			1	//定义发送同步跳转宽度
#define CAN_LMD_FIRST	1	//定义发送优先级策略为低MD号首先发送

//定时所使用到的MB序号及方式
#define CAN_MB_TXD		0
#define CAN_MB_RXD		1
#define CAN_MB_DISALBE	2

#define CAN_MB0			0	//用作发送
#define CAN_MB1			1	//用作接收
#define CAN_MB14		14	//用作接收

#define MAX_MB_NO		15

//定义发送MB命令
#define MB_RX_INACTIVE		(0x0000<<8)
#define MB_RX_EMPTY			(0x0004<<8)
#define MB_RX_FULL			(0x0002<<8)
#define MB_RX_OVERRUN		(0x0006<<8)
#define MB_RX_BUSY			(0x0001<<8)	

//定义接收MB命令
#define MB_TX_INACTIVE		(0x0008<<8)
#define MB_TX_SEND_DATA		(0x000c<<8)	//数据帧发送结束后进入到MB_TX_INACTIVE状态

#define MB_CODE_MASK		(0x000f<<8)

//定义MB中控制标志位
#define MB_CLT_LEN_OFFSET	0
#define MB_CLT_RTR_OFFSET	4
#define MB_CTL_IDE_OFFSET	5
#define MB_CTL_SRR_OFFSET	6

#define MB_CTL_LEN_MASK		(0x0f<<MB_CLT_LEN_OFFSET)
#define MB_CTL_RTR_MASK		(0x01<<MB_CLT_RTR_OFFSET)
#define MB_CTL_IDE_MASK		(0x01<<MB_CTL_IDE_OFFSET)
#define MB_CTL_SRR_MASK		(0x01<<MB_CTL_SRR_OFFSET)

//定义所有中断屏蔽位，指正常的发送/接收中断
#define MB_INT_MASK	(MCF_CAN_IFLAG_BUF0I|MCF_CAN_IFLAG_BUF1I|MCF_CAN_IFLAG_BUF14I)

/*
//ID有关位置定义
//ID中相关信息偏移定义
#define ID_LFRAM_TYPE_OFFSET		0		//链路层报文类型偏移
#define ID_BMANAGER_ADDR_OFFSET		4		//备管理机地址
#define ID_MMANAGER_ADDR_OFFSET		5		//主管理机地址
#define ID_SMODLUE_GRP_ADDR_OFFSET	6		//从模块组播地址
#define ID_API_FRAM_INDEX_OFFSET	8		//应用层帧分包序号
#define ID_API_FRAM_END_OFFSET		16		//应用层分包结束标志
#define ID_SMODULE_PPP_ADDR_OFFSET	17		//从模块地址

#define ID_LFRAM_TYPE_MASK			(0x0f<<ID_LFRAM_TYPE_OFFSET)
#define ID_BMANAGER_ADDR_MASK		(0x01<<ID_BMANAGER_ADDR_OFFSET)
#define ID_MMANAGER_ADDR_MASK		(0x01<<ID_MMANAGER_ADDR_OFFSET)
#define ID_SMODLUE_GRP_ADDR_MASK	(0x03<<ID_SMODLUE_GRP_ADDR_OFFSET)
#define ID_API_FRAM_INDEX_MASK		(0xff<<ID_API_FRAM_INDEX_OFFSET)
#define ID_API_FRAM_END_MASK		(0x01<<ID_API_FRAM_END_OFFSET)
#define ID_SMODULE_PPP_ADDR_MASK	(0x7f<<ID_SMODULE_PPP_ADDR_OFFSET)

//每个分包序号累加值
#define FLEXCAN_FRAM_INDEX_ADD		(0x01<<ID_API_FRAM_INDEX_OFFSET)
*/
//定义复位计数门限，当错误中断次数超过此值时复位硬件
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

//定义CAN软件复位
#define FlexCanSReset()		(MCF_CAN_CANMCR0 |= MCF_CAN_CANMCR_SOFTRST)	

//定义CAN模块功能禁止
#define FlexCanMDis()		(MCF_CAN_CANMCR0 |= MCF_CAN_CANMCR_MDIS)

#define FlexCanEnb()		(MCF_CAN_CANMCR0 &= (~MCF_CAN_CANMCR_MDIS))

#define ClearMDCode(no)		(MCF_CAN0_MBUFX_CTRL(no) &= ~MB_CODE_MASK)	

//激活MB的接收功能
#define ActiveMBRxd(no)	\
do{	\
	MCF_CAN0_MBUFX_CTRL(no) &= (~MB_CODE_MASK);	\
	MCF_CAN0_MBUFX_CTRL(no) |= 0x0400;	\
}while(0);

//清除flexcan中断标志，置位清零
#define ClearFlexCanIntFlg(no)	((MCF_CAN_IFLAG0 |= (0x01<<no)))

//根据ID取从模块地址
#define GetFlexCanAdd(id)	((BYTE)(id>>ID_SMODULE_PPP_ADDR_OFFSET))

/*************************************************************************************
* flexcan硬件初始化                                                          
*************************************************************************************/
DWORD FlexCanOpen(WORD port);

/*************************************************************************************
* flexcan的MB功能初始化                                                          
*************************************************************************************/
void FlexCanMBInit(BYTE no, BYTE type);

/*************************************************************************************
* flexcan的MB0中断处理函数														  
*************************************************************************************/
void FlexCanInt0(void * arg);

/*************************************************************************************
* flexcan的MB1中断处理函数														  
*************************************************************************************/
void FlexCanInt1(void * arg);

/*************************************************************************************
* flexcan的MB14中断处理函数														  
*************************************************************************************/
void FlexCanInt14(void * arg);

/*************************************************************************************
* 中断服务程序中发送处理过程														  
*************************************************************************************/
void FlexCanTxd(BYTE no);

/*************************************************************************************
* 中断服务程序中接收处理过程														  
*************************************************************************************/
void FlexCanRxd(BYTE no);

/*************************************************************************************
* flexcan错误中断处理过程														  
*************************************************************************************/
void FlexCanErrInt(void * arg);

/*************************************************************************************
* flexcan数据发送														  
*************************************************************************************/
DWORD FlexCanWrite(WORD port, BYTE address, BYTE type, BOOL isNewWrite);

DWORD FlexCanReset(WORD port);

#endif

#endif
