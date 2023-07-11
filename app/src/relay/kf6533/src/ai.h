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



#define AI_TIMER_PERIOD			200		//����AI��ʱ��ʱ����Ϊ200����
//ÿ�β�����������Ҫ�����Ƶ�ʸ���
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)


#define NORMAL_AI_TIME_CNT		(100/AI_TIMER_PERIOD)

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
		LONG			*m_pDataBuf;										//�������ݻ�����
		LONG			*m_pCoefAdjustBuf;									//ϵ��У�����ݻ�����
		LONG			*m_pComposeAdjustBuf;								
		
		DWORD			m_AIDataMask;										//����ͨ���õ���������
		DWORD			m_CoefAdjustMask;									//ϵ��У���õ�ͨ����������

		DWORD			m_dwAdjustCounter;				//ϵ��У���������
		BOOL			m_bIsCoefAdjust;				//��ʼϵ��У����־
        BOOL			m_bIsDeadValAdjust;				//����ֵУ����־
		WORD			wCoefAdjustNum;					//ϵ��У������
		WORD			wWaitTimeNum;					//ϵ��У����ʼ�ȴ�����
		WORD			haveAdjustElecSource;			//ԴУ����ȷ��־

	//ң��ֵ  	

		LONG			UA;
		LONG			UB;
		LONG			UC;
		LONG			U0h;
		LONG			Ua;
		LONG			Ub;
		LONG			Uc;
		LONG			U0l;
		LONG			IA;
		LONG			IB;
		LONG			IC;
		LONG			Ia;
		LONG			Ib;
		LONG			Ic;
		LONG			I0;
		LONG			Ijx;

		LONG			Pa;
		LONG			Qa;
		LONG			Pb;
		LONG			Qb;
		LONG			Pc;
		LONG			Qc;
		LLONG			P;
		LLONG			Q;	
		LONG			cos;
  //ң�������飬�������µ�ң��ֵNUMOFYCVALUE����ȡ��ֵд�����ݿ�
  		LONG			UAArray[NUMOFYCVALUE];
		LONG			UBArray[NUMOFYCVALUE];
		LONG			UCArray[NUMOFYCVALUE];
		LONG			U0hArray[NUMOFYCVALUE];
		LONG			UaArray[NUMOFYCVALUE];
		LONG			UbArray[NUMOFYCVALUE];
		LONG			UcArray[NUMOFYCVALUE];
		LONG			U0lArray[NUMOFYCVALUE];
		LONG			IAArray[NUMOFYCVALUE];
		LONG			IBArray[NUMOFYCVALUE];
		LONG			ICArray[NUMOFYCVALUE];
		LONG			IaArray[NUMOFYCVALUE];
		LONG			IbArray[NUMOFYCVALUE];
		LONG			IcArray[NUMOFYCVALUE];
		LONG			I0Array[NUMOFYCVALUE];
		LONG			IjxArray[NUMOFYCVALUE];
		LONG			PArray[NUMOFYCVALUE];
		LONG			QArray[NUMOFYCVALUE];
		LONG			cosArray[NUMOFYCVALUE];
	  
  //����ϵ��У����ң���������
  		LONG			  adjPa;			//yanxuesong20120727 ���ʼ��ǲ����
  		LONG			  adjQa;
  		LONG			  adjPb;
  		LONG			  adjQb;
  		LONG			  adjPc;
  		LONG			  adjQc;
  		LONG			  adjAA;
  		LONG			  adjBA;
  		LONG			  adjCA;			//yanxuesong20120727
		
		LONG			  adjUA;
		LONG			  adjUB;
		LONG			  adjUC;
		LONG			  adjU0h;
		LONG			  adjUa;
		LONG			  adjUb;
		LONG			  adjUc;
		LONG			  adjU0l;
		LONG			  adjIA;
		LONG			  adjIB;
		LONG			  adjIC;
		LONG			  adjIa;
		LONG			  adjIb;
		LONG			  adjIc;
		LONG			  adjI0;
		LONG			  adjIjx;		
 //����ϵ��У����������������		
		TRelElecValPar			   adjUAp;
		TRelElecValPar			   adjUBp;
		TRelElecValPar			   adjUCp;
		TRelElecValPar			   adjU0hp;
		TRelElecValPar			   adjUap;
		TRelElecValPar			   adjUbp;
		TRelElecValPar			   adjUcp;
		TRelElecValPar			   adjU0lp;
		TRelElecValPar			   adjIAp;
		TRelElecValPar			   adjIBp;
		TRelElecValPar			   adjICp;
		TRelElecValPar			   adjIap;
		TRelElecValPar			   adjIbp;
		TRelElecValPar			   adjIcp;
		TRelElecValPar			   adjI0p;
		TRelElecValPar			   adjIjxp;		 
		
