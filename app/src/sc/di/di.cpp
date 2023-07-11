/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			di.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           DI�ɼ�����                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      DI�ɼ���������ң�ź͵�ȴ���                          
*      ���ڿ�������ң�ź��ڲ�ң�ţ����ҵ���ʵң�ŵ�·����һ����8�ı����������Ҫͳһ���ǡ�                                                                                  
*      ����pDIBitDataң��״̬��������������ң�����������䣬�����洢��Ч��Ϣ��                                                                                  
*      pDIPortData�˿�״̬����������ʵң�š���ң�ź��ڲ�ң�ţ�ÿ����Ϣ����8�ı���λ���棬                                                                                  
*      ��ͬ�����м�����в���λ��Ч�����ڶ��ң�Ű���������ÿ����34·�����ռ��5���ֽڵ�                                                                                  
*      �洢������������ɨ���λ�Ƚ�ʱ���㡣�������д���ݿ�ʱ�����޳���Чλ��                                                                            
*      ����ң������˳��Ϊ:ʵ���ڲ�����                                                                            
*                                                                                  
* ����                                                                             
*                                                                                  
*      	DITask				DI������ں���
*		Init				DI��ʼ��
*		DIPortInit			DI�����ֽڶ˿ڵĲ�����ʼ��
*		DIBitInit			DI����λ�Ĳ�����ʼ��
*		BIStatusInit		DI��ʼ״̬ȷ��
*		OnDIChange			DI��λ��Ϣ��Ӧ
*		OnTimeOut			DI��ʱ��Ϣ��Ӧ
*		GetDIIndex			����ָ���ֽڶ˿��������Ӧ���ܵ�DI���
*		ErrorProc			DI�ɼ������쳣����
*		DITimerInt			DI�ɼ���ʱ�жϴ���
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/8/14    ��ʼ����                         
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
extern DWORD dwRelayMSCnt;		//�����ú��������
BEGIN_MESSAGE_MAP(CDIProc,CApp)
	ON_SM_TIMEOUT(CDIProc)
	ON_SM_DICHANGE(CDIProc)
	ON_SM_BIPARACHANGE(CDIProc)
END_MESSAGE_MAP()

//DIλ״̬��Ϣ
TDIBitData *pDIBitData = NULL;

//DI�˿�״̬��Ϣ
TDIPortData *pDIPortData = NULL; 

BOOL bDIInitFlag = FALSE;

//DI�仯������
TDIBuf *pDIBuffer = NULL;
WORD wDIReadPtr;
WORD wDIWritePtr;

DWORD dwDIIntCnt;		//DI��ʱ���жϼ���
WORD wMSecondCnt = 0;		//�������

TDIType DIType;		//��������DI�Ļ�����Ϣ

DWORD dwTaskDIId;
DWORD dwInDIBitFlag;	//�ڲ�ң��λ��־��Ϣ,����Ӧλ��λ��˵����Ӧλ���ڲ�ң����Ч


DWORD dwDIBaseAdd[]=
{
	BIA_CS_BASE,
	BIB_CS_BASE	
};
//STATIC DWORD dwSOECnt, dwDITimeCnt;

extern DWORD 	dwRelayMSCnt;		//�����ú��������
extern BOOL isVirDIValide;


STATUS ReadAllInnerDI(BYTE *pBuf);
STATUS ReadAllVirDI(BYTE *pBuf);

