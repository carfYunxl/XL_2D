#ifndef BACTH_VERTEX_ARRAY_H_
#define BACTH_VERTEX_ARRAY_H_

#include "XL_Core.hpp"
#include "XL_VertexBuffer.hpp"

_NAMESPACE_BEGIN

class BatchVertexArray
{
public:
    BatchVertexArray();
    ~BatchVertexArray();

    void Bind(const std::vector<BatchRenderVertex>& batch_data);
    void UnBind();
private:
    unsigned int VAO;

	std::unique_ptr<BatchVertexBuffer> m_BatchVertexBuffer;
};

_NAMESPACE_END

#endif //BACTH_VERTEX_ARRAY_H_