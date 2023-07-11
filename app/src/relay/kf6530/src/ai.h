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
		LONG			*m_pDataBuf;
		LONG			*m_pCoefAdjustBuf;									//ϵ��У�����ݻ�����		
		DWORD			m_CoefAdjustMask;									//ϵ��У���õ�ͨ����������
		DWORD			m_dwAdjustCnt;					//ϵ��У���������
		BOOL			m_bIsCoefAdjust;				//��ʼϵ��У����־
		BOOL			m_bIsHaveAdjustSource;			//�Ѿ�ԴУ����־
		DWORD			m_dwWaitCnt;					//ϵ��У����ʼ�ȴ�����


//        SHORT           *dwTestFBuf;		  //��Ƶ������
//        DWORD           dwTestFDataMask;	  //��Ƶ��������
//        DWORD           m_dwFpre; 			  //ǰ�β�Ƶ
//        DWORD           m_dwFnow;              //���β�Ƶֵ
//		DWORD           m_dwTs;                //���β�Ƶ��������

		LONG	Ua;
		LONG	Ub;
		LONG	Uc;
		LONG	IA;
		LONG	IB;
		LONG	IC;
		LONG	I1;
		LONG	I2;
		LONG	I3;
		LONG	I4;

		
		LONG	UaArray[NUMOFYCVALUE];
		LONG	UbArray[NUMOFYCVALUE];
		LONG	UcArray[NUMOFYCVALUE];
		LONG	IAArray[NUMOFYCVALUE];
		LONG	IBArray[NUMOFYCVALUE];
		LONG	ICArray[NUMOFYCVALUE];
		LONG	I1Array[NUMOFYCVALUE];
		LONG	I2Array[NUMOFYCVALUE];
		LONG	I3Array[NUMOFYCVALUE];
		LONG	I4Array[NUMOFYCVALUE];


//---------------����ϵ��У���ı�������-----------------
		LONG			m_dwSourceU;		//ϵ��У��Դ��ѹ
		LONG			m_dwSourceI;		//ϵ��У��Դ����

		TRelElecValPar		m_AdjustUA;
		TRelElecValPar		m_AdjustUB;
		TRelElecValPar		m_AdjustUC;
		TRelElecValPar		m_AdjustIHA;
		TRelElecValPar		m_AdjustIHB;
		TRelElecValPar		m_AdjustIHC;
		TRelElecValPar		m_AdjustIL1;
		TRelElecValPar		m_AdjustIL2;
		TRelElecValPar		m_AdjustIL3;
		TRelElecValPar		m_AdjustIL4;

		LONG MidUA;
		LONG MidUB;
		LONG MidUC;
		LONG MidIHA;
		LONG MidIHB;
		LONG MidIHC;
		LONG MidIL1;
		LONG MidIL2;
		LONG MidIL3;
		LONG MidIL4;

		BOOL m_bIsDriftAdjust;
		LONG	UaDrift;
		LONG	UbDrift;
		LONG	UcDrift;
		LONG	IADrift;
		LONG	IBDrift;
		LONG	ICDrift;
		LONG	I1Drift;
		LONG	I2Drift;
		LONG	I3Drift;
		LONG	I4Drift;

		LONG	UaM;
		LONG	UbM;
		LONG	UcM;
		LONG	IAM;
		LONG	IBM;
		LONG	ICM;
		LONG	I1M;
		LONG	I2M;
		LONG	I3M;
		LONG	I4M;

		LONG	MidUaM;
		LONG	MidUbM;
		LONG	MidUcM;
		LONG	MidIAM;
		LONG	MidIBM;
		LONG	MidICM;
		LONG	MidI1M;
		LONG	MidI2M;
		LONG	MidI3M;
		LONG	MidI4M;


		
//---------------������������鶨��------------------------


		//ϵ��У��������������
		LONG			AdjustUA[NUMOFYCVALUE];
		LONG			AdjustUB[NUMOFYCVALUE];
		LONG			AdjustUC[NUMOFYCVALUE];
		LONG			AdjustIHA[NUMOFYCVALUE];
		LONG			AdjustIHB[NUMOFYCVALUE];
		LONG			AdjustIHC[NUMOFYCVALUE];		
		LONG			AdjustIL1[NUMOFYCVALUE];
		LONG			AdjustIL2[NUMOFYCVALUE];
		LONG			AdjustIL3[NUMOFYCVALUE];
		LONG			AdjustIL4[NUMOFYCVALUE];

		LONG			AdjustUaM[NUMOFYCVALUE];
		LONG			AdjustUbM[NUMOFYCVALUE];
		LONG			AdjustUcM[NUMOFYCVALUE];
		LONG			AdjustIAM[NUMOFYCVALUE];
		LONG			AdjustIBM[NUMOFYCVALUE];
		LONG			AdjustICM[NUMOFYCVALUE];
		LONG			AdjustI1M[NUMOFYCVALUE];
		LONG			AdjustI2M[NUMOFYCVALUE];
		LONG			AdjustI3M[NUMOFYCVALUE];
		LONG			AdjustI4M[NUMOFYCVALUE];
		
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[30];		//���ڱ���ϵ�����ۼ�ֵ
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
