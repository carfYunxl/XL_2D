#ifndef SHADER_H_
#define SHADER_H_

#include "XL_Core.hpp"

_NAMESPACE_BEGIN
class Shader
{
public:
    enum class ShaderType
    {
        NONE = 0,
        VERTEX,
        FRAGMENT
    };

    Shader();
    ~Shader();

    void LoadShader(const char* shader_source);
    void LoadShader(const char* vertex_shader_source, const char* frag_shader_source);

    void Bind();
    void UnBind();
        
    unsigned int const GetProgram() const {return m_nProgramID;}

    void Set4f(const char* name, const glm::vec4& color);
    void Set3f(const char* name, const glm::vec3& color);
    void Set2f(const char* name, const glm::vec2& pos);
    void SetInt(const char* name, unsigned int index);
    void SetFloat(const char* name, float val);
    void SetMat4(const char* name, const glm::mat4& trans);

private:
    unsigned int    Compile(const char* shader, unsigned int type);
    void            Link(unsigned int vShader, unsigned int fShader);

    std::pair<std::string, std::string> ParserShaderFile(const char* shader_source);
private:
    unsigned int    m_nProgramID;
};
_NAMESPACE_END


#endif //SHADER_H_