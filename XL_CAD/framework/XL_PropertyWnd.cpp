#include "XL_PCH.h"
#include "XL_PropertyWnd.h"
#include "XL_CAD_Frame.h"
#include "XL_CAD_View.h"
#include "XL_CAD_Def.h"
#include "XL_FuncHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const CString NAME_SHAPE_PROPERTY = L"Shape Property";

XL_PropertiesWnd::XL_PropertiesWnd() noexcept {
}

XL_PropertiesWnd::~XL_PropertiesWnd()
{
	::DeleteObject(m_fntPropList.Detach());
}

BEGIN_MESSAGE_MAP(XL_PropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &XL_PropertiesWnd::OnWmPropertyChanged)
END_MESSAGE_MAP()

void XL_PropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	RecalcLayout();
}

int XL_PropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MAINFRAME);
	m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, TRUE /* 已锁定*/);
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void XL_PropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void XL_PropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void XL_PropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void XL_PropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void XL_PropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void XL_PropertiesWnd::OnProperties1()
{
	// TODO: 在此处添加命令处理程序代码
}

void XL_PropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void XL_PropertiesWnd::OnProperties2()
{
	// TODO: 在此处添加命令处理程序代码
}

void XL_PropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 在此处添加命令更新 UI 处理程序代码
}

void XL_PropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.SetDescriptionRows(1);

	CMFCPropertyGridProperty* pGroupShape = new CMFCPropertyGridProperty(NAME_SHAPE_PROPERTY, static_cast<DWORD_PTR>(XL::PROPERTY_ID::SHAPE));
	m_wndPropList.AddProperty(pGroupShape);

	m_wndPropList.SetCustomColors(-1, -1, -1, RGB(0, 0, 0), -1, -1, RGB(0, 0, 0));
}

void XL_PropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void XL_PropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void XL_PropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	memset(&info, 0, sizeof(info));
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	//m_wndObjectCombo.SetFont(&m_fntPropList);
}

