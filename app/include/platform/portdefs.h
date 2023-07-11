/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司							                                    */
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      portdefs.h                                       1.0                         					*/
/*																										*/
/* 描述																									*/
/*																										*/
/*      该文件定义端口管理的相关数据结构并声明端口管理的系列函数                                        */
/*																										*/
/* 作者												                                   					*/
/*																										*/
/*      l.z											                                   					*/
/*																										*/
/* 定义的数据结构                                                                    					*/
/*                                      端口控制结构                                					*/
/*      TCtlComm                                                                    					*/
/*                                                                                  					*/
/* 函数                                                                             					*/
/*                                                                                  					*/
/*      OpenComm                        打开端口                                    					*/
/*      ReadComm                        读端口                                      					*/
/*      WriteComm                       写端口                                      					*/
/*      CloseComm                       控制端口                                    					*/
/*      CtlComm                         端口控制信息                                					*/
/*                                                                                  					*/
/*												                                   						*/
/* 头文件说明									                                   						*/
/*												                                   						*/
/*      __def.h                          基本数据类型定义                                   			*/
/*											                                   							*/
/* 修改历史										                                   						*/
/*												                                   						*/
/*      修改人           修改日期       修改内容	                                   					*/
/*												                                   						*/
/*      l.z             2008-07-15      初始创建,版本号1.0	                                   			*/
/********************************************************************************************************/

#ifndef __PORTDEFS_H
#define __PORTDEFS_H

#include "system.h"

#define	CM_ALL				((DWORD)0xFFFFFFFF);	/*所有设置有效*/
/*参数设置屏蔽字定义*/
#define	CM_NOTIFYWRITE		((DWORD)1<<0)			/*设置通知发送*/
#define	CM_NOTIFYREAD		((DWORD)1<<1)			/*设置通知接收*/
#define	CM_NOTIFYRXDBREAK	((DWORD)1<<2)			/*设置通知接收中断*/
#define	CM_NOTIFYTXDBREAK	((DWORD)1<<3)			/*设置通知发送中断*/
#define	CM_NOTIFYCOMMBREAK	((DWORD)1<<4)			/*设置通知通道中断*/
#define	CM_CLOSELINK		((DWORD)1<<5) 			/*设置通知通道中断*/

/*控制字屏蔽字定义*/
#define	CM_INITCOMM			((DWORD)1<<16)       	/*重新复位端口*/
#define	CM_HUNTSYNC			((DWORD)1<<17)       	/*搜索同步字*/
#define	CM_CONNECT			((DWORD)1<<18)       	/*当端口接拨号MODEM时，用于指示端口进行拨号操作*/
#define	CM_HUNGUP			((DWORD)1<<19)       	/*当端口接拨号MODEM时，用于指示端口进行挂断操作*/

/*定义SM_READCOMM消息传送的标志字节*/
#define	RCF_FRAME_TAIL		((DWORD)1<<31)
#define	RCF_TXDISABLE		((DWORD)1<<30)			//发送禁止

#define	RCF_RXDTIMEOUT		((DWORD)1<<29)			//接收超时（在规定时间内，未收到对方的应答）
#define	RCF_RESETLINK		((DWORD)1<<28) 			//复位链路
#define	RCF_TXDTIMEOUT		((DWORD)1<<27)			//发送超时（在规定时间内，未能发送该报文）
#define	RCF_RECEIVEREADY	((DWORD)1<<26)			//主要用于HDLC链路控制的RR帧

//WriteComm flag 定义
#define	WCF_URGENT			((DWORD)1<<31)      	//紧急报文标志                                                                  
#define	WCF_BROADCAST		((DWORD)1<<30)       	//广播报文标志                                                              
#define	WCF_FRAMETAIL		((DWORD)1<<29)       	//报文结束标志                                                              
#define	WCF_APPIDLE			((DWORD)1<<28)       	//应用层没有现在要发送的报文                                                    
#define	WCF_SST				((DWORD)1<<27)       	//对钟报文                                                                      

