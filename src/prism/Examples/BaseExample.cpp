#include "BaseExample.h"

#include <vector>

#include "imgui.h"
#include "prism/Components/Camera/CameraEditorController.h"
#include "prism/Components/Camera/FPSCameraController.h"

#include "glm/ext/matrix_transform.hpp"

std::vector<float> TexturedCubeData = {
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

std::vector<float> PlaneData = {
		-0.5f,   0.5f,  0.f,	0.f, 1.f,
		 0.5f,   0.5f,  0.f,	1.f, 1.f,
		-0.5f,  -0.5f,  0.f,    0.f, 0.f,
		 0.5,	-0.5,   0.f,    1.f, 0.f,
};

namespace Prism::Examples
{
	std::string Base::s_CubeTextureFilename		= "res/cube.jpg";
	std::string Base::s_PlaneTextureFilename	= "res/cube.jpg";
	
	std::string Base::s_CubeVertFilename		= "res/cube.vert";
	std::string Base::s_CubeFragFilename		= "res/cube.frag";

	std::string Base::s_PlaneVertFilename		= "res/plane.vert";
	std::string Base::s_PlaneFragFilename		= "res/plane.frag";
	
	Base::Base(Core::SharedContextRef ctx, const std::string& name)
		: ILayer(ctx, name)
	{
	}

	Base::~Base()
	{
		
	}

	void Base::OnAttach()
	{
		m_Ctx->Assets.Textures->QueueLoad("plane", { s_PlaneTextureFilename });
		m_Ctx->Assets.Shaders->QueueLoad("plane", { s_PlaneVertFilename, s_PlaneFragFilename });
		m_Ctx->Assets.Textures->QueueLoad("cube", { s_CubeTextureFilename });
		m_Ctx->Assets.Shaders->QueueLoad("cube", { s_CubeVertFilename, s_CubeFragFilename });

		m_Ctx->Assets.Textures->AsyncLoad();
		m_Ctx->Assets.Shaders->AsyncLoad();

		m_Ctx->Assets.Textures->Wait();
		m_Ctx->Assets.Shaders->Wait();
		
		m_Camera.SetPosition({ 0.f, 1.f, -4.f});
		m_Camera.SetLookAt({ 0.f, 0.f, 0.f });
		//m_Camera.AttachController<Renderer::CameraEditorController<Renderer::PerspectiveCamera>>();

		m_Camera.AttachController<Renderer::CameraEditorController<Renderer::PerspectiveCamera>>();
		
		m_Cube.AddVertexData(TexturedCubeData);


		auto buf = m_Plane.CreateNewVertexBuffer({
			{ Gl::ShaderDataType::Float3, "color"},
			{ Gl::ShaderDataType::Float3, "param"},
		});

		m_Plane.AddVertex(buf, glm::vec3{ 1.f, 0.f, 0.f });
		m_Plane.AddVertex(buf, glm::vec3{ 0.f, 1.f, 0.f });
		m_Plane.AddVertex(buf, glm::vec3{ 0.f,0.f, 1.f});
		
		m_Plane.AddVertex(buf, glm::vec3{ 1.f, 0.f, 0.f });
		m_Plane.AddVertex(buf, glm::vec3{ 0.f, 1.f, 0.f });
		m_Plane.AddVertex(buf, glm::vec3{ 0.f,0.f, 1.f });
		
		m_Cube.FlushVertexData();

		m_Plane.AddVertexData(PlaneData);
		m_Plane.ConnectVertices(0, 1, 2);
		m_Plane.ConnectVertices(2, 1, 3);
		m_Plane.Flush();

		{
			const auto& CubeShader = m_Ctx->Assets.Shaders->Get("cube");
			const auto& PlaneShader = m_Ctx->Assets.Shaders->Get("plane");

			CubeShader->Bind();
			CubeShader->SetInt("tex", 0);

			PlaneShader->Bind();
			PlaneShader->SetInt("tex", 1);
		}

		
		m_CubeTransform = glm::translate(m_CubeTransform, { 0.f, 0.5f, 0.f });
		m_PlaneTransform = glm::rotate(m_PlaneTransform, glm::radians(90.f), { 1.f, 0.f, 0.f });
		m_PlaneTransform = glm::translate(m_PlaneTransform, { 0.f, -0.5f, 0.f });
		m_PlaneTransform = glm::scale(m_PlaneTransform, { 5.f, 5.f, 5.f });
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
		m_Camera.ShouldLock(m_CursorOverGui);
		m_Camera.OnUpdate(dt);

		m_CubeTransform = glm::translate(m_CubeTransform, (m_CubePosImGui - m_LastCubePos));
		m_LastCubePos = m_CubePosImGui;
	}

	void Base::OnGuiDraw()
	{
		ImGui::Begin("Controls");
		auto io = ImGui::GetIO();
		m_CursorOverGui = io.WantCaptureMouse;

		ImGui::SliderFloat3("Cube Position", (float*) &m_CubePosImGui, -5, 5);
		ImGui::SliderFloat3("Light Position", (float*) &m_LightPosition, -5, 5);
		ImGui::ColorEdit3("Light Color", (float*) &m_LightClr);
		ImGui::SliderFloat("Light Intensity", &m_LightIntensity, 0, 5);
		
		ImGui::End();
	}
	
	void Base::OnDraw()
	{
		const auto& CubeShader = m_Ctx->Assets.Shaders->Get("cube");
		const auto& PlaneShader = m_Ctx->Assets.Shaders->Get("plane");
		
		m_Ctx->Assets.Textures->Get("cube")->Bind(0);
		CubeShader->Bind();
		CubeShader->SetInt("tex", 0);
		CubeShader->SetMat4("transform", m_CubeTransform);
		CubeShader->SetFloat3("lightPos", m_LightPosition);
		CubeShader->SetFloat("lightIntens", m_LightIntensity);
		CubeShader->SetFloat3("lightClr", m_LightClr);
		CubeShader->SetMat4("projectedview", m_Camera.GetProjectedView());
		m_Cube.DrawArrays();

		m_Ctx->Assets.Textures->Get("plane")->Bind(1);
		PlaneShader->Bind();
		PlaneShader->SetInt("tex", 1);
		PlaneShader->SetMat4("transform", m_PlaneTransform);
		PlaneShader->SetMat4("projectedview", m_Camera.GetProjectedView());
		m_Plane.DrawIndexed();
	}
}
