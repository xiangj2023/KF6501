/********************************************************************************************
*                                                                                  
*        版权所有(C) 天津凯发电气股份有限公司(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* 文件名称          
*			platform.cpp                                                      
*                                                                                  
*                                                                                  
* 软件模块                                                                         
*           vxworks系统文件操作接口                                                                       
*			                                                                
* 描述                                                                             
*                                                                                  
*      针对于vxworks系统的文件系统操作接口                          
*                                                                                        
*                                                                                  
* 函数                                                                             
*                                                                                  
*      	VX_ReadFile			读文件
*      	VX_WriteFile		写文件                                                              
*      	VX_DeleteFile   	删除文件                                                                         
*      	VX_MkDir 	    	创建目录                                                                        
*      	VX_DeleteDir    	删除目录                                                                       
*      	VX_ReadParaFile 	读参数文件                                                                       
*      	VX_ReadFileHeader   读参数文件头                                                                      
*      	VX_WriteParaFile   	写参数文件                                                                        
*      	VX_WriteFileHeader  写参数文件头                                                                      
*      	VX_GetMagic       	读参数文件中的magic标志                                                                     
*      	VX_SetMagic       	写参数文件中的magic标志                                                                     
*		VX_GetCodeRootDir	取代码分区根目录(即磁盘分区)名称
*		VX_GetDataRootDir	取数据分区根目录(即磁盘分区)名称
*
* 修改历史                                                                         
*                                                                                  
*      修改人           修改日期     修改内容                                      
*                                                                                  
*      ZQZ               08/6/23    初始创建                         
*                                                                                  
********************************************************************************************/


//注意需要测试文件写入后长度是否自动累加，否则需要通过ioctl (fid, FIOTRUNC, len)来调整长度

#include "vx_file.h"
#include "endian.h"

#if (OS_PLATFORM == OS_VXWORKS)

extern WORD PCheckSum(void *p,DWORD length);

