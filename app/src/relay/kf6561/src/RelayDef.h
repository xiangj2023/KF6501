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
#define 	 FAILDATA_DISPTYPE_CFGNO 	 47 		 //���ϵ�����ʾ�����������
#define 	 LangTYPE_SYSSETNO 	 0 		 //�������͸�����ֵ���

//��ǰ��ֵ���Ŷ���-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //��ǰ��ֵ����

//��ֵȡֵ�궨��

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----���ѹ-----
#define 	PHASEU 	0	//57.7V
#define 	LINEU 	1	//100V

//��������ֵ����-----------------------
#define  RM_UA 0
extern LONG* 	RM_pnUa; 		 // Ua
#define  RM_UB 1
extern LONG* 	RM_pnUb; 		 // Ub
#define  RM_UC 2
extern LONG* 	RM_pnUc; 		 // Uc
#define  RM_U0 3
extern LONG* 	RM_pnU0; 		 // U0
#define  RM_UL1 4
extern LONG* 	RM_pnUL1; 		 // UL1
#define  RM_UL2 5
extern LONG* 	RM_pnUL2; 		 // UL2
#define  RM_UL3 6
extern LONG* 	RM_pnUL3; 		 // UL3
#define  RM_UAB 7
extern LONG* 	RM_pnUab; 		 // Uab
#define  RM_UBC 8
extern LONG* 	RM_pnUbc; 		 // Ubc
#define  RM_UCA 9
extern LONG* 	RM_pnUca; 		 // Uca
#define  RM_U2 10
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_U05 11
extern LONG* 	RM_pnU05; 		 // U05
#define  RM_IA 12
extern LONG* 	RM_pnIa; 		 // Ia
#define  RM_IB 13
extern LONG* 	RM_pnIb; 		 // Ib
#define  RM_IC 14
extern LONG* 	RM_pnIc; 		 // Ic
#define  RM_I0 15
extern LONG* 	RM_pnI0; 		 // I0
#define  RM_I1 16
extern LONG* 	RM_pnI1; 		 // I1
#define  RM_I05 17
extern LONG* 	RM_pnI05; 		 // I05
#define  RM_AUA 18
extern LONG* 	RM_pnAUa; 		 // ��Ua
#define  RM_AUB 19
extern LONG* 	RM_pnAUb; 		 // ��Ub
#define  RM_AUC 20
extern LONG* 	RM_pnAUc; 		 // ��Uc
#define  RM_AUAB 21
extern LONG* 	RM_pnAUab; 		 // ��Uab
#define  RM_AUBC 22
extern LONG* 	RM_pnAUbc; 		 // ��Ubc
#define  RM_AUCA 23
extern LONG* 	RM_pnAUca; 		 // ��Uca
#define  RM_AUL1 24
extern LONG* 	RM_pnAUl1; 		 // ��Ul1
#define  RM_AUL2 25
extern LONG* 	RM_pnAUl2; 		 // ��Ul2
#define  RM_AUL3 26
extern LONG* 	RM_pnAUl3; 		 // ��Ul3
#define  RM_AIPA 27
extern LONG* 	RM_pnAIpa; 		 // ��Ipa
#define  RM_AIPB 28
extern LONG* 	RM_pnAIpb; 		 // ��Ipb
#define  RM_AIPC 29
extern LONG* 	RM_pnAIpc; 		 // ��Ipc
#define  RM_AIP1 30
extern LONG* 	RM_pnAIP1; 		 // ��Ip1
#define  RM_AIMA 31
extern LONG* 	RM_pnAIma; 		 // ��Ima
#define  RM_AIMB 32
extern LONG* 	RM_pnAImb; 		 // ��Imb
#define  RM_AIMC 33
extern LONG* 	RM_pnAImc; 		 // ��Imc
#define  RM_AU01 34
extern LONG* 	RM_pnAU01; 		 // ��U01
#define  RM_AI01 35
extern LONG* 	RM_pnAI01; 		 // ��I01
#define  RM_AU05 36
extern LONG* 	RM_pnAU05; 		 // ��U05
#define  RM_AI05 37
extern LONG* 	RM_pnAI05; 		 // ��I05
#define  RM_F1 38
extern LONG* 	RM_pnF1; 		 // F1
#define  RM_IAM 39
extern LONG* 	RM_pnIam; 		 // Iam
#define  RM_IBM 40
extern LONG* 	RM_pnIbm; 		 // Ibm
#define  RM_ICM 41
extern LONG* 	RM_pnIcm; 		 // Icm
#define  RM_BOBRCH1 42
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 43
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 44
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 45
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     46    //��������ֵ����
#define RM_BUF_SIZE     184    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UA 0
extern LONG* 	M_pnUa; 		 //Ua Ua
#define  M_UB 1
extern LONG* 	M_pnUb; 		 //Ub Ub
#define  M_UC 2
extern LONG* 	M_pnUc; 		 //Uc Uc
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_UAB 4
extern LONG* 	M_pnUab; 		 //Uab Uab
#define  M_UBC 5
extern LONG* 	M_pnUbc; 		 //Ubc Ubc
#define  M_UCA 6
extern LONG* 	M_pnUca; 		 //Uca Uca
#define  M_U1 7
extern LONG* 	M_pnU1; 		 //U1 U1
#define  M_U2 8
extern LONG* 	M_pnU2; 		 //U2 U2
#define  M_U3 9
extern LONG* 	M_pnU3; 		 //U3 U3
#define  M_IA 10
extern LONG* 	M_pnIa; 		 //Ia Ia
#define  M_IB 11
extern LONG* 	M_pnIb; 		 //Ib Ib
#define  M_IC 12
extern LONG* 	M_pnIc; 		 //Ic Ic
#define  M_I0 13
extern LONG* 	M_pnI0; 		 //I0 I0
#define  M_I1 14
extern LONG* 	M_pnI1; 		 //I1 I1
#define  M_P 15
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 16
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 17
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_F1 18
extern LONG* 	M_pnF1; 		 //F1 F1

