/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Relaymem.c
* 
*   ���ģ�飺�����ӿ��ļ�
*
* 
*	����	���������ݶ��壬�洢���䣬 �°벿���ɹ�������Զ�����
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
* 
*************************************************************************/



#include "RelayDef.h"
#include "Logicdef.h"
#include "Relay_defs.h"

#ifdef __PL_WIN_SIMULATION__
    BYTE _RAMDataBuf[32*1024];             //��windowģ��ƽ̨��ģ��
#endif

//���ڲ����� ��ʱ��ֵ �ļ̵����� ָ��ֵΪ0�ĳ����� ������δ����ʹ����ͬ����ʽ
const LONG NoTimeAddr[1] = {0};

//���´����ɹ����������




//��ǰ��ֵ���Ŷ���-----------------------
WORD* 	G_pwCurAreaNo 	= 	(WORD*)(RELAY_RAM_ADDR_BASE + 0); 	//��ǰ��ֵ����
//��ֵȡֵ��Ϣ
const TSetValueInf SetValueInf[]=
{
	{YD11_T, "Y/D-11ǣ��"},
	{YD11_E, "Y/D-11����"},
	{YV_T, "Y/Vƽ���"},
	{YA_T, "Y/Aƽ���"},
	{SCOTT, "SCOTT��"},
	{VV3_T, "����V/V��"},
	{VV1_T, "����V/V��"},
	{SIGNP_T, "�����"},
	{VX_T, "V/X��ѹ��"},
	{CT_Y, "�ǽ�"},
	{CT_D, "�ǽ�"},
	{APHASE, "A�����"},
	{BPHASE, "B�����"},
	{CPHASE, "C�����"},
	{-1, NULL},
};
const TSetValueInfMap SetValueInfMap[]=
{
	{0, NULL},
	{0, NULL},
	{9, (TSetValueInf*)&(SetValueInf[0])}, //��ֵ��ѹ������
	{0, NULL},
	{2, (TSetValueInf*)&(SetValueInf[9])}, //��ֵ�߲�CT����
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{3, (TSetValueInf*)&(SetValueInf[11])}, //��ֵVV�������
	{0, NULL},
};
//������ֵȡֵ��Ϣ
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //������ֵ�����
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
};

//��������ֵ����-----------------------
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// IA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// IB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// IC
LONG* 	RM_pnIL1 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// I1
LONG* 	RM_pnIL2 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// I2
LONG* 	RM_pnIL3 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// I3
LONG* 	RM_pnIL4 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// I4
LONG* 	RM_pnIDA 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// IDA
LONG* 	RM_pnIDB 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// IDB
LONG* 	RM_pnIDC 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// IDC
LONG* 	RM_pnIRA 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// IRA
LONG* 	RM_pnIRB 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// IRB
LONG* 	RM_pnIRC 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IRC
LONG* 	RM_pnIDA2 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// IDA2
LONG* 	RM_pnIDA3 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// IDA3
LONG* 	RM_pnIDA5 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// IDA5
LONG* 	RM_pnIDB2 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// IDB2
LONG* 	RM_pnIDB3 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IDB3
LONG* 	RM_pnIDB5 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// IDB5
LONG* 	RM_pnIDC2 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// IDC2
LONG* 	RM_pnIDC3 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IDC3
LONG* 	RM_pnIDC5 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// IDC5
LONG* 	RM_pnIRA2 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// IRA2
LONG* 	RM_pnIRB2 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// IRB2
LONG* 	RM_pnIRC2 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// IRC2
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// ��IA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// ��IB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// ��IC
LONG* 	RM_pnAIL1 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// ��I1
LONG* 	RM_pnAIL2 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// ��I2
LONG* 	RM_pnAIL3 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// ��I3
LONG* 	RM_pnAIL4 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// ��I4
LONG* 	RM_pnUA 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// UA
LONG* 	RM_pnUB 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// UB
LONG* 	RM_pnUC 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// UC
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnIHAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//IHAM IHAM
LONG* 	M_pnIHBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//IHBM IHBM
LONG* 	M_pnIHCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//IHCM IHCM
LONG* 	M_pnIL1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//IL1M IL1M
LONG* 	M_pnIL2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IL2M IL2M
LONG* 	M_pnIL3M 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IL3M IL3M
LONG* 	M_pnIL4M 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IL4M IL4M


