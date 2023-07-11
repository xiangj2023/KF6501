/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiList.h
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

#ifndef __HmiList_H
#define __HmiList_H

//Ϊtrue, Ϊstatic�ǣ� �����б���һ���洢�ռ�(ӦΪĿǰҺ��ͬʱֻ����ʾһ�����ڣ� ���޶�ÿ������ֻ��һ���б�
//#define LIST_SHARE_SPACE TRUE
#define  LISTMEMDEF  //static //����Ϊstaticʱ���б���һ���洢�ռ䣨ÿ���������ֻ����һ���б�


// HmiList.h: interface for the CHmiList class.
//
//////////////////////////////////////////////////////////////////////

#define MAX_LISTITEM 32 //�������м�¼
//#define MAX_LENGTH 64 //ÿ�е�������ݳ���

#define MAX_LISTCOLMN 3 //�������У���Ϊ4��
#define  MAX_TXTLEN 64  //ÿ�е�������ݳ���

#include "HmiObj.h"
#include  <string.h>

//�����ݽṹ
typedef struct TListItem
{
	char szColumnTxt[MAX_LISTCOLMN][MAX_TXTLEN]; //һ���ڸ��д洢������
	long nData; 
}TListItem;

class CHmiList : public CHmiObj  
{
public:
	char* GetItemText(LONG nItem, LONG nSubItem); //��ȡnItem�У�nSubItem�е�����
	void SetItemText(LONG nItem, LONG nSubItem, char* txt);
	void SetSize(LONG x, LONG y, LONG xSize, LONG ySize);
	void SetItemTxt(int nItemIndex, char* txt);

	BOOL OnRight();
	BOOL OnLeft();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnEnter();
	BOOL OnEsc();

	void SelectItem(LONG nItem, LONG nSubItem); //ѡ��nItem�У�nSubItem�е��ı�

	void OnVScroll(LONG offset);
	void OnChgPage(LONG offset); //��ҳ

	LONG AddRow(const char* pszItem); //����һ�� pszItem����0�е��ַ���
	LONG SetItemData(LONG nItem, LONG nData);
	LONG GetItemData(LONG nItem);


	LONG GetCount( ) const;  
	LONG GetCurSel( ) const; //�õ���ǰѡ�е��к�
	LONG SetCurSel(LONG nSelect );

	LONG GetCurCol(); //�õ���ǰѡ�е��к�
	void SetCurCol(LONG nColunm);

	void ResetContent( ); //�������

	void DrawCursor(LONG x0, LONG y0, LONG x1, LONG y1);
	void SetColPos(LONG col, LONG pos); //�����е���ʼλ��pos�����ַ�������������λ��
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

	CHmiWnd* m_pEnterWnd; //�س�Ҫ����Ľ���

	CHmiList();
	virtual ~CHmiList();
	void OnPaint();
protected:
	void OnChgValue(LONG offset, char strList[][32] = NULL, LONG num =0);
	LONG m_nCurItemIndex; //��ǰ�����Ǳ�ҳ�ڵڼ���(
	LONG m_nCurPage; //��ǰ�ǵڼ�ҳ

//	char m_nItemTxt[MAX_LISTITEM][MAX_LENGTH]; //���е��ַ���
//	LONG m_nItemData[MAX_LISTITEM];
	LONG m_nColPos[MAX_LISTCOLMN]; //���е�λ�ã���ʼƫ������

	TListItem m_tItem[MAX_LISTITEM]; 

	LONG m_nPageSize; //ÿҳ��ʾ�����У����ݸ߶Ⱥ��и߼����)
	LONG m_nItemHeight;//�и�
	LONG m_nItemCount; //����
	LONG m_nColumnCount; //����
	LONG m_nCursorPos; //���λ�ã�����������еĵڼ����ַ�
	LONG m_nCursorWidth; //���Ŀ�ȳߴ�

	LONG m_nUpDownFun; //���¼�����
	LONG m_nLeftRightFun; // ���Ҽ�����

	LONG m_nEditableColumn; //���޸ĵ��е���� -1Ϊû�п��޸ĵ�
	LONG m_nCurColumnIndex; //��ǰ����ڼ���(�༭̬)
	LONG m_nMaxLenEditable; //�ɱ༭�е���󳤶�
	LONG m_bIsShowNegative; //�Ƿ���������
	
	//���ÿɱ༭�кţ� �ɱ༭����󳤶ȣ� �ɱ༭���Ƿ���ʾ������
	LONG SetEditableColum(LONG nCol, LONG nMaxLen, BOOL bIsShowNagetive = FALSE);
	
	//�Ƿ����Զ��庯��
	BOOL m_bHaveSelChgFun, m_bHaveUpDownFun, m_bHaveLeftRightFun, m_bHaveEnterFun, m_bHaveAddSubFun;
};



/**********************/

#endif /* __HmiList_H */
