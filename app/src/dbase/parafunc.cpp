/********************************************************************************************************/
/*																										*/
/*        						天津凯发电气股份有限公司												*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      parafunc.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义参数相关的接口函数																*/
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
#include "parafunc.h"		
#include "daefunc.h"		
#include "dbfunc.h"			

extern	TSysConfigTable	*G_pCfgTable;

STATUS DB_RegisterParaRuntimeOpt(WORD wType,WORD no)
{
	TRunTimeOPT		*pParaOpt;
	
	pParaOpt	= G_pCfgTable->DBaseConfig.pParaOpt;
	ASSERT(pParaOpt != NULL);
	if(pParaOpt == NULL)
	{
        LogError("DB_RegisterParaRuntimeOpt",FILE_LINE,"The Para RuntinmeOpt is null!");
        return DB_ERR_BASENULL;
	}
	
	pParaOpt->dwAppID 	= RtosGetCurrentTaskId();
	pParaOpt->dwOption1 = wType;
	pParaOpt->dwOption2 = no;
	pParaOpt->wTimeCnt 	= OPT_TIMEOUT_NUM;
	pParaOpt->wStatus 	= OPT_STATUS_SET;

	return DB_ERR_OK;
}

STATUS DB_UnRegisterParaRuntimeOpt()
{
	TRunTimeOPT		*pParaOpt;
	
	pParaOpt	= G_pCfgTable->DBaseConfig.pParaOpt;
	ASSERT(pParaOpt != NULL);

	pParaOpt->dwAppID 	= APP_TASKID_NULL;
	pParaOpt->wTimeCnt 	= 0;
	pParaOpt->wStatus 	= OPT_STATUS_READY;	

	return DB_ERR_OK;
}

