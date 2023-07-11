
#include "clink.h"           /* Clink class define */


/*--------------------------------------------------------------------------------------*/
/* 功能:链表管理类构造函数																*/
/* 输入:maxnode 本双向链表要求管理的最大节点个数,=0时表示本链表不限制节点个数			*/
/* 输出:无																				*/
/* 返回:无																				*/
/*--------------------------------------------------------------------------------------*/
CLink::CLink(DWORD maxnode)
{
	//初始化链表首节点指针为NULL
    m_pLink = NULL;

	//当前链表节点指针为NULL
    m_pCurrent = NULL;

	//初始化链表允许的最大节点个数
	m_nMaxNode = maxnode;

	//链表节点计数清零
	m_nNode = 0;
}

/*--------------------------------------------------------------------------------------*/
/* 功能:在链表尾部追加一个节点															*/
/* 输入:node 要追加节点的指针															*/
/* 输出:无																				*/
/* 返回:!=NULL时,该双向链表已满,首节点已出链,返回首节点指针;=NULL时,操作正常			*/
/*--------------------------------------------------------------------------------------*/
CNode* CLink::AddTail(CNode *node)
{
    DWORD imask;
	ASSERT(node != NULL);
	CNode *theNode = NULL;

    //判断链表是否为空
    if (m_pLink)
    {
		//链表不空
		//判断该链表是否需要限定个数(m_nMaxNOde!=0),判断链表节点数是否大于限定数
		if((m_nMaxNode!=0)&&(m_nNode >= m_nMaxNode))
		{
			//超过限定个数，调用删除首节点函数,保留首节点指针,把首节点从链表中摘除
			theNode = RemoveHead();
		}
        
		//禁止系统调度
		OS_Protect(0);

		//插入节点首指针指向链表尾节点
        node -> Prev = m_pLink -> Prev;

		//链表尾节点的首指针指向插入节点
        (node -> Prev) -> Next =  node;

		//插入节点尾指针指向链表首节点
        node -> Next = m_pLink;

		//链表首节点的首指针指向插入节点
        m_pLink->Prev =  node;

		//恢复系统调度
		OS_UnProtect(0);
    }
    else
    {
		//链表为空,禁止系统调度
		OS_Protect(0);

		//链表首节点指针指向插入节点
        m_pLink =  node;

		//插入节点的首指针指向插入节点
        node -> Prev =  node;

		//插入节点的尾指针指向插入节点
        node -> Next =  node;

		//恢复系统调度
		OS_UnProtect(0);
    }

	//链表节点总个数加一
	m_nNode++;

	//链表满时返回出链的首节点指针,否则返回NULL
	return theNode;
}

/*--------------------------------------------------------------------------------------*/
/* 功能:把双向链表的首节点从链表中删除													*/
/* 输入:node 用于存放首节点指针的指针													*/
/* 输出:首节点指针的地址																*/
/* 返回:无																				*/
/*--------------------------------------------------------------------------------------*/
void CLink::RemoveHead(CNode **node)
{
    DWORD imask;

    //禁止系统调度
	OS_Protect(0);

	//检查链表是否为空链
    if(m_pLink == NULL)
    {
		//链表为空,设置输出节点指针为NULL
        (*node) = NULL;

		//恢复系统调度,函数退出
		OS_UnProtect(0);
        return;
    }
    
    //判断链表是否只有一个节点
    if(m_pLink->Next == m_pLink)
    {
		//链表只有一个节点,返回首节点
        *node = m_pLink;

		//链表节点计数清零
		m_nNode = 0;

		//链表首节点指针为NULL
        m_pLink = NULL;

		//恢复系统调度,函数退出
		OS_UnProtect(0);
        return;
    }
    
	//链表中有多个节点,输出链表首节点
    *node = m_pLink;
    m_pLink = m_pLink->Next;
    m_pLink->Prev = (*node) -> Prev;
    m_pLink -> Prev -> Next = m_pLink;
	m_nNode--;
	
	//恢复系统调度
	OS_UnProtect(0);
}

