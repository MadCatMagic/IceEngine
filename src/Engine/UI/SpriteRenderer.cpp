#include "UI/SpriteRenderer.h"

namespace UI
{
    SpriteRenderer::SpriteRenderer() { }

    SpriteRenderer::SpriteRenderer(Sprite* sprite)
    {
        this->sprite = sprite;
    }

    SpriteRenderer::~SpriteRenderer() { }

    void SpriteRenderer::SetSprite(Sprite* sprite)
    {
        this->sprite = sprite;
    }

    void SpriteRenderer::Draw(Vector2 pos, Vector2 size)
    {
        // Use the shader
        spriteMat->Bind();
        glActiveTexture(GL_TEXTURE0);
        sprite->Bind();
        spriteMat->SetTexture("sprite", 0);

        // quad vertex buffero
        float spriteQuad[30] {
        //  pos.x           pos.y           pos.z uv.u  uv.v
            pos.x,          pos.y,          0.0f, 1.0f, 0.0f,
            pos.x + size.x, pos.y,          0.0f, 0.0f, 0.0f,
            pos.x,          pos.y + size.y, 0.0f, 1.0f, 1.0f,
            pos.x,          pos.y + size.y, 0.0f, 1.0f, 1.0f,
            pos.x + size.x, pos.y,          0.0f, 0.0f, 0.0f,
            pos.x + size.x, pos.y + size.y, 0.0f, 0.0f, 1.0f
        };

        spriteVB->ModifyData(spriteQuad, sizeof(float) * 30, 0);
        spriteVA->Bind();
        spriteVA->EnableAttribute(0);
        spriteVA->EnableAttribute(1);

        // Draw the triangles !
        glDrawArrays(GL_TRIANGLES, 0, 6);
        spriteVA->DisableAttribute(0); 
        spriteVA->DisableAttribute(1);
    }

    VertexBuffer* SpriteRenderer::spriteVB = nullptr;
    VertexArray* SpriteRenderer::spriteVA = nullptr;
    Shader* SpriteRenderer::spriteShader = nullptr;
    Material* SpriteRenderer::spriteMat = nullptr;

    void SpriteRenderer::SetupRenderer()
    {
        // setup blit stuff
        spriteVB = new VertexBuffer(0, sizeof(float) * 30, VertexBuffer::UsageHint::DynamicDraw);
        spriteVA = new VertexArray();
        spriteVA->Construct();
        spriteVA->EnableAttribute(0);
        spriteVA->EnableAttribute(1);
        spriteVA->FormatAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 5, 0);
        spriteVA->FormatAttribute(1, 2, GL_FLOAT, false, sizeof(float) * 5, (void*)(sizeof(float) * 3));
        spriteVA->DisableAttribute(0);
        spriteVA->DisableAttribute(1);

        // Create and compile our GLSL program from the shaders
        spriteShader = new Shader("res/shaders/Sprite.shader");
        spriteMat = new Material(*spriteShader);
    }

    void SpriteRenderer::ReleaseRenderer()
    {
        delete spriteVB;
        delete spriteVA;
        delete spriteShader;
        delete spriteMat;
    }
}