#define MEASURE_COUNT     19    //����ֵ����
#define MEASURE_BUF_SIZE     76    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 188)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby50I0; 		 //�����ٶ�
extern BYTE* 	SW_pby50I1; 		 //��������
extern BYTE* 	SW_pby59U; 		 //��ѹ����
extern BYTE* 	SW_pby27U; 		 //��ѹ����
extern BYTE* 	SW_pby60I; 		 //��ƽ�����
extern BYTE* 	SW_pby60U; 		 //��ƽ���ѹ
extern BYTE* 	SW_pbyJDXX; 		 //�ӵ�ѡ��1
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     10    //��ѹ�����
#define SW_BUF_SIZE     10    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 264)

//����----------------------------------
extern BYTE* 	CFG_pby50I0; 		 //�����ٶ�
extern BYTE* 	CFG_pby50I1; 		 //��������
extern BYTE* 	CFG_pby59U; 		 //��ѹ����
extern BYTE* 	CFG_pby27U; 		 //��ѹ����
extern BYTE* 	CFG_pby60I; 		 //��ƽ���������
extern BYTE* 	CFG_pby60U; 		 //��ƽ���ѹ����
extern BYTE* 	CFG_pby59N; 		 //��ѹ�澯
extern BYTE* 	CFG_pbyJDXX; 		 //�ӵ�ѡ��
extern BYTE* 	CFG_pbyXHXQ; 		 //������Ȧ�ӵ�
extern BYTE* 	CFG_pbyPTBR; 		 //PT���߼��
extern BYTE* 	CFG_pbyCTBR; 		 //CT���߼��
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     14    //���ø���/
#define CFG_BUF_SIZE     14    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 276)

