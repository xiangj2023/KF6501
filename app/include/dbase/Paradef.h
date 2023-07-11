/********************************************************************************************************/
/*																										*/
/*        		天津凯发电气股份有限公司							                                    */
/*																										*/
/********************************************************************************************************/

/********************************************************************************************************/
/*																										*/
/* 文件名称                                           版本号						                    */
/*																										*/
/*      Paradef.h                                     1.0					                            */
/*																										*/
/* 描述																									*/
/*																										*/
/*   	该文件定义了系统所有参数的数据结构				                                   				*/
/*																										*/
/* 作者												                                   					*/
/*																										*/
/*      l.z											                                   					*/
/*																										*/
/* 定义的数据结构									                                   					*/
/*																										*/
/*    	TFileHead							文件头定义										    		*/
/*		TParaConfigTable					常规参数配置表								         		*/
/*		TParaDispTable						常规参数描述表									       		*/
/*		TRelayDispTable						保护描述表											   		*/
/*		TDAIPara							模拟量直接采样点参数									   	*/
/*		TDIPara								数字量直接采样点参数                                   		*/
/*		TAIPara								遥测参数													*/
/*		TBIPara								遥信参数										            */
/*		TCIPara								电度参数													*/
/*		TBOPara								遥控参数													*/
/*		TAOPara								遥调参数													*/
/*		TSerialPara							串口参数													*/
/*		TCanBusPara							CAN网参数													*/
/*		TEthPortPara						以太网参数		                                   			*/
/*		TSW_Disp							软压板描述		                                   			*/
/*		TCFG_Disp							配置字描述		                                   			*/
/*		TSET_Disp							定值描述		                                   			*/
/*		TSysSET_Disp						辅助定值描述                                   				*/
/*		TCF_Disp							系数描述		                                   			*/
/*		TRM_Disp							保护测量值描述		                                   		*/
/*		TRE_Disp							保护动作描述	                                   			*/
/*		TRW_Disp							继电器字描述	                                   			*/
/* 函数												                                   					*/
/*												                                   						*/
/*      None										                                   					*/
/*												                                   						*/
/* 头文件说明									                                   						*/
/*												                                   						*/
/*      Sysdefs.h                          基本数据类型定义                                   			*/
/*											                                   							*/
/* 修改历史										                                   						*/
/*												                                   						*/
/*      修改人           修改日期       修改内容	                                   					*/
/*												                                   						*/
/*      l.z             2008-03-15      初始创建,版本号1.0	                                   			*/
/********************************************************************************************************/
#ifndef __PARADEFS_H
#define __PARADEFS_H
#include "system.h"	/*数据类型定义*/
#include "vx_file.h"

#ifdef _BIG_ENDIAN_
#define CONVERTFLOAT		_ConvertFloat
#else
#define CONVERTFLOAT		
#endif

//#define _RELAY_DATA_FILE_
#ifdef _RELAY_DATA_FILE_
#define	RLTOBS				ltobs
#define	RLTOBL				ltobl
#else
#define	RLTOBS		
#define	RLTOBL		
#endif

#define		PARACFGFILE				"paraconf.dar"
#define		RELAYDISPFILE			"relaydisp.dar"
#define		RELAYDATAFILE			"relaydata.dar"
#define		RELAYEVENTFILE			"relayevent.dat"
#define		BASICEVENTFILE			"basicevent.dat"

//常规参数文件配置表
#define DEV_LIMIT_BO			((DWORD)(0x01<<0))    //遥控允许
#define DEV_LIMIT_AO			((DWORD)(0x01<<1))    //遥调允许
#define DEV_LIMIT_TIMING		((DWORD)(0x01<<2))    //对钟允许
#define DEV_LIMIT_AICAL			((DWORD)(0x01<<3))    //遥测上传乘系数
#define DEV_LIMIT_CICAL			((DWORD)(0x01<<4))    //电度上传乘系数
#define DEV_LIMIT_ERRREP		((DWORD)(0x01<<5))    //异常数据主动上报
#define DEV_LIMIT_CITIME		((DWORD)(0x01<<6))    //电度定时上发主站
#define DEV_LIMIT_MAKEMISC		((DWORD)(0x01<<7))    //是否产生操作事件

