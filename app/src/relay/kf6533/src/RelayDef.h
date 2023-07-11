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
#define 	 FAILDATA_DISPTYPE_CFGNO 	 6 		 //���ϵ�����ʾ�����������
#define 	 LangTYPE_SYSSETNO 	 0 		 //�������͸�����ֵ���

//��ǰ��ֵ���Ŷ���-----------------------
extern WORD* 	G_pwCurAreaNo; 		 //��ǰ��ֵ����

//��ֵȡֵ�궨��

//������ֵȡֵ�궨��
//----�����-----
#define 	RATE_1A 	0	//1A
#define 	RATE_5A 	1	//5A

//��������ֵ����-----------------------
#define  RM_UHA 0
extern LONG* 	RM_pnUHA; 		 //UA UA
#define  RM_UHB 1
extern LONG* 	RM_pnUHB; 		 //UB UB
#define  RM_UHC 2
extern LONG* 	RM_pnUHC; 		 //UC UC
#define  RM_UAB 3
extern LONG* 	RM_pnUAB; 		 //UAB UAB
#define  RM_UBC 4
extern LONG* 	RM_pnUBC; 		 //UBC UBC
#define  RM_UCA 5
extern LONG* 	RM_pnUCA; 		 //UCA UCA
#define  RM_U0H 6
extern LONG* 	RM_pnU0H; 		 //U0H U0H
#define  RM_ULA 7
extern LONG* 	RM_pnULa; 		 //Ua Ua
#define  RM_ULB 8
extern LONG* 	RM_pnULb; 		 //Ub Ub
#define  RM_ULC 9
extern LONG* 	RM_pnULc; 		 //Uc Uc
#define  RM_ULAB 10
extern LONG* 	RM_pnULab; 		 // Uab
#define  RM_ULBC 11
extern LONG* 	RM_pnULbc; 		 // Ubc
#define  RM_ULCA 12
extern LONG* 	RM_pnULca; 		 // Uca
#define  RM_U0L 13
extern LONG* 	RM_pnU0L; 		 //U0L U0L
#define  RM_IHA 14
extern LONG* 	RM_pnIHA; 		 //IA IA
#define  RM_IHB 15
extern LONG* 	RM_pnIHB; 		 //IB IB
#define  RM_IHC 16
extern LONG* 	RM_pnIHC; 		 //IC IC
#define  RM_ILA 17
extern LONG* 	RM_pnILa; 		 //Ia Ia
#define  RM_ILB 18
extern LONG* 	RM_pnILb; 		 //Ib Ib
#define  RM_ILC 19
extern LONG* 	RM_pnILc; 		 //Ic Ic
#define  RM_I0 20
extern LONG* 	RM_pnI0; 		 //I0 I0
#define  RM_IJX 21
extern LONG* 	RM_pnIjx; 		 //Ijx Ijx
#define  RM_U2H 22
extern LONG* 	RM_pnU2H; 		 //U2H U2H
#define  RM_U2L 23
extern LONG* 	RM_pnU2L; 		 //U2L U2L
#define  RM_IDA 24
extern LONG* 	RM_pnIDA; 		 //IDA IDA
#define  RM_IDB 25
extern LONG* 	RM_pnIDB; 		 //IDB IDB
#define  RM_IDC 26
extern LONG* 	RM_pnIDC; 		 //IDC IDC
#define  RM_IRA 27
extern LONG* 	RM_pnIRA; 		 //IRA IRA
#define  RM_IRB 28
extern LONG* 	RM_pnIRB; 		 //IRB IRB
#define  RM_IRC 29
extern LONG* 	RM_pnIRC; 		 //IRC IRC
#define  RM_IDA2 30
extern LONG* 	RM_pnIDA2; 		 //IDA2 IDA2
#define  RM_IDA3 31
extern LONG* 	RM_pnIDA3; 		 //IDA3 IDA3
#define  RM_IDA5 32
extern LONG* 	RM_pnIDA5; 		 //IDA5 IDA5
#define  RM_IDB2 33
extern LONG* 	RM_pnIDB2; 		 //IDB2 IDB2
#define  RM_IDB3 34
extern LONG* 	RM_pnIDB3; 		 //IDB3 IDB3
#define  RM_IDB5 35
extern LONG* 	RM_pnIDB5; 		 //IDB5 IDB5
#define  RM_IDC2 36
extern LONG* 	RM_pnIDC2; 		 //IDC2 IDC2
#define  RM_IDC3 37
extern LONG* 	RM_pnIDC3; 		 //IDC3 IDC3
#define  RM_IDC5 38
extern LONG* 	RM_pnIDC5; 		 //IDC5 IDC5
#define  RM_IRA2 39
extern LONG* 	RM_pnIRA2; 		 //IRA2 IRA2
#define  RM_IRB2 40
extern LONG* 	RM_pnIRB2; 		 //IRB2 IRB2
#define  RM_IRC2 41
extern LONG* 	RM_pnIRC2; 		 //IRC2 IRC2
#define  RM_AUHA 42
extern LONG* 	RM_pnAUHA; 		 //AUA ��UA
#define  RM_AUHB 43
extern LONG* 	RM_pnAUHB; 		 //AUB ��UB
#define  RM_AUHC 44
extern LONG* 	RM_pnAUHC; 		 //AUC ��UC
#define  RM_AULA 45
extern LONG* 	RM_pnAULa; 		 //AUa ��Ua
#define  RM_AULB 46
extern LONG* 	RM_pnAULb; 		 //AUb ��Ub
#define  RM_AULC 47
extern LONG* 	RM_pnAULc; 		 //AUc ��Uc
#define  RM_AIHA 48
extern LONG* 	RM_pnAIHA; 		 //AIA ��IA
#define  RM_AIHB 49
extern LONG* 	RM_pnAIHB; 		 //AIB ��IB
#define  RM_AIHC 50
extern LONG* 	RM_pnAIHC; 		 //AIC ��IC
#define  RM_AILA 51
extern LONG* 	RM_pnAILa; 		 //AIa ��Ia
#define  RM_AILB 52
extern LONG* 	RM_pnAILb; 		 //AIb ��Ib
#define  RM_AILC 53
extern LONG* 	RM_pnAILc; 		 //AIc ��Ic
#define  RM_BOBRCH1 54
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 55
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 56
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 57
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     58    //��������ֵ����
#define RM_BUF_SIZE     232    //��������ֵ��������С
#define RM_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 4)

