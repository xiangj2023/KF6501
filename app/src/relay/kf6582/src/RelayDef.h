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
#define  RM_UP1 0
extern LONG* 	RM_pnUp1; 		 // Up1
#define  RM_UP2 1
extern LONG* 	RM_pnUp2; 		 // Up2
#define  RM_UP3 2
extern LONG* 	RM_pnUp3; 		 // Up3
#define  RM_UP4 3
extern LONG* 	RM_pnUp4; 		 // Up4
#define  RM_UP5 4
extern LONG* 	RM_pnUp5; 		 // Up5
#define  RM_UP6 5
extern LONG* 	RM_pnUp6; 		 // Up6
#define  RM_UP7 6
extern LONG* 	RM_pnUp7; 		 // Up7
#define  RM_UP8 7
extern LONG* 	RM_pnUp8; 		 // Up8
#define  RM_IP1 8
extern LONG* 	RM_pnIp1; 		 // Ip1
#define  RM_IP2 9
extern LONG* 	RM_pnIp2; 		 // Ip2
#define  RM_IP3 10
extern LONG* 	RM_pnIp3; 		 // Ip3
#define  RM_IP4 11
extern LONG* 	RM_pnIp4; 		 // Ip4
#define  RM_IP5 12
extern LONG* 	RM_pnIp5; 		 // Ip5
#define  RM_IP6 13
extern LONG* 	RM_pnIp6; 		 // Ip6
#define  RM_IP7 14
extern LONG* 	RM_pnIp7; 		 // Ip7
#define  RM_IP8 15
extern LONG* 	RM_pnIp8; 		 // Ip8
#define  RM_BOBRCH1 16
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 17
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 18
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 19
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     20    //��������ֵ����
#define RM_BUF_SIZE     80    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UM1 0
extern LONG* 	M_pnUm1; 		 //Um1 Um1
#define  M_UM2 1
extern LONG* 	M_pnUm2; 		 //Um2 Um2
#define  M_UM3 2
extern LONG* 	M_pnUm3; 		 //Um3 Um3
#define  M_UM4 3
extern LONG* 	M_pnUm4; 		 //Um4 Um4
#define  M_UM5 4
extern LONG* 	M_pnUm5; 		 //Um5 Um5
#define  M_UM6 5
extern LONG* 	M_pnUm6; 		 //Um6 Um6
#define  M_UM7 6
extern LONG* 	M_pnUm7; 		 //Um7 Um7
#define  M_UM8 7
extern LONG* 	M_pnUm8; 		 //Um8 Um8
#define  M_IM1 8
extern LONG* 	M_pnIm1; 		 //Im1 Im1
#define  M_IM2 9
extern LONG* 	M_pnIm2; 		 //Im2 Im2
#define  M_IM3 10
extern LONG* 	M_pnIm3; 		 //Im3 Im3
#define  M_IM4 11
extern LONG* 	M_pnIm4; 		 //Im4 Im4
#define  M_IM5 12
extern LONG* 	M_pnIm5; 		 //Im5 Im5
#define  M_IM6 13
extern LONG* 	M_pnIm6; 		 //Im6 Im6
#define  M_IM7 14
extern LONG* 	M_pnIm7; 		 //Im7 Im7
#define  M_IM8 15
extern LONG* 	M_pnIm8; 		 //Im8 Im8

#define MEASURE_COUNT     16    //����ֵ����
#define MEASURE_BUF_SIZE     64    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 84)

//��ѹ��----------------------------------
extern BYTE* 	SW_pbyBZT; 		 //����Ͷ����
extern BYTE* 	SW_pbyZBBZT; 		 //���䱸��Ͷ
extern BYTE* 	SW_pbySYBZT; 		 //ʧѹ����Ͷ
extern BYTE* 	SW_pbyEARTHQK; 		 //������
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2
extern BYTE* 	SW_pbyBAK3; 		 //����ѹ��3
extern BYTE* 	SW_pbyBAK4; 		 //����ѹ��4

#define SW_COUNT     9    //��ѹ�����
#define SW_BUF_SIZE     9    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 148)

