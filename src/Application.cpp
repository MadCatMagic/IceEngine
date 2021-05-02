#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Time.h"
#include "UI.h"

#include "Quaternion.h"
#include "Input.h"

#include "Mesh.h"
#include "MeshFilter.h"
#include "FMaths.h"

#include "Camera.h"
#include "Entity.h"
#include "PlayerController.h"
#include "EntityInspector.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const Vector2i winSize(1600, 900);
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

    player.transform->AssignChild(playerCam.transform);
    PlayerController* pc = player.AddBehaviour<PlayerController>();
    EntityInspector* ei = player.AddBehaviour<EntityInspector>();
    ei->SelectEntity(&playerCam);

    const float PI = 3.14159265359f;
	Camera cam = Camera(0.1f, 50.0f, 120.0f * (PI / 180.0f), (float)winSize.x / (float)winSize.y);
    pc->SetCam(&cam);

    // button testing
    //UI::Sprite buttonSprite = UI::Sprite("res/sprites/pleasedonotthecat.png");
    //buttonSprite.LoadTexture();
    //UI::Button button = UI::Button(Vector2i(100, 100), Vector2i(-200, -200), &buttonSprite);

	Shader shader = Shader("res/shaders/Basic.shader");
    Material mat = Material(shader);
	Mesh mesh = Mesh("res/models/monke.obj");
    Mesh axis = Mesh("res/models/Axis.obj");

    // mesh filter brings it all together
    Entity monke = Entity();
    monke.transform->Rotate(Vector3(2.0f, 1.0f, 0.0f));
    MeshFilter* monkeFilter = monke.AddBehaviour<MeshFilter>();
    monkeFilter->SetMesh(&mesh);
    monkeFilter->SetMat(&mat);

    // mesh filter brings it all together
    Entity axise = Entity();
    MeshFilter* axisFilter = axise.AddBehaviour<MeshFilter>();
    axisFilter->SetMesh(&axis);
    axisFilter->SetMat(&mat);
    
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
    float rotX = 0.0f; // just used for logging
    
    Behaviour::StartBehaviours();

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
        mat.Bind();
        mat.SetVector4("setColour", Vector4(r, 0.0f, 1.0f - r, 1.0f));
        axisFilter->DrawMesh(&cam);
        monkeFilter->DrawMesh(&cam);

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
        rotX -= Input::diffX / 120.0f;

        playerCam.transform->SetLocalRot(Vector3((float)rotY, 0.0f, 0.0f));
        player.transform->Rotate(Vector3(0.0f, (float)-Input::diffX / 120.0f, 0.0f));

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