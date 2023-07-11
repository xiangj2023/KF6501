#ifndef _SYSTEM_H
#define _SYSTEM_H

/****************************************************************************************/
/*                                                                                  	*/
/* �ļ�����                                           �汾��                        	*/
/*                                                                                  	*/
/*      system.h	                               1.0                          	*/
/*                                                                                  	*/
/* ��Ȩ                                                                                 */
/*      ��򿭷������ɷ����޹�˾                                                       	*/
/*                                                                                  	*/
/* ����                                                                             	*/
/*                                                                                  	*/
/*      ϵͳ����ͷ�ļ�����ʵ��				            			                   	*/
/*                                                                                  	*/
/* �޸���ʷ                                                                         	*/
/*                                                                                  	*/
/*      �޸���           �޸�����       		�޸�����                                */
/*                                                                                  	*/
/*      zqz      		 08/03/21			��ʼ����,�汾��1.0                          */
/*                                                                                  	*/
/****************************************************************************************/

#include "syscfg.h"	/*ϵͳ���ģ�����ü����Զ˿ڶ����*/

#include "sysdefs.h"	/*ϵͳ�������Ͷ��弰���ú궨��*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "osdefs.h"
#include "platform.h"
#include "timedef.h"

#include "errmsg.h"
#include "hash.h"

#include "msgdefs.h"

#include "debug.h"
#include "debugcom.h"

#if(OS_PLATFORM == OS_VXWORKS)

	#include "resm5234.h"
	#include "mcf523x.h"
	
	//������vxworks�йص�ͷ�ļ�
	#include <vxWorks.h>
	#include <semLib.h>
	#include <taskLib.h>
	#include <msgQLib.h>
	#include <msgQEvLib.h>
	#include <eventLib.h>
	#include <taskVarLib.h>
	#include <wdLib.h>
	#include <logLib.h>
	#include <tickLib.h>
	#include <sysLib.h>
	#include <errno.h>
	#include <configall.h>
	#include <intlib.h>
	#include <ctype.h>
	#include <intLib.h>
	#include <iv.h>
	#include <cacheLib.h>
	#include <ipProto.h>
	#include <rebootLib.h>
	#include <elf.h>
#elif(OS_PLATFORM == OS_UCOSII)
	#include "includes.h"	/*��ucosii��صĽӿ�*/
	#include  "config.h"
	#include "target.h"
#elif(OS_PLATFORM == OS_WIN32)
	#include <windows.h>
#endif
	#include "endian.h"
	#include "filesys.h"

#endif
