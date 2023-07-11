
#ifndef _RELAY_H_
#define _RELAY_H_

#include "system.h"
#include "msgmap.h"
#include "sample.h"
#include "relay_defs.h"

#define CHECK_NUM			1 

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:

		DWORD   dwFreqMask;
		SHORT    *dwFreqBuf;
		DWORD   m_dwFreq;
		DWORD   m_dwTs;
		
		LONG 	*dwSampBuf;     									//�������㻺���� 	
		DWORD	dwChanDataMask;									//����ͨ����������
		
		DWORD	dwDCDataMask;		//������ͨ����������		
		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ

		
		BOOL				m_bIsLogicOk;
		BOOL				m_bLastRecStatusFlag;
		TRelElecValPar 		  UHA;
		TRelElecValPar 		  UHB;
		TRelElecValPar 		  UHC;
		TRelElecValPar 		  U0;
		TRelElecValPar 		  ULA;
		TRelElecValPar 		  ULB;
		TRelElecValPar 		  ULC;
		TRelElecValPar 		  IHA;
		TRelElecValPar 		  IHB;
		TRelElecValPar 		  IHC;
		TRelElecValPar 		  ILA;
		TRelElecValPar 		  ILB;
		TRelElecValPar 		  ILC;
		TRelElecValPar 		  I1;
		TRelElecValPar 		  I2;
		
		TRelElecValPar			UHA1;
		TRelElecValPar			UHA2;
		TRelElecValPar			UHA3;
		TRelElecValPar			UHA5;
		TRelElecValPar			UHA7;
		TRelElecValPar			UHA9;
		TRelElecValPar			UHA11;		
		TRelElecValPar			UHB1;
		TRelElecValPar			UHB2;
		TRelElecValPar			UHB3;
		TRelElecValPar			UHB5;
		TRelElecValPar			UHB7;
		TRelElecValPar			UHB9;
		TRelElecValPar			UHB11;
		TRelElecValPar			UHC1;
		TRelElecValPar			UHC2;
		TRelElecValPar			UHC3;
		TRelElecValPar			UHC5;
		TRelElecValPar			UHC7;
		TRelElecValPar			UHC9;
		TRelElecValPar			UHC11;		
		TRelElecValPar			IHA1;
		TRelElecValPar			IHA2;
		TRelElecValPar			IHA3;
		TRelElecValPar			IHA5;
		TRelElecValPar			IHA7;
		TRelElecValPar			IHA9;
		TRelElecValPar			IHA11;		
		TRelElecValPar			IHB1;
		TRelElecValPar			IHB2;
		TRelElecValPar			IHB3;
		TRelElecValPar			IHB5;
		TRelElecValPar			IHB7;
		TRelElecValPar			IHB9;
		TRelElecValPar			IHB11;
		TRelElecValPar			IHC1;
		TRelElecValPar			IHC2;
		TRelElecValPar			IHC3;
		TRelElecValPar			IHC5;
		TRelElecValPar			IHC7;
		TRelElecValPar			IHC9;
		TRelElecValPar			IHC11;
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
		
		void InitStructCheck(void);		
		void CaluUI(void);
		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������

};		

void RelayTask(DWORD *pdwApp);


#endif
