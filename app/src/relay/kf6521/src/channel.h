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
#define	MT_KF6521		10


#define MODUEL_TYPE		MT_KF6521

#define PRO_TYPE			"KF6521"		//װ������
#define FREQ_TYPE			FREQ_TYPE_MEA	//װ������

//����ԭʼ����ͨ��˳�����
//������¼��ͨ���̶�Ϊ24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29


//6520װ��ģ����ͨ��
#define CHANNEL_6521_UA			0
#define CHANNEL_6521_UB			1
#define CHANNEL_6521_UC			2
#define CHANNEL_6521_U0			3
#define CHANNEL_6521_U1			4
#define CHANNEL_6521_U2			5
#define CHANNEL_6521_U3			6

#define CHANNEL_6521_IAM		8
#define CHANNEL_6521_IBM		9
#define CHANNEL_6521_ICM		10
#define CHANNEL_6521_IA			11
#define CHANNEL_6521_IB			12
#define CHANNEL_6521_IC			13
#define CHANNEL_6521_I0			14
#define CHANNEL_6521_I1			15

#define CHANNEL_6521_UAB			16
#define CHANNEL_6521_UBC			17
#define CHANNEL_6521_UCA			18


#define POINT_PER_PERIOD		32	//ÿ�ܲ���������
#define PERIOD_NUMBER			120	//�������ݵ��ܲ���
#define CHANNNEL_NUM			30	//ͨ������,ǰ16·ΪAD����ͨ������12·Ϊ�ϲ�ͨ��
#define AD_CHAN_NUM				16	//AD����ͨ��
#define A_COM_CHAN_NUM			8	//ģ��ϳ�ͨ������
#define D_SAMP_CHAN_NUM			6	//����¼��ͨ������
#define RELAY_SAMP_INTERVAL		7


#endif