//����----------------------------------
extern BYTE* 	CFG_pbyBZT; 		 //����Ͷ����
extern BYTE* 	CFG_pbyREBZT; 		 //����Ͷ����Ͷ��
extern BYTE* 	CFG_pbyZBBZT; 		 //���䱸��Ͷ
extern BYTE* 	CFG_pbySYBZT; 		 //ʧѹ����Ͷ
extern BYTE* 	CFG_pbyZLYX; 		 //ֱ������
extern BYTE* 	CFG_pby74PT1; 		 //PT1���߼��
extern BYTE* 	CFG_pby74PT2; 		 //PT2���߼��
extern BYTE* 	CFG_pby74PT3; 		 //PT3���߼��
extern BYTE* 	CFG_pby74PT4; 		 //PT4���߼��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2
extern BYTE* 	CFG_pbyBAK3; 		 //��������3
extern BYTE* 	CFG_pbyBAK4; 		 //��������4

#define CFG_COUNT     13    //���ø���/
#define CFG_BUF_SIZE     13    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 158)

//��ֵ----------------------------------
extern LONG* 	RS_pnF; 		 //������ F
extern LONG* 	RS_pn59U1; 		 //U1��ѹ 59U1
extern LONG* 	RS_pn27U1; 		 //U1��ѹ 27U1
extern LONG* 	RS_pn59U2; 		 //U2��ѹ 59U2
extern LONG* 	RS_pn27U2; 		 //U2��ѹ 27U2
extern LONG* 	RS_pn59U3; 		 //U3��ѹ 59U3
extern LONG* 	RS_pn27U3; 		 //U3��ѹ 27U3
extern LONG* 	RS_pn59U4; 		 //U4��ѹ 59U4
extern LONG* 	RS_pn27U4; 		 //U4��ѹ 27U4
extern LONG* 	RS_pn59U5; 		 //U5��ѹ 59U5
extern LONG* 	RS_pn27U5; 		 //U5��ѹ 27U5
extern LONG* 	RS_pn59U6; 		 //U6��ѹ 59U6
extern LONG* 	RS_pn27U6; 		 //U6��ѹ 27U6
extern LONG* 	RS_pn59U7; 		 //U7��ѹ 59U7
extern LONG* 	RS_pn27U7; 		 //U7��ѹ 27U7
extern LONG* 	RS_pn59U8; 		 //U8��ѹ 59U8
extern LONG* 	RS_pn27U8; 		 //U8��ѹ 27U8
extern LONG* 	RS_pn50I1; 		 //I1���� 50I1
extern LONG* 	RS_pn37I1; 		 //I1���� 37I1
extern LONG* 	RS_pn50I2; 		 //I2���� 50I2
extern LONG* 	RS_pn37I2; 		 //I2���� 37I2
extern LONG* 	RS_pn50I3; 		 //I3���� 50I3
extern LONG* 	RS_pn37I3; 		 //I3���� 37I3
extern LONG* 	RS_pn50I4; 		 //I4���� 50I4
extern LONG* 	RS_pn37I4; 		 //I4���� 37I4
extern LONG* 	RS_pn50I5; 		 //I5���� 50I5
extern LONG* 	RS_pn37I5; 		 //I5���� 37I5
extern LONG* 	RS_pn50I6; 		 //I6���� 50I6
extern LONG* 	RS_pn37I6; 		 //I6���� 37I6
extern LONG* 	RS_pn50I7; 		 //I7���� 50I7
extern LONG* 	RS_pn37I7; 		 //I7���� 37I7
extern LONG* 	RS_pn50I8; 		 //I8���� 50I8
extern LONG* 	RS_pn37I8; 		 //I8���� 37I8
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
extern LONG* 	RS_pnT17PI; 		 //T17������ʱ T17PI
extern LONG* 	RS_pnT17RE; 		 //T17������ʱ T17RE
extern LONG* 	RS_pnT18PI; 		 //T18������ʱ T18PI
extern LONG* 	RS_pnT18RE; 		 //T18������ʱ T18RE
extern LONG* 	RS_pnT19PI; 		 //T19������ʱ T19PI
extern LONG* 	RS_pnT19RE; 		 //T19������ʱ T19RE
extern LONG* 	RS_pnT20PI; 		 //T20������ʱ T20PI
extern LONG* 	RS_pnT20RE; 		 //T20������ʱ T20RE
extern LONG* 	RS_pnT21PI; 		 //T21������ʱ T21PI
extern LONG* 	RS_pnT21RE; 		 //T21������ʱ T21RE
extern LONG* 	RS_pnT22PI; 		 //T22������ʱ T22PI
extern LONG* 	RS_pnT22RE; 		 //T22������ʱ T22RE
extern LONG* 	RS_pnT23PI; 		 //T23������ʱ T23PI
extern LONG* 	RS_pnT23RE; 		 //T23������ʱ T23RE
extern LONG* 	RS_pnT24PI; 		 //T24������ʱ T24PI
extern LONG* 	RS_pnT24RE; 		 //T24������ʱ T24RE
extern LONG* 	RS_pnT25PI; 		 //T25������ʱ T25PI
extern LONG* 	RS_pnT25RE; 		 //T25������ʱ T25RE
extern LONG* 	RS_pnT26PI; 		 //T26������ʱ T26PI
extern LONG* 	RS_pnT26RE; 		 //T26������ʱ T26RE
extern LONG* 	RS_pnT27PI; 		 //T27������ʱ T27PI
extern LONG* 	RS_pnT27RE; 		 //T27������ʱ T27RE
extern LONG* 	RS_pnT28PI; 		 //T28������ʱ T28PI
extern LONG* 	RS_pnT28RE; 		 //T28������ʱ T28RE
extern LONG* 	RS_pnT29PI; 		 //T29������ʱ T29PI
extern LONG* 	RS_pnT29RE; 		 //T29������ʱ T29RE
extern LONG* 	RS_pnT30PI; 		 //T30������ʱ T30PI
extern LONG* 	RS_pnT30RE; 		 //T30������ʱ T30RE
extern LONG* 	RS_pnT31PI; 		 //T31������ʱ T31PI
extern LONG* 	RS_pnT31RE; 		 //T31������ʱ T31RE
extern LONG* 	RS_pnT32PI; 		 //T32������ʱ T32PI
extern LONG* 	RS_pnT32RE; 		 //T32������ʱ T32RE
extern LONG* 	RS_pnTCB; 		 //��·������ʱ�� TCB
extern LONG* 	RS_pnTSW; 		 //�綯��������ʱ�� TSW
extern LONG* 	RS_pnREBZTD; 		 //��Ͷ��Ͷ��ȴ�ʱ�� REBZTD
extern LONG* 	RS_pnCHARD; 		 //����Ͷ���ʱ�� CHARD

