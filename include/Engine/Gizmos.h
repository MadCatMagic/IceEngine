#pragma once
#include "Core.h"
#include "Mesh/BasicMesh.h"
#include "Mesh/VertexBuffer.h"
#include "Mesh/IndexBuffer.h"

class Camera;

namespace Gizmos {
	class GizmosMesh : public BasicMesh 
	{
	public:
		inline GizmosMesh() { }
		GizmosMesh(const std::string& filepath);
		GizmosMesh(const GizmosMesh& obj);
		GizmosMesh(GizmosMesh&& obj) noexcept;
		~GizmosMesh();

		GizmosMesh& operator=(const GizmosMesh& obj);
		GizmosMesh& operator=(GizmosMesh&& obj) noexcept;

		void Draw() const;

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const;
	private:
		void CreateBuffers();

		VertexBuffer vb;
		IndexBuffer ib;
	};

	// set colour used to draw gizmos
	extern void SetColour(const Colour& col);
	// return colour used to draw gizmos
	extern Colour GetColour();

	// primitives
	// draws a solid cube at position centre with scale scale
	extern void DrawCube(const Vector3& centre, const Vector3& scale);
	// draws a solid sphere at position centre with a radius of radius
	extern void DrawSphere(const Vector3& centre, float radius);
	// wireframe versions
	// draws a wireframe cube at position centre with scale scale
	extern void DrawWireCube(const Vector3& centre, const Vector3& scale);
	// draws a wireframe sphere at position centre with a radius of radius
	extern void DrawWireSphere(const Vector3& centre, float radius);

	// similar
	// draws a line from point p1 to point p2
	extern void DrawLine(const Vector3& p1, const Vector3& p2);
	// draws a ray as either a Ray class or 
	extern void DrawRay(const Ray& ray, float length);


	// used by Renderer in render pipeline to set view and proj matrices
	extern void SetTargetCamera(Camera* cam);
	// sets up gizmos material and the primitive meshes
	extern void SetupGizmos();
	// releases all heap objects
	extern void Release();
}