/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			time.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           ʱ�Ӳ���                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      �ṩϵͳʱ�Ӷ�д�����ӿ�                          
*                                                                                  
* ����                                                                             
*                                                                                  
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/12/20    ��ʼ����                         
*                                                                                  
********************************************************************************************/
#include "timedef.h"
#include "daedef.h"
#include "dbfunc.h"
#include "daefunc.h"

//װ�ø�λ��־
#define SYS_RESET_FLAG		0x55aa33cc

extern TSysConfigTable	*G_pCfgTable;

//ϵͳͣ�ϵ��¼���¼
TMiscEvent PowerOnEvent;


DWORD Month[12]={MaxMonth,MinMonth,MaxMonth,MinMonth,MaxMonth,MinMonth,MaxMonth,MaxMonth,MinMonth,MaxMonth,MinMonth,MaxMonth};

//ϵͳʱ�ӳ�ʼ������Ӳ��ʱ��оƬ�ж�ȡ,��д���ڴ�ʱ�ӣ��������к���B��ʽˢ��ʱ��,
//ͬʱ���붨ʱ����ɺ����ʱ
BOOL		g_bPowerOff = FALSE;
TMiscEvent	g_tPowerOffMisc;
TMiscEvent	g_tPowerOnMisc;
TMiscEvent	g_tResetMisc;

void SysTimeInit()
{
	TSysTime SystemTime;
	
	DWORD *pFlag = (DWORD*)SYS_START_FLAG_ADD;	//װ�ø�λ�ڲ�RAM��־

	//�жϸ�λ����
	if(*pFlag != SYS_RESET_FLAG)	//��ͣ�ϵ����
	{
		*pFlag = SYS_RESET_FLAG;

		//��¼ͣ���¼�
		ReadAbsTime(&g_tPowerOffMisc.absTime);
		g_tPowerOffMisc.nClass	= MISC_CLASS_POWER;
		g_tPowerOffMisc.dwDevName = DB_GetDevID();
		g_tPowerOffMisc.dwOperator= DAE_GetCurrentTaskPortId();
		g_tPowerOffMisc.dwValue	= 1;
		g_bPowerOff = TRUE;
	}

	memset((void *)&SystemTime, 0, sizeof(TSysTime));


	//��ʼ��Ӳ��ʱ��оƬ��IIC�ӿ�
	ClockDevOpen();	

	if(ReadHardTime(&SystemTime) != ERR_SUCCESS)
	{
		//����ʱ��оƬ��Ϣ���粻�ܱ��棬���ÿ�ζ���ʼ��ΪĬ��ʱ��	
		SystemTime.Year = 2008;
		SystemTime.Month = 1;
		SystemTime.Day = 1;
		SystemTime.Hour = 0;
		SystemTime.Minute = 0;
		SystemTime.Second = 0;
		WriteHardTime(&SystemTime);
	}
	//��ʼд��ϵͳʱ��
	WriteSystemTime(&SystemTime);

	//���������ͣ�磬���¼�ϵ��¼�����λ�¼��������ݿ��ʼ������������
	if(g_bPowerOff == TRUE)
	{
		ReadAbsTime(&g_tPowerOnMisc.absTime);
		g_tPowerOnMisc.nClass	= MISC_CLASS_POWER;
		g_tPowerOnMisc.dwDevName = DB_GetDevID();
		g_tPowerOnMisc.dwOperator= DAE_GetCurrentTaskPortId();
		g_tPowerOnMisc.dwValue	= 0;
	}
	else
	{
//	    DB_Register_Reset(0);		//�ȸ�λ
		ReadAbsTime(&g_tResetMisc.absTime);
		g_tResetMisc.nClass 	= MISC_CLASS_RESET;
		g_tResetMisc.dwDevName = DB_GetDevID();
		g_tResetMisc.dwOperator = DAE_GetCurrentTaskPortId();
		g_tResetMisc.dwValue = 0;
	}
}

