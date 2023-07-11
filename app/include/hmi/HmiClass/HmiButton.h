/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiButton.h
* 
*   软件模块：界面控件
*
* 
*	描述	按钮控件
* 
* 
* 
*   函数	
*			SetEnterWnd(CHmiWnd* pEnterWnd) 设置回车跳转目标窗口
*			SetHaveEnterFun(BOOL b_HaveEnterFun) 设置是否有回车响应函数
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-4-23		初始创建
* 
*************************************************************************/

#ifndef __HmiButton_H
#define __HmiButton_H



#include "LcdDef.h"

// HmiButton.h: interface for the CHmiButton class.
//
//////////////////////////////////////////////////////////////////////


#include "HmiObj.h"

class CHmiButton : public CHmiObj  
{
public:

	void OnPaint();
	BOOL OnEnter();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnLeft();
	BOOL OnRight();

	CHmiButton();
	CHmiButton(LONG x, LONG y, LONG xsize, LONG ysize);
	virtual ~CHmiButton();

	inline void SetEnterWnd(CHmiWnd* pEnterWnd)
	{
		m_pEnterWnd = pEnterWnd;
	}
	inline CHmiWnd* GetEnterWnd()
	{
		return m_pEnterWnd;
	}
	inline void SetHaveEnterFun(BOOL b_HaveEnterFun)
	{
		m_bHaveEnterFun = b_HaveEnterFun;
	}
	inline BOOL GetHaveEnterFun()
	{
		return m_bHaveEnterFun;
	}

private:
	CHmiWnd* m_pEnterWnd; //回车要到达的界面
	BOOL m_bHaveEnterFun; //是否有自定义回车函数 

};


/**********************/

#endif /* __HmiButton_H */
