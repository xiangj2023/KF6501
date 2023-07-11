/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			bo.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           遥控处理                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      	本遥控处理只负责遥控流程的处理，不具体操作遥控继电器。此处的一路遥控指一个单路遥控
*		而不是一分一合，一分一合表示两路遥控。根据需要，允许多路遥控的并发执行；对于同一路                                                                                        
*       遥控，若该遥控进入遥控状态(从遥控预置到遥控执行返回)，则不允许对此路遥控的操作。                                                                           
*		遥控预置后需要延时一定时间等待预置继电器的动作。
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	BOTask				bo任务入口函数
*		Init				BO初始化
*		OnTimeOut			BO定时消息处理
*		OnBOSelect			遥控预置消息响应
*		OnBOExec			遥控执行消息响应
*		OnBODirect			直接遥控消息响应
*
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/8/20    初始创建                         
*                                                                                  
********************************************************************************************/
#include "bo.h"
#include "m5234timer.h"
#include "logic.h"
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器

BEGIN_MESSAGE_MAP(CBoProc,CApp)
	ON_SM_TIMEOUT(CBoProc)
	ON_SM_BOSELECT(CBoProc)
	ON_SM_BOEXEC(CBoProc)
	ON_SM_BODIRECT(CBoProc)
	ON_SM_BOTEST(CBoProc)
END_MESSAGE_MAP()

/*
void RemoteCtrlPre(WORD wBOIndex, BOOL bContext);
BOOL RemoteCtrlVer(WORD wBOIndex);
void RemoteCtrlExe(WORD wBoIndex, BOOL bContext);
*/
void BOTest(char cmd, char index);


//根据模块类型直接初始化BO配置,如果需要采用参数方式的话，需要增加相应的处理过程
TBOCfg ModuleBoCfg = 
{
	MAX_BO_BOARD_NUM,
	{BO_BOARD1_ADDR, 	BO_BOARD2_ADDR, 	BO_BOARD3_ADDR},
	{BO_BOARD1_BONUM, 	BO_BOARD2_BONUM,	BO_BOARD3_BONUM},
};

DWORD dwBOBaseAdd[]=
{
	DOA_CS_BASE,
//	DOB_CS_BASE,
	DOC_CS_BASE,
	DOD_CS_BASE
};

WORD wRealBONum = 0;	//装置实遥信个数,需要根据本参数来判断一共几块遥控板
DWORD dwBOStatus[MAX_BO_BOARD_NUM][2];//出口操作状态位信息

/*
//遥控操作状态标志，每一位对应一路遥控，置位说明该路遥控正在操作，只有dwBoCtlSts中的32位
//都为零时才能打开该板的闭锁继电器
DWORD dwBoCtlSts[MAX_BO_BOARD_NUM];
*/

/*********************************************************************************************
 *
 * 功能描述     bo任务入口函数
 *
 * 参数说明      - pdwApp	: 输入 	任务入口函数
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void BOTask(DWORD *pdwApp)
{
	CBoProc *pBOTask = new CBoProc(pdwApp);
	pBOTask->Init();
	pBOTask->Run();
}	

/*********************************************************************************************
 *
 * 功能描述     CBoProc类初始化，主要为有关变量及指针初始化
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
CBoProc::CBoProc(DWORD *pdwApp):CApp(pdwApp)
{
	//计算模块包括的BO路数
/*	
	bModuleBoNum = 0;
	for(int i=0; i<ModuleBoCfg.bBOBoardNum; i++)
		bModuleBoNum += ModuleBoCfg.bBONumPerBoard[i];
*/
	bModuleBoNum = DB_GetBONum();	//注意此处的遥控包括虚遥控
	ASSERT(bModuleBoNum <= MAX_BO_NUM);
	if(bModuleBoNum > MAX_BO_NUM)
	{
		LogError("CBoProc",FILE_LINE,"BO number invalidate");
		ErrorProc();
	}

	//初始化BO数据结构
	pBoInfor = new TBOInfor[bModuleBoNum];
	ASSERT(pBoInfor != NULL);
	if(pBoInfor == NULL)
	{
		LogError("CBoProc",FILE_LINE,"malloc error!");
		ErrorProc();
	}
	memset((void*)pBoInfor, 0, sizeof(TBOInfor)*bModuleBoNum);

	//初始化遥控操作状态初始标志数据
