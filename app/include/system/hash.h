
#ifndef __HASH_H
#define __HASH_H

#include "sysdefs.h"

//Hash表节点定义
struct THashNode
{
    THashNode *pNext;       //指向下一个Hash节点
    void *pData;            //Hash节点的数据区
    DWORD nKey;             //Hash节点的索引关键字
};

//当空闲HASH节点链表为空时，每次申请的空闲节点个数
#define MAX_HASH_NODE (256/sizeof(THashNode))

//Hash表的缺省大小
#define HASH_TABLE_SIZE   64

//hash函数指针类型定义
typedef DWORD (*THashValue)(DWORD);

//Hash表数据结构定义
struct THashTable{
    int m_nHashSize;
    int m_nCount;
    THashNode **m_ppTable;
    THashValue m_pfHashValue;
};

// 缺省hash函数
#define _HASH_VALUE(key)  ((key)&0xFFFF)

/*------------------------------------------------------------------------------------------*/
/* 功能: 创建HASH表                                                                         */
/* 输入: size HASH表的大小；fp 该HASH表的HASH函数指针                                       */
/* 输出: 无                                                                                 */
/* 返回: HASH表指针                                                                         */
/*------------------------------------------------------------------------------------------*/
THashTable* Hash_CreateTable(DWORD size=HASH_TABLE_SIZE,THashValue fp = NULL);

/*------------------------------------------------------------------------------------------*/
/* 功能: 向HASH表中增加一个节点                                                             */
/* 输入: pht HASH表指针，key 该节点的关键字,val 该节点的值                                  */
/* 输出:无                                                                                  */
/* 返回:                                                                                    */
/*------------------------------------------------------------------------------------------*/
void Hash_AddObject(THashTable *pht,DWORD key,void *val);

/*------------------------------------------------------------------------------------------*/
/* 功能: 从HASH表中删除指定关键字的节点                                                     */
/* 输入: pht HASH表指针,key 需要删除的关键字                                                */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的节点值                                                               */
/*------------------------------------------------------------------------------------------*/
void* Hash_DeleteObject(THashTable *pht,DWORD key);

/*------------------------------------------------------------------------------------------*/
/* 功能: 在HASH表中查找关键字key对应的值                                                    */
/* 输入: pht HASH表指针，key 要查找的关键字                                                 */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的值，=NULL HASH表中，未找到该关键字                                   */
/*------------------------------------------------------------------------------------------*/
void *Hash_LookUp(THashTable *pht,DWORD key);

/*------------------------------------------------------------------------------------------*/
/* 功能: 根据关键字计算HASH表的HASH值                                                       */
/* 输入: pht HASH表指针，key关键字                                                          */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的HASH值                                                               */
/*------------------------------------------------------------------------------------------*/
DWORD Hash_Value(THashTable *pht,DWORD key);

/*------------------------------------------------------------------------------------------*/
/* 功能: 在HASH表中查找关键字key对应的值                                                    */
/* 输入: pht HASH表指针，key 要查找的关键字                                                 */
/* 输出: 无                                                                                 */
/* 返回: 该关键字对应的值，=NULL HASH表中，未找到该关键字                                   */
/*------------------------------------------------------------------------------------------*/
ITERATE Hash_HeadIterate(THashTable *pht);

/*------------------------------------------------------------------------------------------*/
/* 功能: 取叠代指针对应的HASH节点，同时更新叠代指针                                         */
/* 输入: pht HASH表指针，iterate 叠代指针                                                   */
/* 输出: iterate 下一个叠代指针                                                             */
/* 返回: 该叠代指针对应的值                                                                 */
/*------------------------------------------------------------------------------------------*/
void* Hash_GetNext(THashTable *pht,ITERATE& iterate);

/*------------------------------------------------------------------------------------------*/
/* 功能: 释放HASH表资源，包括HASH表内存和HASH节点内存                                       */
/* 输入: 无                                                                                 */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
/*------------------------------------------------------------------------------------------*/
void Hash_DeleteTable(THashTable *pht);
/*------------------------------------------------------------------------------------------*/
/* 功能: 取HASH表的节点个数                                                     */
/* 输入: pht HASH表指针                                                */
/* 输出: 无                                                                                 */
/* 返回: 该HASH表的节点个数                                                               */
/*------------------------------------------------------------------------------------------*/
int Hash_GetObjectNum(THashTable *pht);

#endif

       
