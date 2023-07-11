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
	{WCTH_0, "��У��"},
	{WCTH_1, "30��У��"},
	{WCTH_2, "60��У��"},
	{WCTH_3, "90��У��"},
	{WCTH_4, "120��У��"},
	{WCTH_5, "150��У��"},
	{WCTH_6, "180��У��"},
	{WCTH_7, "210��У��"},
	{WCTH_8, "240��У��"},
	{WCTH_9, "270��У��"},
	{WCTH_10, "300��У��"},
	{WCTH_11, "330��У��"},
	{WCTH_12, "360��У��"},
	{WCTM_0, "��У��"},
	{WCTM_1, "30��У��"},
	{WCTM_2, "60��У��"},
	{WCTM_3, "90��У��"},
	{WCTM_4, "120��У��"},
	{WCTM_5, "150��У��"},
	{WCTM_6, "180��У��"},
	{WCTM_7, "210��У��"},
	{WCTM_8, "240��У��"},
	{WCTM_9, "270��У��"},
	{WCTM_10, "300��У��"},
	{WCTM_11, "330��У��"},
	{WCTM_12, "360��У��"},
	{WCTL_0, "��У��"},
	{WCTL_1, "30��У��"},
	{WCTL_2, "60��У��"},
	{WCTL_3, "90��У��"},
	{WCTL_4, "120��У��"},
	{WCTL_5, "150��У��"},
	{WCTL_6, "180��У��"},
	{WCTL_7, "210��У��"},
	{WCTL_8, "240��У��"},
	{WCTL_9, "270��У��"},
	{WCTL_10, "300��У��"},
	{WCTL_11, "330��У��"},
	{WCTL_12, "360��У��"},
	{WCTH_0, "��/����"},
	{WCTH_1, "��/����"},
	{WCTH_2, "��/����"},
	{WCTM_0, "��/����"},
	{WCTM_1, "��/����"},
	{WCTM_2, "��/����"},
	{WCTL_0, "��/����"},
	{WCTL_1, "��/����"},
	{WCTL_2, "��/����"},
	{MBPH_0, "A��"},
	{MBPH_1, "B��"},
	{MBPH_2, "C��"},
	{LBPH_0, "A��"},
	{LBPH_1, "B��"},
	{LBPH_2, "C��"},
	{-1, NULL},
};
const TSetValueInfMap SetValueInfMap[]=
{
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{13, (TSetValueInf*)&(SetValueInf[0])}, //��ֵ��ѹ������Ƕ�У��
	{13, (TSetValueInf*)&(SetValueInf[13])}, //��ֵ��ѹ������Ƕ�У��
	{13, (TSetValueInf*)&(SetValueInf[26])}, //��ֵ��ѹ������Ƕ�У��
	{3, (TSetValueInf*)&(SetValueInf[39])}, //��ֵ�߲�����/CT���ӷ�ʽ
	{3, (TSetValueInf*)&(SetValueInf[42])}, //��ֵ�в�����/CT���ӷ�ʽ
	{3, (TSetValueInf*)&(SetValueInf[45])}, //��ֵ�Ͳ�����/CT���ӷ�ʽ
	{3, (TSetValueInf*)&(SetValueInf[48])}, //��ֵ��ѹ�๫����
	{3, (TSetValueInf*)&(SetValueInf[51])}, //��ֵ��ѹ�๫����
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
LONG* 	RM_pnUA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// UA
LONG* 	RM_pnUB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// UB
LONG* 	RM_pnUC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// UC
LONG* 	RM_pnUAB 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// UAB
LONG* 	RM_pnUBC 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// UBC
LONG* 	RM_pnUCA 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// UCA
LONG* 	RM_pnU2H 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// U2H
LONG* 	RM_pnU0 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// U0
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// IHA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// IHB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// IHC
LONG* 	RM_pnIMA 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// IMA
LONG* 	RM_pnIMB 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IMB
LONG* 	RM_pnIMC 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// IMC
LONG* 	RM_pnILA 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// ILA
LONG* 	RM_pnILB 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// ILB
LONG* 	RM_pnILC 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// ILC
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// I0
LONG* 	RM_pnIjx 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// Ijx
LONG* 	RM_pnIDA 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// IDA
LONG* 	RM_pnIDB 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IDB
LONG* 	RM_pnIDC 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// IDC
LONG* 	RM_pnIRA 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// IRA
LONG* 	RM_pnIRB 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// IRB
LONG* 	RM_pnIRC 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// IRC
LONG* 	RM_pnIDA2 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// IDA2
LONG* 	RM_pnIDA3 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// IDA3
LONG* 	RM_pnIDA5 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// IDA5
LONG* 	RM_pnIDB2 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// IDB2
LONG* 	RM_pnIDB3 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// IDB3
LONG* 	RM_pnIDB5 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// IDB5
LONG* 	RM_pnIDC2 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// IDC2
LONG* 	RM_pnIDC3 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// IDC3
LONG* 	RM_pnIDC5 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// IDC5
LONG* 	RM_pnIRA2 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// IRA2
LONG* 	RM_pnIRB2 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// IRB2
LONG* 	RM_pnIRC2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// IRC2
LONG* 	RM_pnAUA 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// ��UA
LONG* 	RM_pnAUB 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// ��UB
LONG* 	RM_pnAUC 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// ��UC
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// ��IHA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// ��IHB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// ��IHC
LONG* 	RM_pnAIMA 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// ��IMA
LONG* 	RM_pnAIMB 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// ��IMB
LONG* 	RM_pnAIMC 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// ��IMC
LONG* 	RM_pnAILA 	= 	(LONG*)(RM_BUF_ADDR + 184); 	// ��ILA
LONG* 	RM_pnAILB 	= 	(LONG*)(RM_BUF_ADDR + 188); 	// ��ILB
LONG* 	RM_pnAILC 	= 	(LONG*)(RM_BUF_ADDR + 192); 	// ��ILC
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 196); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 200); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 204); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 208); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnIHAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//IHAM IHAM
LONG* 	M_pnIHBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//IHBM IHBM
LONG* 	M_pnIHCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//IHCM IHCM
LONG* 	M_pnIMAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IMAM IMAM
LONG* 	M_pnIMBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IMBM IMBM
LONG* 	M_pnIMCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IMCM IMCM
LONG* 	M_pnILAM 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//ILAM ILAM
LONG* 	M_pnILBM 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//ILBM ILBM
LONG* 	M_pnILCM 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//ILCM ILCM
LONG* 	M_pnI0M 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I0M I0M
LONG* 	M_pnIjxM 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//IjxM IjxM


