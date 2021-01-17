#include <GL/glew.h>
#include <iostream>

#include "Material.h"

Material::Material() { }

Material::Material(Shader& shader)
{
	this->shader = shader;
}


Material::Material(const Material& obj) {
	// Copy Constructor
	// copy of object is created
	this->shader = obj.shader;
	// Deep copying
}

Material::Material(Material&& obj) noexcept {
	// Move constructor
	// It will simply shift the resources,
	// without creating a copy.
	this->shader = obj.shader;
	obj.shader = Shader();
}

Material::~Material()
{
	shader.~Shader();
	uniformLocations.~unordered_map();
}

void Material::SetShader(Shader& shader)
{
	this->shader = shader;
}

void Material::SetVector2(const std::string& name, const Vector2& vector)
{
	int location = GetUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}

void Material::SetVector3(const std::string& name, const Vector3& vector)
{
	int location = GetUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Material::SetVector4(const std::string& name, const Vector4& vector)
{
	int location = GetUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Material::SetMatrix4x4(const std::string& name, const Matrix4x4 matrix)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, true, matrix.m);
}

void Material::SetTexture(const std::string& name, unsigned int texid)
{
	int location = GetUniformLocation(name);
	glUniform1i(location, texid);
}

Material& Material::operator=(const Material& other)
{
	this->shader = other.shader;
	return *this;
}

Material& Material::operator=(Material&& other) noexcept
{
	this->shader = other.shader;
	other.shader = Shader();
	return *this;
}

void Material::Bind() const
{
	shader.Bind();
}

void Material::Unbind() const
{
	shader.Unbind();
}

int Material::GetUniformLocation(const std::string& name)
{
	if (uniformLocations.find(name) != uniformLocations.end())
		return uniformLocations[name];
	else
	{
		int location = glGetUniformLocation(shader.id, name.c_str());
		uniformLocations[name] = location;
		return location;
	}
}
