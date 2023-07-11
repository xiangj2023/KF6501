#ifndef _CANLINK_H_
#define _CANLINK_H_

#include "msgmap.h"
#include "system.h"
#include "portdefs.h"
//临时定义

//地址标志定义
#define MAIN_MANGER_ADDR		(0x01<<0)
#define BACK_MANAGER_ADDR		(0x01<<1)
#define SLAVE_MODULE_GRP_ADDR	(0x01<<2)

//定义允许对时标志，端口打开时需要检查
#define ENABLE_BROAD_TIME		0x01

#define GetSModuleNum()		(10)	//取从模块个数
#define GetSModuleAdd(no)	(no)	//取各个从模块地址
#define isMastModule()		(FALSE)	//是否为主机模块

//临时定义结束


//#define GetCanModulueAddr()	(0x1a&SLAVE_MODULE_ADDR_MASK)	//取从模块CAN通信地址
#define CAN_API_FRAME		1		
#define CAN_LINK_FRAME		2

#define MAX_CAN_PORT		2		//定义CAN最大端口个数
#define MAX_MODULE_NUM		64		//定义最大模块个数，注意对于从模块，个数定义为1

//应用层报文缓冲区长度
//#define CAN_API_BUF_LEN		1024

//定义应用层统一接收缓冲区长度
//#define CAN_PORT_BUF_LEN	(CAN_API_BUF_LEN*2)

//定义缓冲区状态,发送需要4个状态空闲-->准备好-->发送忙-->结束-->空闲,接收需要3个空闲-->接收忙-->结束-->空闲
#define CAN_BUF_EMPTY		(0x00)		//缓冲区空
#define CAN_DATA_READY		(0x01)		//缓冲区数据准备好,等待发送
#define CAN_BUF_BUSY		(0x02)		//缓冲区一直处于发送或接收某个应用层报文状态
#define CAN_DATA_FINISH		(0x03)		//数据中断发送完成或接收结束，但是需要链路层确认

//缓冲区状态定义
#define API_BMANAGER_DATA	(0x01<<0)	//需要广播发送给备用管理机数据标志
#define SUB_FRAM_INDEX_ERR	(0x01<<1)	//分包序号错误
#define FRAM_LEN_ERR		(0x01<<2)	//所有分包总长度错误(超出缓冲区大小)
#define RXD_BUF_OVERRUN		(0x01<<3)	//接收缓冲区过载，中断接收到的数据无法保存
#define RESEND_FLAG			(0x01<<4)	//重发标志

//定义链路层有效数据长度
#define CAN_LINK_BUF_LEN	8

#define CAN_RESEND_LEVEL	3		//定义重发限值

//定义标准ID标志位偏移
#define SUB_FRAM_NO_OFFSET			(0)			//应用层分包序号
#define SUB_FRAM_END_OFFSET			(8)			//分包结束
#define LINK_FRAM_TYPE_OFFSET		(9)			//报文类型标示
#define STANDBY_AREA_OFFSET			(13)		//备用区域
#define SLAVE_MODULE_ADDR_OFFSET	(17)		//从模块地址
#define SLAVE_MOD_GRP_ADD_OFFSET	(25)		//从模块组播地址
#define B_MANAGER_ADDR_OFFSET		(27)		//备机地址
#define M_MANAGER_ADDR_OFFSET		(28)		//主机地址

//定义标示ID各个标示的掩码
#define SUB_FRAM_NO_MASK			(0xff)		//应用层分包序号
#define SUB_FRAM_END_MASK			(0x01)		//分包结束
#define LINK_FRAM_TYPE_MASK			(0x0f)		//报文类型标示
#define STANDBY_AREA_MASK			(0x0f)		//备用区域
#define SLAVE_MODULE_ADDR_MASK		(0xff)		//从模块地址
#define SLAVE_MOD_GRP_ADD_MASK		(0x03)		//从模块组播地址
#define B_MANAGER_ADDR_MASK			(0x01)		//备机地址
#define M_MANAGER_ADDR_MASK			(0x01)		//主机地址

