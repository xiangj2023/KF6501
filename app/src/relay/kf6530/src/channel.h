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

#define MODUEL_TYPE		MT_KF6530

#define PRO_TYPE			"KF6530"
#define FREQ_TYPE			FREQ_TYPE_PRO	//装置类型

//定义原始采样通道顺据序号
//数字量录波通道固定为24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29

//6530装置模拟通道
#define CHANNEL_6530_UA			0
#define CHANNEL_6530_UB			1
#define CHANNEL_6530_UC			2

#define CHANNEL_6530_IHA		3
#define CHANNEL_6530_IHB		4
#define CHANNEL_6530_IHC		5
#define CHANNEL_6530_IL1		6		//IL1
#define CHANNEL_6530_IL2		7		//IL2
#define CHANNEL_6530_IL3		8		//IL3
#define CHANNEL_6530_IL4		9		//IL4
#define	CHANNEL_6530_IDA		16
#define	CHANNEL_6530_IRA		17
#define	CHANNEL_6530_IDB		18
#define	CHANNEL_6530_IRB		19
#define	CHANNEL_6530_IDC		20
#define	CHANNEL_6530_IRC		21

#define POINT_PER_PERIOD		32	//每周波采样点数
#define PERIOD_NUMBER			120	//保存数据的周波数
#define CHANNNEL_NUM			30	//通道个数,前16路为AD采样通道，后12路为合并通道
#define AD_CHAN_NUM				16	//AD采样通道
#define A_COM_CHAN_NUM			8	//模拟合成通道个数
#define D_SAMP_CHAN_NUM			6	//数字录波通道个数
#define RELAY_SAMP_INTERVAL		7


#endif
