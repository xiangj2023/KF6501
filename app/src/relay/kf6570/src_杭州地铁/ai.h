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


//定义AI定时器时间间隔为240毫秒，修改时需注意其可能会影响到测频,需保证如下关系:AI_TIMER_PERIOD/20 > FREQBUFSIZE
#define AI_TIMER_PERIOD			240	
//每次测量任务中需要计算的频率个数
#define	NUM_FREQ_PER_AITASK		6		
#define	FREQBUFSIZE		(NUM_FREQ_PER_AITASK+5)



class CAIProc:public CApp
{
	DECLARE_MESSAGE_MAP()

public:
//---------------测频用相关变量------------------------------
		SHORT 			*m_wFreqBuf;		
		LONG 		 	*m_FreqCaluBuf;
		DWORD 			m_dwFreqMask;
		LONG 			m_dwFreq[NUM_FREQ_PER_AITASK];
		DWORD 			m_dwPreFreq;		
		BOOL 			m_FreqFlag;


//------------------遥测计算所用变量------------------------------------
		LONG			*m_pDataBuf;	//测量数据缓冲区		
		DWORD			m_AIDataMask;	//测量通道用的数据掩码
		LONG			*m_pComposeBuf; //合成数据缓冲区
		LONG			m_UHA;
		LONG			m_UHB;
		LONG 			m_UHC;
		LONG			m_U0;
		LONG			m_ULA;
		LONG			m_ULB;
		LONG			m_ULC;
		LONG			m_IHA;
		LONG			m_IHB;
		LONG			m_IHC;
		LONG			m_ILA;
		LONG			m_ILB;
		LONG			m_ILC;
		LONG			m_I1;
		LONG			m_I2;
		LONG			m_PA;
		LONG			m_QA;
		LONG			m_COSA;
		LONG			m_PB;
		LONG			m_QB;
		LONG			m_COSB;
		LONG			m_PC;
		LONG			m_QC;
		LONG			m_COSC;
		LONG			m_P;
		LONG			m_Q;
		LONG			m_COS;
		LONG			m_UHAB;
		LONG			m_UHBC;
		LONG			m_UHCA;
		LONG 			m_Freq1;
		TRelElecValPar			m_UHA1;
		TRelElecValPar			m_UHA2;
		TRelElecValPar			m_UHA3;
		TRelElecValPar			m_UHA5;
		TRelElecValPar			m_UHA7;
		TRelElecValPar			m_UHA9;
		TRelElecValPar			m_UHA11;		
		TRelElecValPar			m_UHB1;
		TRelElecValPar			m_UHB2;
		TRelElecValPar			m_UHB3;
		TRelElecValPar			m_UHB5;
		TRelElecValPar			m_UHB7;
		TRelElecValPar			m_UHB9;
		TRelElecValPar			m_UHB11;		
		TRelElecValPar			m_UHC1;
		TRelElecValPar			m_UHC2;
		TRelElecValPar			m_UHC3;
		TRelElecValPar			m_UHC5;
		TRelElecValPar			m_UHC7;
		TRelElecValPar			m_UHC9;
		TRelElecValPar			m_UHC11;		
		TRelElecValPar			m_IHA1;
		TRelElecValPar			m_IHA2;
		TRelElecValPar			m_IHA3;
		TRelElecValPar			m_IHA5;
		TRelElecValPar			m_IHA7;
		TRelElecValPar			m_IHA9;
		TRelElecValPar			m_IHA11;		
		TRelElecValPar			m_IHB1;
		TRelElecValPar			m_IHB2;
		TRelElecValPar			m_IHB3;
		TRelElecValPar			m_IHB5;
		TRelElecValPar			m_IHB7;
		TRelElecValPar			m_IHB9;
		TRelElecValPar			m_IHB11;		
		TRelElecValPar			m_IHC1;
		TRelElecValPar			m_IHC2;
		TRelElecValPar			m_IHC3;
		TRelElecValPar			m_IHC5;
		TRelElecValPar			m_IHC7;
		TRelElecValPar			m_IHC9;
		TRelElecValPar			m_IHC11;

