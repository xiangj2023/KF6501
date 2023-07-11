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
	{-1, NULL},
};
const TSetValueInfMap SetValueInfMap[]=
{
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
};

//��������ֵ����-----------------------
LONG* 	RM_pnUHA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	//UA UA
LONG* 	RM_pnUHB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	//UB UB
LONG* 	RM_pnUHC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	//UC UC
LONG* 	RM_pnUAB 	= 	(LONG*)(RM_BUF_ADDR + 12); 	//UAB UAB
LONG* 	RM_pnUBC 	= 	(LONG*)(RM_BUF_ADDR + 16); 	//UBC UBC
LONG* 	RM_pnUCA 	= 	(LONG*)(RM_BUF_ADDR + 20); 	//UCA UCA
LONG* 	RM_pnU0H 	= 	(LONG*)(RM_BUF_ADDR + 24); 	//U0H U0H
LONG* 	RM_pnULa 	= 	(LONG*)(RM_BUF_ADDR + 28); 	//Ua Ua
LONG* 	RM_pnULb 	= 	(LONG*)(RM_BUF_ADDR + 32); 	//Ub Ub
LONG* 	RM_pnULc 	= 	(LONG*)(RM_BUF_ADDR + 36); 	//Uc Uc
LONG* 	RM_pnULab 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// Uab
LONG* 	RM_pnULbc 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Ubc
LONG* 	RM_pnULca 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Uca
LONG* 	RM_pnU0L 	= 	(LONG*)(RM_BUF_ADDR + 52); 	//U0L U0L
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 56); 	//IA IA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 60); 	//IB IB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 64); 	//IC IC
LONG* 	RM_pnILa 	= 	(LONG*)(RM_BUF_ADDR + 68); 	//Ia Ia
LONG* 	RM_pnILb 	= 	(LONG*)(RM_BUF_ADDR + 72); 	//Ib Ib
LONG* 	RM_pnILc 	= 	(LONG*)(RM_BUF_ADDR + 76); 	//Ic Ic
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 80); 	//I0 I0
LONG* 	RM_pnIjx 	= 	(LONG*)(RM_BUF_ADDR + 84); 	//Ijx Ijx
LONG* 	RM_pnU2H 	= 	(LONG*)(RM_BUF_ADDR + 88); 	//U2H U2H
LONG* 	RM_pnU2L 	= 	(LONG*)(RM_BUF_ADDR + 92); 	//U2L U2L
LONG* 	RM_pnIDA 	= 	(LONG*)(RM_BUF_ADDR + 96); 	//IDA IDA
LONG* 	RM_pnIDB 	= 	(LONG*)(RM_BUF_ADDR + 100); 	//IDB IDB
LONG* 	RM_pnIDC 	= 	(LONG*)(RM_BUF_ADDR + 104); 	//IDC IDC
LONG* 	RM_pnIRA 	= 	(LONG*)(RM_BUF_ADDR + 108); 	//IRA IRA
LONG* 	RM_pnIRB 	= 	(LONG*)(RM_BUF_ADDR + 112); 	//IRB IRB
LONG* 	RM_pnIRC 	= 	(LONG*)(RM_BUF_ADDR + 116); 	//IRC IRC
LONG* 	RM_pnIDA2 	= 	(LONG*)(RM_BUF_ADDR + 120); 	//IDA2 IDA2
LONG* 	RM_pnIDA3 	= 	(LONG*)(RM_BUF_ADDR + 124); 	//IDA3 IDA3
LONG* 	RM_pnIDA5 	= 	(LONG*)(RM_BUF_ADDR + 128); 	//IDA5 IDA5
LONG* 	RM_pnIDB2 	= 	(LONG*)(RM_BUF_ADDR + 132); 	//IDB2 IDB2
LONG* 	RM_pnIDB3 	= 	(LONG*)(RM_BUF_ADDR + 136); 	//IDB3 IDB3
LONG* 	RM_pnIDB5 	= 	(LONG*)(RM_BUF_ADDR + 140); 	//IDB5 IDB5
LONG* 	RM_pnIDC2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	//IDC2 IDC2
LONG* 	RM_pnIDC3 	= 	(LONG*)(RM_BUF_ADDR + 148); 	//IDC3 IDC3
LONG* 	RM_pnIDC5 	= 	(LONG*)(RM_BUF_ADDR + 152); 	//IDC5 IDC5
LONG* 	RM_pnIRA2 	= 	(LONG*)(RM_BUF_ADDR + 156); 	//IRA2 IRA2
LONG* 	RM_pnIRB2 	= 	(LONG*)(RM_BUF_ADDR + 160); 	//IRB2 IRB2
LONG* 	RM_pnIRC2 	= 	(LONG*)(RM_BUF_ADDR + 164); 	//IRC2 IRC2
LONG* 	RM_pnAUHA 	= 	(LONG*)(RM_BUF_ADDR + 168); 	//AUA ��UA
LONG* 	RM_pnAUHB 	= 	(LONG*)(RM_BUF_ADDR + 172); 	//AUB ��UB
LONG* 	RM_pnAUHC 	= 	(LONG*)(RM_BUF_ADDR + 176); 	//AUC ��UC
LONG* 	RM_pnAULa 	= 	(LONG*)(RM_BUF_ADDR + 180); 	//AUa ��Ua
LONG* 	RM_pnAULb 	= 	(LONG*)(RM_BUF_ADDR + 184); 	//AUb ��Ub
LONG* 	RM_pnAULc 	= 	(LONG*)(RM_BUF_ADDR + 188); 	//AUc ��Uc
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 192); 	//AIA ��IA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 196); 	//AIB ��IB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 200); 	//AIC ��IC
LONG* 	RM_pnAILa 	= 	(LONG*)(RM_BUF_ADDR + 204); 	//AIa ��Ia
LONG* 	RM_pnAILb 	= 	(LONG*)(RM_BUF_ADDR + 208); 	//AIb ��Ib
LONG* 	RM_pnAILc 	= 	(LONG*)(RM_BUF_ADDR + 212); 	//AIc ��Ic
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 216); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 220); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 224); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 228); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnU0h 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0h U0h
LONG* 	M_pnUla 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Ua Ua
LONG* 	M_pnUlb 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ub Ub
LONG* 	M_pnUlc 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Uc Uc
LONG* 	M_pnU0l 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//U0l U0l
LONG* 	M_pnIHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IA IA
LONG* 	M_pnIHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IB IB
LONG* 	M_pnIHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//IC IC
LONG* 	M_pnIla 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Ia Ia
LONG* 	M_pnIlb 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ib Ib
LONG* 	M_pnIlc 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//Ic Ic
LONG* 	M_pnI0 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I0 I0
LONG* 	M_pnIjx 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//Ijx Ijx
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//COS COS


