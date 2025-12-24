#include "pch.h"
#include "framework.h"
#include "XL_2D_Demo.h"
#include "ChildView.h"
#include <chrono>
#include "XL_2D.hpp"

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
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CChildView::OnDrawFollowHeart()
{
	
}
