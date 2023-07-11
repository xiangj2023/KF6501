/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司																*/
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号											*/
/*																										*/
/*      port.cpp                                        1.0                         					*/
/*																										*/
/* 描述																									*/
/*																										*/
/*      该文件定义端口管理的系列函数                                                    				*/
/*																										*/
/* 作者																									*/
/*																										*/
/*      李政																							*/
/*																										*/
/* 定义的数据结构																						*/
/*																										*/
/*      None																							*/
/*																										*/
/* 函数																									*/
/*																										*/
/*    OpenComm                          打开端口                                    					*/
/*    ReadComm                          读端口接收缓冲区内容                       	 					*/
/*    WriteComm                         向端口发送缓冲区写数据                      					*/
/*    CloseComm                         关闭端口                                    					*/
/*    CtrlComm                          控制端口                                    					*/
/*																										*/
/* 头文件说明																							*/
/*																										*/
/*      system.h                       	系统头文件														*/
/*																										*/
/* 修改历史																								*/
/*																										*/
/*      修改人           修改日期       修改内容														*/
/*																										*/
/*      l.z             2008-07-15      初始创建,版本号1.0												*/
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

//端口映射表
STATIC THashTable 	*g_hashPortMap=NULL;

/*------------------------------------------------------------------------------------------*/
/* 功能:打开端口,提供给规约任务程序的接口                                                   */
/* 输入:port 端口号；txd 发送缓冲区长度；rxd 接收缓冲区长度；mode 打开模式                  */
/* 输出:无                                                                                  */
/* 返回:ERR_SUCCESS 打开成功,ERR_SERIAL_CHANNEL 打开端口错误                                */
/*------------------------------------------------------------------------------------------*/
DWORD OpenComm(WORD port,WORD txdLen,WORD rxdLen,DWORD flag)
{
    BYTE portType,portSort;
    DWORD status;
    /*取端口类型*/
    portType = MAJOR_PORT(port);
    /*取端口编号*/
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
/* 功能:读端口接收缓冲区内容,提供给规约任务程序的接口                                       */
/* 输入:port 端口号；buf 存放接收数据的指针；len 要求读的长度；读模式                       */
/* 输出:buf 读到的缓冲区数据                                                                */
/* 返回:实际读到的长度                                                                      */
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
/* 功能:向端口发送缓冲区写数据,提供给规约任务程序的接口                                     */
/* 输入:port 端口号；buf 写数据的首指针；len 请求写的长度；写模式                           */
/* 输出:无                                                                                  */
/* 返回:实际写端口的长度                                                                    */
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
/* 功能:关闭端口,提供给规约任务程序的接口                                                   */
/* 输入:port 端口号                                                                         */
/* 输出:无                                                                                  */
/* 返回:ERR_SUCCESS 关闭成功,ERR_SERIAL_CHANNEL 关闭端口失败                                */
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
/* 功能:控制端口,提供给规约任务程序的接口                                                   */
/* 输入:port 端口号;TCommCtl 控制结构                                                       */
/* 输出:无                                                                                  */
/* 返回:ERR_SUCCESS 关闭成功,ERR_SERIAL_CHANNEL 关闭端口失败,ERR_SUPPORT端口不支持该功能    */
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
/* 功能: 取端口信息                                                                         */
/* 输入: port 端口ID号                                                                      */
/* 输出: 无                                                                                 */
/* 返回: 无                                                                                 */
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
/* 功能: 取端口数据掩码                                                                         */
/* 输入: port 端口ID号                                                                      */
/* 输出: 数据掩码                                                                                 */
/* 返回: ERR_SUCCESS 成功,失败返回错误码                                */
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

