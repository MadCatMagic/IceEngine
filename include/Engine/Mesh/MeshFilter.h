#pragma once
#include "Engine/Behaviour.h"
#include "Mesh.h"
#include "Engine/Graphics/Camera.h"

#include <vector>

class MeshFilter : public Behaviour
{
public:
	Mesh* mesh{ nullptr };
	Material* mat{ nullptr };

	MeshFilter();
	MeshFilter(Mesh* mesh, Material* mat);
	~MeshFilter();

	void SetMesh(Mesh* mesh);
	void SetMat(Material* mat);

	static inline std::vector<MeshFilter*>* GetMeshFilters() { return &meshArray; };

private:
	static std::vector<MeshFilter*> meshArray;
};

