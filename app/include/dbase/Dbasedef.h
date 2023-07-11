/********************************************************************************************************/
/*																										*/
/*        		��򿭷������ɷ����޹�˾																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      Dbasedef.h                                     1.0												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ�������ϵͳ���ݿ������е����ݽṹ															*/
/*																										*/
/* ����																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* ��������ݽṹ																						*/
/*																										*/
/*	TDBaseConfigTable						���ݿ�������												*/
/*	TRunTimeAI								ң��ʵʱ���ݶ���											*/
/*	TRunTimeBI								ң��ʵʱ���ݶ���											*/
/*	TRunTimeCI								���ʵʱ���ݶ���											*/
/*	TRunTimeBO								ң��ʵʱ���ݶ���											*/
/*	TRunTimeAO								ң��ʵʱ���ݶ���											*/
/*	TBurstBIWrap							ң�ű�λ���ݿ�												*/
/*	TBurstBI								ң�ű�λ���ݶ���											*/
/*	TEventBIWrap							SOE���ݿ�													*/
/*	TEventBI								SOE���ݶ���													*/
/*	TRunTimeRM								��������ֵ���ݶ���											*/
/*	TRelayRecord							��������ͷ����												*/
/*	TRelayEvent								�����ֱ��Ķ���												*/
/*	TRelayItem								����������Ŀ����											*/
/*	TDistData								����¼���ļ���������										*/
/*	TRelayEventPtrList						��������Ŀ¼��												*/
/*	TBasicEventPtrList						��������Ŀ¼��												*/
/* ����																									*/
/*																										*/
/*      None																							*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*      Paradef.h                        �����ṹ����													*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0												*/
/********************************************************************************************************/
#ifndef __DBASEDEF_H
#define __DBASEDEF_H

#include "Paradef.h"	/*�����ṹ����*/
#include "clink.h" 

/*������������*/
/*������������*/
#define MASK_ALL				0xffffffff
#define MASK_NULL				0x00000000

#define	MASK_AI					((DWORD)(0x01<<0))
#define MASK_BI					((DWORD)(0x01<<1))
#define MASK_CI					((DWORD)(0x01<<2))
#define MASK_BCD				((DWORD)(0x01<<3))
#define MASK_SPI				((DWORD)(0x01<<4))
#define MASK_DPI				((DWORD)(0x01<<5))
#define MASK_READ_SET			((DWORD)(0x01<<6))
#define MASK_READ_PARA			((DWORD)(0x01<<7))

#define MASK_AIE				((DWORD)(0x01<<8))
#define MASK_BIE				((DWORD)(0x01<<9))
#define MASK_SOE				((DWORD)(0x01<<10))
#define MASK_RELAYALARMEV		((DWORD)(0x01<<11))
#define MASK_RELAYSTARTEV		((DWORD)(0x01<<12))
#define MASK_RELAYACTIONEV		((DWORD)(0x01<<13))
#define MASK_DISTDATA			((DWORD)(0x01<<15))

#define MASK_AIFRZ				((DWORD)(0x01<<16))
#define MASK_CIFRZ				((DWORD)(0x01<<17))
#define MASK_BO					((DWORD)(0x01<<18))
#define MASK_AO					((DWORD)(0x01<<19))
#define MASK_SYNC				((DWORD)(0x01<<20))
#define MASK_WRITE_SET			((DWORD)(0x01<<21))
#define MASK_WRITE_PARA			((DWORD)(0x01<<22))

#define MASK_DEVSTATUS			((DWORD)(0x01<<24))
#define MASK_TIME				((DWORD)(0x01<<31))

/*��������ļ�����*/
#define DB_FILE_PARACFG			0x01
#define DB_FILE_RELAYDISP		0x02
#define DB_FILE_RELAYDATA		0x03
#define DB_FILE_RELAYEV			0x04
#define DB_FILE_BASICEV			0x05

/*���������������*/
#define DB_PARA_DAI				0x01
#define DB_PARA_DI				0x02
#define DB_PARA_AI				0x03
#define DB_PARA_BI				0x04
#define DB_PARA_CI				0x05
#define DB_PARA_BO				0x06
#define DB_PARA_AO				0x07
#define DB_PARA_SER				0x08
#define DB_PARA_CAN				0x09
#define DB_PARA_ETH				0x10
#define DB_PARA_SYS				0x11

#define DB_PARA_DIST			0x21
#define DB_PARA_CFG				0x22
#define DB_PARA_SET				0x23
#define DB_PARA_SYSTEMSET		0x24
#define DB_PARA_CF				0x25
#define DB_PARA_RM				0x26
#define DB_PARA_CHANNELSRC		0x27
#define DB_PARA_CHANNEL			0x28
#define DB_PARA_ZERODRIFT		0x29
#define DB_PARA_CHANNELFULLVALUE	0x2A
#define DB_PARA_MBO				0x2B
#define DB_PARA_FL				0x2C

