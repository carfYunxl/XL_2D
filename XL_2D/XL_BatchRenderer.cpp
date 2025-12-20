#include "XL_BatchRenderer.hpp"
#include "XL_Data.hpp"

_NAMESPACE_BEGIN

void BatchRenderer::Resize(int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);

    if (nWidth > 0 && nHeight > 0)
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 非预乘 alpha（常用）
    glBlendEquation(GL_FUNC_ADD); // 默认即可
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_Shader = std::make_unique<Shader>();
    m_Shader->LoadShader(batch_vs, batch_fs);
    m_Shader->UnBind();

	m_TriangleBatchVertex = std::make_unique<BatchVertexArray>();
	m_TriangleBatchVertex->Bind(m_TriangleBatchVertices);
	m_LineBatchVertex = std::make_unique<BatchVertexArray>();
	m_LineBatchVertex->Bind(m_LineBatchVertices);

    m_Camera = std::make_unique<Camera>(10.0f / 9.0f);

    return true;
}

void BatchRenderer::OnKeyDown(uint32_t nChar, uint32_t nRepCnt)
{
    m_Camera->OnKeyDown(nChar, nRepCnt);
}

void BatchRenderer::OnMouseWheel(int yOffset)
{
    m_Camera->OnMouseWheel(yOffset);
}

void BatchRenderer::OnMouseMove(int xOffset, int yOffset)
{
    m_Camera->OnMouseMove(xOffset, yOffset);
}

static glm::vec3 TransformPos(const glm::vec3& in, const glm::mat4& model)
{
    glm::vec4 p = model * glm::vec4(in, 1.0f);
    return glm::vec3(p.x, p.y, p.z);
}

void BatchRenderer::DrawTriangle(
    DrawPlane plane,
    const glm::vec3& pos,
    const glm::vec3& rotate,
    const glm::vec3& scale,
    const glm::vec4& color
)
{
    // build model
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), scale);

    const auto* srcVerts = &g_sTriangleVertices.vertices_XY[0];
    if (plane == DrawPlane::XZ) srcVerts = &g_sTriangleVertices.vertices_XZ[0];
    if (plane == DrawPlane::YZ) srcVerts = &g_sTriangleVertices.vertices_YZ[0];

    // 每个三角形三顶点加入 batch
    for (int i = 0; i < 3; ++i)
    {
        BatchRenderVertex v;
        glm::vec3 local{ (srcVerts[i])[0], (srcVerts[i])[1], (srcVerts[i])[2] };
        v.position = TransformPos(local, model);
        v.color = color;
        m_TriangleBatchVertices.push_back(v);
    }

    // 如果超限则 flush
    if (m_TriangleBatchVertices.size() + 3 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawRectangle(
    DrawPlane plane,
    const glm::vec3& pos,
    const glm::vec3& rotate,
    const glm::vec3& scale,
    const glm::vec4& color
)
{
    // 使用批处理方式：把矩形索引展开为两个三角形（6 顶点），加入 m_TriangleBatchVertices
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), scale);

    const auto* srcVerts = &g_sRectangleVertices.vertices_XY[0];
    const auto& indices = g_sRectangleVertices.indices;
    if (plane == DrawPlane::XZ) srcVerts = &g_sRectangleVertices.vertices_XZ[0];
    if (plane == DrawPlane::YZ) srcVerts = &g_sRectangleVertices.vertices_YZ[0];

    // indices 有 6 个元素：0,1,2,2,3,0 -> 展开为 6 个顶点
    for (size_t i = 0; i < indices.size(); ++i)
    {
        unsigned int idx = indices[i];
        BatchRenderVertex v;
        glm::vec3 local{ (srcVerts[idx])[0], (srcVerts[idx])[1], (srcVerts[idx])[2] };
        v.position = TransformPos(local, model);
        v.color = color;
        m_TriangleBatchVertices.push_back(v);
    }

    if (m_TriangleBatchVertices.size() + 6 > MaxBatchVertices)
        Flush();
}

void BatchRenderer::DrawCube(
    const glm::vec3& pos,
    const glm::vec3& rotate,
    const glm::vec3& scale,
    const glm::vec4& color
)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1, 0, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotate.z), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), scale);

    const auto* srcVerts = &g_sCubeVertices.vertices[0];
    const auto& indices = g_sCubeVertices.indices; // 36 个索引

    for (size_t i = 0; i < indices.size(); ++i)
    {
        unsigned int idx = indices[i];
        BatchRenderVertex v;
        glm::vec3 local{ (srcVerts[idx])[0], (srcVerts[idx])[1], (srcVerts[idx])[2] };
        v.position = TransformPos(local, model);
        v.color = color;
        m_TriangleBatchVertices.push_back(v);
    }

    if (m_TriangleBatchVertices.size() + 36 > MaxBatchVertices)
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

    m_LineBatchVertices.push_back(v0);
    m_LineBatchVertices.push_back(v1);

    if (m_LineBatchVertices.size() + 2 > MaxBatchVertices)
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
    // Draw triangles (filled)
    if (!m_TriangleBatchVertices.empty())
    {
        m_Shader->Bind();
        m_Shader->SetMat4("u_View", m_Camera->GetView());
        m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());

		m_TriangleBatchVertex->Bind(m_TriangleBatchVertices);

        glDrawArrays(GL_TRIANGLES, 0, m_TriangleBatchVertices.size());

		m_TriangleBatchVertex->UnBind();
        m_Shader->UnBind();

        m_TriangleBatchVertices.clear();
    }

    // Draw lines
    if (!m_LineBatchVertices.empty())
    {
        m_Shader->Bind();
        m_Shader->SetMat4("u_View", m_Camera->GetView());
        m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());

		m_LineBatchVertex->Bind(m_LineBatchVertices);
        //glDisable(GL_DEPTH_TEST); // lines often drawn on top; keep choice to user — adjust if needed
        glDrawArrays(GL_LINES, 0, (GLsizei)m_LineBatchVertices.size());
        //glEnable(GL_DEPTH_TEST);
		m_LineBatchVertex->UnBind();
        m_Shader->UnBind();

        m_LineBatchVertices.clear();
    }
}

_NAMESPACE_END