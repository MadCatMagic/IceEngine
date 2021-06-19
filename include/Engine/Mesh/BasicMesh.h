#pragma once
#include "Engine/Core.h"

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

// container for vertices and indices
// vertices : of type VertexData, and contain position, normal, texture, and tangent
// indices : of type unsigned int and point to vertices
// three indices make up a triangle
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

// used in the mesh importing process
struct MeshFace {
	unsigned int v[3];
	unsigned int vt[3];
	unsigned int vn[3];
};

// contains lots of utilities for generating meshes
// base class for anything which wants to be a custom type of mesh
class BasicMesh
{
public:

	inline virtual void SetMeshFromFile(const std::string& filepath, bool generateICEMFile = false) { md = ReadMeshFile(filepath, generateICEMFile); }
	inline void SetMeshData(const MeshData& meshData) { md = meshData; }

	// returns a const reference
	inline const MeshData& GetMeshData() const { return md; }

	// calculates the normal of a triangle with points v1, v2, and v3
	static Vector3 TriangleNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	// calculates the tangent for tangent space of a triangle (pos1, pos2, pos3) with uvs (uv1, uv2, uv3)
	static Vector3 Tangent(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3);

	// -- mesh loading pipeline --

	// reads meshdata from any filetype
	// automatically detects what filetype to use
	// will generate a .icem file if filetype not .icem and generateICEMFile == true
	static MeshData ReadMeshFile(const std::string& filepath, bool generateICEMFile = false);

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

	// -- custom .icem pipeline --

	// reads mesh from icem file
	// no need for ConsolidateVertexData() so is much faster
	static MeshData ReadMeshFileICEM(const std::string& filepath);

	// compiles 
	static ustring CompileICEMFile(const MeshData& meshData);
	static void WriteICEMFile(const ustring& data, const std::string& filepath);

protected:
	MeshData md{};
};

