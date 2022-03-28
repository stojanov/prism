#pragma once

#include "prism/Interfaces/ILayer.h"
#include "TerrainChunk.h"


class ProcGenTerrain: public Prism::ILayer
{
public:
	static constexpr glm::vec2 TerrainSize = { 1000, 1000 };

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

	TerrainChunk m_Terrain{ TerrainSize.x, TerrainSize.y};
};