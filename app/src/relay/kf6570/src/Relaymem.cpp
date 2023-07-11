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
};

//��������ֵ����-----------------------
LONG* 	RM_pnAUHA 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// ��UHA
LONG* 	RM_pnAUHB 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// ��UHB
LONG* 	RM_pnAUHC 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// ��UHC
LONG* 	RM_pnAULA 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// ��ULA
LONG* 	RM_pnAULB 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// ��ULB
LONG* 	RM_pnAULC 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// ��ULC
LONG* 	RM_pnAIHA 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// ��IHA
LONG* 	RM_pnAIHB 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// ��IHB
LONG* 	RM_pnAIHC 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// ��IHC
LONG* 	RM_pnAILA 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// ��ILA
LONG* 	RM_pnAILB 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// ��ILB
LONG* 	RM_pnAILC 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// ��ILC
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// IN2


//����ֵ����-----------------------
LONG* 	M_pnUHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 0); 	//UA UA
LONG* 	M_pnUHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 4); 	//UB UB
LONG* 	M_pnUHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 8); 	//UC UC
LONG* 	M_pnU0 	= 	(LONG*)(MEASURE_BUF_ADDR + 12); 	//U0 U0
LONG* 	M_pnULA 	= 	(LONG*)(MEASURE_BUF_ADDR + 16); 	//Ua Ua
LONG* 	M_pnULB 	= 	(LONG*)(MEASURE_BUF_ADDR + 20); 	//Ub Ub
LONG* 	M_pnULC 	= 	(LONG*)(MEASURE_BUF_ADDR + 24); 	//Uc Uc
LONG* 	M_pnIHA 	= 	(LONG*)(MEASURE_BUF_ADDR + 28); 	//IA IA
LONG* 	M_pnIHB 	= 	(LONG*)(MEASURE_BUF_ADDR + 32); 	//IB IB
LONG* 	M_pnIHC 	= 	(LONG*)(MEASURE_BUF_ADDR + 36); 	//IC IC
LONG* 	M_pnILA 	= 	(LONG*)(MEASURE_BUF_ADDR + 40); 	//Ia Ia
LONG* 	M_pnILB 	= 	(LONG*)(MEASURE_BUF_ADDR + 44); 	//Ib Ib
LONG* 	M_pnILC 	= 	(LONG*)(MEASURE_BUF_ADDR + 48); 	//Ic Ic
LONG* 	M_pnI1 	= 	(LONG*)(MEASURE_BUF_ADDR + 52); 	//I1 I1
LONG* 	M_pnI2 	= 	(LONG*)(MEASURE_BUF_ADDR + 56); 	//I2 I2
LONG* 	M_pnPA 	= 	(LONG*)(MEASURE_BUF_ADDR + 60); 	//PA PA
LONG* 	M_pnQA 	= 	(LONG*)(MEASURE_BUF_ADDR + 64); 	//QA QA
LONG* 	M_pnCOSA 	= 	(LONG*)(MEASURE_BUF_ADDR + 68); 	//COSA COSA
LONG* 	M_pnPB 	= 	(LONG*)(MEASURE_BUF_ADDR + 72); 	//PB PB
LONG* 	M_pnQB 	= 	(LONG*)(MEASURE_BUF_ADDR + 76); 	//QB QB
LONG* 	M_pnCOSB 	= 	(LONG*)(MEASURE_BUF_ADDR + 80); 	//COSB COSB
LONG* 	M_pnPC 	= 	(LONG*)(MEASURE_BUF_ADDR + 84); 	//PC PC
LONG* 	M_pnQC 	= 	(LONG*)(MEASURE_BUF_ADDR + 88); 	//QC QC
LONG* 	M_pnCOSC 	= 	(LONG*)(MEASURE_BUF_ADDR + 92); 	//COSC COSC
LONG* 	M_pnP 	= 	(LONG*)(MEASURE_BUF_ADDR + 96); 	//P P
LONG* 	M_pnQ 	= 	(LONG*)(MEASURE_BUF_ADDR + 100); 	//Q Q
LONG* 	M_pnCOS 	= 	(LONG*)(MEASURE_BUF_ADDR + 104); 	//COS COS
LONG* 	M_pnF 	= 	(LONG*)(MEASURE_BUF_ADDR + 108); 	//F F
LONG* 	M_pnUHAB 	= 	(LONG*)(MEASURE_BUF_ADDR + 112); 	//UAB UAB
LONG* 	M_pnUHBC 	= 	(LONG*)(MEASURE_BUF_ADDR + 116); 	//UBC UBC
LONG* 	M_pnUHCA 	= 	(LONG*)(MEASURE_BUF_ADDR + 120); 	//UCA UCA
LONG* 	M_pnUHA1 	= 	(LONG*)(MEASURE_BUF_ADDR + 124); 	//UA1 UA1
LONG* 	M_pnUHA2 	= 	(LONG*)(MEASURE_BUF_ADDR + 128); 	//UA2 UA2
LONG* 	M_pnUHA3 	= 	(LONG*)(MEASURE_BUF_ADDR + 132); 	//UA3 UA3
LONG* 	M_pnUHA5 	= 	(LONG*)(MEASURE_BUF_ADDR + 136); 	//UA5 UA5
LONG* 	M_pnUHA7 	= 	(LONG*)(MEASURE_BUF_ADDR + 140); 	//UA7 UA7
LONG* 	M_pnUHA9 	= 	(LONG*)(MEASURE_BUF_ADDR + 144); 	//UA9 UA9
LONG* 	M_pnUHA11 	= 	(LONG*)(MEASURE_BUF_ADDR + 148); 	//UA11 UA11
LONG* 	M_pnUHB1 	= 	(LONG*)(MEASURE_BUF_ADDR + 152); 	//UB1 UB1
LONG* 	M_pnUHB2 	= 	(LONG*)(MEASURE_BUF_ADDR + 156); 	//UB2 UB2
LONG* 	M_pnUHB3 	= 	(LONG*)(MEASURE_BUF_ADDR + 160); 	//UB3 UB3
LONG* 	M_pnUHB5 	= 	(LONG*)(MEASURE_BUF_ADDR + 164); 	//UB5 UB5
LONG* 	M_pnUHB7 	= 	(LONG*)(MEASURE_BUF_ADDR + 168); 	//UB7 UB7
LONG* 	M_pnUHB9 	= 	(LONG*)(MEASURE_BUF_ADDR + 172); 	//UB9 UB9
LONG* 	M_pnUHB11 	= 	(LONG*)(MEASURE_BUF_ADDR + 176); 	//UB11 UB11
LONG* 	M_pnUHC1 	= 	(LONG*)(MEASURE_BUF_ADDR + 180); 	//UC1 UC1
LONG* 	M_pnUHC2 	= 	(LONG*)(MEASURE_BUF_ADDR + 184); 	//UC2 UC2
LONG* 	M_pnUHC3 	= 	(LONG*)(MEASURE_BUF_ADDR + 188); 	//UC3 UC3
LONG* 	M_pnUHC5 	= 	(LONG*)(MEASURE_BUF_ADDR + 192); 	//UC5 UC5
LONG* 	M_pnUHC7 	= 	(LONG*)(MEASURE_BUF_ADDR + 196); 	//UC7 UC7
LONG* 	M_pnUHC9 	= 	(LONG*)(MEASURE_BUF_ADDR + 200); 	//UC9 UC9
LONG* 	M_pnUHC11 	= 	(LONG*)(MEASURE_BUF_ADDR + 204); 	//UC11 UC11
LONG* 	M_pnIHA1 	= 	(LONG*)(MEASURE_BUF_ADDR + 208); 	//IA1 IA1
LONG* 	M_pnIHA2 	= 	(LONG*)(MEASURE_BUF_ADDR + 212); 	//IA2 IA2
LONG* 	M_pnIHA3 	= 	(LONG*)(MEASURE_BUF_ADDR + 216); 	//IA3 IA3
LONG* 	M_pnIHA5 	= 	(LONG*)(MEASURE_BUF_ADDR + 220); 	//IA5 IA5
LONG* 	M_pnIHA7 	= 	(LONG*)(MEASURE_BUF_ADDR + 224); 	//IA7 IA7
LONG* 	M_pnIHA9 	= 	(LONG*)(MEASURE_BUF_ADDR + 228); 	//IA9 IA9
LONG* 	M_pnIHA11 	= 	(LONG*)(MEASURE_BUF_ADDR + 232); 	//IA11 IA11
LONG* 	M_pnIHB1 	= 	(LONG*)(MEASURE_BUF_ADDR + 236); 	//IB1 IB1
LONG* 	M_pnIHB2 	= 	(LONG*)(MEASURE_BUF_ADDR + 240); 	//IB2 IB2
LONG* 	M_pnIHB3 	= 	(LONG*)(MEASURE_BUF_ADDR + 244); 	//IB3 IB3
LONG* 	M_pnIHB5 	= 	(LONG*)(MEASURE_BUF_ADDR + 248); 	//IB5 IB5
LONG* 	M_pnIHB7 	= 	(LONG*)(MEASURE_BUF_ADDR + 252); 	//IB7 IB7
LONG* 	M_pnIHB9 	= 	(LONG*)(MEASURE_BUF_ADDR + 256); 	//IB9 IB9
LONG* 	M_pnIHB11 	= 	(LONG*)(MEASURE_BUF_ADDR + 260); 	//IB11 IB11
LONG* 	M_pnIHC1 	= 	(LONG*)(MEASURE_BUF_ADDR + 264); 	//IC1 IC1
LONG* 	M_pnIHC2 	= 	(LONG*)(MEASURE_BUF_ADDR + 268); 	//IC2 IC2
LONG* 	M_pnIHC3 	= 	(LONG*)(MEASURE_BUF_ADDR + 272); 	//IC3 IC3
LONG* 	M_pnIHC5 	= 	(LONG*)(MEASURE_BUF_ADDR + 276); 	//IC5 IC5
LONG* 	M_pnIHC7 	= 	(LONG*)(MEASURE_BUF_ADDR + 280); 	//IC7 IC7
LONG* 	M_pnIHC9 	= 	(LONG*)(MEASURE_BUF_ADDR + 284); 	//IC9 IC9
LONG* 	M_pnIHC11 	= 	(LONG*)(MEASURE_BUF_ADDR + 288); 	//IC11 IC11
LONG* 	M_pnIa1 	= 	(LONG*)(MEASURE_BUF_ADDR + 292); 	//Ia1 Ia1
LONG* 	M_pnIa2 	= 	(LONG*)(MEASURE_BUF_ADDR + 296); 	//Ia2 Ia2
LONG* 	M_pnIa3 	= 	(LONG*)(MEASURE_BUF_ADDR + 300); 	//Ia3 Ia3
LONG* 	M_pnIa5 	= 	(LONG*)(MEASURE_BUF_ADDR + 304); 	//Ia5 Ia5
LONG* 	M_pnIa7 	= 	(LONG*)(MEASURE_BUF_ADDR + 308); 	//Ia7 Ia7
LONG* 	M_pnIa9 	= 	(LONG*)(MEASURE_BUF_ADDR + 312); 	//Ia9 Ia9
LONG* 	M_pnIa11 	= 	(LONG*)(MEASURE_BUF_ADDR + 316); 	//Ia11 Ia11
LONG* 	M_pnIb1 	= 	(LONG*)(MEASURE_BUF_ADDR + 320); 	//Ib1 Ib1
LONG* 	M_pnIb2 	= 	(LONG*)(MEASURE_BUF_ADDR + 324); 	//Ib2 Ib2
LONG* 	M_pnIb3 	= 	(LONG*)(MEASURE_BUF_ADDR + 328); 	//Ib3 Ib3
LONG* 	M_pnIb5 	= 	(LONG*)(MEASURE_BUF_ADDR + 332); 	//Ib5 Ib5
LONG* 	M_pnIb7 	= 	(LONG*)(MEASURE_BUF_ADDR + 336); 	//Ib7 Ib7
LONG* 	M_pnIb9 	= 	(LONG*)(MEASURE_BUF_ADDR + 340); 	//Ib9 Ib9
LONG* 	M_pnIb11 	= 	(LONG*)(MEASURE_BUF_ADDR + 344); 	//Ib11 Ib11
LONG* 	M_pnIc1 	= 	(LONG*)(MEASURE_BUF_ADDR + 348); 	//Ic1 Ic1
LONG* 	M_pnIc2 	= 	(LONG*)(MEASURE_BUF_ADDR + 352); 	//Ic2 Ic2
LONG* 	M_pnIc3 	= 	(LONG*)(MEASURE_BUF_ADDR + 356); 	//Ic3 Ic3
LONG* 	M_pnIc5 	= 	(LONG*)(MEASURE_BUF_ADDR + 360); 	//Ic5 Ic5
LONG* 	M_pnIc7 	= 	(LONG*)(MEASURE_BUF_ADDR + 364); 	//Ic7 Ic7
LONG* 	M_pnIc9 	= 	(LONG*)(MEASURE_BUF_ADDR + 368); 	//Ic9 Ic9
LONG* 	M_pnIc11 	= 	(LONG*)(MEASURE_BUF_ADDR + 372); 	//Ic11 Ic11


