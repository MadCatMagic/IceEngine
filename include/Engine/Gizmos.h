#pragma once
#include "Core.h"

class Camera;

namespace Gizmos {
	extern void SetColour(const Colour& col);
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


	// used by Renderer in render pipeline
	extern void SetTargetCamera(Camera* cam);
	// sets up gizmos material and the primitive meshes
	extern void SetupGizmos();
	// releases all heap objects
	extern void Release();
}