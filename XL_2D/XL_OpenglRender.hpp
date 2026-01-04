#ifndef XL_OPENGL_RENDERER_HPP_
#define XL_OPENGL_RENDERER_HPP_

#include <cstdint>
#include <wtypes.h>
#include <memory>
#include "glm/glm.hpp"

namespace XL
{
    class BatchRenderer;
    class FrameBuffer;
}

struct XL_RectF
{
    float l;
    float t;
    float r;
    float b;
};

struct XL_ColorF
{
    float r;
    float g;
    float b;
    float a;
};

struct XL_PointF
{
    float x;
	float y;
};

struct XL_TriangleF
{
    XL_PointF p0;
    XL_PointF p1;
    XL_PointF p2;
};

class OpenglRender
{
public:
    OpenglRender(HWND hWnd, uint32_t Width, uint32_t Height);
    ~OpenglRender();

    void OnSize(int cx, int cy);
    bool Init();
    void OnPaint();

	void FillRectangle(const XL_RectF& rect, const XL_ColorF& bg_color, float tess_level);
	void DrawRectangle(const XL_RectF& rect, const XL_ColorF& border_color, float border_width);
    void FillTriangle(const XL_TriangleF& riangle, const XL_ColorF& bg_color);

	void FillEllipse(const XL_PointF& center, float pixelX, float pixelY, const XL_ColorF& fill_color);
	void DrawEllipse(const XL_PointF& center, float pixelX, float pixelY, const XL_ColorF& border_color);

	void FillCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& fill_color);
	void DrawCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color, float border_width);
private:
    bool        SetupPixelFormat(HDC hdc);
	XL_PointF   ScreenToWorld(const XL_PointF& screenPos);
    glm::vec4   ToColorF(const XL_ColorF& color) { return glm::vec4{color.r, color.g, color.b, color.a}; }
private:
    HWND                                m_hWnd;
    HDC                                 m_hDC;
    HGLRC                               m_hRC;
	uint32_t                            m_WindowWidth;
	uint32_t							m_WindowHeight;
    std::unique_ptr<XL::BatchRenderer>  m_Renderer;
    std::unique_ptr<XL::FrameBuffer>    m_FrameBuffer;
};

#endif // XL_OPENGL_RENDERER_HPP_
