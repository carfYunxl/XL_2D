#ifndef __OPENGL_RENDERER_HPP__
#define __OPENGL_RENDERER_HPP__

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
    class Renderer;
    class GlFrameBuffer;
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
    std::unique_ptr<XL::Renderer>       m_Renderer;
    std::unique_ptr<XL::GlFrameBuffer>  m_FrameBuffer;
};

#endif // __OPENGL_RENDERER_HPP__
