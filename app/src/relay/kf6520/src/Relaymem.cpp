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
	{FOREWORD, "������"},
	{BACKWORD, "������"},
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
	{2, (TSetValueInf*)&(SetValueInf[0])}, //��ֵ����귽��
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
	{BTMODE, "ֱ��"},
	{AT1MODE, "AT"},
	{AT2MODE, "��ר"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //������ֵ�����
	{3, (TSetValueInf*)&(SysSetValueInf[2])}, //������ֵ���緽ʽ
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnIp 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// Ip
LONG* 	RM_pnIt 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// It
LONG* 	RM_pnIf 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// If
LONG* 	RM_pnUp 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// Up
LONG* 	RM_pnU1 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// U1
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// U2
LONG* 	RM_pnU3 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// U3
LONG* 	RM_pnU4 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// U4
LONG* 	RM_pnTL 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// TL
LONG* 	RM_pnFL 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// FL
LONG* 	RM_pnSS 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// SS
LONG* 	RM_pnI2 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// I2
LONG* 	RM_pnI3 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// I3
LONG* 	RM_pnI5 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// I5
LONG* 	RM_pnItm 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// Itm
LONG* 	RM_pnIfm 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// Ifm
LONG* 	RM_pnR 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// R
LONG* 	RM_pnX 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// X
LONG* 	RM_pnZA 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// ��Z
LONG* 	RM_pnTR 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// TR
LONG* 	RM_pnTX 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// TX
LONG* 	RM_pnTZA 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// ��T
LONG* 	RM_pnFR 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// FR
LONG* 	RM_pnFX 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// FX
LONG* 	RM_pnFZA 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// ��F
LONG* 	RM_pnAUP 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// ��UP
LONG* 	RM_pnAU1 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// ��U1
LONG* 	RM_pnAU2 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// ��U2
LONG* 	RM_pnAIP 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// ��IP
LONG* 	RM_pnAIT 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// ��IT
LONG* 	RM_pnAIF 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// ��IF
LONG* 	RM_pnAITM 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// ��ItM
LONG* 	RM_pnAIFM 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// ��IfM
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnU1 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//U1 U1
LONG* 	M_pnU2 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//U2 U2
LONG* 	M_pnU3 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//U3 U3
LONG* 	M_pnU4 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U4 U4
LONG* 	M_pnIT 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//IT IT
LONG* 	M_pnIF 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//IF IF
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//COS COS
LONG* 	M_pnI1 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//I1 I1
LONG* 	M_pnI2 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//I2 I2
LONG* 	M_pnI3 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//I3 I3
LONG* 	M_pnI5 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//I5 I5
LONG* 	M_pnI7 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I7 I7
LONG* 	M_pnI9 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I9 I9
LONG* 	M_pnI11 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//I11 I11
LONG* 	M_pnR 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//R R
LONG* 	M_pnX 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//X X
LONG* 	M_pnF 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//F F


//��ѹ��----------------------------------
BYTE* SW_pby21 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//���뱣��
BYTE* SW_pby50_0 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//�����ٶ�
BYTE* SW_pby50_123 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//��������
BYTE* SW_pby50D 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//������������
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//��ѹ����
BYTE* SW_pby79 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//�غ�բ
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//����ѹ��
BYTE* SW_pbyRESD1 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//����ѹ��1
BYTE* SW_pbyRESD2 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby21_1 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//����I��
BYTE* CFG_pby21_1_D 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//����I������
BYTE* CFG_pby21_2 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//����II��
BYTE* CFG_pby21_2_D 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//����II������
BYTE* CFG_pby21_3 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//����III��
BYTE* CFG_pby21_3_D 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//����III������
BYTE* CFG_pby21_4 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//����IV��
BYTE* CFG_pby21_4_D 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//����IV������
BYTE* CFG_pbyParallelC 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//ƽ���ı�������
BYTE* CFG_pby21_HLK 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//����г������
BYTE* CFG_pby21_HRES 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//����г������
BYTE* CFG_pby50_0 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//�����ٶ�
BYTE* CFG_pby50_0_27 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//�ٶϵ�ѹ����
BYTE* CFG_pby50_0_HLK 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//�ٶ�г������
BYTE* CFG_pby50_0_HRES 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//�ٶ�г������
BYTE* CFG_pby50_0_D 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//�ٶϱ���������
BYTE* CFG_pby50_0_67 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//�ٶϹ��ʷ���
BYTE* CFG_pby50_1 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//����I��
BYTE* CFG_pby50_1_D 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//����I��������
BYTE* CFG_pby50_1_67 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//����I�ι��ʷ���
BYTE* CFG_pby50_1_27 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//����I�ε�ѹ����
BYTE* CFG_pby50_2 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//����II��
BYTE* CFG_pby50_2_D 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//����II��������
BYTE* CFG_pby50_2_67 	= 	(BYTE*)(CFG_BUF_ADDR + 23); 	//����II�ι��ʷ���
BYTE* CFG_pby50_2_27 	= 	(BYTE*)(CFG_BUF_ADDR + 24); 	//����II�ε�ѹ����
BYTE* CFG_pby50_3 	= 	(BYTE*)(CFG_BUF_ADDR + 25); 	//����III��
BYTE* CFG_pby50_HLK 	= 	(BYTE*)(CFG_BUF_ADDR + 26); 	//����г������
BYTE* CFG_pby50_HRES 	= 	(BYTE*)(CFG_BUF_ADDR + 27); 	//����г������
BYTE* CFG_pby51 	= 	(BYTE*)(CFG_BUF_ADDR + 28); 	//��ʱ�޹���
BYTE* CFG_pby50D 	= 	(BYTE*)(CFG_BUF_ADDR + 29); 	//������������
BYTE* CFG_pby79UC 	= 	(BYTE*)(CFG_BUF_ADDR + 30); 	//͵���غ�բ
BYTE* CFG_pby79_21 	= 	(BYTE*)(CFG_BUF_ADDR + 31); 	//���뱣���غ�
BYTE* CFG_pby79_50_0 	= 	(BYTE*)(CFG_BUF_ADDR + 32); 	//�����ٶ��غ�
BYTE* CFG_pby79_50_123 	= 	(BYTE*)(CFG_BUF_ADDR + 33); 	//���������غ�
BYTE* CFG_pby79_51 	= 	(BYTE*)(CFG_BUF_ADDR + 34); 	//��ʱ���غ�
BYTE* CFG_pby79_50D 	= 	(BYTE*)(CFG_BUF_ADDR + 35); 	//���������غ�
BYTE* CFG_pby79_59 	= 	(BYTE*)(CFG_BUF_ADDR + 36); 	//�غ�բ����ѹ
BYTE* CFG_pby21_2_94 	= 	(BYTE*)(CFG_BUF_ADDR + 37); 	//����II�μ���
BYTE* CFG_pby21_3_94 	= 	(BYTE*)(CFG_BUF_ADDR + 38); 	//����III�μ���
BYTE* CFG_pby21_4_94 	= 	(BYTE*)(CFG_BUF_ADDR + 39); 	//����IV�μ���
BYTE* CFG_pby50_1_94 	= 	(BYTE*)(CFG_BUF_ADDR + 40); 	//����I�μ���
BYTE* CFG_pby50_2_94 	= 	(BYTE*)(CFG_BUF_ADDR + 41); 	//����II�μ���
BYTE* CFG_pby50_3_94 	= 	(BYTE*)(CFG_BUF_ADDR + 42); 	//����III�μ���
BYTE* CFG_pby27 	= 	(BYTE*)(CFG_BUF_ADDR + 43); 	//��ѹ����
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 44); 	//ʧ�鱣��
BYTE* CFG_pbyPT1BR 	= 	(BYTE*)(CFG_BUF_ADDR + 45); 	//PT1���߼��
BYTE* CFG_pbyPT2BR 	= 	(BYTE*)(CFG_BUF_ADDR + 46); 	//PT2���߼��
BYTE* CFG_pby21_FL 	= 	(BYTE*)(CFG_BUF_ADDR + 47); 	//���ϲ��
BYTE* CFG_pbyLOADREC 	= 	(BYTE*)(CFG_BUF_ADDR + 48); 	//����¼��
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 49); 	//CT���߼��
BYTE* CFG_pbyRESD1 	= 	(BYTE*)(CFG_BUF_ADDR + 50); 	//��������1
BYTE* CFG_pbyRESD2 	= 	(BYTE*)(CFG_BUF_ADDR + 51); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnFUNCTION1 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������1 FUNCTION1
LONG* RS_pnFUNCTION2 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//������2 FUNCTION2
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//ͻ������������ PIC
LONG* RS_pn21_1R 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//����I�ε��� 21_1R
LONG* RS_pn21_1X 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//����I�ε翹 21_1X
LONG* RS_pn21_1D 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//����I��ʱ�� 21_1D
LONG* RS_pn21_2R 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//����II�ε��� 21_2R
LONG* RS_pn21_2X 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//����II�ε翹 21_2X
LONG* RS_pn21_2D 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//����II��ʱ�� 21_2D
LONG* RS_pn21_3R 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//����III�ε��� 21_3R
LONG* RS_pn21_3X 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//����III�ε翹 21_3X
LONG* RS_pn21_3D 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//����III��ʱ�� 21_3D
LONG* RS_pn21_4R 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//����IV�ε��� 21_4R
LONG* RS_pn21_4X 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//����IV�ε翹 21_4X
LONG* RS_pn21_4D 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//����IV��ʱ�� 21_4D
LONG* RS_pn21ZOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//ƽ���ı���ƫ���迹 21ZOFFSET
LONG* RS_pn21AOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//�迹�ı���ƫ�ƽǶ� 21AOFFSET
LONG* RS_pn50_0C 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//�����ٶϵ��� 50_0C
LONG* RS_pn50_0D 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//�����ٶ�ʱ�� 50_0D
LONG* RS_pn50_0_27V 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//�ٶϵ�ѹ���� 50_0_27V
LONG* RS_pn50_1C 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//����I�ε��� 50_1C
LONG* RS_pn50_1D 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//����I��ʱ�� 50_1D
LONG* RS_pn50_1_27V 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//����I�ε�ѹ���� 50_1_27V
LONG* RS_pn50_2C 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//����II�ε��� 50_2C
LONG* RS_pn50_2D 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//����II��ʱ�� 50_2D
LONG* RS_pn50_2_27V 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//����II�ε�ѹ���� 50_2_27V
LONG* RS_pn50_3C 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//����III�ε��� 50_3C
LONG* RS_pn50_3D 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//����III��ʱ�� 50_3D
LONG* RS_pn51C 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//��ʱ�޹������� 51C
LONG* RS_pn51TD 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//��ʱ�޹���ʱ�� 51TD
LONG* RS_pn51TYPE 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//��ʱ���������� 51TYPE
LONG* RS_pn50DC 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//���������������� 50DC
LONG* RS_pn50DD 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//������������ʱ�� 50DD
LONG* RS_pn50D_37C 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//��������С�������� 50D_37C
LONG* RS_pnH2_PCT 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//����г������ϵ�� H2_PCT
LONG* RS_pnFACTOR1 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//�������г����Ȩ FACTOR1
LONG* RS_pnFACTOR2 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//��������г����Ȩ FACTOR2
LONG* RS_pnZANGLE 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//��·�迹�� ZANGLE
LONG* RS_pn79SH 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//�غ�բͶ����� 79SH
LONG* RS_pn79D1 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//��һ���غ�բʱ�� 79D1
LONG* RS_pn79D2 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//�ڶ����غ�բʱ�� 79D2
LONG* RS_pn79RD 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//�غ�բ���ʱ�� 79RD
LONG* RS_pn79CLSD 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//�غ�բ��������ʱ�� 79CLSD
LONG* RS_pn94D 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//����ٶ���ʱ�� 94D
LONG* RS_pnSOTFD 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//�����Ͷ��ʱ�� SOTFD
LONG* RS_pn27V 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//��ѹ������ѹ 27V
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//��ѹ����ʱ�� 27D
LONG* RS_pn27_37C 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//��ѹ�������� 27_37C
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//ʧ�鱣��ʱ�� 52BFD
LONG* RS_pn74VTV 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//PT���߼���ѹ 74VTV
LONG* RS_pn74VTBLC 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//PT���߱������� 74VTBLC
LONG* RS_pn74VTC 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//PT���߼����� 74VTC
LONG* RS_pn74VTD 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//PT���߼��ʱ�� 74VTD
LONG* RS_pnS0 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//����� S0
LONG* RS_pnSD 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//����귽�� SD
LONG* RS_pnLc 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//�������� Lc
LONG* RS_pnT21FLN 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//T�߲��ֶ��� T21FLN
LONG* RS_pnT21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//T�ߵ�һ�ξ��� T21FLN1L
LONG* RS_pnT21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//T�ߵ�һ�ε�λ�翹 T21FLN1X
LONG* RS_pnT21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//T�ߵڶ��ξ��� T21FLN2L
LONG* RS_pnT21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//T�ߵڶ��ε�λ�翹 T21FLN2X
LONG* RS_pnT21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//T�ߵ����ξ��� T21FLN3L
LONG* RS_pnT21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//T�ߵ����ε�λ�翹 T21FLN3X
LONG* RS_pnT21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//T�ߵ��Ķξ��� T21FLN4L
LONG* RS_pnT21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//T�ߵ��Ķε�λ�翹 T21FLN4X
LONG* RS_pnT21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//T�ߵ���ξ��� T21FLN5L
LONG* RS_pnT21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//T�ߵ���ε�λ�翹 T21FLN5X
LONG* RS_pnT21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//T�ߵ����ξ��� T21FLN6L
LONG* RS_pnT21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//T�ߵ����ε�λ�翹 T21FLN6X
LONG* RS_pnT21FLN7L 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//T�ߵ��߶ξ��� T21FLN7L
LONG* RS_pnT21FLN7X 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//T�ߵ��߶ε�λ�翹 T21FLN7X
LONG* RS_pnT21FLN8L 	= 	(LONG*)(RS_BUF_ADDR + 284); 	//T�ߵڰ˶ξ��� T21FLN8L
LONG* RS_pnT21FLN8X 	= 	(LONG*)(RS_BUF_ADDR + 288); 	//T�ߵڰ˶ε�λ�翹 T21FLN8X
LONG* RS_pnT21FLN9L 	= 	(LONG*)(RS_BUF_ADDR + 292); 	//T�ߵھŶξ��� T21FLN9L
LONG* RS_pnT21FLN9X 	= 	(LONG*)(RS_BUF_ADDR + 296); 	//T�ߵھŶε�λ�翹 T21FLN9X
LONG* RS_pnT21FLN10L 	= 	(LONG*)(RS_BUF_ADDR + 300); 	//T�ߵ�ʮ�ξ��� T21FLN10L
LONG* RS_pnT21FLN10X 	= 	(LONG*)(RS_BUF_ADDR + 304); 	//T�ߵ�ʮ�ε�λ�翹 T21FLN10X
LONG* RS_pnF21FLN 	= 	(LONG*)(RS_BUF_ADDR + 308); 	//F�߲��ֶ��� F21FLN
LONG* RS_pnF21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 312); 	//F�ߵ�һ�ξ��� F21FLN1L
LONG* RS_pnF21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 316); 	//F�ߵ�һ�ε�λ�翹 F21FLN1X
LONG* RS_pnF21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 320); 	//F�ߵڶ��ξ��� F21FLN2L
LONG* RS_pnF21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 324); 	//F�ߵڶ��ε�λ�翹 F21FLN2X
LONG* RS_pnF21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 328); 	//F�ߵ����ξ��� F21FLN3L
LONG* RS_pnF21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 332); 	//F�ߵ����ε�λ�翹 F21FLN3X
LONG* RS_pnF21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 336); 	//F�ߵ��Ķξ��� F21FLN4L
LONG* RS_pnF21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 340); 	//F�ߵ��Ķε�λ�翹 F21FLN4X
LONG* RS_pnF21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 344); 	//F�ߵ���ξ��� F21FLN5L
LONG* RS_pnF21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 348); 	//F�ߵ���ε�λ�翹 F21FLN5X
LONG* RS_pnF21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 352); 	//F�ߵ����ξ��� F21FLN6L
LONG* RS_pnF21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 356); 	//F�ߵ����ε�λ�翹 F21FLN6X
LONG* RS_pnF21FLN7L 	= 	(LONG*)(RS_BUF_ADDR + 360); 	//F�ߵ��߶ξ��� F21FLN7L
LONG* RS_pnF21FLN7X 	= 	(LONG*)(RS_BUF_ADDR + 364); 	//F�ߵ��߶ε�λ�翹 F21FLN7X
LONG* RS_pnF21FLN8L 	= 	(LONG*)(RS_BUF_ADDR + 368); 	//F�ߵڰ˶ξ��� F21FLN8L
LONG* RS_pnF21FLN8X 	= 	(LONG*)(RS_BUF_ADDR + 372); 	//F�ߵڰ˶ε�λ�翹 F21FLN8X
LONG* RS_pnF21FLN9L 	= 	(LONG*)(RS_BUF_ADDR + 376); 	//F�ߵھŶξ��� F21FLN9L
LONG* RS_pnF21FLN9X 	= 	(LONG*)(RS_BUF_ADDR + 380); 	//F�ߵھŶε�λ�翹 F21FLN9X
LONG* RS_pnF21FLN10L 	= 	(LONG*)(RS_BUF_ADDR + 384); 	//F�ߵ�ʮ�ξ��� F21FLN10L
LONG* RS_pnF21FLN10X 	= 	(LONG*)(RS_BUF_ADDR + 388); 	//F�ߵ�ʮ�ε�λ�翹 F21FLN10X

