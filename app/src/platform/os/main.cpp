
#include "system.h"
#include "daedef.h"
#include "led.h"
#include "daefunc.h"

extern void VXMain(void);
extern void CreateDAE();


//extern void TestCanHard(DWORD *pdwApp);
//extern void TestSja1000Hard(DWORD *pdwApp);


extern int EtpuInit();

#ifdef __cplusplus
extern "C" {
#endif

void SysMain();

#ifdef __cplusplus
}
#endif

TCodeFileInf FileInf;


void SysMain()
{
	BYTE flag =1;
	WORD counter = 0;


	while( flag == 0)
	{
		OS_Sleep(20);
	}		

	DWORD dwCheckLen;
	if(DB_GetCodeFileVerInf(&FileInf, &dwCheckLen) == ERR_SUCCESS)
	{
		printf("\n***********************代码文件信息***********************************\n");
		printf("*	校验码:     %4x\n", FileInf.wCheckSum);
		printf("*	编译日期:   %d/%d/%d/ %d:%d:%d\n", FileInf.CompileTime.Year, FileInf.CompileTime.Month, FileInf.CompileTime.Day, \
					FileInf.CompileTime.Hour, FileInf.CompileTime.Minute, FileInf.CompileTime.Second);
		printf("*	编辑日期:   %d/%d/%d/ %d:%d:%d	\n", FileInf.EditTime.Year, FileInf.EditTime.Month, FileInf.EditTime.Day, \
					FileInf.EditTime.Hour, FileInf.EditTime.Minute, FileInf.EditTime.Second);
		printf("*	软件版本:   %s\n", FileInf.SoftWareVer);
		printf("*	硬件版本:   %s\n", FileInf.HardWareVer);
		printf("*	用户信息: %s\n", FileInf.UserInf);
		printf("************************************************************************\n");

		//dwCheckLen -= 16;	//文件开始的16个字节不参与校验 	
		DB_CodeFileCheckSum(&FileInf, dwCheckLen);
		DAE_ClearRunningFlags(SS_PROGRAM_ROM_EERR);
	}
	else
		DAE_SetRunningFlags(SS_PROGRAM_ROM_EERR);

	
	VXMain();
	SysTimeInit();
	CreateDAE();
	EtpuInit();

//	OS_CreateTask("flex", (TTASK_ENTRY)TestCanHard, 100, 10000, 0, 0, 0);
//	OS_CreateTask("sja1000", (TTASK_ENTRY)TestSja1000Hard, 100, 10000, 0, 0, 0);
}

/*
void TaskCreat()
{
	DWORD status;
	status = OS_CreateTask("wdog", (TTASK_ENTRY)WDogTask, WDOG_PRIORITY, 10000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create wdog task error !");

	status = OS_CreateTask("lcd", (TTASK_ENTRY)LcdTask, LCD_PRIORITY, 20000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create wdog task error !");

	status = OS_CreateTask("relay", (TTASK_ENTRY)RelayTask, RELAY_PRIORITY, 20000, 0, 0, 0);
	if(status == ERR_FAILURE)
		ReportMsg("Create relay task error !");
		
}


void TestNVRam()
{
	WORD *pRamBase = (WORD *)0x20000000;
	DWORD dwLen;
	int i;
	TSysConfigTable *pMyCfgTable = (TSysConfigTable *)DBASE_NVRAM_BASE;

	dwLen = 0x80000/sizeof(WORD);

	for(i=0; i<dwLen; i++, pRamBase++)
	{
		*pRamBase = 0xaaaa;
		if(*pRamBase != 0xaaaa)
			break;

		*pRamBase = 0x5555;
		if(*pRamBase != 0x5555)
			break;
		
		*pRamBase = 0xffff;
		if(*pRamBase != 0xffff)
			break;
		
		*pRamBase = 0x0000;
		if(*pRamBase != 0x0000)
			break;
	}
	if(i==dwLen)
		ReportMsg("NVRAM test OK");
	else 
		ReportMsg("NVRAM test error at address %x", i);

	memset(&pMyCfgTable->DBaseConfig, 0, sizeof(TDBaseConfigTable));
	memset(&pMyCfgTable->SystemStatus, 0, sizeof(TStatus));
	memset(&pMyCfgTable->NvRamLink, 0, sizeof(TMemLink));
	memset(&pMyCfgTable->RecordWrap, 0, sizeof(TRecordWrap));
	memset(&pMyCfgTable->MiscWrap, 0, sizeof(TMiscWrap));
	
}
*/

/*
void IdleTask(DWORD *pdwApp)
{
	DWORD dwCounter;	
	while(1)
	{
		dwCounter++;
		if(dwCounter&0x01)
			MCF_GPIO_PODR_TIMER |= 0x01;		//YK_EN2=2
		else
			MCF_GPIO_PODR_TIMER &= ~0x01;		//YK_EN2=2
			
	}
}
*/

