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

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
		LONG			*m_pCoefAdjustBuf;									//ϵ��У�����ݻ�����
		LONG			*m_pDataBuf;										//�������ݻ�����
		LONG			*m_pComposeBuf;										//�Ͳ�ϳɵ���������
		DWORD			m_CoefAdjustMask;									//ϵ��У���õ�ͨ����������
		DWORD			m_dwAdjustCnt;					//ϵ��У���������
		BOOL			m_bIsCoefAdjust;				//��ʼϵ��У����־
		BOOL			m_bIsHaveAdjustSource;			//�Ѿ�ԴУ����־
		DWORD			m_dwWaitCnt;					//ϵ��У����ʼ�ȴ�����

		LONG			Uha;
		LONG			Uhb;
		LONG			Uhc;
		LONG			U0;
		LONG			Ula;
		LONG			Ulb;
		LONG			Iha;
		LONG			Ihb;
		LONG			Ihc;
		LONG			I0;
		LONG			I1;
		LONG			I2;
		LONG			I3;
		LONG			I4;
		LONG			Ijx;
		LONG			Ila;
		LONG			Ilb;

//---------------����ϵ��У���ı�������-----------------
		LONG			m_dwSourceU;		//ϵ��У��Դ��ѹ
		LONG			m_dwSourceI;		//ϵ��У��Դ����
		
		TRelElecValPar		m_AdjustUHA;
		TRelElecValPar		m_AdjustUHB;
		TRelElecValPar		m_AdjustUHC;
		TRelElecValPar		m_AdjustU0;
		TRelElecValPar		m_AdjustULA;
		TRelElecValPar		m_AdjustULB;
		TRelElecValPar		m_AdjustIHA;		
		TRelElecValPar		m_AdjustIHB;
		TRelElecValPar		m_AdjustIHC;
		TRelElecValPar		m_AdjustI0;
		TRelElecValPar		m_AdjustIL1;
		TRelElecValPar		m_AdjustIL2;
		TRelElecValPar		m_AdjustIL3;
		TRelElecValPar		m_AdjustIL4;


		LONG MidUHA;
		LONG MidUHB;
		LONG MidUHC;
		LONG MidU0;
		LONG MidULA;
		LONG MidULB;
		LONG MidIHA;
		LONG MidIHB;
		LONG MidIHC;
		LONG MidI0;
		LONG MidIL1;
		LONG MidIL2;
		LONG MidIL3;
		LONG MidIL4;
		
		BOOL 	m_bIsDriftAdjust;
		LONG	UHADrift;
		LONG	UHBDrift;
		LONG	UHCDrift;
		LONG	U0Drift;
		LONG	ULADrift;
		LONG	ULBDrift;
		LONG	IHADrift;
		LONG	IHBDrift;
		LONG	IHCDrift;
		LONG	I0Drift;
		LONG	IL1Drift;
		LONG	IL2Drift;
		LONG	IL3Drift;
		LONG	IL4Drift;

		LONG 	MidUHAM;
		LONG 	MidUHBM;
		LONG 	MidUHCM;
		LONG 	MidU0M;
		LONG 	MidULAM;
		LONG 	MidULBM;
		LONG 	MidIHAM;
		LONG 	MidIHBM;
		LONG 	MidIHCM;
		LONG 	MidI0M;
		LONG 	MidIL1M;
		LONG 	MidIL2M;
		LONG 	MidIL3M;
		LONG 	MidIL4M;
		
		LONG 	m_adjUHAM;
		LONG 	m_adjUHBM;
		LONG 	m_adjUHCM;
		LONG 	m_adjU0M;
		LONG 	m_adjULAM;
		LONG 	m_adjULBM;
		LONG 	m_adjIHAM;
		LONG 	m_adjIHBM;
		LONG 	m_adjIHCM;
		LONG 	m_adjI0M;
		LONG 	m_adjIL1M;
		LONG 	m_adjIL2M;
		LONG 	m_adjIL3M;
		LONG 	m_adjIL4M;
		
//---------------������������鶨��------------------------


		//ϵ��У��������������
		LONG			AdjustUHA[NUMOFYCVALUE];
		LONG			AdjustUHB[NUMOFYCVALUE];
		LONG			AdjustUHC[NUMOFYCVALUE];
		LONG			AdjustU0[NUMOFYCVALUE];
		LONG			AdjustULA[NUMOFYCVALUE];
		LONG			AdjustULB[NUMOFYCVALUE];
		LONG			AdjustIHA[NUMOFYCVALUE];
		LONG			AdjustIHB[NUMOFYCVALUE];
		LONG			AdjustIHC[NUMOFYCVALUE];
		LONG			AdjustI0[NUMOFYCVALUE];
		LONG			AdjustIL1[NUMOFYCVALUE];
		LONG			AdjustIL2[NUMOFYCVALUE];
		LONG			AdjustIL3[NUMOFYCVALUE];
		LONG			AdjustIL4[NUMOFYCVALUE];

		
		LONG			AdjustUHAM[NUMOFYCVALUE];
		LONG			AdjustUHBM[NUMOFYCVALUE];
		LONG			AdjustUHCM[NUMOFYCVALUE];
		LONG			AdjustU0M[NUMOFYCVALUE];
		LONG			AdjustULAM[NUMOFYCVALUE];
		LONG			AdjustULBM[NUMOFYCVALUE];
		LONG			AdjustIHAM[NUMOFYCVALUE];
		LONG			AdjustIHBM[NUMOFYCVALUE];
		LONG			AdjustIHCM[NUMOFYCVALUE];
		LONG			AdjustI0M[NUMOFYCVALUE];
		LONG			AdjustIL1M[NUMOFYCVALUE];
		LONG			AdjustIL2M[NUMOFYCVALUE];
		LONG			AdjustIL3M[NUMOFYCVALUE];
		LONG			AdjustIL4M[NUMOFYCVALUE];


		//��������������ң��ֵ����������
		
		LONG			ArrayUHAM[NUMOFYCVALUE];
		LONG			ArrayUHBM[NUMOFYCVALUE];
		LONG			ArrayUHCM[NUMOFYCVALUE];
		LONG			ArrayU0M[NUMOFYCVALUE];
		LONG			ArrayULAM[NUMOFYCVALUE];
		LONG			ArrayULBM[NUMOFYCVALUE];
		LONG			ArrayIHAM[NUMOFYCVALUE];
		LONG			ArrayIHBM[NUMOFYCVALUE];
		LONG			ArrayIHCM[NUMOFYCVALUE];
		LONG			ArrayI0M[NUMOFYCVALUE];
		LONG			ArrayIL1M[NUMOFYCVALUE];
		LONG			ArrayIL2M[NUMOFYCVALUE];
		LONG			ArrayIL3M[NUMOFYCVALUE];
		LONG			ArrayIL4M[NUMOFYCVALUE];
		LONG			ArrayILAM[NUMOFYCVALUE];
		LONG			ArrayILBM[NUMOFYCVALUE];
		LONG			ArrayIJXM[NUMOFYCVALUE];
//---------------------------------------------------------		
		LONG			m_pAdjustData[42];		//���ڱ���ϵ�����ۼ�ֵ
		LONG			m_pBubbleData[NUMOFYCVALUE];	//������ң������



		DWORD      dwMsgID;
		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){};
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
		
		void CoefAdjust(void);//ϵ��У��
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		LONG CoefAdjustSort(LONG Val,LONG* OriginalData);		
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);

};
void DITask(DWORD *pdwApp);

#endif
