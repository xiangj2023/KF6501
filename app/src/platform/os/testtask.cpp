#ifdef _TEST_TASK

#include "testtask.h"
#include "vx_file.h"
#include "timedef.h"

#include "m5234spi.h"
#include "eeprom.h"

BEGIN_MESSAGE_MAP(CSendTask,CApp)
	ON_SM_TIMEOUT(CSendTask)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CRecvTask,CApp)
	ON_SM_NOTIFY(CRecvTask)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFileTask,CApp)
	ON_SM_TIMEOUT(CFileTask)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CTimeTask,CApp)
	ON_SM_TIMEOUT(CTimeTask)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CEpromTask,CApp)
	ON_SM_TIMEOUT(CEpromTask)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CADCTask,CApp)
	ON_SM_TIMEOUT(CADCTask)
END_MESSAGE_MAP()

extern void TestTask();
extern void InitR8025Sub();

extern void AA1024WriteFlash();
extern void AA1024ReadFlash();
extern void AA1024ReadStatus();
extern void AA1024EnablWrite();

extern void ReadAD7782();

extern DWORD FlashSectErase(WORD sectorno);
extern DWORD FlashWordProg(DWORD dwAddress, WORD data);
extern void FlashTest();

//extern DWORD dwICCounter[];
//extern void ReadEtpuICValue();


//extern void LcdInit(void);
//extern void LcdDraw();


/*定义当前文件名称指针，并获取本文件名称*/
#ifdef __DEBUG__
	const STATIC char *pThisFile = __FILE__;
#endif

DWORD dwSendTaskId, dwRecvTaskId;
DWORD dwTaskFlag, dwFileFlag;
BYTE bRunFlag = 0;


void STask(DWORD *pdwApp)
{
	OS_Sleep(1000);
	CSendTask *pSendTask = new CSendTask(pdwApp);
	pSendTask->Init();
	pSendTask->Run();
}	

CSendTask::CSendTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CSendTask::Init(void)
{
	dwSendCnt = 0;
	dwTaskFlag = 0;
	dwSendTaskId = GetTaskId();
	SetTimer(1, 1000);
}

void CSendTask::OnTimeOut(DWORD id)
{
	dwSendCnt++;
//	if((dwSendCnt%100) == 0)
	{
		ReportMsg("CSendTask timeout cnt=%d...", dwSendCnt);
		
	}
	OS_PostMessage(dwRecvTaskId, SM_NOTIFY, 0, 0, 0, 0);

	/*测试任务挂起、恢复及删除操作，测试定时器删除操作*/
	if(dwTaskFlag == 0x11)	//
	{
		OS_Suspend(0);
		OS_Suspend(dwRecvTaskId);
	}
	else if(dwTaskFlag == 0x22)
	{
		OS_Resume(0);
		OS_Resume(dwRecvTaskId);
	}
	else if(dwTaskFlag == 0x33)
	{
		OS_Kill(0);
	}
	else if(dwTaskFlag == 0x44)
	{
		KillTimer(1);
	}
}


void RTask(DWORD *pdwApp)
{
	CRecvTask *pRecvTask = new CRecvTask(pdwApp);
	pRecvTask->Init();
	pRecvTask->Run();
}

CRecvTask::CRecvTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CRecvTask::Init(void)
{
	dwRecvCnt = 0;

	dwRecvTaskId = GetTaskId();
}

void CRecvTask::OnNotify(DWORD id)
{
	dwRecvCnt++;
//	if( (dwRecvCnt%2000) == 0)
		ReportMsg("message RecvCnt=%d...", dwRecvCnt);
}

void FileTask(DWORD *pdwApp)
{
	CFileTask *pFileTask = new CFileTask(pdwApp);
	pFileTask->Init();
	pFileTask->Run();
}

CFileTask::CFileTask(DWORD *pdwApp):CApp(pdwApp)
{
	
}

