#include "XL_OpenglRender.hpp"
#include "XL_Renderer.hpp"
#include "XL_FrameBuffer.hpp"
#include <windows.h>
#include <gl/wglext.h>
#include <gl/gl.h>
#include <iostream>

#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#endif
#ifndef WGL_CONTEXT_MINOR_VERSION_ARB
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#endif
#ifndef WGL_CONTEXT_PROFILE_MASK_ARB
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#endif
#ifndef WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

using GLAD_PROC = void(*)(void);

static GLAD_PROC glad_wgl_get_proc(void* userptr, const char* name) {
    return GLAD_PROC(wglGetProcAddress(name));
}

OpenglRender::OpenglRender(HWND hWnd, uint32_t Width, uint32_t Height)
    : m_hWnd{ hWnd }
    , m_hDC { NULL }
    , m_hRC { NULL }
    , m_WindowWidth{ Width }
    , m_WindowHeight{ Height }
{
}

OpenglRender::~OpenglRender()
{
    if (m_hRC)
    {
        wglMakeCurrent(m_hDC, m_hRC);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
        m_hRC = NULL;
    }

    if (m_hDC)
    {
        ::ReleaseDC(m_hWnd, m_hDC);
        m_hDC = NULL;
    }
}

bool OpenglRender::Init()
{
    m_hDC = ::GetDC(m_hWnd);
    if (!m_hDC)
        return false;

    if (!SetupPixelFormat(m_hDC))
        return false;

    HGLRC tempContext = wglCreateContext(m_hDC);
    if (!tempContext)
        return false;

    if (!wglMakeCurrent(m_hDC, tempContext)) {
        wglDeleteContext(tempContext);
        return false;
    }

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB)
    {
        int attribs[] = { 
            WGL_CONTEXT_MAJOR_VERSION_ARB,
            4,
            WGL_CONTEXT_MINOR_VERSION_ARB,
            5,
            WGL_CONTEXT_PROFILE_MASK_ARB,
            WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        HGLRC modernContext = wglCreateContextAttribsARB(m_hDC, 0, attribs);
        if (modernContext)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(tempContext);
            m_hRC = modernContext;
            if ( !wglMakeCurrent(m_hDC, m_hRC) ) 
            { 
                wglDeleteContext(m_hRC);
                m_hRC = NULL; return false;
            }
        }
        else
        {
            m_hRC = tempContext;
        }
    }
    else {
        m_hRC = tempContext;
    }

    m_Renderer = std::make_unique<XL::BatchRenderer>();
    if (!m_Renderer->Init(glad_wgl_get_proc))
        return false;
    m_Renderer->Resize(m_WindowWidth, m_WindowHeight);

    XL::FramebufferSpecification fbSpec;
    fbSpec.Width = m_WindowWidth;
    fbSpec.Height = m_WindowHeight;
    m_FrameBuffer = std::make_unique<XL::FrameBuffer>(fbSpec);

    return true;
}

INNER_RectF* OpenglRender::GetRect(uint32_t id)
{
    auto itr = std::find_if(m_InnerRects.begin(), m_InnerRects.end(), [=](const INNER_RectF& rect) {
        return rect.id == id;
        });

    if (itr == m_InnerRects.end())
        return nullptr;

	return &(*itr);
}

INNER_RectF* OpenglRender::GetCurrentRect()
{
    if (m_InnerRects.empty() || m_ActiveId >= m_InnerRects.size())
        return nullptr;

    return &m_InnerRects[m_ActiveId];
}

void OpenglRender::OnLButtonDown(int x, int y)
{
    std::vector<INNER_RectF*> hitRects;

    for ( auto& rect : m_InnerRects)
    {
        if (PtInRect(XL_PointF{(float)x,(float)y}, rect.rect))
        {
            hitRects.push_back(&rect);
        }
    }

    if (!hitRects.empty())
    {
        auto itr = std::min_element(hitRects.begin(), hitRects.end(), [](const INNER_RectF* a, const INNER_RectF* b) {
            return a->z_near < b->z_near;
            });

        INNER_RectF& topRect = *(*itr);
		m_ActiveId = topRect.id;
        topRect.b_clicked = true;
		topRect.background_color.r = static_cast<float>((static_cast<int>(topRect.background_color.r * 255) ^ m_nXORKey)) / 255.0f;
		std::cout << "Rect ID: " << topRect.id << " clicked." << std::endl;
		OnPaint();
	}
}