/*********************************************************************************************
 *
 * 功能描述     读文件
 *
 * 参数说明      - filename	: 输入 	文件名称
 *       		 - offset	: 输入	读文件的内容偏移
 * 		         - length	: 输入	希望读取文件的长度
 * 		         - buf		: 输出	读文件缓冲区
 * 		         - len		: 输出	实际读取文件的长度
 * 		         - control	: 输入	读文件操作控制标志
 *
 * 返回代码
 *                成功、失败或读文件结束
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_ReadFile( char * filename, DWORD offset, DWORD length, BYTE *buf, DWORD *len, WORD control )
{
    char  fullname[40]; 
    DWORD end=0;
    DWORD filesize, length1;
    struct stat filestat;
	int fid;
	STATUS	rc;

	// 参数合法性检查
	ASSERT(length > 0);
	ASSERT(buf != NULL);
	if( length <= 0 || buf==NULL )
		return FILE_ERROR;

	// Clear the buffer
	memset(buf, 0, length);

    // First Add path to filename according to filename
    ToFullName( filename, fullname );

	OS_LockM(FLASH_LOCK);
	
    // Open the file
    fid = open( fullname,O_RDWR , 0);
    if( fid == ERROR )
    {
		OS_UnLockM(FLASH_LOCK);
        LogError("OnReadFile",FILE_LINE,"Open the file %s error",filename);
        return FILE_ERROR;
    }

    // Get the file status
	rc=fstat(fid,&filestat);	
	if( rc == ERROR )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("OnReadFile",FILE_LINE,"Get the file %s's status error",fullname);
        return FILE_ERROR;
    }

    // Check the parameter validate
    filesize=filestat.st_size;
    if( filesize <= offset )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("OnReadFile",FILE_LINE,"When read %s, offset is overflow!",filename);
        return FILE_ERROR;
    }

    // Move the file pointer
	rc = lseek( fid, offset, SEEK_SET);
    if( rc == ERROR )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("OnReadFile",FILE_LINE,"When read %s, move file pointer error!",filename);
        return FILE_ERROR;
    }

    // Calucate the length should be get out
    if( filesize > offset+length )
        length1 = length;
    else if( filesize == offset+length )
	{
		length1 = length;
		end =1;
	}
	else
    {
        length1 = filesize-offset;
        end     = 1;
    }

    // Read the file
    rc = read( fid, (char*)buf, length1);
    if( (rc == ERROR) || (rc != length1) )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("OnReadFile",FILE_LINE,"When read %s, read file error!",filename);
        return FILE_ERROR;
    }

	// Return get char number
	*len = rc;

    // Close the file
    close(fid);
	OS_UnLockM(FLASH_LOCK);

    if( end )
        return FILE_END;
    else
        return FILE_OK;	
}

/*********************************************************************************************
 *
 * 功能描述     写文件,若文件不存在则首先创建，然后写文件
 *
 * 参数说明      - filename	: 输入 	文件名称
 *       		 - offset	: 输入	写文件的内容偏移
 * 		         - length	: 输入	写入文件的长度
 * 		         - buf		: 输入	写文件缓冲区
 * 		         - type		: 输入	写文件标志(头/尾/其它)
 * 		         - control	: 输入	写文件操作控制标志
 *
 * 返回代码
 *                成功、失败或写文件结束
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_WriteFile( char * filename, DWORD offset, DWORD length, BYTE *buf, WORD type, WORD control )
{
    char  fullname[40], fullname2[40];
    int fid;
	STATUS rc;

	if( length == 0 || buf==NULL )
		return FILE_ERROR;

    ToFullName( filename, fullname );

	OS_LockM(FLASH_LOCK);

	// If the file packet is first packet, create new file
	if( (offset == 0) || (type&FILE_HEAD) )
	{
		remove( fullname );
		rc = creat( fullname, O_RDWR);	//返回创建的文件ID
		if( rc == ERROR )
		{
			OS_UnLockM(FLASH_LOCK);
			LogError("OnWriteFile",FILE_LINE,"When create %s error!",filename);
			return FILE_ERROR;
		}
		//必须关闭，因为creat创建后已经打开，如果不关闭会造成重复打开，最后导致打开文件过多
		//超过文件系统规定的个数后导致文件系统异常
		close( rc );
	}

	// Open the file
	fid = open( fullname, O_RDWR, 0 );
	if( fid == ERROR )
	{
		OS_UnLockM(FLASH_LOCK);
		//close( fid ); 	//不能直接删除，因为fid异常
		LogError("OnWriteFile",FILE_LINE,"When open %s error!",filename);
		return FILE_ERROR;
	}

	// Move the file pointer
	rc = lseek( fid, offset, SEEK_SET );
	if( rc == ERROR )
	{
		close( fid );
		OS_UnLockM(FLASH_LOCK);
		LogError("OnWriteFile",FILE_LINE,"When move the pointer of %s error!",filename);
		return FILE_ERROR;
	}

	// Write the file, return the data length that has been writed to file or ERROR 
	rc = write( fid, (char*)buf, length );
	if( rc == ERROR )
	{
		close( fid );
		OS_UnLockM(FLASH_LOCK);
		LogError("OnWriteFile",FILE_LINE,"When write %s error!",filename);
		return FILE_ERROR;
	}

	rc = close( fid );

	// If the file has writed over, move it to "DATA_DIRECTORY/" directory
	if( type & FILE_END )
	{
		OS_UnLockM(FLASH_LOCK);
		return FILE_END;
	}
	else
	{
		OS_UnLockM(FLASH_LOCK);
		// Return acknowledge
		return FILE_OK;
	}

/*
	if( length == 0 || buf==NULL )
		return FILE_ERROR;

    // Write file to temp directory first.
    strcpy( fullname, TEMP_DIR );
    strcat( fullname, filename );

	OS_LockM(FLASH_LOCK);
	
    // If the file packet is first packet, create new file
    if( (offset == 0) || (type&FILE_HEAD) )
    {
        remove( fullname );
        rc = creat( fullname, O_RDWR);	//返回创建的文件ID
        if( rc == ERROR )
        {
			OS_UnLockM(FLASH_LOCK);
            LogError("OnWriteFile",FILE_LINE,"When create %s error!",filename);
            return FILE_ERROR;
        }
		//必须关闭，因为creat创建后已经打开，如果不关闭会造成重复打开，最后导致打开文件过多
		//超过文件系统规定的个数后导致文件系统异常
        close( rc );
    }

    // Open the file
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
    {
		OS_UnLockM(FLASH_LOCK);
    	//close( fid );		//不能直接删除，因为fid异常
        LogError("OnWriteFile",FILE_LINE,"When open %s error!",filename);
        return FILE_ERROR;
    }

    // Move the file pointer
    rc = lseek( fid, offset, SEEK_SET );
    if( rc == ERROR )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("OnWriteFile",FILE_LINE,"When move the pointer of %s error!",filename);
        return FILE_ERROR;
    }

    // Write the file, return the data length that has been writed to file or ERROR 
    rc = write( fid, (char*)buf, length );
    if( rc == ERROR )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("OnWriteFile",FILE_LINE,"When write %s error!",filename);
        return FILE_ERROR;
    }

	rc = close( fid );


    // If the file has writed over, move it to "DATA_DIRECTORY/" directory
    if( type & FILE_END )
    {
        ToFullName( filename, fullname2 );
		rc = remove( fullname2 );		//删除
		rc = mv( fullname, fullname2 );	//移动

		OS_UnLockM(FLASH_LOCK);
		return FILE_END;
    }
    else
    {
		OS_UnLockM(FLASH_LOCK);
        // Return acknowledge
		return FILE_OK;
    }
*/    
}

