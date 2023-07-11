
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#define CHECK_NUM			17 				//���Լ�Ľṹ��ĸ���
class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		LONG 	*dwSampBuf;     	//�������㻺���� 
		LONG 	*dwRecBuf;			//�ϳɲ��������ݻ����������ںϳ����ݵ�¼�� 
		LONG 	*dwComposeBuf;		//�ϳɲ��������ݻ����������ںϳ����ݵĸ��ϼ��� 
		LONG 	*dwIncreCurBuf;		//����������ʷ���������ݻ����� 
		LONG	*dwPickDataBuf;		//ͻ�����������������ݻ�����
		LONG	*dwPDDataBuf;
		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 
		LONG	K35;
		DWORD	dwChanDataMask;		//����ͨ����������
		DWORD	dwInCreDataMask;	//���������������õ�ͨ����������
		DWORD	dwComposeDataMask;	//ģ�����ϳ�ͨ����������
		DWORD	dwDCDataMask;		//������ͨ����������		

	
		LONG	lSampData1;
		LONG	lSampData2;
		LONG	lSampDate3;
		LONG	lPickValue;
		LONG	m_lPickUpValue;
		WORD	PickCheck;		
		WORD	PickCheckNext;
		
		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ

		TRelElecValPar			It;
		TRelElecValPar			If;
		TRelElecValPar			Ip;
		TRelElecValPar			U1;
		TRelElecValPar			U2;
		TRelElecValPar			U3;
		TRelElecValPar			U4;
		TRelElecValPar			Up;
		TRelElecValPar			ULock;		//�ٶϹ��������еĵ�ѹ������ѹ
		TRelElecValPar			I2;
		TRelElecValPar			I3;
		TRelElecValPar			I5;
		TRelElecValPar			Impd;
		TRelElecValPar			ImpdT;
		TRelElecValPar			ImpdF;
		
		TRelGeneOvValPar 		Rel50I0;  							//�ٶϱ���Ԫ�� 
		TRelGeneOvValPar	 	Rel50I1; 							//����1�α���Ԫ�� 
		TRelGeneOvValPar 		Rel50I2;							//����2�α���Ԫ�� 
		TRelGeneOvValPar 		Rel50I3;							//����3�α���Ԫ�� 

		TRel21Par 				Rel211;									//����1�α���Ԫ�� 
		TRel21Par 				Rel212;									//����2�α���Ԫ�� 
		TRel21Par				Rel213;									//����3�α���Ԫ�� 
		TRel21Par 				Rel214;									//����4�α���Ԫ�� 

		TRelGeneUnValPar 		Rel27U;							//�͵�ѹ����Ԫ�� 
		TRelInverseCurPar 		Rel51I;							//��ʱ�޹�������Ԫ�� 		
		TRelFaultLocatePar 		Rel21FL_T;						//T�߹��ϲ��Ԫ�� 
		TRelFaultLocatePar 		Rel21FL_F;						//F�߹��ϲ��Ԫ�� 
		TRelPTBrokPar 			RelPT1;							//PT1���߸澯Ԫ�� 
		TRelPTBrokPar 			RelPT2;							//PT2���߸澯Ԫ�� 

		TRel79Par				Rel79;							//�غ�բ����Ԫ�� 
		TRel50DIPar				Rel50DI;							//��������Ԫ�� 

		
		BOOL				TripFailFlag;				//��բʧ�ܱ�־ 
		BOOL 				FaultOnFlag;				//�������ѭ�������־ 
		BOOL				CurRecoverFlag;				//�غ�բ�����󣬵����ָ���־ 
		BOOL				FaultCheckOutFlag;			//����ѭ�����������ϱ�־ 
		BOOL 				HaveMakeStartRptFlag;		//�����������־ 
		BOOL				m_bFaultLKFlag;				//������ʼʱ��ʱ��������־������ͻ��������	
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
		//BOOL				m_bNextRelayStart;			//���α��������ж��п��ܷ����ٴζ�����־
		BOOL				m_bHaveTripFailedRpt;		//�Ѿ�������·����բʧ�ܱ���
		BOOL				m_bHaveRelayAct;			//���α��������ж��б�������������־	
		BOOL				m_bIsLogicOk;
		BOOL				m_bIsHaveMakePTRpt;		//����PT���߱������뱣������
		BOOL				m_bOneOhmPTLK;			//�����迹С��1ŷķPT����		
		BOOL				m_bPDCheck;				//�Ƿ�����书�ʷ����ж�
		BOOL 				m_b52BFFLAG;
		BOOL				m_bR79PickUpRpt;
		LONG				m_lOtherRelayTrip;		//��������������������������
		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		WORD 				PickUpRW;					//ģֵ����ͻ���������̵����� 
		TAbsTime 			PickUpTime;					//ģֵ��������ͻ��������ʱ�䶨ʱ�� 

