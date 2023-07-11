#include "appcore.h"

extern TTask_Map TaskTable[];

//定义任务调度基类的消息映射表
const AFX_MSGMAP CApp::messageMap =
{
    //基类的消息映射入口
    NULL,

    //自身的消息映射入口
    &CApp::_messageEntries[0]
};

//当前激活的应用程序
CApp *CApp::m_pActive=NULL;

//取自身消息映射表入口
const AFX_MSGMAP* CApp::GetMessageMap() const 
{
    return &CApp::messageMap;
}

//定义基类的消息映射
//CApp为所有任务类的基类，其本身处理的消息
const AFX_MSGMAP_ENTRY CApp::_messageEntries[] =
{
    { 0, 0, AfxSig_end, 0 }     // nothing here
};

CApp::CApp(DWORD *pAppId)
{
	
	//注意ucos中只能传递一个指针参数
    AppID = pAppId[0];
    m_dwTask = pAppId[0];
	
	ASSERT(pAppId[1]<MAX_MSG_BUF_LEN);
	
	m_nMsgLen = pAppId[1];
	
    //分配任务交换数据缓冲区
    if(m_nMsgLen != 0)
    {
        m_pMsgBuf = new BYTE[m_nMsgLen];
    	ASSERT(m_pMsgBuf);

        //把任务交换内存区清零
        m_nMsgLen = m_nMsgLen;
  	    memset(m_pMsgBuf,0,m_nMsgLen);
    }
}

/*------------------------------------------------------------------------------------------*/
/* 功能:从任务的消息映射表中,查找消息对应的消息映射入口                                     */
/* 输入:lpEntry 任务的消息映射表指针,mMsg 需要查找的消息,nCode 消息参数                     */
/* 输出:无                                                                                  */
/* 返回:在映射表中查找到该消息返回该消息对应的消息映射,否则返回NULL                         */
/*------------------------------------------------------------------------------------------*/
const AFX_MSGMAP_ENTRY* FindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry,
                                         WORD nMsg, WORD nCode)
{    
    // 遍历消息映射表，查找msg消息的映射表入口
    while (lpEntry->nSig != AfxSig_end)
    {
        //当前映射表是否符合检索的条件
        if (lpEntry->nMessage == nMsg && lpEntry->nCode == nCode )
        {
            //符合条件,返回当前消息表入口
            return lpEntry;
        }

        //比较下一个映射表项
        lpEntry++;
    }

    //映射表内,未找到对应的消息入口,返回NULL
    return NULL;    // not found
}

/*------------------------------------------------------------------------------------------*/
/* 功能:任务消息调度函数，根据接收的消息，自动调度相应的消息处理函数                        */
/* 输入:msg任务接收到的消息,lpEntry 消息映射表指针                                          */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/

void CApp::Dispatch(const TMessage *msg)
{
    BYTE  IsMap;
    const AFX_MSGMAP_ENTRY* lpEntry;
    BOOL bFound = FALSE;

        
    //取当前应用程序类的消息映射表
    const AFX_MSGMAP* pMessageMap = GetMessageMap();

    //遍历本应用程序类及其所有基类的消息映射表,查找msg消息的调度函数
    for ( ; pMessageMap != NULL;pMessageMap = pMessageMap->pBaseMap)
    {
        //调用消息映射查找函数,查找该消息在映射表中的入口
        lpEntry = FindMessageEntry(pMessageMap->lpEntries,LOWORD(msg->lParam), 0);
        
        //判断在本类是否找到该消息的映射
        if(lpEntry != NULL)
        {
            //找到该消息的映射,设置查找成功标志,退出循环
            bFound = TRUE;
            break;
        }
    }
    
    if(bFound)
    {
        /*找到相应的消息映射*/
        IsMap = 1;
        union MessageMapFunctions mmf;

        //把消息处理函数指针转换为消息映射格式
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
        //未找到该消息对应的消息映射入口
        IsMap = 0;

        //调用系统缺省消息处理函数
        OnDefault();
    }
#if (__DEBUG_MESSAGE == YES)
    
    //在调试模式下,输出消息处理信息到调试台
    TraceMsg(msg,IsMap);
#endif

}

/*------------------------------------------------------------------------------------------*/
/* 功能:任务的消息循环处理,该函数一般没有返回,除非该任务被删除                              */
/* 输入:tid任务标识号, lpEntry消息映射表指针                                                */
/* 输出:无                                                                                  */
/* 返回:无                                                                                  */
/*------------------------------------------------------------------------------------------*/

void CApp::Run(void)
{
    TMessage msg;
  
    //设置初始状态为接收消息正确
    DWORD status = ERR_MESSAGE_OK;
    
    //循环调用进行消息调度,直到收到任务退出消息
    //while(status != ERR_MESSAGE_QUIT)
    while(1)
    {
        //调用取消息函数,从任务消息队列头,取需要处理的消息
        status = OS_GetMessage(m_dwTask,&msg);

		if(status == ERR_MESSAGE_QUIT)
			break;
		
		//清除任务运行标志计数器
		if(TaskTable[m_dwTask-TASK_HANDLE].pTask!=NULL)
			TaskTable[m_dwTask-TASK_HANDLE].pTask->Counter = 0;
		
        //判断接收的消息是否为有效消息
        if(status == ERR_MESSAGE_OK)
        {
            //消息有效,调用消息调度函数对该消息进行调度
            Dispatch(&msg);
        }
    }

    //收到任务退出消息,登记任务退出事件
   

    //调用任务退出函数,删除该运行任务
    OS_Exit(1);

    //调用任务析构函数,释放相关资源
    this->~CApp();
    
}

