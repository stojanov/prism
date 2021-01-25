#pragma once

#include "Pointers.h"
#include "SystemEventManager.h"
#include "SystemController.h"
#include "Window.h"

namespace Prism::Core
{

	// Shared data context between layers
	struct SharedContext
	{
		Ref<Window> Window;
		Ref<SystemController> SystemController;
		// Asset Manager
		// Graphics/Renderer API
	};

	using SharedContextRef = Ref<SharedContext>;
	
	inline SharedContextRef CreateSharedContext(Ref<Window> win)
	{
		auto ctx = MakeRef<SharedContext>();

		ctx->Window = win;
		ctx->SystemController = MakeRef<SystemController>(win);
		
		return ctx;
	}
	
}
