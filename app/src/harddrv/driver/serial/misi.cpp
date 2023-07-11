/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			misi.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           串口守护任务管理                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      串口守护任务完成对所有串口的管理，并为端口层提供相应接口                          
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	MisiDaemon			串口守护任务入口函数
*      	MisiOnTimer			串口任务定时处理过程                                                              
*      	MisiInit	   		                                                                         
*      	MisiOpen    		串口打开，供端口层使用                                                                        
*      	MisiCtrl	    	串口设置，供端口层使用                                                                       
*      	MisiWrite		 	向串口写入数据，供端口层使用                                                                       
*      	MisiRead		    从串口读取一定长度数据，供端口层使用                                                                      
*      	MisiReset		   	串口复位，供端口层使用                                                                        
*      	GetPortSort	   		根据端口号获取串口序号                                                                        
*      	GetSortSerial	   	根据串口序号获取对应串口通道的类指针                                                                        
*      	GetSerial		   	根据端口号获取对应串口通道的类指针                                                                        
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/7/7    初始创建                         
*                                                                                  
********************************************************************************************/
#include "misi.h"
#include "msgdefs.h"
#include "ttydrv.h"
#include "parafunc.h"

CSerial * m_pSerial = NULL;
DWORD m_dwMisiTaskID;
DWORD m_dwPortNum = 0;


/*********************************************************************************************
 *
 * 功能描述     串口守护任务入口函数
 *
 * 参数说明      - arg	: 输入 	任务入口参数，数组下标与任务创建时参数顺序对应
 *
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void MisiDaemon(DWORD *arg)
{
	DWORD dwCount,dwNum;
	TMessage msg;
	TSerialPara	tSerialPara;
	TMBO_Disp	tMapBitObj;
	int		i,j;
	
	ASSERT(arg != NULL);
	if(arg == NULL)
	{
		LogError("MisiDaemon", FILE_LINE, "arg = NULL");
		OS_Exit(6);
	}
	//注意arg为参数数组地址，第一个参数为任务ID，第二个参数为串口个数,即本任务创建时参数顺序
	m_dwPortNum = DB_GetSerialNum();

	// 添加镜像位对象端口
	for(i=0; i<DB_GetMapBitObjNum(); i++)
	{
		DB_GetMapBitObjDisp(i, &tMapBitObj);
		
		// 在已初始化的端口里找不到时，累加
		for(j=0; j<DB_GetSerialNum(); j++)
		{
			DB_GetSerialPara(j, &tSerialPara);
			if(tSerialPara.wPortID == DB_GetMBOPortID(&tMapBitObj))
				break;
		}
		
		if(j == DB_GetSerialNum())
			m_dwPortNum++;
	}
	
	if(m_dwPortNum == 0)
	{
		LogError("MisiDaemon", FILE_LINE, "dwPortNum = 0");
		return;
	}
		
	m_pSerial = new CSerial[m_dwPortNum];
	
	CSerial::ResetSerialMap();
	
	if(m_pSerial == NULL)
	{
		LogError("MisiDaemon",FILE_LINE,"m_pSerial=NULL");
		return;
	}

	MisiInit();
	SerialInit();

	m_dwMisiTaskID = arg[0];//GetAppID();

	////////////////////////
	OS_SetTimer(m_dwMisiTaskID,1,SERIAL_TASK_TIMER, NULL);
	////////////////////////
                        
    dwCount = 0;
    dwNum = 1000/SERIAL_TASK_TIMER;   
    
    ///////////////////////////////////////test
    //设置初始状态为接收消息正确
    DWORD status = ERR_MESSAGE_OK;
	OS_TaskDesp *pTask;
	pTask = OS_GetTaskTab(m_dwMisiTaskID);
    
    //循环调用进行消息调度,直到收到任务退出消息
    while(status != ERR_MESSAGE_QUIT)
    {
        //调用取消息函数,从任务消息队列头,取需要处理的消息
        status = OS_GetMessage(m_dwMisiTaskID,&msg);
        
        //判断接收的消息是否为有效消息
        if(status == ERR_MESSAGE_OK)
        {

			//清除任务运行计数
			if(pTask != NULL)
				pTask->Counter = 0;

       	    switch(LOWORD(msg.lParam))
       	    {
       	    	case    SM_TIMEOUT:
       	    		MisiOnTimer();
					break;
/*		    
			if( dwCount++ > dwNum )
			{
			        dwCount = 0;
			        PSOS_TaskRegister(0,NULL);
			}
*/			
	        default:
       	    		break;
       	    }
            //消息有效,调用消息调度函数对该消息进行调度
     
        }
    }    
	
}

