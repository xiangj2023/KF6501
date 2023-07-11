/********************************************************************************************************/
/*																										*/
/*        						天津凯发电气股份有限公司												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           													*/
/*																										*/
/*      relayfunc.cpp                                     												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义保护库中相关的接口函数																*/
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
/*	DB_GetRMNum()					从数据库中取保护测量值个数											*/
/*	DB_ReadRM()						从遥测库中取一个保护测量值数据										*/
/*	DB_WriteRM()					向遥测库中写一个保护测量值数据										*/
/*	DB_GetAITofRM()					读取一个保护测量值的遥测类型描述									*/
/*	DB_GetCFofRM()					读取一个保护测量值的系数											*/
/*	DB_GetRTofRM()					读取一个保护测量值的变比											*/
/*	DB_RelayStart()					保护动作启动注册													*/
/*	DB_RelayWriteFD()				写入保护启动的故障电量												*/
/*	DB_RelayAction()				写入保护动作分报告													*/
/*	DB_RelayEnd()					保护动作结束														*/
/*	DB_RelayAlarm()					产生保护告警报告													*/
/*	DB_ReadRelayAlarmReport()		从设备库中读取保护高警报文											*/
/*	DB_WriteRelayAlarmReport()		向设备库中写入保护告警报文											*/
/*	DB_GetRelayAlarmReportNum()		获取保护告警报文库中未读取报文的数目								*/
/*	DB_GetRelayAlarmReportRead()	获取保护告警报文库的读指针											*/
/*	DB_GetRelayAlarmReportHead()	获取保护告警信息缓冲区的头指针										*/
/*	DB_IncRelayAlarmReportRead()	递增保护告警信息缓冲区的头指针										*/
/*	DB_ReadRelayStartReport()		从设备库中读取保护启动报文											*/
/*	DB_WriteRelayStartReport()		向设备库中写入保护启动报文											*/
/*	DB_GetRelayStartReportNum()		获取保护启动报文库中未读取报文的数目								*/
/*	DB_GetRelayStartReportRead()	获取保护启动报文库的读指针											*/
/*	DB_GetRelayStartReportHead()	获取保护启动信息缓冲区的头指针										*/
/*	DB_IncRelayStartReportRead()	递增保护启动信息缓冲区的头指针										*/
/*	DB_ReadRelayActionReport()		从设备库中读取保护动作报文											*/
/*	DB_WriteRelayActionReport()		向设备库中写入保护动作报文											*/
/*	DB_GetRelayActionReportNum()	获取保护动作报文库中未读取报文的数目								*/
/*	DB_GetRelayActionReportRead()	获取保护动作报文库的读指针											*/
/*	DB_GetRelayActionReportHead()	获取保护动作信息缓冲区的头指针										*/
/*	DB_IncRelayActionReportRead()	递增保护动作信息缓冲区的头指针										*/
/*	DB_GetSWNum()					从数据库中取软压板个数												*/
/*	DB_GetSWStatus()				从数据库中取一个软压板状态											*/
/*	DB_SetSWStatus()				向数据库中写入一个软压板状态										*/
/*	DB_SetSWStatusConfirm()			向数据库中写入一个软压板状态确认									*/
/*	DB_GetCFGNum()					从数据库中取一个配置字状态											*/
/*	DB_GetCFGStatus()				从数据库中取一个配置字状态											*/
/*	DB_SetCFGStatus()				向数据库中写入一个配置字状态										*/
/*	DB_SetCFGStatusConfirm()		向数据库中写入一个配置字状态确认									*/
/*	DB_GetSETNum()					从数据库中取定值个数												*/
/*	DB_GetSETFloatValue()			从数据库中读取定值的浮点数											*/
/*	DB_GetSETFixValue()				从数据库中读取定值的整点数											*/
/*	DB_SetSETValue()				向数据库中写入定值													*/
/*	DB_SetSETValueConfirm()			向数据库中写入定值确认												*/
/*	DB_GetCurrentSETAreaNo()		读取当前定值区号													*/
/*	DB_SetCurrentSETAreaNo()		设置当前定值区号													*/
/*	DB_GetSystemSETNum()			从数据库中取辅助定值个数											*/
/*	DB_GetSystemSETFloatValue()			从数据库中读取辅助定值（浮点数）									*/
/*	DB_GetSystemSETFixValue()			从数据库中读取辅助定值（定点数）									*/
/*	DB_SetSystemSETValue()			向数据库中写入辅助定值确认											*/
/*	DB_SetSystemSETValueConfirm()	向数据库中写入辅助定值确认											*/
/*	DB_GetRTNum()					从数据库中取变比个数												*/
/*	DB_GetRTValue()					从数据库中读取变比（浮点数）										*/
/*	DB_SetRTValue()					向数据库中写入变比													*/
/*	DB_SetRTValueConfirm()			向数据库中写入变比确认												*/
/*	DB_GetCFNum()					从数据库中取系数个数												*/
/*	DB_GetCFValue()					从数据库中读取系数值												*/
/*	DB_SetCFValue()					向数据库中写入系数值												*/
/*	DB_SetCFValueConfirm()			向数据库中写入系数值确认											*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*		Dbfunc.h						数据库函数声明													*/
/*		Db_err.h						数据库返回值													*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
/********************************************************************************************************/
#include "daefunc.h"		
#include "dbfunc.h"			
#include "parafunc.h"	
#include "Relaydef.h"
#include "Sample.h"
#include "initrelaydata.h"

#define	SYSTEM_SET_DEFINE1	0

extern	TSysConfigTable		*G_pCfgTable;
extern	CLink				G_RelayRepSaveLink;
extern	CLink				G_RelayRepFreeLink;
extern	CLink				G_DistDataSaveLink;		//故障录波数据存储链表
extern	CLink				G_DistDataFreeLink;		//故障录波数据空闲链表
extern 	HANDLE				g_hBOTaskID;
extern 	HANDLE				g_hLCDTaskID;
extern 	HANDLE				g_hRelayTaskID;
extern 	HANDLE				g_hDbaseTaskID;

extern	void				*G_pDistFileBlock;		//录波文件缓冲

STATUS ReadHisSampData(DWORD dwPtr, DWORD dwMask, SHORT *pBuf, DWORD dwPointNum);
BOOL isSampDataLenFill(DWORD dwStartPtr, DWORD wPointNum);
DWORD BackSamplePtr(DWORD dwStartPtr, DWORD wPointNum);
STATUS DAE_ReadFlashFile(char *strFileName,void *pFile,DWORD dwFileLen);

WORD	g_wRelayDistNo;

DWORD	*DB_GetSWBase()
{
	return G_pCfgTable->DBaseConfig.pSWData;
}

DWORD	*DB_GetCFGBase()
{
	return G_pCfgTable->DBaseConfig.pCFGData;
}

float	*DB_GetSETFloatBase()
{
	return G_pCfgTable->DBaseConfig.pSETDataFloat;
}

long	*DB_GetSETFixBase()
{
	return G_pCfgTable->DBaseConfig.pSETDataFix;
}

float	*DB_GetSystemSETBase()
{
	return G_pCfgTable->DBaseConfig.pSystemSETData;
}

long	*DB_GetCFBase()
{
	return G_pCfgTable->DBaseConfig.pCFData;
}

