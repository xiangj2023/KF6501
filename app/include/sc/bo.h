#ifndef _BO_H_
#define _BO_H_

#include "system.h"
#include "msgmap.h"
#include "scdefs.h"
#include "timedef.h"
#include "dbfunc.h"
#include "parafunc.h"

/********************************************************************************
		BO处理临时定义
********************************************************************************/
//模块BO配置信息
struct TBOCfg
{
	BYTE bBOBoardNum;		//BO板个数
	DWORD dwBOBoardAddr[MAX_BO_BOARD_NUM];	//各个BO板基地址
	BYTE bBONumPerBoard[MAX_BO_BOARD_NUM];	//每个BO板包含的BO路数
};

//临时定义闭锁继电器控制函数

#define BJ_RELAY_CTL(cmd)		outportb(0x60010000, cmd)

//遥控命令定义
//#define BO_OPEN				0x55
//#define BO_CLOSE			0xaa
//#define BO_CANCLE			0xff
//#define BO_INVALIDE			0x00

/********************************************************************************
		DI处理临时定义结束
********************************************************************************/

//根据状态是否保持来定义遥控操作状态
#define BO_CTL_IDLE			0	//遥控空闲状态
#define BO_CTL_SET			1	//遥控预置状态
#define BO_CTL_VER			2	//遥控反校
//#define BO_CTL_VER_END		3	//遥控反校结束
#define BO_CTL_EXECUTE		4	//遥控执行状态
//#define BO_CTL_EXE_END		5	//遥控执行完成
//#define BO_CTL_DIR_SET		6	//直接遥控预置状态
#define BO_CTL_DIR_EXECUTE	7	//直接遥控执行状态
//#define BO_CTL_END			8	//遥控操作结束，此状态为考虑继电器返回的时间延迟

//定义遥控操作错误信息
#define ERR_BO_SUCCESS		0
#define ERR_BO_POINT		1	//遥控点号不对
#define ERR_BO_CLOSE_TIME	2	//遥控吸合时间错误（下发时间为0且系统设定时间错误）
#define ERR_BO_SET_CHECK	3	//遥控预置反校错误
#define ERR_BO_BUSY			4	//该路遥控正在操作
#define ERR_BO_CMD			5	//遥控命令错误，如单独收到一个分预置继电器的操作
#define ERR_BO_SET_TIMEOUT	6	//反校超时

#define BO_TEST_INVALID		0xffff	//传动无效状态
#define BO_TEST_DELAY		(5000/BO_TASK_TIME)	//传动时间
/*
//定义遥控操作命令
#define BO_SET_OPEN_CMD		0x11
#define BO_SET_CLOSE_CMD	0x22
#define BO_EXE_OPEN_CMD		0x33
#define BO_EXE_CLOSE_CMD	0x44
#define BO_EXE_CANCEL_CMD	0x55
*/

//定义遥控预置反校字
#define BO_SET_CHECK_KEY	0x88

//定义BO任务定时间隔
#define BO_TASK_TIME		20		//毫秒

//定义遥控继电器延时		
#define BO_CTL_DELAY		(20/BO_TASK_TIME)

//定义缺省遥控保持时间
#define BO_DEFAULT_KEEP_TIME	(5000/BO_TASK_TIME)

//定义遥控超时等待时间
#define BO_TIME_OUT			(30000/BO_TASK_TIME)

//反校等待时间
#define BO_VER_TIME				(120/BO_TASK_TIME)

struct TBOInfor
{
	BYTE bBOCtlStatus;		//遥控操作当前状态
	WORD wIndex;			//遥控索引，备用
	WORD wKeepTime;			//遥控执行保持时间限值
	WORD wTimeOut;			//遥控超时时间
	WORD wStsTimeCnt;		//遥控操作当前状态持续时间计数
	DWORD dwSetPortAddr;	//遥控预置操作端口地址,对应的执行操作端口地址为+1

};

class CBoProc:public CApp
{
protected:
	DECLARE_MESSAGE_MAP()

public:
	TBOInfor *pBoInfor;		//
	BYTE bModuleBoNum;		//模块BO总个数
	BYTE bBOTestNo;			//传动路号
	DWORD dwBOTestCnt;		//保护传动时间计数
	

//	DWORD dwTestCnt;		//测试用临时计数器
			
	CBoProc(DWORD *pdwApp);
	
	/*************************************************************************************
	* BO初始化														  
	*************************************************************************************/
	void Init();

	/*************************************************************************************
	* BO定时消息处理，完成所有遥控状态的刷新														  
	*************************************************************************************/
	void OnTimeOut(DWORD id);
	
	/*************************************************************************************
	* 遥控预置消息响应，若当前遥控正在处理本次预置不能执行														  
	*************************************************************************************/
	void OnBOSelect(WORD no, WORD attr, WORD par);
	
	/*************************************************************************************
	* 遥控执行消息响应，包括执行和撤销两种操作														  
	*************************************************************************************/
	void OnBOExec(WORD no, WORD attr, WORD mode);
	
	/*************************************************************************************
	* 直接遥控消息响应，如果本路遥控正忙，则本次直接遥控不能执行														  
	*************************************************************************************/
	void OnBODirect(WORD no, WORD attr, WORD par);

	/*********************************************************************************************
	 * 功能描述 	保护传动消息响应函数
	**********************************************************************************************/
	void OnBOTest(WORD no, WORD par1, WORD par2);
	
	/*************************************************************************************
	* 异常处理														  
	*************************************************************************************/
	void ErrorProc();
	
};

/*************************************************************************************
* bo任务入口函数														  
*************************************************************************************/
void BOTask(DWORD *pdwApp);
/*************************************************************************************
* 闭锁继电器处理														  
*************************************************************************************/
DWORD BSRelayCtl(BYTE cmd, BYTE no);

/*********************************************************************************************
 * 功能描述     出口控制操作,本函数提供给逻辑方程使用
**********************************************************************************************/
DWORD RealBOControl(WORD no, BOOL isExe);


//遥控出口继电器映射表
struct TBOMapTable
{
	BYTE bOffset;		//在字节中的偏移
	DWORD dwAddress;	//对应端口地址

	BYTE bPreJOffset;	//对应预置继电器在字节中的偏移
	DWORD dwPreJAddr;	//对应预置继电器的端口地址
};


#endif
