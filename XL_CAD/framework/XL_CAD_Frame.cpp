#include "XL_PCH.h"
#include "framework.h"
#include "XL_CAD_App.h"
#include "XL_CAD_Frame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(XL_CAD_Frame, CFrameWndEx)

BEGIN_MESSAGE_MAP(XL_CAD_Frame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_CBN_SELCHANGE(IDR_SHAPE_COMBO, &XL_CAD_Frame::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDR_SHAPE_SHOW_MODE, &XL_CAD_Frame::OnCbnSelchangeShowMode)
	ON_EN_CHANGE(IDR_SHAPE_EDIT, &XL_CAD_Frame::OnEnChangeEdit)
	ON_COMMAND(IDR_SHAPE_COMBO, &XL_CAD_Frame::OnIdrShapeCombo)
	ON_COMMAND(IDR_SHAPE_EDIT, &XL_CAD_Frame::OnIdrShapeEdit)
	ON_COMMAND(IDR_SHAPE_MENU, &XL_CAD_Frame::OnIdrShapeMenu)
	ON_COMMAND(ID_MENUBUTTON_1, &XL_CAD_Frame::OnMenubutton1)
	ON_COMMAND(ID_MENUBUTTON_2, &XL_CAD_Frame::OnMenubutton2)
	ON_COMMAND(IDR_SHAPE_SHOW_MODE, &XL_CAD_Frame::OnIdrShapeShowMode)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

XL_CAD_Frame::XL_CAD_Frame() noexcept
{
}

XL_CAD_Frame::~XL_CAD_Frame()
{
	delete m_ToolBarComboBoxButton;
	delete m_ToolBarComboBoxShowMode;
	delete m_ToolBarEditBoxButton;
	delete m_ToolBarMenuButton;
}

int XL_CAD_Frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);
#if 0
	// 先创建并加载 CMFCMenuBar
	if (!m_wndMenuBar.Create(this))
	{
		return -1;
	}
	// 可选：设置样式
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);

	//m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC);
	m_wndMenuBar.EnableCustomizeButton(TRUE, -1, _T(""));
	// first parameter is the command ID for the button of the Help combo box
	// third parameter is the width of the button for the combo box in pixels.
	m_wndMenuBar.EnableHelpCombobox(1, _T("enter text here"), 30);
	m_wndMenuBar.EnableMenuShadows();
	m_wndMenuBar.SetMaximizeMode(true);

	//m_wndMenuBar.SetForceDownArrows(FALSE);
	//CMFCPopupMenu::SetForceMenuFocus(FALSE);
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	//m_wndMenuBar.SetControlBarStyle(AFX_CBRS_RESIZE);

	//if (!m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME))
	//{
	//	TRACE0("未能加载菜单资源到 CMFCMenuBar\n");
	//}
	// 把菜单栏停靠到框架顶部（与工具栏一致的停靠机制）
	DockPane(&m_wndMenuBar);
#endif
	InitView();
	InitToolBar();
	InitStatusBar();
	InitUserDockPanel();

	// 移除 caption/border 后，额外清除扩展样式与边框样式并强制更新框架
	// 放在 InitUserDockPanel() 之后或窗口创建完成后执行
#if 0
	LONG ex = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	ex &= ~(WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_EX_STATICEDGE);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, ex);

	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~(WS_CAPTION | WS_BORDER | WS_DLGFRAME | WS_THICKFRAME);
	::SetWindowLong(m_hWnd, GWL_STYLE, style);

	// 通知系统样式改变并重绘非客户区
	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
#endif
	//  隐藏TitleBar  
	//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
	//ModifyStyle(WS_BORDER, 0, SWP_FRAMECHANGED);
	//  显示TitleBar  
	//ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);

	return 0;
}

BOOL XL_CAD_Frame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

#ifdef _DEBUG
void XL_CAD_Frame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void XL_CAD_Frame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif

void XL_CAD_Frame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndView.SetFocus();
}

#define ID_COMBO_SHAPES 1991

