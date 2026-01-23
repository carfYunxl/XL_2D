#pragma once

//#include "XL_OpenglRender.hpp"
#include <memory>

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
private:
	bool m_bStartDraw{ false };
	bool m_bMouseDown{ false };

	CPoint m_StartPoint;
	CPoint m_EndPoint;

	float	m_RectangleDivid{ 2 };
	int		m_RectangleID{ 0 };
	float   m_tessLevel{ 2.0f };
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawFollowHeart();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonRectangle();
	afx_msg void OnUpdateButtonRectangle(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnPropertyRectangleDivide2();
	afx_msg void OnUpdatePropertyRectangleDivide2(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide3();
	afx_msg void OnUpdatePropertyRectangleDivide3(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide4();
	afx_msg void OnUpdatePropertyRectangleDivide4(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide5();
	afx_msg void OnUpdatePropertyRectangleDivide5(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide6();
	afx_msg void OnUpdatePropertyRectangleDivide6(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide8();
	afx_msg void OnUpdatePropertyRectangleDivide8(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide16();
	afx_msg void OnUpdatePropertyRectangleDivide16(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivide32();
	afx_msg void OnUpdatePropertyRectangleDivide32(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDivideUserDefine();
	afx_msg void OnUpdatePropertyRectangleDivideUserDefine(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleDelete();
	afx_msg void OnUpdatePropertyRectangleDelete(CCmdUI* pCmdUI);
	afx_msg void OnPropertyRectangleBorderWidth();
	afx_msg void OnPropertyRectangleColor();
	afx_msg void OnUpdatePropertyRectangleBorderWidth(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertyRectangleColor(CCmdUI* pCmdUI);
};