/*********************************************************************************************
 *
 * 功能描述     删除文件
 *
 * 参数说明      - filename	: 输入 	文件名称
 * 			     - control  : 输入  控制标志
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_DeleteFile( char * filename, WORD control )
{
    char  fullname[40];
    STATUS rc;

    // Add path to filename
    ToFullName( filename, fullname );
	OS_LockM(FLASH_LOCK);
    // Delete the file
    rc= remove(fullname);

	OS_UnLockM(FLASH_LOCK);
    // Return message
    if(rc == ERROR)
    {
        LogError("VX_DeleteFile",FILE_LINE,"When delete %s error!",filename);
        return FILE_ERROR;
    }	
	else    
        return FILE_OK;
}

/*********************************************************************************************
 *
 * 功能描述     取文件长度
 *
 * 参数说明      - filename	: 输入 	文件名称
 * 			     - filesize : 输出  获取的文件长度
 *
 * 返回代码
 *                TRUE、FALSE
 *
 * 其它说明：          
 *
**********************************************************************************************/
BOOL VX_GetFileLength( char *filename, DWORD *filesize )
{
    char fullname[40];
    int fid;
    struct stat filestat;
	STATUS rc;

    // First Add path to filename according to filename
    ToFullName( filename, fullname );

	OS_LockM(FLASH_LOCK);

    // Open the file
    fid = open( fullname, O_RDONLY, 0 );
    if( fid == ERROR )
    {
		OS_UnLockM(FLASH_LOCK);
        LogError("GetFileLen",FILE_LINE,"Open the file %s error",filename);
        return FALSE;
    }

    // Get the file status
	rc=fstat(fid, &filestat);
	if( rc == ERROR )
    {
        close( fid );
		OS_UnLockM(FLASH_LOCK);
        LogError("GetFileLen",FILE_LINE,"Get the file %s's status error",filename);
        return FALSE;
    }

    // Check the parameter validate
    *filesize=filestat.st_size;
    close( fid );
	
	OS_UnLockM(FLASH_LOCK);

    return TRUE;
}

