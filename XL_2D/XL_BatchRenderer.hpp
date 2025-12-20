#ifndef __BATCH_RENDERER_H__
#define __BATCH_RENDERER_H__
#include "XL_Core.hpp"
#include "XL_Camera.hpp"
#include "XL_Shader.hpp"
#include "XL_Data.hpp"
#include "XL_BatchVertexArray.hpp"
#include "XL_BatchVertexBuffer.hpp"

_NAMESPACE_BEGIN

class BatchRenderer
{
    using GLAD_PROC = void(*)();
    using pfnGladLoader = GLAD_PROC(*)(void* userptr, const char* name);
private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Camera> m_Camera;
private:
    // for batch render
    std::unique_ptr<BatchVertexArray> m_TriangleBatchVertex;
    std::unique_ptr<BatchVertexArray> m_LineBatchVertex;

    std::vector<BatchRenderVertex> m_TriangleBatchVertices;
    std::vector<BatchRenderVertex> m_LineBatchVertices;

    static constexpr size_t MaxBatchVertices = 65536; // ¿Éµ÷
public:
    void Resize(int nWidth, int nHeight);
    void UpdateCamera();
    void ClearScene() { glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    bool Init(pfnGladLoader loader);

    void OnKeyDown(uint32_t nChar, uint32_t nRepCnt);
    void OnMouseWheel(int yOffset);
    void OnMouseMove(int xOffset, int yOffset);

    // for batch render
    void Flush();
public:
    void DrawTriangle(
        DrawPlane plane,
        const glm::vec3& pos,
        const glm::vec3& rotate,
        const glm::vec3& scale,
        const glm::vec4& color
    );

    void DrawRectangle(
        DrawPlane plane,
        const glm::vec3& pos,
        const glm::vec3& rotate,
        const glm::vec3& scale,
        const glm::vec4& color
    );

    void DrawCube(
        const glm::vec3& pos,
        const glm::vec3& rotate,
        const glm::vec3& scale,
        const glm::vec4& color
    );

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

private:
    int  GladLoadWithRetry(pfnGladLoader loader, int maxAttempts, int delayMs);
};

_NAMESPACE_END
#endif //__BATCH_RENDERER_H__