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

    // 使用复合 tessellation shader
    m_QuadBatch.m_Shader = std::make_unique<Shader>();
    m_QuadBatch.m_Shader->LoadShaderStages(QUAD_DATA::quad_tess);
    m_QuadBatch.m_Shader->UnBind();
    m_QuadBatch.m_Vertex = std::make_unique<BatchVertexArray>(VertexType::BatchQuadVertex);
    m_QuadBatch.m_Vertex->Bind(m_QuadBatch.m_Vertices, m_QuadBatch.m_VertexCount);
    m_QuadBatch.m_VertexCount = 0;

    // circle/line 仍使用原有简单 shader
    m_CircleBatch.m_Shader = std::make_unique<Shader>();
    m_CircleBatch.m_Shader->LoadShader(CIRCLE_DATA::circle_vs, CIRCLE_DATA::circle_fs);
    m_CircleBatch.m_Shader->UnBind();
    m_CircleBatch.m_Vertex = std::make_unique<BatchVertexArray>(VertexType::BatchCircleVertex);
    m_CircleBatch.m_Vertex->Bind(m_QuadBatch.m_Vertices, m_QuadBatch.m_VertexCount);
    m_CircleBatch.m_VertexCount = 0;

    m_LineBatch.m_Shader = std::make_unique<Shader>();
    m_LineBatch.m_Shader->LoadShader(LINE_DATA::line_vs, LINE_DATA::line_fs);
    m_LineBatch.m_Shader->UnBind();
    m_LineBatch.m_Vertex = std::make_unique<BatchVertexArray>(VertexType::BatchLineVertex);
    m_LineBatch.m_Vertex->Bind(m_QuadBatch.m_Vertices, m_QuadBatch.m_VertexCount);
    m_LineBatch.m_VertexCount = 0;

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
#if 0
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
#endif
}