//��ֵ----------------------------------
extern LONG* 	RS_pnCFG; 		 //������ CFG
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn50I0C; 		 //�����ٶϵ��� 50I0C
extern LONG* 	RS_pn50I0D; 		 //�����ٶ�ʱ�� 50I0D
extern LONG* 	RS_pn50I1C; 		 //����������ֵ 50I1C
extern LONG* 	RS_pn50I1D; 		 //����ʱ�䶨ֵ 50I1D
extern LONG* 	RS_pn59U; 		 //��ѹ��ѹ��ֵ 59U
extern LONG* 	RS_pn59D; 		 //��ѹʱ�䶨ֵ 59D
extern LONG* 	RS_pn27U; 		 //��ѹ������ѹ 27U
extern LONG* 	RS_pn27D; 		 //��ѹ����ʱ�� 27D
extern LONG* 	RS_pn27ILO; 		 //��ѹ����������ֵ 27ILO
extern LONG* 	RS_pn60IC; 		 //��ƽ������������� 60IC
extern LONG* 	RS_pn60ID; 		 //��ƽ���������ʱ�� 60ID
extern LONG* 	RS_pn60UV; 		 //��ƽ���ѹ������ѹ 60UV
extern LONG* 	RS_pn60UD; 		 //��ƽ���ѹ����ʱ�� 60UD
extern LONG* 	RS_pn59NU; 		 //��ѹ�澯��ѹ 59NU
extern LONG* 	RS_pn59ND; 		 //��ѹ�澯ʱ�� 59ND
extern LONG* 	RS_pn74PTU; 		 //PT���߼���ѹ 74PTU
extern LONG* 	RS_pn74PTI; 		 //PT���߼����� 74PTI
extern LONG* 	RS_pn74PTD; 		 //PT���߼��ʱ�� 74PTD
extern LONG* 	RS_pn74PTU2; 		 //�����ѹ��ⶨֵ 74PTU2
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD

#define RS_COUNT     22    //��ֵ����/
#define RS_BUF_SIZE     88    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 292)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnPM; 		 //���ѹ
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
extern LONG* 	AS_pnTSW; 		 //���������ʱ
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnTR3D; 		 //TR3����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��
extern LONG* 	AS_pn59UA; 		 //Ua��ѹ��ֵ
extern LONG* 	AS_pn27UA; 		 //Ua��ѹ��ֵ
extern LONG* 	AS_pn59UB; 		 //Ub��ѹ��ֵ
extern LONG* 	AS_pn27UB; 		 //Ub��ѹ��ֵ
extern LONG* 	AS_pn59UC; 		 //Uc��ѹ��ֵ
extern LONG* 	AS_pn27UC; 		 //Uc��ѹ��ֵ
extern LONG* 	AS_pn59U0; 		 //U0��ѹ��ֵ
extern LONG* 	AS_pn27U0; 		 //U0��ѹ��ֵ
extern LONG* 	AS_pn59U1; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn27U1; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn59U2; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn27U2; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn59U3; 		 //U3��ѹ��ֵ
extern LONG* 	AS_pn27U3; 		 //U3��ѹ��ֵ
extern LONG* 	AS_pn50IA; 		 //Ia������ֵ
extern LONG* 	AS_pn37IA; 		 //Ia������ֵ
extern LONG* 	AS_pn50IB; 		 //Ib������ֵ
extern LONG* 	AS_pn37IB; 		 //Ib������ֵ
extern LONG* 	AS_pn50IC; 		 //Ic������ֵ
extern LONG* 	AS_pn37IC; 		 //Ic������ֵ
extern LONG* 	AS_pn50I0; 		 //I0������ֵ
extern LONG* 	AS_pn37I0; 		 //I0������ֵ
extern LONG* 	AS_pn50I1; 		 //I1������ֵ
extern LONG* 	AS_pn37I1; 		 //I1������ֵ

//���----------------------------------
extern LONG* 	AS_pnPTRatio; 		 //PT���
extern LONG* 	AS_pnPCTRatio; 		 //����CT���
extern LONG* 	AS_pnMCTRatio; 		 //����CT���
extern LONG* 	AS_pnZCTRatio; 		 //����CT���
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     5    //������ֵ�б�ȸ���

#define AS_COUNT     69    //������ֵ����
#define AS_BUF_SIZE     276    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 380)

