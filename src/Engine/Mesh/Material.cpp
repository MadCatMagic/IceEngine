#include <GL/glew.h>
#include <iostream>
#include <vector>

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

void Material::SetInt(const std::string& name, int i)
{
	int location = GetUniformLocation(name);
	glUniform1i(location, i);
}

void Material::SetIntArray(const std::string& name, const int* ints, unsigned int count)
{
	int location = GetUniformLocation(name);
	glUniform1iv(location, count, ints);
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

void Material::SetVector2Array(const std::string& name, const Vector2* vectors, unsigned int count)
{
	int location = GetUniformLocation(name);
	std::vector<float> floats = std::vector<float>();
	for (unsigned int i = 0; i < count; i++)
	{
		floats.push_back(vectors[i].x);
		floats.push_back(vectors[i].y);
	}
	glUniform2fv(location, count, floats.data());
}

void Material::SetVector3Array(const std::string& name, const Vector3* vectors, unsigned int count)
{
	int location = GetUniformLocation(name);
	std::vector<float> floats = std::vector<float>();
	for (unsigned int i = 0; i < count; i++)
	{
		floats.push_back(vectors[i].x);
		floats.push_back(vectors[i].y);
		floats.push_back(vectors[i].z);
	}
	glUniform3fv(location, count, floats.data());
}

void Material::SetVector4Array(const std::string& name, const Vector4* vectors, unsigned int count)
{
	int location = GetUniformLocation(name);
	std::vector<float> floats = std::vector<float>();
	for (unsigned int i = 0; i < count; i++)
	{
		floats.push_back(vectors[i].x);
		floats.push_back(vectors[i].y);
		floats.push_back(vectors[i].z);
		floats.push_back(vectors[i].w);
	}
	glUniform4fv(location, count, floats.data());
}

void Material::SetMatrix4x4(const std::string& name, const Matrix4x4& matrix)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, true, matrix.m);
}

void Material::SetMatrix4x4Array(const std::string& name, const Matrix4x4* matrix, unsigned int count)
{
	int location = GetUniformLocation(name);
	std::vector<float> floats = std::vector<float>();
	// assign function takes range such as array and adds to vector
	for (unsigned int i = 0; i < count; i++)
		floats.assign(matrix[i].m, matrix[i].m + 16);
	glUniformMatrix4fv(location, 1, true, floats.data());
}

void Material::SetTexture(const std::string& name, unsigned int texid)
{
	int location = GetUniformLocation(name);
	glUniform1i(location, texid);
}

void Material::SetTextures(const std::string& name, const int* texids, unsigned int count)
{
	int location = GetUniformLocation(name);
	glUniform1iv(location, count, texids);
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
