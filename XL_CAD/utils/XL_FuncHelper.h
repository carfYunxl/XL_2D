#ifndef FUNC_HELPER_H_
#define FUNC_HELPER_H_

#include "XL_CAD_Frame.h"
#include "XL_CAD_App.h"

namespace XL
{
	class FuncHelper
	{
	public:
		static XL_CAD_Frame& GetFrame()
		{
			return *(XL_CAD_Frame*)(theApp.m_pMainWnd);
		}

		static XL_CAD_View& GetView()
		{
			return GetFrame().m_wndView;
		}

	};
}

#endif // FUNC_HELPER_H_