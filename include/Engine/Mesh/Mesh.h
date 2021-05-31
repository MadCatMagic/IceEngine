#pragma once

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

#include <list>

// full mesh
// contains render function

struct VertexData;

struct MeshData {
	VertexData* vertices;
	unsigned int vertexCount;
	unsigned int* indices;
	unsigned int indiceCount;
};

struct MeshFace {
	unsigned int v[3];
	unsigned int vt[3];
	unsigned int vn[3];
	//Vector3 n;
};

class Mesh
{
public:
	Mesh(VertexBuffer& vb, IndexBuffer& ib);
	Mesh(const std::string& filepath);
	~Mesh();

	void AddVertexAttribute(int fsize); // offset is sum of previous types
	void AddVertexAttributes(int fsizes[], int count);
	void SetVertexStructure();

	void SetVertexBuffer(const void* data, unsigned int size);
	void SetIndexBuffer(const unsigned int* data, unsigned int count);

	void SetMeshFromFile(const std::string& filepath);

	void Bind() const;
	void Unbind() const;

	inline VertexBuffer GetVertexBuffer() const { return vertexBuffer; }
	inline IndexBuffer GetIndexBuffer() const { return indexBuffer; }

	inline unsigned int GetIndexBufferSize() const { return indexBuffer.GetCount(); }
	
private:
	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
	IndexBuffer indexBuffer;

	std::list <int> typeList;
	int vertexSize;

	static Vector3 TriangleNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	static Vector3 Tangent(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);

	MeshData md{};
	std::vector <float> vvb;
	MeshData ReadMeshFile(const std::string& filepath);
	MeshData ConsolidateVertexData(
		const std::vector <Vector3>& vertices, 
		const std::vector <Vector3>& normals, 
		const std::vector <Vector2>& texUVs, 
		MeshFace* faces, 
		unsigned int faceCount
	);
	void ConstructBuffersFromMeshData(const MeshData& meshData);
	void StandardVertexArray();
};