//��ѹ��----------------------------------
BYTE* SW_pby87U 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//������ٶ�
BYTE* SW_pby87R 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//���ʲ����
BYTE* SW_pby50I_H 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//��ѹ���������
BYTE* SW_pby50I_M 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//��ѹ���������
BYTE* SW_pby50I_L 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//��ѹ���������
BYTE* SW_pby50OL 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//�����ɱ���
BYTE* SW_pby27U 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//ʧѹ����
BYTE* SW_pby59UN 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//�����ѹ����
BYTE* SW_pby50IN 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//�����������
BYTE* SW_pby50IJX 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//��϶��������
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby87U_A 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//A������ٶϱ���
BYTE* CFG_pby87U_B 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//B������ٶϱ���
BYTE* CFG_pby87U_C 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//C������ٶϱ���
BYTE* CFG_pby87R_A 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//A����ʲ����
BYTE* CFG_pby87R_B 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//B����ʲ����
BYTE* CFG_pby87R_C 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//C����ʲ����
BYTE* CFG_pbyDCRES_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//������ڷ�������
BYTE* CFG_pby68IdH2M_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//����������������
BYTE* CFG_pby68IdH2C_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//���������ۺ������
BYTE* CFG_pby68IdH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//�������η������
BYTE* CFG_pby68IrH2I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//�ƶ��������α���
BYTE* CFG_pby68IdH5I_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//�������г������
BYTE* CFG_pby68IdHMUL_87R 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//�����ۺ�г������
BYTE* CFG_pbyMAB 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//��ѹ������
BYTE* CFG_pbyLAB 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//��ѹ������
BYTE* CFG_pby50I_H1 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//��ѹ��������
BYTE* CFG_pby50I_H1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//�߹�����ι��ʷ���
BYTE* CFG_pby50I_H1_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//�߹�����θ�ѹ����
BYTE* CFG_pby50I_H2 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//��ѹ��������
BYTE* CFG_pby50I_H2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//�߹�����ι��ʷ���
BYTE* CFG_pby50I_H2_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//�߹�����θ�ѹ����
BYTE* CFG_pby50I_M1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//��ѹ��������
BYTE* CFG_pby50I_M2 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//��ѹ��������
BYTE* CFG_pby50I_L1 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//��ѹ��������
BYTE* CFG_pby50I_L2 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//��ѹ��������
BYTE* CFG_pby50OL_1 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//�����ɢ��
BYTE* CFG_pby50OL_2 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//�����ɢ��
BYTE* CFG_pby50OL_3 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//�����ɢ��
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//ʧѹ����
BYTE* CFG_pby27U_50I 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//ʧѹ������������
BYTE* CFG_pby59UN 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//�����ѹ����
BYTE* CFG_pby50IN 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//�����������
BYTE* CFG_pby50IJX 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//��϶��������
BYTE* CFG_pby74VT 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//PT����
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnCFG1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������1 CFG1
LONG* RS_pnCFG2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//������2 CFG2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//ͻ������������ PIC
LONG* RS_pnWHCTC 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//��ѹ������Ƕ�У�� WHCTC
LONG* RS_pnWMCTC 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//��ѹ������Ƕ�У�� WMCTC
LONG* RS_pnWLCTC 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//��ѹ������Ƕ�У�� WLCTC
LONG* RS_pnWCTHCON 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//�߲�����/CT���ӷ�ʽ WCTHCON
LONG* RS_pnWCTMCON 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//�в�����/CT���ӷ�ʽ WCTMCON
LONG* RS_pnWCTLCON 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//�Ͳ�����/CT���ӷ�ʽ WCTLCON
LONG* RS_pnMBph 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//��ѹ�๫���� MBph
LONG* RS_pnLBph 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//��ѹ�๫���� LBph
LONG* RS_pn87UIA 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//A���ٶ϶�ֵ 87UIA
LONG* RS_pn87RIA 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//A��������ֵ 87RIA
LONG* RS_pnIRS1A 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//A���ƶ��յ�1 IRS1A
LONG* RS_pnIRS2A 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//A���ƶ��յ�2 IRS2A
LONG* RS_pn87UIB 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//B���ٶ϶�ֵ 87UIB
LONG* RS_pn87RIB 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//B��������ֵ 87RIB
LONG* RS_pnIRS1B 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//B���ƶ��յ�1 IRS1B
LONG* RS_pnIRS2B 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//B���ƶ��յ�2 IRS2B
LONG* RS_pn87UIC 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//C���ٶ϶�ֵ 87UIC
LONG* RS_pn87RIC 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//C��������ֵ 87RIC
LONG* RS_pnIRS1C 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//C���ƶ��յ�1 IRS1C
LONG* RS_pnIRS2C 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//C���ƶ��յ�2 IRS2C
LONG* RS_pnSLP1 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//�����ƶ�ϵ��1 SLP1
LONG* RS_pnSLP2 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//�����ƶ�ϵ��2 SLP2
LONG* RS_pnKrh2 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//����г���ƶ�ϵ�� Krh2
LONG* RS_pnKrh5 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//���г���ƶ�ϵ�� Krh5
LONG* RS_pnKrhm 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//�ۺ�г���ƶ�ϵ�� Krhm
LONG* RS_pnKrav 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//������ڼ�Ȩϵ�� Krav
LONG* RS_pnK2av 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//���η����ڼ�Ȩϵ�� K2av
LONG* RS_pnKma 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//��ѹ��A��ƽ��ϵ�� Kma
LONG* RS_pnKmb 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//��ѹ��B��ƽ��ϵ�� Kmb
LONG* RS_pnKmc 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//��ѹ��C��ƽ��ϵ�� Kmc
LONG* RS_pnKla 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//��ѹ��A��ƽ��ϵ�� Kla
LONG* RS_pnKlb 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//��ѹ��B��ƽ��ϵ�� Klb
LONG* RS_pnKlc 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//��ѹ��C��ƽ��ϵ�� Klc
LONG* RS_pnK87AR 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//����Խ�ޱ������� K87AR
LONG* RS_pn50IH1C 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//��ѹ�������ε��� 50IH1C
LONG* RS_pn50IH1D 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//��ѹ��������ʱ�� 50IH1D
LONG* RS_pn50IH2C 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//��ѹ�������ε��� 50IH2C
LONG* RS_pn50IH2D 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//��ѹ��������ʱ�� 50IH2D
LONG* RS_pn47U_27U1 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//�߹��������͵�ѹ 47U_27U1
LONG* RS_pn47U_59U2 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//�߹������������ѹ 47U_59U2
LONG* RS_pnA_50IH 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//��ѹ����������� A_50IH
LONG* RS_pn50IM1C 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//��ѹ�������ε��� 50IM1C
LONG* RS_pn50IM1D 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//��ѹ��������ʱ�� 50IM1D
LONG* RS_pn50IM2C 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//��ѹ�������ε��� 50IM2C
LONG* RS_pn50IM2D 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//��ѹ��������ʱ�� 50IM2D
LONG* RS_pn50IL1C 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//��ѹ�������ε��� 50IL1C
LONG* RS_pn50IL1D 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//��ѹ��������ʱ�� 50IL1D
LONG* RS_pn50IL2C 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//��ѹ�������ε��� 50IL2C
LONG* RS_pn50IL2D 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//��ѹ��������ʱ�� 50IL2D
LONG* RS_pn50OL1C 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//�����ɢ�ε��� 50OL1C
LONG* RS_pn50OL1D 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//�����ɢ��ʱ�� 50OL1D
LONG* RS_pn50OL2C 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//�����ɢ�ε��� 50OL2C
LONG* RS_pn50OL2D 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//�����ɢ��ʱ�� 50OL2D
LONG* RS_pn50OL3C 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//�����ɢ�ε��� 50OL3C
LONG* RS_pn50OL3D 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//�����ɢ��ʱ�� 50OL3D
LONG* RS_pn27UV 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//ʧѹ�������ѹ 27UV
LONG* RS_pn27UD 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//ʧѹ����ʱ�� 27UD
LONG* RS_pn27U_50I 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//ʧѹ������������ 27U_50I
LONG* RS_pn59UNV 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//�����ѹ��ѹ��ֵ 59UNV
LONG* RS_pn59UND 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//�����ѹʱ�䶨ֵ 59UND
LONG* RS_pn50INC 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//�������������ֵ 50INC
LONG* RS_pn50IND 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//�������ʱ�䶨ֵ 50IND
LONG* RS_pn50IJXC 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//��϶����������ֵ 50IJXC
LONG* RS_pn50IJXD 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//��϶����ʱ�䶨ֵ 50IJXD
LONG* RS_pn74VTV 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//PT���߼���ѹ 74VTV
LONG* RS_pn74VTC 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//PT���߼����� 74VTC
LONG* RS_pn74VTD 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//PT���߼��ʱ�� 74VTD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//ʧ�鱣��ʱ�� 52BFD

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
LONG* AS_pn59UA 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//UA��ѹ��ֵ
LONG* AS_pn27UA 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//UA��ѹ��ֵ
LONG* AS_pn59UB 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//UB��ѹ��ֵ
LONG* AS_pn27UB 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//UB��ѹ��ֵ
LONG* AS_pn59UC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//UC��ѹ��ֵ
LONG* AS_pn27UC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//UC��ѹ��ֵ
LONG* AS_pn59UN 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//UN��ѹ��ֵ
LONG* AS_pn27UN 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//UN��ѹ��ֵ
LONG* AS_pn50HAC 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//IHA������ֵ
LONG* AS_pn37HAC 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//IHA������ֵ
LONG* AS_pn50HBC 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//IHB������ֵ
LONG* AS_pn37HBC 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//IHB������ֵ
LONG* AS_pn50HCC 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//IHC������ֵ
LONG* AS_pn37HCC 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//IHC������ֵ
LONG* AS_pn50MAC 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//IMA������ֵ
LONG* AS_pn37MAC 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//IMA������ֵ
LONG* AS_pn50MBC 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//IMB������ֵ
LONG* AS_pn37MBC 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//IMB������ֵ
LONG* AS_pn50MCC 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//IMC������ֵ
LONG* AS_pn37MCC 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//IMC������ֵ
LONG* AS_pn50LAC 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//ILA������ֵ
LONG* AS_pn37LAC 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//ILA������ֵ
LONG* AS_pn50LBC 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//ILB������ֵ
LONG* AS_pn37LBC 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//ILB������ֵ
LONG* AS_pn50LCC 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//ILC������ֵ
LONG* AS_pn37LCC 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//ILC������ֵ
LONG* AS_pn50INC 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//IN������ֵ
LONG* AS_pn37INC 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//IN������ֵ
LONG* AS_pn50IJXC 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//IJX������ֵ
LONG* AS_pn37IJXC 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//IJX������ֵ
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//��·��������ʱ
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//����������ʱ
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//TR2����ʱ��
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//TR3����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnHPT 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//��ѹ��PT���
LONG* AS_pnNPT 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//����PT���
LONG* AS_pnHACT 	= 	(LONG*)(AS_BUF_ADDR + 284); 	//��ѹ��A��CT���
LONG* AS_pnHBCT 	= 	(LONG*)(AS_BUF_ADDR + 288); 	//��ѹ��B��CT���
LONG* AS_pnHCCT 	= 	(LONG*)(AS_BUF_ADDR + 292); 	//��ѹ��C��CT���
LONG* AS_pnMACT 	= 	(LONG*)(AS_BUF_ADDR + 296); 	//��ѹ��A��CT���
LONG* AS_pnMBCT 	= 	(LONG*)(AS_BUF_ADDR + 300); 	//��ѹ��B��CT���
LONG* AS_pnMCCT 	= 	(LONG*)(AS_BUF_ADDR + 304); 	//��ѹ��C��CT���
LONG* AS_pnLACT 	= 	(LONG*)(AS_BUF_ADDR + 308); 	//��ѹ��A��CT���
LONG* AS_pnLBCT 	= 	(LONG*)(AS_BUF_ADDR + 312); 	//��ѹ��B��CT���
LONG* AS_pnLCCT 	= 	(LONG*)(AS_BUF_ADDR + 316); 	//��ѹ��C��CT���
LONG* AS_pnNCT 	= 	(LONG*)(AS_BUF_ADDR + 320); 	//����CT���
LONG* AS_pnJXCT 	= 	(LONG*)(AS_BUF_ADDR + 324); 	//��϶CT���
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 328); 	//�̶����

//ϵ��----------------------------------
LONG* CF_pnUp 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//Up
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U0
LONG* CF_pnIp 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//Ip
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//I0
LONG* CF_pnIjx 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//Ijx
LONG* CF_pnUAM 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UAM
LONG* CF_pnUBM 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//UBM
LONG* CF_pnUCM 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UCM
LONG* CF_pnU0M 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//U0M
LONG* CF_pnIHAM 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IHAM
LONG* CF_pnIHBM 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IHBM
LONG* CF_pnIHCM 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IHCM
LONG* CF_pnIMAM 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IMAM
LONG* CF_pnIMBM 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IMBM
LONG* CF_pnIMCM 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//IMCM
LONG* CF_pnILAM 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//ILAM
LONG* CF_pnILBM 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//ILBM
LONG* CF_pnILCM 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//ILCM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//I0M
LONG* CF_pnIjxM 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//IjxM
LONG* CF_pnUAD 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//UAD
LONG* CF_pnUBD 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//UBD
LONG* CF_pnUCD 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//UCD
LONG* CF_pnU0D 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//U0D
LONG* CF_pnIHAD 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//IHAD
LONG* CF_pnIHBD 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//IHBD
LONG* CF_pnIHCD 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//IHCD
LONG* CF_pnIMAD 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//IMAD
LONG* CF_pnIMBD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//IMBD
LONG* CF_pnIMCD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//IMCD
LONG* CF_pnILAD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//ILAD
LONG* CF_pnILBD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//ILBD
LONG* CF_pnILCD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//ILCD
LONG* CF_pnI0D 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//I0D
LONG* CF_pnIjxD 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//IjxD
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//C4

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 204), // IN1
    (LONG*)(RM_BUF_ADDR + 208) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 196), // BR1
    (LONG*)(RM_BUF_ADDR + 200) // BR2
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