//����ֵ����-----------------------
#define  M_UHA 0
extern LONG* 	M_pnUHA; 		 //UA UA
#define  M_UHB 1
extern LONG* 	M_pnUHB; 		 //UB UB
#define  M_UHC 2
extern LONG* 	M_pnUHC; 		 //UC UC
#define  M_U0H 3
extern LONG* 	M_pnU0h; 		 //U0h U0h
#define  M_ULA 4
extern LONG* 	M_pnUla; 		 //Ua Ua
#define  M_ULB 5
extern LONG* 	M_pnUlb; 		 //Ub Ub
#define  M_ULC 6
extern LONG* 	M_pnUlc; 		 //Uc Uc
#define  M_U0L 7
extern LONG* 	M_pnU0l; 		 //U0l U0l
#define  M_IHA 8
extern LONG* 	M_pnIHA; 		 //IA IA
#define  M_IHB 9
extern LONG* 	M_pnIHB; 		 //IB IB
#define  M_IHC 10
extern LONG* 	M_pnIHC; 		 //IC IC
#define  M_ILA 11
extern LONG* 	M_pnIla; 		 //Ia Ia
#define  M_ILB 12
extern LONG* 	M_pnIlb; 		 //Ib Ib
#define  M_ILC 13
extern LONG* 	M_pnIlc; 		 //Ic Ic
#define  M_I0 14
extern LONG* 	M_pnI0; 		 //I0 I0
#define  M_IJX 15
extern LONG* 	M_pnIjx; 		 //Ijx Ijx
#define  M_P 16
extern LONG* 	M_pnP; 		 //P P
#define  M_Q 17
extern LONG* 	M_pnQ; 		 //Q Q
#define  M_COS 18
extern LONG* 	M_pnCOS; 		 //COS COS

