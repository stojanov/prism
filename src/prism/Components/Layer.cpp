#include "Layer.h"



namespace Prism
{
	Layer::Layer(Core::SharedContextRef ctx, const std::string& name)
		:
		m_Name(name),
		m_Ctx(ctx)
	{
	}

	Layer::~Layer() = default;
}
