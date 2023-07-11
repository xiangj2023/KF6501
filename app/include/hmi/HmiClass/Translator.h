/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Translator.h
* 
*   ���ģ�飺Һ�� �����Է���
*
* 
*	���� 
* 
* 
* 
*   ���� 	LONG Load(char* LanName) װ�������ļ��� LanName�������ļ���
*			char* GetTrString(char* str) �õ��ַ���str�ķ����ַ���
*			char* GetTrString(char* pContext, char* str, char* comment) 	�õ�������ΪpContext���ַ���str�ķ����ַ���		
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2009-11-18		��ʼ����
* 
*************************************************************************/
#ifndef __Translator_H
#define __Translator_H

//��������
#define TR_LAN_CH 0 //����
#define TR_LAN_TR 1 //��������

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



#define  MAX_LANGNUM  16 //ͬʱ���ص�������ļ���
#define  TRANS_TITLE_SIZE 14 
typedef struct  THashFileHead
{
	WORD nKeyCount; //�ؼ��ָ���
	WORD nHashCount; //hash�����С
	WORD nType; //ʹ�õ�hash�㷨
	char szLangCaption[TRANS_TITLE_SIZE]; //���ı��� 2010-8-31����Ϊ14�ֽڣ�ʹ�ṹ��4�ֽڶ���
}THashFileHead;

typedef struct THmiHashNode
{
	DWORD nKeyValue; //hash�ؼ���
	WORD nAddress; //Ŀ���ַ�����ַ
	WORD nSrcAddress; //Դ�ַ�����ַ������ؼ����ظ�����Ҫ����Դ�ַ���
}TmiHashNode;

//���������ṹ��ͨ���ļ�ͷ��
struct TSysTimeTrans{
    WORD        Year;                   /*��*/
    BYTE        Month;                  /*��*/
    BYTE        Day;                    /*��*/
    BYTE        Hour;                   /*ʱ*/
    BYTE        Minute;                 /*��*/
    BYTE        Second;                 /*��*/
    BYTE 		Week;					/*����*/
	WORD		MSecond;				/*����*/
};

typedef struct _TCommonFileHeadTrans
{
	DWORD dwSize; //�ļ��ܳ���
	WORD wVersion; //�������ݿ�汾��
	WORD wFlag; //Ԥ��
	DWORD dwMagic; //Ԥ��
	TSysTimeTrans sysTime; //����ʱ��
	DWORD dwCheck; //У����
}TCommonFileHeadTrans;

class CTranslator
{
	TCommonFileHeadTrans *m_pCommonFileHead; //ͨ���ļ�ͷ

	THashFileHead *m_pHashHead; //ָ��hash��ͷ

	THmiHashNode *m_pHashNode; //ָ��hash���׽ڵ�
	char* m_LangString; //ָ�������ַ������ֵĿ�ʼ�� �����ַ�������ַ

	WORD* m_pFileCheck; //ָ��У����

	int m_nKeyCount; //�ؼ��ָ���
	int m_nHashSize; //hash���С
	DWORD  JSHash(const  char   * str); //�����ַ���str�Ĺ�ϣֵ
	int GetHashTablePos(DWORD dwKey, const char* strSrc,const char* strcmpStr); //�õ���ϣֵΪdwKey��λ��
	int m_nCurLangIndex; //��ǰʹ�õ��������
	
