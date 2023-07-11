/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiDBI.h
* 
*   ���ģ�飺//˫��ң��
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
*		kangqian	2011-3-30		��ʼ����
* 
*************************************************************************/

#ifndef __HmiDBI_H
#define __HmiDBI_H

#define  BI_TYPE_DPI 2 //˫��
#define  BI_TYPE_SPI 1 //����

#define  DBI_VALUE_ON 2
#define  DBI_VALUE_OFF 1
#define  DBI_VALUE_ERR 3
#include "Paradef.h"

#include "HmiStatic.h"


class CHmiDoubleBI : public CHmiStatic  
{
public:
	CHmiDoubleBI();

	virtual ~CHmiDoubleBI();
	void OnPaint();

/*	BOOL Init(WORD x,WORD y, WORD OnId, WORD OffId, 
		unsigned char* bmpOff, unsigned char* bmpOn, unsigned char* bmpErr,
		WORD bmpWidth, WORD bmpHeight, WORD bmpBPL);*/
	BOOL GetDataFromDB();
	void UpdateData(BOOL bIsSave = TRUE);

	void SetDBIBitmap(unsigned char* Onbmp,unsigned char* Offbmp, 
					LONG Width, LONG Height, LONG bytePerLine,
					LONG nOnId,LONG nType = 1,LONG nOffId = -1, 
					unsigned char* Errbmp = NULL);
	

protected:
	LONG m_nOnId; //��λ��Ӧ��ң��
	LONG m_nOffId; //��λ��Ӧ��ң��

	LONG m_nType; //��˫��ң�Ż��ǵ���ң�� 1���㣬 2˫��

	WORD m_wValue; //��ǰֵ�� ��λΪ1�� ��λΪ2�� ����λΪ3

	unsigned char* m_OnbmData; //��λλͼ����
	unsigned char* m_OffbmData; //��λλͼ����
	unsigned char* m_ErrbmData; //��λλͼ����

	LONG m_nTimer; //ˢ��ʱ�ӣ� ��ˢ��Ϊ-1

};

/**********************/
#endif /* __HmiDBI_H */