#define	DEV_LIMIT_FDFIRST		((DWORD)(0x01<<8))	  //故障电量按照二次数据存储
#define	DEV_LIMIT_CACLINT		((DWORD)(0x01<<9))	  //是否计算积分电度

#define HOST_NAME_LEN		16		//定义主机名长度

#define LANGUAGE_CHINESE	0
#define LANGUAGE_ENGLISH	1

typedef struct
{
	DWORD			dwDevID;
	char			chHostName[HOST_NAME_LEN];		//用于FTP访问以太网操作
	WORD			wAddress;			//本设备地址				
	WORD			wMainAddr;			//上位机地址				
	WORD			wTYPE;				//模块类型
	WORD			wCodeCrc;			//程序CRC码
	char			chPassWord1[8];		//密码1					
	char			chPassWord2[8];		//密码2					
	DWORD			dwControl;			//控制字					
										//D0=1 遥控允许
										//D1=1 遥调允许
										//D2=1 对钟允许
										//D3=1 遥测上传乘系数
										//D4=1 电度上传乘系数
										//D5=1 异常数据主动上报
										//D6=1 电度定时上发主站
										//D7=1 是否产生操作事件
										//D8=1 故障电量按照一次值存储
										//D9=1 是否计算积分电度
	WORD			wAIFCycle;			//电度冻结周期,分钟为单位
	WORD			wCIFCycle;			//遥测冻结周期,分钟为单位
//	WORD			nAIType;			//遥测类型数	
	WORD			nDI;				//直接模拟采样点个数		
	WORD			nDAI;				//直接数字采样点个数		
	WORD			nAI;				//遥测个数				
	WORD			nBI;				//遥信个数				
	WORD			nDBI;				//双点遥信个数			
	WORD			nCI;				//电度个数				
	WORD			nSPI;				//SPI个数					
	WORD			nBCD;				//BCD个数					
	WORD			nBO;				//遥控个数				
	WORD			nAO;				//遥调个数				
	WORD			nSerNum;			//串口个数				
	WORD			nCanNum;			//CAN网端口个数			
	WORD			nEthNum;			//以太网端口个数			
	WORD			nLanguageNo;		//语言编号
	WORD			Rsv[12];			//备用,预留12个字
}TParaConfigTable;

//常规参数描述文件配置表
typedef struct
{
	WORD			nAI;				//遥测个数	
	WORD			nBI;				//遥信个数	
	WORD			nCI;				//电度个数	
	WORD			nBO;				//遥控个数	
	WORD			nAO;				//遥调个数	
	WORD			Rsv[11];			//备用		
}TParaDispTable;

//保护描述文件目录
typedef struct
{
	WORD			nSW;				//软压板个数		
	WORD			nCFG;				//配置字个数		
	WORD			nSET;				//定值个数		
	WORD			nSETArea;			//定值区个数		
	WORD			nSystemSET;			//辅助定值个数	
	WORD			nRT;				//变比个数		
	WORD			nCF;				//系数个数		
	WORD			nRM;				//保护测量值个数	
	WORD			nRE;				//保护动作个数	
	WORD			nRW;				//继电器字个数	
	WORD			nDataName;			//数据名称总个数	
	WORD			nTC;				//传动
	WORD			nMBO;				//镜像位对象个数
	WORD			nFL;				//测距对象
	WORD			Rsv[2];				//备用,预留2个字节
}TRelayDispTable;

