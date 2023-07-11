/********************************************************************************************************/
/*																										*/
/*        		��򿭷������ɷ����޹�˾																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* �ļ�����                                           �汾��											*/
/*																										*/
/*      port.cpp                                        1.0                         					*/
/*																										*/
/* ����																									*/
/*																										*/
/*      ���ļ�����˿ڹ����ϵ�к���                                                    				*/
/*																										*/
/* ����																									*/
/*																										*/
/*      ����																							*/
/*																										*/
/* ��������ݽṹ																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* ����																									*/
/*																										*/
/*    OpenComm                          �򿪶˿�                                    					*/
/*    ReadComm                          ���˿ڽ��ջ���������                       	 					*/
/*    WriteComm                         ��˿ڷ��ͻ�����д����                      					*/
/*    CloseComm                         �رն˿�                                    					*/
/*    CtrlComm                          ���ƶ˿�                                    					*/
/*																										*/
/* ͷ�ļ�˵��																							*/
/*																										*/
/*      system.h                       	ϵͳͷ�ļ�														*/
/*																										*/
/* �޸���ʷ																								*/
/*																										*/
/*      �޸���           �޸�����       �޸�����														*/
/*																										*/
/*      l.z             2008-07-15      ��ʼ����,�汾��1.0												*/
/********************************************************************************************************/
#include "system.h"
#include "parafunc.h"

#if (INSTALL_SERIAL==YES)
#include "misi.h"
#endif

#if (INSTALL_CANBUS==YES)
#include "canlink.h"
#endif

#if (INSTALL_NET==YES)
#include "netentry.h"
#endif

//�˿�ӳ���
STATIC THashTable 	*g_hashPortMap=NULL;