//��ѹ��----------------------------------
BYTE* SW_pby87U 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//��ٶϱ���
BYTE* SW_pby87R 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//���ʲ����
BYTE* SW_pby50I_H 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//��ѹ���������
BYTE* SW_pby50I_L 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//��ѹ���������
BYTE* SW_pby50I_SH 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//ĸ�߳�籣��
BYTE* SW_pby50N 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//�����������
BYTE* SW_pby59N 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//�����ѹ����
BYTE* SW_pby50OL 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//�����ɱ���
BYTE* SW_pby50I_JX 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//��϶��������
BYTE* SW_pby49I 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//��ʱ�޹�����
BYTE* SW_pby27U 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//ʧѹ����
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 13); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby87U 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//�����ٶϱ���
BYTE* CFG_pby87R 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//���ʲ����
BYTE* CFG_pby68IDH2MAXLK 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//����г����������
BYTE* CFG_pby68IDH2MULLK 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//����г���ۺ������
BYTE* CFG_pby68IDH2SPLLK 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//����г���������
BYTE* CFG_pby68IRH2LK 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//�ƶ���������г������
BYTE* CFG_pby68IDMULLK 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//�ۺ�г������
BYTE* CFG_pbyHCT1CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//��ѹ��CT1��У��
BYTE* CFG_pbyHCT11CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//��ѹ��CT11��У��
BYTE* CFG_pbyLCT1CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//��ѹ��CT1��У��
BYTE* CFG_pbyLCT11CLOCK 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//��ѹ��CT11��У��
BYTE* CFG_pbyHAB 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//��ѹ������
BYTE* CFG_pby50I_H1 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//��ѹ��������
BYTE* CFG_pby50I_H1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//��ѹ�������ι��ʷ���
BYTE* CFG_pby50I_H1_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//��ѹ�������θ�ѹ����
BYTE* CFG_pby50I_H2 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//��ѹ��������
BYTE* CFG_pby50I_H2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//��ѹ�������ι��ʷ���
BYTE* CFG_pby50I_H2_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//��ѹ�������θ�ѹ����
BYTE* CFG_pby50I_H3 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//��ѹ��������
BYTE* CFG_pby50I_H3_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//��ѹ�������ι��ʷ���
BYTE* CFG_pby50I_H3_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//��ѹ�������θ�ѹ����
BYTE* CFG_pby50I_L1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//��ѹ��������
BYTE* CFG_pby50I_L1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//��ѹ�������ι��ʷ���
BYTE* CFG_pby50I_L1_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//��ѹ�������θ�ѹ����
BYTE* CFG_pby50I_L2 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//��ѹ��������
BYTE* CFG_pby50I_L2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//��ѹ�������ι��ʷ���
BYTE* CFG_pby50I_L2_47U 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//��ѹ�������θ�ѹ����
BYTE* CFG_pby50I_SH 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//ĸ�߳�籣��
BYTE* CFG_pby49I 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//��ʱ�޹����ɱ���
BYTE* CFG_pby50I_N1 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//����������
BYTE* CFG_pby50I_N2 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//����������
BYTE* CFG_pby59U_NH 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//��ѹ�������ѹ����
BYTE* CFG_pby59U_NL 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//��ѹ�������ѹ�澯
BYTE* CFG_pby50OL_1 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//�����ɢ��
BYTE* CFG_pby50OL_2 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//�����ɢ��
BYTE* CFG_pby50OL_3 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//�����ɢ��
BYTE* CFG_pby50OL_A 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//�����ɸ澯
BYTE* CFG_pby50I_JX 	= 	(BYTE*)(CFG_BUF_ADDR + 37); 	//��϶��������
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 38); 	//ʧѹ����
BYTE* CFG_pby27U_50I 	= 	(BYTE*)(CFG_BUF_ADDR + 39); 	//ʧѹ������������
BYTE* CFG_pbyPTCK 	= 	(BYTE*)(CFG_BUF_ADDR + 40); 	//PT���߼��
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 41); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 42); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 43); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnFUNCTION1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������1 FUNCTION1
LONG* RS_pnFUNCTION2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//������2 FUNCTION2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//ͻ������������ PIC
LONG* RS_pn87UC 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//��ٶϵ��� 87UC
LONG* RS_pn87RC 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//��ż����� 87RC
LONG* RS_pnIRS1 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//�����ƶ��յ�1 IRS1
LONG* RS_pnIRS2 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//�����ƶ��յ�2 IRS2
LONG* RS_pnSLP1 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//�����ƶ�ϵ��1 SLP1
LONG* RS_pnSLP2 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//�����ƶ�ϵ��2 SLP2
LONG* RS_pnkrh2 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//����г���ƶ�ϵ�� krh2
LONG* RS_pnkrh 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//�ۺ�г���ƶ�ϵ�� krh
LONG* RS_pnkb 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//ƽ��ϵ�� kb
LONG* RS_pn50IH1C 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//��ѹ�������ε��� 50IH1C
LONG* RS_pn50IH1D 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//��ѹ��������ʱ�� 50IH1D
LONG* RS_pn50IH2C 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//��ѹ�������ε��� 50IH2C
LONG* RS_pn50IH2D 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//��ѹ��������ʱ�� 50IH2D
LONG* RS_pn50IH3C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//��ѹ�������ε��� 50IH3C
LONG* RS_pn50IH3D 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//��ѹ��������ʱ�� 50IH3D
LONG* RS_pn50IL1C 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//��ѹ�������ε��� 50IL1C
LONG* RS_pn50IL1D 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//��ѹ��������ʱ�� 50IL1D
LONG* RS_pn50IL2C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//��ѹ�������ε��� 50IL2C
LONG* RS_pn50IL2D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//��ѹ��������ʱ�� 50IL2D
LONG* RS_pn47U_27U1 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//��ѹ�����͵�ѹ 47U_27U1
LONG* RS_pn47U_59U2 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//��ѹ���������ѹ 47U_59U2
LONG* RS_pnA_50I 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//�������������� A_50I
LONG* RS_pn50ISHC 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//ĸ�䱣������ 50ISHC
LONG* RS_pn50ISHD 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//ĸ�䱣��ʱ�� 50ISHD
LONG* RS_pn50ISHT 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//ĸ�䱣��Ͷ��ʱ�� 50ISHT
LONG* RS_pn50IN1C 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//���������ε��� 50IN1C
LONG* RS_pn50IN1D 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//����������ʱ�� 50IN1D
LONG* RS_pn50IN2C 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//���������ε��� 50IN2C
LONG* RS_pn50IN2D 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//����������ʱ�� 50IN2D
LONG* RS_pn59UNHV 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//��ѹ�������ѹ��ѹ 59UNHV
LONG* RS_pn59UNHD 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//��ѹ�������ѹʱ�� 59UNHD
LONG* RS_pn59UNLV 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//��ѹ����ѹ�澯��ѹ 59UNLV
LONG* RS_pn59UNLD 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//��ѹ����ѹ�澯ʱ�� 59UNLD
LONG* RS_pn50OL1C 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//�����ɢ�ε��� 50OL1C
LONG* RS_pn50OL1D 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//�����ɢ��ʱ�� 50OL1D
LONG* RS_pn50OL2C 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//�����ɢ�ε��� 50OL2C
LONG* RS_pn50OL2D 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//�����ɢ��ʱ�� 50OL2D
LONG* RS_pn50OL3C 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//�����ɢ�ε��� 50OL3C
LONG* RS_pn50OL3D 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//�����ɢ��ʱ�� 50OL3D
LONG* RS_pn50OLAC 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//�����ɸ澯���� 50OLAC
LONG* RS_pn50OLAD 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//�����ɸ澯ʱ�� 50OLAD
LONG* RS_pn50I_JXC 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//��϶�������� 50I_JXC
LONG* RS_pn50I_JXD 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//��϶����ʱ�� 50I_JXD
LONG* RS_pnRAC 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//��ѹ�����ζ���� RAC
LONG* RS_pn49K 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//�ȹ����ɱ������� 49K
LONG* RS_pn49T 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//�ȹ�����ʱ�䳣�� 49T
LONG* RS_pn49AR 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//�ȹ����ɸ澯���� 49AR
LONG* RS_pn49KT 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//�豸ֹͣ����ϵ�� 49KT
LONG* RS_pn27V 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//ʧѹ������ѹ 27V
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//ʧѹ����ʱ�� 27D
LONG* RS_pn27U_50I 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//ʧѹ��������������ֵ 27U_50I
LONG* RS_pn74VTC 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//PT���߼����� 74VTC
LONG* RS_pn74VTV 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//PT���߼���ѹ 74VTV
LONG* RS_pn74VTD 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//PT���߼��ʱ�� 74VTD
LONG* RS_pnK87AR 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//����Խ�ޱ������� K87AR
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//ʧ�鱣��ʱ�� 52BFD

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
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//��·��������ʱ
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//�綯����������ʱ
LONG* AS_pn50IAC 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//IA������ֵ
LONG* AS_pn37IAC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IA������ֵ
LONG* AS_pn50IBC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IB������ֵ
LONG* AS_pn37IBC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//IB������ֵ
LONG* AS_pn50ICC 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IC������ֵ
LONG* AS_pn37ICC 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//IC������ֵ
LONG* AS_pn50IaC 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//Ia������ֵ
LONG* AS_pn37IaC 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//Ia������ֵ
LONG* AS_pn50IbC 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//Ib������ֵ
LONG* AS_pn37IbC 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//Ib������ֵ
LONG* AS_pn50IcC 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//Ic������ֵ
LONG* AS_pn37IcC 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//Ic������ֵ
LONG* AS_pn50I0C 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//I0������ֵ
LONG* AS_pn37I0C 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//I0������ֵ
LONG* AS_pn50IjxC 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//Ijx������ֵ
LONG* AS_pn37IjxC 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//Ijx������ֵ
LONG* AS_pn59UAV 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//UA��ѹ��ֵ
LONG* AS_pn27UAV 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//UA��ѹ��ֵ
LONG* AS_pn59UBV 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//UB��ѹ��ֵ
LONG* AS_pn27UBV 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//UB��ѹ��ֵ
LONG* AS_pn59UCV 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//UC��ѹ��ֵ
LONG* AS_pn27UCV 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//UC��ѹ��ֵ
LONG* AS_pn59U0hV 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//U0h��ѹ��ֵ
LONG* AS_pn27U0hV 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//U0h��ѹ��ֵ
LONG* AS_pn59UaV 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//Ua��ѹ��ֵ
LONG* AS_pn27UaV 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//Ua��ѹ��ֵ
LONG* AS_pn59UbV 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//Ub��ѹ��ֵ
LONG* AS_pn27UbV 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//Ub��ѹ��ֵ
LONG* AS_pn59UcV 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//Uc��ѹ��ֵ
LONG* AS_pn27UcV 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//Uc��ѹ��ֵ
LONG* AS_pn59U0lV 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//U0l��ѹ��ֵ
LONG* AS_pn27U0lV 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//U0l��ѹ��ֵ
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//TR2����ʱ��
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//TR3����ʱ��
LONG* AS_pnTR4D 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//TR4����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 284); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnHPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 288); 	//HPT���
LONG* AS_pnH0PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 292); 	//H0PT���
LONG* AS_pnLPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 296); 	//LPT���
LONG* AS_pnL0PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 300); 	//L0PT���
LONG* AS_pnHCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 304); 	//HCT���
LONG* AS_pnLCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 308); 	//LCT���
LONG* AS_pnL0CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 312); 	//L0CT���
LONG* AS_pnJXCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 316); 	//JXCT���
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 320); 	//rt1

