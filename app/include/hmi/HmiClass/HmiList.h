/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiList.h
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
*		kangqian	2008-5-13		初始创建
* 
*************************************************************************/

#ifndef __HmiList_H
#define __HmiList_H

//为true, 为static是， 所有列表共用一个存储空间(应为目前液晶同时只能显示一个窗口， 且限定每个窗口只有一个列表
//#define LIST_SHARE_SPACE TRUE
#define  LISTMEMDEF  //static //定义为static时，列表共有一个存储空间（每个窗口最多只能有一个列表）


// HmiList.h: interface for the CHmiList class.
//
//////////////////////////////////////////////////////////////////////

#define MAX_LISTITEM 32 //最多多少行记录
//#define MAX_LENGTH 64 //每列的最大数据长度

#define MAX_LISTCOLMN 3 //最多多少列，设为4列
#define  MAX_TXTLEN 64  //每列的最大数据长度

#include "HmiObj.h"
#include  <string.h>

//行数据结构
typedef struct TListItem
{
	char szColumnTxt[MAX_LISTCOLMN][MAX_TXTLEN]; //一行内各列存储的数据
	long nData; 
}TListItem;

class CHmiList : public CHmiObj  
{
public:
	char* GetItemText(LONG nItem, LONG nSubItem); //读取nItem行，nSubItem列的数据
	void SetItemText(LONG nItem, LONG nSubItem, char* txt);
	void SetSize(LONG x, LONG y, LONG xSize, LONG ySize);
	void SetItemTxt(int nItemIndex, char* txt);

	BOOL OnRight();
	BOOL OnLeft();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnEnter();
	BOOL OnEsc();

	void SelectItem(LONG nItem, LONG nSubItem); //选中nItem行，nSubItem列的文本

	void OnVScroll(LONG offset);
	void OnChgPage(LONG offset); //翻页

	LONG AddRow(const char* pszItem); //增加一行 pszItem：第0列的字符串
	LONG SetItemData(LONG nItem, LONG nData);
	LONG GetItemData(LONG nItem);


	LONG GetCount( ) const;  
	LONG GetCurSel( ) const; //得到当前选中的行号
	LONG SetCurSel(LONG nSelect );

	LONG GetCurCol(); //得到当前选中的列号
	void SetCurCol(LONG nColunm);

	void ResetContent( ); //清空内容

	void DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1);
	void SetColPos(LONG col, LONG pos); //设置列的起始位置pos代表字符数，不是像素位置
	LONG GetColPos(LONG col);



	inline void SetEnterWnd(CHmiWnd* pEnterWnd)
	{
		m_pEnterWnd = pEnterWnd;
	}
	inline CHmiWnd* GetEnterWnd()
	{
		return m_pEnterWnd;
	}

	inline void SetLRFun(LONG fun)
	{
		m_nLeftRightFun = fun;
	}
	inline LONG GetLRFun()
	{
		return m_nLeftRightFun ;
	}

	inline void SetUDFun(LONG fun)
	{
		m_nUpDownFun = fun;
	}

	inline LONG GetUDFun()
	{
		return m_nUpDownFun;
	}

	inline void SetHaveEnterFun(BOOL bHaveEnterFun)
	{

		m_bHaveEnterFun = bHaveEnterFun;
	}
	inline BOOL GetHaveEnterFun()
	{
		return m_bHaveEnterFun;
	}

	inline void SetHaveAddSubFun(BOOL bHaveAddSubFun)
	{
		m_bHaveAddSubFun = bHaveAddSubFun;
	}
	inline BOOL GetHaveAddSubFun()
	{
		return m_bHaveAddSubFun;
	}

	void SetHaveUpDownFun(BOOL bHaveUpDownFun)
	{
		m_bHaveUpDownFun = bHaveUpDownFun;
	}
	BOOL GetHaveUpDownFun()
	{
		return m_bHaveUpDownFun;
	}

	void SetHaveLeftRightFun(BOOL bHaveLeftRightFun)
	{
		m_bHaveLeftRightFun =  bHaveLeftRightFun;
	}
	BOOL GetHaveLeftRightFun()
	{
		return m_bHaveLeftRightFun;
	}

	inline void SetFont(LONG font)
	{
		m_nFont = font;
		m_nItemHeight = font+2;
	}
	inline void SetCurorWidth(LONG width)
	{
		m_nCursorWidth = width;
	}
	inline void SetItemCount(LONG count)
	{
		m_nItemCount = count;
	}
	LONG GetItemCount()
	{
		return m_nItemCount;
	}
	void SetColumnCount(LONG count)
	{
		m_nColumnCount = count;
	}
	LONG GetColumnCount()
	{
		return m_nColumnCount;
	}

	CHmiWnd* m_pEnterWnd; //回车要到达的界面

	CHmiList();
	virtual ~CHmiList();
	void OnPaint();
protected:
	void OnChgValue(LONG offset, char strList[][32] = NULL, LONG num =0);
	LONG m_nCurItemIndex; //当前焦点是本页内第几行(
	LONG m_nCurPage; //当前是第几页

//	char m_nItemTxt[MAX_LISTITEM][MAX_LENGTH]; //各行的字符串
//	LONG m_nItemData[MAX_LISTITEM];
	LONG m_nColPos[MAX_LISTCOLMN]; //各列的位置，起始偏移像素

	TListItem m_tItem[MAX_LISTITEM]; 

	LONG m_nPageSize; //每页显示多少行（根据高度和行高计算出)
	LONG m_nItemHeight;//行高
	LONG m_nItemCount; //行数
	LONG m_nColumnCount; //列数
	LONG m_nCursorPos; //光标位置，光标在输入列的第几个字符
	LONG m_nCursorWidth; //光标的宽度尺寸

	LONG m_nUpDownFun; //上下键功能
	LONG m_nLeftRightFun; // 左右键功能

	LONG m_nEditableColumn; //可修改的列的序号 -1为没有可修改的
	LONG m_nCurColumnIndex; //当前焦点第几列(编辑态)
	LONG m_nMaxLenEditable; //可编辑列的最大长度
	LONG m_bIsShowNegative; //是否有正负号
	
	//设置可编辑列号， 可编辑列最大长度， 可编辑列是否显示正负号
	LONG SetEditableColum(LONG nCol, LONG nMaxLen, BOOL bIsShowNagetive = FALSE);
	
	//是否有自定义函数
	BOOL m_bHaveSelChgFun, m_bHaveUpDownFun, m_bHaveLeftRightFun, m_bHaveEnterFun, m_bHaveAddSubFun;
};



/**********************/

#endif /* __HmiList_H */