BOOL DB_CheckSWData(DWORD *pSW,WORD num)
{
	WORD 					i,j;
	DWORD					Mask;

	if(pSW == NULL)
		return FALSE;
	
	//逐位检测反码校验
	j = (num+31)/32;
	
	for(i=0;i<num;i++)
	{
		Mask = 0x01<<(i%32);
		
		if((pSW[i/32]&Mask) == (pSW[j+(i/32)]&Mask))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL DB_CheckSETData(float *pSET,WORD	num)
{
	if(pSET == NULL)
		return FALSE;
	
	if(DAE_IsCheckSumOK((BYTE *)pSET, num*sizeof(float)))
		return TRUE;
	else
		return FALSE;
}

BOOL DB_CheckSETFixData(long *pSET,WORD	num)
{
	if(pSET == NULL)
		return FALSE;
	
	if(DAE_IsCheckSumOK((BYTE *)pSET, num*sizeof(long)))
		return TRUE;
	else
		return FALSE;
}

BOOL DB_CheckSETArea(WORD AreaCur,WORD AreaB)
{
	if((AreaCur^AreaB)!=0xFFFF)
		return FALSE;
	else
		return TRUE;
}

BOOL DB_CheckCFData(long *pCF,WORD num)
{
	if(pCF == NULL)
		return FALSE;
	
	if(DAE_IsCheckSumOK((BYTE *)pCF, num*sizeof(long)))
		return TRUE;
	else
		return FALSE;
}

/*浮点定值到整点定值*/
STATUS ConvertSETFloat2Fix(WORD no,float fValue,long *plValue)
{
	TSET_Disp				*pSETDisp;
	WORD					CFGNum,nCFIndex;
	long					lcf,lRatio;
	
	ASSERT(plValue != NULL)
	if(plValue == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSETNum())
	if(no >= DB_GetSETNum())
		return DB_ERR_NO;
	
		//配置字例外
	CFGNum = (DB_GetCFGNum()+31)/32;
	if(no<CFGNum)
	{
		*plValue = *(DWORD *)&fValue;
		return DB_ERR_OK;
	}	
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSETDisp;
	nCFIndex = pSETDisp[no].nCFIndex;

	ASSERT(nCFIndex < DB_GetCFNum())
	if(nCFIndex >= DB_GetCFNum())
	{
		LogError("ConvertSETFloat2Fix",FILE_LINE,"InValid CF No %d!!!",nCFIndex);
		return DB_ERR_NO;
	}
	
	lcf	 		= G_pCfgTable->DBaseConfig.pCFData[nCFIndex];
	lRatio	 	= G_pCfgTable->DBaseConfig.pCFDisp[nCFIndex].wCof;

	if(lcf == 0) 
		lcf = 1;
	if(lRatio == 0) 
		lRatio = 1;
	
	//	乘系数/倍率	
	*plValue = (long)(fValue*lcf)/lRatio;

	return DB_ERR_OK;
}

/*整点定值到浮点定值*/
STATUS ConvertSETFix2Float(WORD no,long lValue,float *pfValue)
{
	TSET_Disp				*pSETDisp;
	WORD					nCFIndex;
	long					lcf,lRatio;
	
	ASSERT(pfValue != NULL)
	if(pfValue == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSETNum())
	if(no >= DB_GetSETNum())
		return DB_ERR_NO;
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSETDisp;
	nCFIndex = pSETDisp[no].nCFIndex;

	ASSERT(nCFIndex < DB_GetCFNum())
	if(nCFIndex >= DB_GetCFNum())
	{
		LogError("ConvertSETFix2Float",FILE_LINE,"InValid CF No %d!!!",nCFIndex);
		return DB_ERR_NO;
	}
	
	lcf	 		= G_pCfgTable->DBaseConfig.pCFData[nCFIndex];
	lRatio	 	= G_pCfgTable->DBaseConfig.pCFDisp[nCFIndex].wCof;

	if(lcf == 0) 
		lcf = 1;
	if(lRatio == 0) 
		lRatio = 1;
	
	//	乘系数/倍率	
	*pfValue = (float)(lValue*lRatio)/lcf;

	return DB_ERR_OK;
}

STATUS ConvertSystemSETFloat2Fix(WORD no,float fValue,long *plValue)
{
	TSysSET_Disp			*pSETDisp;
	WORD					nCFIndex;
	long					lcf,lRatio;
	
	ASSERT(plValue != NULL)
	if(plValue == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSystemSETNum())
	if(no >= DB_GetSystemSETNum())
		return DB_ERR_NO;
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSystemSETDisp;
	nCFIndex = pSETDisp[no].nCFIndex;

	ASSERT(nCFIndex < DB_GetCFNum())
	if(nCFIndex >= DB_GetCFNum())
	{
		LogError("ConvertSystemSETFloat2Fix",FILE_LINE,"InValid CF No!!!");
		return DB_ERR_NO;
	}
	
	lcf	 		= G_pCfgTable->DBaseConfig.pCFData[nCFIndex];
	lRatio	 	= G_pCfgTable->DBaseConfig.pCFDisp[nCFIndex].wCof;

	if(lcf == 0) 
		lcf = 1;
	if(lRatio == 0) 
		lRatio = 1;
	
	//	乘系数/倍率	
	*plValue = (long)(fValue*lcf)/lRatio;

	return DB_ERR_OK;
}

/************************************************************
	功能：转换保护测量值为二次浮点数
	参数：wNo		-- 保护测量值序号
		lValue		-- 保护测量值定点数
		*pfValue	--转换后的浮点数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS ConvertRMFix2SecondFloat(WORD	wNo,long lValue,float *pfValue)
{
	TCF_Disp			tCFDisp;
	STATUS				rc;
	long				lCFValue = 1;
	long				lCFRatio = 1;
	
	// 1。检查指针的合法性
	ASSERT(pfValue != NULL);
	if(pfValue == NULL)
		return DB_ERR_PTRNULL;

	//转换数值为浮点格式
	rc = DB_GetCFofRM(wNo, &lCFValue);
	if((rc != DB_ERR_OK)||(lCFValue == 0))
	{
		lCFValue = 1;
		LogError("ConvertRMFix2SecondFloat",FILE_LINE,"InValid CF !!!");
	}

	rc = DB_GetCFDispofRM(wNo, &tCFDisp);
	if((rc != DB_ERR_OK)||(tCFDisp.wCof == 0))
	{
		lCFRatio = 1;
		LogError("ConvertRMFix2SecondFloat",FILE_LINE,"InValid COF !!!");
	}
	else
		lCFRatio = tCFDisp.wCof;

	
	*pfValue = (float)(lValue*lCFRatio)/lCFValue;

	return rc;
}

/************************************************************
	功能：转换保护测量值为一次浮点数
	参数：wNo		-- 保护测量值序号
		lValue		-- 保护测量值定点数
		*pfValue	--转换后的浮点数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS ConvertRMFix2FirstFloat(WORD	wNo,long lValue,float *pfValue)
{
	TCF_Disp			tCFDisp;
	STATUS				rc;
	long				lCFValue = 1;
	long				lCFRatio = 1;
	float				fRT = 1;
	long				FirRate=1;
	
	TRM_Disp		tRMDisp;
	// 1。检查指针的合法性
	ASSERT(pfValue != NULL);
	if(pfValue == NULL)
		return DB_ERR_PTRNULL;

	//转换数值为浮点格式
	rc = DB_GetCFofRM(wNo, &lCFValue);
	if((rc != DB_ERR_OK)||(lCFValue == 0))
	{
		lCFValue = 1;
		LogError("ConvertRMFix2FirstFloat",FILE_LINE,"InValid CF !!!");
	}

	rc = DB_GetCFDispofRM(wNo, &tCFDisp);
	if((rc != DB_ERR_OK)||(tCFDisp.wCof == 0))
	{
		lCFRatio = 1;
		LogError("ConvertRMFix2FirstFloat",FILE_LINE,"InValid COF !!!");
	}
	else
		lCFRatio = tCFDisp.wCof;

	//一次数据要乘变比
	rc = DB_GetRTofRM(wNo, &fRT);
	if(rc != DB_ERR_OK)
	{
		fRT = 1;
		LogError("ConvertRMFix2FirstFloat",FILE_LINE,"InValid RT !!!");
	}

	
	rc = DB_GetRMDisp(wNo, &tRMDisp); //保护测量值显示一次时要除进制，yanzh 2012年7月14日15:33:13
	if(rc != DB_ERR_OK)
		return rc;
	
	switch(tRMDisp.nFirRate)   
	{
		case 0:  break; 
		case 1: FirRate = 10; break;	
		case 2: FirRate = 100; break;	
		case 3: FirRate = 1000; break;	
		default:  break;	
	}
	*pfValue = (float)(lValue*lCFRatio*fRT)/(lCFValue*FirRate);

	return rc;
}

//保护故障电量转换成一次数据
STATUS	ConvertRelayEvent2FirstData(TRelayRecordHead *pEvent)
{
	TRelayEventHead tRelayEvent;
	TRE_Disp		tREDisp;
	BYTE			*aEvent;
	BYTE			*pDataBuf;
	DWORD			dwValue;
	float			fValue;
	float			fRT = 1;

	DWORD			dwOffset;
	int 			m,n;
	STATUS			rc;
	TRM_Disp		tRMDisp;

	
	ASSERT(pEvent != NULL);
	if(pEvent == NULL)
	{
		LogError("DB_ConvertRelayEvent2FirstData",FILE_LINE,"pEvent Is NULL!!!");
		return DB_ERR_BUFNULL;
	}

	aEvent = (BYTE *)&pEvent->tAction;
	dwOffset = 0;

	// 逐个处理分报告
	for(n=0;n<pEvent->Num;n++)
	{
		// 1。取一个分报文头
		memcpy(&tRelayEvent,aEvent+dwOffset,sizeof(TRelayEventHead)); 	
	
		// 2。取出并转换故障电量
		dwOffset += sizeof(tRelayEvent);
		pDataBuf = (BYTE *)&aEvent[dwOffset];
		
		// 2.1 取出保护动作描述
		rc = DB_GetREDispByInf(tRelayEvent.nINF, &tREDisp);		
		if(rc != DB_ERR_OK)
		{
			LogError("DB_ConvertRelayEvent2FirstData",FILE_LINE,"InValid INF !!!");
			return rc;
		}
		
		// 2.2 逐个转换故障电量值
		for(m=0;m<tRelayEvent.FD_Num;m++,pDataBuf+=6)
		{
			rc = DB_GetRMDisp(tREDisp.wRMList[m], &tRMDisp);
			if(rc != DB_ERR_OK)
				return rc;

			if(tRMDisp.wAttr == RM_ATTR_HEX)
				continue;
			
			// 2.2.1 先提取浮点数
			dwValue = MAKEDWORD(pDataBuf[2],pDataBuf[3],pDataBuf[4],pDataBuf[5]);
			fValue = *(float *)&dwValue;

			// 2.2.2 再乘上变比
			rc = DB_GetRTofRM(tREDisp.wRMList[m], &fRT);
			if(rc != DB_ERR_OK)
			{
				fRT = 1;
				LogError("DB_ConvertRelayEvent2FirstData",FILE_LINE,"InValid RT !!!");
			}	
			fValue *= fRT;


			switch(tRMDisp.nFirRate)
			{
				case 0:  break;	
				case 1: fValue /= 10; break;	
				case 2: fValue /= 100; break;	
				case 3: fValue /= 1000; break;	
				default:  break;	
			}
			switch(tRMDisp.nFirDec)
			{
				case 0: fValue = (float)((int)fValue);break;	
				case 1: fValue = ((float)((int)(fValue*10)))/10; break;	
				case 2: fValue = ((float)((int)(fValue*100)))/100; break;	
				case 3: fValue = ((float)((int)(fValue*1000)))/1000; break;	
				default:  break;	
			}

			// 2.2.2 再转回缓冲区
			dwValue = *(DWORD *)&fValue;
			pDataBuf[2] = LLBYTE(dwValue);
			pDataBuf[3] = LHBYTE(dwValue);
			pDataBuf[4] = HLBYTE(dwValue);
			pDataBuf[5] = HHBYTE(dwValue);
		}
	
		// 3。递增至下一个分报告
		dwOffset += tRelayEvent.FD_Num*6;			
	}

	return DB_ERR_OK;
}


/************************************************************
	功能：从数据库中取保护测量值个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetRMNum()
{
	return G_pCfgTable->DBaseConfig.nRM;
}

/************************************************************
	功能：从数据库中取保护动作描述个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetRENum()
{
	return G_pCfgTable->DBaseConfig.nRE;
}

/************************************************************
	功能：从数据库中取传动个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetTCNum()
{
	return G_pCfgTable->DBaseConfig.nTC;
}

/************************************************************
	功能：读取保护测量值原始数据。
	参数：no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadRM( WORD no, long *retval )
{
	TRunTimeRM		*pRM;
	WORD			num;
	float			fValue;
	STATUS			rc;
			   
	if( retval == NULL )
		return DB_ERR_BUFNULL;

	pRM = G_pCfgTable->DBaseConfig.pRM;
	num = G_pCfgTable->DBaseConfig.nRM;

	// 1. 检查序号
	if(no >= num)
	{
		LogError("DB_ReadRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 检查指针
	if( pRM == NULL )
	{
		LogError("DB_ReadRM",FILE_LINE,"The RM base is null!");
		return DB_ERR_BASENULL;
	}

	*retval = pRM[no].lValue;

	return DB_ERR_OK;
}

/************************************************************
	功能：从遥测库中取二次侧保护测量值数据。
	参数：no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadSecondRM( WORD no, long *retval )
{
	TRM_Disp		tRMDisp;
	long			lValue;
	float			fValue;
	STATUS			rc;
			   
	rc = DB_ReadRM(no,&lValue);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_ReadSecondRM",FILE_LINE,"Read RM raw data error!");
		return rc;
	}

	// 3. 将读出的值进行换算
	rc = ConvertRMFix2SecondFloat(no, lValue, &fValue);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_ReadSecondRM",FILE_LINE,"The RM Convert Error!");
		return rc;
	}

	// 4. 按照二次值的小数位数保存定点数
	if(DB_GetRMDisp(no,&tRMDisp) != DB_ERR_OK)											
	{
		LogError("DB_ReadSecondRM",FILE_LINE,"The RM Typr is Error!");
		fValue =  (long)(fValue*1000);
	}

	switch(tRMDisp.nSecDec)
	{
	case 0: *retval = (long)fValue; break;	
	case 1: *retval = (long)(fValue*10); break; 
	case 2: *retval = (long)(fValue*100); break;	
	case 3: *retval = (long)(fValue*1000); break;	
	case 4: *retval = (long)(fValue*10000); break;	
	default: *retval = (long)(fValue*100000); break;	
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：从遥测库中取一次侧保护测量值数据。
	参数：no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadFirstRM( WORD no, long *retval )
{
	TRM_Disp		tRMDisp;
	TRunTimeRM		*pRM;
    WORD 			num;
	float			fValue;
    STATUS			rc;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pRM = G_pCfgTable->DBaseConfig.pRM;
	num = G_pCfgTable->DBaseConfig.nRM;
	
	// 1. 检查序号
	if(no >= num)
	{
		LogError("DB_ReadFirstRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 检查指针
    if( pRM == NULL )
    {
        LogError("DB_ReadFirstRM",FILE_LINE,"The RM base is null!");
        return DB_ERR_BASENULL;
    }
	
	// 3. 将读出的值进行换算
	rc = ConvertRMFix2FirstFloat(no, pRM[no].lValue, &fValue);
	if( rc != DB_ERR_OK)
	{
        LogError("DB_ReadFirstRM",FILE_LINE,"The RM Convert Error!");
		return rc;
	}
	
	// 4. 按照一次值的小数位数保存定点数
	if(DB_GetRMDisp(no,&tRMDisp) != DB_ERR_OK)											
	{
        LogError("DB_ReadFirstRM",FILE_LINE,"The RM disp is Error!");
		fValue =  (long)(fValue*1000);
	}

	switch(tRMDisp.nFirDec)
	{
	case 0: *retval = (long)fValue; break;	
	case 1: *retval = (long)(fValue*10); break;	
	case 2: *retval = (long)(fValue*100); break;	
	case 3: *retval = (long)(fValue*1000); break;	
	case 4: *retval = (long)(fValue*10000); break;	
	default: *retval = (long)(fValue*100000); break;	
	}
	
	return DB_ERR_OK;
}


/************************************************************
	功能：向遥测库中写一个保护测量值数据。
	参数：no,		--  序号
		  value,	--  测量值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteRM(WORD no, long value )
{
	TRunTimeRM		*pRM;
    WORD 			num;
               	
	pRM = G_pCfgTable->DBaseConfig.pRM;
	num = G_pCfgTable->DBaseConfig.nRM;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_WriteRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pRM == NULL )
    {
        LogError("DB_WriteRM",FILE_LINE,"The RM base is null!");
        return DB_ERR_BASENULL;
    }
	
	pRM[no].lValue = value;
		
	return DB_ERR_OK;
}

CNode *DB_RelayCacheSearch (WORD wSeq)
{
	TRelayEventCache	*pRelayCache;
    CNode 				*pTempNode;
	CLink 				*pLink;

	
	pLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	if(pLink == NULL)
		return NULL;
	
	pTempNode = pLink->GetHead();
	//判断链表是否为空链,链表为空链返回
	if(pTempNode == NULL)
		return NULL;

	do
    {
		pRelayCache = (TRelayEventCache	*)pTempNode->pData;
        if ( pRelayCache->nSeq == wSeq)
        {
            return pTempNode;
        }

		//遍历下个节点
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != pLink->GetHead()));

	return NULL;
}

CNode *DB_GetEndedRelayCache ()
{
	TRelayEventCache	*pRelayCache;
    CNode 				*pTempNode;
	CNode 				*pOlderNode= NULL;
	CLink 				*pLink;
	WORD				OlderTimeHi=0xFFFF;					//发生时间。绝对时标
	DWORD				OlderTimeLo=0xFFFFFFFF;				//发生时间。绝对时标
	
	
	pLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	if(pLink == NULL)
		return NULL;
	
	pTempNode = pLink->GetHead();
	//判断链表是否为空链,链表为空链返回
	if(pTempNode == NULL)
		return NULL;
	do
    {
		pRelayCache = (TRelayEventCache	*)pTempNode->pData;

		// 1.是否有待存盘节点
        if ( pRelayCache->bStatus&RELAY_CACHE_END )
        {
			// 2.取较老的事件
			if((pRelayCache->pAction->stHead.AbsTimeHi < OlderTimeHi)||\
				((pRelayCache->pAction->stHead.AbsTimeHi == OlderTimeHi)&&(pRelayCache->pAction->stHead.AbsTimeLo < OlderTimeLo)))
			{
				pOlderNode = pTempNode;
				OlderTimeHi = pRelayCache->pAction->stHead.AbsTimeHi;
				OlderTimeLo = pRelayCache->pAction->stHead.AbsTimeLo;
			}					
        }

		//遍历下个节点
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != pLink->GetHead()));

	return pOlderNode;
}

//将CACHE中的保护动作事项存入动作事项表中
STATUS	DB_RelayCache2Event(TRelayActionFix *pAction,TRelayEventHead *pEvent)
{
	TRE_Disp 			tRelayInfDisp;
	TRM_Disp 			tRMDisp;
	DWORD				dwFDData;
	STATUS				rc;
	
	BYTE				*pFDData;
	int					i;
	
	ASSERT(pAction != NULL);
	if(pAction == NULL)
	{
		LogError("DB_RelayCache2Event",FILE_LINE,"Cache Is NULL!!!");
		return DB_ERR_BUFNULL;
	}

	ASSERT(pEvent != NULL);
	if(pEvent == NULL)
	{
		LogError("DB_RelayCache2Event",FILE_LINE,"pEvent Is NULL!!!");
		return DB_ERR_BUFNULL;
	}
		
	memcpy(pEvent,&pAction->stHead,sizeof(TRelayEventHead));

	// 1 。取保护动作描述
	rc = DB_GetREDispByInf(pAction->stHead.nINF,&tRelayInfDisp);
	ASSERT(rc == DB_ERR_OK);
	if(rc != DB_ERR_OK)
	{
		return DB_ERR_RELAY_INF;
	}
	
	pFDData = (BYTE *)(pEvent+1);
	for(i=0;i<pEvent->FD_Num;i++)
	{	
		if(DB_GetRMDisp(tRelayInfDisp.wRMList[i], &tRMDisp) == DB_ERR_OK)
		{
			pFDData[0] = LOBYTE(tRMDisp.nIndex);
			pFDData[1] = HIBYTE(tRMDisp.nIndex);
		}
		else
		{
			LogError("DB_RelayCache2Event",FILE_LINE,"DB_GetRMDisp(%d) is error!!!",tRelayInfDisp.wRMList[i]);
			pFDData[0] = LOBYTE(tRelayInfDisp.wRMList[i]);
			pFDData[1] = HIBYTE(tRelayInfDisp.wRMList[i]);
		}
		
		// 2.按照保护测量值号转换数值为浮点格式
		if(tRMDisp.wAttr == RM_ATTR_HEX)
			dwFDData = pAction->FD_Value[i];
		else 
		{
//			if(DB_IsFDShowFirst())
//				rc = ConvertRMFix2FirstFloat(tRelayInfDisp.wRMList[i],pAction->FD_Value[i],(float *)&dwFDData);
//			else
				rc = ConvertRMFix2SecondFloat(tRelayInfDisp.wRMList[i],pAction->FD_Value[i],(float *)&dwFDData);
		}
		
		if(rc != DB_ERR_OK)
		{
			LogError("DB_RelayCache2Event",FILE_LINE,"FD Fix to Float Convert Error!!!");
		}
				
		pFDData[2] = LLBYTE(dwFDData);
		pFDData[3] = LHBYTE(dwFDData);
		pFDData[4] = HLBYTE(dwFDData);
		pFDData[5] = HHBYTE(dwFDData);
	
		pFDData += 6;
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：保护动作启动注册
	参数：wSeq		-- 全局临时序号，用于区分并发的两个动作信息，生命周期仅贯穿于一个完整动作报告产生过程
		wInf		--保护动作索引编号
		*pTime		--启动时间
		bMakeStart	--是否记录本次启动报告
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_RelayStart (WORD wSeq, WORD wInf, TAbsTime *pTime,BOOL bMakeStart)
{
	TRelayEventCache	*pRelayCache;
	TRelayEventHead		*pEventHead;
	CLink				*pFreeLink;
	CLink				*pSaveLink;
	CNode				*pNode;
	TAbsTime			tTime;

//	ReportMsg("start wSeq=%d,inf=%d",wSeq,wInf);
	// 1。检查CACHE链表的合法性
	pSaveLink = &G_RelayRepSaveLink;//G_pCfgTable->DBaseConfig.pRelayRepSaveLink;
	pFreeLink = &G_RelayRepFreeLink;//G_pCfgTable->DBaseConfig.pRelayRepFreeLink;

	ASSERT((pSaveLink != NULL)&&(pFreeLink != NULL));
	if( (pSaveLink == NULL)||(pFreeLink == NULL))
	{
		LogError("DB_RelayStart",FILE_LINE,"The Cache Link is NULL");
		return DB_ERR_BASENULL;
	}
	
	// 2。检查序号的合法性
	if( DB_RelayCacheSearch(wSeq) != NULL)
	{
		//序号不合法，为避免报文混乱，直接返回
		LogError("DB_RelayStart",FILE_LINE,"SEQ Error!!!");
		return DB_ERR_RELAY_SEQ;
	}
	
	OS_Protect(0);
	
	// 3。申请一个空闲CANCHE节点
	pNode = pFreeLink->RemoveHead();
	if(pNode == NULL)
	{
		OS_UnProtect(0);
		LogError("DB_RelayStart",FILE_LINE,"The Report Cache run out!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
	pRelayCache = (TRelayEventCache	*)pNode->pData;
	// 申请失败直接返回
	ASSERT(pRelayCache != NULL);
	
	// 4。注册CACHE节点相关信息
	pRelayCache->bStatus = 0;
	
	if(bMakeStart&&(wInf != RELAY_INF_NOUSED))
		pRelayCache->bStatus |= RELAY_CACHE_MAKESTART;
	else
		pRelayCache->bStatus &= ~RELAY_CACHE_MAKESTART;
	
	pRelayCache->bStatus 	&= ~RELAY_CACHE_STATUSMASK;
	pRelayCache->bStatus 	|= RELAY_CACHE_START;
	pRelayCache->nSeq 		= wSeq;
	pRelayCache->nCnt 		= 1;
	pRelayCache->dwTimeOut 	= RELAY_CACHE_HOLDTIME;

	pEventHead = &pRelayCache->pAction[0].stHead;
	pEventHead->nINF	= wInf;
	pEventHead->bStatus = RELAYEVENT_STATUS_ACT;
	pEventHead->FD_Num  = 0;

	if(pTime == NULL)
	{
		ReadAbsTime(&tTime);
		pEventHead->AbsTimeHi = tTime.Hi;
		pEventHead->AbsTimeLo = tTime.Lo;	
	}
	else
	{
		pEventHead->AbsTimeHi = pTime->Hi;
		pEventHead->AbsTimeLo = pTime->Lo;	
	}
	// 5。链入待存盘链表
	pSaveLink->AddTail(pNode);
		
	OS_UnProtect(0);

	return DB_ERR_OK;
}

/************************************************************
	功能：写入保护启动的故障电量
	参数：Seq	-- 全局临时序号，同上。
	  pFDList	-- 故障电量列表
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_RelayWriteFD (WORD Seq, long *pFDList)
{
	TRE_Disp 			tRelayInfDisp;
	TRelayEventCache	*pCache;
	TRelayActionFix		*pEvent;
    CNode 				*pNode;
	STATUS				rc;
	
	// 1。寻找对应序号的CACHE报告
	pNode = DB_RelayCacheSearch(Seq);
	ASSERT(pNode != NULL);
	if(pNode == NULL)
	{
		LogError("DB_RelayWriteFD",FILE_LINE,"InValid SEQ !!!");
		return DB_ERR_RELAY_SEQ;
	}

	OS_Protect(0);
	
	// 2。读取INF信息
	pCache = (TRelayEventCache *)pNode->pData;

	if(pCache->bStatus&RELAY_CACHE_END)
	{
		OS_UnProtect(0);
		LogError("DB_RelayWriteFD",FILE_LINE,"SEQ%d is end!!!",Seq);
		return DB_ERR_RELAY_SEQ;
	}
	
	pEvent = &pCache->pAction[0];
	if(pEvent->stHead.nINF == RELAY_INF_NOUSED)
	{
		OS_UnProtect(0);
		return DB_ERR_OK;
	}

	rc = DB_GetREDispByInf(pEvent->stHead.nINF,&tRelayInfDisp);
	ASSERT(rc == DB_ERR_OK);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_RelayWriteFD",FILE_LINE,"InValid INF!!!");
		OS_UnProtect(0);
		return DB_ERR_RELAY_INF;
	}
	
	// 3。写入故障电量
	pEvent->stHead.FD_Num = tRelayInfDisp.nFD;
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		//pEvent->FD_Index[i] = tRelayInfDisp.wFDList[i];
		pEvent->FD_Value[i] = pFDList[i];
	}

	pCache->dwTimeOut 	= RELAY_CACHE_HOLDTIME;
	
	OS_UnProtect(0);

	return DB_ERR_OK;
}

/************************************************************
	功能：写入保护动作分报告
	参数：Seq	-- 全局临时序号，同上。
		pAction	-- 保护分报告信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_RelayAction (WORD Seq, TRelayEventItem *pAction)
{
	TRE_Disp 			tRelayInfDisp;
	TRelayEventCache	*pCache;
	TRelayActionFix		*pEvent;
    CNode 				*pNode;
	STATUS				rc;

	// 1。检查事项指针的合法性
	ASSERT(pAction != NULL);
	if(pAction == NULL)
		return DB_ERR_PTRNULL;
	
	// 2。寻找对应序号的CACHE报告
	pNode = DB_RelayCacheSearch(Seq);
	ASSERT(pNode != NULL);
	if(pNode == NULL)
	{
		LogError("DB_RelayAction",FILE_LINE,"InValid SEQ !!!");
		return DB_ERR_RELAY_SEQ;
	}
	
	pCache = (TRelayEventCache *)pNode->pData;
	ASSERT(pCache != NULL);
	if(pCache == NULL)
	{
		LogError("DB_RelayAction",FILE_LINE,"Cache Is Empty!!!");
		return DB_ERR_RELAY_NOCACHE;
	}

	if(pCache->bStatus&RELAY_CACHE_END)
	{
		LogError("DB_RelayAction",FILE_LINE,"SEQ%d is end!!!",Seq);
		return DB_ERR_RELAY_SEQ;
	}
	
	if(pCache->nCnt >= MAX_SEGACT_NUM)
	{
		LogError("DB_RelayAction",FILE_LINE,"Too Many Part Report!!!");
		return DB_ERR_RELAY_OVERFLOW;
	}
	
	// 3。读取INF信息
	rc = DB_GetREDispByInf(pAction->nINF,&tRelayInfDisp);
	ASSERT(rc == DB_ERR_OK)
	if(rc != DB_ERR_OK)
	{
		LogError("DB_RelayAction",FILE_LINE,"InValid INF(%d)!!!",pAction->nINF);
		return DB_ERR_RELAY_INF;
	}
	
	OS_Protect(0);
	
	// 3。写入报告
	pEvent = &pCache->pAction[pCache->nCnt];
	
	pEvent->stHead.nINF 	= pAction->nINF;
	pEvent->stHead.bStatus 	= pAction->bStatus&RELAYEVENT_STATUS_MASK;
	pEvent->stHead.FD_Num 	= tRelayInfDisp.nFD;
	pEvent->stHead.AbsTimeHi= pAction->stTime.Hi;
	pEvent->stHead.AbsTimeLo= pAction->stTime.Lo;
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		//pEvent->FD_Index[i] = tRelayInfDisp.wFDList[i];
		pEvent->FD_Value[i] = pAction->FD_Value[i];
	}
	
	// 2。更新CACHE信息
	pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
	pCache->bStatus |= RELAY_CACHE_ACTED;
	pCache->dwTimeOut= RELAY_CACHE_HOLDTIME;
	pCache->nCnt++;
	
//	ReportMsg("act wSeq=%d,inf=%d",Seq,pAction->nINF);
	OS_UnProtect(0);

	return DB_ERR_OK;
}
	
/************************************************************
	功能：保护动作结束
	参数：Seq	-- 全局临时序号，同上。
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_RelayEnd (WORD Seq,BOOL bValid)
{
	TRelayEventCache	*pCache;
    CNode 				*pCacheNode;
	
	// 1。寻找对应序号的CACHE报告
	pCacheNode = DB_RelayCacheSearch(Seq);
	ASSERT(pCacheNode != NULL);
	if(pCacheNode == NULL)
	{
		LogError("DB_RelayEnd",FILE_LINE,"InValid SEQ !!!");
		return DB_ERR_RELAY_SEQ;
	}
	
	pCache = (TRelayEventCache *)pCacheNode->pData;
	ASSERT(pCache != NULL);
	if(pCache == NULL)
	{
		LogError("DB_RelayEnd",FILE_LINE,"Cache Is Empty!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
	
	OS_Protect(0);

	// 2。更新CACHE信息
	if(!bValid)
	{
		pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
		pCache->bStatus  = RELAY_CACHE_READY;
		pCache->dwTimeOut= 0;

		// 释放存储链表
		G_RelayRepSaveLink.RemoveNode(pCacheNode);
		G_RelayRepFreeLink.AddTail(pCacheNode);
		
		LogError("DB_RelayEnd",FILE_LINE,"Cache is abandoned!");

		OS_UnProtect(0);
		
		return DB_ERR_OK;
	}

	pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
	pCache->bStatus |= RELAY_CACHE_END;
	pCache->dwTimeOut= RELAY_CACHE_HOLDTIME;

	OS_UnProtect(0);

//	ReportMsg("end wSeq=%d",Seq);

	OS_PostMessage(g_hDbaseTaskID, SM_SAVEREPORT, 1, Seq, 0, 0);
	
	return DB_ERR_OK;
}
/************************************************************
	功能：写入保护动作分报告
	参数：Seq	-- 全局临时序号，同上。
		pAction	-- 保护分报告信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_RelayAlarm (TRelayEventItem *pAlarm)
{
	TRE_Disp 			tRelayInfDisp;
	TRM_Disp 			tRMDisp;
	TRelayAlarmEv		tAlarmEv;
	TRelayEventHead		*pEvent;
	TRelayRecordHead	*pRecord;
	BYTE				*pFDData;
	STATUS				rc;
	WORD				wLength;
	WORD				wFDIndex;
	DWORD				dwFDData;

	// 1。检查事项指针的合法性
	if( pAlarm == NULL )
		 return DB_ERR_BUFNULL;

	// 2。读取INF信息
	rc = DB_GetREDispByInf(pAlarm->nINF,&tRelayInfDisp);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_RelayAlarm",FILE_LINE,"InValid INF %d!!!",pAlarm->nINF);
		return DB_ERR_RELAY_INF;
	}

	// 3。检查报告长度不能越限
	wLength = sizeof(TRelayRecordHead)+tRelayInfDisp.nFD*(sizeof(WORD)+sizeof(float));
	ASSERT(wLength<=MAX_RALM_LENGTH);
	if(wLength > MAX_RALM_LENGTH)
	{
		LogError("DB_RelayAlarm",FILE_LINE,"ALARM REPORT too long!!! FD=%d",tRelayInfDisp.nFD);
		return DB_ERR_RELAY_OVERFLOW;
	}
	
	// 4。填充报文头
	pRecord = &tAlarmEv.tEvent;
	pRecord->Num = 1;
	pRecord->wFaultNo = 0xFFFF;
	
	pEvent = &pRecord->tAction;
	pEvent->bStatus = pAlarm->bStatus;
	pEvent->nINF 	= pAlarm->nINF;
	pEvent->FD_Num 	= tRelayInfDisp.nFD;
	pEvent->AbsTimeHi	= pAlarm->stTime.Hi;
	pEvent->AbsTimeLo	= pAlarm->stTime.Lo;
		
	// 5。写入故障电量
	pFDData = tAlarmEv.aEvent+sizeof(TRelayRecordHead);
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		if(DB_GetRMDisp(tRelayInfDisp.wRMList[i], &tRMDisp) == DB_ERR_OK)
			wFDIndex = tRMDisp.nIndex;
		else
		{
			LogError("DB_RelayAlarm",FILE_LINE,"DB_GetRMDisp(%d) is error!!!",tRelayInfDisp.wRMList[i]);
			wFDIndex = tRelayInfDisp.wRMList[i];
		}
		
		// 2.按照保护测量值号转换数值为浮点格式
		if(tRMDisp.wAttr == RM_ATTR_HEX)
			dwFDData = pAlarm->FD_Value[i];
		else 
		{
//			if(DB_IsFDShowFirst())
//				rc = ConvertRMFix2FirstFloat(tRelayInfDisp.wRMList[i],pAlarm->FD_Value[i],(float *)&dwFDData);
//			else
				rc = ConvertRMFix2SecondFloat(tRelayInfDisp.wRMList[i],pAlarm->FD_Value[i],(float *)&dwFDData);
		}

		if(rc != DB_ERR_OK)
		{
			LogError("DB_RelayAlarm",FILE_LINE,"FD Error!!!");
			return DB_ERR_RELAY_FD;
		}

		pFDData[0] = LOBYTE(wFDIndex);
		pFDData[1] = HIBYTE(wFDIndex);
		pFDData[2] = LLBYTE(dwFDData);
		pFDData[3] = LHBYTE(dwFDData);
		pFDData[4] = HLBYTE(dwFDData);
		pFDData[5] = HHBYTE(dwFDData);

		pFDData += 6;
	}

	// 6。写入报告
	return DB_WriteRelayAlarmReport(&tAlarmEv);
}

/************************************************************
	功能：产生保护事项报告
	参数：	wInf		--保护动作索引编号
		 	*pTime		--启动时间，空指针时取系统当前时间
			pFDList		--故障电量列表
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_MakeRelayEventReport(WORD wInf, TAbsTime *pTime,long *pFDList)
{
	TRE_Disp 			tRelayInfDisp;
	TRelayStartEv		tStartEv;
	TAbsTime			tEventTime;
	TRM_Disp			tRMDisp;
	TRelayEventHead		*pEvent;
	TRelayRecordHead	*pRecord;
	BYTE				*pFDData;
	STATUS				rc;
	WORD				wLength;
	WORD				wFDIndex;
	DWORD				dwFDData;

	// 1。读取INF信息
	rc = DB_GetREDispByInf(wInf,&tRelayInfDisp);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_MakeRelayEventReport",FILE_LINE,"InValid INF %d!!!",wInf);
		return DB_ERR_RELAY_INF;
	}

	// 3。检查报告长度不能越限
	wLength = sizeof(TRelayRecordHead)+tRelayInfDisp.nFD*(sizeof(WORD)+sizeof(float));
	ASSERT(wLength<=MAX_RSTT_LENGTH);
	if(wLength > MAX_RSTT_LENGTH)
	{
		LogError("DB_MakeRelayEventReport",FILE_LINE,"Relay event too long!!!");
		return DB_ERR_RELAY_OVERFLOW;
	}
	
	// 4。填充报文头
	pRecord = &tStartEv.tEvent;
	pRecord->Num = 1;
	pRecord->wFaultNo = 0x33CC;
	
	pEvent = &pRecord->tAction;
	pEvent->bStatus = 1;
	pEvent->nINF 	= wInf;
	pEvent->FD_Num 	= tRelayInfDisp.nFD;

	if(pTime != NULL)
		tEventTime = *pTime;
	else
		ReadAbsTime(&tEventTime);
		
	pEvent->AbsTimeHi	= tEventTime.Hi;
	pEvent->AbsTimeLo	= tEventTime.Lo;
		
	// 5。写入故障电量
	pFDData = tStartEv.aEvent+sizeof(TRelayRecordHead);
	
	for(int i=0;i<tRelayInfDisp.nFD;i++)
	{
		if(DB_GetRMDisp(tRelayInfDisp.wRMList[i], &tRMDisp) == DB_ERR_OK)
			wFDIndex = tRMDisp.nIndex;
		else
		{
			LogError("DB_MakeRelayEventReport",FILE_LINE,"DB_GetRMDisp(%d) is error!!!",tRelayInfDisp.wRMList[i]);
			wFDIndex = tRelayInfDisp.wRMList[i];
		}

		rc = ConvertRMFix2SecondFloat(tRelayInfDisp.wRMList[i],pFDList[i],(float *)&dwFDData);
		if(rc != DB_ERR_OK)
		{
			LogError("DB_MakeRelayEventReport",FILE_LINE,"FD Error!!!");
			return DB_ERR_RELAY_FD;
		}

		pFDData[0] = LOBYTE(wFDIndex);
		pFDData[1] = HIBYTE(wFDIndex);
		pFDData[2] = LLBYTE(dwFDData);
		pFDData[3] = LHBYTE(dwFDData);
		pFDData[4] = HLBYTE(dwFDData);
		pFDData[5] = HHBYTE(dwFDData);

		pFDData += 6;
	}

	// 6。写入报告
	return DB_WriteRelayStartReport(&tStartEv);
}

/************************************************************
	功能：保护动作定时存盘
	参数：无
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_SaveRelayActionOnTime()
{
	TRelayEventHead		*pEvent;
	TRelayStartEv		tStartEv;
	TRelayActionEv		tActionEv;
	TRelayRecordHead	*pRecord;
    CNode 				*pCacheNode;
	TRelayEventCache	*pCache;
	WORD				wItemLen,wOffset,wStartInf;
	int					i;
	
	// 1。寻找待存盘的CACHE报告
	do{
		
		pCacheNode = DB_GetEndedRelayCache();
		if(pCacheNode == NULL)
			break;
		
		pCache = (TRelayEventCache *)pCacheNode->pData;
		ASSERT(pCache != NULL);
		if(pCache == NULL)
		{
			LogError("DB_SaveRelayActionOnTime",FILE_LINE,"Cache Is Empty!!!");
			break;
		}
		
		// 3。写入报告
		// 3。1 写入启动报告
		
		if(pCache->bStatus & RELAY_CACHE_MAKESTART)
		{
			if((pCache->nCnt <= 1)&&(pCache->pAction->stHead.FD_Num == 0))
			{
				//如果只有一个启动报告，而且没有故障电量，不再存盘
			}
			else
			{
				pRecord = &tStartEv.tEvent;
				pRecord->Num = 1;
				pRecord->wFaultNo = g_wRelayDistNo;
				
				//组织报文数据
				pEvent = &pRecord->tAction;

				//报告长度不能越限
				wItemLen = sizeof(TRelayRecordHead)+pCache->pAction[0].stHead.FD_Num*(sizeof(WORD)+sizeof(float));
				ASSERT(wItemLen<=MAX_RSTT_LENGTH);
				if(wItemLen<=MAX_RSTT_LENGTH)
					DB_RelayCache2Event(pCache->pAction, pEvent);
				else
				{
					LogError("DB_SaveRelayActionOnTime",FILE_LINE,"START REPORT too long!!!");
				}
				
				if(DB_WriteRelayStartReport(&tStartEv,NULL) != DB_ERR_OK)
				{
					LogError("DB_SaveRelayActionOnTime",FILE_LINE,"Save START REPORT error!!!");
				}
			}
		}
		
		// 3。2 写入动作报告
		if(pCache->nCnt > 1)
		{
			pRecord = &tActionEv.tEvent;
			pEvent	= &pRecord->tAction;
			
			pRecord->Num = 0;
			wOffset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
			
			for(i=0;i<pCache->nCnt;i++)
			{
				wStartInf = pCache->pAction[i].stHead.nINF;
				if(wStartInf == RELAY_INF_NOUSED)
					continue;
				
				// 3。2。1 计算分报告长度
				wItemLen = sizeof(TRelayEventHead)+pCache->pAction[i].stHead.FD_Num*(sizeof(WORD)+sizeof(float));

				// 3。2。2 累计分报告之前进行长度判断
				if((wOffset + wItemLen) > (MAX_RACT_LENGTH-2))
				{
					// 1。先存一个报告
					pRecord->wFaultNo = g_wRelayDistNo;
					DB_WriteRelayActionReport(&tActionEv,NULL);
					g_wRelayDistNo = (g_wRelayDistNo+1)%10000;

					// 3。重新填充一个启动报告
					memset(&tActionEv,0,sizeof(TRelayActionEv));
					pEvent	= &pRecord->tAction;
					
//					if(pCache->pAction[0].stHead.nINF != RELAY_INF_NOUSED)
//					{
//						DB_RelayCache2Event(&pCache->pAction[0], pEvent);
//						wOffset = sizeof(TRelayRecordHead)+pCache->pAction[0].stHead.FD_Num*(sizeof(WORD)+sizeof(float));

//						// 4。更新填充指针
//						pEvent = (TRelayEventHead *)(tActionEv.aEvent+wOffset);
//						pRecord->Num = 1;
//					}
//					else
					{
						pRecord->Num = 0;
						wOffset = sizeof(TRelayRecordHead)-sizeof(TRelayEventHead);
					}
				}
				
				// 依次添加后续分报告
				DB_RelayCache2Event(&pCache->pAction[i], pEvent);
				wOffset += wItemLen;
				
				pRecord->Num++;
				pEvent = (TRelayEventHead *)(tActionEv.aEvent+wOffset);
			}

			pRecord->wFaultNo = g_wRelayDistNo;
			DB_WriteRelayActionReport(&tActionEv, NULL);
			g_wRelayDistNo = (g_wRelayDistNo+1)%10000;
		}
		
		OS_Protect(0);

		// 4。释放存储链表
		G_RelayRepSaveLink.RemoveNode(pCacheNode);
		G_RelayRepFreeLink.AddTail(pCacheNode);
		
		pCache->bStatus &= ~RELAY_CACHE_STATUSMASK;
		pCache->bStatus = RELAY_CACHE_READY;
		pCache->dwTimeOut= 0;

//		LogError("DB_SaveRelayActionOnTime",FILE_LINE,"<------INF:%d Num:%d  FreeNodeNum=%x SaveNodeNum=%x------>",tActionEv.tEvent.wFaultNo,pCache->nCnt,\
	//		(DWORD)G_RelayRepFreeLink.GetNodeNum(),(DWORD)G_RelayRepSaveLink.GetNodeNum());

		OS_UnProtect(0);
	
	}while(pCacheNode != NULL);
	
	return DB_ERR_OK;
}
/************************************************************
	功能：从设备库中读取保护告警报文
	参数： *pReport 	--  读出的保护告警信息
			rp    		--  外部读指针（维护用），一般任务用BLANK
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_ReadRelayAlarmReport(TRelayAlarmEv *pReport, WORD rp)
{
    TRelayAlarmWrap 	*pWrap;
	TAppInfo			*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( pReport==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nRALMRead%MAX_RALM_NUM;
    }
    else
    {
		rp = rp%MAX_RALM_NUM;
    }
                       
	// 4。读取事项信息
	*pReport = pWrap->pEvent[rp];

	// 检测报告校验，Modify by lz. 2013.10.08
	if(!DAE_IsCheckSumOK(pReport->aEvent,MAX_RALM_LENGTH-2))
		return DB_ERR_CHECK;
		
	// 显示一次故障电量情况下，需要转换
	if(DB_IsFDShowFirst())
		ConvertRelayEvent2FirstData(&pReport->tEvent);

	// 5。事项读取完毕直接清除事项标志
    DB_ClearEventFlag(pAppInfo,APP_EVENT_RALM);
                                     
	return DB_ERR_OK;
}

/************************************************************
	功能：向设备库中写入保护告警报文
	参数： *pReport 	--  待写入的保护告警信息                      
			pTime 		--  保护告警发生的时间。当为空时，使用系统时间。
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_WriteRelayAlarmReport(TRelayAlarmEv *pReport,TAbsTime *pTime)
{
    TRelayAlarmWrap 	*pWrap;
    TAppInfo 			*pAppInfo;
    WORD 				i, wWritePtr;
                     
	// 1。检查事项指针的合法性
	if( pReport == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 计算报告校验，Modify by lz. 2013.10.08
	DAE_MakeCheckSum(pReport->aEvent,MAX_RALM_LENGTH-2);
	
	// 3。存数据
	wWritePtr = pWrap->nTailIndex;

	OS_Protect(0);
	
	pWrap->pEvent[wWritePtr] = *pReport;
    pWrap->nTailIndex = (wWritePtr+1)%MAX_RALM_NUM;
	
	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RALM_NUM;

	OS_UnProtect(0);
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	
	for(i=0;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
	{	
		if(!(pAppInfo->bMask & APP_EVENT_RALM))
		{
			pAppInfo->nRALMRead = pWrap->nTailIndex;
			continue;
		}

		if( pAppInfo->nRALMRead == pWrap->nTailIndex )
			 pAppInfo->nRALMRead = (pWrap->nTailIndex+1)%MAX_RALM_NUM;

		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_RALM))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_RALM);

			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_RELAYEVENT,2,wWritePtr,0,0);
		}
	}

	DB_RegisterFileChange(DB_FILE_RELAYEV);

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护告警报文库中未读取报文的数目
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayAlarmReportNum(WORD *retval)
{
	TRelayAlarmWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3。获取读指针    
    rc = DB_GetRelayAlarmReportRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_RALM_NUM-rp)%MAX_RALM_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：获取保护告警报文库的读指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayAlarmReportRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    *retval = pAppInfo->nRALMRead%MAX_RALM_NUM;

	return DB_ERR_OK;
}
/************************************************************
	功能：获取保护告警信息缓冲区的头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayAlarmReportHead(WORD *retval)
{
	TRelayAlarmWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RALM_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护告警信息缓冲区的尾指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayAlarmReportTail(WORD *retval)
{
	TRelayAlarmWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RALM_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增保护告警信息缓冲区的头指针
	参数：无
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_IncRelayAlarmReportRead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nRALMRead = (pAppInfo->nRALMRead+1)%MAX_RALM_NUM;

	//DB_RegisterFileChange(DB_FILE_RELAYEV);
		
	return DB_ERR_OK;
}

/************************************************************
	功能：从设备库中读取保护启动报文
	参数： *pReport 	--  读出的保护启动信息
			rp    		--  外部读指针（维护用），一般任务用BLANK
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_ReadRelayStartReport(TRelayStartEv *pReport, WORD rp)
{
    TRelayStartWrap 	*pWrap;
	TAppInfo			*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( pReport==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nRSTTRead%MAX_RSTT_NUM;
    }
    else
    {
		rp = rp%MAX_RSTT_NUM;
    }
                       
	// 4。读取事项信息
	*pReport = pWrap->pEvent[rp];

	
	// 显示一次故障电量情况下，需要转换
	if(DB_IsFDShowFirst())
		ConvertRelayEvent2FirstData(&pReport->tEvent);

	// 5。事项读取完毕直接清除事项标志
    DB_ClearEventFlag(pAppInfo,APP_EVENT_RSTT);
                                     
	return DB_ERR_OK;
}

/************************************************************
	功能：向设备库中写入保护启动报文
	参数： *pReport 	--  读出的保护启动信息                     
			pTime 		--  保护启动发生的时间，此项用于产生虚拟遥信变化事件用。当为空时，使用系统时间。
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_WriteRelayStartReport(TRelayStartEv *pReport, TAbsTime *pTime)
{
    TRelayStartWrap 	*pWrap;
    TAppInfo 			*pAppInfo;
    WORD 				i, wWritePtr;
                     
	// 1。检查事项指针的合法性
	if( pReport == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3。存数据
	wWritePtr = pWrap->nTailIndex;

	OS_Protect(0);
	
    pWrap->pEvent[wWritePtr] = *pReport;
    pWrap->nTailIndex = (wWritePtr+1)%MAX_RSTT_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RSTT_NUM;

	OS_UnProtect(0);
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_RSTT))
		{
			pAppInfo->nRSTTRead = pWrap->nTailIndex;
			continue;
		}
		
	    if( pAppInfo->nRSTTRead == pWrap->nTailIndex )
	         pAppInfo->nRSTTRead = (pWrap->nTailIndex+1)%MAX_RSTT_NUM;

		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_RSTT))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_RSTT);
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_RELAYEVENT, 0,wWritePtr,0,0);
		}
	}

//	DB_RegisterFileChange(DB_FILE_BASICEV);

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护启动报文库中未读取报文的数目
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayStartReportNum(WORD *retval)
{
	TRelayStartWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3。获取读指针    
    rc = DB_GetRelayStartReportRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_RSTT_NUM-rp)%MAX_RSTT_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：获取保护启动报文库的读指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayStartReportRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    *retval = pAppInfo->nRSTTRead%MAX_RSTT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护启动信息缓冲区的头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayStartReportHead(WORD *retval)
{
	TRelayStartWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RSTT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护启动信息缓冲区的头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayStartReportTail(WORD *retval)
{
	TRelayStartWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RSTT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增保护启动信息缓冲区的头指针
	参数：无
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_IncRelayStartReportRead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nRSTTRead = (pAppInfo->nRSTTRead+1)%MAX_RSTT_NUM;

	// 3。存储指针
	//DB_RegisterFileChange(DB_FILE_BASICEV);
	
	return DB_ERR_OK;
}


/************************************************************
	功能：从设备库中读取保护告警报文
	参数：	*pReport 	--  读出的保护动作信息
			rp    		--  外部读指针（维护用），一般任务用BLANK
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_ReadRelayActionReport(TRelayActionEv *pReport, WORD rp)
{
    TRelayActionWrap 	*pWrap;
	TAppInfo			*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( pReport==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nRACTRead%MAX_RACT_NUM;
    }
    else
    {
		rp = rp%MAX_RACT_NUM;
    }
                       
	// 4。读取事项信息
	*pReport = pWrap->pEvent[rp];

	// 检测报告校验，Modify by lz. 2013.10.08
	if(!DAE_IsCheckSumOK(pReport->aEvent,MAX_RACT_LENGTH-2))
		return DB_ERR_CHECK;
	
	// 显示一次故障电量情况下，需要转换
	if(DB_IsFDShowFirst())
		ConvertRelayEvent2FirstData(&pReport->tEvent);

	// 5。事项读取完毕直接清除事项标志
    DB_ClearEventFlag(pAppInfo,APP_EVENT_RACT);
									 
	return DB_ERR_OK;
}

/************************************************************
	功能：向设备库中写入保护动作报文
	参数： *pReport 	--  待写入的保护动作信息                      
			pTime 		--  保护动作发生的时间，此项用于产生虚拟遥信变化事件用。当为空时，使用系统时间。
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_WriteRelayActionReport(TRelayActionEv *pReport, TAbsTime *pTime)
{
    TRelayActionWrap 	*pWrap;
    TAppInfo 			*pAppInfo;
    WORD 				i, wWritePtr;
                     
	// 1。检查事项指针的合法性
	if( pReport == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 计算报告校验，Modify by lz. 2013.10.08
	DAE_MakeCheckSum(pReport->aEvent,MAX_RACT_LENGTH-2);

	// 3。存数据
	wWritePtr = pWrap->nTailIndex;
	
	OS_Protect(0);
	
    pWrap->pEvent[wWritePtr] = *pReport;
    pWrap->nTailIndex = (wWritePtr+1)%MAX_RACT_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_RACT_NUM;

	OS_UnProtect(0);
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++,pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_RACT))
		{
			pAppInfo->nRACTRead = pWrap->nTailIndex;
			continue;
		}
		
	    if( pAppInfo->nRACTRead == pWrap->nTailIndex )
	         pAppInfo->nRACTRead = (pWrap->nTailIndex+1)%MAX_RACT_NUM;

//		if(!DB_GetEventFlag(&pAppInfo[i], APP_EVENT_RACT))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_RACT);
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_RELAYEVENT,1,wWritePtr,0,0);
		}
	}

	DB_RegisterFileChange(DB_FILE_RELAYEV);

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护动作报文库中未读取报文的数目
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayActionReportNum(WORD *retval)
{
	TRelayActionWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3。获取读指针    
    rc = DB_GetRelayActionReportRead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_RACT_NUM-rp)%MAX_RACT_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：获取保护动作报文库的读指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayActionReportRead(WORD *retval)
{
	TAppInfo	*pAppInfo;

	ASSERT(retval != NULL);
	if(retval == NULL)
		return DB_ERR_BUFNULL;
	
    // 取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
    // 返回任务事项指针
    *retval = pAppInfo->nRACTRead%MAX_RACT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护动作信息缓冲区的头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayActionReportHead(WORD *retval)
{
	TRelayActionWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_RACT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：获取保护动作信息缓冲区的尾指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetRelayActionReportTail(WORD *retval)
{
	TRelayActionWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_RACT_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增保护动作信息缓冲区的头指针
	参数：无
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_IncRelayActionReportRead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nRACTRead = (pAppInfo->nRACTRead+1)%MAX_RACT_NUM;

	// 3。存储指针
	//DB_RegisterFileChange(DB_FILE_RELAYEV);
		
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取软压板个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetSWNum()
{
	return G_pCfgTable->DBaseConfig.nSW;
}
		
/************************************************************
	功能：从数据库中取一个软压板状态
	参数：no,    		--  软压板序号
		  Status	    --  软压板状态	
						投：SW_STATUS_SET，切：SW_STATUS_CUT
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSWStatus(WORD no, BYTE &Status)
{
	DWORD 			*pSW;
    WORD 			num;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
               
	pSW =  G_pCfgTable->DBaseConfig.pSWData;
	num  = G_pCfgTable->DBaseConfig.nSW;
	
	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_GetSWStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSW == NULL )
    {
        LogError("DB_GetSWStatus",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	if(pSW[DwordNo] & BitSign)
		Status = SW_STATUS_SET;
	else
		Status = SW_STATUS_CUT;
		
	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个软压板状态
	参数：no,    		--  软压板序号
		  Status	    --  软压板状态	
						投：CFG_STATUS_SET，切：CFG_STATUS_CLR
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetSWStatus( WORD no, BYTE Status )
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
               
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;
	
	// 1。检查序号
	if(no >= num)
	{
		LogError("DB_SetSWStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSWOnline == NULL )
    {
        LogError("DB_SetSWStatus",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 2。写入在线修改区
	for(int i=0;i<2*((num+31)/32);i++)
		pSWOnline[i] = G_pCfgTable->DBaseConfig.pSWData[i];
	
	if(Status == SW_STATUS_SET)
	{
	 	pSWOnline[DwordNo] |= BitSign;
	}
	else
	{
	 	pSWOnline[DwordNo] &= ~BitSign;
	}
	
	// 3。注册修改软压板操作
	pSWOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSWOpt->dwOption1 	= no;
	pSWOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSWOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个软压板状态确认
	参数：no,    		--  软压板序号
			Attr		--  操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetSWStatusConfirm( WORD no ,WORD Attr)
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSW;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
    STATUS			rc;
	
	pSW 		= G_pCfgTable->DBaseConfig.pSWData;
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;

	// 1。检查操作合法性
	if(pSWOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSWOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pSWOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;
	
	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_SetSWStatusConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pSW == NULL)||(pSWOnline == NULL))
    {
        LogError("DB_SetSWStatusConfirm",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

	if(Attr == DB_CANCEL)
	{
		pSWOpt->wTimeCnt	= 0;
		pSWOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	
    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 3。根据在线修改区的结果写入软压板数据
	if(pSWOnline[DwordNo] & BitSign)
	{
	 	pSW[DwordNo] |= BitSign;
	 	pSW[DwordNo+(num+31)/32] &= ~BitSign;
	}
	else
	{
	 	pSW[DwordNo] &= ~BitSign;
	 	pSW[DwordNo+(num+31)/32] |= BitSign;
	}

	// 4。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_SW,no);
	DAE_ClearRunningFlags(SS_EEPROM_SW_ERR); 	
	
	// 5。注册修改软压板操作
	pSWOpt->wTimeCnt 	= 0;
	pSWOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, no, 0, 0);

#if(MODUEL_TYPE == MT_KF6571)
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL), SM_RELAYDATA, MISC_CLASS_SW, no, 0, 0);
#endif
	
	if(pSWOnline[DwordNo] & BitSign)
		DB_Register_SW(no,SW_STATUS_SET);
	else
		DB_Register_SW(no,SW_STATUS_CUT);
		
	return rc;
}

/************************************************************
	功能：向数据库中写入全部软压板状态
	参数：no,    		--  软压板序号
		  Status	    --  软压板状态表	
						投：CFG_STATUS_SET，切：CFG_STATUS_CLR
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetAllSWStatus(PBYTE Status)
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
               
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;
	
    if( pSWOnline == NULL )
    {
        LogError("DB_SetSWStatus",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

	// 逐个状态写入
	for(int i=0;i<num;i++)
	{
		DwordNo 	= i/32;
		BitSign 	= (0x01<<(i%32));
		
		if(Status[i/8]&(0x01<<(i%8)))
		{
			pSWOnline[DwordNo] |= BitSign;
		}
		else
		{
			pSWOnline[DwordNo] &= ~BitSign;
		}
	}
	
	// 3。注册修改软压板操作
	pSWOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSWOpt->dwOption1 	= SET_VALUE_ALL;
	pSWOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSWOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入全部软压板状态确认
	参数：Attr		--  操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetAllSWStatusConfirm(WORD Attr)
{
	TRunTimeOPT		*pSWOpt;
	DWORD 			*pSW;
	DWORD 			*pSWOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
    STATUS			rc;
	
	pSW 		= G_pCfgTable->DBaseConfig.pSWData;
	pSWOnline 	= G_pCfgTable->DBaseConfig.tModData.pSWData;
	pSWOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nSW;

	// 1。检查操作合法性
	if(pSWOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSWOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pSWOpt->dwOption1 != SET_VALUE_ALL)
		return DB_ERR_OPT_PARA;
	
	// 2。检查序号
    if((pSW == NULL)||(pSWOnline == NULL))
    {
        LogError("DB_SetSWStatusConfirm",FILE_LINE,"The SW base is null!");
        return DB_ERR_BASENULL;
    }

	if(Attr == DB_CANCEL)
	{
		pSWOpt->wTimeCnt	= 0;
		pSWOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 保存正反码
	memcpy(pSW,pSWOnline,sizeof(DWORD)*((num+31)/32));
	for(int i=0;i<((num+31)/32);i++)
		pSW[i+((num+31)/32)] = ~pSW[i];
		
	// 4。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_SW,SET_VALUE_ALL);
	DAE_ClearRunningFlags(SS_EEPROM_SW_ERR); 	
	
	// 5。注册修改软压板操作
	pSWOpt->wTimeCnt 	= 0;
	pSWOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, SET_VALUE_ALL, 0, 0);
	
	DB_Register_SW(SET_VALUE_ALL,0);
		
	return rc;
}

/************************************************************
	功能：从数据库中取配置字个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetCFGNum()
{
	return G_pCfgTable->DBaseConfig.nCFG;
}

/************************************************************
	功能：从数据库中取配置字信息
	参数：AreaNo,    	--  配置字区号
		  dwCFGWord	    --  配置字列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCFGWord(WORD AreaNo,DWORD *dwCFGWord)
{
    WORD 			num,i;
	STATUS			rc;
	
	num  = G_pCfgTable->DBaseConfig.nCFG;

	for(i=0;i<(num+31)/32;i++)	
	{
		rc = DB_GetSETFloatValue(AreaNo,i,(float *)&dwCFGWord[i]);
		if(rc != DB_ERR_OK)
		{
			LogError("DB_GetCFGStatus",FILE_LINE,"Read CFG Word Error!");
			return rc;
		}
	}
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入配置字
	参数：AreaNo,    	--  配置字区号
		  dwCFGWord	    --  配置字列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCFGWord(WORD AreaNo, DWORD *dwCFGWord)
{
	TRunTimeOPT				*pSETOpt;
	TDBaseConfigTable		*pDbase;
	DWORD 					*pSET;
	DWORD 					*pSETMap;
	WORD					num;
	int 					i;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	num  	= (pDbase->nCFG+31)/32;
	pSET 	= (DWORD *)pDbase->tModData.pSETData;
	pSETOpt	= pDbase->pSETOpt;
	
	// 1。检查数据指针
	ASSERT(dwCFGWord != NULL);
	if(dwCFGWord == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pSET != NULL);
	ASSERT(pSETOpt != NULL);
	if((pSET == NULL)||(pSETOpt == NULL))
		return DB_ERR_BASENULL;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 2。检查定值区号
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetCFGWord",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 3。写入在线修改区	
	if(pDbase->tMapData.pSETData == NULL)
		return DB_ERR_BASENULL;
	
	pSETMap	= (DWORD *)&pDbase->tMapData.pSETData[AreaNo*(num+1)];

	// 3。1 先更新在线修改数据
	for(i=0;i<num;i++)
	 	pSET[i] = ltobl(pSETMap[i]);

	// 3。2 再写入要修改的数据
	for(i=0;i<num;i++)
		pSET[i] = dwCFGWord[i];

	// 4。注册修改配置字操作
	pSETOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSETOpt->dwOption1 	= AreaNo;
	pSETOpt->dwOption2 	= SET_VALUE_ALL;
	pSETOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSETOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个完整的配置字确认
	参数：AreaNo,    	--  配置字区号
			Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCFGWordConfirm(WORD AreaNo,WORD Attr )
{
	TDBaseConfigTable		*pDbase;				//数据库配置表
	TRunTimeOPT				*pSETOpt;
	DWORD 					*pSET;
    WORD 					CFGNum;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= (DWORD *)pDbase->tModData.pSETData;
	pSETOpt	= pDbase->pSETOpt;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1。检查操作合法性
	if(pSETOpt->dwAppID != RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETOpt->wStatus != OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if((pSETOpt->dwOption1 != AreaNo)||(pSETOpt->dwOption2 != SET_VALUE_ALL))
		return DB_ERR_OPT_PARA;

	// 2。检查数据指针
	ASSERT(pSET != NULL);
	if(pSET == NULL)
		return DB_ERR_BASENULL;

	// 3。检查定值区号
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetCFGWordConfirm",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETOpt->wTimeCnt	= 0;
		pSETOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 5。如果是当前定值区，先写入RAM
	CFGNum = (pDbase->nCFG+31)/32;
	
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		for(int i=0;i<CFGNum;i++)
		{
			pDbase->pSETDataFloat[i] = *(float*)(pSET+i);
			pDbase->pSETDataFix[i] = pSET[i];
		}
	}

	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFloat, sizeof(float)*pDbase->nSET);
	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFix, sizeof(long)*pDbase->nSET);

	// 4。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_SET,SET_VALUE_ALL);

	// 5。注册修改软压板操作
	pSETOpt->wTimeCnt 	= 0;
	pSETOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(SET_VALUE_ALL,AreaNo), 0, 0);

	DB_Register_SET(AreaNo,SET_VALUE_ALL);
	
	return rc;
}

/************************************************************
	功能：从数据库中取一个配置字位状态
	参数：no,    		--  配置字位序号
		  Status	    --  配置字位状态	
							投：CFG_STATUS_SET，切：CFG_STATUS_CLR
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCFGStatus(WORD AreaNo,BYTE no, BYTE &Status)
{
	DWORD 			dwCFGWord;
	DWORD 			BitSign; 	
	WORD 			DwordNo; 	
    WORD 			num;
	STATUS			rc;
	
	num  = G_pCfgTable->DBaseConfig.nCFG;

	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_GetCFGStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	rc = DB_GetSETFloatValue(AreaNo,DwordNo,(float *)&dwCFGWord);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_GetCFGStatus",FILE_LINE,"Read CFG Word Error!",no);
		return rc;
	}
	
	if(dwCFGWord & BitSign)
		Status = CFG_STATUS_SET;
	else
		Status = CFG_STATUS_CLR;
		
	return DB_ERR_OK;
}
/************************************************************
	功能：向数据库中写入一个配置字位状态
	参数：no,    		--  配置字位序号
		  Status	    --  配置字位状态	
							投：CFG_STATUS_SET，切：CFG_STATUS_CLR
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCFGStatus(WORD AreaNo, WORD no, BYTE Status)
{
	TRunTimeOPT		*pCFGOpt;		
	DWORD 			dwCFGWord;
	DWORD 			BitSign; 	
	WORD 			DwordNo; 	
    WORD 			num;
    STATUS			rc;

	pCFGOpt		= G_pCfgTable->DBaseConfig.pCFGOpt;
	num			= G_pCfgTable->DBaseConfig.nCFG;
	
	// 1。检查序号
	if(no >= num)
	{
		LogError("DB_SetCFGStatus",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	rc = DB_GetSETFloatValue(AreaNo,DwordNo,(float *)&dwCFGWord);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_SetCFGStatus",FILE_LINE,"Read CFG Word Error!",no);
		return rc;
	}
	
	if(Status == CFG_STATUS_SET)
	 	dwCFGWord |= BitSign;
	else
	 	dwCFGWord &= ~BitSign;

	// 3。注册修改配置字操作
	pCFGOpt->dwAppID 	= RtosGetCurrentTaskId();
	pCFGOpt->dwOption1 	= no;
	pCFGOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pCFGOpt->wStatus 	= OPT_STATUS_SET;

	return DB_SetSETValue(AreaNo, DwordNo, (float *)&dwCFGWord);

/*	
	TRunTimeOPT		*pCFGOpt;
	DWORD 			*pCFGOnline;
	
	pCFGOnline 	= G_pCfgTable->DBaseConfig.tModData.pCFGData;
	pCFGOpt		= G_pCfgTable->DBaseConfig.pSWOpt;
	
    if( pCFGOnline == NULL )
    {
        LogError("DB_SetCFGStatus",FILE_LINE,"The CFG base is null!");
        return DB_ERR_BASENULL;
    }
    
	// 2。写入在线修改区
	for(int i=0;i<(num+31)/32;i++)
		pCFGOnline[i] = G_pCfgTable->DBaseConfig.pCFGData;
	
	if(Status == CFG_STATUS_SET)
	 	pCFGOnline[DwordNo] |= BitSign;
	else
	 	pCFGOnline[DwordNo] &= ~BitSign;

	// 3。注册修改配置字操作
	pCFGOpt->dwAppID 	= RtosGetCurrentTaskId();
	pCFGOpt->dwOption1 	= no;
	pCFGOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pCFGOpt->wStatus 	= OPT_STATUS_SET;
*/
}

/************************************************************
	功能：向数据库中写入一个配置字位状态确认
	参数：no,    		--  配置字位序号
			Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCFGStatusConfirm(WORD AreaNo,WORD no,WORD Attr)
{
	TRunTimeOPT		*pCFGOpt;
	DWORD 			dwCFGWord;
	DWORD 			BitSign; 	
	WORD 			DwordNo; 	
    WORD 			num;
    STATUS			rc;
	
	pCFGOpt	 	= G_pCfgTable->DBaseConfig.pCFGOpt;
	num  		= G_pCfgTable->DBaseConfig.nCFG;
	// 1。检查操作合法性
	if(pCFGOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pCFGOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pCFGOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;

	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_SetCFGStatusConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pCFGOpt->wTimeCnt	= 0;
		pCFGOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 3。注册修改配置字操作
	pCFGOpt->wTimeCnt 	= 0;
	pCFGOpt->wStatus 	= OPT_STATUS_READY;

	rc = DB_SetSETValueConfirm(AreaNo, DwordNo,Attr);
	if(rc != DB_ERR_OK)
		return rc;
	
	dwCFGWord = *(DWORD *)&G_pCfgTable->DBaseConfig.tModData.pSETData[DwordNo];

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_CFG, no, 0, 0);

	if(dwCFGWord & BitSign)
		DB_Register_CFG(no,CFG_STATUS_SET);
	else
		DB_Register_CFG(no,CFG_STATUS_CLR);

	return rc;
/*	
	TRunTimeOPT		*pCFGOpt;
	DWORD 			*pCFG;
	DWORD 			*pCFGOnline;
	DWORD 			DwordNo; 	
	DWORD 			BitSign; 	
    WORD 			num;
               
	pCFG		= G_pCfgTable->DBaseConfig.pCFGData;
	pCFGOnline 	= G_pCfgTable->DBaseConfig.tModData.pCFGData;
	pCFGOpt	 	= G_pCfgTable->DBaseConfig.pSWOpt;
	num  		= G_pCfgTable->DBaseConfig.nCFG;
	
	// 1。检查操作合法性
	if(pCFGOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pCFGOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pCFGOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;

	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_SetCFGStatusConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pCFG == NULL)||(pCFGOnline == NULL))
    {
        LogError("DB_SetCFGStatusConfirm",FILE_LINE,"The CFG base is null!");
        return DB_ERR_BASENULL;
    }

    DwordNo 	= no/32;
	BitSign  	= (0x01<<(no%32));

	// 3。根据在线修改区的结果写入软压板数据
	if(pCFGOnline[DwordNo] & BitSign)
	{
	 	pCFG[DwordNo] |= BitSign;
	}
	else
	{
	 	pCFG[DwordNo] &= ~BitSign;
	}

	// 4。保存保护数据文件
	DB_ModifyDataFile(DB_DATA_CFG,no);

	// 5。注册修改软压板操作
	pCFGOpt->wTimeCnt 	= 0;
	pCFGOpt->wStatus 	= OPT_STATUS_READY;

	if(pCFGOnline[DwordNo] & BitSign)
		DB_Register_CFG(no,CFG_STATUS_SET);
	else
		DB_Register_CFG(no,CFG_STATUS_CLR);
	
	return DB_ERR_OK;
*/	
}

