#pragma once

#include "prism/Interfaces/ILayer.h"
#include "TerrainChunk.h"
#include "prism/Renderer/PerspectiveCamera.h"


class ProcGenTerrain: public Prism::ILayer
{
public:
	static constexpr Prism::Renderer::Vertex2i TerrainSize = { 100, 100 };

	ProcGenTerrain(Prism::Core::SharedContextRef ctx, const std::string& name);
	virtual ~ProcGenTerrain();

	void OnAttach() override;
	void OnDetach() override;
	void OnDraw() override;
	void OnSystemEvent(Prism::Event& e) override;
	void OnGuiDraw() override;
	void OnUpdate(float dt) override;
private:
	bool m_ShowInfo{ true };
	bool m_ShowGeneration{ false };
	bool m_ShowShader{ false };

	float m_CameraMoveSpeed{ 35.f };
	float m_CameraRotationSpeed{ 0.3f };
	bool m_CameraLocked{ true };

	TerrainChunk m_Terrain{ TerrainSize.x, TerrainSize.y};
	Prism::Ref<Prism::Gl::Shader> m_Shader{ nullptr };
	Prism::Renderer::PerspectiveCamera m_Camera{ 90, 1280, 720, 0.1f, 2048.f };
};
