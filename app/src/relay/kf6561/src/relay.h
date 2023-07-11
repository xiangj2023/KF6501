
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#define CHECK_NUM			7 				//���Լ�Ľṹ��ĸ���

#define	RELAY_PHASE_A 0x10
#define	RELAY_PHASE_B 0x11
#define	RELAY_PHASE_C 0x12

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		LONG 	*dwSampBuf;     	//�������㻺���� 
		LONG 	*dwComposeBuf;		//�ϳɲ��������ݻ����������ںϳ����ݵĸ��ϼ��� 
		LONG	*dwPickDataBuf;		//ͻ�����������������ݻ�����
		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 

		DWORD	dwChanDataMask;		//����ͨ����������
		DWORD	dwDCDataMask;		//������ͨ����������	

		//ͻ�������㲿��
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;		 //ͻ�������μ���ֵ
		LONG	m_lPickUpValue;  //��һ�ε�ͻ����
		WORD	PickCheck;		 //��������
		WORD	PickCheckNext[5];	 //����������
		
		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ

  
		TRelElecValPar			Ua;
		TRelElecValPar			Ub;
		TRelElecValPar			Uc;
		TRelElecValPar			Uab;
		TRelElecValPar			Ubc;
		TRelElecValPar			Uca;
		TRelElecValPar			U0;
		TRelElecValPar			U1;
		TRelElecValPar			U2;
		TRelElecValPar			U3;
		TRelElecValPar			Ia;
		TRelElecValPar			Ib;
		TRelElecValPar			Ic;
		TRelElecValPar			I0;
		TRelElecValPar			I1;
		TRelElecValPar			Ulmin;		//���ڵ�ѹ�����ĵ�ѹ,Ϊ�����ߵ�ѹ�е���Сֵ
		TRelElecValPar			Ulmax;		//���ڵ�ѹ������Ϊ�����ߵ�ѹ�е����ֵ
		TRelElecValPar			NU2;		//�����ѹ
		TRelElecValPar			Ipmax;		//��������е����ֵ
		TRelElecValPar			U5;
		TRelElecValPar			I5;
		TRelElecValPar			Factor1; //����1
		TRelElecValPar			Factor2;//����2


		TRelGeneOvValPar		R50I0;  //�ٶ�Ԫ��
		TRelGeneOvValPar		R50I1;	//����Ԫ��
		TRelGeneOvValPar		R59U;	//��ѹԪ��
		TRelGeneOvValPar		R60I;	//��ƽ�����Ԫ��
		TRelGeneOvValPar		R60U;	//��ƽ���ѹԪ��
		TRelGeneUnValPar		R27U;	//��ѹ


		
		BOOL				TripFailFlag;				//��բʧ�ܱ�־ 
		BOOL 				FaultOnFlag;				//�������ѭ�������־ 
		BOOL				FaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				HaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		BOOL				m_bIsLogicOk;
		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����
		BOOL 				m_b52BFFLAG;
		
		BOOL 				m_bHaveMakeTripFailRpt;
		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		TAbsTime 			PickUpTime;					//ģֵ��������ͻ��������ʱ�䶨ʱ�� 

		
		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;		//�����ϵ��������������к�
		WORD				RptSerialNumOf27U;			//��ѹ�����������к�

		LONG				m_U2Set;
		DWORD				m_BusPTTimer;
		

public:
		void OnSampleEnd(DWORD ptr, DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitR50I0(void);
		void InitR50I1(void);
		void InitR60I(void);
		void InitR60U(void);
		void InitR59U(void);
		void InitR27U(void);		
		void InitStructCheck(void);
		void PTCheck();
		void R59NAlarm(); //��ѹ�澯
		void WriteComposeBuf();
		void FalutProcess(void);												//����ѭ��������� 
		BOOL RelayPickUpCHK(void);										//ģֵ�����б���� 
		void CaluUI(void);
		void CaluUpIp(void);
		void CaluU2(void);
		void CaluU5I5(void);
		void CaluUIAngle();
		void Process52BF(void);
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//����Ԫ�����鸴�麯�� 
		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);		
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase);
		TRelElecValPar ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);

};		

void RelayTask(DWORD *pdwApp);


#endif
