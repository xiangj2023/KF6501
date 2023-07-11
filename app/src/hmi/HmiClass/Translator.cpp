/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			Translator.cpp
* 
*   软件模块：液晶 多语言翻译
*			  		
* 
*	描述	多语言6500和1300及pc上应用的主体功能代码是相同的，
*			1300和pc上的数据库翻译部分CDBtranslator放在DBtranslator.cpp，除引用时对个数的判断外 完全相同
				文件检索管理CLanFileMan部分放在LangfileMan.cpp 初始化不相同
				翻译函数引用接口TR_Load，TR_PerviewLang 等放在ktrans.cpp，完全相同
*			6500的都直接放在Translator.cpp文件的结尾 
* 			6500比其他多一组const char* TR_GetSETName(const TSET_Disp* pSET_Disp, WORD wIndex)函数
* 
* 
*   函数 	LONG Load(char* LanName) 装载语言文件， LanName是语言文件名
*			char* GetTrString(char* str) 得到字符串str的翻译字符串
*			char* GetTrString(char* pContext, char* str) 	得到上下文为pContext的字符串str的翻译字符串		
*
*
*	CDBTranslator 函数
*	
*   函数  
*	long Load(const BYTE* pFilebuf, DWORD dwLen) 加载数据库翻译
*	long UnLoad() 卸载数据库字符串翻译，选中中文的时候序号调用该函数
*	const char* GetSetTr(WORD wIndex); 读取定值译文
*	const char* GetSysSetTr(WORD wIndex); 读取辅助定值译文
*	const char* GetReTr(WORD wInf); //通过inf号读取报告名称译文
*	const char* GetTCTr(WORD wIndex); 读取传动译文
*	const char* GetSWTr(WORD wIndex); 读取软压板译文
*	const char* GetCFGTr(WORD wIndex); 读取配置字译文
*	读取字符串列表
*	BOOL GetTxtList(WORD wStart, WORD wCount, char* pNameList);
*
*	为了方便使用，翻译时不需要调用CDBTranslator的成员函数，直接使用以下接口函数
const char* TR_GetSETName(const TSET_Disp* tSET_Disp, WORD wIndex );
const char* TR_GetSysSETName(const TSysSET_Disp* tSysSET_Disp,WORD wIndex);
const char* TR_GetREName(const TRE_Disp* tRE_Disp,WORD wIndex );
const char* TR_GetTCName(const TTC_Disp* tTC_Disp, WORD wIndex );
const char* TR_GetCFGName(const TCFG_Disp* tCFG_Disp,WORD wIndex );
const char* TR_GetSWName(const TSW_Disp* tSW_Disp, WORD wIndex );
const char* TR_GetBIName(const TBIPara* pBIPara, WORD wIndex);
const char* TR_GetRMName(const TRM_Disp* pRM_Disp, WORD wIndex);
const char* TR_GetSETName(const TSET_Disp* pSET_Disp, WORD wIndex);

BOOL TR_GetSETDataName(const TSET_Disp* pSET_Disp, char* pName, WORD& wSetNameCount);
BOOL TR_GetSysSETDataName(const TSysSET_Disp* pSysSET_Disp,  char* pName, WORD& wSetNameCount);

*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2009-11-18		初始创建
*		kangqian	2011-08-16		增加CDBTranslator 及 CLanFileMan的代码
*		kangqian	2012-10-23		CDBTranslator增加测量值名称，遥信名称等，与1300保持一致
*************************************************************************/
#ifdef __PL_WIN_SIMULATION__
#include "../stdafx.h"
extern CFile g_Hashfile;
extern CFile g_DBHSfile;
#include "../WinGui.h"
#include "../WinGuiDlg.h"
#endif

#include "parafunc.h"		
#include "DaeFunc.h"
#include "dbfunc.h"			

#include "translator.h"

CTranslator g_Trans;
CDBTranslator g_DBTrans;
CLanFileMan g_LanFileMan;
/****************************************************************************************
 * 功能描述: 装载译文数据 
 * 注意：在调用该函数之前需要 先调用系统的读文件函数把译文数据从翻译文件读取到pbuf中，文件长度保存到dwFileLength
 * 参数说明: pbuf和dwFileLength都是输入参数， 调用函数前要正确初始化
 *			- const char* pbuf 传入指向保存了翻译数据的指针
 *			- DWORD dwFileLength 传入译文数据的长度
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
LONG TR_Load(const BYTE* pbuf, DWORD dwFileLength)
{
	if((pbuf == NULL)|| dwFileLength == 0)
	{
		//错误提示
		return -1;
	}
	return g_Trans.Load(pbuf, dwFileLength);
}

/****************************************************************************************
 * 功能描述: 预览译文名称
 * 参数说明: 
 *			-[in] const char* pbuf  传入译文头指针
 *			-[out] const char* pszTitle 返回的译文名称字符串,至少14个字节的空间
 * 返回代码: 0 成功 其他失败
 * 其它: 要得到某个译文文件的名称，首先从译文文件读取MULTILANG_PREVIEW_SIZE大小的数据
 *		到pbuf， 之后调用TR_PerviewLangTitle， 返回的译文名称保存在pszTitle中
 *****************************************************************************************/
LONG TR_PerviewLangTitle(const char* pbuf,  char* pszTitle)
{
	THashFileHead* pHashHead = (THashFileHead*)(pbuf+sizeof(TCommonFileHeadTrans));
	memcpy(pszTitle, pHashHead->szLangCaption,  TRANS_TITLE_SIZE);
	return 0;
}


/****************************************************************************************
 * 功能描述: 得到当前装载的译文的标题
 * 参数说明: -[out] const char* pszTitle 返回的译文名称字符串,至少14个字节的空间
 * 返回代码: 0 成功 其他失败
 * 其它: 调用过TR_Load以后才可以调用该函数返回装载的译文的标题
 *****************************************************************************************/
