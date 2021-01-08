#pragma once

#include "prism/Core/Core.h"
#include "Buffer.h"
#include "glad/glad.h"

namespace Prism::Gl
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count)
		{
			CreateBuffer();
			glBufferData(GL_ARRAY_BUFFER, count, indices, GL_DYNAMIC_DRAW);
		}

		IndexBuffer(std::vector<uint32_t> indices)
		{
			CreateBuffer();
			glBufferData(GL_ARRAY_BUFFER, indices.size(), &indices[0], GL_DYNAMIC_DRAW);
		}
		
		static Ref<IndexBuffer> CreateRef(uint32_t* indices, uint32_t count)
		{
			return MakeRef<IndexBuffer>(indices, count);
		}

		static Ptr<IndexBuffer> CreatePtr(uint32_t* indices, uint32_t count)
		{
			return MakePtr<IndexBuffer>(indices, count);
		}

		virtual ~IndexBuffer()
		{
			glDeleteBuffers(1, &m_BufferID);
		}

		void Bind() const override
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		}

		void Unbind() const override
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	private:
		void CreateBuffer()
		{
			glCreateBuffers(1, &m_BufferID);
			// Used GL_ARRAY_BUFFER so i can load data without an active VAO
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		}

		unsigned m_BufferID;
		uint32_t m_Count;
	};
}
