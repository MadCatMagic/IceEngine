#include "Engine/Light.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Gizmos.h"

Light::Light()
{
	lights.push_back(this);
}

Light::~Light()
{
	std::vector<Light*>::iterator index = std::find(lights.begin(), lights.end(), this);
	lights.erase(index);

	DeleteTexture();
}

Matrix4x4 Light::GetMatrix()
{
	// sun matrices have an orthographic matrix and face down
	if (type == Type::Sun)
		return 
			Matrix4x4::OrthoMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 20.0f, -20.0f) * 
			Matrix4x4::ViewMatrix(Renderer::GetMainCamera()->transform->GetPos(), -transform->Up(), Vector3(0.0f, 1.0f, 0.0f));

	// spot matrices use a projection matrix and face forward
	else if (type == Type::Spot)
		return 
			Matrix4x4::ProjectionMatrix(DegreesToRadii(90), 0.1f, 50.0f, 1.0f) *
			Matrix4x4::ViewMatrix(transform->GetPos(), -transform->Forward(), Vector3(0.0f, 1.0f, 0.0f));

	return Matrix4x4::identity;
}

void Light::GenerateTexture()
{
	texture = new RenderTexture(SHADOWMAP_SIZE, SHADOWMAP_SIZE, Texture::Format::Depth16, Texture::Format::None);
	texture->GenerateBuffers();
	texture->depthBuffer->wrapMode = Texture::WrapMode::Border;
	if (type == Type::Sun)
		texture->depthBuffer->SetBorderColour(Vector4::one);
	else
		texture->depthBuffer->SetBorderColour(Vector4::zero);
	texture->depthBuffer->Bind();
	texture->depthBuffer->ApplyFiltering();
}

void Light::DeleteTexture()
{
	// delete texture
	if (texture)
		delete(texture);
	texture = nullptr;
}

Vector3 Light::GetLightDir() const
{
	return type == Type::Sun ? -transform->Up() : transform->Forward();
}

void Light::Update()
{
}

void Light::OnDrawGizmos()
{
	Gizmos::SetColour(colour);
	if (type != Type::Sun)
	{
		Gizmos::DrawWireSphere(transform->GetPos(), 0.2f);
		Gizmos::DrawWireCube(transform->GetPos() + Vector3::one * 0.2f, Vector3::one * 0.2f);
		//Gizmos::DrawLine(transform->GetPos(), transform->GetPos() + GetLightDir() * 0.5f);
	}
	else
		Gizmos::DrawWireCube(transform->GetPos() + Vector3::one * 0.2f, Vector3::one * 0.4f);
}

std::vector<Light*> Light::lights = std::vector<Light*>();