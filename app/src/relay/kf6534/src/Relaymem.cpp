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
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
};

//��������ֵ����-----------------------
LONG* 	RM_pnUp1 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// Up1
LONG* 	RM_pnUp2 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// Up2
LONG* 	RM_pnUp3 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// Up3
LONG* 	RM_pnUp4 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// Up4
LONG* 	RM_pnUp5 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// Up5
LONG* 	RM_pnUp6 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// Up6
LONG* 	RM_pnUp7 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// Up7
LONG* 	RM_pnUp8 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// Up8
LONG* 	RM_pnIp1 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// Ip1
LONG* 	RM_pnIp2 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// Ip2
LONG* 	RM_pnIp3 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// Ip3
LONG* 	RM_pnIp4 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Ip4
LONG* 	RM_pnIp5 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// Ip5
LONG* 	RM_pnIp6 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// Ip6
LONG* 	RM_pnIp7 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// Ip7
LONG* 	RM_pnIp8 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// Ip8
LONG* 	RM_pnIC1 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// IC1
LONG* 	RM_pnIC2 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// IC2
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUm1 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//Um1 Um1
LONG* 	M_pnUm2 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//Um2 Um2
LONG* 	M_pnUm3 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//Um3 Um3
LONG* 	M_pnUm4 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//Um4 Um4
LONG* 	M_pnUm5 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Um5 Um5
LONG* 	M_pnUm6 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Um6 Um6
LONG* 	M_pnUm7 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Um7 Um7
LONG* 	M_pnUm8 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//Um8 Um8
LONG* 	M_pnIm1 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//Im1 Im1
LONG* 	M_pnIm2 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//Im2 Im2
LONG* 	M_pnIm3 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//Im3 Im3
LONG* 	M_pnIm4 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Im4 Im4
LONG* 	M_pnIm5 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Im5 Im5
LONG* 	M_pnIm6 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//Im6 Im6
LONG* 	M_pnIm7 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//Im7 Im7
LONG* 	M_pnIm8 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//Im8 Im8


//��ѹ��----------------------------------
BYTE* SW_pbyBZT 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//����Ͷ����
BYTE* SW_pbyACAT1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//1AT�Զ�����
BYTE* SW_pbyACAT2 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//2AT�Զ�����
BYTE* SW_pby60I_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//1AT��������
BYTE* SW_pby50T_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//1ATT�߹���
BYTE* SW_pby50F_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 5); 	//1ATF�߹���
BYTE* SW_pby50CS_AT1 	= 	(BYTE*)(SW_BUF_ADDR + 6); 	//1AT���ǹ���
BYTE* SW_pby60I_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 7); 	//2AT��������
BYTE* SW_pby50T_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 8); 	//2ATT�߹���
BYTE* SW_pby50F_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 9); 	//2ATF�߹���
BYTE* SW_pby50CS_AT2 	= 	(BYTE*)(SW_BUF_ADDR + 10); 	//2AT���ǹ���
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 11); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 12); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 13); 	//����ѹ��2
BYTE* SW_pbyBAK3 	= 	(BYTE*)(SW_BUF_ADDR + 14); 	//����ѹ��3
BYTE* SW_pbyBAK4 	= 	(BYTE*)(SW_BUF_ADDR + 15); 	//����ѹ��4

//����----------------------------------
BYTE* CFG_pbyBZT 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//����Ͷ����
BYTE* CFG_pbyREBZT 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//����Ͷ����Ͷ��
BYTE* CFG_pbyACAT1 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//1AT�Զ�����
BYTE* CFG_pbyACAT2 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//2AT�Զ�����
BYTE* CFG_pby60I_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//1AT��������
BYTE* CFG_pby50T_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//1ATT�߹���
BYTE* CFG_pby50F_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//1ATF�߹���
BYTE* CFG_pby50CS_AT1 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//1AT���ǹ���
BYTE* CFG_pby60I_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//2AT��������
BYTE* CFG_pby50T_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//2ATT�߹���
BYTE* CFG_pby50F_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//2ATF�߹���
BYTE* CFG_pby50CS_AT2 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//2AT���ǹ���
BYTE* CFG_pby52BF 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//ʧ�鱣��
BYTE* CFG_pby74PT1 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//PT1���߼��
BYTE* CFG_pby74PT2 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//PT2���߼��
BYTE* CFG_pby74PT3 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//PT3���߼��
BYTE* CFG_pby74PT4 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//PT4���߼��
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//��������2
BYTE* CFG_pbyBAK3 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//��������3
BYTE* CFG_pbyBAK4 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//��������4