//����ϵͳ��������
#define DB_SYSPARA_ETHA			0x01
#define DB_SYSPARA_ETHB			0x02
#define DB_SYSPARA_ADDR			0x03
#define DB_SYSPARA_PASSWORD		0x04
#define DB_SYSPARA_SERPARA		0x05
#define DB_SYSPARA_CANPARA		0x06
#define DB_SYSPARA_ETHPARA		0x07
#define DB_SYSPARA_CIOPT		0x08
#define DB_SYSPARA_LANGUAGE		0x09
#define DB_SYSPARA_FDSHOW		0x0A
#define DB_SYSPARA_CTRLWORD		0x0B
#define DB_SYSPARA_IRGBTYPE		0x0C

/*�������������������*/
#define DB_DATA_SET				0x01
#define DB_DATA_SETAREACUR		0x02
#define DB_DATA_SYSSET			0x03
#define DB_DATA_CF				0x04
#define DB_DATA_SW				0x05

/*������������¼�����*/
#define DB_EVENT_RACT			0x00
#define DB_EVENT_RALM			0x01
#define DB_EVENT_RSTT			0x02
#define DB_EVENT_BIE			0x03
#define DB_EVENT_SOE			0x04
#define DB_EVENT_MISC			0x05
#define DB_EVENT_LOG			0x06
#define DB_EVENT_RELAYPTR		0x07
#define DB_EVENT_BASICPTR		0x08

//����ȱʡ���ݸ���
#define	DB_DEFAULT_DAINUM		10		
#define	DB_DEFAULT_DINUM		(34+13+10)		
#define	DB_DEFAULT_AINUM		4		
#define	DB_DEFAULT_BINUM		(34+13+10)		
#define	DB_DEFAULT_CINUM		2		
#define	DB_DEFAULT_AONUM		10		
#define	DB_DEFAULT_BONUM		20		

//ʵʱң�ⶨ��

#define AI_STATUS_ONLINE		((WORD)(0x01<<0))
#define AI_STATUS_RESET			((WORD)(0x01<<1))
#define AI_STATUS_COMBRK		((WORD)(0x01<<2))
#define AI_STATUS_MODIFY		((WORD)(0x01<<3))

typedef struct
{
	long				lValue;					//ң��ֵ		
	WORD				wStatus;				//״̬	D0=1����		0	����
												//		D1=1������		0	����                
												//		D2=1ͨѶ�ж�    0 	����             
												//		D3=1ǿ���޸�    0 	����
}TRunTimeAI;

//ʵʱң�Ŷ���

#define BI_STATUS_ONLINE		((BYTE)(0x01<<0))
#define BI_STATUS_RESET			((BYTE)(0x01<<1))
#define BI_STATUS_COMBRK		((BYTE)(0x01<<2))
#define BI_STATUS_MODIFY		((BYTE)(0x01<<3))

typedef struct
{
	BYTE				bStatus;				//ң��״̬,���λ��ʾң�ŵĵ�ǰ״̬
												//D0=1���� 0 ����
												//D1=1������0 ����               
												//D2=1ͨѶ�ж� 0 ����            
												//D3=1ǿ���޸� 0 ����           
												//D4~6 ����                       
												//D7��ǰ״̬
}TRunTimeBI;

//ʵʱ��ȶ���
#define CI_STATUS_ONLINE		((WORD)(0x01<<0))
#define CI_STATUS_RESET			((WORD)(0x01<<1))
#define CI_STATUS_COMBRK		((WORD)(0x01<<2))
#define CI_STATUS_MODIFY		((WORD)(0x01<<3))
#define CI_STATUS_OVERFLOW		((WORD)(0x01<<5))

typedef struct
{
	WORD				bStatus;				//״̬	D0=1���� 0 ����
												//		D3=1ǿ���޸� 0 ����            
												//		D5=1 ��� 0 ����               
												//		D6~15 ����
	DWORD				dwValue;				//���ֵ	
}TRunTimeCI;

#define	TRunTimeBCD				void
#define TRunTimeSPI				void

//ʵʱң�ض���

#define BO_STATUS_ONLINE		((WORD)(0x01<<0))
#define BO_STATUS_RESET			((WORD)(0x01<<1))
#define BO_STATUS_REMOT			((WORD)(0x01<<3))
#define BO_STATUS_LOCAL			((WORD)(0x01<<4))

