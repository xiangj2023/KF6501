
#ifndef _RESM5234_H_
#define _RESM5234_H_

#include "syscfg.h"

#define HW_VERSION_10		10
#define HW_VERSION_11		11
#define HW_VERSION_12		12

#define HARD_WARE_VER	HW_VERSION_12

//Ӳ����Դ����

//CPU�ڲ��Ĵ����ռ��ַ���壬ʹ��ȱʡ����
#define IPSBAR_ADDRESS      0x40000000	
#define __IPSBAR			IPSBAR_ADDRESS

//�����ж������ŷ�Χ
//����INTC0,������=64+�ж���Դ��, ����INTC1,������=128+�ж���Դ��
#define MIN_I_VECT_NO		65		
#define MAX_I_VECT_NO		192

//�ⲿ�ж϶���
#define BASE_CAN_IRQ		3
#define EXTERN_CAN_IRQ		4	//��չCAN����չ��̫��������ж���Դ
#define ADC_IRQ				6	//�½�����Ч

//���崮��������
#define MAX_SERIAL_NUM		(2+1)


//����Ӳ�����ö���
#define MCF5234_UART		YES
#define MCF5235_FLEXCAN		YES
#define MCF5234_SJA1000		YES

//װ��ģ�����Ͷ���
#define MODULE_TYPE			MD_KV3523

//�����ڲ�ϵͳʱ��Ƶ�ʣ���CPU�ں�Ƶ�ʵ�һ��
#define IN_SYS_CLOCK		75000000	/* System clock speed 75 MHz */

//ϵͳ��Դ���壬ֻ����������ֵ�Ƭѡ��ַ
#define SDRAM_BASE			0x80000000		//SDRAM��16M�ֽڣ�32λ
#define CODE_FLASH_BASE		0x00000000		//����FLASH��4M�ֽڣ�16λ
#define DATE_FLASH_BASE		0x10000000		//����FLASH��4M�ֽڣ�16λ
#define ADC_CS_BASE			0x30000000
#define ADC1_CS_BASE		0x30050050		//ADC1��1�֣�16λ	
#define ADC2_CS_BASE		0x300500a0		//ADC2��1�֣�16λ			
#define ADC3_CS_BASE		0x300500C0		//ADC3��1�֣�16λ			
#define EX_NET_CS_BASE		0x90000000		//��չ��̫����16λ
#define EX_CAN_CS_BASE		0xa2000000		//��չCAN����128�ֽڣ�8λ
#define EEPROM_PRO			0xa2030055		//EEPROMд������0��ֹ��1����1�ֽ�
#define WATCHDOG_BASE		0xa203005a		//���Ź�ˢ��
#define EXTER_IF_BASE		0xb0000000
#define DOA_CS_BASE			0xb0050000		//YKA��дң�ؼ����ڲ�ң�š���У��32�ֽڣ�8λ
#define DOB_CS_BASE			0xb00500a0		//YKB������������32�ֽڣ�8λ
#define DOC_CS_BASE			0xb0050060		//YKC��дң�ؼ����ڲ�ң�š���У��32�ֽڣ�8λ
#define DOD_CS_BASE			0xb00500c0		//YKD������������32�ֽڣ�8λ

#define DBASE_NVRAM_SIZE      	0x80000 

#define DBASE_NVRAM_LENGTH      0x60000 
#define DBASE_NVRAM_BASE        (0x20000000)     
#define	DBASE_NVRAM_END			(DBASE_NVRAM_BASE + DBASE_NVRAM_LENGTH)

#define	DBASE_NVRAM_DATA		0x20060000			//���ڵ�ȵȶ�̬����
#define	DBASE_NVRAM_CI			DBASE_NVRAM_DATA	//���



#define	DBASE_NVRAM_6520			0x20070000			//����6520װ�ø��ɼ��
#define DBASE_NVRAM_6520_LENGTH     512 
#define	DBASE_NVRAM_6520_END		(DBASE_NVRAM_6520 + DBASE_NVRAM_6520_LENGTH)

#define LOGIC_NVRAM_BASE (DBASE_NVRAM_6520_END+0xFF)
#define LOGIC_NVRAM_LENGTH 0xFF
#define LOGIC_NVRAM_END (LOGIC_NVRAM_BASE+LOGIC_NVRAM_LENGTH)


#define FL_NAVRAM_BASE (LOGIC_NVRAM_END)
#define FL_NAVRAM_LENGTH  1536
#define FL_NAVRAM_END (FL_NAVRAM_BASE+FL_NAVRAM_LENGTH)

