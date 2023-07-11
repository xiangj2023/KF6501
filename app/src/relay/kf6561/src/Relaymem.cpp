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
};
//������ֵȡֵ��Ϣ
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{PHASEU, "57.7V"},
	{LINEU, "100V"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //������ֵ�����
	{2, (TSetValueInf*)&(SysSetValueInf[2])}, //������ֵ���ѹ
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnUa 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// Ua
LONG* 	RM_pnUb 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// Ub
LONG* 	RM_pnUc 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// Uc
LONG* 	RM_pnU0 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// U0
LONG* 	RM_pnUL1 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// UL1
LONG* 	RM_pnUL2 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// UL2
LONG* 	RM_pnUL3 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// UL3
LONG* 	RM_pnUab 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// Uab
LONG* 	RM_pnUbc 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// Ubc
LONG* 	RM_pnUca 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// Uca
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// U2
LONG* 	RM_pnU05 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// U05
LONG* 	RM_pnIa 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Ia
LONG* 	RM_pnIb 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// Ib
LONG* 	RM_pnIc 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// Ic
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// I0
LONG* 	RM_pnI1 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// I1
LONG* 	RM_pnI05 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// I05
LONG* 	RM_pnAUa 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// ��Ua
LONG* 	RM_pnAUb 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// ��Ub
LONG* 	RM_pnAUc 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// ��Uc
LONG* 	RM_pnAUab 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// ��Uab
LONG* 	RM_pnAUbc 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// ��Ubc
LONG* 	RM_pnAUca 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// ��Uca
LONG* 	RM_pnAUl1 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// ��Ul1
LONG* 	RM_pnAUl2 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// ��Ul2
LONG* 	RM_pnAUl3 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// ��Ul3
LONG* 	RM_pnAIpa 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// ��Ipa
LONG* 	RM_pnAIpb 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// ��Ipb
LONG* 	RM_pnAIpc 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// ��Ipc
LONG* 	RM_pnAIP1 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// ��Ip1
LONG* 	RM_pnAIma 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// ��Ima
LONG* 	RM_pnAImb 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// ��Imb
LONG* 	RM_pnAImc 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// ��Imc
LONG* 	RM_pnAU01 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// ��U01
LONG* 	RM_pnAI01 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// ��I01
LONG* 	RM_pnAU05 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// ��U05
LONG* 	RM_pnAI05 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// ��I05
LONG* 	RM_pnF1 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// F1
LONG* 	RM_pnIam 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// Iam
LONG* 	RM_pnIbm 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// Ibm
LONG* 	RM_pnIcm 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// Icm
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUa 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//Ua Ua
LONG* 	M_pnUb 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//Ub Ub
LONG* 	M_pnUc 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//Uc Uc
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnUab 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Uab Uab
LONG* 	M_pnUbc 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ubc Ubc
LONG* 	M_pnUca 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Uca Uca
LONG* 	M_pnU1 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//U1 U1
LONG* 	M_pnU2 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//U2 U2
LONG* 	M_pnU3 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//U3 U3
LONG* 	M_pnIa 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//Ia Ia
LONG* 	M_pnIb 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Ib Ib
LONG* 	M_pnIc 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ic Ic
LONG* 	M_pnI0 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I0 I0
LONG* 	M_pnI1 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I1 I1
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//COS COS
LONG* 	M_pnF1 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//F1 F1


//��ѹ��----------------------------------
BYTE* SW_pby50I0 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//�����ٶ�
BYTE* SW_pby50I1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//��������
BYTE* SW_pby59U 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//��ѹ����
BYTE* SW_pby27U 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//��ѹ����
BYTE* SW_pby60I 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//��ƽ�����
BYTE* SW_pby60U 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//��ƽ���ѹ
BYTE* SW_pbyJDXX 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//�ӵ�ѡ��1
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby50I0 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//�����ٶ�
BYTE* CFG_pby50I1 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//��������
BYTE* CFG_pby59U 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//��ѹ����
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//��ѹ����
BYTE* CFG_pby60I 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//��ƽ���������
BYTE* CFG_pby60U 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//��ƽ���ѹ����
BYTE* CFG_pby59N 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//��ѹ�澯
BYTE* CFG_pbyJDXX 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//�ӵ�ѡ��
BYTE* CFG_pbyXHXQ 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//������Ȧ�ӵ�
BYTE* CFG_pbyPTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//PT���߼��
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//CT���߼��
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnCFG 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ CFG
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//ͻ������������ PIC
LONG* RS_pn50I0C 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//�����ٶϵ��� 50I0C
LONG* RS_pn50I0D 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//�����ٶ�ʱ�� 50I0D
LONG* RS_pn50I1C 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//����������ֵ 50I1C
LONG* RS_pn50I1D 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//����ʱ�䶨ֵ 50I1D
LONG* RS_pn59U 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//��ѹ��ѹ��ֵ 59U
LONG* RS_pn59D 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//��ѹʱ�䶨ֵ 59D
LONG* RS_pn27U 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//��ѹ������ѹ 27U
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//��ѹ����ʱ�� 27D
LONG* RS_pn27ILO 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//��ѹ����������ֵ 27ILO
LONG* RS_pn60IC 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//��ƽ������������� 60IC
LONG* RS_pn60ID 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//��ƽ���������ʱ�� 60ID
LONG* RS_pn60UV 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//��ƽ���ѹ������ѹ 60UV
LONG* RS_pn60UD 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//��ƽ���ѹ����ʱ�� 60UD
LONG* RS_pn59NU 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//��ѹ�澯��ѹ 59NU
LONG* RS_pn59ND 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//��ѹ�澯ʱ�� 59ND
LONG* RS_pn74PTU 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//PT���߼���ѹ 74PTU
LONG* RS_pn74PTI 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//PT���߼����� 74PTI
LONG* RS_pn74PTD 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//PT���߼��ʱ�� 74PTD
LONG* RS_pn74PTU2 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//�����ѹ��ⶨֵ 74PTU2
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//ʧ�鱣��ʱ�� 52BFD

//������ֵ----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//�����
LONG* AS_pnPM 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//���ѹ
LONG* AS_pnT1PI 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//T1������ʱ
LONG* AS_pnT1RE 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//T1������ʱ
LONG* AS_pnT2PI 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//T2������ʱ
LONG* AS_pnT2RE 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//T2������ʱ
LONG* AS_pnT3PI 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//T3������ʱ
LONG* AS_pnT3RE 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//T3������ʱ
LONG* AS_pnT4PI 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//T4������ʱ
LONG* AS_pnT4RE 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//T4������ʱ
LONG* AS_pnT5PI 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//T5������ʱ
LONG* AS_pnT5RE 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//T5������ʱ
LONG* AS_pnT6PI 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//T6������ʱ
LONG* AS_pnT6RE 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//T6������ʱ
LONG* AS_pnT7PI 	= 	(LONG*)(AS_BUF_ADDR + 56); 	//T7������ʱ
LONG* AS_pnT7RE 	= 	(LONG*)(AS_BUF_ADDR + 60); 	//T7������ʱ
LONG* AS_pnT8PI 	= 	(LONG*)(AS_BUF_ADDR + 64); 	//T8������ʱ
LONG* AS_pnT8RE 	= 	(LONG*)(AS_BUF_ADDR + 68); 	//T8������ʱ
LONG* AS_pnT9PI 	= 	(LONG*)(AS_BUF_ADDR + 72); 	//T9������ʱ
LONG* AS_pnT9RE 	= 	(LONG*)(AS_BUF_ADDR + 76); 	//T9������ʱ
LONG* AS_pnT10PI 	= 	(LONG*)(AS_BUF_ADDR + 80); 	//T10������ʱ
LONG* AS_pnT10RE 	= 	(LONG*)(AS_BUF_ADDR + 84); 	//T10������ʱ
LONG* AS_pnT11PI 	= 	(LONG*)(AS_BUF_ADDR + 88); 	//T11������ʱ
LONG* AS_pnT11RE 	= 	(LONG*)(AS_BUF_ADDR + 92); 	//T11������ʱ
LONG* AS_pnT12PI 	= 	(LONG*)(AS_BUF_ADDR + 96); 	//T12������ʱ
LONG* AS_pnT12RE 	= 	(LONG*)(AS_BUF_ADDR + 100); 	//T12������ʱ
LONG* AS_pnT13PI 	= 	(LONG*)(AS_BUF_ADDR + 104); 	//T13������ʱ
LONG* AS_pnT13RE 	= 	(LONG*)(AS_BUF_ADDR + 108); 	//T13������ʱ
LONG* AS_pnT14PI 	= 	(LONG*)(AS_BUF_ADDR + 112); 	//T14������ʱ
LONG* AS_pnT14RE 	= 	(LONG*)(AS_BUF_ADDR + 116); 	//T14������ʱ
LONG* AS_pnT15PI 	= 	(LONG*)(AS_BUF_ADDR + 120); 	//T15������ʱ
LONG* AS_pnT15RE 	= 	(LONG*)(AS_BUF_ADDR + 124); 	//T15������ʱ
LONG* AS_pnT16PI 	= 	(LONG*)(AS_BUF_ADDR + 128); 	//T16������ʱ
LONG* AS_pnT16RE 	= 	(LONG*)(AS_BUF_ADDR + 132); 	//T16������ʱ
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//��·��������ʱ
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//���������ʱ
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//TR2����ʱ��
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//TR3����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//����¼������ʱ��
LONG* AS_pn59UA 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//Ua��ѹ��ֵ
LONG* AS_pn27UA 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//Ua��ѹ��ֵ
LONG* AS_pn59UB 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//Ub��ѹ��ֵ
LONG* AS_pn27UB 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//Ub��ѹ��ֵ
LONG* AS_pn59UC 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//Uc��ѹ��ֵ
LONG* AS_pn27UC 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//Uc��ѹ��ֵ
LONG* AS_pn59U0 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//U0��ѹ��ֵ
LONG* AS_pn27U0 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//U0��ѹ��ֵ
LONG* AS_pn59U1 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//U1��ѹ��ֵ
LONG* AS_pn27U1 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U1��ѹ��ֵ
LONG* AS_pn59U2 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U2��ѹ��ֵ
LONG* AS_pn27U2 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U2��ѹ��ֵ
LONG* AS_pn59U3 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//U3��ѹ��ֵ
LONG* AS_pn27U3 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//U3��ѹ��ֵ
LONG* AS_pn50IA 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//Ia������ֵ
LONG* AS_pn37IA 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//Ia������ֵ
LONG* AS_pn50IB 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//Ib������ֵ
LONG* AS_pn37IB 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//Ib������ֵ
LONG* AS_pn50IC 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//Ic������ֵ
LONG* AS_pn37IC 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//Ic������ֵ
LONG* AS_pn50I0 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//I0������ֵ
LONG* AS_pn37I0 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//I0������ֵ
LONG* AS_pn50I1 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//I1������ֵ
LONG* AS_pn37I1 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//I1������ֵ

//���----------------------------------
LONG* AS_pnPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//PT���
LONG* AS_pnPCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//����CT���
LONG* AS_pnMCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//����CT���
LONG* AS_pnZCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//����CT���
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//rt1

//ϵ��----------------------------------
LONG* CF_pnUp 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//Up
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U0
LONG* CF_pnIp 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//Ip
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//I0
LONG* CF_pnI1 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//I1
LONG* CF_pnUaM 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UaM
LONG* CF_pnUbM 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//UbM
LONG* CF_pnUcM 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UcM
LONG* CF_pnUL1M 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//UL1M
LONG* CF_pnUL2M 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//UL2M
LONG* CF_pnUL3M 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//UL3M
LONG* CF_pnU0M 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//U0M
LONG* CF_pnIAM 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IAM
LONG* CF_pnIBM 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IBM
LONG* CF_pnICM 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//ICM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//I0M
LONG* CF_pnI1M 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//I1M
LONG* CF_pnPA 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//PA
LONG* CF_pnQA 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//QA
LONG* CF_pnPB 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//PB
LONG* CF_pnQB 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//QB
LONG* CF_pnPC 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//PC
LONG* CF_pnQC 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//QC
LONG* CF_pnAA 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//AA
LONG* CF_pnBA 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//BA
LONG* CF_pnCA 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//CA
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//C4
LONG* CF_pnIaDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//IaDRIFT
LONG* CF_pnIbDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IbDRIFT
LONG* CF_pnIcDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IcDRIFT
LONG* CF_pnI0DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//I0DRIFT
LONG* CF_pnI1DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//I1DRIFT
LONG* CF_pnUaDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//UaDRIFT
LONG* CF_pnUbDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//UbDRIFT
LONG* CF_pnUcDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//UcDRIFT
LONG* CF_pnU0DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//U0DRIFT
LONG* CF_pnUl1DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//Ul1DRIFT
LONG* CF_pnUl2DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//Ul2DRIFT
LONG* CF_pnUl3DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//Ul3DRIFT

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 176), // IN1
    (LONG*)(RM_BUF_ADDR + 180) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 168), // BR1
    (LONG*)(RM_BUF_ADDR + 172) // BR2
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

