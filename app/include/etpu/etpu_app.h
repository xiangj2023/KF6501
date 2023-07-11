#ifndef _EPTU_APP_H_
#define _EPTU_APP_H_

#include "system.h"

#include "etpu_util.h"
#include "etpu_set1.h"
#include "etpu_pwm.h"
#include "etpu_gpio.h"
#include "etpu_ic.h"

//����Ƶ�ʲ�������������ֵ,��������������װ��	9375000/Ƶ��
#define M_FREQ_UPPER_LIMIT	210700		//44.5Hz
#define M_FREQ_LOWER_LIMIT	168900		//55.5Hz
#define P_FREQ_UPPER_LIMIT	190000		//49.5Hz
#define P_FREQ_LOWER_LIMIT	185500		//50.5Hz

//����B��ʽ��Ϣλ����������
#define IRIG_B_ERR		0
#define IRIG_B_8MS		1
#define IRIG_B_5MS		2
#define IRIG_B_2MS		3

//�����������ļ�����Χ����(37.5/4)MHzΪʱ�ӻ�׼
#define PULSE_OFFSET	1000
#define PULSE_2MS_H		(18750+PULSE_OFFSET)	// 2����������� 18750+400
#define PULSE_2MS_L		(18750-PULSE_OFFSET)	// 2����������� 18750-400
#define PULSE_5MS_H		(46875+PULSE_OFFSET)	// 5����������� 46875+400
#define PULSE_5MS_L		(46875-PULSE_OFFSET)	// 5����������� 46875-400
#define PULSE_8MS_H		(75000+PULSE_OFFSET)	// 8����������� 75000+400
#define PULSE_8MS_L		(75000-PULSE_OFFSET)	// 8����������� 75000-400

//����B��ʽ��������Ϣ�ֵĸ�������Ϣλ�ĸ���
#define IRIG_B_WORD_NUM	10
#define IRIG_B_BIT_NUM	9

//����B��ʽ������Ϣ�ֵ�����
#define IRIG_B_SEC		0
#define IRIG_B_MIN		1
#define IRIG_B_HOUR		2
#define IRIG_B_DAY		3
#define IRIG_B_H_DAY	4
#define IRIG_B_YEAR		5
#define IRIG_B_SAT		6
#define IRIG_B_QT		7
#define IRIG_B_SBS1		8
#define IRIG_B_SBS2		9

#define FREQ_DATA_BUF	9

//KF3500ϵͳB��ʽ���ֶ���
#define IRIG_35_PLANET	5
#define IRIG_35_YEAR	6	
//��������ʱ�Ӹ�ʽ
//#define		IRIG_KF6500	((DWORD)(0x01<<8))		//���ϵ������ն������ݴ洢
//#define		DB_RELAY_CACLINT	((DWORD)(0x01<<9))		//�Ƿ������ֵ��

#define IRIG_KF6500 FALSE 
#define IRIG_DK3500 TRUE
struct TIrigBInt
{
	BOOL isInResolve;		//��ǰ�Ƿ���B��ʽ����״̬�����Ƿ��ҵ���ͷ
	BYTE bCurPulseType;		//��ǰ��������
	BYTE bOldPulseType;		//�ϴ���������
	BYTE bCurPulseWordNum;	//��ǰ���ڵ���Ϣ����
	BYTE bCurPulseBitNum;	//��ǰ���ڵ���Ϣ���е�λ��
	WORD bIrigWordInf[IRIG_B_WORD_NUM];
	TAbsTime OldAbsTime;	//��һ���������ϵͳʱ��
	BOOL isAbsTimeOk;		//��ǰϵͳʱ���Ƿ���Ч(����+1�봦��)
};

//Ƶ�ʲ���
struct TFreqData
{
	DWORD dwOldCounter;
	DWORD dwPeriod;		//��λ΢��
	DWORD dwErrCnt;
	DWORD dwIntCnt;
	DWORD dwUpLimit;
	DWORD dwLowLimit;
	DWORD dwPulseLen[FREQ_DATA_BUF];
	DWORD dwFreq;
};

//���ݽ�����������Ƶ��ֵ����������ADת��������PWM��Ƶ��ֵ,ռ�ձȹ̶�
#define ADStartFreqUpdate(freq)		fs_etpu_pwm_update(ETPU_AD_START, freq, 500, 9375000)
#define AD1StartFreqUpdate(freq)	fs_etpu_pwm_update(ETPU_AD1_START, freq, 500, 9375000)

/*��һ·Ƶ�ʲ����жϴ�����ͨ�����ڿ���AD������Ƶ��*/
void EtpuFreq1Int(void *arg);

/*�ڶ�·Ƶ�ʲ����жϴ�����ͨ������ͬ�ڿ���*/
void EtpuFreq2Int(void *arg);

/*ETPU��Ƶ��������ͨ��GPIO���빦��ʵ�֣��жϷ�ʽ�����������*/
BOOL EtpuFreqProc(BYTE channel);

/*��ʱƵ�ʲ���ͨ������*/
void FreqChanMonitor();

/*����ͨ����ȡ��������Ƶ��ֵ���Ŵ�100��*/
DWORD GetFreqValue(BYTE channel);

DWORD GetMidFreqValue(BYTE channel);

/*ETPUϵͳ��Դƫ�ͼ����жϺ���������GPIO�����벶׽��ʽ�������½���,�������ж�*/
void EtpuPfoInt(void *arg);

/*ETPU���´�����������GPIO�����벶׽��ʽ����׽�������*/
void EtpuTempProc(void *arg);

/*B��ʽʱ�ӽ����жϴ�������GPIO�����빦�ܣ���׽������ز������ж�*/
void EtpuIrigInt(void * arg);

/*����������B��ʽʱ����Ϣת��Ϊ�����1970��ĺ�����*/
void IrigBToAbsTime();

/*���B��ʽ�ź��Ƿ�����*/
void isIrigOk();
/*��ȡʱ�ӵĸ�ʽ*/


#endif
