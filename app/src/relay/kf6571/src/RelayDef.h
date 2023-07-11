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
#define 	 FAILDATA_DISPTYPE_CFGNO 	 12 		 //���ϵ�����ʾ�����������
#define 	 LangTYPE_SYSSETNO 	 0 		 //�������͸�����ֵ���

//��ǰ��ֵ���Ŷ���-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //��ǰ��ֵ����

//��ֵȡֵ�궨��
//----��������0:+,1------
#define 	FORWORD 	0	//������
#define 	BACKWORD 	1	//������
//----����귽��0:+,1------
#define 	FORWORD 	0	//������
#define 	BACKWORD 	1	//������
//----ͨ������-----
#define 	ETHNET 	0	//��̫��ֱ��
#define 	SERIAL 	1	//��������
#define 	ROUTER 	2	//·������

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----���緽ʽ-----
#define 	AT1MODE 	0	//AT����
#define 	AT2MODE 	1	//��ר��ʽ

//��������ֵ����-----------------------
#define  RM_U1 0
extern LONG* 	RM_pnU1; 		 // U1
#define  RM_U2 1
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_U3 2
extern LONG* 	RM_pnU3; 		 // U3
#define  RM_U4 3
extern LONG* 	RM_pnU4; 		 // U4
#define  RM_IT1 4
extern LONG* 	RM_pnIt1; 		 // It1
#define  RM_IF1 5
extern LONG* 	RM_pnIf1; 		 // If1
#define  RM_IT2 6
extern LONG* 	RM_pnIt2; 		 // It2
#define  RM_IF2 7
extern LONG* 	RM_pnIf2; 		 // If2
#define  RM_IT3 8
extern LONG* 	RM_pnIt3; 		 // It3
#define  RM_IF3 9
extern LONG* 	RM_pnIf3; 		 // If3
#define  RM_IAT1 10
extern LONG* 	RM_pnIat1; 		 // Iat1
#define  RM_IAT2 11
extern LONG* 	RM_pnIat2; 		 // Iat2
#define  RM_SSIAT 12
extern LONG* 	RM_pnSSIAT; 		 // IAT
#define  RM_U1A 13
extern LONG* 	RM_pnU1A; 		 // AU1
#define  RM_U2A 14
extern LONG* 	RM_pnU2A; 		 // AU2
#define  RM_U3A 15
extern LONG* 	RM_pnU3A; 		 // AU3
#define  RM_U4A 16
extern LONG* 	RM_pnU4A; 		 // AU4
#define  RM_IT1A 17
extern LONG* 	RM_pnIT1A; 		 // AIT1
#define  RM_IF1A 18
extern LONG* 	RM_pnIF1A; 		 // AIF1
#define  RM_IT2A 19
extern LONG* 	RM_pnIT2A; 		 // AIT2
#define  RM_IF2A 20
extern LONG* 	RM_pnIF2A; 		 // AIF2
#define  RM_IT3A 21
extern LONG* 	RM_pnIT3A; 		 // AIT3
#define  RM_IF3A 22
extern LONG* 	RM_pnIF3A; 		 // AIF3
#define  RM_BOBRCH1 23
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 24
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 25
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 26
extern LONG* 	RM_pnBOINCH2; 		 // IN2
#define  RM_SUB1_U1 27
extern LONG* 	RM_pnSub1_U1; 		 // U1
#define  RM_SUB1_U2 28
extern LONG* 	RM_pnSub1_U2; 		 // U2
#define  RM_SUB1_U3 29
extern LONG* 	RM_pnSub1_U3; 		 // U3
#define  RM_SUB1_U4 30
extern LONG* 	RM_pnSub1_U4; 		 // U4
#define  RM_SUB1_IT1 31
extern LONG* 	RM_pnSub1_IT1; 		 // IT1
#define  RM_SUB1_IF1 32
extern LONG* 	RM_pnSub1_IF1; 		 // IF1
#define  RM_SUB1_IT2 33
extern LONG* 	RM_pnSub1_IT2; 		 // IT2
#define  RM_SUB1_IF2 34
extern LONG* 	RM_pnSub1_IF2; 		 // IF2
#define  RM_SUB1_IT3 35
extern LONG* 	RM_pnSub1_IT3; 		 // IT3
#define  RM_SUB1_IF3 36
extern LONG* 	RM_pnSub1_IF3; 		 // IF3
#define  RM_SUB1_IAT1 37
extern LONG* 	RM_pnSub1_IAT1; 		 // IAT1
#define  RM_SUB1_IAT2 38
extern LONG* 	RM_pnSub1_IAT2; 		 // IAT2
#define  RM_SUB1_IAT 39
extern LONG* 	RM_pnSub1_IAT; 		 // IAT
#define  RM_SUB1_AIT1 40
extern LONG* 	RM_pnSub1_AIT1; 		 // AIT1
#define  RM_SUB1_AIF1 41
extern LONG* 	RM_pnSub1_AIF1; 		 // AIF1
#define  RM_SUB1_AIT2 42
extern LONG* 	RM_pnSub1_AIT2; 		 // AIT2
#define  RM_SUB1_AIF2 43
extern LONG* 	RM_pnSub1_AIF2; 		 // AIF2
#define  RM_SUB1_AIT3 44
extern LONG* 	RM_pnSub1_AIT3; 		 // AIT3
#define  RM_SUB1_AIF3 45
extern LONG* 	RM_pnSub1_AIF3; 		 // AIF3
#define  RM_SUB1_STATUS 46
extern LONG* 	RM_pnSub1_Status; 		 // ST
#define  RM_SUB1_YEAR 47
extern LONG* 	RM_pnSub1_year; 		 // Year
#define  RM_SUB1_MONTH 48
extern LONG* 	RM_pnSub1_month; 		 // Mon.
#define  RM_SUB1_DAY 49
extern LONG* 	RM_pnSub1_day; 		 // Day
#define  RM_SUB1_HOUR 50
extern LONG* 	RM_pnSub1_hour; 		 // Hour
#define  RM_SUB1_MINUTE 51
extern LONG* 	RM_pnSub1_minute; 		 // Min.
#define  RM_SUB1_NS 52
extern LONG* 	RM_pnSub1_NS; 		 // NS
#define  RM_SUB2_U1 53
extern LONG* 	RM_pnSub2_U1; 		 // U1
#define  RM_SUB2_U2 54
extern LONG* 	RM_pnSub2_U2; 		 // U2
#define  RM_SUB2_U3 55
extern LONG* 	RM_pnSub2_U3; 		 // U3
#define  RM_SUB2_U4 56
extern LONG* 	RM_pnSub2_U4; 		 // U4
#define  RM_SUB2_IT1 57
extern LONG* 	RM_pnSub2_IT1; 		 // IT1
#define  RM_SUB2_IF1 58
extern LONG* 	RM_pnSub2_IF1; 		 // IF1
#define  RM_SUB2_IT2 59
extern LONG* 	RM_pnSub2_IT2; 		 // IT2
#define  RM_SUB2_IF2 60
extern LONG* 	RM_pnSub2_IF2; 		 // IF2
#define  RM_SUB2_IT3 61
extern LONG* 	RM_pnSub2_IT3; 		 // IT3
#define  RM_SUB2_IF3 62
extern LONG* 	RM_pnSub2_IF3; 		 // IF3
#define  RM_SUB2_IAT1 63
extern LONG* 	RM_pnSub2_IAT1; 		 // IAT1
#define  RM_SUB2_IAT2 64
extern LONG* 	RM_pnSub2_IAT2; 		 // IAT2
#define  RM_SUB2_IAT 65
extern LONG* 	RM_pnSub2_IAT; 		 // IAT
#define  RM_SUB2_AIT1 66
extern LONG* 	RM_pnSub2_AIT1; 		 // AIT1
#define  RM_SUB2_AIF1 67
extern LONG* 	RM_pnSub2_AIF1; 		 // AIF1
#define  RM_SUB2_AIT2 68
extern LONG* 	RM_pnSub2_AIT2; 		 // AIT2
#define  RM_SUB2_AIF2 69
extern LONG* 	RM_pnSub2_AIF2; 		 // AIF2
#define  RM_SUB2_AIT3 70
extern LONG* 	RM_pnSub2_AIT3; 		 // AIT3
#define  RM_SUB2_AIF3 71
extern LONG* 	RM_pnSub2_AIF3; 		 // AIF3
#define  RM_SUB2_STATUS 72
extern LONG* 	RM_pnSub2_Status; 		 // ST
#define  RM_SUB2_YEAR 73
extern LONG* 	RM_pnSub2_year; 		 // Year
#define  RM_SUB2_MONTH 74
extern LONG* 	RM_pnSub2_month; 		 // Mon.
#define  RM_SUB2_DAY 75
extern LONG* 	RM_pnSub2_day; 		 // Day
#define  RM_SUB2_HOUR 76
extern LONG* 	RM_pnSub2_hour; 		 // Hour
#define  RM_SUB2_MINUTE 77
extern LONG* 	RM_pnSub2_minute; 		 // Min.
#define  RM_SUB2_NS 78
extern LONG* 	RM_pnSub2_NS; 		 // NS
#define  RM_SUB3_U1 79
extern LONG* 	RM_pnSub3_U1; 		 // U1
#define  RM_SUB3_U2 80
extern LONG* 	RM_pnSub3_U2; 		 // U2
#define  RM_SUB3_U3 81
extern LONG* 	RM_pnSub3_U3; 		 // U3
#define  RM_SUB3_U4 82
extern LONG* 	RM_pnSub3_U4; 		 // U4
#define  RM_SUB3_IT1 83
extern LONG* 	RM_pnSub3_IT1; 		 // IT1
#define  RM_SUB3_IF1 84
extern LONG* 	RM_pnSub3_IF1; 		 // IF1
#define  RM_SUB3_IT2 85
extern LONG* 	RM_pnSub3_IT2; 		 // IT2
#define  RM_SUB3_IF2 86
extern LONG* 	RM_pnSub3_IF2; 		 // IF2
#define  RM_SUB3_IT3 87
extern LONG* 	RM_pnSub3_IT3; 		 // IT3
#define  RM_SUB3_IF3 88
extern LONG* 	RM_pnSub3_IF3; 		 // IF3
#define  RM_SUB3_IAT1 89
extern LONG* 	RM_pnSub3_IAT1; 		 // IAT1
#define  RM_SUB3_IAT2 90
extern LONG* 	RM_pnSub3_IAT2; 		 // IAT2
#define  RM_SUB3_IAT 91
extern LONG* 	RM_pnSub3_IAT; 		 // IAT
#define  RM_SUB3_AIT1 92
extern LONG* 	RM_pnSub3_AIT1; 		 // AIT1
#define  RM_SUB3_AIF1 93
extern LONG* 	RM_pnSub3_AIF1; 		 // AIF1
#define  RM_SUB3_AIT2 94
extern LONG* 	RM_pnSub3_AIT2; 		 // AIT2
#define  RM_SUB3_AIF2 95
extern LONG* 	RM_pnSub3_AIF2; 		 // AIF2
#define  RM_SUB3_AIT3 96
extern LONG* 	RM_pnSub3_AIT3; 		 // AIT3
#define  RM_SUB3_AIF3 97
extern LONG* 	RM_pnSub3_AIF3; 		 // AIF3
#define  RM_SUB3_STATUS 98
extern LONG* 	RM_pnSub3_Status; 		 // ST
#define  RM_SUB3_YEAR 99
extern LONG* 	RM_pnSub3_year; 		 // Year
#define  RM_SUB3_MONTH 100
extern LONG* 	RM_pnSub3_month; 		 // Mon.
#define  RM_SUB3_DAY 101
extern LONG* 	RM_pnSub3_day; 		 // Day
#define  RM_SUB3_HOUR 102
extern LONG* 	RM_pnSub3_hour; 		 // Hour
#define  RM_SUB3_MINUTE 103
extern LONG* 	RM_pnSub3_minute; 		 // Min.
#define  RM_SUB3_NS 104
extern LONG* 	RM_pnSub3_NS; 		 // NS
#define  RM_SUB4_U1 105
extern LONG* 	RM_pnSub4_U1; 		 // U1
#define  RM_SUB4_U2 106
extern LONG* 	RM_pnSub4_U2; 		 // U2
#define  RM_SUB4_U3 107
extern LONG* 	RM_pnSub4_U3; 		 // U3
#define  RM_SUB4_U4 108
extern LONG* 	RM_pnSub4_U4; 		 // U4
#define  RM_SUB4_IT1 109
extern LONG* 	RM_pnSub4_IT1; 		 // IT1
#define  RM_SUB4_IF1 110
extern LONG* 	RM_pnSub4_IF1; 		 // IF1
#define  RM_SUB4_IT2 111
extern LONG* 	RM_pnSub4_IT2; 		 // IT2
#define  RM_SUB4_IF2 112
extern LONG* 	RM_pnSub4_IF2; 		 // IF2
#define  RM_SUB4_IT3 113
extern LONG* 	RM_pnSub4_IT3; 		 // IT3
#define  RM_SUB4_IF3 114
extern LONG* 	RM_pnSub4_IF3; 		 // IF3
#define  RM_SUB4_IAT1 115
extern LONG* 	RM_pnSub4_IAT1; 		 // IAT1
#define  RM_SUB4_IAT2 116
extern LONG* 	RM_pnSub4_IAT2; 		 // IAT2
#define  RM_SUB4_IAT 117
extern LONG* 	RM_pnSub4_IAT; 		 // IAT
#define  RM_SUB4_AIT1 118
extern LONG* 	RM_pnSub4_AIT1; 		 // AIT1
#define  RM_SUB4_AIF1 119
extern LONG* 	RM_pnSub4_AIF1; 		 // AIF1
#define  RM_SUB4_AIT2 120
extern LONG* 	RM_pnSub4_AIT2; 		 // AIT2
#define  RM_SUB4_AIF2 121
extern LONG* 	RM_pnSub4_AIF2; 		 // AIF2
#define  RM_SUB4_AIT3 122
extern LONG* 	RM_pnSub4_AIT3; 		 // AIT3
#define  RM_SUB4_AIF3 123
extern LONG* 	RM_pnSub4_AIF3; 		 // AIF3
#define  RM_SUB4_STATUS 124
extern LONG* 	RM_pnSub4_Status; 		 // ST
#define  RM_SUB4_YEAR 125
extern LONG* 	RM_pnSub4_year; 		 // Year
#define  RM_SUB4_MONTH 126
extern LONG* 	RM_pnSub4_month; 		 // Mon.
#define  RM_SUB4_DAY 127
extern LONG* 	RM_pnSub4_day; 		 // Day
#define  RM_SUB4_HOUR 128
extern LONG* 	RM_pnSub4_hour; 		 // Hour
#define  RM_SUB4_MINUTE 129
extern LONG* 	RM_pnSub4_minute; 		 // Min.
#define  RM_SUB4_NS 130
extern LONG* 	RM_pnSub4_NS; 		 // NS
#define  RM_SUB5_U1 131
extern LONG* 	RM_pnSub5_U1; 		 // U1
#define  RM_SUB5_U2 132
extern LONG* 	RM_pnSub5_U2; 		 // U2
#define  RM_SUB5_U3 133
extern LONG* 	RM_pnSub5_U3; 		 // U3
#define  RM_SUB5_U4 134
extern LONG* 	RM_pnSub5_U4; 		 // U4
#define  RM_SUB5_IT1 135
extern LONG* 	RM_pnSub5_IT1; 		 // IT1
#define  RM_SUB5_IF1 136
extern LONG* 	RM_pnSub5_IF1; 		 // IF1
#define  RM_SUB5_IT2 137
extern LONG* 	RM_pnSub5_IT2; 		 // IT2
#define  RM_SUB5_IF2 138
extern LONG* 	RM_pnSub5_IF2; 		 // IF2
#define  RM_SUB5_IT3 139
extern LONG* 	RM_pnSub5_IT3; 		 // IT3
#define  RM_SUB5_IF3 140
extern LONG* 	RM_pnSub5_IF3; 		 // IF3
#define  RM_SUB5_IAT1 141
extern LONG* 	RM_pnSub5_IAT1; 		 // IAT1
#define  RM_SUB5_IAT2 142
extern LONG* 	RM_pnSub5_IAT2; 		 // IAT2
#define  RM_SUB5_IAT 143
extern LONG* 	RM_pnSub5_IAT; 		 // IAT
#define  RM_SUB5_AIT1 144
extern LONG* 	RM_pnSub5_AIT1; 		 // AIT1
#define  RM_SUB5_AIF1 145
extern LONG* 	RM_pnSub5_AIF1; 		 // AIF1
#define  RM_SUB5_AIT2 146
extern LONG* 	RM_pnSub5_AIT2; 		 // AIT2
#define  RM_SUB5_AIF2 147
extern LONG* 	RM_pnSub5_AIF2; 		 // AIF2
#define  RM_SUB5_AIT3 148
extern LONG* 	RM_pnSub5_AIT3; 		 // AIT3
#define  RM_SUB5_AIF3 149
extern LONG* 	RM_pnSub5_AIF3; 		 // AIF3
#define  RM_SUB5_STATUS 150
extern LONG* 	RM_pnSub5_Status; 		 // ST
#define  RM_SUB5_YEAR 151
extern LONG* 	RM_pnSub5_year; 		 // Year
#define  RM_SUB5_MONTH 152
extern LONG* 	RM_pnSub5_month; 		 // Mon.
#define  RM_SUB5_DAY 153
extern LONG* 	RM_pnSub5_day; 		 // Day
#define  RM_SUB5_HOUR 154
extern LONG* 	RM_pnSub5_hour; 		 // Hour
#define  RM_SUB5_MINUTE 155
extern LONG* 	RM_pnSub5_minute; 		 // Min.
#define  RM_SUB5_NS 156
extern LONG* 	RM_pnSub5_NS; 		 // NS
#define  RM_DIST 157
extern LONG* 	RM_pnDist; 		 // D
#define  RM_KMS 158
extern LONG* 	RM_pnKMS; 		 // KMS
#define  RM_R_T 159
extern LONG* 	RM_pnR_T; 		 // R_T
#define  RM_X_T 160
extern LONG* 	RM_pnX_T; 		 // X_T
#define  RM_R_F 161
extern LONG* 	RM_pnR_F; 		 // R_F
#define  RM_X_F 162
extern LONG* 	RM_pnX_F; 		 // X_F
#define  RM_R_TF 163
extern LONG* 	RM_pnR_TF; 		 // R_TF
#define  RM_X_TF 164
extern LONG* 	RM_pnX_TF; 		 // X_TF
#define  RM_NS 165
extern LONG* 	RM_pnNS; 		 // NS
#define  RM_IP_H2 166
extern LONG* 	RM_pnIP_H2; 		 // IPh2
#define  RM_IP_H3 167
extern LONG* 	RM_pnIP_H3; 		 // IPh3
#define  RM_IP_H5 168
extern LONG* 	RM_pnIP_H5; 		 // IPh5
#define  RM_UP 169
extern LONG* 	RM_pnUp; 		 // Up
#define  RM_I1 170
extern LONG* 	RM_pnI1; 		 // I1
#define  RM_I2 171
extern LONG* 	RM_pnI2; 		 // I2
#define  RM_I3 172
extern LONG* 	RM_pnI3; 		 // I3