/************************************************************
	功能：从数据库中取定值个数。
	参数：无
	返回：正确返回个数，错误返回0xFFFF。
************************************************************/
WORD	DB_GetSETNum()
{
	return G_pCfgTable->DBaseConfig.nSET;
}

/************************************************************
	功能：从数据库中读取定值的浮点数。
	参数：	AreaNo		--  定值区号。	SET_AREA_CURRENT代表当前定值区，
			no,   		--  定值序号。	SET_VALUE_ALL代表全部定值

			fValue	    --  浮点定值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSETFloatValue( WORD AreaNo,WORD no, float *fValueList)
{
	float 			*pSET;
    WORD 			num;
               
	pSET = G_pCfgTable->DBaseConfig.pSETDataFloat;
	num  = G_pCfgTable->DBaseConfig.nSET;

	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1。检查数据指针
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2。检查定值区号
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_GetSETFloatValue",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 3。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSETFloatValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4。如果是当前定值区，直接从RAM读取,否则从文件中读取
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		if(no == SET_VALUE_ALL)
			memcpy(fValueList,pSET,sizeof(float)*num);
		else
			*fValueList = pSET[no];		
	}
	else
	{
		// 当前定值镜像异常
		if(G_pCfgTable->DBaseConfig.tMapData.pSETData != NULL)
		{
			pSET = &G_pCfgTable->DBaseConfig.tMapData.pSETData[AreaNo*(num+1)];
		}

		if(no == SET_VALUE_ALL)
		{
			for(int i=0;i<num;i++)
				fValueList[i] = CONVERTFLOAT(pSET[i]);
		}
		else
			*fValueList = CONVERTFLOAT(pSET[no]);		
	}
	
	return DB_ERR_OK;
}
	
/************************************************************
	功能：从数据库中读取定值的整点数。
	参数：	AreaNo		--  定值区号。同上。
			no,   		--  定值序号。同上。
		  fValue	    --  定点定值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSETFixValue( WORD AreaNo,WORD no, long *lValueList)
{
	long 			*pSETFix;
	float 			*pSETFloat;
    WORD 			num;
               
	pSETFix = G_pCfgTable->DBaseConfig.pSETDataFix;
	num  = G_pCfgTable->DBaseConfig.nSET;

	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1。检查数据指针
	ASSERT(lValueList != NULL);
	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2。检查定值区号
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_GetSETFixValue",FILE_LINE,"The Area No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	// 3。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSETFixValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4。如果是当前定值区，直接从RAM读取,否则从文件中读取
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		if(no == SET_VALUE_ALL)
			memcpy(lValueList,pSETFix,sizeof(float)*num);
		else
			*lValueList = pSETFix[no];		
	}
	else
	{
		if(G_pCfgTable->DBaseConfig.tMapData.pSETData != NULL)
			pSETFloat = &G_pCfgTable->DBaseConfig.tMapData.pSETData[AreaNo*(num+1)];

		if(no == SET_VALUE_ALL)
		{
			for(int i=0;i<num;i++)
			{
				ConvertSETFloat2Fix(i, CONVERTFLOAT(pSETFloat[i]), &lValueList[i]);
			}
		}
		else
		{
			ConvertSETFloat2Fix(no, CONVERTFLOAT(pSETFloat[no]), lValueList);
		}
	}
	
	return DB_ERR_OK;
}
	
/************************************************************
	功能：向数据库中写入定值
	参数：	AreaNo		--	定值区号。同上。
			no, 		--	定值序号。同上。
			fValue		--	定点定值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetSETValue(WORD AreaNo,WORD no, float *fValueList )
{
	TRunTimeOPT 			*pSETOpt;
	TDBaseConfigTable		*pDbase;				//数据库配置表
	float					*pSET;
	float					*pSETMap;
	TSET_Disp				tSetDisp;
	WORD					num,CFGNum;
	
	pDbase	= &G_pCfgTable->DBaseConfig;	 
	num 	= pDbase->nSET;
	CFGNum	= (pDbase->nCFG+31)/32;
	pSET	= pDbase->tModData.pSETData;
	pSETOpt = pDbase->pSETOpt;
	
	// 1。检查数据指针
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pSET != NULL);
	ASSERT(pSETOpt != NULL);
	if((pSET == NULL)||(pSETOpt == NULL))
		return DB_ERR_BASENULL;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 2。检查定值区号
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetSETValue",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 3。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSETValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4。写入在线修改区	
	if(pDbase->tMapData.pSETData == NULL)
		return DB_ERR_BASENULL;
	
	pSETMap = &pDbase->tMapData.pSETData[AreaNo*(num+1)];

	// 4。1 先更新在线修改数据
	for(int i=0;i<num;i++)
		pSET[i] = CONVERTFLOAT(pSETMap[i]);

	// 4。2 再写入要修改的数据
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
		{
			if(i >= CFGNum)
			if(DB_GetSETDisp(i, &tSetDisp) == DB_ERR_OK)
			{
				if((fValueList[i] - tSetDisp.fMaxValue) > 0.000001)
				{
					LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
						i,fValueList[i],tSetDisp.fMaxValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
				
				if((fValueList[i]-tSetDisp.fMinValue) < -0.000001 )
				{
					LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
						i,fValueList[i],tSetDisp.fMinValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
			}
			pSET[i] = fValueList[i];
		}
	}
	else
	{
		if(no >= CFGNum)
		if(DB_GetSETDisp(no, &tSetDisp) == DB_ERR_OK)
		{
			if((*fValueList - tSetDisp.fMaxValue) > 0.000001)
			{
				LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
					no,*fValueList,tSetDisp.fMaxValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
			
			if((*fValueList - tSetDisp.fMinValue) < -0.000001 )
			{
				LogError("DB_SetSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
					no,*fValueList,tSetDisp.fMinValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
		}
		pSET[no]= *fValueList;		
	}
	
	// 5。注册修改配置字操作
	pSETOpt->dwAppID	= RtosGetCurrentTaskId();
	pSETOpt->dwOption1	= AreaNo;
	pSETOpt->dwOption2	= no;
	pSETOpt->wTimeCnt	= OPT_TIMEOUT_NUM;
	pSETOpt->wStatus	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入定值确认
	参数：	AreaNo   --  定值区号。同上。
			no,   	 --  定值序号。同上。
		Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetSETValueConfirm(WORD AreaNo,WORD no,WORD Attr)
{
	TDBaseConfigTable		*pDbase;				//数据库配置表
	TRunTimeOPT				*pSETOpt;
	float 					*pSET;
    WORD 					num;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= pDbase->tModData.pSETData;
	pSETOpt	= pDbase->pSETOpt;
	num  	= pDbase->nSET;
	
	if(AreaNo == SET_AREA_CURRENT)
		AreaNo = DB_GetCurrentSETAreaNo();
	
	// 1。检查操作合法性
	if(pSETOpt->dwAppID != RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETOpt->wStatus != OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if((pSETOpt->dwOption1 != AreaNo)||(pSETOpt->dwOption2 != no))
		return DB_ERR_OPT_PARA;

	// 2。检查数据指针
	ASSERT(pSET != NULL);
	if(pSET == NULL)
		return DB_ERR_BASENULL;

	// 3。检查定值区号
	if(AreaNo >= DB_GetSETAreaNum())
	{
		LogError("DB_SetSETValueConfirm",FILE_LINE,"The Area No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	// 4。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSETValueConfirm",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETOpt->wTimeCnt	= 0;
		pSETOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 5。如果是当前定值区，先写入RAM
	if(AreaNo == DB_GetCurrentSETAreaNo())
	{
		if(no == SET_VALUE_ALL)
		{
			for(int i=0;i<num;i++)
			{
				pDbase->pSETDataFloat[i] = pSET[i];
				//配置字例外
				ConvertSETFloat2Fix(i,pSET[i],pDbase->pSETDataFix+i);
			}
		}
		else
		{
			pDbase->pSETDataFloat[no] = pSET[no];
			//配置字例外
			ConvertSETFloat2Fix(no,pSET[no],pDbase->pSETDataFix+no);
		}
	}

	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFloat, sizeof(float)*num);
	DAE_MakeCheckSum((BYTE *)pDbase->pSETDataFix, sizeof(long)*num);

	// 4。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_SET,no);
	DAE_ClearRunningFlags(SS_EEPROM_SET_ERR); 	

	// 5。注册修改软压板操作
	pSETOpt->wTimeCnt 	= 0;
	pSETOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(no,AreaNo), 0, 0);

	// 修改定值时通知测距通信任务
	
#if(MODUEL_TYPE == MT_KF6571)
	OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL), SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(no,AreaNo), 0, 0);
#endif

	DB_Register_SET(AreaNo,no);
	
	return rc;
}

/************************************************************
	功能：读取当前定值区号。
	参数：	无	
	返回：正确返回定值区号，错误返回0xFF。
************************************************************/
WORD 	DB_GetSETAreaNum()
{
	return G_pCfgTable->DBaseConfig.nSETArea;
}