void XL_PropertiesWnd::SetPropertyValue(int id, COleVariant value)
{
	switch (m_nCurrShapeType)
	{
	case Shape_Point:
		break;
	case Shape_Line: {
		auto pLine = XL_2D_GetLine(m_nCurrID);
		auto type = static_cast<XL::PROPERTY_ID>(id);
		switch (type)
		{
		case XL::PROPERTY_ID::LINE_ID:
			pLine->u_id = value.intVal;
			break;
		case XL::PROPERTY_ID::LINE_Z_ORDER:
			pLine->f_z_near = value.fltVal;
			break;
		case XL::PROPERTY_ID::LINE_START_X:
			pLine->pt_start.x= value.intVal;
			break;
		case XL::PROPERTY_ID::LINE_START_Y:
			pLine->pt_start.y = value.intVal;
			break;
		case XL::PROPERTY_ID::LINE_END_X:
			pLine->pt_end.x = value.intVal;
			break;
		case XL::PROPERTY_ID::LINE_END_Y:
			pLine->pt_end.y = value.intVal;
			break;
		case XL::PROPERTY_ID::LINE_WIDTH:
			pLine->f_line_width = value.intVal;
			break;
		case XL::PROPERTY_ID::RECT_BG_COLOR:
			pLine->c_color.r = float((value.lVal) & 0xFF) / (float)255;
			pLine->c_color.g = float((value.lVal >> 8) & 0x00FF) / (float)255;
			pLine->c_color.b = float((value.lVal >> 16) & 0x0000FF) / (float)255;
			pLine->c_color.a = 1.0f;
			break;
		default:
			break;
		}
		break;
	}
	case Shape_Rectangle: {
		auto pRect = XL_2D_GetRect(m_nCurrID);

		auto type = static_cast<XL::PROPERTY_ID>(id);
		switch (type)
		{
		case XL::PROPERTY_ID::RECT_Z_ORDER:
			pRect->z_near = value.fltVal;
			break;
		case XL::PROPERTY_ID::RECT_LEFT:
			pRect->rect.l = value.fltVal;
			break;
		case XL::PROPERTY_ID::RECT_TOP:
			pRect->rect.t = value.fltVal;
			break;
		case XL::PROPERTY_ID::RECT_RIGHT:
			pRect->rect.r = value.fltVal;
			break;
		case XL::PROPERTY_ID::RECT_BOTTOM:
			pRect->rect.b = value.fltVal;
			break;
		case XL::PROPERTY_ID::RECT_BG_COLOR:
			pRect->background_color.r = float((value.lVal) & 0xFF) / (float)255;
			pRect->background_color.g = float((value.lVal >> 8) & 0x00FF) / (float)255;
			pRect->background_color.b = float((value.lVal >> 16) & 0x0000FF) / (float)255;
			pRect->background_color.a = 1.0f;
			break;
		case XL::PROPERTY_ID::RECT_TESS_LEVEL:
			pRect->tess_level = value.lVal;
			break;
		case XL::PROPERTY_ID::RECT_BORDER_WIDTH_X:
			pRect->thickness_x = value.fltVal;
			break;
		case XL::PROPERTY_ID::RECT_BORDER_WIDTH_Y:
			pRect->thickness_y = value.fltVal;
			break;
		default:
			break;
		}
		break;
	}
	case Shape_Triangle:
		break;
	case Shape_Circle: {
		auto pCircle = XL_2D_GetCircle(m_nCurrID);
		auto type = static_cast<XL::PROPERTY_ID>(id);
		switch (type)
		{
		case XL::PROPERTY_ID::CIRCLE_Z_ORDER:
			pCircle->f_z_near = value.fltVal;
			break;
		case XL::PROPERTY_ID::CIRCLE_RADIUS_X:
			pCircle->f_radiusX = value.fltVal;
			break;
		case XL::PROPERTY_ID::CIRCLE_RADIUS_Y:
			pCircle->f_radiusY = value.fltVal;
			break;
		case XL::PROPERTY_ID::CIRCLE_CENTER_X:
			pCircle->pt_center.x = value.fltVal;
			break;
		case XL::PROPERTY_ID::CIRCLE_CENTER_Y:
			pCircle->pt_center.y = value.fltVal;
			break;
		case XL::PROPERTY_ID::CIRCLE_BORDER_WIDTH:
			pCircle->f_border_width = value.fltVal;
			break;
		case XL::PROPERTY_ID::CIRCLE_BORDER_COLOR:
			pCircle->c_border_color.r = float((value.lVal) & 0xFF) / (float)255;
			pCircle->c_border_color.g = float((value.lVal >> 8) & 0x00FF) / (float)255;
			pCircle->c_border_color.b = float((value.lVal >> 16) & 0x0000FF) / (float)255;
			pCircle->c_border_color.a = 1.0f;
			break;
		default:
			break;
		}
		break;
	}
	case Shape_Ellipse:
		break;
	case Shape_Polygon:
		break;
	default:
		break;
	}

	XL::FuncHelper::GetView().Invalidate();
}

void XL_PropertiesWnd::AddComponentsProperty(XL::SHAPE_TYPE type)
{
	switch (type)
	{
		case XL::SHAPE_TYPE::POINT:
		{
			AddPointProperty();
			break;
		}
		case XL::SHAPE_TYPE::LINE:
		{
			INNER_LineF line_F;
			AddLineProperty(line_F);
			break;
		}
		case XL::SHAPE_TYPE::RECTANGLE:
		{
			INNER_RectF rect_F;
			AddRectangleProperty(rect_F);
			break;
		}
		case XL::SHAPE_TYPE::CIRCLE:
		{
			INNER_CircleF circle_F;
			AddCircleProperty(circle_F);
			break;
		}
		default:
			break;
	}
}

