#include "XL_VertexArray.hpp"

_NAMESPACE_BEGIN

BatchVertexArray::BatchVertexArray(VertexType type)
    : m_Type(type)
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
        indexData.push_back(base + 0);
        indexData.push_back(base + 1);
        indexData.push_back(base + 2);
        indexData.push_back(base + 2);
        indexData.push_back(base + 3);
        indexData.push_back(base + 0);
    }

    m_BatchIndexBuffer = std::make_unique<IndexBuffer>(indexData.data(), static_cast<unsigned int>(indexData.size()));
    m_BatchIndexBuffer->Bind();

    switch (type)
    {
    case VertexType::BatchQuadVertex:
        m_RenderVertexQuad = std::make_unique<RenderVertexQuad>();
        m_BatchVertexBuffer = std::make_unique<BatchVertexBuffer>(GetVertexSize(*m_RenderVertexQuad));
        m_BatchVertexBuffer->Bind();
        EnableVertexAttribPointer(*m_RenderVertexQuad);
        break;
    case VertexType::BatchCircleVertex:
        m_RenderVertexCircle = std::make_unique<RenderVertexCircle>();
        m_BatchVertexBuffer = std::make_unique<BatchVertexBuffer>(GetVertexSize(*m_RenderVertexCircle));
        m_BatchVertexBuffer->Bind();
        EnableVertexAttribPointer(*m_RenderVertexCircle);
        break;
    case VertexType::BatchLineVertex:
        m_RenderVertexLine = std::make_unique<RenderVertexLine>();
        m_BatchVertexBuffer = std::make_unique<BatchVertexBuffer>(GetVertexSize(*m_RenderVertexLine));
        m_BatchVertexBuffer->Bind();
        EnableVertexAttribPointer(*m_RenderVertexLine);
        break;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
BatchVertexArray::~BatchVertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}

void BatchVertexArray::Bind(const void* batch_data, uint32_t batch_data_size)
{
    glBindVertexArray(VAO);

    m_BatchVertexBuffer->Bind();
    if (batch_data_size > 0 && batch_data != nullptr)
    {
        size_t size = 0;
        switch (m_Type)
        {
        case VertexType::BatchQuadVertex:
            size = GetVertexSize(*m_RenderVertexQuad);
            break;
        case VertexType::BatchCircleVertex:
            size = GetVertexSize(*m_RenderVertexCircle);
            break;
        case VertexType::BatchLineVertex:
            size = GetVertexSize(*m_RenderVertexLine);
            break;
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)batch_data_size * size, batch_data);
    }
}

void BatchVertexArray::UnBind()
{
    glBindVertexArray(0);
}

_NAMESPACE_END