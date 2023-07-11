/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			m5234uart.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           m5234��UART��������                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      �ṩUART0/UART1���йس�ʼ�����򿪡����Ϳ��ơ����տ��ơ�IO���ƵȲ����ӿ�                          
*      M523X��UART��FIFO��С������4�ֽ�(��1�ֽڵ���λ�Ĵ���)������2�ֽ�(��1�ֽڵ���λ�Ĵ���)                                                                                  
*                                                                                  
* ����                                                                             
*                                                                                  
*      	M5234UartInit		UART��ʼ�����ر��շ�����
*      	M5234UartOpen		�򿪴��ڣ������жϣ����ò����ʡ�У�顢��������źŵ�                                                              
*      	M5234UartSetBaud	����UART������                                                                         
*      	M5234UartSetComPar 	����UART����ͨ�Ų�������У�顢����λ����ʼλ��ֹͣλ��                                                                        
*      	M5234UartModemInit 	���ݴ���ͨ�����ͳ�ʼ��MODEM�����źţ�����UARTֻ֧��RTS��CTS�ź�                                                                       
*      	M5234UartTxd		UART���Ϳ��ƣ���������ʱ������һ���ֽ������Դ��������ж�                                                                       
*      	M5234UartRxd	    UART���տ���                                                                      
*      	M5234UartInt	    UART�жϴ�����                                                                      
*      	M5234UartRxdInt	    UART�жϽ��չ��̴���                                                                      
*      	M5234UartTxdInt	    UART�жϷ��͹��̴���                                                                      
*      	M5234UartWriteAChar	ͨ��UART����һ���ֽ����ݣ��Դ��������ж�                                                                        
*      	M5234UartIOCtl	   	UART��IO���ƽӿں���                                                                        
*      	M5234UartModemSet	MODEM�ź�������ƣ�����colefire��UART��ֻ�ܿ���RTS�ź�                                                                     
*      	M5234UartModemGet  	��ȡMODEM�����ź�״̬������colefire��UART��ֻ�ܶ�ȡCTS�ź�                                                                        
*      	M5234UartBaudSet	UART����������                                                                        
*      	M5234UartBaudGet	UART������ֵ��ȡ��ʵ���϶�ȡ���Ǵ�����Ĳ������������Ӧ�Ĳ�����ֵ                                                                        
*      	M5234UartParSet	   	UARTͨ�Ų������ã���������λ��У�顢ֹͣλ������                                                                        
*      	M5235UartParGet	   	����UART��ؼĴ�����ֵ����õ�ǰ����ͨ�Ų���                                                                        
*      	M5234UartReset	   	UART��λ����λ���͡����ա�����״̬�������´򿪷��͡����գ�����������ж�                                                                        
*      	M5234UartIsTxdEnd	���UART������һ֡���������Ƿ������������                                                                        
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/7/10    ��ʼ����                         
*                                                                                  
********************************************************************************************/

#include "m5234uart.h"
#include "misi.h"

#define UART_HARD_BUF_LEN	512

BYTE bUartHardBuf[UART_HARD_BUF_LEN];
WORD wUartBufPtr = 0;

#if (MCF5234_UART == YES)

/*********************************************************************************************
 *
 * ��������     UART��ʼ�����ر��շ�����
 *
 * ����˵��      
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234UartInit()
{

	//����UART0��Ҫ��Ϊ�����ã���˳�ʼ��UART1ʱ���ܹرո�ͨ����������ʱ�����κδ���
/*	
	WORD IntLev
	IntLev = OS_IntLock();
	
	//�ر�UART0�ķ��ͺͽ���
	*MCF_UART_UCR0 = MCF_UART_UCR_TX_DISABLED|MCF_UART_UCR_RX_DISABLED;

	//�ر�UART1�ķ��ͺͽ���
	*MCF_UART_UCR1 = MCF_UART_UCR_TX_DISABLED|MCF_UART_UCR_RX_DISABLED;

	OS_IntUnLock(IntLev);
*/	
	//����ʹ��UART0��UART2��UART1����CAN0���շ�
	MCF_GPIO_PAR_UART = 0x3A0C;	//syslib.c�г�ʼ��Ϊ0x40FF
}

