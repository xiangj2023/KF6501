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
//----��ѹ������-----
#define 	YD11_T 	0	//Y/D-11ǣ��
#define 	YD11_E 	1	//Y/D-11����
#define 	YV_T 	2	//Y/Vƽ���
#define 	YA_T 	3	//Y/Aƽ���
#define 	SCOTT 	4	//SCOTT��
#define 	VV3_T 	5	//����V/V��
#define 	VV1_T 	6	//����V/V��
#define 	SIGNP_T 	7	//�����
#define 	VX_T 	8	//V/X��ѹ��
//----�߲�CT����-----
#define 	CT_Y 	0	//�ǽ�
#define 	CT_D 	1	//�ǽ�
//----VV�������-----
#define 	APHASE 	0	//A�����
#define 	BPHASE 	1	//B�����
#define 	CPHASE 	2	//C�����

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//��������ֵ����-----------------------
#define  RM_IHA 0
extern LONG* 	RM_pnIHA; 		 // IA
#define  RM_IHB 1
extern LONG* 	RM_pnIHB; 		 // IB
#define  RM_IHC 2
extern LONG* 	RM_pnIHC; 		 // IC
#define  RM_IL1 3
extern LONG* 	RM_pnIL1; 		 // I1
#define  RM_IL2 4
extern LONG* 	RM_pnIL2; 		 // I2
#define  RM_IL3 5
extern LONG* 	RM_pnIL3; 		 // I3
#define  RM_IL4 6
extern LONG* 	RM_pnIL4; 		 // I4
#define  RM_IDA 7
extern LONG* 	RM_pnIDA; 		 // IDA
#define  RM_IDB 8
extern LONG* 	RM_pnIDB; 		 // IDB
#define  RM_IDC 9
extern LONG* 	RM_pnIDC; 		 // IDC
#define  RM_IRA 10
extern LONG* 	RM_pnIRA; 		 // IRA
#define  RM_IRB 11
extern LONG* 	RM_pnIRB; 		 // IRB
#define  RM_IRC 12
extern LONG* 	RM_pnIRC; 		 // IRC
#define  RM_IDA2 13
extern LONG* 	RM_pnIDA2; 		 // IDA2
#define  RM_IDA3 14
extern LONG* 	RM_pnIDA3; 		 // IDA3
#define  RM_IDA5 15
extern LONG* 	RM_pnIDA5; 		 // IDA5
#define  RM_IDB2 16
extern LONG* 	RM_pnIDB2; 		 // IDB2
#define  RM_IDB3 17
extern LONG* 	RM_pnIDB3; 		 // IDB3
#define  RM_IDB5 18
extern LONG* 	RM_pnIDB5; 		 // IDB5
#define  RM_IDC2 19
extern LONG* 	RM_pnIDC2; 		 // IDC2
#define  RM_IDC3 20
extern LONG* 	RM_pnIDC3; 		 // IDC3
#define  RM_IDC5 21
extern LONG* 	RM_pnIDC5; 		 // IDC5
#define  RM_IRA2 22
extern LONG* 	RM_pnIRA2; 		 // IRA2
#define  RM_IRB2 23
extern LONG* 	RM_pnIRB2; 		 // IRB2
#define  RM_IRC2 24
extern LONG* 	RM_pnIRC2; 		 // IRC2
#define  RM_AIHA 25
extern LONG* 	RM_pnAIHA; 		 // ��IA
#define  RM_AIHB 26
extern LONG* 	RM_pnAIHB; 		 // ��IB
#define  RM_AIHC 27
extern LONG* 	RM_pnAIHC; 		 // ��IC
#define  RM_AIL1 28
extern LONG* 	RM_pnAIL1; 		 // ��I1
#define  RM_AIL2 29
extern LONG* 	RM_pnAIL2; 		 // ��I2
#define  RM_AIL3 30
extern LONG* 	RM_pnAIL3; 		 // ��I3
#define  RM_AIL4 31
extern LONG* 	RM_pnAIL4; 		 // ��I4
#define  RM_UA 32
extern LONG* 	RM_pnUA; 		 // UA
#define  RM_UB 33
extern LONG* 	RM_pnUB; 		 // UB
#define  RM_UC 34
extern LONG* 	RM_pnUC; 		 // UC
#define  RM_BOBRCH1 35
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 36
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 37
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 38
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     39    //��������ֵ����
#define RM_BUF_SIZE     156    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UA 0
extern LONG* 	M_pnUA; 		 //UA UA
#define  M_UB 1
extern LONG* 	M_pnUB; 		 //UB UB
#define  M_UC 2
extern LONG* 	M_pnUC; 		 //UC UC
#define  M_IHAM 3
extern LONG* 	M_pnIHAM; 		 //IHAM IHAM
#define  M_IHBM 4
extern LONG* 	M_pnIHBM; 		 //IHBM IHBM
#define  M_IHCM 5
extern LONG* 	M_pnIHCM; 		 //IHCM IHCM
#define  M_IL1M 6
extern LONG* 	M_pnIL1M; 		 //IL1M IL1M
#define  M_IL2M 7
extern LONG* 	M_pnIL2M; 		 //IL2M IL2M
#define  M_IL3M 8
extern LONG* 	M_pnIL3M; 		 //IL3M IL3M
#define  M_IL4M 9
extern LONG* 	M_pnIL4M; 		 //IL4M IL4M

