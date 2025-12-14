#include "pch.h"
#include "framework.h"
#include "XL_2D_Demo.h"
#include "ChildView.h"
#include <chrono>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView(){
}

CChildView::~CChildView(){
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
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
	
	auto now = std::chrono::system_clock::now();
	if(m_Renderer)
		m_Renderer->OnPaint();

	auto end = std::chrono::system_clock::now();
	auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(end-now).count();
	TRACE("FPS: %.2f\n", 1000.0f / (float)time_span);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	GetClientRect(&rect);

	WindowProp prop;
	prop.Width = rect.Width();
	prop.Height = rect.Height();
	m_Renderer = std::make_unique<OpenglRender>(m_hWnd, prop);
	m_Renderer->Init();

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_Renderer)
		m_Renderer->OnSize(cx,cy);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_Renderer)
	{
		m_Renderer->OnKeyDown(nChar, nRepCnt);
		m_Renderer->OnPaint();
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_Renderer)
	{
		m_Renderer->OnMouseWheel(zDelta);
		m_Renderer->OnPaint();
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_Renderer)
	{
		m_Renderer->OnMouseMove(point.x, point.y);
		m_Renderer->OnPaint();
	}

	CWnd::OnMouseMove(nFlags, point);
}