STATUS DB_CheckParaRuntimeOpt(WORD wType,WORD no)
{
	TRunTimeOPT		*pParaOpt;
	
	pParaOpt	= G_pCfgTable->DBaseConfig.pParaOpt;
	ASSERT(pParaOpt != NULL);
	
	if(pParaOpt->dwAppID 	!= RtosGetCurrentTaskId())
		return DB_ERR_OPT_TASK;
	
	if(pParaOpt->wStatus 	!= OPT_STATUS_SET)
		return DB_ERR_OPT_STATUS;
		
	if(pParaOpt->dwOption1 != wType)
		return DB_ERR_OPT_PARA;

//	if(pParaOpt->dwOption2 != no)
//		return DB_ERR_OPT_PARA;

	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中读出指定直接测量点参数					
	参数：no,    	--  直接测量点的序号					
		  P			--  直接测量点参数值指针				
	返回：正确返回0，错误返回错误值。					
************************************************************/
STATUS DB_GetDAIPara( WORD no, TDAIPara  *p )
{
	TDAIPara		*pDAI;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nDAI;
	pDAI	= G_pCfgTable->DBaseConfig.pDAIPara;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetDAIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pDAI == NULL )
    {
        LogError("DB_GetDAIPara",FILE_LINE,"The DAI Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pDAI[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个直接测量点参数
	参数：no,    	--  直接测量点的序号
		  P			--  直接测量点参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteDAIPara( WORD no, TDAIPara *p )
{
	TDAIPara		*pDAI;
    WORD 			num;
               
	pDAI		= G_pCfgTable->DBaseConfig.tModData.pDAIAttr;
	num 		= G_pCfgTable->DBaseConfig.nAI;
	
	if(no >= num)
	{
		LogError("DB_WriteDAIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pDAI == NULL )
    {
        LogError("DB_WriteDAIPara",FILE_LINE,"The DAI Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pDAI = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_DAI,no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：确认向数据库中写入一个直接测量点参数
	参数：no,    	--  直接测量点的序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteDAIParaConfirm( WORD no )
{
	TDAIPara 		*pDAI;
	TDAIPara 		*pDAIFile;
	TDAIPara 		*pDAIOnline;
	STATUS			rc;
    WORD 			num;
               
	pDAI 		= G_pCfgTable->DBaseConfig.pDAIPara+no;
	pDAIFile	= G_pCfgTable->DBaseConfig.tMapData.pDAIAttr+no;
	pDAIOnline 	= G_pCfgTable->DBaseConfig.tModData.pDAIAttr;
	num  		= G_pCfgTable->DBaseConfig.nDAI;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_DAI,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteDAIParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_WriteDAIParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pDAI == NULL)||(pDAIFile == NULL)||(pDAIOnline == NULL))
    {
        LogError("DB_WriteDAIParaConfirm",FILE_LINE,"The DAI base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	*pDAI = *pDAIOnline;
	
	pDAIFile->wLineNo 	= ltobs(pDAIOnline->wLineNo);
	pDAIFile->wClass	= ltobs(pDAIOnline->wClass);

	// 4。保存参数数据文件
	DB_ModifyParaFile(DB_PARA_DAI,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();

	return DB_ERR_OK;
}

/************************************************************
	功能：取数字量直接采样点参数
	参数：no,   -- 采样点序号
		p  		-- 采样点参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetDIPara ( WORD no, TDIPara *p )
{
	TDIPara			*pDI;
    WORD 			i,num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nDI;
	pDI		= G_pCfgTable->DBaseConfig.pDIPara;
	
	// 检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_GetDIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pDI == NULL )
    {
        LogError("DB_GetDIPara",FILE_LINE,"The DI Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	if(no == DB_PARA_ALL)
		for(i=0;i<num;i++,p++,pDI++)
		{
			*p = *pDI;
		}
	else
		*p = pDI[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：写数字量直接采样点参数
	参数：no,   -- 采样点序号
	    p  		-- 采样点参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteDIPara ( WORD no, TDIPara *p )
{
	TDIPara 		*pDIFile;
    WORD 			num,i;

	num 	   = G_pCfgTable->DBaseConfig.nDI;

	// 1. 编号判断
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteDIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 指针判断
    if((G_pCfgTable->DBaseConfig.pDIPara == NULL)||\
		(G_pCfgTable->DBaseConfig.tMapData.pDIAttr == NULL))
    {
        LogError("DB_WriteDIPara",FILE_LINE,"The DI para base is null!");
        return DB_ERR_BASENULL;
    }

	// 3. 写入文件
	if(no == DB_PARA_ALL)
	{
		pDIFile	   = G_pCfgTable->DBaseConfig.tModData.pDIAttr;

		// 全部更新
		for(i=0;i<num;i++,p++,pDIFile++)
		{
			pDIFile->wFilterTime = ltobs(p->wFilterTime);
			pDIFile->wClass = ltobs(p->wClass);
		}
	}
	else
	{
		// 单个更新
		pDIFile	   = G_pCfgTable->DBaseConfig.tModData.pDIAttr+no;
		
		pDIFile->wFilterTime		= ltobs(p->wFilterTime);
		pDIFile->wClass		= ltobs(p->wClass);
	}

	// 注册文件修改
	DB_RegisterParaRuntimeOpt(DB_PARA_DI, no);
	
	return DB_ERR_OK;	
}

/************************************************************
	功能：取数字量直接采样点参数头指针
	参数：p   -- 采样点参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
TDIPara *DB_GetDIParaHead ()
{
	return G_pCfgTable->DBaseConfig.pDIPara;
}

/************************************************************
	功能：写数字量直接采样点参数确认
	参数：no,   -- 采样点序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteDIParaConfirm ( WORD no )
{
	TDIPara 		*pDI;
	TDIPara 		*pDIFile;
	TDIPara 		*pDIOnline;
	WORD			wFilterTime[MAX_DI_NUM+1];
	STATUS			rc;
    WORD 			i,num;
               
	pDI 		= G_pCfgTable->DBaseConfig.pDIPara;
	pDIFile		= G_pCfgTable->DBaseConfig.tMapData.pDIAttr;
	pDIOnline 	= G_pCfgTable->DBaseConfig.tModData.pDIAttr;
	num  		= G_pCfgTable->DBaseConfig.nDI;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_DI,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteDIParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteDIParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pDI == NULL)||(pDIFile == NULL)||(pDIOnline == NULL))
    {
        LogError("DB_WriteDIParaConfirm",FILE_LINE,"The DI base is null!");
        return DB_ERR_BASENULL;
    }

	// 提取遥信去抖时间
	EEPROM_Read(EEPROM_DI_FILTERTIME,sizeof(WORD)*(MAX_DI_NUM+1),wFilterTime);

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	if(no == DB_PARA_ALL)
	{
		// 全部更新
		for(i=0;i<num;i++,pDI++,pDIFile++,pDIOnline++)
		{
			pDI->wFilterTime = pDIOnline->wFilterTime;
			pDI->wClass = pDIOnline->wClass;
			if(i<MAX_DI_NUM)
				wFilterTime[i] = pDIOnline->wFilterTime;
			pDIFile->wFilterTime = ltobs(pDIOnline->wFilterTime);
			pDIFile->wClass = ltobs(pDIOnline->wClass);
		}
	}
	else
	{
		// 单个更新
		pDI += no;
		pDIFile += no;
		pDIOnline += no;
		
		pDI->wFilterTime = pDIOnline->wFilterTime;
		pDI->wClass = pDIOnline->wClass;
		if(no<MAX_DI_NUM)
			wFilterTime[no] = pDIOnline->wFilterTime;
		pDIFile->wFilterTime	= ltobs(pDIOnline->wFilterTime);
		pDIFile->wClass = ltobs(pDIOnline->wClass);
	}

	 // 保存EEPROM	
	if(num<MAX_DI_NUM)
		DAE_MakeCheckSum((BYTE *)wFilterTime,sizeof(WORD)*num);
	EEPROM_Write(EEPROM_DI_FILTERTIME,sizeof(WORD)*(MAX_DI_NUM+1),wFilterTime);

	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_DI,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();

	 // 6 消息通知DI任务
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DI), SM_BIPARACHANGE, 0, 0, 0, 0);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取一个遥测参数
	参数：no,    	--  遥测的序号
		  P			--  遥测参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetAIPara( WORD no, TAIPara  *p )
{
	TAIPara			*pAI;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nAI;
	pAI		= G_pCfgTable->DBaseConfig.pAIAttr;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetAIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pAI == NULL )
    {
        LogError("DB_GetAIPara",FILE_LINE,"The AI Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pAI[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：读取一个遥测的变比
	参数：no,    	--  序号
		  *fValue,  	--  变比数值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetRTofAI (WORD no, float *fValue)
{
	TAIPara			tAIPara;
	STATUS			rc;
               
	// 1。检查序号
	if(no >= G_pCfgTable->DBaseConfig.nAI)
	{
		LogError("DB_GetRTofAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2。检查指针
	if( fValue == NULL )
		return DB_ERR_BUFNULL;
	
	// 3。读取保护测量值描述
	rc = DB_GetAIPara(no, &tAIPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 4。读取系数描述
	rc = DB_GetRTValue(tAIPara.wRatioIndex, fValue);
	if(rc != DB_ERR_OK)
		return rc;

	return DB_ERR_OK;
}

/************************************************************
	功能：读取一个遥测的系数值
	参数：no,    	--  序号
		  *pValue,  --  系数值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetCFofAI (WORD no, long *pValue)
{
	TAIPara			tAIPara;
	STATUS			rc;
    WORD 			num;
               
	if( pValue == NULL )
		return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nAI;
	
	// 1。检查序号
	if(no >= num)
	{
		LogError("DB_GetCFofAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2。检查指针
	if( pValue == NULL )
		return DB_ERR_BUFNULL;

	// 3。读取保护测量值描述
	rc = DB_GetAIPara(no, &tAIPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 4。读取系数描述
	return DB_GetCFValue(tAIPara.nCFIndex, pValue);
}

/************************************************************
	功能：读取一个遥测的系数描述
	参数：no,    		--  序号
		  *pCFDisp,  	--  系数描述
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetCFDispofAI (WORD no, TCF_Disp *pCFDisp)
{
	TAIPara		tAIPara;
	STATUS			rc;
    WORD 			num;
               
	num 	= G_pCfgTable->DBaseConfig.nAI;
	
	// 1。检查序号
	if(no >= num)
	{
		LogError("DB_GetCFofAI",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	// 2。检查指针
	if( pCFDisp == NULL )
		return DB_ERR_BUFNULL;

	// 3。读取保护测量值描述
	rc = DB_GetAIPara(no, &tAIPara);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 4。读取系数描述
	return DB_GetCFDisp(tAIPara.nCFIndex, pCFDisp);
}
		
	
/************************************************************
	功能：向数据库中写入一个遥测参数
	参数：no,    	--  遥测的序号
		  P			--  遥测参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteAIPara( WORD no, TAIPara  * p )
{
	TAIPara			*pAI;
    WORD 			num;
               
	pAI			= G_pCfgTable->DBaseConfig.tModData.pAIAttr;
	num 		= G_pCfgTable->DBaseConfig.nAI;
	
	if(no >= num)
	{
		LogError("DB_WriteAIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pAI == NULL )
    {
        LogError("DB_WriteAIPara",FILE_LINE,"The AI Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pAI = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_AI, no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个遥测参数确认
	参数：no,    	--  遥测的序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteAIParaConfirm( WORD no )
{
	TAIPara 		*pAI;
	TAIPara 		*pAIFile;
	TAIPara 		*pAIOnline;
	STATUS			rc;
    WORD 			num;
               
	pAI 		= G_pCfgTable->DBaseConfig.pAIAttr+no;
	pAIFile		= G_pCfgTable->DBaseConfig.tMapData.pAIAttr+no;
	pAIOnline 	= G_pCfgTable->DBaseConfig.tModData.pAIAttr;
	num  		= G_pCfgTable->DBaseConfig.nAI;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_AI,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteAIParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_WriteAIParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pAI == NULL)||(pAIFile == NULL)||(pAIOnline == NULL))
    {
        LogError("DB_WriteAIParaConfirm",FILE_LINE,"The AI base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	*pAI = *pAIOnline;
	
	pAIFile->wNameIndex 	= ltobs(pAIOnline->wNameIndex);
	pAIFile->wRatioIndex 	= ltobs(pAIOnline->wRatioIndex);
	pAIFile->wControl 		= ltobs(pAIOnline->wControl);
	pAIFile->a 				= ltobs(pAIOnline->a);
	pAIFile->b 				= ltobs(pAIOnline->b);
	pAIFile->c 				= ltobs(pAIOnline->c);
	pAIFile->wDeadLine		= ltobs(pAIOnline->wDeadLine);
	pAIFile->lHiUpLimit		= ltobl(pAIOnline->lHiUpLimit);
	pAIFile->lUpLimit		= ltobl(pAIOnline->lUpLimit);
	pAIFile->lDownLimit		= ltobl(pAIOnline->lDownLimit);
	pAIFile->lLowDownLimit	= ltobl(pAIOnline->lLowDownLimit);

	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_AI,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取一个遥信参数
	参数：no,    	--  遥信的序号
		  P			--  遥信参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetBIPara( WORD no,TBIPara  * p )
{
	TBIPara			*pBI;
    WORD 			i,num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nBI;
	pBI		= G_pCfgTable->DBaseConfig.pBIAttr;
	
	// 检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_GetBIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBI == NULL )
    {
        LogError("DB_GetBIPara",FILE_LINE,"The BI Para Base is null!");
        return DB_ERR_BASENULL;
    }

	if(no == DB_PARA_ALL)
		for(i=0;i<num;i++,p++,pBI++)
		{
			*p = *pBI;
		}
	else
		*p = pBI[no];
	
	return DB_ERR_OK;
}

/************************************************************
	功能：取遥信参数头指针
	参数：
	返回：遥信参数指针
************************************************************/
TBIPara *DB_GetBIParaHead ()
{
	return G_pCfgTable->DBaseConfig.pBIAttr;
}
		
/************************************************************
	功能：向数据库中写入一个遥信参数
	参数：no,    	--  遥信的序号,0xFFFF代表所有数据
		  P			--  遥信参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteBIPara( WORD no, TBIPara  * p)
{
	TBIPara 		*pBIFile;
    WORD 			num,i;

	num 	   = G_pCfgTable->DBaseConfig.nBI;

	// 1. 编号判断
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteBIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 指针判断
    if((G_pCfgTable->DBaseConfig.pBIAttr == NULL)||\
		(G_pCfgTable->DBaseConfig.tMapData.pBIAttr == NULL))
    {
        LogError("DB_WriteBIPara",FILE_LINE,"The BI para base is null!");
        return DB_ERR_BASENULL;
    }

	// 3. 写入文件
	if(no == DB_PARA_ALL)
	{
		pBIFile	   = G_pCfgTable->DBaseConfig.tModData.pBIAttr;

		// 全部更新
		for(i=0;i<num;i++,p++,pBIFile++)
		{
			pBIFile->wControl		= ltobs(p->wControl);
		}
	}
	else
	{
		// 单个更新
		pBIFile	   = G_pCfgTable->DBaseConfig.tModData.pBIAttr+no;
		
		pBIFile->wControl		= ltobs(p->wControl);
	}

	// 4。注册文件修改
	DB_RegisterParaRuntimeOpt(DB_PARA_BI, no);
	
	return DB_ERR_OK;	
}

/************************************************************
	功能：向数据库中写入一个遥信参数确认
	参数：no,    	--  遥信的序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteBIParaConfirm( WORD no )
{
	TBIPara 		*pBI;
	TBIPara 		*pBIFile;
	TBIPara 		*pBIOnline;
	STATUS			rc;
    WORD 			i,num;
               
	pBI 		= G_pCfgTable->DBaseConfig.pBIAttr;
	pBIFile		= G_pCfgTable->DBaseConfig.tMapData.pBIAttr;
	pBIOnline 	= G_pCfgTable->DBaseConfig.tModData.pBIAttr;
	num  		= G_pCfgTable->DBaseConfig.nBI;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_BI,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteBIParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteBIParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pBI == NULL)||(pBIFile == NULL)||(pBIOnline == NULL))
    {
        LogError("DB_WriteBIParaConfirm",FILE_LINE,"The BI base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	if(no == DB_PARA_ALL)
	{
		// 全部更新
		for(i=0;i<num;i++,pBI++,pBIFile++,pBIOnline++)
		{
			pBI->wControl = ltobs(pBIOnline->wControl);
			pBIFile->wControl 	= ltobs(pBIOnline->wControl);
		}
	}
	else
	{
		// 单个更新
		pBI += no;
		pBIFile += no;
		pBIOnline += no;
		
		pBI->wControl = ltobs(pBIOnline->wControl);
		pBIFile->wControl		= ltobs(pBIOnline->wControl);
	}
	
	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_BI,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();

	 // 6 消息通知DI任务
		OS_PostMessage(DAE_GetTaskID(SYS_PORTID_DI), SM_BIPARACHANGE, 0, 0, 0, 0);
	
	return DB_ERR_OK;
}
	
/************************************************************
	功能：读取脉冲电度个数
	参数：无
	返回：脉冲电度个数
************************************************************/
WORD	DB_GetPulseCINum()
{
	TCIPara 		*pCI;
	WORD			num,cnt=0;
			   
	num 	= G_pCfgTable->DBaseConfig.nCI;
	pCI 	= G_pCfgTable->DBaseConfig.pCIAttr;
	
	// 检查序号
	if( pCI == NULL )
		return 0;
	
	for(int i=0;i<num;i++)
	{
		if(pCI[i].wControl&CI_ATTR_PULSE)
			cnt++;
	}

	return cnt;
}

/************************************************************
	功能：读取积分电度个数
	参数：无
	返回：积分电度个数
************************************************************/
WORD	DB_GetIntegralCINum()
{
	TCIPara			*pCI;
    WORD 			num,cnt=0;
               
	num 	= G_pCfgTable->DBaseConfig.nCI;
	pCI		= G_pCfgTable->DBaseConfig.pCIAttr;
	
	// 检查序号
    if( pCI == NULL )
        return 0;
	
	for(int i=0,cnt=0;i<num;i++)
	{
		if(!(pCI[i].wControl&CI_ATTR_PULSE))
			cnt++;
	}

	return cnt;
}

/************************************************************
	功能：从数据库中取一个电度参数
	参数：no,    	--  电度的序号
		  P			--  电度参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCIPara( WORD no, TCIPara  * p )
{
	TCIPara			*pCI;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nCI;
	pCI		= G_pCfgTable->DBaseConfig.pCIAttr;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetCIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCI == NULL )
    {
        LogError("DB_GetCIPara",FILE_LINE,"The CI Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pCI[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：取电度参数头指针
	参数：
	返回：电度参数指针
************************************************************/
TCIPara *DB_GetCIParaHead ()
{
	return G_pCfgTable->DBaseConfig.pCIAttr;
}
		
/************************************************************
	功能：向数据库中写入一个电度参数
	参数：no,    	--  电度的序号
		  P			--  电度参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteCIPara( WORD no, TCIPara  * p )
{
	TCIPara			*pCI;
    WORD 			num;
               
	pCI			= G_pCfgTable->DBaseConfig.tModData.pCIAttr;
	num 		= G_pCfgTable->DBaseConfig.nCI;
	
	if(no >= num)
	{
		LogError("DB_WriteCIPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCI == NULL )
    {
        LogError("DB_WriteCIPara",FILE_LINE,"The CI Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pCI = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_CI, no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个电度参数确认
	参数：no,    	--  电度的序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteCIParaConfirm( WORD no )
{
	TCIPara 		*pCI;
	TCIPara 		*pCIFile;
	TCIPara 		*pCIOnline;
	STATUS			rc;
    WORD 			num;
               
	pCI 		= G_pCfgTable->DBaseConfig.pCIAttr+no;
	pCIFile		= G_pCfgTable->DBaseConfig.tMapData.pCIAttr+no;
	pCIOnline 	= G_pCfgTable->DBaseConfig.tModData.pCIAttr;
	num  		= G_pCfgTable->DBaseConfig.nCI;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_CI,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteCIParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_WriteCIParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pCI == NULL)||(pCIFile == NULL)||(pCIOnline == NULL))
    {
        LogError("DB_WriteCIParaConfirm",FILE_LINE,"The CI base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数区和文件镜像区
	*pCI = *pCIOnline;
	
	pCIFile->wNameIndex 	= ltobs(pCIOnline->wNameIndex);
	pCIFile->wControl 		= ltobs(pCIOnline->wControl);
	pCIFile->dwConst		= ltobl(pCIOnline->dwConst);
	pCIFile->a				= ltobs(pCIOnline->a);
	pCIFile->b				= ltobs(pCIOnline->b);
	pCIFile->wMaptoDINo		= ltobs(pCIOnline->wMaptoDINo);
		
	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_CI,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取一个遥调参数
	参数：no,    	--  遥调的序号
		  P			--  遥调参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetAOPara( WORD no, TAOPara  * p )
{
	TAOPara			*pAO;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nAO;
	pAO		= G_pCfgTable->DBaseConfig.pAOAttr;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetAOPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pAO == NULL )
    {
        LogError("DB_GetAOPara",FILE_LINE,"The AO Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pAO[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个遥调参数
	参数：no,    	--  遥调的序号
		  P			--  遥调参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteAOPara( WORD no, TAOPara  * p )
{
	TAOPara			*pAO;
    WORD 			num;
               
	pAO			= G_pCfgTable->DBaseConfig.tModData.pAOAttr;
	num 		= G_pCfgTable->DBaseConfig.nAO;
	
	if(no >= num)
	{
		LogError("DB_WriteAOPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pAO == NULL )
    {
        LogError("DB_WriteAOPara",FILE_LINE,"The AO Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pAO = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_AO, no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个遥调参数确认
	参数：no,    	--  遥调的序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteAOParaConfirm( WORD no )
{
	TAOPara 		*pAO;
	TAOPara 		*pAOFile;
	TAOPara 		*pAOOnline;
	STATUS			rc;
    WORD 			num;
               
	pAO 		= G_pCfgTable->DBaseConfig.pAOAttr+no;
	pAOFile		= G_pCfgTable->DBaseConfig.tMapData.pAOAttr+no;
	pAOOnline 	= G_pCfgTable->DBaseConfig.tModData.pAOAttr;
	num  		= G_pCfgTable->DBaseConfig.nAO;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_AO,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteAOParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if(no >= num)
	{
		LogError("DB_WriteAOParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pAO == NULL)||(pAOFile == NULL)||(pAOOnline == NULL))
    {
        LogError("DB_WriteAOParaConfirm",FILE_LINE,"The AO base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	*pAO = *pAOOnline;
	
	pAOFile->wNameIndex 	= ltobs(pAOOnline->wNameIndex);
	pAOFile->wControl 		= ltobs(pAOOnline->wControl);
	pAOFile->nID 			= ltobs(pAOOnline->nID);
	pAOFile->a 				= ltobs(pAOOnline->a);
	pAOFile->b 				= ltobs(pAOOnline->b);
	pAOFile->c 				= ltobs(pAOOnline->c);

	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_AO,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取一个遥控参数
	参数：no,    	--  遥控的序号
		  P			--  遥控参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetBOPara( WORD no, TBOPara  * p )
{
	TBOPara			*pBO;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nBO;
	pBO		= G_pCfgTable->DBaseConfig.pBOAttr;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetBOPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pBO == NULL )
    {
        LogError("DB_GetBOPara",FILE_LINE,"The BO Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pBO[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取遥控参数头指针
	参数：
	返回：采样点参数指针
************************************************************/
TBOPara * DB_GetBOParaHead ()
{
	return G_pCfgTable->DBaseConfig.pBOAttr;
}
		
/************************************************************
	功能：向数据库中写入一个遥控参数
	参数：no,    	--  遥控的序号
		  P			--  遥控参数值指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteBOPara( WORD no, TBOPara  * p )
{
	TBOPara 		*pBOFile;
    WORD 			num,i;

	num 	   = G_pCfgTable->DBaseConfig.nBO;

	// 1. 编号判断
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteBOPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 指针判断
    if((G_pCfgTable->DBaseConfig.pBOAttr == NULL)||\
		(G_pCfgTable->DBaseConfig.tMapData.pBOAttr == NULL))
    {
        LogError("DB_WriteBOPara",FILE_LINE,"The BO para base is null!");
        return DB_ERR_BASENULL;
    }

	// 3. 写入文件
	if(no == DB_PARA_ALL)
	{
		pBOFile	   = G_pCfgTable->DBaseConfig.tModData.pBOAttr;

		// 全部更新
		for(i=0;i<num;i++,pBOFile++,p++)
			pBOFile->wLastTime 	= ltobs(p->wLastTime);
	}
	else
	{
		// 单个更新
		pBOFile	   = G_pCfgTable->DBaseConfig.tModData.pBOAttr+no;
		
		pBOFile->wLastTime	= ltobs(p->wLastTime);
	}

	// 4。保存文件
	DB_RegisterParaRuntimeOpt(DB_PARA_BO, no);
	
	return DB_ERR_OK;	
}

/************************************************************
	功能：向数据库中写入一个遥控参数确认
	参数：no,    	--  遥控的序号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteBOParaConfirm( WORD no )
{
	TBOPara 		*pBO;
	TBOPara 		*pBOFile;
	TBOPara 		*pBOOnline;
	STATUS			rc;
    WORD 			i,num;
               
	pBO		  	= G_pCfgTable->DBaseConfig.pBOAttr;
	pBOFile		= G_pCfgTable->DBaseConfig.tMapData.pBOAttr;
	pBOOnline 	= G_pCfgTable->DBaseConfig.tModData.pBOAttr;
	num  		= G_pCfgTable->DBaseConfig.nBO;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_BO,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteBOParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteBOParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pBO == NULL)||(pBOFile == NULL)||(pBOOnline == NULL))
    {
        LogError("DB_WriteBOParaConfirm",FILE_LINE,"The BO base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	if(no == DB_PARA_ALL)
	{
		// 全部更新
		for(i=0;i<num;i++,pBO++,pBOFile++,pBOOnline++)
		{
			pBO->wLastTime = pBOOnline->wLastTime;
			pBOFile->wLastTime 	= ltobs(pBOOnline->wLastTime);
		}
	}
	else
	{
		// 单个更新
		pBO	 += no;
		pBOFile += no;
		pBOOnline += no;		
		pBO	 = G_pCfgTable->DBaseConfig.pBOAttr+no;		
		pBO->wLastTime = pBOOnline->wLastTime;
		pBOFile->wLastTime		= ltobs(pBOOnline->wLastTime);
	}
	
	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_BO,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：获取参数中的串口个数
	参数：无
	返回：串口个数
************************************************************/
BYTE	DB_GetSerialNum()
{
	return G_pCfgTable->DBaseConfig.nSerNum;
}

/************************************************************
	功能：从数据库中取一个串口参数
	参数：no,    	--  串口号
		  P			--  串口参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSerialPara( WORD no, TSerialPara * p )
{
	TSerialPara			*pSerial;
    WORD 				num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nSerNum;
	pSerial	= G_pCfgTable->DBaseConfig.pSerial;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetSerialPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSerial == NULL )
    {
        LogError("DB_GetSerialPara",FILE_LINE,"The Serial Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pSerial[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个串口参数
	参数：no,    	--  串口号
		  P			--  串口参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteSerialPara( WORD no, TSerialPara * p )
{
	TSerialPara			*pSerial;
    WORD 				num;
               
	pSerial		= G_pCfgTable->DBaseConfig.tModData.pSerial+no;
	num 		= G_pCfgTable->DBaseConfig.nSerNum;
	
	if(no >= num)
	{
		LogError("DB_WriteSerialPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSerial == NULL )
    {
        LogError("DB_WriteSerialPara",FILE_LINE,"The Serial Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pSerial = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_SER, no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个串口参数确认
	参数：no,    	--  串口号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteSerialParaConfirm( WORD no )
{
	TSerialPara 		*pSerial;
	TSerialPara 		*pSerialFile;
	TSerialPara 		*pSerialOnline;
	STATUS				rc;
    WORD 				num;
               
	pSerial 		= G_pCfgTable->DBaseConfig.pSerial;
	pSerialFile		= G_pCfgTable->DBaseConfig.tMapData.pSerial;
	pSerialOnline 	= G_pCfgTable->DBaseConfig.tModData.pSerial;
	num  		= G_pCfgTable->DBaseConfig.nSerNum;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_SER,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteSerialParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	static WORD mlgb = DB_PARA_ALL;
	
	if((no != mlgb)&&(no >= num))
	{
		LogError("DB_WriteSerialParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pSerial == NULL)||(pSerialFile == NULL)||(pSerialOnline == NULL))
    {
        LogError("DB_WriteSerialParaConfirm",FILE_LINE,"The Serial base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	for(int i=0;i<num;i++)
	{
		if(no != DB_PARA_ALL)
			i = no;
		
		pSerial[i] = pSerialOnline[i];
		
		pSerialFile[i].wPortID 		= ltobs(pSerialOnline[i].wPortID);
		pSerialFile[i].bFlag 		= ltobs(pSerialOnline[i].bFlag);
		pSerialFile[i].byBaud 		=  pSerialOnline[i].byBaud;
		pSerialFile[i].byDriver 	=  pSerialOnline[i].byDriver;
		
		if(no != DB_PARA_ALL)
			break;
	}

	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_ETH,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：获取参数中的CAN口个数
	参数：无
	返回：CAN口个数
************************************************************/
BYTE	DB_GetCANNum()
{
	return G_pCfgTable->DBaseConfig.nCanNum;
}

/************************************************************
	功能：从数据库中取一个CAN口参数
	参数：no,    	--  CAN口号
		  P			--  CAN口参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCANPara( WORD no, TCanBusPara * p )
{
	TCanBusPara			*pCAN;
    WORD 				num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nCanNum;
	pCAN	= G_pCfgTable->DBaseConfig.pCAN;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetCANPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCAN == NULL )
    {
        LogError("DB_GetCANPara",FILE_LINE,"The CAN Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pCAN[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个CAN口参数
	参数：no,    	--  CAN口号
		  P			--  CAN口参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteCANPara( WORD no, TCanBusPara * p )
{
	TCanBusPara			*pCAN;
    WORD 				num;
               
	pCAN = G_pCfgTable->DBaseConfig.tModData.pCANBus+no;
	num = G_pCfgTable->DBaseConfig.nCanNum;
	
	if(no >= num)
	{
		LogError("DB_WriteCANPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCAN == NULL )
    {
        LogError("DB_WriteCANPara",FILE_LINE,"The CAN Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pCAN = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_CAN, no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个CAN口参数确认
	参数：no,    	--  CAN口号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteCANParaConfirm( WORD no )
{
	TCanBusPara 		*pCAN;
	TCanBusPara 		*pCANFile;
	TCanBusPara 		*pCANOnline;
	STATUS				rc;
    WORD 				num;
               
	pCAN 			= G_pCfgTable->DBaseConfig.pCAN;
	pCANFile		= G_pCfgTable->DBaseConfig.tMapData.pCANBus;
	pCANOnline 		= G_pCfgTable->DBaseConfig.tModData.pCANBus;
	num  			= G_pCfgTable->DBaseConfig.nCanNum;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_CAN,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteCANParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteCANParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pCAN == NULL)||(pCANFile == NULL)||(pCANOnline == NULL))
    {
        LogError("DB_WriteCANParaConfirm",FILE_LINE,"The CAN base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	for(int i=0;i<num;i++)
	{
		if(no != DB_PARA_ALL)
			i = no;

		pCAN[i] = pCANOnline[i];
		
		pCANFile[i].wPortID 		= ltobs(pCANOnline[i].wPortID);
		pCANFile[i].wBaud 		= ltobs(pCANOnline[i].wBaud);
		pCANFile[i].dwMask 		=  ltobl(pCANOnline[i].dwMask);
		pCANFile[i].byDriver 	=  ltobl(pCANOnline[i].byDriver);

		if(no != DB_PARA_ALL)
			break;
	}

	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_ETH,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：获取参数中的以太网连接个数
	参数：无
	返回：以太网连接个数
************************************************************/
BYTE	DB_GetEthLinkNum()
{
	return G_pCfgTable->DBaseConfig.nEthNum;
}

/************************************************************
	功能：从数据库中取一个以太网连接参数
	参数：no,    	--  以太网连接号
		  P			--  以太网连接参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetETHPara( WORD no, TEthPortPara * p )
{
	TEthPortPara		*pEthernet;
    WORD 				num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nEthNum;
	pEthernet	= G_pCfgTable->DBaseConfig.pEthernet;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetETHPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pEthernet == NULL )
    {
        LogError("DB_GetETHPara",FILE_LINE,"The Ethernet Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pEthernet[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个CAN口参数
	参数：no,    	--  以太网连接号
		  P			--  以太网连接参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteETHPara( WORD no, TEthPortPara * p )
{
	TEthPortPara		*pEthernet;
    WORD 				num;
               
	pEthernet	= G_pCfgTable->DBaseConfig.tModData.pEthernet+no;
	num 		= G_pCfgTable->DBaseConfig.nEthNum;
	
	if(no >= num)
	{
		LogError("DB_WriteETHPara",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pEthernet == NULL )
    {
        LogError("DB_WriteETHPara",FILE_LINE,"The Ethernet Online Para is null!");
        return DB_ERR_BASENULL;
    }

	*pEthernet = *p;

	DB_RegisterParaRuntimeOpt(DB_PARA_ETH, no);
	
	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个以太网连接参数确认
	参数：no,    	--  以太网连接号
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_WriteETHParaConfirm( WORD no )
{
	TEthPortPara 		*pEthernet;
	TEthPortPara 		*pEthernetFile;
	TEthPortPara 		*pEthernetOnline;
	STATUS				rc;
    WORD 				num;
               
	pEthernet		= G_pCfgTable->DBaseConfig.pEthernet;
	pEthernetFile	= G_pCfgTable->DBaseConfig.tMapData.pEthernet;
	pEthernetOnline	= G_pCfgTable->DBaseConfig.tModData.pEthernet;
	num  			= G_pCfgTable->DBaseConfig.nEthNum;
	
	// 1。检查操作合法性
	rc = DB_CheckParaRuntimeOpt(DB_PARA_ETH,no);
	if( rc != DB_ERR_OK)
	{
		LogError("DB_WriteETHParaConfirm",FILE_LINE,"The Operate is illegal!",no);
		return rc;
	}
	
	// 2。检查序号
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_WriteETHParaConfirm",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if((pEthernet == NULL)||(pEthernetFile == NULL)||(pEthernetOnline == NULL))
    {
        LogError("DB_WriteETHParaConfirm",FILE_LINE,"The Ethernet base is null!");
        return DB_ERR_BASENULL;
    }

	// 3。根据在线修改区的结果分别写入参数和文件镜像区
	for(int i=0;i<num;i++)
	{
		if(no != DB_PARA_ALL)
			i = no;
		
		pEthernet[i] = pEthernetOnline[i];
	
		pEthernetFile[i].wPortID 		= ltobs(pEthernetOnline[i].wPortID);
		pEthernetFile[i].wDriver 		= ltobs(pEthernetOnline[i].wDriver);
		pEthernetFile[i].dwMask 		= ltobl(pEthernetOnline[i].dwMask);
		pEthernetFile[i].dwSideIP 		= ltobl(pEthernetOnline[i].dwSideIP);
		pEthernetFile[i].dwSideMask 	= ltobl(pEthernetOnline[i].dwSideMask);
		pEthernetFile[i].dwSidePort		= ltobl(pEthernetOnline[i].dwSidePort);
		pEthernetFile[i].dwSelfIP 		= ltobl(pEthernetOnline[i].dwSelfIP);
		pEthernetFile[i].dwSelfMask 	= ltobl(pEthernetOnline[i].dwSelfMask);
		pEthernetFile[i].dwSelfPort		= ltobl(pEthernetOnline[i].dwSelfPort);
		pEthernetFile[i].dwMode			= ltobl(pEthernetOnline[i].dwMode);
		pEthernetFile[i].dwGateWay1		= ltobl(pEthernetOnline[i].dwGateWay1);
		pEthernetFile[i].dwGateWay2		= ltobl(pEthernetOnline[i].dwGateWay2);
		pEthernetFile[i].dwDHCPIP		= ltobl(pEthernetOnline[i].dwDHCPIP);

		if(no != DB_PARA_ALL)
			break;
	}
	
	// 4。保存保护数据文件
	DB_ModifyParaFile(DB_PARA_ETH,no);
	
	// 5。退出修改参数过程
	DB_UnRegisterParaRuntimeOpt();
	
	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取一个软压板描述
	参数：no,    	--  软压板序号
		  P			--  软压板描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSWDisp ( WORD no, TSW_Disp* p )
{
	TSW_Disp		*pSWDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nSW;
	pSWDisp		= G_pCfgTable->DBaseConfig.pSWDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetSWDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSWDisp == NULL )
    {
        LogError("DB_GetSWDisp",FILE_LINE,"The SWDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pSWDisp[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个软压板描述
	参数：no,    	--  软压板序号
		  P			--  软压板描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteSWDisp ( WORD no, TSW_Disp * p );

/************************************************************
	功能：向数据库中写入一个软压板描述确认
	参数：no,    	--  软压板序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteSWDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个配置字描述
	参数：no,    	--  配置字序号
		  P			--  配置字描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCFGDisp ( WORD no, TCFG_Disp * p )
{
	TCFG_Disp		*pCFGDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nCFG;
	pCFGDisp	= G_pCfgTable->DBaseConfig.pCFGDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetCFGDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCFGDisp == NULL )
    {
        LogError("DB_GetCFGDisp",FILE_LINE,"The CFGDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pCFGDisp[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个配置字描述
	参数：no,    	--  配置字序号
		  P			--  配置字描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WritCFGDisp ( WORD no, TCFG_Disp * p );

/************************************************************
	功能：向数据库中写入一个配置字描述确认
	参数：no,    	--  配置字序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteCFGDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个定值描述
	参数：no,    	--  定值序号
		  P			--  定值描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSETDisp ( WORD no, TSET_Disp * p )
{
	TSET_Disp		*pSETDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nSET;
	pSETDisp	= G_pCfgTable->DBaseConfig.pSETDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetSETDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSETDisp == NULL )
    {
        LogError("DB_GetSETDisp",FILE_LINE,"The SETDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pSETDisp[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个定值描述
	参数：no,    	--  定值序号
		  P			--  定值描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WritSETDisp( WORD no, TSET_Disp * p );

/************************************************************
	功能：向数据库中写入一个定值描述确认
	参数：no,    	--  定值序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteSETDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个辅助定值描述
	参数：no,    	--  定值序号
		  P			--  辅助定值描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetSysSETDisp ( WORD no, TSysSET_Disp * p )
{
	TSysSET_Disp		*pSETDisp;
    WORD 				num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nSystemSET;
	pSETDisp	= G_pCfgTable->DBaseConfig.pSystemSETDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetSysSETDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pSETDisp == NULL )
    {
        LogError("DB_GetSysSETDisp",FILE_LINE,"The SysSETDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pSETDisp[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个辅助定值描述
	参数：no,    	--  定值序号
		  P			--  辅助定值描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_GetSysSETDisp ( WORD no, TSysSET_Disp * p );

/************************************************************
	功能：向数据库中写入一个辅助定值描述确认
	参数：no,    	--  辅助定值序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_SysSETDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个系数描述
	参数：no,    	--  系数序号
		  P			--  系数描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetCFDisp ( WORD no, TCF_Disp * p )
{
	TCF_Disp		*pCFDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nCF;
	pCFDisp		= G_pCfgTable->DBaseConfig.pCFDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetCFDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pCFDisp == NULL )
    {
        LogError("DB_GetCFDisp",FILE_LINE,"The CFDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pCFDisp[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个系数描述
	参数：no,    	--  系数序号
		  P			--  系数描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WritCFDisp ( WORD no, TCF_Disp * p );

/************************************************************
	功能：向数据库中写入一个系数描述确认
	参数：no,    	--  定值序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteCFDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个保护测量值描述
	参数：no,    	--  保护测量值序号
		  P			--  保护测量值描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetRMDisp ( WORD no, TRM_Disp * p )
{
	TRM_Disp		*pRMDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nRM;
	pRMDisp		= G_pCfgTable->DBaseConfig.pRMDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetRMDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pRMDisp == NULL )
    {
        LogError("DB_GetRMDisp",FILE_LINE,"The RMDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pRMDisp[no];

	return DB_ERR_OK;
}
		
/************************************************************
	功能：向数据库中写入一个保护测量值描述
	参数：no,    	--  保护测量值序号
		  P			--  保护测量值描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WritRMDisp ( WORD no, TRM_Disp * p );

/************************************************************
	功能：读取一个保护测量值的系数值
	参数：no,    	--  序号
		  *pValue,  	--  系数值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetCFofRM (WORD no, long *pValue)
{
	TRM_Disp		tRMDisp;
	STATUS			rc;
    WORD 			num;
               
	if( pValue == NULL )
		return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nRM;
	
	// 1。检查序号
	if(no >= num)
	{
		LogError("DB_GetCFofRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2。检查指针
	if( pValue == NULL )
		return DB_ERR_BUFNULL;

	// 3。读取保护测量值描述
	rc = DB_GetRMDisp(no, &tRMDisp);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 4。读取系数描述
	return DB_GetCFValue(tRMDisp.nCFIndex, pValue);
}

/************************************************************
	功能：读取一个保护测量值的系数描述
	参数：no,    		--  序号
		  *pCFDisp,  	--  系数描述
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetCFDispofRM (WORD no, TCF_Disp *pCFDisp)
{
	TRM_Disp		tRMDisp;
	STATUS			rc;
    WORD 			num;
               
	num 	= G_pCfgTable->DBaseConfig.nRM;
	
	// 1。检查序号
	if(no >= num)
	{
		LogError("DB_GetCFofRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}
	
	// 2。检查指针
	if( pCFDisp == NULL )
		return DB_ERR_BUFNULL;

	// 3。读取保护测量值描述
	rc = DB_GetRMDisp(no, &tRMDisp);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 4。读取系数描述
	return DB_GetCFDisp(tRMDisp.nCFIndex, pCFDisp);
}

/************************************************************
	功能：读取一个保护测量值的变比
	参数：no,    	--  序号
		  *fValue,  	--  变比数值
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetRTofRM (WORD no, float *fValue)
{
	TRM_Disp		tRMDisp;
	STATUS			rc;
               
	// 1。检查序号
	if(no >= G_pCfgTable->DBaseConfig.nRM)
	{
		LogError("DB_GetRTofRM",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2。检查指针
	if( fValue == NULL )
		return DB_ERR_BUFNULL;
	
	// 3。读取保护测量值描述
	rc = DB_GetRMDisp(no, &tRMDisp);
	if(rc != DB_ERR_OK)
		return rc;
	
	// 4。读取变比
	rc = DB_GetRTValue(tRMDisp.nRTIndex, fValue);
	if(rc != DB_ERR_OK)
		return rc;

	return DB_ERR_OK;
}

/************************************************************
	功能：向数据库中写入一个保护测量值描述确认
	参数：no,    	--  保护测量值序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteRMDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个保护动作描述
	参数：no,    	--  保护动作信息序号
		  P			--  保护动作信息描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetREDisp( WORD no, TRE_Disp * p )
{
	TRE_Disp		*pREDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nRE;
	pREDisp		= G_pCfgTable->DBaseConfig.pREDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetREDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pREDisp == NULL )
    {
        LogError("DB_GetREDisp",FILE_LINE,"The REDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pREDisp[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：从数据库中取一个传动参数
	参数：no,    	--  传动序号
		  P			--  传动参数描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetTCDisp ( WORD no, TTC_Disp* p )
{
	TTC_Disp		*pTCDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nTC;
	pTCDisp		= G_pCfgTable->DBaseConfig.pTCDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetTCDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pTCDisp == NULL )
    {
        LogError("DB_GetTCDisp",FILE_LINE,"The TCDisp Base is null!");
        return DB_ERR_BASENULL;
    }
	
	*p = pTCDisp[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：根据INF号读取一个保护动作描述
	参数：no,    	--  保护动作信息序号
		  P			--  保护动作信息描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetREDispByInf( WORD wInf, TRE_Disp * p )
{
	TRE_Disp		*pREDisp;
    WORD 			num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;

	if(wInf == RELAY_INF_NOUSED)
	{
		return DB_ERR_OK;
	}
	
	num 		= G_pCfgTable->DBaseConfig.nRE;
	pREDisp		= G_pCfgTable->DBaseConfig.pREDisp;
	
    if( pREDisp == NULL )
    {
        LogError("DB_GetREDisp",FILE_LINE,"The REDisp Base is null!");
        return DB_ERR_BASENULL;
    }

	for(int i=0;i<num;i++)
	{
		if(pREDisp[i].wInf == wInf)
		{
			*p = pREDisp[i];
			return DB_ERR_OK;
		}
	}
	
	memset(p,0,sizeof(TRE_Disp));
	strcpy(p->strCHName,"NULL");

	return DB_ERR_RELAY_INF;
}
		
/************************************************************
	功能：向数据库中写入一个保护动作描述
	参数：no,    	--  保护动作信息序号
		  P			--  保护动作信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WritREDisp( WORD no, TRE_Disp * p );

/************************************************************
	功能：向数据库中写入一个保护动作描述确认
	参数：no,    	--  保护动作信息序号
	返回：正确返回0，错误返回错误值。
************************************************************/
//STATUS DB_WriteREDispConfirm( WORD no );

/************************************************************
	功能：从数据库中取一个数据名称
	参数：no,    	--  数据名称条目号
		  P			--  数据名称
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetDataName( WORD no, char *p )
{
	char		*pItem;
    WORD 		num;
               
    if( p == NULL )
        return DB_ERR_BUFNULL;
	
	num 		= G_pCfgTable->DBaseConfig.nDataName;
	pItem		= G_pCfgTable->DBaseConfig.pDataName;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetDataName",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pItem == NULL )
    {
        LogError("DB_GetDataName",FILE_LINE,"The DataName Base is null!");
        return DB_ERR_BASENULL;
    }
	
	memcpy(p,pItem+no*SETNAME_MAXLENGTH,SETNAME_MAXLENGTH);

	return DB_ERR_OK;
}

/************************************************************
	功能：取定值的数据名称列表
	参数：no,    	--  定值序号
		  pName		--  定值数据名称列表
		  wNum		--  定值数据名称个数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetSETDataName(WORD no,char *pName,WORD &wNum)
{
	TSET_Disp			*pSETDisp;
	WORD				nItemStart,nItemNum,nDataName;
	
	ASSERT(pName != NULL)
	if(pName == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSETNum())
	if(no >= DB_GetSETNum())
		return DB_ERR_NO;
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSETDisp;
	nItemStart = pSETDisp[no].nItemStart;
	nItemNum = pSETDisp[no].nItemNum;

	nDataName = G_pCfgTable->DBaseConfig.nDataName;

	if(G_pCfgTable->DBaseConfig.pDataName == NULL)
		return DB_ERR_BASENULL;

	if(nItemStart > nDataName)
		return DB_ERR_NO;

	if((nItemStart+nItemNum) > nDataName)
		nItemNum = nDataName - nItemStart;
	
	memcpy(pName,G_pCfgTable->DBaseConfig.pDataName+nItemStart*SETNAME_MAXLENGTH,\
		nItemNum*SETNAME_MAXLENGTH);

	wNum = nItemNum;
	
	return DB_ERR_OK;
}

/************************************************************
	功能：取辅助定值的数据名称列表
	参数：no,    	--  辅助定值序号
		  pName		--  辅助定值数据名称列表
		  wNum		--  辅助定值数据名称个数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS	DB_GetSystemSETDataName(WORD no,char *pName,WORD &wNum)
{
	TSysSET_Disp		*pSETDisp;
	WORD				nItemStart,nItemNum,nDataName;
	
	ASSERT(pName != NULL)
	if(pName == NULL)
		return DB_ERR_PTRNULL;

	ASSERT(no < DB_GetSystemSETNum())
	if(no >= DB_GetSystemSETNum())
		return DB_ERR_NO;
	
	pSETDisp = G_pCfgTable->DBaseConfig.pSystemSETDisp;
	nItemStart = pSETDisp[no].nItemStart;
	nItemNum = pSETDisp[no].nItemNum;

	nDataName = G_pCfgTable->DBaseConfig.nDataName;

	if(G_pCfgTable->DBaseConfig.pDataName == NULL)
		return DB_ERR_BASENULL;

	if(nItemStart > nDataName)
		return DB_ERR_NO;

	if((nItemStart+nItemNum) > nDataName)
		nItemNum = nDataName - nItemStart;
	
	memcpy(pName,G_pCfgTable->DBaseConfig.pDataName+nItemStart*SETNAME_MAXLENGTH,\
		nItemNum*SETNAME_MAXLENGTH);

	wNum = nItemNum;
	
	return DB_ERR_OK;
}

STATUS DB_ModifyParaFile(WORD wType,WORD no)
{
    TFileHead         	*filehead;

	// 1。注册参数修改事件
	DB_Register_ParaChange(wType,no);

	if(G_pCfgTable->DBaseConfig.pBasicParaFileMap == NULL)
		return DB_ERR_FILE_WRITE;
	
	// 2。重新计算文件校验
	filehead = (TFileHead *)G_pCfgTable->DBaseConfig.pBasicParaFileMap->pRamData;
	ReadSystemTime(&filehead->stTime);
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-sizeof(WORD));

	// 3。写入文件数据
	DAE_SetRunningFlags(SS_FILE_BASICPARA_CHG);
	
	return DAE_WriteFile(PARACFGFILE, filehead,filehead->dwSize);
}

STATUS DB_ModifyRelayDispFile(WORD wType,WORD no)
{
    TFileHead         	*filehead;

	// 1。注册参数修改事件
	DB_Register_ParaChange(wType,no);

	if(G_pCfgTable->DBaseConfig.pRelayDispFileMap == NULL)
		return DB_ERR_FILE_WRITE;
	
	// 2。重新计算文件校验
	filehead = (TFileHead *)G_pCfgTable->DBaseConfig.pRelayDispFileMap->pRamData;
	ReadSystemTime(&filehead->stTime);
	DAE_MakeCheckSum((BYTE *)filehead, sizeof(TFileHead)-sizeof(WORD));
	DAE_MakeCheckSum((BYTE *)filehead, filehead->dwSize-sizeof(WORD));

	// 3。写入文件数据
	return DAE_WriteFile(RELAYDISPFILE, filehead,filehead->dwSize);
}

void *DB_GetParaModBuf(BYTE byParaType)
{
	switch(byParaType)
	{
		case DB_PARA_DI:
			return (void *)G_pCfgTable->DBaseConfig.tModData.pDIAttr;
		case DB_PARA_BI:
			return (void *)G_pCfgTable->DBaseConfig.tModData.pBIAttr;
		case DB_PARA_BO:
			return (void *)G_pCfgTable->DBaseConfig.tModData.pBOAttr;
		case DB_PARA_AO:
			return (void *)G_pCfgTable->DBaseConfig.tModData.pAOAttr;
		default:
			return NULL;
	}
}

/************************************************************
	功能：获取参数中的镜像位参数个数
	参数：无
	返回：CAN口个数
************************************************************/
BYTE	DB_GetMapBitObjNum()
{
	return G_pCfgTable->DBaseConfig.nMBO;
}


/************************************************************
	功能：取镜像位参数
	参数：	no,    	--  镜像位参数索引
			pDisp	--  镜像位参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetMapBitObjDisp(WORD no,TMBO_Disp *pDisp)
{
	TMBO_Disp			*pMBO;
    WORD 				num;
               
    if( pDisp == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nMBO;
	pMBO	= G_pCfgTable->DBaseConfig.pMBODisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetMapBitObjDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pMBO == NULL )
    {
        LogError("DB_GetMapBitObjDisp",FILE_LINE,"The map para base is null!");
        return DB_ERR_BASENULL;
    }
	
	*pDisp = pMBO[no];

	return DB_ERR_OK;
}

/************************************************************
	功能：设置镜像位参数
	参数：	no,    	--  镜像位参数索引
			pDisp	--  镜像位参数指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetMapBitObjDisp(WORD no,TMBO_Disp *pDisp)
{
	TFileHead			*filehead;
	TMBO_Disp			*pMBO,*pMBOFile;
    TRelayDispTable		*pRelayDispTable;
    WORD 				num,i;

	num 	 = G_pCfgTable->DBaseConfig.nMBO;
	pMBO 	 = G_pCfgTable->DBaseConfig.pMBODisp;
	pMBOFile = G_pCfgTable->DBaseConfig.tMapData.pMBODisp;

	// 1. 编号判断
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_SetMapBitObjDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 指针判断
    if((pMBO == NULL)||(pMBOFile == NULL))
    {
        LogError("DB_SetMapBitObjDisp",FILE_LINE,"The MBO para base is null!");
        return DB_ERR_BASENULL;
    }

	// 3. 修改文件镜像
	if(no == DB_PARA_ALL)
	{
		// 全部更新
		for(i=0;i<num;i++,pMBO++,pMBOFile++,pDisp++)
		{
			*pMBO = *pDisp;
			*pMBOFile = *pDisp;
			
			pMBOFile->wControl = ltobs(pDisp->wControl);
			pMBOFile->dwSideIP = ltobl(pDisp->dwSideIP);
			pMBOFile->dwBaud = ltobl(pDisp->dwBaud);
			pMBOFile->dwRsv = ltobl(pDisp->dwRsv);
		}
	}
	else
	{
		// 单个更新
		pMBO += no;
		pMBOFile += no;
		
		*pMBO = *pDisp;
		*pMBOFile = *pDisp;
		
		pMBOFile->wControl = ltobs(pDisp->wControl);
		pMBOFile->dwSideIP = ltobl(pDisp->dwSideIP);
		pMBOFile->dwBaud = ltobl(pDisp->dwBaud);
		pMBOFile->dwRsv = ltobl(pDisp->dwRsv);
	}

	// 4。写入文件
	DB_ModifyRelayDispFile(DB_PARA_MBO, no);

	DB_Register_ParaChange(DB_PARA_MBO,no);

	return DB_ERR_OK;	
}

/************************************************************
	功能：根据端口号获取串口镜像位对象描述
	参数：	wPortID		--	端口ID
			pDisp,    	--  镜像位对象参数
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_PortID2MapBitObjDisp(WORD wPortID,TMBO_Disp *pDisp)
{
	TMBO_Disp 			*pBase;
 	WORD 				num;
               
    if( pDisp == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nMBO;
	pBase	= G_pCfgTable->DBaseConfig.pMBODisp;
	
    if( pBase == NULL )
    {
        LogError("DB_GetSerialMapBitObjDisp",FILE_LINE,"The MBO Para Base is null!");
        return DB_ERR_BASENULL;
    }
	
	for(int i=0;i<num;i++)
		if(DB_GetMBOPortID(pBase+i) == wPortID)
		{
			*pDisp = pBase[i];
			return DB_ERR_OK;
		}
		
	return DB_ERR_FAILURE;
}

/************************************************************
	功能：取指定镜像位对象端口ID
	参数：pMBO,    	--  镜像位对象指针
	返回：正确返回0，错误返回错误值。
************************************************************/
WORD DB_GetMBOPortID(TMBO_Disp	*pMBO)
{
	switch(pMBO->wControl&0x0F)
	{
		case MAPBIT_PORTTYPE_ETHA:
			return (SC_PORT_ETHERNETA<<8);
			
		case MAPBIT_PORTTYPE_ETHB:
			return (SC_PORT_ETHERNETB<<8);
			
		case MAPBIT_PORTTYPE_CANA:	
			return (SC_PORT_CANBUS<<8)+1;
				
		case MAPBIT_PORTTYPE_CANB:
			return (SC_PORT_CANBUS<<8)+2;
	
		case MAPBIT_PORTTYPE_SERIAL1:
			return (SC_PORT_SERIAL<<8)+2;

		case MAPBIT_PORTTYPE_SERIAL2:
			return (SC_PORT_SERIAL<<8)+3;

		case MAPBIT_PORTTYPE_SERIAL3:
			return (SC_PORT_SERIAL<<8)+4;
	
		default:
			return 0xFFFF;
	}
}

/************************************************************
	功能：判断某个端口是否被镜像位占用
	参数：wPortID,    	--  端口ID
	返回：正确返回0，错误返回错误值。
************************************************************/
BOOL DB_IsMapBitPort(WORD wPortID)
{
	TMBO_Disp			*pMBO;
    WORD 				num;
    int 				i; 
	
	num 	= G_pCfgTable->DBaseConfig.nMBO;
	pMBO	= G_pCfgTable->DBaseConfig.pMBODisp;
	
	if(num == 0)
		return FALSE;
	
    if( pMBO == NULL )
		return FALSE;

	for(int i=0;i<num;i++)
		if(DB_GetMBOPortID(pMBO+i) == wPortID)
			return TRUE;

	return FALSE;
}

/************************************************************
	功能：获取参数中的测距对象描述
	参数：无
	返回：测距对象描述个数
************************************************************/
BYTE	DB_GetFLObjNum()
{
	return G_pCfgTable->DBaseConfig.nFL;
}


/************************************************************
功能：从数据库中取一个测距对象描述
参数：no,		--	测距对象序号
	  	pDisp 	--	测距对象描述信息指针
	返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_GetFLObjDisp ( WORD no,TFL_Disp * pDisp )
{
	TFL_Disp			*pFL;
    WORD 				num;
               
    if( pDisp == NULL )
        return DB_ERR_BUFNULL;
	
	num 	= G_pCfgTable->DBaseConfig.nFL;
	pFL	= G_pCfgTable->DBaseConfig.pFLDisp;
	
	// 检查序号
	if(no >= num)
	{
		LogError("DB_GetFLObjDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

    if( pFL == NULL )
    {
        LogError("DB_GetFLObjDisp",FILE_LINE,"The map para base is null!");
        return DB_ERR_BASENULL;
    }
	
	*pDisp = pFL[no];

	return DB_ERR_OK;
}

/************************************************************
功能：向数据库中直接写入一个测距对象描述
参数：no,		--	测距对象序号
	  	pDisp 	--	测距描述信息指针
返回：正确返回0，错误返回错误值。
************************************************************/
STATUS DB_SetFLObjDisp ( WORD no, TFL_Disp * pDisp )
{
	TFileHead			*filehead;
	TFL_Disp			*pFL,*pFLFile;
    TRelayDispTable		*pRelayDispTable;
    WORD 				num,i;

	num 	 = G_pCfgTable->DBaseConfig.nFL;
	pFL 	 = G_pCfgTable->DBaseConfig.pFLDisp;
	pFLFile = G_pCfgTable->DBaseConfig.tMapData.pFLDisp;

	// 1. 编号判断
	if((no != DB_PARA_ALL)&&(no >= num))
	{
		LogError("DB_SetMapBitObjDisp",FILE_LINE,"The no=%d is overflow!",no);
		return DB_ERR_NO;
	}

	// 2. 指针判断
    if((pFL == NULL)||(pFLFile == NULL))
    {
        LogError("DB_WriteChannelPara",FILE_LINE,"The CH para base is null!");
        return DB_ERR_BASENULL;
    }

	// 3. 修改文件镜像
	if(no == DB_PARA_ALL)
	{
		// 全部更新
		for(i=0;i<num;i++,pFL++,pFLFile++,pDisp++)
		{
			*pFL = *pDisp;
			*pFLFile = *pDisp;
			
			pFLFile->wControl = ltobs(pDisp->wControl);
			pFLFile->byNetAddr = pDisp->byNetAddr;
			pFLFile->byFLNo = pDisp->byFLNo;
			pFLFile->dwMNetIP = ltobl(pDisp->dwMNetIP);
			pFLFile->dwSNetIP = ltobl(pDisp->dwSNetIP);
		}
	}
	else
	{
		// 单个更新
		pFL += no;
		pFLFile += no;
		
		*pFL = *pDisp;
		
		pFLFile->wControl = ltobs(pDisp->wControl);
		pFLFile->byNetAddr = pDisp->byNetAddr;
		pFLFile->byFLNo = pDisp->byFLNo;
		pFLFile->dwMNetIP = ltobl(pDisp->dwMNetIP);
		pFLFile->dwSNetIP = ltobl(pDisp->dwSNetIP);
	}

	// 4。写入文件
	DB_ModifyRelayDispFile(DB_PARA_FL, no);

	return DB_ERR_OK;	
}