/************************************************************
	功能：读取当前定值区号。
	参数：	无	
	返回：正确返回定值区号，错误返回0xFF。
************************************************************/
WORD 	DB_GetCurrentSETAreaNo()
{
	return G_pCfgTable->DBaseConfig.nSETAreaCur;
}

/************************************************************
	功能：设置当前定值区号。
	参数：	AreaNo   --  定值区号。
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCurrentSETAreaNo ( WORD AreaNo)
{
	TRunTimeOPT				*pSETAreaOpt;
	TDBaseConfigTable		*pDbase;				//数据库配置表

	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSETAreaOpt	= pDbase->pSETAreaOpt;
	
	// 1。检查定值区号的合法性
	if( AreaNo >= pDbase->nSETArea)
	{
		LogError("DB_SetCurrentSETAreaNo",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	// 2。修改定值区号
	pDbase->tModData.pRelayCfg->nSETAreaCur = AreaNo;
	pDbase->tModData.pRelayCfg->nSETAreaCurB = ~AreaNo;
	
	// 3。注册在线修改过程
	pSETAreaOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSETAreaOpt->dwOption1 	= AreaNo;
	pSETAreaOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSETAreaOpt->wStatus 	= OPT_STATUS_SET;

	return DB_ERR_OK;
}

/************************************************************
	功能：设置当前定值区号确认
	参数：	AreaNo   --  定值区号。
			Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCurrentSETAreaNoConfirm ( WORD AreaNo,WORD Attr)
{
	TRunTimeOPT				*pSETAreaOpt;
	TDBaseConfigTable		*pDbase;				//数据库配置表
	TRelayDataTable			*pRelayCfg;				//保护配置表
	TMapDataList			*pMap;
	float					*pSETfloat;
	long					*pSETFix;
	WORD					Num;
	STATUS					rc;
	
	pDbase 		= &G_pCfgTable->DBaseConfig;
	pMap 		= &pDbase->tMapData;
	pRelayCfg	= pDbase->tModData.pRelayCfg;
	pSETAreaOpt	= pDbase->pSETAreaOpt;
	pSETfloat	= pDbase->pSETDataFloat;
	pSETFix		= pDbase->pSETDataFix;

	ASSERT(pRelayCfg);
	ASSERT(pSETAreaOpt);
	ASSERT(pSETfloat);
	ASSERT(pSETFix);
	
	// 1。检查操作合法性
	if(pSETAreaOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETAreaOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pSETAreaOpt->dwOption1 != AreaNo)
		return DB_ERR_OPT_PARA;

	// 2。检查区号合法性
	if( AreaNo >= pDbase->nSETArea)
	{
		LogError("DB_SetCurrentSETAreaNoConfirm",FILE_LINE,"The Area No=%d is overflow!",AreaNo);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETAreaOpt->wTimeCnt	= 0;
		pSETAreaOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	

	// 3。先写入RAM
	pDbase->nSETAreaCur		= pRelayCfg->nSETAreaCur;		
	pDbase->nSETAreaCurB 	= pRelayCfg->nSETAreaCurB;		

	// 4。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_SETAREACUR,0);
	DAE_ClearRunningFlags(SS_EEPROM_SETAREA_ERR); 	

	// 5。再更新当前定值
	Num = DB_GetSETNum();
	for(int i=0;i<Num;i++)
	{
		pSETfloat[i] = CONVERTFLOAT(pMap->pSETData[pDbase->nSETAreaCur*(Num+1)+i]);
		ConvertSETFloat2Fix(i,pSETfloat[i],&pSETFix[i]);
	}

	// 重新计算校验
	DAE_MakeCheckSum((BYTE *)pSETfloat,sizeof(float)*Num);
	DAE_MakeCheckSum((BYTE *)pSETFix,sizeof(long)*Num);
	
	// 6。注册修改软压板操作
	pSETAreaOpt->wTimeCnt 	= 0;
	pSETAreaOpt->wStatus 	= OPT_STATUS_READY;
	
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SETAREA, AreaNo, 0, 0);

	// 非测距通信任务切换定值区时通知测距通信任务
	if(MODUEL_TYPE == MT_KF6571)
	{
		if(DAE_GetCurrentTaskPortId() != SYS_PORTID_FL)
			OS_PostMessage(DAE_GetTaskID(SYS_PORTID_FL), SM_RELAYDATA, MISC_CLASS_SETAREA, AreaNo, 0, 0);
	}

	DB_Register_SETArea(AreaNo);

	return rc;	
}

/************************************************************
	功能：从数据库中取辅助定值个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetSystemSETNum()
{
	return G_pCfgTable->DBaseConfig.nSystemSET;
}

/************************************************************
	功能：从数据库中读取辅助定值（浮点数）。
	参数：	no,   		--  定值序号	SET_VALUE_ALL代表全部定值
		    fValue		--  浮点定值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSystemSETFloatValue(WORD no, float *fValueList)
{
	float 			*pSET;
    WORD 			num;
               
	pSET =  G_pCfgTable->DBaseConfig.pSystemSETData;
	num  = G_pCfgTable->DBaseConfig.nSystemSET;
	
	// 1。检查数据指针
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSystemSETFloatValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 3。直接从RAM读取
	if(no == SET_VALUE_ALL)
		memcpy(fValueList,pSET,sizeof(float)*num);
	else
		*fValueList = pSET[no];		
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中读取辅助定值（定点数）。
	参数：	no,   		--  定值序号	SET_VALUE_ALL代表全部定值
		    lValue		--  定点定值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSystemSETFixValue(WORD no, long *lValueList)
{
	float 			*pSETFloat;
    WORD 			num;
               
	pSETFloat =  G_pCfgTable->DBaseConfig.pSystemSETData;
	num  = G_pCfgTable->DBaseConfig.nSystemSET;
	
	// 1。检查数据指针
	ASSERT(lValueList != NULL);
	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetSystemSETFixValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 3。直接从RAM读取
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
		{
			ConvertSystemSETFloat2Fix(i, pSETFloat[i], &lValueList[i]);
		}
	}
	else
	{
		ConvertSystemSETFloat2Fix(no, pSETFloat[no], lValueList);
	}
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入辅助定值。
	参数：no,   	 	--  定值序号。同上。
		  fValue	   	--  定点定值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetSystemSETValue (WORD no, float *fValueList )
{
	TRunTimeOPT				*pSETOpt;
	TDBaseConfigTable		*pDbase;				//数据库配置表
	float 					*pSET;
	float 					*pSETMap;
	TSysSET_Disp			tSetDisp;
    WORD 					num;

	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= pDbase->tModData.pSystemSETData;
	pSETMap	= pDbase->tMapData.pSystemSETData;
	pSETOpt	= pDbase->pCFOpt;
	num  	= pDbase->nSystemSET;
	
	// 1。检查数据指针
	ASSERT(fValueList != NULL);
	if(fValueList == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pSET != NULL);
	ASSERT(pSETMap != NULL);
	ASSERT(pSETOpt != NULL);
	if((pSET == NULL)||(pSETMap == NULL)||(pSETOpt == NULL))
		return DB_ERR_BASENULL;

	// 2。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSystemSETValue",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4。写入在线修改区

	// 4。1 先更新在线修改数据
	for(int i=0;i<num;i++)
	{
	 	pSET[i] = CONVERTFLOAT(pSETMap[i]);
	}
	
	// 4。2 再写入要修改的数据
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
		{
			// 越限检查
			if(DB_GetSysSETDisp(i, &tSetDisp) == DB_ERR_OK)
			{
				if((fValueList[i] - tSetDisp.fMaxValue) > 0.000001)
				{
					LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
						i,fValueList[i],tSetDisp.fMaxValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
				
				if((fValueList[i]-tSetDisp.fMinValue) < -0.000001 )
				{
					LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
						i,fValueList[i],tSetDisp.fMinValue);
					return DB_ERR_RELAY_OVERFLOW;
				}
			}
			pSET[i] = fValueList[i];
		}
	}
	else
	{
		// 越限检查
		if(DB_GetSysSETDisp(no, &tSetDisp) == DB_ERR_OK)
		{
			if((*fValueList - tSetDisp.fMaxValue) > 0.000001)
			{
				LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f > MAX(%f)!",\
					no,*fValueList,tSetDisp.fMaxValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
			
			if((*fValueList - tSetDisp.fMinValue) < -0.000001 )
			{
				LogError("DB_SetSystemSETValue",FILE_LINE,"SET %d =%f < MIN(%f)!",\
					no,*fValueList,tSetDisp.fMinValue);
				return DB_ERR_RELAY_OVERFLOW;
			}
		}
		pSET[no]= *fValueList;		
	}
	
	// 6。注册修改配置字操作
	pSETOpt->dwAppID 	= RtosGetCurrentTaskId();
	pSETOpt->dwOption1 	= SET_AREANO_INVALID;
	pSETOpt->dwOption2 	= no;
	pSETOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pSETOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入辅助定值确认。
	参数：	AreaNo   --  定值区号。同上。
			no,   	 --  定值序号。同上。
			Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetSystemSETValueConfirm(WORD no,WORD Attr)
{
	TDBaseConfigTable		*pDbase;				//数据库配置表
	TRunTimeOPT				*pSETOpt;
	float 					*pSET;
    WORD 					num;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pSET 	= pDbase->tModData.pSystemSETData;
	pSETOpt	= pDbase->pCFOpt;
	num  	= pDbase->nSystemSET;
	
	// 1。检查操作合法性
	if(pSETOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pSETOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if((pSETOpt->dwOption1 != SET_AREANO_INVALID)||(pSETOpt->dwOption2 != no))
		return DB_ERR_OPT_PARA;

	// 2。检查数据指针
	ASSERT(pSET != NULL);
	if(pSET == NULL)
		return DB_ERR_BASENULL;
	
	// 3。检查定值序号
	if((no != SET_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetSystemSETValueConfirm",FILE_LINE,"The SET No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pSETOpt->wTimeCnt	= 0;
		pSETOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}
	

	// 5。先写入RAM
	if(no == SET_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
			pDbase->pSystemSETData[i] = pSET[i];
	}
	else
		pDbase->pSystemSETData[no] = pSET[no];	
	
	DAE_MakeCheckSum((BYTE *)pDbase->pSystemSETData, sizeof(float)*num);

	// 4。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_SYSSET,no);
	DAE_ClearRunningFlags(SS_EEPROM_SYSSET_ERR); 	

	// 5。注册修改软压板操作
	pSETOpt->wTimeCnt 	= 0;
	pSETOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, no, 0, 0);

	DB_Register_SET(SET_AREANO_INVALID,no);

	//如果第0个辅助定值更改则重新初始化系数区
	if((no == SET_VALUE_ALL)||(no == 0))
		DB_UpdateCFBase();
	
	return rc;
}

/************************************************************
功能：通过第一个辅助定值（额定电流）判断当前系统是否事是1A额定电流。
	  该函数用于当前系数读写时，判断应该读写那个区。
参数：	无	
返回：辅助定值1A返回TRUE，否则返回错误。		
************************************************************/
BOOL	DB_Is1ASystem()
{
/*	float 	fRating = 5.0;
	long	lRating;
	
	DB_GetSystemSETFloatValue(SYSTEM_SET_DEFINE1, &fRating);

	lRating = fRating;

	if(lRating == 1)
		return TRUE;
	else
		return FALSE;
*/
	
	long	lRating;
	float	fRating = 5.0;

	
	DB_GetSystemSETFloatValue(SYSTEM_SET_DEFINE1, &fRating);
	lRating = (long)fRating;

	if(lRating == RATE_1A)
		return TRUE;
	else
		return FALSE;
}