//保护数据文件目录
typedef struct
{
	WORD			nSW;				//软压板个数			
	WORD			nCFG;				//配置字个数			
	WORD			nSET;				//定值个数			
	WORD			nSETArea;			//定值区个数			
	WORD			nSETAreaCur;		//当前定值区号		
	WORD			nSETAreaCurB;		//当前定值区号反码	
	WORD			nSystemSET;			//辅助定值个数		
	WORD			nRT;				//变比个数			
	WORD			nCF;				//系数个数			
	WORD			Rsv[7];			//备用,预留14个字节
}TRelayDataTable;

//直接模拟量采样点参数
#define DAI_ATTR_DC				((WORD)(0x01<<0))    //直流回路
#define DAI_ATTR_3J				((WORD)(0x01<<1))    //三表法
#define DAI_ATTR_LINE			((WORD)(0x01<<2))    //线接入
#define DAI_ATTR_PUBPT			((WORD)(0x01<<3))    //共PT
#define DAI_ATTR_SWTPT			((WORD)(0x01<<4))    //PT切换
#define DAI_ATTR_DCD			((WORD)(0x01<<5))    //直流双端输入
#define DAI_ATTR_DC2			((WORD)(0x01<<6))    //双端第二点
#define DAI_ATTR_PLUSMASK		((WORD)(0x03<<7))    //增益

typedef struct
{
	WORD			wLineNo;			//线路号,所属线路编号
	WORD			wClass;				//属性字	
										//D0：	=0定义为交流采样回路
										//		=1定义为直流直接采样点
										//D1：	=0该回路采用两表法采集
										//		=1该回路使用三表法采集
										//D2：	=0两表法时采用相接入
										//		=1两表法时采用线接入
										//D3：	=0该回路不使用共PT方式
										//		=1该回路使用共PT方式
										//D4：	=0共PT时不进行PT切换
										//		=1共PT时进行PT切换
										//D5：	=0直流量单端输入
										//		=1直流量双端输入
										//D6：	=0双端第一点
										//		=1双端第二点
										//D7~8：=0：0~20ma
										//		=1：4~20ma；其他备用
										//D9~D15：备用
}TDAIPara;

//直接数字量采样点参数
#define DI_ATTR_TYPEMASK		((WORD)(0x0F<<0))		//类型掩码
#define DI_ATTR_SEQ				((WORD)(0x01<<8))		//序号掩码
#define DI_TYPE_INVALID			0						//无效
#define DI_TYPE_SP				1						//单点遥信
#define DI_TYPE_DP				2						//双点遥信
#define DI_TYPE_SPI				3						//SPI
#define DI_TYPE_BCD				4						//BCD
#define DI_TYPE_PLUS			5						//脉冲
#define DI_TYPE_INNER			6						//内部遥信
#define DI_TYPE_VIR				7						//虚遥信

typedef struct
{
	WORD			wClass;				//属性字,D0~3 类型：
										//			=0 无效
										//			=1 单点遥信
										//			=2 双点遥信
										//			=3 SPI 信息 如变压器分接头位置
										//			=4 BCD码信息，如水位信息
										//			=5 脉冲输入*/
										//			=6 内部遥信*/
										//			=7 虚遥信*/
										//			=8~15 预留*/
										//		D4~7 保留
										//		D8~15序号：
										//			=0双点遥信的第一点
										//			!=0双点遥信第二点或计数点号
	WORD			wFilterTime;		//去抖时间,以毫秒为单位
//	WORD			wMap;				//对应的遥信或者电度的点号
	BYTE			bInDINo;			//内部遥信的板内序号,从0开始,第一路内部遥信为信号复归
	BYTE			bInDIBoardNo;		//内部遥信所在的板号,从0开始
	WORD			wRsv;				//预留
}TDIPara;

