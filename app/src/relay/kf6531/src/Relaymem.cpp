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
};
//������ֵȡֵ��Ϣ
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{LCUR_2, "�Ͳ���·����"},
	{LCUR_4, "�Ͳ���·����"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //������ֵ�����
	{2, (TSetValueInf*)&(SysSetValueInf[2])}, //������ֵ�Ͳ����·��
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnUHAB 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// UHAB
LONG* 	RM_pnUHBC 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// UHBC
LONG* 	RM_pnUHCA 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// UHCA
LONG* 	RM_pnUHA 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// UHA
LONG* 	RM_pnUHB 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// UHB
LONG* 	RM_pnUHC 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// UHC
LONG* 	RM_pnU0 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// U0
LONG* 	RM_pnULA 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// ULA
LONG* 	RM_pnULB 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// ULB
LONG* 	RM_pnULAB 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// ULAB
LONG* 	RM_pnIHA 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// IHA
LONG* 	RM_pnIHB 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// IHB
LONG* 	RM_pnIHC 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IHC
LONG* 	RM_pnI0 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// I0
LONG* 	RM_pnIJX 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// IJX
LONG* 	RM_pnILA 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// ILA
LONG* 	RM_pnILB 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// ILB
LONG* 	RM_pnIL1 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IL1
LONG* 	RM_pnIL2 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// IL2
LONG* 	RM_pnIL3 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// IL3
LONG* 	RM_pnIL4 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IL4
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UHA UHA
LONG* 	M_pnUHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UHB UHB
LONG* 	M_pnUHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UHC UHC
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnULA 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//ULA ULA
LONG* 	M_pnULB 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//ULB ULB
LONG* 	M_pnIHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//IHA IHA
LONG* 	M_pnIHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IHB IHB
LONG* 	M_pnIHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IHC IHC
LONG* 	M_pnI0 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//I0 I0
LONG* 	M_pnILA 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//ILA ILA
LONG* 	M_pnILB 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//ILB ILB
LONG* 	M_pnIjx 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ijx Ijx
LONG* 	M_pnIL1 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//IL1 IL1
LONG* 	M_pnIL2 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//IL2 IL2
LONG* 	M_pnIL3 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//IL3 IL3
LONG* 	M_pnIL4 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//IL4 IL4


//��ѹ��----------------------------------
BYTE* SW_pby50H 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//�߲��������
BYTE* SW_pby49 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//��ʱ�޹�����
BYTE* SW_pby50L 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//�Ͳ��������
BYTE* SW_pby50OL 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//�����ɱ���
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//�߲�ʧѹ����
BYTE* SW_pby50N 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//�����������
BYTE* SW_pby59N 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//�����ѹ����
BYTE* SW_pby50JX 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//��϶��������
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby50HA 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//�߲�A�����
BYTE* CFG_pby50HB 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//�߲�B�����
BYTE* CFG_pby50HC 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//�߲�C�����
BYTE* CFG_pby50H_27 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//�߲��ѹ����
BYTE* CFG_pby49 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//��ʱ�޹�����
BYTE* CFG_pby50LA1 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//�Ͳ�A�����I��
BYTE* CFG_pby50LA2 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//�Ͳ�A�����II��
BYTE* CFG_pby50LA_27 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//�Ͳ�A��ѹ����
BYTE* CFG_pby50LB1 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//�Ͳ�B�����I��
BYTE* CFG_pby50LB2 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//�Ͳ�B�����II��
BYTE* CFG_pby50LB_27 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//�Ͳ�B��ѹ����
BYTE* CFG_pby50OLA1 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//A�����ɸ澯
BYTE* CFG_pby50OLB1 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//B�����ɸ澯
BYTE* CFG_pby50OLC1 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//C�����ɸ澯
BYTE* CFG_pby59OLA2 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//A��������բ
BYTE* CFG_pby50OLB2 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//B��������բ
BYTE* CFG_pby50OLC2 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//C��������բ
BYTE* CFG_pby27 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//��ѹ��ʧѹ����
BYTE* CFG_pby50N 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//�����������
BYTE* CFG_pby59N 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//�����ѹ����
BYTE* CFG_pby74PT 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//PT���߼��
BYTE* CFG_pby50DCG 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//��϶��������
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnF1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������1 F1
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//ͻ������������ PIC
LONG* RS_pn50HAC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//��ѹ��A�������� 50HAC
LONG* RS_pn50HAD 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//��ѹ��A����ʱ�� 50HAD
LONG* RS_pn50HBC 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//��ѹ��B�������� 50HBC
LONG* RS_pn50HBD 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//��ѹ��B����ʱ�� 50HBD
LONG* RS_pn50HCC 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//��ѹ��C�������� 50HCC
LONG* RS_pn50HCD 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//��ѹ��C����ʱ�� 50HCD
LONG* RS_pn27LU 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//��ѹ���ѹ��ֵ 27LU
LONG* RS_pnTIN 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//��ѹ�����ζ���� TIN
LONG* RS_pn49K 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//�ȹ����ɱ������� 49K
LONG* RS_pn49T 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//�ȹ�����ʱ�䳣�� 49T
LONG* RS_pn49AR 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//�ȹ����ɸ澯��ֵ 49AR
LONG* RS_pn49KT 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//�豸ֹͣ����ϵ�� 49KT
LONG* RS_pn50LA1C 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//�Ͳ�A�����I�ζ�ֵ 50LA1C
LONG* RS_pn50LA1D 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//�Ͳ�A�����I��ʱ�� 50LA1D
LONG* RS_pn50LA2C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//�Ͳ�A�����II�ζ�ֵ 50LA2C
LONG* RS_pn50LA2D 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//�Ͳ�A�����II��ʱ�� 50LA2D
LONG* RS_pn50LB1C 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//�Ͳ�B�����I�ζ�ֵ 50LB1C
LONG* RS_pn50LB1D 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//�Ͳ�B�����I��ʱ�� 50LB1D
LONG* RS_pn50LB2C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//�Ͳ�B�����II�ζ�ֵ 50LB2C
LONG* RS_pn50LB2D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//�Ͳ�B�����II��ʱ�� 50LB2D
LONG* RS_pn50OLA1C 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//A�������I�ε��� 50OLA1C
LONG* RS_pn50OLA1D 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//A�������I��ʱ�� 50OLA1D
LONG* RS_pn50OLB1C 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//B�������I�ε��� 50OLB1C
LONG* RS_pn50OLB1D 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//B�������I��ʱ�� 50OLB1D
LONG* RS_pn50OLC1C 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//C�������I�ε��� 50OLC1C
LONG* RS_pn50OLC1D 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//C�������I��ʱ�� 50OLC1D
LONG* RS_pn50OLA2C 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//A�������II�ε��� 50OLA2C
LONG* RS_pn50OLA2D 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//A�������II��ʱ�� 50OLA2D
LONG* RS_pn50OLB2C 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//B�������II�ε��� 50OLB2C
LONG* RS_pn50OLB2D 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//B�������II��ʱ�� 50OLB2D
LONG* RS_pn50OLC2C 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//C�������II�ε��� 50OLC2C
LONG* RS_pn50OLC2D 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//C�������II��ʱ�� 50OLC2D
LONG* RS_pn50NC 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//���������ֵ 50NC
LONG* RS_pn50ND 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//�������ʱ�� 50ND
LONG* RS_pn59NV 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//�����ѹ��ֵ 59NV
LONG* RS_pn59ND 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//�����ѹʱ�� 59ND
LONG* RS_pn27HV 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//��ѹ��ʧѹ��ֵ 27HV
LONG* RS_pn27HD 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//��ѹ��ʧѹʱ�� 27HD
LONG* RS_pn27H_LK50 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//�߲�ʧѹ�������� 27H_LK50
LONG* RS_pn74HPTV 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//��ѹ��PT���߶�ֵ 74HPTV
LONG* RS_pn74LPTV 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//��ѹ��PT���߶�ֵ 74LPTV
LONG* RS_pn50DCGC 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//��϶������������ 50DCGC
LONG* RS_pn50DCGD 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//��϶��������ʱ�� 50DCGD
LONG* RS_pn74PTHC 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//�߲�PT���߱������� 74PTHC
LONG* RS_pn74PTLC 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//�Ͳ�PT���߱������� 74PTLC
LONG* RS_pn74PTD 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//PT�����ж�ʱ�� 74PTD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//ʧ�鱣���ж�ʱ�� 52BFD

//������ֵ----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//�����
LONG* AS_pnLCN 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//�Ͳ����·��
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
LONG* AS_pn50HAC 	= 	(LONG*)(AS_BUF_ADDR + 136); 	//IHA������ֵ
LONG* AS_pn37HAC 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//IHA������ֵ
LONG* AS_pn50HBC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IHB������ֵ
LONG* AS_pn37HBC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IHB������ֵ
LONG* AS_pn50HCC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//IHC������ֵ
LONG* AS_pn37HCC 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IHC������ֵ
LONG* AS_pn59HAV 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//UHA��ѹ��ֵ
LONG* AS_pn27HAV 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//UHA��ѹ��ֵ
LONG* AS_pn59HBV 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//UHB��ѹ��ֵ
LONG* AS_pn27HBV 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//UHB��ѹ��ֵ
LONG* AS_pn59HCV 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//UHC��ѹ��ֵ
LONG* AS_pn27HCV 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//UHC��ѹ��ֵ
LONG* AS_pn50LAC 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//ILA������ֵ
LONG* AS_pn37LAC 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//ILA������ֵ
LONG* AS_pn50LBC 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//ILB������ֵ
LONG* AS_pn37LBC 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//ILB������ֵ
LONG* AS_pn59LAV 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//ULA��ѹ��ֵ
LONG* AS_pn27LAV 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//ULA��ѹ��ֵ
LONG* AS_pn59LBV 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//ULB��ѹ��ֵ
LONG* AS_pn27LBV 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//ULB��ѹ��ֵ
LONG* AS_pn50NC 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//I0������ֵ
LONG* AS_pn37NC 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//I0������ֵ
LONG* AS_pn59NV 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//U0��ѹ��ֵ
LONG* AS_pn27NV 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//U0��ѹ��ֵ
LONG* AS_pn50JXC 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//IJX������ֵ
LONG* AS_pn37JXC 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//IJX������ֵ
LONG* AS_pnTCB 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//��·��������ʱ
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//����������ʱ
LONG* AS_pnTR1T 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//TR1����ʱ��
LONG* AS_pnTR2T 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//TR2����ʱ��
LONG* AS_pnTR3T 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//TR3����ʱ��
LONG* AS_pnTR4T 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//TR4����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnHPTRT 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//�߲�PT���
LONG* AS_pnHU0PTRT 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//�߲�����PT���
LONG* AS_pnHCTRT 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//�߲�CT���
LONG* AS_pnHI0CTRT 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//�߲�����CT���
LONG* AS_pnLPTRT 	= 	(LONG*)(AS_BUF_ADDR + 284); 	//�Ͳ�PT���
LONG* AS_pnLCT1RT 	= 	(LONG*)(AS_BUF_ADDR + 288); 	//�Ͳ�CT1���
LONG* AS_pnLCT2RT 	= 	(LONG*)(AS_BUF_ADDR + 292); 	//�Ͳ�CT2���
LONG* AS_pnLCT3RT 	= 	(LONG*)(AS_BUF_ADDR + 296); 	//�Ͳ�CT3���
LONG* AS_pnLCT4RT 	= 	(LONG*)(AS_BUF_ADDR + 300); 	//�Ͳ�CT4���
LONG* AS_pnRT1 	= 	(LONG*)(AS_BUF_ADDR + 304); 	//RT1

//ϵ��----------------------------------
LONG* CF_pnUHAB 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UHAB
LONG* CF_pnUHBC 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UHBC
LONG* CF_pnUHCA 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//UHCA
LONG* CF_pnUHA 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//UHA
LONG* CF_pnUHB 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//UHB
LONG* CF_pnUHC 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UHC
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//U0
LONG* CF_pnULA 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//ULA
LONG* CF_pnULB 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//ULB
LONG* CF_pnIHA 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IHA
LONG* CF_pnIHB 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IHB
LONG* CF_pnIHC 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IHC
LONG* CF_pnI0 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//I0
LONG* CF_pnIL1 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IL1
LONG* CF_pnIL2 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//IL2
LONG* CF_pnIL3 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//IL3
LONG* CF_pnIL4 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IL4
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//C4
LONG* CF_pnUHAM 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//UHAM
LONG* CF_pnUHBM 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//UHBM
LONG* CF_pnUHCM 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//UHCM
LONG* CF_pnU0M 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//U0M
LONG* CF_pnULAM 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//ULAM
LONG* CF_pnULBM 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//ULBM
LONG* CF_pnIHAM 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//IHAM
LONG* CF_pnIHBM 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//IHBM
LONG* CF_pnIHCM 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//IHCM
LONG* CF_pnI0M 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//I0M
LONG* CF_pnIL1M 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IL1M
LONG* CF_pnIL2M 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IL2M
LONG* CF_pnIL3M 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//IL3M
LONG* CF_pnIL4M 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//IL4M
LONG* CF_pnUHAD 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//UHAD
LONG* CF_pnUHBD 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//UHBD
LONG* CF_pnUHCD 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//UHCD
LONG* CF_pnU0D 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//U0D
LONG* CF_pnULAD 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//ULAD
LONG* CF_pnULBD 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//ULBD
LONG* CF_pnIHAD 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//IHAD
LONG* CF_pnIHBD 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//IHBD
LONG* CF_pnIHCD 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//IHCD
LONG* CF_pnI0D 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//I0D
LONG* CF_pnIL1D 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//IL1D
LONG* CF_pnIL2D 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//IL2D
LONG* CF_pnIL3D 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//IL3D
LONG* CF_pnIL4D 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//IL4D

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 92), // IN1
    (LONG*)(RM_BUF_ADDR + 96) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 84), // BR1
    (LONG*)(RM_BUF_ADDR + 88) // BR2
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