void XL_PropertiesWnd::AddPointProperty()
{
#if 0
	auto pGroup = m_wndPropList.GetProperty(static_cast<int>(GRAPH_ID::GRAPH));
	int nCnt = pGroup->GetSubItemsCount();
	if (nCnt != 0)
	{
		for (int i = nCnt - 1; i >= 0; --i)
		{
			auto item = pGroup->GetSubItem(i);
			pGroup->RemoveSubItem(item);
		}
	}

	auto& wnd_view = ((HF_MainFrame*)(theApp.m_pMainWnd))->m_wndView;
	auto entity = wnd_view.GetEntity(WND_TYPE::GRAPH);
	if (!entity.isValid())
		return;
	if (!HFST::HasComponent<PointComponent>(entity.GetScene(), entity.GetHandleID()))
		return;

	auto& pts = HFST::GetComponent<PointComponent>(entity.GetScene(), entity.GetHandleID());
	//pts.m_TpResWidth = (float)wnd_view.GetResX();
	//pts.m_TpResHeight = (float)wnd_view.GetResY();

	auto shape = wnd_view.GetSettingDlg().Tp_GetShape();
	if (shape == TP_SHAPE::RECTANGLE || shape == TP_SHAPE::ROUND_RECT)
	{
		CMFCPropertyGridProperty* pProp1 = new CMFCPropertyGridProperty(_T("模式"), _T("13点"), _T("选择点的模式"), static_cast<int>(GRAPH_ID::RANDOM_PT_MODE));
		pProp1->AddOption(_T("13点"));
		pProp1->AddOption(_T("任意点"));
		pProp1->AddOption(_T("中心十字"));
		pProp1->AddOption(_T("对角十字"));
		pProp1->AddOption(_T("中心+对角十字"));
		pProp1->AddOption(_T("两指分离度"));
		//pProp1->AddOption(_T("中心十字+13点"));
		pProp1->AllowEdit(FALSE);
		pGroup->AddSubItem(pProp1);
	}
	else if (shape == TP_SHAPE::CIRCLE)
	{
		CMFCPropertyGridProperty* pProp1 = new CMFCPropertyGridProperty(_T("模式"), _T("9点"), _T("选择点的模式"), static_cast<int>(GRAPH_ID::RANDOM_PT_MODE));
		pProp1->AddOption(_T("9点"));
		pProp1->AddOption(_T("17点"));
		pProp1->AddOption(_T("任意点"));
		pProp1->AddOption(_T("中心十字"));
		pProp1->AddOption(_T("对角十字"));
		pProp1->AddOption(_T("中心+对角十字"));
		pProp1->AddOption(_T("两指分离度"));
		//pProp1->AddOption(_T("中心十字+13点"));
		pProp1->AllowEdit(FALSE);
		pGroup->AddSubItem(pProp1);
		CMFCPropertyGridProperty* pProp11 = new CMFCPropertyGridProperty(_T("分布范围"), _T("内接矩形"), _T("选择点的分布范围"), static_cast<int>(GRAPH_ID::RANDOM_PT_RANGE));
		pProp11->AddOption(_T("内接矩形"));
		pProp11->AddOption(_T("外接矩形"));
		pProp11->AllowEdit(FALSE);
		pProp11->Enable(FALSE);
		pGroup->AddSubItem(pProp11);

	}
	CMFCPropertyGridProperty* pProp12 = new CMFCPropertyGridProperty(_T("两指分离度点模式"), _T("L->R"), _T("从左到右"), static_cast<int>(GRAPH_ID::RANDOM_PT_TFS_MODE));
	pProp12->AddOption(_T("L->R"));
	pProp12->AddOption(_T("T->B"));
	pProp12->AddOption(_T("LT->RB"));
	pProp12->AddOption(_T("LB->RT"));
	pProp12->AllowEdit(FALSE);
	pProp12->Enable(FALSE);
	pGroup->AddSubItem(pProp12);

	CMFCPropertyGridProperty* pProp2 = new CMFCPropertyGridProperty(_T("X的数量"), (_variant_t)pts.m_CntX, _T("X 方向点的数量"), static_cast<int>(GRAPH_ID::RANDOM_PT_COUNT_X));
	pProp2->Enable(FALSE);
	pGroup->AddSubItem(pProp2);

	CMFCPropertyGridProperty* pProp3 = new CMFCPropertyGridProperty(_T("Y的数量"), (_variant_t)pts.m_CntY, _T("Y 方向点的数量"), static_cast<int>(GRAPH_ID::RANDOM_PT_COUNT_Y));
	pProp3->Enable(FALSE);
	pGroup->AddSubItem(pProp3);

	CMFCPropertyGridProperty* pProp4 = new CMFCPropertyGridProperty(_T("随机打点"), _T("否"), _T("是否启用均匀打点。若是，打点位置将均匀分布在整个 TP 空间。若否，打点位置将完全随机。"), static_cast<int>(GRAPH_ID::RANDOM_PT_RANDOM));
	pProp4->AddOption(_T("是"));
	pProp4->AddOption(_T("否"));
	pProp4->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp4);

	CMFCPropertyGridProperty* pProp6 = new CMFCPropertyGridProperty(_T("NG 复测"), _T("否"), _T("NG 后是否对 NG 点重测"), static_cast<int>(GRAPH_ID::RANDOM_PT_NG_RETEST));
	pProp6->AddOption(_T("是"));
	pProp6->AddOption(_T("否"));
	pProp6->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp6);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(
		_T("图形颜色"),
		RGB(pts.m_Color.r * 255, pts.m_Color.g * 255, pts.m_Color.b * 255),
		nullptr,
		_T("指定默认的图形颜色"),
		static_cast<int>(GRAPH_ID::RANDOM_PT_COLOR)
	);
	pGroup->AddSubItem(pColorProp);

	auto p = new CMFCPropertyGridProperty(_T("半径"), (_variant_t)pts.m_Radius, _T("每个点绘制的大小"), static_cast<int>(GRAPH_ID::RANDOM_PT_RADIUS));
	//p->EnableSpinControl();
	pGroup->AddSubItem(p);

	p = new CMFCPropertyGridProperty(_T("打点次数"), (_variant_t)pts.m_TouchCount, _T("同一个点需要打几次"), static_cast<int>(GRAPH_ID::RANDOM_PT_TOUCH_COUNT));
	p->EnableSpinControl(TRUE);
	pGroup->AddSubItem(p);

	p = new CMFCPropertyGridProperty(_T("当前点序号"), (_variant_t)pts.m_nCurPtIndex, _T("当前点序号"), static_cast<int>(GRAPH_ID::RANDOM_PT_CUR_INDEX));
	if (pts.m_PtArray.empty())
		p->EnableSpinControl(TRUE, 0, 0);
	else
		p->EnableSpinControl(TRUE, 0, pts.m_PtArray.size() - 1u);
	pGroup->AddSubItem(p);

	p = new CMFCPropertyGridProperty(_T("当前点X坐标(pixel)"), (_variant_t)pts.m_PtArray[pts.m_nCurPtIndex].PF.x, _T("当前点X坐标(pixel)"), static_cast<int>(GRAPH_ID::RANDOM_PT_CUR_X));
	pGroup->AddSubItem(p);

	p = new CMFCPropertyGridProperty(_T("当前点Y坐标(pixel)"), (_variant_t)pts.m_PtArray[pts.m_nCurPtIndex].PF.y, _T("当前点X坐标(pixel)"), static_cast<int>(GRAPH_ID::RANDOM_PT_CUR_Y));
	pGroup->AddSubItem(p);

	pGroup->SetName(NAME_POINT);
	m_wndPropList.ExpandAll(FALSE);
	m_wndPropList.ExpandAll(TRUE);
