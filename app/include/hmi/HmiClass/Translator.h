/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			Translator.h
* 
*   软件模块：液晶 多语言翻译
*
* 
*	描述 
* 
* 
* 
*   函数 	LONG Load(char* LanName) 装载语言文件， LanName是语言文件名
*			char* GetTrString(char* str) 得到字符串str的翻译字符串
*			char* GetTrString(char* pContext, char* str, char* comment) 	得到上下文为pContext的字符串str的翻译字符串		
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2009-11-18		初始创建
* 
*************************************************************************/
#ifndef __Translator_H
#define __Translator_H

//定义语言
#define TR_LAN_CH 0 //中文
#define TR_LAN_TR 1 //翻译语言

#include "daefunc.h"
#include "Dbfunc.h"
#include "parafunc.h"
#include "db_err.h"

#ifdef ____PL_WIN_SIMULATION__
#include <windows.h>
#include <string.h>
#include "StdAfx.h"
#endif


//@const char* TR_GetSETName(const TSET_Disp* tSET_Disp, WORD wIndex = (WORD)-1);
//@const char* TR_GetSysSETName(const TSysSET_Disp* tSysSET_Disp,WORD wIndex = (WORD)-1);
//@const char* TR_GetREName(const TRE_Disp* tRE_Disp,WORD wIndex = (WORD)-1);
//@const char* TR_GetTCName(const TTC_Disp* tTC_Disp, WORD wIndex = (WORD)-1);
//@const char* TR_GetCFGName(const TCFG_Disp* tCFG_Disp,WORD wIndex = (WORD)-1);
//@const char* TR_GetSWName(const TSW_Disp* tSW_Disp, WORD wIndex = (WORD)-1);

const char* TR_GetSETName(const TSET_Disp* tSET_Disp, WORD wIndex );
const char* TR_GetSysSETName(const TSysSET_Disp* tSysSET_Disp,WORD wIndex);
const char* TR_GetREName(const TRE_Disp* tRE_Disp,WORD wInf );
const char* TR_GetTCName(const TTC_Disp* tTC_Disp, WORD wIndex );
const char* TR_GetCFGName(const TCFG_Disp* tCFG_Disp,WORD wIndex );
const char* TR_GetSWName(const TSW_Disp* tSW_Disp, WORD wIndex );
const char* TR_GetBIName(const TBIPara* pBIPara, WORD wIndex);
const char* TR_GetRMName(const TRM_Disp* pRM_Disp, WORD wIndex);
const char* TR_GetSETName(const TSET_Disp* pSET_Disp, WORD wIndex);

BOOL TR_GetSETDataName(const TSET_Disp* pSET_Disp,WORD wIndex, char* pName, WORD& wSetNameCount);
BOOL TR_GetSysSETDataName(const TSysSET_Disp* pSysSET_Disp, WORD wIndex, char* pName, WORD& wSetNameCount);



#define  MAX_LANGNUM  16 //同时加载的最大翻译文件数
#define  TRANS_TITLE_SIZE 14 
typedef struct  THashFileHead
{
	WORD nKeyCount; //关键字个数
	WORD nHashCount; //hash数组大小
	WORD nType; //使用的hash算法
	char szLangCaption[TRANS_TITLE_SIZE]; //译文标题 2010-8-31，该为14字节，使结构按4字节对齐
}THashFileHead;

typedef struct THmiHashNode
{
	DWORD nKeyValue; //hash关键字
	WORD nAddress; //目标字符串地址
	WORD nSrcAddress; //源字符串地址，如果关键字重复就需要保存源字符串
}TmiHashNode;

//下面两个结构是通用文件头用
struct TSysTimeTrans{
    WORD        Year;                   /*年*/
    BYTE        Month;                  /*月*/
    BYTE        Day;                    /*日*/
    BYTE        Hour;                   /*时*/
    BYTE        Minute;                 /*分*/
    BYTE        Second;                 /*秒*/
    BYTE 		Week;					/*星期*/
	WORD		MSecond;				/*毫秒*/
};

