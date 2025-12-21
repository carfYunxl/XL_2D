#ifndef XL_INDEX_BUFFER_H_
#define XL_INDEX_BUFFER_H_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN

class IndexBuffer
{
public:
    IndexBuffer(unsigned int* buffer, unsigned int size);
    ~IndexBuffer();

    void Bind();
    void UnBind(); 
private:
    unsigned int    m_IBO{0};
    unsigned int*   m_Buffer{nullptr};
    unsigned int    m_Size{0};
};

_NAMESPACE_END
#endif //XL_INDEX_BUFFER_H_