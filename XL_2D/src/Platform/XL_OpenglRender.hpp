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


class OpenglRender
{
public:
    OpenglRender(HWND hWnd, uint32_t Width, uint32_t Height);
    ~OpenglRender();

    void SetPolygonModeFill();
    void SetPolygonModeLine();

    void OnSize(int cx, int cy);
    bool Init();
    void OnPaint();

    void FillTriangle(const XL_TriangleF& triangle, const XL_ColorF& bg_color);
	void FillRectangle(const XL_RectF& rect, const XL_ColorF& bg_color, float tess_level, float thicknessX, float thicknessY);
	void DrawRectangle(const XL_RectF& rect, const XL_ColorF& border_color, float border_width);
	void FillEllipse(const XL_PointF& center, float pixel_radius_x, float pixel_radius_y, const XL_ColorF& border_color);
	void DrawEllipse(const XL_PointF& center, float pixel_radius_x, float pixel_radius_y, const XL_ColorF& border_color, float border_width);
	void FillCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color);
	void DrawCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color, float border_width);

	void DrawLine(const XL_PointF& start, const XL_PointF& end, const XL_ColorF& color, float line_width);

	// information getters
	uint64_t        GetFrameTime() const { return m_nFrameTime; }
    uint32_t        GetSelectID() const { return m_ActiveId; }

	INNER_RectF*    GetRect(uint32_t id);
	INNER_CircleF*  GetCircle(uint32_t id);
	INNER_LineF*    GetLine(uint32_t id);

	INNER_RectF*    GetCurrentRect();
    INNER_CircleF*  GetCurrentCircle();
    INNER_LineF*    GetCurrentLine();

	ShapeType       GetCurrentShapeType();
public:
	// Event Handlers
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y, ShapeType shape, bool bSelect, bool bHover);
private:
    void UpdateRect(const XL_PointF& rb);
	void ModifyRect(const XL_PointF& offset);
    void RemoveBackRect();

    void UpdateCircle(const XL_PointF& pt_on_circle);
	void ModifyCircle(const XL_PointF& offset);

    void UpdateEllipse(const XL_PointF& pt_on_ellipse);
	void ModifyEllipse(const XL_PointF& offset);

	void UpdateLine(const XL_PointF& pt_end);
	void ModifyLine(const XL_PointF& offset);

    bool        SetupPixelFormat(HDC hdc);
	XL_PointF   ScreenToWorld(const XL_PointF& screenPos);
    glm::vec4   ToColorF(const XL_ColorF& color) { return glm::vec4{color.r, color.g, color.b, color.a}; }
    bool        PtInRect(const XL_PointF& pt, const XL_RectF& rect) { return pt.x >= rect.l && pt.x <= rect.r && pt.y >= rect.t && pt.y <= rect.b; }
    bool        PtInCircle(const XL_PointF& pt, const XL_PointF& center, float radius);
    bool        PtInEllipse(const XL_PointF& pt, const XL_PointF& center, float radius_x, float radius_y);
    bool        PtInLine(const XL_PointF& start, const XL_PointF& end, const XL_PointF& pt);
private:
    HWND                                m_hWnd;
    HDC                                 m_hDC;
    HGLRC                               m_hRC;
	uint32_t                            m_WindowWidth;
	uint32_t							m_WindowHeight;
    std::unique_ptr<XL::BatchRenderer>  m_Renderer;
    std::unique_ptr<XL::FrameBuffer>    m_FrameBuffer;

    std::vector<INNER_RectF>            m_InnerRects;
    std::vector<INNER_CircleF>          m_InnerCircles;
	std::vector<INNER_LineF>            m_InnerLines;
	unsigned char                       m_nXORKey{ 0xFF };
    uint64_t				            m_nFrameTime{ 0 }; //us
    float                               m_fZnear{0.0f};
    uint32_t                            m_id{ 0 };
    uint32_t                            m_ActiveId{ 0xFFFF };
    float                               m_QuadBorderWidth_X{ 0.0f };
    float                               m_QuadBorderWidth_Y{ 0.0f };

	ShapeType                           m_emCurrentShape{ Shape_None };
};

#endif // XL_OPENGL_RENDERER_HPP_
