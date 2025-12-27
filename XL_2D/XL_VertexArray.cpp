#include "XL_VertexArray.hpp"

_NAMESPACE_BEGIN

BatchVertexArray::BatchVertexArray()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    const size_t quadCount = MaxBatchVertices / 4; // 每个 quad 使用 4 个顶点
    const size_t indexCount = quadCount * 6;
    std::vector<unsigned int> indexData;
    indexData.reserve(indexCount);

    for (size_t q = 0; q < quadCount; ++q)
    {
        unsigned int base = static_cast<unsigned int>(q * 4);
        // two triangles per quad: (0,1,2) (2,3,0) with base offset
        indexData.push_back(base + 0);
        indexData.push_back(base + 1);
        indexData.push_back(base + 2);
        indexData.push_back(base + 2);
        indexData.push_back(base + 3);
        indexData.push_back(base + 0);
    }

    m_BatchIndexBuffer = std::make_unique<IndexBuffer>(indexData.data(), static_cast<unsigned int>(indexData.size()));
    m_BatchIndexBuffer->Bind();

    m_BatchVertexBuffer = std::make_unique<BatchVertexBuffer>();
    m_BatchVertexBuffer->Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BatchRenderVertex), (void*)offsetof(BatchRenderVertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(BatchRenderVertex), (void*)offsetof(BatchRenderVertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
BatchVertexArray::~BatchVertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}

void BatchVertexArray::Bind(const BatchRenderVertex * const batch_data, uint32_t batch_data_size)
{
    glBindVertexArray(VAO);

    m_BatchVertexBuffer->Bind();
    if (batch_data_size > 0 && batch_data != nullptr)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)batch_data_size * sizeof(BatchRenderVertex), static_cast<const void*>(batch_data));
    }
}

void BatchVertexArray::UnBind()
{
    glBindVertexArray(0);
}

_NAMESPACE_END