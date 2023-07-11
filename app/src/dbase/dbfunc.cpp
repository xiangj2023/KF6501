/********************************************************************************************************/
/*																										*/
/*        						天津凯发电气股份有限公司												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Dbfunc.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义数据库中相关的接口函数																*/
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
/*	DB_GetAINum()					读取遥测个数														*/
/*	DB_ReadAI()						从遥测库中取一个遥测数据											*/
/*	DB_WriteAI()					向遥测库中写一个遥测数据											*/
/*	DB_GetDispOfAI()				读取一个遥测的类型描述												*/
/*	DB_GetBINum()					读取遥信个数														*/
/*	DB_ReadBI()						从遥信库中取一个遥信数据											*/
/*	DB_WriteBI()					向遥信库中写一个遥信数据											*/
/*	DB_GetCINum()					读取电度个数														*/
/*	DB_ReadCI()						从电度库中取一个电度数据											*/
/*	DB_WriteCI()					向电度库中写一个电度数据											*/
/* 	DB_GetBONum()					读取遥控个数														*/
/*	DB_BOSelect()					遥控预置操作														*/
/*	DB_BOSelectEcho()				遥控预置响应														*/
/*	DB_BOExec()						遥控执行操作														*/
/*	DB_BOExecEcho()					遥控执行响应														*/
/*	DB_BODirect()					遥控直接执行操作													*/
/*	DB_BODirectEcho()				遥控直接执行响应													*/
/*	DB_AOSelcet()					遥调预置操作														*/
/*	DB_AOSelectEcho()				遥调预置响应														*/
/*	DB_AOExec()						遥调执行操作														*/
/*	DB_AOExecEcho()					遥调执行响应														*/
/*	DB_AODirect()					遥调直接执行操作													*/
/*	DB_AODirectEcho()				遥调直接执行响应													*/
/*	DB_GetBIENum()					获取未读取变位遥信数据个数											*/
/*	DB_BIEIsEmpty()					变位遥信缓冲区是否为空												*/
/*	DB_ReadBIE()					从数据库中读取一条变位遥信数据										*/
/*	DB_WriteBIE()					向数据库中写入一条变位遥信数据										*/
/*	DB_GetBIEHead()					取变位遥信缓冲区头指针												*/
/*	DB_GetBIERead()					取变位遥信缓冲区读指针												*/
/*	DB_IncBIERead()					递增变位遥信缓冲区读指针											*/
/*	DB_GetSOENum()					获取未读取SOE数据个数												*/
/*	DB_SOEIsEmpty()					SOE缓冲区是否为空													*/
/*	DB_ReadSOE()					从数据库中读取一条SOE数据											*/
/*	DB_WriteSOE()					向数据库中写入一条SOE数据											*/
/*	DB_GetSOEHead()					取SOE缓冲区头指针													*/
/*	DB_GetSOERead()					取SOE缓冲区读指针													*/
/*	DB_IncSOERead()					递增SOE缓冲区读指针													*/
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

extern TBasicConfig	G_tBasicConfig;
extern TSysConfigTable	*G_pCfgTable;
extern HANDLE			g_hBOTaskID;
extern HANDLE			g_hLCDTaskID;
extern HANDLE			g_hRelayTaskID;

static CLink	G_BORegisterList;		//遥控注册表

/************************************************************
	功能：注册一路遥控，用于主站类规约接管遥控
	参数：nBO,		--  遥控序号
	返回：正确返回DB_ERR_OK，错误返回错误值。
************************************************************/
STATUS DB_RegisterBOMsg(WORD nBO)
{
	CNode	*pNode;
	TBORegisterInfo	*pBO;

	// 1.2.1 分配空间
	pBO = (TBORegisterInfo *)OS_MemAlloc(sizeof(TBORegisterInfo));
	ASSERT(pBO != NULL);
	if(pBO == NULL)
	{
		LogError("RegisterBO",FILE_LINE,"Malloc BO register node error!!!");
		return DB_ERR_MEM_ALLOC;
	}

	if(nBO >= DB_GetBONum())
	{
		LogError("RegisterBO",FILE_LINE,"Bo no is overflow!!!");
		return DB_ERR_MEM_ALLOC;
	}
		
	// 1.2.2 初始化数据节点
    pBO->hTaskID = RtosGetCurrentTaskId();
	pBO->nBO = nBO;
	
	// 1.2.3 入链
	pNode = (CNode *)OS_MemAlloc(sizeof(CNode));
	pNode->pData = pBO;
	G_BORegisterList.AddTail(pNode);

	return DB_ERR_OK;
}

/************************************************************
	功能：检索遥控是否被注册
	参数：nBO,		--  遥控序号
	返回：正确返回遥控注册信息，错误返回空指针
************************************************************/
TBORegisterInfo *DB_SearchRegisterBO(WORD nBO)
{
	TBORegisterInfo		*pBO;
    CNode 				*pTempNode;
	
	pTempNode = G_BORegisterList.GetHead();
	//判断链表是否为空链,链表为空链返回
	if(pTempNode == NULL)
		return NULL;

	do
    {
		pBO = (TBORegisterInfo	*)pTempNode->pData;
        if ( pBO->nBO == nBO)
        {
            return pBO;
        }

		//遍历下个节点
        pTempNode = pTempNode->Next;
    }while((pTempNode)&&(pTempNode != G_BORegisterList.GetHead()));

	return NULL;
}

float _ConvertFloat(float fIn)
{
	BYTE	*pTemp,byTemp;
	
	pTemp = (BYTE *)&fIn;

	byTemp = pTemp[0];
	pTemp[0] = pTemp[3];
	pTemp[3] = byTemp;

	byTemp = pTemp[1];
	pTemp[1] = pTemp[2];
	pTemp[2] = byTemp;

	return *(float *)pTemp;
}

/*-----------------------------------------------------------------------------

            以下函数是内部函数，用于取设备中各分类数据库的指针

-----------------------------------------------------------------------------*/


TRunTimeAI	*DB_GetAIBase()
{
    return G_pCfgTable->DBaseConfig.pAI;
}
TRunTimeBI	*DB_GetBIBase()
{
    return G_pCfgTable->DBaseConfig.pBI;
}
TRunTimeCI	*DB_GetCIBase()
{
    return G_pCfgTable->DBaseConfig.pCI;
}
TRunTimeBO	*DB_GetBOBase()
{
    return G_pCfgTable->DBaseConfig.pBO;
}

/************************************************************
	功能：读取遥测个数
	参数：无
	返回：遥测个数
************************************************************/
WORD	DB_GetAINum()
{
	return G_pCfgTable->DBaseConfig.nAI;
}

