#include "XL_Shader.hpp"

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

        Link(std::vector<unsigned int>{ vShader, fShader });
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Shader::LoadShader(const char* vertex_shader_source, const char* frag_shader_source)
{
    try
    {
        unsigned int vShader = Compile(vertex_shader_source, GL_VERTEX_SHADER);
        unsigned int fShader = Compile(frag_shader_source, GL_FRAGMENT_SHADER);

        Link(std::vector<unsigned int>{ vShader, fShader });
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

// 新增：从单一包含多个 "#type <stage>" 段的字符串编译多阶段着色器并链接
void Shader::LoadShaderStages(const char* combined_shader_source)
{
    try
    {
        auto stages = ParserShaderStagesFromString(combined_shader_source);

        std::vector<unsigned int> compiledShaders;
        for (auto& kv : stages)
        {
            const std::string& type = kv.first;
            const std::string& src = kv.second;
            if (type == "vertex")
                compiledShaders.push_back(Compile(src.c_str(), GL_VERTEX_SHADER));
            else if (type == "fragment")
                compiledShaders.push_back(Compile(src.c_str(), GL_FRAGMENT_SHADER));
            else if (type == "tess_control" || type == "tesscontrol" || type == "tess_control_shader")
                compiledShaders.push_back(Compile(src.c_str(), GL_TESS_CONTROL_SHADER));
            else if (type == "tess_evaluation" || type == "tessevaluation" || type == "tess_evaluation_shader")
                compiledShaders.push_back(Compile(src.c_str(), GL_TESS_EVALUATION_SHADER));
            else if (type == "geometry")
                compiledShaders.push_back(Compile(src.c_str(), GL_GEOMETRY_SHADER));
            // 忽略未知类型
        }

        Link(compiledShaders);
    }
    catch (const std::exception& e)
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

void Shader::Link(const std::vector<unsigned int>& shaders)
{
    try{
        m_nProgramID = glCreateProgram();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    for (auto s : shaders)
        glAttachShader(m_nProgramID, s);

    glLinkProgram(m_nProgramID);

    int success;
    char infoLog[1024];
    glGetProgramiv(m_nProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_nProgramID, 1024, nullptr, infoLog);
        std::cout << "[ERROR] Link Program with : " << infoLog << std::endl;
    }

    for (auto s : shaders)
        glDeleteShader(s);
}

unsigned int Shader::Compile(const char* shader, unsigned int type)
{
    unsigned int nShader = glCreateShader(type);
    glShaderSource(nShader, 1, &shader, nullptr);
    glCompileShader(nShader);

    int success;
    char infoLog[1024];
    glGetShaderiv(nShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(nShader, 1024, nullptr, infoLog);
        std::cout << "[ERROR] Compile shader type " << type << " with : " << infoLog << std::endl;
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

// 新增：从传入的字符串解析多个 #type 段，返回 map<type, source>
std::unordered_map<std::string, std::string> Shader::ParserShaderStagesFromString(const char* source)
{
    std::unordered_map<std::string, std::string> result;
    std::istringstream iss(source);
    std::string line;
    std::string currentType;
    std::ostringstream ss;

    auto flush = [&]() {
        if (!currentType.empty())
        {
            result[currentType] = ss.str();
            ss.str(""); ss.clear();
        }
    };

    while (std::getline(iss, line))
    {
        // trim left spaces
        std::string tmp = line;
        // find #type
        size_t p = tmp.find("#type");
        if (p != std::string::npos)
        {
            // flush previous
            flush();

            // parse type token after #type
            size_t pos = p + 5;
            while (pos < tmp.size() && isspace((unsigned char)tmp[pos])) pos++;
            std::string t;
            while (pos < tmp.size() && !isspace((unsigned char)tmp[pos])) { t += tmp[pos]; pos++; }
            // normalize to lower
            for (auto &c : t) c = (char)tolower((unsigned char)c);
            currentType = t;
            continue;
        }

        if (!currentType.empty())
        {
            ss << line << "\n";
        }
    }
    flush();
    return result;
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

_NAMESPACE_END