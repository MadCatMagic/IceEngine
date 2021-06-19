#include "Engine/Mesh/BasicMesh.h"

#include <fstream>
#include <sstream>

Vector3 BasicMesh::TriangleNormal(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	return Vector3::Normalize(Vector3::Cross(v1 - v0, v2 - v0));
}

Vector3 BasicMesh::Tangent(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3, const Vector2& uv1, const Vector2& uv2, const Vector2& uv3)
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

MeshData BasicMesh::ReadMeshFile(const std::string& filepath, bool generateICEMFile)
{
	MeshData data;

	std::vector<std::string> split;
	StrSplit(filepath, ".", split);

	// read meshdata
	if (split.back() == "obj")
		data = ReadMeshFileOBJ(filepath);
	else if (split.back() == "icem")
		data = ReadMeshFileICEM(filepath);

	// generate icem file 
	if (generateICEMFile && split.back() != "icem")
	{
		ustring charData = CompileICEMFile(data);
		WriteICEMFile(charData, filepath + ".icem");
	}

	return data;
}

MeshData BasicMesh::ReadMeshFileOBJ(const std::string& filepath)
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
				}
				faces.push_back(mf);
			}
		}
	}
	stream.close();
	return ConsolidateVertexData(vertices, normals, texUVs, &faces[0], faces.size());
}

MeshData BasicMesh::ConsolidateVertexData(
	const std::vector <Vector3>& vertices,
	const std::vector <Vector3>& normals,
	const std::vector <Vector2>& texUVs,
	MeshFace* faces, unsigned int faceCount)
{
	std::vector <VertexData> vertexData;
	std::vector <unsigned int> faceData;

	// loop through each face
	for (unsigned int i = 0; i < faceCount; i++)
	{
		// loop through each vertex expression
		for (int j = 0; j < 3; j++)
		{
			VertexData cvd = VertexData();
			cvd.pos = vertices[faces[i].v[j] - 1];
			cvd.normal = -normals[faces[i].vn[j] - 1];//TriangleNormal(vertices[faces[i].v[0] - 1], vertices[faces[i].v[1] - 1], vertices[faces[i].v[2] - 1]);
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

	// evil data hack to convert to heap allocated data
	VertexData* vertexPointer = new VertexData[vertexData.size()]();
	memcpy((void*)vertexPointer, (void*)vertexData.data(), vertexData.size() * VERTEXDATA_SIZE * 4);
	unsigned int* indexPointer = new unsigned int[faceData.size()];
	memcpy((void*)indexPointer, (void*)faceData.data(), faceData.size() * 4);

	return { vertexPointer, vertexData.size(), indexPointer, faceData.size(), true };
}


MeshData BasicMesh::ReadMeshFileICEM(const std::string& filepath)
{
	std::ifstream readStream = std::ifstream(filepath, std::ios::in | std::ios::binary);

	// read and convert the vertexCount and vertexData from the first 8 bytes in the file
	// gets reused so only 4 bytes
	unsigned char* byteBuffer = new unsigned char[4];

	// vertex count
	readStream.read((char*)byteBuffer, 4);
	unsigned int vertexCount = ustringTob4<unsigned int>(ustring(byteBuffer, 4));

	// indice count
	readStream.read((char*)byteBuffer, 4);
	unsigned int indexCount = ustringTob4<unsigned int>(ustring(byteBuffer, 4));

	// destroy unused byteBuffer now
	delete[] byteBuffer;

	// read vertexData
	unsigned char* vertexData = new unsigned char[vertexCount * 4 * VERTEXDATA_SIZE];
	// 4 bytes per value and VERTEXDATA_SIZE values per vertex
	// read takes an input of (long long) and visual studio complains if its not explicitly cast to
	readStream.read((char*)vertexData, (long long)vertexCount * 4 * VERTEXDATA_SIZE);

	// get that vertexData back
	std::vector<VertexData> vertexArray(vertexCount);
	for (unsigned int i = 0; i < vertexCount; i++)
	{
		VertexData vertex = VertexData();
		// loop through and retrieve each value
		for (int j = 0; j < VERTEXDATA_SIZE; j++)
		{
			// put 4 bytes into ustring
			ustring str = ustring(&vertexData[(i * VERTEXDATA_SIZE + j) * 4], 4);
			// convert to value
			float val = ustringTob4<float>(str);
			// assign to meshdata
			vertex.DirectIndex(j) = val;
		}
		vertexArray[i] = vertex;
	}
	// horrible memory hack to heap allocated object
	VertexData* vertexPointer = new VertexData[vertexCount]();
	memcpy((void*)vertexPointer, (void*)vertexArray.data(), vertexCount * VERTEXDATA_SIZE * 4);

	// read indexData
	unsigned char* indexData = new unsigned char[indexCount * 4];
	// 4 bytes per value
	// read takes an input of (long long) and visual studio complains if its not explicitly cast to
	readStream.read((char*)indexData, (long long)indexCount * 4);

	// get that indexData back
	std::vector<unsigned int> indexArray(indexCount);
	for (unsigned int i = 0; i < indexCount; i++)
	{
		// put 4 bytes into ustring
		ustring str = ustring(&indexData[i * 4], 4);
		// convert to value
		unsigned int val = ustringTob4<unsigned int>(str);
		// assign to meshdata
		indexArray[i] = val;
	}
	// horrible memory hack to heap allocated object
	unsigned int* indexPointer = new unsigned int[indexCount];
	memcpy((void*)indexPointer, (void*)indexArray.data(), indexCount * 4);

	readStream.close();
	return { vertexPointer, vertexCount, indexPointer, indexCount, true };
}

/* Format
vertexCount
indiceCount
vertices
indices
*/

ustring BasicMesh::CompileICEMFile(const MeshData& meshData)
{
	ustring returnStr;

	// vertexCount
	returnStr += b4Toustring<unsigned int>(meshData.vertexCount);
	// indexCount
	returnStr += b4Toustring<unsigned int>(meshData.indiceCount);

	// loop through each vertex
	for (unsigned int i = 0; i < meshData.vertexCount; i++)
	{
		// loop through every value in the vertex
		for (int j = 0; j < VERTEXDATA_SIZE; j++)
		{
			float val = meshData.vertices[i].DirectIndex(j);
			returnStr += b4Toustring<float>(val);
		}
	}

	// loop through each index
	for (unsigned int i = 0; i < meshData.indiceCount; i++)
	{
		unsigned int val = meshData.indices[i];
		returnStr += b4Toustring<unsigned int>(val);
	}

	return returnStr;
}

void BasicMesh::WriteICEMFile(const ustring& data, const std::string& filepath)
{
	std::ofstream writeStream = std::ofstream(filepath, std::ios::out | std::ios::trunc | std::ios::binary);
	writeStream.write((char*)data.data(), data.size());
	writeStream.close();
}