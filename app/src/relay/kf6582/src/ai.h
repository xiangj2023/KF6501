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
//---------------��Ƶ����ر���------------------------------

		SHORT *m_wFreqBuf;		
		LONG  *m_FreqCaluBuf;
		DWORD m_dwFreqMask;
		LONG m_dwFreq[NUM_FREQ_PER_AITASK];
		DWORD m_dwPreFreq;		
		BOOL 			m_FreqFlag;

//------------------ң��������ñ���------------------------------------
		LONG			*m_pDataBuf;	//�������ݻ�����	
		DWORD			m_AIDataMask;	//����ͨ���õ���������
		
		LONG			m_U1;
		LONG			m_U2;
		LONG			m_U3;
		LONG			m_U4;
		LONG			m_U5;
		LONG			m_U6;
		LONG			m_U7;
		LONG			m_U8;
		LONG			m_I1;
		LONG			m_I2;
		LONG			m_I3;
		LONG			m_I4;
		LONG			m_I5;
		LONG			m_I6;
		LONG			m_I7;
		LONG			m_I8;
		
		LONG 			UM1Array[NUMOFYCVALUE];
		LONG 			UM2Array[NUMOFYCVALUE];
		LONG 			UM3Array[NUMOFYCVALUE];
		LONG 			UM4Array[NUMOFYCVALUE];
		LONG 			UM5Array[NUMOFYCVALUE];
		LONG 			UM6Array[NUMOFYCVALUE];
		LONG 			UM7Array[NUMOFYCVALUE];
		LONG 			UM8Array[NUMOFYCVALUE];
		LONG			IM1Array[NUMOFYCVALUE];
		LONG			IM2Array[NUMOFYCVALUE];
		LONG			IM3Array[NUMOFYCVALUE];
		LONG			IM4Array[NUMOFYCVALUE];
		LONG			IM5Array[NUMOFYCVALUE];
		LONG			IM6Array[NUMOFYCVALUE];
		LONG			IM7Array[NUMOFYCVALUE];
		LONG			IM8Array[NUMOFYCVALUE];

