#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Time.h"
#include "UI.h"

#include "Quaternion.h"
#include "Input.h"

#include "Mesh.h"
#include "FMaths.h"

#include "Camera.h"
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
    UI::Initialize(winSize);
    
    // entity testing
    Entity player = Entity();
    Entity playerCam = Entity();
    player.DefaultTransform();
    playerCam.DefaultTransform();

    player.transform->AssignChild(playerCam.transform);
    PlayerController* pc = player.AddBehaviour<PlayerController>();

	Camera cam = Camera(0.1f, 50.0f, 90.0f);
    pc->SetCam(&cam);

    // button testing
    UI::Sprite buttonSprite = UI::Sprite("res/sprites/pleasedonotthecat.png");
    buttonSprite.LoadTexture();
    UI::Button button = UI::Button(Vector2i(100, 100), Vector2i(-200, -200), &buttonSprite);

    // text testing
    UI::Text text = UI::Text(60, Vector2i(200, 200), "EeeG..!", "res/fonts/Helvetica.ttf");
    text.CreateRenderer();

	Shader shader = Shader("res/shaders/Basic.shader");
    Material mat = Material(shader);
	Mesh mesh = Mesh(mat, "res/models/monke.obj");
    
    // texture stufffff
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    RenderTexture renderTexture = RenderTexture(winSize.x, winSize.y, Texture::Format::Depth, Texture::Format::RGB);
    renderTexture.GenerateBuffers();

    float r = 0.0f;
    float increment = 0.05f;

    // fps counter
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    float frameTime = (float)glfwGetTime();

    float prevCursorPosX = -1.0;
    float prevCursorPosY = -1.0;
    float rotY = 0.0f;
    
    const float halfPI = 1.5705f;
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
        float currentFrameTime = (float)glfwGetTime();
        Time::SetUDT(currentFrameTime - frameTime);
        frameTime = currentFrameTime;

        // Render to renderTexture
        // rendertexture should be containing the drawn texture in renderTexture.colourBuffer
        renderTexture.Bind();
        Renderer::Viewport(winSize);
        Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // drawing stuff
        mesh.Bind();
        mat.SetVector4("setColour", Vector4(r, 0.0f, 1.0f - r, 1.0f));
        mat.SetMatrix4x4("projectionMatrix", pc->GetCamMatrix());
        mesh.DrawMesh();
        mesh.Unbind();

        // renders the ui ontop
        UI::RenderUI(&renderTexture);
        // clears the screen ready for the new rendertexture to be blit onto it
        Renderer::BindScreenBuffer();
        Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // should draw the texture onto the screen
        Renderer::Blit(renderTexture.colourBuffer->GetID(), 0, winSize);

        Behaviour::UpdateBehaviours();
        
        // changing values
        if (r > 1.0f)
            increment = -0.05f;
        if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        // cursor difference to last frame
        if (prevCursorPosX != -1.0)
        {
            Input::diffX = Input::cursorPosX - prevCursorPosX;
            Input::diffY = Input::cursorPosY - prevCursorPosY;
        }
        prevCursorPosX = Input::cursorPosX;
        prevCursorPosY = Input::cursorPosY;

        // rotation magic ???
        //rotX -= diffX / 120.0f;
        rotY -= Input::diffY / 120.0f;
        rotY = FMaths::Clamp(rotY, -halfPI, halfPI);

        playerCam.transform->SetLocalRot(Vector3((float)rotY, 0.0f, 0.0f));
        player.transform->Rotate(Vector3(0.0f, (float)-Input::diffX / 120.0f, 0.0f));

        //std::cout << playerCam.transform->rotation.ToString() << std::endl;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // clean up
    Renderer::Release();
    Behaviour::ReleaseBehaviours();
    UI::SpriteRenderer::ReleaseRenderer();

    glfwTerminate();
    return 0;
}