//��ʱ�޹����ɸ澯1100----------
const  LONG* FailureInf1100[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ʱ�޹����ɶ���1101----------
const  LONG* FailureInf1101[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ʱ�޹����ɷ���1102----------
const  LONG* FailureInf1102[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48) // IHC
};

//��ѹ��A��������1103----------
const  LONG* FailureInf1103[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��A��������1104----------
const  LONG* FailureInf1104[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��A��������1105----------
const  LONG* FailureInf1105[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��B��������1106----------
const  LONG* FailureInf1106[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��B��������1107----------
const  LONG* FailureInf1107[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��B��������1108----------
const  LONG* FailureInf1108[] = {
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��C��������1109----------
const  LONG* FailureInf1109[] = {
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��C��������1110----------
const  LONG* FailureInf1110[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��C��������1111----------
const  LONG* FailureInf1111[] = {
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��A�����������1112----------
const  LONG* FailureInf1112[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��A������ζ���1113----------
const  LONG* FailureInf1113[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��A������η���1114----------
const  LONG* FailureInf1114[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��A�����������1115----------
const  LONG* FailureInf1115[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��A������ζ���1116----------
const  LONG* FailureInf1116[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��A������η���1117----------
const  LONG* FailureInf1117[] = {
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��B�����������1118----------
const  LONG* FailureInf1118[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��B������ζ���1119----------
const  LONG* FailureInf1119[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��B������η���1120----------
const  LONG* FailureInf1120[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��B�����������1121----------
const  LONG* FailureInf1121[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��ѹ��B������ζ���1122----------
const  LONG* FailureInf1122[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��B������η���1123----------
const  LONG* FailureInf1123[] = {
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36) // ULAB
};

//��϶������������1124----------
const  LONG* FailureInf1124[] = {
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//��϶������������1125----------
const  LONG* FailureInf1125[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��϶������������1126----------
const  LONG* FailureInf1126[] = {
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//���������������1127----------
const  LONG* FailureInf1127[] = {
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//���������������1128----------
const  LONG* FailureInf1128[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//���������������1129----------
const  LONG* FailureInf1129[] = {
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//�����ѹ��������1130----------
const  LONG* FailureInf1130[] = {
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//�����ѹ��������1131----------
const  LONG* FailureInf1131[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�����ѹ��������1132----------
const  LONG* FailureInf1132[] = {
    (LONG*)(RM_BUF_ADDR + 24) // U0
};

//�߲�A�����ɢ�澯1133----------
const  LONG* FailureInf1133[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲�B�����ɢ�澯1134----------
const  LONG* FailureInf1134[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲�C�����ɢ�澯1135----------
const  LONG* FailureInf1135[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲�A�����ɢ�����1136----------
const  LONG* FailureInf1136[] = {
    (LONG*)(RM_BUF_ADDR + 40) // IHA
};

//�߲�A�����ɢ���1137----------
const  LONG* FailureInf1137[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲�A�����ɢ򷵻�1138----------
const  LONG* FailureInf1138[] = {
    (LONG*)(RM_BUF_ADDR + 40) // IHA
};

//�߲�B�����ɢ�����1139----------
const  LONG* FailureInf1139[] = {
    (LONG*)(RM_BUF_ADDR + 44) // IHB
};

//�߲�B�����ɢ���1140----------
const  LONG* FailureInf1140[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲�B�����ɢ򷵻�1141----------
const  LONG* FailureInf1141[] = {
    (LONG*)(RM_BUF_ADDR + 44) // IHB
};

//�߲�C�����ɢ�����1142----------
const  LONG* FailureInf1142[] = {
    (LONG*)(RM_BUF_ADDR + 48) // IHC
};

//�߲�C�����ɢ���1143----------
const  LONG* FailureInf1143[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲�C�����ɢ򷵻�1144----------
const  LONG* FailureInf1144[] = {
    (LONG*)(RM_BUF_ADDR + 48) // IHC
};

//��ѹ��ʧѹ����1145----------
const  LONG* FailureInf1145[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��PT����1146----------
const  LONG* FailureInf1146[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��PT���߻ָ�1147----------
const  LONG* FailureInf1147[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��A��PT����1148----------
const  LONG* FailureInf1148[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��A��PT���߻ָ�1149----------
const  LONG* FailureInf1149[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��B��PT����1150----------
const  LONG* FailureInf1150[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��ѹ��B��PT���߻ָ�1151----------
const  LONG* FailureInf1151[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//��������1152----------
const  LONG* FailureInf1152[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�߲��·����բʧ��1153----------
const  LONG* FailureInf1153[] = {
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//ͨ������1154----------
const  LONG* FailureInf1154[] = {
    (LONG*)(RM_BUF_ADDR + 0), // UHAB
    (LONG*)(RM_BUF_ADDR + 4), // UHBC
    (LONG*)(RM_BUF_ADDR + 8), // UHCA
    (LONG*)(RM_BUF_ADDR + 12), // UHA
    (LONG*)(RM_BUF_ADDR + 16), // UHB
    (LONG*)(RM_BUF_ADDR + 20), // UHC
    (LONG*)(RM_BUF_ADDR + 24), // U0
    (LONG*)(RM_BUF_ADDR + 28), // ULA
    (LONG*)(RM_BUF_ADDR + 32), // ULB
    (LONG*)(RM_BUF_ADDR + 36), // ULAB
    (LONG*)(RM_BUF_ADDR + 40), // IHA
    (LONG*)(RM_BUF_ADDR + 44), // IHB
    (LONG*)(RM_BUF_ADDR + 48), // IHC
    (LONG*)(RM_BUF_ADDR + 52), // I0
    (LONG*)(RM_BUF_ADDR + 56), // IJX
    (LONG*)(RM_BUF_ADDR + 60), // ILA
    (LONG*)(RM_BUF_ADDR + 64), // ILB
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//ʧ�鱣��1155----------
const  LONG* FailureInf1155[] = {
    {NULL}
};

//�Ͳ�A���·����բʧ��1156----------
const  LONG* FailureInf1156[] = {
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
};

//�Ͳ�B���·����բʧ��1157----------
const  LONG* FailureInf1157[] = {
    (LONG*)(RM_BUF_ADDR + 68), // IL1
    (LONG*)(RM_BUF_ADDR + 72), // IL2
    (LONG*)(RM_BUF_ADDR + 76), // IL3
    (LONG*)(RM_BUF_ADDR + 80) // IL4
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
    {21, FailureInf1100, 1100},//��ʱ�޹����ɸ澯
    {21, FailureInf1101, 1101},//��ʱ�޹����ɶ���
    {3, FailureInf1102, 1102},//��ʱ�޹����ɷ���
    {4, FailureInf1103, 1103},//��ѹ��A��������
    {21, FailureInf1104, 1104},//��ѹ��A��������
    {4, FailureInf1105, 1105},//��ѹ��A��������
    {4, FailureInf1106, 1106},//��ѹ��B��������
    {21, FailureInf1107, 1107},//��ѹ��B��������
    {4, FailureInf1108, 1108},//��ѹ��B��������
    {4, FailureInf1109, 1109},//��ѹ��C��������
    {21, FailureInf1110, 1110},//��ѹ��C��������
    {4, FailureInf1111, 1111},//��ѹ��C��������
    {4, FailureInf1112, 1112},//��ѹ��A�����������
    {21, FailureInf1113, 1113},//��ѹ��A������ζ���
    {4, FailureInf1114, 1114},//��ѹ��A������η���
    {4, FailureInf1115, 1115},//��ѹ��A�����������
    {21, FailureInf1116, 1116},//��ѹ��A������ζ���
    {4, FailureInf1117, 1117},//��ѹ��A������η���
    {4, FailureInf1118, 1118},//��ѹ��B�����������
    {21, FailureInf1119, 1119},//��ѹ��B������ζ���
    {4, FailureInf1120, 1120},//��ѹ��B������η���
    {4, FailureInf1121, 1121},//��ѹ��B�����������
    {21, FailureInf1122, 1122},//��ѹ��B������ζ���
    {4, FailureInf1123, 1123},//��ѹ��B������η���
    {2, FailureInf1124, 1124},//��϶������������
    {21, FailureInf1125, 1125},//��϶������������
    {2, FailureInf1126, 1126},//��϶������������
    {2, FailureInf1127, 1127},//���������������
    {21, FailureInf1128, 1128},//���������������
    {2, FailureInf1129, 1129},//���������������
    {1, FailureInf1130, 1130},//�����ѹ��������
    {21, FailureInf1131, 1131},//�����ѹ��������
    {1, FailureInf1132, 1132},//�����ѹ��������
    {21, FailureInf1133, 1133},//�߲�A�����ɢ�澯
    {21, FailureInf1134, 1134},//�߲�B�����ɢ�澯
    {21, FailureInf1135, 1135},//�߲�C�����ɢ�澯
    {1, FailureInf1136, 1136},//�߲�A�����ɢ�����
    {21, FailureInf1137, 1137},//�߲�A�����ɢ���
    {1, FailureInf1138, 1138},//�߲�A�����ɢ򷵻�
    {1, FailureInf1139, 1139},//�߲�B�����ɢ�����
    {21, FailureInf1140, 1140},//�߲�B�����ɢ���
    {1, FailureInf1141, 1141},//�߲�B�����ɢ򷵻�
    {1, FailureInf1142, 1142},//�߲�C�����ɢ�����
    {21, FailureInf1143, 1143},//�߲�C�����ɢ���
    {1, FailureInf1144, 1144},//�߲�C�����ɢ򷵻�
    {21, FailureInf1145, 1145},//��ѹ��ʧѹ����
    {21, FailureInf1146, 1146},//��ѹ��PT����
    {21, FailureInf1147, 1147},//��ѹ��PT���߻ָ�
    {21, FailureInf1148, 1148},//��ѹ��A��PT����
    {21, FailureInf1149, 1149},//��ѹ��A��PT���߻ָ�
    {21, FailureInf1150, 1150},//��ѹ��B��PT����
    {21, FailureInf1151, 1151},//��ѹ��B��PT���߻ָ�
    {21, FailureInf1152, 1152},//��������
    {8, FailureInf1153, 1153},//�߲��·����բʧ��
    {21, FailureInf1154, 1154},//ͨ������
    {0, FailureInf1155, 1155},//ʧ�鱣��
    {4, FailureInf1156, 1156},//�Ͳ�A���·����բʧ��
    {4, FailureInf1157, 1157},//�Ͳ�B���·����բʧ��
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 240) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 244) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 248) ,
	 (LONG*)(AS_BUF_ADDR + 252) ,
	 (LONG*)(AS_BUF_ADDR + 256) ,
	 (LONG*)(AS_BUF_ADDR + 260) ,
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
	 (WORD)(68) ,
	 (WORD)(69) ,
	 (WORD)(70) ,
	 (WORD)(71) ,
	 (WORD)(72) ,
	 (WORD)(73) ,
	 (WORD)(74) ,
	 (WORD)(75) ,
	 (WORD)(76) ,
	 (WORD)(77) ,
	 (WORD)(78) ,
	 (WORD)(79) ,
	 (WORD)(80) ,
	 (WORD)(81) ,
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
	 (WORD)(30) ,
	 (WORD)(31) ,
	 (WORD)(32) ,
	 (WORD)(33) ,
	 (WORD)(34) ,
	 (WORD)(35) ,
	 (WORD)(36) ,
	 (WORD)(37) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 7) ,
	 (BYTE*)(CFG_BUF_ADDR + 8) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 6) ,
	 (BYTE*)(CFG_BUF_ADDR + 9) ,
	 (BYTE*)(CFG_BUF_ADDR + 22) ,
	 (BYTE*)(CFG_BUF_ADDR + 23) ,
	 (BYTE*)(CFG_BUF_ADDR + 24) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	11500, //UHAB
	11500, //UHBC
	11500, //UHCA
	11500, //UHA
	11500, //UHB
	11500, //UHC
	11500, //U0
	11500, //ULA
	11500, //ULB
	3050, //IHA
	3050, //IHB
	3050, //IHC
	3050, //I0
	3050, //IL1
	3050, //IL2
	3050, //IL3
	3050, //IL4
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	16100, //UHAM
	16100, //UHBM
	16100, //UHCM
	16100, //U0M
	16100, //ULAM
	16100, //ULBM
	4334, //IHAM
	4334, //IHBM
	4334, //IHCM
	4334, //I0M
	4334, //IL1M
	4334, //IL2M
	4334, //IL3M
	4334, //IL4M
	21, //UHAD
	21, //UHBD
	21, //UHCD
	21, //U0D
	21, //ULAD
	21, //ULBD
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //I0D
	21, //IL1D
	21, //IL2D
	21, //IL3D
	21, //IL4D
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	11500, //UHAB
	11500, //UHBC
	11500, //UHCA
	11500, //UHA
	11500, //UHB
	11500, //UHC
	11500, //U0
	11500, //ULA
	11500, //ULB
	15250, //IHA
	15250, //IHB
	15250, //IHC
	15250, //I0
	15250, //IL1
	15250, //IL2
	15250, //IL3
	15250, //IL4
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	16100, //UHAM
	16100, //UHBM
	16100, //UHCM
	16100, //U0M
	16100, //ULAM
	16100, //ULBM
	21670, //IHAM
	21670, //IHBM
	21670, //IHCM
	21670, //I0M
	21670, //IL1M
	21670, //IL2M
	21670, //IL3M
	21670, //IL4M
	21, //UHAD
	21, //UHBD
	21, //UHCD
	21, //U0D
	21, //ULAD
	21, //ULBD
	21, //IHAD
	21, //IHBD
	21, //IHCD
	21, //I0D
	21, //IL1D
	21, //IL2D
	21, //IL3D
	21, //IL4D
};

//����----------------------------------
