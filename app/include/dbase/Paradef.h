/********************************************************************************************************/
/*																										*/
/*        		��򿭷������ɷ����޹�˾							                                    */
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��						                    */
/*																										*/
/*      Paradef.h                                     1.0					                            */
/*																										*/
/* ����																									*/
/*																										*/
/*   	���ļ�������ϵͳ���в��������ݽṹ				                                   				*/
/*																										*/
/* ����												                                   					*/
/*																										*/
/*      l.z											                                   					*/
/*																										*/
/* ��������ݽṹ									                                   					*/
/*																										*/
/*    	TFileHead							�ļ�ͷ����										    		*/
/*		TParaConfigTable					����������ñ�								         		*/
/*		TParaDispTable						�������������									       		*/
/*		TRelayDispTable						����������											   		*/
/*		TDAIPara							ģ����ֱ�Ӳ��������									   	*/
/*		TDIPara								������ֱ�Ӳ��������                                   		*/
/*		TAIPara								ң�����													*/
/*		TBIPara								ң�Ų���										            */
/*		TCIPara								��Ȳ���													*/
/*		TBOPara								ң�ز���													*/
/*		TAOPara								ң������													*/
/*		TSerialPara							���ڲ���													*/
/*		TCanBusPara							CAN������													*/
/*		TEthPortPara						��̫������		                                   			*/
/*		TSW_Disp							��ѹ������		                                   			*/
/*		TCFG_Disp							����������		                                   			*/
/*		TSET_Disp							��ֵ����		                                   			*/
/*		TSysSET_Disp						������ֵ����                                   				*/
/*		TCF_Disp							ϵ������		                                   			*/
/*		TRM_Disp							��������ֵ����		                                   		*/
/*		TRE_Disp							������������	                                   			*/
/*		TRW_Disp							�̵���������	                                   			*/
/* ����												                                   					*/
/*												                                   						*/
/*      None										                                   					*/
/*												                                   						*/
/* ͷ�ļ�˵��									                                   						*/
/*												                                   						*/
/*      Sysdefs.h                          �����������Ͷ���                                   			*/
/*											                                   							*/
/* �޸���ʷ										                                   						*/
/*												                                   						*/
/*      �޸���           �޸�����       �޸�����	                                   					*/
/*												                                   						*/
/*      l.z             2008-03-15      ��ʼ����,�汾��1.0	                                   			*/
/********************************************************************************************************/
#ifndef __PARADEFS_H
#define __PARADEFS_H
#include "system.h"	/*�������Ͷ���*/
#include "vx_file.h"

#ifdef _BIG_ENDIAN_
#define CONVERTFLOAT		_ConvertFloat
#else
#define CONVERTFLOAT		
#endif

//#define _RELAY_DATA_FILE_
#ifdef _RELAY_DATA_FILE_
#define	RLTOBS				ltobs
#define	RLTOBL				ltobl
#else
#define	RLTOBS		
#define	RLTOBL		
#endif

#define		PARACFGFILE				"paraconf.dar"
#define		RELAYDISPFILE			"relaydisp.dar"
#define		RELAYDATAFILE			"relaydata.dar"
#define		RELAYEVENTFILE			"relayevent.dat"
#define		BASICEVENTFILE			"basicevent.dat"

//��������ļ����ñ�
#define DEV_LIMIT_BO			((DWORD)(0x01<<0))    //ң������
#define DEV_LIMIT_AO			((DWORD)(0x01<<1))    //ң������
#define DEV_LIMIT_TIMING		((DWORD)(0x01<<2))    //��������
#define DEV_LIMIT_AICAL			((DWORD)(0x01<<3))    //ң���ϴ���ϵ��
#define DEV_LIMIT_CICAL			((DWORD)(0x01<<4))    //����ϴ���ϵ��
#define DEV_LIMIT_ERRREP		((DWORD)(0x01<<5))    //�쳣���������ϱ�
#define DEV_LIMIT_CITIME		((DWORD)(0x01<<6))    //��ȶ�ʱ�Ϸ���վ
#define DEV_LIMIT_MAKEMISC		((DWORD)(0x01<<7))    //�Ƿ���������¼�

#define	DEV_LIMIT_FDFIRST		((DWORD)(0x01<<8))	  //���ϵ������ն������ݴ洢
#define	DEV_LIMIT_CACLINT		((DWORD)(0x01<<9))	  //�Ƿ������ֵ��

#define HOST_NAME_LEN		16		//��������������

#define LANGUAGE_CHINESE	0
#define LANGUAGE_ENGLISH	1

typedef struct
{
	DWORD			dwDevID;
	char			chHostName[HOST_NAME_LEN];		//����FTP������̫������
	WORD			wAddress;			//���豸��ַ				
	WORD			wMainAddr;			//��λ����ַ				
	WORD			wTYPE;				//ģ������
	WORD			wCodeCrc;			//����CRC��
	char			chPassWord1[8];		//����1					
	char			chPassWord2[8];		//����2					
	DWORD			dwControl;			//������					
										//D0=1 ң������
										//D1=1 ң������
										//D2=1 ��������
										//D3=1 ң���ϴ���ϵ��
										//D4=1 ����ϴ���ϵ��
										//D5=1 �쳣���������ϱ�
										//D6=1 ��ȶ�ʱ�Ϸ���վ
										//D7=1 �Ƿ���������¼�
										//D8=1 ���ϵ�������һ��ֵ�洢
										//D9=1 �Ƿ������ֵ��
	WORD			wAIFCycle;			//��ȶ�������,����Ϊ��λ
	WORD			wCIFCycle;			//ң�ⶳ������,����Ϊ��λ
//	WORD			nAIType;			//ң��������	
	WORD			nDI;				//ֱ��ģ����������		
	WORD			nDAI;				//ֱ�����ֲ��������		
	WORD			nAI;				//ң�����				
	WORD			nBI;				//ң�Ÿ���				
	WORD			nDBI;				//˫��ң�Ÿ���			
	WORD			nCI;				//��ȸ���				
	WORD			nSPI;				//SPI����					
	WORD			nBCD;				//BCD����					
	WORD			nBO;				//ң�ظ���				
	WORD			nAO;				//ң������				
	WORD			nSerNum;			//���ڸ���				
	WORD			nCanNum;			//CAN���˿ڸ���			
	WORD			nEthNum;			//��̫���˿ڸ���			
	WORD			nLanguageNo;		//���Ա��
	WORD			Rsv[12];			//����,Ԥ��12����
}TParaConfigTable;

//������������ļ����ñ�
typedef struct
{
	WORD			nAI;				//ң�����	
	WORD			nBI;				//ң�Ÿ���	
	WORD			nCI;				//��ȸ���	
	WORD			nBO;				//ң�ظ���	
	WORD			nAO;				//ң������	
	WORD			Rsv[11];			//����		
}TParaDispTable;

//���������ļ�Ŀ¼
typedef struct
{
	WORD			nSW;				//��ѹ�����		
	WORD			nCFG;				//�����ָ���		
	WORD			nSET;				//��ֵ����		
	WORD			nSETArea;			//��ֵ������		
	WORD			nSystemSET;			//������ֵ����	
	WORD			nRT;				//��ȸ���		
	WORD			nCF;				//ϵ������		
	WORD			nRM;				//��������ֵ����	
	WORD			nRE;				//������������	
	WORD			nRW;				//�̵����ָ���	
	WORD			nDataName;			//���������ܸ���	
	WORD			nTC;				//����
	WORD			nMBO;				//����λ�������
	WORD			nFL;				//������
	WORD			Rsv[2];				//����,Ԥ��2���ֽ�
}TRelayDispTable;

//���������ļ�Ŀ¼
typedef struct
{
	WORD			nSW;				//��ѹ�����			
	WORD			nCFG;				//�����ָ���			
	WORD			nSET;				//��ֵ����			
	WORD			nSETArea;			//��ֵ������			
	WORD			nSETAreaCur;		//��ǰ��ֵ����		
	WORD			nSETAreaCurB;		//��ǰ��ֵ���ŷ���	
	WORD			nSystemSET;			//������ֵ����		
	WORD			nRT;				//��ȸ���			
	WORD			nCF;				//ϵ������			
	WORD			Rsv[7];			//����,Ԥ��14���ֽ�
}TRelayDataTable;