/*********************************************************************************************
 *
 * 功能描述     串口任务定时处理过程
 *
 * 参数说明      
 *
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void MisiOnTimer()
{
	for(int i=0; i<m_dwPortNum; i++)
		m_pSerial[i].MainTask();
}

/*********************************************************************************************
 *
 * 功能描述     串口初始化
 *
 * 参数说明      
 *
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void MisiInit()
{
	TSerialPara 	SerialPar;
	TMBO_Disp		tMapBitObj;
	int 			i,j,m=DB_GetSerialNum();
	WORD			wMapPortID;
	
	for(i=0; i<DB_GetSerialNum(); i++)
	{
		if(DB_GetSerialPara(i, &SerialPar) != DB_ERR_OK)
			continue;

		// 如果是map端口，按照镜像位对象初始化
		if(DB_IsMapBitPort(SerialPar.wPortID))
		{
			DB_PortID2MapBitObjDisp(SerialPar.wPortID,&tMapBitObj);
			
			m_pSerial[i].m_dwPort = SerialPar.wPortID;
			m_pSerial[i].m_serialPara.wPortID	= SerialPar.wPortID;
			m_pSerial[i].m_serialPara.bFlag 	= 0;
			m_pSerial[i].m_serialPara.byDriver	= 0;	
			
			BaudToNum(tMapBitObj.dwBaud,&m_pSerial[i].m_serialPara.byBaud);
		}
		else
		{
			m_pSerial[i].m_dwPort = SerialPar.wPortID;
			m_pSerial[i].m_serialPara = SerialPar;
			m_pSerial[i].InitSerialMap();
		}
	}

	// 添加镜像位对象端口
	for(i=0; i<DB_GetMapBitObjNum(); i++)
	{
		DB_GetMapBitObjDisp(i, &tMapBitObj);

		// 在已初始化的端口里找不到时，初始化本镜像端口
		for(j=0; j<m_dwPortNum; j++)
			if(m_pSerial[j].m_dwPort == DB_GetMBOPortID(&tMapBitObj))
				break;

		if(j == m_dwPortNum)
		{
			m_pSerial[m].m_dwPort = DB_GetMBOPortID(&tMapBitObj);
			m_pSerial[m].m_serialPara.wPortID	= DB_GetMBOPortID(&tMapBitObj);
			m_pSerial[m].m_serialPara.bFlag		= 0;
			m_pSerial[m].m_serialPara.byDriver	= 0;	
			
			BaudToNum(tMapBitObj.dwBaud,&m_pSerial[m].m_serialPara.byBaud);

			m++;
		}
	}
}

/*********************************************************************************************
 *
 * 功能描述     串口打开，供端口层使用
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *				 - dwRxdLen : 输入	应用层最大接收报文长度
 *				 - dwTxdLen : 输入	应用层最大发送报文长度
 *				 - dwMode	: 输入	备用参数
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD  MisiOpen(DWORD dwPort, DWORD dwRxdLen, DWORD dwTxdLen, DWORD dwMode)
{
	int nNum;
	STATUS bRtn;
//	DWORD dwMask;
	
	int pn = MAX_SERIAL_NUM;
	if(dwPort >= pn)
		return ERR_FAILURE;
	
	nNum = GetPortSort(dwPort);
	
	if(nNum < 0 || nNum >= m_dwPortNum)
		return ERR_FAILURE;

//	dwMask = splx(MAX_ILEV);

	m_pSerial[nNum].m_dwTagetID = OS_GetTaskTab(SELF_TASK_ID)->dwTaskID;

	bRtn = m_pSerial[nNum].OpenComm(dwRxdLen, dwTxdLen);

//	(void)splx(dwMask);
	
	return bRtn;
}

/*********************************************************************************************
 *
 * 功能描述     串口设置，供端口层使用
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *				 - pCtrl 	: 输入	串口设置参数
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD  MisiCtrl(DWORD dwPort , TCtlComm *pCtrl)
{
	int nNum;
	DWORD dwRtn;

	if(pCtrl == NULL || dwPort >= MAX_SERIAL_NUM)	
		return ERR_FAILURE;

	nNum = GetPortSort(dwPort);

	if(nNum < 0 || nNum > m_dwPortNum)
		return ERR_FAILURE;
	
	dwRtn =  m_pSerial[nNum].SerialControl(pCtrl);

	return dwRtn;
}

/*********************************************************************************************
 *
 * 功能描述     向串口写入数据，供端口层使用
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *				 - pBuf 	: 输入	准备写入的数据
 *				 - wLen		: 输入	准备写入的数据字节长度
 *				 - dwFlag	: 输入	备用参数
 *
 * 返回代码
 *                实际写入的数据长度
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD MisiWrite(DWORD dwPort, BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	CSerial* pSerial;
	
	if(dwPort >= MAX_SERIAL_NUM)
		return 0;
			
	pSerial = GetSerial(dwPort);
	if(pSerial == NULL)
		return 0;
	
	
	return pSerial->WriteData(pBuf, wLen, dwFlag);
}

/*********************************************************************************************
 *
 * 功能描述     从串口读取一定长度数据，供端口层使用
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *				 - pBuf 	: 输出	准备读取的数据
 *				 - wLen		: 输入	准备读取的数据字节长度
 *				 - dwFlag	: 输入	备用参数
 *
 * 返回代码
 *                实际读取的数据长度
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD MisiRead(DWORD dwPort, BYTE* pBuf, WORD wLen, DWORD dwFlag)
{
	CSerial* pSerial;

	if(dwPort >= MAX_SERIAL_NUM)
		return 0;
	
	pSerial = GetSerial(dwPort);
	
	if(pSerial == NULL)
		return 0;
	
	
	return pSerial->ReadData(pBuf, wLen, dwFlag);
}

/*********************************************************************************************
 *
 * 功能描述     串口复位，供端口层使用
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *				 - dwFlag 	: 输出	复位标志信息
 *
 * 返回代码
 *                成功或失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD MisiReset(DWORD dwPort, DWORD dwFlag)
{
	CSerial* pSerial;
	
	if(dwPort >= MAX_SERIAL_NUM)
		return ERR_FAILURE;
	
	pSerial = GetSerial(dwPort);
	
	if(pSerial == NULL)
		return ERR_FAILURE;
	
	pSerial->ResetSerial(dwFlag);
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     根据端口号获取串口序号
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *
 * 返回代码
 *                串口序号
 *
 * 其它说明：          
 *
**********************************************************************************************/
int GetPortSort(DWORD dwPort)
{
	DWORD dwLPort;
	
	if(m_pSerial == NULL)
		return -1;

	for(int i = 0; i < m_dwPortNum; i++)
	{
		dwLPort = MINOR_PORT(m_pSerial[i].m_dwPort);
		if( dwLPort == dwPort)
			return i;
	}

	return -1;
}

/*********************************************************************************************
 *
 * 功能描述     根据串口序号获取对应串口通道的类指针
 *
 * 参数说明      - wNum	: 输入 	串口序号
 *
 * 返回代码
 *                串口类指针
 *
 * 其它说明：          
 *
**********************************************************************************************/
CSerial * GetSortSerial(WORD wNum)
{
	if(wNum >= m_dwPortNum)
		return NULL;
	return &m_pSerial[wNum];
}

/*********************************************************************************************
 *
 * 功能描述     根据端口号获取对应串口通道的类指针
 *
 * 参数说明      - dwPort	: 输入 	端口号
 *
 * 返回代码
 *                串口类指针
 *
 * 其它说明：          
 *
**********************************************************************************************/
CSerial * GetSerial(DWORD dwPort)
{
	int nNum;

	nNum = GetPortSort(dwPort);
	if(nNum < 0 || nNum > m_dwPortNum)
		return NULL;

	return &m_pSerial[nNum];
}


