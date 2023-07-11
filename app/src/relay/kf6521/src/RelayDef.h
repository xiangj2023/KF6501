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
//----ͬ�����-----
#define 	SYN_PHASE_UA 	0	//UA
#define 	SYN_PHASE_UB 	1	//UB
#define 	SYN_PHASE_UC 	2	//UC
#define 	SYN_PHASE_UAB 	3	//UAB
#define 	SYN_PHASE_UBC 	4	//UBC
#define 	SYN_PHASE_UCA 	5	//UCA

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
#define  RM_F2 39
extern LONG* 	RM_pnF2; 		 // F2
#define  RM_DIFFU 40
extern LONG* 	RM_pnDiffU; 		 // ��U
#define  RM_DIFFA 41
extern LONG* 	RM_pnDiffA; 		 // ����
#define  RM_DIFFDQ 42
extern LONG* 	RM_pnDiffdq; 		 // ��dq
#define  RM_IAM 43
extern LONG* 	RM_pnIam; 		 // Iam
#define  RM_IBM 44
extern LONG* 	RM_pnIbm; 		 // Ibm
#define  RM_ICM 45
extern LONG* 	RM_pnIcm; 		 // Icm
#define  RM_BOBRCH1 46
extern LONG* 	RM_pnBOBRCH1; 		 // BR1
#define  RM_BOBRCH2 47
extern LONG* 	RM_pnBOBRCH2; 		 // BR2
#define  RM_BOINCH1 48
extern LONG* 	RM_pnBOINCH1; 		 // IN1
#define  RM_BOINCH2 49
extern LONG* 	RM_pnBOINCH2; 		 // IN2

#define RM_COUNT     50    //��������ֵ����
#define RM_BUF_SIZE     200    //��������ֵ��������С
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
#define  M_F2 19
extern LONG* 	M_pnF2; 		 //F2 F2
#define  M_VD 20
extern LONG* 	M_pnVD; 		 //VD VD
#define  M_AD 21
extern LONG* 	M_pnAD; 		 //AD AD

#define MEASURE_COUNT     22    //����ֵ����
#define MEASURE_BUF_SIZE     88    //����ֵ��������С
#define MEASURE_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 204)

//��ѹ��----------------------------------
extern BYTE* 	SW_pby50I0; 		 //�����ٶ�
extern BYTE* 	SW_pby50I1; 		 //�������
extern BYTE* 	SW_pby50I2; 		 //�������
extern BYTE* 	SW_pby50I3; 		 //�������
extern BYTE* 	SW_pby50N1; 		 //����������
extern BYTE* 	SW_pby50N2; 		 //����������
extern BYTE* 	SW_pby50ISH; 		 //ĸ�߳�籣��
extern BYTE* 	SW_pby27; 		 //��ѹ����
extern BYTE* 	SW_pby79; 		 //�غ�բ
extern BYTE* 	SW_pbyJDXX; 		 //�ӵ�ѡ��
extern BYTE* 	SW_pby25; 		 //��ͬ��
extern BYTE* 	SW_pby83; 		 //����Ͷ
extern BYTE* 	SW_pby50IOL; 		 //�����ɸ澯
extern BYTE* 	SW_pbyMAINT; 		 //����ѹ��
extern BYTE* 	SW_pbyBAK1; 		 //����ѹ��1
extern BYTE* 	SW_pbyBAK2; 		 //����ѹ��2

#define SW_COUNT     16    //��ѹ�����
#define SW_BUF_SIZE     16    //��ѹ�建������С
#define SW_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 292)