//��ֵ----------------------------------
LONG* RS_pnF 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ F
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//ͻ������������ PIC
LONG* RS_pn60I_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//1AT������ֵ 60I_AT1C
LONG* RS_pn60I_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//1AT��������ʱ�� 60I_AT1T
LONG* RS_pn50T_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//1ATT�߹�����ֵ 50T_AT1C
LONG* RS_pn50T_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//1ATT�߹�������ʱ�� 50T_AT1T
LONG* RS_pn50F_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//1ATF�߹�����ֵ 50F_AT1C
LONG* RS_pn50F_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//1ATF�߹�������ʱ�� 50F_AT1T
LONG* RS_pn50CS_AT1C 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//1AT���ǹ�����ֵ 50CS_AT1C
LONG* RS_pn50CS_AT1T 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//1AT���ǹ�������ʱ�� 50CS_AT1T
LONG* RS_pn60I_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//2AT������ֵ 60I_AT2C
LONG* RS_pn60I_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//2AT��������ʱ�� 60I_AT2T
LONG* RS_pn50T_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//2ATT�߹�����ֵ 50T_AT2C
LONG* RS_pn50T_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//2ATT�߹�������ʱ�� 50T_AT2T
LONG* RS_pn50F_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//2ATF�߹�����ֵ 50F_AT2C
LONG* RS_pn50F_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//2ATF�߹�������ʱ�� 50F_AT2T
LONG* RS_pn50CS_AT2C 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//2AT���ǹ�����ֵ 50CS_AT2C
LONG* RS_pn50CS_AT2T 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//2AT���ǹ�������ʱ�� 50CS_AT2T
LONG* RS_pn59U1 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//U1��ѹ 59U1
LONG* RS_pn27U1 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//U1��ѹ 27U1
LONG* RS_pn59U2 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//U2��ѹ 59U2
LONG* RS_pn27U2 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//U2��ѹ 27U2
LONG* RS_pn59U3 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//U3��ѹ 59U3
LONG* RS_pn27U3 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//U3��ѹ 27U3
LONG* RS_pn59U4 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//U4��ѹ 59U4
LONG* RS_pn27U4 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//U4��ѹ 27U4
LONG* RS_pn59U5 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//U5��ѹ 59U5
LONG* RS_pn27U5 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//U5��ѹ 27U5
LONG* RS_pn59U6 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//U6��ѹ 59U6
LONG* RS_pn27U6 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//U6��ѹ 27U6
LONG* RS_pn59U7 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//U7��ѹ 59U7
LONG* RS_pn27U7 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//U7��ѹ 27U7
LONG* RS_pn59U8 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//U8��ѹ 59U8
LONG* RS_pn27U8 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//U8��ѹ 27U8
LONG* RS_pn50I1 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//I1���� 50I1
LONG* RS_pn37I1 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//I1���� 37I1
LONG* RS_pn50I2 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//I2���� 50I2
LONG* RS_pn37I2 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//I2���� 37I2
LONG* RS_pn50I3 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//I3���� 50I3
LONG* RS_pn37I3 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//I3���� 37I3
LONG* RS_pn50I4 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//I4���� 50I4
LONG* RS_pn37I4 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//I4���� 37I4
LONG* RS_pn50I5 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//I5���� 50I5
LONG* RS_pn37I5 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//I5���� 37I5
LONG* RS_pn50I6 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//I6���� 50I6
LONG* RS_pn37I6 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//I6���� 37I6
LONG* RS_pn50I7 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//I7���� 50I7
LONG* RS_pn37I7 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//I7���� 37I7
LONG* RS_pn50I8 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//I8���� 50I8
LONG* RS_pn37I8 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//I8���� 37I8
LONG* RS_pnT1PI 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//T1������ʱ T1PI
LONG* RS_pnT1RE 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//T1������ʱ T1RE
LONG* RS_pnT2PI 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//T2������ʱ T2PI
LONG* RS_pnT2RE 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//T2������ʱ T2RE
LONG* RS_pnT3PI 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//T3������ʱ T3PI
LONG* RS_pnT3RE 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//T3������ʱ T3RE
LONG* RS_pnT4PI 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//T4������ʱ T4PI
LONG* RS_pnT4RE 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//T4������ʱ T4RE
LONG* RS_pnT5PI 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//T5������ʱ T5PI
LONG* RS_pnT5RE 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//T5������ʱ T5RE
LONG* RS_pnT6PI 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//T6������ʱ T6PI
LONG* RS_pnT6RE 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//T6������ʱ T6RE
LONG* RS_pnT7PI 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//T7������ʱ T7PI
LONG* RS_pnT7RE 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//T7������ʱ T7RE
LONG* RS_pnT8PI 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//T8������ʱ T8PI
LONG* RS_pnT8RE 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//T8������ʱ T8RE
LONG* RS_pnT9PI 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//T9������ʱ T9PI
LONG* RS_pnT9RE 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//T9������ʱ T9RE
LONG* RS_pnT10PI 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//T10������ʱ T10PI
LONG* RS_pnT10RE 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//T10������ʱ T10RE
LONG* RS_pnT11PI 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//T11������ʱ T11PI
LONG* RS_pnT11RE 	= 	(LONG*)(RS_BUF_ADDR + 284); 	//T11������ʱ T11RE
LONG* RS_pnT12PI 	= 	(LONG*)(RS_BUF_ADDR + 288); 	//T12������ʱ T12PI
LONG* RS_pnT12RE 	= 	(LONG*)(RS_BUF_ADDR + 292); 	//T12������ʱ T12RE
LONG* RS_pnT13PI 	= 	(LONG*)(RS_BUF_ADDR + 296); 	//T13������ʱ T13PI
LONG* RS_pnT13RE 	= 	(LONG*)(RS_BUF_ADDR + 300); 	//T13������ʱ T13RE
LONG* RS_pnT14PI 	= 	(LONG*)(RS_BUF_ADDR + 304); 	//T14������ʱ T14PI
LONG* RS_pnT14RE 	= 	(LONG*)(RS_BUF_ADDR + 308); 	//T14������ʱ T14RE
LONG* RS_pnT15PI 	= 	(LONG*)(RS_BUF_ADDR + 312); 	//T15������ʱ T15PI
LONG* RS_pnT15RE 	= 	(LONG*)(RS_BUF_ADDR + 316); 	//T15������ʱ T15RE
LONG* RS_pnT16PI 	= 	(LONG*)(RS_BUF_ADDR + 320); 	//T16������ʱ T16PI
LONG* RS_pnT16RE 	= 	(LONG*)(RS_BUF_ADDR + 324); 	//T16������ʱ T16RE
LONG* RS_pnT17PI 	= 	(LONG*)(RS_BUF_ADDR + 328); 	//T17������ʱ T17PI
LONG* RS_pnT17RE 	= 	(LONG*)(RS_BUF_ADDR + 332); 	//T17������ʱ T17RE
LONG* RS_pnT18PI 	= 	(LONG*)(RS_BUF_ADDR + 336); 	//T18������ʱ T18PI
LONG* RS_pnT18RE 	= 	(LONG*)(RS_BUF_ADDR + 340); 	//T18������ʱ T18RE
LONG* RS_pnT19PI 	= 	(LONG*)(RS_BUF_ADDR + 344); 	//T19������ʱ T19PI
LONG* RS_pnT19RE 	= 	(LONG*)(RS_BUF_ADDR + 348); 	//T19������ʱ T19RE
LONG* RS_pnT20PI 	= 	(LONG*)(RS_BUF_ADDR + 352); 	//T20������ʱ T20PI
LONG* RS_pnT20RE 	= 	(LONG*)(RS_BUF_ADDR + 356); 	//T20������ʱ T20RE
LONG* RS_pnT21PI 	= 	(LONG*)(RS_BUF_ADDR + 360); 	//T21������ʱ T21PI
LONG* RS_pnT21RE 	= 	(LONG*)(RS_BUF_ADDR + 364); 	//T21������ʱ T21RE
LONG* RS_pnT22PI 	= 	(LONG*)(RS_BUF_ADDR + 368); 	//T22������ʱ T22PI
LONG* RS_pnT22RE 	= 	(LONG*)(RS_BUF_ADDR + 372); 	//T22������ʱ T22RE
LONG* RS_pnT23PI 	= 	(LONG*)(RS_BUF_ADDR + 376); 	//T23������ʱ T23PI
LONG* RS_pnT23RE 	= 	(LONG*)(RS_BUF_ADDR + 380); 	//T23������ʱ T23RE
LONG* RS_pnT24PI 	= 	(LONG*)(RS_BUF_ADDR + 384); 	//T24������ʱ T24PI
LONG* RS_pnT24RE 	= 	(LONG*)(RS_BUF_ADDR + 388); 	//T24������ʱ T24RE
LONG* RS_pnT25PI 	= 	(LONG*)(RS_BUF_ADDR + 392); 	//T25������ʱ T25PI
LONG* RS_pnT25RE 	= 	(LONG*)(RS_BUF_ADDR + 396); 	//T25������ʱ T25RE
LONG* RS_pnT26PI 	= 	(LONG*)(RS_BUF_ADDR + 400); 	//T26������ʱ T26PI
LONG* RS_pnT26RE 	= 	(LONG*)(RS_BUF_ADDR + 404); 	//T26������ʱ T26RE
LONG* RS_pnT27PI 	= 	(LONG*)(RS_BUF_ADDR + 408); 	//T27������ʱ T27PI
LONG* RS_pnT27RE 	= 	(LONG*)(RS_BUF_ADDR + 412); 	//T27������ʱ T27RE
LONG* RS_pnT28PI 	= 	(LONG*)(RS_BUF_ADDR + 416); 	//T28������ʱ T28PI
LONG* RS_pnT28RE 	= 	(LONG*)(RS_BUF_ADDR + 420); 	//T28������ʱ T28RE
LONG* RS_pnT29PI 	= 	(LONG*)(RS_BUF_ADDR + 424); 	//T29������ʱ T29PI
LONG* RS_pnT29RE 	= 	(LONG*)(RS_BUF_ADDR + 428); 	//T29������ʱ T29RE
LONG* RS_pnT30PI 	= 	(LONG*)(RS_BUF_ADDR + 432); 	//T30������ʱ T30PI
LONG* RS_pnT30RE 	= 	(LONG*)(RS_BUF_ADDR + 436); 	//T30������ʱ T30RE
LONG* RS_pnT31PI 	= 	(LONG*)(RS_BUF_ADDR + 440); 	//T31������ʱ T31PI
LONG* RS_pnT31RE 	= 	(LONG*)(RS_BUF_ADDR + 444); 	//T31������ʱ T31RE
LONG* RS_pnT32PI 	= 	(LONG*)(RS_BUF_ADDR + 448); 	//T32������ʱ T32PI
LONG* RS_pnT32RE 	= 	(LONG*)(RS_BUF_ADDR + 452); 	//T32������ʱ T32RE
LONG* RS_pnTCB 	= 	(LONG*)(RS_BUF_ADDR + 456); 	//��·������ʱ�� TCB
LONG* RS_pnTSW 	= 	(LONG*)(RS_BUF_ADDR + 460); 	//�綯��������ʱ�� TSW
LONG* RS_pnREBZTD 	= 	(LONG*)(RS_BUF_ADDR + 464); 	//��Ͷ��Ͷ��ȴ�ʱ�� REBZTD
LONG* RS_pnCHARD 	= 	(LONG*)(RS_BUF_ADDR + 468); 	//����Ͷ���ʱ�� CHARD
LONG* RS_pn52BFD 	= 	(LONG*)(RS_BUF_ADDR + 472); 	//ʧ�鱣��ʱ�� 52BFD

//������ֵ----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//�����
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//TR1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//TR2����ʱ��
LONG* AS_pnTR3D 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//TR3����ʱ��
LONG* AS_pnTR4D 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//TR4����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnPT1Ratio 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//PT1���
LONG* AS_pnPT2Ratio 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//PT2���
LONG* AS_pnPT3Ratio 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//PT3���
LONG* AS_pnPT4Ratio 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//PT4���
LONG* AS_pnPT5Ratio 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//PT5���
LONG* AS_pnPT6Ratio 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//PT6���
LONG* AS_pnPT7Ratio 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//PT7���
LONG* AS_pnPT8Ratio 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//PT8���
LONG* AS_pnCT1Ratio 	= 	(LONG*)(AS_BUF_ADDR + 56); 	//CT1���
LONG* AS_pnCT2Ratio 	= 	(LONG*)(AS_BUF_ADDR + 60); 	//CT2���
LONG* AS_pnCT3Ratio 	= 	(LONG*)(AS_BUF_ADDR + 64); 	//CT3���
LONG* AS_pnCT4Ratio 	= 	(LONG*)(AS_BUF_ADDR + 68); 	//CT4���
LONG* AS_pnCT5Ratio 	= 	(LONG*)(AS_BUF_ADDR + 72); 	//CT5���
LONG* AS_pnCT6Ratio 	= 	(LONG*)(AS_BUF_ADDR + 76); 	//CT6���
LONG* AS_pnCT7Ratio 	= 	(LONG*)(AS_BUF_ADDR + 80); 	//CT7���
LONG* AS_pnCT8Ratio 	= 	(LONG*)(AS_BUF_ADDR + 84); 	//CT8���
LONG* AS_pnRT1 	= 	(LONG*)(AS_BUF_ADDR + 88); 	//RT1

//ϵ��----------------------------------
LONG* CF_pnUP1 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UP1
LONG* CF_pnUP2 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UP2
LONG* CF_pnUP3 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//UP3
LONG* CF_pnUP4 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//UP4
LONG* CF_pnUP5 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//UP5
LONG* CF_pnUP6 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//UP6
LONG* CF_pnUP7 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//UP7
LONG* CF_pnUP8 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//UP8
LONG* CF_pnIP1 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IP1
LONG* CF_pnIP2 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IP2
LONG* CF_pnIP3 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IP3
LONG* CF_pnIP4 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IP4
LONG* CF_pnIP5 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//IP5
LONG* CF_pnIP6 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//IP6
LONG* CF_pnIP7 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//IP7
LONG* CF_pnIP8 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//IP8
LONG* CF_pnIC1 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//IC1
LONG* CF_pnIC2 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//IC2
LONG* CF_pnUM1 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//UM1
LONG* CF_pnUM2 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//UM2
LONG* CF_pnUM3 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//UM3
LONG* CF_pnUM4 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//UM4
LONG* CF_pnUM5 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//UM5
LONG* CF_pnUM6 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//UM6
LONG* CF_pnUM7 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//UM7
LONG* CF_pnUM8 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//UM8
LONG* CF_pnIM1 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//IM1
LONG* CF_pnIM2 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//IM2
LONG* CF_pnIM3 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//IM3
LONG* CF_pnIM4 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//IM4
LONG* CF_pnIM5 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//IM5
LONG* CF_pnIM6 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IM6
LONG* CF_pnIM7 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IM7
LONG* CF_pnIM8 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//IM8
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//C4
LONG* CF_pnU1D 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//U1D
LONG* CF_pnU2D 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//U2D
LONG* CF_pnU3D 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//U3D
LONG* CF_pnU4D 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//U4D
LONG* CF_pnU5D 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//U5D
LONG* CF_pnU6D 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//U6D
LONG* CF_pnU7D 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//U7D
LONG* CF_pnU8D 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//U8D
LONG* CF_pnI1D 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//I1D
LONG* CF_pnI2D 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//I2D
LONG* CF_pnI3D 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//I3D
LONG* CF_pnI4D 	= 	(LONG*)(CF_BUF_ADDR + 196); 	//I4D
LONG* CF_pnI5D 	= 	(LONG*)(CF_BUF_ADDR + 200); 	//I5D
LONG* CF_pnI6D 	= 	(LONG*)(CF_BUF_ADDR + 204); 	//I6D
LONG* CF_pnI7D 	= 	(LONG*)(CF_BUF_ADDR + 208); 	//I7D
LONG* CF_pnI8D 	= 	(LONG*)(CF_BUF_ADDR + 212); 	//I8D

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 80), // IN1
    (LONG*)(RM_BUF_ADDR + 84) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 72), // BR1
    (LONG*)(RM_BUF_ADDR + 76) // BR2
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

