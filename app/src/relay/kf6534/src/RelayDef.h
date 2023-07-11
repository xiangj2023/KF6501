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
#define  RM_IC1 16
extern LONG* 	RM_pnIC1; 		 // IC1
#define  RM_IC2 17
extern LONG* 	RM_pnIC2; 		 // IC2
#define  RM_BOBRCH1 18
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 19
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 20
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 21
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     22    //��������ֵ����
#define RM_BUF_SIZE     88    //��������ֵ��������С
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
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 92)

//��ѹ��----------------------------------
extern BYTE* 	SW_pbyBZT; 		 //����Ͷ����
extern BYTE* 	SW_pbyACAT1; 		 //1AT�Զ�����
extern BYTE* 	SW_pbyACAT2; 		 //2AT�Զ�����
extern BYTE* 	SW_pby60I_AT1; 		 //1AT��������
extern BYTE* 	SW_pby50T_AT1; 		 //1ATT�߹���
extern BYTE* 	SW_pby50F_AT1; 		 //1ATF�߹���
extern BYTE* 	SW_pby50CS_AT1; 		 //1AT���ǹ���
extern BYTE* 	SW_pby60I_AT2; 		 //2AT��������
extern BYTE* 	SW_pby50T_AT2; 		 //2ATT�߹���
extern BYTE* 	SW_pby50F_AT2; 		 //2ATF�߹���
extern BYTE* 	SW_pby50CS_AT2; 		 //2AT���ǹ���
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2
extern BYTE* 	SW_pbyBAK3; 		 //����ѹ��3
extern BYTE* 	SW_pbyBAK4; 		 //����ѹ��4

#define SW_COUNT     16    //��ѹ�����
#define SW_BUF_SIZE     16    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 156)

//����----------------------------------
extern BYTE* 	CFG_pbyBZT; 		 //����Ͷ����
extern BYTE* 	CFG_pbyREBZT; 		 //����Ͷ����Ͷ��
extern BYTE* 	CFG_pbyACAT1; 		 //1AT�Զ�����
extern BYTE* 	CFG_pbyACAT2; 		 //2AT�Զ�����
extern BYTE* 	CFG_pby60I_AT1; 		 //1AT��������
extern BYTE* 	CFG_pby50T_AT1; 		 //1ATT�߹���
extern BYTE* 	CFG_pby50F_AT1; 		 //1ATF�߹���
extern BYTE* 	CFG_pby50CS_AT1; 		 //1AT���ǹ���
extern BYTE* 	CFG_pby60I_AT2; 		 //2AT��������
extern BYTE* 	CFG_pby50T_AT2; 		 //2ATT�߹���
extern BYTE* 	CFG_pby50F_AT2; 		 //2ATF�߹���
extern BYTE* 	CFG_pby50CS_AT2; 		 //2AT���ǹ���
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pby74PT1; 		 //PT1���߼��
extern BYTE* 	CFG_pby74PT2; 		 //PT2���߼��
extern BYTE* 	CFG_pby74PT3; 		 //PT3���߼��
extern BYTE* 	CFG_pby74PT4; 		 //PT4���߼��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2
extern BYTE* 	CFG_pbyBAK3; 		 //��������3
extern BYTE* 	CFG_pbyBAK4; 		 //��������4

#define CFG_COUNT     21    //���ø���/
#define CFG_BUF_SIZE     21    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 172)