//ϵ��----------------------------------
extern LONG* 	CF_pnUp; 		 //Up
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnIp; 		 //Ip
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnI1; 		 //I1
extern LONG* 	CF_pnUaM; 		 //UaM
extern LONG* 	CF_pnUbM; 		 //UbM
extern LONG* 	CF_pnUcM; 		 //UcM
extern LONG* 	CF_pnUL1M; 		 //UL1M
extern LONG* 	CF_pnUL2M; 		 //UL2M
extern LONG* 	CF_pnUL3M; 		 //UL3M
extern LONG* 	CF_pnU0M; 		 //U0M
extern LONG* 	CF_pnIAM; 		 //IAM
extern LONG* 	CF_pnIBM; 		 //IBM
extern LONG* 	CF_pnICM; 		 //ICM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnI1M; 		 //I1M
extern LONG* 	CF_pnPA; 		 //PA
extern LONG* 	CF_pnQA; 		 //QA
extern LONG* 	CF_pnPB; 		 //PB
extern LONG* 	CF_pnQB; 		 //QB
extern LONG* 	CF_pnPC; 		 //PC
extern LONG* 	CF_pnQC; 		 //QC
extern LONG* 	CF_pnAA; 		 //AA
extern LONG* 	CF_pnBA; 		 //BA
extern LONG* 	CF_pnCA; 		 //CA
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnIaDRIFT; 		 //IaDRIFT
extern LONG* 	CF_pnIbDRIFT; 		 //IbDRIFT
extern LONG* 	CF_pnIcDRIFT; 		 //IcDRIFT
extern LONG* 	CF_pnI0DRIFT; 		 //I0DRIFT
extern LONG* 	CF_pnI1DRIFT; 		 //I1DRIFT
extern LONG* 	CF_pnUaDRIFT; 		 //UaDRIFT
extern LONG* 	CF_pnUbDRIFT; 		 //UbDRIFT
extern LONG* 	CF_pnUcDRIFT; 		 //UcDRIFT
extern LONG* 	CF_pnU0DRIFT; 		 //U0DRIFT
extern LONG* 	CF_pnUl1DRIFT; 		 //Ul1DRIFT
extern LONG* 	CF_pnUl2DRIFT; 		 //Ul2DRIFT
extern LONG* 	CF_pnUl3DRIFT; 		 //Ul3DRIFT

#define CF_COUNT     42    //ϵ������
#define CF_BUF_SIZE     168    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 656)

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
#define R_ADERR_R                  12    //AD���ϻָ�
#define R_SLERR_R                  13    //��ѹ����ϻָ�
#define R_EEPROMERR_R              14    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 15    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 16    //����ROM���ϻָ�
#define R_COMERR_R                 17    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               18    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               19    //�߼����̹��ϻָ�
#define R_ACTPICK                  20    //��������
#define R_R50I0_PI                 21    //�����ٶ�����
#define R_R50I0                    22    //�����ٶ϶���
#define R_R50I0_RES                23    //�����ٶϷ���
#define R_R50I1_PI                 24    //������������
#define R_R50I1                    25    //������������
#define R_R50I1_RES                26    //������������
#define R_R60I_PI                  27    //��ƽ�������������
#define R_R60I                     28    //��ƽ�������������
#define R_R60I_RES                 29    //��ƽ�������������
#define R_R60U_PI                  30    //��ƽ���ѹ��������
#define R_R60U                     31    //��ƽ���ѹ��������
#define R_R60U_RES                 32    //��ƽ���ѹ��������
#define R_R59U_PI                  33    //��ѹ��������
#define R_R59U                     34    //��ѹ��������
#define R_R59U_RES                 35    //��ѹ��������
#define R_R27U                     36    //��ѹ��������
#define R_R59NU                    37    //��ѹ�澯
#define R_R74PT                    38    //ĸ��PT����
#define R_64LINE                   39    //��·�ӵ�
#define R_64BUS                    40    //ĸ�߽ӵ�
#define R_PICKUP                   41    //��������
#define R_TRFAIL                   42    //��·����բʧ��
#define R_COMTEST                  43    //ͨ������
#define R_52BF                     44    //ʧ�鱣��
#define R_CTBR_A                   45    //A��CT����
#define R_CTBR_A_RES               46    //A��CT���߻ָ�
#define R_CTBR_B                   47    //B��CT����
#define R_CTBR_B_RES               48    //B��CT���߻ָ�
#define R_CTBR_C                   49    //C��CT����
#define R_CTBR_C_RES               50    //C��CT���߻ָ�
#define R_POWERERR                 51    //5V��Դ����
#define R_EEPROM_W_ERR             52    //EEPROMд�����
#define R_POWERERR_R               53    //5V��Դ���ϻָ�
#define R_R74PT_RES                54    //ĸ��PT���߻ָ�
#define R_RELDATAERR               55    //���������쳣
#define R_RELDATAERR_R             56    //���������쳣�ָ�
#define MAX_REPORT_INDEX           56    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     10    //��������

#endif