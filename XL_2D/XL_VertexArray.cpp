#include "XL_VertexArray.hpp"
#include "XL_VertexBuffer.hpp"
#include "XL_IndexBuffer.hpp"

_NAMESPACE_BEGIN

VertexArray::VertexArray(
    float* vertices,
    unsigned int vertexSize,
    unsigned int* indices,
    unsigned int indexCount
)
{
    Init(vertices, vertexSize, indices, indexCount);
}
VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}

void VertexArray::Bind()
{
    glBindVertexArray(VAO);
}

void VertexArray::UnBind()
{
    glBindVertexArray(0);
}

void VertexArray::Init(
    float* vertices,
    unsigned int vertexSize,
    unsigned int* indices,
    unsigned int indexCount
)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, vertexSize);
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, indexCount * sizeof(unsigned int));

	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	m_VertexBuffer->UnBind();
    glBindVertexArray(0);
}

void VertexArray::Reset(float* vertices, unsigned int vertexSize)
{
    m_VertexBuffer->Reset(vertices, vertexSize);
}

_NAMESPACE_END