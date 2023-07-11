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
//----����귽��-----
#define 	FOREWORD 	0	//������
#define 	BACKWORD 	1	//������

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A
//----���緽ʽ-----
#define 	BTMODE 	0	//ֱ��
#define 	AT1MODE 	1	//AT
#define 	AT2MODE 	2	//��ר

//��������ֵ����-----------------------
#define  RM_IP 0
extern LONG* 	RM_pnIp; 		 // Ip
#define  RM_IT 1
extern LONG* 	RM_pnIt; 		 // It
#define  RM_IF 2
extern LONG* 	RM_pnIf; 		 // If
#define  RM_UP 3
extern LONG* 	RM_pnUp; 		 // Up
#define  RM_U1 4
extern LONG* 	RM_pnU1; 		 // U1
#define  RM_U2 5
extern LONG* 	RM_pnU2; 		 // U2
#define  RM_U3 6
extern LONG* 	RM_pnU3; 		 // U3
#define  RM_U4 7
extern LONG* 	RM_pnU4; 		 // U4
#define  RM_TL 8
extern LONG* 	RM_pnTL; 		 // TL
#define  RM_FL 9
extern LONG* 	RM_pnFL; 		 // FL
#define  RM_SS 10
extern LONG* 	RM_pnSS; 		 // SS
#define  RM_I2 11
extern LONG* 	RM_pnI2; 		 // I2
#define  RM_I3 12
extern LONG* 	RM_pnI3; 		 // I3
#define  RM_I5 13
extern LONG* 	RM_pnI5; 		 // I5
#define  RM_ITM 14
extern LONG* 	RM_pnItm; 		 // Itm
#define  RM_IFM 15
extern LONG* 	RM_pnIfm; 		 // Ifm
#define  RM_R 16
extern LONG* 	RM_pnR; 		 // R
#define  RM_X 17
extern LONG* 	RM_pnX; 		 // X
#define  RM_ZA 18
extern LONG* 	RM_pnZA; 		 // ��Z
#define  RM_TR 19
extern LONG* 	RM_pnTR; 		 // TR
#define  RM_TX 20
extern LONG* 	RM_pnTX; 		 // TX
#define  RM_TZA 21
extern LONG* 	RM_pnTZA; 		 // ��T
#define  RM_FR 22
extern LONG* 	RM_pnFR; 		 // FR
#define  RM_FX 23
extern LONG* 	RM_pnFX; 		 // FX
#define  RM_FZA 24
extern LONG* 	RM_pnFZA; 		 // ��F
#define  RM_AUP 25
extern LONG* 	RM_pnAUP; 		 // ��UP
#define  RM_AU1 26
extern LONG* 	RM_pnAU1; 		 // ��U1
#define  RM_AU2 27
extern LONG* 	RM_pnAU2; 		 // ��U2
#define  RM_AIP 28
extern LONG* 	RM_pnAIP; 		 // ��IP
#define  RM_AIT 29
extern LONG* 	RM_pnAIT; 		 // ��IT
#define  RM_AIF 30
extern LONG* 	RM_pnAIF; 		 // ��IF
#define  RM_AITM 31
extern LONG* 	RM_pnAITM; 		 // ��ItM
#define  RM_AIFM 32
extern LONG* 	RM_pnAIFM; 		 // ��IfM
#define  RM_BOBRCH1 33
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 34
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 35
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 36
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     37    //��������ֵ����
#define RM_BUF_SIZE     148    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_U1 0
extern LONG* 	M_pnU1; 		 //U1 U1
#define  M_U2 1
extern LONG* 	M_pnU2; 		 //U2 U2
#define  M_U3 2
extern LONG* 	M_pnU3; 		 //U3 U3
#define  M_U4 3
extern LONG* 	M_pnU4; 		 //U4 U4
#define  M_IT 4
extern LONG* 	M_pnIT; 		 //IT IT
#define  M_IF 5
extern LONG* 	M_pnIF; 		 //IF IF
#define  M_P 6
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 7
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 8
extern LONG* 	M_pnCOS; 		 //COS COS
#define  M_I1 9
extern LONG* 	M_pnI1; 		 //I1 I1
#define  M_I2 10
extern LONG* 	M_pnI2; 		 //I2 I2
#define  M_I3 11
extern LONG* 	M_pnI3; 		 //I3 I3
#define  M_I5 12
extern LONG* 	M_pnI5; 		 //I5 I5
#define  M_I7 13
extern LONG* 	M_pnI7; 		 //I7 I7
#define  M_I9 14
extern LONG* 	M_pnI9; 		 //I9 I9
#define  M_I11 15
extern LONG* 	M_pnI11; 		 //I11 I11
#define  M_R 16
extern LONG* 	M_pnR; 		 //R R
#define  M_X 17
extern LONG* 	M_pnX; 		 //X X
#define  M_F 18
extern LONG* 	M_pnF; 		 //F F

