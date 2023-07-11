/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiGraphWnd.cpp
* 
*   软件模块：
*
* 
*	描述
* 
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2011-3-30		初始创建
* 
*************************************************************************/

#ifdef __PL_WIN_SIMULATION__
#include "../stdafx.h"
//#include <winuser.h>
//@#include "LCD.H"
//@#include "LCD_Private.h"              /* include LCDConf.h */
//@#include "GUI_Private.h"
#include "../WinGui.h"
#include "../WinGuiDlg.h"
extern  CFile g_GraphFile;
#endif

#include "DaeFunc.h"
#include "Paradef.h"
#include "parafunc.h"		
#include "dbfunc.h"	

#include "HmiGraphWnd.h"


BEGIN_HMIMESSAGE_MAP(CHmiGraphWnd,CHmiWnd)
//{{HMI_MSG_MAP(CHmiGraphWnd)
///}}HMI_MSG
END_HMIMESSAGE_MAP()

CHmiGraphWnd::CHmiGraphWnd()
{
	int i;
	for(i=0; i<MAX_GRAPH_AI; i++)
	{
		m_AI[i].SetVisible(FALSE);

	}

	for(i=0; i<MAX_GRAPH_BI; i++)
	{
		m_BI[i].SetVisible(FALSE);

	}

}
CHmiGraphWnd::~CHmiGraphWnd()
{

}

BOOL CHmiGraphWnd::InitDlg()
{
	return true;

}

void CHmiGraphWnd::OnPaint()
{
	char szSet[32];
	
	WORD wSetArea = DB_GetCurrentSETAreaNo();
	sprintf(szSet, "SET:%02d", wSetArea);
	m_stSetArea.SetCaption(szSet);

	CHmiWnd::OnPaint();
}

BOOL CHmiGraphWnd::Init()
{
	//时间和定值区号校验码的位置
	SetCaption("Keyvia Electric Co.,LTD");
	m_stSetArea.SetPosition(4,146,44,160);
	m_stCrc.SetPosition(53,146,93,160);
	m_stTime.SetPosition(115,146,240,160);
//	m_stCrc.SetPosition(182,146,237,160);

	m_stTime.SetFont(12);
	m_stSetArea.SetFont(12);
	m_stCrc.SetFont(12);

	for(int i= 0; i<m_nBICount; i++)
	{
		m_pObjList[i+1] = &m_BI[i];
		m_BI[i].SetTimer(1); //定时刷新
	}

	for(int j=0; j<m_nAICount; j++)
	{
		m_pObjList[m_nBICount+1+j] = &m_AI[j];
		m_AI[j].SetTimer(1); //
	}

	m_stTime.SetDataType(Time);
	m_stTime.SetTimer(1);

	//添加到对象列表中
	m_pObjList[m_nBICount+m_nAICount+1] = &m_stTime;
	m_pObjList[m_nBICount+m_nAICount+2] = &m_stSetArea;
	m_pObjList[m_nBICount+m_nAICount+3] = &m_stCrc;

	m_pObjList[0] = NULL;
	m_ppObjList = &(m_pObjList[1]);

	SetPageNum(1);
	SetObjNum(m_nBICount+m_nAICount+3);
	m_pActiveObj = m_pObjList[1];
	m_pActiveObj->m_bIsFocus = TRUE;


	char szCRC[32];
	TCodeFileInf FileInf;
	DWORD dwCheckLen;
	if(DB_GetCodeFileVerInf(&FileInf, &dwCheckLen) == ERR_FAILURE)
	{
		sprintf(szCRC, "CRC:NULL");
	}
	else
	{
		//校验码
		sprintf(szCRC,"CRC:%X",FileInf.wCheckSum);
	}
	
	m_stCrc.SetCaption(szCRC);
	SetTimer(1);
	return true;
}

BOOL CHmiGraphWnd::CheckFile(BYTE* pFileBuf,DWORD dwFileLen)
{
	WORD wCheck = DAE_CheckSum(pFileBuf, sizeof(TFileHead)-4);

#ifdef __PL_WIN_SIMULATION__
	DWORD dwCheck = MAKELONG(0,wCheck);
#else
	DWORD dwCheck = MAKELONG(wCheck,0);
#endif
	
	TFileHead* pHead = (TFileHead*)pFileBuf;
	
	if(dwCheck != pHead->dwCheck)
	{
		LogError("GraphSelfCheck",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,pHead->dwCheck);
		return FALSE;
	}
	if(dwFileLen != pHead->dwSize)
	{
		LogError("SelfCheck",FILE_LINE,"File len  0x%X, 0x%X", dwFileLen,pHead->dwSize);
		return FALSE;
	}
	WORD* pwFilecheck =  (WORD*)(pFileBuf + dwFileLen -2);
	
	//文件
	wCheck = DAE_CheckSum(pFileBuf+sizeof(TFileHead),
		pHead->dwSize - 2-sizeof(TFileHead));

	if(wCheck != *pwFilecheck)
	{
		LogError("GraphSelfCheck",FILE_LINE,"File checksum error 0x%X, 0x%X", wCheck,*pwFilecheck);
		return FALSE;
	}
	return TRUE;
}

