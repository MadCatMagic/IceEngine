#pragma once

struct ShaderProgramSource;
// self contained shader class
// give it a directory and it will parse the shader, compile it, 
// and create a program id for use with mesh
class Shader
{
public:
    enum class BlendType {
        Zero = GL_ZERO, One = GL_ONE, SrcColour = GL_SRC_COLOR,
        SrcAlpha = GL_SRC_ALPHA, OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
        ConstantColour = GL_CONSTANT_COLOR, ConstantAlpha = GL_CONSTANT_ALPHA
    };

    unsigned int id = -1;                // program id
    Shader();
    Shader(const std::string& filepath); // constructor
    Shader(const Shader& obj);
    Shader(Shader&& obj) noexcept;
    ~Shader();

    void SetPath(const std::string& filepath);

    Shader& operator=(const Shader& other);
    Shader& operator=(Shader&& other) noexcept;

    void Bind() const;
    void Unbind() const;

    inline bool AutoSetProjMatrix() const { return autoSetProjMatrix; }

private:
    bool zwriteon{};
    bool cullon{};
    bool blendon{};
    BlendType srcblend{ BlendType::SrcAlpha };
    BlendType destblend{ BlendType::OneMinusSrcAlpha };

    bool autoSetProjMatrix{ false };

    ShaderProgramSource ParseShader(const std::string& filepath);                                  // splits shader into vert + frag
    unsigned int CompileShader(const std::string& source, unsigned int type);                      // compiles one shader
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader); // compiles vert + frag into program
};

