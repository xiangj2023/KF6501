// HmiSettingChgWnd.h: interface for the CHmiSettingChgWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HMISETTINGCHGWND_H__FC4FCC8B_7151_4CE5_A20C_5A3D7D4D54E8__INCLUDED_)
#define AFX_HMISETTINGCHGWND_H__FC4FCC8B_7151_4CE5_A20C_5A3D7D4D54E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HmiWnd.h"

class CHmiSettingChgWnd : public CHmiWnd  
{
public:
	CHmiSettingChgWnd();
	virtual ~CHmiSettingChgWnd();
	LONG m_nSettingAreaCode;
	LONG m_nSettingIndex;

};

#endif // !defined(AFX_HMISETTINGCHGWND_H__FC4FCC8B_7151_4CE5_A20C_5A3D7D4D54E8__INCLUDED_)
