#ifdef _TEST_TASK

#ifndef		_TESTTASK_H_
#define		_TESTTASK_H_

#include "system.h"
#include "msgmap.h"

#define FILE_BUF_LEN	500
#define FILE_LEN		30000

#define EPROM_LEN		100000
#define EPROM_BUF_LEN	50

class CSendTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
	DWORD dwSendCnt;
	
	private:
		void OnTimeOut(DWORD id);
	
	public:
		CSendTask(DWORD *pdwApp);
		void Init(void);
};		

class CRecvTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
	DWORD dwRecvCnt;
	
	private:
		void OnNotify(DWORD id);
	
	public:
		CRecvTask(DWORD *pdwApp);
		void Init(void);
};		

class CFileTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
	DWORD dwRunCnt, dwFileCnt;
	DWORD dwFileOffset;
	BYTE bWriteBuf[FILE_BUF_LEN];
	BYTE bReadBuf[FILE_BUF_LEN];
	BYTE *pbFileBuf;
	
	
	private:
		void OnTimeOut(DWORD id);
	
	public:
		CFileTask(DWORD *pdwApp);
		void Init(void);
};		

class CTimeTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
//	DWORD dwSendCnt;
	
	private:
		void OnTimeOut(DWORD id);
	
	public:
		CTimeTask(DWORD *pdwApp);
		void Init(void);
};		

class CEpromTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
	BYTE bEpromData[EPROM_BUF_LEN];
	BYTE bEpromCheckBuf[EPROM_BUF_LEN];
	DWORD dwAddress, dwOffset, dwWriteLen;
	
	private:
		void OnTimeOut(DWORD id);
	
	public:
		CEpromTask(DWORD *pdwApp);
		void Init(void);
};		

class CADCTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
	
	private:
		void OnTimeOut(DWORD id);
	
	public:
		CADCTask(DWORD *pdwApp);
		void Init(void);
};		

extern void STask(DWORD *pdwApp);

extern void RTask(DWORD *pdwApp);

extern void FileTask(DWORD *pdwApp);

extern void TimeTask(DWORD *pdwApp);

extern void EpromTask(DWORD *pdwApp);

extern void ADCTask(DWORD *pdwApp);

#endif

#endif