/************************************************************
	功能：从遥测库中取一个遥测数据
	参数：no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadAI( WORD no, long	*retval)
{
	TRunTimeAI		*pAI;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pAI = G_pCfgTable->DBaseConfig.pAI;
	num = G_pCfgTable->DBaseConfig.nAI;
	
	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_ReadAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pAI==NULL )
    {
        LogError("DB_ReadAI",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }
	
	*retval = pAI[no].lValue;

	return DB_ERR_OK;
}

/************************************************************
	功能：从遥测库中取遥测浮点数据。
	参数：no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadFloatAI( WORD no, float *retval )
{
	TRunTimeAI		*pAI;
	TCF_Disp		tCFDisp;
	long			lCFValue = 1;
	long			lCFRatio = 1;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pAI = G_pCfgTable->DBaseConfig.pAI;
	num = G_pCfgTable->DBaseConfig.nAI;
	
	// 1. 检查序号
	if(no >= num)
	{
		LogError("DB_ReadFloatAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 检查指针
    if( pAI == NULL )
    {
        LogError("DB_ReadFloatAI",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }
	
	// 3. 将读出的值进行换算
	
	//转换数值为浮点格式
	if((DB_GetCFofAI(no, &lCFValue) != DB_ERR_OK)||(lCFValue == 0))
	{
		lCFValue = 1;
		LogError("DB_ReadFloatAI",FILE_LINE,"InValid CF !!!");
	}
	
	if((DB_GetCFDispofAI(no, &tCFDisp) != DB_ERR_OK)||(tCFDisp.wCof == 0))
	{
		lCFRatio = 1;
		LogError("DB_ReadFloatAI",FILE_LINE,"InValid COF !!!");
	}
	else
		lCFRatio = tCFDisp.wCof;
	

	*retval = (float)(pAI[no].lValue*lCFRatio)/lCFValue;	
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从遥测库中取二次侧遥测数据。
	参数：no,		--  序号
		  retval	--  返回的值
			sUnit	  --  单位
			nDec	  --  小数点位数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadSecondAI( WORD no, long *retval,char *sUnit, BYTE *nDec)
{
	TAIPara			tAIPara;
	float			fValue;
    STATUS			rc;
	
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	// 1. 读取二次测量值浮点数
	rc = DB_ReadFloatAI(no,&fValue);
	if( rc != DB_ERR_OK)
        return rc;
	
	// 2. 按照二次值的小数位数保存定点数
	if(DB_GetAIPara(no,&tAIPara) != DB_ERR_OK)											
	{
        LogError("DB_ReadSecondAI",FILE_LINE,"The AI Type is Error!");
		return (long)(fValue*1000);
	}

	if(nDec != NULL)
		*nDec = tAIPara.nSecDec;

	if(sUnit != NULL)
	{
		memcpy(sUnit,tAIPara.strSecUnit,6);
		sUnit[5] = '\0';
	}
	
	switch(tAIPara.nSecDec)
	{
	case 0: *retval = (long)fValue; break;	
	case 1: *retval = (long)(fValue*10); break;	
	case 2: *retval = (long)(fValue*100); break;	
	case 3: *retval = (long)(fValue*1000); break;	
	case 4: *retval = (long)(fValue*10000); break;	
	default: *retval = (long)(fValue*100000); break;	
	}

	// 不在线时直接返回当前值
	if( !(G_pCfgTable->DBaseConfig.pAI[no].wStatus & AI_STATUS_ONLINE) )
		*retval = G_pCfgTable->DBaseConfig.pAI[no].lValue;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从遥测库中取一次侧遥测数据。
	参数：no,		--  序号
		  retval	--  返回的值
		  sUnit		--  单位
		  nDec		--	小数点位数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadFirstAI( WORD no, long *retval, char *sUnit, BYTE *nDec)
{
	TAIPara			tAIPara;
	float			fAIValue,fAbsValue;
	float			fRTValue;
	long			lRate;
    STATUS			rc;
	
    if( retval == NULL )
        return DB_ERR_BUFNULL;
		
	// 1. 读取二次测量值浮点数
	rc = DB_ReadFloatAI(no,&fAIValue);
	if( rc != DB_ERR_OK)
        return rc;
	
	// 2。读取保护测量值描述
	if(DB_GetAIPara(no,&tAIPara) == DB_ERR_OK)											
	{
		// 3。乘上变比
		rc = DB_GetRTValue(tAIPara.wRatioIndex, &fRTValue);
		if(rc == DB_ERR_OK)
			fAIValue *= fRTValue;
		else
	        LogError("DB_ReadFirstAI",FILE_LINE,"Read AI RT Error!");

		// 4.判定范围
		if(fAIValue < 0)
			fAbsValue = fAIValue*(-1);
		else
			fAbsValue = fAIValue;
		
		if(fAbsValue < tAIPara.lDispLimit12)
		{
			if(nDec != NULL)
				*nDec = tAIPara.nFirDec1;
			lRate = 0;
			if(sUnit != NULL)
				memcpy(sUnit,tAIPara.strFirUnit1,6);
		}
		else if(fAbsValue < tAIPara.lDispLimit23)
		{
			if(nDec != NULL)
				*nDec = tAIPara.nFirDec2;
			lRate = tAIPara.nDataRate2; 
			if(sUnit != NULL)
				memcpy(sUnit,tAIPara.strFirUnit2,6);
		}
		else
		{
			if(nDec != NULL)
				*nDec = tAIPara.nFirDec3;
			lRate = tAIPara.nDataRate3; 
			if(sUnit != NULL)
				memcpy(sUnit,tAIPara.strFirUnit3,6);
		}	
	}
	else
	{
		//缺省放大1000倍显示
        LogError("DB_ReadFirstAI",FILE_LINE,"Read AI para Error!");
		if(sUnit != NULL)
			sUnit[0] = '\0';
		
		if(nDec != NULL)
			*nDec = 3;
		lRate = 0;
	}

	if(sUnit != NULL)
		sUnit[5] = '\0';
	
	// 5. 按照一次值的小数位数保存定点数
	switch(lRate)
	{
	case 1: fAbsValue /= 10; break;	
	case 2: fAbsValue /= 100; break;	
	case 3: fAbsValue /= 1000; break;	
	case 6: fAbsValue /= 1000000; break;	
	default:  break;	
	}

	switch(*nDec)
	{
	case 0: fAbsValue = fAbsValue; break;	
	case 1: fAbsValue = (fAbsValue*10+0.5); break;	
	case 2: fAbsValue = (fAbsValue*100+0.5); break;	
	case 3: fAbsValue = (fAbsValue*1000+0.5); break;	
	case 4: fAbsValue = (fAbsValue*10000+0.5); break;	
	default: fAbsValue = (fAbsValue*100000+0.5); break;	
	}

	//根据符号返回
	if(fAIValue < 0)
		*retval = (long)fAbsValue*(-1);
	else
		*retval = (long)fAbsValue;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向遥测库中写一个遥测数据
	参数：no,		--  遥测的序号
		  value,	--  遥测值
	     mode		--  写入模式 	DB_WRITE_COMPEL：强制置数 DB_WRITE_UNCOMPEL：取消强制置数数		
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteAI(WORD no, long value, DWORD mode )
{
	TRunTimeAI		*pAI;
    WORD 			num;
               
	pAI = G_pCfgTable->DBaseConfig.pAI;
	num = G_pCfgTable->DBaseConfig.nAI;
	
	// 检查读取个数
	if(no >= num)
	{
		return DB_ERR_NO;
	}

    if( pAI==NULL )
    {
        LogError("DB_WriteAI",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		// 当处于置数模式下,设置远方置数标志,清除在线标志,更新数据
		pAI[no].wStatus &= ~AI_STATUS_ONLINE;
		pAI[no].wStatus |= AI_STATUS_MODIFY;
		pAI[no].lValue = value;

		DB_Register_SetData(MISC_CLASS_MODIAI, no, value);
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// 当处于取消置数模式下,设置在线标志,清除远方置数标志
		pAI[no].wStatus |= AI_STATUS_ONLINE;
		pAI[no].wStatus &= ~AI_STATUS_MODIFY;
		pAI[no].lValue = value;
	}
	else if( pAI[no].wStatus & AI_STATUS_ONLINE )
	{
		pAI[no].lValue = value;
	}

	pAI[no].wStatus &= ~AI_STATUS_RESET;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：读取遥信个数
	参数：无
	返回：遥信个数
************************************************************/
WORD	DB_GetBINum()
{
	return G_pCfgTable->DBaseConfig.nBI;
}

/************************************************************
	功能：向遥信库中写一个遥信数据
	参数： 
	      no,		--  遥信的序号
		  value,	--  遥信值		BI_OPEN BI_CLOSE
	     mode		--  写入模式 	DB_COMPEL：强制置数	DB_UNCOMPEL：取消强制置数
	返回：正确返回0，错误返回错误值。
************************************************************/
/*
STATUS DB_WriteBI(WORD no, BYTE  value, DWORD mode)
{
	TRunTimeBI		*pBI;
    WORD 			num;
               
	pBI = G_pCfgTable->DBaseConfig.pBI;
	num = G_pCfgTable->DBaseConfig.nBI;
	
	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_WriteBI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI==NULL )
    {
        LogError("DB_WriteBI",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		TBurstBI tBIE;
		TEventBI tSOE;
		
		// 当处于置数模式下,设置远方置数标志,清除在线标志,更新数据
		pBI[no].bStatus &= ~BI_STATUS_ONLINE;
		pBI[no].bStatus |= BI_STATUS_MODIFY;

		// 记忆旧状态
		if(pBI[no].bStatus&0x80)
			pBI[no].bStatus |= (0x01<<6);
		else
			pBI[no].bStatus &= ~(0x01<<6);

		// 更改新状态
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= (0x01<<7);
		else
			pBI[no].bStatus &= ~(0x01<<7);

		tBIE.wNo = no;
		tBIE.bStatus = value;
		DB_WriteBIE(&tBIE);
		
		tSOE.wNo = no;
		tSOE.bStatus = value;
		ReadAbsTime(&tSOE.stTime);
		DB_WriteSOE(&tSOE);
		
		DB_Register_SetData(MISC_CLASS_MODIBI, no, value);
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// 当处于取消置数模式下,设置在线标志,清除远方置数标志
		pBI[no].bStatus |= AI_STATUS_ONLINE;
		pBI[no].bStatus &= ~AI_STATUS_MODIFY;
		// 恢复旧状态
		if(pBI[no].bStatus&0x40)
			pBI[no].bStatus |= (0x01<<7);
		else
			pBI[no].bStatus &= ~(0x01<<7);
	}
	else if( pBI[no].bStatus & AI_STATUS_ONLINE )
	{
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;
	}

	pBI[no].bStatus &= ~BI_STATUS_RESET;
		
	return DB_ERR_OK;
}
*/
STATUS DB_WriteBI(WORD no, BYTE  value, DWORD mode)
{
	TRunTimeBI		*pBI;
    WORD 			num;
               
	pBI = G_pCfgTable->DBaseConfig.pBI;
	num = G_pCfgTable->DBaseConfig.nBI;
	
	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_WriteBI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI==NULL )
    {
        LogError("DB_WriteBI",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		TBurstBI tBIE;
		TEventBI tSOE;
		
		// 当处于置数模式下,设置远方置数标志,清除在线标志,更新数据
		pBI[no].bStatus &= ~BI_STATUS_ONLINE;
		pBI[no].bStatus |= BI_STATUS_MODIFY;
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;

		tBIE.wNo = no;
		tBIE.bStatus = value;
		DB_WriteBIE(&tBIE);
		
		tSOE.wNo = no;
		tSOE.bStatus = value;
		ReadAbsTime(&tSOE.stTime);
		DB_WriteSOE(&tSOE);
		
		DB_Register_SetData(MISC_CLASS_MODIBI, no, value);
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// 当处于取消置数模式下,设置在线标志,清除远方置数标志
		pBI[no].bStatus |= AI_STATUS_ONLINE;
		pBI[no].bStatus &= ~AI_STATUS_MODIFY;
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;
	}
	else if( pBI[no].bStatus & AI_STATUS_ONLINE )
	{
		if(value == DB_BI_CLOSE)
			pBI[no].bStatus |= 0x80;
		else
			pBI[no].bStatus &= 0x7F;
	}

	pBI[no].bStatus &= ~BI_STATUS_RESET;
		
	return DB_ERR_OK;
}

/************************************************************
	功能：从遥信库中取一个带品质描述的遥信
	参数： 
	      no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadBIWithQuality(WORD no, BYTE *retval)
{
	TRunTimeBI		*pBI;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pBI = G_pCfgTable->DBaseConfig.pBI;
	num = G_pCfgTable->DBaseConfig.nBI;
	
	// 检查读取个数
	if(no >= num)
	{
		//LogError("DB_ReadBI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI==NULL )
    {
        LogError("DB_ReadBI",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	*retval = pBI[no].bStatus;

	return DB_ERR_OK;
}

/************************************************************
	功能：从遥信库中取一个遥信状态
	参数： 
	      no,		--  序号
		  retval	--  返回的值 BI_OPEN BI_CLOSE
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadBI(WORD no, BYTE *retval)
{
	DB_ReadBIWithQuality(no,retval);

	if((*retval)&0x80)
		*retval = DB_BI_CLOSE;
	else
		*retval = DB_BI_OPEN;

	return DB_ERR_OK;
}

/*------------------------------------------------------------------------------------------
	功  能：从设备中取一个字节遥信（小于等于8个遥信）数据
	参数： 
	 	dev,     	--  设备名,没用和主处理器兼容
	  	start,    	--  起始序号
	 	retval   	--  返回的值,
         mode      	--  遥信字节中遥信的排列顺序。
                         DB_BI_BYTE18:遥信从D0排到D7
                         DB_BI_BYTE81:遥信从D7排到D0
	返回：正确返回0，错误返回错误值。
-------------------------------------------------------------------------------------------*/
STATUS DB_ReadBIByte(WORD start, BYTE *retval, WORD mode)
{
	STATUS		status;
    BYTE	 	temp;

    if( retval == NULL )
        return DB_ERR_BUFNULL;

	*retval = 0;
	
    for(int i=0; i<8; i++ )
    {
        if( mode == DB_BI_BYTE81)
            status = DB_ReadBI(start+i, &temp);
        else
            status = DB_ReadBI(start+7-i, &temp);

        if( status != DB_ERR_OK )
            return status;

		if(temp == DB_BI_CLOSE)
			*retval |= 0x01<<i;
		else
			*retval &= ~(0x01<<i);
    }
	
    return DB_ERR_OK;
}

/************************************************************
	功能：读取电度个数
	参数：无
	返回：电度个数
************************************************************/
WORD	DB_GetCINum()
{
	return G_pCfgTable->DBaseConfig.nCI;
}

