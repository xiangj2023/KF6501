#ifndef _SCDEFS_H_
#define _SCDEFS_H_

#include "resm5234.h"
#include "sysdefs.h"

//定义模块中各种类型板的最大配置，其中混合类型板如:遥信/遥控板根据功能各算一块。
#define MAX_AI_BOARD_NUM	3
#define MAX_BO_BOARD_NUM	3
#define MAX_DI_BOARD_NUM	3

//定义采集最大配置,注意必须保证是对应类型端口宽度的整倍数
#define MAX_AI_NUM			96
#define MAX_BO_NUM			72
#define MAX_DI_NUM			160

//定义单个遥控板最大遥控点数目
#define MAX_BO_NUM_PER_BOARD	24	//遥控启动占用1路

//定义单个遥控板的内部遥信路数
#define MAX_IN_DI_NUM_PER_BOARD	12	

//定义单个遥信板最大路数
#define MAX_DI_NUM_PER_BOARD	34

//定义各个类型端口宽度
#define AI_BOARD_WIDTH		16
#define BO_BOARD_WIDTH		8
#define DI_BOARD_WIDTH		8

//根据模块类型定义板的配置，以及各个板对应地址和路数
#if(MODULE_TYPE == MD_KV3523)

	//定义各种板的配置个数
	#define AI_BOARD_NUM	1
	#define BO_BOARD_NUM	1
	#define DI_BOARD_NUM	1

	//定义各个AI板的地址及采集路数,定义时根据满配置定义，没有的定义为0
	#define AI_BOARD1_ADDR		0x60000000
	#define AI_BOARD1_AINUM		12
	#define AI_BOARD2_ADDR		0
	#define AI_BOARD2_AINUM		0
	#define AI_BOARD3_ADDR		0
	#define AI_BOARD3_AINUM		0

	//定义各个BO板地址及路数
	#define BO_BOARD1_ADDR		0x60002000
	#define BO_BOARD1_BONUM		24
	#define BO_BOARD2_ADDR		0
	#define BO_BOARD2_BONUM		0
	#define BO_BOARD3_ADDR		0
	#define BO_BOARD3_BONUM		0

	//定义各个DI板地址及路数	
	#define DI_BOARD1_ADDR		BIA_CS_BASE
	#define DI_BOARD1_DINUM		34
	#define DI_BOARD2_ADDR		0
	#define DI_BOARD2_DINUM 	0
	#define DI_BOARD3_ADDR		0
	#define DI_BOARD3_DINUM 	0
	
#endif


#endif