/************************************************************
功能：当前系统故障电量是否显示一次值 
参数：无	
返回：TRUE/FALSE		
************************************************************/
BOOL	DB_IsFDShowFirst()
{
	return (G_pCfgTable->DBaseConfig.dwControl&DB_RELAY_FDSECOND)?FALSE:TRUE;
}

/************************************************************
功能：当前系统是否要计算积分电度
参数：无	
返回：TRUE/FALSE		
************************************************************/
BOOL	DB_IsCaculateIntegral()
{
	return (G_pCfgTable->DBaseConfig.dwControl&DB_RELAY_CACLINT)?TRUE:FALSE;
}
/************************************************************
功能：获取当前的时钟源类型
参数：无	
返回：TRUE，DK3511/FALSE,KF6511		
************************************************************/
BOOL	DB_WhichIRGB()
{
	return (G_pCfgTable->DBaseConfig.dwControl&DB_IRGB_DK3511)?TRUE:FALSE;
}

/************************************************************
	功能：从数据库中取变比个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetRTNum()
{
	return G_pCfgTable->DBaseConfig.nRT;
}

/************************************************************
	功能：从数据库中读取变比（浮点数）。
	参数：	no,   	 	--  变比序号
		    fValueList	--  浮点变比列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetRTValue(WORD no, float *fValueList)
{
//	TSysSET_Disp		*pSETDisp;
//	TCF_Disp			*pCFDisp;
//	long				Ratio;
	int					SETNo;
	STATUS 				rc;

	// 1。取定点定值
	SETNo = no+DB_GetSystemSETNum()-DB_GetRTNum();
	
	rc = DB_GetSystemSETFloatValue(SETNo, fValueList);
	if(rc != DB_ERR_OK)
	{
		LogError("DB_GetRTValue",FILE_LINE,"Get RT Value Error!");
		return DB_ERR_RELAY_SET;
	}

/*
	// 2。除掉倍数	
	pSETDisp = G_pCfgTable->DBaseConfig.pSystemSETDisp;
	Ratio	 = G_pCfgTable->DBaseConfig.pCFDisp[pSETDisp[SETNo].nCFIndex].wCof;
	if(Ratio == 0)
		Ratio = 1;

	*fValueList = (*fValueList)/Ratio;
*/
	
	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入变比。
	参数：no,   	 	--  变比序号。同上。
		  fValue	   	--  浮点变比列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetRTValue (WORD no, float *fValueList )
{
	if(no >= DB_GetRTNum())
	{
		LogError("DB_SetRTValue",FILE_LINE,"The RT No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	if(DB_GetSystemSETNum() < DB_GetRTNum())
	{
		LogError("DB_SetRTValue",FILE_LINE,"The RTNum=%d is overflow!",DB_GetRTNum());
		return DB_ERR_NO;
	}

	return DB_SetSystemSETValue(no+DB_GetSystemSETNum()-DB_GetRTNum(), fValueList);
}

/************************************************************
	功能：向数据库中写入变比确认。
	参数：	AreaNo   --  变比区号。同上。
			no,   	 --  变比序号。同上。
			Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetRTValueConfirm(WORD no,WORD Attr)
{
	if(no >= DB_GetRTNum())
	{
		LogError("DB_SetRTValueConfirm",FILE_LINE,"The RT No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	if(DB_GetSystemSETNum() < DB_GetRTNum())
	{
		LogError("DB_SetRTValueConfirm",FILE_LINE,"The RTNum=%d is overflow!",DB_GetRTNum());
		return DB_ERR_NO;
	}
		
	return DB_SetSystemSETValueConfirm(no+DB_GetSystemSETNum()-DB_GetRTNum(),Attr);
}

/************************************************************
	功能：从数据库中取系数个数。
	参数：无
	返回：正确返回个数，错误返回0xFF。
************************************************************/
WORD	DB_GetCFNum()
{
	return G_pCfgTable->DBaseConfig.nCF;
}

/************************************************************
	功能：从数据库中读取系数值。
	参数：	no,   		--  系数序号	CF_VALUE_ALL代表全部系数值
		    lValueList	--  系数值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCFValue(WORD no,long *lValueList)
{
	long 			*pCF;
    WORD 			num;
               
	pCF =  G_pCfgTable->DBaseConfig.pCFData;
	num  = G_pCfgTable->DBaseConfig.nCF;
	
	// 1。检查数据指针
	ASSERT(pCF != NULL);
	ASSERT(lValueList != NULL);

	if(pCF == NULL)
		return DB_ERR_BASENULL;

	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	// 2。检查定值序号
	if((no != CF_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_GetCFValue",FILE_LINE,"The CF No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 3。直接从RAM读取
	if(no == CF_VALUE_ALL)
		memcpy(lValueList,pCF,sizeof(long)*num);
	else
		*lValueList = pCF[no];		
	
	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入系数值。
	参数：	no,   		--  系数序号。同上。
		    lValueList	--  系数值列表	
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCFValue (WORD no, long *lValueList )
{
	TRunTimeOPT				*pCFOpt;
	TDBaseConfigTable		*pDbase;				//数据库配置表
	long 					*pCF;
	long 					*pCFMap;
    WORD 					num;

	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pCF 	= pDbase->tModData.pCFData;
	pCFMap	= pDbase->tMapData.pCFData;
	pCFOpt	= pDbase->pCFOpt;
	num  	= pDbase->nCF;
	
	// 1。检查数据指针
	ASSERT(lValueList != NULL);
	if(lValueList == NULL)
		return DB_ERR_BUFNULL;

	ASSERT(pCF != NULL);
	ASSERT(pCFMap != NULL);
	ASSERT(pCFOpt != NULL);
	if((pCF == NULL)||(pCFMap == NULL)||(pCFOpt == NULL))
		return DB_ERR_BASENULL;

	// 2。检查定值序号
	if((no != CF_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetCFValue",FILE_LINE,"The CF No=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 4。写入在线修改区

	// 4。1 先更新在线修改数据
	for(int i=0;i<num;i++)
	 	pCF[i] = RLTOBL(pCFMap[i]);
	
	// 4。2 再写入要修改的数据
	if(no == CF_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
			pCF[i] = lValueList[i];
	}
	else
		pCF[no]= *lValueList;		

	// 6。注册修改配置字操作
	pCFOpt->dwAppID 	= RtosGetCurrentTaskId();
	pCFOpt->dwOption1 	= no;
	pCFOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pCFOpt->wStatus 	= OPT_STATUS_SET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入系数值确认。
	参数：	no,   		--  系数序号。同上。
			Attr		--	操作属性,=DB_EXECUTE 执行;=DB_CANCEL 取消
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCFValueConfirm(WORD no,WORD Attr)
{
	TDBaseConfigTable		*pDbase;				//数据库配置表
	TRunTimeOPT				*pCFOpt;
	long 					*pCF;
    WORD 					num;
	STATUS					rc;
	
	pDbase 	= &G_pCfgTable->DBaseConfig;	 
	pCF 	= pDbase->tModData.pCFData;
	pCFOpt	= pDbase->pCFOpt;
	num  	= pDbase->nCF;
	
	// 1。检查操作合法性
	if(pCFOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pCFOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pCFOpt->dwOption1 != no)
		return DB_ERR_OPT_PARA;

	// 2。检查数据指针
	ASSERT(pCF != NULL);
	if(pCF == NULL)
		return DB_ERR_BASENULL;
	
	// 3。检查定值序号
	if((no != CF_VALUE_ALL)&&(no >= num))
	{
		LogError("DB_SetCFValueConfirm",FILE_LINE,"The CF No=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	if(Attr == DB_CANCEL)
	{
		pCFOpt->wTimeCnt	= 0;
		pCFOpt->wStatus 	= OPT_STATUS_READY;
		return DB_ERR_OK;
	}

	// 4。先写入RAM
	if(no == CF_VALUE_ALL)
	{
		for(int i=0;i<num;i++)
			pDbase->pCFData[i] = pCF[i];
	}
	else
		pDbase->pCFData[no] = pCF[no];	

	DAE_MakeCheckSum((BYTE *)pDbase->pCFData, sizeof(float)*num);
	
	// 5。保存保护数据文件
	rc = DB_ModifyDataFile(DB_DATA_CF,no);

	DAE_ClearRunningFlags(SS_EEPROM_CF_ERR); 	

	// 6。注册修改软压板操作
	pCFOpt->wTimeCnt 	= 0;
	pCFOpt->wStatus 	= OPT_STATUS_READY;

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_CF, no, 0, 0);

	DB_Register_CF(no);
	
	// 7。再更新定点定值
	DB_UpdateSETFixDataBase();
	
	return rc;
}

//BYTE TempTestBuffer[sizeof(float)*(RS_COUNT+1)*SETTING_AREA_COUNT];

STATUS DB_ModifyDataFile(WORD wType,WORD no)
{
    TFileHead         	*filehead;
	TMapDataList		*pMap;
	TMapDataList		*pMod;
	TRunTimeOPT			*pSETOpt;
//	long				*pCFData;					//系数数据指针		
	WORD				num,i;
	DWORD				dwEEPROMAddr;
	DWORD				dwEEPROMLen;
	BYTE				*pEEPROMBuf;
	
//	DWORD				dwTemp;
	
	pMap = &G_pCfgTable->DBaseConfig.tMapData;
	pMod = &G_pCfgTable->DBaseConfig.tModData;

	ASSERT(pMap != NULL);
	ASSERT(pMod != NULL);
	if((pMap == NULL)||(pMod == NULL))
	{
		return DB_ERR_OPT_SAVE;
	}

	dwEEPROMAddr  = sizeof(TFileHead)+sizeof(TRelayDataTable);
	
	// 1。修正文件内容
	switch(wType)
	{
	case DB_DATA_SW:
		num = (DB_GetSWNum()+31)/32;
		for(i=0;i<num;i++)
		{
			pMap->pSWData[i] = RLTOBL(pMod->pSWData[i]);
			pMap->pSWData[num+i] = ~pMap->pSWData[i];
		}

		dwEEPROMLen = (SW_COUNT+31)/32*2*sizeof(DWORD);
		pEEPROMBuf = (BYTE *)pMap->pSWData;
		break;	
		
/*
	case DB_DATA_CFG:
		num = DB_GetCFGNum();
		for(i=0;i<(num+31)/32;i++)
		{
			dwTemp = *(DWORD *)(pMod->pSETData+i);
			*(DWORD *)(pMap->pSETData+i) = RLTOBL(dwTemp);
		}
		break;	
*/

	case DB_DATA_SET:
		pSETOpt	= G_pCfgTable->DBaseConfig.pSETOpt;
		ASSERT(pSETOpt->dwOption1<DB_GetSETAreaNum());
		if(pSETOpt->dwOption1 >= DB_GetSETAreaNum())
		{
			LogError("DB_ModifyDataFile", FILE_LINE, "Set SET Area No Err!");
			break;
		}
		
		num 	= DB_GetSETNum();
		for(i=0;i<num;i++)
		{
			pMap->pSETData[pSETOpt->dwOption1*(num+1)+i] = CONVERTFLOAT(pMod->pSETData[i]);
		}
		DAE_MakeCheckSum((BYTE *)&pMap->pSETData[pSETOpt->dwOption1*(num+1)],sizeof(float)*num);

//		LogError("DB_ModifyDataFile",FILE_LINE,"The Set Data Modified! Zero=%d, Num=%d Check=0x%x:%x ",\
//			G_pCfgTable->DBaseConfig.nSETAreaCur,G_pCfgTable->DBaseConfig.nSET,\
//			*((    (BYTE *)&pMap->pSETData[pSETOpt->dwOption1*(num+1)]  )+num*sizeof(float)),\
//			*((    (BYTE *)&pMap->pSETData[pSETOpt->dwOption1*(num+1)]  )+num*sizeof(float)+1));

		dwEEPROMAddr += (SW_COUNT+31)/32*2*sizeof(DWORD);
		dwEEPROMLen   = (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		pEEPROMBuf = (BYTE *)pMap->pSETData;

		break;	
		
	case DB_DATA_SETAREACUR:
		pMap->pRelayCfg->nSETAreaCur = RLTOBS(pMod->pRelayCfg->nSETAreaCur);
		pMap->pRelayCfg->nSETAreaCurB = RLTOBS(pMod->pRelayCfg->nSETAreaCurB);

		dwEEPROMAddr  = sizeof(TFileHead);
		dwEEPROMLen   = sizeof(TRelayDataTable);
		pEEPROMBuf = (BYTE *)pMap->pRelayCfg;
		break;	
		
	case DB_DATA_SYSSET:
		num 	= DB_GetSystemSETNum();
		for(i=0;i<num;i++)
		{
			pMap->pSystemSETData[i] = CONVERTFLOAT(pMod->pSystemSETData[i]);
		}
		DAE_MakeCheckSum((BYTE *)pMap->pSystemSETData,sizeof(float)*num);

		dwEEPROMAddr += (SW_COUNT+31)/32*2*sizeof(DWORD);
		dwEEPROMAddr += (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		dwEEPROMLen  = (AS_COUNT+1)*sizeof(float);
		pEEPROMBuf = (BYTE *)pMap->pSystemSETData;

		break;	

	case DB_DATA_CF:
/*
		num 	= DB_GetCFNum();
		//根据额定值选取不同的定值区
		if(DB_Is1ASystem())
		{
			pCFData = pMap->pCFData+num+1;
		}
		else
		{
			pCFData = pMap->pCFData;
		}
		
		for(i=0;i<num;i++)
		{
			pCFData[i] = RLTOBL(pMod->pCFData[i]);
		}
		
		DAE_MakeCheckSum((BYTE *)pCFData,sizeof(long)*num);
		DAE_MakeCheckSum((BYTE *)(pCFData+num+1),sizeof(long)*num);

		dwEEPROMAddr += (SW_COUNT+31)/32*2*sizeof(DWORD);
		dwEEPROMAddr += (RS_COUNT+1)*SETTING_AREA_COUNT*sizeof(float);
		dwEEPROMAddr += (AS_COUNT+1)*sizeof(float);
		dwEEPROMLen  = (CF_COUNT+1)*2*sizeof(long)+2;
		pEEPROMBuf = (BYTE *)pMap->pCFData;
*/

		// 改为直接写入指定缓冲区
		DAE_MakeCheckSum((BYTE *)pMod->pCFData,sizeof(long)*CF_COUNT);

		if(DB_Is1ASystem())
			dwEEPROMAddr = EEPROM_RELAY_CFBASE+(CF_COUNT+1)*sizeof(long);
		else
			dwEEPROMAddr = EEPROM_RELAY_CFBASE;
			
		dwEEPROMLen  = (CF_COUNT+1)*sizeof(long);

		EEPROM_Write(dwEEPROMAddr,dwEEPROMLen,pMod->pCFData);

		// 直接返回
		return DB_ERR_OK;	

	default:
		return DB_ERR_OPT_SAVE;
	}

	// 2。重新计算文件校验
	if(G_pCfgTable->DBaseConfig.pRelayDataFileMap == NULL)
		return DB_ERR_FILE_WRITE;
		
	filehead = (TFileHead *)G_pCfgTable->DBaseConfig.pRelayDataFileMap->pRamData;

	//跟新修改时间
	ReadSystemTime(&filehead->stTime);	
	filehead->stTime.Year = ltobs(filehead->stTime.Year);
	filehead->stTime.MSecond= ltobs(filehead->stTime.MSecond);

	//更新修改标志
	filehead->dwMagic = ltobl(FILE_MAGIC_MODI);

	//重新计算校验
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-sizeof(WORD));	

	// 3。写入文件数据
//	return DAE_WriteFile(RELAYDATAFILE, filehead,filehead->dwSize);

	// 3.1 修改内容写入EEPROM文件
	EEPROM_Write(EEPROM_RELAY_DATA+dwEEPROMAddr,dwEEPROMLen,pEEPROMBuf);

//	LogError("DB_ModifyDataFile",FILE_LINE,"write data! Start=0x%x, Len=%d", \
//		EEPROM_RELAY_DATA+dwEEPROMAddr,dwEEPROMLen);

	
	// 3.1 文件头写入EEPROM文件
	EEPROM_Write(EEPROM_RELAY_DATA,sizeof(TFileHead),(BYTE *)filehead);

//	LogError("DB_ModifyDataFile",FILE_LINE,"write head! Start=0x%x, Len=%d", \
//		EEPROM_RELAY_DATA,sizeof(TFileHead));

	// 3.2 文件校验写入EEPROM
	pEEPROMBuf = (BYTE *)filehead;
	pEEPROMBuf += filehead->dwSize-sizeof(WORD);	
	EEPROM_Write(EEPROM_RELAY_DATA+filehead->dwSize-2,2,pEEPROMBuf);

//	LogError("DB_ModifyDataFile",FILE_LINE,"write check! Start=0x%x, Len=%d", \
//		EEPROM_RELAY_DATA+filehead->dwSize-2,2);

	//再读出来看一下
//	EEPROM_Read(EEPROM_RELAY_DATA+dwEEPROMAddr,dwEEPROMLen,TempTestBuffer);
//	for(i=0;i<dwEEPROMLen;i++)
//	{
//		if(TempTestBuffer[i]!=pEEPROMBuf[i])
//		{
//			LogError("DB_ModifyDataFile",FILE_LINE,"The EEPROM write error! Pos=%d", i);
//			break;
//		}
//	}

	return DB_ERR_OK;
}

STATUS DB_SaveRelayEventFile()
{	
    TFileHead         		*filehead;
	TFileMap				*pFileMap;
	TRelayAlarmWrap			*pRALMWrap;
	TRelayActionWrap		*pRACTWrap;
	TRelayEventPtrList		*pPtrList;
	TRelayAlarmEv			*pRALMEvent;	
	TRelayActionEv			*pRACTEvent;	
	TAppInfo				tTempApp;
	WORD					wPtr;
	
	pRALMWrap 	= G_pCfgTable->DBaseConfig.pRALMWrap;
	pRACTWrap 	= G_pCfgTable->DBaseConfig.pRACTWrap;
	pFileMap 	= G_pCfgTable->DBaseConfig.pRelayEventFileMap;

	if((pRALMWrap == NULL)||(pRACTWrap == NULL))
	{
		LogError("DB_SaveRelayEventFile",FILE_LINE,"The wrap database is NULL!!!");
		return DB_ERR_BASENULL;
	}
	
	if(pFileMap == NULL)
		return DB_ERR_MAP_NULL;
	
	filehead = (TFileHead *)pFileMap->pRamData;
	pPtrList = (TRelayEventPtrList	*)(filehead+1);

	// 1。修正文件内容
	
/*
	// 1.1 保护告警事项
	pRALMEvent = (TRelayAlarmEv  *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
					+sizeof(TRelayActionEv)*MAX_RACT_NUM);

	//对于空表，直接全部写入;否则只写新产生的
	if(pPtrList->nRelayAlarmH == pPtrList->nRelayAlarmT)
		wPtr = pRALMWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayAlarmT%MAX_RALM_NUM;

	for(;wPtr !=pRALMWrap->nTailIndex;)
	{
		pRALMEvent[wPtr] = pRALMWrap->pEvent[wPtr];
					
		//按照文件操作规范，应该更新文件，但是模拟文件存盘效率就比较低了
		//所以直接操作EEPROM了				
		EEPROM_Write(EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead), \
						sizeof(TRelayAlarmEv), &pRALMEvent[wPtr]);

		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM alarm event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead),sizeof(TRelayAlarmEv));
		wPtr = (wPtr+1)%MAX_RALM_NUM;
	}
							
	// 1.2 保护动作事项
	pRACTEvent = (TRelayActionEv	 *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList));
	
	//对于空表，直接全部写入;否则只写新产生的
	if(pPtrList->nRelayActionH == pPtrList->nRelayActionT)
		wPtr = pRACTWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayActionT%MAX_RACT_NUM;

	for(;wPtr !=pRACTWrap->nTailIndex;)
	{
		pRACTEvent[wPtr] = pRACTWrap->pEvent[wPtr];
		//按照文件操作规范，应该更新文件，但是模拟文件存盘效率就比较低了
		//所以直接操作EEPROM了
		EEPROM_Write(EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead, \
						sizeof(TRelayActionEv), &pRACTEvent[wPtr]);
					
		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM action event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead,sizeof(TRelayActionEv));
		wPtr = (wPtr+1)%MAX_RACT_NUM;
	}
				
	// 1.3 保护事项指针
	pPtrList->nMaxRalayAction = MAX_RACT_NUM;
	pPtrList->nRelayActionT = pRACTWrap->nTailIndex;
	pPtrList->nRelayActionH = pRACTWrap->nHeadIndex;
	
	pPtrList->nMaxRalayAlarm = MAX_RALM_NUM;
	pPtrList->nRelayAlarmT = pRALMWrap->nTailIndex;
	pPtrList->nRelayAlarmH = pRALMWrap->nHeadIndex;

	//保存液晶事件读指针
	if(DAE_Portid2AppInfo(SYS_PORTID_LCD,&tTempApp) == DB_ERR_OK)
	{
		pPtrList->nRelayActionLEDR = tTempApp.nRACTRead;
		pPtrList->nRelayAlarmLEDR = tTempApp.nRALMRead;
	}
	//保存网口1事件读指针
	if((DAE_Portid2AppInfo((SC_PORT_CANBUS<<8)+1,&tTempApp) == DB_ERR_OK)\
		||(DAE_Portid2AppInfo((SC_PORT_ETHERNETA<<8)+1,&tTempApp) == DB_ERR_OK))
	{
		pPtrList->nRelayActionNet1R = tTempApp.nRACTRead;
		pPtrList->nRelayAlarmNet1R = tTempApp.nRALMRead;
	}
	//保存网口2事件读指针
	if((DAE_Portid2AppInfo((SC_PORT_CANBUS<<8)+2,&tTempApp) == DB_ERR_OK)\
		||(DAE_Portid2AppInfo((SC_PORT_ETHERNETB<<8)+1,&tTempApp) == DB_ERR_OK))
	{
		pPtrList->nRelayActionNet2R = tTempApp.nRACTRead;
		pPtrList->nRelayAlarmNet2R = tTempApp.nRALMRead;
	}
*/
	// 1.1 保护告警事项
	pRALMEvent = (TRelayAlarmEv  *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList)\
					+sizeof(TRelayActionEv)*MAX_RACT_NUM);

	//对于空表，直接全部写入;否则只写新产生的
	if(pPtrList->nRelayAlarmH == pPtrList->nRelayAlarmT)
		wPtr = pRALMWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayAlarmT%MAX_RALM_NUM;

	if(wPtr !=pRALMWrap->nTailIndex)
	{
		pRALMEvent[wPtr] = pRALMWrap->pEvent[wPtr];
					
		//按照文件操作规范，应该更新文件，但是模拟文件存盘效率就比较低了
		//所以直接操作EEPROM了				
		EEPROM_Write(EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead), \
						sizeof(TRelayAlarmEv), &pRALMEvent[wPtr]);

//		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM alarm event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+((BYTE *)(pRALMEvent+wPtr)-(BYTE *)filehead),sizeof(TRelayAlarmEv));
		wPtr = (wPtr+1)%MAX_RALM_NUM;

		pPtrList->nRelayAlarmT = wPtr;
		pPtrList->nRelayAlarmH = pRALMWrap->nHeadIndex;		
		pPtrList->nMaxRalayAlarm = MAX_RALM_NUM;
	}
							
	// 1.2 保护动作事项
	pRACTEvent = (TRelayActionEv	 *)((BYTE *)pFileMap->pRamData\
					+sizeof(TFileHead)+sizeof(TRelayEventPtrList));
	
	//对于空表，直接全部写入;否则只写新产生的
	if(pPtrList->nRelayActionH == pPtrList->nRelayActionT)
		wPtr = pRACTWrap->nHeadIndex;
	else	
		wPtr = pPtrList->nRelayActionT%MAX_RACT_NUM;

	if(wPtr !=pRACTWrap->nTailIndex)
	{
		pRACTEvent[wPtr] = pRACTWrap->pEvent[wPtr];
		//按照文件操作规范，应该更新文件，但是模拟文件存盘效率就比较低了
		//所以直接操作EEPROM了
		EEPROM_Write(EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead, \
						sizeof(TRelayActionEv), &pRACTEvent[wPtr]);
					
//		LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM action event! Index=%d Addr=0x%08x Len=%d",wPtr,EEPROM_RELAY_EVENT+(BYTE *)(pRACTEvent+wPtr)-(BYTE *)filehead,sizeof(TRelayActionEv));
		wPtr = (wPtr+1)%MAX_RACT_NUM;

		pPtrList->nRelayActionT = wPtr;
		pPtrList->nRelayActionH = pRACTWrap->nHeadIndex;
		pPtrList->nMaxRalayAction = MAX_RACT_NUM;
	}
				
	ReadSystemTime(&filehead->stTime);

	//BigEnden存储
	filehead->wFlag = 0x8000;
	
	// 2。重新计算文件校验
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-sizeof(WORD));	
	
	// 3。写入文件数据
	//按照文件操作规范，应该更新文件，但是模拟文件存盘效率就比较低了
	//所以直接操作EEPROM了

	// 3.1 写文件头和指针表
	EEPROM_Write(EEPROM_RELAY_EVENT, sizeof(TFileHead)+sizeof(TRelayEventPtrList), filehead);