//遥测参数
typedef struct
{
	WORD			wNameIndex;			//名称	索引号
	char			strSymbol[8];		//简称			
	char			strFirUnit1[6];		//一次数据1段单位		
	char			strFirUnit2[6];		//一次数据2段单位		
	char			strFirUnit3[6];		//一次数据3段单位
	char			strSecUnit[6];		//二次单位
	
	BYTE			nFirDec1;			//一次数据1段小数位数	
	BYTE			nFirDec2;			//一次数据2段小数位数	
	BYTE			nFirDec3;			//一次数据3段小数位数	
	BYTE			nDataRate2;			//2// 2段进制
	BYTE			nDataRate3;			// 3段进制
	BYTE			nSecDec;			//二次小数位数

	WORD			wRatioIndex;		//变比	索引号
	WORD			wControl;			//控制字D0=1 死区有效；
										//		D1=1上上限越限检查；   
										//		D2=1上限越限检查；
										//		D3=1下限越限检查；
										//		D4=1下下限越限检查
										//		D5=1 模块乘系数
										//		D6=1 管理机乘系数
	short			a;					//参数1	Y=a*x/b+c
	short			b;					//参数2	
	short			c;					//参数3	
	WORD			wDeadLine;			//死区	D15=1 相对值（单位为千分）；D15=0，绝对值
	long			lHiUpLimit;			//上上限	
	long			lUpLimit;			//上限	
	long			lDownLimit;			//下限	
	long			lLowDownLimit;		//下下限

	long			lDispLimit12;		//显示分段界限1,2
	long			lDispLimit23;		//显示分段界限2,3
	
	WORD			nCFIndex;			//系数索引号
	
	WORD			wRsv[3];			//预留
}TAIPara;

//遥信参数
#define BI_ATTR_NTPEVENT		((WORD)(0x01<<0))		//产生不带时标的事件数据
#define BI_ATTR_WTPEVENT		((WORD)(0x01<<1))		//产生带时标的事件数据
#define BI_ATTR_CONVERT			((WORD)(0x01<<2))		//进行取反操作
#define BI_ATTR_DEFAULT			((WORD)(0x01<<3))		//参与判断事故总遥信
#define BI_ATTR_VIRTUAL			((WORD)(0x01<<4))		//是虚拟遥信

typedef struct
{
	char			strSymbol[8];		//简称					
	WORD			wNameIndex;			//名称					
	WORD			wControl;			//控制字
										//	D0	1：模块产生不带时标的事件数据
										//	D1	1：模块产生带时标的事件数据
										//	D2	1：模块进行取反操作
										//	D3	1：需参与判断事故总遥信
										//	D4	1：是事故总遥信
										//	D5	1：是虚拟遥信
										//	D6~D7	备用
										//	D8	1：管理机产生不带时标的事件数据
										//	D9	1：管理机产生带时标的事件数据
										//	D10	1：管理机进行取反操作
										//	D11	1：管理机进行自动复归
										//	D12~D15	备用
	WORD			wMaptoDINo;			//	映射到DI的序号
}TBIPara;

//电度参数
#define CI_ATTR_PULSE			((WORD)(0x01<<0))		//脉冲电度
#define CI_ATTR_FRZENWITHTP		((WORD)(0x01<<1))		//产生带时标的冻结数据
#define CI_ATTR_CYCLESHIFT		3						//冻结周期偏移
#define CI_ATTR_CYCLEMASK		((WORD)(0x07<<3))		//冻结周期，5分钟为单位

typedef struct
{
	char			strSymbol[8];		//简称						
	char			strUnit[8];			//电度单位		
	WORD			wNameIndex;			//名称,索引号
	WORD			wControl;			//控制字	D0：=1脉冲电度；=0积分电度
										//			D1：=1产生带时标的冻结数据；
										//			D3~D5：冻结周期，5分钟为单位；
	DWORD			dwConst;			//脉冲常数				
	short			a;					//参数1	,	Y=a*x/b
	short			b;					//参数2	
	WORD			wMaptoDINo;			//	映射到DI的序号
	WORD			wRsv;
}TCIPara;

