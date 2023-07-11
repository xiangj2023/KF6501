/********************************************************************************************************/
/*																										*/
/*        		��򿭷������ɷ����޹�˾																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      paradefs.h                                     1.0												*/
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ��������л�����ص����ݽṹ																*/
/*																										*/
/* ����																									*/
/*																										*/
/*      ����																							*/
/*																										*/
/* ��������ݽṹ																						*/
/*																										*/
/*    	TSysConfigTable						ϵͳ���ñ�													*/
/*		TStatus								ϵͳ״̬													*/
/*		TMCBH								��̬�ڴ����												*/
/*		TRecordWrap							���м�¼��													*/
/*		TRecordBlock						���м�¼����												*/
/*		TMiscWrap							������¼��													*/
/*		TEventMisc							������¼����												*/
/*		TAppInfo							���ݿ������												*/
/* ����																									*/
/*																										*/
/*      None																							*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*		Dbasedef.h						���ݿ�ṹ����													*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0												*/
/********************************************************************************************************/
#ifndef __DAEDEFS_H
#define __DAEDEFS_H

#include "Dbasedef.h"		/*���ݿⶨ��*/

//ϵͳ�Լ�״̬
#define		SS_INTERNAL_RAM_ERR			(0)    	//�ڲ�RAM�쳣
#define		SS_EETERNAL_RAM_ERR			(1)    	//�ⲿRAM�쳣
#define		SS_PROGRAM_ROM_EERR			(2)    	//����ROM�쳣
#define		SS_NVRAM_ERR				(3)    	//NVRAM���Գ���
#define		SS_FLASH_ERR				(4)   	//FLASH���Գ���
#define		SS_EEPROM_ERR				(5)    	//EEPROM����
#define		SS_AD_ERR					(6)    	//AD�����쳣
#define		SS_LCD_ERR					(7)    	//Һ����ʾ����
#define		SS_INSULATION_ERR			(8)    	//����������
#define		SS_INSULATION_INV			(9)    	//������ʧЧ
#define		SS_INSULATION_ALM			(10)   	//���ʧ��澯 
#define		SS_CONTROL_LOOP_BREAK		(11)   	//���ƻ�·����
#define		SS_PT_BREAK					(12)   	//PT����
#define		SS_CT_BREAK					(13)   	//CT����
#define		SS_CLOCK_ERR				(14)   	//ʱ�ӹ���
#define		SS_PPWER_ERR				(15)   	//��Դ����
#define		SS_DSP_ERR					(16)   	//DSP����

#define		SS_TIME_B					(17)    //B��ʽ�����쳣
#define		SS_TIME_GPS					(18)    //GPSʱ�ӹ���
#define		SS_ETHERNET_ERR				(19)    //��̫��ͨѶ����
#define		SS_CAN_ERR					(20)    //�豸ͨѶ����
#define		SS_PARA_CHANGE				(21)    //�����ļ��ı�                    
#define		SS_DOWNLOAD_PARA			(22)    //��Ҫ��װ����
#define		SS_UPLOAD_PARA				(23)    //��Ҫ��װ����                  
#define		SS_PARA_ERR					(24)    //�����ļ���
#define		SS_PARA_ROM_ERR				(26)    //����ROM�쳣
#define		SS_CANBUS_ERR				(27)    //CANBUS������
#define		SS_PARTNER_ERR				(28)    //��������
#define		SS_COLD_RESET				(29)    //ϵͳ������ 
#define		SS_NETACOMM					(30)    //A��ͨ���쳣 
#define		SS_NETBCOMM					(31)    //B��ͨ���쳣 
#define 	SS_12V_ERR					(32)	//12V��Դ�쳣

#define		SS_SYS_HARD_ERR				(40)    //Ӳ��
#define		SS_SYS_CONFIG_ERR			(41)    //ϵͳ���ô�
#define		SS_SYS_STATUS_ERR			(42)    //ϵͳ״̬��
#define		SS_NVRAM_LINK_ERR			(43)    //NVRAM�����
#define		SS_NVRAM_INIT				(44)    //NVRAM��ʼ��

