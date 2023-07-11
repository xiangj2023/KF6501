
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			9 

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:
		LONG 	*dwSampBuf;     									//�������㻺���� 
		LONG 	*dwRecBuf;											//�ϳɲ��������ݻ����������ںϳ����ݵ�¼�� 
		LONG	*dwComposeBuf;		
	
		DWORD	dwChanDataMask;									//����ͨ����������
		DWORD	dwRecDataMask;									//���������������õ�ͨ����������
		DWORD	dwDCDataMask;		//������ͨ����������		
		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ
		
		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 

/*		
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;		
		WORD	m_wPickCheck;					//���α�������������ͻ�������ڶ�ֵ������
		WORD	m_wPickCheckNext[8];			//���α�������������ͻ�������ڶ�ֵ������
		LONG	m_lPickUpValueArray[87];			//���α�������������ͻ�������ڶ�ֵ������
*/		
		TRelElecValPar			U1;
		TRelElecValPar			U2;
		TRelElecValPar			U3;
		TRelElecValPar			U4;
		TRelElecValPar			U5;
		TRelElecValPar			U6;
		TRelElecValPar			U7;
		TRelElecValPar			U8;

		TRelElecValPar			I1;     //IT1
		TRelElecValPar			I2;		//IF1
		TRelElecValPar			I3;		//IP1
		TRelElecValPar			I4;		//IT2
		TRelElecValPar			I5;		//IF2
		TRelElecValPar			I6;		//IP2
		TRelElecValPar			I7;
		TRelElecValPar			I8;
		
		TRelElecValPar			IC1;		//IC1
		TRelElecValPar			IC2;		//IC2

		TRelGeneOvValPar	RelR50IC1;
		TRelGeneOvValPar	RelR50IT1;
		TRelGeneOvValPar	RelR50IF1;
		TRelGeneOvValPar	RelR50IP1;
		TRelGeneOvValPar	RelR50IC2;
		TRelGeneOvValPar	RelR50IT2;
		TRelGeneOvValPar	RelR50IF2;
		TRelGeneOvValPar	RelR50IP2;
	
		BOOL				m_bIsLogicOk;
		BOOL				m_bLastRecStatusFlag;			//�ϴ�¼���̵�����״̬

		
/***********************AT1��ر�����******************************/

		LONG		*dwPickDataBuf_AT1;		 //ͻ�����������������ݻ�����
		WORD    	RptSerialNumOfAct_AT1;   //�����������к�
		WORD    	RptSerialNumOfStart_AT1; //�����������к�		
		TAbsTime 	PickUpTime_AT1;			 //ģֵ��������ͻ��������ʱ�䶨ʱ��
		WORD		PickCheck_AT1;			 //ͻ��������������
		LONG		m_lPickUpValue_AT1[4];	 //���ڵ�ͻ��������ֵ
		WORD		m_wPickCheckNext_AT1[4]; //���α�������������ͻ�������ڶ�ֵ�ļ�����		
		WORD		m_wPickCheck_AT1;		//�����ж�������ͻ�������ڶ�ֵ�ļ�����
		
		LONG		lSampData1_AT1;   		//ͻ����ֵ1
		LONG		lSampData2_AT1;			//ͻ����ֵ2
		LONG		lSampDate3_AT1;			//ͻ����ֵ3
		LONG		lPickValue_AT1;   		//����ͻ����ֵ

		
		BOOL 		FaultOnFlag_AT1;					//�������ѭ�������־ 		
		DWORD		FaultSINum_AT1;					//���ϴ��������������� 		
		BOOL 		HaveMakeStartRptFlag_AT1;		//�����������־ 
		BOOL		FaultCheckOutFlag_AT1;			//����ѭ�����������ϱ�־ 
		DWORD		FaultLockTime_AT1;				//����������ʱ�� 		
		BOOL		m_bFaultLKFlag_AT1;				//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL 		m_b52BFFlag_AT1;
		BOOL        m_bHaveRelayAct_AT1;				//���α��������ж��б�������������־
		BOOL        m_bHaveMake52BFRpt_AT1;				//��·��ʧ�ܱ���������
/***********************AT2��ر�����******************************/

		LONG		*dwPickDataBuf_AT2; 	 //ͻ�����������������ݻ�����
		WORD		RptSerialNumOfAct_AT2;	 //�����������к�
		WORD		RptSerialNumOfStart_AT2; //�����������к�		
		TAbsTime	PickUpTime_AT2; 		 //ģֵ��������ͻ��������ʱ�䶨ʱ��
		WORD		PickCheck_AT2;			 //ͻ��������������
		LONG		m_lPickUpValue_AT2[4];	 //���ڵ�ͻ��������ֵ
		WORD		m_wPickCheckNext_AT2[4]; //���α�������������ͻ�������ڶ�ֵ�ļ����� 	
		WORD		m_wPickCheck_AT2;		//�����ж�������ͻ�������ڶ�ֵ�ļ�����
		
		LONG		lSampData1_AT2; 		//ͻ����ֵ1
		LONG		lSampData2_AT2; 		//ͻ����ֵ2
		LONG		lSampDate3_AT2; 		//ͻ����ֵ3
		LONG		lPickValue_AT2; 		//����ͻ����ֵ

		
		BOOL		FaultOnFlag_AT2;					//�������ѭ�������־		
		DWORD		FaultSINum_AT2; 				//���ϴ���������������		
		BOOL		HaveMakeStartRptFlag_AT2;		//�����������־ 
		BOOL		FaultCheckOutFlag_AT2;			//����ѭ�����������ϱ�־ 
		DWORD		FaultLockTime_AT2;				//����������ʱ��		
		BOOL		m_bFaultLKFlag_AT2; 			//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL		m_b52BFFlag_AT2;
		
		BOOL        m_bHaveMake52BFRpt_AT2;
		BOOL		m_bHaveRelayAct_AT2;

		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����



		
private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void WriteComposeBuf(DWORD dwMask);
		
		void InitRelR50IC1(void);
		void InitRelR50IT1(void);
		void InitRelR50IF1(void);
		void InitRelR50IP1(void);
		void InitRelR50IC2(void);
		void InitRelR50IT2(void);
		void InitRelR50IF2(void);
		void InitRelR50IP2(void);
		
		void InitStructCheck(void);
		void CaluUI(void);

		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		void OnLogicChanged(void);

		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������

		
/************AT1��������***************************************************************/		
		void ResetAllRelay_AT1(BOOL resetFlag = FALSE);		
		void FalutProcess_AT1(void);//����ѭ���������		
		BOOL RelayPickUpCHK_AT1(void);
		void Process52BF_AT1();
		
/************AT1��������***************************************************************/		
		void ResetAllRelay_AT2(BOOL resetFlag = FALSE);		
		void FalutProcess_AT2(void);//����ѭ���������		
		BOOL RelayPickUpCHK_AT2(void);
		void Process52BF_AT2();

};		

void RelayTask(DWORD *pdwApp);


#endif
