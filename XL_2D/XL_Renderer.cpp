#include "XL_Renderer.hpp"
#include "XL_Data.hpp"

_NAMESPACE_BEGIN

BatchRenderer::BatchRenderer()
	: mViewportWidth{ 1920 }, mViewportHeight{ 1080 } {
}

void BatchRenderer::Resize(int nWidth, int nHeight)
{
	mViewportWidth = nWidth;
	mViewportHeight = nHeight;
    glViewport(0, 0, mViewportWidth, mViewportHeight);

    if (mViewportWidth > 0 && mViewportHeight > 0)
        m_Camera->OnResize((float)nWidth, (float)nHeight);
}

void BatchRenderer::UpdateCamera()
{
    m_Camera->OnUpdate();
}

bool BatchRenderer::Init(pfnGladLoader loader)
{
    int gladLoaded = GladLoadWithRetry(loader, 5, 10);
    if (!gladLoaded)
        return false;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_Shader = std::make_unique<Shader>();
    m_Shader->LoadShader(batch_vs, batch_fs);
    m_Shader->UnBind();

	m_TriangleBatchVertex = std::make_unique<BatchVertexArray>();
	m_TriangleBatchVertex->Bind( m_TriangleBatchVertices, m_TriangleVertexCount );
	m_LineBatchVertex = std::make_unique<BatchVertexArray>();
	m_LineBatchVertex->Bind( m_LineBatchVertices, m_LineVertexCount );

    m_Camera = std::make_unique<Camera>(16.0f / 9.0f);

    return true;
}

void BatchRenderer::OnKeyDown(uint32_t nChar, uint32_t nRepCnt)
{
    m_Camera->OnKeyDown(nChar, nRepCnt);
}

void BatchRenderer::OnMouseWheel(int yOffset)
{
    //m_Camera->OnMouseWheel(yOffset);
}

void BatchRenderer::OnMouseMove(int xOffset, int yOffset)
{
   // m_Camera->OnMouseMove(xOffset, yOffset);
}

glm::vec3 BatchRenderer::TransformPos(const glm::vec3& in, const glm::mat4& model)
{
    glm::vec4 p = model * glm::vec4(in, 1.0f);
    return glm::vec3(p.x, p.y, p.z);
}

