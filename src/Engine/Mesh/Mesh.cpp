#include <fstream>
#include <sstream>
#include <iostream>

#include "Mesh.h"
#include "StringUtils.h"
#include "ListUtils.h"

Mesh::Mesh(Material& mat, VertexBuffer& vb, IndexBuffer& ib)
	: vertexBuffer(vb), indexBuffer(ib)
{
	vertexArray.Construct(); 
	// obscure error here - if you use a constructor here like VertexArray()
	// it will not set correctly and will cause obscure errors later on when attempting to bind
	// just use default vertexArray and construct it
	vertexSize = 0;
	material = mat;
}

Mesh::Mesh(Material& mat, const std::string& filepath)
{
	vertexArray.Construct();
	vertexSize = 0;
	material = mat;
	SetMeshFromFile(filepath);
}

Mesh::~Mesh()
{
	Unbind();
}

void Mesh::AddVertexAttribute(int fsize)
{
	typeList.push_back(fsize);
	vertexSize += fsize;
}

void Mesh::AddVertexAttributes(int fsizes[], int count)
{
	for (int i = 0; i < count; i++)
	{
		typeList.push_back(fsizes[i]);
		vertexSize += fsizes[i];
	}
}

void Mesh::SetVertexStructure()
{
	int i = 0;
	unsigned int sum = Sum(typeList);
	int cumulative = 0;
	for (int s : typeList)
	{
		vertexArray.EnableAttribute(i);
		vertexArray.FormatAttribute(i, s, GL_FLOAT, false, sizeof(float) * sum, (void*)(cumulative * sizeof(float)));
		i++;
		cumulative += s;
	}
}

void Mesh::SetVertexBuffer(const void* data, unsigned int size)
{
	vertexBuffer = VertexBuffer(data, size);
}

void Mesh::SetIndexBuffer(const unsigned int* data, unsigned int count)
{
	indexBuffer = IndexBuffer(data, count);
}

void Mesh::DrawMesh() const
{
	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::SetMeshFromFile(const std::string& filepath)
{
	MeshData data = ReadMeshFile(filepath);
	ConstructBuffersFromMeshData(data);
	StandardVertexArray();
}

void Mesh::Bind() const
{
	vertexBuffer.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();
	material.Bind();
}

void Mesh::Unbind() const
{
	indexBuffer.Unbind();
	vertexBuffer.Unbind();
	vertexArray.Unbind();
	material.Unbind();
}

Vector3 Mesh::TriangleNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;
	return Vector3::Normalize(Vector3::Cross(v1 - v0, v2 - v0));
}

MeshData Mesh::ReadMeshFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;

	std::vector <Vector3> vertices;
	std::vector <Vector3> normals;
	std::vector <MeshFace> faces;

	while (getline(stream, line))
	{
		std::istringstream ss(line);
		std::string identifier;
		if (ss >> identifier)
		{
			// vertex
			if (identifier == "v")
			{
				Vector3 vert = Vector3();
				ss >> vert.x;
				ss >> vert.y;
				ss >> vert.z;
				vertices.push_back(vert);
			}

			// vertex normal
			else if (identifier == "vn")
			{
				Vector3 normal = Vector3();
				ss >> normal.x;
				ss >> normal.y;
				ss >> normal.z;
				normals.push_back(normal);
			}

			// face
			else if (identifier == "f")
			{
				MeshFace mf;
				for (int i = 0; i < 3; i++)
				{
					std::string vdatastr;
					ss >> vdatastr;
					std::vector <std::string> vdata;
					Split(vdatastr, "/", vdata);
					mf.v[i] = stoul(vdata[0]);
					mf.vt[i] = stoul(vdata[1]);
					mf.vn[i] = stoul(vdata[2]);
					//mf.n = normals[stoul(vdata[2])];
				}
				faces.push_back(mf);
			}
		}
	}
	std::cout << "read file" << std::endl;
	return ConsolidateVertexData(vertices, normals, &faces[0], faces.size());
}

MeshData Mesh::ConsolidateVertexData(
	const std::vector <Vector3>& vertices, 
	const std::vector <Vector3>& normals, 
	MeshFace* faces, unsigned int faceCount)
{
	static std::vector <VertexData> vertexData;
	static std::vector <unsigned int> faceData;

	// loop through each face
	for (unsigned int i = 0; i < faceCount; i++)
	{
		// loop through each vertex expression
		for (int j = 0; j < 3; j++)
		{
			VertexData cvd = VertexData();
			cvd.pos = vertices[faces[i].v[j] - 1];
			cvd.normal = TriangleNormal(vertices[faces[i].v[0] - 1], vertices[faces[i].v[1] - 1], vertices[faces[i].v[2] - 1]);

			std::vector<VertexData>::iterator found = std::find(vertexData.begin(), vertexData.end(), cvd);
			if (found == vertexData.end())
			{
				// if vertex doesnt exist already create new vertex and add vertex id to faceData
				vertexData.push_back(cvd);
				faceData.push_back(vertexData.size() - 1);
			}
			else
			{
				// else add existing vertex id to faceData
				faceData.push_back(found - vertexData.begin());
			}
		}
	}
	std::cout << "consolidated vertices" << std::endl;
	MeshData md = MeshData();
	md.vertices = vertexData.data();
	md.vertexCount = vertexData.size();
	md.indices = faceData.data();
	md.indiceCount = faceData.size();
	return md;
}

void Mesh::ConstructBuffersFromMeshData(const MeshData& meshData)
{
	std::vector<float> vBuffer;
	for (unsigned int i = 0; i < meshData.vertexCount; i++)
	{
		vBuffer.push_back(meshData.vertices[i].pos.x);
		vBuffer.push_back(meshData.vertices[i].pos.y);
		vBuffer.push_back(meshData.vertices[i].pos.z);
		vBuffer.push_back(meshData.vertices[i].normal.x);
		vBuffer.push_back(meshData.vertices[i].normal.y);
		vBuffer.push_back(meshData.vertices[i].normal.z);
	}

	md = MeshData(meshData);
	vvb = std::vector<float>(vBuffer);
	vertexBuffer = VertexBuffer(&vvb[0], vvb.size() * sizeof(float));
	indexBuffer = IndexBuffer(md.indices, md.indiceCount);
	std::cout << "made buffers" << std::endl;
}

void Mesh::StandardVertexArray()
{
	AddVertexAttribute(3); // position
	AddVertexAttribute(3); // normal
	SetVertexStructure();
}
