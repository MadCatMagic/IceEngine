#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine/Core.h"

#include "Engine/Renderer.h"
#include "Engine/Time.h"
#include "Engine/UI.h"

#include "Engine/Quaternion.h"
#include "Engine/Input.h"

#include "Engine/Mesh.h"

#include "Engine/Camera.h"
#include "Engine/Entity.h"
#include "PlayerController.h"
#include "EntityInspector.h"

#include "Engine/Light.h"
#include "TemporalReprojection.h"

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
    
    Renderer::Init(true, window);
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
	Camera* cam = playerCam.AddBehaviour<Camera>();
    //TemporalReprojection* tr = playerCam.AddBehaviour<TemporalReprojection>();
    cam->SetParams(0.1f, 50.0f, 120.0f * (PI / 180.0f), (float)winSize.x / (float)winSize.y);
    Renderer::SetMainCamera(cam);

    // button testing
    UI::Sprite panelSprite = UI::Sprite("res/sprites/pleasedonotthecat.png");
    panelSprite.LoadTexture();
    UI::Panel panel = UI::Panel(Vector2i(100, 100), Vector2i(-200, -200), &panelSprite);
    
	Shader shader = Shader("res/shaders/LitNormals.shader");
    Material mat = Material(shader);
	Mesh mesh = Mesh("res/models/monke.obj.icem");
    Mesh axis = Mesh("res/models/Axis.obj.icem");
    Mesh sceneMesh = Mesh("res/models/scene2t.obj.icem");

    // texturing
    Texture2D panelDiffuse = Texture2D("res/textures/PanelDiffuse.png");
    Texture2D panelSpecular = Texture2D("res/textures/PanelSpecular.bmp");
    Texture2D panelNormal = Texture2D("res/textures/PanelNormal.bmp");

    /* MESH CREATION */
    // mesh filter brings it all together
    Entity monke = Entity();
    monke.transform->Move(Vector3(-1.0f, 1.0f, 3.0f));
    MeshFilter* monkeFilter = monke.AddBehaviour<MeshFilter>();
    monkeFilter->SetMesh(&mesh);
    monkeFilter->SetMat(&mat);

    // mesh filter brings it all together
    Entity axise = Entity();
    MeshFilter* axisFilter = axise.AddBehaviour<MeshFilter>();
    axisFilter->SetMesh(&axis);
    axisFilter->SetMat(&mat);
    // scene
    Entity scenee = Entity();
    MeshFilter* sceneFilter = scenee.AddBehaviour<MeshFilter>();
    sceneFilter->SetMesh(&sceneMesh);
    sceneFilter->SetMat(&mat);

    /* LIGHT SETUP */
    // entity with sun light facing nearly down
    // NOTE: sun doesnt like being straight down - view matrix breaks or something
    Entity lightEntity = Entity();
    lightEntity.transform->Rotate(Vector3(DegreesToRadii(30.0f), 0.0f, DegreesToRadii(-20.0f)));
    Light* light = lightEntity.AddBehaviour<Light>();
    light->colour = Colour(0.85f, 0.7f, 0.65f) * 0.6f;
    light->GenerateTexture();

    // second light for spot testing
    Entity lightEntity2 = Entity();
    lightEntity2.transform->Rotate(Vector3(DegreesToRadii(20.0f), DegreesToRadii(-50.0f), 0.0f));
    lightEntity2.transform->Move(Vector3(1.5f, 1.5f, 4.0f));
    Light* light2 = lightEntity2.AddBehaviour<Light>();
    light2->type = Light::Type::Spot;
    light2->colour = Colour::yellow;
    light2->strength = 2.0f;
    light2->GenerateTexture();

    // texture stufffff
    // camera rendertexture
    RenderTexture camTexture = RenderTexture(winSize.x, winSize.y, Texture::Format::Depth, Texture::Format::RGB);
    camTexture.GenerateBuffers();

    // secondary texture for post processing
    // must change this
    RenderTexture renderTexture = RenderTexture(winSize.x, winSize.y, Texture::Format::Depth, Texture::Format::RGB);
    renderTexture.GenerateBuffers();
    
    // assign camera target
    cam->target = &camTexture;

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
    
    Behaviour::AwakeBehaviours();
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

        // drawing stuff
        mat.Bind();
        Texture::BindTextureUnit(0);
        panelDiffuse.Bind();
        mat.SetTexture("textureDiffuse", 0);
        Texture::BindTextureUnit(1);
        panelSpecular.Bind();
        mat.SetTexture("textureSpecular", 1);
        Texture::BindTextureUnit(2);
        panelNormal.Bind();
        mat.SetTexture("textureNormal", 2);

        // clear renderTexture
        renderTexture.Bind();
        Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::RenderToCamera(cam);
        Renderer::PostProcess(cam, renderTexture);
        Renderer::RenderGizmos(cam);

        // renders the ui ontop
        UI::RenderUI(&renderTexture);
        // clears the screen ready for the new rendertexture to be blit onto it
        Renderer::BindScreenBuffer();
        Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // colour corrected for srgb
        glEnable(GL_FRAMEBUFFER_SRGB);
        // should draw the texture onto the screen
        Renderer::BlitToScreen(renderTexture, winSize);
        glDisable(GL_FRAMEBUFFER_SRGB);

        //Renderer::Blit(light2->GetTexture()->depthBuffer->GetID(), 0, winSize);

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
        rotY = Clamp(rotY, -halfPI, halfPI);
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