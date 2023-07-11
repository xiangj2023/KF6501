/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiDataStatic.h
* 
*   软件模块：
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
*		kangqian	2008-5-21		初始创建
* 
*************************************************************************/

#ifndef __HmiDataStatic_H
#define __HmiDataStatic_H


// HmiDataStatic.h: interface for the CHmiDataStatic class.
//
//////////////////////////////////////////////////////////////////////
//@	WORD			nAI;				//遥测个数	
//@	WORD			nBI;				//遥信个数	
//@	WORD			nCI;				//电度个数	
//@	WORD			nBO;				//遥控个数	
//@	WORD			nAO;				//遥调个数	

#include "Paradef.h"
enum EDataType
{
	hAI = 0, //遥测一次
	lAI, //遥测二次
	phAI, //保护一次
	plAI, //保护二次
	BI, //遥信
	CI, //电度
	Time, //时间
	DI,	//直接采样点，包括CI和BI
};

#include "HmiStatic.h"

class CHmiDataStatic : public CHmiStatic  
{
public:
	CHmiDataStatic();
	virtual ~CHmiDataStatic();
	BOOL Init();
	BOOL GetDataFromDB();
	void UpdateData(BOOL bIsSave = TRUE);

	inline void SetDataType(LONG type)
	{
		m_nDataType = type;
	}
	inline LONG  GetDataType()
	{
		return m_nDataType;
	}
	inline void SetOffset(LONG offset)
	{
		m_nDataOffset = offset;
	}
	inline LONG GetOffset()
	{
		return m_nDataOffset;
	}

	inline void SetDataSource(LONG type, LONG offset)
	{
		m_nDataType = type;
		m_nDataOffset = offset;
	}
protected:
	LONG m_nDataType; //显示的数据类型类型
	LONG m_nDataOffset; //显示的数据在数据表中的便宜
	LONG m_nTimer; //刷新时钟， 不刷新为-1
};


/**********************/

#endif /* __HmiDataStatic_H */
