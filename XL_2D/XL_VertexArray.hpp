#ifndef XL_BACTH_VERTEX_ARRAY_H_
#define XL_BACTH_VERTEX_ARRAY_H_

#include "XL_Core.hpp"
#include "XL_VertexBuffer.hpp"
#include "XL_IndexBuffer.hpp"

_NAMESPACE_BEGIN

struct RenderVV
{
    glm::vec3 position;
    glm::vec4 color;
};

struct RenderVertexBase
{   
	virtual ~RenderVertexBase() = default;

	virtual size_t GetVertexSize() const = 0;
	virtual void EnableVertexAttribPointer() const = 0;
};

struct RenderVertexQuad : public RenderVertexBase
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 local; // 新增：局部坐标（用于象限/细分判断）
	float tessLevel; // 新增：细分等级

    virtual size_t GetVertexSize() const override {
        return sizeof(RenderVertexQuad);
    }

    virtual void EnableVertexAttribPointer() const override
    {
		unsigned int offset = 0;
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, int(GetVertexSize()), (void*)offset);
        glEnableVertexAttribArray(0);

		offset += sizeof(float);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, int(GetVertexSize()), (void*)offset);
        glEnableVertexAttribArray(1);

        offset += sizeof(glm::vec3);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, int(GetVertexSize()), (void*)offset);
        glEnableVertexAttribArray(2);

        offset += sizeof(glm::vec4);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, int(GetVertexSize()), (void*)offset);
        glEnableVertexAttribArray(3);

        offset += sizeof(glm::vec2);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, int(GetVertexSize()), (void*)offset);
        glEnableVertexAttribArray(4);
	}
};

struct RenderVertexCircle : public RenderVertexBase
{
    glm::vec3 worldPosition;
    glm::vec3 localPosition;
    glm::vec4 color;
    float thickness;
    float fade;
    virtual size_t GetVertexSize() const override {
        return sizeof(RenderVertexCircle);
    }

    virtual void EnableVertexAttribPointer() const override {
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(0 + sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(sizeof(glm::vec3) + sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(sizeof(glm::vec3) * 2 + sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(sizeof(glm::vec3) * 2 + sizeof(glm::vec4) + sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(sizeof(glm::vec3) * 2 + sizeof(glm::vec4) + sizeof(float) + sizeof(float)));
        glEnableVertexAttribArray(5);
    }
};

struct RenderVertexLine : public RenderVertexBase
{
    glm::vec3 position;
    glm::vec4 color;

    virtual size_t GetVertexSize() const override {
        return sizeof(RenderVertexLine);
    }

    virtual void EnableVertexAttribPointer() const override {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(0 + sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(sizeof(glm::vec3) + sizeof(float)));
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
	std::unique_ptr<RenderVertexBase>  m_RenderVertex;
};

_NAMESPACE_END

#endif //XL_BACTH_VERTEX_ARRAY_H_