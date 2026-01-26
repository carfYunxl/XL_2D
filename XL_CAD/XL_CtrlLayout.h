/*****************************************************************//**
 * @file   XL_CtrlLayout.h
 * @brief  XL_CtrlLayout is a simple layout class for MFC UI layout.
 *           now it's support simple interface to layout UI And Support DPI changes automatically.
 * 
 *          1 - HOW TO USE IT:
 *              Two ways here:
 *              (1) use default constructor : 
 *                      HF_CtrlLayout layout;                     // create a default HF_CtrlLayout object
 *                      layout.SetStartPoint( ... );            // set start point
 *                      layout.SetCtrlHeight( ... );            // set ctrl default height
 *                      layout.SetGap( ... )                    // set gap
 * 
 *                      layout.Add( wnd1, 100, SWP_SHOWWINDOW ); // Add wnd1 to layout, it's width = 100, show status = SWP_SHOWWINDOW
 *                      layout.Add( wnd2, 200, SWP_SHOWWINDOW ); // Add wnd2 to layout, it's width = 200, show status = SWP_SHOWWINDOW
 *                      ...                                      // other operations
 *              (2) use overloaded constructor:
 *                      HF_CtrlLayout layout(..., ..., ...);
 *                      layout.Add( wnd1, 100, SWP_SHOWWINDOW ); // Add wnd1 to layout, it's width = 100, show status = SWP_SHOWWINDOW
 *                      layout.Add( wnd2, 200, SWP_SHOWWINDOW ); // Add wnd2 to layout, it's width = 200, show status = SWP_SHOWWINDOW
 *                      layout.NextLine();                       // New Line
 *                      layout.Add( wnd3, 100, SWP_SHOWWINDOW ); // Add wnd2 to layout, it's width = 300, show status = SWP_SHOWWINDOW
 *                      ...                                      // other operations
 *         2 - SETTINGS : 
 *              you can change the default settings with below functions:
 *                  SetStartPoint();
 *                  SetCtrlHeight();
 *                  SetCurrentPos();
 *                  SetGap();
 *          see these functions for detail!!!
 * 
 * @author river_yun
 * @date   2024/04/19
 * 
 * Version v0.1
 *********************************************************************/

#ifndef __XL_CCTRL_LAYOUT_HPP__
#define __XL_CCTRL_LAYOUT_HPP__

namespace XL
{
    class CtrlLayout
    {
    public:
        CtrlLayout();
        CtrlLayout(const CPoint& start, int CtrlHeight, int Gap);

        /**
         * @brief Add a ctrl to the layout.
         *        *** the layout direction fix to horizon direction
         *                  and the ctrl will be added to the tail of current line.
         *        *** if you want a new line to layout the ctrl, call NextLine() first!!!
         * 
         * @param wnd           [out]   the ctrl you want to layout
         * @param nCtrlWidth    [in]    the width of wnd
         * @param nCmdShow      [in]    the wnd show status cmd. typically SWP_SHOWWINDOW , see SetWindowPos for detail.
         */
        void Add( CWnd& wnd, int nCtrlWidth, UINT nCmdShow );
        void Add( CWnd* wnd, int nCtrlWidth, UINT nCmdShow );
        void Add( CWnd* parent, UINT id, int nCtrlWidth, UINT nCmdShow );

        /**
         * @brief move to new line of the layout.
         */
        void NextLine();

        /**
         * @brief Set start point of the Layout.
         * 
         * @param start [in] current start point value
         */
        void SetStartPoint(const CPoint& start){ m_ptStart = start; m_ptCurPos = { 0 , 0 }; }

        /**
         * @brief Set ctrl height.
         * 
         * @param height [in] current ctrl height value
         */
        void SetCtrlHeight(int height){ m_nHeight = height; }

        /**
         * @brief Set current position.
         * 
         * @param pos [in] current position value
         */
        void SetCurrentPos(const CPoint& pos) { m_ptCurPos = pos; }

        /**
         * @brief Set current ctrl gap. Notes: horizon and vertical is the same.
         * 
         * @param gap [in] current ctrl gap.
         */
        void SetGap(int gap) { m_nGap = static_cast<int>( gap * m_DpiScale ); }

        /**
         * @brief Set indent offset.
         * 
         * @param nIndentOffset [in] set value of indent offset
         */
        void SetIndentOffset(int nIndentOffset) { m_nIndentOffset = static_cast<int>( nIndentOffset * m_DpiScale ); }

        /**
         * @brief Get indent offset.
         * 
         * @return current value of indent offset
         */
        const int GetIndentOffset() const { return m_nIndentOffset; }

        /**
         * @brief Get current position.
         * 
         * @return 
         */
        CPoint& GetCurrentPos() {  return m_ptCurPos; }

        /**
         * Get current DPI scale factor.
         * 
         * @return Current value of DPI scale factor
         */
        const float GetScale() const { return m_DpiScale; }

        /**
         * @brief enable dpi scale or not
         */
        void EnableDpiScale() { m_DpiScale = 1.0f; }
        void DisableDpiScale() { GetSystemScaleFactor(); }
    private:

        /**
         * @brief Get current system DPI scale factor.
         */
        void GetSystemScaleFactor();

    private:
        CPoint  m_ptStart;              // UI布局的起始点
        CPoint  m_ptCurPos;             // 当前位置（以起始点为基准的相对位置）
        int     m_nHeight;              // 控件的高度
        int     m_nGap;                 // 控件之间的间隙
        int     m_nIndentOffset;        // 缩进量，相对于起始点 x 位置的偏移量，如果设置该值，新行都会偏移该值
        float   m_DpiScale;             // DPI缩放比例
    };
}

#endif //__HFST_CCTRL_LAYOUT_HPP__