//遥控参数
#define BO_ATTR_VIRTUAL			((DWORD)(0x01<<31))			//虚遥控
#define BO_ATTR_TYPESHIFT		24							//虚遥控类别位置
#define BO_ATTR_TYPEMASK		((DWORD)(0x7F<<24))			//虚遥控类别掩码
#define BO_ATTR_OPTSHIFT		0							//遥控属性位置
#define BO_ATTR_OPTMASK			((DWORD)(0x03<<0))			//遥控属性掩码
#define BO_GET_OPT(a)			(((a)&BO_ATTR_OPTMASK)>>BO_ATTR_OPTSHIFT)

typedef struct
{
	char			strSymbol[8];		//简称
	DWORD			dwControl;			//控制字    D31：=1虚遥控；=0实遥控
										//			D24~D30：虚遥控类别；
										//			D0~D1：遥控属性（分/合/NULL）；
	WORD			wNameIndex;			//名称,索引号
	WORD			wSwitchNo;			//开关号,对应的继电器号或其他
	WORD			wRelayNo;			//继电器字偏移,保护虚遥控有效
	WORD			wLastTime;			//动作持续时间,单位：ms
}TBOPara;

//遥调参数
typedef struct
{
	char			strSymbol[8];		//简称		
	WORD			wNameIndex;			//名称	索引号
	WORD			wControl;			//控制字D0~D2：	
										//		=0:0~10V;
										//		=1;0~20mA; 
										//		=2:4~20mA;
	WORD			nID;				//动作号	
	short			a;					//参数1	
	short			b;					//参数2	
	short			c;					//参数3	
}TAOPara;

#define MAJOR_PORT(port) 	(HIBYTE(port))
#define MINOR_PORT(port) 	(LOBYTE(port))

/*-----------------------------------------------------------------------------------------*/
/* 端口类型定义,定义主端口类型                                                             */
/*-----------------------------------------------------------------------------------------*/
#define SC_PORT_SERIAL          0x01                      /*串口*/
#define SC_PORT_CANBUS          0x02                      /*CAN-BUS*/
#define SC_PORT_ETHERNETA       0x03                      /*以太网A*/
#define SC_PORT_ETHERNETB       0x04                      /*以太网B*/
#define SC_PORT_GPRS        	0x05                      /*CAN-BUS*/
#define SC_PORT_VIRTUAL         0x06                      /*虚拟端口*/
#define SC_PORT_SYSTEM         	0x07                      /*虚拟端口*/

//串口参数

#define SERIAL_ATTR_DATBITSHIFT			0		//数据位起始偏移
#define SERIAL_ATTR_CHKBITSHIFT			2		//校验位起始偏移
#define SERIAL_ATTR_CTLBITSHIFT			5		//校验位起始偏移

#define SERIAL_ATTR_DATBITMASK			0x03	//数据位掩码
#define SERIAL_ATTR_CHKBITMASK			0x0C	//校验位掩码
#define SERIAL_ATTR_STPBITMASK			0x10	//停止位掩码
#define SERIAL_ATTR_CTLBITMASK			0x60	//流控制位掩码

#define SERIAL_ATTR_RS485				0x0080	//485通信方式
#define SERIAL_ATTR_RTSCTR				0x0100	//RTS方向控制
#define SERIAL_ATTR_RTSHIGH				0x0200	//RTS有效位
#define SERIAL_ATTR_RTSDELAY			0x0400	//RTS延时	


#define	GET_SERIAL_DATBIT(F)			(((F)&SERIAL_ATTR_DATBITMASK)>>SERIAL_ATTR_DATBITSHIFT);
#define	GET_SERIAL_CHKBIT(F)			(((F)&SERIAL_ATTR_CHKBITMASK)>>SERIAL_ATTR_CHKBITSHIFT);
#define	GET_SERIAL_CTLBIT(F)			(((F)&SERIAL_ATTR_CTLBITMASK)>>SERIAL_ATTR_CTLBITSHIFT);

#define SERIAL_DATBIT_8					0
#define SERIAL_DATBIT_7					1
#define SERIAL_DATBIT_6					2

#define SERIAL_CHKBIT_NONE				0
#define SERIAL_CHKBIT_ODD				1
#define SERIAL_CHKBIT_EVEN				2

