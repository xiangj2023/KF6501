/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			RelayDef.h
* 
*   ���ģ�飺�����ӿ��ļ�
*
* 
*	����  ���������ļ����ϰ벿���ǹ̶������ݽṹ�� �°벿���ɱ����ӿڹ�������Զ�����
* 
*	ע��	�����ֶ��޸ĸ��ļ�
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-12-17		��ʼ����
*		kangqian	2010-04-21		����ѹ��Ͷ�˼̵���ʹ�õĲ�������
*************************************************************************/

#ifndef __RelayDef_H
#define __RelayDef_H


#include "sysdefs.h"


#ifdef __PL_WIN_SIMULATION__
	// ��windowģ��ƽ̨��ʹ��һ������ģ��ram 
	extern unsigned char _RAMDataBuf[];
	#define RELAY_RAM_ADDR_BASE	_RAMDataBuf
#else
#include "resm5234.h"
	//�������õ�Ƭ��ram��ַ
	#define RELAY_RAM_ADDR_BASE	RELAY_RAM_BASE // Ӳ������
#endif

//��ֵ ֵ���� ����󳤶�
#define  MAX_SETVALUNAME  256 

// ���������ṹ
typedef struct TReportInf{
    DWORD dwFialureNum;                // ���ϵ������� 
    const LONG** pFailureData; // ���ϵ���������Ϣ 
	WORD wInf; //����inf��
}TReportInf;

//�߼�������ʱ��̵���ʱ�������������ֵ������ֵ��
typedef struct TTimeSetting{
	LONG* pStartTime; //����ʱ�䣬ָ������Ķ�ֵ��������ֵ��
	LONG* pReturnTime;	//����ʱ�䣬ָ������Ķ�ֵ��������ֵ��
}TTimeSetting;

//��ֵȡֵ�����б�
typedef struct TSetValueInf
{
	LONG lValue;
	char strName[MAX_SETVALUNAME];
}TSetValueInf;

//��ֵȡֵ������
typedef struct TSetValueInfMap
{
	LONG nItem;
	TSetValueInf* pSetValueInf;
}TSetValueInfMap;

extern const WORD RelayWordTypeList[]; //�̵������ͱ�
extern const struct TSetValueInfMap SetValueInfMap[];
extern const struct TSetValueInf SetValueInf[];

extern const struct TSetValueInfMap SysSetValueInfMap[];
extern const struct TSetValueInf SysSetValueInf[];

extern const struct TTimeSetting TimeSetting[];          // ʱ��̵�����ֵ
extern  LONG*	TripSetting[]; //��բ�̵�����ʱ��ֵ

extern WORD const OutPutMap[]; //����̵���-���� ӳ��� ���濪����
extern WORD const InPutMap[]; //����̵���-���� ӳ���

extern BYTE* const SWMap[]; //ѹ��ӳ��� ֱ��ָ���������ѹ�����ڴ��еĸ���
extern BYTE* const CFGMap[]; //����ӳ���

extern const struct TReportInf ReportInf[];          // �������� 
//ϵ��Ĭ��ֵ
extern const LONG CFDefaultVal1[]; //ϵ��1
extern const LONG CFDefaultVal2[]; //ϵ��2

//2010-4-21 11:25:33 
extern WORD const SWIndexMap[]; //ѹ��̵�����ѹ��ŵ�ӳ���ϵ�� ������ѹ�����
                                //ע����SWMap[]������SWIndexMap��Ϊͨ��ѹ��Ͷ�˼̵���Ͷ����ѹ��ʱʹ�õ�

//���´����ɹ����������


#define 	 SETTING_AREA_COUNT 	 16 		 //��ֵ������
#define 	 RELAY_DATA_COUNT 	 16 		 //�Ŷ�����������
#define 	 SAMPLE_COUNT 	 32 		 //ÿ�ܲ���������
#define 	 A15TYPE_SYSSETNO 	 0 		 //1,5A ���͸�����ֵ���
#define 	 FAILDATA_DISPTYPE_CFGNO 	 12 		 //���ϵ�����ʾ�����������
#define 	 LangTYPE_SYSSETNO 	 0 		 //�������͸�����ֵ���

//��ǰ��ֵ���Ŷ���-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //��ǰ��ֵ����

//��ֵȡֵ�궨��
//----��ѹ������Ƕ�У��-----
#define 	WCTH_0 	0	//��У��
#define 	WCTH_1 	1	//30��У��
#define 	WCTH_2 	2	//60��У��
#define 	WCTH_3 	3	//90��У��
#define 	WCTH_4 	4	//120��У��
#define 	WCTH_5 	5	//150��У��
#define 	WCTH_6 	6	//180��У��
#define 	WCTH_7 	7	//210��У��
#define 	WCTH_8 	8	//240��У��
#define 	WCTH_9 	9	//270��У��
#define 	WCTH_10 	10	//300��У��
#define 	WCTH_11 	11	//330��У��
#define 	WCTH_12 	12	//360��У��
//----��ѹ������Ƕ�У��-----
#define 	WCTM_0 	0	//��У��
#define 	WCTM_1 	1	//30��У��
#define 	WCTM_2 	2	//60��У��
#define 	WCTM_3 	3	//90��У��
#define 	WCTM_4 	4	//120��У��
#define 	WCTM_5 	5	//150��У��
#define 	WCTM_6 	6	//180��У��
#define 	WCTM_7 	7	//210��У��
#define 	WCTM_8 	8	//240��У��
#define 	WCTM_9 	9	//270��У��
#define 	WCTM_10 	10	//300��У��
#define 	WCTM_11 	11	//330��У��
#define 	WCTM_12 	12	//360��У��
//----��ѹ������Ƕ�У��-----
#define 	WCTL_0 	0	//��У��
#define 	WCTL_1 	1	//30��У��
#define 	WCTL_2 	2	//60��У��
#define 	WCTL_3 	3	//90��У��
#define 	WCTL_4 	4	//120��У��
#define 	WCTL_5 	5	//150��У��
#define 	WCTL_6 	6	//180��У��
#define 	WCTL_7 	7	//210��У��
#define 	WCTL_8 	8	//240��У��
#define 	WCTL_9 	9	//270��У��
#define 	WCTL_10 	10	//300��У��
#define 	WCTL_11 	11	//330��У��
#define 	WCTL_12 	12	//360��У��
//----�߲�����/CT���ӷ�ʽ-----
#define 	WCTH_0 	0	//��/����
#define 	WCTH_1 	1	//��/����
#define 	WCTH_2 	2	//��/����
//----�в�����/CT���ӷ�ʽ-----
#define 	WCTM_0 	0	//��/����
#define 	WCTM_1 	1	//��/����
#define 	WCTM_2 	2	//��/����
//----�Ͳ�����/CT���ӷ�ʽ-----
#define 	WCTL_0 	0	//��/����
#define 	WCTL_1 	1	//��/����
#define 	WCTL_2 	2	//��/����
//----��ѹ�๫����-----
#define 	MBPH_0 	0	//A��
#define 	MBPH_1 	1	//B��
#define 	MBPH_2 	2	//C��
//----��ѹ�๫����-----
#define 	LBPH_0 	0	//A��
#define 	LBPH_1 	1	//B��
#define 	LBPH_2 	2	//C��

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//��������ֵ����-----------------------
#define  RM_UA 0
extern LONG* 	RM_pnUA; 		 // UA
#define  RM_UB 1
extern LONG* 	RM_pnUB; 		 // UB
#define  RM_UC 2
extern LONG* 	RM_pnUC; 		 // UC
#define  RM_UAB 3
extern LONG* 	RM_pnUAB; 		 // UAB
#define  RM_UBC 4
extern LONG* 	RM_pnUBC; 		 // UBC
#define  RM_UCA 5
extern LONG* 	RM_pnUCA; 		 // UCA
#define  RM_U2H 6
extern LONG* 	RM_pnU2H; 		 // U2H
#define  RM_U0 7
extern LONG* 	RM_pnU0; 		 // U0
#define  RM_IHA 8
extern LONG* 	RM_pnIHA; 		 // IHA
#define  RM_IHB 9
extern LONG* 	RM_pnIHB; 		 // IHB
#define  RM_IHC 10
extern LONG* 	RM_pnIHC; 		 // IHC
#define  RM_IMA 11
extern LONG* 	RM_pnIMA; 		 // IMA
#define  RM_IMB 12
extern LONG* 	RM_pnIMB; 		 // IMB
#define  RM_IMC 13
extern LONG* 	RM_pnIMC; 		 // IMC
#define  RM_ILA 14
extern LONG* 	RM_pnILA; 		 // ILA
#define  RM_ILB 15
extern LONG* 	RM_pnILB; 		 // ILB
#define  RM_ILC 16
extern LONG* 	RM_pnILC; 		 // ILC
#define  RM_I0 17
extern LONG* 	RM_pnI0; 		 // I0
#define  RM_IJX 18
extern LONG* 	RM_pnIjx; 		 // Ijx
#define  RM_IDA 19
extern LONG* 	RM_pnIDA; 		 // IDA
#define  RM_IDB 20
extern LONG* 	RM_pnIDB; 		 // IDB
#define  RM_IDC 21
extern LONG* 	RM_pnIDC; 		 // IDC
#define  RM_IRA 22
extern LONG* 	RM_pnIRA; 		 // IRA
#define  RM_IRB 23
extern LONG* 	RM_pnIRB; 		 // IRB
#define  RM_IRC 24
extern LONG* 	RM_pnIRC; 		 // IRC
#define  RM_IDA2 25
extern LONG* 	RM_pnIDA2; 		 // IDA2
#define  RM_IDA3 26
extern LONG* 	RM_pnIDA3; 		 // IDA3
#define  RM_IDA5 27
extern LONG* 	RM_pnIDA5; 		 // IDA5
#define  RM_IDB2 28
extern LONG* 	RM_pnIDB2; 		 // IDB2
#define  RM_IDB3 29
extern LONG* 	RM_pnIDB3; 		 // IDB3
#define  RM_IDB5 30
extern LONG* 	RM_pnIDB5; 		 // IDB5
#define  RM_IDC2 31
extern LONG* 	RM_pnIDC2; 		 // IDC2
#define  RM_IDC3 32
extern LONG* 	RM_pnIDC3; 		 // IDC3
#define  RM_IDC5 33
extern LONG* 	RM_pnIDC5; 		 // IDC5
#define  RM_IRA2 34
extern LONG* 	RM_pnIRA2; 		 // IRA2
#define  RM_IRB2 35
extern LONG* 	RM_pnIRB2; 		 // IRB2
#define  RM_IRC2 36
extern LONG* 	RM_pnIRC2; 		 // IRC2
#define  RM_AUA 37
extern LONG* 	RM_pnAUA; 		 // ��UA
#define  RM_AUB 38
extern LONG* 	RM_pnAUB; 		 // ��UB
#define  RM_AUC 39
extern LONG* 	RM_pnAUC; 		 // ��UC
#define  RM_AIHA 40
extern LONG* 	RM_pnAIHA; 		 // ��IHA
#define  RM_AIHB 41
extern LONG* 	RM_pnAIHB; 		 // ��IHB
#define  RM_AIHC 42
extern LONG* 	RM_pnAIHC; 		 // ��IHC
#define  RM_AIMA 43
extern LONG* 	RM_pnAIMA; 		 // ��IMA
#define  RM_AIMB 44
extern LONG* 	RM_pnAIMB; 		 // ��IMB
#define  RM_AIMC 45
extern LONG* 	RM_pnAIMC; 		 // ��IMC
#define  RM_AILA 46
extern LONG* 	RM_pnAILA; 		 // ��ILA
#define  RM_AILB 47
extern LONG* 	RM_pnAILB; 		 // ��ILB
#define  RM_AILC 48
extern LONG* 	RM_pnAILC; 		 // ��ILC
#define  RM_BOBRCH1 49
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 50
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 51
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 52
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     53    //��������ֵ����
#define RM_BUF_SIZE     212    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UA 0
extern LONG* 	M_pnUA; 		 //UA UA
#define  M_UB 1
extern LONG* 	M_pnUB; 		 //UB UB
#define  M_UC 2
extern LONG* 	M_pnUC; 		 //UC UC
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_IHAM 4
extern LONG* 	M_pnIHAM; 		 //IHAM IHAM
#define  M_IHBM 5
extern LONG* 	M_pnIHBM; 		 //IHBM IHBM
#define  M_IHCM 6
extern LONG* 	M_pnIHCM; 		 //IHCM IHCM
#define  M_IMAM 7
extern LONG* 	M_pnIMAM; 		 //IMAM IMAM
#define  M_IMBM 8
extern LONG* 	M_pnIMBM; 		 //IMBM IMBM
#define  M_IMCM 9
extern LONG* 	M_pnIMCM; 		 //IMCM IMCM
#define  M_ILAM 10
extern LONG* 	M_pnILAM; 		 //ILAM ILAM
#define  M_ILBM 11
extern LONG* 	M_pnILBM; 		 //ILBM ILBM
#define  M_ILCM 12
extern LONG* 	M_pnILCM; 		 //ILCM ILCM
#define  M_I0M 13
extern LONG* 	M_pnI0M; 		 //I0M I0M
#define  M_IJXM 14
extern LONG* 	M_pnIjxM; 		 //IjxM IjxM

