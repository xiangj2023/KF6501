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

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----���緽ʽ-----
#define 	BTMODE 	0	//ֱ������
#define 	ATMODE 	1	//AT����

//��������ֵ����-----------------------
#define  RM_UP 0
extern LONG* 	RM_pnUp; 		 // U
#define  RM_U1 1
extern LONG* 	RM_pnU1; 		 // U1
#define  RM_U2 2
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_UD1 3
extern LONG* 	RM_pnUd1; 		 // Ud1
#define  RM_UD2 4
extern LONG* 	RM_pnUd2; 		 // Ud2
#define  RM_UD3 5
extern LONG* 	RM_pnUd3; 		 // Ud3
#define  RM_IP 6
extern LONG* 	RM_pnIp; 		 // I
#define  RM_IM 7
extern LONG* 	RM_pnIm; 		 // Im
#define  RM_I1 8
extern LONG* 	RM_pnI1; 		 // I1
#define  RM_I2 9
extern LONG* 	RM_pnI2; 		 // I2
#define  RM_I3 10
extern LONG* 	RM_pnI3; 		 // I3
#define  RM_IH3 11
extern LONG* 	RM_pnIh3; 		 // Ih3
#define  RM_IH5 12
extern LONG* 	RM_pnIh5; 		 // Ih5
#define  RM_IH7 13
extern LONG* 	RM_pnIh7; 		 // Ih7
#define  RM_BOBRCH1 14
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 15
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 16
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 17
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     18    //��������ֵ����
#define RM_BUF_SIZE     72    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UM 0
extern LONG* 	M_pnUM; 		 //U U
#define  M_U1M 1
extern LONG* 	M_pnU1M; 		 //U1 U1
#define  M_U2M 2
extern LONG* 	M_pnU2M; 		 //U2 U2
#define  M_UD1M 3
extern LONG* 	M_pnUd1M; 		 //Ud1 Ud1
#define  M_UD2M 4
extern LONG* 	M_pnUd2M; 		 //Ud2 Ud2
#define  M_UD3M 5
extern LONG* 	M_pnUd3M; 		 //Ud3 Ud3
#define  M_IM 6
extern LONG* 	M_pnIM; 		 //I I
#define  M_I1M 7
extern LONG* 	M_pnI1M; 		 //I1 I1
#define  M_I2M 8
extern LONG* 	M_pnI2M; 		 //I2 I2
#define  M_I3M 9
extern LONG* 	M_pnI3M; 		 //I3 I3
#define  M_P 10
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 11
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 12
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_IH1 13
extern LONG* 	M_pnIH1; 		 //IH1 IH1
#define  M_IH2 14
extern LONG* 	M_pnIH2; 		 //IH2 IH2
#define  M_IH3 15
extern LONG* 	M_pnIH3; 		 //IH3 IH3
#define  M_IH5 16
extern LONG* 	M_pnIH5; 		 //IH5 IH5
#define  M_IH7 17
extern LONG* 	M_pnIH7; 		 //IH7 IH7
#define  M_IH9 18
extern LONG* 	M_pnIH9; 		 //IH9 IH9
#define  M_IH11 19
extern LONG* 	M_pnIH11; 		 //IH11 IH11

#define MEASURE_COUNT     20    //����ֵ����
#define MEASURE_BUF_SIZE     80    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 76)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby50_0; 		 //�����ٶ�
extern BYTE* 	SW_pby50_1; 		 //��������
extern BYTE* 	SW_pby59; 		 //��ѹ����
extern BYTE* 	SW_pby27; 		 //��ѹ����
extern BYTE* 	SW_pby50_H; 		 //г����������
extern BYTE* 	SW_pby50ST; 		 //֧·��������
extern BYTE* 	SW_pby60I; 		 //��������
extern BYTE* 	SW_pby60U; 		 //��ѹ����
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     11    //��ѹ�����
#define SW_BUF_SIZE     11    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 156)