//��ֵ----------------------------------
extern LONG* 	RS_pnF; 		 //������ F
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn60I_AT1C; 		 //1AT������ֵ 60I_AT1C
extern LONG* 	RS_pn60I_AT1T; 		 //1AT��������ʱ�� 60I_AT1T
extern LONG* 	RS_pn50T_AT1C; 		 //1ATT�߹�����ֵ 50T_AT1C
extern LONG* 	RS_pn50T_AT1T; 		 //1ATT�߹�������ʱ�� 50T_AT1T
extern LONG* 	RS_pn50F_AT1C; 		 //1ATF�߹�����ֵ 50F_AT1C
extern LONG* 	RS_pn50F_AT1T; 		 //1ATF�߹�������ʱ�� 50F_AT1T
extern LONG* 	RS_pn50CS_AT1C; 		 //1AT���ǹ�����ֵ 50CS_AT1C
extern LONG* 	RS_pn50CS_AT1T; 		 //1AT���ǹ�������ʱ�� 50CS_AT1T
extern LONG* 	RS_pn60I_AT2C; 		 //2AT������ֵ 60I_AT2C
extern LONG* 	RS_pn60I_AT2T; 		 //2AT��������ʱ�� 60I_AT2T
extern LONG* 	RS_pn50T_AT2C; 		 //2ATT�߹�����ֵ 50T_AT2C
extern LONG* 	RS_pn50T_AT2T; 		 //2ATT�߹�������ʱ�� 50T_AT2T
extern LONG* 	RS_pn50F_AT2C; 		 //2ATF�߹�����ֵ 50F_AT2C
extern LONG* 	RS_pn50F_AT2T; 		 //2ATF�߹�������ʱ�� 50F_AT2T
extern LONG* 	RS_pn50CS_AT2C; 		 //2AT���ǹ�����ֵ 50CS_AT2C
extern LONG* 	RS_pn50CS_AT2T; 		 //2AT���ǹ�������ʱ�� 50CS_AT2T
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
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD

#define RS_COUNT     119    //��ֵ����/
#define RS_BUF_SIZE     476    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 194)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnTR3D; 		 //TR3����ʱ��
extern LONG* 	AS_pnTR4D; 		 //TR4����ʱ��
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

#define AS_COUNT     23    //������ֵ����
#define AS_BUF_SIZE     92    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 670)

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
extern LONG* 	CF_pnIC1; 		 //IC1
extern LONG* 	CF_pnIC2; 		 //IC2
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

