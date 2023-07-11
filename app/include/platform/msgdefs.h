#ifndef _MSGDEFS_H
#define _MSGDEFS_H

#include "sysdefs.h"
/*��Ϣ�ṹ���壬��Ϣ�����ĸ�������ɣ�����LOWORD(lParam)��ʾ��Ϣ���ͣ��������������ݾ����
��Ϣ���ͣ�ȷ����������ݶ���
*/

typedef struct message{
  DWORD		lParam;    
  DWORD  	hParam;     
  DWORD  	wParam;
  DWORD 	pParam;     
}TMessage;

#define SM_EVENTMSG			  0xfff0					//�����Ӧ����Ϣ����  
#define SM_EVENTD0			  (	SM_EVENTMSG+(1<<0))		//D0�����Ӧ����Ϣ
#define SM_EVENTD1			  (	SM_EVENTMSG+(1<<1))		//D1�����Ӧ����Ϣ
#define SM_EVENTD2			  (	SM_EVENTMSG+(1<<2))		//D2�����Ӧ����Ϣ
#define SM_EVENTD3			  (	SM_EVENTMSG+(1<<3))		//D3�����Ӧ����Ϣ

#define SM_QUIT               255   //�˳���Ϣ

enum EMessage{ 
	SM_TIMEOUT=100,   	//��ʱ����Ϣ
	SM_BIEVENT, 		//��SOE
	SM_BIBURST, 		//����λң��
	SM_DOE, 			//��DOE��Ϣ
	SM_DPIBURST,		//����λ˫ң��
	SM_RELAYEVENT,		//������������
	SM_RELAYDATA,		//�������ݸı�
//	SM_RELAYALARM,		//���������澯������Ϣ
//	SM_RELAYSTART,		//������������������Ϣ
//	SM_RELAYACTION, 	//������������������Ϣ
		
	// ������Ϣ
	SM_BOSELECT,		 //ң��Ԥ��
	SM_BOSELECTECHO,	 //ң��Ԥ����Ӧ
	SM_BOEXEC,			 //ң��ִ��
	SM_BOEXECECHO,		 //ң��ִ����Ӧ
	SM_BODIRECT,		 //ң��ֱ�Ӳ���
	SM_BODIRECTECHO,	 //ң��ֱ�Ӳ�����Ӧ
	SM_BOTEST,			 //��������
	
	SM_AOSELECT,		 //ң��Ԥ��
	SM_AOSELECTECHO,	 //ң��Ԥ����Ӧ
	SM_AOEXEC,			 //ң��ִ��
	SM_AOEXECECHO,		 //ң��ִ����Ӧ
	SM_AODIRECT,		 //ң��ֱ�Ӳ���
	SM_AODIRECTECHO,	 //ң��ֱ�Ӳ�����Ӧ
			
	SM_SYNCSELECT,		 //ͬ��Ԥ��
	SM_SYNCSELECTECHO,	 //ͬ��Ԥ����Ӧ
	SM_SYNCEXEC,		 //ͬ��ִ��
	SM_SYNCEXECECHO,	 //ͬ��ִ����Ӧ
	SM_SYNCDIRECT,		 //ͬ��ֱ�Ӳ��� 
	SM_SYNCDIRECTECHO,	 //ͬ��ֱ�Ӳ�����Ӧ
	
	SM_TQSELECT,		 //ͬ��Ԥ��
	SM_TQSELECTECHO,	 //ͬ��Ԥ����Ӧ
	SM_TQEXEC,			 //ͬ��ִ��
	SM_TQEXECECHO,		 //ͬ��ִ����Ӧ
	
	//��������Ϣ
	SM_READCOMM,		 //��������Ϣ
	SM_WRITECOMM,		 //д������Ϣ
	SM_COMMIDLE,		 //�˿�ͨ�����У�����һ��ʱ����û�н���Ҳû�з���
	SM_COMMTXDIDLE, 	 //����ͨ������
	SM_COMMRXDIDLE, 
	SM_COMMEXCEPT,		 //ͨ���쳣
	SM_WRITECONT,		 //����������Ϣ
	
	SM_KEYDOWN, 		 //���̰���
	SM_LCD, 			 //LCD֪ͨ����������ͣ������Ϣ
	SM_TRANSOIE,		 //͸�����ݴ�����Ϣ
	SM_TRANSBHDZ,		 //���ͱ�����ֵ
	SM_NOTIFY,

	SM_DICHANGE,
	SM_CANTXDRXDOK,
	SM_PARACHANGE,
	SM_TASKREGISTER,
	SM_COEMAKE,

	SM_SAMPLEEND,		//AD��������
	SM_POWERDOWN,		//ϵͳ��Դ��ѹƫ��
//	SM_AUTOADJUSTSTEP1,		//ϵ���Զ�У��,����У��
	SM_AUTOADJUST,		//ϵ���Զ�У����ϵ��У��
	SM_AUTOADJUSTECHO,	//�Զ�У׼���
//	SM_AUTOADJUSTECHO2,	//�Զ�У׼���
	SM_STARTDIST,		//����¼��
//	SM_THERMALOVERLOAD,   //��ʱ�����ȹ������ۼӳ���
	SM_RELAY_INT,		//���������ж�
	SM_RELAYTIMER,		//�����ö�ʱ
	SM_LEDCHECK,		//LED�Լ�
	SM_SAVEREPORT,		//�汣������
	SM_RELAYRTMB,		//˫����������λ�շ�
	SM_BIPARACHANGE,	
	SM_WRITESINGNALRW,	//�޸��źż̵�����	
	SM_SYS_RESET,
	SM_CALLSUBDATA,     //���ϲ������װ����ȡ��������
	SM_GETFLDATA,		//���ϲ������װ��ͨѶ�����·�����������ȡ����Ϣ
	SM_DATAISOK,			//�������ݼ�����ɺ��͸�ͨѶ�������Ϣ
	SM_STOPCALLDATA,		//��೬ʱ��ֹͣ�ٻ�����
	SM_STEPOVERCMD, 	//��������
	SM_STEPOVERECHO,		//�������У	
	SM_LOGICCHANGED,			//�߼��ļ��޸ģ���Ҫ���³�ʼ���߼�
	SM_NETCARDPARACHG,	//���������ı�
	SM_NOTIFYETHLINKED, 	//��̫������
	SM_NOTIFYETHCLOSED,		//��̫�����ӶϿ�
	SM_AIEVENT, 			//AIE����֪ͨ
	SM_JDXXPICK,				//�ӵ�ѡ��������Ϣ
	SM_JDXXRESULT,				//�ӵ�ѡ�߽��
	SM_DISTEVENT			//¼������֪ͨ
};


#endif