/*--------------------------------------------------------------------------------------*/
/* 功能:把双向链表的首节点从链表中删除													*/
/* 输入:无																				*/
/* 输出:无																				*/
/* 返回:链表首节点指针																	*/
/*--------------------------------------------------------------------------------------*/
CNode* CLink::RemoveHead()
{
    CNode *node;
    CNode *Last;

	//禁止系统调度
	OS_Protect(0);
	
	//判断链表是否为空
    if(m_pLink == NULL)
    {
		//链表为空,返回NULL
        node = NULL;

		//恢复系统调度
		OS_UnProtect(0);
        return (CNode *)node;
    }
    
    //链表不为空,判断链表中是否只有一个节点
    if(m_pLink -> Next == m_pLink)
    {
		//链表只有一个节点,返回首节点
        node = m_pLink;

		//链表首节点指针为NULL
        m_pLink = NULL;

		//恢复系统调度
		OS_UnProtect(0);
        return (CNode *)node;
    }
    
    //链表中有多个节点,摘除链表首节点
    node = m_pLink;
    Last = m_pLink->Prev;
    m_pLink = m_pLink->Next;
    m_pLink->Prev = Last;
    Last->Next = m_pLink;

	//链表计数减1
	m_nNode--;

	//恢复系统调度
	OS_UnProtect(0);
    return (CNode *)node;
}

/*--------------------------------------------------------------------------------------*/
/* 功能:从链表中删除指定节点															*/
/* 输入:node 要删除的节点指针															*/
/* 输出:无																				*/
/* 返回:无																				*/
/*--------------------------------------------------------------------------------------*/
void CLink::RemoveNode(CNode *node)
{
    DWORD imask;
    ASSERT(node != NULL);

    //判断链表中是否只有一个节点
    if(node -> Prev == node)
    {
		//链表中只有一个节点,置链表首节点指针为NULL
        m_pLink = NULL;

		//链表节点计数清零
		m_nNode = 0;
    }
    else
    {
		//链表中有多个节点
		//禁止系统调度
		OS_Protect(0);

		//把删除的节点出链
        node->Prev->Next = node->Next;
        node->Next->Prev = node->Prev;

        //判断删除的节点是否链表首节点
        if(node == m_pLink)
			//删除的节点为链表首节点,链表首节点指针指向删除节点的尾指针
            m_pLink = node->Next;
		
		//链表节点计数减1
		m_nNode--;

		//恢复系统调度
		OS_UnProtect(0);
    }
	
}

/*--------------------------------------------------------------------------------------*/
/* 功能:取链表内当前节点的下一个节点													*/
/* 输入:无																				*/
/* 输出:无																				*/
/* 返回:下一个节点的指针																*/
/*--------------------------------------------------------------------------------------*/
CNode *CLink::Next()
{
    CNode *tmp;

	//判断链表是否为空链,判断当前节点指针是否为空(必须先调用ToHead再调用本函数
    if(m_pLink&&m_pCurrent)
    {
		//链表不空且当前节点指针不空，把当前节点指针指向下一个节点
        tmp = m_pCurrent;
        m_pCurrent = m_pCurrent->Next;

		//判断链表是否搜索完毕
        if(m_pCurrent == m_pLink)
			//链表搜索完毕置当前节点指针为NULL
            m_pCurrent = NULL;
    }
    else
		//链表为空链,或者链表已经搜索完毕,返回NULL
		tmp = NULL;

	//返回该节点指针
    return tmp;
}

/*--------------------------------------------------------------------------------------*/
/* 功能:对链表内所有节点执行特定操作													*/
/* 输入:func 对节点执行的特定操作函数指针;para 操作参数									*/
/* 输出:无																				*/
/* 返回:无																				*/
/*--------------------------------------------------------------------------------------*/
void CLink::ForEach(TActionFunc func,void * para)
{
    CNode *unit = m_pLink;
	
	//判断链表是否为空链,链表为空链返回
    if(unit == NULL) return;

	//链表不空,遍历链表内所有节点
    do
    {
		//用特定函数对本节点执行操作
        (*func)((void*)unit,para);

		//遍历下个节点
        unit = unit->Next;
    }while((unit)&&(unit->Next != m_pLink));
}

/*--------------------------------------------------------------------------------------*/
/* 功能:遍历整个链表,检索符合特定条件的节点												*/
/* 输入:func 检索的条件函数指针;para 操作参数											*/
/* 输出:无																				*/
/* 返回:链表第一个符合条件节点的指针													*/
/*--------------------------------------------------------------------------------------*/
CNode *CLink::FirstThat(TCondFunc Test,void * para)
{
    BOOL Found = FALSE;
    CNode *unit=m_pLink;
    
	//判断链表是否为空链,链表为空链返回
	if(unit == NULL) return NULL;

	//链表不空,遍历链表所有节点
    do
    {
		//调用测试函数,判断该节点是否符合检索的条件
        if ( (*Test)( unit, para) )
        {
			//符合检索条件,返回该节点指针
            Found = TRUE;
            break;
        }

		//遍历下个节点
        unit = unit->Next;
    }while((unit)&&(unit->Next != m_pLink));

	//搜索到符合条件的节点返回该节点指针,否则返回NULL
    if(Found)
        return unit;
    else return NULL;
}

DWORD CLink::GetNodeNum()
{
	return m_nNode;
}

