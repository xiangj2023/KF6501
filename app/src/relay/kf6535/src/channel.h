#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#define	MT_KF6520		1
#define	MT_KF6530		2
#define	MT_KF6531		3
#define	MT_KF6533		4
#define	MT_KF6534		5
#define	MT_KF6560		6
#define	MT_KF6570		7
#define	MT_KF6571		8
#define	MT_KF6582		9
#define MT_KF6521		10
#define MT_KF6561		11
#define MT_KF6535		12


#define MODUEL_TYPE		MT_KF6535

#define PRO_TYPE			"KF6535"
#define PRO_6535			//装置类型
#define FREQ_TYPE			FREQ_TYPE_PRO	//装置类型

//定义原始采样通道顺据序号
//数字量录波通道固定为27-32
#define CHANNEL_DC_1			27
#define CHANNEL_DC_2			28
#define CHANNEL_DC_3			29
#define CHANNEL_DC_4			30
//#define CHANNEL_DC_5			31
//#define CHANNEL_DC_6			32

//6530装置模拟通道
#define CHANNEL_6535_UA			0
#define CHANNEL_6535_UB			1
#define CHANNEL_6535_UC			2
#define CHANNEL_6535_U0			3

#define CHANNEL_6535_IHA		4
#define CHANNEL_6535_IHB		5
#define CHANNEL_6535_IHC		6
#define CHANNEL_6535_IMA		7
#define CHANNEL_6535_I0			8
#define CHANNEL_6535_IMB		9
#define CHANNEL_6535_IMC		10
#define CHANNEL_6535_Ijx		11
#define CHANNEL_6535_ILA		12
#define CHANNEL_6535_ILB		13
#define CHANNEL_6535_ILC		14
#define	CHANNEL_6535_IDA		15
#define	CHANNEL_6535_IDB		16
#define	CHANNEL_6535_IDC		17
#define	CHANNEL_6535_IRA_1		18
#define	CHANNEL_6535_IRA_2		19
#define	CHANNEL_6535_IRA_3		20
#define	CHANNEL_6535_IRB_1		21
#define	CHANNEL_6535_IRB_2		22
#define	CHANNEL_6535_IRB_3		23
#define	CHANNEL_6535_IRC_1		24
#define	CHANNEL_6535_IRC_2		25
#define	CHANNEL_6535_IRC_3		26

#define POINT_PER_PERIOD		32	//每周波采样点数
#define PERIOD_NUMBER			120	//保存数据的周波数
#define CHANNNEL_NUM			31	//通道个数,前15路为AD采样通道，后12路为合并通道
#define AD_CHAN_NUM				15	//AD采样通道
#define A_COM_CHAN_NUM			12	//模拟合成通道个数
#define D_SAMP_CHAN_NUM			4	//数字录波通道个数
#define RELAY_SAMP_INTERVAL		8


#endif
