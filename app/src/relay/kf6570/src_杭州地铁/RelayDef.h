/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			RelayDef.h
* 
*   ���ģ�飺�����ӿ��ļ�
*
* 
*	����  ���������ļ����ϰ벿���ǹ̶������ݽṹ�� �°벿���ɱ����ӿڹ�������Զ�����
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
*		kangqian	2010-04-21		����ѹ��Ͷ�˼̵���ʹ�õĲ�������
*************************************************************************/

#ifndef __RelayDef_H
#define __RelayDef_H


#include "sysdefs.h"


#ifdef __PL_WIN_SIMULATION__
	// ��windowģ��ƽ̨��ʹ��һ������ģ��ram 
	extern unsigned char _RAMDataBuf[];
	#define RELAY_RAM_ADDR_BASE	_RAMDataBuf
#else
#include "resm5234.h"
	//�������õ�Ƭ��ram��ַ
	#define RELAY_RAM_ADDR_BASE	RELAY_RAM_BASE // Ӳ������
#endif

//��ֵ ֵ���� ����󳤶�
#define  MAX_SETVALUNAME  256 

// ���������ṹ
typedef struct TReportInf{
    DWORD dwFialureNum;                // ���ϵ������� 
    const LONG** pFailureData; // ���ϵ���������Ϣ 
	WORD wInf; //����inf��
}TReportInf;

//�߼�������ʱ��̵���ʱ�������������ֵ������ֵ��
typedef struct TTimeSetting{
	LONG* pStartTime; //����ʱ�䣬ָ������Ķ�ֵ��������ֵ��
	LONG* pReturnTime;	//����ʱ�䣬ָ������Ķ�ֵ��������ֵ��
}TTimeSetting;

//��ֵȡֵ�����б�
typedef struct TSetValueInf
{
	LONG lValue;
	char strName[MAX_SETVALUNAME];
}TSetValueInf;

//��ֵȡֵ������
typedef struct TSetValueInfMap
{
	LONG nItem;
	TSetValueInf* pSetValueInf;
}TSetValueInfMap;

extern const WORD RelayWordTypeList[]; //�̵������ͱ�
extern const struct TSetValueInfMap SetValueInfMap[];
extern const struct TSetValueInf SetValueInf[];

extern const struct TSetValueInfMap SysSetValueInfMap[];
extern const struct TSetValueInf SysSetValueInf[];

extern const struct TTimeSetting TimeSetting[];          // ʱ��̵�����ֵ
extern  LONG*	TripSetting[]; //��բ�̵�����ʱ��ֵ

extern WORD const OutPutMap[]; //����̵���-���� ӳ��� ���濪����
extern WORD const InPutMap[]; //����̵���-���� ӳ���

extern BYTE* const SWMap[]; //ѹ��ӳ��� ֱ��ָ���������ѹ�����ڴ��еĸ���
extern BYTE* const CFGMap[]; //����ӳ���

extern const struct TReportInf ReportInf[];          // �������� 
//ϵ��Ĭ��ֵ
extern const LONG CFDefaultVal1[]; //ϵ��1
extern const LONG CFDefaultVal2[]; //ϵ��2

//2010-4-21 11:25:33 
extern WORD const SWIndexMap[]; //ѹ��̵�����ѹ��ŵ�ӳ���ϵ�� ������ѹ�����
                                //ע����SWMap[]������SWIndexMap��Ϊͨ��ѹ��Ͷ�˼̵���Ͷ����ѹ��ʱʹ�õ�

//���´����ɹ����������


#define 	 SETTING_AREA_COUNT 	 16 		 //��ֵ������
#define 	 RELAY_DATA_COUNT 	 16 		 //�Ŷ�����������
#define 	 SAMPLE_COUNT 	 32 		 //ÿ�ܲ���������
#define 	 A15TYPE_SYSSETNO 	 0 		 //1,5A ���͸�����ֵ���
#define 	 FAILDATA_DISPTYPE_CFGNO 	 0 		 //���ϵ�����ʾ�����������
#define 	 LangTYPE_SYSSETNO 	 0 		 //�������͸�����ֵ���

