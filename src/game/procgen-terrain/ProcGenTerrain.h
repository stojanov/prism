#pragma once

#include "prism/Interfaces/ILayer.h"
#include "TerrainChunk.h"
#include "prism/Math/PerlinNoise.h"
#include "prism/Renderer/PerspectiveCamera.h"


class ProcGenTerrain : public Prism::ILayer
{
public:
	static constexpr Prism::Renderer::Vertex2i TerrainSize = { 500, 500 };

	ProcGenTerrain(Prism::Core::SharedContextRef ctx, const std::string& name);
	virtual ~ProcGenTerrain();

	void OnAttach() override;
	void OnDetach() override;
	void OnDraw() override;
	void OnSystemEvent(Prism::Event& e) override;
	void OnGuiDraw() override;
	void OnUpdate(float dt) override;
private:
	float m_CameraMoveSpeed{ 50.f };
	float m_CameraRotationSpeed{ 0.3f };
	bool m_CameraLocked{ true };

	bool m_ShowInfo{ true };
	bool m_ShowCPUOptions{ false };
	bool m_ShowGPUOptions{ false };

	bool m_GPUUsingSimplex{ true };
	float m_GPUPersistance{ 0.5f };
	int m_GPUOctaves{ 7 };
	float m_GPUScale{ 0.1f };
	float m_GPUScaleMultiplier{ 1.f };
	glm::vec2 m_GPUOffset{ 0.f };
	float m_GPUElevation{ 25.f };

	Prism::Math::PerlinNoise m_Noise{ 9, 0.5 };
	bool m_RenderingGPU{ false };

	TerrainChunk m_TerrainCPU{ TerrainSize.x, TerrainSize.y};
	TerrainChunk m_TerrainGPU{ TerrainSize.x, TerrainSize.y };

	Prism::Ref<Prism::Gl::Shader> m_ShaderCPU{ nullptr };
	Prism::Ref<Prism::Gl::Shader> m_ShaderGPU{ nullptr };
	Prism::Renderer::PerspectiveCamera m_Camera{ 90, 1280, 720, 0.1f, 2048.f };
};
