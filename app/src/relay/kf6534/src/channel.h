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

#define MODUEL_TYPE		MT_KF6534

#define PRO_TYPE			"KF6534"
#define FREQ_TYPE			FREQ_TYPE_MEA	//���

//����ԭʼ����ͨ��˳�����
//������¼��ͨ���̶�Ϊ24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29


//6534װ��ģ����ͨ��
#define CHANNEL_6534_U1			0
#define CHANNEL_6534_U2			1
#define CHANNEL_6534_U3			2
#define CHANNEL_6534_U4			3
#define CHANNEL_6534_U5			4
#define CHANNEL_6534_U6			5
#define CHANNEL_6534_U7			6
#define CHANNEL_6534_U8			7
#define CHANNEL_6534_I1			8
#define CHANNEL_6534_I2			9
#define CHANNEL_6534_I3			10
#define CHANNEL_6534_I4			11
#define CHANNEL_6534_I5			12
#define CHANNEL_6534_I6			13
#define	CHANNEL_6534_I7			14
#define CHANNEL_6534_I8			15
#define CHANNEL_6534_IC1		16
#define CHANNEL_6534_IC2		17


#define POINT_PER_PERIOD		32	//ÿ�ܲ���������
#define PERIOD_NUMBER			120	//�������ݵ��ܲ���
#define CHANNNEL_NUM			30	//ͨ������,ǰ16·ΪAD����ͨ������12·Ϊ�ϲ�ͨ��
#define AD_CHAN_NUM				16	//AD����ͨ��
#define A_COM_CHAN_NUM			8	//ģ��ϳ�ͨ������
#define D_SAMP_CHAN_NUM			6	//����¼��ͨ������
#define RELAY_SAMP_INTERVAL		7


#endif
