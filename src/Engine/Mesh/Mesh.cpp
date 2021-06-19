#include <fstream>
#include <sstream>

#include "Engine/Mesh.h"

Mesh::Mesh(VertexBuffer& vb, IndexBuffer& ib)
	: vertexBuffer(vb), indexBuffer(ib)
{
	vertexArray.Construct(); 
	// obscure error here - if you use a constructor here like VertexArray()
	// it will not set correctly and will cause obscure errors later on when attempting to bind
	// just use default vertexArray and construct it
	vertexSize = 0;
}

Mesh::Mesh(const std::string& filepath)
{
	vertexArray.Construct();
	vertexSize = 0;
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
	unsigned int sum = Sum<int, std::list<int>>(typeList);
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

void Mesh::SetMeshFromFile(const std::string& filepath, bool generateICEMFile)
{
	std::vector<std::string> split;
	StrSplit(filepath, ".", split);
	if (split.back() == "obj") 
	{
		MeshData data = ReadMeshFileOBJ(filepath);
		ConstructBuffersFromMeshData(data);
		
		if (generateICEMFile)
		{
			ustring charData = CompileICEMFile(data);
			WriteICEMFile(charData, filepath + ".icem");
		}
	}
	else if (split.back() == "icem")
	{
		MeshData data = ReadMeshFileICEM(filepath);
		ConstructBuffersFromMeshData(data);
	}

	StandardVertexArray();
}

void Mesh::Bind() const
{
	vertexBuffer.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();
}

void Mesh::Unbind() const
{
	indexBuffer.Unbind();
	vertexBuffer.Unbind();
	vertexArray.Unbind();
}

MeshData Mesh::GetMeshDataFromFile(const std::string& filepath)
{
	std::vector<std::string> split;
	StrSplit(filepath, ".", split);
	MeshData data = MeshData();
	if (split.back() == "obj")
	{
		data = ReadMeshFileOBJ(filepath);
		// testing .icem
		// always generates file
		ustring charData = CompileICEMFile(data);
		WriteICEMFile(charData, filepath + ".icem");
	}
	else if (split.back() == "icem")
	{
		data = ReadMeshFileICEM(filepath);
	}
	return data;
}

void Mesh::ConstructBuffersFromMeshData(const MeshData& meshData)
{
	std::vector<float> vBuffer = std::vector<float>(meshData.vertexCount * VERTEXDATA_SIZE);
	for (unsigned int i = 0; i < meshData.vertexCount; i++)
	{
		// position : 0
		vBuffer[i * VERTEXDATA_SIZE + 0] = meshData.vertices[i].pos.x;
		vBuffer[i * VERTEXDATA_SIZE + 1] = meshData.vertices[i].pos.y;
		vBuffer[i * VERTEXDATA_SIZE + 2] = meshData.vertices[i].pos.z;
		// normal : 1
		vBuffer[i * VERTEXDATA_SIZE + 3] = meshData.vertices[i].normal.x;
		vBuffer[i * VERTEXDATA_SIZE + 4] = meshData.vertices[i].normal.y;
		vBuffer[i * VERTEXDATA_SIZE + 5] = meshData.vertices[i].normal.z;
		// texCoord : 2
		vBuffer[i * VERTEXDATA_SIZE + 6] = meshData.vertices[i].texture.x;
		vBuffer[i * VERTEXDATA_SIZE + 7] = meshData.vertices[i].texture.y;
		// tangent : 3
		vBuffer[i * VERTEXDATA_SIZE + 8] = meshData.vertices[i].tangent.x;
		vBuffer[i * VERTEXDATA_SIZE + 9] = meshData.vertices[i].tangent.y;
		vBuffer[i * VERTEXDATA_SIZE + 10] = meshData.vertices[i].tangent.z;
	}
	
	md = MeshData(meshData);
	vertexBuffer = VertexBuffer(&vBuffer[0], vBuffer.size() * sizeof(float));
	indexBuffer = IndexBuffer(md.indices, md.indiceCount);
}

void Mesh::StandardVertexArray()
{
	AddVertexAttribute(3); // position
	AddVertexAttribute(3); // normal
	AddVertexAttribute(2); // texCoord
	AddVertexAttribute(3); // tangent
	SetVertexStructure();
}