#define MEASURE_COUNT     19    //����ֵ����
#define MEASURE_BUF_SIZE     76    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 236)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby87U; 		 //��ٶϱ���
extern BYTE* 	SW_pby87R; 		 //���ʲ����
extern BYTE* 	SW_pby50I_H; 		 //��ѹ���������
extern BYTE* 	SW_pby50I_L; 		 //��ѹ���������
extern BYTE* 	SW_pby50I_SH; 		 //ĸ�߳�籣��
extern BYTE* 	SW_pby50N; 		 //�����������
extern BYTE* 	SW_pby59N; 		 //�����ѹ����
extern BYTE* 	SW_pby50OL; 		 //�����ɱ���
extern BYTE* 	SW_pby50I_JX; 		 //��϶��������
extern BYTE* 	SW_pby49I; 		 //��ʱ�޹�����
extern BYTE* 	SW_pby27U; 		 //ʧѹ����
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     14    //��ѹ�����
#define SW_BUF_SIZE     14    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 312)

//����----------------------------------
extern BYTE* 	CFG_pby87U; 		 //�����ٶϱ���
extern BYTE* 	CFG_pby87R; 		 //���ʲ����
extern BYTE* 	CFG_pby68IDH2MAXLK; 		 //����г����������
extern BYTE* 	CFG_pby68IDH2MULLK; 		 //����г���ۺ������
extern BYTE* 	CFG_pby68IDH2SPLLK; 		 //����г���������
extern BYTE* 	CFG_pby68IRH2LK; 		 //�ƶ���������г������
extern BYTE* 	CFG_pby68IDMULLK; 		 //�ۺ�г������
extern BYTE* 	CFG_pbyHCT1CLOCK; 		 //��ѹ��CT1��У��
extern BYTE* 	CFG_pbyHCT11CLOCK; 		 //��ѹ��CT11��У��
extern BYTE* 	CFG_pbyLCT1CLOCK; 		 //��ѹ��CT1��У��
extern BYTE* 	CFG_pbyLCT11CLOCK; 		 //��ѹ��CT11��У��
extern BYTE* 	CFG_pbyHAB; 		 //��ѹ������
extern BYTE* 	CFG_pby50I_H1; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_H1_PD; 		 //��ѹ�������ι��ʷ���
extern BYTE* 	CFG_pby50I_H1_47U; 		 //��ѹ�������θ�ѹ����
extern BYTE* 	CFG_pby50I_H2; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_H2_PD; 		 //��ѹ�������ι��ʷ���
extern BYTE* 	CFG_pby50I_H2_47U; 		 //��ѹ�������θ�ѹ����
extern BYTE* 	CFG_pby50I_H3; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_H3_PD; 		 //��ѹ�������ι��ʷ���
extern BYTE* 	CFG_pby50I_H3_47U; 		 //��ѹ�������θ�ѹ����
extern BYTE* 	CFG_pby50I_L1; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_L1_PD; 		 //��ѹ�������ι��ʷ���
extern BYTE* 	CFG_pby50I_L1_47U; 		 //��ѹ�������θ�ѹ����
extern BYTE* 	CFG_pby50I_L2; 		 //��ѹ��������
extern BYTE* 	CFG_pby50I_L2_PD; 		 //��ѹ�������ι��ʷ���
extern BYTE* 	CFG_pby50I_L2_47U; 		 //��ѹ�������θ�ѹ����
extern BYTE* 	CFG_pby50I_SH; 		 //ĸ�߳�籣��
extern BYTE* 	CFG_pby49I; 		 //��ʱ�޹����ɱ���
extern BYTE* 	CFG_pby50I_N1; 		 //����������
extern BYTE* 	CFG_pby50I_N2; 		 //����������
extern BYTE* 	CFG_pby59U_NH; 		 //��ѹ�������ѹ����
extern BYTE* 	CFG_pby59U_NL; 		 //��ѹ�������ѹ�澯
extern BYTE* 	CFG_pby50OL_1; 		 //�����ɢ��
extern BYTE* 	CFG_pby50OL_2; 		 //�����ɢ��
extern BYTE* 	CFG_pby50OL_3; 		 //�����ɢ��
extern BYTE* 	CFG_pby50OL_A; 		 //�����ɸ澯
extern BYTE* 	CFG_pby50I_JX; 		 //��϶��������
extern BYTE* 	CFG_pby27U; 		 //ʧѹ����
extern BYTE* 	CFG_pby27U_50I; 		 //ʧѹ������������
extern BYTE* 	CFG_pbyPTCK; 		 //PT���߼��
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     44    //���ø���/
#define CFG_BUF_SIZE     44    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 328)

