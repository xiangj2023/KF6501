#ifndef _DI_H_
#define _DI_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbasedef.h"
#include "dbfunc.h"
#include "parafunc.h"

/********************************************************************************
		DI处理临时定义
********************************************************************************/
//模块DI配置信息
struct TDICfg
{
	BYTE bDiBoardNum;		//DI板个数
	DWORD dwDiBoardAddr[MAX_DI_BOARD_NUM];	//各个DI板基地址
	BYTE bDiNumPerBoard[MAX_DI_BOARD_NUM];	//每个DI板包含的DI路数
};

//定义脉冲电度刷新方式
//#define DB_IMCREMENT   		0x01                                                           
//#define DB_UPDATE      		0x02

//DI位属性定义
#define DI_IS_CI			0x01	//是电度
#define DI_IS_REVERSE		0x02	//取反
#define DI_CREAT_SOE		0x04	//是否产生SOE

/********************************************************************************
		DI处理临时定义结束
********************************************************************************/

#define DI_TIMER_PERIOD		10		//定义DI定时器时间间隔为10毫秒

//定义遥信板识别码地址定义
#define DI_BOARD_ID_ADD		0x04	
#define DI_BOARD_ID			0xa8


//定义去抖时间,单位为0.5MS，即遥信采集定时器的时间间隔
#define MAX_SMOOTH_TIME			5000
#define DEFAULT_BI_SMOOTH_TIME	(40*2)		//缺省遥信去抖时间
#define DEFAULT_CI_SMOOTH_TIME	(50*2)		//缺省电度去抖时间

//SOE临时缓冲区长度定义
#define SOE_BUF_LEN		64

//DI变位临时缓冲区
#define DI_BUF_LEN		128

//定义DI采集处理各个状态
#define DI_STEP_0		0
#define DI_STEP_3		3
#define DI_STEP_4		4
#define DI_STEP_5		5
#define DI_STEP_6		6
#define DI_STEP_9		9
#define DI_STEP_10		10
#define DI_STEP_11		11

//定义遥信类型
#define MAX_DI_TYPE		3
#define REAL_DI			0
#define INNER_DI		1
#define VIR_DI			2

struct TDIType{
	WORD wDIBitNum[MAX_DI_TYPE];	//每种类型DI位的个数
	WORD wDIPortBase[MAX_DI_TYPE];	//每种类型DI位所占的端口地址的起始地址
	WORD wDIPortNum;				//所有类型DI共占有的端口地址数	
};

//DI变化记录缓冲，记录的是同一个字节端口地址上同时发生变位的多个位的信息
struct TDIBuf{
	BYTE		bDIStatus;		//DI当前状态，注意是一个端口地址上的所有位信息
	BYTE		bDIPortNo;		//发生状态变化的端口号
	BYTE 		bDIChgBit;		//发生变化的位信息(该位为1说明该位发生了变位)
	DWORD		dwDITimeCnt;	//发生变化时DI定时器计数值，用来生成毫秒时间记录
	TAbsTime	Time;			//发生变化时的系统时间
};

//DI位信息结构定义
struct TDIBitData
{
	BYTE bStatusTab;		//DI位状态表
	BYTE bAttribute;		//DI位属性，包括遥信还是电度、是否取反等
	WORD wIndex;			//DI索引表,此表为遥信或脉冲的实际序号，需要根据参数初始化
	DWORD dwSmoothTime;		//DI消颤去抖时间
	DWORD dwKeepTime;		//DI状态保持时间
	TAbsTime qwChangeTime;		//DI状态变化时刻
};

//DI端口方式结构定义(一个实际板可能需要分为多个8位端口的信息结构)，由于每个DI逻辑板DI个数不一定
//是8的整数倍(如34路DI)，因此需要知道每一个字节端口上真正有效的DI位数。
struct TDIPortData
{
	DWORD dwPortAddr;		//本端口对应的物理地址
	BYTE bBiNumInPort;		//本端口中有效DI位的个数
	BYTE bOldStatus;		//老的DI状态
//	BYTE bTaskOldStatus;	//任务中老的DI状态
};

//DI采集处理类定义
class CDIProc:public CApp
{
protected:
    DECLARE_MESSAGE_MAP()
		
public:
	
	TEventBI *pSoeBuf;		//SOE临时缓冲，减少写数据库的频率
	WORD wSoeReadPtr;		//SOE临时缓冲读指针
	WORD wSoeWritePtr;		//SOE临时缓冲写指针
	
	CDIProc(DWORD *pdwApp);
	~CDIProc();
	/*************************************************************************************
	* DI初始化														  
	*************************************************************************************/
	void Init();
	
	/*************************************************************************************
	* DI基于字节端口的参数初始化														  
	*************************************************************************************/
	void DIPortInit();

	/*************************************************************************************
	* DI基于位的参数初始化														  
	*************************************************************************************/
	void DIBitInit();

	/*************************************************************************************
	* DI初始状态确定														  
	*************************************************************************************/
	void BIStatusInit(BYTE type);

	/*************************************************************************************
	* DI变位消息响应														  
	*************************************************************************************/
	void OnDIChange(DWORD id);

	/*************************************************************************************
	* DI定时消息响应														  
	*************************************************************************************/
	void OnTimeOut(DWORD id);

	/*************************************************************************************
	* DI采集任务异常处理														  
	*************************************************************************************/
	void ErrorProc();
	
	/*************************************************************************************
	*  计算指定字节端口序号所对应的总的DI序号														  
	*************************************************************************************/
	WORD GetDIIndex(BYTE bPortNo);
	/*************************************************************************************
	*实时修改BI参数生效
	**************************************************************************************/
	void OnBiParaChange(DWORD id);
};

void DITask(DWORD *pdwApp);

void DITimerInt();
STATUS ScanDIPort(BYTE type);


#endif
