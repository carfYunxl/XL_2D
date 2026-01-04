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