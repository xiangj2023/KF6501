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

#define MODUEL_TYPE		MT_KF6570

#define PRO_TYPE			"KF6570"
#define PRO_6570
#define FREQ_TYPE			FREQ_TYPE_MEA	//���װ��

//����ԭʼ����ͨ��˳�����
//������¼��ͨ���̶�Ϊ24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29


//6570װ��ģ����ͨ��
#define CHANNEL_6570_UHA		0
#define CHANNEL_6570_UHB		1
#define CHANNEL_6570_UHC		2
#define CHANNEL_6570_U0			3
#define CHANNEL_6570_ULA		4
#define CHANNEL_6570_ULB		5
#define CHANNEL_6570_ULC		6
#define CHANNEL_6570_IHA		7
#define CHANNEL_6570_IHB		8
#define CHANNEL_6570_IHC		9
#define CHANNEL_6570_ILA		10
#define CHANNEL_6570_ILB		11
#define CHANNEL_6570_ILC		13
#define CHANNEL_6570_I1			14
#define CHANNEL_6570_I2			15

#define POINT_PER_PERIOD		32	//ÿ�ܲ���������
#define PERIOD_NUMBER			120	//�������ݵ��ܲ���
#define CHANNNEL_NUM			30	//ͨ������,ǰ16·ΪAD����ͨ������12·Ϊ�ϲ�ͨ��
#define AD_CHAN_NUM				16	//AD����ͨ��
#define A_COM_CHAN_NUM			8	//ģ��ϳ�ͨ������
#define D_SAMP_CHAN_NUM			6	//����¼��ͨ������
#define RELAY_SAMP_INTERVAL		6


#endif
