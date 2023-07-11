/********************************************************************************************************/
/*																										*/
/*        						��򿭷������ɷ����޹�˾												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      Daefunc.h                                     1.0												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ��������л�����صĽӿں���																*/
/*																										*/
/* ����																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* ��������ݽṹ																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* ����																									*/
/*																										*/
/*	DB_GetRunRecordNum()			��ȡ���м�¼��������δ��ȡ���ݸ���									*/
/*	DB_IsRunRecordEmpty()			���м�¼�������Ƿ�Ϊ��												*/
/*	DB_GetRunRecordHead()			��ȡ���м�¼������ͷָ��											*/
/*	DB_GetRunRecordRead()			��ȡ���м�¼��������ָ��											*/
/*	DB_SetRunRecordRead()			�������м�¼��������ָ��											*/
/*	DB_IncRunRecordRead()			�������м�¼��������ָ��											*/
/*	ReportMsg()						����������Ϣ������������Ϣ�ʹ����¼								*/
/*	LogError()						�Ǽ��쳣��Ϣ�������쳣��Ϣ����,�ļ���,�к�,��ǰ����,ʱ��,�쳣��Ϣ��	*/
/*	DB_ReadLogInfo()				��ȡ���м�¼														*/
/*	DB_Misc_GetNum()				��ȡ�����¼���������δ��ȡ�¼����ݸ���								*/
/*	DB_Misc_IsEmpty()				�����¼��������Ƿ�Ϊ��												*/
/*	DB_Misc_GetHead()				��ȡ�����¼�������ͷָ��											*/
/*	DB_Misc_GetRead()				��ȡ�����¼���������ָ��											*/
/*	DB_Misc_SetRead()				���ò����¼���������ָ��											*/
/*	DB_Misc_IncRead()				���������¼���������ָ��											*/
/*	DB_Misc_Read()					�Ӳ����¼��������ж�һ�������¼�����								*/
/*	DB_Misc_Write()					������¼���������д��һ�������¼�����								*/
/*	DB_Register_Reset ()			ע��һ��ϵͳ��λ�¼�												*/
/*	DB_Register_FileDown ()			ע��һ���ļ���װ�¼�												*/
/*	DB_Register_Comm ()				ע��һ��ͨ���¼�													*/
/*	DB_Register_BO()				ע��һ��ң���¼�													*/
/*	DB_Register_AO()				ע��һ��ң���¼�													*/
/*	DB_Register_SETTING()			ע��һ���޸Ķ�ֵ�¼�												*/
/*	DB_Register_SETArea ()			ע��һ���޸ĵ�ǰ��ֵ�����¼�										*/
/*	DB_Register_ParaChange ()		ע��һ���޸Ĳ����¼�												*/
/*	DB_Register_PowerOnOff ()		ע��һ���޸Ĳ����¼�												*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*      Daedef.h                     ���л�����ص����ݽṹ												*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0												*/
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
	���ܣ���ȡ���м�¼��������δ��ȡ���ݸ�����
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordNum(WORD *retval);

/************************************************************
	���ܣ����м�¼�������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_IsRunRecordEmpty();

/************************************************************
	���ܣ������ݿ��ж�ȡһ�����м�¼��Ϣ
	������	rp   	--  ��ȡ�����õ�ָ��,��BLANKʱ���ڲ���ָ��
			*event	--	���ص�����ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_ReadRunRecord(TRecordBlock *event, DWORD rp );

/************************************************************
	���ܣ������м�¼����д��һ�����м�¼���ݡ�
	������*str,		--  ���м�¼�ַ���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_WriteRunRecord(char *str);

/************************************************************
	���ܣ���ȡ���м�¼������ͷָ�롣
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordHead(WORD *retval);

/************************************************************
	���ܣ���ȡ���м�¼������βָ�롣
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordTail(WORD *retval);

/************************************************************
	���ܣ���ȡ���м�¼��������ָ�롣
	������*retval	��������ָ��
	���أ�δ��ȡ���ݸ���
************************************************************/
STATUS DB_GetRunRecordRead(WORD *retval);

/************************************************************
	���ܣ��������м�¼��������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_SetRunRecordRead();

/************************************************************
	���ܣ��������м�¼��������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_IncRunRecordRead();

/************************************************************
	���ܣ�����������Ϣ������������Ϣ�ʹ����¼
	������iof,  --  �ɱ����
	���أ���
************************************************************/
//void ReportMsg(char *iof,...);

/************************************************************
	���ܣ��Ǽ��쳣��Ϣ�������쳣��Ϣ����,�ļ���,�к�,��ǰ����,ʱ��,�쳣��Ϣ��
	������	func	--��ǰ������;
			file	--��FILE_LINE�굱ǰ�������ƺ��к�;
			iof		--�ɱ����
	���أ���
************************************************************/
//void LogError(char *func,char *file,char *iof,...);


/************************************************************
	���ܣ���ȡ�����¼���������δ��ȡ�¼����ݸ�����
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_Misc_GetNum(WORD *retval);

/************************************************************
	���ܣ������¼��������Ƿ�Ϊ�ա�
	��������
	���أ�TRUE/FALSE
************************************************************/
BOOL DB_Misc_IsEmpty();

/************************************************************
	���ܣ���ȡ�����¼�������ͷָ�롣
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_Misc_GetHead(WORD *retval);

/************************************************************
	���ܣ���ȡ�����¼�������βָ�롣
	��������
	���أ��¼�������βָ�롣
************************************************************/
STATUS DB_Misc_GetTail(WORD *retval);

/************************************************************
	���ܣ���ȡ�����¼���������ָ�롣
	������*retval	��������ָ��
	���أ���ȷ����0�����󷵻ش������
************************************************************/
STATUS DB_Misc_GetRead(WORD *retval);


