/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Translator.cpp
* 
*   ���ģ�飺Һ�� �����Է���
*			  		
* 
*	����	������6500��1300��pc��Ӧ�õ����幦�ܴ�������ͬ�ģ�
*			1300��pc�ϵ����ݿⷭ�벿��CDBtranslator����DBtranslator.cpp��������ʱ�Ը������ж��� ��ȫ��ͬ
				�ļ���������CLanFileMan���ַ���LangfileMan.cpp ��ʼ������ͬ
				���뺯�����ýӿ�TR_Load��TR_PerviewLang �ȷ���ktrans.cpp����ȫ��ͬ
*			6500�Ķ�ֱ�ӷ���Translator.cpp�ļ��Ľ�β 
* 			6500��������һ��const char* TR_GetSETName(const TSET_Disp* pSET_Disp, WORD wIndex)����
* 
* 
*   ���� 	LONG Load(char* LanName) װ�������ļ��� LanName�������ļ���
*			char* GetTrString(char* str) �õ��ַ���str�ķ����ַ���
*			char* GetTrString(char* pContext, char* str) 	�õ�������ΪpContext���ַ���str�ķ����ַ���		
*
*
*	CDBTranslator ����
*	
*   ����  
*	long Load(const BYTE* pFilebuf, DWORD dwLen) �������ݿⷭ��
*	long UnLoad() ж�����ݿ��ַ������룬ѡ�����ĵ�ʱ����ŵ��øú���
*	const char* GetSetTr(WORD wIndex); ��ȡ��ֵ����
*	const char* GetSysSetTr(WORD wIndex); ��ȡ������ֵ����
*	const char* GetReTr(WORD wInf); //ͨ��inf�Ŷ�ȡ������������
*	const char* GetTCTr(WORD wIndex); ��ȡ��������
*	const char* GetSWTr(WORD wIndex); ��ȡ��ѹ������
*	const char* GetCFGTr(WORD wIndex); ��ȡ����������
*	��ȡ�ַ����б�
*	BOOL GetTxtList(WORD wStart, WORD wCount, char* pNameList);
*
*	Ϊ�˷���ʹ�ã�����ʱ����Ҫ����CDBTranslator�ĳ�Ա������ֱ��ʹ�����½ӿں���
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

*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2009-11-18		��ʼ����
*		kangqian	2011-08-16		����CDBTranslator �� CLanFileMan�Ĵ���
*		kangqian	2012-10-23		CDBTranslator���Ӳ���ֵ���ƣ�ң�����Ƶȣ���1300����һ��
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
 * ��������: װ���������� 
 * ע�⣺�ڵ��øú���֮ǰ��Ҫ �ȵ���ϵͳ�Ķ��ļ��������������ݴӷ����ļ���ȡ��pbuf�У��ļ����ȱ��浽dwFileLength
 * ����˵��: pbuf��dwFileLength������������� ���ú���ǰҪ��ȷ��ʼ��
 *			- const char* pbuf ����ָ�򱣴��˷������ݵ�ָ��
 *			- DWORD dwFileLength �����������ݵĳ���
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
LONG TR_Load(const BYTE* pbuf, DWORD dwFileLength)
{
	if((pbuf == NULL)|| dwFileLength == 0)
	{
		//������ʾ
		return -1;
	}
	return g_Trans.Load(pbuf, dwFileLength);
}

/****************************************************************************************
 * ��������: Ԥ����������
 * ����˵��: 
 *			-[in] const char* pbuf  ��������ͷָ��
 *			-[out] const char* pszTitle ���ص����������ַ���,����14���ֽڵĿռ�
 * ���ش���: 0 �ɹ� ����ʧ��
 * ����: Ҫ�õ�ĳ�������ļ������ƣ����ȴ������ļ���ȡMULTILANG_PREVIEW_SIZE��С������
 *		��pbuf�� ֮�����TR_PerviewLangTitle�� ���ص��������Ʊ�����pszTitle��
 *****************************************************************************************/
