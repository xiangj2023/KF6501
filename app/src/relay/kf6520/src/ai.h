#ifndef _AI_H_
#define _AI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbfunc.h"
#include "parafunc.h"
#include "Relay_defs.h"

//����AI��ʱ��ʱ����Ϊ240���룬�޸�ʱ��ע������ܻ�Ӱ�쵽��Ƶ,�豣֤���¹�ϵ:AI_TIMER_PERIOD/20 > FREQBUFSIZE
//ң������ÿ20ms��һ�ܲ�����һ��
#define AI_TIMER_PERIOD_ONE			20
//�������ֵ�����������ȼ����
#define AI_TIMER_PERIOD_NUM			10
//�������ֵ����ļ��ʱ��
#define AI_TIMER_PERIOD			(AI_TIMER_PERIOD_ONE*AI_TIMER_PERIOD_NUM)	
//ÿ�β�����������Ҫ�����Ƶ�ʸ���
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)
//------------------------------------------------------------------
#define CTBRKPM				15				//CT���߼��Ip��Im������ϵ
#define CTBRCHECKVALUE 		1612		//CT���߼�ֵⷧ��0.05In��Ӧ�Ĳ���ֵ������
//------------------------------------------------------------------
class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:

		WORD 			wTaskScheduleCnt;//����������ȼ�����
		BOOL			m_bCaluFreqFlag;
//---------------��Ƶ����ر���------------------------------
		SHORT			*m_wFreqBuf;		
		LONG			*m_FreqCaluBuf;
		DWORD			m_dwFreqMask;
		LONG			m_dwFreq[NUM_FREQ_PER_AITASK];
		LONG			m_dwPreFreq;		
		BOOL			m_FreqFlag;
//-----------------------------------------------------------
		LONG			*m_pDataBuf;			//�������ݻ�����
		LONG			*m_pComposeBuf;			//�������ݺϳɻ�����
		LONG			*m_pCoefAdjustBuf;		//ϵ��У�����ݻ�����
//		LONG			*m_pComposeAdjustBuf;								
//---------------�仯ң����ز���-----------------------------
		LONG			*m_pRTBuf;
		DWORD			m_RTMask;

		LONG			m_U1Last;
		LONG			m_U2Last;
		LONG			m_ItLast;
		LONG			m_IfLast;
		LONG			m_U1WritePtr;
		LONG			m_U2WritePtr;
		LONG			m_ItWritePtr;
		LONG			m_IfWritePtr;
		TAbsTime		m_AbsTime;
		TEventAI		m_EventAI;
//------------------------------------------------------------
		
		DWORD			m_AIDataMask;			//����ͨ���õ���������
		DWORD			m_CoefAdjustMask;		//ϵ��У���õ�ͨ����������


		LONG			m_lDACoef;				//ϵ��У��ʱ�������õĽǲ�ϵ��
		DWORD			m_dwAdjustCnt;			//ϵ��У���������
		BOOL			m_bIsCoefAdjust;		//��ʼϵ��У����־
		BOOL			m_bIsSourceOK;			//Դ��У����־
		BOOL			m_bIsDriftAdjust;		//����ֵУ����־
		DWORD			m_dwWaitCnt;			//ϵ��У����ʼ�ȴ�����

		
		QWORD			m_OneKilowattP;			// 1�ȵ��������,�й�		
		QWORD			m_OneKilowattQ;			// 1�ȵ��������,�޹�
		QWORD			m_AccKilowattP;			//�ۼӵĹ���ֵ���й�
		QWORD			m_AccKilowattQ;			//�ۼӵĹ���ֵ���޹�
		//**************************CT���߼��****************			yanxs 2013-03-06
		TRelCTBrokPar	RelCTIt;
		TRelCTBrokPar	RelCTIf;
		LONG			CTCurSet;			//CT���߼������ż�ֵ
		LONG			CTTimeSet;			//CT���߸澯ʱ��
		LONG			m_It_p;					//CT���߼�Ᵽ������It
		LONG			m_If_p;
		LONG			m_ItMax;				//CT���߼��T��������
		LONG			m_ItMin;
		LONG			m_IfMax;
		LONG			m_IfMin;
		//***************************************************
		


//----------------ң��ֵ����-----------------------------
		LONG			m_Um;
		LONG			m_U1m;
		LONG			m_U2m;
		LONG			m_Im;
		LONG			m_Itm;
		LONG			m_Ifm;
		LONG			m_U3m;
		LONG			m_U4m;
		LONG			m_lMSP;				
		LONG			m_lMSQ;				
		LONG			m_lMSS;			
		LONG			m_lMSCOS;			
		TRelElecValPar		m_sI1;
		TRelElecValPar		m_sI2;
		TRelElecValPar		m_sI3;
		TRelElecValPar		m_sI5;
		TRelElecValPar		m_sI7;
		TRelElecValPar		m_sI9;
		TRelElecValPar		m_sI11;
		TRelElecValPar		m_Impd;
		LONG	 			m_Freq1;
		LONG				m_Freq2;
		
