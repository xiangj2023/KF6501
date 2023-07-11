/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			HmiWnd.h
* 
*   ���ģ�飺 Һ������
*
* 
*	���� �����༰������Ϣӳ���ú궨��
* 
* 
* 
*   ���� 
*	�� DECLARE_HMIMESSAGE_MAP() messagemap������ ��Ҫʹ����Ϣӳ���CHmiWnd��������Ҫ���ඨ����ʹ�øú�
*   �� BEGIN_HMIMESSAGE_MAP(theClass, baseClass) END_HMIMESSAGE_MAP��Ҫ��.cpp�ļ��гɶԳ��֣� �м������Ϣӳ���
*
*	void DrawCaption()  ���Ʊ�������Һ����ʾ������
*	
*	//�������������صĺ��������غ���Ҫ���û��ຯ���Ŀ���CHmiWnd::OnWnd***������
*	virtual void OnWndInit(); ��ʼ�������� ÿ��show֮ǰϵͳ�Զ����ã�CHmiwnd�ĸú���Ϊ��
*
*	virtual void OnWndEnter(); enter����Ӧ������CHmiWnd�иú�����������ת��EnterWnd
*			������ظú��������һ���Ҫ��ת��EnterWnd�����Ե���CHmiWnd::OnWndEnter();
*	virtual void OnWndEsc(); Esc����Ӧ������CHmiWnd�иú�����������ת��EscWnd
*	virtual void OnWndAddSub(LONG AddSub); �Ӽ�����Ӧ������CHmiWnd�иú���Ϊ��
*	virtual void OnWndUpDown(LONG UpDown); ���¼���Ӧ������CHmiWnd�й���Ϊ�ƶ�������߷�ҳ
*	virtual void OnWndLeftRight(LONG LeftRight); ���Ҽ���Ӧ������CHmiWnd�й���Ϊ�ƶ�������߷�ҳ
*	virtual void OnWndTimer(LONG TimerId); ��ʱ����Ӧ������ ���ڶ�ʱˢ�¼��������ʱ����ʱ�������
*					CHmiWnd�й���Ϊ�Զ�ˢ�´�����ˢ�¼����Ϊ0�Ŀؼ�(Ŀǰֻ��CHmiDataStatic)				
* 
*
*  �޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2008-8-11		��ʼ����
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
//@/* ���崰������                                                                       */
//@/*----------------------------------------------------------------------------------------*/
//@#define DECLARE_CLASSNAME() \
//@public: \
//@	const char m_pszClassName[32] = ; \
//@	char* GetClassName(){return m_pszClassName;} 
//@



/*----------------------------------------------------------------------------------------*/
/* ���崰����Ϣӳ��                                                                       */
/*----------------------------------------------------------------------------------------*/
#define DECLARE_HMIMESSAGE_MAP() \
private: \
    static const HMI_MSGMAP_ENTRY _messageEntries[]; \
protected: \
    static const HMI_MSGMAP messageMap; \
	virtual const HMI_MSGMAP* GetMessageMap() const; 

/*----------------------------------------------------------------------------------------*/
/* ��Ϣӳ�����ڶ���                                                                     */
/*----------------------------------------------------------------------------------------*/
#define BEGIN_HMIMESSAGE_MAP(theClass, baseClass) \
    const HMI_MSGMAP* theClass::GetMessageMap() const \
        { return &theClass::messageMap; } \
        const HMI_MSGMAP theClass::messageMap = \
    { &baseClass::messageMap, &theClass::_messageEntries[0] }; \
    const HMI_MSGMAP_ENTRY theClass::_messageEntries[] = \
    { \
/*----------------------------------------------------------------------------------------*/
/* ��Ϣӳ�����                                                                           */
/*----------------------------------------------------------------------------------------*/
#define END_HMIMESSAGE_MAP() \
        {0, 0, 0,0,HmiSig_end, (HMI_PMSG)0 } \
    }; \


/*----------------------------------------------------------------------------------------*/
/* ������Ϣӳ�亯��                                                                       */
/*----------------------------------------------------------------------------------------*/
typedef void (CHmiWnd::*HMI_PMSG)(void);
//@typedef    void (CHmiWnd::*pfn_vv)(); //void (void)
//@typedef    void (CHmiWnd::*pfn_vw)(LONG);
//@typedef    void (CHmiWnd::*pfn_vwdw)(WORD,DWORD);

/*----------------------------------------------------------------------------------------*/
/* ��Ϣӳ����ڶ���                                                                       */
/*----------------------------------------------------------------------------------------*/
struct HMI_MSGMAP_ENTRY
{
    WORD        nMessage;               /*��Ϣ����*/
    UINT        nCode;                  /*controlcode or WM_NOTIFY code*/
	UINT		nID;					//control id
	UINT		nLastID;					//last control id for range
    WORD        nSig;                   /*��Ϣ��Ӧ����ں�������*/
    HMI_PMSG    pfn;                    /*��Ϣ��Ӧ����ں���*/
};

/*----------------------------------------------------------------------------------------*/
/* ������Ϣӳ�䶨��                                                                       */
/*----------------------------------------------------------------------------------------*/
struct HMI_MSGMAP
{
    const HMI_MSGMAP*       pBaseMap;   /*�������Ϣӳ�����*/
    const HMI_MSGMAP_ENTRY* lpEntries;  /*�����ڵ���Ϣ���*/
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
	NoneFun = -1, //-1��
	MoveFocus =0, //0�ƶ�����
	ChgPage =1, //1��ҳ
	Function =2, //����
	ChgValue = 3, //��ֵ
};