//��ѹ��----------------------------------
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//����ѹ��
BYTE* SW_pbyBAK1 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//����ѹ��1
BYTE* SW_pbyBAK2 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//����ѹ��2
BYTE* SW_pbyBAK3 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//����ѹ��3
BYTE* SW_pbyBAK4 	= 	(BYTE*)(SW_BUF_ADDR + 4); 	//����ѹ��4

//����----------------------------------
BYTE* CFG_pbyMODSET 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//��ֵ�޸Ĺ���
BYTE* CFG_pbyBAK1 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//��������1
BYTE* CFG_pbyBAK2 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//��������2
BYTE* CFG_pbyBAK3 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//��������3
BYTE* CFG_pbyBAK4 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//��������4

//��ֵ----------------------------------
LONG* RS_pnF 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ F
LONG* RS_pnT1PI 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//T1������ʱ T1PI
LONG* RS_pnT1RE 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//T1������ʱ T1RE
LONG* RS_pnT2PI 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//T2������ʱ T2PI
LONG* RS_pnT2RE 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//T2������ʱ T2RE
LONG* RS_pnT3PI 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//T3������ʱ T3PI
LONG* RS_pnT3RE 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//T3������ʱ T3RE
LONG* RS_pnT4PI 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//T4������ʱ T4PI
LONG* RS_pnT4RE 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//T4������ʱ T4RE
LONG* RS_pnT5PI 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//T5������ʱ T5PI
LONG* RS_pnT5RE 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//T5������ʱ T5RE
LONG* RS_pnT6PI 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//T6������ʱ T6PI
LONG* RS_pnT6RE 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//T6������ʱ T6RE
LONG* RS_pnT7PI 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//T7������ʱ T7PI
LONG* RS_pnT7RE 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//T7������ʱ T7RE
LONG* RS_pnT8PI 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//T8������ʱ T8PI
LONG* RS_pnT8RE 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//T8������ʱ T8RE
LONG* RS_pnT9PI 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//T9������ʱ T9PI
LONG* RS_pnT9RE 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//T9������ʱ T9RE
LONG* RS_pnT10PI 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//T10������ʱ T10PI
LONG* RS_pnT10RE 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//T10������ʱ T10RE
LONG* RS_pnT11PI 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//T11������ʱ T11PI
LONG* RS_pnT11RE 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//T11������ʱ T11RE
LONG* RS_pnT12PI 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//T12������ʱ T12PI
LONG* RS_pnT12RE 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//T12������ʱ T12RE
LONG* RS_pnT13PI 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//T13������ʱ T13PI
LONG* RS_pnT13RE 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//T13������ʱ T13RE
LONG* RS_pnT14PI 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//T14������ʱ T14PI
LONG* RS_pnT14RE 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//T14������ʱ T14RE
LONG* RS_pnT15PI 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//T15������ʱ T15PI
LONG* RS_pnT15RE 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//T15������ʱ T15RE
LONG* RS_pnT16PI 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//T16������ʱ T16PI
LONG* RS_pnT16RE 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//T16������ʱ T16RE
LONG* RS_pn59HA 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//UHA��ѹ��ֵ 59HA
LONG* RS_pn27HA 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//UHA��ѹ��ֵ 27HA
LONG* RS_pn59HB 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//UHB��ѹ��ֵ 59HB
LONG* RS_pn27HB 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//UHB��ѹ��ֵ 27HB
LONG* RS_pn59HC 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//UHC��ѹ��ֵ 59HC
LONG* RS_pn27HC 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//UHC��ѹ��ֵ 27HC
LONG* RS_pn59N 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//U0��ѹ��ֵ 59N
LONG* RS_pn27N 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//U0��ѹ��ֵ 27N
LONG* RS_pn59LA 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//ULA��ѹ��ֵ 59LA
LONG* RS_pn27LA 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//ULA��ѹ��ֵ 27LA
LONG* RS_pn59LB 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//ULB��ѹ��ֵ 59LB
LONG* RS_pn27LB 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//ULB��ѹ��ֵ 27LB
LONG* RS_pn59LC 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//ULC��ѹ��ֵ 59LC
LONG* RS_pn27LC 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//ULC��ѹ��ֵ 27LC
LONG* RS_pn50HA 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//IHA������ֵ 50HA
LONG* RS_pn37HA 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//IHA������ֵ 37HA
LONG* RS_pn50HB 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//IHB������ֵ 50HB
LONG* RS_pn37HB 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//IHB������ֵ 37HB
LONG* RS_pn50HC 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//IHC������ֵ 50HC
LONG* RS_pn37HC 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//IHC������ֵ 37HC
LONG* RS_pn50LA 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//ILA������ֵ 50LA
LONG* RS_pn37LA 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//ILA������ֵ 37LA
LONG* RS_pn50LB 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//ILB������ֵ 50LB
LONG* RS_pn37LB 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//ILB������ֵ 37LB
LONG* RS_pn50LC 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//ILC������ֵ 50LC
LONG* RS_pn37LC 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//ILC������ֵ 37LC
LONG* RS_pn50I1 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//I1������ֵ 50I1
LONG* RS_pn37I1 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//I1������ֵ 37I1
LONG* RS_pn50I2 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//I2������ֵ 50I2
LONG* RS_pn37I2 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//I2������ֵ 37I2
LONG* RS_pnTCB 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//��·������ʱ�� TCB
LONG* RS_pnTSW 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//�綯��������ʱ�� TSW

