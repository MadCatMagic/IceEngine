#include "Transform.h"
#include "Entity.h"

Transform::Transform()
{
    this->scale = Vector3::one;
}

Transform::Transform(const Vector3& position)
{
    this->position = position;
    this->scale = Vector3::one;
}

Transform::Transform(const Vector3& position, const Quaternion& rotation)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = Vector3::one;
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Transform::~Transform() { }

Vector3 Transform::Forward() const
{
    return Vector3::Normalize(rotation.RotateVector(Vector3(0.0f, 0.0f, 1.0f)));
}

Vector3 Transform::ForwardXZ() const
{
    Vector3 rotated = rotation.RotateVector(Vector3(0.0f, 0.0f, 1.0f));
    rotated.y = 0.0f;
    return Vector3::Normalize(rotated);
}

Vector3 Transform::Right() const
{
    return Vector3::Normalize(rotation.RotateVector(Vector3(-1.0f, 0.0f, 0.0f)));
}

Vector3 Transform::RightXZ() const
{
    Vector3 rotated = rotation.RotateVector(Vector3(-1.0f, 0.0f, 0.0f));
    rotated.y = 0.0f;
    return Vector3::Normalize(rotated);
}

Vector3 Transform::Up() const
{
    return rotation.RotateVector(Vector3(0.0f, 1.0f, 0.0f));
}

void Transform::Rotate(const Vector3& euler)
{
    rotation *= Quaternion(euler);
}

void Transform::Rotate(const Vector3& axis, float angle)
{
    rotation *= Quaternion(axis, angle);
}

void Transform::SetRotation(const Vector3& euler)
{
    rotation = Quaternion(euler);
}

void Transform::SetRotation(const Vector3& axis, float angle)
{
    rotation = Quaternion(axis, angle);
}

void Transform::AssignParent(Transform* parent)
{
    this->parent = parent;
    parent->child = this;
}

void Transform::AssignChild(Transform* child)
{
    this->child = child;
    child->parent = this;
}

void Transform::AssignEntity(Entity* entity)
{
    this->entity = entity;
    entity->transform = this;
}
