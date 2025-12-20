#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "XL_Core.hpp"

_NAMESPACE_BEGIN
class VertexBuffer
{
public:
    VertexBuffer(float* buffer, unsigned int size);
    ~VertexBuffer();

    void Bind();
    void UnBind();

    void Reset(float* buffer, unsigned int size);

private:
    unsigned int    m_VBO;
    float*          m_Buffer{nullptr};
    unsigned int    m_Size{0};
};
_NAMESPACE_END
#endif //VERTEX_BUFFER_H