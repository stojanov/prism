#include "ILayer.h"



namespace Prism
{
	ILayer::ILayer(Core::SharedContextRef ctx, const std::string& name)
		:
		m_Name(name),
		m_Ctx(ctx)
	{
	}

	ILayer::~ILayer() = default;
}
