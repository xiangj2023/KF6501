/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiGraphWnd.h
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
*		kangqian	2011-3-30		��ʼ����
* 
*************************************************************************/

#ifndef __HmiGraphWnd_H
#define __HmiGraphWnd_H
#define MAX_GRAPH_BI 16
#define MAX_GRAPH_AI 16

#include "HmiDataStatic.h"
#include "HmiDBI.h"
#include "HmiWnd.h"

#define  AITYPE_AI 2
#define  AITYPE_RM 1

//��ʾһ�λ��Ƕ���ֵ
#define  AIDISTYPE_PRI 1
#define  AIDISTYPE_SECONDERY 2



#define  BITYPE_NONE  0  //��ң��
#define  BITYPE_SPI	   1 //����
#define  BITYPE_DPI    2 //˫��


typedef struct TGraph_AIDisp
{
	WORD	wX;
	WORD	wY;
	WORD	wAIID;
	BYTE	byDisType; //��ʾ���ͣ� ��ʾ1��ֵ������ʾ����ֵ�� Ĭ����ʾ����
	BYTE	byAIType; //ң�⣬ ��������ֵ
	BYTE	byFont;
	BYTE	rsv;
}TGraph_AIDisp;

typedef struct TGraph_Disp
{
	WORD	wSwitchCount;
	WORD	wAICount;
	WORD	wGraphCount;
	WORD	wBKGraphDBIndex; //�����ڱ���ͼ�����
	WORD	wBKGraphIndex; //����ͼ��ͼƬ���������
	char	szTitle[32];
}TGraph_Disp;
typedef struct TGraph_SwitchDisp
{
	WORD	wX;
	WORD	wY;
	WORD	wBitmapType; //ͼ������, ��ͼ����ܵ����
	WORD	wGraphIndex; //�����ļ���ͼƬ��������ͼƬ���
	WORD	wNegative; //�Ƿ�ȡ��
	BYTE	byInputType; //�������� ��λ��1��˫λ��2�� ����������0
	BYTE	byOutputType; //�������� ����1�� �ֺϿ�2�� ����������0
	WORD	wBIOnID; //��λ�ÿ����
	WORD	wBIOffID; //��λ�ÿ����
	WORD	wBOCloseID; //�Ͽ�����
	WORD	wBOOpenID; //�ֿ�����
	BYTE	byRsv[4];
}TGraph_SwitchDisp;

typedef struct TBMP_Disp
{
	WORD	wDataBase; //	����������ַ		
	WORD	wXSize; //	��		
	WORD	wYSize; //��
	WORD	wBPL; //bytePerLine	
}TBMP_Disp;



class CHmiGraphWnd : public CHmiWnd
{
	CHmiDataStatic m_AI[MAX_GRAPH_AI];
	CHmiDoubleBI m_BI[MAX_GRAPH_BI];
	CHmiDataStatic m_stTime; //��ǰʱ��
	CHmiStatic m_stSetArea; //��ֵ����
	CHmiStatic m_stCrc; //CRCУ����

	CHmiObj* m_pObjList[MAX_GRAPH_AI+MAX_GRAPH_BI+3];
	BOOL InitDlg();
	BOOL Init();
	BOOL CheckFile(BYTE* pFileBuf, DWORD dwFileLen);
	
public:
	CHmiGraphWnd();
	BOOL Load(char* pszGraphDataFile);
	void OnPaint();

	virtual ~CHmiGraphWnd();
	DECLARE_HMIMESSAGE_MAP() 
	//��ֹ�ⲿ����Show����,ȷ��ֻ��ͨ��DoModal����ʾ
private: 
//	void Show(DWORD param1=0, DWORD param2=0){CHmiWnd::Show(param1,param1);};
	WORD m_nAICount;
	WORD m_nBICount;
		
};
/**********************/

#endif /* __HmiGraphWnd_H */
