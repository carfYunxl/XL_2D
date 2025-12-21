#ifndef XL_BACTH_VERTEX_ARRAY_H_
#define XL_BACTH_VERTEX_ARRAY_H_

#include "XL_Core.hpp"
#include "XL_VertexBuffer.hpp"

_NAMESPACE_BEGIN

class BatchVertexArray
{
public:
    BatchVertexArray();
    ~BatchVertexArray();

    void Bind(const BatchRenderVertex* const batch_data, uint32_t batch_data_size);
    void UnBind();
private:
    unsigned int VAO;

	std::unique_ptr<BatchVertexBuffer> m_BatchVertexBuffer;
};

_NAMESPACE_END

#endif //XL_BACTH_VERTEX_ARRAY_H_