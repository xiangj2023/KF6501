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
//----�Ͳ����·��-----
#define 	LCUR_2 	0	//�Ͳ���·����
#define 	LCUR_4 	1	//�Ͳ���·����

//��������ֵ����-----------------------
#define  RM_UHAB 0
extern LONG* 	RM_pnUHAB; 		 // UHAB
#define  RM_UHBC 1
extern LONG* 	RM_pnUHBC; 		 // UHBC
#define  RM_UHCA 2
extern LONG* 	RM_pnUHCA; 		 // UHCA
#define  RM_UHA 3
extern LONG* 	RM_pnUHA; 		 // UHA
#define  RM_UHB 4
extern LONG* 	RM_pnUHB; 		 // UHB
#define  RM_UHC 5
extern LONG* 	RM_pnUHC; 		 // UHC
#define  RM_U0 6
extern LONG* 	RM_pnU0; 		 // U0
#define  RM_ULA 7
extern LONG* 	RM_pnULA; 		 // ULA
#define  RM_ULB 8
extern LONG* 	RM_pnULB; 		 // ULB
#define  RM_ULAB 9
extern LONG* 	RM_pnULAB; 		 // ULAB
#define  RM_IHA 10
extern LONG* 	RM_pnIHA; 		 // IHA
#define  RM_IHB 11
extern LONG* 	RM_pnIHB; 		 // IHB
#define  RM_IHC 12
extern LONG* 	RM_pnIHC; 		 // IHC
#define  RM_I0 13
extern LONG* 	RM_pnI0; 		 // I0
#define  RM_IJX 14
extern LONG* 	RM_pnIJX; 		 // IJX
#define  RM_ILA 15
extern LONG* 	RM_pnILA; 		 // ILA
#define  RM_ILB 16
extern LONG* 	RM_pnILB; 		 // ILB
#define  RM_IL1 17
extern LONG* 	RM_pnIL1; 		 // IL1
#define  RM_IL2 18
extern LONG* 	RM_pnIL2; 		 // IL2
#define  RM_IL3 19
extern LONG* 	RM_pnIL3; 		 // IL3
#define  RM_IL4 20
extern LONG* 	RM_pnIL4; 		 // IL4
#define  RM_BOBRCH1 21
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 22
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 23
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 24
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     25    //��������ֵ����
#define RM_BUF_SIZE     100    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UHA 0
extern LONG* 	M_pnUHA; 		 //UHA UHA
#define  M_UHB 1
extern LONG* 	M_pnUHB; 		 //UHB UHB
#define  M_UHC 2
extern LONG* 	M_pnUHC; 		 //UHC UHC
#define  M_U0 3
extern LONG* 	M_pnU0; 		 //U0 U0
#define  M_ULA 4
extern LONG* 	M_pnULA; 		 //ULA ULA
#define  M_ULB 5
extern LONG* 	M_pnULB; 		 //ULB ULB
#define  M_IHA 6
extern LONG* 	M_pnIHA; 		 //IHA IHA
#define  M_IHB 7
extern LONG* 	M_pnIHB; 		 //IHB IHB
#define  M_IHC 8
extern LONG* 	M_pnIHC; 		 //IHC IHC
#define  M_I0 9
extern LONG* 	M_pnI0; 		 //I0 I0
#define  M_ILA 10
extern LONG* 	M_pnILA; 		 //ILA ILA
#define  M_ILB 11
extern LONG* 	M_pnILB; 		 //ILB ILB
#define  M_IJX 12
extern LONG* 	M_pnIjx; 		 //Ijx Ijx
#define  M_IL1 13
extern LONG* 	M_pnIL1; 		 //IL1 IL1
#define  M_IL2 14
extern LONG* 	M_pnIL2; 		 //IL2 IL2
#define  M_IL3 15
extern LONG* 	M_pnIL3; 		 //IL3 IL3
#define  M_IL4 16
extern LONG* 	M_pnIL4; 		 //IL4 IL4