//	memset((void*)&dwBoCtlSts[0], 0, sizeof(dwBoCtlSts));
}

/*********************************************************************************************
 *
 * 功能描述     BO初始化，包括每路遥控的端口地址、遥控序号、初始状态等的初始化
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::Init()
{
	int i, j;
	TBOInfor *pBo;
    TBOPara		*pAttr = NULL;

	//取BO参数头
	pAttr = DB_GetBOParaHead();
	if(pAttr == NULL)
	{
		LogError("BOinit",FILE_LINE,"GetBOPara error!");
		return;
	}
		
	//根据数据库参数，初始化BO初始参数
	for(i=0; i<bModuleBoNum; i++, pAttr++)
	{
		pBoInfor[i].bBOCtlStatus = BO_CTL_IDLE;	
		//初始化遥控保持时间
		pBoInfor[i].wKeepTime = pAttr->wLastTime/BO_TASK_TIME;//BO_DEFAULT_KEEP_TIME;
		if(pBoInfor[i].wKeepTime == 0)
			pBoInfor[i].wKeepTime = BO_DEFAULT_KEEP_TIME;
		pBoInfor[i].wTimeOut = BO_TIME_OUT;	//遥控超时时间
		pBoInfor[i].wIndex = pAttr->wSwitchNo; 		//i;		//默认外部遥控路号与内部编号相同

		//计算实遥控路数
		if( (pAttr->dwControl & BO_ATTR_VIRTUAL) == 0)
			wRealBONum++;
	}

/*
	//初始化每个遥控操作的端口地址
	pBo = pBoInfor;
	for(j=0; j<ModuleBoCfg.bBOBoardNum; j++)
	{
		for(i=0; i<ModuleBoCfg.bBONumPerBoard[j]; i++)
		{
			pBo->dwSetPortAddr = ModuleBoCfg.dwBOBoardAddr[j]+i;
			pBo++;
		}
	}
*/	
//	ReadInYX();
//	BOEnable();

//	dwTestCnt = 0;

	dwBOTestCnt = BO_TEST_INVALID;//传动处于无效状态

	//初始化开出初始端口状态
	for(i=0; i<MAX_BO_BOARD_NUM; i++)
		dwBOStatus[i][0] = dwBOStatus[i][1] = 0;

	//设置BO定时器
	SetTimer(1,BO_TASK_TIME);
		
}

