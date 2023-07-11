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



		
		
		
//------------------ϵ��У���ñ���---------------------------------------------

		TRelElecValPar		U1;
		TRelElecValPar		U2;
		TRelElecValPar		U3;
		TRelElecValPar		U4;
		TRelElecValPar		IT1;
		TRelElecValPar		IF1;
		TRelElecValPar		IT2;
		TRelElecValPar		IF2;
		TRelElecValPar		IT3;
		TRelElecValPar		IF3;
		TRelElecValPar		IAT1;
		TRelElecValPar		IAT2;

		LONG				midU1;
		LONG				midU2;
		LONG				midU3;
		LONG				midU4;
		LONG				midIT1;
		LONG				midIF1;
		LONG				midIT2;
		LONG				midIF2;
		LONG				midIT3;
		LONG				midIF3;
		LONG				midIAT1;
		LONG				midIAT2;
		
		LONG				m_pAdjustData[12];
		
		LONG				AdjustU1[NUMOFYCVALUE];
		LONG				AdjustU2[NUMOFYCVALUE];
		LONG				AdjustU3[NUMOFYCVALUE];
		LONG				AdjustU4[NUMOFYCVALUE];
		LONG				AdjustIT1[NUMOFYCVALUE];
		LONG				AdjustIF1[NUMOFYCVALUE];
		LONG				AdjustIT2[NUMOFYCVALUE];
		LONG				AdjustIF2[NUMOFYCVALUE];
		LONG				AdjustIT3[NUMOFYCVALUE];
		LONG				AdjustIF3[NUMOFYCVALUE];
		LONG				AdjustIAT1[NUMOFYCVALUE];
		LONG				AdjustIAT2[NUMOFYCVALUE];
		


	// 1.��־����
		DWORD			m_CoefAdjustDataMask;		//ͨ������
		LONG			*m_pCoefAdjustBuf;			//ϵ��У���������ݻ�����
		DWORD			m_dwSourceU;				//ϵ��У����ѹ
		DWORD			m_dwSourceI;				//ϵ��У������
		DWORD			m_dwAdjustCnt;				//ϵ��У���������		
		DWORD			m_dwWaitCnt;				//ϵ��У����ȴ��Ĵ���
		BOOL			m_bIsCoefAdjust;			//��ʼϵ��У����־		
		BOOL			m_bIsSourceOK;				//ԴУ����־
		LONG			m_pBubbleData[NUMOFYCVALUE];	//������ң������

		DWORD      dwMsgID;

		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
		
		LONG CoefAdjustSort(LONG Val,LONG* OriginalData);
		void WriteMSComposeBuf(LONG *ComposeBuf,LONG*DataBuf);
		void CoefAdjust(void);//ϵ��У��		
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
};
void DITask(DWORD *pdwApp);

#endif
