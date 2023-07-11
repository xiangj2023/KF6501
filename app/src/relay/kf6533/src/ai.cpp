/*******************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
*******************************************************************************************/ 
                          
/*******************************************************************************************
*                                                                                  
* 文件名称          
*			ai.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           		遥测任务                                                                      
*			                                                                
* 描述                                                                             
*                                                                                  
*                                
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	
*		
*		
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      yanzh             09/08/20    初始创建                         
*                                                                                  
*******************************************************************************************/
#include "ai.h"
#include "m5234timer.h"
#include "max11046.h"
#include "led.h"
#include "RelayMemInit.h"
#include "RelayDef.h"
#include "dbasedef.h"
#include "dbfunc.h"
#include "logic.h"
#include "logicdef.h"
#include "daedef.h"
#include "relayfun.h"
#include "hmiapp.h"


DWORD dwAITaskID;
extern TSysConfigTable	*G_pCfgTable;
extern DWORD g_hLCDTaskID;

BEGIN_MESSAGE_MAP(CAIProc,CApp)
	ON_SM_TIMEOUT(CAIProc)
	ON_SM_AUTOADJUST(CAIProc)	
END_MESSAGE_MAP()

/*********************************************************************************************
 * 功能: AI任务入口函数
 * 输入: pdwApp	
 * 返回: 无
 * 备注: 无        
**********************************************************************************************/

void AITask(DWORD *pdwApp)
{
	CAIProc *pAITask = new CAIProc(pdwApp);
	pAITask->Init();
	pAITask->Run();
}	
/*********************************************************************************
*功能: 遥测初始化子程序
*输入: 无
*返回: 无
*备注: 无
**************************************************************************************/

