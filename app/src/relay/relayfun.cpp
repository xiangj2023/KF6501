/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			relayfun.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           		�����������ܺ���                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*                                
*                                                                                  
* ����                                                                             
*                                                                                  
*      	
*		
*		
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      yanzh             08/12/19    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "relayfun.h"
#include "logic.h"
#include "dbasedef.h"
#include "RelayDef.h"
#include "dbfunc.h"
#include "logicdef.h"
#include "sample.h"
#include "max11046.h"
#include "led.h"
#include "m5234timer.h"
#include "relay.h"
#include "scdefs.h"
#include "daefunc.h"
#include "RelayMemInit.h"
#include "relay_defs.h"
#include "ai.h"





extern DWORD 	dwRelayMSCnt;		//�����ú��������
extern BYTE 	byRelayWordValue[];
extern DWORD	dwBoInvalidStatus[BO_CHECK_BUF_LEN];	//�����Լ�ʧЧ״̬����λ��ʾ�쳣
extern DWORD	dwBoBreakStatus[BO_CHECK_BUF_LEN];		//�����Լ����״̬����λ��ʾ�쳣
extern DWORD 	dwBOStatus[MAX_BO_BOARD_NUM][2];
extern DWORD 	dwADDataWritePtr;

extern BYTE byDebugRelayBuf[]; //���Լ̵���������

//TAbsTime     RelayPickUpTime;
BYTE 	*g_MaintSW = NULL;
BYTE 	g_RptSerialNum = 0;			//�������к� 
BOOL    RelayCheckErr = FALSE;			//�����Լ����
BOOL 	RelayCheckInitOK = FALSE;
WORD	RelayCheckAcc = 0;

/************************************************************
	���ܣ����ϻ��ֻ����㷨����16
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluBaseFourier_S16(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14703*(-pDataBuf[1]+pDataBuf[33]);
			dwRealAmplitude += 565*(pDataBuf[2]+pDataBuf[16]-pDataBuf[18]-pDataBuf[32]);
			dwRealAmplitude += 1108*(pDataBuf[3]+pDataBuf[15]-pDataBuf[19]-pDataBuf[31]);
			dwRealAmplitude += 1609*(pDataBuf[4]+pDataBuf[14]-pDataBuf[20]-pDataBuf[30]);
			dwRealAmplitude += 2048*(pDataBuf[5]+pDataBuf[13]-pDataBuf[21]-pDataBuf[29]);
			dwRealAmplitude += 2408*(pDataBuf[6]+pDataBuf[12]-pDataBuf[22]-pDataBuf[28]);
			dwRealAmplitude += 2676*(pDataBuf[7]+pDataBuf[11]-pDataBuf[23]-pDataBuf[27]);
			dwRealAmplitude += 2841*(pDataBuf[8]+pDataBuf[10]-pDataBuf[24]-pDataBuf[26]);
			dwRealAmplitude += 2896*(pDataBuf[9]-pDataBuf[25]);
			dwRealAmplitude += 0*(-pDataBuf[17]);
			if(dwRealAmplitude>=0)                                                           /*�������뱣֤����*/
				elec->Real = (dwRealAmplitude+32767)>>16;
			else
				elec->Real = (dwRealAmplitude-32767)>>16;

			dwImagAmplitude += 1448*(pDataBuf[1]+pDataBuf[33]);
			dwImagAmplitude += 2841*(pDataBuf[2]-pDataBuf[16]-pDataBuf[18]+pDataBuf[32]);
			dwImagAmplitude += 2676*(pDataBuf[3]-pDataBuf[15]-pDataBuf[19]+pDataBuf[31]);
			dwImagAmplitude += 2408*(pDataBuf[4]-pDataBuf[14]-pDataBuf[20]+pDataBuf[30]);
			dwImagAmplitude += 2048*(pDataBuf[5]-pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
			dwImagAmplitude += 1609*(pDataBuf[6]-pDataBuf[12]-pDataBuf[22]+pDataBuf[28]);
			dwImagAmplitude += 1108*(pDataBuf[7]-pDataBuf[11]-pDataBuf[23]+pDataBuf[27]);
			dwImagAmplitude += 565*(pDataBuf[8]-pDataBuf[10]-pDataBuf[24]+pDataBuf[26]);
			dwImagAmplitude += 0*(-pDataBuf[9]-pDataBuf[25]);
			dwImagAmplitude += 2896*(-pDataBuf[17]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+32767)>>16;
			else
				elec->Imag = (dwImagAmplitude-32767)>>16;
		}
}
/************************************************************
	���ܣ����ϻ��ֻ����㷨����15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/

void CaluBaseFourier_S15(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14703*(-pDataBuf[1]+pDataBuf[33]);
			dwRealAmplitude += 565*(pDataBuf[2]+pDataBuf[16]-pDataBuf[18]-pDataBuf[32]);
			dwRealAmplitude += 1108*(pDataBuf[3]+pDataBuf[15]-pDataBuf[19]-pDataBuf[31]);
			dwRealAmplitude += 1609*(pDataBuf[4]+pDataBuf[14]-pDataBuf[20]-pDataBuf[30]);
			dwRealAmplitude += 2048*(pDataBuf[5]+pDataBuf[13]-pDataBuf[21]-pDataBuf[29]);
			dwRealAmplitude += 2408*(pDataBuf[6]+pDataBuf[12]-pDataBuf[22]-pDataBuf[28]);
			dwRealAmplitude += 2676*(pDataBuf[7]+pDataBuf[11]-pDataBuf[23]-pDataBuf[27]);
			dwRealAmplitude += 2841*(pDataBuf[8]+pDataBuf[10]-pDataBuf[24]-pDataBuf[26]);
			dwRealAmplitude += 2896*(pDataBuf[9]-pDataBuf[25]);
//			dwRealAmplitude += 0*(-pDataBuf[17]);

			elec->Real  = dwRealAmplitude>>15;
			
//			if(dwRealAmplitude>=0)                                                          //�������뱣֤����
//				elec->Real = (dwRealAmplitude+16384)>>15;
//			else
//				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 1448*(pDataBuf[1]+pDataBuf[33]);
			dwImagAmplitude += 2841*(pDataBuf[2]-pDataBuf[16]-pDataBuf[18]+pDataBuf[32]);
			dwImagAmplitude += 2676*(pDataBuf[3]-pDataBuf[15]-pDataBuf[19]+pDataBuf[31]);
			dwImagAmplitude += 2408*(pDataBuf[4]-pDataBuf[14]-pDataBuf[20]+pDataBuf[30]);
			dwImagAmplitude += 2048*(pDataBuf[5]-pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
			dwImagAmplitude += 1609*(pDataBuf[6]-pDataBuf[12]-pDataBuf[22]+pDataBuf[28]);
			dwImagAmplitude += 1108*(pDataBuf[7]-pDataBuf[11]-pDataBuf[23]+pDataBuf[27]);
			dwImagAmplitude += 565*(pDataBuf[8]-pDataBuf[10]-pDataBuf[24]+pDataBuf[26]);
//			dwImagAmplitude += 0*(-pDataBuf[9]-pDataBuf[25]);
			dwImagAmplitude += 2896*(-pDataBuf[17]);

			elec->Imag = dwImagAmplitude>>15;
			
//			if(dwImagAmplitude>=0)
//				elec->Imag = (dwImagAmplitude+16384)>>15;
//			else
//				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	���ܣ����ϻ��ֻ����㷨����14
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluBaseFourier_S14(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14703*(-pDataBuf[0]+pDataBuf[32]);
			dwRealAmplitude += 565*(pDataBuf[1]+pDataBuf[15]-pDataBuf[17]-pDataBuf[31]);
			dwRealAmplitude += 1108*(pDataBuf[2]+pDataBuf[14]-pDataBuf[18]-pDataBuf[30]);
			dwRealAmplitude += 1609*(pDataBuf[3]+pDataBuf[13]-pDataBuf[19]-pDataBuf[29]);
			dwRealAmplitude += 2048*(pDataBuf[4]+pDataBuf[12]-pDataBuf[20]-pDataBuf[28]);
			dwRealAmplitude += 2408*(pDataBuf[5]+pDataBuf[11]-pDataBuf[21]-pDataBuf[27]);
			dwRealAmplitude += 2676*(pDataBuf[6]+pDataBuf[10]-pDataBuf[22]-pDataBuf[26]);
			dwRealAmplitude += 2841*(pDataBuf[7]+pDataBuf[9]-pDataBuf[23]-pDataBuf[25]);
			dwRealAmplitude += 2896*(pDataBuf[8]-pDataBuf[24]);
			dwRealAmplitude += 0*(-pDataBuf[16]);
			if(dwRealAmplitude>=0)                                                           /*�������뱣֤����*/
				elec->Real = (dwRealAmplitude+8192)>>14;
			else
				elec->Real = (dwRealAmplitude-8192)>>14;

			dwImagAmplitude += 1448*(pDataBuf[0]+pDataBuf[32]);
			dwImagAmplitude += 2841*(pDataBuf[1]-pDataBuf[15]-pDataBuf[17]+pDataBuf[31]);
			dwImagAmplitude += 2676*(pDataBuf[2]-pDataBuf[14]-pDataBuf[18]+pDataBuf[30]);
			dwImagAmplitude += 2408*(pDataBuf[3]-pDataBuf[13]-pDataBuf[19]+pDataBuf[29]);
			dwImagAmplitude += 2048*(pDataBuf[4]-pDataBuf[12]-pDataBuf[20]+pDataBuf[28]);
			dwImagAmplitude += 1609*(pDataBuf[5]-pDataBuf[11]-pDataBuf[21]+pDataBuf[27]);
			dwImagAmplitude += 1108*(pDataBuf[6]-pDataBuf[10]-pDataBuf[22]+pDataBuf[26]);
			dwImagAmplitude += 565*(pDataBuf[7]-pDataBuf[9]-pDataBuf[23]+pDataBuf[25]);
			dwImagAmplitude += 0*(-pDataBuf[8]-pDataBuf[24]);
			dwImagAmplitude += 2896*(-pDataBuf[16]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+8192)>>14;
			else
				elec->Imag = (dwImagAmplitude-8192)>>14;
		}
}

/************************************************************
	���ܣ����ϻ��ֶ���г�����㣬��15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluSecondFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 7103*(-pDataBuf[0]);
			dwRealAmplitude += 1365*(pDataBuf[1]-pDataBuf[9]+pDataBuf[17]-pDataBuf[25]);
			dwRealAmplitude += 2239*(pDataBuf[2]-pDataBuf[10]+pDataBuf[18]-pDataBuf[26]);
			dwRealAmplitude += 2772*(pDataBuf[3]-pDataBuf[11]+pDataBuf[19]-pDataBuf[27]);
			dwRealAmplitude += 2882*(pDataBuf[4]-pDataBuf[12]+pDataBuf[20]-pDataBuf[28]);
			dwRealAmplitude += 2554*(pDataBuf[5]-pDataBuf[13]+pDataBuf[21]-pDataBuf[29]);
			dwRealAmplitude += 1837*(pDataBuf[6]-pDataBuf[14]+pDataBuf[22]-pDataBuf[30]);
			dwRealAmplitude += 841*(pDataBuf[7]-pDataBuf[15]+pDataBuf[23]-pDataBuf[31]);
			dwRealAmplitude += 284*(-pDataBuf[8]+pDataBuf[16]-pDataBuf[24]);
			dwRealAmplitude += 7387*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 2155*(pDataBuf[0]);
			dwImagAmplitude += 2554*(pDataBuf[1]-pDataBuf[9]+pDataBuf[17]-pDataBuf[25]);
			dwImagAmplitude += 1837*(pDataBuf[2]-pDataBuf[10]+pDataBuf[18]-pDataBuf[26]);
			dwImagAmplitude += 841*(pDataBuf[3]-pDataBuf[11]+pDataBuf[19]-pDataBuf[27]);
			dwImagAmplitude += 284*(-pDataBuf[4]+pDataBuf[12]-pDataBuf[20]+pDataBuf[28]);
			dwImagAmplitude += 1365*(-pDataBuf[5]+pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
			dwImagAmplitude += 2239*(-pDataBuf[6]+pDataBuf[14]-pDataBuf[22]+pDataBuf[30]);
			dwImagAmplitude += 2772*(-pDataBuf[7]+pDataBuf[15]-pDataBuf[23]+pDataBuf[31]);
			dwImagAmplitude += 2882*(-pDataBuf[8]+pDataBuf[16]-pDataBuf[24]);
			dwImagAmplitude += 728*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	���ܣ����ϻ�������г�����㣬��15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluThirdFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 4400*(-pDataBuf[0]);
			dwRealAmplitude += 2048*(pDataBuf[1]-pDataBuf[9]-pDataBuf[17]+pDataBuf[25]);
			dwRealAmplitude += 2841*(pDataBuf[2]-pDataBuf[8]-pDataBuf[18]+pDataBuf[24]);
			dwRealAmplitude += 2676*(pDataBuf[3]-pDataBuf[7]-pDataBuf[19]+pDataBuf[23]);
			dwRealAmplitude += 1609*(pDataBuf[4]-pDataBuf[6]-pDataBuf[20]+pDataBuf[22]);
			dwRealAmplitude += 0*(-pDataBuf[5]-pDataBuf[21]);
			dwRealAmplitude += 565*(-pDataBuf[10]-pDataBuf[16]+pDataBuf[26]);
			dwRealAmplitude += 1108*(pDataBuf[11]+pDataBuf[15]-pDataBuf[27]-pDataBuf[31]);
			dwRealAmplitude += 2408*(pDataBuf[12]+pDataBuf[14]-pDataBuf[28]-pDataBuf[30]);
			dwRealAmplitude += 2896*(pDataBuf[13]-pDataBuf[29]);
			dwRealAmplitude += 4965*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;
			
			dwImagAmplitude += 2352*(pDataBuf[0]);
			dwImagAmplitude += 2048*(pDataBuf[1]+pDataBuf[9]-pDataBuf[17]-pDataBuf[25]);
			dwImagAmplitude += 565*(pDataBuf[2]+pDataBuf[8]-pDataBuf[18]-pDataBuf[24]);
			dwImagAmplitude += 1108*(-pDataBuf[3]-pDataBuf[7]+pDataBuf[19]+pDataBuf[23]);
			dwImagAmplitude += 2408*(-pDataBuf[4]-pDataBuf[6]+pDataBuf[20]+pDataBuf[22]);
			dwImagAmplitude += 2896*(-pDataBuf[5]+pDataBuf[21]);
			dwImagAmplitude += 2841*(pDataBuf[10]-pDataBuf[16]-pDataBuf[26]);
			dwImagAmplitude += 2676*(pDataBuf[11]-pDataBuf[15]-pDataBuf[27]+pDataBuf[31]);
			dwImagAmplitude += 1609*(pDataBuf[12]-pDataBuf[14]-pDataBuf[28]+pDataBuf[30]);
			dwImagAmplitude += 0*(-pDataBuf[13]-pDataBuf[29]);
			dwImagAmplitude += 489*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	���ܣ����ϻ������г�����㣬��15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluFifthFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 1949*(-pDataBuf[0]);
			dwRealAmplitude += 2841*(pDataBuf[1]-pDataBuf[11]-pDataBuf[17]+pDataBuf[27]);
			dwRealAmplitude += 2048*(pDataBuf[2]-pDataBuf[10]-pDataBuf[18]+pDataBuf[26]);
			dwRealAmplitude += 565*(-pDataBuf[3]+pDataBuf[9]+pDataBuf[19]-pDataBuf[25]);
			dwRealAmplitude += 2676*(-pDataBuf[4]+pDataBuf[8]+pDataBuf[20]-pDataBuf[24]);
			dwRealAmplitude += 2408*(-pDataBuf[5]+pDataBuf[7]+pDataBuf[21]-pDataBuf[23]);
			dwRealAmplitude += 0*(-pDataBuf[6]-pDataBuf[22]);
			dwRealAmplitude += 1108*(-pDataBuf[12]-pDataBuf[16]+pDataBuf[28]);
			dwRealAmplitude += 1609*(pDataBuf[13]+pDataBuf[15]-pDataBuf[29]-pDataBuf[31]);
			dwRealAmplitude += 2896*(pDataBuf[14]-pDataBuf[30]);
			dwRealAmplitude += 3057*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 2375*(pDataBuf[0]);
			dwImagAmplitude += 565*(pDataBuf[1]+pDataBuf[11]-pDataBuf[17]-pDataBuf[27]);
			dwImagAmplitude += 2048*(-pDataBuf[2]-pDataBuf[10]+pDataBuf[18]+pDataBuf[26]);
			dwImagAmplitude += 2841*(-pDataBuf[3]-pDataBuf[9]+pDataBuf[19]+pDataBuf[25]);
			dwImagAmplitude += 1108*(-pDataBuf[4]-pDataBuf[8]+pDataBuf[20]+pDataBuf[24]);
			dwImagAmplitude += 1609*(pDataBuf[5]+pDataBuf[7]-pDataBuf[21]-pDataBuf[23]);
			dwImagAmplitude += 2896*(pDataBuf[6]-pDataBuf[22]);
			dwImagAmplitude += 2676*(pDataBuf[12]-pDataBuf[16]-pDataBuf[28]);
			dwImagAmplitude += 2408*(pDataBuf[13]-pDataBuf[15]-pDataBuf[29]+pDataBuf[31]);
			dwImagAmplitude += 0*(-pDataBuf[14]-pDataBuf[30]);
			dwImagAmplitude += 301*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;	
		}
}
/************************************************************
	���ܣ����ϻ����ߴ�г�����㣬��15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluSeventhFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 663*(-pDataBuf[0]);
			dwRealAmplitude += 2676*(pDataBuf[1]+pDataBuf[5]-pDataBuf[17]-pDataBuf[21]);
			dwRealAmplitude += 565*(-pDataBuf[2]-pDataBuf[4]+pDataBuf[18]+pDataBuf[20]);
			dwRealAmplitude += 2896*(-pDataBuf[3]+pDataBuf[19]);
			dwRealAmplitude += 1609*(pDataBuf[6]-pDataBuf[16]-pDataBuf[22]);
			dwRealAmplitude += 2048*(-pDataBuf[7]+pDataBuf[15]+pDataBuf[23]-pDataBuf[31]);
			dwRealAmplitude += 2408*(-pDataBuf[8]+pDataBuf[14]+pDataBuf[24]-pDataBuf[30]);
			dwRealAmplitude += 1108*(pDataBuf[9]-pDataBuf[13]-pDataBuf[25]+pDataBuf[29]);
			dwRealAmplitude += 2841*(pDataBuf[10]-pDataBuf[12]-pDataBuf[26]+pDataBuf[28]);
			dwRealAmplitude += 0*(-pDataBuf[11]-pDataBuf[27]);
			dwRealAmplitude += 2272*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 2184*(pDataBuf[0]);
			dwImagAmplitude += 1108*(-pDataBuf[1]+pDataBuf[5]+pDataBuf[17]-pDataBuf[21]);
			dwImagAmplitude += 2841*(-pDataBuf[2]+pDataBuf[4]+pDataBuf[18]-pDataBuf[20]);
			dwImagAmplitude += 0*(-pDataBuf[3]-pDataBuf[19]);
			dwImagAmplitude += 2408*(-pDataBuf[6]-pDataBuf[16]+pDataBuf[22]);
			dwImagAmplitude += 2048*(-pDataBuf[7]-pDataBuf[15]+pDataBuf[23]+pDataBuf[31]);
			dwImagAmplitude += 1609*(pDataBuf[8]+pDataBuf[14]-pDataBuf[24]-pDataBuf[30]);
			dwImagAmplitude += 2676*(pDataBuf[9]+pDataBuf[13]-pDataBuf[25]-pDataBuf[29]);
			dwImagAmplitude += 565*(-pDataBuf[10]-pDataBuf[12]+pDataBuf[26]+pDataBuf[28]);
			dwImagAmplitude += 2896*(-pDataBuf[11]+pDataBuf[27]);
			dwImagAmplitude += 224*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	���ܣ����ϻ��־Ŵ�г�����㣬��15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluNinthFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 184*(pDataBuf[0]);
			dwRealAmplitude += 1609*(pDataBuf[1]+pDataBuf[7]-pDataBuf[17]-pDataBuf[23]);
			dwRealAmplitude += 2676*(-pDataBuf[2]-pDataBuf[6]+pDataBuf[18]+pDataBuf[22]);
			dwRealAmplitude += 565*(-pDataBuf[3]-pDataBuf[5]+pDataBuf[19]+pDataBuf[21]);
			dwRealAmplitude += 2896*(pDataBuf[4]-pDataBuf[20]);
			dwRealAmplitude += 2048*(pDataBuf[8]-pDataBuf[16]-pDataBuf[24]);
			dwRealAmplitude += 2408*(-pDataBuf[9]+pDataBuf[15]+pDataBuf[25]-pDataBuf[31]);
			dwRealAmplitude += 1108*(-pDataBuf[10]+pDataBuf[14]+pDataBuf[26]-pDataBuf[30]);
			dwRealAmplitude += 2841*(pDataBuf[11]-pDataBuf[13]-pDataBuf[27]+pDataBuf[29]);
			dwRealAmplitude += 0*(-pDataBuf[12]-pDataBuf[28]);
			dwRealAmplitude += 1864*(pDataBuf[32]);
			if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;

			dwImagAmplitude += 1864*(pDataBuf[0]);
			dwImagAmplitude += 2408*(-pDataBuf[1]+pDataBuf[7]+pDataBuf[17]-pDataBuf[23]);
			dwImagAmplitude += 1108*(-pDataBuf[2]+pDataBuf[6]+pDataBuf[18]-pDataBuf[22]);
			dwImagAmplitude += 2841*(pDataBuf[3]-pDataBuf[5]-pDataBuf[19]+pDataBuf[21]);
			dwImagAmplitude += 0*(-pDataBuf[4]-pDataBuf[20]);
			dwImagAmplitude += 2048*(-pDataBuf[8]-pDataBuf[16]+pDataBuf[24]);
			dwImagAmplitude += 1609*(-pDataBuf[9]-pDataBuf[15]+pDataBuf[25]+pDataBuf[31]);
			dwImagAmplitude += 2676*(pDataBuf[10]+pDataBuf[14]-pDataBuf[26]-pDataBuf[30]);
			dwImagAmplitude += 565*(pDataBuf[11]+pDataBuf[13]-pDataBuf[27]-pDataBuf[29]);
			dwImagAmplitude += 2896*(-pDataBuf[12]+pDataBuf[28]);
			dwImagAmplitude += 184*(pDataBuf[32]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
		}
}
/************************************************************
	���ܣ����ϻ���ʮһ��г�����㣬��15
	������pDataBuf	--	����Ҷ���ݻ�����
		  elec		--	����������ṹ
	���أ���
************************************************************/
void CaluEleventhFourier(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 774*(pDataBuf[0]);
		dwRealAmplitude += 0*(-pDataBuf[1]-pDataBuf[17]);
		dwRealAmplitude += 2408*(-pDataBuf[2]-pDataBuf[16]+pDataBuf[18]);
		dwRealAmplitude += 2676*(pDataBuf[3]+pDataBuf[15]-pDataBuf[19]-pDataBuf[31]);
		dwRealAmplitude += 565*(-pDataBuf[4]-pDataBuf[14]+pDataBuf[20]+pDataBuf[30]);
		dwRealAmplitude += 2048*(-pDataBuf[5]-pDataBuf[13]+pDataBuf[21]+pDataBuf[29]);
		dwRealAmplitude += 2841*(pDataBuf[6]+pDataBuf[12]-pDataBuf[22]-pDataBuf[28]);
		dwRealAmplitude += 1108*(-pDataBuf[7]-pDataBuf[11]+pDataBuf[23]+pDataBuf[27]);
		dwRealAmplitude += 1609*(-pDataBuf[8]-pDataBuf[10]+pDataBuf[24]+pDataBuf[26]);
		dwRealAmplitude += 2896*(pDataBuf[9]-pDataBuf[25]);
		dwRealAmplitude += 1634*(pDataBuf[32]);
		if(dwRealAmplitude>=0)
				elec->Real = (dwRealAmplitude+16384)>>15;
			else
				elec->Real = (dwRealAmplitude-16384)>>15;
		dwImagAmplitude += 1448*(pDataBuf[0]);
		dwImagAmplitude += 2896*(-pDataBuf[1]+pDataBuf[17]);
		dwImagAmplitude += 1609*(pDataBuf[2]-pDataBuf[16]-pDataBuf[18]);
		dwImagAmplitude += 1108*(pDataBuf[3]-pDataBuf[15]-pDataBuf[19]+pDataBuf[31]);
		dwImagAmplitude += 2841*(-pDataBuf[4]+pDataBuf[14]+pDataBuf[20]-pDataBuf[30]);
		dwImagAmplitude += 2048*(pDataBuf[5]-pDataBuf[13]-pDataBuf[21]+pDataBuf[29]);
		dwImagAmplitude += 565*(pDataBuf[6]-pDataBuf[12]-pDataBuf[22]+pDataBuf[28]);
		dwImagAmplitude += 2676*(-pDataBuf[7]+pDataBuf[11]+pDataBuf[23]-pDataBuf[27]);
		dwImagAmplitude += 2408*(pDataBuf[8]-pDataBuf[10]-pDataBuf[24]+pDataBuf[26]);
		dwImagAmplitude += 0*(-pDataBuf[9]-pDataBuf[25]);
		dwImagAmplitude += 161*(pDataBuf[32]);
		if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+16384)>>15;
			else
				elec->Imag = (dwImagAmplitude-16384)>>15;
	}
}