//����----------------------------------
extern BYTE* 	CFG_pby50_0; 		 //�ٶϱ���
extern BYTE* 	CFG_pby50_1; 		 //��������
extern BYTE* 	CFG_pby50_H; 		 //г����������
extern BYTE* 	CFG_pby59; 		 //��ѹ����
extern BYTE* 	CFG_pby27; 		 //��ѹ����
extern BYTE* 	CFG_pby60I1; 		 //֧·1����
extern BYTE* 	CFG_pby60I2; 		 //֧·2����
extern BYTE* 	CFG_pby50ST; 		 //֧·����
extern BYTE* 	CFG_pby60U1; 		 //֧·1��ѹ
extern BYTE* 	CFG_pby60U2; 		 //֧·2��ѹ
extern BYTE* 	CFG_pby60U3; 		 //֧·3��ѹ
extern BYTE* 	CFG_pby74PT; 		 //PT���߼��
extern BYTE* 	CFG_pbyCTBR; 		 //CT���߼��
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     16    //���ø���/
#define CFG_BUF_SIZE     16    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 170)

//��ֵ----------------------------------
extern LONG* 	RS_pnF; 		 //������ F
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn50_0C; 		 //�ٶϵ�����ֵ 50_0C
extern LONG* 	RS_pn50_0D; 		 //�ٶ�ʱ�䶨ֵ 50_0D
extern LONG* 	RS_pn50_1C; 		 //����������ֵ 50_1C
extern LONG* 	RS_pn50_1D; 		 //����ʱ�䶨ֵ 50_1D
extern LONG* 	RS_pn50_HC; 		 //г������������ֵ 50_HC
extern LONG* 	RS_pn50_HD; 		 //г������ʱ�䶨ֵ 50_HD
extern LONG* 	RS_pn59V; 		 //��ѹ������ѹ 59V
extern LONG* 	RS_pn59D; 		 //��ѹ����ʱ�� 59D
extern LONG* 	RS_pn27V; 		 //��ѹ������ѹ 27V
extern LONG* 	RS_pn27D; 		 //��ѹ����ʱ�� 27D
extern LONG* 	RS_pn27_LK50; 		 //��ѹ������������ 27_LK50
extern LONG* 	RS_pn60I1C; 		 //֧·1������������ 60I1C
extern LONG* 	RS_pn60I1D; 		 //֧·1��������ʱ�� 60I1D
extern LONG* 	RS_pn60I2C; 		 //֧·2������������ 60I2C
extern LONG* 	RS_pn60I2D; 		 //֧·2��������ʱ�� 60I2D
extern LONG* 	RS_pn50STC; 		 //֧·������������ 50STC
extern LONG* 	RS_pn50STD; 		 //֧·��������ʱ�� 50STD
extern LONG* 	RS_pn60U1V; 		 //֧·1��ѹ������ѹ 60U1V
extern LONG* 	RS_pn60U1D; 		 //֧·1��ѹ����ʱ�� 60U1D
extern LONG* 	RS_pn60U2V; 		 //֧·2��ѹ������ѹ 60U2V
extern LONG* 	RS_pn60U2D; 		 //֧·2��ѹ����ʱ�� 60U2D
extern LONG* 	RS_pn60U3V; 		 //֧·3��ѹ������ѹ 60U3V
extern LONG* 	RS_pn60U3D; 		 //֧·3��ѹ����ʱ�� 60U3D
extern LONG* 	RS_pn74PTC; 		 //PT���߼����� 74PTC
extern LONG* 	RS_pn74PTV; 		 //PT���߼���ѹ 74PTV
extern LONG* 	RS_pn74PTD; 		 //PT���߼��ʱ�� 74PTD
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD

