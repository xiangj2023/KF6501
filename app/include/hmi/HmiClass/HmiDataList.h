/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiDataList.h
* 
*   软件模块：液晶界面 
*			CHmiDatalist 数据列表控件 及4个报告详情显示窗口
*			
* 
*	描述	CHmiDatalist 数据列表控件， 根据类型自动从数据库读取并显示数据
*			数据列表分两类情况
*			1)仅显示不修改的  直接显示从数据库中读取的值， 一次只读取当前显示的几条记录
*			2)需要接受修改的  适当的时机读取到buf中， 显示时显示buf中记录，修改的内容存入buf	

*	
*   函数
*		CSOEDetailDlg soe详情显示对话框
*		使用方法1)init 
*				2)ReadReport(LONG nItem,LONG DispIndex),参数 读取的soe报告的序号， 显示序号
*				3)DoModal(CHmiWnd* pWndReturn)参数 回车返回窗口指针
*
*		COperationDetailDlg 操作事项详情显示对话框
*		使用方法1)init 
*				2)ReadReport(LONG nItem,LONG DispIndex),参数为读取的操作记录的序号,显示序号
*				3)DoModal(CHmiWnd* pWndReturn)参数 回车返回窗口指针
*
*		CHmiAlarmDetailDlg 告警事项详情显示对话框
*		使用方法1)init 
*				2)ReadReport(LONG nItem,LONG DispIndex),参数为读取的告警报告的序号,显示序号
*				3)DoModal(CHmiWnd* pWndReturn)参数 回车返回窗口指针
*
*		CHmiStartDetailDlg 动作报告详情显示对话框
*		使用方法1)init 
*				2)ReadReport(LONG ItemIndex, LONG subItemIndex,LONG DispIndex),参数：动作报告号,分报告号,显示序号
*				3)DoModal(CHmiWnd* pWndReturn)参数 回车返回窗口指针
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-12-16		初始创建
* 
*************************************************************************/

#ifndef __HmiDataList_H
#define __HmiDataList_H

//////////////////////////////////////////////////////////////////////
#include "HmiWnd.h"
#include "HmiButton.h"
#include "HmiEdit.h"
#include "HmiList.h"
#include "HmiStatic.h"
#include "HmiDataStatic.h"
#include "Translator.h"


//定值，公用定值描述列表的最多条
#define MAX_DISP_COUNT 32

//积分电度路数
#define INTERGRAL_KWH_COUNT 2


//接受输入时焦点框的宽度,几个字符
#define  WIDTH_CF 6 //cf字符宽度
#define  WIDTH_DI_FILTERTIME 5 //di去抖时间
#define  WIDTH_CI_BASE 5 //电度底数
#define  WIDTH_CI_CONST 5 //电度脉冲常数

#define MAX_FDNUM 48  //最大故障电量个数

//数据列表支持的显示内容类型
enum EListType
{ 
	Action = 1, //1//动作报告
	Alarm, //2//告警报告
	Start, //3//启动报告
	Operation, //4//操作事项
	Soe, //5soe
	ListCI, //电度
	HAI, //HAi
	LAI, //LAI
	ListBI, //9 

	ConfigQuery, //10配置查询
	SettingQuery, //11定值查询
	ConfigChange, //12
	SoftSwitch, //13软压板
	ASQuery, // 14 公用定值查询
	SysStatus, //15 系统状态
	DIPara, //16开入属性，电度或遥信
	DIFilterTime, //17去抖时间
	DINor, //18是否取反
	CIBase, //19电度底数
	CIConst, //20脉冲常数
	SoePara, //21soe属性，是否记录
	CF, //系数
	BOTime,//遥控保持时间
	PortPara //端口规约
};




//数据列表控件
class CHmiDataList : public CHmiList  
{
public:
	
	DECLARE_HMI_TR()
	BOOL GetDataFromDB(); //读取要显示的内容
	CHmiDataList();
	CHmiDataList(LONG dataType);
	void UpdateData(BOOL bIsSave=FALSE);
	LONG GetItemBase();

	virtual ~CHmiDataList();
//	BOOL OnEnter();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnLeft();
	BOOL OnRight();
	BOOL OnAdd();
	BOOL OnSub();


	inline void SetType(LONG nType)
	{
		m_nDataType = nType;
	}
	inline LONG GetType()
	{
		return m_nDataType;
	}
	inline void SetAreaNo(LONG nAreaNoToShow)
	{
		m_nAreaNo = nAreaNoToShow;
	}
	inline LONG	GetAreaNo()
	{
		return m_nAreaNo;
	}

	BOOL Init();

private:

	LONG m_nDataType; //列表显示的数据类型,具体取值用宏定义，和工具软件中的定义保持一致
	LONG m_nRecordPerLine; //（预留功能，目前只能取值1） 每行显示几条记录，为了在一屏内多显示数据，
						//对于无选择操作的可以一行显示多个记录
						
	LONG m_nAreaNo; //当前显示的定值区号
	LONG m_nAllArea; //定值区数
	LONG m_nRunAreaNo; //运行定值区号
	LONG m_nDataPageNum; //当前显示的是数据记录的第几页
	LONG m_nAllItemNum; //所以记录的个数（不是当前显示的，是所有要显示的）

protected:
	
	BOOL ChgDIFilterTimeBuf(int nOffset); //修改开入去抖时间缓冲区
	BOOL ChgBINOBuf(int nOffset); //修改遥信取反缓冲区
	BOOL ChgBISOEParaBuf(int nOffset); //修改BI中wControl-D1是否产生soe
	BOOL ChgCFBufValue(LONG nOffset); //更改系数缓冲区
	BOOL ChgCIBaseBufValue(LONG nOffset); //更改电度底数
	BOOL ChgCIConstBufValue(LONG nOffset); //更改脉冲常数
	BOOL ChgSWValue(); //更改软压板值
	BOOL ChgCfgValue();//更改配置值
	BOOL ChgBOTime(LONG nOffset);//更改遥控保持时间
	BOOL ChgDIPara_wClassBuf(LONG nOffset);//更改DI属性
	BOOL ChgPortPara(LONG nOffset);

	void ChgQueryAera(BOOL isAdd = TRUE);

	//告警，动作， 启动， soe， 操作报告
	BOOL ShowAlarmReportDB();
	BOOL ShowActionReportDB();
	BOOL ShowStartReportDB(); //启动
	BOOL ShowSOEReportDB();
	BOOL ShowOperationReportDB();


	BOOL ShowBIValue(void);
	BOOL ShowSettingQueryDB(LONG nAreaNo); //显示数据库中的定值
	BOOL ShowSysSettingQueryDB(); //显示数据库中的公用定值
	BOOL ShowCIConstBuf();
	BOOL ShowBINorBuf();
	BOOL ShowDIPara_wFilterTimeBuf(); //显示修改缓冲区里的值
	BOOL ShowDIPara_wClassBuf(); //显示修改缓冲区里的值（DIWClass)
	BOOL ShowSOEParaBuf(); //BI是否产生带时标的SOE wControl的D1
	BOOL ShowSoftSwitchBuf(); //显示压板缓冲区
	BOOL ShowConfigDB(); //查询的时候直接显示数据库内的
	BOOL ShowConfigBuf();
//	void DispSoeDB(); //显示数据库Soe记录
	BOOL ShowCIValue();
	BOOL ShowCIBaseBuf(); //显示ci缓冲区的值
	BOOL ShowCFValueBuf(); //显示系数
	BOOL ShowBOLastTimeBuf();
	BOOL ShowPortPara();
};

#endif /* __HmiDataList_H */