//��ֵ----------------------------------
extern LONG* 	RS_pnFUNCTION1; 		 //������1 FUNCTION1
extern LONG* 	RS_pnFUNCTION2; 		 //������2 FUNCTION2
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn87UC; 		 //��ٶϵ��� 87UC
extern LONG* 	RS_pn87RC; 		 //��ż����� 87RC
extern LONG* 	RS_pnIRS1; 		 //�����ƶ��յ�1 IRS1
extern LONG* 	RS_pnIRS2; 		 //�����ƶ��յ�2 IRS2
extern LONG* 	RS_pnSLP1; 		 //�����ƶ�ϵ��1 SLP1
extern LONG* 	RS_pnSLP2; 		 //�����ƶ�ϵ��2 SLP2
extern LONG* 	RS_pnkrh2; 		 //����г���ƶ�ϵ�� krh2
extern LONG* 	RS_pnkrh; 		 //�ۺ�г���ƶ�ϵ�� krh
extern LONG* 	RS_pnkb; 		 //ƽ��ϵ�� kb
extern LONG* 	RS_pn50IH1C; 		 //��ѹ�������ε��� 50IH1C
extern LONG* 	RS_pn50IH1D; 		 //��ѹ��������ʱ�� 50IH1D
extern LONG* 	RS_pn50IH2C; 		 //��ѹ�������ε��� 50IH2C
extern LONG* 	RS_pn50IH2D; 		 //��ѹ��������ʱ�� 50IH2D
extern LONG* 	RS_pn50IH3C; 		 //��ѹ�������ε��� 50IH3C
extern LONG* 	RS_pn50IH3D; 		 //��ѹ��������ʱ�� 50IH3D
extern LONG* 	RS_pn50IL1C; 		 //��ѹ�������ε��� 50IL1C
extern LONG* 	RS_pn50IL1D; 		 //��ѹ��������ʱ�� 50IL1D
extern LONG* 	RS_pn50IL2C; 		 //��ѹ�������ε��� 50IL2C
extern LONG* 	RS_pn50IL2D; 		 //��ѹ��������ʱ�� 50IL2D
extern LONG* 	RS_pn47U_27U1; 		 //��ѹ�����͵�ѹ 47U_27U1
extern LONG* 	RS_pn47U_59U2; 		 //��ѹ���������ѹ 47U_59U2
extern LONG* 	RS_pnA_50I; 		 //�������������� A_50I
extern LONG* 	RS_pn50ISHC; 		 //ĸ�䱣������ 50ISHC
extern LONG* 	RS_pn50ISHD; 		 //ĸ�䱣��ʱ�� 50ISHD
extern LONG* 	RS_pn50ISHT; 		 //ĸ�䱣��Ͷ��ʱ�� 50ISHT
extern LONG* 	RS_pn50IN1C; 		 //���������ε��� 50IN1C
extern LONG* 	RS_pn50IN1D; 		 //����������ʱ�� 50IN1D
extern LONG* 	RS_pn50IN2C; 		 //���������ε��� 50IN2C
extern LONG* 	RS_pn50IN2D; 		 //����������ʱ�� 50IN2D
extern LONG* 	RS_pn59UNHV; 		 //��ѹ�������ѹ��ѹ 59UNHV
extern LONG* 	RS_pn59UNHD; 		 //��ѹ�������ѹʱ�� 59UNHD
extern LONG* 	RS_pn59UNLV; 		 //��ѹ����ѹ�澯��ѹ 59UNLV
extern LONG* 	RS_pn59UNLD; 		 //��ѹ����ѹ�澯ʱ�� 59UNLD
extern LONG* 	RS_pn50OL1C; 		 //�����ɢ�ε��� 50OL1C
extern LONG* 	RS_pn50OL1D; 		 //�����ɢ��ʱ�� 50OL1D
extern LONG* 	RS_pn50OL2C; 		 //�����ɢ�ε��� 50OL2C
extern LONG* 	RS_pn50OL2D; 		 //�����ɢ��ʱ�� 50OL2D
extern LONG* 	RS_pn50OL3C; 		 //�����ɢ�ε��� 50OL3C
extern LONG* 	RS_pn50OL3D; 		 //�����ɢ��ʱ�� 50OL3D
extern LONG* 	RS_pn50OLAC; 		 //�����ɸ澯���� 50OLAC
extern LONG* 	RS_pn50OLAD; 		 //�����ɸ澯ʱ�� 50OLAD
extern LONG* 	RS_pn50I_JXC; 		 //��϶�������� 50I_JXC
extern LONG* 	RS_pn50I_JXD; 		 //��϶����ʱ�� 50I_JXD
extern LONG* 	RS_pnRAC; 		 //��ѹ�����ζ���� RAC
extern LONG* 	RS_pn49K; 		 //�ȹ����ɱ������� 49K
extern LONG* 	RS_pn49T; 		 //�ȹ�����ʱ�䳣�� 49T
extern LONG* 	RS_pn49AR; 		 //�ȹ����ɸ澯���� 49AR
extern LONG* 	RS_pn49KT; 		 //�豸ֹͣ����ϵ�� 49KT
extern LONG* 	RS_pn27V; 		 //ʧѹ������ѹ 27V
extern LONG* 	RS_pn27D; 		 //ʧѹ����ʱ�� 27D
extern LONG* 	RS_pn27U_50I; 		 //ʧѹ��������������ֵ 27U_50I
extern LONG* 	RS_pn74VTC; 		 //PT���߼����� 74VTC
extern LONG* 	RS_pn74VTV; 		 //PT���߼���ѹ 74VTV
extern LONG* 	RS_pn74VTD; 		 //PT���߼��ʱ�� 74VTD
extern LONG* 	RS_pnK87AR; 		 //����Խ�ޱ������� K87AR
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD

