#ifndef _TIMEDEF_H_
#define _TIMEDEF_H_

#include "sysdefs.h"
#include "resm5234.h"

/*----------------------------------------------------------------------------------------*/
/* ����ʱ���ʽ����,��ǰʱ��Ϊ�����1970��1��1��0ʱ0��0��0������ܺ�����                  */
/*----------------------------------------------------------------------------------------*/
struct TAbsTime{
    DWORD       Lo;                     /*��λϵͳʱ��*/                    
    WORD        Hi;                     /*��λϵͳʱ��*/
};
/*----------------------------------------------------------------------------------------*/
/* �������ʱ���ʽ                                                                       */
/*----------------------------------------------------------------------------------------*/
struct TSysTime{
    WORD        Year;                   /*��*/
    BYTE        Month;                  /*��*/
    BYTE        Day;                    /*��*/
    BYTE        Hour;                   /*ʱ*/
    BYTE        Minute;                 /*��*/
    BYTE        Second;                 /*��*/
    BYTE 		Week;					/*����*/
	WORD		MSecond;				/*����*/
};

#define MaxMonth           2678400  /*���·ݵ�������*/
#define MinMonth           2592000  /*С�·ݵ�������*/
#define Leap2Month         2505600  /*����ʱ���·ݵ�������*/
#define NoneLeap2Month     (28*24*60*60)  /*������ʱ���·ݵ�������*/
#define NONELEAP2MONTH     2419200  /*������ʱ���·ݵ�������*/
#define LeapYearSecond     31622400 /*����ʱÿ���������*/
#define NoneLeapYearSecond 31536000 /*������ʱÿ���������*/

//ʱ�ӺϷ����ж�
//#define VALIDATE_TIME(tm) ((tm->byMonth<12)||(tm->byDay<31)||(tm->byHour<24)||(tm->byMinute<60)||(tm->byWeek<7))


#if(CLOCK_MODULE == R8025)
	#include "r8025.h"

	#define ClockDevOpen		InitR8025
	#define ReadHardTime		ReadR8025Time 
	#define WriteHardTime		WriteR8025Time
#endif

void SysTimeInit();
STATUS ReadAbsTime(TAbsTime *time);
STATUS WriteAbsTime(TAbsTime *time);
STATUS ReadSystemTime(TSysTime *time);
STATUS WriteSystemTime(TSysTime *time);
void AbsTimeTo(TAbsTime *in, TSysTime *out);
void ToAbsTime(TSysTime *in, TAbsTime *out);
void MSecondToSecond(DWORD *psec,WORD *pmsec,TAbsTime *tm);
void SecondToMSecond(DWORD sec,WORD msec,void *ptr);
void  Unixtodos(DWORD i,TSysTime *pp);
DWORD Dostounix(TSysTime *Time);

BOOL ClockInvalidate(TSysTime *tm);
BOOL AbsTimeAdd(TAbsTime *t1,TAbsTime *t2);
BOOL AbsTimeSub(TAbsTime *t1,TAbsTime *t2);

#endif
