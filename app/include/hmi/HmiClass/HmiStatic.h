/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：HmiStatic.h
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-4-3
*   作    者：kangqian
*   功    能：<...>
* 
*************************************************************************/

#ifndef __HmiStatic_H
#define __HmiStatic_H


// HmiStatic.h: interface for the CHmiStatic class.
//
//////////////////////////////////////////////////////////////////////


#include "HmiObj.h"

class CHmiStatic : public CHmiObj  
{
public:
	CHmiStatic();
	virtual ~CHmiStatic();
	void OnPaint();
	int m_nUpdateTimer; //-1不刷新，，定时刷新时钟编号（父窗口的时钟）
	int m_nBoolType;//如果是bool型变量的话如何显示（是否， 投退，分合， 框框）
	bool m_bAutoMultiLine; //是否多行显示（一行显示不下的时候自动换行显示)
	
private:
};


/**********************/

#endif /* __HmiStatic_H */

/*************************************************************************
* 改动历史纪录：

Revision 1.0, 2008-4-3, kangqian
describe: 初始创建.
*************************************************************************/