typedef struct _TCommonFileHeadTrans
{
	DWORD dwSize; //文件总长度
	WORD wVersion; //所用数据库版本号
	WORD wFlag; //预留
	DWORD dwMagic; //预留
	TSysTimeTrans sysTime; //生成时间
	DWORD dwCheck; //校验码
}TCommonFileHeadTrans;

class CTranslator
{
	TCommonFileHeadTrans *m_pCommonFileHead; //通用文件头

	THashFileHead *m_pHashHead; //指向hash表头

	THmiHashNode *m_pHashNode; //指向hash表首节点
	char* m_LangString; //指向译文字符串部分的开始， 译文字符串基地址

	WORD* m_pFileCheck; //指向校验码

	int m_nKeyCount; //关键字个数
	int m_nHashSize; //hash表大小
	DWORD  JSHash(const  char   * str); //计算字符串str的哈希值
	int GetHashTablePos(DWORD dwKey, const char* strSrc,const char* strcmpStr); //得到哈希值为dwKey的位置
	int m_nCurLangIndex; //当前使用的语言序号
	
	char m_szLanCaption[16];
	BYTE* m_pBufHead; //缓冲区头指针
	BOOL m_bIsTransOk; //翻译文件是否正常
	WORD CalCheckSum(BYTE *pBuf,DWORD nLen);
	LONG CheckCommonFileHead();
	LONG CheckFile();

public:
//	const char* GetTrString(const char* str);
	const char* GetTrString(const char* pContext, const char* str, const char* szComment, int& nResult);

	LONG Load(const BYTE* pFileBuf, DWORD dwLength);
	LONG UnLoad();
	LONG LoadFile(const BYTE* LanName);
	void SetCurLang(int nLangIndex);
	const char* GetLangCaption();
	BOOL GetTransStatus();

	CTranslator();
	~CTranslator();
};
/**********************/

///////////////////CDBTranslator
#define  HMI_DBSTRING_SIZE 32
//2012-2-27 14:12:14 空间不够用增加20个字节给数据库翻译头
typedef struct TDBTransHead
{
	WORD wSETCount; //定值
	WORD wSysSETCount; //辅助定值
	WORD wRepCount; //报告
	WORD wCFGCount; //配置
	WORD wSWCount; //压板
	
	WORD wRelayWordCount;
	WORD wRelayFuncTypeCount;
	WORD wRelayTypeCount; //继电器类型个数
	WORD wTxtCount; //名称文本列表
	WORD wTCCount; //传动
	WORD wRMCount; //保护测量值的

	WORD wAICount; //遥测
	WORD wBICount; //遥信
	WORD wCICount; //电度
	WORD wBOCount; //遥控

	WORD wRsv[17]; //2011-9-19 9:22:45 预留以后可以增加其他的翻译项

}TDBTransHead;

class CDBTranslator
{
public:
	CDBTranslator();
	long Load(const BYTE* pFilebuf, DWORD dwLen);
	long UnLoad();
	
	BOOL m_bIsDBTransOk;
	
	const char* GetSetTr(WORD wIndex);
	WORD GetSetCount();

	const char* GetSysSetTr(WORD wIndex);
	WORD GetSysSetCount();

	const char* GetReTr(WORD wInf); //通过inf号而不是顺序号
	WORD GetReCount();

	const char* GetTCTr(WORD wIndex);
	WORD GetTCCount();

	const char* GetSWTr(WORD wIndex);
	WORD GetSWCount();

	const char* GetCFGTr(WORD wIndex);
	WORD GetCFGCount();
	//返回名称字符列表
	BOOL GetTxtList(WORD wStart, WORD wCount, char* pNameList);
	BOOL GetTxtList(WORD wStart,const char* &pNameList);
	WORD GetTxtCount();
	
	//2011-9-19 14:39:21
	const char* GetReTrByOrder(WORD wIndex); //通过顺序号返回报告
	const char* GetRelaywordTr(WORD wIndex); //继电器
	const char* GetRWFuncTypeTr(WORD wIndex); //函数
	const char* GetRWTypeTr(WORD wIndex);
	const char* GetRWFuncTypeTrByorder(WORD wIndex);
	
