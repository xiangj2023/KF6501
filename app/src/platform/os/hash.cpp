#if (INSTALL_HASH == YES)

#include "platform.h"      
#include "hash.h"        

//   -------------- 
//  |g_hashAllocMem|->  |  pNext    |-----> |  pNext    |--->|  pNext   |--->NULL
//   -------------- 
//              Null<---|   pNext   |       |           |    |          |   
//                  |-->|-----------|       |           |    |          |   
//                  |   |   nKey    |       |           |    |          |
//                  |   |-----------|       |   已      |    |  已      |   
//                  |   |   pData   |       |   分      |    |  分      |
//                  |   |-----------|       |   配      |    |  配      |
//                  ----|   pNext   |       |   的      |    |  的      |
//                  |-->|-----------|       |  Hash     |    |  hash    |
//                  |   |   nKey    |       |   节      |    |  节      |
//                  |   |-----------|       |   点      |    |  点      |
//                  |   |   pData   |       |           |    |          |
//                  |   |-----------|       |           |    |          |
//                  ----|   pNext   |       |           |    |          |
//                      |-----------|       |           |    |          |
//                          .               |           |    |          |
//                  |->     .               |           |    |          |
//                  ----|   .       |       |           |    |          |
//  ---------------     |-----------|       |           |    |          |
//  |g_hashFreeNode|->  |   pNext   |       |           |    |          |
//  ---------------     -------------       -------------    ------------
//可变结构数据管理

//空闲Hash表节点链表
STATIC THashNode *g_hashFreeNode = NULL;

//Hash表中的节点个数
STATIC DWORD g_hashNodeCount = 0;

//可变内存区的首指针
STATIC TVMem *g_hashAllocMem=NULL;

//定义文件内部使用的函数
STATIC THashNode *Hash_GetNode();
STATIC void Hash_FreeNode(THashNode *pNode);