//���������쳣22----------
const  LONG* FailureInf22[] = {
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

//���������쳣�ָ�65----------
const  LONG* FailureInf65[] = {
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

//����Խ��4100----------
const  LONG* FailureInf4100[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//A���ٶ�����4101----------
const  LONG* FailureInf4101[] = {
    {NULL}
};

//A������ٶ϶���4102----------
const  LONG* FailureInf4102[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//A���ٶϷ���4103----------
const  LONG* FailureInf4103[] = {
    {NULL}
};

//B���ٶ�����4104----------
const  LONG* FailureInf4104[] = {
    {NULL}
};

//B���ٶ϶���4105----------
const  LONG* FailureInf4105[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//B���ٶϷ���4106----------
const  LONG* FailureInf4106[] = {
    {NULL}
};

//C���ٶ�����4107----------
const  LONG* FailureInf4107[] = {
    {NULL}
};

//C���ٶ϶���4108----------
const  LONG* FailureInf4108[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//C���ٶϷ���4109----------
const  LONG* FailureInf4109[] = {
    {NULL}
};

//A����ʲ����4110----------
const  LONG* FailureInf4110[] = {
    {NULL}
};

//A����ʲ����4111----------
const  LONG* FailureInf4111[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//A����ʲ����4112----------
const  LONG* FailureInf4112[] = {
    {NULL}
};

//B����ʲ����4113----------
const  LONG* FailureInf4113[] = {
    {NULL}
};

//B����ʲ����4114----------
const  LONG* FailureInf4114[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//B����ʲ����4115----------
const  LONG* FailureInf4115[] = {
    {NULL}
};

//C����ʲ����4116----------
const  LONG* FailureInf4116[] = {
    {NULL}
};

//C����ʲ����4117----------
const  LONG* FailureInf4117[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144) // IRC2
};

//C����ʲ����4118----------
const  LONG* FailureInf4118[] = {
    {NULL}
};

//��ѹ������������4119----------
const  LONG* FailureInf4119[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//��ѹ�������ζ���4120----------
const  LONG* FailureInf4120[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ��ILC
};

//��ѹ�������η���4121----------
const  LONG* FailureInf4121[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//��ѹ������������4122----------
const  LONG* FailureInf4122[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//��ѹ�������ζ���4123----------
const  LONG* FailureInf4123[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ��ILC
};

//��ѹ�������η���4124----------
const  LONG* FailureInf4124[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//��ѹ������������4125----------
const  LONG* FailureInf4125[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//��ѹ�������ζ���4126----------
const  LONG* FailureInf4126[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ��UC
};

//��ѹ�������η���4127----------
const  LONG* FailureInf4127[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//��ѹ������������4128----------
const  LONG* FailureInf4128[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//��ѹ�������ζ���4129----------
const  LONG* FailureInf4129[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ��UC
};

//��ѹ�������η���4130----------
const  LONG* FailureInf4130[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52) // IMC
};

//��ѹ������������4131----------
const  LONG* FailureInf4131[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//��ѹ�������ζ���4132----------
const  LONG* FailureInf4132[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ��UC
};

//��ѹ�������η���4133----------
const  LONG* FailureInf4133[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//��ѹ������������4134----------
const  LONG* FailureInf4134[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//��ѹ�������ζ���4135----------
const  LONG* FailureInf4135[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192), // ��ILC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156) // ��UC
};

//��ѹ�������η���4136----------
const  LONG* FailureInf4136[] = {
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64) // ILC
};

//�����ɢ������4137----------
const  LONG* FailureInf4137[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//�����ɢ�ζ���4138----------
const  LONG* FailureInf4138[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ��ILC
};

//�����ɢ�η���4139----------
const  LONG* FailureInf4139[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//�����ɢ������4140----------
const  LONG* FailureInf4140[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//�����ɢ�ζ���4141----------
const  LONG* FailureInf4141[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ��ILC
};

//�����ɢ�η���4142----------
const  LONG* FailureInf4142[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//�����ɢ������4143----------
const  LONG* FailureInf4143[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//�����ɢ�ζ���4144----------
const  LONG* FailureInf4144[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24), // U2H
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 192) // ��ILC
};

//�����ɢ�η���4145----------
const  LONG* FailureInf4145[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//ʧѹ��������4146----------
const  LONG* FailureInf4146[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168) // ��IHC
};

//��ѹ��PT����4147----------
const  LONG* FailureInf4147[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 148), // ��UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 152), // ��UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 156), // ��UC
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 12), // UAB
    (LONG*)(RM_BUF_ADDR + 16), // UBC
    (LONG*)(RM_BUF_ADDR + 20), // UCA
    (LONG*)(RM_BUF_ADDR + 24) // U2H
};

//ͨ������4148----------
const  LONG* FailureInf4148[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 100), // IDA2
    (LONG*)(RM_BUF_ADDR + 104), // IDA3
    (LONG*)(RM_BUF_ADDR + 108), // IDA5
    (LONG*)(RM_BUF_ADDR + 112), // IDB2
    (LONG*)(RM_BUF_ADDR + 116), // IDB3
    (LONG*)(RM_BUF_ADDR + 120), // IDB5
    (LONG*)(RM_BUF_ADDR + 124), // IDC2
    (LONG*)(RM_BUF_ADDR + 128), // IDC3
    (LONG*)(RM_BUF_ADDR + 132), // IDC5
    (LONG*)(RM_BUF_ADDR + 136), // IRA2
    (LONG*)(RM_BUF_ADDR + 140), // IRB2
    (LONG*)(RM_BUF_ADDR + 144), // IRC2
    (LONG*)(RM_BUF_ADDR + 160), // ��IHA
    (LONG*)(RM_BUF_ADDR + 164), // ��IHB
    (LONG*)(RM_BUF_ADDR + 168), // ��IHC
    (LONG*)(RM_BUF_ADDR + 172), // ��IMA
    (LONG*)(RM_BUF_ADDR + 176), // ��IMB
    (LONG*)(RM_BUF_ADDR + 180), // ��IMC
    (LONG*)(RM_BUF_ADDR + 184), // ��ILA
    (LONG*)(RM_BUF_ADDR + 188), // ��ILB
    (LONG*)(RM_BUF_ADDR + 192) // ��ILC
};

//��������4149----------
const  LONG* FailureInf4149[] = {
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 44), // IMA
    (LONG*)(RM_BUF_ADDR + 48), // IMB
    (LONG*)(RM_BUF_ADDR + 52), // IMC
    (LONG*)(RM_BUF_ADDR + 56), // ILA
    (LONG*)(RM_BUF_ADDR + 60), // ILB
    (LONG*)(RM_BUF_ADDR + 64), // ILC
    (LONG*)(RM_BUF_ADDR + 76), // IDA
    (LONG*)(RM_BUF_ADDR + 80), // IDB
    (LONG*)(RM_BUF_ADDR + 84), // IDC
    (LONG*)(RM_BUF_ADDR + 88), // IRA
    (LONG*)(RM_BUF_ADDR + 92), // IRB
    (LONG*)(RM_BUF_ADDR + 96), // IRC
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8) // UC
};

//ʧ�鱣��4150----------
const  LONG* FailureInf4150[] = {
    {NULL}
};

//�߲��·����բʧ��4151----------
const  LONG* FailureInf4151[] = {
    {NULL}
};

//�в��·����բʧ��4152----------
const  LONG* FailureInf4152[] = {
    {NULL}
};

//�Ͳ��·����բʧ��4153----------
const  LONG* FailureInf4153[] = {
    {NULL}
};

//�����ѹ��������4154----------
const  LONG* FailureInf4154[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 28) // U0
};

//�����ѹ��������4155----------
const  LONG* FailureInf4155[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 28) // U0
};

//�����ѹ��������4156----------
const  LONG* FailureInf4156[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UA
    (LONG*)(RM_BUF_ADDR + 4), // UB
    (LONG*)(RM_BUF_ADDR + 8), // UC
    (LONG*)(RM_BUF_ADDR + 28) // U0
};

//���������������4157----------
const  LONG* FailureInf4157[] = {
    (LONG*)(RM_BUF_ADDR + 68) // I0
};

//���������������4158----------
const  LONG* FailureInf4158[] = {
    (LONG*)(RM_BUF_ADDR + 28), // U0
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 68) // I0
};

//���������������4159----------
const  LONG* FailureInf4159[] = {
    (LONG*)(RM_BUF_ADDR + 68) // I0
};

//��϶������������4160----------
const  LONG* FailureInf4160[] = {
    (LONG*)(RM_BUF_ADDR + 72) // Ijx
};

//��϶������������4161----------
const  LONG* FailureInf4161[] = {
    (LONG*)(RM_BUF_ADDR + 28), // U0
    (LONG*)(RM_BUF_ADDR + 32), // IHA
    (LONG*)(RM_BUF_ADDR + 36), // IHB
    (LONG*)(RM_BUF_ADDR + 40), // IHC
    (LONG*)(RM_BUF_ADDR + 68), // I0
    (LONG*)(RM_BUF_ADDR + 72) // Ijx
};

//��϶������������4162----------
const  LONG* FailureInf4162[] = {
    (LONG*)(RM_BUF_ADDR + 28), // U0
    (LONG*)(RM_BUF_ADDR + 72) // Ijx
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
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf50, 50},//AD���ϻָ�
    {0, FailureInf51, 51},//��ѹ����ϻָ�
    {0, FailureInf52, 52},//EEPROM��ֵ���ϻָ�
    {0, FailureInf55, 55},//RAM��ֵ���ϻָ�
    {0, FailureInf58, 58},//����ROM���ϻָ�
    {0, FailureInf60, 60},//ͨ��ͨ���쳣�ָ�
    {0, FailureInf62, 62},//NVRAM���ݳ���ָ�
    {0, FailureInf63, 63},//�߼����̹��Ϲ��ϻָ�
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf65, 65},//���������쳣�ָ�
    {0, FailureInf500, 500},//��������
    {0, FailureInf501, 501},//����˹
    {0, FailureInf502, 502},//ѹ���ͷ�
    {0, FailureInf503, 503},//������բ
    {0, FailureInf504, 504},//����˹
    {0, FailureInf505, 505},//��λ�쳣
    {0, FailureInf506, 506},//�¶ȹ���
    {27, FailureInf4100, 4100},//����Խ��
    {0, FailureInf4101, 4101},//A���ٶ�����
    {36, FailureInf4102, 4102},//A������ٶ϶���
    {0, FailureInf4103, 4103},//A���ٶϷ���
    {0, FailureInf4104, 4104},//B���ٶ�����
    {36, FailureInf4105, 4105},//B���ٶ϶���
    {0, FailureInf4106, 4106},//B���ٶϷ���
    {0, FailureInf4107, 4107},//C���ٶ�����
    {36, FailureInf4108, 4108},//C���ٶ϶���
    {0, FailureInf4109, 4109},//C���ٶϷ���
    {0, FailureInf4110, 4110},//A����ʲ����
    {36, FailureInf4111, 4111},//A����ʲ����
    {0, FailureInf4112, 4112},//A����ʲ����
    {0, FailureInf4113, 4113},//B����ʲ����
    {36, FailureInf4114, 4114},//B����ʲ����
    {0, FailureInf4115, 4115},//B����ʲ����
    {0, FailureInf4116, 4116},//C����ʲ����
    {36, FailureInf4117, 4117},//C����ʲ����
    {0, FailureInf4118, 4118},//C����ʲ����
    {6, FailureInf4119, 4119},//��ѹ������������
    {28, FailureInf4120, 4120},//��ѹ�������ζ���
    {6, FailureInf4121, 4121},//��ѹ�������η���
    {6, FailureInf4122, 4122},//��ѹ������������
    {28, FailureInf4123, 4123},//��ѹ�������ζ���
    {6, FailureInf4124, 4124},//��ѹ�������η���
    {3, FailureInf4125, 4125},//��ѹ������������
    {24, FailureInf4126, 4126},//��ѹ�������ζ���
    {3, FailureInf4127, 4127},//��ѹ�������η���
    {3, FailureInf4128, 4128},//��ѹ������������
    {24, FailureInf4129, 4129},//��ѹ�������ζ���
    {3, FailureInf4130, 4130},//��ѹ�������η���
    {3, FailureInf4131, 4131},//��ѹ������������
    {24, FailureInf4132, 4132},//��ѹ�������ζ���
    {3, FailureInf4133, 4133},//��ѹ�������η���
    {3, FailureInf4134, 4134},//��ѹ������������
    {24, FailureInf4135, 4135},//��ѹ�������ζ���
    {3, FailureInf4136, 4136},//��ѹ�������η���
    {6, FailureInf4137, 4137},//�����ɢ������
    {28, FailureInf4138, 4138},//�����ɢ�ζ���
    {6, FailureInf4139, 4139},//�����ɢ�η���
    {6, FailureInf4140, 4140},//�����ɢ������
    {28, FailureInf4141, 4141},//�����ɢ�ζ���
    {6, FailureInf4142, 4142},//�����ɢ�η���
    {6, FailureInf4143, 4143},//�����ɢ������
    {28, FailureInf4144, 4144},//�����ɢ�ζ���
    {6, FailureInf4145, 4145},//�����ɢ�η���
    {12, FailureInf4146, 4146},//ʧѹ��������
    {16, FailureInf4147, 4147},//��ѹ��PT����
    {36, FailureInf4148, 4148},//ͨ������
    {18, FailureInf4149, 4149},//��������
    {0, FailureInf4150, 4150},//ʧ�鱣��
    {0, FailureInf4151, 4151},//�߲��·����բʧ��
    {0, FailureInf4152, 4152},//�в��·����բʧ��
    {0, FailureInf4153, 4153},//�Ͳ��·����բʧ��
    {4, FailureInf4154, 4154},//�����ѹ��������
    {4, FailureInf4155, 4155},//�����ѹ��������
    {4, FailureInf4156, 4156},//�����ѹ��������
    {1, FailureInf4157, 4157},//���������������
    {5, FailureInf4158, 4158},//���������������
    {1, FailureInf4159, 4159},//���������������
    {1, FailureInf4160, 4160},//��϶������������
    {6, FailureInf4161, 4161},//��϶������������
    {2, FailureInf4162, 4162},//��϶������������
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 280) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 280) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 280) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 260) ,
	 (LONG*)(AS_BUF_ADDR + 264) ,
	 (LONG*)(AS_BUF_ADDR + 268) ,
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
	 (WORD)(65) ,
};
//�����̵���ӳ�䵽ʵ�ʿ���
const WORD OutPutMap[] = {
	 (WORD)(0) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
	 (WORD)(19) ,
	 (WORD)(20) ,
	 (WORD)(21) ,
	 (WORD)(22) ,
	 (WORD)(23) ,
	 (WORD)(40) ,
	 (WORD)(41) ,
	 (WORD)(42) ,
	 (WORD)(43) ,
	 (WORD)(44) ,
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
};
//ѹ��̵���ӳ�䵽ѹ��ֵ���ڴ���λ��
BYTE* const SWMap[] = {
	 (BYTE*)(SW_BUF_ADDR + 0) ,
	 (BYTE*)(SW_BUF_ADDR + 1) ,
	 (BYTE*)(SW_BUF_ADDR + 2) ,
	 (BYTE*)(SW_BUF_ADDR + 3) ,
	 (BYTE*)(SW_BUF_ADDR + 4) ,
	 (BYTE*)(SW_BUF_ADDR + 5) ,
	 (BYTE*)(SW_BUF_ADDR + 6) ,
	 (BYTE*)(SW_BUF_ADDR + 7) ,
	 (BYTE*)(SW_BUF_ADDR + 8) ,
	 (BYTE*)(SW_BUF_ADDR + 9) ,
	 (BYTE*)(SW_BUF_ADDR + 10) ,
	 (BYTE*)(SW_BUF_ADDR + 11) ,
	 (BYTE*)(SW_BUF_ADDR + 12) ,
};
//ѹ��̵���ӳ�䵽ѹ���ѹ���
const WORD  SWIndexMap[] = {
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
	 (BYTE*)(CFG_BUF_ADDR + 15) ,
	 (BYTE*)(CFG_BUF_ADDR + 16) ,
	 (BYTE*)(CFG_BUF_ADDR + 17) ,
	 (BYTE*)(CFG_BUF_ADDR + 18) ,
	 (BYTE*)(CFG_BUF_ADDR + 19) ,
	 (BYTE*)(CFG_BUF_ADDR + 20) ,
	 (BYTE*)(CFG_BUF_ADDR + 21) ,
	 (BYTE*)(CFG_BUF_ADDR + 22) ,
	 (BYTE*)(CFG_BUF_ADDR + 23) ,
	 (BYTE*)(CFG_BUF_ADDR + 24) ,
	 (BYTE*)(CFG_BUF_ADDR + 25) ,
	 (BYTE*)(CFG_BUF_ADDR + 26) ,
	 (BYTE*)(CFG_BUF_ADDR + 27) ,
	 (BYTE*)(CFG_BUF_ADDR + 28) ,
	 (BYTE*)(CFG_BUF_ADDR + 29) ,
	 (BYTE*)(CFG_BUF_ADDR + 30) ,
	 (BYTE*)(CFG_BUF_ADDR + 31) ,
	 (BYTE*)(CFG_BUF_ADDR + 32) ,
	 (BYTE*)(CFG_BUF_ADDR + 33) ,
	 (BYTE*)(CFG_BUF_ADDR + 34) ,
	 (BYTE*)(CFG_BUF_ADDR + 35) ,
	 (BYTE*)(CFG_BUF_ADDR + 36) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	30592, //Up
	11600, //U0
	3050, //Ip
	3050, //I0
	3050, //Ijx
	43238, //UAM
	43238, //UBM
	43238, //UCM
	16400, //U0M
	4336, //IHAM
	4336, //IHBM
	4336, //IHCM
	4336, //IMAM
	4336, //IMBM
	4336, //IMCM
	4336, //ILAM
	4336, //ILBM
	4336, //ILCM
	4336, //I0M
	4336, //IjxM
	21, //UAD
	21, //UBD
	21, //UCD
	21, //U0D
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //IMAD
	21, //IMBD
	21, //IMCD
	21, //ILAD
	21, //ILBD
	21, //ILCD
	21, //I0D
	21, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	30592, //Up
	11600, //U0
	15250, //Ip
	15250, //I0
	15250, //Ijx
	43238, //UAM
	43238, //UBM
	43238, //UCM
	16400, //U0M
	21680, //IHAM
	21680, //IHBM
	21680, //IHCM
	21680, //IMAM
	21680, //IMBM
	21680, //IMCM
	21680, //ILAM
	21680, //ILBM
	21680, //ILCM
	21680, //I0M
	21680, //IjxM
	21, //UAD
	21, //UBD
	21, //UCD
	21, //U0D
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //IMAD
	21, //IMBD
	21, //IMCD
	21, //ILAD
	21, //ILBD
	21, //ILCD
	21, //I0D
	21, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};

//����----------------------------------
