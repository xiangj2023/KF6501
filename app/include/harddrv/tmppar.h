#ifndef _TMPPAR_H_
#define _TMPPAR_H_

#include "system.h"

//���ļ�Ϊ������ʱ����ṹ

//��ʱ����
/*
#define SM_WRITECONT	10
#define SM_READCOMM		11
#define SM_COMMIDLE		12
#define SM_COMMTXDIDLE	13
#define SM_WRITECOMM	14
*/
#define RCF_FRAME_TAIL		(DWORD)(1<<31)

/*����ͨ�Ž��ʶ���*/
#define TRANSPARENT 0x00	/* ͸������				*/
#define RADIO       0x01	/* ���ߵ�̨				*/
#define SPTELEPHONE 0x02	/* ר�ߵ绰				*/
#define PBTELEPHONE 0x03	/* ���õ绰				*/
#define PPFIBER     0x04	/* ������				*/
#define NOSCARFIBER 0x05	/* ���������ܵĻ�������	*/
#define SCARFIBER   0x06	/* ���������ܵĻ�������	*/
#define SPMODEM     0x07	/* ר��MODEM			*/
#define FIBERMODEM  0x08	/* ����MODEM			*/
#define INFRARED	0x09	/* ����˿�				*/
#define AUTO_485	0x0a	/* 485��ʽRTS����Ч 	*/
#define GPRS		0x0b	//gprs

//��ʱ���崮���йؽṹ
#define MAJOR_PORT(port) (HIWORD(port))
#define MINOR_PORT(port) (LOWORD(port))

/*======================================================================
* ����˿ڿ��ƹ�����                                                                     
*======================================================================
*/
typedef enum{
    FC_GET=1,                           /*ȡ�˿���Ӧ����*/
    FC_SET=2,                           /*д�˿ڲ���*/
    FC_OPERATOR=3,                      /*���п��Ʋ���*/
    FC_GETSTATUS=4                      /*ȡ�˿�ʵʱ����״̬*/
}TCtlCommFuncCode;

/*======================================================================
* �˿ڿ��ƽṹ                                                                          
*======================================================================
*/

typedef struct  {
    DWORD       dwCtlMask;              /*���ƶ˿�������*/

    TCtlCommFuncCode dwFuncCode;        /*���ƹ�����*/

    WORD        dwNotifyWrite;          /*�����ͻ���������С��dwNotifyWriteʱ���ϲ��Լ*/
                                        /*������SM_WRITECOMM��Ϣ,ָʾ��Լ������Լ�����*/
                                        /*�ͣ�dwNotifyWrite=0ʱ��ֹSM_WRITECOMM*/
    WORD        dwNotifyRead;           /*�����ջ��������ȴ���dwNotifyReadʱ���ϲ��Լ*/
                                        /*������SM_READCOMM��Ϣ,ָʾ��Լ������˿ڣ�*/
                                        /*dwNotifyRead=0ʱ��ֹSM_READCOMM*/
    WORD        dwNotifyRxdBreak;       /*�ڽ�������ʱ,������ϴν��յ��ַ���ʱ��,��dwNotifyRxdBreak*/
                                        /*δ�����µ��ַ�,�����Լ������SM_COMMRXDIDLE��Ϣ*/   
                                        /*dwNotifyRxdBreak=0ʱ,��ֹ����SM_COMMRXDIDLE��Ϣ*/
    WORD        dwNotifyTxdBreak;       /*�ڷ�������ʱ,������ϴη����ַ���ʱ��,��dwNotifyTxdBreak*/
                                        /*δ�����µ��ַ�,�����Լ������SM_COMMTXDIDLE��Ϣ*/   
                                        /*dwNotifyTxdBreak=0ʱ,��ֹ����SM_COMMTXDIDLE��Ϣ*/
    WORD        dwNotifyCommBreak;      /*�˿���dwNotifyCommBreakʱ����,û�з���,Ҳû�н��ղ���*/
                                        /*ʱ,���ϲ�������SM_COMMIDLE��Ϣ;dwNotifyCommBreak=0*/
                                        /*���ֹ����Ϣ�ķ���*/
    WORD        dwBaud;                 /*�˿ڲ�����*/

    WORD        wSync;                  /*ͬ����ʽʱ��ͬ��ͨ���֣�=0xFFFF�ö˿�Ϊ�첽����ģʽ*/

    WORD        wFrameDelay;            /*���͵�֡���*/

    void*       pPara;
}TCtlComm;

//���ڿڲ���
typedef struct {
	WORD		wPortID;	/*��Ӧ�Ķ˿�ID��,����+���*/
	WORD		byFlag;	    /*���ǵ�����Ӧ�ã���ʱ������ͬ����ʽ*/
							/*D0~D1 ����λ��=0 8λ����λ��=1 7λ���ݣ�=2 6λ���ݣ�=3 5λ����*/
	                        /*D2~D3У��λ��=0��У�飬=1 żУ�飬=2 ��У��*/
	                        /*D4 ֹͣλ��=0 1λֹͣλ��=1 2λֹͣλ*/
	BYTE		byBaud;		/*������,0--No Use,1--150,2--300,3--600,4--1200,5--2400*/
	                        /*6--4800,7--9600,8--19200,9--38400,10--56000,11--64000*/
							
	BYTE		byDriver;	/*�ö˿ڶ�Ӧ�Ĺ�Լ��*/
	
	//��������Ϊ2004��Լ�ն�ͨ�Ų���
	BYTE 		byRTSDelay;			//rts��ʱʱ�䣬��̨ʱʹ��
	BYTE		nRetry;				//�������Դ���
	BYTE    	byValidInterval;	//����ʱЧ�������λΪ����
	BYTE    	bySafetyInterval;	//ƽ�����ļ������λΪ����
	BYTE		byConfirm;  		//��Ҫ��վȷ�ϵı�־��D0=1 ��Ҫ�¼���Ҫȷ�ϣ�D1=1һ���¼���Ҫȷ��
	//BYTE		byAttr;				//D0:1 ��Ҫ���� 0 �������
	WORD    	wTimeOut;			//���ʱʱ�䣬������ʱ�䣬����վ����Ϊ��ʱ����
	BYTE 		bMedium;			//�Ƿ�Modem
	BYTE		rsv[3];				//�����ֽ�
	DWORD		dwRsv[5];			//���ó���		
}TSerialP;

//���崮������λ����ӦTSerialP��wFlag��D0~D1
#define DATA_BIT_MASK		(0x03<<0)
#define DATA_BIT_8			(0x00<<0)
#define DATA_BIT_7			(0x01<<0)
#define DATA_BIT_6			(0x02<<0)
#define DATA_BIT_5			(0x03<<0)

//У��λ��־���壬��ӦD2~D3
#define PARITY_BIT_MASK		(0x03<<2)
#define NO_PARITY			(0x00<<2)
#define EVEN_PARITY			(0x01<<2)
#define ODD_PARITY			(0x02<<2)

//ֹͣλ��־���壬��ӦD4
#define STOP_BIT_MASK		(0x01<<4)
#define STOP_BIT_1			(0x00<<4)
#define STOP_BIT_2			(0x01<<4)

//��ʱ�������

#endif