#define MEASURE_COUNT     19    //����ֵ����
#define MEASURE_BUF_SIZE     76    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 152)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby21; 		 //���뱣��
extern BYTE* 	SW_pby50_0; 		 //�����ٶ�
extern BYTE* 	SW_pby50_123; 		 //��������
extern BYTE* 	SW_pby50D; 		 //������������
extern BYTE* 	SW_pby27; 		 //��ѹ����
extern BYTE* 	SW_pby79; 		 //�غ�բ
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyRESD1; 		 //����ѹ��1
extern BYTE* 	SW_pbyRESD2; 		 //����ѹ��2

#define SW_COUNT     9    //��ѹ�����
#define SW_BUF_SIZE     9    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 228)

//����----------------------------------
extern BYTE* 	CFG_pby21_1; 		 //����I��
extern BYTE* 	CFG_pby21_1_D; 		 //����I������
extern BYTE* 	CFG_pby21_2; 		 //����II��
extern BYTE* 	CFG_pby21_2_D; 		 //����II������
extern BYTE* 	CFG_pby21_3; 		 //����III��
extern BYTE* 	CFG_pby21_3_D; 		 //����III������
extern BYTE* 	CFG_pby21_4; 		 //����IV��
extern BYTE* 	CFG_pby21_4_D; 		 //����IV������
extern BYTE* 	CFG_pbyParallelC; 		 //ƽ���ı�������
extern BYTE* 	CFG_pby21_HLK; 		 //����г������
extern BYTE* 	CFG_pby21_HRES; 		 //����г������
extern BYTE* 	CFG_pby50_0; 		 //�����ٶ�
extern BYTE* 	CFG_pby50_0_27; 		 //�ٶϵ�ѹ����
extern BYTE* 	CFG_pby50_0_HLK; 		 //�ٶ�г������
extern BYTE* 	CFG_pby50_0_HRES; 		 //�ٶ�г������
extern BYTE* 	CFG_pby50_0_D; 		 //�ٶϱ���������
extern BYTE* 	CFG_pby50_0_67; 		 //�ٶϹ��ʷ���
extern BYTE* 	CFG_pby50_1; 		 //����I��
extern BYTE* 	CFG_pby50_1_D; 		 //����I��������
extern BYTE* 	CFG_pby50_1_67; 		 //����I�ι��ʷ���
extern BYTE* 	CFG_pby50_1_27; 		 //����I�ε�ѹ����
extern BYTE* 	CFG_pby50_2; 		 //����II��
extern BYTE* 	CFG_pby50_2_D; 		 //����II��������
extern BYTE* 	CFG_pby50_2_67; 		 //����II�ι��ʷ���
extern BYTE* 	CFG_pby50_2_27; 		 //����II�ε�ѹ����
extern BYTE* 	CFG_pby50_3; 		 //����III��
extern BYTE* 	CFG_pby50_HLK; 		 //����г������
extern BYTE* 	CFG_pby50_HRES; 		 //����г������
extern BYTE* 	CFG_pby51; 		 //��ʱ�޹���
extern BYTE* 	CFG_pby50D; 		 //������������
extern BYTE* 	CFG_pby79UC; 		 //͵���غ�բ
extern BYTE* 	CFG_pby79_21; 		 //���뱣���غ�
extern BYTE* 	CFG_pby79_50_0; 		 //�����ٶ��غ�
extern BYTE* 	CFG_pby79_50_123; 		 //���������غ�
extern BYTE* 	CFG_pby79_51; 		 //��ʱ���غ�
extern BYTE* 	CFG_pby79_50D; 		 //���������غ�
extern BYTE* 	CFG_pby79_59; 		 //�غ�բ����ѹ
extern BYTE* 	CFG_pby21_2_94; 		 //����II�μ���
extern BYTE* 	CFG_pby21_3_94; 		 //����III�μ���
extern BYTE* 	CFG_pby21_4_94; 		 //����IV�μ���
extern BYTE* 	CFG_pby50_1_94; 		 //����I�μ���
extern BYTE* 	CFG_pby50_2_94; 		 //����II�μ���
extern BYTE* 	CFG_pby50_3_94; 		 //����III�μ���
extern BYTE* 	CFG_pby27; 		 //��ѹ����
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyPT1BR; 		 //PT1���߼��
extern BYTE* 	CFG_pbyPT2BR; 		 //PT2���߼��
extern BYTE* 	CFG_pby21_FL; 		 //���ϲ��
extern BYTE* 	CFG_pbyLOADREC; 		 //����¼��
extern BYTE* 	CFG_pbyCTBR; 		 //CT���߼��
extern BYTE* 	CFG_pbyRESD1; 		 //��������1
extern BYTE* 	CFG_pbyRESD2; 		 //��������2

