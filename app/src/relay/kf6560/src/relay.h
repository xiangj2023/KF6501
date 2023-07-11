
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			13 				//���Լ�Ľṹ��ĸ���

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
		
public:
		LONG			*dwSampBuf;			//�������ݻ�����
		LONG			*dwPickDataBuf;			//ͻ����������ʷ�������ݻ�����
		LONG			*dwComposeBuf;
		LONG			*dwRECBuf;
		BYTE			RelCFGCLR;				//���������˳�
		BYTE			RelCFGSET;				//���������˳�

		DWORD			dwChanDataMask;		//������ȡ�������ݵ�ͨ������
		DWORD			dwDCDataMask;
		DWORD			dwComposeMask;

		LONG    lSampData1;
		LONG    lSampData2;
		LONG    lSampDate3;
		LONG 	lPickValue;
		LONG	m_lPickUpValue;
		WORD	PickCheck;
		WORD	PickCheckNext;
		
		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ

		
		TRelElecValPar		UP;	//������ѹ
		TRelElecValPar		U1;	//��ѹ1
		TRelElecValPar		U2;	//��ѹ2
		TRelElecValPar		Ud1;	//��ѹ2
		TRelElecValPar		Ud2;	//��ѹ2
		TRelElecValPar		Ud3;	//��ѹ3
		TRelElecValPar		IP;	//��������
		TRelElecValPar		I1;	//����1
		TRelElecValPar		I2;	//����2
		TRelElecValPar		I3;	//����3
		TRelElecValPar		IP2;
		TRelElecValPar		IP3; //����г��
		TRelElecValPar		IP5; //���г��
		TRelElecValPar		IP7; //�ߴ�г��
		TRelElecValPar		IPH; //г�����������õĺϳ�г������
		QWORD				m_IPH;
		
		

		 TRelGeneOvValPar  		Rel50I0;			//�ٶϱ���Ԫ��
		 TRelGeneOvValPar		Rel50I1;			//��������Ԫ��
		 TRelGeneOvValPar		Rel59U;				//��ѹ����Ԫ��
		 TRelGeneOvValPar		Rel50IH;			//г����������Ԫ��
		 TRelGeneOvValPar		Rel60I1;			//����1����Ԫ��
		 TRelGeneOvValPar		Rel60I2;			//����2����Ԫ��
		 TRelGeneOvValPar		Rel50ST;			//֧·��������Ԫ��
		 TRelGeneOvValPar		Rel60Ud1;			//��ѹ1����Ԫ��
		 TRelGeneOvValPar		Rel60Ud2;			//��ѹ2����Ԫ��
		 TRelGeneOvValPar		Rel60Ud3;			//��ѹ3����Ԫ��
		 TRelGeneUnValPar		Rel27U;				//��ѹ����Ԫ��
		 TRelPTBrokPar			RelPT;


		BOOL				m_b52BFFlag;					//��բʧ�ܱ�־ 
		BOOL 				FaultOnFlag;					//�������ѭ�������־ 
		BOOL				FaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				HaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������		
//		BOOL				m_bNextRelayStart;			//���α��������ж��п��ܷ����ٴζ�����־
		BOOL				m_bHaveRelayAct;			//���α��������ж��б�������������־	
		
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		BOOL				m_bHaveMakeTripFailRpt;
		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		WORD 				PickUpRW;					//ģֵ����ͻ���������̵����� 
		TAbsTime 			PickUpTime;					//ģֵ��������ͻ��������ʱ�䶨ʱ�� 


		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;			//�����ϵ��������������к�
		WORD				RptSerialNumOf27U;			//��ѹ�����������к�
		BOOL				m_bIsLogicOk;
		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����


private:

public:
		CRelayTask(DWORD *pdwApp);

		void OnSampleEnd(DWORD arg);
		void WriteCompseData(void);
		void OnTimeOut(DWORD id);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
	    void InitRel50I0(void);		//��ʼ���ٶϱ���Ԫ��
		void InitRel50I1(void);		//��ʼ����������Ԫ��
	    void InitRel59U(void);		//��ʼ����ѹ����Ԫ��
	    void InitRel50IH(void);		//��ʼ��г����������Ԫ��
	    void InitRel60I1(void);		//��ʼ������1����Ԫ��
	    void InitRel60I2(void);		//��ʼ������2����Ԫ��
	    void InitRel50ST(void);		//��ʼ��֧·��������Ԫ��
	    void InitRel60Ud1(void);		//��ʼ����ѹ1����Ԫ��
	    void InitRel60Ud2(void);		//��ʼ����ѹ2����Ԫ��
	    void InitRel60Ud3(void);		//��ʼ����ѹ3����Ԫ��
	    void InitRel27U(void);		//��ʼ����ѹ����Ԫ��
	    void InitRelPT(void);		//��ʼ��PT���߱���Ԫ��
		void InitStructCheck(void);

		void FalutProcess(void);												//����ѭ��������� 
		BOOL RelayPickUpCHK(void);										//ģֵ�����б���� 
		void CaluUI(void);
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//����Ԫ�����鸴�麯��
		void Process52BF();

		
		void InitDCRecInfo();	//��ʼ��¼������
		void WriteDCChannel();	//д������¼������		
};		

void RelayTask(DWORD *pdwApp);


#endif
