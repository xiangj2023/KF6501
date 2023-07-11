/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiDataList.h
* 
*   ���ģ�飺Һ������ 
*			CHmiDatalist �����б�ؼ� ��4������������ʾ����
*			
* 
*	����	CHmiDatalist �����б�ؼ��� ���������Զ������ݿ��ȡ����ʾ����
*			�����б���������
*			1)����ʾ���޸ĵ�  ֱ����ʾ�����ݿ��ж�ȡ��ֵ�� һ��ֻ��ȡ��ǰ��ʾ�ļ�����¼
*			2)��Ҫ�����޸ĵ�  �ʵ���ʱ����ȡ��buf�У� ��ʾʱ��ʾbuf�м�¼���޸ĵ����ݴ���buf	

*	
*   ����
*		CSOEDetailDlg soe������ʾ�Ի���
*		ʹ�÷���1)init 
*				2)ReadReport(LONG nItem,LONG DispIndex),���� ��ȡ��soe�������ţ� ��ʾ���
*				3)DoModal(CHmiWnd* pWndReturn)���� �س����ش���ָ��
*
*		COperationDetailDlg ��������������ʾ�Ի���
*		ʹ�÷���1)init 
*				2)ReadReport(LONG nItem,LONG DispIndex),����Ϊ��ȡ�Ĳ�����¼�����,��ʾ���
*				3)DoModal(CHmiWnd* pWndReturn)���� �س����ش���ָ��
*
*		CHmiAlarmDetailDlg �澯����������ʾ�Ի���
*		ʹ�÷���1)init 
*				2)ReadReport(LONG nItem,LONG DispIndex),����Ϊ��ȡ�ĸ澯��������,��ʾ���
*				3)DoModal(CHmiWnd* pWndReturn)���� �س����ش���ָ��
*
*		CHmiStartDetailDlg ��������������ʾ�Ի���
*		ʹ�÷���1)init 
*				2)ReadReport(LONG ItemIndex, LONG subItemIndex,LONG DispIndex),���������������,�ֱ����,��ʾ���
*				3)DoModal(CHmiWnd* pWndReturn)���� �س����ش���ָ��
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-12-16		��ʼ����
* 
*************************************************************************/

#ifndef __HmiDataList_H
#define __HmiDataList_H

//////////////////////////////////////////////////////////////////////
#include "HmiWnd.h"
#include "HmiButton.h"
#include "HmiEdit.h"
#include "HmiList.h"
#include "HmiStatic.h"
#include "HmiDataStatic.h"
#include "Translator.h"


//��ֵ�����ö�ֵ�����б�������
#define MAX_DISP_COUNT 32

//���ֵ��·��
#define INTERGRAL_KWH_COUNT 2


//��������ʱ�����Ŀ��,�����ַ�
#define  WIDTH_CF 6 //cf�ַ����
#define  WIDTH_DI_FILTERTIME 5 //diȥ��ʱ��
#define  WIDTH_CI_BASE 5 //��ȵ���
#define  WIDTH_CI_CONST 5 //������峣��

#define MAX_FDNUM 48  //�����ϵ�������

//�����б�֧�ֵ���ʾ��������
enum EListType
{ 
	Action = 1, //1//��������
	Alarm, //2//�澯����
	Start, //3//��������
	Operation, //4//��������
	Soe, //5soe
	ListCI, //���
	HAI, //HAi
	LAI, //LAI
	ListBI, //9 

	ConfigQuery, //10���ò�ѯ
	SettingQuery, //11��ֵ��ѯ
	ConfigChange, //12
	SoftSwitch, //13��ѹ��
	ASQuery, // 14 ���ö�ֵ��ѯ
	SysStatus, //15 ϵͳ״̬
	DIPara, //16�������ԣ���Ȼ�ң��
	DIFilterTime, //17ȥ��ʱ��
	DINor, //18�Ƿ�ȡ��
	CIBase, //19��ȵ���
	CIConst, //20���峣��
	SoePara, //21soe���ԣ��Ƿ��¼
	CF, //ϵ��
	BOTime,//ң�ر���ʱ��
	PortPara //�˿ڹ�Լ
};




//�����б�ؼ�
class CHmiDataList : public CHmiList  
{
public:
	
