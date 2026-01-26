#include "XL_2D.hpp"
#include "XL_OpenglRender.hpp"

static void* g_sHandle = nullptr;

int XL_2D_CreateRenderer(HWND hWnd, uint32_t width, uint32_t height)
{
	if (g_sHandle)
		return 0;

	auto* renderer = new OpenglRender(hWnd, width, height);
	if (!renderer->Init())
	{
		delete renderer;
		return XL_2D_RENDERER_CREATE_FAIL;
	}
	g_sHandle = renderer;
	return 0;
}

void XL_2D_ReleaseRenderer()
{
	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	delete renderer;
}

void XL_2D_OnPaint()
{
	if(!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->OnPaint();
}

void XL_2D_OnSize(int width, int height)
{
	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->OnSize(width, height);
}

void XL_2D_FillRectangle(const XL_RectF* rect, const XL_ColorF* bg_color, float tess_level, float border_width)
{
	if (!rect)
		return;

	if (!bg_color)
		return;

	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->FillRectangle(*rect, *bg_color, tess_level, border_width, border_width);
}

void XL_2D_OnLButtonDown(int x, int y)
{
	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->OnLButtonDown(x, y);
}

void XL_2D_OnLButtonUp(int x, int y)
{
	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->OnLButtonUp(x, y);
}

void XL_2D_OnMouseMove(int x, int y, bool bSelect, bool bHover)
{
	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->OnMouseMove(x, y, bSelect, bHover);
}

void XL_2D_OnMouseHover(int x, int y)
{
}

uint64_t XL_2D_GetFrameRate()
{
	if (!g_sHandle)
		return -1;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	auto frameTime = renderer->GetFrameTime();
	if (frameTime != 0)
	{
		uint64_t nFps = 1000000ull / frameTime;

		return nFps;
	}
	
	return 1000;
}

uint32_t XL_2D_GetSelectID()
{
	if (!g_sHandle)
		return 0;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	return renderer->GetSelectID();
}

INNER_RectF* XL_2D_GetRect(uint32_t id)
{
	if (!g_sHandle)
		return 0;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);

	return renderer->GetRect(id);
}

INNER_RectF* XL_2D_Current_GetRect()
{
	if (!g_sHandle)
		return 0;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);

	return renderer->GetCurrentRect();
}