#define MEASURE_COUNT     15    //����ֵ����
#define MEASURE_BUF_SIZE     60    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 216)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby87U; 		 //������ٶ�
extern BYTE* 	SW_pby87R; 		 //���ʲ����
extern BYTE* 	SW_pby50I_H; 		 //��ѹ���������
extern BYTE* 	SW_pby50I_M; 		 //��ѹ���������
extern BYTE* 	SW_pby50I_L; 		 //��ѹ���������
extern BYTE* 	SW_pby50OL; 		 //�����ɱ���
extern BYTE* 	SW_pby27U; 		 //ʧѹ����
extern BYTE* 	SW_pby59UN; 		 //�����ѹ����
extern BYTE* 	SW_pby50IN; 		 //�����������
extern BYTE* 	SW_pby50IJX; 		 //��϶��������
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     13    //��ѹ�����
#define SW_BUF_SIZE     13    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 276)

//����----------------------------------
extern BYTE* 	CFG_pby87U_A; 		 //A������ٶϱ���
extern BYTE* 	CFG_pby87U_B; 		 //B������ٶϱ���
extern BYTE* 	CFG_pby87U_C; 		 //C������ٶϱ���
extern BYTE* 	CFG_pby87R_A; 		 //A����ʲ����
extern BYTE* 	CFG_pby87R_B; 		 //B����ʲ����
extern BYTE* 	CFG_pby87R_C; 		 //C����ʲ����
extern BYTE* 	CFG_pbyDCRES_87R; 		 //������ڷ�������
extern BYTE* 	CFG_pby68IdH2M_87R; 		 //����������������
extern BYTE* 	CFG_pby68IdH2C_87R; 		 //���������ۺ������
extern BYTE* 	CFG_pby68IdH2I_87R; 		 //�������η������
extern BYTE* 	CFG_pby68IrH2I_87R; 		 //�ƶ��������α���
extern BYTE* 	CFG_pby68IdH5I_87R; 		 //�������г������
extern BYTE* 	CFG_pby68IdHMUL_87R; 		 //�����ۺ�г������
extern BYTE* 	CFG_pbyMAB; 		 //��ѹ������
extern BYTE* 	CFG_pbyLAB; 		 //��ѹ������
extern BYTE* 	CFG_pby50I_H1; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_H1_PD; 		 //�߹�����ι��ʷ���
extern BYTE* 	CFG_pby50I_H1_47U; 		 //�߹�����θ�ѹ����
extern BYTE* 	CFG_pby50I_H2; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_H2_PD; 		 //�߹�����ι��ʷ���
extern BYTE* 	CFG_pby50I_H2_47U; 		 //�߹�����θ�ѹ����
extern BYTE* 	CFG_pby50I_M1; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_M2; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_L1; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_L2; 		 //��ѹ��������
extern BYTE* 	CFG_pby50OL_1; 		 //�����ɢ��
extern BYTE* 	CFG_pby50OL_2; 		 //�����ɢ��
extern BYTE* 	CFG_pby50OL_3; 		 //�����ɢ��
extern BYTE* 	CFG_pby27U; 		 //ʧѹ����
extern BYTE* 	CFG_pby27U_50I; 		 //ʧѹ������������
extern BYTE* 	CFG_pby59UN; 		 //�����ѹ����
extern BYTE* 	CFG_pby50IN; 		 //�����������
extern BYTE* 	CFG_pby50IJX; 		 //��϶��������
extern BYTE* 	CFG_pby74VT; 		 //PT����
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     37    //���ø���/
#define CFG_BUF_SIZE     37    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 290)