LONG TR_GetCurLangTitle( char* pszTitle)
{
	memcpy(pszTitle, g_Trans.GetLangCaption(),  TRANS_TITLE_SIZE);
	return 0;
}
WORD g_CalCheckSum(const BYTE *pBuf,DWORD nLen)
{
	WORD	wTmp = 0x55AA;
	DWORD	i;
	
	if(pBuf == NULL)
		return 0;
	
	for(i=0; i < nLen; i++)
	{
		wTmp += pBuf[i];
	}
	return ~wTmp;
}

LONG g_CheckFile(const BYTE* pFileBuf)
{
	TCommonFileHeadTrans* pCommonFileHead;
	pCommonFileHead = (TCommonFileHeadTrans*)pFileBuf;
	if(pCommonFileHead == NULL)
		return 1;
	if(pCommonFileHead->dwSize ==0)
		return 1;

	//1.检查文件头校验码是否正确
	//计算文件头校验码是否正确
	WORD wCheck = g_CalCheckSum((unsigned char *)pCommonFileHead, sizeof(TCommonFileHeadTrans)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);
	
	if(dwCheck != pCommonFileHead->dwCheck)
		return 2;

	//2.检查文件校验码
	//计算文件校验码
	wCheck = g_CalCheckSum((unsigned char*)(pCommonFileHead+1),
		pCommonFileHead->dwSize - 2-sizeof(TCommonFileHeadTrans));
	
	//@	//2011-3-10 9:32:14 pFileCheck 可能不按word对齐的,所以不直接用word*而是用字节拼合
	char* pFileCheck = ((char*)pCommonFileHead+pCommonFileHead->dwSize -2);
	WORD wFileCheck =  MAKEWORD(pFileCheck[0], pFileCheck[1]);
	
	if(wCheck != wFileCheck)
		return 3;
	return 0;
}

const char* TR_GetBIName(const TBIPara* pBIPara, WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetBITr(wIndex);
	if(str != NULL)
		return str;
	if(pBIPara)
		return pBIPara->strSymbol;
	else
		return NULL;
}

const char* TR_GetRMName(const TRM_Disp* pRM_Disp, WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetRMTr(wIndex);
	if(str != NULL)
		return str;
	if(pRM_Disp)
		return pRM_Disp->strSymbol;
	else
		return NULL;
}
const char* TR_GetSETName(const TSET_Disp* pSET_Disp, WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetSetTr(wIndex);
	if(str != NULL)
		return str;
	if(pSET_Disp)
		return pSET_Disp->strCHName;
	else
		return NULL;
}
const char* TR_GetSysSETName(const TSysSET_Disp* pSysSET_Disp,WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetSysSetTr(wIndex);
	if(str != NULL)
		return str;
	if(pSysSET_Disp)
		return pSysSET_Disp->strCHName;
	else
		return NULL;
}

/****************************************************************************************
 * 功能描述: 得到报告名称的译文
 * 参数说明: 
 *			- const TRE_Disp* pRE_Disp [in] 报告描述
 *			--WORD wInf [in] 报告的inf号
 * 返回代码: BOOL 是否成功返回
 * 其它: 如果希望用报告在数据库中的排序号返回译文，请查看函数TR_GetRENameByOrder
 *		 
 *****************************************************************************************/

const char* TR_GetREName(const TRE_Disp* pRE_Disp,WORD wInf)
{
	const char* str = NULL;
	str = g_DBTrans.GetReTr(wInf);
	if(str != NULL)
		return str;
	if(pRE_Disp)
		return pRE_Disp->strCHName;
	else
		return NULL;

}
/****************************************************************************************
 * 功能描述: 根据排序号得到报告名称的译文
 * 参数说明: 
 *			- const TRE_Disp* pRE_Disp [in] 报告描述
 *			--WORD wInf [in] 报告顺序序号
 * 返回代码: BOOL 是否成功返回
 * 其它: 如果希望用报告的inf号返回译文，请查看函数TR_GetREName
 *		 
 *****************************************************************************************/

const char* TR_GetRENameByOrder(const TRE_Disp* pRE_Disp,WORD wOrder)
{
	const char* str = NULL;
	str = g_DBTrans.GetReTrByOrder(wOrder);
	if(str != NULL)
		return str;
	if(pRE_Disp)
		return pRE_Disp->strCHName;
	else
		return NULL;

}


const char* TR_GetTCName(const TTC_Disp* pTC_Disp, WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetTCTr(wIndex);
	if(str != NULL)
		return str;
	if(pTC_Disp)
		return pTC_Disp->strCHName;
	else
		return NULL;
}
const char* TR_GetCFGName(const TCFG_Disp* pCFG_Disp,WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetCFGTr(wIndex);
	if(str != NULL)
		return str;
	if(pCFG_Disp)
		return pCFG_Disp->strCHName;
	else
		return NULL;
}

const char* TR_GetSWName(const TSW_Disp* pSW_Disp, WORD wIndex)
{
	const char* str = NULL;
	str = g_DBTrans.GetSWTr(wIndex);
	if(str != NULL)
		return str;
	if(pSW_Disp)
		return pSW_Disp->strCHName;
	else
		return NULL;
}