LONG TR_PerviewLangTitle(const char* pbuf,  char* pszTitle)
{
	THashFileHead* pHashHead = (THashFileHead*)(pbuf+sizeof(TCommonFileHeadTrans));
	memcpy(pszTitle, pHashHead->szLangCaption,  TRANS_TITLE_SIZE);
	return 0;
}


/****************************************************************************************
 * ��������: �õ���ǰװ�ص����ĵı���
 * ����˵��: -[out] const char* pszTitle ���ص����������ַ���,����14���ֽڵĿռ�
 * ���ش���: 0 �ɹ� ����ʧ��
 * ����: ���ù�TR_Load�Ժ�ſ��Ե��øú�������װ�ص����ĵı���
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

	//1.����ļ�ͷУ�����Ƿ���ȷ
	//�����ļ�ͷУ�����Ƿ���ȷ
	WORD wCheck = g_CalCheckSum((unsigned char *)pCommonFileHead, sizeof(TCommonFileHeadTrans)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);
	
	if(dwCheck != pCommonFileHead->dwCheck)
		return 2;

	//2.����ļ�У����
	//�����ļ�У����
	wCheck = g_CalCheckSum((unsigned char*)(pCommonFileHead+1),
		pCommonFileHead->dwSize - 2-sizeof(TCommonFileHeadTrans));
	
	//@	//2011-3-10 9:32:14 pFileCheck ���ܲ���word�����,���Բ�ֱ����word*�������ֽ�ƴ��
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
 * ��������: �õ��������Ƶ�����
 * ����˵��: 
 *			- const TRE_Disp* pRE_Disp [in] ��������
 *			--WORD wInf [in] �����inf��
 * ���ش���: BOOL �Ƿ�ɹ�����
 * ����: ���ϣ���ñ��������ݿ��е�����ŷ������ģ���鿴����TR_GetRENameByOrder
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
 * ��������: ��������ŵõ��������Ƶ�����
 * ����˵��: 
 *			- const TRE_Disp* pRE_Disp [in] ��������
 *			--WORD wInf [in] ����˳�����
 * ���ش���: BOOL �Ƿ�ɹ�����
 * ����: ���ϣ���ñ����inf�ŷ������ģ���鿴����TR_GetREName
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
 * ��������: �õ���ֵȡֵ���Ӧ�������б��� pName��������㹻�Ŀռ�
 * ����˵��: 
 *			- const TSET_Disp* pSET_Disp [in] ��ֵ����
 			- WORD wIndex [in] ��ֵ���
 *			- char* pName [out] ��ֵȡֵ�������б�, ÿ������HMI_DBSTRING_SIZE�ֽ�
 *			--WORD& wSetNameCount [out] ��ֵȡֵ����
 * ���ش���: BOOL �Ƿ�ɹ�����
 * ����: ���ڶ�ֵȡֵ���ݴ���ĳ��ѡ��������ݿ���ȡֵ���б����Ķ�ֵ��Ч �繩��ģʽ 0����AT 1����BT ������ֱ��
 *		 pName ���� "AT������\0BT����\0ֱ��������\0"��wSetNameCount����3, ÿ������HMI_DBSTRING_SIZE�ֽ�
 *****************************************************************************************/
BOOL TR_GetSETDataName(const TSET_Disp* pSET_Disp, WORD wIndex, char* pName, WORD& wSetNameCount)
{
	
	char strSetDataName[32][SETNAME_MAXLENGTH];
	
	if(!pSET_Disp)
		return FALSE;
	
	WORD wStart = pSET_Disp->nItemStart;
	WORD wCount = pSET_Disp->nItemNum;
	
	int rc = DB_GetSETDataName(wIndex, &strSetDataName[0][0],wSetNameCount);
	//������ݿ���󣬲����Ƿ����ļ����������ش���
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
		//��Ϊ���ݿ��ַ������ ΪSETNAME_MAXLENGTH���������ļ��ַ������ΪHMI_DBSTRING_SIZE
		//ͳһ����HMI_DBSTRING_SIZE���ַ������� ��Ҫ����ת��
		for(int i=0; i<wCount; i++)
		{
			memcpy(pName, strSetDataName[i],SETNAME_MAXLENGTH);
			pName += HMI_DBSTRING_SIZE;
		}
		return TRUE;
	}
		
	
}