//ϵ��----------------------------------
LONG* CF_pnUp 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//Up
LONG* CF_pnU0h 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U0h
LONG* CF_pnU0l 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U0l
LONG* CF_pnIp 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//Ip
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//I0
LONG* CF_pnIjx 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//Ijx
LONG* CF_pnId 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//Id
LONG* CF_pnUAM 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UAM
LONG* CF_pnUBM 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//UBM
LONG* CF_pnUCM 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//UCM
LONG* CF_pnU0hM 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//U0hM
LONG* CF_pnUaM 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//UaM
LONG* CF_pnUbM 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//UbM
LONG* CF_pnUcM 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//UcM
LONG* CF_pnU0lM 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//U0lM
LONG* CF_pnIAM 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//IAM
LONG* CF_pnIBM 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IBM
LONG* CF_pnICM 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//ICM
LONG* CF_pnIaM 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//IaM
LONG* CF_pnIbM 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//IbM
LONG* CF_pnIcM 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//IcM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//I0M
LONG* CF_pnIjxM 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//IjxM
LONG* CF_pnUAD 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//UAD
LONG* CF_pnUBD 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//UBD
LONG* CF_pnUCD 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//UCD
LONG* CF_pnU0hD 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//U0hD
LONG* CF_pnUaD 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//UaD
LONG* CF_pnUbD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//UbD
LONG* CF_pnUcD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//UcD
LONG* CF_pnU0lD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//U0lD
LONG* CF_pnIAD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IAD
LONG* CF_pnIBD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IBD
LONG* CF_pnICD 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//ICD
LONG* CF_pnIaD 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//IaD
LONG* CF_pnIbD 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//IbD
LONG* CF_pnIcD 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//IcD
LONG* CF_pnI0D 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//I0D
LONG* CF_pnIjxD 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//IjxD
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//C4
LONG* CF_pnPA 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//PA
LONG* CF_pnQA 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//QA
LONG* CF_pnPB 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//PB
LONG* CF_pnQB 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//QB
LONG* CF_pnPC 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//PC
LONG* CF_pnQC 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//QC
LONG* CF_pnAA 	= 	(LONG*)(CF_BUF_ADDR + 196); 	//AA
LONG* CF_pnBA 	= 	(LONG*)(CF_BUF_ADDR + 200); 	//BA
LONG* CF_pnCA 	= 	(LONG*)(CF_BUF_ADDR + 204); 	//CA

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 224), // IN1
    (LONG*)(RM_BUF_ADDR + 228) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 216), // BR1
    (LONG*)(RM_BUF_ADDR + 220) // BR2
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

