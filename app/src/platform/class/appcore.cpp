#include "appcore.h"

extern TTask_Map TaskTable[];

//����������Ȼ������Ϣӳ���
const AFX_MSGMAP CApp::messageMap =
{
    //�������Ϣӳ�����
    NULL,

    //�������Ϣӳ�����
    &CApp::_messageEntries[0]
};

//��ǰ�����Ӧ�ó���
CApp *CApp::m_pActive=NULL;

//ȡ������Ϣӳ������
const AFX_MSGMAP* CApp::GetMessageMap() const 
{
    return &CApp::messageMap;
}

//����������Ϣӳ��
//CAppΪ����������Ļ��࣬�䱾�������Ϣ
const AFX_MSGMAP_ENTRY CApp::_messageEntries[] =
{
    { 0, 0, AfxSig_end, 0 }     // nothing here
};

CApp::CApp(DWORD *pAppId)
{
	
	//ע��ucos��ֻ�ܴ���һ��ָ�����
    AppID = pAppId[0];
    m_dwTask = pAppId[0];
	
	ASSERT(pAppId[1]<MAX_MSG_BUF_LEN);
	
	m_nMsgLen = pAppId[1];
	
    //�������񽻻����ݻ�����
    if(m_nMsgLen != 0)
    {
        m_pMsgBuf = new BYTE[m_nMsgLen];
    	ASSERT(m_pMsgBuf);

        //�����񽻻��ڴ�������
        m_nMsgLen = m_nMsgLen;
  	    memset(m_pMsgBuf,0,m_nMsgLen);
    }
}

/*------------------------------------------------------------------------------------------*/
/* ����:���������Ϣӳ�����,������Ϣ��Ӧ����Ϣӳ�����                                     */
/* ����:lpEntry �������Ϣӳ���ָ��,mMsg ��Ҫ���ҵ���Ϣ,nCode ��Ϣ����                     */
/* ���:��                                                                                  */
/* ����:��ӳ����в��ҵ�����Ϣ���ظ���Ϣ��Ӧ����Ϣӳ��,���򷵻�NULL                         */
/*------------------------------------------------------------------------------------------*/
const AFX_MSGMAP_ENTRY* FindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry,
                                         WORD nMsg, WORD nCode)
{    
    // ������Ϣӳ�������msg��Ϣ��ӳ������
    while (lpEntry->nSig != AfxSig_end)
    {
        //��ǰӳ����Ƿ���ϼ���������
        if (lpEntry->nMessage == nMsg && lpEntry->nCode == nCode )
        {
            //��������,���ص�ǰ��Ϣ�����
            return lpEntry;
        }

        //�Ƚ���һ��ӳ�����
        lpEntry++;
    }

    //ӳ�����,δ�ҵ���Ӧ����Ϣ���,����NULL
    return NULL;    // not found
}

/*------------------------------------------------------------------------------------------*/
/* ����:������Ϣ���Ⱥ��������ݽ��յ���Ϣ���Զ�������Ӧ����Ϣ������                        */
/* ����:msg������յ�����Ϣ,lpEntry ��Ϣӳ���ָ��                                          */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
/*------------------------------------------------------------------------------------------*/