#define RS_COUNT     59    //��ֵ����/
#define RS_BUF_SIZE     236    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 372)

//������ֵ----------------------------------
extern LONG* 	AS_pnRAC; 		 //�����
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
extern LONG* 	AS_pnTSW; 		 //�綯����������ʱ
extern LONG* 	AS_pn50IAC; 		 //IA������ֵ
extern LONG* 	AS_pn37IAC; 		 //IA������ֵ
extern LONG* 	AS_pn50IBC; 		 //IB������ֵ
extern LONG* 	AS_pn37IBC; 		 //IB������ֵ
extern LONG* 	AS_pn50ICC; 		 //IC������ֵ
extern LONG* 	AS_pn37ICC; 		 //IC������ֵ
extern LONG* 	AS_pn50IaC; 		 //Ia������ֵ
extern LONG* 	AS_pn37IaC; 		 //Ia������ֵ
extern LONG* 	AS_pn50IbC; 		 //Ib������ֵ
extern LONG* 	AS_pn37IbC; 		 //Ib������ֵ
extern LONG* 	AS_pn50IcC; 		 //Ic������ֵ
extern LONG* 	AS_pn37IcC; 		 //Ic������ֵ
extern LONG* 	AS_pn50I0C; 		 //I0������ֵ
extern LONG* 	AS_pn37I0C; 		 //I0������ֵ
extern LONG* 	AS_pn50IjxC; 		 //Ijx������ֵ
extern LONG* 	AS_pn37IjxC; 		 //Ijx������ֵ
extern LONG* 	AS_pn59UAV; 		 //UA��ѹ��ֵ
extern LONG* 	AS_pn27UAV; 		 //UA��ѹ��ֵ
extern LONG* 	AS_pn59UBV; 		 //UB��ѹ��ֵ
extern LONG* 	AS_pn27UBV; 		 //UB��ѹ��ֵ
extern LONG* 	AS_pn59UCV; 		 //UC��ѹ��ֵ
extern LONG* 	AS_pn27UCV; 		 //UC��ѹ��ֵ
extern LONG* 	AS_pn59U0hV; 		 //U0h��ѹ��ֵ
extern LONG* 	AS_pn27U0hV; 		 //U0h��ѹ��ֵ
extern LONG* 	AS_pn59UaV; 		 //Ua��ѹ��ֵ
extern LONG* 	AS_pn27UaV; 		 //Ua��ѹ��ֵ
extern LONG* 	AS_pn59UbV; 		 //Ub��ѹ��ֵ
extern LONG* 	AS_pn27UbV; 		 //Ub��ѹ��ֵ
extern LONG* 	AS_pn59UcV; 		 //Uc��ѹ��ֵ
extern LONG* 	AS_pn27UcV; 		 //Uc��ѹ��ֵ
extern LONG* 	AS_pn59U0lV; 		 //U0l��ѹ��ֵ
extern LONG* 	AS_pn27U0lV; 		 //U0l��ѹ��ֵ
extern LONG* 	AS_pnTR1D; 		 //TR1����ʱ��
extern LONG* 	AS_pnTR2D; 		 //TR2����ʱ��
extern LONG* 	AS_pnTR3D; 		 //TR3����ʱ��
extern LONG* 	AS_pnTR4D; 		 //TR4����ʱ��
extern LONG* 	AS_pnRECD; 		 //����¼������ʱ��

//���----------------------------------
extern LONG* 	AS_pnHPTRatio; 		 //HPT���
extern LONG* 	AS_pnH0PTRatio; 		 //H0PT���
extern LONG* 	AS_pnLPTRatio; 		 //LPT���
extern LONG* 	AS_pnL0PTRatio; 		 //L0PT���
extern LONG* 	AS_pnHCTRatio; 		 //HCT���
extern LONG* 	AS_pnLCTRatio; 		 //LCT���
extern LONG* 	AS_pnL0CTRatio; 		 //L0CT���
extern LONG* 	AS_pnJXCTRatio; 		 //JXCT���
extern LONG* 	AS_pnrt1; 		 //rt1

#define AS_RT_NUM     9    //������ֵ�б�ȸ���

#define AS_COUNT     81    //������ֵ����
#define AS_BUF_SIZE     324    //������ֵ��������С
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 608)

//ϵ��----------------------------------
extern LONG* 	CF_pnUp; 		 //Up
extern LONG* 	CF_pnU0h; 		 //U0h
extern LONG* 	CF_pnU0l; 		 //U0l
extern LONG* 	CF_pnIp; 		 //Ip
extern LONG* 	CF_pnI0; 		 //I0
extern LONG* 	CF_pnIjx; 		 //Ijx
extern LONG* 	CF_pnId; 		 //Id
extern LONG* 	CF_pnUAM; 		 //UAM
extern LONG* 	CF_pnUBM; 		 //UBM
extern LONG* 	CF_pnUCM; 		 //UCM
extern LONG* 	CF_pnU0hM; 		 //U0hM
extern LONG* 	CF_pnUaM; 		 //UaM
extern LONG* 	CF_pnUbM; 		 //UbM
extern LONG* 	CF_pnUcM; 		 //UcM
extern LONG* 	CF_pnU0lM; 		 //U0lM
extern LONG* 	CF_pnIAM; 		 //IAM
extern LONG* 	CF_pnIBM; 		 //IBM
extern LONG* 	CF_pnICM; 		 //ICM
extern LONG* 	CF_pnIaM; 		 //IaM
extern LONG* 	CF_pnIbM; 		 //IbM
extern LONG* 	CF_pnIcM; 		 //IcM
extern LONG* 	CF_pnI0M; 		 //I0M
extern LONG* 	CF_pnIjxM; 		 //IjxM
extern LONG* 	CF_pnUAD; 		 //UAD
extern LONG* 	CF_pnUBD; 		 //UBD
extern LONG* 	CF_pnUCD; 		 //UCD
extern LONG* 	CF_pnU0hD; 		 //U0hD
extern LONG* 	CF_pnUaD; 		 //UaD
extern LONG* 	CF_pnUbD; 		 //UbD
extern LONG* 	CF_pnUcD; 		 //UcD
extern LONG* 	CF_pnU0lD; 		 //U0lD
extern LONG* 	CF_pnIAD; 		 //IAD
extern LONG* 	CF_pnIBD; 		 //IBD
extern LONG* 	CF_pnICD; 		 //ICD
extern LONG* 	CF_pnIaD; 		 //IaD
extern LONG* 	CF_pnIbD; 		 //IbD
extern LONG* 	CF_pnIcD; 		 //IcD
extern LONG* 	CF_pnI0D; 		 //I0D
extern LONG* 	CF_pnIjxD; 		 //IjxD
extern LONG* 	CF_pnC1; 		 //C1
extern LONG* 	CF_pnC2; 		 //C2
extern LONG* 	CF_pnC3; 		 //C3
extern LONG* 	CF_pnC4; 		 //C4
extern LONG* 	CF_pnPA; 		 //PA
extern LONG* 	CF_pnQA; 		 //QA
extern LONG* 	CF_pnPB; 		 //PB
extern LONG* 	CF_pnQB; 		 //QB
extern LONG* 	CF_pnPC; 		 //PC
extern LONG* 	CF_pnQC; 		 //QC
extern LONG* 	CF_pnAA; 		 //AA
extern LONG* 	CF_pnBA; 		 //BA
extern LONG* 	CF_pnCA; 		 //CA

