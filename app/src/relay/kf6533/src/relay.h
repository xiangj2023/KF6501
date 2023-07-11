
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			26 				//���Լ�Ľṹ��ĸ���
#define PHASE_A 100
#define PHASE_B 200
#define PHASE_C 300
#define	R87IALARMTIME		16000/RELAY_SAMP_INTERVAL  //����Խ�޸澯ʱ�� = 10s/RELAY_SAMP_INTERVAL*625(΢��)

class CRelayTask:public CApp
{
   DECLARE_MESSAGE_MAP()
public:   

		LONG 	*dwSampBuf;     		//�������㻺���� 
		LONG	*dwPickDataBuf;			//ͻ�����������������ݻ�����
		LONG	*dwComposeDataBuf;		//�ϳ����ݻ�����
		LONG	*dwLineUDataBuf;		
		LONG	*dwPDDataBuf;
		LONG	*dwRecBuf;
		
		BYTE  	RelCFGCLR;				//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ 
		BYTE	RelCFGSET;				//���������˳�
		DWORD	dwChanDataMask;			//����ͨ����������
		DWORD	dwDCDataMask;			//������ͨ����������	
		DWORD	dwComposeMask;

		LONG    lSampData1;
		LONG    lSampData2;
		LONG    lSampDate3;
		LONG 	lPickValue;		
		WORD	PickCheck;
		WORD	PickCheckNext[11];		
		TAbsTime 			PickUpTime;		//ģֵ��������ͻ��������ʱ�䶨ʱ�� 
		
		const LONG TimeSet;			//��ٶϵı���ʱ�䶨ֵ��Ϊ0
		BOOL 	b68IdH2Flag_A;	//A��������α�����־	
		BOOL 	b68IdH2Flag_B;	//B��������α�����־	
		BOOL 	b68IdH2Flag_C;	//C��������α�����־	
		BOOL 	b68IdH5Flag_A;	//A�����5�α�����־	
		BOOL 	b68IdH5Flag_B;	//B�����5�α�����־	
		BOOL 	b68IdH5Flag_C;	//C�����5�α�����־			
		BOOL 	b68IrH2Flag_A;	//A���ƶ�����2�α�����־	
		BOOL 	b68IrH2Flag_B;	//B���ƶ�����2�α�����־	
		BOOL 	b68IrH2Flag_C;	//C���ƶ�����2�α�����־
		BOOL 	b68IdHMulFlag_A; //A������ۺ�г��������־
		BOOL 	b68IdHMulFlag_B;	//B������ۺ�г��������־
		BOOL 	b68IdHMulFlag_C;	//C������ۺ�г��������־
		
        WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ



		TRelElecValPar			UA;
		TRelElecValPar			UB;
		TRelElecValPar			UC;
		TRelElecValPar			U0h;
		TRelElecValPar			Ua;
		TRelElecValPar			Ub;
		TRelElecValPar			Uc;
		TRelElecValPar			U0l;
		TRelElecValPar			IA;
		TRelElecValPar			IB;
		TRelElecValPar			IC;
		TRelElecValPar			Ia;
		TRelElecValPar			Ib;
		TRelElecValPar			Ic;
		TRelElecValPar			I0;
		TRelElecValPar			Ijx;
		TRelElecValPar			U2h;		//��ѹ�ฺ���ѹ
		TRelElecValPar			U2l;		//��ѹ�ฺ���ѹ
		TRelElecValPar			UAB;
		TRelElecValPar			UBC;
		TRelElecValPar			UCA;
		TRelElecValPar			Uab;
		TRelElecValPar			Ubc;
		TRelElecValPar			Uca;
		
		TRelElecValPar			Ida;		//A�����
		TRelElecValPar			Idb;
		TRelElecValPar			Idc;
		TRelElecValPar			Ira;		//A���ƶ�����
		TRelElecValPar			Irb;
		TRelElecValPar			Irc;
		TRelElecValPar			Ida2;
		TRelElecValPar			Ida3;
		TRelElecValPar			Ira2;
		TRelElecValPar			Idb2;
		TRelElecValPar			Idb3;
		TRelElecValPar			Irb2;
		TRelElecValPar			Idc2;
		TRelElecValPar			Idc3;
		TRelElecValPar			Irc2;

