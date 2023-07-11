/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			aa1024.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           25AA1024����EEPROM������SPI�ӿڣ�128K�ֽڣ�ҳ����Ϊ256�ֽڣ����ʱ������20MHz                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      ���ݱ���Ҫ���ṩ���в����ӿڣ�������д���������������͹��Ŀ��Ƶȡ�                          
*                                                                                  
* ����                                                                             
*                                                                                  
*      	AA1024WEnable		25AA1024дʹ�ܿ��ؿ���
*		AA1024ReadStatus	25AA1024��״̬�Ĵ���
*		AA1024WriteStatus	25AA1024д״̬�Ĵ���
*		AA1024WriteBP		25AA1024д״̬�Ĵ�����BP0/BP1
*		AA1024DPDInOut		25AA1024�͹���ģʽ����
*		AA1024PageRead		AA1024��ҳ��Χ�ڶ�����
*		AA1024PageWrite		AA1024��ҳ��Χ��д����
*		AA1024Read			AA1024������
*		AA1024Write			AA1024��ҳ��Χ��д����
*		AA1024PageWrite		AA1024��ҳ��Χ��д����
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               09/02/14    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#if(EEPROM_MODULE == AA1024)

#include "aa1024.h"
#include "m5234spi.h"

/*********************************************************************************************
 *
 * ��������     25AA1024дʹ�ܿ��ؿ���
 *
 * ����˵��      - isEnable	: ���� 	дʹ�ܿ��ر�־
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024WEnable(BOOL isEnable)
{
	BYTE bCmdBuf[10],bCmdLen,bStatusReg;
	BYTE bDataBuf[10], bDataLen;
	
	//��SPI����ΪAA1024��Ҫ��ģʽ
	SetAA1024Mode();
	
	SetSpiCs0Disact();	//����Ƭѡ��Ч

	if(isEnable == TRUE)	//д����
		bCmdBuf[0] = AA1024_WREN;
	else					//д��ֹ
		bCmdBuf[0] = AA1024_WRDI;
	
	//�������3�ο���дʹ��
	for(int i=0; i<3; i++)
	{
		//ʹ�ܿ��Ʋ���
		if(M5234SpiOperate(AA1024_CS_ADDR, bCmdBuf, 1, bDataBuf, 0, SPI_WRITE) == ERR_FAILURE)
		{
			M5234SpiReset();
			ReportMsg("AA1024EnablWrite Error.");
			continue;
		}
		
		//���ڷ��������дʹ�����������ӳ��״̬�Ĵ�����һ����ʱ
		for(int j=0; j<AA1024_STSREG_DELAY; j++)
		{
			//��״̬�Ĵ���
			if(AA1024ReadStatus(&bStatusReg) == ERR_SUCCESS)
			{
				//���дʹ�ܲ����Ƿ�ɹ�
				if( (isEnable == TRUE) && (bStatusReg&AA1024_STS_WEL) )
					return ERR_SUCCESS;
				
				//���д��ֹ�����Ƿ�ɹ�
				if( (isEnable == FALSE) && (~(bStatusReg&AA1024_STS_WEL)) )
					return ERR_SUCCESS;
			}
		}
	}

	//�ظ���β�����дʹ�ܿ��ؿ���ʧ��
	ReportMsg("AA1024EnablWrite Error1.");
	return 	ERR_FAILURE;
}

/*********************************************************************************************
 *
 * ��������     25AA1024��״̬�Ĵ���
 *
 * ����˵��      - status	: ��� 	��ȡ��״̬�Ĵ���ֵ
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024ReadStatus(BYTE *status)
{
	BYTE bCmdBuf[10],bCmdLen;
//	BYTE bDataBuf[10], bDataLen;
//	DWORD rtn;
	
	bCmdBuf[0] = AA1024_RDSR;
	
//	bCmdLen = 1;
	
//	memset((void *)bDataBuf, 0, sizeof(bDataBuf));
	
//	bDataLen = 1;
	
	SetSpiCs0Disact();	//����Ƭѡ��Ч
	
	return M5234SpiOperate(AA1024_CS_ADDR, bCmdBuf, 1, status, 1, SPI_READ);
/*	
	*status = bDataBuf[0];
	if(rtn == ERR_FAILURE)
	{
		ReportMsg("AA1024ReadStatus ERROR, data=%x.", bDataBuf[0]);
	}
	return rtn;
*/	
}