/************************************************************
	���ܣ���֪ʵ�����鲿����ģֵ
	������elec		--	����������ṹ�����ѽ���ʵ���鲿����
	���أ���
************************************************************/
void CaluModValue(TRelElecValPar *elec)
{
	DWORD dwBig,dwSmall,dwTemp1;
	if(elec!=NULL)
	{
		dwBig   = ToAbs(elec->Imag);
		dwSmall = ToAbs(elec->Real);
		if (dwBig!=dwSmall)
		{
			if(dwSmall>dwBig)
			{
				dwTemp1 = dwSmall;
				dwSmall = dwBig;
				dwBig   = dwTemp1; 
			}												     /*�������뱣֤����*/
			dwTemp1 = dwBig+(dwSmall*10+15)/30;                                /* B+S/3 */
			if(dwTemp1<2)
			{
				elec->Mod =0;
				return;
			}
			dwTemp1 = dwTemp1+(dwBig*dwBig+dwSmall*dwSmall+dwTemp1/2)/dwTemp1;  /* B+S/3+(B*B+S*S)/(B+S/3) */
			elec->Mod = (dwTemp1+1)/2;
		}
		else
			elec->Mod = (dwBig*1414+500)/1000;
	}
	
}
/************************************************************
	���ܣ����迹�ĽǶ�
	������elec		--	����������ṹ�����ѽ���ʵ���鲿����
	���أ���
************************************************************/
void CaluImpdAngle(TRelElecValPar *pElec,const WORD *pAngleTab)
{
	TRelElecValPar elec;
	DWORD dwBig,dwSmall,dwTemp1;
	
	elec.Real = pElec->Real/10;
	elec.Imag = pElec->Imag/10;
	
	dwBig   = ToAbs(elec.Imag);
	dwSmall = ToAbs(elec.Real);
	if (dwBig!=dwSmall)
	{
		if(dwSmall>dwBig)
		{
			dwTemp1 = dwSmall;
			dwSmall = dwBig;
			dwBig   = dwTemp1; 
		}												     /*�������뱣֤����*/
		dwTemp1 = dwBig+(dwSmall)/3;                                /* B+S/3 */
		dwTemp1 = dwTemp1+(dwBig*dwBig+dwSmall*dwSmall)/dwTemp1;  /* B+S/3+(B*B+S*S)/(B+S/3) */
		elec.Mod = dwTemp1/2;
	}
	else
		elec.Mod = (dwBig*1414+500)/1000;

	CaluAngle(&elec,pAngleTab);
	
	pElec->Mod   = elec.Mod*10;
	pElec->Angle = elec.Angle;
	
}

/************************************************************
	���ܣ��Ƕȼ���
	������elec		--	����������ṹ�����ѽ���ʵ���鲿ģֵ����
		  pAngleTab	--	�Ƕȼ����ѯ���ַ
	���أ���
************************************************************/
void CaluAngle(TRelElecValPar *elec,const WORD *pAngleTab)
{
	LONG swReal;
	LONG swImag;
	DWORD  dwMod,dwReal,dwImag,dwAsin,dwangle;
	
	if (elec !=0)
	{
		swReal 	= elec->Real;
		swImag 	= elec->Imag;
		dwMod  	= elec->Mod;
		dwReal 	= ToAbs(swReal);
		dwImag 	= ToAbs(swImag);
		
		if((swReal==0)&&(swImag>0))			  /*�жϵ����Ƿ�λ����������*/
				dwangle = 9000;			
		if((swReal==0)&&(swImag<0))	
				dwangle = 27000;			
		if((swImag==0)&&(swReal>0))		
				dwangle = 0;			
		if((swImag==0)&&(swReal<0))			
				dwangle = 18000;

		if ((dwMod>1)&&swReal&&swImag)    
		{
			if(dwImag!=dwReal)
			{
				if(dwImag<dwReal)              /*����鲿С��ʵ����������Ϊ����ֵ*/
					dwAsin = (dwImag*6364)/(10*dwMod);
				else						   /*����鲿����ʵ����������Ϊ����ֵ*/
					dwAsin = (dwReal*6364)/(10*dwMod);	

				if(dwAsin>449)  //�±�Խ��
				{
					elec->Angle = 0;
					return;
				}
				dwangle = pAngleTab[dwAsin];				
				if(dwImag>dwReal)			   /*��Ϊ���ң�ת�������ҽǶ�*/
					dwangle = 9000-dwangle;
			}
			else
				dwangle = 4500;
			
			if (swReal<0&&swImag>0)	           /*�����鲿ʵ���������жϸõ���λ����һ����*/
				dwangle = 18000-dwangle;
			if (swReal<0&&swImag<0)
				dwangle = 18000+dwangle;
			if (swReal>0&&swImag<0)
				dwangle = 36000-dwangle;
			
		}
	//	dwangle = dwangle - BASEANGLE;  /*��ʱ���룬�˴�Ϊ�ǶȻ�׼*/ 
	//	if(dwangle<=0)
	//		dwangle += 36000;			
		elec->Angle = dwangle;		
	}
}
/************************************************************
	���ܣ�ͨ���˲�
	������pDataBuf	--	���˲�ͨ��ԭʼ���ݻ�����
	���أ���
************************************************************/
void Filter(LONG  pDataBuf[READ_POINT_NUM])
{
	LONG DCComponent=0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		DCComponent +=pDataBuf[i]; 
	}
	DCComponent=DCComponent/POINT_PER_PERIOD;
	for(LONG i=0;i<READ_POINT_NUM;i++)
	{
		pDataBuf[i]=pDataBuf[i]-DCComponent;
	}

}
/************************************************************
	���ܣ�32λ��������
	������par			--	��������
	���أ���
************************************************************/
WORD Evolution32(DWORD par)
{
	WORD SquareRoot=0x8000;
	if(par<0)
		return 0;
	for(LONG i=0;i<16;i++)
	{
		SquareRoot |= (1<<(15-i));			
		if(par < (DWORD)SquareRoot*SquareRoot)
			SquareRoot &=~(1<<(15-i));
	}
	return SquareRoot;
}
/************************************************************
	���ܣ�64λ��������
	������par			--	��������
	���أ���
************************************************************/
DWORD Evolution64(QWORD par)
{
	DWORD SquareRoot=0x80000000;
	if(par<0)
		return 0;
	for(LONG i=0;i<32;i++)
	{
		SquareRoot |= (1<<(31-i));			
		if(par < (QWORD)SquareRoot*SquareRoot)
			SquareRoot &=~(1<<(31-i));
	}
	return SquareRoot;
}

/************************************************************
	���ܣ�������ֵ��ģֵ
	������DataBuf		--	���������ݻ�����
		  Elec		--   ���ò�������������ģֵ
	���أ���
************************************************************/
void  CaluMSMod(const LONG *DataBuf,LONG &Elec)
{
	LLONG tElec = 0;
	for(LONG i=0;i<POINT_PER_PERIOD;i++)
	{
		tElec += DataBuf[i+2]*DataBuf[i+2];
	}
	
	Elec = Evolution64(tElec)/2;		//ע�⣬�˴�Ӧ�ó�������32������ֻ�� ��4����ģֵ�Ŵ���2.828��

}
/************************************************************
	���ܣ�ð������
	������Buf			--   ���������ݻ�����
		  BufSize		--   ���ݻ�������С
	���أ���
************************************************************/
void BubbleSort(LONG *Buf,LONG BufSize)						//ð�ݷ����� 
{
	LONG i,j;
	LONG temp;
	for (i=1;i<BufSize;i++)
	{
		for(j=BufSize-1;j>=i;j--) 
		{
			if (Buf[j]<Buf[j-1])
			{
				temp = Buf[j];
				Buf[j] = Buf[j-1];
				Buf[j-1] = temp;
			}
		}
	}	
}

/************************************************************
	���ܣ������ݿ���д��ң��ֵ---����������ֵ
	������no			--   ң�����
		  val			--   ң��ֵ
		  BubbleData	--   ���򻺳���
		  OriginalData	--   ���������ݻ�����
	���أ���
************************************************************/
void WriteAIData(WORD no,LONG val, LONG *BubbleData,LONG *OriginalData)
{
	LONG value;
	if(NUMOFYCVALUE>3)  //����������С��3������������
	{
		for(LONG i=0;i<NUMOFYCVALUE-1;i++)
			OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];
		
		OriginalData[0]=val;	
		for(LONG i=0;i<NUMOFYCVALUE;i++)
		{	
			BubbleData[i]=OriginalData[i];
		}
		BubbleSort(BubbleData,NUMOFYCVALUE);
		value = BubbleData[NUMOFYCVALUE/2];
	}
	else
		value = val;
	
	DB_WriteAI(no,value);

}
void WriteAIData(WORD no,LONG val, LONG Drift,LONG *BubbleData,LONG *OriginalData,BOOL haveDrift) //дң��ֵ
{
	LONG value;
	if(NUMOFYCVALUE>3)  //����������С��3������������
	{
		for(LONG i=0;i<NUMOFYCVALUE-1;i++)
			OriginalData[NUMOFYCVALUE-1-i]=OriginalData[NUMOFYCVALUE-2-i];
		
		OriginalData[0]=val;	
		for(LONG i=0;i<NUMOFYCVALUE;i++)
		{	
			BubbleData[i]=OriginalData[i];
		}
		BubbleSort(BubbleData,NUMOFYCVALUE);
		value = BubbleData[NUMOFYCVALUE/2];
	}
	else
		value = val;

	if(haveDrift)
	{
		if(value<Drift)
			value =0;
	}
	
	DB_WriteAI(no,value);
}
/************************************************************
	���ܣ������ݿ���д�뱣������ֵ
	������RMNo			--   �����������
		  val			--   ��������ֵ
		  pRM			--   ��������ֵ��ַ
	���أ���
************************************************************/
void WriteRMData(LONG *pRM,DWORD RMNo, LONG val) //дң��ֵ
{
	*pRM = val;
	if(RMNo>=RM_COUNT)
	{
		ReportMsg("relayfun err! rm no overflow!");
		return;
	}
	DB_LogWriteRM(RMNo, val); 
}

/************************************************************
	���ܣ�ԭʼ���ݾ���ð�������ȡ�м�ֵ
	������num			--   ԭʼ���ݸ���
		  val			--   ֵ
		  OriginalData	--   ���������ݻ�����
	���أ���
************************************************************/
LONG GetSortData(LONG val,WORD num,LONG *OriginalData,LONG *BubbleData)
{
	LONG value;	
	if(num>3)
	{
		for(LONG i=0;i<num-1;i++)
			OriginalData[num-1-i]=OriginalData[num-2-i];
		
		OriginalData[0]=val;	
		for(LONG i=0;i<num;i++)
		{	
			BubbleData[i]=OriginalData[i];
		}
		BubbleSort(BubbleData,num);
		value = BubbleData[num/2];
	}
	else
		value = val;

	return value;
	
}

/************************************************************
	���ܣ��迹����
	������Uele		--   ��ѹָ��
		  Iele		--   ����ָ��
		  Zele		--	�����迹ָ��
		  MinCur		--	С����ֵ
	���أ���
************************************************************/

void CaluRX(TRelElecValPar *Uele,TRelElecValPar *Iele,TRelElecValPar *Zele,DWORD RW50I,LONG *Zcoef)
{		
//	if(ReadRelayWord(RW50I))
	if(Iele->Mod<15)
	{		
#ifdef PRO_6571
		Zele->Real = (INVALIDRX*(*Zcoef)/(*AS_pnZRatio))*1000;
		Zele->Imag = (INVALIDRX*(*Zcoef)/(*AS_pnZRatio))*1000;	
#else
		Zele->Real = INVALIDRX*(*Zcoef);
		Zele->Imag = INVALIDRX*(*Zcoef);	
#endif
	}
	else      //������������������ 
	{
		LLONG temp1,temp2,temp3;
		temp1 = Iele->Real* Iele->Real + Iele->Imag*Iele->Imag;
		temp2 = ((LLONG)Iele->Real * Uele->Real + (LLONG)Iele->Imag * Uele->Imag)*IMPDMULTIPLE;
		temp3 = ((LLONG)Iele->Real * Uele->Imag -(LLONG)Iele->Imag * Uele->Real)*IMPDMULTIPLE;

		if(temp1<100)
		{				
#ifdef PRO_6571
			Zele->Real = INVALIDRX*(*Zcoef)/(*AS_pnZRatio);
			Zele->Imag = INVALIDRX*(*Zcoef)/(*AS_pnZRatio); 
#else
			Zele->Real = INVALIDRX*(*Zcoef);
			Zele->Imag = INVALIDRX*(*Zcoef);	
#endif
			return;
		}		
		Zele->Real 	= temp2 /temp1;
		Zele->Imag	= temp3 /temp1;
	}	
}
/************************************************************
	���ܣ��򵥹�������
	������Relay		--   �򵥵Ĺ�Ƿ�������ṹ
	���أ���
************************************************************/
void SimpleOverValRelay(TRelSimpGeneOvUnPar *Relay)
{
	if((Relay->pTEleVal->Mod) >(*(Relay->pEleSet)))
	{
		if(ReadRelayWord(Relay->ACTRelayWord))
			return;
		if(ReadRelayWord(Relay->StartRelayWord))
		{
			WriteRelayWord(Relay->ACTRelayWord, RELAY_ON);
			return;
		}
		else
		{
			WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
			return;
		}
	}

	if((Relay->pTEleVal->Mod) <(*(Relay->pEleSet))*99/100)
	{
		WriteRelayWord(Relay->ACTRelayWord, RELAY_OFF);
		WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	}
    else
  	{	     		
  		if(ReadRelayWord(Relay->StartRelayWord))
			WriteRelayWord(Relay->ACTRelayWord, RELAY_ON);
		else
			return;
	}
}
/************************************************************
	���ܣ���Ƿ������
	������Relay		--   �򵥵Ĺ�Ƿ�������ṹ
	���أ���
************************************************************/
void SimpleUnderValRelay(TRelSimpGeneOvUnPar *Relay)
{
	if ((Relay->pTEleVal->Mod) < (*(Relay->pEleSet)))
	{
		if (!ReadRelayWord(Relay->ACTRelayWord))
		{
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
			return;
		}
		else
			return;
	}
	else
	{
		if ((Relay->pTEleVal->Mod)<=(*(Relay->pEleSet))*101/100)
			return;
		else
		{
			WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
		}
	}

}

