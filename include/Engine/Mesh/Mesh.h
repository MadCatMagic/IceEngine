#pragma once

#include "Engine/Core.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

#include "BasicMesh.h"

#include <list>

// full mesh
// contains render function


// can load from file formats .obj and .icem
// .icem is fastest and has no required processing for running
// .icem is a cache object so would have to be regenerated after any changes aswell
class Mesh : public BasicMesh
{
public:

	Mesh(VertexBuffer& vb, IndexBuffer& ib);
	// load mesh from Format file
	Mesh(const std::string& filepath);
	~Mesh();

	void AddVertexAttribute(int fsize); // offset is sum of previous types
	void AddVertexAttributes(int fsizes[], int count);
	void SetVertexStructure();

	void SetVertexBuffer(const void* data, unsigned int size);
	void SetIndexBuffer(const unsigned int* data, unsigned int count);

	virtual void SetMeshFromFile(const std::string& filepath, bool generateICEMFile = false) override;

	void Bind() const;
	void Unbind() const;

	static MeshData GetMeshDataFromFile(const std::string& filepath);

	// returns a const reference
	inline const VertexBuffer& GetVertexBuffer() const { return vertexBuffer; }
	// returns a const reference
	inline const IndexBuffer& GetIndexBuffer() const { return indexBuffer; }

	inline unsigned int GetIndexBufferSize() const { return indexBuffer.GetCount(); }
	
private:
	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
	IndexBuffer indexBuffer;

	std::list <int> typeList;
	int vertexSize;

	std::vector <float> vvb;

	// takes meshdata and constructs buffers from it
	void ConstructBuffersFromMeshData(const MeshData& meshData);

	// position, normal, uv, tangent
	void StandardVertexArray();
};

