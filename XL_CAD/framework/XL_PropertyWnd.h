#ifndef XL_PROPERTIES_WND_H__
#define XL_PROPERTIES_WND_H__

#include "afxdockablepane.h"
#include "XL_CAD_Def.h"
#include "XL_2D.hpp"
#include <utility>
class XL_PropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class XL_PropertiesWnd : public CDockablePane
{
public:
	XL_PropertiesWnd() noexcept;
	virtual ~XL_PropertiesWnd();
private:
	void AdjustLayout();
	void SetVSDotNetLook(BOOL bSet) {
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
	void InitPropList();
	void SetPropListFont();
	//std::pair<WND_TYPE, int> GetWndTypeAndSubTypeByName(const CString& name);
	void SetPropertyValue(int id, COleVariant value);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnWmPropertyChanged(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

public:
	CMFCPropertyGridCtrl& GetPropertyCtrl() { return m_wndPropList; }

	void AddComponentsProperty(XL::SHAPE_TYPE type);

	// GRAPH
	void AddPointProperty();		// 随机点
	void AddLineProperty(const INNER_LineF& InnerLine);			// 直线
	void AddRectangleProperty(const INNER_RectF& InnerRect);	// 矩形
	void AddCircleProperty(const INNER_CircleF& InnerCircle);		// 圆形

	void UpdateValueByData(DWORD id, float value);
	void UpdateValueByData(DWORD id, DWORD value);
	void UpdateValueByData(DWORD id, int value);
	void UpdateValueByData(DWORD id, size_t value);
	void UpdateValueByData(DWORD id, const CString& value);
public:
	CMFCPropertyGridCtrl	m_wndPropList;
private:
	CFont					m_fntPropList;
	XL_PropertiesToolBar	m_wndToolBar;

	uint32_t				m_nCurrID{ 0 };
	ShapeType				m_nCurrShapeType{ ShapeType::Shape_None };
};

#endif //XL_PROPERTIES_WND_H__