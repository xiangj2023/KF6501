/********************************************************************************************************/
/*																										*/
/*        						天津凯发电气股份有限公司												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Daefunc.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件声明运行环境相关的接口函数																*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* 函数																									*/
/*																										*/
/*	DB_GetRunRecordNum()			读取运行记录缓冲区中未读取数据个数									*/
/*	DB_IsRunRecordEmpty()			运行记录缓冲区是否为空												*/
/*	DB_GetRunRecordHead()			读取运行记录缓冲区头指针											*/
/*	DB_GetRunRecordRead()			读取运行记录缓冲区读指针											*/
/*	DB_SetRunRecordRead()			设置运行记录缓冲区读指针											*/
/*	DB_IncRunRecordRead()			递增运行记录缓冲区读指针											*/
/*	ReportMsg()						发布运行信息，包括调试信息和错误记录								*/
/*	LogError()						登记异常信息，包括异常信息计数,文件名,行号,当前任务,时间,异常信息等	*/
/*	DB_ReadLogInfo()				读取运行记录														*/
/*	DB_Misc_GetNum()				读取操作事件缓冲区中未读取事件数据个数								*/
/*	DB_Misc_IsEmpty()				操作事件缓冲区是否为空												*/
/*	DB_Misc_GetHead()				读取操作事件缓冲区头指针											*/
/*	DB_Misc_GetRead()				读取操作事件缓冲区读指针											*/
/*	DB_Misc_SetRead()				设置操作事件缓冲区读指针											*/
/*	DB_Misc_IncRead()				递增操作事件缓冲区读指针											*/
/*	DB_Misc_Read()					从操作事件缓冲区中读一条操作事件数据								*/
/*	DB_Misc_Write()					向操作事件缓冲区中写入一条操作事件数据								*/
/*	DB_Register_Reset ()			注册一个系统复位事件												*/
/*	DB_Register_FileDown ()			注册一个文件下装事件												*/
/*	DB_Register_Comm ()				注册一个通信事件													*/
/*	DB_Register_BO()				注册一个遥控事件													*/
/*	DB_Register_AO()				注册一个遥调事件													*/
/*	DB_Register_SETTING()			注册一个修改定值事件												*/
/*	DB_Register_SETArea ()			注册一个修改当前定值区号事件										*/
/*	DB_Register_ParaChange ()		注册一个修改参数事件												*/
/*	DB_Register_PowerOnOff ()		注册一个修改参数事件												*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*      Daedef.h                     运行环境相关的数据结构												*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
/********************************************************************************************************/
#ifndef __DAEFUNC_H
#define __DAEFUNC_H

#include "Daedef.h"

/*
void 	DAE_SetRunningFlags(DWORD FlagNo);
void 	DAE_ClearRunningFlags(DWORD FlagNo);
BOOL 	DAE_GetRunningFlags(DWORD FlagNo);
void 	DAE_SetInitFlags(DWORD FlagNo);
void 	DAE_ClearInitFlags(DWORD FlagNo);
BOOL 	DAE_GetInitFlags(DWORD FlagNo);
*/

extern DWORD	g_RunningFlag[];
extern TSysConfigTable	*G_pCfgTable;
	
#define 	DAE_SetRunningFlags(FlagNo)			g_RunningFlag[(FlagNo)/32] |= (0x01<<((FlagNo)%32))
#define 	DAE_ClearRunningFlags(FlagNo)		g_RunningFlag[(FlagNo)/32] &= ~(0x01<<((FlagNo)%32))
#define 	DAE_GetRunningFlags(FlagNo)			( ((g_RunningFlag[(FlagNo)/32]&(0x01<<((FlagNo)%32)))>0)?TRUE:FALSE )
	
#define 	DAE_SetInitFlags(FlagNo)			G_pCfgTable->SystemStatus.InitStatus.Flag[(FlagNo)/32] |= (0x01<<((FlagNo)%32))
#define 	DAE_ClearInitFlags(FlagNo)			G_pCfgTable->SystemStatus.InitStatus.Flag[(FlagNo)/32] &= ~(0x01<<((FlagNo)%32))
#define 	DAE_GetInitFlags(FlagNo)			( ((G_pCfgTable->SystemStatus.InitStatus.Flag[(FlagNo)/32]&(0x01<<((FlagNo)%32)))>0)?TRUE:FALSE )

void 	DAE_InitNvMem( void );
void	*DAE_MemAlloc(DWORD size);

WORD 	DAE_CheckSum(BYTE *pBuf,DWORD nLen);
BOOL 	DAE_IsCheckSumOK(BYTE *pBuf,DWORD nLen);
BOOL 	DAE_MakeCheckSum(BYTE *pBuf,DWORD nLen);
BOOL 	DAE_NvRAMAddrCheck(void *Addr);
STATUS 	DAE_Portid2AppInfo(WORD wPortID,TAppInfo *pDesApp);
HANDLE  DAE_GetTaskID(WORD wPortID);

