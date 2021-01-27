#pragma once

#include "Pointers.h"
#include "SystemEventManager.h"
#include "RenderOptions.h"
#include "SystemOptions.h"
#include "Window.h"
#include "AssetLoader.h"
#include "Assets.h"
#include "prism/Renderer/Texture.h"

namespace Prism::Core
{
	struct GroupAssets
	{
		Ref<TextureAssets>	Textures;
		Ref<ShaderAssets>	Shaders;
	};
	
	// Shared data context between layers
	struct SharedContext
	{
		Ref<Window>			Window;
		Ref<SystemOptions>	SystemOptions;
		Ref<RenderOptions>	RenderOptions;
		GroupAssets			Assets;
		// Asset Manager
		// Graphics/Renderer API
	};

	using SharedContextRef = Ref<SharedContext>;
	
	inline SharedContextRef CreateSharedContext(Ref<Window> win)
	{
		auto ctx = MakeRef<SharedContext>();

		ctx->Window	= win;
		ctx->SystemOptions = MakeRef<SystemOptions>(win);
		ctx->RenderOptions = MakeRef<RenderOptions>(); // Ref as to keep consistency in the api
		ctx->Assets.Textures = MakeRef<TextureAssets>("Textures");
		ctx->Assets.Shaders = MakeRef<ShaderAssets>("Shaders");
		
		return ctx;
	}	
}