#define MEASURE_COUNT     17    //����ֵ����
#define MEASURE_BUF_SIZE     68    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 104)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby50H; 		 //�߲��������
extern BYTE* 	SW_pby49; 		 //��ʱ�޹�����
extern BYTE* 	SW_pby50L; 		 //�Ͳ��������
extern BYTE* 	SW_pby50OL; 		 //�����ɱ���
extern BYTE* 	SW_pby27; 		 //�߲�ʧѹ����
extern BYTE* 	SW_pby50N; 		 //�����������
extern BYTE* 	SW_pby59N; 		 //�����ѹ����
extern BYTE* 	SW_pby50JX; 		 //��϶��������
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     11    //��ѹ�����
#define SW_BUF_SIZE     11    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 172)

//����----------------------------------
extern BYTE* 	CFG_pby50HA; 		 //�߲�A�����
extern BYTE* 	CFG_pby50HB; 		 //�߲�B�����
extern BYTE* 	CFG_pby50HC; 		 //�߲�C�����
extern BYTE* 	CFG_pby50H_27; 		 //�߲��ѹ����
extern BYTE* 	CFG_pby49; 		 //��ʱ�޹�����
extern BYTE* 	CFG_pby50LA1; 		 //�Ͳ�A�����I��
extern BYTE* 	CFG_pby50LA2; 		 //�Ͳ�A�����II��
extern BYTE* 	CFG_pby50LA_27; 		 //�Ͳ�A��ѹ����
extern BYTE* 	CFG_pby50LB1; 		 //�Ͳ�B�����I��
extern BYTE* 	CFG_pby50LB2; 		 //�Ͳ�B�����II��
extern BYTE* 	CFG_pby50LB_27; 		 //�Ͳ�B��ѹ����
extern BYTE* 	CFG_pby50OLA1; 		 //A�����ɸ澯
extern BYTE* 	CFG_pby50OLB1; 		 //B�����ɸ澯
extern BYTE* 	CFG_pby50OLC1; 		 //C�����ɸ澯
extern BYTE* 	CFG_pby59OLA2; 		 //A��������բ
extern BYTE* 	CFG_pby50OLB2; 		 //B��������բ
extern BYTE* 	CFG_pby50OLC2; 		 //C��������բ
extern BYTE* 	CFG_pby27; 		 //��ѹ��ʧѹ����
extern BYTE* 	CFG_pby50N; 		 //�����������
extern BYTE* 	CFG_pby59N; 		 //�����ѹ����
extern BYTE* 	CFG_pby74PT; 		 //PT���߼��
extern BYTE* 	CFG_pby50DCG; 		 //��϶��������
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     25    //���ø���/
#define CFG_BUF_SIZE     25    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 186)