/*********************************************************************************************
 *
 * ��������     DI������ں���
 *
 * ����˵��      - pdwApp	: ���� 	������ں���
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
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
 * ��������     DIProc���ʼ������ҪΪ�йر�����ָ���ʼ��
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
CDIProc::CDIProc(DWORD *pdwApp):CApp(pdwApp)
{
	TDIPara *pPar = NULL;
	WORD number;

	//�����������ң��λ�ĸ���
	number = DB_GetDINum();
	pPar = DB_GetDIParaHead();
	ASSERT(pPar != NULL);
	if(pPar == NULL)
	{
		LogError("CDIProc",FILE_LINE,"get DI par error.");
		return;
	}

	//����ͳ��ң�Ÿ���
	for(int i=0; i<number; i++, pPar++)
	{
		switch(pPar->wClass&DI_ATTR_TYPEMASK)
		{
			case DI_TYPE_VIR:	//��ң��
				DIType.wDIBitNum[VIR_DI]++;
				break;
			case DI_TYPE_INNER:	//�ڲ�ң��
				DIType.wDIBitNum[INNER_DI]++;
				break;
			case DI_TYPE_INVALID:
				break;
			default:	//����ʵң�š��ڲ�ң��֮�����ЧDI�������ڲ�ң��
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

	//ң��λ����Ϣ�ṹ����ʵ����ң��λ��������
	pDIBitData = new TDIBitData[number];
	ASSERT(pDIBitData != NULL);
	if(pDIBitData == NULL)
	{
		LogError("CDIProc",FILE_LINE,"pDIBitData invalidate");
		ErrorProc();
	}
	memset((void*)pDIBitData, 0, sizeof(TDIBitData)*number);

	//��ʼ���ֽڶ˿���Ϣ�ṹ
	// 1��ң�Ű�(34·)��ռ5���ֽڣ�2����ռ10��

	//����ʵң�Ŷ˿ڻ�ַ
	DIType.wDIPortBase[REAL_DI] = 0;

	//�����ڲ�ң�Ŷ˿ڻ�ַ
	DIType.wDIPortBase[INNER_DI]= ((DIType.wDIBitNum[REAL_DI]+MAX_DI_NUM_PER_BOARD-1)/\
			MAX_DI_NUM_PER_BOARD)*((MAX_DI_NUM_PER_BOARD+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH);
	
	//������ң�Ŷ˿ڻ�ַ
	DIType.wDIPortBase[VIR_DI]= (DIType.wDIBitNum[INNER_DI]+DI_BOARD_WIDTH-1)/DI_BOARD_WIDTH+\
								DIType.wDIPortBase[INNER_DI];

	//��������ң��ռ�ö˿ڸ���
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

	//SOE��ʱ�����ʼ��
	pSoeBuf = new TEventBI[SOE_BUF_LEN];
	memset((void *)pSoeBuf, 0, sizeof(TEventBI)*SOE_BUF_LEN);
	wSoeReadPtr = wSoeWritePtr;

	//DI�仯��¼��������ʼ��
	pDIBuffer = new  TDIBuf[DI_BUF_LEN];
	memset((void *)pDIBuffer, 0, sizeof(TDIBuf)*DI_BUF_LEN);
	wDIReadPtr = wDIWritePtr = 0;

	//ȡDI����ID
	dwTaskDIId = GetTaskId();

	//��ʼ���ڲ�ң����Чλ��־
	dwInDIBitFlag = 0;
	
}

/*********************************************************************************************
 *
 * ��������     DI��ʼ�������ݲ�����ɻ���DI�ֽڶ˿ڵĲ����ͻ���DIλ�Ĳ�����ʼ����ȷ��DI��
 *				ʼ״̬����������ʱ��
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
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
	
	//����DI��ʱ��
	SetTimer(1,DI_TIMER_PERIOD);

	//DI��ʼ����ɣ�DI������ʱ�жϿ��Խ���DI��������
	bDIInitFlag = TRUE;
}

/*********************************************************************************************
 *
 * ��������     ����������ң������Ӧ���ֽڶ˿ڵĲ�����ʼ��
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CDIProc::DIPortInit()
{
	int i,j;
	BYTE boardnum, portnum, portbase;

	//ʵң�Ŵ���
	//����ʵң��·������ң�Ű����
	boardnum = ((DIType.wDIBitNum[REAL_DI]+MAX_DI_NUM_PER_BOARD-1)/MAX_DI_NUM_PER_BOARD);
	//ÿ����ң����ռ�õĶ˿ڸ���
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
			
			//ע������ÿ�����DI������һ����8����������������һ����Ҫ��������ʵ�ʸ���
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

	//�ڲ�ң��
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
	
	//��ң��
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
 * ��������     DI����λ�Ĳ�����ʼ��
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
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

	pDIPar = DB_GetDIParaHead();	//ȡDI����ͷ
	pBIPar = DB_GetBIParaHead();	//ȡBI����ͷ
	pCIPar = DB_GetCIParaHead();	//ȡCI����ͷ
	pCIPar += INTERGRAL_KWH_COUNT;  //ȡ�������ȵ�ƫ��
	number = DB_GetDINum();		//ȡDI����
	
	dwInDIBitFlag = 0;  //�ڲ�ң�ű�־λ������0
	//DIλ������ʼ��
	for(int i=0; i<number; i++, pDIPar++)
	{
		//��ʼ��ȥ��ʱ��
		pDIBitData[i].dwSmoothTime = pDIPar->wFilterTime*2;	//ȥ��ʱ��
		if(pDIBitData[i].dwSmoothTime == 0)
			pDIBitData[i].dwSmoothTime = DEFAULT_BI_SMOOTH_TIME;
		
//		pDIBitData[i].wIndex = pDIPar->wMap;		//�������

		//ȡDI����
		type = pDIPar->wClass&DI_ATTR_TYPEMASK;
		
		//����ң��������Ҫ��ʼ���Ƿ�ȡ��
		if( (type == DI_TYPE_SP) || (type == DI_TYPE_DP) )
		{
			pDIBitData[i].bAttribute &= ~DI_IS_CI;

			//����ң��λ����������Ƿ�ȡ��
//			if(pBIPar[pDIPar->wMap].wControl&BI_ATTR_CONVERT)
//				pDIBitData[i].bAttribute |= DI_IS_REVERSE;
		}
		else if(type == DI_TYPE_PLUS)//����Ƿ�Ϊ������
			pDIBitData[i].bAttribute |= DI_IS_CI;

		else if(type == DI_TYPE_INNER)//�����Ч���ڲ�ң��λ
		{
			
			offset = pDIPar->bInDINo%MAX_IN_DI_NUM_PER_BOARD;	//�����ڲ�ң���ڰ��ڵ����
			board = pDIPar->bInDIBoardNo%MAX_BO_BOARD_NUM;	//���������İ��

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

	//����ң�Ų�����ʼ��DI��BI��ӳ��
	number = DB_GetBINum();		//ȡBI����
	for(int i=0; i<number; i++, pBIPar++)
	{
		pDIBitData[pBIPar->wMaptoDINo].wIndex = i;//ȷ��DI��BI��ӳ���ϵ
		
		//����Ƿ���Ҫң��ȡ��
		if(pBIPar->wControl&BI_ATTR_CONVERT)
			pDIBitData[pBIPar->wMaptoDINo].bAttribute |= DI_IS_REVERSE;
		else
			pDIBitData[pBIPar->wMaptoDINo].bAttribute &= (~DI_IS_REVERSE);
			
		//����Ƿ���Ҫ��Ҫ����SOE
		if(pBIPar->wControl&BI_ATTR_WTPEVENT)
			pDIBitData[pBIPar->wMaptoDINo].bAttribute |= DI_CREAT_SOE;
		else
			pDIBitData[pBIPar->wMaptoDINo].bAttribute &= (~DI_CREAT_SOE);
	}

	//���ݵ�Ȳ�������DI��CI��ӳ��
	number = DB_GetPulseCINum();		//ȡCI����
	for(int i=0; i<number; i++, pCIPar++)
	{
		pDIBitData[pCIPar->wMaptoDINo].wIndex = i;//ȷ��DI��CI��ӳ���ϵ
	}
	
	//���������һ·�ڲ�ң�ż��źŸ���
	dwInDIBitFlag <<= 1;
	dwInDIBitFlag |= 0x01;
}

/*********************************************************************************************
 *
 * ��������     DI��ʼ״̬ȷ��
 *
 * ����˵��      - arg	: ���� 	DI����
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
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
	
	//����DI��ͬ����ȷ�����ԵĶ˿ڷ�Χ�������ڲ�ң�ź���ң�ţ�Ԥ�ȶ�ȡȫ��״̬��Ϣ
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
		//ע�ⲻͬ���͵�DI����������Դ��ͬ
		if(type == REAL_DI)
		{
			//����ʵң�ţ���Ҫ���ң�Ű��Ƿ���ڣ���������ң��ˢ��Ϊ��λ��
			//��ң�Ű�ʶ����,ֻ�и�6λ��Ч
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

		//����ÿ���˿�ʵ�ʵ�DIλ����������
		for(j=0; j<pDIPortData[i].bBiNumInPort; j++)
		{	
			//ң�ų�ʼ״̬����
			if((pDIBitData[DIIndex].bAttribute & DI_IS_CI) == 0)
			{
				if(bBIStatus&0x01)	//��ʼ״̬Ϊ��λ
				{
					if(pDIBitData[DIIndex].bAttribute & DI_IS_REVERSE)
						BIData.bStatus = BI_OPEN;
					else
						BIData.bStatus = BI_CLOSE;

					//��ʼ��DI�����ʼ״̬
	                pDIBitData[DIIndex].bStatusTab = DI_STEP_6;
				}
				else		//��ʼ״̬Ϊ��λ
				{
					if(pDIBitData[DIIndex].bAttribute & DI_IS_REVERSE)
						BIData.bStatus = BI_CLOSE;
					else
						BIData.bStatus = BI_OPEN;

					//��ʼ��DI�����ʼ״̬
	                pDIBitData[DIIndex].bStatusTab = DI_STEP_0;
				}
				//ң�ų�ʼ״̬д���ݿ�
				DB_WriteBI(pDIBitData[DIIndex].wIndex, BIData.bStatus);
			}
			else	//��ȳ�ʼ״̬����
			{
				//��ʼ��DI�����ʼ״̬
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
 * ��������     DI��λ��Ϣ��Ӧ
 *
 * ����˵��      - id	: ���� 	����
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
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
		//ȡ��λDI�Ķ˿����
		bPortNo = pDIBuffer[wDIReadPtr].bDIPortNo;

		//ȡ��λDI��λ����Ϣ
		bDIChangeBit = pDIBuffer[wDIReadPtr].bDIChgBit;
		
		//�����λDI���ܵ�λ���
		wDIBitNo = GetDIIndex(bPortNo);

		//��鲢�����λDI�ֽ���Ϣ�о��巢����λ��λ��
		for(int i=0; i<pDIPortData[bPortNo].bBiNumInPort; i++)
		{
			pBitData = &pDIBitData[wDIBitNo];
			if(bDIChangeBit & 0x01)	//�б�λ
			{
				//DI��λ״̬����
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
							if(bAttribute&DI_IS_CI) //��ȴ���
							{
								//ˢ��������
								DB_WriteCI( pBitData->wIndex+INTERGRAL_KWH_COUNT, 1);
							}
							else	//ң�Ŵ���
							{
								if(bAttribute&DI_IS_REVERSE)//ң��ȡ��
									BurstBI.bStatus = BI_OPEN;
								else
									BurstBI.bStatus = BI_CLOSE;
								BurstBI.wNo = pBitData->wIndex;
								
								//ˢ�����ݿ�ң��״̬
								DB_WriteBI(BurstBI.wNo, BurstBI.bStatus);

								DB_GetBIPara(BurstBI.wNo, &BIPara);
								DB_GetDIPara(BIPara.wMaptoDINo, &DIPara);
								DIPara.wClass &= DI_ATTR_TYPEMASK;
								if((DIPara.wClass != DI_TYPE_VIR)||((DIPara.wClass == DI_TYPE_VIR)&&(isVirDIValide == TRUE)))
								{
										//ˢ��ң�ű�λ
										DB_WriteBIE(&BurstBI);
										
										//SOE����
										//ע�⣬SOE�ĺ���ʱ�䴦����Ҫ��ϸ����
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
							if((bAttribute&DI_IS_CI)==0) //ң�Ŵ���
							{
								if(bAttribute&DI_IS_REVERSE)//ң��ȡ��
									BurstBI.bStatus = BI_CLOSE;
								else
									BurstBI.bStatus = BI_OPEN;
									
								BurstBI.wNo = pBitData->wIndex;
													
								//ˢ�����ݿ�ң��״̬
								DB_WriteBI(BurstBI.wNo, BurstBI.bStatus);
														
								DB_GetBIPara(BurstBI.wNo, &BIPara);
								DB_GetDIPara(BIPara.wMaptoDINo, &DIPara);
								DIPara.wClass &= DI_ATTR_TYPEMASK;
								if((DIPara.wClass != DI_TYPE_VIR)||((DIPara.wClass == DI_TYPE_VIR)&&(isVirDIValide == TRUE)))
								{
										//ˢ��ң�ű�λ
										DB_WriteBIE(&BurstBI);
																
										//SOE����
										//ע�⣬SOE�ĺ���ʱ�䴦����Ҫ��ϸ����
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
 * ��������     DI��ʱ��Ϣ��Ӧ
 *
 * ����˵��      - id	: ���� 	����
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
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
	//��������ֽڶ˿�
	for(i=0; i<DIType.wDIPortNum; i++)
	{
		//���ÿ���ֽڵ�ַ�е���ЧDIλ����Ч������
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
						if(bAttribute&DI_IS_CI)	//��ȴ���
						{
							if(status == DI_STEP_3)
							{
								//ˢ��������
								DB_WriteCI( pBitData->wIndex+INTERGRAL_KWH_COUNT, 1);
							}
						}
						else	//ң�Ŵ���
						{
							if(status == DI_STEP_3)
							{
								if(bAttribute&DI_IS_REVERSE)//ң��ȡ��
									BurstBI.bStatus = BI_OPEN;
								else
									BurstBI.bStatus = BI_CLOSE;
							}
							else if(status == DI_STEP_9)
							{
								if(bAttribute&DI_IS_REVERSE)//ң��ȡ��
									BurstBI.bStatus = BI_CLOSE;
								else
									BurstBI.bStatus = BI_OPEN;
							}
							BurstBI.wNo = pBitData->wIndex;
							
							//ˢ�����ݿ�ң��״̬
							DB_WriteBI(BurstBI.wNo, BurstBI.bStatus);

							DB_GetBIPara(BurstBI.wNo, &BIPara);
							DB_GetDIPara(BIPara.wMaptoDINo, &DIPara);
							DIPara.wClass &= DI_ATTR_TYPEMASK;
							if((DIPara.wClass != DI_TYPE_VIR)||((DIPara.wClass == DI_TYPE_VIR)&&(isVirDIValide == TRUE)))
							{
									//ˢ��ң�ű�λ
									DB_WriteBIE(&BurstBI);
									
									//SOE����
									//ע�⣬SOE�ĺ���ʱ�䴦����Ҫ��ϸ����
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

	//����ѩ��
//	if((dwDITimeCnt%200)==0)
//		ReportMsg("SOE number=%d.", dwSOECnt);
}

/*********************************************************************************************
 *
 * ��������     ����ָ���ֽڶ˿��������Ӧ���ܵ�DI���
 *
 * ����˵��      - bPortNo	: ���� 	DI�ֽڶ˿����
 *				 
 *
 * ���ش���
 *                DI���
 *
 * ����˵����          
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
 * ��������     DI�ɼ������쳣����
 *
 * ����˵��      ��
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CDIProc::ErrorProc()
{
	OS_Suspend(0);
}

/*********************************************************************************************
 *
 * ��������     DI�ɼ���ʱ�жϴ���
 *
 * ����˵��      - arg	: ���� 	����
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void DITimerInt()  //0.5ms�ж�һ��
{
	BYTE bDIStatus, bChgBit;
	STATUS status = 0;
	static DWORD dwCnt, dwtemp;
	
	//����ʱ����㣬��������ʱ����B��ʽ��ȡ������ʱ�䵥����������λ������������B��ʽ����
	//�жϴ���
	if(wMSecondCnt<1999)
	{
		wMSecondCnt++;
	}
	//���DI�����ʼ���Ƿ����
	if(bDIInitFlag == FALSE)
		return;

	dwCnt++;
	dwDIIntCnt = dwCnt;
	
	//ɨ������DI�˿ڵ�DI״̬
	status = ScanDIPort(REAL_DI)|ScanDIPort(INNER_DI)|ScanDIPort(VIR_DI);

	//��DI״̬�����仯��֪ͨDI������д���
	if(status)
	{
		dwtemp++;
//		printf("DI task postmsg %d.\n", dwtemp);
		OS_PostMessage(dwTaskDIId, SM_DICHANGE, 0, 0, 0, 0);
	}

}

/*********************************************************************************************
 *
 * ��������     DI�˿�״̬ɨ�账��
 *
 * ����˵��      - type	: ���� 	DI����
 *				 
 *
 * ���ش���
 *                �Ƿ���״̬�仯
 *
 * ����˵����          
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
	
	//����DI��ͬ����ȷ�����ԵĶ˿ڷ�Χ�������ڲ�ң�ź���ң�ţ�Ԥ�ȶ�ȡȫ��״̬��Ϣ
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
	//�������Ͳ�ͬ���ֱ�ɨ��DI�˿ڵ�DI״̬
	for(i=start, j=0; i<end; i++, j++)
	{
		//ע��������Դ��ͬ
		if(type == REAL_DI)
		{
			//����ʵң�ţ���Ҫ���ң�Ű��Ƿ���ڣ���������ң��ˢ��Ϊ��λ��
			//��ң�Ű�ʶ����,ֻ�и�6λ��Ч
			address = (pDIPortData[i].dwPortAddr&0xfffffff0)+DI_BOARD_ID_ADD;
			
			//��һ�����õĵ�ַд0xFF����������ˢ�ɸߵ�ƽ����ֹ��Ϊ����ң�Ű��ʶ��ID������ȡ��������⡣
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

		//����Ƿ��б�λ
		bChgBit = bDIStatus ^ pDIPortData[i].bOldStatus;
		if(bChgBit)
		{
			pDIBuffer[wDIWritePtr].bDIStatus = bDIStatus;	//��¼��λʱ״̬	
			pDIBuffer[wDIWritePtr].bDIPortNo = i;			//��¼��λ�ֽڶ˿����
			ReadAbsTime(&pDIBuffer[wDIWritePtr].Time);		//��¼��λʱϵͳʱ��
			pDIBuffer[wDIWritePtr].dwDITimeCnt = dwDIIntCnt;//��¼��λʱDI�жϼ���	
			pDIBuffer[wDIWritePtr].bDIChgBit = bChgBit;		//��¼��λλ�ľ���λ��
			pDIPortData[i].bOldStatus = bDIStatus;
			wDIWritePtr++;
			wDIWritePtr %= DI_BUF_LEN;
			status = 1;
		}
	}
	return status;
}
	
//��ʱ�����ȡȫ���ڲ�ң�Žӿں���
STATUS ReadAllInnerDI(BYTE *pBuf)
{

	DWORD AllStatus, data, ValidStatus;
	int i, j;
	BYTE status;
	
	DWORD dwBase[] = {DOA_CS_BASE+28, DOC_CS_BASE+28};

	//��ȡ���е��ڲ�ң��״̬,ע������Ӳ����Դ���ƣ����24���ڲ�ң�ţ�����3��ң�ذ壬ֻ��ǰ������24���ڲ�ң����Ч
	AllStatus = 0;	
	for(i=0; i<2; i++)
	{
		data = (~((DWORD)MAKEWORD(inportb(dwBase[i]), inportb(dwBase[i]+1))));
		data &= 0x0fff;
		
//		if(data)
//			printf("\n the board %d's inner yx status = 0x%x.", i, data);
		AllStatus |= (data<<(i*MAX_IN_DI_NUM_PER_BOARD));
	}

	

	//�������λ���źŸ���ң��
	AllStatus <<= 1;
	status = (inportb(KEY2_CS_BASE)&0x01)^0x01;
	AllStatus |= status;//(inportb(KEY2_CS_BASE)&0x01);	//�Ӽ��̶˿��϶�ȡ�źŸ����ֵ

	//������Чλ,�����д洢
	ValidStatus = 0;
	j = 0;
	for(i=0; i<24; i++)	//���ֻ��24λ��Դ
	{
		if(dwInDIBitFlag&(0x01<<i))
		{
			ValidStatus |= (((AllStatus>>i)&0x01)<<j);
			j++;
		}
	}
	if(ValidStatus != 0)
		status = 1;
	//����Чλ���浽������
	for(j=0; j<((DIType.wDIBitNum[INNER_DI]+7)/8); j++)
	{
		pBuf[j] = (BYTE)ValidStatus;
		ValidStatus >>= 8;
	}
	
	return ERR_SUCCESS;
}

//��ʱ�����ȡȫ����ң�Žӿں���
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