#define BO_STATUS_INDSHIFT		5
#define BO_STATUS_INDMASK		((WORD)(0x07<<5))
#define GET_BO_STATUS(s)		(((s)&BO_STATUS_INDMASK)>>BO_STATUS_INDSHIFT)

#define BO_STATUS_READY			0
#define BO_STATUS_SELECT		1
#define BO_STATUS_SELSUCCESS	2
#define BO_STATUS_EXE			3
#define BO_STATUS_DIREXE		4

#define BO_RUNTIME_TIMEOUT		20

typedef struct
{
	WORD				wStatus;				//״̬��D0=1 ���ߣ����Ƹ��������豸�����������Ըõ�Ķ�����������������ȷ������=0 ���ߣ��豸�������������Ըõ�Ķ������������ܳɹ�
												//D1=1 ��������=0������
												//D3=1 Զ�����ơ�������������ܿ���Դ���豸���䵱ǰ״̬һʾ��STATEλ��=0����
												//D4=1 ���ؿ��ơ�������������ܿ��ڱ��豸���䵱ǰ״̬һʾ��STATEλ
												//����������D5~D7��ʾ��	
												//	=0 ң�ؾ���
												//	=1 ����ң��Ԥ�ã��ȴ�Ԥ�÷���
												//	=2 Ԥ�óɹ����ȴ�ִ��
												//	=3 ����ң��ִ�У��ȴ�ִ�з���
	WORD				wTimeOutCnt;			//��ʱ����											
	DWORD				dwAppID;				//Դ�����,����ң�ص������
	TAbsTime			tLastExecTime;			//����ʱ��,��һ��ң�ز���ʱ��
}TRunTimeBO;

#define	TRunTimeTQ				void

#define OPT_STATUS_READY			0
#define OPT_STATUS_SET				1
#define OPT_TIMEOUT_NUM				60

typedef struct
{
	WORD				wStatus;				//	=0 ��������
												//	=1 ��������
												//	=2 ����ȷ��
	WORD				wTimeCnt;				//��ʱ��ʱ����λS											
	DWORD				dwOption1;				//ѡ��1
	DWORD				dwOption2;				//ѡ��2
	DWORD				dwAppID;				//Դ�����,����ң�ص������
}TRunTimeOPT;

//ʵʱң������
#define AO_STATUS_ONLINE		((DWORD)(0x01<<0))
#define AO_STATUS_RESET			((DWORD)(0x01<<1))
#define AO_STATUS_REMOT			((DWORD)(0x01<<3))
#define AO_STATUS_LOCAL			((DWORD)(0x01<<4))

#define AO_STATUS_INDSHIFT		5
#define AO_STATUS_INDMASK		((DWORD)(0x07<<5))
#define GET_AO_STATUS(s)		(((s)&BO_STATUS_INDMASK)>>BO_STATUS_INDSHIFT)

#define AO_STATUS_READY			0
#define AO_STATUS_SELECT		1
#define AO_STATUS_SELSUCCESS	2
#define AO_STATUS_EXE			3

typedef struct
{
	WORD				wStatus;				//״̬	D0=1 ���ߣ����Ƹ��������豸�����������Ըõ��ң�����������ȷ������=0 ���ߣ��豸�������������Ըõ��ң������ܳɹ�
												//		D2=1 ��������=0������
												//		D3=1 Զ��ǿ�����ݡ�������������ܿ���Դ���豸���䵱ǰ״̬һʾ��STATEλ��=0����
												//		D4=1 ����ǿ�����ݡ�������������ܿ��ڱ��豸���䵱ǰ״̬һʾ��STATEλ

												//����������D5~D7��ʾ��
												//	=0 ң������
												//	=1 ����ң��Ԥ�ã��ȴ�Ԥ�÷���
												//	=2 Ԥ�óɹ����ȴ�ִ��
												//	=3 ����ң��ִ�У��ȴ�ִ�з���
	DWORD				dwAppID;				//Դ����ŷ���ң���������
	TAbsTime			tLastExecTime;			//����ʱ����һ��ң������ʱ��
}TRunTimeAO;

//��������ֵ���п�
#define RM_STATUS_ONLINE		((DWORD)(0x01<<0))
#define RM_STATUS_MODIFY		((DWORD)(0x01<<3))

typedef struct
{
	long					lValue;						//����ֵ			
	WORD					wStatus;					//״̬��D0=1����        0 ����
}TRunTimeRM;											//		D3=1ǿ���޸�    0 	����


#define MAX_BIE_NUM					1024	

//ң��״̬����
#define	DB_BI_OPEN					0
#define	DB_BI_CLOSE					1

#define BI_OPEN						DB_BI_OPEN
#define BI_CLOSE					DB_BI_CLOSE