DWORD	DAE_GetNetADataMask();
DWORD	DAE_GetNetBDataMask();
void	DAE_SetNetADataMask(DWORD dwMask);
void	DAE_SetNetBDataMask(DWORD dwMask);
WORD	DAE_GetCurrentTaskPortId();


WORD 	DB_GetAppNum();
TAppInfo *DB_GetAppInfo();

STATUS  DB_SetEventFlag(TAppInfo *pApp,DWORD dwFlag);
STATUS  DB_ClearEventFlag(TAppInfo	*pApp,DWORD	dwFlag);
BOOL  	DB_GetEventFlag(TAppInfo *pApp,DWORD dwFlag);

STATUS 	DB_GetPortName(WORD wPort,char *strName);

BOOL 	CheckSystemState();

/************************************************************
	功能：读取运行记录缓冲区中未读取数据个数。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordNum(WORD *retval);

/************************************************************
	功能：运行记录缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_IsRunRecordEmpty();

/************************************************************
	功能：从数据库中读取一条运行记录信息
	参数：	rp   	--  读取数据用的指针,当BLANK时用内部读指针
			*event	--	返回的事项指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_ReadRunRecord(TRecordBlock *event, DWORD rp );

/************************************************************
	功能：向运行记录库中写入一条运行记录数据。
	参数：*str,		--  运行记录字符串
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteRunRecord(char *str);

/************************************************************
	功能：读取运行记录缓冲区头指针。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordHead(WORD *retval);

/************************************************************
	功能：读取运行记录缓冲区尾指针。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordTail(WORD *retval);

/************************************************************
	功能：读取运行记录缓冲区读指针。
	参数：*retval	返回数据指针
	返回：未读取数据个数
************************************************************/
STATUS DB_GetRunRecordRead(WORD *retval);

/************************************************************
	功能：设置运行记录缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetRunRecordRead();

/************************************************************
	功能：递增运行记录缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_IncRunRecordRead();

/************************************************************
	功能：发布运行信息，包括调试信息和错误记录
	参数：iof,  --  可变参数
	返回：无
************************************************************/
//void ReportMsg(char *iof,...);

/************************************************************
	功能：登记异常信息，包括异常信息计数,文件名,行号,当前任务,时间,异常信息等
	参数：	func	--当前函数名;
			file	--用FILE_LINE宏当前程序名称和行号;
			iof		--可变参数
	返回：无
************************************************************/
//void LogError(char *func,char *file,char *iof,...);


/************************************************************
	功能：读取操作事件缓冲区中未读取事件数据个数。
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_Misc_GetNum(WORD *retval);

/************************************************************
	功能：操作事件缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_Misc_IsEmpty();

/************************************************************
	功能：读取操作事件缓冲区头指针。
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_Misc_GetHead(WORD *retval);

/************************************************************
	功能：读取操作事件缓冲区尾指针。
	参数：无
	返回：事件缓冲区尾指针。
************************************************************/
STATUS DB_Misc_GetTail(WORD *retval);

/************************************************************
	功能：读取操作事件缓冲区读指针。
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_Misc_GetRead(WORD *retval);


/************************************************************
	功能：设置操作事件缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_SetRead(WORD rp);

/************************************************************
	功能：递增操作事件缓冲区读指针。
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_IncRead();

/************************************************************
	功能：从操作事件缓冲区中读一条操作事件数据。
	参数：rp	    --  序号
		data,		--  操作事件记录
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_Read(DWORD rp, TMiscEvent *data);
		
/************************************************************
	功能：向操作事件缓冲区中写入一条操作事件数据。
	参数：data,		--  操作事件记录
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_Misc_Write(TMiscEvent *data);

/************************************************************
	功能：读取文件长度
	参数：strFileName,		--  文件名
			dwSize			--  文件长度
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DAE_GetFileLength(char *strFileName,DWORD *dwSize);

/************************************************************
	功能：检查系统文件合法性
	参数：pFile,		--  文件内容缓冲区
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DAE_CheckSystemFile(BYTE *pFile,DWORD dwLen);

/************************************************************
	功能：读取文件内容
	参数：strFileName,		--  文件名
			pFile			--  文件缓冲指针
			dwFileLen		--  文件长度
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DAE_ReadFile(char *strFileName,BYTE *pFile,DWORD dwFileLen);

/************************************************************
	功能：读取文件内容
	参数：strFileName,		--  文件名
			ppFile			--  文件缓冲指针，
			dwFileLen		--	要写入文件的长度
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DAE_WriteFile(char *strFileName,void *pFile,DWORD dwFileLen);

STATUS EEPROM_Read(DWORD Addr,DWORD nSize,void *pBuf);
STATUS EEPROM_Write(DWORD Addr,DWORD nSize,void *pBuf);

/************************************************************
	功能：取设备的型号
	参数：无
	返回：装置类型。
************************************************************/
WORD DB_GetDevType();

