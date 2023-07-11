/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiGraphWnd.h
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

#ifndef __HmiGraphWnd_H
#define __HmiGraphWnd_H
#define MAX_GRAPH_BI 16
#define MAX_GRAPH_AI 16

#include "HmiDataStatic.h"
#include "HmiDBI.h"
#include "HmiWnd.h"

#define  AITYPE_AI 2
#define  AITYPE_RM 1

//显示一次还是二次值
#define  AIDISTYPE_PRI 1
#define  AIDISTYPE_SECONDERY 2



#define  BITYPE_NONE  0  //无遥信
#define  BITYPE_SPI	   1 //单点
#define  BITYPE_DPI    2 //双点


typedef struct TGraph_AIDisp
{
	WORD	wX;
	WORD	wY;
	WORD	wAIID;
	BYTE	byDisType; //显示类型， 显示1次值还是显示二次值， 默认显示二次
	BYTE	byAIType; //遥测， 保护测量值
	BYTE	byFont;
	BYTE	rsv;
}TGraph_AIDisp;

typedef struct TGraph_Disp
{
	WORD	wSwitchCount;
	WORD	wAICount;
	WORD	wGraphCount;
	WORD	wBKGraphDBIndex; //背景在背景图库序号
	WORD	wBKGraphIndex; //背景图在图片数据区序号
	char	szTitle[32];
}TGraph_Disp;
typedef struct TGraph_SwitchDisp
{
	WORD	wX;
	WORD	wY;
	WORD	wBitmapType; //图标类型, 是图标库总的序号
	WORD	wGraphIndex; //参数文件中图片数据区中图片序号
	WORD	wNegative; //是否取反
	BYTE	byInputType; //开入类型 单位置1，双位置2， 不关联开入0
	BYTE	byOutputType; //开出类型 单控1， 分合控2， 不关联开出0
	WORD	wBIOnID; //合位置开入号
	WORD	wBIOffID; //分位置开入号
	WORD	wBOCloseID; //合开出号
	WORD	wBOOpenID; //分开出号
	BYTE	byRsv[4];
}TGraph_SwitchDisp;

typedef struct TBMP_Disp
{
	WORD	wDataBase; //	数据区基地址		
	WORD	wXSize; //	宽		
	WORD	wYSize; //高
	WORD	wBPL; //bytePerLine	
}TBMP_Disp;



class CHmiGraphWnd : public CHmiWnd
{
	CHmiDataStatic m_AI[MAX_GRAPH_AI];
	CHmiDoubleBI m_BI[MAX_GRAPH_BI];
	CHmiDataStatic m_stTime; //当前时间
	CHmiStatic m_stSetArea; //定值区号
	CHmiStatic m_stCrc; //CRC校验码

	CHmiObj* m_pObjList[MAX_GRAPH_AI+MAX_GRAPH_BI+3];
	BOOL InitDlg();
	BOOL Init();
	BOOL CheckFile(BYTE* pFileBuf, DWORD dwFileLen);
	
public:
	CHmiGraphWnd();
	BOOL Load(char* pszGraphDataFile);
	void OnPaint();

	virtual ~CHmiGraphWnd();
	DECLARE_HMIMESSAGE_MAP() 
	//防止外部调用Show函数,确保只能通过DoModal来显示
private: 
//	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
	WORD m_nAICount;
	WORD m_nBICount;
		
};
/**********************/

#endif /* __HmiGraphWnd_H */