#endif
}

void XL_PropertiesWnd::AddLineProperty(const INNER_LineF& InnerLine)
{
	m_nCurrID = InnerLine.u_id;
	m_nCurrShapeType = Shape_Line;

	auto cnt = m_wndPropList.GetPropertyCount();
	auto pGroup = m_wndPropList.GetProperty(static_cast<int>(XL::PROPERTY_ID::SHAPE));
	int nCnt = pGroup->GetSubItemsCount();
	if (nCnt != 0)
	{
		for (int i = nCnt - 1; i >= 0; --i)
		{
			auto item = pGroup->GetSubItem(i);
			pGroup->RemoveSubItem(item);
		}
	}

	auto pID = new CMFCPropertyGridProperty(_T("ID"), (_variant_t)InnerLine.u_id, _T("ID"), static_cast<int>(XL::PROPERTY_ID::LINE_ID));
	pID->Enable(FALSE);
	pGroup->AddSubItem(pID);

	auto pZOrder = new CMFCPropertyGridProperty(_T("Z_Order"), (_variant_t)InnerLine.f_z_near, _T("Z_Ooder"), static_cast<int>(XL::PROPERTY_ID::LINE_Z_ORDER));
	pGroup->AddSubItem(pZOrder);

	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Left"), (_variant_t)InnerLine.pt_start.x, _T("X"), static_cast<int>(XL::PROPERTY_ID::LINE_START_X)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Top"), (_variant_t)InnerLine.pt_start.y, _T("Y"), static_cast<int>(XL::PROPERTY_ID::LINE_START_Y)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Right"), (_variant_t)InnerLine.pt_end.x, _T("X"), static_cast<int>(XL::PROPERTY_ID::LINE_END_X)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)InnerLine.pt_end.y, _T("Y"), static_cast<int>(XL::PROPERTY_ID::LINE_END_Y)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("LineWidth"), (_variant_t)InnerLine.f_line_width, _T("Line Width"), static_cast<int>(XL::PROPERTY_ID::LINE_WIDTH)));

	CMFCPropertyGridColorProperty* pBgColorProp = new CMFCPropertyGridColorProperty(
		_T("Line Color"),
		RGB(InnerLine.c_color.r * 255, InnerLine.c_color.g * 255, InnerLine.c_color.b * 255),
		nullptr,
		_T("Line Color"),
		static_cast<int>(XL::PROPERTY_ID::LINE_COLOR)
	);
	pBgColorProp->EnableOtherButton(_T("其他..."));
	pBgColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pBgColorProp);

	pGroup->SetName(NAME_SHAPE_PROPERTY);
	m_wndPropList.ExpandAll(FALSE);
	m_wndPropList.ExpandAll(TRUE);
}

