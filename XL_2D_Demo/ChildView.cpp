#include "pch.h"
#include "framework.h"
#include "XL_2D_Demo.h"
#include "ChildView.h"
#include <chrono>
#include "XL_2D.hpp"
#include "ShapeGenerator.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView(){
	
}

CChildView::~CChildView(){
	XL_2D_ReleaseRenderer();
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_DRAW_FOLLOW_HEART, &CChildView::OnDrawFollowHeart)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_BUTTON_RECTANGLE, &CChildView::OnButtonRectangle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RECTANGLE, &CChildView::OnUpdateButtonRectangle)
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_2, &CChildView::OnPropertyRectangleDivide2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_2, &CChildView::OnUpdatePropertyRectangleDivide2)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_3, &CChildView::OnPropertyRectangleDivide3)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_3, &CChildView::OnUpdatePropertyRectangleDivide3)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_4, &CChildView::OnPropertyRectangleDivide4)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_4, &CChildView::OnUpdatePropertyRectangleDivide4)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_5, &CChildView::OnPropertyRectangleDivide5)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_5, &CChildView::OnUpdatePropertyRectangleDivide5)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_6, &CChildView::OnPropertyRectangleDivide6)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_6, &CChildView::OnUpdatePropertyRectangleDivide6)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_8, &CChildView::OnPropertyRectangleDivide8)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_8, &CChildView::OnUpdatePropertyRectangleDivide8)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_16, &CChildView::OnPropertyRectangleDivide16)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_16, &CChildView::OnUpdatePropertyRectangleDivide16)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_32, &CChildView::OnPropertyRectangleDivide32)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_32, &CChildView::OnUpdatePropertyRectangleDivide32)
	ON_COMMAND(ID_PROPERTY_RECTANGLE_DIVIDE_USER_DEFINE, &CChildView::OnPropertyRectangleDivideUserDefine)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DIVIDE_USER_DEFINE, &CChildView::OnUpdatePropertyRectangleDivideUserDefine)
//	ON_COMMAND(ID_PROPERTY_RECTANGLE_DELETE, &CChildView::OnPropertyRectangleDelete)
ON_COMMAND(ID_PROPERTY_RECTANGLE_DELETE, &CChildView::OnPropertyRectangleDelete)
ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_DELETE, &CChildView::OnUpdatePropertyRectangleDelete)
ON_COMMAND(ID_PROPERTY_RECTANGLE_BORDER_WIDTH, &CChildView::OnPropertyRectangleBorderWidth)
ON_COMMAND(ID_PROPERTY_RECTANGLE_COLOR, &CChildView::OnPropertyRectangleColor)
ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_BORDER_WIDTH, &CChildView::OnUpdatePropertyRectangleBorderWidth)
ON_UPDATE_COMMAND_UI(ID_PROPERTY_RECTANGLE_COLOR, &CChildView::OnUpdatePropertyRectangleColor)
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	//m_Renderer->OnPaint();
	XL_2D_OnPaint();

	auto frame = (CMainFrame*)theApp.m_pMainWnd;
	CString strFps;
	strFps.Format(L"FPS: %lld", XL_2D_GetFrameRate());
	if (frame->m_wndStatusBar.m_hWnd != NULL)
		frame->m_wndStatusBar.SetPaneText(0, strFps);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect(&rect);
	//m_Renderer = std::make_unique<OpenglRender>(m_hWnd, rect.Width(), rect.Height());
	//m_Renderer->Init();
	XL_2D_CreateRenderer(m_hWnd, rect.Width(), rect.Height());

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	/*if (m_Renderer)
		m_Renderer->OnSize(cx,cy);*/
	XL_2D_OnSize(cx, cy);

	auto frame = (CMainFrame*)theApp.m_pMainWnd;
	CString strFps;
	strFps.Format(L"FPS: %d", XL_2D_GetFrameRate());
	if(frame->m_wndStatusBar.m_hWnd != NULL)
		frame->m_wndStatusBar.SetPaneText(0, strFps);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	if (!m_bMouseDown)
	{
		if (!m_bStartDraw)
		{
			XL_2D_OnMouseMove(point.x, point.y, false, true);
			Invalidate();
		}
		return;
	}

	CPoint offset;
	offset.x = point.x - m_StartPoint.x;
	offset.y = point.y - m_StartPoint.y;
	bool bSelected = true;
	if (m_bStartDraw)
	{
		bSelected = false;
		offset = point;
	}
	else
	{
		m_StartPoint = point;
	}

	XL_2D_OnMouseMove(offset.x, offset.y, bSelected, false);
	Invalidate();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}


