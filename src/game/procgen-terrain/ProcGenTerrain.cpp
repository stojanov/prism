#include "ProcGenTerrain.h"

#include "ext/scalar_constants.hpp"
#include "prism/Core/Events/KeyEvents.h"
#include "prism/Core/Events/WindowEvents.h"
#include "prism/Interfaces/Camera/FPSCameraController.h"

using namespace Prism;

ProcGenTerrain::ProcGenTerrain(Core::SharedContextRef ctx, const std::string& name)
	:
	ILayer(ctx, name)
{
	
}

ProcGenTerrain::~ProcGenTerrain()
{

}

void ProcGenTerrain::OnAttach()
{
	if (!m_Ctx->Assets.Shaders->LoadAsset("procgen-terrain", 
		{ "res/procgen-terrain.vert", "res/procgen-terrain.frag"}
	))
	{
		m_Shader = m_Ctx->Assets.Shaders->Get("procgen-terrain");
		PR_INFO("Shader Loaded");
	}

	{
		const auto& ctrl = m_Camera.AttachController<Renderer::FPSCameraController<Renderer::PerspectiveCamera>>();
		ctrl->SetMoveSpeed(m_CameraMoveSpeed);
		ctrl->SetRotationSpeed(m_CameraRotationSpeed);
	}

	m_Camera.SetPosition({ 0.f, 3.f, 0.f });
	m_Camera.SetLookAt({ 0.f, 0.f, 0.f });

	float height = 25.f;
	// sin(10(x^2+y^2))/10 
	m_Terrain.SetHeightFunc([height](int x, int y, int w, int h)
		{
			static constexpr float pi = glm::pi<float>();

			x -= w / 2;
			y -= h / 2;

			float dX = (x * 1.f / w) * pi;
			float dY = (y * 1.f / h) * pi;

			float z = (glm::sin(10 * ((dX * dX) + (dY * dY))) / 10) * height;

			return z;
		});

	m_Terrain.BakeMap();
	m_Terrain.UpdateMesh(false);
}

void ProcGenTerrain::OnDetach()
{
	
}

void ProcGenTerrain::OnSystemEvent(Event& e)
{
	EventHandler handler(e);

	m_Camera.OnSystemEvent(e);

	handler.Handle<WindowResizeEvent>([this](WindowResizeEvent& evt)
	{
		m_Camera.UpdatePerspective(90, evt.GetWidth(), evt.GetHeight(), 0.1f, 2048.f);
	});

	handler.Handle<KeyPressedEvent>([this] (KeyPressedEvent& evt)
	{
		switch (evt.GetKey())
		{
		case Keyboard::F2:
			if (m_CameraLocked)
			{
				m_Ctx->SystemOptions->DisableCursor();
			}
			else
			{
				m_Ctx->SystemOptions->EnableCursor();
			}
			m_CameraLocked = !m_CameraLocked;
			break;

		}
	});
}

void ProcGenTerrain::OnUpdate(float dt)
{
	m_Camera.ShouldLock(m_CameraLocked);
	m_Camera.OnUpdate(dt);
}


void ProcGenTerrain::OnGuiDraw()
{
	auto& io = ImGui::GetIO();

	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("Info", 0, &m_ShowInfo);
	ImGui::MenuItem("Shader", 0, &m_ShowShader);
	ImGui::EndMainMenuBar();

	if (m_ShowInfo)
	{
		ImGui::Begin("Options");
		ImGui::Text("Use (WASD) to move around the world");
		ImGui::Text("Use (F2) to active the camera move");
		ImGui::Text("Toggle Wireframe = F1");
		ImGui::Text("Toggle Camera = F2");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		auto& pos = m_Camera.GetPosition();
		auto& rot = m_Camera.GetRotation();
		ImGui::Text("Camera Position: (%.2f %.2f %.2f)", pos.x, pos.y, pos.z);
		ImGui::Text("Camera Rotaton: (%.2f %.2f)", rot.x, rot.y);

		ImGui::End();
	}

	if (m_ShowShader)
	{
		ImGui::Begin("Shader Options");
		ImGui::Text("Use (WASD) to move around the world");
		ImGui::Text("Use (F2) to active the camera move");
		ImGui::End();
	}

	
}

void ProcGenTerrain::OnDraw()
{
	m_Shader->Bind();
	m_Shader->SetFloat("height", 25.f);
	m_Shader->SetMat4("transform", m_Terrain.GetTransform());
	m_Shader->SetMat4("projectedView", m_Camera.GetProjectedView());
	m_Terrain.Render();
}


