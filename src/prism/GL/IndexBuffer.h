#pragma once

#include "prism/Core/Core.h"
#include "Buffer.h"
#include "glad/glad.h"

namespace Prism::Gl
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer();
		IndexBuffer(uint32_t* indices, uint32_t count);
		IndexBuffer(std::vector<uint32_t>& indices);
		
		virtual ~IndexBuffer();
		
		static Ref<IndexBuffer> CreateRef(uint32_t* indices, uint32_t count);
		static Ptr<IndexBuffer> CreatePtr(uint32_t* indices, uint32_t count);
		static Ref<IndexBuffer> CreateRef();
		static Ptr<IndexBuffer> CreatePtr();

		void SetData(uint32_t* indices, uint32_t count);
		void SetData(std::vector<uint32_t>& indices);
		
		void Bind() const override;
		void Unbind() const override;
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
