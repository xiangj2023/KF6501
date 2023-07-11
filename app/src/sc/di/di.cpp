/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			di.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           DI采集处理                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      DI采集处理，包括遥信和电度处理                          
*      由于考虑了虚遥信和内部遥信，并且单板实遥信的路数不一定是8的倍数，因此需要统一考虑。                                                                                  
*      首先pDIBitData遥信状态缓冲区按照所有遥信总数来分配，连续存储有效信息；                                                                                  
*      pDIPortData端口状态缓冲区包括实遥信、虚遥信和内部遥信，每种信息按照8的倍数位保存，                                                                                  
*      不同类型中间可能有部分位无效，对于多个遥信板的情况，如每个板34路，则个占用5个字节的                                                                                  
*      存储区域，这样进行扫描和位比较时方便。但是最后写数据库时必须剔除无效位。                                                                            
*      假设遥信排列顺序为:实、内部、虚                                                                            
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	DITask				DI任务入口函数
*		Init				DI初始化
*		DIPortInit			DI基于字节端口的参数初始化
*		DIBitInit			DI基于位的参数初始化
*		BIStatusInit		DI初始状态确定
*		OnDIChange			DI变位消息响应
*		OnTimeOut			DI定时消息响应
*		GetDIIndex			计算指定字节端口序号所对应的总的DI序号
*		ErrorProc			DI采集任务异常处理
*		DITimerInt			DI采集定时中断处理
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/8/14    初始创建                         
*                                                                                  
********************************************************************************************/
#include "di.h"
#include "dbfunc.h"			 
#include "led.h"
#include "hmidatalist.h"
#include "logic.h"
#ifdef TEST_DI
void TestDI();
#endif
extern DWORD dwRelayMSCnt;		//保护用毫秒计数器
BEGIN_MESSAGE_MAP(CDIProc,CApp)
	ON_SM_TIMEOUT(CDIProc)
	ON_SM_DICHANGE(CDIProc)
	ON_SM_BIPARACHANGE(CDIProc)
END_MESSAGE_MAP()

//DI位状态信息
TDIBitData *pDIBitData = NULL;

//DI端口状态信息
TDIPortData *pDIPortData = NULL; 

BOOL bDIInitFlag = FALSE;

//DI变化缓冲区
TDIBuf *pDIBuffer = NULL;
WORD wDIReadPtr;
WORD wDIWritePtr;

DWORD dwDIIntCnt;		//DI定时器中断计数
WORD wMSecondCnt = 0;		//毫秒计数

TDIType DIType;		//各种类型DI的基本信息

DWORD dwTaskDIId;
DWORD dwInDIBitFlag;	//内部遥信位标志信息,若对应位置位，说明对应位的内部遥信有效


DWORD dwDIBaseAdd[]=
{
	BIA_CS_BASE,
	BIB_CS_BASE	
};
//STATIC DWORD dwSOECnt, dwDITimeCnt;

extern DWORD 	dwRelayMSCnt;		//保护用毫秒计数器
extern BOOL isVirDIValide;


STATUS ReadAllInnerDI(BYTE *pBuf);
STATUS ReadAllVirDI(BYTE *pBuf);

