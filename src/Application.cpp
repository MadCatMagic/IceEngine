#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Time.h"

#include "Quaternion.h"
#include "Input.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Mesh.h"

#include "FMaths.h"
#include "Camera.h"
#include "RenderTexture.h"

#include "Entity.h"
#include "PlayerController.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const Vector2i winSize(1280, 960);
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    window = glfwCreateWindow(winSize.x, winSize.y, "IceEngine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK) 
    {
        std::cout << "GLEW ERROR" << std::endl;
    }

    Renderer::Init(true);
    Input::EnableInput(window);

    // entity testing
    Entity ent = Entity();
    ent.DefaultTransform();
    ent.AddBehaviour<PlayerController>();

	Camera cam = Camera(Transform(), 0.1f, 50.0f, 90.0f);

	Shader shader = Shader("res/shaders/Basic.shader");
    Material mat = Material(shader);
	Mesh mesh = Mesh(mat, "res/models/monke.obj");
    
    // texture stufffff
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    RenderTexture renderTexture = RenderTexture();
    
    // textures and buffers
    Texture2D renderedTexture = Texture2D(0, Texture2D::TextureFormat::RGB, winSize, Texture2D::TextureFormat::RGB);
    renderedTexture.SetDefaultFiltering();
    RenderBuffer depthrenderbuffer = RenderBuffer(RenderBuffer::RenderBufferType::DEPTH, winSize);
    renderTexture.SetAttachment(RenderTexture::AttachmentType::DEPTH, depthrenderbuffer);
    renderTexture.SetAttachment(RenderTexture::AttachmentType::COLOR0, renderedTexture, 0);
    
    // draw ffbbuyfferers
    RenderTexture::AttachmentType DrawBuffers[1] = { RenderTexture::AttachmentType::COLOR0 };
    renderTexture.SetDrawBuffers(1, DrawBuffers);
    if (!renderTexture.TextureOK())
        std::cout << "rendertexture error" << std::endl;

    float r = 0.0f;
    float increment = 0.05f;

    // fps counter
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    float frameTime = (float)glfwGetTime();

    double prevCursorPosX = -1.0;
    double prevCursorPosY = -1.0;

    double rotX = 0.0f;
    double rotY = 0.0f;

    const double halfPI = 1.5705f;

    const float speed = 0.04f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // fps counter
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0) {
            std::cout << nbFrames << " fps" << std::endl;
            nbFrames = 0;
            lastTime += 1.0;
        }
        
        // Time settings
        float currentFrameTime = glfwGetTime();
        Time::SetUDT(currentFrameTime - frameTime);
        frameTime = currentFrameTime;

        // Render to renderTexture
        renderTexture.Bind();
        Renderer::Viewport(winSize); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // drawing stuff
        mesh.Bind();
        mat.SetVector4("setColour", Vector4(r, 0.0f, 1.0f - r, 1.0f));
        mat.SetMatrix4x4("projectionMatrix", cam.GetProjectionMatrix());
        mesh.DrawMesh();
        mesh.Unbind();
        
        Renderer::Blit(renderedTexture.GetID(), 0, winSize);

        Behaviour::UpdateBehaviours();
        
        // changing values
        if (r > 1.0f)
            increment = -0.05f;
        if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        // camera directions
        Vector3 forward = cam.transform.ForwardXZ();
        Vector3 right = cam.transform.RightXZ();

        // movement
        if (Input::spaceState == GLFW_PRESS || Input::spaceState == GLFW_REPEAT)
            cam.transform.position += Vector3(0.0f, speed, 0.0f);
        else if (Input::lshiftState == GLFW_PRESS || Input::lshiftState == GLFW_REPEAT)
            cam.transform.position -= Vector3(0.0f, speed, 0.0f);
        if (Input::wState == GLFW_PRESS || Input::wState == GLFW_REPEAT)
            cam.transform.position += forward * speed;
        else if (Input::sState == GLFW_PRESS || Input::sState == GLFW_REPEAT)
            cam.transform.position -= forward * speed;
        if (Input::dState == GLFW_PRESS || Input::dState == GLFW_REPEAT)
            cam.transform.position -= right * speed;
        else if (Input::aState == GLFW_PRESS || Input::aState == GLFW_REPEAT)
            cam.transform.position += right * speed;

        // cursor difference to last frame
        double diffX = 0;
        double diffY = 0;
        if (prevCursorPosX != -1.0)
        {
            diffX = Input::cursorPosX - prevCursorPosX;
            diffY = Input::cursorPosY - prevCursorPosY;
        }
        prevCursorPosX = Input::cursorPosX;
        prevCursorPosY = Input::cursorPosY;

        // rotation magic ???
        rotX -= diffX / 120.0f;
        rotY -= diffY / 120.0f; 
        rotY = FMaths::Clamp(rotY, -halfPI, halfPI);
        Quaternion yrot(Vector3((float)rotY));
        Quaternion xrot(Vector3(0.0f, (float)rotX));
        cam.viewDir = (yrot * xrot).RotateVector(Vector3(0.0f, 0.0f, 1.0f));
        //cam.transform.SetRotation(Vector3((float)rotY));
        //cam.transform.Rotate(Vector3(0.0f, (float)rotX));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Renderer::Release();
    Behaviour::ReleaseBehaviours();

    glfwTerminate();
    return 0;
}