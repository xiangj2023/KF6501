/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiObj.h
* 
*   软件模块：界面控件
*
* 
*	描述	所有界面控件基类， 定义公共接口等
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
*		kangqian	2008-03-03		初始创建
* 
*************************************************************************/


#ifndef __HmiObj_H
#define __HmiObj_H

#include "LcdDef.h"
#include "translator.h"
//@#include "HmiWnd.h"
// HmiObj.h: interface for the CHmiObj class.
//
//////////////////////////////////////////////////////////////////////
//@typedef void WM_FUNCTION(LONG parm1, LONG parm2);
//@#define OBJTYPE_EDIT  1
//@#define OBJTYPE_BUTTON 2
//@#define OBJTYPE_STATIC 3
//@#define OBJTYPE_LIST 4
//@#define OBJTYPE_WND	5

#define HMI_LIST_NORECORD  0xFFFF


//通用函数， 得到浮点字符串
void GetFloatString(LONG value, LONG nDec, char* str,LONG dwDataType);

#include "HmiMSG.h"

class CHmiWnd;
enum HmiObjType 
{
		HmiWnd = 1,
		HmiButton = 2,
		HmiEdit = 3,
		HmiList = 4,
		HmiStatic = 5,
		HmiCombo = 6,

};
//光标类型
enum HmiCusorMOD
{
	CusorUnderLine =0, //下划线
	CusorHighLight =1, //反黑
};


class CHmiObj  
{

public:
	
	DECLARE_HMI_TR()
	BOOL UpdateWindow( );
	BOOL InvalidateRect(LONG l,LONG t,LONG r,LONG b, BOOL bErase = TRUE); //把区域加入无效区
	void Invalidate(BOOL bErase = TRUE); //把该控件加入无效区
	void ReDraw(); //绘制（到数据区）

	//为使代码结构看起来更清楚，不管实现代码是否在.h文件中，都显式使用inline
	inline void SetSize(LONG x , LONG y, LONG xsize, LONG ysize)
	{
		m_nX = x;
		m_nY = y;
		m_nXSize = xsize;
		m_nYSize = ysize;
	}
	
	void SetCusorType(LONG nType)
	{
		m_nCursorType = nType;
	}

	LONG GetCursorType()
	{
		return m_nCursorType;
	}

	inline LONG GetX()
	{
		return m_nX;
	}
	inline LONG GetY()
	{
		return m_nY;
	}
	inline LONG GetXSize()
	{
		return m_nXSize;
	}
	inline LONG GetYSize()
	{
		return m_nYSize;
	}
	inline LONG GetPageIndex()
	{
		return m_nPageIndex;
	}
	inline void SetOwner(CHmiWnd* pOwner)
	{
		m_pOwner = pOwner;
	}
	inline CHmiWnd* GetOwner()
	{
		return (CHmiWnd*)m_pOwner;
	}

	inline void SetFocus(BOOL bFocus)
	{
		m_bIsFocus = bFocus;
	}
	inline BOOL GetFocus()
	{
		return m_bIsFocus;
	}

	inline void SetTabOrder(LONG order)
	{
		m_nTabOrder = order;
	}
	inline LONG GetTabOrder()
	{
		return m_nTabOrder;
	}

	inline void SetFont(LONG font)
	{
		if(font !=12)
			font = 16;
		m_nFont = font;
	}
	LONG GetFont(void)
	{
		return m_nFont;
	}

	inline void SetAlign(LONG Align)
	{
		m_nAlign = Align;
	}

	inline void SetPosition(LONG left, LONG top, LONG right, LONG bottom, LONG page=0)
	{
		m_nX = left;
		m_nY = top;
		m_nXSize = right -left;
		m_nYSize = bottom - top;

		//2010-5-11， 控件宽度和高度不能小于0
		if(m_nXSize <0)
			m_nXSize =1;
		if(m_nYSize <0)
			m_nYSize =1;
		m_nPageIndex = page;
		
	}

	LONG GetAlign()
	{
		return m_nAlign;
	}

	//timer 时间间隔，单位秒  timer大于0时启动定时器，小于等于0时停止定时器，
	void SetTimer(LONG timer)
	{
		m_nFreshTimer = timer;
	}
	LONG GetTimer()
	{
		return m_nFreshTimer;
	}
	void SetVisible(BOOL bIsVisible)
	{

		m_bIsVisible = bIsVisible;
	}
	BOOL  GetVisible()
	{
		return m_bIsVisible;

	}

	virtual BOOL Init();
	virtual void OnPaint();

	virtual BOOL OnEnter(); //Enter键响应函数
	virtual BOOL OnEsc();	//esc键响应函数
	virtual BOOL OnRight(); //→键响应函数
	virtual BOOL OnLeft();	//←键响应函数
	virtual BOOL OnUp(); //↑键响应函数
	virtual BOOL OnDown(); //↓键响应函数
	virtual BOOL OnSub(); //-键响应函数
	virtual BOOL OnAdd(); //+键响应函数
	virtual void UpdateData(BOOL bIsSave = TRUE);

	void SetBitmap(unsigned char* bitmap, LONG l=0, LONG t=0, LONG Width=0, LONG Height=0, LONG bytePerLine=8);
	void SetCaption(const char *szCaption);
	const char* GetCaption();

 	//发消息并且直接调用消息处理函数，处理完成之后才返回
	LONG SendMessage( LONG message, LONG wParam = 0, LONG lParam = 0 );

	//发消息到消息队列并立即返回
	BOOL PostMessage( LONG message, LONG wParam = 0, LONG lParam = 0 );
	void EnableAutoEnterWnd();
	void DisableAutoEnterWnd();

	CHmiObj();
	CHmiObj(LONG x, LONG y, LONG xsize, LONG ysize);

	virtual ~CHmiObj();

	LONG m_nX, m_nY, m_nXSize, m_nYSize; // 位置，大小, 支持2种在线语言的不同位置
	LONG m_nTxtX, m_nTxtY; //标题的位置（对于按钮和statica控件，标题位置默认居中，需要计算）

	LONG m_nTxtIndex; //字符串指针
	LONG m_nOwnerWndIndex; //所属窗口指针
	CHmiWnd* m_pOwner; // 指向所属窗口
	LONG m_nTabOrder; //焦点序号,无焦点的焦点序号为负数
	BOOL m_bIsFocus;

	LONG m_nFont; //字体 
	LONG m_nAlign; //对齐方式
	LONG m_nObjType; //是什么类型的控件（窗口，列表， 按钮 。。。）
	LONG m_nPageIndex; // 在父wnd中页码数

	LONG m_nFreshTimer; //定时器时间间隔， 单位为秒， 小于等于0时不刷新

	static LONG s_nLangIndex; //当前使用的语言

	unsigned char* m_bmData; //位图数据
	LONG m_bmLeft, m_bmTop, m_bmHeight, m_bmWidth, m_bmBPL; //位图左，上， 高，宽， 每行字节数

	char m_szCaption[LCD_STRLEN];

	WORD m_nTimerValue; //定时器计时
	LONG m_nCursorType; //光标类型
	BOOL m_bIsAutoEnterWnd; //是否自动跳转到回车窗口


private:
	BOOL m_bIsVisible; //是否可见
	

};

typedef void (*YES_NO_PTR)(void);		//YES_NO响应函数 

/**********************/
#endif /* __HmiObj_H */