/*********************************************************************************************
 *
 * ��������     �򿪴��ڣ������жϣ����ò����ʡ�У�顢��������źŵ�
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *				 - portInfo	: ����	�˿ڲ���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234UartOpen(WORD wPort, TSerialMap *portInfo)
{
	CSerial* pSerial;
	BYTE channel;
	DWORD IntLev;
//	static BYTE ModeReg[2];

	//��鴮����ĺϷ���
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartOpen",FILE_LINE,"port invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	pSerial->m_PortInfo = *portInfo;

	channel = portInfo->nChannel;
	if( (channel != M5234_UART_CHAN0) && (channel != M5234_UART_CHAN1) )
	{
		LogError("M5234UartOpen",FILE_LINE,"uart channel invalidate");
		return ERR_SERIAL_CHANNEL;
	}
	
	//�жϼ���,ע�����һ���жϺ������ݵĲ����Ƿ�������
	if(channel == M5234_UART_CHAN0)	//UART0
	{
		OS_SetVector(MCF_INTC0_LVL_UART0, M5234UartInt, (DWORD)pSerial);
		//syslib.c�ж�UART0~2��MCF_INTC0_ICRn�����˳�ʼ��
//		OS_IntEnable(MCF_INTC0_LVL_UART0);
//	  	MCF_INTC0_ICRn(13) = MCF_INTC0_ICRn_IL(4) | MCF_INTC0_ICRn_IP(2);  
//	    MCF_GPIO_PAR_UART |= 0x40FF;
	}
	else	//UART1
	{
		//ע�⣬ʵ����ʹ�õ���UART2��������Դ
		OS_SetVector(MCF_INTC0_LVL_UART2, M5234UartInt, (DWORD)pSerial);
		OS_IntEnable(MCF_INTC0_LVL_UART2);
		MCF_INTC0_ICRn(15) = MCF_INTC0_ICRn_IL(5) | MCF_INTC0_ICRn_IP(0);  
	}

	IntLev = OS_IntLock();
	//����ж����μĴ���
	MCF_UART_UIMR(channel) = 0x00;

	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//��λ����
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_RX;

	//��λ����
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_TX;

	//��λ����״̬��־λ
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_ERROR;

	//���2��ģʽ�Ĵ�����ע������ͬ����2��ָ���Ƕ�2��ģʽ�Ĵ��������������
	MCF_UART_UMR(channel) = 0x00;
	MCF_UART_UMR(channel) = 0x00;

	//����ʱ��ѡ��Ĵ���
	MCF_UART_UCSR(channel) = MCF_UART_UCSR_RCS_SYS_CLK|MCF_UART_UCSR_TCS_SYS_CLK;

	OS_IntUnLock(IntLev);
	
	//���ò�����
	M5234UartSetBaud(channel, pSerial->m_serialPara.byBaud);

	//���ô�������λ��У��λ��
	M5234UartSetComPar(channel, pSerial->m_serialPara.bFlag);

	//����MODEM��������ź�
	M5234UartModemInit(channel, pSerial->m_serialPara.bFlag);

	IntLev = OS_IntLock();
	//�����ͺͽ���
	MCF_UART_UCR(channel) = MCF_UART_UCR_TX_ENABLED;
	MCF_UART_UCR(channel) = MCF_UART_UCR_RX_ENABLED;

	//�򿪽����жϣ������ж�ֻ���ڷ�������֮ǰ�򿪣��ҷ�����ɺ�رշ����ж��Ա�����Ч���ж�
	MCF_UART_UIMR(channel) = MCF_UART_UIMR_RXRDY_FU;	//����׼�����ж�
	pSerial->m_bIntMode = MCF_UART_UIMR_RXRDY_FU;
	
	OS_IntUnLock(IntLev);

	//���Ĵ����Ƿ�������ȷ
//	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
//	ModeReg[0] = MCF_UART_UMR(channel);
//	ModeReg[1] = MCF_UART_UMR(channel);
	
	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ����UART�����ʣ�ע�Ⲩ���ʵļ��㷽��
 *
 * ����˵��      - channel	: ���� 	UART���ڲ����
 *				 - BaudNum	: ����	���������
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234UartSetBaud(BYTE channel, BYTE BaudNum)
{
	DWORD val, baud;
	DWORD IntLev;

	if( (channel != M5234_UART_CHAN0) && (channel != M5234_UART_CHAN1) )
	{
		LogError("M5234UartSetBaud",FILE_LINE,"uart channel invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	//���ݲ��������ȡ��Ӧ������ֵ
	NumToBaud(BaudNum, &baud);

	//�����ʼ���
	val = (IN_SYS_CLOCK/baud + 16)/32;

	//���ò����ʼĴ���
	IntLev = OS_IntLock();
	MCF_UART_UBG1(channel) = HIBYTE(val);
	MCF_UART_UBG2(channel) = LOBYTE(val);
	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ����UART����ͨ�Ų�������У�顢����λ����ʼλ��ֹͣλ��
 *
 * ����˵��      - channel	: ���� 	UART���ڲ����
 *				 - par		: ����	����ͨ�Ų���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234UartSetComPar(BYTE channel, BYTE par)
{
	BYTE ModeReg1, ModeReg2;
	DWORD IntLev;
	
	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
		
	//���ȶ�ȡģʽ�Ĵ���ԭʼֵ��ע�������UMR1x��ָ����Զ�ָ��UMR2x
	ModeReg1 = MCF_UART_UMR(channel);
	ModeReg2 = MCF_UART_UMR(channel);

	//��������λ
	ModeReg1 &= 0xfc;
	switch(par&DATA_BIT_MASK)
	{
		case DATA_BIT_5:
			//ModeReg1 &= 0xfc;	//��־λֵΪ0
			break;
		case DATA_BIT_6:
			ModeReg1 |= MCF_UART_UMR_BC_6;
			break;
		case DATA_BIT_7:
			ModeReg1 |= MCF_UART_UMR_BC_7;
			break;
		case DATA_BIT_8:
			ModeReg1 |= MCF_UART_UMR_BC_8;
			break;
	}

	//����У��
	ModeReg1 &= ~(0x07<<2);
	switch(par&PARITY_BIT_MASK)
	{
		case NO_PARITY:
			ModeReg1 |= MCF_UART_UMR_PM_NONE;
			break;
		case EVEN_PARITY:	//��־λֵΪ0
			break;
		case ODD_PARITY:
			ModeReg1 |= MCF_UART_UMR_PM_ODD;
			break;
	}

	//����ֹͣλ
	ModeReg2 &= ~0x0f;
	switch(par&STOP_BIT_MASK)
	{
		case STOP_BIT_1:
			ModeReg2 |= MCF_UART_UMR_SB_STOP_BITS_1;
			break;
		case STOP_BIT_2:
			ModeReg2 |= MCF_UART_UMR_SB_STOP_BITS_2;
			break;
	}

	IntLev = OS_IntLock();
	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//����UARTģʽ�Ĵ���
	MCF_UART_UMR(channel) = ModeReg1;
	MCF_UART_UMR(channel) = ModeReg2;

	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     ���ݴ���ͨ�����ͳ�ʼ��MODEM�����źţ�����UARTֻ֧��RTS��CTS�ź�
 *				����485��ʽ�������Զ�RTS���Ʒ�ʽ
 *
 * ����˵��      - channel	: ���� 	UART���ڲ����
 *				 - par		: ����	����ͨ�Ų���
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234UartModemInit(BYTE channel, BYTE medium)
{
	BYTE ModeReg1;
	DWORD IntLev;
	
	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
		
	//���ȶ�ȡģʽ�Ĵ���ԭʼֵ��ע�������UMR1x��ָ����Զ�ָ��UMR2x
	ModeReg1 = MCF_UART_UMR(channel);

	ModeReg1 &= 0x7f;	//�������RTS����λ

	//��ʼ��RTS�����ź�,������Զ�485��ʽ����Ҫ��ʼ��RTS�źţ�͸����ʽ����Ҫ����RTS,
	//������ʽ�������RTS�ź�
	if( medium & SERIAL_ATTR_RS485)
		ModeReg1 |= MCF_UART_UMR_RXRTS;	
	
	//����Ҫ�������RTS�ź�ʱ������RTS�ĳ�ʼ״̬Ϊ�߼���Ч״̬
	//ע��RTS�Ŀ��Ʒ�ʽҪͨ��UOP0n(��)��UOP1n(��)�ֱ����
	if( medium & SERIAL_ATTR_RTSCTR )
		MCF_UART_UOP0(channel) = MCF_UART_UOP0_RTS;	

	IntLev = OS_IntLock();
	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;
	
	//����RTS�����ź�
	MCF_UART_UMR(channel) = ModeReg1;
	
	OS_IntUnLock(IntLev);
	
	return ERR_SUCCESS;
	
}


/*********************************************************************************************
 *
 * ��������     UART���Ϳ��ƣ���������ʱ������һ���ֽ������Դ��������ж�
 *
 * ����˵��      - wPort	: ���� 	�˿ں�
 *				 - bFlag	: ����	���Ϳ��Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234UartTxd(WORD wPort, BYTE bFlag)
{
	CSerial* pSerial;
	BYTE channel, data;
	DWORD IntLev;

	//��鴮����ĺϷ���
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartTxd",FILE_LINE,"port invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	channel = pSerial->m_PortInfo.nChannel;
	
	IntLev = OS_IntLock();

	if(bFlag)
	{
		pSerial->m_bIntMode |= MCF_UART_UIMR_TXRDY;	//����׼�����ж�
	}
	else
	{
		pSerial->m_bIntMode &= ~MCF_UART_UIMR_TXRDY;
//		MCF_UART_UCR(channel) = MCF_UART_UCR_TX_DISABLED;
	}


	//����һ���ֽ��������ݴ��������ж�
	if(bFlag)
	{
		if(pSerial->ReadFromBuf(&data, 1))
			MCF_UART_UTB(channel) = data;	//��������д�뷢��BUFFER
//		MCF_UART_UCR(channel) = MCF_UART_UCR_TX_ENABLED;
	}

	MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * ��������     UART���տ���
 *
 * ����˵��      - wPort	: ���� 	�˿ں�
 *				 - bFlag	: ����	���տ��Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD M5234UartRxd(WORD wPort, BYTE bFlag)
{
	CSerial* pSerial;
	BYTE channel;
	DWORD IntLev;

	//��鴮����ĺϷ���
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartRxd",FILE_LINE,"port invalidate");
		return ERR_SERIAL_CHANNEL;
	}

	channel = pSerial->m_PortInfo.nChannel;
	

	if(bFlag)
		pSerial->m_bIntMode |= MCF_UART_UIMR_RXRDY_FU;	//����׼�����ж�
	else
		pSerial->m_bIntMode &= ~MCF_UART_UIMR_RXRDY_FU;

	IntLev = OS_IntLock();
	MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
	OS_IntUnLock(IntLev);

	return ERR_SUCCESS;
	
}

/*********************************************************************************************
 *
 * ��������     UART�жϴ�����
 *
 * ����˵��      - arg		: ���� 	������ʵ��Ϊ������ָ���ַ
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234UartInt(void * arg)
{
	BYTE status, channel;
	DWORD IntLev;
	static BYTE Regisr, Regsr;
	CSerial* pSerial = (CSerial*)arg;

	channel = pSerial->m_PortInfo.nChannel;

//	Regisr = MCF_UART_UISR(channel);
//	Regsr = MCF_UART_USR(channel);
		
	IntLev = OS_IntLock();
	
	//����ж�״̬�Ĵ������������йص��жϴ���
	while( status = ( MCF_UART_UISR(channel) & pSerial->m_bIntMode ) )
	{
		if(status & MCF_UART_UIMR_RXRDY_FU)
			M5234UartRxdInt(pSerial);
		if(status & MCF_UART_UIMR_TXRDY)
			M5234UartTxdInt(pSerial);
	}
	OS_IntUnLock(IntLev);
}

/*********************************************************************************************
 *
 * ��������     UART�жϽ��չ��̴���
 *
 * ����˵��      - pSerial	: ���� 	������ָ��
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234UartRxdInt(CSerial* pSerial)
{
	BYTE status, channel, data, flag;
	int i = 0;

	channel = pSerial->m_PortInfo.nChannel;

	//����״̬�Ĵ�����״̬�������
	while( (status = MCF_UART_USR(channel)) & MCF_UART_USR_RXRDY )
	{
		i++;
		flag = 0x0;
		data = MCF_UART_URB(channel);

		if(status & MCF_UART_USR_PE)	//У�����
		{
			flag = 0xaa;
			pSerial->m_dwPECnt++;
		}
		if(status & MCF_UART_USR_FE)	//û�м�⵽ֹͣλ
		{
			flag = 0xaa;
			pSerial->m_dwFECnt++;
		}
		if(status & MCF_UART_USR_RB)	//������ֹ����ʱ����FIFO���и���Ч�������ݽ���
		{
			flag = 0xaa;
		}
		if(status & MCF_UART_USR_OE)	//���ش���
		{
			pSerial->m_dwOECnt++;
			flag = 0xaa;
		}

		//��ȷ�Ľ�������д�봮�ڽ��ջ�����
		if(flag == 0)	
		{
			pSerial->WriteToBuf(&data, 1);
			bUartHardBuf[wUartBufPtr] = data;
			wUartBufPtr++;
			wUartBufPtr %= UART_HARD_BUF_LEN;
		}
		else
		{
			//��λ����״̬��־λ
			MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_ERROR;
		}

		//�����������ݸ��������ܳ�������FIFO����
		if(i > UART_RXD_FIFO_LEN)	
			break;
	}
	
}

/*********************************************************************************************
 *
 * ��������     UART�жϷ��͹��̴���
 *
 * ����˵��      - pSerial	: ���� 	������ָ��
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void M5234UartTxdInt(CSerial* pSerial)
{
	BYTE data, channel, status;
	int i=0;

	channel = pSerial->m_PortInfo.nChannel;

	status = MCF_UART_USR(channel);
	//����״̬�Ĵ�������ر�־���з��ʹ���
	while(MCF_UART_USR(channel) & MCF_UART_USR_TXRDY)
	{
		i++;

		//���ڷ��ͻ������գ���رշ����ж�
		if(!pSerial->GetCanSendNum())
		{
			pSerial->m_bIntMode &= ~MCF_UART_UIMR_TXRDY; //��ֹ�������Ϳ��ж�
			
			MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
			
			pSerial->SerialTxdCtr(FALSE);
			break;
		}
		
		//��������
		if(pSerial->ReadFromBuf(&data, 1))
			MCF_UART_UTB(channel) = data;


		//�����������ݸ��������ܳ�������FIFO����
		if(i>UART_TXD_FIFO_LEN)			
			break;
	}
}

/*********************************************************************************************
 *
 * ��������     ͨ��UART����һ���ֽ����ݣ��Դ��������ж�
 *
 * ����˵��      - wPort	: ���� 	���ں�
 *				 - bData	: ����	׼�����͵��ֽ�����
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartWriteAChar(WORD wPort, BYTE bData)
{
	CSerial* pSerial;
	BYTE channel;
	DWORD IntLev;

	//��鴮����ĺϷ���
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartWriteAChar",FILE_LINE,"port invalidate");
		return FALSE;
	}

	channel = pSerial->m_PortInfo.nChannel;
	
	IntLev = OS_IntLock();

	pSerial->m_bIntMode |= MCF_UART_UIMR_TXRDY;	//����׼�����ж�

	MCF_UART_UIMR(channel) = pSerial->m_bIntMode;	
	
	MCF_UART_UTB(channel) = bData;	//��������д�뷢��BUFFER

	OS_IntUnLock(IntLev);

	return TRUE;
}

/*********************************************************************************************
 *
 * ��������     UART��IO���ƽӿں�������Ҫע����������ĺ����˳��
 *
 * ����˵��      - pdwPar1	: |---MODEM_SET:		����	MODEM��������źű�־λ
 *							  |---MODEM_GET:		���	MODEM����״̬�ź�		
 *							  |---BAUD_SET:			����	׼��д��Ĳ�����ֵ		
 *							  |---BAUD_GET:			���	��ȡ�Ĳ�����ֵ	
 *							  |---COM_PAR_SET:		����	׼�����õĴ���ͨ�Ų�����־		
 *							  |---COM_PAR_GET:		���	������ͨ�Ų�����־		
 *							  |---COM_CHAN_RESET	��Ч		
 *							  |---COM_TXD_END		��Ч		
 *
 *				 - pdwPar2	: |---MODEM_SET:		����	MODEM��������źŸߵͱ�־
 *							  |---MODEM_GET:		��Ч		
 *							  |---BAUD_SET:			��Ч		
 *							  |---BAUD_GET:			��Ч
 *							  |---COM_PAR_SET:		��Ч		
 *							  |---COM_PAR_GET:		��Ч		
 *							  |---COM_CHAN_RESET	��Ч		
 *							  |---COM_TXD_END		��Ч		
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartIOCtl(WORD wPort, BYTE bCmd, DWORD *pdwPar1, DWORD *pdwPar2)
{
	CSerial* pSerial;
	BYTE channel;

	//��鴮����ĺϷ���
	pSerial = GetSerial(wPort);
	if(pSerial == NULL)
	{
		LogError("M5234UartIOCtl",FILE_LINE,"port invalidate");
		return FALSE;
	}
	//��鴮��ͨ����ŵĺϷ���
	channel = pSerial->m_PortInfo.nChannel;
	if( (channel != M5234_UART_CHAN0) && (channel != M5234_UART_CHAN1) )
	{
		LogError("M5234UartIOCtl",FILE_LINE,"channel invalidate");
		return FALSE;
	}
		
	switch(bCmd)
	{
		case MODEM_SET:	//MODEM����źſ���
			return M5234UartModemSet(channel, (BYTE)(*pdwPar1), (BYTE)(*pdwPar2));
		case MODEM_GET:	//MODEM�����źŶ�ȡ
			return M5234UartModemGet(channel, (BYTE*)pdwPar1);
		case BAUD_SET:	//����������
			return M5234UartBaudSet(channel, *pdwPar1);
		case BAUD_GET:	//������ֵ��ȡ
			return M5234UartBaudGet(pSerial, pdwPar1);
		case COM_PAR_SET://����ͨ�Ų�������
			return M5234UartParSet(channel, (BYTE)(*pdwPar1));
		case COM_PAR_GET://����ͨ�Ų�����ȡ
			return M5235UartParGet(channel, (BYTE*)pdwPar1);
		case COM_CHAN_RESET://���ڸ�λ
			return M5234UartReset(channel);
		case COM_TXD_END:	//��鷢���Ƿ���������
			return M5234UartIsTxdEnd(channel);
		default:
			return FALSE;
	}
}

/*********************************************************************************************
 *
 * ��������     MODEM�ź�������ƣ�����colefire��UART��ֻ�ܿ���RTS�ź�
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *				 - out		: ����	MODEM��������źŸ���־λ
 *				 - flag		: ����	MODEM������Ƹߵͱ�־
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartModemSet(BYTE channel, BYTE out, BYTE flag)
{
	if(flag)	//��MODEM����ź���Ч
	{
		//����ֻ��RTS�źſ��Կ��ƣ���˲���Ҫ�ж�out�еı�־λ
		MCF_UART_UOP1(channel) = MCF_UART_UOP1_RTS;	//UOP1��λ����RTS�ź���Ч
	}
	else		//��MODEM����ź���Ч
	{
		MCF_UART_UOP0(channel) = MCF_UART_UOP0_RTS;	//UOP0��λ����RTS�ź���Ч
	}
	return TRUE;
}

/*********************************************************************************************
 *
 * ��������     ��ȡMODEM�����ź�״̬������colefire��UART��ֻ�ܶ�ȡCTS�ź�
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *				 - flag		: ���	MODEM�������źŸߵ�״̬
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartModemGet(BYTE channel, BYTE *pFlag)
{
	BYTE status;

	*pFlag = 0;

	//��UART������˿ڼĴ����е�CTS״̬λ
	status = MCF_UART_UIP(channel);
	if(status & MCF_UART_UIP_CTS)
		*pFlag |= MODEM_CTS;

	return TRUE;
}

/*********************************************************************************************
 *
 * ��������     UART����������
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *				 - baud		: ����	׼�����õĲ�����ֵ
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartBaudSet(BYTE channel, DWORD baud)
{
	DWORD val;
	DWORD IntLev;
	
	val = (IN_SYS_CLOCK/baud + 16)/32;

	//���ò����ʼĴ���
	IntLev = OS_IntLock();
	MCF_UART_UBG1(channel) = HIBYTE(val);
	MCF_UART_UBG1(channel) = LOBYTE(val);
	OS_IntUnLock(IntLev);

	return TRUE;
}

/*********************************************************************************************
 *
 * ��������     UART������ֵ��ȡ��ʵ���϶�ȡ���Ǵ�����Ĳ������������Ӧ�Ĳ�����ֵ
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *				 - pBaud	: ���	��ȡ�Ĳ�����ֵ
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartBaudGet(CSerial* pSerial, DWORD *pBaud)
{
	NumToBaud(pSerial->m_serialPara.byBaud, pBaud);
	return TRUE;
}

/*********************************************************************************************
 *
 * ��������     UARTͨ�Ų������ã���������λ��У�顢ֹͣλ������
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *				 - par		: ����	׼�����õ�ͨ�Ų���
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartParSet(BYTE channel, BYTE par)
{
	if( M5234UartSetComPar(channel, par) == ERR_SUCCESS )
		return TRUE;
	else
		return FALSE;
}

/*********************************************************************************************
 *
 * ��������     ����UART��ؼĴ�����ֵ����õ�ǰ����ͨ�Ų���
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *				 - pPar		: ���	��ȡ�ĵ�ǰUARTͨ�Ų���
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5235UartParGet(BYTE channel, BYTE *pPar)
{
	BYTE umr1, umr2, par;

	par = 0;
	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//��ȡUART��ģʽ�Ĵ���
	umr1 = MCF_UART_UMR(channel);
	umr2 = MCF_UART_UMR(channel);

	//�������λ����
	switch(umr1&MCF_UART_UMR_BC_8)
	{
		case MCF_UART_UMR_BC_5:
			par |= DATA_BIT_5;
			break;
		case MCF_UART_UMR_BC_6:
			par |= DATA_BIT_6;
			break;
		case MCF_UART_UMR_BC_7:
			par |= DATA_BIT_7;
			break;
		case MCF_UART_UMR_BC_8:
			par |= DATA_BIT_8;
			break;
	}
	//���У��λ���
	switch( umr1&(0x03<<3) )
	{
		case 0:
			if(umr1&MCF_UART_UMR_PM_ODD)
				par |= ODD_PARITY;
			else
				par |= EVEN_PARITY;
			break;
		case MCF_UART_UMR_PM_NONE:
			par |= NO_PARITY;
			break;
	}
	//���ֹͣλ
	switch(umr2&0x0f)
	{
		case MCF_UART_UMR_SB_STOP_BITS_1:
			par |= STOP_BIT_1;
			break;
		case MCF_UART_UMR_SB_STOP_BITS_2:
			par |= STOP_BIT_2;
			break;
	}

	*pPar = par;
	return TRUE;
}

/*********************************************************************************************
 *
 * ��������     UART��λ����λ���͡����ա�����״̬�������´򿪷��͡����գ�����������ж�
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartReset(BYTE channel)
{
	DWORD IntLev;

	IntLev = OS_IntLock();
	//����ж����μĴ���
	MCF_UART_UIMR(channel) = 0x00;

	//�رշ��ͺͽ���
	MCF_UART_UCR(channel) = MCF_UART_UCR_TX_DISABLED;
	MCF_UART_UCR(channel) = MCF_UART_UCR_RX_DISABLED;
	
	//��λģʽ�Ĵ���ָ�룬ʹģʽ�Ĵ���ָ��ָ��UMR1n
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_MR;

	//��λ����
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_RX;

	//��λ����
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_TX;

	//��λ����״̬��־λ
	MCF_UART_UCR(channel) = MCF_UART_UCR_RESET_ERROR;
	
	//�����ͺͽ���
	MCF_UART_UCR(channel) = MCF_UART_UCR_TX_ENABLED;
	MCF_UART_UCR(channel) = MCF_UART_UCR_RX_ENABLED;

	//�򿪽����жϣ������ж�ֻ���ڷ�������֮ǰ�򿪣��ҷ�����ɺ�رշ����ж��Ա�����Ч���ж�
	MCF_UART_UIMR(channel) |= MCF_UART_UIMR_RXRDY_FU;	//����׼�����ж�

	OS_IntUnLock(IntLev);
	
	return TRUE;
}


/*********************************************************************************************
 *
 * ��������     ���UART������һ֡���������Ƿ�����������ɣ���Ϊд�뷢��FIFO�����ȷ���
 *				������ɣ�����485��ʽ��ʱ������з�����ƵĻ����ܵ��·������ݲ�����
 *
 * ����˵��      - channel	: ���� 	UARTͨ�����
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
 *
**********************************************************************************************/
BOOL M5234UartIsTxdEnd(BYTE channel)
{
	BYTE status;
	//��鷢�Ϳձ�־λ����λ��λ˵��������λ�Ĵ����ͷ��ͱ��ּĴ�����û������
	status = MCF_UART_USR(channel);
	if(status & MCF_UART_USR_TXEMP)
		return TRUE;
	else
		return FALSE;
}

#endif
