#include "TextureAtlas.h"

#include "prism/Math/Interpolation.h"

namespace Prism::Renderer
{
	TextureAtlas::TextureAtlas()
	{
		m_Texture = MakeRef<Texture>();
	}


	TextureAtlas::TextureAtlas(const std::string& path)
	{
		m_Texture = Texture::CreateRef(path);
		m_Width = m_Texture->GetWidth();
		m_Height = m_Texture->GetHeight();
	}

	TextureAtlas::TextureAtlas(Ref<Texture> texture)
	{
		m_Texture = texture;
		m_Width = m_Texture->GetWidth();
		m_Height = m_Texture->GetHeight();
	}

	void TextureAtlas::Bind(uint8_t slot)
	{
		m_Texture->Bind(slot);
	}

	void TextureAtlas::SetInstanceSize(const glm::vec2& size)
	{
		m_InstSize = size;
	}

	std::array<float, 8> TextureAtlas::GetTexture(int x, int y)
	{
		float xStart = x * m_InstSize.x;
		float xEnd = xStart + m_InstSize.x;
		float yStart = m_Height - (y + 1) * m_InstSize.y;
		float yEnd = m_Height - y * m_InstSize.y;

		float xVertStart = Math::LinearTranslate(0.f, m_Width * 1.f, 0.f, 1.f, xStart);
		float xVertEnd = Math::LinearTranslate(0.f, m_Width * 1.f, 0.f, 1.f, xEnd);

		float yVertStart = Math::LinearTranslate(0.f, m_Height * 1.f, 0.f, 1.f, yStart);
		float yVertEnd = Math::LinearTranslate(0.f, m_Height * 1.f, 0.f, 1.f, yEnd);

		return {
			xVertStart, yVertStart,	// TOP LEFT
			xVertStart, yVertEnd,	// BOT LEFT
			xVertEnd, yVertEnd,		// BOT RIGHT
			xVertEnd, yVertStart	// TOP RIGHT
		};
	}

	std::array<glm::vec2, 4> TextureAtlas::GetTextureVec(int x, int y) const
	{
		float xStart = x * m_InstSize.x;
		float xEnd = xStart + m_InstSize.x;
		float yStart = m_Height - ((y + 1) * m_InstSize.y);
		float yEnd = m_Height - (y * m_InstSize.y);

		float xVertStart = glm::clamp(xStart / m_Width, 0.f, 1.f);
		float xVertEnd = glm::clamp(xEnd / m_Width, 0.f, 1.f);

		float yVertStart = glm::clamp(yStart / m_Height, 0.f, 1.f);
		float yVertEnd = glm::clamp(yEnd / m_Height, 0.f, 1.f);

		return {
			glm::vec2{ xVertStart, yVertStart },	// TOP LEFT
			glm::vec2{ xVertStart, yVertEnd	},		// BOT LEFT
			glm::vec2{ xVertEnd, yVertEnd },		// BOT RIGHT
			glm::vec2{ xVertEnd, yVertStart },		// TOP RIGHT
		};
	}
}