/************************************************************************************************/
/*CopyRight:																						*/
/*						��򿭷������ɷ����޹�˾													*/
/*Description:																						*/
/*						�����õ����ݽṹ���弰�궨��														*/
/*Modification history:																				*/
/*						������	yanzh 	08/03/21												*/
/*																								*/
/************************************************************************************************/



#ifndef _RELAY_DEFS_H_
#define _RELAY_DEFS_H_
#include "sysdefs.h"
#include "timedef.h"
//#include "relay.h"

#define MAX_SUBSTATION_NUM 	5		//������5����վ
#define FLCOMPLICATENUM		4		//�����󲢷���

#define	CTRATIO 150
#define	STANDARDCOEF (23170/CTRATIO)    //23170=32768/sqrt(2)
#define	MULTIPLES 2			//ͻ��������ʱ����Ĳ�����ֵ�ķŴ���
#define ToAbs(val) (((val)>=0)? ((val)):(-(val)))    					//�����ֵ 
#define RELAY_OFF 			0     							//�̵���״̬�� �˳� 
#define RELAY_ON  			1							//�̵���״̬�� Ͷ�� 

#define TANG85	 			(1143)						//85�ȵ�����ֵ 
#define MAXINVERSETIME		100  						 //��ʱ�ޱ����������ʱ��������ۼӼ������ 100�� 
#define INVERSECOEFA 		28000						//һ�㷴ʱ���ۼ�����ֵ����ϵ�� 
#define INVERSECOEFB 		1350						//����ʱ���ۼ�����ֵ����ϵ�� 
#define INVERSECOEFC 		80							//����ʱ���ۼ�����ֵ����ϵ�� 
#define MAXFLNUM			10							//���ϲ�������� 
#define NOCURCOEF			90							//�ж�������բ�������ϵ�� 
#define RECOVERVOLCOEF		110							//Ƿ�����������ָ�ϵ�� 
#define IMPDMULTIPLE   		2000 						//�迹�Ŵ��� 
#define TANGMULTIPLE		100							//���б�ֵ�Ŵ��� 
#define SINWTS				1248					// 32*100*2*1000*SIN(wTs)��6400*SIN(11.25��)
#define NUMOFYCVALUE		9						//����ң��ֵ����������ȡ��ֵд�����ݿ⣬����Ϊ����
//#define	INHERENTTIME		40						//��������ʱ������ֵ
//#define	INHERENTTIME		40   //���������Ĺ���ʱ��
#define FREQ_TYPE_PRO		0    //װ������Ϊ����װ��
#define FREQ_TYPE_MEA		1	 //װ������Ϊ���װ��
#define BO_CHECK_BUF_LEN	2	 //���忪���Լ�״ֵ̬��������Ĵ�С

#define MAX_FREQUENCE  56000  //�����Ƶ�����ЧƵ��
#define MIN_FREQUENCE  44000	 //�����Ƶ��С��ЧƵ��
#define MIN_TESTU      1610  //10V��ѹ�ż�ֵ

#define BO_BREAK_CNT		5	//ÿ·�����Լ����

#define INVALIDRX 3276
#define ZERODRIFT   0
#define FULLVALUE   1

#define NORMALPICK  0x55  //�����������
#define ACCPICK		0xAA	//���ٱ������

extern WORD AngleTab[];
extern LONG InverseCurCoefA[20][3];
extern LONG InverseCurCoefB[];
extern LONG InverseCurCoefC[];
extern WORD CosAngleTab[];
extern WORD SinAngleTab[];
extern WORD TangAngleTab[];
extern WORD VolHarmCal[];
extern WORD CurHarmCal[];
extern WORD CurHarmCal_6570_I[];
extern WORD VolHarmCal_6570_U[];
extern WORD CurHarmCal_6571_I[];
extern WORD VolHarmCal_6571_U[];

extern LONG MatrixCoef[8][9];
extern LONG MartixVV3[3][9];
extern LONG MartixVV1[3][9];
extern LONG MartixAT2[3][6];
extern LONG Matrix6533[9];
extern LONG Martix6535[13][9];
extern LONG Martix6535_AB[3][9];
extern LONG WCT6535[3][3];
extern WORD CRCTab[];
/*
typedef struct RELAYINFO{
	char RelayType[16];
	WORD nBI;
	WORD nBO;
	WORD nAI;
	WORD npAI;
	WORD nTO;
	WORD bak;
}TRelayInfo;
*/
/********************************************************
* ����:		TLoadSupV
* ����:		���ɼ��ӵ����ṹ
* ����: 
* ����:
* ��ע:
********************************************************/
typedef struct TLOADSUPERVISION {
	LONG Voltage;
	LONG Current;
	LONG AngleVolt;
//	LONG AngleCurr;		//�̶������Ƕ�Ϊ0�ȣ���ѹ�Ƕ��Ե����Ƕ�Ϊ��׼
	TAbsTime Time;
}TLoadSupV;

/********************************************************
* ����:		TRelElecValPar
* ����:		ͨ�õ��������ṹ
* ����: 
* ����:
* ��ע:
********************************************************/
typedef struct TREL_ELECVALUE_PAR {	
	LONG	Real;				//ʵ�� 
	LONG	Imag;				//�鲿 
	LONG	Mod;             			//ģֵ 
	LONG	Angle;			      //��� 
}TRelElecValPar;