/****************************************************************************************
 * ��������: �õ�������ֵȡֵ���Ӧ�������б��� pName��������㹻�Ŀռ�
 * ����˵��: 
 *			- const TSysSET_Disp* pSysSET_Disp [in] ������ֵ����
 
 			- WORD wIndex [in] ������ֵ���
 *			- char* pName [out] ������ֵȡֵ�������б� �������HMI_DBSTRING_SIZE�ֽ�
 *			--WORD& wSetNameCount [out] ������ֵȡֵ����
 * ���ش���: BOOL �Ƿ�ɹ�����
 * ����: ���ڶ�ֵȡֵ���ݴ���ĳ��ѡ��������ݿ���ȡֵ���б����Ķ�ֵ��Ч �繩��ģʽ 0����AT 1����BT ������ֱ��
 *		 pName ���� "AT������\0BT����\0ֱ��������\0"��wSetNameCount����3
 *****************************************************************************************/

BOOL TR_GetSysSETDataName(const TSysSET_Disp* pSysSET_Disp, WORD wIndex, char* pName, WORD& wSetNameCount)
{
	char strSetDataName[32][SETNAME_MAXLENGTH];
	
	if(!pSysSET_Disp)
		return FALSE;
	
	WORD wStart = pSysSET_Disp->nItemStart;
	WORD wCount = pSysSET_Disp->nItemNum;
	
	int rc = DB_GetSystemSETDataName(wIndex, &strSetDataName[0][0],wSetNameCount);
	//������ݿ���󣬲����Ƿ����ļ����������ش���
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
		//��Ϊ���ݿ��ַ������ ΪSETNAME_MAXLENGTH���������ļ��ַ������ΪHMI_DBSTRING_SIZE
		//ͳһ����HMI_DBSTRING_SIZE���ַ������� ��Ҫ����ת��
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
	m_nCurLangIndex = 1; //Ĭ�ϵ���Ҫ�����
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
 * ��������: 
 * ����˵��: 
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
LONG CTranslator::CheckCommonFileHead()
{
	if(m_pCommonFileHead == NULL)
		return 1;
	//�����ļ�ͷУ�����Ƿ���ȷ
	WORD wCheck = CalCheckSum((unsigned char *)m_pCommonFileHead, sizeof(TCommonFileHeadTrans)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);
	
	if(dwCheck != m_pCommonFileHead->dwCheck)
		return 2;
	return 0;

}

/****************************************************************************************
 * ��������: 
 * ����˵��: 
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
LONG CTranslator::CheckFile()
{
	if(m_pCommonFileHead == NULL)
		return 1;
	if(m_pCommonFileHead->dwSize ==0)
		return 1;

	if(m_pHashHead == NULL)
		return 1;

	//�����ļ�ͷУ�����Ƿ���ȷ
	WORD wCheck = CalCheckSum((unsigned char*)m_pHashHead, 
				m_pCommonFileHead->dwSize - 2-sizeof(TCommonFileHeadTrans));

	//@	//2011-3-10 9:32:14 pFileCheck ���ܲ���word�����,���Բ�ֱ����word*�������ֽ�ƴ��
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
 * ��������: �õ��ַ�����Ӧ�ķ����ַ���
 * ����˵��: 
 *			- char* pContext ������
 *			- char* str Ҫ������ַ���
 * ���ش���: ����
 * ����: 
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

	//�����ϣֵ
	DWORD dwKey = JSHash(strHash); 
	//�ҵ��ùؼ�����hash���е�����
	int pos = GetHashTablePos(dwKey,strHash, str);

	//@	//2011-3-10 9:33:09 pos ����=0�ģ�ԭ���Д�<=0�І��}
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
 * ��������: �õ��ַ�����Ӧ�ķ����ַ���
 * ����˵��: 
 *			- char* str Ҫ������ַ���
 * ���ش���: ����õ��ַ���
 * ����: 
 *****************************************************************************************/
/*const char* CTranslator::GetTrString(const char* str)
{
	if ((m_nKeyCount ==0) || (m_nCurLangIndex == 0))
	{
		return str;
	}

	//�����ϣֵ
	DWORD dwKey = JSHash(str); 
	//�ҵ��ùؼ�����hash���е�����
	int pos = GetHashTablePos(dwKey,str, str);

//@	//2011-3-10 9:33:09 pos ����=0�ģ�ԭ���Д�<=0�І��}
	if (pos < 0)
	{
		return str;
	}
	else
		 return  m_LangString + m_pHashNode[pos].nAddress;

}*/
/****************************************************************************************
 * ��������: �õ��ؼ���ΪdwKey�Ľڵ�λ��
 * ����˵��: 
 *			- DWORD dwKey hash�ؼ���
 *			- char* strSrc Դ�ַ���
 *			--char* strcmpStr ��ֵ�ظ�ʱ���������ַ���
 * ���ش���: �ڵ�λ�ã� ����-1��ʾ�Ҳ�����Ӧ�ڵ�
 * ����: 
 *****************************************************************************************/
int CTranslator::GetHashTablePos(DWORD dwKey, const char* strSrc,const char* strcmpStr)
{
	int i= 1;
	//����ɢ��λ��
	int nPos = dwKey%m_nHashSize;
	int nStart = nPos;
	char* strFileSrc = NULL;

	//2011-2-16 10:37:21 �ж�.naddress != -1
	//ɢ�е��Ľڵ����ݲ��ǿյ�   �����ļ��Ľڵ������ڳ�ʼ��ʱ.naddress��ʼ��Ϊ-1�� nkeyvalue��ʼ��Ϊ0
	//�ǿսڵ��.nAddress ��������-1�� nkeyvalueҲ��������0
	while((m_pHashNode[nPos].nAddress !=(WORD)-1 )&&(m_pHashNode[nPos].nKeyValue !=0))
	{
		if (dwKey == m_pHashNode[nPos].nKeyValue)
		{
			//�����ж��Ƿ��ֵ���ظ���
			if(m_pHashNode[nPos].nSrcAddress == (WORD)-1)
				return nPos;
			else
			{
				//���ظ���ֵ�ģ���Ҫ�ж�Դ�ַ�
				strFileSrc = (char*)(m_LangString + m_pHashNode[nPos].nSrcAddress);
				if(strcmp(strcmpStr, strFileSrc) == 0)
					return nPos;
			}
		}
		nPos = (dwKey+i)%m_nHashSize;
		if (nPos == nStart)
		{ 
			//ת��һȦ�ֻص�ԭ��λ�ã� ˵��û��ƥ��Ĺؼ���
			return -1;
		}
		i++;
	}
	return -1;
}

/****************************************************************************************
 * ��������: �ַ���hash�㷨 JSHash
 * ����˵��: 
 *			- char * str Ҫ������ַ���
 * ���ش���:  ��ϣֵ
 * ����:  
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
 * ��������: װ�������ļ��� ���CTranslator�ĳ�ʼ�����ܣ�
 *			��ʼ����ͷָ��m_pHashNode�� ����ָ��m_LangString���ؼ��ָ���m_nKeyCount�� hash��Сm_nHashSize
 * ����˵��: 
 *			- char* LanFileBuf �ļ�����
 * ���ش���: 0 װ�سɹ�
 * ����: 
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
		m_LangString =  (char*)(m_pHashNode+m_nHashSize);  //�����ַ�������ַ
	}
	else
	{
		m_nHashSize = 0;
		m_nKeyCount = 0;
		m_bIsTransOk = FALSE;
		m_pHashNode = NULL;
		m_LangString =  NULL;  //�����ַ�������ַ
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
//@#else  //��windowsģ������
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
 * ��������: װ�����ݿⷭ���ļ���ʹ�øú���֮ǰ��Ҫ�����ݿⷭ���ȡ��pFilebuf
 * ����˵��: 
 *			- const BYTE* pFilebuf[in] ָ�����ļ����ݻ�����
 *			- DWORD dwLen[in] �����ļ��Ĵ�С
 * ���ش���: 
 * ����: 
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
		
		//��Ҫ��鶨ֵ�ȵĸ����Ƿ���ͬ�� ��ͬ�Ĳ�����ȷ��
		
		//��ʼ�������ַ�����ָ��
		m_pSetting = pFilebuf + sizeof(TCommonFileHeadTrans) + sizeof(TDBTransHead);
		m_pSysSetting = m_pSetting + HMI_DBSTRING_SIZE*m_pDBHead->wSETCount;
		m_pReport = m_pSysSetting + HMI_DBSTRING_SIZE*m_pDBHead->wSysSETCount;
		m_pCFG = m_pReport + HMI_DBSTRING_SIZE*m_pDBHead->wRepCount;
		m_pSoftSwitch = m_pCFG + HMI_DBSTRING_SIZE*m_pDBHead->wCFGCount;
		m_pTxtList = m_pSoftSwitch + HMI_DBSTRING_SIZE*m_pDBHead->wSWCount;
		m_pTC = m_pTxtList + HMI_DBSTRING_SIZE*m_pDBHead->wTxtCount;

		m_pRelayword = m_pTC + HMI_DBSTRING_SIZE*m_pDBHead->wTCCount; //�̵�������
		m_pRelayFunType = m_pRelayword + HMI_DBSTRING_SIZE*m_pDBHead->wRelayWordCount; //�̵�������
		m_pRelaywordType = m_pRelayFunType + HMI_DBSTRING_SIZE*m_pDBHead->wRelayFuncTypeCount;

		//rm 2011-11-8 14:15:32
		m_pRM = m_pRelaywordType + HMI_DBSTRING_SIZE*m_pDBHead->wRelayTypeCount;

		//2012-2-29 13:43:38 ң��ң�ŵ��ң��
		m_pAI = m_pRM + HMI_DBSTRING_SIZE*m_pDBHead->wRMCount;
		m_pBI = m_pAI + HMI_DBSTRING_SIZE*m_pDBHead->wAICount;
		m_pCI = m_pBI + HMI_DBSTRING_SIZE*m_pDBHead->wBICount;
		m_pBO = m_pCI + HMI_DBSTRING_SIZE*m_pDBHead->wCICount;
		return 0;
	}
	return 1;
}
/****************************************************************************************
 * ��������: ��ȡ��ֵ���Ƶ�����
 * ����˵��: 
 *			- WORD wIndex ��ֵ���
 * ���ش���: ���ض�ֵ���������ַ���ָ��
 * ����: 
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
 * ��������: ��������ֵ��������
 * ����˵��: 
 *			- WORD wIndex ������ֵ���
 * ���ش���: 
 * ����: 
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
 * ��������: ͨ��inf�Ŷ�ȡ������������
 * ����˵��: 
 *			- WORD wInf �����inf��
 * ���ش���: 
 * ����: 
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
 * ��������: ��ȡ����ֵ�͸�����ֵ���ַ����б������
 * ����˵��: 
 *			- WORD[in] wStart �ַ��б��е���ʼ���
 *			- WORD[in] wCount  �ַ�������
 *			- char*[out] pNameList ���淵�ص��ַ����б�
 * ���ش���:  �ɹ�����TRUE�����򷵻�FALSE
 * ����: 
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
//2011-9-19 14:46:36 Ϊ�߼������ö����ӵļ̵����ķ�����Ϣ
const char* CDBTranslator::GetReTrByOrder(WORD wIndex) //ͨ��˳��ŷ��ر���
{
	if(!m_bIsDBTransOk)
		return NULL;
	return (char*)(m_pReport+HMI_DBSTRING_SIZE*wIndex);

}
const char* CDBTranslator::GetRelaywordTr(WORD wRelayWordID) //�̵���
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

//�����Ǻ�������
const char* CDBTranslator::GetRWFuncTypeTr(WORD wFunTypeID) //����
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

//����������ID,����˳���
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
 * ��������: ��ȡ����ֵ�͸�����ֵ���ַ����б�����ݣ�pNameListʹָ�����ַ���
 * ����˵��: 
 *			- WORD[in] wStart �ַ��б��е���ʼ���
 *			- const char*[out] pNameList ���淵�ص��ַ����б�
 * ���ش���:  �ɹ�����TRUE�����򷵻�FALSE
 * ����: 
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
	AddLanInf("����", "");
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
 * ��������: ��ʼ�������ԣ�1����ȡϵͳ�����п��������ļ���Ϣ 2��װ�ص�ǰѡ�е�����
 * ����˵��: 
 * ���ش���: 
 * ����: 
 *****************************************************************************************/
LONG CLanFileMan::Init()
{
	//1����ȡϵͳ�����п��������ļ���Ϣ
	LoadLangsInf();

	//2)
	WORD wLanIndexnSel = DB_GetLanguageNo();
//	WORD wLanIndexnSel = 1;

	return LoadLan(wLanIndexnSel);
}

LONG CLanFileMan::LoadLan(WORD wLanIndexnSel)
{
	//�¼��ط����ļ����������Ļ�����Ҫ�����
	bool bNeedDefault = FALSE;

	
	if(wLanIndexnSel == m_nLanCurSel)
		return 0;
	if(m_nLanCount == 1) //ֻ�д����ڵ�����һ������
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

	//Դ�������ļ���
	sprintf(strFileName, "%d.bhs", m_nLanCurSel);
		
	STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc == DB_ERR_OK) && (dwFileLen > 0))
	{
		//�������ļ������Ա���
		rc = DAE_ReadFile(strFileName, m_pLangbuf,dwFileLen);
		if(rc != DB_ERR_OK)
		{
			//�����ļ�����Ļ�����Ҫж�ص������ļ�������ѡ��Ĭ�ϵ�����
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

	//������淭���ļ�����ʧ���ˣ������Լ������ݿ������ļ�
	if(bNeedDefault)
	{
		g_Trans.UnLoad();
		g_DBTrans.UnLoad();
		DB_SetLanguageNo(0);
		return 0;
	}

	//���ݿ������ļ�
	sprintf(strFileName, "%d.dbbhs", m_nLanCurSel);
	
	rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc == DB_ERR_OK) && (dwFileLen > 0))
	{
		//�������ļ������Ա���
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
	//Դ�������ļ���
//	sprintf(strFileName, "%s", GetLanName(m_nLanCurSel));
	strFileName = GetLanName(m_nLanCurSel);

	BOOL res = lanFile.Open(strFileName,CFile::modeRead|CFile::typeBinary);
	DWORD dwFileLen = lanFile.GetLength();
	lanFile.Read(m_pLangbuf,dwFileLen);
	lanFile.Close();
	g_Trans.Load(m_pLangbuf, dwFileLen);

	//���ݿⷭ���ļ�
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
	//�����ļ���Ϊ1.bhs, 2.bhs....n.bhs�������ġ�����һ��û�еģ��Ͳ����������������
	while(bIsFound)
	{
		sprintf(strFileName, "%d.bhs", i);
		
		STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
		if((rc == DB_ERR_OK) && (dwFileLen > 0))
		{
			//�������ļ������Ա���
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
	GetModuleFileName(NULL,filepath,MAX_PATH);//�õ���ǰ���г����ȫ·�� �Ժ�Ҫ��
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
			
			//�õ��ļ�����
			AddLanInf(pszFileTitle,strPathName.GetBuffer(1));
		}
		
		strPathName = fileFind.GetFilePath();
		Res = lanFile.Open(strPathName,CFile::modeRead|CFile::typeBinary);
		lanFile.Read(pszBuf,MULTILANG_PREVIEW_SIZE);
		TR_PerviewLangTitle(pszBuf,pszFileTitle);
		lanFile.Close();
		
		//�õ��ļ�����
		AddLanInf(pszFileTitle,strPathName.GetBuffer(1));
	}
#endif
	return 0;
}
