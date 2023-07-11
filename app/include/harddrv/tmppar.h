#ifndef _TMPPAR_H_
#define _TMPPAR_H_

#include "system.h"

//本文件为串口临时定义结构

//临时定义
/*
#define SM_WRITECONT	10
#define SM_READCOMM		11
#define SM_COMMIDLE		12
#define SM_COMMTXDIDLE	13
#define SM_WRITECOMM	14
*/
#define RCF_FRAME_TAIL		(DWORD)(1<<31)

/*串口通信介质定义*/
#define TRANSPARENT 0x00	/* 透明介质				*/
#define RADIO       0x01	/* 无线电台				*/
#define SPTELEPHONE 0x02	/* 专线电话				*/
#define PBTELEPHONE 0x03	/* 公用电话				*/
#define PPFIBER     0x04	/* 点点光纤				*/
#define NOSCARFIBER 0x05	/* 无自愈功能的环网光纤	*/
#define SCARFIBER   0x06	/* 有自愈功能的环网光纤	*/
#define SPMODEM     0x07	/* 专线MODEM			*/
#define FIBERMODEM  0x08	/* 光纤MODEM			*/
#define INFRARED	0x09	/* 红外端口				*/
#define AUTO_485	0x0a	/* 485方式RTS低有效 	*/
#define GPRS		0x0b	//gprs

//临时定义串口有关结构
#define MAJOR_PORT(port) (HIWORD(port))
#define MINOR_PORT(port) (LOWORD(port))

/*======================================================================
* 定义端口控制功能码                                                                     
*======================================================================
*/
typedef enum{
    FC_GET=1,                           /*取端口相应参数*/
    FC_SET=2,                           /*写端口参数*/
    FC_OPERATOR=3,                      /*进行控制操作*/
    FC_GETSTATUS=4                      /*取端口实时工作状态*/
}TCtlCommFuncCode;

/*======================================================================
* 端口控制结构                                                                          
*======================================================================
*/

typedef struct  {
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

//串口口参数
typedef struct {
	WORD		wPortID;	/*对应的端口ID号,类型+编号*/
	WORD		byFlag;	    /*考虑到具体应用，暂时不考虑同步方式*/
							/*D0~D1 数据位，=0 8位数据位，=1 7位数据，=2 6位数据，=3 5位数据*/
	                        /*D2~D3校验位，=0无校验，=1 偶校验，=2 奇校验*/
	                        /*D4 停止位，=0 1位停止位，=1 2位停止位*/
	BYTE		byBaud;		/*波特率,0--No Use,1--150,2--300,3--600,4--1200,5--2400*/
	                        /*6--4800,7--9600,8--19200,9--38400,10--56000,11--64000*/
							
	BYTE		byDriver;	/*该端口对应的规约号*/
	
	//以下内容为2004规约终端通信参数
	BYTE 		byRTSDelay;			//rts延时时间，电台时使用
	BYTE		nRetry;				//发送重试次数
	BYTE    	byValidInterval;	//报文时效间隔，单位为分钟
	BYTE    	bySafetyInterval;	//平安报文间隔，单位为分钟
	BYTE		byConfirm;  		//需要主站确认的标志，D0=1 重要事件需要确认，D1=1一般事件需要确认
	//BYTE		byAttr;				//D0:1 需要加密 0 无需加密
	WORD    	wTimeOut;			//最大超时时间，超过该时间，启动站就作为超时处理
	BYTE 		bMedium;			//是否Modem
	BYTE		rsv[3];				//备用字节
	DWORD		dwRsv[5];			//备用长字		
}TSerialP;

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

//临时定义结束

#endif
