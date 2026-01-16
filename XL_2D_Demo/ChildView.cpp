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
		XL_2D_FillRectangle(&rect, &bg_color, 3, 10.0f);
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
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
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