#define RM_COUNT     173    //��������ֵ����
#define RM_BUF_SIZE     692    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------

#define MEASURE_COUNT     0    //����ֵ����
#define MEASURE_BUF_SIZE     0    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 696)

//��ѹ��----------------------------------
extern BYTE* 	SW_pbyFL; 		 //���ϲ��
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyRESD1; 		 //����ѹ��1
extern BYTE* 	SW_pbyRESD2; 		 //����ѹ��2

#define SW_COUNT     4    //��ѹ�����
#define SW_BUF_SIZE     4    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 696)

//����----------------------------------
extern BYTE* 	CFG_pby21_PI; 		 //�迹����
extern BYTE* 	CFG_pby50_PI; 		 //��������
extern BYTE* 	CFG_pby50DI_PI; 		 //��������
extern BYTE* 	CFG_pbyFL_AUTO; 		 //�������Ӧ
extern BYTE* 	CFG_pbyCCMODE; 		 //�����ϳ�
extern BYTE* 	CFG_pbyAT_C_RATE; 		 //���ϵ�����
extern BYTE* 	CFG_pbyUD_C_RATE; 		 //�����е�����
extern BYTE* 	CFG_pbyNL_C_RATE; 		 //����������
extern BYTE* 	CFG_pbyPHASESC; 		 //����·�ж�
extern BYTE* 	CFG_pbyNONEAT; 		 //�������AT
extern BYTE* 	CFG_pbyISSS; 		 //����Ϊ�����
extern BYTE* 	CFG_pbyPARALLG; 		 //ƽ���ı���
extern BYTE* 	CFG_pby21_H_S; 		 //����г������
extern BYTE* 	CFG_pby21_H_L; 		 //����г������
extern BYTE* 	CFG_pby50_H_S; 		 //����г������
extern BYTE* 	CFG_pby50_H_L; 		 //����г������
extern BYTE* 	CFG_pby50_27V; 		 //����������ѹ����
extern BYTE* 	CFG_pbyCALLDATA; 		 //�ٲ�ȡ��ѹ����
extern BYTE* 	CFG_pbyCHECKGPS; 		 //GPS���ϼ��
extern BYTE* 	CFG_pbyCHECKCOM; 		 //ͨ���Զ����
extern BYTE* 	CFG_pbyCROSSAERA; 		 //����Խ��
extern BYTE* 	CFG_pbyRESD1; 		 //��������1
extern BYTE* 	CFG_pbyRESD2; 		 //��������2

