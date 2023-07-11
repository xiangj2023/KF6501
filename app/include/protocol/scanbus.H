#ifndef _SCANBUS_H_
#define _SCANBUS_H_

#include "system.h" 
#include "msgmap.h"
//�궨����

/*------------------��Լ�궨��------------------------*/
#define		PRO_CANBUS			1		//��վCANbus��Լ�Ĺ�Լ��

#define		MAX_FRAME_LEN		1200	//���֡��
#define		MIN_FRAME_LEN		14		//��С֡��
#define		MAX_RETRY_NUM		5		//����ط�����

#define		CAN_MAXFRAMELEN		255		//CANBUS���֡��
#define		CAN_TIMER_TICK		100		//CANBUS������ʱ��
#define		CAN_COMMIDLE		100		//CANBUS����ͨ�ſ���ʱ��
#define		CAN_RETRYIDLE		200		//CANBUS�ط�����ʱ��
#define		CAN_RUNIDLE			2000	//CANBUS������ʱ�Ŀ���ʱ�䣬����ͨ������֡

#define		SER_MAXFRAMELEN		511		//�������֡��
#define		SER_TIMER_TICK		100		//���ڻ�����ʱ��
#define		SER_COMMIDLE		500		//��������ͨ�ſ���ʱ��
#define		SER_RETRYIDLE		1000	//�����ط�����ʱ��
#define		SER_RUNIDLE			2000	//����������ʱ�Ŀ���ʱ�䣬����ͨ������֡

#define		ETH_MAXFRAMELEN		1200	//��̫�����֡��
#define		ETH_TIMER_TICK		100		//��̫��������ʱ��
#define		ETH_COMMIDLE		100		//��̫������ͨ�ſ���ʱ��
#define		ETH_RETRYIDLE		200		//��̫���ط�����ʱ��
#define		ETH_RUNIDLE			2000	//��̫��������ʱ�Ŀ���ʱ�䣬����ͨ������֡

#define     CAN_COMMERR			(20000/CAN_TIMER_TICK) 			//20����������ͨ��ʧ��
#define		MAX_MEMBLOCK_SIZE	(200*1024)						//����ڴ�鳤��
#define		MAX_FILE_WAITTIME	(30*60*1000/CAN_TIMER_TICK)		//����ļ����䳬ʱʱ��


#define 	PERFRM_NUM_SET		40
#define 	PERFRM_NUM_SW		64
#define 	PERFRM_NUM_CF		40

#define 	PERFRM_NUM_BIATTR	40
#define 	PERFRM_NUM_BOATTR	40
#define 	PERFRM_NUM_AOATTR	40
#define 	PERFRM_NUM_MBO		6
#define 	PERFRM_NUM_DISTDIR	6

#if(SYSTEM_TYPE == _SYS_KF1300_)
#define		MAX_PROFILE_LEN		(5*1024*1024)	//����Լ�����ļ�����
#else
#define		MAX_PROFILE_LEN		(3*1024*1024)	//����Լ�����ļ�����
#endif

/*---------------------ң����Ϣ��ʾ-----------------------*/
#define		PBO_SELECT			1		//ң��ѡ��
#define	 	PBO_EXECUTE			2		//ң��ִ��
#define	  	PBO_CANCEL 			3		//ң�س���
#define		PBO_DIRECT			4		//ң��ֱ��ִ��
#define		PBO_SUCESS  		1		//ң�سɹ�
#define		PBO_FAIL			0		//ң��ʧ��
			
#define     PBO_NULL			0		//����
#define 	PBO_OPEN			1		//�ط�
#define 	PBO_CLOSE			2		//�غ�

#define		PBOINFOSIZE			9		//ң�ر�����Ϣ�ֽ���
//��λ���
#define		 RESET_WARM			0
#define      RESET_COLD			1
#define      RESET_DATA			2
#define      RECOVER_DATA			3
#define      RECOVER_COEF			4
#define      RECOVER_PARA			5

//��������
/*
#define		AI_VALID			((DWORD)1<<0)	//����ң�������ϴ�
#define		BI_VALID			((DWORD)1<<1)	//����ң�������ϴ�
#define		CI_VALID			((DWORD)1<<2)	//������������ϴ�
#define		BURSTAI_VALID		((DWORD)1<<8)	//�����仯ң�������ϴ�
#define		SOE_VALID			((DWORD)1<<9)	//����SOE�����ϴ�	
#define		BURSTBI_VALID		((DWORD)1<<10)	//�����仯ң�������ϴ�
#define		RELAYALARM_VALID	((DWORD)1<<11)	//���������澯���������ϴ�
#define		RELAYSTART_VALID	((DWORD)1<<12)	//���������������������ϴ�
#define		RELAYACTION_VALID	((DWORD)1<<13)	//���������������������ϴ�
#define		BO_VALID			((DWORD)1<<21)	//����ң��
#define		SAFETY_VALID		((DWORD)1<<7)	//����ƽ�������ϱ�
#define		FILE_VALID			((DWORD)1<<24)	//�����ļ�����	
*/

