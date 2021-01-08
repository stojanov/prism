#pragma once

#include <string>
#include "glad/glad.h"

namespace Prism::Renderer
{
	class Texture
	{
	public:
		Texture();
		Texture(const std::string& path, bool flip = true);
		~Texture();

		void SetData(uint8_t* data, uint32_t size);
		
		void Bind(uint8_t slot);
	private:
		GLenum m_Format;
		unsigned m_TextureID;
		int m_Width;
		int m_Height;
		int m_ChannelCount;
	};
}