/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiEdit.h
* 
*   软件模块：液晶界面 
*
* 
*	描述  输入框
* 
*   函数
* 
*  
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-5-14		初始创建
* 
*************************************************************************/

#ifndef __HmiEdit_H
#define __HmiEdit_H



#include "LcdDef.h"

// HmiEdit.h: interface for the CHmiEdit class.
//
//////////////////////////////////////////////////////////////////////


#include "HmiObj.h"

enum EDisplayFormat
{
	Dec = 10, //十进制
	Bin = 2, //二进制
	Hex = 16, //十六进制
	Boolean,
	String, //字符串， 输入的是值， 显示的是字符串（如 0. AT, 1.BT, 2.) 当显示AT是+1则显示BT, 而返回值也是1.)
};

typedef struct Item
{
	char* pszItem;
	LONG nValue;
}Item;

#ifndef __PL_WIN_SIMULATION__
typedef struct POINT 
{
	LONG x;
	LONG y;
}POINT;
#endif

class CHmiEdit : public CHmiObj  
{
public:
	//void SetCaption(const char* str);
	//const char* GetCaption();
	
	void SetValueRange(float max, float min);
	void UpdateData(BOOL bIsSave = TRUE);
	void OnPaint();
	BOOL OnAdd();
	BOOL OnSub();
	BOOL OnLeft();
	BOOL OnRight();
	BOOL OnEnter();
	CHmiEdit();
	virtual ~CHmiEdit();
	void SetValue(float val, LONG nDecimalNum, LONG nLength);
	CHmiWnd* m_pEnterWnd; //回车要到达的界面

	//设置值得上下限
	void SetUpperLimit( float fMax );
	float GetUpperLimit( ) const;

	void SetLowerLimit( float fMin );
	float GetLowerLimit( ) const;
	inline float GetValue()
	{
		return m_fCurValue;
	}
	inline void SetValue(float value)
	{
		m_fCurValue = value;
	}

	//设置最大长度， 及最多显示几位，包括小数点的
	inline void SetMaxLen(LONG len)
	{
		if(m_nMaxLen >= LCD_STRLEN)
			m_nMaxLen = LCD_STRLEN-1;
		m_nMaxLen = len;
		m_szCaption[len] = '\0';

	}
	inline LONG GetMaxLen()
	{
		return m_nMaxLen;
	}

	inline void SetDecimalNum(LONG decnum)
	{
		m_nDecimalNum = decnum;
		if((decnum>0)&&(decnum<m_nMaxLen))
			m_szCaption[m_nMaxLen-decnum-1] = '.';
	}

	inline LONG GetDecimalNum()
	{
		return m_nDecimalNum;
	}

	inline void SetIsShowNegative(BOOL  bIsShow)
	{
		m_bIsShowNegative = bIsShow;
	}
	inline BOOL GetIsShowNegative()
	{
		return m_bIsShowNegative;
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


	inline void SetAddSubFun(LONG fun)
	{
		m_nAddSubFun = fun;
	}
	inline LONG GetAddSubFun()
	{
		return m_nAddSubFun;
	}
	inline void SetCursorPos(LONG pos)
	{
		m_nCursorPos = pos;
	}
	inline LONG GetCursorPos()
	{
		return m_nCursorPos;
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

	inline void SetHaveLeftRightFun(BOOL bHaveLeftRightFun)
	{
		m_bHaveLeftRightFun = bHaveLeftRightFun;
	}
	inline BOOL GetHaveLeftRightFun()
	{
		return m_bHaveLeftRightFun;
	}

	inline void SetHaveUpDownFun(BOOL bHaveUpDownFun)
	{
		m_bHaveUpDownFun = bHaveUpDownFun;
	}
	inline BOOL GetHaveUpDownFun()
	{
		return m_bHaveUpDownFun;
	}



private:
	void DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1);


	LONG m_nUpDownFun; //上下键功能
	LONG m_nLeftRightFun; // 左右键功能
	LONG m_nAddSubFun; //+-键功能

	//是否有自定义函数
	BOOL m_bHaveEnterFun, m_bHaveLeftRightFun, m_bHaveUpDownFun, m_bHaveAddSubFun; 

	LONG m_nCursorPos; // 鼠标位置，第几个字符位置
	LONG m_nMaxLen; //最大长度
	float m_fMAx, m_fMin; //最大最小值
	BOOL m_bIsShowNegative; //是否显示正负号
	float m_fCurValue; //当前值
	LONG m_nCursorType; //光标类型//鼠标模式，下划线0， 反黑1，
	LONG m_nDecimalNum; //小数位数
	LONG m_nDisplayType; //显示类型 时间， 如2进制，10进制，16进制,bool型（是否还可以细化为取，不娶，投，退等呢）
//	char  m_szCaption[LCD_STRLEN]; // 值

//	LONG m_nCurSel; //当前选中了第几个字符串

};



/**********************/

#endif /* __HmiEdit_H */
