#pragma once
#include "UI/Sprite.h"

#include "Material.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace UI {
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
};