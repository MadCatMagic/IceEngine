#include "Engine/Mesh/MeshFilter.h"
#include <exception>

MeshFilter::MeshFilter() { meshArray.push_back(this); }

MeshFilter::MeshFilter(Mesh* mesh, Material* mat)
{
	this->mesh = mesh;
	this->mat = mat;
	meshArray.push_back(this);
}

MeshFilter::~MeshFilter() 
{ 
	std::vector<MeshFilter*>::iterator index = std::find(meshArray.begin(), meshArray.end(), this);
	meshArray.erase(index);
}

void MeshFilter::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void MeshFilter::SetMat(Material* mat)
{
	this->mat = mat;
}

std::vector<MeshFilter*> MeshFilter::meshArray = std::vector<MeshFilter*>();