/************************************************************
	���ܣ���������������
	������RptCategory	--   ���������Inf��
		  SerialNum	--	�������кţ�Ψһ�����ݿ���֯��
		  RptRatioTab	--	���ϵ�����ʾһ��ֵ����ȱ��ַ
	���أ���
************************************************************/
void MakeActRpt(WORD RptCategory,WORD SerialNum)
{
	if(RptCategory == RPT_NULL) //��Ч��INF��
		return;
	TRelayEventItem eventreport; 	

	
	//��ɶ�������
	ReadAbsTime(&eventreport.stTime);	
	eventreport.bStatus  = 1;   //����
	eventreport.nINF     = ReportInf[RptCategory].wInf;//INF��
	
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
		eventreport.FD_Value[j] = *(ReportInf[RptCategory].pFailureData[j]);
	
	DB_RelayAction(SerialNum,&eventreport);  //�����ݿ���д�붯������
}
/************************************************************
	���ܣ����澯����
	������RptCategory	--   �澯�����Inf��
		  RptRatioTab	--	���ϵ�����ʾһ��ֵ����ȱ��ַ
	���أ���
************************************************************/
void MakeAlarmRpt(WORD RptCategory)
{
	TRelayEventItem eventreport; 	
	ReadAbsTime(&eventreport.stTime);	
	eventreport.bStatus  = 1;   //����
	eventreport.nINF     = ReportInf[RptCategory].wInf;//INF��
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
	{
		eventreport.FD_Value[j] = *(ReportInf[RptCategory].pFailureData[j]);
	}
	DB_RelayAlarm(&eventreport);
}
/************************************************************
	���ܣ��������ϵ�������������
	������RptCategory	--   ���������Inf��
		  RptSerialNum	--   �������кţ�Ψһ�����ݿ���֯��
	���أ���
************************************************************/

void MakeStartRpt(WORD RptCategory,WORD RptSerialNum )
{	
	LONG pFD[48]; //Ҫ��֤��������Ĺ��ϵ�����������16
	
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
		pFD[j] = *(ReportInf[RptCategory].pFailureData[j]);

	DB_RelayWriteFD (RptSerialNum, pFD);
	DB_RelayEnd(RptSerialNum);		
}
/************************************************************
	���ܣ��������ϵ�������α���
	������RptCategory	--   ���������Inf��
		  RptSerialNum	--   �������кţ�Ψһ�����ݿ���֯��
	���أ���
************************************************************/
void MakePickupRpt(WORD RptCategory, WORD RptSerialNum)
{	
	LONG pFD[MAX_FD_NUM]; 
	for(int j=0;j<ReportInf[RptCategory].dwFialureNum ;j++)
		pFD[j] = *(ReportInf[RptCategory].pFailureData[j]);
	DB_RelayWriteFD (RptSerialNum, pFD);
}
/************************************************************
	���ܣ�ͨ�ù�������
	������Relay		--  ͨ�ù������������ṹ
		  dwK35		--  K35��������г������Ӧ��
		  RptSerialNum --  �������кţ�Ψһ�����ݿ���֯������
	���أ���
************************************************************/
void GeneralOverValueRelay(TRelGeneOvValPar *Relay)
{	
	LONG ElecSet = *(Relay->pElecSet);

	if(*(Relay->pRelSW) == SW_STATUS_CUT)   //ѹ��δͶ�룬ֱ�Ӹ�λ�˳� 
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay(Relay);   //����δͶ���Ҽ���δͶ����ٲ��������־������ 
		return;
	}
		

	if (*(Relay->pRelCFG) == CFG_STATUS_CLR)  //����δͶ�� 
	{
		if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord)))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);   //����δͶ���Ҽ���δͶ����ٲ��������־������ 
			return;
		}		
	}
	if (*(Relay->pRelHSuprCFG) == CFG_STATUS_SET) //г������Ͷ�� 
	{
		//���ǵ���Ϊ0����� 
		if (Relay->pRelElec->Mod != 0)
			ElecSet = (*Relay->dwK35)*ElecSet/(*Relay->pH2Coef);
	}

	if (ReadRelayWord(Relay->AccActRelayWord) || ReadRelayWord(Relay->ActRelayWord)) //���ٻ��߹��������Ѿ������� 
	{	
		//���������ķ��ط��������
		// 1. ������������բ�ɹ��������ʧ����������
		// 2. ��ѹ��������բ�ɹ����п�����Ȼ��ѹ����ʱ����������������
		if (((Relay->pRelElec->Mod) < (ElecSet*NOCURCOEF/100))||(!ReadRelayWord(Relay->RSTRelayWord)))
		{		
			Relay->byRelRCounter++;
			if(Relay->byRelRCounter>3)   //���ӷ��ؼ�����yanzh 2012��8��23��14:04:07
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay(Relay);
			}
			return;
		}
		else
		{
			Relay->byRelRCounter=0;
			return;		
		}
	}
	
	if (Relay->pRelElec->Mod < ElecSet) //��ǰ����С�ڶ�ֵ 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //��������� 
		{
			if (Relay->pRelElec->Mod < ElecSet*99/100) //�����Ѿ�����ҵ���С�ڷ��ض�ֵ 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay(Relay);
				return;
			}			
		}
		else
		{
			if(Relay->pRelElec->Mod >= *(Relay->pElecSet))      //г������Ͷ�������£����������ڳ�ʼ��ֵʱ�������ܷ���
				return;
			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);
			return;		
		}
	}
	
	if (ReadRelayWord(Relay->LockRelayWord))
	{
		//��α����̵�������λ
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay(Relay); 
		return;
	}	
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (*(Relay->pRelHLockCFG) == CFG_STATUS_SET)
	{
		if (ReadRelayWord(Relay->H2LockRelayWord))      //����г������Ͷ���ұ��������� 
		{
			ResetOverValueRelay(Relay);
			return;
		}
	}
	if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord))) //����δͶ����߼��ٲ��������й��ʷ����ж� 
	{
		if (*(Relay->pRelPDCFG) == CFG_STATUS_SET)   //�����������ʷ���Ͷ�� 
		{
			if(*(Relay->pRelFDCFG) == CFG_STATUS_SET)   //��������������Ͷ�� 
			{
				if(ReadRelayWord(Relay->PRewardRelayWord)&&(!ReadRelayWord(Relay->PForwardRelayWord)))  //������̵�������λ��������̵�����û����λ 
				{
					ResetOverValueRelay(Relay);
					return;
				}
			}
			else
			{
				if(ReadRelayWord(Relay->PForwardRelayWord)&&(!ReadRelayWord(Relay->PRewardRelayWord)))   //������̵�������λ��������̵�����û����λ 
				{
					ResetOverValueRelay(Relay);
					return;
				}				
			}
		}
	}
	
	if (*(Relay->pRelVolLockCFG) == CFG_STATUS_SET)
	{
		if ((Relay->pRelVolt->Mod) > (*(Relay->pVolLockSet)))     //��ѹ����Ͷ���ҵ�ѹ���ڶ�ֵ 
		{
			ResetOverValueRelay(Relay);
			return;
		}
	}
	if (*(Relay->pRel47UCFG) == CFG_STATUS_SET)   //��ѹ��������
	{
		if(ReadRelayWord(Relay->R47ULKRelayWord))
		{
			ResetOverValueRelay(Relay);
			return;
		}
	}
	if (!ReadRelayWord(Relay->StartRelayWord))   //����û����� 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>=2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			if((*(Relay->pRelAccCFG) == CFG_STATUS_SET)&&(ReadRelayWord(Relay->AccRelayWord))) //yanzh 2012-5-7 15:21:11
			{
				Relay->wPickUpType = ACCPICK;
				MakeActRpt(Relay->wAccPickUpRptNum,*Relay->RptSerialNum);
			}
			else
			{				
				if(Relay->bMakeActRpt == TRUE)
					MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
				Relay->wPickUpType = NORMALPICK;
			}
			return;        //������ʽʵ��ע�͵������ٱ�������ʱ�䣬1����Σ�2����բ
		}
		else
			return;
	}  
	
	if ((*(Relay->pRelAccCFG) == CFG_STATUS_SET) && (Relay->wPickUpType == ACCPICK))
	{															   //����Ͷ���Ҽ��������ˣ����ٶ��� 	
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pAccActTimeSet)))
		{
			WriteRelayWord(Relay->AccActRelayWord,RELAY_ON);      //��ʱ�ѵ��������ٶ����̵�������λ 
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //��ʱ�ѵ���ͬʱ�������̵�������λ 
			MakeActRpt(Relay->wAccReportNum,*Relay->RptSerialNum);
			Relay->byRelRCounter = 0;
			return;
		}		
	}
	if(*(Relay->pRelCFG) == CFG_STATUS_SET)
	{
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pTimeSet)))
		{
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //��ʱ�ѵ������̵�������λ 			
			Relay->byRelRCounter = 0;
			if(Relay->bMakeActRpt == TRUE)
			{
				MakeActRpt(Relay->wReportNum,*Relay->RptSerialNum);
			}
			else
				MakeAlarmRpt(Relay->wReportNum);
		}
	}

}
/************************************************************
	���ܣ�ͨ�ù�������,ֻ��λ�̵����֣�����г�����ƹ���
	������Relay		--  ͨ�ù������������ṹ
		  dwK35		--  K35��������г������Ӧ��
		  RptSerialNum --  �������кţ�Ψһ�����ݿ���֯������
	���أ���
************************************************************/
void GeneralOverValueRelay_2(TRelGeneOvValPar *Relay)
{	
	LONG ElecSet = *(Relay->pElecSet);

	if((*(Relay->pRelSW) == SW_STATUS_CUT)||(*(Relay->pRelCFG) == CFG_STATUS_CLR))   //ѹ��δͶ�룬ֱ�Ӹ�λ�˳� 
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay(Relay);   //����δͶ���Ҽ���δͶ����ٲ��������־������ 
		return;
	}
	if (ReadRelayWord(Relay->AccActRelayWord) || ReadRelayWord(Relay->ActRelayWord)) //���ٻ��߹��������Ѿ������� 
	{	
		//���������ķ��ط��������
		// 1. ������������բ�ɹ��������ʧ����������
		// 2. ��ѹ��������բ�ɹ����п�����Ȼ��ѹ����ʱ����������������
		if (((Relay->pRelElec->Mod) < (ElecSet*NOCURCOEF/100))||(!ReadRelayWord(Relay->RSTRelayWord)))
		{		
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);
			return;
		}
		else			
			return;		
	}
	
	if (Relay->pRelElec->Mod < ElecSet) //��ǰ����С�ڶ�ֵ 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //��������� 
		{
			if (Relay->pRelElec->Mod < ElecSet*99/100) //�����Ѿ�����ҵ���С�ڷ��ض�ֵ 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay(Relay);
				return;
			}			
		}
		else
		{			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay(Relay);
			return;		
		}
	}
	
	if (ReadRelayWord(Relay->LockRelayWord))
	{
		ResetOverValueRelay(Relay); //��μ̵���������������α�����
		return;
	}
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (!ReadRelayWord(Relay->StartRelayWord))   //����û����� 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			return;
		}
		else
			return;
	}  
	if ((*(Relay->pSysTimer) - Relay->dwRelTimer + INHERENTTIME) > (*(Relay->pTimeSet)))
	{
		WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //��ʱ�ѵ������̵�������λ 
	}

}

/************************************************************
	���ܣ���λͨ�ù�������
	������Relay		--  ͨ�ù������������ṹ
	���أ���
************************************************************/
void ResetOverValueRelay(TRelGeneOvValPar *Relay,BOOL resetFlag)
{	
	//if (ReadRelayWord(Relay->StartRelayWord)&&(*Relay->RptPickupFlag == TRUE))   //������� 
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //������� 
	{
		if(Relay->wPickUpType == ACCPICK)
		{
			MakeActRpt(Relay->wAccResetRptNum, *Relay->RptSerialNum);	
		}
		else
		{
			if(Relay->bMakeActRpt == TRUE)
			MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
		}
	}
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);    
	WriteRelayWord(Relay->AccActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ActRelayWord,RELAY_OFF);
	Relay->dwRelTimer = 0;
	Relay->byRelFCounter =0;
	Relay->byRelRCounter = 0;	
}
/************************************************************
	���ܣ�ͨ�ù�������---���������ж�
	������Relay		--  ͨ�ù������������ṹ
		  dwK35		--  K35��������г������Ӧ��
		  RptSerialNum --  �������кţ�Ψһ�����ݿ���֯������
	���أ���
************************************************************/
void GeneralOverValueRelay_3(TRelGeneOvValPar_2 *Relay)
{	
	LONG ElecSet = *(Relay->pElec1Set);

	if(*(Relay->pRelSW) == SW_STATUS_CUT)   //ѹ��δͶ�룬ֱ�Ӹ�λ�˳� 
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay_3(Relay);   //����δͶ���Ҽ���δͶ����ٲ��������־������ 
		return;
	}
		

	if (*(Relay->pRelCFG) == CFG_STATUS_CLR)  //����δͶ�� 
	{
		if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord)))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay_3(Relay);   //����δͶ���Ҽ���δͶ����ٲ��������־������ 
			return;
		}		
	}
	if (*(Relay->pRelHSuprCFG) == CFG_STATUS_SET) //г������Ͷ�� 
	{
		//���ǵ���Ϊ0����� 
		if (Relay->pRelElec1->Mod != 0)
			ElecSet = (*Relay->dwK35)*ElecSet/(*Relay->pH2Coef);
	}

	if (ReadRelayWord(Relay->AccActRelayWord) || ReadRelayWord(Relay->ActRelayWord)) //���ٻ��߹��������Ѿ������� 
	{	
		//���������ķ��ط��������
		// 1. ������������բ�ɹ��������ʧ����������
		// 2. ��ѹ��������բ�ɹ����п�����Ȼ��ѹ����ʱ����������������
		if (((Relay->pRelElec1->Mod) < (ElecSet*NOCURCOEF/100))||(!ReadRelayWord(Relay->RSTRelayWord)))
		{		
			Relay->byRelRCounter++;
			if(Relay->byRelRCounter >3)
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetOverValueRelay_3(Relay);
			}
			return;
		}
		else
		{
			Relay->byRelRCounter = 0;
			return;	
		}
	}
	
	if (Relay->pRelElec1->Mod < ElecSet) //��ǰ������С�ڶ�ֵ 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //��������� 
		{
			if ((Relay->pRelElec1->Mod < ElecSet*99/100)&&(Relay->pRelElec2->Mod < *Relay->pElec2Set)) //�����Ѿ�����ҵ���С�ڷ��ض�ֵ 
			{
				Relay->byRelRCounter++;
				if(Relay->byRelRCounter>3)
				{
					WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
					ResetOverValueRelay_3(Relay);
					return;
				}
			}
			else
				Relay->byRelRCounter = 0;
		}
		else
		{
			if(Relay->pRelElec1->Mod >= *(Relay->pElec1Set))      //г������Ͷ�������£����������ڳ�ʼ��ֵʱ�������ܷ���
				return;
			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetOverValueRelay_3(Relay);
			return;		
		}
	}
	
	if (ReadRelayWord(Relay->LockRelayWord))
	{
		//��α����̵�������λ
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetOverValueRelay_3(Relay); 
		return;
	}	
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (*(Relay->pRelHLockCFG) == CFG_STATUS_SET)
	{
		if (ReadRelayWord(Relay->H2LockRelayWord))      //����г������Ͷ���ұ��������� 
		{
			ResetOverValueRelay_3(Relay);
			return;
		}
	}
	if (*(Relay->pRelAccCFG) == CFG_STATUS_CLR || (!ReadRelayWord(Relay->AccRelayWord))) //����δͶ����߼��ٲ��������й��ʷ����ж� 
	{
		if (*(Relay->pRelPDCFG) == CFG_STATUS_SET)   //�����������ʷ���Ͷ�� 
		{
			if(*(Relay->pRelFDCFG) == CFG_STATUS_SET)   //��������������Ͷ�� 
			{
				if(ReadRelayWord(Relay->PRewardRelayWord)&&(!ReadRelayWord(Relay->PForwardRelayWord)))  //������̵�������λ��������̵�����û����λ 
				{
					ResetOverValueRelay_3(Relay);
					return;
				}
			}
			else
			{
				if(ReadRelayWord(Relay->PForwardRelayWord)&&(!ReadRelayWord(Relay->PRewardRelayWord)))   //������̵�������λ��������̵�����û����λ 
				{
					ResetOverValueRelay_3(Relay);
					return;
				}				
			}
		}
	}
	
	if (*(Relay->pRelVolLockCFG) == CFG_STATUS_SET)
	{
		if ((Relay->pRelVolt->Mod) > (*(Relay->pVolLockSet)))     //��ѹ����Ͷ���ҵ�ѹ���ڶ�ֵ 
		{
			ResetOverValueRelay_3(Relay);
			return;
		}
	}
	if (*(Relay->pRel47UCFG) == CFG_STATUS_SET)   //��ѹ��������
	{
		if(ReadRelayWord(Relay->R47ULKRelayWord))
		{
			ResetOverValueRelay_3(Relay);
			return;
		}
	}
	if (!ReadRelayWord(Relay->StartRelayWord))   //����û����� 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>=2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			if((*(Relay->pRelAccCFG) == CFG_STATUS_SET)&&(ReadRelayWord(Relay->AccRelayWord))) //yanzh 2012-5-7 15:21:11
			{
				Relay->wPickUpType = ACCPICK;
				MakeActRpt(Relay->wAccPickUpRptNum,*Relay->RptSerialNum);
			}
			else
			{
				Relay->wPickUpType = NORMALPICK;				
				if(Relay->bMakeActRpt == TRUE)							//yanxs  2013-03-18   ������������
					MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
			}
			return;
		}
		else
			return;
	}  
	
	if ((*(Relay->pRelAccCFG) == CFG_STATUS_SET) && (Relay->wPickUpType == ACCPICK))
	{															   //����Ͷ���Ҽ��������ˣ����ٶ��� 	
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pAccActTimeSet)))
		{
			WriteRelayWord(Relay->AccActRelayWord,RELAY_ON);      //��ʱ�ѵ��������ٶ����̵�������λ 
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //��ʱ�ѵ���ͬʱ�������̵�������λ 
			MakeActRpt(Relay->wAccReportNum,*Relay->RptSerialNum);
			Relay->byRelRCounter = 0;
			return;
		}		
	}
	if(*(Relay->pRelCFG) == CFG_STATUS_SET)
	{
		if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*(Relay->pTimeSet)))
		{
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);      //��ʱ�ѵ������̵�������λ
			Relay->byRelRCounter = 0;
			if(Relay->bMakeActRpt == TRUE)
			{
				MakeActRpt(Relay->wReportNum,*Relay->RptSerialNum);
			}
			else
				MakeAlarmRpt(Relay->wReportNum);
		}
	}

}
/************************************************************
	���ܣ���λͨ�ù�������
	������Relay		--  ͨ�ù������������ṹ
	���أ���
************************************************************/
void ResetOverValueRelay_3(TRelGeneOvValPar_2 *Relay ,BOOL resetFlag)
{	
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //������� 
	{
		if(Relay->wPickUpType == ACCPICK)
			MakeActRpt(Relay->wAccResetRptNum, *Relay->RptSerialNum);	
		else
		{
		if(Relay->bMakeActRpt == TRUE)
			MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);
		}
	}
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);    
	WriteRelayWord(Relay->AccActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ActRelayWord,RELAY_OFF);
	Relay->dwRelTimer = 0;
	Relay->byRelFCounter =0;
	Relay->byRelRCounter = 0;	
}

