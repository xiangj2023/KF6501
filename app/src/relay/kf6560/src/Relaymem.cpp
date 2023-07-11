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
};
//������ֵȡֵ��Ϣ
const TSetValueInf SysSetValueInf[]=
{
	{RATE_1A, "1A"},
	{RATE_5A, "5A"},
	{BTMODE, "ֱ������"},
	{ATMODE, "AT����"},
	{-1, NULL},
};
const TSetValueInfMap SysSetValueInfMap[]=
{
	{2, (TSetValueInf*)&(SysSetValueInf[0])}, //������ֵ�����
	{2, (TSetValueInf*)&(SysSetValueInf[2])}, //������ֵ���緽ʽ
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
LONG* 	RM_pnUp 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// U
LONG* 	RM_pnU1 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// U1
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// U2
LONG* 	RM_pnUd1 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// Ud1
LONG* 	RM_pnUd2 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// Ud2
LONG* 	RM_pnUd3 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// Ud3
LONG* 	RM_pnIp 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// I
LONG* 	RM_pnIm 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// Im
LONG* 	RM_pnI1 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// I1
LONG* 	RM_pnI2 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// I2
LONG* 	RM_pnI3 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// I3
LONG* 	RM_pnIh3 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Ih3
LONG* 	RM_pnIh5 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Ih5
LONG* 	RM_pnIh7 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// Ih7
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUM 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//U U
LONG* 	M_pnU1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//U1 U1
LONG* 	M_pnU2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//U2 U2
LONG* 	M_pnUd1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//Ud1 Ud1
LONG* 	M_pnUd2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Ud2 Ud2
LONG* 	M_pnUd3M 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ud3 Ud3
LONG* 	M_pnIM 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//I I
LONG* 	M_pnI1M 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//I1 I1
LONG* 	M_pnI2M 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//I2 I2
LONG* 	M_pnI3M 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//I3 I3
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//COS COS
LONG* 	M_pnIH1 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//IH1 IH1
LONG* 	M_pnIH2 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//IH2 IH2
LONG* 	M_pnIH3 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//IH3 IH3
LONG* 	M_pnIH5 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//IH5 IH5
LONG* 	M_pnIH7 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//IH7 IH7
LONG* 	M_pnIH9 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//IH9 IH9
LONG* 	M_pnIH11 	= 	(LONG*)(MEASURE_BUF_ADDR + 76); 	//IH11 IH11


//��ѹ��----------------------------------
BYTE* SW_pby50_0 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//�����ٶ�
BYTE* SW_pby50_1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//��������
BYTE* SW_pby59 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//��ѹ����
BYTE* SW_pby27 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//��ѹ����
BYTE* SW_pby50_H 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//г����������
BYTE* SW_pby50ST 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//֧·��������
BYTE* SW_pby60I 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//��������
BYTE* SW_pby60U 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//��ѹ����
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby50_0 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//�ٶϱ���
BYTE* CFG_pby50_1 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//��������
BYTE* CFG_pby50_H 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//г����������
BYTE* CFG_pby59 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//��ѹ����
BYTE* CFG_pby27 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//��ѹ����
BYTE* CFG_pby60I1 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//֧·1����
BYTE* CFG_pby60I2 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//֧·2����
BYTE* CFG_pby50ST 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//֧·����
BYTE* CFG_pby60U1 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//֧·1��ѹ
BYTE* CFG_pby60U2 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//֧·2��ѹ
BYTE* CFG_pby60U3 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//֧·3��ѹ
BYTE* CFG_pby74PT 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//PT���߼��
BYTE* CFG_pbyCTBR 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//CT���߼��
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//ʧ�鱣��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnF 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ F
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//ͻ������������ PIC
LONG* RS_pn50_0C 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//�ٶϵ�����ֵ 50_0C
LONG* RS_pn50_0D 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//�ٶ�ʱ�䶨ֵ 50_0D
LONG* RS_pn50_1C 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//����������ֵ 50_1C
LONG* RS_pn50_1D 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//����ʱ�䶨ֵ 50_1D
LONG* RS_pn50_HC 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//г������������ֵ 50_HC
LONG* RS_pn50_HD 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//г������ʱ�䶨ֵ 50_HD
LONG* RS_pn59V 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//��ѹ������ѹ 59V
LONG* RS_pn59D 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//��ѹ����ʱ�� 59D
LONG* RS_pn27V 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//��ѹ������ѹ 27V
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//��ѹ����ʱ�� 27D
LONG* RS_pn27_LK50 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//��ѹ������������ 27_LK50
LONG* RS_pn60I1C 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//֧·1������������ 60I1C
LONG* RS_pn60I1D 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//֧·1��������ʱ�� 60I1D
LONG* RS_pn60I2C 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//֧·2������������ 60I2C
LONG* RS_pn60I2D 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//֧·2��������ʱ�� 60I2D
LONG* RS_pn50STC 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//֧·������������ 50STC
LONG* RS_pn50STD 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//֧·��������ʱ�� 50STD
LONG* RS_pn60U1V 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//֧·1��ѹ������ѹ 60U1V
LONG* RS_pn60U1D 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//֧·1��ѹ����ʱ�� 60U1D
LONG* RS_pn60U2V 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//֧·2��ѹ������ѹ 60U2V
LONG* RS_pn60U2D 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//֧·2��ѹ����ʱ�� 60U2D
LONG* RS_pn60U3V 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//֧·3��ѹ������ѹ 60U3V
LONG* RS_pn60U3D 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//֧·3��ѹ����ʱ�� 60U3D
LONG* RS_pn74PTC 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//PT���߼����� 74PTC
LONG* RS_pn74PTV 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//PT���߼���ѹ 74PTV
LONG* RS_pn74PTD 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//PT���߼��ʱ�� 74PTD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//ʧ�鱣��ʱ�� 52BFD

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
LONG* AS_pnTSW 	= 	(LONG*)(AS_BUF_ADDR + 140); 	//����������ʱ
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 144); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 148); 	//TR2����ʱ��
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//TR3����ʱ��
LONG* AS_pn50IP 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//IP������ֵ
LONG* AS_pn37IP 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//IP������ֵ
LONG* AS_pn50I1 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//I1������ֵ
LONG* AS_pn37I1 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//I1������ֵ
LONG* AS_pn50I2 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//I2������ֵ
LONG* AS_pn37I2 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//I2������ֵ
LONG* AS_pn50I3 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//I3������ֵ
LONG* AS_pn37I3 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//I3������ֵ
LONG* AS_pn59UP 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//Up��ѹ��ֵ
LONG* AS_pn27UP 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//Up��ѹ��ֵ
LONG* AS_pn59U1 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U1��ѹ��ֵ
LONG* AS_pn27U1 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U1��ѹ��ֵ
LONG* AS_pn59U2 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U2��ѹ��ֵ
LONG* AS_pn27U2 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//U2��ѹ��ֵ
LONG* AS_pn59Ud1 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//Ud1��ѹ��ֵ
LONG* AS_pn27Ud1 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//Ud1��ѹ��ֵ
LONG* AS_pn59Ud2 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//Ud2��ѹ��ֵ
LONG* AS_pn27Ud2 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//Ud2��ѹ��ֵ
LONG* AS_pn59Ud3 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//Ud3��ѹ��ֵ
LONG* AS_pn27Ud3 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//Ud3��ѹ��ֵ
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnU1PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 240); 	//U1PT���
LONG* AS_pnU2PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 244); 	//U2PT���
LONG* AS_pnUd1PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 248); 	//Ud1PT���
LONG* AS_pnUd2PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 252); 	//Ud2PT���
LONG* AS_pnUd3PTRatio 	= 	(LONG*)(AS_BUF_ADDR + 256); 	//Ud3PT���
LONG* AS_pnIPCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 260); 	//����CT���
LONG* AS_pnIMCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 264); 	//����CT���
LONG* AS_pnId1CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 268); 	//Id1CT���
LONG* AS_pnId2CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 272); 	//Id2CT���
LONG* AS_pnId3CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 276); 	//Id3CT���
LONG* AS_pnrt1 	= 	(LONG*)(AS_BUF_ADDR + 280); 	//rt1

