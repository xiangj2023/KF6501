/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   �ļ����ƣ�HmiStatic.h
*   ���ģ�飺<...>
*   �� �� �ţ�1.0
*   �������ڣ�2008-4-3
*   ��    �ߣ�kangqian
*   ��    �ܣ�<...>
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
	int m_nUpdateTimer; //-1��ˢ�£�����ʱˢ��ʱ�ӱ�ţ������ڵ�ʱ�ӣ�
	int m_nBoolType;//�����bool�ͱ����Ļ������ʾ���Ƿ� Ͷ�ˣ��ֺϣ� ���
	bool m_bAutoMultiLine; //�Ƿ������ʾ��һ����ʾ���µ�ʱ���Զ�������ʾ)
	
private:
};


/**********************/

#endif /* __HmiStatic_H */

/*************************************************************************
* �Ķ���ʷ��¼��

Revision 1.0, 2008-4-3, kangqian
describe: ��ʼ����.
*************************************************************************/
