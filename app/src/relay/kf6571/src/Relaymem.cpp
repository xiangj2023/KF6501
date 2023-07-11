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
	{FORWORD, "������"},
	{BACKWORD, "������"},
	{FORWORD, "������"},
	{BACKWORD, "������"},
	{ETHNET, "��̫��ֱ��"},
	{SERIAL, "��������"},
	{ROUTER, "·������"},
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
	{2, (TSetValueInf*)&(SetValueInf[0])}, //��ֵ��������0:+,1-
	{0, NULL},
	{2, (TSetValueInf*)&(SetValueInf[2])}, //��ֵ����귽��0:+,1-
	{0, NULL},
	{0, NULL},
	{3, (TSetValueInf*)&(SetValueInf[4])}, //��ֵͨ������
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
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
	{AT1MODE, "AT����"},
	{AT2MODE, "��ר��ʽ"},
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
};

//��������ֵ����-----------------------
LONG* 	RM_pnU1 	= 	(LONG*)(RM_BUF_ADDR + 0); 	// U1
LONG* 	RM_pnU2 	= 	(LONG*)(RM_BUF_ADDR + 4); 	// U2
LONG* 	RM_pnU3 	= 	(LONG*)(RM_BUF_ADDR + 8); 	// U3
LONG* 	RM_pnU4 	= 	(LONG*)(RM_BUF_ADDR + 12); 	// U4
LONG* 	RM_pnIt1 	= 	(LONG*)(RM_BUF_ADDR + 16); 	// It1
LONG* 	RM_pnIf1 	= 	(LONG*)(RM_BUF_ADDR + 20); 	// If1
LONG* 	RM_pnIt2 	= 	(LONG*)(RM_BUF_ADDR + 24); 	// It2
LONG* 	RM_pnIf2 	= 	(LONG*)(RM_BUF_ADDR + 28); 	// If2
LONG* 	RM_pnIt3 	= 	(LONG*)(RM_BUF_ADDR + 32); 	// It3
LONG* 	RM_pnIf3 	= 	(LONG*)(RM_BUF_ADDR + 36); 	// If3
LONG* 	RM_pnIat1 	= 	(LONG*)(RM_BUF_ADDR + 40); 	// Iat1
LONG* 	RM_pnIat2 	= 	(LONG*)(RM_BUF_ADDR + 44); 	// Iat2
LONG* 	RM_pnSSIAT 	= 	(LONG*)(RM_BUF_ADDR + 48); 	// IAT
LONG* 	RM_pnU1A 	= 	(LONG*)(RM_BUF_ADDR + 52); 	// AU1
LONG* 	RM_pnU2A 	= 	(LONG*)(RM_BUF_ADDR + 56); 	// AU2
LONG* 	RM_pnU3A 	= 	(LONG*)(RM_BUF_ADDR + 60); 	// AU3
LONG* 	RM_pnU4A 	= 	(LONG*)(RM_BUF_ADDR + 64); 	// AU4
LONG* 	RM_pnIT1A 	= 	(LONG*)(RM_BUF_ADDR + 68); 	// AIT1
LONG* 	RM_pnIF1A 	= 	(LONG*)(RM_BUF_ADDR + 72); 	// AIF1
LONG* 	RM_pnIT2A 	= 	(LONG*)(RM_BUF_ADDR + 76); 	// AIT2
LONG* 	RM_pnIF2A 	= 	(LONG*)(RM_BUF_ADDR + 80); 	// AIF2
LONG* 	RM_pnIT3A 	= 	(LONG*)(RM_BUF_ADDR + 84); 	// AIT3
LONG* 	RM_pnIF3A 	= 	(LONG*)(RM_BUF_ADDR + 88); 	// AIF3
LONG* 	RM_pnBOBRCH1 	= 	(LONG*)(RM_BUF_ADDR + 92); 	// BR1
LONG* 	RM_pnBOBRCH2 	= 	(LONG*)(RM_BUF_ADDR + 96); 	// BR2
LONG* 	RM_pnBOINCH1 	= 	(LONG*)(RM_BUF_ADDR + 100); 	// IN1
LONG* 	RM_pnBOINCH2 	= 	(LONG*)(RM_BUF_ADDR + 104); 	// IN2
LONG* 	RM_pnSub1_U1 	= 	(LONG*)(RM_BUF_ADDR + 108); 	// U1
LONG* 	RM_pnSub1_U2 	= 	(LONG*)(RM_BUF_ADDR + 112); 	// U2
LONG* 	RM_pnSub1_U3 	= 	(LONG*)(RM_BUF_ADDR + 116); 	// U3
LONG* 	RM_pnSub1_U4 	= 	(LONG*)(RM_BUF_ADDR + 120); 	// U4
LONG* 	RM_pnSub1_IT1 	= 	(LONG*)(RM_BUF_ADDR + 124); 	// IT1
LONG* 	RM_pnSub1_IF1 	= 	(LONG*)(RM_BUF_ADDR + 128); 	// IF1
LONG* 	RM_pnSub1_IT2 	= 	(LONG*)(RM_BUF_ADDR + 132); 	// IT2
LONG* 	RM_pnSub1_IF2 	= 	(LONG*)(RM_BUF_ADDR + 136); 	// IF2
LONG* 	RM_pnSub1_IT3 	= 	(LONG*)(RM_BUF_ADDR + 140); 	// IT3
LONG* 	RM_pnSub1_IF3 	= 	(LONG*)(RM_BUF_ADDR + 144); 	// IF3
LONG* 	RM_pnSub1_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 148); 	// IAT1
LONG* 	RM_pnSub1_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 152); 	// IAT2
LONG* 	RM_pnSub1_IAT 	= 	(LONG*)(RM_BUF_ADDR + 156); 	// IAT
LONG* 	RM_pnSub1_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 160); 	// AIT1
LONG* 	RM_pnSub1_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 164); 	// AIF1
LONG* 	RM_pnSub1_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 168); 	// AIT2
LONG* 	RM_pnSub1_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 172); 	// AIF2
LONG* 	RM_pnSub1_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 176); 	// AIT3
LONG* 	RM_pnSub1_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 180); 	// AIF3
LONG* 	RM_pnSub1_Status 	= 	(LONG*)(RM_BUF_ADDR + 184); 	// ST
LONG* 	RM_pnSub1_year 	= 	(LONG*)(RM_BUF_ADDR + 188); 	// Year
LONG* 	RM_pnSub1_month 	= 	(LONG*)(RM_BUF_ADDR + 192); 	// Mon.
LONG* 	RM_pnSub1_day 	= 	(LONG*)(RM_BUF_ADDR + 196); 	// Day
LONG* 	RM_pnSub1_hour 	= 	(LONG*)(RM_BUF_ADDR + 200); 	// Hour
LONG* 	RM_pnSub1_minute 	= 	(LONG*)(RM_BUF_ADDR + 204); 	// Min.
LONG* 	RM_pnSub1_NS 	= 	(LONG*)(RM_BUF_ADDR + 208); 	// NS
LONG* 	RM_pnSub2_U1 	= 	(LONG*)(RM_BUF_ADDR + 212); 	// U1
LONG* 	RM_pnSub2_U2 	= 	(LONG*)(RM_BUF_ADDR + 216); 	// U2
LONG* 	RM_pnSub2_U3 	= 	(LONG*)(RM_BUF_ADDR + 220); 	// U3
LONG* 	RM_pnSub2_U4 	= 	(LONG*)(RM_BUF_ADDR + 224); 	// U4
LONG* 	RM_pnSub2_IT1 	= 	(LONG*)(RM_BUF_ADDR + 228); 	// IT1
LONG* 	RM_pnSub2_IF1 	= 	(LONG*)(RM_BUF_ADDR + 232); 	// IF1
LONG* 	RM_pnSub2_IT2 	= 	(LONG*)(RM_BUF_ADDR + 236); 	// IT2
LONG* 	RM_pnSub2_IF2 	= 	(LONG*)(RM_BUF_ADDR + 240); 	// IF2
LONG* 	RM_pnSub2_IT3 	= 	(LONG*)(RM_BUF_ADDR + 244); 	// IT3
LONG* 	RM_pnSub2_IF3 	= 	(LONG*)(RM_BUF_ADDR + 248); 	// IF3
LONG* 	RM_pnSub2_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 252); 	// IAT1
LONG* 	RM_pnSub2_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 256); 	// IAT2
LONG* 	RM_pnSub2_IAT 	= 	(LONG*)(RM_BUF_ADDR + 260); 	// IAT
LONG* 	RM_pnSub2_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 264); 	// AIT1
LONG* 	RM_pnSub2_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 268); 	// AIF1
LONG* 	RM_pnSub2_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 272); 	// AIT2
LONG* 	RM_pnSub2_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 276); 	// AIF2
LONG* 	RM_pnSub2_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 280); 	// AIT3
LONG* 	RM_pnSub2_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 284); 	// AIF3
LONG* 	RM_pnSub2_Status 	= 	(LONG*)(RM_BUF_ADDR + 288); 	// ST
LONG* 	RM_pnSub2_year 	= 	(LONG*)(RM_BUF_ADDR + 292); 	// Year
LONG* 	RM_pnSub2_month 	= 	(LONG*)(RM_BUF_ADDR + 296); 	// Mon.
LONG* 	RM_pnSub2_day 	= 	(LONG*)(RM_BUF_ADDR + 300); 	// Day
LONG* 	RM_pnSub2_hour 	= 	(LONG*)(RM_BUF_ADDR + 304); 	// Hour
LONG* 	RM_pnSub2_minute 	= 	(LONG*)(RM_BUF_ADDR + 308); 	// Min.
LONG* 	RM_pnSub2_NS 	= 	(LONG*)(RM_BUF_ADDR + 312); 	// NS
LONG* 	RM_pnSub3_U1 	= 	(LONG*)(RM_BUF_ADDR + 316); 	// U1
LONG* 	RM_pnSub3_U2 	= 	(LONG*)(RM_BUF_ADDR + 320); 	// U2
LONG* 	RM_pnSub3_U3 	= 	(LONG*)(RM_BUF_ADDR + 324); 	// U3
LONG* 	RM_pnSub3_U4 	= 	(LONG*)(RM_BUF_ADDR + 328); 	// U4
LONG* 	RM_pnSub3_IT1 	= 	(LONG*)(RM_BUF_ADDR + 332); 	// IT1
LONG* 	RM_pnSub3_IF1 	= 	(LONG*)(RM_BUF_ADDR + 336); 	// IF1
LONG* 	RM_pnSub3_IT2 	= 	(LONG*)(RM_BUF_ADDR + 340); 	// IT2
LONG* 	RM_pnSub3_IF2 	= 	(LONG*)(RM_BUF_ADDR + 344); 	// IF2
LONG* 	RM_pnSub3_IT3 	= 	(LONG*)(RM_BUF_ADDR + 348); 	// IT3
LONG* 	RM_pnSub3_IF3 	= 	(LONG*)(RM_BUF_ADDR + 352); 	// IF3
LONG* 	RM_pnSub3_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 356); 	// IAT1
LONG* 	RM_pnSub3_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 360); 	// IAT2
LONG* 	RM_pnSub3_IAT 	= 	(LONG*)(RM_BUF_ADDR + 364); 	// IAT
LONG* 	RM_pnSub3_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 368); 	// AIT1
LONG* 	RM_pnSub3_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 372); 	// AIF1
LONG* 	RM_pnSub3_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 376); 	// AIT2
LONG* 	RM_pnSub3_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 380); 	// AIF2
LONG* 	RM_pnSub3_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 384); 	// AIT3
LONG* 	RM_pnSub3_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 388); 	// AIF3
LONG* 	RM_pnSub3_Status 	= 	(LONG*)(RM_BUF_ADDR + 392); 	// ST
LONG* 	RM_pnSub3_year 	= 	(LONG*)(RM_BUF_ADDR + 396); 	// Year
LONG* 	RM_pnSub3_month 	= 	(LONG*)(RM_BUF_ADDR + 400); 	// Mon.
LONG* 	RM_pnSub3_day 	= 	(LONG*)(RM_BUF_ADDR + 404); 	// Day
LONG* 	RM_pnSub3_hour 	= 	(LONG*)(RM_BUF_ADDR + 408); 	// Hour
LONG* 	RM_pnSub3_minute 	= 	(LONG*)(RM_BUF_ADDR + 412); 	// Min.
LONG* 	RM_pnSub3_NS 	= 	(LONG*)(RM_BUF_ADDR + 416); 	// NS
LONG* 	RM_pnSub4_U1 	= 	(LONG*)(RM_BUF_ADDR + 420); 	// U1
LONG* 	RM_pnSub4_U2 	= 	(LONG*)(RM_BUF_ADDR + 424); 	// U2
LONG* 	RM_pnSub4_U3 	= 	(LONG*)(RM_BUF_ADDR + 428); 	// U3
LONG* 	RM_pnSub4_U4 	= 	(LONG*)(RM_BUF_ADDR + 432); 	// U4
LONG* 	RM_pnSub4_IT1 	= 	(LONG*)(RM_BUF_ADDR + 436); 	// IT1
LONG* 	RM_pnSub4_IF1 	= 	(LONG*)(RM_BUF_ADDR + 440); 	// IF1
LONG* 	RM_pnSub4_IT2 	= 	(LONG*)(RM_BUF_ADDR + 444); 	// IT2
LONG* 	RM_pnSub4_IF2 	= 	(LONG*)(RM_BUF_ADDR + 448); 	// IF2
LONG* 	RM_pnSub4_IT3 	= 	(LONG*)(RM_BUF_ADDR + 452); 	// IT3
LONG* 	RM_pnSub4_IF3 	= 	(LONG*)(RM_BUF_ADDR + 456); 	// IF3
LONG* 	RM_pnSub4_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 460); 	// IAT1
LONG* 	RM_pnSub4_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 464); 	// IAT2
LONG* 	RM_pnSub4_IAT 	= 	(LONG*)(RM_BUF_ADDR + 468); 	// IAT
LONG* 	RM_pnSub4_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 472); 	// AIT1
LONG* 	RM_pnSub4_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 476); 	// AIF1
LONG* 	RM_pnSub4_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 480); 	// AIT2
LONG* 	RM_pnSub4_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 484); 	// AIF2
LONG* 	RM_pnSub4_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 488); 	// AIT3
LONG* 	RM_pnSub4_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 492); 	// AIF3
LONG* 	RM_pnSub4_Status 	= 	(LONG*)(RM_BUF_ADDR + 496); 	// ST
LONG* 	RM_pnSub4_year 	= 	(LONG*)(RM_BUF_ADDR + 500); 	// Year
LONG* 	RM_pnSub4_month 	= 	(LONG*)(RM_BUF_ADDR + 504); 	// Mon.
LONG* 	RM_pnSub4_day 	= 	(LONG*)(RM_BUF_ADDR + 508); 	// Day
LONG* 	RM_pnSub4_hour 	= 	(LONG*)(RM_BUF_ADDR + 512); 	// Hour
LONG* 	RM_pnSub4_minute 	= 	(LONG*)(RM_BUF_ADDR + 516); 	// Min.
LONG* 	RM_pnSub4_NS 	= 	(LONG*)(RM_BUF_ADDR + 520); 	// NS
LONG* 	RM_pnSub5_U1 	= 	(LONG*)(RM_BUF_ADDR + 524); 	// U1
LONG* 	RM_pnSub5_U2 	= 	(LONG*)(RM_BUF_ADDR + 528); 	// U2
LONG* 	RM_pnSub5_U3 	= 	(LONG*)(RM_BUF_ADDR + 532); 	// U3
LONG* 	RM_pnSub5_U4 	= 	(LONG*)(RM_BUF_ADDR + 536); 	// U4
LONG* 	RM_pnSub5_IT1 	= 	(LONG*)(RM_BUF_ADDR + 540); 	// IT1
LONG* 	RM_pnSub5_IF1 	= 	(LONG*)(RM_BUF_ADDR + 544); 	// IF1
LONG* 	RM_pnSub5_IT2 	= 	(LONG*)(RM_BUF_ADDR + 548); 	// IT2
LONG* 	RM_pnSub5_IF2 	= 	(LONG*)(RM_BUF_ADDR + 552); 	// IF2
LONG* 	RM_pnSub5_IT3 	= 	(LONG*)(RM_BUF_ADDR + 556); 	// IT3
LONG* 	RM_pnSub5_IF3 	= 	(LONG*)(RM_BUF_ADDR + 560); 	// IF3
LONG* 	RM_pnSub5_IAT1 	= 	(LONG*)(RM_BUF_ADDR + 564); 	// IAT1
LONG* 	RM_pnSub5_IAT2 	= 	(LONG*)(RM_BUF_ADDR + 568); 	// IAT2
LONG* 	RM_pnSub5_IAT 	= 	(LONG*)(RM_BUF_ADDR + 572); 	// IAT
LONG* 	RM_pnSub5_AIT1 	= 	(LONG*)(RM_BUF_ADDR + 576); 	// AIT1
LONG* 	RM_pnSub5_AIF1 	= 	(LONG*)(RM_BUF_ADDR + 580); 	// AIF1
LONG* 	RM_pnSub5_AIT2 	= 	(LONG*)(RM_BUF_ADDR + 584); 	// AIT2
LONG* 	RM_pnSub5_AIF2 	= 	(LONG*)(RM_BUF_ADDR + 588); 	// AIF2
LONG* 	RM_pnSub5_AIT3 	= 	(LONG*)(RM_BUF_ADDR + 592); 	// AIT3
LONG* 	RM_pnSub5_AIF3 	= 	(LONG*)(RM_BUF_ADDR + 596); 	// AIF3
LONG* 	RM_pnSub5_Status 	= 	(LONG*)(RM_BUF_ADDR + 600); 	// ST
LONG* 	RM_pnSub5_year 	= 	(LONG*)(RM_BUF_ADDR + 604); 	// Year
LONG* 	RM_pnSub5_month 	= 	(LONG*)(RM_BUF_ADDR + 608); 	// Mon.
LONG* 	RM_pnSub5_day 	= 	(LONG*)(RM_BUF_ADDR + 612); 	// Day
LONG* 	RM_pnSub5_hour 	= 	(LONG*)(RM_BUF_ADDR + 616); 	// Hour
LONG* 	RM_pnSub5_minute 	= 	(LONG*)(RM_BUF_ADDR + 620); 	// Min.
LONG* 	RM_pnSub5_NS 	= 	(LONG*)(RM_BUF_ADDR + 624); 	// NS
LONG* 	RM_pnDist 	= 	(LONG*)(RM_BUF_ADDR + 628); 	// D
LONG* 	RM_pnKMS 	= 	(LONG*)(RM_BUF_ADDR + 632); 	// KMS
LONG* 	RM_pnR_T 	= 	(LONG*)(RM_BUF_ADDR + 636); 	// R_T
LONG* 	RM_pnX_T 	= 	(LONG*)(RM_BUF_ADDR + 640); 	// X_T
LONG* 	RM_pnR_F 	= 	(LONG*)(RM_BUF_ADDR + 644); 	// R_F
LONG* 	RM_pnX_F 	= 	(LONG*)(RM_BUF_ADDR + 648); 	// X_F
LONG* 	RM_pnR_TF 	= 	(LONG*)(RM_BUF_ADDR + 652); 	// R_TF
LONG* 	RM_pnX_TF 	= 	(LONG*)(RM_BUF_ADDR + 656); 	// X_TF
LONG* 	RM_pnNS 	= 	(LONG*)(RM_BUF_ADDR + 660); 	// NS
LONG* 	RM_pnIP_H2 	= 	(LONG*)(RM_BUF_ADDR + 664); 	// IPh2
LONG* 	RM_pnIP_H3 	= 	(LONG*)(RM_BUF_ADDR + 668); 	// IPh3
LONG* 	RM_pnIP_H5 	= 	(LONG*)(RM_BUF_ADDR + 672); 	// IPh5
LONG* 	RM_pnUp 	= 	(LONG*)(RM_BUF_ADDR + 676); 	// Up
LONG* 	RM_pnI1 	= 	(LONG*)(RM_BUF_ADDR + 680); 	// I1
LONG* 	RM_pnI2 	= 	(LONG*)(RM_BUF_ADDR + 684); 	// I2
LONG* 	RM_pnI3 	= 	(LONG*)(RM_BUF_ADDR + 688); 	// I3


