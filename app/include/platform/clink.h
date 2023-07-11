
#ifndef _CLINK_H
#define _CLINK_H

#include "sysdefs.h"
#include "system.h"

typedef void (*TActionFunc )(void* unit, void *para);
typedef BOOL (*TCondFunc )(void* unit, void *para);

struct CNode{
  CNode *Next;      //后一个节点指针
  CNode *Prev;      //前一个节点指针
  void	*pData;
};

//双向链表管理类定义,该双向链表即可以实现有限节点个数的双向链表,也可以实现不限制节点
//个数的双向链表;在限制节点个数的双向链表中,当节点个数满继续添加节点时,将把链表首节点
//自动出链
class CLink{
    
    //双向链表首节点
    CNode *m_pLink;

    //当前链表节点
    CNode *m_pCurrent;

    //链表节点计数
    DWORD m_nNode;

    //该链表最大可以联结的节点数,=0表示联结的节点个数不受限制
    DWORD m_nMaxNode;

  public:

    /*--------------------------------------------------------------------------------------*/
    /* 功能:链表管理类构造函数                                                              */
    /* 输入:maxnode 本双向链表要求管理的最大节点个数,=0时表示本链表不限制节点个数           */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    CLink(DWORD maxnode=0);
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:双向链表初始化函数                                                              */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    void Init()
    {
        //首节点为NULL
        m_pLink = NULL;

        //当前节点指针为NULL
        m_pCurrent = NULL;

        //链表内节点总数为零
        m_nNode = 0;
    }

    /*--------------------------------------------------------------------------------------*/
    /* 功能:检查该链表是否为空                                                              */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:=TRUE 链表为空; =FALSE 本链表不空                                               */
    /*--------------------------------------------------------------------------------------*/
    BOOL IsEmpty() {return (m_pLink == NULL);}
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:取链表的尾节点指针                                                              */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:链表尾节点指针                                                                  */
    /*--------------------------------------------------------------------------------------*/
    CNode *GetTail() {return m_pLink->Prev;}

    /*--------------------------------------------------------------------------------------*/
    /* 功能:在链表尾部追加一个节点                                                          */
    /* 输入:node 要追加节点的指针                                                           */
    /* 输出:无                                                                              */
    /* 返回:!=NULL时,该双向链表已满,首节点已出链,返回首节点指针;=NULL时,操作正常            */
    /*--------------------------------------------------------------------------------------*/
    CNode* AddTail(CNode *node);
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:把双向链表的首节点从链表中删除                                                  */
    /* 输入:node 用于存放首节点指针的指针                                                   */
    /* 输出:首节点指针的地址                                                                */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    void RemoveHead(CNode **node);
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:把双向链表的首节点从链表中删除                                                  */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:链表首节点指针                                                                  */
    /*--------------------------------------------------------------------------------------*/
    CNode *RemoveHead();

    /*--------------------------------------------------------------------------------------*/
    /* 功能:从链表中删除指定节点                                                            */
    /* 输入:node 要删除的节点指针                                                           */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    void RemoveNode(CNode *node);
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:把链表内当前节点指向链表首节点,用于链表检索                                     */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    void ToHead() { m_pCurrent = m_pLink;}
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:取链表内当前节点的下一个节点                                                    */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:下一个节点的指针                                                                */
    /*--------------------------------------------------------------------------------------*/
    CNode *Next();
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:取当前链表的首节点指针                                                          */
    /* 输入:无                                                                              */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    CNode *GetHead() { return m_pLink; }

    /*--------------------------------------------------------------------------------------*/
    /* 功能:把特定节点指定为链表的首节点                                                    */
    /* 输入:node 特定节点的指针                                                             */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    void Attach(CNode *node) { m_pLink = m_pCurrent = node; }
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:对链表内所有节点执行特定操作                                                    */
    /* 输入:func 对节点执行的特定操作函数指针;para 操作参数                                 */
    /* 输出:无                                                                              */
    /* 返回:无                                                                              */
    /*--------------------------------------------------------------------------------------*/
    void ForEach(TActionFunc func,void* para);
    
    /*--------------------------------------------------------------------------------------*/
    /* 功能:遍历整个链表,检索符合特定条件的节点                                             */
    /* 输入:func 检索的条件函数指针;para 操作参数                                           */
    /* 输出:无                                                                              */
    /* 返回:链表第一个符合条件节点的指针                                                    */
    /*--------------------------------------------------------------------------------------*/
    CNode *FirstThat(TCondFunc func,void* para);
	DWORD GetNodeNum();
};

#endif