void CAIProc::Init(void)
{
	
	m_AIDataMask = ((0x01<<CHANNEL_6533_UA)|(0x01<<CHANNEL_6533_UB)|(0x01<<CHANNEL_6533_UC)|\
						(0x01<<CHANNEL_6533_U0h)|(0x01<<CHANNEL_6533_Ua)|(0x01<<CHANNEL_6533_Ub) \
						|(0x01<<CHANNEL_6533_Uc)|(0x01<<CHANNEL_6533_U0l)|(0x01<<CHANNEL_6533_IA)
						|(0x01<<CHANNEL_6533_IB)|(0x01<<CHANNEL_6533_IC)|(0x01<<CHANNEL_6533_Ia)\
						|(0x01<<CHANNEL_6533_Ib)|(0x01<<CHANNEL_6533_Ic)|(0x01<<CHANNEL_6533_I0)
						|(0x01<<CHANNEL_6533_Ijx));
	
	m_pDataBuf = (LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*16);
	m_pCoefAdjustBuf =(LONG*)OS_MemAlloc(sizeof(DWORD)*READ_POINT_NUM*16);
		
	ASSERT(m_pDataBuf != NULL);
	if(m_pDataBuf == NULL)
	{
		LogError("CAIProc,m_pDataBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pDataBuf, 0, sizeof(DWORD)*READ_POINT_NUM*16);
	

	ASSERT(m_pCoefAdjustBuf != NULL);
	if(m_pCoefAdjustBuf == NULL)
	{
		LogError("CAIProc,m_pCoefAdjustBuf",FILE_LINE,"malloc error.");
		return;
	}
	memset(m_pCoefAdjustBuf, 0, sizeof(DWORD)*READ_POINT_NUM*16);

	
	for(LONG i=0;i<51;i++) 
	{
	    m_pAdjustData[i]=0;
	}

	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{
		//遥测量数组，保存最新的遥测值NUMOFYCVALUE个，取中值写入数据库
		UAArray[i] = 0;
		UBArray[i] = 0;
		UCArray[i] = 0;
		U0hArray[i] = 0;
		UaArray[i] = 0;
		UbArray[i] = 0;
		UcArray[i] = 0;
		U0lArray[i] = 0;
		IAArray[i] = 0;
		IBArray[i] = 0;
		ICArray[i] = 0;
		IaArray[i] = 0;
		IbArray[i] = 0;
		IcArray[i] = 0;
		I0Array[i] = 0;
		IjxArray[i] = 0;

	    //系数校正电量排序数组
		ArrayadjUA[i] = 0;
		ArrayadjUB[i] = 0;
		ArrayadjUC[i] = 0;
		ArrayadjU0h[i] = 0;
		ArrayadjUa[i] = 0;
		ArrayadjUb[i] = 0;
		ArrayadjUc[i] = 0;
		ArrayadjU0l[i] = 0;
		ArrayadjIA[i] = 0;
		ArrayadjIB[i] = 0;
		ArrayadjIC[i] = 0;
		ArrayadjIa[i] = 0;
		ArrayadjIb[i] = 0;
		ArrayadjIc[i] = 0;
		ArrayadjI0[i] = 0;
		ArrayadjIjx[i] = 0;			
		ArrayadjUAp[i] = 0;
		ArrayadjUBp[i] = 0;
		ArrayadjUCp[i] = 0;
		ArrayadjU0hp[i] = 0;
		ArrayadjUap[i] = 0;
		ArrayadjUbp[i] = 0;
		ArrayadjUcp[i] = 0;
		ArrayadjU0lp[i] = 0;
		ArrayadjIAp[i] = 0;
		ArrayadjIBp[i] = 0;
		ArrayadjICp[i] = 0;
		ArrayadjIap[i] = 0;
		ArrayadjIbp[i] = 0;
		ArrayadjIcp[i] = 0;
		ArrayadjI0p[i] = 0;
		ArrayadjIjxp[i] = 0;		
		m_pBubbleData[i] = 0;

	}
	//初始化系数校准标志
	m_bIsCoefAdjust = FALSE;
	haveAdjustElecSource = FALSE;

    wCoefAdjustNum = 0;
	wWaitTimeNum = 0;

	m_dwAdjustCounter=0;
	dwAITaskID = GetTaskId();
	//设置AI定时器
	SetTimer(1,AI_TIMER_PERIOD);
	
}
/*********************************************************************************
*功能: AI定时消息响应
*输入: 定时器ID
*输出: 无
*返回: 无
**************************************************************************************/

void CAIProc::OnTimeOut(DWORD id)
{
 
	QWORD SS;
	STATUS rc;
	//开始系数校准
	if(m_bIsCoefAdjust == TRUE)
	{
		CoefAdjust();	
		return;
	}

	rc = ReadSampleData(m_pDataBuf,m_AIDataMask,0);		//读取原始采样数据
	if(rc == ERR_FAILURE)
		return;

	//测量通道滤波
	for(LONG i=0;i<16;i++)
	{
		Filter(m_pDataBuf+READ_POINT_NUM*i);
	}
	
	
	//均方根法求测量值的模值
	CaluMSMod(m_pDataBuf,UA);						
	CaluMSMod(m_pDataBuf+READ_POINT_NUM,UB);			
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*2,UC);		
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*3,U0h);      
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*4,Ua);       
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*5,Ub); 
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*6,Uc); 
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*7,U0l); 

	CaluMSMod(m_pDataBuf+READ_POINT_NUM*8,IA);			
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*9,IB);		
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*10,IC);      
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*11,Ia);       
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*13,Ib); 
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*15,Ic); 
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*12,I0);
	CaluMSMod(m_pDataBuf+READ_POINT_NUM*14,Ijx);
	
	CaluMSPQ(m_pDataBuf,m_pDataBuf+READ_POINT_NUM*8,*CF_pnAA,Pa,Qa);				//YANXUESONG 20120726
	CaluMSPQ(m_pDataBuf+READ_POINT_NUM,m_pDataBuf+READ_POINT_NUM*9,*CF_pnBA,Pb,Qb);
	CaluMSPQ(m_pDataBuf+READ_POINT_NUM*2,m_pDataBuf+READ_POINT_NUM*10,*CF_pnCA,Pc,Qc);
	if(((*CF_pnPA)==0)||((*CF_pnPB)==0)||((*CF_pnPC)==0)||((*CF_pnQA)==0)||((*CF_pnQB)==0)||((*CF_pnQC)==0))
	{
		P = 0;
		Q = 0;
		cos = 0;
	}
	else
	{
		LLONG S;
	   	P = (LLONG)Pa*10000/(*CF_pnPA)+(LLONG)Pb*10000/(*CF_pnPB)+(LLONG)Pc*10000/(*CF_pnPC);
	   	Q = (LLONG)Qa*10000/(*CF_pnQA)+(LLONG)Qb*10000/(*CF_pnQB)+(LLONG)Qc*10000/(*CF_pnQC);
	   	SS = P*P+Q*Q;
		S = Evolution64(SS);
		if(S<=20)
			cos = 0;
	   	else
			cos = 10000*P/(LONG)S;
	   	if(ToAbs(cos)>9990)
			Q = 0;
	   	if(ToAbs(cos)<10)
			P =0;
	   	cos = cos/10;
	}

	//写入遥测值
	WriteAIData(M_UHA, UA, *CF_pnUAD, m_pBubbleData , UAArray ,TRUE);
	WriteAIData(M_UHB, UB, *CF_pnUBD, m_pBubbleData , UBArray ,TRUE);
	WriteAIData(M_UHC, UC, *CF_pnUCD, m_pBubbleData , UCArray ,TRUE);
	WriteAIData(M_U0H, U0h, *CF_pnU0hD, m_pBubbleData , U0hArray ,TRUE);
	WriteAIData(M_ULA, Ua, *CF_pnUaD, m_pBubbleData , UaArray ,TRUE);
	WriteAIData(M_ULB, Ub, *CF_pnUbD, m_pBubbleData , UbArray ,TRUE);
	WriteAIData(M_ULC, Uc, *CF_pnUcD, m_pBubbleData , UcArray ,TRUE);
	WriteAIData(M_U0L, U0l, *CF_pnU0lD, m_pBubbleData , U0lArray ,TRUE);
	WriteAIData(M_IHA, IA, *CF_pnIAD, m_pBubbleData , IAArray ,TRUE);
	WriteAIData(M_IHB, IB, *CF_pnIBD, m_pBubbleData , IBArray ,TRUE);
	WriteAIData(M_IHC, IC, *CF_pnICD, m_pBubbleData , ICArray ,TRUE);
	WriteAIData(M_ILA, Ia, *CF_pnIaD, m_pBubbleData , IaArray ,TRUE);
	WriteAIData(M_ILB, Ib, *CF_pnIbD, m_pBubbleData , IbArray ,TRUE);
	WriteAIData(M_ILC, Ic, *CF_pnIcD, m_pBubbleData , IcArray ,TRUE);
	WriteAIData(M_I0, I0, *CF_pnI0D, m_pBubbleData , I0Array ,TRUE);
	WriteAIData(M_IJX, Ijx, *CF_pnIjxD, m_pBubbleData , IjxArray ,TRUE);
	WriteAIData(M_P, P, m_pBubbleData, PArray);
	WriteAIData(M_Q, Q, m_pBubbleData, QArray);
	WriteAIData(M_COS, cos, m_pBubbleData, cosArray);
	
	RefreshCommRW(RW_ET11ERR,RW_COM1ERR,RW_CAN1ERR,RW_MAINTLINK);

