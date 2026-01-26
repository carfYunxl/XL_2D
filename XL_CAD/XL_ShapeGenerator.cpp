#include "XL_PCH.h"
#include "XL_CAD_App.h"
#include "XL_ShapeGenerator.h"

IMPLEMENT_DYNAMIC(XL_ShapeGenerator, CDialogEx)

XL_ShapeGenerator::XL_ShapeGenerator(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SHAPE_GENERATOR, pParent)
{

}

XL_ShapeGenerator::~XL_ShapeGenerator()
{
}

void XL_ShapeGenerator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(XL_ShapeGenerator, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GERERATOR, &XL_ShapeGenerator::OnBnClickedButtonGererator)
END_MESSAGE_MAP()

void XL_ShapeGenerator::OnBnClickedButtonGererator()
{
	int L = GetDlgItemInt(IDC_EDIT_L);
	int T = GetDlgItemInt(IDC_EDIT_T);
	int R = GetDlgItemInt(IDC_EDIT_R);
	int B = GetDlgItemInt(IDC_EDIT_B);

	int CR = GetDlgItemInt(IDC_EDIT_COLOR_R);
	int CG = GetDlgItemInt(IDC_EDIT_COLOR_G);
	int CB = GetDlgItemInt(IDC_EDIT_COLOR_B);
	int CA = GetDlgItemInt(IDC_EDIT_COLOR_A);

	int nMesh = GetDlgItemInt(IDC_EDIT_MESH);

	XL_RectF rec{ (float)L, (float)T, (float)R, (float)B };
	XL_ColorF color{ CR / 255.0f, CG / 255.0f, CB / 255.0f, CA / 255.0f };
	XL_2D_FillRectangle(
		&rec,
		&color,
		(float)nMesh,
		10.0f
	);
	XL_2D_OnPaint();
}

BOOL XL_ShapeGenerator::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemInt(IDC_EDIT_L, 100);
	SetDlgItemInt(IDC_EDIT_T, 100);
	SetDlgItemInt(IDC_EDIT_R, 300);
	SetDlgItemInt(IDC_EDIT_B, 300);

	SetDlgItemInt(IDC_EDIT_COLOR_R, 255);
	SetDlgItemInt(IDC_EDIT_COLOR_G, 0);
	SetDlgItemInt(IDC_EDIT_COLOR_B, 0);
	SetDlgItemInt(IDC_EDIT_COLOR_A, 255);

	SetDlgItemInt(IDC_EDIT_MESH, 1);

	return TRUE;
}
