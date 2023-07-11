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

#define MODUEL_TYPE		MT_KF6533

#define PRO_TYPE			"KF6533"
#define PRO_6533							//װ������
#define FREQ_TYPE			FREQ_TYPE_PRO	//װ������
#define PRO_49I

//����ԭʼ����ͨ��˳�����
//������¼��ͨ���̶�Ϊ24-27
#define CHANNEL_DC_1			24
#define CHANNEL_DC_2			25
#define CHANNEL_DC_3			26
#define CHANNEL_DC_4			27
#define CHANNEL_DC_5			28
#define CHANNEL_DC_6			29


//6533װ��ģ����ͨ��
#define CHANNEL_6533_UA		    0
#define CHANNEL_6533_UB		    1
#define CHANNEL_6533_UC		    2
#define CHANNEL_6533_U0h		3
#define CHANNEL_6533_Ua		    4
#define CHANNEL_6533_Ub		    5
#define CHANNEL_6533_Uc		    6
#define CHANNEL_6533_U0l		7
#define CHANNEL_6533_IA		    8
#define CHANNEL_6533_IB		    9
#define CHANNEL_6533_IC	    	10
#define CHANNEL_6533_Ia		    11
#define CHANNEL_6533_Ib		    13
#define CHANNEL_6533_Ic		    15
#define CHANNEL_6533_I0		    12
#define CHANNEL_6533_Ijx		14
#define	CHANNEL_6533_IDA		16
#define	CHANNEL_6533_IRA		17
#define	CHANNEL_6533_IDB		18
#define	CHANNEL_6533_IRB		19
#define	CHANNEL_6533_IDC		20
#define	CHANNEL_6533_IRC		21

#define POINT_PER_PERIOD		32	//ÿ�ܲ���������
#define PERIOD_NUMBER			120	//�������ݵ��ܲ���
#define CHANNNEL_NUM			30	//ͨ������,ǰ16·ΪAD����ͨ������12·Ϊ�ϲ�ͨ��
#define AD_CHAN_NUM				16	//AD����ͨ��
#define A_COM_CHAN_NUM			8	//ģ��ϳ�ͨ������
#define D_SAMP_CHAN_NUM			6	//����¼��ͨ������
#define RELAY_SAMP_INTERVAL		8


#endif