#define MEASURE_COUNT     10    //����ֵ����
#define MEASURE_BUF_SIZE     40    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 160)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby87U; 		 //������ٶ�
extern BYTE* 	SW_pby87R; 		 //���ʲ����
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     5    //��ѹ�����
#define SW_BUF_SIZE     5    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 200)

//����----------------------------------
extern BYTE* 	CFG_pby87U_A; 		 //A������ٶϱ���
extern BYTE* 	CFG_pby87U_B; 		 //B������ٶϱ���
extern BYTE* 	CFG_pby87U_C; 		 //C������ٶϱ���
extern BYTE* 	CFG_pby87R_A; 		 //A����ʲ����
extern BYTE* 	CFG_pby87R_B; 		 //B����ʲ����
extern BYTE* 	CFG_pby87R_C; 		 //C����ʲ����
extern BYTE* 	CFG_pby68IdH2M_87R; 		 //����������������
extern BYTE* 	CFG_pby68IdH2C_87R; 		 //���������ۺ������
extern BYTE* 	CFG_pby68IdH2I_87R; 		 //�������η������
extern BYTE* 	CFG_pby68IrH2I_87R; 		 //�ƶ����α���
extern BYTE* 	CFG_pby68IdH5I_87R; 		 //������α���
extern BYTE* 	CFG_pby68IdHMUL_87R; 		 //�����ۺ�г������
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     15    //���ø���/
#define CFG_BUF_SIZE     15    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 206)

//��ֵ----------------------------------
extern LONG* 	RS_pnCFG1; 		 //������ CFG1
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pnTRTY; 		 //��ѹ������ TRTY
extern LONG* 	RS_pnLINUM; 		 //��ѹ��������� LINUM
extern LONG* 	RS_pnCT1CON; 		 //�߲�CT���� CT1CON
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
extern LONG* 	RS_pnKrh2; 		 //�����ƶ�ϵ�� Krh2
extern LONG* 	RS_pnKrh5; 		 //����ƶ�ϵ�� Krh5
extern LONG* 	RS_pnKrh; 		 //�ۺ�г���ƶ�ϵ�� Krh
extern LONG* 	RS_pnKrav; 		 //������ڼ�Ȩ Krav
extern LONG* 	RS_pnK2av; 		 //���η����ڼ�Ȩ K2av
extern LONG* 	RS_pnKba; 		 //a��ƽ��ϵ�� Kba
extern LONG* 	RS_pnKbb; 		 //b��ƽ��ϵ�� Kbb
extern LONG* 	RS_pnKbc; 		 //c��ƽ��ϵ�� Kbc
extern LONG* 	RS_pnK87AR; 		 //����Խ�ޱ������� K87AR
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD
extern LONG* 	RS_pnBph; 		 //VV������� Bph

