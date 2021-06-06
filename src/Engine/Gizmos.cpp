#include "Engine/Gizmos.h"
#include "Engine/Mesh/Material.h"
#include "Engine/Mesh/VertexArray.h"
#include "Engine/Mesh/VertexBuffer.h"
#include "Engine/Mesh/IndexBuffer.h"
#include "Engine/Mesh/Mesh.h"

#include "Engine/Transform.h"
#include "Engine/Camera.h"

namespace Gizmos
{
	static Colour colour = Colour::white;

	// visuals
	static Shader* gizmosShader;
	static Material* gizmosMaterial;

	// for all data
	static VertexArray* gizmosVA;

	// cube data
	static VertexBuffer* gizmosVBCube;
	static IndexBuffer* gizmosIBCube;

	// sphere data
	static VertexBuffer* gizmosVBSphere;
	static IndexBuffer* gizmosIBSphere;

	// line data
	static VertexBuffer* gizmosLine;

	// gets set by SetCameraTarget();
	static Camera* cameraTarget{ nullptr };

	// forward declaring functions
	void DrawCubeGL();
	void DrawSphereGL();
	void SetMatUniforms(const Matrix4x4& modelMatrix);
	void GenerateVertexIndexData(const std::string& filepath, VertexBuffer** vb, IndexBuffer** ib);

	void SetColour(const Colour& col)
	{
		colour = col;
	}

	Colour GetColour()
	{
		return colour;
	}

	void DrawCube(const Vector3& centre, const Vector3& scale)
	{
		Transform t = Transform(centre, Quaternion::identity, scale / 2.0f);
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		DrawCubeGL();
	}

	void DrawSphere(const Vector3& centre, float radius)
	{
		Transform t = Transform(centre, Quaternion::identity, Vector3::one * (radius / 2.0f));
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		DrawSphereGL();
	}

	void DrawWireCube(const Vector3& centre, const Vector3& scale)
	{
		Transform t = Transform(centre, Quaternion::identity, scale / 2.0f);
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		Shader::ToggleWireframe(true);
		DrawCubeGL();
		Shader::ToggleWireframe(false);
	}

	void DrawWireSphere(const Vector3& centre, float radius)
	{
		Transform t = Transform(centre, Quaternion::identity, Vector3::one * (radius / 2.0f));
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		Shader::ToggleWireframe(true);
		DrawSphereGL();
		Shader::ToggleWireframe(false);
	}

	void DrawLine(const Vector3& p1, const Vector3& p2)
	{
		float data[12] { };
		data[0] = p1.x; data[1] =  p1.y; data[2] =  p1.z;
		data[3] = 0.0f; data[4] =  1.0f; data[5] =  0.0f;
		data[6] = p2.x; data[7] =  p2.y; data[8] =  p2.z;
		data[9] = 0.0f; data[10] = 1.0f; data[11] = 0.0f;
		gizmosLine->Bind();
		gizmosLine->ModifyData(data, 12 * sizeof(float), 0);

		SetMatUniforms(Matrix4x4::identity);

		gizmosVA->Bind();
		glDrawArrays(GL_LINES, 0, 2);
	}

	void DrawRay(const Ray& ray, float length)
	{
		DrawLine(ray.origin, ray.origin + ray.direction * length);
	}

	void DrawCubeGL()
	{
		gizmosVA->Bind();
		gizmosVBCube->Bind();
		gizmosIBCube->Bind();
		glDrawElements(GL_TRIANGLES, gizmosIBCube->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void DrawSphereGL()
	{
		gizmosVA->Bind();
		gizmosVBSphere->Bind();
		gizmosIBSphere->Bind();
		glDrawElements(GL_TRIANGLES, gizmosIBSphere->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void SetupGizmos()
	{
		gizmosShader = new Shader("res/shaders/Gizmos.shader");
		gizmosMaterial = new Material(*gizmosShader);

		// generating vertexdata for cube
		//GenerateVertexIndexData("res/models/cube.obj", &gizmosVBCube, &gizmosIBCube);

		// generating vertexdata for sphere
		GenerateVertexIndexData("res/models/icosphere.obj", &gizmosVBSphere, &gizmosIBSphere);

		// line data
		float nulldata[12]{ };
		gizmosLine = new VertexBuffer(nulldata, 12 * sizeof(float), VertexBuffer::UsageHint::DynamicDraw);

		gizmosVA = new VertexArray(true);
		gizmosVA->EnableAttribute(0);
		gizmosVA->EnableAttribute(1);
		gizmosVA->FormatAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
		gizmosVA->FormatAttribute(1, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	}

	void GenerateVertexIndexData(const std::string& filepath, VertexBuffer** vb, IndexBuffer** ib) 
	{
		MeshData md = Mesh::GetMeshDataFromFile(filepath);
		std::vector<float> floats = std::vector<float>(md.vertexCount * 6);
		for (unsigned int i = 0; i < md.vertexCount; i++)
		{
			floats[i * 6 + 0] = md.vertices[i].pos.x;
			floats[i * 6 + 1] = md.vertices[i].pos.y;
			floats[i * 6 + 2] = md.vertices[i].pos.z;
			floats[i * 6 + 3] = md.vertices[i].normal.x;
			floats[i * 6 + 4] = md.vertices[i].normal.y;
			floats[i * 6 + 5] = md.vertices[i].normal.z;
		}

		*vb = new VertexBuffer(&floats[0], floats.size() * sizeof(float));
		*ib = new IndexBuffer(md.indices, md.indiceCount);
	}

	void SetTargetCamera(Camera* target)
	{
		cameraTarget = target;
	}

	void SetMatUniforms(const Matrix4x4& modelMatrix)
	{
		gizmosMaterial->Bind();
		gizmosMaterial->SetMatrix4x4("projectionMatrix", cameraTarget->GetProjectionMatrix());
		gizmosMaterial->SetMatrix4x4("modelMatrix", modelMatrix);
		gizmosMaterial->SetVector3("diffuse", colour.ToVector3());
	}

	void Release()
	{
		delete gizmosShader;
		delete gizmosMaterial;

		delete gizmosVA;

		delete gizmosVBCube;
		delete gizmosIBCube;
		delete gizmosVBSphere;
		delete gizmosIBSphere;
		delete gizmosLine;
	}
}