/************************************************************
	功能：向电度库中累加一个电度数据
	参数： 
	      no,		--  电度序号
		  value,	--  值，包含两位小数的电度整数值。如:123.45，应该输入12345
	     mode		--  写入模式 	DB_COMPEL：强制置数	DB_UNCOMPEL：取消强制置数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteCI(WORD no, DWORD value, DWORD mode )
{
	TRunTimeCI		*pCI;
	TCIPara			tPara;
	STATUS			rc;
    WORD 			num;
               
	pCI = G_pCfgTable->DBaseConfig.pCI;
	num = G_pCfgTable->DBaseConfig.nCI;

	// 读脉冲常数
	rc = DB_GetCIPara(no,&tPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_WriteCI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCI==NULL )
    {
        LogError("DB_WriteCI",FILE_LINE,"The CI base is null!");
        return DB_ERR_BASENULL;
    }

	if( mode == DB_WRITE_COMPEL )
	{
		// 当处于置数模式下,设置远方置数标志,清除在线标志,更新数据
		pCI[no].bStatus &= ~CI_STATUS_ONLINE;
		pCI[no].bStatus |= CI_STATUS_MODIFY;
		
		if(tPara.wControl&CI_ATTR_PULSE)
			value = (DWORD)((QWORD)value*tPara.dwConst/100); 		//电度精度调整为0.001度   同时考虑溢出问题  yanxs 20140405
		
		pCI[no].dwValue = value;
	}
	else if( mode==DB_WRITE_UNCOMPEL )
	{
		// 当处于取消置数模式下,设置在线标志,清除远方置数标志
		pCI[no].bStatus |= CI_STATUS_ONLINE;
		pCI[no].bStatus &= ~CI_STATUS_MODIFY;
		
		if(tPara.wControl&CI_ATTR_PULSE)
			value = (DWORD)((QWORD)value*tPara.dwConst/100); 		//电度精度调整为0.001度   同时考虑溢出问题  yanxs 20140405
		
		pCI[no].dwValue = value;
	}
	else if( pCI[no].bStatus & AI_STATUS_ONLINE )
	{
		pCI[no].dwValue += value;
	}

	// 最大99999.99
	if(tPara.wControl&CI_ATTR_PULSE)
	{
		// 如果是脉冲电度需要检查脉冲量满值
		pCI[no].dwValue %= (100000*tPara.dwConst);
	}
	else
	{
		// 如果不是脉冲电度需要按照0.01取模
		pCI[no].dwValue %= 10000000;
	}
	
	return DB_ERR_OK;
}

/************************************************************
	功能：设置一个电度底数
	参数： 
	      no,		--  电度序号
		  value,	--  值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetCI(WORD no, DWORD value)
{
	DB_Register_SetData(MISC_CLASS_MODICI, no, value);
	return DB_WriteCI(no, value,DB_WRITE_UNCOMPEL);
}

/************************************************************
	功能：从电度库中取一个电度底数
	参数： 
	      no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadCIRaw(WORD no, DWORD *retval )
{
	TRunTimeCI		*pCI;
    WORD 			num;
               
    if( retval == NULL )
        return DB_ERR_BUFNULL;
	
	pCI = G_pCfgTable->DBaseConfig.pCI;
	num = G_pCfgTable->DBaseConfig.nCI;
	
	// 检查读取个数
	if(no >= num)
	{
		LogError("DB_ReadCI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCI==NULL )
    {
        LogError("DB_ReadCI",FILE_LINE,"The CI base is null!");
        return DB_ERR_BASENULL;
    }
	
	*retval = pCI[no].dwValue;

	return DB_ERR_OK;
}

/************************************************************
	功能：从电度库中取一个电度数据
	参数： 
	      no,		--  序号
		  retval	--  返回的值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_ReadCI(WORD no, DWORD *retval )
{
	TCIPara	tPara;
	DWORD	dwValue;
	STATUS	rc;
	
	// 读脉冲常数
	rc = DB_GetCIPara(no,&tPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	rc = DB_ReadCIRaw(no,&dwValue);
	if(rc != DB_ERR_OK)
		return rc;

	// 固定两位小数
	if(tPara.wControl&CI_ATTR_PULSE)
	{
		//  脉冲常数累加的是脉冲量，需要换算电度
		dwValue = (long long)dwValue*100/tPara.dwConst;
	}
	
	*retval = dwValue;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：读取遥控个数
	参数：无
	返回：遥控个数
************************************************************/
WORD	DB_GetBONum()
{
	return G_pCfgTable->DBaseConfig.nBO;
}

/************************************************************
	功能：读取遥遥调个数
	参数：无
	返回：遥控个数
************************************************************/
WORD	DB_GetAONum()
{
	return G_pCfgTable->DBaseConfig.nAO;
}

/************************************************************
	功能：读取直接数字采样点个数
	参数：无
	返回：数字采样点个数
************************************************************/
WORD	DB_GetDINum()
{
	return G_pCfgTable->DBaseConfig.nDI;
}