//������ֵ----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//�����
LONG* AS_pnPM 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//���緽ʽ
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
LONG* AS_pn50IPC 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//IP������ֵ
LONG* AS_pn37IPC 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//IP������ֵ
LONG* AS_pn50ITC 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//It������ֵ
LONG* AS_pn37ITC 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//It������ֵ
LONG* AS_pn50IFC 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//If������ֵ
LONG* AS_pn37IFC 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//If������ֵ
LONG* AS_pn59UPV 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//Up��ѹ��ֵ
LONG* AS_pn27UPV 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//Up��ѹ��ֵ
LONG* AS_pn59U1V 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//U1��ѹ��ֵ
LONG* AS_pn27U1V 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//U1��ѹ��ֵ
LONG* AS_pn59U2V 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//U2��ѹ��ֵ
LONG* AS_pn27U2V 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//U2��ѹ��ֵ
LONG* AS_pn59U3V 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//U3��ѹ��ֵ
LONG* AS_pn27U3V 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U3��ѹ��ֵ
LONG* AS_pn59U4V 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U4��ѹ��ֵ
LONG* AS_pn27U4V 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U4��ѹ��ֵ
LONG* AS_pnLMT 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//���ɼ����ʼʱ��
LONG* AS_pnLMCMAX 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//���ɼ�������ֵ
LONG* AS_pnLMVMIN 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//���ɼ���ѹ��ֵ
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//TR2����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//����¼������ʱ��
LONG* AS_pnC_THRD 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//�����仯��¼��ֵ
LONG* AS_pnV_THRD 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//��ѹ�仯��¼��ֵ

