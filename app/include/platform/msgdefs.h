#ifndef _MSGDEFS_H
#define _MSGDEFS_H

#include "sysdefs.h"
/*消息结构定义，消息块由四个长字组成，其中LOWORD(lParam)表示消息类型，其它数据区根据具体的
消息类型，确定具体的数据定义
*/

typedef struct message{
  DWORD		lParam;    
  DWORD  	hParam;     
  DWORD  	wParam;
  DWORD 	pParam;     
}TMessage;

#define SM_EVENTMSG			  0xfff0					//事项对应的消息基数  
#define SM_EVENTD0			  (	SM_EVENTMSG+(1<<0))		//D0事项对应的消息
#define SM_EVENTD1			  (	SM_EVENTMSG+(1<<1))		//D1事项对应的消息
#define SM_EVENTD2			  (	SM_EVENTMSG+(1<<2))		//D2事项对应的消息
#define SM_EVENTD3			  (	SM_EVENTMSG+(1<<3))		//D3事项对应的消息

#define SM_QUIT               255   //退出消息

enum EMessage{ 
	SM_TIMEOUT=100,   	//定时到消息
	SM_BIEVENT, 		//读SOE
	SM_BIBURST, 		//读变位遥信
	SM_DOE, 			//读DOE消息
	SM_DPIBURST,		//读变位双遥信
	SM_RELAYEVENT,		//产生保护事项
	SM_RELAYDATA,		//保护数据改变
//	SM_RELAYALARM,		//产生保护告警报告消息
//	SM_RELAYSTART,		//产生保护启动报告消息
//	SM_RELAYACTION, 	//产生保护动作报告消息
		
	// 控制消息
	SM_BOSELECT,		 //遥控预置
	SM_BOSELECTECHO,	 //遥控预置响应
	SM_BOEXEC,			 //遥控执行
	SM_BOEXECECHO,		 //遥控执行响应
	SM_BODIRECT,		 //遥控直接操作
	SM_BODIRECTECHO,	 //遥控直接操作响应
	SM_BOTEST,			 //保护传动
	
	SM_AOSELECT,		 //遥调预置
	SM_AOSELECTECHO,	 //遥调预置响应
	SM_AOEXEC,			 //遥调执行
	SM_AOEXECECHO,		 //遥调执行响应
	SM_AODIRECT,		 //遥调直接操作
	SM_AODIRECTECHO,	 //遥调直接操作响应
			
	SM_SYNCSELECT,		 //同期预置
	SM_SYNCSELECTECHO,	 //同期预置响应
	SM_SYNCEXEC,		 //同期执行
	SM_SYNCEXECECHO,	 //同期执行响应
	SM_SYNCDIRECT,		 //同期直接操作 
	SM_SYNCDIRECTECHO,	 //同期直接操作响应
	
	SM_TQSELECT,		 //同期预置
	SM_TQSELECTECHO,	 //同期预置响应
	SM_TQEXEC,			 //同期执行
	SM_TQEXECECHO,		 //同期执行响应
	
	//读串口消息
	SM_READCOMM,		 //读串口消息
	SM_WRITECOMM,		 //写串口消息
	SM_COMMIDLE,		 //端口通道空闲，即在一定时间内没有接收也没有发送
	SM_COMMTXDIDLE, 	 //发送通道空闲
	SM_COMMRXDIDLE, 
	SM_COMMEXCEPT,		 //通道异常
	SM_WRITECONT,		 //继续发送消息
	
	SM_KEYDOWN, 		 //键盘按下
	SM_LCD, 			 //LCD通知键盘任务暂停发送消息
	SM_TRANSOIE,		 //透明数据传输消息
	SM_TRANSBHDZ,		 //传送保护定值
	SM_NOTIFY,

	SM_DICHANGE,
	SM_CANTXDRXDOK,
	SM_PARACHANGE,
	SM_TASKREGISTER,
	SM_COEMAKE,

	SM_SAMPLEEND,		//AD采样结束
	SM_POWERDOWN,		//系统电源电压偏低
//	SM_AUTOADJUSTSTEP1,		//系数自动校正,死区校正
	SM_AUTOADJUST,		//系数自动校正，系数校正
	SM_AUTOADJUSTECHO,	//自动校准结果
//	SM_AUTOADJUSTECHO2,	//自动校准结果
	SM_STARTDIST,		//启动录波
//	SM_THERMALOVERLOAD,   //定时调用热过负荷累加程序
	SM_RELAY_INT,		//保护采样中断
	SM_RELAYTIMER,		//保护用定时
	SM_LEDCHECK,		//LED自检
	SM_SAVEREPORT,		//存保护报告
	SM_RELAYRTMB,		//双边联跳镜像位收发
	SM_BIPARACHANGE,	
	SM_WRITESINGNALRW,	//修改信号继电器字	
	SM_SYS_RESET,
	SM_CALLSUBDATA,     //故障测距主所装置召取子所数据
	SM_GETFLDATA,		//故障测距子所装置通讯任务下发给测距任务的取数消息
	SM_DATAISOK,			//子所数据计算完成后发送给通讯任务的消息
	SM_STOPCALLDATA,		//测距超时，停止召唤数据
	SM_STEPOVERCMD, 	//跨区命令
	SM_STEPOVERECHO,		//跨区命令返校	
	SM_LOGICCHANGED,			//逻辑文件修改，需要重新初始化逻辑
	SM_NETCARDPARACHG,	//网卡参数改变
	SM_NOTIFYETHLINKED, 	//以太网连接
	SM_NOTIFYETHCLOSED,		//以太网连接断开
	SM_AIEVENT, 			//AIE事项通知
	SM_JDXXPICK,				//接地选线启动消息
	SM_JDXXRESULT,				//接地选线结果
	SM_DISTEVENT			//录波事项通知
};


#endif