//��ǰ��ֵ���Ŷ���-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //��ǰ��ֵ����

//��ֵȡֵ�궨��

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//��������ֵ����-----------------------
#define  RM_AUHA 0
extern LONG* 	RM_pnAUHA; 		 // ��UHA
#define  RM_AUHB 1
extern LONG* 	RM_pnAUHB; 		 // ��UHB
#define  RM_AUHC 2
extern LONG* 	RM_pnAUHC; 		 // ��UHC
#define  RM_AULA 3
extern LONG* 	RM_pnAULA; 		 // ��ULA
#define  RM_AULB 4
extern LONG* 	RM_pnAULB; 		 // ��ULB
#define  RM_AULC 5
extern LONG* 	RM_pnAULC; 		 // ��ULC
#define  RM_AIHA 6
extern LONG* 	RM_pnAIHA; 		 // ��IHA
#define  RM_AIHB 7
extern LONG* 	RM_pnAIHB; 		 // ��IHB
#define  RM_AIHC 8
extern LONG* 	RM_pnAIHC; 		 // ��IHC
#define  RM_AILA 9
extern LONG* 	RM_pnAILA; 		 // ��ILA
#define  RM_AILB 10
extern LONG* 	RM_pnAILB; 		 // ��ILB
#define  RM_AILC 11
extern LONG* 	RM_pnAILC; 		 // ��ILC
#define  RM_BOBRCH1 12
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 13
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 14
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 15
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     16    //��������ֵ����
#define RM_BUF_SIZE     64    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UHA 0
extern LONG* 	M_pnUHA; 		 //UA UA
#define  M_UHB 1
extern LONG* 	M_pnUHB; 		 //UB UB
#define  M_UHC 2
extern LONG* 	M_pnUHC; 		 //UC UC
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_ULA 4
extern LONG* 	M_pnULA; 		 //Ua Ua
#define  M_ULB 5
extern LONG* 	M_pnULB; 		 //Ub Ub
#define  M_ULC 6
extern LONG* 	M_pnULC; 		 //Uc Uc
#define  M_IHA 7
extern LONG* 	M_pnIHA; 		 //IA IA
#define  M_IHB 8
extern LONG* 	M_pnIHB; 		 //IB IB
#define  M_IHC 9
extern LONG* 	M_pnIHC; 		 //IC IC
#define  M_ILA 10
extern LONG* 	M_pnILA; 		 //Ia Ia
#define  M_ILB 11
extern LONG* 	M_pnILB; 		 //Ib Ib
#define  M_ILC 12
extern LONG* 	M_pnILC; 		 //Ic Ic
#define  M_I1 13
extern LONG* 	M_pnI1; 		 //I1 I1
#define  M_I2 14
extern LONG* 	M_pnI2; 		 //I2 I2
#define  M_PA 15
extern LONG* 	M_pnPA; 		 //PA PA
#define  M_QA 16
extern LONG* 	M_pnQA; 		 //QA QA
#define  M_COSA 17
extern LONG* 	M_pnCOSA; 		 //COSA COSA
#define  M_PB 18
extern LONG* 	M_pnPB; 		 //PB PB
#define  M_QB 19
extern LONG* 	M_pnQB; 		 //QB QB
#define  M_COSB 20
extern LONG* 	M_pnCOSB; 		 //COSB COSB
#define  M_PC 21
extern LONG* 	M_pnPC; 		 //PC PC
#define  M_QC 22
extern LONG* 	M_pnQC; 		 //QC QC
#define  M_COSC 23
extern LONG* 	M_pnCOSC; 		 //COSC COSC
#define  M_P 24
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 25
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 26
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_F 27
extern LONG* 	M_pnF; 		 //F F
#define  M_UHAB 28
extern LONG* 	M_pnUHAB; 		 //UAB UAB
#define  M_UHBC 29
extern LONG* 	M_pnUHBC; 		 //UBC UBC
#define  M_UHCA 30
extern LONG* 	M_pnUHCA; 		 //UCA UCA
#define  M_UHA1 31
extern LONG* 	M_pnUHA1; 		 //UA1 UA1
#define  M_UHA2 32
extern LONG* 	M_pnUHA2; 		 //UA2 UA2
#define  M_UHA3 33
extern LONG* 	M_pnUHA3; 		 //UA3 UA3
#define  M_UHA5 34
extern LONG* 	M_pnUHA5; 		 //UA5 UA5
#define  M_UHA7 35
extern LONG* 	M_pnUHA7; 		 //UA7 UA7
#define  M_UHA9 36
extern LONG* 	M_pnUHA9; 		 //UA9 UA9
#define  M_UHA11 37
extern LONG* 	M_pnUHA11; 		 //UA11 UA11
#define  M_UHB1 38
extern LONG* 	M_pnUHB1; 		 //UB1 UB1
#define  M_UHB2 39
extern LONG* 	M_pnUHB2; 		 //UB2 UB2
#define  M_UHB3 40
extern LONG* 	M_pnUHB3; 		 //UB3 UB3
#define  M_UHB5 41
extern LONG* 	M_pnUHB5; 		 //UB5 UB5
#define  M_UHB7 42
extern LONG* 	M_pnUHB7; 		 //UB7 UB7
#define  M_UHB9 43
extern LONG* 	M_pnUHB9; 		 //UB9 UB9
#define  M_UHB11 44
extern LONG* 	M_pnUHB11; 		 //UB11 UB11
#define  M_UHC1 45
extern LONG* 	M_pnUHC1; 		 //UC1 UC1
#define  M_UHC2 46
extern LONG* 	M_pnUHC2; 		 //UC2 UC2
#define  M_UHC3 47
extern LONG* 	M_pnUHC3; 		 //UC3 UC3
#define  M_UHC5 48
extern LONG* 	M_pnUHC5; 		 //UC5 UC5
#define  M_UHC7 49
extern LONG* 	M_pnUHC7; 		 //UC7 UC7
#define  M_UHC9 50
extern LONG* 	M_pnUHC9; 		 //UC9 UC9
#define  M_UHC11 51
extern LONG* 	M_pnUHC11; 		 //UC11 UC11
#define  M_IHA1 52
extern LONG* 	M_pnIHA1; 		 //IA1 IA1
#define  M_IHA2 53
extern LONG* 	M_pnIHA2; 		 //IA2 IA2
#define  M_IHA3 54
extern LONG* 	M_pnIHA3; 		 //IA3 IA3
#define  M_IHA5 55
extern LONG* 	M_pnIHA5; 		 //IA5 IA5
#define  M_IHA7 56
extern LONG* 	M_pnIHA7; 		 //IA7 IA7
#define  M_IHA9 57
extern LONG* 	M_pnIHA9; 		 //IA9 IA9
#define  M_IHA11 58
extern LONG* 	M_pnIHA11; 		 //IA11 IA11
#define  M_IHB1 59
extern LONG* 	M_pnIHB1; 		 //IB1 IB1
#define  M_IHB2 60
extern LONG* 	M_pnIHB2; 		 //IB2 IB2
#define  M_IHB3 61
extern LONG* 	M_pnIHB3; 		 //IB3 IB3
#define  M_IHB5 62
extern LONG* 	M_pnIHB5; 		 //IB5 IB5
#define  M_IHB7 63
extern LONG* 	M_pnIHB7; 		 //IB7 IB7
#define  M_IHB9 64
extern LONG* 	M_pnIHB9; 		 //IB9 IB9
#define  M_IHB11 65
extern LONG* 	M_pnIHB11; 		 //IB11 IB11
#define  M_IHC1 66
extern LONG* 	M_pnIHC1; 		 //IC1 IC1
#define  M_IHC2 67
extern LONG* 	M_pnIHC2; 		 //IC2 IC2
#define  M_IHC3 68
extern LONG* 	M_pnIHC3; 		 //IC3 IC3
#define  M_IHC5 69
extern LONG* 	M_pnIHC5; 		 //IC5 IC5
#define  M_IHC7 70
extern LONG* 	M_pnIHC7; 		 //IC7 IC7
#define  M_IHC9 71
extern LONG* 	M_pnIHC9; 		 //IC9 IC9
#define  M_IHC11 72
extern LONG* 	M_pnIHC11; 		 //IC11 IC11
#define  M_IA1 73
extern LONG* 	M_pnIa1; 		 //Ia1 Ia1
#define  M_IA2 74
extern LONG* 	M_pnIa2; 		 //Ia2 Ia2
#define  M_IA3 75
extern LONG* 	M_pnIa3; 		 //Ia3 Ia3
#define  M_IA5 76
extern LONG* 	M_pnIa5; 		 //Ia5 Ia5
#define  M_IA7 77
extern LONG* 	M_pnIa7; 		 //Ia7 Ia7
#define  M_IA9 78
extern LONG* 	M_pnIa9; 		 //Ia9 Ia9
#define  M_IA11 79
extern LONG* 	M_pnIa11; 		 //Ia11 Ia11
#define  M_IB1 80
extern LONG* 	M_pnIb1; 		 //Ib1 Ib1
#define  M_IB2 81
extern LONG* 	M_pnIb2; 		 //Ib2 Ib2
#define  M_IB3 82
extern LONG* 	M_pnIb3; 		 //Ib3 Ib3
#define  M_IB5 83
extern LONG* 	M_pnIb5; 		 //Ib5 Ib5
#define  M_IB7 84
extern LONG* 	M_pnIb7; 		 //Ib7 Ib7
#define  M_IB9 85
extern LONG* 	M_pnIb9; 		 //Ib9 Ib9
#define  M_IB11 86
extern LONG* 	M_pnIb11; 		 //Ib11 Ib11
#define  M_IC1 87
extern LONG* 	M_pnIc1; 		 //Ic1 Ic1
#define  M_IC2 88
extern LONG* 	M_pnIc2; 		 //Ic2 Ic2
#define  M_IC3 89
extern LONG* 	M_pnIc3; 		 //Ic3 Ic3
#define  M_IC5 90
extern LONG* 	M_pnIc5; 		 //Ic5 Ic5
#define  M_IC7 91
extern LONG* 	M_pnIc7; 		 //Ic7 Ic7
#define  M_IC9 92
extern LONG* 	M_pnIc9; 		 //Ic9 Ic9
#define  M_IC11 93
extern LONG* 	M_pnIc11; 		 //Ic11 Ic11
#define  M_CUSTOM1 94
extern LONG* 	M_pnCustom1; 		 //CTM1 CTM1
#define  M_CUSTOM2 95
extern LONG* 	M_pnCustom2; 		 //CTM2 CTM2
#define  M_CUSTOM3 96
extern LONG* 	M_pnCustom3; 		 //CTM3 CTM3
#define  M_CUSTOM4 97
extern LONG* 	M_pnCustom4; 		 //CTM4 CTM4
#define  M_CUSTOM5 98
extern LONG* 	M_pnCustom5; 		 //CTM5 CTM5
#define  M_CUSTOM6 99
extern LONG* 	M_pnCustom6; 		 //CTM6 CTM6
#define  M_CUSTOM7 100
extern LONG* 	M_pnCustom7; 		 //CTM7 CTM7
#define  M_CUSTOM8 101
extern LONG* 	M_pnCustom8; 		 //CTM8 CTM8