#define GET_CAN_MODULE_ADD(id)		((id>>SLAVE_MODULE_ADDR_OFFSET)&SLAVE_MODULE_ADDR_MASK)
#define GET_CAN_FRAME_TYPE(id)		((id>>LINK_FRAM_TYPE_OFFSET)&LINK_FRAM_TYPE_MASK)


//链路层帧类型定义
#define API_N_FRAM_TYPE		(0x01)	//应用层普通报文
#define API_U_FRAM_TYPE		(0x02)	//应用层紧急报文

#define LINK_BROAD_TIME		(0x08)	//广播对时
#define LINK_ACK_TYPE		(0x09)	//肯定确认回答
#define LINK_NACK_TYPE		(0x0a)	//否定回答
#define LINK_RESET_TYPE		(0x0b)	//链路复位
#define LINK_FRAM_FLAG		(0x08)	//链路层报文标示

/*
//链路层消息参数类型定义
#define HD_TXD_API_FRAM_END		1	//应用层数据被硬件完全发送
#define HD_TXD_LINK_FRAM_END	2	//链路层报文被硬件完全发送
#define HD_RXD_API_FRAM_END		3	//应用层数据被完全接收,以收到最有一个分包为准，但是不一定完整或正确
#define HD_RXD_LINK_FRAM_END	4
#define HD_RXD_LINK_FRAM_ACK	4	//接收到链路肯定回答(说明发送成功)
#define HD_RXD_LINK_FRAM_NACK	5	//接收到链路否定回答(说明发送失败)
#define HD_RXD_BROAD_TIME		6	//接收到广播对时报文
*/

//定义CAN链路任务定时时间间隔
#define CAN_LINK_TIME		10	
#define CAN_BROADCAST_TIME	(10000/CAN_LINK_TIME)	//广播对时间隔
#define CAN_PORT_BUSY_CNT	(5000/CAN_LINK_TIME)	//定义端口忙计数限值
#define CAN_RESEND_TIME		(2000/CAN_LINK_TIME)	//超时重发等待时间
#define CAN_RXD_BUSY_TIME	(6000/CAN_LINK_TIME)	//接收忙时间限值

#define CAN_HW_RESET_LEVEL	200		// 硬件复位限值，连续发送失败累计次数

//CAN端口标志位定义
#define CAN_PORT_OPEN		(0x01<<0)
#define CAN_PORT_TXD_IDLE	(0x01<<1)
#define CAN_PORT_RXD_IDLE	(0x01<<2)


//定义CAN网链路层异常类型
#define CAN_PORT_TXD_BUSY_ERR		1
#define CAN_PORT_RXD_BUSY_ERR		2
#define CAN_API_TXD_BUF_BUSY_ERR	3
#define CAN_API_RXD_BUF_BUSY_ERR	4
#define CAN_LINK_TXD_BUF_BUSY_ERR	5
#define CAN_LINK_RXD_BUF_BUSY_ERR	6

//定义CAN统一发送缓冲区个数
#define CAN_TXD_PORT_BUF_NUM	2

#define CAN_FRAME_BUF_LEN		(512+64)		//报文缓存长度
#define CAN_API_BUF_NUM			4			//应用层报文缓存个数

#define CAN_MONITOR_BUF_LEN		1024

//定义CAN链路层数据监视缓冲区
struct TCanMonitorBuf
{
	BYTE TxdBuffer[CAN_MONITOR_BUF_LEN];
	WORD wTxdWritePtr;
	BYTE RxdBuffer[CAN_MONITOR_BUF_LEN];
	WORD wRxdWritePtr;
};