	//2011-11-8 14:18:47
	const char* GetRMTr(WORD wIndex);
	WORD GetRMCount();

	//2012-2-29 10:49:03
	const char* GetAITr(WORD wIndex);
	WORD GetAICount();
	const char* GetBITr(WORD wIndex); //遥信
	WORD GetBICount();
	const char* GetCItr(WORD wIndex); //电度
	WORD GetCICount();
	const char* GetBOTr(WORD wIndex); //遥控
	WORD GetBOCount();
	
private:
	TDBTransHead* m_pDBHead;
	
	const BYTE* m_pSetting;
	const BYTE* m_pSysSetting;
	const BYTE* m_pReport;
	const BYTE* m_pCFG;
	const BYTE* m_pSoftSwitch;
	const BYTE* m_pTxtList;
	const BYTE* m_pTC; //传动
	
	//2011-9-19 14:22:15 
	const BYTE* m_pRelayword; //继电器名称
	const BYTE* m_pRelaywordType; //继电器类型
	const BYTE* m_pRelayFunType; //逻辑方程函数	

	//2011-11-8 14:19:54
	const BYTE* m_pRM;
	
	//2012-2-27 14:18:29 
	const BYTE* m_pAI; //遥测
	const BYTE* m_pBI; //遥信
	const BYTE* m_pCI; //电度
	const BYTE* m_pBO; //遥控

};



/////////////////////CLanFileMan////////////////
#define  MAX_LANG_COUNT  16 //同时加载的最大翻译文件数
#define  LANG_OK  0
#define  LAN_BUF_SIZE 1024*256
#define  LAN_DBBUF_SIZE 1024*64

class CLanFileMan
{
		
	struct TLanInf
	{
		char strCaption[256];
		char strLanName[256];
	};
	int m_nLanCount;
	char m_strCurLanName[256];
	TLanInf m_tLanInf[MAX_LANG_COUNT];
	int m_nLanCurSel;
	//保存选中的翻译文件的缓冲区，按照最大的翻译文件的大小分配
	//不大于定义的宏 LAN_BUF_SIZE
	BYTE m_pLangbuf[LAN_BUF_SIZE];
	BYTE m_pDBLangbuf[LAN_DBBUF_SIZE];
	
public:	
	CLanFileMan();
	LONG Init(); //返回装载了几个语言文件
	LONG LoadLan(WORD wLanIndex);
	//装载所有的可用翻译文件信息
	long LoadLangsInf();
	const char* GetLanCaption(int nIndex);
	
	int GetCurLang(){return m_nLanCurSel;};
	int GetLanCount(){return m_nLanCount;};
protected:
	void AddLanInf(char* strCap, char* strLanName);
	void ClearAllInf(){m_nLanCount = 0;}
	const char* GetLanName(int nIndex);
	void SetCurLang(int nIndex){m_nLanCurSel =  nIndex;};
};

/////////////////ktrans/////////////////////
extern CTranslator g_Trans;
extern CDBTranslator g_DBTrans;
extern CLanFileMan g_LanFileMan;


//定义需要翻译的字符串数组时使用 ， scope是分组
#define QT_TRANSLATE_NOOP(scope,x)    x

//定义需要翻译的字符串数组时使用，如果在类成员函数范围内使用，分组是类名
//否则分组是默认分组"default"
#define QT_TR_NOOP(x) x

#define MULTILANG_PREVIEW_SIZE  (sizeof(THashFileHead)+sizeof(TCommonFileHeadTrans))

WORD g_CalCheckSum(const BYTE *pBuf,DWORD nLen);
LONG g_CheckFile(const BYTE* pFileBuf);

/****************************************************************************************
 * 功能描述: 预览译文名称
 * 参数说明: 
 *			-[in] const char* pbuf  传入译文头指针
 *			-[out] const char* pszTitle 返回的译文名称字符串,至少14个字节的空间
 * 返回代码: 0 成功 其他失败
 * 其它: 要得到某个译文文件的名称，首先从译文文件读取MULTILANG_PREVIEW_SIZE大小的数据
 *		到pbuf， 之后调用TR_PerviewLangTitle， 返回的译文名称保存在pszTitle中
 *****************************************************************************************/