/*********************************************************************************************
 *
 * 功能描述     创建目录,若目录存在则不重复创建
 *
 * 参数说明      - dirname	: 输入 	目录名称
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_MkDir( char * dirname)
{
    STATUS rc;
    DIR *pxdir;

    // Check whether the directory exist.
	pxdir = opendir(dirname);
    if(pxdir != NULL)
    {
		closedir(pxdir);
        return FILE_OK;
	}
	
    // Create the directory
    rc = mkdir(dirname);
    if(rc == ERROR)
    {
        LogError("MakeDir",FILE_LINE,"make the dir %s's error",dirname);
        return FILE_ERROR;
    }
	//注意mkdir创建目录后自动关闭该目录
    return FILE_OK;
}

/*********************************************************************************************
 *
 * 功能描述     删除目录
 *
 * 参数说明      - dirname	: 输入 	目录名称
 *
 * 返回代码
 *                成功、失败
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_DeleteDir( char * dirname )
{
    STATUS rc;

    // Delete the file
    rc= rmdir(dirname);

    // Return message
    if(rc == ERROR)
    {
        LogError("DeleteDir",FILE_LINE,"delete the dir %s's error",dirname);
        return FILE_ERROR;
    }
	else    
        return FILE_OK;
}

/*********************************************************************************************
 *
 * 功能描述     读参数文件
 *
 * 参数说明      - filename	: 输入 	文件名称
 * 			     - buf 		: 输出  读出的文件内容缓冲区
 * 			     - len 		: 输出  获取的文件长度
 *
 * 返回代码
 *                成功或其它错误
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_ReadParaFile( char *filename, BYTE *buf, DWORD *len )
{
    struct stat filestat;
    char  fullname[50];
    DWORD filesize;
    WORD  checksum, filesum;
	STATUS rc;
	int fid;
    
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//打开文件
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
    {
        LogError("ReadParaFile",FILE_LINE,"Open the file %s error",filename);
        return ERR_OPEN;
    }
    
	//读文件状态信息    
    rc=fstat(fid,&filestat);
    if( rc == ERROR)
    {
        LogError("ReadParaFile",FILE_LINE,"read the file %s's status error",filename);
        close(fid);
        return ERR_FILESTATE;
    }

	//检查文件长度
    filesize=filestat.st_size;
    if( filesize < sizeof(TFileHead))
    {
        LogError("ReadParaFile",FILE_LINE,"read the file %s's length error",filename);
        close(fid);
        return ERR_FILELEN;
    }

	//读取整个文件内容
    rc = read( fid, (char*)buf, filesize);
    if( (rc == ERROR) || (filesize != rc) )
    {
        LogError("ReadParaFile",FILE_LINE,"read the file %s' error",filename);
        close(fid);
        return ERR_FILEREAD;
    }
	*len = rc;

	//对文件头和文件内容分别校验
    checksum = PCheckSum( (void *)buf,sizeof(TFileHead)-sizeof(WORD)); 
    filesum = ((TFileHead*)buf)->dwCheck;  
    filesum = ltobs(filesum);
    if( checksum == filesum )
    {   
        checksum = PCheckSum((void *)(buf+sizeof(TFileHead)),filesize-sizeof(TFileHead)-2);
        filesum  = buf[*len-2]+(buf[*len-1]<<8);
        if( checksum == filesum )
		{
			close(fid);
			return ERR_SUCCESS;
		}
    }
    //文件校验错误
	LogError("ReadParaFile",FILE_LINE,"checksum the file %s' error.",filename);
    close(fid);
    return ERR_CHECKSUM;
}

/*********************************************************************************************
 *
 * 功能描述     读文件头
 *
 * 参数说明      - filename		: 输入 	文件名称
 * 			     - fileheader 	: 输出 	文件头结构信息
 *
 * 返回代码
 *                成功或其它错误
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_ReadFileHead( char *filename, TFileHead *fileheader )
{
    struct stat filestat;
    char  fullname[50];
    DWORD filesize, length;
    WORD  checksum, filesum;
	STATUS rc;
	int fid;

	//恢复成带目录的文件名
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//打开文件
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
    {
        LogError("ReadFileHead",FILE_LINE,"Open the file %s error",filename);
        return ERR_OPEN;
    }
    
	//读文件状态信息    
    rc=fstat(fid,&filestat);
    if( rc == ERROR)
    {
        LogError("ReadFileHead",FILE_LINE,"read the file %s's status error",filename);
        close(fid);
        return ERR_FILESTATE;
    }

	//检查文件头长度与实际文件长度是否匹配
    filesize=filestat.st_size;
	length=sizeof(TFileHead);
    if( filesize < length )
    {
        LogError("ReadFileHead",FILE_LINE,"read head length of the file %s' error",filename);
        close(fid);
        return ERR_FILELEN;
    }

	//读文件，并检查长度是否有效
    rc = read( fid, (char *)fileheader, length );
    if( (rc == ERROR) || (length != rc) )
    {
        LogError("ReadFileHead",FILE_LINE,"read the file %s' error",filename);
        close(fid);
        return ERR_FILEREAD;
    }

	//文件头内容校验
    checksum = PCheckSum( (void *)fileheader,length-sizeof(WORD)); 
    filesum = ltobs(fileheader->dwCheck);
    if( checksum == filesum )
    {   
		close(fid);
		return 0;
    }

	//文件头校验错误
	LogError("ReadFileHead",FILE_LINE,"the file %s' checksum error",filename);
    close(fid);
    return ERR_CHECKSUM;
}

/*********************************************************************************************
 *
 * 功能描述     写参数文件
 *
 * 参数说明      - filename	: 输入 	文件名称
 * 			     - buf 		: 输入  写入的文件内容缓冲区
 * 			     - len 		: 输入  写入的文件内容长度
 *
 * 返回代码
 *                成功或其它错误
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_WriteParaFile( char *filename, BYTE *buf, DWORD len )
{
    char  fullname[50];
    STATUS rc;
	int fid;
    TFileHead *fhead;
    WORD checksum,headsize;

	//恢复带目录的全文件名称
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//检查读取长度参数的合法性
	if( len<=sizeof(TFileHead) )
	{
		LogError("WriteParaFile",FILE_LINE,"write file length error");
		return ERR_WRITELEN;
	}

	//对文件头和文件内容分别校验
    fhead = (TFileHead*)buf;
	headsize = sizeof(TFileHead);
    fhead->dwCheck = PCheckSum( (void *)buf, headsize-sizeof(WORD) ); 
    fhead->dwCheck = ltobs(fhead->dwCheck);
    checksum = PCheckSum( (void *)(buf+headsize),len-headsize-2 );
    checksum = ltobs(checksum);

	//打开文件
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
	{
        LogError("WriteParaFile",FILE_LINE,"Open the file %s error",filename);
		close(fid);
		return ERR_OPEN;
	}

	//写入文件内容
    rc = write( fid, (char*)buf, len-2 );
	if( rc == ERROR )
	{
        LogError("WriteParaFile",FILE_LINE,"write the file %s error",filename);
		close(fid);
		return ERR_WRITEFILE;
	}

	//写入文件校验
	rc = write( fid, (char*)(&checksum), sizeof(WORD) );
    if( rc == ERROR )
	{
        LogError("WriteParaFile",FILE_LINE,"write the file %s's checksum error",filename);
		close(fid);
		return ERR_WRITEFILE;
	}
	
    close(fid);
    return ERR_SUCCESS;
}

/*********************************************************************************************
 *
 * 功能描述     写文件头
 *
 * 参数说明      - filename		: 输入 	文件名称
 * 			     - fileheader 	: 输入 	文件头结构信息
 *
 * 返回代码
 *                成功或其它错误
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_WriteFileHead( char *filename, TFileHead fileheader )
{
    char  fullname[50];
    DWORD headsize;
    STATUS rc;
	int fid;

	//恢复带目录的全文件名
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//计算文件头校验
	headsize = sizeof(TFileHead);
    fileheader.dwCheck = PCheckSum( (void *)&fileheader, headsize-sizeof(WORD) );
    fileheader.dwCheck = ltobs(fileheader.dwCheck);

	//打开文件
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
	{
        LogError("WriteFileHeader",FILE_LINE,"Open the file %s error",filename);
		close(fid);
		return ERR_OPEN;
	}

	//写入文件头信息
    rc = write( fid, (char *)&fileheader, headsize );
	if( rc == ERROR )
	{
		close(fid);
		return ERR_WRITEFILE;
	}
	
    close(fid);
    return 0;
}

/*********************************************************************************************
 *
 * 功能描述     取文件头中的magic标志信息
 *
 * 参数说明      - filename	: 输入 	文件名称
 * 			     - magic 	: 输出 	文件头标志信息
 *
 * 返回代码
 *                成功或其它错误
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_GetMagic(char *filename,DWORD &magic)
{
    TFileHead fileheader;
    DWORD rc;

	rc= VX_ReadFileHead( filename, &fileheader );
    if( !rc )
        magic = ltobl(fileheader.dwMagic);

    return rc;
}

/*********************************************************************************************
 *
 * 功能描述     写文件头中的magic标志信息
 *
 * 参数说明      - filename	: 输入 	文件名称
 * 			     - magic 	: 输入 	文件头标志信息
 *
 * 返回代码
 *                成功或其它错误
 *
 * 其它说明：          
 *
**********************************************************************************************/
DWORD VX_SetMagic(char *filename,DWORD magic)
{
    TFileHead fileheader;
    DWORD rc;

	rc= VX_ReadFileHead( filename, &fileheader );
    if( !rc )
    {
        fileheader.dwMagic = ltobl(magic);
		rc = VX_WriteFileHead( filename, fileheader );
    }
    return rc;
}