		TRelElecValPar			m_ILA1;
		TRelElecValPar			m_ILA2;
		TRelElecValPar			m_ILA3;
		TRelElecValPar			m_ILA5;
		TRelElecValPar			m_ILA7;
		TRelElecValPar			m_ILA9;
		TRelElecValPar			m_ILA11;		
		TRelElecValPar			m_ILB1;
		TRelElecValPar			m_ILB2;
		TRelElecValPar			m_ILB3;
		TRelElecValPar			m_ILB5;
		TRelElecValPar			m_ILB7;
		TRelElecValPar			m_ILB9;
		TRelElecValPar			m_ILB11;		
		TRelElecValPar			m_ILC1;
		TRelElecValPar			m_ILC2;
		TRelElecValPar			m_ILC3;
		TRelElecValPar			m_ILC5;
		TRelElecValPar			m_ILC7;
		TRelElecValPar			m_ILC9;
		TRelElecValPar			m_ILC11;
		TRelElecValPar			m_sULA;
		TRelElecValPar			m_sULB;
		TRelElecValPar			m_sULC;
		TRelElecValPar			m_sILA;
		TRelElecValPar			m_sILB;
		TRelElecValPar			m_sILC;


		
		
		LONG					ArrayUHA[NUMOFYCVALUE];
		LONG					ArrayUHB[NUMOFYCVALUE];
		LONG					ArrayUHC[NUMOFYCVALUE];
		LONG					ArrayU0[NUMOFYCVALUE];
		LONG					ArrayULA[NUMOFYCVALUE];
		LONG					ArrayULB[NUMOFYCVALUE];
		LONG					ArrayULC[NUMOFYCVALUE];
		LONG					ArrayIHA[NUMOFYCVALUE];
		LONG					ArrayIHB[NUMOFYCVALUE];
		LONG					ArrayIHC[NUMOFYCVALUE];
		LONG					ArrayILA[NUMOFYCVALUE];
		LONG					ArrayILB[NUMOFYCVALUE];
		LONG					ArrayILC[NUMOFYCVALUE];
		LONG					ArrayI1[NUMOFYCVALUE];
		LONG					ArrayI2[NUMOFYCVALUE];
		LONG					ArrayPA[NUMOFYCVALUE];
		LONG					ArrayQA[NUMOFYCVALUE];
		LONG					ArrayCOSA[NUMOFYCVALUE];
		LONG					ArrayPB[NUMOFYCVALUE];
		LONG					ArrayQB[NUMOFYCVALUE];
		LONG					ArrayCOSB[NUMOFYCVALUE];
		LONG					ArrayPC[NUMOFYCVALUE];
		LONG					ArrayQC[NUMOFYCVALUE];
		LONG					ArrayCOSC[NUMOFYCVALUE];
		LONG					ArrayP[NUMOFYCVALUE];
		LONG					ArrayQ[NUMOFYCVALUE];
		LONG					ArrayCOS[NUMOFYCVALUE];
		LONG					ArrayUHAB[NUMOFYCVALUE];
		LONG					ArrayUHBC[NUMOFYCVALUE];
		LONG					ArrayUHCA[NUMOFYCVALUE];
		
		LONG					ArrayUHA1[NUMOFYCVALUE];
		LONG					ArrayUHA2[NUMOFYCVALUE];
		LONG					ArrayUHA3[NUMOFYCVALUE];
		LONG					ArrayUHA5[NUMOFYCVALUE];
		LONG					ArrayUHA7[NUMOFYCVALUE];
		LONG					ArrayUHA9[NUMOFYCVALUE];
		LONG					ArrayUHA11[NUMOFYCVALUE];		
		LONG					ArrayUHB1[NUMOFYCVALUE];
		LONG					ArrayUHB2[NUMOFYCVALUE];
		LONG					ArrayUHB3[NUMOFYCVALUE];
		LONG					ArrayUHB5[NUMOFYCVALUE];
		LONG					ArrayUHB7[NUMOFYCVALUE];
		LONG					ArrayUHB9[NUMOFYCVALUE];
		LONG					ArrayUHB11[NUMOFYCVALUE];
		LONG					ArrayUHC1[NUMOFYCVALUE];
		LONG					ArrayUHC2[NUMOFYCVALUE];
		LONG					ArrayUHC3[NUMOFYCVALUE];
		LONG					ArrayUHC5[NUMOFYCVALUE];
		LONG					ArrayUHC7[NUMOFYCVALUE];
		LONG					ArrayUHC9[NUMOFYCVALUE];
		LONG					ArrayUHC11[NUMOFYCVALUE];
		LONG					ArrayIHA1[NUMOFYCVALUE];
		LONG					ArrayIHA2[NUMOFYCVALUE];
		LONG					ArrayIHA3[NUMOFYCVALUE];
		LONG					ArrayIHA5[NUMOFYCVALUE];
		LONG					ArrayIHA7[NUMOFYCVALUE];
		LONG					ArrayIHA9[NUMOFYCVALUE];
		LONG					ArrayIHA11[NUMOFYCVALUE];		
		LONG					ArrayIHB1[NUMOFYCVALUE];
		LONG					ArrayIHB2[NUMOFYCVALUE];
		LONG					ArrayIHB3[NUMOFYCVALUE];
		LONG					ArrayIHB5[NUMOFYCVALUE];
		LONG					ArrayIHB7[NUMOFYCVALUE];
		LONG					ArrayIHB9[NUMOFYCVALUE];
		LONG					ArrayIHB11[NUMOFYCVALUE];
		LONG					ArrayIHC1[NUMOFYCVALUE];
		LONG					ArrayIHC2[NUMOFYCVALUE];
		LONG					ArrayIHC3[NUMOFYCVALUE];
		LONG					ArrayIHC5[NUMOFYCVALUE];
		LONG					ArrayIHC7[NUMOFYCVALUE];
		LONG					ArrayIHC9[NUMOFYCVALUE];
		LONG					ArrayIHC11[NUMOFYCVALUE];