#define MEASURE_COUNT     102    //����ֵ����
#define MEASURE_BUF_SIZE     408    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 68)

//��ѹ��----------------------------------
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2
extern BYTE* 	SW_pbyBAK3; 		 //����ѹ��3
extern BYTE* 	SW_pbyBAK4; 		 //����ѹ��4

#define SW_COUNT     5    //��ѹ�����
#define SW_BUF_SIZE     5    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 476)

//����----------------------------------
extern BYTE* 	CFG_pbyMODSET; 		 //��ֵ�޸Ĺ���
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2
extern BYTE* 	CFG_pbyBAK3; 		 //��������3
extern BYTE* 	CFG_pbyBAK4; 		 //��������4

#define CFG_COUNT     5    //���ø���/
#define CFG_BUF_SIZE     5    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 482)

//��ֵ----------------------------------
extern LONG* 	RS_pnF; 		 //������ F
extern LONG* 	RS_pnT1PI; 		 //T1������ʱ T1PI
extern LONG* 	RS_pnT1RE; 		 //T1������ʱ T1RE
extern LONG* 	RS_pnT2PI; 		 //T2������ʱ T2PI
extern LONG* 	RS_pnT2RE; 		 //T2������ʱ T2RE
extern LONG* 	RS_pnT3PI; 		 //T3������ʱ T3PI
extern LONG* 	RS_pnT3RE; 		 //T3������ʱ T3RE
extern LONG* 	RS_pnT4PI; 		 //T4������ʱ T4PI
extern LONG* 	RS_pnT4RE; 		 //T4������ʱ T4RE
extern LONG* 	RS_pnT5PI; 		 //T5������ʱ T5PI
extern LONG* 	RS_pnT5RE; 		 //T5������ʱ T5RE
extern LONG* 	RS_pnT6PI; 		 //T6������ʱ T6PI
extern LONG* 	RS_pnT6RE; 		 //T6������ʱ T6RE
extern LONG* 	RS_pnT7PI; 		 //T7������ʱ T7PI
extern LONG* 	RS_pnT7RE; 		 //T7������ʱ T7RE
extern LONG* 	RS_pnT8PI; 		 //T8������ʱ T8PI
extern LONG* 	RS_pnT8RE; 		 //T8������ʱ T8RE
extern LONG* 	RS_pnT9PI; 		 //T9������ʱ T9PI
extern LONG* 	RS_pnT9RE; 		 //T9������ʱ T9RE
extern LONG* 	RS_pnT10PI; 		 //T10������ʱ T10PI
extern LONG* 	RS_pnT10RE; 		 //T10������ʱ T10RE
extern LONG* 	RS_pnT11PI; 		 //T11������ʱ T11PI
extern LONG* 	RS_pnT11RE; 		 //T11������ʱ T11RE
extern LONG* 	RS_pnT12PI; 		 //T12������ʱ T12PI
extern LONG* 	RS_pnT12RE; 		 //T12������ʱ T12RE
extern LONG* 	RS_pnT13PI; 		 //T13������ʱ T13PI
extern LONG* 	RS_pnT13RE; 		 //T13������ʱ T13RE
extern LONG* 	RS_pnT14PI; 		 //T14������ʱ T14PI
extern LONG* 	RS_pnT14RE; 		 //T14������ʱ T14RE
extern LONG* 	RS_pnT15PI; 		 //T15������ʱ T15PI
extern LONG* 	RS_pnT15RE; 		 //T15������ʱ T15RE
extern LONG* 	RS_pnT16PI; 		 //T16������ʱ T16PI
extern LONG* 	RS_pnT16RE; 		 //T16������ʱ T16RE
extern LONG* 	RS_pn59HA; 		 //UHA��ѹ��ֵ 59HA
extern LONG* 	RS_pn27HA; 		 //UHA��ѹ��ֵ 27HA
extern LONG* 	RS_pn59HB; 		 //UHB��ѹ��ֵ 59HB
extern LONG* 	RS_pn27HB; 		 //UHB��ѹ��ֵ 27HB
extern LONG* 	RS_pn59HC; 		 //UHC��ѹ��ֵ 59HC
extern LONG* 	RS_pn27HC; 		 //UHC��ѹ��ֵ 27HC
extern LONG* 	RS_pn59N; 		 //U0��ѹ��ֵ 59N
extern LONG* 	RS_pn27N; 		 //U0��ѹ��ֵ 27N
extern LONG* 	RS_pn59LA; 		 //ULA��ѹ��ֵ 59LA
extern LONG* 	RS_pn27LA; 		 //ULA��ѹ��ֵ 27LA
extern LONG* 	RS_pn59LB; 		 //ULB��ѹ��ֵ 59LB
extern LONG* 	RS_pn27LB; 		 //ULB��ѹ��ֵ 27LB
extern LONG* 	RS_pn59LC; 		 //ULC��ѹ��ֵ 59LC
extern LONG* 	RS_pn27LC; 		 //ULC��ѹ��ֵ 27LC
extern LONG* 	RS_pn50HA; 		 //IHA������ֵ 50HA
extern LONG* 	RS_pn37HA; 		 //IHA������ֵ 37HA
extern LONG* 	RS_pn50HB; 		 //IHB������ֵ 50HB
extern LONG* 	RS_pn37HB; 		 //IHB������ֵ 37HB
extern LONG* 	RS_pn50HC; 		 //IHC������ֵ 50HC
extern LONG* 	RS_pn37HC; 		 //IHC������ֵ 37HC
extern LONG* 	RS_pn50LA; 		 //ILA������ֵ 50LA
extern LONG* 	RS_pn37LA; 		 //ILA������ֵ 37LA
extern LONG* 	RS_pn50LB; 		 //ILB������ֵ 50LB
extern LONG* 	RS_pn37LB; 		 //ILB������ֵ 37LB
extern LONG* 	RS_pn50LC; 		 //ILC������ֵ 50LC
extern LONG* 	RS_pn37LC; 		 //ILC������ֵ 37LC
extern LONG* 	RS_pn50I1; 		 //I1������ֵ 50I1
extern LONG* 	RS_pn37I1; 		 //I1������ֵ 37I1
extern LONG* 	RS_pn50I2; 		 //I2������ֵ 50I2
extern LONG* 	RS_pn37I2; 		 //I2������ֵ 37I2
extern LONG* 	RS_pnTCB; 		 //��·������ʱ�� TCB
extern LONG* 	RS_pnTSW; 		 //�綯��������ʱ�� TSW