//ϵ��----------------------------------
LONG* CF_pnUP 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UP
LONG* CF_pnU1P 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U1P
LONG* CF_pnU2P 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U2P
LONG* CF_pnUd1P 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//Ud1P
LONG* CF_pnUd2P 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//Ud2P
LONG* CF_pnUd3P 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//Ud3P
LONG* CF_pnIP 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//IP
LONG* CF_pnI1P 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//I1P
LONG* CF_pnI2P 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//I2P
LONG* CF_pnI3P 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//I3P
LONG* CF_pnUM 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//UM
LONG* CF_pnU1M 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//U1M
LONG* CF_pnU2M 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//U2M
LONG* CF_pnUd1M 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//Ud1M
LONG* CF_pnUd2M 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//Ud2M
LONG* CF_pnUd3M 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//Ud3M
LONG* CF_pnIM 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IM
LONG* CF_pnI1M 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//I1M
LONG* CF_pnI2M 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//I2M
LONG* CF_pnI3M 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//I3M
LONG* CF_pnP 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//P
LONG* CF_pnQ 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//Q
LONG* CF_pnA 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//A
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//C4
LONG* CF_pnDRIFTU 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//DRIFTU
LONG* CF_pnDRIFTU1 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//DRIFTU1
LONG* CF_pnDRIFTU2 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//DRIFTU2
LONG* CF_pnDRIFTUD1 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//DRIFTUD1
LONG* CF_pnDRIFTUD2 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//DRIFTUD2
LONG* CF_pnDRIFTUD3 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//DRIFTUD3
LONG* CF_pnDRIFTIM 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//DRIFTIM
LONG* CF_pnDRIFTI1 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//DRIFTI1
LONG* CF_pnDRIFTI2 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//DRIFTI2
LONG* CF_pnDRIFTI3 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//DRIFTI3

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 64), // IN1
    (LONG*)(RM_BUF_ADDR + 68) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 56), // BR1
    (LONG*)(RM_BUF_ADDR + 60) // BR2
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

