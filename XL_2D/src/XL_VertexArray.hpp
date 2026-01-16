#ifndef XL_BACTH_VERTEX_ARRAY_H_
#define XL_BACTH_VERTEX_ARRAY_H_

#include "XL_Core.hpp"
#include "XL_VertexBuffer.hpp"
#include "XL_IndexBuffer.hpp"

_NAMESPACE_BEGIN

struct RenderVertexQuad
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 local; // 新增：局部坐标（用于象限/细分判断）
    float tessLevel; // 新增：细分等级
    glm::vec2 thickness;//边框宽度
    glm::ivec2 select_cell;//当前选择的子单元格

    size_t GetVertexSize() const{
        return sizeof(RenderVertexQuad);
    }

    void EnableVertexAttribPointer() const
    {
        int size = GetVertexSize();

		unsigned int offset = 0;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(0);

        offset += sizeof(glm::vec3);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(1);

        offset += sizeof(glm::vec4);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(2);

        offset += sizeof(glm::vec2);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(3);

        offset += sizeof(float);
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(4);

        offset += sizeof(glm::vec2);
        glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, size, (void*)offset);
        glEnableVertexAttribArray(5);
	}
};

struct RenderVertexCircle
{
    glm::vec3 worldPosition;
    glm::vec3 localPosition;
    glm::vec4 color;
    float thickness;
    float fade;
    size_t GetVertexSize() const{
        return sizeof(RenderVertexCircle);
    }

    void EnableVertexAttribPointer() const
    {
        unsigned int offset = 0;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)offset);
        glEnableVertexAttribArray(0);

        offset += sizeof(glm::vec3);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)offset);
        glEnableVertexAttribArray(1);

        offset += sizeof(glm::vec3);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)offset);
        glEnableVertexAttribArray(2);

        offset += sizeof(glm::vec4);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)offset);
        glEnableVertexAttribArray(3);

        offset += sizeof(float);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)offset);
        glEnableVertexAttribArray(4);
    }
};

struct RenderVertexLine
{
    glm::vec3 position;
    glm::vec4 color;

    size_t GetVertexSize() const{
        return sizeof(RenderVertexLine);
    }

    void EnableVertexAttribPointer() const
    {
        unsigned int offset = 0;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(offset));
        glEnableVertexAttribArray(0);

        offset += sizeof(glm::vec3);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(offset));
        glEnableVertexAttribArray(1);
    }
};

class BatchVertexArray
{
public:
    BatchVertexArray(VertexType type);
    ~BatchVertexArray();

    void Bind(const void* batch_data, uint32_t batch_data_size);
    void UnBind();
private:
    unsigned int    VAO;
    VertexType      m_Type;

    std::unique_ptr<IndexBuffer>       m_BatchIndexBuffer;
	std::unique_ptr<BatchVertexBuffer> m_BatchVertexBuffer;

	std::unique_ptr<RenderVertexQuad>  m_RenderVertexQuad;
	std::unique_ptr<RenderVertexCircle>  m_RenderVertexCircle;
	std::unique_ptr<RenderVertexLine>  m_RenderVertexLine;
};

_NAMESPACE_END

#endif //XL_BACTH_VERTEX_ARRAY_H_