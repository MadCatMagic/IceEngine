#pragma once
#include <vector>
#include <functional>
#include <map>

#include "Vector.h"
#include "Texture.h"

#include "Material.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace UI
{
	extern Vector2 pixelsPerUnit;
	extern Vector2i winSize;

	/* Get pixel scaling and stuff for UI */
	extern void Initialize(const Vector2i& windowSize);

	/* Render GUI */
	extern void RenderUI(RenderTexture* dest);

	class Sprite
	{
	public:
		Sprite();
		Sprite(const std::string& filepath);
		~Sprite();

		void SetPath(const std::string& filepath);
		void LoadTexture();

		void Bind() const;
		void Unbind() const;

	private:
		std::string filepath;
		Texture2D* texture{ nullptr };
	};

	class SpriteRenderer
	{
	public:
		SpriteRenderer();
		SpriteRenderer(Sprite* sprite);
		~SpriteRenderer();

		void SetSprite(Sprite* sprite);

		void Draw(Vector2 pos, Vector2 size);

		static void SetupRenderer();
		static void ReleaseRenderer();

	private:
		Sprite* sprite{ nullptr };

		static VertexBuffer* spriteVB;
		static VertexArray* spriteVA;
		static Shader* spriteShader;
		static Material* spriteMat;
	};

	/* UI Element - base class */
	class Element
	{
	public:
		inline Element() { elements.push_back(this); }
		virtual ~Element() { for (auto& element : children) delete element; }

		virtual void HandleMouseClick(int action, Vector2i location) = 0;
		virtual void Draw() = 0;
		virtual void OnTransform() = 0;

		void AssignChild(Element* child);

		inline Vector2i GetPixelPosition() const { return pixelPosition; }
		inline Vector2 GetPosition() const { return position; }
		inline Vector2 GetLocalPosition() const { return localPosition; }
		void SetPixelPosition(const Vector2i& pos);
		void SetPosition(const Vector2& pos);
		void SetLocalPosition(const Vector2& pos);

		inline Vector2 GetGlobalScale() const { return globalScale; }
		inline Vector2 GetLocalScale() const { return localScale; }
		void SetGlobalScale(const Vector2& scale);
		void SetLocalScale(const Vector2& scale);

		static void DrawElements();

	protected:
		Vector2i pixelPosition;
		Vector2 position;
		Vector2 localPosition;

		Vector2 globalScale;
		Vector2 localScale;

		void UpdateGlobalPosition();
		void UpdateGlobalScale();

		Element* parent = nullptr;

	private:
		std::vector<Element*> children;
		static std::vector<Element*> elements;
	};

	/* UI Button - button class */
	class Button : public Element
	{
	public:
		Button(const Vector2i& size, const Vector2i& pixelPos);
		Button(const Vector2i& size, const Vector2i& pixelPos, Sprite* sprite);
		virtual ~Button();

		virtual void HandleMouseClick(int action, Vector2i location) override;
		virtual void Draw() override;
		virtual void OnTransform() override;

		void SetAction(std::function<void(int)> action);
		void SetSprite(Sprite* sprite);

	private:
		Vector2i size;
		SpriteRenderer* spriteRenderer{ nullptr };

		std::function<void(int)> action = [](int button) {};

		void CreateRenderer(Sprite* sprite);
	};

	/* UI Button - button class */
	class Grid : public Element
	{
	public:
		Grid(const Vector2i& size, const Vector2i& pixelPos);
		Grid(const Vector2i& size, const Vector2i& pixelPos, const Vector2i& gridSize);
		virtual ~Grid();

		inline virtual void HandleMouseClick(int action, Vector2i location) override {};
		virtual void Draw() override;
		virtual void OnTransform() override;

	private:
		Vector2i size;
		Vector2i gridSize;
	};

	class TextRenderer
	{
	public:
		TextRenderer(const std::string& filepath);
		TextRenderer(const std::string& filepath, int size);
		~TextRenderer();

		// sets pixel size
		void SetSize(unsigned int size);
		// loads relevant characters into memory
		void LoadChars();
		// draws the characters at the pos with some text as source
		void Draw(const Vector2& pos, const std::string& text, float scale, const Vector3& colour);

		static void SetupRenderer();
		static void ReleaseRenderer();

	private:
		unsigned int size;
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

	class Text : public Element
	{
	public:
		Text(unsigned int size, const Vector2i& pixelPos);
		Text(unsigned int size, const Vector2i& pixelPos, const std::string& text);
		Text(unsigned int size, const Vector2i& pixelPos, const std::string& text, const std::string& filepath);		
		virtual ~Text();

		inline virtual void HandleMouseClick(int action, Vector2i location) override {};
		virtual void Draw() override;
		virtual void OnTransform() override;

		void SetText(const std::string& text);
		void SetFont(const std::string& filepath);

		void CreateRenderer();

	private:
		int size;
		std::string text;
		std::string font;
		TextRenderer* textRenderer{ nullptr };
	};
}