#define RS_COUNT     101    //��ֵ����/
#define RS_BUF_SIZE     404    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 172)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnPT1Ratio; 		 //PT1���
extern LONG* 	AS_pnPT2Ratio; 		 //PT2���
extern LONG* 	AS_pnPT3Ratio; 		 //PT3���
extern LONG* 	AS_pnPT4Ratio; 		 //PT4���
extern LONG* 	AS_pnPT5Ratio; 		 //PT5���
extern LONG* 	AS_pnPT6Ratio; 		 //PT6���
extern LONG* 	AS_pnPT7Ratio; 		 //PT7���
extern LONG* 	AS_pnPT8Ratio; 		 //PT8���
extern LONG* 	AS_pnCT1Ratio; 		 //CT1���
extern LONG* 	AS_pnCT2Ratio; 		 //CT2���
extern LONG* 	AS_pnCT3Ratio; 		 //CT3���
extern LONG* 	AS_pnCT4Ratio; 		 //CT4���
extern LONG* 	AS_pnCT5Ratio; 		 //CT5���
extern LONG* 	AS_pnCT6Ratio; 		 //CT6���
extern LONG* 	AS_pnCT7Ratio; 		 //CT7���
extern LONG* 	AS_pnCT8Ratio; 		 //CT8���
extern LONG* 	AS_pnRT1; 		 //RT1

#define AS_RT_NUM     17    //������ֵ�б�ȸ���

