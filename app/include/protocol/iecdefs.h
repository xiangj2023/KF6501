//=======================================================================================
// ����: Siec104.h
// ����: IEC104��վ��Լ��Ķ���
// ��д��lz 2011-12-23
// ��д��
//=======================================================================================

#ifndef _IECDEFS_H
#define _IECDEFS_H

#include "sysdefs.h"	/*ϵͳ�������Ͷ��弰���ú궨��*/

/*---------------------�궨����-----------------------*/
#define		PBO_SELECT			1		//ң��ѡ��
#define	 	PBO_EXECUTE			2		//ң��ִ��
#define	  	PBO_CANCEL 			3		//ң�س���
#define		PBO_DIRECT			4		//ң��ֱ��ִ��
#define		PBO_SUCESS  		0		//ң�سɹ�
#define		PBO_FAIL			1		//ң��ʧ��
				
#define     PBO_NULL			0		//����
#define 	PBO_OPEN			1		//�ط�
#define 	PBO_CLOSE			2		//�غ�
	
#define		PBOINFOSIZE			4		//ң�ر�����Ϣ�ֽ���

//240-255,ͨ�÷��๦��
#define INF_S_GEN_RGROUPITEM   240   //�����б�������ı���
#define INF_S_GEN_RGROUPVALUE  241   //��һ�����ȫ����Ŀ��ֵ������
#define INF_S_GEN_RENTRYITEM   243   //��������Ŀ��Ŀ¼
#define INF_S_GEN_RENTRYVALUE  244   //��������Ŀ��ֵ������
#define INF_S_GEN_CALLALL      245   //��ͨ�÷������ݵ��ܲ�ѯ
#define INF_S_GEN_WENTRY       248   //д��Ŀ
#define INF_S_GEN_WENTRYACK    249   //��ȷ�ϵ�д��Ŀ
#define INF_S_GEN_WENTRYEXEC   250   //��ִ�е�д��Ŀ
#define INF_S_GEN_WENTRYCANCEL 251   //д��Ŀ��ֹ

//240-255,ͨ�÷��๦��
#define INF_M_GEN_RGROUPITEM   240   //�����б�������ı���
#define INF_M_GEN_RGROUPVALUE  241   //��һ�����ȫ����Ŀ��ֵ������
#define INF_M_GEN_RENTRYITEM   243   //��������Ŀ��Ŀ¼
#define INF_M_GEN_RENTRYVALUE  244   //��������Ŀ��ֵ������
#define INF_M_GEN_CALLALL      245   //��ͨ�÷������ݵ��ܲ�ѯ
#define INF_M_GEN_WENTRY       248   //д��Ŀ
#define INF_M_GEN_WENTRYACK    249   //��ȷ�ϵ�д��Ŀ
#define INF_M_GEN_WENTRYEXEC   250   //��ִ�е�д��Ŀ
#define INF_M_GEN_WENTRYCANCEL 251   //д��Ŀ��ֹ

//GDD�е���������  ��ֵ��
#define GDD_DATATYP_NULL	0	//������
#define GDD_DATATYP_OS8		1	//ASCII8λ��
#define GDD_DATATYP_UI		3	//�޷�������
#define GDD_DATATYP_I		4	//���� 
#define GDD_DATATYP_UF		5	//�޷��Ÿ�����
#define GDD_DATATYP_F		6	//������
#define GDD_DATATYP_R32_23	7	//IEEE��׼754��ʵ��

//==== ͨ�÷������ݶ��� ====

//���������(KOD)
#define KOD_0                  0     //����ָ�����������
#define KOD_1                  1     //ʵ��ֵ
#define KOD_2                  2     //ȱʡֵ
#define KOD_3                  3     //����
#define KOD_5                  5     //����
#define KOD_6                  6     //����
#define KOD_7                  7     //�α�
#define KOD_8                  8     //�б�
#define KOD_9                  9     //����
#define KOD_10                 10    //����
#define KOD_12                 12    //������Ŀ
#define KOD_19                 19    //��Ӧ�Ĺ������ͺ���Ϣ���
#define KOD_23                 23    //���������Ŀ

