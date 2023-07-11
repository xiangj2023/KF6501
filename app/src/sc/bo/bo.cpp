/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			bo.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ң�ش���                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      	��ң�ش���ֻ����ң�����̵Ĵ������������ң�ؼ̵������˴���һ·ң��ָһ����·ң��
*		������һ��һ�ϣ�һ��һ�ϱ�ʾ��·ң�ء�������Ҫ�������·ң�صĲ���ִ�У�����ͬһ·                                                                                        
*       ң�أ�����ң�ؽ���ң��״̬(��ң��Ԥ�õ�ң��ִ�з���)��������Դ�·ң�صĲ�����                                                                           
*		ң��Ԥ�ú���Ҫ��ʱһ��ʱ��ȴ�Ԥ�ü̵����Ķ�����
*                                                                                  
* ����                                                                             
*                                                                                  
*      	BOTask				bo������ں���
*		Init				BO��ʼ��
*		OnTimeOut			BO��ʱ��Ϣ����
*		OnBOSelect			ң��Ԥ����Ϣ��Ӧ
*		OnBOExec			ң��ִ����Ϣ��Ӧ
*		OnBODirect			ֱ��ң����Ϣ��Ӧ
*
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/8/20    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "bo.h"
#include "m5234timer.h"
#include "logic.h"
extern DWORD dwRelayMSCnt;		//�����ú��������

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


//����ģ������ֱ�ӳ�ʼ��BO����,�����Ҫ���ò�����ʽ�Ļ�����Ҫ������Ӧ�Ĵ������
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

WORD wRealBONum = 0;	//װ��ʵң�Ÿ���,��Ҫ���ݱ��������ж�һ������ң�ذ�
DWORD dwBOStatus[MAX_BO_BOARD_NUM][2];//���ڲ���״̬λ��Ϣ

/*
//ң�ز���״̬��־��ÿһλ��Ӧһ·ң�أ���λ˵����·ң�����ڲ�����ֻ��dwBoCtlSts�е�32λ
//��Ϊ��ʱ���ܴ򿪸ð�ı����̵���
DWORD dwBoCtlSts[MAX_BO_BOARD_NUM];
*/

/*********************************************************************************************
 *
 * ��������     bo������ں���
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
void BOTask(DWORD *pdwApp)
{
	CBoProc *pBOTask = new CBoProc(pdwApp);
	pBOTask->Init();
	pBOTask->Run();
}	

/*********************************************************************************************
 *
 * ��������     CBoProc���ʼ������ҪΪ�йر�����ָ���ʼ��
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
CBoProc::CBoProc(DWORD *pdwApp):CApp(pdwApp)
{
	//����ģ�������BO·��
/*	
	bModuleBoNum = 0;
	for(int i=0; i<ModuleBoCfg.bBOBoardNum; i++)
		bModuleBoNum += ModuleBoCfg.bBONumPerBoard[i];
*/
	bModuleBoNum = DB_GetBONum();	//ע��˴���ң�ذ�����ң��
	ASSERT(bModuleBoNum <= MAX_BO_NUM);
	if(bModuleBoNum > MAX_BO_NUM)
	{
		LogError("CBoProc",FILE_LINE,"BO number invalidate");
		ErrorProc();
	}

	//��ʼ��BO���ݽṹ
	pBoInfor = new TBOInfor[bModuleBoNum];
	ASSERT(pBoInfor != NULL);
	if(pBoInfor == NULL)
	{
		LogError("CBoProc",FILE_LINE,"malloc error!");
		ErrorProc();
	}
	memset((void*)pBoInfor, 0, sizeof(TBOInfor)*bModuleBoNum);

	//��ʼ��ң�ز���״̬��ʼ��־����
//	memset((void*)&dwBoCtlSts[0], 0, sizeof(dwBoCtlSts));
}