//����ֵ����-----------------------


//��ѹ��----------------------------------
BYTE* SW_pbyFL 	= 	(BYTE*)(SW_BUF_ADDR + 0); 	//���ϲ��
BYTE* SW_pbyMAINT 	= 	(BYTE*)(SW_BUF_ADDR + 1); 	//����ѹ��
BYTE* SW_pbyRESD1 	= 	(BYTE*)(SW_BUF_ADDR + 2); 	//����ѹ��1
BYTE* SW_pbyRESD2 	= 	(BYTE*)(SW_BUF_ADDR + 3); 	//����ѹ��2

//����----------------------------------
BYTE* CFG_pby21_PI 	= 	(BYTE*)(CFG_BUF_ADDR + 0); 	//�迹����
BYTE* CFG_pby50_PI 	= 	(BYTE*)(CFG_BUF_ADDR + 1); 	//��������
BYTE* CFG_pby50DI_PI 	= 	(BYTE*)(CFG_BUF_ADDR + 2); 	//��������
BYTE* CFG_pbyFL_AUTO 	= 	(BYTE*)(CFG_BUF_ADDR + 3); 	//�������Ӧ
BYTE* CFG_pbyCCMODE 	= 	(BYTE*)(CFG_BUF_ADDR + 4); 	//�����ϳ�
BYTE* CFG_pbyAT_C_RATE 	= 	(BYTE*)(CFG_BUF_ADDR + 5); 	//���ϵ�����
BYTE* CFG_pbyUD_C_RATE 	= 	(BYTE*)(CFG_BUF_ADDR + 6); 	//�����е�����
BYTE* CFG_pbyNL_C_RATE 	= 	(BYTE*)(CFG_BUF_ADDR + 7); 	//����������
BYTE* CFG_pbyPHASESC 	= 	(BYTE*)(CFG_BUF_ADDR + 8); 	//����·�ж�
BYTE* CFG_pbyNONEAT 	= 	(BYTE*)(CFG_BUF_ADDR + 9); 	//�������AT
BYTE* CFG_pbyISSS 	= 	(BYTE*)(CFG_BUF_ADDR + 10); 	//����Ϊ�����
BYTE* CFG_pbyPARALLG 	= 	(BYTE*)(CFG_BUF_ADDR + 11); 	//ƽ���ı���
BYTE* CFG_pby21_H_S 	= 	(BYTE*)(CFG_BUF_ADDR + 12); 	//����г������
BYTE* CFG_pby21_H_L 	= 	(BYTE*)(CFG_BUF_ADDR + 13); 	//����г������
BYTE* CFG_pby50_H_S 	= 	(BYTE*)(CFG_BUF_ADDR + 14); 	//����г������
BYTE* CFG_pby50_H_L 	= 	(BYTE*)(CFG_BUF_ADDR + 15); 	//����г������
BYTE* CFG_pby50_27V 	= 	(BYTE*)(CFG_BUF_ADDR + 16); 	//����������ѹ����
BYTE* CFG_pbyCALLDATA 	= 	(BYTE*)(CFG_BUF_ADDR + 17); 	//�ٲ�ȡ��ѹ����
BYTE* CFG_pbyCHECKGPS 	= 	(BYTE*)(CFG_BUF_ADDR + 18); 	//GPS���ϼ��
BYTE* CFG_pbyCHECKCOM 	= 	(BYTE*)(CFG_BUF_ADDR + 19); 	//ͨ���Զ����
BYTE* CFG_pbyCROSSAERA 	= 	(BYTE*)(CFG_BUF_ADDR + 20); 	//����Խ��
BYTE* CFG_pbyRESD1 	= 	(BYTE*)(CFG_BUF_ADDR + 21); 	//��������1
BYTE* CFG_pbyRESD2 	= 	(BYTE*)(CFG_BUF_ADDR + 22); 	//��������2

