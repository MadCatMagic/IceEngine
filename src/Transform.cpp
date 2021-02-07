#include "Transform.h"
#include "Entity.h"

Transform::Transform()
{
    this->localScale = Vector3::one;
    UpdateGlobalTransform();
}

Transform::Transform(const Vector3& position)
{
    this->localPosition = position;
    this->localScale = Vector3::one;
    UpdateGlobalTransform();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation)
{
    this->localPosition = position;
    this->localRotation = rotation;
    this->localScale = Vector3::one;
    UpdateGlobalTransform();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    this->localPosition = position;
    this->localRotation = rotation;
    this->localScale = scale;
    UpdateGlobalTransform();
}

Transform::~Transform() { this->parent->child = nullptr; this->child->parent = nullptr; }

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
    localRotation *= Quaternion(euler);
    localRotation = Quaternion::Normalize(localRotation);
    UpdateGlobalRot();
}

void Transform::Rotate(const Vector3& axis, float angle)
{
    localRotation *= Quaternion(axis, angle);
    localRotation = Quaternion::Normalize(localRotation);
    UpdateGlobalRot();
}

void Transform::SetRotation(const Vector3& euler)
{
    localRotation = Quaternion(euler);
    localRotation = Quaternion::Normalize(localRotation);
    UpdateGlobalRot();
}

void Transform::SetRotation(const Vector3& axis, float angle)
{
    localRotation = Quaternion(axis, angle);
    localRotation = Quaternion::Normalize(localRotation);
    UpdateGlobalRot();
}

void Transform::AssignParent(Transform* parent)
{
    this->parent = parent;
    parent->child = this;
    UpdateGlobalTransform();
}

void Transform::AssignChild(Transform* child)
{
    this->child = child;
    child->parent = this;
    child->UpdateGlobalTransform();
}

void Transform::AssignEntity(Entity* entity)
{
    this->entity = entity;
    entity->transform = this;
}

void Transform::UpdateGlobalPos()
{
    if (parent != nullptr)
        position = parent->position + localPosition;
    else
        position = localPosition;
    if (child != nullptr)
        child->UpdateGlobalPos();
}

void Transform::UpdateGlobalRot()
{
    if (parent != nullptr)
    {
        rotation = parent->rotation * localRotation;
        rotation = Quaternion::Normalize(rotation);
    }
    else
        rotation = localRotation;
    if (child != nullptr)
        child->UpdateGlobalRot();
}

void Transform::UpdateGlobalScale()
{
    if (parent != nullptr)
    {
        globalScale.x = parent->globalScale.x * localScale.x;
        globalScale.y = parent->globalScale.y * localScale.y;
        globalScale.z = parent->globalScale.z * localScale.z;
    }
    else
        globalScale = localScale;
    if (child != nullptr)
        child->UpdateGlobalScale();
}

void Transform::UpdateGlobalTransform()
{
    UpdateGlobalPos();
    UpdateGlobalRot();
    UpdateGlobalScale();
}