//定义统计记录
struct TCanStatic
{
	DWORD dwApiRxdOkCnt;
	DWORD dwApiRxdErrCnt;
	DWORD dwApiTxdOkCnt;
	DWORD dwApiTxdErrCnt;
	DWORD dwApiRxdOverRun;
	DWORD dwApiTxdOverRun;
	DWORD dwApiResendCnt;

	DWORD dwLinkRxdOkCnt;
	DWORD dwLinkRxdErrCnt;
	DWORD dwLinkTxdOkCnt;
	DWORD dwLinkTxdErrCnt;
	DWORD dwLinkRxdOverRun;		//接收过载
};

struct TCanCellBuf
{
	BYTE Buffer[CAN_FRAME_BUF_LEN];
	WORD length;
	WORD ptr;
	DWORD dwStatus;		//缓冲区状态
	DWORD dwFlag;		//数据标志
	WORD wResendDelay;		//应用层报文超时重发间隔
	WORD wResendCnt;		//超时重发次数
	DWORD dwCanID;
	DWORD dwRxdBusyCnt;		//接收忙计数，超时需要做异常处理
};

//CAN设备层缓存,实现CAN标准信元的拆分和重组的缓存,由驱动层代码完成操作
struct TCanDevBuf
{
	TCanCellBuf TxdBuf;
	TCanCellBuf RxdBuf;
};

//链路层处理帧，数据长度控制在8字节内
struct TCanLinkFram
{
	BYTE buffer[CAN_LINK_BUF_LEN];
	BYTE length;
	DWORD dwStatus;		//缓冲区状态
	DWORD dwFlag;		//数据标志
	DWORD dwCanID;
};

//CAN链路层报文缓存，该数据直接在链路层处理，不上传给应用层,因此不考虑多组缓存，可以及时处理
struct TCanLinkCmdBuf
{
	TCanLinkFram TxdBuf;
	TCanLinkFram RxdBuf;
};

struct TCanFramBuf
{
	BYTE Buffer[CAN_FRAME_BUF_LEN];
	WORD length;
	DWORD dwCanID;
};

//CAN应用层报文定义，发送和接收报文缓冲区为环形，在上层应用和链路层之间形成缓冲
struct TCanApiFram
{
	TCanFramBuf TxdBuf[CAN_API_BUF_NUM];	
	BYTE TxdWBufNo;
	BYTE TxdRBufNo;
	BOOL isTxdBufEmpty;
	
	TCanFramBuf RxdBuf[CAN_API_BUF_NUM];	
	BYTE RxdWBufNo;
	BYTE RxdRBufNo;
	BOOL isRxdBufEmpty;
};

//CAN端口数据结构定义
struct TCanPortData
{
	BYTE address;
	
	TCanDevBuf CanDevBuf;		//设备层缓存，完成最小信元的拆分和重组

	TCanLinkCmdBuf CanLinkCmdBuf;		//链路层命令缓存;

	TCanApiFram CanApiNFram;	//应用层普通帧缓存
	
	TCanApiFram CanApiUFram;	//应用层紧急帧缓存,需要优先处理

	TCanMonitorBuf MonitorBuf;

	TCanStatic CanStatic;

	TCtlComm CommCtl;
	DWORD dwComIdleCnt;
	DWORD dwComIdleLevel;
};

//CAN端口属性
struct TCanPort
{
	DWORD dwTargetId;		//端口对应应用层任务ID
	DWORD dwFlag;
	DWORD dwTxdBusyCnt;		
	DWORD dwRxdBusyCnt;
	BOOL bEnableBroadTime;	//允许对时
};

class CCanTask:public CApp
{
    DECLARE_MESSAGE_MAP()

	BYTE bModuleNum;
	DWORD dwTimeCnt;
	BYTE address; 
	
	private:
		//CAN链路定时消息处理														  
		void OnTimeOut(DWORD id);

		//CAN通道负荷监视
		void PortLoadMonitor(BYTE port);
		
		//链路层肯定确认报文发送
		DWORD TxdAck(BYTE port);