//�ⲿRAM����15----------
const  LONG* FailureInf15[] = {
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

//�߼����̹��Ϲ��ϻָ�63----------
const  LONG* FailureInf63[] = {
    {NULL}
};

//��������500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//��ѹ��������3000----------
const  LONG* FailureInf3000[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//��ѹ��������3001----------
const  LONG* FailureInf3001[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//��ѹ��������3002----------
const  LONG* FailureInf3002[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//г��������������3003----------
const  LONG* FailureInf3003[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//г��������������3004----------
const  LONG* FailureInf3004[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//г��������������3005----------
const  LONG* FailureInf3005[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·1������������3006----------
const  LONG* FailureInf3006[] = {
    (LONG*)(RM_BUF_ADDR + 32) // I1
};

//֧·1������������3007----------
const  LONG* FailureInf3007[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·1������������3008----------
const  LONG* FailureInf3008[] = {
    (LONG*)(RM_BUF_ADDR + 32) // I1
};

//֧·2������������3009----------
const  LONG* FailureInf3009[] = {
    (LONG*)(RM_BUF_ADDR + 36) // I2
};

//֧·2������������3010----------
const  LONG* FailureInf3010[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·2������������3011----------
const  LONG* FailureInf3011[] = {
    (LONG*)(RM_BUF_ADDR + 36) // I2
};

//֧·������������3012----------
const  LONG* FailureInf3012[] = {
    (LONG*)(RM_BUF_ADDR + 40) // I3
};

//֧·������������3013----------
const  LONG* FailureInf3013[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·������������3014----------
const  LONG* FailureInf3014[] = {
    (LONG*)(RM_BUF_ADDR + 40) // I3
};

//֧·1��ѹ��������3015----------
const  LONG* FailureInf3015[] = {
    (LONG*)(RM_BUF_ADDR + 12) // Ud1
};

//֧·1��ѹ��������3016----------
const  LONG* FailureInf3016[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·1��ѹ��������3017----------
const  LONG* FailureInf3017[] = {
    (LONG*)(RM_BUF_ADDR + 12) // Ud1
};

//֧·2��ѹ��������3018----------
const  LONG* FailureInf3018[] = {
    (LONG*)(RM_BUF_ADDR + 16) // Ud2
};

//֧·2��ѹ��������3019----------
const  LONG* FailureInf3019[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·2��ѹ��������3020----------
const  LONG* FailureInf3020[] = {
    (LONG*)(RM_BUF_ADDR + 16) // Ud2
};

//֧·3��ѹ��������3021----------
const  LONG* FailureInf3021[] = {
    (LONG*)(RM_BUF_ADDR + 20) // Ud3
};

//֧·3��ѹ��������3022----------
const  LONG* FailureInf3022[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//֧·3��ѹ��������3023----------
const  LONG* FailureInf3023[] = {
    (LONG*)(RM_BUF_ADDR + 20) // Ud3
};

//�����ٶ�����3024----------
const  LONG* FailureInf3024[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//�����ٶ϶���3025----------
const  LONG* FailureInf3025[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//�����ٶϷ���3026----------
const  LONG* FailureInf3026[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//������������3027----------
const  LONG* FailureInf3027[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//������������3028----------
const  LONG* FailureInf3028[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//������������3029----------
const  LONG* FailureInf3029[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//��ѹ��������3030----------
const  LONG* FailureInf3030[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//PT����3031----------
const  LONG* FailureInf3031[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//PT���߻ָ�3032----------
const  LONG* FailureInf3032[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//CT����3033----------
const  LONG* FailureInf3033[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 28) // Im
};

//CT���߻ָ�3034----------
const  LONG* FailureInf3034[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 28) // Im
};

//��·����բʧ��3035----------
const  LONG* FailureInf3035[] = {
    (LONG*)(RM_BUF_ADDR + 24) // I
};

//ͨ������3036----------
const  LONG* FailureInf3036[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
};

//��������3037----------
const  LONG* FailureInf3037[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U
    (LONG*)(RM_BUF_ADDR + 4), // U1
    (LONG*)(RM_BUF_ADDR + 8), // U2
    (LONG*)(RM_BUF_ADDR + 12), // Ud1
    (LONG*)(RM_BUF_ADDR + 16), // Ud2
    (LONG*)(RM_BUF_ADDR + 20), // Ud3
    (LONG*)(RM_BUF_ADDR + 24), // I
    (LONG*)(RM_BUF_ADDR + 32), // I1
    (LONG*)(RM_BUF_ADDR + 36), // I2
    (LONG*)(RM_BUF_ADDR + 40), // I3
    (LONG*)(RM_BUF_ADDR + 44), // Ih3
    (LONG*)(RM_BUF_ADDR + 48), // Ih5
    (LONG*)(RM_BUF_ADDR + 52) // Ih7
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

//���������쳣22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//���������쳣�ָ�65----------
const  LONG* FailureInf65[] = {
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
    {0, FailureInf15, 15},//�ⲿRAM����
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
    {0, FailureInf63, 63},//�߼����̹��Ϲ��ϻָ�
    {0, FailureInf500, 500},//��������
    {2, FailureInf3000, 3000},//��ѹ��������
    {13, FailureInf3001, 3001},//��ѹ��������
    {2, FailureInf3002, 3002},//��ѹ��������
    {3, FailureInf3003, 3003},//г��������������
    {13, FailureInf3004, 3004},//г��������������
    {3, FailureInf3005, 3005},//г��������������
    {1, FailureInf3006, 3006},//֧·1������������
    {13, FailureInf3007, 3007},//֧·1������������
    {1, FailureInf3008, 3008},//֧·1������������
    {1, FailureInf3009, 3009},//֧·2������������
    {13, FailureInf3010, 3010},//֧·2������������
    {1, FailureInf3011, 3011},//֧·2������������
    {1, FailureInf3012, 3012},//֧·������������
    {13, FailureInf3013, 3013},//֧·������������
    {1, FailureInf3014, 3014},//֧·������������
    {1, FailureInf3015, 3015},//֧·1��ѹ��������
    {13, FailureInf3016, 3016},//֧·1��ѹ��������
    {1, FailureInf3017, 3017},//֧·1��ѹ��������
    {1, FailureInf3018, 3018},//֧·2��ѹ��������
    {13, FailureInf3019, 3019},//֧·2��ѹ��������
    {1, FailureInf3020, 3020},//֧·2��ѹ��������
    {1, FailureInf3021, 3021},//֧·3��ѹ��������
    {13, FailureInf3022, 3022},//֧·3��ѹ��������
    {1, FailureInf3023, 3023},//֧·3��ѹ��������
    {1, FailureInf3024, 3024},//�����ٶ�����
    {13, FailureInf3025, 3025},//�����ٶ϶���
    {1, FailureInf3026, 3026},//�����ٶϷ���
    {1, FailureInf3027, 3027},//������������
    {13, FailureInf3028, 3028},//������������
    {1, FailureInf3029, 3029},//������������
    {13, FailureInf3030, 3030},//��ѹ��������
    {4, FailureInf3031, 3031},//PT����
    {4, FailureInf3032, 3032},//PT���߻ָ�
    {3, FailureInf3033, 3033},//CT����
    {3, FailureInf3034, 3034},//CT���߻ָ�
    {1, FailureInf3035, 3035},//��·����բʧ��
    {13, FailureInf3036, 3036},//ͨ������
    {13, FailureInf3037, 3037},//��������
    {0, FailureInf20, 20},//5V��Դ����
    {0, FailureInf21, 21},//EEPROMд�����
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
    {0, FailureInf501, 501},//����˹
    {0, FailureInf502, 502},//ѹ���ͷ�
    {0, FailureInf503, 503},//������բ
    {0, FailureInf504, 504},//����˹
    {0, FailureInf505, 505},//��λ�쳣
    {0, FailureInf506, 506},//�¶ȹ���
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 112) },
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
	 (BYTE*)(SW_BUF_ADDR + 0) ,
	 (BYTE*)(SW_BUF_ADDR + 8) ,
	 (BYTE*)(SW_BUF_ADDR + 9) ,
	 (BYTE*)(SW_BUF_ADDR + 10) ,
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
	 (WORD)(0) ,
	 (WORD)(8) ,
	 (WORD)(9) ,
	 (WORD)(10) ,
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
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	11400, //UP
	11400, //U1P
	11400, //U2P
	30400, //Ud1P
	30400, //Ud2P
	30400, //Ud3P
	3050, //IP
	3060, //I1P
	3040, //I2P
	3040, //I3P
	16100, //UM
	16100, //U1M
	16100, //U2M
	42900, //Ud1M
	42900, //Ud2M
	42900, //Ud3M
	64500, //IM
	4310, //I1M
	4290, //I2M
	4300, //I3M
	346400, //P
	346400, //Q
	116, //A
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	1, //DRIFTU
	1, //DRIFTU1
	1, //DRIFTU2
	1, //DRIFTUD1
	1, //DRIFTUD2
	1, //DRIFTUD3
	1, //DRIFTIM
	1, //DRIFTI1
	1, //DRIFTI2
	1, //DRIFTI3
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	11400, //UP
	11400, //U1P
	11400, //U2P
	30400, //Ud1P
	30400, //Ud2P
	30400, //Ud3P
	15250, //IP
	15250, //I1P
	15240, //I2P
	15250, //I3P
	16100, //UM
	16100, //U1M
	16100, //U2M
	42900, //Ud1M
	42900, //Ud2M
	42900, //Ud3M
	322500, //IM
	21500, //I1M
	21500, //I2M
	21500, //I3M
	346400, //P
	346400, //Q
	116, //A
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	1, //DRIFTU
	1, //DRIFTU1
	1, //DRIFTU2
	1, //DRIFTUD1
	1, //DRIFTUD2
	1, //DRIFTUD3
	1, //DRIFTIM
	1, //DRIFTI1
	1, //DRIFTI2
	1, //DRIFTI3
};

//����----------------------------------
