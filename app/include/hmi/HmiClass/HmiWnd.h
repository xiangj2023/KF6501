/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiWnd.h
* 
*   软件模块： 液晶界面
*
* 
*	描述 窗口类及窗口消息映射用宏定义
* 
* 
* 
*   函数 
*	宏 DECLARE_HMIMESSAGE_MAP() messagemap声明， 凡要使用消息映射的CHmiWnd派生类需要在类定义中使用该宏
*   宏 BEGIN_HMIMESSAGE_MAP(theClass, baseClass) END_HMIMESSAGE_MAP需要在.cpp文件中成对出现， 中间放置消息映射宏
*
*	void DrawCaption()  绘制标题栏到液晶显示缓冲区
*	
*	//子类如果需可重载的函数，重载后还需要掉用基类函数的可以CHmiWnd::OnWnd***来调用
*	virtual void OnWndInit(); 初始化函数， 每次show之前系统自动调用，CHmiwnd的该函数为空
*
*	virtual void OnWndEnter(); enter键响应函数，CHmiWnd中该函数功能是跳转到EnterWnd
*			如果重载该函数，并且还需要调转到EnterWnd，可以调用CHmiWnd::OnWndEnter();
*	virtual void OnWndEsc(); Esc键响应函数，CHmiWnd中该函数功能是跳转到EscWnd
*	virtual void OnWndAddSub(LONG AddSub); 加减键响应函数，CHmiWnd中该函数为空
*	virtual void OnWndUpDown(LONG UpDown); 上下键响应函数，CHmiWnd中功能为移动焦点或者翻页
*	virtual void OnWndLeftRight(LONG LeftRight); 左右键响应函数，CHmiWnd中功能为移动焦点或者翻页
*	virtual void OnWndTimer(LONG TimerId); 定时器响应函数， 窗口定时刷新间隔大于零时按定时间隔调用
*					CHmiWnd中功能为自动刷新窗口中刷新间隔不为0的控件(目前只有CHmiDataStatic)				
* 
*
*  修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2008-8-11		初始创建
* 
*************************************************************************/

#ifndef __HmiWnd_H
#define __HmiWnd_H



#include "LcdDef.h"

// HmiWnd.h: interface                                for the CHmiWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "HmiMSG.h"
#include "HmiObj.h"
//@/*----------------------------------------------------------------------------------------*/
//@/* 定义窗口名称                                                                       */
//@/*----------------------------------------------------------------------------------------*/
//@#define DECLARE_CLASSNAME() \
//@public: \
//@	const char m_pszClassName[32] = ; \
//@	char* GetClassName(){return m_pszClassName;} 
//@



/*----------------------------------------------------------------------------------------*/
/* 定义窗口消息映射                                                                       */
/*----------------------------------------------------------------------------------------*/
#define DECLARE_HMIMESSAGE_MAP() \
private: \
    static const HMI_MSGMAP_ENTRY _messageEntries[]; \
protected: \
    static const HMI_MSGMAP messageMap; \
	virtual const HMI_MSGMAP* GetMessageMap() const; 

/*----------------------------------------------------------------------------------------*/
/* 消息映射的入口定义                                                                     */
/*----------------------------------------------------------------------------------------*/
#define BEGIN_HMIMESSAGE_MAP(theClass, baseClass) \
    const HMI_MSGMAP* theClass::GetMessageMap() const \
        { return &theClass::messageMap; } \
        const HMI_MSGMAP theClass::messageMap = \
    { &baseClass::messageMap, &theClass::_messageEntries[0] }; \
    const HMI_MSGMAP_ENTRY theClass::_messageEntries[] = \
    { \
/*----------------------------------------------------------------------------------------*/
/* 消息映射结束                                                                           */
/*----------------------------------------------------------------------------------------*/
#define END_HMIMESSAGE_MAP() \
        {0, 0, 0,0,HmiSig_end, (HMI_PMSG)0 } \
    }; \


/*----------------------------------------------------------------------------------------*/
/* 定义消息映射函数                                                                       */
/*----------------------------------------------------------------------------------------*/
typedef void (CHmiWnd::*HMI_PMSG)(void);
//@typedef    void (CHmiWnd::*pfn_vv)(); //void (void)
//@typedef    void (CHmiWnd::*pfn_vw)(LONG);
//@typedef    void (CHmiWnd::*pfn_vwdw)(WORD,DWORD);

