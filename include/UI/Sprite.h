#pragma once
#include "UI/UICore.h"

namespace UI {
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
};