/************************************************************
	功能：遥控预置操作
	参数： switchno		-- 遥控点号
	       attr,     	-- 遥控属性 BO_OPEN、BO_CLOSE、 BO_NULL
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	 DB_BOSelect(WORD  switchno, WORD  attr)
{
    TRunTimeBO	*pBase;
    TBOPara		*pAttr;
    WORD 		num, index;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;

/*
    index = DB_BLANK;

	// 1。按照遥控开关号和属性查找对应的遥控
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/
    index = switchno;

	// 2。记录遥控任务
    pBase[index].dwAppID = RtosGetCurrentTaskId();

	// 3。启动遥控超时计时
	pBase[index].wTimeOutCnt = BO_RUNTIME_TIMEOUT;
	
	// 4。记录本次操作遥控时间
	ReadAbsTime(&pBase[index].tLastExecTime);

	// 5。记录遥控状态
	pBase[index].wStatus = BO_STATUS_SELECT;

	TBORegisterInfo *pRegBO = DB_SearchRegisterBO(switchno);

	// 6。发送遥控消息。注意虚遥控的处理>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(pRegBO != NULL)
	{
		LogError("DB_BOSelect",FILE_LINE,"Send BO %d message!!!",index);
		OS_PostMessage(pRegBO->hTaskID, SM_BOSELECT, index, attr, 0,0);
	}
	else	
	    OS_PostMessage(g_hBOTaskID, SM_BOSELECT, index, attr, 0,0);
	
    return DB_ERR_OK;
}

/************************************************************
	功能：遥控预置响应
	参数：index, 	-- 遥控点号
	          attr,     -- 遥控属性 BO_OPEN、BO_CLOSE、BO_NULL
              result    -- 遥控反较结果 0 -- 正确，其他 -- 错误
	返回：正确返回0，错误返回错误值。
************************************************************/
DWORD DB_BOSelectEcho(WORD index,WORD attr,WORD result )
{
    TRunTimeBO		*pBase;
    TBOPara			*pAttr;
    WORD 			num;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;

/*
    index = DB_BLANK;

	// 1。按照遥控开关号和属性查找对应的遥控
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/

	if(index >= num)
        return DB_ERR_NOSUPPORT;
		
	// 2。启动遥控超时计时
	pBase[index].wTimeOutCnt = 0;
	
	// 3。记录遥控状态
	pBase[index].wStatus = BO_STATUS_SELSUCCESS;

	// 4。发送遥控返校消息
    OS_PostMessage( pBase[index].dwAppID, SM_BOSELECTECHO, 0, MAKELONG( pAttr[index].wSwitchNo, attr ), result, 0 );

	return DB_ERR_OK;
}

/************************************************************
	功能：遥控执行操作
	参数： switchno,	-- 遥控点号
	          attr,		-- 遥控属性 BO_OPEN、 BO_CLOSE、BO_NULL
              mode		-- 遥控方式 1 -- 遥控执行，2 -- 遥控撤消
	返回：正确返回0，错误返回错误值。
************************************************************/
DWORD DB_BOExec(WORD switchno,WORD attr,WORD mode )
{
    TRunTimeBO	*pBase;
    TBOPara		*pAttr;
    WORD 		num, index;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;
/*
    index = DB_BLANK;

	// 1。按照遥控开关号和属性查找对应的遥控
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/

	index = switchno;

	// 2。记录遥控任务
    pBase[index].dwAppID = RtosGetCurrentTaskId();

	// 3。启动遥控超时计时
	pBase[index].wTimeOutCnt = BO_RUNTIME_TIMEOUT;
	
	// 4。记录本次操作遥控时间
	ReadAbsTime(&pBase[index].tLastExecTime);

	// 5。记录遥控状态
	pBase[index].wStatus = BO_STATUS_EXE;

	// 6。发送遥控消息
	TBORegisterInfo *pRegBO = DB_SearchRegisterBO(switchno);

	if(pRegBO != NULL)
	{
		LogError("DB_BOExec",FILE_LINE,"Send BO %d message!!!");
		OS_PostMessage(pRegBO->hTaskID, SM_BOEXEC, index, attr, mode,  0 );
	}
	else
		OS_PostMessage(g_hBOTaskID, SM_BOEXEC, index, attr, mode,  0 );

	// 7。注册遥控操作事件
    DB_Register_BO(switchno, attr);
	
    return DB_ERR_OK;
}

/************************************************************
	功能：遥控执行响应
	参数：index,  	--   实际库遥控号
			attr,		--   遥控属性 BO_OPEN、BO_CLOSE、BO_NULL
     		mode		-- 	遥控方式1 -- 遥控执行，2 -- 遥控撤消
			result		--   返回值 0 -- 正确，其他 -- 错误
	返回：正确返回0，错误返回错误值。
************************************************************/
DWORD DB_BOExecEcho(WORD index,WORD attr,WORD mode,WORD result )
{
    TRunTimeBO		*pBase;
    TBOPara			*pAttr;
    WORD 			num;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;
/*
    index = DB_BLANK;

	// 1。按照遥控开关号和属性查找对应的遥控
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/
	if(index >= num)
        return DB_ERR_NOSUPPORT;

	// 2。启动遥控超时计时
	pBase[index].wTimeOutCnt = 0;
	
	// 3。记录遥控状态
	pBase[index].wStatus = BO_STATUS_READY;

	// 4。发送遥控返校消息
    OS_PostMessage( pBase[index].dwAppID, SM_BOEXECECHO, 0, MAKELONG( pAttr[index].wSwitchNo, attr ), result, 0 );

	return DB_ERR_OK;
}

/************************************************************
	功能：遥控直接执行操作
	参数： switchno,	-- 遥控点号
	       attr,		-- 遥控属性 BO_OPEN、BO_CLOSE、BO_NULL
	返回：正确返回0，错误返回错误值。
************************************************************/
DWORD DB_BODirect(WORD switchno, WORD attr )
{
    TRunTimeBO	*pBase;
    TBOPara		*pAttr;
    WORD 		num, index;

    //Get BO Attribute Base
    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;

/*
    index = DB_BLANK;

	// 1。按照遥控开关号和属性查找对应的遥控
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/

	index = switchno;

	// 2。记录遥控任务
    pBase[index].dwAppID = RtosGetCurrentTaskId();

	// 3。启动遥控超时计时
	pBase[index].wTimeOutCnt = BO_RUNTIME_TIMEOUT;
	
	// 4。记录本次操作遥控时间
	ReadAbsTime(&pBase[index].tLastExecTime);

	// 5。记录遥控状态
	pBase[index].wStatus = BO_STATUS_DIREXE;

	// 6。发送遥控消息
	TBORegisterInfo *pRegBO = DB_SearchRegisterBO(switchno);

	if(pRegBO != NULL)
	{
		LogError("DB_BODirect",FILE_LINE,"Send BO %d message!!!");
		OS_PostMessage(pRegBO->hTaskID, SM_BODIRECT, index, attr , 0, 0 );
	}
	else
    	OS_PostMessage(g_hBOTaskID, SM_BODIRECT, index, attr , 0, 0 );

	// 7。注册遥控操作事件
    DB_Register_BO(switchno, attr);
	
    return DB_ERR_OK;
}

/************************************************************
	功能：遥控直接执行响应
	参数： index,		-- 遥控点号
			attr,		--   遥控属性 BO_OPEN、BO_CLOSE、BO_NULL
     		mode		-- 	遥控方式1 -- 遥控执行，2 -- 遥控撤消
			result		--   返回值 0 -- 正确，其他 -- 错误
	返回：正确返回0，错误返回错误值。
************************************************************/
DWORD DB_BODirectEcho(WORD index,WORD attr,WORD result )
{
    TRunTimeBO		*pBase;
    TBOPara			*pAttr;
    WORD 			num;

    num   = G_pCfgTable->DBaseConfig.nBO;
    pBase = G_pCfgTable->DBaseConfig.pBO;
	pAttr = G_pCfgTable->DBaseConfig.pBOAttr;
	
	ASSERT(pBase != NULL);
    if( pBase==NULL )
        return DB_ERR_BASENULL;

	ASSERT(pAttr != NULL);
    if( pAttr == NULL )
        return DB_ERR_BASENULL;
	
    if( num == 0 )
        return DB_ERR_BASENULL;
/*
    index = DB_BLANK;

	// 1。按照遥控开关号和属性查找对应的遥控
    for( i=0; i<num; i++ )
    {
        if( switchno == pAttr[i].wSwitchNo)
        {
            if( attr == BO_GET_OPT(pAttr[i].dwControl) )
            {
                index = i;
                break;
            }
        }
    }

    if( index == DB_BLANK )
        return DB_ERR_NOSUPPORT;
*/
	if(index >= num)
        return DB_ERR_NOSUPPORT;

	// 2。启动遥控超时计时
	pBase[index].wTimeOutCnt = 0;
	
	// 3。记录遥控状态
	pBase[index].wStatus = BO_STATUS_READY;

	// 4。发送遥控返校消息
    OS_PostMessage( pBase[index].dwAppID, SM_BODIRECTECHO, 0, MAKELONG( pAttr[index].wSwitchNo, attr ), result, 0 );

	return DB_ERR_OK;
}

/************************************************************
	功能：保护传动操作
	参数： no			-- 传动路号，与遥控路号不同
	       par,     	-- 传动保持时间
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	 DB_BOTest(WORD  no, WORD  par)
{
	//目前数据库参数中没有传动路数的参数，需要增加此项以进行必要的检查
	
	// 6。发送遥控消息。注意虚遥控的处理>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    OS_PostMessage(g_hBOTaskID, SM_BOTEST, no, par, 0,0);
	
    return DB_ERR_OK;
}


/************************************************************
	功能：读取遥调个数
	参数：无
	返回：遥调个数
************************************************************/
//WORD	DB_GetAONum();

/************************************************************
	功能：遥调请求操作
	参数： no		-- 遥调点号
	       value,	-- 遥调值 
	返回：正确返回0，错误返回错误值
************************************************************/
//DWORD DB_AOSelect(WORD no,long value);

/************************************************************
	功能：遥调预置返校
	参数： no,     	--   实际库遥调号
		value   	--   遥调值 
		result 		--   返回值 0 -- 正确， 
								1 -- 请求的遥调路号错误，
                                2 -- 请求的遥调值不合法
	返回：正确返回0，错误返回错误值
************************************************************/
//DWORD DB_AOSelectEcho(WORD no, long  value, WORD result );

/************************************************************
	功能：遥调执行操作
	参数： no,    	-- 遥调点号
	       value, 	-- 遥调值
            mode   	-- 执行方式 1 -- 遥调执行，2 -- 遥调撤消
	返回：正确返回0，错误返回错误值
************************************************************/
//DWORD DB_AOExec(WORD no, short value, WORD mode );

/************************************************************
	功能：遥调执行反校
	参数： no,    		--   实际库遥调号
			value,		--   遥调值   
           mode,  		--   执行方式	1 -- 遥调执行， 2 -- 遥调撤消
			result		--   返回值		0 -- 正确，		1 -- 错误
	返回：正确返回0，错误返回错误值
************************************************************/
//DWORD DB_AOExecEcho(WORD no, short value, WORD mode, WORD result );

/************************************************************
	功能：遥调直接执行操作
	参数： no,     	-- 遥调点号
	       value,  	-- 遥调值 
	返回：正确返回0，错误返回错误值
************************************************************/
//DWORD DB_AODirect(WORD no,short value);

/************************************************************
	功能：遥调直接执行反较
	参数： no,    	--   实际库遥调号
			value  	--   遥调值 
			result 	--   返回值 0 -- 遥调执行成功， 1 -- 遥调执行失败
	返回：正确返回0，错误返回错误值
************************************************************/
//DWORD DB_AODirectEcho(WORD no, short value, WORD result );

/************************************************************
	功能：获取未读取变位遥信数据个数
	参数：*retval	返回数据指针
	返回：未读取变位遥信数据个数
************************************************************/
STATUS DB_GetBIENum(WORD *retval)
{
	TBurstBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3。获取读指针    
    rc = DB_GetBIERead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_BIE_NUM-rp)%MAX_BIE_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：变位遥信缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_BIEIsEmpty()
{
	TBurstBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return TRUE;

    // 2。获取读指针    
    rc = DB_GetBIERead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	功能：从数据库中读取一条变位遥信数据
	参数：	rp   	--  读取数据用的指针,当BLANK时用内部读指针
			*event	--	返回的事项指针
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_ReadBIE(TBurstBI *event, DWORD rp )
{
    TBurstBIWrap 	*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nBIERead%MAX_BIE_NUM;
    }
    else
    {
		rp = rp%MAX_BIE_NUM;
    }
                       
	// 4。读取事项信息
	*event = pWrap->pEvent[rp];
	
	// 5。事项读取完毕直接清除事项标志
   	DB_ClearEventFlag(pAppInfo,APP_EVENT_BIE);
                                     
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一条变位遥信数据
	参数：event, --  写入的数据 
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_WriteBIE(TBurstBI *pEvent )
{
    TBurstBIWrap 	*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1。检查事项指针的合法性
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3。存数据
	OS_Protect(0);
	
    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_BIE_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_BIE_NUM;

	OS_UnProtect(0);
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++, pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_BIE))
		{
			pAppInfo->nBIERead = pWrap->nTailIndex;
			continue;
		}
		
	    if( pAppInfo->nBIERead == pWrap->nTailIndex )
	         pAppInfo->nBIERead = (pWrap->nTailIndex+1)%MAX_BIE_NUM;

		//由数据库任务统一处理，防止雪崩时不能及时处理
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_BIE))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_BIE);
			
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_BIBURST, 0,0,0,0);
		}
	}

	return DB_ERR_OK;
}

