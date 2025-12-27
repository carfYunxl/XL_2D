#ifndef XL_INDEX_BUFFER_H_
#define XL_INDEX_BUFFER_H_

#include "XL_Core.hpp"
#include <vector>

_NAMESPACE_BEGIN

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* buffer, unsigned int count);
    ~IndexBuffer();

    void Bind();
    void UnBind(); 
private:
    unsigned int    m_IBO{0};
    std::vector<unsigned int>   m_Buffer;
    unsigned int    m_Count{0};
};

_NAMESPACE_END
#endif //XL_INDEX_BUFFER_H_