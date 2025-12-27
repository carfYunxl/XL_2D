#include "XL_IndexBuffer.hpp"

_NAMESPACE_BEGIN

IndexBuffer::IndexBuffer(const unsigned int* buffer, unsigned int count)
    : m_Buffer(), m_Count(count)
{
    if (buffer && count > 0)
        m_Buffer.assign(buffer, buffer + count);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    if (!m_Buffer.empty())
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Buffer.size() * sizeof(unsigned int), m_Buffer.data(), GL_STATIC_DRAW);
    else
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
IndexBuffer::~IndexBuffer()
{
    if (m_IBO)
        glDeleteBuffers(1, &m_IBO);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}
void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

_NAMESPACE_END