#pragma once

#include "Texture.h"

namespace Prism::Renderer
{
	// TODO: Needs complete rework
	class TextureAtlas
	{
	public:
		TextureAtlas();
		TextureAtlas(const std::string& path);
		TextureAtlas(Ref<Texture> texture);
		void SetInstanceSize(const glm::vec2& size);
		void Bind(uint8_t slot);
		std::array<float, 8> GetTexture(int x, int y);
		std::array<glm::vec2, 4> GetTextureVec(int x, int y) const;
	private:
		Ref<Texture> m_Texture;
		int m_Width{ 0 };
		int m_Height{ 0 };
		glm::vec2 m_InstSize{ 0.f, 0.f };
	};
}