//��ֵ----------------------------------
extern LONG* 	RS_pnCFG1; 		 //������1 CFG1
extern LONG* 	RS_pnCFG2; 		 //������2 CFG2
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pnWHCTC; 		 //��ѹ������Ƕ�У�� WHCTC
extern LONG* 	RS_pnWMCTC; 		 //��ѹ������Ƕ�У�� WMCTC
extern LONG* 	RS_pnWLCTC; 		 //��ѹ������Ƕ�У�� WLCTC
extern LONG* 	RS_pnWCTHCON; 		 //�߲�����/CT���ӷ�ʽ WCTHCON
extern LONG* 	RS_pnWCTMCON; 		 //�в�����/CT���ӷ�ʽ WCTMCON
extern LONG* 	RS_pnWCTLCON; 		 //�Ͳ�����/CT���ӷ�ʽ WCTLCON
extern LONG* 	RS_pnMBph; 		 //��ѹ�๫���� MBph
extern LONG* 	RS_pnLBph; 		 //��ѹ�๫���� LBph
extern LONG* 	RS_pn87UIA; 		 //A���ٶ϶�ֵ 87UIA
extern LONG* 	RS_pn87RIA; 		 //A��������ֵ 87RIA
extern LONG* 	RS_pnIRS1A; 		 //A���ƶ��յ�1 IRS1A
extern LONG* 	RS_pnIRS2A; 		 //A���ƶ��յ�2 IRS2A
extern LONG* 	RS_pn87UIB; 		 //B���ٶ϶�ֵ 87UIB
extern LONG* 	RS_pn87RIB; 		 //B��������ֵ 87RIB
extern LONG* 	RS_pnIRS1B; 		 //B���ƶ��յ�1 IRS1B
extern LONG* 	RS_pnIRS2B; 		 //B���ƶ��յ�2 IRS2B
extern LONG* 	RS_pn87UIC; 		 //C���ٶ϶�ֵ 87UIC
extern LONG* 	RS_pn87RIC; 		 //C��������ֵ 87RIC
extern LONG* 	RS_pnIRS1C; 		 //C���ƶ��յ�1 IRS1C
extern LONG* 	RS_pnIRS2C; 		 //C���ƶ��յ�2 IRS2C
extern LONG* 	RS_pnSLP1; 		 //�����ƶ�ϵ��1 SLP1
extern LONG* 	RS_pnSLP2; 		 //�����ƶ�ϵ��2 SLP2
extern LONG* 	RS_pnKrh2; 		 //����г���ƶ�ϵ�� Krh2
extern LONG* 	RS_pnKrh5; 		 //���г���ƶ�ϵ�� Krh5
extern LONG* 	RS_pnKrhm; 		 //�ۺ�г���ƶ�ϵ�� Krhm
extern LONG* 	RS_pnKrav; 		 //������ڼ�Ȩϵ�� Krav
extern LONG* 	RS_pnK2av; 		 //���η����ڼ�Ȩϵ�� K2av
extern LONG* 	RS_pnKma; 		 //��ѹ��A��ƽ��ϵ�� Kma
extern LONG* 	RS_pnKmb; 		 //��ѹ��B��ƽ��ϵ�� Kmb
extern LONG* 	RS_pnKmc; 		 //��ѹ��C��ƽ��ϵ�� Kmc
extern LONG* 	RS_pnKla; 		 //��ѹ��A��ƽ��ϵ�� Kla
extern LONG* 	RS_pnKlb; 		 //��ѹ��B��ƽ��ϵ�� Klb
extern LONG* 	RS_pnKlc; 		 //��ѹ��C��ƽ��ϵ�� Klc
extern LONG* 	RS_pnK87AR; 		 //����Խ�ޱ������� K87AR
extern LONG* 	RS_pn50IH1C; 		 //��ѹ�������ε��� 50IH1C
extern LONG* 	RS_pn50IH1D; 		 //��ѹ��������ʱ�� 50IH1D
extern LONG* 	RS_pn50IH2C; 		 //��ѹ�������ε��� 50IH2C
extern LONG* 	RS_pn50IH2D; 		 //��ѹ��������ʱ�� 50IH2D
extern LONG* 	RS_pn47U_27U1; 		 //�߹��������͵�ѹ 47U_27U1
extern LONG* 	RS_pn47U_59U2; 		 //�߹������������ѹ 47U_59U2
extern LONG* 	RS_pnA_50IH; 		 //��ѹ����������� A_50IH
extern LONG* 	RS_pn50IM1C; 		 //��ѹ�������ε��� 50IM1C
extern LONG* 	RS_pn50IM1D; 		 //��ѹ��������ʱ�� 50IM1D
extern LONG* 	RS_pn50IM2C; 		 //��ѹ�������ε��� 50IM2C
extern LONG* 	RS_pn50IM2D; 		 //��ѹ��������ʱ�� 50IM2D
extern LONG* 	RS_pn50IL1C; 		 //��ѹ�������ε��� 50IL1C
extern LONG* 	RS_pn50IL1D; 		 //��ѹ��������ʱ�� 50IL1D
extern LONG* 	RS_pn50IL2C; 		 //��ѹ�������ε��� 50IL2C
extern LONG* 	RS_pn50IL2D; 		 //��ѹ��������ʱ�� 50IL2D
extern LONG* 	RS_pn50OL1C; 		 //�����ɢ�ε��� 50OL1C
extern LONG* 	RS_pn50OL1D; 		 //�����ɢ��ʱ�� 50OL1D
extern LONG* 	RS_pn50OL2C; 		 //�����ɢ�ε��� 50OL2C
extern LONG* 	RS_pn50OL2D; 		 //�����ɢ��ʱ�� 50OL2D
extern LONG* 	RS_pn50OL3C; 		 //�����ɢ�ε��� 50OL3C
extern LONG* 	RS_pn50OL3D; 		 //�����ɢ��ʱ�� 50OL3D
extern LONG* 	RS_pn27UV; 		 //ʧѹ�������ѹ 27UV
extern LONG* 	RS_pn27UD; 		 //ʧѹ����ʱ�� 27UD
extern LONG* 	RS_pn27U_50I; 		 //ʧѹ������������ 27U_50I
extern LONG* 	RS_pn59UNV; 		 //�����ѹ��ѹ��ֵ 59UNV
extern LONG* 	RS_pn59UND; 		 //�����ѹʱ�䶨ֵ 59UND
extern LONG* 	RS_pn50INC; 		 //�������������ֵ 50INC
extern LONG* 	RS_pn50IND; 		 //�������ʱ�䶨ֵ 50IND
extern LONG* 	RS_pn50IJXC; 		 //��϶����������ֵ 50IJXC
extern LONG* 	RS_pn50IJXD; 		 //��϶����ʱ�䶨ֵ 50IJXD
extern LONG* 	RS_pn74VTV; 		 //PT���߼���ѹ 74VTV
extern LONG* 	RS_pn74VTC; 		 //PT���߼����� 74VTC
extern LONG* 	RS_pn74VTD; 		 //PT���߼��ʱ�� 74VTD
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD

#define RS_COUNT     71    //��ֵ����/
#define RS_BUF_SIZE     284    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 328)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnT1PI; 		 //T1������ʱ
extern LONG* 	AS_pnT1RE; 		 //T1������ʱ
extern LONG* 	AS_pnT2PI; 		 //T2������ʱ
extern LONG* 	AS_pnT2RE; 		 //T2������ʱ
extern LONG* 	AS_pnT3PI; 		 //T3������ʱ
extern LONG* 	AS_pnT3RE; 		 //T3������ʱ
extern LONG* 	AS_pnT4PI; 		 //T4������ʱ
extern LONG* 	AS_pnT4RE; 		 //T4������ʱ
extern LONG* 	AS_pnT5PI; 		 //T5������ʱ
extern LONG* 	AS_pnT5RE; 		 //T5������ʱ
extern LONG* 	AS_pnT6PI; 		 //T6������ʱ
extern LONG* 	AS_pnT6RE; 		 //T6������ʱ
extern LONG* 	AS_pnT7PI; 		 //T7������ʱ
extern LONG* 	AS_pnT7RE; 		 //T7������ʱ
extern LONG* 	AS_pnT8PI; 		 //T8������ʱ
extern LONG* 	AS_pnT8RE; 		 //T8������ʱ
extern LONG* 	AS_pnT9PI; 		 //T9������ʱ
extern LONG* 	AS_pnT9RE; 		 //T9������ʱ
extern LONG* 	AS_pnT10PI; 		 //T10������ʱ
extern LONG* 	AS_pnT10RE; 		 //T10������ʱ
extern LONG* 	AS_pnT11PI; 		 //T11������ʱ
extern LONG* 	AS_pnT11RE; 		 //T11������ʱ
extern LONG* 	AS_pnT12PI; 		 //T12������ʱ
extern LONG* 	AS_pnT12RE; 		 //T12������ʱ
extern LONG* 	AS_pnT13PI; 		 //T13������ʱ
extern LONG* 	AS_pnT13RE; 		 //T13������ʱ
extern LONG* 	AS_pnT14PI; 		 //T14������ʱ
extern LONG* 	AS_pnT14RE; 		 //T14������ʱ
extern LONG* 	AS_pnT15PI; 		 //T15������ʱ
extern LONG* 	AS_pnT15RE; 		 //T15������ʱ
extern LONG* 	AS_pnT16PI; 		 //T16������ʱ
extern LONG* 	AS_pnT16RE; 		 //T16������ʱ
extern LONG* 	AS_pn59UA; 		 //UA��ѹ��ֵ
extern LONG* 	AS_pn27UA; 		 //UA��ѹ��ֵ
extern LONG* 	AS_pn59UB; 		 //UB��ѹ��ֵ
extern LONG* 	AS_pn27UB; 		 //UB��ѹ��ֵ
extern LONG* 	AS_pn59UC; 		 //UC��ѹ��ֵ
extern LONG* 	AS_pn27UC; 		 //UC��ѹ��ֵ
extern LONG* 	AS_pn59UN; 		 //UN��ѹ��ֵ
extern LONG* 	AS_pn27UN; 		 //UN��ѹ��ֵ
extern LONG* 	AS_pn50HAC; 		 //IHA������ֵ
extern LONG* 	AS_pn37HAC; 		 //IHA������ֵ
extern LONG* 	AS_pn50HBC; 		 //IHB������ֵ
extern LONG* 	AS_pn37HBC; 		 //IHB������ֵ
extern LONG* 	AS_pn50HCC; 		 //IHC������ֵ
extern LONG* 	AS_pn37HCC; 		 //IHC������ֵ
extern LONG* 	AS_pn50MAC; 		 //IMA������ֵ
extern LONG* 	AS_pn37MAC; 		 //IMA������ֵ
extern LONG* 	AS_pn50MBC; 		 //IMB������ֵ
extern LONG* 	AS_pn37MBC; 		 //IMB������ֵ
extern LONG* 	AS_pn50MCC; 		 //IMC������ֵ
extern LONG* 	AS_pn37MCC; 		 //IMC������ֵ
extern LONG* 	AS_pn50LAC; 		 //ILA������ֵ
extern LONG* 	AS_pn37LAC; 		 //ILA������ֵ
extern LONG* 	AS_pn50LBC; 		 //ILB������ֵ
extern LONG* 	AS_pn37LBC; 		 //ILB������ֵ
extern LONG* 	AS_pn50LCC; 		 //ILC������ֵ
extern LONG* 	AS_pn37LCC; 		 //ILC������ֵ
extern LONG* 	AS_pn50INC; 		 //IN������ֵ
extern LONG* 	AS_pn37INC; 		 //IN������ֵ
extern LONG* 	AS_pn50IJXC; 		 //IJX������ֵ
extern LONG* 	AS_pn37IJXC; 		 //IJX������ֵ
extern LONG* 	AS_pnTCB; 		 //��·��������ʱ
extern LONG* 	AS_pnTSW; 		 //����������ʱ
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnTR3D; 		 //TR3����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnHPT; 		 //��ѹ��PT���
extern LONG* 	AS_pnNPT; 		 //����PT���
extern LONG* 	AS_pnHACT; 		 //��ѹ��A��CT���
extern LONG* 	AS_pnHBCT; 		 //��ѹ��B��CT���
extern LONG* 	AS_pnHCCT; 		 //��ѹ��C��CT���
extern LONG* 	AS_pnMACT; 		 //��ѹ��A��CT���
extern LONG* 	AS_pnMBCT; 		 //��ѹ��B��CT���
extern LONG* 	AS_pnMCCT; 		 //��ѹ��C��CT���
extern LONG* 	AS_pnLACT; 		 //��ѹ��A��CT���
extern LONG* 	AS_pnLBCT; 		 //��ѹ��B��CT���
extern LONG* 	AS_pnLCCT; 		 //��ѹ��C��CT���
extern LONG* 	AS_pnNCT; 		 //����CT���
extern LONG* 	AS_pnJXCT; 		 //��϶CT���
extern LONG* 	AS_pnRT; 		 //�̶����

#define AS_RT_NUM     14    //������ֵ�б�ȸ���

#define AS_COUNT     83    //������ֵ����
#define AS_BUF_SIZE     332    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 612)

//ϵ��----------------------------------
extern LONG* 	CF_pnUp; 		 //Up
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnIp; 		 //Ip
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnIjx; 		 //Ijx
extern LONG* 	CF_pnUAM; 		 //UAM
extern LONG* 	CF_pnUBM; 		 //UBM
extern LONG* 	CF_pnUCM; 		 //UCM
extern LONG* 	CF_pnU0M; 		 //U0M
extern LONG* 	CF_pnIHAM; 		 //IHAM
extern LONG* 	CF_pnIHBM; 		 //IHBM
extern LONG* 	CF_pnIHCM; 		 //IHCM
extern LONG* 	CF_pnIMAM; 		 //IMAM
extern LONG* 	CF_pnIMBM; 		 //IMBM
extern LONG* 	CF_pnIMCM; 		 //IMCM
extern LONG* 	CF_pnILAM; 		 //ILAM
extern LONG* 	CF_pnILBM; 		 //ILBM
extern LONG* 	CF_pnILCM; 		 //ILCM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnIjxM; 		 //IjxM
extern LONG* 	CF_pnUAD; 		 //UAD
extern LONG* 	CF_pnUBD; 		 //UBD
extern LONG* 	CF_pnUCD; 		 //UCD
extern LONG* 	CF_pnU0D; 		 //U0D
extern LONG* 	CF_pnIHAD; 		 //IHAD
extern LONG* 	CF_pnIHBD; 		 //IHBD
extern LONG* 	CF_pnIHCD; 		 //IHCD
extern LONG* 	CF_pnIMAD; 		 //IMAD
extern LONG* 	CF_pnIMBD; 		 //IMBD
extern LONG* 	CF_pnIMCD; 		 //IMCD
extern LONG* 	CF_pnILAD; 		 //ILAD
extern LONG* 	CF_pnILBD; 		 //ILBD
extern LONG* 	CF_pnILCD; 		 //ILCD
extern LONG* 	CF_pnI0D; 		 //I0D
extern LONG* 	CF_pnIjxD; 		 //IjxD
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4