/*ȱʡ�ϱ����*/
#define     CAN_AIREPTM			3
#define     CAN_BIREPTM			5
#define     CAN_CIREPTM			30
#define     CAN_SAFETYREPTM		5
#define     CAN_AIDEADZONE		1
#define     CAN_BIEREPTM		100
#define     CAN_BIBREPTM		100
#define     CAN_RELAYREPTM		100
#define     CAN_BURSTAIREPTM	500
#define     CAN_LEDREPTM		60

/*-----------------�����붨����-----------------------*/
#define		FUN_CON				00	//ȷ��
#define		FUN_IDLE			01	//����
#define		FUN_READVALUE		02	//��ָ�����ݶ����ֵ
#define		FUN_READSPECIALVALUE	03	//���ض����ݶ����ֵ
#define		FUN_READATTRIB		12	//��ָ�����ݶ��������
#define		FUN_WRITEVALUE		13	//дָ�����ݶ���
#define		FUN_WRITEATTRIB		14	//дָ�����ݶ��������
#define		FUN_SUBSTITUTE		17	//���ָ�����ݶ����ֵ
#define		FUN_UNSUBSTITUTE	18	//ȡ�����ָ�����ݶ����ֵ

#define		FUN_SELECT			20	//ѡ��ָ�������ݶ���
#define		FUN_SELECTWITHVALUE	21	//��ֵ�Ķ���ѡ��
#define		FUN_OPERATE			22	//����ָ�������ݶ���
#define		FUN_CANCEL			23	//ȡ��ָ�����ݶ����ѡ�����
#define		FUN_DIROPERATE		24	//ֱ�Ӳ���ָ�������ݶ���
#define		FUN_READFILE		31	//���ļ�����
#define		FUN_WRITEFILE		32	//д�ļ�����
#define		FUN_DELETEFILE		33	//ɾ���ļ�����
#define		FUN_REPORTVALUE		40	//���ݶ���ֵ����
#define		FUN_QUERYLOGBYREAD	41	//���ڶ�ָ��������ѯ
#define		FUN_QUERYLOGBYWRITE	42	//����дָ��������ѯ
#define		FUN_QUERYLOGBYOFF	43	//����ƫ�����������ѯ
#define		FUN_COMMONCONTROL	50	//ͨ�ÿ�������

#define		FUN_LOGICDEBUG		60	//�߼����̵�����

/*-----------------���ݶ��󼯶�����-----------------------*/
#define		DOC_NONE			0	//��Ч��Ϣ
#define		DOC_AI				01	//ң��
#define		DOC_AIWITHQUALITY	02	//��Ʒ��������ң��
#define		DOC_AIBURST			03	//�仯ң��
#define		DOC_AIEVENT			04	//ң������˳���¼

#define		DOC_BI				8	//����ң��
#define		DOC_DBI				9	//˫��ң��
#define		DOC_BIGROUP			10	//����ң��
#define		DOC_BIWITHQUALITY	11	//��Ʒ�������ĵ���ң��
#define		DOC_DBI				12	//˫��ң��
#define		DOC_DBIWITHQUALITY	13	//��Ʒ��������˫��ң��
#define		DOC_BIBURST			14	//����仯ң��
#define		DOC_DBIBURST		15	//˫��仯ң��
#define		DOC_BIEVENT			16	//���������˳���¼
#define		DOC_DBIEVENTT		17	//˫������˳���¼
#define		DOC_RW				18	//�̵�����״̬
#define		DOC_CI				20	//��ȼ���ֵ
#define		DOC_CIWITHQUALITY	21	//��Ʒ�������ĵ�ȼ���ֵ
#define		DOC_SRM				22	//���βౣ������ֵ
#define		DOC_SRMITHQUALITY	23	//��Ʒ�������Ķ��βౣ������ֵ
#define		DOC_FRM				24	//һ�βౣ������ֵ
#define		DOC_FRMITHQUALITY	25	//��Ʒ��������һ�βౣ������ֵ
#define		DOC_DCSIDATA		26	//ֱ������ͳ������

#define		DOC_BO				30	//����ң������
#define		DOC_DBO				31	//˫��ң������
#define		DOC_AO				32	//ң��
#define		DOC_SYNC			34	//ͬ��
#define		DOC_RELAYCTRL		35	//����
#define		DOC_SPECIALCMD		36	//ģ�ⱨ��\����¼��\��ֵ����
#define		DOC_LEDSTATUS		37	//LED��״̬
#define		DOC_RELAYCTRLWITHBLOCK		38	//��������������


#define		DOC_SETTING			40	//��ֵ
#define		DOC_SETTINGAREA		41	//��ǰ��ֵ����
#define		DOC_SW				42	//��ѹ��
#define		DOC_SYSTEMSETTING	43	//������ֵ
#define		DOC_CF				44	//ϵ��
#define		DOC_RELAYALARMREPORT		46	//�����澯����
#define		DOC_RELAYSTARTREPORT		47	//������������
#define		DOC_RELAYACTIONREPORT		48	//������������

#define		DOC_OPERATEEVENT	51	//�����¼�
#define		DOC_LOGERROR		52	//�쳣��Ϣ
#define		DOC_VERSION			53	//ϵͳ�汾��Ϣ
#define		DOC_NETCFG			54	//��������
#define		DOC_SAFETY			55	//ƽ������
#define		DOC_DEVSTATUS		56	//�豸״̬
#define		DOC_WATCHPORT		57	//�˿ڱ��ļ���
#define		DOC_DEVDISP			58	//�豸������Ϣ

#define		DOC_SYSTIME			60	//���ʱ��
#define		DOC_ABSTIME			61	//����ʱ��
#define		DOC_FILEDATA		62	//�ļ�����
#define		DOC_FILEDISP		63	//�ļ�����
#define		DOC_VARIAL			64	//�������˿ڻ������ݣ�
#define     DOC_COE				65  //����ϵ��
#define		DOC_SYSRESET		66	//ϵͳ��λ
#define		DOC_TRANSDATA		67	//͸������
#define		DOC_DISTDIR			68	//¼��Ŀ¼

#define		DOC_CHANNELSRC		70	//ͨ��Դ
#define		DOC_CHANNEL			71	//ͨ��
#define		DOC_ZERODRIFT		72	//ͨ����Ư
#define		DOC_ZEROOFFSET		73	//ͨ�����ƫ��
#define		DOC_FULLVALUE		74	//ͨ����ֵ
#define		DOC_CFG				75	//������

#define		DOC_RELAYALARMITEM		76	//�����澯������Ŀ
#define		DOC_RELAYSTARTITEM		77	//��������������Ŀ
#define		DOC_RELAYACTIONITEM		78	//��������������Ŀ

#define		DOC_SYSCONFIG		80	//ϵͳ���ò���
#define		DOC_MAPBITOBJ		81	//����λ����
#define		DOC_BREAKERTRIPCNT	82	//��·����բ����
#define		DOC_FLPARA			83	//������

#define		DOC_CLASS1			84	// һ������
#define		DOC_CLASS2			85	// ��������

/*-----------------�߼����̵������ݶ�����---------------*/
#define		DEBUG_RELAY_STEP1   90  //���Խ׶�1�� дֵ��
#define		DEBUG_RELAY_STEP2	91	//���Խ׶�2,  дֵ��
#define		DEBUG_RELAY_CONTEXT	92	//�̵��������߼����������ģ� ��һ���������к�
//#define		DEBUG_RELAY_LINE	93	//���Լ̵����ã� ��ǰ�кż��ϵ㣨�Ѿ������˵��У�
#define		DEBUG_RELAY_BP 93 //���Զϵ�
#define		DEBUG_RELAY_STOP 94 //���Խ���
#define		DEBUG_RELAY_WATCH 95 //���̵���ֵ
#define		DEBUG_RELAY_DBOUT 96 //���Կ��� 
#define		DEBUG_RELAY_DBIN 97 //���Կ���

/*-----------------------������Ŀ����---------------------------------*/
#define		SELF_CLASS		4	//��վ��ַ����

#define		CLASS_1_ADDR_MASK	0x4000	//һ����ַ����
#define		CLASS_2_ADDR_MASK	0x1800	//������ַ����
#define		CLASS_3_ADDR_MASK	0x0380	//������ַ����
#define		CLASS_4_ADDR_MASK	0x003F	//�ļ���ַ����

#define		CLASS_1_ADDR_OFFSET		14	//һ����ַƫ��
#define		CLASS_2_ADDR_OFFSET		11	//������ַƫ��
#define		CLASS_3_ADDR_OFFSET		7	//������ַƫ��
#define		CLASS_4_ADDR_OFFSET		0	//�ļ���ַƫ��

#define		CLASS_1_ADDR_VALID	0x8000	//һ����ַ��Ч
#define		CLASS_2_ADDR_VALID	0x2000	//������ַ��Ч
#define		CLASS_3_ADDR_VALID	0x0400	//������ַ��Ч
#define		CLASS_4_ADDR_VALID	0x0040	//�ļ���ַ��Ч


#if (SELF_CLASS==1)
#define 	NET_ADDR_MASK		CLASS_1_ADDR_MASK	//��վ��ַ����
#define 	NET_ADDR_VALID		CLASS_1_ADDR_VALID	//��Ч��ַ
#endif

#if (SELF_CLASS==2)
#define 	NET_ADDR_MASK		CLASS_2_ADDR_MASK
#define 	NET_ADDR_VALID		CLASS_2_ADDR_VALID	//��Ч��ַ
#endif

#if (SELF_CLASS==3)
#define 	NET_ADDR_MASK		CLASS_3_ADDR_MASK
#define 	NET_ADDR_VALID		CLASS_3_ADDR_VALID	//��Ч��ַ
#endif

#if (SELF_CLASS==4)
#define 	NET_ADDR_MASK		CLASS_4_ADDR_MASK
#define 	NET_ADDR_VALID		CLASS_4_ADDR_VALID	//��Ч��ַ
#endif