/*------------------------------------------------------------------------------------------*/
/* ����:�򿪶˿�,�ṩ����Լ�������Ľӿ�                                                   */
/* ����:port �˿ںţ�txd ���ͻ��������ȣ�rxd ���ջ��������ȣ�mode ��ģʽ                  */
/* ���:��                                                                                  */
/* ����:ERR_SUCCESS �򿪳ɹ�,ERR_SERIAL_CHANNEL �򿪶˿ڴ���                                */
/*------------------------------------------------------------------------------------------*/
DWORD OpenComm(WORD port,WORD txdLen,WORD rxdLen,DWORD flag)
{
    BYTE portType,portSort;
    DWORD status;
    /*ȡ�˿�����*/
    portType = MAJOR_PORT(port);
    /*ȡ�˿ڱ��*/
    portSort = MINOR_PORT(port);
    
    switch(portType)
    {
#if (INSTALL_SERIAL==YES)
    case SC_PORT_SERIAL:
        status = MisiOpen(portSort,rxdLen,txdLen,flag);
        break;
#endif
#if (INSTALL_NET==YES)
    case SC_PORT_ETHERNETA:
        status = EthernetOpen(port,rxdLen,txdLen,flag);
        break;
    case SC_PORT_ETHERNETB:
        status = EthernetOpen(port,rxdLen,txdLen,flag);
        break;
#endif
#if (INSTALL_CANBUS==YES)
    case SC_PORT_CANBUS:
        status = CanOpen(portSort,flag);
        break;
#endif
  default:
        LogError("OpenComm",FILE_LINE,"not support this port");
        status = ERR_SERIAL_CHANNEL;
        break;
    }
    return status;
}
/*------------------------------------------------------------------------------------------*/
/* ����:���˿ڽ��ջ���������,�ṩ����Լ�������Ľӿ�                                       */
/* ����:port �˿ںţ�buf ��Ž������ݵ�ָ�룻len Ҫ����ĳ��ȣ���ģʽ                       */
/* ���:buf �����Ļ���������                                                                */
/* ����:ʵ�ʶ����ĳ���                                                                      */
/*------------------------------------------------------------------------------------------*/
DWORD ReadComm(WORD port,PBYTE buf,WORD len,DWORD flag)
{
    BYTE portType,portSort;
    DWORD readLen;
    portType = MAJOR_PORT(port);
    portSort = MINOR_PORT(port);
    switch(portType)
    {
#if (INSTALL_SERIAL==YES)
    case SC_PORT_SERIAL:
        readLen = MisiRead(portSort,buf,len,flag);
        break;
#endif
#if (INSTALL_NET==YES)
    case SC_PORT_ETHERNETA:
        readLen = EthernetRead(port,buf,len,flag);
        break;
    case SC_PORT_ETHERNETB:
        readLen = EthernetRead(port,buf,len,flag);
        break;
#endif
#if (INSTALL_CANBUS==YES)
    case SC_PORT_CANBUS:
        readLen = CanRead(portSort,buf,len,flag);
        break;
#endif
  default:
        LogError("ReadComm",FILE_LINE,"not support this port");
        readLen=0;
        break;
    }
    return readLen;
}
/*------------------------------------------------------------------------------------------*/
/* ����:��˿ڷ��ͻ�����д����,�ṩ����Լ�������Ľӿ�                                     */
/* ����:port �˿ںţ�buf д���ݵ���ָ�룻len ����д�ĳ��ȣ�дģʽ                           */
/* ���:��                                                                                  */
/* ����:ʵ��д�˿ڵĳ���                                                                    */
/*------------------------------------------------------------------------------------------*/
DWORD WriteComm(WORD port,PBYTE buf,WORD len,DWORD flag)
{
    BYTE portType,portSort;
    DWORD writeLen;
	
    portType = MAJOR_PORT(port);
    portSort = MINOR_PORT(port);
	
    switch(portType)
    {
#if (INSTALL_SERIAL==YES)
    case SC_PORT_SERIAL:
        writeLen = MisiWrite(portSort,buf,len,flag);
        break;
#endif
#if (INSTALL_NET==YES)
    case SC_PORT_ETHERNETA:
        writeLen = EthernetWrite(port,buf,len,flag);
        break;
    case SC_PORT_ETHERNETB:
        writeLen = EthernetWrite(port,buf,len,flag);
        break;
#endif
#if (INSTALL_CANBUS==YES)
    case SC_PORT_CANBUS:
        writeLen = CanWrite(portSort,buf,len,flag);
        break;
#endif
  default:
        LogError("WriteComm",FILE_LINE,"not support this port");
        writeLen=0;
        break;
    }
    return writeLen;
}
/*------------------------------------------------------------------------------------------*/
/* ����:�رն˿�,�ṩ����Լ�������Ľӿ�                                                   */
/* ����:port �˿ں�                                                                         */
/* ���:��                                                                                  */
/* ����:ERR_SUCCESS �رճɹ�,ERR_SERIAL_CHANNEL �رն˿�ʧ��                                */
/*------------------------------------------------------------------------------------------*/