#define CFG_COUNT     52    //���ø���/
#define CFG_BUF_SIZE     52    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 238)

//��ֵ----------------------------------
extern LONG* 	RS_pnFUNCTION1; 		 //������1 FUNCTION1
extern LONG* 	RS_pnFUNCTION2; 		 //������2 FUNCTION2
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn21_1R; 		 //����I�ε��� 21_1R
extern LONG* 	RS_pn21_1X; 		 //����I�ε翹 21_1X
extern LONG* 	RS_pn21_1D; 		 //����I��ʱ�� 21_1D
extern LONG* 	RS_pn21_2R; 		 //����II�ε��� 21_2R
extern LONG* 	RS_pn21_2X; 		 //����II�ε翹 21_2X
extern LONG* 	RS_pn21_2D; 		 //����II��ʱ�� 21_2D
extern LONG* 	RS_pn21_3R; 		 //����III�ε��� 21_3R
extern LONG* 	RS_pn21_3X; 		 //����III�ε翹 21_3X
extern LONG* 	RS_pn21_3D; 		 //����III��ʱ�� 21_3D
extern LONG* 	RS_pn21_4R; 		 //����IV�ε��� 21_4R
extern LONG* 	RS_pn21_4X; 		 //����IV�ε翹 21_4X
extern LONG* 	RS_pn21_4D; 		 //����IV��ʱ�� 21_4D
extern LONG* 	RS_pn21ZOFFSET; 		 //ƽ���ı���ƫ���迹 21ZOFFSET
extern LONG* 	RS_pn21AOFFSET; 		 //�迹�ı���ƫ�ƽǶ� 21AOFFSET
extern LONG* 	RS_pn50_0C; 		 //�����ٶϵ��� 50_0C
extern LONG* 	RS_pn50_0D; 		 //�����ٶ�ʱ�� 50_0D
extern LONG* 	RS_pn50_0_27V; 		 //�ٶϵ�ѹ���� 50_0_27V
extern LONG* 	RS_pn50_1C; 		 //����I�ε��� 50_1C
extern LONG* 	RS_pn50_1D; 		 //����I��ʱ�� 50_1D
extern LONG* 	RS_pn50_1_27V; 		 //����I�ε�ѹ���� 50_1_27V
extern LONG* 	RS_pn50_2C; 		 //����II�ε��� 50_2C
extern LONG* 	RS_pn50_2D; 		 //����II��ʱ�� 50_2D
extern LONG* 	RS_pn50_2_27V; 		 //����II�ε�ѹ���� 50_2_27V
extern LONG* 	RS_pn50_3C; 		 //����III�ε��� 50_3C
extern LONG* 	RS_pn50_3D; 		 //����III��ʱ�� 50_3D
extern LONG* 	RS_pn51C; 		 //��ʱ�޹������� 51C
extern LONG* 	RS_pn51TD; 		 //��ʱ�޹���ʱ�� 51TD
extern LONG* 	RS_pn51TYPE; 		 //��ʱ���������� 51TYPE
extern LONG* 	RS_pn50DC; 		 //���������������� 50DC
extern LONG* 	RS_pn50DD; 		 //������������ʱ�� 50DD
extern LONG* 	RS_pn50D_37C; 		 //��������С�������� 50D_37C
extern LONG* 	RS_pnH2_PCT; 		 //����г������ϵ�� H2_PCT
extern LONG* 	RS_pnFACTOR1; 		 //�������г����Ȩ FACTOR1
extern LONG* 	RS_pnFACTOR2; 		 //��������г����Ȩ FACTOR2
extern LONG* 	RS_pnZANGLE; 		 //��·�迹�� ZANGLE
extern LONG* 	RS_pn79SH; 		 //�غ�բͶ����� 79SH
extern LONG* 	RS_pn79D1; 		 //��һ���غ�բʱ�� 79D1
extern LONG* 	RS_pn79D2; 		 //�ڶ����غ�բʱ�� 79D2
extern LONG* 	RS_pn79RD; 		 //�غ�բ���ʱ�� 79RD
extern LONG* 	RS_pn79CLSD; 		 //�غ�բ��������ʱ�� 79CLSD
extern LONG* 	RS_pn94D; 		 //����ٶ���ʱ�� 94D
extern LONG* 	RS_pnSOTFD; 		 //�����Ͷ��ʱ�� SOTFD
extern LONG* 	RS_pn27V; 		 //��ѹ������ѹ 27V
extern LONG* 	RS_pn27D; 		 //��ѹ����ʱ�� 27D
extern LONG* 	RS_pn27_37C; 		 //��ѹ�������� 27_37C
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD
extern LONG* 	RS_pn74VTV; 		 //PT���߼���ѹ 74VTV
extern LONG* 	RS_pn74VTBLC; 		 //PT���߱������� 74VTBLC
extern LONG* 	RS_pn74VTC; 		 //PT���߼����� 74VTC
extern LONG* 	RS_pn74VTD; 		 //PT���߼��ʱ�� 74VTD
extern LONG* 	RS_pnS0; 		 //����� S0
extern LONG* 	RS_pnSD; 		 //����귽�� SD
extern LONG* 	RS_pnLc; 		 //�������� Lc
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
extern LONG* 	RS_pnT21FLN7L; 		 //T�ߵ��߶ξ��� T21FLN7L
extern LONG* 	RS_pnT21FLN7X; 		 //T�ߵ��߶ε�λ�翹 T21FLN7X
extern LONG* 	RS_pnT21FLN8L; 		 //T�ߵڰ˶ξ��� T21FLN8L
extern LONG* 	RS_pnT21FLN8X; 		 //T�ߵڰ˶ε�λ�翹 T21FLN8X
extern LONG* 	RS_pnT21FLN9L; 		 //T�ߵھŶξ��� T21FLN9L
extern LONG* 	RS_pnT21FLN9X; 		 //T�ߵھŶε�λ�翹 T21FLN9X
extern LONG* 	RS_pnT21FLN10L; 		 //T�ߵ�ʮ�ξ��� T21FLN10L
extern LONG* 	RS_pnT21FLN10X; 		 //T�ߵ�ʮ�ε�λ�翹 T21FLN10X
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
extern LONG* 	RS_pnF21FLN7L; 		 //F�ߵ��߶ξ��� F21FLN7L
extern LONG* 	RS_pnF21FLN7X; 		 //F�ߵ��߶ε�λ�翹 F21FLN7X
extern LONG* 	RS_pnF21FLN8L; 		 //F�ߵڰ˶ξ��� F21FLN8L
extern LONG* 	RS_pnF21FLN8X; 		 //F�ߵڰ˶ε�λ�翹 F21FLN8X
extern LONG* 	RS_pnF21FLN9L; 		 //F�ߵھŶξ��� F21FLN9L
extern LONG* 	RS_pnF21FLN9X; 		 //F�ߵھŶε�λ�翹 F21FLN9X
extern LONG* 	RS_pnF21FLN10L; 		 //F�ߵ�ʮ�ξ��� F21FLN10L
extern LONG* 	RS_pnF21FLN10X; 		 //F�ߵ�ʮ�ε�λ�翹 F21FLN10X