/************************************************************
	���ܣ����ʷ����ж�
	������Uelec		--  ��ѹ
		  Ielec		--  ����	
		  PFRW		--  ����������̵�����
		  PRRW		--  ���ʷ�����̵�����
	���أ�FALSE		--  �������ʷ����ж�
		  TRUE		--  �����书�ʷ����ж�
************************************************************/
BOOL  PowerDirCheck(TRelElecValPar *Uelec,TRelElecValPar *Ielec, LONG *Databuf,LONG LineAngle,LONG UPCOEF,WORD PFRW,WORD PRRW,WORD channel,BOOL traceflag)
{
  	
	LONG temp1,temp2,temp3,temp4;
	LONG Angle;
	DWORD dwmask;
	TRelElecValPar tempU;
	BOOL	result;
	temp3=Uelec->Mod*Ielec->Mod/32;
	if(temp3 < 100)
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_OFF);
		return FALSE;
		
	}
	//�����书�ʷ����ж�
	if(traceflag)
	{
		if(Uelec->Mod<15*UPCOEF/100)
		{
			dwmask =0x01<<channel; 
			ReadSampleData(Databuf, dwmask, 4);
			CaluBaseFourier_S15(Databuf,&tempU);
			CaluModValue(&tempU);
			result = TRUE;
			if(tempU.Mod<15*UPCOEF/100)
			{			
				WriteRelayWord(PFRW, RELAY_OFF);
				WriteRelayWord(PRRW, RELAY_OFF);
				return result;
			}
		}		
		else
		{
			tempU = *Uelec;
			result =FALSE;
		}
	}
	else
	{
		tempU = *Uelec;
		result =FALSE;
	}
	
	temp1=((tempU.Real*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)+(tempU.Imag*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*CosAngleTab[LineAngle];
	temp2=((tempU.Imag*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)-(tempU.Real*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*SinAngleTab[LineAngle];
	temp4 = temp1+temp2;
	if((temp1 + temp2) > 0)   
	{
		WriteRelayWord(PFRW, RELAY_ON);
		WriteRelayWord(PRRW, RELAY_OFF);		
	}	
	else 
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_ON);
	}
	return result;
}

/************************************************************
	���ܣ�90����ߵĹ��ʷ����ж�
	������Uelec		--  ��ѹ
		  Ielec		--  ����	
		  PFRW		--  ����������̵�����
		  PRRW		--  ���ʷ�����̵�����
	���أ�FALSE		--  �������ʷ����ж�
		  TRUE		--  �����书�ʷ����ж�
************************************************************/
BOOL  PowerDirCheck_90(TRelElecValPar *Uelec,TRelElecValPar *Ielec, LONG *Databuf,LONG LineAngle,LONG UPCOEF,WORD PFRW,WORD PRRW,WORD channel,BOOL traceflag)
{
  	
	LONG temp1,temp2,temp3,temp4;
	LONG Angle;
	DWORD dwmask;
	TRelElecValPar tempU;
	BOOL	result;
	temp3=Uelec->Mod*Ielec->Mod/32;
	if(temp3 < 100)
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_OFF);
		return FALSE;
		
	}
	//�����书�ʷ����ж�
	if(traceflag)
	{
		if(Uelec->Mod<15*UPCOEF/100)
		{
			dwmask =0x01<<channel; 
			ReadSampleData(Databuf, dwmask, 4);
			CaluBaseFourier_S15(Databuf,&tempU);
			CaluModValue(&tempU);
			result = TRUE;
			if(tempU.Mod<15*UPCOEF/100)
			{			
				WriteRelayWord(PFRW, RELAY_OFF);
				WriteRelayWord(PRRW, RELAY_OFF);
				return result;
			}
		}		
		else
		{
			tempU = *Uelec;
			result =FALSE;
		}
	}
	else
	{
		tempU = *Uelec;
		result =FALSE;
	}
	temp1=((tempU.Real*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)+(tempU.Imag*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*CosAngleTab[LineAngle];
	temp2=((tempU.Imag*1000/tempU.Mod)*(Ielec->Real*1000/Ielec->Mod)-(tempU.Real*1000/tempU.Mod)*(Ielec->Imag*1000/Ielec->Mod))*SinAngleTab[LineAngle];
	temp4 = temp1+temp2;
	if((temp1 - temp2) > 0)   
	{
		WriteRelayWord(PFRW, RELAY_ON);
		WriteRelayWord(PRRW, RELAY_OFF);		
	}	
	else 
	{
		WriteRelayWord(PFRW, RELAY_OFF);
		WriteRelayWord(PRRW, RELAY_ON);		
	}
	return result;
}

/************************************************************
	���ܣ�ͨ��Ƿ������
	������Relay		--  ͨ��Ƿ�����������ṹ
		  RptNum		--  �������кţ�Ψһ�����ݿ���֯�����ã����ò������ں�����
		  			     ���޸ĸ�ֵ����ͨ�ù��������Ȳ�ͬ
	���أ���
************************************************************/
void GeneralUnderValRelay(TRelGeneUnValPar *Relay)
{
	if ((*(Relay->pRelCFG) == CFG_STATUS_CLR) || (*(Relay->pRelSW) == SW_STATUS_CUT))
	{
		ResetUnderValRelay(Relay);  //����δͶ�� 
		return;		
	}

	if (ReadRelayWord(Relay->ActRelayWord)||ReadRelayWord(Relay->AlarmRelayWord)) //�����Ѿ��������� 
	{
		if(ReadRelayWord(Relay->wR52BFRealyword))    //��·����բʧ��
		{
			if(!Relay->bHaveMake52BFRptFlag)
			{
				MakeActRpt(Relay->wTRFailRptNum,Relay->R27RptSerialNum);   //����·����բʧ�ܱ���
//				DB_RelayEnd(Relay->R27RptSerialNum);
				Relay->bHaveMake52BFRptFlag = TRUE;				
				ResetUnderValRelay(Relay);
			}
			return;	

		}
		if ((Relay->pRelElec->Mod > (*(Relay->pELeSet)*RECOVERVOLCOEF/100)) || \
			(!ReadRelayWord(Relay->RSTRelayWord)))//�������ػ��߱������ 
		{
//			DB_RelayEnd(Relay->R27RptSerialNum);		
			ResetUnderValRelay(Relay);
			return;	
		}
		else    //���������ˣ�����û�з��� 
			return;
	}

	if(ReadRelayWord(Relay->LockRelayWord))  //��α����̵�����Ч���ұ���λ 
	{
		ResetUnderValRelay(Relay);
		return;	
	}
	
	if (Relay->pRelElec->Mod > (*Relay->pELeSet))   //��ǰ�������ڶ�ֵ 
	{
		if (ReadRelayWord(Relay->StartRelayWord))  //������� 
		{
			if (Relay->pRelElec->Mod > (*Relay->pELeSet*101/100))  //���ڶ�ֵ��1.01�� 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetUnderValRelay(Relay); 				
//				DB_RelayEnd(Relay->R27RptSerialNum);
				return;
			}
		}
		else
		{         
			//�������ڶ�ֵ�ұ���û����Σ������־ 
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetUnderValRelay(Relay);   //�������ڶ�ֵ��1.01�� 
			return;
		}
	}
	if(*Relay->pRelCurLockCFG == CFG_STATUS_SET)
	{
		if(Relay->pRelLockElec->Mod > *Relay->pLockCurSet)
		{
			ResetUnderValRelay(Relay);  
			return;
		}
	}
		
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	
	if (!ReadRelayWord(Relay->StartRelayWord))  //������δ��� 
	{
		Relay->byRelFCounter++;
		if (Relay->byRelFCounter>2)   //���ϼ���������2������� 
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *(Relay->pSysTimer);
			ReadAbsTime(&Relay->R27UPickUpTime);
			DB_RelayStart(++(*Relay->RptSerialNum),ReportInf[Relay->wPickUpRptNum].wInf,&Relay->R27UPickUpTime,FALSE); //�����ݿ���д����������			
			Relay->R27RptSerialNum = *Relay->RptSerialNum;
//			MakeActRpt(Relay->wPickUpReportNum,Relay->R27RptSerialNum);
			return;
		}
		else
			return;
	}
	if ((*(Relay->pSysTimer) - Relay->dwRelTimer+INHERENTTIME) > (*Relay->pTimeSet) )  //ʱ�������� 
	{
		if (*(Relay->pRelAlarmCFG) == CFG_STATUS_SET)      //Ƿ������Ͷ��澯
		{			
			WriteRelayWord(Relay->AlarmRelayWord,RELAY_ON);	 //�澯 
			MakeAlarmRpt(Relay->wAlarmRptNum);
		}
		else
		{			
			WriteRelayWord(Relay->ActRelayWord,RELAY_ON);  //��բ 			
			MakeActRpt(Relay->wActRptNum,Relay->R27RptSerialNum);
		}
	}
}
/************************************************************
	���ܣ���λͨ��Ƿ������
	������Relay		--  ͨ��Ƿ�����������ṹ
	���أ��ޣ�����Ӧ�ļ̵�����
************************************************************/
void ResetUnderValRelay(TRelGeneUnValPar *Relay,BOOL resetFlag )
{
	Relay->byRelFCounter = 0;
	Relay->byRelRCounter = 0;
	Relay->dwRelTimer    = 0;
	Relay->bHaveMake52BFRptFlag = FALSE;
	if(ReadRelayWord(Relay->StartRelayWord))
	{	
//		MakeActRpt(Relay->wResetReportNum, Relay->R27RptSerialNum);	
		DB_RelayEnd(Relay->R27RptSerialNum);
	}
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->AlarmRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
}
/************************************************************
	���ܣ�ƽ���ı����ж�
	������Rel21		--  ͨ�þ��뱣�������ṹ
	���أ�TRUE		--  ��ǰ�迹��λ���ı�����
		  FALSE		--  ��ǰ�迹��λ���ı�����
************************************************************/
BOOL Parallelogram(TRel21Par *Relay)
{
	LONG RSet = *(Relay->pRel21RSet);
	LONG XSet = *(Relay->pRel21XSet);
	LONG RCurrent = Relay->pRel21Impd->Real;
	LONG XCurrent = Relay->pRel21Impd->Imag;
	//LONG K35;
	DWORD tempimpd;
	LONG Angle;

	if ((Relay->pRel21Impd->Imag == (INVALIDRX*(*Relay->pnZCoef))) || (Relay->pRel21Impd->Real == (INVALIDRX*(*Relay->pnZCoef))))
	{
		return FALSE;
	}
	if (*(Relay->pRel21DirCFG) == CFG_STATUS_CLR)   //��ǰͶ����Ƿ����� 
	{
		RCurrent = 0 - RCurrent;     
		XCurrent = 0 - XCurrent;
	}
	if (*(Relay->pRel21HSuprCFG) == CFG_STATUS_SET)  //г������Ͷ�� 
	{
		RSet = RSet*(*Relay->pH2Coef)/(*Relay->dwK35);
		XSet = XSet*(*Relay->pH2Coef)/(*Relay->dwK35);
	}
	//ƽ���ı����ϱ� 
	if (XCurrent> XSet)  
	{
		return FALSE;
	}
	//ƽ���ı����±� 
	if ((XCurrent + (*(Relay->pRel21ZOffset))) < 0)  
	{
		return FALSE;
	}
	 //ƽ���ı������ 
	 //��Һ������֤  *Relay->pRel21LineAngleSet-10 �Ǹ�
	if (XCurrent*TANGMULTIPLE > (RCurrent + (*(Relay->pRel21ZOffset)))*(Relay->pTangTabAddr[*Relay->pRel21LineAngleSet-10]))
	{  
		return FALSE;
	}
	//ƽ���ı����ұ� 
	if (XCurrent*TANGMULTIPLE < (RCurrent- RSet )*(Relay->pTangTabAddr[*Relay->pRel21LineAngleSet-10]))
	{	
		return FALSE;
	}
	
	return TRUE;

}
/************************************************************
	���ܣ��迹�ı����ж�
	������Rel21		--  ͨ�þ��뱣�������ṹ
	���أ�TRUE		--  ��ǰ�迹��λ���ı�����
		  FALSE		--  ��ǰ�迹��λ���ı�����
************************************************************/
BOOL ImpdQuadrangle(TRel21Par *Relay)
{
	LONG RSet = *(Relay->pRel21RSet);
	LONG XSet = *(Relay->pRel21XSet);
	LONG RCurrent = Relay->pRel21Impd->Real;
	LONG XCurrent = Relay->pRel21Impd->Imag;
	DWORD tempimpd;
	LONG	Angle;
	if ((Relay->pRel21Impd->Imag == (INVALIDRX*(*Relay->pnZCoef))) || (Relay->pRel21Impd->Real == (INVALIDRX*(*Relay->pnZCoef))))
	{
		return FALSE;
	}
	if (*(Relay->pRel21DirCFG) == CFG_STATUS_CLR)   //��ǰͶ����Ƿ����� 
	{
		RCurrent = 0 - RCurrent;
		XCurrent = 0 - XCurrent;
	}
	if (*(Relay->pRel21HSuprCFG) == CFG_STATUS_SET)  //г������Ͷ�� 
	{
		RSet = RSet*(*Relay->pH2Coef)/(*Relay->dwK35);
		XSet = XSet*(*Relay->pH2Coef)/(*Relay->dwK35);
	}
	
	//�ı����жϲ��֣� 
	//�迹�ı����ڵ�һ���������� 
	//�迹�ı����ϱ߽� 
	if(XCurrent > XSet)  
	{
		return FALSE;
	}
	if(RCurrent < 0)
	{
		return FALSE;
	}
	//�ı���15���±߽� 
	if (XCurrent*TANGMULTIPLE < (Relay->pTangTabAddr[*(Relay->pRel21AOffset)-10])*RCurrent*(-1))
	{
		return FALSE;
	}
	//�ı��δ�ֱR�Ჿ�ֱ߽� 
	if (XCurrent < 0)
	{
		if (RCurrent > RSet)
		{
			return FALSE;
		}
	}
	else
	{
		//�ı���85��߽� 
		if (XCurrent*TANGMULTIPLE >TANG85*RCurrent)
		{
			return FALSE;
		}
		
		//�ı�����·�迹�Ǳ߽� 
		if (XCurrent*TANGMULTIPLE < ((RCurrent - RSet)*(Relay->pTangTabAddr[*(Relay->pRel21LineAngleSet)-10])))
		{
			return FALSE;
		}	
	}
	return TRUE;
}

/************************************************************
	���ܣ�ͨ�þ��뱣��
	������Rel21		--  ͨ�þ��뱣�������ṹ
	���أ���
************************************************************/
void GeneralRelay21(TRel21Par *Relay)
{
	if(*(Relay->pRel21SW) == SW_STATUS_CUT)
	{		
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetRelay21(Relay);		   //���뱣��δͶ�롢�����δͶ���ߺ���ٲ������˳�
		return;
	}
	if (*(Relay->pRel21CFG) == CFG_STATUS_CLR)
	{
		if ((*(Relay->pRel21ACCCFG) == CFG_STATUS_CLR) || (!ReadRelayWord(Relay->ACCRelayWord)) )
		{
			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);           //���뱣��δͶ�롢�����δͶ���ߺ���ٲ������˳�
			return;
		}		
	}
	if(ReadRelayWord(Relay->ACTRelayWord)||ReadRelayWord(Relay->ACCActRelayWord))
	{
		if(ReadRelayWord(Relay->R37IRelayWord))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);	//�迹��λ���ı���֮��
			return;
		}
		else
		{
			return;
		}			
	}

	if (*(Relay->pRel21QuadTypeCFG) == CFG_STATUS_SET)
	{
		//����ƽ���ı����ж�
		if(!Parallelogram(Relay))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);	//�迹��λ���ı���֮��
			return;
		}
	}
	else
	{
		//�����迹�ı����ж� 
		if(!ImpdQuadrangle(Relay))
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);//�迹��λ���ı���֮�� 
			return;
		}
	}
	
	if(ReadRelayWord(Relay->LockRelayWord))
	{		
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetRelay21(Relay);//��α����̵����ֱ���λ������ 
		return;
	}
	
	if(*Relay->pRel21HLockCFG == CFG_STATUS_SET)	//����г������Ͷ��?
	{
		if(ReadRelayWord(Relay->H2LockRelayWord))  //����г�������̵�������λ
		{			
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetRelay21(Relay);            
			return;
		}
	}
	//���������߼����Ѿ��������ˣ���δ���� 
	if (ReadRelayWord(Relay->ACCActRelayWord) || ReadRelayWord(Relay->ACTRelayWord))
	{
		return;
	}
	
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);//�ù���ѭ�����Ƽ̵����� 
	if (!ReadRelayWord(Relay->StartRelayWord))   //����û����� 
	{
		Relay->byRel21FCounter++;
		if (Relay->byRel21FCounter>=2)
		{
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRel21Timer = *(Relay->pSysTimer);
			if((*(Relay->pRel21ACCCFG) == CFG_STATUS_SET) && (ReadRelayWord(Relay->ACCRelayWord))) //yanzh 2012-5-7 15:21:50
			{					
				MakeActRpt(Relay->wAccPickUpRptNum,*Relay->RptSerialNum);
				Relay->wPickUpType = ACCPICK;		
			}
			else
			{				
				MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
				Relay->wPickUpType = NORMALPICK;
			}
		}
		else
		{
			return;
		}
	}
	if ((*(Relay->pRel21ACCCFG) == CFG_STATUS_SET) && (Relay->wPickUpType == ACCPICK))
	{
	    //�����Ͷ������������ٶ��� 
		if ((*(Relay->pSysTimer) - Relay->dwRel21Timer+INHERENTTIME) > (*(Relay->pRel21AccTSet)))
		{
			WriteRelayWord(Relay->ACCActRelayWord,RELAY_ON);  //��ʱ�������ü��ٶ����̵����� 
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);      //ͬʱ�������̵�������λ 
			MakeActRpt(Relay->wRel21AccRptNum,*Relay->RptSerialNum);
			return;
		}
	}
	if(*(Relay->pRel21CFG) == CFG_STATUS_SET)
	{
		if ((*(Relay->pSysTimer) - Relay->dwRel21Timer+INHERENTTIME) > (*(Relay->pRel21TSet)))
		{		
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
			MakeActRpt(Relay->wRel21RptNum,*Relay->RptSerialNum);
		}
	}
}
/************************************************************
	���ܣ���λͨ�þ��뱣��Ԫ��
	������Rel21		--  ͨ�þ��뱣�������ṹ
	���أ���
************************************************************/
void ResetRelay21(TRel21Par *Relay,BOOL resetFlag)
{
	
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //������� 
	{
		if(Relay->wPickUpType == ACCPICK)
		{
			MakeActRpt(Relay->wAccResetRptNum, *Relay->RptSerialNum);	
		}
		else
		{
			MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
		}
	}
	WriteRelayWord(Relay->ACCActRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	Relay->dwRel21Timer = 0;
	Relay->byRel21FCounter = 0;
	Relay->byRel21RCounter = 0;

}
/************************************************************
	���ܣ�PT���߼��
	������Relay		--  PT����
	���أ���
************************************************************/
void CheckPTBrok_1U(TRelPTBrokPar *Relay)
{
	if ((*Relay->pRelPTBrokCFG) == CFG_STATUS_CLR)  //PT����δͶ�� 
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		if(ReadRelayWord(Relay->ActRelayWord))
		{
			MakeAlarmRpt(Relay->wRelResRptNum);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		}
		return;
	}	
	if(Relay->pRelU1->Mod > *Relay->pR74PTU1Set)   //PT���߻ָ�
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		if(ReadRelayWord(Relay->ActRelayWord))
		{
			MakeAlarmRpt(Relay->wRelResRptNum);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		}
		return;
	}
	if(Relay->pRelCur->Mod < *Relay->pR74PTISet)   //������ֱ���˳�
	{
		return;
	}
	
	if(Relay->pRelU1->Mod < *Relay->pR74PTU1Set)  //UС�ڶ�ֵ
	{
		if(!ReadRelayWord(Relay->ActRelayWord))		//��δ����
		{
			if(!ReadRelayWord(Relay->StartRelayWord))		//��δ���
			{
				Relay->dwRelTimer = *Relay->dwSysTimer;
				WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
			}
			else
			{
				if((*Relay->dwSysTimer - Relay->dwRelTimer) > *Relay->pR74PTTSet)
				{
					WriteRelayWord(Relay->ActRelayWord, RELAY_ON);
					MakeAlarmRpt(Relay->wRelRptNum);
				}
			}
		}		
	}
}
/************************************************************
	���ܣ�PT���߼��
	������Relay		--  PT����
	���أ���
************************************************************/
void CheckPTBrok_2U(TRelPTBrokPar *Relay)
{
	if ((*Relay->pRelPTBrokCFG) == CFG_STATUS_CLR)  //PT����δͶ�� 
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		if(ReadRelayWord(Relay->ActRelayWord))
		{
			MakeAlarmRpt(Relay->wRelResRptNum);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		}
		return;
	}
	if(ReadRelayWord(Relay->ActRelayWord)) //����Ѿ�����PT���ߣ�ֻ���ڵ�ѹ�ָ�������²��ܸ���PT����
	{
		if(Relay->pRelU1->Mod > *Relay->pR74PTU1Set)   //PT���߻ָ�
		{
			Relay->dwRelTimer = 0;
			WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
			WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);			
			MakeAlarmRpt(Relay->wRelResRptNum);
			return;
		}
		else
			return;
	}
	if(ReadRelayWord(Relay->LKPIRelayWord)) //��α����̵����ֱ���λ		
	{		
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		return;
	}
	if(Relay->pRelU1->Mod > *Relay->pR74PTU1Set)    //���жϵ�ѹ���ڶ�ֵ
	{
		Relay->dwRelTimer = 0;
		WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
		WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
		return;
	}
	if((Relay->pRelCur->Mod > *Relay->pR74PTISet)||(Relay->pRelU2->Mod > *Relay->pR74PTU2Set))
	{
		if(!ReadRelayWord(Relay->StartRelayWord))		//��δ���
		{
			Relay->dwRelTimer = *Relay->dwSysTimer;
			WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
		}
		else
		{
			if((*Relay->dwSysTimer - Relay->dwRelTimer) > *Relay->pR74PTTSet)
			{
				WriteRelayWord(Relay->ActRelayWord, RELAY_ON);
				MakeAlarmRpt(Relay->wRelRptNum);
			}
		}
	}
}

