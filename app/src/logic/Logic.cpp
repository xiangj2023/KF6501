/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Logic.c
* 
*   ���ģ�飺�ɱ���߼�����
*
* 
*	����	ʵ�ֿɱ���߼�����ģ��
*
* 	���ڼ̵����ز����ļ���˵��:
*		����ȷ��Ч�������ؼ�����
*		1)�̵���д������д��ֵ֮ǰ�Ѿ�ֵ���浽��λ�� ����ֻ������̵��������ϣ������̵����������λ��
*		2)�߼����̵�ÿ��ˢ�������ڼ̵���ֵ��ˢ�¡�
*		  �߼������Զ�ˢ��=��ߵļ̵���;�Զ�ˢ�¿�������Ӽ̵���
* 		  ���Գ�����������⣬��������=��߳��ֵļ̵������ز������ǲ�֧�ֵģ�����ÿ���ڶ������д����ˢ��(����ֵ�Ƿ�仯)
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	28/11/2008		��ʼ����
* 		kagnqian 	21/04/2010		1.����ʱ��̵�������ѹ�����������ʵ��
* 									2.�޸�ϵͳ�����ӿ�RELAY_SYSFUN_PTR�Ķ��壬
* 									 ��1��������Ϊ2�������� ���溯������ѹ�����������2������
*		kangqian	12/10/2010	1.���Ӹ�ʽ���źż̵���eeprom�ĺ���FormatSignalEEprom
*								2.�źż̵������浽eeprom�޸�Ϊ��������ֵ2��
*								3.��ȡ�߼������ļ��ȶ��ļ�ͷ�� �õ��ļ���С�ٶ��߼����ڴ�
*		kangqian	06/16/2011	1.���ƺ������������Ӽ̵������������Ӽ̵���
*		kangqian	09/15/2011	1.����nvram�м�̵������Ա��̵ּ���
*		kangqian	12/13/2011	1.���ӵ��Լ̵���������ݣ���Ҫ��DebugRunLogic����
*		kangqian 	12/20/2011  1.�޸�ResetLogicParseModule���ݣ������������߼�.plc�ļ�ʱ����
*								   �ú�������ʹ���߼�����Ͷ��ʹ�ã�����Ҫ����
*		kangqian 	02/08/2012	1.���ӹ��ڵ��Կ���DBIn�����Կ���DBOut�Ĵ�����
*		kangqian	02/13/2012	1.����һ�ֵ��ؿ��Ƽ̵���LocalCtrlExe
*		kangqian 	03/05/2012	1.��TCommonFileHead�е�wVersion��ΪDWORD,ԭ����wFlagȥ������Ӧ�޸�GetPLCDBVer()
*		kangqian	03/27/2012	1.���Ӽ����̵�������غ������������ݼ������飬�޸ıȽϼ̵���ʹ�Ƚϲ���Ҳ�����Ǽ����̵���
*	�����ļ��ṹ�仯���ش��޸�	2.�޸�kTLogicPara_Expression���ݽṹ��������չ����paraex, paraex1����Ӧ�޸�Ĭ���߼���
*								3.�Ƚϼ̵����޸ģ��Ƚ����ݿ����ǳ�������������ֵ����ֵ��������ֵ��������
*		kangqian	05/22/2012	1.����OUTPUT_NVRAM��ǣ� ѡ���Ƿ�ѿ����̵������浽nvram��
*		kangqian	05/25/2012  1.���ӹ���plc�����汾�Ƿ���ݵ��жϣ����ӹ������ݿ�汾���ж� _CheckFileFlage
*		kangqian	07/30/2012	1._OUTPUT_NVRAM_���巴�ˣ�SYS_CPU_TYPE==CPU_COLDFIRE��Ӧ��Ϊfalse
								2.InitNVRamRelyWord��(g_pByNVRam[2*i]^g_pByNVRam[2*i+1]) == 0�Ǵ���ģ��෴���ֽ�������Ϊ255
								3.�źż�Ҫдeeprom,�����ݿ������б���ģ��������0x00,0x01, ����0x01,0x00,
									������nvram�б���ļ̵�������ֵ��ȡ��ֵ��ͬ	���ж���Ҫ��bBuf[2*i]^bBuf[2*i+1]==0x01							
		kangqian	07/312012	1.ResetLogicParseModule(BOOL bIsNewPLCDownLoad = FALSE),���Ӳ���bIsNewPLCDownLoad����������ϵͳ��ʼ��
								ʱװ���߼��������������������߼�������eeprom��nvram�б���ļ̵������������߼�Ҫ�������ֵ�����
		kangqian	12/31/2014  1.InitNVRamRelyWord ������byNKeepBufƫ�Ƽ��㲻�ԣ���g_pByNVRam + NM_RW_NUMӦ���� _pByNVRam + NM_RW_NUM*2
								  Ϊ�˷������壬����g_pByNKeepRamָ��nKeep�̵�����nvram������
*************************************************************************/
//#ifdef __PL_WIN_SIMULATION__
//#include "stdafx.h" 
//#endif

#include "plconfig.h"

#include "Logic.h"
#include "logicdef.h"
//#include "sysdef.h"
#include "relaydef.h" 
#include "sample.h"
#include "Aa1024.h"
#ifdef __PL_WIN_SIMULATION__
#include "platform.h"
#define	SW_STATUS_SET			1
#define	SW_STATUS_CUT			2
#define	CFG_STATUS_SET			1
#define	CFG_STATUS_CUT			2
#define	DB_BI_OPEN				0
#define	DB_BI_CLOSE				1
#define DB_EXECUTE				1
#define DB_CANCEL 				2
#else
#include "Dbasedef.h"
#include "Dbfunc.h"
#include "DaeFunc.h"
#include "Db_err.h"
#include "BO.h"
#include "relayfun.h"
//1300�п����̵�����Ҫ���浽nvram��6500������
//
#if(SYS_CPU_TYPE==CPU_COLDFIRE)   //1//6500����led.h�ļ�
#define _OUTPUT_NVRAM_  FALSE
#include "led.h"
extern  HANDLE g_hDbaseTaskID;
#else 
#define _OUTPUT_NVRAM_  TRUE //1300ʹ�ñ�����nram�е�output
extern  OSHANDLE g_hDbaseTaskID;
#endif //end of #if(SYS_CPU_TYPE==CPU_COLDFIRE)   //1//1300������led.h�ļ�

#endif //end of #ifdef __PL_WIN_SIMULATION__
#include "DefaultLogic.h"
//װ�ø�λ��־ 
#define SYS_RESET_FLAG		0x55aa33cc

static BYTE* g_pByNVRam = (BYTE*)LOGIC_NVRAM_BASE;
static BYTE* g_pByNKeepRam = (BYTE*)(LOGIC_NVRAM_BASE+ NM_RW_NUM*2); //2014/12/31
static BYTE* g_pByOutputNVRam = (BYTE*)(LOGIC_NVRAM_BASE+ (NM_RW_NUM+NKEEP_RW_NUM)*2);

extern TTimeRelayWordInf TimeRelayWordInf[];  // ʱ��̵����ֵĶ����������ݽṹ 

extern TTimeRelayWordInf ConstTimeRelayWordInf[]; //����ʱ��̵�����
extern 	WORD TIMERelayWordConst[][2]; //����ʱ��̵�������(�ڷ�����д����)

extern	DWORD TripRelayWordInf[]; //��բ�̵����Ķ���ṹ
extern TPulseRelayWordInf PulseRelayWordInf[]; //����̵����ṹ
extern TGERelayWordInf GERelayWordInf[]; //�Ƚϼ̵����ṹ
extern LONG GEParmList[]; //�Ƚϼ̵�������

//@extern LONG InputRelayWordBI[]; //����̵�����BI�Ĺ�����ϵ


extern BYTE byVInValue[]; //Vin���
extern BYTE byOutPreValue[]; //out�ϴε�״̬
extern BYTE byDebugRelayBuf[]; //���Լ̵���������

//2011-6-16 10:53:08 ���ƻ�������������·�̵����� ������·�̵���
extern BYTE byLedFlashBuf[];
extern BYTE byTxLinkBuf[];
extern BYTE byRxLinkBuf[];

//2012-02-13 ���Կ�����Կ����̵���������
extern BYTE byDebugInRWBuf[];
extern BYTE byDebugOutRwBuf[];

//2012-5-3 ������
extern LONG nCountRelayBuf[];

//@extern TTimeSetting	TimeSetting[]; //ʱ��̵�����ֵ
extern  BYTE byRelayWordValue[]; //�̵���ֵ
extern BYTE*  g_byPLCSource;      //�߼����̲�����e2�ĵ�ַ

extern volatile DWORD dwRelayMSCnt;		//�����ú��������

extern TSysConfigTable	*G_pCfgTable;

extern TRunTimeBI  *pSysBI;		//ң��ʵʱ����ָ��	��relay.cpp�ж���	

extern USER_FUN_RELAY_WORD_PTR s_pfnUserFunList[];
//extern kTLogicPara_Expression Logic_Expression[];
//extern kTLogicPara_Head Logic_Head;
//extern kTLogicPara_CalcNode Logic_Node[];
//extern TCommonFileHead CommonFile_Head;

#define  DITYPE_PULSE 5 //������
#define  DITYPE_VIRIN 7 //����ң��
#define  DITYPE_INSIDEIN 6 //�ڲ�ң��
#define  DITYPE_SBI 1 //����ң��

//��չ��������,���ɵ������ļ����
enum ParaExType
{
	Type_NONE = 128,
	Type_Const,
	Type_RM,
	Type_RS ,
	Type_AS,
	Type_Counter,
	Type_Report ,
	Type_Float,
	Type_SEQ ,
};

//plc�ļ���ǣ� 3001��ʾ��������İ汾��Ϊ3.0.0.1�� ����3�����汾�ţ���������ṹ�����仯
//plc�ļ����ܼ��ݣ����汾�Ż�仯
const BYTE g_byPLCFlag[8] = "PLC3001";


//������{
#ifdef __PL_WIN_SIMULATION__
MSG msg;
extern BOOL g_bIsEndThread;
#endif
//}


#define _WriteRelayWord(index,bContext)\
{\
	byRelayWordValue[index] <<= 4;\
	(bContext) ? (byRelayWordValue[index] |= 0x01) : (byRelayWordValue[index] &= 0xF0);\
}

/****************************************************/
/*		�ɱ���߼�����ģ���ڲ����ݽṹ				*/
/****************************************************/

typedef BOOL (*READ_RELAY_WORD_PTR)(WORD);		//ReadRelayWord���� 
typedef void (*WRITE_RELAY_WORD_PTR)(WORD, BOOL);//WriteRelayWord���� , �̵�����ţ� д��ֵ
//2009.10.15 ����������Ϊ2��, 2011-6-16����������Ϊ3����������������=���ұߵĽ��
//��ǰ���к��������ڵȺ��ұߵ���true��ʱ���ִ�У����ڰѵȺ��ұߵ�ֵ
//��Ϊһ��������������������ledflash =1��=0�������е������ͬ
typedef void (*RELAY_SYSFUN_PTR)(WORD, WORD);  //�߼�����ϵͳ����

//�߼�����״̬
LONG g_PLCStatus = PLC_STATUS_ERR; //syslogic.plc ״

//�����߼������� 
DWORD g_dwPLCDebugStep1=0, g_dwPLCDebugStep2=0; //����״̬��־
DWORD g_dwPLCCurLine=0, g_dwPLCBreakPoint=0; //g_dwPLCCurLine���е��У� g_dwPLCBreakPoint���Զϵ�
DWORD g_dwPLCDebugLoop=0; //ѭ��������g_dwPLCCurLine==g_dwPLCBreakPointʱ����
DWORD g_dwPLCDebugOneStep=0; //��������

LONG g_nPLCLineSys = 0;


BYTE* g_LogicParaBuf = NULL;		// �߼����̲��������� 
static struct _TCommonFileHead* s_pCommonFileHead = NULL;
static struct _TLogicPara_Head* s_pLogicParaHead = NULL;		// �߼����̲����ļ�ͷ�� 
static struct _TLogicPara_Expression* s_pExpression = NULL ;		// ���ʽ���� 
static struct _TLogicPara_CalcNode* s_pCalcNode = NULL;			// ִ�нڵ����� 
static WORD* s_pwChecksum = NULL;
//static BOOL bIsRepStarted = FALSE;  //�Ƿ��Ѿ�����������


// �̵�����ִ��״̬,����ʱ��̵�����
#define RELAY_STATE_A	0	//��״̬
#define RELAY_STATE_B	1	//��->�ϼ�ʱ״̬
#define RELAY_STATE_C	2	//��״̬
#define RELAY_STATE_D	3	//��->�ּ�ʱ״̬

//��ң��״̬�״�ˢ�³ɹ���־
BOOL isVirDIValide = FALSE;

void DebugRunLogic(DWORD* CurLine);


//------------�ɱ���߼�����ģ���ڲ��ӿں���----------------------------

