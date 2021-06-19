#pragma once
#include "Core/Quaternion.h"

class Entity;

// contains data about the parenting of an object, the position, rotation, and scale of an object, hidden behind functions to allow for dynamic updating of the local or global variant
struct Transform
{
	// references
	Entity* entity{ nullptr };    // do not directly assign to without UpdateGlobalTransform() after
	Transform* parent{ nullptr }; // do not directly assign to without UpdateGlobalTransform() after
	Transform* child{ nullptr };  // do not directly assign to without UpdateGlobalTransform() after

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

	Matrix4x4 TransformationMatrix();

	void Move(const Vector3& move);
	void SetLocalPos(const Vector3& pos);
	void SetPos(const Vector3& pos);
	Vector3 GetLocalPos() const;
	Vector3 GetPos() const;

	void Rotate(const Vector3& euler);
	void Rotate(const Vector3& axis, float angle);
	void SetLocalRot(const Quaternion& quat);
	void SetRot(const Quaternion& quat);
	Quaternion GetLocalRot() const;
	Quaternion GetRot() const;

	void SetLocalScale(const Vector3& scale);
	void SetGlobalScale(const Vector3& scale);
	Vector3 GetLocalScale() const;
	Vector3 GetGlobalScale() const;

	void AssignParent(Transform* parent);
	void AssignChild(Transform* child);
	void AssignEntity(Entity* entity);

	inline bool operator==(const Transform& a) const 
	{ 
		return 
			localPosition == a.localPosition && 
			localRotation == a.localRotation && 
			localScale == a.localScale && 
			position == a.position && 
			rotation == a.rotation && 
			globalScale == a.globalScale &&
			parent == a.parent &&
			child == a.child &&
			entity == a.entity; 
	}
	inline bool operator!=(const Transform& a) const { return !(*this == a); }

private:
	// transforms
	Vector3 localPosition;
	Vector3 position;
	Quaternion localRotation{};
	Quaternion rotation{};
	Vector3 localScale;
	Vector3 globalScale;

	void GlobalFromLocalPos();
	void GlobalFromLocalRot();
	void GlobalFromLocalScale();
	void GlobalFromLocalTransform();

	void LocalFromGlobalPos();
	void LocalFromGlobalRot();
	void LocalFromGlobalScale();
	void LocalFromGlobalTransform();
};