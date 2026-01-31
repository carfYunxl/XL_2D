#include "XL_PCH.h"
#include "framework.h"
#include "XL_CAD_App.h"
#include "XL_CAD_View.h"
#include <chrono>
#include "XL_2D.hpp"
#include "XL_ShapeGenerator.h"
#include "XL_CAD_Frame.h"
#include <iostream>
#include "XL_FuncHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

XL_CAD_View::XL_CAD_View(){
	
}

XL_CAD_View::~XL_CAD_View(){
	XL_2D_ReleaseRenderer();
}

BEGIN_MESSAGE_MAP(XL_CAD_View, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()

	ON_COMMAND(IDR_SHAPE_POINT, &XL_CAD_View::OnIdrShapePoint)
	ON_COMMAND(IDR_SHAPE_LINE, &XL_CAD_View::OnIdrShapeLine)
	ON_COMMAND(IDR_SHAPE_RECT, &XL_CAD_View::OnIdrShapeRect)
	ON_COMMAND(IDR_SHAPE_TRIANGLE, &XL_CAD_View::OnIdrShapeTriangle)
	ON_COMMAND(IDR_SHAPE_CIRCLE, &XL_CAD_View::OnIdrShapeCircle)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_POINT, &XL_CAD_View::OnUpdateIdrShapePoint)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_EDIT_OPEN, &XL_CAD_View::OnEditOpen)
	ON_COMMAND(ID_EDIT_SAVE, &XL_CAD_View::OnEditSave)
	ON_COMMAND(ID_EDIT_COPY, &XL_CAD_View::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &XL_CAD_View::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &XL_CAD_View::OnEditPaste)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_TRIANGLE, &XL_CAD_View::OnUpdateIdrShapeTriangle)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_RECT, &XL_CAD_View::OnUpdateIdrShapeRect)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_CIRCLE, &XL_CAD_View::OnUpdateIdrShapeCircle)
	ON_COMMAND(IDR_SHAPE_ELLIPSE, &XL_CAD_View::OnIdrShapeEllipse)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_ELLIPSE, &XL_CAD_View::OnUpdateIdrShapeEllipse)
	ON_COMMAND(IDR_SHAPE_POLYGON, &XL_CAD_View::OnIdrShapePolygon)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_POLYGON, &XL_CAD_View::OnUpdateIdrShapePolygon)
	ON_UPDATE_COMMAND_UI(IDR_SHAPE_LINE, &XL_CAD_View::OnUpdateIdrShapeLine)
END_MESSAGE_MAP()

BOOL XL_CAD_View::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void XL_CAD_View::OnPaint() 
{
	CPaintDC dc(this);
	//m_Renderer->OnPaint();
	XL_2D_OnPaint();

	auto frame = (XL_CAD_Frame*)theApp.m_pMainWnd;
	CString strFps;
	strFps.Format(L"FPS: %lld", XL_2D_GetFrameRate());
	if (frame->m_wndStatusBar.m_hWnd != NULL)
		frame->m_wndStatusBar.SetPaneText(0, strFps);
}

int XL_CAD_View::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void XL_CAD_View::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	/*if (m_Renderer)
		m_Renderer->OnSize(cx,cy);*/
	XL_2D_OnSize(cx, cy);
}

void XL_CAD_View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void XL_CAD_View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

BOOL XL_CAD_View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void XL_CAD_View::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	if (!m_bMouseDown)
	{
		if (m_DrawShapeType == ShapeType::Shape_None)
		{
			XL_2D_OnMouseMove(point.x, point.y, m_DrawShapeType, false, true);
			Invalidate();
		}
		return;
	}

	CPoint offset;
	offset.x = point.x - m_StartPoint.x;
	offset.y = point.y - m_StartPoint.y;
	bool bSelected = true;

	switch (m_DrawShapeType)
	{
	case ShapeType::Shape_None:
		m_StartPoint = point;
		break;
	case ShapeType::Shape_Point:
	case ShapeType::Shape_Line:
	case ShapeType::Shape_Rectangle:
	case ShapeType::Shape_Triangle:
	case ShapeType::Shape_Circle:
	case ShapeType::Shape_Ellipse:
	case ShapeType::Shape_Polygon:
		bSelected = false;
		offset = point;
		break;
	}

	XL_2D_OnMouseMove(offset.x, offset.y, m_DrawShapeType, bSelected, false);
	Invalidate();
}

