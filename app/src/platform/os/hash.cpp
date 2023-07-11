#if (INSTALL_HASH == YES)

#include "platform.h"      
#include "hash.h"        

//   -------------- 
//  |g_hashAllocMem|->  |  pNext    |-----> |  pNext    |--->|  pNext   |--->NULL
//   -------------- 
//              Null<---|   pNext   |       |           |    |          |   
//                  |-->|-----------|       |           |    |          |   
//                  |   |   nKey    |       |           |    |          |
//                  |   |-----------|       |   ��      |    |  ��      |   
//                  |   |   pData   |       |   ��      |    |  ��      |
//                  |   |-----------|       |   ��      |    |  ��      |
//                  ----|   pNext   |       |   ��      |    |  ��      |
//                  |-->|-----------|       |  Hash     |    |  hash    |
//                  |   |   nKey    |       |   ��      |    |  ��      |
//                  |   |-----------|       |   ��      |    |  ��      |
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
//�ɱ�ṹ���ݹ���

//����Hash��ڵ�����
STATIC THashNode *g_hashFreeNode = NULL;

//Hash���еĽڵ����
STATIC DWORD g_hashNodeCount = 0;

//�ɱ��ڴ�������ָ��
STATIC TVMem *g_hashAllocMem=NULL;

//�����ļ��ڲ�ʹ�õĺ���
STATIC THashNode *Hash_GetNode();
STATIC void Hash_FreeNode(THashNode *pNode);

/*------------------------------------------------------------------------------------------*/
/* ����: �������HASH�ڵ�,����HASH�ڵ�ʱ����                                                */
/* ����:��                                                                                  */
/* ���:��                                                                                  */
/* ����:����Ŀ���HASH�ڵ�ָ�룬=NULL ����ʧ��                                              */
/*------------------------------------------------------------------------------------------*/
THashNode *Hash_GetNode()
{
    THashNode *pHashNode;

    OSLockM(HASH_LOCK);

    //�жϵ�ǰ����Hash�ڵ����Ƿ�Ϊ��
    if(g_hashFreeNode == NULL)
    {
        //���нڵ���Ϊ�գ�����һ��MAX_HASH_NODE���нڵ���ڴ棬�����ӳɱ�
        //�����ڴ���亯���������ڴ������ĸ��ֽڣ����ڰ�����Ϊ���нڵ���������ڴ涼��������
        pHashNode = (THashNode*)DF_GetVMem(&g_hashAllocMem,MAX_HASH_NODE,sizeof(THashNode));

        //�ѷ����MAX_HASH_NODE������Hash�ڵ����ӵ�һ��
        for(int i=0;i<MAX_HASH_NODE;i++,pHashNode++)
            INSERT_HEAD(g_hashFreeNode,pHashNode);
    }
    
    //hash���нڵ����Ϊ��
    ASSERT(g_hashFreeNode != NULL);

    //��hash���нڵ�������׽ڵ����
    REMOVE_HEAD(g_hashFreeNode,pHashNode);

    g_hashNodeCount++;

    DF_UnLockM(HASH_LOCK);

    //���ط���Ŀ��нڵ�ָ��
    return pHashNode;
}

/*------------------------------------------------------------------------------------------*/
/* ����: �ѿ���HASH�ڵ�������нڵ�����ɾ��HASH�ڵ�ʱ����                                   */
/* ����: pNode Ҫ�ͷŵĽڵ�ָ��                                                             */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
void Hash_FreeNode(THashNode *pNode)
{
    DF_LockM(HASH_LOCK);

    //��pNode�ڵ��������HASH��
    INSERT_HEAD(g_hashFreeNode,pNode);
    
    //����Ŀ���HASH�ڵ�-1
    g_hashNodeCount--;
    ASSERT(g_hashNodeCount >= 0);
    
    //�жϴӿ������з���Ľڵ�����Ƿ�Ϊ��
    if(g_hashNodeCount == 0)
    {
        //�ӿ������з���Ľڵ����Ϊ0�����ͷſ��нڵ��ڴ�
        DF_FreeVMem(g_hashAllocMem);
        
        //��HASH�ڴ桢HASH����ڵ������HASH���нڵ�����Ϊ��ʼֵ
        g_hashAllocMem = NULL;
        g_hashNodeCount = 0;
        g_hashFreeNode = NULL;
    }

    DF_UnLockM(HASH_LOCK);
}