#define RS_COUNT     98    //��ֵ����/
#define RS_BUF_SIZE     392    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 290)

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
extern LONG* 	AS_pnTSW; 		 //���������ʱ
extern LONG* 	AS_pn50IPC; 		 //IP������ֵ
extern LONG* 	AS_pn37IPC; 		 //IP������ֵ
extern LONG* 	AS_pn50ITC; 		 //It������ֵ
extern LONG* 	AS_pn37ITC; 		 //It������ֵ
extern LONG* 	AS_pn50IFC; 		 //If������ֵ
extern LONG* 	AS_pn37IFC; 		 //If������ֵ
extern LONG* 	AS_pn59UPV; 		 //Up��ѹ��ֵ
extern LONG* 	AS_pn27UPV; 		 //Up��ѹ��ֵ
extern LONG* 	AS_pn59U1V; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn27U1V; 		 //U1��ѹ��ֵ
extern LONG* 	AS_pn59U2V; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn27U2V; 		 //U2��ѹ��ֵ
extern LONG* 	AS_pn59U3V; 		 //U3��ѹ��ֵ
extern LONG* 	AS_pn27U3V; 		 //U3��ѹ��ֵ
extern LONG* 	AS_pn59U4V; 		 //U4��ѹ��ֵ
extern LONG* 	AS_pn27U4V; 		 //U4��ѹ��ֵ
extern LONG* 	AS_pnLMT; 		 //���ɼ����ʼʱ��
extern LONG* 	AS_pnLMCMAX; 		 //���ɼ�������ֵ
extern LONG* 	AS_pnLMVMIN; 		 //���ɼ���ѹ��ֵ
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��
extern LONG* 	AS_pnC_THRD; 		 //�����仯��¼��ֵ
extern LONG* 	AS_pnV_THRD; 		 //��ѹ�仯��¼��ֵ