/****************************************************************************************
 * 功能描述: 得到定值取值表对应的名称列表翻译 pName必须分配足够的空间
 * 参数说明: 
 *			- const TSET_Disp* pSET_Disp [in] 定值描述
 			- WORD wIndex [in] 定值序号
 *			- char* pName [out] 定值取值表名称列表, 每个名称HMI_DBSTRING_SIZE字节
 *			--WORD& wSetNameCount [out] 定值取值个数
 * 返回代码: BOOL 是否成功返回
 * 其它: 对于定值取值内容代表某个选项，并且数据库中取值按列表管理的定值有效 如供电模式 0代表AT 1代表BT ２代表直供
 *		 pName 返回 "AT的译文\0BT译文\0直供的译文\0"，wSetNameCount返回3, 每个名称HMI_DBSTRING_SIZE字节
 *****************************************************************************************/
BOOL TR_GetSETDataName(const TSET_Disp* pSET_Disp, WORD wIndex, char* pName, WORD& wSetNameCount)
{
	
	char strSetDataName[32][SETNAME_MAXLENGTH];
	
	if(!pSET_Disp)
		return FALSE;
	
	WORD wStart = pSET_Disp->nItemStart;
	WORD wCount = pSET_Disp->nItemNum;
	
	int rc = DB_GetSETDataName(wIndex, &strSetDataName[0][0],wSetNameCount);
	//如果数据库错误，不管是否翻译文件正常都返回错误
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	if(g_DBTrans.GetTxtList(wStart, wCount, pName))
	{
		return TRUE;
	}
	else
	{
		//因为数据库字符串宽度 为SETNAME_MAXLENGTH，而翻译文件字符串宽度为HMI_DBSTRING_SIZE
		//统一返回HMI_DBSTRING_SIZE的字符串数组 需要进行转换
		for(int i=0; i<wCount; i++)
		{
			memcpy(pName, strSetDataName[i],SETNAME_MAXLENGTH);
			pName += HMI_DBSTRING_SIZE;
		}
		return TRUE;
	}
		
	
}

/****************************************************************************************
 * 功能描述: 得到辅助定值取值表对应的名称列表翻译 pName必须分配足够的空间
 * 参数说明: 
 *			- const TSysSET_Disp* pSysSET_Disp [in] 辅助定值描述
 
 			- WORD wIndex [in] 辅助定值序号
 *			- char* pName [out] 辅助定值取值表名称列表 扛雒艸MI_DBSTRING_SIZE字节
 *			--WORD& wSetNameCount [out] 辅助定值取值个数
 * 返回代码: BOOL 是否成功返回
 * 其它: 对于定值取值内容代表某个选项，并且数据库中取值按列表管理的定值有效 如供电模式 0代表AT 1代表BT ２代表直供
 *		 pName 返回 "AT的译文\0BT译文\0直供的译文\0"，wSetNameCount返回3
 *****************************************************************************************/

BOOL TR_GetSysSETDataName(const TSysSET_Disp* pSysSET_Disp, WORD wIndex, char* pName, WORD& wSetNameCount)
{
	char strSetDataName[32][SETNAME_MAXLENGTH];
	
	if(!pSysSET_Disp)
		return FALSE;
	
	WORD wStart = pSysSET_Disp->nItemStart;
	WORD wCount = pSysSET_Disp->nItemNum;
	
	int rc = DB_GetSystemSETDataName(wIndex, &strSetDataName[0][0],wSetNameCount);
	//如果数据库错误，不管是否翻译文件正常都返回错误
	if(DB_ERR_OK != rc)
	{
		return FALSE;
	}
	if(g_DBTrans.GetTxtList(wStart, wCount, pName))
	{
		return TRUE;
	}
	else
	{
		//因为数据库字符串宽度 为SETNAME_MAXLENGTH，而翻译文件字符串宽度为HMI_DBSTRING_SIZE
		//统一返回HMI_DBSTRING_SIZE的字符串数组 需要进行转换
		for(int i=0; i<wCount; i++)
		{
			memcpy(pName, strSetDataName[i],SETNAME_MAXLENGTH);
			pName += HMI_DBSTRING_SIZE;
		}
		return TRUE;
	}
}

CTranslator::CTranslator()
{
	m_nHashSize = 1;
	m_pHashNode = NULL;
	m_LangString = NULL;
	m_nKeyCount = 0;
	m_nCurLangIndex = 1; //默认的是要翻译的
}

CTranslator::~CTranslator()
{

}

WORD CTranslator::CalCheckSum(BYTE *pBuf,DWORD nLen)
{
	WORD	wTmp = 0x55AA;
	DWORD	i;
	
	if(pBuf == NULL)
		return 0;
	
	for(i=0; i < nLen; i++)
	{
		wTmp += pBuf[i];
	}
	return ~wTmp;
}

/****************************************************************************************
 * 功能描述: 
 * 参数说明: 
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
LONG CTranslator::CheckCommonFileHead()
{
	if(m_pCommonFileHead == NULL)
		return 1;
	//计算文件头校验码是否正确
	WORD wCheck = CalCheckSum((unsigned char *)m_pCommonFileHead, sizeof(TCommonFileHeadTrans)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);
	
	if(dwCheck != m_pCommonFileHead->dwCheck)
		return 2;
	return 0;

}

/****************************************************************************************
 * 功能描述: 
 * 参数说明: 
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
LONG CTranslator::CheckFile()
{
	if(m_pCommonFileHead == NULL)
		return 1;
	if(m_pCommonFileHead->dwSize ==0)
		return 1;

	if(m_pHashHead == NULL)
		return 1;

	//计算文件头校验码是否正确
	WORD wCheck = CalCheckSum((unsigned char*)m_pHashHead, 
				m_pCommonFileHead->dwSize - 2-sizeof(TCommonFileHeadTrans));

	//@	//2011-3-10 9:32:14 pFileCheck 可能不按word对齐的,所以不直接用word*而是用字节拼合
	char* pFileCheck = ((char*)m_pCommonFileHead+m_pCommonFileHead->dwSize -2);
	WORD wFileCheck =  MAKEWORD(pFileCheck[0], pFileCheck[1]);

	if(wCheck != wFileCheck)
		return 3;
	return 0;
}


void CTranslator::SetCurLang(int nLangIndex)
{
	m_nCurLangIndex = nLangIndex;
}
/****************************************************************************************
 * 功能描述: 得到字符串对应的翻译字符串
 * 参数说明: 
 *			- char* pContext 上下文
 *			- char* str 要翻译的字符串
 * 返回代码: 译文
 * 其它: 
 *****************************************************************************************/