/*********************************************************************************************
 *
 * 功能描述     取代码分区根目录(即磁盘分区)名称
 *
 * 参数说明      - dir	: 输出 	根目录名称
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void VX_GetCodeDir( char *dir )
{
	strcpy(dir, CODE_PARTITION);
}

/*********************************************************************************************
 *
 * 功能描述     取数据分区根目录(即磁盘分区)名称
 *
 * 参数说明      - dir	: 输出 	根目录名称
 *
 * 返回代码
 *                无
 *
 * 其它说明：          
 *
**********************************************************************************************/
void VX_GetDataDir( char *dir )
{
	strcpy(dir, DATA_PARTITION);
}

/*********************************************************************************************
 *
 * 功能描述     根据文件名恢复出带目录的全文件名
 *
 * 参数说明      - filename
 *                  类型      : 输入
 *                  说明      : 文件名
 *       		 - fullname
 *                  类型      : 输出
 *                  说明      : 带目录文件名
 *
 * 返回代码
 *                TRUE or FALSE
 *
 * 其它说明：     none     
 *
**********************************************************************************************/
BOOL ToFullName( char *filename, char *fullname )
{
    WORD dot=0;
    WORD len;

    // Clear fullname
    strcpy( fullname, "" );

	//单独处理vxworks文件
	if( ( !strcmp( filename, "vxWorks" ) ) || ( !strcmp( filename, "vxworks") ) )
	{
		strcpy( fullname, CODE_PARTITION);
		strcat( fullname, filename );
		return TRUE;
	}

    // Get the filename length
    len = strlen(filename);

    // Find the dot in filename
    while( len-- )
    {
        filename[len] = tolower(filename[len]);
        if( filename[len] == '.' )
        {
            dot=len;
        }
    }

    // If filename hasn't dot, the filename is error!
    if( dot==0 )
    {
        LogError("ToFullName",FILE_LINE,"the file name %s error",filename);
        return FALSE;
    }

    // Add path to filename according to file suffix
	if( !strcmp( filename+dot+1, "dar" ) )		 strcpy( fullname, PARA_DIR);
	else if( !strcmp( filename+dot+1, "dal" ) )  strcpy( fullname, PARA_DIR);
	else if( !strcmp( filename+dot+1, "rel" ) )  strcpy( fullname, PARA_DIR);
	//	  else if( !strcmp( filename+dot+1, "dat" ) )  strcpy( fullname, PARA_DIR);
	else if( !strcmp( filename+dot+1, "plo" ) )  strcpy( fullname, PARA_DIR);
	else if( !strcmp( filename+dot+1, "ci" ) )	 strcpy( fullname, PARA_DIR); //new!
	else if( !strcmp( filename, "test.bin" ) )	  strcpy( fullname, CODE_PARTITION);
	else if( !strcmp( filename, "hzk16"))		 strcpy( fullname, LCD_DIR);
	else if( !strcmp( filename+dot+1, "bin" ) )  strcpy( fullname, CODE_PARTITION);
	else if( !strcmp( filename+dot+1, "plc" ) )  strcpy( fullname, OTHERS_DIR);
	else if( !strcmp( filename+dot+1, "cfg" ) )  strcpy( fullname, DATA_DIR);
	else if( !strcmp( filename+dot+1, "dat" ) )  strcpy( fullname, DATA_DIR);
	else if( !strcmp( filename+dot+1, "ind" ) )  strcpy( fullname, DATA_DIR);
	else strcpy( fullname, OTHERS_DIR );
	
    // Create the fullname
    strcat( fullname, filename );

    return TRUE;
}



//临时定义文件校验函数
WORD PCheckSum(void *p,DWORD length)
{
    DWORD sum=0;
    DWORD i;
    BYTE *datap;
    
    datap=(BYTE *)p;
    for(i=0;i<length;i++)
    {
        sum+=datap[i];
        if(sum & 0xffff0000)
        {
            sum++;
            sum &= 0xffff;
        }
    }
    return((WORD)sum);
}
#endif

