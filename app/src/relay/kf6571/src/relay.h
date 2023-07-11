
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"
#include "channel.h"

#define CHECK_NUM			1 
#define RPT_NULL 65535

#define	T_R			0x200  	//TR��·����
#define	F_R			0x201	//FR��·����
#define	T_F			0x202	//TF��·����
#define	U_D			0x203	//����·����

#define	RANGE1		0x210	//��һ�������
#define	RANGE2		0x211	//�ڶ��������
#define	RANGE3		0x212	//�����������
#define	RANGE4		0x213	//�����������
#define	RANGE5		0x214	//�����������

#define	UPLINE		0x220	//����
#define	DOWNLINE	0x221	//����

//���ڼ�¼���ж�3�������Ĵ�С�������õ����Ǳ�����һ·�ı�־
#define I1I2I3		0x300   //I1>I2>I3
#define I1I3I2		0x301	//I1>I3>I2
#define I3I1I2		0x302	//I3>I1>I2
#define I3I2I1		0x303	//I3>I2>I1
#define I2I1I3		0x304	//I2>I1>I3
#define I2I3I1		0x305	//I2>I3>I1

#define LOCKSSDATANUM FLCOMPLICATENUM+1

#define FLMODE_NULL   0x400   //δ�����ģʽ
#define FLMODE_0   0x401
#define FLMODE_1   0x402
#define FLMODE_2   0x403
#define FLMODE_3   0x404
#define FLMODE_4   0x405
#define FLMODE_5   0x406
#define FLMODE_6   0x407
#define FLMODE_7   0x408
#define FLMODE_8   0x409
#define FLMODE_9   0x410
#define FLMODE_10   0x411
#define FLMODE_11   0x412
#define FLMODE_12   0x413

enum RelayType{Relay50,Relay21,Relay50D1,Relay50D2};

