#define CFG_COUNT     23    //���ø���/
#define CFG_BUF_SIZE     23    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 700)

//��ֵ----------------------------------
extern LONG* 	RS_pnCFG; 		 //������ CFG
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn21R; 		 //������������ 21R
extern LONG* 	RS_pn21X; 		 //���������翹 21X
extern LONG* 	RS_pn78LA; 		 //��·�迹�� 78LA
extern LONG* 	RS_pn50C; 		 //������ֵ 50C
extern LONG* 	RS_pn50_27V; 		 //����������ѹ������ֵ 50_27V
extern LONG* 	RS_pn50DC; 		 //������������������ֵ 50DC
extern LONG* 	RS_pn50D_37C; 		 //��������С�������� 50D_37C
extern LONG* 	RS_pnFLT; 		 //�ʱ�������ʱ FLT
extern LONG* 	RS_pnKh1; 		 //����г������ϵ�� Kh1
extern LONG* 	RS_pnKhr1; 		 //�������г����Ȩ Khr1
extern LONG* 	RS_pnKhr2; 		 //��������г����Ȩ Khr2
extern LONG* 	RS_pn27Uab; 		 //����·��ѹ��ֵ 27Uab
extern LONG* 	RS_pn27U; 		 //��ѹ��¼���ݶ�ֵ 27U
extern LONG* 	RS_pn27D; 		 //��ѹ���ݼ�¼��ʱ 27D
extern LONG* 	RS_pnN; 		 //�������� N
extern LONG* 	RS_pnNID; 		 //����վ�� NID
extern LONG* 	RS_pnD1; 		 //AT����1���� D1
extern LONG* 	RS_pnD2; 		 //AT����2���� D2
extern LONG* 	RS_pnD3; 		 //AT����3���� D3
extern LONG* 	RS_pnD4; 		 //AT����4���� D4
extern LONG* 	RS_pnD5; 		 //AT����5���� D5
extern LONG* 	RS_pnQss; 		 //�����AT©�� Qss
extern LONG* 	RS_pnQ1; 		 //����1AT©�� Q1
extern LONG* 	RS_pnQ2; 		 //����2AT©�� Q2
extern LONG* 	RS_pnQ3; 		 //����3AT©�� Q3
extern LONG* 	RS_pnQ4; 		 //����4AT©�� Q4
extern LONG* 	RS_pnQ5; 		 //����5AT©�� Q5
extern LONG* 	RS_pnKss; 		 //����������ֲ�ϵ�� Kss
extern LONG* 	RS_pnK1; 		 //����1AT�����ֲ�ϵ�� K1
extern LONG* 	RS_pnK2; 		 //����2AT�����ֲ�ϵ�� K2
extern LONG* 	RS_pnK3; 		 //����3AT�����ֲ�ϵ�� K3
extern LONG* 	RS_pnK4; 		 //����4AT�����ֲ�ϵ�� K4
extern LONG* 	RS_pnK5; 		 //����5AT�����ֲ�ϵ�� K5
extern LONG* 	RS_pnKF; 		 //�����ж�ϵ�� KF
extern LONG* 	RS_pnLa; 		 //����۳��� La
extern LONG* 	RS_pnLc; 		 //�������� Lc
extern LONG* 	RS_pnLD; 		 //��������0:+,1- LD
extern LONG* 	RS_pnS0; 		 //���������� S0
extern LONG* 	RS_pnSD; 		 //����귽��0:+,1- SD
extern LONG* 	RS_pnZOFFSET; 		 //ƽ���ı���ƫ���迹 ZOFFSET
extern LONG* 	RS_pnAOFFSET; 		 //�迹�ı���ƫ�ƽǶ� AOFFSET
extern LONG* 	RS_pnChanType; 		 //ͨ������ ChanType
extern LONG* 	RS_pnCAID; 		 //Խ��ȫ�ֱ�� CAID
extern LONG* 	RS_pnT21FLN; 		 //T�߲��ֶ��� T21FLN
extern LONG* 	RS_pnT21FLN1L; 		 //T�ߵ�һ�ξ��� T21FLN1L
extern LONG* 	RS_pnT21FLN1X; 		 //T�ߵ�һ�ε�λ�翹 T21FLN1X
extern LONG* 	RS_pnT21FLN2L; 		 //T�ߵڶ��ξ��� T21FLN2L
extern LONG* 	RS_pnT21FLN2X; 		 //T�ߵڶ��ε�λ�翹 T21FLN2X
extern LONG* 	RS_pnT21FLN3L; 		 //T�ߵ����ξ��� T21FLN3L
extern LONG* 	RS_pnT21FLN3X; 		 //T�ߵ����ε�λ�翹 T21FLN3X
extern LONG* 	RS_pnT21FLN4L; 		 //T�ߵ��Ķξ��� T21FLN4L
extern LONG* 	RS_pnT21FLN4X; 		 //T�ߵ��Ķε�λ�翹 T21FLN4X
extern LONG* 	RS_pnT21FLN5L; 		 //T�ߵ���ξ��� T21FLN5L
extern LONG* 	RS_pnT21FLN5X; 		 //T�ߵ���ε�λ�翹 T21FLN5X
extern LONG* 	RS_pnT21FLN6L; 		 //T�ߵ����ξ��� T21FLN6L
extern LONG* 	RS_pnT21FLN6X; 		 //T�ߵ����ε�λ�翹 T21FLN6X
extern LONG* 	RS_pnF21FLN; 		 //F�߲��ֶ��� F21FLN
extern LONG* 	RS_pnF21FLN1L; 		 //F�ߵ�һ�ξ��� F21FLN1L
extern LONG* 	RS_pnF21FLN1X; 		 //F�ߵ�һ�ε�λ�翹 F21FLN1X
extern LONG* 	RS_pnF21FLN2L; 		 //F�ߵڶ��ξ��� F21FLN2L
extern LONG* 	RS_pnF21FLN2X; 		 //F�ߵڶ��ε�λ�翹 F21FLN2X
extern LONG* 	RS_pnF21FLN3L; 		 //F�ߵ����ξ��� F21FLN3L
extern LONG* 	RS_pnF21FLN3X; 		 //F�ߵ����ε�λ�翹 F21FLN3X
extern LONG* 	RS_pnF21FLN4L; 		 //F�ߵ��Ķξ��� F21FLN4L
extern LONG* 	RS_pnF21FLN4X; 		 //F�ߵ��Ķε�λ�翹 F21FLN4X
extern LONG* 	RS_pnF21FLN5L; 		 //F�ߵ���ξ��� F21FLN5L
extern LONG* 	RS_pnF21FLN5X; 		 //F�ߵ���ε�λ�翹 F21FLN5X
extern LONG* 	RS_pnF21FLN6L; 		 //F�ߵ����ξ��� F21FLN6L
extern LONG* 	RS_pnF21FLN6X; 		 //F�ߵ����ε�λ�翹 F21FLN6X
extern LONG* 	RS_pnTF21FLN; 		 //TF���ֶ��� TF21FLN
extern LONG* 	RS_pnTF21FLN1L; 		 //TF����һ�ξ��� TF21FLN1L
extern LONG* 	RS_pnTF21FLN1X; 		 //TF���һ�ε�λ�翹 TF21FLN1X
extern LONG* 	RS_pnTF21FLN2L; 		 //TF���ڶ��ξ��� TF21FLN2L
extern LONG* 	RS_pnTF21FLN2X; 		 //TF�����ε�λ�翹 TF21FLN2X
extern LONG* 	RS_pnTF21FLN3L; 		 //TF�������ξ��� TF21FLN3L
extern LONG* 	RS_pnTF21FLN3X; 		 //TF������ε�λ�翹 TF21FLN3X
extern LONG* 	RS_pnTF21FLN4L; 		 //TF�����Ķξ��� TF21FLN4L
extern LONG* 	RS_pnTF21FLN4X; 		 //TF����Ķε�λ�翹 TF21FLN4X
extern LONG* 	RS_pnTF21FLN5L; 		 //TF������ξ��� TF21FLN5L
extern LONG* 	RS_pnTF21FLN5X; 		 //TF�����ε�λ�翹 TF21FLN5X
extern LONG* 	RS_pnTF21FLN6L; 		 //TF�������ξ��� TF21FLN6L
extern LONG* 	RS_pnTF21FLN6X; 		 //TF������ε�λ�翹 TF21FLN6X