#define AS_COUNT     21    //������ֵ����
#define AS_BUF_SIZE     84    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 576)

//ϵ��----------------------------------
extern LONG* 	CF_pnUP1; 		 //UP1
extern LONG* 	CF_pnUP2; 		 //UP2
extern LONG* 	CF_pnUP3; 		 //UP3
extern LONG* 	CF_pnUP4; 		 //UP4
extern LONG* 	CF_pnUP5; 		 //UP5
extern LONG* 	CF_pnUP6; 		 //UP6
extern LONG* 	CF_pnUP7; 		 //UP7
extern LONG* 	CF_pnUP8; 		 //UP8
extern LONG* 	CF_pnIP1; 		 //IP1
extern LONG* 	CF_pnIP2; 		 //IP2
extern LONG* 	CF_pnIP3; 		 //IP3
extern LONG* 	CF_pnIP4; 		 //IP4
extern LONG* 	CF_pnIP5; 		 //IP5
extern LONG* 	CF_pnIP6; 		 //IP6
extern LONG* 	CF_pnIP7; 		 //IP7
extern LONG* 	CF_pnIP8; 		 //IP8
extern LONG* 	CF_pnUM1; 		 //UM1
extern LONG* 	CF_pnUM2; 		 //UM2
extern LONG* 	CF_pnUM3; 		 //UM3
extern LONG* 	CF_pnUM4; 		 //UM4
extern LONG* 	CF_pnUM5; 		 //UM5
extern LONG* 	CF_pnUM6; 		 //UM6
extern LONG* 	CF_pnUM7; 		 //UM7
extern LONG* 	CF_pnUM8; 		 //UM8
extern LONG* 	CF_pnIM1; 		 //IM1
extern LONG* 	CF_pnIM2; 		 //IM2
extern LONG* 	CF_pnIM3; 		 //IM3
extern LONG* 	CF_pnIM4; 		 //IM4
extern LONG* 	CF_pnIM5; 		 //IM5
extern LONG* 	CF_pnIM6; 		 //IM6
extern LONG* 	CF_pnIM7; 		 //IM7
extern LONG* 	CF_pnIM8; 		 //IM8
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnU1D; 		 //U1D
extern LONG* 	CF_pnU2D; 		 //U2D
extern LONG* 	CF_pnU3D; 		 //U3D
extern LONG* 	CF_pnU4D; 		 //U4D
extern LONG* 	CF_pnU5D; 		 //U5D
extern LONG* 	CF_pnU6D; 		 //U6D
extern LONG* 	CF_pnU7D; 		 //U7D
extern LONG* 	CF_pnU8D; 		 //U8D
extern LONG* 	CF_pnI1D; 		 //I1D
extern LONG* 	CF_pnI2D; 		 //I2D
extern LONG* 	CF_pnI3D; 		 //I3D
extern LONG* 	CF_pnI4D; 		 //I4D
extern LONG* 	CF_pnI5D; 		 //I5D
extern LONG* 	CF_pnI6D; 		 //I6D
extern LONG* 	CF_pnI7D; 		 //I7D
extern LONG* 	CF_pnI8D; 		 //I8D