typedef struct LOCKSSDATA{

	TSubData subData;	
	TRelElecValPar			lockU1; 
	TRelElecValPar			lockU2;
	TRelElecValPar			lockU3;
	TRelElecValPar			lockU4;
	TRelElecValPar			lockIT1;
	TRelElecValPar			lockIF1;
	TRelElecValPar			lockIT2;
	TRelElecValPar			lockIF2;
	TRelElecValPar			lockIT3;
	TRelElecValPar			lockIF3;
	TRelElecValPar			lockIAT1;
	TRelElecValPar			lockIAT2;		
	TRelElecValPar			lockUp1;
	TRelElecValPar			lockUp2;
	TRelElecValPar			lockIp1;
	TRelElecValPar			lockIp2;
	TRelElecValPar			lockIp3;
	TRelElecValPar			lockImpd;
	TRelElecValPar			lockImpd_T;
	TRelElecValPar			lockImpd_F;

	WORD					exchangeFlag;
	RelayType               relaytype;
	
}SLockSSData;

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		DWORD   dwDataPtr;

		BYTE	RelCFGCLR;
		BYTE 	RelCFGSET;
		WORD    wFaultSINum;
		DWORD	dwChanDataMask;									//����ͨ����������
		DWORD   dwComposeDataMask;			
		DWORD	dwInCre1DataMask;	//���������������õ�ͨ����������
		DWORD	dwInCre2DataMask;	//���������������õ�ͨ����������
		LONG	*dwComposeBuf;   //�ϳ����ݻ�����
		LONG 	*dwSampBuf;     	//�������㻺���� 	
		LONG    *dwATSingleBuf;		//�������ATʱ�������е���AT���ϵ����ϳ�
		LONG	*dwComposeUBuf;
		LONG	*dwRecBuf;		
		LONG 	*dwIncre1CurBuf;		//����1������ʷ���������ݻ�����		
		LONG 	*dwIncre2CurBuf;		//����1������ʷ���������ݻ����� 
		LONG	*dwTraceDataBuf;
		LONG 	*dwCCModeDataBuf;	//�����ϳɷ�ʽ���ϳ�ͨ�����ݻ�����
		LONG 	*dwSubCCModeDataBuf; //�����ϳɷ�ʽ���������ٻ�����ʱ,�����ϳ����ݻ�����
		DWORD	dwCTRateCoef1;		//�����ϳɷ�ʽ���ϳ�ͨ��ϵ������ϵ��,����
		DWORD	dwCTRateCoef2;		//�����ϳɷ�ʽ���ϳ�ͨ��ϵ������ϵ��,����
		
		DWORD	dwDCDataMask;							//������ͨ����������		
		WORD	DCMacroArray[33];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*2];	//����¼��ͨ������
		LONG	DCCaluResult[2];						//����¼���̵��������ֵ		
		BOOL	m_bIsLogicOk;				//�߼����̳�ʼ���ɹ���־
		BOOL	m_bLastRecStatusFlag;       //�ɱ��¼��������־
		BOOL 	m_bStartHandOverFlag;		//����Խ���л������־
		BOOL	m_bRecoverHandOverFlag;		//�ָ�Խ���л������־
		WORD	m_SSPSelect;



		LONG				m_lOtherRelayTrip;		//��������������������������


		
		TRelElecValPar 			m_U1;                   //�����ñ�������
		TRelElecValPar 			m_U2;
		TRelElecValPar 			m_U3;
		TRelElecValPar 			m_U4;
		TRelElecValPar			m_ULock;	//����������ѹ�����õĵ�ѹ
		TRelElecValPar 			m_IT1;
		TRelElecValPar 			m_IF1;
		TRelElecValPar 			m_IT2;
		TRelElecValPar 			m_IF2;
		TRelElecValPar 			m_IT3;
		TRelElecValPar 			m_IF3;
		TRelElecValPar 			m_IAT1;
		TRelElecValPar 			m_IAT2;
		TRelElecValPar 			m_UP1;
		TRelElecValPar 			m_UP2;
		TRelElecValPar 			m_U13;
		TRelElecValPar 			m_U24;
		TRelElecValPar 			m_IP1;
		TRelElecValPar 			m_IP2;
		TRelElecValPar 			m_IP3;
		TRelElecValPar			m_IP1_h2;
		TRelElecValPar			m_IP1_h3;
		TRelElecValPar			m_IP1_h5;
		TRelElecValPar			m_IP2_h2;
		TRelElecValPar			m_IP2_h3;
		TRelElecValPar			m_IP2_h5;
		TRelElecValPar			m_IP3_h2;
		TRelElecValPar			m_IP3_h3;
		TRelElecValPar			m_IP3_h5;		
		TRelElecValPar			m_UP;     //�����������������������������������ĵ���
		TRelElecValPar			m_IP;
		TRelElecValPar			m_Impd;
		TRelElecValPar			m_IP_h2;
		TRelElecValPar			m_IP_h3;
		TRelElecValPar			m_IP_h5;
		TRelElecValPar			m_I1_AT;   //����������1�ϳ����ϵ���
		TRelElecValPar 			m_I2_AT;   //����������2�ϳ����ϵ���
		TRelElecValPar 			m_I3_AT;   //����������3�ϳ����ϵ���
		TRelElecValPar 			m_I_AT;   //���������ϵ���
		LONG					K35;
		WORD					RW37IP;

		//��������Ԫ������
		TRel50DIPar				Rel50D1;
		TRel50DIPar				Rel50D2;
		TRel21Par				Rel21;
		TRelGeneOvValPar		Rel50;		
		TRelFaultLocatePar 		T_FaultLocate;
		TRelFaultLocatePar		F_FaultLocate;
		TRelFaultLocatePar		TF_FaultLocate;
		//�������������жϵ��м���������ݱ��õ�����������и�ֵ
		TRelElecValPar  		m_I50D1_0;
		TRelElecValPar  		m_I50D1_2;
		TRelElecValPar  		m_I50D1_3;
		TRelElecValPar  		m_I50D1_5;
		TRelElecValPar  		m_I50D2_0;
		TRelElecValPar  		m_I50D2_2;
		TRelElecValPar  		m_I50D2_3;
		TRelElecValPar  		m_I50D2_5;
		//һ��ֵ
		LONG					m_U1mod;				//��������ģֵ
		LONG					m_U2mod;
		LONG					m_U3mod;
		LONG					m_U4mod;
		LONG					m_IT1mod;
		LONG					m_IF1mod;
		LONG					m_IT2mod;
		LONG					m_IF2mod;
		LONG					m_IT3mod;
		LONG					m_IF3mod;
		LONG					m_IAT1mod;
		LONG					m_IAT2mod;
		LONG					m_UP1mod;
		LONG					m_UP2mod;
		LONG					m_U13mod;
		LONG					m_U24mod;
		LONG					m_IP1mod;
		LONG					m_IP2mod;
		LONG					m_IP3mod;
		LONG					m_IAT_SS;//��������ϵ���һ��ֵ
		LONG					m_iat_ss;//��������ϵ�������ֵ
		BYTE					m_CB;

		//������Ϊ����ʱ��ʹ�����±������м���׷��ֵ
		TRelElecValPar					subU1;		
		TRelElecValPar					subU2;
		TRelElecValPar					subU3;
		TRelElecValPar					subU4;
		TRelElecValPar					subIT1;
		TRelElecValPar					subIF1;
		TRelElecValPar					subIT2;
		TRelElecValPar					subIF2;
		TRelElecValPar					subIT3;
		TRelElecValPar					subIF3;
		TRelElecValPar					subIAT1;
		TRelElecValPar					subIAT2;
		TRelElecValPar					subIAT;
		TRelElecValPar					subIp1;
		TRelElecValPar					subIp2;
		TRelElecValPar					subIp3;

		//���¶���������ڲ�����Ļ�����
		//��������������,����ǰ4�����ڱ����Ĳ��������һ�������ֶ��ٲ�
		SLockSSData					lkSSData[FLCOMPLICATENUM+1]; 
		//�������õ����ݣ����������Լ����4������
		TSubData					RelaySubData[FLCOMPLICATENUM][MAX_SUBSTATION_NUM+1];          
		TSubData					ManualSubData[MAX_SUBSTATION_NUM+1];
		TSubData					HistorySubData[MAX_SUBSTATION_NUM+1];
		TSubData					LowVolSubData[MAX_SUBSTATION_NUM+1];
		TSubData					HeartbeatSubData[MAX_SUBSTATION_NUM+1];
		TSubData					*pTSubData;

		
		//���������ض�ֵ
		LONG						DistanceSet[MAX_SUBSTATION_NUM];
		LONG						DistCoef[MAX_SUBSTATION_NUM+1];//�����ֲ�ϵ��
		WORD						ChanErrRW[MAX_SUBSTATION_NUM];//����ͨ�����ϼ̵����ֱ�
		LONG						QValue[MAX_SUBSTATION_NUM+1];


		
		BOOL					bStopRefreshRMFlag;//���ϲ��������ֹͣˢ�±�������ֵ��־
		TAbsTime 				FLPickTime;			//���ϲ����������ʱ��
		DWORD					dwFLPickupTime; 	//�������ʱ��
		DWORD					dwNoFaultTime; 		//�޹���ʱ��
		DWORD					dwFaultLKTime;		//���϶���ʱ��
		TAbsTime 				PickUpTime;			//ģֵ��������ͻ��������ʱ�䶨ʱ�� 		
		WORD					RptSerialNumOfStart;//�����ϵ��������������к�
		LONG					*dwPickUpBuf;  		//ͻ���������ж��õ����ݻ�����
		
		BOOL 					m_bFaultOnFlag;		//�������ѭ�������־ 
		BOOL 					m_bHaveTripFlag;    //���϶�����־
		BOOL					m_bFaultCheckOutFlag;//������������ϱ�־
		DWORD					m_dwFaultSINum;
		
		BOOL 					HaveMakeStartRptFlag;		//�����������־ 
		BOOL					putoutLedFlag;//��������Ĳ�ද��LED��
		
		LONG					lSampData1;
		LONG					lSampData2;
		LONG					lSampDate3;
		LONG					lPickValue;		
		WORD					m_wPickCheck;					//���α�������������ͻ�������ڶ�ֵ������
		WORD					m_wPickCheckNext[3];			//���α�������������ͻ�������ڶ�ֵ������
		LONG					m_lPickUpValueArray[3];			//���α�������������ͻ�������ڶ�ֵ������

		
		BYTE					channelCheckRptSerNo;
		DWORD					manualFLPickTime; //ʵ��������ʱ��
		BOOL					manualFLPickFlag; //ʵ����������־
		BOOL					manualFLTimeoutFlag; //ʵ���೬ʱ��־
		BOOL					lastManualRWStatus;
		WORD					manualFLSerialNo;	//ʵ�������Ĺ��ϲ�౨�����к�
		WORD    				RptSerialNumOfAct;  //���������Ĺ��ϲ�౨�����к�
		BOOL					SubGPSTimeoutFlag;//������GPSʱ��������

		DWORD					faultINF;
		WORD					faultType;//��������
		WORD					faultRange;//��������
		WORD					faultDirection; //���Ϸ���
		WORD					exchangeElecFlag; //�ڳ��ֱ������������Ͷ�˵�����£���¼�½���״̬
		LONG					*pRMHeadAddr;//������������ֵ�׵�ַ
		BOOL					lastVolHaveMark;//��һ�ε�ѹ��ֵ�Ƚ����
		DWORD					lowVolCounter;
		DWORD					lowVolPerTime;
		DWORD					lowVolTimeSet;
		BYTE 					manualDataType; //�ֶ��ٲ�ʱ��Ҫ�ٲ����������

		BOOL					haveMakeGPSERRRpt; //�Ѿ�����GPS���ϱ���
		BOOL 					m_bLogicChanged;            //�ɱ���߼��仯��־
		DWORD					m_dwWaitLogicCounter;		//�ɱ���߼��仯��ֹͣ�Լ��õļ�����


		//��ʱ�迹�������翹�����ʱ���������������迹
		TRelElecValPar			m_LImpd_T;
		TRelElecValPar			m_LImpd_F;
		TRelElecValPar			m_LImpd_TF;
		//ʵʱ������迹
		TRelElecValPar			Impd_T;
		TRelElecValPar			Impd_F;
		TRelElecValPar			Impd_TF;




		int   			rpt_F_TR;  //����TR����
		int 			rpt_F_FR; //����FR����
		int				rpt_B_TR;	//����TR����
		int				rpt_B_FR;   //����FR����
		int				rpt_TF;     //TF����
		int				rpt_TimeOut;//��ʱ���ϲ��ʧ��
		int				rpt_ComTest; //ͨ�Ų���
		int				rpt_GpsErr; //GPS���ϲ��ʧ��
		int				rpt_FLFail; //���ʧ��
		int 			rpt_F_TF; //����TF����
		int				rpt_B_TF;	//����TF����

		
		WORD 			powerSupplyMode;
		//��������Ԫ����ʶ�����������н��к��ж���������·�����Ĺ���
		RelayType      relayType;
