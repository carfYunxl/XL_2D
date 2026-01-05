#include "pch.h"
#include "XL_CtrlLayout.h"

namespace XL
{
    CtrlLayout::CtrlLayout()
        : m_ptStart(0)
        , m_ptCurPos(0)
        , m_nHeight{ 0 }
        , m_nGap{ 0 }
        , m_nIndentOffset{ 0 }
        , m_DpiScale{ 1.0f }
    {
        GetSystemScaleFactor();
    }

    CtrlLayout::CtrlLayout(const CPoint& start, int CtrlHeight, int Gap)
        : m_ptStart(start)
        , m_nHeight(CtrlHeight)
        , m_nGap(Gap)
        , m_ptCurPos(0)
        , m_nIndentOffset{ 0 }
        , m_DpiScale{ 1.0f }
    {
        GetSystemScaleFactor();
    }

    void CtrlLayout::Add( CWnd& wnd, int nCtrlWidth, UINT nCmdShow )
    {
        int nWidth = static_cast<int>(nCtrlWidth * m_DpiScale);
        int nHeight = static_cast<int>(m_nHeight * m_DpiScale);

        wnd.SetWindowPos(nullptr, m_ptStart.x + m_nIndentOffset + m_ptCurPos.x, m_ptStart.y + m_ptCurPos.y, nWidth, nHeight, nCmdShow);

        m_ptCurPos.x += (m_nGap + nWidth);
    }

    void CtrlLayout::Add(CWnd* wnd, int nCtrlWidth, UINT nCmdShow)
    {
        ASSERT(wnd);

        Add( *wnd, nCtrlWidth, nCmdShow );
    }

    void CtrlLayout::Add( CWnd* parent, UINT id, int nCtrlWidth, UINT nCmdShow)
    {
        ASSERT(parent);

        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);

        Add( *pWnd, nCtrlWidth, nCmdShow );
    }

    void CtrlLayout::NextLine()
    {
        int nHeight = static_cast<int>(m_nHeight * m_DpiScale);

        m_ptCurPos.x = 0;
        m_ptCurPos.y += (m_nGap + nHeight);
    }

    void CtrlLayout::GetSystemScaleFactor()
    {
        HDC		hdc;

        hdc = ::GetDC(NULL);

        UINT	dpiX = 96;

        dpiX = GetDeviceCaps(hdc, LOGPIXELSX);

        m_DpiScale = (float)dpiX / 96.0f;
        if (m_DpiScale < 1.0)
            m_DpiScale = 1.0;

        ::ReleaseDC(NULL, hdc);
    }
}

