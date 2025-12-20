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

void BatchVertexArray::Bind(const std::vector<BatchRenderVertex>& batch_data)
{
    glBindVertexArray(VAO);
    m_BatchVertexBuffer->Bind();
    //glBufferData(GL_ARRAY_BUFFER, batch_data.size() * sizeof(BatchRenderVertex), batch_data.data(), GL_DYNAMIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, batch_data.size() * sizeof(BatchRenderVertex), (float*)batch_data.data());
    glBufferSubData(GL_ARRAY_BUFFER, 0, batch_data.size() * sizeof(BatchRenderVertex), static_cast<const void*>(batch_data.data()));
}

void BatchVertexArray::UnBind()
{
    glBindVertexArray(0);
}

_NAMESPACE_END