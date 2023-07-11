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
	{SYN_PHASE_UA, "UA"},
	{SYN_PHASE_UB, "UB"},
	{SYN_PHASE_UC, "UC"},
	{SYN_PHASE_UAB, "UAB"},
	{SYN_PHASE_UBC, "UBC"},
	{SYN_PHASE_UCA, "UCA"},
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
	{6, (TSetValueInf*)&(SetValueInf[0])}, //��ֵͬ�����
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnF2 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// F2
LONG* 	RM_pnDiffU 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// ��U
LONG* 	RM_pnDiffA 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// ����
LONG* 	RM_pnDiffdq 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// ��dq
LONG* 	RM_pnIam 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// Iam
LONG* 	RM_pnIbm 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// Ibm
LONG* 	RM_pnIcm 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// Icm
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 184); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 188); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 192); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 196); 	// IN2


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
LONG* 	M_pnF2 	= 	(LONG*)(MEASURE_BUF_ADDR + 76); 	//F2 F2
LONG* 	M_pnVD 	= 	(LONG*)(MEASURE_BUF_ADDR + 80); 	//VD VD
LONG* 	M_pnAD 	= 	(LONG*)(MEASURE_BUF_ADDR + 84); 	//AD AD


//��ѹ��----------------------------------
BYTE* SW_pby50I0 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//�����ٶ�
BYTE* SW_pby50I1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//�������
BYTE* SW_pby50I2 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//�������
BYTE* SW_pby50I3 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//�������
BYTE* SW_pby50N1 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//����������
BYTE* SW_pby50N2 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//����������
BYTE* SW_pby50ISH 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//ĸ�߳�籣��
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//��ѹ����
BYTE* SW_pby79 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//�غ�բ
BYTE* SW_pbyJDXX 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//�ӵ�ѡ��
BYTE* SW_pby25 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//��ͬ��
BYTE* SW_pby83 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//����Ͷ
BYTE* SW_pby50IOL 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//�����ɸ澯
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 13); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 14); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 15); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby50I0 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//�����ٶ�
BYTE* CFG_pby50I0_47 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//�����ٶϸ�ѹ����
BYTE* CFG_pby50I0_67 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//�����ٶϹ��ʷ���
BYTE* CFG_pby50I0_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//�����ٶϷ�������
BYTE* CFG_pby50I1 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//����I��
BYTE* CFG_pby50I1_47 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//I�θ�ѹ����
BYTE* CFG_pby50I1_67 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//I�ι��ʷ���
BYTE* CFG_pby50I1_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//I�η�������
BYTE* CFG_pby50I2 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//����II��
BYTE* CFG_pby50I2_47 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//II�θ�ѹ����
BYTE* CFG_pby50I2_67 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//II�ι��ʷ���
BYTE* CFG_pby50I2_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//II�η�������
BYTE* CFG_pby50I3 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//����III��
BYTE* CFG_pby50I3_47 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//III�θ�ѹ����
BYTE* CFG_pby50I3_67 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//III�ι��ʷ���
BYTE* CFG_pby50I3_PD 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//III�η�������
BYTE* CFG_pby50N1 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//�������I��
BYTE* CFG_pby50N2 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//�������II��
BYTE* CFG_pby50I_SH 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//ĸ�߳�籣��
BYTE* CFG_pby27U 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//��ѹ����
BYTE* CFG_pby79_50I0 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//�����ٶ��غ�
BYTE* CFG_pby79_50I1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//����I���غ�
BYTE* CFG_pby79_50I2 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//����II���غ�
BYTE* CFG_pby79_50I3 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//����III���غ�
BYTE* CFG_pby79UC 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//͵���غ�բ
BYTE* CFG_pby79_25 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//�غ�բ��ͬ��
BYTE* CFG_pby79_27 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//�غ�բ����ѹ
BYTE* CFG_pby79_59 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//�غ�բ����ѹ
BYTE* CFG_pby50I1_94 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//����I�μ���
BYTE* CFG_pby50I2_94 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//����II�μ���
BYTE* CFG_pby50I3_94 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//����III�μ���
BYTE* CFG_pby59N_74 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//��ѹ�澯
BYTE* CFG_pbyJDXX 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//�ӵ�ѡ��
BYTE* CFG_pbyXHJD 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//������Ȧ�ӵ�
BYTE* CFG_pbyPTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//PT���߼��
BYTE* CFG_pby25 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//�ֺϼ�ͬ��
BYTE* CFG_pbySH_27 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//�ֺϼ���ѹ
BYTE* CFG_pby83 	= 	(BYTE*)(CFG_BUF_ADDR + 37); 	//����Ͷ
BYTE* CFG_pby50IOL 	= 	(BYTE*)(CFG_BUF_ADDR + 38); 	//�����ɸ澯
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 39); 	//CT���߼��
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 40); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 41); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 42); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnCFG1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ CFG1
LONG* RS_pnCFG2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//������2 CFG2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//ͻ������������ PIC
LONG* RS_pn50P0C 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//�����ٶϵ��� 50P0C
LONG* RS_pn50P0D 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//�����ٶ�ʱ�� 50P0D
LONG* RS_pn50P0_27 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//�����ٶϵ�ѹ��ֵ 50P0_27
LONG* RS_pn50P0_47 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//�����ٶϸ����ѹ 50P0_47
LONG* RS_pn50P1C 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//������ε��� 50P1C
LONG* RS_pn50P1D 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//�������ʱ�� 50P1D
LONG* RS_pn50P1_27 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//������ε�ѹ��ֵ 50P1_27
LONG* RS_pn50P1_47 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//������θ����ѹ 50P1_47
LONG* RS_pn50P2C 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//������ε��� 50P2C
LONG* RS_pn50P2D 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//�������ʱ�� 50P2D
LONG* RS_pn50P2_27 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//������ε�ѹ��ֵ 50P2_27
LONG* RS_pn50P2_47 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//������θ����ѹ 50P2_47
LONG* RS_pn50P3C 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//������ε��� 50P3C
LONG* RS_pn50P3D 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//�������ʱ�� 50P3D
LONG* RS_pn50P3_27 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//������ε�ѹ��ֵ 50P3_27
LONG* RS_pn50P3_47 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//������θ����ѹ 50P3_47
LONG* RS_pn67ANG 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//���ʷ��������� 67ANG
LONG* RS_pn50N1C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//���������ε��� 50N1C
LONG* RS_pn50N1D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//����������ʱ�� 50N1D
LONG* RS_pn50N2C 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//���������ε��� 50N2C
LONG* RS_pn50N2D 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//����������ʱ�� 50N2D
LONG* RS_pn50ISHC 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//ĸ�߳�籣������ 50ISHC
LONG* RS_pn50ISHD 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//ĸ�߳�籣��ʱ�� 50ISHD
LONG* RS_pn50ISHT 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//ĸ�䱣��Ͷ��ʱ�� 50ISHT
LONG* RS_pn79SH 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//�غ�բͶ����� 79SH
LONG* RS_pn79D1 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//��һ���غ�բʱ�� 79D1
LONG* RS_pn79D2 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//�ڶ����غ�բʱ�� 79D2
LONG* RS_pn79CLSD 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//�غ�բ����������ʱ 79CLSD
LONG* RS_pn79CT 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//�غ�բ���ʱ�� 79CT
LONG* RS_pn25_27 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//�غ�բ����ѹ��ֵ 25_27
LONG* RS_pn25PA 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//ͬ�ڹ��� 25PA
LONG* RS_pn25PH 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//ͬ����� 25PH
LONG* RS_pn25DT 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//ͬ�ڼ��ʱ�� 25DT
LONG* RS_pn25VUL 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//��ͬ�ڵ�ѹ���� 25VUL
LONG* RS_pn25VLL 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//��ͬ�ڵ�ѹ���� 25VLL
LONG* RS_pn25VD 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//ͬ������ѹ�� 25VD
LONG* RS_pn25FD 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//ͬ������Ƶ�� 25FD
LONG* RS_pn52CLT 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//��·����բʱ�� 52CLT
LONG* RS_pn94D 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//����ٶ���ʱ�� 94D
LONG* RS_pnSOTFD 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//�����Ͷ��ʱ�� SOTFD
LONG* RS_pn27U 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//��ѹ������ѹ 27U
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//��ѹ����ʱ�� 27D
LONG* RS_pn27_LK37 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//��ѹ����������ֵ 27_LK37
LONG* RS_pn59NU 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//��ѹ�澯��ѹ 59NU
LONG* RS_pn59ND 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//��ѹ�澯ʱ�� 59ND
LONG* RS_pn27BZT 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//����Ͷ��ѹ��ֵ 27BZT
LONG* RS_pn59BZT 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//����Ͷ��ѹ��ֵ 59BZT
LONG* RS_pn50BZT 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//����Ͷ������ֵ 50BZT
LONG* RS_pnCHARD 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//����Ͷ���ʱ�� CHARD
LONG* RS_pn50OLC 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//�����ɵ��� 50OLC
LONG* RS_pn50OLD 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//������ʱ�� 50OLD
LONG* RS_pnPT1_V 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//ĸ��PT���߼���ѹ PT1_V
LONG* RS_pnPT1_C 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//ĸ��PT���߼����� PT1_C
LONG* RS_pnPT1_D 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//ĸ��PT���߼��ʱ�� PT1_D
LONG* RS_pnPT2_V1 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//��·PT����ĸ����ѹ PT2_V1
LONG* RS_pnPT2_C 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//��·PT����ĸ������ PT2_C
LONG* RS_pnPT2_V2 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//��·PT������·��ѹ PT2_V2
LONG* RS_pnPT2_D 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//��·PT���߼��ʱ�� PT2_D
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//ʧ�鱣��ʱ�� 52BFD

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
    (LONG*)(RM_BUF_ADDR + 192), // IN1
    (LONG*)(RM_BUF_ADDR + 196) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 184), // BR1
    (LONG*)(RM_BUF_ADDR + 188) // BR2
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