//ֱ��ģ�������������
#define DAI_ATTR_DC				((WORD)(0x01<<0))    //ֱ����·
#define DAI_ATTR_3J				((WORD)(0x01<<1))    //����
#define DAI_ATTR_LINE			((WORD)(0x01<<2))    //�߽���
#define DAI_ATTR_PUBPT			((WORD)(0x01<<3))    //��PT
#define DAI_ATTR_SWTPT			((WORD)(0x01<<4))    //PT�л�
#define DAI_ATTR_DCD			((WORD)(0x01<<5))    //ֱ��˫������
#define DAI_ATTR_DC2			((WORD)(0x01<<6))    //˫�˵ڶ���
#define DAI_ATTR_PLUSMASK		((WORD)(0x03<<7))    //����

typedef struct
{
	WORD			wLineNo;			//��·��,������·���
	WORD			wClass;				//������	
										//D0��	=0����Ϊ����������·
										//		=1����Ϊֱ��ֱ�Ӳ�����
										//D1��	=0�û�·���������ɼ�
										//		=1�û�·ʹ�������ɼ�
										//D2��	=0����ʱ���������
										//		=1����ʱ�����߽���
										//D3��	=0�û�·��ʹ�ù�PT��ʽ
										//		=1�û�·ʹ�ù�PT��ʽ
										//D4��	=0��PTʱ������PT�л�
										//		=1��PTʱ����PT�л�
										//D5��	=0ֱ������������
										//		=1ֱ����˫������
										//D6��	=0˫�˵�һ��
										//		=1˫�˵ڶ���
										//D7~8��=0��0~20ma
										//		=1��4~20ma����������
										//D9~D15������
}TDAIPara;

//ֱ�����������������
#define DI_ATTR_TYPEMASK		((WORD)(0x0F<<0))		//��������
#define DI_ATTR_SEQ				((WORD)(0x01<<8))		//�������
#define DI_TYPE_INVALID			0						//��Ч
#define DI_TYPE_SP				1						//����ң��
#define DI_TYPE_DP				2						//˫��ң��
#define DI_TYPE_SPI				3						//SPI
#define DI_TYPE_BCD				4						//BCD
#define DI_TYPE_PLUS			5						//����
#define DI_TYPE_INNER			6						//�ڲ�ң��
#define DI_TYPE_VIR				7						//��ң��

typedef struct
{
	WORD			wClass;				//������,D0~3 ���ͣ�
										//			=0 ��Ч
										//			=1 ����ң��
										//			=2 ˫��ң��
										//			=3 SPI ��Ϣ ���ѹ���ֽ�ͷλ��
										//			=4 BCD����Ϣ����ˮλ��Ϣ
										//			=5 ��������*/
										//			=6 �ڲ�ң��*/
										//			=7 ��ң��*/
										//			=8~15 Ԥ��*/
										//		D4~7 ����
										//		D8~15��ţ�
										//			=0˫��ң�ŵĵ�һ��
										//			!=0˫��ң�ŵڶ����������
	WORD			wFilterTime;		//ȥ��ʱ��,�Ժ���Ϊ��λ
//	WORD			wMap;				//��Ӧ��ң�Ż��ߵ�ȵĵ��
	BYTE			bInDINo;			//�ڲ�ң�ŵİ������,��0��ʼ,��һ·�ڲ�ң��Ϊ�źŸ���
	BYTE			bInDIBoardNo;		//�ڲ�ң�����ڵİ��,��0��ʼ
	WORD			wRsv;				//Ԥ��
}TDIPara;