void XL_PropertiesWnd::AddRectangleProperty(const INNER_RectF& InnerRect)
{
	m_nCurrID = InnerRect.id;
	m_nCurrShapeType = Shape_Rectangle;

	auto cnt = m_wndPropList.GetPropertyCount();
	auto pGroup = m_wndPropList.GetProperty(static_cast<int>(XL::PROPERTY_ID::SHAPE));
	int nCnt = pGroup->GetSubItemsCount();
	if (nCnt != 0)
	{
		for (int i = nCnt - 1; i >= 0; --i)
		{
			auto item = pGroup->GetSubItem(i);
			pGroup->RemoveSubItem(item);
		}
	}

	auto pID = new CMFCPropertyGridProperty(_T("ID"), (_variant_t)InnerRect.id, _T("ID"), static_cast<int>(XL::PROPERTY_ID::RECT_ID));
	pID->Enable(FALSE);
	pGroup->AddSubItem(pID);

	auto pZOrder = new CMFCPropertyGridProperty(_T("Z_Order"), (_variant_t)InnerRect.z_near, _T("Z_Ooder"), static_cast<int>(XL::PROPERTY_ID::RECT_Z_ORDER));
	pGroup->AddSubItem(pZOrder);

	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Left"), (_variant_t)InnerRect.rect.l, _T("X"), static_cast<int>(XL::PROPERTY_ID::RECT_LEFT)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Top"), (_variant_t)InnerRect.rect.t, _T("Y"), static_cast<int>(XL::PROPERTY_ID::RECT_TOP)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Right"), (_variant_t)InnerRect.rect.r, _T("X"), static_cast<int>(XL::PROPERTY_ID::RECT_RIGHT)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)InnerRect.rect.b, _T("Y"), static_cast<int>(XL::PROPERTY_ID::RECT_BOTTOM)));

	pGroup->AddSubItem(
		new CMFCPropertyGridProperty(
			_T("BorderWidth X"),
			(_variant_t)InnerRect.thickness_x,
			_T("Border Width X"),
			static_cast<int>(XL::PROPERTY_ID::RECT_BORDER_WIDTH_X)
		)
	);

	pGroup->AddSubItem(
		new CMFCPropertyGridProperty(
			_T("BorderWidth "),
			(_variant_t)InnerRect.thickness_x,
			_T("Border Width Y"),
			static_cast<int>(XL::PROPERTY_ID::RECT_BORDER_WIDTH_Y)
		)
	);

	auto pTessLevel = new CMFCPropertyGridProperty(_T("Tess Level"), (_variant_t)InnerRect.tess_level, _T("tess level"), static_cast<int>(XL::PROPERTY_ID::RECT_TESS_LEVEL));
	pTessLevel->EnableSpinControl(TRUE, 0, 255);
	pGroup->AddSubItem(pTessLevel);

	CMFCPropertyGridColorProperty* pBgColorProp = new CMFCPropertyGridColorProperty(
		_T("BackGround Color"),
		RGB(InnerRect.background_color.r * 255, InnerRect.background_color.g * 255, InnerRect.background_color.b * 255),
		nullptr,
		_T("Background Color"),
		static_cast<int>(XL::PROPERTY_ID::RECT_BG_COLOR)
	);
	pBgColorProp->EnableOtherButton(_T("其他..."));
	pBgColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pBgColorProp);

	CMFCPropertyGridColorProperty* pSelectdColorProp = new CMFCPropertyGridColorProperty(
		_T("Selected Color"),
		RGB(InnerRect.selected_color.r * 255, InnerRect.selected_color.g * 255, InnerRect.selected_color.b * 255),
		nullptr,
		_T("Selected Color"),
		static_cast<int>(XL::PROPERTY_ID::RECT_BG_COLOR)
	);
	pSelectdColorProp->EnableOtherButton(_T("其他..."));
	pSelectdColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pSelectdColorProp);

	pGroup->SetName(NAME_SHAPE_PROPERTY);
	m_wndPropList.ExpandAll(FALSE);
	m_wndPropList.ExpandAll(TRUE);
}

