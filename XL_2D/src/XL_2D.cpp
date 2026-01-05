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

void XL_2D_FillRectangle(const XL_RectF* rect, const XL_ColorF* bg_color, float tess_level)
{
	if (!rect)
		return;

	if (!bg_color)
		return;

	if (!g_sHandle)
		return;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	renderer->FillRectangle(*rect, *bg_color, tess_level);
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

int XL_2D_GetFrameRate()
{
	if (!g_sHandle)
		return -1;

	auto* renderer = static_cast<OpenglRender*>(g_sHandle);
	auto frameTime = renderer->GetFrameTime();
	if (frameTime != 0)
	{
		int nFps = 1000000 / frameTime;

		return nFps;
	}
	
	return 1000;
}