//ң�����
typedef struct
{
	WORD			wNameIndex;			//����	������
	char			strSymbol[8];		//���			
	char			strFirUnit1[6];		//һ������1�ε�λ		
	char			strFirUnit2[6];		//һ������2�ε�λ		
	char			strFirUnit3[6];		//һ������3�ε�λ
	char			strSecUnit[6];		//���ε�λ
	
	BYTE			nFirDec1;			//һ������1��С��λ��	
	BYTE			nFirDec2;			//һ������2��С��λ��	
	BYTE			nFirDec3;			//һ������3��С��λ��	
	BYTE			nDataRate2;			//2// 2�ν���
	BYTE			nDataRate3;			// 3�ν���
	BYTE			nSecDec;			//����С��λ��

	WORD			wRatioIndex;		//���	������
	WORD			wControl;			//������D0=1 ������Ч��
										//		D1=1������Խ�޼�飻   
										//		D2=1����Խ�޼�飻
										//		D3=1����Խ�޼�飻
										//		D4=1������Խ�޼��
										//		D5=1 ģ���ϵ��
										//		D6=1 �������ϵ��
	short			a;					//����1	Y=a*x/b+c
	short			b;					//����2	
	short			c;					//����3	
	WORD			wDeadLine;			//����	D15=1 ���ֵ����λΪǧ�֣���D15=0������ֵ
	long			lHiUpLimit;			//������	
	long			lUpLimit;			//����	
	long			lDownLimit;			//����	
	long			lLowDownLimit;		//������

	long			lDispLimit12;		//��ʾ�ֶν���1,2
	long			lDispLimit23;		//��ʾ�ֶν���2,3
	
	WORD			nCFIndex;			//ϵ��������
	
	WORD			wRsv[3];			//Ԥ��
}TAIPara;

//ң�Ų���
#define BI_ATTR_NTPEVENT		((WORD)(0x01<<0))		//��������ʱ����¼�����
#define BI_ATTR_WTPEVENT		((WORD)(0x01<<1))		//������ʱ����¼�����
#define BI_ATTR_CONVERT			((WORD)(0x01<<2))		//����ȡ������
#define BI_ATTR_DEFAULT			((WORD)(0x01<<3))		//�����ж��¹���ң��
#define BI_ATTR_VIRTUAL			((WORD)(0x01<<4))		//������ң��

typedef struct
{
	char			strSymbol[8];		//���					
	WORD			wNameIndex;			//����					
	WORD			wControl;			//������
										//	D0	1��ģ���������ʱ����¼�����
										//	D1	1��ģ�������ʱ����¼�����
										//	D2	1��ģ�����ȡ������
										//	D3	1��������ж��¹���ң��
										//	D4	1�����¹���ң��
										//	D5	1��������ң��
										//	D6~D7	����
										//	D8	1���������������ʱ����¼�����
										//	D9	1�������������ʱ����¼�����
										//	D10	1�����������ȡ������
										//	D11	1������������Զ�����
										//	D12~D15	����
	WORD			wMaptoDINo;			//	ӳ�䵽DI�����
}TBIPara;

//��Ȳ���
#define CI_ATTR_PULSE			((WORD)(0x01<<0))		//������
#define CI_ATTR_FRZENWITHTP		((WORD)(0x01<<1))		//������ʱ��Ķ�������
#define CI_ATTR_CYCLESHIFT		3						//��������ƫ��
#define CI_ATTR_CYCLEMASK		((WORD)(0x07<<3))		//�������ڣ�5����Ϊ��λ

typedef struct
{
	char			strSymbol[8];		//���						
	char			strUnit[8];			//��ȵ�λ		
	WORD			wNameIndex;			//����,������
	WORD			wControl;			//������	D0��=1�����ȣ�=0���ֵ��
										//			D1��=1������ʱ��Ķ������ݣ�
										//			D3~D5���������ڣ�5����Ϊ��λ��
	DWORD			dwConst;			//���峣��				
	short			a;					//����1	,	Y=a*x/b
	short			b;					//����2	
	WORD			wMaptoDINo;			//	ӳ�䵽DI�����
	WORD			wRsv;
}TCIPara;

//ң�ز���
#define BO_ATTR_VIRTUAL			((DWORD)(0x01<<31))			//��ң��
#define BO_ATTR_TYPESHIFT		24							//��ң�����λ��
#define BO_ATTR_TYPEMASK		((DWORD)(0x7F<<24))			//��ң���������
#define BO_ATTR_OPTSHIFT		0							//ң������λ��
#define BO_ATTR_OPTMASK			((DWORD)(0x03<<0))			//ң����������
#define BO_GET_OPT(a)			(((a)&BO_ATTR_OPTMASK)>>BO_ATTR_OPTSHIFT)