void BatchRenderer::DrawRectangle(
    DrawPlane plane,
    float l, float t, float r, float b,
    const glm::vec4& color,
    float tess_level
)
{
    switch (plane)
    {
    case XL::DrawPlane::XY:
    {
        // 顶点顺序必须是： left-top, right-top, right-bottom, left-bottom
        // 并且 local 值按象限设置： LT(-1,1) RT(1,1) RB(1,-1) LB(-1,-1)

        // left-top
        auto& vert_lt = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];

        vert_lt.position = glm::vec3(l, t, 0.0f);
        vert_lt.color = color;
        vert_lt.local = glm::vec2(-1.0f, 1.0f);
        vert_lt.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        // right-top
        auto& vert_rt = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_rt.position = glm::vec3(r, t, 0.0f);
        vert_rt.color = color;
        vert_rt.local = glm::vec2(1.0f, 1.0f);
        vert_rt.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        // right-bottom
        auto& vert_rb = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_rb.position = glm::vec3(r, b, 0.0f);
        vert_rb.color = color;
        vert_rb.local = glm::vec2(1.0f, -1.0f);
        vert_rb.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        // left-bottom
        auto& vert_lb = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_lb.position = glm::vec3(l, b, 0.0f);
        vert_lb.color = color;
        vert_lb.local = glm::vec2(-1.0f, -1.0f);
        vert_lb.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;
        break;
    }
    case XL::DrawPlane::XZ:
    {
        // left-top (LT) maps to (l, 0, t)
        auto& vert_lt = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_lt.position = glm::vec3(l, 0.0f, t);
        vert_lt.color = color;
        vert_lt.local = glm::vec2(-1.0f, 1.0f);
        vert_lt.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        // right-top
        auto& vert_rt = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_rt.position = glm::vec3(r, 0.0f, t);
        vert_rt.color = color;
        vert_rt.local = glm::vec2(1.0f, 1.0f);
        vert_rt.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        // right-bottom
        auto& vert_rb = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_rb.position = glm::vec3(r, 0.0f, b);
        vert_rb.color = color;
        vert_rb.local = glm::vec2(1.0f, -1.0f);
        vert_rb.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        // left-bottom
        auto& vert_lb = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_lb.position = glm::vec3(l, 0.0f, b);
        vert_lb.color = color;
        vert_lb.local = glm::vec2(-1.0f, -1.0f);
        vert_lb.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;
        break;
    }
    case XL::DrawPlane::YZ:
    {
        // left-top -> (0, l, t) 这里 local.x 用于行方向(local.x 对应 second axis)
        auto& vert_lt = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_lt.position = glm::vec3(0.0f, l, t);
        vert_lt.color = color;
        vert_lt.local = glm::vec2(-1.0f, 1.0f);
        vert_lt.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        auto& vert_rt = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_rt.position = glm::vec3(0.0f, r, t);
        vert_rt.color = color;
        vert_rt.local = glm::vec2(1.0f, 1.0f);
        vert_rt.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        auto& vert_rb = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_rb.position = glm::vec3(0.0f, r, b);
        vert_rb.color = color;
        vert_rb.local = glm::vec2(1.0f, -1.0f);
        vert_rb.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;

        auto& vert_lb = m_QuadBatch.m_Vertices[m_QuadBatch.m_VertexCount];
        vert_lb.position = glm::vec3(0.0f, l, b);
        vert_lb.color = color;
        vert_lb.local = glm::vec2(-1.0f, -1.0f);
        vert_lb.tessLevel = tess_level;
        m_QuadBatch.m_VertexCount++;
        break;
    }
    default:
        break;
    }

    if (m_QuadBatch.m_VertexCount + 4 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawCube(const glm::vec4& color)
{
#if 0
    const auto* srcVerts = &g_sCubeVertices.vertices[0];
    const auto& indices = g_sCubeVertices.indices; // 36 个索引

    for (size_t i = 0; i < indices.size(); ++i)
    {
        unsigned int idx = indices[i];
        glm::vec3 local{ (srcVerts[idx])[0], (srcVerts[idx])[1], (srcVerts[idx])[2] };

        m_TriangleBatchVertices[m_TriangleVertexCount].position = local;
        m_TriangleBatchVertices[m_TriangleVertexCount].color = color;
        m_TriangleVertexCount++;
    }

    if (m_TriangleVertexCount + 36 > MaxBatchVertices)
        Flush();
#endif
}

void BatchRenderer::DrawLine(
    const glm::vec3& start,
    const glm::vec3& end,
    const glm::vec4& color,
    float line_width /*= 1.0f*/
)
{
	auto& vert_start = m_LineBatch.m_Vertices[m_LineBatch.m_VertexCount];
    vert_start.position = start;
    vert_start.color = color;
    m_LineBatch.m_VertexCount++;

    auto& vert_end = m_LineBatch.m_Vertices[m_LineBatch.m_VertexCount];
    vert_end.position = end;
    vert_end.color = color;
    m_LineBatch.m_VertexCount++;

    if (m_LineBatch.m_VertexCount + 2 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawCircle(
    DrawPlane plane,
    float x,
    float y,
    float radiusX,
    float radiusY,
    const glm::vec4& color, 
    float thickness /*= 1.0f*/,
    float fade /*= 0.005f*/
)
{
    // 计算四边形在“世界/NDC”空间的边界
    float l = x - radiusX;
    float t = y + radiusY;
    float r = x + radiusX;
    float b = y - radiusY;

    glm::vec3 center = glm::vec3(x, y, 0.0f);

    // 四角世界坐标
    const glm::vec3 worldPts[4] = {
        glm::vec3(l, t, 0.0f),
        glm::vec3(r, t, 0.0f),
        glm::vec3(r, b, 0.0f),
        glm::vec3(l, b, 0.0f)
    };

    // 对应的归一化局部坐标 (相对于中心、按 radiusX/radiusY 缩放)
    for (int i = 0; i < 4; ++i)
    {
        glm::vec3 local;
        local.x = (worldPts[i].x - center.x) / radiusX; // -1..1
        local.y = (worldPts[i].y - center.y) / radiusY; // -1..1
        local.z = 0.0f;

        auto& v = m_CircleBatch.m_Vertices[m_CircleBatch.m_VertexCount];
        v.worldPosition = worldPts[i];
        v.localPosition = local;
        v.color = color;
        v.thickness = thickness;
        v.fade = fade;
        m_CircleBatch.m_VertexCount++;
    }

    if (m_CircleBatch.m_VertexCount + 4 > MaxBatchVertices)
        Flush();
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
    // 使用补丁绘制四边形以触发 tessellation (每个 patch 4 个控制点)
    if (m_QuadBatch.m_VertexCount > 0)
    {
        m_QuadBatch.m_Shader->Bind();

        //m_QuadBatch.m_Shader->SetInt("u_cX", 2);
        //m_QuadBatch.m_Shader->SetInt("u_cY", 3);
        m_QuadBatch.m_Shader->SetFloat("u_BorderThickness", 0.02f);
        m_QuadBatch.m_Shader->Set4f("u_BorderColor", glm::vec4{ 0.0, 0.0, 0.0, 1.0 });
        m_QuadBatch.m_Shader->SetFloat("u_BorderAA", 0.01f);

        // 上传顶点数据到 GPU
        m_QuadBatch.m_Vertex->Bind(m_QuadBatch.m_Vertices, m_QuadBatch.m_VertexCount);

        // 设置 patch 顶点数为 4
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        // 按连续 control points 绘制 patch（注意：顶点按 4 个一组排列）
        glDrawArrays(GL_PATCHES, 0, static_cast<GLsizei>(m_QuadBatch.m_VertexCount));

        m_QuadBatch.m_Vertex->UnBind();
        m_QuadBatch.m_Shader->UnBind();
        m_QuadBatch.m_VertexCount = 0;
        m_DrawCall += 1;
    }

    // circle 使用原来的绘制流程
    if (m_CircleBatch.m_VertexCount > 0)
    {
        m_CircleBatch.m_Shader->Bind();

        m_CircleBatch.m_Vertex->Bind(m_CircleBatch.m_Vertices, m_CircleBatch.m_VertexCount);
        glDrawElements(GL_TRIANGLES, (GLsizei)(m_CircleBatch.m_VertexCount * 6u / 4u ), GL_UNSIGNED_INT, 0);
        m_CircleBatch.m_Vertex->UnBind();

        m_CircleBatch.m_Shader->UnBind();
        m_CircleBatch.m_VertexCount = 0;
        m_DrawCall += 1;
    }

    if (m_LineBatch.m_VertexCount > 0)
    {
        m_LineBatch.m_Shader->Bind();
        m_LineBatch.m_Vertex->Bind(m_LineBatch.m_Vertices, m_LineBatch.m_VertexCount);
        glDrawArrays(GL_LINES, 0, (GLsizei)(m_LineBatch.m_VertexCount));
        m_LineBatch.m_Vertex->UnBind();
        m_LineBatch.m_Shader->UnBind();
        m_LineBatch.m_VertexCount = 0;
		m_DrawCall += 1;
    }
}

_NAMESPACE_END