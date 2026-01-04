#ifndef XL_2D_HPP_
#define XL_2D_HPP_

#define XL_2D_ERROR_NONE				 0
#define XL_2D_RENDERER_NOT_CREATE		-1
#define XL_2D_RENDERER_CREATE_FAIL		-2

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _cplusplus
extern "C" {
#endif

	int XL_2D_CreateRenderer(HWND hWnd, uint32_t width, uint32_t height);

	void XL_2D_ReleaseRenderer();

	void XL_2D_OnPaint();

	void XL_2D_OnSize(int width, int height);

#ifdef _cplusplus
}
#endif

#endif //XL_2D_HPP_