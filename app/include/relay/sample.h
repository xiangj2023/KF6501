#ifndef _SAMPLE_H_
#define _SAMPLE_H_
#include "system.h"
#include "channel.h"
//����ԭʼ����ͨ��˳�����
//������¼��ͨ���̶�Ϊ24-27


#define COMP_CHAN_NUM			(A_COM_CHAN_NUM+D_SAMP_CHAN_NUM)//�ϳ�ͨ������,����8��ģ��ϳ�ͨ����6�����ֺϳ�ͨ��

#if(AD_DEV_TYPE == MAX11046_DEV)
	#define CHANNEL_PER_AD			8	//ÿ��AD��ͨ������
#elif(AD_DEV_TYPE == AD7656_DEV)
	#define CHANNEL_PER_AD			6	//ÿ��AD��ͨ������
#elif(AD_DEV_TYPE == AD7606_DEV)
	#define CHANNEL_PER_AD			8	//ÿ��AD��ͨ������
#endif

#define READ_POINT_NUM			(POINT_PER_PERIOD+2)	//ÿ�ζ�ȡ�Ĳ�������
#define READ_SAMPLE_LEN			(READ_POINT_NUM*CHANNNEL_NUM)

#define DISP_DATA_LEN			(POINT_PER_PERIOD*CHANNNEL_NUM)	//��ʾ����������

//�����������������
#define SAMPLE_DATA_LEN			(POINT_PER_PERIOD*PERIOD_NUMBER*CHANNNEL_NUM)

//����ÿ�ܲ��������ݳ���
#define SAM_LEN_PER_CYC			(POINT_PER_PERIOD*CHANNNEL_NUM)

//���屣����������ڲ����жϵı���
#define RELAY_PROC_INTERVAL		1

//���屣����������ڲ����жϵ�ʱ�䱶��
//#define RELAY_SAMP_INTERVAL		13

//���ڰ��ղ������ȡ��������ÿ�ζ�ȡ���ܲ���������ǰ�ܲ������ܲ��������ܲ�
#define READ_SAMP_CYC_NUM		3
#define FREQUENCE_SAMP_NUM		POINT_PER_PERIOD*4	

#define DEV_FREQUENCE 			50
#define SAMP_PER_SECOND 		(DEV_FREQUENCE*POINT_PER_PERIOD)


#endif
