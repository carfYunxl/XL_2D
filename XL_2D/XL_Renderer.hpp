#ifndef XL_BATCH_RENDERER_H_
#define XL_BATCH_RENDERER_H_
#include "XL_Core.hpp"
#include "XL_Camera.hpp"
#include "XL_Shader.hpp"
#include "XL_Data.hpp"
#include "XL_VertexArray.hpp"
#include "XL_VertexBuffer.hpp"

_NAMESPACE_BEGIN

struct TransFormData
{
    glm::vec3 position;
    glm::vec3 rotate;
    glm::vec3 scale;
};

class BatchRenderer
{
    using GLAD_PROC = void(*)();
    using pfnGladLoader = GLAD_PROC(*)(void* userptr, const char* name);
    static constexpr size_t MaxBatchVertices = 65536;
public:
    BatchRenderer();
    ~BatchRenderer() {}

    void Resize(int nWidth, int nHeight);
    void UpdateCamera();
    void ClearScene() { glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    bool Init(pfnGladLoader loader);

    void OnKeyDown(uint32_t nChar, uint32_t nRepCnt);
    void OnMouseWheel(int yOffset);
    void OnMouseMove(int xOffset, int yOffset);

    void DrawTriangle( DrawPlane plane, const TransFormData& trans, const glm::vec4& color );
    void DrawRectangle( DrawPlane plane, const TransFormData& trans, const glm::vec4& color );
    void DrawCube( const TransFormData& trans, const glm::vec4& color );

    void DrawLine(
        const glm::vec3& start,
        const glm::vec3& end,
        const glm::vec4& color
    );

    void DrawCircle(
        DrawPlane plane,
        const glm::vec3& pos,
        const glm::vec3& rotate,
        float radius,
        const glm::vec4& color,
        bool filled = true,
        int segments = 36
    );

    void DrawCircle_Pixel(
        DrawPlane plane,
        const glm::vec3& pos,
        const glm::vec3& rotate,
        float radius_pixels,
        const glm::vec4& color,
        bool filled = true,
        int segments = 36
    );

    void Flush();
private:
    int  GladLoadWithRetry(pfnGladLoader loader, int maxAttempts, int delayMs);
    glm::vec3 TransformPos(const glm::vec3& in, const glm::mat4& model);
private:
    std::unique_ptr<Shader>             m_Shader;
    std::unique_ptr<Camera>             m_Camera;
    int                                 mViewportWidth;
    int                                 mViewportHeight;
    std::unique_ptr<BatchVertexArray>   m_TriangleBatchVertex;
    std::unique_ptr<BatchVertexArray>   m_LineBatchVertex;
    BatchRenderVertex                   m_TriangleBatchVertices[MaxBatchVertices];
    BatchRenderVertex                   m_LineBatchVertices[MaxBatchVertices];

	uint32_t							m_TriangleVertexCount = 0;
    uint32_t                            m_LineVertexCount = 0;
};

_NAMESPACE_END
#endif //XL_BATCH_RENDERER_H_