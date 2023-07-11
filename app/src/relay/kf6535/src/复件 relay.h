
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"


#define CHECK_NUM			18 				//���Լ�Ľṹ��ĸ���
#define PHASE_A 100							//��ѹ��A��Ϊ��������࣬���ڹ��ʷ����ж�,���ڼ����ƶ�����ʱIR1Ϊ����࣬���ڼ����ƶ�����2��ֵ��
#define PHASE_B 200
#define PHASE_C 300
#define	R87IALARMTIME		16000/RELAY_SAMP_INTERVAL  //����Խ�޸澯ʱ�� = 10s/RELAY_SAMP_INTERVAL*625(΢��)

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		const LONG TimeSet;			//��ٶϵı���ʱ�䶨ֵ��Ϊ0
		
		LONG 	*dwSampBuf;     	//�������㻺���� 
		LONG 	*dwRecBuf;			//�ϳɲ��������ݻ����������ںϳ����ݵ�¼�� 		
		LONG	*dwPickDataBuf;		//ͻ�����������������ݻ�����
		LONG	*dwComposeBuf;		//�ϳ�ͨ�����ݻ�����
		LONG	*dwLineUDataBuf;	//�ߵ�ѹ�ϳ�ͨ�����ݻ�����
		LONG    *dwPDDataBuf;		//���ʷ�����仺����
		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 
		DWORD	dwChanDataMask;		//����ͨ����������
		DWORD   dwComposeDataMask;
		DWORD	dwDCDataMask;		//������ͨ����������
		
		LONG	IDADrift[5];		//����A��Ưֵ����������4���ܲ�
		LONG	IDBDrift[5];		//����B��Ưֵ����������4���ܲ�
		LONG	IDCDrift[5];		//����C��Ưֵ����������4���ܲ�
		LONG    lSampData1;
		LONG    lSampData2;
		LONG    lSampDate3;
		LONG 	lPickValue;
		WORD	PickCheck;
		WORD	PickCheckNext[3];
		LONG	lPickUpValueArray[3];

		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ

		//--------------------����ϳ�ͨ��ʱϵ������������λ-----------------
		LONG	CoefCorH[9];		//��ѹ������������ƶ�����ʱ����У��ϵ��		
		LONG	CoefCorM[9];		//��ѹ������������ƶ�����ʱ����У��ϵ��
		LONG	CoefCorL[9];		//��ѹ������������ƶ�����ʱ����У��ϵ��

		TRelElecValPar			UA;		
		TRelElecValPar			UB;
		TRelElecValPar			UC;
		TRelElecValPar			UAB;		
		TRelElecValPar			UBC;
		TRelElecValPar			UCA;
		TRelElecValPar			U2H;		//��ѹ�ฺ���ѹ
		TRelElecValPar			UHmax_ph; 			//ʧѹ�����õĵ�ѹ��ȡ���ѹ���ֵ
		TRelElecValPar			UHmin_li; 			//���ڸ߹�����ѹ������ȡ�ߵ�ѹ��Сֵ

		TRelElecValPar			IHA;			//�߲�A�����
		TRelElecValPar			IHB;			//�߲�B�����
		TRelElecValPar			IHC;			//�߲�C�����
		TRelElecValPar			IMA;			//�в�A�����
		TRelElecValPar			IMB;			//�в�B�����
		TRelElecValPar			IMC;			//�в�C�����
		TRelElecValPar			ILA;			//�Ͳ�A�����
		TRelElecValPar			ILB;			//�Ͳ�B�����
		TRelElecValPar			ILC;			//�Ͳ�C�����
		
		TRelElecValPar			IHmax; 			//��ѹ����������
		TRelElecValPar			IMmax; 			//��ѹ����������
		TRelElecValPar			ILmax; 			//��ѹ����������
		WORD					wHIPhase;       //����жϣ�������ѹ���������Ԫ����ʹ�õ����

		TRelElecValPar			IRA_1;			//A���ƶ������м�ֵ1
		TRelElecValPar			IRA_2;			//A���ƶ������м�ֵ2
		TRelElecValPar			IRA_3;     		//A���ƶ������м�ֵ3
		TRelElecValPar			IRB_1;
		TRelElecValPar			IRB_2;
		TRelElecValPar			IRB_3;
		TRelElecValPar			IRC_1;
		TRelElecValPar			IRC_2;
		TRelElecValPar			IRC_3;
		WORD					wIRAPhase;		//ȡ����ƶ����������ڼ����ƶ���������г��
		WORD					wIRBPhase;		//
		WORD					wIRCPhase;		//

		TRelElecValPar			IDA;			//A�����
		TRelElecValPar			IDB;			//B�����
		TRelElecValPar			IDC;			//C�����
		TRelElecValPar			IRA;			//A���ƶ�����
		TRelElecValPar			IRB;			//B���ƶ�����
		TRelElecValPar			IRC;     		//C���ƶ�����
		TRelElecValPar			IDA2;
		TRelElecValPar			IDA3;
		TRelElecValPar			IDA5;
		TRelElecValPar			IRA2;		
		TRelElecValPar			IDB2;
		TRelElecValPar			IDB3;
		TRelElecValPar			IDB5;
		TRelElecValPar			IRB2;
		TRelElecValPar			IDC2;
		TRelElecValPar			IDC3;
		TRelElecValPar			IDC5;
		TRelElecValPar			IRC2;

		LONG					A_87RPI;		//�����ڷ������ƺ�Ĳ�����ż�ֵ
		LONG					B_87RPI;		//�����ڷ������ƺ�Ĳ�����ż�ֵ
		LONG					C_87RPI;		//�����ڷ������ƺ�Ĳ�����ż�ֵ
		LONG					IDAH2;			//�����ڷ������ƺ��A���������г��
		LONG 					IDBH2;			//�����ڷ������ƺ��B���������г��
		LONG					IDCH2;			//�����ڷ������ƺ��C���������г��
		TRelGeneOvValPar 		R87U_A;  		//A���ٶ�
		TRelGeneOvValPar	 	R87U_B; 		//B���ٶ�
		TRelGeneOvValPar 		R87U_C;			//C���ٶ�
		TRel87R					R87R_A;			//A����ʲ
		TRel87R					R87R_B;			//B����ʲ
		TRel87R					R87R_C;			//C����ʲ
		TRelGeneOvValPar 		R50IH1;  		//��ѹ��1�ι���
		TRelGeneOvValPar	 	R50IH2; 		//��ѹ��2�ι���
		TRelGeneOvValPar 		R50IM1;			//��ѹ��1�ι���
		TRelGeneOvValPar 		R50IM2;			//��ѹ��2�ι���
		TRelGeneOvValPar 		R50IL1;			//��ѹ��1�ι���
		TRelGeneOvValPar 		R50IL2;			//��ѹ��2�ι���
		TRelGeneOvValPar 		R50OL1;			//������1��
		TRelGeneOvValPar 		R50OL2;			//������2��
		TRelGeneOvValPar 		R50OL3;			//������3��
		TRelGeneUnValPar		Rel27U;			//ʧѹ����
		T87IAlarm				R87Alarm;		//����Խ�ޱ���Ԫ��


		BOOL					HA_PDWithMemoryFlag;	//A�๦�ʷ�������־
		BOOL					HB_PDWithMemoryFlag;
		BOOL					HC_PDWithMemoryFlag;

		BOOL 					b68IdH2Flag_A;	//A��������α�����־	
		BOOL 					b68IdH2Flag_B;	//B��������α�����־	
		BOOL 					b68IdH2Flag_C;	//C��������α�����־	
		BOOL 					b68IdH5Flag_A;	//A�����5�α�����־	
		BOOL 					b68IdH5Flag_B;	//B�����5�α�����־	
		BOOL 					b68IdH5Flag_C;	//C�����5�α�����־			
		BOOL 					b68IrH2Flag_A;	//A���ƶ�����2�α�����־	
		BOOL 					b68IrH2Flag_B;	//B���ƶ�����2�α�����־	
		BOOL 					b68IrH2Flag_C;	//C���ƶ�����2�α�����־
		BOOL 					b68IdHMulFlag_A; //A���ۺ�г��������־
		BOOL 					b68IdHMulFlag_B;//B���ۺ�г��������־
		BOOL 					b68IdHMulFlag_C;//C���ۺ�г��������־

		BOOL 				FaultOnFlag;				//�������ѭ�������־ 
		BOOL				FaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				HaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		BOOL				m_bIsLogicOk;		

		BOOL				m_bHaveTripFailedRpt_H;		//������ѹ���·����բʧ�ܱ���
		BOOL				m_bHaveTripFailedRpt_M;		//������ѹ���·����բʧ�ܱ���
		BOOL				m_bHaveTripFailedRpt_L;		//������ѹ���·����բʧ�ܱ���
		BOOL				m_b52BFRecoverFLAG_H;		//��ѹ���·����բʧ�ָܻ���־
		BOOL				m_b52BFRecoverFLAG_M;		//��ѹ���·����բʧ�ָܻ���־
		BOOL				m_b52BFRecoverFLAG_L;		//��ѹ���·����բʧ�ָܻ���־
		BOOL 				m_b52BFHFlag;				//�Ѽ���߲��·����բʧ�ܱ�־
		BOOL 				m_b52BFMFlag;				//�Ѽ���в��·����բʧ�ܱ�־
		BOOL 				m_b52BFLFlag;				//�Ѽ���Ͳ��·����բʧ�ܱ�־
		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		WORD 				PickUpRW;					//ģֵ����ͻ���������̵����� 
		TAbsTime 			PickUpTime;					//ģֵ��������ͻ��������ʱ�䶨ʱ�� 

		
		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;		//�����ϵ��������������к�

		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����
		//------------PT���߼�ⲿ��--------------
		LONG				U2Set;				//PT���߼�⸺���ѹ
		DWORD				HPTCheckTimer1;		
		DWORD				HPTCheckTimer2;
		DWORD				HPTCheckTimer3;
		DWORD				HPTCheckTimer4;
		BOOL				HPTMakeReportFlag;
		DWORD 				m_PTCheckTimer;  


