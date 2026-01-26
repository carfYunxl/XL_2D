#ifndef XL_SHAPE_GENERATOR_H__
#define XL_SHAPE_GENERATOR_H__

class XL_ShapeGenerator : public CDialogEx
{
	DECLARE_DYNAMIC(XL_ShapeGenerator)

public:
	XL_ShapeGenerator(CWnd* pParent = nullptr);
	virtual ~XL_ShapeGenerator();
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
#endif //XL_SHAPE_GENERATOR_H__