//ң�ű�λ�¼�����
typedef struct
{
	WORD				wNo;					//ң�����	
	BYTE				bStatus;				//��λ��״̬	0x00���� 0xFF����
}TBurstBI;

//ң�ű�λ�¼���
typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TBurstBI				pEvent[MAX_BIE_NUM];		//�����			
}TBurstBIWrap;

#define MAX_SOE_NUM			1024	

//SOE
typedef struct
{
	WORD					wNo;						//ң�����	
	BYTE					bStatus;					//��λ��״̬,0x00���� 0xFF����
	TAbsTime				stTime;						//����ʱ��,����ʱ��
}TEventBI;

//SOE�¼���
typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TEventBI				pEvent[MAX_SOE_NUM];		//�����			
}TEventBIWrap;


#define MAX_AIE_NUM            4096 /*ÿ���豸���õ�ң���¼��⻷�ͻ���������*/

/*----------------------------------------------------------------------------------------*/
/* ң���¼�����														              */
/*----------------------------------------------------------------------------------------*/
typedef struct{
	WORD		nNo;					/*ң���¼���Ӧ�ĵ��*/
	BYTE		bStatus;				/*ң���״̬*/
	long		lValue;					/*ң��ֵ*/
	TAbsTime	stTime;					/*ң���¼���ʱ��*/
}TEventAI;

/*----------------------------------------------------------------------------------------*/
/* ң���¼����ͻ���������													              */
/*----------------------------------------------------------------------------------------*/
typedef struct{
	DWORD		dwMagic;				//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD		hSem;					//�ź���ID,������Դ����
	WORD		nMax;					//�����С		
	WORD		nHeadIndex;				//ͷָ��			
	WORD		nTailIndex;				//βָ��			
	TEventAI	pEvent[MAX_AIE_NUM];	/*�������*/
}TEventAIWrap;

//¼������
#define		MAX_DISTBLOCK_NUM				4			//������¼�����ݿ����
#define		MAX_DISTBLOCK_LEN				(256*1024)	//������¼�����ݿ鳤��

#define		DIST_STATUS_IDLE				0x0000		//����
#define		DIST_STATUS_BUSY				0x55AA		//���ڲ���BUSY 59535542
#define		DIST_STATUS_SAVE				0x33CC		//׼������SAVE 45564153

typedef struct
{
	WORD		wStatus;			//  CACHE״̬
	WORD		wPerSize;			//  ���ݵ�Ԫ����
	DWORD		dwPrevSampleNum;	//	����ǰ��������
	DWORD		dwRearSampleNum;	//	�������������
	DWORD		dwMask;				//  ͨ������
	DWORD		dwSamplePtr;		//  ��������ָ��
	TAbsTime	tTime;				//  ����ʱ��
}TDistHead;

//��������
#define		MAX_RECACHE_NUM					16			//��󲢷���������CACHE�ռ����
#define		MAX_SEGACT_NUM					32			//ÿ��CACHE�ռ����֧�ֵķֱ��ĸ���
#define 	MAX_RSTT_NUM					256			//��������������󱣴����
#define 	MAX_RALM_NUM					128			//�����澯������󱣴����
#define 	MAX_RACT_NUM					128			//��������������󱣴����
#define		MAX_RACT_LENGTH					512			//��������������󳤶�
#define		MAX_RSTT_LENGTH					304			//��������������󳤶�
#define		MAX_RALM_LENGTH					224	//64			//�����澯������󳤶�

//��������ͷ
#define		RELAYEVENT_TYPE_ACTION		0
#define		RELAYEVENT_TYPE_ALARM		1
#define		RELAYEVENT_TYPE_START		2	

#define		RELAYEVENT_STATUS_MASK		0x0F	
#define		RELAYEVENT_STATUS_HAVEDIST	0x80	
#define		RELAYEVENT_STATUS_ACT		1	
#define		RELAYEVENT_STATUS_RLS		0	
		

//��������ͷ
typedef struct
{
	WORD					nINF;						//INF						
	WORD					bStatus;					//״̬��1������������0����������
	WORD					FD_Num;						//���ϵ�������		
	WORD					AbsTimeHi;					//����ʱ�䡣����ʱ��
	DWORD					AbsTimeLo;					//����ʱ�䡣����ʱ��
}TRelayEventHead;

typedef struct
{
//	DWORD					dwDevType;					//�豸����	
//	DWORD					dwRepType;					//�������͡�0:����1:�澯2:����
	WORD					Num;						//�ֱ��ĸ���	
	WORD					wFaultNo;					//���ϱ��
	TRelayEventHead			tAction;					//�ֱ������ݡ��ɱ䳤��
}TRelayRecordHead;

