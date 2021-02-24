#pragma once
#include <vector>
#include <functional>
#include "Vector.h"
#include "Texture.h"

#include "Material.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

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

		void SetPath(const std::string& filepath);

	private:
		std::string filepath;
		Texture2D* texture{ nullptr };
	};

	class SpriteRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void Draw();

	private:
		bool spriteSetup{ false };
		static float spriteQuad[18];
		VertexBuffer* spriteVB{ nullptr };
		VertexArray* spriteVA{ nullptr };

		static bool spriteMatSetup;
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
		virtual ~Button();

		virtual void HandleMouseClick(int action, Vector2i location) override;
		virtual void Draw() override;
		virtual void OnTransform() override;

		void SetAction(std::function<void(int)> action);

	private:
		Vector2i size;

		bool buttonSetup{ false };
		float buttonQuad[18];
		VertexBuffer* buttonVB{ nullptr };
		VertexArray* buttonVA{ nullptr };

		static bool buttonMatSetup;
		static Shader* buttonShader;
		static Material* buttonMat;

		std::function<void(int)> action = [](int button) {};
	};

	/* UI Button - button class */
	class Panel : public Element
	{
	public:
		Panel(const Vector2i& size, const Vector2i& pixelPos);
		Panel(const Vector2i& size, const Vector2i& pixelPos, const Vector2i& gridSize);
		virtual ~Panel();

		inline virtual void HandleMouseClick(int action, Vector2i location) override {};
		virtual void Draw() override;
		virtual void OnTransform() override;

	private:
		Vector2i size;
		Vector2i gridSize;

		bool buttonSetup{ false };
		float buttonQuad[18];
		VertexBuffer* buttonVB{ nullptr };
		VertexArray* buttonVA{ nullptr };

		static bool buttonMatSetup;
		static Shader* buttonShader;
		static Material* buttonMat;

		std::function<void(int)> action = [](int button) {};
	};
}