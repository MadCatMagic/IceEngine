#include "MeshFilter.h"

MeshFilter::MeshFilter() { }

MeshFilter::MeshFilter(Mesh* mesh, Material* mat)
{
	this->mesh = mesh;
	this->mat = mat;
}

MeshFilter::~MeshFilter() { }

void MeshFilter::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void MeshFilter::SetMat(Material* mat)
{
	this->mat = mat;
}

std::vector<MeshFilter> MeshFilter::meshArray = std::vector<MeshFilter>();