/****************************************************************************************
 * 
 * ��������: д�̵�����(���ڳ���̵�����)�������ϴα�λ��Ϣ�������ز��� 
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵��������
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ�� TRUE or FAlSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_Normal(WORD wRelayWordIndex, BOOL bContext)
{
	_WriteRelayWord(wRelayWordIndex, bContext);
	
//@	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
//@	if (bContext)
//@	{
//@		byRelayWordValue[wRelayWordIndex] |= 0x01;
//@	}
//@	else
//@	{
//@		byRelayWordValue[wRelayWordIndex] &= 0xF0;
//@	}
}



/****************************************************************************************
 * 
 * ��������: д�̵�����(����ʱ��̵�����) 
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵�������ţ����Լ̵�����һ�����ţ� ����ʱ��̵����ڲ���ţ�
 *
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ �� TRUE �� FALSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Time(WORD wRelayWordIndex,  BOOL bContext)
{
	WORD wIndex = wRelayWordIndex - TIME_RELAY_WORD_START;
	WORD dwState = TimeRelayWordInf[wIndex].dwState;

	//2011-7-5 ÿ�θ�ֵ��Ҫ���Ƶģ� ����ԭ����ʱ����в����Ƶ�����һֱ���ڵĴ���
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
	
	if (bContext)
	{
		//��������ʱ��ֱ�Ӻ���
		if(*(TimeSetting[wIndex].pStartTime) == 0)  
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			return;
		}

		//��ǰ��λ  ��¼ʱ�䣬׼��Ϊ��->�ϼ�ʱ
		if (dwState == RELAY_STATE_A) 
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_B;
			TimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;//�����ú��������;
			byRelayWordValue[wRelayWordIndex] &= 0xF0;
			return;
		}

		//��ǰ��->�ϼ�ʱ  ʱ�䵽��ת��Ϊ��λ��  ʱ�䲻��������->�ϼ�ʱ
		else if(dwState == RELAY_STATE_B) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - TimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= *(TimeSetting[wIndex].pStartTime) )
			{
				/* ����ʱ�䵽 */
				TimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			else
			{
				byRelayWordValue[wRelayWordIndex] &= 0xF0; //���Ƿ�
			}
			return;
		}

		//�ϣ���-���ּ�ʱ  ת��Ϊ��λ
		else if (dwState == RELAY_STATE_C || dwState == RELAY_STATE_D) 
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;

			return;
		}
	}
	else
	{
		//�޸�����ʱ��ֱ�ӷ���
		if( *(TimeSetting[wIndex].pReturnTime) == 0)
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 

			return;

		}

		 //�֣���-���ϼ�ʱ  ת��Ϊ ��λ 
		if (dwState == RELAY_STATE_B || dwState == RELAY_STATE_A)
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 

			return;
		}

		 //��  ת��Ϊ��->�ּ�ʱ
		else if (dwState == RELAY_STATE_C)
		{
			TimeRelayWordInf[wIndex].dwState = RELAY_STATE_D;
			TimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			return;
		}

		//��->�ּ�ʱ	ʱ�䵽ת��Ϊ��λ�� ʱ�䲻��������ʱ
		else if(dwState == RELAY_STATE_D) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - TimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= *(TimeSetting[wIndex].pReturnTime) )
			{
				/* ����ʱ�䵽 */
				TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
				byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			}
			else
			{
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			return;


		}
	}
}


/****************************************************************************************
 * 
 * ��������: д����ʱ��̵�����
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵�������ţ����Լ̵�����һ�����ţ� ���ǳ���ʱ��̵����ڲ���ţ�
 *
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ �� TRUE �� FALSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_ConstTime(WORD wRelayWordIndex,  BOOL bContext)
{
	WORD wIndex = wRelayWordIndex - CONST_TIME_RW_START;
	WORD dwState = ConstTimeRelayWordInf[wIndex].dwState;

	//2011-7-5 ÿ�θ�ֵ��Ҫ���Ƶģ� ����ԭ����ʱ����в����Ƶ�����һֱ���ڵĴ���
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 

	if (bContext)
	{
		//��������ʱ��ֱ�Ӻ���
		if(TIMERelayWordConst[wIndex][0] == 0)  
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			return;

		}

		//��ǰ��λ  ת��Ϊ��->�ϼ�ʱ
		if (dwState == RELAY_STATE_A) 
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_B;
			ConstTimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;//�����ú��������;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			return;
		}

		//��ǰ��->�ϼ�ʱ
		else if(dwState == RELAY_STATE_B) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - ConstTimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= TIMERelayWordConst[wIndex][0] )
			{
				/* ����ʱ�䵽 */
				ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			else
			{
				
				byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			}
			return;
		}

		//�ϣ���-���ּ�ʱ
		else if (dwState == RELAY_STATE_C || dwState == RELAY_STATE_D) 
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_C;
			byRelayWordValue[wRelayWordIndex] |= 0x01;

			return;
		}
	}
	else
	{
		//�޸�����ʱ��ֱ�ӷ���
		if(TIMERelayWordConst[wIndex][1] == 0)
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			return;

		}

		 //�֣���-���ϼ�ʱ
		if (dwState == RELAY_STATE_B || dwState == RELAY_STATE_A)
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
			byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			return;
		}

		 //�� ת��Ϊ��->�ּ�ʱ
		else if (dwState == RELAY_STATE_C)
		{
			ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_D;
			//2012-2-13 ��ʼ��ʱ
			ConstTimeRelayWordInf[wIndex].dwTime = dwRelayMSCnt;
			
			byRelayWordValue[wRelayWordIndex] |= 0x01;

			return;
		}

		//��->�ּ�ʱ		
		else if(dwState == RELAY_STATE_D) 
		{
			DWORD dwTimeGap = dwRelayMSCnt - ConstTimeRelayWordInf[wIndex].dwTime;
			if ( dwTimeGap >= TIMERelayWordConst[wIndex][1] )
			{
				/* ����ʱ�䵽 */
				ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A;
				byRelayWordValue[wRelayWordIndex] &= 0xF0; 
			}
			else
			{
				
				byRelayWordValue[wRelayWordIndex] |= 0x01;
			}
			return;


		}
	}
}

/****************************************************************************************
 * 
 * ��������: д�Ա��̵ּ�����
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ TRURE �� FALSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_Keep(WORD wRelayWordIndex,  BOOL bContext)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];

	//ÿ�θ�ֵ��Ҫ���ƣ�����״̬��ô�仯
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 

	if ((byValue & 0x01) == 0x01)  //ԭ��Ϊ1�򲻹�bContext ����1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;  /* ����1״̬���� */
		return;	
	}
	else
	{	
		if(bContext)
			byRelayWordValue[wRelayWordIndex] |= 0x01;

	}

}

/****************************************************************************************
 * 
 * ��������: д��բ�̵���
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: 
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: 
 *
 * ���ش���: 
 * 
 * ����: д��բ�̵����� Ҳ��һ���Ա��̵ּ���
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Trip(WORD wRelayWordIndex,  BOOL bContext)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	WORD wIndex = wRelayWordIndex - TRIP_RELAY_WORD_START;


	//ÿ�θ�ֵ��Ҫ����
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 

	if ((byValue & 0x01) == 0x01)  //ԭ��Ϊ1����1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;  /* ����1״̬���� */
		return;	
	}
	else
	{	//ԭ����0����д1
		if(bContext)
		{
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			//��¼��0-��1��ʱ��
			TripRelayWordInf[wIndex] = dwRelayMSCnt; 
		}

	}
}
/****************************************************************************************
 * 
 * ��������: д����̵����̵���ֵ
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Pulse(WORD wRelayWordIndex,  BOOL bContext)
{
	if (wRelayWordIndex < PULSE_RELAY_WORD_START) return;
	WORD wIndex = wRelayWordIndex - PULSE_RELAY_WORD_START;
	if (wIndex>= PULSE_RELAY_WORD_NUM) return;
	
	//����
	if (bContext && !PulseRelayWordInf[wIndex].bIsRun)
	{
		PulseRelayWordInf[wIndex].bIsRun = TRUE;
		PulseRelayWordInf[wIndex].dwTime = dwRelayMSCnt;
	}

	//ֹͣ
	if (!bContext && PulseRelayWordInf[wIndex].bIsRun)
	{
		PulseRelayWordInf[wIndex].bIsRun = FALSE;
		PulseRelayWordInf[wIndex].dwTime = 0;
		byRelayWordValue[wRelayWordIndex] = 0;  //ֹͣ�Ժ�ֵҲ�б��0�����Ǳ��ֲ���
	}

	//��������
	if (bContext && PulseRelayWordInf[wIndex].bIsRun)
	{
		if ((dwRelayMSCnt - PulseRelayWordInf[wIndex].dwTime) > PulseRelayWordInf[wIndex].dwTimeParam )
		{
			//ȡ���� ���¼�ʱ
			byRelayWordValue[wRelayWordIndex] = !byRelayWordValue[wRelayWordIndex];
			PulseRelayWordInf[wIndex].dwTime =  dwRelayMSCnt;
		}
	}
}



/****************************************************************************************
 * 
 * ��������: д�Ƚϼ̵����̵���ֵ
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_GE(WORD wRelayWordIndex,  BOOL bContext)
{
	if (wRelayWordIndex < GE_RELAY_WORD_START) return;
	WORD wIndex = wRelayWordIndex - GE_RELAY_WORD_START;
	if (wIndex>= GE_RELAY_WORD_NUM) return;

	if(!bContext)
		return;
	if( *(GERelayWordInf[wIndex].npParm1) >= *(GERelayWordInf[wIndex].npParm2))
		{
			_WriteRelayWord(wRelayWordIndex, TRUE);
		}
	else
		{
			_WriteRelayWord(wRelayWordIndex, FALSE);
		}
}

/****************************************************************************************
 * 
 * ��������: д�źż̵����̵���ֵ, �źż̵���Ҳ���Ա��ֵģ���ǰֵ�����仯ʱҪ���浽e2
 * e2��ֻҪ���浱ǰֵ���У� �������ء��̵������ڴ��б�����
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_Signal(WORD wRelayWordIndex,  BOOL bContext)
{
	//��һ���Ա��̵ּ���
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
//	BYTE byNotValue = ~byValue;
	WORD wIndex = wRelayWordIndex - SIGNAL_RELAY_WORD_START;
	if (wIndex>= SIGNAL_RELAY_WORD_NUM) return;

	//ֵ�仯Ҫд��e2, e2�в����ϴ�ֵ�� ֻ���ı���ֵ��
/*	if((byValue & 0x01) != bContext)
	{
		//д��e2��, ������������
		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex, 1, (void*)&(byValue));
		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex+1, 1, (void*)&(byNotValue));
	}
*/	
	//ÿ�θ�ֵ��Ҫ����
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
	
	if ((byValue & 0x01) == 0x01)  //ԭ��Ϊ1����1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;	/* ����1״̬���� */
		return; 
	}
	else
	{	
		if(bContext) //ԭ��Ϊ0������Ҫд1
		{
//			BYTE byVal = 1;
//			BYTE byNotVal = ~byVal;
			byRelayWordValue[wRelayWordIndex] |= 0x01;
			//�������߼�����������дEEPROM��֪ͨ���ݿ��޸�
			OS_PostMessage(g_hDbaseTaskID, SM_WRITESINGNALRW, EEPROM_RELAY_SINGAL+2*wIndex,0x10,0,0);
			
//			EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex, 1, &(byVal));
//			EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex+1, 1, &(byNotVal));
			
		}
		
	}
}


/****************************************************************************************
 * 
 * ��������: д��ң�ż̵����̵���ֵ
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static void _WriteRelayWord_VIn(WORD wRelayWordIndex,  BOOL bContext)
{
//	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	WORD wIndex = wRelayWordIndex - VIN_RELAY_WORD_START;

	//д������
	_WriteRelayWord(wRelayWordIndex, bContext);

	//��Ҫд���ڴ���
	byVInValue[wIndex] =  bContext;
}


/****************************************************************************************
 * 
 * ��������: ������̵����̵���ֵ
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Normal(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x01) == 0x01) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * ��������: ��ȡ���̵���ֵ
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Not(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x01) == 0) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * ��������: �������ؼ̵���
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Up(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x01) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * ��������: ���½��ؼ̵���
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_Down(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x10) ? TRUE : FALSE;
}

/****************************************************************************************
 * 
 * ��������: �������ز�ȡ���̵���
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_NotUp(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x01) ? FALSE : TRUE;
}

/****************************************************************************************
 * 
 * ��������: ���½��ز�ȡ���̵���
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵��������
 *
 * ���ش���: �̵���ֵ TRURE �� FALSE
 * 
 * ����: 
 * 
 *****************************************************************************************/
static BOOL _ReadRelayWord_NotDown(WORD wRelayWordIndex)
{
	BYTE byValue = byRelayWordValue[wRelayWordIndex];
	return ((byValue & 0x11) == 0x10) ? FALSE : TRUE;
}
/****************************************************************************************
 * 
 * ��������: д�籣�濪���̵����֣������ϴα�λ��Ϣ�������ز�������ֵ�仯ʱҪ���浽nvram
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵��������
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ�� TRUE or FAlSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_NMOut(WORD wRelayWordIndex, BOOL bContext)
{
	//ֵ���ϴεĲ�ͬ����Ҫ���浽nvram��
	BYTE byOld = byRelayWordValue[wRelayWordIndex];
	_WriteRelayWord(wRelayWordIndex, bContext);
	
	if(byOld != byRelayWordValue[wRelayWordIndex])
	{
		//�б仯��д��nvram��
		WORD wOutIndex = wRelayWordIndex - OUTPUT_RELAY_WORD_START;
		if(wOutIndex >= OUTPUT_RELAY_WORD_NUM)
			return;
	
		g_pByOutputNVRam[2*wOutIndex]= byRelayWordValue[wRelayWordIndex];
		g_pByOutputNVRam[2*wOutIndex+1] = ~byRelayWordValue[wRelayWordIndex];	
	}
}

/****************************************************************************************
 * 
 * ��������: д�籣�̵ּ�����(���ڵ籣���м�̵�����)�������ϴα�λ��Ϣ�������ز�������ֵ�仯ʱҪ���浽nvram
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵��������
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ�� TRUE or FAlSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_NMNormal(WORD wRelayWordIndex, BOOL bContext)
{
	//ֵ���ϴεĲ�ͬ����Ҫ���浽nvram��
	BYTE byOld = byRelayWordValue[wRelayWordIndex];
	_WriteRelayWord(wRelayWordIndex, bContext);
	
	if(byOld != byRelayWordValue[wRelayWordIndex])
	{
		//�б仯��д��nvram��
		WORD wNHIndex = wRelayWordIndex - NM_RW_START;
		if(wNHIndex >= NM_RW_NUM)
			return;
	
		g_pByNVRam[2*wNHIndex]= byRelayWordValue[wRelayWordIndex];
		g_pByNVRam[2*wNHIndex+1] = ~byRelayWordValue[wRelayWordIndex];	
	}
}
/****************************************************************************************
 * 
 * ��������: д�籣�ּ��Ա��ֵ����֣���ֵ�仯ʱҪ���浽nvram����Ҫ���麯�����ܱ�0
 * 
 * ����˵��: 
 *			- WORD wRelayWordIndex
 *			  ����: 
 *			  ˵��: �̵��������
 *
 *			- BOOL bContext
 *			  ����: 
 *			  ˵��: д��ֵ�� TRUE or FAlSE
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/

static void _WriteRelayWord_NHKeep(WORD wRelayWordIndex,  BOOL bContext)
{
	ASSERT(wRelayWordIndex > NKEEP_RW_START);
	ASSERT((wRelayWordIndex -  NKEEP_RW_NUM) > NKEEP_RW_START);
	
	BYTE byValue = byRelayWordValue[wRelayWordIndex];

	//ÿ�θ�ֵ��Ҫ���ƣ�����״̬��ô�仯
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 

	if ((byValue & 0x01) == 0x01)  //ԭ��Ϊ1�򲻹�bContext ����1
	{
		byRelayWordValue[wRelayWordIndex] |= 0x01;  /* ����1״̬���� */
		return;	
	}
	else
	{	
		if(bContext)
			byRelayWordValue[wRelayWordIndex] |= 0x01;

	}
	if(byValue != byRelayWordValue[wRelayWordIndex] )
	{
		
		//�б仯��д��nvram��
		WORD wNHIndex = wRelayWordIndex - NKEEP_RW_START;
		if(wNHIndex >= NKEEP_RW_NUM)
			return;
		g_pByNVRam[2*wNHIndex+NM_RW_NUM]= byRelayWordValue[wRelayWordIndex];
		g_pByNVRam[2*wNHIndex+1+NM_RW_NUM] = ~byRelayWordValue[wRelayWordIndex];	
	}
}

