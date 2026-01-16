
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame() noexcept;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//CMFCToolBar		  m_wndToolBar;
	//CMFCToolBarImages	m_UserImages;

	CStatusBar        m_wndStatusBar;
	CChildView		  m_wndView;

protected:
	DECLARE_DYNAMIC(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
};