//	LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM relay event file Head! Addr=0x%08x Len=%d",EEPROM_RELAY_EVENT,sizeof(TFileHead)+sizeof(TRelayEventPtrList));

	// 3.2 写校验
	EEPROM_Write(EEPROM_RELAY_EVENT+filehead->dwSize-2, 2, (BYTE *)filehead+filehead->dwSize-2);
//	LogError("DB_SaveRelayEventFile",FILE_LINE,"Save EEPROM relay event file check! Addr=0x%08x",EEPROM_RELAY_EVENT+filehead->dwSize-2);

	if(pPtrList->nRelayActionT ==pRACTWrap->nTailIndex)
	if(pPtrList->nRelayAlarmT ==pRALMWrap->nTailIndex)
		pFileMap->wFlag &= ~FILEMAP_FLAGS_MODIFY;
	
	return DB_ERR_OK;	
}

/*=================录波调试用=======================*/

/************************************************************
* 参数说明      - dwStartPtr	: 输入 	原始采样数据缓冲区开始位置
*				- wPointNum : 输入	参与比较的通道点数
*************************************************************/
/*
BOOL isSampDataLenFill(DWORD dwStartPtr, DWORD wPointNum)
{
	return TRUE;
}
*/
/************************************************************
* 参数说明		- dwPtr    : 输入  历史数据起始位置
*				- ChanMask : 输入  通道掩码
*				- pBuf	   : 输出  需要保存的数据缓冲区
*				- wPointNum    : 输入  需要读取的采样点数
*************************************************************/
/*
STATUS ReadHisSampData(DWORD dwPtr, DWORD dwMask, SHORT *pBuf, DWORD dwPointNum)
{
	WORD	wCnt=0;
	BYTE	*pDistData = (BYTE *)pBuf;
	
	for(int i=0;i<32;i++)
		if(dwMask&(0x01<<i))
			wCnt++;
		
	if(dwPtr == 0)
	{
		//总采样点数
		for(int m=0;m<dwPointNum;m++)
		{
			//各模拟通道附值
			for(int n=0;n<wCnt;n++)
			{
				pDistData[m*wCnt*2+n*2] = 0x11*(n+1)+(m%10);
				pDistData[m*wCnt*2+n*2+1] = 0;//0x11*(n+1)+(m%10);
			}
		}	
	}
	else
	{
		//总采样点数
		for(int m=0;m<dwPointNum;m++)
		{
			//各模拟通道附值
			for(int n=0;n<wCnt;n++)
			{
				pDistData[m*wCnt*2+n*2] = 0x11*(n+1);
				pDistData[m*wCnt*2+n*2+1] = 0x11*(n+1);
			}
		}	
	}	

	return DB_ERR_OK;
}
*/
/*=================录波调试用=======================*/

static void FormatDecData(char *str,DWORD data,BYTE MaxLen)
{
	BYTE 	BitData;
	DWORD	HiData;
	
	HiData = data;
	str[MaxLen] = '\0';
	
	for(int i=0;i<MaxLen;i++)
	{
		BitData = HiData%10;
		str[MaxLen-1-i] = BitData+'0';
		HiData /= 10;		
	}
}

/************************************************************
	功能：获取录波数据文件名
	参数: pDistBlock	-- 录波数据指针
	返回：正确返回0，错误返回错误值。
************************************************************/
BOOL	DB_GetDistFileName(TAbsTime *pTime,char *strFileName)
{
	TSysTime	tSysTime;
	
	// 1.2 转换成系统时标
	AbsTimeTo(pTime, &tSysTime);
	tSysTime.Year %= 100;
	
	strFileName[0] = '0'+tSysTime.Year/10;
	strFileName[1] = '0'+tSysTime.Year%10;
	strFileName[2] = '0'+tSysTime.Month/10;
	strFileName[3] = '0'+tSysTime.Month%10;
	strFileName[4] = '0'+tSysTime.Day/10;
	strFileName[5] = '0'+tSysTime.Day%10;
	strFileName[6] = '0'+tSysTime.Hour/10;
	strFileName[7] = '0'+tSysTime.Hour%10;
	strFileName[8] = '0'+tSysTime.Minute/10;
	strFileName[9] = '0'+tSysTime.Minute%10;
	strFileName[10] = '0'+tSysTime.Second/10;
	strFileName[11] = '0'+tSysTime.Second%10;
	strFileName[12] = '0'+tSysTime.MSecond/100;
	strFileName[13] = '0'+(tSysTime.MSecond%100)/10;
	strFileName[14] = '0'+tSysTime.MSecond%10;
	strFileName[15] = '\0';
	
	return TRUE;
}

