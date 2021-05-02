#include "Transform.h"
#include "Entity.h"

Transform::Transform()
{
    this->localScale = Vector3::one;
    GlobalFromLocalTransform();
}

Transform::Transform(const Vector3& position)
{
    this->localPosition = position;
    this->localScale = Vector3::one;
    GlobalFromLocalTransform();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation)
{
    this->localPosition = position;
    this->localRotation = rotation;
    this->localScale = Vector3::one;
    GlobalFromLocalTransform();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    this->localPosition = position;
    this->localRotation = rotation;
    this->localScale = scale;
    GlobalFromLocalTransform();
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

Matrix4x4 Transform::TransformationMatrix()
{
    Matrix4x4 pos = Matrix4x4::identity;
    pos.m[3] = position.x; pos.m[7] = position.y; pos.m[11] = position.z;
    Matrix4x4 scale = Matrix4x4(Vector4(globalScale.x, globalScale.y, globalScale.z, 1));
    Matrix4x4 rot = rotation.GetRotationMatrix();
    
    return scale;
}

void Transform::Move(const Vector3& move)
{
    localPosition += move;
    GlobalFromLocalPos();
}

void Transform::SetLocalPos(const Vector3& pos)
{
    localPosition = pos;
    GlobalFromLocalPos();
}

void Transform::SetPos(const Vector3& pos)
{
    position = pos;
    LocalFromGlobalPos();
}

Vector3 Transform::GetLocalPos() const
{
    return localPosition;
}

Vector3 Transform::GetPos() const
{
    return position;
}

void Transform::Rotate(const Vector3& euler)
{
    localRotation *= Quaternion(euler);
    localRotation = Quaternion::Normalize(localRotation);
    GlobalFromLocalRot();
}

void Transform::Rotate(const Vector3& axis, float angle)
{
    localRotation *= Quaternion(axis, angle);
    localRotation = Quaternion::Normalize(localRotation);
    GlobalFromLocalRot();
}

void Transform::SetLocalRot(const Quaternion& quat)
{
    localRotation = quat;
    GlobalFromLocalRot();
}

void Transform::SetRot(const Quaternion& quat)
{
    localRotation = quat;
    LocalFromGlobalRot();
}

Quaternion Transform::GetLocalRot() const
{
    return localRotation;
}

Quaternion Transform::GetRot() const
{
    return rotation;
}

void Transform::SetLocalScale(const Vector3& scale)
{
    localScale = scale;
    GlobalFromLocalScale();
}

void Transform::SetGlobalScale(const Vector3& scale)
{
    globalScale = scale;
    LocalFromGlobalScale();
}

Vector3 Transform::GetLocalScale() const
{
    return localScale;
}

Vector3 Transform::GetGlobalScale() const
{
    return globalScale;
}

void Transform::AssignParent(Transform* parent)
{
    this->parent = parent;
    parent->child = this;
    GlobalFromLocalTransform();
}

void Transform::AssignChild(Transform* child)
{
    this->child = child;
    child->parent = this;
    child->GlobalFromLocalTransform();
}

void Transform::AssignEntity(Entity* entity)
{
    this->entity = entity;
    entity->transform = this;
}

void Transform::GlobalFromLocalPos()
{
    if (parent != nullptr)
        position = parent->position + localPosition;
    else
        position = localPosition;
    if (child != nullptr)
        child->GlobalFromLocalPos();
}

void Transform::GlobalFromLocalRot()
{
    if (parent != nullptr)
    {
        rotation = parent->rotation * localRotation;
        rotation = Quaternion::Normalize(rotation);
    }
    else
        rotation = localRotation;
    if (child != nullptr)
        child->GlobalFromLocalRot();
}

void Transform::GlobalFromLocalScale()
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
        child->GlobalFromLocalScale();
}

void Transform::GlobalFromLocalTransform()
{
    GlobalFromLocalPos();
    GlobalFromLocalRot();
    GlobalFromLocalScale();
}

void Transform::LocalFromGlobalPos()
{
    if (parent != nullptr)
        localPosition = position - parent->position;
    else
        localPosition = position;
    if (child != nullptr)
        child->GlobalFromLocalPos();
}

void Transform::LocalFromGlobalRot()
{
    if (parent != nullptr)
        localRotation = rotation / parent->rotation;
    else
        localRotation = rotation;
    if (child != nullptr)
        child->GlobalFromLocalRot();
}

void Transform::LocalFromGlobalScale()
{
    if (parent != nullptr)
    {
        localScale.x = globalScale.x / parent->globalScale.x;
        localScale.y = globalScale.y / parent->globalScale.y;
        localScale.z = globalScale.z / parent->globalScale.z;
    }
    else
        localScale = globalScale;
    if (child != nullptr)
        child->GlobalFromLocalScale();
}

void Transform::LocalFromGlobalTransform()
{
    LocalFromGlobalPos();
    LocalFromGlobalRot();
    LocalFromGlobalScale();
}
