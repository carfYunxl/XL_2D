#ifndef XL_OPENGL_RENDERER_HPP_
#define XL_OPENGL_RENDERER_HPP_

#include <cstdint>
#include <wtypes.h>
#include <memory>

struct WindowProp
{
    uint32_t Width;
    uint32_t Height;
};

namespace XL
{
    class BatchRenderer;
    class FrameBuffer;
}

class OpenglRender
{
public:
    OpenglRender(HWND hWnd, const WindowProp& prop);
    ~OpenglRender();

    void OnSize(int cx, int cy);
    bool Init();
    void OnPaint();
	void OnKeyDown(UINT nChar, UINT nRepCnt);
    void OnMouseWheel(int yOffset);
    void OnMouseMove(int xOffset,int yOffset);
private:
    bool SetupPixelFormat(HDC hdc);
private:
    HWND                                m_hWnd;
    HDC                                 m_hDC;
    HGLRC                               m_hRC;
    WindowProp                          m_WinProp;
    std::unique_ptr<XL::BatchRenderer>   m_Renderer;
    std::unique_ptr<XL::FrameBuffer>    m_FrameBuffer;
};

#endif // XL_OPENGL_RENDERER_HPP_