		LONG					ArrayILA1[NUMOFYCVALUE];
		LONG					ArrayILA2[NUMOFYCVALUE];
		LONG					ArrayILA3[NUMOFYCVALUE];
		LONG					ArrayILA5[NUMOFYCVALUE];
		LONG					ArrayILA7[NUMOFYCVALUE];
		LONG					ArrayILA9[NUMOFYCVALUE];
		LONG					ArrayILA11[NUMOFYCVALUE];		
		LONG					ArrayILB1[NUMOFYCVALUE];
		LONG					ArrayILB2[NUMOFYCVALUE];
		LONG					ArrayILB3[NUMOFYCVALUE];
		LONG					ArrayILB5[NUMOFYCVALUE];
		LONG					ArrayILB7[NUMOFYCVALUE];
		LONG					ArrayILB9[NUMOFYCVALUE];
		LONG					ArrayILB11[NUMOFYCVALUE];
		LONG					ArrayILC1[NUMOFYCVALUE];
		LONG					ArrayILC2[NUMOFYCVALUE];
		LONG					ArrayILC3[NUMOFYCVALUE];
		LONG					ArrayILC5[NUMOFYCVALUE];
		LONG					ArrayILC7[NUMOFYCVALUE];
		LONG					ArrayILC9[NUMOFYCVALUE];
		LONG					ArrayILC11[NUMOFYCVALUE];
		
		
//------------------系数校正用变量---------------------------------------------
		// 1.标志定义
		DWORD			m_CoefAdjustDataMask;		//通道掩码
		LONG			*m_pCoefAdjustBuf;			//系数校正采样数据缓冲区
		DWORD			m_dwSourceU;				//系数校正电压
		DWORD			m_dwSourceI;				//系数校正电流
		DWORD			m_dwAdjustCnt;				//系数校正计算次数		
		DWORD			m_dwWaitCnt;				//系数校正后等待的次数
		BOOL			m_bIsCoefAdjust;			//开始系数校正标志		
		BOOL			m_bIsDriftAdjust;			//死区值校正标志
		BOOL			m_bIsSourceOK;				//源校正标志
		LONG			m_pAdjustData[28];
		LONG			m_pBubbleData[NUMOFYCVALUE];	//待排序遥测数据

		LONG			m_tDA;
		LONG			m_tDB;
		LONG			m_tDC;
//系数校正计算变量
		LONG 			m_AdjUHA;
		LONG			m_AdjUHB;
		LONG			m_AdjUHC;
		LONG			m_AdjU0;
		LONG			m_AdjULA;
		LONG			m_AdjULB;
		LONG			m_AdjULC;
		LONG			m_AdjIHA;
		LONG			m_AdjIHB;
		LONG			m_AdjIHC;
		LONG			m_AdjILA;
		LONG			m_AdjILB;
		LONG			m_AdjILC;
		LONG			m_AdjI1;
		LONG			m_AdjI2;
		LONG			m_AdjPA;
		LONG			m_AdjQA;
		LONG			m_AdjDA;
		LONG			m_AdjPB;
		LONG			m_AdjQB;
		LONG			m_AdjDB;
		LONG  			m_AdjPC;
		LONG			m_AdjQC;
		LONG			m_AdjDC;