/*********************************************************************************************
 *
 * ��������     BO��ʼ��������ÿ·ң�صĶ˿ڵ�ַ��ң����š���ʼ״̬�ȵĳ�ʼ��
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
void CBoProc::Init()
{
	int i, j;
	TBOInfor *pBo;
    TBOPara		*pAttr = NULL;

	//ȡBO����ͷ
	pAttr = DB_GetBOParaHead();
	if(pAttr == NULL)
	{
		LogError("BOinit",FILE_LINE,"GetBOPara error!");
		return;
	}
		
	//�������ݿ��������ʼ��BO��ʼ����
	for(i=0; i<bModuleBoNum; i++, pAttr++)
	{
		pBoInfor[i].bBOCtlStatus = BO_CTL_IDLE;	
		//��ʼ��ң�ر���ʱ��
		pBoInfor[i].wKeepTime = pAttr->wLastTime/BO_TASK_TIME;//BO_DEFAULT_KEEP_TIME;
		if(pBoInfor[i].wKeepTime == 0)
			pBoInfor[i].wKeepTime = BO_DEFAULT_KEEP_TIME;
		pBoInfor[i].wTimeOut = BO_TIME_OUT;	//ң�س�ʱʱ��
		pBoInfor[i].wIndex = pAttr->wSwitchNo; 		//i;		//Ĭ���ⲿң��·�����ڲ������ͬ

		//����ʵң��·��
		if( (pAttr->dwControl & BO_ATTR_VIRTUAL) == 0)
			wRealBONum++;
	}

/*
	//��ʼ��ÿ��ң�ز����Ķ˿ڵ�ַ
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

	dwBOTestCnt = BO_TEST_INVALID;//����������Ч״̬

	//��ʼ��������ʼ�˿�״̬
	for(i=0; i<MAX_BO_BOARD_NUM; i++)
		dwBOStatus[i][0] = dwBOStatus[i][1] = 0;

	//����BO��ʱ��
	SetTimer(1,BO_TASK_TIME);
		
}

/*********************************************************************************************
 *
 * ��������     BO��ʱ��Ϣ�����������ң��״̬��ˢ��
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
void CBoProc::OnTimeOut(DWORD id)
{
	int i;
	BYTE status, number;
	//�������
	if(dwBOTestCnt != BO_TEST_INVALID)	//�д���
	{
		if(dwBOTestCnt)
		{
			dwBOTestCnt--;
		}
		else	//����ʱ�䵽���ջس���
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
			case BO_CTL_IDLE:		//ң�ز�������״̬
				break;
			case BO_CTL_SET:		//ң�ز���Ԥ��״̬���ȴ�Ԥ�ü̵�����ʱʱ��
				if(pBoInfor[number].wStsTimeCnt > 0)	//Ԥ�ü̵�����ʱʱ�䵽�����з�У
				{
					if(RemoteCtrlVer(number) == TRUE)	//��У�ɹ�
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
						//֪ͨ���ݿ�ң��Ԥ�÷�У����,���Զ�����Ԥ��
						DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SET_CHECK);
						RemoteCtrlPre(number, FALSE);	//�Զ�����Ԥ��
						ReportMsg("BO verify Error.");
						
						pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
						pBoInfor[number].wStsTimeCnt = 0;
*/						
					}
				}
				
				else	//Ԥ�ü̵�����ʱ�ȴ�δ��
				{
					//֪ͨ���ݿ�ң��Ԥ�÷�У����,���Զ�����Ԥ��
					DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SET_CHECK);
					RemoteCtrlPre(number, FALSE);	//�Զ�����Ԥ��
					ReportMsg("BO verify Error.");
					
					pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
					pBoInfor[number].wStsTimeCnt = 0;
				}
				
				break;
			case BO_CTL_VER:	//ң�ط�У״̬�����ң�س�ʱ���
				//ң�صȴ���ʱ������ң��ʧ�ܣ��Զ�����Ԥ��
				if(pBoInfor[number].wStsTimeCnt == 0)	
				{
					//֪ͨ���ݿ�ң�س�ʱ���Զ�����
					DB_BOSelectEcho(i, BO_CLOSE, ERR_BO_SET_TIMEOUT);
					RemoteCtrlPre(number, FALSE);	//�Զ�����Ԥ��
					ReportMsg("BO control timeout Error.");
					
					pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
					pBoInfor[number].wStsTimeCnt = 0;
					
				}
				else
				{
					pBoInfor[number].wStsTimeCnt--;
				}
				break;
			case BO_CTL_EXECUTE:		//��ͨң��ִ��
			case BO_CTL_DIR_EXECUTE:	//ֱ��ң��ִ��
				//ң��ִ�б���ʱ�䵽���ջس���
				if(pBoInfor[number].wStsTimeCnt == 0)
				{
					RemoteCtrlExe(number, FALSE);
					RemoteCtrlPre(number, FALSE);
					pBoInfor[i].bBOCtlStatus = BO_CTL_IDLE;
					ReportMsg("BO execute return.dwRelayMSCnt = %d",dwRelayMSCnt);
				}
				else //��ʱ�ȴ�
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
 * ��������     ң��Ԥ����Ϣ��Ӧ������ǰң�����ڴ�����Ԥ�ò���ִ��
 *
 * ����˵��      - no	: ���� 	�ⲿң��·��
 * 			     - attr	: ���� 	ң��Ԥ������
 *				 - par	: ����	���ò���
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CBoProc::OnBOSelect(WORD no, WORD attr, WORD par)
{
	WORD number;	//ң���ڲ����
		
	number = pBoInfor[no].wIndex;
	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("OnBoSelect",FILE_LINE,"BO NO error!");
		//֪ͨ���ݿ�ң��Ԥ��ʧ��
		DB_BOSelectEcho(no, attr, ERR_BO_POINT);
		return;
	}
	
	//����·ң���Ƿ����
	if(pBoInfor[number].bBOCtlStatus != BO_CTL_IDLE)
	{
		ReportMsg("BO setting busy, and no=%d.",no);

		//֪ͨ���ݿ�ң��Ԥ��ʧ��
		DB_BOSelectEcho(no, attr, ERR_BO_BUSY);
		return;
	}

	ReportMsg("BO setting, and no=%d.",no);

	//Ԥ�ò���
	//����Ԥ��״̬
	pBoInfor[number].bBOCtlStatus = BO_CTL_SET;
	
	//ң��Ԥ�ú����Ƚ���һ�η�У
	RemoteCtrlPre(number, TRUE);
	if(RemoteCtrlVer(number) == TRUE)	//��У�ɹ�
	{	
		DB_BOSelectEcho(no, attr, ERR_BO_SUCCESS);
		pBoInfor[number].bBOCtlStatus = BO_CTL_VER;
		pBoInfor[number].wStsTimeCnt = pBoInfor[number].wTimeOut;
		ReportMsg("OnBOSelect: BO verify OK");
	}
	else
	{
		//���뷴У�ȴ�״̬(���ǵ�Ԥ�ü̵�������ʱ,����ȴ�һ��ʱ�䣬20����)	
		pBoInfor[number].wStsTimeCnt = BO_VER_TIME;
	}
	
}