//���������ֱ���
typedef struct
{
	TRelayEventHead			stHead;
//	WORD					FD_Index[MAX_FD_NUM];		//���ϵ�������,������󳤶�Ԥ��
	long					FD_Value[MAX_FD_NUM];		//���ϵ�����ֵ,������󳤶�Ԥ��
}TRelayActionFix;


//������������CACHE�ռ�ṹ(�м����)

#define		RELAY_CACHE_HOLDTIME				300
#define		RELAY_CACHE_STATUSMASK				0x000F
#define		RELAY_CACHE_MAKESTART				((WORD)(0x01<<4))
#define		RELAY_CACHE_READY					0
#define		RELAY_CACHE_START					((WORD)(0x01<<0))
#define		RELAY_CACHE_ACTED					((WORD)(0x01<<1))
#define		RELAY_CACHE_END						((WORD)(0x01<<2))

typedef struct
{
	WORD					bStatus;					// D0~D3״̬	0������
														//		1���Ѽ�¼����������
														//		2���Ѽ�¼����������
														//		3: ��������
														// D5:��Ҫ��¼��������
														//	������Ч
	WORD					nSeq;						//ȫ�����
	WORD					nCnt;						//�ֱ��ļ���
	DWORD					dwTimeOut;					//�����ʱ�����ʱ�꣬��¼���һ���ֱ��ĵ�ʱ�꣬��ֹ��ʱ�䲻��������CACHE����
	TRelayActionFix			pAction[MAX_SEGACT_NUM];	//������������	
}TRelayEventCache;


//����������д�ֱ��Ľṹ(�м����)
typedef struct
{
	WORD					nINF;						//INF					
	WORD					bStatus;					//״̬��1������������0����������
	TAbsTime				stTime;						//����ʱ�䡣����ʱ��
	long					FD_Value[MAX_FD_NUM];		//���ϵ�����ֵ�������������ϵ�������
}TRelayEventItem;

//���������¼�
typedef union
{
	BYTE				aEvent[MAX_RSTT_LENGTH];
	TRelayRecordHead	tEvent;
}TRelayStartEv;

//�����澯�¼�
typedef union
{
	BYTE				aEvent[MAX_RALM_LENGTH];
	TRelayRecordHead	tEvent;
}TRelayAlarmEv;

typedef struct
{
	WORD				nIndex;							//״̬λ����
	WORD				nAlarmINF;						//�澯��Ϣ���INF
	WORD				nRecoverINF;					//�ָ���Ϣ���INF
}TSCAlarmItem;

//���������¼�
typedef union
{
	BYTE				aEvent[MAX_RACT_LENGTH];
	TRelayRecordHead	tEvent;
}TRelayActionEv;

typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TRelayStartEv			pEvent[MAX_RSTT_NUM];		//�����			
}TRelayStartWrap;

typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TRelayAlarmEv			pEvent[MAX_RALM_NUM];		//�����			
}TRelayAlarmWrap;

typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TRelayActionEv			pEvent[MAX_RACT_NUM];		//�����			
}TRelayActionWrap;

//¼���ļ�Ŀ¼��
#define	DIST_INDEX_INIT			0x55AA
#define	DIST_INDEX_NAMELEN		32

typedef struct
{
	WORD					wMagic;						//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��	
    BYTE        Year;                   /*��*/
    BYTE        Month;                  /*��*/
    BYTE        Day;                    /*��*/
    BYTE        Hour;                   /*ʱ*/
    BYTE        Minute;                 /*��*/
    BYTE        Second;                 /*��*/
	BYTE		MSecondHi;				/*����*/
	BYTE		MSecondLo;				/*����*/
}TDistIndex;

//�����¼�ָ��Ŀ¼��
typedef struct
{
	WORD					nMaxRalayAction;			//���������¼��洢����	
	WORD					nRelayActionT;				//���������¼�βָ��		
	WORD					nRelayActionH;				//���������¼�ͷָ��		
	WORD					nRelayActionLEDR;			//���������¼�LED��ָ��	
	WORD					nRelayActionNet1R;			//���������¼�NET1��ָ��	
	WORD					nRelayActionNet2R;			//���������¼�NET2��ָ��	
	WORD					nMaxRalayAlarm;				//�����澯�¼��洢����	
	WORD					nRelayAlarmT;				//�����澯�¼�βָ��		
	WORD					nRelayAlarmH;				//�����澯�¼�ͷָ��		
	WORD					nRelayAlarmLEDR;			//�����澯�¼�LED��ָ��	
	WORD					nRelayAlarmNet1R;			//�����澯�¼�NET1��ָ��	
	WORD					nRelayAlarmNet2R;			//�����澯�¼�NET2��ָ��	
	WORD					nDistCnt;					//�Ŷ����ݼ�����			
	WORD					Rsv[7];						//����					
}TRelayEventPtrList;