#define CF_COUNT     54    //ϵ������
#define CF_BUF_SIZE     216    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 762)

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
#define R_NVRAMERR                  9    //NVRAM���ݳ���
#define R_LOGICERR                 10    //�߼����̹���
#define R_RCERR                    11    //�����Լ����
#define R_COMTEST                  12    //ͨ������
#define R_ADERR_R                  13    //AD���ϻָ�
#define R_SLERR_R                  14    //��ѹ����ϻָ�
#define R_EEPROMERR_R              15    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 16    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 17    //����ROM���ϻָ�
#define R_COMERR_R                 18    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               19    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               20    //�߼����̹��Ϲ��ϻָ�
#define R_ACTPICK                  21    //��������
#define R_PI_BZT                   22    //����Ͷ����
#define R_PI_1ZB                   23    //1#�����������Ͷ
#define R_PI_2ZB                   24    //2#�����������Ͷ
#define R_PI_1SY                   25    //1#����ʧѹ������Ͷ
#define R_PI_2SY                   26    //2#����ʧѹ������Ͷ
#define R_SY1ZB1                   27    //1#����ʧѹ��1#�����
#define R_SY1ZB2                   28    //1#����ʧѹ��2#�����
#define R_SY2ZB2                   29    //2#����ʧѹ��2#�����
#define R_SY2ZB1                   30    //2#����ʧѹ��1#�����
#define R_BZTOFF                   31    //��Ͷ�˳�
#define R_ZB1GZ                    32    //1#�����
#define R_ZB2GZ                    33    //2#�����
#define R_SY1                      34    //1#����ʧѹ
#define R_SY2                      35    //2#����ʧѹ
#define R_SU_BZT                   36    //����Ͷ�ɹ�
#define R_FA_BZT                   37    //����Ͷʧ�� 
#define R_PT1DX                    38    //PT1����
#define R_PT2DX                    39    //PT2����
#define R_PT3DX                    40    //PT3����
#define R_PT4DX                    41    //PT4����
#define R_INF_LOCK                 42    //װ�ñ�����
#define R_INF_LKCL                 43    //���������
#define R_60I_1AT                  44    //1AT��������
#define R_50T_1AT                  45    //1ATT�߹�������
#define R_50F_1AT                  46    //1ATF�߹�������
#define R_50CS_1AT                 47    //1AT���ǹ�����������
#define R_60I_2AT                  48    //2AT��������
#define R_50T_2AT                  49    //2ATT�߹�����������
#define R_50F_2AT                  50    //2ATF�߹�����������
#define R_50CS_2AT                 51    //2AT���ǹ�����������
#define R_60I_3AT                  52    //3AT��������
#define R_50T_3AT                  53    //3ATT�߹�������
#define R_50F_3AT                  54    //3ATF�߹�������
#define R_50CS_3AT                 55    //3AT���ǹ�����������
#define R_60I_4AT                  56    //4AT��������
#define R_50T_4AT                  57    //4ATT�߹�����������
#define R_50F_4AT                  58    //4ATF�߹�����������
#define R_50CS_4AT                 59    //4AT���ǹ�����������
#define R_27T_L1                   60    //1#���ߵ�ѹ��������
#define R_27T_L2                   61    //2#���ߵ�ѹ��������
#define R_TR_1DL                   62    //��1��·��
#define R_CL_1DL                   63    //��1��·��
#define R_FA_TR1DL                 64    //��1��·��ʧ��
#define R_FA_CL1DL                 65    //��1��·��ʧ��
#define R_TR_2DL                   66    //��2��·��
#define R_CL_2DL                   67    //��2��·��
#define R_FA_TR2DL                 68    //��2��·��ʧ��
#define R_FA_CL2DL                 69    //��2��·��ʧ��
#define R_TR_3DL                   70    //��3��·��
#define R_CL_3DL                   71    //��3��·��
#define R_FA_TR3DL                 72    //��3��·��ʧ��
#define R_FA_CL3DL                 73    //��3��·��ʧ��
#define R_TR_4DL                   74    //��4��·��
#define R_CL_4DL                   75    //��4��·��
#define R_FA_TR4DL                 76    //��4��·��ʧ��
#define R_FA_CL4DL                 77    //��4��·��ʧ��
#define R_TR_5DL                   78    //��5��·��
#define R_CL_5DL                   79    //��5��·��
#define R_FA_TR5DL                 80    //��5��·��ʧ��
#define R_FA_CL5DL                 81    //��5��·��ʧ��
#define R_TR_6DL                   82    //��6��·��
#define R_CL_6DL                   83    //��6��·��
#define R_FA_TR6DL                 84    //��6��·��ʧ��
#define R_FA_CL6DL                 85    //��6��·��ʧ��
#define R_TR_7DL                   86    //��7��·��
#define R_CL_7DL                   87    //��7��·��
#define R_FA_TR7DL                 88    //��7��·��ʧ��
#define R_FA_CL7DL                 89    //��7��·��ʧ��
#define R_TR_8DL                   90    //��8��·��
#define R_CL_8DL                   91    //��8��·��
#define R_FA_TR8DL                 92    //��8��·��ʧ��
#define R_FA_CL8DL                 93    //��8��·��ʧ��
#define R_TR_9DL                   94    //��9��·��
#define R_CL_9DL                   95    //��9��·��
#define R_FA_TR9DL                 96    //��9��·��ʧ��
#define R_FA_CL9DL                 97    //��9��·��ʧ��
#define R_TR_10DL                  98    //��10��·��
#define R_CL_10DL                  99    //��10��·��
#define R_FA_TR10DL               100    //��10��·��ʧ��
#define R_FA_CL10DL               101    //��10��·��ʧ��
#define R_TR_1GK                  102    //��1����
#define R_CL_1GK                  103    //��1����
#define R_FA_TR1GK                104    //��1����ʧ��
#define R_FA_CL1GK                105    //��1����ʧ��
#define R_TR_2GK                  106    //��2����
#define R_CL_2GK                  107    //��2����
#define R_FA_TR2GK                108    //��2����ʧ��
#define R_FA_CL2GK                109    //��2����ʧ��
#define R_TR_3GK                  110    //��3����
#define R_CL_3GK                  111    //��3����
#define R_FA_TR3GK                112    //��3����ʧ��
#define R_FA_CL3GK                113    //��3����ʧ��
#define R_TR_4GK                  114    //��4����
#define R_CL_4GK                  115    //��4����
#define R_FA_TR4GK                116    //��4����ʧ��
#define R_FA_CL4GK                117    //��4����ʧ��
#define R_TR_5GK                  118    //��5����
#define R_CL_5GK                  119    //��5����
#define R_FA_TR5GK                120    //��5����ʧ��
#define R_FA_CL5GK                121    //��5����ʧ��
#define R_TR_6GK                  122    //��6����
#define R_CL_6GK                  123    //��6����
#define R_FA_TR6GK                124    //��6����ʧ��
#define R_FA_CL6GK                125    //��6����ʧ��
#define R_TR_7GK                  126    //��7����
#define R_CL_7GK                  127    //��7����
#define R_FA_TR7GK                128    //��7����ʧ��
#define R_FA_CL7GK                129    //��7����ʧ��
#define R_TR_8GK                  130    //��8����
#define R_CL_8GK                  131    //��8����
#define R_FA_TR8GK                132    //��8����ʧ��
#define R_FA_CL8GK                133    //��8����ʧ��
#define R_TR_9GK                  134    //��9����
#define R_CL_9GK                  135    //��9����
#define R_FA_TR9GK                136    //��9����ʧ��
#define R_FA_CL9GK                137    //��9����ʧ��
#define R_TR_10GK                 138    //��10����
#define R_CL_10GK                 139    //��10����
#define R_FA_TR10GK               140    //��10����ʧ��
#define R_FA_CL10GK               141    //��10����ʧ��
#define R_TR_101                  142    //��101��·��
#define R_CL_101                  143    //��101��·��
#define R_FA_TR101                144    //��101��·��ʧ��
#define R_FA_CL101                145    //��101��·��ʧ��
#define R_TR_102                  146    //��102��·��
#define R_CL_102                  147    //��102��·��
#define R_FA_TR102                148    //��102��·��ʧ��
#define R_FA_CL102                149    //��102��·��ʧ��
#define R_TR_103                  150    //��103��·��
#define R_CL_103                  151    //��103��·��
#define R_FA_TR103                152    //��103��·��ʧ��
#define R_FA_CL103                153    //��103��·��ʧ��
#define R_TR_104                  154    //��104��·��
#define R_CL_104                  155    //��104��·��
#define R_FA_TR104                156    //��104��·��ʧ��
#define R_FA_CL104                157    //��104��·��ʧ��
#define R_TR_201                  158    //��201��·��
#define R_CL_201                  159    //��201��·��
#define R_FA_TR201                160    //��201��·��ʧ��
#define R_FA_CL201                161    //��201��·��ʧ��
#define R_TR_202                  162    //��202��·��
#define R_CL_202                  163    //��202��·��
#define R_FA_TR202                164    //��202��·��ʧ��
#define R_FA_CL202                165    //��202��·��ʧ��
#define R_TR_203                  166    //��203��·��
#define R_CL_203                  167    //��203��·��
#define R_FA_TR203                168    //��203��·��ʧ��
#define R_FA_CL203                169    //��203��·��ʧ��
#define R_TR_204                  170    //��204��·��
#define R_CL_204                  171    //��204��·��
#define R_FA_TR204                172    //��204��·��ʧ��
#define R_FA_CL204                173    //��204��·��ʧ��
#define R_TR_201A                 174    //��201A��·��
#define R_CL_201A                 175    //��201A��·��
#define R_FA_TR201A               176    //��201A��·��ʧ��
#define R_FA_CL201A               177    //��201A��·��ʧ��
#define R_TR_201B                 178    //��201B��·��
#define R_CL_201B                 179    //��201B��·��
#define R_FA_TR201B               180    //��201B��·��ʧ��
#define R_FA_CL201B               181    //��201B��·��ʧ��
#define R_TR_202A                 182    //��202A��·��
#define R_CL_202A                 183    //��202A��·��
#define R_FA_TR202A               184    //��202A��·��ʧ��
#define R_FA_CL202A               185    //��202A��·��ʧ��
#define R_TR_202B                 186    //��202B��·��   
#define R_CL_202B                 187    //��202B��·��
#define R_FA_TR202B               188    //��202B��·��ʧ��
#define R_FA_CL202B               189    //��202B��·��ʧ��
#define R_TR_1011                 190    //��1011����
#define R_CL_1011                 191    //��1011����
#define R_FA_TR1011               192    //��2011����ʧ��
#define R_FA_CL1011               193    //��1011����ʧ��
#define R_TR_1021                 194    //��1021����
#define R_CL_1021                 195    //��1021����
#define R_FA_TR1021               196    //��1021����ʧ��
#define R_FA_CL1021               197    //��1021����ʧ��
#define R_TR_1001                 198    //��1001����
#define R_CL_1001                 199    //��1001����
#define R_FA_TR1001               200    //��1001����ʧ��
#define R_FA_CL1001               201    //��1001����ʧ��
#define R_TR_1002                 202    //��1002����
#define R_CL_1002                 203    //��1002����
#define R_FA_TR1002               204    //��1002����ʧ��
#define R_FA_CL1002               205    //��1002����ʧ��
#define R_TR_1019                 206    //��1019����
#define R_CL_1019                 207    //��1019����
#define R_FA_TR1019               208    //��1019����ʧ��
#define R_FA_CL1019               209    //��1019����ʧ��
#define R_TR_1029                 210    //��1029����
#define R_CL_1029                 211    //��1029����
#define R_FA_TR1029               212    //��1029����ʧ��
#define R_FA_CL1029               213    //��1029����ʧ��
#define R_TR_1039                 214    //��1039����
#define R_CL_1039                 215    //��1039����
#define R_FA_TR1039               216    //��1039����ʧ��
#define R_FA_CL1039               217    //��1039����ʧ��
#define R_TR_1049                 218    //��1049����
#define R_CL_1049                 219    //��1049����
#define R_FA_TR1049               220    //��1049����ʧ��
#define R_FA_CL1049               221    //��1049����ʧ��
#define R_AT1_ZWS                 222    //1AT����˹
#define R_AT1_YLSF                223    //1ATѹ���ͷ�
#define R_AT1_WDGG                224    //1AT�¶ȹ���
#define R_AT1_QWS                 225    //1AT����˹
#define R_AT1_WDGJ                226    //1AT�¶ȸ澯
#define R_AT1_YWYC                227    //1AT��λ�쳣
#define R_AT1_YSYC                228    //1AT�����쳣
#define R_AT2_ZWS                 229    //2AT����˹
#define R_AT2_YLSF                230    //2ATѹ���ͷ�
#define R_AT2_WDGG                231    //2AT�¶ȹ���
#define R_AT2_QWS                 232    //2AT����˹
#define R_AT2_WDGJ                233    //2AT�¶ȸ澯
#define R_AT2_YWYC                234    //2AT��λ�쳣
#define R_AT2_YSYC                235    //2AT�����쳣
#define R_AT3_ZWS                 236    //3AT����˹
#define R_AT3_YLSF                237    //3ATѹ���ͷ�
#define R_AT3_WDGG                238    //3AT�¶ȹ���
#define R_AT3_QWS                 239    //3AT����˹
#define R_AT3_WDGJ                240    //3AT�¶ȸ澯
#define R_AT3_YWYC                241    //3AT��λ�쳣
#define R_AT3_YSYC                242    //3AT�����쳣
#define R_AT4_ZWS                 243    //4AT����˹
#define R_AT4_YLSF                244    //4ATѹ���ͷ�
#define R_AT4_WDGG                245    //4AT�¶ȹ���
#define R_AT4_QWS                 246    //4AT����˹
#define R_AT4_WDGJ                247    //4AT�¶ȸ澯
#define R_AT4_YWYC                248    //4AT��λ�쳣
#define R_AT4_YSYC                249    //4AT�����쳣
#define R_TR_1ATKF                250    //��1AT��ֿ���
#define R_TR_2ATKF                251    //��2AT��ֿ���
#define R_TR_3ATKF                252    //��3AT��ֿ���
#define R_TR_4ATKF                253    //��4AT��ֿ���
#define R_FA_TR1ATKF              254    //��1AT���ʧ��
#define R_FA_TR2ATKF              255    //��2AT���ʧ��
#define R_FA_TR3ATKF              256    //��3AT���ʧ��
#define R_FA_TR4ATKF              257    //��4AT���ʧ��
#define R_SU_ATOP                 258    //�����ɹ�
#define R_FA_ATOP                 259    //����ʧ��
#define R_SU_79_59                260    //����ѹ��բ�ɹ�
#define R_FA_79_59                261    //���к�բʧ��
#define R_TR_241                  262    //��241��·��
#define R_CL_241                  263    //��241��·��
#define R_FA_TR241                264    //��241��·��ʧ��
#define R_FA_CL241                265    //��241��·��ʧ��
#define R_TR_242                  266    //��242��·��
#define R_CL_242                  267    //��242��·��
#define R_FA_TR242                268    //��242��·��ʧ��
#define R_FA_CL242                269    //��242��·��ʧ��
#define R_TR_271                  270    //��271��·��
#define R_CL_271                  271    //��271��·��
#define R_FA_TR271                272    //��271��·��ʧ��
#define R_FA_CL271                273    //��271��·��ʧ��
#define R_TR_272                  274    //��272��·��
#define R_CL_272                  275    //��272��·��
#define R_FA_TR272                276    //��272��·��ʧ��
#define R_FA_CL272                277    //��272��·��ʧ��
#define R_TR_281                  278    //��281��·��
#define R_CL_281                  279    //��281��·��
#define R_FA_TR281                280    //��281��·��ʧ��
#define R_FA_CL281                281    //��281��·��ʧ��
#define R_TR_2411                 282    //��2411����
#define R_CL_2411                 283    //��2411����
#define R_FA_TR2411               284    //��2411����ʧ��
#define R_FA_CL2411               285    //��2411����ʧ��
#define R_TR_2421                 286    //��2421����
#define R_CL_2421                 287    //��2421����
#define R_FA_TR2421               288    //��2421����ʧ��
#define R_FA_CL2421               289    //��2421����ʧ��
#define R_TR_2711                 290    //��2711����
#define R_CL_2711                 291    //��2711����
#define R_FA_TR2711               292    //��2711����ʧ��
#define R_FA_CL2711               293    //��2711����ʧ��
#define R_TR_2721                 294    //��2721����
#define R_CL_2721                 295    //��2721����
#define R_FA_TR2721               296    //��2721����ʧ��
#define R_FA_CL2721               297    //��2721����ʧ��
#define R_60I_1AT_PI              298    //1AT��������
#define R_60I_1AT_RES             299    //1AT��������
#define R_50T_1AT_PI              300    //1AT T�߹�������
#define R_50T_1AT_RES             301    //1AT T�߹�������
#define R_50F_1AT_PI              302    //1AT F�߹�������
#define R_50F_1AT_RES             303    //1AT F�߹�������
#define R_50CS_1AT_PI             304    //1AT���ǹ�������
#define R_50CS_1AT_RES            305    //1AT���ǹ�������
#define R_60I_2AT_PI              306    //2AT��������
#define R_60I_2AT_RES             307    //2AT��������
#define R_50T_2AT_PI              308    //2AT T�߹�������
#define R_50T_2AT_RES             309    //2AT T�߹�������
#define R_50F_2AT_PI              310    //2AT F�߹�������
#define R_50F_2AT_RES             311    //2AT F�߹�������
#define R_50CS_2AT_PI             312    //2AT���ǹ�������
#define R_50CS_2AT_RES            313    //2AT���ǹ�������
#define R_PICKUP                  314    //��������
#define R_52BF_AT1                315    //AT1��·����բʧ��
#define R_52BF_AT2                316    //AT2��·����բʧ��
#define R_PI_1AT                  317    //1AT����������Ͷ
#define R_PI_2AT                  318    //2AT����������Ͷ
#define R_POWERERR                319    //5V��Դ����
#define R_EEPROM_W_ERR            320    //EEPROMд�����
#define R_POWERERR_R              321    //5V��Դ���ϻָ�
#define R_RELDATAERR              322    //���������쳣
#define R_RELDATAERR_R            323    //���������쳣�ָ�
#define MAX_REPORT_INDEX          323    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     28    //��������

#endif