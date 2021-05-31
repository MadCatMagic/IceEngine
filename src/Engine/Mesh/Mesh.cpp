#include <fstream>
#include <sstream>

#include "Engine/Mesh.h"
#include "Engine/Core.h"

struct VertexData {
	Vector3 pos;
	Vector3 normal;
	Vector2 texture;
	Vector3 tangent;

	bool operator==(const VertexData& v2)
	{
		return
			pos == v2.pos &&
			normal == v2.normal &&
			texture == v2.texture &&
			tangent == v2.tangent;
	}
};

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
}

void Mesh::Unbind() const
{
	indexBuffer.Unbind();
	vertexBuffer.Unbind();
	vertexArray.Unbind();
}

Vector3 Mesh::TriangleNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	return Vector3::Normalize(Vector3::Cross(v1 - v0, v2 - v0));
}

Vector3 Mesh::Tangent(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
{
	Vector3 edge1 = pos2 - pos1;
	Vector3 edge2 = pos3 - pos1;
	Vector2 deltaUV1 = uv2 - uv1;
	Vector2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	return Vector3(
		f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
		f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
		f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
	);
}

MeshData Mesh::ReadMeshFile(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;

	std::vector <Vector3> vertices;
	std::vector <Vector3> normals;
	std::vector <Vector2> texUVs;
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

			// texture coord
			else if (identifier == "vt")
			{
				Vector2 coord = Vector2();
				ss >> coord.x;
				ss >> coord.y;
				texUVs.push_back(coord);
			}

			// face
			else if (identifier == "f")
			{
				MeshFace mf = MeshFace();
				for (int i = 2; i >= 0; i--)
				{
					std::string vdatastr;
					ss >> vdatastr;
					std::vector <std::string> vdata;
					StrSplit(vdatastr, "/", vdata);
					mf.v[i] = stoul(vdata[0]);
					mf.vt[i] = stoul(vdata[1]);
					mf.vn[i] = stoul(vdata[2]);
					//mf.n = normals[stoul(vdata[2])];
				}
				faces.push_back(mf);
			}
		}
	}
	stream.close();
	return ConsolidateVertexData(vertices, normals, texUVs, &faces[0], faces.size());
}

MeshData Mesh::ConsolidateVertexData(
	const std::vector <Vector3>& vertices, 
	const std::vector <Vector3>& normals, 
	const std::vector <Vector2>& texUVs,
	MeshFace* faces, unsigned int faceCount)
{
	static std::vector <VertexData> vertexData;
	static std::vector <unsigned int> faceData;
	vertexData.clear();
	faceData.clear();

	// loop through each face
	for (unsigned int i = 0; i < faceCount; i++)
	{
		// loop through each vertex expression
		for (int j = 0; j < 3; j++)
		{
			VertexData cvd = VertexData();
			cvd.pos = vertices[faces[i].v[j] - 1];
			cvd.normal = TriangleNormal(vertices[faces[i].v[0] - 1], vertices[faces[i].v[1] - 1], vertices[faces[i].v[2] - 1]);
			cvd.texture = texUVs[faces[i].vt[j] - 1];
			cvd.tangent = Tangent(
				vertices[faces[i].v[0] - 1], vertices[faces[i].v[1] - 1], vertices[faces[i].v[2] - 1],
				texUVs[faces[i].vt[0] - 1], texUVs[faces[i].vt[1] - 1], texUVs[faces[i].vt[2] - 1]
			);

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
	MeshData md = MeshData();
	md.vertices = vertexData.data();
	md.vertexCount = vertexData.size();
	md.indices = faceData.data();
	md.indiceCount = faceData.size();
	return md;
}

void Mesh::ConstructBuffersFromMeshData(const MeshData& meshData)
{
	std::vector<float> vBuffer = std::vector<float>(meshData.vertexCount * 11);
	for (unsigned int i = 0; i < meshData.vertexCount; i++)
	{
		// position : 0
		vBuffer[i * 11 + 0] = meshData.vertices[i].pos.x;
		vBuffer[i * 11 + 1] = meshData.vertices[i].pos.y;
		vBuffer[i * 11 + 2] = meshData.vertices[i].pos.z;
		// normal : 1
		vBuffer[i * 11 + 3] = meshData.vertices[i].normal.x;
		vBuffer[i * 11 + 4] = meshData.vertices[i].normal.y;
		vBuffer[i * 11 + 5] = meshData.vertices[i].normal.z;
		// texCoord : 2
		vBuffer[i * 11 + 6] = meshData.vertices[i].texture.x;
		vBuffer[i * 11 + 7] = meshData.vertices[i].texture.y;
		// tangent : 3
		vBuffer[i * 11 + 8] = meshData.vertices[i].tangent.x;
		vBuffer[i * 11 + 9] = meshData.vertices[i].tangent.y;
		vBuffer[i * 11 + 10] = meshData.vertices[i].tangent.z;
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