//��������
#define DAT_1                  1     //ASCII8λ��
#define DAT_2                  2     //����8λ��
#define DAT_3                  3     //�޷�������
#define DAT_4                  4     //����
#define DAT_5                  5     //�޷��Ÿ�����
#define DAT_6                  6     //������
#define DAT_7                  7     //IEEE754��ʵ��
#define DAT_8                  8     //IEEE754ʵ��
#define DAT_9                  9     //˫����Ϣ
#define DAT_10                 10    //������Ϣ
#define DAT_12                 12    //��Ʒ�������ı���ֵ
#define DAT_14                 14    //������ʱ��
#define DAT_15                 15    //ͨ�÷����ʶ���
#define DAT_16                 16    //���ʱ��
#define DAT_17                 17    //�������ͺ���Ϣ���
#define DAT_18                 18    //��ʱ��ı���
#define DAT_19                 19    //�����ʱ��ı���
#define DAT_20                 20    //�����ʱ��ı���ֵ
#define DAT_21                 21    //�ⲿ�ı����
#define DAT_22                 22    //ͨ�÷���ش���
#define DAT_23                 23    //���ݽṹ
#define DAT_24                 24    //����

//********************	�ṹ������	**********************

struct TBO{
	WORD Flag;		//����״̬	PBO_NEW, PBO_BUSY, PBO_OK, PBO_NUL
	WORD No;		//ң��·��
	WORD Command;	//���� PBO_SELECT PBO_EXECUTE  PBO_CANCEL PBO_DIRECT
	WORD Attrib;	//״̬���Ϸ֣�PBO_NULL PBO_OPEN, PBO_CLOSE,

	WORD NeedRetFlag; //�Ƿ���Ҫң�ط�У��Ϣ��־
	WORD RetResult; //ң�ط�У״̬ PBO_SUCESS  PBO_FAIL
	BYTE Info[20];	//������ң�ڱ���������Ϣ
};

struct TST
{
	WORD	Group;	 
	WORD	No; 	
	WORD	Command;	
	WORD	Attrib; 
	WORD	Num;	  //���ݸ���
	WORD	Offset;   //ƫ����
	WORD	Result;   //���ؽ��
	PBYTE	Buf;  
};//��ֵ 


#pragma	pack(1)

//ͨ�÷����ʶ��Žṹ����
typedef struct
{
	BYTE byGroup;               //��
	BYTE byEntry;               //��Ŀ
}TGin;

//�������ݽṹ
typedef struct
{
	BYTE Cont:1;                //����״̬λ
	BYTE Number:7;              //��Ŀ
}TNext;

//ͨ�÷������������ṹ
typedef struct
{
	BYTE byDataType;            //��������
	BYTE byDataSize;            //���ݿ��
	BYTE byDataNum;
	//	TNext Next;                 //��������
}TGdd;

//ͨ�÷������ݼ���Ŀ
typedef struct
{
	BYTE Cont:1;                //����״̬λ
	BYTE Count:1;				//��ͬRII��һλ���� 0<-->1����
	BYTE Number:6;              //��Ŀ
}TNgd;

//ͨ�÷������ݽṹ
typedef struct
{
	TNgd Ngd;					//ͨ�÷������ݼ���Ŀ
	TGin Gin;                   //ͨ�÷����ʶ���
	BYTE byKod;                 //���������
	TGdd Gdd;                   //ͨ�÷�����������
	BYTE byGid;                 //ͨ�÷����ʶ����
}TGenData;

struct TGenItem
{
	TGin Gin;                   //ͨ�÷����ʶ���
	BYTE byKod;                 //���������
	TGdd Gdd;                   //ͨ�÷�����������
	BYTE byGid;                 //ͨ�÷����ʶ����
};

//���崦��ʱ��Ҫ�洢����Ϣ�ṹ
typedef struct
{
	BYTE byOldFCB;          //�ϴ���ȷ���պ󱣴��FCBλ
	BYTE byLastCnt;			//ͨ�����ݼ������		
	WORD byRII;				//ң�ط��ؽ��
	BYTE byDCC;
	BYTE byCallDDSQ;		//�ٻ����˳���	
	BYTE byScanNumber;      //ɨ�����
	BYTE byCOL;             //���ݼ���
	BYTE byGRC;             //GRC
}TIECInfo;

#pragma pack()

typedef union{
	TRelayAlarmEv 	tAlram;
	TRelayStartEv 	tStart;
	TRelayActionEv 	tAction;
}TRelayReport;

#endif