	DECLARE_HMI_TR()
	BOOL GetDataFromDB(); //��ȡҪ��ʾ������
	CHmiDataList();
	CHmiDataList(LONG dataType);
	void UpdateData(BOOL bIsSave=FALSE);
	LONG GetItemBase();

	virtual ~CHmiDataList();
//	BOOL OnEnter();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnLeft();
	BOOL OnRight();
	BOOL OnAdd();
	BOOL OnSub();


	inline void SetType(LONG nType)
	{
		m_nDataType = nType;
	}
	inline LONG GetType()
	{
		return m_nDataType;
	}
	inline void SetAreaNo(LONG nAreaNoToShow)
	{
		m_nAreaNo = nAreaNoToShow;
	}
	inline LONG	GetAreaNo()
	{
		return m_nAreaNo;
	}

	BOOL Init();

private:

	LONG m_nDataType; //�б���ʾ����������,����ȡֵ�ú궨�壬�͹�������еĶ��屣��һ��
	LONG m_nRecordPerLine; //��Ԥ�����ܣ�Ŀǰֻ��ȡֵ1�� ÿ����ʾ������¼��Ϊ����һ���ڶ���ʾ���ݣ�
						//������ѡ������Ŀ���һ����ʾ�����¼
						
	LONG m_nAreaNo; //��ǰ��ʾ�Ķ�ֵ����
	LONG m_nAllArea; //��ֵ����
	LONG m_nRunAreaNo; //���ж�ֵ����
	LONG m_nDataPageNum; //��ǰ��ʾ�������ݼ�¼�ĵڼ�ҳ
	LONG m_nAllItemNum; //���Լ�¼�ĸ��������ǵ�ǰ��ʾ�ģ�������Ҫ��ʾ�ģ�

protected:
	
	BOOL ChgDIFilterTimeBuf(int nOffset); //�޸Ŀ���ȥ��ʱ�仺����
	BOOL ChgBINOBuf(int nOffset); //�޸�ң��ȡ��������
	BOOL ChgBISOEParaBuf(int nOffset); //�޸�BI��wControl-D1�Ƿ����soe
	BOOL ChgCFBufValue(LONG nOffset); //����ϵ��������
	BOOL ChgCIBaseBufValue(LONG nOffset); //���ĵ�ȵ���
	BOOL ChgCIConstBufValue(LONG nOffset); //�������峣��
	BOOL ChgSWValue(); //������ѹ��ֵ
	BOOL ChgCfgValue();//��������ֵ
	BOOL ChgBOTime(LONG nOffset);//����ң�ر���ʱ��
	BOOL ChgDIPara_wClassBuf(LONG nOffset);//����DI����
	BOOL ChgPortPara(LONG nOffset);

	void ChgQueryAera(BOOL isAdd = TRUE);

	//�澯�������� ������ soe�� ��������
	BOOL ShowAlarmReportDB();
	BOOL ShowActionReportDB();
	BOOL ShowStartReportDB(); //����
	BOOL ShowSOEReportDB();
	BOOL ShowOperationReportDB();


	BOOL ShowBIValue(void);
	BOOL ShowSettingQueryDB(LONG nAreaNo); //��ʾ���ݿ��еĶ�ֵ
	BOOL ShowSysSettingQueryDB(); //��ʾ���ݿ��еĹ��ö�ֵ
	BOOL ShowCIConstBuf();
	BOOL ShowBINorBuf();
	BOOL ShowDIPara_wFilterTimeBuf(); //��ʾ�޸Ļ��������ֵ
	BOOL ShowDIPara_wClassBuf(); //��ʾ�޸Ļ��������ֵ��DIWClass)
	BOOL ShowSOEParaBuf(); //BI�Ƿ������ʱ���SOE wControl��D1
	BOOL ShowSoftSwitchBuf(); //��ʾѹ�建����
	BOOL ShowConfigDB(); //��ѯ��ʱ��ֱ����ʾ���ݿ��ڵ�
	BOOL ShowConfigBuf();
//	void DispSoeDB(); //��ʾ���ݿ�Soe��¼
	BOOL ShowCIValue();
	BOOL ShowCIBaseBuf(); //��ʾci��������ֵ
	BOOL ShowCFValueBuf(); //��ʾϵ��
	BOOL ShowBOLastTimeBuf();
	BOOL ShowPortPara();
};

#endif /* __HmiDataList_H */