//���----------------------------------
LONG* AS_pnPT1Ratio 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//PT1���
LONG* AS_pnPT2Ratio 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//PT2���
LONG* AS_pnPT3Ratio 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//PT3���
LONG* AS_pnPT4Ratio 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//PT4���
LONG* AS_pnPCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//����CT���
LONG* AS_pnMCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//����CT���
LONG* AS_pnZRatio 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//�迹���
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//rt1

//ϵ��----------------------------------
LONG* CF_pnIP 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//IP
LONG* CF_pnUP 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UP
LONG* CF_pnU3 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U3
LONG* CF_pnU4 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//U4
LONG* CF_pnITM 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//ITM
LONG* CF_pnIFM 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//IFM
LONG* CF_pnU1M 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//U1M
LONG* CF_pnU2M 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//U2M
LONG* CF_pnU3M 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//U3M
LONG* CF_pnU4M 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//U4M
LONG* CF_pnP 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//P
LONG* CF_pnQ 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//Q
LONG* CF_pnA 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//A
LONG* CF_pnZ 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//Z
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//C4
LONG* CF_pnITDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//ITDRIFT
LONG* CF_pnIFDRIFT 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//IFDRIFT
LONG* CF_pnU1DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//U1DRIFT
LONG* CF_pnU2DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//U2DRIFT
LONG* CF_pnU3DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//U3DRIFT
LONG* CF_pnU4DRIFT 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//U4DRIFT

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 140), // IN1
    (LONG*)(RM_BUF_ADDR + 144) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 132), // BR1
    (LONG*)(RM_BUF_ADDR + 136) // BR2
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