void XL_PropertiesWnd::AddCircleProperty(const INNER_CircleF& InnerCircle)
{
	m_nCurrID = InnerCircle.u_id;
	m_nCurrShapeType = Shape_Circle;

	auto cnt = m_wndPropList.GetPropertyCount();
	auto pGroup = m_wndPropList.GetProperty(static_cast<int>(XL::PROPERTY_ID::SHAPE));
	int nCnt = pGroup->GetSubItemsCount();
	if (nCnt != 0)
	{
		for (int i = nCnt - 1; i >= 0; --i)
		{
			auto item = pGroup->GetSubItem(i);
			pGroup->RemoveSubItem(item);
		}
	}

	auto pID = new CMFCPropertyGridProperty(_T("ID"), (_variant_t)InnerCircle.u_id, _T("ID"), static_cast<int>(XL::PROPERTY_ID::CIRCLE_ID));
	pID->Enable(FALSE);
	pGroup->AddSubItem(pID);

	auto pZOrder = new CMFCPropertyGridProperty(_T("Z_Order"), (_variant_t)InnerCircle.f_z_near, _T("Z_Order"), static_cast<int>(XL::PROPERTY_ID::CIRCLE_Z_ORDER));
	pGroup->AddSubItem(pZOrder);

	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Center X"), (_variant_t)InnerCircle.pt_center.x, _T("X"), static_cast<int>(XL::PROPERTY_ID::CIRCLE_CENTER_X)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Center Y"), (_variant_t)InnerCircle.pt_center.y, _T("Y"), static_cast<int>(XL::PROPERTY_ID::CIRCLE_CENTER_Y)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Radius X"), (_variant_t)InnerCircle.f_radiusX, _T("X"), static_cast<int>(XL::PROPERTY_ID::CIRCLE_RADIUS_X)));
	pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Radius Y"), (_variant_t)InnerCircle.f_radiusY, _T("Y"), static_cast<int>(XL::PROPERTY_ID::CIRCLE_RADIUS_Y)));

	pGroup->AddSubItem(
		new CMFCPropertyGridProperty(
			_T("BorderWidth"),
			(_variant_t)InnerCircle.f_border_width,
			_T("Border Width"),
			static_cast<int>(XL::PROPERTY_ID::CIRCLE_BORDER_WIDTH)
		)
	);

	CMFCPropertyGridColorProperty* pBgColorProp = new CMFCPropertyGridColorProperty(
		_T("Border Color"),
		RGB(InnerCircle.c_border_color.r * 255, InnerCircle.c_border_color.g * 255, InnerCircle.c_border_color.b * 255),
		nullptr,
		_T("Border Color"),
		static_cast<int>(XL::PROPERTY_ID::CIRCLE_BORDER_COLOR)
	);
	pBgColorProp->EnableOtherButton(_T("其他..."));
	pBgColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pBgColorProp);

	pGroup->SetName(NAME_SHAPE_PROPERTY);
	m_wndPropList.ExpandAll(FALSE);
	m_wndPropList.ExpandAll(TRUE);
}