typedef struct
{
	char			strSymbol[8];		//���
	DWORD			dwControl;			//������    D31��=1��ң�أ�=0ʵң��
										//			D24~D30����ң�����
										//			D0~D1��ң�����ԣ���/��/NULL����
	WORD			wNameIndex;			//����,������
	WORD			wSwitchNo;			//���غ�,��Ӧ�ļ̵����Ż�����
	WORD			wRelayNo;			//�̵�����ƫ��,������ң����Ч
	WORD			wLastTime;			//��������ʱ��,��λ��ms
}TBOPara;

//ң������
typedef struct
{
	char			strSymbol[8];		//���		
	WORD			wNameIndex;			//����	������
	WORD			wControl;			//������D0~D2��	
										//		=0:0~10V;
										//		=1;0~20mA; 
										//		=2:4~20mA;
	WORD			nID;				//������	
	short			a;					//����1	
	short			b;					//����2	
	short			c;					//����3	
}TAOPara;

#define MAJOR_PORT(port) 	(HIBYTE(port))
#define MINOR_PORT(port) 	(LOBYTE(port))

/*-----------------------------------------------------------------------------------------*/
/* �˿����Ͷ���,�������˿�����                                                             */
/*-----------------------------------------------------------------------------------------*/
#define SC_PORT_SERIAL          0x01                      /*����*/
#define SC_PORT_CANBUS          0x02                      /*CAN-BUS*/
#define SC_PORT_ETHERNETA       0x03                      /*��̫��A*/
#define SC_PORT_ETHERNETB       0x04                      /*��̫��B*/
#define SC_PORT_GPRS        	0x05                      /*CAN-BUS*/
#define SC_PORT_VIRTUAL         0x06                      /*����˿�*/
#define SC_PORT_SYSTEM         	0x07                      /*����˿�*/

//���ڲ���

#define SERIAL_ATTR_DATBITSHIFT			0		//����λ��ʼƫ��
#define SERIAL_ATTR_CHKBITSHIFT			2		//У��λ��ʼƫ��
#define SERIAL_ATTR_CTLBITSHIFT			5		//У��λ��ʼƫ��

#define SERIAL_ATTR_DATBITMASK			0x03	//����λ����
#define SERIAL_ATTR_CHKBITMASK			0x0C	//У��λ����
#define SERIAL_ATTR_STPBITMASK			0x10	//ֹͣλ����
#define SERIAL_ATTR_CTLBITMASK			0x60	//������λ����

#define SERIAL_ATTR_RS485				0x0080	//485ͨ�ŷ�ʽ
#define SERIAL_ATTR_RTSCTR				0x0100	//RTS�������
#define SERIAL_ATTR_RTSHIGH				0x0200	//RTS��Чλ
#define SERIAL_ATTR_RTSDELAY			0x0400	//RTS��ʱ	


#define	GET_SERIAL_DATBIT(F)			(((F)&SERIAL_ATTR_DATBITMASK)>>SERIAL_ATTR_DATBITSHIFT);
#define	GET_SERIAL_CHKBIT(F)			(((F)&SERIAL_ATTR_CHKBITMASK)>>SERIAL_ATTR_CHKBITSHIFT);
#define	GET_SERIAL_CTLBIT(F)			(((F)&SERIAL_ATTR_CTLBITMASK)>>SERIAL_ATTR_CTLBITSHIFT);

#define SERIAL_DATBIT_8					0
#define SERIAL_DATBIT_7					1
#define SERIAL_DATBIT_6					2

#define SERIAL_CHKBIT_NONE				0
#define SERIAL_CHKBIT_ODD				1
#define SERIAL_CHKBIT_EVEN				2

#define SERIAL_CTLBIT_NONE				0
#define SERIAL_CTLBIT_RTS				1
#define SERIAL_CTLBIT_DTR				2