const char* CTranslator::GetTrString(const char* pContext, const char* str, const char* szComment, int& nResult)
{
	if ((m_nKeyCount ==0)||!m_bIsTransOk)
	{	
		nResult = -1;
		return str;
	}
	char strHash[512];
	if(szComment)
		sprintf(strHash, "%s%s%s", pContext, str, szComment);
	else
		sprintf(strHash, "%s%s", pContext, str);

	//计算哈希值
	DWORD dwKey = JSHash(strHash); 
	//找到该关键字在hash表中的文字
	int pos = GetHashTablePos(dwKey,strHash, str);

	//@	//2011-3-10 9:33:09 pos 可能=0的，原來判斷<=0有問題
	if (pos < 0)
	{
		nResult = -1;

		return str;
	}
	else
	{
		nResult = 0;

		 return  m_LangString + m_pHashNode[pos].nAddress;
	}

}

/****************************************************************************************
 * 功能描述: 得到字符串对应的翻译字符串
 * 参数说明: 
 *			- char* str 要翻译的字符串
 * 返回代码: 翻译好的字符串
 * 其它: 
 *****************************************************************************************/
/*const char* CTranslator::GetTrString(const char* str)
{
	if ((m_nKeyCount ==0) || (m_nCurLangIndex == 0))
	{
		return str;
	}

	//计算哈希值
	DWORD dwKey = JSHash(str); 
	//找到该关键字在hash表中的文字
	int pos = GetHashTablePos(dwKey,str, str);

//@	//2011-3-10 9:33:09 pos 可能=0的，原來判斷<=0有問題
	if (pos < 0)
	{
		return str;
	}
	else
		 return  m_LangString + m_pHashNode[pos].nAddress;

}*/
/****************************************************************************************
 * 功能描述: 得到关键字为dwKey的节点位置
 * 参数说明: 
 *			- DWORD dwKey hash关键字
 *			- char* strSrc 源字符串
 *			--char* strcmpStr 键值重复时用来区别字符串
 * 返回代码: 节点位置， 返回-1表示找不到对应节点
 * 其它: 
 *****************************************************************************************/
int CTranslator::GetHashTablePos(DWORD dwKey, const char* strSrc,const char* strcmpStr)
{
	int i= 1;
	//初次散列位置
	int nPos = dwKey%m_nHashSize;
	int nStart = nPos;
	char* strFileSrc = NULL;

	//2011-2-16 10:37:21 判读.naddress != -1
	//散列到的节点内容不是空的   翻译文件的节点内容在初始化时.naddress初始化为-1， nkeyvalue初始化为0
	//非空节点的.nAddress 不可能是-1， nkeyvalue也不可能是0
	while((m_pHashNode[nPos].nAddress !=(WORD)-1 )&&(m_pHashNode[nPos].nKeyValue !=0))
	{
		if (dwKey == m_pHashNode[nPos].nKeyValue)
		{
			//还需判断是否键值有重复的
			if(m_pHashNode[nPos].nSrcAddress == (WORD)-1)
				return nPos;
			else
			{
				//有重复键值的，需要判断源字符
				strFileSrc = (char*)(m_LangString + m_pHashNode[nPos].nSrcAddress);
				if(strcmp(strcmpStr, strFileSrc) == 0)
					return nPos;
			}
		}
		nPos = (dwKey+i)%m_nHashSize;
		if (nPos == nStart)
		{ 
			//转了一圈又回到原来位置， 说明没有匹配的关键字
			return -1;
		}
		i++;
	}
	return -1;
}

/****************************************************************************************
 * 功能描述: 字符串hash算法 JSHash
 * 参数说明: 
 *			- char * str 要计算的字符串
 * 返回代码:  哈希值
 * 其它:  
 *****************************************************************************************/
DWORD CTranslator::JSHash(const char * str)
{
	DWORD  hash  =   1315423911 ;
	
	while  ( * str)
	{
		hash  ^=  ((hash  <<   5 )  +  ( * str ++ )  +  (hash  >>   2 ));
	} 
	
	return  (hash  &   0x7FFFFFFF );
}
const char* CTranslator::GetLangCaption()
{
	if(m_pHashHead)
		return m_pHashHead->szLangCaption;
	else
		return NULL;
}

BOOL CTranslator::GetTransStatus()
{
	return m_bIsTransOk;
}

LONG CTranslator::UnLoad()
{
	m_bIsTransOk = FALSE;
	m_nKeyCount = 0;
	return 0;
}

/****************************************************************************************
 * 功能描述: 装载语言文件， 完成CTranslator的初始化功能，
 *			初始化表头指针m_pHashNode， 译文指针m_LangString，关键字个数m_nKeyCount， hash大小m_nHashSize
 * 参数说明: 
 *			- char* LanFileBuf 文件内容
 * 返回代码: 0 装载成功
 * 其它: 
 *****************************************************************************************/
