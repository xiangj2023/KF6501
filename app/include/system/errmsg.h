#ifndef _ERRMSG_H
#define _ERRMSG_H

/**********************************************************************/
/* ����ϵͳ���ô�����Ϣ,����ucosii(v2.5)ϵͳ,����1~255Ϊ������Ϣ����  */
/* Ϊ���ڹ���������и��죬ͳһ������Ϣ����Ϊ��0x8000+errinf	      */
/* �����󣬴�0x800~0x8fffΪ����ϵͳƽ̨���ô�����Ϣ������ϵͳ������Ϣ */
/* Ӧ�ڴ˷�Χ֮��													  */
/**********************************************************************/

#define ERR_SUCCESS     0   		  	/*�������óɹ�*/
#define ERR_FAILURE     0x1    			/*��������ʧ��*/

//����ϵͳƽ̨������Ϣ����
#define ERR_NO_FIND_TASK      	0x8100  //û���ҵ�ָ��������������
#define ERR_NO_FIND_TIMER     	0x8101  //û���ҵ�ָ���Ķ�ʱ��
#define ERR_INVALID_TIMER     	0x8102  //��Ч�Ķ�ʱ��
#define ERR_MEM_FAILED        	0x8103  //ϵͳ�����ڴ�ʧ��
#define ERR_CREATE_MSG_FAILED 	0x8104  //������Ϣ����ʧ��
#define ERR_INVALID_PRIO      	0x8105  //�������ȼ��ظ���
#define ERR_INVALID_SEM       	0x8106  //���õ��ź�����Ч,ָ��ʶ��Խ��
#define ERR_SEM_NO_CREATE     	0x8107  //�ź���û�б�����
#define ERR_SEM_CREATE_ERR    	0x8108  //�ź�������ʧ��
#define ERR_GET_SEM_ERR       	0x8109  //��ȡ�ź���ʧ��
#define ERR_RELEASE_SEM_ERR   	0x8109  //��ȡ�ź���ʧ��

//��Ϣ���������Ϣ����
#define ERR_APPIDINVALIDATE    	0x2000  /*appid invalidate*/

#define ERR_MESSAGE_NULL       	0x2100  /*GetMessage routine*/
#define ERR_MESSAGE_QUIT       	0x2103  /*GetMessage routine */
#define ERR_MESSAGE_OK         	0x2104  /*GetMessage routine */
#define ERR_POST_EVSEND        	0x2005   /*PostMessage routine*/

#define ERR_PMTIMEOUT          	0x2001 /*post message timeout*/
#define ERR_POST_QSEND         	0x2002   /*PostMessage routine*/
#define ERR_ALLOCMEM           	0x2003
#define ERR_GET_FREE_NODE      	0x2004
#define ERR_GET_MUTEX	       	0x2005
#define ERR_POST_SEM	       	0x2006
#define ERR_GET_SEM	       	   	0x2007
#define ERR_POST_MSG			0x2008

/*GetDevID�������ش������*/
#define ERR_PORTID              0x2600     /*�˿ںŷǷ�*/  
/*WaitPortReady�������ش������*/
#define ERR_PORT_INIT           0x2601     /*�˿ڳ�ʼ������*/

#define ERR_SERIAL_CHANNEL      0x2610     /*����ͨ���Ŵ���*/
#define ERR_SERIAL_INIT         0x2611     /*����û��ʼ��*/
#define ERR_SERIAL_OPEN         0x2612     /*���ڴ򿪴���*/
/*prompt���ش������*/
#define ERR_PROMPT              0x2620     /*����̨�ַ���������Ϊ��*/
/*LookVar��AddVar�������*/
#define ERR_VARID               0x2621     /*����ID �Ŵ���*/
#define ERR_VARTABLEFULL        0x2622     /*����������*/

/*ReadFile�������ش������*/
#define ERR_OPEN                0x25001  /*�ļ��򿪴���*/
#define ERR_FILESTATE           0x25002  /*���ļ�״̬����*/
#define ERR_FILELEN             0x25003  /*�ļ����ȷǷ�*/
#define ERR_OFFSET              0x25004  /*����������ļ�ƫ�����Ƿ�,��дƫ�����Ƿ�WriteFile����*/
#define ERR_FILESEEK            0x25005  /*�ļ�ָ���ƶ�����,WriteFile��ReadFile*/
#define ERR_FILEREAD            0x25006  /*���ļ�����*/
#define ERR_CHECKSUM            0x25007  /*�ļ�У�����*/

/*WriteFle�������ش������*/
#define ERR_WRITELEN            0x25008   /*д�ļ����ȴ���*/
#define ERR_WRITEFILE           0x25009   /*д�ļ�����*/

#endif
