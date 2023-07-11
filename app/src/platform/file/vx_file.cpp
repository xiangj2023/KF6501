/********************************************************************************************
*                                                                                  
*        ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia)                                    
*                                                                                  
********************************************************************************************/
                          
/********************************************************************************************
*                                                                                  
* �ļ�����          
*			platform.cpp                                                      
*                                                                                  
*                                                                                  
* ���ģ��                                                                         
*           vxworksϵͳ�ļ������ӿ�                                                                       
*			                                                                
* ����                                                                             
*                                                                                  
*      �����vxworksϵͳ���ļ�ϵͳ�����ӿ�                          
*                                                                                        
*                                                                                  
* ����                                                                             
*                                                                                  
*      	VX_ReadFile			���ļ�
*      	VX_WriteFile		д�ļ�                                                              
*      	VX_DeleteFile   	ɾ���ļ�                                                                         
*      	VX_MkDir 	    	����Ŀ¼                                                                        
*      	VX_DeleteDir    	ɾ��Ŀ¼                                                                       
*      	VX_ReadParaFile 	�������ļ�                                                                       
*      	VX_ReadFileHeader   �������ļ�ͷ                                                                      
*      	VX_WriteParaFile   	д�����ļ�                                                                        
*      	VX_WriteFileHeader  д�����ļ�ͷ                                                                      
*      	VX_GetMagic       	�������ļ��е�magic��־                                                                     
*      	VX_SetMagic       	д�����ļ��е�magic��־                                                                     
*		VX_GetCodeRootDir	ȡ���������Ŀ¼(�����̷���)����
*		VX_GetDataRootDir	ȡ���ݷ�����Ŀ¼(�����̷���)����
*
* �޸���ʷ                                                                         
*                                                                                  
*      �޸���           �޸�����     �޸�����                                      
*                                                                                  
*      ZQZ               08/6/23    ��ʼ����                         
*                                                                                  
********************************************************************************************/


//ע����Ҫ�����ļ�д��󳤶��Ƿ��Զ��ۼӣ�������Ҫͨ��ioctl (fid, FIOTRUNC, len)����������

#include "vx_file.h"
#include "endian.h"

#if (OS_PLATFORM == OS_VXWORKS)

extern WORD PCheckSum(void *p,DWORD length);