private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitR87U_A(void);
		void InitR87U_B(void);
		void InitR87U_C(void);
		void InitR87R_A(void);
		void InitR87R_B(void);
		void InitR87R_C(void);
		void InitR50IH1(void);
		void InitR50IH2(void);
		void InitR50IM1(void);
		void InitR50IM2(void);
		void InitR50IL1(void);
		void InitR50IL2(void);
		void InitR50OL1(void);
		void InitR50OL2(void);
		void InitR50OL3(void);
		void InitR27U(void);
		void InitR87IAlarm(void);
		void InitStructCheck(void);		
		BOOL WriteComposeBuf();

		void ResetIdHLock();
		void R68IdH2M(void);		//����г����������
		void R68IdH2C(void);		//����г���ۺ������
		void R68IdH2I(void);		//����г���������
		void R68IrH2M(void);		//�ƶ���������г������
		void R68IdH5(void);			//���г������
		void R68IdHI(void);			//�ۺ�г������
		void R68IdAped(void);		//�����ڷ�������
		
		
		void Process52BF(void);
		void FalutProcess(void);												//����ѭ��������� 
		BOOL RelayPickUpCHK(void);												//ģֵ�����б���� 
		
		void CaluUI(void);
		void CaluMaxMin(void);
		void CaluR87(void);
		void CaluDrift(void);
		void PTCheck(void);//PT�����ж� 
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//����Ԫ�����鸴�麯�� 

		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������
		
		TRelElecValPar CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc); //���㸺���ѹ
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);		
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase);
		TRelElecValPar ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);
		inline LONG MaxAB(LONG par1,LONG par2)
		{
			return (par1>=par2)? par1:par2;
		};
		
		inline LONG MaxABC(LONG x,LONG y,LONG z)
        {
             return (x>y?(x>z?x:z):(y>z?y:z));
        };	

};		

void RelayTask(DWORD *pdwApp);


#endif
