#include "VertexBuffer.hpp"

_NAMESPACE_BEGIN

    VertexBuffer::VertexBuffer(float* buffer, unsigned int size)
        : m_Buffer(buffer), m_Size(size)
    {
        glGenBuffers(1, &m_VBO);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_VBO);
    }

    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Size, m_Buffer, GL_STATIC_DRAW);
    }

    void VertexBuffer::UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::Reset(float* buffer, unsigned int size)
    {
        m_Size = size;
        m_Buffer = buffer;
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Size, m_Buffer, GL_STATIC_DRAW);
    }

_NAMESPACE_END