// ���̵����ֺ����б� 
static READ_RELAY_WORD_PTR s_pfnReadList[] =
{
	_ReadRelayWord_Normal,
	_ReadRelayWord_Not,
	_ReadRelayWord_Up,
	_ReadRelayWord_Down,
	_ReadRelayWord_NotUp,
	_ReadRelayWord_NotDown,
};

// д�̵����ֺ����б� , ˳����ExpType����һ��
//@TypeID	TypeName	TypeLabel	Dec
//@0	����̵���	Normal	
//@1	�Ա��̵ּ���	Keep	
//@2	ʱ��̵���	Time	
//@3	��բ�̵���	TR	
//@4	���Լ̵���	Virtual	
//@5	����̵���	Pulse	
//@6	�źż̵���	Single	
//@7	���⿪��̵���	Vin	
//@8	�Ƚϼ̵���	GE	
//@9	����ʱ��̵���	CT

static WRITE_RELAY_WORD_PTR s_pfnWriteList[] =
{
	_WriteRelayWord_Normal,
	_WriteRelayWord_Keep,
	_WriteRelayWord_Time,
	_WriteRelayWord_Trip,
	NULL,
	_WriteRelayWord_Pulse, //�����߼�����д����
	_WriteRelayWord_Signal, //�źż̵���д����
	_WriteRelayWord_VIn, //���⿪��̵���д����
	_WriteRelayWord_GE, //�Ƚϼ̵���д����
	_WriteRelayWord_ConstTime, //����ʱ��̵���д����
	_WriteRelayWord_NMNormal, //�籣���м�̵���
	_WriteRelayWord_NHKeep,//�籣���Ա��̵ּ���
	_WriteRelayWord_NMOut,//�籣�濪��

};
//2012-5-25 ���plc���ļ���־�Լ� �汾���Ƿ�����ݿ�汾��һ��
BOOL _CheckFileFlage(_TCommonFileHead* pFileHead, _TLogicPara_Head* pHead)
{
	BYTE byVersion[8];
	
	BYTE* pFlag = pHead->byFileFlag;
	if((pFlag[0]!=g_byPLCFlag[0])||(pFlag[1]!=g_byPLCFlag[1])||(pFlag[2]!=g_byPLCFlag[2]))
	{
		LogError("_CheckFileFlage",FILE_LINE,"�ļ�����plc��ʽ��!");
		return FALSE;
	}
	if(pFlag[3]!=g_byPLCFlag[3])
	{
		LogError("_CheckFileFlage",FILE_LINE,"plc�ļ��ṹ�뵱ǰ���򲻼���!");
		return FALSE;
	}
	
	DWORD wdVersion;
	
	STATUS rc = DB_GetDataBaseVersion(byVersion);
	if(rc == DB_ERR_OK)
	{
		wdVersion = MAKEDWORD(byVersion[0], byVersion[1], byVersion[2], byVersion[3]);
		if(pFileHead->dwVersion != wdVersion)
		{
			
			LogError("_CheckFileFlage",FILE_LINE,"plc�ļ��汾��:%X�����ݿ�汾��:%X����ͬ",
				pFileHead->dwVersion, wdVersion);
			//return FALSE;
		}
	}
	return TRUE;
	
}


