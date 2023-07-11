#ifndef _AI_H_
#define _AI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbfunc.h"
#include "parafunc.h"
#include "Relay_defs.h"

//#define AI_TIMER_PERIOD			200		//����AI��ʱ��ʱ����Ϊ10����
//����AI��ʱ��ʱ����Ϊ240���룬�޸�ʱ��ע������ܻ�Ӱ�쵽��Ƶ,�豣֤���¹�ϵ:AI_TIMER_PERIOD/20 > FREQBUFSIZE
#define AI_TIMER_PERIOD			200	
//ÿ�β�����������Ҫ�����Ƶ�ʸ���
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)
//------------------------------------------------------------------
#define CTBROKERDELAY		15			//CT���߼��ʱ�� 3s     3000/AI_TIMER_PERIOD = 15
#define CTBRKPM				15				//CT���߼��Ip��Im������ϵ
#define CTBRCHECKVALUE 		1612		//CT���߼�ֵⷧ��0.05In��Ӧ�Ĳ���ֵ������
//------------------------------------------------------------------

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
//---------------��Ƶ����ر���------------------------------
		SHORT			*m_wFreqBuf;		
		LONG			*m_FreqCaluBuf;
		DWORD			m_dwFreqMask;
		LONG			m_dwFreq;
		DWORD			m_dwPreFreq;		
		BOOL			m_FreqFlag;
//-----------------------------------------------------------
		LONG			*m_pDataBuf;			//�������ݻ�����
		LONG			*m_pComposeBuf;			//�������ݺϳɻ�����
		LONG			*m_pCoefAdjustBuf;		//ϵ��У�����ݻ�����
		
		DWORD			m_AIDataMask;			//����ͨ���õ���������
		DWORD			m_CoefAdjustMask;		//ϵ��У���õ�ͨ����������


		DWORD			m_dwAdjustCnt;			//ϵ��У���������
		BOOL			m_bIsCoefAdjust;		//��ʼϵ��У����־
		BOOL			m_bIsSourceOK;			//Դ��У����־
		BOOL			m_bIsDriftAdjust;		//����ֵУ����־
		DWORD			m_dwWaitCnt;			//ϵ��У����ʼ�ȴ�����

		LONG			m_dwSourceU;
		LONG			m_dwSourceI;
		QWORD			m_OneKilowattP;			// 1�ȵ��������,�й�		
		QWORD			m_OneKilowattQ;			// 1�ȵ��������,�޹�
		QWORD			m_AccKilowattP;			//�ۼӵĹ���ֵ���й�
		QWORD			m_AccKilowattQ;			//�ۼӵĹ���ֵ���޹�


//----------------ң��ֵ����-----------------------------

		LONG			Ua;
		LONG			Ub;
		LONG			Uc;
		LONG			U0;
		LONG			U1;
		LONG			U2;
		LONG			U3;
		LONG			Uab;
		LONG			Ubc;
		LONG			Uca;
		LONG			IA;
		LONG			IB;
		LONG			IC;
		LONG			I0;
		LONG			I1;
		
		LONG			Pa;
		LONG			Qa;
		LONG			Pb;
		LONG			Qb;
		LONG			Pc;
		LONG			Qc;
		LLONG			P;
		LLONG			Q;
		LLONG			S;
		LONG			cos;

		TRelElecValPar	IAM;
		TRelElecValPar	IBM;
		TRelElecValPar	ICM;
		TRelElecValPar	UAM;
//---------------CT���߼�鹦����������-----------------------	 yanxs 2013-03-22
		TRelCTBrokPar	RelCTA;
		TRelCTBrokPar	RelCTB;
		TRelCTBrokPar	RelCTC;
		LONG			CTCurSet;			//CT���߼������ż�ֵ
		LONG			CTTimeSet;			//CT���߸澯ʱ��
		LONG			Ia_p;				//CT���߼�Ᵽ������Ia
		LONG			Ib_p;
		LONG			Ic_p;
		LONG			IaMax;				//CT���߼��a��������
		LONG			IaMin;
		LONG			IbMax;
		LONG			IbMin;
		LONG			IcMax;
		LONG			IcMin;