//#define	DBASE_NVRAM_6520		0x20078000			//����6520װ�ø��ɼ��

/*
#define EEPROM_RELAY_EVENT      			0		
#define EEPROM_RELAY_SINGAL     			(111*1024)		
#define EEPROM_SYSPARA_LCDCTLPAR        	(111*1024+512)	//Һ��������ڲ�����4�ֽڣ���ǰ�Զȼ��Ƿ��Զ�����
#define EEPROM_RELAY_DATA					(112*1024)
*/
	//=======================EEPROM�ĵ�ַ�ռ仮��===============================
	// ************************ϵͳ������********************
#define 	EEPROM_SYSPARA_START     	(0)			//��ʼ
#define 	EEPROM_SYSPARA_LENGTH     	(2*1024)	//����
	
#define 	EEPROM_SYSPARA_LCDCTLPAR    (EEPROM_SYSPARA_START)	//Һ��������ڲ�����4�ֽڣ���ǰ�Զȼ��Ƿ��Զ�����
#define		EEPROM_BASICCONFIG_START	(EEPROM_SYSPARA_START+4)	//ϵͳ������Ϣ������IP��ַ�������ַ��
	
#define		EEPROM_DI_FILTERTIME		(EEPROM_SYSPARA_START+512)
#define		EEPROM_DI_FILTERTIME_SIZE	(512)  		//DI���������ڳ���
		
#define		EEPROM_DISTINDEX_START		(EEPROM_DI_FILTERTIME+EEPROM_DI_FILTERTIME_SIZE)	
#define		EEPROM_DISTINDEX_SIZE		(1024)  	//¼�������ļ�
	
	// ************************ϵ����*************************
#define     EEPROM_RELAY_CFBASE			(EEPROM_SYSPARA_START+EEPROM_SYSPARA_LENGTH) //��ʼ
#define     EEPROM_RELAY_CFSIZE			(2*1024)	//����
	
	// ************************������ֵ��������***************
#define 	EEPROM_RELAY_DATA			(EEPROM_RELAY_CFBASE+EEPROM_RELAY_CFSIZE)	//��ʼ	       			
#define 	EEPROM_RELAY_DATASIZE		(28*1024)	//����
	
	// ************************��������������*****************
#define 	EEPROM_RELAY_EVENT       	(EEPROM_RELAY_DATA+EEPROM_RELAY_DATASIZE)//��ʼ
#define 	EEPROM_RELAY_EVENTSIZE      (sizeof(TFileHead)+sizeof(TRelayEventPtrList)+\
													sizeof(TRelayActionEv)*MAX_RACT_NUM+sizeof(TRelayAlarmEv)*MAX_RALM_NUM+1024)
													
	// ************************�ź���������******************
#define 	EEPROM_RELAY_SINGAL			(EEPROM_RELAY_EVENT+EEPROM_RELAY_EVENTSIZE)	//��ʼ
#define 	EEPROM_RELAY_SINGALSIZE		(1*1024)	//����

#define	RELAY_RAM_BASE		0xA0008000		//���屣�����ڲ�RAM��ʼ��ַ
#define RELAY_RAM_LEN		0x8000			//���屣�����ڲ�RAM����

//�ж�װ���ϵ��λ�����жϱ�־(��CPU�ڲ�RAM�ж���)
#define SYS_START_FLAG_ADD	(RELAY_RAM_BASE+RELAY_RAM_LEN-4)

//ϵͳ����ʱ���жϳ����Ƿ��Զ�����bootrom�ı�־λ�ã�������bootrom�н����޸ģ����綪ʧ
#define SYS_BROM_FLAG_ADD	(RELAY_RAM_BASE+RELAY_RAM_LEN-8)

//�ж���Դ����
#define EXT_CAN_INT			MCF_INTC0_LVL_LVL4	//��չ��̫������չCAN���ж�
#define ADC_INT				MCF_INTC0_LVL_LVL6	//�����жϣ�ȫ��ADC������ɺ�����ж�
#define LOW_POWER_INT		MCF_INTC0_LVL_LVL7	//ϵͳ�����ѹ�ͱ���

//����ʱ��оƬ����
#define R8025				1
#define CLOCK_MODULE		R8025


//���崮��EEPROM����
#define AA1024				1
#define AA512				2
#define EEPROM_MODULE		AA1024

//����AD����оƬ����
#define MAX11046_DEV			1
#define AD7656_DEV			2
#define AD7606_DEV			3

