/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			m5234iic.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           m5234��IIC��������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      �ṩIIC���йس�ʼ������д���ƵȲ����ӿ�                          
*                                                                                  
* ����                                                                             
*                                                                                  
*      	M5234IICInit		IIC��ʼ��
*		M5234IICRW			IIC��д�����ӿ�
*		M5234IICInt			IIC�жϴ�����
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/11/21    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "m5234iic.h"

TM5234iic TM5234IICBuf;		//iicͨ�Ż���

/*********************************************************************************************
 *
 * ��������     IIC��ʼ�������ö�Ӧ���ţ�����ʱ��Ƶ�ʣ������ж�
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234IICInit(void)
{
	memset((void*)&TM5234IICBuf, 0, sizeof(TM5234iic));

	//����I2C_SCL(L14) I2C_SDA(L15)��ΪI2C��������
	MCF_GPIO_PAR_FECI2C |= 0x0f;

	//����ʱ������Ϊ468.75kHz����Ҫ̫�ߣ���Ϊʱ��оƬҪ���ܳ���400kHz
	MCF_I2C_I2FDR = IIC_PRESCALER;

	MCF_I2C_I2CR |= MCF_I2C_I2CR_IEN;	//iic��������

	MCF_INTC0_ICRn(17) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(3);

	OS_SetVector(MCF_INTC0_LVL_IIC, M5234IICInt, 0);	//�жϼ���
	OS_IntEnable(MCF_INTC0_LVL_IIC);

		
	MCF_I2C_I2CR |= MCF_I2C_I2CR_IIEN;	//�ж�����

}

/*********************************************************************************************
 *
 * ��������     IIC��д�����ӿڣ���ɶ�д���������ڶ���������ַ������д���ֱ���л�������ģʽ
 *				û�в���RESTART��ʽ��
 *
 * ����˵��      - bDevAddr	: ���� 	�豸��ַ
 *				 - bCmdAddr	: ����	�����ַ
 *				 - bBuf		: ˫��  д����ȡ���ݻ�������ַ
 *				 - bLen		: ����	д����ȡ�����ݳ���
 *				 - isRead	: ����	�Ƿ�Ϊ������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234IICRW(BYTE bDevAddr, BYTE bCmdAddr, BYTE *bBuf, BYTE bLen, BOOL isRead)
{
	int Delay;
	BYTE status;

	TM5234IICBuf.bSlaveDevAddr = bDevAddr;

	TM5234IICBuf.bDevCmdAddr = bCmdAddr; 		//���汻д�豸�����ַ
	TM5234IICBuf.bIicBuf = bBuf; 				//���淢�ͻ�������ַ
	TM5234IICBuf.bIicDataLen = bLen; 			//���淢�����ݳ���
	TM5234IICBuf.isRead = isRead;
	TM5234IICBuf.bIicEnd = 0;
	TM5234IICBuf.bCmdAddrEnb = 2;	//���������ַ��־
	Delay = IIC_DELAY*(bLen+2)*100;	//��ʱ����

	MCF_I2C_I2CR |= MCF_I2C_I2CR_IEN;	//iic��������

	MCF_I2C_I2AR = bDevAddr;	
	
	MCF_I2C_I2CR |= MCF_I2C_I2CR_TXAK;	//������ACK�ź���Ӧ(��������ʱ��Ҫ)
		
	MCF_I2C_I2CR |= MCF_I2C_I2CR_MTX;	//���÷���ģʽ
	
	MCF_I2C_I2CR |= MCF_I2C_I2CR_MSTA;	//������ģʽ������start�ź�
	MCF_I2C_I2DR = TM5234IICBuf.bSlaveDevAddr;	//���͵�ַ�ֽ�

	//	OS_Sleep(Delay);	//�˴�������sleep��ʽ�ȴ�����֪��ԭ��	
	//�ȴ����ݷ������
	for(int i=0; i<100; i++)
	{
		SysDelay(Delay);	//�ȴ����ݷ������
		if(0xff == TM5234IICBuf.bIicEnd)	//����Ƿ��������ͳɹ�
			return ERR_SUCCESS;
	}
	return ERR_FAILURE;
	
}

/*********************************************************************************************
 *
 * ��������     IIC�жϴ�����
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234IICInt(void * arg)
{
	BYTE status, temp;

	status = MCF_I2C_I2SR;	//��ȡ״̬�Ĵ���
//	TM5234IICBuf.bReg = status;
	MCF_I2C_I2SR = 0x00;					//���״̬�Ĵ���״̬

	if(status & MCF_I2C_I2SR_IAL)	//�ٲ��źŶ�ʧ
	{
		MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//����STOP�ź�
		MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//��ֹiic����
		TM5234IICBuf.bIicEnd = 1;	//�쳣��ֹ
		return;
	}

	if(MCF_I2C_I2CR & MCF_I2C_I2CR_MTX)	//����ģʽ
	{
		if( (status&MCF_I2C_I2SR_RXAK) == 0)	//�յ�ACK�ź�
		{
			if(TM5234IICBuf.bCmdAddrEnb == 2)	//�ղŷ��͵��Ǵӵ�ַ�ֽ�
			{
				MCF_I2C_I2DR = TM5234IICBuf.bDevCmdAddr;	//���ʹ�������Ϣ
				TM5234IICBuf.bCmdAddrEnb = 0;
			}
			else	//������Ϣ�������,���ͻ����������Ϣ
			{
				if(TM5234IICBuf.isRead == FALSE)	//д����������������Ϣ
				{
					if(TM5234IICBuf.bIicDataLen)	//����ʣ������
					{
						MCF_I2C_I2DR = *TM5234IICBuf.bIicBuf++;
						TM5234IICBuf.bIicDataLen--;
					}
					else	//���ݷ������,��������
					{
						MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//����STOP�ź�
						TM5234IICBuf.bIicEnd = 0xff;	//д������������
						MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//��ֹiic����
						return;
					}
				}
				else	//��������������ӵ�ַ��������Ϣ���л�������״̬
				{
					//�л�������״̬
					MCF_I2C_I2CR &= ~(MCF_I2C_I2CR_MTX|MCF_I2C_I2CR_TXAK);
					if(TM5234IICBuf.bIicDataLen == 1)	//�������ڶ�����ǰ�ò���STOP�źű�־
						MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);

					//ģ�����һ�ζ�����
					temp = MCF_I2C_I2DR;
				}
			}
		}
		else	//����״̬û���յ�ACK�źţ���ֹ
		{
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//����STOP�ź�
			TM5234IICBuf.bIicEnd = 2;	//�쳣��ֹ
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//��ֹiic����
			return;
		}
	}
	else	//����ģʽ������ÿ�յ�һ���ֽ���Ϣ����Զ���ӦACK
	{
		if(TM5234IICBuf.bIicDataLen == 1)	//�������ڶ�����ǰ�ò�����ACK�źű�־
			MCF_I2C_I2CR |= MCF_I2C_I2CR_TXAK;
		else if(TM5234IICBuf.bIicDataLen == 0)//���һ���������ݣ���������������
		{
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_MSTA);	//����STOP�ź�
			TM5234IICBuf.bIicEnd = 0xff;	//
			MCF_I2C_I2CR &= (~MCF_I2C_I2CR_IEN);	//��ֹiic����
		}
		*TM5234IICBuf.bIicBuf++ = MCF_I2C_I2DR;
		if(TM5234IICBuf.bIicDataLen)
			TM5234IICBuf.bIicDataLen--;
	}
}