BOOL CHmiGraphWnd::Load(char* pszGraphDataFile)
{

	DWORD dwFileLen;
	BYTE *pBuf = NULL;
	
#ifndef __PL_WIN_SIMULATION__
	
	STATUS rc = DAE_GetFileLength(pszGraphDataFile, &dwFileLen);
	if((rc == DB_ERR_OK) && (dwFileLen > 0))
	{
		pBuf = new BYTE[dwFileLen];
		rc = DAE_ReadFile(pszGraphDataFile, pBuf,dwFileLen);
		if(rc != DB_ERR_OK)
		{
			LogError("DAE_ReadFile",FILE_LINE,"The graph Date File Read Err!");
			delete []pBuf;
			pBuf  = NULL;
		}
	}
	else
		pBuf  = NULL;
	
#else  //在windows模拟运行
	dwFileLen = g_GraphFile.GetLength();
	if (dwFileLen!=0)
	{
		pBuf = new BYTE[dwFileLen];
		g_GraphFile.Read(pBuf, dwFileLen);
	}
	else
		pBuf = NULL;
	
#endif
	
	WORD nBmpDispCount;
	TFileHead* pHead;
	TGraph_Disp* pGraph_Disp;
	TGraph_SwitchDisp* pSwitchDisp;
	TGraph_AIDisp* pAIDisp;
	TBMP_Disp* pBMPDisp;
	unsigned char* pBmpData;

	BOOL bIsGraphOk = FALSE;

	
	//得到接线图文件的信息
	if(pBuf)
	{
		//还需要校验图文件先
		bIsGraphOk = CheckFile(pBuf, dwFileLen);
	}

	if(bIsGraphOk)
	{
		pHead = (TFileHead*)pBuf;
		pGraph_Disp = (TGraph_Disp*)(pHead+1);
		pSwitchDisp = (TGraph_SwitchDisp*)(pGraph_Disp+1);
		m_nAICount = pGraph_Disp->wAICount;
		m_nBICount = pGraph_Disp->wSwitchCount;
		pAIDisp = (TGraph_AIDisp*)(pSwitchDisp+pGraph_Disp->wSwitchCount);
		pBMPDisp = (TBMP_Disp*)(pAIDisp+pGraph_Disp->wAICount);
		pBmpData = (unsigned char*)(pBMPDisp+pGraph_Disp->wGraphCount);
	}
	else
	{
		pHead = NULL;
		pGraph_Disp = NULL;
		pSwitchDisp = NULL;
		m_nAICount = 0;
		m_nBICount = 0;
		pAIDisp = NULL;
		pBMPDisp = NULL;
		pBmpData = NULL;
	}

	unsigned char *pBmp, *pBmpoff, *pBmpErr;
	int GraphDBIndex;
	
	//窗口背景位图
	if(pGraph_Disp)
		GraphDBIndex = pGraph_Disp->wBKGraphIndex;
	else
		GraphDBIndex = -1;

	if(GraphDBIndex != -1)
	{
		pBmp = pBmpData + pBMPDisp[GraphDBIndex].wDataBase;
		SetBitmap(pBmp, 0, 0 , pBMPDisp[GraphDBIndex].wXSize, 
			pBMPDisp[GraphDBIndex].wYSize, pBMPDisp[GraphDBIndex].wBPL);
		
	}

	SetCaption(pGraph_Disp->szTitle);
	//双点遥信控件
	
	int nType, nDataType, nDispType, nFont;
	//遥测控件
	for(int i=0; i<m_nAICount; i++)
	{
		if(pAIDisp[i].wAIID == (WORD)-1)
			continue;
		m_AI[i].SetVisible(true);
		//2011-5-4 AI 简称 等号 数值 单位一共占16个字符
		m_AI[i].SetPosition(pAIDisp[i].wX, pAIDisp[i].wY,
			pAIDisp[i].wX+pAIDisp[i].byFont*18/2, pAIDisp[i].wY+pAIDisp[i].byFont+2, 0);
		nType = pAIDisp[i].byAIType;
		nDispType = pAIDisp[i].byDisType;
		
		if((nType == AITYPE_AI)&&(nDispType == AIDISTYPE_PRI))
			nDataType = hAI;
		else if ((nType == AITYPE_AI)&&(nDispType == AIDISTYPE_SECONDERY))
			nDataType = lAI;
		else if((nType == AITYPE_RM)&&(nDispType == AIDISTYPE_PRI))
			nDataType = phAI;
		else if((nType == AITYPE_RM)&&(nDispType == AIDISTYPE_SECONDERY))
			nDataType = plAI;
		
		m_AI[i].SetDataSource(nDataType, pAIDisp[i].wAIID);
		m_AI[i].SetFont(pAIDisp[i].byFont);
		//2010 5-4 先读一次值，以后每次都在定时器里读取的
		m_AI[i].UpdateData(FALSE); //2
	}
	
	for(int j=0; j<m_nBICount; j++)
	{
		m_BI[j].SetVisible(true);
		m_BI[j].SetPosition(pSwitchDisp[j].wX, pSwitchDisp[j].wY, 0, 0, 0);
		GraphDBIndex = pSwitchDisp[j].wGraphIndex;
		
		pBmp = (unsigned char*)(pBmpData + pBMPDisp[GraphDBIndex].wDataBase);
		pBmpoff = (unsigned char*)(pBmpData + pBMPDisp[GraphDBIndex+1].wDataBase);
		pBmpErr = (unsigned char*)(pBmpData + pBMPDisp[GraphDBIndex+2].wDataBase);
		
		m_BI[j].SetDBIBitmap(pBmp, pBmpoff, pBMPDisp[GraphDBIndex].wXSize, 
			pBMPDisp[GraphDBIndex].wYSize, pBMPDisp[GraphDBIndex].wBPL, 
			pSwitchDisp[j].wBIOnID, pSwitchDisp[j].byInputType, 
			pSwitchDisp[j].wBIOffID, pBmpErr);
		
		//2010 5-4 先读一次值，以后每次都在定时器里读取的
		m_BI[j].UpdateData(FALSE);
	}
	
	Init();
	
	return 0;
}