typedef struct
{
	WORD			wPortID;			//�˿ڱ�ʶ,��Ӧ�Ķ˿�ID��,����+���
	WORD			bFlag;				//�˿ڱ�־λ,���ǵ�����Ӧ�ã���ʱ������ͬ����ʽ
										//	D0~D1 ����λ	=0 8λ����λ�� =1 7λ���ݣ�	=2 6λ���ݣ�   =3 5λ����
										//	D2~D3У��λ	    =0��У�飬=1 żУ�飬=2 ��У��
										//	D4 ֹͣλ		=0 1λֹͣλ��=1 2λֹͣλ
										//	D5~D6 ������λ	=0�ޣ�=1RTS��=2DTR
										//	D7 ͨ�ŷ�ʽ		=0 RS232 ��=1 RS485
										//	D8 RTS�������	=0 ����ҪRTS�������;=1 ��ҪRTS�������
										//	D9 RTS��Чλ	=0 �߼�����Ч;=1 �߼�����Ч
										//	D10	RTS��ʱ		=0 RTS����Ҫ��ʱ;=1 RTS����Ҫ��ʱ
										//  ����λ����
										
	BYTE			byBaud;				//	������	0--NoUse,	1--150,		2--300,		3--600,		4--1200,
										//			5-240,		6--4800,	7--9600,	8-19200,	9-38400
	BYTE			byDriver;			//��Լ��,�ö˿ڶ�Ӧ�Ĺ�Լ���
	BYTE			byRTSOnDelay;		//RTS����ʱ
	BYTE			byRTSOffDelay;		//RTS�ر���ʱ
	BYTE			byRsv[4];			//����				
}TSerialPara;

//CAN������
typedef struct
{
	WORD			wPortID;			//�˿ڱ�ʶ,��Ӧ�Ķ˿�ID��,����+���
	WORD			wBaud;				//������,ͨ�����ʣ���KΪ��λ
	DWORD			dwMask;				//��������,�ö˿ڶ�Ӧ����������,����˫�������ȿ���
	BYTE			byDriver;			//��Լ��,�ö˿ڶ�Ӧ�Ĺ�Լ���
	BYTE			byRsv[7];			//����
}TCanBusPara;

#define	ETHLINK_MAXLEN_USERNAME			16				
#define	ETHLINK_MAXLEN_PASSWORD			16				
#define ETHLINK_MODE_CLIENT				((DWORD)(0x01<<0))			//�ͻ���
#define ETHLINK_MODE_UDP				((DWORD)(0x01<<1))			//UDP����
#define ETHLINK_MODE_BACKSEND			((DWORD)(0x01<<31))			//��������

//��̫�����Ӳ���
typedef struct
{
	WORD			wPortID;			//�˿ڱ�ʶ,		��Ӧ�Ķ˿�ID��,����+���
	WORD			wDriver;			//��Լ��,		�ö˿ڶ�Ӧ�Ĺ�Լ���
	DWORD			dwMask;				//��������,		�ö˿ڶ�Ӧ���������룬����˫�������ȿ��ơ�
	DWORD			dwSideIP;			//�Է�IP��ַ		
	DWORD			dwSideMask;			//�Է�IP��������	
	DWORD			dwSidePort;			//�Է�IP�˿�		
	DWORD			dwSelfIP;			//�Լ�IP��ַ		
	DWORD			dwSelfMask;			//�Լ�IP��������	
	DWORD			dwSelfPort;			//�Լ�IP�˿�		
	DWORD			dwMode;				//ͨ��ģʽ	D0	=1,��Ϊ�ͻ���,=0��Ϊ��������
										//			D1	=1,ʹ��UDP��ʽ,=0ʹ��TCP��ʽ		
										//			D31	=0 ����״̬��ֹ����,=1����״̬������
	DWORD			dwGateWay1;			//��һ·����IP	
	DWORD			dwGateWay2;			//�ڶ�·����IP
	DWORD			dwDHCPIP;			//DHCP����IP
	char			chUser1[ETHLINK_MAXLEN_USERNAME];		//��¼�û���		
	char			chPassword1[ETHLINK_MAXLEN_PASSWORD];	//��½����		
	char			chUser2[ETHLINK_MAXLEN_USERNAME];		//��¼�û���		
	char			chPassword2[ETHLINK_MAXLEN_PASSWORD];	//��½����		
	char			chUser3[ETHLINK_MAXLEN_USERNAME];		//��¼�û���		
	char			chPassword3[ETHLINK_MAXLEN_PASSWORD];	//��½����		
	char			chUser4[ETHLINK_MAXLEN_USERNAME];		//��¼�û���		
	char			chPassword4[ETHLINK_MAXLEN_PASSWORD];	//��½����		
	DWORD			dwRsv[4];			//����			
}TEthPortPara;