/*********************************************************************************************
 *
 * ��������     ���ļ�
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 *       		 - offset	: ����	���ļ�������ƫ��
 * 		         - length	: ����	ϣ����ȡ�ļ��ĳ���
 * 		         - buf		: ���	���ļ�������
 * 		         - len		: ���	ʵ�ʶ�ȡ�ļ��ĳ���
 * 		         - control	: ����	���ļ��������Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ�ܻ���ļ�����
 *
 * ����˵����          
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

	// �����Ϸ��Լ��
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
 * ��������     д�ļ�,���ļ������������ȴ�����Ȼ��д�ļ�
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 *       		 - offset	: ����	д�ļ�������ƫ��
 * 		         - length	: ����	д���ļ��ĳ���
 * 		         - buf		: ����	д�ļ�������
 * 		         - type		: ����	д�ļ���־(ͷ/β/����)
 * 		         - control	: ����	д�ļ��������Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ�ܻ�д�ļ�����
 *
 * ����˵����          
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
		rc = creat( fullname, O_RDWR);	//���ش������ļ�ID
		if( rc == ERROR )
		{
			OS_UnLockM(FLASH_LOCK);
			LogError("OnWriteFile",FILE_LINE,"When create %s error!",filename);
			return FILE_ERROR;
		}
		//����رգ���Ϊcreat�������Ѿ��򿪣�������رջ�����ظ��򿪣�����´��ļ�����
		//�����ļ�ϵͳ�涨�ĸ��������ļ�ϵͳ�쳣
		close( rc );
	}

	// Open the file
	fid = open( fullname, O_RDWR, 0 );
	if( fid == ERROR )
	{
		OS_UnLockM(FLASH_LOCK);
		//close( fid ); 	//����ֱ��ɾ������Ϊfid�쳣
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
        rc = creat( fullname, O_RDWR);	//���ش������ļ�ID
        if( rc == ERROR )
        {
			OS_UnLockM(FLASH_LOCK);
            LogError("OnWriteFile",FILE_LINE,"When create %s error!",filename);
            return FILE_ERROR;
        }
		//����رգ���Ϊcreat�������Ѿ��򿪣�������رջ�����ظ��򿪣�����´��ļ�����
		//�����ļ�ϵͳ�涨�ĸ��������ļ�ϵͳ�쳣
        close( rc );
    }

    // Open the file
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
    {
		OS_UnLockM(FLASH_LOCK);
    	//close( fid );		//����ֱ��ɾ������Ϊfid�쳣
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
		rc = remove( fullname2 );		//ɾ��
		rc = mv( fullname, fullname2 );	//�ƶ�

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
 * ��������     ɾ���ļ�
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 * 			     - control  : ����  ���Ʊ�־
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
 * ��������     ȡ�ļ�����
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 * 			     - filesize : ���  ��ȡ���ļ�����
 *
 * ���ش���
 *                TRUE��FALSE
 *
 * ����˵����          
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
 * ��������     ����Ŀ¼,��Ŀ¼�������ظ�����
 *
 * ����˵��      - dirname	: ���� 	Ŀ¼����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
	//ע��mkdir����Ŀ¼���Զ��رո�Ŀ¼
    return FILE_OK;
}

/*********************************************************************************************
 *
 * ��������     ɾ��Ŀ¼
 *
 * ����˵��      - dirname	: ���� 	Ŀ¼����
 *
 * ���ش���
 *                �ɹ���ʧ��
 *
 * ����˵����          
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
 * ��������     �������ļ�
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 * 			     - buf 		: ���  �������ļ����ݻ�����
 * 			     - len 		: ���  ��ȡ���ļ�����
 *
 * ���ش���
 *                �ɹ�����������
 *
 * ����˵����          
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

	//���ļ�
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
    {
        LogError("ReadParaFile",FILE_LINE,"Open the file %s error",filename);
        return ERR_OPEN;
    }
    
	//���ļ�״̬��Ϣ    
    rc=fstat(fid,&filestat);
    if( rc == ERROR)
    {
        LogError("ReadParaFile",FILE_LINE,"read the file %s's status error",filename);
        close(fid);
        return ERR_FILESTATE;
    }

	//����ļ�����
    filesize=filestat.st_size;
    if( filesize < sizeof(TFileHead))
    {
        LogError("ReadParaFile",FILE_LINE,"read the file %s's length error",filename);
        close(fid);
        return ERR_FILELEN;
    }

	//��ȡ�����ļ�����
    rc = read( fid, (char*)buf, filesize);
    if( (rc == ERROR) || (filesize != rc) )
    {
        LogError("ReadParaFile",FILE_LINE,"read the file %s' error",filename);
        close(fid);
        return ERR_FILEREAD;
    }
	*len = rc;

	//���ļ�ͷ���ļ����ݷֱ�У��
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
    //�ļ�У�����
	LogError("ReadParaFile",FILE_LINE,"checksum the file %s' error.",filename);
    close(fid);
    return ERR_CHECKSUM;
}

/*********************************************************************************************
 *
 * ��������     ���ļ�ͷ
 *
 * ����˵��      - filename		: ���� 	�ļ�����
 * 			     - fileheader 	: ��� 	�ļ�ͷ�ṹ��Ϣ
 *
 * ���ش���
 *                �ɹ�����������
 *
 * ����˵����          
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

	//�ָ��ɴ�Ŀ¼���ļ���
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//���ļ�
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
    {
        LogError("ReadFileHead",FILE_LINE,"Open the file %s error",filename);
        return ERR_OPEN;
    }
    
	//���ļ�״̬��Ϣ    
    rc=fstat(fid,&filestat);
    if( rc == ERROR)
    {
        LogError("ReadFileHead",FILE_LINE,"read the file %s's status error",filename);
        close(fid);
        return ERR_FILESTATE;
    }

	//����ļ�ͷ������ʵ���ļ������Ƿ�ƥ��
    filesize=filestat.st_size;
	length=sizeof(TFileHead);
    if( filesize < length )
    {
        LogError("ReadFileHead",FILE_LINE,"read head length of the file %s' error",filename);
        close(fid);
        return ERR_FILELEN;
    }

	//���ļ�������鳤���Ƿ���Ч
    rc = read( fid, (char *)fileheader, length );
    if( (rc == ERROR) || (length != rc) )
    {
        LogError("ReadFileHead",FILE_LINE,"read the file %s' error",filename);
        close(fid);
        return ERR_FILEREAD;
    }

	//�ļ�ͷ����У��
    checksum = PCheckSum( (void *)fileheader,length-sizeof(WORD)); 
    filesum = ltobs(fileheader->dwCheck);
    if( checksum == filesum )
    {   
		close(fid);
		return 0;
    }

	//�ļ�ͷУ�����
	LogError("ReadFileHead",FILE_LINE,"the file %s' checksum error",filename);
    close(fid);
    return ERR_CHECKSUM;
}

/*********************************************************************************************
 *
 * ��������     д�����ļ�
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 * 			     - buf 		: ����  д����ļ����ݻ�����
 * 			     - len 		: ����  д����ļ����ݳ���
 *
 * ���ش���
 *                �ɹ�����������
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD VX_WriteParaFile( char *filename, BYTE *buf, DWORD len )
{
    char  fullname[50];
    STATUS rc;
	int fid;
    TFileHead *fhead;
    WORD checksum,headsize;

	//�ָ���Ŀ¼��ȫ�ļ�����
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//����ȡ���Ȳ����ĺϷ���
	if( len<=sizeof(TFileHead) )
	{
		LogError("WriteParaFile",FILE_LINE,"write file length error");
		return ERR_WRITELEN;
	}

	//���ļ�ͷ���ļ����ݷֱ�У��
    fhead = (TFileHead*)buf;
	headsize = sizeof(TFileHead);
    fhead->dwCheck = PCheckSum( (void *)buf, headsize-sizeof(WORD) ); 
    fhead->dwCheck = ltobs(fhead->dwCheck);
    checksum = PCheckSum( (void *)(buf+headsize),len-headsize-2 );
    checksum = ltobs(checksum);

	//���ļ�
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
	{
        LogError("WriteParaFile",FILE_LINE,"Open the file %s error",filename);
		close(fid);
		return ERR_OPEN;
	}

	//д���ļ�����
    rc = write( fid, (char*)buf, len-2 );
	if( rc == ERROR )
	{
        LogError("WriteParaFile",FILE_LINE,"write the file %s error",filename);
		close(fid);
		return ERR_WRITEFILE;
	}

	//д���ļ�У��
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
 * ��������     д�ļ�ͷ
 *
 * ����˵��      - filename		: ���� 	�ļ�����
 * 			     - fileheader 	: ���� 	�ļ�ͷ�ṹ��Ϣ
 *
 * ���ش���
 *                �ɹ�����������
 *
 * ����˵����          
 *
**********************************************************************************************/
DWORD VX_WriteFileHead( char *filename, TFileHead fileheader )
{
    char  fullname[50];
    DWORD headsize;
    STATUS rc;
	int fid;

	//�ָ���Ŀ¼��ȫ�ļ���
    strcpy( fullname, DATA_DIR );
    strcat( fullname, filename );

	//�����ļ�ͷУ��
	headsize = sizeof(TFileHead);
    fileheader.dwCheck = PCheckSum( (void *)&fileheader, headsize-sizeof(WORD) );
    fileheader.dwCheck = ltobs(fileheader.dwCheck);

	//���ļ�
    fid = open( fullname, O_RDWR, 0 );
    if( fid == ERROR )
	{
        LogError("WriteFileHeader",FILE_LINE,"Open the file %s error",filename);
		close(fid);
		return ERR_OPEN;
	}

	//д���ļ�ͷ��Ϣ
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
 * ��������     ȡ�ļ�ͷ�е�magic��־��Ϣ
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 * 			     - magic 	: ��� 	�ļ�ͷ��־��Ϣ
 *
 * ���ش���
 *                �ɹ�����������
 *
 * ����˵����          
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
 * ��������     д�ļ�ͷ�е�magic��־��Ϣ
 *
 * ����˵��      - filename	: ���� 	�ļ�����
 * 			     - magic 	: ���� 	�ļ�ͷ��־��Ϣ
 *
 * ���ش���
 *                �ɹ�����������
 *
 * ����˵����          
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
 * ��������     ȡ���������Ŀ¼(�����̷���)����
 *
 * ����˵��      - dir	: ��� 	��Ŀ¼����
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void VX_GetCodeDir( char *dir )
{
	strcpy(dir, CODE_PARTITION);
}

/*********************************************************************************************
 *
 * ��������     ȡ���ݷ�����Ŀ¼(�����̷���)����
 *
 * ����˵��      - dir	: ��� 	��Ŀ¼����
 *
 * ���ش���
 *                ��
 *
 * ����˵����          
 *
**********************************************************************************************/
void VX_GetDataDir( char *dir )
{
	strcpy(dir, DATA_PARTITION);
}

/*********************************************************************************************
 *
 * ��������     �����ļ����ָ�����Ŀ¼��ȫ�ļ���
 *
 * ����˵��      - filename
 *                  ����      : ����
 *                  ˵��      : �ļ���
 *       		 - fullname
 *                  ����      : ���
 *                  ˵��      : ��Ŀ¼�ļ���
 *
 * ���ش���
 *                TRUE or FALSE
 *
 * ����˵����     none     
 *
**********************************************************************************************/
BOOL ToFullName( char *filename, char *fullname )
{
    WORD dot=0;
    WORD len;

    // Clear fullname
    strcpy( fullname, "" );

	//��������vxworks�ļ�
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



//��ʱ�����ļ�У�麯��
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

