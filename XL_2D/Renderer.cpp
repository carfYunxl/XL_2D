#include "Renderer.hpp"
#include <windows.h>

_NAMESPACE_BEGIN

void Renderer::Resize(int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);

    if(nWidth > 0 && nHeight > 0 )
        m_Camera->OnResize((float)nWidth, (float)nHeight);
}

void Renderer::UpdateCamera()
{
    m_Camera->OnUpdate();
}

bool Renderer::Init()
{
    int gladLoaded = GladLoadWithRetry(5, 10);
    if (!gladLoaded)
        return false;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 非预乘 alpha（常用）
    glBlendEquation(GL_FUNC_ADD); // 默认即可
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // XY Plane
    m_RectangleVertices.vertices[0] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f};
    m_RectangleVertices.vertices[1] = { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f};
    m_RectangleVertices.vertices[2] = { 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f};
    m_RectangleVertices.vertices[3] = {-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f};

    m_TriangleVertices.vertices[0] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f};
    m_TriangleVertices.vertices[1] = { 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f};
    m_TriangleVertices.vertices[2] = { 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f};

    // XZ Plane
    m_RectangleVertices_XZ.vertices[0] = {-0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f};
    m_RectangleVertices_XZ.vertices[1] = { 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f};
    m_RectangleVertices_XZ.vertices[2] = { 0.5f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f};
    m_RectangleVertices_XZ.vertices[3] = {-0.5f, 0.0f,  0.5f, 1.0f, 1.0f, 0.0f};

    m_TriangleVertices_XZ.vertices[0] = {-0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f};
    m_TriangleVertices_XZ.vertices[1] = { 0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f};
    m_TriangleVertices_XZ.vertices[2] = { 0.0f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f};

    // YZ Plane
    m_RectangleVertices_YZ.vertices[0] = {0.0f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f};
    m_RectangleVertices_YZ.vertices[1] = {0.0f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
    m_RectangleVertices_YZ.vertices[2] = {0.0f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f};
    m_RectangleVertices_YZ.vertices[3] = {0.0f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f};

    m_TriangleVertices_YZ.vertices[0] = {0.0f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f};
    m_TriangleVertices_YZ.vertices[1] = {0.0f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
    m_TriangleVertices_YZ.vertices[2] = {0.0f,  0.0f,  0.5f, 0.0f, 0.0f, 1.0f};

    m_CubeVertices.vertices[0] = {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f};
    m_CubeVertices.vertices[1] = { 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
    m_CubeVertices.vertices[2] = { 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f};
    m_CubeVertices.vertices[3] = {-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f};
    m_CubeVertices.vertices[4] = {-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f};
    m_CubeVertices.vertices[5] = { 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f};
    m_CubeVertices.vertices[6] = { 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f};
    m_CubeVertices.vertices[7] = {-0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f};

    m_LineVertices.vertices[0] = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    m_LineVertices.vertices[1] = { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f };

    m_VertexArray_Line = std::make_unique<VertexArray>(
        m_LineVertices.vertices[0].data(),
        sizeof(m_LineVertices),
        m_LineVertices.indices.data(),
        sizeof(m_LineVertices.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Rectangle = std::make_unique<VertexArray>(
    m_RectangleVertices.vertices[0].data(),
    sizeof(m_RectangleVertices),
    m_RectangleVertices.indices.data(),
    sizeof(m_RectangleVertices.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Triangle = std::make_unique<VertexArray>(
        m_TriangleVertices.vertices[0].data(),
        sizeof(m_TriangleVertices),
        m_TriangleVertices.indices.data(),
        sizeof(m_TriangleVertices.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Rectangle_XZ = std::make_unique<VertexArray>(
    m_RectangleVertices_XZ.vertices[0].data(),
    sizeof(m_RectangleVertices_XZ),
    m_RectangleVertices_XZ.indices.data(),
    sizeof(m_RectangleVertices_XZ.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Triangle_XZ = std::make_unique<VertexArray>(
        m_TriangleVertices_XZ.vertices[0].data(),
        sizeof(m_TriangleVertices_XZ),
        m_TriangleVertices_XZ.indices.data(),
        sizeof(m_TriangleVertices_XZ.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Rectangle_YZ = std::make_unique<VertexArray>(
        m_RectangleVertices_YZ.vertices[0].data(),
        sizeof(m_RectangleVertices_YZ),
        m_RectangleVertices_YZ.indices.data(),
        sizeof(m_RectangleVertices_YZ.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Triangle_YZ = std::make_unique<VertexArray>(
        m_TriangleVertices_YZ.vertices[0].data(),
        sizeof(m_TriangleVertices_YZ),
        m_TriangleVertices_YZ.indices.data(),
        sizeof(m_TriangleVertices_YZ.indices) / sizeof(unsigned int)
    );

    m_VertexArray_Cube = std::make_unique<VertexArray>(
        m_CubeVertices.vertices[0].data(),
        sizeof(m_CubeVertices),
        m_CubeVertices.indices.data(),
        sizeof(m_CubeVertices.indices) / sizeof(unsigned int)
    );

    m_Shader = std::make_unique<Shader>();
    std::filesystem::path path = std::filesystem::current_path();
    std::filesystem::path shader_path = path / "assets" / "Shader" / "FlatColor.glsl";
    m_Shader->LoadShader(shader_path.string().c_str());
    m_Shader->UnBind();

    m_Camera =  std::make_unique<Camera>(10.0f / 9.0f);

    return true;
}

void Renderer::OnKeyDown(uint32_t nChar, uint32_t nRepCnt)
{
	m_Camera->OnKeyDown(nChar, nRepCnt);
}

void Renderer::OnMouseWheel(int yOffset)
{
    m_Camera->OnMouseWheel(yOffset);
}

void Renderer::OnMouseMove(int xOffset, int yOffset)
{
    m_Camera->OnMouseMove(xOffset, yOffset);
}

void Renderer::DrawTriangle(
    DrawPlane plane,
    const glm::vec3& pos,
    const glm::vec3& rotate,
    const glm::vec3& scale, 
    const glm::vec4& color
)
{
    m_Shader->Bind();
    m_Shader->Set4f("u_Color", color);

    auto transform = 
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.x), {1,0,0} ) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.y), {0,1,0} ) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.z), {0,0,1} ) *
        glm::scale(glm::translate(glm::mat4(1.0f), pos), scale);
    m_Shader->SetMat4("u_Model", transform);
    m_Shader->SetMat4("u_View", m_Camera->GetView());
    m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());

    switch (plane)
    {
        case DrawPlane::XY:
            m_VertexArray_Triangle->Bind();
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            m_VertexArray_Triangle->UnBind();
            break;
        case DrawPlane::XZ:
            m_VertexArray_Triangle_XZ->Bind();
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            m_VertexArray_Triangle_XZ->UnBind();
            break;
        case DrawPlane::YZ:
            m_VertexArray_Triangle_YZ->Bind();
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            m_VertexArray_Triangle_YZ->UnBind();
            break;
    }

    m_Shader->UnBind();
}

void Renderer::DrawRectangle(
    DrawPlane plane,
    const glm::vec3& pos,
    const glm::vec3& rotate,
    const glm::vec3& scale,
    const glm::vec4& color
)
{
    m_Shader->Bind();
    m_Shader->Set4f("u_Color", color);

    auto transform =
        glm::scale(glm::translate(glm::mat4(1.0f), pos), scale) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.x), {1,0,0} ) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.y), {0,1,0} ) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.z), {0,0,1} ) ;
    m_Shader->SetMat4("u_Model", transform);
    m_Shader->SetMat4("u_View", m_Camera->GetView());
    m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());

    switch (plane)
    {
        case DrawPlane::XY:
            m_VertexArray_Rectangle->Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            m_VertexArray_Rectangle->UnBind();
            break;
        case DrawPlane::XZ:
            m_VertexArray_Rectangle_XZ->Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            m_VertexArray_Rectangle_XZ->UnBind();
            break;
        case DrawPlane::YZ:
            m_VertexArray_Rectangle_YZ->Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            m_VertexArray_Rectangle_YZ->UnBind();
            break;
    }

    m_Shader->UnBind();
}