LONG CTranslator::Load(const BYTE* LanFileBuf, DWORD dwLength)
{

	
	const  BYTE *pBuf = NULL;
	pBuf = LanFileBuf;
	m_bIsTransOk = FALSE;

	if(pBuf)
	{
		m_pCommonFileHead = (TCommonFileHeadTrans*)pBuf;
		m_pHashHead = (THashFileHead*)(m_pCommonFileHead+1);
//@		if(CheckCommonFileHead() == 0)
//@		{
//@			if (CheckFile() == 0)
//@				m_bIsTransOk = TRUE;
//@		}
		if(g_CheckFile(pBuf) == 0)
		{
			m_bIsTransOk = TRUE;
		}
	}

	if(m_bIsTransOk)
	{
		m_nHashSize = m_pHashHead->nHashCount;
		m_nKeyCount = m_pHashHead->nKeyCount;
		m_pHashNode = (THmiHashNode*)(m_pHashHead+1);
		m_LangString =  (char*)(m_pHashNode+m_nHashSize);  //译文字符串基地址
	}
	else
	{
		m_nHashSize = 0;
		m_nKeyCount = 0;
		m_bIsTransOk = FALSE;
		m_pHashNode = NULL;
		m_LangString =  NULL;  //译文字符串基地址
		return 1;
	}
	return 0;
}

LONG CTranslator::LoadFile(const BYTE* LanName)
{
	
//@	BYTE *pBuf = NULL;
	
//@#ifndef __PL_WIN_SIMULATION__
//@	
//@	STATUS rc = DAE_GetFileLength(LanName, &dwFileLen);
//@	if((rc == DB_ERR_OK) && (dwFileLen > 0))
//@	{
//@		pBuf = new BYTE[dwFileLen];
//@		rc = DAE_ReadFile(LanName, pBuf,dwFileLen);
//@		if(rc != DB_ERR_OK)
//@		{
//@			LogError("DAE_ReadFile",FILE_LINE,"The Lanstr Date File fileName=%%s Read Err!",LanName);
//@			delete []pBuf;
//@			return -1;
//@		}
//@	}
//@	else
//@		pBuf  = NULL;
//@	
//@#else  //在windows模拟运行
//@	dwFileLen = g_Hashfile.GetLength();
//@	if (dwFileLen!=0)
//@	{
//@		pBuf = new BYTE[dwFileLen];
//@		g_Hashfile.Read(pBuf, dwFileLen);
//@	}
//@	else
//@		return -1;
//@	
//@#endif
	return 0;
}

///////////////////////CDBTranslator//////////////////

CDBTranslator::CDBTranslator()
{
	m_bIsDBTransOk = FALSE;
	m_pSetting = NULL;
	m_pSysSetting = NULL;
	m_pReport= NULL;
	m_pCFG = NULL;
	m_pSoftSwitch = NULL;
	m_pTxtList = NULL;
	m_pTC = NULL;
}

long CDBTranslator::UnLoad()
{
	m_bIsDBTransOk = FALSE;
	return 0;
}
/****************************************************************************************
 * 功能描述: 装载数据库翻译文件，使用该函数之前需要把数据库翻译读取到pFilebuf
 * 参数说明: 
 *			- const BYTE* pFilebuf[in] 指向翻译文件内容缓存区
 *			- DWORD dwLen[in] 翻译文件的大小
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/

long CDBTranslator::Load(const BYTE* pFilebuf, DWORD dwLen)
{
	m_bIsDBTransOk = FALSE;
	if(g_CheckFile(pFilebuf) ==0)
	{
		m_bIsDBTransOk = TRUE;
	}
	
	if(m_bIsDBTransOk)
	{
		m_pDBHead = (TDBTransHead*)(pFilebuf + sizeof(TCommonFileHeadTrans));
		
		//需要检查定值等的个数是否相同， 相同的才是正确的
		
		//初始化各个字符串的指针
		m_pSetting = pFilebuf + sizeof(TCommonFileHeadTrans) + sizeof(TDBTransHead);
		m_pSysSetting = m_pSetting + HMI_DBSTRING_SIZE*m_pDBHead->wSETCount;
		m_pReport = m_pSysSetting + HMI_DBSTRING_SIZE*m_pDBHead->wSysSETCount;
		m_pCFG = m_pReport + HMI_DBSTRING_SIZE*m_pDBHead->wRepCount;
		m_pSoftSwitch = m_pCFG + HMI_DBSTRING_SIZE*m_pDBHead->wCFGCount;
		m_pTxtList = m_pSoftSwitch + HMI_DBSTRING_SIZE*m_pDBHead->wSWCount;
		m_pTC = m_pTxtList + HMI_DBSTRING_SIZE*m_pDBHead->wTxtCount;

		m_pRelayword = m_pTC + HMI_DBSTRING_SIZE*m_pDBHead->wTCCount; //继电器名称
		m_pRelayFunType = m_pRelayword + HMI_DBSTRING_SIZE*m_pDBHead->wRelayWordCount; //继电器类型
		m_pRelaywordType = m_pRelayFunType + HMI_DBSTRING_SIZE*m_pDBHead->wRelayFuncTypeCount;

		//rm 2011-11-8 14:15:32
		m_pRM = m_pRelaywordType + HMI_DBSTRING_SIZE*m_pDBHead->wRelayTypeCount;

		//2012-2-29 13:43:38 遥测遥信电度遥控
		m_pAI = m_pRM + HMI_DBSTRING_SIZE*m_pDBHead->wRMCount;
		m_pBI = m_pAI + HMI_DBSTRING_SIZE*m_pDBHead->wAICount;
		m_pCI = m_pBI + HMI_DBSTRING_SIZE*m_pDBHead->wBICount;
		m_pBO = m_pCI + HMI_DBSTRING_SIZE*m_pDBHead->wCICount;
		return 0;
	}
	return 1;
}
/****************************************************************************************
 * 功能描述: 读取定值名称的译文
 * 参数说明: 
 *			- WORD wIndex 定值序号
 * 返回代码: 返回定值名称译文字符串指针
 * 其它: 
 *****************************************************************************************/
