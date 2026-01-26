#pragma once
#include "XL_CAD_View.h"
#include "XL_PropertyWnd.h"

class XL_CAD_Frame : public CFrameWndEx
{
	
public:
	XL_CAD_Frame() noexcept;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual ~XL_CAD_Frame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CMFCToolBar		  m_wndToolBar;
	CMFCToolBarImages	m_UserImages;

	CMFCToolBar		  m_ShapeToolBar;

	CStatusBar        m_wndStatusBar;
	XL_CAD_View		  m_wndView;

	XL_PropertiesWnd	m_wndProperty;

protected:
	DECLARE_DYNAMIC(XL_CAD_Frame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);

	afx_msg void OnIdrShapeCombo();
	afx_msg void OnIdrShapeEdit();
	afx_msg void OnIdrShapeMenu();
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangeEdit();
	DECLARE_MESSAGE_MAP()
private:
	void InitToolBar();
	void InitStatusBar();
	void InitView();
	void InitUserDockPanel();
private:
	CMFCToolBarComboBoxButton*	m_ToolBarComboBoxButton{ nullptr };
	CMFCToolBarEditBoxButton*	m_ToolBarEditBoxButton{ nullptr };
	CMFCToolBarMenuButton*		m_ToolBarMenuButton{ nullptr };

public:
	afx_msg void OnMenubutton1();
	afx_msg void OnMenubutton2();
};


