#include "Shader.hpp"

_NAMESPACE_BEGIN

Shader::Shader()
{ }

Shader::~Shader()
{
    glDeleteProgram(m_nProgramID);
}

void Shader::LoadShader(const char* shader_source)
{
    try
    {
        auto [x, y] = ParserShaderFile(shader_source);

        unsigned int vShader = Compile(x.c_str(),GL_VERTEX_SHADER);
        unsigned int fShader = Compile(y.c_str(),GL_FRAGMENT_SHADER);

        Link(vShader, fShader);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Shader::Bind()
{
    glUseProgram(m_nProgramID);
}

void Shader::UnBind()
{
    glUseProgram(0);
}

void Shader::Link(unsigned int vShader, unsigned int fShader)
{
    try{
        m_nProgramID = glCreateProgram();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    glAttachShader(m_nProgramID, vShader);
    glAttachShader(m_nProgramID, fShader);
    glLinkProgram(m_nProgramID);

    int success;
    char infoLog[512];
    glGetProgramiv(m_nProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_nProgramID, 512, nullptr, infoLog);
        std::cout << "[ERROR] Link Program with : " << infoLog << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

unsigned int Shader::Compile(const char* shader, unsigned int type)
{
    unsigned int nShader = glCreateShader(type);
    glShaderSource(nShader, 1, &shader, nullptr);
    glCompileShader(nShader);

    int success;
    char infoLog[512];
    glGetShaderiv(nShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(nShader, 512, nullptr, infoLog);
        std::cout << "[ERROR] Compile" <<(type == GL_VERTEX_SHADER ? "VertexShader" : "FragmentShader") << " with : " << infoLog << std::endl;
    }

    return nShader;
}

std::pair<std::string, std::string> Shader::ParserShaderFile(const char* shader_source)
{
    std::ifstream ifs(shader_source);
    if(!ifs)
    {
        throw std::runtime_error("open shader file failed!");
    }

    std::string sVertex, sFragment;
    std::string sRead;
    ShaderType type = ShaderType::NONE;
    while(getline(ifs, sRead))
    {
        if(sRead.find("#type") != std::string::npos)
        {
            if(sRead.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(sRead.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }

            continue;
        }

        switch(type)
        {
            case ShaderType::VERTEX:
            {
                sVertex.append("\n");
                sVertex.append(sRead);
                break;
            }
            case ShaderType::FRAGMENT:
            {
                sFragment.append("\n");
                sFragment.append(sRead);
                break;
            }
        }
    }

    return {sVertex, sFragment};
}

void Shader::Set4f(const char* name, const glm::vec4& color)
{
    int nLoc = glGetUniformLocation(m_nProgramID, name);
    glUniform4f(nLoc, color.x, color.y, color.z, color.w);
}

void Shader::Set3f(const char* name, const glm::vec3& color)
{
    int nLoc = glGetUniformLocation(m_nProgramID, name);
    glUniform3f(nLoc, color.x, color.y, color.z);
}

void Shader::Set2f(const char* name, const glm::vec2& pos)
{
    int nLoc = glGetUniformLocation(m_nProgramID, name);
    glUniform2f(nLoc, pos.x, pos.y);
}

void Shader::SetFloat(const char* name, float val)
{
    int nLoc = glGetUniformLocation(m_nProgramID, name);
    glUniform1f(nLoc, val);
}

void Shader::SetInt(const char* name, unsigned int index)
{
    int nLoc = glGetUniformLocation(m_nProgramID, name);
    glUniform1i(nLoc, index);
}

void Shader::SetMat4(const char* name, const glm::mat4& trans)
{
    int nLoc = glGetUniformLocation(m_nProgramID, name);
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

// void Shader::SetUniformAttr(const Attribute& data)
// {
//     shader.Set3f( "u_ObjColor",     cubeAttr.m_Color);
//     shader.Set3f( "u_LightPos",     LightAttr.m_Pos);
//     shader.Set3f( "u_ViewPos",      g_Camera.GetCameraPos());
//     shader.Set3f( "u_LightColor",   g_SceneData.m_LightColor);
// }

_NAMESPACE_END