#define SERIAL_CTLBIT_NONE				0
#define SERIAL_CTLBIT_RTS				1
#define SERIAL_CTLBIT_DTR				2

typedef struct
{
	WORD			wPortID;			//端口标识,对应的端口ID号,类型+编号
	WORD			bFlag;				//端口标志位,考虑到具体应用，暂时不考虑同步方式
										//	D0~D1 数据位	=0 8位数据位， =1 7位数据，	=2 6位数据，   =3 5位数据
										//	D2~D3校验位	    =0无校验，=1 偶校验，=2 奇校验
										//	D4 停止位		=0 1位停止位，=1 2位停止位
										//	D5~D6 流控制位	=0无，=1RTS，=2DTR
										//	D7 通信方式		=0 RS232 ，=1 RS485
										//	D8 RTS方向控制	=0 不需要RTS方向控制;=1 需要RTS方向控制
										//	D9 RTS有效位	=0 逻辑低有效;=1 逻辑高有效
										//	D10	RTS延时		=0 RTS不需要延时;=1 RTS不需要延时
										//  其他位保留
										
	BYTE			byBaud;				//	波特率	0--NoUse,	1--150,		2--300,		3--600,		4--1200,
										//			5-240,		6--4800,	7--9600,	8-19200,	9-38400
	BYTE			byDriver;			//规约号,该端口对应的规约编号
	BYTE			byRTSOnDelay;		//RTS打开延时
	BYTE			byRTSOffDelay;		//RTS关闭延时
	BYTE			byRsv[4];			//保留				
}TSerialPara;

//CAN网参数
typedef struct
{
	WORD			wPortID;			//端口标识,对应的端口ID号,类型+编号
	WORD			wBaud;				//波特率,通信速率，以K为单位
	DWORD			dwMask;				//数据掩码,该端口对应的数据掩码,用于双网分流等控制
	BYTE			byDriver;			//规约号,该端口对应的规约编号
	BYTE			byRsv[7];			//保留
}TCanBusPara;

#define	ETHLINK_MAXLEN_USERNAME			16				
#define	ETHLINK_MAXLEN_PASSWORD			16				
#define ETHLINK_MODE_CLIENT				((DWORD)(0x01<<0))			//客户端
#define ETHLINK_MODE_UDP				((DWORD)(0x01<<1))			//UDP连接
#define ETHLINK_MODE_BACKSEND			((DWORD)(0x01<<31))			//备机发送

//以太网连接参数
typedef struct
{
	WORD			wPortID;			//端口标识,		对应的端口ID号,类型+编号
	WORD			wDriver;			//规约号,		该端口对应的规约编号
	DWORD			dwMask;				//数据掩码,		该端口对应的数据掩码，用于双网分流等控制。
	DWORD			dwSideIP;			//对方IP地址		
	DWORD			dwSideMask;			//对方IP子网掩码	
	DWORD			dwSidePort;			//对方IP端口		
	DWORD			dwSelfIP;			//自己IP地址		
	DWORD			dwSelfMask;			//自己IP子网掩码	
	DWORD			dwSelfPort;			//自己IP端口		
	DWORD			dwMode;				//通信模式	D0	=1,作为客户端,=0作为服务器端
										//			D1	=1,使用UDP方式,=0使用TCP方式		
										//			D31	=0 备机状态禁止发送,=1备机状态允许发送
	DWORD			dwGateWay1;			//第一路网关IP	
	DWORD			dwGateWay2;			//第二路网关IP
	DWORD			dwDHCPIP;			//DHCP主机IP
	char			chUser1[ETHLINK_MAXLEN_USERNAME];		//等录用户名		
	char			chPassword1[ETHLINK_MAXLEN_PASSWORD];	//登陆密码		
	char			chUser2[ETHLINK_MAXLEN_USERNAME];		//等录用户名		
	char			chPassword2[ETHLINK_MAXLEN_PASSWORD];	//登陆密码		
	char			chUser3[ETHLINK_MAXLEN_USERNAME];		//等录用户名		
	char			chPassword3[ETHLINK_MAXLEN_PASSWORD];	//登陆密码		
	char			chUser4[ETHLINK_MAXLEN_USERNAME];		//等录用户名		
	char			chPassword4[ETHLINK_MAXLEN_PASSWORD];	//登陆密码		
	DWORD			dwRsv[4];			//保留			
}TEthPortPara;

