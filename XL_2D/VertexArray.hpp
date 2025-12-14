#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

    class VertexBuffer;
    class IndexBuffer;

    class VertexArray
    {
    public:
        VertexArray(
            float* vertices = nullptr,
            unsigned int vertexSize = 0,
            unsigned int* indices = nullptr,
            unsigned int indexCount = 0
        );
        ~VertexArray();

        void Bind();
        void UnBind();

        void Reset(float* vertices, unsigned int vertexSize);
    private:
        void Init(
            float* vertices = nullptr,
            unsigned int vertexSize = 0,
            unsigned int* indices = nullptr,
            unsigned int indexCount = 0
        );
    private:
        unsigned int VAO;

        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
    };

_NAMESPACE_END

#endif //VERTEX_ARRAY_H_