//������ֵ----------------------------------
LONG* AS_pnRAC 	= 	(LONG*)(AS_BUF_ADDR + 0); 	//�����
LONG* AS_pnTR1D 	= 	(LONG*)(AS_BUF_ADDR + 4); 	//��բ�̵�����1����ʱ��
LONG* AS_pnTR2D 	= 	(LONG*)(AS_BUF_ADDR + 8); 	//��բ�̵�����2����ʱ��
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 12); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnHPTRT 	= 	(LONG*)(AS_BUF_ADDR + 16); 	//�߲�PT���
LONG* AS_pnLPTRT 	= 	(LONG*)(AS_BUF_ADDR + 20); 	//�Ͳ�PT���
LONG* AS_pnU0PTRT 	= 	(LONG*)(AS_BUF_ADDR + 24); 	//����PT���
LONG* AS_pnHCTRT 	= 	(LONG*)(AS_BUF_ADDR + 28); 	//�߲�CT���
LONG* AS_pnLACTRT 	= 	(LONG*)(AS_BUF_ADDR + 32); 	//�Ͳ�ACT���
LONG* AS_pnLBCTRT 	= 	(LONG*)(AS_BUF_ADDR + 36); 	//�Ͳ�BCT���
LONG* AS_pnLCCTRT 	= 	(LONG*)(AS_BUF_ADDR + 40); 	//�Ͳ�CCT���
LONG* AS_pnI1CTRT 	= 	(LONG*)(AS_BUF_ADDR + 44); 	//I1CT���
LONG* AS_pnI2CTRT 	= 	(LONG*)(AS_BUF_ADDR + 48); 	//I2CT���
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 52); 	//RT

