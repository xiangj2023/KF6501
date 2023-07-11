
#include "debug.h"

STATUS DB_WriteRunRecord(char *str);

//�����������̨����
#if(OS_PLATFORM == OS_UCOSII)
	#define MAX_STDOUT_STRING	64
#else
	#define MAX_STDOUT_STRING	1024
	
	//����Hash��
	THashTable *g_hashTaskMap;
#endif

//�������̨������
STATIC STRING g_pStdOut[MAX_STDOUT_STRING];
//�����Ϣ����Ŀ����
STATIC DWORD g_nOutPut=0;

void ReportNormal(char *iof,...)
{
    DWORD len;
    va_list args;
    char *pbuf = (char*)g_pStdOut;

    //ʹ�ñ�׼����̨����
    OS_LockM(STDOUT_LOCK);

    //���쳣��Ϣ������д��:�쳣��Ϣ�ܼ�������ǰ�������ơ���ǰ�ļ����ƺ��кš���ǰʱ��
    len = sprintf(pbuf,"\r\nE%02d:",g_nOutPut);

    //д��ɱ����
    va_start(args,iof);
    len += vsprintf(pbuf+len,iof, args);
    va_end(args);

    //�����̨������ַ�������ӦС�ڿ���̨����󻺳���
    //ASSERT(strlen(pbuf)<MAX_STDOUT_STRING);

    //����debug.h��_DMESSAGE_OUTPUT�Ķ��壬�ض��������Ϣ
    DebugOutput(g_pStdOut);    //remed for test

    //�����Ϣ�ĸ�����1
    g_nOutPut++;

    //�ͷſ���̨��Դ
    OS_UnLockM(STDOUT_LOCK);
}

/*--------------------------------------------------------------------------------------*/
/* ����:ϵͳ�쳣��Ϣ�Ǽ�                                                                */
/* ����:func ��ǰ������;file ��FILE_LINE�굱ǰ�������ƺ��к�;iof �ɱ䳤�Ȳ���,ʹ��ͬ    */
/*      printf����;������쳣��Ϣ����:�쳣��Ϣ����,�ļ���,�к�,��ǰ����,ʱ��,�쳣��Ϣ.. */
/* ���:��                                                                              */
/* ����:��                                                                              */
/*--------------------------------------------------------------------------------------*/
void ReportError(char *func,char *file,char *iof,...)
{  
    DWORD len=0;
    va_list args;
   	OS_TaskDesp *pInfo = OS_GetTaskTab(SELF_TASK_ID);
    char *pbuf = (char*)g_pStdOut;

    //ʹ�ñ�׼����̨����
    OS_LockM(STDOUT_LOCK);

    //���쳣��Ϣ������д��:�쳣��Ϣ�ܼ�������ǰ�������ơ���ǰ�ļ����ƺ��кš���ǰʱ��
    len = sprintf(pbuf,"\r\nE%02d:[%s]",g_nOutPut,
        (pInfo==NULL)?"null":pInfo->TaskName);

    //����ļ����Ʋ�Ϊ�㣬����ļ�����
    if(file != NULL)
        len+=sprintf(pbuf+len,"%s",file);

    //�������Ʋ�Ϊ�㣬����ļ�����
    if(func != NULL)
        len += sprintf(pbuf+len," %s()",func);
  
    //д��ɱ����
    va_start(args,iof);
    len += vsprintf(pbuf+len,iof, args);
    va_end(args);

    //����debug.h��_DMESSAGE_OUTPUT�Ķ��壬�ض��������Ϣ
    DebugOutput(g_pStdOut);

    //�����Ϣ�ĸ�����1
    g_nOutPut++;

    //�ͷſ���̨��Դ
    OS_UnLockM(STDOUT_LOCK);
}


/*------------------------------------------------------------------------------------------*/
/* ����:�ѵ�ǰָ�����ڵĳ������ƺ��к�ת��Ϊ�ַ�����ʽ                                      */
/* ����:file��__FILE__��,�ṩ��ǰ������;line __LINE__��,�ṩ��ǰ�к�                        */
/* ���:��                                                                                  */
/* ����:�������������кŵ��ַ���                                                            */
/*------------------------------------------------------------------------------------------*/
char *FileLine(char file[],int line)
{
    static char fileline[52];
    WORD len=strlen(file);
    
    //��ȥ����Ŀ¼��ȡ��ǰ�ļ���
    while((file[len] != '\\')&&(file[len]!='/')&&(len != 0))
        len--;
    if(len != 0)
        len++;
    sprintf(fileline,"%s-%d",&file[len],line);
    return fileline;
}

#if (__DEBUG__ == YES)
void AssertFaild(LPSTR str,LPSTR fileline)
{
    ReportMsg("Assert: \"%s\" failed,in %s\n", str,fileline);
    Abort(EC_ASSERT);
}
#endif