/*	UA = AiBubbleSort(UA,UAArray);
	if(UA<=*CF_pnUAD)		
		UA = 0;
	DB_WriteAI(M_UHA,UA);

	UB = AiBubbleSort(UB,UBArray);
	if(UB<=*CF_pnUBD)		
		UB = 0;
	DB_WriteAI(M_UHB,UB);
	
	UC = AiBubbleSort(UC,UCArray);
	if(UC<=*CF_pnUCD)		
		UC = 0;
	DB_WriteAI(M_UHC,UC);

	U0h = AiBubbleSort(U0h,U0hArray);
	if(U0h<=*CF_pnU0hD)		
		U0h = 0;
	DB_WriteAI(M_U0H,U0h);

	Ua = AiBubbleSort(Ua,UaArray);
	if(Ua<=*CF_pnUaD)		
		Ua = 0;
	DB_WriteAI(M_ULA,Ua);

	Ub = AiBubbleSort(Ub,UbArray);
	if(Ub<=*CF_pnUbD)		
		Ub = 0;
	DB_WriteAI(M_ULB,Ub);
	
	Uc = AiBubbleSort(Uc,UcArray);
	if(Uc<=*CF_pnUcD)		
		Uc = 0;
	DB_WriteAI(M_ULC,Uc);

	U0l = AiBubbleSort(U0l,U0lArray);
	if(U0l<=*CF_pnU0lD) 	
		U0l = 0;
	DB_WriteAI(M_U0L,U0l);

	IA = AiBubbleSort(IA,IAArray);
	if(IA<=*CF_pnIAD)		
		IA = 0;
	DB_WriteAI(M_IHA,IA);

	IB = AiBubbleSort(IB,IBArray);
	if(IB<=*CF_pnIBD)		
		IB = 0;
	DB_WriteAI(M_IHB,IB);

	IC = AiBubbleSort(IC,ICArray);
	if(IC<=*CF_pnICD)		
		IC = 0;
	DB_WriteAI(M_IHC,IC);

	Ia = AiBubbleSort(Ia,IaArray);
	if(Ia<=*CF_pnIaD)		
		Ia = 0;
	DB_WriteAI(M_ILA,Ia);

	Ib = AiBubbleSort(Ib,IbArray);
	if(Ib<=*CF_pnIbD)		
		Ib = 0;
	DB_WriteAI(M_ILB,Ib);

	Ic = AiBubbleSort(Ic,IcArray);
	if(Ic<=*CF_pnIcD)		
		Ic = 0;
	DB_WriteAI(M_ILC,Ic);

	I0 = AiBubbleSort(I0,I0Array);
	if(I0<=*CF_pnI0D)		
		I0 = 0;
	DB_WriteAI(M_I0,I0);

	Ijx = AiBubbleSort(Ijx,IjxArray);
	if(Ijx<=*CF_pnIjxD)		
		Ijx = 0;
	DB_WriteAI(M_IJX,Ijx);	*/			//yanxuesong 20120727			
	
}