void XL_CAD_Frame::InitToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | /*CBRS_GRIPPER |*/ CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return;
	}
	m_wndToolBar.SetWindowTextW(L"File");
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);

	m_ShapeToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP /*| CBRS_GRIPPER*/ | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_TOOLBAR_SHAPE);
	m_ShapeToolBar.LoadToolBar(IDR_TOOLBAR_SHAPE, 0, 0, TRUE);
	m_ShapeToolBar.SetPermament(TRUE);
	m_ShapeToolBar.SetShowTooltips(TRUE);
	m_ShapeToolBar.SetWindowTextW(_T("Shape"));

	// ComboBox Button
	m_ToolBarComboBoxButton = new CMFCToolBarComboBoxButton(
		IDR_SHAPE_COMBO, GetCmdMgr()->GetCmdImage(IDR_SHAPE_COMBO),
		CBS_DROPDOWNLIST, 
		80
	);
	m_ToolBarComboBoxButton -> EnableWindow(TRUE);
	m_ToolBarComboBoxButton->SetCenterVert();
	m_ToolBarComboBoxButton->SetDropDownHeight(200);
	m_ToolBarComboBoxButton->SetFlatMode();

	m_ToolBarComboBoxButton->AddItem(_T("2D"));
	m_ToolBarComboBoxButton->AddItem(_T("3D"));
	m_ToolBarComboBoxButton->SelectItem(0);
	m_ShapeToolBar.ReplaceButton(IDR_SHAPE_COMBO, *m_ToolBarComboBoxButton);

	// Edit Button
	m_ToolBarEditBoxButton = new CMFCToolBarEditBoxButton(
		IDR_SHAPE_EDIT, NULL,
		128,60
	);
	m_ToolBarEditBoxButton->EnableWindow(TRUE);
	m_ToolBarEditBoxButton->HaveHotBorder();
	m_ToolBarEditBoxButton->SetFlatMode();
	m_ToolBarEditBoxButton->OnSize(50);
	m_ToolBarEditBoxButton->SetContents(L"2.1");

	m_ShapeToolBar.ReplaceButton(IDR_SHAPE_EDIT, *m_ToolBarEditBoxButton);

	// Menu Button
	CMenu menu;
	menu.LoadMenu(IDR_MENU_TOOLBAR);
	menu.m_hMenu = menu.GetSubMenu(0)->m_hMenu;
	m_ToolBarMenuButton = new CMFCToolBarMenuButton(IDR_SHAPE_MENU, menu.m_hMenu, GetCmdMgr()->GetCmdImage(IDR_SHAPE_MENU), _T(""));
	m_ShapeToolBar.ReplaceButton(IDR_SHAPE_MENU, *m_ToolBarMenuButton);
	menu.Detach();

	// Show Mode ComboBox Button
	m_ToolBarComboBoxShowMode = new CMFCToolBarComboBoxButton(
		IDR_SHAPE_SHOW_MODE, GetCmdMgr()->GetCmdImage(IDR_SHAPE_SHOW_MODE),
		CBS_DROPDOWNLIST,
		80
	);
	m_ToolBarComboBoxShowMode->EnableWindow(TRUE);
	m_ToolBarComboBoxShowMode->SetCenterVert();
	m_ToolBarComboBoxShowMode->SetDropDownHeight(200);
	m_ToolBarComboBoxShowMode->SetFlatMode();
	m_ShapeToolBar.ReplaceButton(IDR_SHAPE_SHOW_MODE, *m_ToolBarComboBoxShowMode);
	m_ShapeToolBar.RecalcLayout();
	m_ShapeToolBar.UpdateWindow();

	//m_ShapeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_ShapeToolBar);

	int idx = m_ShapeToolBar.CommandToIndex(IDR_SHAPE_SHOW_MODE);
	if (idx >= 0)
	{
		CMFCToolBarButton* pBtn = m_ShapeToolBar.GetButton(idx);
		CMFCToolBarComboBoxButton* pComboBtn = DYNAMIC_DOWNCAST(CMFCToolBarComboBoxButton, pBtn);
		if (pComboBtn)
		{
			pComboBtn->AddItem(_T("GL_Fill"));
			pComboBtn->AddItem(_T("GL_Line"));
			pComboBtn->SelectItem(0); // 让工具栏内部的按钮设置当前项

			CComboBox* pCombo = pComboBtn->GetComboBox(); // 这才是实际的 combobox
			if (pCombo)
			{
				pCombo->SetCurSel(0);
			}
		}
	}

	m_wndToolBar.SetButtonText(0, _T("&Open"));
	m_wndToolBar.SetButtonText(1, _T("&Save"));
	m_wndToolBar.SetButtonText(2, _T("&Copy"));
	m_wndToolBar.SetButtonText(4, _T("&Cut"));
	m_wndToolBar.SetButtonText(5, _T("&Paste"));
	m_wndToolBar.EnableTextLabels();
	m_wndToolBar.EnableToolTips();

	m_ShapeToolBar.SetButtonText(0, _T("&Point"));
	m_ShapeToolBar.SetButtonText(1, _T("&Line"));
	m_ShapeToolBar.SetButtonText(2, _T("&Rectangle"));
	m_ShapeToolBar.SetButtonText(3, _T("&Circle"));
	m_ShapeToolBar.SetButtonText(4, _T("&Triangle"));
	m_ShapeToolBar.SetButtonText(5, _T("&Ellipse"));
	m_ShapeToolBar.SetButtonText(6, _T("&Polygon"));
	m_ShapeToolBar.SetButtonText(7, _T("&Mode"));
	m_ShapeToolBar.SetButtonText(9, _T("&Space"));
	m_ShapeToolBar.SetButtonText(10, _T("&Edit"));
	//m_ShapeToolBar.SetButtonText(11, _T("&Menu"));
	m_ShapeToolBar.EnableTextLabels();
	m_ShapeToolBar.EnableToolTips();
}