LRESULT XL_PropertiesWnd::OnWmPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lparam;

	COleVariant i = pProp->GetValue();
	LPVARIANT pVar = (LPVARIANT)i;
	int x;
	short y;
	double d;
	float f;
	bool status;
	CString str1;
	switch (pVar->vt)
	{
	case VT_I2:    // short
		y = pVar->iVal;
		break;
	case VT_I4:     // int
		x = pVar->lVal;
		break;
	case VT_R4:    // float
		f = pVar->fltVal;
		break;
	case VT_R8:    // double
		d = pVar->dblVal;
		break;
	case VT_INT:
		x = pVar->lVal;
		break;
	case VT_BOOL:
		status = pVar->boolVal;
		break;
	case VT_BSTR:
		str1 = pVar->bstrVal;
		break;
		// etc.
	}

	/*CString strWndTypeName = pProp->GetParent()->GetName();
	auto [WndType, nSubType] = GetWndTypeAndSubTypeByName(strWndTypeName);*/

	int pID = (int)(pProp->GetData());
	SetPropertyValue(pID, i);

	return 0;
}

void XL_PropertiesWnd::UpdateValueByData(DWORD id, float value)
{
	auto property = m_wndPropList.FindItemByData(id);
	if (property != nullptr)
	{
		property->SetValue((_variant_t)value);
	}
}

void XL_PropertiesWnd::UpdateValueByData(DWORD id, DWORD value)
{
#if 0
	auto property = m_wndPropList.FindItemByData(id);
	if (property != nullptr)
	{
		if (id == static_cast<DWORD>(GRAPH_ID::RANDOM_PT_COLOR))
		{
			auto p = dynamic_cast<CMFCPropertyGridColorProperty*>(property);
			p->SetColor((COLORREF)value);
		}
		else
		{
			property->SetValue((_variant_t)value);
		}
	}
#endif
}

void XL_PropertiesWnd::UpdateValueByData(DWORD id, int value)
{
	auto property = m_wndPropList.FindItemByData(id);
	if (property != nullptr)
	{
		property->SetValue((_variant_t)value);
	}
}

void XL_PropertiesWnd::UpdateValueByData(DWORD id, size_t value)
{
	auto property = m_wndPropList.FindItemByData(id);
	if (property != nullptr)
	{
		property->SetValue((_variant_t)value);
	}  
}

void XL_PropertiesWnd::UpdateValueByData(DWORD id, const CString& value)
{
	auto property = m_wndPropList.FindItemByData(id);
	if (property != nullptr)
	{
		property->SetValue((_variant_t)value);
	}
}