/************************************************************
	功能：取变位遥信缓冲区头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetBIEHead(WORD *retval)
{
	TBurstBIWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_BIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：取变位遥信缓冲区尾指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetBIETail(WORD *retval)
{
	TBurstBIWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pBurstBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_BIE_NUM;

	return DB_ERR_OK;
}
		
/************************************************************
	功能：取变位遥信缓冲区读指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetBIERead(WORD *retval)
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
    *retval = pAppInfo->nBIERead%MAX_BIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增变位遥信缓冲区读指针
	参数：无
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_IncBIERead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nBIERead = (pAppInfo->nBIERead+1)%MAX_BIE_NUM;
		
	return DB_ERR_OK;
}


/************************************************************
	功能：获取未读取SOE数据个数
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetSOENum(WORD *retval)
{
	TEventBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3。获取读指针    
    rc = DB_GetSOERead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_SOE_NUM-rp)%MAX_SOE_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：SOE缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_SOEIsEmpty()
{
	TEventBIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return TRUE;

    // 2。获取读指针    
    rc = DB_GetSOERead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	功能：从数据库中读取一条SOE数据
	参数：rp		--  读取数据用的指针,当BLANK时用内部读指针
	输出：event		--  返回的数据 
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_ReadSOE(TEventBI *event, DWORD rp )
{
    TEventBIWrap 	*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nSOERead%MAX_SOE_NUM;
    }
    else
    {
		rp = rp%MAX_SOE_NUM;
    }
                       
	// 4。读取事项信息
	*event = pWrap->pEvent[rp];
	
	// 5。事项读取操作清除事项标志
    DB_ClearEventFlag(pAppInfo,APP_EVENT_SOE);
                                     
	return DB_ERR_OK;
}

/************************************************************
	功能：向设备中写入一条SOE数据
	参数：pEvent,	--  写入的数据 
 	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_WriteSOE( TEventBI *pEvent )
{
    TEventBIWrap 	*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1。检查事项指针的合法性
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;

	// 3。存数据
	OS_Protect(0);
	
    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_SOE_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_SOE_NUM;

	OS_UnProtect(0);
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++, pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_SOE))
		{
			pAppInfo->nSOERead = pWrap->nTailIndex;
			continue;
		}
	    if( pAppInfo->nSOERead == pWrap->nTailIndex )
	         pAppInfo->nSOERead = (pWrap->nTailIndex+1)%MAX_SOE_NUM;
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_SOE))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_SOE);
			//由数据库任务统一发消息，防止雪崩时不能及时处理
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_BIEVENT, 0,0,0,0);
		}
	}
	
	DB_RegisterFileChange(DB_FILE_BASICEV);

	return DB_ERR_OK;
}

/************************************************************
	功能：取SOE缓冲区头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetSOEHead(WORD *retval)
{
	TEventBIWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_SOE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：取SOE缓冲区尾指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetSOETail(WORD *retval)
{
	TEventBIWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventBIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_SOE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：取SOE缓冲区读指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetSOERead(WORD *retval)
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
    *retval = pAppInfo->nSOERead%MAX_SOE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增SOE缓冲区读指针
	参数：无
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_IncSOERead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nSOERead = (pAppInfo->nSOERead+1)%MAX_SOE_NUM;

	//DB_RegisterFileChange(DB_FILE_BASICEV);
		
	return DB_ERR_OK;
}

STATUS DB_RegisterFileChange(WORD wFileType)
{
	TFileMap				*pFileMap;

	// 镜像文件
	switch(wFileType)
	{
	case DB_FILE_PARACFG:
		pFileMap = G_pCfgTable->DBaseConfig.pBasicParaFileMap;
		break;
	case DB_FILE_RELAYDATA:
		pFileMap = G_pCfgTable->DBaseConfig.pRelayDataFileMap;
		break;
	case DB_FILE_RELAYEV:		
		pFileMap = G_pCfgTable->DBaseConfig.pRelayEventFileMap;
		break;
	case DB_FILE_BASICEV:
		pFileMap = G_pCfgTable->DBaseConfig.pBasicEventFileMap;
		break;
	default:
		return DB_ERR_FILE_TYPE;
	}
	
	if(pFileMap == NULL)
		return DB_ERR_MAP_NULL;

	// 更新时间
	ReadAbsTime(&pFileMap->absTime);

	// 置标志
	pFileMap->wFlag |= FILEMAP_FLAGS_MODIFY;

	return DB_ERR_OK;
}

STATUS DB_SaveBasicEventFile()
{	
    TFileHead         		*filehead;
	TFileMap				*pFileMap;
	TEventBI				*pEventBI;	
	TEventBIWrap			*pEventBIWrap;
//	TRelayStartEv			*pRSTT;
//	TRelayStartWrap 		*pRSTTWrap;
	TMiscEvent				*pMISC;	
	TMiscWrap				*pMiscWrap;
	TBasicEventPtrList		*pPtrList;
	
	pEventBIWrap 	= G_pCfgTable->DBaseConfig.pEventBIWrap;
//	pRSTTWrap		= G_pCfgTable->DBaseConfig.pRSTTWrap;
	pMiscWrap 		= &G_pCfgTable->MiscWrap;
	pFileMap 		= G_pCfgTable->DBaseConfig.pBasicEventFileMap;
	
	if(pFileMap == NULL)
		return DB_ERR_MAP_NULL;
	
	filehead = (TFileHead *)pFileMap->pRamData;
	pPtrList = (TBasicEventPtrList	*)(filehead+1);

	// 1。修正文件内容
	pEventBI = (TEventBI	 *)((BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TBasicEventPtrList));
	
	memcpy(pEventBI,pEventBIWrap->pEvent,sizeof(TEventBI)*MAX_SOE_NUM);
				
	pPtrList->nMaxSOE = pEventBIWrap->nMax;
	pPtrList->nSOET = pEventBIWrap->nTailIndex;
	pPtrList->nSOEH = pEventBIWrap->nHeadIndex;

	pMISC = (TMiscEvent *)((BYTE *)pFileMap->pRamData\
		+sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
		+sizeof(TEventBI)*MAX_SOE_NUM);
	
	memcpy(pMISC,pMiscWrap->pEvent,sizeof(TMiscEvent)*MAX_MISC_NUM);
	
	pPtrList->nMaxOptRecord = pMiscWrap->nMax;
	pPtrList->nOptRecordT = pMiscWrap->nTailIndex;
	pPtrList->nOptRecordH = pMiscWrap->nHeadIndex;

/*	
	pRSTT = (TRelayStartEv	*)((BYTE *)pFileMap->pRamData\
			+sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
			+sizeof(TEventBI)*MAX_SOE_NUM\
			+sizeof(TMiscEvent)*MAX_MISC_NUM);
	memcpy(pRSTT,pRSTTWrap->pEvent,sizeof(TRelayStartEv)*MAX_RSTT_NUM);
	
	pPtrList->nMaxRelaySart= pRSTTWrap->nMax;
	pPtrList->nRelayStartT = pRSTTWrap->nTailIndex;
	pPtrList->nRelayStartH = pRSTTWrap->nHeadIndex;
*/
	// 2。重新计算文件校验
	filehead->dwSize = sizeof(TFileHead)+sizeof(TBasicEventPtrList)\
				+sizeof(TEventBI)*MAX_SOE_NUM\
				+sizeof(TMiscEvent)*MAX_MISC_NUM\
				+sizeof(WORD);
	filehead->dwSize = ltobl(filehead->dwSize);
	
	ReadSystemTime(&filehead->stTime);
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-2);
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-2);

	// 3。写入文件
	return DAE_WriteFile(BASICEVENTFILE, filehead,filehead->dwSize);
}

/************************************************************
	功能：取用户登录密码
	参数：*strPassWord	返回密码指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetUserPassWord(char *strPassWord)
{
	memcpy(strPassWord,G_pCfgTable->DBaseConfig.chPassWord1,8);
	return DB_ERR_OK;
}

/************************************************************
	功能：取管理员登录密码
	参数：*strPassWord	返回密码指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetAdminPassWord(char *strPassWord)
{
	memcpy(strPassWord,G_pCfgTable->DBaseConfig.chPassWord2,8);
	return DB_ERR_OK;
}

/************************************************************
	功能：取用户登录密码
	参数：	strUserPassWord	返回用户密码指针
			strAdminPassWord	返回管理员密码指针;
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetAccessPassWord(char *strUserPassWord,char *strAdminPassWord)
{
	if(strUserPassWord)
		memcpy(strUserPassWord,G_pCfgTable->DBaseConfig.chPassWord1,8);
	
	if(strAdminPassWord)
		memcpy(strAdminPassWord,G_pCfgTable->DBaseConfig.chPassWord2,8);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：取语言编号
	参数：NONE
	返回：语言编号
************************************************************/
WORD DB_GetLanguageNo()
{
	return G_pCfgTable->DBaseConfig.nLanguageNo;
}

/************************************************************
	功能：取装置地址
	参数：NONE
	返回：装置地址
************************************************************/
WORD DB_GetAddress()
{
	return G_pCfgTable->DBaseConfig.wAddress;
}

/************************************************************
	功能：读取网卡A的IP、子网掩码
	参数：
			dwIP 		IP地址
			dwMask		子网掩码
			dwGateWay	网关
	返回：读取结果
************************************************************/
STATUS DB_GetEthAPara(DWORD &dwIP,DWORD &dwMask,DWORD &dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			dwGateWayIP = pEthernet->dwGateWay1;
			return DB_ERR_OK;
		}
	}

	return DB_ERR_FAILURE;
}

/************************************************************
	功能：读取网卡B的IP、子网掩码
	参数：
			dwIP 		IP地址
			dwMask		子网掩码
			dwGateWay	网关
	返回：读取结果
************************************************************/
STATUS DB_GetEthBPara(DWORD &dwIP,DWORD &dwMask,DWORD &dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			dwGateWayIP = pEthernet->dwGateWay1;
			return DB_ERR_OK;
		}
	}

	return DB_ERR_FAILURE;
}

/************************************************************
	功能：修改装置地址
	参数：wAddr 装置地址
	返回：修改结果
************************************************************/
STATUS DB_ModifyAddress(WORD wAddr)
{	
	G_pCfgTable->DBaseConfig.wAddress = wAddr;
	G_pCfgTable->DBaseConfig.tMapData.pConfig->wAddress = ltobs(wAddr);

	// 更新EEPORM参数
	G_tBasicConfig.wNetAddress = wAddr;
	DAE_UpdateBasicConfig();

	//保存保护数据文件
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_ADDR);
}

/************************************************************
	功能：读取网卡A的IP、子网掩码
	参数：
			dwIP 	IP地址
			wMask	子网掩码
	返回：读取结果
************************************************************/
/*
STATUS DB_GetNetAIPMask(DWORD &dwIP,DWORD &dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			return DB_ERR_OK;
		}
	}

	dwIP = 0xC0A864c8;
	dwMask = 0xFFFFFF00;
	return DB_ERR_FAILURE;
}
*/
/************************************************************
	功能：读取网卡B的IP、子网掩码
	参数：
			dwIP 	IP地址
			wMask	子网掩码
	返回：读取结果
************************************************************/
/*
STATUS DB_GetNetBIPMask(DWORD &dwIP,DWORD &dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			dwIP = pEthernet->dwSelfIP;
			dwMask = pEthernet->dwSelfMask;
			return DB_ERR_OK;
		}
	}

	dwIP = 0xAC140A14;
	dwMask = 0xFFFFFF00;
	return DB_ERR_FAILURE;
}
*/

/************************************************************
	功能：修改网卡A的IP、子网掩码
	参数：
			dwIP 	IP地址
			wMask	子网掩码
	返回：修改结果
************************************************************/
/*
STATUS DB_ModifyNetAIPMask(DWORD dwIP,DWORD dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			// 1. 修改数据库所有A网IP
			pEthernet->dwSelfIP = dwIP;
			pEthernet->dwSelfMask= dwMask;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. 同步修改文件
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);

			// 3。同步修改EEPROM参数
			G_tBasicConfig.dwNetAIP = dwIP;
			G_tBasicConfig.dwNetAMask = dwMask;
		}
	}
	DAE_UpdateBasicConfig();
	
	// 3。通知网络任务重新初始化网卡
	
	// 4。保存参数文件
	return DB_ModifyParaFile(DB_PARA_ETH,1);
}
*/
/************************************************************
	功能：修改网卡B的IP、子网掩码
	参数：
			dwIP 	IP地址
			wMask	子网掩码
	返回：修改结果
************************************************************/
/*
STATUS DB_ModifyNetBIPMask(DWORD dwIP,DWORD dwMask)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			// 1. 修改数据库所有A网IP
			pEthernet->dwSelfIP = dwIP;
			pEthernet->dwSelfMask= dwMask;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. 同步修改文件
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);

			// 3。同步修改EEPROM参数
			G_tBasicConfig.dwNetBIP = dwIP;
			G_tBasicConfig.dwNetBMask = dwMask;
		}
	}
	
	DAE_UpdateBasicConfig();
	// 3。通知网络任务重新初始化网卡
	
	// 4。保存参数文件
	return DB_ModifyParaFile(DB_PARA_ETH,2);
}
*/
/************************************************************
	功能：修改网卡A的IP、子网掩码
	参数：
			dwIP 		IP地址
			dwMask		子网掩码
			dwGateWay	网关
	返回：修改结果
************************************************************/
STATUS DB_ModifyEthAPara(DWORD dwIP,DWORD dwMask,DWORD dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETA)
		{
			// 1. 修改数据库所有A网IP
			pEthernet->dwSelfIP   = dwIP;
			pEthernet->dwSelfMask = dwMask;
			pEthernet->dwGateWay1 = dwGateWayIP;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. 同步修改文件
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);
			pEthernet->dwGateWay1= ltobl(dwGateWayIP);
			
			// 3。同步修改EEPROM参数
			G_tBasicConfig.dwNetAIP = dwIP;
			G_tBasicConfig.dwNetAMask = dwMask;
			G_tBasicConfig.dwNetAGWIP = dwGateWayIP;				
		}
	}
	
	// 4 更新EEPORM参数
	DAE_UpdateBasicConfig();
	
	// 5。通知网络任务重新初始化网卡
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_MAPBIT), SM_NETCARDPARACHG, 0, 0, 0, 0);
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_ETHMAIN), SM_NETCARDPARACHG, 0, 0, 0, 0);
	
	// 6。保存参数文件
	return DB_ModifyParaFile(DB_PARA_ETH,1);
}