//����ϵͳƵ�ʼ��·��
#define MAX_FREQ_NUM		2

//����Ӧ�ó��򳤶ȷ�Χ
#define MOST_CODE_FILE_LEN		0x380000
#define LEAST_CODE_FILE_LEN		0x100000

#if(HARD_WARE_VER == HW_VERSION_10)
	//һ�����������Դ����
	#define LED1_CS_BASE		0xb0030050		//���LED��4�ֽڣ�8λ��32����
	#define LED2_CS_BASE		0xb0030051		//���LED��4�ֽڣ�8λ��32����
	#define LED3_CS_BASE		0xb0030052		//���LED��4�ֽڣ�8λ��32����
	#define LED4_CS_BASE		0xb0030053		//���LED��4�ֽڣ�8λ��32����
	#define KEY1_CS_BASE		0xb0030054		//���KEY��2�ֽڣ�8λ
	#define LCD_CS_BASE			0xb0030000		//���LCD��2�ֽڣ�8λ

	//����LED

	//����Һ��������Ӳ��������ַ
	#define LCD_DATA_REG		0xB0030057		//��д���ݼĴ���
	#define LCD_CMD_REG			0xB0030056		//��д����Ĵ���
	#define LCD_RESET_ADDR		0xB0030055		//��λLCD���米����

	//����ʹ�õ�ADоƬ
	#define AD_DEV_TYPE			MAX11046_DEV

	#define BIA_CS_BASE			0xb00a0055		//YXA��5�ֽڣ�8λ,��ŵߵ�
	#define BIB_CS_BASE			0xb00a00a0		//YXB��5�ֽڣ�8λ

#elif(HARD_WARE_VER == HW_VERSION_11 ||HARD_WARE_VER == HW_VERSION_12)

	//�������������Դ����
	//ָʾ�ƶ���(ֻд)
	#define LED1_CS_BASE		0xb0030052		//���LED��4�ֽڣ�8λ��32����
	#define LED2_CS_BASE		0xb0030053		
	#define LED3_CS_BASE		0xb0030054		
	#define LED4_CS_BASE		0xb0030055		
	//���̶���(ֻ��)
	#define KEY1_CS_BASE		0xb0030052		//���KEY��2�ֽڣ�8λ
	#define KEY2_CS_BASE		0xb0030053		
	//����Һ��������Ӳ��������ַ
	#define LCD_DATA_REG		0xC0050001		//��д���ݼĴ���
	#define LCD_CMD_REG			0xC0050000		//��д����Ĵ���
	#define LCD_RESET_ADDR		0xC0050006		//��λLCD(0x5x)
	#define LCD_BACK_LIGHT		0xC0050007		//Һ���������(0xx5����������)

	//����ʹ�õ�ADоƬ
	#define AD_DEV_TYPE 		AD7606_DEV//AD7656_DEV////AD7656_DEV//

	#define BIA_CS_BASE 		0xb00a0050		//YXA��5�ֽڣ�8λ,���Ϊ��
	#define BIB_CS_BASE 		0xb00a00a0		//YXB��5�ֽڣ�8λ
	
	//����AD7606������ַ,�뿴�Ź�ͬһ��Ƭѡ��8λ��
	#define AD7606_BASE 	0xa2030090		//���Ź�ˢ��

#endif


//��ʱ����ϵͳ�������ȼ�
/****************************************************************************
	���ò���ϵͳ���ȼ�˳��˵��
	PSOS:		0~255�����ȼ����Խ�󼶱�Խ�ߣ�
	vxworks:	0~255�����ȼ����ԽС����Խ�ߣ�
	UCOSII:		0~63,���ȼ����ԽС����Խ��;
*****************************************************************************/
#if OS_PLATFORM == OS_VXWORKS

		//��������
#define RELAY_PRIORITY		50
#define WDOG_PRIORITY		65
		
		//���ö˿���·�ػ�����
#define MISI_PRIORITY		150
#define CAN_PRIORITY		145
#define ETHERNET_PRIORITY	146
		
#define DI_PRIORITY 		170
#define AI_PRIORITY 		180
#define BO_PRIORITY 		185

#define MAPBIT_PRIORITY 	190
#define NETPRO_PRIORITY 	191
#define REMOTE_PRIORITY		192
#define SHELL_PRIORITY		193
#define DB_PRIORITY 		199
#define LCD_PRIORITY		200
#define BACKUP_PRIORITY 	201

		//����ȱʡ�����ջ
#define TASK_STACK_LEN		(1024*10)
	
#endif	

#endif 