/*------------------------------------------------------------------------------------------*/
/* 功能: 分配空闲HASH节点,插入HASH节点时调用                                                */
/* 输入:无                                                                                  */
/* 输出:无                                                                                  */
/* 返回:分配的空闲HASH节点指针，=NULL 分配失败                                              */
/*------------------------------------------------------------------------------------------*/
THashNode *Hash_GetNode()
{
    THashNode *pHashNode;

    OSLockM(HASH_LOCK);

    //判断当前空闲Hash节点链是否为空
    if(g_hashFreeNode == NULL)
    {
        //空闲节点链为空，分配一块MAX_HASH_NODE空闲节点的内存，并链接成表
        //调用内存分配函数，分配内存多分配四个字节，用于把所有为空闲节点链分配的内存都链接起来
        pHashNode = (THashNode*)DF_GetVMem(&g_hashAllocMem,MAX_HASH_NODE,sizeof(THashNode));

        //把分配的MAX_HASH_NODE个空闲Hash节点链接到一起
        for(int i=0;i<MAX_HASH_NODE;i++,pHashNode++)
            INSERT_HEAD(g_hashFreeNode,pHashNode);
    }
    
    //hash空闲节点表不能为空
    ASSERT(g_hashFreeNode != NULL);

    //把hash空闲节点链表的首节点出链
    REMOVE_HEAD(g_hashFreeNode,pHashNode);

    g_hashNodeCount++;

    DF_UnLockM(HASH_LOCK);

    //返回分配的空闲节点指针
    return pHashNode;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 把空闲HASH节点链入空闲节点链，删除HASH节点时调用                                   */
/* 输入: pNode 要释放的节点指针                                                             */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void Hash_FreeNode(THashNode *pNode)
{
    DF_LockM(HASH_LOCK);

    //把pNode节点链入空闲HASH链
    INSERT_HEAD(g_hashFreeNode,pNode);
    
    //分配的空闲HASH节点-1
    g_hashNodeCount--;
    ASSERT(g_hashNodeCount >= 0);
    
    //判断从空闲链中分配的节点个数是否为零
    if(g_hashNodeCount == 0)
    {
        //从空闲链中分配的节点个数为0，则释放空闲节点内存
        DF_FreeVMem(g_hashAllocMem);
        
        //把HASH内存、HASH分配节点计数、HASH空闲节点链置为初始值
        g_hashAllocMem = NULL;
        g_hashNodeCount = 0;
        g_hashFreeNode = NULL;
    }

    DF_UnLockM(HASH_LOCK);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 创建HASH表                                                                         */
/* 输入: size HASH表的大小；fp 该HASH表的HASH函数指针                                       */
/* 输出: 无                                                                                 */
/* 返回: HASH表指针                                                                         */
/*------------------------------------------------------------------------------------------*/
THashTable* Hash_CreateTable(DWORD size,THashValue fp)
{
    ASSERT(size != 0);
    THashTable *pTable;
    //分配HASH表头和HASH表内存
    pTable = (THashTable*)DF_MemAlloc(sizeof(THashTable)+size*sizeof(THashNode*));
    
    ASSERT(pTable);

    //初始化HASH表项指针
    pTable->m_ppTable = (THashNode**)(pTable+1);
    pTable->m_nHashSize = size;     //HASH表大小
    pTable->m_nCount = 0;           //HASH表节点计数
    pTable->m_pfHashValue = fp;     //HASH函数指针

    //初始化Hash表
    for(int i=0;i<pTable->m_nHashSize;i++)
        pTable->m_ppTable[i] = NULL;
    
    //返回Hash表首指针
    return pTable;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 根据关键字计算HASH表的HASH值                                                       */
/* 输入: pht HASH表指针，key关键字                                                          */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的HASH值                                                               */
/*------------------------------------------------------------------------------------------*/
DWORD Hash_Value(THashTable *pht,DWORD key)
{
    DWORD theKey;
    
    ASSERT(pht != NULL);
    ASSERT(pht->m_nHashSize);

    //如果该HASH表的HASH函数为NULL，则使用系统缺省HASH函数
    if(pht->m_pfHashValue == NULL)
        theKey = _HASH_VALUE(key)%pht->m_nHashSize;
    else 
		theKey = pht->m_pfHashValue(key)%pht->m_nHashSize;
    return theKey;
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 向HASH表中增加一个节点                                                             */
/* 输入: pht HASH表指针，key 该节点的关键字,val 该节点的值                                  */
/* 输出:无                                                                                  */
/* 返回:                                                                                    */
/*------------------------------------------------------------------------------------------*/
void Hash_AddObject(THashTable *pht,DWORD key,void *val)
{
    ASSERT(pht != NULL);
    
    //计算关键字key对应的Hash值
    DWORD ind = Hash_Value(pht,key);

    //分配空闲Hash表节点
    THashNode *pNode = Hash_GetNode();

    //给Hash节点赋值
    pNode->pData = val;
    pNode->nKey = key;
    pNode->pNext = NULL;

    //判断关键字对应的Hash表入口是否为空，即判断是否有相同hash值的节点存在
    if(pht->m_ppTable[ind] != NULL) 
    {
        //对应的hash表入口不为空，把该节点插入到相应的hash表入口的链表头
        INSERT_HEAD(pht->m_ppTable[ind],pNode);
        //pNode->pNext = ppht[ind];
        //ppht[ind] = pNode;
    }
    else
        //对应的hash表入口不为空，对应的hash表入口指针指向该节点
        pht->m_ppTable[ind] = pNode;
    pht->m_nCount++;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 从HASH表中删除指定关键字的节点                                                     */
/* 输入: pht HASH表指针,key 需要删除的关键字                                                */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的节点值                                                               */
/*------------------------------------------------------------------------------------------*/
void* Hash_DeleteObject(THashTable *pht,DWORD key)
{
    THashNode *pNode;
    THashNode **ppPrev;
    void *pData;
    
    ASSERT(pht != NULL);
        
    DWORD ind = Hash_Value(pht,key);  //计算key对应的HASH值

    //找到对应的HASH表项入口
    ppPrev = &pht->m_ppTable[ind];

    //在相应的HASH表项中查找关键字为key的节点
    for(pNode = *ppPrev;pNode != NULL;pNode = pNode->pNext)
    {
        if(pNode->nKey == key)
        {
            //找到关键字为key的节点，从HASH表中删除
            *ppPrev = pNode->pNext;
            pData = pNode->pData;

            //释放该HASH节点
            Hash_FreeNode(pNode);
            pht->m_nCount--;
            
            //返回该节点对应值
            return pData;
        }
        ppPrev = &pNode->pNext;
    }
    //在HASH表中未找到该关键字，返回NULL
    return NULL;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 在HASH表中查找关键字key对应的值                                                    */
/* 输入: pht HASH表指针，key 要查找的关键字                                                 */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的值，=NULL HASH表中，未找到该关键字                                   */
/*------------------------------------------------------------------------------------------*/
void *Hash_LookUp(THashTable *pht,DWORD key)
{
    THashNode *pNode;

    ASSERT(pht != NULL);
    
    //计算HASH值
    DWORD ind = Hash_Value(pht,key);
    pNode = pht->m_ppTable[ind];

    //在HASH值的表项中查找该关键字对应的节点
    while((pNode != NULL)&&(pNode->nKey != key))
        pNode = pNode->pNext;
    
    //如果未找到关键字KEY，返回NULL
    if(pNode == NULL)
        return NULL;

    //找到关键字KEY，返回该关键字对应的值
    return pNode->pData;
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 取HASH表的叠代首指针                                                               */
/* 输入: pht HASH表指针                                                                     */
/* 输出: 无                                                                                 */
/* 返回: HASH表的叠代首指针，=NULL该HASH表为空                                              */
/*------------------------------------------------------------------------------------------*/
ITERATE Hash_HeadIterate(THashTable *pht)
{
    ASSERT(pht != NULL);
    
    if(pht->m_nCount==0)
        return NULL;
    return (ITERATE_BEGIN);
}
/*------------------------------------------------------------------------------------------*/
/* 功能: 取叠代指针对应的HASH节点，同时更新叠代指针                                         */
/* 输入: pht HASH表指针，iterate 叠代指针                                                   */
/* 输出: iterate 下一个叠代指针                                                             */
/* 返回: 该叠代指针对应的值                                                                 */
/*------------------------------------------------------------------------------------------*/
void* Hash_GetNext(THashTable *pht,ITERATE& iterate)
{
    THashNode *pNextHash;
    THashNode *pNode = (THashNode *)(iterate);
    ASSERT(pht != NULL);
    //判断该叠代指针是否为HASH表的叠代首指针
    if(pNode == ITERATE_BEGIN)
    {
        //是叠代首指针，在HASH表中找到第一个不为NULL的节点
        for(int i=0;i<pht->m_nHashSize;i++)
        {
            if(pht->m_ppTable[i] != NULL)
            {
                pNode = pht->m_ppTable[i];
                break;
            }
        }
    }

    //更新叠代指针，指向下一个不为NULL的HASH节点
    pNextHash = pNode->pNext;
    if(pNextHash == NULL)
    {
        for(int i=Hash_Value(pht,pNode->nKey)+1;i<pht->m_nHashSize;i++)
        {
            if(pht->m_ppTable[i] != NULL)
            {
                pNextHash = pht->m_ppTable[i];
                break;
            }
        }

    }
    iterate = (ITERATE)pNextHash;

    //返回叠代指针对应的值
    return (void*)pNode->pData;
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 释放HASH表资源，包括HASH表内存和HASH节点内存                                       */
/* 输入: 无                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void Hash_DeleteTable(THashTable *pht)
{
    THashNode *pNode,*pNext;
    void* pData;

    for(int i=0;i<pht->m_nHashSize;i++)
    {
        if(pht->m_ppTable[i] != NULL)
        {
            pNode = pht->m_ppTable[i];
            while(pNode != NULL)
            {
                pNext = pNode->pNext;
                pData = pNode->pData;
                Hash_FreeNode(pNode);
                DF_MemFree(pData);
                pNode = pNext;
            }
            break;
        }
    }
    DF_MemFree(pht);
}

/*------------------------------------------------------------------------------------------*/
/* 功能: 取HASH表的节点个数                                                     */
/* 输入: pht HASH表指针                                                */
/* 输出: 无                                                                                 */
/* 返回: 该HASH表的节点个数                                                               */
/*------------------------------------------------------------------------------------------*/
int Hash_GetObjectNum(THashTable *pht)
{
	return	pht->m_nCount;
}

#endif        