void OpenglRender::OnLButtonUp(int x, int y)
{
    auto itr = std::find_if(m_InnerRects.begin(), m_InnerRects.end(), [=](const INNER_RectF& rect) {
        return rect.id == m_ActiveId;
    });

    if(itr == m_InnerRects.end())
        return;

	auto& topRect = (*itr);
    if (!topRect.b_clicked)
        return;

    topRect.b_clicked = false;
	std::cout << "Rect ID: " << topRect.id << " released." << std::endl;
    topRect.background_color.r = static_cast<float>((static_cast<int>(topRect.background_color.r * 255) ^ m_nXORKey)) / 255.0f;
    OnPaint();
}

void OpenglRender::OnMouseMove(int x, int y, ShapeType shape, bool bSelect, bool bHover)
{
    if (bHover)
    {
        std::vector<INNER_RectF*> hitRects;

        for (auto& rect : m_InnerRects)
        {
            if (PtInRect(XL_PointF{ (float)x,(float)y }, rect.rect))
            {
                hitRects.push_back(&rect);
            }
        }

        if (!hitRects.empty())
        {
            auto itr = std::min_element(hitRects.begin(), hitRects.end(), [](const INNER_RectF* a, const INNER_RectF* b) {
                return a->z_near < b->z_near;
                });

            INNER_RectF& topRect = *(*itr);

            float xOffset = x - topRect.rect.l;
            float yOffset = y - topRect.rect.t;
            float nTess_level = topRect.tess_level;
            float nX = xOffset / ((topRect.rect.r - topRect.rect.l) / nTess_level);
            float nY = yOffset / ((topRect.rect.b - topRect.rect.t) / nTess_level);
            std::cout << "Hover Rect ID: " << topRect.id << " Cell[" << nX << "," << nY << "]" << std::endl;

			topRect.select_cell_x = (int)nX;
			topRect.select_cell_y = (int)nY;
        }
        return;
    }

    if (bSelect)
    {
        ModifyRect(XL_PointF{ (float)x,(float)y });
    }
    else
    {
        switch (shape)
        {
        case Shape_None:
            break;
        case Shape_Point:
            break;
        case Shape_Line:
            break;
        case Shape_Rectangle:
            UpdateRect(XL_PointF{ (float)x,(float)y });
            break;
        case Shape_Triangle:
            break;
        case Shape_Circle:
            UpdateCircle(XL_PointF{ (float)x,(float)y });
            break;
        }
    }
}

void OpenglRender::UpdateRect(XL_PointF rb)
{
    if (m_InnerRects.empty())
        return;
    auto& rect = m_InnerRects.back();
    rect.rect.r = rb.x;
    rect.rect.b = rb.y;

    rect.thickness_x = m_QuadBorderWidth_X/* / (rect.rect.r - rect.rect.l)*/;
    rect.thickness_y = m_QuadBorderWidth_Y/* / (rect.rect.b - rect.rect.t)*/;

}

void OpenglRender::ModifyRect(XL_PointF offset)
{
    auto itr = std::find_if(m_InnerRects.begin(), m_InnerRects.end(), [=](const INNER_RectF& rect) {
        return rect.id == m_ActiveId;
        });

    if (itr == m_InnerRects.end())
        return;

    auto& topRect = (*itr);
    if (!topRect.b_clicked)
        return;

    std::cout << "[MouseMove]: " << offset.x << "," << offset.y << std::endl;

	topRect.rect.l += offset.x;
	topRect.rect.r += offset.x;
	topRect.rect.t += offset.y;
	topRect.rect.b += offset.y;
}

void OpenglRender::RemoveBackRect()
{
    if (m_InnerRects.empty())
        return;

    m_InnerRects.erase(m_InnerRects.end());
}

void OpenglRender::UpdateCircle(XL_PointF pt_on_circle)
{
    if (m_InnerCircles.empty())
        return;

	auto& circle = m_InnerCircles.back();
    float dx = pt_on_circle.x - circle.pt_center.x;
    float dy = pt_on_circle.y - circle.pt_center.y;
	circle.f_radiusX = dx;
	circle.f_radiusY = dy;
}

bool OpenglRender::SetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1 };
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pf = ChoosePixelFormat(hdc, &pfd);
    if (pf == 0)
        return false;

    if (!SetPixelFormat(hdc, pf, &pfd)) 
        return false;

    return true;
}

XL_PointF OpenglRender::ScreenToWorld(const XL_PointF& screenPos)
{
	float width = static_cast<float>(m_WindowWidth);
	float height = static_cast<float>(m_WindowHeight);

    XL_PointF worldPos{ (screenPos.x / width) * 2.0f - 1.0f , 1.0f - (screenPos.y / height) * 2.0f };
	return worldPos;
}

