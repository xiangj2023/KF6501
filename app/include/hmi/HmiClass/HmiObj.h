/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiObj.h
* 
*   ���ģ�飺����ؼ�
*
* 
*	����	���н���ؼ����࣬ ���幫���ӿڵ�
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
*		kangqian	2008-03-03		��ʼ����
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


//ͨ�ú����� �õ������ַ���
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
//�������
enum HmiCusorMOD
{
	CusorUnderLine =0, //�»���
	CusorHighLight =1, //����
};


class CHmiObj  
{

public:
	
	DECLARE_HMI_TR()
	BOOL UpdateWindow( );
	BOOL InvalidateRect(LONG l,LONG t,LONG r,LONG b, BOOL bErase = TRUE); //�����������Ч��
	void Invalidate(BOOL bErase = TRUE); //�Ѹÿؼ�������Ч��
	void ReDraw(); //���ƣ�����������

	//Ϊʹ����ṹ�����������������ʵ�ִ����Ƿ���.h�ļ��У�����ʽʹ��inline
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

		//2010-5-11�� �ؼ���Ⱥ͸߶Ȳ���С��0
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

	//timer ʱ��������λ��  timer����0ʱ������ʱ����С�ڵ���0ʱֹͣ��ʱ����
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

	virtual BOOL OnEnter(); //Enter����Ӧ����
	virtual BOOL OnEsc();	//esc����Ӧ����
	virtual BOOL OnRight(); //������Ӧ����
	virtual BOOL OnLeft();	//������Ӧ����
	virtual BOOL OnUp(); //������Ӧ����
	virtual BOOL OnDown(); //������Ӧ����
	virtual BOOL OnSub(); //-����Ӧ����
	virtual BOOL OnAdd(); //+����Ӧ����
	virtual void UpdateData(BOOL bIsSave = TRUE);

	void SetBitmap(unsigned char* bitmap, LONG l=0, LONG t=0, LONG Width=0, LONG Height=0, LONG bytePerLine=8);
	void SetCaption(const char *szCaption);
	const char* GetCaption();

 	//����Ϣ����ֱ�ӵ�����Ϣ���������������֮��ŷ���
	LONG SendMessage( LONG message, LONG wParam = 0, LONG lParam = 0 );

	//����Ϣ����Ϣ���в���������
	BOOL PostMessage( LONG message, LONG wParam = 0, LONG lParam = 0 );
	void EnableAutoEnterWnd();
	void DisableAutoEnterWnd();

	CHmiObj();
	CHmiObj(LONG x, LONG y, LONG xsize, LONG ysize);

	virtual ~CHmiObj();

	LONG m_nX, m_nY, m_nXSize, m_nYSize; // λ�ã���С, ֧��2���������ԵĲ�ͬλ��
	LONG m_nTxtX, m_nTxtY; //�����λ�ã����ڰ�ť��statica�ؼ�������λ��Ĭ�Ͼ��У���Ҫ���㣩

	LONG m_nTxtIndex; //�ַ���ָ��
	LONG m_nOwnerWndIndex; //��������ָ��
	CHmiWnd* m_pOwner; // ָ����������
	LONG m_nTabOrder; //�������,�޽���Ľ������Ϊ����
	BOOL m_bIsFocus;

	LONG m_nFont; //���� 
	LONG m_nAlign; //���뷽ʽ
	LONG m_nObjType; //��ʲô���͵Ŀؼ������ڣ��б� ��ť ��������
	LONG m_nPageIndex; // �ڸ�wnd��ҳ����

	LONG m_nFreshTimer; //��ʱ��ʱ������ ��λΪ�룬 С�ڵ���0ʱ��ˢ��

	static LONG s_nLangIndex; //��ǰʹ�õ�����

	unsigned char* m_bmData; //λͼ����
	LONG m_bmLeft, m_bmTop, m_bmHeight, m_bmWidth, m_bmBPL; //λͼ���ϣ� �ߣ��� ÿ���ֽ���

	char m_szCaption[LCD_STRLEN];

	WORD m_nTimerValue; //��ʱ����ʱ
	LONG m_nCursorType; //�������
	BOOL m_bIsAutoEnterWnd; //�Ƿ��Զ���ת���س�����


private:
	BOOL m_bIsVisible; //�Ƿ�ɼ�
	

};

typedef void (*YES_NO_PTR)(void);		//YES_NO��Ӧ���� 

/**********************/
#endif /* __HmiObj_H */