void Renderer::DrawCube(
    const glm::vec3& pos,
    const glm::vec3& rotate,
    const glm::vec3& scale,
    const glm::vec4& color
)
{
    m_Shader->Bind();
    m_Shader->Set4f("u_Color", color);

    auto transform =
        glm::scale(glm::translate(glm::mat4(1.0f), pos), scale) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.x), {1,0,0} ) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.y), {0,1,0} ) *
        glm::rotate( glm::mat4(1.0f) , glm::radians(rotate.z), {0,0,1} ) ;
    m_Shader->SetMat4("u_Model", transform);
    m_Shader->SetMat4("u_View", m_Camera->GetView());
    m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());

    m_VertexArray_Cube->Bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_VertexArray_Cube->UnBind();

    m_Shader->UnBind();
}

void Renderer::DrawLine(
    const glm::vec3& start,
    const glm::vec3& end,
    const glm::vec4& color
)
{
    m_Shader->Bind();
    m_Shader->Set4f("u_Color", color);

    auto transform =
        glm::translate(glm::mat4(1.0f), start) *
        glm::scale(glm::mat4(1.0f), end - start);
    m_Shader->SetMat4("u_Model", transform);
    m_Shader->SetMat4("u_View", m_Camera->GetView());
    m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());

    m_VertexArray_Line->Bind();
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    m_VertexArray_Line->UnBind();

    m_Shader->UnBind();
}

// Call gladLoadGLUserPtr with a small retry loop and short sleep between attempts.
// Some drivers/platforms may need a tiny delay after making a context current.

static GLADapiproc glad_wgl_get_proc(void* userptr, const char* name) {
    return (GLADapiproc)wglGetProcAddress(name);
}
int Renderer::GladLoadWithRetry(int maxAttempts, int delayMs)
{
    for (int attempt = 1; attempt <= maxAttempts; ++attempt)
    {
        int ok = gladLoadGLUserPtr(glad_wgl_get_proc, NULL);

        if (ok)
            return ok;

        if (attempt < maxAttempts)
            Sleep(delayMs);
    }

    return 0;
}

_NAMESPACE_END