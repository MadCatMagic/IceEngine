#pragma once

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Matrix.h"

class Material
{
public:
	Material();
	Material(Shader& shader);
	Material(const Material& obj);
	Material(Material&& obj) noexcept;
	~Material();

	void SetShader(Shader& shader);

	void SetVector2(const std::string& name, const Vector2& vector);
	void SetVector3(const std::string& name, const Vector3& vector);
	void SetVector4(const std::string& name, const Vector4& vector);

	void SetMatrix4x4(const std::string& name, const Matrix4x4 matrix);

	void SetTexture(const std::string& name, unsigned int texid);

	Material& operator=(const Material& other);
	Material& operator=(Material&& other) noexcept;

	void Bind() const;
	void Unbind() const;

private:
	Shader shader;
	std::unordered_map <std::string, int> uniformLocations;

	int GetUniformLocation(const std::string& name);
};

