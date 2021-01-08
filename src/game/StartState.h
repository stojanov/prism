#pragma once
#include "prism/Core/Layer.h"

class StartState: public Prism::Core::Layer
{
public:
	StartState();
	
	void OnAttach() override;
	void OnDetach() override;
	void OnDraw() override;
	void OnUpdate() override;
	void OnSystemEvent(Prism::Event& e) override;
private:
	
};
