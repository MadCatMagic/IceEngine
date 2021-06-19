#include "TemporalReprojection.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Mesh/Material.h"
#include "Engine/Graphics/Camera.h"

TemporalReprojection::TemporalReprojection()
{
}

TemporalReprojection::~TemporalReprojection()
{
	delete previousFrame;
	delete reprojectionShader;
	delete reprojectionMaterial;
}

void TemporalReprojection::OnPostProcess(RenderTexture* src, RenderTexture* dest)
{
	Matrix4x4 camMatrix = camera->GetProjectionMatrix();
	// never apply on first frame
	if (notFirstFrame)
	{
		// note that Blit uses texture units 0 and 1
		reprojectionMaterial->Bind();
		Texture::BindTextureUnit(2);
		previousFrame->colourBuffer->Bind();
		reprojectionMaterial->SetTexture("previousFrame", 2);
		Texture::BindTextureUnit(3);
		previousFrame->depthBuffer->Bind();
		reprojectionMaterial->SetTexture("previousFrameDepth", 3);

		reprojectionMaterial->SetMatrix4x4("previousCamMatrix", previousCamMatrix);
		reprojectionMaterial->SetMatrix4x4("camMatrixInv", camMatrix.GetInverse());

		reprojectionMaterial->SetFloat("nearPlane", camera->zNear);
		reprojectionMaterial->SetFloat("farPlane", camera->zFar);

		Vector3 jitter = (Vector3(
			(float)(rand() % 1000),
			(float)(rand() % 1000),
			(float)(rand() % 1000))
			* 0.002f // scale to 0, 2
			- Vector3::one) // scale to -1, 1
			* 0.001f; // scale to centimetres

		reprojectionMaterial->SetVector3("cameraJitter", jitter);

		Renderer::Blit(*src, *dest, reprojectionMaterial, true);
	}
	else
	{
		notFirstFrame = true;
		Renderer::Blit(*src, *dest, true);
	}
	
	// make sure to clear previousFrame before blitting
	previousFrame->Bind();
	Renderer::Viewport(Vector2i(previousFrame->width, previousFrame->height));
	Renderer::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer::Blit(*src, *previousFrame, true);
	previousCamMatrix = camMatrix;
}

void TemporalReprojection::Awake()
{
	// initialize stuff
	Vector2i winSize = Renderer::GetWinSize();
	previousFrame = new RenderTexture(winSize.x, winSize.y, Texture::Format::Depth, Texture::Format::RGB);
	previousFrame->GenerateBuffers();

	reprojectionShader = new Shader("res/shaders/TemporalReprojection.shader");
	reprojectionMaterial = new Material(*reprojectionShader);
}