#define		BIT_FIR				0x80	//�װ�����λ
#define		BIT_FIN				0x40	//ĩ������λ
#define		BIT_CON				0x20	//��ȷ��λ
#define		TRAN_SEQ_MASK		0x3F	//�����֡���к�����
#define		APP_SEQ_MASK		0x1F	//Ӧ�ò�֡���к�����
#define		APP_REQSEQ_MAX		15		//�������֡���
#define		APP_RESSEQ_MIN		16		//��С��Ӧ֡���
#define		APP_RESSEQ_MAX		31		//�����Ӧ֡���

#define		SYS_RESET			0x01	//ϵͳ��λ
#define		HAVE_CLASS1			0x02	//��һ������
#define		HAVE_CLASS2			0x04	//�ж�������
#define		NEED_CONFIG			0x08	//����������
#define		STATUS_TIM_ERROR	0x10	//����ʱ��
#define		STATUS_CTL_ERROR	0x20	//�����쳣
#define		STATUS_CTL_BLOCK	0x40	//��������

#define		INDEX_SIZE_MASK		0x70	//ָ���ģ����

#define		INDEX_SIZE_0		0		//�޶�������
#define		INDEX_8BIT			1		//����ǰ��1���ֽ�����
#define		INDEX_16BIT			2		//����ǰ��2���ֽ�����
#define		INDEX_32BIT			3		//����ǰ��4���ֽ�����
#define		SCALE_8BIT			4		//����ǰ��1���ֽڶ����ģ
#define		SCALE_16BIT			5		//����ǰ��2���ֽڶ����ģ
#define		SCALE_32BIT			6		//����ǰ��4���ֽڶ����ģ

#define		QUALIFIER_CODE_MASK	0x0F	//�޶���������

#define		QUALI_START_STOP_8	0		//�������8λ��START��8λ�ֽڵ�STOP
#define		QUALI_START_STOP_16	1		//�������16λ��START��16λ��STOP
#define		QUALI_START_STOP_32	2		//�������32λ��START��32λ��STOP
#define		QUALI_NUM_8			3		//�������8λ�Ķ������
#define		QUALI_NUM_16		4		//�������16λ�Ķ������
#define		QUALI_NUM_32		5		//�������32λ�Ķ������
#define		QUALI_ADDR_8		6		//�������8λ�Ķ�����Ե�ַ�͸���
#define		QUALI_ADDR_16		7		//�������16λ�Ķ�����Ե�ַ�͸���
#define		QUALI_ADDR_32		8		//�������32λ�Ķ�����Ե�ַ�͸���
#define		QUALI_NORANGE		9		//�ޱ�̶Ρ���ָ���Ǹ�����ʽ��ȫ�����ݶ���

/*---------------------�����־������----------------------------*/
//����  0-15
#define 	SF_Time		  		(USER_SFLAG+0)    //��Ҫ��ʱ
#define 	SF_HaveBO         	(USER_SFLAG+1)    //��ң��
#define 	SF_HaveBOReturn   	(USER_SFLAG+2)    //��ң�ط�У
#define 	SF_HaveTQ         	(USER_SFLAG+3)    //��ͬ��
#define 	SF_HaveST         	(USER_SFLAG+4)    //�ж�ֵ
#define 	SF_NewHaveTQ      	(USER_SFLAG+5)    //��newͬ��
#define 	SF_PROTECTRESET   	(USER_SFLAG+6)    //�б�������
#define		SF_TIMING			(USER_SFLAG+7)	  //�ж�ʱ����

//״̬ 16-31
#define 	SF_NeedConfig       (USER_SFLAG+16)   //��Ҫ�·�����
#define 	SF_BURSTAI          (USER_SFLAG+17)   //�仯ң��ʱ������
#define 	SF_BURSTBI	    	(USER_SFLAG+18)   //δ���ͱ�λң��ʱ������
#define 	SF_AIREPORT         (USER_SFLAG+19)   //ȫң���ϱ�ʱ������
#define 	SF_BIREPORT         (USER_SFLAG+20)   //ȫң���ϱ�ʱ������
#define 	SF_CIREPORT         (USER_SFLAG+21)   //ȫң���ϱ�ʱ������
#define 	SF_SOE		    	(USER_SFLAG+22)   //δ����SOEʱ������
#define 	SF_RELAYALARM		(USER_SFLAG+23)   //δ���ͱ����澯����ʱ������
#define 	SF_RELAYSTART		(USER_SFLAG+24)   //δ���ͱ�����������ʱ������
#define 	SF_RELAYACTION		(USER_SFLAG+25)   //δ���ͱ�����������ʱ������
#define		SF_WAITCONFIRM		(USER_SFLAG+26)   //��վ�ȴ�ȷ�ϱ�־
#define		SF_NEEDCONFIRM		(USER_SFLAG+27)   //��վ����ȴ�ȷ�ϱ�־
#define 	SF_SAFEREPORT		(USER_SFLAG+28)	  //ƽ������
#define		SF_MAKECOEECHO		(USER_SFLAG+29)	  //�޸�ϵ������
#define 	SF_LEDREPORT        (USER_SFLAG+30)   //LED״̬�ϱ�ʱ������
#define 	SF_LEDBURST         (USER_SFLAG+31)   //LED�仯״̬