/*----------------------------------------------------------------------------------------*/
/* 消息映射入口定义                                                                       */
/*----------------------------------------------------------------------------------------*/
struct HMI_MSGMAP_ENTRY
{
    WORD        nMessage;               /*消息类型*/
    UINT        nCode;                  /*controlcode or WM_NOTIFY code*/
	UINT		nID;					//control id
	UINT		nLastID;					//last control id for range
    WORD        nSig;                   /*消息对应的入口函数类型*/
    HMI_PMSG    pfn;                    /*消息对应的入口函数*/
};

/*----------------------------------------------------------------------------------------*/
/* 窗口消息映射定义                                                                       */
/*----------------------------------------------------------------------------------------*/
struct HMI_MSGMAP
{
    const HMI_MSGMAP*       pBaseMap;   /*基类的消息映射入口*/
    const HMI_MSGMAP_ENTRY* lpEntries;  /*本窗口的消息入口*/
};

union HmiMessageMapFunctions
{
    HMI_PMSG pHmifn;   // generic member function pointer
    
    // specific type safe variants for WM_COMMAND and WM_NOTIFY messages
    void (CHmiWnd::*pHmifn_vv)(); //void (void)
    void (CHmiWnd::*pHmifn_vw)(LONG);
    void (CHmiWnd::*pHmifn_vwdw)(WORD,DWORD);
    void (CHmiWnd::*pHmifn_vwdwdw)(WORD,DWORD,DWORD);
    void (CHmiWnd::*pHmifn_vwdwdwdw)(WORD,DWORD,DWORD,DWORD);
    void (CHmiWnd::*pHmifn_vdwdwdww)(DWORD,DWORD,DWORD,WORD);
    void (CHmiWnd::*pHmifn_vdwdwdw)(DWORD,DWORD,DWORD);
    void (CHmiWnd::*pHmifn_vdwdw)(DWORD,DWORD);
    void (CHmiWnd::*pHmifn_vptrdw)(PBYTE,DWORD);
    void (CHmiWnd::*pHmifn_vptrdwdw)(PBYTE,DWORD,DWORD);

    void (CHmiWnd::*pHmifn_vdw)(DWORD);
    void (CHmiWnd::*pHmifn_vdww)(DWORD,WORD);
    void (CHmiWnd::*pHmifn_vdwww)(DWORD,WORD,WORD);
    void (CHmiWnd::*pHmifn_vdwwww)(DWORD,WORD,WORD,WORD);
    void (CHmiWnd::*pHmifn_vdwwwww)(DWORD,WORD,WORD,WORD,WORD);

    void (CHmiWnd::*pHmifn_vdwwwptrw)(DWORD,WORD,WORD,PBYTE,WORD);
    void (CHmiWnd::*pHmifn_vdwdwptrw)(DWORD,DWORD,PBYTE,WORD);
};

enum EKeyFun
{ 
	NoneFun = -1, //-1无
	MoveFocus =0, //0移动焦点
	ChgPage =1, //1翻页
	Function =2, //函数
	ChgValue = 3, //改值
};

#define  CLASSNAME_LEN  32 //类名字符的长度
class CHmiWnd  : public CHmiObj
{
public:
	LONG SendMessage(UINT message, DWORD wParam = 0,LONG lParam = 0);
	void SetActiveObj(CHmiObj* pObj);
	void Show(DWORD param1=0, DWORD param2=0);
	//关闭窗口，目前唯一的作用是使窗口下次show时再调用一次默认的init
	void Close(); 

	LONG WndProc(LONG MsgCode, LONG wParam, LONG lParam);
	//设置控件焦点，同时根据焦点控件来翻屏
	void SetFocus(LONG nTaborder);
	void SetFocus(CHmiObj* pObj);
	virtual BOOL Init();
	const char* tr(const char* pszSrc,const char* pszComment=NULL);

	//子类如果需要就继承的函数
	virtual BOOL OnWndInit();
	virtual void OnWndEnter();
	virtual void OnWndEsc();
	virtual void OnWndAddSub(LONG AddSub);
	virtual void OnWndUpDown(LONG UpDown);
	virtual void OnWndLeftRight(LONG LeftRight);
	virtual void OnWndTimer(LONG TimerId);
	
	//消息映射调用的函数
	void OnPaint();
	void OnMoveFocus(LONG offset); //移动焦点
	void OnChgPage(LONG offset); //翻页

	BOOL OnShowWindow(); //the wnd is about to be shown
	virtual void OnTimer(LONG TimerID);