		TRelElecValPar  m_AdjUHA1;
		TRelElecValPar  m_AdjUHB1;
		TRelElecValPar  m_AdjUHC1;
		TRelElecValPar  m_AdjIHA1;
		TRelElecValPar  m_AdjIHB1;
		TRelElecValPar  m_AdjIHC1;
		



		
//系数校正各通道死区值
		LONG 			m_DriftUHA;
		LONG			m_DriftUHB;
		LONG			m_DriftUHC;
		LONG			m_DriftU0;
		LONG			m_DriftULA;
		LONG			m_DriftULB;
		LONG			m_DriftULC;
		LONG			m_DriftIHA;
		LONG			m_DriftIHB;
		LONG			m_DriftIHC;
		LONG			m_DriftILA;
		LONG			m_DriftILB;
		LONG			m_DriftILC;
		LONG			m_DriftI1;
		LONG			m_DriftI2;
//排序数组
		LONG			ArrayAdjUHA[NUMOFYCVALUE];
		LONG			ArrayAdjUHB[NUMOFYCVALUE];
		LONG			ArrayAdjUHC[NUMOFYCVALUE];
		LONG			ArrayAdjU0[NUMOFYCVALUE];
		LONG			ArrayAdjULA[NUMOFYCVALUE];
		LONG			ArrayAdjULB[NUMOFYCVALUE];
		LONG			ArrayAdjULC[NUMOFYCVALUE];
		LONG			ArrayAdjIHA[NUMOFYCVALUE];
		LONG			ArrayAdjIHB[NUMOFYCVALUE];
		LONG			ArrayAdjIHC[NUMOFYCVALUE];
		LONG			ArrayAdjILA[NUMOFYCVALUE];
		LONG			ArrayAdjILB[NUMOFYCVALUE];
		LONG			ArrayAdjILC[NUMOFYCVALUE];
		LONG			ArrayAdjI1[NUMOFYCVALUE];
		LONG			ArrayAdjI2[NUMOFYCVALUE];
		LONG			ArrayAdjPA[NUMOFYCVALUE];
		LONG			ArrayAdjQA[NUMOFYCVALUE];
		LONG			ArrayAdjPB[NUMOFYCVALUE];
		LONG			ArrayAdjQB[NUMOFYCVALUE];
		LONG			ArrayAdjPC[NUMOFYCVALUE];
		LONG			ArrayAdjQC[NUMOFYCVALUE];
		LONG			ArrayAdjUHA1[NUMOFYCVALUE];
		LONG			ArrayAdjUHB1[NUMOFYCVALUE];
		LONG			ArrayAdjUHC1[NUMOFYCVALUE];
		LONG			ArrayAdjIHA1[NUMOFYCVALUE];
		LONG			ArrayAdjIHB1[NUMOFYCVALUE];
		LONG			ArrayAdjIHC1[NUMOFYCVALUE];
//排序后的计算值
		LONG			m_MidUHA;
		LONG			m_MidUHB;
		LONG			m_MidUHC;
		LONG			m_MidU0;
		LONG			m_MidULA;
		LONG			m_MidULB;
		LONG			m_MidULC;
		LONG			m_MidIHA;
		LONG			m_MidIHB;
		LONG			m_MidIHC;
		LONG			m_MidILA;
		LONG			m_MidILB;
		LONG			m_MidILC;
		LONG			m_MidI1;
		LONG			m_MidI2;
		LONG			m_MidPA;
		LONG			m_MidQA;
		LONG			m_MidPB;
		LONG			m_MidQB;
		LONG			m_MidPC;
		LONG			m_MidQC;
		LONG			m_MidUHA1;
		LONG			m_MidUHB1;
		LONG			m_MidUHC1;
		LONG			m_MidIHA1;
		LONG			m_MidIHB1;
		LONG			m_MidIHC1;
		
		DWORD      dwMsgID;

		
private:


public:
		CAIProc(DWORD *pdwApp):CApp(pdwApp){}
		void OnTimeOut(DWORD id);
		void CaluMSPQ(const LONG *UBuf,const LONG *IBuf,LONG &P,LONG &Q,LONG &Cos,LONG dA,BOOL coef = TRUE);



		
		//void OnAutoAdjustStep1(DWORD arg);	
		void OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival);	
		void Init(void);
		
		void WriteMSComposeBuf(LONG *ComposeBuf,LONG*DataBuf);
		void CoefAdjust(void);//系数校正		
		void CoefAdjustCalu(void);//处理系数校正中的计算
		void CoefAdjustEnd(BOOL result); //系数校正结束处理
		LONG AiBubbleSort(LONG Val,LONG* OriginalData);
		
//		void RefreshCommRW();
//		void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus);
};
void DITask(DWORD *pdwApp);

#endif
