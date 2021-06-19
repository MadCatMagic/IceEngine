#include "Engine/Mesh/BasicMesh.h"

MeshData::MeshData() { }

MeshData::MeshData(VertexData* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indiceCount, bool heapAllocated)
	: vertices(vertices), vertexCount(vertexCount), indices(indices), indiceCount(indiceCount), heapAllocated(heapAllocated)
{ }

MeshData::MeshData(const MeshData& obj)
{
	this->vertices = obj.vertices;
	this->vertexCount = obj.vertexCount;
	this->indices = obj.indices;
	this->indiceCount = obj.indiceCount;
	this->heapAllocated = obj.heapAllocated;
}

MeshData::MeshData(MeshData&& obj) noexcept
{
	this->vertices = obj.vertices;
	obj.vertices = nullptr;
	this->vertexCount = obj.vertexCount;
	obj.vertexCount = 0;
	this->indices = obj.indices;
	obj.indices = nullptr;
	this->indiceCount = obj.indiceCount;
	obj.indiceCount = 0;
	this->heapAllocated = obj.heapAllocated;
	obj.heapAllocated = false;
}

MeshData::~MeshData()
{
	if (heapAllocated) {
		delete[] vertices;
		delete[] indices;
	}
}

MeshData& MeshData::operator=(const MeshData& obj)
{
	this->vertices = obj.vertices;
	this->vertexCount = obj.vertexCount;
	this->indices = obj.indices;
	this->indiceCount = obj.indiceCount;
	this->heapAllocated = obj.heapAllocated;
	return *this;
}

MeshData& MeshData::operator=(MeshData&& obj) noexcept
{
	this->vertices = obj.vertices;
	obj.vertices = nullptr;
	this->vertexCount = obj.vertexCount;
	obj.vertexCount = 0;
	this->indices = obj.indices;
	obj.indices = nullptr;
	this->indiceCount = obj.indiceCount;
	obj.indiceCount = 0;
	this->heapAllocated = obj.heapAllocated;
	obj.heapAllocated = false;
	return *this;
}