#ifndef _SAMPLE_H_
#define _SAMPLE_H_
#include "system.h"
#include "channel.h"
//定义原始采样通道顺据序号
//数字量录波通道固定为24-27


#define COMP_CHAN_NUM			(A_COM_CHAN_NUM+D_SAMP_CHAN_NUM)//合成通道个数,包括8个模拟合成通道和6个数字合成通道

#if(AD_DEV_TYPE == MAX11046_DEV)
	#define CHANNEL_PER_AD			8	//每个AD的通道个数
#elif(AD_DEV_TYPE == AD7656_DEV)
	#define CHANNEL_PER_AD			6	//每个AD的通道个数
#elif(AD_DEV_TYPE == AD7606_DEV)
	#define CHANNEL_PER_AD			8	//每个AD的通道个数
#endif

#define READ_POINT_NUM			(POINT_PER_PERIOD+2)	//每次读取的采样点数
#define READ_SAMPLE_LEN			(READ_POINT_NUM*CHANNNEL_NUM)

#define DISP_DATA_LEN			(POINT_PER_PERIOD*CHANNNEL_NUM)	//显示缓冲区长度

//定义采样缓冲区长度
#define SAMPLE_DATA_LEN			(POINT_PER_PERIOD*PERIOD_NUMBER*CHANNNEL_NUM)

//定义每周波采样数据长度
#define SAM_LEN_PER_CYC			(POINT_PER_PERIOD*CHANNNEL_NUM)

//定义保护处理相对于采样中断的倍数
#define RELAY_PROC_INTERVAL		1

//定义保护处理相对于采样中断的时间倍数
//#define RELAY_SAMP_INTERVAL		13

//对于按照采样间隔取数，定义每次读取的周波数，即当前周波、上周波和上上周波
#define READ_SAMP_CYC_NUM		3
#define FREQUENCE_SAMP_NUM		POINT_PER_PERIOD*4	

#define DEV_FREQUENCE 			50
#define SAMP_PER_SECOND 		(DEV_FREQUENCE*POINT_PER_PERIOD)


#endif