void BatchRenderer::DrawTriangle(
    DrawPlane plane, 
    float x0, 
    float y0, 
    float x1, 
    float y1, 
    float x2, 
    float y2,
    const glm::vec4& color
)
{
    switch (plane)
    {
        case XL::DrawPlane::XY:
        {
            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ x0, y0, 0.0 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ x1, y1, 0.0 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ x2, y2, 0.0 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
            break;
        }
        case XL::DrawPlane::XZ:
        {
            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ x0, 0.0, y0 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ x1, 0.0, y1 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ x2, 0.0, y2 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
            break;
        }
        case XL::DrawPlane::YZ:
        {
            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ 0.0, x0, y0 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ 0.0, x1, y1 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3{ 0.0, x2, y2 };
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
            break;
        }
        default:
            break;
    }

    // 如果超限则 flush
    if (m_TriangleVertexCount + 3U > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawRectangle(
    DrawPlane plane,
    float l, float t, float r, float b,
    const glm::vec4& color
)
{
    switch (plane)
    {
    case XL::DrawPlane::XY:
    {
        // 0
		m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(l, t, 0.0f);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //1
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(r, t, 0.0f);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //2
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(r, b, 0.0f);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //3
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(l, b, 0.0f);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        break;
    }
    case XL::DrawPlane::XZ:
    {
        // 0
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(l, 0.0f, t);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //1
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(r, 0.0f, t);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //2
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(r, 0.0f, b);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //3
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(l, 0.0f, b);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        break;
    }
    case XL::DrawPlane::YZ:
    {
        // 0
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(0.0f, l, t);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //1
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(0.0f, r, t);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //2
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(0.0f, r, b);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        //3
        m_TriangleBatchVertices[m_TriangleVertexCount].position = glm::vec3(0.0f, l, b);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
        break;
    }
    default:
        break;
    }

    if (m_TriangleVertexCount + 4 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawCube(
    const TransFormData& trans,
    const glm::vec4& color
)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), trans.position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(trans.rotate.x), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(trans.rotate.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(trans.rotate.z), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), trans.scale);

    const auto* srcVerts = &g_sCubeVertices.vertices[0];
    const auto& indices = g_sCubeVertices.indices; // 36 个索引

    for (size_t i = 0; i < indices.size(); ++i)
    {
        unsigned int idx = indices[i];
        glm::vec3 local{ (srcVerts[idx])[0], (srcVerts[idx])[1], (srcVerts[idx])[2] };

        m_TriangleBatchVertices[m_TriangleVertexCount].position = TransformPos(local, model);
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
    }

    if (m_TriangleVertexCount + 36 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawLine(
    const glm::vec3& start,
    const glm::vec3& end,
    const glm::vec4& color
)
{
    BatchRenderVertex v0;
    v0.position = start;
    v0.color = color;
    BatchRenderVertex v1;
    v1.position = end;
    v1.color = color;

	m_LineBatchVertices[m_LineVertexCount].position = start;
	m_LineBatchVertices[m_LineVertexCount].color = color;
    m_LineVertexCount++;
    m_LineBatchVertices[m_LineVertexCount].position = end;
    m_LineBatchVertices[m_LineVertexCount].color = color;
    m_LineVertexCount++;

    if (m_LineVertexCount + 2 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawCircle(
    DrawPlane plane,
    const glm::vec3& pos,
    const glm::vec3& rotate,
    float radius,
    const glm::vec4& color,
    bool filled,
    int segments
)
{
    if (radius <= 0.0f)
        return;

    if (segments < 3) segments = 3;

    // build model: translate -> rotateX/Y/Z -> scale(radius)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, radius));

    // 生成圆周点（局部单位圆）
    std::vector<glm::vec3> pts;
    pts.reserve(segments + 1);
    const float TWO_PI = glm::two_pi<float>();
    for (int i = 0; i <= segments; ++i) // <= 用于闭合（最后一个等于第一个）
    {
        float t = (float)i / (float)segments;
        float ang = t * TWO_PI;
        glm::vec3 p;
        switch (plane)
        {
        case DrawPlane::XY:
            p = glm::vec3(std::cos(ang), std::sin(ang), 0.0f);
            break;
        case DrawPlane::XZ:
            p = glm::vec3(std::cos(ang), 0.0f, std::sin(ang));
            break;
        case DrawPlane::YZ:
            p = glm::vec3(0.0f, std::cos(ang), std::sin(ang));
            break;
        default:
            p = glm::vec3(std::cos(ang), std::sin(ang), 0.0f);
            break;
        }
        pts.push_back(TransformPos(p, model));
    }

    if (filled)
    {
        // triangle fan: center + (p_i, p_{i+1})
        // 预计添加 vertices = segments * 3
        if (m_TriangleVertexCount + (size_t)segments * 3 > MaxBatchVertices)
            Flush();

        glm::vec3 center = TransformPos(glm::vec3(0.0f), model);
        for (int i = 0; i < segments; ++i)
        {
			m_TriangleBatchVertices[m_TriangleVertexCount].position = center;
			m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i + 1];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
        }
    }
    else
    {
        // 绘制轮廓：把相邻点作为线段加入 m_LineBatchVertices
        if (m_TriangleVertexCount + (size_t)segments * 2 > MaxBatchVertices)
            Flush();

        for (int i = 0; i < segments; ++i)
        {
            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i + 1];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
        }
    }
}

void BatchRenderer::DrawCircle_Pixel(
    DrawPlane plane,
    const glm::vec3& pos,
    const glm::vec3& rotate,
    float radius_pixels,
    const glm::vec4& color,
    bool filled,
    int segments
)
{
    if (radius_pixels <= 0.0f || segments < 3)
        return;

    // build model without scale (we'll apply radius in world units computed from pixels)
    glm::mat4 modelBase = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0, 0, 1));
    // no scale here

// center in world space
    glm::vec3 centerWorld = TransformPos(glm::vec3(0.0f), modelBase);

    // get viewport
    GLint vp[4]{ 0,0,0,0 };
    glGetIntegerv(GL_VIEWPORT, vp);
    glm::vec4 viewport = glm::vec4((float)vp[0], (float)vp[1], (float)vp[2], (float)vp[3]);
    if (viewport.z <= 0.0f || viewport.w <= 0.0f)
        return;

    // project center to window coords
    glm::vec3 centerWin = glm::project(centerWorld, m_Camera->GetView(), m_Camera->GetProjection(), viewport);

    // construct a window-space point offset by radius_pixels along +X
    glm::vec3 offsetWin = centerWin;
    offsetWin.x += radius_pixels;

    // unproject both to world to compute world-space radius length
    glm::vec3 offsetWorld = glm::unProject(offsetWin, m_Camera->GetView(), m_Camera->GetProjection(), viewport);

    float worldRadius = glm::length(offsetWorld - centerWorld);
    if (worldRadius <= std::numeric_limits<float>::epsilon())
        return;

    // build circle points in local unit circle scaled by worldRadius, then apply rotation+translation (modelBase)
    std::vector<glm::vec3> pts;
    pts.reserve(segments + 1);
    const float TWO_PI = glm::two_pi<float>();
    for (int i = 0; i <= segments; ++i)
    {
        float t = (float)i / (float)segments;
        float ang = t * TWO_PI;
        glm::vec3 pLocal;
        switch (plane)
        {
        case DrawPlane::XY:
            pLocal = glm::vec3(std::cos(ang) * worldRadius, std::sin(ang) * worldRadius, 0.0f);
            break;
        case DrawPlane::XZ:
            pLocal = glm::vec3(std::cos(ang) * worldRadius, 0.0f, std::sin(ang) * worldRadius);
            break;
        case DrawPlane::YZ:
            pLocal = glm::vec3(0.0f, std::cos(ang) * worldRadius, std::sin(ang) * worldRadius);
            break;
        default:
            pLocal = glm::vec3(std::cos(ang) * worldRadius, std::sin(ang) * worldRadius, 0.0f);
            break;
        }
        pts.push_back(TransformPos(pLocal, modelBase));
    }

    if (filled)
    {
        // triangle fan: center + (p_i, p_{i+1})
        if (m_TriangleVertexCount + (size_t)segments * 3 > MaxBatchVertices)
            Flush();

        for (int i = 0; i < segments; ++i)
        {
            m_TriangleBatchVertices[m_TriangleVertexCount].position = centerWorld;
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i + 1];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
        }
    }
    else
    {
        // outline: adjacent points as line segments
        if (m_TriangleVertexCount + (size_t)segments * 2 > MaxBatchVertices)
            Flush();

        for (int i = 0; i < segments; ++i)
        {
            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;

            m_TriangleBatchVertices[m_TriangleVertexCount].position = pts[i + 1];
            m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
            m_TriangleVertexCount++;
        }
    }
}