//�߼����̹��ϻָ�63----------
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

//�¶ȸ澯506----------
const  LONG* FailureInf506[] = {
    {NULL}
};

//��������2800----------
const  LONG* FailureInf2800[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ��IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ��IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ��IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ��Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ��Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ��Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 156), //IRA2 IRA2
    (LONG*)(RM_BUF_ADDR + 160), //IRB2 IRB2
    (LONG*)(RM_BUF_ADDR + 164), //IRC2 IRC2
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ٶ�����(A��)2801----------
const  LONG* FailureInf2801[] = {
    {NULL}
};

//��ٶ�����(B��)2802----------
const  LONG* FailureInf2802[] = {
    {NULL}
};

//��ٶ�����(C��)2803----------
const  LONG* FailureInf2803[] = {
    {NULL}
};

//��ٶϱ�������2804----------
const  LONG* FailureInf2804[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ��IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ��IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ��IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ��Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ��Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ��Ic
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 156), //IRA2 IRA2
    (LONG*)(RM_BUF_ADDR + 160), //IRB2 IRB2
    (LONG*)(RM_BUF_ADDR + 164), //IRC2 IRC2
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8) //UC UC
};

//��ٶϷ���(A��)2805----------
const  LONG* FailureInf2805[] = {
    {NULL}
};

//��ٶϷ���(B��)2806----------
const  LONG* FailureInf2806[] = {
    {NULL}
};

