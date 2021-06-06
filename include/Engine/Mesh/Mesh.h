#pragma once

#include "Engine/Core.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"

#include <list>

// full mesh
// contains render function

#define VERTEXDATA_SIZE 11
struct VertexData {
	Vector3 pos;
	Vector3 normal;
	Vector2 texture;
	Vector3 tangent;

	inline VertexData() {}
	inline ~VertexData() {}

	// inefficient
	// want to find better way of doing this
	inline float& DirectIndex(int i)
	{
		switch (i) {
		case 0:
			return pos.x;
		case 1:
			return pos.y;
		case 2:
			return pos.z;
		case 3:
			return normal.x;
		case 4:
			return normal.y;
		case 5:
			return normal.z;
		case 6:
			return texture.x;
		case 7:
			return texture.y;
		case 8:
			return tangent.x;
		case 9:
			return tangent.y;
		case 10:
			return tangent.z;
		}
		return pos.x;
	}

	inline bool operator==(const VertexData& v2)
	{
		return
			pos == v2.pos &&
			normal == v2.normal &&
			texture == v2.texture &&
			tangent == v2.tangent;
	}
};

struct MeshData {
	VertexData* vertices{ nullptr };
	unsigned int vertexCount{ };
	unsigned int* indices{ nullptr };
	unsigned int indiceCount{ };

	bool heapAllocated{ false };

	MeshData();
	MeshData(VertexData* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indiceCount, bool heapAllocated);
	MeshData(const MeshData& obj);
	MeshData(MeshData&& obj) noexcept;
	~MeshData();

	MeshData& operator=(const MeshData& obj);
	MeshData& operator=(MeshData&& obj) noexcept;
};

struct MeshFace {
	unsigned int v[3];
	unsigned int vt[3];
	unsigned int vn[3];
};

// can load from file formats .obj and .icem
// .icem is fastest and has no required processing for running
// .icem is a cache object so would have to be regenerated after any changes aswell
class Mesh
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

	void SetMeshFromFile(const std::string& filepath);

	void Bind() const;
	void Unbind() const;

	static MeshData GetMeshDataFromFile(const std::string& filepath);

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

	// -- mesh loading pipeline --
	// reads meshdata from .obj file
	static MeshData ReadMeshFileOBJ(const std::string& filepath);

	// generates cleaner meshdata
	static MeshData ConsolidateVertexData(
		const std::vector <Vector3>& vertices, 
		const std::vector <Vector3>& normals, 
		const std::vector <Vector2>& texUVs, 
		MeshFace* faces, 
		unsigned int faceCount
	);

	// takes meshdata and constructs buffers from it
	void ConstructBuffersFromMeshData(const MeshData& meshData);

	// position, normal, uv, tangent
	void StandardVertexArray();

	// -- custom .icem pipeline --

	// reads mesh from icem file
	// no need for ConsolidateVertexData() so is much faster
	static MeshData ReadMeshFileICEM(const std::string& filepath);

	// compiles 
	static ustring CompileICEMFile(const MeshData& meshData);
	static void WriteICEMFile(const ustring& data, const std::string& filepath);
};