/************************************************************
	���ܣ����ò����¼���������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_SetRead(WORD rp);

/************************************************************
	���ܣ����������¼���������ָ�롣
	��������
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_IncRead();

/************************************************************
	���ܣ��Ӳ����¼��������ж�һ�������¼����ݡ�
	������rp	    --  ���
		data,		--  �����¼���¼
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_Read(DWORD rp, TMiscEvent *data);
		
/************************************************************
	���ܣ�������¼���������д��һ�������¼����ݡ�
	������data,		--  �����¼���¼
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DB_Misc_Write(TMiscEvent *data);

/************************************************************
	���ܣ���ȡ�ļ�����
	������strFileName,		--  �ļ���
			dwSize			--  �ļ�����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DAE_GetFileLength(char *strFileName,DWORD *dwSize);

/************************************************************
	���ܣ����ϵͳ�ļ��Ϸ���
	������pFile,		--  �ļ����ݻ�����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DAE_CheckSystemFile(BYTE *pFile,DWORD dwLen);

/************************************************************
	���ܣ���ȡ�ļ�����
	������strFileName,		--  �ļ���
			pFile			--  �ļ�����ָ��
			dwFileLen		--  �ļ�����
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DAE_ReadFile(char *strFileName,BYTE *pFile,DWORD dwFileLen);

/************************************************************
	���ܣ���ȡ�ļ�����
	������strFileName,		--  �ļ���
			ppFile			--  �ļ�����ָ�룬
			dwFileLen		--	Ҫд���ļ��ĳ���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DAE_WriteFile(char *strFileName,void *pFile,DWORD dwFileLen);

STATUS EEPROM_Read(DWORD Addr,DWORD nSize,void *pBuf);
STATUS EEPROM_Write(DWORD Addr,DWORD nSize,void *pBuf);

/************************************************************
	���ܣ�ȡ�豸���ͺ�
	��������
	���أ�װ�����͡�
************************************************************/
WORD DB_GetDevType();

/************************************************************
	���ܣ�ȡ�����CRC��
	��������
	���أ�װ�����͡�
************************************************************/
WORD DB_GetCodeCrc();

/************************************************************
	���ܣ�ȡ�豸��ID
	��������
	���أ�װ�����͡�
************************************************************/
DWORD DB_GetDevID();

/************************************************************
	���ܣ�ȡ�豸��ID
	��������
	���أ�װ�����͡�
************************************************************/
DWORD DB_GetDevAddress();

/************************************************************
	���ܣ�ȡ��λ����ַ
	��������
	���أ���λ����ַ��
************************************************************/
DWORD DB_GetMainAddress();

/************************************************************
	���ܣ���ȡӦ�ó����ļ��汾��Ϣ
	������pFileInf			--   �ļ��汾��Ϣ����ָ��
		  pCrcLen			--   CRCУ�鳤��
	���أ���ȷ�����
************************************************************/
STATUS	DB_GetCodeFileVerInf(TCodeFileInf *pFileInf, DWORD *pCrcLen);

//�����������У�飬16λ�ۼӺ�У��
STATUS DB_CodeFileCheckSum(TCodeFileInf *pFileInf, DWORD dwCheckLen);

//Ϊ��֤ϵͳ�ɿ��ԣ���Ҫ��Ӧ�ó�������ݽ��кϷ��Լ�飬ȷ��д���Ӧ�ó���������Ч
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
	���ܣ�ע��˿�ͨ��״̬
	������wPort			--   �˿�ID
		  Status		--   ͨ��״̬
	���أ���ȷʱ����DB_ERR_OK�����󷵻ش�����
************************************************************/
STATUS DAE_SetCommStatus(WORD wPort,BYTE Status);

/************************************************************
	���ܣ���ȡ�˿�ͨ��״̬
	������wPort			--   �˿�ID
		  Status		--   ͨ��״̬
	���أ���ȷʱ����DB_ERR_OK�����󷵻ش�����
************************************************************/
STATUS DAE_GetCommStatus(WORD wPort,BYTE &Status);


STATUS DAE_UpdateBasicConfig();

/************************************************************
	���ܣ���ȡ����λ����ͨ��״̬
	������pStatus		--   ͨ��״̬λ��ָ��
	���أ���ȷʱ����DB_ERR_OK�����󷵻ش�����
************************************************************/
STATUS DAE_GetMBOCommStatus(BYTE *pStatus);

#define DAE_TRANSPORT_BROADCAST	0xAAAA

/************************************************************
	���ܣ���ȡ͸�����ݻ�����βָ�롣
	��������
	���أ��¼�������βָ�롣
************************************************************/
WORD DAE_GetTransDataTail();

/************************************************************
	���ܣ������ݿ��ж�ȡһ��͸������
	������	rp   	--  ��ȡ�����õ�ָ��
			*event	--	���ص�����ָ��
	���أ���Ч�������ݳ��ȣ���Ч����0
************************************************************/
WORD DAE_ReadTransData(BYTE *pBuf,WORD rp);

/************************************************************
	���ܣ���͸�����ݿ���д��һ��͸�����ݡ�
	������*str,		--  ���м�¼�ַ���
	���أ���ȷ����0�����󷵻ش���ֵ��
************************************************************/
STATUS DAE_WriteTransData(WORD wClass,WORD wDesPort,WORD wDataLen,BYTE *pBuf);

/************************************************************
	���ܣ���ȡ��Ӧ��̫���ڵ�MAC��Ϣ
	������devno		--   ���ڱ��
	������pMac		--   ��Ҫ��ȡ��MAC��Ϣ
	���أ���ȷʱ����DB_ERR_OK�����󷵻ش�����
************************************************************/
STATUS DB_GetNetMac(BYTE devno, BYTE *pMac);

#endif
