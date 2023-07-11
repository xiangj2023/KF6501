/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      Db_err.h                                     1.0												*/
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义了数据库函数返回值																	*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      l.z																								*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* 函数																									*/
/*																										*/
/*      None																							*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*      None																							*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-03-15      初始创建,版本号1.0												*/
/********************************************************************************************************/
#ifndef __DB_ERR_H
#define __DB_ERR_H

#define DB_ERR_OK              	0x00000000
#define DB_ERR_FAILURE          0x00000001

#define DB_ERR_ID              	0x00000100
#define DB_ERR_NO      			0x00000101
#define DB_ERR_BUFNULL        	0x00000102
#define DB_ERR_BASENULL			0x00000103
#define DB_ERR_PTRNULL        	0x00000104
#define	DB_ERR_NOSUPPORT		0x00000105
#define DB_ERR_RANGE      		0x00000106
#define DB_ERR_CHECK      		0x00000107

#define DB_ERR_MEM_ALLOC       	0x00000107
#define DB_ERR_MEM_FREE        	0x00000108
#define DB_ERR_NVMEM_ALLOC     	0x00000109
#define DB_ERR_NVMEM_ADDR   	0x00000110

#define DB_ERR_EEPROM      		0x00000111

#define DB_ERR_AO_NO           	0x00000200
#define DB_ERR_AO_NOSELECT     	0x00000201
#define DB_ERR_BO_NO           	0x00000202
#define DB_ERR_BO_NOSELECT     	0x00000203
#define DB_ERR_BO_LOCK         	0x00000204

#define DB_ERR_TQ_NO           	0x00000205
#define DB_ERR_TQ_NOSELECT     	0x00000206

#define DB_ERR_WRAP_INIT		0x00000207       

#define DB_ERR_RELAY_SEQ        0x00000208
#define DB_ERR_RELAY_INF        0x00000209
#define DB_ERR_RELAY_FD        	0x00000210
#define DB_ERR_RELAY_NOCACHE    0x00000211
#define DB_ERR_RELAY_OVERFLOW   0x00000212
#define DB_ERR_RELAY_STATUS     0x00000213

#define DB_ERR_RELAY_SET        0x00000214

#define DB_ERR_OPT_STATUS   	0x00000213
#define DB_ERR_OPT_TASK   		0x00000214
#define DB_ERR_OPT_PARA   		0x00000215
#define DB_ERR_OPT_SAVE   		0x00000216

#define DB_ERR_FILE_CREATE     	0x00000300
#define DB_ERR_FILE_OPEN       	0x00000301
#define DB_ERR_FILE_WRITE      	0x00000302
#define DB_ERR_FILE_READ       	0x00000303
#define DB_ERR_FILE_CHECK       0x00000304
#define DB_ERR_FILE_TYPE       	0x00000305

#define DB_ERR_MAP_NULL       	0x00000310

#endif
