
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Mesh.h"

#include "Engine/Light.h"
#include "Engine/Graphics/PostProcessEffect.h"

#include "Engine/Entity.h"
#include "Engine/Gizmos.h"

namespace Renderer
{
    static void APIENTRY GLErrorCall(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        std::string s = "-- OpenGL Error --\n";

        if (source == GL_DEBUG_SOURCE_API)             s += "Source : API\n";
        if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)   s += "Source : WINDOW_SYSTEM\n";
        if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) s += "Source : SHADER_COMPILER\n";
        if (source == GL_DEBUG_SOURCE_THIRD_PARTY)     s += "Source : THIRD_PARTY\n";
        if (source == GL_DEBUG_SOURCE_APPLICATION)     s += "Source : APPLICATION\n";
        if (source == GL_DEBUG_SOURCE_OTHER)           s += "Source : OTHER\n";
        
        if (type == GL_DEBUG_TYPE_ERROR)               s += "Type : ERROR\n";
        if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) s += "Type : DEPRECATED_BEHAVIOUR\n";
        if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)  s += "Type : UNDEFINED_BEHAVIOUR\n";
        if (type == GL_DEBUG_TYPE_PORTABILITY)         s += "Type : PORTABILITY\n";
        if (type == GL_DEBUG_TYPE_PERFORMANCE)         s += "Type : PERFORMANCE\n";
        if (type == GL_DEBUG_TYPE_OTHER)               s += "Type : OTHER\n";
        if (type == GL_DEBUG_TYPE_MARKER)              s += "Type : MARKER\n";
        if (type == GL_DEBUG_TYPE_PUSH_GROUP)          s += "Type : PUSH_GROUP\n";
        if (type == GL_DEBUG_TYPE_POP_GROUP)           s += "Type : POP_GROUP\n";

        if (severity == GL_DEBUG_SEVERITY_HIGH)         s += "Severity : HIGH\n";
        if (severity == GL_DEBUG_SEVERITY_MEDIUM)       s += "Severity : MEDIUM\n";
        if (severity == GL_DEBUG_SEVERITY_LOW)          s += "Severity : LOW\n";
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) s += "Severity : NOTIFICATION\n";

        s += "Id: " + std::to_string(id) + "\n";
        s += "Message: ";
        s += message;
        s += "\n";
        


        if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            Console::LogError(s);
            exit(-1);
        }
        else
            Console::LogWarning(s);
    }

    void DebugEnable()
    {
        // auto debugging
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLErrorCall, NULL);
    }

    static GLFWwindow* glfwWindow = nullptr;
    Vector2i GetWinSize()
    {
        int width, height;
        glfwGetWindowSize(glfwWindow, &width, &height);
        return Vector2i(width, height);
    }

    static Camera* mainCamera = nullptr;
    void SetMainCamera(Camera* cam)
    {
        mainCamera = cam;
    }

    Camera* GetMainCamera()
    {
        return mainCamera;
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
    
    static void SetupLightMat();
    static void BlitSetup();
    static void BlitRaw(int src, int depth, int dest, const Vector2i& winSize, Material* mat);

    void Init(bool debugEnable, GLFWwindow* window)
    {
        glfwWindow = window;
        if (debugEnable)
            DebugEnable();
        SetupLightMat();
        BlitSetup();

        Gizmos::SetupGizmos();
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

    void BlitSetup()
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
    }

    // Blit variations
    // Texture2D
    void Blit(Texture2D src, RenderTexture dest)
    {
        BlitRaw(src.GetID(), 0, dest.GetID(), Vector2i(dest.width, dest.height), blitMat);
    }

    void Blit(Texture2D src, RenderTexture dest, Material* mat)
    {
        BlitRaw(src.GetID(), 0, dest.GetID(), Vector2i(dest.width, dest.height), mat);
    }

    void Blit(Texture2D src, Texture2D depth, RenderTexture dest)
    {
        BlitRaw(src.GetID(), depth.GetID(), dest.GetID(), Vector2i(dest.width, dest.height), blitMat);
    }

    void Blit(Texture2D src, Texture2D depth, RenderTexture dest, Material* mat)
    {
        BlitRaw(src.GetID(), depth.GetID(), dest.GetID(), Vector2i(dest.width, dest.height), mat);
    }

    // RenderTexture
    void Blit(RenderTexture src, RenderTexture dest, bool drawDepth)
    {
        BlitRaw(src.colourBuffer->GetID(), drawDepth ? src.depthBuffer->GetID() : 0, dest.GetID(), Vector2i(dest.width, dest.height), blitMat);
    }

    void Blit(RenderTexture src, RenderTexture dest, Material* mat, bool drawDepth)
    {
        BlitRaw(src.colourBuffer->GetID(), drawDepth ? src.depthBuffer->GetID() : 0, dest.GetID(), Vector2i(dest.width, dest.height), mat);
    }

    // Raw
    static void BlitRaw(int src, int depth, int dest, const Vector2i& winSize, Material* mat)
    {
        // Render to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, dest);
        Viewport(winSize);

        // Use the shader
        mat->Bind();

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, src);
        // Set our "renderedTexture" sampler to use Texture Unit 0
        mat->SetTexture("blitTexture", 0);

        // depth texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depth);
        mat->SetTexture("depthTexture", 1);

        // 1rst attribute buffer : vertices
        blitVB->Bind();
        blitVA->Bind();
        blitVA->EnableAttribute(0);

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, 6);
        blitVA->DisableAttribute(0);
    }

    // onto RenderTexture id 0
    void BlitToScreen(Texture2D src, const Vector2i& winSize)
    {
        BlitRaw(src.GetID(), 0, 0, winSize, blitMat);
    }

    void BlitToScreen(RenderTexture src, const Vector2i& winSize)
    {
        BlitRaw(src.colourBuffer->GetID(), 0, 0, winSize, blitMat);
    }

    void RenderToCamera(Camera* cam)
    {
        /* first render each light rendertexture out and grab the textures and data */
        std::vector<Light*>* lights = Light::GetLights();
        int num = lights->size();

        // setup lighting information vectors
        std::vector<Matrix4x4> lightMatrices = std::vector<Matrix4x4>(num);
        std::vector<int> textureIndices = std::vector<int>(num);

        std::vector<Vector3> lightPositions = std::vector<Vector3>(num);
        std::vector<Vector3> lightDirs = std::vector<Vector3>(num);
        std::vector<float> lightCutoff = std::vector<float>(num);
        std::vector<float> lightOuterCutoff = std::vector<float>(num);
        std::vector<Vector4> lightColours = std::vector<Vector4>(num);
        std::vector<int> lightTypes = std::vector<int>(num);

        int counter = 4;
        for (int i = 0; i < num; i++) {
            // render light onto RenderTexture rt
            Light* light = (*lights)[i];
            RenderTexture* rt = light->GetTexture();

            rt->Bind();
            Viewport(Vector2i(SHADOWMAP_SIZE, SHADOWMAP_SIZE));
            ClearScreen(GL_DEPTH_BUFFER_BIT);
            RenderLight(light);

            // assign texture to texture unit i
            Texture::BindTextureUnit(counter);
            rt->depthBuffer->Bind();

            // add data
            lightMatrices[i] = light->GetMatrix();
            textureIndices[i] = counter;

            lightPositions[i] = light->transform->GetPos();
            lightDirs[i] = (light->type == Light::Type::Sun) ? -light->transform->Up() : -light->transform->Forward();
            lightCutoff[i] = (float)cos(DegreesToRadii(40));
            lightOuterCutoff[i] = (float)cos(DegreesToRadii(45));
            lightColours[i] = Vector4(light->colour.r, light->colour.g, light->colour.b, light->strength);
            lightTypes[i] = (int)light->type;
            counter++;
        }
        Texture::BindTextureUnit(0);

        // set active rendertexture as camera target
        cam->target->Bind();
        Renderer::Viewport(Vector2i(cam->target->width, cam->target->height));
        Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* render all filters */
        std::vector<MeshFilter*>* filters = MeshFilter::GetMeshFilters();
        Matrix4x4 mvpMatrix = cam->GetProjectionMatrix();
        for (MeshFilter* filter : *filters) {
            if (filter->mesh != nullptr && filter->mat != nullptr)
            {
                // bind mesh and material
                filter->mesh->Bind();
                filter->mat->Bind();

                if (filter->mat->GetShaderReference()->GetParam(Shader::Param::SetProjMatrix))
                {
                    // set all the data prepared earlier
                    filter->mat->SetTextures("shadowMap", textureIndices.data(), num);
                    filter->mat->SetVector3("viewPos", cam->transform->GetPos());
                    filter->mat->SetInt("usedLights", counter - 4);

                    // lighting information
                    filter->mat->SetMatrix4x4Array("lightSpaceMatrix", lightMatrices.data(), num);
                    filter->mat->SetVector3Array("posL", lightPositions.data(), num);
                    filter->mat->SetVector3Array("dirL", lightDirs.data(), num);
                    filter->mat->SetFloatArray("cutoffL", lightCutoff.data(), num);
                    filter->mat->SetFloatArray("outerCutoffL", lightOuterCutoff.data(), num);
                    filter->mat->SetVector4Array("colourL", lightColours.data(), num);
                    filter->mat->SetIntArray("typeL", lightTypes.data(), num);

                    // light attenuation over distance values
                    filter->mat->SetFloat("constantAtten", 1.0f);
                    filter->mat->SetFloat("linearAtten", 0.9f);
                    filter->mat->SetFloat("quadraticAtten", 0.032f);
                }

                // automatically set the uniform projectionMatrix if the flag is set in mat's shader
                if (filter->mat->GetShaderReference()->GetParam(Shader::Param::SetProjMatrix))
                {
                    Matrix4x4 transMatrix = filter->transform->TransformationMatrix();
                    filter->mat->SetMatrix4x4("projectionMatrix", mvpMatrix);
                    filter->mat->SetMatrix4x4("modelMatrix", transMatrix);
                }
                // assign all lighting elements to the texture
                glDrawElements(GL_TRIANGLES, filter->mesh->GetIndexBufferSize(), GL_UNSIGNED_INT, nullptr);
            }
        }
    }

    void PostProcess(Camera* cam, RenderTexture dest)
    {
        PostProcessEffect* p = cam->entity->GetBehaviour<PostProcessEffect>();
        if (p != nullptr)
            p->OnPostProcess(cam->target, &dest);
        else
            Blit(*cam->target, dest, true);
    }

    void RenderGizmos(Camera* cam)
    {
        Gizmos::SetTargetCamera(cam);
        Behaviour::DrawGizmosFromBehaviours();
    }

    static Shader* lightShader;
    static Material* lightMat;

    void SetupLightMat() 
    {
        // Create and compile our GLSL program from the shaders
        lightShader = new Shader("res/shaders/Light.shader");
        lightMat = new Material(*lightShader);
    }

    // renders the scene to renderbuffer using the light shader
    void RenderLight(Light* light)
    {
        std::vector<MeshFilter*>* filters = MeshFilter::GetMeshFilters();
        lightMat->Bind();
        Matrix4x4 mvpMatrix = light->GetMatrix();
        lightMat->SetMatrix4x4("projectionMatrix", mvpMatrix);
        for (MeshFilter* filter : *filters) {
            if (filter->mesh != nullptr)
            {
                filter->mesh->Bind();
                Matrix4x4 transMatrix = filter->transform->TransformationMatrix();
                lightMat->SetMatrix4x4("modelMatrix", transMatrix);
                glDrawElements(GL_TRIANGLES, filter->mesh->GetIndexBufferSize(), GL_UNSIGNED_INT, nullptr);
            }
        }
    }

    void Release()
    {
        delete blitVB;
        delete blitVA;
        delete blitShader;
        delete blitMat;

        delete lightShader;
        delete lightMat;

        Gizmos::Release();
    }
}