//ϵ��----------------------------------
LONG* CF_pnUHA 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//UHA
LONG* CF_pnUHB 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//UHB
LONG* CF_pnUHC 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//UHC
LONG* CF_pnU0 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//U0
LONG* CF_pnULA 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//ULA
LONG* CF_pnULB 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//ULB
LONG* CF_pnULC 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//ULC
LONG* CF_pnIHA 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//IHA
LONG* CF_pnIHB 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IHB
LONG* CF_pnIHC 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IHC
LONG* CF_pnILA 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//ILA
LONG* CF_pnILB 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//ILB
LONG* CF_pnILC 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//ILC
LONG* CF_pnI1 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//I1
LONG* CF_pnI2 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//I2
LONG* CF_pnPA 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//PA
LONG* CF_pnPB 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//PB
LONG* CF_pnPC 	= 	(LONG*)(CF_BUF_ADDR + 68); 	//PC
LONG* CF_pnQA 	= 	(LONG*)(CF_BUF_ADDR + 72); 	//QA
LONG* CF_pnQB 	= 	(LONG*)(CF_BUF_ADDR + 76); 	//QB
LONG* CF_pnQC 	= 	(LONG*)(CF_BUF_ADDR + 80); 	//QC
LONG* CF_pnAA 	= 	(LONG*)(CF_BUF_ADDR + 84); 	//AA
LONG* CF_pnAB 	= 	(LONG*)(CF_BUF_ADDR + 88); 	//AB
LONG* CF_pnAC 	= 	(LONG*)(CF_BUF_ADDR + 92); 	//AC
LONG* CF_pnUHAD 	= 	(LONG*)(CF_BUF_ADDR + 96); 	//UHADRIFT
LONG* CF_pnUHBD 	= 	(LONG*)(CF_BUF_ADDR + 100); 	//UHBDRIFT
LONG* CF_pnUHCD 	= 	(LONG*)(CF_BUF_ADDR + 104); 	//UHCDRIFT
LONG* CF_pnU0D 	= 	(LONG*)(CF_BUF_ADDR + 108); 	//U0DRIFT
LONG* CF_pnULAD 	= 	(LONG*)(CF_BUF_ADDR + 112); 	//ULADRIFT
LONG* CF_pnULBD 	= 	(LONG*)(CF_BUF_ADDR + 116); 	//ULBDRIFT
LONG* CF_pnULCD 	= 	(LONG*)(CF_BUF_ADDR + 120); 	//ULCDRIFT
LONG* CF_pnIHAD 	= 	(LONG*)(CF_BUF_ADDR + 124); 	//IHADRIFT
LONG* CF_pnIHBD 	= 	(LONG*)(CF_BUF_ADDR + 128); 	//IHBDRIFT
LONG* CF_pnIHCD 	= 	(LONG*)(CF_BUF_ADDR + 132); 	//IHCDRIFT
LONG* CF_pnILAD 	= 	(LONG*)(CF_BUF_ADDR + 136); 	//ILADRIFT
LONG* CF_pnILBD 	= 	(LONG*)(CF_BUF_ADDR + 140); 	//ILBDRIFT
LONG* CF_pnILCD 	= 	(LONG*)(CF_BUF_ADDR + 144); 	//ILCDRIFT
LONG* CF_pnI1D 	= 	(LONG*)(CF_BUF_ADDR + 148); 	//I1DRIFT
LONG* CF_pnI2D 	= 	(LONG*)(CF_BUF_ADDR + 152); 	//I2DRIFT
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 156); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 160); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 164); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 168); 	//C4
LONG* CF_pnUHAH 	= 	(LONG*)(CF_BUF_ADDR + 172); 	//UHAH
LONG* CF_pnUHBH 	= 	(LONG*)(CF_BUF_ADDR + 176); 	//UHBH
LONG* CF_pnUHCH 	= 	(LONG*)(CF_BUF_ADDR + 180); 	//UHCH
LONG* CF_pnIHAH 	= 	(LONG*)(CF_BUF_ADDR + 184); 	//IHAH
LONG* CF_pnIHBH 	= 	(LONG*)(CF_BUF_ADDR + 188); 	//IHBH
LONG* CF_pnIHCH 	= 	(LONG*)(CF_BUF_ADDR + 192); 	//IHCH
LONG* CF_pnUAB 	= 	(LONG*)(CF_BUF_ADDR + 196); 	//UAB
LONG* CF_pnUBC 	= 	(LONG*)(CF_BUF_ADDR + 200); 	//UBC
LONG* CF_pnUCA 	= 	(LONG*)(CF_BUF_ADDR + 204); 	//UCA