/************************************************************
	功能：获取录波数据长度
	参数: None
	返回：正确返回0，错误返回错误值。
************************************************************/
DWORD DB_GetDistDataLength()
{
	TDist_Disp		*pDistDisp;
	DWORD			dwBLength;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	if(pDistDisp == NULL)
		dwBLength= MAX_DISTBLOCK_LEN;
	else
	{
		//每个条目长度
		dwBLength = 4+4+pDistDisp->wAnalogChannelNum*2+(pDistDisp->wDigitalChannelNum+15)/16*2;
		//所有条目长度
		dwBLength *= pDistDisp->dwTotalSampleNum;
		//加上一个头
		dwBLength += sizeof(TDistHead);
	}

	//最少分配2K
	if(dwBLength < 2048)
		dwBLength = 2048;

	//最多分配128K
	//if(dwBLength > MAX_DISTBLOCK_LEN)
	//	dwBLength = MAX_DISTBLOCK_LEN;

	return dwBLength;
}

/************************************************************
	功能：写入录波数据块
	参数: pDistBlock	-- 录波数据指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_WriteDistDataBlock (void *pDistBlock)
{
    CNode 				*pNode;
	
	// 1。检查数据指针的合法性
	ASSERT(pDistBlock != NULL);
	if(pDistBlock == NULL)
		return DB_ERR_PTRNULL;
	
	// 2。写入录波数据信息	
	OS_Protect(0);
	
	// 3.1 申请一个空闲数据块节点
	pNode = G_DistDataFreeLink.RemoveHead();
	if(pNode == NULL)
	{
		OS_UnProtect(0);
		LogError("DB_RelayWriteDistData",FILE_LINE,"The Dist Block run out!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
	
	// 3.2 拷贝数据块
	memcpy(pNode->pData,pDistBlock,DB_GetDistDataLength());	
	
	// 3.3 链入CACHE录波数据链
	G_DistDataSaveLink.AddTail(pNode);
	
	OS_UnProtect(0);

	return DB_ERR_OK;
}

/************************************************************
	功能：启动一次录波，写入触发前录波数据
	参数: 	pTime		-- 触发时间指针
			dwSamplePtr	-- 采样指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_StartDistRecord(DWORD	dwSamplePtr,TAbsTime *pTime,DWORD dwPrevSampleNum)
{
    CNode 				*pNode;
	TDistHead			*pHead;
	TDist_Disp			*pDistDisp;
	TAC_Disp			*pACDisp;
	TDC_Disp			*pDCDisp;
	WORD				i;
	
	if(G_DistDataFreeLink.IsEmpty())
		return DB_ERR_OK;
	
	// 判断数据库指针合法性
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pACDisp = G_pCfgTable->DBaseConfig.pACPara;
	pDCDisp = G_pCfgTable->DBaseConfig.pDCPara;
	if((pDistDisp == NULL)||(pACDisp == NULL)||(pDCDisp == NULL))
		return DB_ERR_BASENULL;
	
	// 1. 获取空闲节点
	pNode = G_DistDataFreeLink.RemoveHead();
	if(pNode == NULL)
	{
		LogError("DB_StartDistRecord",FILE_LINE,"The Dist Block run out!!!");
		return DB_ERR_RELAY_NOCACHE;
	}
			
	// 2. 更新录波数据头
	pHead = (TDistHead *)pNode->pData;

	pHead->wStatus 	= DIST_STATUS_BUSY;
	pHead->wPerSize = pDistDisp->wAnalogChannelNum*2+(pDistDisp->wDigitalChannelNum+15)/16*2;
	pHead->dwSamplePtr 	= dwSamplePtr;
	pHead->tTime 		= *pTime;

	// 2.1 计算点数，增加容错性
	if(dwPrevSampleNum <= pDistDisp->dwTotalSampleNum)
	{
		pHead->dwPrevSampleNum = dwPrevSampleNum;
	}
	else if(pDistDisp->dwPrevSampleNum <= pDistDisp->dwTotalSampleNum)
	{
		pHead->dwPrevSampleNum = pDistDisp->dwPrevSampleNum;
	}
	else
	{
		pHead->dwPrevSampleNum = pDistDisp->dwTotalSampleNum;
	}
	
	pHead->dwRearSampleNum = pDistDisp->dwTotalSampleNum-pHead->dwPrevSampleNum;
	
	// 2.2 计算通道掩码
	pHead->dwMask = 0;
	for(i=0;i<pDistDisp->wAnalogChannelNum;i++)
		pHead->dwMask |= 0x01<<pACDisp[i].wIndex;
	for(i=0;i<(pDistDisp->wDigitalChannelNum+15)/16;i++)
		pHead->dwMask |= 0x01<<(AD_CHAN_NUM+A_COM_CHAN_NUM+i);

	dwSamplePtr = BackSamplePtr(dwSamplePtr,pHead->dwPrevSampleNum);
	
	// 3。写入触发前录波数据信息
	ReadHisSampData(dwSamplePtr,pHead->dwMask,(SHORT *)(pHead+1),pHead->dwPrevSampleNum);
	
	//ReportMsg("DB_StartDistRecord:  Start dist(%x->%x) PrevSampleNum =%d",(DWORD)pNode,(DWORD)pHead,pHead->dwPrevSampleNum);

	// 4. 节点写入待存储链表	
	G_DistDataSaveLink.AddTail(pNode);

	return DB_ERR_OK;
}

/************************************************************
	功能：跟踪写入录波数据块
	参数: pDistBlock	-- 录波数据指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_TraceDistRecord ()
{
	TDistHead			*pHead;
    CNode 				*pTempNode;
	BYTE				*pDistData;

	if(G_DistDataSaveLink.IsEmpty())
		return DB_ERR_OK;

	// 1. 顺序检索各待存节点
	G_DistDataSaveLink.ToHead();
	
	pTempNode = G_DistDataSaveLink.Next();

	//判断链表是否为空链,链表为空链返回
	while(pTempNode != NULL)
    {
		// 2. 对待追踪的节点进行追踪
		pHead = (TDistHead *)pTempNode->pData;
		if(pHead->wStatus == DIST_STATUS_BUSY)
		{
			// 3. 录波结束，写入节点内存
			if(isSampDataLenFill(pHead->dwSamplePtr,pHead->dwRearSampleNum))
			{
				// 3.1 写入数据
				pDistData = (BYTE *)(pHead+1);
				pDistData += pHead->wPerSize*pHead->dwPrevSampleNum;
				ReadHisSampData(pHead->dwSamplePtr,pHead->dwMask,(SHORT *)pDistData,pHead->dwRearSampleNum);

				// 3.2 更新录波存储状态
				pHead->wStatus = DIST_STATUS_SAVE;

				//ReportMsg("DB_TraceDistRecord:	Stop dist(%x->%x) RearSampleNum =%d",(DWORD)pTempNode,(DWORD)pHead,pHead->dwRearSampleNum);
			}
		}
		
		pTempNode = G_DistDataSaveLink.Next();
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：生成录波配置文件
	参数: pDistBlock	-- 录波数据指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SaveDistConfFile(char	 *strFileName,TAbsTime *pDistTime,DWORD dwPrevSampleNum)
{
	char				strConfigFileName[32];
	char 				strFHour[3];
	char 				strFMinute[3];
	char 				strFSecond[3];	
	char 				strFMSecond[7];
	
	TAC_Disp			*pACDisp;
	TDC_Disp			*pDCDisp;
	TDist_Disp			*pDistDisp;
	char				*pFileData;
	TCF_Disp			tCFDisp;
	TSysTime			tSysTime;
	TAbsTime			t1,t2;
	long				lCFValue = 1;
	long				lCFRatio = 1;
	float				a,fRTValue;
	WORD				wDCNum,wACNum;
	STATUS				rc;
	int					i;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pACDisp = G_pCfgTable->DBaseConfig.pACPara;
	pDCDisp = G_pCfgTable->DBaseConfig.pDCPara;

	if((pDistDisp == NULL)||(pACDisp == NULL)||(pDCDisp == NULL))
		return DB_ERR_BASENULL;
			
	// 1.先写入配置文件
		
	// 1.1 生成配置文件 
	pFileData = (char *)G_pDistFileBlock;

	// station_name,recdev_id,rev_year<CR/LF>
	pFileData[0] = '\0';
	sprintf(pFileData,"KEYVIA,%s,2000\r\n",pDistDisp->strDevName);
	pFileData += strlen(pFileData);
	
	// TT,##A,##D<CR/LF>
	wDCNum	= pDistDisp->wDigitalChannelNum;
	wACNum	= pDistDisp->wAnalogChannelNum;

	pFileData[0] = '\0';
	sprintf(pFileData,"%d,%dA,%dD\r\n",wACNum+wDCNum,wACNum,wDCNum);
	pFileData += strlen(pFileData);
	
	// An,ch_id,ph,ccbm,uu,a,b,skew,min,max,primary,secondary,PS<CR/LF>
	for(i=0;i<wACNum;i++)
	{
		// 取系数
		rc = DB_GetCFValue(pACDisp[i].wCoefIndex, &lCFValue);
		if((rc != DB_ERR_OK)||(lCFValue == 0))
		{
			lCFValue = 1;
			LogError("DB_SaveDistConfFile",FILE_LINE,"InValid AC's CF !!!");
		}
		
		rc = DB_GetCFDisp(pACDisp[i].wCoefIndex, &tCFDisp);
		if((rc != DB_ERR_OK)||(tCFDisp.wCof == 0))
		{
			lCFRatio = 1;
			LogError("DB_SaveDistConfFile",FILE_LINE,"InValid AC's CF Disp !!!");
		}
		else
			lCFRatio = tCFDisp.wCof;
	
		a = ((float)lCFRatio)/((float)lCFValue);

		//取变比
		rc = DB_GetRTValue(pACDisp[i].wRTIndex,&fRTValue);
		if((rc != DB_ERR_OK)||(fRTValue == 0))
		{
			fRTValue = 1;
			LogError("DB_SaveDistConfFile",FILE_LINE,"InValid RT Value !!!");
		}
		
		//调整缩放比例
		if(pACDisp[i].wCompressMultiple != 0)
			a *= pACDisp[i].wCompressMultiple;

		//换算系数比率
		a *= pACDisp[i].fCFRatio;
		
		pFileData[0] = '\0';
		sprintf(pFileData,"%d,%s,,,%s,%f,0,0,-32768,32767,%d,1,S\r\n",\
			i+1,pACDisp[i].strName,pACDisp[i].strUnit,a,\
			/*(float)1000000/(pDistDisp->wSampleRate*pDistDisp->wAnalogChannelNum),\*/
			(long)fRTValue);
		pFileData += strlen(pFileData);
	}
	
	// Dn,ch_id,ph,ccbm,y<CR/LF>
	for(i=0;i<wDCNum;i++)
	{
		pFileData[0] = '\0';
		sprintf(pFileData,"%d,%s,,,0\r\n",i+1,pDCDisp[i].strName);
		pFileData += strlen(pFileData);
	}
	
	// If<CR/LF>
	pFileData[0] = '\0';
	if(pDistDisp->wSysFrequency%100)
		sprintf(pFileData,"%d.%d\r\n",pDistDisp->wSysFrequency/100,pDistDisp->wSysFrequency%100);
	else
		sprintf(pFileData,"%d\r\n",pDistDisp->wSysFrequency/100);
	pFileData += strlen(pFileData);
	
	// nrates
	pFileData[0] = '\0';
	sprintf(pFileData,"1\r\n");
	pFileData += strlen(pFileData);
	
	// samp,endsamp
	pFileData[0] = '\0';
	sprintf(pFileData,"%d,%d\r\n",pDistDisp->wSampleRate,pDistDisp->dwTotalSampleNum);
	pFileData += strlen(pFileData);
	
	// dd/mm/yyyy,hh:mm:ss.ssssss<CR/LF>

	// 计算第一个数据点时间
	t1 = *pDistTime;
	t2.Hi = 0;
	t2.Lo = 1000*dwPrevSampleNum/pDistDisp->wSampleRate;
	AbsTimeSub(&t1,&t2);

	AbsTimeTo(&t1,&tSysTime);

	//填充格式化时间
	FormatDecData(strFHour, tSysTime.Hour,2);
	FormatDecData(strFMinute, tSysTime.Minute,2);
	FormatDecData(strFSecond, tSysTime.Second,2);
	FormatDecData(strFMSecond, tSysTime.MSecond*1000,6);
	
	pFileData[0] = '\0';
	sprintf(pFileData,"%d/%d/%d,%s:%s:%s.%s\r\n",tSysTime.Day,tSysTime.Month,tSysTime.Year,\
			strFHour,strFMinute,strFSecond,strFMSecond);
	pFileData += strlen(pFileData);

	// dd/mm/yyyy,hh:mm:ss.ssssss<CR/LF>

	//计算触发点时间
	AbsTimeTo(pDistTime,&tSysTime);
	
	//填充格式化时间
	FormatDecData(strFHour, tSysTime.Hour,2);
	FormatDecData(strFMinute, tSysTime.Minute,2);
	FormatDecData(strFSecond, tSysTime.Second,2);
	FormatDecData(strFMSecond, tSysTime.MSecond*1000,6);
	
	pFileData[0] = '\0';
	sprintf(pFileData,"%d/%d/%d,%s:%s:%s.%s\r\n",tSysTime.Day,tSysTime.Month,tSysTime.Year,\
		strFHour,strFMinute,strFSecond,strFMSecond);
	pFileData += strlen(pFileData);
	
	// ft<CR/LF>
	pFileData[0] = '\0';
	sprintf(pFileData,"BINARY\r\n");
	pFileData += strlen(pFileData);
	
	// Timemult<CR/LF>
	pFileData[0] = '\0';
	sprintf(pFileData,"1\r\n");
	pFileData += strlen(pFileData);

	// 1.2 整块写入文件
	strcpy(strConfigFileName,strFileName);
	strcat(strConfigFileName,".CFG");

	return DAE_WriteFile(strConfigFileName,(BYTE *)G_pDistFileBlock,pFileData-(char *)G_pDistFileBlock);
}

/************************************************************
	功能：生成录波数据文件
	参数: pDistBlock	-- 录波数据指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SaveDistDataFile(char	 *strFileName,BYTE *pDistData)
{
	char				strDataFileName[32];
	TDist_Disp			*pDistDisp;
	TAC_Disp			*pACDisp;
	BYTE				*pFileData;
	WORD				wACNum,wDCNum;
	DWORD				dwOffset,dwSampleTime;
	short				sSampleValue;
	STATUS				rc;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	pACDisp = G_pCfgTable->DBaseConfig.pACPara;
	if((pDistDisp == NULL)||(pACDisp == NULL))
		return DB_ERR_BASENULL;

	// 1.先创建数据文件
	
	// 2.生成配置文件 
	pFileData = (BYTE *)G_pDistFileBlock;
	pDistData += sizeof(TDistHead);
	
	dwOffset = 0;
	dwSampleTime = 0;

	wDCNum	= (pDistDisp->wDigitalChannelNum+15)/16;
	wACNum	= pDistDisp->wAnalogChannelNum;
			
	//从第一次采样开始逐次填充缓冲区
 	for(long i=0; i<pDistDisp->dwTotalSampleNum; i++)
	{
		// 2.1 序号
		pFileData[dwOffset++] = LLBYTE(i+1);
		pFileData[dwOffset++] = LHBYTE(i+1);
		pFileData[dwOffset++] = HLBYTE(i+1);
		pFileData[dwOffset++] = HHBYTE(i+1);

		// 2.2 采样时间
		pFileData[dwOffset++] = LLBYTE(dwSampleTime);
		pFileData[dwOffset++] = LHBYTE(dwSampleTime);
		pFileData[dwOffset++] = HLBYTE(dwSampleTime);
		pFileData[dwOffset++] = HHBYTE(dwSampleTime);
		//有可能造成累计误差!!!
		dwSampleTime += 1000000/pDistDisp->wSampleRate;

		// 2.3 依次填充模拟量数据
		for(int m=0;m<wACNum;m++)
		{
			sSampleValue = MAKEWORD(pDistData[m*2+1],pDistData[m*2]);
			//换算压缩倍率
			if(pACDisp[m].wCompressMultiple != 0)
				sSampleValue /= pACDisp[m].wCompressMultiple;
			
			pFileData[dwOffset++] = LOBYTE(sSampleValue);
			pFileData[dwOffset++] = HIBYTE(sSampleValue);
		}
		

		// 2.4 依次填充数字量数据
		pDistData += wACNum*2;

		for(int n=0;n<wDCNum;n++)
		{
			pFileData[dwOffset++] = pDistData[n*2+1];
			pFileData[dwOffset++] = pDistData[n*2];
		}
		
		pDistData += wDCNum*2;



//		memcpy(pFileData+dwOffset,pDistData+wACNum*2,wDCNum*2);
//		dwOffset += wDCNum*2;

//		pDistData += wACNum*2+wDCNum*2;
	}

	// 3. 整块写入文件
	strcpy(strDataFileName,strFileName);
	strcat(strDataFileName,".DAT");

//	TAbsTime	t1,t2;
//	ReadAbsTime(&t1);
	rc = DAE_WriteFile(strDataFileName,(BYTE *)G_pDistFileBlock,dwOffset);
//	ReadAbsTime(&t2);

//	AbsTimeSub(&t2, &t1);
//	ReportMsg("Write Dist_Data File Last %d MS",t2.Lo);
	
	return rc;
}

/************************************************************
	功能：根据索引删除录波文件
	参数: none
	返回：正确返回0，错误返回错误值。
************************************************************/
void DB_DeleteIndexDistFile(char *strFileName)
{
	char	strIndexFileName[DIST_INDEX_NAMELEN];
	char	strOldFileName[DIST_INDEX_NAMELEN];
	char	*pTempFileName;

	strIndexFileName[0] = 0;

	//避免越界
	strFileName[DIST_INDEX_NAMELEN-1] = 0;
	
	strcat(strIndexFileName,strFileName);
	
	if(pTempFileName=strstr(strIndexFileName,"INDEX_"))
	{
		//删除配置文件
		strcpy(strOldFileName,pTempFileName+6);
		strcat(strOldFileName,".CFG");
		FS_DelFile(strOldFileName,0);

		//删除数据文件
		strcpy(strOldFileName,pTempFileName+6);
		strcat(strOldFileName,".DAT");
		FS_DelFile(strOldFileName,0);
	}
}

