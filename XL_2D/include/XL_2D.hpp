#ifndef XL_2D_HPP_
#define XL_2D_HPP_

#define XL_2D_ERROR_NONE				 0
#define XL_2D_RENDERER_NOT_CREATE		-1
#define XL_2D_RENDERER_CREATE_FAIL		-2

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#endif

enum ShapeType : uint8_t
{
    Shape_None = 0,
    Shape_Point,
    Shape_Line,
    Shape_Rectangle,
    Shape_Triangle,
    Shape_Circle,
	Shape_Ellipse,
	Shape_Polygon
};

enum PolygonMode : uint8_t
{
    PolygonMode_Fill = 0,
    PolygonMode_Line
};

typedef struct _XL_RectF
{
    float l{0.0f};
    float t{0.0f};
    float r{0.0f};
    float b{0.0f};
}XL_RectF;

typedef struct _XL_ColorF
{
    float r{0.0f};
    float g{0.0f};
    float b{0.0f};
    float a{1.0f};
}XL_ColorF;

typedef struct _XL_PointF
{
    float x{0.0f};
    float y{0.0f};
}XL_PointF;

typedef struct _XL_TriangleF
{
    XL_PointF p0;
    XL_PointF p1;
    XL_PointF p2;
}XL_TriangleF;

typedef struct _INNER_RectF
{
    uint32_t    id{ 0 };
    float       z_near{ 0.0f };
    float       thickness_x;
    float       thickness_y;
    XL_RectF    rect;
    XL_ColorF   background_color;
    int         tess_level;
    XL_ColorF   selected_color{ 1.0,1.0,1.0,1.0 };
    bool        b_selected{ false };
    bool        b_clicked{ false };
    int         select_cell_x{ -1 };
    int         select_cell_y{ -1 };
}INNER_RectF;

typedef struct _Inner_CircleF
{
    uint32_t    u_id{ 0 };
    float       f_z_near{ 0.0f };
    //float       f_thickness{ 0.0f };
    //float       f_fade{ 0.0f };
    XL_PointF   pt_center{ 0.0f, 0.0f };
    float       f_radiusX{ 0.0f };
    float       f_radiusY{ 0.0f };
    XL_ColorF   c_border_color{ 0.0f, 0.0f, 0.0f, 0.0f };
	float       f_border_width{ 0.0f };
    bool        b_selected{ false };
    bool        b_clicked{ false };
}INNER_CircleF;

#ifdef _cplusplus
extern "C" {
#endif

	// Renderer Management
	int XL_2D_CreateRenderer(HWND hWnd, uint32_t width, uint32_t height);
	void XL_2D_ReleaseRenderer();

	void XL_2D_SetPolygonMode(PolygonMode mode);

    // Draw Calls
	void XL_2D_FillRectangle(const XL_RectF* rect, const XL_ColorF* bg_color, float tess_level, float border_width);

    void XL_2D_DrawCircle(const XL_PointF* center, const XL_ColorF* border_color, float radius, float border_width);
    void XL_2D_FillCircle(const XL_PointF* center, const XL_ColorF* border_color, float radius);

	void XL_2D_DrawEllipse(const XL_PointF* center, float pixel_radius_x, float pixel_radius_y, const XL_ColorF* border_color, float border_width);
	void XL_2D_FillEllipse(const XL_PointF* center, float pixel_radius_x, float pixel_radius_y, const XL_ColorF* border_color);
	// Event Handlers
	void XL_2D_OnPaint();
	void XL_2D_OnSize(int width, int height);
	void XL_2D_OnLButtonDown(int x, int y);
	void XL_2D_OnLButtonUp(int x, int y);
	void XL_2D_OnMouseMove(int x, int y, ShapeType shape, bool bSelect, bool bHover);
	void XL_2D_OnMouseHover(int x, int y);

    // Infomations
    uint64_t  XL_2D_GetFrameRate();
    uint32_t  XL_2D_GetSelectID();

	INNER_RectF* XL_2D_GetRect(uint32_t id);
    INNER_CircleF* XL_2D_GetCircle(uint32_t id);

	ShapeType XL_2D_GetCurrentShapeType();
	INNER_RectF* XL_2D_Current_GetRect();
	INNER_CircleF* XL_2D_Current_GetCicle();

#ifdef _cplusplus
}
#endif

#endif //XL_2D_HPP_