//��ѹ��----------------------------------
BYTE* SW_pby87U 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//������ٶ�
BYTE* SW_pby87R 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//���ʲ����
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby87U_A 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//A������ٶϱ���
BYTE* CFG_pby87U_B 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//B������ٶϱ���
BYTE* CFG_pby87U_C 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//C������ٶϱ���
BYTE* CFG_pby87R_A 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//A����ʲ����
BYTE* CFG_pby87R_B 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//B����ʲ����
BYTE* CFG_pby87R_C 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//C����ʲ����
BYTE* CFG_pby68IdH2M_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//����������������
BYTE* CFG_pby68IdH2C_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//���������ۺ������
BYTE* CFG_pby68IdH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//�������η������
BYTE* CFG_pby68IrH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//�ƶ����α���
BYTE* CFG_pby68IdH5I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//������α���
BYTE* CFG_pby68IdHMUL_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//�����ۺ�г������
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnCFG1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ CFG1
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//ͻ������������ PIC
LONG* RS_pnTRTY 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//��ѹ������ TRTY
LONG* RS_pnLINUM 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//��ѹ��������� LINUM
LONG* RS_pnCT1CON 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//�߲�CT���� CT1CON
LONG* RS_pn87UIA 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//A���ٶ϶�ֵ 87UIA
LONG* RS_pn87RIA 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//A��������ֵ 87RIA
LONG* RS_pnIRS1A 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//A���ƶ��յ�1 IRS1A
LONG* RS_pnIRS2A 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//A���ƶ��յ�2 IRS2A
LONG* RS_pn87UIB 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//B���ٶ϶�ֵ 87UIB
LONG* RS_pn87RIB 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//B��������ֵ 87RIB
LONG* RS_pnIRS1B 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//B���ƶ��յ�1 IRS1B
LONG* RS_pnIRS2B 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//B���ƶ��յ�2 IRS2B
LONG* RS_pn87UIC 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//C���ٶ϶�ֵ 87UIC
LONG* RS_pn87RIC 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//C��������ֵ 87RIC
LONG* RS_pnIRS1C 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//C���ƶ��յ�1 IRS1C
LONG* RS_pnIRS2C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//C���ƶ��յ�2 IRS2C
LONG* RS_pnSLP1 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//�����ƶ�ϵ��1 SLP1
LONG* RS_pnSLP2 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//�����ƶ�ϵ��2 SLP2
LONG* RS_pnKrh2 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//�����ƶ�ϵ�� Krh2
LONG* RS_pnKrh5 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//����ƶ�ϵ�� Krh5
LONG* RS_pnKrh 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//�ۺ�г���ƶ�ϵ�� Krh
LONG* RS_pnKrav 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//������ڼ�Ȩ Krav
LONG* RS_pnK2av 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//���η����ڼ�Ȩ K2av
LONG* RS_pnKba 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//a��ƽ��ϵ�� Kba
LONG* RS_pnKbb 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//b��ƽ��ϵ�� Kbb
LONG* RS_pnKbc 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//c��ƽ��ϵ�� Kbc
LONG* RS_pnK87AR 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//����Խ�ޱ������� K87AR
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//ʧ�鱣��ʱ�� 52BFD
LONG* RS_pnBph 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//VV������� Bph