/*********************************************************************************************
 *
 * ��������     25AA1024д״̬�Ĵ���
 *
 * ����˵��      - status	: ���� 	д���״ֵ̬
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024WriteStatus(BYTE status)
{
	BYTE bCmdBuf[10],bCmdLen;
	BYTE bDataBuf[10], bDataLen, data;
	int i=3;
	BYTE flag = 0;

	//����WP���ţ�����д����
	AA1024WUnP();

	//дʹ�ܴ�
	if( AA1024WEnable(TRUE) == ERR_FAILURE )
		return ERR_FAILURE;


	//д״̬�Ĵ���
	bCmdBuf[0] = AA1024_WRSR;
	bCmdBuf[1] = status;

	//Ϊ��֤�ɿ���֧��3���ظ�
	//for(i=0; i<3; i++)
	while(i--)
	{
		if(M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 2, bDataBuf, 0, SPI_WRITE) == ERR_SUCCESS)
		{
			flag = 0xff;
			break; 
		}
	}

	if(flag == 0)
	{
		ReportMsg("AA1024WriteStatus Error.");
		return ERR_FAILURE;
	}
	//ͨ����ȡWIP״̬�����д��״̬�Ƿ���ȷ
	for(i=0; i<AA1024_STSREG_DELAY; i++)
	{
		//��״̬�Ĵ�������SysDelay��ʱ����
		OS_Sleep(10);
		if( AA1024ReadStatus(&data) == ERR_SUCCESS)
		{
			if( (data&AA1024_STS_WIP) == 0)
			{
				//����WP���ţ���ֹд����
				AA1024WP();
				return ERR_SUCCESS;
			}
		}
	}
	//����WP���ţ���ֹд����
	AA1024WP();
	
	ReportMsg("AA1024WriteStatus Error1, data=%x status=%x.", data, status);
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * ��������     25AA1024д״̬�Ĵ�����BP0/BP1�����п鱣������
 *
 * ����˵��      - data	: ���� 	д���������Ϣ
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024WriteBP(BYTE data)
{
	BYTE bStatusReg;

	//���д�������Ƿ�Ϸ�
	if( (data != AA1024_BLOCK_PT) && (data != AA1024_BLOCK_UNPT) )
		return ERR_FAILURE;

	//��SPI����ΪAA1024��Ҫ��ģʽ
	SetAA1024Mode();

	SetSpiCs0Disact();	//����Ƭѡ��Ч

	//д״̬�Ĵ���,��Ϊд�Ĵ����ڲ��ж�״̬��飬��˲����ظ����
	return AA1024WriteStatus(data);
	/*	
	if(AA1024WriteStatus(data) == ERR_FAILURE)
	{
		ReportMsg("AA1024WriteBP Error.");
		return ERR_FAILURE;
	}
	//���ڷ��������дʹ�����������ӳ��״̬�Ĵ�����һ����ʱ
	for(int j=0; j<AA1024_STSREG_DELAY; j++)
	{
		//��״̬�Ĵ���
		if(AA1024ReadStatus(&bStatusReg) == ERR_SUCCESS)
		{
			//����־д������Ƿ�ɹ�
			if( (bStatusReg&0xfc) == (data&0xfc) )
				return ERR_SUCCESS;
		}
	}
	
	ReportMsg("AA1024WriteStatus Error1, data=%x bStatusReg=%x.", data, bStatusReg);
	return	ERR_FAILURE;
*/	
}