private:


public:
		void OnSampleEnd(DWORD arg);
		void OnTimeOut(DWORD id);
		
		CRelayTask(DWORD *pdwApp);
		void Init(void);
		void InitSet(void);
		void InitRealy(void);
		void InitRel50D();
		void InitRel21();
		void InitRel50();
		void InitSubInfo();
		void InitRptSN();
		void InitFLIntervalSet(); //��ʼ�������������ض�ֵ		
		void InitFaultLocate();
		void CaluFaultLocateSetTable(TRelFaultLocatePar*);		
 		void WriteComposeBuf();
		void InitStructCheck(void);		
		void CaluUI(void);
		void CaluImpd(WORD no);
		void CaluKM();//���㹫���
		void WriteRM(void);//�����ݿ���д�뱣������ֵ
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		void OnLogicChanged(void);
		void OnStepOver(WORD rc);
		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������
		BOOL RelayPickUpCheck();
		void ChannelCheck();
		void FaultLocate();
		void FLPickUpCheck();
		void FaultLocateAuto();   //��������
		void FaultLocateManual(); //�ֶ�ʵ������
 		void FaultLocateCalu(TSubData *subData,subChannelCheck FLData[],WORD no,BYTE rptSerialNo);//������
		void BoostingCurrent(TSubData *subData);		//���ϵ�����
		void UpDownLineCurrent(BYTE rptSerialNo); // �����е�����
		void B_LineCurrent(BYTE rptSerialNo);		//����������
		BOOL XLineWithSub(TSubData *tsubData,WORD no,BYTE rptSerialNo); //���Ե翹�����
		BOOL XLineWithoutSub(BYTE rptSerialNo);
 		void FLSubTimeout(subChannelCheck FLData[],BYTE rptSerialNo);//�ٻ��������ݳ�ʱ����		
		void LockingSSData(WORD no,WORD FLType,BOOL refreshFlag); //������������
		void RefreshSubData(TSubData *subData,subChannelCheck fldata[]);	//������������
 		void MakeSSFLRpt(WORD no,BYTE rptSerialNo,WORD SSInf); //���������ද������
		void MakeSubFLRpt(subChannelCheck FLData[],WORD no,BYTE rptSerialNo,WORD subInfType); //��������ද������
		void MakeSingleSubFLRpt(WORD subno,DWORD inf,subChannelCheck *data,BYTE rptSerialNo);
		void ResetFoultLocate();		
		void ResetAllRelay();
		void OnGetFLData(WORD arg1,DWORD arg2,DWORD dataType);
		void LockDataForLowVol();//����ѹ����һ��ֵʱ��������
		DWORD GetGPSStatus(STATUS flag,BYTE CB);
		void checkSelfGps();//��鱾װ�õ�GPS״̬		
 		void HandOverProcess(void);
		
		LONG MAX4(LONG x,LONG y,LONG z,LONG w);
		LONG MAX5(LONG x,LONG y,LONG z,LONG w,long v);
	 	LONG MAX3(LONG x,LONG y,LONG z);		
	 	LONG MIN3(LONG x,LONG y,LONG z);
		LONG MAX2(LONG x,LONG y);
	 	LONG MIN2(LONG x,LONG y);

		WORD CheckPowerSupplyMode(TSubData *tsubData);
		void CalcFLMode_0(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_1(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_2(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_3(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_4(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_5(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_6(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_7(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_8(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_9(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_10(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_11(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		void CalcFLMode_12(TSubData *tsubData,WORD no,BYTE rptSerialNo);		
		void CalcFLMode_13(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		//�翹����࣬����Ҫ�ж�ѹ�����õ�Ͷ�룬������Ӧģʽ��ʹ��
		void CalcFLX(TRelFaultLocatePar *Relay);
		//���ݱ�����λ��״̬����������˳��
		void AdjustCurrent();
		//�������н���ʱ���ж������з������ϻ������з�������
		int  WhichShortcircuit(TSubData *tsubData,WORD no);
		//���������Ĺ��ϲ�࣬����������Ӧ�жϣ����ù̶�ģʽ
		void CalcFLWith3Sub(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		//Խ����࣬����������Ӧ�жϣ����ù̶�ģʽ
		void CalcFLWithHandOver(TSubData *tsubData,WORD no,BYTE rptSerialNo);
		//�ж�����������ģֵ�Ƿ�������
		BOOL IsAEqualB(LONG A, LONG B);
		//����Ϊ����ʱ�����������ݻ�������д��ʵʱ����
		void RefreshHeartData();
		//�����Լ�
		void BOCheck();
		// TF����ʱ���Ϸ�����ж�
		WORD TFFaultDir(TSubData *tsubData);

};		

void RelayTask(DWORD *pdwApp);


#endif