#define		SS_FILE_BASICPARA_ERR		(64)	//���������ļ�����
#define		SS_FILE_RELAYDISP_ERR		(65)	//���������ļ�����
#define		SS_FILE_RELAYDATA_ERR		(66)	//���������ļ�����
#define		SS_FILE_RELAYEVENT_ERR		(67)	//�����¼��ļ�����
#define		SS_FILE_BASICEVENT_ERR		(68)	//�����¼��ļ�����

#define		SS_FILE_BASICPARA_CHG		(72)	//���������ļ��޸�
#define		SS_FILE_RELAYDISP_CHG		(73)	//���������ļ��޸�
#define		SS_FILE_RELAYDATA_CHG		(74)	//���������ļ��޸�
#define		SS_FILE_RELAYEVENT_CHG		(75)	//�����¼��ļ��޸�
#define		SS_FILE_BASICEVENT_CHG		(76)	//�����¼��ļ��޸�


#define		SS_FILE_DIFFRELAYDATA_ERR	(80)	//�������ݸ�����һ��

#define		SS_RAM_SW_ERR				(96)    //RAM��ѹ�����
#define		SS_RAM_CFG_ERR				(97)    //RAM�����ֳ���
#define		SS_RAM_SET_ERR				(98)    //RAM��ֵ����
#define		SS_RAM_SYSSET_ERR			(99)    //RAM������ֵ����
#define		SS_RAM_CF_ERR				(100)    //RAMϵ������
#define		SS_RAM_DATA_ERR				(111)    //RAM���ݳ���

#define		SS_NVRAM_MISC_ERR			(112)    //NVRAM������¼���ݴ�
#define		SS_NVRAM_RECORD_ERR			(113)    //NVRAM���м�¼�����
#define		SS_NVRAM_BURSTBI_ERR		(114)    //NVRAM��λң�ų���
#define		SS_NVRAM_EVENTBI_ERR		(115)    //NVRAM SOE ����
#define		SS_NVRAM_RSTTWRAP_ERR		(116)    //NVRAM �����������ĳ���
#define		SS_NVRAM_CI_ERR				(117)    //NVRAM���м�¼�����
#define		SS_NVRAM_RALMWRAP_ERR		(118)    //NVRAM�澯�����쳣
#define		SS_NVRAM_RACTWRAP_ERR		(119)    //NVRAM���������쳣

#define		SS_EEPROM_SW_ERR			(128)    //EEPROM��ѹ�����
#define		SS_EEPROM_CFG_ERR			(129)    //EEPROM�����ֳ���
#define		SS_EEPROM_SET_ERR			(130)    //EEPROM��ֵ����
#define		SS_EEPROM_SETAREA_ERR		(131)    //EEPROM��ֵ���ų���
#define		SS_EEPROM_SYSSET_ERR		(132)    //EEPROM������ֵ����
#define		SS_EEPROM_CF_ERR			(133)    //EEPROMϵ������
#define		SS_EEPROM_RACTWRAP_ERR		(134)    //
#define		SS_EEPROM_RALMWRAP_ERR		(135)    //

#define		SS_EEPROM_DATA_ERR			(136)    //EEPROM���ݳ���
#define		SS_EEPROM_WRITE_ERR			(137)    //EEPROMд�����

#define		SS_MAPBIT_PARA_ERR			(256)    //����λ��������
#define		SS_MAPBIT_FUNC_USE			(257)    //����λ����Ͷ��
#define		SS_MAPBIT_COMM_ERR			(264)    //����λ����ͨ��ʧ��(32λ)

#define		SS_FL1_COMM_ERR			(296)	 //���1ͨ��ʧ��
#define		SS_FL2_COMM_ERR			(297)	 //���2ͨ��ʧ��
#define		SS_FL3_COMM_ERR			(298)	 //���3ͨ��ʧ��
#define		SS_FL4_COMM_ERR			(299)	 //���4ͨ��ʧ��
#define		SS_FL5_COMM_ERR			(300)	 //���5ͨ��ʧ��

#define		SS_PORT_COMM_ERR		(320)	 //�˿�ͨ��״̬(Ԥ��16��)
#define		SS_ETHA_COMM_ERR		(320)	 //��̫��Aͨ��ʧ��(Ԥ��5������)
#define		SS_ETHB_COMM_ERR		(325)	 //��̫��Bͨ��ʧ��(Ԥ��5������)
#define		SS_SER_COMM_ERR			(330)	 //����ͨ��ʧ��(Ԥ��4������)
#define		SS_CAN_COMM_ERR			(334)	 //CAN1ͨ��ʧ��(Ԥ��2��CAN��)

#define	DAE_MAX_FLAGS			512

typedef struct 
{
    DWORD Flag[ DAE_MAX_FLAGS/32 ];  //
}DAEFLAGS ; 

#define	SYS_RUNNING_OK		0x494E4954				//"INIT"

//ϵͳ����״̬
typedef struct
{
	DWORD					dwFlag;					//���б�־		
	TAbsTime				stTime;					//ϵͳʱ��		
	DAEFLAGS				Status;					//����״̬
	DAEFLAGS				InitStatus;				//װ�ó�ʼ��״̬
	DWORD					dwNetAMask;				//A����������		
	DWORD					dwNetBMask;				//B����������		
	DWORD					dwRelayFaultNo;			//�������ϱ��
	DWORD					dwRsv[8];				//����				
}TStatus;

//NvRAM�ڴ�ǰ׺
#define	MCB_SIGN_ALLOC			0x4E474953				//"SIGN"
#define	MCB_SIGN_FREE			0x5453414c				//"LAST"

typedef struct
{
	DWORD					dwSign;					//���ݱ�־	=MCB_SIGN_ALLOC  �ѷ���
													//			=MCB_SIGN_LAST  ���һ��
													//			=����  ��Ч
	DWORD					dwSize;					//�ڴ�鳤��	
}TMCB;

//NvRAM�ڴ��������
#define	MMLNK_SIGN_VALID		0x54494E49				//"INIT"

typedef struct
{
	DWORD					dwSign;					//��Ч��־		=0xAAAA��Ч��������Ч
	DWORD					dwSize;					//�����ڴ泤��	
	BYTE*					pFree;					//�����ڴ�ָ��	
	TMCB*					pAlloced;				//�ѷ����ڴ�ָ��	
}TMemLink;


#define WRAP_INIT_FLAG		0x54494E49				//"INIT"

//���м�¼��
#define	ES_MAX_LINE			254
#define MAX_RUNREC_NUM		256	

typedef struct
{
	WORD					nCharSize;					//��¼����,������¼�ĳ���
	char					szRecordMsg[ES_MAX_LINE];	//��¼�� 
}TRecordBlock;


typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TRecordBlock			pEvent[MAX_RUNREC_NUM];		//�����			
}TRecordWrap;


//������¼��
#define MAX_MISC_NUM			256	

#define MISC_CLASS_RESET		1   //ϵͳ��λ�¼�
#define MISC_CLASS_DOWNFILE		2	//��װ�ļ�
#define MISC_CLASS_COMM			3	//ͨѶ�ж�
#define MISC_CLASS_BO			4	//ң�ز���
#define MISC_CLASS_AO			5	//ң������
#define MISC_CLASS_SYNC			6	//ͬ�ڲ���
#define MISC_CLASS_SET			7	//���Ķ�ֵ
#define MISC_CLASS_SETAREA		8	//���Ķ�ֵ����
#define MISC_CLASS_SW			9	//������ѹ��
#define MISC_CLASS_CFG			10	//��������
#define MISC_CLASS_CF			11	//����ϵ��
#define MISC_CLASS_PARA			12	//�޸Ĳ���
#define MISC_CLASS_POWER		13	//װ�õ���
#define MISC_CLASS_CLEARCI		14  //��ȵ�������
#define MISC_CLASS_MODIAI		15	//�޸�ң��
#define MISC_CLASS_MODIBI		16	//�޸�ң��
#define MISC_CLASS_MODICI		17	//�޸ĵ��
#define MISC_CLASS_CHANNELSRC	18	//Ͷ��ͨ������
#define MISC_CLASS_ZERODRIFT	19	//������Ư
#define MISC_CLASS_FULLVALUE	20	//������ֵ
#define MISC_CLASS_AOCOEF		21	//ģ�����������

typedef struct
{
	WORD					nClass;						//�������	
	TAbsTime				absTime;					//����ʱ��	
	DWORD					dwDevName;					//��������	���������¼����豸��
	DWORD					dwOperator;					//����Դ		��������Ż�˿ں�
	DWORD					dwValue;					//����ֵ		
	DWORD					dwAttr;						//��������	
}TMiscEvent;

typedef struct
{
	DWORD					dwMagic;					//��ʼ����־,��Ч��־WRAP_INIT_FLAG
	DWORD					hSem;						//�ź���ID,������Դ����
	WORD					nMax;						//�����С		
	WORD					nHeadIndex;					//ͷָ��			
	WORD					nTailIndex;					//βָ��			
	TMiscEvent				pEvent[MAX_MISC_NUM];		//�����			
}TMiscWrap;

/*----------------------------------------------------------------------------------------*/
/* �����������ӳ�����                                                                   */
/*----------------------------------------------------------------------------------------*/
#define MAX_TASK_NAME 		8
#define DEFINE_APPLICATION(name,entry,pri,sstack)      {name,(TTASK_ENTRY)entry,pri,sstack}

typedef struct {
    char          			szName[MAX_TASK_NAME];      /*��������*/
    TTASK_ENTRY   			fpEntry;                  	/*���������Ӧ���������*/
    WORD          			wPriority;            	 	/*��Ӧ��������ȼ�*/
    WORD          			wSysStack;             		/*����������ʱ��Ҫ��ϵͳ��ջ��С*/
}TAppItem;

//Lcd��Dbase��Misi��WatchDag��AI��BI��BO��
//���ݿ������
#define SYS_PORT_MAX       				16
#define SYS_PORTID_AI       			((SC_PORT_SYSTEM<<8)+1)
#define SYS_PORTID_DI       			((SC_PORT_SYSTEM<<8)+2)
#define SYS_PORTID_BO       			((SC_PORT_SYSTEM<<8)+3)
#define SYS_PORTID_DBASE        		((SC_PORT_SYSTEM<<8)+4)
#define SYS_PORTID_SERADMIN     		((SC_PORT_SYSTEM<<8)+5)
#define SYS_PORTID_CANADMIN     		((SC_PORT_SYSTEM<<8)+6)
#define SYS_PORTID_ETHMAIN     			((SC_PORT_SYSTEM<<8)+7)
#define SYS_PORTID_WATCHDOG          	((SC_PORT_SYSTEM<<8)+8)
#define SYS_PORTID_RELAY          		((SC_PORT_SYSTEM<<8)+9)
#define SYS_PORTID_BACKUP          		((SC_PORT_SYSTEM<<8)+10)
#define SYS_PORTID_MAPBIT          		((SC_PORT_SYSTEM<<8)+11)
#define SYS_PORTID_LCD        			((SC_PORT_SYSTEM<<8)+12)    
#define SYS_PORTID_FL        			((SC_PORT_SYSTEM<<8)+13)    

#define	APP_STATUS_PORTSTATUSSHIFT		0
#define	APP_STATUS_PORTSTATUSMASK		0x00000003
#define	APP_STATUS_ACTIVEMASK			0x00000080

#define	APP_STATUS_MAIN					0
#define	APP_STATUS_WARMBACK				1
#define	APP_STATUS_COLDBACK				2