//������ֵ----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//�����
LONG* AS_pnT1PI 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//T1������ʱ
LONG* AS_pnT1RE 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//T1������ʱ
LONG* AS_pnT2PI 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//T2������ʱ
LONG* AS_pnT2RE 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//T2������ʱ
LONG* AS_pnT3PI 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//T3������ʱ
LONG* AS_pnT3RE 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//T3������ʱ
LONG* AS_pnT4PI 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//T4������ʱ
LONG* AS_pnT4RE 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//T4������ʱ
LONG* AS_pnT5PI 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//T5������ʱ
LONG* AS_pnT5RE 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//T5������ʱ
LONG* AS_pnT6PI 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//T6������ʱ
LONG* AS_pnT6RE 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//T6������ʱ
LONG* AS_pnT7PI 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//T7������ʱ
LONG* AS_pnT7RE 	= 	(LONG*)(AS_BUF_ADDR + 56); 	//T7������ʱ
LONG* AS_pnT8PI 	= 	(LONG*)(AS_BUF_ADDR + 60); 	//T8������ʱ
LONG* AS_pnT8RE 	= 	(LONG*)(AS_BUF_ADDR + 64); 	//T8������ʱ
LONG* AS_pnT9PI 	= 	(LONG*)(AS_BUF_ADDR + 68); 	//T9������ʱ
LONG* AS_pnT9RE 	= 	(LONG*)(AS_BUF_ADDR + 72); 	//T9������ʱ
LONG* AS_pnT10PI 	= 	(LONG*)(AS_BUF_ADDR + 76); 	//T10������ʱ
LONG* AS_pnT10RE 	= 	(LONG*)(AS_BUF_ADDR + 80); 	//T10������ʱ
LONG* AS_pnT11PI 	= 	(LONG*)(AS_BUF_ADDR + 84); 	//T11������ʱ
LONG* AS_pnT11RE 	= 	(LONG*)(AS_BUF_ADDR + 88); 	//T11������ʱ
LONG* AS_pnT12PI 	= 	(LONG*)(AS_BUF_ADDR + 92); 	//T12������ʱ
LONG* AS_pnT12RE 	= 	(LONG*)(AS_BUF_ADDR + 96); 	//T12������ʱ
LONG* AS_pnT13PI 	= 	(LONG*)(AS_BUF_ADDR + 100); 	//T13������ʱ
LONG* AS_pnT13RE 	= 	(LONG*)(AS_BUF_ADDR + 104); 	//T13������ʱ
LONG* AS_pnT14PI 	= 	(LONG*)(AS_BUF_ADDR + 108); 	//T14������ʱ
LONG* AS_pnT14RE 	= 	(LONG*)(AS_BUF_ADDR + 112); 	//T14������ʱ
LONG* AS_pnT15PI 	= 	(LONG*)(AS_BUF_ADDR + 116); 	//T15������ʱ
LONG* AS_pnT15RE 	= 	(LONG*)(AS_BUF_ADDR + 120); 	//T15������ʱ
LONG* AS_pnT16PI 	= 	(LONG*)(AS_BUF_ADDR + 124); 	//T16������ʱ
LONG* AS_pnT16RE 	= 	(LONG*)(AS_BUF_ADDR + 128); 	//T16������ʱ
LONG* AS_pn50HAC 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//IHA������ֵ
LONG* AS_pn37HAC 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//IHA������ֵ
LONG* AS_pn50HBC 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//IHB������ֵ
LONG* AS_pn37HBC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IHB������ֵ
LONG* AS_pn50HCC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IHC������ֵ
LONG* AS_pn37HCC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//IHC������ֵ
LONG* AS_pn50L1C 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IL1������ֵ
LONG* AS_pn37L1C 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//IL1������ֵ
LONG* AS_pn50L2C 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//IL2������ֵ
LONG* AS_pn37L2C 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//IL2������ֵ
LONG* AS_pn50L3C 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//IL3������ֵ
LONG* AS_pn37L3C 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//IL3������ֵ
LONG* AS_pn50L4C 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//IL4������ֵ
LONG* AS_pn37L4C 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//IL4������ֵ
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//��·��������ʱ
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//����������ʱ
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//TR2����ʱ��
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//TR3����ʱ��
LONG* AS_pn59UA 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//UA��ѹ��ֵ
LONG* AS_pn27UA 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//UA��ѹ��ֵ
LONG* AS_pn59UB 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//UB��ѹ��ֵ
LONG* AS_pn27UB 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//UB��ѹ��ֵ
LONG* AS_pn59UC 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//UC��ѹ��ֵ
LONG* AS_pn27UC 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//UC��ѹ��ֵ
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnHACT 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//��ѹ��A��CT���
LONG* AS_pnHBCT 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//��ѹ��B��CT���
LONG* AS_pnHCCT 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//��ѹ��C��CT���
LONG* AS_pnL1CT 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//��ѹ��CT1���
LONG* AS_pnL2CT 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//��ѹ��CT2���
LONG* AS_pnL3CT 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//��ѹ��CT3���
LONG* AS_pnL4CT 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//��ѹ��CT4���
LONG* AS_pnHPT 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//��ѹ��PT���
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//�̶����

