
#ifndef _RELAY_H_
#define _RELAY_H_

#include "msgmap.h"
#include "sample.h"
#include "system.h"
#include "relay_defs.h"

//#define CHECK_NUM			9 
#define CHECK_NUM			1 

class CRelayTask:public CApp
{
    DECLARE_MESSAGE_MAP()
public:
		LONG 	*dwSampBuf;     									//�������㻺���� 
//		LONG 	*dwRecBuf;											//�ϳɲ��������ݻ����������ںϳ����ݵ�¼�� 
//		LONG	*dwComposeBuf;
	
		DWORD	dwChanDataMask;									//����ͨ����������
//		DWORD	dwRecDataMask;									//���������������õ�ͨ����������
		DWORD	dwDCDataMask;		//������ͨ����������		
		WORD	DCMacroArray[97];						//����¼���̵�������
		LONG	DCChannelVal[RELAY_SAMP_INTERVAL*6];	//����¼��ͨ������
		LONG	DCCaluResult[6];						//����¼���̵��������ֵ
		
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
		
/*		TRelElecValPar			IC1;		//IC1
		TRelElecValPar			IC2;		//IC2


		TRelSimpGeneOvUnPar	RelR50IC1;
		TRelSimpGeneOvUnPar	RelR50IT1;
		TRelSimpGeneOvUnPar	RelR50IF1;
		TRelSimpGeneOvUnPar	RelR50IP1;
		TRelSimpGeneOvUnPar	RelR50IC2;
		TRelSimpGeneOvUnPar	RelR50IT2;
		TRelSimpGeneOvUnPar	RelR50IF2;
		TRelSimpGeneOvUnPar	RelR50IP2;
*/	
		BOOL				m_bIsLogicOk;
		BOOL				m_bLastRecStatusFlag;		//�ϴ�¼���̵�����״̬
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
//		void WriteComposeBuf(DWORD dwMask);
		
/*		void InitRelR50IC1(void);
		void InitRelR50IT1(void);
		void InitRelR50IF1(void);
		void InitRelR50IP1(void);
		void InitRelR50IC2(void);
		void InitRelR50IT2(void);
		void InitRelR50IF2(void);
		void InitRelR50IP2(void);
*/		
		void InitStructCheck(void);
//		void FalutProcess(void);												//����ѭ��������� 
		void CaluUI(void);

		void OnRelayChange(DWORD dwDataType,DWORD dwIndex);
		
		void OnLogicChanged(void);
		void InitDCRecInfo();
		void WriteDCChannel();	//д������¼������

};		

void RelayTask(DWORD *pdwApp);


#endif
