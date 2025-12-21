#include "XL_VertexArray.hpp"

_NAMESPACE_BEGIN

BatchVertexArray::BatchVertexArray()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	m_BatchVertexBuffer = std::make_unique<BatchVertexBuffer>();
	m_BatchVertexBuffer->Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BatchRenderVertex), (void*)offsetof(BatchRenderVertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(BatchRenderVertex), (void*)offsetof(BatchRenderVertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
BatchVertexArray::~BatchVertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}

void BatchVertexArray::Bind(const BatchRenderVertex * const batch_data, uint32_t batch_data_size)
{
    glBindVertexArray(VAO);
    m_BatchVertexBuffer->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch_data_size * sizeof(BatchRenderVertex), static_cast<const void*>(batch_data));
}

void BatchVertexArray::UnBind()
{
    glBindVertexArray(0);
}

_NAMESPACE_END