#define		SF_SENDRES_SET			(USER_SFLAG+32)	  //��ֵ����
#define		SF_SENDRES_SYSTEMSET	(USER_SFLAG+33)	  //������ֵ����
#define		SF_SENDRES_SW			(USER_SFLAG+34)	  //��ѹ�巵��
#define		SF_SENDRES_CF			(USER_SFLAG+35)	  //ϵ������

#define		SF_SENDRES_BIATTR		(USER_SFLAG+36)	  //ң�����Է���
#define		SF_SENDRES_BOATTR		(USER_SFLAG+37)	  //ң�����Է���
#define		SF_SENDRES_AOATTR		(USER_SFLAG+38)	  //ң�����Է���
#define		SF_SENDRES_MBOPARA		(USER_SFLAG+39)	  //����λ�����������

#define 	SF_AIE		    		(USER_SFLAG+40)   //δ����AIEʱ������
#define		SF_SENDRES_DISTDIR		(USER_SFLAG+41)	  //¼��Ŀ¼����


#define		PORT_ID_CANA		((SC_PORT_CANBUS<<8)+1)
#define		PORT_ID_CANB		((SC_PORT_CANBUS<<8)+2)
#define		PORT_ID_ETHA		((SC_PORT_ETHERNETA<<8)+1)
#define		PORT_ID_ETHB		((SC_PORT_ETHERNETB<<8)+1)

/*-----------------------�ṹ������------------------------------*/
//����֡��ʽ����
typedef struct
{
	BYTE 	START;			//��ʼ�ֽ�
	BYTE	LengthLo;		//֡�����ֽ�
	BYTE	LengthHi;		//֡�����ֽ�
	BYTE    DestAddrLO;		//Ŀ�ĵ�ַ
	BYTE    DestAddrHI;		//Ŀ�ĵ�ַ
	BYTE    SourceAddrLO;	//Դ��ַ
	BYTE    SourceAddrHI;	//Դ��ַ
	BYTE	TControl;		//���������ֽ�
	BYTE	APPControl;		//Ӧ�ò�����ֽ�
	BYTE	FunCode;		//������
	BYTE	Status;			//��վ״̬
	BYTE	ObjType;		//��������
	BYTE	ObjQulifier;	//�����޶���
	BYTE	Range;			//���
}TTxdHead;

typedef struct
{
	BYTE 	START;			//��ʼ�ֽ�
	BYTE	LengthLo;		//֡�����ֽ�
	BYTE	LengthHi;		//֡�����ֽ�
	BYTE    DestAddrLO;		//Ŀ�ĵ�ַ
	BYTE    DestAddrHI;		//Ŀ�ĵ�ַ
	BYTE    SourceAddrLO;	//Ŀ�ĵ�ַ
	BYTE    SourceAddrHI;	//Ŀ�ĵ�ַ
	BYTE	TControl;		//���������ֽ�
	BYTE	APPControl;		//Ӧ�ò�����ֽ�
	BYTE	FunCode;		//������
	BYTE	ObjType;		//��������
	BYTE	ObjQulifier;	//�����޶���
	BYTE	Range;			//���
}TRxdHead;

//ң�ؽṹ
typedef struct
{
	WORD No;	 			//ң��·��
	WORD Command;			//���� PBO_SELECT PBO_EXECUTE  PBO_CANCEL PBO_DIRECT
	WORD Attrib;			//״̬���Ϸ֣�PBO_NULL PBO_OPEN, PBO_CLOSE,
	WORD NeedRetFlag; 		//�Ƿ���Ҫң�ط�У��Ϣ��־
	BYTE Info[PBOINFOSIZE]; //������ң�ڱ���������Ϣ
	WORD RetResult;			//ң�ط�У״̬ PBO_SUCESS  PBO_FAIL
}TCanBo;

//��ֵ�ṹ
typedef struct
{
	float *pSET;  			//��ֵ������
	WORD AreaNo;			//��ֵ����
	WORD SETNo;				//��ֵ���
	WORD Off;   			//����ƫ����
}TCanST;

//����������Ϣ
typedef struct
{
  	DWORD   	dwStatus;		//ͨ����������
	WORD		wAIRepTM;		//ң���ϱ����
	WORD		wBIRepTM;		//ң���ϱ����
	WORD		wCIRepTM;		//����ϱ����
	WORD		AIDeadArea;		//ң��������Χ(���֮)
	WORD		AIBurstRepTM;   //�仯ң���ϱ�
	WORD		BIBurstRepTM; 	//�仯ң���ϱ�
	WORD		SoeRepTM;		//SOE�ϱ����
	WORD		TimingTM;       //��ʱ���
	WORD		SaftyRepTM;		//ƽ������
	WORD		wLEDStatusTM;	//LED״̬
}TCanNetCfg;


#define	MAX_FILE_SECTIONSIZE		512	//�ļ��γ�
#define	FILE_STATUS_BEGINTRANS		1	

