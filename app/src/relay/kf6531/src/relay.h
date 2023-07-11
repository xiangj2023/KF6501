
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			24 				//���Լ�Ľṹ��ĸ���

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:
		LONG 	*dwSampBuf;     	//�������㻺���� 
		LONG 	*dwRecBuf;			//�ϳɲ��������ݻ����������ںϳ����ݵ�¼�� 		
		LONG	*dwPickDataBuf;		//ͻ�����������������ݻ�����
		LONG	*dwComposeBuf;
		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 
		DWORD	dwChanDataMask;		//����ͨ����������
		DWORD	dwDCDataMask;		//������ͨ����������
		DWORD	dwComposeDataMask;		//�ϳ�ͨ��ILA,ILB,IJX������ͨ����������
	
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;		
		WORD	m_wPickCheck;					//���α�������������ͻ�������ڶ�ֵ������
		WORD	m_wPickCheckNext[7];			//���α�������������ͻ�������ڶ�ֵ������
		LONG	m_lPickUpValueArray[7];			//���α�������������ͻ�������ڶ�ֵ������

		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ��������

		LONG 					IAP;				//�ȹ����ɱ�������A,����Чֵ
		LONG 					IBP;				//�ȹ����ɱ�������B,����Чֵ
		LONG 					ICP;				//�ȹ����ɱ�������C,����Чֵ

		TRelElecValPar			U_R50IH;		//�߲������ѹ�����õĵ�ѹ
		TRelElecValPar			U_R27U;			//����ʧѹ�����ж��õĵ�ѹ,Ϊ�ߵ�ѹ��5����ѹ�е����ֵ
		TRelElecValPar			PhaseUHMax;		//�߲����ѹ���ֵ
		TRelElecValPar			PhaseUHMin;		//�߲����ѹ��Сֵ
		TRelElecValPar			PhaseULMin;		//�Ͳ��ѹ��Сֵ
		TRelElecValPar			PhaseULMax;		//�Ͳ��ѹ��Сֵ
		TRelElecValPar			LineULAB;		//�Ͳ��ߵ�ѹ
		TRelElecValPar			I_R27U;			//�߲�ʧѹ�������������õĵ���
	
		TRelElecValPar			LineUHAB;		//AB�ߵ�ѹ
		TRelElecValPar			LineUHBC;		//BC�ߵ�ѹ
		TRelElecValPar			LineUHCA;		//CA�ߵ�ѹ
		TRelElecValPar			LineUHMax;		//�ߵ�ѹ���ֵ
		TRelElecValPar			UHMin;			//�ߵ�ѹ��Сֵ
		TRelElecValPar			ULmin1;
		TRelElecValPar			ULmin2;
		TRelElecValPar			UHA;
		TRelElecValPar			UHB;
		TRelElecValPar			UHC;
		TRelElecValPar			U0;
		TRelElecValPar			ULA;
		TRelElecValPar			ULB;		
		TRelElecValPar			IHA;
		TRelElecValPar			IHB;
		TRelElecValPar			IHC;
		TRelElecValPar			I0;
		TRelElecValPar			IL1;
		TRelElecValPar			IL2;
		TRelElecValPar			IL3;
		TRelElecValPar			IL4;
		TRelElecValPar			ILA;
		TRelElecValPar			ILB;
		TRelElecValPar			IJX;

		
		
		TRelGeneOvValPar 		Rel50I_IA;  		//��ѹ��A�����
		TRelGeneOvValPar	 	Rel50I_IB; 			//��ѹ��B�����			
		TRelGeneOvValPar 		Rel50I_IC;			//��ѹ��C�����			
		TRelGeneOvValPar 		Rel50I_Ia1;			//��ѹ��a�����I��	
		TRelGeneOvValPar 		Rel50I_Ia2;			//��ѹ��a�����II��			
		TRelGeneOvValPar 		Rel50I_Ib1;			//��ѹ��b�����I��
		TRelGeneOvValPar 		Rel50I_Ib2;			//��ѹ��b�����II��
		TRelGeneOvValPar_2		Rel50I_JX;			//��϶����
		TThermalOverLoad		Rel49I_IA;			//��ʱ���ȹ����ɱ���
		TThermalOverLoad		Rel49I_IB;			//��ʱ���ȹ����ɱ���
		TThermalOverLoad		Rel49I_IC;			//��ʱ���ȹ����ɱ���
		TRelGeneOvValPar		Rel50IA1;			//A������1
		TRelGeneOvValPar		Rel50IA2;			//A������2
		TRelGeneOvValPar		Rel50IB1;			//B������1
		TRelGeneOvValPar		Rel50IB2;			//B������2
		TRelGeneOvValPar		Rel50IC1;			//C������1
		TRelGeneOvValPar		Rel50IC2;			//C������2
		TRelGeneOvValPar		Rel50I0;			//�����������
		TRelGeneOvValPar		Rel59U0;			//�����ѹ����
		TRelGeneUnValPar 		Rel27U;				//�͵�ѹ����Ԫ��
		TRelPTBrokPar			Rel74PTH;			//�߲�PT����
		TRelPTBrokPar			Rel74PTLA;			//�Ͳ�A��PT����
		TRelPTBrokPar			Rel74PTLB;			//�Ͳ�B��PT����

		
		BOOL				m_bTripFailFlag;				//��բʧ�ܱ�־ 
		BOOL 				m_bFaultOnFlag;				//�������ѭ�������־ 
		BOOL				m_bFaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				m_bHaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		BOOL				m_bIsLogicOk;				//�߼����̳�ʼ����־
		BOOL 				m_bHaveMakeWarnRpt;			//�ȹ����ɱ��������澯�����־
		BOOL 				m_bHaveMakeActRpt;			//�ȹ����ɱ����������������־
		BOOL 				m_bHaveMakeRSTRpt;			//�ȹ����ɱ����������鱨���־

		BOOL 				m_b52BFLAFlag;				//�Ͳ�A��·����բʧ�ܱ�־
		BOOL 				m_b52BFLBFlag;				//�Ͳ�B��·����բʧ�ܱ�־
		BOOL 				m_b52BFHFlag;
		BOOL				m_bHaveMake52BF_H_RptFlag; 	//������ѹ���·����բʧ�ܱ���		
		BOOL				m_bHaveMake52BF_LA_RptFlag; 	//������ѹ���·����բʧ�ܱ���
		BOOL				m_bHaveMake52BF_LB_RptFlag; 	//������ѹ���·����բʧ�ܱ���
		BOOL				m_b52BFRecover_H_FLAG;
		BOOL				m_b52BFRecover_LA_FLAG;
		BOOL				m_b52BFRecover_LB_FLAG;
		
		
		DWORD				m_dwFaultSINum;					//���ϴ��������������� 
		DWORD				m_dwFaultLockTime;				//����������ʱ�� 
		TAbsTime 			PickUpTime;						//ģֵ��������ͻ��������ʱ�䶨ʱ�� 

		
		WORD				m_wRptSerialNumOfAct;			//�����������к�
		WORD				m_wRptSerialNumOfStart;		//�����ϵ��������������к�
		WORD				m_wRptSerialNumOf27U;			//��ѹ�����������к�
		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����


