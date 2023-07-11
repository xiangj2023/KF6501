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
		LONG	U0;
		LONG	IHA;
		LONG	IHB;
		LONG	IHC;
		LONG	IMA;
		LONG	IMB;
		LONG	IMC;
		LONG	ILA;
		LONG	ILB;
		LONG	ILC;
		LONG	I0;
		LONG	IJX;

		
		LONG	UaArray[NUMOFYCVALUE];
		LONG	UbArray[NUMOFYCVALUE];
		LONG	UcArray[NUMOFYCVALUE];
		LONG	U0Array[NUMOFYCVALUE];
		LONG	IHAArray[NUMOFYCVALUE];
		LONG	IHBArray[NUMOFYCVALUE];
		LONG	IHCArray[NUMOFYCVALUE];
		LONG	IMAArray[NUMOFYCVALUE];
		LONG	IMBArray[NUMOFYCVALUE];
		LONG	IMCArray[NUMOFYCVALUE];
		LONG	ILAArray[NUMOFYCVALUE];
		LONG	ILBArray[NUMOFYCVALUE];
		LONG	ILCArray[NUMOFYCVALUE];
		LONG	I0Array[NUMOFYCVALUE];
		LONG	IJXArray[NUMOFYCVALUE];


//---------------����ϵ��У���ı�������-----------------
		LONG			m_dwSourceU;		//ϵ��У��Դ��ѹ
		LONG			m_dwSourceI;		//ϵ��У��Դ����

		TRelElecValPar		m_AdjustUA;
		TRelElecValPar		m_AdjustUB;
		TRelElecValPar		m_AdjustUC;
		TRelElecValPar		m_AdjustU0;
		TRelElecValPar		m_AdjustIHA;
		TRelElecValPar		m_AdjustIHB;
		TRelElecValPar		m_AdjustIHC;
		TRelElecValPar		m_AdjustIMA;
		TRelElecValPar		m_AdjustIMB;
		TRelElecValPar		m_AdjustIMC;
		TRelElecValPar		m_AdjustILA;
		TRelElecValPar		m_AdjustILB;
		TRelElecValPar		m_AdjustILC;
		TRelElecValPar		m_AdjustI0;
		TRelElecValPar		m_AdjustIJX;

		LONG MidUA;
		LONG MidUB;
		LONG MidUC;
		LONG MidU0;
		LONG MidIHA;
		LONG MidIHB;
		LONG MidIHC;
		LONG MidIMA;
		LONG MidIMB;
		LONG MidIMC;
		LONG MidILA;
		LONG MidILB;
		LONG MidILC;
		LONG MidI0;
		LONG MidIJX;

		BOOL m_bIsDriftAdjust;
		LONG	UaDrift;
		LONG	UbDrift;
		LONG	UcDrift;
		LONG	U0Drift;
		LONG	IHADrift;
		LONG	IHBDrift;
		LONG	IHCDrift;
		LONG	IMADrift;
		LONG	IMBDrift;
		LONG	IMCDrift;
		LONG	ILADrift;
		LONG	ILBDrift;
		LONG	ILCDrift;
		LONG	I0Drift;
		LONG	IJXDrift;

		LONG	UaM;
		LONG	UbM;
		LONG	UcM;
		LONG	U0M;
		LONG	IHAM;
		LONG	IHBM;
		LONG	IHCM;
		LONG	IMAM;
		LONG	IMBM;
		LONG	IMCM;
		LONG	ILAM;
		LONG	ILBM;
		LONG	ILCM;
		LONG	I0M;
		LONG	IJXM;

		LONG	MidUaM;
		LONG	MidUbM;
		LONG	MidUcM;
		LONG	MidU0M;
		LONG	MidIHAM;
		LONG	MidIHBM;
		LONG	MidIHCM;
		LONG	MidIMAM;
		LONG	MidIMBM;
		LONG	MidIMCM;
		LONG	MidILAM;
		LONG	MidILBM;
		LONG	MidILCM;
		LONG	MidI0M;
		LONG	MidIJXM;


		
//---------------������������鶨��------------------------


		//ϵ��У��������������
		LONG			AdjustUA[NUMOFYCVALUE];
		LONG			AdjustUB[NUMOFYCVALUE];
		LONG			AdjustUC[NUMOFYCVALUE];
		LONG			AdjustU0[NUMOFYCVALUE];
		LONG			AdjustIHA[NUMOFYCVALUE];
		LONG			AdjustIHB[NUMOFYCVALUE];
		LONG			AdjustIHC[NUMOFYCVALUE];		
		LONG			AdjustIMA[NUMOFYCVALUE];
		LONG			AdjustIMB[NUMOFYCVALUE];
		LONG			AdjustIMC[NUMOFYCVALUE];
		LONG			AdjustILA[NUMOFYCVALUE];
		LONG			AdjustILB[NUMOFYCVALUE];
		LONG			AdjustILC[NUMOFYCVALUE];
		LONG			AdjustI0[NUMOFYCVALUE];
		LONG			AdjustIJX[NUMOFYCVALUE];

		LONG			AdjustUaM[NUMOFYCVALUE];
		LONG			AdjustUbM[NUMOFYCVALUE];
		LONG			AdjustUcM[NUMOFYCVALUE];
		LONG			AdjustU0M[NUMOFYCVALUE];
		LONG			AdjustIHAM[NUMOFYCVALUE];
		LONG			AdjustIHBM[NUMOFYCVALUE];
		LONG			AdjustIHCM[NUMOFYCVALUE];
		LONG			AdjustIMAM[NUMOFYCVALUE];
		LONG			AdjustIMBM[NUMOFYCVALUE];
		LONG			AdjustIMCM[NUMOFYCVALUE];
		LONG			AdjustILAM[NUMOFYCVALUE];
		LONG			AdjustILBM[NUMOFYCVALUE];
		LONG			AdjustILCM[NUMOFYCVALUE];
		LONG			AdjustI0M[NUMOFYCVALUE];
		LONG			AdjustIJXM[NUMOFYCVALUE];
		
		
//---------------------------------------------------------		
		LONG			m_pAdjustData[45];		//���ڱ���ϵ�����ۼ�ֵ
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

};
void DITask(DWORD *pdwApp);

#endif