void XL_CAD_Frame::InitStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
}

void XL_CAD_Frame::InitView()
{
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("未能创建视图窗口\n");
		return;
	}
}

void XL_CAD_Frame::InitUserDockPanel()
{
	if (!m_wndProperty.Create(
		_T("属性"),
		this,
		CRect(0, 350, 260, 500),
		TRUE,
		ID_VIEW_PROPERTY_WND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI
	)
		)
		return;
	m_wndProperty.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperty.SetControlBarStyle(~AFX_CBRS_CLOSE);
	//m_wndProperty.SetControlBarStyle(AFX_CBRS_RESIZE);
	DockPane(&m_wndProperty);
}

BOOL XL_CAD_Frame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void XL_CAD_Frame::OnIdrShapeCombo()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_Frame::OnIdrShapeEdit()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_Frame::OnIdrShapeMenu()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_Frame::OnCbnSelchangeCombo()
{
	AfxMessageBox(L"Combo Change");
}

void XL_CAD_Frame::OnCbnSelchangeShowMode()
{
	CWnd* pWndCombo = m_ShapeToolBar.GetDlgItem(IDR_SHAPE_SHOW_MODE);
	if (!pWndCombo)
		return;

	CComboBox* pCombo = DYNAMIC_DOWNCAST(CComboBox, pWndCombo);
	if (!pCombo)
		return;

	int nSel = pCombo->GetCurSel();
	switch (static_cast<PolygonMode>(nSel))
	{
		case PolygonMode_Fill:
		{
			XL_2D_SetPolygonMode(PolygonMode_Fill);
			break;
		}
		case PolygonMode_Line:
		{
			XL_2D_SetPolygonMode(PolygonMode_Line);
			break;
		}
	default:
		break;
	}
}

void XL_CAD_Frame::OnEnChangeEdit()
{
	//m_ToolBarEditBoxButton->
	TRACE("Edit change...\n");
}

void XL_CAD_Frame::OnMenubutton1()
{
	AfxMessageBox(L"Menu Button 1");
}

void XL_CAD_Frame::OnMenubutton2()
{
	AfxMessageBox(L"Menu Button 2");
}

void XL_CAD_Frame::OnIdrShapeShowMode()
{
	// TODO: 在此添加命令处理程序代码
}