//��������500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//�����ٶ�����3100----------
const  LONG* FailureInf3100[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//�����ٶ϶���3101----------
const  LONG* FailureInf3101[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//�����ٶϷ���3102----------
const  LONG* FailureInf3102[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//������������3103----------
const  LONG* FailureInf3103[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//������������3104----------
const  LONG* FailureInf3104[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//������������3105----------
const  LONG* FailureInf3105[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//��ƽ�������������3106----------
const  LONG* FailureInf3106[] = {
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��ƽ�������������3107----------
const  LONG* FailureInf3107[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��ƽ�������������3108----------
const  LONG* FailureInf3108[] = {
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��ƽ���ѹ��������3109----------
const  LONG* FailureInf3109[] = {
    (LONG*)(RM_BUF_ADDR + 16) // UL1
};

//��ƽ���ѹ��������3110----------
const  LONG* FailureInf3110[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��ƽ���ѹ��������3111----------
const  LONG* FailureInf3111[] = {
    (LONG*)(RM_BUF_ADDR + 16) // UL1
};

//��ѹ��������3112----------
const  LONG* FailureInf3112[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//��ѹ��������3113----------
const  LONG* FailureInf3113[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��ѹ��������3114----------
const  LONG* FailureInf3114[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//��ѹ��������3115----------
const  LONG* FailureInf3115[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��ѹ�澯3116----------
const  LONG* FailureInf3116[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//ĸ��PT����3117----------
const  LONG* FailureInf3117[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 40), // U2
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//��·�ӵ�3118----------
const  LONG* FailureInf3118[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//ĸ�߽ӵ�3119----------
const  LONG* FailureInf3119[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��������3120----------
const  LONG* FailureInf3120[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//��·����բʧ��3121----------
const  LONG* FailureInf3121[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//ͨ������3122----------
const  LONG* FailureInf3122[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64) // I1
};

//ʧ�鱣��3123----------
const  LONG* FailureInf3123[] = {
    {NULL}
};

//A��CT����3124----------
const  LONG* FailureInf3124[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//A��CT���߻ָ�3125----------
const  LONG* FailureInf3125[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//B��CT����3126----------
const  LONG* FailureInf3126[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//B��CT���߻ָ�3127----------
const  LONG* FailureInf3127[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//C��CT����3128----------
const  LONG* FailureInf3128[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//C��CT���߻ָ�3129----------
const  LONG* FailureInf3129[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 156), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 160), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 164) // Icm
};

//5V��Դ����20----------
const  LONG* FailureInf20[] = {
    {NULL}
};

//EEPROMд�����21----------
const  LONG* FailureInf21[] = {
    {NULL}
};

//5V��Դ���ϻָ�64----------
const  LONG* FailureInf64[] = {
    {NULL}
};

//ĸ��PT���߻ָ�3130----------
const  LONG* FailureInf3130[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 40), // U2
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56) // Ic
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
    {0, FailureInf50, 50},//AD���ϻָ�
    {0, FailureInf51, 51},//��ѹ����ϻָ�
    {0, FailureInf52, 52},//EEPROM��ֵ���ϻָ�
    {0, FailureInf55, 55},//RAM��ֵ���ϻָ�
    {0, FailureInf58, 58},//����ROM���ϻָ�
    {0, FailureInf60, 60},//ͨ��ͨ���쳣�ָ�
    {0, FailureInf62, 62},//NVRAM���ݳ���ָ�
    {0, FailureInf63, 63},//�߼����̹��ϻָ�
    {0, FailureInf500, 500},//��������
    {3, FailureInf3100, 3100},//�����ٶ�����
    {15, FailureInf3101, 3101},//�����ٶ϶���
    {3, FailureInf3102, 3102},//�����ٶϷ���
    {3, FailureInf3103, 3103},//������������
    {15, FailureInf3104, 3104},//������������
    {3, FailureInf3105, 3105},//������������
    {1, FailureInf3106, 3106},//��ƽ�������������
    {15, FailureInf3107, 3107},//��ƽ�������������
    {1, FailureInf3108, 3108},//��ƽ�������������
    {1, FailureInf3109, 3109},//��ƽ���ѹ��������
    {15, FailureInf3110, 3110},//��ƽ���ѹ��������
    {1, FailureInf3111, 3111},//��ƽ���ѹ��������
    {6, FailureInf3112, 3112},//��ѹ��������
    {15, FailureInf3113, 3113},//��ѹ��������
    {6, FailureInf3114, 3114},//��ѹ��������
    {15, FailureInf3115, 3115},//��ѹ��������
    {15, FailureInf3116, 3116},//��ѹ�澯
    {14, FailureInf3117, 3117},//ĸ��PT����
    {15, FailureInf3118, 3118},//��·�ӵ�
    {15, FailureInf3119, 3119},//ĸ�߽ӵ�
    {15, FailureInf3120, 3120},//��������
    {5, FailureInf3121, 3121},//��·����բʧ��
    {15, FailureInf3122, 3122},//ͨ������
    {0, FailureInf3123, 3123},//ʧ�鱣��
    {6, FailureInf3124, 3124},//A��CT����
    {6, FailureInf3125, 3125},//A��CT���߻ָ�
    {6, FailureInf3126, 3126},//B��CT����
    {6, FailureInf3127, 3127},//B��CT���߻ָ�
    {6, FailureInf3128, 3128},//C��CT����
    {6, FailureInf3129, 3129},//C��CT���߻ָ�
    {0, FailureInf20, 20},//5V��Դ����
    {0, FailureInf21, 21},//EEPROMд�����
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {14, FailureInf3130, 3130},//ĸ��PT���߻ָ�
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(AS_BUF_ADDR + 8), (LONG*)(AS_BUF_ADDR + 12) },
	{ (LONG*)(AS_BUF_ADDR + 16), (LONG*)(AS_BUF_ADDR + 20) },
	{ (LONG*)(AS_BUF_ADDR + 24), (LONG*)(AS_BUF_ADDR + 28) },
	{ (LONG*)(AS_BUF_ADDR + 32), (LONG*)(AS_BUF_ADDR + 36) },
	{ (LONG*)(AS_BUF_ADDR + 40), (LONG*)(AS_BUF_ADDR + 44) },
	{ (LONG*)(AS_BUF_ADDR + 48), (LONG*)(AS_BUF_ADDR + 52) },
	{ (LONG*)(AS_BUF_ADDR + 56), (LONG*)(AS_BUF_ADDR + 60) },
	{ (LONG*)(AS_BUF_ADDR + 64), (LONG*)(AS_BUF_ADDR + 68) },
	{ (LONG*)(AS_BUF_ADDR + 72), (LONG*)(AS_BUF_ADDR + 76) },
	{ (LONG*)(AS_BUF_ADDR + 80), (LONG*)(AS_BUF_ADDR + 84) },
	{ (LONG*)(AS_BUF_ADDR + 88), (LONG*)(AS_BUF_ADDR + 92) },
	{ (LONG*)(AS_BUF_ADDR + 96), (LONG*)(AS_BUF_ADDR + 100) },
	{ (LONG*)(AS_BUF_ADDR + 104), (LONG*)(AS_BUF_ADDR + 108) },
	{ (LONG*)(AS_BUF_ADDR + 112), (LONG*)(AS_BUF_ADDR + 116) },
	{ (LONG*)(AS_BUF_ADDR + 120), (LONG*)(AS_BUF_ADDR + 124) },
	{ (LONG*)(AS_BUF_ADDR + 128), (LONG*)(AS_BUF_ADDR + 132) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 140) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 84) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 144) ,
	 (LONG*)(AS_BUF_ADDR + 148) ,
	 (LONG*)(AS_BUF_ADDR + 152) ,
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
	 (WORD)(14) ,
	 (WORD)(15) ,
	 (WORD)(16) ,
	 (WORD)(17) ,
	 (WORD)(18) ,
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
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	30890, //Up
	30890, //U0
	3050, //Ip
	15250, //I0
	15250, //I1
	43690, //UaM
	43690, //UbM
	43690, //UcM
	43690, //UL1M
	43690, //UL2M
	43690, //UL3M
	43690, //U0M
	64480, //IAM
	64480, //IBM
	64480, //ICM
	21600, //I0M
	21600, //I1M
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	4, //IaDRIFT
	4, //IbDRIFT
	4, //IcDRIFT
	4, //I0DRIFT
	4, //I1DRIFT
	4, //UaDRIFT
	4, //UbDRIFT
	4, //UcDRIFT
	4, //U0DRIFT
	4, //Ul1DRIFT
	4, //Ul2DRIFT
	4, //Ul3DRIFT
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	30890, //Up
	30890, //U0
	15250, //Ip
	15250, //I0
	15250, //I1
	43690, //UaM
	43690, //UbM
	43690, //UcM
	43690, //UL1M
	43690, //UL2M
	43690, //UL3M
	43690, //U0M
	322400, //IAM
	322400, //IBM
	322400, //ICM
	21600, //I0M
	21600, //I1M
	129900, //PA
	129900, //QA
	129900, //PB
	129900, //QB
	129900, //PC
	129900, //QC
	105, //AA
	105, //BA
	105, //CA
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	4, //IaDRIFT
	4, //IbDRIFT
	4, //IcDRIFT
	4, //I0DRIFT
	4, //I1DRIFT
	4, //UaDRIFT
	4, //UbDRIFT
	4, //UcDRIFT
	4, //U0DRIFT
	4, //Ul1DRIFT
	4, //Ul2DRIFT
	4, //Ul3DRIFT
};

//����----------------------------------
