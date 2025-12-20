#include "XL_OpenglRender.hpp"
#include "XL_BatchRenderer.hpp"
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

OpenglRender::OpenglRender(HWND hWnd, const WindowProp& prop)
    : m_hWnd{ hWnd }
    , m_hDC { NULL }
    , m_hRC { NULL }
    , m_WinProp(prop)
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
    m_Renderer->Init(glad_wgl_get_proc);
    m_Renderer->Resize(m_WinProp.Width, m_WinProp.Height);

    XL::FramebufferSpecification fbSpec;
    fbSpec.Width = m_WinProp.Width;
    fbSpec.Height = m_WinProp.Height;
    m_FrameBuffer = std::make_unique<XL::FrameBuffer>(fbSpec);

    return true;
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

void OpenglRender::OnSize(int cx, int cy)
{
    if (m_hRC) {
        wglMakeCurrent(m_hDC, m_hRC);
        m_Renderer->Resize(cx, cy);

        m_WinProp.Width = cx;
        m_WinProp.Height = cy;

		m_FrameBuffer->Resize(cx, cy);
    }
}

void OpenglRender::OnPaint()
{
    m_FrameBuffer->Bind();

    m_Renderer->ClearScene();
    m_Renderer->UpdateCamera();
    /*  Core Draw Functions Here */
    ///////////////////////////////////////////
    //m_Renderer->DrawTriangle(
    //    XL::DrawPlane::XY,
    //    glm::vec3(-0.5f, -0.5f, 1.0f), 	    // translate
    //    glm::vec3(0.0f, 0.0f, 30.0f),	    // rotate°¡
    //    glm::vec3(1.0f, 1.0f, 1.0f), 	    // scales
    //    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)	// color
    //);

    //m_Renderer->DrawTriangle(
    //    XL::DrawPlane::XY,
    //    glm::vec3(0.5f, 0.5f, 1.0f), 	    // translate
    //    glm::vec3(0.0f, 0.0f, 0.0f),	    // rotate
    //    glm::vec3(1.0f, 1.0f, 1.0f), 	    // scales
    //    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)	// color
    //);

    m_Renderer->DrawRectangle(
        XL::DrawPlane::XY,
        glm::vec3(-0.5f, -0.5f, 1.0f), 	    // translate
        glm::vec3(0.0f, 30.0f, 0.0f),	    // rotate
        glm::vec3(1.0f, 1.0f, 1.0f), 	    // scales
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)	// color
    );

    m_Renderer->DrawCube(
        glm::vec3(0.0f, 0.0f, -0.0f), 	// translate
        glm::vec3(0.0f, 0.0f, 0.0f),	// rotate
        glm::vec3(2.5f, 2.5f, 2.5f), 	// scales
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)		// color
    );

    constexpr float F_SIZE = 6.0f;
    constexpr float F_STEP = 1.0f;

    for (float x = -F_SIZE; x <= F_SIZE; x += F_STEP)
    {
        m_Renderer->DrawLine(
            glm::vec3(x, F_SIZE, 0.0f), 			// start
            glm::vec3(x, -F_SIZE, 0.0f), 			// end
            glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)	// color
        );
    }

    for (float y = -F_SIZE; y <= F_SIZE; y += F_STEP)
    {
        m_Renderer->DrawLine(
            glm::vec3(-F_SIZE, y, 0.0f), 			// start
            glm::vec3(F_SIZE, y, 0.0f), 			// end
            glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)	// color
        );
    }

    constexpr float STEP = 1.0f;
    glm::vec3 prevPoint{ 0.f, 0.f, 0.f };
    float z = 0.f;
    for (float theta = 0.f; theta <= 360.0f * 5.0f; theta += STEP)
    {
        float x = theta * 0.005f * cos(glm::radians(theta));
        float y = theta * 0.005f * sin(glm::radians(theta));
        glm::vec3 newPoint{ x, y, 0.0f };
        m_Renderer->DrawLine(prevPoint, newPoint, glm::vec4(1.f, 0.f, 0.f, 1.f));
        prevPoint = newPoint;
        z += 0.015f;
    }

    m_Renderer->DrawCircle(
        XL::DrawPlane::XY,
        glm::vec3(0.0f, 0.0f, -0.0f), 	// translate
        glm::vec3(0.0f, 0.0f, 0.0f),	// rotate
        3.0f,
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),	// color
        true,
        72
    );

    ///////////////////////////////////////////
	m_Renderer->Flush();
    m_FrameBuffer->Unbind();

    SwapBuffers(m_hDC);
}

void OpenglRender::OnKeyDown(UINT nChar, UINT nRepCnt)
{
    m_Renderer->OnKeyDown(nChar, nRepCnt);
}

void OpenglRender::OnMouseWheel(int yOffset)
{
	m_Renderer->OnMouseWheel(yOffset);
}

void OpenglRender::OnMouseMove(int xOffset, int yOffset)
{
    m_Renderer->OnMouseMove(xOffset,yOffset);
}