//保护软压板描述
typedef struct
{
	char			strCHName[20];		//中文名称		用于液晶显示和维护软件查看
}TSW_Disp;

//保护配置字描述
typedef struct
{
	char			strCHName[20];		//中文名称		用于液晶显示和维护软件查看
}TCFG_Disp;

//保护定值描述
#define SETCFG_INVALID		0xFF			//无效的关联配置号
#define SETNAME_INVALID		0xFFFF			//无效的定值名称索引
#define SETNAME_MAXLENGTH	20				//每个定值描述的最大长度
#define SETCFG_MAXNUM		32				//最大定值关联配置字个数
typedef struct
{
	char			strCHName[20];		//中文名称		用于液晶显示和维护软件查看
	char			strSymbol[16];		//简称				
	float			fMaxValue;			//最大值			
	float			fMinValue;			//最小值			
	char			strUnit[8];			//单位				
	WORD			nCFG;				//关联配置字个数
	WORD			nItemStart;			//定值描述条目起始
	WORD			nItemNum;			//定值描述条目个数
	BYTE			CFGList[SETCFG_MAXNUM];//配置字表
	WORD			nDec;				//小数点位数		液晶显示用
	WORD			nCFIndex;			//系数索引号
	WORD			wRsv[3];			//保留
}TSET_Disp;

//保护辅助定值描述
typedef struct
{
	char			strCHName[20];		//中文名称		用于液晶显示和维护软件查看
	char			strSymbol[16];		//简称				
	float			fMaxValue;			//最大值			
	float			fMinValue;			//最小值			
	char			strUnit[8];			//单位				
	WORD			nItemStart;			//定值描述条目起始
	WORD			nItemNum;			//定值描述条目个数
	BYTE			nDec;				//小数点位数		液晶显示用
	BYTE			nCFIndex;			//系数索引号
	BYTE			byRsv[6];
}TSysSET_Disp;

//保护系数描述
typedef struct
{
	char			strSymbol[8];		//简称			
	WORD			wCof;				//放大倍数		
	WORD			wRsv;
}TCF_Disp;

//保护测量值描述
#define	RT_INDEX_INVALID					0xFFFF
#define	RM_ATTR_HEX							0x0001
typedef struct
{
	char			strSymbol[8];		//简称			
	char			strFirUnit[6];		//一次单位		
	char			strSecUnit[6];		//二次单位		
	BYTE			nFirDec;			//一次小数位数	
	BYTE			nSecDec;			//二次小数位数
	WORD			nIndex;				//电量索引号,全局索引号，故障电量全局大表，用作搜索键值，不用下到装置里
	WORD			nRTIndex;			//变比索引号,0xFFFF代表二次数据
	WORD			wAttr;				//数值类型,10进制还是16进制等
	WORD			nCFIndex;			//系数索引号
	BYTE			nFirRate;			//一次数据进制
	BYTE			byRsv[5];			//保留
}TRM_Disp;

#define		MAX_FD_NUM						48			//保护动作最大故障电量个数

//保护动作描述
typedef struct
{
	char			strCHName[20];		//中文名称		用于液晶显示和维护软件查看
	WORD			wInf;					//	报告索引号 全局索引号
	WORD			nFD;					//	故障电量个数		
//	WORD			wFDList[MAX_FD_NUM];	//	故障电量索引表
	WORD			wRMList[MAX_FD_NUM];	//	保护测量值索引表 便于LED获取保护测量值描述信息
}TRE_Disp;

