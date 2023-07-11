#ifndef __DBASE_H
#define __DBASE_H

#include "appcore.h" 
#include "msgmap.h"

#define	DB_TIMER_MAX		10
#define	DB_TIMER_INIT		0x55AA
#define	DB_TIMER_EMPTY		0x0000

typedef void (*TIMERTASK_ENTRY)(void);

typedef struct{
	WORD 				wTick;
	WORD 				wValue;
	TIMERTASK_ENTRY 	fpEntry;
}TMyTimer;

class CDBaseTask:public CApp
{
	DECLARE_MESSAGE_MAP()

private:
//	TMyTimer    	m_Timer[DB_TIMER_MAX];	//定时器
	DWORD	m_dwTimerCnt;
protected:
public:

    // Constructor.
	CDBaseTask(DWORD *pdwApp):CApp(pdwApp){};
        
    // Initializer.
	BOOL Init();
	void  OnTimeOut(DWORD id);
	void  OnSaveReport(DWORD MsgCode, DWORD wParam);
	void  OnStartDist(DWORD dwStart,DWORD dwPrevSampleNum,DWORD dwTimeLo,WORD wTimeHi);
	void  OnWriteSingnalRW(DWORD dwEEPRomAddr,DWORD val);
	/*	
	void SetMyTimer(BYTE bTimerID,WORD wTime,TIMERTASK_ENTRY fpEntry);
	
	void KillMyTimer(BYTE bTimerID);
	
	void OnMyTimer();
	
	//出现超时无应答的桢
	BOOL MyOnTimer(BYTE bTimerID);
	*/
}; 


class CBackUpTask:public CApp
{
	DECLARE_MESSAGE_MAP()

private:
	DWORD	m_dwTimerCnt;

public:

    // Constructor.
	CBackUpTask(DWORD *pdwApp):CApp(pdwApp){};        
    // Initializer.
	BOOL Init();

	void  OnTimeOut(DWORD id);
}; 


#endif