/*********************************************************************************************
 *
 * ��������     25AA1024�͹���ģʽ����
 *
 * ����˵��      - isIn	: ���� 	���뻹���˳�
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024DPDInOut(BOOL isIn)
{
	BYTE bCmdBuf[10],bCmdLen;
	BYTE bDataBuf[10], bDataLen;
	DWORD rtn;
//	BYTE status = 0;
//	DWORD delay = 1000;

	//�ж��ǽ��뻹���˳��͹���ģʽ
	if(isIn == TRUE)
		bCmdBuf[0] = AA1024_DPD;
	else
		bCmdBuf[0] = AA1024_RDPD;
	
	SetSpiCs0Disact();	//����Ƭѡ��Ч
	
	rtn = M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 1, bDataBuf, 0, SPI_WRITE);

	//�͹���ģʽ���л���Ҫһ�����ȶ�ʱ�䣬Լ100us
	OS_Sleep(10);

//	AA1024ReadStatus(&status);
	
	return rtn;
}


/*********************************************************************************************
 *
 * ��������     AA1024��ҳ��Χ�ڶ���������ҳд���ʹ�ã��������ҳд�������Ƿ���ȷ
 *
 * ����˵��      - pBuf		: ��� 	�����ݻ�����
 *				 - len		: ����	��ȡ���ݳ���
 *				 - address	: ����	��ȡ���ݵ���ʼ��ַ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024PageRead(BYTE *pBuf, DWORD len, DWORD address)
{
	BYTE bCmdBuf[10],bCmdLen;
	int i=3;
	
/*
	ASSERT(len <= AA1024_PAGE_LEN);
	if(len>AA1024_PAGE_LEN)
		return ERR_FAILURE;

	ASSERT(address<AA1024_LEN);
	if(address>=AA1024_LEN)
		return ERR_FAILURE;
*/	
	bCmdBuf[0] = AA1024_READ;
	bCmdBuf[1] = (BYTE)(address>>16);
	bCmdBuf[2] = (BYTE)(address>>8);
	bCmdBuf[3] = (BYTE)(address);

	while(i--)
	{
		if(M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 4, pBuf, \
					len, SPI_READ) == ERR_SUCCESS)
			return ERR_SUCCESS;
	}
	
	return ERR_FAILURE;
}