//�����¼�ָ��Ŀ¼��
typedef struct
{
	WORD					nMaxSOE;					//SOE�洢����			
	WORD					nSOET;						//SOEβָ��			
	WORD					nSOEH;						//SOEͷָ��			
	WORD					nMaxOptRecord;				//������¼�洢����	
	WORD					nOptRecordT;				//������¼βָ��		
	WORD					nOptRecordH;				//������¼ͷָ��		
	WORD					nMaxRelaySart;				//������������洢����
	WORD					nRelayStartT;				//������������βָ��	
	WORD					nRelayStartH;				//������������ͷָ��	
	WORD					Rsv[7];						//����				
}TBasicEventPtrList;


//�ļ�����
#define	FILE_MAXLEN_NAME		16
#define	FILE_TYPE_PARACFG		0x01
#define	FILE_TYPE_PARADISP		0x02
#define	FILE_TYPE_RELAYDISP		0x03
#define	FILE_TYPE_RELAYDATA		0x04

#define FILEMAP_FLAGS_INIT			((WORD)(0x01<<0))
#define FILEMAP_FLAGS_MODIFY		((WORD)(0x01<<1))
#define FILEMAP_FLAGS_MODEND		((WORD)(0x01<<2))

typedef struct
{
	char					sFileName[FILE_MAXLEN_NAME];				//�ļ���,��¼�������ļ���������ΪDISTxxxx������xxxx����¼�����ݹ��ϵ��
	WORD					wFileType;					//�ļ�����			
	WORD					wFlag;						//�ļ���־��D0��=1 �ļ���ʼ��
														//			D1��=1 �ļ��ı�
														//			D3��=1 �����ļ�����
														//			��������
	void					*pRamData;					//RAM����ָ�롣�������е��ļ���ַ
	DWORD					dwFlashOff;					//Flash�ļ�ƫ�ơ�FLASH���е��ļ�ƫ�ƣ��ļ�ϵͳ�ò��ϣ�
	DWORD					nSize;						//�ļ�����			
	TAbsTime				absTime;					//����ʱ�䡣�ϴ��ļ�����ʱ��
	WORD					wCheck;						//У���롣�ۼӺ�У��
}TFileMap;

//�����޸Ĳ���ָ��
typedef struct
{
	TParaConfigTable		*pConfig;
	TDAIPara				*pDAIAttr;					//ģ�����������			
	TAIPara					*pAIAttr;					//ң������				
	TDIPara					*pDIAttr;					//���ֲ���������			
	TBIPara					*pBIAttr;					//ң������				
	TCIPara					*pCIAttr;					//�������				
	TBOPara					*pBOAttr;					//ң������				
	TAOPara					*pAOAttr;					//ң������				
	TSerialPara				*pSerial;					//���ڲ���					
	TCanBusPara				*pCANBus;					//CAN����						
	TEthPortPara			*pEthernet;					//��̫������	
	TMBO_Disp				*pMBODisp;					//����λ����������
	TRelayDataTable			*pRelayCfg;					//�������ñ�
	TFL_Disp				*pFLDisp;					//������������
	DWORD					*pSWData;					//��ѹ������ָ��		
	DWORD					*pCFGData;					//����������ָ��		
	float					*pSETData;					//������ֵ����ָ��	
	float					*pSystemSETData;			//������ֵ����ָ��	
	long					*pCFData;					//ϵ������ָ��		
}TMapDataList;

//���ݿ����ñ�
#define		DB_MAXLEN_PASSWORD	8		
#define		DB_CONTROL_BO		((DWORD)(0x01<<0))
#define		DB_CONTROL_AO		((DWORD)(0x01<<1))
#define		DB_CONTROL_TIME		((DWORD)(0x01<<2))
#define		DB_CONTROL_AICAL	((DWORD)(0x01<<3))
#define		DB_CONTROL_CICAL	((DWORD)(0x01<<4))
#define		DB_CONTROL_EVREP	((DWORD)(0x01<<5))
#define		DB_CONTROL_MISC		((DWORD)(0x01<<7))

#define		DB_RELAY_FDSECOND	((DWORD)(0x01<<8))		//���ϵ������ն������ݴ洢
#define		DB_RELAY_CACLINT	((DWORD)(0x01<<9))		//�Ƿ������ֵ��
#define		DB_IRGB_DK3511		((DWORD)(0x01<<10))		//GPSʱ��ԴΪDK3511

#define		SET_AREANO_INVALID	0xAAAA