void CChildView::OnDrawFollowHeart()
{
	ShapeGenerator gen;
	gen.DoModal();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	m_bMouseDown = true;
	m_StartPoint = point;

	if (m_bStartDraw)
	{
		XL_RectF rect{
			(float)m_StartPoint.x,
			(float)m_StartPoint.y,
			(float)m_StartPoint.x,
			(float)m_StartPoint.y
		};

		XL_ColorF bg_color{ 0.0f, 0.0f, 1.0f, 1.0f };
		XL_2D_FillRectangle(&rect, &bg_color, 5, 6.0f);
		Invalidate();
		return;
	}

	XL_2D_OnLButtonDown(point.x, point.y);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
	m_bMouseDown = false;
	if (m_bStartDraw)
	{
		m_EndPoint = point;
		return;
	}

	XL_2D_OnLButtonUp(point.x, point.y);
}

void CChildView::OnButtonRectangle()
{
	m_bStartDraw = !m_bStartDraw;
}

void CChildView::OnUpdateButtonRectangle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bStartDraw);
}

void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_CONTEXT_MENU_RECTANGLE);
	CMenu* pMenu = menu.GetSubMenu(0);
	pMenu->EnableMenuItem(ID_PROPERTY_RECTANGLE_DELETE, MF_BYCOMMAND | MF_ENABLED);

	HMENU hMenu = pMenu->GetSafeHmenu();
	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON;
	HWND hOwner = AfxGetMainWnd() ? AfxGetMainWnd()->GetSafeHwnd() : m_hWnd;

	// TrackPopupMenuEx 会把 WM_INITMENUPOPUP 发送到 hOwner，从而触发菜单更新（ON_UPDATE_COMMAND_UI）
	::TrackPopupMenuEx(hMenu, flags, point.x, point.y, hOwner, nullptr);
	pMenu->Detach();
	menu.DestroyMenu();
}

void CChildView::OnMouseHover(UINT nFlags, CPoint point)
{
	CWnd::OnMouseHover(nFlags, point);
}

void CChildView::OnMouseLeave()
{
	CWnd::OnMouseLeave();
}

void CChildView::OnPropertyRectangleDivide2()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 2.0f;
	rect->tess_level = m_tessLevel;
	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide2(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level-m_tessLevel) < 0.01f);
}
void CChildView::OnPropertyRectangleDivide3()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 3.0f;
	rect->tess_level = m_tessLevel;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide3(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivide4()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 4.0f;
	rect->tess_level = m_tessLevel;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide4(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivide5()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 5.0f;
	rect->tess_level = m_tessLevel;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide5(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivide6()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 6.0f;
	rect->tess_level = m_tessLevel;
	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide6(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivide8()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 8.0f;
	rect->tess_level = m_tessLevel;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide8(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivide16()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 16.0f;
	rect->tess_level = m_tessLevel;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide16(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivide32()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	m_tessLevel = 32.0f;
	rect->tess_level = m_tessLevel;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivide32(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDivideUserDefine()
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	rect->tess_level = 0.0f;

	Invalidate();
}

void CChildView::OnUpdatePropertyRectangleDivideUserDefine(CCmdUI* pCmdUI)
{
	auto rect = XL_2D_GetRect(m_RectangleID);
	pCmdUI->SetCheck(abs(rect->tess_level - m_tessLevel) < 0.01f);
}

void CChildView::OnPropertyRectangleDelete()
{
	int x = 1;
	x++;
}

void CChildView::OnUpdatePropertyRectangleDelete(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CChildView::OnPropertyRectangleBorderWidth()
{
	// TODO: 在此添加命令处理程序代码
}

void CChildView::OnPropertyRectangleColor()
{
	// TODO: 在此添加命令处理程序代码
}

void CChildView::OnUpdatePropertyRectangleBorderWidth(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CChildView::OnUpdatePropertyRectangleColor(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}
