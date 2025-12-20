#include "XL_IndexBuffer.hpp"

_NAMESPACE_BEGIN

IndexBuffer::IndexBuffer(unsigned int* buffer, unsigned int size)
    : m_Buffer(buffer), m_Size(size)
{
    glGenBuffers(1, &m_IBO);
}
IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_IBO);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, m_Buffer, GL_STATIC_DRAW );
}
void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

_NAMESPACE_END