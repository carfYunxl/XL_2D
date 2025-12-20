#include "XL_BatchVertexBuffer.hpp"

_NAMESPACE_BEGIN

BatchVertexBuffer::BatchVertexBuffer()
{
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 65536 * sizeof(BatchRenderVertex), nullptr, GL_DYNAMIC_DRAW);
}

BatchVertexBuffer::~BatchVertexBuffer()
{
    glDeleteBuffers(1, &m_VBO);
}

void BatchVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    //glBufferData(GL_ARRAY_BUFFER, 65536 * sizeof(BatchRenderVertex), nullptr, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, 65536 * sizeof(BatchRenderVertex), nullptr, GL_STATIC_DRAW);
}

void BatchVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

_NAMESPACE_END