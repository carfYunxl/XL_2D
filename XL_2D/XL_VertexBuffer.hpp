#ifndef XL_BATCH_VERTEX_BUFFER_H_
#define XL_BATCH_VERTEX_BUFFER_H_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN
class BatchVertexBuffer
{
public:
    BatchVertexBuffer();
    ~BatchVertexBuffer();

    void Bind();
    void UnBind();

private:
    unsigned int    m_VBO;
};
_NAMESPACE_END

#endif//XL_BATCH_VERTEX_BUFFER_H_