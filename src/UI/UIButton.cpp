#include "UI.h"
#include <iostream>

namespace UI
{
	Button::Button(const Vector2i& size, const Vector2i& pixelPos)
	{
		for (int i = 0; i < 18; i++)
			buttonQuad[i] = 0.0f;
		this->size = size;
		this->SetPixelPosition(pixelPos);
	}

	Button::~Button()
	{
		delete buttonVB;
		delete buttonVA;
		delete buttonMat;
		delete buttonShader;
	}

	void Button::HandleMouseClick(int button, Vector2i location)
	{
		// if in bounds do stuff
		if (pixelPosition.x <= location.x && pixelPosition.x + size.x >= location.x && 
			pixelPosition.y <= location.y && pixelPosition.y + size.y >= location.y)
		{
			action(button);
		}
	}

	void Button::Draw()
	{
		if (!buttonSetup)
		{
			// setup blit stuff
			buttonVB = new VertexBuffer(buttonQuad, sizeof(float) * 18);
			buttonVA = new VertexArray();
			buttonVA->Construct();
			buttonVA->EnableAttribute(0);
			buttonVA->FormatAttribute(0, 3, GL_FLOAT, false, 0, 0);
			buttonVA->DisableAttribute(0);
			
			if (!buttonMatSetup)
			{
				// Create and compile our GLSL program from the shaders
				buttonShader = new Shader("res/shaders/Sprite.shader");
				buttonMat = new Material(*buttonShader);
				buttonMatSetup = true;
			}
			buttonSetup = true;
		}

		// Use the shader
		buttonMat->Bind();

		// 1rst attribute buffer : vertices
		buttonVB->Bind();
		buttonVA->Bind();
		buttonVA->EnableAttribute(0);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
		buttonVA->DisableAttribute(0);
	}

	void Button::OnTransform()
	{
		Vector2 ou = Vector2(size.x / pixelsPerUnit.x, size.y / pixelsPerUnit.y);
		
		// quad vertex buffero
		buttonQuad[0] = position.x;         buttonQuad[1] = position.y;
		buttonQuad[3] = position.x + ou.x;  buttonQuad[4] = position.y;
		buttonQuad[6] = position.x;         buttonQuad[7] = position.y + ou.y;
		buttonQuad[9] = position.x;         buttonQuad[10] = position.y + ou.y;
		buttonQuad[12] = position.x + ou.x; buttonQuad[13] = position.y;
		buttonQuad[15] = position.x + ou.x; buttonQuad[16] = position.y + ou.y;
	}

	void Button::SetAction(std::function<void(int)> action)
	{
		this->action = action;
	}

	bool Button::buttonMatSetup = false;
	Shader* Button::buttonShader = nullptr;
	Material* Button::buttonMat = nullptr;
}