#define RS_COUNT     65    //��ֵ����/
#define RS_BUF_SIZE     260    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 488)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnTR1D; 		 //��բ�̵�����1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //��բ�̵�����2����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnHPTRT; 		 //�߲�PT���
extern LONG* 	AS_pnLPTRT; 		 //�Ͳ�PT���
extern LONG* 	AS_pnU0PTRT; 		 //����PT���
extern LONG* 	AS_pnHCTRT; 		 //�߲�CT���
extern LONG* 	AS_pnLACTRT; 		 //�Ͳ�ACT���
extern LONG* 	AS_pnLBCTRT; 		 //�Ͳ�BCT���
extern LONG* 	AS_pnLCCTRT; 		 //�Ͳ�CCT���
extern LONG* 	AS_pnI1CTRT; 		 //I1CT���
extern LONG* 	AS_pnI2CTRT; 		 //I2CT���
extern LONG* 	AS_pnRT; 		 //RT

#define AS_RT_NUM     10    //������ֵ�б�ȸ���

#define AS_COUNT     14    //������ֵ����
#define AS_BUF_SIZE     56    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 748)

//ϵ��----------------------------------
extern LONG* 	CF_pnUHA; 		 //UHA
extern LONG* 	CF_pnUHB; 		 //UHB
extern LONG* 	CF_pnUHC; 		 //UHC
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnULA; 		 //ULA
extern LONG* 	CF_pnULB; 		 //ULB
extern LONG* 	CF_pnULC; 		 //ULC
extern LONG* 	CF_pnIHA; 		 //IHA
extern LONG* 	CF_pnIHB; 		 //IHB
extern LONG* 	CF_pnIHC; 		 //IHC
extern LONG* 	CF_pnILA; 		 //ILA
extern LONG* 	CF_pnILB; 		 //ILB
extern LONG* 	CF_pnILC; 		 //ILC
extern LONG* 	CF_pnI1; 		 //I1
extern LONG* 	CF_pnI2; 		 //I2
extern LONG* 	CF_pnPA; 		 //PA
extern LONG* 	CF_pnPB; 		 //PB
extern LONG* 	CF_pnPC; 		 //PC
extern LONG* 	CF_pnQA; 		 //QA
extern LONG* 	CF_pnQB; 		 //QB
extern LONG* 	CF_pnQC; 		 //QC
extern LONG* 	CF_pnAA; 		 //AA
extern LONG* 	CF_pnAB; 		 //AB
extern LONG* 	CF_pnAC; 		 //AC
extern LONG* 	CF_pnUHAD; 		 //UHADRIFT
extern LONG* 	CF_pnUHBD; 		 //UHBDRIFT
extern LONG* 	CF_pnUHCD; 		 //UHCDRIFT
extern LONG* 	CF_pnU0D; 		 //U0DRIFT
extern LONG* 	CF_pnULAD; 		 //ULADRIFT
extern LONG* 	CF_pnULBD; 		 //ULBDRIFT
extern LONG* 	CF_pnULCD; 		 //ULCDRIFT
extern LONG* 	CF_pnIHAD; 		 //IHADRIFT
extern LONG* 	CF_pnIHBD; 		 //IHBDRIFT
extern LONG* 	CF_pnIHCD; 		 //IHCDRIFT
extern LONG* 	CF_pnILAD; 		 //ILADRIFT
extern LONG* 	CF_pnILBD; 		 //ILBDRIFT
extern LONG* 	CF_pnILCD; 		 //ILCDRIFT
extern LONG* 	CF_pnI1D; 		 //I1DRIFT
extern LONG* 	CF_pnI2D; 		 //I2DRIFT
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnUHAH; 		 //UHAH
extern LONG* 	CF_pnUHBH; 		 //UHBH
extern LONG* 	CF_pnUHCH; 		 //UHCH
extern LONG* 	CF_pnIHAH; 		 //IHAH
extern LONG* 	CF_pnIHBH; 		 //IHBH
extern LONG* 	CF_pnIHCH; 		 //IHCH
extern LONG* 	CF_pnUAB; 		 //UAB
extern LONG* 	CF_pnUBC; 		 //UBC
extern LONG* 	CF_pnUCA; 		 //UCA