//��ֵ----------------------------------
extern LONG* 	RS_pnF1; 		 //������1 F1
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn50HAC; 		 //��ѹ��A�������� 50HAC
extern LONG* 	RS_pn50HAD; 		 //��ѹ��A����ʱ�� 50HAD
extern LONG* 	RS_pn50HBC; 		 //��ѹ��B�������� 50HBC
extern LONG* 	RS_pn50HBD; 		 //��ѹ��B����ʱ�� 50HBD
extern LONG* 	RS_pn50HCC; 		 //��ѹ��C�������� 50HCC
extern LONG* 	RS_pn50HCD; 		 //��ѹ��C����ʱ�� 50HCD
extern LONG* 	RS_pn27LU; 		 //��ѹ���ѹ��ֵ 27LU
extern LONG* 	RS_pnTIN; 		 //��ѹ�����ζ���� TIN
extern LONG* 	RS_pn49K; 		 //�ȹ����ɱ������� 49K
extern LONG* 	RS_pn49T; 		 //�ȹ�����ʱ�䳣�� 49T
extern LONG* 	RS_pn49AR; 		 //�ȹ����ɸ澯��ֵ 49AR
extern LONG* 	RS_pn49KT; 		 //�豸ֹͣ����ϵ�� 49KT
extern LONG* 	RS_pn50LA1C; 		 //�Ͳ�A�����I�ζ�ֵ 50LA1C
extern LONG* 	RS_pn50LA1D; 		 //�Ͳ�A�����I��ʱ�� 50LA1D
extern LONG* 	RS_pn50LA2C; 		 //�Ͳ�A�����II�ζ�ֵ 50LA2C
extern LONG* 	RS_pn50LA2D; 		 //�Ͳ�A�����II��ʱ�� 50LA2D
extern LONG* 	RS_pn50LB1C; 		 //�Ͳ�B�����I�ζ�ֵ 50LB1C
extern LONG* 	RS_pn50LB1D; 		 //�Ͳ�B�����I��ʱ�� 50LB1D
extern LONG* 	RS_pn50LB2C; 		 //�Ͳ�B�����II�ζ�ֵ 50LB2C
extern LONG* 	RS_pn50LB2D; 		 //�Ͳ�B�����II��ʱ�� 50LB2D
extern LONG* 	RS_pn50OLA1C; 		 //A�������I�ε��� 50OLA1C
extern LONG* 	RS_pn50OLA1D; 		 //A�������I��ʱ�� 50OLA1D
extern LONG* 	RS_pn50OLB1C; 		 //B�������I�ε��� 50OLB1C
extern LONG* 	RS_pn50OLB1D; 		 //B�������I��ʱ�� 50OLB1D
extern LONG* 	RS_pn50OLC1C; 		 //C�������I�ε��� 50OLC1C
extern LONG* 	RS_pn50OLC1D; 		 //C�������I��ʱ�� 50OLC1D
extern LONG* 	RS_pn50OLA2C; 		 //A�������II�ε��� 50OLA2C
extern LONG* 	RS_pn50OLA2D; 		 //A�������II��ʱ�� 50OLA2D
extern LONG* 	RS_pn50OLB2C; 		 //B�������II�ε��� 50OLB2C
extern LONG* 	RS_pn50OLB2D; 		 //B�������II��ʱ�� 50OLB2D
extern LONG* 	RS_pn50OLC2C; 		 //C�������II�ε��� 50OLC2C
extern LONG* 	RS_pn50OLC2D; 		 //C�������II��ʱ�� 50OLC2D
extern LONG* 	RS_pn50NC; 		 //���������ֵ 50NC
extern LONG* 	RS_pn50ND; 		 //�������ʱ�� 50ND
extern LONG* 	RS_pn59NV; 		 //�����ѹ��ֵ 59NV
extern LONG* 	RS_pn59ND; 		 //�����ѹʱ�� 59ND
extern LONG* 	RS_pn27HV; 		 //��ѹ��ʧѹ��ֵ 27HV
extern LONG* 	RS_pn27HD; 		 //��ѹ��ʧѹʱ�� 27HD
extern LONG* 	RS_pn27H_LK50; 		 //�߲�ʧѹ�������� 27H_LK50
extern LONG* 	RS_pn74HPTV; 		 //��ѹ��PT���߶�ֵ 74HPTV
extern LONG* 	RS_pn74LPTV; 		 //��ѹ��PT���߶�ֵ 74LPTV
extern LONG* 	RS_pn50DCGC; 		 //��϶������������ 50DCGC
extern LONG* 	RS_pn50DCGD; 		 //��϶��������ʱ�� 50DCGD
extern LONG* 	RS_pn74PTHC; 		 //�߲�PT���߱������� 74PTHC
extern LONG* 	RS_pn74PTLC; 		 //�Ͳ�PT���߱������� 74PTLC
extern LONG* 	RS_pn74PTD; 		 //PT�����ж�ʱ�� 74PTD
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣���ж�ʱ�� 52BFD

