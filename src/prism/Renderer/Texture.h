#pragma once

#include <string>
#include "glad/glad.h"
#include "prism/Core/Pointers.h"
#include "vec2.hpp"

namespace Prism::Renderer
{
	class Texture
	{
	public:
		Texture();
		Texture(const std::string& path, bool flip = true);
		~Texture();

		void SetData(uint8_t* data, uint32_t size);
		static Ref<Texture> CreateRef(const std::string& path, bool flip = true);
		void Bind(uint8_t slot);

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		const glm::vec2& GetSize() { return { m_Width, m_Height }; }

		static int GetTexSlot(uint8_t slot)
		{
			static constexpr int slots[] = {
				GL_TEXTURE0,
				GL_TEXTURE1,
				GL_TEXTURE2,
				GL_TEXTURE3,
				GL_TEXTURE4,
				GL_TEXTURE5,
				GL_TEXTURE6,
				GL_TEXTURE7,
				GL_TEXTURE8,
				GL_TEXTURE9,
				GL_TEXTURE10,
				GL_TEXTURE11,
				GL_TEXTURE12,
				GL_TEXTURE13,
				GL_TEXTURE14,
				GL_TEXTURE15,
				GL_TEXTURE16,
				GL_TEXTURE17,
				GL_TEXTURE18,
				GL_TEXTURE19,
				GL_TEXTURE20,
				GL_TEXTURE31,
				GL_TEXTURE22,
				GL_TEXTURE23,
				GL_TEXTURE24,
				GL_TEXTURE25,
				GL_TEXTURE26,
				GL_TEXTURE27,
				GL_TEXTURE28,
				GL_TEXTURE29,
				GL_TEXTURE30,
				GL_TEXTURE31,
			};

			PR_ASSERT(slot < 32, "Texture slot doesn't exist");
			
			return slots[slot];
		}
	private:
		GLenum m_Format;
		unsigned m_TextureID;
		int m_Width;
		int m_Height;
		int m_ChannelCount;
	};
}