//����������2000----------
const  LONG* FailureInf2000[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2001----------
const  LONG* FailureInf2001[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2002----------
const  LONG* FailureInf2002[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������2003----------
const  LONG* FailureInf2003[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2004----------
const  LONG* FailureInf2004[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2005----------
const  LONG* FailureInf2005[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������2006----------
const  LONG* FailureInf2006[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2007----------
const  LONG* FailureInf2007[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2008----------
const  LONG* FailureInf2008[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2009----------
const  LONG* FailureInf2009[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2010----------
const  LONG* FailureInf2010[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2011----------
const  LONG* FailureInf2011[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ�������2012----------
const  LONG* FailureInf2012[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ��ٶ���2013----------
const  LONG* FailureInf2013[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����μ��ٷ���2014----------
const  LONG* FailureInf2014[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ�������2015----------
const  LONG* FailureInf2015[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ��ٶ���2016----------
const  LONG* FailureInf2016[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����μ��ٷ���2017----------
const  LONG* FailureInf2017[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2018----------
const  LONG* FailureInf2018[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2019----------
const  LONG* FailureInf2019[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2020----------
const  LONG* FailureInf2020[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ٶ�����2021----------
const  LONG* FailureInf2021[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ٶ϶���2022----------
const  LONG* FailureInf2022[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����ٶϷ���2023----------
const  LONG* FailureInf2023[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2024----------
const  LONG* FailureInf2024[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2025----------
const  LONG* FailureInf2025[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2026----------
const  LONG* FailureInf2026[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2027----------
const  LONG* FailureInf2027[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2028----------
const  LONG* FailureInf2028[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2029----------
const  LONG* FailureInf2029[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2030----------
const  LONG* FailureInf2030[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2031----------
const  LONG* FailureInf2031[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2032----------
const  LONG* FailureInf2032[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2033----------
const  LONG* FailureInf2033[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2034----------
const  LONG* FailureInf2034[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2035----------
const  LONG* FailureInf2035[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2036----------
const  LONG* FailureInf2036[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2037----------
const  LONG* FailureInf2037[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2038----------
const  LONG* FailureInf2038[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2039----------
const  LONG* FailureInf2039[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2040----------
const  LONG* FailureInf2040[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2041----------
const  LONG* FailureInf2041[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������������2042----------
const  LONG* FailureInf2042[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������������2043----------
const  LONG* FailureInf2043[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//����������������2044----------
const  LONG* FailureInf2044[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ʱ�޹�������2045----------
const  LONG* FailureInf2045[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ʱ�޹�������2046----------
const  LONG* FailureInf2046[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//��ʱ�޹�������2047----------
const  LONG* FailureInf2047[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ѹ������բ2048----------
const  LONG* FailureInf2048[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//��ѹ�����澯2049----------
const  LONG* FailureInf2049[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//͵���غ�բ2050----------
const  LONG* FailureInf2050[] = {
    {NULL}
};

//һ���غ�բ����2051----------
const  LONG* FailureInf2051[] = {
    {NULL}
};

//�����غ�բ����2052----------
const  LONG* FailureInf2052[] = {
    {NULL}
};

//PT���߱������뱣��2053----------
const  LONG* FailureInf2053[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//һ���غ�բ������2054----------
const  LONG* FailureInf2054[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20) // U2
};

//�����غ�բ������2250----------
const  LONG* FailureInf2250[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20) // U2
};

//ʧ�鱣������2055----------
const  LONG* FailureInf2055[] = {
    {NULL}
};

//PT1����2056----------
const  LONG* FailureInf2056[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//PT2����2057----------
const  LONG* FailureInf2057[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//PT1���߷���2058----------
const  LONG* FailureInf2058[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//PT2���߷���2059----------
const  LONG* FailureInf2059[] = {
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//�غ�բʧ��2060----------
const  LONG* FailureInf2060[] = {
    {NULL}
};

//��������2061----------
const  LONG* FailureInf2061[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//��·����բʧ��2062----------
const  LONG* FailureInf2062[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8) // If
};

//ͨ������2063----------
const  LONG* FailureInf2063[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 16), // U1
    (LONG*)(RM_BUF_ADDR + 20), // U2
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52), // I5
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 36), // FL
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 76), // TR
    (LONG*)(RM_BUF_ADDR + 80), // TX
    (LONG*)(RM_BUF_ADDR + 84), // ��T
    (LONG*)(RM_BUF_ADDR + 88), // FR
    (LONG*)(RM_BUF_ADDR + 92), // FX
    (LONG*)(RM_BUF_ADDR + 96), // ��F
    (LONG*)(RM_BUF_ADDR + 100), // ��UP
    (LONG*)(RM_BUF_ADDR + 104), // ��U1
    (LONG*)(RM_BUF_ADDR + 108), // ��U2
    (LONG*)(RM_BUF_ADDR + 112), // ��IP
    (LONG*)(RM_BUF_ADDR + 116), // ��IT
    (LONG*)(RM_BUF_ADDR + 120) // ��IF
};

//CT1���߸澯2064----------
const  LONG* FailureInf2064[] = {
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 56) // Itm
};

//CT2���߸澯2065----------
const  LONG* FailureInf2065[] = {
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 60) // Ifm
};

//CT1���߷���2066----------
const  LONG* FailureInf2066[] = {
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 56) // Itm
};

//CT2���߷���2067----------
const  LONG* FailureInf2067[] = {
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 60) // Ifm
};

//һ���غ�բ����2068----------
const  LONG* FailureInf2068[] = {
    {NULL}
};

//�����غ�բ����2249----------
const  LONG* FailureInf2249[] = {
    {NULL}
};

//һ���غ�բ�ɹ�2069----------
const  LONG* FailureInf2069[] = {
    {NULL}
};

//һ���غ�բʧ��2070----------
const  LONG* FailureInf2070[] = {
    {NULL}
};

//�����غ�բ�ɹ�2071----------
const  LONG* FailureInf2071[] = {
    {NULL}
};

//�����غ�բʧ��2072----------
const  LONG* FailureInf2072[] = {
    {NULL}
};

//һ���غ�բδ����2073----------
const  LONG* FailureInf2073[] = {
    {NULL}
};

//�����غ�բδ����2251----------
const  LONG* FailureInf2251[] = {
    {NULL}
};

//����������2100----------
const  LONG* FailureInf2100[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2101----------
const  LONG* FailureInf2101[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2102----------
const  LONG* FailureInf2102[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������2103----------
const  LONG* FailureInf2103[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2104----------
const  LONG* FailureInf2104[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2105----------
const  LONG* FailureInf2105[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������2106----------
const  LONG* FailureInf2106[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2107----------
const  LONG* FailureInf2107[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2108----------
const  LONG* FailureInf2108[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2109----------
const  LONG* FailureInf2109[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2110----------
const  LONG* FailureInf2110[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2111----------
const  LONG* FailureInf2111[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ�������2112----------
const  LONG* FailureInf2112[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ��ٶ���2113----------
const  LONG* FailureInf2113[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����μ��ٷ���2114----------
const  LONG* FailureInf2114[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ�������2115----------
const  LONG* FailureInf2115[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ��ٶ���2116----------
const  LONG* FailureInf2116[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����μ��ٷ���2117----------
const  LONG* FailureInf2117[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2118----------
const  LONG* FailureInf2118[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2119----------
const  LONG* FailureInf2119[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2120----------
const  LONG* FailureInf2120[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ٶ�����2121----------
const  LONG* FailureInf2121[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ٶ϶���2122----------
const  LONG* FailureInf2122[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����ٶϷ���2123----------
const  LONG* FailureInf2123[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2124----------
const  LONG* FailureInf2124[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2125----------
const  LONG* FailureInf2125[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2126----------
const  LONG* FailureInf2126[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2127----------
const  LONG* FailureInf2127[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2128----------
const  LONG* FailureInf2128[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2129----------
const  LONG* FailureInf2129[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2130----------
const  LONG* FailureInf2130[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2131----------
const  LONG* FailureInf2131[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2132----------
const  LONG* FailureInf2132[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2133----------
const  LONG* FailureInf2133[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2134----------
const  LONG* FailureInf2134[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2135----------
const  LONG* FailureInf2135[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2136----------
const  LONG* FailureInf2136[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2137----------
const  LONG* FailureInf2137[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2138----------
const  LONG* FailureInf2138[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2139----------
const  LONG* FailureInf2139[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2140----------
const  LONG* FailureInf2140[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2141----------
const  LONG* FailureInf2141[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������������2142----------
const  LONG* FailureInf2142[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������������2143----------
const  LONG* FailureInf2143[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//����������������2144----------
const  LONG* FailureInf2144[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ʱ�޹�������2145----------
const  LONG* FailureInf2145[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ʱ�޹�������2146----------
const  LONG* FailureInf2146[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//��ʱ�޹�������2147----------
const  LONG* FailureInf2147[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ѹ������բ2148----------
const  LONG* FailureInf2148[] = {
    (LONG*)(RM_BUF_ADDR + 32), // TL
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//����������2200----------
const  LONG* FailureInf2200[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2201----------
const  LONG* FailureInf2201[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2202----------
const  LONG* FailureInf2202[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������2203----------
const  LONG* FailureInf2203[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2204----------
const  LONG* FailureInf2204[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2205----------
const  LONG* FailureInf2205[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������2206----------
const  LONG* FailureInf2206[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ζ���2207----------
const  LONG* FailureInf2207[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����η���2208----------
const  LONG* FailureInf2208[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2209----------
const  LONG* FailureInf2209[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2210----------
const  LONG* FailureInf2210[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2211----------
const  LONG* FailureInf2211[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ�������2212----------
const  LONG* FailureInf2212[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ��ٶ���2213----------
const  LONG* FailureInf2213[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����μ��ٷ���2214----------
const  LONG* FailureInf2214[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ�������2215----------
const  LONG* FailureInf2215[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����μ��ٶ���2216----------
const  LONG* FailureInf2216[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����μ��ٷ���2217----------
const  LONG* FailureInf2217[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2218----------
const  LONG* FailureInf2218[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2219----------
const  LONG* FailureInf2219[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2220----------
const  LONG* FailureInf2220[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ٶ�����2221----------
const  LONG* FailureInf2221[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����ٶ϶���2222----------
const  LONG* FailureInf2222[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//�����ٶϷ���2223----------
const  LONG* FailureInf2223[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2224----------
const  LONG* FailureInf2224[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2225----------
const  LONG* FailureInf2225[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2226----------
const  LONG* FailureInf2226[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2227----------
const  LONG* FailureInf2227[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2228----------
const  LONG* FailureInf2228[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2229----------
const  LONG* FailureInf2229[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//�����������2230----------
const  LONG* FailureInf2230[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������ζ���2231----------
const  LONG* FailureInf2231[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������η���2232----------
const  LONG* FailureInf2232[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2233----------
const  LONG* FailureInf2233[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2234----------
const  LONG* FailureInf2234[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2235----------
const  LONG* FailureInf2235[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2236----------
const  LONG* FailureInf2236[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2237----------
const  LONG* FailureInf2237[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2238----------
const  LONG* FailureInf2238[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ�������2239----------
const  LONG* FailureInf2239[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//������μ��ٶ���2240----------
const  LONG* FailureInf2240[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//������μ��ٷ���2241----------
const  LONG* FailureInf2241[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������������2242----------
const  LONG* FailureInf2242[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//����������������2243----------
const  LONG* FailureInf2243[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//����������������2244----------
const  LONG* FailureInf2244[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ʱ�޹�������2245----------
const  LONG* FailureInf2245[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ʱ�޹�������2246----------
const  LONG* FailureInf2246[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
};

//��ʱ�޹�������2247----------
const  LONG* FailureInf2247[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 72) // ��Z
};

//��ѹ������բ2248----------
const  LONG* FailureInf2248[] = {
    (LONG*)(RM_BUF_ADDR + 12), // Up
    (LONG*)(RM_BUF_ADDR + 0), // Ip
    (LONG*)(RM_BUF_ADDR + 64), // R
    (LONG*)(RM_BUF_ADDR + 68), // X
    (LONG*)(RM_BUF_ADDR + 72), // ��Z
    (LONG*)(RM_BUF_ADDR + 4), // It
    (LONG*)(RM_BUF_ADDR + 8), // If
    (LONG*)(RM_BUF_ADDR + 44), // I2
    (LONG*)(RM_BUF_ADDR + 48), // I3
    (LONG*)(RM_BUF_ADDR + 52) // I5
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
    {0, FailureInf63, 63},//�߼����̹��ϻָ�
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf500, 500},//��������
    {3, FailureInf2000, 2000},//����������
    {18, FailureInf2001, 2001},//�����ζ���
    {3, FailureInf2002, 2002},//�����η���
    {3, FailureInf2003, 2003},//����������
    {18, FailureInf2004, 2004},//�����ζ���
    {3, FailureInf2005, 2005},//�����η���
    {3, FailureInf2006, 2006},//����������
    {18, FailureInf2007, 2007},//�����ζ���
    {3, FailureInf2008, 2008},//�����η���
    {3, FailureInf2009, 2009},//�����������
    {18, FailureInf2010, 2010},//������ζ���
    {3, FailureInf2011, 2011},//������η���
    {3, FailureInf2012, 2012},//�����μ�������
    {18, FailureInf2013, 2013},//�����μ��ٶ���
    {3, FailureInf2014, 2014},//�����μ��ٷ���
    {3, FailureInf2015, 2015},//�����μ�������
    {18, FailureInf2016, 2016},//�����μ��ٶ���
    {3, FailureInf2017, 2017},//�����μ��ٷ���
    {3, FailureInf2018, 2018},//������μ�������
    {18, FailureInf2019, 2019},//������μ��ٶ���
    {3, FailureInf2020, 2020},//������μ��ٷ���
    {3, FailureInf2021, 2021},//�����ٶ�����
    {18, FailureInf2022, 2022},//�����ٶ϶���
    {3, FailureInf2023, 2023},//�����ٶϷ���
    {3, FailureInf2024, 2024},//�����������
    {18, FailureInf2025, 2025},//������ζ���
    {3, FailureInf2026, 2026},//������η���
    {3, FailureInf2027, 2027},//�����������
    {18, FailureInf2028, 2028},//������ζ���
    {3, FailureInf2029, 2029},//������η���
    {3, FailureInf2030, 2030},//�����������
    {18, FailureInf2031, 2031},//������ζ���
    {3, FailureInf2032, 2032},//������η���
    {3, FailureInf2033, 2033},//������μ�������
    {18, FailureInf2034, 2034},//������μ��ٶ���
    {3, FailureInf2035, 2035},//������μ��ٷ���
    {3, FailureInf2036, 2036},//������μ�������
    {18, FailureInf2037, 2037},//������μ��ٶ���
    {3, FailureInf2038, 2038},//������μ��ٷ���
    {3, FailureInf2039, 2039},//������μ�������
    {18, FailureInf2040, 2040},//������μ��ٶ���
    {3, FailureInf2041, 2041},//������μ��ٷ���
    {3, FailureInf2042, 2042},//����������������
    {18, FailureInf2043, 2043},//����������������
    {3, FailureInf2044, 2044},//����������������
    {3, FailureInf2045, 2045},//��ʱ�޹�������
    {18, FailureInf2046, 2046},//��ʱ�޹�������
    {3, FailureInf2047, 2047},//��ʱ�޹�������
    {18, FailureInf2048, 2048},//��ѹ������բ
    {18, FailureInf2049, 2049},//��ѹ�����澯
    {0, FailureInf2050, 2050},//͵���غ�բ
    {0, FailureInf2051, 2051},//һ���غ�բ����
    {0, FailureInf2052, 2052},//�����غ�բ����
    {8, FailureInf2053, 2053},//PT���߱������뱣��
    {2, FailureInf2054, 2054},//һ���غ�բ������
    {2, FailureInf2250, 2250},//�����غ�բ������
    {0, FailureInf2055, 2055},//ʧ�鱣������
    {5, FailureInf2056, 2056},//PT1����
    {5, FailureInf2057, 2057},//PT2����
    {5, FailureInf2058, 2058},//PT1���߷���
    {5, FailureInf2059, 2059},//PT2���߷���
    {0, FailureInf2060, 2060},//�غ�բʧ��
    {10, FailureInf2061, 2061},//��������
    {3, FailureInf2062, 2062},//��·����բʧ��
    {26, FailureInf2063, 2063},//ͨ������
    {2, FailureInf2064, 2064},//CT1���߸澯
    {2, FailureInf2065, 2065},//CT2���߸澯
    {2, FailureInf2066, 2066},//CT1���߷���
    {2, FailureInf2067, 2067},//CT2���߷���
    {0, FailureInf2068, 2068},//һ���غ�բ����
    {0, FailureInf2249, 2249},//�����غ�բ����
    {0, FailureInf2069, 2069},//һ���غ�բ�ɹ�
    {0, FailureInf2070, 2070},//һ���غ�բʧ��
    {0, FailureInf2071, 2071},//�����غ�բ�ɹ�
    {0, FailureInf2072, 2072},//�����غ�բʧ��
    {0, FailureInf2073, 2073},//һ���غ�բδ����
    {0, FailureInf2251, 2251},//�����غ�բδ����
    {3, FailureInf2100, 2100},//����������
    {9, FailureInf2101, 2101},//�����ζ���
    {3, FailureInf2102, 2102},//�����η���
    {3, FailureInf2103, 2103},//����������
    {9, FailureInf2104, 2104},//�����ζ���
    {3, FailureInf2105, 2105},//�����η���
    {3, FailureInf2106, 2106},//����������
    {9, FailureInf2107, 2107},//�����ζ���
    {3, FailureInf2108, 2108},//�����η���
    {3, FailureInf2109, 2109},//�����������
    {9, FailureInf2110, 2110},//������ζ���
    {3, FailureInf2111, 2111},//������η���
    {3, FailureInf2112, 2112},//�����μ�������
    {9, FailureInf2113, 2113},//�����μ��ٶ���
    {3, FailureInf2114, 2114},//�����μ��ٷ���
    {3, FailureInf2115, 2115},//�����μ�������
    {9, FailureInf2116, 2116},//�����μ��ٶ���
    {3, FailureInf2117, 2117},//�����μ��ٷ���
    {3, FailureInf2118, 2118},//������μ�������
    {9, FailureInf2119, 2119},//������μ��ٶ���
    {3, FailureInf2120, 2120},//������μ��ٷ���
    {3, FailureInf2121, 2121},//�����ٶ�����
    {9, FailureInf2122, 2122},//�����ٶ϶���
    {3, FailureInf2123, 2123},//�����ٶϷ���
    {3, FailureInf2124, 2124},//�����������
    {9, FailureInf2125, 2125},//������ζ���
    {3, FailureInf2126, 2126},//������η���
    {3, FailureInf2127, 2127},//�����������
    {9, FailureInf2128, 2128},//������ζ���
    {3, FailureInf2129, 2129},//������η���
    {3, FailureInf2130, 2130},//�����������
    {9, FailureInf2131, 2131},//������ζ���
    {3, FailureInf2132, 2132},//������η���
    {3, FailureInf2133, 2133},//������μ�������
    {9, FailureInf2134, 2134},//������μ��ٶ���
    {3, FailureInf2135, 2135},//������μ��ٷ���
    {3, FailureInf2136, 2136},//������μ�������
    {9, FailureInf2137, 2137},//������μ��ٶ���
    {3, FailureInf2138, 2138},//������μ��ٷ���
    {3, FailureInf2139, 2139},//������μ�������
    {9, FailureInf2140, 2140},//������μ��ٶ���
    {3, FailureInf2141, 2141},//������μ��ٷ���
    {3, FailureInf2142, 2142},//����������������
    {9, FailureInf2143, 2143},//����������������
    {3, FailureInf2144, 2144},//����������������
    {3, FailureInf2145, 2145},//��ʱ�޹�������
    {9, FailureInf2146, 2146},//��ʱ�޹�������
    {3, FailureInf2147, 2147},//��ʱ�޹�������
    {9, FailureInf2148, 2148},//��ѹ������բ
    {3, FailureInf2200, 2200},//����������
    {10, FailureInf2201, 2201},//�����ζ���
    {3, FailureInf2202, 2202},//�����η���
    {3, FailureInf2203, 2203},//����������
    {10, FailureInf2204, 2204},//�����ζ���
    {3, FailureInf2205, 2205},//�����η���
    {3, FailureInf2206, 2206},//����������
    {10, FailureInf2207, 2207},//�����ζ���
    {3, FailureInf2208, 2208},//�����η���
    {3, FailureInf2209, 2209},//�����������
    {10, FailureInf2210, 2210},//������ζ���
    {3, FailureInf2211, 2211},//������η���
    {3, FailureInf2212, 2212},//�����μ�������
    {10, FailureInf2213, 2213},//�����μ��ٶ���
    {3, FailureInf2214, 2214},//�����μ��ٷ���
    {3, FailureInf2215, 2215},//�����μ�������
    {10, FailureInf2216, 2216},//�����μ��ٶ���
    {3, FailureInf2217, 2217},//�����μ��ٷ���
    {3, FailureInf2218, 2218},//������μ�������
    {10, FailureInf2219, 2219},//������μ��ٶ���
    {3, FailureInf2220, 2220},//������μ��ٷ���
    {3, FailureInf2221, 2221},//�����ٶ�����
    {10, FailureInf2222, 2222},//�����ٶ϶���
    {3, FailureInf2223, 2223},//�����ٶϷ���
    {3, FailureInf2224, 2224},//�����������
    {10, FailureInf2225, 2225},//������ζ���
    {3, FailureInf2226, 2226},//������η���
    {3, FailureInf2227, 2227},//�����������
    {10, FailureInf2228, 2228},//������ζ���
    {3, FailureInf2229, 2229},//������η���
    {3, FailureInf2230, 2230},//�����������
    {10, FailureInf2231, 2231},//������ζ���
    {3, FailureInf2232, 2232},//������η���
    {3, FailureInf2233, 2233},//������μ�������
    {10, FailureInf2234, 2234},//������μ��ٶ���
    {3, FailureInf2235, 2235},//������μ��ٷ���
    {3, FailureInf2236, 2236},//������μ�������
    {10, FailureInf2237, 2237},//������μ��ٶ���
    {3, FailureInf2238, 2238},//������μ��ٷ���
    {3, FailureInf2239, 2239},//������μ�������
    {10, FailureInf2240, 2240},//������μ��ٶ���
    {3, FailureInf2241, 2241},//������μ��ٷ���
    {3, FailureInf2242, 2242},//����������������
    {10, FailureInf2243, 2243},//����������������
    {3, FailureInf2244, 2244},//����������������
    {3, FailureInf2245, 2245},//��ʱ�޹�������
    {10, FailureInf2246, 2246},//��ʱ�޹�������
    {3, FailureInf2247, 2247},//��ʱ�޹�������
    {10, FailureInf2248, 2248},//��ѹ������բ
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 176) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 192) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 220) ,
	 (LONG*)(AS_BUF_ADDR + 224) ,
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
	 (BYTE*)(SW_BUF_ADDR + 5) ,
	 (BYTE*)(SW_BUF_ADDR + 6) ,
	 (BYTE*)(SW_BUF_ADDR + 7) ,
	 (BYTE*)(SW_BUF_ADDR + 8) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 43) ,
	 (BYTE*)(CFG_BUF_ADDR + 44) ,
	 (BYTE*)(CFG_BUF_ADDR + 45) ,
	 (BYTE*)(CFG_BUF_ADDR + 46) ,
	 (BYTE*)(CFG_BUF_ADDR + 47) ,
	 (BYTE*)(CFG_BUF_ADDR + 30) ,
	 (BYTE*)(CFG_BUF_ADDR + 34) ,
	 (BYTE*)(CFG_BUF_ADDR + 31) ,
	 (BYTE*)(CFG_BUF_ADDR + 32) ,
	 (BYTE*)(CFG_BUF_ADDR + 33) ,
	 (BYTE*)(CFG_BUF_ADDR + 35) ,
	 (BYTE*)(CFG_BUF_ADDR + 37) ,
	 (BYTE*)(CFG_BUF_ADDR + 38) ,
	 (BYTE*)(CFG_BUF_ADDR + 39) ,
	 (BYTE*)(CFG_BUF_ADDR + 40) ,
	 (BYTE*)(CFG_BUF_ADDR + 41) ,
	 (BYTE*)(CFG_BUF_ADDR + 42) ,
	 (BYTE*)(CFG_BUF_ADDR + 36) ,
	 (BYTE*)(CFG_BUF_ADDR + 48) ,
	 (BYTE*)(CFG_BUF_ADDR + 49) ,
	 (BYTE*)(CFG_BUF_ADDR + 50) ,
	 (BYTE*)(CFG_BUF_ADDR + 51) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	3050, //IP
	11500, //UP
	11500, //U3
	11500, //U4
	64480, //ITM
	64480, //IFM
	16100, //U1M
	16100, //U2M
	16100, //U3M
	16100, //U4M
	129900, //P
	129900, //Q
	18, //A
	752, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	14, //ITDRIFT
	14, //IFDRIFT
	30, //U1DRIFT
	30, //U2DRIFT
	30, //U3DRIFT
	30, //U4DRIFT
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	15250, //IP
	11500, //UP
	11500, //U3
	11500, //U4
	322400, //ITM
	322400, //IFM
	16100, //U1M
	16100, //U2M
	16100, //U3M
	16100, //U4M
	658980, //P
	658980, //Q
	18, //A
	150, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	14, //ITDRIFT
	14, //IFDRIFT
	30, //U1DRIFT
	30, //U2DRIFT
	30, //U3DRIFT
	30, //U4DRIFT
};

//����----------------------------------
