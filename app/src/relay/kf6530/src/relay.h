
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"


#define CHECK_NUM			8 				//���Լ�Ľṹ��ĸ���
#define	R87IALARMTIME		16000/RELAY_SAMP_INTERVAL  //����Խ�޸澯ʱ�� = 10s/RELAY_SAMP_INTERVAL*625(΢��)

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		const LONG TimeSet;			//��ٶϵı���ʱ�䶨ֵ��Ϊ0
		
		LONG 	*dwSampBuf;     	//�������㻺���� 
		LONG 	*dwRecBuf;			//�ϳɲ��������ݻ����������ںϳ����ݵ�¼�� 		
		LONG	*dwPickDataBuf;		//ͻ�����������������ݻ�����
		LONG	*dwComposeBuf;
		LONG	*dwLIDataBuf;		//��ѹ����ѹ��������ݻ�����
		BYTE  	RelCFGCLR;			//���������˳����������III���޵�Ѻ�������ܣ��򸳸�ֵ
		BYTE  	RelCFGSET;			//��������Ͷ�룬�������III���޵�Ѻ�������ܣ��򸳸�ֵ 
		DWORD	dwChanDataMask;		//����ͨ����������
		DWORD   dwComposeDataMask;
		DWORD	dwDCDataMask;		//������ͨ����������
		LONG	*ptrMartix;
		
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


		TRelElecValPar			UA;		
		TRelElecValPar			UB;
		TRelElecValPar			UC;

		TRelElecValPar			IHA;			//�߲�A�����
		TRelElecValPar			IHB;			//�߲�B�����
		TRelElecValPar			IHC;			//�߲�C�����
		TRelElecValPar			IL1;			//�Ͳ�I1����
		TRelElecValPar			IL2;			//�Ͳ�I2����
		TRelElecValPar			IL3;			//�Ͳ�I3����
		TRelElecValPar			IL4;			//�Ͳ�I4����
		
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
		T87IAlarm				R87Alarm;		//����Խ�ޱ���Ԫ��

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
		BOOL 				m_b52BFFlag;
		BOOL				m_bHaveMakeTripFailRpt;
		
		DWORD				FaultSINum;					//���ϴ��������������� 
		DWORD				FaultLockTime;				//����������ʱ�� 
		WORD 				PickUpRW;					//ģֵ����ͻ���������̵����� 
		TAbsTime 			PickUpTime;					//ģֵ��������ͻ��������ʱ�䶨ʱ�� 

		
		WORD				RptSerialNumOfAct;			//�����������к�
		WORD				RptSerialNumOfStart;		//�����ϵ��������������к�

		LONG				(*pMatrix)[9];  //ϵ������ָ�룬����VV��ѹ����
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
		void InitR87U_A(void);
		void InitR87U_B(void);
		void InitR87U_C(void);
		void InitR87R_A(void);
		void InitR87R_B(void);
		void InitR87R_C(void);
		void InitR87IAlarm(void);
		void InitStructCheck(void);		
		BOOL WriteComposeBuf();

		void ResetIdHLock();
		void R68IdH2M(void);		//����г����������
		void R68IdH2C(void);		//����г���ۺ������
		void R68IdH2I(void);		//����г���������
		void R68IrH2M(void);		//�ƶ���������г������
		void R68IdH5(void);			//���г������
		void R68IdAped(void);		//�����ڷ�������
		
		void R68IdHI(void);	//�ۺ�г������
		
		void Process52BF(void);
		void FalutProcess(void);												//����ѭ��������� 
		BOOL RelayPickUpCHK(void);												//ģֵ�����б���� 
		
		void CaluUI(void);
		void CaluDrift(void);
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void ResetAllRelay(BOOL resetFlag = FALSE);											//����Ԫ�����鸴�麯�� 

		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������
		
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
