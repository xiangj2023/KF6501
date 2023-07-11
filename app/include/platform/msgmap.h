#ifndef _MSGMAP_H_
#define _MSGMAP_H_

#include "msgdefs.h"
#include "appcore.h"


#define ON_SM_TIMEOUT(theBase) \
{ SM_TIMEOUT,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnTimeOut},
	
#define ON_SM_NOTIFY(theBase) \
{ SM_NOTIFY,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnNotify},
	
#define ON_SM_DICHANGE(theBase) \
{ SM_DICHANGE,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnDIChange},

#define ON_SM_CANTXDRXDOK(theBase)	\
{SM_CANTXDRXDOK,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnCanHardWare},

#define ON_SM_PARACHANGE(theBase) \
{ SM_PARACHANGE,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnParaChange},

#define ON_SM_TASKREGISTER(theBase) \
{ SM_TASKREGISTER,0,AfxSig_vw,(AFX_PMSG)&theBase::OnTaskRegister},

#define ON_SM_READCOMM(theBase) \
{ SM_READCOMM,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnReadComm},

#define ON_SM_WRITECOMM(theBase) \
{ SM_WRITECOMM,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnWriteComm},

#define ON_SM_COMMIDLE(theBase) \
{ SM_COMMIDLE,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnCommIdle},

#define ON_SM_WRITECONT(theBase) \
{ SM_WRITECONT,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnWriteCont},

#define ON_SM_COMMTXDIDLE(theBase) \
{ SM_COMMTXDIDLE,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnCommTxdIdle},

#define ON_SM_SOE(theBase) \
{ SM_BIEVENT,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnSOE},

#define ON_SM_BIBURST(theBase) \
{ SM_BIBURST,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnBIBurst},

#define ON_SM_RELAYEVENT(theBase) \
{ SM_RELAYEVENT,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnRelayEvent},

#define ON_SM_RELAYDATA(theBase) \
{ SM_RELAYDATA,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnRelayChange},

#define ON_SM_DOE(theBase) \
{ SM_DOE,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnDOE},

#define ON_SM_DPIBURST(theBase) \
{ SM_DPIBURST,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnDPIBurst},

#define ON_SM_KEYDOWN(theBase) \
{ SM_KEYDOWN,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnKeyDown},

#define ON_SM_LCD(theBase) \
{ SM_LCD,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnLcd},

#define ON_SM_TRANSOIE(theBase) \
{ SM_TRANSOIE,0,AfxSig_vdw, (AFX_PMSG)&theBase::OnTransOIE},

#define ON_SM_SWITCH(theBase) \
{ SM_SWITCH,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnSwitch},

#define ON_SM_TIMING(theBase) \
{ SM_TIMING,0,AfxSig_vw,(AFX_PMSG)&theBase::OnTiming},

#define ON_SM_BOSELECT(theBase) \
{ SM_BOSELECT,0, AfxSig_vwww, (AFX_PMSG)&theBase::OnBOSelect},

#define ON_SM_BOSELECTECHO(theBase) \
{ SM_BOSELECTECHO, 0,AfxSig_vwww, (AFX_PMSG)&theBase::OnBOSelectEcho},

#define ON_SM_BOEXEC(theBase) \
{ SM_BOEXEC, 0,AfxSig_vwww, (AFX_PMSG)&theBase::OnBOExec},

#define ON_SM_BOEXECECHO(theBase) \
{ SM_BOEXECECHO, 0,AfxSig_vwww, (AFX_PMSG)&theBase::OnBOExecEcho},

#define ON_SM_BODIRECT(theBase) \
{ SM_BODIRECT, 0,AfxSig_vwww, (AFX_PMSG)&theBase::OnBODirect},

#define ON_SM_BODIRECTECHO(theBase) \
{ SM_BODIRECTECHO,0,AfxSig_vwww, (AFX_PMSG)&theBase::OnBODirectEcho},

#define ON_SM_BOTEST(theBase) \
{ SM_BOTEST,0, AfxSig_vwww, (AFX_PMSG)&theBase::OnBOTest},

#define ON_SM_AOSELECT(theBase) \
{ SM_AOSELECT, 0,AfxSig_vdwww, (AFX_PMSG)&theBase::OnAOSelect},

#define ON_SM_AOSELECTECHO(theBase) \
{ SM_AOSELECTECHO, 0,AfxSig_vdwwww, (AFX_PMSG)&theBase::OnAOSelectEcho},

#define ON_SM_AOEXEC(theBase) \
{ SM_AOEXEC, 0,AfxSig_vdwwww, (AFX_PMSG)&theBase::OnAOExec},

#define ON_SM_AOEXECECHO(theBase) \
{ SM_AOEXECECHO, 0,AfxSig_vdwwwww, (AFX_PMSG)&theBase::OnAOExecEcho},

#define ON_SM_AODIRECT(theBase) \
{ SM_AODIRECT, 0,AfxSig_vdwww, (AFX_PMSG)&theBase::OnAODirect},

#define ON_SM_AODIRECTECHO(theBase) \
{ SM_AODIRECTECHO,0,AfxSig_vdwwww, (AFX_PMSG)&theBase::OnAODirectEcho},

#define ON_SM_COEMAKE(theBase) \
{ SM_COEMAKE,0,AfxSig_vdwdwdww, (AFX_PMSG)&theBase::OnCOEMake},

#define ON_SM_SAMPLEEND(theBase) \
{ SM_SAMPLEEND,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnSampleEnd},
	