#define CF_COUNT     52    //ϵ������
#define CF_BUF_SIZE     208    //ϵ����������С
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 932)

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
#define R_TRPICK                   23    //��������
#define R_RTR_GAS                  24    //����˹
#define R_RTR_YLSF                 25    //ѹ���ͷ�
#define R_RTR_23                   26    //������բ
#define R_R74_GAS                  27    //����˹
#define R_G74_OIL                  28    //��λ�쳣
#define R_G74_23                   29    //�¶ȸ澯
#define R_PICKUP                   30    //��������
#define R_87UA_PI                  31    //��ٶ�����(A��)
#define R_87UB_PI                  32    //��ٶ�����(B��)
#define R_87UC_PI                  33    //��ٶ�����(C��)
#define R_87U                      34    //��ٶϱ�������
#define R_87UA_RES                 35    //��ٶϷ���(A��)
#define R_87UB_RES                 36    //��ٶϷ���(B��)
#define R_87UC_RES                 37    //��ٶϷ���(C��)
#define R_87RA_PI                  38    //���ʲ����(A��)
#define R_87RB_PI                  39    //���ʲ����(B��)
#define R_87RC_PI                  40    //���ʲ����(C��)
#define R_87R                      41    //���ʲ��������
#define R_87RA_RES                 42    //���ʲ����(A��)
#define R_87RB_RES                 43    //���ʲ����(B��)
#define R_87RC_RES                 44    //���ʲ����(C��)
#define R_50I_H1_PI                45    //��ѹ������������
#define R_50I_H1                   46    //��ѹ�������ζ���
#define R_50I_H1_RES               47    //��ѹ�������η���
#define R_50I_H2_PI                48    //��ѹ������������
#define R_50I_H2                   49    //��ѹ�������ζ���
#define R_50I_H2_RES               50    //��ѹ�������η���
#define R_50I_H3_PI                51    //��ѹ������������
#define R_50I_H3                   52    //��ѹ�������ζ���
#define R_50I_H3_RES               53    //��ѹ�������η���
#define R_50I_L1_PI                54    //��ѹ������������
#define R_50I_L1                   55    //��ѹ�������ζ���
#define R_50I_L1_RES               56    //��ѹ�������η���
#define R_50I_L2_PI                57    //��ѹ������������
#define R_50I_L2                   58    //��ѹ�������ζ���
#define R_50I_L2_RES               59    //��ѹ�������η���
#define R_50I_SH_PI                60    //ĸ�߳�籣������
#define R_50I_SH                   61    //ĸ�߳�籣������
#define R_50I_SH_RES               62    //ĸ�߳�籣������
#define R_49IAR                    63    //��ʱ�޹����ɸ澯
#define R_49I                      64    //��ʱ�޹����ɶ���
#define R_49I_RES                  65    //��ʱ�޹����ɷ���
#define R_50I_N1_PI                66    //��������������
#define R_50I_N1                   67    //���������ζ���
#define R_50I_N1_RES               68    //���������η���
#define R_50I_N2_PI                69    //��������������
#define R_50I_N2                   70    //���������ζ���
#define R_50I_N2_RES               71    //���������η���
#define R_59U_H_PI                 72    //��ѹ�������ѹ����
#define R_59U_H                    73    //��ѹ�������ѹ����
#define R_59U_H_RES                74    //��ѹ�������ѹ����
#define R_50OL_1_PI                75    //�����ɢ������
#define R_50OL_1                   76    //�����ɢ�ζ���
#define R_50OL_1_RES               77    //�����ɢ�η���
#define R_50OL_2_PI                78    //�����ɢ������
#define R_50OL_2                   79    //�����ɢ�ζ���
#define R_50OL_2_RES               80    //�����ɢ�η���
#define R_50OL_3_PI                81    //�����ɢ������
#define R_50OL_3                   82    //�����ɢ�ζ���
#define R_50OL_3_RES               83    //�����ɢ�η���
#define R_50I_JX_PI                84    //��϶������������
#define R_50I_JX                   85    //��϶������������
#define R_50I_JX_RES               86    //��϶������������
#define R_27U                      87    //ʧѹ����
#define R_59U_L                    88    //��ѹ�������ѹ�澯
#define R_87AR                     89    //����Խ��
#define R_HPTAR                    90    //��ѹ��PT����
#define R_LPTAR                    91    //��ѹ��PT����
#define R_52BF                     92    //��·����բʧ��
#define R_COMTEST                  93    //ͨ������
#define R_HPTAR_RES                94    //��ѹ��PT���߻ָ�
#define R_LPTAR_RES                95    //��ѹ��PT���߻ָ�
#define R_87AR_RES                 96    //����Խ�޻ָ�
#define R_RELDATAERR               97    //���������쳣
#define R_RELDATAERR_R             98    //���������쳣�ָ�
#define R_50OL_A                   99    //�����ɸ澯
#define R_50OL_A_PI               100    //�����ɸ澯����
#define R_50OL_A_RES              101    //�����ɸ澯����
#define MAX_REPORT_INDEX          101    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     19    //��������

#endif