STATUS ReadAbsTime(TAbsTime *time)
{
	ASSERT(time != NULL);

	*time = G_pCfgTable->SystemStatus.stTime;
//	*time = MySysTime;//G_pCfgTable->SystemStatus.stTime;

	return ERR_SUCCESS;
}

STATUS WriteAbsTime(TAbsTime *time)
{
	ASSERT(time != NULL);
	G_pCfgTable->SystemStatus.stTime = *time; 
//	MySysTime = *time;
	return ERR_SUCCESS;
}

STATUS ReadSystemTime(TSysTime *time)
{
	TAbsTime AbsTime;

	AbsTime = G_pCfgTable->SystemStatus.stTime;
//	AbsTime = MySysTime;	//G_pCfgTable->SystemStatus.stTime;

	AbsTimeTo(&AbsTime, time);
	
	return ERR_SUCCESS;
}

STATUS WriteSystemTime(TSysTime *time)
{
	TAbsTime AbsTime;

	if(ClockInvalidate(time) == TRUE)
	{
		ToAbsTime(time, &AbsTime);
//		MySysTime = AbsTime;
		G_pCfgTable->SystemStatus.stTime = AbsTime;
		WriteHardTime(time);
		return ERR_SUCCESS;
	}
	else
		return ERR_FAILURE;
}

void AbsTimeTo(TAbsTime *in, TSysTime *out)
{
	DWORD sec;
	WORD msec;
    
	MSecondToSecond(&sec,&msec,in);
	Unixtodos(sec,out);
	out->MSecond = msec;
}

void ToAbsTime(TSysTime *in, TAbsTime *out)
{
    DWORD sec;
    //ASSERT((in != 0)&&(out != 0));
    
    sec = Dostounix(in);
    SecondToMSecond(sec,in->MSecond,out);
	
}

void MSecondToSecond(DWORD *psec,WORD *pmsec,TAbsTime *tm)
{
    DWORD sec,msec;
    
    sec   =  tm->Hi * 4294967;        /* 4294967 = # of seconds in 32 bits of milliseconds             */
    sec  +=  tm->Lo/1000;             /* Add number of seconds in lower 32 bits.                       */
    sec  += (296 * tm->Hi)/1000;      /* Adjust for rounding errors since 4294967 is not exact         */
    
    /* ie. there are 4294967.295 seconds in 32 bits of miliseconds.  */
    /* therefore each count in u2 contributes 295 milliseconds.      */
    msec  = (tm->Lo % 1000);          /* Get milliseonds in lower 32 bits.                             */
    msec += ((tm->Hi * 296) % 1000);  /* Add contribution of upper 16 bits                             */
    
    /* ie. 2**32 msec = 4294967296 milliseconds                      */
    /* therefore each count in u2 contributes 296 milliseconds.      */
    if(msec >= 1000)                  /* Check for overflow of milliseconds */
    {
        msec -= 1000;
        sec ++;
    }
    *psec = sec;
    *pmsec = msec;
}

void SecondToMSecond(DWORD sec,WORD msec,void *ptr)
{
    TAbsTime* tm =(TAbsTime*)ptr;
    DWORD m1,m2,m3,tmp;
    
    m1 = (sec & 0x0000FFFFl) * 1000l;
    m2 = ((sec & 0xFFFF0000l) >> 16) * 1000l;
    m3 = m2 + ((m1 & 0xFFFF0000l) >> 16);
    
    /* get upper two bytes of 48 bits milliseconds */
    tm->Hi = (m3 & 0xFFFF0000l) >> 16;
    
    /* get lower four bytes of 48 bits milliseconds */
    tm->Lo = ((m3 & 0x0000FFFFl) << 16) + (m1 & 0x0000FFFFl);
    
    /* add microseconds to lower four bytes */
    tmp = tm->Lo;
    tm->Lo += msec;
    
    /* Check for wrap around of l4 */
    if (tmp > tm->Lo)
        tm->Hi++;
}