void CApp::Dispatch(const TMessage *msg)
{
    BYTE  IsMap;
    const AFX_MSGMAP_ENTRY* lpEntry;
    BOOL bFound = FALSE;

        
    //ȡ��ǰӦ�ó��������Ϣӳ���
    const AFX_MSGMAP* pMessageMap = GetMessageMap();

    //������Ӧ�ó����༰�����л������Ϣӳ���,����msg��Ϣ�ĵ��Ⱥ���
    for ( ; pMessageMap != NULL;pMessageMap = pMessageMap->pBaseMap)
    {
        //������Ϣӳ����Һ���,���Ҹ���Ϣ��ӳ����е����
        lpEntry = FindMessageEntry(pMessageMap->lpEntries,LOWORD(msg->lParam), 0);
        
        //�ж��ڱ����Ƿ��ҵ�����Ϣ��ӳ��
        if(lpEntry != NULL)
        {
            //�ҵ�����Ϣ��ӳ��,���ò��ҳɹ���־,�˳�ѭ��
            bFound = TRUE;
            break;
        }
    }
    
    if(bFound)
    {
        /*�ҵ���Ӧ����Ϣӳ��*/
        IsMap = 1;
        union MessageMapFunctions mmf;

        //����Ϣ������ָ��ת��Ϊ��Ϣӳ���ʽ
        mmf.pfn = lpEntry->pfn;
        
        WORD nSig = lpEntry->nSig;
        
        switch(nSig)
        {
        	default:
            		//ASSERT(FALSE);
            		break;
        	case AfxSig_vv:
            		(this->*mmf.pfn_vv)();
            		break;          
        	case AfxSig_vw:
            		(this->*mmf.pfn_vw)(HIWORD(msg->lParam));
            		break;          
        	case AfxSig_vwww:
            		(this->*mmf.pfn_vwww)(msg->hParam,msg->wParam,LOWORD(msg->pParam));
            		break;          
        	case AfxSig_vwdw:
            		(this->*mmf.pfn_vwdw)(msg->hParam,msg->pParam);
            		break;
            
        	case AfxSig_vwdwdw:
            		(this->*mmf.pfn_vwdwdw)(msg->hParam,msg->wParam,msg->pParam);
            		break;

        	case AfxSig_vwdwdwdw:
            		(this->*mmf.pfn_vwdwdwdw)(HIWORD(msg->lParam),msg->hParam,msg->wParam,msg->pParam);
            		break;

        	case AfxSig_vdwdwdww:
            		(this->*mmf.pfn_vdwdwdww)(msg->hParam,msg->wParam,msg->pParam,HIWORD(msg->lParam));
            		break;

        	case AfxSig_vdw:
            		(this->*mmf.pfn_vdw)(msg->hParam);
            		break;

        	case AfxSig_vdwdw:
            		(this->*mmf.pfn_vdwdw)(msg->hParam,msg->wParam);
            		break;
        	case AfxSig_vdwdwdw:
            		(this->*mmf.pfn_vdwdwdw)(msg->hParam,msg->wParam,msg->pParam);
            		break;
        	case AfxSig_vptrdw:
            		break;
        	case AfxSig_vptrdwdw:        
            		break;

        	case AfxSig_vdww: 
            		(this->*mmf.pfn_vdww)(msg->hParam,msg->wParam);
            		break;
        	case AfxSig_vdwww: 
            		(this->*mmf.pfn_vdwww)(msg->hParam,msg->wParam,LOWORD(msg->pParam));
            		break;
        	case AfxSig_vdwwww:
            		(this->*mmf.pfn_vdwwww)(msg->hParam,msg->wParam,LOWORD(msg->pParam),HIWORD(msg->pParam));
            		break;
        	case AfxSig_vdwwwww:
            		(this->*mmf.pfn_vdwwwww)(msg->hParam,msg->wParam,LOWORD(msg->pParam),HIWORD(msg->pParam),0);
            		break;
		}            		
    }
    else
    {
        //δ�ҵ�����Ϣ��Ӧ����Ϣӳ�����
        IsMap = 0;

        //����ϵͳȱʡ��Ϣ������
        OnDefault();
    }
#if (__DEBUG_MESSAGE == YES)
    
    //�ڵ���ģʽ��,�����Ϣ������Ϣ������̨
    TraceMsg(msg,IsMap);
#endif

}

/*------------------------------------------------------------------------------------------*/
/* ����:�������Ϣѭ������,�ú���һ��û�з���,���Ǹ�����ɾ��                              */
/* ����:tid�����ʶ��, lpEntry��Ϣӳ���ָ��                                                */
/* ���:��                                                                                  */
/* ����:��                                                                                  */
/*------------------------------------------------------------------------------------------*/

void CApp::Run(void)
{
    TMessage msg;
  
    //���ó�ʼ״̬Ϊ������Ϣ��ȷ
    DWORD status = ERR_MESSAGE_OK;
    
    //ѭ�����ý�����Ϣ����,ֱ���յ������˳���Ϣ
    //while(status != ERR_MESSAGE_QUIT)
    while(1)
    {
        //����ȡ��Ϣ����,��������Ϣ����ͷ,ȡ��Ҫ�������Ϣ
        status = OS_GetMessage(m_dwTask,&msg);

		if(status == ERR_MESSAGE_QUIT)
			break;
		
		//����������б�־������
		if(TaskTable[m_dwTask-TASK_HANDLE].pTask!=NULL)
			TaskTable[m_dwTask-TASK_HANDLE].pTask->Counter = 0;
		
        //�жϽ��յ���Ϣ�Ƿ�Ϊ��Ч��Ϣ
        if(status == ERR_MESSAGE_OK)
        {
            //��Ϣ��Ч,������Ϣ���Ⱥ����Ը���Ϣ���е���
            Dispatch(&msg);
        }
    }

    //�յ������˳���Ϣ,�Ǽ������˳��¼�
   

    //���������˳�����,ɾ������������
    OS_Exit(1);

    //����������������,�ͷ������Դ
    this->~CApp();
    
}

