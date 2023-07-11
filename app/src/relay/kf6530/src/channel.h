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
#define FREQ_TYPE			FREQ_TYPE_PRO	//װ������

//����ԭʼ����ͨ��˳�����
//������¼��ͨ���̶�Ϊ24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29

//6530װ��ģ��ͨ��
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

#define POINT_PER_PERIOD		32	//ÿ�ܲ���������
#define PERIOD_NUMBER			120	//�������ݵ��ܲ���
#define CHANNNEL_NUM			30	//ͨ������,ǰ16·ΪAD����ͨ������12·Ϊ�ϲ�ͨ��
#define AD_CHAN_NUM				16	//AD����ͨ��
#define A_COM_CHAN_NUM			8	//ģ��ϳ�ͨ������
#define D_SAMP_CHAN_NUM			6	//����¼��ͨ������
#define RELAY_SAMP_INTERVAL		7


#endif