/*********************************************************************************************
 *
 * 功能描述     BO定时消息处理，完成所有遥控状态的刷新
 *
 * 参数说明      - id	: 输入 	备用
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::OnTimeOut(DWORD id)
{
	int i;
	BYTE status, number;
	//传动检查
	if(dwBOTestCnt != BO_TEST_INVALID)	//有传动
	{
		if(dwBOTestCnt)
		{
			dwBOTestCnt--;
		}
		else	//传动时间到，收回出口
		{
			dwBOTestCnt = BO_TEST_INVALID;
			CtrlTest(bBOTestNo, FALSE);
		}
	}
	
	for(i=0; i<bModuleBoNum; i++)
	{
		status = pBoInfor[i].bBOCtlStatus;
		number = pBoInfor[i].wIndex;
		switch(status)
		{
			case BO_CTL_IDLE:		//遥控操作空闲状态
				break;
			case BO_CTL_SET:		//遥控操作预置状态，等待预置继电器延时时间
				if(pBoInfor[number].wStsTimeCnt > 0)	//预置继电器延时时间到，进行反校
				{
					if(RemoteCtrlVer(number) == TRUE)	//反校成功
					{	
						DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SUCCESS);
						pBoInfor[number].bBOCtlStatus = BO_CTL_VER;
						pBoInfor[number].wStsTimeCnt = pBoInfor[number].wTimeOut;
						ReportMsg("OnTimeOut: BO verify OK.");
					}
					else
					{
						pBoInfor[number].wStsTimeCnt--;
						ReportMsg("BO verify delay.........");
/*						
						//通知数据库遥控预置反校错误,并自动撤销预置
						DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SET_CHECK);
						RemoteCtrlPre(number, FALSE);	//自动撤销预置
						ReportMsg("BO verify Error.");
						
						pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
						pBoInfor[number].wStsTimeCnt = 0;
*/						
					}
				}
				
				else	//预置继电器延时等待未到
				{
					//通知数据库遥控预置反校错误,并自动撤销预置
					DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SET_CHECK);
					RemoteCtrlPre(number, FALSE);	//自动撤销预置
					ReportMsg("BO verify Error.");
					
					pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
					pBoInfor[number].wStsTimeCnt = 0;
				}
				
				break;
			case BO_CTL_VER:	//遥控反校状态，完成遥控超时检查
				//遥控等待超时，本次遥控失败，自动撤销预置
				if(pBoInfor[number].wStsTimeCnt == 0)	
				{
					//通知数据库遥控超时，自动撤销
					DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SET_TIMEOUT);
					RemoteCtrlPre(number, FALSE);	//自动撤销预置
					ReportMsg("BO control timeout Error.");
					
					pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
					pBoInfor[number].wStsTimeCnt = 0;
					
				}
				else
				{
					pBoInfor[number].wStsTimeCnt--;
				}
				break;
			case BO_CTL_EXECUTE:		//普通遥控执行
			case BO_CTL_DIR_EXECUTE:	//直接遥控执行
				//遥控执行保持时间到，收回出口
				if(pBoInfor[number].wStsTimeCnt == 0)
				{
					RemoteCtrlExe(number, FALSE);
					RemoteCtrlPre(number, FALSE);
					pBoInfor[i].bBOCtlStatus = BO_CTL_IDLE;
					ReportMsg("BO execute return.dwRelayMSCnt = %d",dwRelayMSCnt);
				}
				else //延时等待
				{
					pBoInfor[number].wStsTimeCnt--;
				}
				break;
			default:
				break;
		}
	}
}