DWORD CloseComm(WORD port)
{
    BYTE 	portType,portSort;
    DWORD 	status;
	
    portType = MAJOR_PORT(port);
    portSort = MINOR_PORT(port);
	
    switch(portType)
    {
#if (INSTALL_SERIAL==YES)
    case SC_PORT_SERIAL:
//      status = MisiClose(portSort);
        break;
#endif
#if (INSTALL_NET==YES)
    case SC_PORT_ETHERNETA:
        status = EthernetClose(port);
        break;
    case SC_PORT_ETHERNETB:
        status = EthernetClose(port);
        break;
#endif
#if (INSTALL_CANBUS==YES)
    case SC_PORT_CANBUS:
//      status = CanClose(portSort);
        break;
#endif
  default:
        LogError("OpenClose",FILE_LINE,"not support this port");
        status = ERR_SERIAL_CHANNEL;
        break;
    }
    return status;
}
/*------------------------------------------------------------------------------------------*/
/* ����:���ƶ˿�,�ṩ����Լ�������Ľӿ�                                                   */
/* ����:port �˿ں�;TCommCtl ���ƽṹ                                                       */
/* ���:��                                                                                  */
/* ����:ERR_SUCCESS �رճɹ�,ERR_SERIAL_CHANNEL �رն˿�ʧ��,ERR_SUPPORT�˿ڲ�֧�ָù���    */
/*------------------------------------------------------------------------------------------*/
DWORD CtrlComm(WORD port,TCtlComm *comm)
{
    BYTE 	portType,portSort;
    DWORD 	status;
	
    portType = MAJOR_PORT(port);
    portSort = MINOR_PORT(port);
    switch(portType)
    {
#if (INSTALL_SERIAL==YES)
    case SC_PORT_SERIAL:
        status = MisiCtrl(portSort,comm);
        break;
#endif
#if (INSTALL_NET==YES)
    case SC_PORT_ETHERNETA:
        status = EthernetControl(port,(char*)comm);
        break;
    case SC_PORT_ETHERNETB:
        status = EthernetControl(port,(char*)comm);
        break;
#endif
#if (INSTALL_CANBUS==YES)
    case SC_PORT_CANBUS:
      status = CanCtrl(portSort,comm);
        break;
#endif
  default:
        LogError("CtrlComm",FILE_LINE,"not support this port");
        status = ERR_SERIAL_CHANNEL;
        break;
    }
    return status;
}

/*------------------------------------------------------------------------------------------*/
/* ����: ȡ�˿���Ϣ                                                                         */
/* ����: port �˿�ID��                                                                      */
/* ���: ��                                                                                 */
/* ����: ��                                                                                 */
/*------------------------------------------------------------------------------------------*/
STATUS GetPortPara(WORD port,void *pPortPara)
{
    BYTE 		portType,portSort;
	STATUS		rc;
	
    portType = MAJOR_PORT(port);
    portSort = MINOR_PORT(port);
	
    switch(portType)
    {
#if (INSTALL_SERIAL==YES)
		case SC_PORT_SERIAL:
			rc = DB_GetSerialPara(portSort-1,(TSerialPara *)pPortPara);
			break;
#endif
#if (INSTALL_NET==YES)
		case SC_PORT_ETHERNETA:
		case SC_PORT_ETHERNETB:
			rc = DB_GetETHPara(portSort-1,(TEthPortPara *)pPortPara);
			break;
#endif
#if (INSTALL_CANBUS==YES)
		case SC_PORT_CANBUS:
			rc = DB_GetCANPara(portSort-1,(TCanBusPara *)pPortPara);
			break;
#endif
		default:
			LogError("CtrlComm",FILE_LINE,"not support this port");
			rc = DB_ERR_ID;
			break;
    }

	return rc; 
}

/*------------------------------------------------------------------------------------------*/
/* ����: ȡ�˿���������                                                                         */
/* ����: port �˿�ID��                                                                      */
/* ���: ��������                                                                                 */
/* ����: ERR_SUCCESS �ɹ�,ʧ�ܷ��ش�����                                */
/*------------------------------------------------------------------------------------------*/
STATUS GetPortDataMask(WORD port,DWORD &dwMask)
{
	TCanBusPara		tCanPara;
	TEthPortPara	tEthPara;
	BYTE 			portType,portSort;
	STATUS			rc = DB_ERR_ID;

	portType = MAJOR_PORT(port);
	portSort = MINOR_PORT(port);

	switch(portType)
	{
#if (INSTALL_NET==YES)
		case SC_PORT_ETHERNETA:
		case SC_PORT_ETHERNETB:
			rc = GetPortPara(port,&tEthPara);
			dwMask = tEthPara.dwMask;
			break;
#endif
#if (INSTALL_CANBUS==YES)
		case SC_PORT_CANBUS:
			rc = GetPortPara(port,&tCanPara);
			dwMask = tCanPara.dwMask;
			break;
#endif
	  default:
			break;
	}
	
	return rc;
}

