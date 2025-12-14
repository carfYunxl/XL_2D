#ifndef __RENDERER_H__
#define __RENDERER_H__
#include "XL_Core.hpp"
#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

_NAMESPACE_BEGIN

    struct TriangleVertex
    {
        std::array<std::array<float, 6>, 3> vertices;
        std::array<unsigned int, 3> indices{
            0, 1, 2
        };
    };

    struct RectangleVertex
    {
        std::array<std::array<float, 6>, 4> vertices;
        std::array<unsigned int, 6> indices{
            0, 1, 2, 2, 3, 0
        };
    };

    struct CubeVertex
    {
        std::array<std::array<float, 6>, 8> vertices;
        std::array<unsigned int, 36> indices{
            0,1,2, 2,3,0,
            4,5,6, 6,7,4,
            0,1,5, 5,4,0,
            2,3,7, 7,6,2,
            0,3,7, 7,4,0,
            1,2,6, 6,5,1
        };
    };

    struct LineVertex
    {
        std::array<std::array<float, 6>, 2> vertices;
        std::array<unsigned int, 2> indices{
            0, 1
        };
    };

    enum class DrawPlane
    {
        XY = 0,
        XZ = 1,
        YZ = 2
    };

    class Renderer
    {
    private:
        // XY Plane 
        TriangleVertex  m_TriangleVertices;
        RectangleVertex m_RectangleVertices;

        // XZ Plane
        TriangleVertex  m_TriangleVertices_XZ;
        RectangleVertex m_RectangleVertices_XZ;

        // YZ Plane
        TriangleVertex  m_TriangleVertices_YZ;
        RectangleVertex m_RectangleVertices_YZ;

        CubeVertex      m_CubeVertices;

        LineVertex     m_LineVertices;

        std::unique_ptr<VertexArray> m_VertexArray_Rectangle;
        std::unique_ptr<VertexArray> m_VertexArray_Triangle;

        std::unique_ptr<VertexArray> m_VertexArray_Rectangle_XZ;
        std::unique_ptr<VertexArray> m_VertexArray_Triangle_XZ;

        std::unique_ptr<VertexArray> m_VertexArray_Rectangle_YZ;
        std::unique_ptr<VertexArray> m_VertexArray_Triangle_YZ;
        std::unique_ptr<VertexArray> m_VertexArray_Cube;

        std::unique_ptr<VertexArray> m_VertexArray_Line;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Camera> m_Camera;
    public:
        void Resize(int nWidth, int nHeight);
        void UpdateCamera();
        void ClearScene() { glClearColor(0.2f, 0.3f, 0.3f, 1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
        bool Init();

        void OnKeyDown(uint32_t nChar, uint32_t nRepCnt);
        void OnMouseWheel(int yOffset);
        void OnMouseMove(int xOffset, int yOffset);
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
    private:
        int  GladLoadWithRetry(int maxAttempts, int delayMs);
    };
_NAMESPACE_END
#endif //__RENDERER_H__