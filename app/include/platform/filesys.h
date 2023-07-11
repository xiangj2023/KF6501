#ifndef _FILESYS_H_
#define _FILESYS_H_

#include "syscfg.h"

#if(OS_PLATFORM == OS_VXWORKS)
	#include "vx_file.h"
	
	#define FS_ReadFile			VX_ReadFile
	#define FS_WriteFile		VX_WriteFile
	#define FS_DelFile			VX_DeleteFile
	#define FS_GetFileLength	VX_GetFileLength
	#define FS_MkDir			VX_MkDir
	#define FS_DeleteDir		VX_DeleteDir
	#define FS_ReadParFile		VX_ReadParaFile
	#define FS_ReadFileHead		VX_ReadFileHead
	#define FS_WriteParFile		VX_WriteParaFile
	#define FS_WritefileHead	VX_WriteFileHead
	#define FS_GetMagic			VX_GetMagic
	#define FS_SetMagic			VX_SetMagic
	#define FS_GetCodeDir		VX_GetCodeDir
	#define FS_GetDataDir		VX_GetDataDir
	
#elif(OS_PLATFORM == OS_WIN32)

#endif

#endif