		TRelElecValPar			IHmax; //��ѹ����������
		TRelElecValPar			ILmax; //��ѹ����������
		TRelElecValPar			UHmax; //��ѹ��������ѹ
		TRelElecValPar			ULmax; //��ѹ��������ѹ
		TRelElecValPar			UHmin;
		TRelElecValPar			ULmin;
		TRelElecValPar			U_27U;  //ʧѹ�����õĵ�ѹ���ڸ�ѹ������ʱΪ��������ѹ����ѹ������ʱΪ������С��ѹ
		
		LONG					dw49IA;
		LONG					dw49IB;
		LONG					dw49IC;
		


		TRelGeneOvValPar		Rel87U_A;		//A��ٶ�
		TRelGeneOvValPar		Rel87U_B;		//B��ٶ�
		TRelGeneOvValPar		Rel87U_C;		//C��ٶ�		
		TRel87R					Rel87R_A;		//A���ʲ
		TRel87R					Rel87R_B;		//B���ʲ
		TRel87R					Rel87R_C;		//C���ʲ		
		T87IAlarm				R87Alarm;		//����Խ�ޱ���Ԫ��
		TRelGeneOvValPar		Rel50I_H1;		//��ѹ�����1�Σ�
		TRelGeneOvValPar		Rel50I_H2;		//��ѹ�����2�Σ�
		TRelGeneOvValPar		Rel50I_H3;		//��ѹ�����3��
		TRelGeneOvValPar		Rel50I_L1;		//��ѹ�����1��
		TRelGeneOvValPar		Rel50I_L2;		//��ѹ�����2��
		TRelGeneOvValPar		Rel50I_SH;		//ĸ�߳�籣��
		TRelGeneOvValPar		Rel50I_N1;		//�������1��
		TRelGeneOvValPar		Rel50I_N2;		//�������2��
		TRelGeneOvValPar		Rel59U_NH;		//��ѹ�������ѹ
		TRelGeneOvValPar		Rel59U_NL;		//��ѹ�������ѹ
		TRelGeneOvValPar		Rel50OL_1;		//������1��
		TRelGeneOvValPar		Rel50OL_2;		//������2��
		TRelGeneOvValPar		Rel50OL_3;		//������3��
		TRelGeneOvValPar_2		Rel50I_JX;		//��϶��������
		TRelGeneUnValPar		Rel27U;			//ʧѹ����
		TThermalOverLoad		Rel49IA;			//��ʱ���ȹ����ɱ���
		TThermalOverLoad		Rel49IB;			//��ʱ���ȹ����ɱ���
		TThermalOverLoad		Rel49IC;			//��ʱ���ȹ����ɱ���

		TRelGeneOvValPar		Rel50OL_A;		//�����ɸ澯
		
		
		BOOL				TripFailFlag;					//��բʧ�ܱ�־ 
		BOOL 				FaultOnFlag;					//�������ѭ�������־ 
		BOOL				FaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				HaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		BOOL				m_bHaveMakeTripFailRpt;

		BOOL				HA_PDWithMemoryFlag;
		BOOL				HB_PDWithMemoryFlag;
		BOOL				HC_PDWithMemoryFlag;
		BOOL				LA_PDWithMemoryFlag;
		BOOL				LB_PDWithMemoryFlag;
		BOOL				LC_PDWithMemoryFlag;


		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		WORD 				PickUpRW;					//ģֵ����ͻ���������̵����� 
		WORD				HPFRW; 						//���ʷ���������̵�����
		WORD				HPRRW; 						//���ʷ��򸺷���̵�����		
		WORD				LPFRW; 						//���ʷ���������̵�����
		WORD				LPRRW; 						//���ʷ��򸺷���̵�����
		WORD				wHIPhase;                     //����жϣ������ߵ�ѹ���������Ԫ����ʹ�õ����
		WORD				wLIPhase;                     //����жϣ������ߵ�ѹ���������Ԫ����ʹ�õ����

		
		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;		//�����ϵ��������������к�
		WORD				RptSerialNumOf27U;			//��ѹ�����������к�
		BOOL				m_bIsLogicOk;

