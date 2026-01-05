#include "XL_OpenglRender.hpp"
#include "XL_Renderer.hpp"
#include "XL_FrameBuffer.hpp"
#include <windows.h>
#include <gl/wglext.h>
#include <gl/gl.h>

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

    std::sort(hitRects.begin(), hitRects.end(), [](const INNER_RectF* a, const INNER_RectF* b) {
        return a->z_order < b->z_order;
    });

    if (!hitRects.empty())
    {
        INNER_RectF& topRect = *hitRects.back();
		topRect.background_color.r = static_cast<float>((static_cast<int>(topRect.background_color.r * 255) ^ m_nXORKey)) / 255.0f;
		OnPaint();
	}
}

void OpenglRender::OnLButtonUp(int x, int y)
{
    std::vector<INNER_RectF*> hitRects;

    for (auto& rect : m_InnerRects)
    {
        if (PtInRect(XL_PointF{ (float)x,(float)y }, rect.rect))
        {
            hitRects.push_back(&rect);
        }
    }

    std::sort(hitRects.begin(), hitRects.end(), [](const INNER_RectF* a, const INNER_RectF* b) {
        return a->z_order < b->z_order;
        });

    if (!hitRects.empty())
    {
        INNER_RectF& topRect = *hitRects.back();
        topRect.background_color.r = static_cast<float>((static_cast<int>(topRect.background_color.r * 255) ^ m_nXORKey)) / 255.0f;
        OnPaint();
    }
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

	XL_PointF worldPos;
	worldPos.x = (screenPos.x / width) * 2.0f - 1.0f;
	worldPos.y = 1.0f - (screenPos.y / height) * 2.0f;
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
    constexpr float W = 15;
	constexpr float H = 7;
	constexpr float G = 2;
	int nCX = static_cast<int>(m_WindowWidth / (W + G));
	int nCY = static_cast<int>(m_WindowHeight / (H + G));
    /*for (float i = 0;i < nCX; i++)
    {
        for (float j = 0; j < nCY; j++)
        {
            FillRectangle(XL_RectF{ i * (W + G) ,j * (H + G),i * (W + G) + W, j * (H + G) + H }, XL_ColorF{ (int(i * j) % 255) / 255.0f,(int(i + j) % 255) / 255.0f,(int(pow(i, j)) % 255) / 255.0f,1.0f });
        }
    }*/
    //for (float j = 0.; j < 2.0; j+=0.1)
    //{
    //    m_Renderer->DrawLine(glm::vec3{ -1.0 + j, -0.8, 0.0 }, glm::vec3{ -1.0 + j, 0.8, 0.0 }, glm::vec4{ 1.0f,0.0f,0.0f,1.0f }, 1.0f);
    //}

    std::sort(m_InnerRects.begin(), m_InnerRects.end(), [](const INNER_RectF& a, const INNER_RectF& b) {
        return a.z_order > b.z_order;
		});

    for ( const auto& inner_rect : m_InnerRects)
    {
        auto lt = ScreenToWorld(XL_PointF{ inner_rect.rect.l, inner_rect.rect.t });
        auto br = ScreenToWorld(XL_PointF{ inner_rect.rect.r, inner_rect.rect.b });

        m_Renderer->DrawRectangle(XL::DrawPlane::XY, lt.x, lt.y, br.x, br.y, ToColorF(inner_rect.background_color), inner_rect.tess_level);
    }

    /*FillRectangle(XL_RectF{ 100,100,105,105 }, XL_ColorF{ 1.0f,0.0f,0.0f,1.0f });
    FillRectangle(XL_RectF{ 100,110,300,310 }, XL_ColorF{ 1.0f,0.0f,1.0f,1.0f });

    FillEllipse(XL_PointF{500,500}, 100,200, XL_ColorF{ 1.0f, 0.0f, 1.0f, 1.0f });
    DrawEllipse(XL_PointF{500,550}, 100,100, XL_ColorF{ 0.3f, 0.0f, 0.0f, 1.0f });

    FillCircle(XL_PointF{ 800,800 }, 5, XL_ColorF{ 1.0f, 0.0f, 1.0f, 1.0f });
    DrawCircle(XL_PointF{ 900,900 }, 5, XL_ColorF{ 1.0f, 0.0f, 0.0f, 1.0f }, 0.1f);*/
    //FillRectangle(XL_RectF{ 300,300,400,400 }, XL_ColorF{ 1.0f,0.0f,1.0f,1.0f });
    //DrawRectangle(XL_RectF{ 500,500,800,600 }, XL_ColorF{ 1.0f,0.0f,1.0f,1.0f }, 5.0f);

    ///////////////////////////////////////////
	m_Renderer->Flush();
    m_FrameBuffer->Unbind();

    SwapBuffers(m_hDC);
    auto end = std::chrono::system_clock::now();
    m_nFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    int x = 1;
    x++;
}

