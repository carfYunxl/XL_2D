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
	ON_EN_CHANGE(IDR_SHAPE_EDIT, &XL_CAD_Frame::OnEnChangeEdit)
	ON_COMMAND(IDR_SHAPE_COMBO, &XL_CAD_Frame::OnIdrShapeCombo)
	ON_COMMAND(IDR_SHAPE_EDIT, &XL_CAD_Frame::OnIdrShapeEdit)
	ON_COMMAND(IDR_SHAPE_MENU, &XL_CAD_Frame::OnIdrShapeMenu)
	ON_COMMAND(ID_MENUBUTTON_1, &XL_CAD_Frame::OnMenubutton1)
	ON_COMMAND(ID_MENUBUTTON_2, &XL_CAD_Frame::OnMenubutton2)
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
	delete m_ToolBarEditBoxButton;
	delete m_ToolBarMenuButton;
}

int XL_CAD_Frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	InitView();
	InitToolBar();
	InitStatusBar();
	InitUserDockPanel();

	//  隐藏TitleBar  
	//ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
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
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return;
	}
	m_wndToolBar.SetWindowTextW(L"File");
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);

	m_ShapeToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_TOOLBAR_SHAPE);
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

	m_ShapeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_ShapeToolBar);

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
	m_ShapeToolBar.SetButtonText(6, _T("&Space"));
	m_ShapeToolBar.SetButtonText(7, _T("&Edit"));
	//m_ShapeToolBar.SetButtonText(8, _T("&Menu"));
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