/*串口通信介质定义*/
#define TRANSPARENT 		0x00	/* 透明介质				*/
#define RADIO       		0x01	/* 无线电台				*/
#define SPTELEPHONE 		0x02	/* 专线电话				*/
#define PBTELEPHONE 		0x03	/* 公用电话				*/
#define PPFIBER     		0x04	/* 点点光纤				*/
#define NOSCARFIBER 		0x05	/* 无自愈功能的环网光纤	*/
#define SCARFIBER   		0x06	/* 有自愈功能的环网光纤	*/
#define SPMODEM     		0x07	/* 专线MODEM			*/
#define FIBERMODEM  		0x08	/* 光纤MODEM			*/
#define INFRARED			0x09	/* 红外端口				*/
#define AUTO_485			0x0a	/* 485方式RTS低有效 	*/
#define GPRS				0x0b	//gprs

//定义串口数据位，对应TSerialP的wFlag的D0~D1
#define DATA_BIT_MASK		(0x03<<0)
#define DATA_BIT_8			(0x00<<0)
#define DATA_BIT_7			(0x01<<0)
#define DATA_BIT_6			(0x02<<0)
#define DATA_BIT_5			(0x03<<0)

//校验位标志定义，对应D2~D3
#define PARITY_BIT_MASK		(0x03<<2)
#define NO_PARITY			(0x00<<2)
#define EVEN_PARITY			(0x01<<2)
#define ODD_PARITY			(0x02<<2)

//停止位标志定义，对应D4
#define STOP_BIT_MASK		(0x01<<4)
#define STOP_BIT_1			(0x00<<4)
#define STOP_BIT_2			(0x01<<4)

/*----------------------------------------------------------------------------------------*/
/* 定义端口控制功能码                                                                     */
/*----------------------------------------------------------------------------------------*/
enum TCtlCommFuncCode{
    FC_GET=1,                           /*取端口相应参数*/
    FC_SET=2,                           /*写端口参数*/
    FC_OPERATOR=3,                      /*进行控制操作*/
    FC_GETSTATUS=4                      /*取端口实时工作状态*/
};
/*----------------------------------------------------------------------------------------*/
/* 端口控制结构                                                                           */
/*----------------------------------------------------------------------------------------*/
typedef struct{
    DWORD       dwCtlMask;              /*控制端口屏蔽字*/

    TCtlCommFuncCode dwFuncCode;        /*控制功能码*/

    WORD        dwNotifyWrite;          /*当发送缓冲区长度小于dwNotifyWrite时向上层规约*/
                                        /*任务发送SM_WRITECOMM消息,指示规约任务可以继续发*/
                                        /*送；dwNotifyWrite=0时禁止SM_WRITECOMM*/
    WORD        dwNotifyRead;           /*当接收缓冲区长度大于dwNotifyRead时向上层规约*/
                                        /*任务发送SM_READCOMM消息,指示规约任务读端口；*/
                                        /*dwNotifyRead=0时禁止SM_READCOMM*/
    WORD        dwNotifyRxdBreak;       /*在接收数据时,如果从上次接收到字符的时刻,在dwNotifyRxdBreak*/
                                        /*未接收新的字符,则向规约任务发送SM_COMMRXDIDLE消息*/   
                                        /*dwNotifyRxdBreak=0时,禁止发送SM_COMMRXDIDLE消息*/
    WORD        dwNotifyTxdBreak;       /*在发送数据时,如果从上次发送字符的时刻,在dwNotifyTxdBreak*/
                                        /*未发送新的字符,则向规约任务发送SM_COMMTXDIDLE消息*/   
                                        /*dwNotifyTxdBreak=0时,禁止发送SM_COMMTXDIDLE消息*/
    WORD        dwNotifyCommBreak;      /*端口在dwNotifyCommBreak时间内,没有发送,也没有接收操作*/
                                        /*时,向上层任务发送SM_COMMIDLE消息;dwNotifyCommBreak=0*/
                                        /*则禁止该消息的发送*/
    WORD        dwBaud;                 /*端口波特率*/

    WORD        wSync;                  /*同步方式时的同步通信字，=0xFFFF该端口为异步工作模式*/

    WORD        wFrameDelay;            /*发送的帧间隔*/

    void*       pPara;
}TCtlComm;
/*----------------------------------------------------------------------------------------*/
/* 当前端口工作状态                                                                       */
/*----------------------------------------------------------------------------------------*/
typedef struct{
    WORD        nTxdNum;                /*发送缓冲区未发送字符的个数*/
    WORD        nTxdEmpty;              /*发送缓冲区的最大可发送字符个数*/
    WORD        nRxdNum;                /*接收缓冲区接收字符的个数*/
    WORD        nRxdEmpty;              /*接收缓冲区空闲字节个数*/
}TCommStatus;
/*------------------------------------------------------------------------------------------*/
/* 功能:打开端口,提供给规约任务程序的接口                                                   */
/* 输入:port 端口号；txd 发送缓冲区长度；rxd 接收缓冲区长度；mode 打开模式                  */
/* 输出:无                                                                                  */
/* 返回:ERR_SUCCESS 打开成功,ERR_SERIAL_CHANNEL 打开端口错误                                */
/*------------------------------------------------------------------------------------------*/
DWORD OpenComm(WORD port,WORD txd,WORD rxd,DWORD mode=0);
/*------------------------------------------------------------------------------------------*/
/* 功能:读端口接收缓冲区内容,提供给规约任务程序的接口                                       */
/* 输入:port 端口号；buf 存放接收数据的指针；len 要求读的长度；读模式                       */
/* 输出:buf 读到的缓冲区数据                                                                */
/* 返回:实际读到的长度                                                                      */
/*------------------------------------------------------------------------------------------*/
DWORD ReadComm(WORD port,PBYTE buf,WORD len,DWORD flag=0);