/*********************************************************************************************
 *
 * ��������     ң��ִ����Ϣ��Ӧ������ִ�кͳ������ֲ���
 *
 * ����˵��      - no	: ���� 	�ⲿң��·��
 * 			     - attr	: ���� 	ң��Ԥ������
 *				 - mode	: ����	ִ�л���
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CBoProc::OnBOExec(WORD no, WORD attr, WORD mode)
{
	WORD number;	//ң���ڲ����
	BYTE status;
		
	number = pBoInfor[no].wIndex;
	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("OnBoExecute",FILE_LINE,"BO NO error!");
		//֪ͨ���ݿ�ң��ִ��ʧ��
		DB_BOExecEcho(no, attr, mode, ERR_BO_POINT);
		return;
	}

	//���״̬�Ƿ�Ϸ�
	status = pBoInfor[number].bBOCtlStatus;
	if(mode == 1)	//ң��ִ��
	{
		if(status == BO_CTL_VER)	//ֻ��ң��Ԥ�óɹ�����ܽ���ң��ִ��
		{
			//ң��ִ��
			RemoteCtrlExe(number, TRUE);
			
			ReportMsg("BO number%d execute.", number);
			//����ң��ִ�б���״̬
			pBoInfor[number].bBOCtlStatus = BO_CTL_EXECUTE;
			TBOPara BoPara;
			DB_GetBOPara(number,&BoPara);
			pBoInfor[number].wStsTimeCnt = BoPara.wLastTime/BO_TASK_TIME;
			//pBoInfor[number].wStsTimeCnt = pBoInfor[number].wKeepTime;

			//֪ͨ���ݿ�ң��ִ�гɹ�
			DB_BOExecEcho(no, attr, mode, ERR_BO_SUCCESS);
		}
		else
		{
			LogError("OnBoExecute",FILE_LINE,"BO busy!");
			//֪ͨ���ݿ�ң��ִ��ʧ��
			DB_BOExecEcho(no, attr, mode, ERR_BO_BUSY);
		}
	}
	else if(mode == 2)	//ң��Ԥ�ó���
	{
		//����ң��Ԥ�û�У״̬���ܷ��ͳ�������
		if( (status == BO_CTL_VER) || (status == BO_CTL_SET) )
		{
			//ң��Ԥ�ó���
			RemoteCtrlPre(number, FALSE);
			
			ReportMsg("BO number%d cancel.", number);
			//�������״̬
			pBoInfor[number].bBOCtlStatus = BO_CTL_IDLE;
			pBoInfor[number].wStsTimeCnt = 0;
			//֪ͨ���ݿ�ң�س���
			DB_BOExecEcho(no, attr, mode, ERR_BO_SUCCESS);
		}
		else
		{
			LogError("OnBoExecute",FILE_LINE,"BO busy!");
			//֪ͨ���ݿ�ң��ִ��ʧ��
			DB_BOExecEcho(no, attr, mode, ERR_BO_BUSY);
		}
	}
}

/*********************************************************************************************
 *
 * ��������     ֱ��ң����Ϣ��Ӧ�������·ң����æ���򱾴�ֱ��ң��ȡ��
 *
 * ����˵��      - no	: ���� 	�ⲿң��·��
 * 			     - attr	: ���� 	ң��Ԥ������
 *				 - par	: ����	���ò���		 
 *				 
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CBoProc::OnBODirect(WORD no, WORD attr, WORD par)
{
	WORD number;	//ң���ڲ����
	
	number = pBoInfor[no].wIndex;
	ASSERT(no<bModuleBoNum);
	if(no>=bModuleBoNum)
	{
		LogError("OnBoDirExecute",FILE_LINE,"BO NO error!");
		//֪ͨ���ݿ�ң��ִ��ʧ��
		DB_BODirectEcho(no, attr, ERR_BO_POINT);
		return;
	}

	//����·ң���Ƿ����
	if(pBoInfor[number].bBOCtlStatus != BO_CTL_IDLE)
	{
		//֪ͨ���ݿ�ֱ��ң��ʧ�ܣ���ǰң����æ
		DB_BODirectEcho( no, attr, ERR_BO_BUSY);
	}
	else
	{

		ReportMsg("BO number%d direct execute.dwRelayMSCnt = %d", number+1,dwRelayMSCnt);
		
		//ͨ��ң��ִ�к������ֱ��ң��
		RemoteCtrlPre(number, TRUE);
		RemoteCtrlExe(number, TRUE);
		
		//ң�ز�������ֱ��ң��Ԥ��״̬
		pBoInfor[number].bBOCtlStatus = BO_CTL_DIR_EXECUTE;

		TBOPara BoPara;
		DB_GetBOPara(number,&BoPara);
		pBoInfor[number].wStsTimeCnt = BoPara.wLastTime/BO_TASK_TIME;
		//pBoInfor[number].wStsTimeCnt = pBoInfor[number].wKeepTime;
		
		//֪ͨ���ݿ�ң��ִ�гɹ�
		DB_BODirectEcho(no, attr, ERR_BO_SUCCESS);
	}
}
/*********************************************************************************************
 *
 * ��������     �쳣����
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
void CBoProc::ErrorProc()
{
	OS_Suspend(0);
}

/*********************************************************************************************
 *
 * ��������     ����������Ϣ��Ӧ����
 *
 * ����˵��      - no	: ���� 	�ⲿ����·��
 *				 - par1	: ����	���ò���
 *				 - par2	: ����	���ò���
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void CBoProc::OnBOTest(WORD no, WORD par1, WORD par2)
{
	if(dwBOTestCnt == BO_TEST_INVALID)	//��ǰ�޴�������ִ�У����Խ��д�������
	{
		ReportMsg("BOTest:no=%d",no+1);
		dwBOTestCnt = par1/BO_TASK_TIME;
		bBOTestNo = no;
		CtrlTest(no, TRUE);
		
	}
}

/*********************************************************************************************
 *
 * ��������     ���ڿ��Ʋ���,�������ṩ���߼�����ʹ��,�����ʵң�ص�
 *
 * ����˵��      - no	: ���� 	���ڿ������,��Ӳ��ң�ص����
 *				 - isExe: ���� ��ִ�л��ǳ���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
/*
DWORD RealBOControl(WORD no, BOOL isExe)
{
	BYTE number, offset, cmd1, cmd2;
	DWORD dwBaseAdd;
	BYTE status[3];

	//������·���Ƿ���Ч
	ASSERT(no<(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD));
	if(no>=(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD))
		return ERR_FAILURE;

	number = no/MAX_BO_NUM_PER_BOARD;
	offset = no%MAX_BO_NUM_PER_BOARD;

	//ȡ��Ӧң�س��ڵĻ�ַ,ң����������(��0·���)
	if(offset)
	{
		dwBaseAdd = dwBOBaseAdd[number]+0x02*(offset/8);
		if(isExe == TRUE)	//ִ��
		{
			dwBOStatus[number][0] &= (~(0x01<<offset));
			dwBOStatus[number][1] |= (0x01<<offset);
		}
		else	//����
		{
			dwBOStatus[number][0] |= (0x01<<offset);
			dwBOStatus[number][1] &= (~(0x01<<offset));
		}

		//���ڿ���
		cmd1 = (BYTE)(dwBOStatus[number][0]>>(offset/8*8));
		cmd2 = (BYTE)(dwBOStatus[number][1]>>(offset/8*8));
		outportb(dwBaseAdd,  cmd1);
		outportb(dwBaseAdd+1, cmd2);
	}
	else	//��������ÿ��ң�ذ��ң����������
	{
		dwBaseAdd = dwBOBaseAdd[number]+6;	//����ң������·�Ķ˿ڵ�ַ
		if(isExe == TRUE)	//ִ��
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
		else	//����
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

	//������·���Ƿ���Ч
	ASSERT(no<(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD));
	if(no>=(MAX_BO_BOARD_NUM*MAX_BO_NUM_PER_BOARD))
		return ERR_FAILURE;

	number = no/MAX_BO_NUM_PER_BOARD;
	offset = no%MAX_BO_NUM_PER_BOARD;

	cmd1=0x55;  //ִ��
	cmd2=0xAA;	//����
	dwBaseAdd = dwBOBaseAdd[number]+offset;
	if(isExe == TRUE) //ң��ִ��
	{
		if(offset == 0)//�ܱ����Ŀ���
		{
			if(number == 0) //��һ���
			{
				MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
				MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=1
			}
			else			//�ڶ����
			{
				MCF_GPIO_PODR_TIMER &= (~DTI1_YKEN3);	//YK_EN3=0
				MCF_GPIO_PODR_TIMER |= DTI0_YKEN4;		//YK_EN4=1
			}
		}
		outportb(dwBaseAdd,  cmd1);		
	}
	else //ң�س���
	{
		if(offset == 0)//�ܱ����Ŀ���
		{
			if(number == 0) //��һ���
			{
				MCF_GPIO_PODR_TIMER |= DTO1_YKEN1;		//YK_EN1=1
				MCF_GPIO_PODR_TIMER &= (~DTO0_YKEN2);	//YK_EN2=0
			}
			else			//�ڶ����
			{
				MCF_GPIO_PODR_TIMER |= DTI1_YKEN3;		//YK_EN3=1
				MCF_GPIO_PODR_TIMER &= (~DTI0_YKEN4);	//YK_EN4=0
			}
		}
		outportb(dwBaseAdd,  cmd2);		
	}
/*
	//ȡ��Ӧң�س��ڵĻ�ַ,ң����������(��0·���)
	if(offset)
	{
		dwBaseAdd = dwBOBaseAdd[number]+0x02*(offset/8);
		if(isExe == TRUE)	//ִ��
		{
			dwBOStatus[number][0] &= (~(0x01<<offset));
			dwBOStatus[number][1] |= (0x01<<offset);
		}
		else	//����
		{
			dwBOStatus[number][0] |= (0x01<<offset);
			dwBOStatus[number][1] &= (~(0x01<<offset));
		}

		//���ڿ���
		cmd1 = (BYTE)(dwBOStatus[number][0]>>(offset/8*8));
		cmd2 = (BYTE)(dwBOStatus[number][1]>>(offset/8*8));
		outportb(dwBaseAdd,  cmd1);
		outportb(dwBaseAdd+1, cmd2);
	}
	else	//��������ÿ��ң�ذ��ң����������
	{
		dwBaseAdd = dwBOBaseAdd[number]+6;	//����ң������·�Ķ˿ڵ�ַ
		if(isExe == TRUE)	//ִ��
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
		else	//����
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
 * ��������     �����̵�������ÿ����һ�������̵��������ÿ������һ����Ӧÿ·ң�ز�����״̬
 *				��¼����ÿ·ң�ض���������ܴ򿪱����̵���������ֻ����һ·ң�ز�������Ҫ����
 *				�����̵������ں�λ��
 *
 * ����˵��      - cmd	: ���� 	ң������
 * 			     - no	: ���� 	�ڲ�ң�����
 *				 
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
		BJ_RELAY_CTL(BO_CLOSE);	//�ϱ����̵���

		//��ң��״̬��־λ
		OS_Protect(0);
		dwBoCtlSts[num] |= (0x01<<(no%MAX_BO_NUM_PER_BOARD));
		OS_UnProtect(0);
	}
	else if(cmd == BO_OPEN)
	{
		//��ң��״̬��־λ
		OS_Protect(0);
		dwBoCtlSts[num] &= (~(0x01<<(no%MAX_BO_NUM_PER_BOARD)));
		//ֻ�иð��ϵ�����ң�ض���������ܴ򿪸ð��ϵı����̵���
		if(dwBoCtlSts[num] == 0)
			BJ_RELAY_CTL(BO_OPEN); //�ϱ����̵���
		OS_UnProtect(0);
	}
	
	return ERR_SUCCESS;
}
*/
//��ʱ������ң�س���ӳ���
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
//��ʱ����ң��Ԥ�ú���
//wBOIndex--ң����ţ�bContext--Ԥ�û���Ԥ��
void RemoteCtrlPre(WORD wBOIndex, BOOL bContext)
{
	BYTE value = 0;

	value = (0x01<<BoMapTable[wBOIndex].bPreJOffset);

	ReportMsg("\nң��Ԥ��ǰ״̬:");
	ReadInYX();

	if(bContext == TRUE)
	{
		outportb(BoMapTable[wBOIndex].dwPreJAddr, 0);
		outportb(BoMapTable[wBOIndex].dwPreJAddr+1, value);
		ReportMsg("ң��Ԥ�����ã����:%d .", wBOIndex);
	}
	else
	{
		outportb(BoMapTable[wBOIndex].dwPreJAddr, 0);
		outportb(BoMapTable[wBOIndex].dwPreJAddr+1, 0);
		ReportMsg("ң��Ԥ�ó��������:%d .", wBOIndex);
	}
	ReportMsg("ң��Ԥ�ú�״̬:");
	ReadInYX();
	return;
}

