/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			HmiMSG.h
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
*		kangqian	2008-5-16		初始创建
* 
*************************************************************************/
#include "LcdDef.h"

#ifndef __HmiMSG_H
#define __HmiMSG_H

/*----------------------------------------------------------------------------------------*/
/* 消息映射类型定义                                                                       */
/*----------------------------------------------------------------------------------------*/
enum HmiSig
{
    HmiSig_end = 0,     // [marks end of message map]
        HmiSig_vv,
        HmiSig_vw,
        HmiSig_vwdw,
        HmiSig_vwdwdw,
        HmiSig_vwdwdwdw,
        HmiSig_vdwdwdww,
        HmiSig_vdwdwdw,
        HmiSig_vdwdw,
        HmiSig_vdw,
        HmiSig_vptrdw,
        HmiSig_vptrdwdw,           

        /* Add by Guowei for control message */
        HmiSig_vdww,
        HmiSig_vdwww,
        HmiSig_vdwwww,
        HmiSig_vdwwwww,
        HmiSig_vdwwwptrw,
        HmiSig_vdwdwptrw
};

struct THmiMessage
{
  DWORD lParam;     /*其中,LOWORD(lParam)表示消息类型*/
  DWORD hParam;     
  DWORD wParam;     
  DWORD pParam;     
};

enum EHmiMessage
{ 
    HMI_KEYDOWN = 0,      //键盘
	HMI_WND_TIMER,

	HMI_WND_PAINT,
	HMI_WND_INIT,
	HMI_WND_MOVEFOCUS,
	HMI_WND_CHGPAGE,
	HMI_WND_UPDOWN,
	HMI_WND_RIGHTLEFT,
	HMI_WND_ENTER,
	HMI_WND_ADDSUB,
	HMIWM_COMMAND,
	HMI_WND_SHOWWINDOW,

	HMILIST_SELCHANGE,
	HMILIST_ADDSUB,
	HMILIST_ENTER,
	HMILIST_LEFTRIGHT,
	HMILIST_UPDOWN,

	HMIBTN_ENTER,

	HMIEDIT_CHANGE,
	HMIEDIT_ENTER,
	HMIEDIT_ADDSUB,
	HMIEDIT_UPDOWN,
	HMIEDIT_LEFTRIGHT,

	HMICOMBO_ENTER,
	HMICOMBO_ADDSUB,

	HMICN_COMMAND,
};
/*-----------------------------------------------------------------------------------------

                        定义通用窗口消息映射

-----------------------------------------------------------------------------------------*/
#define ON_HMICOMMAND(id, memberFxn) \
	{ HMIWM_COMMAND, HMICN_COMMAND, (WORD)id, (WORD)id, HmiSig_vv, (HMI_PMSG)&memberFxn },
		// ON_COMMAND(id, OnFoo) is the same as
		//   ON_CONTROL(0, id, OnFoo) or ON_BN_CLICKED(0, id, OnFoo)

//@#define ON_HMIWM_ENTER() \
//@	{HMIWM_ENTER,0,0,0,HmiSig_vv, (HMI_PMSG)(void (CHmiWnd::*)(void))&OnEnter},


// for general controls
#define ON_HMICONTROL(wNotifyCode, id, memberFxn) \
	{ HMIWM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)id, HmiSig_vv, \
		(HMI_PMSG)&memberFxn },

#define ON_HMICONTROL_W(wNotifyCode, id, memberFxn) \
	{ HMIWM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)id, HmiSig_vw, \
		(HMI_PMSG)(void (CHmiWnd::*)(LONG))&memberFxn },

//window
#define ON_HMIWND_PAINT() \
{ HMI_WND_PAINT,0, 0,0, HmiSig_vv, (HMI_PMSG)(void (CHmiWnd::*)(void))&OnPaint},

#define ON_HMIWND_MOVEFOCUS() \
{ HMI_WND_MOVEFOCUS,0, 0,0, HmiSig_vw, (HMI_PMSG)(void (CHmiWnd::*)(LONG))&OnMoveFocus},

#define ON_HMIWND_CHGPAGE() \
{ HMI_WND_CHGPAGE,0, 0,0, HmiSig_vw, (HMI_PMSG)(void (CHmiWnd::*)(LONG))&OnChgPage},

#define ON_HMIWND_UPDOWN() \
{ HMI_WND_UPDOWN, 0, 0,0, HmiSig_vw, (HMI_PMSG)(void (CHmiWnd::*)(LONG))&OnWndUpDown},

#define ON_HMIWND_LEFTRIGHT() \
{ HMI_WND_RIGHTLEFT,0, 0,0, HmiSig_vw, (HMI_PMSG)(void (CHmiWnd::*)(LONG))&OnWndLeftRight},

#define ON_HMIWND_ENTER() \
{ HMI_WND_ENTER,0, 0,0, HmiSig_vv, (HMI_PMSG)(void (CHmiWnd::*)(void))&OnWndEnter},