/************************************************************
	���ܣ�ʵ�ַ�ʱ�޹�������
	������Relay		--  ��ʱ�޹������������ṹ
	���أ���
************************************************************/
void InverseCurRelay(TRelInverseCurPar *Relay)
{
	if ((*Relay->pRelCFG == CFG_STATUS_CLR) || (*Relay->pRelSW == SW_STATUS_CUT)) //����δͶ�� 
	{
		ResetInverseCurRel(Relay);
		return;
	}
	if (ReadRelayWord(Relay->ACTRelayWord))   //�����Ѿ������� 
	{
		if(Relay->pRelElec->Mod<*Relay->pRelCurSet*NOCURCOEF/100)
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_OFF);
			ResetInverseCurRel(Relay);
			return;
		}
		else 
			return;
	}
	if (ReadRelayWord(Relay->StartRelayWord)) //�����Ѿ������ 
	{
		if ((Relay->pRelElec->Mod) < Relay->dwCaluCurSet)   						//��ʱ���Ѿ���Σ���ǰ����С�ڶ�ֵ 
		{
			Relay->byRelRCounter += RELAY_SAMP_INTERVAL; 											//���Ϸ��ؼ������ۼ� 
			if (Relay->byRelRCounter >= POINT_PER_PERIOD)	//һ�ܲ���ʼ��С�ڶ�ֵ������Ԫ����λ 
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_OFF);
				ResetInverseCurRel(Relay);
				return;
			}
			else
				return;
		}		
	}
	else
	{
		if (Relay->pRelElec->Mod < Relay->dwCaluCurSet)	//û����Σ�����С��1.1����ֵ��ֱ�ӷ��� 
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_OFF);
			ResetInverseCurRel(Relay);
			return;
		}
		else		//��������1.1����ֵ�����ұ���û�����
		{
			/*if (*Relay->RptPickupFlag ==FALSE)
			{
				*Relay->RptPickupFlag =TRUE;
				ReadAbsTime(&RelayPickUpTime);
				DB_RelayStart(*Relay->RptPickupSerialNum,ReportInf[Relay->wPickUpReportNum].wInf,&RelayPickUpTime,FALSE); //�����ݿ���д����������
				MakePickupRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);
			}
			else
				MakeActRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);*/
			MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON); //����μ̵����� 
			WriteRelayWord(Relay->LoopCtrlRelayWord, RELAY_ON);
		}		
	}	
	Relay->LimitCouter++;							//��ʱ�޼��޶���ʱ��������ۼ� 
	Relay->byRelRCounter = 0;
	
	if (Relay->LimitCouter > Relay->MaxCouter)		
	{
		WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);  //��ʱ�޶��� 
		MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
	}	

	InverseCurIntegral(Relay);						   //���÷�ʱ���ۼӳ��� 
	
	if (Relay->CurCounter > Relay->CurCounterSet)
	{
		WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);  //��ʱ�޶���
		MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
	}	
}
/************************************************************
	���ܣ���ʱ�޹������������ۼ�������
	������Relay		--  ��ʱ�޹������������ṹ
	���أ���
************************************************************/
void InverseCurIntegral(TRelInverseCurPar *Relay)
{
	BYTE Multiples;
	LONG coefA,coefB,coefC;
	if ((*Relay->pRelTypeSet > 3) || (*Relay->pRelTypeSet < 1) )
	{
		return;
	}
	if (*Relay->pRelTypeSet == 1)
	{
		Multiples	= Relay->pRelElec->Mod/(*(Relay->pRelCurSet));
				
		if (Multiples>=20)
			Multiples = 19;
		
		coefA = Relay->pCoefTabA[Multiples][0];
		coefB = Relay->pCoefTabA[Multiples][1];
		coefC = Relay->pCoefTabA[Multiples][2];		
	}
	else if (*Relay->pRelTypeSet == 2)
	{
		coefA= Relay->pCoefTabB[0];
		coefB = Relay->pCoefTabB[1];
		coefC = Relay->pCoefTabB[2];
	}
	else if (*Relay->pRelTypeSet == 3)
	{
		coefA = Relay->pCoefTabC[0];
		coefB = Relay->pCoefTabC[1];
		coefC = Relay->pCoefTabC[2];
	}
	
	Relay->CurItem = (LLONG)coefA*(*(Relay->pRelCurSet))*(*(Relay->pRelCurSet)) \
		+(LLONG)coefB*(*(Relay->pRelCurSet))*Relay->pRelElec->Mod \
			+(LLONG)coefC*Relay->pRelElec->Mod*Relay->pRelElec->Mod; 
	Relay->CurCounter += Relay->CurItem;
}
/************************************************************
	���ܣ���λ��ʱ�޹�������Ԫ��
	������Relay		--  ��ʱ�޹������������ṹ
	���أ���
************************************************************/
void ResetInverseCurRel(TRelInverseCurPar *Relay,BOOL resetFlag)
{
	//if (ReadRelayWord(Relay->StartRelayWord)&&(*Relay->RptPickupFlag == TRUE))   //������� 
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //������� 
	{
	//	MakeActRpt(Relay->wResetReportNum, *Relay->RptPickupSerialNum);	
		MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
	}
	Relay->byRelFCounter=0;
	Relay->byRelRCounter=0;
	Relay->LimitCouter =0;
	Relay->CurCounter =0;
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);

}
/************************************************************
	���ܣ�������������
	������Relay		--  �����������������ṹ
	���أ���
************************************************************/
void IncreCurrentRelay(TRel50DIPar *Relay)									
{
	LONG	IncreCur;
	if((*Relay->pRelCFG == CFG_STATUS_CLR) ||(*Relay->pRelSW ==SW_STATUS_CUT ))     //����δͶ��
	{	
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetR50DI(Relay);
		return;
	}
	if(ReadRelayWord(Relay->ACTRelayWord))				//�����Ѿ�������
	{
		if(Relay->pRelElec1->Mod < *Relay->pRelLockCurSet)	//��ǰ����С��С������������
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;			
		}
		else 
			return;
	}

	if(ReadRelayWord(Relay->StartRelayWord))		//�����Ѿ������
	{
		if(Relay->pRelElec1->Mod < *Relay->pRelLockCurSet)	//��ǰ����С��С������������
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;			
		}
		//��������ˣ���������������������
		if(*Relay->OtherRelTrip == TRUE)
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;
		}
		
		if(*Relay->pFaultSINum<SAMP_PER_SECOND) //��������Ѿ���Σ�����δ�ﵽ1s����ʱ����Ҫ�ж������������ڶ�ֵ����С���򷵻� yanzhh 2012��5��25��16:31:17
		{
			IncreCur = (Relay->pRelElec1->Mod - Relay->dwPreI1Mod)*(*Relay->pH2Coef) - (*Relay->pRelKhr)*(Relay->pRelElec3->Mod + Relay->pRelElec5->Mod \
				- Relay->dwPreI3Mod - Relay->dwPreI5Mod);
			if(IncreCur < *Relay->pRelCurSet*(*Relay->pH2Coef))
			{
				WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
				ResetR50DI(Relay);
				return;
			}
		}
		if(*(Relay->pSysTimer) -Relay->dwRelTimer+INHERENTTIME >(*Relay->pRelTimeSet))     //��ʱ������
		{
			WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
			MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
			return;			
		}
		else
			return;
	}
	if(*Relay->pFaultSINum<SAMP_PER_SECOND)   //ֻ��1s���ж����������ڸ�ʱ���ڣ���������С�ڶ�ֵ���򱣻�����
	{
		if(Relay->pRelElec1->Mod < *Relay->pRelLockCurSet)
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;			
		}
		if(*Relay->OtherRelTrip == TRUE)  //��������������?
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;
		}
		if(ReadRelayWord(Relay->H2LockRelayWord))   //����г������
		{
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
			ResetR50DI(Relay);
			return;
		}
		//������������ I=I1h-I1q-Khr*(I3h+I5h-I3q-I5q)
		IncreCur = (Relay->pRelElec1->Mod - Relay->dwPreI1Mod)*(*Relay->pH2Coef) - (*Relay->pRelKhr)*(Relay->pRelElec3->Mod + Relay->pRelElec5->Mod \
			- Relay->dwPreI3Mod - Relay->dwPreI5Mod);
		if(IncreCur > *Relay->pRelCurSet*(*Relay->pH2Coef))
		{	
			Relay->wRelFCounter++;
			if(Relay->wRelFCounter<3)
				return;
			if( ReadRelayWord(Relay->LockRelayWord))
			{
				ResetR50DI(Relay);
				return;
			}
			/*if (*Relay->RptPickupFlag ==FALSE)
			{
				*Relay->RptPickupFlag =TRUE;
				ReadAbsTime(&RelayPickUpTime);
				DB_RelayStart(*Relay->RptPickupSerialNum,ReportInf[Relay->wPickUpReportNum].wInf,&RelayPickUpTime,FALSE); //�����ݿ���д����������
				MakePickupRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);
			}
			else
				MakeActRpt(Relay->wPickUpReportNum,*Relay->RptPickupSerialNum);*/
			MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
			WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
			Relay->dwRelTimer = *Relay->pSysTimer;
			WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
		}
	}
	else   //���ϼ��ʱ�䳬����500ms�����ڽ����жϣ�ֱ�Ӹ�λ����Ԫ��
	{
		WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
		ResetR50DI(Relay);
		return;
	}
}
/************************************************************
	���ܣ������������ǰ�����������
	������Relay		--  �����������������ṹ
	���أ���
************************************************************/

void CalcInCreCurrent(TRel50DIPar *Relay,LONG *DataBuf,DWORD dwMask)	
{	
	if((*Relay->pRelCFG == CFG_STATUS_CLR) ||(*Relay->pRelSW ==SW_STATUS_CUT ))
		return;
	if(Relay->PreCurHaveCalcFlag)
		return;
	if(ReadRelayWord(Relay->R79ActRelayWord)) //�غ�բ�����̵�����
	{
		Relay->dwPreI1Mod = 0;
		Relay->dwPreI3Mod = 0;
		Relay->dwPreI5Mod = 0;
		Relay->PreCurHaveCalcFlag = TRUE;
	}
	else
	{
		TRelElecValPar I1p,I3p,I5p;
		
		ReadSampleData(DataBuf,dwMask,2);  //��ȡ����ǰ2�ܲ�����
#ifdef PRO_6571
		CaluBaseFourier_20(DataBuf,&I1p);  //6571װ��Ϊ20�����
		CaluThirdFourier_20(DataBuf,&I3p);
		CaluFifthFourier_20(DataBuf,&I5p);
#else
		CaluBaseFourier_S15(DataBuf,&I1p);
		CaluThirdFourier(DataBuf,&I3p);
		CaluFifthFourier(DataBuf,&I5p);
#endif	
		CaluModValue(&I1p);
		CaluModValue(&I3p);
		CaluModValue(&I5p);
	
		Relay->dwPreI1Mod = I1p.Mod;
		Relay->dwPreI3Mod = I3p.Mod;
		Relay->dwPreI5Mod = I5p.Mod;
		Relay->PreCurHaveCalcFlag = TRUE;	
	}
}
/************************************************************
	���ܣ���λ��������Ԫ��
	������Relay		--  �����������������ṹ
	���أ���
************************************************************/

