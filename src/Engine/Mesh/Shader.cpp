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
    bool blend;
    Shader::BlendType srcblendtype{ Shader::BlendType::SrcAlpha };
    Shader::BlendType destblendtype{ Shader::BlendType::OneMinusSrcAlpha };

    bool autoSetProjMatrix;
};

Shader::Shader() { }

Shader::Shader(const std::string& filepath)
{
    ShaderProgramSource src = ParseShader(filepath);
    id = CreateShader(src.vertexSource, src.fragmentSource);
    zwriteon = src.zwrite;
    cullon = src.cull;
    blendon = src.blend;
    srcblend = src.srcblendtype;
    destblend = src.destblendtype;
    autoSetProjMatrix = src.autoSetProjMatrix;
}

Shader::Shader(const Shader& obj)
{
    this->id = obj.id;
    this->zwriteon = obj.zwriteon;
    this->cullon = obj.cullon;
    this->blendon = obj.blendon;
    this->srcblend = obj.srcblend;
    this->destblend = obj.destblend;
    this->autoSetProjMatrix = obj.autoSetProjMatrix;
}

Shader::Shader(Shader&& obj) noexcept
{
    this->id = obj.id;
    obj.id = -1;
    this->zwriteon = obj.zwriteon;
    obj.zwriteon = true;
    this->cullon = obj.cullon;
    obj.cullon = true;
    this->blendon = obj.blendon;
    obj.blendon = false;
    this->srcblend = obj.srcblend;
    obj.srcblend = BlendType::SrcAlpha;
    this->destblend = obj.destblend;
    obj.destblend = BlendType::OneMinusSrcAlpha;
    this->autoSetProjMatrix = obj.autoSetProjMatrix;
    obj.autoSetProjMatrix = false;
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
    this->blendon = other.blendon;
    this->srcblend = other.srcblend;
    this->destblend = other.destblend;
    this->autoSetProjMatrix = other.autoSetProjMatrix;
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
    this->blendon = other.blendon;
    other.blendon = false;
    this->srcblend = other.srcblend;
    other.srcblend = BlendType::SrcAlpha;
    this->destblend = other.destblend;
    other.destblend = BlendType::OneMinusSrcAlpha;
    this->autoSetProjMatrix = other.autoSetProjMatrix;
    other.autoSetProjMatrix = false;
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
    // blending
    if (blendon)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    glBlendFunc((int)srcblend, (int)destblend);
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
#blend [off/on] - whether to use blending

#blendsrc [blendtype]; - must use semicolon to remove ambiguity
#blenddest [blendtype]; - must use semicolon to remove ambiguity
blendtype - Zero, One, SrcColour, SrcAlpha, OneMinusSrcAlpha, ConstantColour, ConstantAlpha

#setProjectionMatrix [off/on] - sets the mat4x4 uniform "projectionMatrix" automatically
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
    bool blend = false;
    BlendType srcblend = BlendType::SrcAlpha;
    BlendType destblend = BlendType::OneMinusSrcAlpha;

    bool setProjectionMatrix = false;

    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        // for zwrite, cull, etc
        bool option = false;
        if (line.find("on") != std::string::npos)
            option = true;

        // for blend type
        BlendType blendType = BlendType::Zero;
        if (line.find("Zero") != std::string::npos)
            blendType = BlendType::Zero;
        else if (line.find("One") != std::string::npos)
            blendType = BlendType::One;
        else if (line.find("SrcColour") != std::string::npos)
            blendType = BlendType::SrcColour;
        else if (line.find("SrcAlpha") != std::string::npos)
            blendType = BlendType::SrcAlpha;
        if (line.find("OneMinusSrcAlpha") != std::string::npos)
            blendType = BlendType::OneMinusSrcAlpha;
        else if (line.find("ConstantColour") != std::string::npos)
            blendType = BlendType::ConstantColour;
        else if (line.find("ConstantAlpha") != std::string::npos)
            blendType = BlendType::ConstantAlpha;

        // shader type
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }

        // zwrite on
        else if (line.find("#zwrite ") != std::string::npos)
            zwrite = option;
        // cull on
        else if (line.find("#cull ") != std::string::npos)
            cull = option;
        // blending
        else if (line.find("#blend ") != std::string::npos)
            blend = option;
        else if (line.find("#blendsrc ") != std::string::npos)
            srcblend = blendType;
        else if (line.find("#blenddest ") != std::string::npos)
            destblend = blendType;

        // automatically set projection matrix
        else if (line.find("#setProjectionMatrix ") != std::string::npos)
            setProjectionMatrix = option;

        else if (type != ShaderType::NONE)
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str(), zwrite, cull, blend, srcblend, destblend, setProjectionMatrix };
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