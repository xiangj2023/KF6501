#ifndef _CHANNEL_H_
#define _CHANNEL_H_
//#include "system.h"
#define	MT_KF6520		1
#define	MT_KF6530		2
#define	MT_KF6531		3
#define	MT_KF6533		4
#define	MT_KF6534		5
#define	MT_KF6560		6
#define	MT_KF6570		7
#define	MT_KF6571		8
#define	MT_KF6582		9

#define MODUEL_TYPE		MT_KF6582

#define PRO_TYPE			"KF6582"
#define FREQ_TYPE			FREQ_TYPE_MEA	//测控

//定义原始采样通道顺据序号
//数字量录波通道固定为24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29


//6582装置模拟量通道
#define CHANNEL_6582_U1			0
#define CHANNEL_6582_U2			1
#define CHANNEL_6582_U3			2
#define CHANNEL_6582_U4			3
#define CHANNEL_6582_U5			4
#define CHANNEL_6582_U6			5
#define CHANNEL_6582_U7			6
#define CHANNEL_6582_U8			7
#define CHANNEL_6582_I1			8
#define CHANNEL_6582_I2			9
#define CHANNEL_6582_I3			10
#define CHANNEL_6582_I4			11
#define CHANNEL_6582_I5			12
#define CHANNEL_6582_I6			13
#define	CHANNEL_6582_I7			14
#define CHANNEL_6582_I8			15
#define CHANNEL_6582_IC1		16
#define CHANNEL_6582_IC2		17

#define POINT_PER_PERIOD		32	//每周波采样点数
#define PERIOD_NUMBER			120	//保存数据的周波数
#define CHANNNEL_NUM			30	//通道个数,前16路为AD采样通道，后12路为合并通道
#define AD_CHAN_NUM				16	//AD采样通道
#define A_COM_CHAN_NUM			8	//模拟合成通道个数
#define D_SAMP_CHAN_NUM			6	//数字录波通道个数
#define RELAY_SAMP_INTERVAL		8


#endif