#define RS_COUNT     84    //��ֵ����/
#define RS_BUF_SIZE     336    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 726)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnPM; 		 //���緽ʽ
extern LONG* 	AS_pnT1PI; 		 //T1������ʱ
extern LONG* 	AS_pnT1RE; 		 //T1������ʱ
extern LONG* 	AS_pnT2PI; 		 //T2������ʱ
extern LONG* 	AS_pnT2RE; 		 //T2������ʱ
extern LONG* 	AS_pnT3PI; 		 //T3������ʱ
extern LONG* 	AS_pnT3RE; 		 //T3������ʱ
extern LONG* 	AS_pnT4PI; 		 //T4������ʱ
extern LONG* 	AS_pnT4RE; 		 //T4������ʱ
extern LONG* 	AS_pnT5PI; 		 //T5������ʱ
extern LONG* 	AS_pnT5RE; 		 //T5������ʱ
extern LONG* 	AS_pnT6PI; 		 //T6������ʱ
extern LONG* 	AS_pnT6RE; 		 //T6������ʱ
extern LONG* 	AS_pnT7PI; 		 //T7������ʱ
extern LONG* 	AS_pnT7RE; 		 //T7������ʱ
extern LONG* 	AS_pnT8PI; 		 //T8������ʱ
extern LONG* 	AS_pnT8RE; 		 //T8������ʱ
extern LONG* 	AS_pnT9PI; 		 //T9������ʱ
extern LONG* 	AS_pnT9RE; 		 //T9������ʱ
extern LONG* 	AS_pnT10PI; 		 //T10������ʱ
extern LONG* 	AS_pnT10RE; 		 //T10������ʱ
extern LONG* 	AS_pnT11PI; 		 //T11������ʱ
extern LONG* 	AS_pnT11RE; 		 //T11������ʱ
extern LONG* 	AS_pnT12PI; 		 //T12������ʱ
extern LONG* 	AS_pnT12RE; 		 //T12������ʱ
extern LONG* 	AS_pnT13PI; 		 //T13������ʱ
extern LONG* 	AS_pnT13RE; 		 //T13������ʱ
extern LONG* 	AS_pnT14PI; 		 //T14������ʱ
extern LONG* 	AS_pnT14RE; 		 //T14������ʱ
extern LONG* 	AS_pnT15PI; 		 //T15������ʱ
extern LONG* 	AS_pnT15RE; 		 //T15������ʱ
extern LONG* 	AS_pnT16PI; 		 //T16������ʱ
extern LONG* 	AS_pnT16RE; 		 //T16������ʱ
extern LONG* 	AS_pnTCB; 		 //��·��������ʱ
extern LONG* 	AS_pnTSW; 		 //����������ʱ
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pn50I1; 		 //I1������ֵ
extern LONG* 	AS_pn37I1; 		 //I1������ֵ
extern LONG* 	AS_pn50I2; 		 //I2������ֵ
extern LONG* 	AS_pn37I2; 		 //I2������ֵ
extern LONG* 	AS_pn50I3; 		 //I3������ֵ
extern LONG* 	AS_pn37I3; 		 //I3������ֵ
extern LONG* 	AS_pn59U1; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn27U1; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn59U2; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn27U2; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn59U3; 		 //U3��ѹ��ֵ
extern LONG* 	AS_pn27U3; 		 //U3��ѹ��ֵ
extern LONG* 	AS_pn59U4; 		 //U4��ѹ��ֵ
extern LONG* 	AS_pn27U4; 		 //U4��ѹ��ֵ
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnUPTRatio; 		 //U���
extern LONG* 	AS_pnI1CTRatio; 		 //I1���
extern LONG* 	AS_pnI2CTRatio; 		 //I2���
extern LONG* 	AS_pnI3CTRatio; 		 //I3���
extern LONG* 	AS_pnIATCTRatio; 		 //IAT���
extern LONG* 	AS_pnZRatio; 		 //�迹���
extern LONG* 	AS_pnRT; 		 //RT