void CFileTask::Init(void)
{
	DWORD status, length;
	
	dwRunCnt = 0;
	dwFileFlag = 0x11;
	dwFileOffset = 0;
	
	for(int i=0; i<FILE_BUF_LEN; i++)
	{
		bWriteBuf[i] = i;
		bReadBuf[i] = 0;
	}

	pbFileBuf = (BYTE*)malloc(FILE_LEN);
	
	/*	

	//测试目录创建及删除操作
	if(dwFileFlag == 0x11)
	{
		status = VX_MkDir( "/tffs0/code/");
		if(status == FILE_OK)
			ReportMsg("Create dir /tffs0/code/ OK !");

		status = VX_MkDir("/tffs1/data/");
		if(status == FILE_OK)
			ReportMsg("Create dir /tffs1/data/ OK !");

		status = VX_MkDir("/tffs1/temp/");
		if(status == FILE_OK)
			ReportMsg("Create dir /tffs1/temp/ OK !");
	}

	if(dwFileFlag == 0x22)
	{
		status = VX_DeleteDir("/tffs0/code/");
		if(status == FILE_OK)
			ReportMsg("delete dir /tffs0/code/ OK !");

		status = VX_DeleteDir("/tffs1/data/");
		if(status == FILE_OK)
			ReportMsg("delete dir /tffs1/data/ OK !");
	}

	if(dwFileFlag == 0x33)
	{
		status = VX_MkDir( "/tffs0/code/");
		if(status == FILE_OK)
			ReportMsg("Receate dir /tffs0/code/ OK !");

		status = VX_MkDir("/tffs1/data/");
		if(status == FILE_OK)
			ReportMsg("Recreate dir /tffs1/data/ OK !");
	}

	if(dwFileFlag == 0x44)
	{
		memset(pbFileBuf, 0, FILE_LEN);
		VX_ReadFile("s1.dat", 0, FILE_LEN, pbFileBuf, &length, 0);
		
		memset(pbFileBuf, 0, FILE_LEN);
		VX_ReadFile("s6.dat", 0, FILE_LEN, pbFileBuf, &length, 0);

		memset(pbFileBuf, 0, FILE_LEN);
		VX_ReadFile("test.dat", 0, FILE_LEN, pbFileBuf, &length, 0);
	}
	if(dwFileFlag == 0x55)
	{
		VX_DeleteFile("s1.dat", 0);
		VX_DeleteFile("s6.dat", 0);
	}
	*/	
	
	SetTimer(1, 1000);
}

/* 测试文件读写功能*/
void CFileTask::OnTimeOut(DWORD id)
{
	DWORD status, length;
	WORD type;

	
	if(dwRunCnt<5)
	{
		bWriteBuf[0] = (BYTE)dwRunCnt;
		if(dwRunCnt == 0)
			type = FILE_HEAD;
		else if(dwRunCnt == 4)
			type = FILE_END;
		else 
			type = 0;
		
		status = VX_WriteFile("test.dat", dwFileOffset, FILE_BUF_LEN, bWriteBuf, type, 0);
		if(status != FILE_ERROR)
			ReportMsg("Write file test.dat OK !");

		dwFileOffset += FILE_BUF_LEN;
		dwRunCnt++;
	}
	else if(dwRunCnt==5)
	{
//		if(dwRunCnt == 5)
		memset(bReadBuf, 0, FILE_BUF_LEN);
		status = VX_ReadFile("test.dat", 0, FILE_BUF_LEN, bReadBuf, &length, 0);
		if(status != FILE_ERROR)
			ReportMsg("Read file test.dat OK !");
		dwFileOffset = 0;
		dwRunCnt=0;

		if( VX_DeleteFile("test.dat", 0) == FILE_ERROR)
			ReportMsg("delete file test.dat error !");
		else
			ReportMsg("delete file test.dat OK !");
			
			
	}

	
}

void TimeTask(DWORD *pdwApp)
{
	OS_Sleep(1000);
	CTimeTask *pTimeTask = new CTimeTask(pdwApp);
	pTimeTask->Init();
	pTimeTask->Run();
}	

CTimeTask::CTimeTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CTimeTask::Init(void)
{
	TSysTime SystimTime;
	

	//初始化系统硬件时钟
	ClockDevOpen();
	//设置硬件初始时间信息
	SystimTime.Year = 8;
	SystimTime.Day = 24;
	SystimTime.Month = 11;
	SystimTime.Hour = 9;
	SystimTime.Minute = 9;
	SystimTime.Second = 9;
	WriteHardTime(&SystimTime);
	
	SetTimer(1, 1000);
}

void CTimeTask::OnTimeOut(DWORD id)
{
	TSysTime SystimTime;

	if(ReadHardTime(&SystimTime) == ERR_SUCCESS)
	{
		ReportMsg("Systim hard time is %2d/%2d/%2d %2d:%2d:%2d .", SystimTime.Year, \
			SystimTime.Month, SystimTime.Day, SystimTime.Hour,SystimTime.Minute,SystimTime.Second);
	}
}