/********************************************************
* ����:		TRelSimpGeneOvUnPar
* ����:		ͨ�õļ򵥹�����Ƿ������Ԫ�������ṹ
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TREL_SIMPLEGENERALOVUN_PAR{
	const LONG			*pEleSet;						//������ֵָ�� 
	TRelElecValPar			*pTEleVal;						//����ָ�� 
	WORD				ACTRelayWord;					//�����̵����� 
	WORD				StartRelayWord;					//��μ̵�����
	WORD				wExcepWordNum;					//���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ2 
}TRelSimpGeneOvUnPar;
/********************************************************
* ����:		TRelGeneOvValPar
* ����:		ͨ�ù�������Ԫ�������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TREL_GENERELOVERVALUE_PAR{
	const LONG		*pElecSet;          						//��������������ֵ 
	const LONG		*pTimeSet;				 			//��������ʱ�䶨ֵ 
	const LONG		*pHarmKhr1Set;						//��������г����Ȩϵ�� 
	const LONG		*pVolLockSet;				 	    	//����������ѹ������ֵ 
	const LONG		*pAccActTimeSet;						//������������ٶ���ʱ�䶨ֵ 
	const LONG		*pH2Coef;							//г������ϵ��������ϵ���ķŴ��� 

	const BYTE		*pRelCFG;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	const BYTE		*pRelSW;         						//����������ѹ��------1:Ͷ�룬2:�˳�  
	const BYTE		*pRelHLockCFG;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	const BYTE		*pRelHSuprCFG; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	const BYTE		*pRelPDCFG;		    					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	const BYTE		*pRelVolLockCFG;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	const BYTE		*pRelFDCFG;							//���������������ܿ���λ----1:Ͷ�룬2:�˳� 
	const BYTE		*pRelAccCFG;							//���޺���ٹ���------1:�У�  2:�� 
	const BYTE      *pRel47UCFG;						//��ѹ�������ܿ���λ
	const BYTE 		*pRelFLCFG;							//���ϲ�������� 
	DWORD			*pSysTimer;							//ϵͳ��ʱ��ָ�� 	
	
	TRelElecValPar		*pRelElec;							//��ǰ����ָ�� 		
	TRelElecValPar		*pRelHarm3;							//��ǰ����г��ָ�� 	
	TRelElecValPar		*pRelHarm5;							//��ǰ���г��ָ�� 	
	TRelElecValPar		*pRelVolt;							//��ǰ���������ĵ�ѹ��ָ�� 

	WORD			StartRelayWord;						//����������μ̵����� 
	WORD			ActRelayWord;						//�������������̵����� 
	WORD			AccRelayWord;						//���������̵����� 
	WORD			AccActRelayWord;					//����ٶ����̵����� 
	WORD			PForwardRelayWord;					//����������̵����� 
	WORD			PRewardRelayWord;					//���ʷ�����̵����� 
	WORD			H2LockRelayWord;					//����г�������̵����� 
	WORD			R47ULKRelayWord;					//��ѹ�����̵�����
	WORD			LockRelayWord;						//��α����̵����� 
	WORD			RSTRelayWord;						//���ر����̵����� 
	WORD			LoopCtrlRelayWord;					//����ѭ�����Ƽ̵����� 
	
	WORD			wReportNum;							//��������������� 
	WORD			wAccReportNum;						//���ٶ���������� 
	WORD			*RptSerialNum;
	LONG			*dwK35;

	BOOL			bMakeActRpt;						//FALSE,���澯���棻TRUE������������
	//**********************��η��ر��沿��*************	yanxs  13-02-05
	WORD			wPickUpReportNum;						//��α������к�
	WORD			wResetReportNum;						//���ر������к�
	WORD			wAccPickUpRptNum;						//��α������к�
	WORD			wAccResetRptNum;						//���ر������к�
//	WORD			*RptPickupSerialNum;					
//	BOOL			*RptPickupFlag;							//�Ƿ�������α���������־
	//**********************************************************************	
	DWORD			dwRelTimer;			 				//��ʱ�� 
	WORD			byRelFCounter;						//���ϼ����� 
	WORD			byRelRCounter;						//���ؼ����� 
	WORD			wExcepWordNum;						//TRelGeneOvValPar ͨ�ù��� ���ṹ�������CRCУ��ʱ���ų���������������ṹΪ8,���ṹ��СΪ72�� 
	WORD			wPickUpType;						//�����������:������λ���һ�����			
	WORD			wInfo;								//���� 
	
}TRelGeneOvValPar;
/********************************************************
* ����:		TRelGeneOvValPar_2
* ����:		ͨ�ù�������Ԫ�������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TREL_GENERELOVERVALUE_PAR_2{
	const LONG		*pElec1Set;          						//��������������ֵ 	
	const LONG		*pElec2Set;          						//��������������ֵ 
	const LONG		*pTimeSet;				 			//��������ʱ�䶨ֵ 
	const LONG		*pHarmKhr1Set;						//��������г����Ȩϵ�� 
	const LONG		*pVolLockSet;				 	    	//����������ѹ������ֵ 
	const LONG		*pAccActTimeSet;						//������������ٶ���ʱ�䶨ֵ 
	const LONG		*pH2Coef;							//г������ϵ��������ϵ���ķŴ��� 

	const BYTE		*pRelCFG;							//�����������ܿ�����--1:Ͷ�룬2:�˳�  
	const BYTE		*pRelSW;         						//����������ѹ��------1:Ͷ�룬2:�˳�  
	const BYTE		*pRelHLockCFG;			    			//г���������ܿ���λ--1:Ͷ�룬2:�˳� 
	const BYTE		*pRelHSuprCFG; 						//г�����ƹ��ܿ���λ--1:Ͷ�룬2:�˳� 
	const BYTE		*pRelPDCFG;		    					//���ʷ����ܿ���λ--1:Ͷ�룬2:�˳� 
	const BYTE		*pRelVolLockCFG;						//��ѹ�������ܿ���λ--1:Ͷ�룬2:�˳�  
	const BYTE		*pRelFDCFG;							//���������������ܿ���λ----1:Ͷ�룬2:�˳� 
	const BYTE		*pRelAccCFG;							//���޺���ٹ���------1:�У�  2:�� 
	const BYTE      *pRel47UCFG;						//��ѹ�������ܿ���λ
	const BYTE 		*pRelFLCFG;							//���ϲ�������� 
	DWORD			*pSysTimer;							//ϵͳ��ʱ��ָ�� 	
	
	TRelElecValPar		*pRelElec1;							//��ǰ������ָ��	
	TRelElecValPar		*pRelElec2;							//��ǰ�ε���ָ�� 		
	TRelElecValPar		*pRelHarm3;							//��ǰ����г��ָ�� 	
	TRelElecValPar		*pRelHarm5;							//��ǰ���г��ָ�� 	
	TRelElecValPar		*pRelVolt;							//��ǰ���������ĵ�ѹ��ָ�� 

	WORD			StartRelayWord;						//����������μ̵����� 
	WORD			ActRelayWord;						//�������������̵����� 
	WORD			AccRelayWord;						//���������̵����� 
	WORD			AccActRelayWord;					//����ٶ����̵����� 
	WORD			PForwardRelayWord;					//����������̵����� 
	WORD			PRewardRelayWord;					//���ʷ�����̵����� 
	WORD			H2LockRelayWord;					//����г�������̵����� 
	WORD			R47ULKRelayWord;					//��ѹ�����̵�����
	WORD			LockRelayWord;						//��α����̵����� 
	WORD			RSTRelayWord;						//���ر����̵����� 
	WORD			LoopCtrlRelayWord;					//����ѭ�����Ƽ̵����� 
	
	WORD			wReportNum;							//��������������� 
	WORD			wAccReportNum;						//���ٶ���������� 
	WORD			*RptSerialNum;
	LONG			*dwK35;

	BOOL			bMakeActRpt;						//FALSE,���澯���棻TRUE������������
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	WORD			wPickUpReportNum;						//��α������к�
	WORD			wResetReportNum;						//���ر������к�
	WORD			wAccPickUpRptNum;						//��α������к�
	WORD			wAccResetRptNum;						//���ر������к�
	//**********************************************************************	


	
	DWORD			dwRelTimer;			 				//��ʱ�� 
	WORD			byRelFCounter;						//���ϼ����� 
	WORD			byRelRCounter;						//���ؼ����� 
	WORD			wExcepWordNum;						//TRelGeneOvValPar_2 ͨ�ù���2 ���ṹ�������CRCУ��ʱ���ų���������������ṹΪ8,���ṹ��СΪ76�� 
	WORD			wPickUpType;						//�����������:������λ���һ�����			
	WORD			wInfo;								//���� 
	
}TRelGeneOvValPar_2;

/********************************************************
* ����:		TRel21Par
* ����:		ͨ�þ��뱣��Ԫ�������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct  TREL21PAR{
	const LONG		*pRel21RSet;					//���뱣�����趨ֵ 
	const LONG		*pRel21XSet;						//���뱣���翹��ֵ 
	const LONG		*pRel21TSet;						//���뱣��ʱ�䶨ֵ 
	const LONG		*pRel21Khr1Set;					//���뱣��г������ϵ�� 
	const LONG		*pRel21LineAngleSet;				//��·�迹�� 
	const LONG		*pRel21AccTSet;					//�������ٶ���ʱ�� 
	const LONG		*pRel21PTBreakCurSet;			//PT���߱���������ֵ 
	const LONG		*pH2Coef;						//г������ϵ��������ϵ���ķŴ��� 
	const LONG		*pRel21ZOffset;					//ƽ���ı����迹ƫ��ֵ 
	const LONG		*pRel21AOffset;					//�迹�ı��νǶ�ƫ�ƶ�ֵ

	
	const BYTE		*pRel21CFG;						//���뱣������ 
	const BYTE		*pRel21SW;						//���뱣����ѹ�� 
	const BYTE		*pRel21DirCFG;					//���뱣������ 
	const BYTE		*pRel21QuadTypeCFG;				//ƽ���ı��� 
	const BYTE		*pRel21HLockCFG;				//���뱣��г������ 
	const BYTE		*pRel21HSuprCFG;				//���뱣��г������ 
	const BYTE		*pRel21PTBreakCFG;				//PT���߼�� 
	const BYTE		*pRel21ACCCFG;					//����ٹ��ܿ��� 
	const BYTE 		*pRelFLCFG;						//���ϲ�������� 
	const DWORD   	*pSysTimer;						//ϵͳ��ʱ��ָ�� 
	const WORD		*pTangTabAddr;					//���б��׵�ַ 
	

	TRelElecValPar		*pRel21Cur;						//����ָ�� 
	TRelElecValPar		*pRel21Vol;						//��ѹָ�� 
	TRelElecValPar  	*pRel21Impd;					//��ǰ���迹 
	TRelElecValPar		*pRel21H3;						//����г��ָ�� 
	TRelElecValPar		*pRel21H5;						//���г��ָ�� 
	
	LONG		   *pnZCoef;						   //�迹ϵ��     //2013-04-17   �����迹�����жϷ���

	WORD			StartRelayWord;					//���뱣����μ̵����� 
	WORD			ACTRelayWord;					//���뱣�������̵����� 
	WORD			ACCRelayWord;					//���������̵����� 
	WORD			ACCActRelayWord;				//����ٶ����̵����� 
	WORD			PTBreakRelayWord;				//PT���߼̵����� 
	WORD			H2LockRelayWord;				//����г�������̵����� 
	WORD			LockRelayWord;					//��α����̵����� 
	WORD			LoopCtrlRelayWord;				//����ѭ�����Ƽ̵����� 

	WORD			wRel21RptNum;				     	//���뱣��������� 
	WORD			wRel21AccRptNum;				//���뱣������ٱ������ 
	WORD			wRel21PTLockRptNum;			//PT���߱������뱣��������� 

	WORD			*RptSerialNum;
	LONG			*dwK35;
	
	WORD			R37IRelayWord;					//�����̵�����	
	//**********************��η��ر��沿��*************	yanxs  13-03-08
	WORD			wPickUpReportNum;						//��α������к�
	WORD			wResetReportNum;						//���ر������к�
	WORD			wAccPickUpRptNum;						//��α������к�
	WORD			wAccResetRptNum;						//���ر������к�
//	WORD			*RptPickupSerialNum;
//	BOOL			*RptPickupFlag;
	//**********************************************************************	
	DWORD			dwRel21Timer;					//��ʱ�� 	
	WORD			byRel21FCounter;					//���뱣�����ϼ����� 
	WORD			byRel21RCounter;				//���뱣�����ؼ����� 
	WORD			wExcepWordNum;					//TRel21Par ���뱣�� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ8����СΪ82 	
	WORD			wPickUpType;						//�����������:������λ���һ�����			
	WORD			wInfo;							//���� 
}TRel21Par;

/********************************************************
* ����:		TRelGeneUnValPar
* ����:		ͨ��Ƿ������Ԫ�������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TREL_GENERELUNDERVALUE_PAR{
	
	const LONG		*pELeSet;						//������ֵ 
	const LONG		*pTimeSet;						//ʱ�䶨ֵ 
	const LONG		*pLockCurSet;					//����������ֵ 

	const BYTE		*pRelCFG;					  	 //Ƿ���������� 
	const BYTE		*pRelSW;					  	 //Ƿ��������ѹ�� 
	const BYTE		*pRelAlarmCFG;						//Ƿ������Ͷ�澯
	const BYTE		*pRelCurLockCFG;					//Ƿ��������������
	const DWORD		*pSysTimer;						//ϵͳ��ʱ��ָ�� 

	TRelElecValPar		*pRelElec;						//��ǰ�ĵ���ָ�� 
	TRelElecValPar		*pRelLockElec;					//�������� 

	WORD			ActRelayWord;					//Ƿ�����������̵����� 
	WORD			StartRelayWord;					//Ƿ��������μ̵����� 
	WORD			AlarmRelayWord;					//Ƿ�������澯�̵����� 
	WORD			LockRelayWord;					//��α����̵����� 
	WORD			RSTRelayWord;					//���Ϸ��ر����̵����� 
	WORD			LoopCtrlRelayWord;				//����ѭ�����Ƽ̵����� 
	WORD			wR52BFRealyword;				//��·����բʧ�̵ܼ�����
	WORD			wActRptNum;						//Ƿ����������������� 
	WORD			wAlarmRptNum;					//Ƿ�������澯������� 
	WORD			wTRFailRptNum;					//��·����բʧ�ܱ������
	WORD			wPickUpRptNum;					//�����������
	BYTE			*RptSerialNum;	
//	//***************************�����������ӱ������������ر���**********		yanxs  2013-03-08
//	WORD			wResetReportNum;				//��ѹ�������ر���
//	WORD			wPickUpReportNum;				//��ѹ������������
//	//**********************************************************************
    TAbsTime		R27UPickUpTime;					//��ѹ��������ʱ��
    WORD			R27RptSerialNum;				//������ʹ�õ����к�
	DWORD			dwRelTimer;						//Ƿ��������ʱ�� 	
	WORD			byRelFCounter;					//���ϼ����� 
	WORD			byRelRCounter;					//���ؼ����� 
	BOOL			bHaveMake52BFRptFlag;
	WORD			wExcepWordNum;					//TRelGeneUnValPar ͨ��Ƿ�� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14 ���ṹ��СΪ48
	WORD			wInfo;							//���� 

}TRelGeneUnValPar;
/********************************************************
* ����:		TRelInverseCurPar
* ����:		��ʱ�޹��������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 

typedef struct TRELINVERSECURPAR{
	
	const LONG		*pRelCurSet;                   			 //��ʱ�ޱ���������ֵ 
	const LONG		*pRelTimeSet;					//��ʱ�ޱ���ʱ�䶨ֵ 
	const LONG		*pRelTypeSet;					//��ʱ�ޱ����������� 

	const BYTE		*pRelCFG;						//��ʱ�ޱ������� 
	const BYTE		*pRelSW;						//��ʱ�ޱ�����ѹ�� 
	const DWORD		*pSysTimer;						//ϵͳ��ʱ�� 
	const LONG		(*pCoefTabA)[3];					//һ�㷴ʱ��ϵ�����ַ 
	const LONG		*pCoefTabB;						//����ʱ��ϵ�����ַ 
	const LONG		*pCoefTabC;						//����ʱ��ϵ�����ַ 

	TRelElecValPar		*pRelElec;						//��ǰ�ĵ���ָ�� 
	DWORD			dwCaluCurSet;					//�����ķ�ʱ�޵�����ֵ 

	WORD			ACTRelayWord;					//��ʱ�޶����̵����� 
	WORD			StartRelayWord;					//��ʱ����μ̵����� 
	WORD			LockRelayWord;					//��α����̵����� 
	WORD			RSTRelayWord;					//���ر����̵����� 
	WORD			LoopCtrlRelayWord;				//����ѭ�����Ƽ̵����� 
	WORD			wRelRptNum;					//��ʱ�ޱ���������� 
	WORD			*RptSerialNum;		
	//**********************��η��ر��沿��*************	yanxs  13-03-08
	WORD			wPickUpReportNum;						//��α������к�
	WORD			wResetReportNum;						//���ر������к�
//	WORD			*RptPickupSerialNum;
//	BOOL			*RptPickupFlag;
	//**********************************************************************	
	LONG			MaxCouter;						//����ۼӴ��� 	
	LLONG			CurCounterSet;					//�����ۼ��������Ķ�ֵ (28000*50*SAMPNUM*Is*Is*Ts)  
	
	LLONG			CurItem;						//�����ۼ����ĵ�ǰ�� 
	LLONG			CurCounter;						//�����ۼ��� 
	LONG			LimitCouter;					//���޶���ʱ���ж��õ��ۼӴ��� 	
	WORD			byRelFCounter;					//���ϼ����� 
	WORD			byRelRCounter;					//���ؼ����� 
	DWORD			dwRelTimer;						//��ʱ�ޱ�����ʱ�� 	
	WORD			wExcepWordNum;					//TRelInverseCurPar ��ʱ�޹��� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ16 ��СΪ56 
	WORD			wInfo;							//���� 

}TRelInverseCurPar;
/********************************************************
* ����:		TRelFaultLocatePar
* ����:		���ϲ������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 

typedef struct TRELFAULTLOCATEPAR{
	
	const BYTE		*pRel21FLCFG;						//���ϲ�������� 
	const LONG		*pRel21FLNSet;                             	       //���ֶ�����ֵ 
	const LONG		*pRel21FL1LSet; 						//��1�ξ��붨ֵ 
	const LONG		*pRel21FL1XSet;						//��1�ε�λ�翹��ֵ 
	const LONG		*pRel21FL2LSet; 						//��2�ξ��붨ֵ 
	const LONG		*pRel21FL2XSet;  					//��2�ε�λ�翹��ֵ 
	const LONG		*pRel21FL3LSet;  						//��3�ξ��붨ֵ 
	const LONG		*pRel21FL3XSet; 						//��3�ε�λ�翹��ֵ 
	const LONG		*pRel21FL4LSet;  						//��4�ξ��붨ֵ 
	const LONG		*pRel21FL4XSet; 						//��4�ε�λ�翹��ֵ 
	const LONG		*pRel21FL5LSet;  						//��5�ξ��붨ֵ 
	const LONG		*pRel21FL5XSet; 						//��5�ε�λ�翹��ֵ 
	const LONG		*pRel21FL6LSet; 						//��6�ξ��붨ֵ 
	const LONG		*pRel21FL6XSet; 						//��6�ε�λ�翹��ֵ 
	const LONG		*pRel21FL7LSet;						//��7�ξ��붨ֵ 
	const LONG		*pRel21FL7XSet; 						//��7�ε�λ�翹��ֵ 
	const LONG		*pRel21FL8LSet; 						//��8�ξ��붨ֵ 
	const LONG		*pRel21FL8XSet;						//��8�ε�λ�翹��ֵ 
	const LONG		*pRel21FL9LSet; 						//��9�ξ��붨ֵ 
	const LONG		*pRel21FL9XSet;						//��9�ε�λ�翹��ֵ 
	const LONG		*pRel21FL10LSet; 					//��10�ξ��붨ֵ 
	const LONG		*pRel21FL10XSet;					//��10�ξ��붨ֵ 
	const LONG		*pRel21FLS0;	//�����
	const LONG	    *pRel21FLSD;	//����귽��
	const LONG	    *pRel21FLLc;	//��������

     LONG			*pFaultValueL;						//���ϵ���L��ָ�� 
	 WORD			FaultValueNum;						//���ϵ������ 
	 LONG			*pSS;
	 WORD			SSNO;
	 
	 LONG			*pnLCoef;							//����Ŵ��� 
	 DWORD			SetTab[30];							//���㶨ֵ��

	TRelElecValPar  *pRel21FLImpd;						//��ǰ�迹��ָ�� 	
	WORD			wExcepWordNum;						//TRelFaultLocatePar ���ϲ�� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ0����СΪ114 
	
}TRelFaultLocatePar;

/********************************************************
* ����:		TRelPTBrokPar
* ����:		pt�����жϲ����ṹ 
* ����: 
* ����:
* ��ע: �ýṹ�����ж���·��ѹ1������������1·��ѹ1·���������
********************************************************/ 
typedef struct TRELPTBROKPAR{

	const BYTE 		*pRelPTBrokCFG;						//PT�������� 

	const LONG		*pR74PTU1Set;						//PT���߼���ѹ��ֵ
	const LONG		*pR74PTU2Set;						//PT���߼���ѹ��ֵ
	const LONG		*pR74PTISet;						//pt���߼�������ֵ	
	const LONG		*pR74PTTSet;						//pt���߼��ʱ�䶨ֵ
	
	TRelElecValPar		*pRelCur;						//�ж��õ��� 
	TRelElecValPar		*pRelU1;						//�ж��õ�һ·��ѹ�����жϵ�ѹ	
	TRelElecValPar		*pRelU2;						//�ж��õڶ�·��ѹ�������жϵ�ѹ

	WORD			StartRelayWord;						//PT������μ̵����� 
	WORD			ActRelayWord;						//PT���߶����̵����� 	
	const DWORD		*dwSysTimer;						//ϵͳ��ʱ�� 
	WORD			LKPIRelayWord;						//��α����̵�����
	WORD			wRelRptNum;							//PT���߶��������
	WORD			wRelResRptNum;						//PT���߷��ر����
	
	WORD			wFCounter;
	DWORD			dwRelTimer;							//PT���߶�ʱ�� 	
	WORD			wExcepWordNum;						//TRelPTBrokPar PT���� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ5����СΪ28 

}TRelPTBrokPar;
/********************************************************
* ����:		TRel79Par
* ����:		�غ�բԪ�������ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TRELAUTORECLOSEPAR{

	const LONG		*R79NumSet;						//Ͷ����غ�բ���� 
	const LONG		*R79T1Set;							//��һ���غ�բ����ʱ�� 
	const LONG		*R79T2Set;							//�ڶ����غ�բ����ʱ�� 
	const LONG		*R79RSTSet;							//�غ�բ��ŵ�ʱ�� 
	const LONG		*R79SOTFTSet;						//���ڹ��ϼ����ʱ����Ϊ�����Ͷ��ʱ�� 
	const LONG		*R79PISTSet;						//�غ�բ����������ʱ����Ϊ��·��������ʱ 
	const LONG		*R79CLSTSet;						//�غ�բ����������ʱ 

	
	const BYTE		*R79SCFG;						//͵���غ�բ����
	const BYTE		*R79SW;						//�غ�բ��ѹ��

	WORD			R79RW;						//�غ�բ���ü̵�����--�м�̵����֣����߼��н�������
	WORD			R79CYSRW;					//�غ�բ����״̬�̵�����--�Ա��̵ּ����� 
	WORD			R79RSSRW;					//�غ�բ����״̬�̵�����--�Ա��̵ּ����� 
	WORD			R79LOSRW;					//�غ�բ����״̬�̵�����--�Ա��̵ּ����� 

	WORD			R79SH0RW;					//�غ�բ����״̬�̵�����һ--����̵����� 		
	WORD			R79SH1RW;					//�غ�բ����״̬�̵����ֶ�--����̵����� 

	WORD			R79PIRW;					//�غ�բ�����߼��̵�����--�Ա��̵ּ����� 
	WORD			R79PISRW;					//�غ�բ���������߼��̵�����--�м�̵����� 
	WORD			R79TORW;					//�غ�բ��ʱ���߼��̵�����--����̵����� 
	WORD			R79CLSRW;					//�غ�բ���������߼��̵�����--�м�̵����� 
	WORD			R79CLRW;					//�غ�բ�����߼��̵�����--�Ա��̵ּ����� 
	WORD			R79CLCRW;					//�غ�բ����������߼��̵�����--�м�̵����� 
	WORD			R79LOCKRW;					//�غ�բ�����߼��̵�����--�м�̵����� 
	WORD			R79SOTFRW;					//�غ�բ���ڹ����߼��̵�����--����̵����� 
	WORD			R79TRCHECK;					//�غ�բ���������̵�����--�м�̵����� by yanzh 2013��6��18�� 16:57:16
	
	WORD			R7952A;						//�غ�բ��·����λ�̵����� 
	WORD			R7952B;						//�غ�բ��·����λ�̵����� 
	const DWORD		*dwSysTimer;				//ϵͳ��ʱ��ָ�� 
	WORD			wRpt79_1;					//һ���غ�բ��������� 
	WORD			wRpt79_2;					//�����غ�բ��������� 
	WORD			wRpt79Fail;					//�غ�բʧ�ܱ���� 
	WORD			wRpt79LK_1;					//�غ�բ�����������	
	WORD			wRpt79LK_2;					//�غ�բ�����������
	WORD			wPickUpRptNum;				//���������������

	WORD			wRpt79_1_SUCC;			//һ���غ�բ�ɹ�����  by yanzh 2013��6��18�� 17:31:57
	WORD			wRpt79_1_FAIL;			//һ���غ�բʧ�ܱ���
	WORD			wRpt79_2_SUCC;			//�����غ�բ�ɹ�����
	WORD			wRpt79_2_FAIL;			//�����غ�բʧ�ܱ���
	WORD			wRpt79PI_1;				//һ���غ�բ��������	
	WORD			wRpt79PI_2;				//�����غ�բ��������	
	WORD			wRpt79NPI_1;				//�غ�բδ��������	
	WORD			wRpt79NPI_2;				//�غ�բδ��������

	

	WORD			*RptSerialNum;
	BYTE			*SysRptSerialNum;
	BOOL			*pFaultOnFlag;
	BOOL			*pR79PIRptFlag;				//�غ�բ���������־


	BOOL			R79RSFLAG;					//�غ�բ��ŵ���ʱ������־ 
	BOOL			R79PISFLAG;					//�غ�բ����������ʱ������־ 
	BOOL			R79TFLAG;					//�غ�բ������ʱ������־ 
	BOOL			R79CLSFLAG;					//�غ�բ����������ʱ������־ 
	BOOL			R79LKFlag;					//���غ�բ�����������־	
	BOOL 			R79CLFlag;					//�غ�բ������־
	
	DWORD			R79Timer;					//�غ�բ������ʱ��ʱ�� 
	WORD			R79ShotCnt;					//�غ�բ�Ѿ��������� 	
	WORD			wExcepWordNum;				//TRel79Par �غ�բ ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ16����СΪ64 
}TRel79Par;

/********************************************************
* ����:		TRel50DIPar
* ����:		��������Ԫ�� 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct INCRECURRENTPAR{

	const LONG		*pRelCurSet;                   			 //��������������ֵ 
	const LONG		*pRelTimeSet;					//��������ʱ�䶨ֵ 
	const LONG		*pRelLockCurSet;					//��������С����������ֵ 
	const LONG		*pRelKh1;						//����г������ϵ�� 
	const LONG		*pRelKhr;						//��������г����Ȩϵ�� 
	const LONG		*pH2Coef;						//����г������ϵ����г����Ȩϵ���ȵķŴ��� 

	const BYTE		*pRelCFG;						//������������ 
	const BYTE		*pRelSW;						//����������ѹ�� 


	TRelElecValPar		*pRelElec1;						//��ǰ��������ָ�� 
	TRelElecValPar		*pRelElec2;						//��ǰ����г������ָ�� 
	TRelElecValPar		*pRelElec3;						//��ǰ����г������ָ�� 
	TRelElecValPar		*pRelElec5;						//��ǰ���г������ָ�� 

	DWORD     		*pFaultSINum;					//���ϴ���������������ָ�� 

	WORD			ACTRelayWord;					//�������������̵����� 
	WORD			StartRelayWord;					//����������μ̵����� 
	WORD			LockRelayWord;					//��α����̵����� 
	WORD			RSTRelayWord;					//���ر����̵����� 
	WORD			LoopCtrlRelayWord;				//����ѭ�����Ƽ̵����� 
	WORD			R79ActRelayWord;
	WORD			H2LockRelayWord;				//����г�������̵�����
	WORD			*RptSerialNum;					//ϵͳ�������к� 
	const DWORD		*pSysTimer;						//ϵͳ��ʱ�� 	
	LONG			*OtherRelTrip;					//���������Ѿ�������־
	WORD			wRelRptNum;					//���������������� 
	//**********************��η��ر��沿��*************	yanxs  13-03-08
	WORD			wPickUpReportNum;						//��α������к�
	WORD			wResetReportNum;						//���ر������к�
//	WORD			*RptPickupSerialNum;
//	BOOL			*RptPickupFlag;
	//**********************************************************************	

	LONG			dwPreI1Mod;						//����ǰ����ģֵ 
	LONG			dwPreI3Mod;						//����ǰ����г��ģֵ 
	LONG			dwPreI5Mod;						//����ǰ���г��ģֵ 
	BOOL			PreCurHaveCalcFlag;				//����ǰ�����Ѿ������־ 
	WORD			wRelFCounter;					//���ϼ����� 
	WORD			wRelRCounter;					//���ؼ����� 	
	DWORD			dwRelTimer;						//������ʱ�� 		
	WORD			wExcepWordNum;					//TRel50DIPar �������� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ14����СΪ58 
	WORD			Info;							//������Ϣ 
	
}TRel50DIPar;
/********************************************************
* ����:		ThermalOverLoad
* ����:		��ʱ���ȹ����ɱ���Ԫ�� 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct THERMALOVERLOAD{

	const LONG		*p49KSet;		//��ʱ�޹����ɱ�������
	const LONG		*p49TSet;		//��ʱ�޹�����ʱ�䳣��
	const LONG		*p49ARSet;		//��ʱ�޹����ɸ澯��ֵ
	const LONG		*p49KTSet;		//�豸ֹͣ����ϵ��
	const LONG		*pRatedInSet;		//�����ֵ
	const BYTE		*pRel49CFG;		//��ʱ�޹����ɱ�������
	const BYTE		*pRel49SW;		//��ʱ�޹�������ѹ��
	const LONG		*pICoef;
	const LONG		*pIHACoef;
	
	DWORD			dwKIn;			//K�������ֵ		
	LONG			l49OT;			//��ʱ�޹��������ۼ�ʱ����
	LONG			l49KOT;			//��ʱ�޹�����ɢ��ʱ����
	LONG			lExpT;			//��ʱ�޹��������ۼ�ʱ�䳣��
	LONG			lExpKT;			//��ʱ�޹�����ɢ��ʱ�䳣��
	
	WORD			R49WarnRW;		//�澯�̵�����	
	WORD			R49ActRW;		//�����̵�����
	WORD			R49ActRptNum;	//�����������
	WORD			R49WarnRptNum;	//�澯�������
	WORD			R49RSTRptNum;	//���ر������
	const DWORD		*pSysTimer;		//ϵͳ��ʱ�� 	
	DWORD			dw49Warn;		//�����ۼ�ֵ�澯ֵ
	DWORD			dw49Warn_099;	//���ۼӸ澯����ֵ
	
	LLONG			dw49AThermaVal;		//���ۼ�ֵ
	LLONG			dw49AThermaPreVal;	//ǰ�����ۼ�ֵ
	DWORD			pCurrent;			//��ǰ����ֵ	
	DWORD			PreCurrent;			//��һ�μ������ֵ			
	DWORD			dwRelTimer;			//Ƿ��������ʱ�� 	
	WORD			wExcepWordNum;		//TThermalOverLoad �ȹ����� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ16����СΪ56 
	
	
	
} TThermalOverLoad;
/********************************************************
* ����:		TStructCheck
* ����:		����Ԫ���Լ�ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TSTRUCTCHECK{
		WORD		*pstruct;
		WORD		 bigsize;
		WORD		 acc;
} TStructCheck;
/********************************************************
* ����:		TRel87R
* ����:		���ʲ����Ԫ���ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TREL87R{
	const LONG		*p87RISet;		//�������ֵ
	const LONG		*pIRS1Set;		//�ƶ��յ�1��ֵ
	const LONG		*pIRS2Set;		//�ƶ��յ�2��ֵ	
	const LONG		*pSLP1Set;		//�����ƶ�ϵ��1
	const LONG		*pSLP2Set;		//�����ƶ�ϵ��2	
	const LONG		*pKrh2Set;		//����г���ƶ�ϵ��
	const LONG		*pKrh5Set;		//���г���ƶ�ϵ��
	const LONG		*pKravSet;		//���ʲ�����ڷ�����Ȩϵ��
	const LONG		*pK2avSet;		//����г�������ڷ�����Ȩϵ��

	const BYTE		*p87RCFG;		//��������
	const BYTE		*p87RSW;		//��ѹ��
	const BYTE		*p68IdH2MCFG;		//��������г��������������
	const BYTE		*p68IdH2CCFG;		//��������г���ۺ����������
	const BYTE		*p68IdH2ICFG;		//��������г�������������
	const BYTE		*p68IrH2MCFG;		//�ƶ���������г����������
	const BYTE		*p68IdH5CFG;		//����5��г������
	const BYTE		*p68IdHMulCFG;		//�����ۺ�г����������

	const BOOL		*pIdH2Flag;		//����г��������־
	const BOOL		*pIdH5Flag;		//���г��������־
	const BOOL		*pIrH2Flag;		//�ƶ���������г��������־
	const BOOL      *pIdHMulFlag;	//�ۺ�г��������־

	LONG 			*p87RPI;		//���Ƽ����Ĳ�����ż���ֵ
	TRelElecValPar			*pId;			//��������
	TRelElecValPar			*pIr;			//�ƶ�����

	WORD			ACTRelayWord;					//�����̵����� 
	WORD			StartRelayWord;					//��μ̵����� 
	WORD			LockRelayWord;					//��α����̵����� 
	WORD			RSTRelayWord;					//���ر����̵����� 
	WORD			LoopCtrlRelayWord;				//����ѭ�����Ƽ̵����� 
	
	WORD			*RptSerialNum;					//ϵͳ�������к� 
	const DWORD		*pSysTimer;						//ϵͳ��ʱ�� 
	WORD			wRelRptNum;					//������������ 
	//**********************��η��ر��沿��*************	yanxs  13-03-18
	WORD			wPickUpReportNum;						//��α������к�
	WORD			wResetReportNum;						//���ر������к�
	//**********************************************************************	
	WORD			wRelFCounter;					//���ϼ����� 
	WORD			wRelRCounter;					//���ؼ����� 	
	DWORD			dwRelTimer;						//������ʱ�� 		
	WORD			wExcepWordNum;					//TRel87R ���ʲ ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ7����СΪ68
	WORD			Info;							//������Ϣ 
} TRel87R;
/********************************************************
* ����:		TRel87R
* ����:		���ʲ����Ԫ���ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct T87IALRAM{

	const BYTE		*p87RSW;		//��ѹ��
	const BYTE		*p87R_A_CFG;		//��������
	const BYTE		*p87R_B_CFG;		//��������
	const BYTE		*p87R_C_CFG;		//��������

	const LONG		*p87RIASet; 	//A�������ֵ
	const LONG		*p87RIBSet; 	//B�������ֵ
	const LONG		*p87RICSet; 	//C�������ֵ
	const LONG		*p87RIKSet;		//����Խ�޸澯����

	TRelElecValPar  *pIDA;			//����A
	TRelElecValPar	*pIDB;			//����B
	TRelElecValPar	*pIDC;			//����C
	
	LONG			AlarmCnt;		//����Խ�޸澯ʱ��
	WORD			ARRW;			//����Խ�޸澯�̵�����
	WORD			wRelRptNum;		//����Խ�޸澯�������к�

	LONG			ACnt;			//A�������
	LONG			BCnt;			//B�������
	LONG			CCnt;			//C�������
	BOOL			AAlarmFlag;		//A��澯��־
	BOOL			BAlarmFlag;		//B��澯��־
	BOOL			CAlarmFlag;		//C��澯��־
	BOOL			MakeRptFlag;	//���澯�����־	
	WORD			wExcepWordNum;					//T87IAlarm ����Խ�� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ16����СΪ42 
	WORD			Info;							//������Ϣ 
}T87IAlarm;

typedef struct SUBCHANNELCHECK{

	WORD 	subNo;				//����ͨ�����
	DWORD 	subTimer;			//����ͨ�����Ӷ�ʱ��
	BOOL 	subRxdFlag;			//�����������ݱ�־
	BOOL 	subFaultFlag;		//����ͨ�����ϱ�־
	WORD	subFaultInf;		//����ͨ�����ϱ���INF��
	WORD	subGPSInf;			//����GPSʱ���������INF��
	WORD	subDataInf;			//������������INF��	
	WORD	subDataLossInf;		//����ͨ�����Ϻ����ݶ�ʧINF��
	DWORD	GPSStatus;		  	//GPS����������
	TAbsTime SubClock;			//����ʱ��
	WORD	ComplicateNo;		//�������к�
	
	LONG	U1;
	LONG	U2;
	LONG	U3;
	LONG	U4;
	LONG	It1;
	LONG 	If1;
	LONG	It2;
	LONG	If2;
	LONG	It3;
	LONG	If3;
	LONG	Iat1;
	LONG	Iat2;
	LONG	Iat;
	LONG	AIt1;
	LONG	AIf1;
	LONG	AIt2;
	LONG	AIf2;
	LONG	AIt3;
	LONG	AIf3;
	LONG 	Ip1;
	LONG	Ip2;
	LONG	Ip3;

} subChannelCheck;
typedef struct SUBDATA{
	
	LONG					U1; //����1����
	LONG					U2;
	LONG					U3;
	LONG					U4;
	LONG					IT1;
	LONG					IF1;		
	LONG					IT2;
	LONG					IF2;
	LONG					IT3;
	LONG					IF3;
	LONG					IAT1;
	LONG					IAT2;
	LONG					IAT;
	LONG					AU1;
	LONG					AU2;
	LONG					AU3;
	LONG					AU4;
	LONG					AIT1;
	LONG					AIF1;
	LONG					AIT2;
	LONG					AIF2;
	LONG					AIT3;
	LONG					AIF3;
	LONG					AIAT1;
	LONG					AIAT2;
	LONG					CBStatus;
	LONG 					IP1;
	LONG					IP2;
	LONG					IP3;
}TSubData;
typedef struct FLCOMPLICATE {

	BOOL pickupFlag;      //���β��������־
	BOOL dataOKFlag;		//���β�����������־
	DWORD pickupTime;    //���β������ʱ��
	BYTE rptSerialNo;	//���β��ı�����ţ���Ϊ������������������
						//���в���������¸����
} FLComplicate;

/********************************************************
* ����:		TRel25R
* ����:		��ͬ��Ԫ���ṹ 
* ����: 
* ����:
* ��ע:
********************************************************/ 
typedef struct TREL25R{

	const LONG		*p25_27Set; 	//��բ����ѹ��ֵ
	const LONG		*p25DTSet;		//ͬ�ڼ��ʱ�䶨ֵ
	const LONG		*p25VULSet; 	//��ͬ�ڵ�ѹ���޶�ֵ
	const LONG		*p25VLLSet; 	//��ͬ�ڵ�ѹ���޶�ֵ
	const LONG		*p25VDSet;   	//ͬ������ѹ�ֵ
	const LONG		*p25FDSet;		//ͬ������Ƶ�ֵ
	const LONG		*p25PASet;		//ͬ�ڹ��Ƕ�ֵ
	const LONG		*p25PHSet;		//ͬ�����
	const LONG		*p52CLT;		//��·����բʱ��
	
	const BYTE		*p25RCFG;		//�ֺϼ�ͬ������
	const BYTE		*p79_25RCFG;	//�غ�բ��ͬ������
	const BYTE		*p25RSW;		//�ֺϼ�ͬ����ѹ��

	TRelElecValPar	*pUsyn;
	TRelElecValPar	*pU1;
	LONG 			*pDA;		    //�ǲ�
	LONG 			*pDV; 	//ѹ��
	LONG 			*pDF;	//Ƶ��
	LONG            *pPA;         //��ǰ��
	
	WORD			w52A;			//��·����λ�̵�����
	WORD            w79_25PI;          //ͬ�������̵�����
	WORD            wMCL_25PI;          //ͬ�������̵�����
	WORD            w25LK;          //��ͬ�ڱ����̵�����
	WORD			w25ActRW;		//��ͬ�ڶ����̵�����
	WORD			w25INT;			//��ͬ���жϼ̵�����
	WORD			R25SucRptNum;	//��ͬ�ڳɹ��������
	WORD			R25FalRptNum;	//��ͬ��ʧ�ܱ������
	WORD			R25LKRptNum;	//��ͬ�ڱ������������
	WORD			R25INTRptNum;	//��ͬ�ڱ��жϱ������
	WORD			R25_27RptNum;	//��ѹ�պϱ������
	WORD			*RptSerialNum;					//ϵͳ�������к� 
	const DWORD		*pSysTimer;						//ϵͳ��ʱ�� 


	BOOL			bManSynFlag;		        //�ֺϼ�ͬ�ڱ�־
	BOOL            bJudgeNetend;            //���ж���ɱ�־
	BOOL			bSameNetFlag;		    //ͬ����־
	BOOL			bDiffNetFlag;		    //������־
	BOOL			bSigleNetFlag;		    //������־
	BOOL			bJudgeSameNetPIFlag;		//ͬ���ж���ʱ��α�־
	BOOL			bJudgeDiffNetPIFlag;		//�����ж���ʱ��α�־
	BOOL			bJudgeSigleNetPIFlag;	//�����ж���ʱ��α�־
	BOOL			bBusPTBreakFlag;	    //ĸ��PT���߱�־
	BOOL			bHaveMakeLKRptFlag;		//����ͬ�ڱ����������־
	BOOL			bHaveMakeINTRptFlag;
	BOOL			bDVOutLimitFlag;		//��ѹԽ�ޱ�־

	DWORD            dwInitSingleNetChkTimer;      //�����ж�ʱ�䶨ʱ��
	DWORD            dwInitSameleNetChkTimer;      //ͬ���ж�ʱ�䶨ʱ��
	DWORD            dwInitDiffleNetChkTimer;      //�����ж�ʱ�䶨ʱ��
	DWORD            dwSynClosePulseTimer;         //��ͬ�ں�բ���嶨ʱ��
	DWORD            dwSynCheckTime;               //��ͬ��ʱ�䶨ʱ��

	WORD			wExcepWordNum;					//TRel25R ��ͬ�� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ36����СΪ88 
	WORD			Info;							//������Ϣ 
}TRel25R;