//ң������ͱ���������ֵ����
		LONG			MidUA;
		LONG			MidUB;
		LONG			MidUC;
		LONG			MidU0h;
		LONG			MidUa;
		LONG			MidUb;
		LONG			MidUc;
		LONG			MidU0l;
		LONG			MidIA;
		LONG			MidIB;
		LONG			MidIC;
		LONG			MidIa;
		LONG			MidIb;
		LONG			MidIc;
		LONG			MidI0;
		LONG			MidIjx;
		LONG			MidUAp;
		LONG			MidUBp;
		LONG			MidUCp;
		LONG			MidU0hp;
		LONG			MidUap;
		LONG			MidUbp;
		LONG			MidUcp;
		LONG			MidU0lp;
		LONG			MidIAp;
		LONG			MidIBp;
		LONG			MidICp;
		LONG			MidIap;
		LONG			MidIbp;
		LONG			MidIcp;
		LONG			MidI0p;
		LONG			MidIjxp;


 //ϵ��У��������������
		 LONG			   ArrayadjUA[NUMOFYCVALUE];
		 LONG			   ArrayadjUB[NUMOFYCVALUE];
		 LONG			   ArrayadjUC[NUMOFYCVALUE];
		 LONG			   ArrayadjU0h[NUMOFYCVALUE];
		 LONG			   ArrayadjUa[NUMOFYCVALUE];
		 LONG			   ArrayadjUb[NUMOFYCVALUE];
		 LONG			   ArrayadjUc[NUMOFYCVALUE];
		 LONG			   ArrayadjU0l[NUMOFYCVALUE];
		 LONG			   ArrayadjIA[NUMOFYCVALUE];
		 LONG			   ArrayadjIB[NUMOFYCVALUE];
		 LONG			   ArrayadjIC[NUMOFYCVALUE];
		 LONG			   ArrayadjIa[NUMOFYCVALUE];
		 LONG			   ArrayadjIb[NUMOFYCVALUE];
		 LONG			   ArrayadjIc[NUMOFYCVALUE];
		 LONG			   ArrayadjI0[NUMOFYCVALUE];
		 LONG			   ArrayadjIjx[NUMOFYCVALUE];			 
		 LONG			   ArrayadjUAp[NUMOFYCVALUE];
		 LONG			   ArrayadjUBp[NUMOFYCVALUE];
		 LONG			   ArrayadjUCp[NUMOFYCVALUE];
		 LONG			   ArrayadjU0hp[NUMOFYCVALUE];
		 LONG			   ArrayadjUap[NUMOFYCVALUE];
		 LONG			   ArrayadjUbp[NUMOFYCVALUE];
		 LONG			   ArrayadjUcp[NUMOFYCVALUE];
		 LONG			   ArrayadjU0lp[NUMOFYCVALUE];
		 LONG			   ArrayadjIAp[NUMOFYCVALUE];
		 LONG			   ArrayadjIBp[NUMOFYCVALUE];
		 LONG			   ArrayadjICp[NUMOFYCVALUE];
		 LONG			   ArrayadjIap[NUMOFYCVALUE];
		 LONG			   ArrayadjIbp[NUMOFYCVALUE];
		 LONG			   ArrayadjIcp[NUMOFYCVALUE];
		 LONG			   ArrayadjI0p[NUMOFYCVALUE];
		 LONG			   ArrayadjIjxp[NUMOFYCVALUE];		 

		LONG				m_pAdjustData[51];
		LONG				m_pBubbleData[NUMOFYCVALUE];	//������ң������

  //����ֵ����
		  LONG				adjUADV;
		  LONG				adjUBDV;
		  LONG				adjUCDV;
		  LONG				adjU0hDV;
		  LONG				adjUaDV;
		  LONG				adjUbDV;
		  LONG				adjUcDV;
		  LONG				adjU0lDV;
		  LONG				adjIADV;
		  LONG				adjIBDV;
		  LONG				adjICDV;
		  LONG				adjIaDV;
		  LONG				adjIbDV;
		  LONG				adjIcDV;
		  LONG				adjI0DV;
		  LONG				adjIjxDV; 
		  DWORD      		dwMsgID;
		  LONG				m_dwSourceU;
		  LONG				m_dwSourceI;

		
private:

public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);		
		void Init(void);
		
		
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
		void CaluMSPQ(LONG *UBuf,LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ);//��������й����޹�
		void CoefAdjust(void);//ϵ��У��
		void CoefAdjustCalu(void);//����ϵ��У���еļ���
		void CoefAdjustEnd(BOOL result); //ϵ��У����������
		LONG CoefAdjustSort(LONG Val,LONG* OriginalData);
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);


};
void DITask(DWORD *pdwApp);

#endif
