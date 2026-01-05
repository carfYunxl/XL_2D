#ifndef XL_OPENGL_RENDERER_HPP_
#define XL_OPENGL_RENDERER_HPP_

#include <cstdint>
#include <wtypes.h>
#include <memory>
#include "glm/glm.hpp"
#include "XL_2D.hpp"

namespace XL
{
    class BatchRenderer;
    class FrameBuffer;
}

struct INNER_RectF
{
    int         z_order{-1};
	XL_RectF    rect;
    XL_ColorF   background_color;
	float       tess_level;
    XL_ColorF   selected_color{ 1.0,1.0,1.0,1.0 };
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
    void FillTriangle(const XL_TriangleF& triangle, const XL_ColorF& bg_color);

	void FillEllipse(const XL_PointF& center, float pixelX, float pixelY, const XL_ColorF& fill_color);
	void DrawEllipse(const XL_PointF& center, float pixelX, float pixelY, const XL_ColorF& border_color);

	void FillCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& fill_color);
	void DrawCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color, float border_width);

	int  GetFrameTime() const { return m_nFrameTime; }
public:
	// Event Handlers
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
private:
    bool        SetupPixelFormat(HDC hdc);
	XL_PointF   ScreenToWorld(const XL_PointF& screenPos);
    glm::vec4   ToColorF(const XL_ColorF& color) { return glm::vec4{color.r, color.g, color.b, color.a}; }
    bool        PtInRect(const XL_PointF& pt, const XL_RectF& rect) {
        return pt.x >= rect.l && pt.x <= rect.r && pt.y >= rect.t && pt.y <= rect.b;
	}
private:
    HWND                                m_hWnd;
    HDC                                 m_hDC;
    HGLRC                               m_hRC;
	uint32_t                            m_WindowWidth;
	uint32_t							m_WindowHeight;
    std::unique_ptr<XL::BatchRenderer>  m_Renderer;
    std::unique_ptr<XL::FrameBuffer>    m_FrameBuffer;

    std::vector<INNER_RectF>            m_InnerRects;
    int                                 m_ZOrderCounter{-1};
	unsigned char                       m_nXORKey{ 0xFF };
    unsigned long long				    m_nFrameTime{ 0 }; //us
};

#endif // XL_OPENGL_RENDERER_HPP_