/*����˿��¼���־*/
#define APP_EVENT_ALL     				(0xFFFF)
#define APP_EVENT_AIE     				(1<<0)     /* ��ң���¼�*/
#define APP_EVENT_AIF     				(1<<1)     /* �ж���ң��*/
#define APP_EVENT_SOE     				(1<<2)     /* ��SOE*/
#define APP_EVENT_BIE     				(1<<3)     /* �б�λң��*/
#define APP_EVENT_CIF     				(1<<4)     /* �ж�����*/
#define APP_EVENT_OIE     				(1<<5)     /* ��͸������*/
#define APP_EVENT_SPIE    				(1<<6)     /* �д�ʱ���SPI*/
#define APP_EVENT_RALM    				(1<<7)     /* �д�ʱ���SPI*/
#define APP_EVENT_RSTT    				(1<<8)     /* �д�ʱ���SPI*/
#define APP_EVENT_RACT    				(1<<9)     /* �д�ʱ���SPI*/
#define APP_EVENT_COMM    				(1<<10)     /* �˿�ͨ���ж�*/

#define	GET_APPINFO_PORTSTATUS(s)			((s)&APP_STATUS_PORTSTATUSMASK)	

typedef struct
{
	TAppItem 				*pAppItem;					//����������
	HANDLE					hTaskID;					//�����ʶ			
	DWORD					dwPortID;					//�˿ں�				
	DWORD					bStatus;					//����״̬,D0~D3�˿�״̬
														//			=0 ���˿�                       
														//			=1 �䱸�ö˿�
														//			=2 �ȱ��ö˿�        

														//			D7
														//			=1 �˿ڼ��� 
														//			=0 ������
														//��������
	DWORD					bMask;						//��������,�ɷ�������Ȩ��
	DWORD					bEvent;						//�����־��,��������ı�־��Ϣ
	WORD					nBIERead;					//��λң�Ŷ�ָ��		
	WORD					nSOERead;					//SOE��ָ��			
	WORD					nRSTTRead;					//�������������ָ��	
	WORD					nRALMRead;					//�����澯�����ָ��	
	WORD					nRACTRead;					//�������������ָ��	
	WORD					nAIERead;					//AIE��¼��ָ��			
	WORD					nLogRead;					//���м�¼��ָ��		
	WORD					nMISCRead;					//������¼��ָ��			
}TAppInfo;

#define DAE_MAX_APP_NUM			32

//�������ñ�
typedef struct
{
	TStatus					SystemStatus;				//ϵͳ״̬	
	TMemLink				NvRamLink;					//��̬�ڴ����
	TRecordWrap				RecordWrap;					//���м�¼	
	TMiscWrap				MiscWrap;					//������¼	
	TAppInfo				TaskApp[DAE_MAX_APP_NUM];   //���ݿ������
	TDBaseConfigTable		DBaseConfig;				//���ݿ����ñ�
}TSysConfigTable;

#define	DB_BLANK			0xFFFF

#define BAISIC_INIT_FLAG		0x494E4954				//"INIT"
//�������ñ�
typedef struct
{
	DWORD	dwInitFlag;			//��ʼ����־
	DWORD	dwNetAIP;			//A��IP��ַ
	DWORD	dwNetAMask;			//A��IP����
	DWORD	dwNetAGWIP;			//A������IP��ַ
	DWORD	dwNetBIP;			//A��IP��ַ
	DWORD	dwNetBMask;		//A��IP����
	DWORD	dwNetBGWIP;			//A������IP��ַ
	WORD	wNetAddress;		//������ַ
	char	chPassWord1[8];		//����1					
	char	chPassWord2[8];		//����2					
	WORD	wCheck;				//У��
}TBasicConfig;

#define DAE_MAX_TRANSLEN		1024
#define DAE_MAX_TRANSNUM		16

//͸������������
typedef struct
{
	WORD  	wClass;		//����
	WORD	wDesPort;	//Ŀ�Ķ˿ںţ�
	WORD	wSrcPort;	//Դ�˿ںţ����������һ��
	WORD	wDataLen;	//���ݳ���
	BYTE	Data;		//������
}TTransDataHead;

typedef union
{
	TTransDataHead tHead;
	BYTE	byDataBuf[DAE_MAX_TRANSLEN];
}TFixTransData;

typedef struct
{
	WORD					nTailIndex;						//βָ��			
	TFixTransData			pEvent[DAE_MAX_TRANSNUM];		//�����			
}TTransWrap;

#endif
