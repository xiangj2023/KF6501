#ifndef _TIMEDEF_H_
#define _TIMEDEF_H_

#include "sysdefs.h"
#include "resm5234.h"

/*----------------------------------------------------------------------------------------*/
/* 绝对时间格式定义,当前时间为相对于1970年1月1号0时0分0秒0毫秒的总毫秒数                  */
/*----------------------------------------------------------------------------------------*/
struct TAbsTime{
    DWORD       Lo;                     /*低位系统时间*/                    
    WORD        Hi;                     /*高位系统时间*/
};
/*----------------------------------------------------------------------------------------*/
/* 定义基本时间格式                                                                       */
/*----------------------------------------------------------------------------------------*/
struct TSysTime{
    WORD        Year;                   /*年*/
    BYTE        Month;                  /*月*/
    BYTE        Day;                    /*日*/
    BYTE        Hour;                   /*时*/
    BYTE        Minute;                 /*分*/
    BYTE        Second;                 /*秒*/
    BYTE 		Week;					/*星期*/
	WORD		MSecond;				/*毫秒*/
};

#define MaxMonth           2678400  /*大月份的总秒数*/
#define MinMonth           2592000  /*小月份的总秒数*/
#define Leap2Month         2505600  /*闰月时二月份的总秒数*/
#define NoneLeap2Month     (28*24*60*60)  /*不闰月时二月份的总秒数*/
#define NONELEAP2MONTH     2419200  /*不闰月时二月份的总秒数*/
#define LeapYearSecond     31622400 /*闰年时每年的总秒数*/
#define NoneLeapYearSecond 31536000 /*不闰年时每年的总秒数*/

//时钟合法性判断
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
