#ifndef XL_BATCH_RENDERER_H_
#define XL_BATCH_RENDERER_H_
#include "XL_Core.hpp"
#include "XL_Camera.hpp"
#include "XL_Shader.hpp"
#include "XL_Data.hpp"
#include "XL_VertexArray.hpp"
#include "XL_VertexBuffer.hpp"
#include "XL_2D.hpp"

_NAMESPACE_BEGIN

struct BatchBase
{
    std::unique_ptr<Shader>             m_Shader;
    std::unique_ptr<BatchVertexArray>   m_Vertex;
    uint32_t							m_VertexCount{ 0 };
};

struct QuadBatch : BatchBase
{
    RenderVertexQuad                    m_Vertices[MaxBatchVertices];
};

struct CircleBatch : BatchBase
{
    RenderVertexCircle                  m_Vertices[MaxBatchVertices];
};

struct LineBatch : BatchBase
{
    RenderVertexLine                    m_Vertices[MaxBatchVertices];
};

class BatchRenderer
{
    using GLAD_PROC = void(*)();
    using pfnGladLoader = GLAD_PROC(*)(void* userptr, const char* name);
public:
    BatchRenderer();
    ~BatchRenderer() {}

    void Resize(int nWidth, int nHeight);
    void UpdateCamera();
    void ClearScene() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    bool Init(pfnGladLoader loader);

    void OnKeyDown(uint32_t nChar, uint32_t nRepCnt);
    void OnMouseWheel(int yOffset);
    void OnMouseMove(int xOffset, int yOffset);
    void OnMouseHover(int xOffset, int yOffset);

    void DrawTriangle( DrawPlane plane, float x0, float y0, float x1, float y1, float x2, float y2, const glm::vec4& color );
    void DrawRectangle(
        DrawPlane plane, 
        const XL_RectF& rect, 
        float z_near, 
        const glm::vec4& color, 
        float tess_level,
        const glm::vec2& thickness,
        const glm::ivec2& select_cell
    );
    void DrawCube( const glm::vec4& color );

    void DrawLine(
        const glm::vec3& start,
        const glm::vec3& end,
        const glm::vec4& color, 
		float line_width /*= 1.0f*/
    );

    void DrawCircle(
        DrawPlane plane,
        float x,
        float y,
        float z_near,
        float radiusX,
        float radiusY,
        const glm::vec4& color, 
        float thickness /*= 1.0f*/, 
        float fade /*= 0.005f*/ );

    void Flush();
	void ResetDrawCall() { m_DrawCall = -1; }   
private:
    int  GladLoadWithRetry(pfnGladLoader loader, int maxAttempts, int delayMs);
	glm::vec2 ScreenToWorld(float x, float y); // screen pixels -> opengl coords
private:
    std::unique_ptr<Camera>   m_Camera;
    int                       mViewportWidth;
    int                       mViewportHeight;

	QuadBatch                 m_QuadBatch;
	CircleBatch			      m_CircleBatch;
	LineBatch                 m_LineBatch;

	int                       m_DrawCall = -1;
	float                     z_near = 0.0f;
};

_NAMESPACE_END
#endif //XL_BATCH_RENDERER_H_