int BatchRenderer::GladLoadWithRetry(pfnGladLoader loader, int maxAttempts, int delayMs)
{
    for (int attempt = 1; attempt <= maxAttempts; ++attempt)
    {
        int ok = gladLoadGLUserPtr(loader, NULL);

        if (ok)
            return ok;

        if (attempt < maxAttempts)
            XL_mSleep(delayMs);
    }

    return 0;
}

void BatchRenderer::Flush()
{
    if ( m_TriangleVertexCount > 0 )
    {
        m_Shader->Bind();

        m_TriangleBatchVertex->Bind(m_TriangleBatchVertices, m_TriangleVertexCount);
        glDrawElements(GL_TRIANGLES, (GLsizei)(m_TriangleVertexCount * 6u), GL_UNSIGNED_INT, 0);
        m_TriangleBatchVertex->UnBind();
        m_Shader->UnBind();
        m_TriangleVertexCount = 0;
        m_DrawCall += 1;
    }

    if ( m_LineVertexCount > 0 )
    {
        m_Shader->Bind();

        m_LineBatchVertex->Bind(m_LineBatchVertices, m_LineVertexCount);
        glDrawElements(GL_LINES, (GLsizei)m_LineVertexCount, GL_UNSIGNED_INT, 0);

        m_LineBatchVertex->UnBind();
        m_Shader->UnBind();

        m_LineVertexCount = 0;
        m_DrawCall += 1;
    }
}

_NAMESPACE_END