/************************************************************
	功能：取程序的CRC码
	参数：无
	返回：装置类型。
************************************************************/
WORD DB_GetCodeCrc();

/************************************************************
	功能：取设备的ID
	参数：无
	返回：装置类型。
************************************************************/
DWORD DB_GetDevID();

/************************************************************
	功能：取设备的ID
	参数：无
	返回：装置类型。
************************************************************/
DWORD DB_GetDevAddress();

/************************************************************
	功能：取上位机地址
	参数：无
	返回：上位机地址。
************************************************************/
DWORD DB_GetMainAddress();

/************************************************************
	功能：读取应用程序文件版本信息
	参数：pFileInf			--   文件版本信息数据指针
		  pCrcLen			--   CRC校验长度
	返回：正确或错误
************************************************************/
STATUS	DB_GetCodeFileVerInf(TCodeFileInf *pFileInf, DWORD *pCrcLen);

//程序代码内容校验，16位累加和校验
STATUS DB_CodeFileCheckSum(TCodeFileInf *pFileInf, DWORD dwCheckLen);

//为保证系统可靠性，需要对应用程序的内容进行合法性检查，确保写入的应用程序完整有效
STATUS DB_CodeFileIntegrityCheck(void *pFile, DWORD length);

STATUS DAE_GetFlashFileLength(char *strFileName,DWORD *dwSize);
STATUS DAE_ReadFlashFile(char *strFileName,void *pFile,DWORD dwFileLen);

#define	DAE_VARTYPE_NVRAM			0x00
#define	DAE_VARTYPE_EEPROM			0x01
#define	DAE_VARTYPE_SERTXDBUF		0x02
#define	DAE_VARTYPE_SERRXDBUF		0x03
#define	DAE_VARTYPE_CANTXDBUF		0x04
#define	DAE_VARTYPE_CANRXDBUF		0x05
#define	DAE_VARTYPE_ETHATXDBUF		0x06
#define	DAE_VARTYPE_ETHARXDBUF		0x07
#define	DAE_VARTYPE_ETHBTXDBUF		0x08
#define	DAE_VARTYPE_ETHBRXDBUF		0x09

DWORD DAE_GetMemVarSize(DWORD dwID);

WORD DAE_ReadMemVar(DWORD dwID,DWORD dwOffset,DWORD wLen,BYTE *pBuf);

void DAE_ResetDevice(DWORD type);


#define COMM_STATUS_BREAK	0x55
#define COMM_STATUS_OK		0xAA

/************************************************************
	功能：注册端口通信状态
	参数：wPort			--   端口ID
		  Status		--   通断状态
	返回：正确时返回DB_ERR_OK，错误返回错误码
************************************************************/
STATUS DAE_SetCommStatus(WORD wPort,BYTE Status);

/************************************************************
	功能：获取端口通信状态
	参数：wPort			--   端口ID
		  Status		--   通断状态
	返回：正确时返回DB_ERR_OK，错误返回错误码
************************************************************/
STATUS DAE_GetCommStatus(WORD wPort,BYTE &Status);


STATUS DAE_UpdateBasicConfig();

/************************************************************
	功能：获取镜像位对象通信状态
	参数：pStatus		--   通断状态位串指针
	返回：正确时返回DB_ERR_OK，错误返回错误码
************************************************************/
STATUS DAE_GetMBOCommStatus(BYTE *pStatus);

#define DAE_TRANSPORT_BROADCAST	0xAAAA

/************************************************************
	功能：读取透明数据缓冲区尾指针。
	参数：无
	返回：事件缓冲区尾指针。
************************************************************/
WORD DAE_GetTransDataTail();

/************************************************************
	功能：从数据库中读取一条透明数据
	参数：	rp   	--  读取数据用的指针
			*event	--	返回的事项指针
	返回：有效返回数据长度，无效返回0
************************************************************/
WORD DAE_ReadTransData(BYTE *pBuf,WORD rp);

/************************************************************
	功能：向透明数据库中写入一条透明数据。
	参数：*str,		--  运行记录字符串
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DAE_WriteTransData(WORD wClass,WORD wDesPort,WORD wDataLen,BYTE *pBuf);

/************************************************************
	功能：获取对应以太网口的MAC信息
	参数：devno		--   网口编号
	参数：pMac		--   需要获取的MAC信息
	返回：正确时返回DB_ERR_OK，错误返回错误码
************************************************************/
STATUS DB_GetNetMac(BYTE devno, BYTE *pMac);

#endif
