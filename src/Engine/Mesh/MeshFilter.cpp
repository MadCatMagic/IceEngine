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

void MeshFilter::DrawMesh(Camera* cam)
{
	if (mesh != nullptr && mat != nullptr)
	{
		mesh->Bind();
		mat->Bind();
		// automatically set the uniform projectionMatrix if the flag is set in mat's shader
		if (mat->GetShaderReference()->AutoSetProjMatrix())
		{
			Matrix4x4 transMatrix = this->transform->TransformationMatrix();
			Matrix4x4 mvpMatrix = cam->GetProjectionMatrix();
			mat->SetMatrix4x4("projectionMatrix", mvpMatrix);
			mat->SetMatrix4x4("modelMatrix", transMatrix);
		}
		glDrawElements(GL_TRIANGLES, mesh->GetIndexBufferSize(), GL_UNSIGNED_INT, nullptr);
	}
}

std::vector<MeshFilter> MeshFilter::meshArray = std::vector<MeshFilter>();