#pragma once
#include "UICore.h"

#include "Engine/Mesh/Material.h"
#include "Engine/Mesh/VertexBuffer.h"
#include "Engine/Mesh/VertexArray.h"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace UI {
	class TextRenderer
	{
	public:
		TextRenderer(const std::string& filepath);
		TextRenderer(const std::string& filepath, int size);
		TextRenderer(const std::string& filepath, int size, int offset);
		~TextRenderer();

		// sets pixel size
		void SetSize(unsigned int size);
		void SetNewlineOffset(int offset);
		// loads relevant characters into memory
		void LoadChars();
		// draws the characters at the pos with some text as source
		void Draw(const Vector2& pos, const std::string& text, float scale, const Vector3& colour);

		static void SetupRenderer();
		static void ReleaseRenderer();

	private:
		unsigned int size;
		int newlineOffset;
		std::string filepath;

		struct Character {
			Texture2D* texture{ nullptr };  // ID handle of the glyph texture
			Vector2i size;       // Size of glyph
			Vector2i bearing;    // Offset from baseline to left/top of glyph
			unsigned int advance{ 0 };    // Offset to advance to next glyph
		};
		std::map<char, Character> characters;

		static FT_Library ftlib;
		static FT_Face CreateFontFace(const std::string& filepath);
		static void FreeFontFace(FT_Face* face);

		static VertexBuffer* textVB;
		static VertexArray* textVA;
		static Shader* textShader;
		static Material* textMat;
	};
};