//ͨ������19----------
const  LONG* FailureInf19[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
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

//����Ͷ����1500----------
const  LONG* FailureInf1500[] = {
    {NULL}
};

//1#�����������Ͷ1501----------
const  LONG* FailureInf1501[] = {
    {NULL}
};

//2#�����������Ͷ1502----------
const  LONG* FailureInf1502[] = {
    {NULL}
};

//1#����ʧѹ������Ͷ1503----------
const  LONG* FailureInf1503[] = {
    {NULL}
};

//2#����ʧѹ������Ͷ1504----------
const  LONG* FailureInf1504[] = {
    {NULL}
};

//1#����ʧѹ��1#�����1505----------
const  LONG* FailureInf1505[] = {
    {NULL}
};

//1#����ʧѹ��2#�����1506----------
const  LONG* FailureInf1506[] = {
    {NULL}
};

//2#����ʧѹ��2#�����1507----------
const  LONG* FailureInf1507[] = {
    {NULL}
};

//2#����ʧѹ��1#�����1508----------
const  LONG* FailureInf1508[] = {
    {NULL}
};

//��Ͷ�˳�1509----------
const  LONG* FailureInf1509[] = {
    {NULL}
};

//1#�����1510----------
const  LONG* FailureInf1510[] = {
    {NULL}
};

//2#�����1511----------
const  LONG* FailureInf1511[] = {
    {NULL}
};

//1#����ʧѹ1512----------
const  LONG* FailureInf1512[] = {
    {NULL}
};

//2#����ʧѹ1513----------
const  LONG* FailureInf1513[] = {
    {NULL}
};

//����Ͷ�ɹ�1514----------
const  LONG* FailureInf1514[] = {
    {NULL}
};

//����Ͷʧ�� 1515----------
const  LONG* FailureInf1515[] = {
    {NULL}
};

//PT1����1516----------
const  LONG* FailureInf1516[] = {
    {NULL}
};

//PT2����1517----------
const  LONG* FailureInf1517[] = {
    {NULL}
};

//PT3����1518----------
const  LONG* FailureInf1518[] = {
    {NULL}
};

//PT4����1519----------
const  LONG* FailureInf1519[] = {
    {NULL}
};

//װ�ñ�����1520----------
const  LONG* FailureInf1520[] = {
    {NULL}
};

//���������1521----------
const  LONG* FailureInf1521[] = {
    {NULL}
};

//1AT��������1522----------
const  LONG* FailureInf1522[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//1ATT�߹�������1523----------
const  LONG* FailureInf1523[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//1ATF�߹�������1524----------
const  LONG* FailureInf1524[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//1AT���ǹ�����������1525----------
const  LONG* FailureInf1525[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2AT��������1526----------
const  LONG* FailureInf1526[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2ATT�߹�����������1527----------
const  LONG* FailureInf1527[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2ATF�߹�����������1528----------
const  LONG* FailureInf1528[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//2AT���ǹ�����������1529----------
const  LONG* FailureInf1529[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//3AT��������1530----------
const  LONG* FailureInf1530[] = {
    {NULL}
};

//3ATT�߹�������1531----------
const  LONG* FailureInf1531[] = {
    {NULL}
};

//3ATF�߹�������1532----------
const  LONG* FailureInf1532[] = {
    {NULL}
};

//3AT���ǹ�����������1533----------
const  LONG* FailureInf1533[] = {
    {NULL}
};

//4AT��������1534----------
const  LONG* FailureInf1534[] = {
    {NULL}
};

//4ATT�߹�����������1535----------
const  LONG* FailureInf1535[] = {
    {NULL}
};

//4ATF�߹�����������1536----------
const  LONG* FailureInf1536[] = {
    {NULL}
};

//4AT���ǹ�����������1537----------
const  LONG* FailureInf1537[] = {
    {NULL}
};

//1#���ߵ�ѹ��������1538----------
const  LONG* FailureInf1538[] = {
    {NULL}
};

//2#���ߵ�ѹ��������1539----------
const  LONG* FailureInf1539[] = {
    {NULL}
};

//��1��·��1540----------
const  LONG* FailureInf1540[] = {
    {NULL}
};

//��1��·��1541----------
const  LONG* FailureInf1541[] = {
    {NULL}
};

//��1��·��ʧ��1542----------
const  LONG* FailureInf1542[] = {
    {NULL}
};

//��1��·��ʧ��1543----------
const  LONG* FailureInf1543[] = {
    {NULL}
};

//��2��·��1544----------
const  LONG* FailureInf1544[] = {
    {NULL}
};

//��2��·��1545----------
const  LONG* FailureInf1545[] = {
    {NULL}
};

//��2��·��ʧ��1546----------
const  LONG* FailureInf1546[] = {
    {NULL}
};

//��2��·��ʧ��1547----------
const  LONG* FailureInf1547[] = {
    {NULL}
};

//��3��·��1548----------
const  LONG* FailureInf1548[] = {
    {NULL}
};

//��3��·��1549----------
const  LONG* FailureInf1549[] = {
    {NULL}
};

//��3��·��ʧ��1550----------
const  LONG* FailureInf1550[] = {
    {NULL}
};

//��3��·��ʧ��1551----------
const  LONG* FailureInf1551[] = {
    {NULL}
};

//��4��·��1552----------
const  LONG* FailureInf1552[] = {
    {NULL}
};

//��4��·��1553----------
const  LONG* FailureInf1553[] = {
    {NULL}
};

//��4��·��ʧ��1554----------
const  LONG* FailureInf1554[] = {
    {NULL}
};

//��4��·��ʧ��1555----------
const  LONG* FailureInf1555[] = {
    {NULL}
};

//��5��·��1556----------
const  LONG* FailureInf1556[] = {
    {NULL}
};

//��5��·��1557----------
const  LONG* FailureInf1557[] = {
    {NULL}
};

//��5��·��ʧ��1558----------
const  LONG* FailureInf1558[] = {
    {NULL}
};

//��5��·��ʧ��1559----------
const  LONG* FailureInf1559[] = {
    {NULL}
};

//��6��·��1560----------
const  LONG* FailureInf1560[] = {
    {NULL}
};

//��6��·��1561----------
const  LONG* FailureInf1561[] = {
    {NULL}
};

//��6��·��ʧ��1562----------
const  LONG* FailureInf1562[] = {
    {NULL}
};

//��6��·��ʧ��1563----------
const  LONG* FailureInf1563[] = {
    {NULL}
};

//��7��·��1564----------
const  LONG* FailureInf1564[] = {
    {NULL}
};

//��7��·��1565----------
const  LONG* FailureInf1565[] = {
    {NULL}
};

//��7��·��ʧ��1566----------
const  LONG* FailureInf1566[] = {
    {NULL}
};

//��7��·��ʧ��1567----------
const  LONG* FailureInf1567[] = {
    {NULL}
};

//��8��·��1568----------
const  LONG* FailureInf1568[] = {
    {NULL}
};

//��8��·��1569----------
const  LONG* FailureInf1569[] = {
    {NULL}
};

//��8��·��ʧ��1570----------
const  LONG* FailureInf1570[] = {
    {NULL}
};

//��8��·��ʧ��1571----------
const  LONG* FailureInf1571[] = {
    {NULL}
};

//��9��·��1572----------
const  LONG* FailureInf1572[] = {
    {NULL}
};

//��9��·��1573----------
const  LONG* FailureInf1573[] = {
    {NULL}
};

//��9��·��ʧ��1574----------
const  LONG* FailureInf1574[] = {
    {NULL}
};

//��9��·��ʧ��1575----------
const  LONG* FailureInf1575[] = {
    {NULL}
};

//��10��·��1576----------
const  LONG* FailureInf1576[] = {
    {NULL}
};

//��10��·��1577----------
const  LONG* FailureInf1577[] = {
    {NULL}
};

//��10��·��ʧ��1578----------
const  LONG* FailureInf1578[] = {
    {NULL}
};

//��10��·��ʧ��1579----------
const  LONG* FailureInf1579[] = {
    {NULL}
};

//��1����1580----------
const  LONG* FailureInf1580[] = {
    {NULL}
};

//��1����1581----------
const  LONG* FailureInf1581[] = {
    {NULL}
};

//��1����ʧ��1582----------
const  LONG* FailureInf1582[] = {
    {NULL}
};

//��1����ʧ��1583----------
const  LONG* FailureInf1583[] = {
    {NULL}
};

//��2����1584----------
const  LONG* FailureInf1584[] = {
    {NULL}
};

//��2����1585----------
const  LONG* FailureInf1585[] = {
    {NULL}
};

//��2����ʧ��1586----------
const  LONG* FailureInf1586[] = {
    {NULL}
};

//��2����ʧ��1587----------
const  LONG* FailureInf1587[] = {
    {NULL}
};

//��3����1588----------
const  LONG* FailureInf1588[] = {
    {NULL}
};

//��3����1589----------
const  LONG* FailureInf1589[] = {
    {NULL}
};

//��3����ʧ��1590----------
const  LONG* FailureInf1590[] = {
    {NULL}
};

//��3����ʧ��1591----------
const  LONG* FailureInf1591[] = {
    {NULL}
};

//��4����1592----------
const  LONG* FailureInf1592[] = {
    {NULL}
};

//��4����1593----------
const  LONG* FailureInf1593[] = {
    {NULL}
};

//��4����ʧ��1594----------
const  LONG* FailureInf1594[] = {
    {NULL}
};

//��4����ʧ��1595----------
const  LONG* FailureInf1595[] = {
    {NULL}
};

//��5����1596----------
const  LONG* FailureInf1596[] = {
    {NULL}
};

//��5����1597----------
const  LONG* FailureInf1597[] = {
    {NULL}
};

//��5����ʧ��1598----------
const  LONG* FailureInf1598[] = {
    {NULL}
};

//��5����ʧ��1599----------
const  LONG* FailureInf1599[] = {
    {NULL}
};

//��6����1600----------
const  LONG* FailureInf1600[] = {
    {NULL}
};

//��6����1601----------
const  LONG* FailureInf1601[] = {
    {NULL}
};

//��6����ʧ��1602----------
const  LONG* FailureInf1602[] = {
    {NULL}
};

//��6����ʧ��1603----------
const  LONG* FailureInf1603[] = {
    {NULL}
};

//��7����1604----------
const  LONG* FailureInf1604[] = {
    {NULL}
};

//��7����1605----------
const  LONG* FailureInf1605[] = {
    {NULL}
};

//��7����ʧ��1606----------
const  LONG* FailureInf1606[] = {
    {NULL}
};

//��7����ʧ��1607----------
const  LONG* FailureInf1607[] = {
    {NULL}
};

//��8����1608----------
const  LONG* FailureInf1608[] = {
    {NULL}
};

//��8����1609----------
const  LONG* FailureInf1609[] = {
    {NULL}
};

//��8����ʧ��1610----------
const  LONG* FailureInf1610[] = {
    {NULL}
};

//��8����ʧ��1611----------
const  LONG* FailureInf1611[] = {
    {NULL}
};

//��9����1612----------
const  LONG* FailureInf1612[] = {
    {NULL}
};

//��9����1613----------
const  LONG* FailureInf1613[] = {
    {NULL}
};

//��9����ʧ��1614----------
const  LONG* FailureInf1614[] = {
    {NULL}
};

//��9����ʧ��1615----------
const  LONG* FailureInf1615[] = {
    {NULL}
};

//��10����1616----------
const  LONG* FailureInf1616[] = {
    {NULL}
};

//��10����1617----------
const  LONG* FailureInf1617[] = {
    {NULL}
};

//��10����ʧ��1618----------
const  LONG* FailureInf1618[] = {
    {NULL}
};

//��10����ʧ��1619----------
const  LONG* FailureInf1619[] = {
    {NULL}
};

//��101��·��1620----------
const  LONG* FailureInf1620[] = {
    {NULL}
};

//��101��·��1621----------
const  LONG* FailureInf1621[] = {
    {NULL}
};

//��101��·��ʧ��1622----------
const  LONG* FailureInf1622[] = {
    {NULL}
};

//��101��·��ʧ��1623----------
const  LONG* FailureInf1623[] = {
    {NULL}
};

//��102��·��1624----------
const  LONG* FailureInf1624[] = {
    {NULL}
};

//��102��·��1625----------
const  LONG* FailureInf1625[] = {
    {NULL}
};

//��102��·��ʧ��1626----------
const  LONG* FailureInf1626[] = {
    {NULL}
};

//��102��·��ʧ��1627----------
const  LONG* FailureInf1627[] = {
    {NULL}
};

//��103��·��1628----------
const  LONG* FailureInf1628[] = {
    {NULL}
};

//��103��·��1629----------
const  LONG* FailureInf1629[] = {
    {NULL}
};

//��103��·��ʧ��1630----------
const  LONG* FailureInf1630[] = {
    {NULL}
};

//��103��·��ʧ��1631----------
const  LONG* FailureInf1631[] = {
    {NULL}
};

//��104��·��1632----------
const  LONG* FailureInf1632[] = {
    {NULL}
};

//��104��·��1633----------
const  LONG* FailureInf1633[] = {
    {NULL}
};

//��104��·��ʧ��1634----------
const  LONG* FailureInf1634[] = {
    {NULL}
};

//��104��·��ʧ��1635----------
const  LONG* FailureInf1635[] = {
    {NULL}
};

//��201��·��1636----------
const  LONG* FailureInf1636[] = {
    {NULL}
};

//��201��·��1637----------
const  LONG* FailureInf1637[] = {
    {NULL}
};

//��201��·��ʧ��1638----------
const  LONG* FailureInf1638[] = {
    {NULL}
};

//��201��·��ʧ��1639----------
const  LONG* FailureInf1639[] = {
    {NULL}
};

//��202��·��1640----------
const  LONG* FailureInf1640[] = {
    {NULL}
};

//��202��·��1641----------
const  LONG* FailureInf1641[] = {
    {NULL}
};

//��202��·��ʧ��1642----------
const  LONG* FailureInf1642[] = {
    {NULL}
};

//��202��·��ʧ��1643----------
const  LONG* FailureInf1643[] = {
    {NULL}
};

//��203��·��1644----------
const  LONG* FailureInf1644[] = {
    {NULL}
};

//��203��·��1645----------
const  LONG* FailureInf1645[] = {
    {NULL}
};

//��203��·��ʧ��1646----------
const  LONG* FailureInf1646[] = {
    {NULL}
};

//��203��·��ʧ��1647----------
const  LONG* FailureInf1647[] = {
    {NULL}
};

//��204��·��1648----------
const  LONG* FailureInf1648[] = {
    {NULL}
};

//��204��·��1649----------
const  LONG* FailureInf1649[] = {
    {NULL}
};

//��204��·��ʧ��1650----------
const  LONG* FailureInf1650[] = {
    {NULL}
};

//��204��·��ʧ��1651----------
const  LONG* FailureInf1651[] = {
    {NULL}
};

//��201A��·��1652----------
const  LONG* FailureInf1652[] = {
    {NULL}
};

//��201A��·��1653----------
const  LONG* FailureInf1653[] = {
    {NULL}
};

//��201A��·��ʧ��1654----------
const  LONG* FailureInf1654[] = {
    {NULL}
};

//��201A��·��ʧ��1655----------
const  LONG* FailureInf1655[] = {
    {NULL}
};

//��201B��·��1656----------
const  LONG* FailureInf1656[] = {
    {NULL}
};

//��201B��·��1657----------
const  LONG* FailureInf1657[] = {
    {NULL}
};

//��201B��·��ʧ��1658----------
const  LONG* FailureInf1658[] = {
    {NULL}
};

//��201B��·��ʧ��1659----------
const  LONG* FailureInf1659[] = {
    {NULL}
};

//��202A��·��1660----------
const  LONG* FailureInf1660[] = {
    {NULL}
};

//��202A��·��1661----------
const  LONG* FailureInf1661[] = {
    {NULL}
};

//��202A��·��ʧ��1662----------
const  LONG* FailureInf1662[] = {
    {NULL}
};

//��202A��·��ʧ��1663----------
const  LONG* FailureInf1663[] = {
    {NULL}
};

//��202B��·��   1664----------
const  LONG* FailureInf1664[] = {
    {NULL}
};

//��202B��·��1665----------
const  LONG* FailureInf1665[] = {
    {NULL}
};

//��202B��·��ʧ��1666----------
const  LONG* FailureInf1666[] = {
    {NULL}
};

//��202B��·��ʧ��1667----------
const  LONG* FailureInf1667[] = {
    {NULL}
};

//��1011����1668----------
const  LONG* FailureInf1668[] = {
    {NULL}
};

//��1011����1669----------
const  LONG* FailureInf1669[] = {
    {NULL}
};

//��2011����ʧ��1670----------
const  LONG* FailureInf1670[] = {
    {NULL}
};

//��1011����ʧ��1671----------
const  LONG* FailureInf1671[] = {
    {NULL}
};

//��1021����1672----------
const  LONG* FailureInf1672[] = {
    {NULL}
};

//��1021����1673----------
const  LONG* FailureInf1673[] = {
    {NULL}
};

//��1021����ʧ��1674----------
const  LONG* FailureInf1674[] = {
    {NULL}
};

//��1021����ʧ��1675----------
const  LONG* FailureInf1675[] = {
    {NULL}
};

//��1001����1676----------
const  LONG* FailureInf1676[] = {
    {NULL}
};

//��1001����1677----------
const  LONG* FailureInf1677[] = {
    {NULL}
};

//��1001����ʧ��1678----------
const  LONG* FailureInf1678[] = {
    {NULL}
};

//��1001����ʧ��1679----------
const  LONG* FailureInf1679[] = {
    {NULL}
};

//��1002����1680----------
const  LONG* FailureInf1680[] = {
    {NULL}
};

//��1002����1681----------
const  LONG* FailureInf1681[] = {
    {NULL}
};

//��1002����ʧ��1682----------
const  LONG* FailureInf1682[] = {
    {NULL}
};

//��1002����ʧ��1683----------
const  LONG* FailureInf1683[] = {
    {NULL}
};

//��1019����1684----------
const  LONG* FailureInf1684[] = {
    {NULL}
};

//��1019����1685----------
const  LONG* FailureInf1685[] = {
    {NULL}
};

//��1019����ʧ��1686----------
const  LONG* FailureInf1686[] = {
    {NULL}
};

//��1019����ʧ��1687----------
const  LONG* FailureInf1687[] = {
    {NULL}
};

//��1029����1688----------
const  LONG* FailureInf1688[] = {
    {NULL}
};

//��1029����1689----------
const  LONG* FailureInf1689[] = {
    {NULL}
};

//��1029����ʧ��1690----------
const  LONG* FailureInf1690[] = {
    {NULL}
};

//��1029����ʧ��1691----------
const  LONG* FailureInf1691[] = {
    {NULL}
};

//��1039����1692----------
const  LONG* FailureInf1692[] = {
    {NULL}
};

//��1039����1693----------
const  LONG* FailureInf1693[] = {
    {NULL}
};

//��1039����ʧ��1694----------
const  LONG* FailureInf1694[] = {
    {NULL}
};

//��1039����ʧ��1695----------
const  LONG* FailureInf1695[] = {
    {NULL}
};

//��1049����1696----------
const  LONG* FailureInf1696[] = {
    {NULL}
};

//��1049����1697----------
const  LONG* FailureInf1697[] = {
    {NULL}
};

//��1049����ʧ��1698----------
const  LONG* FailureInf1698[] = {
    {NULL}
};

//��1049����ʧ��1699----------
const  LONG* FailureInf1699[] = {
    {NULL}
};

//1AT����˹1700----------
const  LONG* FailureInf1700[] = {
    {NULL}
};

//1ATѹ���ͷ�1701----------
const  LONG* FailureInf1701[] = {
    {NULL}
};

//1AT�¶ȹ���1702----------
const  LONG* FailureInf1702[] = {
    {NULL}
};

//1AT����˹1703----------
const  LONG* FailureInf1703[] = {
    {NULL}
};

//1AT�¶ȸ澯1704----------
const  LONG* FailureInf1704[] = {
    {NULL}
};

//1AT��λ�쳣1705----------
const  LONG* FailureInf1705[] = {
    {NULL}
};

//1AT�����쳣1706----------
const  LONG* FailureInf1706[] = {
    {NULL}
};

//2AT����˹1707----------
const  LONG* FailureInf1707[] = {
    {NULL}
};

//2ATѹ���ͷ�1708----------
const  LONG* FailureInf1708[] = {
    {NULL}
};

//2AT�¶ȹ���1709----------
const  LONG* FailureInf1709[] = {
    {NULL}
};

//2AT����˹1710----------
const  LONG* FailureInf1710[] = {
    {NULL}
};

//2AT�¶ȸ澯1711----------
const  LONG* FailureInf1711[] = {
    {NULL}
};

//2AT��λ�쳣1712----------
const  LONG* FailureInf1712[] = {
    {NULL}
};

//2AT�����쳣1713----------
const  LONG* FailureInf1713[] = {
    {NULL}
};

//3AT����˹1714----------
const  LONG* FailureInf1714[] = {
    {NULL}
};

//3ATѹ���ͷ�1715----------
const  LONG* FailureInf1715[] = {
    {NULL}
};

//3AT�¶ȹ���1716----------
const  LONG* FailureInf1716[] = {
    {NULL}
};

//3AT����˹1717----------
const  LONG* FailureInf1717[] = {
    {NULL}
};

//3AT�¶ȸ澯1718----------
const  LONG* FailureInf1718[] = {
    {NULL}
};

//3AT��λ�쳣1719----------
const  LONG* FailureInf1719[] = {
    {NULL}
};

//3AT�����쳣1720----------
const  LONG* FailureInf1720[] = {
    {NULL}
};

//4AT����˹1721----------
const  LONG* FailureInf1721[] = {
    {NULL}
};

//4ATѹ���ͷ�1722----------
const  LONG* FailureInf1722[] = {
    {NULL}
};

//4AT�¶ȹ���1723----------
const  LONG* FailureInf1723[] = {
    {NULL}
};

//4AT����˹1724----------
const  LONG* FailureInf1724[] = {
    {NULL}
};

//4AT�¶ȸ澯1725----------
const  LONG* FailureInf1725[] = {
    {NULL}
};

//4AT��λ�쳣1726----------
const  LONG* FailureInf1726[] = {
    {NULL}
};

//4AT�����쳣1727----------
const  LONG* FailureInf1727[] = {
    {NULL}
};

//��1AT��ֿ���1728----------
const  LONG* FailureInf1728[] = {
    {NULL}
};

//��2AT��ֿ���1729----------
const  LONG* FailureInf1729[] = {
    {NULL}
};

//��3AT��ֿ���1730----------
const  LONG* FailureInf1730[] = {
    {NULL}
};

//��4AT��ֿ���1731----------
const  LONG* FailureInf1731[] = {
    {NULL}
};

//��1AT���ʧ��1732----------
const  LONG* FailureInf1732[] = {
    {NULL}
};

//��2AT���ʧ��1733----------
const  LONG* FailureInf1733[] = {
    {NULL}
};

//��3AT���ʧ��1734----------
const  LONG* FailureInf1734[] = {
    {NULL}
};

//��4AT���ʧ��1735----------
const  LONG* FailureInf1735[] = {
    {NULL}
};

//�����ɹ�1736----------
const  LONG* FailureInf1736[] = {
    {NULL}
};

//����ʧ��1737----------
const  LONG* FailureInf1737[] = {
    {NULL}
};

//����ѹ��բ�ɹ�1738----------
const  LONG* FailureInf1738[] = {
    {NULL}
};

//���к�բʧ��1739----------
const  LONG* FailureInf1739[] = {
    {NULL}
};

//��241��·��1740----------
const  LONG* FailureInf1740[] = {
    {NULL}
};

//��241��·��1741----------
const  LONG* FailureInf1741[] = {
    {NULL}
};

//��241��·��ʧ��1742----------
const  LONG* FailureInf1742[] = {
    {NULL}
};

//��241��·��ʧ��1743----------
const  LONG* FailureInf1743[] = {
    {NULL}
};

//��242��·��1744----------
const  LONG* FailureInf1744[] = {
    {NULL}
};

//��242��·��1745----------
const  LONG* FailureInf1745[] = {
    {NULL}
};

//��242��·��ʧ��1746----------
const  LONG* FailureInf1746[] = {
    {NULL}
};

//��242��·��ʧ��1747----------
const  LONG* FailureInf1747[] = {
    {NULL}
};

//��271��·��1748----------
const  LONG* FailureInf1748[] = {
    {NULL}
};

//��271��·��1749----------
const  LONG* FailureInf1749[] = {
    {NULL}
};

//��271��·��ʧ��1750----------
const  LONG* FailureInf1750[] = {
    {NULL}
};

//��271��·��ʧ��1751----------
const  LONG* FailureInf1751[] = {
    {NULL}
};

//��272��·��1752----------
const  LONG* FailureInf1752[] = {
    {NULL}
};

//��272��·��1753----------
const  LONG* FailureInf1753[] = {
    {NULL}
};

//��272��·��ʧ��1754----------
const  LONG* FailureInf1754[] = {
    {NULL}
};

//��272��·��ʧ��1755----------
const  LONG* FailureInf1755[] = {
    {NULL}
};

//��281��·��1756----------
const  LONG* FailureInf1756[] = {
    {NULL}
};

//��281��·��1757----------
const  LONG* FailureInf1757[] = {
    {NULL}
};

//��281��·��ʧ��1758----------
const  LONG* FailureInf1758[] = {
    {NULL}
};

//��281��·��ʧ��1759----------
const  LONG* FailureInf1759[] = {
    {NULL}
};

//��2411����1760----------
const  LONG* FailureInf1760[] = {
    {NULL}
};

//��2411����1761----------
const  LONG* FailureInf1761[] = {
    {NULL}
};

//��2411����ʧ��1762----------
const  LONG* FailureInf1762[] = {
    {NULL}
};

//��2411����ʧ��1763----------
const  LONG* FailureInf1763[] = {
    {NULL}
};

//��2421����1764----------
const  LONG* FailureInf1764[] = {
    {NULL}
};

//��2421����1765----------
const  LONG* FailureInf1765[] = {
    {NULL}
};

//��2421����ʧ��1766----------
const  LONG* FailureInf1766[] = {
    {NULL}
};

//��2421����ʧ��1767----------
const  LONG* FailureInf1767[] = {
    {NULL}
};

//��2711����1768----------
const  LONG* FailureInf1768[] = {
    {NULL}
};

//��2711����1769----------
const  LONG* FailureInf1769[] = {
    {NULL}
};

//��2711����ʧ��1770----------
const  LONG* FailureInf1770[] = {
    {NULL}
};

//��2711����ʧ��1771----------
const  LONG* FailureInf1771[] = {
    {NULL}
};

//��2721����1772----------
const  LONG* FailureInf1772[] = {
    {NULL}
};

//��2721����1773----------
const  LONG* FailureInf1773[] = {
    {NULL}
};

//��2721����ʧ��1774----------
const  LONG* FailureInf1774[] = {
    {NULL}
};

//��2721����ʧ��1775----------
const  LONG* FailureInf1775[] = {
    {NULL}
};

//1AT��������1777----------
const  LONG* FailureInf1777[] = {
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT��������1778----------
const  LONG* FailureInf1778[] = {
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT T�߹�������1779----------
const  LONG* FailureInf1779[] = {
    (LONG*)(RM_BUF_ADDR + 32) // Ip1
};

//1AT T�߹�������1780----------
const  LONG* FailureInf1780[] = {
    (LONG*)(RM_BUF_ADDR + 32) // Ip1
};

//1AT F�߹�������1781----------
const  LONG* FailureInf1781[] = {
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT F�߹�������1782----------
const  LONG* FailureInf1782[] = {
    (LONG*)(RM_BUF_ADDR + 36) // Ip2
};

//1AT���ǹ�������1783----------
const  LONG* FailureInf1783[] = {
    (LONG*)(RM_BUF_ADDR + 40) // Ip3
};

//1AT���ǹ�������1784----------
const  LONG* FailureInf1784[] = {
    (LONG*)(RM_BUF_ADDR + 40) // Ip3
};

//2AT��������1785----------
const  LONG* FailureInf1785[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT��������1786----------
const  LONG* FailureInf1786[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT T�߹�������1787----------
const  LONG* FailureInf1787[] = {
    (LONG*)(RM_BUF_ADDR + 44) // Ip4
};

//2AT T�߹�������1788----------
const  LONG* FailureInf1788[] = {
    (LONG*)(RM_BUF_ADDR + 44) // Ip4
};

//2AT F�߹�������1789----------
const  LONG* FailureInf1789[] = {
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT F�߹�������1790----------
const  LONG* FailureInf1790[] = {
    (LONG*)(RM_BUF_ADDR + 48) // Ip5
};

//2AT���ǹ�������1791----------
const  LONG* FailureInf1791[] = {
    (LONG*)(RM_BUF_ADDR + 52) // Ip6
};

//2AT���ǹ�������1792----------
const  LONG* FailureInf1792[] = {
    (LONG*)(RM_BUF_ADDR + 52) // Ip6
};

//��������4000----------
const  LONG* FailureInf4000[] = {
    (LONG*)(RM_BUF_ADDR + 0), // Up1
    (LONG*)(RM_BUF_ADDR + 4), // Up2
    (LONG*)(RM_BUF_ADDR + 8), // Up3
    (LONG*)(RM_BUF_ADDR + 12), // Up4
    (LONG*)(RM_BUF_ADDR + 16), // Up5
    (LONG*)(RM_BUF_ADDR + 20), // Up6
    (LONG*)(RM_BUF_ADDR + 24), // Up7
    (LONG*)(RM_BUF_ADDR + 28), // Up8
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40), // Ip3
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52), // Ip6
    (LONG*)(RM_BUF_ADDR + 56), // Ip7
    (LONG*)(RM_BUF_ADDR + 60), // Ip8
    (LONG*)(RM_BUF_ADDR + 64), // IC1
    (LONG*)(RM_BUF_ADDR + 68) // IC2
};

//AT1��·����բʧ��4001----------
const  LONG* FailureInf4001[] = {
    (LONG*)(RM_BUF_ADDR + 32), // Ip1
    (LONG*)(RM_BUF_ADDR + 36), // Ip2
    (LONG*)(RM_BUF_ADDR + 40) // Ip3
};

//AT2��·����բʧ��4002----------
const  LONG* FailureInf4002[] = {
    (LONG*)(RM_BUF_ADDR + 44), // Ip4
    (LONG*)(RM_BUF_ADDR + 48), // Ip5
    (LONG*)(RM_BUF_ADDR + 52) // Ip6
};

//1AT����������Ͷ4003----------
const  LONG* FailureInf4003[] = {
    {NULL}
};

//2AT����������Ͷ4004----------
const  LONG* FailureInf4004[] = {
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
    {18, FailureInf19, 19},//ͨ������
    {0, FailureInf50, 50},//AD���ϻָ�
    {0, FailureInf51, 51},//��ѹ����ϻָ�
    {0, FailureInf52, 52},//EEPROM��ֵ���ϻָ�
    {0, FailureInf55, 55},//RAM��ֵ���ϻָ�
    {0, FailureInf58, 58},//����ROM���ϻָ�
    {0, FailureInf60, 60},//ͨ��ͨ���쳣�ָ�
    {0, FailureInf62, 62},//NVRAM���ݳ���ָ�
    {0, FailureInf63, 63},//�߼����̹��Ϲ��ϻָ�
    {0, FailureInf500, 500},//��������
    {0, FailureInf1500, 1500},//����Ͷ����
    {0, FailureInf1501, 1501},//1#�����������Ͷ
    {0, FailureInf1502, 1502},//2#�����������Ͷ
    {0, FailureInf1503, 1503},//1#����ʧѹ������Ͷ
    {0, FailureInf1504, 1504},//2#����ʧѹ������Ͷ
    {0, FailureInf1505, 1505},//1#����ʧѹ��1#�����
    {0, FailureInf1506, 1506},//1#����ʧѹ��2#�����
    {0, FailureInf1507, 1507},//2#����ʧѹ��2#�����
    {0, FailureInf1508, 1508},//2#����ʧѹ��1#�����
    {0, FailureInf1509, 1509},//��Ͷ�˳�
    {0, FailureInf1510, 1510},//1#�����
    {0, FailureInf1511, 1511},//2#�����
    {0, FailureInf1512, 1512},//1#����ʧѹ
    {0, FailureInf1513, 1513},//2#����ʧѹ
    {0, FailureInf1514, 1514},//����Ͷ�ɹ�
    {0, FailureInf1515, 1515},//����Ͷʧ�� 
    {0, FailureInf1516, 1516},//PT1����
    {0, FailureInf1517, 1517},//PT2����
    {0, FailureInf1518, 1518},//PT3����
    {0, FailureInf1519, 1519},//PT4����
    {0, FailureInf1520, 1520},//װ�ñ�����
    {0, FailureInf1521, 1521},//���������
    {18, FailureInf1522, 1522},//1AT��������
    {18, FailureInf1523, 1523},//1ATT�߹�������
    {18, FailureInf1524, 1524},//1ATF�߹�������
    {18, FailureInf1525, 1525},//1AT���ǹ�����������
    {18, FailureInf1526, 1526},//2AT��������
    {18, FailureInf1527, 1527},//2ATT�߹�����������
    {18, FailureInf1528, 1528},//2ATF�߹�����������
    {18, FailureInf1529, 1529},//2AT���ǹ�����������
    {0, FailureInf1530, 1530},//3AT��������
    {0, FailureInf1531, 1531},//3ATT�߹�������
    {0, FailureInf1532, 1532},//3ATF�߹�������
    {0, FailureInf1533, 1533},//3AT���ǹ�����������
    {0, FailureInf1534, 1534},//4AT��������
    {0, FailureInf1535, 1535},//4ATT�߹�����������
    {0, FailureInf1536, 1536},//4ATF�߹�����������
    {0, FailureInf1537, 1537},//4AT���ǹ�����������
    {0, FailureInf1538, 1538},//1#���ߵ�ѹ��������
    {0, FailureInf1539, 1539},//2#���ߵ�ѹ��������
    {0, FailureInf1540, 1540},//��1��·��
    {0, FailureInf1541, 1541},//��1��·��
    {0, FailureInf1542, 1542},//��1��·��ʧ��
    {0, FailureInf1543, 1543},//��1��·��ʧ��
    {0, FailureInf1544, 1544},//��2��·��
    {0, FailureInf1545, 1545},//��2��·��
    {0, FailureInf1546, 1546},//��2��·��ʧ��
    {0, FailureInf1547, 1547},//��2��·��ʧ��
    {0, FailureInf1548, 1548},//��3��·��
    {0, FailureInf1549, 1549},//��3��·��
    {0, FailureInf1550, 1550},//��3��·��ʧ��
    {0, FailureInf1551, 1551},//��3��·��ʧ��
    {0, FailureInf1552, 1552},//��4��·��
    {0, FailureInf1553, 1553},//��4��·��
    {0, FailureInf1554, 1554},//��4��·��ʧ��
    {0, FailureInf1555, 1555},//��4��·��ʧ��
    {0, FailureInf1556, 1556},//��5��·��
    {0, FailureInf1557, 1557},//��5��·��
    {0, FailureInf1558, 1558},//��5��·��ʧ��
    {0, FailureInf1559, 1559},//��5��·��ʧ��
    {0, FailureInf1560, 1560},//��6��·��
    {0, FailureInf1561, 1561},//��6��·��
    {0, FailureInf1562, 1562},//��6��·��ʧ��
    {0, FailureInf1563, 1563},//��6��·��ʧ��
    {0, FailureInf1564, 1564},//��7��·��
    {0, FailureInf1565, 1565},//��7��·��
    {0, FailureInf1566, 1566},//��7��·��ʧ��
    {0, FailureInf1567, 1567},//��7��·��ʧ��
    {0, FailureInf1568, 1568},//��8��·��
    {0, FailureInf1569, 1569},//��8��·��
    {0, FailureInf1570, 1570},//��8��·��ʧ��
    {0, FailureInf1571, 1571},//��8��·��ʧ��
    {0, FailureInf1572, 1572},//��9��·��
    {0, FailureInf1573, 1573},//��9��·��
    {0, FailureInf1574, 1574},//��9��·��ʧ��
    {0, FailureInf1575, 1575},//��9��·��ʧ��
    {0, FailureInf1576, 1576},//��10��·��
    {0, FailureInf1577, 1577},//��10��·��
    {0, FailureInf1578, 1578},//��10��·��ʧ��
    {0, FailureInf1579, 1579},//��10��·��ʧ��
    {0, FailureInf1580, 1580},//��1����
    {0, FailureInf1581, 1581},//��1����
    {0, FailureInf1582, 1582},//��1����ʧ��
    {0, FailureInf1583, 1583},//��1����ʧ��
    {0, FailureInf1584, 1584},//��2����
    {0, FailureInf1585, 1585},//��2����
    {0, FailureInf1586, 1586},//��2����ʧ��
    {0, FailureInf1587, 1587},//��2����ʧ��
    {0, FailureInf1588, 1588},//��3����
    {0, FailureInf1589, 1589},//��3����
    {0, FailureInf1590, 1590},//��3����ʧ��
    {0, FailureInf1591, 1591},//��3����ʧ��
    {0, FailureInf1592, 1592},//��4����
    {0, FailureInf1593, 1593},//��4����
    {0, FailureInf1594, 1594},//��4����ʧ��
    {0, FailureInf1595, 1595},//��4����ʧ��
    {0, FailureInf1596, 1596},//��5����
    {0, FailureInf1597, 1597},//��5����
    {0, FailureInf1598, 1598},//��5����ʧ��
    {0, FailureInf1599, 1599},//��5����ʧ��
    {0, FailureInf1600, 1600},//��6����
    {0, FailureInf1601, 1601},//��6����
    {0, FailureInf1602, 1602},//��6����ʧ��
    {0, FailureInf1603, 1603},//��6����ʧ��
    {0, FailureInf1604, 1604},//��7����
    {0, FailureInf1605, 1605},//��7����
    {0, FailureInf1606, 1606},//��7����ʧ��
    {0, FailureInf1607, 1607},//��7����ʧ��
    {0, FailureInf1608, 1608},//��8����
    {0, FailureInf1609, 1609},//��8����
    {0, FailureInf1610, 1610},//��8����ʧ��
    {0, FailureInf1611, 1611},//��8����ʧ��
    {0, FailureInf1612, 1612},//��9����
    {0, FailureInf1613, 1613},//��9����
    {0, FailureInf1614, 1614},//��9����ʧ��
    {0, FailureInf1615, 1615},//��9����ʧ��
    {0, FailureInf1616, 1616},//��10����
    {0, FailureInf1617, 1617},//��10����
    {0, FailureInf1618, 1618},//��10����ʧ��
    {0, FailureInf1619, 1619},//��10����ʧ��
    {0, FailureInf1620, 1620},//��101��·��
    {0, FailureInf1621, 1621},//��101��·��
    {0, FailureInf1622, 1622},//��101��·��ʧ��
    {0, FailureInf1623, 1623},//��101��·��ʧ��
    {0, FailureInf1624, 1624},//��102��·��
    {0, FailureInf1625, 1625},//��102��·��
    {0, FailureInf1626, 1626},//��102��·��ʧ��
    {0, FailureInf1627, 1627},//��102��·��ʧ��
    {0, FailureInf1628, 1628},//��103��·��
    {0, FailureInf1629, 1629},//��103��·��
    {0, FailureInf1630, 1630},//��103��·��ʧ��
    {0, FailureInf1631, 1631},//��103��·��ʧ��
    {0, FailureInf1632, 1632},//��104��·��
    {0, FailureInf1633, 1633},//��104��·��
    {0, FailureInf1634, 1634},//��104��·��ʧ��
    {0, FailureInf1635, 1635},//��104��·��ʧ��
    {0, FailureInf1636, 1636},//��201��·��
    {0, FailureInf1637, 1637},//��201��·��
    {0, FailureInf1638, 1638},//��201��·��ʧ��
    {0, FailureInf1639, 1639},//��201��·��ʧ��
    {0, FailureInf1640, 1640},//��202��·��
    {0, FailureInf1641, 1641},//��202��·��
    {0, FailureInf1642, 1642},//��202��·��ʧ��
    {0, FailureInf1643, 1643},//��202��·��ʧ��
    {0, FailureInf1644, 1644},//��203��·��
    {0, FailureInf1645, 1645},//��203��·��
    {0, FailureInf1646, 1646},//��203��·��ʧ��
    {0, FailureInf1647, 1647},//��203��·��ʧ��
    {0, FailureInf1648, 1648},//��204��·��
    {0, FailureInf1649, 1649},//��204��·��
    {0, FailureInf1650, 1650},//��204��·��ʧ��
    {0, FailureInf1651, 1651},//��204��·��ʧ��
    {0, FailureInf1652, 1652},//��201A��·��
    {0, FailureInf1653, 1653},//��201A��·��
    {0, FailureInf1654, 1654},//��201A��·��ʧ��
    {0, FailureInf1655, 1655},//��201A��·��ʧ��
    {0, FailureInf1656, 1656},//��201B��·��
    {0, FailureInf1657, 1657},//��201B��·��
    {0, FailureInf1658, 1658},//��201B��·��ʧ��
    {0, FailureInf1659, 1659},//��201B��·��ʧ��
    {0, FailureInf1660, 1660},//��202A��·��
    {0, FailureInf1661, 1661},//��202A��·��
    {0, FailureInf1662, 1662},//��202A��·��ʧ��
    {0, FailureInf1663, 1663},//��202A��·��ʧ��
    {0, FailureInf1664, 1664},//��202B��·��   
    {0, FailureInf1665, 1665},//��202B��·��
    {0, FailureInf1666, 1666},//��202B��·��ʧ��
    {0, FailureInf1667, 1667},//��202B��·��ʧ��
    {0, FailureInf1668, 1668},//��1011����
    {0, FailureInf1669, 1669},//��1011����
    {0, FailureInf1670, 1670},//��2011����ʧ��
    {0, FailureInf1671, 1671},//��1011����ʧ��
    {0, FailureInf1672, 1672},//��1021����
    {0, FailureInf1673, 1673},//��1021����
    {0, FailureInf1674, 1674},//��1021����ʧ��
    {0, FailureInf1675, 1675},//��1021����ʧ��
    {0, FailureInf1676, 1676},//��1001����
    {0, FailureInf1677, 1677},//��1001����
    {0, FailureInf1678, 1678},//��1001����ʧ��
    {0, FailureInf1679, 1679},//��1001����ʧ��
    {0, FailureInf1680, 1680},//��1002����
    {0, FailureInf1681, 1681},//��1002����
    {0, FailureInf1682, 1682},//��1002����ʧ��
    {0, FailureInf1683, 1683},//��1002����ʧ��
    {0, FailureInf1684, 1684},//��1019����
    {0, FailureInf1685, 1685},//��1019����
    {0, FailureInf1686, 1686},//��1019����ʧ��
    {0, FailureInf1687, 1687},//��1019����ʧ��
    {0, FailureInf1688, 1688},//��1029����
    {0, FailureInf1689, 1689},//��1029����
    {0, FailureInf1690, 1690},//��1029����ʧ��
    {0, FailureInf1691, 1691},//��1029����ʧ��
    {0, FailureInf1692, 1692},//��1039����
    {0, FailureInf1693, 1693},//��1039����
    {0, FailureInf1694, 1694},//��1039����ʧ��
    {0, FailureInf1695, 1695},//��1039����ʧ��
    {0, FailureInf1696, 1696},//��1049����
    {0, FailureInf1697, 1697},//��1049����
    {0, FailureInf1698, 1698},//��1049����ʧ��
    {0, FailureInf1699, 1699},//��1049����ʧ��
    {0, FailureInf1700, 1700},//1AT����˹
    {0, FailureInf1701, 1701},//1ATѹ���ͷ�
    {0, FailureInf1702, 1702},//1AT�¶ȹ���
    {0, FailureInf1703, 1703},//1AT����˹
    {0, FailureInf1704, 1704},//1AT�¶ȸ澯
    {0, FailureInf1705, 1705},//1AT��λ�쳣
    {0, FailureInf1706, 1706},//1AT�����쳣
    {0, FailureInf1707, 1707},//2AT����˹
    {0, FailureInf1708, 1708},//2ATѹ���ͷ�
    {0, FailureInf1709, 1709},//2AT�¶ȹ���
    {0, FailureInf1710, 1710},//2AT����˹
    {0, FailureInf1711, 1711},//2AT�¶ȸ澯
    {0, FailureInf1712, 1712},//2AT��λ�쳣
    {0, FailureInf1713, 1713},//2AT�����쳣
    {0, FailureInf1714, 1714},//3AT����˹
    {0, FailureInf1715, 1715},//3ATѹ���ͷ�
    {0, FailureInf1716, 1716},//3AT�¶ȹ���
    {0, FailureInf1717, 1717},//3AT����˹
    {0, FailureInf1718, 1718},//3AT�¶ȸ澯
    {0, FailureInf1719, 1719},//3AT��λ�쳣
    {0, FailureInf1720, 1720},//3AT�����쳣
    {0, FailureInf1721, 1721},//4AT����˹
    {0, FailureInf1722, 1722},//4ATѹ���ͷ�
    {0, FailureInf1723, 1723},//4AT�¶ȹ���
    {0, FailureInf1724, 1724},//4AT����˹
    {0, FailureInf1725, 1725},//4AT�¶ȸ澯
    {0, FailureInf1726, 1726},//4AT��λ�쳣
    {0, FailureInf1727, 1727},//4AT�����쳣
    {0, FailureInf1728, 1728},//��1AT��ֿ���
    {0, FailureInf1729, 1729},//��2AT��ֿ���
    {0, FailureInf1730, 1730},//��3AT��ֿ���
    {0, FailureInf1731, 1731},//��4AT��ֿ���
    {0, FailureInf1732, 1732},//��1AT���ʧ��
    {0, FailureInf1733, 1733},//��2AT���ʧ��
    {0, FailureInf1734, 1734},//��3AT���ʧ��
    {0, FailureInf1735, 1735},//��4AT���ʧ��
    {0, FailureInf1736, 1736},//�����ɹ�
    {0, FailureInf1737, 1737},//����ʧ��
    {0, FailureInf1738, 1738},//����ѹ��բ�ɹ�
    {0, FailureInf1739, 1739},//���к�բʧ��
    {0, FailureInf1740, 1740},//��241��·��
    {0, FailureInf1741, 1741},//��241��·��
    {0, FailureInf1742, 1742},//��241��·��ʧ��
    {0, FailureInf1743, 1743},//��241��·��ʧ��
    {0, FailureInf1744, 1744},//��242��·��
    {0, FailureInf1745, 1745},//��242��·��
    {0, FailureInf1746, 1746},//��242��·��ʧ��
    {0, FailureInf1747, 1747},//��242��·��ʧ��
    {0, FailureInf1748, 1748},//��271��·��
    {0, FailureInf1749, 1749},//��271��·��
    {0, FailureInf1750, 1750},//��271��·��ʧ��
    {0, FailureInf1751, 1751},//��271��·��ʧ��
    {0, FailureInf1752, 1752},//��272��·��
    {0, FailureInf1753, 1753},//��272��·��
    {0, FailureInf1754, 1754},//��272��·��ʧ��
    {0, FailureInf1755, 1755},//��272��·��ʧ��
    {0, FailureInf1756, 1756},//��281��·��
    {0, FailureInf1757, 1757},//��281��·��
    {0, FailureInf1758, 1758},//��281��·��ʧ��
    {0, FailureInf1759, 1759},//��281��·��ʧ��
    {0, FailureInf1760, 1760},//��2411����
    {0, FailureInf1761, 1761},//��2411����
    {0, FailureInf1762, 1762},//��2411����ʧ��
    {0, FailureInf1763, 1763},//��2411����ʧ��
    {0, FailureInf1764, 1764},//��2421����
    {0, FailureInf1765, 1765},//��2421����
    {0, FailureInf1766, 1766},//��2421����ʧ��
    {0, FailureInf1767, 1767},//��2421����ʧ��
    {0, FailureInf1768, 1768},//��2711����
    {0, FailureInf1769, 1769},//��2711����
    {0, FailureInf1770, 1770},//��2711����ʧ��
    {0, FailureInf1771, 1771},//��2711����ʧ��
    {0, FailureInf1772, 1772},//��2721����
    {0, FailureInf1773, 1773},//��2721����
    {0, FailureInf1774, 1774},//��2721����ʧ��
    {0, FailureInf1775, 1775},//��2721����ʧ��
    {2, FailureInf1777, 1777},//1AT��������
    {2, FailureInf1778, 1778},//1AT��������
    {1, FailureInf1779, 1779},//1AT T�߹�������
    {1, FailureInf1780, 1780},//1AT T�߹�������
    {1, FailureInf1781, 1781},//1AT F�߹�������
    {1, FailureInf1782, 1782},//1AT F�߹�������
    {1, FailureInf1783, 1783},//1AT���ǹ�������
    {1, FailureInf1784, 1784},//1AT���ǹ�������
    {2, FailureInf1785, 1785},//2AT��������
    {2, FailureInf1786, 1786},//2AT��������
    {1, FailureInf1787, 1787},//2AT T�߹�������
    {1, FailureInf1788, 1788},//2AT T�߹�������
    {1, FailureInf1789, 1789},//2AT F�߹�������
    {1, FailureInf1790, 1790},//2AT F�߹�������
    {1, FailureInf1791, 1791},//2AT���ǹ�������
    {1, FailureInf1792, 1792},//2AT���ǹ�������
    {18, FailureInf4000, 4000},//��������
    {3, FailureInf4001, 4001},//AT1��·����բʧ��
    {3, FailureInf4002, 4002},//AT2��·����բʧ��
    {0, FailureInf4003, 4003},//1AT����������Ͷ
    {0, FailureInf4004, 4004},//2AT����������Ͷ
    {0, FailureInf20, 20},//5V��Դ����
    {0, FailureInf21, 21},//EEPROMд�����
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(RS_BUF_ADDR + 200), (LONG*)(RS_BUF_ADDR + 204) },
	{ (LONG*)(RS_BUF_ADDR + 208), (LONG*)(RS_BUF_ADDR + 212) },
	{ (LONG*)(RS_BUF_ADDR + 216), (LONG*)(RS_BUF_ADDR + 220) },
	{ (LONG*)(RS_BUF_ADDR + 224), (LONG*)(RS_BUF_ADDR + 228) },
	{ (LONG*)(RS_BUF_ADDR + 232), (LONG*)(RS_BUF_ADDR + 236) },
	{ (LONG*)(RS_BUF_ADDR + 240), (LONG*)(RS_BUF_ADDR + 244) },
	{ (LONG*)(RS_BUF_ADDR + 248), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(RS_BUF_ADDR + 256), (LONG*)(RS_BUF_ADDR + 260) },
	{ (LONG*)(RS_BUF_ADDR + 264), (LONG*)(RS_BUF_ADDR + 268) },
	{ (LONG*)(RS_BUF_ADDR + 272), (LONG*)(RS_BUF_ADDR + 276) },
	{ (LONG*)(RS_BUF_ADDR + 280), (LONG*)(RS_BUF_ADDR + 284) },
	{ (LONG*)(RS_BUF_ADDR + 288), (LONG*)(RS_BUF_ADDR + 292) },
	{ (LONG*)(RS_BUF_ADDR + 296), (LONG*)(RS_BUF_ADDR + 300) },
	{ (LONG*)(RS_BUF_ADDR + 304), (LONG*)(RS_BUF_ADDR + 308) },
	{ (LONG*)(RS_BUF_ADDR + 312), (LONG*)(RS_BUF_ADDR + 316) },
	{ (LONG*)(RS_BUF_ADDR + 320), (LONG*)(RS_BUF_ADDR + 324) },
	{ (LONG*)(RS_BUF_ADDR + 328), (LONG*)(RS_BUF_ADDR + 332) },
	{ (LONG*)(RS_BUF_ADDR + 336), (LONG*)(RS_BUF_ADDR + 340) },
	{ (LONG*)(RS_BUF_ADDR + 344), (LONG*)(RS_BUF_ADDR + 348) },
	{ (LONG*)(RS_BUF_ADDR + 352), (LONG*)(RS_BUF_ADDR + 356) },
	{ (LONG*)(RS_BUF_ADDR + 360), (LONG*)(RS_BUF_ADDR + 364) },
	{ (LONG*)(RS_BUF_ADDR + 368), (LONG*)(RS_BUF_ADDR + 372) },
	{ (LONG*)(RS_BUF_ADDR + 376), (LONG*)(RS_BUF_ADDR + 380) },
	{ (LONG*)(RS_BUF_ADDR + 384), (LONG*)(RS_BUF_ADDR + 388) },
	{ (LONG*)(RS_BUF_ADDR + 392), (LONG*)(RS_BUF_ADDR + 396) },
	{ (LONG*)(RS_BUF_ADDR + 400), (LONG*)(RS_BUF_ADDR + 404) },
	{ (LONG*)(RS_BUF_ADDR + 408), (LONG*)(RS_BUF_ADDR + 412) },
	{ (LONG*)(RS_BUF_ADDR + 416), (LONG*)(RS_BUF_ADDR + 420) },
	{ (LONG*)(RS_BUF_ADDR + 424), (LONG*)(RS_BUF_ADDR + 428) },
	{ (LONG*)(RS_BUF_ADDR + 432), (LONG*)(RS_BUF_ADDR + 436) },
	{ (LONG*)(RS_BUF_ADDR + 440), (LONG*)(RS_BUF_ADDR + 444) },
	{ (LONG*)(RS_BUF_ADDR + 448), (LONG*)(RS_BUF_ADDR + 452) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 456) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 460) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 464) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(RS_BUF_ADDR + 468), (LONG*)(NoTimeAddr) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 472) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 472) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 4) ,
	 (LONG*)(AS_BUF_ADDR + 8) ,
	 (LONG*)(AS_BUF_ADDR + 12) ,
	 (LONG*)(AS_BUF_ADDR + 16) ,
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
	 (WORD)(38) ,
	 (WORD)(39) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 8) ,
	 (BYTE*)(CFG_BUF_ADDR + 5) ,
	 (BYTE*)(CFG_BUF_ADDR + 6) ,
	 (BYTE*)(CFG_BUF_ADDR + 7) ,
	 (BYTE*)(CFG_BUF_ADDR + 9) ,
	 (BYTE*)(CFG_BUF_ADDR + 10) ,
	 (BYTE*)(CFG_BUF_ADDR + 11) ,
	 (BYTE*)(CFG_BUF_ADDR + 13) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 15) ,
	 (BYTE*)(CFG_BUF_ADDR + 16) ,
	 (BYTE*)(CFG_BUF_ADDR + 17) ,
	 (BYTE*)(CFG_BUF_ADDR + 18) ,
	 (BYTE*)(CFG_BUF_ADDR + 19) ,
	 (BYTE*)(CFG_BUF_ADDR + 20) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	11500, //UP1
	11500, //UP2
	11500, //UP3
	11500, //UP4
	11500, //UP5
	11500, //UP6
	11500, //UP7
	11500, //UP8
	3060, //IP1
	3060, //IP2
	3060, //IP3
	3060, //IP4
	3060, //IP5
	3060, //IP6
	3060, //IP7
	3060, //IP8
	3060, //IC1
	3060, //IC2
	16100, //UM1
	16100, //UM2
	16100, //UM3
	16100, //UM4
	16100, //UM5
	16100, //UM6
	16100, //UM7
	16100, //UM8
	4336, //IM1
	4334, //IM2
	4334, //IM3
	4334, //IM4
	4335, //IM5
	4334, //IM6
	4332, //IM7
	4332, //IM8
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	11, //U1D
	11, //U2D
	11, //U3D
	12, //U4D
	12, //U5D
	11, //U6D
	11, //U7D
	11, //U8D
	3, //I1D
	3, //I2D
	3, //I3D
	3, //I4D
	3, //I5D
	3, //I6D
	3, //I7D
	3, //I8D
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	11500, //UP1
	11500, //UP2
	11500, //UP3
	11500, //UP4
	11500, //UP5
	11500, //UP6
	11500, //UP7
	11500, //UP8
	15300, //IP1
	15300, //IP2
	15300, //IP3
	15300, //IP4
	15300, //IP5
	15300, //IP6
	15300, //IP7
	15300, //IP8
	15300, //IC1
	15300, //IC2
	16100, //UM1
	16100, //UM2
	16100, //UM3
	16100, //UM4
	16100, //UM5
	16100, //UM6
	16100, //UM7
	16100, //UM8
	21600, //IM1
	21600, //IM2
	21600, //IM3
	21600, //IM4
	21600, //IM5
	21600, //IM6
	21600, //IM7
	21600, //IM8
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	1, //U1D
	1, //U2D
	1, //U3D
	1, //U4D
	1, //U5D
	1, //U6D
	1, //U7D
	1, //U8D
	1, //I1D
	1, //I2D
	1, //I3D
	1, //I4D
	1, //I5D
	1, //I6D
	1, //I7D
	1, //I8D
};

//����----------------------------------