//---------------����ϵ��У���ı�������-----------------
		LONG			AdjAA;				//ϵ��У��ʱ�������õĽǲ�ϵ��
		LONG			AdjBA;				//ϵ��У��ʱ�������õĽǲ�ϵ��		
		LONG			AdjCA;				//ϵ��У��ʱ�������õĽǲ�ϵ��
		
		LONG			AdjUa;
		LONG			AdjUb;
		LONG			AdjUc;
		LONG			AdjU0;
		LONG			AdjU1;
		LONG			AdjU2;
		LONG			AdjU3;		
		LONG			AdjIA;
		LONG			AdjIB;
		LONG			AdjIC;
		LONG			AdjI0;
		LONG			AdjI1;
		LONG			AdjPa;
		LONG			AdjQa;
		LONG			AdjPb;
		LONG			AdjQb;
		LONG			AdjPc;
		LONG			AdjQc;

		TRelElecValPar	Uap;
		TRelElecValPar	Ubp;
		TRelElecValPar	Ucp;
		TRelElecValPar	U0p;
		TRelElecValPar	U1p;
		TRelElecValPar	U2p;
		TRelElecValPar	U3p;
		TRelElecValPar	Iap;
		TRelElecValPar	Ibp;
		TRelElecValPar	Icp;
		TRelElecValPar	I0p;
		TRelElecValPar	I1p;
		

		LONG			MidAdjUa;
		LONG			MidAdjUb;
		LONG			MidAdjUc;
		LONG			MidAdjU0;
		LONG			MidAdjU1;
		LONG			MidAdjU2;
		LONG			MidAdjU3;		
		LONG			MidAdjIA;
		LONG			MidAdjIB;
		LONG			MidAdjIC;
		LONG			MidAdjI0;
		LONG			MidAdjI1;
		
		LONG			MidAdjUap;
		LONG			MidAdjUbp;
		LONG			MidAdjUcp;
		LONG			MidAdjU0p;
		LONG			MidAdjU1p;
		LONG			MidAdjU2p;
		LONG			MidAdjU3p;
		LONG			MidAdjIap;
		LONG			MidAdjIbp;
		LONG			MidAdjIcp;
		LONG			MidAdjI0p;
		LONG			MidAdjI1p;
		
		
		
//---------------������������鶨��------------------------

		// �ϴ�ң��ֵ��������
		
		LONG			UaArray[NUMOFYCVALUE];
		LONG			UbArray[NUMOFYCVALUE];
		LONG			UcArray[NUMOFYCVALUE];
		LONG			U0Array[NUMOFYCVALUE];
		LONG			U1Array[NUMOFYCVALUE];
		LONG			U2Array[NUMOFYCVALUE];
		LONG			U3Array[NUMOFYCVALUE];
		LONG			UabArray[NUMOFYCVALUE];
		LONG			UbcArray[NUMOFYCVALUE];
		LONG			UcaArray[NUMOFYCVALUE];
		LONG			IaArray[NUMOFYCVALUE];
		LONG			IbArray[NUMOFYCVALUE];
		LONG			IcArray[NUMOFYCVALUE];
		LONG			I0Array[NUMOFYCVALUE];
		LONG			I1Array[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			cosArray[NUMOFYCVALUE];
		

		//ϵ��У��������������

		
		LONG			AdjUaArray[NUMOFYCVALUE];
		LONG			AdjUbArray[NUMOFYCVALUE];
		LONG			AdjUcArray[NUMOFYCVALUE];
		LONG			AdjU0Array[NUMOFYCVALUE];
		LONG			AdjU1Array[NUMOFYCVALUE];
		LONG			AdjU2Array[NUMOFYCVALUE];
		LONG			AdjU3Array[NUMOFYCVALUE];		
		LONG			AdjIAArray[NUMOFYCVALUE];
		LONG			AdjIBArray[NUMOFYCVALUE];
		LONG			AdjICArray[NUMOFYCVALUE];
		LONG			AdjI0Array[NUMOFYCVALUE];
		LONG			AdjI1Array[NUMOFYCVALUE];
		
		LONG			UapArray[NUMOFYCVALUE];
		LONG			UbpArray[NUMOFYCVALUE];
		LONG			UcpArray[NUMOFYCVALUE];
		LONG			U0pArray[NUMOFYCVALUE];
		LONG			U1pArray[NUMOFYCVALUE];
		LONG			U2pArray[NUMOFYCVALUE];
		LONG			U3pArray[NUMOFYCVALUE];
		LONG			IapArray[NUMOFYCVALUE];
		LONG			IbpArray[NUMOFYCVALUE];
		LONG			IcpArray[NUMOFYCVALUE];
		LONG			I0pArray[NUMOFYCVALUE];
		LONG			I1pArray[NUMOFYCVALUE];
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[39];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//������ң������


//-----------------��ʱ����ֵ-------------------------
		LONG			UaDrift;
		LONG			UbDrift;
		LONG			UcDrift;
		LONG			U0Drift;
		LONG			U1Drift;
		LONG			U2Drift;
		LONG			U3Drift;
		LONG			IADrift;
		LONG			IBDrift;
		LONG			ICDrift;
		LONG			I0Drift;
		LONG			I1Drift;

		DWORD      dwMsgID;
		
		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){};
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);
		void Init(void);
		void CaluUIAngle(void);
		void WriteMSComposeBuf(LONG *ComposeBuf,const LONG*DataBuf);
		void CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ);//��������й����޹�
		void CoefAdjust(void);//ϵ��У��
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
		//----------CT���߼��-------------
		void CaluMaxMin(void);
		void InitCTACheck();
		void InitCTBCheck();		
		void InitCTCCheck();		
		//--------------------------------
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);

};
void DITask(DWORD *pdwApp);

#endif