//����----------------------------------
extern BYTE* 	CFG_pby50I0; 		 //�����ٶ�
extern BYTE* 	CFG_pby50I0_47; 		 //�����ٶϸ�ѹ����
extern BYTE* 	CFG_pby50I0_67; 		 //�����ٶϹ��ʷ���
extern BYTE* 	CFG_pby50I0_PD; 		 //�����ٶϷ�������
extern BYTE* 	CFG_pby50I1; 		 //����I��
extern BYTE* 	CFG_pby50I1_47; 		 //I�θ�ѹ����
extern BYTE* 	CFG_pby50I1_67; 		 //I�ι��ʷ���
extern BYTE* 	CFG_pby50I1_PD; 		 //I�η�������
extern BYTE* 	CFG_pby50I2; 		 //����II��
extern BYTE* 	CFG_pby50I2_47; 		 //II�θ�ѹ����
extern BYTE* 	CFG_pby50I2_67; 		 //II�ι��ʷ���
extern BYTE* 	CFG_pby50I2_PD; 		 //II�η�������
extern BYTE* 	CFG_pby50I3; 		 //����III��
extern BYTE* 	CFG_pby50I3_47; 		 //III�θ�ѹ����
extern BYTE* 	CFG_pby50I3_67; 		 //III�ι��ʷ���
extern BYTE* 	CFG_pby50I3_PD; 		 //III�η�������
extern BYTE* 	CFG_pby50N1; 		 //�������I��
extern BYTE* 	CFG_pby50N2; 		 //�������II��
extern BYTE* 	CFG_pby50I_SH; 		 //ĸ�߳�籣��
extern BYTE* 	CFG_pby27U; 		 //��ѹ����
extern BYTE* 	CFG_pby79_50I0; 		 //�����ٶ��غ�
extern BYTE* 	CFG_pby79_50I1; 		 //����I���غ�
extern BYTE* 	CFG_pby79_50I2; 		 //����II���غ�
extern BYTE* 	CFG_pby79_50I3; 		 //����III���غ�
extern BYTE* 	CFG_pby79UC; 		 //͵���غ�բ
extern BYTE* 	CFG_pby79_25; 		 //�غ�բ��ͬ��
extern BYTE* 	CFG_pby79_27; 		 //�غ�բ����ѹ
extern BYTE* 	CFG_pby79_59; 		 //�غ�բ����ѹ
extern BYTE* 	CFG_pby50I1_94; 		 //����I�μ���
extern BYTE* 	CFG_pby50I2_94; 		 //����II�μ���
extern BYTE* 	CFG_pby50I3_94; 		 //����III�μ���
extern BYTE* 	CFG_pby59N_74; 		 //��ѹ�澯
extern BYTE* 	CFG_pbyJDXX; 		 //�ӵ�ѡ��
extern BYTE* 	CFG_pbyXHJD; 		 //������Ȧ�ӵ�
extern BYTE* 	CFG_pbyPTBR; 		 //PT���߼��
extern BYTE* 	CFG_pby25; 		 //�ֺϼ�ͬ��
extern BYTE* 	CFG_pbySH_27; 		 //�ֺϼ���ѹ
extern BYTE* 	CFG_pby83; 		 //����Ͷ
extern BYTE* 	CFG_pby50IOL; 		 //�����ɸ澯
extern BYTE* 	CFG_pbyCTBR; 		 //CT���߼��
extern BYTE* 	CFG_pby52BF; 		 //ʧ�鱣��
extern BYTE* 	CFG_pbyBAK1; 		 //��������1
extern BYTE* 	CFG_pbyBAK2; 		 //��������2

#define CFG_COUNT     43    //���ø���/
#define CFG_BUF_SIZE     43    //���û�������С
#define CFG_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 308)

