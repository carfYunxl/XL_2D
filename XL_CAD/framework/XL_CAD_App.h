#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"

class XL_CAD_App : public CWinApp
{
public:
	XL_CAD_App() noexcept;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern XL_CAD_App theApp;