//������ѹ������
typedef struct
{
	char			strCHName[20];		//��������		����Һ����ʾ��ά������鿴
}TSW_Disp;

//��������������
typedef struct
{
	char			strCHName[20];		//��������		����Һ����ʾ��ά������鿴
}TCFG_Disp;

//������ֵ����
#define SETCFG_INVALID		0xFF			//��Ч�Ĺ������ú�
#define SETNAME_INVALID		0xFFFF			//��Ч�Ķ�ֵ��������
#define SETNAME_MAXLENGTH	20				//ÿ����ֵ��������󳤶�
#define SETCFG_MAXNUM		32				//���ֵ���������ָ���
typedef struct
{
	char			strCHName[20];		//��������		����Һ����ʾ��ά������鿴
	char			strSymbol[16];		//���				
	float			fMaxValue;			//���ֵ			
	float			fMinValue;			//��Сֵ			
	char			strUnit[8];			//��λ				
	WORD			nCFG;				//���������ָ���
	WORD			nItemStart;			//��ֵ������Ŀ��ʼ
	WORD			nItemNum;			//��ֵ������Ŀ����
	BYTE			CFGList[SETCFG_MAXNUM];//�����ֱ�
	WORD			nDec;				//С����λ��		Һ����ʾ��
	WORD			nCFIndex;			//ϵ��������
	WORD			wRsv[3];			//����
}TSET_Disp;

//����������ֵ����
typedef struct
{
	char			strCHName[20];		//��������		����Һ����ʾ��ά������鿴
	char			strSymbol[16];		//���				
	float			fMaxValue;			//���ֵ			
	float			fMinValue;			//��Сֵ			
	char			strUnit[8];			//��λ				
	WORD			nItemStart;			//��ֵ������Ŀ��ʼ
	WORD			nItemNum;			//��ֵ������Ŀ����
	BYTE			nDec;				//С����λ��		Һ����ʾ��
	BYTE			nCFIndex;			//ϵ��������
	BYTE			byRsv[6];
}TSysSET_Disp;

//����ϵ������
typedef struct
{
	char			strSymbol[8];		//���			
	WORD			wCof;				//�Ŵ���		
	WORD			wRsv;
}TCF_Disp;

//��������ֵ����
#define	RT_INDEX_INVALID					0xFFFF
#define	RM_ATTR_HEX							0x0001
typedef struct
{
	char			strSymbol[8];		//���			
	char			strFirUnit[6];		//һ�ε�λ		
	char			strSecUnit[6];		//���ε�λ		
	BYTE			nFirDec;			//һ��С��λ��	
	BYTE			nSecDec;			//����С��λ��
	WORD			nIndex;				//����������,ȫ�������ţ����ϵ���ȫ�ִ������������ֵ�������µ�װ����
	WORD			nRTIndex;			//���������,0xFFFF�����������
	WORD			wAttr;				//��ֵ����,10���ƻ���16���Ƶ�
	WORD			nCFIndex;			//ϵ��������
	BYTE			nFirRate;			//һ�����ݽ���
	BYTE			byRsv[5];			//����
}TRM_Disp;

#define		MAX_FD_NUM						48			//�������������ϵ�������

//������������
typedef struct
{
	char			strCHName[20];		//��������		����Һ����ʾ��ά������鿴
	WORD			wInf;					//	���������� ȫ��������
	WORD			nFD;					//	���ϵ�������		
//	WORD			wFDList[MAX_FD_NUM];	//	���ϵ���������
	WORD			wRMList[MAX_FD_NUM];	//	��������ֵ������ ����LED��ȡ��������ֵ������Ϣ
}TRE_Disp;