/************************************************************
	功能：修改网卡B的IP、子网掩码
	参数：
			dwIP 		IP地址
			dwMask		子网掩码
			dwGateWay	网关
	返回：修改结果
************************************************************/
STATUS DB_ModifyEthBPara(DWORD dwIP,DWORD dwMask,DWORD dwGateWayIP)
{
	TEthPortPara *pEthernet;

	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthernet = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		if(HIBYTE(pEthernet->wPortID) == SC_PORT_ETHERNETB)
		{
			// 1. 修改数据库所有A网IP
			pEthernet->dwSelfIP = dwIP;
			pEthernet->dwSelfMask= dwMask;
			pEthernet->dwGateWay1 = dwGateWayIP;
			
			pEthernet = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
			
			// 2. 同步修改文件
			pEthernet->dwSelfIP = ltobl(dwIP);
			pEthernet->dwSelfMask= ltobl(dwMask);
			pEthernet->dwGateWay1= ltobl(dwGateWayIP);

			// 3。同步修改EEPROM参数
			G_tBasicConfig.dwNetBIP = dwIP;
			G_tBasicConfig.dwNetBMask = dwMask;
			G_tBasicConfig.dwNetBGWIP = dwGateWayIP;	
		}
	}
	
	// 4 更新EEPORM参数
	DAE_UpdateBasicConfig();
	
	// 5。通知网络任务重新初始化网卡
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_MAPBIT), SM_NETCARDPARACHG, 0, 0, 0, 0);
	
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_ETHMAIN), SM_NETCARDPARACHG, 0, 0, 0, 0);
	
	// 6。保存参数文件
	return DB_ModifyParaFile(DB_PARA_ETH,2);
}


/************************************************************
功能：设置当前系统故障电量显示模式 
参数：mode 显示模式	FD_SHOWMODE_FIRST/FD_SHOWMODE_SECOND
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS	DB_SetFDShowMode(BYTE mode)
{
	if(mode==FD_SHOWMODE_FIRST)
	{
		G_pCfgTable->DBaseConfig.dwControl &=  ~DB_RELAY_FDSECOND;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl &=  ~DB_RELAY_FDSECOND;
	}
	else
	{
		G_pCfgTable->DBaseConfig.dwControl |=  DB_RELAY_FDSECOND;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl |=  DB_RELAY_FDSECOND;
	}
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_FDSHOW);
}
/************************************************************
功能：设置当前系统GPS时钟源格式
参数：flag，为TRUE,则表示DK3511,否则为KF6511
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS	DB_SetIRGBType(BOOL flag)
{
	if(flag)
	{
		G_pCfgTable->DBaseConfig.dwControl |=  DB_IRGB_DK3511;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl |=  DB_IRGB_DK3511;
	}
	else
	{
		G_pCfgTable->DBaseConfig.dwControl &=  ~DB_IRGB_DK3511;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl &=  ~DB_IRGB_DK3511;
	}
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_IRGBTYPE);
}

/************************************************************
功能：设置当前系统语言编号
参数：no 语言编号	
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS DB_SetLanguageNo(BYTE no)
{
	G_pCfgTable->DBaseConfig.nLanguageNo = no;
	G_pCfgTable->DBaseConfig.tMapData.pConfig->nLanguageNo = no;
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_LANGUAGE);
}

/************************************************************
功能：设置当前系统用户密码
参数：strPassWord 新密码	
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS DB_SetUserPassWord(const char *strPassWord)
{
	memcpy(G_pCfgTable->DBaseConfig.chPassWord1,strPassWord,8);
	memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord1,strPassWord,8);
	// 更新EEPORM参数
	memcpy(G_tBasicConfig.chPassWord1,strPassWord,8);
	DAE_UpdateBasicConfig();

	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_PASSWORD);
}

/************************************************************
功能：设置当前系统管理员密码
参数：strPassWord 新密码	
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS DB_SetAdminPassWord(const char *strPassWord)
{
	memcpy(G_pCfgTable->DBaseConfig.chPassWord2,strPassWord,8);
	memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord2,strPassWord,8);
	// 更新EEPORM参数
	memcpy(G_tBasicConfig.chPassWord2,strPassWord,8);
	DAE_UpdateBasicConfig();
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_PASSWORD);
}

/************************************************************
功能：设置当前系统密码
参数：	strUserPassWord 用户密码密码；
		strAdminPassWord管理员密码	
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS DB_SetAccessPassWord(char *strUserPassWord,char *strAdminPassWord)
{
	if(strUserPassWord != NULL)
	{
		memcpy(G_pCfgTable->DBaseConfig.chPassWord1,strUserPassWord,8);
		memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord1,strUserPassWord,8);
		memcpy(G_tBasicConfig.chPassWord1,strUserPassWord,8);
	}

	if(strAdminPassWord != NULL)
	{
		memcpy(G_pCfgTable->DBaseConfig.chPassWord2,strAdminPassWord,8);
		memcpy(G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord2,strAdminPassWord,8);
		memcpy(G_tBasicConfig.chPassWord2,strAdminPassWord,8);
	}

	// 更新EEPORM参数
	DAE_UpdateBasicConfig();

	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_PASSWORD);
}

/************************************************************
功能：设置当前系统是否计算积分电度
参数：mode: TRUE 计算/FALSE 不计算
返回：设置正确返回 DB_ERR_FILE_OK; 否则返回错误代码		
************************************************************/
STATUS	DB_SetCICaculate(BOOL mode)
{
	if(mode)
	{
		G_pCfgTable->DBaseConfig.dwControl |=  DB_RELAY_CACLINT;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl |=  DB_RELAY_CACLINT;
	}
	else
	{
		G_pCfgTable->DBaseConfig.dwControl &=  ~DB_RELAY_CACLINT;
		G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl &=  ~DB_RELAY_CACLINT;
	}
	
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_CIOPT);
}

// 是否保留端口
BOOL IsReservPort(WORD wPortID)
{	
	TEthPortPara		tEthernet;

	switch(MAJOR_PORT(wPortID))
	{
		case SC_PORT_SERIAL:
			// 仅保留第一个端口
			if(MINOR_PORT(wPortID) < 2)
				return TRUE;
			break;
			
		case SC_PORT_ETHERNETA:
//		case SC_PORT_ETHERNETB:
			if(MINOR_PORT(wPortID) <= 2)
				return TRUE;
/*			
			for(int i=0;i<DB_GetEthLinkNum();i++)
			{
				DB_GetETHPara(i,&tEthernet);

				// 找到端口
				if(tEthernet.wPortID != wPortID)
					continue;

				// 服务器
				if(tEthernet.dwMode & ETHLINK_MODE_CLIENT)
					continue;

				// TCP
				if(tEthernet.dwMode & ETHLINK_MODE_UDP)
					continue;

				// 端口号701、702保留维护和Shell规约
				if(MAJOR_PORT(wPortID) == SC_PORT_ETHERNETA)
				if((tEthernet.dwSelfPort == 701)||(tEthernet.dwSelfPort == 702))
					return TRUE;
			}
*/			
			break;
		default:
			break;
	}

	return FALSE;
}

/************************************************************
	功能：读取串口规约参数
	参数：
			no 			串口序号
			pConfig		串口规约参数
	返回：读取结果
************************************************************/
STATUS DB_GetSerialConfig(WORD no,TSerialConfig *pConfig)
{
	TSerialPara 	tPara;
	STATUS			rc;

	rc = DB_GetSerialPara(no, &tPara);
	
	if(rc == DB_ERR_OK)
	{
		pConfig->byNo = no+1;
		pConfig->byBaud = tPara.byBaud;
		pConfig->byMode = tPara.bFlag;
		pConfig->byDriver = tPara.byDriver;
	}

	return rc;
}

/************************************************************
	功能：设置串口规约参数
	参数：
			no 			串口序号
			pConfig		串口规约参数
	返回：读取结果
************************************************************/
STATUS DB_SetSerialConfig(WORD no,TSerialConfig *pConfig)
{
	TSerialPara 	tPara;
	STATUS			rc;
	
	// 转换为从0编号	
	for(int i=0;i<DB_GetSerialNum();i++,pConfig++)
	{
		// 单个写入时
		if(no != DB_PARA_ALL)
			i = no-1;
		
		rc = DB_GetSerialPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

		if(!IsReservPort(tPara.wPortID))
		{
			tPara.byBaud = pConfig->byBaud;
			tPara.bFlag = pConfig->byMode;
			tPara.byDriver = pConfig->byDriver;
		}
		
		// 2。回写参数
		rc = DB_WriteSerialPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;
	
		// 单个写入时
		if(no != DB_PARA_ALL)
			break;
	}

	if(no != DB_PARA_ALL)
		return DB_WriteSerialParaConfirm(no-1);
	else
		return DB_WriteSerialParaConfirm(DB_PARA_ALL);
}

/************************************************************
	功能：读取CAN网规约参数
	参数：
			no 			CANBUS序号
			pConfig		CANBUS规约参数
	返回：读取结果
************************************************************/
STATUS DB_GetCanbusConfig(WORD no,TCanbusConfig *pConfig)
{
	TCanBusPara 	tPara;
	STATUS			rc;

	rc = DB_GetCANPara(no, &tPara);
	
	if(rc == DB_ERR_OK)
	{
		pConfig->byNo = no+1;
		pConfig->wBaud = tPara.wBaud;
		pConfig->byDriver = tPara.byDriver;
	}

	return rc;
}