void ResetR50DI(TRel50DIPar *Relay,BOOL resetFlag)
{
	//if (ReadRelayWord(Relay->StartRelayWord)&&(*Relay->RptPickupFlag == TRUE))   //������� 
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //������� 
	{
	//	MakeActRpt(Relay->wResetReportNum, *Relay->RptPickupSerialNum);	
		MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
	}
	Relay->PreCurHaveCalcFlag = FALSE;
	Relay->dwRelTimer =0;
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	
}
/************************************************************
	���ܣ��غ�բ��ŵ�Ԫ�� 
	������Relay		--  �غ�բ�����ṹ
	���أ���
************************************************************/
void AutoRCCharge(TRel79Par *Relay)
{
	
	if(ReadRelayWord(Relay->R79LOCKRW))   //�غ�բ������
		return;
	
	if(ReadRelayWord(Relay->R79RSSRW))                         //�غ�բ���ڸ���״̬
	{
		if(ReadRelayWord(Relay->R7952A)&&(!ReadRelayWord(Relay->R7952B)))				//��·�����ں�λ
		{
			WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79RSSRW,RELAY_ON);
			Relay->R79RSFLAG = FALSE;
			Relay->R79ShotCnt = 0;	
			
			if(Relay->R79ShotCnt == 0)		//��������Ϊ0����һ���غ�բ
			{
				WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
				WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
			}
			if(Relay->R79ShotCnt == 1)		//��������Ϊ1���Ƕ����غ�բ
			{		
				WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
				WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
			}		
			return;

		}
		else
		{
			if(Relay->R79RSFLAG)     //��ŵ���ʱ�Ѿ�����
			{
				if((*Relay->dwSysTimer - Relay->R79Timer) > *Relay->R79RSTSet)//�ŵ�ʱ���Ѿ�����
				{
					WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
					WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
					WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
					Relay->R79RSFLAG = FALSE;
					return;
				}
				else       //�ŵ�ʱ���в����㣬ֱ�ӷ���
					return;
			}
			else				//�ŵ���ʱ��δ����
			{
				Relay->R79RSFLAG = TRUE;		//�����ŵ���ʱ
				Relay->R79Timer = *(Relay->dwSysTimer);
				return;
			}
		}
	}
	if(ReadRelayWord(Relay->R79CYSRW))	//�غ�բ��������״̬
	{
		WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79CYSRW, RELAY_ON);
		Relay->R79RSFLAG = FALSE;
		
		if(Relay->R79ShotCnt == 0)		//��������Ϊ0����һ���غ�բ
		{
			WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
			WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
		}
		if(Relay->R79ShotCnt == 1)		//��������Ϊ1���Ƕ����غ�բ
		{		
			WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
			WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
		}		
		return;

	}
	if(!ReadRelayWord(Relay->R7952A))	//��·�����ڷ�λ
	{
		WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
		Relay->R79RSFLAG = FALSE;

		if(Relay->R79ShotCnt == 0)		//��������Ϊ0����һ���غ�բ
		{
			WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
			WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
		}
		if(Relay->R79ShotCnt == 1)		//��������Ϊ1���Ƕ����غ�բ
		{		
			WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
			WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
		}		
		return;

	}
	if(!Relay->R79RSFLAG)		//��·����λ�������ʱ��δ����
	{
		Relay->R79RSFLAG=TRUE;             //���������ʱ
		Relay->R79Timer = *(Relay->dwSysTimer);	//��ʼ�������ʱ
		return;
	}
	if((*Relay->dwSysTimer - Relay->R79Timer) > *Relay->R79RSTSet) //�����ʱ�Ѿ�����
	{
		WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW, RELAY_ON);
		Relay->R79RSFLAG = FALSE;
		Relay->R79ShotCnt  = 0;
		Relay->R79LKFlag 	 = FALSE;
	}
	if(Relay->R79ShotCnt == 0)		//��������Ϊ0����һ���غ�բ
	{
		WriteRelayWord(Relay->R79SH0RW, RELAY_ON);
		WriteRelayWord(Relay->R79SH1RW, RELAY_OFF);
	}
	if(Relay->R79ShotCnt == 1)		//��������Ϊ1���Ƕ����غ�բ
	{		
		WriteRelayWord(Relay->R79SH0RW, RELAY_OFF);
		WriteRelayWord(Relay->R79SH1RW, RELAY_ON);
	}		
}
/************************************************************
	���ܣ��غ�բ����Ԫ�� 
	������Relay		--  �غ�բ�����ṹ
	���أ���
************************************************************/	
void AutoRCAct(TRel79Par *Relay)
{	
	//LK�̵�����ֻ�ڸ���״̬���жϣ��غ�բ����������Ӽ̵������ܴ���غ�բ
	if((ReadRelayWord(Relay->R79RSSRW))&&(ReadRelayWord(Relay->R79LOCKRW)))
	{
		ResetAutoRC(Relay);		
		return;		
	}
	if(ReadRelayWord(Relay->R79TRCHECK)&&!ReadRelayWord(Relay->R79PIRW))
	{
		if(!(*Relay->pR79PIRptFlag))
		{			
			MakeActRpt(Relay->wRpt79NPI_1,*Relay->RptSerialNum);
			*Relay->pR79PIRptFlag = TRUE;
		}
	}

	if(Relay->R79TFLAG == FALSE)  //�غ�բδ������ʱ״̬
	{
		if(*Relay->R79SW==SW_STATUS_CUT) //ѹ��δͶ�룬ֱ�Ӹ�λ�˳�;�������ڱ����̵������С�
		{
			ResetAutoRC(Relay);
			return;
		}
	}
			
	AutoRCCharge(Relay);		//�����غ�բ����ӳ���

	if(ReadRelayWord(Relay->R79LOSRW))	//�غ�բ�ڱ���״̬
	{
		WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
		WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
		return;
	}
	if(ReadRelayWord(Relay->R79CLCRW))	//�غ�բ������̵�������λ
	{
		if(ReadRelayWord(Relay->R79CLRW))
		{
			if(Relay->R79ShotCnt == 1)				
				MakeActRpt(Relay->wRpt79_1_FAIL,*Relay->RptSerialNum);
			else
				MakeActRpt(Relay->wRpt79_2_FAIL,*Relay->RptSerialNum);
		}
		WriteRelayWord(Relay->R79CLRW,RELAY_OFF);//��λ�غ�բ�����̵�����
		WriteRelayWord(Relay->R79TORW, RELAY_OFF);
		Relay->R79TFLAG = FALSE;

		if(Relay->R79ShotCnt >= *Relay->R79NumSet)    //��ߴ��غ�բ�Ѿ�������
		{
			ResetAutoRC(Relay);
			return;
		}		
	}
	else    //�غ�բ������߼��̵�����û����λ
	{
//		if(ReadRelayWord(Relay->R79CLRW))//�غ�բ�Ѿ�������		
		if(Relay->R79CLFlag)//�����غ�բ������־���ñ�־�ڸ�λ�����к�79PI����ʱ���㡣by yanzh
		{
			if((*(Relay->dwSysTimer) - Relay->R79Timer) > (*Relay->R79SOTFTSet+1000))		//���ڹ�����ʱ������
			{
				if(ReadRelayWord(Relay->R7952A)&&!ReadRelayWord(Relay->R7952B))
				{					
					if(Relay->R79ShotCnt == 1)
						MakeActRpt(Relay->wRpt79_1_SUCC,*Relay->RptSerialNum);
					else
						MakeActRpt(Relay->wRpt79_2_SUCC,*Relay->RptSerialNum);
				}
				else
				{
					if(Relay->R79ShotCnt == 1)
						MakeActRpt(Relay->wRpt79_1_FAIL,*Relay->RptSerialNum);
					else
						MakeActRpt(Relay->wRpt79_2_FAIL,*Relay->RptSerialNum);
				}
				ResetAutoRC(Relay);
				return;
			}
			else			//���ڹ��ϼ����ʱδ���㣬ֱ�ӷ��أ��п��ܼ����������������������غ�բ
				return;
		}
		if(Relay->R79TFLAG) //�غ�բ�Ѿ�������ʱ״̬
		{
			if(!ReadRelayWord(Relay->R79TORW))	  //�غ�բ������ʱ��δ����
			{
				if(Relay->R79ShotCnt == 0)
				{
					if((*Relay->dwSysTimer - Relay->R79Timer+AUTORCTIME) < (*Relay->R79T1Set))  //������ʱ������ 766ms
						return;
				}
				if(Relay->R79ShotCnt == 1)
				{
					if((*Relay->dwSysTimer - Relay->R79Timer+AUTORCTIME) < (*Relay->R79T2Set)) //������ʱ������ 766ms
						return;
				}
			}
	
			if(!Relay->R79CLSFLAG) //�غ�բ����������ʱ��־��δ��λ
			{
				WriteRelayWord(Relay->R79TORW, RELAY_ON);   //����ʱ���̵����֣���ʼ���������Ӽ̵�����
				Relay->R79Timer = *Relay->dwSysTimer;	
				Relay->R79CLSFLAG = TRUE;
				return;
			}
			if(ReadRelayWord(Relay->R79CLSRW))  //�����������㣬���غ�բ�����̵�����
			{
				WriteRelayWord(Relay->R79CLRW, RELAY_ON);
				Relay->R79CLFlag = TRUE;
				ResetHoldRelayWord(Relay->R79PIRW);
				WriteRelayWord(Relay->R79TORW, RELAY_OFF);
				Relay->R79ShotCnt++;
				Relay->R79TFLAG = FALSE;
				Relay->R79CLSFLAG = FALSE;
				Relay->R79PISFLAG = FALSE;
				Relay->R79Timer = *(Relay->dwSysTimer);	

				if(*Relay->pFaultOnFlag == FALSE)    //�غ�բ͵��
				{
					TAbsTime 	PickUpTime;

					ReadAbsTime(&PickUpTime);
					DB_RelayStart(++(*Relay->SysRptSerialNum),RELAY_INF_NOUSED,&PickUpTime,TRUE); 
					*Relay->RptSerialNum = *Relay->SysRptSerialNum;

					if(ReadRelayWord(Relay->R79SH0RW)&&!ReadRelayWord(Relay->R79SH1RW))   //���غ�բ��������
						MakeActRpt(Relay->wRpt79_1,*Relay->RptSerialNum);
					if(!ReadRelayWord(Relay->R79SH0RW)&&ReadRelayWord(Relay->R79SH1RW))
						MakeActRpt(Relay->wRpt79_2,*Relay->RptSerialNum);
					
					DB_RelayEnd(*Relay->RptSerialNum);
					return;

				}
				else
				{
					if(ReadRelayWord(Relay->R79SH0RW)&&!ReadRelayWord(Relay->R79SH1RW))   //���غ�բ��������
						MakeActRpt(Relay->wRpt79_1,*Relay->RptSerialNum);
					if(!ReadRelayWord(Relay->R79SH0RW)&&ReadRelayWord(Relay->R79SH1RW))
						MakeActRpt(Relay->wRpt79_2,*Relay->RptSerialNum);
					
					return;
				}
			}
			else
			{
				if(*(Relay->dwSysTimer) - Relay->R79Timer>(*Relay->R79CLSTSet)) //�ڶ�ֵʱ���ڣ��غ�բ���������߼�һֱ�����㣬�������״̬
				{					
					if(Relay->R79ShotCnt == 0)
						MakeActRpt(Relay->wRpt79LK_1,*Relay->RptSerialNum);	
					if(Relay->R79ShotCnt == 1)						
						MakeActRpt(Relay->wRpt79LK_2,*Relay->RptSerialNum); 
					
					ResetAutoRC(Relay);//���غ�բ����������	
					return;
				}
				else
				{
					WriteRelayWord(Relay->R79TORW, RELAY_ON);   //�ڶ��������ڼ䣬һֱд��ʱ���̵�����
					return;
				}			
			}
		}			
	}
		
	if(!Relay->R79PISFLAG)//�غ�բ����������ʱ��δ����
	{
		if(ReadRelayWord(Relay->R79PIRW)) //�غ�բ�����߼��Ѿ�����
		{			
			Relay->R79PISFLAG = TRUE;
			Relay->R79Timer = *Relay->dwSysTimer;//��ʼ������������ʱ��ʱ��			
			Relay->R79CLFlag = FALSE;
		}
		else
			return;
	}
	if(!ReadRelayWord(Relay->R79PISRW)) //�غ�բ���������߼���δ����,�ü̵�����Ϊ�м�̵����֣����߼���������
	{
		if((*Relay->dwSysTimer - Relay->R79Timer) >*Relay->R79PISTSet) //��������ʱ�������㣬�����غ�բ�����������������������״̬
		{
			if(Relay->R79ShotCnt == 0)
				MakeActRpt(Relay->wRpt79NPI_1,*Relay->RptSerialNum);
			if(Relay->R79ShotCnt == 1)				
				MakeActRpt(Relay->wRpt79NPI_2,*Relay->RptSerialNum);
			ResetAutoRC(Relay);			
			return;
		}
	}
	else
	{
		if(!Relay->R79TFLAG)    //�غ�բ���������߼����㣬�غ�բ��δ��ʼ��ʱ����������״̬
		{			
			if(Relay->R79ShotCnt == 0)
			{
				MakeActRpt(Relay->wRpt79PI_1,*Relay->RptSerialNum); //���غ�բ��������
			}
			if(Relay->R79ShotCnt == 1)
			{
				MakeActRpt(Relay->wRpt79PI_2,*Relay->RptSerialNum); //���غ�բ��������
			}
			*Relay->pR79PIRptFlag = TRUE;
			Relay->R79TFLAG = TRUE;
			Relay->R79Timer = *Relay->dwSysTimer;
			WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79LOSRW,RELAY_OFF);
			WriteRelayWord(Relay->R79CYSRW, RELAY_ON);	
		}
	}
}

/************************************************************
	���ܣ��غ�բ��λ����
	������Relay		--  �غ�բ�����ṹ
	���أ���
************************************************************/
void ResetAutoRC(TRel79Par *Relay)
{
	WriteRelayWord(Relay->R79CLRW,RELAY_OFF);
	ResetHoldRelayWord(Relay->R79PIRW);
	WriteRelayWord(Relay->R79TORW,RELAY_OFF);
	Relay->R79ShotCnt = 0;
	Relay->R79PISFLAG = FALSE;
	Relay->R79TFLAG = FALSE;
	Relay->R79CLFlag = FALSE;		
	Relay->R79CLSFLAG = FALSE;
	WriteRelayWord(Relay->R79CYSRW,RELAY_OFF);
	WriteRelayWord(Relay->R79RSSRW,RELAY_OFF);
	WriteRelayWord(Relay->R79LOSRW, RELAY_ON);
}
/************************************************************
	���ܣ���ѹ���ȹ����ɱ�����ʼ�������ӳ���
	������Relay		--  �غ�բ�����ṹ
	���أ���
************************************************************/
void InitThermalOverLoad(TThermalOverLoad *Relay)
{
	LLONG kin=*Relay->pRatedInSet;
	
	Relay->dwKIn = kin*(*Relay->p49KSet)*1414*(*Relay->pICoef)/(1000*(*Relay->pIHACoef));
	//Relay->dwKIn = *Relay->pRatedInSet*(*Relay->p49KSet)*1414*(*Relay->pICoef)/(1000*(*Relay->pIHACoef));    //K�Ŵ�100����In�Ŵ���Coef��
																									//�˴�KINΪ����Чֵ������ֵ������ϵ��ΪA��ø���Ҷϵ������
																									//Ҫת�������ϵ��
	Relay->dw49Warn = *Relay->p49ARSet*10000;				//�澯ϵ����ֵ�Ŵ���100��
	Relay->dw49Warn_099 = Relay->dw49Warn*99/100;

	if(*Relay->p49TSet>100)
	{
		if(*Relay->p49TSet>1000)
		{			
			Relay->l49OT = *Relay->p49TSet;
			Relay->lExpT = 999833;
		}
		else
		{			
			Relay->l49OT = *Relay->p49TSet*2;
			Relay->lExpT = 999667;
		}
	}
	else
	{		
		Relay->l49OT = *Relay->p49TSet*6;
		Relay->lExpT = 999000;
	}


	Relay->dw49AThermaPreVal =0;
}
/************************************************************
	���ܣ���ѹ���ȹ����ɱ����ӳ���
	������TThermalOverLoad		--  �����ɱ��������ṹ
	���أ���
************************************************************/
void RelThermalOverLoad(TThermalOverLoad *Relay)
{
	LLONG	tempIk;
	tempIk = Relay->pCurrent*100000/Relay->dwKIn;
	tempIk = tempIk*tempIk;
	Relay->dw49AThermaVal = tempIk+(Relay->dw49AThermaPreVal-tempIk)*Relay->lExpT/1000000;
	Relay->dw49AThermaPreVal = Relay->dw49AThermaVal;
	if(Relay->dw49AThermaVal>Relay->dw49Warn)//���ۼ����Ѿ����ڸ澯��ֵ
	{
		if(!ReadRelayWord(Relay->R49WarnRW))
		{
			WriteRelayWord(Relay->R49WarnRW,RELAY_ON);			
		}
		
		if(Relay->dw49AThermaVal>1000000)
		{
			WriteRelayWord(Relay->R49ActRW,RELAY_ON);			
		}
	}
	else
	{
		if(Relay->dw49AThermaVal<Relay->dw49Warn_099)
		{
			WriteRelayWord(Relay->R49ActRW,RELAY_OFF);
			WriteRelayWord(Relay->R49WarnRW,RELAY_OFF);
		}
	}	
}

/************************************************************
	���ܣ��߼����̴���
	��������
	���أ���
************************************************************/
/*
void LogicProcess(void)
{	
	//�߼����̴���
	LONG nLineSys = 0, nLineUser = 0;

	if(byRelayWordValue[1] != 0x1)
	{
		byRelayWordValue[1] = 0x1;
		LogError("CRelayTask",FILE_LINE,"RelayWord True1 changed");
	}
	UpDateInputRelayWord();
	if((gPLC_SysPLCStatus == PLC_ERR_OK)||(gPLC_DefPLCStatus == PLC_ERR_OK))
		RunLogicParseModule(LOGIC_Sys, &nLineSys);
//	if(gPLC_UserPLCStatus == PLC_ERR_OK)
//		RunLogicParseModule(LOGIC_User, &nLineUser);
	
	UpDateLed();

	UpDateOutPut();
}
*/
/*********************************************************************************************
 *
 * ��������     ���ָ����ſ����Ƿ���ڶ���״̬
 *
 * ����˵��      - no	: ���� 	�������
 *				 
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL isBoBusy(BYTE no)
{
	BYTE RegStatus;

	//��鱾·�����Ƿ����ڳ��ڲ���,��������
	if((byRelayWordValue[RW_OUT1+no]&0xf0) == 0)
	{
		return FALSE;
/*		
		//����1�Ϳ���25Ϊң������(����)���ƣ�ֻ�б�Ŀ���û�в���ʱ�����ܽ��д�·�������Լ�
		if(no%24)
		{
			return FALSE;	//���������Լ��־
		}
		else	//��������ң������(����)����ͨ��
		{
			RegStatus = MCF_GPIO_PODR_TIMER;
			if(no/24)	//�ڶ�����ӦYK_EN3��YK_EN4
			{
				RegStatus &= (DTI1_YKEN3|DTI0_YKEN4);
				RegStatus ^= DTI1_YKEN3;
			}
			else
			{
				RegStatus &= (DTO1_YKEN1|DTO0_YKEN2);
				RegStatus ^= DTO1_YKEN1;
			}

			//��·ң�ر������ڱ���״̬�������Լ�
			if(RegStatus == 0)
				return FALSE;
		}
*/		
	}
	return TRUE;
}