//ң��Ԥ�÷�У����
BOOL RemoteCtrlVer(WORD wBOIndex)
{
	return TRUE;
}

//ң��ִ�к���
void RemoteCtrlExe(WORD wBoIndex, BOOL bContext)
{
	BYTE value = 0;
	
	ReportMsg("\nң��ִ��ǰ״̬:");
	ReadInYX();

	if(BoMapTable[wBoIndex].dwPreJAddr == BoMapTable[wBoIndex].dwAddress)
	{
		value = (0x01<<BoMapTable[wBoIndex].bPreJOffset)|(0x01<<BoMapTable[wBoIndex].bOffset);
		if(bContext == TRUE)
		{
			ReportMsg("ң��ִ�����ã����:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, value);
		}
		else
		{
			ReportMsg("ң��ִ�г��������:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, 0);
		}
	}
	else
	{
		if(bContext == TRUE)
		{
			ReportMsg("ң��ִ�����ã����:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwPreJAddr, 0);
			outportb(BoMapTable[wBoIndex].dwPreJAddr+1, 0x01<<BoMapTable[wBoIndex].bPreJOffset);
			
			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, 0x01<<BoMapTable[wBoIndex].bOffset);

		}
		else
		{
			ReportMsg("ң��ִ�г��������:%d .", wBoIndex);
			outportb(BoMapTable[wBoIndex].dwPreJAddr, 0);
			outportb(BoMapTable[wBoIndex].dwPreJAddr+1, 0);

			outportb(BoMapTable[wBoIndex].dwAddress, 0);
			outportb(BoMapTable[wBoIndex].dwAddress+1, 0);

		}
	}
	ReportMsg("ң��ִ�к�״̬:");
	ReadInYX();
	return;
}
*/
//ң�ز��Ժ���
void BOTest(char cmd, char index)
{
	if(index<1)
		return;
	else
		index =index-1;
	switch(cmd)
	{
		case 'p':	//Ԥ��
			DB_BOSelect(index, BO_NULL);
			break;
		case 'e':	//ִ��
			DB_BOExec(index,BO_NULL,1);
			break;
		case 'd':	//ֱ��ִ��
			DB_BODirect(index, BO_NULL);
			break;
		case 'c':	//����
			DB_BOExec(index,BO_NULL,2);
			break;
			
	}
}



#ifdef _TEST_BO
void TestBOPort();
void ReadInYX();
void BOEnable();

//ң������,ע��YK_EN1~4��Ӧ�Ķ�ʱ�����ű���Ԥ�ȳ�ʼ��(�Ѿ��ڶ�ʱ���г�ʼ��)
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

	ReportMsg("		�ڲ�ң��״̬:%x %x, ң�ط�У״̬:%x %x %x .", bInYx[0], bInYx[1],\
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

