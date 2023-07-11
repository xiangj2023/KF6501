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

#define MODUEL_TYPE		MT_KF6571

#define PRO_TYPE			"KF6571"
#define PRO_6571            //装置类型
#define FREQ_TYPE			FREQ_TYPE_PRO	//测控装置


//定义原始采样通道顺据序号
//数字量录波通道固定为24-27
#define CHANNEL_DC_1			16
#define CHANNEL_DC_2			17

//6571装置模拟量通道
#define CHANNEL_6571_U1			0
#define CHANNEL_6571_U2			1
#define CHANNEL_6571_U3			2
#define CHANNEL_6571_U4			3
#define CHANNEL_6571_IT1		4
#define CHANNEL_6571_IF1		5
#define CHANNEL_6571_IT2		6
#define CHANNEL_6571_IF2		7
#define CHANNEL_6571_IT3		8
#define CHANNEL_6571_IF3		9
#define CHANNEL_6571_IAT1		10
#define CHANNEL_6571_IAT2		11
#define CHANNEL_6571_IP1		12
#define CHANNEL_6571_IP2		13
#define CHANNEL_6571_IP3		14
//增加开关量变换遥测，由虚拟开入的前8路按顺序变换而成，第1路虚拟开入对应最低位
#define CHANNEL_6571_CB			15  

#define POINT_PER_PERIOD		20	//每周波采样点数
#define PERIOD_NUMBER			3000	//保存数据的周波数
#define CHANNNEL_NUM			18	//通道总个数
#define AD_CHAN_NUM				12	//AD采样通道
#define A_COM_CHAN_NUM			4	//模拟合成通道个数
#define D_SAMP_CHAN_NUM			2	//数字录波通道个数
#define RELAY_SAMP_INTERVAL		9

#endif