/************************************************************
	功能：设置CAN网规约参数
	参数：
			no 			CANBUS序号
			pConfig		CANBUS规约参数
	返回：读取结果
************************************************************/
STATUS DB_SetCanbusConfig(WORD no,TCanbusConfig *pConfig)
{
	TCanBusPara 	tPara;
	STATUS			rc;
	
	// 转换为从0编号
	for(int i=0;i<DB_GetCANNum();i++,pConfig++)
	{
		// 单个写入时
		if(no != DB_PARA_ALL)
			i = no-1;
		
		// 1。先读参数
		rc = DB_GetCANPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;
		
		if(!IsReservPort(tPara.wPortID))
		{
			tPara.wBaud = pConfig->wBaud;
			tPara.byDriver = pConfig->byDriver;
		}
		
		// 2。回写参数
		rc = DB_WriteCANPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

		// 单个写入时
		if(no != DB_PARA_ALL)
			break;
	}
		
	// 3。确认之行
	if(no != DB_PARA_ALL)
		rc = DB_WriteCANParaConfirm(no-1);
	else
		rc = DB_WriteCANParaConfirm(DB_PARA_ALL);
	return rc;
}

/************************************************************
	功能：读取以太网连接参数
	参数：
			no 			以太网连接序号
			pConfig		以太网连接规约参数
	返回：读取结果
************************************************************/
STATUS DB_GetEthLinkConfig(WORD no,TEthLinkConfig *pConfig)
{
	TEthPortPara	tPara;
	STATUS			rc;

	rc = DB_GetETHPara(no, &tPara);
	
	if(rc == DB_ERR_OK)
	{
		pConfig->byNetCardNo = MAJOR_PORT(tPara.wPortID)-SC_PORT_ETHERNETA+1;
		pConfig->byLinkNo = MINOR_PORT(tPara.wPortID);
		pConfig->byMode = tPara.dwMode;
		pConfig->byDriver = tPara.wDriver;
		pConfig->dwSideIP = tPara.dwSideIP;
		pConfig->wPort = tPara.dwSelfPort;
	}

	return rc;
}

/************************************************************
	功能：设置以太网连接参数
	参数：
			no 			以太网连接序号
			pConfig		以太网连接规约参数
	返回：读取结果
************************************************************/
STATUS DB_SetEthLinkConfig(WORD no,TEthLinkConfig *pConfig)
{
	TEthPortPara	tPara;
	STATUS			rc;
	
	// 转换为从0编号
	for(int i=0;i<DB_GetEthLinkNum();i++,pConfig++)
	{
		// 单个写入时
		if(no != DB_PARA_ALL)
			i = no-1;
		
		// 1。先读参数
		rc = DB_GetETHPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

		//屏蔽知名端口，剔除液晶和Shell规约
		if(!IsReservPort(tPara.wPortID))
		{
			tPara.dwMode = pConfig->byMode;
			tPara.wDriver = pConfig->byDriver;
			tPara.dwSideIP = pConfig->dwSideIP;
			tPara.dwSelfPort = pConfig->wPort;
			tPara.dwSidePort = pConfig->wPort;
		}
		
		// 2。回写参数
		rc = DB_WriteETHPara(i, &tPara);
		if(rc != DB_ERR_OK)
			return rc;

	}
	
	// 3。确认之行
	if(no != DB_PARA_ALL)
		rc = DB_WriteETHParaConfirm(no-1);
	else
		rc = DB_WriteETHParaConfirm(DB_PARA_ALL);
	return rc;
}

/************************************************************
	功能：获取数据库版本号
	参数：无
	返回：版本号
************************************************************/
STATUS DB_GetDataBaseVersion(BYTE *sVersion)
{	
	BYTE				*pFile;
    TFileHead         	*filehead;
	
	if(sVersion == NULL)
		return DB_ERR_PTRNULL;

	// 内容清0
	memset(sVersion,0,8);

	// paraconf.dat
	if(G_pCfgTable->DBaseConfig.pBasicParaFileMap != NULL)
	{
		pFile = (BYTE *)G_pCfgTable->DBaseConfig.pBasicParaFileMap->pRamData;
		filehead = (TFileHead  *)pFile;

		// 版本号
		sVersion[0] = LOBYTE(filehead->wFlag);
		sVersion[1] = HIBYTE(filehead->wFlag);
		sVersion[2] = LOBYTE(filehead->wVersion);
		sVersion[3] = HIBYTE(filehead->wVersion);

		// 校验码
		sVersion[4] = pFile[filehead->dwSize-2];
		sVersion[5] = pFile[filehead->dwSize-1];		
	}

	// relaydisp .dat
	if(G_pCfgTable->DBaseConfig.pRelayDispFileMap != NULL)
	{
		pFile = (BYTE *)G_pCfgTable->DBaseConfig.pRelayDispFileMap->pRamData;
		filehead = (TFileHead  *)pFile;

		// 校验码
		sVersion[6] = pFile[filehead->dwSize-2];
		sVersion[7] = pFile[filehead->dwSize-1];		
	}

	return DB_ERR_OK;
}

STATUS MakeRelayEventFile(void *pFileBuf);

/************************************************************
	功能：清除装置事件区
	参数：wMask 事件类型掩码 
	返回：正确返回DB_ERR_OK;错误返回错误码
************************************************************/
STATUS DB_ClearEvent(WORD wMask)
{
	TRelayActionWrap *pACTWrap;
	TRelayAlarmWrap *pALMWrap;
	TRelayStartWrap *pSTTWrap;
	TEventBIWrap 	*pSOEWrap;
	TBurstBIWrap 	*pBIEWrap;
	TMiscWrap		*pOPTWrap;
	TRecordWrap		*pLOGWrap;
	void	*pFileBuf;

	// 1 清除NvRAM

	// 1.1 清除动作事件
	if(wMask & (0x01<<DB_EVENT_RACT))
	{
		pACTWrap = G_pCfgTable->DBaseConfig.pRACTWrap;
		pACTWrap->dwMagic = WRAP_INIT_FLAG;
		pACTWrap->nMax = MAX_RACT_NUM;
		pACTWrap->nHeadIndex = 0;
		pACTWrap->nTailIndex = 0;
		
		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nRACTRead = 0;
	}
	// 1.2 清除告警事件
	if(wMask & (0x01<<DB_EVENT_RALM))
	{
		pALMWrap = G_pCfgTable->DBaseConfig.pRALMWrap;
		pALMWrap->dwMagic = WRAP_INIT_FLAG;
		pALMWrap->nMax = MAX_RALM_NUM;
		pALMWrap->nHeadIndex = 0;
		pALMWrap->nTailIndex = 0;
		
		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nRALMRead = 0;
	}
	
	// 1.3 清除启动事件
	if(wMask & (0x01<<DB_EVENT_RSTT))
	{
		pSTTWrap = G_pCfgTable->DBaseConfig.pRSTTWrap;
		pSTTWrap->dwMagic = WRAP_INIT_FLAG;
		pSTTWrap->nMax = MAX_RSTT_NUM;
		pSTTWrap->nHeadIndex = 0;
		pSTTWrap->nTailIndex = 0;

		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nRSTTRead = 0;
	}
	
	// 1.4 清除SOE事件
	if(wMask & (0x01<<DB_EVENT_SOE))
	{
		pSOEWrap = 	G_pCfgTable->DBaseConfig.pEventBIWrap;
		pSOEWrap->dwMagic = WRAP_INIT_FLAG;
		pSOEWrap->nMax = MAX_SOE_NUM;
		pSOEWrap->nHeadIndex = 0;
		pSOEWrap->nTailIndex = 0;

		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nSOERead = 0;
	}
	
	// 1.5 清除BIE事件
	if(wMask & (0x01<<DB_EVENT_BIE))
	{
		pBIEWrap = 	G_pCfgTable->DBaseConfig.pBurstBIWrap;
		pBIEWrap->dwMagic = WRAP_INIT_FLAG;
		pBIEWrap->nMax = MAX_BIE_NUM;
		pBIEWrap->nHeadIndex = 0;
		pBIEWrap->nTailIndex = 0;

		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nBIERead = 0;
	}
	
	// 1.6 清除操作记录
	if(wMask & (0x01<<DB_EVENT_MISC))
	{
		pOPTWrap = 	&G_pCfgTable->MiscWrap;
		pOPTWrap->dwMagic = WRAP_INIT_FLAG;
		pOPTWrap->nMax = MAX_MISC_NUM;
		pOPTWrap->nHeadIndex = 0;
		pOPTWrap->nTailIndex = 0;

		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nMISCRead = 0;
	}

	// 1.7 清除LOG信息
	if(wMask & (0x01<<DB_EVENT_LOG))
	{
		pLOGWrap = 	&G_pCfgTable->RecordWrap;
		pLOGWrap->dwMagic = WRAP_INIT_FLAG;
		pLOGWrap->nMax = MAX_RUNREC_NUM;
		pLOGWrap->nHeadIndex = 0;
		pLOGWrap->nTailIndex = 0;

		// 清除所有任务读指针
		for(int i=0; i<DAE_MAX_APP_NUM; i++ )
			G_pCfgTable->TaskApp[i].nLogRead = 0;
	}
	
	// 2 清除EEPROM
	if(wMask & ((0x01<<DB_EVENT_RALM)|(0x01<<DB_EVENT_RACT)))
	{
		// 2.1  指针检查
		if(G_pCfgTable->DBaseConfig.pRelayEventFileMap ==  NULL)
			return DB_ERR_PTRNULL;

		pFileBuf = G_pCfgTable->DBaseConfig.pRelayEventFileMap->pRamData;
		if(pFileBuf ==  NULL)
			return DB_ERR_PTRNULL;
		
		// 2.2 创建事项文件
		MakeRelayEventFile(pFileBuf);	
	}
	
	// 3 清除FLASH
	if(wMask & ((0x01<<DB_EVENT_SOE)|(0x01<<DB_EVENT_MISC)))
		FS_DelFile(BASICEVENTFILE,0);
	
	return DB_ERR_OK;
}

