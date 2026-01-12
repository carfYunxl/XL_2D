#pragma once
#include "afxdialogex.h"

class ShapeGenerator : public CDialogEx
{
	DECLARE_DYNAMIC(ShapeGenerator)

public:
	ShapeGenerator(CWnd* pParent = nullptr);
	virtual ~ShapeGenerator();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SHAPE_GENERATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonGererator();
public:
	virtual BOOL OnInitDialog();
};