void CAIProc::OnAutoAdjust(DWORD arg,DWORD Uval,DWORD Ival)
{
	//设置自动校准标志

	if((Uval == 0)&&(Ival == 0))
	{
		m_bIsDeadValAdjust = TRUE;	
		
		adjUADV = 0;
		adjUBDV = 0;
		adjUCDV = 0;
		adjU0hDV = 0;
		adjUaDV = 0;
		adjUbDV = 0;
		adjUcDV = 0;
		adjU0lDV = 0;
		adjIADV = 0;
		adjIBDV = 0;
		adjICDV = 0;
		adjIaDV = 0;
		adjIbDV = 0;
		adjIcDV = 0;
		adjI0DV = 0;
		adjIjxDV = 0; 
	}
	else
		m_bIsDeadValAdjust = FALSE;

	dwMsgID = arg;
	m_bIsCoefAdjust = TRUE;
	haveAdjustElecSource = FALSE;
	m_dwSourceU = Uval;
	m_dwSourceI = Ival;
	wWaitTimeNum = 0;
	

	MidUA = 0;
	MidUB = 0;
	MidUC = 0;
	MidU0h = 0;
	MidUa = 0;
	MidUb = 0;
	MidUc = 0;
	MidU0l = 0;
	MidIA = 0;
	MidIB = 0;
	MidIC = 0;
	MidIa = 0;
	MidIb = 0;
	MidIc = 0;
	MidI0 = 0;
	MidIjx = 0;
	MidUAp = 0;
	MidUBp = 0;
	MidUCp = 0;
	MidU0hp = 0;
	MidUap = 0;
	MidUbp = 0;
	MidUcp = 0;
	MidU0lp = 0;
	MidIAp = 0;
	MidIBp = 0;
	MidICp = 0;
	MidIap = 0;
	MidIbp = 0;
	MidIcp = 0;
	MidI0p = 0;
	MidIjxp = 0;

	 for(LONG i=0;i<22;i++)
	 	m_pAdjustData[i]=0;

}
/**************************************************************************************************
* 功能: 计算遥测量有功，无功，功率因数
* 参数: 滤波后的数据缓冲区
* 返回: 无
* 备注:
***************************************************************************************************/
void CAIProc::CaluMSPQ(LONG *UBuf,LONG *IBuf,LONG DACoef, LONG &MSP,LONG &MSQ)
{
	LLONG   p 	=0;
	LLONG   q	=0;
	LONG    tp 	= 0;
	LONG    tq 	= 0;
	//功率计算
	for(LONG i=0;i<32;i++)
	{
		p += UBuf[1+i]*IBuf[1+i];
		q += IBuf[1+i]*(UBuf[i]-UBuf[2+i]);
	}
    tp = p/3200;
	tq = q/SINWTS;
	//功率校正
	MSP = tp + tq*DACoef/10000;     
	MSQ = tq - tp*DACoef/10000;  
	
}