#define	FILE_START_READ				1
#define	FILE_STOP_READ				2
#define	FILE_START_WRITE			3
#define	FILE_STOP_WRITE				4
//�ļ��ṹ
typedef struct
{
	BYTE 		*pData;  			//�ļ�������
	char    	strFileName[32];	//�ļ���
	DWORD		dwFileLength;		//�ļ��ܳ���
	WORD		bStatus;			//=0:������1:����������
	WORD 		nLastSection;		//��һ���ļ��κ�
	WORD		nTotalSection;		//�ļ��ܶ���
	WORD 		nPerSize;   		//ÿ���ļ�����
	WORD		wFileCrc;			//�ļ�У��
	WORD		nTimer;				//�ļ����䶨ʱ��
}TCanFile;

//��Ӧ֡��¼
typedef struct
{
	BYTE	ResFunCode;		//��Ӧ������
	BYTE	ResSeq;			//��Ӧ֡���
	BYTE	ObjType;		//��������
	BYTE	ObjNum;			//�������
}TResInfo;


//CAN���ݶ�ʱ����ʶ
enum CANDATA_TIMER
{
	AIBURST_ID=0,		//�����仯ң��
	AI_ID,				//����ȫң��
	BI_ID,				//����ȫң��
	CI_ID,				//����ȫ���
	BIBURST_ID,			//δ���ͳɹ��ı仯ң�ŷ��ͼ��
	SOE_ID,				//δ���ͳɹ���SOE���ͼ��
	RELAYALARM_ID,		//δ���ͳɹ��ı����澯����ͼ��
	RELAYSTART_ID,		//δ���ͳɹ��ı�����������ͼ��
	RELAYACTION_ID,		//δ���ͳɹ��ı�����������ͼ��
	BORET_ID,			//ң�ط�У
	SAFETY_ID,			//ƽ�����ķ��ͼ��
	SNINFO_ID,
	LED_ID,				//LED״̬
	TIMING_ID, 			//��ʱ
	TIMER_NUM			//��ʱ������
};

// BI��������
typedef struct
{
	WORD	wControl;
	WORD	wFilterTime;
}TBIProPara;

// BO��������
typedef struct
{
	WORD	wLastTime;
}TBOProPara;

// AO��������
typedef struct
{
	WORD			wControl;			//������D0~D2��	
										//		=0:��5V;
										//		=1:��10V; 
										//		=2:20mA;
	WORD			wCHIndex;			//����	������
	WORD			wRange;				//����	
}TAOProPara;

#define DIST_FILENAME_LEN	16

typedef struct
{
	BYTE		nFileNum;	// �ļ��ܸ���
	BYTE		nSentCnt;	// �ѷ����ļ�����
	char		strFileNameList[DIST_CAPACITY_MAX][DIST_FILENAME_LEN];//�ļ������б�
	TSysTime	tFileTimeList[DIST_CAPACITY_MAX];//¼���ļ�ʱ���
}TDistDirItem;

void	sCanBusTask(void);

class CSCanbusPro : public CProtocol
{
	DECLARE_MESSAGE_MAP()

public:
	BOOL Init(void);//��ʼ��
	CSCanbusPro(DWORD *pdwApp):  CProtocol(pdwApp){};
			
protected:
	TRxdHead	*m_RxdHead;			//����֡ͷ
	TCanBo		m_Bo;				//ң����Ϣ��¼
	TCanST		m_SET;				//��ֵ��Ϣ��¼
	TCanFile	m_File;				//�ļ����ͼ�¼
	TCanNetCfg	m_NetCfg;			//����������Ϣ
	TResInfo	m_ResInfo;			//��Ӧ��Ϣ��������������Ӧ
	TDistDirItem	m_DistDir;		//¼��Ŀ¼	

	WORD		m_nMaxFrameLen;		//���֡��
	WORD		m_nBurstIdle;		//����ʱ��
	WORD		m_nRetryIdle;		//�ط�����
	
	WORD		m_wSAddr;			//ģ���ַ				
	WORD		m_wDAddr;			//Ŀ�ĵ�ַ				
	WORD 		dwGBAddress;	    //�㲥���ĵĵ�ַ
	WORD 		m_wTimer[TIMER_NUM];//��ʱ��
	DWORD  		m_dwTaskRunCount;  	//������������ʱ��ͳ��(��)
	DWORD		m_NoRxdCounter;		//�޽������ݼ���
	BYTE		m_LastReqSeq;		//�ϴ������SEQ
	BYTE		m_CurrentReqSeq;	//��ǰ����֡��SEQ
	BYTE		m_CurrentResSeq;	//��ǰ��Ӧ֡��SEQ
	WORD		LastAIPtr;			//�ϴ��ѷ���ȫң��ָ��
	WORD		LastBIPtr;			//�ϴ��ѷ���ȫң��ָ��
	WORD		LastCIPtr;			//�ϴ��ѷ���ȫ���ָ��
	WORD		LastBurstAIPtr;		//�ϴ��ѷ��ͱ仯ң��ָ��
	WORD		m_WrapHeadPtr;		//��ǰ�¼�ͷָ�룬ά��������
	WORD		m_OptEventPtr;
	WORD		m_LogInfoPtr;
	WORD		m_wFileOffset;		//��װ�ļ�ƫ����

	WORD		m_RetryAddr;		//�ط���ַ
	WORD		m_RetryLen;			//�ط�����
	WORD		m_RetryCount;		//�ط�����
	
	WORD		m_wWatchPortID; 	//���Ӷ˿�ID
	WORD		m_nWatchPortDog;	//�˿ڼ��ӿ��Ź�

	BYTE		m_CommandSeq;		//��ȷ�����
	WORD        m_CommandAddr;		//��ȷ�ϵ�ַ
	WORD		m_FileTransTimer;	//�ļ����䶨ʱ��
	PBYTE		m_RetryBuf;			//�ط�������
	SWORD		*pOldAI;			//��ž�ң���������ָ��
	void		*m_pMemBlock;		//Ԥ���ڴ��
	BYTE		*m_pLEDStatus;
	BYTE		*m_pLEDFreq;

	WORD		m_TransDataRP;		//͸�����ݶ�ָ��			
	WORD		m_TransReqPortID;	//����͸���˿�ID
	
	WORD	Get_MaxFrameLen();
	WORD 	Get_CommIdle();
	WORD 	Get_RetryIdle();
	
	BOOL 	IsNetTask(void);
	void 	TxdMonitor();
	
	DWORD 	SearchOneFrame(PBYTE Buf,WORD wLen);
	BYTE 	GetStationStatus();
	BYTE 	HaveClass1Data();
	WORD  	GetLayerAddr(WORD wAddr);
	void 	ClearFileTrans();

	void 	OnCommIdle(DWORD dwLen,DWORD PortID);
	void 	OnFrame(PBYTE Buf);
	void 	OnTimeOut(DWORD id);
	
	void 	OnSOE();
	void 	OnAIE();
	void 	OnRelayEvent(DWORD MsgCode, DWORD wParam);
	void 	OnBIBurst();
	void    OnRelayAlarm();
	void    OnRelayStart();
	void    OnRelayAction();
	void 	OnBOSelectEcho(WORD switchno,WORD attrib,WORD result);
	void 	OnBOExecEcho(WORD switchno,WORD Mode,WORD result);
	void 	OnBODirectEcho(WORD switchno,WORD Mode,WORD result);	
	void 	OnCOEMake(DWORD hPara,DWORD wPara,DWORD pPara,WORD lPara);
	void 	OnAutoAdjustEcho(DWORD dwResult,DWORD dwClass);
	void 	OnTransOIE(DWORD dwPort);

	void	AlterConfig(DWORD dwFlag);
	BOOL 	AIChange(long diff,long OldAI,WORD wAIDeadArea);

	void 	YKPocess();
	void 	IncResReq();
	void 	BackRetryFrame();
	BOOL 	ProcessRetryStatus();
	
	void 	TxdHead(BYTE AppCtrl,BYTE FunCode);
	void 	TxdTail(DWORD dwTxdFlag);
	
	void 	TxdConfirm();
	BOOL  	TxdAI();
	BOOL  	TxdBI();
	BOOL  	TxdCI();
	BOOL 	TxdBurstAI();
	BOOL 	TxdBurstBI();
	BOOL 	TxdSoe();
	BOOL 	TxdAIE();
	BOOL 	TxdRelayAlarmReport();
	BOOL 	TxdRelayStartReport();
	BOOL 	TxdRelayActionReport();
	BOOL 	TxdBOReturn();
	void	TxdIdleRep();
	BOOL 	TxdSafetyRep();


	BOOL 	RxdReadSETReq();
	BOOL 	RxdReadSystemSETReq();
	BOOL 	RxdReadSWReq();
	BOOL 	RxdReadCFReq();
	
	BOOL  	TxdAIRes();
	BOOL  	TxdAIWithQualityRes();
	BOOL  	TxdBIRes();
	BOOL  	TxdBIWithQualityRes();
	BOOL  	TxdCIRes();
	BOOL  	TxdCIWithQualityRes();
	BOOL	TxdSetCIValueRes();
	BOOL    TxdWriteSETRes();
	BOOL 	TxdWriteSETAreaRes();
	BOOL    TxdWriteSystemSETRes();
	BOOL    TxdWriteSWRes();
	BOOL 	TxdWriteCFRes();
	BOOL    TxdReadSETRes();
	BOOL 	TxdReadSETAreaRes();
	BOOL    TxdReadSystemSETRes();
	BOOL    TxdReadSWRes();
	BOOL 	TxdReadCFRes();
	
	BOOL  	TxdCoeRes();
	BOOL  	TxdAbsTimeRes();
	BOOL  	TxdLogInfoRes();


	WORD 	FillBurstBIEvent(BYTE *pBuf,TBurstBI *pEvnet);
	WORD  	FillSOEEvent(BYTE *pBuf,TEventBI *pEvnet);
	WORD 	FillOperateEvent(BYTE *pBuf,TMiscEvent *pEvnet);
	WORD 	FillRelayEvent(BYTE *pBuf,BYTE *pEvent,BOOL bItem);
	WORD 	FillInvalidEvent(BYTE *pBuf);
	BOOL 	TxdWrapAttrRes();