const char* CDBTranslator::GetSetTr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
	if(wIndex<DB_GetSETNum())
	{
		return (char*)(m_pSetting+HMI_DBSTRING_SIZE*wIndex);
	}
	else 
		return NULL;

}
/****************************************************************************************
 * 功能描述: 读辅助定值名称译文
 * 参数说明: 
 *			- WORD wIndex 辅助定值序号
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/

const char* CDBTranslator::GetSysSetTr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
	if(wIndex<DB_GetSystemSETNum())
		return (char*)(m_pSysSetting+HMI_DBSTRING_SIZE*wIndex);
	else 
		return NULL;

}
/****************************************************************************************
 * 功能描述: 通过inf号读取报告名称译文
 * 参数说明: 
 *			- WORD wInf 报告的inf号
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
const char* CDBTranslator::GetReTr(WORD wInf)
{
	if((!m_bIsDBTransOk)||(m_pDBHead == NULL))
		return NULL;
	BOOL bFound = FALSE;
	int i;
	WORD wd;
	for(i=0; i<m_pDBHead->wRepCount; i++)
	{
		wd = *((WORD*)(m_pReport+ HMI_DBSTRING_SIZE*i + (HMI_DBSTRING_SIZE-2)));
		if(wd == wInf)
		{
			bFound = TRUE;
			break;
		}
	}
	if(bFound)
		return (char*)(m_pReport+HMI_DBSTRING_SIZE*i);
	else
		return NULL;

}
const char* CDBTranslator::GetTCTr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
	return (char*)(m_pTC+HMI_DBSTRING_SIZE*wIndex);
}
const char* CDBTranslator::GetSWTr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
	return (char*)(m_pSoftSwitch+HMI_DBSTRING_SIZE*wIndex);
}
const char* CDBTranslator::GetCFGTr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
	if(wIndex<DB_GetCFGNum())
		return (char*)(m_pCFG+HMI_DBSTRING_SIZE*wIndex);
	else
		return NULL;
}

const char* CDBTranslator::GetRMTr(WORD wIndex)
{

	if(!m_bIsDBTransOk)
		return NULL;
		if(wIndex<DB_GetRMNum())
	return (char*)(m_pRM+HMI_DBSTRING_SIZE*wIndex);
		else
	return NULL;

}

/****************************************************************************************
 * 功能描述: 读取（定值和辅助定值）字符串列表的内容
 * 参数说明: 
 *			- WORD[in] wStart 字符列表中的起始序号
 *			- WORD[in] wCount  字符串个数
 *			- char*[out] pNameList 保存返回的字符串列表
 * 返回代码:  成功返回TRUE，否则返回FALSE
 * 其它: 
 *****************************************************************************************/
BOOL CDBTranslator::GetTxtList(WORD wStart, WORD wCount, char* pNameList)
{
	if(!m_bIsDBTransOk)
		return FALSE;
	if((wStart+wCount) > m_pDBHead->wTxtCount)
		return FALSE;
	memcpy((void*)pNameList,m_pTxtList+wStart*HMI_DBSTRING_SIZE,wCount*HMI_DBSTRING_SIZE);
	return TRUE;
}
//2011-9-19 14:46:36 为逻辑方程用而增加的继电器的翻译信息
const char* CDBTranslator::GetReTrByOrder(WORD wIndex) //通过顺序号返回报告
{
	if(!m_bIsDBTransOk)
		return NULL;
	return (char*)(m_pReport+HMI_DBSTRING_SIZE*wIndex);

}
const char* CDBTranslator::GetRelaywordTr(WORD wRelayWordID) //继电器
{
	if(!m_bIsDBTransOk)
		return NULL;
	return (char*)(m_pRelayword+HMI_DBSTRING_SIZE*wRelayWordID);

}
const char* CDBTranslator::GetRWFuncTypeTrByorder(WORD wIndex) 
{
	if(!m_bIsDBTransOk)
		return NULL;
	return (char*)(m_pRelayFunType+HMI_DBSTRING_SIZE*wIndex);
}

//参数是函数类型
const char* CDBTranslator::GetRWFuncTypeTr(WORD wFunTypeID) //函数
{
	if(!m_bIsDBTransOk)
		return NULL;
	if((!m_bIsDBTransOk)||(m_pDBHead == NULL))
		return NULL;
	BOOL bFound = FALSE;
	int i;
	WORD wd;
	for(i=0; i<m_pDBHead->wRelayFuncTypeCount; i++)
	{
		wd = *((WORD*)(m_pRelayFunType+ HMI_DBSTRING_SIZE*i + (HMI_DBSTRING_SIZE-2)));
		if(wd == wFunTypeID)
		{
			bFound = TRUE;
			break;
		}
	}
	if(bFound)
		return (char*)(m_pRelayFunType+HMI_DBSTRING_SIZE*i);
	else
		return NULL;
}

//参数是类型ID,不是顺序号
const char* CDBTranslator::GetRWTypeTr(WORD wTypeId)
{
	if(!m_bIsDBTransOk)
		return NULL;
	if(!m_bIsDBTransOk)
		return NULL;
	if((!m_bIsDBTransOk)||(m_pDBHead == NULL))
		return NULL;
	BOOL bFound = FALSE;
	int i;
	WORD wd;
	for(i=0; i<m_pDBHead->wRelayTypeCount; i++)
	{
		wd = *((WORD*)(m_pRelaywordType+ HMI_DBSTRING_SIZE*i + (HMI_DBSTRING_SIZE-2)));
		if(wd == wTypeId)
		{
			bFound = TRUE;
			break;
		}
	}
	if(bFound)
		return (char*)(m_pRelaywordType+HMI_DBSTRING_SIZE*i);
	else
		return NULL;
}

//2012-2-29 13:40:22