//ϵ��----------------------------------
LONG* CF_pnIA 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//IA
LONG* CF_pnIB 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//IB
LONG* CF_pnIC 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//IC
LONG* CF_pnI1 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//I1
LONG* CF_pnI2 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//I2
LONG* CF_pnI3 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//I3
LONG* CF_pnI4 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//I4
LONG* CF_pnIDA 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//IDA
LONG* CF_pnIDB 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IDB
LONG* CF_pnIDC 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IDC
LONG* CF_pnIRA 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IRA
LONG* CF_pnIRB 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IRB
LONG* CF_pnIRC 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IRC
LONG* CF_pnU 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//U
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//C4
LONG* CF_pnUAM 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//UAM
LONG* CF_pnUBM 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//UBM
LONG* CF_pnUCM 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//UCM
LONG* CF_pnIAM 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//IAM
LONG* CF_pnIBM 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//IBM
LONG* CF_pnICM 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//ICM
LONG* CF_pnI1M 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//I1M
LONG* CF_pnI2M 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//I2M
LONG* CF_pnI3M 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//I3M
LONG* CF_pnI4M 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//I4M
LONG* CF_pnUAD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//UAD
LONG* CF_pnUBD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//UBD
LONG* CF_pnUCD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//UCD
LONG* CF_pnIAD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IAD
LONG* CF_pnIBD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IBD
LONG* CF_pnICD 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//ICD
LONG* CF_pnI1D 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//I1D
LONG* CF_pnI2D 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//I2D
LONG* CF_pnI3D 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//I3D
LONG* CF_pnI4D 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//I4D

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 148), // IN1
    (LONG*)(RM_BUF_ADDR + 152) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 140), // BR1
    (LONG*)(RM_BUF_ADDR + 144) // BR2
};

//AD����3----------
const  LONG* FailureInf3[] = {
    {NULL}
};

//��ѹ�����4----------
const  LONG* FailureInf4[] = {
    {NULL}
};

//EEPROM��ֵ����5----------
const  LONG* FailureInf5[] = {
    {NULL}
};

//RAM��ֵ����8----------
const  LONG* FailureInf8[] = {
    {NULL}
};

//����ROM����11----------
const  LONG* FailureInf11[] = {
    {NULL}
};

//װ���쳣��λ13----------
const  LONG* FailureInf13[] = {
    {NULL}
};

//ͨ��ͨ���쳣14----------
const  LONG* FailureInf14[] = {
    {NULL}
};

//NVRAM���ݳ���16----------
const  LONG* FailureInf16[] = {
    {NULL}
};

//�߼����̹���17----------
const  LONG* FailureInf17[] = {
    {NULL}
};

//�����Լ����18----------
const  LONG* FailureInf18[] = {
    {NULL}
};

//5V��Դ����20----------
const  LONG* FailureInf20[] = {
    {NULL}
};

//EEPROMд�����21----------
const  LONG* FailureInf21[] = {
    {NULL}
};

//AD���ϻָ�50----------
const  LONG* FailureInf50[] = {
    {NULL}
};

//��ѹ����ϻָ�51----------
const  LONG* FailureInf51[] = {
    {NULL}
};

//EEPROM��ֵ���ϻָ�52----------
const  LONG* FailureInf52[] = {
    {NULL}
};

//RAM��ֵ���ϻָ�55----------
const  LONG* FailureInf55[] = {
    {NULL}
};

//����ROM���ϻָ�58----------
const  LONG* FailureInf58[] = {
    {NULL}
};

//ͨ��ͨ���쳣�ָ�60----------
const  LONG* FailureInf60[] = {
    {NULL}
};

//NVRAM���ݳ���ָ�62----------
const  LONG* FailureInf62[] = {
    {NULL}
};

//�߼����̹��Ϲ��ϻָ�63----------
const  LONG* FailureInf63[] = {
    {NULL}
};

//5V��Դ���ϻָ�64----------
const  LONG* FailureInf64[] = {
    {NULL}
};

//��������500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//����˹501----------
const  LONG* FailureInf501[] = {
    {NULL}
};

//ѹ���ͷ�502----------
const  LONG* FailureInf502[] = {
    {NULL}
};

//������բ503----------
const  LONG* FailureInf503[] = {
    {NULL}
};

//����˹504----------
const  LONG* FailureInf504[] = {
    {NULL}
};

//��λ�쳣505----------
const  LONG* FailureInf505[] = {
    {NULL}
};

//�¶ȹ���506----------
const  LONG* FailureInf506[] = {
    {NULL}
};

//ʧ��澯507----------
const  LONG* FailureInf507[] = {
    {NULL}
};

//����˹1508----------
const  LONG* FailureInf508[] = {
    {NULL}
};

//ѹ���ͷ�1509----------
const  LONG* FailureInf509[] = {
    {NULL}
};

//������բ1510----------
const  LONG* FailureInf510[] = {
    {NULL}
};

//����˹1511----------
const  LONG* FailureInf511[] = {
    {NULL}
};

//��λ�쳣1512----------
const  LONG* FailureInf512[] = {
    {NULL}
};

