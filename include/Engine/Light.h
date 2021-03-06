#pragma once
#include <vector>

#include "Core/Colour.h"
#include "Core/Vector.h"
#include "Behaviour.h"
#include "Graphics/Texture.h"

#define SHADOWMAP_SIZE 2048

class Light : public Behaviour
{
public:
	enum class Type {
		Sun = 0, Spot = 1, Point = 2
	};

	Light();
	~Light();

	float strength{ 1.0f };
	Colour colour{ Colour::white };
	Type type{ Type::Sun };

	Matrix4x4 GetMatrix();
	void GenerateTexture();
	void DeleteTexture();

	Vector3 GetLightDir() const;

	inline RenderTexture* GetTexture() const { return texture; }
	static inline std::vector<Light*>* GetLights() { return &lights; }

private:
	void Update();
	void OnDrawGizmos();

	RenderTexture* texture{ nullptr };

	static std::vector<Light*> lights;
};