//��ֵ----------------------------------
extern LONG* 	RS_pnCFG1; 		 //������ CFG1
extern LONG* 	RS_pnCFG2; 		 //������2 CFG2
extern LONG* 	RS_pnPIC; 		 //ͻ������������ PIC
extern LONG* 	RS_pn50P0C; 		 //�����ٶϵ��� 50P0C
extern LONG* 	RS_pn50P0D; 		 //�����ٶ�ʱ�� 50P0D
extern LONG* 	RS_pn50P0_27; 		 //�����ٶϵ�ѹ��ֵ 50P0_27
extern LONG* 	RS_pn50P0_47; 		 //�����ٶϸ����ѹ 50P0_47
extern LONG* 	RS_pn50P1C; 		 //������ε��� 50P1C
extern LONG* 	RS_pn50P1D; 		 //�������ʱ�� 50P1D
extern LONG* 	RS_pn50P1_27; 		 //������ε�ѹ��ֵ 50P1_27
extern LONG* 	RS_pn50P1_47; 		 //������θ����ѹ 50P1_47
extern LONG* 	RS_pn50P2C; 		 //������ε��� 50P2C
extern LONG* 	RS_pn50P2D; 		 //�������ʱ�� 50P2D
extern LONG* 	RS_pn50P2_27; 		 //������ε�ѹ��ֵ 50P2_27
extern LONG* 	RS_pn50P2_47; 		 //������θ����ѹ 50P2_47
extern LONG* 	RS_pn50P3C; 		 //������ε��� 50P3C
extern LONG* 	RS_pn50P3D; 		 //�������ʱ�� 50P3D
extern LONG* 	RS_pn50P3_27; 		 //������ε�ѹ��ֵ 50P3_27
extern LONG* 	RS_pn50P3_47; 		 //������θ����ѹ 50P3_47
extern LONG* 	RS_pn67ANG; 		 //���ʷ��������� 67ANG
extern LONG* 	RS_pn50N1C; 		 //���������ε��� 50N1C
extern LONG* 	RS_pn50N1D; 		 //����������ʱ�� 50N1D
extern LONG* 	RS_pn50N2C; 		 //���������ε��� 50N2C
extern LONG* 	RS_pn50N2D; 		 //����������ʱ�� 50N2D
extern LONG* 	RS_pn50ISHC; 		 //ĸ�߳�籣������ 50ISHC
extern LONG* 	RS_pn50ISHD; 		 //ĸ�߳�籣��ʱ�� 50ISHD
extern LONG* 	RS_pn50ISHT; 		 //ĸ�䱣��Ͷ��ʱ�� 50ISHT
extern LONG* 	RS_pn79SH; 		 //�غ�բͶ����� 79SH
extern LONG* 	RS_pn79D1; 		 //��һ���غ�բʱ�� 79D1
extern LONG* 	RS_pn79D2; 		 //�ڶ����غ�բʱ�� 79D2
extern LONG* 	RS_pn79CLSD; 		 //�غ�բ����������ʱ 79CLSD
extern LONG* 	RS_pn79CT; 		 //�غ�բ���ʱ�� 79CT
extern LONG* 	RS_pn25_27; 		 //�غ�բ����ѹ��ֵ 25_27
extern LONG* 	RS_pn25PA; 		 //ͬ�ڹ��� 25PA
extern LONG* 	RS_pn25PH; 		 //ͬ����� 25PH
extern LONG* 	RS_pn25DT; 		 //ͬ�ڼ��ʱ�� 25DT
extern LONG* 	RS_pn25VUL; 		 //��ͬ�ڵ�ѹ���� 25VUL
extern LONG* 	RS_pn25VLL; 		 //��ͬ�ڵ�ѹ���� 25VLL
extern LONG* 	RS_pn25VD; 		 //ͬ������ѹ�� 25VD
extern LONG* 	RS_pn25FD; 		 //ͬ������Ƶ�� 25FD
extern LONG* 	RS_pn52CLT; 		 //��·����բʱ�� 52CLT
extern LONG* 	RS_pn94D; 		 //����ٶ���ʱ�� 94D
extern LONG* 	RS_pnSOTFD; 		 //�����Ͷ��ʱ�� SOTFD
extern LONG* 	RS_pn27U; 		 //��ѹ������ѹ 27U
extern LONG* 	RS_pn27D; 		 //��ѹ����ʱ�� 27D
extern LONG* 	RS_pn27_LK37; 		 //��ѹ����������ֵ 27_LK37
extern LONG* 	RS_pn59NU; 		 //��ѹ�澯��ѹ 59NU
extern LONG* 	RS_pn59ND; 		 //��ѹ�澯ʱ�� 59ND
extern LONG* 	RS_pn27BZT; 		 //����Ͷ��ѹ��ֵ 27BZT
extern LONG* 	RS_pn59BZT; 		 //����Ͷ��ѹ��ֵ 59BZT
extern LONG* 	RS_pn50BZT; 		 //����Ͷ������ֵ 50BZT
extern LONG* 	RS_pnCHARD; 		 //����Ͷ���ʱ�� CHARD
extern LONG* 	RS_pn50OLC; 		 //�����ɵ��� 50OLC
extern LONG* 	RS_pn50OLD; 		 //������ʱ�� 50OLD
extern LONG* 	RS_pnPT1_V; 		 //ĸ��PT���߼���ѹ PT1_V
extern LONG* 	RS_pnPT1_C; 		 //ĸ��PT���߼����� PT1_C
extern LONG* 	RS_pnPT1_D; 		 //ĸ��PT���߼��ʱ�� PT1_D
extern LONG* 	RS_pnPT2_V1; 		 //��·PT����ĸ����ѹ PT2_V1
extern LONG* 	RS_pnPT2_C; 		 //��·PT����ĸ������ PT2_C
extern LONG* 	RS_pnPT2_V2; 		 //��·PT������·��ѹ PT2_V2
extern LONG* 	RS_pnPT2_D; 		 //��·PT���߼��ʱ�� PT2_D
extern LONG* 	RS_pn52BFD; 		 //ʧ�鱣��ʱ�� 52BFD