/*------------------------------------------------------------------------------------------*/
/* ����: ����HASH��                                                                         */
/* ����: size HASH��Ĵ�С��fp ��HASH���HASH����ָ��                                       */
/* ���: ��                                                                                 */
/* ����: HASH��ָ��                                                                         */
/*------------------------------------------------------------------------------------------*/
THashTable* Hash_CreateTable(DWORD size,THashValue fp)
{
    ASSERT(size != 0);
    THashTable *pTable;
    //����HASH��ͷ��HASH���ڴ�
    pTable = (THashTable*)DF_MemAlloc(sizeof(THashTable)+size*sizeof(THashNode*));
    
    ASSERT(pTable);

    //��ʼ��HASH����ָ��
    pTable->m_ppTable = (THashNode**)(pTable+1);
    pTable->m_nHashSize = size;     //HASH���С
    pTable->m_nCount = 0;           //HASH��ڵ����
    pTable->m_pfHashValue = fp;     //HASH����ָ��

    //��ʼ��Hash��
    for(int i=0;i<pTable->m_nHashSize;i++)
        pTable->m_ppTable[i] = NULL;
    
    //����Hash����ָ��
    return pTable;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ���ݹؼ��ּ���HASH���HASHֵ                                                       */
/* ����: pht HASH��ָ�룬key�ؼ���                                                          */
/* ���: ��                                                                                 */
/* ����: �ùؼ��ֶ�Ӧ��HASHֵ                                                               */
/*------------------------------------------------------------------------------------------*/
DWORD Hash_Value(THashTable *pht,DWORD key)
{
    DWORD theKey;
    
    ASSERT(pht != NULL);
    ASSERT(pht->m_nHashSize);

    //�����HASH���HASH����ΪNULL����ʹ��ϵͳȱʡHASH����
    if(pht->m_pfHashValue == NULL)
        theKey = _HASH_VALUE(key)%pht->m_nHashSize;
    else 
		theKey = pht->m_pfHashValue(key)%pht->m_nHashSize;
    return theKey;
}
/*------------------------------------------------------------------------------------------*/
/* ����: ��HASH��������һ���ڵ�                                                             */
/* ����: pht HASH��ָ�룬key �ýڵ�Ĺؼ���,val �ýڵ��ֵ                                  */
/* ���:��                                                                                  */
/* ����:                                                                                    */
/*------------------------------------------------------------------------------------------*/
void Hash_AddObject(THashTable *pht,DWORD key,void *val)
{
    ASSERT(pht != NULL);
    
    //����ؼ���key��Ӧ��Hashֵ
    DWORD ind = Hash_Value(pht,key);

    //�������Hash��ڵ�
    THashNode *pNode = Hash_GetNode();

    //��Hash�ڵ㸳ֵ
    pNode->pData = val;
    pNode->nKey = key;
    pNode->pNext = NULL;

    //�жϹؼ��ֶ�Ӧ��Hash������Ƿ�Ϊ�գ����ж��Ƿ�����ͬhashֵ�Ľڵ����
    if(pht->m_ppTable[ind] != NULL) 
    {
        //��Ӧ��hash����ڲ�Ϊ�գ��Ѹýڵ���뵽��Ӧ��hash����ڵ�����ͷ
        INSERT_HEAD(pht->m_ppTable[ind],pNode);
        //pNode->pNext = ppht[ind];
        //ppht[ind] = pNode;
    }
    else
        //��Ӧ��hash����ڲ�Ϊ�գ���Ӧ��hash�����ָ��ָ��ýڵ�
        pht->m_ppTable[ind] = pNode;
    pht->m_nCount++;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��HASH����ɾ��ָ���ؼ��ֵĽڵ�                                                     */
/* ����: pht HASH��ָ��,key ��Ҫɾ���Ĺؼ���                                                */
/* ���: ��                                                                                 */
/* ����: �ùؼ��ֶ�Ӧ�Ľڵ�ֵ                                                               */
/*------------------------------------------------------------------------------------------*/
void* Hash_DeleteObject(THashTable *pht,DWORD key)
{
    THashNode *pNode;
    THashNode **ppPrev;
    void *pData;
    
    ASSERT(pht != NULL);
        
    DWORD ind = Hash_Value(pht,key);  //����key��Ӧ��HASHֵ

    //�ҵ���Ӧ��HASH�������
    ppPrev = &pht->m_ppTable[ind];

    //����Ӧ��HASH�����в��ҹؼ���Ϊkey�Ľڵ�
    for(pNode = *ppPrev;pNode != NULL;pNode = pNode->pNext)
    {
        if(pNode->nKey == key)
        {
            //�ҵ��ؼ���Ϊkey�Ľڵ㣬��HASH����ɾ��
            *ppPrev = pNode->pNext;
            pData = pNode->pData;

            //�ͷŸ�HASH�ڵ�
            Hash_FreeNode(pNode);
            pht->m_nCount--;
            
            //���ظýڵ��Ӧֵ
            return pData;
        }
        ppPrev = &pNode->pNext;
    }
    //��HASH����δ�ҵ��ùؼ��֣�����NULL
    return NULL;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ��HASH���в��ҹؼ���key��Ӧ��ֵ                                                    */
/* ����: pht HASH��ָ�룬key Ҫ���ҵĹؼ���                                                 */
/* ���: ��                                                                                 */
/* ����: �ùؼ��ֶ�Ӧ��ֵ��=NULL HASH���У�δ�ҵ��ùؼ���                                   */
/*------------------------------------------------------------------------------------------*/
void *Hash_LookUp(THashTable *pht,DWORD key)
{
    THashNode *pNode;

    ASSERT(pht != NULL);
    
    //����HASHֵ
    DWORD ind = Hash_Value(pht,key);
    pNode = pht->m_ppTable[ind];

    //��HASHֵ�ı����в��Ҹùؼ��ֶ�Ӧ�Ľڵ�
    while((pNode != NULL)&&(pNode->nKey != key))
        pNode = pNode->pNext;
    
    //���δ�ҵ��ؼ���KEY������NULL
    if(pNode == NULL)
        return NULL;

    //�ҵ��ؼ���KEY�����ظùؼ��ֶ�Ӧ��ֵ
    return pNode->pData;
}
/*------------------------------------------------------------------------------------------*/
/* ����: ȡHASH��ĵ�����ָ��                                                               */
/* ����: pht HASH��ָ��                                                                     */
/* ���: ��                                                                                 */
/* ����: HASH��ĵ�����ָ�룬=NULL��HASH��Ϊ��                                              */
/*------------------------------------------------------------------------------------------*/
ITERATE Hash_HeadIterate(THashTable *pht)
{
    ASSERT(pht != NULL);
    
    if(pht->m_nCount==0)
        return NULL;
    return (ITERATE_BEGIN);
}
/*------------------------------------------------------------------------------------------*/
/* ����: ȡ����ָ���Ӧ��HASH�ڵ㣬ͬʱ���µ���ָ��                                         */
/* ����: pht HASH��ָ�룬iterate ����ָ��                                                   */
/* ���: iterate ��һ������ָ��                                                             */
/* ����: �õ���ָ���Ӧ��ֵ                                                                 */
/*------------------------------------------------------------------------------------------*/
void* Hash_GetNext(THashTable *pht,ITERATE& iterate)
{
    THashNode *pNextHash;
    THashNode *pNode = (THashNode *)(iterate);
    ASSERT(pht != NULL);
    //�жϸõ���ָ���Ƿ�ΪHASH��ĵ�����ָ��
    if(pNode == ITERATE_BEGIN)
    {
        //�ǵ�����ָ�룬��HASH�����ҵ���һ����ΪNULL�Ľڵ�
        for(int i=0;i<pht->m_nHashSize;i++)
        {
            if(pht->m_ppTable[i] != NULL)
            {
                pNode = pht->m_ppTable[i];
                break;
            }
        }
    }

    //���µ���ָ�룬ָ����һ����ΪNULL��HASH�ڵ�
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

    //���ص���ָ���Ӧ��ֵ
    return (void*)pNode->pData;
}

/*------------------------------------------------------------------------------------------*/
/* ����: �ͷ�HASH����Դ������HASH���ڴ��HASH�ڵ��ڴ�                                       */
/* ����: ��                                                                                 */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
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
/* ����: ȡHASH��Ľڵ����                                                     */
/* ����: pht HASH��ָ��                                                */
/* ���: ��                                                                                 */
/* ����: ��HASH��Ľڵ����                                                               */
/*------------------------------------------------------------------------------------------*/
int Hash_GetObjectNum(THashTable *pht)
{
	return	pht->m_nCount;
}

#endif        