private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		void ThermalOverLoad(void);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitRel50I_IA(void);
		void InitRel50I_IB(void);
		void InitRel50I_IC(void);
		void InitRel50I_Ia1(void);
		void InitRel50I_Ia2(void);
		void InitRel50I_Ib1(void);
		void InitRel50I_Ib2(void);
		void InitRel50I_JX(void);
		void InitRel49IHA(void);
		void InitRel49IHB(void);
		void InitRel49IHC(void);
		void InitRel50IA1(void);
		void InitRel50IA2(void);
		void InitRel50IB1(void);
		void InitRel50IB2(void);
		void InitRel50IC1(void);
		void InitRel50IC2(void);
		void InitRel50IN(void);
		void InitRel59UN(void);
		void InitRel27U(void);
		void InitRel74PT(void);
		void InitStructCheck(void);		
		
		void FalutProcess(void);												//����ѭ��������� 
		BOOL RelayPickUpCHK(void);												//ģֵ�����б���� 

		BOOL WriteComposeBuf(void);
		void CaluUI(void);
		void CaluUpIp(void);
		void CaluUIAngle(void);
		void PTCheck();
		void Process52BF();
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//����Ԫ�����鸴�麯�� 

		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������
		TRelElecValPar& MaxElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C);		
		TRelElecValPar& MinElecABC(TRelElecValPar &A,TRelElecValPar &B,TRelElecValPar &C);
		inline LONG MaxAB(LONG par1,LONG par2)
		{
			return (par1>=par2)? par1:par2;
		};


};		

void RelayTask(DWORD *pdwApp);


#endif