typedef struct
{
	DWORD				dwDevID;				//�豸��ʶ	
	char				chHostName[HOST_NAME_LEN];//������		
	WORD				wAddress;				//���豸��ַ				
	WORD				wMainAddr;				//��λ����ַ				
	WORD				wType;					//ģ������	
	WORD				wCodeCrc;				//����CRC��
	char				chPassWord1[8];			//����1					
	char				chPassWord2[8];			//����2					
	DWORD				dwControl;				//�����֡�	D0=1ң������
												//			D1=1 ң������
												//			D2=1 ��������
												//			D3=1 ң���ϴ���ϵ��
												//			D4=1 ����ϴ���ϵ��
												//			D5=1 �쳣���������ϱ�
												//			D6=1 ��ȶ�ʱ�Ϸ���վ
												//			D7=1 �Ƿ���������¼�
												//			D8=1 ���ϵ�������һ��ֵ�洢
												//			D9=1 �Ƿ������ֵ��
												//			D10=1 GPSʱ��ԴΪDK3511������ΪKF6511
	WORD				nDAI;					//ֱ��ģ����������		
	WORD				nDI;					//ֱ�����ֲ��������		
	WORD				nAI;					//ң�����				
	WORD				nBI;					//ң�Ÿ���				
	WORD				nCI;					//��ȸ���				
	WORD				nSPI;					//SPI����					
	WORD				nBCD;					//BCD����					
	WORD				nBO;					//ң�ظ���				
	WORD				nAO;					//ң������				
//	WORD				nAIType;				//ң��������				
	WORD				nSerNum;				//���ڸ���				
	WORD				nCanNum;				//CAN���˿ڸ���			
	WORD				nEthNum;				//��̫���˿ڸ���			
	WORD				nSW;					//��ѹ�����		
	WORD				nCFG;					//�����ָ���		
	WORD				nSET;					//��ֵ����		
	WORD				nSETArea;				//��ֵ������		
	WORD				nSETAreaCur;			//��ǰ��ֵ����		
	WORD				nSETAreaCurB;			//��ǰ��ֵ���ŷ���	
	WORD				nSystemSET;				//������ֵ����	
	WORD				nRT;					//��ȸ���		
	WORD				nCF;					//ϵ������		
	WORD				nRM;					//��������ֵ����	
	WORD				nRE;					//������������	
	WORD				nRW;					//�̵����ָ���	
	WORD				nTaskAPP;				//���ݿ��������			
	WORD				nDataName;				//���������ܸ���			
	WORD				nTC;					//��������
	WORD				nLanguageNo;			//���Ա��
	WORD				nMBO;					//����λ�������
	WORD				nFL;					//���������
	TRunTimeAI*			pAI;					//ң��ʵʱ����ָ��		
	TRunTimeRM*			pRM;					//��������ֵ����ָ��		
	TRunTimeBI*			pBI;					//ң��ʵʱ����ָ��		
	TRunTimeCI*			pCI;					//���ʵʱ����ָ�룬NvRAM�洢
	TRunTimeBCD*		pBCD;					//BCDʵʱ����ָ��			
	TRunTimeSPI*		pSPI;					//SPIʵʱ����ָ��			
	TRunTimeAO*			pAO;					//ң��ʵʱ����ָ��		
	TRunTimeBO*			pBO;					//ң��ʵʱ����ָ��		
	TRunTimeTQ*			pTQ;					//ͬ��ʵʱ����ָ��		
	TRunTimeOPT*		pParaOpt;				//����������¼		
	TRunTimeOPT*		pSWOpt;					//��ѹ�������¼		
	TRunTimeOPT*		pCFGOpt;				//�����ֲ�����¼	
	TRunTimeOPT*		pSETOpt;				//������ֵ������¼		
	TRunTimeOPT*		pSETAreaOpt;			//������ֵ��������¼		
	TRunTimeOPT*		pCFOpt;					//ϵ��������¼		
	DWORD*				pSWData;				//��ѹ������ָ��			
	DWORD*				pCFGData;				//����������ָ��			
	long*				pSETDataFix;			//������ֵ����ָ��		
	float*				pSETDataFloat;			//������ֵ����ָ��		
	float*				pSystemSETData;			//������ֵ����ָ��		
	long*				pCFData;				//ϵ������ָ��			
	TBurstBIWrap*		pBurstBIWrap;			//ң�ű�λ����ָ�룬NvRAM�洢
	TEventBIWrap*		pEventBIWrap;			//ң�������ָ�룬NvRAM�洢
	TRelayStartWrap*	pRSTTWrap;				//���������¼�����ָ��		
	TRelayAlarmWrap*	pRALMWrap;				//�����澯�¼�����ָ��		
	TRelayActionWrap*	pRACTWrap;				//���������¼�����ָ��		
	TDAIPara*			pDAIPara;				//ģ�������������			
	TDIPara*			pDIPara;				//���ֲ��������
	TAIPara*			pAIAttr;				//ң������				
	TBIPara*			pBIAttr;				//ң������				
	TCIPara*			pCIAttr;				//�������				
	TBOPara*			pBOAttr;				//ң������				
	TAOPara*			pAOAttr;				//ң������				
	TSW_Disp*			pSWDisp;				//��ѹ������ָ��			
	TCFG_Disp*			pCFGDisp;				//����������ָ��			
	TSET_Disp*			pSETDisp;				//������ֵ����ָ��		
	TSysSET_Disp*		pSystemSETDisp;			//������ֵ����ָ��		
	TCF_Disp*			pCFDisp;				//ϵ������ָ��			
	TRM_Disp*			pRMDisp;				//���ϵ�������ָ��			
	TRE_Disp*			pREDisp;				//�����¼�����ָ��			
	TDist_Disp*			pDistPara;				//¼������
	TAC_Disp*			pACPara;				//¼��ģ��ͨ������
	TDC_Disp*			pDCPara;				//¼������ͨ������
	char*				pDataName;				//�������Ʊ�
	TSerialPara*		pSerial;				//���ڲ���				
	TCanBusPara*		pCAN;					//CAN����					
	TEthPortPara*		pEthernet;				//��̫������
	TFileMap*			pBasicParaFileMap;		//��������ļ�������ָ��	
	TFileMap*			pRelayDispFileMap;		//���������ļ�������ָ��	
	TFileMap*			pRelayDataFileMap;		//���������ļ�������ָ��	
	TFileMap*			pBasicEventFileMap;		//���������ļ�������ָ��	
	TFileMap*			pRelayEventFileMap;		//���������ļ�������ָ��
	TMapDataList		tMapData;				//����������	
	TMapDataList		tModData;				//�����޸�������			
	TTC_Disp*			pTCDisp;				//����������
	TMBO_Disp*			pMBODisp;				//����λ���������
	TFL_Disp*			pFLDisp;				//�����������
	TEventAIWrap*		pEventAIWrap;			//ң�������ָ�룬RAM�洢
	void*				pRsv[7];				//���á�Ԥ��7��ָ��
}TDBaseConfigTable;