#define ON_HMIWND_ADDSUB() \
{ HMI_WND_ADDSUB,0, 0,0, HmiSig_vw, (HMI_PMSG)(void (CHmiWnd::*)(LONG))&OnWndAddSub},


#define ON_HMIWND_INIT() \
{ HMI_WND_INIT,0, 0,0, HmiSig_vv, (HMI_PMSG)(void (CHmiWnd::*)(void))&OnWndInit},

#define ON_HMIWND_SHOWWINDOW() \
{ HMI_WND_SHOWWINDOW,0, 0,0, HmiSig_vv, (HMI_PMSG)(void (CHmiWnd::*)(void))&OnShowWindow},

#define ON_HMIWND_TIMER() \
{ HMI_WND_TIMER,0, 0,0, HmiSig_vv, (HMI_PMSG)(void (CHmiWnd::*)(void))&OnWndTimer},

// Edit Control Notification Codes
//@#define ON_EN_SETFOCUS(id, memberFxn) \
//@	ON_CONTROL(EN_SETFOCUS, id, memberFxn)
//@#define ON_EN_KILLFOCUS(id, memberFxn) \
//@	ON_CONTROL(EN_KILLFOCUS, id, memberFxn)
#define ON_HMIEDIT_CHANGE(id, memberFxn) \
	ON_HMICONTROL(HMIEDIT_CHANGE, id, memberFxn)

#define ON_HMIEDIT_ENTER(id, memberFxn) \
	ON_HMICONTROL(HMIEDIT_ENTER, id, memberFxn)

#define ON_HMIEDIT_ADDSUB(id, memberFxn) \
ON_HMICONTROL_W(HMIEDIT_ADDSUB, id, memberFxn)

#define ON_HMIEDIT_UPDOWN(id, memberFxn) \
ON_HMICONTROL_W(HMIEDIT_UPDOWN, id, memberFxn)

#define ON_HMIEDIT_LEFTRIGHT(id, memberFxn) \
ON_HMICONTROL_W(HMIEDIT_LEFTRIGHT, id, memberFxn)

//@#define ON_EN_UPDATE(id, memberFxn) \
//@	ON_CONTROL(EN_UPDATE, id, memberFxn)
//@#define ON_EN_ERRSPACE(id, memberFxn) \
//@	ON_CONTROL(EN_ERRSPACE, id, memberFxn)
//@#define ON_EN_MAXTEXT(id, memberFxn) \
//@	ON_CONTROL(EN_MAXTEXT, id, memberFxn)
//@#define ON_EN_HSCROLL(id, memberFxn) \
//@	ON_CONTROL(EN_HSCROLL, id, memberFxn)
//@#define ON_EN_VSCROLL(id, memberFxn) \
//@	ON_CONTROL(EN_VSCROLL, id, memberFxn)

// User Button Notification Codes
#define ON_HMIBTN_ENTER(id, memberFxn) \
	ON_HMICONTROL(HMIBTN_ENTER, id, memberFxn)
//@#define ON_BN_DOUBLECLICKED(id, memberFxn) \
//@	ON_CONTROL(BN_DOUBLECLICKED, id, memberFxn)
//@#define ON_BN_SETFOCUS(id, memberFxn) \
//@	ON_CONTROL(BN_SETFOCUS, id, memberFxn)
//@#define ON_BN_KILLFOCUS(id, memberFxn) \
//@	ON_CONTROL(BN_KILLFOCUS, id, memberFxn)


////////////////////////////////////////////////////////
// Listbox Notification Codes
#define ON_HMILIST_SELCHANGE(id, memberFxn) \
	ON_HMICONTROL(HMILIST_SELCHANGE, id, memberFxn)

#define ON_HMILIST_ENTER(id, memberFxn) \
	ON_HMICONTROL(HMILIST_ENTER, id, memberFxn)

#define ON_HMILIST_UPDOWN(id, memberFxn) \
	ON_HMICONTROL_W(HMILIST_UPDOWN, id, memberFxn)

#define ON_HMILIST_LEFTRIGHT(id, memberFxn) \
ON_HMICONTROL_W(HMILIST_LEFTRIGHT, id, memberFxn)

#define ON_HMILIST_ADDSUB(id, memberFxn) \
	ON_HMICONTROL_W(HMILIST_ADDSUB, id, memberFxn)

//@#define ON_HMILBN_VSCROLL(id, memberFxn)
//@	ON_HMICONTROL(HMILBN_VSCROLL, id, memberFxn)



//@#define ON_LBN_SETFOCUS(id, memberFxn) \
//@	ON_CONTROL(LBN_SETFOCUS, id, memberFxn)
//@#define ON_LBN_KILLFOCUS(id, memberFxn) \
//@	ON_CONTROL(LBN_KILLFOCUS, id, memberFxn)

#define ON_HMICOMBO_ADDSUB(id, memberFxn) \
ON_HMICONTROL_W(HMICOMBO_ADDSUB, id, memberFxn)

#define ON_HMICOMBO_ENTER(id, memberFxn) \
ON_HMICONTROL(HMICOMBO_ENTER, id, memberFxn)

/**********************/

#endif /* __HmiMSG_H */