//���ϵ������ö���------------------------------------
//RAM��ֵ����8----------
const  LONG* FailureInf8[] = {
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

//����ROM����11----------
const  LONG* FailureInf11[] = {
    {NULL}
};

//EEPROM��ֵ����5----------
const  LONG* FailureInf5[] = {
    {NULL}
};

//AD����3----------
const  LONG* FailureInf3[] = {
    {NULL}
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 48), // BR1
    (LONG*)(RM_BUF_ADDR + 52) // BR2
};

//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 56), // IN1
    (LONG*)(RM_BUF_ADDR + 60) // IN2
};

//��ѹ�����4----------
const  LONG* FailureInf4[] = {
    {NULL}
};

//�߼����̹���17----------
const  LONG* FailureInf17[] = {
    {NULL}
};

//ͨ������19----------
const  LONG* FailureInf19[] = {
    {NULL}
};

//�����Լ����18----------
const  LONG* FailureInf18[] = {
    {NULL}
};

//NVRAM���ݳ���ָ�62----------
const  LONG* FailureInf62[] = {
    {NULL}
};

//����ROM���ϻָ�58----------
const  LONG* FailureInf58[] = {
    {NULL}
};

//RAM��ֵ���ϻָ�55----------
const  LONG* FailureInf55[] = {
    {NULL}
};