void EpromTask(DWORD *pdwApp)
{
	OS_Sleep(1000);
	CEpromTask *pEpromTask = new CEpromTask(pdwApp);
	pEpromTask->Init();
	pEpromTask->Run();
}	

CEpromTask::CEpromTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CEpromTask::Init(void)
{

	dwAddress = 0x10000;
	dwOffset = EPROM_BUF_LEN;
	dwWriteLen = EPROM_BUF_LEN;
	
	//初始化系统硬件时钟
	M5234SpiInit();	

/*	
	if(EEPROMChipErase() == ERR_SUCCESS)
		ReportMsg("EEPROMChipErase OK");
	else
		ReportMsg("EEPROMChipErase ERROR");

	memset(bEpromCheckBuf, 0, dwWriteLen);
	if(EepromRead(bEpromCheckBuf, dwWriteLen, dwAddress) == ERR_SUCCESS)
	{
		ReportMsg("EepromRead OK.\n");
	}
	else
		ReportMsg("EepromRead ERROR.\n");

	MCF_GPIO_PODR_TIMER &= (~0x01);
*/	
	
	DWORD address;
	WORD data = 0xa00;
	for(int i=1; i<64; i++)
	{
		if(FlashSectErase(i) == ERR_SUCCESS)
			ReportMsg("FlashSectErase %d OK.", i);
		else
			ReportMsg("FlashSectErase %d ERROR.", i);

		address = 0x10000000 + 0x10000*i;
/*		
		for(int j=0; j<100;j++)
		{
			if(FlashWordProg(address, data) != ERR_SUCCESS)
			{
				ReportMsg("FlashWordProg address=0x%x, data=0x%x OK.", address, data);
				break;
			}
			address += 2;
			data++;
		}
*/		
	}
/*	
	for(int i=32; i<64; i++)
	{
		if(FlashSectErase(i) == ERR_SUCCESS)
			ReportMsg("FlashSectErase %d OK.", i);
		else
			ReportMsg("FlashSectErase %d ERROR.", i);
				address = 0x10000000 + 0x10000*i;
	}
*/
	SetTimer(1, 2000);
}

void CEpromTask::OnTimeOut(DWORD id)
{
//	FlashTest();
/*	
	ReadAD7782();


//	for(int j=0; j<3; j++)
	{
		for(int i=0; i<EPROM_BUF_LEN; i++)
			bEpromData[i] = (BYTE)(i+dwAddress/256*5+i/256);

		if(dwAddress>EPROM_LEN)
			dwAddress = 0;

		if(EEPROMWrite(bEpromData, dwWriteLen, dwAddress) == ERR_SUCCESS)
			ReportMsg("EEPROMWrite OK.\n");
		else
			ReportMsg("EEPROMWrite ERROR.\n");
		
		memset(bEpromCheckBuf, 0, dwWriteLen);
		if(EepromRead(bEpromCheckBuf, dwWriteLen, dwAddress) == ERR_SUCCESS)
			ReportMsg("EepromRead OK.\n");
		else
			ReportMsg("EepromRead ERROR.\n");
		
		if(DataCmp(bEpromCheckBuf, bEpromData, dwWriteLen) == TRUE)
			ReportMsg("bEpromCheckBuf OK.\n");
		else
			ReportMsg("bEpromCheckBuf ERROR.\n");
			
		dwAddress += dwOffset;	
	}
*/	
}

void ADCTask(DWORD *pdwApp)
{
	OS_Sleep(1000);
	CADCTask *pADCTask = new CADCTask(pdwApp);
	pADCTask->Init();
	pADCTask->Run();
}	

CADCTask::CADCTask(DWORD *pdwApp):CApp(pdwApp)
{
}	

void CADCTask::Init(void)
{
	BYTE status;

//	LcdInit();
	SetTimer(1, 2000);
}

void CADCTask::OnTimeOut(DWORD id)
{
//	ReportMsg("Lcd test .");
//	LcdDraw();
}

