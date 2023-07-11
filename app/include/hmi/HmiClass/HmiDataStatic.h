/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiDataStatic.h
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
*		kangqian	2008-5-21		��ʼ����
* 
*************************************************************************/

#ifndef __HmiDataStatic_H
#define __HmiDataStatic_H


// HmiDataStatic.h: interface for the CHmiDataStatic class.
//
//////////////////////////////////////////////////////////////////////
//@	WORD			nAI;				//ң�����	
//@	WORD			nBI;				//ң�Ÿ���	
//@	WORD			nCI;				//��ȸ���	
//@	WORD			nBO;				//ң�ظ���	
//@	WORD			nAO;				//ң������	

#include "Paradef.h"
enum EDataType
{
	hAI = 0, //ң��һ��
	lAI, //ң�����
	phAI, //����һ��
	plAI, //��������
	BI, //ң��
	CI, //���
	Time, //ʱ��
	DI,	//ֱ�Ӳ����㣬����CI��BI
};

#include "HmiStatic.h"

class CHmiDataStatic : public CHmiStatic  
{
public:
	CHmiDataStatic();
	virtual ~CHmiDataStatic();
	BOOL Init();
	BOOL GetDataFromDB();
	void UpdateData(BOOL bIsSave = TRUE);

	inline void SetDataType(LONG type)
	{
		m_nDataType = type;
	}
	inline LONG  GetDataType()
	{
		return m_nDataType;
	}
	inline void SetOffset(LONG offset)
	{
		m_nDataOffset = offset;
	}
	inline LONG GetOffset()
	{
		return m_nDataOffset;
	}

	inline void SetDataSource(LONG type, LONG offset)
	{
		m_nDataType = type;
		m_nDataOffset = offset;
	}
protected:
	LONG m_nDataType; //��ʾ��������������
	LONG m_nDataOffset; //��ʾ�����������ݱ��еı���
	LONG m_nTimer; //ˢ��ʱ�ӣ� ��ˢ��Ϊ-1
};


/**********************/

#endif /* __HmiDataStatic_H */
