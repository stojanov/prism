#pragma once

#include <iostream>

#include "Pointers.h"
#include "SystemEventManager.h"
#include "RenderOptions.h"
#include "SystemOptions.h"
#include "Window.h"
#include "Assets.h"
#include "BackgroundTasks.h"

namespace Prism::Core
{
	static const std::string SHARECTX_TASKNAME = "glcontext";
	
	struct GroupAssets
	{
		Ref<TextureAssets>	Textures;
		Ref<ShaderAssets>	Shaders;
	};
	
	// Shared data context between layers
	struct SharedContext
	{
		Ref<Window>					Window;
		Ref<SystemOptions>			SystemOptions;
		Ref<RenderOptions>			RenderOptions;
		Ref<BackgroundTasks>		Tasks;
		GroupAssets					Assets;
		// Asset Manager
		// Graphics/Renderer API
	};

	using SharedContextRef = Ref<SharedContext>;
	
	inline SharedContextRef CreateSharedContext(Ref<Window> win)
	{
		auto ctx = MakeRef<SharedContext>();
		auto glLoadingContextPtr = win->GetLoadingContext();
		ctx->Window	= win;
		ctx->SystemOptions = MakeRef<SystemOptions>(win);
		ctx->RenderOptions = MakeRef<RenderOptions>(); // Ref as to keep consistency in the api
		
		ctx->Tasks = MakeRef<BackgroundTasks>();
		
		ctx->Tasks->RegisterWorker(SHARECTX_TASKNAME, 1, [glLoadingContextPtr]
		{
			glfwMakeContextCurrent(glLoadingContextPtr);
			PR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Couldn't initliaize glad for loading context");
		}, []
		{
			glFinish();
		});

		ctx->Tasks->RegisterWorker("bg", 4);
		
		ctx->Assets.Textures = MakeRef<TextureAssets>("Textures", ctx->Tasks->GetWorker(SHARECTX_TASKNAME));
		ctx->Assets.Shaders = MakeRef<ShaderAssets>("Shaders", ctx->Tasks->GetWorker(SHARECTX_TASKNAME));
		
		return ctx;
	}	
}