//��ֵ----------------------------------
LONG* RS_pnCFG 	= 	(LONG*)(RS_BUF_ADDR + 0); 	//������ CFG
LONG* RS_pnPIC 	= 	(LONG*)(RS_BUF_ADDR + 4); 	//ͻ������������ PIC
LONG* RS_pn21R 	= 	(LONG*)(RS_BUF_ADDR + 8); 	//������������ 21R
LONG* RS_pn21X 	= 	(LONG*)(RS_BUF_ADDR + 12); 	//���������翹 21X
LONG* RS_pn78LA 	= 	(LONG*)(RS_BUF_ADDR + 16); 	//��·�迹�� 78LA
LONG* RS_pn50C 	= 	(LONG*)(RS_BUF_ADDR + 20); 	//������ֵ 50C
LONG* RS_pn50_27V 	= 	(LONG*)(RS_BUF_ADDR + 24); 	//����������ѹ������ֵ 50_27V
LONG* RS_pn50DC 	= 	(LONG*)(RS_BUF_ADDR + 28); 	//������������������ֵ 50DC
LONG* RS_pn50D_37C 	= 	(LONG*)(RS_BUF_ADDR + 32); 	//��������С�������� 50D_37C
LONG* RS_pnFLT 	= 	(LONG*)(RS_BUF_ADDR + 36); 	//�ʱ�������ʱ FLT
LONG* RS_pnKh1 	= 	(LONG*)(RS_BUF_ADDR + 40); 	//����г������ϵ�� Kh1
LONG* RS_pnKhr1 	= 	(LONG*)(RS_BUF_ADDR + 44); 	//�������г����Ȩ Khr1
LONG* RS_pnKhr2 	= 	(LONG*)(RS_BUF_ADDR + 48); 	//��������г����Ȩ Khr2
LONG* RS_pn27Uab 	= 	(LONG*)(RS_BUF_ADDR + 52); 	//����·��ѹ��ֵ 27Uab
LONG* RS_pn27U 	= 	(LONG*)(RS_BUF_ADDR + 56); 	//��ѹ��¼���ݶ�ֵ 27U
LONG* RS_pn27D 	= 	(LONG*)(RS_BUF_ADDR + 60); 	//��ѹ���ݼ�¼��ʱ 27D
LONG* RS_pnN 	= 	(LONG*)(RS_BUF_ADDR + 64); 	//�������� N
LONG* RS_pnNID 	= 	(LONG*)(RS_BUF_ADDR + 68); 	//����վ�� NID
LONG* RS_pnD1 	= 	(LONG*)(RS_BUF_ADDR + 72); 	//AT����1���� D1
LONG* RS_pnD2 	= 	(LONG*)(RS_BUF_ADDR + 76); 	//AT����2���� D2
LONG* RS_pnD3 	= 	(LONG*)(RS_BUF_ADDR + 80); 	//AT����3���� D3
LONG* RS_pnD4 	= 	(LONG*)(RS_BUF_ADDR + 84); 	//AT����4���� D4
LONG* RS_pnD5 	= 	(LONG*)(RS_BUF_ADDR + 88); 	//AT����5���� D5
LONG* RS_pnQss 	= 	(LONG*)(RS_BUF_ADDR + 92); 	//�����AT©�� Qss
LONG* RS_pnQ1 	= 	(LONG*)(RS_BUF_ADDR + 96); 	//����1AT©�� Q1
LONG* RS_pnQ2 	= 	(LONG*)(RS_BUF_ADDR + 100); 	//����2AT©�� Q2
LONG* RS_pnQ3 	= 	(LONG*)(RS_BUF_ADDR + 104); 	//����3AT©�� Q3
LONG* RS_pnQ4 	= 	(LONG*)(RS_BUF_ADDR + 108); 	//����4AT©�� Q4
LONG* RS_pnQ5 	= 	(LONG*)(RS_BUF_ADDR + 112); 	//����5AT©�� Q5
LONG* RS_pnKss 	= 	(LONG*)(RS_BUF_ADDR + 116); 	//����������ֲ�ϵ�� Kss
LONG* RS_pnK1 	= 	(LONG*)(RS_BUF_ADDR + 120); 	//����1AT�����ֲ�ϵ�� K1
LONG* RS_pnK2 	= 	(LONG*)(RS_BUF_ADDR + 124); 	//����2AT�����ֲ�ϵ�� K2
LONG* RS_pnK3 	= 	(LONG*)(RS_BUF_ADDR + 128); 	//����3AT�����ֲ�ϵ�� K3
LONG* RS_pnK4 	= 	(LONG*)(RS_BUF_ADDR + 132); 	//����4AT�����ֲ�ϵ�� K4
LONG* RS_pnK5 	= 	(LONG*)(RS_BUF_ADDR + 136); 	//����5AT�����ֲ�ϵ�� K5
LONG* RS_pnKF 	= 	(LONG*)(RS_BUF_ADDR + 140); 	//�����ж�ϵ�� KF
LONG* RS_pnLa 	= 	(LONG*)(RS_BUF_ADDR + 144); 	//����۳��� La
LONG* RS_pnLc 	= 	(LONG*)(RS_BUF_ADDR + 148); 	//�������� Lc
LONG* RS_pnLD 	= 	(LONG*)(RS_BUF_ADDR + 152); 	//��������0:+,1- LD
LONG* RS_pnS0 	= 	(LONG*)(RS_BUF_ADDR + 156); 	//���������� S0
LONG* RS_pnSD 	= 	(LONG*)(RS_BUF_ADDR + 160); 	//����귽��0:+,1- SD
LONG* RS_pnZOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 164); 	//ƽ���ı���ƫ���迹 ZOFFSET
LONG* RS_pnAOFFSET 	= 	(LONG*)(RS_BUF_ADDR + 168); 	//�迹�ı���ƫ�ƽǶ� AOFFSET
LONG* RS_pnChanType 	= 	(LONG*)(RS_BUF_ADDR + 172); 	//ͨ������ ChanType
LONG* RS_pnCAID 	= 	(LONG*)(RS_BUF_ADDR + 176); 	//Խ��ȫ�ֱ�� CAID
LONG* RS_pnT21FLN 	= 	(LONG*)(RS_BUF_ADDR + 180); 	//T�߲��ֶ��� T21FLN
LONG* RS_pnT21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 184); 	//T�ߵ�һ�ξ��� T21FLN1L
LONG* RS_pnT21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 188); 	//T�ߵ�һ�ε�λ�翹 T21FLN1X
LONG* RS_pnT21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 192); 	//T�ߵڶ��ξ��� T21FLN2L
LONG* RS_pnT21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 196); 	//T�ߵڶ��ε�λ�翹 T21FLN2X
LONG* RS_pnT21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 200); 	//T�ߵ����ξ��� T21FLN3L
LONG* RS_pnT21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 204); 	//T�ߵ����ε�λ�翹 T21FLN3X
LONG* RS_pnT21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 208); 	//T�ߵ��Ķξ��� T21FLN4L
LONG* RS_pnT21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 212); 	//T�ߵ��Ķε�λ�翹 T21FLN4X
LONG* RS_pnT21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 216); 	//T�ߵ���ξ��� T21FLN5L
LONG* RS_pnT21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 220); 	//T�ߵ���ε�λ�翹 T21FLN5X
LONG* RS_pnT21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 224); 	//T�ߵ����ξ��� T21FLN6L
LONG* RS_pnT21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 228); 	//T�ߵ����ε�λ�翹 T21FLN6X
LONG* RS_pnF21FLN 	= 	(LONG*)(RS_BUF_ADDR + 232); 	//F�߲��ֶ��� F21FLN
LONG* RS_pnF21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 236); 	//F�ߵ�һ�ξ��� F21FLN1L
LONG* RS_pnF21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 240); 	//F�ߵ�һ�ε�λ�翹 F21FLN1X
LONG* RS_pnF21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 244); 	//F�ߵڶ��ξ��� F21FLN2L
LONG* RS_pnF21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 248); 	//F�ߵڶ��ε�λ�翹 F21FLN2X
LONG* RS_pnF21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 252); 	//F�ߵ����ξ��� F21FLN3L
LONG* RS_pnF21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 256); 	//F�ߵ����ε�λ�翹 F21FLN3X
LONG* RS_pnF21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 260); 	//F�ߵ��Ķξ��� F21FLN4L
LONG* RS_pnF21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 264); 	//F�ߵ��Ķε�λ�翹 F21FLN4X
LONG* RS_pnF21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 268); 	//F�ߵ���ξ��� F21FLN5L
LONG* RS_pnF21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 272); 	//F�ߵ���ε�λ�翹 F21FLN5X
LONG* RS_pnF21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 276); 	//F�ߵ����ξ��� F21FLN6L
LONG* RS_pnF21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 280); 	//F�ߵ����ε�λ�翹 F21FLN6X
LONG* RS_pnTF21FLN 	= 	(LONG*)(RS_BUF_ADDR + 284); 	//TF���ֶ��� TF21FLN
LONG* RS_pnTF21FLN1L 	= 	(LONG*)(RS_BUF_ADDR + 288); 	//TF����һ�ξ��� TF21FLN1L
LONG* RS_pnTF21FLN1X 	= 	(LONG*)(RS_BUF_ADDR + 292); 	//TF���һ�ε�λ�翹 TF21FLN1X
LONG* RS_pnTF21FLN2L 	= 	(LONG*)(RS_BUF_ADDR + 296); 	//TF���ڶ��ξ��� TF21FLN2L
LONG* RS_pnTF21FLN2X 	= 	(LONG*)(RS_BUF_ADDR + 300); 	//TF�����ε�λ�翹 TF21FLN2X
LONG* RS_pnTF21FLN3L 	= 	(LONG*)(RS_BUF_ADDR + 304); 	//TF�������ξ��� TF21FLN3L
LONG* RS_pnTF21FLN3X 	= 	(LONG*)(RS_BUF_ADDR + 308); 	//TF������ε�λ�翹 TF21FLN3X
LONG* RS_pnTF21FLN4L 	= 	(LONG*)(RS_BUF_ADDR + 312); 	//TF�����Ķξ��� TF21FLN4L
LONG* RS_pnTF21FLN4X 	= 	(LONG*)(RS_BUF_ADDR + 316); 	//TF����Ķε�λ�翹 TF21FLN4X
LONG* RS_pnTF21FLN5L 	= 	(LONG*)(RS_BUF_ADDR + 320); 	//TF������ξ��� TF21FLN5L
LONG* RS_pnTF21FLN5X 	= 	(LONG*)(RS_BUF_ADDR + 324); 	//TF�����ε�λ�翹 TF21FLN5X
LONG* RS_pnTF21FLN6L 	= 	(LONG*)(RS_BUF_ADDR + 328); 	//TF�������ξ��� TF21FLN6L
LONG* RS_pnTF21FLN6X 	= 	(LONG*)(RS_BUF_ADDR + 332); 	//TF������ε�λ�翹 TF21FLN6X

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
LONG* AS_pn50I1 	= 	(LONG*)(AS_BUF_ADDR + 152); 	//I1������ֵ
LONG* AS_pn37I1 	= 	(LONG*)(AS_BUF_ADDR + 156); 	//I1������ֵ
LONG* AS_pn50I2 	= 	(LONG*)(AS_BUF_ADDR + 160); 	//I2������ֵ
LONG* AS_pn37I2 	= 	(LONG*)(AS_BUF_ADDR + 164); 	//I2������ֵ
LONG* AS_pn50I3 	= 	(LONG*)(AS_BUF_ADDR + 168); 	//I3������ֵ
LONG* AS_pn37I3 	= 	(LONG*)(AS_BUF_ADDR + 172); 	//I3������ֵ
LONG* AS_pn59U1 	= 	(LONG*)(AS_BUF_ADDR + 176); 	//U1��ѹ��ֵ
LONG* AS_pn27U1 	= 	(LONG*)(AS_BUF_ADDR + 180); 	//U1��ѹ��ֵ
LONG* AS_pn59U2 	= 	(LONG*)(AS_BUF_ADDR + 184); 	//U2��ѹ��ֵ
LONG* AS_pn27U2 	= 	(LONG*)(AS_BUF_ADDR + 188); 	//U2��ѹ��ֵ
LONG* AS_pn59U3 	= 	(LONG*)(AS_BUF_ADDR + 192); 	//U3��ѹ��ֵ
LONG* AS_pn27U3 	= 	(LONG*)(AS_BUF_ADDR + 196); 	//U3��ѹ��ֵ
LONG* AS_pn59U4 	= 	(LONG*)(AS_BUF_ADDR + 200); 	//U4��ѹ��ֵ
LONG* AS_pn27U4 	= 	(LONG*)(AS_BUF_ADDR + 204); 	//U4��ѹ��ֵ
LONG* AS_pnRECD 	= 	(LONG*)(AS_BUF_ADDR + 208); 	//����¼������ʱ��

