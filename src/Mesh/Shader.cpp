#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

#include "Shader.h"

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
    bool zwrite;
    bool cull;
};

Shader::Shader() { }

Shader::Shader(const std::string& filepath)
{
    ShaderProgramSource src = ParseShader(filepath);
    id = CreateShader(src.vertexSource, src.fragmentSource);
    zwriteon = src.zwrite;
    cullon = src.cull;
}

Shader::Shader(const Shader& obj)
{
    this->id = obj.id;
    this->zwriteon = obj.zwriteon;
    this->cullon = obj.cullon;
}

Shader::Shader(Shader&& obj) noexcept
{
    this->id = obj.id;
    obj.id = -1;
    this->zwriteon = obj.zwriteon;
    obj.zwriteon = true;
    this->cullon = obj.cullon;
    obj.cullon = true;
}

Shader::~Shader()
{
    if (id < 150000)
        glDeleteProgram(id);
}

void Shader::SetPath(const std::string& filepath)
{
	ShaderProgramSource src = ParseShader(filepath);
	id = CreateShader(src.vertexSource, src.fragmentSource);
}

Shader& Shader::operator=(const Shader& other)
{
    this->id = other.id;
    this->zwriteon = other.zwriteon;
    this->cullon = other.cullon;
    return *this;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    this->id = other.id;
    other.id = -1;
    this->zwriteon = other.zwriteon;
    other.zwriteon = true;
    this->cullon = other.cullon;
    other.cullon = true;
    return *this;
}

void Shader::Bind() const
{
    // zwrite
    if (zwriteon)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    // cull
    if (cullon)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    glUseProgram(id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

/*
shader #subscripts:
#shader [type] - type is vertex or fragment, denotes the shader section
#zwrite [off/on] - whether to write to the zbuffer
#cull [off/on] - whether to cull
*/

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

    bool zwrite = true;
    bool cull = true;

    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        // for zwrite, cull, etc
        bool option = false;
        if (line.find("on") != std::string::npos)
            option = true;

        // shader type
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }

        // zwrite on
        else if (line.find("#zwrite") != std::string::npos)
            zwrite = option; 
        // cull on
        else if (line.find("#cull") != std::string::npos)
            cull = option;

        else if (type != ShaderType::NONE)
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str(), zwrite, cull };
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // handle errors D:
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}