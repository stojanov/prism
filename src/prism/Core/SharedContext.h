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
		Ref<Window>					window;
		Ref<SystemOptions>			systemOptions;
		Ref<RenderOptions>			renderOptions;
		Ref<BackgroundTasks>		tasks;
		GroupAssets					assets;
		// Graphics/Renderer API
	};

	using SharedContextRef = Ref<SharedContext>;
	
	inline SharedContextRef CreateSharedContext(Ref<Window> win)
	{
		auto ctx = MakeRef<SharedContext>();
		auto glLoadingContextPtr = win->GetLoadingContext();
		ctx->window	= win;
		ctx->systemOptions = MakeRef<SystemOptions>(win);
		ctx->renderOptions = MakeRef<RenderOptions>(); // Ref as to keep consistency in the api
		
		ctx->tasks = MakeRef<BackgroundTasks>();
		
		ctx->tasks->RegisterWorker(SHARECTX_TASKNAME, 1, [glLoadingContextPtr]
		{
			glfwMakeContextCurrent(glLoadingContextPtr);
			PR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Couldn't initliaize glad for loading context");
		}, []
		{
			glFinish();
		});

        // Possibly move this kind of information into a new module
        auto threads = std::thread::hardware_concurrency();
        auto using_threads = threads / 1.5;
        PR_CORE_INFO("Using {} threads", using_threads);

		ctx->Tasks->RegisterWorker("bg", using_threads);
		
		ctx->assets.Textures = MakeRef<TextureAssets>("Textures", ctx->tasks->GetWorker(SHARECTX_TASKNAME));
		ctx->assets.Shaders = MakeRef<ShaderAssets>("Shaders", ctx->tasks->GetWorker(SHARECTX_TASKNAME));
		
		return ctx;
	}	
}