#define DIST_CAPACITY_MAX					16
#define DIST_ANALOGCHN_MAX					24
#define DIST_DIGITALCHN_MAX					96
#define DIST_SAMPLENUM_MAX					8000

//录波参数描述
typedef struct
{
	char 	strDevName[64];					//	装置名称		
	WORD	wSampleRate; 					//	采样频率		
	WORD	wSysFrequency;					//	系统频率
	DWORD	dwTotalSampleNum;				//	每次录波采样点数
	DWORD	dwPrevSampleNum;				//	触发前采样点数
	WORD	wRecordCapacity; 				//	总录波容量
	WORD	wAnalogChannelNum;				//	模拟量通道个数
	WORD	wDigitalChannelNum;				//	数字通道个数
	WORD	wRvs;
}TDist_Disp;

//模拟通道参数
typedef struct
{
	char	strName[16];					//	通道名称
	char	strUnit[8];						//	数据单位
	WORD	wIndex;							//	通道索引
	WORD	wCoefIndex;						//	系数索引
	WORD	wCompressMultiple;				//	压缩倍率
	WORD	wRTIndex;						//	系数索引
	float	fCFRatio;						//  系数比率
	BYTE	byRsv[8];						//	备用	
}TAC_Disp;

//数字通道参数
typedef struct
{
	char	strName[8];						//通道名称	
	WORD	wRelayMacro;					//继电器字宏
	WORD	wRsv;							//预留字	
}TDC_Disp;

//传动参数
typedef struct
{
	char			strCHName[20];		//中文名称		用于液晶显示和维护软件查看
}TTC_Disp;

//镜像位对象
#define MAX_MAPBITOBJ_NUM			32

#define MAPBIT_LISTEN_PORT			6500	//镜像位端口
#define MAPBIT_PORTTYPE_ETHA				0			
#define MAPBIT_PORTTYPE_ETHB				1			
#define MAPBIT_PORTTYPE_CANA				2			
#define MAPBIT_PORTTYPE_CANB				3			
#define MAPBIT_PORTTYPE_SERIAL1				4			
#define MAPBIT_PORTTYPE_SERIAL2				5			
#define MAPBIT_PORTTYPE_SERIAL3				6			

typedef struct
{
	char			strStationName[20];	//站名
	WORD			wControl;			//控制字
										//D0~D3 0:ETHA；	1:ETHB；	2:CAN1; 	
										//		3:CAN2; 	4:Serial1	5:Serial2	6:Serial3; 	其他保留   
										//D15   0:无效;  1:有效 	
										//其他保留
										
	BYTE			bySelfAddr;			//本机站址
	BYTE			bySideAddr;			//对端站址
	DWORD			dwSideIP;			//对端IP地址
	DWORD			dwBaud;				//波特率
	BYTE			bySMBStart;			//发送镜像位起始位置
	BYTE			bySMBNum;			//发送镜像位个数
	BYTE			byRMBStart;			//接收镜像位起始位置
	BYTE			byRMBNum;			//接收镜像位个数
 	DWORD			dwRsv;
}TMBO_Disp;


#define FL_CTRLMASK_STEPOVERVAL	0x8000
#define FL_CTRLMASK_SIDE		0x0080
#define FL_CTRLMASK_SUBNO		0x007F

typedef struct {
		WORD	wControl;		//D15：=1，越区时投入；=0，越区时退出 
								//D12~D14: 连接方式
								// 0=以太网
								// 1=串口2
								// 2=路由
								// 其他保留
								// D8: 1,对侧，0，本测    (应该是D7)
								// D0~D3: 子所编号,1,本侧AT，2本侧SP，3对侧SP，4对侧AT，5，对侧SS
								//其他位保留
		BYTE	byNetAddr;		//子网通信地址
		BYTE	byFLNo;			//测距编号
		DWORD 	dwMNetIP;		//主网IP地址
		DWORD 	dwSNetIP;		//备网IP地址
		DWORD	dwRsv;			//保留
}TFL_Disp;

#endif