#define RS_COUNT     29    //��ֵ����/
#define RS_BUF_SIZE     116    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 186)

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
extern LONG* 	AS_pnTR3D; 		 //TR3����ʱ��
extern LONG* 	AS_pn50IP; 		 //IP������ֵ
extern LONG* 	AS_pn37IP; 		 //IP������ֵ
extern LONG* 	AS_pn50I1; 		 //I1������ֵ
extern LONG* 	AS_pn37I1; 		 //I1������ֵ
extern LONG* 	AS_pn50I2; 		 //I2������ֵ
extern LONG* 	AS_pn37I2; 		 //I2������ֵ
extern LONG* 	AS_pn50I3; 		 //I3������ֵ
extern LONG* 	AS_pn37I3; 		 //I3������ֵ
extern LONG* 	AS_pn59UP; 		 //Up��ѹ��ֵ
extern LONG* 	AS_pn27UP; 		 //Up��ѹ��ֵ
extern LONG* 	AS_pn59U1; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn27U1; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn59U2; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn27U2; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn59Ud1; 		 //Ud1��ѹ��ֵ
extern LONG* 	AS_pn27Ud1; 		 //Ud1��ѹ��ֵ
extern LONG* 	AS_pn59Ud2; 		 //Ud2��ѹ��ֵ
extern LONG* 	AS_pn27Ud2; 		 //Ud2��ѹ��ֵ
extern LONG* 	AS_pn59Ud3; 		 //Ud3��ѹ��ֵ
extern LONG* 	AS_pn27Ud3; 		 //Ud3��ѹ��ֵ
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnU1PTRatio; 		 //U1PT���
extern LONG* 	AS_pnU2PTRatio; 		 //U2PT���
extern LONG* 	AS_pnUd1PTRatio; 		 //Ud1PT���
extern LONG* 	AS_pnUd2PTRatio; 		 //Ud2PT���
extern LONG* 	AS_pnUd3PTRatio; 		 //Ud3PT���
extern LONG* 	AS_pnIPCTRatio; 		 //����CT���
extern LONG* 	AS_pnIMCTRatio; 		 //����CT���
extern LONG* 	AS_pnId1CTRatio; 		 //Id1CT���
extern LONG* 	AS_pnId2CTRatio; 		 //Id2CT���
extern LONG* 	AS_pnId3CTRatio; 		 //Id3CT���
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     11    //������ֵ�б�ȸ���

#define AS_COUNT     71    //������ֵ����
#define AS_BUF_SIZE     284    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 302)

//ϵ��----------------------------------
extern LONG* 	CF_pnUP; 		 //UP
extern LONG* 	CF_pnU1P; 		 //U1P
extern LONG* 	CF_pnU2P; 		 //U2P
extern LONG* 	CF_pnUd1P; 		 //Ud1P
extern LONG* 	CF_pnUd2P; 		 //Ud2P
extern LONG* 	CF_pnUd3P; 		 //Ud3P
extern LONG* 	CF_pnIP; 		 //IP
extern LONG* 	CF_pnI1P; 		 //I1P
extern LONG* 	CF_pnI2P; 		 //I2P
extern LONG* 	CF_pnI3P; 		 //I3P
extern LONG* 	CF_pnUM; 		 //UM
extern LONG* 	CF_pnU1M; 		 //U1M
extern LONG* 	CF_pnU2M; 		 //U2M
extern LONG* 	CF_pnUd1M; 		 //Ud1M
extern LONG* 	CF_pnUd2M; 		 //Ud2M
extern LONG* 	CF_pnUd3M; 		 //Ud3M
extern LONG* 	CF_pnIM; 		 //IM
extern LONG* 	CF_pnI1M; 		 //I1M
extern LONG* 	CF_pnI2M; 		 //I2M
extern LONG* 	CF_pnI3M; 		 //I3M
extern LONG* 	CF_pnP; 		 //P
extern LONG* 	CF_pnQ; 		 //Q
extern LONG* 	CF_pnA; 		 //A
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnDRIFTU; 		 //DRIFTU
extern LONG* 	CF_pnDRIFTU1; 		 //DRIFTU1
extern LONG* 	CF_pnDRIFTU2; 		 //DRIFTU2
extern LONG* 	CF_pnDRIFTUD1; 		 //DRIFTUD1
extern LONG* 	CF_pnDRIFTUD2; 		 //DRIFTUD2
extern LONG* 	CF_pnDRIFTUD3; 		 //DRIFTUD3
extern LONG* 	CF_pnDRIFTIM; 		 //DRIFTIM
extern LONG* 	CF_pnDRIFTI1; 		 //DRIFTI1
extern LONG* 	CF_pnDRIFTI2; 		 //DRIFTI2
extern LONG* 	CF_pnDRIFTI3; 		 //DRIFTI3

