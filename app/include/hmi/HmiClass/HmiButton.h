/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiButton.h
* 
*   ���ģ�飺����ؼ�
*
* 
*	����	��ť�ؼ�
* 
* 
* 
*   ����	
*			SetEnterWnd(CHmiWnd* pEnterWnd) ���ûس���תĿ�괰��
*			SetHaveEnterFun(BOOL b_HaveEnterFun) �����Ƿ��лس���Ӧ����
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-4-23		��ʼ����
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
	CHmiWnd* m_pEnterWnd; //�س�Ҫ����Ľ���
	BOOL m_bHaveEnterFun; //�Ƿ����Զ���س����� 

};


/**********************/

#endif /* __HmiButton_H */
