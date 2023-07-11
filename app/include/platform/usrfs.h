#ifndef _USRFS_H_
#define _USRFS_H_

#include "syscfg.h"

#if(OS_PLATFORM == OS_VXWORKS)

	#include "vx_file.h"
	
	#define FS_ReadFile        	VX_ReadFile
	#define FS_WriteFile       	VX_WriteFile
	#define FS_DeleteFile      	VX_DeleteFile
	#define FS_GetFileLength   	VX_GetFileLength
	#define FS_ReadParaFile    	VX_ReadParaFile
	#define FS_WriteParaFile   	VX_WriteParaFile   
	#define FS_ReadFileHeader  	VX_ReadFileHead
	#define FS_WriteFileHeader 	VX_WriteFileHead
	#define FS_GetMagic        	VX_GetMagic
	#define FS_SetMagic        	VX_SetMagic
	#define FS_MkDir        	VX_MkDir
	#define FS_DeleteDir       	VX_DeleteDir
	#define FS_GetCodeRootDir   VX_GetCodeRootDir
	#define FS_GetDataRootDir	VX_GetDataRootDir
	
#elif(OS_PLATFORM == OS_UCOSII)

#elif(OS_PLATFORM == OS_WIN32)

#endif

#endif