#define VER_INF_LEN			16		//�汾��Ϣ����(���)
#define HW_VER_INF_LEN		8		//Ӳ���汾��Ϣ
#define USER_INF_LEN		126		//�û�������Ϣ����
#define CODE_FILE_BUF_LEN	20000	//�ļ���ȡ���泤��

//Ӧ�ó���汾��Ϣ
typedef struct
{
	WORD wCheckSum;					//Ӧ�ó����ļ�У���룬16λ�ۼӺ�,���ݴ�
	TSysTime CompileTime;			//�ļ�����ʱ��,Ϊ��ʼ����ʱʱ��
	TSysTime EditTime;				//�ļ�����ʱ�䣬�����ļ�����ʱ��
	char SoftWareVer[VER_INF_LEN];	//����汾��Ϣ���ַ�����������:10.2.20
	char HardWareVer[HW_VER_INF_LEN];	//Ӳ���汾��Ϣ���ַ�����������:10.2.20
	char UserInf[USER_INF_LEN];		//�������Ӧ���û�������Ϣ��������:����15���ߵ�����Ŀ��vxwroks����ϵͳ��586CPU��16·����
	DWORD Res[8];					//����,��һ��ΪCRCУ�鳤��(�����ļ���ʼ��16���ֽ�)
}TCodeFileInf;

// ���ڹ�Լ����
typedef struct
{
	BYTE		byNo;		//���ڱ��
	BYTE		byMode;		//���ڿ���ģʽ										
	BYTE		byDriver;	//���ڹ�Լ���
	BYTE		byBaud; 	//���ڲ�����
}TSerialConfig;

// CANBUS��Լ����
typedef struct
{
	BYTE		byNo;		//CAN�ڱ��
	BYTE		byDriver;	//��Լ���
	WORD		wBaud; 		//ͨ������
}TCanbusConfig;

// ��̫�����ӹ�Լ����
typedef struct
{
	BYTE		byNetCardNo;//�������
	BYTE		byLinkNo;	//���ӱ��
	BYTE		byDriver;	//��Լ��ţ�ͬ��
	BYTE		byMode;		//����ģʽ,
	DWORD		dwSideIP;	//�Զ�IP��ַ
	WORD		wPort;		//�˿ں�
}TEthLinkConfig;

// ң��ע���ڵ�
typedef struct
{
	DWORD	hTaskID;
	WORD	nBO;
}TBORegisterInfo;

#endif
