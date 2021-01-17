#pragma once
#include "Quaternion.h"

class Entity;

struct Transform
{
	// references
	Entity* entity{ nullptr };
	Transform* parent{ nullptr };
	Transform* child{ nullptr };

	// transforms
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform();
	Transform(const Vector3& position);
	Transform(const Vector3& position, const Quaternion& rotation);
	Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
	~Transform();

	Vector3 Forward() const;
	Vector3 ForwardXZ() const;
	Vector3 Right() const;
	Vector3 RightXZ() const;
	Vector3 Up() const;

	void Rotate(const Vector3& euler);
	void Rotate(const Vector3& axis, float angle);
	void SetRotation(const Vector3& euler);
	void SetRotation(const Vector3& axis, float angle);

	void AssignParent(Transform* parent);
	void AssignChild(Transform* child);
	void AssignEntity(Entity* entity);

	inline bool operator==(const Transform& a) const { return position == a.position && rotation == a.rotation && scale == a.scale; }
	inline bool operator!=(const Transform& a) const { return !(*this == a); }
};