#define  CLASSNAME_LEN  32 //�����ַ��ĳ���
class CHmiWnd  : public CHmiObj
{
public:
	LONG SendMessage(UINT message, DWORD wParam = 0,LONG lParam = 0);
	void SetActiveObj(CHmiObj* pObj);
	void Show(DWORD param1=0, DWORD param2=0);
	//�رմ��ڣ�ĿǰΨһ��������ʹ�����´�showʱ�ٵ���һ��Ĭ�ϵ�init
	void Close(); 

	LONG WndProc(LONG MsgCode, LONG wParam, LONG lParam);
	//���ÿؼ����㣬ͬʱ���ݽ���ؼ�������
	void SetFocus(LONG nTaborder);
	void SetFocus(CHmiObj* pObj);
	virtual BOOL Init();
	const char* tr(const char* pszSrc,const char* pszComment=NULL);

	//���������Ҫ�ͼ̳еĺ���
	virtual BOOL OnWndInit();
	virtual void OnWndEnter();
	virtual void OnWndEsc();
	virtual void OnWndAddSub(LONG AddSub);
	virtual void OnWndUpDown(LONG UpDown);
	virtual void OnWndLeftRight(LONG LeftRight);
	virtual void OnWndTimer(LONG TimerId);
	
	//��Ϣӳ����õĺ���
	void OnPaint();
	void OnMoveFocus(LONG offset); //�ƶ�����
	void OnChgPage(LONG offset); //��ҳ

	BOOL OnShowWindow(); //the wnd is about to be shown
	virtual void OnTimer(LONG TimerID);

	CHmiWnd();//DWORD *pAppId);
	virtual ~CHmiWnd();
	CHmiObj* m_pActiveObj; //�����ڼ���Ŀؼ�
	
	//���ڿؼ��б�ָ�룬 ���Դ��ڶԿؼ��Ĳ�����ͨ����ָ�� 
	//֮�ʿؼ����ݱ����ھ��崰��ʵ���ڣ�ֻ�г�ʼ��ʱ
	//Ҫ�����ؼ���m_ppObjListָ�������
	CHmiObj** m_ppObjList; 
	LONG m_nObjNum; //�ؼ�����

	CHmiWnd* m_pEscWnd; // esc��Ŀ�Ĵ���
	CHmiWnd* m_pEnterWnd; //enter��Ŀ�Ĵ���

	LONG m_nUpDownFun; //���¼�����
	LONG m_nLeftRightFun; // ���Ҽ�����
	LONG m_nUpDownOffset; //���¼��Ľ����ƶ���
	LONG m_nLeftRightOffset;  //���Ҽ��Ľ����ƶ���

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
	//������Ӧ����
	BOOL OnEnter();
	BOOL OnEsc();
	BOOL OnRight();
	BOOL OnLeft();
	BOOL OnUp();
	BOOL OnDown();
	BOOL OnSub();
	BOOL OnAdd();

protected:
	//�Ƿ����Զ����Զ��庯����2010-9-6ȥ��
	/*BOOL m_bHaveEnterFun; //�Ƿ����Զ���س�����
	BOOL m_bHaveInitFun; //�Ƿ����Զ����ʼ������
	BOOL m_bHaveAddSubFun; //�Ƿ����Զ���Ӽ�������
	BOOL m_bHaveUpDownFun; //�Ƿ����Զ������¼���Ӧ����
	BOOL m_bHaveLeftRightFun; //�Ƿ����Զ������Ҽ���Ӧ����
	BOOL m_bHaveTimerFun; //�Ƿ����Զ��嶨ʱ������*/
	
	LONG m_nCurTaborder; //��ǰ����λ�ú�
	
	LONG m_nPageNum; //ҳ��
	LONG m_nCurPageIndex; //��ǰҳ��

	//�Ѱ�����Ϣת���ɴ�����Ϣ
	//������Ϣ���Ⱥ��������ݽ��յ���Ϣ���Զ�������Ӧ����Ϣ������
	BOOL OnWndMsg(UINT message, DWORD wParam, LONG lParam, LONG *Result);

	BOOL m_bIsVisible; //�����Ƿ�ɼ�
	BOOL m_bIsToolTip; //�Ƿ���ʾ��ʾ���ڸ����ؼ��

	// �����Ƿ�رգ� ���д��ڵ������ڶ�����������������ڣ�
	// �رյĴ�������ʾʱҪ���µ���Ĭ�ϵ�init������ û�رյĲ�����
	BOOL m_bIsClosed; 
	char m_pszClassName[CLASSNAME_LEN]; //����������
	BOOL m_bIsShowTitle;

public:
	void DrawCaption(); //���Ʊ���������ʾ������
    DECLARE_HMIMESSAGE_MAP()   

};

const HMI_MSGMAP_ENTRY* FindMessageEntry(const HMI_MSGMAP_ENTRY* lpEntry,
                                         UINT nMsg, UINT nCode, UINT nID);


/**********************/

#endif /* __HmiWnd_H */