		BOOL				m_b49IHaveMakeWarnRpt; 		//��ʱ���ȹ����ɱ����õ��������־
		BOOL				m_b49IHaveMakeActRpt;			
		BOOL				m_b49IHaveMakeRSTRpt;	
		BOOL				m_b52BFFLAG;




		LONG				U2Set;
		DWORD				HPTCheckTimer1;
		DWORD				HPTCheckTimer2;
		DWORD				HPTCheckTimer3;
		DWORD				HPTCheckTimer4;
		BOOL				HPTMakeReportFlag;

		DWORD				LPTCheckTimer1;
		DWORD				LPTCheckTimer2;
		DWORD				LPTCheckTimer3;
		DWORD				LPTCheckTimer4;
		BOOL				LPTMakeReportFlag;
		BOOL				m_bLogicChanged;
		DWORD				m_dwWaitLogicCounter;
		DWORD 				m_PTCheckTimer;
		
private:


public:
		
	    void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);		
		void InitStructCheck(void);		//��ʼ������Ԫ���ṹУ������

		void InitRel87U_A(void);		//A��ٶ�
		void InitRel87U_B(void);		//B��ٶ�
		void InitRel87U_C(void);		//C��ٶ�		
		void InitRel87R_A(void);		//A���ʲ
		void InitRel87R_B(void);		//B���ʲ
		void InitRel87R_C(void);		//C���ʲ
		void InitRel50I_H1(void);		//��ѹ�����1�Σ�
		void InitRel50I_H2(void);		//��ѹ�����2�Σ�
		void InitRel50I_H3(void);		//��ѹ�����3��
		void InitRel50I_L1(void);		//��ѹ�����1��
		void InitRel50I_L2(void);		//��ѹ�����2��
		void InitRel50I_SH(void);		//ĸ�߳�籣��
		void InitRel50I_N1(void);		//�������1��
		void InitRel50I_N2(void);		//�������2��
		void InitRel59U_NH(void);		//��ѹ�������ѹ
		void InitRel59U_NL(void);		//��ѹ�������ѹ
		void InitRel50OL1(void);		//������1��
		void InitRel50OL2(void);		//������2��
		void InitRel50OL3(void);		//������3��
		void InitRel50IJX(void);		//��϶��������
		void InitRel27U(void);			//ʧѹ����
		void InitRel49I(void);			//��ʱ���ȹ����ɱ���
		void InitR87IAlarm();

		void InitRel50OLA(void);		//�����ɸ澯

		void ResetIdHLock(void);
		void R68IdH2M(void);		//����г����������
		void R68IdH2C(void);		//����г���ۺ������
		void R68IdH2I(void);		//����г���������
		void R68IrH2M(void);		//�ƶ���������г������
		void R68IdHI(void);		//�ۺ�г������
		void FalutProcess(void);									//����ѭ��������� 
		BOOL RelayPickUpCHK(void);									//���������б���� 
		void Process52BF(void);
		void ComposeData(void);
		void CaluUI(void);
		void CaluR87(void);//�����������ƶ�����
		void Calu49I(void); //���㷴ʱ�޹����ɵ���
		void Calu47U(void);//���㸴�ϵ�ѹ������ص�ѹ��
		void CaluMaxMin(void);
		TRelElecValPar CaluU2(TRelElecValPar *ua,TRelElecValPar *ub,TRelElecValPar *uc); //���㸺���ѹ
		void PTCheck(void);//PT�����ж� 
		void InitFalutHAB(void);//��ѹ������Ͷ��ʱ��������ȹ��ϵ�������
//		void InitRepSerialNum(void);//��ʼ�������������档
		
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);										//����Ԫ�����鸴�麯�� 
		void ThermalOverLoad();
		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������

		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);		
		TRelElecValPar ElecMax(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3,WORD &phase);
		TRelElecValPar ElecMin(TRelElecValPar &ele1,TRelElecValPar &ele2,TRelElecValPar &ele3);
		
        //������������ѹ�����ֵ
		inline LONG MAX(LONG x,LONG y,LONG z)
        {
             return (x>y?(x>z?x:z):(y>z?y:z));
        };	
		
        //�������ѹ����Сֵ
		inline LONG MIN(LONG x,LONG y)
        {
             return (x<y?x:y);
        };
	
};

void RelayTask(DWORD *pdwApp);


#endif