#define AS_RT_NUM     7    //������ֵ�б�ȸ���

#define AS_COUNT     60    //������ֵ����
#define AS_BUF_SIZE     240    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 1062)

//ϵ��----------------------------------
extern LONG* 	CF_pnU1; 		 //U1
extern LONG* 	CF_pnU2; 		 //U2
extern LONG* 	CF_pnU3; 		 //U3
extern LONG* 	CF_pnU4; 		 //U4
extern LONG* 	CF_pnIT1; 		 //IT1
extern LONG* 	CF_pnIF1; 		 //IF1
extern LONG* 	CF_pnIT2; 		 //IT2
extern LONG* 	CF_pnIF2; 		 //IF2
extern LONG* 	CF_pnIT3; 		 //IT3
extern LONG* 	CF_pnIF3; 		 //IF3
extern LONG* 	CF_pnIAT1; 		 //IAT1
extern LONG* 	CF_pnIAT2; 		 //IAT2
extern LONG* 	CF_pnZ; 		 //Z
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4

#define CF_COUNT     17    //ϵ������
#define CF_BUF_SIZE     68    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 1302)

//������ʺ�-----------------------
#define R_OEERR                     0    //�������ʧЧ
#define R_OEBR                      1    //�����������
#define R_ADERR                     2    //AD����
#define R_SLERR                     3    //��ѹ�����
#define R_EEPROMERR                 4    //EEPROM��ֵ����
#define R_RAMERR                    5    //RAM��ֵ����
#define R_ROMERR                    6    //����ROM����
#define R_RESETAN                   7    //װ���쳣��λ
#define R_COMERR                    8    //ͨ��ͨ���쳣
#define R_OUTRAMERR                 9    //�ⲿRAM����
#define R_NVRAMERR                 10    //NVRAM���ݳ���
#define R_LOGICERR                 11    //�߼����̹���
#define R_RCERR                    12    //�����Լ����
#define R_POWERERR                 13    //5V��Դ����
#define R_EEPROM_W_ERR             14    //EEPROMд�����
#define R_ADERR_R                  15    //AD���ϻָ�
#define R_SLERR_R                  16    //��ѹ����ϻָ�
#define R_EEPROMERR_R              17    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 18    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 19    //����ROM���ϻָ�
#define R_COMERR_R                 20    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               21    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               22    //�߼����̹��Ϲ��ϻָ�
#define R_POWERERR_R               23    //5V��Դ���ϻָ�
#define R_RELDATAERR_R             24    //���������쳣�ָ�
#define R_ACTPICK                  25    //��������
#define R_FLPICKUP                 26    //���ϲ������
#define R_TRBREAK_F                27    //����T-R����
#define R_FRBREAK_F                28    //����F-R����
#define R_TRBREAK_B                29    //����T-R����
#define R_FRBREAK_B                30    //����F-R����
#define R_TFBREAK                  31    //T-F����
#define R_SUB1GPSERR               32    //����1GPS����
#define R_SUB2GPSERR               33    //����2GPS����
#define R_SUB3GPSERR               34    //����3GPS����
#define R_SUB4GPSERR               35    //����4GPS����
#define R_SUB5GPSERR               36    //����5GPS����
#define R_FLACT                    37    //������ද��
#define R_CHANNELOK                38    //ͨ�����Գɹ�
#define R_CHANNELERR               39    //ͨ������
#define R_CHANNEL1ERR              40    //ͨ��1����
#define R_CHANNEL2ERR              41    //ͨ��2����
#define R_CHANNEL3ERR              42    //ͨ��3����
#define R_CHANNEL4ERR              43    //ͨ��4����
#define R_CHANNEL5ERR              44    //ͨ��5����
#define R_PHASEBR                  45    //������
#define R_GPSERR                   46    //GPS����
#define R_FLFAIL_CHANNEL           47    //ͨ�����ϲ��ʧ��
#define R_TIMEOUT                  48    //��ʱ���ϲ��ʧ��
#define R_CALLACT                  49    //�ٲ��ද��
#define R_PTALARM                  50    //PT����
#define R_COMTEST                  51    //ͨ�Ų���
#define R_SUB1DATA                 52    //����1��������
#define R_SUB2DATA                 53    //����2��������
#define R_SUB3DATA                 54    //����3��������
#define R_SUB4DATA                 55    //����4��������
#define R_SUB5DATA                 56    //����5��������
#define R_GPSFAULT                 57    //GPS���ϲ��ʧ��
#define R_SUB1LOSS                 58    //����1���ݶ�ʧ
#define R_SUB2LOSS                 59    //����2���ݶ�ʧ
#define R_SUB3LOSS                 60    //����3���ݶ�ʧ
#define R_SUB4LOSS                 61    //����4���ݶ�ʧ
#define R_SUB5LOSS                 62    //����5���ݶ�ʧ
#define R_FLFAIL                   63    //���ϲ��ʧ��
#define R_GPSFAULT_UD              64    //GPS���ϲ��ʧ��
#define R_STEPSUCC                 65    //Խ���л��ɹ�
#define R_STEPFAIL                 66    //Խ���л�ʧ��
#define R_TIMEOUT_UD               67    //��ʱ���ϲ��ʧ��
#define R_TR_F_X                   68    //����TR����-�翹���
#define R_FR_F_X                   69    //����FR����-�翹���
#define R_TR_B_X                   70    //����TR����-�翹���
#define R_FR_B_X                   71    //����FR����-�翹���
#define R_PICKUP                   72    //��������
#define R_TRBREAK_F_UD             73    //����T-R����
#define R_FRBREAK_F_UD             74    //����F-R����
#define R_TRBREAK_B_UD             75    //����T-R����
#define R_FRBREAK_B_UD             76    //����F-R����
#define R_TFBREAK_UD               77    //T-F����
#define R_COMTEST_UD               78    //ͨ�Ų���
#define R_FLFAIL_UD                79    //���ϲ��ʧ��
#define R_RELDATAERR               80    //���������쳣
#define R_UNKOWNMODE               81    //���緽ʽ�쳣
#define R_TFBREAK_F_UD             82    //����T-F����
#define R_TFBREAK_B_UD             83    //����T-F����
#define R_TFBREAK_F                84    //����T-F����
#define R_TFBREAK_B                85    //����T-F����
#define MAX_REPORT_INDEX           85    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     7    //��������

#endif