#pragma once
#include <cstdint>

#include "prism/Core/Core.h"
#include "Buffer.h"
#include <glad/glad.h>


namespace Prism::Gl
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const BufferLayout& layout);
		VertexBuffer(void* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		virtual ~VertexBuffer();

		static Ref<VertexBuffer> Create(const BufferLayout& layout);
		static Ref<VertexBuffer> Create(void* data, size_t size, const BufferLayout& layout, const bool dynamic = true);
		
		const BufferLayout& GetLayout() const { return m_Layout; };

		void SetData(void* data, size_t size) const;
		void UpdateSubData(void* data, size_t offset, size_t size) const;

		template<typename T>
		void SetData(std::vector<T>& vertices, size_t count)
		{
			if (!m_Dynamic) return;
			Bind();
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), &vertices[0], GL_DYNAMIC_DRAW);
		}
		
		void Bind() const;
		void Unbind() const;

		void Clear();
	private:
		void CreateBuffer();
		bool m_Dynamic{ false };
		GLenum m_GlBufferType{ GL_DYNAMIC_DRAW };
		unsigned m_BufferID;
		BufferLayout m_Layout;
	};
}
