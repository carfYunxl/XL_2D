#ifndef XL_2D_HPP_
#define XL_2D_HPP_

#define XL_2D_ERROR_NONE				 0
#define XL_2D_RENDERER_NOT_CREATE		-1
#define XL_2D_RENDERER_CREATE_FAIL		-2

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct _XL_RectF
{
    float l;
    float t;
    float r;
    float b;
}XL_RectF;

typedef struct _XL_ColorF
{
    float r;
    float g;
    float b;
    float a;
}XL_ColorF;

typedef struct _XL_PointF
{
    float x;
    float y;
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
    float       tess_level;
    XL_ColorF   selected_color{ 1.0,1.0,1.0,1.0 };
    bool        b_selected{ false };
    bool        b_clicked{ false };
    int         select_cell_x{ -1 };
    int         select_cell_y{ -1 };
}INNER_RectF;

#ifdef _cplusplus
extern "C" {
#endif

	// Renderer Management
	int XL_2D_CreateRenderer(HWND hWnd, uint32_t width, uint32_t height);
	void XL_2D_ReleaseRenderer();

    // Draw Calls
	void XL_2D_FillRectangle(const XL_RectF* rect, const XL_ColorF* bg_color, float tess_level, float border_width);

	// Event Handlers
	void XL_2D_OnPaint();
	void XL_2D_OnSize(int width, int height);
	void XL_2D_OnLButtonDown(int x, int y);
	void XL_2D_OnLButtonUp(int x, int y);
	void XL_2D_OnMouseMove(int x, int y, bool bSelect, bool bHover);
	void XL_2D_OnMouseHover(int x, int y);

    // Infomations
    uint64_t  XL_2D_GetFrameRate();
    uint32_t  XL_2D_GetSelectID();
	INNER_RectF* XL_2D_GetRect(uint32_t id);

#ifdef _cplusplus
}
#endif

#endif //XL_2D_HPP_