#include "XL_VertexBuffer.hpp"

_NAMESPACE_BEGIN

BatchVertexBuffer::BatchVertexBuffer(uint32_t size)
{
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxBatchVertices * size, nullptr, GL_DYNAMIC_DRAW);
}

BatchVertexBuffer::~BatchVertexBuffer()
{
    glDeleteBuffers(1, &m_VBO);
}

void BatchVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void BatchVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

_NAMESPACE_END