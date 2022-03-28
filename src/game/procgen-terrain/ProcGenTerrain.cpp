#include "ProcGenTerrain.h"

ProcGenTerrain::ProcGenTerrain(Prism::Core::SharedContextRef ctx, const std::string& name)
	:
	ILayer(ctx, name)
{
	
}

ProcGenTerrain::~ProcGenTerrain()
{

}

void ProcGenTerrain::OnAttach()
{
	
}

void ProcGenTerrain::OnDetach()
{
	
}

void ProcGenTerrain::OnSystemEvent(Prism::Event& e)
{
	
}

void ProcGenTerrain::OnUpdate(float dt)
{
	
}


void ProcGenTerrain::OnGuiDraw()
{
	auto& io = ImGui::GetIO();

	bool sel = false;
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("Info", 0, &m_ShowInfo);
	ImGui::MenuItem("Shader", 0, &m_ShowShader);
	ImGui::EndMenuBar();

	if (m_ShowInfo)
	{
		ImGui::Begin("Options");
		ImGui::Text("Use (WASD) to move around the world");
		ImGui::Text("Use (F2) to active the camera move");
		ImGui::Text("Toggle Wireframe = F1");
		ImGui::Text("Toggle Camera = F2");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (m_ShowShader)
	{
		ImGui::Begin("Shader Options");

		ImGui::End();
	}
}

void ProcGenTerrain::OnDraw()
{
	
}




