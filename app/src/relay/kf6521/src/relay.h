
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#define CHECK_NUM			12 				//���Լ�Ľṹ��ĸ���

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
		LONG	*dwPDDataBuf;		//���ʷ�����㣬���ڴ�����Ĺ��ʷ������
		LONG	*dwRecBuf;

		
		SHORT			*m_wFreqBuf;		
		LONG			*m_FreqCaluBuf;
		DWORD			m_dwFreqMask1;
		DWORD			m_dwFreqMask2;
		LONG			m_dwFreq1; //ĸ�߲�Ƶ��		
		LONG			m_dwFreq2; //��·��Ƶ��

		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 

		DWORD	dwChanDataMask;		//����ͨ����������
		DWORD	dwDCDataMask;		//������ͨ����������	
		DWORD	dwComposeMask;

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
		TRelElecValPar			ULK;	//���ڵ�ѹ�����ĵ�ѹ,Ϊ�����ߵ�ѹ�е���Сֵ
		TRelElecValPar			Up;		//���ڵ�ѹ������Ϊ�����ߵ�ѹ�е����ֵ
		TRelElecValPar			NU2;		//�����ѹ
		TRelElecValPar			Ip;
		TRelElecValPar			U5;
		TRelElecValPar			I5;
		TRelElecValPar			Factor1; //����1
		TRelElecValPar			Factor2;//����2


		TRelGeneOvValPar		R50I0;    //�����ٶϱ���Ԫ��
		TRelGeneOvValPar		R50IOL;		//�����ɱ���Ԫ��
		TRelGeneOvValPar		R50ISH;		//ĸ�߳�籣��Ԫ��
		TRelGeneOvValPar		R50I1;
		TRelGeneOvValPar		R50I2;
		TRelGeneOvValPar		R50I3;
		TRelGeneOvValPar		R50N1;
		TRelGeneOvValPar		R50N2;
		TRelGeneUnValPar		R27U;			
		TRel79Par				Rel79;							//�غ�բ����Ԫ�� 


		TRelElecValPar		Usyn;
		TRelElecValPar		m_DU;

		LONG				m_DF;	//Ƶ��
		LONG				m_DA;	//�ǲ�	
		LONG				m_DV;	//�ǲ�
		LONG				m_FA;
		LONG				m_DQ; //����
		TREL25R				Rel25;		
		BOOL 				caluFlag;

		//------------------�ӵ�ѡ�߲��ֶ���-------------------
		BOOL				JDXXOnFlag;			//�ӵ�ѡ�����ڵȴ�ѡ�߽����־
		BOOL				JDXXCheckFlag;		//�ӵ�ѡ���Ѿ�������־
		BOOL				JDXXPickFlag;       //�ӵ�ѡ�߽���3U0�����жϱ�־
		BOOL				JDXXForwardFlag;		//�ӵ�ѡ��U0��I0���ʷ���Ϊ�����־
		BOOL				HaveJDXXResultFlag;			//�ӵ�ѡ���ѵó������־
		
		BOOL				TripFailFlag;				//��բʧ�ܱ�־ 
		BOOL 				FaultOnFlag;				//�������ѭ�������־ 
		BOOL				CurRecoverFlag;				//�غ�բ�����󣬵����ָ���־ 
		BOOL				FaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				HaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		BOOL				m_bIsLogicOk;
		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����

		BOOL				m_bIsHaveMakePTRpt;		//����PT���߱������뱣������
		BOOL				m_bPDCheckA;				//�Ƿ�����书�ʷ����ж�		
		BOOL				m_bPDCheckB;				//�Ƿ�����书�ʷ����ж�
		BOOL				m_bPDCheckC;				//�Ƿ�����书�ʷ����ж�
		BOOL 				m_b52BFFLAG;
		BOOL				m_bHaveMakeTripFailRpt;
		BOOL				m_bR79PickUpRpt;
		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		TAbsTime 			PickUpTime;					//ģֵ��������ͻ��������ʱ�䶨ʱ�� 

		
		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;		//�����ϵ��������������к�
		WORD				RptSerialNumOf27U;			//��ѹ�����������к�

		WORD				m_Phase;
		LONG				m_U2Set;
		DWORD				m_BusPTTimer;
		DWORD				m_LinePTTimer;
		

public:
		void OnSampleEnd(DWORD ptr, DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitR50I0(void); //���ӵ����ٶϱ���
		void InitR50I1(void);
		void InitR50I2(void);
		void InitR50I3(void);
		void InitR50N1(void);
		void InitR50N2(void);
		void InitR50ISH(void); //����ĸ�䱣��
		void InitR27U(void);		
		void InitRel79(void);
		void InitR50IOL(void);//���ӹ����ɸ澯����
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



		
		void CaluDF_DA_DV(); //����ѹ��ǲ�Ƶ��		
		void InitRel25(void);
		void Rel25Check(void); //��ͬ��
		void ResetRel25(void);
		void JudgeGrid(void);	//�ж�ͬ���������ǵ���
		void ResetJudgeGrid(void);//��λ�����ж�
		void SameGridCheck(void);
		void SingleGridCheck(void);
		void DiffGridCheck(void);
		void CaluPreAngle(void);
		//---------------------�ӵ�ѡ�߲���------------------
		void JDXXCheck(void);		//�ӵ�ѡ���жϳ���
		void JDXXPickUpCHK(void);	//�ӵ�ѡ�������ж�
		void OnJDXXResult(DWORD arg,DWORD result);
		void JDXXPowerDirCheck(void);
		void WriteJDXXData(void);
};		

void RelayTask(DWORD *pdwApp);


#endif