//EEPROM��ֵ���ϻָ�52----------
const  LONG* FailureInf52[] = {
    {NULL}
};

//��ѹ����ϻָ�51----------
const  LONG* FailureInf51[] = {
    {NULL}
};

//AD���ϻָ�50----------
const  LONG* FailureInf50[] = {
    {NULL}
};

//�߼����̹��Ϲ��ϻָ�63----------
const  LONG* FailureInf63[] = {
    {NULL}
};

//ͨ��ͨ���쳣�ָ�60----------
const  LONG* FailureInf60[] = {
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
    {0, FailureInf8, 8},//RAM��ֵ����
    {0, FailureInf13, 13},//װ���쳣��λ
    {0, FailureInf14, 14},//ͨ��ͨ���쳣
    {0, FailureInf16, 16},//NVRAM���ݳ���
    {0, FailureInf11, 11},//����ROM����
    {0, FailureInf5, 5},//EEPROM��ֵ����
    {0, FailureInf3, 3},//AD����
    {2, FailureInf2, 2},//�����������
    {2, FailureInf1, 1},//�������ʧЧ
    {0, FailureInf4, 4},//��ѹ�����
    {0, FailureInf17, 17},//�߼����̹���
    {0, FailureInf19, 19},//ͨ������
    {0, FailureInf18, 18},//�����Լ����
    {0, FailureInf62, 62},//NVRAM���ݳ���ָ�
    {0, FailureInf58, 58},//����ROM���ϻָ�
    {0, FailureInf55, 55},//RAM��ֵ���ϻָ�
    {0, FailureInf52, 52},//EEPROM��ֵ���ϻָ�
    {0, FailureInf51, 51},//��ѹ����ϻָ�
    {0, FailureInf50, 50},//AD���ϻָ�
    {0, FailureInf63, 63},//�߼����̹��Ϲ��ϻָ�
    {0, FailureInf60, 60},//ͨ��ͨ���쳣�ָ�
    {0, FailureInf20, 20},//5V��Դ����
    {0, FailureInf21, 21},//EEPROMд�����
    {0, FailureInf64, 64},//5V��Դ���ϻָ�
    {0, FailureInf22, 22},//���������쳣
    {0, FailureInf65, 65},//���������쳣�ָ�
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,0,// ѹ��̵���
0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
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
	{ (LONG*)(RS_BUF_ADDR + 4), (LONG*)(RS_BUF_ADDR + 8) },
	{ (LONG*)(RS_BUF_ADDR + 12), (LONG*)(RS_BUF_ADDR + 16) },
	{ (LONG*)(RS_BUF_ADDR + 20), (LONG*)(RS_BUF_ADDR + 24) },
	{ (LONG*)(RS_BUF_ADDR + 28), (LONG*)(RS_BUF_ADDR + 32) },
	{ (LONG*)(RS_BUF_ADDR + 36), (LONG*)(RS_BUF_ADDR + 40) },
	{ (LONG*)(RS_BUF_ADDR + 44), (LONG*)(RS_BUF_ADDR + 48) },
	{ (LONG*)(RS_BUF_ADDR + 52), (LONG*)(RS_BUF_ADDR + 56) },
	{ (LONG*)(RS_BUF_ADDR + 60), (LONG*)(RS_BUF_ADDR + 64) },
	{ (LONG*)(RS_BUF_ADDR + 68), (LONG*)(RS_BUF_ADDR + 72) },
	{ (LONG*)(RS_BUF_ADDR + 76), (LONG*)(RS_BUF_ADDR + 80) },
	{ (LONG*)(RS_BUF_ADDR + 84), (LONG*)(RS_BUF_ADDR + 88) },
	{ (LONG*)(RS_BUF_ADDR + 92), (LONG*)(RS_BUF_ADDR + 96) },
	{ (LONG*)(RS_BUF_ADDR + 100), (LONG*)(RS_BUF_ADDR + 104) },
	{ (LONG*)(RS_BUF_ADDR + 108), (LONG*)(RS_BUF_ADDR + 112) },
	{ (LONG*)(RS_BUF_ADDR + 116), (LONG*)(RS_BUF_ADDR + 120) },
	{ (LONG*)(RS_BUF_ADDR + 124), (LONG*)(RS_BUF_ADDR + 128) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 252) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
	{ (LONG*)(NoTimeAddr), (LONG*)(RS_BUF_ADDR + 256) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 4) ,
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
	 (WORD)(82) ,
	 (WORD)(83) ,
	 (WORD)(84) ,
	 (WORD)(85) ,
	 (WORD)(86) ,
	 (WORD)(87) ,
	 (WORD)(88) ,
	 (WORD)(89) ,
	 (WORD)(90) ,
	 (WORD)(91) ,
	 (WORD)(92) ,
	 (WORD)(93) ,
	 (WORD)(94) ,
	 (WORD)(95) ,
	 (WORD)(96) ,
	 (WORD)(97) ,
	 (WORD)(98) ,
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
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	43126, //UHA
	43121, //UHB
	43111, //UHC
	43110, //U0
	16206, //ULA
	16201, //ULB
	16206, //ULC
	64670, //IHA
	64679, //IHB
	64727, //IHC
	64701, //ILA
	64747, //ILB
	64723, //ILC
	64694, //I1
	64670, //I2
	348694, //PA
	348688, //PB
	348863, //PC
	348694, //QA
	348688, //QB
	348863, //QC
	197, //AA
	171, //AB
	172, //AC
	3, //UHADRIFT
	3, //UHBDRIFT
	3, //UHCDRIFT
	2, //U0DRIFT
	3, //ULADRIFT
	3, //ULBDRIFT
	3, //ULCDRIFT
	3, //IHADRIFT
	3, //IHBDRIFT
	2, //IHCDRIFT
	2, //ILADRIFT
	3, //ILBDRIFT
	3, //ILCDRIFT
	3, //I1DRIFT
	3, //I2DRIFT
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	30520, //UHAH
	30517, //UHBH
	30510, //UHCH
	45768, //IHAH
	45775, //IHBH
	45808, //IHCH
	43123, //UAB
	43116, //UBC
	43118, //UCA
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	42924, //UHA
	42890, //UHB
	42951, //UHC
	42884, //U0
	16384, //ULA
	16384, //ULB
	16384, //ULC
	327670, //IHA
	327670, //IHB
	327670, //IHC
	327670, //ILA
	327670, //ILB
	327670, //ILC
	327670, //I1
	327670, //I2
	345352, //PA
	345490, //PB
	345448, //PC
	345352, //QA
	345490, //QB
	345448, //QC
	183, //AA
	180, //AB
	174, //AC
	10, //UHADRIFT
	10, //UHBDRIFT
	10, //UHCDRIFT
	10, //U0DRIFT
	10, //ULADRIFT
	10, //ULBDRIFT
	10, //ULCDRIFT
	8, //IHADRIFT
	6, //IHBDRIFT
	6, //IHCDRIFT
	6, //ILADRIFT
	8, //ILBDRIFT
	7, //ILCDRIFT
	8, //I1DRIFT
	7, //I2DRIFT
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
	30381, //UHAH
	30357, //UHBH
	30400, //UHCH
	227650, //IHAH
	227650, //IHBH
	227650, //IHCH
	42924, //UAB
	42924, //UBC
	42924, //UCA
};

//����----------------------------------