/************************************************************
	功能：更新录波索引文件
	参数: none
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateDistIndex(char	 *strFileName)
{
	BYTE			*pFileData;
	TDistIndex		*pDistList;
	TDist_Disp		*pDistDisp;
	TSysTime		tSysTime;
	DWORD			dwFileLength,dwRtnLength;
	char			strIndexFileName[DIST_INDEX_NAMELEN];
	STATUS			rc;
	
	pDistDisp = G_pCfgTable->DBaseConfig.pDistPara;
	if(pDistDisp == NULL)
		return DB_ERR_BASENULL;

	pFileData = (BYTE *)G_pDistFileBlock;
	pDistList = (TDistIndex *)pFileData;
	
	// 1。打开录波索引文件
	dwFileLength = sizeof(TDistIndex)+DIST_INDEX_NAMELEN*DIST_CAPACITY_MAX;
	memset(pFileData,0x00,dwFileLength);
	
	rc = FS_ReadFile("DistList.ind",0,dwFileLength,pFileData,&dwRtnLength,0);
	if( (rc == FILE_ERROR)||(dwFileLength != dwRtnLength)||(pDistList->wMagic != DIST_INDEX_INIT))
//	rc = EEPROM_Read(EEPROM_DISTINDEX_START, dwFileLength, pFileData);
//   if( (rc == FILE_ERROR)||(pDistList->wMagic != DIST_INDEX_INIT))
    {
		// 索引文件出错
		LogError("DB_UpdateDistIndex",FILE_LINE,"\n Error on open DistList.ind!");
		pDistList->wMagic = DIST_INDEX_INIT;
		pDistList->nMax 		= pDistDisp->wRecordCapacity;
		pDistList->nHeadIndex	= 0;
		pDistList->nTailIndex	= 0;
    }
			
	// 3。更新录波文件表

	pFileData += sizeof(TDistIndex);
	
	// 3.1 如果录波容量变化，直接复位指针
	if((pDistDisp->wRecordCapacity) != pDistList->nMax)
	{
		//把多出的文件删除
		for(int i=pDistDisp->wRecordCapacity;i<DIST_CAPACITY_MAX;i++)
			DB_DeleteIndexDistFile((char *)pFileData+i*DIST_INDEX_NAMELEN);
		
		pDistList->nMax = pDistDisp->wRecordCapacity;
		pDistList->nHeadIndex = pDistList->nTailIndex = 0;
	}
	
	pFileData += pDistList->nTailIndex*DIST_INDEX_NAMELEN;

	// 3.2 删除当前空间的文件
	DB_DeleteIndexDistFile((char *)pFileData);
	
	// 3.3 填充新的内容
	memset(strIndexFileName,0,DIST_INDEX_NAMELEN);
	strcpy(strIndexFileName,"INDEX_");
	strcpy(strIndexFileName+6,strFileName);
	memcpy(pFileData,strIndexFileName,DIST_INDEX_NAMELEN);

	// 3.4 更新指针
	pDistList->nTailIndex++;
	pDistList->nTailIndex %= pDistList->nMax;
	if(pDistList->nHeadIndex == pDistList->nTailIndex)
	{
		pDistList->nHeadIndex++;
		pDistList->nHeadIndex %= pDistList->nMax;
	}	

	// 3.5 更新索引文件时间,BCD码
	ReadSystemTime(&tSysTime);
	tSysTime.Year 		%= 100;
	pDistList->Year 	= ((tSysTime.Year/10)<<4)+(tSysTime.Year%10);
	pDistList->Month	= ((tSysTime.Month/10)<<4)+(tSysTime.Month%10);
	pDistList->Day		= ((tSysTime.Day/10)<<4)+(tSysTime.Day%10);
	pDistList->Hour		= ((tSysTime.Hour/10)<<4)+(tSysTime.Hour%10);
	pDistList->Minute	= ((tSysTime.Minute/10)<<4)+(tSysTime.Minute%10);
	pDistList->Second	= ((tSysTime.Second/10)<<4)+(tSysTime.Second%10);
	pDistList->MSecondHi= tSysTime.MSecond/100;
	pDistList->MSecondLo= tSysTime.MSecond%100;
	pDistList->MSecondHi	= ((pDistList->MSecondHi/10)<<4)+(pDistList->MSecondHi%10);
	pDistList->MSecondLo	= ((pDistList->MSecondLo/10)<<4)+(pDistList->MSecondLo%10);
		
	// 4。写入文件
//	rc = EEPROM_Write(EEPROM_DISTINDEX_START, dwFileLength, G_pDistFileBlock);
//	if(rc != DB_ERR_OK)
//		LogError("DB_UpdateDistIndex",FILE_LINE,"\n Error on write DistList.ind!");
	
	return DAE_WriteFile("DistList.ind",(BYTE *)G_pDistFileBlock,dwFileLength);
}

/************************************************************
	功能：存储录波数据文件
	参数: none
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_SaveDistRecord()
{
	char				strFileName[32];
    CNode 				*pTempNode;
	BYTE				*pDistData;
	TDistHead			*pHead;
	static STATUS				rc;

	G_DistDataSaveLink.ToHead();
	
	pTempNode = G_DistDataSaveLink.Next();

	//判断链表是否为空链,链表为空链返回
	while(pTempNode != NULL)
    {
		pDistData = (BYTE *)pTempNode->pData;
		pHead = (TDistHead	*)pDistData;

		if(pHead->wStatus == DIST_STATUS_SAVE)
		{
			// 1。根据录波时间生成文件名
			rc = DB_GetDistFileName(&pHead->tTime,strFileName);

			// 2。更新文件索引
			rc = DB_UpdateDistIndex(strFileName);
			
			// 3。生成配置文件
			rc = DB_SaveDistConfFile(strFileName,&pHead->tTime,pHead->dwPrevSampleNum);
			
			// 4。生成数据文件
			rc = DB_SaveDistDataFile(strFileName,pDistData);
			
			// 5。链入空闲节点链表
			pHead->wStatus = DIST_STATUS_IDLE;
			// 5.1 删除待存盘节点
			G_DistDataSaveLink.RemoveNode(pTempNode);
			// 5.2 存入空闲节点链表
			G_DistDataFreeLink.AddTail(pTempNode);

			// 向所有规约任务发送录波变更消息
			for(int i=SYS_PORT_MAX;i<DAE_MAX_APP_NUM;i++)
			{
				if(G_pCfgTable->TaskApp[i].hTaskID != APP_TASKID_NULL)
					OS_PostMessage(G_pCfgTable->TaskApp[i].hTaskID, (WORD)SM_DISTEVENT, 0,0,0,0);
			}

			break;
		}
		
		pTempNode = G_DistDataSaveLink.Next();
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：更新保护数据文件
	参数：NONE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateRelayDataMap()
{
	BYTE					*pParaBuf;
	TRelayDataTable 		*pRelayDataTable;
	TFileMap				*pFileMap;		//保护数据文件镜像区指针	
	TDBaseConfigTable		*pDbase;
	STATUS					rc;
	DWORD					filelength;

	pDbase = &G_pCfgTable->DBaseConfig;
	pFileMap = pDbase->pRelayDataFileMap;

	// 1 读文件长度
	rc = DAE_GetFileLength(RELAYDATAFILE,&filelength);
	if( rc != DB_ERR_OK)
		return DB_ERR_FILE_READ;

	// 2 检查文件长度
	if(filelength > MAX_FILE_LEN)
		return DB_ERR_FILE_CHECK;
	if(pFileMap->nSize != ltobl(filelength))
		return DB_ERR_FILE_CHECK;
	
	// 3 取文件缓冲区
	pParaBuf = (BYTE *)OS_MemAlloc(filelength);
	if( pParaBuf == NULL )
		return DB_ERR_PTRNULL;

	// 4 读取文件
	rc = DAE_ReadFile(RELAYDATAFILE,pParaBuf,filelength);
	
	// 5 检查文件数据校验
	if( (rc != DB_ERR_OK)||(DAE_CheckSystemFile(pParaBuf,filelength) != DB_ERR_OK))
	{
		OS_MemFree(pParaBuf);
		return DB_ERR_FILE_CHECK;
	}

	// 检查文件结构
	pRelayDataTable = (TRelayDataTable	*)(pParaBuf+sizeof(TFileHead));
	if((pRelayDataTable->nSW != ltobs(pDbase->nSW))||\
		(pRelayDataTable->nSET != ltobs(pDbase->nSET))||\
		(pRelayDataTable->nSystemSET!= ltobs(pDbase->nSystemSET))||\
		(pRelayDataTable->nCF != ltobs(pDbase->nCF)))
	{
		OS_MemFree(pParaBuf);
		return DB_ERR_FILE_CHECK;
	}	
	
	// 6 更新RAM镜像区
	
	// 6.1 更新文件镜像表
	pFileMap->wFileType 	= 0;
	pFileMap->wFlag			|= FILEMAP_FLAGS_MODEND;
	pFileMap->dwFlashOff 	= NULL;

	memcpy(pFileMap->pRamData,pParaBuf,filelength);	

	pFileMap->nSize 		= ltobl(filelength);
	pFileMap->absTime.Lo	= ltobl(G_pCfgTable->SystemStatus.stTime.Lo);
	pFileMap->absTime.Hi	= ltobs(G_pCfgTable->SystemStatus.stTime.Hi);
	pFileMap->wCheck		= DAE_CheckSum((BYTE *)pFileMap,(BYTE *)&pFileMap->wCheck-(BYTE *)pFileMap);
	pFileMap->wCheck		= ltobs(pFileMap->wCheck);

	OS_MemFree(pParaBuf);
	return DB_ERR_OK;
}

/************************************************************
	功能：更新浮点定值
	参数：NONE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateSETFloatBase()
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	float 					*pDbaseSET;
	float 					*pMapSET;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	pDbaseSET = pDbase->pSETDataFloat;
	pMapSET = pDbase->tMapData.pSETData;
	num = pDbase->nSET;
	
	if((pDbaseSET == NULL)||(pMapSET == NULL))
		return DB_ERR_BASENULL;
	
	// 1.从RAM文件镜像中恢复
	if(!DB_CheckSETData(pMapSET,num))
	{
		// 2.从EEPROM文件中恢复
		if(DB_UpdateRelayDataMap() != DB_ERR_OK)
			return DB_ERR_FILE_CHECK;
	}
	
	// 2 重新检查
	if(!DB_CheckSETData(pMapSET,num))
	{
		DAE_SetRunningFlags(SS_EEPROM_SET_ERR);
		return DB_ERR_CHECK;
	}
	
	// 3 刷新数据

	// 3.1 字节转换
	for(i=0;i<num;i++)
		pDbaseSET[i] = CONVERTFLOAT(pMapSET[i+pDbase->nSETAreaCur*(num+1)]);
	
	// 3.2 重新计算校验
	DAE_MakeCheckSum((BYTE *)pMapSET,sizeof(float)*num);

	// 4.发送消息，记录更改
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(SET_VALUE_ALL,SET_AREA_CURRENT), 0, 0);

	DB_Register_SET(SET_AREA_CURRENT,SET_VALUE_ALL);

	// 5.再更新定点定值
	DB_UpdateSETFixDataBase();

	return DB_ERR_OK;
}

/************************************************************
	功能：更新辅助定值
	参数：NONE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateSystemSETBase()
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	float 					*pDbaseSET;
	float 					*pMapSET;
	
	pDbase 		= &G_pCfgTable->DBaseConfig;
	pDbaseSET 	= pDbase->pSystemSETData;
	pMapSET 	= pDbase->tMapData.pSystemSETData;
	num 		= pDbase->nSystemSET;
	
	if((pDbaseSET == NULL)||(pMapSET == NULL))
		return DB_ERR_BASENULL;
	
	// 1.从RAM文件镜像中恢复
	if(!DB_CheckSETData(pMapSET,num))
	{
		// 2.从EEPROM文件中恢复
		if(DB_UpdateRelayDataMap() != DB_ERR_OK)
			return DB_ERR_FILE_CHECK;
	}
	
	// 2 重新检查
	if(!DB_CheckSETData(pMapSET,num))
	{
		DAE_SetRunningFlags(SS_EEPROM_SYSSET_ERR);
		return DB_ERR_CHECK;
	}
	
	// 3 刷新数据

	// 3.1 字节转换
	for(i=0;i<num;i++)
		pDbaseSET[i] = CONVERTFLOAT(pMapSET[i]);
	
	// 3.2 重新计算校验
	DAE_MakeCheckSum((BYTE *)pMapSET,sizeof(float)*num);

	// 4.发送消息，记录更改
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, SET_VALUE_ALL, 0, 0);

	DB_Register_SET(SET_AREANO_INVALID,SET_VALUE_ALL);

	return DB_ERR_OK;
}

/************************************************************
	功能：更新定点定值
	参数：NONE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateSETFixDataBase()
{
	WORD 					num,i;
	TDBaseConfigTable		*pDbase;
	float 					*pSETfloat;
	long 					*pSETFix;
	
	pDbase = &G_pCfgTable->DBaseConfig;
	pSETfloat = pDbase->pSETDataFloat;
	pSETFix = pDbase->pSETDataFix;
	num = pDbase->nSET;

	if((pSETfloat == NULL)||(pSETFix == NULL))
		return DB_ERR_BASENULL;
	
	//按照小数位数初始化定点定值
	for(i=0;i<num;i++)
	{
		//配置字特殊
		ConvertSETFloat2Fix(i,pSETfloat[i],&pSETFix[i]);
	}
	
	// 重新计算校验
	DAE_MakeCheckSum((BYTE *)pSETFix,sizeof(float)*num);
	
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SET, MAKELONG(SET_VALUE_ALL,SET_AREA_CURRENT), 0, 0);

	DB_Register_SET(DB_GetCurrentSETAreaNo(),SET_VALUE_ALL);

	return DB_ERR_OK;
}

/************************************************************
	功能：升级系数数据。
	参数: NONE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateCFBase()
{
	WORD 					num,i;
	long					*pData;
	long 					*pCF;
	STATUS					rc;
	
	num = G_pCfgTable->DBaseConfig.nCF;
	pCF = G_pCfgTable->DBaseConfig.pCFData;
	pData = G_pCfgTable->DBaseConfig.tModData.pCFData;
	
	//如果数据指针非空，选择合适的系数区
	if(pData != NULL)
	{
		if(DB_Is1ASystem())
			rc = EEPROM_Read(EEPROM_RELAY_CFBASE+(CF_COUNT+1)*sizeof(long),(CF_COUNT+1)*sizeof(long),pData);
		else
			rc = EEPROM_Read(EEPROM_RELAY_CFBASE,(CF_COUNT+1)*sizeof(long),pData);

		if(rc != DB_ERR_OK)
		{
			LogError("UpdateCFBase",FILE_LINE,"Read eerpom data Error!!!");
			pData = NULL;
		}
	}

	// 重新检查
	if(!DB_CheckCFData(pData,num))
	{
		LogError("UpdateCFBase",FILE_LINE,"CF data Error!!!");
		DAE_SetRunningFlags(SS_EEPROM_CF_ERR);
		pData = NULL;
	}
	else
		DAE_ClearRunningFlags(SS_EEPROM_CF_ERR);
		
	//再检查系数有效性

	/*初始化参数部分*/
	for(i=0;i<num;i++)
	{
		/*初始化缺省参数*/
		if(pData == NULL)
		{
			if(DB_Is1ASystem())
				pCF[i] = CFDefaultVal2[i]; 
			else
				pCF[i] = CFDefaultVal1[i]; 
		}
		else
		{
			//先进行字序转换
			pCF[i] = pData[i];
		}
	}
	
	// 计算校验
	DAE_MakeCheckSum((BYTE *)pCF,sizeof(float)*num);

	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_CF, CF_VALUE_ALL, 0, 0);

	DB_Register_CF(CF_VALUE_ALL);

	// 再更新定点定值
	DB_UpdateSETFixDataBase();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：更新软压板
	参数: NONE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_UpdateSWBase()
{
	WORD 					num,i;
	DWORD					*pData;
	DWORD 					*pSW;
	
	num = G_pCfgTable->DBaseConfig.nSW;
	pSW = G_pCfgTable->DBaseConfig.pSWData;
	pData = G_pCfgTable->DBaseConfig.tMapData.pSWData;

	// 1.从RAM文件镜像中恢复
	if(!DB_CheckSWData(pData,num))
	{
		// 2.从EEPROM文件中恢复
		if(DB_UpdateRelayDataMap() != DB_ERR_OK)
			return DB_ERR_FILE_CHECK;
	}
	
	//再检查系数有效性
	if(!DB_CheckSWData(pData,num))
	{
		LogError("DB_UpdateSWBase",FILE_LINE,"SW data Error!!!");
		DAE_SetRunningFlags(SS_EEPROM_SW_ERR);
		return DB_ERR_CHECK;
	}
	

	/*初始化参数部分*/
	for(i=0;i<((num+31)/32)*2;i++)
		pSW[i] = ltobl(pData[i]);
	
	OS_PostMessage(g_hRelayTaskID, SM_RELAYDATA, MISC_CLASS_SW, 0xFFFF, 0, 0);
	
	DB_Register_SW(0xFFFF,0);
	
	return DB_ERR_OK;
}

STATUS DB_UpdateRelayData()
{
	//将保护数据文件内容从FLASH文件导入EEPROM
	TFileHead			*filehead;
	BYTE				*pParaBuf = NULL;
	DWORD				dwFileLength,dwRtnLength;
	STATUS				rc;
	
	// 1 读取文件
#ifdef _RELAY_DATA_FILE_
	rc = DAE_GetFlashFileLength(RELAYDATAFILE,&dwFileLength);
	if( rc == DB_ERR_OK)
	{
		if(dwFileLength > MAX_FILE_LEN)
			dwFileLength = MAX_FILE_LEN;
		
		//分配临时用的文件缓冲区
		pParaBuf = (BYTE *)OS_MemAlloc(dwFileLength);
		
		if( pParaBuf != NULL )
		{
			memset(pParaBuf,0,dwFileLength);
			rc = DAE_ReadFlashFile(RELAYDATAFILE,pParaBuf,dwFileLength);
		}
		else
		{
			LogError("UpdateRelayData",FILE_LINE,"Allocate memory for read parameter file error!");
			rc = DB_ERR_MEM_ALLOC;
		}
	}
	
	// 2 写入EEPROM
	if(rc != DB_ERR_OK)
	{
		LogError("UpdateRelayData",FILE_LINE,"Read Relay Data File ERR!!!");		
		OS_MemFree(pParaBuf);
		return rc;
	}

	rc = DAE_CheckSystemFile(pParaBuf,dwFileLength);
	
	if(rc == DB_ERR_OK)	
	{
		rc = EEPROM_Write(EEPROM_RELAY_DATA,dwFileLength,pParaBuf);
		
		if(rc == DB_ERR_OK)
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File OK!!!");
		else
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data write EEPROM error!!!");
	}
	else
	{
		LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File check ERROR!!!");
	}
	
	OS_MemFree(pParaBuf);

#else
	dwFileLength = 0;
	pParaBuf = (BYTE *)RelayDataInit;
	
	DAE_GetFileLength(RELAYDATAFILE, &dwFileLength);
	if(dwFileLength != MAKEDWORD(pParaBuf[3],pParaBuf[2],pParaBuf[1],pParaBuf[0]))
	{
		LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File length ERROR!!!");
		rc = DB_ERR_FILE_CHECK;
	}
	
	rc = DAE_CheckSystemFile(pParaBuf,dwFileLength);
	
	if(rc == DB_ERR_OK)	
	{
		rc = EEPROM_Write(EEPROM_RELAY_DATA,dwFileLength,pParaBuf);
		
		if(rc == DB_ERR_OK)
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File OK!!!");
		else
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data write EEPROM error!!!");
	}
	else
	{
		LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File check ERROR!!!");
	}
#endif

	return rc;
}

STATUS DB_UpdateCFData()
{
	//将保护数据文件内容从FLASH文件导入EEPROM
	TFileHead			*filehead;
    TRelayDataTable		*pRelayDataTable;
	BYTE				*pParaBuf = NULL;
	DWORD				dwFileLength,dwRtnLength;
	DWORD				dwCFPos,dwCFLen;
	STATUS				rc = DB_ERR_FAILURE;
	
	// 1 读取文件长度
	do
	{
#ifdef _RELAY_DATA_FILE_
		rc = DAE_GetFileLength(RELAYDATAFILE,&dwFileLength);
		if( rc != DB_ERR_OK)
		{
			ReportMsg("Read Relay Data File fail!!!");
			break;
		}
		
		if(dwFileLength > EEPROM_RELAY_DATASIZE)
		{
			ReportMsg("Relay Data File length overflow!!!");
			break;
		}
			
		// 2.分配临时用的文件缓冲区
		pParaBuf = (BYTE *)OS_MemAlloc(dwFileLength);
			
		if( pParaBuf == NULL )
		{
			ReportMsg("Allocate memory for read parameter file fail!");
			break;
		}
		
		// 3 读取文件内容
		rc = DAE_ReadFlashFile(RELAYDATAFILE,pParaBuf,dwFileLength);
		if(rc != DB_ERR_OK)
		{
			ReportMsg("Read Relay Data File fail!!!");
			break;
		}

#else
		dwFileLength = 0;
		pParaBuf = (BYTE *)RelayDataInit;
				
		DAE_GetFileLength(RELAYDATAFILE, &dwFileLength);
		if(dwFileLength != MAKEDWORD(pParaBuf[3],pParaBuf[2],pParaBuf[1],pParaBuf[0]))
		{
			LogError("UpdateRelayData",FILE_LINE,"Update Relay Data File length ERROR!!!");
			rc = DB_ERR_FILE_CHECK;
		}
#endif
		
		// 3 检查文件合法性
		rc = DAE_CheckSystemFile(pParaBuf,dwFileLength);
		if(rc != DB_ERR_OK) 
		{
			ReportMsg("Check Relay Data File fail!!!");
			break;
		}
		
		// 4 写入EEPROM
		filehead = (TFileHead *)pParaBuf;
		pRelayDataTable = (TRelayDataTable *)( filehead + 1 );
		
		dwCFPos  = sizeof(TFileHead)+sizeof(TRelayDataTable);
		dwCFPos += (pRelayDataTable->nSW+31)/32*2*sizeof(DWORD);
		dwCFPos += (pRelayDataTable->nSET+1)*pRelayDataTable->nSETArea*sizeof(float);
		dwCFPos += (pRelayDataTable->nSystemSET+1)*sizeof(float);
		dwCFLen = (pRelayDataTable->nCF+1)*sizeof(long)*2;
		
		//	4.1 系数长度检查
		if(dwCFLen >= EEPROM_RELAY_CFSIZE)
		{
			ReportMsg("Relay file data fail!!!");
			rc = DB_ERR_FAILURE;
			break;
		}
		
		// 4.2 系数校验检查
		if(!DAE_IsCheckSumOK(pParaBuf+dwCFPos, pRelayDataTable->nCF*sizeof(long)))
		{
			ReportMsg("Relay file data fail!!!");
			rc = DB_ERR_FAILURE;
			break;
		}
		
		if(!DAE_IsCheckSumOK(pParaBuf+dwCFPos+(pRelayDataTable->nCF+1)*sizeof(long), pRelayDataTable->nCF*sizeof(long)))
		{
			ReportMsg("Relay file data fail!!!");
			rc = DB_ERR_FAILURE;
			break;
		}
		
		rc = EEPROM_Write(EEPROM_RELAY_CFBASE,dwCFLen,pParaBuf+dwCFPos);
		
		if(rc == DB_ERR_OK)
			ReportMsg("Update CF data success!!!");
		else
			ReportMsg("Update CF data write eeprom error!!!");
	}while(0);
	
	//	释放临时空间
#ifdef _RELAY_DATA_FILE_
	if( pParaBuf != NULL )
		OS_MemFree(pParaBuf);
#endif

	return rc;
}