//		WORD				RptSerialNumOfPick;			//��α������к�   yanxs 13-02-05
//		BOOL				RptFlagOfPickUp;				//������α���������־
		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;		//�����ϵ��������������к�
		WORD				RptSerialNumOf27U;			//��ѹ�����������к�

		BOOL 				m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD				m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����

		//�������棬��Թ��緽ʽ�Ĳ�ͬ����Ӧ�Ĺ��ϵ�����ͬ
		WORD				wRep211;
		WORD				wRep212;
		WORD				wRep213;
		WORD				wRep214;
		WORD				wRep50I0;
		WORD				wRep50I1;
		WORD				wRep50I2;
		WORD				wRep50I3;
		WORD				wRep51I;
		WORD				wRep50DI;
		WORD				wRepR212Acc;
		WORD				wRepR213Acc;
		WORD				wRepR214Acc;
		WORD				wRep50I1Acc;
		WORD				wRep50I2Acc;
		WORD				wRep50I3Acc;
		WORD				wRep27U;
		//*************************������α��沿��    **********************   yanxs   13-02-05
		WORD				wRep211Pick;
		WORD				wRep212Pick;
		WORD				wRep213Pick;
		WORD				wRep214Pick;
		WORD				wRep50I0Pick;
		WORD				wRep50I1Pick;
		WORD				wRep50I2Pick;
		WORD				wRep50I3Pick;
		WORD				wRep51IPick;
		WORD				wRep50DIPick;
		WORD				wRepR212AccPick;					//���Ӽ�����α���   2013-06-20   yanxs
   		WORD				wRepR213AccPick;
		WORD				wRepR214AccPick;
		WORD				wRep50I1AccPick;
		WORD				wRep50I2AccPick;
		WORD				wRep50I3AccPick;
//		WORD				wRep27UPick;
		//*******************************************
		//*************************���ӷ��ر��沿��    **********************	yanxs	13-06-20
		WORD				wRep211RES;
		WORD				wRep212RES;
		WORD				wRep213RES;
		WORD				wRep214RES;
		WORD				wRep50I0RES;
		WORD				wRep50I1RES;
		WORD				wRep50I2RES;
		WORD				wRep50I3RES;
		WORD				wRep51IRES;
		WORD				wRep50DIRES;
		WORD				wRepR212AccRES;					
		WORD				wRepR213AccRES;
		WORD				wRepR214AccRES;
		WORD				wRep50I1AccRES;
		WORD				wRep50I2AccRES;
		WORD				wRep50I3AccRES;
private:


public:
		void OnSampleEnd(DWORD ptr, DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitRel50I0(void);
		void InitRel50I1(void);
		void InitRel50I2(void);
		void InitRel50I3(void);
		void InitRel211(void);
		void InitRel212(void);
		void InitRel213(void);
		void InitRel214(void);
		void InitRel27U(void);
		void InitRel51I(void);
		void InitRel21FL_T(void);
		void InitRel21FL_F(void);
		void InitRelPT1(void);
		void InitRelPT2(void);
		void InitRel79(void);
		void InitRel50DI(void);
		void InitRepSerialNum(void);
		void InitLoadMonitor(void);
		void InitStructCheck(void);
		void PTCheck();
		void WriteComposeBuf();
		void FalutProcess(void);												//����ѭ��������� 
		BOOL RelayPickUpCHK(void);										//ģֵ�����б���� 
		void CaluUI(void);
		void CaluUpIp(void);
		void CaluZ(void);
		void CaluUIAngle(void);
		void Process52BF(void);

		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void CalcFaultLocateSetTab(TRelFaultLocatePar *Relay);	//������ϲ�ඨֵ�� 
		void FaultLocate(TRelFaultLocatePar *Relay);		//�翹�����ϲ�� 
		void ResetAllRelay(BOOL resetFlag = FALSE);											//����Ԫ�����鸴�麯�� 

		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������
		void LoadMonitor();//���ɼ��

};		

void RelayTask(DWORD *pdwApp);


#endif