#define CF_COUNT     52    //ϵ������
#define CF_BUF_SIZE     208    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 660)

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
#define R_ACTPICK                  11    //��������
#define R_COMTEST                  12    //ͨ������
#define R_RCERR                    13    //�����Լ����
#define R_PI_BZT                   14    //����Ͷ����
#define R_PI_1ZB                   15    //1#�����������Ͷ
#define R_PI_2ZB                   16    //2#�����������Ͷ
#define R_PI_1SY                   17    //1#����ʧѹ������Ͷ
#define R_PI_2SY                   18    //2#����ʧѹ������Ͷ
#define R_SY1ZB1                   19    //1#����ʧѹ��1#�����
#define R_SY1ZB2                   20    //1#����ʧѹ��2#�����
#define R_SY2ZB2                   21    //2#����ʧѹ��2#�����
#define R_SY2ZB1                   22    //2#����ʧѹ��1#�����
#define R_BZTOFF                   23    //��Ͷ�˳�
#define R_ZB1GZ                    24    //1#�����
#define R_ZB2GZ                    25    //2#�����
#define R_SY1                      26    //1#����ʧѹ
#define R_SY2                      27    //2#����ʧѹ
#define R_SU_BZT                   28    //����Ͷ�ɹ�
#define R_FA_BZT                   29    //����Ͷʧ�� 
#define R_PT1DX                    30    //PT1����
#define R_PT2DX                    31    //PT2����
#define R_PT3DX                    32    //PT3����
#define R_PT4DX                    33    //PT4����
#define R_INF_LOCK                 34    //װ�ñ�����
#define R_INF_LKCL                 35    //���������
#define R_27T_L1                   36    //1#���ߵ�ѹ��������
#define R_27T_L2                   37    //2#���ߵ�ѹ��������
#define R_TR_1DL                   38    //��1��·��
#define R_CL_1DL                   39    //��1��·��
#define R_FA_TR1DL                 40    //��1��·��ʧ��
#define R_FA_CL1DL                 41    //��1��·��ʧ��
#define R_TR_2DL                   42    //��2��·��
#define R_CL_2DL                   43    //��2��·��
#define R_FA_TR2DL                 44    //��2��·��ʧ��
#define R_FA_CL2DL                 45    //��2��·��ʧ��
#define R_TR_3DL                   46    //��3��·��
#define R_CL_3DL                   47    //��3��·��
#define R_FA_TR3DL                 48    //��3��·��ʧ��
#define R_FA_CL3DL                 49    //��3��·��ʧ��
#define R_TR_4DL                   50    //��4��·��
#define R_CL_4DL                   51    //��4��·��
#define R_FA_TR4DL                 52    //��4��·��ʧ��
#define R_FA_CL4DL                 53    //��4��·��ʧ��
#define R_TR_5DL                   54    //��5��·��
#define R_CL_5DL                   55    //��5��·��
#define R_FA_TR5DL                 56    //��5��·��ʧ��
#define R_FA_CL5DL                 57    //��5��·��ʧ��
#define R_TR_6DL                   58    //��6��·��
#define R_CL_6DL                   59    //��6��·��
#define R_FA_TR6DL                 60    //��6��·��ʧ��
#define R_FA_CL6DL                 61    //��6��·��ʧ��
#define R_TR_7DL                   62    //��7��·��
#define R_CL_7DL                   63    //��7��·��
#define R_FA_TR7DL                 64    //��7��·��ʧ��
#define R_FA_CL7DL                 65    //��7��·��ʧ��
#define R_TR_8DL                   66    //��8��·��
#define R_CL_8DL                   67    //��8��·��
#define R_FA_TR8DL                 68    //��8��·��ʧ��
#define R_FA_CL8DL                 69    //��8��·��ʧ��
#define R_TR_9DL                   70    //��9��·��
#define R_CL_9DL                   71    //��9��·��
#define R_FA_TR9DL                 72    //��9��·��ʧ��
#define R_FA_CL9DL                 73    //��9��·��ʧ��
#define R_TR_10DL                  74    //��10��·��
#define R_CL_10DL                  75    //��10��·��
#define R_FA_TR10DL                76    //��10��·��ʧ��
#define R_FA_CL10DL                77    //��10��·��ʧ��
#define R_TR_1GK                   78    //��1����
#define R_CL_1GK                   79    //��1����
#define R_FA_TR1GK                 80    //��1����ʧ��
#define R_FA_CL1GK                 81    //��1����ʧ��
#define R_TR_2GK                   82    //��2����
#define R_CL_2GK                   83    //��2����
#define R_FA_TR2GK                 84    //��2����ʧ��
#define R_FA_CL2GK                 85    //��2����ʧ��
#define R_TR_3GK                   86    //��3����
#define R_CL_3GK                   87    //��3����
#define R_FA_TR3GK                 88    //��3����ʧ��
#define R_FA_CL3GK                 89    //��3����ʧ��
#define R_TR_4GK                   90    //��4����
#define R_CL_4GK                   91    //��4����
#define R_FA_TR4GK                 92    //��4����ʧ��
#define R_FA_CL4GK                 93    //��4����ʧ��
#define R_TR_5GK                   94    //��5����
#define R_CL_5GK                   95    //��5����
#define R_FA_TR5GK                 96    //��5����ʧ��
#define R_FA_CL5GK                 97    //��5����ʧ��
#define R_TR_6GK                   98    //��6����
#define R_CL_6GK                   99    //��6����
#define R_FA_TR6GK                100    //��6����ʧ��
#define R_FA_CL6GK                101    //��6����ʧ��
#define R_TR_7GK                  102    //��7����
#define R_CL_7GK                  103    //��7����
#define R_FA_TR7GK                104    //��7����ʧ��
#define R_FA_CL7GK                105    //��7����ʧ��
#define R_TR_8GK                  106    //��8����
#define R_CL_8GK                  107    //��8����
#define R_FA_TR8GK                108    //��8����ʧ��
#define R_FA_CL8GK                109    //��8����ʧ��
#define R_TR_9GK                  110    //��9����
#define R_CL_9GK                  111    //��9����
#define R_FA_TR9GK                112    //��9����ʧ��
#define R_FA_CL9GK                113    //��9����ʧ��
#define R_TR_10GK                 114    //��10����
#define R_CL_10GK                 115    //��10����
#define R_FA_TR10GK               116    //��10����ʧ��
#define R_FA_CL10GK               117    //��10����ʧ��
#define R_TR_101                  118    //��101��·��
#define R_CL_101                  119    //��101��·��
#define R_FA_TR101                120    //��101��·��ʧ��
#define R_FA_CL101                121    //��101��·��ʧ��
#define R_TR_102                  122    //��102��·��
#define R_CL_102                  123    //��102��·��
#define R_FA_TR102                124    //��102��·��ʧ��
#define R_FA_CL102                125    //��102��·��ʧ��
#define R_TR_103                  126    //��103��·��
#define R_CL_103                  127    //��103��·��
#define R_FA_TR103                128    //��103��·��ʧ��
#define R_FA_CL103                129    //��103��·��ʧ��
#define R_TR_104                  130    //��104��·��
#define R_CL_104                  131    //��104��·��
#define R_FA_TR104                132    //��104��·��ʧ��
#define R_FA_CL104                133    //��104��·��ʧ��
#define R_TR_201                  134    //��201��·��
#define R_CL_201                  135    //��201��·��
#define R_FA_TR201                136    //��201��·��ʧ��
#define R_FA_CL201                137    //��201��·��ʧ��
#define R_TR_202                  138    //��202��·��
#define R_CL_202                  139    //��202��·��
#define R_FA_TR202                140    //��202��·��ʧ��
#define R_FA_CL202                141    //��202��·��ʧ��
#define R_TR_203                  142    //��203��·��
#define R_CL_203                  143    //��203��·��
#define R_FA_TR203                144    //��203��·��ʧ��
#define R_FA_CL203                145    //��203��·��ʧ��
#define R_TR_204                  146    //��204��·��
#define R_CL_204                  147    //��204��·��
#define R_FA_TR204                148    //��204��·��ʧ��
#define R_FA_CL204                149    //��204��·��ʧ��
#define R_TR_201A                 150    //��201A��·��
#define R_CL_201A                 151    //��201A��·��
#define R_FA_TR201A               152    //��201A��·��ʧ��
#define R_FA_CL201A               153    //��201A��·��ʧ��
#define R_TR_201B                 154    //��201B��·��
#define R_CL_201B                 155    //��201B��·��
#define R_FA_TR201B               156    //��201B��·��ʧ��
#define R_FA_CL201B               157    //��201B��·��ʧ��
#define R_TR_202A                 158    //��202A��·��
#define R_CL_202A                 159    //��202A��·��
#define R_FA_TR202A               160    //��202A��·��ʧ��
#define R_FA_CL202A               161    //��202A��·��ʧ��
#define R_TR_202B                 162    //��202B��·��   
#define R_CL_202B                 163    //��202B��·��
#define R_FA_TR202B               164    //��202B��·��ʧ��
#define R_FA_CL202B               165    //��202B��·��ʧ��
#define R_TR_1011                 166    //��1011����
#define R_CL_1011                 167    //��1011����
#define R_FA_TR1011               168    //��2011����ʧ��
#define R_FA_CL1011               169    //��1011����ʧ��
#define R_TR_1021                 170    //��1021����
#define R_CL_1021                 171    //��1021����
#define R_FA_TR1021               172    //��1021����ʧ��
#define R_FA_CL1021               173    //��1021����ʧ��
#define R_TR_1001                 174    //��1001����
#define R_CL_1001                 175    //��1001����
#define R_FA_TR1001               176    //��1001����ʧ��
#define R_FA_CL1001               177    //��1001����ʧ��
#define R_TR_1002                 178    //��1002����
#define R_CL_1002                 179    //��1002����
#define R_FA_TR1002               180    //��1002����ʧ��
#define R_FA_CL1002               181    //��1002����ʧ��
#define R_TR_1019                 182    //��1019����
#define R_CL_1019                 183    //��1019����
#define R_FA_TR1019               184    //��1019����ʧ��
#define R_FA_CL1019               185    //��1019����ʧ��
#define R_TR_1029                 186    //��1029����
#define R_CL_1029                 187    //��1029����
#define R_FA_TR1029               188    //��1029����ʧ��
#define R_FA_CL1029               189    //��1029����ʧ��
#define R_TR_1039                 190    //��1039����
#define R_CL_1039                 191    //��1039����
#define R_FA_TR1039               192    //��1039����ʧ��
#define R_FA_CL1039               193    //��1039����ʧ��
#define R_TR_1049                 194    //��1049����
#define R_CL_1049                 195    //��1049����
#define R_FA_TR1049               196    //��1049����ʧ��
#define R_FA_CL1049               197    //��1049����ʧ��
#define R_AT1_ZWS                 198    //1AT����˹
#define R_AT1_YLSF                199    //1ATѹ���ͷ�
#define R_AT1_WDGG                200    //1AT�¶ȹ���
#define R_AT1_QWS                 201    //1AT����˹
#define R_AT1_WDGJ                202    //1AT�¶ȸ澯
#define R_AT1_YWYC                203    //1AT��λ�쳣
#define R_AT1_YSYC                204    //1AT�����쳣
#define R_AT2_ZWS                 205    //2AT����˹
#define R_AT2_YLSF                206    //2ATѹ���ͷ�
#define R_AT2_WDGG                207    //2AT�¶ȹ���
#define R_AT2_QWS                 208    //2AT����˹
#define R_AT2_WDGJ                209    //2AT�¶ȸ澯
#define R_AT2_YWYC                210    //2AT��λ�쳣
#define R_AT2_YSYC                211    //2AT�����쳣
#define R_AT3_ZWS                 212    //3AT����˹
#define R_AT3_YLSF                213    //3ATѹ���ͷ�
#define R_AT3_WDGG                214    //3AT�¶ȹ���
#define R_AT3_QWS                 215    //3AT����˹
#define R_AT3_WDGJ                216    //3AT�¶ȸ澯
#define R_AT3_YWYC                217    //3AT��λ�쳣
#define R_AT3_YSYC                218    //3AT�����쳣
#define R_AT4_ZWS                 219    //4AT����˹
#define R_AT4_YLSF                220    //4ATѹ���ͷ�
#define R_AT4_WDGG                221    //4AT�¶ȹ���
#define R_AT4_QWS                 222    //4AT����˹
#define R_AT4_WDGJ                223    //4AT�¶ȸ澯
#define R_AT4_YWYC                224    //4AT��λ�쳣
#define R_AT4_YSYC                225    //4AT�����쳣
#define R_TR_1ATKF                226    //��1AT��ֿ���
#define R_TR_2ATKF                227    //��2AT��ֿ���
#define R_TR_3ATKF                228    //��3AT��ֿ���
#define R_TR_4ATKF                229    //��4AT��ֿ���
#define R_FA_TR1ATKF              230    //��1AT���ʧ��
#define R_FA_TR2ATKF              231    //��2AT���ʧ��
#define R_FA_TR3ATKF              232    //��3AT���ʧ��
#define R_FA_TR4ATKF              233    //��4AT���ʧ��
#define R_SU_ATOP                 234    //�����ɹ�
#define R_FA_ATOP                 235    //����ʧ��
#define R_SU_79_59                236    //����ѹ��բ�ɹ�
#define R_FA_79_59                237    //���к�բʧ��
#define R_TR_241                  238    //��241��·��
#define R_CL_241                  239    //��241��·��
#define R_FA_TR241                240    //��241��·��ʧ��
#define R_FA_CL241                241    //��241��·��ʧ��
#define R_TR_242                  242    //��242��·��
#define R_CL_242                  243    //��242��·��
#define R_FA_TR242                244    //��242��·��ʧ��
#define R_FA_CL242                245    //��242��·��ʧ��
#define R_TR_271                  246    //��271��·��
#define R_CL_271                  247    //��271��·��
#define R_FA_TR271                248    //��271��·��ʧ��
#define R_FA_CL271                249    //��271��·��ʧ��
#define R_TR_272                  250    //��272��·��
#define R_CL_272                  251    //��272��·��
#define R_FA_TR272                252    //��272��·��ʧ��
#define R_FA_CL272                253    //��272��·��ʧ��
#define R_TR_281                  254    //��281��·��
#define R_CL_281                  255    //��281��·��
#define R_FA_TR281                256    //��281��·��ʧ��
#define R_FA_CL281                257    //��281��·��ʧ��
#define R_TR_2411                 258    //��2411����
#define R_CL_2411                 259    //��2411����
#define R_FA_TR2411               260    //��2411����ʧ��
#define R_FA_CL2411               261    //��2411����ʧ��
#define R_TR_2421                 262    //��2421����
#define R_CL_2421                 263    //��2421����
#define R_FA_TR2421               264    //��2421����ʧ��
#define R_FA_CL2421               265    //��2421����ʧ��
#define R_TR_2711                 266    //��2711����
#define R_CL_2711                 267    //��2711����
#define R_FA_TR2711               268    //��2711����ʧ��
#define R_FA_CL2711               269    //��2711����ʧ��
#define R_TR_2721                 270    //��2721����
#define R_CL_2721                 271    //��2721����
#define R_FA_TR2721               272    //��2721����ʧ��
#define R_FA_CL2721               273    //��2721����ʧ��
#define R_NVRAMERR_R              274    //NVRAM���ݳ���ָ�
#define R_ROMERR_R                275    //����ROM���ϻָ�
#define R_RAMERR_R                276    //RAM��ֵ���ϻָ�
#define R_EEPROMERR_R             277    //EEPROM��ֵ���ϻָ�
#define R_SLERR_R                 278    //��ѹ����ϻָ�
#define R_ADERR_R                 279    //AD���ϻָ�
#define R_LOGICERR_R              280    //�߼����̹��Ϲ��ϻָ�
#define R_COMERR_R                281    //ͨ��ͨ���쳣�ָ�
#define R_EARTHQK                 282    //���𱨾�
#define R_POWERERR                283    //5V��Դ����
#define R_EEPROM_W_ERR            284    //EEPROMд�����
#define R_POWERERR_R              285    //5V��Դ���ϻָ�
#define R_FA_TRDR1                286    //1�ŵ�����բʧ��
#define R_FA_TRDR2                287    //2�ŵ�����բʧ��
#define R_FA_TRDR3                288    //3�ŵ�����բʧ��
#define R_FA_TRDR4                289    //4�ŵ�����բʧ��
#define R_PT1_RES                 290    //PT1���߻ָ�
#define R_PT2_RES                 291    //PT2���߻ָ�
#define R_PT3_RES                 292    //PT3���߻ָ�
#define R_PT4_RES                 293    //PT4���߻ָ�
#define R_1SY_RES                 294    //һ�Ž���ʧѹ�ָ�
#define R_2SY_RES                 295    //���Ž���ʧѹ�ָ�
#define R_RELDATAERR              296    //���������쳣
#define R_RELDATAERR_R            297    //���������쳣�ָ�
#define MAX_REPORT_INDEX          297    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     32    //��������

#endif