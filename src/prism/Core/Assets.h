#pragma once
#include "AssetLoader.h"
#include "prism/GL/Shader.h"
#include "prism/Renderer/Texture.h"

namespace Prism
{
	struct TextureLoadArgs
	{
		std::string Filepath;
	};

	inline Core::AssetLoadResult<Renderer::Texture> TextureLoadFunction(const TextureLoadArgs& args)
	{
		return { Renderer::Texture::CreateRef(args.Filepath), false };
	}
	
	using TextureAssets = Core::AssetLoader<Renderer::Texture, TextureLoadArgs, TextureLoadFunction, true>;

	
	struct ShaderLoadArgs
	{
		std::string VertFilePath;
		std::string FragFilePath;
	};
	
	inline Core::AssetLoadResult<Gl::Shader> ShaderLoadFunction(const ShaderLoadArgs& args)
	{
		return { Gl::Shader::RefFromFiles(args.VertFilePath, args.FragFilePath), false };
	}
	
	using ShaderAssets = Core::AssetLoader<Gl::Shader, ShaderLoadArgs, ShaderLoadFunction, true>;
}