//���----------------------------------
LONG* AS_pnUPTRatio 	= 	(LONG*)(AS_BUF_ADDR + 212); 	//U���
LONG* AS_pnI1CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 216); 	//I1���
LONG* AS_pnI2CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 220); 	//I2���
LONG* AS_pnI3CTRatio 	= 	(LONG*)(AS_BUF_ADDR + 224); 	//I3���
LONG* AS_pnIATCTRatio 	= 	(LONG*)(AS_BUF_ADDR + 228); 	//IAT���
LONG* AS_pnZRatio 	= 	(LONG*)(AS_BUF_ADDR + 232); 	//�迹���
LONG* AS_pnRT 	= 	(LONG*)(AS_BUF_ADDR + 236); 	//RT

//ϵ��----------------------------------
LONG* CF_pnU1 	= 	(LONG*)(CF_BUF_ADDR + 0); 	//U1
LONG* CF_pnU2 	= 	(LONG*)(CF_BUF_ADDR + 4); 	//U2
LONG* CF_pnU3 	= 	(LONG*)(CF_BUF_ADDR + 8); 	//U3
LONG* CF_pnU4 	= 	(LONG*)(CF_BUF_ADDR + 12); 	//U4
LONG* CF_pnIT1 	= 	(LONG*)(CF_BUF_ADDR + 16); 	//IT1
LONG* CF_pnIF1 	= 	(LONG*)(CF_BUF_ADDR + 20); 	//IF1
LONG* CF_pnIT2 	= 	(LONG*)(CF_BUF_ADDR + 24); 	//IT2
LONG* CF_pnIF2 	= 	(LONG*)(CF_BUF_ADDR + 28); 	//IF2
LONG* CF_pnIT3 	= 	(LONG*)(CF_BUF_ADDR + 32); 	//IT3
LONG* CF_pnIF3 	= 	(LONG*)(CF_BUF_ADDR + 36); 	//IF3
LONG* CF_pnIAT1 	= 	(LONG*)(CF_BUF_ADDR + 40); 	//IAT1
LONG* CF_pnIAT2 	= 	(LONG*)(CF_BUF_ADDR + 44); 	//IAT2
LONG* CF_pnZ 	= 	(LONG*)(CF_BUF_ADDR + 48); 	//Z
LONG* CF_pnC1 	= 	(LONG*)(CF_BUF_ADDR + 52); 	//C1
LONG* CF_pnC2 	= 	(LONG*)(CF_BUF_ADDR + 56); 	//C2
LONG* CF_pnC3 	= 	(LONG*)(CF_BUF_ADDR + 60); 	//C3
LONG* CF_pnC4 	= 	(LONG*)(CF_BUF_ADDR + 64); 	//C4