//���----------------------------------
extern LONG* 	AS_pnPT1Ratio; 		 //PT1���
extern LONG* 	AS_pnPT2Ratio; 		 //PT2���
extern LONG* 	AS_pnPT3Ratio; 		 //PT3���
extern LONG* 	AS_pnPT4Ratio; 		 //PT4���
extern LONG* 	AS_pnPCTRatio; 		 //����CT���
extern LONG* 	AS_pnMCTRatio; 		 //����CT���
extern LONG* 	AS_pnZRatio; 		 //�迹���
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     8    //������ֵ�б�ȸ���

#define AS_COUNT     68    //������ֵ����
#define AS_BUF_SIZE     272    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 682)

//ϵ��----------------------------------
extern LONG* 	CF_pnIP; 		 //IP
extern LONG* 	CF_pnUP; 		 //UP
extern LONG* 	CF_pnU3; 		 //U3
extern LONG* 	CF_pnU4; 		 //U4
extern LONG* 	CF_pnITM; 		 //ITM
extern LONG* 	CF_pnIFM; 		 //IFM
extern LONG* 	CF_pnU1M; 		 //U1M
extern LONG* 	CF_pnU2M; 		 //U2M
extern LONG* 	CF_pnU3M; 		 //U3M
extern LONG* 	CF_pnU4M; 		 //U4M
extern LONG* 	CF_pnP; 		 //P
extern LONG* 	CF_pnQ; 		 //Q
extern LONG* 	CF_pnA; 		 //A
extern LONG* 	CF_pnZ; 		 //Z
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnITDRIFT; 		 //ITDRIFT
extern LONG* 	CF_pnIFDRIFT; 		 //IFDRIFT
extern LONG* 	CF_pnU1DRIFT; 		 //U1DRIFT
extern LONG* 	CF_pnU2DRIFT; 		 //U2DRIFT
extern LONG* 	CF_pnU3DRIFT; 		 //U3DRIFT
extern LONG* 	CF_pnU4DRIFT; 		 //U4DRIFT