/************************************************************************************************
*功能: 处理系数校正中的计算
*参数: 
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjustCalu(void)
{

	ReadSampleData(m_pCoefAdjustBuf,m_AIDataMask,0);		//读取原始采样数据	
	
	
	CaluBaseFourier_S15(m_pCoefAdjustBuf,&adjUAp);
	CaluModValue(&adjUAp);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM,&adjUBp);
	CaluModValue(&adjUBp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*2,&adjUCp);
	CaluModValue(&adjUCp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*3,&adjU0hp);
	CaluModValue(&adjU0hp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*4,&adjUap);
	CaluModValue(&adjUap);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*5,&adjUbp);
	CaluModValue(&adjUbp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*6,&adjUcp);
	CaluModValue(&adjUcp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*7,&adjU0lp);
	CaluModValue(&adjU0lp);	
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*8,&adjIAp);
	CaluModValue(&adjIAp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*9,&adjIBp);
	CaluModValue(&adjIBp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*10,&adjICp);
	CaluModValue(&adjICp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*11,&adjIap);
	CaluModValue(&adjIap);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*13,&adjIbp);
	CaluModValue(&adjIbp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*15,&adjIcp);
	CaluModValue(&adjIcp);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*12,&adjI0p);
	CaluModValue(&adjI0p);
	CaluBaseFourier_S15(m_pCoefAdjustBuf+READ_POINT_NUM*14,&adjIjxp);
	CaluModValue(&adjIjxp);

	for(LONG i=0;i<16;i++)
	{
		Filter(m_pCoefAdjustBuf+READ_POINT_NUM*i);
	}
	
	CaluMSMod(m_pCoefAdjustBuf,adjUA);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM,adjUB);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*2,adjUC);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*3,adjU0h);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*4,adjUa);
    CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*5,adjUb);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*6,adjUc);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*7,adjU0l);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*8,adjIA);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*9,adjIB);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*10,adjIC);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*11,adjIa);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*13,adjIb);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*15,adjIc);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*12,adjI0);
	CaluMSMod(m_pCoefAdjustBuf+READ_POINT_NUM*14,adjIjx);
	
	
}
/************************************************************************************************
*功能: 系数校正
*参数: 滤波后数据缓冲区
*返回: 无
*备注:
*************************************************************************************************/
void CAIProc::CoefAdjust(void)
{
	if(m_dwAdjustCounter>99)
	{
       if(m_bIsDeadValAdjust == TRUE)
		{
			
			adjUADV 	= m_pAdjustData[32];
			adjUBDV 	= m_pAdjustData[33];
			adjUCDV 	= m_pAdjustData[34];
			adjU0hDV 	= m_pAdjustData[35];
			adjUaDV 	= m_pAdjustData[36];
			adjUbDV 	= m_pAdjustData[37];
			adjUcDV 	= m_pAdjustData[38];
			adjU0lDV 	= m_pAdjustData[39];
			adjIADV 	= m_pAdjustData[40];
			adjIBDV 	= m_pAdjustData[41];
			adjICDV 	= m_pAdjustData[42];
			adjIaDV 	= m_pAdjustData[43];
			adjIbDV 	= m_pAdjustData[44];
			adjIcDV 	= m_pAdjustData[45];
			adjI0DV 	= m_pAdjustData[46];
			adjIjxDV 	= m_pAdjustData[47]; 
			
			CoefAdjustEnd(SUCCESS);
			return;
       	}
       else
		{
		   LONG Value[52];

			Value[0]	= (m_pAdjustData[0]+m_pAdjustData[1]+m_pAdjustData[2]+ \
						  m_pAdjustData[4]+m_pAdjustData[5]+m_pAdjustData[6])/(m_dwSourceU*6);
			Value[1]	= m_pAdjustData[3]/m_dwSourceU;
			Value[2]	= m_pAdjustData[7]/m_dwSourceU;
			Value[3]	= (m_pAdjustData[8]+m_pAdjustData[9]+m_pAdjustData[10]+ \
						  m_pAdjustData[11]+m_pAdjustData[12]+m_pAdjustData[13])/(m_dwSourceI*60);
			Value[4]	= m_pAdjustData[14]/(m_dwSourceI*10);
			Value[5]	= m_pAdjustData[15]/(m_dwSourceI*10);
			Value[6]	= Value[3];
			Value[7]	= m_pAdjustData[16]/m_dwSourceU;
			Value[8]	= m_pAdjustData[17]/m_dwSourceU;
			Value[9]	= m_pAdjustData[18]/m_dwSourceU;
			Value[10]	= m_pAdjustData[19]/m_dwSourceU;
			Value[11]	= m_pAdjustData[20]/m_dwSourceU;
			Value[12]	= m_pAdjustData[21]/m_dwSourceU;
			Value[13]	= m_pAdjustData[22]/m_dwSourceU;
			Value[14]	= m_pAdjustData[23]/m_dwSourceU;
			Value[15]	= m_pAdjustData[24]/(m_dwSourceI*10);
			Value[16]	= m_pAdjustData[25]/(m_dwSourceI*10);
			Value[17]	= m_pAdjustData[26]/(m_dwSourceI*10);
			Value[18]	= m_pAdjustData[27]/(m_dwSourceI*10);
			Value[19]	= m_pAdjustData[28]/(m_dwSourceI*10);
			Value[20]	= m_pAdjustData[29]/(m_dwSourceI*10);
			Value[21]	= m_pAdjustData[30]/(m_dwSourceI*10);
			Value[22]	= m_pAdjustData[31]/(m_dwSourceI*10);
			
			Value[23]	= adjUADV;
			Value[24]	= adjUBDV;
			Value[25]	= adjUCDV;
			Value[26]	= adjU0hDV;
			Value[27]	= adjUaDV;
			Value[28]	= adjUbDV;
			Value[29]	= adjUcDV;
			Value[30]	= adjU0lDV;
			Value[31]	= adjIADV;
			Value[32]	= adjIBDV;
			Value[33]	= adjICDV;
			Value[34]	= adjIaDV;
			Value[35]	= adjIbDV;
			Value[36]	= adjIcDV;
			Value[37]	= adjI0DV;
			Value[38]	= adjIjxDV;
			Value[39]	= 1000;
			Value[40]	= 100;
			Value[41]	= 10;
			Value[42]	= 1;
			Value[43]	= m_pAdjustData[48]/(m_dwSourceU*m_dwSourceI);		//pa，保存100W的系数;
			Value[44]	= Value[43];										//qa
			Value[45]	= m_pAdjustData[49]/(m_dwSourceU*m_dwSourceI);		//pb，保存100W的系数;
			Value[46]	= Value[45];										//qb
			Value[47]	= m_pAdjustData[50]/(m_dwSourceU*m_dwSourceI);		//pc，保存100W的系数;
			Value[48]	= Value[45];										//qc
			Value[49]	= adjAA;	
			Value[50]	= adjBA;
			Value[51]	= adjCA;
		

			DB_SetCFValue(CF_VALUE_ALL, Value);				//写入系数值
		    DB_SetCFValueConfirm(CF_VALUE_ALL,DB_EXECUTE);	//写入系数确认
		    CoefAdjustEnd(SUCCESS);
		    return;
       	}
	}
	
	CoefAdjustCalu();

   	
	if(!haveAdjustElecSource)  //首先校正角差及电压电流源检测
	{
      LONG ElecVal[]={adjUAp.Mod,adjUBp.Mod,adjUCp.Mod,adjU0hp.Mod,adjUap.Mod,adjUbp.Mod,adjUcp.Mod,adjU0lp.Mod, \
	  	    adjIAp.Mod,adjIBp.Mod,adjICp.Mod,adjIap.Mod,adjIbp.Mod,adjIcp.Mod,adjI0p.Mod,adjIjxp.Mod};
		
	  LONG ElecCoef[]={*CF_pnUp,*CF_pnUp,*CF_pnUp,*CF_pnU0h,*CF_pnUp,*CF_pnUp,*CF_pnUp,*CF_pnU0l, \
	  	    *CF_pnIp,*CF_pnIp,*CF_pnIp,*CF_pnIp,*CF_pnIp,*CF_pnIp,*CF_pnI0,*CF_pnIjx};
	  
	  if(m_bIsDeadValAdjust == TRUE) //死区值校正源判断
		{

			for(LONG i=0;i<8;i++)
			{
				if(ElecVal[i] > ElecCoef[i]/200) // 与小值0.5V作比较
				{
					CoefAdjustEnd(FAILURE);
					return;
				}	
			}			
			for(LONG i=8;i<16;i++)
			{
				if(ElecVal[i] > ElecCoef[i]/5) // 与小值0.2A作比较
				{
					CoefAdjustEnd(FAILURE);
					return;
				}	
			}			
		}
      else	//系数值校正源判断
	    {
			for(LONG i=0;i<8;i++)
			{				
				if((ElecVal[i]<(ElecCoef[i]*m_dwSourceU*9/1000))||(ElecVal[i]>(ElecCoef[i]*m_dwSourceU*11/1000)))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			for(LONG i=8;i<16;i++)
			{				
				if((ElecVal[i]<(ElecCoef[i]*m_dwSourceI*9/100))||(ElecVal[i]>(ElecCoef[i]*m_dwSourceI*11/100)))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}
			for(int i=1;i<16;i++)
			{
				if(!PhaseCheck(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*i))
				{
					CoefAdjustEnd(FAILURE);
					return;
				}
			}			
      	}
	  	CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*8,0,adjPa,adjQa);		
		CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM,m_pCoefAdjustBuf+READ_POINT_NUM*9,0,adjPb,adjQb);
		CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_pCoefAdjustBuf+READ_POINT_NUM*10,0,adjPc,adjQc);

		if((adjQa>*CF_pnQA/3)||(adjQb>*CF_pnQB/3)||(adjQc>*CF_pnQC/3))
		{
			CoefAdjustEnd(FAILURE);
			return;
		}


		//LONG temp1,temp2;
		if(adjQa == 0)
			adjAA = 0;
		else
		{
			//temp1 = Evolution64((LLONG)AdjPa*AdjPa+(LLONG)AdjQa*AdjQa);
			//temp2 = AdjQa*10000;
			//AdjAA = temp2/temp1;
			adjAA = adjQa*10000/((LONG)Evolution64((LLONG)adjPa*adjPa+(LLONG)adjQa*adjQa));
		}
		
		if(adjQb == 0)
			adjBA = 0;
		else
		{			
			//temp1 = Evolution64((LLONG)AdjPb*AdjPb+(LLONG)AdjQb*AdjQb);	
			//temp2 = AdjQb*10000;
			//AdjBA = temp2/temp1;
			adjBA = adjQb*10000/((LONG)Evolution64((LLONG)adjPb*adjPb+(LLONG)adjQb*adjQb));		
		}

		if(adjQc == 0)
			adjCA = 0;
		else
		{			
			//temp1 = Evolution64((LLONG)AdjPc*AdjPc+(LLONG)AdjQc*AdjQc);
			//temp2 = AdjQc*10000;
			//AdjCA = temp2/temp1;
			adjCA = adjQc*10000/((LONG)Evolution64((LLONG)adjPc*adjPc+(LLONG)adjQc*adjQc));
		}

		
		haveAdjustElecSource = TRUE;
		return;
	}
	
	CaluMSPQ(m_pCoefAdjustBuf,m_pCoefAdjustBuf+READ_POINT_NUM*8,adjAA,adjPa,adjQa); 	
	CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM,m_pCoefAdjustBuf+READ_POINT_NUM*9,adjBA,adjPb,adjQb);
	CaluMSPQ(m_pCoefAdjustBuf+READ_POINT_NUM*2,m_pCoefAdjustBuf+READ_POINT_NUM*10,adjCA,adjPc,adjQc);

  	if(m_bIsDeadValAdjust)  //若是死区值校正，取该通道最大零漂值
  	{
		if(adjUA>MidUA)
			MidUA = adjUA;
		if(adjUB>MidUB)
			MidUB = adjUB;
		if(adjUC>MidUC)
			MidUC = adjUC;
		if(adjU0h>MidU0h)
			MidU0h = adjU0h;
		if(adjUa>MidUa)
			MidUa = adjUa;
		if(adjUb>MidUb)
			MidUb = adjUb;
		if(adjUc>MidUc)
			MidUc = adjUc;
		if(adjU0l>MidU0l)
			MidU0l = adjU0l;
		if(adjIA>MidIA)
			MidIA = adjIA;
		if(adjIB>MidIB)
			MidIB = adjIB;
		if(adjIC>MidIC)
			MidIC = adjIC;		
		if(adjIa>MidIa)
			MidIa = adjIa;
		if(adjIb>MidIb)
			MidIb = adjIb;
		if(adjIc>MidIc)
			MidIc = adjIc;
		if(adjI0>MidI0)
			MidI0 = adjI0;
		if(adjIjx>MidIjx)
			MidIjx = adjIjx;		
	}
	else
	{		
		MidUAp= AiBubbleSort(adjUAp.Mod,ArrayadjUAp);//读入新值,重新排序,返回中值
		MidUBp= AiBubbleSort(adjUBp.Mod,ArrayadjUBp);
		MidUCp= AiBubbleSort(adjUCp.Mod,ArrayadjUCp);
		MidU0hp= AiBubbleSort(adjU0hp.Mod,ArrayadjU0hp);
		MidUap= AiBubbleSort(adjUap.Mod,ArrayadjUap);
		MidUbp= AiBubbleSort(adjUbp.Mod,ArrayadjUbp);
		MidUcp= AiBubbleSort(adjUcp.Mod,ArrayadjUcp);
		MidU0lp= AiBubbleSort(adjU0lp.Mod,ArrayadjU0lp);
		MidIAp= AiBubbleSort(adjIAp.Mod,ArrayadjIAp);
		MidIBp= AiBubbleSort(adjIBp.Mod,ArrayadjIBp);
		MidICp= AiBubbleSort(adjICp.Mod,ArrayadjICp);
		MidIap= AiBubbleSort(adjIap.Mod,ArrayadjIap);
		MidIbp= AiBubbleSort(adjIbp.Mod,ArrayadjIbp);
		MidIcp= AiBubbleSort(adjIcp.Mod,ArrayadjIcp);
		MidI0p= AiBubbleSort(adjI0p.Mod,ArrayadjI0p);
		MidIjxp= AiBubbleSort(adjIjxp.Mod,ArrayadjIjxp);


		MidUA= AiBubbleSort(adjUA,ArrayadjUA);
		MidUB= AiBubbleSort(adjUB,ArrayadjUB);
		MidUC= AiBubbleSort(adjUC,ArrayadjUC);
		MidU0h= AiBubbleSort(adjU0h,ArrayadjU0h);
		MidUa= AiBubbleSort(adjUa,ArrayadjUa);
		MidUb= AiBubbleSort(adjUb,ArrayadjUb);
		MidUc= AiBubbleSort(adjUc,ArrayadjUc);
		MidU0l= AiBubbleSort(adjU0l,ArrayadjU0l);
		MidIA= AiBubbleSort(adjIA,ArrayadjIA);
		MidIB= AiBubbleSort(adjIB,ArrayadjIB);
		MidIC= AiBubbleSort(adjIC,ArrayadjIC);
		MidIa= AiBubbleSort(adjIa,ArrayadjIa);
		MidIb= AiBubbleSort(adjIb,ArrayadjIb);
		MidIc= AiBubbleSort(adjIc,ArrayadjIc);
		MidI0= AiBubbleSort(adjI0,ArrayadjI0);
		MidIjx= AiBubbleSort(adjIjx,ArrayadjIjx);
	
	}

	
		
	if(wWaitTimeNum < 2000/AI_TIMER_PERIOD) //系数校正，源校验正确后等待两秒钟
	{
		wWaitTimeNum++;
		return;
	}

	if(m_bIsDeadValAdjust)
	{
		
		m_pAdjustData[32] = MidUA;
		m_pAdjustData[33] = MidUB;
		m_pAdjustData[34] = MidUC;
		m_pAdjustData[35] = MidU0h;
		m_pAdjustData[36] = MidUa;
		m_pAdjustData[37] = MidUb;
		m_pAdjustData[38] = MidUc;
		m_pAdjustData[39] = MidU0l;
		m_pAdjustData[40] = MidIA;
		m_pAdjustData[41] = MidIB;
		m_pAdjustData[42] = MidIC;
		m_pAdjustData[43] = MidIa;
		m_pAdjustData[44] = MidIb;
		m_pAdjustData[45] = MidIc;
		m_pAdjustData[46] = MidI0;
		m_pAdjustData[47] = MidIjx;
	}
	else
	{
		m_pAdjustData[0] += MidUAp;
		m_pAdjustData[1] += MidUBp;
		m_pAdjustData[2] += MidUCp;
		m_pAdjustData[3] += MidU0hp;
		m_pAdjustData[4] += MidUap;
		m_pAdjustData[5] += MidUbp;
		m_pAdjustData[6] += MidUcp;
		m_pAdjustData[7] += MidU0lp;
		m_pAdjustData[8] += MidIAp;
		m_pAdjustData[9] += MidIBp;
		m_pAdjustData[10] += MidICp;
		m_pAdjustData[11] += MidIap;
		m_pAdjustData[12] += MidIbp;
		m_pAdjustData[13] += MidIcp;
		m_pAdjustData[14] += MidI0p;
		m_pAdjustData[15] += MidIjxp; 		
		m_pAdjustData[16] += MidUA;
		m_pAdjustData[17] += MidUB; 
		m_pAdjustData[18] += MidUC;
		m_pAdjustData[19] += MidU0h; 
		m_pAdjustData[20] += MidUa; 
		m_pAdjustData[21] += MidUb; 
		m_pAdjustData[22] += MidUc; 
		m_pAdjustData[23] += MidU0l; 
		m_pAdjustData[24] += MidIA; 
		m_pAdjustData[25] += MidIB; 
		m_pAdjustData[26] += MidIC; 
		m_pAdjustData[27] += MidIa; 
		m_pAdjustData[28] += MidIb; 
		m_pAdjustData[29] += MidIc; 
		m_pAdjustData[30] += MidI0; 
		m_pAdjustData[31] += MidIjx; 
		m_pAdjustData[48] += adjPa; 
		m_pAdjustData[49] += adjPb; 		
		m_pAdjustData[50] += adjPc; 
		
	}
	m_dwAdjustCounter++;
	
}
void CAIProc::CoefAdjustEnd(BOOL result)
{
	//重新初始化累加数据数组，以用于下一次系数校正
	for(LONG i=0;i<51;i++) 
		m_pAdjustData[i]=0;
	
	//角差校正标志清零
	haveAdjustElecSource = FALSE;

	//系数校正计算次数计数器清零
	m_dwAdjustCounter=0;

	//系数校正等待时间清零
	wWaitTimeNum = 0;

	m_bIsCoefAdjust = FALSE;
	
	//向液晶任务发送消息，传递校正结果
	if(m_bIsDeadValAdjust == TRUE)
		OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, ZERODRIFT, 0, 0);
	else
		OS_PostMessage(dwMsgID, SM_AUTOADJUSTECHO, result, FULLVALUE, 0, 0);
	
}

