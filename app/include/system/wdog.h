#ifndef _WDOG_H_
#define _WDOG_H_

#include "resm5234.h"
#include "system.h"
#include "msgmap.h"

#define WDOG_TIME			20		//���忴�Ź���ʱ��ʱ����
#define DWOG_REFRESH_TIME	50		//���忴�Ź�ˢ������
#define DWOG_REFRESH_CNT	(DWOG_REFRESH_TIME/WDOG_TIME)	//���Ź�ˢ�¼�����ֵ
#define KEY_KEEP_CNT		(40/WDOG_TIME)	//��������ʱ�������ֵ
#define SECOND_TIME			(1000/WDOG_TIME)
#define TASK_DEAD_TIME		(60000/WDOG_TIME)

#define RUN_LED_TIME		(500/WDOG_TIME)

class CWDog:public CApp
{
    DECLARE_MESSAGE_MAP()

	DWORD dwRefreshCnt;	//���Ź�ˢ�¼���
	DWORD dwLedCnt;
	WORD wKeyKeepCnt;	//���̱���ʱ�����
	WORD wOldKeyStatus;	//�ϴμ���״̬
	BYTE bCheckLedNo;	//�Լ�����
	BOOL FreshFlag;		//���Ź�ˢ�¿��ر�־
		
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