void TestTask()
{
	DWORD status;

	while(bRunFlag == 0)
	{
		OS_Sleep(1000);
//		ReadEtpuICValue();
//		ReportMsg("trans=%x, final=%x, last=%x, SCR8=%x, SCR9=%x.",\
//				dwICCounter[0],dwICCounter[1], dwICCounter[2], dwICCounter[3], dwICCounter[4]);
	}
	
	/*测试任务调度机制、消息收发机制、定时器功能*/	
/*	status = OS_CreateTask("send", (TTASK_ENTRY)STask, 100, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create send task error !");
	status = OS_CreateTask("recv", (TTASK_ENTRY)RTask, 110, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create receive task error !");

	status = OS_CreateTask("time", (TTASK_ENTRY)TimeTask, 110, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create time task error !");
*/
/*
	status = OS_CreateTask("time", (TTASK_ENTRY)TimeTask, 110, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create time task error !");
*/
/*
	status = OS_CreateTask("spi", (TTASK_ENTRY)EpromTask, 110, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create spi task error !");
*/
/*
	status = OS_CreateTask("ADC", (TTASK_ENTRY)ADCTask, 110, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create ADC task error !");
*/
	/*测试文件系统*/

	status = OS_CreateTask("file", (TTASK_ENTRY)FileTask, 110, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create file task error !");
	
}

#define _DBUG_S29AL032_ 
#ifdef _DBUG_S29AL032_

#define FLASH_CS_BASE		0X10000000
#define	FLASH_START_ADDR	0X10000000
#define	FLASH_SECTOR_LEN	0X10000

WORD *pFlashStart = (WORD *)FLASH_START_ADDR;

DWORD FlashSectErase(WORD sectorno)
{
	WORD *pFlashPtr, data;
	DWORD dwCounter = 0;
	
	//pFlashPtr = (WORD*)(FLASH_START_ADDR+((sectorno*FLASH_SECTOR_LEN)));
	
	pFlashStart[0x555] = 0xaa;
	pFlashStart[0x2aa] = 0x55;
	pFlashStart[0x555] = 0x80;
	pFlashStart[0x555] = 0xaa;
	pFlashStart[0x2aa] = 0x55;
	//*pFlashPtr = 0x30;
	pFlashStart[(sectorno*FLASH_SECTOR_LEN)/2] = 0x30;

	do
	{
		//data = *pFlashPtr;
		data = pFlashStart[(sectorno*FLASH_SECTOR_LEN)/2];
		dwCounter++;
		if(dwCounter>5000000)
		{
			pFlashStart[0] = 0xf0;	//flash 进入read状态
			ReportMsg("FlashSectErase erase block %d error, and data=%x!.\n", sectorno, data);
			return (ERR_FAILURE);
			
		}
	}while((data&0x80) != 0x80);

	pFlashStart[0] = 0xf0;	//flash 进入read状态
	return ERR_SUCCESS;
}

DWORD FlashWordProg(DWORD dwAddress, WORD data)
{
	WORD *pFlashPtr, status, retry, retry1;
	
	pFlashPtr = (WORD*)(dwAddress);
	pFlashStart[0x555] = 0xaa;
	pFlashStart[0x2aa] = 0x55;
	pFlashStart[0x555] = 0xa0;
	*pFlashPtr = data;

	retry = retry1 = 0;
	
	while (1)
	{
		status = *pFlashPtr;
		if ((status & (0x80)) == (data & (0x80)))
		{
			break;
		}
		if (status & (0x20))
		{
			status = *pFlashPtr;
			if ((status & (0x80)) == (data & (0x80)))
			{
				break;
			}
			if (++retry > 10000)
			{
				return ERR_FAILURE;
			}
		}
		else
		{
			if(++retry1>10000)
				return 	ERR_FAILURE;						
		}
	}
	return ERR_SUCCESS;
}

void FlashTest()
{
	static BYTE SectorNum;
	WORD wTestData[100];
	DWORD dwAddress;
	int i;

	for(i=0; i<100; i++)
		wTestData[i] = 0x100+i+SectorNum;
	
	if(FlashSectErase(SectorNum) == ERR_SUCCESS)
		ReportMsg("FlashSectErase %d OK.", SectorNum);
	else
		ReportMsg("FlashSectErase %d ERROR.", SectorNum);

	dwAddress = FLASH_START_ADDR+SectorNum*FLASH_SECTOR_LEN;
	for(i=0; i<100;i++)
	{
		if(FlashWordProg(dwAddress, wTestData[i]) != ERR_SUCCESS)
//			ReportMsg("FlashWordProg address=0x%x, data=0x%x OK.", dwAddress, wTestData[i]);
//		else
		{
			ReportMsg("FlashWordProg address=0x%x, data=0x%x ERROR.", dwAddress, wTestData[i]);
			break;
		}
		dwAddress += 2;
	}

	SectorNum++;
	SectorNum %= 8;
	
}
#endif

#endif