LONG CAIProc::CoefAdjustSort(LONG Val,LONG* OriginalData)
{	
	for(LONG i=0;i<NUMOFYCVALUE-1;i++)
		OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];

	OriginalData[0] = Val;
	
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{	
		m_pBubbleData[i]=OriginalData[i];
	}
	BubbleSort(m_pBubbleData,NUMOFYCVALUE);	

	return m_pBubbleData[NUMOFYCVALUE/2];
	
}
LONG CAIProc::AiBubbleSort(LONG Val,LONG* OriginalData)
{	
	for(LONG i=0;i<NUMOFYCVALUE-1;i++)
		OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];

	OriginalData[0] = Val;
	
	for(LONG i=0;i<NUMOFYCVALUE;i++)
	{	
		m_pBubbleData[i]=OriginalData[i];
	}
	BubbleSort(m_pBubbleData,NUMOFYCVALUE);	

	return m_pBubbleData[NUMOFYCVALUE/2];
}
//void CAIProc::RefreshCommRW()
//{	
//	WORD sernum,cannum,ethnum;	
//	sernum = DB_GetSerialNum();
//	cannum = DB_GetCANNum();
//	ethnum = DB_GetEthLinkNum();
//	TSerialPara tSerial;
//	TCanBusPara tCan;
//	TEthPortPara tEth;
//	BYTE commuStatus;
//	if(ethnum > 0)
//	{
//		for(WORD i=0;i<ethnum;i++)
//		{			
//			DB_GetETHPara(i,&tEth);
//			WriteCommRW(i,tEth.wPortID,RW_ET11ERR,SS_ETHA_COMM_ERR,commuStatus);
//		}
//	}
//	else
//	{
//		for(WORD i=0;i<10;i++)
//			WriteRelayWord(RW_ET11ERR+i,RELAY_ON);
//	}
//	if(sernum > 0)
//	{
//		for(WORD i=0;i<sernum;i++)
//		{
//			DB_GetSerialPara(i, &tSerial);		
//			WriteCommRW(i,tSerial.wPortID,RW_COM1ERR,SS_SER_COMM_ERR,commuStatus);
//		}
//	}
//	else
//		WriteRelayWord(RW_COM1ERR,RELAY_ON);
//	if(cannum > 0)
//	{
//		for(WORD i=0;i<cannum;i++)
//		{
//			DB_GetCANPara(i, &tCan);
//			WriteCommRW(i,tCan.wPortID,RW_CAN1ERR,SS_CAN_COMM_ERR,commuStatus);
//		}
//	}
//	else
//	{
//		for(WORD i=0;i<2;i++)
//			WriteRelayWord(RW_CAN1ERR+i,RELAY_ON);
//	}
//}

//void CAIProc::WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag,BYTE commuStatus)
//{	
//	if(DAE_GetCommStatus(wPortID,commuStatus)!=DB_ERR_OK)	
//		WriteRelayWord(RelayWord+i,RELAY_ON);
//	else
//	{
//		if((DAE_GetRunningFlags(RunningFlag+i))||(commuStatus == COMM_STATUS_BREAK))
//			WriteRelayWord(RelayWord+i,RELAY_ON);
//		else
//			WriteRelayWord(RelayWord+i,RELAY_OFF);
//	}
//}




































