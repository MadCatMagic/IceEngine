#include "Engine/Gizmos.h"
#include "Engine/Mesh/Material.h"
#include "Engine/Mesh/VertexArray.h"

#include "Engine/Transform.h"
#include "Engine/Graphics/Camera.h"

namespace Gizmos
{
	static Colour colour = Colour::white;

	// visuals
	static Shader* gizmosShader;
	static Material* gizmosMaterial;

	// for all data
	static VertexArray* gizmosVA;

	// cube data
	static GizmosMesh cubeMesh;

	// sphere data
	static GizmosMesh sphereMesh;

	// line data
	static VertexBuffer* gizmosLine;

	// gets set by SetCameraTarget();
	static Camera* cameraTarget{ nullptr };

	// forward declaring functions
	void DrawCubeGL();
	void DrawSphereGL();
	void SetMatUniforms(const Matrix4x4& modelMatrix);

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
		gizmosVA->Bind();
		cubeMesh.Draw();
	}

	void DrawSphere(const Vector3& centre, float radius)
	{
		Transform t = Transform(centre, Quaternion::identity, Vector3::one * (radius / 2.0f));
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		gizmosVA->Bind();
		sphereMesh.Draw();
	}

	void DrawWireCube(const Vector3& centre, const Vector3& scale)
	{
		Transform t = Transform(centre, Quaternion::identity, scale / 2.0f);
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		Shader::ToggleWireframe(true);
		gizmosVA->Bind();
		cubeMesh.Draw();
		Shader::ToggleWireframe(false);
	}

	void DrawWireSphere(const Vector3& centre, float radius)
	{
		Transform t = Transform(centre, Quaternion::identity, Vector3::one * (radius / 2.0f));
		Matrix4x4 modelMatrix = t.TransformationMatrix();

		SetMatUniforms(modelMatrix);
		Shader::ToggleWireframe(true);
		gizmosVA->Bind();
		//int val;
		//std::cout << "ast" << std::endl;
		//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &val);
		//std::cout << val << std::endl;
		sphereMesh.Draw();
		//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &val);
		//std::cout << val << std::endl;
		Shader::ToggleWireframe(false);
	}

	void DrawLine(const Vector3& p1, const Vector3& p2)
	{
		float dist = Vector3::Distance(p1, p2);
		Matrix4x4 modelMatrix = 
			Matrix4x4::PointAt(p1, p2, Vector3(0.0f, 1.0f, 0.0f)) * 
			Matrix4x4(Vector4(dist, dist, dist, 1));
		SetMatUniforms(modelMatrix);

		gizmosLine->Bind();
		gizmosVA->Bind();
		glDrawArrays(GL_LINES, 0, 2);
	}

	void DrawRay(const Ray& ray, float length)
	{
		DrawLine(ray.origin, ray.origin + ray.direction * length);
	}

	// line data
	const float linedata[12]{
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	void SetupGizmos()
	{
		gizmosShader = new Shader("res/shaders/Gizmos.shader");
		gizmosMaterial = new Material(*gizmosShader);

		// generating vertexdata for cube
		cubeMesh = GizmosMesh("res/models/cube.obj");

		// generating vertexdata for sphere
		sphereMesh = GizmosMesh("res/models/icosphere.obj");
		cubeMesh.Bind();

		//gizmosLine = new VertexBuffer(linedata, 12 * sizeof(float), VertexBuffer::UsageHint::DynamicDraw);

		gizmosVA = new VertexArray(true);
		gizmosVA->EnableAttribute(0);
		gizmosVA->EnableAttribute(1);
		gizmosVA->FormatAttribute(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
		gizmosVA->FormatAttribute(1, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 3));
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
		delete gizmosLine;
	}

	// gizmos class stuff

	GizmosMesh::GizmosMesh(const std::string& filepath)
	{
		SetMeshFromFile(filepath);
		CreateBuffers();
	}

	GizmosMesh::GizmosMesh(const GizmosMesh& obj)
	{
		ib = obj.ib;
		vb = obj.vb;
		md = obj.md;
	}

	GizmosMesh::GizmosMesh(GizmosMesh&& obj) noexcept
	{
		ib = obj.ib;
		obj.ib = IndexBuffer();
		vb = obj.vb;
		obj.vb = VertexBuffer();
		md = obj.md;
		obj.md = MeshData();
	}

	GizmosMesh::~GizmosMesh()
	{
	}

	GizmosMesh& GizmosMesh::operator=(const GizmosMesh& obj)
	{
		ib = obj.ib;
		vb = obj.vb;
		md = obj.md;
		return *this;
	}

	GizmosMesh& GizmosMesh::operator=(GizmosMesh&& obj) noexcept
	{
		ib = obj.ib;
		obj.ib = IndexBuffer();
		vb = obj.vb;
		obj.vb = VertexBuffer();
		md = obj.md;
		obj.md = MeshData();
		return *this;
	}

	void GizmosMesh::Draw() const
	{
		Bind();
		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void GizmosMesh::Bind() const
	{
		vb.Bind();
		ib.Bind();
	}

	void GizmosMesh::Unbind() const
	{
		vb.Unbind();
		ib.Unbind();
	}

	unsigned int GizmosMesh::GetCount() const
	{
		return ib.GetCount();
	}

	void GizmosMesh::CreateBuffers()
	{
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

		vb.SetData(&floats[0], floats.size() * sizeof(float));
		ib = IndexBuffer(md.indices, md.indiceCount);
	}
}