	BOOL  	TxdBIBByReadRes();
	BOOL  	TxdBIEByReadRes();
	BOOL  	TxdOptEventByReadRes();
	BOOL 	TxdRelayAlarmReportByReadRes(BOOL bList);
	BOOL 	TxdRelayStartReportByReadRes(BOOL bList);
	BOOL 	TxdRelayActionReportByReadRes(BOOL bList);

	BOOL  	TxdBIBByWriteRes();
	BOOL  	TxdBIEByWriteRes();
	BOOL  	TxdOptEventByWriteRes();
	BOOL 	TxdRelayAlarmReportByWriteRes(BOOL bList);
	BOOL 	TxdRelayStartReportByWriteRes(BOOL bList);
	BOOL 	TxdRelayActionReportByWriteRes(BOOL bList);

	BOOL  	TxdVarRes();
	BOOL	TxdWriteVarRes();

	BOOL 	TxdFileLoad();
	BOOL 	TxdFileDown();
	
	BOOL	RxdConfirm();
	void 	RxdNetCfg();
	BOOL  	RxdAbsTime();
	void  	RxdCoe();
	void    RxdFileDown();
//	BOOL 	RxdDDSetValue();

	BOOL 	TxdReadChannelAttrRes();
	BOOL 	TxdWriteChannelAttrRes();

	BOOL 	TxdReadChannelSrcRes();
	BOOL 	TxdWriteChannelSrcRes();

	BOOL 	TxdReadChannelValueRes();
	BOOL 	TxdWriteChannelValueRes();

	BOOL 	RxdReadBIAttrReq();
	BOOL 	TxdReadBIAttrRes();
	BOOL 	TxdWriteBIAttrRes();
	
	BOOL 	RxdReadBOAttrReq();
	BOOL 	TxdReadBOAttrRes();
	BOOL 	TxdWriteBOAttrRes();

	BOOL 	RxdReadAOAttrReq();
	BOOL 	TxdReadAOAttrRes();
	BOOL 	TxdWriteAOAttrRes();

	BOOL 	TxdReadSysParaRes();
	BOOL 	TxdWriteSysParaRes();
	BOOL 	TxdReadVersionRes();
	BOOL 	TxdReadDevStatusRes();

	BOOL 	TxdReadCFGRes();
	BOOL 	TxdWriteCFGRes();

	BOOL 	TxdRelayControlRes(BOOL bBlock);
	BOOL 	TxdSpecialOperateRes();

	BOOL 	TxdReadAOCoeRes();
	BOOL 	TxdWriteAOCoeRes();
	BOOL 	TxdSelectAOCoeRes();

	
	BOOL  	TxdLEDStatus(BOOL bBurst);
	BOOL  	TxdLEDStatusRes();
	BOOL 	TxdWatchPortRes();
	BOOL 	TxdVarDataRes();

	BOOL 	TxdSRMRes();
	BOOL 	TxdReadDevDispRes();

	BOOL	TxdReadRWStatusRes();
	BOOL	RxdWriteRWStatus();	

	BOOL 	RxdReadMBOReq();
	BOOL 	TxdReadMBORes();
	BOOL 	TxdWriteMBORes();

	BOOL 	TxdReadBreakerTripCntRes();
	BOOL 	RxdWriteBreakerTripCnt();

//-------------------------debugPLC----------------
	BOOL Rxd_DebugStep1Req(); 
	BOOL Txd_DebugStep1Res(); 
	BOOL Rxd_DebugStep2Req();
	BOOL Txd_DebugStep2Res();
	BOOL Rxd_DebugSetBreakPointReq(); //�������öϵ�����
	BOOL Txd_DebugSetBreakPointRes(); //���յ����öϵ�Ӧ��
	BOOL Rxd_DebugGetContextReq(); //��ȡ�����ģ������кź������ļ̵�����
	BOOL Txd_DebugGetContextRes(); //���յ����·���
	BOOL Rxd_DebugStopReq();//���Խ���
	BOOL Txd_DebugStopRes();	
	BOOL Rxd_DebugWatchReq();//plc��ȡ�̵���
	BOOL Txd_DebugWatchRes();

	//��ȡ��������̵���
	BOOL Rxd_GetDBOutReq();
	BOOL Txd_GetDBOutRes();
	//���õ�������̵���
	BOOL Rxd_SetDBINReq();
	BOOL Txd_SetDBINRes(BYTE byCout, WORD wStart);
	
	BOOL RxdTransData();//����͸������
	BOOL TxdTransData();//����͸������

	BOOL TxdReadFLParaRes();
	BOOL TxdWriteFLParaRes();
	
	BOOL TxdReadDCSIDataRes();
	BOOL Rxd_SetAIValue(BYTE FunCode);
	BOOL Rxd_SetBIValue(BYTE FunCode);
	
	void ClearAllEvent();
	BOOL RxdReadDistDirReq();
	BOOL TxdReadDistDirRes();

	BOOL TxdReadClass1();
	BOOL TxdReadClass2();
};

#endif	