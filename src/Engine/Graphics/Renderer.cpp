#include <iostream>

#include "Renderer.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

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
    
    void BindScreenBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // blit stuff

    static VertexBuffer* blitVB;
    static VertexArray* blitVA;
    static Shader* blitShader;
    static Material* blitMat;
    static bool blitSetup{};

    void Blit(Texture2D src, RenderTexture dest)
    {
        Blit(src.GetID(), dest.colourBuffer->GetID(), Vector2i(dest.width, dest.height));
    }

    void Blit(RenderTexture src, RenderTexture dest)
    {
        Blit(src.colourBuffer->GetID(), dest.colourBuffer->GetID(), Vector2i(dest.width, dest.height));
    }

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
        glDrawArrays(GL_TRIANGLES, 0, 6);
        blitVA->DisableAttribute(0);
    }

    void BlitToScreen(Texture2D src, const Vector2i& winSize)
    {
        Blit(src.GetID(), 0, winSize);
    }

    void BlitToScreen(RenderTexture src, const Vector2i& winSize)
    {
        Blit(src.colourBuffer->GetID(), 0, winSize);
    }

    void Release()
    {
        delete blitVB;
        delete blitVA;
        delete blitShader;
        delete blitMat;
    }
}