const char* CDBTranslator::GetAITr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
		if(wIndex<DB_GetAINum())
	return (char*)(m_pAI+HMI_DBSTRING_SIZE*wIndex);
		else
	return NULL;

}

const char* CDBTranslator::GetBITr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
		if(wIndex<DB_GetBINum())
	return (char*)(m_pBI+HMI_DBSTRING_SIZE*wIndex);
		else
	return NULL;
}
const char* CDBTranslator::GetCItr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
		if(wIndex<DB_GetCINum())
	return (char*)(m_pCI+HMI_DBSTRING_SIZE*wIndex);
		else
	return NULL;

}
const char* CDBTranslator::GetBOTr(WORD wIndex)
{
	if(!m_bIsDBTransOk)
		return NULL;
		if(wIndex<DB_GetBONum())
	return (char*)(m_pBO+HMI_DBSTRING_SIZE*wIndex);
		else
	return NULL;
}

WORD CDBTranslator::GetSetCount()
{
	if(m_pDBHead)
		return m_pDBHead->wSETCount;
	else
		return 0;
}

WORD CDBTranslator::GetSysSetCount()
{
	if(m_pDBHead)
		return m_pDBHead->wSysSETCount;
	else
		return 0;
}

WORD CDBTranslator::GetReCount()
{
	if(m_pDBHead)
		return m_pDBHead->wRepCount;
	else 
		return 0;
}

WORD CDBTranslator::GetTCCount()
{
	if(m_pDBHead)
		return m_pDBHead->wTCCount;
	else
		return 0;
}

WORD CDBTranslator::GetSWCount()
{
	if(m_pDBHead)
		return m_pDBHead->wSWCount;
	else
		return 0;
}

WORD CDBTranslator::GetCFGCount()
{
	if(m_pDBHead)
		return m_pDBHead->wCFGCount;
	else
		return 0;
}

WORD CDBTranslator::GetRMCount()
{
	if(m_pDBHead)
		return m_pDBHead->wRMCount;
	else
		return 0;
}

WORD CDBTranslator::GetAICount()
{
	if(m_pDBHead)
		return m_pDBHead->wAICount;
	else
		return 0;
}

WORD CDBTranslator::GetBICount()
{
	if(m_pDBHead)
		return m_pDBHead->wBICount;
	else
		return 0;
}

WORD CDBTranslator::GetCICount()
{
	if(m_pDBHead)
		return m_pDBHead->wCICount;
	else
		return 0;
}

WORD CDBTranslator::GetBOCount()
{
	if(m_pDBHead)
		return m_pDBHead->wBOCount;
	else
		return 0;
}

WORD CDBTranslator::GetTxtCount()
{
	if(m_pDBHead)
		return m_pDBHead->wTxtCount;
	else
		return 0;

}

/****************************************************************************************
 * 功能描述: 读取（定值和辅助定值）字符串列表的内容，pNameList使指向翻译字符串
 * 参数说明: 
 *			- WORD[in] wStart 字符列表中的起始序号
 *			- const char*[out] pNameList 保存返回的字符串列表
 * 返回代码:  成功返回TRUE，否则返回FALSE
 * 其它: 
 *****************************************************************************************/
BOOL CDBTranslator::GetTxtList(WORD wStart, const char* &pNameList)
{

	if((wStart) >= m_pDBHead->wTxtCount)
		return FALSE;
	pNameList =  (char*)m_pTxtList+wStart*HMI_DBSTRING_SIZE;
	return TRUE;

}

/////////////////CLanFileMan//////////

CLanFileMan::CLanFileMan()
{
	m_nLanCount =0;
	m_nLanCurSel = 0;
	AddLanInf("中文", "");
}
void CLanFileMan::AddLanInf(char* strCap, char* strLanName)
{
	strcpy(m_tLanInf[m_nLanCount].strCaption, strCap);
	strcpy(m_tLanInf[m_nLanCount].strLanName, strLanName);
	
	if(m_nLanCount<MAX_LANG_COUNT-1)
		m_nLanCount++;
}
const char* CLanFileMan::GetLanCaption(int nIndex)
{
	return m_tLanInf[nIndex].strCaption;
	
}
const char* CLanFileMan::GetLanName(int nIndex)
{
	return m_tLanInf[nIndex].strLanName;
}