#define RS_COUNT     49    //��ֵ����/
#define RS_BUF_SIZE     196    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 212)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
extern LONG* 	AS_pnLCN; 		 //�Ͳ����·��
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
extern LONG* 	AS_pn50HAC; 		 //IHA������ֵ
extern LONG* 	AS_pn37HAC; 		 //IHA������ֵ
extern LONG* 	AS_pn50HBC; 		 //IHB������ֵ
extern LONG* 	AS_pn37HBC; 		 //IHB������ֵ
extern LONG* 	AS_pn50HCC; 		 //IHC������ֵ
extern LONG* 	AS_pn37HCC; 		 //IHC������ֵ
extern LONG* 	AS_pn59HAV; 		 //UHA��ѹ��ֵ
extern LONG* 	AS_pn27HAV; 		 //UHA��ѹ��ֵ
extern LONG* 	AS_pn59HBV; 		 //UHB��ѹ��ֵ
extern LONG* 	AS_pn27HBV; 		 //UHB��ѹ��ֵ
extern LONG* 	AS_pn59HCV; 		 //UHC��ѹ��ֵ
extern LONG* 	AS_pn27HCV; 		 //UHC��ѹ��ֵ
extern LONG* 	AS_pn50LAC; 		 //ILA������ֵ
extern LONG* 	AS_pn37LAC; 		 //ILA������ֵ
extern LONG* 	AS_pn50LBC; 		 //ILB������ֵ
extern LONG* 	AS_pn37LBC; 		 //ILB������ֵ
extern LONG* 	AS_pn59LAV; 		 //ULA��ѹ��ֵ
extern LONG* 	AS_pn27LAV; 		 //ULA��ѹ��ֵ
extern LONG* 	AS_pn59LBV; 		 //ULB��ѹ��ֵ
extern LONG* 	AS_pn27LBV; 		 //ULB��ѹ��ֵ
extern LONG* 	AS_pn50NC; 		 //I0������ֵ
extern LONG* 	AS_pn37NC; 		 //I0������ֵ
extern LONG* 	AS_pn59NV; 		 //U0��ѹ��ֵ
extern LONG* 	AS_pn27NV; 		 //U0��ѹ��ֵ
extern LONG* 	AS_pn50JXC; 		 //IJX������ֵ
extern LONG* 	AS_pn37JXC; 		 //IJX������ֵ
extern LONG* 	AS_pnTCB; 		 //��·��������ʱ
extern LONG* 	AS_pnTSW; 		 //����������ʱ
extern LONG* 	AS_pnTR1T; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2T; 		 //TR2����ʱ��
extern LONG* 	AS_pnTR3T; 		 //TR3����ʱ��
extern LONG* 	AS_pnTR4T; 		 //TR4����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnHPTRT; 		 //�߲�PT���
extern LONG* 	AS_pnHU0PTRT; 		 //�߲�����PT���
extern LONG* 	AS_pnHCTRT; 		 //�߲�CT���
extern LONG* 	AS_pnHI0CTRT; 		 //�߲�����CT���
extern LONG* 	AS_pnLPTRT; 		 //�Ͳ�PT���
extern LONG* 	AS_pnLCT1RT; 		 //�Ͳ�CT1���
extern LONG* 	AS_pnLCT2RT; 		 //�Ͳ�CT2���
extern LONG* 	AS_pnLCT3RT; 		 //�Ͳ�CT3���
extern LONG* 	AS_pnLCT4RT; 		 //�Ͳ�CT4���
extern LONG* 	AS_pnRT1; 		 //RT1

#define AS_RT_NUM     10    //������ֵ�б�ȸ���

#define AS_COUNT     77    //������ֵ����
#define AS_BUF_SIZE     308    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 408)