	char m_szLanCaption[16];
	BYTE* m_pBufHead; //������ͷָ��
	BOOL m_bIsTransOk; //�����ļ��Ƿ�����
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
//2012-2-27 14:12:14 �ռ䲻��������20���ֽڸ����ݿⷭ��ͷ
typedef struct TDBTransHead
{
	WORD wSETCount; //��ֵ
	WORD wSysSETCount; //������ֵ
	WORD wRepCount; //����
	WORD wCFGCount; //����
	WORD wSWCount; //ѹ��
	
	WORD wRelayWordCount;
	WORD wRelayFuncTypeCount;
	WORD wRelayTypeCount; //�̵������͸���
	WORD wTxtCount; //�����ı��б�
	WORD wTCCount; //����
	WORD wRMCount; //��������ֵ��

	WORD wAICount; //ң��
	WORD wBICount; //ң��
	WORD wCICount; //���
	WORD wBOCount; //ң��

	WORD wRsv[17]; //2011-9-19 9:22:45 Ԥ���Ժ�������������ķ�����

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

	const char* GetReTr(WORD wInf); //ͨ��inf�Ŷ�����˳���
	WORD GetReCount();

	const char* GetTCTr(WORD wIndex);
	WORD GetTCCount();

	const char* GetSWTr(WORD wIndex);
	WORD GetSWCount();

	const char* GetCFGTr(WORD wIndex);
	WORD GetCFGCount();
	//���������ַ��б�
	BOOL GetTxtList(WORD wStart, WORD wCount, char* pNameList);
	BOOL GetTxtList(WORD wStart,const char* &pNameList);
	WORD GetTxtCount();
	
	//2011-9-19 14:39:21
	const char* GetReTrByOrder(WORD wIndex); //ͨ��˳��ŷ��ر���
	const char* GetRelaywordTr(WORD wIndex); //�̵���
	const char* GetRWFuncTypeTr(WORD wIndex); //����
	const char* GetRWTypeTr(WORD wIndex);
	const char* GetRWFuncTypeTrByorder(WORD wIndex);
	
	//2011-11-8 14:18:47
	const char* GetRMTr(WORD wIndex);
	WORD GetRMCount();

	//2012-2-29 10:49:03
	const char* GetAITr(WORD wIndex);
	WORD GetAICount();
	const char* GetBITr(WORD wIndex); //ң��
	WORD GetBICount();
	const char* GetCItr(WORD wIndex); //���
	WORD GetCICount();
	const char* GetBOTr(WORD wIndex); //ң��
	WORD GetBOCount();
	
private:
	TDBTransHead* m_pDBHead;
	
	const BYTE* m_pSetting;
	const BYTE* m_pSysSetting;
	const BYTE* m_pReport;
	const BYTE* m_pCFG;
	const BYTE* m_pSoftSwitch;
	const BYTE* m_pTxtList;
	const BYTE* m_pTC; //����
	
	//2011-9-19 14:22:15 
	const BYTE* m_pRelayword; //�̵�������
	const BYTE* m_pRelaywordType; //�̵�������
	const BYTE* m_pRelayFunType; //�߼����̺���	

	//2011-11-8 14:19:54
	const BYTE* m_pRM;
	
	//2012-2-27 14:18:29 
	const BYTE* m_pAI; //ң��
	const BYTE* m_pBI; //ң��
	const BYTE* m_pCI; //���
	const BYTE* m_pBO; //ң��

};



/////////////////////CLanFileMan////////////////
#define  MAX_LANG_COUNT  16 //ͬʱ���ص�������ļ���
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
	//����ѡ�еķ����ļ��Ļ��������������ķ����ļ��Ĵ�С����
	//�����ڶ���ĺ� LAN_BUF_SIZE
	BYTE m_pLangbuf[LAN_BUF_SIZE];
	BYTE m_pDBLangbuf[LAN_DBBUF_SIZE];
	
public:	
	CLanFileMan();
	LONG Init(); //����װ���˼��������ļ�
	LONG LoadLan(WORD wLanIndex);
	//װ�����еĿ��÷����ļ���Ϣ
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


//������Ҫ������ַ�������ʱʹ�� �� scope�Ƿ���
#define QT_TRANSLATE_NOOP(scope,x)    x

//������Ҫ������ַ�������ʱʹ�ã���������Ա������Χ��ʹ�ã�����������
//���������Ĭ�Ϸ���"default"
#define QT_TR_NOOP(x) x

#define MULTILANG_PREVIEW_SIZE  (sizeof(THashFileHead)+sizeof(TCommonFileHeadTrans))

WORD g_CalCheckSum(const BYTE *pBuf,DWORD nLen);
LONG g_CheckFile(const BYTE* pFileBuf);

/****************************************************************************************
 * ��������: Ԥ����������
 * ����˵��: 
 *			-[in] const char* pbuf  ��������ͷָ��
 *			-[out] const char* pszTitle ���ص����������ַ���,����14���ֽڵĿռ�
 * ���ش���: 0 �ɹ� ����ʧ��
 * ����: Ҫ�õ�ĳ�������ļ������ƣ����ȴ������ļ���ȡMULTILANG_PREVIEW_SIZE��С������
 *		��pbuf�� ֮�����TR_PerviewLangTitle�� ���ص��������Ʊ�����pszTitle��
 *****************************************************************************************/
LONG TR_PerviewLangTitle(const char* pbuf,  char* pszTitle);


/****************************************************************************************
 * ��������: �õ���ǰװ�ص����ĵı���
 * ����˵��: -[out] const char* pszTitle ���ص����������ַ���,����14���ֽڵĿռ�
 * ���ش���: 0 �ɹ� ����ʧ��
 * ����: ���ù�TR_Load�Ժ�ſ��Ե��øú�������װ�ص����ĵı���
 *****************************************************************************************/
LONG TR_GetCurLangTitle( char* pszTitle);

/****************************************************************************************
 * ��������: �õ���������ķ����ַ���
 * ����˵��: 
 *			- const char* pzSrc Դ�ַ���
 *			- const char* pzContext Դ�ַ���������
 * ���ش���: ����ֵ�������ַ���
 * ����: 
 *****************************************************************************************/
const char* Outtr(const char* pzSrc, const char* pzContext);

/****************************************************************************************
 * ��������: װ���������� 
 * ע�⣺�ڵ��øú���֮ǰ��Ҫ �ȵ���ϵͳ�Ķ��ļ��������������ݴӷ����ļ���ȡ��pbuf�У��ļ����ȱ��浽dwFileLength
 * ����˵��: pbuf��dwFileLength������������� ���ú���ǰҪ��ȷ��ʼ��
 *			- const char* pbuf ����ָ�򱣴��˷������ݵ�ָ��
 *			- DWORD dwFileLength �����������ݵĳ���
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
LONG TR_Load(const BYTE* pbuf, DWORD dwFileLength);

//�����������IMPLEMENT_HMI_TR  DECLARE_HMI_TR  �����������Ա����tr(const char* pzSrc)
//���д�����Ҫ������ַ���������ʹ����������
//���Ե��� tr(const char* pzSrc) ������tr(const char* pzSrc, const char* pzContext)�������ٹ������ͳ������
/****************************************************************************************
 * ��������: ���Ա����tr(const char* pzSrc)���� 
 * ����˵��: 
 *			- const char* pzSrc Դ�ַ���
 *			- const char* pzContext Դ�ַ���������
 * ���ش���: ����ֵ�������ַ���
 * ����:  ClassName ������  IMPLEMENT_HMI_TR(ClassName)  �������.cpp�ļ���ʹ��
 *		  �������������Ա����tr(const char* pzSrc)��ʵ�ִ��룬Ҫ��DECLARE_HMI_TR���ʹ��
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
//Ӧ����IMPLEMENT_HMI_TR��ƴд���� Ϊ��ԭ���ĳ����������У� �Ա����ö��� 
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
 * ��������: ���Ա����tr(const char* pzSrc)����
 * ����˵��: 
 *			- const char* pzSrc Դ�ַ���
 *			- const char* pzComment ע�ͣ� �����ͬһ��������Դ�ַ�����ͬ�������ַ���ϣ���в�ͬ�����ģ�
										   ��ͨ����ͬ��pzComment�����֣��������øò���)
 *			
 * ���ش���: ����ֵ�������ַ���
 * ����: DECLARE_HMI_TR() �������.h�ļ����ඨ���е��ã�
 *   �����Ǹ����ඨ��һ�����Ա���� tr(const char* pzSrc), ��Ҫ��IMPLEMENT_HMI_TR���ʹ��
 *****************************************************************************************/
#define DECLARE_HMI_TR() \
	static const char* tr(const char* pzSrc, const char* pzComment=NULL);\



#endif /* __Translator_H */
