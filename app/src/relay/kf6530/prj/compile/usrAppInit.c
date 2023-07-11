/* usrAppInit.c - stub application initialization routine */

/* Copyright 1984-1998 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,02jun98,ms   written
*/

/*
DESCRIPTION
Initialize user application code.
*/ 

#include "vxWorks.h"

#include "tffs/tffsDrv.h"

//#include "ipProto.h"

#include "time.h"

#define MAX_TFFS_DEV_NUM	2

/******************************************************************************
*
* usrAppInit - initialize the users application
*/ 

int TffsFlag = 0x0;
void TffsInit(void);
void NetInit();

extern void SysMain();


void usrAppInit (void)
    {
#ifdef	USER_APPL_INIT
	USER_APPL_INIT;		/* for backwards compatibility */
#endif
	//TFFS initialize
	TffsInit();
	
	NetInit();
    /* add application specific code here */
	SysMain();
    }


/*initialize two TrueFFS*/
void TffsInit(void)
{
	STATUS status;
	int i;
	char *DevNum = "/tffs0";
	
	tffsDevFormatParams FormatPar[]=
	{
		{{0x20000, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL,2, 0, NULL}, FTL_FORMAT_IF_NEEDED},
		{{0x100000, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL,2, 0, NULL}, FTL_FORMAT_IF_NEEDED},
	};

	
//	while(TffsFlag == 0)
//		taskDelay(1);	
	
	for(i=0; i<MAX_TFFS_DEV_NUM; i++)
	{
		DevNum[5] += i;	
			
		printf("\n");
		status = usrTffsConfig(i, FALSE, DevNum);
		if(status == OK)
		{
			printf("usrTffsConfig  TFFS%d OK! \n", i);
		}
		/*
		else
		{
			printf("usrTffsConfig TFFS%d error for 0x%x .\n", i,status);
			status = tffsDevFormat (i, (int)&FormatPar[i]);
			if(status != OK)
			{
					printf("tffsDevFormat TFFS%d error for 0x%x .\n", i,status);
					continue;
			}
			else					
			{
				status = usrTffsConfig(i, FALSE, DevNum);
				if(status == OK)
					printf("usrTffsConfig  TFFS%d OK! \n", i);
				else
				{
					printf("usrTffsConfig TFFS%d error for 0x%x .\n", i,status);
					continue;
				}
			}
		}
		*/
	}
}

void NetInit()
{
	char initflag = 0;

//	while(initflag == 0)
		taskDelay(1);	

	if(ipAttach(1,"ene") == OK)
		printf("ipAttach OK.\n");
	else
		printf("ipAttach ERROR.\n");

	if(ifMaskSet("ene1", 0xffffff00)==OK)
		printf("ifMaskSet OK.\n");
	else
		printf("ifMaskSet ERROR");
	
	if(ifAddrSet("ene1", "172.20.10.10")==OK)
		printf("ifAddrSet OK.\n");
	else
		printf("ifAddrSet ERROR");

/*
	if(usrNetIfConfig("ene", 2, "172.20.10.1", "localhost", 0xffffff00) == OK)
		printf("usrNetIfConfig OK.\n");
	else
		printf("usrNetIfConfig ERROR.\n");
*/

}