void  Unixtodos(DWORD i,TSysTime *pp)
{
    DWORD j,t;
    
    for(t=1970;t<2100;t++)
    {
        if((t & 0x03)==0)
            j=LeapYearSecond;           //366*24*60
        else
            j=NoneLeapYearSecond;      //365*24*60
        
        if(i>=j)
            i-=j;
        else
        {
            pp->Year=(WORD)t;
            break;
        }
    }
    
    if((pp->Year & 0x03)==0)
        Month[1] = Leap2Month;
    else 
        Month[1] = NoneLeap2Month;
    
    for(t=0;t<12;t++)
    {
        if(i>=Month[t]) //24*60;
            i-=Month[t];
        else
        {
            pp->Month=(BYTE)(t+1);
            break;
        }
    }
    
    pp->Day=(BYTE)(i/86400+1);
    i=i%86400;
    
    pp->Hour=(BYTE)(i/3600);
    i%=3600;
    pp->Minute=(BYTE)(i/60);
    pp->Second = (BYTE)(i%60);
}

DWORD Dostounix(TSysTime *Time)
{
	DWORD i=0,t;
	if(!ClockInvalidate(Time))
	{
		LogError("Dostounix",FILE_LINE,"time invalidate");
		return 0;
	}
    
    if((Time->Year & 0x03)==0)
        Month[1] = Leap2Month;
    else 
        Month[1] = NoneLeap2Month;
    
    if(Time->Year>2100)
        Time->Year=2100;
    
    for(t=1970;t<Time->Year;t++)
    {
        if((t & 0x03)==0)
            i+=LeapYearSecond;
        else
            i+=NoneLeapYearSecond;
    }
    
    for(t=0;t<Time->Month-1;t++)
        i+=Month[t];       //24*60;
    
	i+=(Time->Day-1)*86400; //24*60*60;
	i+=Time->Hour*3600;     //60;
	i+=Time->Minute*60;
	i+=Time->Second;
    
	return i;
}

BOOL ClockInvalidate(TSysTime *tm)
{
    if((tm->Year<1970) || (tm->Year>2100)) return FALSE;
    if((tm->Month<1) || (tm->Month>12)) return FALSE;
    if((tm->Day<1) || (tm->Day>31)) return FALSE;
    if(tm->Hour>23) return FALSE;
    if(tm->Minute>59) return FALSE;
    if(tm->Second>59) return FALSE;
    if(tm->MSecond>999) return FALSE;
    return TRUE;
}

//����ʱ�����
BOOL AbsTimeAdd(TAbsTime *t1,TAbsTime *t2)
{
	TAbsTime t3;

	ASSERT((t1 != NULL)&&(t2 != NULL));
	if((t1 == NULL)||(t2 == NULL))
	{
		return FALSE;
	}

	t3.Hi = t1->Hi + t2->Hi;
	t3.Lo = t1->Lo + t2->Lo;
	if(t3.Lo < t1->Lo)
	{
		t1->Hi = t3.Hi+1;
		t1->Lo = t3.Lo;
	}
	else
	{
		t1->Hi = t3.Hi;
		t1->Lo = t3.Lo;
	}

	return TRUE;
}

//����ʱ�����
BOOL AbsTimeSub(TAbsTime *t1,TAbsTime *t2)
{	
	ASSERT((t1 != NULL)&&(t2 != NULL));
	if((t1 == NULL)||(t2 == NULL))
	{
		return FALSE;
	}

	if(t1->Hi >= t2->Hi)
	{
		if(t1->Lo >= t2->Lo)
		{
			t1->Hi = t1->Hi - t2->Hi;
			t1->Lo = t1->Lo - t2->Lo;
		}
		else
		{
			t1->Hi = t1->Hi - t2->Hi-1;
			t1->Lo = t1->Lo+(~(t2->Lo))+1;
		}

		return TRUE;
	}
	else
	{
		t1->Hi = 0;
		t1->Lo = 0;
		return FALSE;
	}
}