/*********************************************************************************************
 *
 * 功能描述     DI任务入口函数
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
void DITask(DWORD *pdwApp)
{
	OS_Sleep(100);
	CDIProc *pDITask = new CDIProc(pdwApp);
	pDITask->Init();
	pDITask->Run();
}	

/*********************************************************************************************
 *
 * 功能描述     DIProc类初始化，主要为有关变量及指针初始化
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
CDIProc::CDIProc(DWORD *pdwApp):CApp(pdwApp)
{
	TDIPara *pPar = NULL;
	WORD number;

	//计算各种类型遥信位的个数
	number = DB_GetDINum();
	pPar = DB_GetDIParaHead();
	ASSERT(pPar != NULL);
	if(pPar == NULL)
	{
		LogError("CDIProc",FILE_LINE,"get DI par error.");
		return;
	}

	//分类统计遥信个数
	for(int i=0; i<number; i++, pPar++)
	{
		switch(pPar->wClass&DI_ATTR_TYPEMASK)
		{
			case DI_TYPE_VIR:	//虚遥信
				DIType.wDIBitNum[VIR_DI]++;
				break;
			case DI_TYPE_INNER:	//内部遥信
				DIType.wDIBitNum[INNER_DI]++;
				break;
			case DI_TYPE_INVALID:
				break;
			default:	//除了实遥信、内部遥信之外的有效DI都属于内部遥信
				DIType.wDIBitNum[REAL_DI]++;
				break;
		}
	}
	
	ASSERT(number<=MAX_DI_NUM);
	if(number>MAX_DI_NUM)
	{
		LogError("CDIProc",FILE_LINE,"wDIBitNum invalidate");
		ErrorProc();
	}

	//遥信位的信息结构按照实际总遥信位个数分配
	pDIBitData = new TDIBitData[number];
	ASSERT(pDIBitData != NULL);
	if(pDIBitData == NULL)
	{
		LogError("CDIProc",FILE_LINE,"pDIBitData invalidate");
		ErrorProc();
	}
	memset((void*)pDIBitData, 0, sizeof(TDIBitData)*number);

	//初始化字节端口信息结构
	// 1块遥信板(34路)，占5个字节，2块则占10个

	//计算实遥信端口基址
	DIType.wDIPortBase[REAL_DI] = 0;

	//计算内部遥信端口基址
	DIType.wDIPortBase[INNER_DI]= ((DIType.wDIBitNum[REAL_DI]+MAX_DI_NUM_PER_BOARD-1)/\
			MAX_DI_NUM_PER_BOARD)*((MAX_DI_NUM_PER_BOARD+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH);
	
	//计算虚遥信端口基址
	DIType.wDIPortBase[VIR_DI]= (DIType.wDIBitNum[INNER_DI]+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH+\
								DIType.wDIPortBase[INNER_DI];

	//计算所有遥信占用端口个数
	DIType.wDIPortNum = (DIType.wDIBitNum[VIR_DI]+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH + \
						DIType.wDIPortBase[VIR_DI];
	
	pDIPortData = new TDIPortData[DIType.wDIPortNum];
	ASSERT(pDIPortData != NULL);
	if(pDIPortData == NULL)
	{
		LogError("CDIProc",FILE_LINE,"pDIPortData invalidate");
		ErrorProc();
	}
	memset((void *)pDIPortData, 0, sizeof(TDIPortData)*DIType.wDIPortNum);

	//SOE临时缓冲初始化
	pSoeBuf = new TEventBI[SOE_BUF_LEN];
	memset((void *)pSoeBuf, 0, sizeof(TEventBI)*SOE_BUF_LEN);
	wSoeReadPtr = wSoeWritePtr;

	//DI变化记录缓冲区初始化
	pDIBuffer = new  TDIBuf[DI_BUF_LEN];
	memset((void *)pDIBuffer, 0, sizeof(TDIBuf)*DI_BUF_LEN);
	wDIReadPtr = wDIWritePtr = 0;

	//取DI任务ID
	dwTaskDIId = GetTaskId();

	//初始化内部遥信有效位标志
	dwInDIBitFlag = 0;
	
}

/*********************************************************************************************
 *
 * 功能描述     DI初始化，根据参数完成基于DI字节端口的参数和基于DI位的参数初始化，确定DI初
 *				始状态，创建任务定时器
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
void CDIProc::Init()
{
/*		BYTE flag = 0;
	
		while( flag == 0)
		{
			OS_Sleep(200);
		}
*/
	DIPortInit();

	DIBitInit();

	BIStatusInit(REAL_DI);
	BIStatusInit(INNER_DI);
	BIStatusInit(VIR_DI);
	
	//设置DI定时器
	SetTimer(1,DI_TIMER_PERIOD);

	//DI初始化完成，DI采样定时中断可以进行DI采样处理
	bDIInitFlag = TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     对所有类型遥信所对应的字节端口的参数初始化
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
void CDIProc::DIPortInit()
{
	int i,j;
	BYTE boardnum, portnum, portbase;

	//实遥信处理
	//根据实遥信路数计算遥信板个数
	boardnum = ((DIType.wDIBitNum[REAL_DI]+MAX_DI_NUM_PER_BOARD-1)/MAX_DI_NUM_PER_BOARD);
	//每个板遥信所占用的端口个数
	portnum = (MAX_DI_NUM_PER_BOARD+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH;
	portbase = 0;
	for(i=0; i<boardnum; i++)
	{
		for(j=0; j<portnum; j++)
		{
#if(HARD_WARE_VER == HW_VERSION_10)
			pDIPortData[portbase].dwPortAddr = dwDIBaseAdd[i]-j;
#elif(HARD_WARE_VER >= HW_VERSION_11)
			pDIPortData[portbase].dwPortAddr = dwDIBaseAdd[i]+j;
#endif
			pDIPortData[portbase].bBiNumInPort = DI_BOARD_WIDTH;
			
			//注意由于每个板的DI个数不一定是8的整数倍，因此最后一组需要单独计算实际个数
			if(j == (portnum-1))
			{
				if(MAX_DI_NUM_PER_BOARD%DI_BOARD_WIDTH)
					pDIPortData[portbase].bBiNumInPort = MAX_DI_NUM_PER_BOARD%DI_BOARD_WIDTH;
				else
					pDIPortData[portbase].bBiNumInPort = DI_BOARD_WIDTH;
			}
			portbase++;
		}
	}

	//内部遥信
	portnum = (DIType.wDIBitNum[INNER_DI]+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH;
	for(i=0; i<portnum; i++)
	{
		pDIPortData[portbase].bBiNumInPort = DI_BOARD_WIDTH;
		if(i == (portnum-1))
		{
			if(DIType.wDIBitNum[INNER_DI]%DI_BOARD_WIDTH)
				pDIPortData[portbase].bBiNumInPort = DIType.wDIBitNum[INNER_DI]%DI_BOARD_WIDTH;
			else
				pDIPortData[portbase].bBiNumInPort = DI_BOARD_WIDTH;
		}
		portbase++;
	}
	
	//虚遥信
	portnum = (DIType.wDIBitNum[VIR_DI]+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH;
	for(i=0; i<portnum; i++)
	{
		pDIPortData[portbase].bBiNumInPort = DI_BOARD_WIDTH;
		if(i == (portnum-1))
		{
			if(DIType.wDIBitNum[VIR_DI]%DI_BOARD_WIDTH)
				pDIPortData[portbase].bBiNumInPort = DIType.wDIBitNum[VIR_DI]%DI_BOARD_WIDTH;
			else
				pDIPortData[portbase].bBiNumInPort = DI_BOARD_WIDTH;
		}
		portbase++;
	}
}

void CDIProc::OnBiParaChange(DWORD id)
{
	LogError("OnBiParaChange",FILE_LINE,"Bi Para changed");
	DIBitInit();
	BIStatusInit(REAL_DI);
	BIStatusInit(INNER_DI);
	BIStatusInit(VIR_DI);
}

/*********************************************************************************************
 *
 * 功能描述     DI基于位的参数初始化
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
void CDIProc::DIBitInit()
{
	
	WORD number, type;
	BYTE board, offset;
	BOOL bResetDIFlag = FALSE;
	TDIPara *pDIPar = NULL;
	TBIPara *pBIPar = NULL;
	TCIPara *pCIPar = NULL;

	pDIPar = DB_GetDIParaHead();	//取DI参数头
	pBIPar = DB_GetBIParaHead();	//取BI参数头
	pCIPar = DB_GetCIParaHead();	//取CI参数头
	pCIPar += INTERGRAL_KWH_COUNT;  //取得脉冲电度的偏移
	number = DB_GetDINum();		//取DI个数
	
	dwInDIBitFlag = 0;  //内部遥信标志位重新清0
	//DI位参数初始化
	for(int i=0; i<number; i++, pDIPar++)
	{
		//初始化去抖时间
		pDIBitData[i].dwSmoothTime = pDIPar->wFilterTime*2;	//去抖时间
		if(pDIBitData[i].dwSmoothTime == 0)
			pDIBitData[i].dwSmoothTime = DEFAULT_BI_SMOOTH_TIME;
		
//		pDIBitData[i].wIndex = pDIPar->wMap;		//索引序号

		//取DI类型
		type = pDIPar->wClass&DI_ATTR_TYPEMASK;
		
		//对于遥信类型需要初始化是否取反
		if( (type == DI_TYPE_SP) || (type == DI_TYPE_DP) )
		{
			pDIBitData[i].bAttribute &= ~DI_IS_CI;

			//根据遥信位参数，检查是否取反
//			if(pBIPar[pDIPar->wMap].wControl&BI_ATTR_CONVERT)
//				pDIBitData[i].bAttribute |= DI_IS_REVERSE;
		}
		else if(type == DI_TYPE_PLUS)//检查是否为脉冲电度
			pDIBitData[i].bAttribute |= DI_IS_CI;

		else if(type == DI_TYPE_INNER)//检查有效的内部遥信位
		{
			
			offset = pDIPar->bInDINo%MAX_IN_DI_NUM_PER_BOARD;	//计算内部遥信在板内的序号
			board = pDIPar->bInDIBoardNo%MAX_BO_BOARD_NUM;	//计算所属的板号

			if((board + offset) == 0)
			{
				if(bResetDIFlag == FALSE)
				{
					bResetDIFlag = TRUE;
					continue;
				}
			}
			dwInDIBitFlag |= (0x01<<(board*MAX_IN_DI_NUM_PER_BOARD+offset));
		}
	}

	//根据遥信参数初始化DI到BI的映射
	number = DB_GetBINum();		//取BI个数
	for(int i=0; i<number; i++, pBIPar++)
	{
		pDIBitData[pBIPar->wMaptoDINo].wIndex = i;//确定DI与BI的映射关系
		
		//检查是否需要遥信取反
		if(pBIPar->wControl&BI_ATTR_CONVERT)
			pDIBitData[pBIPar->wMaptoDINo].bAttribute |= DI_IS_REVERSE;
		else
			pDIBitData[pBIPar->wMaptoDINo].bAttribute &= (~DI_IS_REVERSE);
			
		//检查是否需要需要产生SOE
		if(pBIPar->wControl&BI_ATTR_WTPEVENT)
			pDIBitData[pBIPar->wMaptoDINo].bAttribute |= DI_CREAT_SOE;
		else
			pDIBitData[pBIPar->wMaptoDINo].bAttribute &= (~DI_CREAT_SOE);
	}

	//根据电度参数建立DI到CI的映射
	number = DB_GetPulseCINum();		//取CI个数
	for(int i=0; i<number; i++, pCIPar++)
	{
		pDIBitData[pCIPar->wMaptoDINo].wIndex = i;//确定DI与CI的映射关系
	}
	
	//单独处理第一路内部遥信即信号复归
	dwInDIBitFlag <<= 1;
	dwInDIBitFlag |= 0x01;
}

/*********************************************************************************************
 *
 * 功能描述     DI初始状态确定
 *
 * 参数说明      - arg	: 输入 	DI类型
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void CDIProc::BIStatusInit(BYTE type)
{
	BYTE bBIStatus;
	TBurstBI BIData;
	int i, j, k, DIIndex;
	BYTE bDIStatus[10];
	BYTE start,end;
	BYTE bBoardId;
	DWORD address;

	DIIndex = 0;

	memset(bDIStatus, 0, sizeof(bDIStatus));

	start = DIType.wDIPortBase[type];
	
	//根据DI不同类型确定各自的端口范围，对于内部遥信和虚遥信，预先读取全部状态信息
	switch(type)
	{
		case REAL_DI:
			end = DIType.wDIPortBase[INNER_DI];
			DIIndex = 0;
			break;
		case INNER_DI:
			end = DIType.wDIPortBase[VIR_DI];
			ReadAllInnerDI(bDIStatus);
			DIIndex = DIType.wDIBitNum[REAL_DI];
			break;
		case VIR_DI:
			end = DIType.wDIPortNum;
			ReadAllVirDI(bDIStatus);
			DIIndex = DIType.wDIBitNum[REAL_DI]+DIType.wDIBitNum[INNER_DI];
			break;
	}
	
	for(i=start, k=0; i<end; i++, k++)
	{
		//注意不同类型的DI，其数据来源不同
		if(type == REAL_DI)
		{
			//对于实遥信，需要检查遥信板是否存在，不存在则将遥信刷新为分位置
			//读遥信板识别码,只有高6位有效
			address = (pDIPortData[i].dwPortAddr&0xfffffff0)+DI_BOARD_ID_ADD;
			bBoardId = inportb(address)&0xfc;
			if(bBoardId == DI_BOARD_ID)
				bBIStatus = (~inportb(pDIPortData[i].dwPortAddr));
			else
				bBIStatus = 0;
		}
		else
			bBIStatus = bDIStatus[k];
		
		pDIPortData[i].bOldStatus = bBIStatus;

		//根据每个端口实际的DI位个数来处理
		for(j=0; j<pDIPortData[i].bBiNumInPort; j++)
		{	
			//遥信初始状态处理
			if((pDIBitData[DIIndex].bAttribute & DI_IS_CI) == 0)
			{
				if(bBIStatus&0x01)	//初始状态为合位
				{
					if(pDIBitData[DIIndex].bAttribute & DI_IS_REVERSE)
						BIData.bStatus = BI_OPEN;
					else
						BIData.bStatus = BI_CLOSE;

					//初始化DI处理初始状态
	                pDIBitData[DIIndex].bStatusTab = DI_STEP_6;
				}
				else		//初始状态为分位
				{
					if(pDIBitData[DIIndex].bAttribute & DI_IS_REVERSE)
						BIData.bStatus = BI_CLOSE;
					else
						BIData.bStatus = BI_OPEN;

					//初始化DI处理初始状态
	                pDIBitData[DIIndex].bStatusTab = DI_STEP_0;
				}
				//遥信初始状态写数据库
				DB_WriteBI(pDIBitData[DIIndex].wIndex, BIData.bStatus);
			}
			else	//电度初始状态处理
			{
				//初始化DI处理初始状态
				pDIBitData[DIIndex].bStatusTab = DI_STEP_0;
			}
			DIIndex++;
			bBIStatus >>= 1;
		}
	}
/*	static rrr=FALSE;
	if(rrr==FALSE)
	{
		ReportMsg("bi status init!!!!!,%d",dwRelayMSCnt);
		rrr = TRUE;
	}
	*/
	//WORD ttttt;
}

/*********************************************************************************************
 *
 * 功能描述     DI变位消息响应
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
void CDIProc::OnDIChange(DWORD id)
{

	WORD wDIBitNo;
	BYTE bPortNo, bDIChangeBit, bAttribute;
	DWORD dwCurIntCnt, dwTime;
	TDIBitData *pBitData;
	TBurstBI BurstBI;
	TEventBI SoeData;
	TBIPara BIPara;
	TDIPara DIPara;

	dwCurIntCnt = dwDIIntCnt;
	while(wDIWritePtr != wDIReadPtr)
	{
		//取变位DI的端口序号
		bPortNo = pDIBuffer[wDIReadPtr].bDIPortNo;

		//取变位DI的位置信息
		bDIChangeBit = pDIBuffer[wDIReadPtr].bDIChgBit;
		
		//计算变位DI的总的位序号
		wDIBitNo = GetDIIndex(bPortNo);

		//检查并处理变位DI字节信息中具体发生变位的位置
		for(int i=0; i<pDIPortData[bPortNo].bBiNumInPort; i++)
		{
			pBitData = &pDIBitData[wDIBitNo];
			if(bDIChangeBit & 0x01)	//有变位
			{
				//DI变位状态处理
				switch(pBitData->bStatusTab)
				{
					case DI_STEP_0:
						pBitData->bStatusTab = DI_STEP_3;
						pBitData->qwChangeTime = pDIBuffer[wDIReadPtr].Time;
						pBitData->dwKeepTime = pDIBuffer[wDIReadPtr].dwDITimeCnt;
						break;
					case DI_STEP_3:
						if(dwCurIntCnt > pBitData->dwKeepTime)
							dwTime = dwCurIntCnt - pBitData->dwKeepTime;
						else 
							dwTime = 0xffffffff-pBitData->dwKeepTime+dwCurIntCnt+1;

						if(dwTime < pBitData->dwSmoothTime)
							pBitData->bStatusTab = DI_STEP_4;
						else
						{

							bAttribute = pBitData->bAttribute;
							if(bAttribute&DI_IS_CI) //电度处理
							{
								//刷新脉冲电度
								DB_WriteCI( pBitData->wIndex+INTERGRAL_KWH_COUNT, 1);
							}
							else	//遥信处理
							{
								if(bAttribute&DI_IS_REVERSE)//遥信取反
									BurstBI.bStatus = BI_OPEN;
								else
									BurstBI.bStatus = BI_CLOSE;
								BurstBI.wNo = pBitData->wIndex;
								
								//刷新数据库遥信状态
								DB_WriteBI(BurstBI.wNo, BurstBI.bStatus);

								DB_GetBIPara(BurstBI.wNo, &BIPara);
								DB_GetDIPara(BIPara.wMaptoDINo, &DIPara);
								DIPara.wClass &= DI_ATTR_TYPEMASK;
								if((DIPara.wClass != DI_TYPE_VIR)||((DIPara.wClass == DI_TYPE_VIR)&&(isVirDIValide == TRUE)))
								{
										//刷新遥信变位
										DB_WriteBIE(&BurstBI);
										
										//SOE处理
										//注意，SOE的毫秒时间处理需要仔细考虑
										if(bAttribute&DI_CREAT_SOE)
										{
											SoeData.bStatus = BurstBI.bStatus;
											SoeData.wNo = BurstBI.wNo;
											SoeData.stTime = pBitData->qwChangeTime;
											DB_WriteSOE(&SoeData);
										}
								}
							}

							pBitData->qwChangeTime = pDIBuffer[wDIReadPtr].Time;
							pBitData->dwKeepTime = pDIBuffer[wDIReadPtr].dwDITimeCnt;
							pBitData->bStatusTab = DI_STEP_9;
						}
						break;
					case DI_STEP_4:
					case DI_STEP_5:
						pBitData->bStatusTab = DI_STEP_3;
						break;
					case DI_STEP_6:
						pBitData->bStatusTab = DI_STEP_9;
						pBitData->qwChangeTime = pDIBuffer[wDIReadPtr].Time;
						pBitData->dwKeepTime = pDIBuffer[wDIReadPtr].dwDITimeCnt;
						break;
					case DI_STEP_9:
						if(dwCurIntCnt > pBitData->dwKeepTime)
							dwTime = dwCurIntCnt - pBitData->dwKeepTime;
						else 
							dwTime = 0xffffffff-pBitData->dwKeepTime+dwCurIntCnt+1;
						
						if(dwTime < pBitData->dwSmoothTime)
							pBitData->bStatusTab = DI_STEP_10;
						else
						{
							bAttribute = pBitData->bAttribute;
							if((bAttribute&DI_IS_CI)==0) //遥信处理
							{
								if(bAttribute&DI_IS_REVERSE)//遥信取反
									BurstBI.bStatus = BI_CLOSE;
								else
									BurstBI.bStatus = BI_OPEN;
									
								BurstBI.wNo = pBitData->wIndex;
													
								//刷新数据库遥信状态
								DB_WriteBI(BurstBI.wNo, BurstBI.bStatus);
														
								DB_GetBIPara(BurstBI.wNo, &BIPara);
								DB_GetDIPara(BIPara.wMaptoDINo, &DIPara);
								DIPara.wClass &= DI_ATTR_TYPEMASK;
								if((DIPara.wClass != DI_TYPE_VIR)||((DIPara.wClass == DI_TYPE_VIR)&&(isVirDIValide == TRUE)))
								{
										//刷新遥信变位
										DB_WriteBIE(&BurstBI);
																
										//SOE处理
										//注意，SOE的毫秒时间处理需要仔细考虑
										if(bAttribute&DI_CREAT_SOE)
										{
											SoeData.bStatus = BurstBI.bStatus;
											SoeData.wNo = BurstBI.wNo;
											SoeData.stTime = pBitData->qwChangeTime;
											DB_WriteSOE(&SoeData);
										}
								}
							}
							pBitData->qwChangeTime = pDIBuffer[wDIReadPtr].Time;
							pBitData->dwKeepTime = pDIBuffer[wDIReadPtr].dwDITimeCnt;
							pBitData->bStatusTab = DI_STEP_3;
						}
						
						break;
					case DI_STEP_10:
					case DI_STEP_11:
						pBitData->bStatusTab = DI_STEP_9;
						break;
					default:
						LogError("OnDIChange",FILE_LINE,"status table Error for status = %d\
								!", pBitData->bStatusTab);
						pBitData->bStatusTab = DI_STEP_6;
						break;
				}
			}

			bDIChangeBit >>= 1;
			wDIBitNo++;
		}
		wDIReadPtr++;
		wDIReadPtr %= DI_BUF_LEN;
	}
}

/*********************************************************************************************
 *
 * 功能描述     DI定时消息响应
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
void CDIProc::OnTimeOut(DWORD id)
{
	int i, j;
	WORD wDIBitNo;
	BYTE status, bAttribute;
	DWORD dwTime, dwCurIntCnt;
	TBurstBI BurstBI;
	TEventBI SoeData;
	TDIBitData *pBitData;
	wDIBitNo = 0;
	TBIPara BIPara;
	TDIPara DIPara;

#ifdef TEST_DI
	TestDI();
	return;
#endif	

	dwCurIntCnt = dwDIIntCnt;
	//检查所有字节端口
	for(i=0; i<DIType.wDIPortNum; i++)
	{
		//检查每个字节地址中的有效DI位，无效的跳过
		for(j=0; j<pDIPortData[i].bBiNumInPort; j++)
		{
			pBitData = &pDIBitData[wDIBitNo];
			status = pBitData->bStatusTab;
			switch(status)
			{
				case DI_STEP_0:
					break;
				case DI_STEP_3:
				case DI_STEP_9:
					if(dwCurIntCnt > pBitData->dwKeepTime)
						dwTime = dwCurIntCnt - pBitData->dwKeepTime;
					else 
						dwTime = 0xffffffff-pBitData->dwKeepTime+dwCurIntCnt+1;

					if(dwTime>=pBitData->dwSmoothTime)
					{
						bAttribute = pBitData->bAttribute;
						if(bAttribute&DI_IS_CI)	//电度处理
						{
							if(status == DI_STEP_3)
							{
								//刷新脉冲电度
								DB_WriteCI( pBitData->wIndex+INTERGRAL_KWH_COUNT, 1);
							}
						}
						else	//遥信处理
						{
							if(status == DI_STEP_3)
							{
								if(bAttribute&DI_IS_REVERSE)//遥信取反
									BurstBI.bStatus = BI_OPEN;
								else
									BurstBI.bStatus = BI_CLOSE;
							}
							else if(status == DI_STEP_9)
							{
								if(bAttribute&DI_IS_REVERSE)//遥信取反
									BurstBI.bStatus = BI_CLOSE;
								else
									BurstBI.bStatus = BI_OPEN;
							}
							BurstBI.wNo = pBitData->wIndex;
							
							//刷新数据库遥信状态
							DB_WriteBI(BurstBI.wNo, BurstBI.bStatus);

							DB_GetBIPara(BurstBI.wNo, &BIPara);
							DB_GetDIPara(BIPara.wMaptoDINo, &DIPara);
							DIPara.wClass &= DI_ATTR_TYPEMASK;
							if((DIPara.wClass != DI_TYPE_VIR)||((DIPara.wClass == DI_TYPE_VIR)&&(isVirDIValide == TRUE)))
							{
									//刷新遥信变位
									DB_WriteBIE(&BurstBI);
									
									//SOE处理
									//注意，SOE的毫秒时间处理需要仔细考虑
									if(bAttribute&DI_CREAT_SOE)
									{
										SoeData.bStatus = BurstBI.bStatus;
										SoeData.wNo = BurstBI.wNo;
										SoeData.stTime = pBitData->qwChangeTime;
										DB_WriteSOE(&SoeData);
									}
							}
						}
						if(status == DI_STEP_3)
							pBitData->bStatusTab = DI_STEP_6;
						else if(status == DI_STEP_9)
							pBitData->bStatusTab = DI_STEP_0;
					}
						
					break;
				case DI_STEP_4:
					pBitData->bStatusTab = DI_STEP_5;
					break;
				case DI_STEP_5:
					pBitData->bStatusTab = DI_STEP_0;
					break;
				case DI_STEP_6:
					break;
				case DI_STEP_10:
					pBitData->bStatusTab = DI_STEP_11;
					break;
				case DI_STEP_11:
					pBitData->bStatusTab = DI_STEP_6;
					break;
				default:
					LogError("OnTimeOut",FILE_LINE,"status table Error for status = %d\
							!", pBitData->bStatusTab);
					break;
			}
			wDIBitNo++;
		}
	}

	//测试雪崩
//	if((dwDITimeCnt%200)==0)
//		ReportMsg("SOE number=%d.", dwSOECnt);
}

/*********************************************************************************************
 *
 * 功能描述     计算指定字节端口序号所对应的总的DI序号
 *
 * 参数说明      - bPortNo	: 输入 	DI字节端口序号
 *				 
 *
 * 返回代码
 *                DI序号
 *
 * 其它说明：          
 *
**********************************************************************************************/
WORD CDIProc::GetDIIndex(BYTE bPortNo)
{
	WORD wIndex = 0;
	ASSERT(bPortNo <= (MAX_DI_NUM/DI_BOARD_WIDTH));
	if(bPortNo > (MAX_DI_NUM/DI_BOARD_WIDTH))
	{
		LogError("GetDIIndex",FILE_LINE,"bPortNo error !");
		ErrorProc();
		return 0;
	}
	for(int i=0; i<bPortNo; i++)
		wIndex += pDIPortData[i].bBiNumInPort;
	return wIndex;
}
/*********************************************************************************************
 *
 * 功能描述     DI采集任务异常处理
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
void CDIProc::ErrorProc()
{
	OS_Suspend(0);
}

/*********************************************************************************************
 *
 * 功能描述     DI采集定时中断处理
 *
 * 参数说明      - arg	: 输入 	备用
 *				 
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void DITimerInt()  //0.5ms中断一次
{
	BYTE bDIStatus, bChgBit;
	STATUS status = 0;
	static DWORD dwCnt, dwtemp;
	
	//毫秒时间计算，毫秒以上时间由B格式获取，毫秒时间单独处理，不进位。毫秒清零由B格式整秒
	//中断处理
	if(wMSecondCnt<1999)
	{
		wMSecondCnt++;
	}
	//检查DI任务初始化是否完成
	if(bDIInitFlag == FALSE)
		return;

	dwCnt++;
	dwDIIntCnt = dwCnt;
	
	//扫描所有DI端口的DI状态
	status = ScanDIPort(REAL_DI)|ScanDIPort(INNER_DI)|ScanDIPort(VIR_DI);

	//有DI状态发生变化，通知DI任务进行处理
	if(status)
	{
		dwtemp++;
//		printf("DI task postmsg %d.\n", dwtemp);
		OS_PostMessage(dwTaskDIId, SM_DICHANGE, 0, 0, 0, 0);
	}

}

/*********************************************************************************************
 *
 * 功能描述     DI端口状态扫描处理
 *
 * 参数说明      - type	: 输入 	DI类型
 *				 
 *
 * 返回代码
 *                是否有状态变化
 *
 * 其它说明：          
 *
**********************************************************************************************/
STATUS ScanDIPort(BYTE type)
{
	
	BYTE bDIData[10], bDIStatus, bChgBit;
	BYTE start,end;
	STATUS status = 0;
	int i, j;
	BYTE bBoardId;
	DWORD address;
	
	memset(bDIData, 0, sizeof(bDIData));

	start = DIType.wDIPortBase[type];
	
	//根据DI不同类型确定各自的端口范围，对于内部遥信和虚遥信，预先读取全部状态信息
	switch(type)
	{
		case REAL_DI:
			end = DIType.wDIPortBase[INNER_DI];
			break;
		case INNER_DI:
			end = DIType.wDIPortBase[VIR_DI];
			ReadAllInnerDI(bDIData);
			break;
		case VIR_DI:
			end = DIType.wDIPortNum;
			ReadAllVirDI(bDIData);
			break;
	}
	//根据类型不同，分别扫描DI端口的DI状态
	for(i=start, j=0; i<end; i++, j++)
	{
		//注意数据来源不同
		if(type == REAL_DI)
		{
			//对于实遥信，需要检查遥信板是否存在，不存在则将遥信刷新为分位置
			//读遥信板识别码,只有高6位有效
			address = (pDIPortData[i].dwPortAddr&0xfffffff0)+DI_BOARD_ID_ADD;
			
			//向一个不用的地址写0xFF将数据总线刷成高电平，防止因为两块遥信板的识别ID连续读取错误的问题。
			outportb(0xb0030050, 0xff);
			bBoardId = inportb(address)&0xfc;
			if(bBoardId == DI_BOARD_ID)
				bDIStatus = (~inportb(pDIPortData[i].dwPortAddr));
			else
				bDIStatus = 0;
//			bDIStatus = inportb(pDIPortData[i].dwPortAddr);
		}
		else
			bDIStatus = bDIData[j];

		//检查是否有变位
		bChgBit = bDIStatus ^ pDIPortData[i].bOldStatus;
		if(bChgBit)
		{
			pDIBuffer[wDIWritePtr].bDIStatus = bDIStatus;	//记录变位时状态	
			pDIBuffer[wDIWritePtr].bDIPortNo = i;			//记录变位字节端口序号
			ReadAbsTime(&pDIBuffer[wDIWritePtr].Time);		//记录变位时系统时间
			pDIBuffer[wDIWritePtr].dwDITimeCnt = dwDIIntCnt;//记录变位时DI中断计数	
			pDIBuffer[wDIWritePtr].bDIChgBit = bChgBit;		//记录变位位的具体位置
			pDIPortData[i].bOldStatus = bDIStatus;
			wDIWritePtr++;
			wDIWritePtr %= DI_BUF_LEN;
			status = 1;
		}
	}
	return status;
}
	
//临时定义读取全部内部遥信接口函数
STATUS ReadAllInnerDI(BYTE *pBuf)
{

	DWORD AllStatus, data, ValidStatus;
	int i, j;
	BYTE status;
	
	DWORD dwBase[] = {DOA_CS_BASE+28, DOC_CS_BASE+28};

	//读取所有的内部遥信状态,注意由于硬件资源限制，最大24个内部遥信，对于3块遥控板，只有前两块板的24个内部遥信有效
	AllStatus = 0;	
	for(i=0; i<2; i++)
	{
		data = (~((DWORD)MAKEWORD(inportb(dwBase[i]), inportb(dwBase[i]+1))));
		data &= 0x0fff;
		
//		if(data)
//			printf("\n the board %d's inner yx status = 0x%x.", i, data);
		AllStatus |= (data<<(i*MAX_IN_DI_NUM_PER_BOARD));
	}

	

	//处理最低位即信号复归遥信
	AllStatus <<= 1;
	status = (inportb(KEY2_CS_BASE)&0x01)^0x01;
	AllStatus |= status;//(inportb(KEY2_CS_BASE)&0x01);	//从键盘端口上读取信号复归键值

	//挑出有效位,并集中存储
	ValidStatus = 0;
	j = 0;
	for(i=0; i<24; i++)	//最大只有24位资源
	{
		if(dwInDIBitFlag&(0x01<<i))
		{
			ValidStatus |= (((AllStatus>>i)&0x01)<<j);
			j++;
		}
	}
	if(ValidStatus != 0)
		status = 1;
	//将有效位保存到缓冲区
	for(j=0; j<((DIType.wDIBitNum[INNER_DI]+7)/8); j++)
	{
		pBuf[j] = (BYTE)ValidStatus;
		ValidStatus >>= 8;
	}
	
	return ERR_SUCCESS;
}

//临时定义读取全部虚遥信接口函数
/*
STATUS ReadAllVirDI(BYTE *pBuf)
{
	return ERR_SUCCESS;
}
*/

#ifdef TEST_DI
void TestDI()
{
	BYTE bDIStatus[5];

	for(int i=0; i<5; i++)
	{
		bDIStatus[i] = inportb(BIA_CS_BASE+i);
	}
	ReportMsg("YX0=%x YX1=%x YX2=%x YX3=%x YX4=%x .", bDIStatus[0],bDIStatus[1],bDIStatus[2],\
		bDIStatus[3], bDIStatus[4]);
}

#endif

