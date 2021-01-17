#include <iostream>

#include "Renderer.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace Renderer
{
    static void APIENTRY GLErrorCall(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        std::cout << "OpenGL Error:" << std::endl;
        std::cout << "Source: 0x" << std::hex << source << std::endl;
        std::cout << "Type: 0x" << std::hex << type << std::endl;
        std::cout << "Id: " << id << std::endl;
        std::cout << "Severity: 0x" << std::hex << severity << std::endl;
        std::cout << "Message: " << message << std::endl;
        if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
            exit(-1);
    }

    void DebugEnable()
    {
        // auto debugging
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLErrorCall, NULL);
    }

    // The fullscreen quad's FBO
    static const float blitQuadData[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };
    
    void Init(bool debugEnable)
    {
        if (debugEnable)
            DebugEnable();
    }

    void ClearScreen(GLbitfield mask)
    {
        glClear(mask);
    }

    void Viewport(const Vector2i& size)
    {
        glViewport(0, 0, size.x, size.y);
    }

    void Viewport(const Vector2i& size, const Vector2i& offset)
    {
        glViewport(offset.x, offset.y, size.x, size.y);
    }

    static VertexBuffer* blitVB;
    static VertexArray* blitVA;
    static Shader* blitShader;
    static Material* blitMat;
    static bool blitSetup{};

    void Blit(int src, int dest, const Vector2i& winSize)
    {
        if (!blitSetup)
        {
            // setup blit stuff
            blitVB = new VertexBuffer(blitQuadData, sizeof(float) * 18);
            blitVA = new VertexArray();
            blitVA->Construct();
            blitVA->EnableAttribute(0);
            blitVA->FormatAttribute(0, 3, GL_FLOAT, false, 0, 0);
            blitVA->DisableAttribute(0);
            // Create and compile our GLSL program from the shaders
            blitShader = new Shader("res/shaders/Blit.shader");
            blitMat = new Material(*blitShader);
            blitSetup = true;
        }

        // Render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, dest);
        Viewport(winSize);
        ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader
        blitMat->Bind();

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, src);
        // Set our "renderedTexture" sampler to use Texture Unit 0
        blitMat->SetTexture("renderedTexture", 0);

        // 1rst attribute buffer : vertices
        blitVB->Bind();
        blitVA->Bind();
        blitVA->EnableAttribute(0);

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
        blitVA->DisableAttribute(0);
    }

    void Release()
    {
        delete blitVB;
        delete blitVA;
        delete blitShader;
        delete blitMat;
    }
}