/************************************************************
功能：将基本参数内容从FLASH文件导入EEPROM
参数：NONE
返回：NONE
************************************************************/
void DB_ResetBasicConfig()
{
	TEthPortPara			*pEthFile;
	TEthPortPara 			*pEthBase;
	WORD					wFilterTime[MAX_DI_NUM+1];
	
	// 1.从文件中更新A B网参数
	for(int i=0;i<DB_GetEthLinkNum();i++)
	{
		pEthFile = G_pCfgTable->DBaseConfig.tMapData.pEthernet+i;
		pEthBase = G_pCfgTable->DBaseConfig.pEthernet+i;
		
		// 1. 修改数据库所有A网IP
		pEthBase->dwSelfIP   = ltobl(pEthFile->dwSelfIP);
		pEthBase->dwSelfMask = ltobl(pEthFile->dwSelfMask);
		pEthBase->dwGateWay1 = ltobl(pEthFile->dwGateWay1);
						
		if(HIBYTE(pEthBase->wPortID) == SC_PORT_ETHERNETA)
		{
			// 3。同步修改EEPROM参数
			G_tBasicConfig.dwNetAIP = ltobl(pEthFile->dwSelfIP);
			G_tBasicConfig.dwNetAMask = ltobl(pEthFile->dwSelfMask);
			G_tBasicConfig.dwNetAGWIP = ltobl(pEthFile->dwGateWay1);	
		}
		
		if(HIBYTE(pEthBase->wPortID) == SC_PORT_ETHERNETB)
		{
			// 3。同步修改EEPROM参数
			G_tBasicConfig.dwNetBIP = ltobl(pEthFile->dwSelfIP);
			G_tBasicConfig.dwNetBMask = ltobl(pEthFile->dwSelfMask);
			G_tBasicConfig.dwNetBGWIP = ltobl(pEthFile->dwGateWay1);	
		}
	}

	// 2.从文件中更新地址参数
	G_pCfgTable->DBaseConfig.wAddress = ltobs(G_pCfgTable->DBaseConfig.tMapData.pConfig->wAddress);
	G_tBasicConfig.wNetAddress = G_pCfgTable->DBaseConfig.wAddress;
	memcpy(G_tBasicConfig.chPassWord1,G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord1,8);
	memcpy(G_tBasicConfig.chPassWord2,G_pCfgTable->DBaseConfig.tMapData.pConfig->chPassWord2,8);

	DAE_UpdateBasicConfig();
	
	// 3.从文件中更新遥信去抖参数
	for(int i=0;i<DB_GetDINum();i++)
	{
		G_pCfgTable->DBaseConfig.pDIPara[i].wFilterTime = ltobs(G_pCfgTable->DBaseConfig.tMapData.pDIAttr[i].wFilterTime);

		if(i<MAX_DI_NUM)
			wFilterTime[i] = G_pCfgTable->DBaseConfig.pDIPara[i].wFilterTime;
	}
	
	if(DB_GetDINum()<MAX_DI_NUM)
		DAE_MakeCheckSum((BYTE *)wFilterTime,sizeof(WORD)*DB_GetDINum());
	EEPROM_Write(EEPROM_DI_FILTERTIME,sizeof(WORD)*(MAX_DI_NUM+1),wFilterTime);

	LogError("DB_ResetBasicConfig",FILE_LINE,"System basic config is recover OK!!");
}

/************************************************************
	功能：获取未读取AIE数据个数
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetAIENum(WORD *retval)
{
	TEventAIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

    // 3。获取读指针    
    rc = DB_GetAIERead(&rp);
	if(rc != DB_ERR_OK)
		return DB_ERR_NOSUPPORT;

    // 4。计算事项个数    
    *retval = (pWrap->nTailIndex+MAX_AIE_NUM-rp)%MAX_AIE_NUM;
    
    return DB_ERR_OK;    
}

/************************************************************
	功能：AIE缓冲区是否为空。
	参数：无
	返回：TRUE/FALSE
************************************************************/
BOOL DB_AIEIsEmpty()
{
	TEventAIWrap	*pWrap;
	STATUS			rc;
	WORD			rp;

    // 1。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return TRUE;

    // 2。获取读指针    
    rc = DB_GetAIERead(&rp);
	if(rc != DB_ERR_OK)
		return TRUE;
    
    return (pWrap->nTailIndex == rp);    
}

/************************************************************
	功能：从数据库中读取一条AIE数据
	参数：rp		--  读取数据用的指针,当BLANK时用内部读指针
	输出：event		--  返回的数据 
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_ReadAIE(TEventAI *event, DWORD rp )
{
    TEventAIWrap 	*pWrap;
	TAppInfo		*pAppInfo;
	
    // 1。检查事项指针的合法性
    if( event==NULL )
        return DB_ERR_BUFNULL;
                     
    // 2。获取事项库指针
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap == NULL )
        return DB_ERR_BASENULL;
       
	pAppInfo = DB_GetAppInfo();
	// 3。对无效指针，直接读取当前指针的数据，否则读取指定指针数据
    if( rp == DB_BLANK )
    {
		if(pAppInfo == NULL)
			return DB_ERR_NOSUPPORT;

		rp = pAppInfo->nAIERead%MAX_AIE_NUM;
    }
    else
    {
		rp = rp%MAX_AIE_NUM;
    }
                       
	// 4。读取事项信息
	*event = pWrap->pEvent[rp];
	
	// 5。事项读取操作清除事项标志
    DB_ClearEventFlag(pAppInfo,APP_EVENT_AIE);
                                     
	return DB_ERR_OK;
}

DWORD nTestAIEWriteCnt = 0;

/************************************************************
	功能：向设备中写入一条AIE数据
	参数：pEvent,	--  写入的数据 
 	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_WriteAIE( TEventAI *pEvent )
{
    TEventAIWrap 	*pWrap;
    TAppInfo 		*pAppInfo;
    WORD 			i;
                     
	// 1。检查事项指针的合法性
	if( pEvent == NULL )
		 return DB_ERR_BUFNULL;
					  
	// 2。获取事项库指针
	pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
	if( pWrap == NULL )
		 return DB_ERR_BASENULL;
	//将事件中的遥测的原始值值转换为实际遥测值，并放大响应的倍数
	TAIPara			tAIPara;	
	TCF_Disp		tCFDisp;
	long			lCFValue = 1;
	long			lCFRatio = 1;
	float 			value;
	if(DB_GetAIPara(pEvent->nNo,&tAIPara) == DB_ERR_OK)											
	{
		if((DB_GetCFofAI(pEvent->nNo, &lCFValue) != DB_ERR_OK)||(lCFValue == 0))
		{
			lCFValue = 1;
			LogError("DB_ReadFloatAI",FILE_LINE,"InValid CF !!!");
		}		
		if((DB_GetCFDispofAI(pEvent->nNo, &tCFDisp) != DB_ERR_OK)||(tCFDisp.wCof == 0))
		{
			lCFRatio = 1;
			LogError("DB_ReadFloatAI",FILE_LINE,"InValid COF !!!");
		}
		else
			lCFRatio = tCFDisp.wCof;
		
		value = (float)(pEvent->lValue*lCFRatio)/lCFValue;
		switch(tAIPara.nSecDec)
		{
		case 0: pEvent->lValue = (long)value; break;	
		case 1: pEvent->lValue = (long)(value*10); break; 
		case 2: pEvent->lValue = (long)(value*100); break;	
		case 3: pEvent->lValue = (long)(value*1000); break;	
		case 4: pEvent->lValue = (long)(value*10000); break;	
		default: pEvent->lValue = (long)(value*100000); break;	
		}
	}
	// 3。存数据
	OS_Protect(0);

    pWrap->pEvent[pWrap->nTailIndex] = *pEvent;
    pWrap->nTailIndex = (pWrap->nTailIndex+1)%MAX_AIE_NUM;

	// 4。修正事项库头指针
	if( pWrap->nHeadIndex==pWrap->nTailIndex )
		 pWrap->nHeadIndex = (pWrap->nHeadIndex+1)%MAX_AIE_NUM;

	OS_UnProtect(0);
	
	// 5。修正各任务读指针,并注册事项信息
    pAppInfo = G_pCfgTable->TaskApp;
	for(i=0;i<DAE_MAX_APP_NUM;i++, pAppInfo++)
	{
		if(!(pAppInfo->bMask & APP_EVENT_AIE))
		{
			pAppInfo->nAIERead = pWrap->nTailIndex;
			continue;
		}
	    if( pAppInfo->nAIERead == pWrap->nTailIndex )
	         pAppInfo->nAIERead = (pWrap->nTailIndex+1)%MAX_AIE_NUM;
		if(!DB_GetEventFlag(pAppInfo, APP_EVENT_AIE))
		{
			DB_SetEventFlag(pAppInfo, APP_EVENT_AIE);
			//由数据库任务统一发消息，防止雪崩时不能及时处理
			if(pAppInfo->hTaskID != APP_TASKID_NULL)
				OS_PostMessage(pAppInfo->hTaskID, (WORD)SM_AIEVENT, 0,0,0,0);
		}
	}

	nTestAIEWriteCnt++;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：取AIE缓冲区头指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetAIEHead(WORD *retval)
{
	TEventAIWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nHeadIndex%MAX_AIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：取AIE缓冲区尾指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetAIETail(WORD *retval)
{
	TEventAIWrap	*pWrap;

    // 1。检查返回数据指针合法性            
    if( retval==NULL )
        return DB_ERR_BUFNULL;

    // 2。获取事项库指针             
    pWrap = G_pCfgTable->DBaseConfig.pEventAIWrap;
    if( pWrap==NULL )
        return DB_ERR_BASENULL;

	*retval = pWrap->nTailIndex%MAX_AIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：取AIE缓冲区读指针
	参数：*retval	返回数据指针
	返回：正确返回0，错误返回错误值
************************************************************/
STATUS DB_GetAIERead(WORD *retval)
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
    *retval = pAppInfo->nAIERead%MAX_AIE_NUM;

	return DB_ERR_OK;
}

/************************************************************
	功能：递增AIE缓冲区读指针
	参数：无
	返回：正确返回0，错误返回错误代码
************************************************************/
STATUS DB_IncAIERead()
{
	TAppInfo		*pAppInfo;

    // 1。取当前任务
	pAppInfo = DB_GetAppInfo();
    if( pAppInfo==NULL )
        return DB_ERR_NOSUPPORT;
                   
	// 2。递增指针
    pAppInfo->nAIERead = (pAppInfo->nAIERead+1)%MAX_AIE_NUM;

	//DB_RegisterFileChange(DB_FILE_BASICEV);
		
	return DB_ERR_OK;
}

/************************************************************
	功能：取系统控制字
	参数：NONE
	返回：控制字
************************************************************/
DWORD DB_GetSystemControlWord()
{
	return G_pCfgTable->DBaseConfig.dwControl;
}

/************************************************************
	功能：修改系统控制字
	参数：dwCtrlWord 控制字
	返回：修改结果
************************************************************/
STATUS DB_SetSystemControlWord(DWORD dwCtrlWord)
{	
	G_pCfgTable->DBaseConfig.dwControl = dwCtrlWord;
	G_pCfgTable->DBaseConfig.tMapData.pConfig->dwControl = ltobl(dwCtrlWord);

	//保存保护数据文件
	return DB_ModifyParaFile(DB_PARA_SYS,DB_SYSPARA_ADDR);
}