void OpenglRender::OnSize(int cx, int cy)
{
    if (m_hRC) {
        wglMakeCurrent(m_hDC, m_hRC);
        m_Renderer->Resize(cx, cy);

        m_WindowWidth = cx;
        m_WindowHeight = cy;

		m_FrameBuffer->Resize(cx, cy);

		OnPaint();
    }
}

void OpenglRender::OnPaint()
{
    if (m_WindowWidth == 0 && m_WindowHeight == 0)
        return;

    auto start = std::chrono::system_clock::now();
    m_FrameBuffer->Bind();

    m_Renderer->ClearScene();
    m_Renderer->ResetDrawCall();
    //m_Renderer->UpdateCamera();
    /*  Core Draw Functions Here */
    ///////////////////////////////////////////

	float x = 0.0f;
	float y = 0.0f;

    for ( const auto& inner_rect : m_InnerRects)
    {
        x = inner_rect.thickness_x * inner_rect.tess_level / (inner_rect.rect.r - inner_rect.rect.l);
        y = inner_rect.thickness_y * inner_rect.tess_level / (inner_rect.rect.b - inner_rect.rect.t);

        m_Renderer->DrawRectangle(
            XL::DrawPlane::XY,
            inner_rect.rect,
            inner_rect.z_near,
            ToColorF(inner_rect.background_color),
            inner_rect.tess_level,
            glm::vec2{ x, y },
			glm::ivec2{ inner_rect.select_cell_x, inner_rect.select_cell_y }
        );
    }

    for ( const auto& circle : m_InnerCircles )
    {
        auto pCenter = ScreenToWorld(circle.pt_center);

        float radiusX = circle.f_radiusX * 2.0f / m_WindowWidth;
        float radiusY = circle.f_radiusY * 2.0f / m_WindowHeight;

        m_Renderer->DrawCircle(XL::DrawPlane::XY, pCenter.x, pCenter.y, circle.f_z_near, radiusX, radiusY, ToColorF(circle.c_border_color), circle.f_border_width, 0.01f);
    }

    ///////////////////////////////////////////
	m_Renderer->Flush();
    m_FrameBuffer->Unbind();

    SwapBuffers(m_hDC);
    auto end = std::chrono::system_clock::now();
    m_nFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void OpenglRender::FillRectangle(
    const XL_RectF& rect, 
    const XL_ColorF& bg_color, 
    float tess_level, 
    float thicknessX, 
    float thicknessY)
{
    m_QuadBorderWidth_X = thicknessX;
    m_QuadBorderWidth_Y = thicknessY;

    m_InnerRects.emplace_back(m_id, m_fZnear, m_QuadBorderWidth_X, m_QuadBorderWidth_Y, rect, bg_color, tess_level);
    m_fZnear -= 0.000001f;
    m_id++;
}

void OpenglRender::DrawRectangle(const XL_RectF& rect, const XL_ColorF& border_color, float border_width)
{
}

void OpenglRender::FillTriangle(const XL_TriangleF& riangle, const XL_ColorF& bg_color)
{
    auto p0 = ScreenToWorld(riangle.p0);
    auto p1 = ScreenToWorld(riangle.p1);
    auto p2 = ScreenToWorld(riangle.p2);
    m_Renderer->DrawTriangle(
        XL::DrawPlane::XY,
        p0.x,
        p0.y,
        p1.x,
        p1.y,
        p2.x,
        p2.y,
        ToColorF(bg_color)
	);
}

void OpenglRender::FillEllipse(const XL_PointF& center, float pixel_radius_x, float pixel_radius_y, const XL_ColorF& border_color)
{
    DrawEllipse(center, pixel_radius_x, pixel_radius_y, border_color, 10000.0f);
}

void OpenglRender::DrawEllipse(const XL_PointF& center, float pixel_radius_x, float pixel_radius_y, const XL_ColorF& border_color, float border_width)
{
    INNER_CircleF circle;
    circle.u_id = m_id;
    m_id++;

    circle.f_z_near = m_fZnear;
    m_fZnear -= 0.000001f;

    circle.pt_center = center;
    circle.c_border_color = border_color;
    circle.f_radiusX = pixel_radius_x;
    circle.f_radiusY = pixel_radius_y;
    circle.f_border_width = border_width;
    m_InnerCircles.push_back(circle);
}

void OpenglRender::FillCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color)
{
    DrawCircle(center, pixel_radius, border_color, 10000.0f);
}

void OpenglRender::DrawCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color, float border_width)
{
    DrawEllipse(center, pixel_radius, pixel_radius, border_color, border_width);
}
