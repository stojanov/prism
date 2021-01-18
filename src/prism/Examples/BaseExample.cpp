#include "BaseExample.h"

#include <vector>

#include "prism/Components/Camera/CameraEditorController.h"
#include "prism/Components/Camera/FPSCameraController.h"

std::vector<float> texturedCube = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  -1.0f,
		0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  -1.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  -1.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  -1.0f,
		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  -1.0f,

		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

		-0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,
		-0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  -1.0f, 0.0f,
		0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
		0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  -1.0f, 0.0f,

		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,   0.0f,  1.0f,  0.0f
};

namespace Prism::Examples
{
	std::string Base::s_VertFilename = "res/shader.vert";
	std::string Base::s_FragFilename = "res/shader.frag";
	std::string Base::s_TextureFilename = "res/cube.jpg";
	
	Base::Base() = default;

	void Base::OnAttach()
	{
		// Hardcoded for now
		m_Shader = Gl::Shader::RefFromFiles(s_VertFilename, s_FragFilename);
		m_Texture = Renderer::Texture(s_TextureFilename);

		//m_Camera.AttachController<Renderer::CameraEditorController<Renderer::PerspectiveCamera>>();

		m_Camera.AttachController<Renderer::FPSCameraController<Renderer::PerspectiveCamera>>();
		
		m_Cube.AddVertexData(texturedCube);
		m_Cube.FlushVertexData();

		m_Texture.Bind(0);
		m_Shader->Bind();
		m_Shader->SetInt("tex", 0);
	}

	void Base::OnDetach()
	{
		
	}

	void Base::OnSystemEvent(Event& e)
	{
		m_Camera.OnSystemEvent(e);
	}

	void Base::OnUpdate(float dt)
	{
		m_Camera.OnUpdate(dt);
	}

	void Base::OnGuiDraw()
	{
		// GuiStartFrame();
	}
	
	void Base::OnDraw()
	{
		//GuiDraw();
		
		m_Shader->Bind();
		m_Shader->SetMat4("transform", m_CubeTransform);
		m_Shader->SetFloat3("lightPos", m_LightPosition);
		m_Shader->SetFloat("lightIntens", m_LightIntensity);
		m_Shader->SetFloat3("lightClr", m_LightClr);
		m_Shader->SetMat4("projectedview", m_Camera.GetProjectedView());
		m_Cube.DrawArrays();
	}
}
