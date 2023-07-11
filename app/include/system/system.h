#ifndef _SYSTEM_H
#define _SYSTEM_H

/****************************************************************************************/
/*                                                                                  	*/
/* 文件名称                                           版本号                        	*/
/*                                                                                  	*/
/*      system.h	                               1.0                          	*/
/*                                                                                  	*/
/* 版权                                                                                 */
/*      天津凯发电气股份有限公司                                                       	*/
/*                                                                                  	*/
/* 描述                                                                             	*/
/*                                                                                  	*/
/*      系统常用头文件包含实现				            			                   	*/
/*                                                                                  	*/
/* 修改历史                                                                         	*/
/*                                                                                  	*/
/*      修改人           修改日期       		修改内容                                */
/*                                                                                  	*/
/*      zqz      		 08/03/21			初始创建,版本号1.0                          */
/*                                                                                  	*/
/****************************************************************************************/

#include "syscfg.h"	/*系统软件模块配置及调试端口定义等*/

#include "sysdefs.h"	/*系统数据类型定义及常用宏定义*/

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
	
	//包含与vxworks有关的头文件
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
	#include "includes.h"	/*与ucosii相关的接口*/
	#include  "config.h"
	#include "target.h"
#elif(OS_PLATFORM == OS_WIN32)
	#include <windows.h>
#endif
	#include "endian.h"
	#include "filesys.h"

#endif
