#include <stdarg.h>    
#include <string.h>
#include "system.h"
#include "debugcom.h"
 
TCommBlock cbwPrompt;

#define ERRNAME  "SERR"
#define COMNAME  "DCOM"


//��ʼ����������ʾ�ڴ�Ӱ����
void InitDebugInfo()
{
    memset(&cbwPrompt,0,sizeof(TCommBlock));
    
    cbwPrompt.szPrompt = new char[MAX_COMM_PROMPT];
    if( cbwPrompt.szPrompt != NULL )
        cbwPrompt.nSize = MAX_COMM_PROMPT;
    else
        cbwPrompt.nSize = 0;
}

#define MIN_RESERVE_NUM 10

void WritePrompt(char *buf)
{
    char *pBuf = buf;
    
#if(__DEBUG_COMM == YES)
    DebugOutput(buf);
#else
    while(*pBuf != NULL)
    {
        cbwPrompt.szPrompt[cbwPrompt.nWriteIndex++%cbwPrompt.nSize] = *pBuf++;
    }
#endif
    
}

DWORD DisplayCommBuf(DWORD port,void* head,PBYTE pbuf,WORD len,void *tail)
{

    //�����ǰ�����ڶ˿ڵ�ʽ״̬
    if(cbwPrompt.hPort == 0)
        return ERR_PORTID;
    
    //�ж�д����̨�Ķ˿��Ƿ�͵�ǰ�ĵ��Զ˿�һ��
    if((cbwPrompt.hPort != port)&&(port != DEBUG_PORT))
        return ERR_PORTID;

    //���Բ����Ƿ��ʼ��
    if(cbwPrompt.nSize == 0)
        return ERR_PROMPT;
    
    char aBuf[4];
    char *aHead,*aTail;
    aHead = (char*)head;
    aTail = (char*)tail;
    
    //����̨дǰ���ַ���
    if(aHead != NULL)
        WritePrompt(aHead);
    
    //�ѻ�����ת��Ϊ�ַ���д������̨
    if((len!=0)&&(pbuf!=NULL))
    {
        for(int i=0;i<len;i++)
        {
            sprintf(aBuf,"%02X ",pbuf[i]);
            WritePrompt(aBuf);
        }
    }
    
    //�Ѻ�׺�ַ�д�뵽����̨
    if(aTail != NULL)
        WritePrompt(aTail);
    return ERR_SUCCESS;
}

DWORD ReadPrompt(char *buf,DWORD len)
{
    DWORD i,tlen;   //����̨���ַ����ܳ���
    tlen = (cbwPrompt.nWriteIndex-cbwPrompt.nReadIndex)&(cbwPrompt.nSize-1);
	if( tlen <= 0 )
		return 0;

    if(len > tlen)
        len = tlen;
    for(i=0;i<len;i++)
        buf[i] = cbwPrompt.szPrompt[cbwPrompt.nReadIndex++%cbwPrompt.nSize];
    return len;
}

void SetDebugComm(DWORD portid)
{
    cbwPrompt.hPort = portid;
	cbwPrompt.nWriteIndex = 0; 
	cbwPrompt.nReadIndex = 0; 
	memset(cbwPrompt.szPrompt,0,cbwPrompt.nSize);
}

void CancelDebugComm()
{
    cbwPrompt.hPort = 0;
}
 