//���ϵ������ö���------------------------------------
//�������ʧЧ1----------
const  LONG* FailureInf1[] = {
    (LONG*)(RM_BUF_ADDR + 100), // IN1
    (LONG*)(RM_BUF_ADDR + 104) // IN2
};

//�����������2----------
const  LONG* FailureInf2[] = {
    (LONG*)(RM_BUF_ADDR + 92), // BR1
    (LONG*)(RM_BUF_ADDR + 96) // BR2
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

//���������쳣�ָ�65----------
const  LONG* FailureInf65[] = {
    {NULL}
};

//��������500----------
const  LONG* FailureInf500[] = {
    {NULL}
};

//���ϲ������2400----------
const  LONG* FailureInf2400[] = {
    {NULL}
};

//����T-R����2401----------
const  LONG* FailureInf2401[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����F-R����2402----------
const  LONG* FailureInf2402[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-R����2427----------
const  LONG* FailureInf2427[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����F-R����2428----------
const  LONG* FailureInf2428[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//T-F����2403----------
const  LONG* FailureInf2403[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����1GPS����2404----------
const  LONG* FailureInf2404[] = {
    (LONG*)(RM_BUF_ADDR + 108), // U1
    (LONG*)(RM_BUF_ADDR + 112), // U2
    (LONG*)(RM_BUF_ADDR + 124), // IT1
    (LONG*)(RM_BUF_ADDR + 128), // IF1
    (LONG*)(RM_BUF_ADDR + 132), // IT2
    (LONG*)(RM_BUF_ADDR + 136), // IF2
    (LONG*)(RM_BUF_ADDR + 140), // IT3
    (LONG*)(RM_BUF_ADDR + 144), // IF3
    (LONG*)(RM_BUF_ADDR + 148), // IAT1
    (LONG*)(RM_BUF_ADDR + 152), // IAT2
    (LONG*)(RM_BUF_ADDR + 156), // IAT
    (LONG*)(RM_BUF_ADDR + 160), // AIT1
    (LONG*)(RM_BUF_ADDR + 164), // AIF1
    (LONG*)(RM_BUF_ADDR + 168), // AIT2
    (LONG*)(RM_BUF_ADDR + 172), // AIF2
    (LONG*)(RM_BUF_ADDR + 176), // AIT3
    (LONG*)(RM_BUF_ADDR + 180), // AIF3
    (LONG*)(RM_BUF_ADDR + 184) // ST
};

//����2GPS����2405----------
const  LONG* FailureInf2405[] = {
    (LONG*)(RM_BUF_ADDR + 212), // U1
    (LONG*)(RM_BUF_ADDR + 216), // U2
    (LONG*)(RM_BUF_ADDR + 228), // IT1
    (LONG*)(RM_BUF_ADDR + 232), // IF1
    (LONG*)(RM_BUF_ADDR + 236), // IT2
    (LONG*)(RM_BUF_ADDR + 240), // IF2
    (LONG*)(RM_BUF_ADDR + 244), // IT3
    (LONG*)(RM_BUF_ADDR + 248), // IF3
    (LONG*)(RM_BUF_ADDR + 252), // IAT1
    (LONG*)(RM_BUF_ADDR + 256), // IAT2
    (LONG*)(RM_BUF_ADDR + 260), // IAT
    (LONG*)(RM_BUF_ADDR + 264), // AIT1
    (LONG*)(RM_BUF_ADDR + 268), // AIF1
    (LONG*)(RM_BUF_ADDR + 272), // AIT2
    (LONG*)(RM_BUF_ADDR + 276), // AIF2
    (LONG*)(RM_BUF_ADDR + 280), // AIT3
    (LONG*)(RM_BUF_ADDR + 284), // AIF3
    (LONG*)(RM_BUF_ADDR + 288) // ST
};

//����3GPS����2406----------
const  LONG* FailureInf2406[] = {
    (LONG*)(RM_BUF_ADDR + 316), // U1
    (LONG*)(RM_BUF_ADDR + 320), // U2
    (LONG*)(RM_BUF_ADDR + 332), // IT1
    (LONG*)(RM_BUF_ADDR + 336), // IF1
    (LONG*)(RM_BUF_ADDR + 340), // IT2
    (LONG*)(RM_BUF_ADDR + 344), // IF2
    (LONG*)(RM_BUF_ADDR + 348), // IT3
    (LONG*)(RM_BUF_ADDR + 352), // IF3
    (LONG*)(RM_BUF_ADDR + 356), // IAT1
    (LONG*)(RM_BUF_ADDR + 360), // IAT2
    (LONG*)(RM_BUF_ADDR + 364), // IAT
    (LONG*)(RM_BUF_ADDR + 368), // AIT1
    (LONG*)(RM_BUF_ADDR + 372), // AIF1
    (LONG*)(RM_BUF_ADDR + 376), // AIT2
    (LONG*)(RM_BUF_ADDR + 380), // AIF2
    (LONG*)(RM_BUF_ADDR + 384), // AIT3
    (LONG*)(RM_BUF_ADDR + 388), // AIF3
    (LONG*)(RM_BUF_ADDR + 392) // ST
};

//����4GPS����2407----------
const  LONG* FailureInf2407[] = {
    (LONG*)(RM_BUF_ADDR + 420), // U1
    (LONG*)(RM_BUF_ADDR + 424), // U2
    (LONG*)(RM_BUF_ADDR + 436), // IT1
    (LONG*)(RM_BUF_ADDR + 440), // IF1
    (LONG*)(RM_BUF_ADDR + 444), // IT2
    (LONG*)(RM_BUF_ADDR + 448), // IF2
    (LONG*)(RM_BUF_ADDR + 452), // IT3
    (LONG*)(RM_BUF_ADDR + 456), // IF3
    (LONG*)(RM_BUF_ADDR + 460), // IAT1
    (LONG*)(RM_BUF_ADDR + 464), // IAT2
    (LONG*)(RM_BUF_ADDR + 468), // IAT
    (LONG*)(RM_BUF_ADDR + 472), // AIT1
    (LONG*)(RM_BUF_ADDR + 476), // AIF1
    (LONG*)(RM_BUF_ADDR + 480), // AIT2
    (LONG*)(RM_BUF_ADDR + 484), // AIF2
    (LONG*)(RM_BUF_ADDR + 488), // AIT3
    (LONG*)(RM_BUF_ADDR + 492), // AIF3
    (LONG*)(RM_BUF_ADDR + 496) // ST
};

//����5GPS����2453----------
const  LONG* FailureInf2453[] = {
    (LONG*)(RM_BUF_ADDR + 524), // U1
    (LONG*)(RM_BUF_ADDR + 528), // U2
    (LONG*)(RM_BUF_ADDR + 540), // IT1
    (LONG*)(RM_BUF_ADDR + 544), // IF1
    (LONG*)(RM_BUF_ADDR + 548), // IT2
    (LONG*)(RM_BUF_ADDR + 552), // IF2
    (LONG*)(RM_BUF_ADDR + 556), // IT3
    (LONG*)(RM_BUF_ADDR + 560), // IF3
    (LONG*)(RM_BUF_ADDR + 564), // IAT1
    (LONG*)(RM_BUF_ADDR + 568), // IAT2
    (LONG*)(RM_BUF_ADDR + 572), // IAT
    (LONG*)(RM_BUF_ADDR + 576), // AIT1
    (LONG*)(RM_BUF_ADDR + 580), // AIF1
    (LONG*)(RM_BUF_ADDR + 584), // AIT2
    (LONG*)(RM_BUF_ADDR + 588), // AIF2
    (LONG*)(RM_BUF_ADDR + 592), // AIT3
    (LONG*)(RM_BUF_ADDR + 596), // AIF3
    (LONG*)(RM_BUF_ADDR + 600) // ST
};

//������ද��2408----------
const  LONG* FailureInf2408[] = {
    {NULL}
};

//ͨ�����Գɹ�2409----------
const  LONG* FailureInf2409[] = {
    {NULL}
};

//ͨ������2410----------
const  LONG* FailureInf2410[] = {
    {NULL}
};

//ͨ��1����2411----------
const  LONG* FailureInf2411[] = {
    {NULL}
};

//ͨ��2����2412----------
const  LONG* FailureInf2412[] = {
    {NULL}
};

//ͨ��3����2421----------
const  LONG* FailureInf2421[] = {
    {NULL}
};

//ͨ��4����2422----------
const  LONG* FailureInf2422[] = {
    {NULL}
};

//ͨ��5����2454----------
const  LONG* FailureInf2454[] = {
    {NULL}
};

//������2415----------
const  LONG* FailureInf2415[] = {
    {NULL}
};

//GPS����2416----------
const  LONG* FailureInf2416[] = {
    {NULL}
};

//ͨ�����ϲ��ʧ��2414----------
const  LONG* FailureInf2414[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//��ʱ���ϲ��ʧ��2417----------
const  LONG* FailureInf2417[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//�ٲ��ද��2418----------
const  LONG* FailureInf2418[] = {
    {NULL}
};

//PT����2419----------
const  LONG* FailureInf2419[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36) // If3
};

//ͨ�Ų���2420----------
const  LONG* FailureInf2420[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����1��������2423----------
const  LONG* FailureInf2423[] = {
    (LONG*)(RM_BUF_ADDR + 108), // U1
    (LONG*)(RM_BUF_ADDR + 112), // U2
    (LONG*)(RM_BUF_ADDR + 124), // IT1
    (LONG*)(RM_BUF_ADDR + 128), // IF1
    (LONG*)(RM_BUF_ADDR + 132), // IT2
    (LONG*)(RM_BUF_ADDR + 136), // IF2
    (LONG*)(RM_BUF_ADDR + 140), // IT3
    (LONG*)(RM_BUF_ADDR + 144), // IF3
    (LONG*)(RM_BUF_ADDR + 148), // IAT1
    (LONG*)(RM_BUF_ADDR + 152), // IAT2
    (LONG*)(RM_BUF_ADDR + 156), // IAT
    (LONG*)(RM_BUF_ADDR + 160), // AIT1
    (LONG*)(RM_BUF_ADDR + 164), // AIF1
    (LONG*)(RM_BUF_ADDR + 168), // AIT2
    (LONG*)(RM_BUF_ADDR + 172), // AIF2
    (LONG*)(RM_BUF_ADDR + 176), // AIT3
    (LONG*)(RM_BUF_ADDR + 180), // AIF3
    (LONG*)(RM_BUF_ADDR + 184) // ST
};

//����2��������2424----------
const  LONG* FailureInf2424[] = {
    (LONG*)(RM_BUF_ADDR + 212), // U1
    (LONG*)(RM_BUF_ADDR + 216), // U2
    (LONG*)(RM_BUF_ADDR + 228), // IT1
    (LONG*)(RM_BUF_ADDR + 232), // IF1
    (LONG*)(RM_BUF_ADDR + 236), // IT2
    (LONG*)(RM_BUF_ADDR + 240), // IF2
    (LONG*)(RM_BUF_ADDR + 244), // IT3
    (LONG*)(RM_BUF_ADDR + 248), // IF3
    (LONG*)(RM_BUF_ADDR + 252), // IAT1
    (LONG*)(RM_BUF_ADDR + 256), // IAT2
    (LONG*)(RM_BUF_ADDR + 260), // IAT
    (LONG*)(RM_BUF_ADDR + 264), // AIT1
    (LONG*)(RM_BUF_ADDR + 268), // AIF1
    (LONG*)(RM_BUF_ADDR + 272), // AIT2
    (LONG*)(RM_BUF_ADDR + 276), // AIF2
    (LONG*)(RM_BUF_ADDR + 280), // AIT3
    (LONG*)(RM_BUF_ADDR + 284), // AIF3
    (LONG*)(RM_BUF_ADDR + 288) // ST
};

//����3��������2425----------
const  LONG* FailureInf2425[] = {
    (LONG*)(RM_BUF_ADDR + 316), // U1
    (LONG*)(RM_BUF_ADDR + 320), // U2
    (LONG*)(RM_BUF_ADDR + 332), // IT1
    (LONG*)(RM_BUF_ADDR + 336), // IF1
    (LONG*)(RM_BUF_ADDR + 340), // IT2
    (LONG*)(RM_BUF_ADDR + 344), // IF2
    (LONG*)(RM_BUF_ADDR + 348), // IT3
    (LONG*)(RM_BUF_ADDR + 352), // IF3
    (LONG*)(RM_BUF_ADDR + 356), // IAT1
    (LONG*)(RM_BUF_ADDR + 360), // IAT2
    (LONG*)(RM_BUF_ADDR + 364), // IAT
    (LONG*)(RM_BUF_ADDR + 368), // AIT1
    (LONG*)(RM_BUF_ADDR + 372), // AIF1
    (LONG*)(RM_BUF_ADDR + 376), // AIT2
    (LONG*)(RM_BUF_ADDR + 380), // AIF2
    (LONG*)(RM_BUF_ADDR + 384), // AIT3
    (LONG*)(RM_BUF_ADDR + 388), // AIF3
    (LONG*)(RM_BUF_ADDR + 392) // ST
};

//����4��������2426----------
const  LONG* FailureInf2426[] = {
    (LONG*)(RM_BUF_ADDR + 420), // U1
    (LONG*)(RM_BUF_ADDR + 424), // U2
    (LONG*)(RM_BUF_ADDR + 436), // IT1
    (LONG*)(RM_BUF_ADDR + 440), // IF1
    (LONG*)(RM_BUF_ADDR + 444), // IT2
    (LONG*)(RM_BUF_ADDR + 448), // IF2
    (LONG*)(RM_BUF_ADDR + 452), // IT3
    (LONG*)(RM_BUF_ADDR + 456), // IF3
    (LONG*)(RM_BUF_ADDR + 460), // IAT1
    (LONG*)(RM_BUF_ADDR + 464), // IAT2
    (LONG*)(RM_BUF_ADDR + 468), // IAT
    (LONG*)(RM_BUF_ADDR + 472), // AIT1
    (LONG*)(RM_BUF_ADDR + 476), // AIF1
    (LONG*)(RM_BUF_ADDR + 480), // AIT2
    (LONG*)(RM_BUF_ADDR + 484), // AIF2
    (LONG*)(RM_BUF_ADDR + 488), // AIT3
    (LONG*)(RM_BUF_ADDR + 492), // AIF3
    (LONG*)(RM_BUF_ADDR + 496) // ST
};

//����5��������2452----------
const  LONG* FailureInf2452[] = {
    (LONG*)(RM_BUF_ADDR + 524), // U1
    (LONG*)(RM_BUF_ADDR + 528), // U2
    (LONG*)(RM_BUF_ADDR + 540), // IT1
    (LONG*)(RM_BUF_ADDR + 544), // IF1
    (LONG*)(RM_BUF_ADDR + 548), // IT2
    (LONG*)(RM_BUF_ADDR + 552), // IF2
    (LONG*)(RM_BUF_ADDR + 556), // IT3
    (LONG*)(RM_BUF_ADDR + 560), // IF3
    (LONG*)(RM_BUF_ADDR + 564), // IAT1
    (LONG*)(RM_BUF_ADDR + 568), // IAT2
    (LONG*)(RM_BUF_ADDR + 572), // IAT
    (LONG*)(RM_BUF_ADDR + 576), // AIT1
    (LONG*)(RM_BUF_ADDR + 580), // AIF1
    (LONG*)(RM_BUF_ADDR + 584), // AIT2
    (LONG*)(RM_BUF_ADDR + 588), // AIF2
    (LONG*)(RM_BUF_ADDR + 592), // AIT3
    (LONG*)(RM_BUF_ADDR + 596), // AIF3
    (LONG*)(RM_BUF_ADDR + 600) // ST
};

//GPS���ϲ��ʧ��2429----------
const  LONG* FailureInf2429[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����1���ݶ�ʧ2430----------
const  LONG* FailureInf2430[] = {
    (LONG*)(RM_BUF_ADDR + 108), // U1
    (LONG*)(RM_BUF_ADDR + 112), // U2
    (LONG*)(RM_BUF_ADDR + 124), // IT1
    (LONG*)(RM_BUF_ADDR + 128), // IF1
    (LONG*)(RM_BUF_ADDR + 132), // IT2
    (LONG*)(RM_BUF_ADDR + 136), // IF2
    (LONG*)(RM_BUF_ADDR + 140), // IT3
    (LONG*)(RM_BUF_ADDR + 144), // IF3
    (LONG*)(RM_BUF_ADDR + 148), // IAT1
    (LONG*)(RM_BUF_ADDR + 152), // IAT2
    (LONG*)(RM_BUF_ADDR + 156), // IAT
    (LONG*)(RM_BUF_ADDR + 160), // AIT1
    (LONG*)(RM_BUF_ADDR + 164), // AIF1
    (LONG*)(RM_BUF_ADDR + 168), // AIT2
    (LONG*)(RM_BUF_ADDR + 172), // AIF2
    (LONG*)(RM_BUF_ADDR + 176), // AIT3
    (LONG*)(RM_BUF_ADDR + 180), // AIF3
    (LONG*)(RM_BUF_ADDR + 184) // ST
};

//����2���ݶ�ʧ2431----------
const  LONG* FailureInf2431[] = {
    (LONG*)(RM_BUF_ADDR + 212), // U1
    (LONG*)(RM_BUF_ADDR + 216), // U2
    (LONG*)(RM_BUF_ADDR + 228), // IT1
    (LONG*)(RM_BUF_ADDR + 232), // IF1
    (LONG*)(RM_BUF_ADDR + 236), // IT2
    (LONG*)(RM_BUF_ADDR + 240), // IF2
    (LONG*)(RM_BUF_ADDR + 244), // IT3
    (LONG*)(RM_BUF_ADDR + 248), // IF3
    (LONG*)(RM_BUF_ADDR + 252), // IAT1
    (LONG*)(RM_BUF_ADDR + 256), // IAT2
    (LONG*)(RM_BUF_ADDR + 260), // IAT
    (LONG*)(RM_BUF_ADDR + 264), // AIT1
    (LONG*)(RM_BUF_ADDR + 268), // AIF1
    (LONG*)(RM_BUF_ADDR + 272), // AIT2
    (LONG*)(RM_BUF_ADDR + 276), // AIF2
    (LONG*)(RM_BUF_ADDR + 280), // AIT3
    (LONG*)(RM_BUF_ADDR + 284), // AIF3
    (LONG*)(RM_BUF_ADDR + 288) // ST
};

//����3���ݶ�ʧ2432----------
const  LONG* FailureInf2432[] = {
    (LONG*)(RM_BUF_ADDR + 316), // U1
    (LONG*)(RM_BUF_ADDR + 320), // U2
    (LONG*)(RM_BUF_ADDR + 332), // IT1
    (LONG*)(RM_BUF_ADDR + 336), // IF1
    (LONG*)(RM_BUF_ADDR + 340), // IT2
    (LONG*)(RM_BUF_ADDR + 344), // IF2
    (LONG*)(RM_BUF_ADDR + 348), // IT3
    (LONG*)(RM_BUF_ADDR + 352), // IF3
    (LONG*)(RM_BUF_ADDR + 356), // IAT1
    (LONG*)(RM_BUF_ADDR + 360), // IAT2
    (LONG*)(RM_BUF_ADDR + 364), // IAT
    (LONG*)(RM_BUF_ADDR + 368), // AIT1
    (LONG*)(RM_BUF_ADDR + 372), // AIF1
    (LONG*)(RM_BUF_ADDR + 376), // AIT2
    (LONG*)(RM_BUF_ADDR + 380), // AIF2
    (LONG*)(RM_BUF_ADDR + 384), // AIT3
    (LONG*)(RM_BUF_ADDR + 388), // AIF3
    (LONG*)(RM_BUF_ADDR + 392) // ST
};

//����4���ݶ�ʧ2433----------
const  LONG* FailureInf2433[] = {
    (LONG*)(RM_BUF_ADDR + 420), // U1
    (LONG*)(RM_BUF_ADDR + 424), // U2
    (LONG*)(RM_BUF_ADDR + 436), // IT1
    (LONG*)(RM_BUF_ADDR + 440), // IF1
    (LONG*)(RM_BUF_ADDR + 444), // IT2
    (LONG*)(RM_BUF_ADDR + 448), // IF2
    (LONG*)(RM_BUF_ADDR + 452), // IT3
    (LONG*)(RM_BUF_ADDR + 456), // IF3
    (LONG*)(RM_BUF_ADDR + 460), // IAT1
    (LONG*)(RM_BUF_ADDR + 464), // IAT2
    (LONG*)(RM_BUF_ADDR + 468), // IAT
    (LONG*)(RM_BUF_ADDR + 472), // AIT1
    (LONG*)(RM_BUF_ADDR + 476), // AIF1
    (LONG*)(RM_BUF_ADDR + 480), // AIT2
    (LONG*)(RM_BUF_ADDR + 484), // AIF2
    (LONG*)(RM_BUF_ADDR + 488), // AIT3
    (LONG*)(RM_BUF_ADDR + 492), // AIF3
    (LONG*)(RM_BUF_ADDR + 496) // ST
};

//����5���ݶ�ʧ2451----------
const  LONG* FailureInf2451[] = {
    (LONG*)(RM_BUF_ADDR + 524), // U1
    (LONG*)(RM_BUF_ADDR + 528), // U2
    (LONG*)(RM_BUF_ADDR + 540), // IT1
    (LONG*)(RM_BUF_ADDR + 544), // IF1
    (LONG*)(RM_BUF_ADDR + 548), // IT2
    (LONG*)(RM_BUF_ADDR + 552), // IF2
    (LONG*)(RM_BUF_ADDR + 556), // IT3
    (LONG*)(RM_BUF_ADDR + 560), // IF3
    (LONG*)(RM_BUF_ADDR + 564), // IAT1
    (LONG*)(RM_BUF_ADDR + 568), // IAT2
    (LONG*)(RM_BUF_ADDR + 572), // IAT
    (LONG*)(RM_BUF_ADDR + 576), // AIT1
    (LONG*)(RM_BUF_ADDR + 580), // AIF1
    (LONG*)(RM_BUF_ADDR + 584), // AIT2
    (LONG*)(RM_BUF_ADDR + 588), // AIF2
    (LONG*)(RM_BUF_ADDR + 592), // AIT3
    (LONG*)(RM_BUF_ADDR + 596), // AIF3
    (LONG*)(RM_BUF_ADDR + 600) // ST
};

//���ϲ��ʧ��2434----------
const  LONG* FailureInf2434[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//GPS���ϲ��ʧ��2449----------
const  LONG* FailureInf2449[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//Խ���л��ɹ�2435----------
const  LONG* FailureInf2435[] = {
    {NULL}
};

//Խ���л�ʧ��2436----------
const  LONG* FailureInf2436[] = {
    {NULL}
};

//��ʱ���ϲ��ʧ��2445----------
const  LONG* FailureInf2445[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����TR����-�翹���2437----------
const  LONG* FailureInf2437[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����FR����-�翹���2438----------
const  LONG* FailureInf2438[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����TR����-�翹���2439----------
const  LONG* FailureInf2439[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����FR����-�翹���2440----------
const  LONG* FailureInf2440[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//��������2441----------
const  LONG* FailureInf2441[] = {
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 8), // U3
    (LONG*)(RM_BUF_ADDR + 12), // U4
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 60), // AU3
    (LONG*)(RM_BUF_ADDR + 64), // AU4
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-R����2442----------
const  LONG* FailureInf2442[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����F-R����2443----------
const  LONG* FailureInf2443[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-R����2447----------
const  LONG* FailureInf2447[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����F-R����2448----------
const  LONG* FailureInf2448[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//T-F����2444----------
const  LONG* FailureInf2444[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//ͨ�Ų���2446----------
const  LONG* FailureInf2446[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//���ϲ��ʧ��2450----------
const  LONG* FailureInf2450[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//���������쳣22----------
const  LONG* FailureInf22[] = {
    {NULL}
};

//���緽ʽ�쳣2455----------
const  LONG* FailureInf2455[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-F����2456----------
const  LONG* FailureInf2456[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-F����2457----------
const  LONG* FailureInf2457[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-F����2458----------
const  LONG* FailureInf2458[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
};

//����T-F����2459----------
const  LONG* FailureInf2459[] = {
    (LONG*)(RM_BUF_ADDR + 628), // D
    (LONG*)(RM_BUF_ADDR + 632), // KMS
    (LONG*)(RM_BUF_ADDR + 652), // R_TF
    (LONG*)(RM_BUF_ADDR + 656), // X_TF
    (LONG*)(RM_BUF_ADDR + 636), // R_T
    (LONG*)(RM_BUF_ADDR + 640), // X_T
    (LONG*)(RM_BUF_ADDR + 644), // R_F
    (LONG*)(RM_BUF_ADDR + 648), // X_F
    (LONG*)(RM_BUF_ADDR + 676), // Up
    (LONG*)(RM_BUF_ADDR + 0), // U1
    (LONG*)(RM_BUF_ADDR + 4), // U2
    (LONG*)(RM_BUF_ADDR + 680), // I1
    (LONG*)(RM_BUF_ADDR + 16), // It1
    (LONG*)(RM_BUF_ADDR + 20), // If1
    (LONG*)(RM_BUF_ADDR + 684), // I2
    (LONG*)(RM_BUF_ADDR + 24), // It2
    (LONG*)(RM_BUF_ADDR + 28), // If2
    (LONG*)(RM_BUF_ADDR + 688), // I3
    (LONG*)(RM_BUF_ADDR + 32), // It3
    (LONG*)(RM_BUF_ADDR + 36), // If3
    (LONG*)(RM_BUF_ADDR + 40), // Iat1
    (LONG*)(RM_BUF_ADDR + 44), // Iat2
    (LONG*)(RM_BUF_ADDR + 48), // IAT
    (LONG*)(RM_BUF_ADDR + 52), // AU1
    (LONG*)(RM_BUF_ADDR + 56), // AU2
    (LONG*)(RM_BUF_ADDR + 68), // AIT1
    (LONG*)(RM_BUF_ADDR + 72), // AIF1
    (LONG*)(RM_BUF_ADDR + 76), // AIT2
    (LONG*)(RM_BUF_ADDR + 80), // AIF2
    (LONG*)(RM_BUF_ADDR + 84), // AIT3
    (LONG*)(RM_BUF_ADDR + 88) // AIF3
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
    {0, FailureInf65, 65},//���������쳣�ָ�
    {0, FailureInf500, 500},//��������
    {0, FailureInf2400, 2400},//���ϲ������
    {31, FailureInf2401, 2401},//����T-R����
    {31, FailureInf2402, 2402},//����F-R����
    {31, FailureInf2427, 2427},//����T-R����
    {31, FailureInf2428, 2428},//����F-R����
    {31, FailureInf2403, 2403},//T-F����
    {18, FailureInf2404, 2404},//����1GPS����
    {18, FailureInf2405, 2405},//����2GPS����
    {18, FailureInf2406, 2406},//����3GPS����
    {18, FailureInf2407, 2407},//����4GPS����
    {18, FailureInf2453, 2453},//����5GPS����
    {0, FailureInf2408, 2408},//������ද��
    {0, FailureInf2409, 2409},//ͨ�����Գɹ�
    {0, FailureInf2410, 2410},//ͨ������
    {0, FailureInf2411, 2411},//ͨ��1����
    {0, FailureInf2412, 2412},//ͨ��2����
    {0, FailureInf2421, 2421},//ͨ��3����
    {0, FailureInf2422, 2422},//ͨ��4����
    {0, FailureInf2454, 2454},//ͨ��5����
    {0, FailureInf2415, 2415},//������
    {0, FailureInf2416, 2416},//GPS����
    {31, FailureInf2414, 2414},//ͨ�����ϲ��ʧ��
    {31, FailureInf2417, 2417},//��ʱ���ϲ��ʧ��
    {0, FailureInf2418, 2418},//�ٲ��ද��
    {8, FailureInf2419, 2419},//PT����
    {31, FailureInf2420, 2420},//ͨ�Ų���
    {18, FailureInf2423, 2423},//����1��������
    {18, FailureInf2424, 2424},//����2��������
    {18, FailureInf2425, 2425},//����3��������
    {18, FailureInf2426, 2426},//����4��������
    {18, FailureInf2452, 2452},//����5��������
    {31, FailureInf2429, 2429},//GPS���ϲ��ʧ��
    {18, FailureInf2430, 2430},//����1���ݶ�ʧ
    {18, FailureInf2431, 2431},//����2���ݶ�ʧ
    {18, FailureInf2432, 2432},//����3���ݶ�ʧ
    {18, FailureInf2433, 2433},//����4���ݶ�ʧ
    {18, FailureInf2451, 2451},//����5���ݶ�ʧ
    {31, FailureInf2434, 2434},//���ϲ��ʧ��
    {27, FailureInf2449, 2449},//GPS���ϲ��ʧ��
    {0, FailureInf2435, 2435},//Խ���л��ɹ�
    {0, FailureInf2436, 2436},//Խ���л�ʧ��
    {27, FailureInf2445, 2445},//��ʱ���ϲ��ʧ��
    {31, FailureInf2437, 2437},//����TR����-�翹���
    {31, FailureInf2438, 2438},//����FR����-�翹���
    {31, FailureInf2439, 2439},//����TR����-�翹���
    {31, FailureInf2440, 2440},//����FR����-�翹���
    {23, FailureInf2441, 2441},//��������
    {27, FailureInf2442, 2442},//����T-R����
    {27, FailureInf2443, 2443},//����F-R����
    {27, FailureInf2447, 2447},//����T-R����
    {27, FailureInf2448, 2448},//����F-R����
    {27, FailureInf2444, 2444},//T-F����
    {27, FailureInf2446, 2446},//ͨ�Ų���
    {27, FailureInf2450, 2450},//���ϲ��ʧ��
    {0, FailureInf22, 22},//���������쳣
    {31, FailureInf2455, 2455},//���緽ʽ�쳣
    {27, FailureInf2456, 2456},//����T-F����
    {27, FailureInf2457, 2457},//����T-F����
    {31, FailureInf2458, 2458},//����T-F����
    {31, FailureInf2459, 2459},//����T-F����
};

//�̵���ִ����������----------
const WORD RelayWordTypeList[]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵�����
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �м�̵�����
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// �Ա����м�̵�����
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,// ʱ��̵�����
3,3,// ��բ�̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ����̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,// �����̵���
0,0,0,0,// ѹ��̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ���ü̵���
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,// ����̵���
0,0,0,0,0,0,0,// �����̵���
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,// �źż̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// LED�̵���
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,// ���⿪��
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��Ԥ�ü̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң��ִ�м̵���
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,// ң�ط�У�̵���
8,8,8,8,8,8,8,8,// ���ڵ��ڼ̵���
9,9,9,9,9,9,9,// ����ʱ��̵���
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
	{ (LONG*)(NoTimeAddr), (LONG*)(AS_BUF_ADDR + 136) },
};
//��բ�̵���ʱ�䶨ֵ(��������ֵ������ֵȥ)
LONG* TripSetting[] = {
	 (LONG*)(AS_BUF_ADDR + 144) ,
	 (LONG*)(AS_BUF_ADDR + 148) ,
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
};
//ѹ��̵���ӳ�䵽ѹ���ѹ���
const WORD  SWIndexMap[] = {
	 (WORD)(0) ,
	 (WORD)(1) ,
	 (WORD)(2) ,
	 (WORD)(3) ,
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
	 (BYTE*)(CFG_BUF_ADDR + 12) ,
	 (BYTE*)(CFG_BUF_ADDR + 13) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 14) ,
	 (BYTE*)(CFG_BUF_ADDR + 11) ,
	 (BYTE*)(CFG_BUF_ADDR + 16) ,
	 (BYTE*)(CFG_BUF_ADDR + 17) ,
	 (BYTE*)(CFG_BUF_ADDR + 18) ,
	 (BYTE*)(CFG_BUF_ADDR + 19) ,
	 (BYTE*)(CFG_BUF_ADDR + 20) ,
	 (BYTE*)(CFG_BUF_ADDR + 21) ,
	 (BYTE*)(CFG_BUF_ADDR + 22) ,
};
//ϵ��Ĭ��ֵ
//ϵ��1
const LONG CFDefaultVal1[]=
{
	114, //U1
	114, //U2
	114, //U3
	114, //U4
	305, //IT1
	306, //IF1
	304, //IT2
	304, //IF2
	304, //IT3
	304, //IF3
	304, //IAT1
	304, //IAT2
	752, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};
//ϵ��2
const LONG CFDefaultVal2[]=
{
	114, //U1
	114, //U2
	114, //U3
	114, //U4
	1525, //IT1
	1525, //IF1
	1524, //IT2
	1525, //IF2
	1525, //IT3
	1525, //IF3
	1525, //IAT1
	1525, //IAT2
	752, //Z
	1000, //C1
	100, //C2
	10, //C3
	1, //C4
};

//����----------------------------------