#define RS_COUNT     30    //��ֵ����/
#define RS_BUF_SIZE     120    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 224)

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
extern LONG* 	AS_pn50HAC; 		 //IHA������ֵ
extern LONG* 	AS_pn37HAC; 		 //IHA������ֵ
extern LONG* 	AS_pn50HBC; 		 //IHB������ֵ
extern LONG* 	AS_pn37HBC; 		 //IHB������ֵ
extern LONG* 	AS_pn50HCC; 		 //IHC������ֵ
extern LONG* 	AS_pn37HCC; 		 //IHC������ֵ
extern LONG* 	AS_pn50L1C; 		 //IL1������ֵ
extern LONG* 	AS_pn37L1C; 		 //IL1������ֵ
extern LONG* 	AS_pn50L2C; 		 //IL2������ֵ
extern LONG* 	AS_pn37L2C; 		 //IL2������ֵ
extern LONG* 	AS_pn50L3C; 		 //IL3������ֵ
extern LONG* 	AS_pn37L3C; 		 //IL3������ֵ
extern LONG* 	AS_pn50L4C; 		 //IL4������ֵ
extern LONG* 	AS_pn37L4C; 		 //IL4������ֵ
extern LONG* 	AS_pnTCB; 		 //��·��������ʱ
extern LONG* 	AS_pnTSW; 		 //����������ʱ
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnTR3D; 		 //TR3����ʱ��
extern LONG* 	AS_pn59UA; 		 //UA��ѹ��ֵ
extern LONG* 	AS_pn27UA; 		 //UA��ѹ��ֵ
extern LONG* 	AS_pn59UB; 		 //UB��ѹ��ֵ
extern LONG* 	AS_pn27UB; 		 //UB��ѹ��ֵ
extern LONG* 	AS_pn59UC; 		 //UC��ѹ��ֵ
extern LONG* 	AS_pn27UC; 		 //UC��ѹ��ֵ
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnHACT; 		 //��ѹ��A��CT���
extern LONG* 	AS_pnHBCT; 		 //��ѹ��B��CT���
extern LONG* 	AS_pnHCCT; 		 //��ѹ��C��CT���
extern LONG* 	AS_pnL1CT; 		 //��ѹ��CT1���
extern LONG* 	AS_pnL2CT; 		 //��ѹ��CT2���
extern LONG* 	AS_pnL3CT; 		 //��ѹ��CT3���
extern LONG* 	AS_pnL4CT; 		 //��ѹ��CT4���
extern LONG* 	AS_pnHPT; 		 //��ѹ��PT���
extern LONG* 	AS_pnRT; 		 //�̶����

#define AS_RT_NUM     9    //������ֵ�б�ȸ���

#define AS_COUNT     68    //������ֵ����
#define AS_BUF_SIZE     272    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 344)

//ϵ��----------------------------------
extern LONG* 	CF_pnIA; 		 //IA
extern LONG* 	CF_pnIB; 		 //IB
extern LONG* 	CF_pnIC; 		 //IC
extern LONG* 	CF_pnI1; 		 //I1
extern LONG* 	CF_pnI2; 		 //I2
extern LONG* 	CF_pnI3; 		 //I3
extern LONG* 	CF_pnI4; 		 //I4
extern LONG* 	CF_pnIDA; 		 //IDA
extern LONG* 	CF_pnIDB; 		 //IDB
extern LONG* 	CF_pnIDC; 		 //IDC
extern LONG* 	CF_pnIRA; 		 //IRA
extern LONG* 	CF_pnIRB; 		 //IRB
extern LONG* 	CF_pnIRC; 		 //IRC
extern LONG* 	CF_pnU; 		 //U
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnUAM; 		 //UAM
extern LONG* 	CF_pnUBM; 		 //UBM
extern LONG* 	CF_pnUCM; 		 //UCM
extern LONG* 	CF_pnIAM; 		 //IAM
extern LONG* 	CF_pnIBM; 		 //IBM
extern LONG* 	CF_pnICM; 		 //ICM
extern LONG* 	CF_pnI1M; 		 //I1M
extern LONG* 	CF_pnI2M; 		 //I2M
extern LONG* 	CF_pnI3M; 		 //I3M
extern LONG* 	CF_pnI4M; 		 //I4M
extern LONG* 	CF_pnUAD; 		 //UAD
extern LONG* 	CF_pnUBD; 		 //UBD
extern LONG* 	CF_pnUCD; 		 //UCD
extern LONG* 	CF_pnIAD; 		 //IAD
extern LONG* 	CF_pnIBD; 		 //IBD
extern LONG* 	CF_pnICD; 		 //ICD
extern LONG* 	CF_pnI1D; 		 //I1D
extern LONG* 	CF_pnI2D; 		 //I2D
extern LONG* 	CF_pnI3D; 		 //I3D
extern LONG* 	CF_pnI4D; 		 //I4D