#define ON_SM_DAIDATAREADY(theBase) \
{ SM_DAIDATAREADY,0,AfxSig_vdwdw, (AFX_PMSG)&theBase::OnProcessDAIData},

#define ON_SM_DICHANGED(theBase) \
{ SM_DICHANGED,0,AfxSig_vdwwww, (AFX_PMSG)&theBase::OnProcChangeDI},

#define ON_SM_CALCULATECI(theBase) \
{ SM_CALCULATECI,0,AfxSig_vv, (AFX_PMSG)&theBase::OnCumulateCI},*/

#define ON_SM_CALIBRATION(theBase) \
{ SM_CALIBRATION, 0, AfxSig_vdwdw, (AFX_PMSG)&theBase::OnCalibration},

#define ON_SM_CALIBRATIONECHO(theBase) \
{ SM_CALIBRATION_ECHO, 0, AfxSig_vdwdw, (AFX_PMSG)&theBase::OnCalibrationEcho},

#define ON_SM_ADINT(theBase) \
{ SM_ADINT,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnADInt},

#define ON_SM_LCDALARM(theBase) \
{ SM_LCD_ALARM,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnAlarm},

#define ON_SM_MPPARACHANGE(theBase) \
{ SM_MP_CHANGE,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnMPChange},

#define ON_SM_POWERDOWN(theBase) \
{ SM_POWERDOWN,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnPowerDown},

//#define ON_SM_AUTOADJUSTSTEP1(theBase) \
//{ SM_AUTOADJUSTSTEP1,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnAutoAdjustStep1},
	
#define ON_SM_AUTOADJUST(theBase) \
{ SM_AUTOADJUST,0,AfxSig_vdwdwdw,(AFX_PMSG)&theBase::OnAutoAdjust},
	
#define ON_SM_AUTOADJUSTECHO(theBase) \
{ SM_AUTOADJUSTECHO,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnAutoAdjustEcho},
	
//#define ON_SM_AUTOADJUSTECHO2(theBase) \
//{ SM_AUTOADJUSTECHO2,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnAutoAdjustEcho2},

#define ON_SM_STARTDIST(theBase) \
{ SM_STARTDIST,0,AfxSig_vdwdwdww,(AFX_PMSG)&theBase::OnStartDist},
	
//#define ON_SM_THERMALOVERLOAD(theBase) \
//	{ SM_THERMALOVERLOAD,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnThermalOverLoad},

#define ON_SM_LEDCHECK(theBase) \
{ SM_LEDCHECK,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnLedCheck},

#define ON_SM_SAVREPORT(theBase) \
{ SM_SAVEREPORT,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnSaveReport},

#define ON_SM_RELAYRTMB(theBase) \
{ SM_RELAYRTMB,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnRelayRTMB},
	
#define ON_SM_BIPARACHANGE(theBase) \
{ SM_BIPARACHANGE,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnBiParaChange},
		
#define ON_SM_WRITESINGNALRW(theBase) \
{ SM_WRITESINGNALRW,0,AfxSig_vdwdw,(AFX_PMSG)&theBase::OnWriteSingnalRW},
#define ON_SM_SYS_RESET(theBase) \
{ SM_SYS_RESET,0,AfxSig_vv,(AFX_PMSG)&theBase::OnSysReset},

#define ON_SM_CALLSUBDATA(theBase) \
{ SM_CALLSUBDATA,0,AfxSig_vdwww,(AFX_PMSG)&theBase::OnCallSubData},

#define ON_SM_GETFLDATA(theBase) \
{ SM_GETFLDATA,0,AfxSig_vwdwdw,(AFX_PMSG)&theBase::OnGetFLData},

#define ON_SM_DATAISOK(theBase) \
{ SM_DATAISOK,0,AfxSig_vdww,(AFX_PMSG)&theBase::OnDataIsOk},	

#define ON_SM_STOPCALLDATA(theBase) \
{ SM_STOPCALLDATA,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnStopCallData},

#define ON_SM_STEPOVERCMD(theBase) \
{ SM_STEPOVERCMD,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnStepOverCmd},

#define ON_SM_STEPOVERECHO(theBase) \
{ SM_STEPOVERECHO,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnStepOverEcho},

#define ON_SM_LOGICCHANGED(theBase) \
{ SM_LOGICCHANGED,0,AfxSig_vv,(AFX_PMSG)&theBase::OnLogicChanged},

#define ON_SM_NETCARDPARACHG(theBase) \
	{ SM_NETCARDPARACHG,0,AfxSig_vv,(AFX_PMSG)&theBase::OnNetCardParaChange},

#define ON_SM_NOTIFYETHLINKED(theBase) \
{ SM_NOTIFYETHLINKED,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnEthLinked},

#define ON_SM_NOTIFYETHCLOSED(theBase) \
{ SM_NOTIFYETHCLOSED,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnEthClosed},

#define ON_SM_AIE(theBase) \
{ SM_AIEVENT,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnAIE},

#define ON_SM_JDXXPICK(thebase) \
{ SM_JDXXPICK,0,AfxSig_vv,(AFX_PMSG)&theBase::OnJDXXPick},

#define ON_SM_JDXXRESULT(thebase) \
{ SM_JDXXRESULT,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnJDXXResult},

#define ON_SM_DIST(theBase) \
{ SM_DISTEVENT,0,AfxSig_vdw,(AFX_PMSG)&theBase::OnDisturb},

#endif


