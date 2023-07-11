/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiCombo.h
* 
*   ���ģ�飺
*
* 
*	���� 
* 
* 
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-5-13		��ʼ����
* 
*************************************************************************/

#ifndef __HmiCombo_H
#define __HmiCombo_H




// HmiList.h: interface for the CHmiList class.
//
//////////////////////////////////////////////////////////////////////

#define MAX_COMBOITEM 16 //�������м�¼
#define MAX_LENGTH 32 //ÿ�е�������ݳ���

#include "HmiObj.h"
#include  <string.h>

class CHmiCombo : public CHmiObj  
{
public:
	char* GetItemText(LONG nItem);
	void SetSize(LONG x, LONG y, LONG xSize, LONG ySize);
	void SetItemTxt(int nItemIndex, char* txt);
	LONG m_nItemCount; //����
	LONG m_nCursorWidth; //���Ŀ�ȳߴ�

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

	LONG m_nUpDownFun; //���¼�����
	LONG m_nAddSubFun; //+-������
	
	//�Ƿ����Զ��庯��
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

	CHmiWnd* m_pEnterWnd; //�س�Ҫ����Ľ���

	
	CHmiCombo();
	virtual ~CHmiCombo();
	void OnPaint();
protected:
	LONG m_nCurItemIndex; //��ǰ����
	char m_nItemTxt[MAX_COMBOITEM][MAX_LENGTH]; //���е��ַ�
	LONG m_nItemData[MAX_COMBOITEM]; //����������

};



/**********************/

#endif /* __HmiCombo_H */
