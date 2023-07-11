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



#define AI_TIMER_PERIOD			200		//定义AI定时器时间间隔为200毫秒
//每次测量任务中需要计算的频率个数
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)


#define NORMAL_AI_TIME_CNT		(100/AI_TIMER_PERIOD)

class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
		LONG			*m_pDataBuf;										//测量数据缓冲区
		LONG			*m_pCoefAdjustBuf;									//系数校正数据缓冲区
		LONG			*m_pComposeAdjustBuf;								
		
		DWORD			m_AIDataMask;										//测量通道用的数据掩码
		DWORD			m_CoefAdjustMask;									//系数校正用的通道数据掩码

		DWORD			m_dwAdjustCounter;				//系数校正计算次数
		BOOL			m_bIsCoefAdjust;				//开始系数校正标志
        BOOL			m_bIsDeadValAdjust;				//死区值校正标志
		WORD			wCoefAdjustNum;					//系数校正次数
		WORD			wWaitTimeNum;					//系数校正开始等待次数
		WORD			haveAdjustElecSource;			//源校正正确标志

	//遥测值  	

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
  //遥测量数组，保存最新的遥测值NUMOFYCVALUE个，取中值写入数据库
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
	  
  //用于系数校正，遥测电量部分
  		LONG			  adjPa;			//yanxuesong20120727 功率及角差矫正
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
 //用于系数校正，保护电量部分		
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
		
//遥测电量和保护电量中值变量
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


 //系数校正电量排序数组
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
		LONG				m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据

  //死区值定义
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
		void CaluMSPQ(LONG *UBuf,LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ);//计算测量有功，无功
		void CoefAdjust(void);//系数校正
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG CoefAdjustSort(LONG Val,LONG* OriginalData);
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);


};
void DITask(DWORD *pdwApp);

#endif