/*********************************************************************************************
 *
 * ��������     AA1024��ҳ��Χ��д����
 *
 * ����˵��      - pBuf		: ���� 	д�����ݻ�����
 *				 - len		: ����	д�����ݳ���
 *				 - address	: ����	д�����ݵ���ʼ��ַ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024PageWrite(BYTE *pBuf, DWORD len, DWORD address)
{
	BYTE bCmdBuf[10],bCmdLen, status;
	int i = 3;

	ASSERT(len <= AA1024_PAGE_LEN);
	if(len>AA1024_PAGE_LEN)
		return ERR_FAILURE;

	//ע�⣬����ÿ��д������Ҫִ��һ��дʹ�ܲ���������д��ʧ��
	if( AA1024WEnable(TRUE) == ERR_FAILURE )
		return ERR_FAILURE;

	//����д��������Χ������һҳ��
	bCmdBuf[0] = AA1024_WRITE;
	bCmdBuf[1] = (BYTE)(address>>16);
	bCmdBuf[2] = (BYTE)(address>>8);
	bCmdBuf[3] = (BYTE)(address);

	while(i--)
	{
		if(M5234SpiOperate(AA1024_CS_ADDR,bCmdBuf, 4, pBuf, len, SPI_WRITE) == ERR_SUCCESS)
		{
			//����Ƿ�д��
			OS_Sleep(10);
			AA1024ReadStatus(&status);
			if( (status&AA1024_STS_WIP) == 0)	//д���
			{
//				ReportMsg("AA1024PageWrite address=%d, len=%d OK.",address, len);
				return ERR_SUCCESS;
			}
		}
	}

	
	ReportMsg("AA1024PageWrite address=%d, len=%d ERROR  ...",address, len);
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * ��������     AA1024����������ʼ��ַ�ͳ������⣬ֻҪ��AA1024����Ч�ռ��ڼ��ɡ�
 *				���ڳ����ݶ���Ϊ�˷�ֹϵͳ��ʱ�䴦���ж�״̬����ΪС�ζ�ȡ�����ӿɿ���
 *
 * ����˵��      - pBuf		: ��� 	�����ݻ�����
 *				 - len		: ����	��ȡ���ݳ���
 *				 - address	: ����	��ȡ���ݵ���ʼ��ַ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024Read(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD dwTrueLen, dwTrueAddr, dwReadLen;
	BYTE *pReadPtr;
	
	//�����Ϸ��Լ��
	ASSERT((address+len)<=AA1024_LEN);
	if((address+len)>AA1024_LEN)
		return ERR_FAILURE;

	//�˳��͹���ģʽ
	if( AA1024DPDInOut(FALSE) == ERR_FAILURE)
		return ERR_FAILURE;
	
	pReadPtr = pBuf;
	dwTrueLen = len;
	dwTrueAddr = address;

	//Ϊ�˼��ٶ�����ʱ��(��ֹ��ʱ���ж�)��ÿ������ȡһ��ҳ���ȵ����ݣ�
	//ע�ⲻһ����ҳ���룬��Ϊ���������Կ�ҳ
	while(dwTrueLen>0)
	{
		if(dwTrueLen<AA1024_PAGE_LEN)
			dwReadLen = dwTrueLen;
		else
			dwReadLen = AA1024_PAGE_LEN;
		dwTrueLen -= dwReadLen;

		//����ҳ���������ж�����
		if(AA1024PageRead(pReadPtr, dwReadLen, dwTrueAddr) == ERR_FAILURE)
		{
			AA1024DPDInOut(TRUE);	//����ʧ�ܣ�����ǰ����͹���ģʽ
			return ERR_FAILURE;
		}
		//�ƶ���������ָ��Ͷ���ַλ��
		pReadPtr += dwReadLen;
		dwTrueAddr += dwReadLen;
	}

	AA1024DPDInOut(TRUE);	//������ɣ�����ǰ����͹���ģʽ
	return ERR_SUCCESS;
	
}

/*********************************************************************************************
 *
 * ��������     AA1024д��������ʼ��ַ�ͳ������⣬ֻҪ��AA1024����Ч�ռ��ڼ��ɡ�
 *				����AA1024ֻ֧��ҳд�������ȿ�ҳʱ����Ҫ�ֳɶ�ҳд�룬����Ҫͨ����������
 *				���ÿ��д��������Ƿ���ȷ��
 *
 * ����˵��      - pBuf		: ���� 	д�����ݻ�����
 *				 - len		: ����	д�����ݳ���
 *				 - address	: ����	д�����ݵ���ʼ��ַ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024Write(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD dwTrueLen, dwTrueAddr, dwWriteLen;
	BYTE *pWritePtr;
	BYTE flag1 = 0;
	BYTE flag2 = 0;
	BYTE bReadBuf[AA1024_PAGE_LEN];	//�Ƚ϶����ݻ�����

	//�����Ϸ��Լ��
	ASSERT((address+len)<=AA1024_LEN);
	if((address+len)>AA1024_LEN)
		return ERR_FAILURE;


	//�˳��͹���ģʽ
	if( AA1024DPDInOut(FALSE) == ERR_FAILURE)
		return ERR_FAILURE;

	//��ֹ�鱣��
	if( AA1024WriteBP(AA1024_BLOCK_UNPT) == ERR_FAILURE)
	{
		AA1024DPDInOut(TRUE);	//�ָ�����͹���ģʽ
		return ERR_FAILURE;
	}

	pWritePtr = pBuf;
	dwTrueLen = len;
	dwTrueAddr = address;

	//���ȴ����ҳ����Ĳ���
	if((dwTrueAddr%AA1024_PAGE_LEN))
	{
		//�����һ��д�����ݵĳ���
		if((dwTrueAddr%AA1024_PAGE_LEN+dwTrueLen) > AA1024_PAGE_LEN)
			dwWriteLen = AA1024_PAGE_LEN - dwTrueAddr%AA1024_PAGE_LEN;
		else
			dwWriteLen = len;
		//д���һ������
		if( AA1024PageWrite(pWritePtr, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
		{
//			memset(bReadBuf, 0, AA1024_PAGE_LEN);
//			SysDelay(delay);
//			OS_Sleep(10);
			//���¶������ݽ��бȽϼ��
			if(AA1024PageRead(bReadBuf, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
			{
//				ReportMsg("read data:%x %x %x %x", bReadBuf[0],bReadBuf[1],bReadBuf[2],bReadBuf[3]);
				//���ݱȽ�
				if( DataCmp(pWritePtr, bReadBuf, dwWriteLen) == TRUE )
				{
					//д����ȷ�ƶ�д�����ݻ�����ָ���д���ַ,�޸���Ҫд������ݳ���
					pWritePtr += dwWriteLen;
					dwTrueAddr += dwWriteLen;
					dwTrueLen -= dwWriteLen;
					flag1 = 0xaa;	
				}
				else
				{
					ReportMsg("AA1024Write cmp error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
				}
			}
		}
	}
	else
		flag1 = 0xaa;
	
	if(flag1)	//��һ��д����ȷ
	{
		while(dwTrueLen)
		{
			flag2 = 0;
			//����ÿ��д������ݳ���
			if(dwTrueLen<AA1024_PAGE_LEN)
				dwWriteLen = dwTrueLen;
			else
				dwWriteLen = AA1024_PAGE_LEN;
			
			//д������
			if( AA1024PageWrite(pWritePtr, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
			{
				
//				ReportMsg("write data:%x %x %x %x", pWritePtr[0],pWritePtr[1],pWritePtr[2],pWritePtr[3]);
//				memset(bReadBuf, 0, AA1024_PAGE_LEN);
//				OS_Sleep(10);
//				SysDelay(delay);
				//���¶������ݽ��бȽϼ��
				if(AA1024PageRead(bReadBuf, dwWriteLen, dwTrueAddr) == ERR_SUCCESS)
				{
//					ReportMsg("read data:%x %x %x %x", bReadBuf[0],bReadBuf[1],bReadBuf[2],bReadBuf[3]);
					//���ݱȽ�
					if( DataCmp(pWritePtr, bReadBuf, dwWriteLen) == TRUE )
					{
						//д����ȷ�ƶ�д�����ݻ�����ָ���д���ַ,�޸���Ҫд������ݳ���
						pWritePtr += dwWriteLen;
						dwTrueAddr += dwWriteLen;
						dwTrueLen -= dwWriteLen;
						flag2 = 0xaa;	
					}
					else
					{
						ReportMsg("AA1024Write cmp error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
						break;
					}
				}
				else
				{
					ReportMsg("AA1024Write read error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
					break;
				}
			}
			else
			{
				ReportMsg("AA1024Write page read error,add=%d len=%d ..", dwTrueAddr, dwWriteLen);
				break;
			}
		}
		flag2 = 0xaa;
	}

	AA1024WriteBP(AA1024_BLOCK_PT);	//�ָ��鱣��
	
	AA1024WEnable(FALSE);	//д���������󣬵���д��ֹ

	AA1024DPDInOut(TRUE);	//�ָ�����͹���״̬


	if( (flag1 == 0xaa) &&(flag2 == 0xaa) )//����ҳ��д��ɹ�
		return ERR_SUCCESS;

	return ERR_FAILURE;
}

/*********************************************************************************************
 *
 * ��������     AA1024Ƭ��������
 *
 * ����˵��      ��
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD AA1024ChipErase()
{
	BYTE bCmdBuf[10],bCmdLen,bStatusReg;
	BYTE bDataBuf[10], bDataLen;
	int i=3;
	
	//��SPI����ΪAA1024��Ҫ��ģʽ
	SetAA1024Mode();
	
	SetSpiCs0Disact();	//����Ƭѡ��Ч

	bCmdBuf[0] = AA1024_CE;
	

	//�������3�β���
	while(i--)
	{
		//ʹ�ܿ��Ʋ���
		if(M5234SpiOperate(AA1024_CS_ADDR, bCmdBuf, 1, bDataBuf, 0, SPI_WRITE) == ERR_FAILURE)
		{
			M5234SpiReset();
			ReportMsg("AA1024ChipErase Error.");
			continue;
		}

		
		//���ڲ�����������Ҫ10msʱ�䣬��Ҫ��ʱ�ȴ�
		OS_Sleep(10);
		//��״̬�Ĵ���
		if(AA1024ReadStatus(&bStatusReg) == ERR_SUCCESS)
		{
			//���дʹ�ܲ����Ƿ�ɹ�
			if( (bStatusReg&AA1024_STS_WIP) == 0)
				return ERR_SUCCESS;
		}
	}

	//Ƭ����ʧ��
	ReportMsg("AA1024ChipErase Error1.");
	return 	ERR_FAILURE;
}

/*********************************************************************************************
 *
 * ��������     EEPROM����������Ҫ�ٽ���Դ����
 *
 * ����˵��      - pBuf		: ��� 	�����ݻ�����
 *				 - len		: ����	��ȡ���ݳ���
 *				 - address	: ����	��ȡ���ݵ���ʼ��ַ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD EepromRead(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD rtn;

	//�ٽ���Դ����,���Ƕ��SPI��������
    OS_LockM(SPI_LOCK);

	AA1024DPDInOut(FALSE);	//�˳��͹���ģʽ

	rtn = AA1024Read(pBuf, len, address);

	AA1024DPDInOut(TRUE);	//����͹���ģʽ

	OS_UnLockM(SPI_LOCK);

	return rtn;
}

/*********************************************************************************************
 *
 * ��������     EEPROMд��������Ҫ�ٽ���Դ����
 *
 * ����˵��      - pBuf		: ���� 	д�����ݻ�����
 *				 - len		: ����	д�����ݳ���
 *				 - address	: ����	д�����ݵ���ʼ��ַ	
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD EEPROMWrite(BYTE *pBuf, DWORD len, DWORD address)
{
	DWORD rtn;
	int i=3;

	
//	ReportMsg("The task%x write eeprom.", RtosGetCurrentTaskId());
	//�ٽ���Դ����,���Ƕ��SPI��������
    OS_LockM(SPI_LOCK);

	while(i--)	
	{
		if( (rtn = AA1024Write(pBuf, len, address)) == ERR_SUCCESS )
			break;
		
		//д���ɹ�����λSPI�ӿ�
		M5234SpiReset();
	}
	
	OS_UnLockM(SPI_LOCK);

	return rtn;
}

/*********************************************************************************************
 *
 * ��������     EEPROM��Ƭ��������
 *
 * ����˵��      ��
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD EEPROMChipErase()
{
	DWORD rtn;

	//�ٽ���Դ����,���Ƕ��SPI��������
    OS_LockM(SPI_LOCK);

	AA1024DPDInOut(FALSE);	//�˳��͹���ģʽ

	AA1024WEnable(TRUE);	//����ǰ��ִ��WREN
	rtn = AA1024ChipErase();
	AA1024WEnable(FALSE);	
	
	AA1024DPDInOut(TRUE);	//����͹���ģʽ

	OS_UnLockM(SPI_LOCK);

	return rtn;
}
/*********************************************************************************************
 *
 * ��������     �����������ݱȽ�
 *
 
  * ����˵��	  - pData1	 : ����  �Ƚ�����1
  * 			  - pData2 	 : ����  �Ƚ�����2
  * 			  - len		 : ����  �Ƚ����ݳ���  
  *
 * ���ش���
 *                ��������һ�»�һ��
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL DataCmp(BYTE *pData1, BYTE *pData2, DWORD len)
{
	DWORD length;
	BYTE *pBuf1, *pBuf2;

	length = len;
	pBuf1 = pData1;
	pBuf2 = pData2;

	while(length--)
	{
		if(*pBuf1 != *pBuf2)
			return FALSE;
		pBuf1++;
		pBuf2++;
	}
	return TRUE;
}

#ifdef _DBUG_AA1024_

#endif



#endif