typedef struct TLINEDATA{
	float I0;		//�������������ֵ
	float U0;		//�����ѹ������ֵ
	float I0_5;		//����������г����ֵ
	float U0_5;		//�����ѹ���г����ֵ
	float I0_A;		//�������������ԽǶ�
	float I0_5_A;	//����������г����ԽǶ�
}TLineData;

/********************************************************
* ����:		TRelCTBrokPar
* ����:		CT�����жϲ����ṹ 
* ����: 
* ����:
* ��ע: 	�ýṹ�����жϱ���������CT֮�������ͬ���������һ��ֵ��Ӧ��������
********************************************************/ 
typedef struct TRELCTBROKPAR{

	const BYTE 		*pRelCTBrokCFG;						//CT�������� 
	LONG			*pRelCurSet;						//CT�����жϵ�����С�ż�ֵ
	LONG			*pRelTimeSet;						//CT���߼��ʱ��
	const DWORD		*dwSysTimer;						//ϵͳ��ʱ�� 

	LONG			*pRelCurMax;						//�ж��ô���� 
	LONG			*pRelCurMin;						//�ж���С���� 

	WORD			StartRelayWord;						//CT������μ̵����� 
	WORD			ActRelayWord;						//CT���߶����̵����� 	
	WORD			wRelRptNum;							//CT���߶��������
	WORD			wRelResRptNum;							//CT���߶��������
	
	WORD			byRelFCounter;						//���ϼ����� 
	WORD			byRelRCounter;						//���ؼ����� 
	DWORD			dwRelTimer;							//CT���߶�ʱ�� 	
	WORD			wExcepWordNum;						//TRelCTBrokPar PT���� ���ṹ�������CRCУ��ʱ���ų�������ֽ��������ṹΪ6����СΪ18 

}TRelCTBrokPar;

#endif