	CHmiWnd();//DWORD *pAppId);
	virtual ~CHmiWnd();
	CHmiObj* m_pActiveObj; //窗口内激活的控件
	
	//窗口控件列表指针， 所以窗口对控件的操作都通过该指针 
	//之际控件内容保存在具体窗口实例内，只有初始化时
	//要将各控件和m_ppObjList指针关联，
	CHmiObj** m_ppObjList; 
	LONG m_nObjNum; //控件个数

	CHmiWnd* m_pEscWnd; // esc键目的窗口
	CHmiWnd* m_pEnterWnd; //enter键目的窗口

	LONG m_nUpDownFun; //上下键功能
	LONG m_nLeftRightFun; // 左右键功能
	LONG m_nUpDownOffset; //上下键的焦点移动量
	LONG m_nLeftRightOffset;  //左右键的焦点移动量

	inline void SetObjNum(LONG num)
	{
		m_nObjNum = num;
	}
	LONG GetObjNum()
	{
		return m_nObjNum;
	}

	inline void SetPageNum(LONG pageNum)
	{
		m_nPageNum = pageNum;
	}

	inline void SetEnterWnd(CHmiWnd* pEnterWnd)
	{
		m_pEnterWnd = pEnterWnd;
	}
	inline CHmiWnd* GetEnterWnd()
	{
		return m_pEnterWnd;
	}

	inline void SetEscWnd(CHmiWnd* pEscWnd)
	{
		m_pEscWnd = pEscWnd;
	}
	inline CHmiWnd* GetEscWnd()
	{
		return m_pEscWnd;
	}

	inline void SetLRFun(LONG fun)
	{
		m_nLeftRightFun = fun;
	}
	inline LONG GetLRFun()
	{
		return m_nLeftRightFun ;
	}

	inline void SetLROffset(LONG offset)
	{
		m_nLeftRightOffset = offset;
	}

	inline LONG GetLROffset()
	{
		return m_nLeftRightOffset;
	}

	inline void SetUDOffset(LONG offset)
	{
		m_nUpDownOffset = offset;
	}

	inline LONG GetUDOffset()
	{
		return m_nUpDownOffset;
	}

	inline void SetUDFun(LONG fun)
	{
		m_nUpDownFun = fun;
	}

	inline LONG GetUDFun()
	{
		return m_nUpDownFun;
	}

   	BOOL TransKeyMesssage(WORD wKey);

	char* GetClassName();
	void SetClassName(char* name);
	
private:
	//按键响应函数
	BOOL OnEnter();
	BOOL OnEsc();
	BOOL OnRight();
	BOOL OnLeft();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnSub();
	BOOL OnAdd();

protected:
	//是否有自定义自定义函数，2010-9-6去掉
	/*BOOL m_bHaveEnterFun; //是否有自定义回车函数
	BOOL m_bHaveInitFun; //是否有自定义初始化函数
	BOOL m_bHaveAddSubFun; //是否有自定义加减键函数
	BOOL m_bHaveUpDownFun; //是否有自定义上下键响应函数
	BOOL m_bHaveLeftRightFun; //是否有自定义左右键响应函数
	BOOL m_bHaveTimerFun; //是否有自定义定时器函数*/
	
	LONG m_nCurTaborder; //当前焦点位置号
	
	LONG m_nPageNum; //页数
	LONG m_nCurPageIndex; //当前页号

	//把按键消息转换成窗口消息
	//任务消息调度函数，根据接收的消息，自动调度相应的消息处理函数
	BOOL OnWndMsg(UINT message, DWORD wParam, LONG lParam, LONG *Result);

	BOOL m_bIsVisible; //窗口是否可见
	BOOL m_bIsToolTip; //是否显示提示（在各个控件里）

	// 窗口是否关闭， 所有窗口的生存期都是整个程序的生存期，
	// 关闭的窗口再显示时要重新调用默认的init函数， 没关闭的不调用
	BOOL m_bIsClosed; 
	char m_pszClassName[CLASSNAME_LEN]; //窗口类名称
	BOOL m_bIsShowTitle;

public:
	void DrawCaption(); //绘制标题栏到显示缓冲区
    DECLARE_HMIMESSAGE_MAP()   

};

const HMI_MSGMAP_ENTRY* FindMessageEntry(const HMI_MSGMAP_ENTRY* lpEntry,
                                         UINT nMsg, UINT nCode, UINT nID);


/**********************/

#endif /* __HmiWnd_H */
