#pragma once

// self contained shader class
// give it a directory and it will parse the shader, compile it, 
// and create a program id for use with mesh

struct ShaderProgramSource;
class Shader
{
public:
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

private:
    bool zwriteon{};
    bool cullon{};

    ShaderProgramSource ParseShader(const std::string& filepath);                                  // splits shader into vert + frag
    unsigned int CompileShader(const std::string& source, unsigned int type);                      // compiles one shader
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader); // compiles vert + frag into program
};