#define RS_COUNT     62    //��ֵ����/
#define RS_BUF_SIZE     248    //��ֵ��������С
#define RS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 354)

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
#define AS_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 602)

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
#define CF_BUF_ADDR 	 (RELAY_RAM_ADDR_BASE + 878)

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
#define R_COMTEST                  21    //ͨ������
#define R_PICKUP                   22    //��������
#define R_50I1_PI                  23    //�����������
#define R_50I1                     24    //������ζ���
#define R_50I1_RES                 25    //������η���
#define R_50I2_PI                  26    //�����������
#define R_50I2                     27    //������ζ���
#define R_50I2_RES                 28    //������η���
#define R_50I3_PI                  29    //�����������
#define R_50I3                     30    //������ζ���
#define R_50I3_RES                 31    //������η���
#define R_50I2_94                  32    //������μ��ٶ���
#define R_50I3_94                  33    //������μ��ٶ���
#define R_50N1_PI                  34    //��������������
#define R_50N1                     35    //���������ζ���
#define R_50N1_RES                 36    //���������η���
#define R_50N2_PI                  37    //��������������
#define R_50N2                     38    //���������ζ���
#define R_50N2_RES                 39    //���������η���
#define R_27TR                     40    //��ѹ������բ
#define R_TRFAIL                   41    //��·����բʧ��
#define R_R79_1                    42    //һ���غ�բ����
#define R_R79_2                    43    //�����غ�բ����
#define R_R79LK_1                  44    //һ���غ�բ������
#define R_PI_83                    45    //����Ͷ����
#define R_OFF_83                   46    //����Ͷ�˳�
#define R_SU_83                    47    //����Ͷ�ɹ�
#define R_FA_83                    48    //����Ͷʧ��
#define R_TR_1DL                   49    //��1��·��
#define R_CL_1DL                   50    //��1��·��
#define R_FA_TR1DL                 51    //��1��·��ʧ��
#define R_FA_CL1DL                 52    //��1��·��ʧ��
#define R_TR_2DL                   53    //��2��·��
#define R_CL_2DL                   54    //��2��·��
#define R_FA_TR2DL                 55    //��2��·��ʧ��
#define R_FA_CL2DL                 56    //��2��·��ʧ��
#define R_TR_3DL                   57    //��3��·��
#define R_CL_3DL                   58    //��3��·��
#define R_FA_TR3DL                 59    //��3��·��ʧ��
#define R_FA_CL3DL                 60    //��3��·��ʧ��
#define R_TR_4DL                   61    //��4��·��
#define R_CL_4DL                   62    //��4��·��
#define R_FA_TR4DL                 63    //��4��·��ʧ��
#define R_FA_CL4DL                 64    //��4��·��ʧ��
#define R_TR_1GK                   65    //��1����
#define R_CL_1GK                   66    //��1����
#define R_FA_TR1GK                 67    //��1����ʧ��
#define R_FA_CL1GK                 68    //��1����ʧ��
#define R_TR_2GK                   69    //��2����
#define R_CL_2GK                   70    //��2����
#define R_FA_TR2GK                 71    //��2����ʧ��
#define R_FA_CL2GK                 72    //��2����ʧ��
#define R_TR_3GK                   73    //��3����
#define R_CL_3GK                   74    //��3����
#define R_FA_TR3GK                 75    //��3����ʧ��
#define R_FA_CL3GK                 76    //��3����ʧ��
#define R_TR_4GK                   77    //��4����
#define R_CL_4GK                   78    //��4����
#define R_FA_TR4GK                 79    //��4����ʧ��
#define R_FA_CL4GK                 80    //��4����ʧ��
#define R_PI_1SY                   81    //1����ʧѹ������Ͷ
#define R_PI_2SY                   82    //2����ʧѹ������Ͷ
#define R_LINEDX_RES               83    //��·PT���߻ָ�
#define R_BUSDX_RES                84    //ĸ��PT���߻ָ�
#define R_LINEDX                   85    //��·PT����
#define R_BUSDX                    86    //ĸ��PT����
#define R_64LINE                   87    //��·�ӵ�
#define R_64BUS                    88    //ĸ�߽ӵ�
#define R_59NAR                    89    //��ѹ�澯
#define R_R25                      90    //ͬ�ڲ���
#define R_FA_25                    91    //ͬ��ʧ��
#define R_SU_25                    92    //ͬ�ڳɹ�
#define R_WL_CL                    93    //��ѹ�պ�
#define R_25LK                     94    //ͬ�ڱ�����
#define R_25BK                     95    //ͬ�ڱ��ж�
#define R_FA_79                    96    //�غ�բʧ��
#define R_STEAL_79                 97    //͵���غ�բ
#define R_52BF                     98    //ʧ�鱣��
#define R_CTBR_A                   99    //A��CT����
#define R_CTBR_A_RES              100    //A��CT���߻ָ�
#define R_CTBR_B                  101    //B��CT����
#define R_CTBR_B_RES              102    //B��CT���߻ָ�
#define R_CTBR_C                  103    //C��CT����
#define R_CTBR_C_RES              104    //C��CT���߻ָ�
#define R_POWERERR                105    //5V��Դ����
#define R_EEPROM_W_ERR            106    //EEPROMд�����
#define R_POWERERR_R              107    //5V��Դ���ϻָ�
#define R_R79LK_2                 108    //�����غ�բ������
#define R_R79SUCC_1               109    //һ���غ�բ�ɹ�
#define R_R79SUCC_2               110    //�����غ�բ�ɹ�
#define R_R79FAIL_1               111    //һ���غ�բʧ��
#define R_R79FAIL_2               112    //�����غ�բʧ��
#define R_R79PI_1                 113    //һ���غ�բ����
#define R_R79PI_2                 114    //�����غ�բ����
#define R_R79NPI_1                115    //һ���غ�բδ����
#define R_R79NPI_2                116    //�����غ�բδ����
#define R_50I2_94_PI              117    //������μ�������
#define R_50I2_94_RES             118    //������μ��ٷ���
#define R_50I3_94_PI              119    //������μ�������
#define R_50I3_94_RES             120    //������μ��ٷ���
#define R_RELDATAERR              121    //���������쳣
#define R_RELDATAERR_R            122    //���������쳣�ָ�
#define R_50I0_PI                 123    //�����ٶ�����
#define R_50I0                    124    //�����ٶ϶���
#define R_50I0_RES                125    //�����ٶϷ���
#define R_50IOL                   126    //�����ɸ澯
#define R_50I1_94_PI              127    //������μ�������
#define R_50I1_94                 128    //������μ��ٶ���
#define R_50I1_94_RES             129    //������μ��ٷ���
#define R_50ISH_PI                130    //ĸ�߳�籣������
#define R_50ISH                   131    //ĸ�߳�籣������
#define R_50ISH_RES               132    //ĸ�߳�籣������
#define MAX_REPORT_INDEX          132    //��󱨸�������,���ڸ�ֵΪ�Ƿ�����


//����----------------------------------
#define RTC_COUNT     11    //��������

#endif