//ϵ��----------------------------------
extern LONG* 	CF_pnUHAB; 		 //UHAB
extern LONG* 	CF_pnUHBC; 		 //UHBC
extern LONG* 	CF_pnUHCA; 		 //UHCA
extern LONG* 	CF_pnUHA; 		 //UHA
extern LONG* 	CF_pnUHB; 		 //UHB
extern LONG* 	CF_pnUHC; 		 //UHC
extern LONG* 	CF_pnU0; 		 //U0
extern LONG* 	CF_pnULA; 		 //ULA
extern LONG* 	CF_pnULB; 		 //ULB
extern LONG* 	CF_pnIHA; 		 //IHA
extern LONG* 	CF_pnIHB; 		 //IHB
extern LONG* 	CF_pnIHC; 		 //IHC
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnIL1; 		 //IL1
extern LONG* 	CF_pnIL2; 		 //IL2
extern LONG* 	CF_pnIL3; 		 //IL3
extern LONG* 	CF_pnIL4; 		 //IL4
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnUHAM; 		 //UHAM
extern LONG* 	CF_pnUHBM; 		 //UHBM
extern LONG* 	CF_pnUHCM; 		 //UHCM
extern LONG* 	CF_pnU0M; 		 //U0M
extern LONG* 	CF_pnULAM; 		 //ULAM
extern LONG* 	CF_pnULBM; 		 //ULBM
extern LONG* 	CF_pnIHAM; 		 //IHAM
extern LONG* 	CF_pnIHBM; 		 //IHBM
extern LONG* 	CF_pnIHCM; 		 //IHCM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnIL1M; 		 //IL1M
extern LONG* 	CF_pnIL2M; 		 //IL2M
extern LONG* 	CF_pnIL3M; 		 //IL3M
extern LONG* 	CF_pnIL4M; 		 //IL4M
extern LONG* 	CF_pnUHAD; 		 //UHAD
extern LONG* 	CF_pnUHBD; 		 //UHBD
extern LONG* 	CF_pnUHCD; 		 //UHCD
extern LONG* 	CF_pnU0D; 		 //U0D
extern LONG* 	CF_pnULAD; 		 //ULAD
extern LONG* 	CF_pnULBD; 		 //ULBD
extern LONG* 	CF_pnIHAD; 		 //IHAD
extern LONG* 	CF_pnIHBD; 		 //IHBD
extern LONG* 	CF_pnIHCD; 		 //IHCD
extern LONG* 	CF_pnI0D; 		 //I0D
extern LONG* 	CF_pnIL1D; 		 //IL1D
extern LONG* 	CF_pnIL2D; 		 //IL2D
extern LONG* 	CF_pnIL3D; 		 //IL3D
extern LONG* 	CF_pnIL4D; 		 //IL4D

