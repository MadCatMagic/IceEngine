#pragma once
#include "Behaviour.h"
#include "Mesh.h"

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

private:
	static std::vector<MeshFilter> meshArray;
};