/*------------------------------------------------------------------------------------------*/
/* 功能:向端口发送缓冲区写数据,提供给规约任务程序的接口                                     */
/* 输入:port 端口号；buf 写数据的首指针；len 请求写的长度；写模式                           */
/*  写标志定义                                                                              */
/*  WCF_URGENT 紧急报文标志                                                                 */
/*  WCF_BROADCAST 广播报文标志                                                              */
/*  WCF_FRAMETAIL报文结束标志                                                               */
/*  WCF_APPIDLE应用层没有现在要发送的报文                                                   */
/*  WCF_SST对钟报文                                                                         */
/* 输出:无                                                                                  */
/* 返回:实际写端口的长度                                                                    */
/*------------------------------------------------------------------------------------------*/
DWORD WriteComm(WORD port,PBYTE buf,WORD len,DWORD flag=WCF_FRAMETAIL);

/*------------------------------------------------------------------------------------------*/
/* 功能:控制端口,提供给规约任务程序的接口                                                   */
/* 输入:port 端口号;TCommCtl 控制结构；flag :WCF_URGENT 紧急报文标志                        */
/*                                           WCF_BROADCAST 广播报文标志                     */
/*                                           WCF_FRAMETAIL报文结束标志                      */
/*                                           WCF_APPIDLE应用层没有现在要发送的报文          */
/*                                           WCF_SST对钟报文                                */                      
/* 输出:无                                                                                  */
/* 返回:ERR_SUCCESS 关闭成功,ERR_SERIAL_CHANNEL 关闭端口失败,ERR_SUPPORT端口不支持该功能    */
/*------------------------------------------------------------------------------------------*/
DWORD CtrlComm(WORD port,TCtlComm *commCtl);

/*------------------------------------------------------------------------------------------*/
/* 功能:关闭端口,提供给规约任务程序的接口                                                   */
/* 输入:port 端口号                                                                         */
/* 输出:无                                                                                  */
/* 返回:ERR_SUCCESS 关闭成功,ERR_SERIAL_CHANNEL 关闭端口失败                                */
/*------------------------------------------------------------------------------------------*/
DWORD CloseComm(WORD port);

/*------------------------------------------------------------------------------------------*/
/* 功能: 取端口信息                                                                         */
/* 输入: port 端口ID号                                                                      */
/* 输出: 端口参数                                                                                 */
/* 返回: 正确返回DB_ERR_OK，否则返回错误码                                                                                 */
/*------------------------------------------------------------------------------------------*/
STATUS GetPortPara(WORD port,void *pPortPara);

/*------------------------------------------------------------------------------------------*/
/* 功能: 取端口数据掩码                                                                         */
/* 输入: port 端口ID号                                                                      */
/* 输出: 数据掩码                                                                                 */
/* 返回: ERR_SUCCESS 成功,失败返回错误码                                */
/*------------------------------------------------------------------------------------------*/
STATUS GetPortDataMask(WORD port,DWORD &dwMask);

 
#endif 