/****************************************************************************************
 * 功能描述: 初始化多语言，1）读取系统中所有可用语言文件信息 2）装载当前选中的语言
 * 参数说明: 
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
LONG CLanFileMan::Init()
{
	//1）读取系统中所有可用语言文件信息
	LoadLangsInf();

	//2)
	WORD wLanIndexnSel = DB_GetLanguageNo();
//	WORD wLanIndexnSel = 1;

	return LoadLan(wLanIndexnSel);
}

LONG CLanFileMan::LoadLan(WORD wLanIndexnSel)
{
	//新加载翻译文件，如果出错的话，需要处理的
	bool bNeedDefault = FALSE;

	
	if(wLanIndexnSel == m_nLanCurSel)
		return 0;
	if(m_nLanCount == 1) //只有代码内的中文一种语言
		return 1;

	if(wLanIndexnSel >= m_nLanCount)
		m_nLanCurSel = 0;
	else
		m_nLanCurSel =  wLanIndexnSel;
	if(m_nLanCurSel == 0)
	{
		g_Trans.UnLoad();
		g_DBTrans.UnLoad();
		DB_SetLanguageNo(0);
		return 0;
	}

#ifndef __PL_WIN_SIMULATION__
	DWORD dwFileLen;
	char strFileName[256];

	//源码语言文件名
	sprintf(strFileName, "%d.bhs", m_nLanCurSel);
		
	STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc == DB_ERR_OK) && (dwFileLen > 0))
	{
		//读语言文件的语言标题
		rc = DAE_ReadFile(strFileName, m_pLangbuf,dwFileLen);
		if(rc != DB_ERR_OK)
		{
			//翻译文件错误的话，需要卸载掉翻译文件，并且选择默认的语音
			LogError("DAE_ReadFile",FILE_LINE,"Lang File fileName=%%s Read Err!",strFileName);
			bNeedDefault = TRUE;
		}
		else
		{
			if(g_Trans.Load(m_pLangbuf, dwFileLen) == 0)
			{
				DB_SetLanguageNo(m_nLanCurSel);
			}
			else
				bNeedDefault = TRUE;
			
		}
	}

	//如果界面翻译文件加载失败了，不尝试加载数据库语音文件
	if(bNeedDefault)
	{
		g_Trans.UnLoad();
		g_DBTrans.UnLoad();
		DB_SetLanguageNo(0);
		return 0;
	}

	//数据库语言文件
	sprintf(strFileName, "%d.dbbhs", m_nLanCurSel);
	
	rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc == DB_ERR_OK) && (dwFileLen > 0))
	{
		//读语言文件的语言标题
		rc = DAE_ReadFile(strFileName, m_pDBLangbuf,dwFileLen);
		if(rc != DB_ERR_OK)
		{
			LogError("DAE_ReadFile",FILE_LINE,"Lang File fileName=%%s Read Err!",strFileName);
			bNeedDefault = TRUE;
		}
		else
		{
			if(g_DBTrans.Load(m_pDBLangbuf, dwFileLen) != 0)
				bNeedDefault = TRUE;
				
		}
	}
	if(bNeedDefault)
	{
		g_DBTrans.UnLoad();
		return 0;
	}

	
	
#else
	CString strFileName, strPathName;
	CFile lanFile;
	//源码语言文件名
//	sprintf(strFileName, "%s", GetLanName(m_nLanCurSel));
	strFileName = GetLanName(m_nLanCurSel);

	BOOL res = lanFile.Open(strFileName,CFile::modeRead|CFile::typeBinary);
	DWORD dwFileLen = lanFile.GetLength();
	lanFile.Read(m_pLangbuf,dwFileLen);
	lanFile.Close();
	g_Trans.Load(m_pLangbuf, dwFileLen);

	//数据库翻译文件
//	strFileName = GetLanName(m_nLanCurSel)+"DB";
	strFileName = strFileName +"DB";
	
	res = lanFile.Open(strFileName,CFile::modeRead|CFile::typeBinary);
	dwFileLen = lanFile.GetLength();
	lanFile.Read(m_pDBLangbuf,dwFileLen);
	lanFile.Close();
	g_DBTrans.Load(m_pDBLangbuf, dwFileLen);
		
#endif
	return 0;
}



long CLanFileMan::LoadLangsInf()
{
#ifndef __PL_WIN_SIMULATION__
	BOOL bIsFound = TRUE;
	DWORD dwFileLen;
	char strFileName[256];
	char pszBuf[MULTILANG_PREVIEW_SIZE];
	char pszFileTitle[TRANS_TITLE_SIZE];
	int i=1;
	//语言文件名为1.bhs, 2.bhs....n.bhs，连续的。遇到一个没有的，就不再往后继续查找了
	while(bIsFound)
	{
		sprintf(strFileName, "%d.bhs", i);
		
		STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
		if((rc == DB_ERR_OK) && (dwFileLen > 0))
		{
			//读语言文件的语言标题
			rc = DAE_ReadFile(strFileName, (BYTE*)pszBuf,MULTILANG_PREVIEW_SIZE);
			if(rc != DB_ERR_OK)
			{
				LogError("DAE_ReadFile",FILE_LINE,"Lang File fileName=%%s Read Err!",strFileName);
				bIsFound = FALSE;
			}
			else
			{
				bIsFound = TRUE;
				TR_PerviewLangTitle(pszBuf, pszFileTitle);
				AddLanInf(pszFileTitle, strFileName);

			}
		}
		else
		{
			bIsFound = FALSE;
		}
		i++; 
	}


#else
	CFileFind fileFind;
	CString strLanhs, strPathName;
	CFile lanFile;
	char pszBuf[MULTILANG_PREVIEW_SIZE];
	char pszFileTitle[TRANS_TITLE_SIZE];
	
	char filepath[MAX_PATH];
	char path[MAX_PATH];
	GetModuleFileName(NULL,filepath,MAX_PATH);//得到当前运行程序的全路径 以后要用
	int i=strlen(filepath);
	while((filepath[i-1]!='\\') &&i>0)
	{
		i--;
	}
	filepath[i] = '\0';
	CString strpath = filepath;
	strLanhs = strpath + "*.hs";
	BOOL Res;

	if(fileFind.FindFile(strLanhs))
	{
		while(fileFind.FindNextFile())
		{
			strPathName = fileFind.GetFilePath();
			lanFile.Open(strPathName,CFile::modeRead|CFile::typeBinary);
			lanFile.Read(pszBuf,MULTILANG_PREVIEW_SIZE);
			TR_PerviewLangTitle(pszBuf,pszFileTitle);
			lanFile.Close();
			
			//得到文件描述
			AddLanInf(pszFileTitle,strPathName.GetBuffer(1));
		}
		
		strPathName = fileFind.GetFilePath();
		Res = lanFile.Open(strPathName,CFile::modeRead|CFile::typeBinary);
		lanFile.Read(pszBuf,MULTILANG_PREVIEW_SIZE);
		TR_PerviewLangTitle(pszBuf,pszFileTitle);
		lanFile.Close();
		
		//得到文件描述
		AddLanInf(pszFileTitle,strPathName.GetBuffer(1));
	}
#endif
	return 0;
}
