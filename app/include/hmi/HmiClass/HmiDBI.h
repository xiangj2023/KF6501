/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiDBI.h
* 
*   软件模块：//双点遥信
*
* 
*	描述
* 
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2011-3-30		初始创建
* 
*************************************************************************/

#ifndef __HmiDBI_H
#define __HmiDBI_H

#define  BI_TYPE_DPI 2 //双点
#define  BI_TYPE_SPI 1 //单点

#define  DBI_VALUE_ON 2
#define  DBI_VALUE_OFF 1
#define  DBI_VALUE_ERR 3
#include "Paradef.h"

#include "HmiStatic.h"


class CHmiDoubleBI : public CHmiStatic  
{
public:
	CHmiDoubleBI();

	virtual ~CHmiDoubleBI();
	void OnPaint();

/*	BOOL Init(WORD x,WORD y, WORD OnId, WORD OffId, 
		unsigned char* bmpOff, unsigned char* bmpOn, unsigned char* bmpErr,
		WORD bmpWidth, WORD bmpHeight, WORD bmpBPL);*/
	BOOL GetDataFromDB();
	void UpdateData(BOOL bIsSave = TRUE);

	void SetDBIBitmap(unsigned char* Onbmp,unsigned char* Offbmp, 
					LONG Width, LONG Height, LONG bytePerLine,
					LONG nOnId,LONG nType = 1,LONG nOffId = -1, 
					unsigned char* Errbmp = NULL);
	

protected:
	LONG m_nOnId; //合位对应的遥信
	LONG m_nOffId; //分位对应的遥信

	LONG m_nType; //是双点遥信还是单点遥信 1单点， 2双点

	WORD m_wValue; //当前值， 分位为1， 合位为2， 不对位为3

	unsigned char* m_OnbmData; //合位位图数据
	unsigned char* m_OffbmData; //分位位图数据
	unsigned char* m_ErrbmData; //错位位图数据

	LONG m_nTimer; //刷新时钟， 不刷新为-1

};

/**********************/
#endif /* __HmiDBI_H */