//--------------���ڽǶȼ���ı�������---------------------		
		TRelElecValPar		m_sIp;
		TRelElecValPar		m_sItm;
		TRelElecValPar		m_sIfm;
		TRelElecValPar		m_sUp;
		TRelElecValPar		m_sU1;
		TRelElecValPar		m_sU2;
		TRelElecValPar		m_sIt;
		TRelElecValPar		m_sIf;


//---------------����ϵ��У���ı�������-----------------
		LONG			m_AdjustUm;
		LONG			m_AdjustU1m;
		LONG			m_AdjustU2m;
		LONG			m_AdjustIm;
		LONG			m_AdjustItm;
		LONG			m_AdjustIfm;
		LONG			m_AdjustP;
		LONG			m_AdjustQ;
		LONG			m_AdjustCos;
		LONG			m_dwSourceU;		//ϵ��У��Դ��ѹ
		LONG			m_dwSourceI;		//ϵ��У��Դ����
		
		TRelElecValPar		m_AdjustIp;
		TRelElecValPar		m_AdjustIt;
		TRelElecValPar		m_AdjustIf;
		TRelElecValPar		m_AdjustUp;
		TRelElecValPar		m_AdjustU1;
		TRelElecValPar		m_AdjustU2;
		TRelElecValPar		m_AdjustIh1;

		LONG MedianIp;
		LONG MedianIt;
		LONG MedianIf;
		LONG MedianUp;
		LONG MedianU1;
		LONG MedianU2;
		LONG MedianIm;
		LONG MedianItm;
		LONG MedianIfm;
		LONG MedianUm;
		LONG MedianU1m;
		LONG MedianU2m;
		LONG MedianP;
		LONG MedianIh1;
		
		
//---------------������������鶨��------------------------

		// �ϴ�ң��ֵ��������
		LONG			UMArray[NUMOFYCVALUE];		
		LONG			U1Array[NUMOFYCVALUE];
		LONG			U2Array[NUMOFYCVALUE];		
		LONG			U3Array[NUMOFYCVALUE];
		LONG			U4Array[NUMOFYCVALUE];
		LONG			IMArray[NUMOFYCVALUE];
		LONG			ITArray[NUMOFYCVALUE];
		LONG			IFArray[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			COSArray[NUMOFYCVALUE];
		LONG			I1Array[NUMOFYCVALUE];
		LONG			I2Array[NUMOFYCVALUE];
		LONG			I3Array[NUMOFYCVALUE];
		LONG			I5Array[NUMOFYCVALUE];
		LONG			I7Array[NUMOFYCVALUE];
		LONG			I9Array[NUMOFYCVALUE];
		LONG			I11Array[NUMOFYCVALUE];
		LONG			RArray[NUMOFYCVALUE];
		LONG			XArray[NUMOFYCVALUE];
//		LONG			HArray[NUMOFYCVALUE];

		//ϵ��У��������������
		LONG			AdjustIp[NUMOFYCVALUE];
		LONG			AdjustIt[NUMOFYCVALUE];
		LONG			AdjustIf[NUMOFYCVALUE];
		LONG			AdjustUp[NUMOFYCVALUE];
		LONG			AdjustU1[NUMOFYCVALUE];
		LONG			AdjustU2[NUMOFYCVALUE];
		LONG			AdjustIm[NUMOFYCVALUE];
		LONG			AdjustItm[NUMOFYCVALUE];
		LONG			AdjustIfm[NUMOFYCVALUE];
		LONG			AdjustUm[NUMOFYCVALUE];
		LONG			AdjustU1m[NUMOFYCVALUE];
		LONG			AdjustU2m[NUMOFYCVALUE];
		LONG			AdjustP[NUMOFYCVALUE];
//		LONG			AdjustIh1[NUMOFYCVALUE];
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[15];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//������ң������


//-----------------��ʱ����ֵ-------------------------
		LONG      ItmDrift;
		LONG      IfmDrift;
		LONG      U1mDrift;
		LONG      U2mDrift;

		DWORD      dwMsgID;
		
		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){};
		void OnTimeOut(DWORD id);
		//void OnAutoAdjustStep1(DWORD arg);	
		//void OnAutoAdjustStep2(DWORD arg,DWORD Uval,DWORD Ival);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);
		void Init(void);
		
		void WriteMSComposeBuf();
		void CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ,LONG &MSCOS,BOOL coef = TRUE);//��������й����޹�
		void CoefAdjust(void);//ϵ��У��
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
		//***********************CT���߼��
		void InitCTIt(void);
		void InitCTIf(void);
		void CaluCTMaxMin(void);
		//*****************************

};
void DITask(DWORD *pdwApp);

#endif