#define CF_COUNT     49    //ϵ������
#define CF_BUF_SIZE     196    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 716)

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
#define R_POWERERR                 12    //5V��Դ����
#define R_EEPROM_W_ERR             13    //EEPROMд�����
#define R_ADERR_R                  14    //AD���ϻָ�
#define R_SLERR_R                  15    //��ѹ����ϻָ�
#define R_EEPROMERR_R              16    //EEPROM��ֵ���ϻָ�
#define R_RAMERR_R                 17    //RAM��ֵ���ϻָ�
#define R_ROMERR_R                 18    //����ROM���ϻָ�
#define R_COMERR_R                 19    //ͨ��ͨ���쳣�ָ�
#define R_NVRAMERR_R               20    //NVRAM���ݳ���ָ�
#define R_LOGICERR_R               21    //�߼����̹��Ϲ��ϻָ�
#define R_POWERERR_R               22    //5V��Դ���ϻָ�
#define R_ACTPICK                  23    //��������
#define R_R49AR                    24    //��ʱ�޹����ɸ澯
#define R_R49ACT                   25    //��ʱ�޹����ɶ���
#define R_R49RST                   26    //��ʱ�޹����ɷ���
#define R_R50IHA_PI                27    //��ѹ��A��������
#define R_R50IHA                   28    //��ѹ��A��������
#define R_R50IHA_RES               29    //��ѹ��A��������
#define R_R50IHB_PI                30    //��ѹ��B��������
#define R_R50IHB                   31    //��ѹ��B��������
#define R_R50IHB_RES               32    //��ѹ��B��������
#define R_R50IHC_PI                33    //��ѹ��C��������
#define R_R50IHC                   34    //��ѹ��C��������
#define R_R50IHC_RES               35    //��ѹ��C��������
#define R_R50ILA1_PI               36    //��ѹ��A�����������
#define R_R50ILA1                  37    //��ѹ��A������ζ���
#define R_R50ILA1_RES              38    //��ѹ��A������η���
#define R_R50ILA2_PI               39    //��ѹ��A�����������
#define R_R50ILA2                  40    //��ѹ��A������ζ���
#define R_R50ILA2_RES              41    //��ѹ��A������η���
#define R_R50ILB1_PI               42    //��ѹ��B�����������
#define R_R50ILB1                  43    //��ѹ��B������ζ���
#define R_R50ILB1_RES              44    //��ѹ��B������η���
#define R_R50ILB2_PI               45    //��ѹ��B�����������
#define R_R50ILB2                  46    //��ѹ��B������ζ���
#define R_R50ILB2_RES              47    //��ѹ��B������η���
#define R_R50IJX_PI                48    //��϶������������
#define R_R50IJX                   49    //��϶������������
#define R_R50IJX_RES               50    //��϶������������
#define R_R50IN_PI                 51    //���������������
#define R_R50IN                    52    //���������������
#define R_R50IN_RES                53    //���������������
#define R_R59UN_PI                 54    //�����ѹ��������
#define R_R59UN                    55    //�����ѹ��������
#define R_R59UN_RES                56    //�����ѹ��������
#define R_R50IA1                   57    //�߲�A�����ɢ�澯
#define R_R50IB1                   58    //�߲�B�����ɢ�澯
#define R_R50IC1                   59    //�߲�C�����ɢ�澯
#define R_R50IA2_PI                60    //�߲�A�����ɢ�����
#define R_R50IA2                   61    //�߲�A�����ɢ���
#define R_R50IA2_RES               62    //�߲�A�����ɢ򷵻�
#define R_R50IB2_PI                63    //�߲�B�����ɢ�����
#define R_R50IB2                   64    //�߲�B�����ɢ���
#define R_R50IB2_RES               65    //�߲�B�����ɢ򷵻�
#define R_R50IC2_PI                66    //�߲�C�����ɢ�����
#define R_R50IC2                   67    //�߲�C�����ɢ���
#define R_R50IC2_RES               68    //�߲�C�����ɢ򷵻�
#define R_R27U                     69    //��ѹ��ʧѹ����
#define R_RHPTBR                   70    //��ѹ��PT����
#define R_RHPTBRRST                71    //��ѹ��PT���߻ָ�
#define R_RLAPTBR                  72    //��ѹ��A��PT����
#define R_RLAPTBRRST               73    //��ѹ��A��PT���߻ָ�
#define R_RLBPTBR                  74    //��ѹ��B��PT����
#define R_RLBPTBRRST               75    //��ѹ��B��PT���߻ָ�
#define R_PICKUP                   76    //��������
#define R_TR_H_FAIL                77    //�߲��·����բʧ��
#define R_COMTEST                  78    //ͨ������
#define R_52BF                     79    //ʧ�鱣��
#define R_TR_LA_FAIL               80    //�Ͳ�A���·����բʧ��
#define R_TR_LB_FAIL               81    //�Ͳ�B���·����բʧ��
#define R_RELDATAERR               82    //���������쳣
#define R_RELDATAERR_R             83    //���������쳣�ָ�
#define MAX_REPORT_INDEX           83    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     21    //��������

#endif