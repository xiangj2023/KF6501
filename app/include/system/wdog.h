#ifndef _WDOG_H_
#define _WDOG_H_

#include "resm5234.h"
#include "system.h"
#include "msgmap.h"

#define WDOG_TIME			20		//定义看门狗定时器时间间隔
#define DWOG_REFRESH_TIME	50		//定义看门狗刷新周期
#define DWOG_REFRESH_CNT	(DWOG_REFRESH_TIME/WDOG_TIME)	//看门狗刷新计数限值
#define KEY_KEEP_CNT		(40/WDOG_TIME)	//按键保持时间计数限值
#define SECOND_TIME			(1000/WDOG_TIME)
#define TASK_DEAD_TIME		(60000/WDOG_TIME)

#define RUN_LED_TIME		(500/WDOG_TIME)

class CWDog:public CApp
{
    DECLARE_MESSAGE_MAP()

	DWORD dwRefreshCnt;	//看门狗刷新计数
	DWORD dwLedCnt;
	WORD wKeyKeepCnt;	//键盘保持时间计数
	WORD wOldKeyStatus;	//上次键盘状态
	BYTE bCheckLedNo;	//自检灯序号
	BOOL FreshFlag;		//看门狗刷新开关标志
		
	private:
		/*
		int 	m_hSocket;
		DWORD	m_dwError;
		DWORD	m_dwWriteBlockNum;
		*/
		void OnLedCheck(DWORD id);
		void OnTimeOut(DWORD id);
		void OnSysReset(void);
		void TaskWatch(void);
		//void OnRelayRTMB(DWORD num);
	
	public:
		CWDog(DWORD *pdwApp);
		void Init(void);
		/*
		BOOL CreateUDPSocket();
		BOOL CloseUDPSocket();
		BOOL SendUDPData(DWORD ip, BYTE * pBuf, WORD wLen);
		BOOL RecvUDPData(DWORD ip, BYTE * pBuf, WORD &wLen);
		*/
};		

void WDogTask(DWORD *pdwApp);


#endif
