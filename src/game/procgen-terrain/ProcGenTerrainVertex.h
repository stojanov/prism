#pragma once

#include "prism/Interfaces/ILayer.h"
#include "TerrainChunk.h"
#include "prism/Math/PerlinNoise.h"
#include "prism/Renderer/PerspectiveCamera.h"


class ProcGenTerrainVertex : public Prism::ILayer
{
public:
	static constexpr Prism::Renderer::Vertex2i TerrainSize = { 500, 500 };

	ProcGenTerrainVertex(Prism::Core::SharedContextRef ctx, const std::string& name);
	virtual ~ProcGenTerrainVertex();

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

	bool m_ShaderOptions{ true };
	bool m_ShowInfo{ true };

	float m_Elevation{ 25.f };

	int m_NoiseLayerCount{ 1 };
	int m_MaxNoiseLayers{ 10 };

	int m_Modes[10];
	int m_UsingSimplex[10];
	float m_Scales[10];
	float m_ScaleMultipliers[10];
	float m_OffsetsX[10];
	float m_OffsetsY[10];


	TerrainChunk m_TerrainGPU{ TerrainSize.x, TerrainSize.y };

	Prism::Ref<Prism::Gl::Shader> m_Shader{ nullptr };
	Prism::Renderer::PerspectiveCamera m_Camera{ 90, 1280, 720, 0.1f, 2048.f };
};
