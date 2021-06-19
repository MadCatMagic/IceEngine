#include "Engine/UI/TextRenderer.h"

#include "Engine/Core.h"

namespace UI
{
    TextRenderer::TextRenderer(const std::string& filepath)
    {
        this->filepath = filepath;
        this->size = 0;
        this->newlineOffset = 10;
    }

    TextRenderer::TextRenderer(const std::string& filepath, int size)
    {
        this->filepath = filepath;
        this->size = size;
        this->newlineOffset = size;
    }

    TextRenderer::TextRenderer(const std::string& filepath, int size, int offset)
    {
        this->filepath = filepath;
        this->size = size;
        this->newlineOffset = offset;
    }

    TextRenderer::~TextRenderer()
    {
        if (characters.size() > 0)
            for (auto& character : characters)
                delete character.second.texture;
    }

    void TextRenderer::SetSize(unsigned int size)
    {
        this->size = size;
    }

    void TextRenderer::SetNewlineOffset(int offset)
    {
        this->newlineOffset = offset;
    }

    // EXPENSIVE OPERATION
    void TextRenderer::LoadChars()
    {
        FT_Face face = CreateFontFace(filepath);

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, size);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                Console::LogError("ERROR::FREETYPE: Failed to load Glyph");
                continue;
            }

            // generate texture
            Texture2D* texture = new Texture2D(
                Texture::Format::Red, 
                Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
                0, 
                face->glyph->bitmap.buffer
            );

            // now store character for later use
            Character character = {
                texture,
                Vector2i(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                Vector2i(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        FreeFontFace(&face);
    }

    // Draw text to screen :P
    void TextRenderer::Draw(const Vector2& pos, const std::string& text, float scale, const Vector3& colour)
    {
        // activate corresponding render state	
        textMat->Bind();

        textMat->SetVector3("textColour", colour);
        glActiveTexture(GL_TEXTURE0);
        textMat->SetTexture("char", 0);

        textVA->Bind();
        textVA->EnableAttribute(0);

        int x = (int)(pos.x * pixelsPerUnit.x);
        int y = (int)(pos.y * pixelsPerUnit.y);
        int startx = x;

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = characters[*c];

            float xpos = (x + ch.bearing.x) / pixelsPerUnit.x;
            float ypos = (y - ch.size.y + ch.bearing.y) / pixelsPerUnit.y;
            float w = ch.size.x / pixelsPerUnit.x;
            float h = ch.size.y / pixelsPerUnit.y;

            // update VBO for each character
            float quad[24] = {
                xpos,     ypos + h, 0.0f, 0.0f,
                xpos,     ypos,     0.0f, 1.0f,
                xpos + w, ypos,     1.0f, 1.0f,
                xpos,     ypos + h, 0.0f, 0.0f,
                xpos + w, ypos,     1.0f, 1.0f,
                xpos + w, ypos + h, 1.0f, 0.0f
            };
            // render glyph texture over quad
            ch.texture->Bind();

            // update content of VB memory
            textVB->ModifyData(quad, sizeof(quad), 0);

            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph
            x += (int)((ch.advance >> 6) * scale); // bitshift by 6 to get value in pixel
            
            // if newline offset by newlineOffset and put x to startx
            if (*c == 10)
            {
                y -= newlineOffset;
                x = startx;
            }
        }
        textVA->DisableAttribute(0);
        textVA->DisableAttribute(1);

        glBindTexture(GL_TEXTURE_2D, 0);

    }

    VertexBuffer* TextRenderer::textVB = nullptr;
    VertexArray* TextRenderer::textVA = nullptr;
    Shader* TextRenderer::textShader = nullptr;
    Material* TextRenderer::textMat = nullptr;

    void TextRenderer::SetupRenderer()
    {
        // initialize library
        if (FT_Init_FreeType(&ftlib))
        {
            Console::LogError("ERROR::FREETYPE: Could not init FreeType Library");
        }

        // opengl stuff
        textVB = new VertexBuffer(0, sizeof(float) * 30, VertexBuffer::UsageHint::DynamicDraw);
        textVA = new VertexArray();
        textVA->Construct();
        textVA->EnableAttribute(0);
        textVA->FormatAttribute(0, 4, GL_FLOAT, false, 0, 0);
        textVA->DisableAttribute(0);

        // Create and compile our GLSL program from the shaders
        textShader = new Shader("res/shaders/Text.shader");
        textMat = new Material(*textShader);
    }

    void TextRenderer::ReleaseRenderer()
    {
        FT_Done_FreeType(ftlib);
    }

    FT_Face TextRenderer::CreateFontFace(const std::string& filepath)
    {
        // load font
        FT_Face face = nullptr;
        if (FT_New_Face(ftlib, filepath.c_str(), 0, &face))
        {
            Console::LogError("ERROR::FREETYPE: Failed to load font");
        }
        return face;
    }

    void TextRenderer::FreeFontFace(FT_Face* face)
    {
        FT_Done_Face(*face);
    }

    FT_Library TextRenderer::ftlib = { nullptr };
}