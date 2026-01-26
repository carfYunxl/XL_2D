#pragma once

class XL_CAD_View : public CWnd
{
public:
	XL_CAD_View();
	virtual ~XL_CAD_View();
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

	bool	m_bStartDrawPoint{ false };
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnIdrShapePoint();
	afx_msg void OnIdrShapeLine();
	afx_msg void OnIdrShapeRect();
	afx_msg void OnIdrShapeTriangle();
	afx_msg void OnIdrShapeCircle();
	afx_msg void OnUpdateIdrShapePoint(CCmdUI* pCmdUI);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEditOpen();
	afx_msg void OnEditSave();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateIdrShapeTriangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIdrShapeRect(CCmdUI* pCmdUI);
};

