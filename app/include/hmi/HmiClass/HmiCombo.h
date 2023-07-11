/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiCombo.h
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

#ifndef __HmiCombo_H
#define __HmiCombo_H




// HmiList.h: interface for the CHmiList class.
//
//////////////////////////////////////////////////////////////////////

#define MAX_COMBOITEM 16 //最多多少行记录
#define MAX_LENGTH 32 //每行的最大数据长度

#include "HmiObj.h"
#include  <string.h>

class CHmiCombo : public CHmiObj  
{
public:
	char* GetItemText(LONG nItem);
	void SetSize(LONG x, LONG y, LONG xSize, LONG ySize);
	void SetItemTxt(int nItemIndex, char* txt);
	LONG m_nItemCount; //行数
	LONG m_nCursorWidth; //光标的宽度尺寸

	BOOL OnUp();
	BOOL OnDown();
	BOOL OnAdd();
	BOOL OnSub();
	BOOL OnEnter();

	void SelectItem(LONG nItem);

	void OnVScroll(LONG offset);

	LONG AddString(const char* pszItem);
	LONG SetItemData(LONG nItem, LONG nData);
	LONG GetItemData(LONG nItem);

	LONG GetCount( ) const;  
	LONG GetTextLen(LONG nIndex ) const;
	LONG GetCurSel( ) const;
	LONG SetCurSel(LONG nSelect );
	void ResetContent( );

	void DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1,LONG cursorMode =0);

	LONG m_nUpDownFun; //上下键功能
	LONG m_nAddSubFun; //+-键功能
	
	//是否有自定义函数
	BOOL m_bHaveUpDownFun, m_bHaveEnterFun, m_bHaveAddSubFun;


	inline void SetEnterWnd(CHmiWnd* pEnterWnd)
	{
		m_pEnterWnd = pEnterWnd;
	}
	inline CHmiWnd* GetEnterWnd()
	{
		return m_pEnterWnd;
	}
	inline void SetAddSubFun(LONG fun)
	{
		m_nAddSubFun = fun;
	}
	inline LONG GetAddSubFun()
	{
		return m_nAddSubFun;
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

	inline void SetFont(LONG font)
	{
		m_nFont = font;
	}
	inline void SetItemCount(LONG count)
	{
		m_nItemCount = count;
	}
	LONG GetItemCount()
	{
		return m_nItemCount;
	}

	CHmiWnd* m_pEnterWnd; //回车要到达的界面

	
	CHmiCombo();
	virtual ~CHmiCombo();
	void OnPaint();
protected:
	LONG m_nCurItemIndex; //当前焦点
	char m_nItemTxt[MAX_COMBOITEM][MAX_LENGTH]; //各行的字符
	LONG m_nItemData[MAX_COMBOITEM]; //关联的数据

};



/**********************/

#endif /* __HmiCombo_H */