LONG TR_PerviewLangTitle(const char* pbuf,  char* pszTitle);


/****************************************************************************************
 * 功能描述: 得到当前装载的译文的标题
 * 参数说明: -[out] const char* pszTitle 返回的译文名称字符串,至少14个字节的空间
 * 返回代码: 0 成功 其他失败
 * 其它: 调用过TR_Load以后才可以调用该函数返回装载的译文的标题
 *****************************************************************************************/
LONG TR_GetCurLangTitle( char* pszTitle);

/****************************************************************************************
 * 功能描述: 得到其他分组的翻译字符串
 * 参数说明: 
 *			- const char* pzSrc 源字符串
 *			- const char* pzContext 源字符串上下文
 * 返回代码: 返回值是译文字符串
 * 其它: 
 *****************************************************************************************/
const char* Outtr(const char* pzSrc, const char* pzContext);

/****************************************************************************************
 * 功能描述: 装载译文数据 
 * 注意：在调用该函数之前需要 先调用系统的读文件函数把译文数据从翻译文件读取到pbuf中，文件长度保存到dwFileLength
 * 参数说明: pbuf和dwFileLength都是输入参数， 调用函数前要正确初始化
 *			- const char* pbuf 传入指向保存了翻译数据的指针
 *			- DWORD dwFileLength 传入译文数据的长度
 * 返回代码: 
 * 其它: 
 *****************************************************************************************/
LONG TR_Load(const BYTE* pbuf, DWORD dwFileLength);

//下面的两个宏IMPLEMENT_HMI_TR  DECLARE_HMI_TR  给类增加类成员函数tr(const char* pzSrc)
//在有大量需要翻译的字符串的类里使用这两个宏
//可以调用 tr(const char* pzSrc) 而不是tr(const char* pzSrc, const char* pzContext)，来减少工作量和出错机会
/****************************************************************************************
 * 功能描述: 类成员函数tr(const char* pzSrc)定义 
 * 参数说明: 
 *			- const char* pzSrc 源字符串
 *			- const char* pzContext 源字符串上下文
 * 返回代码: 返回值是译文字符串
 * 其它:  ClassName 是类名  IMPLEMENT_HMI_TR(ClassName)  宏在类的.cpp文件中使用
 *		  作用是生成类成员函数tr(const char* pzSrc)的实现代码，要和DECLARE_HMI_TR配合使用
 *****************************************************************************************/
#define IMPLEMENT_HMI_TR(ClassName) \
    const char* ClassName::tr(const char* pszSrc,const char* pzComment)\
{	int nResult; \
	if (pszSrc[0] == '\0')\
	{\
		return pszSrc;\
	}\
	return g_Trans.GetTrString(#ClassName,pszSrc, pzComment, nResult);\
} \
//应该是IMPLEMENT_HMI_TR，拼写错误， 为了原来的程序正常运行， 仍保留该定义 
#define INPLEMENT_HMI_TR(ClassName) \
    const char* ClassName::tr(const char* pszSrc,const char* pzComment)\
{	int nResult; \
	if (pszSrc[0] == '\0')\
	{\
		return pszSrc;\
	}\
	return g_Trans.GetTrString(#ClassName,pszSrc, pzComment, nResult);\
} \

/****************************************************************************************
 * 功能描述: 类成员函数tr(const char* pzSrc)声明
 * 参数说明: 
 *			- const char* pzSrc 源字符串
 *			- const char* pzComment 注释， 如果在同一上下文内源字符串相同的两个字符串希望有不同的译文，
										   则通过不同的pzComment来区分，（否则不用该参数)
 *			
 * 返回代码: 返回值是译文字符串
 * 其它: DECLARE_HMI_TR() 宏在类的.h文件中类定义中调用，
 *   作用是给该类定义一个类成员函数 tr(const char* pzSrc), 需要和IMPLEMENT_HMI_TR配合使用
 *****************************************************************************************/
#define DECLARE_HMI_TR() \
	static const char* tr(const char* pzSrc, const char* pzComment=NULL);\



#endif /* __Translator_H */