		//链路层否定确认报文发送
		DWORD TxdNAck(BYTE port);

		//CAN链路错误处理
		void ErrProc(BYTE type);

		//模块层缓冲接收数据处理
		void RxdProc(BYTE port);

		//模块层缓冲发送数据处理
		void TxdProc(BYTE port);

		//端口缓冲区处理
		void PortBufProc(BYTE port);

		void PortBuf2FramBuf(BYTE port);

		//检查端口层发送缓冲区是否空闲
		BOOL isPortTxdBufIdle(BYTE port);

		//CAN硬件数据收发消息响应处理
		void OnCanHardWare(DWORD arg);

		//接收应用报文处理
		void ApiRxdFramProc(BYTE port);

		//CAN链路层管理初始化
		DWORD CanLinkInit();
		
		DWORD LinkRxdFramProc(BYTE port);
	
	public:
		CCanTask(DWORD *pdwApp);
		void Init(void);
};		

/*************************************************************************************
* CAN网链路层任务入口														  
*************************************************************************************/
void CanLinkTask(DWORD *pdwApp);

/*************************************************************************************
* 检查所有CAN端口的端口层发送缓冲区,将数据复制到对应模块层发送缓冲区														  
*************************************************************************************/
void PortBufToModuleBuf(BYTE port);

/*************************************************************************************
* 根据从模块点对点地址和标志,转换为标准发送ID														  
*************************************************************************************/
DWORD ChangeToCanID(BYTE address, BYTE flag);

/*************************************************************************************
* 取模块层发送应用报文数据，给硬件发送中断调用。														  
*************************************************************************************/
//BOOL GetTxdApiData(TCanApiBuf *pApiTxdBuf, BYTE *pData, WORD *pLen);

/*************************************************************************************
* 检查指定CAN网端口是否发送空闲														  
*************************************************************************************/
BOOL isCanPortTxdIdle(BYTE port);

/*************************************************************************************
* CAN网端口管理接口，端口打开														  
*************************************************************************************/
DWORD CanOpen(BYTE bPortSort, DWORD dwFlag);

/*************************************************************************************
* CAN网端口管理接口，从指定CAN网端口的接收端口缓冲区读取一定长度数据														  
*************************************************************************************/
DWORD CanRead(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag);

/*************************************************************************************
* CAN网端口管理接口，向指定CAN网端口的发送端口缓冲区写入一定长度数据														  
*************************************************************************************/
DWORD CanWrite(BYTE bPortSort, BYTE *buf, DWORD dwLen, DWORD dwFlag);

DWORD CanCtrl(BYTE portSort,TCtlComm *comm);

WORD GetCanTxdWritePtr(WORD portSort);

WORD GetCanRxdWritePtr(WORD portSort);

WORD ReadFromCanTxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen);

WORD ReadFromCanRxdBuf(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen);

WORD ReadNewCanTxdData(WORD portSort, BYTE* pBuf, WORD wOffset,WORD wLen);

WORD ReadNewCanRxdData(WORD portSort,BYTE* pBuf, WORD wOffset,WORD wLen);


//#define CAN_MASTER

#define FLEXCAN_PORT	1
#define SJA1000_PORT	2

class CCanTest:public CApp
{
    DECLARE_MESSAGE_MAP()

	BYTE bTxdBuf[CAN_FRAME_BUF_LEN];
	BYTE bRxdBuf[CAN_FRAME_BUF_LEN];
	BYTE bAnswerFram[20];
	BYTE flag;
	DWORD timecounter;
	WORD port;
	DWORD writecnt, readcnt;
	BYTE address;
	
	private:
		//CAN链路定时消息处理														  
		void OnTimeOut(DWORD id);
		void OnReadComm(DWORD dwPort, DWORD dwFlag);
		void OnCommIdle(DWORD dwLen,DWORD PortID);

	public:
		CCanTest(DWORD *pdwApp);
		void Init(void);
};

#endif
