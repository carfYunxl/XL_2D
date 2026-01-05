#include "pch.h"
#include "XL_2D_Demo.h"
#include "afxdialogex.h"
#include "ShapeGenerator.h"
#include "XL_2D.hpp"

IMPLEMENT_DYNAMIC(ShapeGenerator, CDialogEx)

ShapeGenerator::ShapeGenerator(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SHAPE_GENERATOR, pParent)
{

}

ShapeGenerator::~ShapeGenerator()
{
}

void ShapeGenerator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ShapeGenerator, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GERERATOR, &ShapeGenerator::OnBnClickedButtonGererator)
END_MESSAGE_MAP()

void ShapeGenerator::OnBnClickedButtonGererator()
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
		(float)nMesh
	);
	XL_2D_OnPaint();
}