//�¶ȹ���1513----------
const  LONG* FailureInf513[] = {
    {NULL}
};

//ʧ��澯1514----------
const  LONG* FailureInf514[] = {
    {NULL}
};

//����˹2515----------
const  LONG* FailureInf515[] = {
    {NULL}
};

//ѹ���ͷ�2516----------
const  LONG* FailureInf516[] = {
    {NULL}
};

//������բ2517----------
const  LONG* FailureInf517[] = {
    {NULL}
};

//����˹2518----------
const  LONG* FailureInf518[] = {
    {NULL}
};

//��λ�쳣2519----------
const  LONG* FailureInf519[] = {
    {NULL}
};

//�¶ȹ���2520----------
const  LONG* FailureInf520[] = {
    {NULL}
};

//ʧ��澯2521----------
const  LONG* FailureInf521[] = {
    {NULL}
};

//����Խ��1001----------
const  LONG* FailureInf1001[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//A���ٶ�����1002----------
const  LONG* FailureInf1002[] = {
    {NULL}
};

//A������ٶ϶���1003----------
const  LONG* FailureInf1003[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//A���ٶϷ���1004----------
const  LONG* FailureInf1004[] = {
    {NULL}
};

//B���ٶ�����1005----------
const  LONG* FailureInf1005[] = {
    {NULL}
};

//B���ٶ϶���1006----------
const  LONG* FailureInf1006[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//B���ٶϷ���1007----------
const  LONG* FailureInf1007[] = {
    {NULL}
};

//C���ٶ�����1008----------
const  LONG* FailureInf1008[] = {
    {NULL}
};

//C���ٶ϶���1009----------
const  LONG* FailureInf1009[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//C���ٶϷ���1010----------
const  LONG* FailureInf1010[] = {
    {NULL}
};

//A����ʲ����1011----------
const  LONG* FailureInf1011[] = {
    {NULL}
};

//A����ʲ����1012----------
const  LONG* FailureInf1012[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//A����ʲ����1013----------
const  LONG* FailureInf1013[] = {
    {NULL}
};

//B����ʲ����1014----------
const  LONG* FailureInf1014[] = {
    {NULL}
};

//B����ʲ����1015----------
const  LONG* FailureInf1015[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//B����ʲ����1016----------
const  LONG* FailureInf1016[] = {
    {NULL}
};

//C����ʲ����1017----------
const  LONG* FailureInf1017[] = {
    {NULL}
};

//C����ʲ����1018----------
const  LONG* FailureInf1018[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//C����ʲ����1019----------
const  LONG* FailureInf1019[] = {
    {NULL}
};

//ͨ������1024----------
const  LONG* FailureInf1024[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//��������1025----------
const  LONG* FailureInf1025[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48), // IRC
    (LONG*)(RM_BUF_ADDR + 52), // IDA2
    (LONG*)(RM_BUF_ADDR + 56), // IDA3
    (LONG*)(RM_BUF_ADDR + 60), // IDA5
    (LONG*)(RM_BUF_ADDR + 64), // IDB2
    (LONG*)(RM_BUF_ADDR + 68), // IDB3
    (LONG*)(RM_BUF_ADDR + 72), // IDB5
    (LONG*)(RM_BUF_ADDR + 76), // IDC2
    (LONG*)(RM_BUF_ADDR + 80), // IDC3
    (LONG*)(RM_BUF_ADDR + 84), // IDC5
    (LONG*)(RM_BUF_ADDR + 88), // IRA2
    (LONG*)(RM_BUF_ADDR + 92), // IRB2
    (LONG*)(RM_BUF_ADDR + 96), // IRC2
    (LONG*)(RM_BUF_ADDR + 128), // UA
    (LONG*)(RM_BUF_ADDR + 132), // UB
    (LONG*)(RM_BUF_ADDR + 136) // UC
};

//��·����բʧ��1026----------
const  LONG* FailureInf1026[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 24) // I4
};

//ʧ�鱣��1027----------
const  LONG* FailureInf1027[] = {
    {NULL}
};

//����Խ�޷���1028----------
const  LONG* FailureInf1028[] = {
    (LONG*)(RM_BUF_ADDR + 0), // IA
    (LONG*)(RM_BUF_ADDR + 100), // ��IA
    (LONG*)(RM_BUF_ADDR + 4), // IB
    (LONG*)(RM_BUF_ADDR + 104), // ��IB
    (LONG*)(RM_BUF_ADDR + 8), // IC
    (LONG*)(RM_BUF_ADDR + 108), // ��IC
    (LONG*)(RM_BUF_ADDR + 12), // I1
    (LONG*)(RM_BUF_ADDR + 112), // ��I1
    (LONG*)(RM_BUF_ADDR + 16), // I2
    (LONG*)(RM_BUF_ADDR + 116), // ��I2
    (LONG*)(RM_BUF_ADDR + 20), // I3
    (LONG*)(RM_BUF_ADDR + 120), // ��I3
    (LONG*)(RM_BUF_ADDR + 24), // I4
    (LONG*)(RM_BUF_ADDR + 124), // ��I4
    (LONG*)(RM_BUF_ADDR + 28), // IDA
    (LONG*)(RM_BUF_ADDR + 32), // IDB
    (LONG*)(RM_BUF_ADDR + 36), // IDC
    (LONG*)(RM_BUF_ADDR + 40), // IRA
    (LONG*)(RM_BUF_ADDR + 44), // IRB
    (LONG*)(RM_BUF_ADDR + 48) // IRC
};

//���������쳣22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//���������쳣�ָ�65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//�������ö���------------------------------------------
const struct TReportInf ReportInf[] = {
    {2, FailureInf1, 1},//�������ʧЧ
    {2, FailureInf2, 2},//�����������
    {0, FailureInf3, 3},//AD����
    {0, FailureInf4, 4},//��ѹ�����
    {0, FailureInf5, 5},//EEPROM��ֵ����
    {0, FailureInf8, 8},//RAM��ֵ����
    {0, FailureInf11, 11},//����ROM����
    {0, FailureInf13, 13},//װ���쳣��λ
    {0, FailureInf14, 14},//ͨ��ͨ���쳣
    {0, FailureInf16, 16},//NVRAM���ݳ���
    {0, FailureInf17, 17},//�߼����̹���
    {0, FailureInf18, 18},//�����Լ����
    {0, FailureInf20, 20},//5V��Դ����
    {0, FailureInf21, 21},//EEPROMд�����
    {0, FailureInf50, 50},//AD���ϻָ�
    {0, FailureInf51, 51},//��ѹ����ϻָ�
    {0, FailureInf52, 52},//EEPROM��ֵ���ϻָ�
    {0, FailureInf55, 55},//RAM��ֵ���ϻָ�
    {0, FailureInf58, 58},//����ROM���ϻָ�
    {0, FailureInf60, 60},//ͨ��ͨ���쳣�ָ�
    {0, FailureInf62, 62},//NVRAM���ݳ���ָ�
    {0, FailureInf63, 63},//�߼����̹��Ϲ��ϻָ�
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf500, 500},//��������
    {0, FailureInf501, 501},//����˹
    {0, FailureInf502, 502},//ѹ���ͷ�
    {0, FailureInf503, 503},//������բ
    {0, FailureInf504, 504},//����˹
    {0, FailureInf505, 505},//��λ�쳣
    {0, FailureInf506, 506},//�¶ȹ���
    {0, FailureInf507, 507},//ʧ��澯
    {0, FailureInf508, 508},//����˹1
    {0, FailureInf509, 509},//ѹ���ͷ�1
    {0, FailureInf510, 510},//������բ1
    {0, FailureInf511, 511},//����˹1
    {0, FailureInf512, 512},//��λ�쳣1
    {0, FailureInf513, 513},//�¶ȹ���1
    {0, FailureInf514, 514},//ʧ��澯1
    {0, FailureInf515, 515},//����˹2
    {0, FailureInf516, 516},//ѹ���ͷ�2
    {0, FailureInf517, 517},//������բ2
    {0, FailureInf518, 518},//����˹2
    {0, FailureInf519, 519},//��λ�쳣2
    {0, FailureInf520, 520},//�¶ȹ���2
    {0, FailureInf521, 521},//ʧ��澯2
    {32, FailureInf1001, 1001},//����Խ��
    {0, FailureInf1002, 1002},//A���ٶ�����
    {35, FailureInf1003, 1003},//A������ٶ϶���
    {0, FailureInf1004, 1004},//A���ٶϷ���
    {0, FailureInf1005, 1005},//B���ٶ�����
    {35, FailureInf1006, 1006},//B���ٶ϶���
    {0, FailureInf1007, 1007},//B���ٶϷ���
    {0, FailureInf1008, 1008},//C���ٶ�����
    {35, FailureInf1009, 1009},//C���ٶ϶���
    {0, FailureInf1010, 1010},//C���ٶϷ���
    {0, FailureInf1011, 1011},//A����ʲ����
    {35, FailureInf1012, 1012},//A����ʲ����
    {0, FailureInf1013, 1013},//A����ʲ����
    {0, FailureInf1014, 1014},//B����ʲ����
    {35, FailureInf1015, 1015},//B����ʲ����
    {0, FailureInf1016, 1016},//B����ʲ����
    {0, FailureInf1017, 1017},//C����ʲ����
    {35, FailureInf1018, 1018},//C����ʲ����
    {0, FailureInf1019, 1019},//C����ʲ����
    {35, FailureInf1024, 1024},//ͨ������
    {35, FailureInf1025, 1025},//��������
    {7, FailureInf1026, 1026},//��·����բʧ��
    {0, FailureInf1027, 1027},//ʧ�鱣��
    {20, FailureInf1028, 1028},//����Խ�޷���
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
8,8,8,8,8,8,8,8,// ���ڵ��ڼ̵���
9,9,9,9,9,9,9,9,// ����ʱ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �������Ӽ̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �������Ӽ̵���
// �籣���м��
// �籣���Ա��ּ�
// ��������̵���
// ��������̵���
// ���ؿ��Ƽ̵���
0,0,0,0,0,0,0,0,// �����̵���

};
//�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
const  TTimeSetting TimeSetting[] = {
	{ (LONG*)(AS_BUF_ADDR + 4), (LONG*)(AS_BUF_ADDR + 8) },
	{ (LONG*)(AS_BUF_ADDR + 12), (LONG*)(AS_BUF_ADDR + 16) },
	{ (LONG*)(AS_BUF_ADDR + 20), (LONG*)(AS_BUF_ADDR + 24) },
	{ (LONG*)(AS_BUF_ADDR + 28), (LONG*)(AS_BUF_ADDR + 32) },
	{ (LONG*)(AS_BUF_ADDR + 36), (LONG*)(AS_BUF_ADDR + 40) },
	{ (LONG*)(AS_BUF_ADDR + 44), (LONG*)(AS_BUF_ADDR + 48) },
	{ (LONG*)(AS_BUF_ADDR + 52), (LONG*)(AS_BUF_ADDR + 56) },
	{ (LONG*)(AS_BUF_ADDR + 60), (LONG*)(AS_BUF_ADDR + 64) },
	{ (LONG*)(AS_BUF_ADDR + 68), (LONG*)(AS_BUF_ADDR + 72) },
	{ (LONG*)(AS_BUF_ADDR + 76), (LONG*)(AS_BUF_ADDR + 80) },
	{ (LONG*)(AS_BUF_ADDR + 84), (LONG*)(AS_BUF_ADDR + 88) },
	{ (LONG*)(AS_BUF_ADDR + 92), (LONG*)(AS_BUF_ADDR + 96) },
	{ (LONG*)(AS_BUF_ADDR + 100), (LONG*)(AS_BUF_ADDR + 104) },
	{ (LONG*)(AS_BUF_ADDR + 108), (LONG*)(AS_BUF_ADDR + 112) },
	{ (LONG*)(AS_BUF_ADDR + 116), (LONG*)(AS_BUF_ADDR + 120) },
	{ (LONG*)(AS_BUF_ADDR + 124), (LONG*)(AS_BUF_ADDR + 128) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 188) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 112) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 196) ,
	 (LONG*)(AS_BUF_ADDR + 200) ,
	 (LONG*)(AS_BUF_ADDR + 204) ,
};
//����̵���ӳ�䵽ʵ�ʿ���
const WORD InPutMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
	 (WORD)(5) ,
	 (WORD)(6) ,
	 (WORD)(7) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(10) ,
	 (WORD)(11) ,
	 (WORD)(12) ,
	 (WORD)(13) ,
	 (WORD)(14) ,
	 (WORD)(15) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
	 (WORD)(19) ,
	 (WORD)(20) ,
	 (WORD)(21) ,
	 (WORD)(22) ,
	 (WORD)(23) ,
	 (WORD)(24) ,
	 (WORD)(25) ,
	 (WORD)(26) ,
	 (WORD)(27) ,
	 (WORD)(28) ,
	 (WORD)(29) ,
	 (WORD)(30) ,
	 (WORD)(31) ,
	 (WORD)(32) ,
	 (WORD)(33) ,
	 (WORD)(34) ,
	 (WORD)(35) ,
	 (WORD)(36) ,
	 (WORD)(37) ,
	 (WORD)(38) ,
	 (WORD)(39) ,
	 (WORD)(40) ,
	 (WORD)(41) ,
	 (WORD)(42) ,
	 (WORD)(43) ,
	 (WORD)(44) ,
	 (WORD)(45) ,
	 (WORD)(46) ,
	 (WORD)(47) ,
	 (WORD)(48) ,
	 (WORD)(49) ,
	 (WORD)(50) ,
	 (WORD)(51) ,
	 (WORD)(52) ,
	 (WORD)(53) ,
	 (WORD)(54) ,
	 (WORD)(55) ,
	 (WORD)(56) ,
	 (WORD)(57) ,
	 (WORD)(58) ,
	 (WORD)(59) ,
	 (WORD)(60) ,
	 (WORD)(61) ,
	 (WORD)(62) ,
	 (WORD)(63) ,
	 (WORD)(64) ,
};
//�����̵���ӳ�䵽ʵ�ʿ���
const WORD OutPutMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
	 (WORD)(5) ,
	 (WORD)(6) ,
	 (WORD)(7) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(24) ,
	 (WORD)(25) ,
	 (WORD)(26) ,
	 (WORD)(27) ,
	 (WORD)(28) ,
	 (WORD)(29) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
	 (WORD)(19) ,
	 (WORD)(20) ,
};
//ѹ��̵���ӳ�䵽ѹ��ֵ���ڴ���λ��
BYTE* const SWMap[] = {
	 (BYTE*)(SW_BUF_ADDR + 0) ,
	 (BYTE*)(SW_BUF_ADDR + 1) ,
	 (BYTE*)(SW_BUF_ADDR + 2) ,
	 (BYTE*)(SW_BUF_ADDR + 3) ,
	 (BYTE*)(SW_BUF_ADDR + 4) ,
};
//ѹ��̵���ӳ�䵽ѹ���ѹ���
const WORD  SWIndexMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
	 (WORD)(4) ,
};
//���ü̵���ӳ�䵽����
BYTE* const CFGMap[] = {
	 (BYTE*)(CFG_BUF_ADDR + 0) ,
	 (BYTE*)(CFG_BUF_ADDR + 1) ,
	 (BYTE*)(CFG_BUF_ADDR + 2) ,
	 (BYTE*)(CFG_BUF_ADDR + 3) ,
	 (BYTE*)(CFG_BUF_ADDR + 4) ,
	 (BYTE*)(CFG_BUF_ADDR + 5) ,
	 (BYTE*)(CFG_BUF_ADDR + 6) ,
	 (BYTE*)(CFG_BUF_ADDR + 7) ,
	 (BYTE*)(CFG_BUF_ADDR + 8) ,
	 (BYTE*)(CFG_BUF_ADDR + 9) ,
	 (BYTE*)(CFG_BUF_ADDR + 10) ,
	 (BYTE*)(CFG_BUF_ADDR + 11) ,
	 (BYTE*)(CFG_BUF_ADDR + 12) ,
	 (BYTE*)(CFG_BUF_ADDR + 13) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	3060, //IA
	3060, //IB
	3060, //IC
	3060, //I1
	3060, //I2
	3060, //I3
	3060, //I4
	3060, //IDA
	3060, //IDB
	3060, //IDC
	3050, //IRA
	3050, //IRB
	3050, //IRC
	30592, //U
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	43238, //UAM
	43238, //UBM
	43238, //UCM
	4336, //IAM
	4336, //IBM
	4336, //ICM
	4336, //I1M
	4336, //I2M
	4336, //I3M
	4336, //I4M
	21, //UAD
	21, //UBD
	21, //UCD
	21, //IAD
	21, //IBD
	21, //ICD
	21, //I1D
	21, //I2D
	21, //I3D
	21, //I4D
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	15250, //IA
	15250, //IB
	15250, //IC
	15250, //I1
	15250, //I2
	15250, //I3
	15250, //I4
	15250, //IDA
	15250, //IDB
	15250, //IDC
	15250, //IRA
	15250, //IRB
	15250, //IRC
	30592, //U
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	43238, //UAM
	43238, //UBM
	43238, //UCM
	21680, //IAM
	21680, //IBM
	21680, //ICM
	21680, //I1M
	21680, //I2M
	21680, //I3M
	21680, //I4M
	21, //UAD
	21, //UBD
	21, //UCD
	21, //IAD
	21, //IBD
	21, //ICD
	21, //I1D
	21, //I2D
	21, //I3D
	21, //I4D
};

//����----------------------------------