#define CF_COUNT     37    //ϵ������
#define CF_BUF_SIZE     148    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 586)

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
#define R_ADERR_R                  13    //AD���ϻָ�
#define R_SLERR_R                  14    //��ѹ����ϻָ�
#define R_EEPROMERR_R              15    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 16    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 17    //����ROM���ϻָ�
#define R_COMERR_R                 18    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               19    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               20    //�߼����̹��Ϲ��ϻָ�
#define R_ACTPICK                  21    //��������
#define R_R59U_PI                  22    //��ѹ��������
#define R_R59U                     23    //��ѹ��������
#define R_R59U_RES                 24    //��ѹ��������
#define R_R50IH_PI                 25    //г��������������
#define R_R50IH                    26    //г��������������
#define R_R50IH_RES                27    //г��������������
#define R_R60I1_PI                 28    //֧·1������������
#define R_R60I1                    29    //֧·1������������
#define R_R60I1_RES                30    //֧·1������������
#define R_R60I2_PI                 31    //֧·2������������
#define R_R60I2                    32    //֧·2������������
#define R_R60I2_RES                33    //֧·2������������
#define R_R50ST_PI                 34    //֧·������������
#define R_R50ST                    35    //֧·������������
#define R_R50ST_RES                36    //֧·������������
#define R_R60U1_PI                 37    //֧·1��ѹ��������
#define R_R60U1                    38    //֧·1��ѹ��������
#define R_R60U1_RES                39    //֧·1��ѹ��������
#define R_R60U2_PI                 40    //֧·2��ѹ��������
#define R_R60U2                    41    //֧·2��ѹ��������
#define R_R60U2_RES                42    //֧·2��ѹ��������
#define R_R60U3_PI                 43    //֧·3��ѹ��������
#define R_R60U3                    44    //֧·3��ѹ��������
#define R_R60U3_RES                45    //֧·3��ѹ��������
#define R_R50I0_PI                 46    //�����ٶ�����
#define R_R50I0                    47    //�����ٶ϶���
#define R_R50I0_RES                48    //�����ٶϷ���
#define R_R50I1_PI                 49    //������������
#define R_R50I1                    50    //������������
#define R_R50I1_RES                51    //������������
#define R_R27U                     52    //��ѹ��������
#define R_PTBR                     53    //PT����
#define R_PTBR_RES                 54    //PT���߻ָ�
#define R_CTBR                     55    //CT����
#define R_CTBR_RES                 56    //CT���߻ָ�
#define R_TRFAIL                   57    //��·����բʧ��
#define R_COMTEST                  58    //ͨ������
#define R_PICKUP                   59    //��������
#define R_POWERERR                 60    //5V��Դ����
#define R_EEPROM_W_ERR             61    //EEPROMд�����
#define R_POWERERR_R               62    //5V��Դ���ϻָ�
#define R_RELDATAERR               63    //���������쳣
#define R_RELDATAERR_R             64    //���������쳣�ָ�
#define R_ZWS                      65    //����˹
#define R_YLSF                     66    //ѹ���ͷ�
#define R_CWTZ                     67    //������բ
#define R_QWS                      68    //����˹
#define R_YWYC                     69    //��λ�쳣
#define R_WDGG                     70    //�¶ȹ���
#define MAX_REPORT_INDEX           70    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     7    //��������

#endif