#define CF_COUNT     24    //ϵ������
#define CF_BUF_SIZE     96    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 954)

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
#define R_LOGICERR_R               21    //�߼����̹��ϻָ�
#define R_POWERERR_R               22    //5V��Դ���ϻָ�
#define R_ACTPICK                  23    //��������
#define R_R211_PI                  24    //����������
#define R_R211                     25    //�����ζ���
#define R_R211_RES                 26    //�����η���
#define R_R212_PI                  27    //����������
#define R_R212                     28    //�����ζ���
#define R_R212_RES                 29    //�����η���
#define R_R213_PI                  30    //����������
#define R_R213                     31    //�����ζ���
#define R_R213_RES                 32    //�����η���
#define R_R214_PI                  33    //�����������
#define R_R214                     34    //������ζ���
#define R_R214_RES                 35    //������η���
#define R_R212_94_PI               36    //�����μ�������
#define R_R212_94                  37    //�����μ��ٶ���
#define R_R212_94_RES              38    //�����μ��ٷ���
#define R_R213_94_PI               39    //�����μ�������
#define R_R213_94                  40    //�����μ��ٶ���
#define R_R213_94_RES              41    //�����μ��ٷ���
#define R_R214_94_PI               42    //������μ�������
#define R_R214_94                  43    //������μ��ٶ���
#define R_R214_94_RES              44    //������μ��ٷ���
#define R_R50I0_PI                 45    //�����ٶ�����
#define R_R50I0                    46    //�����ٶ϶���
#define R_R50I0_RES                47    //�����ٶϷ���
#define R_R50I1_PI                 48    //�����������
#define R_R50I1                    49    //������ζ���
#define R_R50I1_RES                50    //������η���
#define R_R50I2_PI                 51    //�����������
#define R_R50I2                    52    //������ζ���
#define R_R50I2_RES                53    //������η���
#define R_R50I3_PI                 54    //�����������
#define R_R50I3                    55    //������ζ���
#define R_R50I3_RES                56    //������η���
#define R_R50I1_94_PI              57    //������μ�������
#define R_R50I1_94                 58    //������μ��ٶ���
#define R_R50I1_94_RES             59    //������μ��ٷ���
#define R_R50I2_94_PI              60    //������μ�������
#define R_R50I2_94                 61    //������μ��ٶ���
#define R_R50I2_94_RES             62    //������μ��ٷ���
#define R_R50I3_94_PI              63    //������μ�������
#define R_R50I3_94                 64    //������μ��ٶ���
#define R_R50I3_94_RES             65    //������μ��ٷ���
#define R_R50DI_PI                 66    //����������������
#define R_R50DI                    67    //����������������
#define R_R50DI_RES                68    //����������������
#define R_R51I_PI                  69    //��ʱ�޹�������
#define R_R51I                     70    //��ʱ�޹�������
#define R_R51I_RES                 71    //��ʱ�޹�������
#define R_R27TR                    72    //��ѹ������բ
#define R_R27ALARM                 73    //��ѹ�����澯
#define R_TRSTEAL                  74    //͵���غ�բ
#define R_R79_1                    75    //һ���غ�բ����
#define R_R79_2                    76    //�����غ�բ����
#define R_PTLK                     77    //PT���߱������뱣��
#define R_R79LK_1                  78    //һ���غ�բ������
#define R_R79LK_2                  79    //�����غ�բ������
#define R_R52BF                    80    //ʧ�鱣������
#define R_PT1                      81    //PT1����
#define R_PT2                      82    //PT2����
#define R_PT1_RES                  83    //PT1���߷���
#define R_PT2_RES                  84    //PT2���߷���
#define R_R79FAIL                  85    //�غ�բʧ��
#define R_PICKUP                   86    //��������
#define R_TRFAIL                   87    //��·����բʧ��
#define R_COMTEST                  88    //ͨ������
#define R_CT1                      89    //CT1���߸澯
#define R_CT2                      90    //CT2���߸澯
#define R_CT1_RES                  91    //CT1���߷���
#define R_CT2_RES                  92    //CT2���߷���
#define R_R79PI_1                  93    //һ���غ�բ����
#define R_R79PI_2                  94    //�����غ�բ����
#define R_R79_1_SUCC               95    //һ���غ�բ�ɹ�
#define R_R79_1_FAIL               96    //һ���غ�բʧ��
#define R_R79_2_SUCC               97    //�����غ�բ�ɹ�
#define R_R79_2_FAIL               98    //�����غ�բʧ��
#define R_R79NPI_1                 99    //һ���غ�բδ����
#define R_R79NPI_2                100    //�����غ�բδ����
#define R_R211_D_PI               101    //����������
#define R_R211_D                  102    //�����ζ���
#define R_R211_D_RES              103    //�����η���
#define R_R212_D_PI               104    //����������
#define R_R212_D                  105    //�����ζ���
#define R_R212_D_RES              106    //�����η���
#define R_R213_D_PI               107    //����������
#define R_R213_D                  108    //�����ζ���
#define R_R213_D_RES              109    //�����η���
#define R_R214_D_PI               110    //�����������
#define R_R214_D                  111    //������ζ���
#define R_R214_D_RES              112    //������η���
#define R_R212_94_D_PI            113    //�����μ�������
#define R_R212_94_D               114    //�����μ��ٶ���
#define R_R212_94_D_RES           115    //�����μ��ٷ���
#define R_R213_94_D_PI            116    //�����μ�������
#define R_R213_94_D               117    //�����μ��ٶ���
#define R_R213_94_D_RES           118    //�����μ��ٷ���
#define R_R214_94_D_PI            119    //������μ�������
#define R_R214_94_D               120    //������μ��ٶ���
#define R_R214_94_D_RES           121    //������μ��ٷ���
#define R_R50I0_D_PI              122    //�����ٶ�����
#define R_R50I0_D                 123    //�����ٶ϶���
#define R_R50I0_D_RES             124    //�����ٶϷ���
#define R_R50I1_D_PI              125    //�����������
#define R_R50I1_D                 126    //������ζ���
#define R_R50I1_D_RES             127    //������η���
#define R_R50I2_D_PI              128    //�����������
#define R_R50I2_D                 129    //������ζ���
#define R_R50I2_D_RES             130    //������η���
#define R_R50I3_D_PI              131    //�����������
#define R_R50I3_D                 132    //������ζ���
#define R_R50I3_D_RES             133    //������η���
#define R_R50I1_94_D_PI           134    //������μ�������
#define R_R50I1_94_D              135    //������μ��ٶ���
#define R_R50I1_94_D_RES          136    //������μ��ٷ���
#define R_R50I2_94_D_PI           137    //������μ�������
#define R_R50I2_94_D              138    //������μ��ٶ���
#define R_R50I2_94_D_RES          139    //������μ��ٷ���
#define R_R50I3_94_D_PI           140    //������μ�������
#define R_R50I3_94_D              141    //������μ��ٶ���
#define R_R50I3_94_D_RES          142    //������μ��ٷ���
#define R_R50DI_D_PI              143    //����������������
#define R_R50DI_D                 144    //����������������
#define R_R50DI_D_RES             145    //����������������
#define R_R51I_D_PI               146    //��ʱ�޹�������
#define R_R51I_D                  147    //��ʱ�޹�������
#define R_R51I_D_RES              148    //��ʱ�޹�������
#define R_R27U_D                  149    //��ѹ������բ
#define R_R211_S_PI               150    //����������
#define R_R211_S                  151    //�����ζ���
#define R_R211_S_RES              152    //�����η���
#define R_R212_S_PI               153    //����������
#define R_R212_S                  154    //�����ζ���
#define R_R212_S_RES              155    //�����η���
#define R_R213_S_PI               156    //����������
#define R_R213_S                  157    //�����ζ���
#define R_R213_S_RES              158    //�����η���
#define R_R214_S_PI               159    //�����������
#define R_R214_S                  160    //������ζ���
#define R_R214_S_RES              161    //������η���
#define R_R212_94_S_PI            162    //�����μ�������
#define R_R212_94_S               163    //�����μ��ٶ���
#define R_R212_94_S_RES           164    //�����μ��ٷ���
#define R_R213_94_S_PI            165    //�����μ�������
#define R_R213_94_S               166    //�����μ��ٶ���
#define R_R213_94_S_RES           167    //�����μ��ٷ���
#define R_R214_94_S_PI            168    //������μ�������
#define R_R214_94_S               169    //������μ��ٶ���
#define R_R214_94_S_RES           170    //������μ��ٷ���
#define R_R50I0_S_PI              171    //�����ٶ�����
#define R_R50I0_S                 172    //�����ٶ϶���
#define R_R50I0_S_RES             173    //�����ٶϷ���
#define R_R50I1_S_PI              174    //�����������
#define R_R50I1_S                 175    //������ζ���
#define R_R50I1_S_RES             176    //������η���
#define R_R50I2_S_PI              177    //�����������
#define R_R50I2_S                 178    //������ζ���
#define R_R50I2_S_RES             179    //������η���
#define R_R50I3_S_PI              180    //�����������
#define R_R50I3_S                 181    //������ζ���
#define R_R50I3_S_RES             182    //������η���
#define R_R50I1_94_S_PI           183    //������μ�������
#define R_R50I1_94_S              184    //������μ��ٶ���
#define R_R50I1_94_S_RES          185    //������μ��ٷ���
#define R_R50I2_94_S_PI           186    //������μ�������
#define R_R50I2_94_S              187    //������μ��ٶ���
#define R_R50I2_94_S_RES          188    //������μ��ٷ���
#define R_R50I3_94_S_PI           189    //������μ�������
#define R_R50I3_94_S              190    //������μ��ٶ���
#define R_R50I3_94_S_RES          191    //������μ��ٷ���
#define R_R50DI_S_PI              192    //����������������
#define R_R50DI_S                 193    //����������������
#define R_R50DI_S_RES             194    //����������������
#define R_R51I_S_PI               195    //��ʱ�޹�������
#define R_R51I_S                  196    //��ʱ�޹�������
#define R_R51I_S_RES              197    //��ʱ�޹�������
#define R_R27U_S                  198    //��ѹ������բ
#define R_RELDATAERR              199    //���������쳣
#define R_RELDATAERR_R            200    //���������쳣�ָ�
#define MAX_REPORT_INDEX          200    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     13    //��������

#endif