#define CF_COUNT     52    //ϵ������
#define CF_BUF_SIZE     208    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 804)

//������ʺ�-----------------------
#define R_RAMERR                    0    //RAM��ֵ����
#define R_RESETAN                   1    //װ���쳣��λ
#define R_COMERR                    2    //ͨ��ͨ���쳣
#define R_NVRAMERR                  3    //NVRAM���ݳ���
#define R_ROMERR                    4    //����ROM����
#define R_EEPROMERR                 5    //EEPROM��ֵ����
#define R_ADERR                     6    //AD����
#define R_OEBR                      7    //�����������
#define R_OEERR                     8    //�������ʧЧ
#define R_SLERR                     9    //��ѹ�����
#define R_LOGICERR                 10    //�߼����̹���
#define R_COMTEST                  11    //ͨ������
#define R_RCERR                    12    //�����Լ����
#define R_NVRAMERR_R               13    //NVRAM���ݳ���ָ�
#define R_ROMERR_R                 14    //����ROM���ϻָ�
#define R_RAMERR_R                 15    //RAM��ֵ���ϻָ�
#define R_EEPROMERR_R              16    //EEPROM��ֵ���ϻָ�
#define R_SLERR_R                  17    //��ѹ����ϻָ�
#define R_ADERR_R                  18    //AD���ϻָ�
#define R_LOGICERR_R               19    //�߼����̹��Ϲ��ϻָ�
#define R_COMERR_R                 20    //ͨ��ͨ���쳣�ָ�
#define R_POWERERR                 21    //5V��Դ����
#define R_EEPROM_W_ERR             22    //EEPROMд�����
#define R_POWERERR_R               23    //5V��Դ���ϻָ�
#define R_RELDATAERR               24    //���������쳣
#define R_RELDATAERR_R             25    //���������쳣�ָ�
#define MAX_REPORT_INDEX           25    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     32    //��������

#endif