/*********************************************************************************************
 *
 * �������� 	���������Լ죬����3�α��������һ·�����Լ죬ÿ�ζ����Ϊ�쳣���ж�Ϊ��ͨ������
 *
 * ����˵�� 	 ��
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/
void BoBreakCheck()
{
	DWORD address;
	BYTE status, mask;
	DWORD BaseAddr[2] = {DOA_CS_BASE+25, DOC_CS_BASE+25};
	DWORD flag = 1;
	static BYTE bBoBreakCheckNo = 0;	
	static BOOL bHaveBreakFlag = FALSE;
	static BYTE bBreakCheckCnt = 0;
	static BYTE bBreakErrCnt = 0;
	WORD wBoNo;
	if(bHaveBreakFlag == TRUE)//�Ѿ��п�������������ˣ������ٽ��м�飬���Ǹ�λװ��
		return;
	
	//����OutPutMap����Ҷ�Ӧ���������
	wBoNo = OutPutMap[bBoBreakCheckNo];
			
	//���������Լ�
	if((byRelayWordValue[RW_OUT1+bBoBreakCheckNo]&0x0f) == 0)	
	{
 		mask = (0x01<<(wBoNo%8));
		address = BaseAddr[wBoNo/24]+(wBoNo%24)/8;

		//��ȡ����״̬
		status = inportb(address);
		//������ڵ�ͨ״̬�Ļ����¶���״̬
		if((status&mask) == 0)
			status = inportb(address);
			
		flag <<= (wBoNo%24);
		if((status&mask) == 0) //��������
		{
			bBreakErrCnt++;
			//ReportMsg("bo%d break error cnt=%d.", wBoNo, bBreakErrCnt);
		}
		
		bBreakCheckCnt++;
		if(bBreakCheckCnt<BO_BREAK_CNT)
			return;
		
		//����BO_BREAK_CNT�μ���쳣�����ж�Ϊ��ͨ������
		if(bBreakErrCnt == BO_BREAK_CNT)
		{
			dwBoBreakStatus[wBoNo/24] |= flag;
			ReportMsg("the BO%d break check error.", wBoNo);
		}
		else
			dwBoBreakStatus[wBoNo/24] &= (~flag);			
		
		bBreakCheckCnt = bBreakErrCnt = 0;
		
		//ȫ��״̬�������򿪳������������������
		if((dwBoBreakStatus[0] == 0)&&(dwBoBreakStatus[1] == 0))
			DAE_ClearRunningFlags(SS_INSULATION_ERR);
		else
			DAE_SetRunningFlags(SS_INSULATION_ERR);
					
	}
		
	//ת��OutPutMap���һ·����,����OUTPUT_RELAY_WORD_NUM��ʵ�ʴ�С���
	bBoBreakCheckNo++;
	if(bBoBreakCheckNo == OUTPUT_RELAY_WORD_NUM)
	{
		if((dwBoBreakStatus[0]!=0)||(dwBoBreakStatus[1]!=0))
		{
			bHaveBreakFlag = TRUE;
			*RM_pnBOBRCH1 = dwBoBreakStatus[0];
			*RM_pnBOBRCH2 = dwBoBreakStatus[1];			
			DB_LogWriteRM(RM_BOBRCH1, *RM_pnBOBRCH1);
			DB_LogWriteRM(RM_BOBRCH2, *RM_pnBOBRCH2);
			MakeAlarmRpt(R_OEBR);			
			DAE_SetRunningFlags(SS_INSULATION_ERR);
		}
		else
		{
			if(bHaveBreakFlag)
			{
				bHaveBreakFlag = FALSE;
			}			
			DAE_ClearRunningFlags(SS_INSULATION_ERR);			
		}
	}
	bBoBreakCheckNo %= OUTPUT_RELAY_WORD_NUM;

}

/*********************************************************************************************
 *
 * �������� 	����ʧЧ�Լ죬ÿ�δ���һ·
 *
 * ����˵�� 	 ��
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/
void BoInvalidCheck()
{
	DWORD flag = 1;
	BYTE status, mask;
	DWORD IntLev;
	DWORD BaseAddr[2] = {DOA_CS_BASE, DOC_CS_BASE};
	DWORD dwOperAdd, dwCheckAdd;
	BYTE command, PortStatus1, PortStatus2, ByteOffset;
	static BYTE bBoInvalidCheckNo = 0;
	static BOOL bHaveInvalidFlag = FALSE;
//	static BOOL bLedIsON = FALSE;
	WORD wBoNo;

	if(bHaveInvalidFlag == TRUE)//�Ѿ������п���ʧЧ�Ĵ��ڣ������������
		return;
	//����OutPutMap�±���Ҷ�Ӧ�������������
	wBoNo = OutPutMap[bBoInvalidCheckNo];	

 
	if((byRelayWordValue[RW_OUT1+bBoInvalidCheckNo]&0x0f) == 0)		
	{
		
 		mask = (0x01<<(wBoNo%8));
		ByteOffset = (wBoNo%24)/8;
		
		dwCheckAdd = BaseAddr[wBoNo/24]+25+ByteOffset;  //��У��ַ
		dwOperAdd = BaseAddr[wBoNo/24]+wBoNo%24;		//������ַ
		
		//�жϱ���
		IntLev = OS_IntLock();
		//��������
		if(wBoNo%24 == 0)
		{
			if(wBoNo == 0)
			{
				MCF_GPIO_PODR_TIMER &= (~DTO1_YKEN1);	//YK_EN1=0
				MCF_GPIO_PODR_TIMER |= DTO0_YKEN2;		//YK_EN2=1
			}
			else
			{
				MCF_GPIO_PODR_TIMER &= (~DTI1_YKEN3);	//YK_EN3=0
				MCF_GPIO_PODR_TIMER |= DTI0_YKEN4;		//YK_EN4=1
			}
		}
		outportb(dwOperAdd,0x55);
		outportb(0xb0030050, 0xff);

//		if(status&mask)
			status = inportb(dwCheckAdd);
		//��������
		if(wBoNo%24 == 0)
		{
			if(wBoNo == 0)
			{
				MCF_GPIO_PODR_TIMER |= DTO1_YKEN1;			//YK_EN1=0
				MCF_GPIO_PODR_TIMER &= (~DTO0_YKEN2);		//YK_EN2=1
			}
			else
			{
				MCF_GPIO_PODR_TIMER |= DTI1_YKEN3;			//YK_EN3=0
				MCF_GPIO_PODR_TIMER &= (~DTI0_YKEN4);		//YK_EN4=1
			}
		}
		outportb(dwOperAdd,0xAA);
		OS_IntUnLock(IntLev);
		
		flag <<= (wBoNo%24);
		if(status&mask) //����ʧЧ(û�е�ͨ)
		{
			dwBoInvalidStatus[wBoNo/24] |= flag;
			ReportMsg("the BO%d invalid check error.", wBoNo);
		}
		else	//��������(��ͨ)
		{
			dwBoInvalidStatus[wBoNo/24] &= (~flag);
		}
	}
	
	//ת��һ·����������OUTPUT_RELAY_WORD_NUM��ʵ�ʶ�����
	bBoInvalidCheckNo++;
	if(bBoInvalidCheckNo == OUTPUT_RELAY_WORD_NUM)
	{
		if((dwBoInvalidStatus[0]!=0)||(dwBoInvalidStatus[1]!=0))  //�����һ�飬�п���ʧЧ��������澯����
		{
			bHaveInvalidFlag = TRUE;
			*RM_pnBOINCH1 = dwBoInvalidStatus[0];
			*RM_pnBOINCH2 = dwBoInvalidStatus[1];			
			DB_LogWriteRM(RM_BOINCH1, *RM_pnBOINCH1);
			DB_LogWriteRM(RM_BOINCH2, *RM_pnBOINCH2);
			DAE_SetRunningFlags(SS_INSULATION_INV);
			MakeAlarmRpt(R_OEERR);			
		}
		else
		{
			if(bHaveInvalidFlag)
			{
				bHaveInvalidFlag = FALSE;				
				DAE_ClearRunningFlags(SS_INSULATION_INV);
			}
			*RM_pnBOINCH1 = dwBoInvalidStatus[0];
			*RM_pnBOINCH2 = dwBoInvalidStatus[1];
		}
	}
	bBoInvalidCheckNo %= OUTPUT_RELAY_WORD_NUM;
}

/*********************************************************************************************
 *
 * �������� 	�ṹ���ۼ�У��ͼ���
 *
 * ����˵�� 	 ��
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/

void StructACCCheck(TStructCheck *structcheck,WORD StructNum)
{
	WORD acc = 0x55aa;
	static WORD checknum = 0;
	for(LONG i=0;i<structcheck[checknum].bigsize;i++)
	{
		acc += structcheck[checknum].pstruct[i];
	}
	if(acc!=structcheck[checknum].acc)
	{
		if(!InitAllRelayMem())
		{
			RelayCheckErr = TRUE;
			MakeAlarmRpt(R_RCERR);
		}
		acc = 0x55aa;
		for(LONG i=0;i<structcheck[checknum].bigsize;i++)
		{
			acc += structcheck[checknum].pstruct[i];
		}
		structcheck[checknum].acc = acc;
	}
	if(StructNum == 0)
	{
		ReportMsg("The relay check vlaue error.StructNum=%d",StructNum);
		return;
	}
	checknum++;	
	checknum %= StructNum;
	
}
/*********************************************************************************************
 *
 * �������� 	������ֵ��ѹ�塢���á�������ֵ��ϵ��������Լ�
 *
 * ����˵�� 	 ��
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/

void RelayCheck()
{
	WORD start = SW_BUF_ADDR;
	WORD end   = CF_BUF_ADDR+CF_BUF_SIZE;
	WORD acc   = 0x55aa;
	WORD *p    = (WORD*)SW_BUF_ADDR;
	for(LONG i=0;i<(end-start)/2;i++)
	{
		acc += *p;
		p++;
	}
	if(acc!=RelayCheckAcc)
	{
		
		ReportMsg("The relay check vlaue error.now acc=%d,RelayCheckAcc = %d",acc,RelayCheckAcc);
		if(!InitAllRelayMem())
		{
			RelayCheckErr = TRUE;
			MakeAlarmRpt(R_RCERR);
			ReportMsg("The relay check value error,and InitAllMem Err!!!!.");
		}
		else
			RelayCheckAcc = acc;
	}
}
/*********************************************************************************************
 *
 * �������� 	���ʲ����Ԫ��
 *
 * ����˵�� 	 ���ʲ���������ṹ
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/
void Rel87RI(TRel87R *Relay)
{
	LONG R87RISet; //�����������ֵ
	if((*Relay->p87RCFG !=CFG_STATUS_SET )||(*Relay->p87RSW != SW_STATUS_SET))  //����δͶ��
	{
		Reset87RI(Relay);
		return;
	}
	if(ReadRelayWord(Relay->ACTRelayWord))		//�����Ѿ�����
	{
		if(Relay->pId->Mod < *Relay->p87RISet*NOCURCOEF/100)	//��ǰ����С�ڶ����ż�ֵ
		{
			Reset87RI(Relay);
			return;
		}
		else
			return;
	}
	if(Relay->pId->Mod > *Relay->p87RPI)  	//��ǰ��������г�����ƺ���ż�ֵ
	{
		/*if((*Relay->p68IdH2MCFG == CFG_STATUS_SET)||(*Relay->p68IdH2CCFG == CFG_STATUS_SET)||(*Relay->p68IdH2ICFG == CFG_STATUS_SET)|| \
			(*Relay->p68IrH2MCFG == CFG_STATUS_SET)||(*Relay->p68IdH5CFG == CFG_STATUS_SET)||(*Relay->p68IdHMulCFG == CFG_STATUS_SET))
		{
			if((*Relay->pIdH2Flag == TRUE)||(*Relay->pIdH5Flag == TRUE)||(*Relay->pIrH2Flag == TRUE)||(*Relay->pIdHMulFlag == TRUE))	//����������λ������
			{
				Reset87RI(Relay);
				return;
			}
		}*/
		if((*Relay->p68IdH2MCFG == CFG_STATUS_SET)||(*Relay->p68IdH2CCFG == CFG_STATUS_SET)||(*Relay->p68IdH2ICFG == CFG_STATUS_SET))
		{
			if(*Relay->pIdH2Flag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
		if(*Relay->p68IrH2MCFG == CFG_STATUS_SET)
		{
			if(*Relay->pIrH2Flag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
		if(*Relay->p68IdH5CFG == CFG_STATUS_SET)
		{
			if(*Relay->pIdH5Flag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
		if(*Relay->p68IdHMulCFG == CFG_STATUS_SET)
		{
			if(*Relay->pIdHMulFlag == TRUE)
			{
				Reset87RI(Relay);
				return;
			}
		}
	}
	else
	{
		Reset87RI(Relay);
		return;
	}
	if(Relay->pIr->Mod <= *Relay->pIRS1Set)  //�ƶ�����λ�ڵ�һ���ڣ�������ֵΪ�ż�ֵ
	{
		R87RISet = *Relay->p87RPI*100;
	}
	else if((Relay->pIr->Mod > *Relay->pIRS1Set)&&(Relay->pIr->Mod <= *Relay->pIRS2Set)) //�ƶ������ڵڶ�����
	{
		R87RISet = *Relay->pSLP1Set*(Relay->pIr->Mod - *Relay->pIRS1Set) + *Relay->p87RPI*100;
	}
	else if(Relay->pIr->Mod > *Relay->pIRS2Set)	//�ƶ������ڵ�������
	{
		LONG tId;
		tId = *Relay->pSLP1Set*(*Relay->pIRS2Set - *Relay->pIRS1Set) + *Relay->p87RPI*100;
		R87RISet = *Relay->pSLP2Set*(Relay->pIr->Mod - *Relay->pIRS2Set) + tId;
	}
	if(Relay->pId->Mod*100 < R87RISet)//��ǰ�����С�ڶ���ֵ
	{		
		Reset87RI(Relay);
		return;
	}
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_ON);
	if(!ReadRelayWord(Relay->StartRelayWord)) //��δ���
	{
		MakeActRpt(Relay->wPickUpReportNum,*Relay->RptSerialNum);
		WriteRelayWord(Relay->StartRelayWord,RELAY_ON);
		Relay->wRelFCounter++;
		return;
	}
	Relay->wRelFCounter++;
	if(Relay->wRelFCounter >=2)	//����
	{
		WriteRelayWord(Relay->ACTRelayWord,RELAY_ON);
		MakeActRpt(Relay->wRelRptNum,*Relay->RptSerialNum);
	}
		

}
/*********************************************************************************************
 *
 * �������� 	��λ���ʲ����Ԫ��
 *
 * ����˵�� 	 ���ʲ���������ṹ
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/
void Reset87RI(TRel87R *Relay,BOOL resetFlag)
{	
	if (ReadRelayWord(Relay->StartRelayWord)&&(!resetFlag))	 //������� 
	{
		MakeActRpt(Relay->wResetReportNum, *Relay->RptSerialNum);	
	}
	WriteRelayWord(Relay->ACTRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord,RELAY_OFF);
	WriteRelayWord(Relay->LoopCtrlRelayWord,RELAY_OFF);
	Relay->wRelFCounter = 0;
	Relay->wRelRCounter = 0;
	Relay->dwRelTimer	= 0;
}
/*********************************************************************************************
 *
 * �������� 	����Խ�޸澯����
 *
 * ����˵�� 	 ����Խ�޸澯Ԫ��
 *				 
 *
 * ���ش���
 *				  ��
 *
 * ����˵����		   
 *
**********************************************************************************************/
void R87IAlarm(T87IAlarm *Relay)
{
	if(*Relay->p87R_A_CFG==CFG_STATUS_SET)
	{
		if(Relay->pIDA->Mod > *Relay->p87RIASet*(*Relay->p87RIKSet)/100)
			Relay->ACnt++;
		else
		{		
			Relay->ACnt = 0;
			Relay->AAlarmFlag = FALSE;
		}
		if(Relay->ACnt > Relay->AlarmCnt)
		{
			Relay->AAlarmFlag = TRUE;
		}
	}
	if(*Relay->p87R_B_CFG==CFG_STATUS_SET)
	{
		if(Relay->pIDB->Mod > *Relay->p87RIBSet*(*Relay->p87RIKSet)/100)
			Relay->BCnt++;
		else
		{
			Relay->BCnt = 0;
			Relay->BAlarmFlag = FALSE;
		}
		if(Relay->BCnt > Relay->AlarmCnt)
		{
			Relay->BAlarmFlag = TRUE;
		}
	}
	if(*Relay->p87R_C_CFG==CFG_STATUS_SET)
	{
		if(Relay->pIDC->Mod > *Relay->p87RICSet*(*Relay->p87RIKSet)/100)
			Relay->CCnt++;
		else
		{
			Relay->CCnt = 0;
			Relay->CAlarmFlag = FALSE;
		}
		if(Relay->CCnt > Relay->AlarmCnt)
		{
			Relay->CAlarmFlag = TRUE;
		}
	}
	
	if((Relay->CAlarmFlag == TRUE)||(Relay->BAlarmFlag == TRUE)||(Relay->AAlarmFlag == TRUE))
	{
		WriteRelayWord(Relay->ARRW,RELAY_ON);
		if(Relay->MakeRptFlag == FALSE)
		{
			MakeAlarmRpt(Relay->wRelRptNum);
			Relay->MakeRptFlag = TRUE;			
		}
	}
	else
	{
		WriteRelayWord(Relay->ARRW,RELAY_OFF);
		Relay->MakeRptFlag = FALSE;		
	}
}
/*********************************************************************************************
 *
 * ��������     ��׽һ�ܲ����˹���㸽���ĸ����������㣬�����������ն��β�ֵ�㷨�ֱ����
                �����������ıƽ�ֵ��������ʱ�����㱻�������Ƶ��
                �ú�������һ��Ƶ��ֵ
 *				
 * ����˵��    - Fpre :      ���� ǰ�β���Ƶ��
			   - TestElec :  ���� �������
			   - TFMask:     ���� ��Ƶ������������		 
			   - dwTestFBuf: ���� ��Ƶ������
			   - Fnow:       ��� ���β�Ƶֵ
			   - ts:         ��� ���β�������

 * ���ش���      ��
**********************************************************************************************/

BOOL TestFrequence(DWORD Fpre,LONG TestElec,DWORD TFMask,SHORT *wTestFBuf,LONG *dwCaluBuf,LONG *Fnow,BOOL trace)
{
	LLONG x0=0,x1=0,x2=0;
	LLONG y0=0,y1=0,y2=0;
	LLONG yb = 0;
	LLONG Fm = 0;
	LLONG temp1,temp2,temp3;
	LONG num = 0;
	DWORD wYval[7];
	DWORD temp;
	WORD wCounter=0;
	DWORD	dwDataPtr = dwADDataWritePtr;

	
	dwDataPtr = BackSamplePtr(dwDataPtr,POINT_PER_PERIOD*FREQBUFSIZE);
	ReadHisSampData(dwDataPtr,TFMask,wTestFBuf,POINT_PER_PERIOD*FREQBUFSIZE);//��ȡ9���ܲ��Ĳ�����

	//�˲����˵��ߴ�г��
	for(LONG i =POINT_PER_PERIOD*FREQBUFSIZE-1;i>16;i-- )
	{
		dwCaluBuf[i]=wTestFBuf[i]-wTestFBuf[i-16];
	}
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-1;i>17;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-17];
	}
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-1;i>18;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-18];
	}
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-1;i>17;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-17];
	}
	
	//ǰ��Ƶ�ʲ��ں���Ƶ�ʱ仯��Χ֮�ڣ�ǰ��Ƶ��ȡ50Hz
	if(Fpre>MAX_FREQUENCE||Fpre<MIN_FREQUENCE) 
	{
		Fpre = 50000;	
//		ReportMsg("error,force freq= 5000");		
	}
	//��ǰ��ѹֵ��С��������Ƶ��ֵ=0������
	if(ToAbs(TestElec)<MIN_TESTU)
	{
	   return FALSE;
	}
	//��׽����㲢��ֵ�����������ֵ
	for(LONG i=POINT_PER_PERIOD*FREQBUFSIZE-2;i>2;i--)
	{		
		//if(((LLONG)dwCaluBuf[i]*dwCaluBuf[i-1]<=0)&&((LLONG)dwCaluBuf[i+1]*dwCaluBuf[i-2]<=0)&&(dwCaluBuf[i]!=0))
		if((((dwCaluBuf[i]<=0)&&(dwCaluBuf[i-1]>=0))||((dwCaluBuf[i]>=0)&&(dwCaluBuf[i-1]<=0)))&&
			(((dwCaluBuf[i+1]<=0)&&(dwCaluBuf[i-2]>=0))||((dwCaluBuf[i+1]>=0)&&(dwCaluBuf[i-2]<=0)))&&
			(dwCaluBuf[i]!=0))
		{
			num++;
			if(num%2)
			{
				x0 = (LONG)dwCaluBuf[i-2];
				y0 = (i-2)*1000;
				x1 = (LONG)dwCaluBuf[i-1];
				y1 = (i-1)*1000;
				x2 = (LONG)dwCaluBuf[i];
				y2 = i*1000;

				temp1 = y0*x1*x2;
				temp2 = (x0-x1)*(x0-x2);
				yb += temp1/temp2;
				temp1 = y1*x0*x2;
				temp2 = (x1-x0)*(x1-x2);
				yb += temp1/temp2;
				temp1 = y2*x0*x1;
				temp2 = (x2-x0)*(x2-x1);
				yb += temp1/temp2;
				
				wYval[wCounter]= yb;
				yb = 0;
				wCounter++;
				if(wCounter == (NUM_FREQ_PER_AITASK+2))
					break;				
			}	
		}
		if(i==3)
		{
	//		ReportMsg("Error: cannot find 7 zero value");
			return FALSE;
		}
	}

	if(wCounter == (NUM_FREQ_PER_AITASK+2))
	{
		for(LONG j=0;j<NUM_FREQ_PER_AITASK;j++)
		{
			if(!trace)
			{
				Fm=1000000000000/(625*(wYval[j]-wYval[j+1]));
				Fnow[j]=(LONG)Fm;	
			}
			else
			{
				temp = wYval[j]-wYval[j+1];
				temp3 = Fpre;
				Fnow[j] = (temp3*10000*POINT_PER_PERIOD+(temp/2))/temp;  //Ƶ�ʸ��٣��̶��������� 
			}
			
		}
	}
	return TRUE;

}
/*********************************************************************************************
 *
 * ��������     ��׽һ�ܲ����˹���㸽���ĸ����������㣬�����������ն��β�ֵ�㷨�ֱ����
                �����������ıƽ�ֵ��������ʱ�����㱻�������Ƶ��
                �ú������㵱ǰ���һ�ܲ��������Ƶ��
 *				
 * ����˵��    - Fpre :      ���� ǰ�β���Ƶ��
			   - TestElec :  ���� �������
			   - TFMask:     ���� ��Ƶ������������		 
			   - dwTestFBuf: ���� ��Ƶ������
			   - Fnow:       ��� ���β�Ƶֵ
			   - ts:         ��� ���β�������

 * ���ش���      ��
**********************************************************************************************/