//��黺�������߼������Ƿ�Ϸ� dwFileLenΪ��������
BOOL _CheckPLCParam(DWORD dwFileLen)
{
	//�����ļ�ͷУ���� ������У���ļ�ͷ�� �ļ�ͷ��У��Ļ�s_pCommonFileHead->dwSize�ǲ����ŵ�
	DWORD i;
	DWORD dwParaOffset = (DWORD)g_LogicParaBuf;
	
	//��ͨ���ļ�ͷ
	s_pCommonFileHead = (_TCommonFileHead*)dwParaOffset;

	//�ж��ļ���С
	if(dwFileLen != s_pCommonFileHead->dwSize)
	{
		LogError("CheckPLCParam",FILE_LINE,"paraLen  error %d %d", dwFileLen, s_pCommonFileHead->dwSize);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	//�����ļ�ͷУ�����Ƿ���ȷ
	WORD wCheck = DAE_CheckSum(g_LogicParaBuf, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);

	if(dwCheck != s_pCommonFileHead->dwCheck)
	{
		LogError("_CheckPLCParam",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,s_pCommonFileHead->dwCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}

	//��������У����(�ļ�ͷ��Ĳ���)�Ƿ���ȷ
	wCheck = DAE_CheckSum(g_LogicParaBuf+sizeof(_TCommonFileHead), 
				s_pCommonFileHead->dwSize - 2-sizeof(_TCommonFileHead));
	WORD* pFileCheck = (WORD*)(g_LogicParaBuf+s_pCommonFileHead->dwSize -2);
	if(wCheck != *pFileCheck)
	{
		LogError("_CheckPLCParam",FILE_LINE,"File checksum error 0x%X  0x%X",
			wCheck,*pFileCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	dwParaOffset += sizeof(_TCommonFileHead);
	
	// ��ȡ�߼����̲����ļ�ͷ�� 
	s_pLogicParaHead = (_TLogicPara_Head*)dwParaOffset;
	if (s_pLogicParaHead->dwRelayWordNum != RELAY_WORD_NUM) 
	{
		LogError("ResetLogicPar",FILE_LINE,"relayWordNum err 0x%X 0x%X ",
			s_pLogicParaHead->dwRelayWordNum, RELAY_WORD_NUM);
		return FALSE;
	}
	if (s_pLogicParaHead->dwRelayWordNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwRelayWordNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwExpressionNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwExpressionNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwCalcNodeNum == 0)
	{
		LogError("ResetLogicPar",FILE_LINE,"dwCalcNodeNum = 0");
		return FALSE;
	}
	
	//2012-5-25 �ļ��ṹ�ж�
	if(!_CheckFileFlage(s_pCommonFileHead,s_pLogicParaHead))
		return FALSE;

	
	dwParaOffset += sizeof(_TLogicPara_Head);


	// ��ȡ���ʽ�����ṹ 
	s_pExpression = (_TLogicPara_Expression*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwExpressionNum; i++)
	{
		if (s_pExpression[i].wCalcNodeNum == 0) 
		{
				
				LogError("ResetLogicPar",FILE_LINE,"s_pExpression[i].wCalcNodeNum == 0");
				return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_Expression) * s_pLogicParaHead->dwExpressionNum;

	// ��ȡִ�нڵ������ṹ 
	s_pCalcNode = (_TLogicPara_CalcNode*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwCalcNodeNum; i++)
	{
		if (s_pCalcNode[i].wRelayIndex >= RELAY_WORD_NUM)
		{
			
			LogError("ResetLogicPar",FILE_LINE,"s_pCalcNode.wRelayIndex >= RELAY_WORD_NUM");
			return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_CalcNode) * s_pLogicParaHead->dwCalcNodeNum;

	
	//��ȡУ����
	//2012-6-8 �Ӱ���dwParaOffset�ҵ�У���Ϊ��s_pCommonFileHead->dwSize�ҵ���pFileCheck
	//��Ϊplc���ֻ��������߼������޹ص�������Ϣ(����ʱ���ã������û��)
	s_pwChecksum = (WORD*)pFileCheck;	

	return TRUE;
}
/****************************************************/
/*		�����ǿɱ���߼�����ģ���ⲿ�ӿں���				*/
/****************************************************/
// ��ʼ���ɱ���߼�ģ�� ,ֻ����һ�Σ� �����߼����̴洢�ռ���
BOOL InitLogicParseModule(void)
{
	//����CPU������Զ�ʶ���ܣ���Ҫͨ������ʵ�ֶ�ָ��NVRAM�ռ�ָ��ĳ�ʼ����������ͨ��
	//�����ʼ����ʽ��ɣ�������ָ���ʼ������
	g_pByNVRam = (BYTE*)LOGIC_NVRAM_BASE;
	g_pByNKeepRam = (BYTE*)(LOGIC_NVRAM_BASE+ NM_RW_NUM*2);
	g_pByOutputNVRam = (BYTE*)(LOGIC_NVRAM_BASE+ (NM_RW_NUM+NKEEP_RW_NUM)*2);

	// Ϊϵͳ�߼�����ģ������ڴ� 
	g_LogicParaBuf = (BYTE*)OS_MemAlloc(__PL_LOGIC_SIZE__);
	

	if((g_LogicParaBuf == NULL))
	{
		LogError("InitLogicParseModule",FILE_LINE,"Mem allocate fail");
		return FALSE;
	}
	
	return TRUE;
}


/************************************************************
	���ܣ�//����Ϊ����ʱ��ĳ�ʼ��
	������
	     WORD wIndex[in] �Ƚϼ̵������
	     WORD wPara[in] ����ֵ 
	     WORD wParaOther[in] ��Ϊ�Ƚϵ���һ������
	     WORD wPara[in] ��Ϊ�Ƚϵ���һ����������չ����ex
	���أ�LONG  0:�ɹ� 1:ʧ��
************************************************************/

BOOL InitConstGePara(WORD wIndex,WORD wPara, WORD wParaOther, WORD wExOther)
{
	
	LONG lCF, LVal, rc, lSetVal, lSysSetVal;
	WORD wRT, wCFIndex, wSetAreaNo,wDec;
	TRM_Disp RM_Disp;
	TCF_Disp CF_Disp;
	TSET_Disp tSET_Disp;
	float fVal;

	//��һ������������
	////2��������ֵ 3��ֵ 4 ������ֵ 5������
	switch(wExOther)
	{
		case Type_RM:
			rc = DB_GetRMDisp(wParaOther, &RM_Disp);
			if(rc == DB_ERR_OK)
			{
				wCFIndex = RM_Disp.nCFIndex;
				DB_GetCFDisp(wCFIndex, &CF_Disp);
				lCF = DB_GetCFValue(wCFIndex, &lCF);
				wRT = CF_Disp.wCof;
			}
			else
			{
				g_PLCStatus = PLC_STATUS_ERR;
				LogError("Logic",FILE_LINE,"InitGERelayWord DB_GetRMDisp");
				return FALSE;
			}

			//
			
			if(lCF == 0) 
				lCF = 1;
			if(wRT == 0) 
				wRT = 1;
			
			//	��ϵ��/����  �����߼����̹�������зŴ���10����
			LVal = (long)(wPara*lCF)/wRT/10;
			GEParmList[wIndex] = LVal;
			break;
		case Type_RS:
			//�ѳ���ת��Ϊ����ֵ
			fVal = (float)wPara/10;
			rc = ConvertSETFloat2Fix(wParaOther, fVal, &LVal);
			
			if(rc != DB_ERR_OK)
			{
				g_PLCStatus = PLC_STATUS_ERR;
				LogError("Logic",FILE_LINE,"InitGERelayWord DB_GetRMDisp");
				return FALSE;
			}
			
			GEParmList[wIndex] = LVal;
			break;
		case Type_AS:
			
			fVal = (float)wPara/10;
			rc = ConvertSystemSETFloat2Fix(wParaOther, fVal, &LVal);
			
			if(rc != DB_ERR_OK)
			{
				g_PLCStatus = PLC_STATUS_ERR;
				LogError("Logic",FILE_LINE,"InitGERelayWord DB_GetRMDisp");
				return FALSE;
			}
			GEParmList[wIndex] = LVal;
			break;
		case RW_TYPE_COUNTER:
			
			GEParmList[wIndex] = wPara/10;
			break;
			
		}
	return TRUE;

}

//��ʼ���Ƚϼ̵������� 1 ���� 2��������ֵ 3��ֵ 4 ������ֵ 5������
//��ʼ�����pExpression ��GE�̵��� �Ĳ���֮һwWhichPara  1:wPara; 2:wPara2
BOOL InitOneGEPara(_TLogicPara_Expression* pExpression, WORD wWhichPara)
{
	
	LONG** pnPara;
	TRM_Disp RM_Disp;
	TCF_Disp CF_Disp;
	LONG lCF, LVal, rc, lSetVal, lSysSetVal;
	WORD wRT, wCFIndex, wSetAreaNo;
	WORD wIndex, wPara, wParaEx, wParaOther, wExOther,wCounterIndex;

	
	wIndex = pExpression->wResultRelayWordIndex - GE_RELAY_WORD_START;

	//�ǲ���1���ǲ���2
	if(wWhichPara == 1)
	{
		
		pnPara = &(GERelayWordInf[wIndex].npParm1);
		
		wPara = pExpression->wParm;
		wParaEx = pExpression->wParaEx;
		wParaOther= pExpression->wParm2;
		wExOther= pExpression->wParaEx2;
	}
	else
	{
		pnPara = &(GERelayWordInf[wIndex].npParm2);
		
		wPara = pExpression->wParm2;
		wParaEx = pExpression->wParaEx2;
		wParaOther= pExpression->wParm;
		wExOther= pExpression->wParaEx;
	}
	
	switch(wParaEx)
	{
		//������ӣ� ��Ҫ�жϱȽϵĶ������˳����ʵı����Ա��ڱȽ�
		case Type_Float:
			{
			if(!InitConstGePara(wIndex,wPara,wParaOther,wExOther))
				return FALSE;
			*pnPara = &(GEParmList[wIndex]);
			}
			break;
		case Type_RM:
			*pnPara= (LONG*)(RM_BUF_ADDR + 4*wPara);
			break;
		case Type_RS:
			*pnPara = (LONG*)(RS_BUF_ADDR + 4*wPara);
			break;
		case Type_AS:
			*pnPara = (LONG*)(AS_BUF_ADDR + 4*wPara);
			break;
		case RW_TYPE_COUNTER:
			wCounterIndex = wPara - CNT_RELAY_WORD_START;
			if(wCounterIndex >= CNT_RELAY_WORD_NUM)
				return FALSE;
			*pnPara = &(nCountRelayBuf[wCounterIndex]);
			break;
	}
	return TRUE;
}

BOOL InitGERelayWord()
{
	int nLineNum;
	nLineNum = s_pLogicParaHead->dwExpressionNum;
	_TLogicPara_Expression* pExpression = s_pExpression;		// ���ʽ���� 
	
	for(int i=0; i<nLineNum; i++)
	{
		if(pExpression[i].wResultRelayWordType != GE)
			continue;

		if(!InitOneGEPara(&(pExpression[i]), 1))
			return FALSE;

		
		if(!InitOneGEPara(&(pExpression[i]), 2))
			return FALSE;
		
	}
	return TRUE;
}





//��ʼ�����浽nvram�ڵļ̵���(NM��NKeep) �Լ�1300�Ŀ����̵���
//���
BOOL InitNVRamRelyWord(BOOL bIsNewPLCDownLoad = FALSE)
{
	int i;
	//����������ص��߼�����������ֵ
	if(bIsNewPLCDownLoad)
	{
		
		LogError("Logic", FILE_LINE, "new logic Reset NVRamRelayword");
		for(i = 0; i<NM_RW_NUM; i++)
		{
			g_pByNVRam[2*i] = 0;
			g_pByNVRam[2*i+1] = 0xFF;
		}
		
		for(i = 0; i<NKEEP_RW_NUM; i++)
		{
			g_pByNKeepRam[2*i] = 0;
			g_pByNKeepRam[2*i+1] = 0xFF;
		}

		if(!_OUTPUT_NVRAM_)
			return TRUE;
		for(i = 0; i<OUTPUT_RELAY_WORD_NUM; i++)
		{
			g_pByOutputNVRam[2*i] = 0;
			g_pByOutputNVRam[2*i+1] = 0xFF;
		}
		
		
		return TRUE;
	}

	//�Ƿ�λ���������Ƿ��nvram��ȡֵ
	DWORD *pFlag = (DWORD*)SYS_START_FLAG_ADD;	//װ�ø�λ�ڲ�RAM��־

	//�������װ�ø�λ������Ҫ����
	if(*pFlag != SYS_RESET_FLAG)	
		return TRUE;

	for(i = 0; i<NM_RW_NUM; i++)
	{
		//У����ȷ
		if(g_pByNVRam[2*i]^g_pByNVRam[2*i+1] == 255 )
			byRelayWordValue[NM_RW_START+i] = g_pByNVRam[2*i]&0x01;
		else
		{
			byRelayWordValue[NM_RW_START+i] = 0;
			BYTE byNotVal = ~byRelayWordValue[NM_RW_START+i];
			g_pByNVRam[2*i] = byRelayWordValue[NM_RW_START+i];
			g_pByNVRam[2*i+1] = byNotVal;
			
			LogError("Logic", FILE_LINE, "NM Relayword value %d in NVRam reset", i);
		}
	}


	for(i = 0; i<NKEEP_RW_NUM; i++)
	{
		//У����ȷ
		if(g_pByNKeepRam[2*i]^g_pByNKeepRam[2*i+1] == 255)
	    	byRelayWordValue[NKEEP_RW_START+i] = g_pByNKeepRam[2*i]&0x01;
		else
		{
			byRelayWordValue[NKEEP_RW_START+i] = 0;
			BYTE byNotVal = ~byRelayWordValue[NKEEP_RW_START+i];
			g_pByNKeepRam[2*i] = byRelayWordValue[NKEEP_RW_START+i];
			g_pByNKeepRam[2*i+1] = byNotVal;
			
			LogError("Logic", FILE_LINE, "NKEEP Relayword value %d in NVRam reset", i);
		}
	}

	//2012-5-23 �����Ƿ񱣴���nvram�е�
	if(!_OUTPUT_NVRAM_)
		return TRUE;

	for(i = 0; i<OUTPUT_RELAY_WORD_NUM; i++)
	{
		//У����ȷ
		if(g_pByOutputNVRam[2*i]^g_pByOutputNVRam[2*i+1] == 255)
			byRelayWordValue[OUTPUT_RELAY_WORD_START+i] = g_pByOutputNVRam[2*i]&0x01;
		else
		{
			byRelayWordValue[OUTPUT_RELAY_WORD_START+i] = 0;
			BYTE byNotVal = ~byRelayWordValue[OUTPUT_RELAY_WORD_START+i];
			g_pByOutputNVRam[2*i] = byRelayWordValue[OUTPUT_RELAY_WORD_START+i];
			g_pByOutputNVRam[2*i+1] = byNotVal;
			
			LogError("Logic", FILE_LINE, "NMout Relayword value %d in NMoutVRam reset", i);
		}
	}

	return TRUE;

}
//�źż�Ҫдeeprom,�����ݿ������б���ģ��������0x00,0x01, ����0x01,0x00,
//������nvram�б���ļ̵�������ֵ��ȡ��ֵ��ͬ
BOOL InitSignalRelayWord(BOOL bIsNewPLCDownLoad = FALSE)
{
	BYTE bBuf[SIGNAL_RELAY_WORD_NUM*2+1];
	//��������������߼����ʼ���źż̵�������Ҫ���eeprom�����ֵ
	if(bIsNewPLCDownLoad)
	{
		for(int j=0;j<SIGNAL_RELAY_WORD_NUM; j++)
		{
			bBuf[2*j] = 0x00;
			bBuf[2*j+1]=0x01;
		}
		EEPROM_Write(EEPROM_RELAY_SINGAL, SIGNAL_RELAY_WORD_NUM*2, bBuf);
		
		LogError("Logic", FILE_LINE, "new logic reset SignalRelayWord");
		return TRUE;
	}

	//�����������߼�����Ҫ��eeprom��ȡֵ���̵���
	EEPROM_Read(EEPROM_RELAY_SINGAL, SIGNAL_RELAY_WORD_NUM*2, bBuf);
	
	for(int i = 0; i<SIGNAL_RELAY_WORD_NUM; i++)
	{
		//У����ȷ
		if(bBuf[2*i]^bBuf[2*i+1] == 0x01)
			byRelayWordValue[SIGNAL_RELAY_WORD_START+i] = bBuf[2*i]&0x01;
		else
		{
			byRelayWordValue[SIGNAL_RELAY_WORD_START+i] = 0x00;
			BYTE byNotValLastBit = 0x01;
			
			EEPROM_Write(EEPROM_RELAY_SINGAL+2*i, 1, &byRelayWordValue[SIGNAL_RELAY_WORD_START+i]);
			
			EEPROM_Write(EEPROM_RELAY_SINGAL+2*i+1, 1, &byNotValLastBit);
			LogError("Logic", FILE_LINE, "signal value %d in eeprom reset", i);
		}
	}
	return TRUE;
	
}

//��ʼ������̵������� �޸Ŀ������Ժ��Ⱥ�ң�Ÿ���������ˣ����Զ�Ӧ��ϵ������
/*
BOOL InitInputRelayPara()
{
	LONG i, j,rc ;
	j=0;
	TDIPara DIPara;
	for(i=0; i<INPUT_RELAY_WORD_NUM; i++)
	{
		rc = DB_GetDIPara(i, &DIPara);
		if(rc != DB_ERR_OK)
		{
			LogError("InitInputRelayPara",FILE_LINE,"DB_GetDIPara error");
			g_PLCStatus = PLC_STATUS_ERR;
			return FALSE;
		}
		if((DIPara.wClass == DITYPE_VIRIN )|| (DIPara.wClass == DITYPE_INSIDEIN) 
			|| (DIPara.wClass == DITYPE_SBI))
		{
			InputRelayWordBI[i] = j++;
		}
		else
		{
			InputRelayWordBI[i] = -1;
			j++;
		}
	}
	return TRUE;
}*/

//��ʼ���̶�ʱ��̵���
BOOL InitConstTimeRelayWord()
{
	int nLineNum;
	nLineNum = s_pLogicParaHead->dwExpressionNum;
	_TLogicPara_Expression* pExpression = s_pExpression;		// ���ʽ���� 
	
	WORD wParm1, wParm2, wIndex;



	memset((void*)TIMERelayWordConst, 0, sizeof(WORD)*(CONST_TIME_RW_NUM+1)*2);
	
	
	for(int i=0; i<nLineNum; i++)
	{


		if(pExpression[i].wResultRelayWordType != CONSTTIME)
			continue;
		wParm1 = pExpression[i].wParm;
		wParm2 = pExpression[i].wParm2;
		wIndex = pExpression[i].wResultRelayWordIndex - CONST_TIME_RW_START;

		if(wParm1 != 0) 
		{
			TIMERelayWordConst[wIndex][0] = wParm1;
//			TimeSetting[wIndex].pStartTime = &(TIMERelayWordConst[wIndex][0]);
		}

		if(wParm2 !=0)
		{
			TIMERelayWordConst[wIndex][1] = wParm2;
//			TimeSetting[wIndex].pReturnTime = &(TIMERelayWordConst[wIndex][1]);
		}

		
	}
	return TRUE;

	
}
//���ļ�װ�ط��̵�������߼����̲���������
DWORD _LoadFileLogicPar(char* strFileName)
{
	DWORD dwFileLen, dwLogicLen;

	STATUS rc = DAE_GetFileLength(strFileName, &dwFileLen);
	if((rc != DB_ERR_OK) || (dwFileLen <=sizeof(TFileHead)) )
	{
		LogError("ResetLogicParseModule",FILE_LINE,"DAE_ReadFile err");
		return 0;
	}
	
	//�ȶ�ȡ�ļ�ͷ�� ��װ�ļ��������ļ���С���߼�����
	TFileHead tFileHead;
	rc = DAE_ReadFile(strFileName, (BYTE*)(&tFileHead),sizeof(TFileHead));
	if((rc != DB_ERR_OK))
	{
		LogError("ResetLogicParseModule",FILE_LINE,"DAE_ReadFile err");
		return 0;
	}
	
	//У���ļ�ͷ�� ��ȡ�ļ�����
	WORD wCheck = DAE_CheckSum((BYTE*)&tFileHead, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);
			
	if(dwCheck != tFileHead.dwCheck)
	{
		LogError("_CheckPLCParam",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,tFileHead.dwCheck);
		return 0;
	}
	dwLogicLen = tFileHead.dwSize;

	
	if((dwLogicLen>__PL_LOGIC_SIZE__)||(dwLogicLen<0))
	{
		LogError("ResetLogicParseModule",FILE_LINE,"The logic size is > __PL_LOGIC_SIZE__!");
		return 0;
	}
	else
	{
		rc = DAE_ReadFile(strFileName, g_LogicParaBuf,dwLogicLen);
		if(DB_ERR_OK == rc)
		{
			return dwLogicLen;
		}
		else
		{
			return 0;
		}
	}
}

//������װ�ط��̵�������߼����̲����������� ���ز�������(������ͬ���Լ�)
DWORD _LoadDefaultLogicPar()
{

	MakeAlarmRpt(R_LOGICERR);
	DWORD dwParaOffset = (DWORD)g_LogicParaBuf;
	
	s_pCommonFileHead = (_TCommonFileHead*)dwParaOffset;
	memcpy(s_pCommonFileHead, (void*)&CommonFile_Head, sizeof(CommonFile_Head));

	s_pLogicParaHead = (kTLogicPara_Head*)(s_pCommonFileHead+1);
	memcpy(s_pLogicParaHead, (void*)&Logic_Head, sizeof(Logic_Head));

	s_pExpression = (kTLogicPara_Expression*)(s_pLogicParaHead+1);
	memcpy(s_pExpression, Logic_Expression, sizeof(Logic_Expression));

	s_pCalcNode =(kTLogicPara_CalcNode*)((BYTE*)s_pExpression + sizeof(Logic_Expression));
	memcpy(s_pCalcNode, Logic_Node, sizeof(Logic_Node));

	//У����
	s_pwChecksum = (WORD*)((BYTE*)s_pCalcNode + sizeof(Logic_Node));
	*s_pwChecksum =  wChksumCpp;
	return (DWORD(s_pwChecksum) - dwParaOffset +2); //�����ܳ���
}


//2012-5-25 ��������ʹ����
/*

//���߼����̲��������������߼�����
BOOL _ResetLogicPar(DWORD dwFileLen)
{
	DWORD i;
	DWORD dwParaOffset = (DWORD)g_LogicParaBuf;
	
	//��ͨ���ļ�ͷ
	s_pCommonFileHead = (_TCommonFileHead*)dwParaOffset;

	//�ж��ļ���С
	if(dwFileLen != s_pCommonFileHead->dwSize)
	{
		LogError("ResetLogicPar",FILE_LINE,"FileLen  error %d %d", dwFileLen, s_pCommonFileHead->dwSize);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	dwParaOffset += sizeof(_TCommonFileHead);
	//�����ļ�ͷУ�����Ƿ���ȷ
	WORD wCheck = DAE_CheckSum(g_LogicParaBuf, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);

	if(dwCheck != s_pCommonFileHead->dwCheck)
	{
		LogError("ResetLogicPar",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,s_pCommonFileHead->dwCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}

	//��������У�����Ƿ���ȷ
	wCheck = DAE_CheckSum(g_LogicParaBuf, s_pCommonFileHead->dwSize - 2);
	WORD* pFileCheck = (WORD*)(g_LogicParaBuf+s_pCommonFileHead->dwSize -2);
	if(wCheck != *pFileCheck)
	{
		LogError("ResetLogicPar",FILE_LINE,"File checksum error 0x%X  0x%X",
			wCheck,*pFileCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return FALSE;
	}
	
	// ��ȡ�߼����̲����ļ�ͷ�� 
	s_pLogicParaHead = (_TLogicPara_Head*)dwParaOffset;

	//2012-5-25 �ļ��ṹ�ж�
	if(!_CheckFileFlage(s_pCommonFileHead,s_pLogicParaHead))
		return FALSE;

	
	if (s_pLogicParaHead->dwRelayWordNum != RELAY_WORD_NUM) 
	{
		LogError("ResetLogicPar",FILE_LINE,"relayWordNum err 0x%X 0x%X ",
			s_pLogicParaHead->dwRelayWordNum, RELAY_WORD_NUM);
		return FALSE;
	}
	if (s_pLogicParaHead->dwRelayWordNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwRelayWordNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwExpressionNum == 0) 
	{
		LogError("ResetLogicPar",FILE_LINE,"dwExpressionNum = 0");
		return FALSE;
	}
	if (s_pLogicParaHead->dwCalcNodeNum == 0)
	{
		LogError("ResetLogicPar",FILE_LINE,"dwCalcNodeNum = 0");
		return FALSE;
	}
	dwParaOffset += sizeof(_TLogicPara_Head);


	// ��ȡ���ʽ�����ṹ 
	s_pExpression = (_TLogicPara_Expression*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwExpressionNum; i++)
	{
		if (s_pExpression[i].wCalcNodeNum == 0) 
		{
				
				LogError("ResetLogicPar",FILE_LINE,"s_pExpression[i].wCalcNodeNum == 0");
				return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_Expression) * s_pLogicParaHead->dwExpressionNum;

	// ��ȡִ�нڵ������ṹ 
	s_pCalcNode = (_TLogicPara_CalcNode*)dwParaOffset;
	for (i = 0; i < s_pLogicParaHead->dwCalcNodeNum; i++)
	{
		if (s_pCalcNode[i].wRelayIndex >= RELAY_WORD_NUM)
		{
			
			LogError("ResetLogicPar",FILE_LINE,"s_pCalcNode.wRelayIndex >= RELAY_WORD_NUM");
			return FALSE;
		}
	}
	dwParaOffset += sizeof(_TLogicPara_CalcNode) * s_pLogicParaHead->dwCalcNodeNum;

	
	//��ȡУ����
	s_pwChecksum = (WORD*)dwParaOffset;	
	
	//�����ṹ��û��������ߵ�����
	g_PLCStatus = PLC_SYS_OK;
	return TRUE;
}*/

// ��λ�ɱ���߼�ģ��,���߼����̲������غ����
//�̵����ָ�λ���뷽�̲����޹ص�ֱ�Ӹ�λ�� �뷽�̲����йصĶ�������ȷ��λ
BOOL ResetLogicParseModule(BOOL bIsNewPLCDownLoad)
{
	//2011-12-20 ��λ���ǰ������PLC_STATUS_ERR�� ��ֹLogicProcess���߼���ʼ��
	//δ���ǰ�������߼����� 
	g_PLCStatus = PLC_STATUS_ERR;
	
	g_dwPLCDebugStep1 = 0;
	g_dwPLCDebugStep2 = 0;
	
	s_pCommonFileHead = NULL;
	
	s_pLogicParaHead = NULL;

	s_pExpression = NULL;

	s_pCalcNode = NULL;

	// ��λ���м̵����� 
	ResetAllRelayWord();

	// ��λʱ��̵����������ṹ 
	memset((void*)TimeRelayWordInf, 0, sizeof(struct TTimeRelayWordInf) * TIME_RELAY_WORD_NUM); 
	memset((void*)ConstTimeRelayWordInf, 0, sizeof(struct TTimeRelayWordInf) * CONST_TIME_RW_NUM);
	memset((void*)TripRelayWordInf, 0, sizeof(DWORD)* TRIP_RELAY_WORD_NUM);
	memset((void*)PulseRelayWordInf, 0, sizeof(struct TPulseRelayWordInf) * PULSE_RELAY_WORD_NUM);
	memset((void*)byVInValue, 0, VIN_RELAY_WORD_NUM);
	
	memset((void*)byOutPreValue, 0, OUTPUT_RELAY_WORD_NUM);
	memset((void*)nCountRelayBuf, 0, CNT_RELAY_WORD_NUM*2);
	
	//��ʼ������̵������� 2012-5-23 ң�źͿ����Ӧ��ϵ���������ñ���ˣ�����Ҫ�ú�����
//@	if(!InitInputRelayPara())
//@		return FALSE;
	

//@	//��eprom�ж�ȡ�߼����̵�ram��

	//���ԣ� �ӻ�������ȡ
	#ifdef __PL_WIN_SIMULATION__
	memcpy(g_LogicParaBuf, g_byPLCSource ,__PL_LOGIC_SIZE__);
	#else
	//������ļ�����Ļ��Ƕ��ļ����ݵ�ram��
	DWORD dwLogicSize;
	
	char strFileName[256];
	sprintf(strFileName, "%s", "Logic.PLC");
	
	//�߼������ļ�
	
	//2011-12-20 nPLCStatus�ݴ�g_PLCStatus״̬
	LONG nPLCStatus;
	
	dwLogicSize = _LoadFileLogicPar(strFileName);
	if(dwLogicSize>0)
	{
		if(!_CheckPLCParam(dwLogicSize))
		{
			nPLCStatus = PLC_STATUS_ERR;
		}
		else
			nPLCStatus = PLC_SYS_OK;
	}
	else
	{
		LogError("ResetLogicParseModule",FILE_LINE,"The Logic Date File fileName=%s Read Err!",strFileName);
		nPLCStatus = PLC_STATUS_ERR;
	}

	//�߼������ļ�����ʹ��Ĭ��ϵͳ�߼�
	if(PLC_STATUS_ERR ==  nPLCStatus)
	{
		dwLogicSize = _LoadDefaultLogicPar();

		if(_CheckPLCParam(dwLogicSize))
		{
			nPLCStatus = PLC_DEFAULT_OK;
			LogError("ResetLogicParseModule",FILE_LINE,"Default logic Ok");
		}
		else
		{
			g_PLCStatus = PLC_STATUS_ERR;
			LogError("ResetLogicParseModule",FILE_LINE,"Default logic ERR");
			return FALSE;
		}
		
	}
	#endif

	//��e2��ȡ�źż̵������ڴ���
	InitSignalRelayWord(bIsNewPLCDownLoad);

	//��nvram��nvram�̵��� 2011-9-15
	InitNVRamRelyWord(bIsNewPLCDownLoad);

	//���������ͷ����йأ� ���뷽����ȷ�˲��ܳ�ʼ��
	//��ʼ���Ƚϼ̵�������
	if(!InitGERelayWord())
	{
		return FALSE;
	}

	//��ʼ������ʱ��̵���
	InitConstTimeRelayWord();

	UpDateInPut();
	UpDateCFG();  //kang �޸�Ϊ�ڱ��������н��յ����õ��޸���Ϣ�Ÿ������ú�ѹ��
	UpDateSW(); 	//   ��OnRelayChange�е���

//kang 2012-5-29 ǿ��ˢ��һ�ο�����ʹ������̵���״̬�����
	for(int i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
		byOutPreValue[i] = ~byRelayWordValue[i+OUTPUT_RELAY_WORD_START];
	UpDateOutPut();
		
	
	//2011-12-20 ��ʼ�������
	g_PLCStatus = nPLCStatus; 
	return TRUE;
}

// ��λ(��)���еļ̵����� 
void ResetAllRelayWord(void)
{
	// ��̵����ֱ��� 
	memset((void*)byRelayWordValue, 0, RELAY_WORD_NUM);

	// ע��:��Ҫ��TRUE�̵�����������Ҫ����Ϊ1
	byRelayWordValue[1] = 1;
}

// ���̵�����
/*BOOL ReadRelayWord(WORD wRelayWordIndex)
{
	if (wRelayWordIndex >= RELAY_WORD_NUM) return FALSE;
	if ((byRelayWordValue[wRelayWordIndex] & 0x01) == 0x01)
		return TRUE;
	return FALSE;
}*/


// д�̵����ֶ���ӿ� 
void WriteRelayWord(WORD wRelayWordIndex, BOOL bContext)
{
	if((wRelayWordIndex >= RELAY_WORD_NUM)|| (wRelayWordIndex < 0))
		return;
	if(wRelayWordIndex<2)
		return;
	OS_Protect(0);
	WORD wExcType = RelayWordTypeList[wRelayWordIndex-2];
	s_pfnWriteList[wExcType](wRelayWordIndex, bContext);
	OS_UnProtect(0);
}

//���źż̵���
void ResetSignalRelayWord(WORD wRelayWordIndex, WORD wRsv)
{
	if (wRelayWordIndex < SIGNAL_RELAY_WORD_START) 
	{
		LogError("ResetSignalRelayWord",FILE_LINE,"wRelayWordIndex < SIGNAL_RELAY_WORD_START");
		return;
	}
	
	if (wRelayWordIndex >= SIGNAL_RELAY_WORD_START + SIGNAL_RELAY_WORD_NUM)
	{
		LogError("ResetSignalRelayWord",FILE_LINE,"wRelayWordIndex Խ����");
		return;
	}

	WORD wIndex = wRelayWordIndex - SIGNAL_RELAY_WORD_START;
	if( (byRelayWordValue[wRelayWordIndex] & 0x01) == 0x1)
	{
//		BYTE byVal = 0;
//		BYTE byNotVal = ~byVal;
		//���ֵ�e2
//		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex, 1, &byVal);
//		EEPROM_Write(EEPROM_RELAY_SINGAL+2*wIndex+1, 1, &byNotVal);
		
		//�������߼�����������дEEPROM��֪ͨ���ݿ��޸�
		OS_PostMessage(g_hDbaseTaskID, SM_WRITESINGNALRW, EEPROM_RELAY_SINGAL+2*wIndex,0x01,0,0);
	}
	
	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
	byRelayWordValue[wRelayWordIndex] <<= 4;	
	// ��̵����� 
	byRelayWordValue[wRelayWordIndex] &= 0xF0;


}

//����բ�̵���
void ResetTripRelayWord(WORD wRelayWordIndex , WORD wRsv)
{
	if (wRelayWordIndex < TRIP_RELAY_WORD_START) return;
	if (wRelayWordIndex >= TRIP_RELAY_WORD_START + TRIP_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - TRIP_RELAY_WORD_START;

	DWORD dwTimeGap = dwRelayMSCnt - TripRelayWordInf[wIndex];
	
	if ( dwTimeGap >= *(TripSetting[wIndex]) ) //����ʱ�䵽
	{
		// ��̵����� 
		byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
		
		byRelayWordValue[wRelayWordIndex] &= 0xF0;

	}
	return;
}

//��ʱ��̵�����(ʱ��̵����ͳ���ʱ��̵��������������)
void ResetTimeRelayWord(WORD wRelayWordIndex , WORD wRsv)
{
	
	WORD wIndex;
	if((wRelayWordIndex >=TIME_RELAY_WORD_START)&&(wRelayWordIndex < TIME_RELAY_WORD_START + TIME_RELAY_WORD_NUM))
	{
		wIndex = wRelayWordIndex - TIME_RELAY_WORD_START;
		TimeRelayWordInf[wIndex].dwState = RELAY_STATE_A; //��״̬
		TimeRelayWordInf[wIndex].dwTime = 0;//�����ú��������;
	}
	else if((wRelayWordIndex >=CONST_TIME_RW_START)
			&&(wRelayWordIndex < CONST_TIME_RW_START + CONST_TIME_RW_NUM))
	{
		wIndex = wRelayWordIndex - CONST_TIME_RW_START;
		ConstTimeRelayWordInf[wIndex].dwState = RELAY_STATE_A; //��״̬
		ConstTimeRelayWordInf[wIndex].dwTime = 0;//�����ú��������;
	}
	else 
		return;
	
	//��̵����֣�ʱ��̵������鲻�����ϴ�״̬��
	byRelayWordValue[wRelayWordIndex] =0;
	

}
void ResetNVRamHoldRelayword(WORD wRelayWordIndex, WORD wRsv)
{
	// �嵥��Nvram�Ա��̵ּ����� 
	// ��ű߽��ж� 
	if (wRelayWordIndex < NKEEP_RW_START) return;
	if (wRelayWordIndex >= NKEEP_RW_START + NKEEP_RW_NUM) return;

	// ��̵����� ��Hold���黹������һ��״̬
	BYTE byOldValue;
	byOldValue = byRelayWordValue[wRelayWordIndex];
	byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
		
	byRelayWordValue[wRelayWordIndex] &= 0xF0;

	if(byOldValue != byRelayWordValue[wRelayWordIndex] )
	{
		
		//�б仯��д��nvram��
		WORD wNHIndex = wRelayWordIndex - NKEEP_RW_START;
		if(wNHIndex >= NKEEP_RW_NUM)
			return;
		g_pByNKeepRam[2*wNHIndex]= byRelayWordValue[wRelayWordIndex];
		g_pByNKeepRam[2*wNHIndex+1] = ~byRelayWordValue[wRelayWordIndex];	
	}
}


// ���Ա��̵ּ����� 
void ResetHoldRelayWord(WORD wRelayWordIndex, WORD wRsv)
{
	if (wRelayWordIndex == CLEAR_ALL_KEEP_RELAY_WORD)
	{
		// �����е��Ա��̵ּ����� 
		int i = 0;
		for (i = KEEP_RELAY_WORD_START; i < KEEP_RELAY_WORD_START + KEEP_RELAY_WORD_NUM; i++)
		{
			byRelayWordValue[i] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 

			byRelayWordValue[i] &= 0xF0;
		}
	}
	else
	{
		// �嵥���Ա��̵ּ����� 
		// ��ű߽��ж� 
		if (wRelayWordIndex < KEEP_RELAY_WORD_START) return;
		if (wRelayWordIndex >= KEEP_RELAY_WORD_START + KEEP_RELAY_WORD_NUM) return;

		// ��̵����� ��Hold���黹������һ��״̬
		byRelayWordValue[wRelayWordIndex] <<= 4;	// �ڸ�λ��¼�ϴ�״̬,�������ж� 
		
		byRelayWordValue[wRelayWordIndex] &= 0xF0;

	}
}

//105 ������������
void Report_Action(WORD wSeqID, WORD wReportIndex)
{
	TRelayEventItem RelayEventItem;
	
	ReadAbsTime(&RelayEventItem.stTime);	
	RelayEventItem.bStatus  = 1;	//����
	RelayEventItem.nINF	 = ReportInf[wReportIndex].wInf;//INF��
	for(int j=0;j<ReportInf[wReportIndex].dwFialureNum ;j++)
		RelayEventItem.FD_Value[j] = *(ReportInf[wReportIndex].pFailureData[j]);
		
	DB_RelayAction(wSeqID,&RelayEventItem);	//�����ݿ���д�붯������

}


//104 ������������
void Report_Start(WORD wSeqID, WORD wReportIndex)
{
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(wSeqID,ReportInf[wReportIndex].wInf,&AbsTime,TRUE);
	
}

//2011-12-13 ���Ӳ���Ϊinf�ŵı��溯��
void Report_StartByInf(WORD wSeqID, WORD wInf)
{
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(wSeqID,wInf,&AbsTime,TRUE);
}
//106 ��������
void Report_End(WORD wSeqID, WORD wRsv)
{
	DB_RelayEnd(wSeqID);
//	bIsRepStarted = FALSE;
}
//107 �澯����
void Report_Alarm(WORD wReportIndex, WORD wRsv)
{
	TRelayEventItem RelayEventItem;
	ReadAbsTime(&RelayEventItem.stTime);	
	RelayEventItem.bStatus  = 1;	//����
	RelayEventItem.nINF	 = ReportInf[wReportIndex].wInf;//INF��
	for(int j=0;j<ReportInf[wReportIndex].dwFialureNum ;j++)
		RelayEventItem.FD_Value[j] = *(ReportInf[wReportIndex].pFailureData[j]);
	
	DB_RelayAlarm(&RelayEventItem);
}
//2011-12-13 ���Ӳ���Ϊinf�ŵı��溯��
void Report_AlarmByInf(WORD wInf, WORD wRsv)
{
	TRelayEventItem RelayEventItem;
	ReadAbsTime(&RelayEventItem.stTime);	
	RelayEventItem.bStatus  = 1;	//����
	RelayEventItem.nINF	 = wInf;//INF��
	int nCount = DB_GetRENum(); //(sizeof(ReportInf))/sizeof(ReportInf[0]);
	WORD wReportIndex = WORD(-1);

	//ͨ��inf���ҵ�����˳���
	for(int i=0; i<nCount; i++)
	{
		if(ReportInf[i].wInf == wInf)
		{
			wReportIndex = i;
			break;
		}
			
	}
	if(wReportIndex != WORD(-1))
	{
		for(int j=0;j<ReportInf[wReportIndex].dwFialureNum ;j++)
			RelayEventItem.FD_Value[j] = *(ReportInf[wReportIndex].pFailureData[j]);
	}
	
	DB_RelayAlarm(&RelayEventItem);
}


//108 ��ѹ��Ͷ�˺���
// wSWRelayIndex ѹ��̵������
// WORD value 0��1 �˻���Ͷ
void SW_OnOff(WORD wSWRelayIndex, WORD value)
{
	if(wSWRelayIndex <SW_RELAY_WORD_START) return;
	if(wSWRelayIndex >SW_RELAY_WORD_START + SW_RELAY_WORD_NUM) return;

	//ֵ��ͬ�� ����Ҫ�޸�
	if(_ReadRelayWord_Normal(wSWRelayIndex) == value)
		return;
	
	WORD wSWIndex = SWIndexMap[wSWRelayIndex-SW_RELAY_WORD_START];
	(value) ? (DB_SetSWStatus(wSWIndex, SW_STATUS_SET)):(DB_SetSWStatus(wSWIndex, SW_STATUS_CUT));
	DB_SetSWStatusConfirm(wSWIndex, DB_EXECUTE);
}


//�߼�����ϵͳ����
//@TypeID	TypeName	TypeLabel	Describe
//@99	��ת�̵���	JMP	��ת������ ����Ϊ��Ż��к�
//@100	�����Ա��̵ּ���	RH	�����Ա��̵ּ��������� ����Ϊ�Ա��̵ּ���
//@101	����ʱ��̵���	RT	����ʱ��̵��������� ����Ϊʱ��̵���
//@102	������բ�̵���	RTR	����Ϊ��բ�̵���
//@103	�����źż̵���	RS	�źż̵������麯���� ����Ϊ�źż̵������
//@104 ��������	RSTART	�������棬 û�в���
//@105 ��������	RACTION �������棬 ����Ϊ�������
//@106 �������	REND	���������	û�в���
//@107 �澯����	RALARM	�澯���棬 ����Ϊ�������
//@108	ѹ��Ͷ��	    OFSL	    ѹ��Ͷ�˺����� ����Ϊѹ��̵����꣬��Ͷ����ON/OFF

static RELAY_SYSFUN_PTR s_pfnSysFunList[] =
{
	ResetHoldRelayWord, //100 �����Ա���
	ResetTimeRelayWord, //101 ����ʱ��
	ResetTripRelayWord, //102 ������բ�̵���
	ResetSignalRelayWord, //103 �����źż̵���
	Report_Start, //104 ������������
	Report_Action, //105 ������������
	Report_End, //106 ��������
	Report_Alarm, //107 �澯����
	SW_OnOff, //108 ѹ��Ͷ�˺���
	ResetNVRamHoldRelayword, //109 ���鲻�����Ա���
	IncreaseCount, //110 ����
	DecreaseCount, //111 �ݼ�
	ResetCount, //112 ��������̵���
};



/****************************************************************************************
 * 
 * ��������:  ����ִ���߼�����(һ��)
 * 
 * ����˵��: 
 *			- DWORD* CurLine
 *			  ����: 
 *			  ˵��: ��ǰִ�е��ڼ�����
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
	void RunLogicParseModule(DWORD* CurLine)
	{
		DWORD i = 0, j = 0;
		BOOL bResult = FALSE;		/* ִ�н�� */
		DWORD dwRelayIndex = 0; 	/* �̵����������� */
		DWORD dwCalcEndIndex = 0;
		DWORD dwResultRelayWordType = 0;
		WORD wResultRelayWordIndex = 0;
		struct _TLogicPara_CalcNode* p = NULL;	/* ��ǰִ�нڵ� */
		DWORD wTmp,wTmp2;
		BYTE mask[] = {0x01, 0x01, 0x11, 0x11, 0x11, 0x11};
		BYTE flag[] = {0x01, 0x00, 0x01, 0x10, 0x01, 0x10};
		DWORD dwIndex;
		
		struct _TLogicPara_Head* pLogicParaHead = s_pLogicParaHead; 	// �߼����̲����ļ�ͷ�� 
		struct _TLogicPara_Expression* pExpression = s_pExpression; 	// ���ʽ���� 
		struct _TLogicPara_CalcNode* pCalcNode = s_pCalcNode;			// ִ�нڵ����� 
	
		if((pLogicParaHead == NULL)||(pExpression == NULL)||(pCalcNode ==NULL))
			return;
		
		//����ǵ����߼�����
		if((g_dwPLCDebugStep1 == PLC_DEBUG_ST1)&&(g_dwPLCDebugStep2 == PLC_DEBUG_ST2))
		{
			DebugRunLogic(CurLine);
			return;
		}
	
	
		/* ѭ�����еı��ʽ */
		for (i = 0; i < pLogicParaHead->dwExpressionNum; i++)
		{
	
	//@ 	//����Ϊ����windows�µ�����
#ifdef __PL_WIN_SIMULATION__
			BOOL ret;
			ret = GetMessage(&msg, NULL, WM_USER, WM_USER+100);
			if(g_bIsEndThread)
			{
				AfxEndThread(0);
			}
#endif
	//@ 	//�����õ�
	
			dwCalcEndIndex = pExpression[i].wStartCalcNodeIndex + pExpression[i].wCalcNodeNum;
	
			/* ѭ��ִ�е�ǰ���ʽ������ִ�нڵ� */
			for (j = pExpression[i].wStartCalcNodeIndex; j < dwCalcEndIndex; j++)
			{
				/* ��ȡ��ǰ�̵��������� */
				p = &(pCalcNode[j]);
				dwRelayIndex = p->wRelayIndex;
	//			bResult = s_pfnReadList[p->dwGetType](dwRelayIndex);
	
				dwIndex = p->wGetType;
				if(dwIndex<4)
				{
					bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
											? TRUE : FALSE;
				}
				else
				{
					bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
											? FALSE : TRUE;
				}
				
				/*
				 * ��(1 + ...)��(0 * ...)�����ʱ��Ծ
				 * ����1:�������ִ�����͵�����,ʹ�жϼ�.
				 * ����2:��������Ϊ:
				 * if (1 + ...) {bResult = 1; j += ��תƫ��;} ��+��dwCalcType 10
				 * if (0 * ...) {bResult = 0; j += ��תƫ��;}  ��*" ��dwCalcType 11
				 * if (#) {bResult = �̵���������; break;}
				 * �ô��ϲ��˸�������,Ч�����,���������Ķ��Ѷ�.
				 */
				if ((bResult + p->wCalcType) == 11)
				{
					//if (p->wOffset == (DWORD)-1) break; 2011-12-9Ӧ����word -1����0xffff
					if (p->wOffset == (WORD)-1) break;
					j += p->wOffset - 1;	/* ע��: ��forѭ����ʼ��j++ */
				}
				
	
			}
	
			dwResultRelayWordType = pExpression[i].wResultRelayWordType;
			wResultRelayWordIndex = pExpression[i].wResultRelayWordIndex;
	
			if (dwResultRelayWordType == PULSE)
			{
				PulseRelayWordInf[wResultRelayWordIndex-PULSE_RELAY_WORD_START].dwTimeParam= pExpression[i].wParm;
			}
	
			if(dwResultRelayWordType < JMP)  //�̵���д����
			{
				// ��ִ�н��������̵������� 
				dwRelayIndex = pExpression[i].wResultRelayWordIndex;
				s_pfnWriteList[pExpression[i].wResultRelayWordType](dwRelayIndex, bResult);
			}
	
			else if(dwResultRelayWordType == JMP)// ��ת�̵���
			{
				  if(bResult)
				  {
					  wTmp = pExpression[i].wParm;
					  i+= wTmp -1; //������, -1ӦΪѭ�����Զ�+1
				  }
			}
	
			else if(dwResultRelayWordType < USERFUN_TYPE_BASE ) //�߼�����ϵͳ����
			{
				if(bResult)
				{
					  wTmp = pExpression[i].wParm;
					 wTmp2 = pExpression[i].wParm2;
					 s_pfnSysFunList[dwResultRelayWordType-SYSFUN_TYPE_BASE](wTmp, wTmp2);
				}
			}
	
			else if(dwResultRelayWordType >= USERFUN_TYPE_BASE) //�߼������Զ��庯������
			{
				wTmp = pExpression[i].wParm;
				wTmp2 = pExpression[i].wParm2;
				s_pfnUserFunList[dwResultRelayWordType-USERFUN_TYPE_BASE](wTmp,wTmp2, bResult);
	
			}
	
	
		}
		//2011-12-13����
		//����̬ʱʵʱ�к�û�����壬Ϊ�˼ӿ��߼��ٶȣ���ת��һȦ(�����������ʱһ��ÿһ��)����һ���к�
		if(pLogicParaHead->dwExpressionNum !=0)
			*CurLine = (i+1)%(pLogicParaHead->dwExpressionNum);

	}



/****************************************************************************************
 * 
 * ��������: ¼���̵���ֵ
 * 
 * ����˵��: 
 *			- WORD* pwRelayIndexList
 *			  ����: 
 *			  ˵��: �̵�������б� ��ʽ ���̵ܼ�������n�� �̵���1��ţ� �̵���2���,..�̵���n���
 *
 *			- BYTE* pDst
 *			  ����: 
 *			  ˵��: ¼����������
 *
 * ���ش���: 
 * 
 * ����: 
 * 
 *****************************************************************************************/
BOOL RecordRelayWord(WORD* pwRelayIndexList, LONG* pDst)
{
	WORD wCount;
	WORD byBitNo = 0;
	int i;

	wCount = pwRelayIndexList[0];

	memset(pDst,0,(wCount+15)/16*sizeof(DWORD));
	
	for(i=1; i<=wCount; i++)
	{
		byBitNo = i-1;
			
		if(ReadRelayWord(pwRelayIndexList[i]))
			pDst[byBitNo/16] |= 0x01<<(byBitNo%16);
		else
			pDst[byBitNo/16] &= ~(0x01<<(byBitNo%16));
	}
	return TRUE;

}

//д����
void CtrlTest(WORD wNo, BOOL  bIsStart)
{
	if (wNo >= CTRLTEST_RELAY_WORD_NUM) return;
	
	WORD wIndex = wNo + CTRLTEST_RELAY_WORD_START;

	_WriteRelayWord(wIndex, bIsStart);

}

//дң��Ԥ�ü̵���
void RemoteCtrlPre(WORD wRCNum, BOOL bContext)
{
//	if (wRCNum < RCP_RELAY_WORD_START) return;
	if (wRCNum >= RCP_RELAY_WORD_NUM) return;
	
	WORD wIndex = wRCNum + RCP_RELAY_WORD_START;
	
	_WriteRelayWord(wIndex, bContext);

}

//����У�̵���
BOOL RemoteCtrlVer(WORD wRCNum )
{
	if (wRCNum >=  RCV_RELAY_WORD_NUM) return FALSE;

	WORD wIndex = wRCNum + RCV_RELAY_WORD_START;
	return ReadRelayWord(wIndex);

}

 /****************************************************************************************
  * 
  * ��������: //дң��ִ�м̵���
  * 
  * ����˵��: 
  * 		 - WORD wRCNum
  * 		   ����: 
  * 		   ˵��: ң�����
  *
  *
  * 		 - BOOL bContext
  * 		   ����: 
  * 		   ˵��: д��ֵ TRURE �� FALSE
  *
  * ���ش���: 
  * 
  * ����: 
  * 
  *****************************************************************************************/

 void RemoteCtrlExe(WORD wRCNum, BOOL bContext)
{
	if (wRCNum >= RCE_RELAY_WORD_NUM) return;
	
	WORD wIndex = wRCNum + RCE_RELAY_WORD_START;
	
	_WriteRelayWord(wIndex, bContext);

}

//2012-02-13 д���ؿ���ִ�м̵��� wLCNum��ң�����
 void LocalCtrlExe(WORD wLCNum, BOOL bContext)
{
	if (wLCNum >= LCE_RELAY_WORD_NUM) return;
	
	WORD wIndex = wLCNum + LCE_RELAY_WORD_START;
	
	_WriteRelayWord(wIndex, bContext);
}


//ˢ��LED�̵�����дLed�̵�����LED��)
void UpDateLed()
{
	
	BOOL value;	
	static BOOL bOldValue[LED_RELAY_WORD_NUM];
	for (int i=0; i<LED_RELAY_WORD_NUM; i++)
	{
		value = ReadRelayWord(i+LED_RELAY_WORD_START);
#if(SYS_CPU_TYPE == CPU_COLDFIRE)
		LEDOnOff(i, value);
#endif
	/*	if(bOldValue[i]!=value)
		{
			LEDOnOff(i, value);
			bOldValue[i] = value;
		}
		*/
	}
	
}


//ˢ�³��ڼ̵�����д���ڼ̵��������ڣ�
void UpDateOutPut()
{
	WORD i, wBIIndex;;

	BOOL bVal;
	for (i=0; i<OUTPUT_RELAY_WORD_NUM; i++)
	{	
		bVal = ReadRelayWord(i+OUTPUT_RELAY_WORD_START);
//		byPreValue = byOutPreValue[i];
		
		wBIIndex = OutPutMap[i]; //�õ���i�����ڼ̵�����Ӧ��ʵ��BO��
		
		//���ԣ� �Ѿ�prvval==1 && bVal == 0   �ճ���
		//		 preval==0 && bVal == 1  ������	
		if(byOutPreValue[i] != bVal)
		{			
			//ReportMsg("OUT=%d, status=%d, laststatus=%d",i+1,bVal,byOutPreValue[i]);
			RealBOControl(wBIIndex, bVal);
			byOutPreValue[i] = bVal;
		}
	}

}

void UpDateInPut()
{
	WORD i;

	for (i=0; i<INPUT_RELAY_WORD_NUM; i++)
	{	
		//���ó�ң�ŵ�Ҫ��ȡ����ֵ�� �������óɵ�ȵĲ�����(��ʼ��ʱΪ0)
//		if(InputRelayWordBI[i] != -1)
//			_WriteRelayWord(i+INPUT_RELAY_WORD_START, DB_LogReadBI(InputRelayWordBI[i]));
//		_WriteRelayWord(i+INPUT_RELAY_WORD_START, DB_LogReadBI(i));
		_WriteRelayWord(i+INPUT_RELAY_WORD_START, ((pSysBI[i].bStatus&0x80) ? TRUE : FALSE) );

		
	}
}

//ˢ����ѹ�壨����ѹ�嵽�̵����֣�
void UpDateSW()
{
	WORD i;
	BYTE byStatus;

	for (i=0; i<SW_RELAY_WORD_NUM; i++)
	{
		byStatus = *(SWMap[i]);
		if (SW_STATUS_SET == byStatus)
		{
			_WriteRelayWord(i+SW_RELAY_WORD_START, TRUE);
		}
		else
		{
			_WriteRelayWord(i+SW_RELAY_WORD_START, FALSE);
		}


	}

}

//ˢ�����ã������õ��̵�����)
void UpDateCFG()
{
	WORD i;
	BYTE byStatus;
	
	for (i=0; i<CFG_RELAY_WORD_NUM; i++)
	{
		byStatus = *(CFGMap[i]);
			if (CFG_STATUS_SET == byStatus)
			{
				_WriteRelayWord(i+CFG_RELAY_WORD_START, TRUE);
			}
			else
			{
				_WriteRelayWord(i+CFG_RELAY_WORD_START, FALSE);
			}
	}

}

//���µ��Լ̵���(��buf�����̵��������)
void UpDataDebugRelay()
{

	int i;
	BYTE byValue;

	for (i=0; i<VIRTURAL_RELAY_WORD_NUM; i++)
	{	
		byValue = byDebugRelayBuf[i];
		if (byValue)
		{
			_WriteRelayWord(i+VIRTUAL_RELAY_WORD_START, TRUE);
		}
		else
			_WriteRelayWord(i+VIRTUAL_RELAY_WORD_START, FALSE);
		
		
	}
	
}
//�������Ӽ̵����ӽ��ܻ��������µ��̵�����
void UpDataRXLinkRelay()
{
	
		WORD i;

		for (i=0; i<RXLINK_RW_NUM; i++)
		{	
			_WriteRelayWord(i+RXLINK_RW_START, byRxLinkBuf[i]);
		}
	
}
//�Ӽ̵����ָ��µ��������ӻ�����, ֻҪ0��1ֵ�� ��Ҫ����
void UpDataTXLinkRelay()
{
	WORD i;
	BYTE byValue;
	for (i=0; i<TXLINK_RW_NUM; i++)
	{	  
		byValue = ReadRelayWord(i+TXLINK_RW_START);
		byTxLinkBuf[i] = byValue;
	}
}

//���¶����߼�������˵������ļ̵���ֵ(���룬 ���ã� ѹ��,���Լ̵���)
inline void UpDateInputRelayWord()
{
//	UpDateCFG();  kang �޸�Ϊ�ڱ��������н��յ����õ��޸���Ϣ�Ÿ������ú�ѹ��
//	UpDateSW();        ��OnRelayChange�е���

	UpDateInPut();
//	UpDataDebugRelay();
	
	UpDataRXLinkRelay();
}

//���¶����߼�������˵������ļ̵���ֵ(������ led,�������Ӽ̵���)
inline void UpDateOutputRelayWord()
{
#if (SYS_CPU_TYPE==CPU_COLDFIRE)    //1300 ����Ҫled����
	UpDateLed();
#endif
	UpDateOutPut();
	UpDataTXLinkRelay();
}

//kangqian    05/04/2011  ReadAllVirDI ���ص����⿪���Ϊ��λ���棬bit0��ʾvin0

STATUS ReadAllVirDI(BYTE *pBuf)
{	
	BYTE byBitNo;
	int i;

//	if(isVirDIValide == FALSE)
//		return  FAILURE;
	
	for(i=0; i<VIN_RELAY_WORD_NUM; i++)
	{
		byBitNo = i;
			
		if(byVInValue[i])
			pBuf[byBitNo/8] |= 0x01<<(byBitNo%8);
		else
			pBuf[byBitNo/8] &= ~(0x01<<(byBitNo%8));
	}

	return SUCCESS;
	
}

DWORD GetPLCDBVer()
{
	if(s_pCommonFileHead)
		return s_pCommonFileHead->dwVersion;
	else
		return 0;
}
//���߼�����У����
WORD GetPLCCheckSum()
{
	if(s_pwChecksum)
		return *s_pwChecksum;
	else
		return 0;
}
//�߼��Լ�
WORD PLCSelfCheck()
{
	if(g_PLCStatus == PLC_STATUS_ERR)
		return 0xFFFF;
	//�ļ�ͷ
	//�����ļ�ͷУ���� ������У���ļ�ͷ�� �ļ�ͷ��У��Ļ�s_pCommonFileHead->dwSize�ǲ����ŵ�
	WORD wCheck = DAE_CheckSum(g_LogicParaBuf, sizeof(TCommonFileHead)-4);
	DWORD dwCheck = MAKELONG(wCheck,0);

	if(dwCheck != s_pCommonFileHead->dwCheck)
	{
		LogError("PLCSelfCheck",FILE_LINE,"FileHead checksum error 0x%X, 0x%X", dwCheck,s_pCommonFileHead->dwCheck);
		g_PLCStatus = PLC_STATUS_ERR;
		return 0xFFFF;
	}

	//�ļ�
	wCheck = DAE_CheckSum(g_LogicParaBuf+sizeof(TCommonFileHead),
				s_pCommonFileHead->dwSize - 2-sizeof(TCommonFileHead));
	if(wCheck != *s_pwChecksum)
	{
		g_PLCStatus = PLC_STATUS_ERR;

		//���У������������߼�����
		LogError("PLCSelfCheck",FILE_LINE,"File checksum error 0x%X, 0x%X", wCheck,*s_pwChecksum);
		ResetLogicParseModule();
	}
	return 0;
}
/************************************************************
	���ܣ��߼����̴���,�ⲿ�����߼�����ʱѭ�����øú���
	��������
	���أ���
************************************************************/
void LogicProcess(void)
{	
	static DWORD dwCnt=0;

	if(g_PLCStatus == PLC_STATUS_ERR)
		return;
	
	//�߼����̴���
	if((byRelayWordValue[1] != 0x1)||(byRelayWordValue[0]!=0x0))
	{
		byRelayWordValue[1] = 0x1;
		LogError("CRelayTask",FILE_LINE,"RelayWord True1 changed");
	}
	
	UpDateInputRelayWord();
	RunLogicParseModule(&g_dwPLCCurLine);//ʹg_dwPLCCurLine���ŷ������и���
	UpDateOutputRelayWord();	

	if(isVirDIValide == FALSE)
	{
		dwCnt++;
		if(dwCnt>100)
		{
			isVirDIValide = TRUE;
		}
	}
}

/************************************************************
	���ܣ���ʼ���źż̵���������eeprom�е�ֵΪ0�� �ڸ����߼�����ʱ��Ҫ����
	��������
	���أ���
************************************************************/
void FormatSignalEEprom()
{
	if(SIGNAL_RELAY_WORD_NUM <= 0)
		return;
	BYTE byVal = 0;
	BYTE byNotVal = ~byVal;
		
	BYTE bBuf[SIGNAL_RELAY_WORD_NUM*2+1];
	int i=0;
	for(int i=0; i<SIGNAL_RELAY_WORD_NUM; i++)
	{
		bBuf[2*i] = byVal;
		bBuf[2*i+1] =  byNotVal;
	}
	EEPROM_Write(EEPROM_RELAY_SINGAL, SIGNAL_RELAY_WORD_NUM*2, bBuf);
	
	LogError("Logic", FILE_LINE, "All signal value  in eeprom reset", i);
}

/************************************************************
	���ܣ���ȡ��˸led��ֵ
	������
	     WORD wIndex[in] Ҫ����led��ţ� ΪLOGIC_READ_ALL_VALUEʱ��ȡ���е�led
	     BYTE* pByFrequency[out] ���ز���ͷָ��
	     BYTE* pByValue[out] ����ֵͷָ��
	���أ�LONG  0:��ȡ�ɹ� 1:��ȡʧ��
************************************************************/

LONG GetFlashLedValue(WORD wIndex, BYTE* pByFrequency, BYTE* pByValue)
{
	if(LOGIC_READ_ALL_VALUE == wIndex)
	{
		for(WORD i=0; i<LED_RELAY_WORD_NUM; i++)
		{
			pByFrequency[i] = (byLedFlashBuf[i]>>4)&0x0F;
			pByValue[i] = byLedFlashBuf[i]&0x01; //���λΪֵ
		}
		return 0;
		
	}
	else if(wIndex<LED_RELAY_WORD_NUM)
	{
		
		*pByFrequency = (byLedFlashBuf[wIndex]>>4)&0x0F;
		*pByValue = byLedFlashBuf[wIndex]&0x01; //���λΪֵ
		return 0;
	}
	return 1;
	
}



/************************************************************
	���ܣ���ȡ�������Ӽ̵�����ֵ
	������
	     WORD wStartIndex[in] Ҫ���ķ������Ӽ̵�����ʼ���
	     WORD wCount[in] Ҫ���ķ������Ӽ̵�������
	     
	     BYTE* pByValue[out] ָ�򷵻�ֵ��ָ��
	���أ�LONG  0:��ȡ�ɹ� 1:��ȡʧ��
************************************************************/
LONG GetTxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue)
{
	
	if((wStartIndex+wCount)>TXLINK_RW_NUM) return 1;
	
	memcpy(pByValue, &(byTxLinkBuf[wStartIndex]),wCount);
	return 0;
}

/************************************************************
	���ܣ����ý������Ӽ̵�����ֵ
	������
	     WORD wStartIndex[in] Ҫд�Ľ������Ӽ̵�����ʼ���
	     WORD wCount[in] Ҫд�Ľ������Ӽ̵�������
	     
	     BYTE* pByValue[in] ָ������ֵ��ָ��
	���أ�LONG  0:��ȡ�ɹ� 1:��ȡʧ��
************************************************************/

LONG SetRxLinkValue(WORD wStartIndex, WORD wCount, BYTE* pByValue)
{
	if((wStartIndex+wCount)>RXLINK_RW_NUM) return 1;
	
	memcpy(&(byRxLinkBuf[wStartIndex]),pByValue, wCount);
	return 0;
}

void WriteDebugRW(WORD wIndex,BYTE byVal)
{
	WORD wDebugIndex;

	if(wIndex < VIRTUAL_RELAY_WORD_START)
		return;

	wDebugIndex = wIndex - VIRTUAL_RELAY_WORD_START;

	if(wDebugIndex >= VIRTURAL_RELAY_WORD_NUM)
		return;
	
	byDebugRelayBuf[wDebugIndex] = byVal;
}
/************************************************************
	���ܣ����̵�������ȫֵ����ֻ��0��1״̬����λ���������ϴε�״̬
	������
	     WORD wIndex[in] �̵������
	     
	���أ�BYTE  �̵�����ȫֵ������bit0�ǵ�ǰֵ��bit4���ϴε�ֵ
************************************************************/
BYTE ReadRWFullValue(WORD wIndex)
{
	if(wIndex >= RELAY_WORD_NUM)
		return BYTE(-1);
	return byRelayWordValue[wIndex];
}

const WORD GetPLCDebugBreakPoint()
{
	return g_dwPLCBreakPoint;
}
const WORD GetPLCCurLine()
{
	return g_dwPLCCurLine;
}

void SetPLCDebugBreakPoint(DWORD dwVal)
{
	g_dwPLCBreakPoint = dwVal;
}

void SetPLCDebugStep1(DWORD dwVal)
{
	g_dwPLCDebugStep1 = dwVal;
}
void SetPLCDebugStep2(DWORD dwVal)
{
	g_dwPLCDebugStep2 = dwVal;
}
void StopPLCDebug()
{
	g_dwPLCDebugStep1 = 0;
	g_dwPLCDebugStep2 = 0;
	g_dwPLCBreakPoint = 0;
}

void SetPLCDebugLoop(DWORD dwVal)
{
	g_dwPLCDebugLoop = dwVal;
}
void SetPLCDebugOneStep(DWORD dwVal)
{
	g_dwPLCDebugOneStep = 1;
}
/****************************************************************************************
 * 
 * ��������:  ����ִ���߼�����(һ��) 
 * 
 * ����˵��: 
 *			- long* CurLine
 *			  ����: 
 *			  ˵��: ��ǰִ�е��ڼ�����
 *
 * ���ش���: 
 * 
 * ����: ��ʵ��Ҫִ�в��ֺ�RunLogicParseModule��ȫ��ͬ��ֻ����ʼ�кźͽ����к�istart��iend
 ���������в�ͬ������Ϊ�˱�����Զ����������߼���� ʵ������Ч�ʼ�����ɶ��ԵĽ��ͣ�
 ���ƴ����γɶ�����RunDebugLogic���� ��������RunLogicParseModule���Ӷ������
 1.��������ʱ�Ǵ�0�����е����һ�У������м���ô��ת(jmpֻ֧��������ת��
 			���Բ����������У�ֻ���Ǵ���Ծ�Ĵ�0���е����һ��
 		 2.���Ե���ִ�� �ж�g_dwPLCBreakPointΪ-1 �ǵ�������һ���s����iend��istart+1,�����Ƿ�����ת������ִֻ��һ�У�ִ��������
 		 g_dwPLCBreakPoint Ϊg_dwPLCCurLine
 		 3.���е��ϵ� iEndΪ�ϵ㣬1)���g_dwPLCBreakPointС��iStart, ����ҪתһȦ�ģ���endΪ���кţ��������е�
 		   ĩ�У��´�ѭ������ĩ�����е��ϵ��С�
 		   2)��������iendΪ�ϵ�
 * 
 *****************************************************************************************/
void DebugRunLogic(DWORD* CurLine)
{
	DWORD i = 0, j = 0;
	BOOL bResult = FALSE;		/* ִ�н�� */
	DWORD dwRelayIndex = 0;		/* �̵����������� */
	DWORD dwCalcEndIndex = 0;
	DWORD dwResultRelayWordType = 0;
	WORD wResultRelayWordIndex = 0;
	struct _TLogicPara_CalcNode* p = NULL;	/* ��ǰִ�нڵ� */
	DWORD wTmp,wTmp2;
	BYTE mask[] = {0x01, 0x01, 0x11, 0x11, 0x11, 0x11};
	BYTE flag[] = {0x01, 0x00, 0x01, 0x10, 0x01, 0x10};
	DWORD dwIndex;
	
	struct _TLogicPara_Head* pLogicParaHead = s_pLogicParaHead;		// �߼����̲����ļ�ͷ�� 
	struct _TLogicPara_Expression* pExpression = s_pExpression;		// ���ʽ���� 
	struct _TLogicPara_CalcNode* pCalcNode = s_pCalcNode;			// ִ�нڵ����� 

	if((pLogicParaHead == NULL)||(pExpression == NULL)||(pCalcNode ==NULL))
		return;

	//�߼����̵�����
	DWORD iStart=0, iEnd=0;

	iStart = g_dwPLCCurLine;//+1;
		//2
	//�������У���������һ��(��������ת�������У���������һ��
	if(g_dwPLCDebugOneStep >0)
	{
		iEnd = iStart+1;
	}
	else
	{
		//����ѭ�������е�ĩ�У��´δ�ĩ�����е��ϵ�
		if((g_dwPLCBreakPoint<iStart) //�ϵ��ڵ�ǰ��ǰ�棬Ҳ������Ҫ��תһȦ�����ŵ��ϵ�
			|| ((g_dwPLCBreakPoint == iStart)&&(g_dwPLCDebugLoop>0)) ) //תһȦ
		{
			iEnd = pLogicParaHead->dwExpressionNum;
		}
		else
		{
			iEnd = g_dwPLCBreakPoint;
		}
		
		if(g_dwPLCDebugLoop>0)
			g_dwPLCDebugLoop--;
		else
			g_dwPLCDebugLoop = 0;
	}
	/* ѭ�����еı��ʽ */
	for (i = iStart; i <iEnd ; i++)
	{

//@		//����Ϊ����windows�µ�����
#ifdef __PL_WIN_SIMULATION__
		BOOL ret;
		ret = GetMessage(&msg, NULL, WM_USER, WM_USER+100);
		if(g_bIsEndThread)
		{
			AfxEndThread(0);
		}
#endif
//@		//�����õ�

		dwCalcEndIndex = pExpression[i].wStartCalcNodeIndex + pExpression[i].wCalcNodeNum;

		/* ѭ��ִ�е�ǰ���ʽ������ִ�нڵ� */
		for (j = pExpression[i].wStartCalcNodeIndex; j < dwCalcEndIndex; j++)
		{
			/* ��ȡ��ǰ�̵��������� */
			p = &(pCalcNode[j]);
			dwRelayIndex = p->wRelayIndex;
//			bResult = s_pfnReadList[p->dwGetType](dwRelayIndex);

			dwIndex = p->wGetType;
			if(dwIndex<4)
			{
				bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
										? TRUE : FALSE;
			}
			else
			{
				bResult = ((byRelayWordValue[dwRelayIndex] & mask[dwIndex]) == flag[dwIndex]) \
										? FALSE : TRUE;
			}
			
			/*
			 * ��(1 + ...)��(0 * ...)�����ʱ��Ծ
			 * ����1:�������ִ�����͵�����,ʹ�жϼ�.
			 * ����2:��������Ϊ:
			 * if (1 + ...) {bResult = 1; j += ��תƫ��;} ��+��dwCalcType 10
			 * if (0 * ...) {bResult = 0; j += ��תƫ��;}  ��*" ��dwCalcType 11
			 * if (#) {bResult = �̵���������; break;}
			 * �ô��ϲ��˸�������,Ч�����,���������Ķ��Ѷ�.
			 */
			if ((bResult + p->wCalcType) == 11)
			{
				//if (p->wOffset == (DWORD)-1) break; 2011-12-9Ӧ����word -1����0xffff
				if (p->wOffset == (WORD)-1) break;
				j += p->wOffset - 1;	/* ע��: ��forѭ����ʼ��j++ */
			}
			

		}

		dwResultRelayWordType = pExpression[i].wResultRelayWordType;
		wResultRelayWordIndex = pExpression[i].wResultRelayWordIndex;

		if (dwResultRelayWordType == PULSE)
		{
			PulseRelayWordInf[wResultRelayWordIndex-PULSE_RELAY_WORD_START].dwTimeParam= pExpression[i].wParm;
		}

		if(dwResultRelayWordType < JMP)  //�̵���д����
		{
			// ��ִ�н��������̵������� 
			dwRelayIndex = pExpression[i].wResultRelayWordIndex;
			s_pfnWriteList[pExpression[i].wResultRelayWordType](dwRelayIndex, bResult);
		}

		else if(dwResultRelayWordType == JMP)//	��ת�̵���
		{
			  if(bResult)
			  {
				  wTmp = pExpression[i].wParm;
				  i+= wTmp -1; //������, -1ӦΪѭ�����Զ�+1
			  }
		}

		else if(dwResultRelayWordType < USERFUN_TYPE_BASE ) //�߼�����ϵͳ����
		{
			if(bResult)
			{
				  wTmp = pExpression[i].wParm;
				 wTmp2 = pExpression[i].wParm2;
				 s_pfnSysFunList[dwResultRelayWordType-SYSFUN_TYPE_BASE](wTmp, wTmp2);
			}
		}

		else if(dwResultRelayWordType >= USERFUN_TYPE_BASE) //�߼������Զ��庯������
		{
			wTmp = pExpression[i].wParm;
			wTmp2 = pExpression[i].wParm2;
			s_pfnUserFunList[dwResultRelayWordType-USERFUN_TYPE_BASE](wTmp,wTmp2, bResult);

		}
		if(pLogicParaHead->dwExpressionNum !=0)
			*CurLine = (i+1)%(pLogicParaHead->dwExpressionNum);
		//CurLine��ʾ������Ҫִ�е���

	}

	if(g_dwPLCDebugOneStep>0)
	{
		g_dwPLCDebugOneStep--;
		g_dwPLCBreakPoint = *CurLine;
	}	
}


void WriteDebugIn(WORD wDebugInIndex,BYTE byVal)
{
	if(wDebugInIndex >= DEBUG_IN_NUM)
		return;

	byDebugInRWBuf[wDebugInIndex] = byVal;
}

BYTE ReadDebugOut(WORD wDebugOutIndex)
{
	
	if(wDebugOutIndex >= DEBUG_OUT_NUM)
		return FALSE1;
	return byDebugOutRwBuf[wDebugOutIndex];
}
//���µ��Կ���̵��� (�ӹ�Լ���������µ��̵�����2012-02-08
void UpDataDebugInRW()
{
	WORD i;
	
	BOOL bVal;
	for (i=0; i<DEBUG_IN_NUM; i++)
	{
		_WriteRelayWord(i+DEBUG_IN_START,((byDebugInRWBuf[i]&0x01) ? TRUE : FALSE) );
	}
	
}

//���µ��Կ����̵��� (�Ӽ̵������µ���Լ��������2012-02-08
void UpDataDebugOutRW()
{
	WORD i, wBIIndex;;
	
	BOOL byVal;
	for (i=0; i<DEBUG_OUT_NUM; i++)
	{	
		byVal = ReadRelayWord(i+DEBUG_OUT_START);
		byDebugOutRwBuf[i] = byVal&0x01;
	}
}
//�����̵���
void ResetCount(WORD wRelayWordIndex, WORD wRsv)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	nCountRelayBuf[wIndex] = 0;
	return;
}
void IncreaseCount(WORD wRelayWordIndex, WORD wOffset)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	nCountRelayBuf[wIndex] += wOffset;
}
void DecreaseCount(WORD wRelayWordIndex, WORD wOffset)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	
	if(nCountRelayBuf[wIndex] <= wOffset)
	{
			nCountRelayBuf[wIndex] = 0;
			return;
	}
	
	nCountRelayBuf[wIndex] -= wOffset;
}
//���̵�������Ŷ�����ֵ
WORD ReadCountByIndex(WORD wRelayWordIndex)
{
	
	if (wRelayWordIndex < CNT_RELAY_WORD_START) return 0;
	if (wRelayWordIndex >= CNT_RELAY_WORD_START + CNT_RELAY_WORD_NUM) return 0;
	WORD wIndex = wRelayWordIndex - CNT_RELAY_WORD_START;
	return nCountRelayBuf[wIndex];
}

//�������̵����Ŷ�����ֵ
WORD ReadCountByNo(WORD CountNo)
{
	if (CountNo >= CNT_RELAY_WORD_NUM) return 0;
	return nCountRelayBuf[CountNo];
}