//ͨ������2600----------
const  LONG* FailureInf2600[] = {
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
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 148), // ��I05
    (LONG*)(RM_BUF_ADDR + 144) // ��U05
};

//��������2601----------
const  LONG* FailureInf2601[] = {
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
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 148), // ��I05
    (LONG*)(RM_BUF_ADDR + 144) // ��U05
};

//�����������2602----------
const  LONG* FailureInf2602[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������ζ���2603----------
const  LONG* FailureInf2603[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//������η���2604----------
const  LONG* FailureInf2604[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//�����������2605----------
const  LONG* FailureInf2605[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������ζ���2606----------
const  LONG* FailureInf2606[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//������η���2607----------
const  LONG* FailureInf2607[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//�����������2608----------
const  LONG* FailureInf2608[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������ζ���2609----------
const  LONG* FailureInf2609[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//������η���2610----------
const  LONG* FailureInf2610[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������μ��ٶ���2611----------
const  LONG* FailureInf2611[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//������μ��ٶ���2612----------
const  LONG* FailureInf2612[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//��������������2613----------
const  LONG* FailureInf2613[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//���������ζ���2614----------
const  LONG* FailureInf2614[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//���������η���2615----------
const  LONG* FailureInf2615[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//��������������2616----------
const  LONG* FailureInf2616[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//���������ζ���2617----------
const  LONG* FailureInf2617[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//���������η���2618----------
const  LONG* FailureInf2618[] = {
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 12) // U0
};

//��ѹ������բ2619----------
const  LONG* FailureInf2619[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//��·����բʧ��2620----------
const  LONG* FailureInf2620[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 68) // I05
};

//һ���غ�բ����2621----------
const  LONG* FailureInf2621[] = {
    {NULL}
};

//�����غ�բ����2622----------
const  LONG* FailureInf2622[] = {
    {NULL}
};

//һ���غ�բ������2623----------
const  LONG* FailureInf2623[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//����Ͷ����2624----------
const  LONG* FailureInf2624[] = {
    {NULL}
};

//����Ͷ�˳�2625----------
const  LONG* FailureInf2625[] = {
    {NULL}
};

//����Ͷ�ɹ�2626----------
const  LONG* FailureInf2626[] = {
    {NULL}
};

//����Ͷʧ��2627----------
const  LONG* FailureInf2627[] = {
    {NULL}
};

//��1��·��2628----------
const  LONG* FailureInf2628[] = {
    {NULL}
};

//��1��·��2629----------
const  LONG* FailureInf2629[] = {
    {NULL}
};

//��1��·��ʧ��2630----------
const  LONG* FailureInf2630[] = {
    {NULL}
};

//��1��·��ʧ��2631----------
const  LONG* FailureInf2631[] = {
    {NULL}
};

//��2��·��2632----------
const  LONG* FailureInf2632[] = {
    {NULL}
};

//��2��·��2633----------
const  LONG* FailureInf2633[] = {
    {NULL}
};

//��2��·��ʧ��2634----------
const  LONG* FailureInf2634[] = {
    {NULL}
};

//��2��·��ʧ��2635----------
const  LONG* FailureInf2635[] = {
    {NULL}
};

//��3��·��2636----------
const  LONG* FailureInf2636[] = {
    {NULL}
};

//��3��·��2637----------
const  LONG* FailureInf2637[] = {
    {NULL}
};

//��3��·��ʧ��2638----------
const  LONG* FailureInf2638[] = {
    {NULL}
};

//��3��·��ʧ��2639----------
const  LONG* FailureInf2639[] = {
    {NULL}
};

//��4��·��2640----------
const  LONG* FailureInf2640[] = {
    {NULL}
};

//��4��·��2641----------
const  LONG* FailureInf2641[] = {
    {NULL}
};

//��4��·��ʧ��2642----------
const  LONG* FailureInf2642[] = {
    {NULL}
};

//��4��·��ʧ��2643----------
const  LONG* FailureInf2643[] = {
    {NULL}
};

//��1����2644----------
const  LONG* FailureInf2644[] = {
    {NULL}
};

//��1����2645----------
const  LONG* FailureInf2645[] = {
    {NULL}
};

//��1����ʧ��2646----------
const  LONG* FailureInf2646[] = {
    {NULL}
};

//��1����ʧ��2647----------
const  LONG* FailureInf2647[] = {
    {NULL}
};

//��2����2648----------
const  LONG* FailureInf2648[] = {
    {NULL}
};

//��2����2649----------
const  LONG* FailureInf2649[] = {
    {NULL}
};

//��2����ʧ��2650----------
const  LONG* FailureInf2650[] = {
    {NULL}
};

//��2����ʧ��2651----------
const  LONG* FailureInf2651[] = {
    {NULL}
};

//��3����2652----------
const  LONG* FailureInf2652[] = {
    {NULL}
};

//��3����2653----------
const  LONG* FailureInf2653[] = {
    {NULL}
};

//��3����ʧ��2654----------
const  LONG* FailureInf2654[] = {
    {NULL}
};

//��3����ʧ��2655----------
const  LONG* FailureInf2655[] = {
    {NULL}
};

//��4����2656----------
const  LONG* FailureInf2656[] = {
    {NULL}
};

//��4����2657----------
const  LONG* FailureInf2657[] = {
    {NULL}
};

//��4����ʧ��2658----------
const  LONG* FailureInf2658[] = {
    {NULL}
};

//��4����ʧ��2659----------
const  LONG* FailureInf2659[] = {
    {NULL}
};

//1����ʧѹ������Ͷ2660----------
const  LONG* FailureInf2660[] = {
    {NULL}
};

//2����ʧѹ������Ͷ2661----------
const  LONG* FailureInf2661[] = {
    {NULL}
};

//��·PT���߻ָ�2662----------
const  LONG* FailureInf2662[] = {
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
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//ĸ��PT���߻ָ�2663----------
const  LONG* FailureInf2663[] = {
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

//��·PT����2664----------
const  LONG* FailureInf2664[] = {
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
    (LONG*)(RM_BUF_ADDR + 56) // Ic
};

//ĸ��PT����2665----------
const  LONG* FailureInf2665[] = {
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

//��·�ӵ�2666----------
const  LONG* FailureInf2666[] = {
    {NULL}
};

//ĸ�߽ӵ�2667----------
const  LONG* FailureInf2667[] = {
    {NULL}
};

//��ѹ�澯2668----------
const  LONG* FailureInf2668[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//ͬ�ڲ���2669----------
const  LONG* FailureInf2669[] = {
    {NULL}
};

//ͬ��ʧ��2670----------
const  LONG* FailureInf2670[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 152), // F1
    (LONG*)(RM_BUF_ADDR + 156), // F2
    (LONG*)(RM_BUF_ADDR + 160), // ��U
    (LONG*)(RM_BUF_ADDR + 164) // ����
};

//ͬ�ڳɹ�2671----------
const  LONG* FailureInf2671[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 152), // F1
    (LONG*)(RM_BUF_ADDR + 156), // F2
    (LONG*)(RM_BUF_ADDR + 160), // ��U
    (LONG*)(RM_BUF_ADDR + 164) // ����
};

//��ѹ�պ�2672----------
const  LONG* FailureInf2672[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36), // Uca
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 152), // F1
    (LONG*)(RM_BUF_ADDR + 156), // F2
    (LONG*)(RM_BUF_ADDR + 160), // ��U
    (LONG*)(RM_BUF_ADDR + 164) // ����
};

//ͬ�ڱ�����2673----------
const  LONG* FailureInf2673[] = {
    {NULL}
};

//ͬ�ڱ��ж�2674----------
const  LONG* FailureInf2674[] = {
    {NULL}
};

//�غ�բʧ��2675----------
const  LONG* FailureInf2675[] = {
    {NULL}
};

//͵���غ�բ2676----------
const  LONG* FailureInf2676[] = {
    {NULL}
};

//ʧ�鱣��2677----------
const  LONG* FailureInf2677[] = {
    {NULL}
};

//A��CT����2678----------
const  LONG* FailureInf2678[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//A��CT���߻ָ�2679----------
const  LONG* FailureInf2679[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//B��CT����2680----------
const  LONG* FailureInf2680[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//B��CT���߻ָ�2681----------
const  LONG* FailureInf2681[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//C��CT����2682----------
const  LONG* FailureInf2682[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
};

//C��CT���߻ָ�2683----------
const  LONG* FailureInf2683[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 172), // Iam
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 176), // Ibm
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 180) // Icm
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

//�����غ�բ������2684----------
const  LONG* FailureInf2684[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 12), // U0
    (LONG*)(RM_BUF_ADDR + 16), // UL1
    (LONG*)(RM_BUF_ADDR + 20), // UL2
    (LONG*)(RM_BUF_ADDR + 24), // UL3
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//һ���غ�բ�ɹ�2685----------
const  LONG* FailureInf2685[] = {
    {NULL}
};

//�����غ�բ�ɹ�2686----------
const  LONG* FailureInf2686[] = {
    {NULL}
};

//һ���غ�բʧ��2687----------
const  LONG* FailureInf2687[] = {
    {NULL}
};

//�����غ�բʧ��2688----------
const  LONG* FailureInf2688[] = {
    {NULL}
};

//һ���غ�բ����2689----------
const  LONG* FailureInf2689[] = {
    {NULL}
};

//�����غ�բ����2690----------
const  LONG* FailureInf2690[] = {
    {NULL}
};

//һ���غ�բδ����2691----------
const  LONG* FailureInf2691[] = {
    {NULL}
};

//�����غ�բδ����2692----------
const  LONG* FailureInf2692[] = {
    {NULL}
};

//������μ�������2693----------
const  LONG* FailureInf2693[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������μ��ٷ���2694----------
const  LONG* FailureInf2694[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������μ�������2695----------
const  LONG* FailureInf2695[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������μ��ٷ���2696----------
const  LONG* FailureInf2696[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//���������쳣22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//���������쳣�ָ�65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//�����ٶ�����2697----------
const  LONG* FailureInf2697[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//�����ٶ϶���2698----------
const  LONG* FailureInf2698[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//�����ٶϷ���2699----------
const  LONG* FailureInf2699[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//�����ɸ澯2700----------
const  LONG* FailureInf2700[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120) // ��Ip1
};

//������μ�������2701----------
const  LONG* FailureInf2701[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//������μ��ٶ���2702----------
const  LONG* FailureInf2702[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//������μ��ٷ���2703----------
const  LONG* FailureInf2703[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//ĸ�߳�籣������2704----------
const  LONG* FailureInf2704[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
};

//ĸ�߳�籣������2705----------
const  LONG* FailureInf2705[] = {
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
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 60), // I0
    (LONG*)(RM_BUF_ADDR + 64), // I1
    (LONG*)(RM_BUF_ADDR + 44), // U05
    (LONG*)(RM_BUF_ADDR + 68), // I05
    (LONG*)(RM_BUF_ADDR + 72), // ��Ua
    (LONG*)(RM_BUF_ADDR + 76), // ��Ub
    (LONG*)(RM_BUF_ADDR + 80), // ��Uc
    (LONG*)(RM_BUF_ADDR + 84), // ��Uab
    (LONG*)(RM_BUF_ADDR + 88), // ��Ubc
    (LONG*)(RM_BUF_ADDR + 92), // ��Uca
    (LONG*)(RM_BUF_ADDR + 96), // ��Ul1
    (LONG*)(RM_BUF_ADDR + 100), // ��Ul2
    (LONG*)(RM_BUF_ADDR + 104), // ��Ul3
    (LONG*)(RM_BUF_ADDR + 108), // ��Ipa
    (LONG*)(RM_BUF_ADDR + 112), // ��Ipb
    (LONG*)(RM_BUF_ADDR + 116), // ��Ipc
    (LONG*)(RM_BUF_ADDR + 120), // ��Ip1
    (LONG*)(RM_BUF_ADDR + 136), // ��U01
    (LONG*)(RM_BUF_ADDR + 140), // ��I01
    (LONG*)(RM_BUF_ADDR + 144), // ��U05
    (LONG*)(RM_BUF_ADDR + 148) // ��I05
};

//ĸ�߳�籣������2706----------
const  LONG* FailureInf2706[] = {
    (LONG*)(RM_BUF_ADDR + 48), // Ia
    (LONG*)(RM_BUF_ADDR + 52), // Ib
    (LONG*)(RM_BUF_ADDR + 56), // Ic
    (LONG*)(RM_BUF_ADDR + 0), // Ua
    (LONG*)(RM_BUF_ADDR + 4), // Ub
    (LONG*)(RM_BUF_ADDR + 8), // Uc
    (LONG*)(RM_BUF_ADDR + 28), // Uab
    (LONG*)(RM_BUF_ADDR + 32), // Ubc
    (LONG*)(RM_BUF_ADDR + 36) // Uca
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
    {33, FailureInf2600, 2600},//ͨ������
    {33, FailureInf2601, 2601},//��������
    {9, FailureInf2602, 2602},//�����������
    {35, FailureInf2603, 2603},//������ζ���
    {9, FailureInf2604, 2604},//������η���
    {9, FailureInf2605, 2605},//�����������
    {35, FailureInf2606, 2606},//������ζ���
    {9, FailureInf2607, 2607},//������η���
    {9, FailureInf2608, 2608},//�����������
    {35, FailureInf2609, 2609},//������ζ���
    {9, FailureInf2610, 2610},//������η���
    {35, FailureInf2611, 2611},//������μ��ٶ���
    {35, FailureInf2612, 2612},//������μ��ٶ���
    {2, FailureInf2613, 2613},//��������������
    {35, FailureInf2614, 2614},//���������ζ���
    {2, FailureInf2615, 2615},//���������η���
    {2, FailureInf2616, 2616},//��������������
    {35, FailureInf2617, 2617},//���������ζ���
    {2, FailureInf2618, 2618},//���������η���
    {35, FailureInf2619, 2619},//��ѹ������բ
    {6, FailureInf2620, 2620},//��·����բʧ��
    {0, FailureInf2621, 2621},//һ���غ�բ����
    {0, FailureInf2622, 2622},//�����غ�բ����
    {10, FailureInf2623, 2623},//һ���غ�բ������
    {0, FailureInf2624, 2624},//����Ͷ����
    {0, FailureInf2625, 2625},//����Ͷ�˳�
    {0, FailureInf2626, 2626},//����Ͷ�ɹ�
    {0, FailureInf2627, 2627},//����Ͷʧ��
    {0, FailureInf2628, 2628},//��1��·��
    {0, FailureInf2629, 2629},//��1��·��
    {0, FailureInf2630, 2630},//��1��·��ʧ��
    {0, FailureInf2631, 2631},//��1��·��ʧ��
    {0, FailureInf2632, 2632},//��2��·��
    {0, FailureInf2633, 2633},//��2��·��
    {0, FailureInf2634, 2634},//��2��·��ʧ��
    {0, FailureInf2635, 2635},//��2��·��ʧ��
    {0, FailureInf2636, 2636},//��3��·��
    {0, FailureInf2637, 2637},//��3��·��
    {0, FailureInf2638, 2638},//��3��·��ʧ��
    {0, FailureInf2639, 2639},//��3��·��ʧ��
    {0, FailureInf2640, 2640},//��4��·��
    {0, FailureInf2641, 2641},//��4��·��
    {0, FailureInf2642, 2642},//��4��·��ʧ��
    {0, FailureInf2643, 2643},//��4��·��ʧ��
    {0, FailureInf2644, 2644},//��1����
    {0, FailureInf2645, 2645},//��1����
    {0, FailureInf2646, 2646},//��1����ʧ��
    {0, FailureInf2647, 2647},//��1����ʧ��
    {0, FailureInf2648, 2648},//��2����
    {0, FailureInf2649, 2649},//��2����
    {0, FailureInf2650, 2650},//��2����ʧ��
    {0, FailureInf2651, 2651},//��2����ʧ��
    {0, FailureInf2652, 2652},//��3����
    {0, FailureInf2653, 2653},//��3����
    {0, FailureInf2654, 2654},//��3����ʧ��
    {0, FailureInf2655, 2655},//��3����ʧ��
    {0, FailureInf2656, 2656},//��4����
    {0, FailureInf2657, 2657},//��4����
    {0, FailureInf2658, 2658},//��4����ʧ��
    {0, FailureInf2659, 2659},//��4����ʧ��
    {0, FailureInf2660, 2660},//1����ʧѹ������Ͷ
    {0, FailureInf2661, 2661},//2����ʧѹ������Ͷ
    {13, FailureInf2662, 2662},//��·PT���߻ָ�
    {14, FailureInf2663, 2663},//ĸ��PT���߻ָ�
    {13, FailureInf2664, 2664},//��·PT����
    {14, FailureInf2665, 2665},//ĸ��PT����
    {0, FailureInf2666, 2666},//��·�ӵ�
    {0, FailureInf2667, 2667},//ĸ�߽ӵ�
    {10, FailureInf2668, 2668},//��ѹ�澯
    {0, FailureInf2669, 2669},//ͬ�ڲ���
    {22, FailureInf2670, 2670},//ͬ��ʧ��
    {22, FailureInf2671, 2671},//ͬ�ڳɹ�
    {22, FailureInf2672, 2672},//��ѹ�պ�
    {0, FailureInf2673, 2673},//ͬ�ڱ�����
    {0, FailureInf2674, 2674},//ͬ�ڱ��ж�
    {0, FailureInf2675, 2675},//�غ�բʧ��
    {0, FailureInf2676, 2676},//͵���غ�բ
    {0, FailureInf2677, 2677},//ʧ�鱣��
    {6, FailureInf2678, 2678},//A��CT����
    {6, FailureInf2679, 2679},//A��CT���߻ָ�
    {6, FailureInf2680, 2680},//B��CT����
    {6, FailureInf2681, 2681},//B��CT���߻ָ�
    {6, FailureInf2682, 2682},//C��CT����
    {6, FailureInf2683, 2683},//C��CT���߻ָ�
    {0, FailureInf20, 20},//5V��Դ����
    {0, FailureInf21, 21},//EEPROMд�����
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {10, FailureInf2684, 2684},//�����غ�բ������
    {0, FailureInf2685, 2685},//һ���غ�բ�ɹ�
    {0, FailureInf2686, 2686},//�����غ�բ�ɹ�
    {0, FailureInf2687, 2687},//һ���غ�բʧ��
    {0, FailureInf2688, 2688},//�����غ�բʧ��
    {0, FailureInf2689, 2689},//һ���غ�բ����
    {0, FailureInf2690, 2690},//�����غ�բ����
    {0, FailureInf2691, 2691},//һ���غ�բδ����
    {0, FailureInf2692, 2692},//�����غ�բδ����
    {9, FailureInf2693, 2693},//������μ�������
    {9, FailureInf2694, 2694},//������μ��ٷ���
    {9, FailureInf2695, 2695},//������μ�������
    {9, FailureInf2696, 2696},//������μ��ٷ���
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
    {9, FailureInf2697, 2697},//�����ٶ�����
    {35, FailureInf2698, 2698},//�����ٶ϶���
    {9, FailureInf2699, 2699},//�����ٶϷ���
    {31, FailureInf2700, 2700},//�����ɸ澯
    {9, FailureInf2701, 2701},//������μ�������
    {35, FailureInf2702, 2702},//������μ��ٶ���
    {9, FailureInf2703, 2703},//������μ��ٷ���
    {9, FailureInf2704, 2704},//ĸ�߳�籣������
    {35, FailureInf2705, 2705},//ĸ�߳�籣������
    {9, FailureInf2706, 2706},//ĸ�߳�籣������
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
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
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 168) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 104) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 204), (LONG*)(NoTimeAddr) },
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
	 (BYTE*)(SW_BUF_ADDR + 10) ,
	 (BYTE*)(SW_BUF_ADDR + 11) ,
	 (BYTE*)(SW_BUF_ADDR + 12) ,
	 (BYTE*)(SW_BUF_ADDR + 13) ,
	 (BYTE*)(SW_BUF_ADDR + 14) ,
	 (BYTE*)(SW_BUF_ADDR + 15) ,
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
	 (WORD)(14) ,
	 (WORD)(15) ,
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