#define CF_COUNT     39    //ϵ������
#define CF_BUF_SIZE     156    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 944)

//������ʺ�-----------------------
#define R_OEERR                     0    //�������ʧЧ
#define R_OEBR                      1    //�����������
#define R_ADERR                     2    //AD����
#define R_SLERR                     3    //��ѹ�����
#define R_EEPROMERR                 4    //EEPROM��ֵ����
#define R_RAMERR                    5    //RAM��ֵ����
#define R_ROMERR                    6    //����ROM����
#define R_RESETAN                   7    //װ���쳣��λ
#define R_COMERR                    8    //ͨ��ͨ���쳣
#define R_NVRAMERR                  9    //NVRAM���ݳ���
#define R_LOGICERR                 10    //�߼����̹���
#define R_RCERR                    11    //�����Լ����
#define R_POWERERR                 12    //5V��Դ����
#define R_EEPROM_W_ERR             13    //EEPROMд�����
#define R_RELDATAERR               14    //���������쳣
#define R_ADERR_R                  15    //AD���ϻָ�
#define R_SLERR_R                  16    //��ѹ����ϻָ�
#define R_EEPROMERR_R              17    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 18    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 19    //����ROM���ϻָ�
#define R_COMERR_R                 20    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               21    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               22    //�߼����̹��Ϲ��ϻָ�
#define R_POWERERR_R               23    //5V��Դ���ϻָ�
#define R_RELDATAERR_R             24    //���������쳣�ָ�
#define R_ACTPICK                  25    //��������
#define R_RTR_GAS                  26    //����˹
#define R_RTR_YLSF                 27    //ѹ���ͷ�
#define R_RTR_23                   28    //������բ
#define R_R74_GAS                  29    //����˹
#define R_R74_OIL                  30    //��λ�쳣
#define R_R74_23                   31    //�¶ȹ���
#define R_AR87R                    32    //����Խ��
#define R_R87UA_PI                 33    //A���ٶ�����
#define R_R87UA                    34    //A������ٶ϶���
#define R_R87UA_RES                35    //A���ٶϷ���
#define R_R87UB_PI                 36    //B���ٶ�����
#define R_R87UB                    37    //B���ٶ϶���
#define R_R87UB_RES                38    //B���ٶϷ���
#define R_R87UC_PI                 39    //C���ٶ�����
#define R_R87UC                    40    //C���ٶ϶���
#define R_R87UC_RES                41    //C���ٶϷ���
#define R_R87RA_PI                 42    //A����ʲ����
#define R_R87RA                    43    //A����ʲ����
#define R_R87RA_RES                44    //A����ʲ����
#define R_R87RB_PI                 45    //B����ʲ����
#define R_R87RB                    46    //B����ʲ����
#define R_R87RB_RES                47    //B����ʲ����
#define R_R87RC_PI                 48    //C����ʲ����
#define R_R87RC                    49    //C����ʲ����
#define R_R87RC_RES                50    //C����ʲ����
#define R_R50IH1_PI                51    //��ѹ������������
#define R_R50IH1                   52    //��ѹ�������ζ���
#define R_R50IH1_RES               53    //��ѹ�������η���
#define R_R50IH2_PI                54    //��ѹ������������
#define R_R50IH2                   55    //��ѹ�������ζ���
#define R_R50IH2_RES               56    //��ѹ�������η���
#define R_R50IM1_PI                57    //��ѹ������������
#define R_R50IM1                   58    //��ѹ�������ζ���
#define R_R50IM1_RES               59    //��ѹ�������η���
#define R_R50IM2_PI                60    //��ѹ������������
#define R_R50IM2                   61    //��ѹ�������ζ���
#define R_R50IM2_RES               62    //��ѹ�������η���
#define R_R50IL1_PI                63    //��ѹ������������
#define R_R50IL1                   64    //��ѹ�������ζ���
#define R_R50IL1_RES               65    //��ѹ�������η���
#define R_R50IL2_PI                66    //��ѹ������������
#define R_R50IL2                   67    //��ѹ�������ζ���
#define R_R50IL2_RES               68    //��ѹ�������η���
#define R_R50OL1_PI                69    //�����ɢ������
#define R_R50OL1                   70    //�����ɢ�ζ���
#define R_R50OL1_RES               71    //�����ɢ�η���
#define R_R50OL2_PI                72    //�����ɢ������
#define R_R50OL2                   73    //�����ɢ�ζ���
#define R_R50OL2_RES               74    //�����ɢ�η���
#define R_R50OL3_PI                75    //�����ɢ������
#define R_R50OL3                   76    //�����ɢ�ζ���
#define R_R50OL3_RES               77    //�����ɢ�η���
#define R_R27U                     78    //ʧѹ��������
#define R_R74VT                    79    //��ѹ��PT����
#define R_COMTEST                  80    //ͨ������
#define R_PICKUP                   81    //��������
#define R_52BF                     82    //ʧ�鱣��
#define R_TRFAIL_H                 83    //�߲��·����բʧ��
#define R_TRFAIL_M                 84    //�в��·����բʧ��
#define R_TRFAIL_L                 85    //�Ͳ��·����բʧ��
#define R_R59UN_PI                 86    //�����ѹ��������
#define R_R59UN                    87    //�����ѹ��������
#define R_R59UN_RES                88    //�����ѹ��������
#define R_R50IN_PI                 89    //���������������
#define R_R50IN                    90    //���������������
#define R_R50IN_RES                91    //���������������
#define R_R50IJX_PI                92    //��϶������������
#define R_R50IJX                   93    //��϶������������
#define R_R50IJX_RES               94    //��϶������������
#define MAX_REPORT_INDEX           94    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     21    //��������

#endif