#define DIST_CAPACITY_MAX					16
#define DIST_ANALOGCHN_MAX					24
#define DIST_DIGITALCHN_MAX					96
#define DIST_SAMPLENUM_MAX					8000

//¼����������
typedef struct
{
	char 	strDevName[64];					//	װ������		
	WORD	wSampleRate; 					//	����Ƶ��		
	WORD	wSysFrequency;					//	ϵͳƵ��
	DWORD	dwTotalSampleNum;				//	ÿ��¼����������
	DWORD	dwPrevSampleNum;				//	����ǰ��������
	WORD	wRecordCapacity; 				//	��¼������
	WORD	wAnalogChannelNum;				//	ģ����ͨ������
	WORD	wDigitalChannelNum;				//	����ͨ������
	WORD	wRvs;
}TDist_Disp;

//ģ��ͨ������
typedef struct
{
	char	strName[16];					//	ͨ������
	char	strUnit[8];						//	���ݵ�λ
	WORD	wIndex;							//	ͨ������
	WORD	wCoefIndex;						//	ϵ������
	WORD	wCompressMultiple;				//	ѹ������
	WORD	wRTIndex;						//	ϵ������
	float	fCFRatio;						//  ϵ������
	BYTE	byRsv[8];						//	����	
}TAC_Disp;

//����ͨ������
typedef struct
{
	char	strName[8];						//ͨ������	
	WORD	wRelayMacro;					//�̵����ֺ�
	WORD	wRsv;							//Ԥ����	
}TDC_Disp;

//��������
typedef struct
{
	char			strCHName[20];		//��������		����Һ����ʾ��ά������鿴
}TTC_Disp;

//����λ����
#define MAX_MAPBITOBJ_NUM			32

#define MAPBIT_LISTEN_PORT			6500	//����λ�˿�
#define MAPBIT_PORTTYPE_ETHA				0			
#define MAPBIT_PORTTYPE_ETHB				1			
#define MAPBIT_PORTTYPE_CANA				2			
#define MAPBIT_PORTTYPE_CANB				3			
#define MAPBIT_PORTTYPE_SERIAL1				4			
#define MAPBIT_PORTTYPE_SERIAL2				5			
#define MAPBIT_PORTTYPE_SERIAL3				6			

typedef struct
{
	char			strStationName[20];	//վ��
	WORD			wControl;			//������
										//D0~D3 0:ETHA��	1:ETHB��	2:CAN1; 	
										//		3:CAN2; 	4:Serial1	5:Serial2	6:Serial3; 	��������   
										//D15   0:��Ч;  1:��Ч 	
										//��������
										
	BYTE			bySelfAddr;			//����վַ
	BYTE			bySideAddr;			//�Զ�վַ
	DWORD			dwSideIP;			//�Զ�IP��ַ
	DWORD			dwBaud;				//������
	BYTE			bySMBStart;			//���;���λ��ʼλ��
	BYTE			bySMBNum;			//���;���λ����
	BYTE			byRMBStart;			//���վ���λ��ʼλ��
	BYTE			byRMBNum;			//���վ���λ����
 	DWORD			dwRsv;
}TMBO_Disp;


#define FL_CTRLMASK_STEPOVERVAL	0x8000
#define FL_CTRLMASK_SIDE		0x0080
#define FL_CTRLMASK_SUBNO		0x007F

typedef struct {
		WORD	wControl;		//D15��=1��Խ��ʱͶ�룻=0��Խ��ʱ�˳� 
								//D12~D14: ���ӷ�ʽ
								// 0=��̫��
								// 1=����2
								// 2=·��
								// ��������
								// D8: 1,�Բ࣬0������    (Ӧ����D7)
								// D0~D3: �������,1,����AT��2����SP��3�Բ�SP��4�Բ�AT��5���Բ�SS
								//����λ����
		BYTE	byNetAddr;		//����ͨ�ŵ�ַ
		BYTE	byFLNo;			//�����
		DWORD 	dwMNetIP;		//����IP��ַ
		DWORD 	dwSNetIP;		//����IP��ַ
		DWORD	dwRsv;			//����
}TFL_Disp;

#endif