BOOL CalcImmeFrequence(DWORD ADDataWritePtr,LONG TestElec,DWORD TFMask,SHORT *wTestFBuf,LONG *dwCaluBuf,LONG *Fnow)
{
	LLONG x0=0,x1=0,x2=0;
	LLONG y0=0,y1=0,y2=0;	
	LLONG temp1,temp2,temp3;
	LLONG yb = 0;
	LLONG Fm = 0;
	LONG num = 0;
	DWORD wYval[7];
	WORD wCounter=0;
	DWORD	dwDataPtr = ADDataWritePtr;
	
	dwDataPtr = BackSamplePtr(dwDataPtr,FREQUENCE_SAMP_NUM);
	
	
	//ReadHisSampData(dwDataPtr,TFMask,wTestFBuf,FREQUENCE_SAMP_NUM);
	ReadHisSampData_Single(dwDataPtr,TFMask,wTestFBuf,FREQUENCE_SAMP_NUM);

	

	//�˲����˵��ߴ�г��
	for(LONG i =FREQUENCE_SAMP_NUM-1;i>16;i-- )
	{
		dwCaluBuf[i]=wTestFBuf[i]-wTestFBuf[i-16];
	}
	for(LONG i=FREQUENCE_SAMP_NUM-1;i>17;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-17];
	}
	for(LONG i=FREQUENCE_SAMP_NUM-1;i>18;i--)
	{
		dwCaluBuf[i]=dwCaluBuf[i]-dwCaluBuf[i-18];
	}
	//��ǰ��ѹֵ��С��������Ƶ��ֵ=0������
	if(ToAbs(TestElec)<MIN_TESTU)
	{
		*Fnow= 0;
	   return FALSE;
	}
	//��׽����㲢��ֵ�����������ֵ
	for(LONG i=FREQUENCE_SAMP_NUM-RELAY_SAMP_INTERVAL-2;i>2;i--)
	{	
		if(((dwCaluBuf[i]>0)&&(dwCaluBuf[i-1]<0)&&(dwCaluBuf[i+1]>0)&&(dwCaluBuf[i-2]<0))||
			((dwCaluBuf[i]<0)&&(dwCaluBuf[i-1]>0)&&(dwCaluBuf[i+1]<0)&&(dwCaluBuf[i-2]>0)))
		{
			num++;
			if(num%2)
			{
				x0 = (LONG)dwCaluBuf[i-2];
				y0 = (i-2)*1000;
				x1 = (LONG)dwCaluBuf[i-1];
				y1 = (i-1)*1000;
				x2 = (LONG)dwCaluBuf[i];
				y2 = i*1000;

				temp1 = y0*x1*x2;
				temp2 = (x0-x1)*(x0-x2);
				yb += temp1/temp2;
				temp1 = y1*x0*x2;
				temp2 = (x1-x0)*(x1-x2);
				yb += temp1/temp2;
				temp1 = y2*x0*x1;
				temp2 = (x2-x0)*(x2-x1);
				yb += temp1/temp2;
				
				wYval[wCounter]= yb;
				yb = 0;
				wCounter++;
				if(wCounter == 2)
					break;				
			}	
		}
	
		if(i==3)
		{
//			ReportMsg("Error: cannot find 7 zero value");
			return FALSE;
		}
	}



	if(wCounter == 2)
	{
		Fm=1000000000000/(625*(wYval[0]-wYval[1]));
		*Fnow=(LONG)Fm;	 //�Ŵ���1000��
//		if((*Fnow>50010)||(*Fnow<49990))
//			ReportMsg("Error: 11111");
	}
	
	return TRUE;
}

/*
void WriteDebugRW(WORD wIndex,BYTE byVal)
{
	WORD wDebugIndex;

	if(wIndex < VIRTUAL_RELAY_WORD_START)
		return;

	wDebugIndex = wIndex - VIRTUAL_RELAY_WORD_START;

	if(wDebugIndex >= VIRTURAL_RELAY_WORD_NUM)
		return;
	
	byDebugRelayBuf[wDebugIndex] = byVal;
}
BYTE ReadRWFullValue(WORD wIndex)
{
	if(wIndex >= RELAY_WORD_NUM)
		return BYTE(-1);
	return byRelayWordValue[wIndex];
}
*/
BOOL Relay_GetBOINStatus(BYTE *pStatus)
{
	if(pStatus == NULL)
		return FALSE;

	*pStatus++ = LLBYTE(dwBoInvalidStatus[0]);
	*pStatus++ = LHBYTE(dwBoInvalidStatus[0]);
	*pStatus++ = HLBYTE(dwBoInvalidStatus[0]);
	*pStatus++ = HHBYTE(dwBoInvalidStatus[0]);
	
	*pStatus++ = LLBYTE(dwBoInvalidStatus[1]);
	*pStatus++ = LHBYTE(dwBoInvalidStatus[1]);
	*pStatus++ = HLBYTE(dwBoInvalidStatus[1]);
	*pStatus++ = HHBYTE(dwBoInvalidStatus[1]);
	
	return TRUE;
}

BOOL Relay_GetBOBRStatus(BYTE *pStatus)
{
	if(pStatus == NULL)
		return FALSE;

	*pStatus++ = LLBYTE(dwBoBreakStatus[0]);
	*pStatus++ = LHBYTE(dwBoBreakStatus[0]);
	*pStatus++ = HLBYTE(dwBoBreakStatus[0]);
	*pStatus++ = HHBYTE(dwBoBreakStatus[0]);
	
	*pStatus++ = LLBYTE(dwBoBreakStatus[1]);
	*pStatus++ = LHBYTE(dwBoBreakStatus[1]);
	*pStatus++ = HLBYTE(dwBoBreakStatus[1]);
	*pStatus++ = HHBYTE(dwBoBreakStatus[1]);
	
	return TRUE;
}
BOOL GetMaintSWStatus()
{
	if(*g_MaintSW == SW_STATUS_CUT)
		return FALSE;
	else
		return TRUE;
}

/************************************************************************************************
*����: ģ��ָ����ŵı���
*����: �������
*����: ��
*��ע: 
*************************************************************************************************/
void SimActionReport(WORD no)
{
	// ��дһ����Ч����
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(0x55AA,RELAY_INF_NOUSED,&AbsTime,TRUE);
	// ��дһ����������
	Report_Action(0x55AA,no);
	// ����һ������
	Report_End(0x55AA,no);
}
/************************************************************************************************
*����: ����Ҷ�����������������������20������Ĳ��װ��
*����: �������
*����: ��
*��ע: 
*************************************************************************************************/
void CaluBaseFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
		{
			dwRealAmplitude += 14629*(-pDataBuf[0]+pDataBuf[20]);
			dwRealAmplitude += 1432*(pDataBuf[1]+pDataBuf[9]-pDataBuf[11]-pDataBuf[19]);
			dwRealAmplitude += 2724*(pDataBuf[2]+pDataBuf[8]-pDataBuf[12]-pDataBuf[18]);
			dwRealAmplitude += 3749*(pDataBuf[3]+pDataBuf[7]-pDataBuf[13]-pDataBuf[17]);
			dwRealAmplitude += 4407*(pDataBuf[4]+pDataBuf[6]-pDataBuf[14]-pDataBuf[16]);
			dwRealAmplitude += 4634*(pDataBuf[5]-pDataBuf[15]);
			dwRealAmplitude += 0*(-pDataBuf[10]);
			if(dwRealAmplitude>=0)                                                           /*�������뱣֤����*/
				elec->Real = (dwRealAmplitude+32767)>>15;
			else
				elec->Real = (dwRealAmplitude-32767)>>15;

			dwImagAmplitude += 2317*(pDataBuf[0]+pDataBuf[20]);
			dwImagAmplitude += 4407*(pDataBuf[1]-pDataBuf[9]-pDataBuf[11]+pDataBuf[19]);
			dwImagAmplitude += 3749*(pDataBuf[2]-pDataBuf[8]-pDataBuf[12]+pDataBuf[18]);
			dwImagAmplitude += 2724*(pDataBuf[3]-pDataBuf[7]-pDataBuf[13]+pDataBuf[17]);
			dwImagAmplitude += 1432*(pDataBuf[4]-pDataBuf[6]-pDataBuf[14]+pDataBuf[16]);
			dwImagAmplitude += 0*(-pDataBuf[5]-pDataBuf[15]);
			dwImagAmplitude += 4634*(-pDataBuf[10]);
			if(dwImagAmplitude>=0)
				elec->Imag = (dwImagAmplitude+32767)>>15;
			else
				elec->Imag = (dwImagAmplitude-32767)>>15;
		}
}
void CaluSecondFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 6681*(-pDataBuf[0]);
		dwRealAmplitude += 3277*(pDataBuf[1]-pDataBuf[6]+pDataBuf[11]-pDataBuf[16]);
		dwRealAmplitude += 4577*(pDataBuf[2]-pDataBuf[7]+pDataBuf[12]-pDataBuf[17]);
		dwRealAmplitude += 4129*(pDataBuf[3]-pDataBuf[8]+pDataBuf[13]-pDataBuf[18]);
		dwRealAmplitude += 2104*(pDataBuf[4]-pDataBuf[9]+pDataBuf[14]-pDataBuf[19]);
		dwRealAmplitude += 725*(-pDataBuf[5]+pDataBuf[10]-pDataBuf[15]);
		dwRealAmplitude += 7406*(pDataBuf[20]);
		if(dwRealAmplitude>=0)                                                           /*�������뱣֤����*/
			elec->Real = (dwRealAmplitude+32767)>>15;
		else
			elec->Real = (dwRealAmplitude-32767)>>15;

		dwImagAmplitude += 3404*(pDataBuf[0]);
		dwImagAmplitude += 3277*(pDataBuf[1]-pDataBuf[6]+pDataBuf[11]-pDataBuf[16]);
		dwImagAmplitude += 725*(pDataBuf[2]-pDataBuf[7]+pDataBuf[12]-pDataBuf[17]);
		dwImagAmplitude += 2104*(-pDataBuf[3]+pDataBuf[8]-pDataBuf[13]+pDataBuf[18]);
		dwImagAmplitude += 4129*(-pDataBuf[4]+pDataBuf[9]-pDataBuf[14]+pDataBuf[19]);
		dwImagAmplitude += 4577*(-pDataBuf[5]+pDataBuf[10]-pDataBuf[15]);
		dwImagAmplitude += 1173*(pDataBuf[20]);
		if(dwImagAmplitude>=0)
			elec->Imag = (dwImagAmplitude+32767)>>15;
		else
			elec->Imag = (dwImagAmplitude-32767)>>15;
	}
}
void CaluThirdFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 3609*(-pDataBuf[0]);
		dwRealAmplitude += 4407*(pDataBuf[1]-pDataBuf[5]-pDataBuf[11]+pDataBuf[15]);
		dwRealAmplitude += 3749*(pDataBuf[2]-pDataBuf[4]-pDataBuf[12]+pDataBuf[14]);
		dwRealAmplitude += 0*(-pDataBuf[3]-pDataBuf[13]);
		dwRealAmplitude += 1432*(-pDataBuf[6]-pDataBuf[10]+pDataBuf[16]);
		dwRealAmplitude += 2724*(pDataBuf[7]+pDataBuf[9]-pDataBuf[17]-pDataBuf[19]);
		dwRealAmplitude += 4634*(pDataBuf[8]-pDataBuf[18]);
		dwRealAmplitude += 5041*(pDataBuf[20]);
		if(dwRealAmplitude>=0)                                                           /*�������뱣֤����*/
			elec->Real = (dwRealAmplitude+32767)>>15;
		else
			elec->Real = (dwRealAmplitude-32767)>>15;

		dwImagAmplitude += 3609*(pDataBuf[0]);
		dwImagAmplitude += 1432*(pDataBuf[1]+pDataBuf[5]-pDataBuf[11]-pDataBuf[15]);
		dwImagAmplitude += 2724*(-pDataBuf[2]-pDataBuf[4]+pDataBuf[12]+pDataBuf[14]);
		dwImagAmplitude += 4634*(-pDataBuf[3]+pDataBuf[13]);
		dwImagAmplitude += 4407*(pDataBuf[6]-pDataBuf[10]-pDataBuf[16]);
		dwImagAmplitude += 3749*(pDataBuf[7]-pDataBuf[9]-pDataBuf[17]+pDataBuf[19]);
		dwImagAmplitude += 0*(-pDataBuf[8]-pDataBuf[18]);
		dwImagAmplitude += 798*(pDataBuf[20]);
		if(dwImagAmplitude>=0)
			elec->Imag = (dwImagAmplitude+32767)>>15;
		else
			elec->Imag = (dwImagAmplitude-32767)>>15;
	}
}
void CaluFifthFourier_20(const LONG *pDataBuf,TRelElecValPar *elec)
{
	LONG dwRealAmplitude = 0;
	LONG dwImagAmplitude = 0;
	if(pDataBuf!=NULL && elec!=NULL)
	{
		dwRealAmplitude += 513*(-pDataBuf[0]);
		dwRealAmplitude += 3749*(pDataBuf[1]-pDataBuf[3]+pDataBuf[5]-pDataBuf[7]+pDataBuf[9] \
			-pDataBuf[11]+pDataBuf[13]-pDataBuf[15]+pDataBuf[17]-pDataBuf[19]);
		dwRealAmplitude += 2724*(-pDataBuf[2]+pDataBuf[4]-pDataBuf[6]+pDataBuf[8]-pDataBuf[10] \
			+pDataBuf[12]-pDataBuf[14]+pDataBuf[16]-pDataBuf[18]);
		dwRealAmplitude += 3236*(pDataBuf[20]);
		
		if(dwRealAmplitude>=0)                                                           /*�������뱣֤����*/
			elec->Real = (dwRealAmplitude+32767)>>15;
		else
			elec->Real = (dwRealAmplitude-32767)>>15;

		dwImagAmplitude += 3236*(pDataBuf[0]);
		dwImagAmplitude += 2724*(-pDataBuf[1]+pDataBuf[3]-pDataBuf[5]+pDataBuf[7]-pDataBuf[9] \
			+pDataBuf[11]-pDataBuf[13]+pDataBuf[15]-pDataBuf[17]+pDataBuf[19]);
		dwImagAmplitude += 3749*(-pDataBuf[2]+pDataBuf[4]-pDataBuf[6]+pDataBuf[8]-pDataBuf[10] \
			+pDataBuf[12]-pDataBuf[14]+pDataBuf[16]-pDataBuf[18]);
		dwImagAmplitude += 513*(pDataBuf[20]);

		if(dwImagAmplitude>=0)
			elec->Imag = (dwImagAmplitude+32767)>>15;
		else
			elec->Imag = (dwImagAmplitude-32767)>>15;
	}
}
void MakeActRptWithoutStart(DWORD inf)
{	
	TAbsTime AbsTime;
	ReadAbsTime(&AbsTime);	
	DB_RelayStart(0x55BB,RELAY_INF_NOUSED,&AbsTime,FALSE);
	MakeActRpt(inf,0x55BB);
	DB_RelayEnd(0x55BB);
}
void CTCheck(TRelCTBrokPar *Relay)
{
	if(*(Relay->pRelCTBrokCFG) == CFG_STATUS_CLR)	//CT����δͶ��
	{
		if(ReadRelayWord(Relay->ActRelayWord))
			MakeAlarmRpt(Relay->wRelResRptNum);
		ResetCTCheck(Relay);
		return;
	}
	if(*(Relay->pRelCurMax) < *(Relay->pRelCurSet))
		return;
	if(ReadRelayWord(Relay->ActRelayWord))
	{		
		if(*(Relay->pRelCurMax) < (*(Relay->pRelCurMin))*3)
		{
			if(*(Relay->pRelCurMax) - *(Relay->pRelCurMin) < *(Relay->pRelCurSet))
			{			
				Relay->byRelRCounter++; 
				if(Relay->byRelRCounter > 5)
				{
					MakeAlarmRpt(Relay->wRelResRptNum);
					ResetCTCheck(Relay);
				}
			}
			else
				Relay->byRelRCounter = 0; 
			return;
		}
		return;
	}
	if(*(Relay->pRelCurMax) < (*(Relay->pRelCurMin))*3)
	{
		ResetCTCheck(Relay);
	}
	else
	{
		if(!ReadRelayWord(Relay->StartRelayWord))		//��δ���
		{
			Relay->byRelFCounter++;
			if(Relay->byRelFCounter > 2)
			{
				Relay->dwRelTimer = *(Relay->dwSysTimer);
				WriteRelayWord(Relay->StartRelayWord, RELAY_ON);
				return;
			}
		}
		else
		{
			if((*(Relay->dwSysTimer) - Relay->dwRelTimer) >= *(Relay->pRelTimeSet))
			{
				WriteRelayWord(Relay->ActRelayWord, RELAY_ON);
				MakeAlarmRpt(Relay->wRelRptNum);
			}
		}
	}
}
void ResetCTCheck(TRelCTBrokPar *Relay)
{
	WriteRelayWord(Relay->ActRelayWord, RELAY_OFF);
	WriteRelayWord(Relay->StartRelayWord, RELAY_OFF);
	Relay->byRelFCounter = 0; 
	Relay->byRelRCounter = 0; 
	Relay->dwRelTimer = 0; 
}
BOOL PhaseCheck(const LONG *A1Buf,const LONG *A2Buf)
{
	LLONG value = 0;
	for(int i=0;i<POINT_PER_PERIOD;i++)
	{
		value += A1Buf[i]*A2Buf[i];
	}
	if(value>0)
		return TRUE;
	else
		return FALSE;
}
void RefreshCommRW(WORD ETHRW,WORD COMRW,WORD CANRW,WORD MAINRW)
{		
	static DWORD cnt = 0;
	WORD sernum,cannum,ethnum;	
	sernum = DB_GetSerialNum();
	cannum = DB_GetCANNum();
	ethnum = DB_GetEthLinkNum();
	TSerialPara tSerial;
	TCanBusPara tCan;
	TEthPortPara tEth;
	BOOL maintFlag = FALSE;
	if(ethnum > 0)
	{
		for(WORD i=0;i<ethnum;i++)
		{			
			DB_GetETHPara(i,&tEth);
			WriteCommRW(i,tEth.wPortID,ETHRW,SS_ETHA_COMM_ERR);
			if(tEth.wDriver == 2) // 2����ά����Լ
			{
				if(!ReadRelayWord(ETHRW+i))
					maintFlag = TRUE;					
			}
		}
	}
	else
	{
		for(WORD i=0;i<10;i++)
		{
			if(!ReadRelayWord(ETHRW+i))
				WriteRelayWord(ETHRW+i,RELAY_ON);
		}
	}
	if(sernum > 0)
	{
		for(WORD i=0;i<sernum;i++)
		{
			DB_GetSerialPara(i, &tSerial);		
			WriteCommRW(i,tSerial.wPortID,COMRW,SS_SER_COMM_ERR);			
			if(tSerial.byDriver == 2) // 2����ά����Լ
			{
				if(!ReadRelayWord(COMRW+i))
					maintFlag = TRUE;					
			}
		}
	}
	else		
	{
		if(!ReadRelayWord(COMRW))
			WriteRelayWord(COMRW,RELAY_ON);
	}
	if(cannum > 0)
	{
		for(WORD i=0;i<cannum;i++)
		{
			DB_GetCANPara(i, &tCan);
			WriteCommRW(i,tCan.wPortID,CANRW,SS_CAN_COMM_ERR);			
			if(tCan.byDriver == 2) // 2����ά����Լ
			{
				if(!ReadRelayWord(CANRW+i))
					maintFlag = TRUE;					
			}
		}
	}
	else
	{
		for(WORD i=0;i<2;i++)			
		{
			if(!ReadRelayWord(CANRW+i))
				WriteRelayWord(CANRW+i,RELAY_ON);
		}
	}
	
	cnt++;
	if(cnt>100)
	{
		if(maintFlag == TRUE)
		{
			if(!ReadRelayWord(MAINRW))
				WriteRelayWord(MAINRW,RELAY_ON);
		}
		else
		{
			if(ReadRelayWord(MAINRW))
				WriteRelayWord(MAINRW,RELAY_OFF);
		}
	}
}

void WriteCommRW(WORD i,WORD wPortID,WORD RelayWord, WORD RunningFlag)
{		
	BYTE commuStatus;
	if(DAE_GetCommStatus(wPortID,commuStatus)!=DB_ERR_OK)		
	{
		if(!ReadRelayWord(RelayWord+i))
			WriteRelayWord(RelayWord+i,RELAY_ON);
	}
	else
	{
//		if((DAE_GetRunningFlags(RunningFlag+i))||(commuStatus == COMM_STATUS_BREAK))
		if(commuStatus == COMM_STATUS_BREAK)    //20140314  yxs
		{
			if(!ReadRelayWord(RelayWord+i))
				WriteRelayWord(RelayWord+i,RELAY_ON);
		}
		else
		{
			if(ReadRelayWord(RelayWord+i))
				WriteRelayWord(RelayWord+i,RELAY_OFF);
		}
	}
}

//:~