//��ٶϷ���(C��)2807----------
const  LONG* FailureInf2807[] = {
    {NULL}
};

//���ʲ����(A��)2808----------
const  LONG* FailureInf2808[] = {
    {NULL}
};

//���ʲ����(B��)2809----------
const  LONG* FailureInf2809[] = {
    {NULL}
};

//���ʲ����(C��)2810----------
const  LONG* FailureInf2810[] = {
    {NULL}
};

//���ʲ��������2811----------
const  LONG* FailureInf2811[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ��IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ��IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ��IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ��Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ��Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ��Ic
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 156), //IRA2 IRA2
    (LONG*)(RM_BUF_ADDR + 160), //IRB2 IRB2
    (LONG*)(RM_BUF_ADDR + 164), //IRC2 IRC2
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8) //UC UC
};

//���ʲ����(A��)2812----------
const  LONG* FailureInf2812[] = {
    {NULL}
};

//���ʲ����(B��)2813----------
const  LONG* FailureInf2813[] = {
    {NULL}
};

//���ʲ����(C��)2814----------
const  LONG* FailureInf2814[] = {
    {NULL}
};

//��ѹ������������2815----------
const  LONG* FailureInf2815[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ѹ�������ζ���2816----------
const  LONG* FailureInf2816[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������η���2817----------
const  LONG* FailureInf2817[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ѹ������������2818----------
const  LONG* FailureInf2818[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ѹ�������ζ���2819----------
const  LONG* FailureInf2819[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������η���2820----------
const  LONG* FailureInf2820[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ѹ������������2821----------
const  LONG* FailureInf2821[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ѹ�������ζ���2822----------
const  LONG* FailureInf2822[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������η���2823----------
const  LONG* FailureInf2823[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��ѹ������������2824----------
const  LONG* FailureInf2824[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//��ѹ�������ζ���2825----------
const  LONG* FailureInf2825[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������η���2826----------
const  LONG* FailureInf2826[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//��ѹ������������2827----------
const  LONG* FailureInf2827[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//��ѹ�������ζ���2828----------
const  LONG* FailureInf2828[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������η���2829----------
const  LONG* FailureInf2829[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//ĸ�߳�籣������2830----------
const  LONG* FailureInf2830[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76) //Ic Ic
};

//ĸ�߳�籣������2831----------
const  LONG* FailureInf2831[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//ĸ�߳�籣������2832----------
const  LONG* FailureInf2832[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76) //Ic Ic
};

//��ʱ�޹����ɸ澯2833----------
const  LONG* FailureInf2833[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ʱ�޹����ɶ���2834----------
const  LONG* FailureInf2834[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ʱ�޹����ɷ���2835----------
const  LONG* FailureInf2835[] = {
    {NULL}
};

//��������������2836----------
const  LONG* FailureInf2836[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//���������ζ���2837----------
const  LONG* FailureInf2837[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//���������η���2838----------
const  LONG* FailureInf2838[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//��������������2839----------
const  LONG* FailureInf2839[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//���������ζ���2840----------
const  LONG* FailureInf2840[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//���������η���2841----------
const  LONG* FailureInf2841[] = {
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//��ѹ�������ѹ����2842----------
const  LONG* FailureInf2842[] = {
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//��ѹ�������ѹ����2843----------
const  LONG* FailureInf2843[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������ѹ����2844----------
const  LONG* FailureInf2844[] = {
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//�����ɢ������2845----------
const  LONG* FailureInf2845[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//�����ɢ�ζ���2846----------
const  LONG* FailureInf2846[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//�����ɢ�η���2847----------
const  LONG* FailureInf2847[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//�����ɢ������2848----------
const  LONG* FailureInf2848[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//�����ɢ�ζ���2849----------
const  LONG* FailureInf2849[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//�����ɢ�η���2850----------
const  LONG* FailureInf2850[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//�����ɢ������2851----------
const  LONG* FailureInf2851[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//�����ɢ�ζ���2852----------
const  LONG* FailureInf2852[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//�����ɢ�η���2853----------
const  LONG* FailureInf2853[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64) //IC IC
};

//��϶������������2854----------
const  LONG* FailureInf2854[] = {
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//��϶������������2855----------
const  LONG* FailureInf2855[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��϶������������2856----------
const  LONG* FailureInf2856[] = {
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 24) //U0H U0H
};

//ʧѹ����2857----------
const  LONG* FailureInf2857[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//��ѹ�������ѹ�澯2858----------
const  LONG* FailureInf2858[] = {
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//����Խ��2859----------
const  LONG* FailureInf2859[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ��IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ��IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ��IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ��Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ��Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ��Ic
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 120), //IDA2 IDA2
    (LONG*)(RM_BUF_ADDR + 124), //IDA3 IDA3
    (LONG*)(RM_BUF_ADDR + 128), //IDA5 IDA5
    (LONG*)(RM_BUF_ADDR + 132), //IDB2 IDB2
    (LONG*)(RM_BUF_ADDR + 136), //IDB3 IDB3
    (LONG*)(RM_BUF_ADDR + 140), //IDB5 IDB5
    (LONG*)(RM_BUF_ADDR + 144), //IDC2 IDC2
    (LONG*)(RM_BUF_ADDR + 148), //IDC3 IDC3
    (LONG*)(RM_BUF_ADDR + 152), //IDC5 IDC5
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36) //Uc Uc
};

//��ѹ��PT����2860----------
const  LONG* FailureInf2860[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 12), //UAB UAB
    (LONG*)(RM_BUF_ADDR + 16), //UBC UBC
    (LONG*)(RM_BUF_ADDR + 20), //UCA UCA
    (LONG*)(RM_BUF_ADDR + 88) //U2H U2H
};

//��ѹ��PT����2861----------
const  LONG* FailureInf2861[] = {
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 40), // Uab
    (LONG*)(RM_BUF_ADDR + 44), // Ubc
    (LONG*)(RM_BUF_ADDR + 48), // Uca
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 92) //U2L U2L
};

//��·����բʧ��2862----------
const  LONG* FailureInf2862[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84) //Ijx Ijx
};

//ͨ������2863----------
const  LONG* FailureInf2863[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8), //UC UC
    (LONG*)(RM_BUF_ADDR + 24), //U0H U0H
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36), //Uc Uc
    (LONG*)(RM_BUF_ADDR + 52), //U0L U0L
    (LONG*)(RM_BUF_ADDR + 88), //U2H U2H
    (LONG*)(RM_BUF_ADDR + 92), //U2L U2L
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 80), //I0 I0
    (LONG*)(RM_BUF_ADDR + 84), //Ijx Ijx
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116), //IRC IRC
    (LONG*)(RM_BUF_ADDR + 168), //AUA ��UA
    (LONG*)(RM_BUF_ADDR + 172), //AUB ��UB
    (LONG*)(RM_BUF_ADDR + 176), //AUC ��UC
    (LONG*)(RM_BUF_ADDR + 180), //AUa ��Ua
    (LONG*)(RM_BUF_ADDR + 184), //AUb ��Ub
    (LONG*)(RM_BUF_ADDR + 188), //AUc ��Uc
    (LONG*)(RM_BUF_ADDR + 192), //AIA ��IA
    (LONG*)(RM_BUF_ADDR + 196), //AIB ��IB
    (LONG*)(RM_BUF_ADDR + 200), //AIC ��IC
    (LONG*)(RM_BUF_ADDR + 204), //AIa ��Ia
    (LONG*)(RM_BUF_ADDR + 208), //AIb ��Ib
    (LONG*)(RM_BUF_ADDR + 212) //AIc ��Ic
};

//��ѹ��PT���߻ָ�2864----------
const  LONG* FailureInf2864[] = {
    (LONG*)(RM_BUF_ADDR + 0), //UA UA
    (LONG*)(RM_BUF_ADDR + 4), //UB UB
    (LONG*)(RM_BUF_ADDR + 8) //UC UC
};

//��ѹ��PT���߻ָ�2865----------
const  LONG* FailureInf2865[] = {
    (LONG*)(RM_BUF_ADDR + 28), //Ua Ua
    (LONG*)(RM_BUF_ADDR + 32), //Ub Ub
    (LONG*)(RM_BUF_ADDR + 36) //Uc Uc
};

//����Խ�޻ָ�2866----------
const  LONG* FailureInf2866[] = {
    (LONG*)(RM_BUF_ADDR + 56), //IA IA
    (LONG*)(RM_BUF_ADDR + 192), //AIA ��IA
    (LONG*)(RM_BUF_ADDR + 60), //IB IB
    (LONG*)(RM_BUF_ADDR + 196), //AIB ��IB
    (LONG*)(RM_BUF_ADDR + 64), //IC IC
    (LONG*)(RM_BUF_ADDR + 200), //AIC ��IC
    (LONG*)(RM_BUF_ADDR + 68), //Ia Ia
    (LONG*)(RM_BUF_ADDR + 204), //AIa ��Ia
    (LONG*)(RM_BUF_ADDR + 72), //Ib Ib
    (LONG*)(RM_BUF_ADDR + 208), //AIb ��Ib
    (LONG*)(RM_BUF_ADDR + 76), //Ic Ic
    (LONG*)(RM_BUF_ADDR + 212), //AIc ��Ic
    (LONG*)(RM_BUF_ADDR + 96), //IDA IDA
    (LONG*)(RM_BUF_ADDR + 100), //IDB IDB
    (LONG*)(RM_BUF_ADDR + 104), //IDC IDC
    (LONG*)(RM_BUF_ADDR + 108), //IRA IRA
    (LONG*)(RM_BUF_ADDR + 112), //IRB IRB
    (LONG*)(RM_BUF_ADDR + 116) //IRC IRC
};

//���������쳣22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//���������쳣�ָ�65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//�����ɸ澯2867----------
const  LONG* FailureInf2867[] = {
    {NULL}
};

//�����ɸ澯����2868----------
const  LONG* FailureInf2868[] = {
    {NULL}
};

//�����ɸ澯����2869----------
const  LONG* FailureInf2869[] = {
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
    {0, FailureInf63, 63},//�߼����̹��ϻָ�
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf500, 500},//��������
    {0, FailureInf501, 501},//����˹
    {0, FailureInf502, 502},//ѹ���ͷ�
    {0, FailureInf503, 503},//������բ
    {0, FailureInf504, 504},//����˹
    {0, FailureInf505, 505},//��λ�쳣
    {0, FailureInf506, 506},//�¶ȸ澯
    {45, FailureInf2800, 2800},//��������
    {0, FailureInf2801, 2801},//��ٶ�����(A��)
    {0, FailureInf2802, 2802},//��ٶ�����(B��)
    {0, FailureInf2803, 2803},//��ٶ�����(C��)
    {33, FailureInf2804, 2804},//��ٶϱ�������
    {0, FailureInf2805, 2805},//��ٶϷ���(A��)
    {0, FailureInf2806, 2806},//��ٶϷ���(B��)
    {0, FailureInf2807, 2807},//��ٶϷ���(C��)
    {0, FailureInf2808, 2808},//���ʲ����(A��)
    {0, FailureInf2809, 2809},//���ʲ����(B��)
    {0, FailureInf2810, 2810},//���ʲ����(C��)
    {33, FailureInf2811, 2811},//���ʲ��������
    {0, FailureInf2812, 2812},//���ʲ����(A��)
    {0, FailureInf2813, 2813},//���ʲ����(B��)
    {0, FailureInf2814, 2814},//���ʲ����(C��)
    {8, FailureInf2815, 2815},//��ѹ������������
    {21, FailureInf2816, 2816},//��ѹ�������ζ���
    {8, FailureInf2817, 2817},//��ѹ�������η���
    {8, FailureInf2818, 2818},//��ѹ������������
    {21, FailureInf2819, 2819},//��ѹ�������ζ���
    {8, FailureInf2820, 2820},//��ѹ�������η���
    {8, FailureInf2821, 2821},//��ѹ������������
    {21, FailureInf2822, 2822},//��ѹ�������ζ���
    {8, FailureInf2823, 2823},//��ѹ�������η���
    {8, FailureInf2824, 2824},//��ѹ������������
    {21, FailureInf2825, 2825},//��ѹ�������ζ���
    {8, FailureInf2826, 2826},//��ѹ�������η���
    {8, FailureInf2827, 2827},//��ѹ������������
    {21, FailureInf2828, 2828},//��ѹ�������ζ���
    {8, FailureInf2829, 2829},//��ѹ�������η���
    {3, FailureInf2830, 2830},//ĸ�߳�籣������
    {21, FailureInf2831, 2831},//ĸ�߳�籣������
    {3, FailureInf2832, 2832},//ĸ�߳�籣������
    {21, FailureInf2833, 2833},//��ʱ�޹����ɸ澯
    {21, FailureInf2834, 2834},//��ʱ�޹����ɶ���
    {0, FailureInf2835, 2835},//��ʱ�޹����ɷ���
    {2, FailureInf2836, 2836},//��������������
    {21, FailureInf2837, 2837},//���������ζ���
    {2, FailureInf2838, 2838},//���������η���
    {2, FailureInf2839, 2839},//��������������
    {21, FailureInf2840, 2840},//���������ζ���
    {2, FailureInf2841, 2841},//���������η���
    {1, FailureInf2842, 2842},//��ѹ�������ѹ����
    {21, FailureInf2843, 2843},//��ѹ�������ѹ����
    {1, FailureInf2844, 2844},//��ѹ�������ѹ����
    {3, FailureInf2845, 2845},//�����ɢ������
    {21, FailureInf2846, 2846},//�����ɢ�ζ���
    {3, FailureInf2847, 2847},//�����ɢ�η���
    {3, FailureInf2848, 2848},//�����ɢ������
    {21, FailureInf2849, 2849},//�����ɢ�ζ���
    {3, FailureInf2850, 2850},//�����ɢ�η���
    {3, FailureInf2851, 2851},//�����ɢ������
    {21, FailureInf2852, 2852},//�����ɢ�ζ���
    {3, FailureInf2853, 2853},//�����ɢ�η���
    {2, FailureInf2854, 2854},//��϶������������
    {21, FailureInf2855, 2855},//��϶������������
    {2, FailureInf2856, 2856},//��϶������������
    {21, FailureInf2857, 2857},//ʧѹ����
    {10, FailureInf2858, 2858},//��ѹ�������ѹ�澯
    {33, FailureInf2859, 2859},//����Խ��
    {10, FailureInf2860, 2860},//��ѹ��PT����
    {10, FailureInf2861, 2861},//��ѹ��PT����
    {8, FailureInf2862, 2862},//��·����բʧ��
    {36, FailureInf2863, 2863},//ͨ������
    {3, FailureInf2864, 2864},//��ѹ��PT���߻ָ�
    {3, FailureInf2865, 2865},//��ѹ��PT���߻ָ�
    {18, FailureInf2866, 2866},//����Խ�޻ָ�
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
    {0, FailureInf2867, 2867},//�����ɸ澯
    {0, FailureInf2868, 2868},//�����ɸ澯����
    {0, FailureInf2869, 2869},//�����ɸ澯����
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 108) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 232) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 268) ,
	 (LONG*)(AS_BUF_ADDR + 272) ,
	 (LONG*)(AS_BUF_ADDR + 276) ,
	 (LONG*)(AS_BUF_ADDR + 280) ,
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
	 (WORD)(66) ,
	 (WORD)(67) ,
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
	 (BYTE*)(SW_BUF_ADDR + 13) ,
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
	 (WORD)(13) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 37) ,
	 (BYTE*)(CFG_BUF_ADDR + 38) ,
	 (BYTE*)(CFG_BUF_ADDR + 39) ,
	 (BYTE*)(CFG_BUF_ADDR + 40) ,
	 (BYTE*)(CFG_BUF_ADDR + 41) ,
	 (BYTE*)(CFG_BUF_ADDR + 42) ,
	 (BYTE*)(CFG_BUF_ADDR + 43) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	30897, //Up
	11500, //U0h
	11500, //U0l
	3050, //Ip
	3050, //I0
	3050, //Ijx
	3050, //Id
	43689, //UAM
	43689, //UBM
	43689, //UCM
	16152, //U0hM
	43689, //UaM
	43689, //UbM
	43689, //UcM
	16152, //U0lM
	4310, //IAM
	4310, //IBM
	4310, //ICM
	4310, //IaM
	4310, //IbM
	4310, //IcM
	4310, //I0M
	4310, //IjxM
	20, //UAD
	20, //UBD
	20, //UCD
	20, //U0hD
	20, //UaD
	20, //UbD
	20, //UcD
	20, //U0lD
	20, //IAD
	20, //IBD
	20, //ICD
	20, //IaD
	20, //IbD
	20, //IcD
	20, //I0D
	20, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	30897, //Up
	11500, //U0h
	11500, //U0l
	15250, //Ip
	15250, //I0
	15250, //Ijx
	15250, //Id
	43689, //UAM
	43689, //UBM
	43689, //UCM
	16152, //U0hM
	43689, //UaM
	43689, //UbM
	43689, //UcM
	16152, //U0lM
	21550, //IAM
	21550, //IBM
	21550, //ICM
	21550, //IaM
	21550, //IbM
	21550, //IcM
	21550, //I0M
	21550, //IjxM
	20, //UAD
	20, //UBD
	20, //UCD
	20, //U0hD
	20, //UaD
	20, //UbD
	20, //UcD
	20, //U0lD
	20, //IAD
	20, //IBD
	20, //ICD
	20, //IaD
	20, //IbD
	20, //IcD
	20, //I0D
	20, //IjxD
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
};

//����----------------------------------