void OpenglRender::FillRectangle(const XL_RectF& rect, const XL_ColorF& bg_color, float tess_level)
{
    m_ZOrderCounter++;
    m_InnerRects.emplace_back(m_ZOrderCounter, rect, bg_color, tess_level);
}

void OpenglRender::DrawRectangle(const XL_RectF& rect, const XL_ColorF& border_color, float border_width)
{
    auto lt = ScreenToWorld(XL_PointF{ rect.l, rect.t });
    auto br = ScreenToWorld(XL_PointF{ rect.r, rect.b });
    // Top
    m_Renderer->DrawRectangle(
        XL::DrawPlane::XY, 
        lt.x, 
        lt.y, 
        br.x, 
        lt.y + border_width * 2.0f / m_WindowHeight, 
        ToColorF(border_color)
    );
    // Bottom
    m_Renderer->DrawRectangle(
        XL::DrawPlane::XY, 
        lt.x, 
        br.y - border_width * 2.0f / m_WindowHeight, 
        br.x, 
        br.y, 
        ToColorF(border_color)
    );
    // Left
    m_Renderer->DrawRectangle(
        XL::DrawPlane::XY, 
        lt.x, 
        lt.y, 
        lt.x + border_width * 2.0f / m_WindowWidth, 
        br.y, ToColorF(border_color)
    );
    // Right
	m_Renderer->DrawRectangle(
        XL::DrawPlane::XY,
        br.x - border_width * 2.0f / m_WindowWidth, 
        lt.y, 
        br.x, 
        br.y, 
        ToColorF(border_color)
    );
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

void OpenglRender::FillEllipse(const XL_PointF& center, float pixelX, float pixelY, const XL_ColorF& fill_color)
{
    auto pCenter = ScreenToWorld(center);

    float radiusX = pixelX * 2.0f / m_WindowWidth;
    float radiusY = pixelY * 2.0f / m_WindowHeight;

    m_Renderer->DrawCircle(XL::DrawPlane::XY, pCenter.x, pCenter.y, radiusX, radiusY, ToColorF(fill_color), 1.0f, 0.01f);
}

void OpenglRender::DrawEllipse(const XL_PointF& center, float pixelX, float pixelY, const XL_ColorF& border_color)
{
    auto pCenter = ScreenToWorld(center);

    float radiusX = pixelX * 2.0f / m_WindowWidth;
    float radiusY = pixelY * 2.0f / m_WindowHeight;

    m_Renderer->DrawCircle(XL::DrawPlane::XY, pCenter.x, pCenter.y, radiusX, radiusY, ToColorF(border_color), 0.1f, 0.01f);
}

void OpenglRender::FillCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& fill_color)
{
    auto pCenter = ScreenToWorld(center);

    float radiusX = pixel_radius * 2.0f / m_WindowWidth;
    float radiusY = pixel_radius * 2.0f / m_WindowHeight;

    m_Renderer->DrawCircle(XL::DrawPlane::XY, pCenter.x, pCenter.y, radiusX, radiusY, ToColorF(fill_color), 1.0f, 0.01f);
    m_Renderer->DrawCircle(XL::DrawPlane::XY, pCenter.x, pCenter.y, radiusX, radiusY, ToColorF(XL_ColorF{0.0f, 0.0f, 0.0f, 1.0f}), 0.01f, 0.01f);
}

void OpenglRender::DrawCircle(const XL_PointF& center, float pixel_radius, const XL_ColorF& border_color, float border_width)
{
    auto pCenter = ScreenToWorld(center);

    float radiusX = pixel_radius * 2.0f / m_WindowWidth;
    float radiusY = pixel_radius * 2.0f / m_WindowHeight;

    m_Renderer->DrawCircle(XL::DrawPlane::XY, pCenter.x, pCenter.y, radiusX, radiusY, ToColorF(border_color), 1.0f, 0.01f);
}