//------------------ϵ��У���ñ���---------------------------------------------
		// 1.��־����
		LONG			*m_pCoefAdjustBuf;			//ϵ��У���������ݻ�����
		DWORD			m_dwSourceU;				//ϵ��У����ѹ
		DWORD			m_dwSourceI;				//ϵ��У������
		DWORD			m_dwAdjustCnt;				//ϵ��У���������		
		DWORD			m_dwWaitCnt;				//ϵ��У����ȴ��Ĵ���
		BOOL			m_bIsCoefAdjust;			//��ʼϵ��У����־		
		BOOL			m_bIsDriftAdjust;			//����ֵУ����־
		BOOL			m_bIsSourceOK;				//ԴУ����־
		// 2.����ֵ����
		LONG			m_lDriftU1;
		LONG			m_lDriftU2;
		LONG			m_lDriftU3;
		LONG			m_lDriftU4;
		LONG			m_lDriftU5;
		LONG			m_lDriftU6;
		LONG			m_lDriftU7;
		LONG			m_lDriftU8;
		LONG			m_lDriftI1;
		LONG			m_lDriftI2;
		LONG			m_lDriftI3;
		LONG			m_lDriftI4;
		LONG			m_lDriftI5;
		LONG			m_lDriftI6;
		LONG			m_lDriftI7;
		LONG			m_lDriftI8;
		// 3.����ð��������У�����
		LONG			m_lMidU1p;
		LONG			m_lMidU2p;
		LONG			m_lMidU3p;
		LONG			m_lMidU4p;
		LONG			m_lMidU5p;
		LONG			m_lMidU6p;
		LONG			m_lMidU7p;
		LONG			m_lMidU8p;
		LONG			m_lMidI1p;
		LONG			m_lMidI2p;
		LONG			m_lMidI3p;
		LONG			m_lMidI4p;
		LONG			m_lMidI5p;
		LONG			m_lMidI6p;
		LONG			m_lMidI7p;
		LONG			m_lMidI8p;
		
		LONG			m_lMidU1m;
		LONG			m_lMidU2m;
		LONG			m_lMidU3m;
		LONG			m_lMidU4m;
		LONG			m_lMidU5m;
		LONG			m_lMidU6m;
		LONG			m_lMidU7m;
		LONG			m_lMidU8m;
		LONG			m_lMidI1m;
		LONG			m_lMidI2m;
		LONG			m_lMidI3m;
		LONG			m_lMidI4m;
		LONG			m_lMidI5m;
		LONG			m_lMidI6m;
		LONG			m_lMidI7m;
		LONG			m_lMidI8m;
		// 4.����������������
		LONG			AArrayU1p[NUMOFYCVALUE];		
		LONG			AArrayU2p[NUMOFYCVALUE];
		LONG			AArrayU3p[NUMOFYCVALUE];
		LONG			AArrayU4p[NUMOFYCVALUE];
		LONG			AArrayU5p[NUMOFYCVALUE];
		LONG			AArrayU6p[NUMOFYCVALUE];
		LONG			AArrayU7p[NUMOFYCVALUE];
		LONG			AArrayU8p[NUMOFYCVALUE];
		LONG			AArrayI1p[NUMOFYCVALUE];
		LONG			AArrayI2p[NUMOFYCVALUE];		
		LONG			AArrayI3p[NUMOFYCVALUE];
		LONG			AArrayI4p[NUMOFYCVALUE];
		LONG			AArrayI5p[NUMOFYCVALUE];
		LONG			AArrayI6p[NUMOFYCVALUE];
		LONG			AArrayI7p[NUMOFYCVALUE];
		LONG			AArrayI8p[NUMOFYCVALUE];
		
		LONG			AArrayU1m[NUMOFYCVALUE];		
		LONG			AArrayU2m[NUMOFYCVALUE];
		LONG			AArrayU3m[NUMOFYCVALUE];
		LONG			AArrayU4m[NUMOFYCVALUE];
		LONG			AArrayU5m[NUMOFYCVALUE];
		LONG			AArrayU6m[NUMOFYCVALUE];
		LONG			AArrayU7m[NUMOFYCVALUE];
		LONG			AArrayU8m[NUMOFYCVALUE];
		LONG			AArrayI1m[NUMOFYCVALUE];
		LONG			AArrayI2m[NUMOFYCVALUE];		
		LONG			AArrayI3m[NUMOFYCVALUE];
		LONG			AArrayI4m[NUMOFYCVALUE];
		LONG			AArrayI5m[NUMOFYCVALUE];
		LONG			AArrayI6m[NUMOFYCVALUE];
		LONG			AArrayI7m[NUMOFYCVALUE];
		LONG			AArrayI8m[NUMOFYCVALUE];
		//δ����ð������ļ�����		
		LONG			m_AdjustU1m;
		LONG			m_AdjustU2m;
		LONG			m_AdjustU3m;
		LONG			m_AdjustU4m;
		LONG			m_AdjustU5m;
		LONG			m_AdjustU6m;
		LONG			m_AdjustU7m;
		LONG			m_AdjustU8m;
		LONG			m_AdjustI1m;
		LONG			m_AdjustI2m;
		LONG			m_AdjustI3m;
		LONG			m_AdjustI4m;
		LONG			m_AdjustI5m;
		LONG			m_AdjustI6m;
		LONG			m_AdjustI7m;
		LONG			m_AdjustI8m;
		
		TRelElecValPar			m_AdjustU1p;
		TRelElecValPar			m_AdjustU2p;
		TRelElecValPar			m_AdjustU3p;
		TRelElecValPar			m_AdjustU4p;
		TRelElecValPar			m_AdjustU5p;
		TRelElecValPar			m_AdjustU6p;
		TRelElecValPar			m_AdjustU7p;
		TRelElecValPar			m_AdjustU8p;
		TRelElecValPar			m_AdjustI1p;
		TRelElecValPar			m_AdjustI2p;
		TRelElecValPar			m_AdjustI3p;
		TRelElecValPar			m_AdjustI4p;
		TRelElecValPar			m_AdjustI5p;
		TRelElecValPar			m_AdjustI6p;
		TRelElecValPar			m_AdjustI7p;
		TRelElecValPar			m_AdjustI8p;
		
		LONG			m_pAdjustData[32];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//������ң������

		
		DWORD      dwMsgID;
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
	//	void OnAutoAdjustStep1(DWORD arg);	
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
//		void RefreshCommRW();		
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
		
//		void WriteMSComposeBuf(LONG *ComposeBuf,LONG*DataBuf);
		void CoefAdjust(void);//ϵ��У��		
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
};
void DITask(DWORD *pdwApp);

#endif