#define CF_COUNT     38    //ϵ������
#define CF_BUF_SIZE     152    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 616)

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
#define R_ADERR_R                  14    //AD���ϻָ�
#define R_SLERR_R                  15    //��ѹ����ϻָ�
#define R_EEPROMERR_R              16    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 17    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 18    //����ROM���ϻָ�
#define R_COMERR_R                 19    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               20    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               21    //�߼����̹��Ϲ��ϻָ�
#define R_POWERERR_R               22    //5V��Դ���ϻָ�
#define R_ACTPICK                  23    //��������
#define R_RTR_GAS                  24    //����˹
#define R_RTR_YLSF                 25    //ѹ���ͷ�
#define R_RTR_23                   26    //������բ
#define R_R74_GAS                  27    //����˹
#define R_R74_OIL                  28    //��λ�쳣
#define R_R74_23                   29    //�¶ȹ���
#define R_RLP                      30    //ʧ��澯
#define R_RTR_GAS_1                31    //����˹1
#define R_RTR_YLSF_1               32    //ѹ���ͷ�1
#define R_RTR_23_1                 33    //������բ1
#define R_R74_GAS_1                34    //����˹1
#define R_R74_OIL_1                35    //��λ�쳣1
#define R_R74_23_1                 36    //�¶ȹ���1
#define R_RLP_1                    37    //ʧ��澯1
#define R_RTR_GAS_2                38    //����˹2
#define R_RTR_YLSF_2               39    //ѹ���ͷ�2
#define R_RTR_23_2                 40    //������բ2
#define R_R74_GAS_2                41    //����˹2
#define R_R74_OIL_2                42    //��λ�쳣2
#define R_R74_23_2                 43    //�¶ȹ���2
#define R_RLP_2                    44    //ʧ��澯2
#define R_AR87R                    45    //����Խ��
#define R_R87UA_PI                 46    //A���ٶ�����
#define R_R87UA                    47    //A������ٶ϶���
#define R_R87UA_RES                48    //A���ٶϷ���
#define R_R87UB_PI                 49    //B���ٶ�����
#define R_R87UB                    50    //B���ٶ϶���
#define R_R87UB_RES                51    //B���ٶϷ���
#define R_R87UC_PI                 52    //C���ٶ�����
#define R_R87UC                    53    //C���ٶ϶���
#define R_R87UC_RES                54    //C���ٶϷ���
#define R_R87RA_PI                 55    //A����ʲ����
#define R_R87RA                    56    //A����ʲ����
#define R_R87RA_RES                57    //A����ʲ����
#define R_R87RB_PI                 58    //B����ʲ����
#define R_R87RB                    59    //B����ʲ����
#define R_R87RB_RES                60    //B����ʲ����
#define R_R87RC_PI                 61    //C����ʲ����
#define R_R87RC                    62    //C����ʲ����
#define R_R87RC_RES                63    //C����ʲ����
#define R_COMTEST                  64    //ͨ������
#define R_PICKUP                   65    //��������
#define R_TRFAIL                   66    //��·����բʧ��
#define R_52BF                     67    //ʧ�鱣��
#define R_AR87R_RES                68    //����Խ�޷���
#define R_RELDATAERR               69    //���������쳣
#define R_RELDATAERR_R             70    //���������쳣�ָ�
#define MAX_REPORT_INDEX           70    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     15    //��������

#endif