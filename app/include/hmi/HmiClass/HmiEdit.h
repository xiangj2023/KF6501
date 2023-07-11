/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiEdit.h
* 
*   ���ģ�飺Һ������ 
*
* 
*	����  �����
* 
*   ����
* 
*  
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-5-14		��ʼ����
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
	Dec = 10, //ʮ����
	Bin = 2, //������
	Hex = 16, //ʮ������
	Boolean,
	String, //�ַ����� �������ֵ�� ��ʾ�����ַ������� 0. AT, 1.BT, 2.) ����ʾAT��+1����ʾBT, ������ֵҲ��1.)
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
	CHmiWnd* m_pEnterWnd; //�س�Ҫ����Ľ���

	//����ֵ��������
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

	//������󳤶ȣ� �������ʾ��λ������С�����
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


	LONG m_nUpDownFun; //���¼�����
	LONG m_nLeftRightFun; // ���Ҽ�����
	LONG m_nAddSubFun; //+-������

	//�Ƿ����Զ��庯��
	BOOL m_bHaveEnterFun, m_bHaveLeftRightFun, m_bHaveUpDownFun, m_bHaveAddSubFun; 

	LONG m_nCursorPos; // ���λ�ã��ڼ����ַ�λ��
	LONG m_nMaxLen; //��󳤶�
	float m_fMAx, m_fMin; //�����Сֵ
	BOOL m_bIsShowNegative; //�Ƿ���ʾ������
	float m_fCurValue; //��ǰֵ
	LONG m_nCursorType; //�������//���ģʽ���»���0�� ����1��
	LONG m_nDecimalNum; //С��λ��
	LONG m_nDisplayType; //��ʾ���� ʱ�䣬 ��2���ƣ�10���ƣ�16����,bool�ͣ��Ƿ񻹿���ϸ��Ϊȡ����Ȣ��Ͷ���˵��أ�
//	char  m_szCaption[LCD_STRLEN]; // ֵ

//	LONG m_nCurSel; //��ǰѡ���˵ڼ����ַ���

};



/**********************/

#endif /* __HmiEdit_H */