BOOL XL_CAD_View::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void XL_CAD_View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	m_bMouseDown = true;
	m_StartPoint = point;

	if (m_DrawShapeType != ShapeType::Shape_None)
	{
		switch (m_DrawShapeType)
		{
			break;
		case Shape_Point:
			break;
		case Shape_Line: {
			XL_PointF start{
				(float)m_StartPoint.x,
				(float)m_StartPoint.y
			};
			XL_PointF end{
				(float)m_StartPoint.x,
				(float)m_StartPoint.y
			};
			XL_ColorF color{ 0.0f, 0.0f, 1.0f, 1.0f };
			XL_2D_DrawLine(&start, &end, &color, 1.0f);
			break;
		}
		case Shape_Rectangle: {
			XL_RectF rect{
				(float)m_StartPoint.x,
				(float)m_StartPoint.y,
				(float)m_StartPoint.x,
				(float)m_StartPoint.y
			};

			XL_ColorF bg_color{ 0.0f, 0.0f, 1.0f, 1.0f };
			XL_2D_FillRectangle(&rect, &bg_color, 5, 6.0f);
			break;
		}
		case Shape_Triangle:
			break;
		case Shape_Circle: {
			XL_PointF center{
				(float)m_StartPoint.x,
				(float)m_StartPoint.y
			};

			XL_ColorF border_color{ 1.0f, 0.0f, 0.0f, 1.0f };
			XL_2D_FillCircle(&center, &border_color, 0.0f);
			break;
		}
		case Shape_Ellipse: {
			XL_PointF center{
					(float)m_StartPoint.x,
					(float)m_StartPoint.y
			};

			XL_ColorF border_color{ 1.0f, 0.0f, 0.0f, 1.0f };
			XL_2D_FillEllipse(&center, 0.0f, 0.0f, &border_color);
			break;
		}
		case Shape_Polygon:
			break;
		}
		Invalidate();
		return;
	}

	XL_2D_OnLButtonDown(point.x, point.y);

	auto current_shape_type = XL_2D_GetCurrentShapeType();
	switch (current_shape_type)
	{
		case ShapeType::Shape_Rectangle: {
			auto pRect = XL_2D_Current_GetRect();

			if (pRect)
			{
				auto& frame = XL::FuncHelper::GetFrame();
				if (frame.m_wndStatusBar.m_hWnd != NULL)
					frame.m_wndProperty.AddRectangleProperty(*pRect);
			}
			break;
		}
		case ShapeType::Shape_Ellipse:
		case ShapeType::Shape_Circle: {
			auto pCircle = XL_2D_Current_GetCicle();
			if (pCircle)
			{
				auto& frame = XL::FuncHelper::GetFrame();
				if (frame.m_wndStatusBar.m_hWnd != NULL)
					frame.m_wndProperty.AddCircleProperty(*pCircle);
			}
			break;
		}
		case ShapeType::Shape_Line: {
			auto line = XL_2D_Current_GetLine();
			if (line)
			{
				auto& frame = XL::FuncHelper::GetFrame();
				if (frame.m_wndStatusBar.m_hWnd != NULL)
					frame.m_wndProperty.AddLineProperty(*line);
			}
			break;
		}
		default:
			break;
	}
}

void XL_CAD_View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
	m_bMouseDown = false;
	if (m_DrawShapeType != Shape_None)
	{
		m_EndPoint = point;
		return;
	}

	XL_2D_OnLButtonUp(point.x, point.y);
}

void XL_CAD_View::OnMouseHover(UINT nFlags, CPoint point)
{
	CWnd::OnMouseHover(nFlags, point);
}

void XL_CAD_View::OnMouseLeave()
{
	CWnd::OnMouseLeave();
}

void XL_CAD_View::OnIdrShapePoint()
{
	if (m_DrawShapeType != ShapeType::Shape_Point)
		m_DrawShapeType = ShapeType::Shape_Point;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnIdrShapeLine()
{
	if (m_DrawShapeType != ShapeType::Shape_Line)
		m_DrawShapeType = ShapeType::Shape_Line;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnIdrShapeRect()
{
	if(m_DrawShapeType != ShapeType::Shape_Rectangle)
		m_DrawShapeType = ShapeType::Shape_Rectangle;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnIdrShapeTriangle()
{
	if (m_DrawShapeType != ShapeType::Shape_Triangle)
		m_DrawShapeType = ShapeType::Shape_Triangle;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnIdrShapeCircle()
{
	if (m_DrawShapeType != ShapeType::Shape_Circle)
		m_DrawShapeType = ShapeType::Shape_Circle;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnUpdateIdrShapePoint(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Point);
}

HBRUSH XL_CAD_View::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void XL_CAD_View::OnEditOpen()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_View::OnEditSave()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_View::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_View::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_View::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
}

void XL_CAD_View::OnUpdateIdrShapeTriangle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Triangle);
}

void XL_CAD_View::OnUpdateIdrShapeRect(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Rectangle);
}

void XL_CAD_View::OnUpdateIdrShapeCircle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Circle);
}

void XL_CAD_View::OnIdrShapeEllipse()
{
	if (m_DrawShapeType != ShapeType::Shape_Ellipse)
		m_DrawShapeType = ShapeType::Shape_Ellipse;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnUpdateIdrShapeEllipse(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Ellipse);
}

void XL_CAD_View::OnIdrShapePolygon()
{
	if (m_DrawShapeType != ShapeType::Shape_Polygon)
		m_DrawShapeType = ShapeType::Shape_Polygon;
	else
		m_DrawShapeType = ShapeType::Shape_None;
}

void XL_CAD_View::OnUpdateIdrShapePolygon(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Polygon);
}
void XL_CAD_View::OnUpdateIdrShapeLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_DrawShapeType == ShapeType::Shape_Line);
}
