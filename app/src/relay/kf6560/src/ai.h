#ifndef _AI_H_
#define _AI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbasedef.h"
#include "dbfunc.h"
#include "parafunc.h"
#include "db_err.h"
#include "Relay_defs.h"


//����AI��ʱ��ʱ����Ϊ240���룬�޸�ʱ��ע������ܻ�Ӱ�쵽��Ƶ,�豣֤���¹�ϵ:AI_TIMER_PERIOD/20 > FREQBUFSIZE
#define AI_TIMER_PERIOD			200	
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
//==============ң������õı�������=====================================
		LONG					*m_pDataBuf;										//�������ݻ�����
		LONG					*m_pComposeBuf;
		DWORD					m_AIDataMask;										//����ͨ���õ���������
		LONG					m_Um;
		LONG					m_U1m;
		LONG					m_U2m;
		
		LONG					m_Ud1m;
		LONG					m_Ud2m;
		LONG					m_Ud3m;
		LONG					m_Im;
		LONG					m_I1m;
		LONG					m_I2m;
		LONG					m_I3m;		
		LONG					m_lMSP;				//�й�
		LONG					m_lMSQ;				//�޹�
		LONG					m_lMSS;				//���ڹ���
		LONG					m_lMSCOS;			//��������
		TRelElecValPar			m_Ih1m;
		TRelElecValPar			m_Ih2m;
		TRelElecValPar			m_Ih3m;
		TRelElecValPar			m_Ih5m;
		TRelElecValPar			m_Ih7m;
		TRelElecValPar			m_Ih9m;
		TRelElecValPar			m_Ih11m;
		//**************************CT���߼��****************			yanxs 2013-03-06
		TRelCTBrokPar			RelCTI;
		LONG					CTCurSet;			//CT���߼������ż�ֵ
		LONG					CTTimeSet;			//CT���߸澯ʱ��
		LONG					m_Ip;				//CT���߼�Ᵽ������It
		LONG					m_IMax;				//CT���߼��T��������
		LONG					m_IMin;
		//***************************************************
		
		LONG			UMArray[NUMOFYCVALUE];
		LONG			U1MArray[NUMOFYCVALUE];
		LONG			U2MArray[NUMOFYCVALUE];
		LONG			Ud1MArray[NUMOFYCVALUE];
		LONG			Ud2MArray[NUMOFYCVALUE];
		LONG			Ud3MArray[NUMOFYCVALUE];
		LONG			IMArray[NUMOFYCVALUE];
		LONG			I1MArray[NUMOFYCVALUE];
		LONG			I2MArray[NUMOFYCVALUE];
		LONG			I3MArray[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			COSArray[NUMOFYCVALUE];
		LONG			IH1MArray[NUMOFYCVALUE];
		LONG			IH2MArray[NUMOFYCVALUE];
		LONG			IH3MArray[NUMOFYCVALUE];
		LONG			IH5MArray[NUMOFYCVALUE];
		LONG			IH7MArray[NUMOFYCVALUE];
		LONG			IH9MArray[NUMOFYCVALUE];
		LONG			IH11MArray[NUMOFYCVALUE];


//=============ϵ��У���õı���===========================================
		LONG			*m_pCoefAdjustBuf;			//ϵ��У�����ݻ�����		
		DWORD			m_CoefAdjustMask;			//ϵ��У���õ�ͨ����������
		DWORD			m_dwAdjustCnt;				//ϵ��У���������
		DWORD			m_dwWaitCnt;				//ϵ��У����ȴ��Ĵ���
		BOOL			m_bIsCoefAdjust;			//��ʼϵ��У����־
		BOOL			m_bHaveAdjustAngle;			//�ǲ���У����־
		BOOL			m_bIsDriftAdjust;			//����ֵУ����־
  // 1.У��������ı���ֵ
		LONG			m_AdjustUm;
		LONG			m_AdjustU1m;
		LONG			m_AdjustU2m;
		LONG			m_AdjustUd1m;
		LONG			m_AdjustUd2m;
		LONG			m_AdjustUd3m;
		LONG			m_AdjustIm;
		LONG			m_AdjustI1m;
		LONG			m_AdjustI2m;
		LONG			m_AdjustI3m;
		LONG			m_AdjustP;
		LONG			m_AdjustQ;			
		LONG			m_lDACoef;	//ϵ��У��ʱ�������õĽǲ�ϵ��
		TRelElecValPar	m_AdjustUp;
		TRelElecValPar	m_AdjustU1p;
		TRelElecValPar	m_AdjustU2p;
		TRelElecValPar	m_AdjustUd1p;
		TRelElecValPar	m_AdjustUd2p;
		TRelElecValPar	m_AdjustUd3p;
		TRelElecValPar	m_AdjustIp;
		TRelElecValPar	m_AdjustI1p;
		TRelElecValPar	m_AdjustI2p;
		TRelElecValPar	m_AdjustI3p;
 // 2.��ͨ������ֵ����		
		LONG			m_lDriftU;
		LONG			m_lDriftU1;
 		LONG			m_lDriftU2;
		LONG			m_lDriftUd1;
		LONG			m_lDriftUd2;
		LONG			m_lDriftUd3;
		LONG			m_lDriftIm;
		LONG			m_lDriftI1;
		LONG			m_lDriftI2;
		LONG			m_lDriftI3;
 // 3.ϵ��У����Դ��ѹ������ֵ
		DWORD			m_dwSourceU;
		DWORD			m_dwSourceI;
 // 4.ϵ��У���и������Ĵ��ۼ���ֵ--����ð��������
		LONG			m_lMidUp;
		LONG			m_lMidU1p;
		LONG			m_lMidU2p;
		LONG			m_lMidUd1p;
		LONG			m_lMidUd2p;
		LONG			m_lMidUd3p;
		LONG			m_lMidIp;
		LONG			m_lMidI1p;
		LONG			m_lMidI2p;
		LONG			m_lMidI3p;
		LONG			m_lMidUm;
		LONG			m_lMidU1m;
		LONG			m_lMidU2m;
		LONG			m_lMidUd1m;
		LONG			m_lMidUd2m;
		LONG			m_lMidUd3m;
		LONG			m_lMidIm;
		LONG			m_lMidI1m;
		LONG			m_lMidI2m;
		LONG			m_lMidI3m;
		LONG			m_lMidP;
		LONG			m_lMidQ;
 // 5.ϵ��У������������
		LONG			AArrayUp[NUMOFYCVALUE];
		LONG			AArrayU1p[NUMOFYCVALUE];
		LONG			AArrayU2p[NUMOFYCVALUE];		
		LONG			AArrayUd1p[NUMOFYCVALUE];
		LONG			AArrayUd2p[NUMOFYCVALUE];
		LONG			AArrayUd3p[NUMOFYCVALUE];
		LONG			AArrayIp[NUMOFYCVALUE];
		LONG			AArrayI1p[NUMOFYCVALUE];
		LONG			AArrayI2p[NUMOFYCVALUE];
		LONG			AArrayI3p[NUMOFYCVALUE];
		LONG			AArrayUm[NUMOFYCVALUE];
		LONG			AArrayU1m[NUMOFYCVALUE];
		LONG			AArrayU2m[NUMOFYCVALUE];		
		LONG			AArrayUd1m[NUMOFYCVALUE];
		LONG			AArrayUd2m[NUMOFYCVALUE];
		LONG			AArrayUd3m[NUMOFYCVALUE];
		LONG			AArrayIm[NUMOFYCVALUE];
		LONG			AArrayI1m[NUMOFYCVALUE];
		LONG			AArrayI2m[NUMOFYCVALUE];
		LONG			AArrayI3m[NUMOFYCVALUE];
		LONG			AArrayP[NUMOFYCVALUE];
		LONG			AArrayQ[NUMOFYCVALUE];
 // 6.ϵ��У���и�ϵ�����ۼ���				
		LONG			m_pAdjustData[22];	 

 
		LONG			m_pBubbleData[NUMOFYCVALUE];	//������ң������

		DWORD      dwMsgID;
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
//		void OnAutoAdjustStep1(DWORD arg);			
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);		
		void Init(void);
		
		void CaluMSPQ(LONG *UDataBuf,LONG *IDataBuf,LONG DACoef, LONG &MSP,LONG &MSQ,LONG &MSCOS,BOOL coef = TRUE);//��������й����޹�
		void CoefAdjust(void);//ϵ��У��
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		LONG AiBubbleSort(LONG Val,LONG OriginalData[NUMOFYCVALUE]);		
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void WriteMSComposeData(LONG *caludatabuf,LONG *originaldatabuf);
		//***********************CT���߼��
		void InitCTI(void);
		void CaluCTMaxMin(void);
		//*****************************
//		void RefreshCommRW();		
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
};
void DITask(DWORD *pdwApp);

#endif
