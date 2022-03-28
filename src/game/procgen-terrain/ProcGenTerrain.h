#pragma once

#include "prism/Interfaces/ILayer.h"
#include "TerrainChunk.h"


class ProcGenTerrain: public Prism::ILayer
{
public:
	ProcGenTerrain(Prism::Core::SharedContextRef ctx, const std::string& name);
	virtual ~ProcGenTerrain();

	void OnAttach() override;
	void OnDetach() override;
	void OnDraw() override;
	void OnSystemEvent(Prism::Event& e) override;
	void OnGuiDraw() override;
	void OnUpdate(float dt) override;
private:

};