/*********************************************************************************************
 *
 * 功能描述     遥控预置消息响应，若当前遥控正在处理本次预置不能执行
 *
 * 参数说明      - no	: 输入 	外部遥控路号
 * 			     - attr	: 输入 	遥控预置属性
 *				 - par	: 输入	备用参数
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::OnBOSelect(WORD no, WORD attr, WORD par)
{
	WORD number;	//遥控内部序号
		
	number = pBoInfor[no].wIndex;
	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("OnBoSelect",FILE_LINE,"BO NO error!");
		//通知数据库遥控预置失败
		DB_BOSelectEcho(no, attr, ERR_BO_POINT);
		return;
	}
	
	//检查该路遥控是否空闲
	if(pBoInfor[number].bBOCtlStatus != BO_CTL_IDLE)
	{
		ReportMsg("BO setting busy, and no=%d.",no);

		//通知数据库遥控预置失败
		DB_BOSelectEcho(no, attr, ERR_BO_BUSY);
		return;
	}

	ReportMsg("BO setting, and no=%d.",no);

	//预置操作
	//设置预置状态
	pBoInfor[number].bBOCtlStatus = BO_CTL_SET;
	
	//遥控预置后首先进行一次反校
	RemoteCtrlPre(number, TRUE);
	if(RemoteCtrlVer(number) == TRUE)	//反校成功
	{	
		DB_BOSelectEcho(no, attr, ERR_BO_SUCCESS);
		pBoInfor[number].bBOCtlStatus = BO_CTL_VER;
		pBoInfor[number].wStsTimeCnt = pBoInfor[number].wTimeOut;
		ReportMsg("OnBOSelect: BO verify OK");
	}
	else
	{
		//进入反校等待状态(考虑到预置继电器的延时,必须等待一定时间，20毫秒)	
		pBoInfor[number].wStsTimeCnt = BO_VER_TIME;
	}
	
}

/*********************************************************************************************
 *
 * 功能描述     遥控执行消息响应，包括执行和撤销两种操作
 *
 * 参数说明      - no	: 输入 	外部遥控路号
 * 			     - attr	: 输入 	遥控预置属性
 *				 - mode	: 输入	执行或撤销
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::OnBOExec(WORD no, WORD attr, WORD mode)
{
	WORD number;	//遥控内部序号
	BYTE status;
		
	number = pBoInfor[no].wIndex;
	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("OnBoExecute",FILE_LINE,"BO NO error!");
		//通知数据库遥控执行失败
		DB_BOExecEcho(no, attr, mode, ERR_BO_POINT);
		return;
	}

	//检查状态是否合法
	status = pBoInfor[number].bBOCtlStatus;
	if(mode == 1)	//遥控执行
	{
		if(status == BO_CTL_VER)	//只有遥控预置成功后才能进行遥控执行
		{
			//遥控执行
			RemoteCtrlExe(number, TRUE);
			
			ReportMsg("BO number%d execute.", number);
			//进入遥控执行保持状态
			pBoInfor[number].bBOCtlStatus = BO_CTL_EXECUTE;
			TBOPara BoPara;
			DB_GetBOPara(number,&BoPara);
			pBoInfor[number].wStsTimeCnt = BoPara.wLastTime/BO_TASK_TIME;
			//pBoInfor[number].wStsTimeCnt = pBoInfor[number].wKeepTime;

			//通知数据库遥控执行成功
			DB_BOExecEcho(no, attr, mode, ERR_BO_SUCCESS);
		}
		else
		{
			LogError("OnBoExecute",FILE_LINE,"BO busy!");
			//通知数据库遥控执行失败
			DB_BOExecEcho(no, attr, mode, ERR_BO_BUSY);
		}
	}
	else if(mode == 2)	//遥控预置撤销
	{
		//处于遥控预置或反校状态才能发送撤销命令
		if( (status == BO_CTL_VER) || (status == BO_CTL_SET) )
		{
			//遥控预置撤销
			RemoteCtrlPre(number, FALSE);
			
			ReportMsg("BO number%d cancel.", number);
			//进入空闲状态
			pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
			pBoInfor[number].wStsTimeCnt = 0;
			//通知数据库遥控撤销
			DB_BOExecEcho(no, attr, mode, ERR_BO_SUCCESS);
		}
		else
		{
			LogError("OnBoExecute",FILE_LINE,"BO busy!");
			//通知数据库遥控执行失败
			DB_BOExecEcho(no, attr, mode, ERR_BO_BUSY);
		}
	}
}

/*********************************************************************************************
 *
 * 功能描述     直接遥控消息响应，如果本路遥控正忙，则本次直接遥控取消
 *
 * 参数说明      - no	: 输入 	外部遥控路号
 * 			     - attr	: 输入 	遥控预置属性
 *				 - par	: 输入	备用参数		 
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::OnBODirect(WORD no, WORD attr, WORD par)
{
	WORD number;	//遥控内部序号
	
	number = pBoInfor[no].wIndex;
	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("OnBoDirExecute",FILE_LINE,"BO NO error!");
		//通知数据库遥控执行失败
		DB_BODirectEcho(no, attr, ERR_BO_POINT);
		return;
	}

	//检查该路遥控是否空闲
	if(pBoInfor[number].bBOCtlStatus != BO_CTL_IDLE)
	{
		//通知数据库直接遥控失败，当前遥控正忙
		DB_BODirectEcho( no, attr, ERR_BO_BUSY);
	}
	else
	{

		ReportMsg("BO number%d direct execute.dwRelayMSCnt = %d", number+1,dwRelayMSCnt);
		
		//通过遥控执行函数完成直接遥控
		RemoteCtrlPre(number, TRUE);
		RemoteCtrlExe(number, TRUE);
		
		//遥控操作进入直接遥控预置状态
		pBoInfor[number].bBOCtlStatus = BO_CTL_DIR_EXECUTE;

		TBOPara BoPara;
		DB_GetBOPara(number,&BoPara);
		pBoInfor[number].wStsTimeCnt = BoPara.wLastTime/BO_TASK_TIME;
		//pBoInfor[number].wStsTimeCnt = pBoInfor[number].wKeepTime;
		
		//通知数据库遥控执行成功
		DB_BODirectEcho(no, attr, ERR_BO_SUCCESS);
	}
}
/*********************************************************************************************
 *
 * 功能描述     异常处理
 *
 * 参数说明      无
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::ErrorProc()
{
	OS_Suspend(0);
}

/*********************************************************************************************
 *
 * 功能描述     保护传动消息响应函数
 *
 * 参数说明      - no	: 输入 	外部传动路号
 *				 - par1	: 输入	备用参数
 *				 - par2	: 输入	备用参数
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CBoProc::OnBOTest(WORD no, WORD par1, WORD par2)
{
	if(dwBOTestCnt == BO_TEST_INVALID)	//当前无传动正在执行，可以进行传动试验
	{
		ReportMsg("BOTest:no=%d",no+1);
		dwBOTestCnt = par1/BO_TASK_TIME;
		bBOTestNo = no;
		CtrlTest(no, TRUE);
		
	}
}

/*********************************************************************************************
 *
 * 功能描述     出口控制操作,本函数提供给逻辑方程使用,仅针对实遥控点
 *
 * 参数说明      - no	: 输入 	出口控制序号,即硬件遥控点序号
 *				 - isExe: 输入 是执行还是撤销
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
/*
DWORD RealBOControl(WORD no, BOOL isExe)
{
	BYTE number, offset, cmd1, cmd2;
	DWORD dwBaseAdd;
	BYTE status[3];

	//检查出口路号是否有效
	ASSERT(no<(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD));
	if(no>=(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD))
		return ERR_FAILURE;

	number = no/MAX_BO_NUM_PER_BOARD;
	offset = no%MAX_BO_NUM_PER_BOARD;

	//取对应遥控出口的基址,遥控启动出外(第0路序号)
	if(offset)
	{
		dwBaseAdd = dwBOBaseAdd[number]+0x02*(offset/8);
		if(isExe == TRUE)	//执行
		{
			dwBOStatus[number][0] &= (~(0x01<<offset));
			dwBOStatus[number][1] |= (0x01<<offset);
		}
		else	//撤销
		{
			dwBOStatus[number][0] |= (0x01<<offset);
			dwBOStatus[number][1] &= (~(0x01<<offset));
		}

		//出口控制
		cmd1 = (BYTE)(dwBOStatus[number][0]>>(offset/8*8));
		cmd2 = (BYTE)(dwBOStatus[number][1]>>(offset/8*8));
		outportb(dwBaseAdd,  cmd1);
		outportb(dwBaseAdd+1, cmd2);
	}
	else	//单独处理每块遥控板的遥控启动控制
	{
		dwBaseAdd = dwBOBaseAdd[number]+6;	//计算遥控启动路的端口地址
		if(isExe == TRUE)	//执行
		{
			if(number == 0)
			{
				MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
				MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=1
			}
			else
			{
				MCF_GPIO_PODR_TIMER &= (~DTI1_YKEN3);	//YK_EN3=0
				MCF_GPIO_PODR_TIMER |= DTI0_YKEN4;		//YK_EN4=1
			}
			outportb(dwBaseAdd, 0x55);
		}
		else	//撤销
		{
			if(number == 0)
			{
				MCF_GPIO_PODR_TIMER |= DTO1_YKEN1;		//YK_EN1=1
				MCF_GPIO_PODR_TIMER &= (~DTO0_YKEN2);	//YK_EN2=0
			}
			else
			{
				MCF_GPIO_PODR_TIMER |= DTI1_YKEN3;		//YK_EN3=1
				MCF_GPIO_PODR_TIMER &= (~DTI0_YKEN4);	//YK_EN4=0
			}
			outportb(dwBaseAdd, 0x00);
		}
		
	}
	return ERR_SUCCESS;
}*/
DWORD RealBOControl(WORD no, BOOL isExe)
{
	BYTE number, offset, cmd1, cmd2;
	DWORD dwBaseAdd;
	BYTE status[3];

	//检查出口路号是否有效
	ASSERT(no<(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD));
	if(no>=(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD))
		return ERR_FAILURE;

	number = no/MAX_BO_NUM_PER_BOARD;
	offset = no%MAX_BO_NUM_PER_BOARD;

	cmd1=0x55;  //执行
	cmd2=0xAA;	//撤销
	dwBaseAdd = dwBOBaseAdd[number]+offset;
	if(isExe == TRUE) //遥控执行
	{
		if(offset == 0)//总闭锁的开出
		{
			if(number == 0) //第一块板
			{
				MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
				MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=1
			}
			else			//第二快板
			{
				MCF_GPIO_PODR_TIMER &= (~DTI1_YKEN3);	//YK_EN3=0
				MCF_GPIO_PODR_TIMER |= DTI0_YKEN4;		//YK_EN4=1
			}
		}
		outportb(dwBaseAdd,  cmd1);		
	}
	else //遥控撤销
	{
		if(offset == 0)//总闭锁的开出
		{
			if(number == 0) //第一块板
			{
				MCF_GPIO_PODR_TIMER |= DTO1_YKEN1;		//YK_EN1=1
				MCF_GPIO_PODR_TIMER &= (~DTO0_YKEN2);	//YK_EN2=0
			}
			else			//第二快板
			{
				MCF_GPIO_PODR_TIMER |= DTI1_YKEN3;		//YK_EN3=1
				MCF_GPIO_PODR_TIMER &= (~DTI0_YKEN4);	//YK_EN4=0
			}
		}
		outportb(dwBaseAdd,  cmd2);		
	}
/*
	//取对应遥控出口的基址,遥控启动出外(第0路序号)
	if(offset)
	{
		dwBaseAdd = dwBOBaseAdd[number]+0x02*(offset/8);
		if(isExe == TRUE)	//执行
		{
			dwBOStatus[number][0] &= (~(0x01<<offset));
			dwBOStatus[number][1] |= (0x01<<offset);
		}
		else	//撤销
		{
			dwBOStatus[number][0] |= (0x01<<offset);
			dwBOStatus[number][1] &= (~(0x01<<offset));
		}

		//出口控制
		cmd1 = (BYTE)(dwBOStatus[number][0]>>(offset/8*8));
		cmd2 = (BYTE)(dwBOStatus[number][1]>>(offset/8*8));
		outportb(dwBaseAdd,  cmd1);
		outportb(dwBaseAdd+1, cmd2);
	}
	else	//单独处理每块遥控板的遥控启动控制
	{
		dwBaseAdd = dwBOBaseAdd[number]+6;	//计算遥控启动路的端口地址
		if(isExe == TRUE)	//执行
		{
			if(number == 0)
			{
				MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
				MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=1
			}
			else
			{
				MCF_GPIO_PODR_TIMER &= (~DTI1_YKEN3);	//YK_EN3=0
				MCF_GPIO_PODR_TIMER |= DTI0_YKEN4;		//YK_EN4=1
			}
			outportb(dwBaseAdd, 0x55);
		}
		else	//撤销
		{
			if(number == 0)
			{
				MCF_GPIO_PODR_TIMER |= DTO1_YKEN1;		//YK_EN1=1
				MCF_GPIO_PODR_TIMER &= (~DTO0_YKEN2);	//YK_EN2=0
			}
			else
			{
				MCF_GPIO_PODR_TIMER |= DTI1_YKEN3;		//YK_EN3=1
				MCF_GPIO_PODR_TIMER &= (~DTI0_YKEN4);	//YK_EN4=0
			}
			outportb(dwBaseAdd, 0x00);
		}
		
	}
	*/
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     闭锁继电器处理，每个板一个闭锁继电器，因此每个板有一个对应每路遥控操作的状态
 *				记录，当每路遥控都结束后才能打开闭锁继电器，否则只有有一路遥控操作都需要保持
 *				闭锁继电器处于合位。
 *
 * 参数说明      - cmd	: 输入 	遥控命令
 * 			     - no	: 输入 	内部遥控序号
 *				 
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
/*
DWORD BSRelayCtl(BYTE cmd, BYTE no)
{
	BYTE num;

	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("BSRelayCtl",FILE_LINE,"BO no error!");
		return ERR_FAILURE;
	}
	num = no/MAX_BO_NUM_PER_BOARD;

	if(cmd == BO_CLOSE)
	{
		BJ_RELAY_CTL(BO_CLOSE);	//合闭锁继电器

		//置遥控状态标志位
		OS_Protect(0);
		dwBoCtlSts[num] |= (0x01<<(no%MAX_BO_NUM_PER_BOARD));
		OS_UnProtect(0);
	}
	else if(cmd == BO_OPEN)
	{
		//清遥控状态标志位
		OS_Protect(0);
		dwBoCtlSts[num] &= (~(0x01<<(no%MAX_BO_NUM_PER_BOARD)));
		//只有该板上的所有遥控都结束后才能打开该板上的闭锁继电器
		if(dwBoCtlSts[num] == 0)
			BJ_RELAY_CTL(BO_OPEN); //合闭锁继电器
		OS_UnProtect(0);
	}
	
	return ERR_SUCCESS;
}
*/
//临时测试用遥控出口映射表
TBOMapTable BoMapTable[16]=
{
	{2,		DOA_CS_BASE, 	1,	DOA_CS_BASE},
	{3,		DOA_CS_BASE,	1,	DOA_CS_BASE},
	{5,		DOA_CS_BASE,	4,	DOA_CS_BASE},
	{6,		DOA_CS_BASE,	4,	DOA_CS_BASE},
	{0,		DOA_CS_BASE+2,	7,	DOA_CS_BASE},
	{1,		DOA_CS_BASE+2,	7,	DOA_CS_BASE},
	{3,		DOA_CS_BASE+2,	2,	DOA_CS_BASE+2},
	{4,		DOA_CS_BASE+2,	2,	DOA_CS_BASE+2},
	{6,		DOA_CS_BASE+2,	5,	DOA_CS_BASE+2},
	{7,		DOA_CS_BASE+2,	5,	DOA_CS_BASE+2},
	{1,		DOA_CS_BASE+4,	0,	DOA_CS_BASE+4},
	{2,		DOA_CS_BASE+4,	0,	DOA_CS_BASE+4},
	{4,		DOA_CS_BASE+4,	3,	DOA_CS_BASE+4},
	{5,		DOA_CS_BASE+4,	3,	DOA_CS_BASE+4},
	{6,		DOA_CS_BASE+4,	3,	DOA_CS_BASE+4},
	{7,		DOA_CS_BASE+4,	3,	DOA_CS_BASE+4},
};
/*
//临时定义遥控预置函数
//wBOIndex--遥控序号，bContext--预置或撤销预置
void RemoteCtrlPre(WORD wBOIndex, BOOL bContext)
{
	BYTE value = 0;

	value = (0x01<<BoMapTable[wBOIndex].bPreJOffset);

	ReportMsg("\n遥控预置前状态:");
	ReadInYX();

	if(bContext == TRUE)
	{
		outportb(BoMapTable[wBOIndex].dwPreJAddr, 0);
		outportb(BoMapTable[wBOIndex].dwPreJAddr+1, value);
		ReportMsg("遥控预置设置，序号:%d .", wBOIndex);
	}
	else
	{
		outportb(BoMapTable[wBOIndex].dwPreJAddr, 0);
		outportb(BoMapTable[wBOIndex].dwPreJAddr+1, 0);
		ReportMsg("遥控预置撤销，序号:%d .", wBOIndex);
	}
	ReportMsg("遥控预置后状态:");
	ReadInYX();
	return;
}

//遥控预置反校函数
BOOL RemoteCtrlVer(WORD wBOIndex)
{
	return TRUE;
}

//遥控执行函数
void RemoteCtrlExe(WORD wBoIndex, BOOL bContext)
{
	BYTE value = 0;
	
	ReportMsg("\n遥控执行前状态:");
	ReadInYX();

	if(BoMapTable[wBoIndex].dwPreJAddr == BoMapTable[wBoIndex].dwAddress)
	{
		value = (0x01<<BoMapTable[wBoIndex].bPreJOffset)|(0x01<<BoMapTable[wBoIndex].bOffset);
		if(bContext == TRUE)
		{
			ReportMsg("遥控执行设置，序号:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, value);
		}
		else
		{
			ReportMsg("遥控执行撤销，序号:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, 0);
		}
	}
	else
	{
		if(bContext == TRUE)
		{
			ReportMsg("遥控执行设置，序号:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwPreJAddr, 0);
			outportb(BoMapTable[wBoIndex].dwPreJAddr+1, 0x01<<BoMapTable[wBoIndex].bPreJOffset);
			
			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, 0x01<<BoMapTable[wBoIndex].bOffset);

		}
		else
		{
			ReportMsg("遥控执行撤销，序号:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwPreJAddr, 0);
			outportb(BoMapTable[wBoIndex].dwPreJAddr+1, 0);

			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, 0);

		}
	}
	ReportMsg("遥控执行后状态:");
	ReadInYX();
	return;
}
*/
//遥控测试函数
void BOTest(char cmd, char index)
{
	if(index<1)
		return;
	else
		index =index-1;
	switch(cmd)
	{
		case 'p':	//预置
			DB_BOSelect(index, BO_NULL);
			break;
		case 'e':	//执行
			DB_BOExec(index,BO_NULL,1);
			break;
		case 'd':	//直接执行
			DB_BODirect(index, BO_NULL);
			break;
		case 'c':	//撤销
			DB_BOExec(index,BO_NULL,2);
			break;
			
	}
}



#ifdef _TEST_BO
void TestBOPort();
void ReadInYX();
void BOEnable();

//遥控启动,注意YK_EN1~4对应的定时器引脚必须预先初始化(已经在定时器中初始化)
void BOEnable()
{
	MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
	MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=2
	outportb(DOA_CS_BASE+6, 0x55);
}
void ReadInYX()
{
	BYTE bInYx[2], bCheckYx[3];
	int i;

	OS_Sleep(10);
	for(i=0; i<2;i ++)
		bInYx[i] = inportb(DOA_CS_BASE+7+i);

	for(i=0; i<3; i++)
		bCheckYx[i] =  inportb(DOA_CS_BASE+9+i);

	ReportMsg("		内部遥信状态:%x %x, 遥控反校状态:%x %x %x .", bInYx[0], bInYx[1],\
					bCheckYx[0], bCheckYx[1], bCheckYx[2]);
	
}

void TestBOPort()
{
	static BYTE bCounter;
	DWORD dwAddress;
	BYTE bValue;

	if(bCounter>=24)
		bCounter = 0;
	
	dwAddress = DOA_CS_BASE+(bCounter/8)*2;
	bValue = 0x01<<(bCounter%8);
	outportb(dwAddress, 0);
	outportb(dwAddress+1, bValue);

	if(bCounter==8)
	{
		outportb(DOA_CS_BASE, 0);
		outportb(DOA_CS_BASE+1, 0);
	}
	else if(bCounter==16)
	{
		outportb(DOA_CS_BASE+2, 0);
		outportb(DOA_CS_BASE+3, 0);
	}
	else if(bCounter == 0)
	{
		outportb(DOA_CS_BASE+4, 0);
		outportb(DOA_CS_BASE+5, 0);
	}
	
	ReadInYX();
	bCounter++;
}

#endif

